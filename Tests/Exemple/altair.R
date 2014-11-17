
#'---
#'title: false
#'author: false
#'date: false
#'output:
#'html_document:
#'css: style.css
#'---

#'<p class = "centered"><img src = "Altair.bmp" /></p>
#'<p class = "title">RH Exemple</p>
#'

#+ echo = FALSE, warning = TRUE, message = FALSE

# comportement global du programme

# Lorsque l'on n'a que une ou deux années, mettre étudier.variations à FALSE
# Lorsque l'on n'étudie pas une base Xémélios, mettre étudier.tests.statutaires à FALSE

library(compiler)
#write.csv(unique(Paie[Paie$delta > 500 & Paie$Montant > 500 & Paie$Type %chin% c("A", "I", "R", "AV", "A"), c("Type",  "Libellé", "Code"), with=F]), chemin("test.csv"))

options(warn = -1, verbose = FALSE, OutDec = ",")

encodage.code.source <- "ISO-8859-15"

# encodage :sous unix, les fichiers sources devraient être encodés en UTF-8 pour permettre une génération correcte des documents
#           sous Windows, en ISO-8859-1.
# Les bases peuvent être en encodage fixe, ici ISO-8859-1 pour des raisons de commodité Windows
# Pour convertir les fichiers, réencoder sous RStudio par "Save with encoding..." les trois fichiers source *.R


# dans cet ordre

try(setwd("Tests/Exemple"), silent = TRUE)

source("prologue.R", encoding = encodage.code.source)

compilerOptions <- setCompilerOptions(suppressAll = TRUE)
JITLevel <- enableJIT(1)

source(file.path(chemin.dossier, "bibliotheque.fonctions.paie.R"), encoding = encodage.code.source)

installer.paquets(knitr, digest, colorspace, ggplot2, assertthat, yaml, gtools, utils, data.table)
if (paralléliser) installer.paquets(parallel)

# + parallel, soSNOW (windows) ou doMC (unix))

# version parallélisée : à ce stade les tests ne sont pas concluant sur les applications de ddply
# toutefois à terme on pourrait couper les bases en 2 selon les lignes.

#cores <- parallel::detectCores()

# if (setOSWindows)
#   {
#     installer.paquets(doSNOW)
#     library(doSNOW)
# si utilisation de foreach :
#     registerDoSNOW(makeCluster(cores, type = "SOCK"))
#   } else {
#     installer.paquets(doMC)
#     library(doMC)
# si utilisation de foreach :
#     registerDoMC(cores = cores, type="SOCK")
#   }

#library(foreach)
library(knitr)
library(ggplot2)
library(assertthat)
library(gtools)
library(data.table)

if (séparateur.décimal.entrée != ".")
  stop("Pour les tables importées par data.table::fread, le séparateur décimal doit être '.'")

if (séparateur.décimal.entrée == séparateur.liste.entrée)
  stop("Le séparateur décimal en entrée doit être différent du séparateur de colonnes !")

if (séparateur.décimal.sortie == séparateur.liste.sortie)
  stop("Le séparateur décimal en sortie doit être différent du séparateur de colonnes !")



if (sauvegarder.bases.analyse) {
  for (path in c("Rémunérations", "Effectifs", "Réglementation"))
    dir.create(file.path(chemin.dossier.bases, path), recursive = TRUE)
}

if (sauvegarder.bases.origine)
  dir.create(file.path(chemin.dossier.bases, "Paiements"), recursive = TRUE)

# problème temporaire avec l'option fig.retina depuis fin mai 2014

knitr::opts_chunk$set(fig.width = 7.5, echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')

#'<p class = "centered"><b>Exercices `r paste(début.période.sous.revue, "à", fin.période.sous.revue)` </b></p>
#'<p class = "author">Fabrice Nicol</h1>
#'
#+ echo = FALSE
#'`r format(Sys.Date(), "%a %d %b %Y")`
#'


# Le format est jour/mois/année avec deux chiffres-séparateur-deux chiffres-séparateur-4 chiffres.
# Le séparateur peut être changé en un autre en modifiant le "/" dans date.format

# Cette section pourra être modifiée en entrée dans d'autres contextes
# Matricule, Codes

# Contrôle de cohérence
#  on vérifie que chaque code de paie est associé, dans le fichier des codes de paiement (par défaut, racinecodes.csv),
#  que à chaque code donné on a associé un et un seul type de rémunération ("INDEMNITAIRE", "TRAITEMENT", etc.)

# Pour le mode rapide, convertir les fichiers base en UTF-8 SANS BOM (par exemple, notepad++ après Excel)


fichier.personnels.existe <- file.exists(chemin(nom.fichier.personnels))

if (fichier.personnels.existe) {
  base.personnels.catégorie <- read.csv.skip(nom.fichier.personnels, séparateur.liste = séparateur.liste.entrée, séparateur.décimal = séparateur.décimal.entrée)
  message("Chargement du fichier des catégories statutaires des personnels.")
}

# Lignes de paie
# On peut lire jusqu'à 50 fichiers csv de lignes de paie qui seront générés au format :

# "chemin dossier + racine-Lignes de Paie-j.csv" où racine est un bref identifiant du type de contrôle (exemple : "c2a-", "PEV-", ...)

lignes.paie <- nom.fichier.paie %+% "-" %+% 1:50 %+% ".csv"
lignes.paie <- lignes.paie[file.exists(chemin(lignes.paie))]

# Bulletins de paie
# On peut lire jusqu'à 10 fichiers csv de bulletins de paie qui seront générés au format :

# "chemin dossier + racine-Bulletins de Paie-j.csv"

bulletins.paie <- nom.bulletin.paie %+% "-" %+% 1:10 %+% ".csv"
bulletins.paie <- bulletins.paie[file.exists(chemin(bulletins.paie))]
nom.table      <- nom.table[file.exists(chemin(nom.table))]

# Programme principal

# Bases

# Lignes de paie

# On ne retient que les bases ayant pour années au minimum début.période.sous.revue
# et au maximum fin.période.sous.revue, qui contiennent toutes les colonnes requises
# pour le contrôle

# Le mode rapide n'est disponible que avec des csv à séparateurs virgule
# Il permet d'économiser environ 8s par million de ligne lues sur une dizaine de champs

Import.Lignes.paie <- function()  {

  res <- NULL
  res2 <- NULL

  res <- try(Read.csv("Lignes.paie",
                      lignes.paie,
                      colClasses = lignes.paie.classes.input,
                      colNames = lignes.paie.input.fallback,
                      séparateur.liste = séparateur.liste.entrée,
                      séparateur.décimal = séparateur.décimal.entrée,
                      drop=1:3,
                      rapide = TRUE),
             silent = TRUE)

  if (inherits(res, 'try-error')) {
    res2 <- try(Read.csv("Lignes.paie",
                         lignes.paie,
                         colClasses = lignes.paie.classes.input.fallback,
                         colNames = lignes.paie.input.fallback,
                         séparateur.liste = séparateur.liste.entrée,
                         séparateur.décimal = séparateur.décimal.entrée,
                         rapide = TRUE),
                silent = TRUE)
  }

  c(res, res2)
}


importer.bases.via.xhl2csv <- function(base) {

  res <- try(Read.csv(base,
                      nom.table,
                      colClasses = colonnes.classes.input,
                      colNames =  colonnes.input,
                      séparateur.liste = séparateur.liste.entrée,
                      séparateur.décimal = séparateur.décimal.entrée,
                      convertir.encodage = (encodage.entrée.xhl2csv != "UTF-8"),
                      encodage = encodage.entrée.xhl2csv,
                      rapide = TRUE),
             silent = FALSE)

  if (inherits(res, 'try-error'))
    stop("Problème de lecture de la base de la table bulletins-lignes de Paie")

  if (!is.null(Paie)) {
    message("Chargement de la table bulletins-lignes de Paie.")
  } else {
    stop("Chargement de la table bulletins-lignes de paie en échec.")
  }
  

  message("Chargement direct des bulletins et lignes de paie")

}


importer.bases.via.xhl2csv("Paie")


if (éliminer.duplications) {
  avant.redressement <- nrow(Paie)
  Paie <- unique(Paie, by=NULL)
  après.redressement <- nrow(Paie)
  if (après.redressement != avant.redressement) {
    cat("Retraitement de la base : ")
    cat("Elimination de ", avant.redressement - après.redressement, " lignes dupliquées")
  }
}


# dans le cas où l'on ne lance le programme que pour certaines années, il préciser début.période sous revue et fin.période .sous.revue
# dans le fichier prologue.R. Sinon le programme travaille sur l'ensemble des années disponibles.

if (! extraire.années) {

    début.période.sous.revue <- min(Paie[[1]])
    fin.période.sous.revue   <- max(Paie[[1]])


} else {

    Paie <- Paie[Paie$Année >= début.période.sous.revue & Paie$Année <= fin.période.sous.revue, ]

}

période                 <- début.période.sous.revue:fin.période.sous.revue
durée.sous.revue        <- fin.période.sous.revue - début.période.sous.revue + 1


message("Contrôle des noms de colonne des bulletins de paie : normal.")

# Lors de la PREMIERE utilisation d'Altair, paramétrer générer.codes <- TRUE dans prologue.R
# pour générer les fichier des codes de paiement sous le dossier des bases (par défaut "Données").
# ce fichier est trier par ordre croissant des codes de paiement sur les trois premiers chiffres des codes
# des anomalies peuvent résiduellement apparaître avec des codes contenant des lettres, en général après
# le troisième chiffre du code.
# L'utilisateur devra alors renseigner la colonne étiquette.type.rémunération de ce fichier

if (générer.codes)   {
  source("générer.codes.R", encoding = encodage.code.source)
  générer.base.codes(Paie) 
}
  
  # Paie <- en raison du fonctionnement de knitr sinon inutile

Paie <- Paie[ , `:=`(delta = sum(Montant*(  Type == "I"
                                        | Type == "T"
                                        | Type == "S"
                                        | Type == "IR"
                                        | Type == "AC"
                                        | Type == "A"
                                        | Type == "R"
                                        | Type == "AV"),
                               na.rm=TRUE)
                            - Brut), by=c("Matricule", "Année", "Mois")]

Bulletins.paie <- unique(Paie[ , .(Matricule, Nom, Année, Mois, Temps.de.travail, Statut, Emploi, Brut, Net.à.Payer, Nir)], by = NULL)



Bulletins.paie <- Bulletins.paie[ ,   quotité   := ifelse(etp.égale.effectif | is.na(Temps.de.travail), 1,  Temps.de.travail / 100)]


Bulletins.paie <- Bulletins.paie[ ,   `:=`(Montant.net.eqtp  = ifelse(is.finite(a<-Net.à.Payer/quotité), a,  NA),
                                           Montant.brut.eqtp = ifelse(is.finite(a<-Brut/quotité), a,  NA))]
    
Bulletins.paie <- Bulletins.paie[ ,   `:=`(Statut.sortie   = Statut[length(Net.à.Payer)],
                                           nb.jours        = calcul.nb.jours.mois(Mois, Année[1]),
                                           nb.mois         = length(Mois),
                                           quotité.moyenne = round(mean.default(quotité, na.rm = TRUE), digits = 1)),
                                      key=c("Matricule", "Année")]

Bulletins.paie <- Bulletins.paie[ ,   `:=`(Montant.brut.annuel      = sum(Brut, na.rm=TRUE),
                                         Montant.brut.annuel.eqtp = sum(Montant.brut.eqtp * 365 / nb.jours, na.rm=TRUE),
                                         Montant.net.annuel.eqtp  = sum(Montant.net.eqtp * 365 / nb.jours, na.rm=TRUE),
                                         permanent                = nb.jours >= 365),
                                      key=c("Matricule", "Année")]

Bulletins.paie.réduit <- unique(Bulletins.paie[ , .(Matricule, Année, quotité.moyenne)], by = NULL)

Bulletins.paie.réduit <- Bulletins.paie.réduit[ , nb.années := length(Année), by="Matricule"]

indicatrice.quotité <- function(matricule, année)  Bulletins.paie.réduit[Matricule == matricule & Année == année, quotité.moyenne][1] ==  Bulletins.paie[Matricule == matricule & (Année == année - 1), quotité.moyenne][1]
                                                  

Bulletins.paie <- merge(Bulletins.paie, cbind(Bulletins.paie.réduit[, .(Matricule, Année, nb.années)], vind = mapply(indicatrice.quotité,
                                               Bulletins.paie.réduit[ , Matricule], 
                                               Bulletins.paie.réduit[ , Année],
                                               USE.NAMES = FALSE)),
                        by = c("Matricule", "Année"))

Paie <- merge(unique(Bulletins.paie[ , c("Matricule", 
                                  "Année",
                                  "Mois",
                                  "quotité",
                                  "Montant.net.eqtp",
                                  "Montant.brut.eqtp",
                                  "Montant.brut.annuel",
                                  "Montant.brut.annuel.eqtp",
                                  "Montant.net.annuel.eqtp",
                                  "Statut.sortie",
                                  "nb.jours",
                                  "nb.mois",
                                  "nb.années",
                                  "quotité.moyenne",
                                  "vind",
                                  "permanent"), with=FALSE], by=NULL),
              Paie, 
              by=c("Année", "Mois", "Matricule"))

if (enlever.quotités.na)    { 
  Paie <- Paie[! is.na(quotité)]
  Bulletins.paie <- Bulletins.paie[! is.na(quotité)]
}

if (enlever.quotités.nulles)  {
  Paie <- Paie[quotité != 0]
  Bulletins.paie <- Bulletins.paie[! is.na(quotité)]
}

matricules <- unique(Bulletins.paie[ ,
                                                 c("Année",
                                                   "Emploi",
                                                   "Nom",
                                                   "Matricule"), 
                                                 with=FALSE], by=NULL)

if (fichier.personnels.existe) {
  matricules <- merge(matricules, base.personnels.catégorie, by = clé.fusion, all=TRUE)
} else {
  Catégorie <- character(length = nrow(matricules))
  matricules <- cbind(matricules, Catégorie)
}

=======
if (enlever.quotités.na) {
  Paie           <- Paie[! is.na(quotité)]
  Bulletins.paie <- Bulletins.paie[! is.na(quotité)]
}

if (enlever.quotités.nulles) {
  Paie           <- Paie[quotité != 0]
  Bulletins.paie <- Bulletins.paie[quotité != 0]
}

matricules <- unique(Bulletins.paie[ ,
                                                 c("Année",
                                                   "Emploi",
                                                   "Nom",
                                                   "Matricule"), 
                                                 with=FALSE], by=NULL)

if (fichier.personnels.existe) {
  matricules <- merge(matricules, base.personnels.catégorie, by = clé.fusion, all=TRUE)
} else {
  Catégorie <- character(length = nrow(matricules))
  matricules <- cbind(matricules, Catégorie)
}

>>>>>>> 8c75bb228c17fc7d2bb753e29d97c4939982b238:Tests/Exemple/altair.R
matricules <- matricules[order(Matricule,
                                                         Année), ]


message("Bulletins de Paie retraités")


###########  Analyse des rémunérations : base globale ###################
#                                 c(clé.fusion, étiquette.année),

  Analyse.rémunérations <- Paie[ , .(Nir          = Nir[1],
                                     Montant.net.annuel.eqtp  = Montant.net.annuel.eqtp[1],
                                     Montant.brut.annuel = Montant.brut.annuel[1],
                                     Montant.brut.annuel.eqtp = Montant.brut.annuel.eqtp[1],
                                     Statut       = Statut[1],
                                     nb.jours     = nb.jours[1],
                                     permanent    = permanent[1],
                                     ind.quotité  = vind[1],
                                     Emploi       = Emploi[1],
                                     temps.complet = all(quotité == 1),
                                     Service      = Service[1],
                                     traitement.indiciaire   = sum(Montant[Type == "T"], na.rm = TRUE),
                                     sft          = sum(Montant[Type == "S"], na.rm = TRUE),
                                     indemnité.résidence = sum(Montant[Type == "IR"], na.rm = TRUE),
                                     indemnités   = sum(Montant[Type == "I"], na.rm = TRUE),
                                     rémunérations.diverses = sum(Montant[Type == "A"], na.rm = TRUE),
                                     autres.rémunérations   = sum(Montant[Type == "AC" | Type == "A" | Type == "AV"], na.rm = TRUE),
                                     rémunération.vacataire = sum(Montant[Type == "VAC"], na.rm = TRUE)),
                                by = c(clé.fusion, étiquette.année)]


Analyse.rémunérations <- Analyse.rémunérations[ , rémunération.indemnitaire.imposable := indemnités + sft + indemnité.résidence + rémunérations.diverses]
                                                 #Montant.brut.annuel - sft - indemnité.résidence - traitement.indiciaire

Analyse.rémunérations <- Analyse.rémunérations[ ,
                                               `:=`(rémunération.indemnitaire.imposable.eqtp = ifelse(is.finite(q <- Montant.brut.annuel.eqtp/Montant.brut.annuel), 
                                                                                                            q * rémunération.indemnitaire.imposable,
                                                                                                            NA),

                                                    total.lignes.paie =  traitement.indiciaire + sft + indemnité.résidence + indemnités + autres.rémunérations,

                                                    part.rémunération.indemnitaire =  ifelse(is.na(s <-  Montant.brut.annuel) | s == 0,
                                                                                                   NA,
                                                                                                  (rémunération.indemnitaire.imposable) / s * 100))]
Analyse.rémunérations <- Analyse.rémunérations[ , indemnités.élu := ifelse(Statut == "ELU", total.lignes.paie, 0)]

Analyse.rémunérations <- Analyse.rémunérations[! is.na(Montant.brut.annuel)]


if (length (Analyse.rémunérations$quotité[Analyse.rémunérations$quotité > 1]) > minimum.positif & comportement.strict ) 
  stop("Détection de quotités > 1", call. = FALSE)

message("Analyse des rémunérations réalisée.")

Analyse.variations.par.exercice <- Analyse.rémunérations[ , c(clé.fusion, étiquette.année,
                                                              "Montant.net.annuel.eqtp",
                                                              "Montant.brut.annuel.eqtp",
                                                              "rémunération.indemnitaire.imposable.eqtp",
                                                              "Statut",
                                                              "nb.jours",
                                                              "temps.complet",
                                                              "ind.quotité",
                                                              "permanent"), with=FALSE]

Analyse.variations.par.exercice <- Analyse.variations.par.exercice[ , indicatrice.année := bitwShiftL(1, Année - début.période.sous.revue) ]

Analyse.variations.synthèse <- Analyse.variations.par.exercice[ ,
                                                                  .(Nexercices = length(Année),
                                                                    statut = Statut[length(Année)],
                                                                    total.jours = sum(nb.jours, na.rm = TRUE),
                                                                    indicatrice.période = sum(indicatrice.année),
                                                                    Montant.net.annuel.eqtp.début  = Montant.net.annuel.eqtp[1],
                                                                    Montant.net.annuel.eqtp.sortie = Montant.net.annuel.eqtp[length(Année)],
                                                                    permanent = all(permanent),
                                                                    temps.complet = all(temps.complet),
                                                                    moyenne.rémunération.annuelle.sur.période =
                                                                         sum(Montant.net.annuel.eqtp, na.rm = TRUE)/length(Année[!is.na(Montant.net.annuel.eqtp) & Montant.net.annuel.eqtp > minimum.positif])),
                                                                by = clé.fusion]

Analyse.variations.synthèse <- Analyse.variations.synthèse[ ,  pris.en.compte := ! is.na(Montant.net.annuel.eqtp.début)
                                                                                 & ! is.na(Montant.net.annuel.eqtp.sortie)
                                                                                 & Montant.net.annuel.eqtp.début  > minimum.positif 
                                                                                 & Montant.net.annuel.eqtp.sortie > minimum.positif ]

Analyse.variations.synthèse <- Analyse.variations.synthèse[ ,  variation.rémunération := ifelse(pris.en.compte,
                                                                          (Montant.net.annuel.eqtp.sortie / Montant.net.annuel.eqtp.début - 1)*100,
                                                                           NA)]

Analyse.variations.synthèse <- Analyse.variations.synthèse[ ,                                            
                                         `:=`(variation.moyenne.rémunération = ifelse(pris.en.compte,
                                                                                 ((variation.rémunération/100 + 1)^(1 / (Nexercices - 1)) - 1) * 100,
                                                                                 NA),
    
                                              variation.rémunération.normalisée = ifelse(durée.sous.revue == Nexercices,
                                                                                    variation.rémunération,
                                                                                    NA))]


Analyse.variations.synthèse <- Analyse.variations.synthèse[ ,                                                                 
                                         `:=`(variation.moyenne.rémunération.normalisée = ifelse(!is.na(variation.rémunération.normalisée),
                                                                                            variation.moyenne.rémunération,
                                                                                            NA),
                                               
                                               plus.2.ans  = (total.jours  >= 730),  
                                               moins.2.ans = (total.jours < 730),
                                               moins.1.an  = (total.jours < 365),
                                               moins.six.mois = (total.jours < 183))]
                                               
# Note : sous environnement knitr/spin, data.table parvient mal à identifier les noms locaux,
# ce qui ne pose pas de problème en environnement standard. Il faut donc rajouter le préfixe de base dans ce cas.


temp2 <- Analyse.variations.synthèse[ , .(Matricule, indicatrice.période, pris.en.compte, Nexercices, plus.2.ans)]


Analyse.variations.par.exercice <- merge(Analyse.variations.par.exercice, temp2, by="Matricule")

rm(temp2)

Analyse.variations.par.exercice <- Analyse.variations.par.exercice[ , est.rmpp :=  Année != début.période.sous.revue  
                                                                                   & ! is.na(ind.quotité)
                                                                                   &  ind.quotité == TRUE
                                                                                   & bitwAnd(bitwShiftL(1, Année - 1 - début.période.sous.revue),
                                                                                             indicatrice.période) != 0]


Analyse.variations.synthèse.plus.2.ans  <- data.frame(NULL)
Analyse.variations.synthèse.moins.2.ans <- data.frame(NULL)

message("Analyse des variations réalisée.")

Bulletins.paie.nir.total.hors.élus <- unique(Bulletins.paie[Année == fin.période.sous.revue
                                                           & Mois == 12
                                                           & Statut != "ELU",
                                                           c(clé.fusion, "Nir"), with=FALSE], by = NULL)

Bulletins.paie.nir.fonctionnaires  <- unique(Bulletins.paie[Année == fin.période.sous.revue
                                                           & Mois  == 12
                                                           & (Statut == "TITULAIRE" |
                                                              Statut == "STAGIAIRE"),
                                                           c(clé.fusion, "Nir"), with=FALSE], by = NULL)

names(Bulletins.paie.nir.total.hors.élus) <- c(clé.fusion, "Nir")

# Age au 31 décembre de l'exercice dernier.exerciceal de la période sous revue
# ne pas oublier [ ,...] ici:

années.fonctionnaires   <- extraire.nir(Bulletins.paie.nir.fonctionnaires)

années.total.hors.élus  <- extraire.nir(Bulletins.paie.nir.total.hors.élus)

Bulletins.paie.nir.total.hors.élus.début <- unique(Bulletins.paie[Année == début.période.sous.revue
                                                                  & Mois == 12
                                                                  & Statut != "ELU",
                                                                  c(clé.fusion, "Nir"), with=FALSE], by = NULL)


Bulletins.paie.nir.fonctionnaires.début  <- unique(Bulletins.paie[Année == début.période.sous.revue
                                                                  & Mois  == 12
                                                                  & (Statut == "TITULAIRE" |
                                                                     Statut == "STAGIAIRE"),
                                                                  c(clé.fusion, "Nir"), with=FALSE], by = NULL)

names(Bulletins.paie.nir.total.hors.élus.début) <- c(clé.fusion, "Nir")

# Age au 31 décembre de l'exercice dernier.exerciceal de la période sous revue
# ne pas oublier [ ,...] ici:

années.fonctionnaires.début   <- extraire.nir(Bulletins.paie.nir.fonctionnaires.début)

années.total.hors.élus.début  <- extraire.nir(Bulletins.paie.nir.total.hors.élus.début)

message("Analyse démographique réalisée.")

if (!is.null(Paie) & !is.null(Analyse.rémunérations)
    & !is.null(Analyse.variations.synthèse) & !is.null(Analyse.variations.par.exercice))
  message("Statistiques de synthèse réalisées")

########### Démographie ########################

#'# 1. Statistiques de population
#'
#'### 1.1 Effectifs

liste.années <- as.character(période)

effectifs <- lapply(période,
                    function(x) {
                      A <- Bulletins.paie[Année == x, .(Matricule, Statut, permanent, quotité, nb.mois)]
                      E <- unique(A[ , .(Matricule, permanent)], by = NULL)
                      ETP <- unique(Bulletins.paie[Année == x, .(quotité, Matricule, Statut, Mois, nb.mois)], by = NULL)
                      F <- E[permanent == TRUE, ]
                      G <- unique(A[A$Statut == "TITULAIRE" | A$Statut == "STAGIAIRE", .(Matricule, permanent)], by = NULL)
                      H <- G[permanent == TRUE, ]
                      I <- unique(A[A$Statut == "ELU", .(Matricule, permanent)], by = NULL)
                      J <- I[permanent == TRUE, ]
                      c(nrow(E), nrow(F), nrow(G),
                        nrow(H), nrow(I), nrow(J),
                        ETP[Statut != "ELU" , sum(quotité/nb.mois, na.rm=TRUE)],
                        ETP[Statut != "ELU" , sum(quotité, na.rm=TRUE)] / 12,
                        ETP[Matricule %chin% Analyse.variations.par.exercice[est.rmpp == TRUE
                                                                             & Statut != "ELU"
                                                                             & Année == x,
                                                                               Matricule],
                            sum(quotité, na.rm=TRUE)] / 12,
                        ETP[Statut == "TITULAIRE" | Statut == "STAGIAIRE",
                              sum(quotité, na.rm=TRUE)] / 12,
                        ETP[Matricule %chin% Analyse.variations.par.exercice[permanent == TRUE
                                                                             & Statut == "TITULAIRE"
                                                                             & temps.complet == TRUE
                                                                             & Année == x,
                                                                             Matricule],
                            sum(quotité, na.rm=TRUE)] / 12)
                     })

for (i in 1:length(effectifs)) names(effectifs[[i]]) <- c("Effectifs", "Effectifs_12", "Effectifs_12_fonct",
                                                      "Effectifs_12_fonct", "Effectifs_élus", "Effectifs_12_élus",
                                                      "ETP", "ETPT", "ETPT_pp", 
                                                      "ETPT_fonct", "Tit_12_100")

effectifs.locale <- lapply(effectifs, function(x) formatC(x, big.mark = " ", format="f", digits=1, decimal.mark=","))

tableau.effectifs <- as.data.frame(effectifs.locale,
                                   row.names = c("Total effectifs", "  dont présents 12 mois", "  dont fonctionnaires",
                                                 "  dont fonct. présents 12 mois", "  dont élus", "  dont élus présents 12 mois",
                                                 "Total ETP/année (a)", "Total ETPT/année (b)", "Total ETPT/année personnes en place (c)",
                                                 "Total ETPT/année fonctionnaires", "Total ETPT/année titulaires à temps complet"))

names(tableau.effectifs) <- liste.années
names(effectifs) <- liste.années
#'  
#  
#'&nbsp;*Tableau `r incrément()`*   
#            
kable(tableau.effectifs, row.names = TRUE, align='c')
#'
#'**Nota:**   
#'*(a) ETP  : Equivalent temps plein = rémunération . quotité*  
#'*(b) ETPT : Equivalent temps plein travaillé = ETP . 12/nombre de mois travaillés dans l'année*  
#'*(c) Personnes en place : présentes en N et N-1 avec la même quotité*   
#'*Les cinq dernières lignes du tableau sont calculées en ne tenant pas compte des élus.   
#'      
#'   
#'   
#'*Effectifs pourvus mesurés par l'émission d'un bulletin de paie dans l'année.*     
#'   
#'[Lien vers la base des effectifs](Bases/Effectifs/tableau.effectifs.csv)
#'
#'
#'### 1.2 Pyramide des âges, personnels non élus

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    
Résumé(c("Âge des personnels <br>au 31/12/" %+% début.période.sous.revue,
         "Effectif"),
       années.total.hors.élus.début,
       extra = "length",
       align = 'c')
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Âge des personnels <br>au 31/12/" %+% fin.période.sous.revue,
         "Effectif"),
       années.total.hors.élus,
       extra = "length",
       align = 'c')

#'  
#'    


if (longueur.non.na(années.total.hors.élus) > 0)
  hist(années.total.hors.élus,
       xlab = "Âge au 31 décembre " %+% fin.période.sous.revue,
       xlim = c(18, 75),
       ylab = "Effectif",
       main = "Pyramide des âges",
       col = "blue",
       nclass = 50)

#'  
#'[Lien vers la base des âges](Bases/Effectifs/Bulletins.paie.nir.total.hors.élus.csv)  
#'  

#'
#'
#'### 1.3 Pyramide des âges, personnels fonctionnaires stagiaires et titulaires

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Âge des personnels <br>au 31/12/" %+% début.période.sous.revue,
         "Effectif"),
       années.fonctionnaires.début,
       extra = "length",
       align = 'c')

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Âge des personnels <br>au 31/12/" %+% fin.période.sous.revue,
         "Effectif"),
       années.fonctionnaires,
       extra = "length",
       align = 'c')

#'

if (longueur.non.na(années.fonctionnaires) > 0)
  hist(années.fonctionnaires,
       xlab = "Âge au 31 décembre " %+% fin.période.sous.revue,
       xlim = c(18,68),
       ylab = "Effectif",
       main = "Pyramide des âges des fonctionnaires",
       col = "navy",
       nclass = 50)

#'  
#'[Lien vers la base des âges](Bases/Effectifs/Bulletins.paie.nir.fonctionnaires.csv)  
#'  

#'### 1.4 Effectifs des personnels par durée de service
#'
#'**Personnels en fonction des exercices `r début.période.sous.revue` à `r fin.période.sous.revue` inclus :**
#'

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    



Tableau(c("Plus de 2 ans",
          "Moins de 2 ans",
          "Moins d'un an",
          "Moins de six mois"),
        sum(Analyse.variations.synthèse$plus.2.ans, na.rm=TRUE),
        sum(Analyse.variations.synthèse$moins.2.ans, na.rm=TRUE),
        sum(Analyse.variations.synthèse$moins.1.an, na.rm=TRUE),
        sum(Analyse.variations.synthèse$moins.six.mois, na.rm=TRUE))


#'

if (nrow(Analyse.variations.par.exercice) > 0)
  qplot(factor(Année),
        data = Analyse.variations.par.exercice,
        geom = "bar",
        fill = factor(!Analyse.variations.par.exercice$plus.2.ans),
        main = "Evolutions entre " %+% début.période.sous.revue %+% " et " %+% fin.période.sous.revue,
        xlab = étiquette.année,
        ylab = "Effectif",
        asp = 1.4)        +
  scale_fill_discrete(name = "Personnels en fonction",
                      breaks = c(TRUE, FALSE),
                      labels = c("Moins de deux ans", "Plus de deux ans"))



#'
#'**Effectifs**   
#'

effectifs.var <- lapply(période,
                        function(x) {

                          E <- unique(Analyse.variations.par.exercice[Année == x , .(Matricule, plus.2.ans)], by=NULL)
                          F <- E[plus.2.ans == TRUE]
                          tot <- nrow(E)
                          plus.2.ans <- nrow(F)
                          résultat <- c(plus.2.ans, tot - plus.2.ans, tot)
                          rm(E, F, tot, plus.2.ans)
                          résultat
                        })

effectifs.var.locale <- lapply(effectifs.var, function(x) formatC(x, big.mark = " ", format="f", digits=0, decimal.mark=","))

tableau.effectifs.var <- as.data.frame(effectifs.var.locale, row.names = c("Plus de deux ans", "Moins de deux ans", "Total"))

names(tableau.effectifs.var) <- liste.années
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

kable(tableau.effectifs.var, row.names = TRUE, align='c')
#'


#'
#'**Nota :**
#'*Personnels en place : ayant servi au moins deux années consécutives pendant la période.*     
#'*Plus/moins de deux ans : plus/mois de 730 jours sur la période sous revue.*   
#'

année <- début.période.sous.revue

colonnes.sélectionnées <- c("traitement.indiciaire",
                            "autres.rémunérations",
                            "rémunération.indemnitaire.imposable",
                            "rémunération.indemnitaire.imposable.eqtp",
                            "total.lignes.paie",
                            "Montant.brut.annuel",
                            "Montant.brut.annuel.eqtp",
                            "part.rémunération.indemnitaire",
                            clé.fusion)

# pour année début #

###########  Analyse des rémunérations  début de période###################
#  ATTENTION : les années doivent être SUCCESSIVES                        #


Analyse.rémunérations.premier.exercice <- Analyse.rémunérations[Analyse.rémunérations$Année == année]

if (fichier.personnels.existe)
{
  Analyse.rémunérations.premier.exercice <- merge(Analyse.rémunérations.premier.exercice,
                                                  base.personnels.catégorie,
                                                  by = clé.fusion, all = FALSE)

  colonnes.sélectionnées <- c(colonnes.sélectionnées, "Catégorie")
}


########### Analyse statique premier exercice ########################

#'# 2. Rémunérations brutes : analyse pour l'exercice `r année`    
#'    
#'## 2.1 Masse salariale brute de l'ensemble des agents     
#'     
#'  
masses.premier.personnels <- colSums(Analyse.rémunérations.premier.exercice[Statut != "ELU",
                                                                            .(Montant.brut.annuel,
                                                                           rémunération.indemnitaire.imposable,
                                                                           indemnités.élu,
                                                                           total.lignes.paie,
                                                                           autres.rémunérations)])
  
masses.premier.élus <- colSums(Analyse.rémunérations.premier.exercice[Statut == "ELU",
                                                                      .(Montant.brut.annuel,
                                                                        rémunération.indemnitaire.imposable,
                                                                        indemnités.élu,
                                                                        total.lignes.paie,
                                                                        autres.rémunérations)])

#'### Cumuls des rémunérations brutes pour l'exercice `r année`
#'  
#'*Personnels (hors élus)*     
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Brut annuel (bulletins)",
                    "Brut annuel (lignes), dont :",
                    "\\ \\ Primes :",
                    "\\ \\ Autres rémunérations",
                    "Part de primes en %"),
                  c(masses.premier.personnels["Montant.brut.annuel"],
                  masses.premier.personnels["total.lignes.paie"],
                  masses.premier.personnels["rémunération.indemnitaire.imposable"],
                  masses.premier.personnels["autres.rémunérations"],
                  masses.premier.personnels["rémunération.indemnitaire.imposable"]/masses.premier.personnels["Montant.brut.annuel"] * 100))

#'  
#'*Elus*    
#'  
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Brut annuel (bulletins)",
                    "Brut annuel (lignes), dont :",
                    "\\ \\ Primes :",
                    "\\ \\ Autres rémunérations"),
                  c(masses.premier.élus["Montant.brut.annuel"],
                    masses.premier.élus["total.lignes.paie"],
                    masses.premier.élus["rémunération.indemnitaire.imposable"],
                    masses.premier.élus["autres.rémunérations"]))

#'  
#'**Définitions :**
#'
#'  *Brut annuel (bulletins)*   : somme du champ *Brut*    
#'  *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
#'  *Primes*                    : indemnités sauf remboursements, certaines IJSS, indemnités d'élu le cas échéant, Supplément familial de traitement et Indemnité de résidence        
#'  *Indemnités d'élu*          : toutes rémunérations indemnitaires des élus    
#'  *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
#'  

#'**Tests de cohérence**
#'
#'Somme des rémunérations brutes versées aux personnels (non élus) :  
#'  
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Bulletins de paie ",
                    "Lignes de paie ",
                    "Différence "),
                  c(masses.premier.personnels["Montant.brut.annuel"],
                    masses.premier.personnels["total.lignes.paie"],
                    masses.premier.personnels["Montant.brut.annuel"] -
                    masses.premier.personnels["total.lignes.paie"]))

#'
#'à comparer aux soldes des comptes 641 et 648 du compte de gestion.
#'
#'Somme des rémunérations brutes versées (élus) :  
#'  
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Bulletins de paie ",
                    "Lignes de paie ",
                    "Différence "),
                  c(masses.premier.élus["Montant.brut.annuel"],
                    masses.premier.élus["total.lignes.paie"],
                    masses.premier.élus["Montant.brut.annuel"] -
                    masses.premier.élus["total.lignes.paie"]))

#'
#'Les résultats sont exprimés en euros.
#'
#'   
#'## 2.2 Masse salariale brute des fonctionnaires
#'
#'*Cette section concerne les personnels fonctionnaires titulaires et stagiaires*  
#'
filtre.fonctionnaire <- function (X) X[ !is.na(X)  & X > minimum.positif ]

AR <- Analyse.rémunérations.premier.exercice[Statut == "TITULAIRE" | Statut == "STAGIAIRE", colonnes.sélectionnées, with=FALSE]
attach(AR)
source("histogrammes.R", encoding = "UTF-8")
histogrammes()
detach(AR)
#'    
#'**Nota :**   
#'*EQTP : Equivalent temps plein = 12 . moyenne du ratio ratio rémunération / quotité*  
#'    
#'**Effectif : `r nrow(AR)`**
#'
#'**Tests de cohérence**

if (nrow(AR) > 0) {
  masses.premier <- colSums(AR[ ,.(Montant.brut.annuel, rémunération.indemnitaire.imposable, total.lignes.paie, autres.rémunérations)])
} else {
  masses.premier <- c(0,0) 
}

#'Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :
#'
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Brut annuel (bulletins)",
                    "Brut annuel (lignes), dont :",
                    "\\ \\ Primes :",
                    "\\ \\ Autres rémunérations",
                    "Part de primes en %"),
                  c(masses.premier["Montant.brut.annuel"],
                    masses.premier["total.lignes.paie"],
                    masses.premier["rémunération.indemnitaire.imposable"],
                    masses.premier["autres.rémunérations"],
                    masses.premier["rémunération.indemnitaire.imposable"]/masses.premier["Montant.brut.annuel"] * 100))

#'
#'**Définitions :**
#'
#'  *Brut annuel (bulletins)*   : somme du champ *Brut*   
#'  *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
#'  *Primes*                    : indemnités sauf remboursements, certaines IJSS, indemnités d'élu le cas échéant, Supplément familial de traitement et Indemnité de résidence        
#'  *Indemnités d'élus*         : toutes rémunérations indemnitaires des élus    
#'  *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
#'
#'**Tests de cohérence**
#'
#'Somme des rémunérations brutes versées aux personnels (non élus) :
#'
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Bulletins de paie ",
                    "Lignes de paie ",
                    "Différence "),
                  c(masses.premier["Montant.brut.annuel"],
                    masses.premier["total.lignes.paie"],
                    masses.premier["Montant.brut.annuel"] -
                    masses.premier["total.lignes.paie"]))


#'
#'A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.
#'
#'
#'**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année `r année`**     
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Traitement indiciaire",
         "Primes",
         "Autres rémunérations",
         "Effectif"),
       AR[ , .(traitement.indiciaire,
               rémunération.indemnitaire.imposable,
               autres.rémunérations)],
       extra = "length")

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Total lignes hors rappels",
         "Total brut",
         "SMPT brut en EQTP",
         "Part de la rém. indemnitaire",
         "Effectif"),
       AR[ , .(total.lignes.paie,
               Montant.brut.annuel,
               Montant.brut.annuel.eqtp,
               part.rémunération.indemnitaire)],
       extra = "length")
#'
#'**Catégorie A**
#'

ARA <- data.table::data.table(NULL)
ARB <- data.table::data.table(NULL)
ARC <- data.table::data.table(NULL)

if (fichier.personnels.existe) {
  ARA <- AR[Catégorie == 'A', ]
  #'  
  #'&nbsp;*Tableau `r incrément()`*   
  #'    
  
  Résumé(c("Traitement indiciaire",
           "Primes",
           "Autres rémunérations"),
         ARA[ , .(traitement.indiciaire,
                  rémunération.indemnitaire.imposable,
                  autres.rémunérations)])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe) {
  #'  
  #'&nbsp;*Tableau `r incrément()`*   
  #'    
  
  Résumé(c("Total rémunérations", 
           "Total rémunérations EQTP", 
           "Part de la rémunération indemnitaire"),
         ARA[ , .(Montant.brut.annuel,
                  Montant.brut.annuel.eqtp,
                  part.rémunération.indemnitaire)])
}

#'
#'**Effectif : `r nrow(ARA)`**  
#'
#'**Catégorie B**
#'

if (fichier.personnels.existe) {
  ARB <- AR[Catégorie == 'B', ]
  #'  
  #'&nbsp;*Tableau `r incrément()`*   
  #'    
  
  Résumé(c("Traitement indiciaire",
           "Primes",
           "Autres rémunérations"),
         ARB[ , .(traitement.indiciaire,
                  rémunération.indemnitaire.imposable,
                  autres.rémunérations)])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe) {
  #'  
  #'&nbsp;*Tableau `r incrément()`*   
  #'    
  
  Résumé(c("Total rémunérations",
           "Total rémunérations EQTP",
           "Part de la rémunération indemnitaire"),
         ARB[ , .(Montant.brut.annuel,
                  Montant.brut.annuel.eqtp,
                  part.rémunération.indemnitaire)])
}

#'
#'**Effectif : `r nrow(ARB)`**
#'
#'**Catégorie C**
#'


if (fichier.personnels.existe) {
  ARC <- AR[Catégorie == 'C', ]
  #'  
  #'&nbsp;*Tableau `r incrément()`*   
  #'    
  
  Résumé(c("Traitement indiciaire",
           "Primes",
           "Autres rémunérations"),
         ARC[ , .(traitement.indiciaire,
                  rémunération.indemnitaire.imposable,
                  autres.rémunérations)])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe) {
  #'  
  #'&nbsp;*Tableau `r incrément()`*   
  #'    
  
  Résumé(c("Total rémunérations",
           "Total rémunérations EQTP",
           "Part de la rémunération indemnitaire"),
         ARC[ , .(Montant.brut.annuel,
                  Montant.brut.annuel.eqtp,
                  part.rémunération.indemnitaire) ])
}

#'**Effectif : `r nrow(ARC)`**
#'
#'
#'######      <br>
#'
#'## 2.3 Contractuels, vacataires et stagiaires inclus   
#'   

attach(Analyse.rémunérations.premier.exercice, warn.conflicts=FALSE)
temp <- rémunération.indemnitaire.imposable.eqtp[Statut != "ELU"
                            & Statut != "TITULAIRE"
                            & Statut != "STAGIAIRE"
                            & rémunération.indemnitaire.imposable.eqtp > 1000] / 1000

if (length(temp > 0))
  hist(temp,
       xlab = "Rémunération indemnitaire brute imposable en milliers d'euros EQTP\n",
       ylab = "Effectif",
       xlim = c(0, 40),
       main = "Rémunération annuelle totale des contractuels en " %+% année,
       col = "red",
       nclass = 50)
#'   
#'**Nota :**
#'Ne sont retenues que les rémunérations supérieures à 1 000 k&euro;.
#'Les élus ne sont pas pris en compte.
#'   

temp <- positive(autres.rémunérations)

detach(Analyse.rémunérations.premier.exercice)

if (length(temp))
  hist(temp,
       xlab = "En euros :\n divers",
       ylab = "Effectif",
       xlim = c(0, 5000),
       main = "Autres rémunérations en " %+% année,
       nclass = 50,
       col = "grey")

#'

AR <- Analyse.rémunérations.premier.exercice[Statut != "ELU"
                                             &  Statut != "TITULAIRE"
                                             &  Statut != "STAGIAIRE",
                                             colonnes.sélectionnées, 
                                             with=FALSE]

#'   
#'**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année `r année`**     
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Primes",
         "Autres rémunérations",
         "Effectif"),
       AR[ , .(rémunération.indemnitaire.imposable,
               autres.rémunérations)],
       extra = "length")

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


Résumé(c("Total rémunérations",
         "Total rémunérations EQTP",
         "Effectif"),
       AR[ , .(Montant.brut.annuel, Montant.brut.annuel.eqtp)],
       extra = "length")
#'

nom.base.analyse <- "Analyse.rémunérations.premier.exercice"


# pour année fin #

année <- fin.période.sous.revue

###########  Analyse des rémunérations  fin de période###################
#  ATTENTION : les années doivent être SUCCESSIVES                      #


Analyse.rémunérations.dernier.exercice <- Analyse.rémunérations[Analyse.rémunérations$Année == année, ]

if (fichier.personnels.existe)
{
  Analyse.rémunérations.dernier.exercice <- merge(Analyse.rémunérations.dernier.exercice, base.personnels.catégorie, by = clé.fusion, all = FALSE)
}


########### Analyse statique dernier exercice ########################

#'# 3. Rémunérations brutes : analyse pour l'exercice `r année`    
#'   
#'## 3.1 Masse salariale brute de l'ensemble des agents     
#'   
  
masses.dernier.personnels <- colSums(Analyse.rémunérations.dernier.exercice[Statut != "ELU",
                                                                            .(Montant.brut.annuel,
                                                                              rémunération.indemnitaire.imposable,
                                                                              indemnités.élu,
                                                                              total.lignes.paie,
                                                                              autres.rémunérations)])

masses.dernier.élus <- colSums(Analyse.rémunérations.dernier.exercice[Statut == "ELU",
                                                                      .(Montant.brut.annuel,
                                                                        rémunération.indemnitaire.imposable,
                                                                        indemnités.élu,
                                                                        total.lignes.paie,
                                                                        autres.rémunérations)])

#'### Cumuls des rémunérations brutes pour l'exercice `r année`
#'  
#'*Personnels (hors élus)*     
#'
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Brut annuel (bulletins)",
                    "Brut annuel (lignes), dont :",
                    "\\ \\ Primes :",
                    "\\ \\ Autres rémunérations",
                    "Part de primes en %"),
                  c(masses.dernier.personnels["Montant.brut.annuel"],
                    masses.dernier.personnels["total.lignes.paie"],
                    masses.dernier.personnels["rémunération.indemnitaire.imposable"],
                    masses.dernier.personnels["autres.rémunérations"],
                    masses.dernier.personnels["rémunération.indemnitaire.imposable"]/masses.dernier.personnels["Montant.brut.annuel"] * 100))
                  
#'  
#'*Elus*    
#'  
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Brut annuel (bulletins)",
                    "Brut annuel (lignes), dont :",
                    "\\ \\ Primes :",
                    "\\ \\ Autres rémunérations"),
                  c(masses.dernier.élus["Montant.brut.annuel"],
                    masses.dernier.élus["total.lignes.paie"],
                    masses.dernier.élus["rémunération.indemnitaire.imposable"],
                    masses.dernier.élus["autres.rémunérations"]))

#'  
#'**Définitions :**
#'
#'  *Brut annuel (bulletins)*   : somme du champ *Brut*    
#'  *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :    
#'  *Primes*                    : indemnités sauf remboursements, certaines IJSS, indemnités d'élu le cas échéant, Supplément familial de traitement et Indemnité de résidence        
#'  *Indemnités d'élu*         : toutes rémunérations indemnitaires des élus    
#'  *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
#'  

#'**Tests de cohérence**
#'
#'Somme des rémunérations brutes versées aux personnels (non élus) :  
#'  
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Bulletins de paie ",
                    "Lignes de paie ",
                    "Différence "),
                  c(masses.dernier.personnels["Montant.brut.annuel"],
                    masses.dernier.personnels["total.lignes.paie"],
                    masses.dernier.personnels["Montant.brut.annuel"] -
                      masses.dernier.personnels["total.lignes.paie"]))

#'
#'à comparer aux soldes des comptes 641 et 648 du compte de gestion.
#'
#'Somme des rémunérations brutes versées (élus) :  
#'  
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Bulletins de paie ",
                    "Lignes de paie ",
                    "Différence "),
                  c(masses.dernier.élus["Montant.brut.annuel"],
                    masses.dernier.élus["total.lignes.paie"],
                    masses.dernier.élus["Montant.brut.annuel"] -
                      masses.dernier.élus["total.lignes.paie"]))

#'
#'Les résultats sont exprimés en euros.
#'

#'   
#'## 3.2 Fonctionnaires titulaires et stagiaires   
#'

filtre.fonctionnaire <- function (X) X[ !is.na(X) & X > minimum.positif ]

AR <- Analyse.rémunérations.dernier.exercice[Statut == "TITULAIRE" | Statut == "STAGIAIRE", colonnes.sélectionnées, with=FALSE]
attach(AR)
source("histogrammes.R", encoding = "UTF-8")
histogrammes()
detach(AR)
#'    
#'**Nota :**   
#'*EQTP = Equivalent temps plein = 12 . moyenne du ratio rémunération / quotité*   
#'   
#'**Effectif : `r nrow(AR)`**
#'
#'**Tests de cohérence**

if (nrow(AR) > 0) {
  masses.dernier <- colSums(AR[ ,.(Montant.brut.annuel, rémunération.indemnitaire.imposable, total.lignes.paie, autres.rémunérations)])
} else {
  masses.dernier <- c(0,0) 
}

#'Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :
#'

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Brut annuel (bulletins)",
                    "Brut annuel (lignes), dont :",
                    "\\ \\ Primes :",
                    "\\ \\ Autres rémunérations",
                    "Part de primes en %"),
                  c(masses.dernier["Montant.brut.annuel"],
                    masses.dernier["total.lignes.paie"],
                    masses.dernier["rémunération.indemnitaire.imposable"],
                    masses.dernier["autres.rémunérations"],
                    masses.dernier["rémunération.indemnitaire.imposable"]/masses.dernier["Montant.brut.annuel"] * 100))

#'
#'**Définitions :**
#'
#'  *Brut annuel (bulletins)*   : somme du champ *Brut*   
#'  *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :   
#'  *Primes*                    : indemnités sauf remboursements, certaines IJSS, indemnités d'élu le cas échéant, Supplément familial de traitement et Indemnité de résidence        
#'  *Indemnités d'élus*         : toutes rémunérations indemnitaires des élus    
#'  *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
#'
#'**Tests de cohérence**
#'
#'Somme des rémunérations brutes versées aux personnels (non élus) :
#'
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Agrégats",
                    "k&euro;"),
                  c("Bulletins de paie ",
                    "Lignes de paie ",
                    "Différence "),
                  c(masses.dernier["Montant.brut.annuel"],
                    masses.dernier["total.lignes.paie"],
                    masses.dernier["Montant.brut.annuel"] -
                      masses.dernier["total.lignes.paie"]))


#'
#'A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.
#'
#'
#'**Formation et distribution du salaire brut moyen par tête (SMPT) en EQTP pour l'année `r année`**     
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Traitement indiciaire",
         "Primes",
         "Autres rémunérations",
         "Effectif"),
       AR[ , .(traitement.indiciaire,
               rémunération.indemnitaire.imposable,
               autres.rémunérations)],
       extra = "length")


#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Total lignes hors rappels",
         "Total brut",
         "SMPT brut en EQTP",
         "Part de la rém. indemnitaire",
         "Effectif"),
       AR[ , .(total.lignes.paie,
               Montant.brut.annuel,
               Montant.brut.annuel.eqtp,
               part.rémunération.indemnitaire)],
       extra = "length")

#'**Catégorie A**
#'
#'

ARA <- data.table::data.table(NULL)
ARB <- data.table::data.table(NULL)
ARC <- data.table::data.table(NULL)

if (fichier.personnels.existe)
{
  ARA <- AR[Catégorie == 'A', ]
  #'  
  #'&nbsp;*Tableau `r incrément()`*   
  #'    
  
  Résumé(c("Traitement indiciaire",
           "Primes",
           "Autres rémunérations"),
         ARA[ , .(traitement.indiciaire,
                  rémunération.indemnitaire.imposable,
                   autres.rémunérations)])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
  #'  
  #'&nbsp;*Tableau `r incrément()`*   
  #'    
  
  Résumé(c("Total rémunérations", 
           "Total rémunérations EQTP",
           "Part de la rémunération indemnitaire"),
         ARA[ , .(Montant.brut.annuel,
                  Montant.brut.annuel.eqtp,
                  part.rémunération.indemnitaire)])
}

#'
#'**Effectif : `r nrow(ARA)`**
#'
#'**Catégorie B**
#'

if (fichier.personnels.existe)
{
  ARB <- AR[Catégorie == 'B', ]
  #'  
  #'&nbsp;*Tableau `r incrément()`*   
  #'    
  
  Résumé(c("Traitement indiciaire",
           "Primes",
           "Autres rémunérations"),
         ARB[ , .(traitement.indiciaire,
                  rémunération.indemnitaire.imposable,
                  autres.rémunérations)])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
  #'  
  #'&nbsp;*Tableau `r incrément()`*   
  #'    
  
  Résumé(c("Total rémunérations",
           "Total rémunérations EQTP",
           "Part de la rémunération indemnitaire"),
         ARB[ c( "Montant.brut.annuel",
                 "Montant.brut.annuel.eqtp",
                 "part.rémunération.indemnitaire")])
}

#'
#'**Effectif : `r nrow(ARB)`**
#'
#'**Catégorie C**
#'


if (fichier.personnels.existe)
{
  ARC <- AR[Catégorie == 'C', ]
  #'  
  #'&nbsp;*Tableau `r incrément()`*   
  #'    
  
  Résumé(c("Traitement indiciaire",
           "Primes",
           "Autres rémunérations"),
         ARC[ , .(traitement.indiciaire,
                  rémunération.indemnitaire.imposable,
                  autres.rémunérations)])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
  #'  
  #'&nbsp;*Tableau `r incrément()`*   
  #'    
  
  Résumé(c("Total rémunérations",
           "Total rémunérations EQTP",
           "Part de la rémunération indemnitaire"),
         ARC[ , .(Montant.brut.annuel,
                 Montant.brut.annuel.eqtp,
                 part.rémunération.indemnitaire) ])
}

#'**Effectif : `r nrow(ARC)`**
#'
#'
#'######      <br>
#'   
#'## 3.3 Contractuels, vacataires et stagiaires inclus   
#'   

attach(Analyse.rémunérations.dernier.exercice, warn.conflicts=FALSE)
temp <- rémunération.indemnitaire.imposable.eqtp[!is.na(rémunération.indemnitaire.imposable.eqtp) 
                                                  & Statut != "ELU"
                                                  & Statut != "TITULAIRE"
                                                  & Statut != "STAGIAIRE"
                                                  & rémunération.indemnitaire.imposable.eqtp > 1000] / 1000

if (length(temp > 0))
  hist(temp,
       xlab = "Rémunération indemnitaire brute imposable en milliers d'euros EQTP\n",
       ylab = "Effectif",
       xlim = c(0, 40),
       main = "Rémunération annuelle totale des contractuels en " %+% année,
       col = "red",
       nclass = 50)
#'
#'**Nota :**
#'Ne sont retenues que les rémunérations supérieures à 1 000 k&euro;.
#'Les élus ne sont pas pris en compte.
#'

temp <- positive(autres.rémunérations)

detach(Analyse.rémunérations.dernier.exercice)

if (length(temp))
  hist(temp,
       xlab = "En euros :\n divers",
       ylab = "Effectif",
       xlim = c(0, 5000),
       main = "Autres rémunérations en " %+% année,
       nclass = 50,
       col = "grey")

#'

AR <- Analyse.rémunérations.dernier.exercice[Statut != "ELU"
                                             &  Statut != "TITULAIRE"
                                             &  Statut != "STAGIAIRE",
                                             colonnes.sélectionnées, with=FALSE]

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Primes",
         "Autres rémunérations",
         "Effectif"),
       AR[ , .(rémunération.indemnitaire.imposable,
               autres.rémunérations)],
       extra = "length")

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


Résumé(c("Total rémunérations",
         "Total rémunérations EQTP",
         "Effectif"),
       AR[ , .(Montant.brut.annuel, Montant.brut.annuel.eqtp)],
       extra = "length")
#'

nom.base.analyse <- "Analyse.rémunérations.dernier.exercice"

#'
#'[Lien vers la base de données](Bases/Rémunérations/Analyse.rémunérations.csv) d'analyse des rémunérations
#'

#'   
#'## 3.4 Comparaisons source INSEE/DGCL   
#'   
#'*Salaires annnuels moyens 2011 en EQTP (hors assistantes maternelles)*   

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Agrégat", "Salaires bruts 2011 (&euro;)", "Salaires bruts 2012 (&euro;)"),
                  c("Ensemble", "Titulaires", "Autres salariés"),
                  12 * c(2159, 2223, 1903),
                  12 * c(2195, 2259, NA))

#'   
#'**Eléments de la rémunération brute pour les titulaires de la FPT entre 2010 et 2012**      
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(
  c("Rém. annuelles", "2010 (&euro;)", "Primes (%)", "2011 (&euro;)", "Primes (%)", "2012 (&euro;)", "Primes (%)"),
  c("Salaire brut",
    "Traitement brut",
    "Primes et rémunérations annexes",
    "y compris IR et SFT"),
  c(26305, 20350,	"", 5955),
  c("", 22.6, "", "" ),
  c(26660, 20562, "", 6098),
  c("", 22.9, "", "" ),
  c(12*2195, 12*1727, "", 532),
  c("", 23.6, "", "" ))
#'   
#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*   												
#'*Les primes sont cumulées au supplément familial de traitement (SFT) et à l'indemnité de résidence (IR). Le cumul est rapporté à la rémunération brute totale.*    
#'[Source INSEE](http://www.insee.fr/fr/ffc/ipweb/ip1486/ip1486.xls)    
#'[Source DGCL](http://www.fonction-publique.gouv.fr/files/files/statistiques/rapports_annuels/2012-2013/xls/Vue3_1_Remunerations.xls)    
#'[Source DGFIP PLF 2015](http://www.performance-publique.budget.gouv.fr/sites/performance_publique/files/farandole/ressources/2015/pap/pdf/jaunes/jaune2015_fonction_publique.pdf)   
#'   

########### Analyse dynamique ########################
#'
#'# 4. Rémunérations nettes : évolutions sur la période `r début.période.sous.revue` - `r fin.période.sous.revue`    
#'
#'Nombre d'exercices: `r durée.sous.revue`   
#'  
#'**Les données présentées dans cette section sont toutes relatives à des rémunérations nettes en équivalent temps plein (EQTP)**      
#'    
#'*Nota :*   
#'*EQTP = Equivalent temps plein  = 12 . moyenne du ratio rémunération / quotité*    
#'    
#'## 4.1 Distribution de la rémunération nette moyenne sur la période    


Analyse.variations.par.exercice <- Analyse.variations.par.exercice[nb.jours > seuil.troncature
                                                                   & ! is.na(Montant.net.annuel.eqtp)
                                                                   & Montant.net.annuel.eqtp  > minimum.positif 
                                                                   & ! is.na(Statut)
                                                                   & Statut !=  "ELU"] 

attach(Analyse.variations.synthèse)

temp <- positive(moyenne.rémunération.annuelle.sur.période) / 1000

if (longueur.non.na(temp) > 0)
  hist(temp,
       xlab = "Sur la période " %+% début.période.sous.revue %+% "-" %+% fin.période.sous.revue %+% " en milliers d'euros",
       ylab = "Effectif",
       main = "Rémunération nette moyenne",
       col = "blue",
       nclass = 200)

#'
#+ fig.height=4.5

temp <- na.omit(moyenne.rémunération.annuelle.sur.période[  moyenne.rémunération.annuelle.sur.période > minimum.positif
                                                            & (statut == "TITULAIRE"  | statut == "STAGIAIRE")] / 1000)

if (longueur.non.na(temp) > 0)
  hist(temp,
       xlab = "Sur la période "%+% début.période.sous.revue %+% "-" %+% fin.période.sous.revue %+% " en milliers d'euros",
       ylab = "Effectif",
       main = "Rémunération nette moyenne des fonctionnaires",
       col = "blue",
       nclass = 200)

detach(Analyse.variations.synthèse)

#'
#'[Lien vers la base de données synthétique](Bases/Rémunérations/Analyse.variations.synthèse.csv)
#'[Lien vers la base de données détaillée par année](Bases/Rémunérations/Analyse.variations.par.exercice.csv)
#'
#'## 4.2 Evolutions des rémunérations nettes sur la période `r début.période.sous.revue` - `r fin.période.sous.revue`   
#'
#'### 4.2.1 Ensemble des personnels fonctionnaires et non titulaires (hors élus)
#'
#'


masse.salariale.nette <- rep(0, durée.sous.revue)

f <- function(x) prettyNum(masse.salariale.nette[x - début.période.sous.revue + 1] <<- 
                             sum(Analyse.variations.par.exercice[Année == x, 
                                                               Montant.net.annuel.eqtp],
                               na.rm = TRUE) / 1000,
                           big.mark = " ",
                           digits = 5,
                           format = "fg")

g <- function(x) prettyNum(mean.default(Analyse.variations.par.exercice[Année == x, 
                                                               Montant.net.annuel.eqtp],
                               na.rm = TRUE),
                           big.mark = " ",
                           digits = 1,
                           format = "fg")
#'    
#'**Salaire net moyen par tête (SMPT net) en EQTP, hors élus**         
#'       
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical(c(étiquette.année, "Rém. nette totale (k&euro;)", "SMPT net (&euro;)"),
                 période,
                 extra = "variation",
                 f,
                 g)

entrants <- function(x)   {
  
  A <- setdiff(Analyse.variations.par.exercice[Année == x, Matricule], 
               Analyse.variations.par.exercice[Année == x -1, Matricule])
  

  B <- unique(Bulletins.paie[Année == x 
                             & Matricule %chin% A, .(Année, quotité, Matricule, Mois, Statut)], by = NULL)

  eqtp.agent <- B[ , sum(quotité, na.rm=TRUE)] / 12
  eqtp.fonct <- B[Statut == "TITULAIRE" | Statut == "STAGIAIRE", sum(quotité, na.rm=TRUE)] / 12

  list(A, eqtp.agent, eqtp.fonct)
}

sortants <- function(x)   {
  
  A <- setdiff(Analyse.variations.par.exercice[Année == x-1, Matricule], 
               Analyse.variations.par.exercice[Année == x, Matricule])
    
  B <- unique(Bulletins.paie[Année == x - 1
                             & Matricule %chin% A, .(Année, quotité, Matricule, Mois, Statut)], by = NULL)
  
  eqtp.agent <- B[ , sum(quotité, na.rm=TRUE)] / 12
  eqtp.fonct <- B[Statut == "TITULAIRE" | Statut == "STAGIAIRE", sum(quotité, na.rm=TRUE)] / 12
  
  list(A, eqtp.agent, eqtp.fonct)
}

s <- list(0)
e <- list(0)
noria <- rep(0, durée.sous.revue)
remplacements <- rep(0, durée.sous.revue)

f <- function(x) {
  y <- x - début.période.sous.revue
  
  s[[y]] <<- sortants(x)
  e[[y]] <<- entrants(x)
  
  noria[y] <<- mean.default(Analyse.variations.par.exercice[Année == x 
                                                            & Matricule %chin% e[[y]][[1]], 
                                                       Montant.net.annuel.eqtp],
                    na.rm = TRUE) - mean.default(Analyse.variations.par.exercice[Année == x- 1 
                                                                                 & Matricule %chin% s[[y]][[1]], 
                                                                                          Montant.net.annuel.eqtp],
                                                 na.rm = TRUE)
  
  prettyNum(noria[y],
            big.mark = " ",
            digits = 5,
            format = "fg")
}

g <- function(x) {
  
  y <- x - début.période.sous.revue

  remplacements[y] <<- min(e[[y]][[2]], s[[y]][[2]], na.rm=TRUE)
  
  prettyNum(noria[y] * remplacements[y] / (masse.salariale.nette[y] * 10),
                           big.mark = " ",
                           digits = 3,
                           format = "fg")
}
#'   
#'**Effet de noria sur salaires nets et taux de remplacements**       
#'   
#'**Effet de noria** : *différence entre la rémunération annuelle des entrants à l'année N et des sortants à l'année N-1*.  
#'*Usuellement calculée sur les rémunérations brutes, ici sur les rémunérations nettes EQTP*  
#'*afin d'apprécier l'impact de cet effet sur l'évolution des rémunérations nette moyennes calculée au tableau précédent.*               
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    
if (durée.sous.revue > 1) {
Tableau.vertical(c(étiquette.année,  "Noria EQTP (&euro;)", "En % de la MS N-1", "Remplacements EQTP", "Taux de remplacements (%)"),
                 période[2:durée.sous.revue],
                 extra = "no",
                 f,
                 g,
                 function(x) prettyNum(remplacements[x - début.période.sous.revue], digits=0, format="f"),
                 function(x) prettyNum(remplacements[x - début.période.sous.revue]/ effectifs[[as.character(x)]]["ETPT"] * 100, digits=2, format="f"))
} else {
  cat("L'effet de noria ne peut être calculé que pour des durées sous revue supérieures à un exercice.")
}

#'
#'*MS N-1 : masse salariale nette de l'année n-1.*   
#'       
#'**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         
#'       
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé("Première année",
       Analyse.variations.par.exercice[Année == début.période.sous.revue , Montant.net.annuel.eqtp])

Résumé("Dernière année",
       Analyse.variations.par.exercice[Année == fin.période.sous.revue , Montant.net.annuel.eqtp])


#'  
#'*Nota :*  La population retenue est constituée des agents qui :   
#'&nbsp;&nbsp;- ne font pas partie des `r quantile.cut` centiles extrêmaux   
#'&nbsp;&nbsp;- sont au moins présents `r seuil.troncature` jours la première et la dernière année d'activité  
#'   
#'**Comparaisons source INSEE/DGCL**   
#'
#'**Salaires annuels moyens 2011 et 2012 en EQTP (hors assistantes maternelles)**   
#'  
#'&nbsp;*Tableau `r incrément()`*   '    

  
Tableau.vertical2(c("Agrégat",  "Salaires nets 2011 (&euro;)", "Salaires nets 2012 (&euro;)"),
                  c("Ensemble", "Titulaires", "Autres salariés"),
                  12*c(1823, 1886, 1572),
                  12*c(1848, 1910, NA))

#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			


matrice.déciles <- t(matrix(12*c(1458, 1274, 1382, 1170, 1743, 1376, 1514, 1305, 1921, 1459, 1635, 1428, 2076, 1540, 1754,
                                 1559, 2236, 1636, 1883, 1712, 2412, 1751, 2042, 1902, 2636, 1905, 2268, 2156, 2966, 2133,
                                 2583, 2569, 3538, 2573, 3151, 3400),  ncol = 9))


#'**Distribution des salaires nets annuels en EQTP dans la fonction publique par versant en 2011**   
#' 
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


Tableau.vertical2(c("Décile (k&euro;)", "FPE", "FPT", "FPH", "Secteur privé"),
                  paste0("D", 1:9),
                  matrice.déciles[,1],
                  matrice.déciles[,2],
                  matrice.déciles[,3],
                  matrice.déciles[,4])


matrice.déciles.cat <- matrix(12*c(2170,2416,2606,2789,2985,3222,3523,3927,4570,1823,
                                    1715,1856,1971,2080,2187,2303,2430,2582,2817,3225,
                                    1331,1408,1471,1530,1597,1675,1768,1890,2083,2244,
                                    1135,1195,1252,1307,1364,1436,1540,1732,2243,1668), nrow = 10)



#'**Distribution des salaires nets annuels en EQTP dans la fonction publique territoriale par catégorie en 2011**   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


Tableau.vertical2(c("Décile (k&euro;)", "Catégorie A", "Catégorie B", "Catégorie C", "Autres salariés"),
                  c(paste0("D", 1:9), "Moyenne"),
                  matrice.déciles.cat[,1],
                  matrice.déciles.cat[,2],
                  matrice.déciles.cat[,3],
                  matrice.déciles.cat[,4])

#'[Source INSEE, onglets Figure3, F1web et F3web](http://www.insee.fr/fr/ffc/ipweb/ip1486/ip1486.xls)   
#'   
#'[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)
#'   

#'### 4.2.2 Fonctionnaires
#'
#'**Titulaires et stagiaires**      

f <- function(x) {

  masse.salariale.nette[x - début.période.sous.revue + 1] <<-  sum(Analyse.variations.par.exercice[Année == x
                                                                                                   & (Statut == "TITULAIRE" | Statut == "STAGIAIRE"), 
                                                                                                      Montant.net.annuel.eqtp],
                                                                na.rm = TRUE) / 1000

 prettyNum(masse.salariale.nette[x - début.période.sous.revue + 1],
                           big.mark = " ",
                           digits = 5,
                           format = "fg")
}

g <- function(x) prettyNum(mean.default(Analyse.variations.par.exercice[Année == x 
                                                                        & (Statut == "TITULAIRE" | Statut == "STAGIAIRE"), 
                                                                         Montant.net.annuel.eqtp],
                           na.rm = TRUE),
                   big.mark = " ",
                   digits = 1,
                   format = "fg")


#'**Salaire net moyen par tête (SMPT net) en EQTP**       
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical(c(étiquette.année, "Rém. nette totale (k&euro;)", "SMPT net en EQTP (&euro;)"),
                 période,
                 extra = "variation",
                 f,
                 g)

#'
f <- function(x) {
  y <- x - début.période.sous.revue
  
  noria[y] <<- sum(Analyse.variations.par.exercice[Année == x 
                                                   & (Statut == "TITULAIRE" | Statut == "STAGIAIRE") 
                                                   & Matricule %chin% e[[y]][[1]], 
                                                     Montant.net.annuel.eqtp],
                   na.rm = TRUE) / e[[y]][[3]] -  sum(Analyse.variations.par.exercice[Année == x- 1
                                                                                      & (Statut == "TITULAIRE" | Statut == "STAGIAIRE") 
                                                                                      & Matricule %chin% s[[y]][[1]],
                                                                                        Montant.net.annuel.eqtp],
                                                  na.rm = TRUE) / s[[y]][[3]]
  
  prettyNum(noria[y],
            big.mark = " ",
            digits = 5,
            format = "fg")
}

g <- function(x) {
  
  y <- x - début.période.sous.revue

  remplacements[y] <<- min(e[[y]][[3]], s[[y]][[3]], na.rm=TRUE)
  
  prettyNum(noria[y] * remplacements[y] / (masse.salariale.nette[y] * 10),
            big.mark = " ",
            digits = 3,
            format = "fg")
}

#'   
#'**Effet de noria sur salaires nets et taux de remplacements**       
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

if (durée.sous.revue > 1) {
Tableau.vertical(c(étiquette.année,  "Noria EQTP (&euro;)", "En % de la  MSN N-1", "Remplacements EQTP", "Taux de remplacements (%)"),
                 période[2:length(période)],
                 extra = "no",
                 f,
                 g,
                 function(x) prettyNum(remplacements[x - début.période.sous.revue], digits=0, format="f"),
                 function(x) prettyNum(remplacements[x - début.période.sous.revue]/ effectifs[[as.character(x)]]["ETPT_fonct"] * 100, digits=2, format="f"))
} else {
  cat("L'effet de noria ne peut être calculé que pour des durées sous revue supérieures à un exercice.")
}
#'     
#'*MS N-1 : masse salariale nette de l'année n-1.*   
#'       
#'**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


Résumé("Première année",
       Analyse.variations.par.exercice[Année == début.période.sous.revue & (Statut == "TITULAIRE" | Statut == "STAGIAIRE"),
                                           Montant.net.annuel.eqtp])

Résumé("Dernière année",
       Analyse.variations.par.exercice[Année == fin.période.sous.revue & (Statut == "TITULAIRE" | Statut == "STAGIAIRE"),
                                       Montant.net.annuel.eqtp])


#'    
f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[Année == x 
                                                               & Statut == "TITULAIRE"
                                                               & temps.complet == TRUE & permanent == TRUE, 
                                                               Montant.net.annuel.eqtp],
                               na.rm = TRUE) / 1000,
                           big.mark = " ",
                           digits = 5,
                           format = "fg")

g <- function(x) prettyNum(mean.default(Analyse.variations.par.exercice[Année == x
                                                               & Statut == "TITULAIRE"
                                                               & temps.complet == TRUE & permanent == TRUE, 
                                                               Montant.net.annuel.eqtp],
                               na.rm = TRUE),
                           big.mark = " ",
                           digits = 1,
                           format = "fg")

#'   
#'**Evolution du SMPT net des titulaires titulaires à temps complet**     
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical(c(étiquette.année, "Rémunération nette totale (k&euro;)", "SMPT (&euro;)"),
                 période,
                 extra = "variation",
                 f,
                 g)

#'    
#'**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) des titulaires à temps complet**         
#'       
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


Résumé("Première année",
       Analyse.variations.par.exercice[Année == début.période.sous.revue
                                       & Statut == "TITULAIRE" 
                                       & temps.complet == TRUE
                                       & permanent == TRUE,
                                           Montant.net.annuel.eqtp])

#'   
Résumé("Dernière année",
       Analyse.variations.par.exercice[Année == fin.période.sous.revue
                                       & Statut == "TITULAIRE" 
                                       & temps.complet == TRUE
                                       & permanent == TRUE,
                                       Montant.net.annuel.eqtp])


#'
#'[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)
#'
#'

#'## 4.3 Glissement viellesse-technicité (GVT)   
#'
#'### 4.3.1 Ensemble des personnels   
#'   
#'*Cette section est consacrée à la rémunération moyenne des personnes en place (RMPP), définies comme présentes deux années consécutives avec la même quotité*   
#'*L'évolution de la RMPP permet d'étudier le glissement viellesse-technicité "positif", à effectifs constants sur deux années*      
#'*Le GVT positif est dû aux mesures statutaires et individuelles, à l'avancement et aux changements d'activité*  


# Appliquer les filtres maintenant

q3 <- quantile(Analyse.variations.synthèse$variation.rémunération, c(quantile.cut/100, 1 - quantile.cut/100), na.rm=TRUE)

# Filtrage : on enlève les personnels présents depuis moins d'un seuil de troncature (ex. 120 jours) dans l'année et les élus
# (paramètre seuil.troncature) 

# Filtrage pour l'étude des variations : on enlève les valeurs manquantes des variations, les centiles extrêmaux,
# les rémunérations nettes négatives ou proche de zéro. On exige un statut explicite en fin de période.
# Paramétrable par :
# minimum.positif, quantile.cut 


Analyse.variations.synthèse <- Analyse.variations.synthèse[ total.jours > 2 * seuil.troncature
                                                            & pris.en.compte == TRUE
                                                            & ! is.na(statut)   
                                                            & statut !=  "ELU"
                                                            & ! is.na(variation.rémunération) 
                                                            & variation.rémunération > q3[[1]]
                                                            & variation.rémunération < q3[[2]]]

Analyse.variations.synthèse.plus.2.ans  <- Analyse.variations.synthèse[! is.na(plus.2.ans) & plus.2.ans == TRUE]
Analyse.variations.synthèse.moins.2.ans <- Analyse.variations.synthèse[! is.na(plus.2.ans) & plus.2.ans == FALSE]

#Analyse.variations.par.exercice <- Analyse.variations.par.exercice[Nexercices > 1]


if (nrow(Analyse.variations.synthèse.plus.2.ans) > 0)
  hist(Analyse.variations.synthèse.plus.2.ans$variation.moyenne.rémunération,
       xlab ="Variation annuelle moyenne en %",
       las = 1,
       xlim = c(-5,30),
       ylab ="Effectifs",
       main ="Rémunération nette des personnes en place",
       col ="blue",
       nclass=1000,
       xaxt = 'n')

try(axis(side=1, at=seq(-5,30, 1), labels=seq(-5,30,1), lwd=2))

#'
#'

f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[Année == x
                                                               & est.rmpp == TRUE,
                                                               Montant.net.annuel.eqtp],
                               na.rm = TRUE)/ 1000,
                           big.mark = " ",
                           digits = 5,
                           format = "fg")

g <- function(x) prettyNum(mean.default(Analyse.variations.par.exercice[Année == x 
                                                               & est.rmpp == TRUE,
                                                               Montant.net.annuel.eqtp],
                               na.rm = TRUE) ,
                           big.mark = " ",
                           digits = 1,
                           format = "fg")
#'   
#'**Evolution de la RMPP nette en EQTP**     
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical(c(étiquette.année,
                   "Rémunération nette totale (k&euro;)",
                   "RMPP nette (k&euro;)"),
                 période[2:durée.sous.revue],
                 extra = "variation",
                 f,
                 g)


#'    
#'**Distribution et variation sur la période de la rémunération nette des personnes en place**                
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

# La légère différence de pérmètre entre Analyse.variations.synthèse et Analyse.variations.par.exercice tient au filtrage des quantiles
# extrêmaux et des valeurs manquantes des variations


masque.rmpp.fin.période    <- bitwShiftL(3, durée.sous.revue - 2)      #  11{0,1}...{0,1}
masque.rmpp.début.période  <- 3                                        #  {0,1}...{0,1}11
masque.présent.début.fin   <- bitwShiftL(1, durée.sous.revue - 1) + 1  #  10000..1
masque.présent.sur.période <- bitwShiftL(1, durée.sous.revue) -1       #  11111..1

#'  
Résumé(c("Première année",
         "Effectif"),
       Analyse.variations.synthèse[bitwAnd(indicatrice.période, masque.rmpp.début.période) == masque.rmpp.début.période, 
                                           Montant.net.annuel.eqtp.début],
       extra = "length")
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Dernière année",
         "Effectif"),
        Analyse.variations.synthèse[indicatrice.période >= masque.rmpp.fin.période, Montant.net.annuel.eqtp.sortie],
        extra = "length")
#'
#'*Variation individuelle de rémunération nette en EQTP pour les personnels présents la première et la dernière année*   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Variation normalisée (%)",
         "Variation annuelle moyenne normalisée (%)",
         "Effectif"),
       Analyse.variations.synthèse[bitwAnd(indicatrice.période, masque.présent.début.fin) == masque.présent.début.fin, c("variation.rémunération.normalisée",
                                                                                                                          "variation.moyenne.rémunération.normalisée"), with=FALSE],
       extra = "length")

# #'
# #'*Variation individuelle de rémunération nette en EQTP pour les personnels présents sur toute la période*   
# #'  
# #'&nbsp;*Tableau `r incrément()`*   
# #'  
# 
# Résumé("Variation normalisée (%)",
#         # "Variation annuelle moyenne normalisée (%)",
#          "Effectif"),
#        Analyse.variations.synthèse[indicatrice.période == masque.présent.sur.période, variation.rémunération.normalisée],
#        extra = "length")

#'    
#'### 4.3.2 Titulaires et stagiaires     
#'   

f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[Année == x
                                                               & est.rmpp == TRUE
                                                               & (Statut == "TITULAIRE" | Statut == "STAGIAIRE"),
                                                                 Montant.net.annuel.eqtp],
                               na.rm = TRUE)/ 1000,
                           big.mark = " ",
                           digits = 5,
                           format = "fg")

g <- function(x) prettyNum(mean.default(Analyse.variations.par.exercice[Année == x 
                                                                        & est.rmpp == TRUE
                                                                        & (Statut == "TITULAIRE" | Statut == "STAGIAIRE"),
                                                                        Montant.net.annuel.eqtp],
                                        na.rm = TRUE) ,
                           big.mark = " ",
                           digits = 1,
                           format = "fg")
#'   
#'**Evolution de la RMPP nette en EQTP**     
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


Tableau.vertical(c(étiquette.année,
                   "Rémunération nette totale (k&euro;)",
                   "RMPP nette (k&euro;)"),
                 période[2:durée.sous.revue],
                 extra = "variation",
                 f,
                 g)

#'    
#'**Distribution et variation sur la période de la rémunération nette des fonctionnaires en place**                
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

#'  
Résumé(c("Première année",
         "Effectif"),
       Analyse.variations.synthèse[(statut == "TITULAIRE" | statut == "STAGIAIRE")
                                   & bitwAnd(indicatrice.période, masque.rmpp.début.période) == masque.rmpp.début.période, 
                                   Montant.net.annuel.eqtp.début],
       extra = "length")
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Dernière année",
         "Effectif"),
       Analyse.variations.synthèse[statut == "TITULAIRE" | statut == "STAGIAIRE"
                                   & indicatrice.période >= masque.rmpp.fin.période, Montant.net.annuel.eqtp.sortie],
       extra = "length")
#'
#'*Variation individuelle de rémunération nette en EQTP pour les personnels présents la première et la dernière année*   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Variation normalisée (%)",
         "Variation annuelle moyenne normalisée (%)",
         "Effectif"),
       Analyse.variations.synthèse[(statut == "TITULAIRE" | statut == "STAGIAIRE")
                                   & bitwAnd(indicatrice.période, masque.présent.début.fin) == masque.présent.début.fin,
                                   c("variation.rémunération.normalisée",  "variation.moyenne.rémunération.normalisée"),
                                   with = FALSE],
       extra = "length")


#'
#'
#'[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)
#'
#'**Nota**   
#'*Personnes en place :* en fonction au moins deux années consécutives sur la période `r début.période.sous.revue` à `r fin.période.sous.revue`    
#'*Variation sur la période d'activité :* entre l'arrivée et le départ de la personne      
#'*Variation normalisée :* conforme à la définition INSEE (présente en début et en fin de période avec la même quotité)  
#'  
#'**Commentaire**       
#'Les différences éventuelles constatées entre l'évolution de la RMPP au tableau `r numéro.tableau-2` sont dues soit à l'effet de noria soit à l'effet périmètre.    
#'      

#'
########### Tests statutaires ########################
#'
#'[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)
#'
#'
#'### 4.4 Comparaisons avec la situation nationale des rémunérations   
#'  
#'**Évolution en euros courants du SMPT et de la RMPP dans la FPT (en % et euros courants)**    

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Année", "2008-2009", "2009-2010", "2010-2011", "2011-1012", "2008-2012", "Moy. 2008-2012", "Médiane 2007-2011"),
                  c("SMPT brut", "SMPT net", "RMPP brute", "RMPP nette"),         
                  t(data.frame(c("2,5", "1,3", "1,5", "1,7", "7,2", "1,8", ""),
                  c("3,0", "1,4", "1,3", "1,4", "7,3", "1,8", "13,4"),
                  c("3,3", "2,5", "2,5", "2,7", "11,5", "2,8", ""),
                  c("3,3", "2,5", "2,3", "2,4", "10,9", "2,6", ""))))


#'*Source : fichier général de l'État (FGE), DADS, SIASP, Insee, Drees. Traitement Insee, Drees, DGCL*    
#'Hors assistants maternels et familiaux, y compris bénéficiaires de contrats aidés.   
#'SMPT : Salaire moyen par tête en EQTP.   
#'RMPP : Agents présents 24 mois consécutifs chez le même employeur avec la même quotité de travail.  
#'Moyenne des variation géométriques annuelles pour les agents du champ.  
#'La dernière colonne présente la médiane des augmentations du SMPT net pour les agents présents en 2007 et 2011.   
#'  
#'**Salaires nets annuels et évolution moyenne type de collectivité en &euro; courants  EQTP**    
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Type de collectivité", "SMPT net 2011 (&euro;)", "SMPT net 2012 (&euro;)", "Évolution annuelle moy. 2007-2011 (%)"),
  c("Communes",
    "CCAS et caisses des écoles",
    "EPCI à fiscalité propre",
    "Autres structures intercommunales",
    "Départements",
    "SDIS",
    "Régions",		 
    "Autres collectivités locales",	 
    "Ensemble (moyenne)",
    "Ensemble (médiane)"),	
   c(21575, 20326, 24088, 22686, 26910, 30548, 24137,	26242, 22954, 19626),
  12*c(1760, 1643, 1924, 1807, 2062, 2495, 1903,  2058, 1848, NA),
   c("2,5", "2,4", "3,1", "3,0", "3,9", "3,4", "3,8", "3,2", "2,9", ""))

#'
#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			
#'Conversion en euros courants, calcul CRC.  
#'[Source INSEE](http://www.insee.fr/fr/ffc/ipweb/ip1486/ip1486.xls)   
#'[Source DGAFP](http://infos.emploipublic.fr/dossiers/la-fonction-publique-en-chiffres/la-fonction-publique-en-chiffre-2013/non-defini-08/apm-71444/)  
#'   
#'
#'# 5. Tests réglementaires   
#'## 5.1 Contrôle des heures supplémentaires, des NBI et primes informatiques   
#'   


if (N <- length(unique(Paie[Statut != "TITULAIRE"
                                        & Statut != "STAGIAIRE"
                                        & NBI != 0, 
                                        Matricule])))
  cat("Il existe ", N, "non titulaires percevant une NBI.")

NBI.aux.non.titulaires <- Paie[Statut != "TITULAIRE"
                               & Statut != "STAGIAIRE"
                               & NBI != 0
                               & grepl("N.*B.*I", Libellé, ignore.case=TRUE),
                               c(étiquette.matricule,
                                 "Statut",
                                 étiquette.code,
                                 étiquette.libellé,
                                 étiquette.année,
                                 "Mois",
                                 "NBI",
                                 étiquette.montant),
                               with=FALSE]

nombre.Lignes.paie.NBI.nontit <- nrow(NBI.aux.non.titulaires)

# Prime de fonctions informatiques : pas dans la base de VLB
# on cherche la chaine de char. "INFO" dans les libellés de primes

# variante : filtre <- regexpr(".*(INFO|PFI|P.F.I).*", toupper(Paie$Libellé)) et regmatches(Paie$Libellé, filtre)

attach(Paie, warn.conflicts=FALSE)
filtre <- grep(".*(INFO|PFI|P.F.I).*", Libellé)

personnels.prime.informatique <- Paie[ filtre,
                                       c(étiquette.matricule,
                                         étiquette.année,
                                         "Mois",
                                         "Statut",
                                         étiquette.code,
                                         étiquette.libellé,
                                         étiquette.montant), with=FALSE]

primes.informatiques.potentielles <- unique(Libellé[filtre], by=NULL)

if  (length(primes.informatiques.potentielles) == 0)
  primes.informatiques.potentielles <- "aucune"

nombre.personnels.pfi <- nrow(personnels.prime.informatique)

detach(Paie)
#'Primes informatiques potentielles : `r primes.informatiques.potentielles`
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau(
  c("Nombre de lignes NBI pour non titulaires",
    "Nombre de bénéficiaires de PFI"),
  nombre.Lignes.paie.NBI.nontit,
  nombre.personnels.pfi)

#'   
#'[Lien vers la base de données NBI aux non titulaires](Bases/Réglementation/NBI.aux.non.titulaires.csv)   
#'[Lien vers la base de données Primes informatiques](Bases/Réglementation/personnels.prime.informatique.csv)   
#'   
#'**Nota :**   
#'NBI: nouvelle bonification indiciaire   
#'PFI: prime de fonctions informatiques   
#'  
#'## 5.2 Contrôle des vacations pour les fonctionnaires

# Vacations et statut de fonctionnaire

  lignes.fonctionnaires.et.vacations <- Paie[(Statut == "TITULAIRE" | Statut == "STAGIAIRE") & Grade == "V",
                                              c(étiquette.matricule,
                                                "Nom", "Prénom",
                                                "Statut",
                                                étiquette.code,
                                                étiquette.libellé,
                                                étiquette.montant),
                                              with=FALSE]

  matricules.fonctionnaires.et.vacations <- unique(lignes.fonctionnaires.et.vacations[ , .(Matricule, Nom, Prénom)], by=NULL)
  nombre.fonctionnaires.et.vacations <- nrow(matricules.fonctionnaires.et.vacations)
#'

if (! is.null(nombre.fonctionnaires.et.vacations)) {
  cat("Il y a ",
      nombre.fonctionnaires.et.vacations,
      "fonctionnaire(s) effectuant des vacations pour son propre établissement. Les bulletins concernés sont donnés en lien." )
}  else  {
  cat("Pas de vacation détectée.")
}


#'
#'[Lien vers les matricules des fonctionnaires concernés](Bases/Réglementation/matricules.fonctionnaires.et.vacations.csv)
#'[Lien vers les bulletins de paye correspondants](Bases/Réglementation/lignes.fonctionnaires.et.vacations.csv)
#'
#'## 5.3 Contrôles sur les cumuls traitement indiciaire, indemnités et vacations des contractuels

# Vacations et régime indemnitaire

  lignes.contractuels.et.vacations <- Paie[Statut != "TITULAIRE"
                                           & Statut != "STAGIAIRE"
                                           & Grade == "V",
                                           c(étiquette.matricule,
                                             "Nom", "Prénom",
                                             étiquette.code,
                                             étiquette.libellé,
                                             étiquette.montant),
                                           with=FALSE]

  matricules.contractuels.et.vacations <- unique(lignes.contractuels.et.vacations[ , .(Matricule, Nom, Prénom)], by=NULL)

  nombre.contractuels.et.vacations     <- nrow(matricules.contractuels.et.vacations)
    
  RI.et.vacations <- data.frame(NULL)
  traitement.et.vacations <- data.frame(NULL)

 if (nombre.contractuels.et.vacations) 
  {
     RI.et.vacations <- Paie[Type == "I"
                             & Matricule %chin% matricules.contractuels.et.vacations$Matricule,
                             c(étiquette.matricule,
                               "Statut",
                               étiquette.code,
                               "Type",
                               étiquette.libellé,
                               étiquette.montant), 
                             with=FALSE]
  
  # Vacations et indiciaire
  
    traitement.et.vacations <- Paie[Type == "T" 
                                    & Matricule %chin% matricules.contractuels.et.vacations$Matricule,
                                    c(étiquette.matricule,
                                      "Statut",
                                      étiquette.code,
                                      "Type",
                                      étiquette.libellé,
                                      étiquette.montant),
                                    with=FALSE]
  }

  nombre.Lignes.paie.contractuels.et.vacations <- nrow(lignes.contractuels.et.vacations)
  nombre.Lignes.paie.RI.et.vacations           <- nrow(RI.et.vacations)
  nombre.Lignes.paie.traitement.et.vacations   <- nrow(traitement.et.vacations)

#'
#'**Contractuels effectuant des vacations (CEV)**
#'


if (exists("nombre.contractuels.et.vacations")) {
  #'  
  #'&nbsp;*Tableau `r incrément()`*   
  #'    
  
  Tableau(c("Nombre de CEV",
            "Nombre de lignes",
            "Nombre de lignes indemnitaires",
            "Nombre de lignes de traitement"),
          nombre.contractuels.et.vacations,
          nombre.Lignes.paie.contractuels.et.vacations,
          nombre.Lignes.paie.RI.et.vacations,
          nombre.Lignes.paie.traitement.et.vacations)
}
#'  
#'[Lien vers le bulletins des CEV](Bases/Réglementation/lignes.contractuels.et.vacations.csv)   
#'[Lien vers la base de données Matricules des CEV](Bases/Réglementation/matricules.contractuels.et.vacations.csv)  
#'[Lien vers la base de données Cumul régime indemnitaire et vacations de CEV](Bases/Réglementation/RI.et.vacations.csv)  
#'[Lien vers la base de données Lignes de traitement indiciaire pour CEV](Bases/Réglementation/traitement.et.vacations.csv)  
#'  
#'
#'
#'## 5.4 Contrôle sur les indemnités IAT et IFTS

#IAT et IFTS

ifts.logical <- grepl(expression.rég.ifts, Paie$Libellé, ignore.case=TRUE)
iat.logical  <- grepl(expression.rég.iat, Paie$Libellé, ignore.case=TRUE)
codes.ifts  <- unique(Paie[ifts.logical, Code], by=NULL)
personnels.iat.ifts <- intersect(Paie[iat.logical, Matricule],
                                 Paie[ifts.logical, Matricule])
if (length(personnels.iat.ifts))  names(personnels.iat.ifts) <- "Matricules des agents percevant IAT et/ou IFTS sur la période"

nombre.personnels.iat.ifts <- length(personnels.iat.ifts)
base.iat.ifts <- Paie[Matricule %chin% personnels.iat.ifts & (ifts.logical | iat.logical)]
base.iat.ifts <- base.iat.ifts[order(Matricule, Année, Mois)]

#'
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau(c("Codes IFTS", "Nombre de personnels percevant IAT et IFTS"),
        sep.milliers = "",
        paste0(codes.ifts[[1]], collapse = " "),
        nombre.personnels.iat.ifts)

#'
#'[Codes IFTS retenus](Bases/Réglementation/codes.ifts.csv)
#'[Lien vers la base de données](Bases/Réglementation/base.iat.ifts.csv)
#'
#'### Contrôle sur les IFTS pour catégories B et contractuels

#IFTS et IB >= 380 (IM >= 350)

lignes.ifts.anormales <- na.omit(Paie[as.integer(Indice) < 350   & ifts.logical == TRUE,
                                      c(clé.fusion,
                                        étiquette.année,
                                        "Mois",
                                        "Statut",
                                        étiquette.code,
                                        étiquette.libellé,
                                        "Indice",
                                        étiquette.montant), 
                                      with=FALSE])

nombre.lignes.ifts.anormales <- nrow(lignes.ifts.anormales)

# IFTS et non tit

ifts.et.contractuel <- Paie[ Statut != "TITULAIRE"
                             & Statut != "STAGIAIRE"
                             & ifts.logical,
                             c(étiquette.matricule,
                               étiquette.année,
                               "Mois",
                               "Statut",
                               étiquette.code,
                               étiquette.libellé,
                               "Indice",
                               étiquette.montant),
                             with=FALSE]

nombre.lignes.ifts.et.contractuel <- nrow(ifts.et.contractuel)

#'
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau(c("Nombre de lignes de paye de contractuels percevant des IFTS", "Nombre de lignes IFTS pour IB < 380"), nombre.lignes.ifts.et.contractuel, nombre.lignes.ifts.anormales)

#'
#'[Lien vers la base de données Lignes IFTS pour contractuels](Bases/Réglementation/ifts.et.contractuel.csv)
#'[Lien vers la base de données Lignes IFTS pour IB < 380](Bases/Réglementation/lignes.ifts.anormales.csv)
#'
#'**Nota :**
#'IB < 380 : fonctionnaire percevant un indice brut inférieur à 380
#'
#'######
#'
#'## 5.5 Contrôle sur les heures supplémentaires

# Sont repérées comme heures supplémentaires ou complémentaires les heures dont le libellé obéissent à
# l'expression régulière expression.rég.heures.sup donnée par le fichier prologue.R

colonnes <- c(étiquette.matricule,
              étiquette.année,
              "Mois",
              "Statut",
              "Libellé",
              étiquette.code,
              "Heures",
              "Heures.Sup.",
              "Base",
              "Taux",
              "Montant",
              "Type")

HS.sup.25 <- Paie[Heures.Sup. >= 25, colonnes, with=FALSE]

# version 1 : 48 s
# HS.sup.25 <- merge(HS.sup.25,
#                    ddply(Paie,
#                          .(Matricule, Année, Mois),
#                          summarise,
#                          "Traitement indiciaire mensuel" = sum(montant.traitement.indiciaire),
#                          .progress = "tk"))

# version optimisée : 0,15 s soit x300


  HS.sup.indiciaire.mensuel <- HS.sup.25[Type == "T", .(Matricule, Année, Mois, Montant)]

  HS.sup.25 <-  HS.sup.25[Type %chin% c("I", "T", "R")
                            & ! grepl(".*SMIC.*",
                                      Libellé, ignore.case = TRUE)
                            & grepl(expression.rég.heures.sup,
                                    Libellé, ignore.case = TRUE), ]

  HS.sup.25 <- HS.sup.25[order(Matricule, Année, Mois), ]

# donne un tableau à 3 dimensions [Matricules, Années, Mois] dont les valeurs sont nommées par matricule
# bizarrement le hashage de la variable année se fait par charactère alors que le mois reste entier dans certaines exécutions et pas dana d'autres !
# Tout convertir en as.character() est plus prudent.

temp <- with(HS.sup.indiciaire.mensuel,
             tapply(Montant, list(Matricule, Année, Mois), FUN=sum))

traitement.indiciaire.mensuel <- unlist(Map(function(x, y, z) temp[x, y, z],
                                             as.character(HS.sup.indiciaire.mensuel$Matricule),
                                             as.character(HS.sup.indiciaire.mensuel$Année),
                                             as.character(HS.sup.indiciaire.mensuel$Mois)), use.names=FALSE)

HS.sup.25 <- merge(as.data.frame(HS.sup.25), data.frame(Matricule=HS.sup.indiciaire.mensuel$Matricule,
                                             Année=HS.sup.indiciaire.mensuel$Année,
                                             Mois=HS.sup.indiciaire.mensuel$Mois,
                                             "Traitement indiciaire mensuel"=traitement.indiciaire.mensuel), 
                                             by=c("Matricule", "Année", "Mois"))

rm(temp, traitement.indiciaire.mensuel, HS.sup.indiciaire.mensuel )

HS.sup.25 <- merge(HS.sup.25, Analyse.rémunérations[ , .(Matricule, Année, traitement.indiciaire)], by=c("Matricule", "Année"))

HS.sup.25 <- unique(HS.sup.25, by=NULL)

names(HS.sup.25) <- sub("traitement.indiciaire", "Traitement indiciaire annuel", names(HS.sup.25))

nombre.Lignes.paie.HS.sup.25 <- nrow(HS.sup.25)

# with(Base2,
#      ihts.anormales <<- Base2[! Code.catégorie %chin% c("B", "C") & substr(Code,1,2) %chin% c("19") & ! grepl(" ENS", Libellé), c(étiquette.matricule, étiquette.code, étiquette.libellé, étiquette.montant, "Code.catégorie")]
# )

ihts.anormales <- data.frame(NULL)
if (fichier.personnels.existe) nombre.ihts.anormales <- nrow(ihts.anormales) else nombre.ihts.anormales <- NA
#'
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau(c("Nombre de lignes HS en excès", "Nombre de lignes IHTS anormales"), nombre.Lignes.paie.HS.sup.25, nombre.ihts.anormales)

#'
#'[Lien vers la base de données Heures suplémentaires en excès : matricules](Bases/Réglementation/HS.sup.25.csv)
#'[Lien vers la base de données IHTS anormales](Bases/Réglementation/ihts.anormales.csv)
#'
#'**Nota :**
#'HS en excès : au-delà de 25 heures par mois
#'IHTS anormales : non attribuées à des fonctionnaires de catégorie B ou C.
#'
#'## 5.6 Contrôle sur les indemnités des élus
#'


rémunérations.élu <- Analyse.rémunérations[ indemnités.élu > minimum.positif,
                                            c(clé.fusion,
                                              "Année",
                                              "Emploi",
                                              "indemnités.élu",
                                              "autres.rémunérations",
                                              "rémunération.indemnitaire.imposable"),
                                            with=FALSE ]


rémunérations.élu <- rémunérations.élu[ , rémunération.indemnitaire.imposable := indemnités.élu +  rémunération.indemnitaire.imposable]

if (!fusionner.nom.prénom)
  rémunérations.élu <- merge(unique(matricules[ , .(Nom,  Matricule)], by=NULL),
                             rémunérations.élu,
                             by = étiquette.matricule,
                             all.y = TRUE,
                             all.x = FALSE)

names(rémunérations.élu) <- c(union(clé.fusion, "Nom"),
                              "Année",
                              "Emploi",
                              "Indemnités ",
                              "Autres ",
                              "Total ")

rémunérations.élu <- na.omit(rémunérations.élu)

#'
if (générer.table.élus)
{
    if (nrow(rémunérations.élu) > 0)
      kable(rémunérations.élu, row.names = FALSE)
} else {
  
   cat("Tableau des indemnités d'élu : non générée.")
}

#'
if (sauvegarder.bases.analyse)
  Sauv.base(file.path(chemin.dossier.bases, "Effectifs"),
            "matricules",
            fichier.personnels)

#'[Lien vers la base de données Rémunérations des élus](Bases/Réglementation/rémunérations.élu.csv)
#'

#'# Annexe

#'## Liens complémentaires
#'
#'[Lien vers la base de données des bulletins et lignes de paie](Bases/Paiements/Bulletins.paie.csv)
#'[Lien vers la base de données fusionnées des bulletins et lignes de paie](Bases/Paiements/Paie.csv)
#'
#'
#'[Lien vers le fichier des personnels](Bases/Effectifs/Catégories des personnels.csv)
#'
#'
#'# Tableau des personnels : renseigner la catégorie
#'
#'Utiliser les codes : A, B, C, ELU, AUTRES
#'
#'En cas de changement de catégorie en cours de période, utiliser la catégorie AUTRES
#'Cela peut conduire à modifier manuellement le fichier Catégories des personnels.csv
#'
if (générer.table.effectifs)
  kable(matricules, row.names = FALSE)
#'


# ------------------------------------------------------------------------------------------------------------------
#  Sauvegardes : enlever les commentaires en mode opérationnel
##

if (sauvegarder.bases.analyse) {

  sauv.bases(file.path(chemin.dossier.bases, "Rémunérations"),
             "Analyse.rémunérations",
             "Analyse.variations.synthèse",
             "Analyse.variations.par.exercice",
             "masses.premier.personnels",
             "masses.premier.élus",
             "masses.dernier.personnels",
             "masses.dernier.élus")

  sauv.bases(file.path(chemin.dossier.bases, "Effectifs"),
             "Bulletins.paie.nir.total.hors.élus",
             "Bulletins.paie.nir.fonctionnaires",
             "tableau.effectifs")

  sauv.bases(file.path(chemin.dossier.bases, "Réglementation"),
             "base.iat.ifts",
             "codes.ifts",
             "HS.sup.25",
             "ifts.et.contractuel",
             "ihts.anormales",
             "lignes.contractuels.et.vacations",
             "lignes.fonctionnaires.et.vacations",
             "lignes.ifts.anormales",
             "matricules.contractuels.et.vacations",
             "matricules.fonctionnaires.et.vacations",
             "NBI.aux.non.titulaires",
             "personnels.prime.informatique",
             "personnels.iat.ifts",
             "rémunérations.élu",
             "RI.et.vacations",
             "traitement.et.vacations")
}

if (sauvegarder.bases.origine)
  sauv.bases(file.path(chemin.dossier.bases, "Paiements"),
             "Paie",
             "Bulletins.paie")



