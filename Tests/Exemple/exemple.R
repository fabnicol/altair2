
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
#write.csv(unique(Paie[Paie$delta > 500 & Paie$Montant > 500 & Paie$Type %in% c("A", "I", "R", "AV", "A"), c("Type",  "Libellé", "Code"), with=F]), chemin("test.csv"))

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

installer.paquets(knitr, plyr, digest, colorspace, ggplot2, assertthat, yaml, gtools, utils, data.table)
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
library(plyr)
library(ggplot2)
library(assertthat)
library(gtools)

if (séparateur.liste != ",")
  stop("Pour les tables importées par data.table::fread, le séparateur de champs doit être ','")

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
  base.personnels.catégorie <- read.csv.skip(nom.fichier.personnels, séparateur.liste = séparateur.liste, séparateur.décimal = séparateur.décimal)
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
                      séparateur.liste = séparateur.liste,
                      séparateur.décimal = séparateur.décimal,
                      drop=1:3,
                      rapide = TRUE),
             silent = TRUE)

  if (inherits(res, 'try-error')) {
    res2 <- try(Read.csv("Lignes.paie",
                         lignes.paie,
                         colClasses = lignes.paie.classes.input.fallback,
                         colNames = lignes.paie.input.fallback,
                         séparateur.liste = séparateur.liste,
                         séparateur.décimal = séparateur.décimal,
                         rapide = TRUE),
                silent = TRUE)
  }

  c(res, res2)
}


importer.bases.via.xhl2csv <- function() {

  res <- try(Read.csv("Paie",
                      nom.table,
                      #                      colClasses = union(bulletins.paie.classes.input, lignes.paie.classes.input),
                      #                      colNames = bulletins.paie.input,
                      séparateur.liste = séparateur.liste,
                      séparateur.décimal = séparateur.décimal,
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


importer.bases.via.xhl2csv()

names(Paie) <- c("Année", "Mois", "Nom", "Prénom", "Matricule",
                  "Service", "Statut", "Temps.de.travail", "Heures.Sup.", "Heures",
                   "Indice", "Brut", "Net", "Net.à.Payer", "NBI", "Libellé", "Code", "Base",
                  "Taux", "Nb.Unité", "Montant", "Type", "Emploi", "Grade", "Nir")


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

                    
Bulletins.paie <- unique(Paie[ , c("Matricule", "Année", "Mois", "Temps.de.travail", "Statut", "Brut", "Net.à.Payer", champ.nir), with=FALSE], by = NULL)

Bulletins.paie <- Bulletins.paie[ ,   quotité   := ifelse(etp.égale.effectif | is.na(Temps.de.travail), 1,  Temps.de.travail / 100)]

Bulletins.paie <- Bulletins.paie[ ,   `:=`(Montant.net.eqtp  = ifelse(is.finite(a<-Net.à.Payer/quotité), a,  NA),
                                           Montant.brut.eqtp = ifelse(is.finite(a<-Brut/quotité), a,  NA))]
    
Bulletins.paie <- Bulletins.paie[ ,   `:=`(Statut.sortie   = Statut[length(Net.à.Payer)],
                                           nb.jours        = calcul.nb.jours.mois(Mois, Année[1])),
                                      key=c("Matricule", "Année")]

Bulletins.paie <- Bulletins.paie[ ,   `:=`(Montant.brut.annuel      = sum(Brut, na.rm=TRUE),
                                         Montant.brut.annuel.eqtp = sum(Montant.brut.eqtp * 365 / nb.jours, na.rm=TRUE),
                                         Montant.net.annuel.eqtp  = sum(Montant.net.eqtp * 365 / nb.jours, na.rm=TRUE),
                                         permanent                = nb.jours >= 365),
                                      key=c("Matricule", "Année")]

# Il y a des duplications occasionnelles à éclaircir (Romans)

Bulletins.paie <- unique(Bulletins.paie, by = NULL)

Paie <- merge(Bulletins.paie[ , c("Matricule", "Année", "Mois", "quotité", "Montant.net.eqtp", "Montant.brut.eqtp", "Montant.brut.annuel", "Montant.brut.annuel.eqtp", "Montant.net.annuel.eqtp", "Statut.sortie", "nb.jours", "permanent"), with=FALSE],
              Paie, 
              by=c("Année", "Mois", "Matricule"))

class(Paie) <- "data.frame"

Paie$quotité[is.na(Paie$quotité)] <- 0

message("Bulletins de Paie retraités")


###########  Analyse des rémunérations : base globale ###################

  Analyse.rémunérations <- ddply(Paie,
                                 c(clé.fusion, étiquette.année),
                                 summarise,
                                 Nir          = Nir[1],
                                 Montant.net.annuel.eqtp  = Montant.net.annuel.eqtp[1],
                                 Montant.brut.annuel = Montant.brut.annuel[1],
                                 Montant.brut.annuel.eqtp = Montant.brut.annuel.eqtp[1],
                                 Statut       = Statut[1],
                                 nb.jours     = nb.jours[1],
                                 permanent    = permanent[1],
                                 Emploi       = Emploi[1],
                                 Service      = Service[1],
                                 traitement.indiciaire   = sum(Montant[Type == "T"], na.rm = TRUE),
                                 sft          = sum(Montant[Type == "S"], na.rm = TRUE),
                                 indemnité.résidence = sum(Montant[Type == "IR"], na.rm = TRUE),
                                 indemnités   = sum(Montant[Type == "I"], na.rm = TRUE),
                                 rémunération.principale.contractuel = 0,
                                 rémunération.vacataire              = 0,
                                 rémunération.indemnitaire.imposable = Montant.brut.annuel - sft - indemnité.résidence - traitement.indiciaire,
                                 rémunération.indemnitaire.imposable.eqtp = ifelse(is.finite(q <- Montant.brut.annuel.eqtp/Montant.brut.annuel), 
                                                                                   q * rémunération.indemnitaire.imposable,
                                                                                   NA),
                                 autres.rémunérations                = sum(Montant[Type == "AC" | Type == "A" | Type == "AV"], na.rm = TRUE),
                                                                  
                                 # on ne considère que les rémunérations brutes (sans prise en compte des remboursements de frais aux salariés ou des régularisations)
                                 # pour être en homogénéïté avec la colonne Brut/Montant.brut.annuel

                                 # ici on ajoute les remboursements de frais professionnels (autres.rémunérations), il manque les rappels sur rémunérations 

                                 total.lignes.paie =  traitement.indiciaire + sft + indemnité.résidence + indemnités + autres.rémunérations,
                                 
                                 indemnités.élu = ifelse(Statut == "ELU", total.lignes.paie, 0),

                                 part.rémunération.indemnitaire =  ifelse(is.na(s <-  Montant.brut.annuel) | s == 0,
                                                                          NA,
                                                                          (rémunération.indemnitaire.imposable) / s * 100))


Analyse.rémunérations <- Analyse.rémunérations[! is.na(Analyse.rémunérations$Montant.brut.annuel), ]


if (length (Analyse.rémunérations$quotité[Analyse.rémunérations$quotité > 1]) > 0 & comportement.strict ) 
  stop("Détection de quotités > 1", call. = FALSE)

message("Analyse des rémunérations réalisée.")

Analyse.variations.par.exercice <- Analyse.rémunérations[ , c(clé.fusion, étiquette.année,
                                                              "Montant.net.annuel.eqtp",
                                                              "Montant.brut.annuel.eqtp",
                                                              "rémunération.indemnitaire.imposable.eqtp",
                                                              "Statut",
                                                              "nb.jours",
                                                              "permanent")]

Analyse.variations.par.exercice <- Analyse.variations.par.exercice[Analyse.variations.par.exercice$nb.jours > seuil.troncature, ]

Analyse.variations.synthèse <- ddply(Analyse.variations.par.exercice,
                                     clé.fusion,
                                     summarise,
                                     Nexercices = length(Année),
                                     total.jours = sum(nb.jours, na.rm = TRUE),
                                     Montant.net.annuel.eqtp.début  = Montant.net.annuel.eqtp[1],
                                     Montant.net.annuel.eqtp.sortie = Montant.net.annuel.eqtp[Nexercices],
                                     moyenne.rémunération.annuelle.sur.période =
                                       sum(Montant.net.annuel.eqtp, na.rm = TRUE)/length(Année[!is.na(Montant.net.annuel.eqtp) & Montant.net.annuel.eqtp > 0]),
                                     
                                     pris.en.compte = Nexercices > 1 
                                                        & ! is.na(Montant.net.annuel.eqtp.début)
                                                        & ! is.na(Montant.net.annuel.eqtp.sortie)
                                                        & Montant.net.annuel.eqtp.début  > 0 
                                                        & Montant.net.annuel.eqtp.sortie > 0
                                                        & total.jours >= 365,
                                     
                                     permanent = all(permanent),  
                                     
                                     variation.rémunération = ifelse(pris.en.compte,
                                                                      (Montant.net.annuel.eqtp.sortie / Montant.net.annuel.eqtp.début - 1)*100,
                                                                       NA),
                                     
                                     variation.moyenne.rémunération = ifelse(pris.en.compte,
                                                                             ((variation.rémunération/100 + 1)^(1 / (Nexercices - 1)) - 1) * 100,
                                                                             NA),

                                     variation.rémunération.normalisée = ifelse(durée.sous.revue == Nexercices
                                                                                & permanent,
                                                                                variation.rémunération,
                                                                                NA),
                                     
                                     variation.moyenne.rémunération.normalisée = ifelse(!is.na(variation.rémunération.normalisée),
                                                                                        variation.moyenne.rémunération,
                                                                                        NA),
                                     plus.2.ans  = (total.jours  >= 730),
                                     moins.2.ans = (total.jours < 730),
                                     moins.1.an  = (total.jours < 365),
                                     moins.six.mois = (total.jours < 183),
                                     statut = Statut[1])

q3 <- quantile(Analyse.variations.synthèse$variation.rémunération, c(quantile.cut/100, 1 - quantile.cut/100), na.rm=TRUE)

Analyse.variations.synthèse <- Analyse.variations.synthèse[is.na(Analyse.variations.synthèse$variation.rémunération) ||
                                                             (Analyse.variations.synthèse$variation.rémunération > q3[[1]]
                                                             & Analyse.variations.synthèse$variation.rémunération < q3[[2]]), ]

temp <- Analyse.variations.synthèse[Analyse.variations.synthèse$plus.2.ans, clé.fusion]

trouver.ligne <- function(x, y) anyDuplicated(rbind(x, y)) > 1

if (fusionner.nom.prénom) {

  Analyse.variations.par.exercice <- mutate(Analyse.variations.par.exercice,
                                            plus.2.ans = trouver.ligne(c(Nom, Prénom), temp))
} else {

  Analyse.variations.par.exercice <- mutate(Analyse.variations.par.exercice,
                                            plus.2.ans = Matricule %in% temp)
}

rm(temp)

message("Analyse des variations réalisée.")

Bulletins.paie.nir.total.hors.élus <- unique(Bulletins.paie[Bulletins.paie$Année == fin.période.sous.revue
                                           & Bulletins.paie$Mois == 12
                                           & Bulletins.paie$Statut != "ELU",
                                           c(clé.fusion, champ.nir), with = FALSE], by = NULL)


Bulletins.paie.nir.fonctionnaires  <- unique(Bulletins.paie[Bulletins.paie$Année == fin.période.sous.revue
                                                  & Bulletins.paie$Mois  == 12
                                                  & (Bulletins.paie$Statut == "TITULAIRE" |
                                                     Bulletins.paie$Statut == "STAGIAIRE"),
                                                  c(clé.fusion, champ.nir), with = FALSE], by = NULL)

names(Bulletins.paie.nir.total.hors.élus) <- c(clé.fusion, champ.nir)

class(Bulletins.paie.nir.total.hors.élus) <- "data.frame"
class(Bulletins.paie.nir.fonctionnaires)  <- "data.frame"

# Age au 31 décembre de l'exercice dernier.exerciceal de la période sous revue
# ne pas oublier [ ,...] ici:

années.fonctionnaires   <- extraire.nir(Bulletins.paie.nir.fonctionnaires)

années.total.hors.élus  <- extraire.nir(Bulletins.paie.nir.total.hors.élus)

message("Analyse démographique réalisée.")

if (!is.null(Paie) & !is.null(Analyse.rémunérations)
    & !is.null(Analyse.variations.synthèse) & !is.null(Analyse.variations.par.exercice))
  message("Statistiques de synthèse réalisées")

class(Paie) <- "data.frame"

########### Démographie ########################

#'# 1. Statistiques de population
#'
#'### 1.1 Effectifs

liste.années <- as.character(période)

effectifs <- lapply(période,
                    function(x) {
                      A <- Bulletins.paie[Bulletins.paie$Année == x, c("Matricule", "Statut",  "permanent", "quotité"), with=FALSE]
                      E <- unique(A[ , c("Matricule", "permanent"), with=FALSE], by = NULL)
                      ETP <- unique(Bulletins.paie[Bulletins.paie$Matricule %in% E[[1]], c("quotité", "Matricule", "Mois"), with=FALSE], by = NULL)
                      F <- E[E$permanent, ]
                      G <- unique(A[A$Statut == "TITULAIRE" | A$Statut == "STAGIAIRE", c("Matricule", "permanent"), with=FALSE], by = NULL)
                      H <- G[G$permanent, ]
                      I <- unique(A[A$Statut == "ELU", c("Matricule", "permanent"), with=FALSE], by = NULL)
                      J <- I[I$permanent, ]
                      c(nrow(E), nrow(F), nrow(G), nrow(H), nrow(I), nrow(J), sum(ETP[[1]], na.rm=TRUE)/12)
                      
                    })

effectifs <- lapply(effectifs, function(x) formatC(x, big.mark = " ", format="f", digits=1, decimal.mark=","))

tableau.effectifs <- as.data.frame(effectifs, row.names = c("Total effectifs", "  dont présents 12 mois", "  dont fonctionnaires",
                                                            "  dont fonct. présents 12 mois", "  dont élus", "  dont élus présents 12 mois", "Total ETP/année"))

names(tableau.effectifs) <- liste.années
#'
kable(tableau.effectifs, row.names = TRUE, align='c')
#'
#'
class(Bulletins.paie) <- "data.frame"

#'[Lien vers la base des effectifs](Bases/Effectifs/tableau.effectifs.csv)
#'
#'
#'### 1.2 Pyramide des âges, personnels non élus

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

Résumé(c("Âge des personnels <br>au 31/12/" %+% fin.période.sous.revue,
         "Effectif"),
       années.total.hors.élus,
       extra = "length",
       align = 'c')

#'
#'
#'### 1.3 Pyramide des âges, personnels fonctionnaires stagiaires et titulaires

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

Résumé(c("Âge des personnels <br>au 31/12/" %+% fin.période.sous.revue,
         "Effectif"),
       années.fonctionnaires,
       extra = "length",
       align = 'c')

#'
#'### 1.4 Effectifs des personnels par durée de service
#'
#'**Personnels en fonction des exercices `r début.période.sous.revue` à `r fin.période.sous.revue` inclus :**
#'
attach(Analyse.variations.synthèse)

Tableau(c("Plus de 2 ans",
          "Moins de 2 ans",
          "Moins d'un an",
          "Moins de six mois"),
        sum(plus.2.ans, na.rm=TRUE),
        sum(moins.2.ans, na.rm=TRUE),
        sum(moins.1.an, na.rm=TRUE),
        sum(moins.six.mois, na.rm=TRUE))

#'

if (nrow(Analyse.variations.par.exercice) > 0)
  qplot(factor(Année),
        data = Analyse.variations.par.exercice,
        geom = "bar",
        fill = factor(!plus.2.ans),
        main = "Evolutions entre " %+% début.période.sous.revue %+% " et " %+% fin.période.sous.revue,
        xlab = étiquette.année,
        ylab = "Effectif",
        asp = 1.4)        +
  scale_fill_discrete(name = "Personnels en fonction",
                      breaks = c(TRUE, FALSE),
                      labels = c("Moins de deux ans", "Plus de deux ans"))

#'
#'#### Effectifs
#'

effectifs.var <- lapply(période,
                        function(x) {

                          E <- unique(Analyse.variations.par.exercice[Analyse.variations.par.exercice$Année == x , c("Matricule", "plus.2.ans")])
                          F <- E[E$plus.2.ans == TRUE, ]
                          tot <- nrow(E)
                          plus.2.ans <- nrow(F)
                          résultat <- c(plus.2.ans, tot - plus.2.ans, tot)
                          rm(E, F, tot, plus.2.ans)
                          résultat
                        })

effectifs.var <- prettyNum(effectifs.var, big.mark = " ")
tableau.effectifs.var <- as.data.frame(effectifs.var, row.names = c("Plus de deux ans", "Moins de deux ans", "Total"))

names(tableau.effectifs.var) <- liste.années
#'
kable(tableau.effectifs.var, row.names = TRUE, align='c')
#'

detach(Analyse.variations.synthèse)

#'
#'**Nota:**
#'Personnels en place : ayant servi au moins 730 jours pendant la période.
#'Dans les statistiques de cette section, les élus ne sont pas pris en compte.
#'

# on ne compte pas les élus dans le total (voir 5.6)

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


Analyse.rémunérations.premier.exercice <- Analyse.rémunérations[Analyse.rémunérations$Année == année, ]

if (fichier.personnels.existe)
{
  Analyse.rémunérations.premier.exercice <- merge(Analyse.rémunérations.premier.exercice,
                                                  base.personnels.catégorie,
                                                  by = clé.fusion, all = FALSE)

  colonnes.sélectionnées <- c(colonnes.sélectionnées, "Catégorie")
}

attach(Analyse.rémunérations.premier.exercice, warn.conflicts = FALSE)

########### Analyse statique premier exercice ########################

#'# 2. Rémunérations brutes : analyse pour l'exercice `r année`    
#'    
#'## 2.1 Statistiques de position globales     
#'     

masses.premier.personnels <- colSums(Analyse.rémunérations.premier.exercice[Statut != "ELU",
                                                                            c("Montant.brut.annuel",
                                                                           "rémunération.indemnitaire.imposable",
                                                                           "indemnités.élu",
                                                                           "total.lignes.paie",
                                                                           "autres.rémunérations")])
  
masses.premier.élus <- colSums(Analyse.rémunérations.premier.exercice[Statut == "ELU",
                                                                      c("Montant.brut.annuel",
                                                                        "rémunération.indemnitaire.imposable",
                                                                        "indemnités.élu",
                                                                        "total.lignes.paie",
                                                                        "autres.rémunérations")])

#'### Cumuls des rémunérations brutes pour l'exercice `r année`
#'  
#'*Personnels*     

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Brut annuel (bulletins)",
                    "Brut annuel (lignes), dont :",
                    "\\ \\ Indemnités imposables :",
                    "\\ \\ Autres rémunérations"),
                  c(masses.premier.personnels["Montant.brut.annuel"],
                  masses.premier.personnels["total.lignes.paie"],
                  masses.premier.personnels["rémunération.indemnitaire.imposable"],
                  masses.premier.personnels["autres.rémunérations"]))

#'  
#'*Elus*    
#'  

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Brut annuel (bulletins)",
                    "Brut annuel (lignes), dont :",
                    "\\ \\ Indemnités imposables :",
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
#'  *Indemnités imposables*     : indemnités sauf remboursements, certaines IJSS, indemnités d'élu      
#'  *Indemnités d'élu*         : toutes rémunérations indemnitaires des élus    
#'  *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
#'  

#'**Tests de cohérence**
#'
#'Somme des rémunérations brutes versées aux personnels (non élus) :  
#'  

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Bulletins de paie (euros)",
                    "Lignes de paie (euros)",
                    "Différence (euros)"),
                  c(masses.premier.personnels["Montant.brut.annuel"],
                    masses.premier.personnels["total.lignes.paie"],
                    masses.premier.personnels["Montant.brut.annuel"] -
                    masses.premier.personnels["total.lignes.paie"]))

#'
#'à comparer aux soldes des comptes 641 et 648 du compte de gestion.
#'
#'Somme des rémunérations brutes versées (élus) :  
#'  

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Bulletins de paie (euros)",
                    "Lignes de paie (euros)",
                    "Différence (euros)"),
                  c(masses.premier.élus["Montant.brut.annuel"],
                    masses.premier.élus["total.lignes.paie"],
                    masses.premier.élus["Montant.brut.annuel"] -
                    masses.premier.élus["total.lignes.paie"]))

#'
#'Les résultats sont exprimés en euros.
#'

#'   
#'## 2.2 Fonctionnaires titulaires et stagiaires   
#'

filtre.fonctionnaire <- function (X) X[ !is.na(X) & (Statut == "TITULAIRE" | Statut == "STAGIAIRE") & X >0 ]

AR <- Analyse.rémunérations.premier.exercice[Statut == "TITULAIRE" | Statut == "STAGIAIRE", colonnes.sélectionnées]

source("histogrammes.R", encoding = "UTF-8")
histogrammes()

#'
#'**Effectif : `r nrow(AR)`**
#'
#'**Tests de cohérence**

if (nrow(AR) > 0) {
  masses.premier <- colSums(AR[ ,c("Montant.brut.annuel", "rémunération.indemnitaire.imposable", "total.lignes.paie", "autres.rémunérations")])
} else {
  masses.premier <- c(0,0) 
}

#'Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :
#'

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Brut annuel (bulletins)",
                    "Brut annuel (lignes), dont :",
                    "\\ \\ Indemnités imposables :",
                    "\\ \\ Autres rémunérations"),
                  c(masses.premier["Montant.brut.annuel"],
                    masses.premier["total.lignes.paie"],
                    masses.premier["rémunération.indemnitaire.imposable"],
                    masses.premier["autres.rémunérations"]))

#'
#'**Définitions :**
#'
#'  *Brut annuel (bulletins)*   : somme du champ *Brut*
#'  *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :  
#'  *Indemnités imposables*     : champ *Brut* moins le SFT, le traitement indiciaire et l'indemnité de résidence 
#'  *Indemnités d'élus*         : toutes rémunérations indemnitaires des élus    
#'  *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
#'
#'**Tests de cohérence**
#'
#'Somme des rémunérations brutes versées aux personnels (non élus) :
#'

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Bulletins de paie (euros)",
                    "Lignes de paie (euros)",
                    "Différence (euros)"),
                  c(masses.premier["Montant.brut.annuel"],
                    masses.premier["total.lignes.paie"],
                    masses.premier["Montant.brut.annuel"] -
                    masses.premier["total.lignes.paie"]))


#'
#'A comparer aux soldes des comptes 6411, 6419 et 648 du conmpte de gestion.
#'
#'
#'### Statistiques de position pour l'exercice `r année`
#'

Résumé(c("Traitement indiciaire",
         "Indemnités imposables",
         "Autres rémunérations",
         "Effectif"),
       AR[c("traitement.indiciaire",
            "rémunération.indemnitaire.imposable",
            "autres.rémunérations")],
       extra = "length")


#'
Résumé(c("Total lignes hors rappels",
         "Total brut",
         "Total brut EQTP",
         "Part de la rém. indemnitaire",
         "Effectif"),
       AR[c("total.lignes.paie",
            "Montant.brut.annuel",
            "Montant.brut.annuel.eqtp",
            "part.rémunération.indemnitaire")],
       extra = "length")

#'### Statistiques de position par catégorie pour l'exercice `r année`
#'

ARA <- AR[AR$Catégorie == 'A', ]
ARB <- AR[AR$Catégorie == 'B', ]
ARC <- AR[AR$Catégorie == 'C', ]

#'**Catégorie A**
#'
#'

if (fichier.personnels.existe)
{
  Résumé(c("Traitement indiciaire",
           "Indemnités imposables",
           "Autres rémunérations"),
         ARA[c("traitement.indiciaire",
               "rémunération.indemnitaire.imposable",
               "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
  Résumé(c("Total rémunérations", 
           "Total rémunérations EQTP", 
           "Part de la rémunération indemnitaire"),
         ARA[c( "Montant.brut.annuel",
                "Montant.brut.annuel.eqtp",
                "part.rémunération.indemnitaire")])
}

#'
#'**Effectif : `r nrow(ARA)`**
#'
#'**Catégorie B**
#'

if (fichier.personnels.existe)
{
  Résumé(c("Traitement indiciaire",
           "Indemnités imposables",
           "Autres rémunérations"),
         ARB[ c("traitement.indiciaire",
                "rémunération.indemnitaire.imposable",
                "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
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
  Résumé(c("Traitement indiciaire",
           "Indemnités imposables",
           "Autres rémunérations"),
         ARC[ c("traitement.indiciaire",
                "rémunération.indemnitaire.imposable",
                "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
  Résumé(c("Total rémunérations",
           "Total rémunérations EQTP",
           "Part de la rémunération indemnitaire"),
         ARC[ c( "Montant.brut.annuel",
                 "Montant.brut.annuel.eqtp",
                 "part.rémunération.indemnitaire") ])
}

#'**Effectif : `r nrow(ARC)`**
#'
#'
#'######      <br>
#'
#'## 2.3 Contractuels, vacataires et stagiaires inclus   
#'   

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
#'**Nota:**
#'Ne sont retenues que les rémunérations supérieures à 1 000 euros.
#'Les élus ne sont pas pris en compte.
#'

temp <- positive(autres.rémunérations)

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
                                             colonnes.sélectionnées]

#'
Résumé(c("Indemnités imposables",
         "Autres rémunérations",
         "Effectif"),
       AR[c("rémunération.indemnitaire.imposable",
            "autres.rémunérations")],
       extra = "length")

#'

Résumé(c("Total rémunérations",
         "Total rémunérations EQTP",
         "Effectif"),
       AR[c("Montant.brut.annuel", "Montant.brut.annuel.eqtp")],
       extra = "length")
#'

nom.base.analyse <- "Analyse.rémunérations.premier.exercice"

detach(Analyse.rémunérations.premier.exercice)

# pour année fin #

année <- fin.période.sous.revue

###########  Analyse des rémunérations  fin de période###################
#  ATTENTION : les années doivent être SUCCESSIVES                      #


Analyse.rémunérations.dernier.exercice <- Analyse.rémunérations[Analyse.rémunérations$Année == année, ]

if (fichier.personnels.existe)
{
  Analyse.rémunérations.dernier.exercice <- merge(Analyse.rémunérations.dernier.exercice, base.personnels.catégorie, by = clé.fusion, all = FALSE)
}

attach(Analyse.rémunérations.dernier.exercice, warn.conflicts = FALSE)

########### Analyse statique dernier exercice ########################

#'# 3. Rémunérations brutes : analyse pour l'exercice `r année`    
#'   
#'## 3.1 Statistiques de position globales (tous statuts)    
#'   

masses.dernier.personnels <- colSums(Analyse.rémunérations.dernier.exercice[Statut != "ELU",
                                                                            c("Montant.brut.annuel",
                                                                              "rémunération.indemnitaire.imposable",
                                                                              "indemnités.élu",
                                                                              "total.lignes.paie",
                                                                              "autres.rémunérations")])

masses.dernier.élus <- colSums(Analyse.rémunérations.dernier.exercice[Statut == "ELU",
                                                                      c("Montant.brut.annuel",
                                                                        "rémunération.indemnitaire.imposable",
                                                                        "indemnités.élu",
                                                                        "total.lignes.paie",
                                                                        "autres.rémunérations")])

#'### Cumuls des rémunérations brutes pour l'exercice `r année`
#'  
#'*Personnels*     

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Brut annuel (bulletins)",
                    "Brut annuel (lignes), dont :",
                    "\\ \\ Indemnités imposables :",
                    "\\ \\ Autres rémunérations"),
                  c(masses.dernier.personnels["Montant.brut.annuel"],
                    masses.dernier.personnels["total.lignes.paie"],
                    masses.dernier.personnels["rémunération.indemnitaire.imposable"],
                    masses.dernier.personnels["autres.rémunérations"]))

#'  
#'*Elus*    
#'  

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Brut annuel (bulletins)",
                    "Brut annuel (lignes), dont :",
                    "\\ \\ Indemnités imposables :",
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
#'  *Indemnités imposables*     : indemnités sauf remboursements, certaines IJSS, indemnités d'élu      
#'  *Indemnités d'élu*         : toutes rémunérations indemnitaires des élus    
#'  *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
#'  

#'**Tests de cohérence**
#'
#'Somme des rémunérations brutes versées aux personnels (non élus) :  
#'  

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Bulletins de paie (euros)",
                    "Lignes de paie (euros)",
                    "Différence (euros)"),
                  c(masses.dernier.personnels["Montant.brut.annuel"],
                    masses.dernier.personnels["total.lignes.paie"],
                    masses.dernier.personnels["Montant.brut.annuel"] -
                      masses.dernier.personnels["total.lignes.paie"]))

#'
#'à comparer aux soldes des comptes 641 et 648 du compte de gestion.
#'
#'Somme des rémunérations brutes versées (élus) :  
#'  

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Bulletins de paie (euros)",
                    "Lignes de paie (euros)",
                    "Différence (euros)"),
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

filtre.fonctionnaire <- function (X) X[ (Statut == "TITULAIRE" | Statut == "STAGIAIRE") & X >0 ]

AR <- Analyse.rémunérations.dernier.exercice[Statut == "TITULAIRE" | Statut == "STAGIAIRE", colonnes.sélectionnées]

source("histogrammes.R", encoding = "UTF-8")
histogrammes()

#'
#'**Effectif : `r nrow(AR)`**
#'
#'**Tests de cohérence**

if (nrow(AR) > 0) {
  masses.dernier <- colSums(AR[ ,c("Montant.brut.annuel", "rémunération.indemnitaire.imposable", "total.lignes.paie", "autres.rémunérations")])
} else {
  masses.dernier <- c(0,0) 
}

#'Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :
#'

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Brut annuel (bulletins)",
                    "Brut annuel (lignes), dont :",
                    "\\ \\ Indemnités imposables :",
                    "\\ \\ Autres rémunérations"),
                  c(masses.dernier["Montant.brut.annuel"],
                    masses.dernier["total.lignes.paie"],
                    masses.dernier["rémunération.indemnitaire.imposable"],
                    masses.dernier["autres.rémunérations"]))

#'
#'**Définitions :**
#'
#'  *Brut annuel (bulletins)*   : somme du champ *Brut*
#'  *Brut annuel (lignes)*      : somme du champ *Montant* des lignes de paye, dont :  
#'  *Indemnités imposables*     : champ *Brut* moins le SFT, le traitement indiciaire et l'indemnité de résidence 
#'  *Indemnités d'élus*         : toutes rémunérations indemnitaires des élus    
#'  *Autres rémunérations*      : acomptes, retenues sur brut, rémunérations diverses, rappels   
#'
#'**Tests de cohérence**
#'
#'Somme des rémunérations brutes versées aux personnels (non élus) :
#'

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Bulletins de paie (euros)",
                    "Lignes de paie (euros)",
                    "Différence (euros)"),
                  c(masses.dernier["Montant.brut.annuel"],
                    masses.dernier["total.lignes.paie"],
                    masses.dernier["Montant.brut.annuel"] -
                      masses.dernier["total.lignes.paie"]))


#'
#'A comparer aux soldes des comptes 6411, 6419 et 648 du conmpte de gestion.
#'
#'
#'### Statistiques de position pour l'exercice `r année`
#'

Résumé(c("Traitement indiciaire",
         "Indemnités imposables",
         "Autres rémunérations",
         "Effectif"),
       AR[c("traitement.indiciaire",
            "rémunération.indemnitaire.imposable",
            "autres.rémunérations")],
       extra = "length")


#'
Résumé(c("Total lignes hors rappels",
         "Total brut",
         "Total brut EQTP",
         "Part de la rém. indemnitaire",
         "Effectif"),
       AR[c("total.lignes.paie",
            "Montant.brut.annuel",
            "Montant.brut.annuel.eqtp",
            "part.rémunération.indemnitaire")],
       extra = "length")

#'### Statistiques de position par catégorie pour l'exercice `r année`
#'

ARA <- AR[AR$Catégorie == 'A', ]
ARB <- AR[AR$Catégorie == 'B', ]
ARC <- AR[AR$Catégorie == 'C', ]

#'**Catégorie A**
#'
#'

if (fichier.personnels.existe)
{
  Résumé(c("Traitement indiciaire",
           "Indemnités imposables",
           "Autres rémunérations"),
         ARA[c("traitement.indiciaire",
               "rémunération.indemnitaire.imposable",
               "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
  Résumé(c("Total rémunérations", 
           "Total rémunérations EQTP",
           "Part de la rémunération indemnitaire"),
         ARA[c( "Montant.brut.annuel",
                "Montant.brut.annuel.eqtp",
                "part.rémunération.indemnitaire")])
}

#'
#'**Effectif : `r nrow(ARA)`**
#'
#'**Catégorie B**
#'

if (fichier.personnels.existe)
{
  Résumé(c("Traitement indiciaire",
           "Indemnités imposables",
           "Autres rémunérations"),
         ARB[ c("traitement.indiciaire",
                "rémunération.indemnitaire.imposable",
                "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
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
  Résumé(c("Traitement indiciaire",
           "Indemnités imposables",
           "Autres rémunérations"),
         ARC[ c("traitement.indiciaire",
                "rémunération.indemnitaire.imposable",
                "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
  Résumé(c("Total rémunérations",
           "Total rémunérations EQTP",
           "Part de la rémunération indemnitaire"),
         ARC[ c( "Montant.brut.annuel",
                 "Montant.brut.annuel.eqtp",
                 "part.rémunération.indemnitaire") ])
}

#'**Effectif : `r nrow(ARC)`**
#'
#'
#'######      <br>
#'
#'## 3.3 Contractuels, vacataires et stagiaires inclus   
#'   

temp <- rémunération.indemnitaire.imposable.eqtp[!is.na(rémunération.indemnitaire.imposable.eqtp) &
                                                  Statut != "ELU"
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
#'**Nota:**
#'Ne sont retenues que les rémunérations supérieures à 1 000 euros.
#'Les élus ne sont pas pris en compte.
#'

temp <- positive(autres.rémunérations)

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
                                             colonnes.sélectionnées]

#'
Résumé(c("Indemnités imposables",
         "Autres rémunérations",
         "Effectif"),
       AR[c("rémunération.indemnitaire.imposable",
            "autres.rémunérations")],
       extra = "length")

#'

Résumé(c("Total rémunérations",
         "Total rémunérations EQTP",
         "Effectif"),
       AR[c("Montant.brut.annuel", "Montant.brut.annuel.eqtp")],
       extra = "length")
#'

nom.base.analyse <- "Analyse.rémunérations.dernier.exercice"

detach(Analyse.rémunérations.dernier.exercice)
#'
#'[Lien vers la base de données](Bases/Rémunérations/Analyse.rémunérations.csv) d'analyse des rémunérations
#'


########### Analyse dynamique ########################
#'
#'# 4. Rémunérations nettes : évolutions sur la période `r début.période.sous.revue` - `r fin.période.sous.revue`    
#'
#'Nombre d'exercices: `r durée.sous.revue`   
#'  
#'**Les données présentées dans cette section sont toutes relatives à des rémunérations nettes en équivalent temps plein (EQTP)**      
#'  
#'## 4.1 Rémunération nette moyenne sur la période    

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

temp <- na.omit(moyenne.rémunération.annuelle.sur.période[  moyenne.rémunération.annuelle.sur.période > 0
                                                            & (statut == "TITULAIRE"  | statut == "STAGIAIRE")] / 1000)

if (longueur.non.na(temp) > 0)
  hist(temp,
       xlab = "Sur la période "%+% début.période.sous.revue %+% "-" %+% fin.période.sous.revue %+% " en milliers d'euros",
       ylab = "Effectif",
       main = "Rémunération nette moyenne des fonctionnaires",
       col = "blue",
       nclass = 200)

#'
#'[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)
#'
#'**Nota:** La rémunération nette perçue est rapportée au cumul des jours d'activité.

Analyse.variations.synthèse.filtrée <- Analyse.variations.synthèse[total.jours > seuil.troncature
                                                                   & statut !=  "ELU",
                                                                   c("Montant.net.annuel.eqtp.début",
                                                                     "Montant.net.annuel.eqtp.sortie",
                                                                     "moyenne.rémunération.annuelle.sur.période",
                                                                     "variation.rémunération",
                                                                     "variation.moyenne.rémunération",
                                                                     "variation.rémunération.normalisée",
                                                                     "variation.moyenne.rémunération.normalisée",
                                                                     "plus.2.ans",
                                                                     clé.fusion)]

Analyse.variations.synthèse.filtrée.plus.2.ans  <- Analyse.variations.synthèse.filtrée[Analyse.variations.synthèse.filtrée$plus.2.ans, ]
Analyse.variations.synthèse.filtrée.moins.2.ans <- Analyse.variations.synthèse.filtrée[! Analyse.variations.synthèse.filtrée$plus.2.ans, ]


detach(Analyse.variations.synthèse)
#'
#'## 4.2 Evolutions des rémunérations nettes sur la période `r début.période.sous.revue` - `r fin.période.sous.revue`   
#'
#'### 4.2.1 Ensemble des personnels fonctionnaires et non titulaires   
#'
#'

f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[Analyse.variations.par.exercice$Année == x,
                                                               "Montant.net.annuel.eqtp"], na.rm = TRUE)/ 1000,
                           big.mark = " ",
                           digits = 5,
                           format = "fg")
#'
#'#### Rémunération
#'

Tableau.vertical(c(étiquette.année, "Rémunération nette totale (k&euro;)"),
                 période,
                 f)


#'
#'[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.par.exercice.csv)
#'
#'######
#'

Résumé(c("Première année",
         "Dernière année",
         "Moyenne sur la période <br>d'activité"),
       Analyse.variations.synthèse.filtrée[c("Montant.net.annuel.eqtp.début",
                                             "Montant.net.annuel.eqtp.sortie",
                                             "moyenne.rémunération.annuelle.sur.période")])

#'

Résumé(c("Variation sur la période <br>d'activité (%)",
         "Variation annuelle moyenne (%)",
         "Effectif"),
       Analyse.variations.synthèse.filtrée[c("variation.rémunération",
                                             "variation.moyenne.rémunération")],
       extra = "length")

#'

Résumé(c("Variation normalisée (%)",
         "Variation annuelle moyenne normalisée (%)",
         "Effectif"),
       Analyse.variations.synthèse.filtrée[ c("variation.rémunération.normalisée",
                                              "variation.moyenne.rémunération.normalisée")],
       extra = "length")

#'
#'[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.filtrée.csv)
#'
#'### 4.2.2 Personnels fonctionnaires et non titulaires en place   
#'   

if (nrow(Analyse.variations.synthèse.filtrée.plus.2.ans) > 0)
  hist(Analyse.variations.synthèse.filtrée.plus.2.ans$variation.moyenne.rémunération,
       xlab ="Variation annuelle moyenne en %",
       las = 1,
       xlim = c(-5,30),
       ylab ="Effectifs",
       main ="Rémunération nette des personnels en place",
       col ="blue",
       nclass=1000,
       xaxt = 'n')

try(axis(side=1, at=seq(-5,30, 1), labels=seq(-5,30,1), lwd=2))

#'
#'

f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[Analyse.variations.par.exercice$Année == x
                                                               & Analyse.variations.par.exercice$plus.2.ans,
                                                               "Montant.net.annuel.eqtp"], na.rm = TRUE)/ 1000,
                           big.mark = " ",
                           digits = 5,
                           format = "fg")

Tableau.vertical(c(étiquette.année, "Rémunération nette totale <br>des agents en place (k&euro;)"),
                 période,
                 f)

#'

Résumé(c("Première année",
         "Dernière année",
         "Moyenne sur la période <br>d'activité",
         "Effectif"),
       Analyse.variations.synthèse.filtrée.plus.2.ans[1:3],
       extra = "length")

#'

Résumé(c("Variation sur la période <br>d'activité (%)",
         "Variation annuelle moyenne (%)",
         "Effectif"),
       Analyse.variations.synthèse.filtrée.plus.2.ans[4:5],
       extra = "length")
#'
#'

Résumé(c("Variation normalisée (%)",
         "Variation annuelle moyenne normalisée (%)",
         "Effectif"),
       Analyse.variations.synthèse.filtrée.plus.2.ans[ c("variation.rémunération.normalisée",
                                                         "variation.moyenne.rémunération.normalisée")],
       extra = "length")

#'
#'
#'[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.filtrée.plus.2.ans.csv)
#'
#'**Nota**
#'Personnels en place : en fonction au moins 730 jours sur la période `r début.période.sous.revue` à `r fin.période.sous.revue`
#'
#'######
#'
#'### 4.2.3 Personnels fonctionnaires et non titulaires en fonction moins de deux ans   
#'
#'

if (nrow(na.omit(Analyse.variations.synthèse.filtrée.moins.2.ans)) > 0)
  hist(Analyse.variations.synthèse.filtrée.moins.2.ans$variation.moyenne.rémunération,
       xlab ="Variation annuelle moyenne en %",
       xlim = c(-10,30),
       las = 1,
       ylab ="Effectifs",
       main ="Rémunération nette des personnels en fonction moins de deux ans",
       col ="turquoise",
       nclass = 800)

#'
#'##
#'

f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[Analyse.variations.par.exercice$Année == x
                                                               & ! Analyse.variations.par.exercice$plus.2.ans,
                                                               "Montant.net.annuel.eqtp"], na.rm = TRUE)/ 1000,
                           big.mark = " ",
                           digits = 5,
                           format = "fg")

Tableau.vertical(c(étiquette.année, "Rémunération nette totale <br>des agents en fonction moins de deux ans (k&euro;)"),
                 période,
                 f)

#'
#'

Résumé(c("Première année",
         "Dernière année",
         "Moyenne sur la période <br>d'activité",
         "Effectif"),
       Analyse.variations.synthèse.filtrée.moins.2.ans[1:3],
       extra = "length")

#'

Résumé(c("Variation sur la période <br>d'activité (%)",
         "Variation annuelle moyenne (%)",
         "Effectif"),
       Analyse.variations.synthèse.filtrée.moins.2.ans[4:5],
       extra = "length")
#'
#'

if (durée.sous.revue < 2)
  Résumé(c("Variation normalisée (%)",
           "Variation annuelle moyenne normalisée (%)",
           "Effectif"),
         Analyse.variations.synthèse.filtrée.moins.2.ans[c("variation.rémunération.normalisée",
                                                           "variation.moyenne.rémunération.normalisée")],
         extra = "length")

#'
########### Tests statutaires ########################
#'
#'[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.filtrée.moins.2.ans.csv)
#'
#'**Nota:** Au 4.2 seuls sont pris en compte les personnels en fonction au moins 100 jours la première et la dernière année
#'Ne sont pas pris en compte les élus, stagiaires et apprentis ou assimilés.
#'
#'Les résultats sont exprimés en euros.
#'
#'# 5. Tests réglementaires   
#'## 5.1 Contrôle des heures supplémentaires, des NBI et primes informatiques   
#'   

attach(Paie, warn.conflicts = FALSE)

NBI.aux.non.titulaires <- Paie[Statut != "TITULAIRE"
                               & Statut != "STAGIAIRE"
                               & as.character(Code) %in% codes.NBI,
                               c(étiquette.matricule,
                                 "Statut",
                                 étiquette.code,
                                 étiquette.libellé,
                                 "Mois",
                                 étiquette.montant)]

nombre.Lignes.paie.NBI.nontit <- nrow(NBI.aux.non.titulaires)

# Prime de fonctions informatiques : pas dans la base de VLB
# on cherche la chaine de char. "INFO" dans les libellés de primes

# variante : filtre <- regexpr(".*(INFO|PFI|P.F.I).*", toupper(Paie$Libellé)) et regmatches(Paie$Libellé, filtre)

filtre <- grep(".*(INFO|PFI|P.F.I).*", Libellé)

personnels.prime.informatique <- Paie[ filtre,
                                       c(étiquette.matricule,
                                         "Statut",
                                         étiquette.code,
                                         étiquette.libellé,
                                         étiquette.montant)]

primes.informatiques.potentielles <- unique(Libellé[filtre])

if  (length(primes.informatiques.potentielles) == 0)
  primes.informatiques.potentielles <- "aucune"

nombre.personnels.pfi <- nrow(personnels.prime.informatique)

#'Primes informatiques potentielles : `r primes.informatiques.potentielles`

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
#'#
#'
#'## 5.2 Contrôle des vacations pour les fonctionnaires

# Vacations et statut de fonctionnaire

if (exists("Codes.paiement.vacations")) {

  lignes.fonctionnaires.et.vacations <- Paie[ (Statut == "TITULAIRE" | Statut == "STAGIAIRE")
                                              & Codes.paiement.vacations[Code] != 0,
                                              c(étiquette.matricule,
                                                "Nom", "Prénom",
                                                "Statut",
                                                étiquette.code,
                                                étiquette.libellé,
                                                étiquette.montant)]

  matricules.fonctionnaires.et.vacations <- unique(lignes.fonctionnaires.et.vacations[c("Matricule", "Nom", "Prénom")])
  nombre.fonctionnaires.et.vacations <- nrow(matricules.fonctionnaires.et.vacations)
  nombre.Lignes.paie.fonctionnaires.et.vacations <- nrow(lignes.fonctionnaires.et.vacations)
}

#'

if (exists("nombre.fonctionnaires.et.vacations"))
  Tableau(
    c("Nombre de FEV",
      "Nombre de lignes de vacations pour FEV"),
    nombre.fonctionnaires.et.vacations,
    nombre.Lignes.paie.fonctionnaires.et.vacations)


#'
#'[Lien vers la base de données Matricules des FEV](Bases/Réglementation/matricules.fonctionnaires.et.vacations.csv)
#'[Lien vers la base de données Lignes de vacations de FEV](Bases/Réglementation/lignes.fonctionnaires.et.vacations.csv)
#'
#'**Nota:**
#'FEV : fonctionnaire effectuant des vacations
#'
#'## 5.3 Contrôles sur les cumuls traitement indiciaire, indemnités et vacations des contractuels

# Vacations et régime indemnitaire

if (exists("Codes.paiement.vacations")) {

  lignes.contractuels.et.vacations <- Paie[Statut != "TITULAIRE"
                                           & Statut != "STAGIAIRE"
                                           & Codes.paiement.vacations[Code],
                                           c(étiquette.matricule,
                                             "Nom", "Prénom",
                                             étiquette.code,
                                             étiquette.libellé,
                                             étiquette.montant)]


  matricules.contractuels.et.vacations <- unique(lignes.contractuels.et.vacations[c("Matricule", "Nom", "Prénom")])

  nombre.contractuels.et.vacations <- nrow(matricules.contractuels.et.vacations)

  RI.et.vacations <- Paie[Codes.paiement.indemnitaire[Code] != 0
                          & Matricule %in% matricules.contractuels.et.vacations$Matricule,
                          c(étiquette.matricule,
                            "Statut",
                            étiquette.code,
                            étiquette.libellé,
                            étiquette.montant)]
  # Vacations et indiciaire

  traitement.et.vacations <- Paie[Codes.paiement.traitement[Code] != 0
                                  & Matricule %in% matricules.contractuels.et.vacations$Matricule,
                                  c(étiquette.matricule,
                                    "Statut",
                                    étiquette.code,
                                    étiquette.libellé,
                                    étiquette.montant)]

  nombre.Lignes.paie.contractuels.et.vacations <- nrow(lignes.contractuels.et.vacations)
  nombre.Lignes.paie.RI.et.vacations <- nrow(RI.et.vacations)
  nombre.Lignes.paie.traitement.et.vacations <- nrow(traitement.et.vacations)
} else {
  message("Pas de traitement des vacations")
}

#'
#'**Contractuels effectuant des vacations (CEV)**
#'

if (exists("nombre.contractuels.et.vacations"))
  Tableau(c("Nombre de CEV",
            "Nombre de lignes",
            "Nombre de lignes indemnitaires",
            "Nombre de lignes de traitement"),
          nombre.contractuels.et.vacations,
          nombre.Lignes.paie.contractuels.et.vacations,
          nombre.Lignes.paie.RI.et.vacations,
          nombre.Lignes.paie.traitement.et.vacations)

#'
#'[Lien vers la base de données Matricules des CEV](Bases/Réglementation/matricules.contractuels.et.vacations.csv)
#'[Lien vers la base de données Lignes de paie de CEV](Bases/Réglementation/RI.et.vacations.csv)
#'[Lien vers la base de données Lignes de traitement indiciaire pour CEV](Bases/Réglementation/traitement.et.vacations.csv)
#'
#'
#'
#'## 5.4 Contrôle sur les indemnités IAT et IFTS

#IAT et IFTS

ifts.logical <- grepl(expression.rég.ifts, Paie$Libellé, ignore.case=TRUE)
codes.ifts  <- levels(as.factor(Paie[ifts.logical, étiquette.code]))
personnels.iat.ifts <- intersect(Paie[grepl(expression.rég.iat, Libellé, ignore.case=TRUE), clé.fusion[1]],
                                 Paie[ifts.logical, clé.fusion[1]])
if (length(personnels.iat.ifts))  names(personnels.iat.ifts) <- "Matricules des agents percevant IAT et/ou IFTS sur la période"

nombre.personnels.iat.ifts <- length(personnels.iat.ifts)

#'

Tableau(c("Codes IFTS", "Nombre de personnels percevant IAT et IFTS"),
        sep.milliers = "",
        paste0(codes.ifts, collapse = " "),
        nombre.personnels.iat.ifts)

#'
#'[Codes IFTS retenus](Bases/Réglementation/codes.ifts.csv)
#'[Lien vers la base de données](Bases/Réglementation/personnels.iat.ifts.csv)
#'
#'### Contrôle sur les IFTS pour catégories B et contractuels

#IFTS et IB >= 380 (IM >= 350)

lignes.ifts.anormales <- na.omit(Paie[as.integer(Indice) < 350  & Code %in% codes.ifts,
                                      c(clé.fusion,
                                        "Statut",
                                        étiquette.code,
                                        étiquette.libellé,
                                        "Indice",
                                        étiquette.montant)])

nombre.lignes.ifts.anormales <- nrow(lignes.ifts.anormales)

# IFTS et non tit

ifts.et.contractuel <- Paie[ Statut != "TITULAIRE"
                             & Statut != "STAGIAIRE"
                             & Code %in% codes.ifts,
                             c(étiquette.matricule,
                               "Statut",
                               étiquette.code,
                               étiquette.libellé,
                               "Indice",
                               étiquette.montant)]

nombres.lignes.ifts.et.contractuel <- nrow(ifts.et.contractuel)

#'

Tableau(c("Nombre de contractuels percevant des IFTS", "Nombre de lignes IFTS pour IB < 380"), nombres.lignes.ifts.et.contractuel, nombre.lignes.ifts.anormales)

#'
#'[Lien vers la base de données Lignes IFTS pour contractuels](Bases/Réglementation/ifts.et.contractuel.csv)
#'[Lien vers la base de données Lignes IFTS pour IB < 380](Bases/Réglementation/lignes.ifts.anormales.csv)
#'
#'**Nota:**
#'IB < 380 : fonctionnaire percevant un indice brut inférieur à 380
#'
#'######
#'
#'## 5.5 Contrôle sur les heures supplémentaires



# Sont repérées comme heures supplémentaires ou complémentaires les heures dont le libellé obéissent à
# l'expression régulière expression.rég.heures.sup donnée par le fichier prologue.R


colonnes <- c(étiquette.matricule,
              étiquette.année,
              "Statut",
              "Mois",
              "Libellé",
              étiquette.code,
              "Heures",
              "Heures.Sup.",
              "Base",
              "Taux",
              "Montant",
              "Type")

HS.sup.25 <- Paie[Heures.Sup. >= 25, colonnes]

# version 1 : 48 s
# HS.sup.25 <- merge(HS.sup.25,
#                    ddply(Paie,
#                          .(Matricule, Année, Mois),
#                          summarise,
#                          "Traitement indiciaire mensuel" = sum(montant.traitement.indiciaire),
#                          .progress = "tk"))

# version optimisée : 0,15 s soit x300


  HS.sup.indiciaire.mensuel <- with(HS.sup.25, HS.sup.25[Type == "T", c("Matricule", "Année", "Mois", "Montant")])

  HS.sup.25 <- with(HS.sup.25,
                  HS.sup.25[Type %in% c("I", "T", "R")
                            & ! grepl(".*SMIC.*",
                                      Libellé, ignore.case = TRUE)
                            & grepl(expression.rég.heures.sup,
                                    Libellé, ignore.case = TRUE), ])

HS.sup.25 <- with(HS.sup.25, HS.sup.25[order(Matricule, Année, Mois), names(HS.sup.25)])

# donne un tableau à 3 dimensions [Matricules, Années, Mois] dont les valeurs sont nommées par matricule
# bizarrement le hashage de la variable année se fait par charactère alors que le mois reste entier dans certaines exécutions et pas dana d'autres !
# Tout convertir en as.character() est plus prudent.

temp <- with(HS.sup.indiciaire.mensuel,
             tapply(Montant, list(Matricule, Année, Mois), FUN=sum))

traitement.indiciaire.mensuel <-    unlist(Map(function(x, y, z) temp[x, y, z],
                                               as.character(HS.sup.indiciaire.mensuel$Matricule),
                                               as.character(HS.sup.indiciaire.mensuel$Année),
                                               as.character(HS.sup.indiciaire.mensuel$Mois)), use.names=FALSE)

HS.sup.25 <- merge(HS.sup.25, data.frame(Matricule=HS.sup.indiciaire.mensuel$Matricule,
                                         Année=HS.sup.indiciaire.mensuel$Année,
                                         Mois=HS.sup.indiciaire.mensuel$Mois,
                                         "Traitement indiciaire mensuel"=traitement.indiciaire.mensuel))

rm(temp, traitement.indiciaire.mensuel, HS.sup.indiciaire.mensuel )

HS.sup.25 <- merge(HS.sup.25, Analyse.rémunérations[ , c(étiquette.matricule, étiquette.année, "traitement.indiciaire")])

HS.sup.25 <- unique(HS.sup.25)

names(HS.sup.25) <- sub("traitement.indiciaire", "Traitement indiciaire annuel", names(HS.sup.25))

nombre.Lignes.paie.HS.sup.25 <- nrow(HS.sup.25)

# with(Base2,
#      ihts.anormales <<- Base2[! Code.catégorie %in% c("B", "C") & substr(Code,1,2) %in% c("19") & ! grepl(" ENS", Libellé), c(étiquette.matricule, étiquette.code, étiquette.libellé, étiquette.montant, "Code.catégorie")]
# )

ihts.anormales <- data.frame(NULL)
nombre.ihts.anormales <- nrow(ihts.anormales)
#'

Tableau(c("Nombre de lignes HS en excès", "Nombre de lignes IHTS anormales"), nombre.Lignes.paie.HS.sup.25, nombre.ihts.anormales)

#'
#'[Lien vers la base de données Heures suplémentaires en excès : matricules](Bases/Réglementation/HS.sup.25.csv)
#'[Lien vers la base de données IHTS anormales](Bases/Réglementation/ihts.anormales.csv)
#'
#'**Nota:**
#'HS en excès : au-delà de 25 heures par mois
#'IHTS anormales : non attribuées à des fonctionnaires de catégorie B ou C.
#'
#'## 5.6 Contrôle sur les indemnités des élus
#'

matricules.à.identifier <- unique(Paie[ ,
                                       c(étiquette.année,
                                         "Emploi",
                                         "Nom",
                                         étiquette.matricule)])

if (fichier.personnels.existe) {
  matricules.à.identifier <- merge(matricules.à.identifier, base.personnels.catégorie, by = clé.fusion, all=TRUE)
} else {
  Catégorie <- character(length = nrow(matricules.à.identifier))
  matricules.à.identifier <- cbind(matricules.à.identifier, Catégorie)
}


matricules.à.identifier <- matricules.à.identifier[order(matricules.à.identifier$Matricule,
                                                         matricules.à.identifier$Année), ]


rémunérations.élu <- Analyse.rémunérations[ Analyse.rémunérations$indemnités.élu > 0,
                                            c(clé.fusion,
                                              "Année",
                                              "Emploi",
                                              "indemnités.élu",
                                              "autres.rémunérations",
                                              "rémunération.indemnitaire.imposable") ]


rémunérations.élu <- mutate(rémunérations.élu,
                            rémunération.indemnitaire.imposable = indemnités.élu +  rémunération.indemnitaire.imposable)

if (!fusionner.nom.prénom)
  rémunérations.élu <- merge(unique(matricules.à.identifier[c("Nom",  étiquette.matricule)]),
                             rémunérations.élu,
                             by = étiquette.matricule,
                             all.y = TRUE,
                             all.x = FALSE)

names(rémunérations.élu) <- c(union(clé.fusion, "Nom"),
                              "Année",
                              "Emploi",
                              "Indemnités (euros)",
                              "Autres (euros)",
                              "Total (euros)")

rémunérations.élu <- na.omit(rémunérations.élu)

#'
if (nrow(rémunérations.élu) >0)
  kable(rémunérations.élu, row.names = FALSE)

#'
if (sauvegarder.bases.analyse)
  Sauv.base(file.path(chemin.dossier.bases, "Effectifs"),
            "matricules.à.identifier",
            fichier.personnels)

#'[Lien vers la base de données Rémunérations des élus](Bases/Réglementation/rémunérations.élu.csv)
#'

#'# Annexe

#'## Liens complémentaires
#'
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
  kable(matricules.à.identifier, row.names = FALSE)
#'

detach(Paie)

# ------------------------------------------------------------------------------------------------------------------
#  Sauvegardes : enlever les commentaires en mode opérationnel
##

if (sauvegarder.bases.analyse) {

  sauv.bases(file.path(chemin.dossier.bases, "Rémunérations"),
             "Analyse.rémunérations",
             "Analyse.variations.synthèse",
             "Analyse.variations.par.exercice",
             "Analyse.variations.synthèse.filtrée",
             "Analyse.variations.synthèse.filtrée.plus.2.ans",
             "Analyse.variations.synthèse.filtrée.moins.2.ans",
             "masses.premier.personnels",
             "masses.premier.élus",
             "masses.dernier.personnels",
             "masses.dernier.élus")

  sauv.bases(file.path(chemin.dossier.bases, "Effectifs"),
             "Bulletins.paie.nir.total.hors.élus",
             "Bulletins.paie.nir.fonctionnaires",
             "tableau.effectifs")

  sauv.bases(file.path(chemin.dossier.bases, "Réglementation"),
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



