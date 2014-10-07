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

options(warn = -1, verbose = FALSE, OutDec = ",")

encodage.code.source <- "ISO-8859-1"

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

installer.paquets(knitr, plyr, ggplot2, assertthat, yaml, gtools, utils, data.table)
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

if (table.rapide && séparateur.liste != ",") 
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

if (! import.direct && file.exists(chemin(nom.fichier.codes.paiement)))
{
  Codes.paiement <- read.csv.skip(nom.fichier.codes.paiement, rapide = FALSE, séparateur.liste = séparateur.liste, séparateur.décimal = séparateur.décimal)

  if (nlevels(as.factor(Codes.paiement$Code)) != nrow(unique(Codes.paiement[ , c(étiquette.code, "Type.rémunération")])))
  {
    message("Davantage de types de rémunérations que de codes distincts : incohérence de la base de codes.")

    V <- tapply(Codes.paiement$Type.rémunération, Codes.paiement$Code, function(x) length(unique(x)))
    V <- V[V > 1]

    print(unique(merge(data.frame(Code = names(V),
                                  "Nombre de types de rémunérations distincts" = V,
                                   row.names=NULL),
                       Codes.paiement[Codes.paiement$Code %in% names(V),
                                      c(étiquette.code, "Type.rémunération")],
                       by = étiquette.code,
                       all=TRUE)))

    stop("Vérifier le fichier " %+% nom.fichier.codes.paiement)

  }

  message("Chargement des codes de paiement.")
}


  fichier.personnels.existe <- file.exists(chemin(nom.fichier.personnels))

  if (fichier.personnels.existe) {
   base.personnels.catégorie <- read.csv.skip(nom.fichier.personnels, séparateur.liste = séparateur.liste, séparateur.décimal = séparateur.décimal)
   message("Chargement du fichier des catégories statutaires des personnels.")
  }

# Lignes de paie
# On peut lire jusqu'à 50 fichiers csv de lignes de paie qui seront générés au format :

# "chemin dossier + racine-Lignes de paye-j.csv" où racine est un bref identifiant du type de contrôle (exemple : "c2a-", "PEV-", ...)

lignes.paie <- nom.fichier.paie %+% "-" %+% 1:50 %+% ".csv"
lignes.paie <- lignes.paie[file.exists(chemin(lignes.paie))]

# Bulletins de paie
# On peut lire jusqu'à 10 fichiers csv de bulletins de paie qui seront générés au format :

# "chemin dossier + racine-Bulletins de paye-j.csv"

bulletins.paie <- nom.bulletin.paie %+% "-" %+% 1:10 %+% ".csv"
bulletins.paie <- bulletins.paie[file.exists(chemin(bulletins.paie))]

if (import.direct)
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
                    rapide = table.rapide),
                    silent = TRUE)

  if (inherits(res, 'try-error')) {
    res2 <- try(Read.csv("Lignes.paie",
                         lignes.paie,
                         colClasses = lignes.paie.classes.input.fallback,
                         colNames = lignes.paie.input.fallback,
                         séparateur.liste = séparateur.liste,
                         séparateur.décimal = séparateur.décimal,
                         rapide = table.rapide),
                         silent = TRUE)
  }
  
  c(res, res2)
}

if (! import.direct)
{
  résultat <- Import.Lignes.paie()

if (table.rapide && résultat[2] != NULL && inherits(résultat[2], 'try-error'))
{
   message("Conversion en UTF-8...")
   Vectorize(file2utf8, simplify = FALSE, USE.NAMES = FALSE)(lignes.paie)
   message("Conversion en UTF-8 terminée")
   
   résultat <- Import.Lignes.paie()
   
   if (résultat[2] != NULL && inherits(résultat[2], 'try-error')) 
      stop("Problème de lecture des bases de lignes de paye")
}       

if (!is.null(Lignes.paie))
   message("Chargement des lignes de paie.") else stop("Chargement des lignes de paie en échec.")

if (table.rapide) {
    
  Lignes.paie <- Lignes.paie[ ,setdiff(names(Lignes.paie), c("Année.1","Mois.1","Matricule.1")), with = FALSE]
  
  } else {
    
  Lignes.paie <- Lignes.paie[ ,setdiff(names(Lignes.paie), c("Année.1","Mois.1","Matricule.1"))]
  
}

res <- try(Read.csv("Bulletins.paie",
                    bulletins.paie,
                    colClasses = bulletins.paie.classes.input,
                    colNames = bulletins.paie.input,
                    séparateur.liste = séparateur.liste,
                    séparateur.décimal = séparateur.décimal,
                    rapide = table.rapide), silent = TRUE)

if (inherits(res, 'try-error'))
    stop("Problème de lecture des bases de bulletins de paye")

if (!is.null(Bulletins.paie)) message("Chargement des bulletins de paie.") else stop("Chargement des bulletins de paie en échec.")

} else {
  
    res <- try(Read.csv("Bulletins.paie.Lignes.paie",
                        nom.table,
  #                      colClasses = union(bulletins.paie.classes.input, lignes.paie.classes.input),
  #                      colNames = bulletins.paie.input,
                        séparateur.liste = séparateur.liste,
                        séparateur.décimal = séparateur.décimal,
                        rapide = table.rapide), silent = FALSE)
    
    if (inherits(res, 'try-error'))
      stop("Problème de lecture de la base de la table bulletins-lignes de paye")
    
    if (!is.null(Bulletins.paie.Lignes.paie)) {
      message("Chargement de la table bulletins-lignes de paye.") 
    } else {
      stop("Chargement de la table bulletins-lignes de paie en échec.")
    }
  
    message("Chargement direct des bulletins et lignes de paie")
}  


# dans le cas où l'on ne lance le programme que pour certaines années, il préciser début.période sous revue et fin.période .sous.revue
# dans le fichier prologue.R. Sinon le programme travaille sur l'ensemble des années disponibles.

if (! extraire.années) {

  if (import.direct)
  {
    début.période.sous.revue <- min(Bulletins.paie.Lignes.paie$Année)
    fin.période.sous.revue   <- max(Bulletins.paie.Lignes.paie$Année)
    
  } else {
    
    début.période.sous.revue    <- c(min(Bulletins.paie$Année), min(Lignes.paie$Année))
    fin.période.sous.revue      <- c(max(Bulletins.paie$Année), max(Lignes.paie$Année))
    if (début.période.sous.revue[1] != début.période.sous.revue[2])
      stop("Les bases des bulletins et lignes de paye
ne portent pas sur le même nombre d'années : " %+%  début.période.sous.revue[1] %+% "... pour les bulletins et " %+% début.période.sous.revue[1] %+% "... pour les lignes de paye.")
    
    if (fin.période.sous.revue[1] != fin.période.sous.revue[2])
      stop("Les bases des bulletins et lignes de paye
ne portent pas sur le même nombre d'années : ..." %+%  fin.période.sous.revue[1] %+% " pour les bulletins et ..." %+% fin.période.sous.revue[1] %+% "... pour les lignes de paye.")
    
    début.période.sous.revue <- début.période.sous.revue[1]
    fin.période.sous.revue   <- fin.période.sous.revue[1]
    
  }

  
} else {

  if (import.direct)
  {
    Bulletins.paie.Lignes.paie <- Bulletins.paie.Lignes.paie[Bulletins.paie.Lignes.paie$Année >= début.période.sous.revue
                                                             & Bulletins.paie.Lignes.paie$Année <= fin.période.sous.revue, ]  
      
  } else {
    
    Bulletins.paie <- Bulletins.paie[Bulletins.paie$Année >= début.période.sous.revue
                                     & Bulletins.paie$Année <= fin.période.sous.revue, ]
    Lignes.paie    <- Lignes.paie[Lignes.paie$Année >= début.période.sous.revue
                                  & Lignes.paie$Année <= fin.période.sous.revue, ]
  }
}

période                     <- début.période.sous.revue:fin.période.sous.revue
durée.sous.revue            <- fin.période.sous.revue - début.période.sous.revue + 1

if (! import.direct)
{
  if (table.rapide) {
  
  matricules.à.retirer  <- Lignes.paie[Lignes.paie$Code == "", "Matricule", with=F]
  matricules.à.retirer  <- matricules.à.retirer[[1]]
  
  } else {
  
  matricules.à.retirer  <- Lignes.paie[Lignes.paie$Code == "", "Matricule"]
  }
  
  Lignes.paie    <- Lignes.paie[Lignes.paie$Code != "", ]
  Bulletins.paie <- Bulletins.paie[! Bulletins.paie$Matricule %in% matricules.à.retirer, ]
  
  Bulletins.paie.contiennent.colonnes.requises <- colonnes.requises %in% names(Bulletins.paie)
  
  if (! all(Bulletins.paie.contiennent.colonnes.requises)) {
  
  stop("Il manque les colonnes suivantes au(x) fichier(s) Bulletins de paie :",
       colonnes.requises[! Bulletins.paie.contiennent.colonnes.requises])
  
  } else {
  
  message("Contrôle des noms de colonne des bulletins de paie : normal.")
  }
}

# Lors de la PREMIERE utilisation d'Altair, paramétrer générer.codes <- TRUE dans prologue.R
# pour générer les fichier des codes de paiement sous le dossier des bases (par défaut "Données").
# ce fichier est trier par ordre croissant des codes de paiement sur les trois premiers chiffres des codes
# des anomalies peuvent résiduellement apparaître avec des codes contenant des lettres, en général après
# le troisième chiffre du code.
# L'utilisateur devra alors renseigner la colonne étiquette.type.rémunération de ce fichier

if (générer.codes) source("générer.codes.R", encoding = encodage.code.source)
if (! import.direct) {
  source("fusionner.bulletins.lignes.R", encoding = encodage.code.source)
 } else {
  Bulletins.paie <- Bulletins.paie.Lignes.paie
 }

if (table.rapide == TRUE) {
  
  Bulletins.paie.Lignes.paie[ ,   quotité   := ifelse(etp.égale.effectif | is.na(Temps.de.travail), 1,  Temps.de.travail / 100)]
  Bulletins.paie.Lignes.paie[ ,   Montant.net.eqtp := ifelse(is.finite(Net.à.Payer/quotité), Net.à.Payer/quotité,  NA)]
  
} else {
  Bulletins.paie.Lignes.paie <- mutate(Bulletins.paie.Lignes.paie,
                           
                           ### EQTP  ###
                           
                           quotité                  = ifelse(etp.égale.effectif | is.na(Temps.de.travail), 1,  Temps.de.travail / 100),
                           #                          * ((corriger.quotité)*(is.na(Taux) * (1- Taux)  + Taux) + 1 - corriger.quotité)
                           # if( ...) ne fonctionne pas.
                           
                           Montant.net.eqtp         = ifelse(is.finite(Net.à.Payer/quotité), Net.à.Payer/quotité, NA))
}

anavar <- ddply(Bulletins.paie.Lignes.paie,
                c(clé.fusion, étiquette.année),
                summarise,
                # partie Analyse des variations par exercice #
                
                Montant.net.annuel.eqtp = sum(Montant.net.eqtp, na.rm = TRUE),
                # En principe la colonne Brut ne tient pas compte des remboursements d efrais ou des régularisations
                Montant.brut.annuel = sum(Brut),
                Statut.sortie       = Statut[length(Net.à.Payer)],
                mois.entrée         = ifelse((minimum <- min(Mois)) != Inf, minimum, 0),
                mois.sortie         = ifelse((maximum <- max(Mois)) != -Inf, maximum, 0),
                nb.jours            = calcul.nb.jours.mois(mois.entrée[1], mois.sortie[1], Année[1]),
                nb.mois             = mois.sortie[1] - mois.entrée[1] + 1)

Bulletins.paie.Lignes.paie <- merge (Bulletins.paie.Lignes.paie, anavar, by = c(étiquette.matricule, étiquette.année))

Bulletins.paie.Lignes.paie$quotité[is.na(Bulletins.paie.Lignes.paie$quotité)] <- 0

message("Bulletins de paye retraités")

if (! import.direct) {
  Analyse.rémunérations <- ddply(Bulletins.paie.Lignes.paie,
                                 c(clé.fusion, étiquette.année),
                                 summarise,
                                 mois.entrée  = mois.entrée[1],
                                 Nir          = Nir[1],
                                 Montant.net.annuel.eqtp  = Montant.net.annuel.eqtp[1],
                                 Montant.brut.annuel = Montant.brut.annuel[1],
                                 Statut       = Statut[1],
  
                                 nb.jours     = nb.jours[1],
                                 nb.mois      = nb.mois[1],
                                 mois.sortie  = mois.sortie[1],
                                 Emploi       = Emploi[length(Net.à.Payer)],
                                 traitement.indiciaire               = sum(montant.traitement.indiciaire),
                                 rémunération.principale.contractuel = sum(montant.rémunération.principale.contractuel),
                                 rémunération.vacataire              = sum(montant.rémunération.vacataire),
                                 rémunération.indemnitaire           = sum(montant.primes),
                                 indemnités.élu                      = sum(montant.indemnité.élu),
                                 autres.rémunérations                = sum(montant.autres.rémunérations),
  
                                 # on ne considère que les rémunérations brutes (sans prise en compte des remboursements de frais aux salariés ou des régularisations)
                                 # pour être en homogénéïté avec la colonne Brut/Montant.brut.annuel
  
                                 total.rémunérations                 =  traitement.indiciaire                       # le premier et deuxième terme sont exclusifs
                                                                      + rémunération.principale.contractuel
                                                                      + rémunération.vacataire
                                                                      + rémunération.indemnitaire,
  
                                 # ici on ajoute les remboursements de frais professionnels (autres.rémunérations) et on enlève les régularisations (détachements..., màd...)
  
                                 total.rémunérations.et.remboursements =  total.rémunérations
                                                                        + autres.rémunérations
                                                                        + indemnités.élu,
  
                                 part.rémunération.indemnitaire =  ifelse(is.na(s <-  traitement.indiciaire
                                                                            + rémunération.principale.contractuel
                                                                            + rémunération.indemnitaire) | s == 0,
                                                                           NA, (rémunération.indemnitaire + rémunération.principale.contractuel )
                                                                                   / s * 100))

} else {
  Analyse.rémunérations <- ddply(Bulletins.paie.Lignes.paie,
                                 c(clé.fusion, étiquette.année),
                                 summarise,
                                 mois.entrée  = mois.entrée[1],
                                 Nir          = Nir[1],
                                 Montant.net.annuel.eqtp  = Montant.net.annuel.eqtp[1],
                                 Montant.brut.annuel = Montant.brut.annuel[1],
                                 Statut       = Statut[1],
                                 
                                 nb.jours     = nb.jours[1],
                                 nb.mois      = nb.mois[1],
                                 mois.sortie  = mois.sortie[1],
                                 Emploi       = Emploi[length(Net.à.Payer)],
                                 traitement.indiciaire               = sum(Montant[Type == "Traitement"]),
                                # rémunération.principale.contractuel = sum(Montant[Type == "Traitement"]),
                                # rémunération.vacataire              = sum(Montant[Type == "Traitement"]),
                                 rémunération.indemnitaire           = sum(Montant[Type == "Indemmnité" | Type == "Indemnité de résidence"]),
                                 indemnités.élu                      = sum(Montant[Type == "Indemmnité" & (grepl(expression.rég.élus, Service) | grepl(expression.rég.élus, Emploi))]),
                                 autres.rémunérations                = sum(Montant[Type == "Rappel"]),
                                 
                                 # on ne considère que les rémunérations brutes (sans prise en compte des remboursements de frais aux salariés ou des régularisations)
                                 # pour être en homogénéïté avec la colonne Brut/Montant.brut.annuel
                                 
                                 total.rémunérations                 =  traitement.indiciaire                       # le premier et deuxième terme sont exclusifs
                                # + rémunération.principale.contractuel
                                # + rémunération.vacataire
                                 + rémunération.indemnitaire
                                 + autres.rémunérations,
                                 
                                 # ici on ajoute les remboursements de frais professionnels (autres.rémunérations) et on enlève les régularisations (détachements..., màd...)
                                 
                                 total.rémunérations.et.remboursements =  total.rémunérations
                                                                 + indemnités.élu,
                                 
                                 part.rémunération.indemnitaire =  ifelse(is.na(s <-  traitement.indiciaire
                                                        #                        + rémunération.principale.contractuel
                                                                                + rémunération.indemnitaire) | s == 0,
                                                                          NA, (rémunération.indemnitaire 
                                                                               # + rémunération.principale.contractuel 
                                                                               )
                                                                          / s * 100))
  Bulletins.paie <- Bulletins.paie.Lignes.paie
}


  Analyse.rémunérations <- Analyse.rémunérations[!is.na(Analyse.rémunérations$total.rémunérations), ]


  if (length (Analyse.rémunérations$quotité[Analyse.rémunérations$quotité > 1]) > 0 & comportement.strict ) stop("Détection de quotités > 1", call. = FALSE)

  message("Analyse des rémunérations réalisée.")

  Analyse.variations.par.exercice <- Analyse.rémunérations[ , c(clé.fusion, étiquette.année,
                                                                "Montant.net.annuel.eqtp",
                                                                "Statut",
                                                                "nb.jours",
                                                                "nb.mois")]

 Analyse.variations.synthèse <- ddply(Analyse.variations.par.exercice,
                                       clé.fusion,
                                       summarise,
                                       Nexercices = length(Année),
                                       nb.mois.exercice.début  = nb.mois[1],
                                       nb.mois.exercice.sortie = nb.mois[Nexercices],
                                       total.jours = sum(nb.jours),
                                       total.mois  = sum(nb.mois),
                                       Montant.net.annuel.eqtp.début  = Montant.net.annuel.eqtp[1],
                                       Montant.net.annuel.eqtp.sortie = Montant.net.annuel.eqtp[Nexercices],
                                       moyenne.rémunération.annuelle.sur.période =
                                         sum(Montant.net.annuel.eqtp, na.rm = TRUE)/length(Année[!is.na(Montant.net.annuel.eqtp) & Montant.net.annuel.eqtp > 0]),
                                       variation.rémunération = ifelse(Nexercices > 1 & !is.na(Montant.net.annuel.eqtp.début) & !is.na(Montant.net.annuel.eqtp.sortie) & Montant.net.annuel.eqtp.début > 0 & Montant.net.annuel.eqtp.sortie > 0,
                                                                 (Montant.net.annuel.eqtp.sortie / Montant.net.annuel.eqtp.début - 1)*100, NA),
                                       variation.moyenne.rémunération = ifelse(is.na(total.mois)
                                                                            | is.na(variation.rémunération)
                                                                            | total.mois < 13,
                                                                         NA,
                                                                         ((Montant.net.annuel.eqtp.sortie/Montant.net.annuel.eqtp.début)^(12 / (total.mois - 12)) - 1) * 100),
                                       variation.rémunération.normalisée = ifelse(durée.sous.revue == Nexercices
                                                                               & nb.mois.exercice.début == 12
                                                                               & nb.mois.exercice.sortie == 12,
                                                                            variation.rémunération, NA),
                                       variation.moyenne.rémunération.normalisée = ifelse(!is.na(variation.rémunération.normalisée), variation.moyenne.rémunération, NA),
                                       plus.2.ans  = (total.mois  >= 2*12),
                                       moins.2.ans = (total.mois < 2*12),
                                       moins.1.an  = (total.mois < 12),
                                       moins.six.mois = (total.mois < 6),
                                       statut = Statut[1])

  Analyse.variations.par.exercice <- na.omit(Analyse.variations.par.exercice[ ( Analyse.variations.par.exercice$Année > début.période.sous.revue
                                                                               & Analyse.variations.par.exercice$Année < fin.période.sous.revue) |
                                                                               Analyse.variations.par.exercice$nb.mois > seuil.troncature, ])

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

  if (table.rapide) {

    Bulletins.paie.nir.total.hors.élus <- merge(Bulletins.paie[Bulletins.paie$Année == fin.période.sous.revue
                                                               & Bulletins.paie$Mois == 12,
                                                               c(clé.fusion, champ.nir), with = FALSE],
                                                Analyse.rémunérations[Analyse.rémunérations$Année == fin.période.sous.revue
                                                                      & Analyse.rémunérations$indemnités.élu == 0,
                                                                      c(clé.fusion, champ.nir)],
                                                by = clé.fusion)

  } else {

    Bulletins.paie.nir.total.hors.élus <- merge(Bulletins.paie[Bulletins.paie$Année == fin.période.sous.revue
                                                               & Bulletins.paie$Mois == 12,
                                                               c(clé.fusion, champ.nir)],
                                                Analyse.rémunérations[Analyse.rémunérations$Année == fin.période.sous.revue
                                                                      & Analyse.rémunérations$indemnités.élu == 0,
                                                                      c(clé.fusion, champ.nir)],
                                                by = clé.fusion)
  }


 if (table.rapide) {

   Bulletins.paie.nir.fonctionnaires  <- unique(Bulletins.paie[Bulletins.paie$Année == fin.période.sous.revue
                                                               & Bulletins.paie$Mois  == 12
                                                               & (Bulletins.paie$Statut == "TITULAIRE" |
                                                                  Bulletins.paie$Statut == "STAGIAIRE"),
                                                                 c(clé.fusion, champ.nir), with = FALSE])

 } else {

   Bulletins.paie.nir.fonctionnaires  <- unique(Bulletins.paie[Bulletins.paie$Année == fin.période.sous.revue
                                                               & Bulletins.paie$Mois  == 12
                                                               & (Bulletins.paie$Statut == "TITULAIRE" |
                                                                  Bulletins.paie$Statut == "STAGIAIRE"),
                                                                 c(clé.fusion, champ.nir)])

 }

  Bulletins.paie.nir.total.hors.élus$Nir.y  <- NULL
  names(Bulletins.paie.nir.total.hors.élus) <- c(clé.fusion, champ.nir)

 if (table.rapide) {
  class(Bulletins.paie.nir.total.hors.élus) <- "data.frame"
  class(Bulletins.paie.nir.fonctionnaires)  <- "data.frame"
 }

  # Age au 31 décembre de l'exercice dernier.exerciceal de la période sous revue
  # ne pas oublier [ ,...] ici:

  années.fonctionnaires   <- extraire.nir(Bulletins.paie.nir.fonctionnaires)

  années.total.hors.élus  <- extraire.nir(Bulletins.paie.nir.total.hors.élus)


message("Analyse démographique réalisée.")

if (!is.null(Bulletins.paie.Lignes.paie) & !is.null(Analyse.rémunérations)
    & !is.null(Analyse.variations.synthèse) & !is.null(Analyse.variations.par.exercice))
  message("Statistiques de synthèse réalisées")

if (table.rapide) {
  class(Bulletins.paie)             <- "data.frame"
  class(Bulletins.paie.Lignes.paie) <- "data.frame"
}


########### Démographie ########################

#'# 1. Statistiques de population
#'
#'### 1.1 Effectifs

liste.années <- as.character(période)

effectifs <- lapply(période,
                    function(x) {
                      A <- Bulletins.paie[Bulletins.paie$Année == x, c("Matricule", "Statut", "Service", "Emploi", "nb.mois")]
                      E <- unique(A[ , c("Matricule", "nb.mois")])
                      F <- E[E$nb.mois == 12, ]
                      G <- unique(A[(A$Statut == "TITULAIRE" | A$Statut == "STAGIAIRE") , c("Matricule", "nb.mois")])
                      H <- G[G$nb.mois == 12, ]
                      I <- unique(A[grepl(expression.rég.élus, A$Service, ignore.case=TRUE)
                                    | grepl(expression.rég.élus, A$Emploi, ignore.case=TRUE), c("Matricule", "nb.mois")])
                      J <- I[I$nb.mois == 12, ]
                      résultat <- c(nrow(E), nrow(F), nrow(G), nrow(H), nrow(I), nrow(J))
                      rm(A, E, F, G, H, I, J)
                      résultat
                    })

effectifs <- prettyNum(effectifs, big.mark = " ")
tableau.effectifs <- as.data.frame(effectifs, row.names = c("Total", "  dont présents 12 mois", "  dont fonctionnaires", "  dont fonct. présents 12 mois", "  dont élus", "  dont élus présents 12 mois"))

names(tableau.effectifs) <- liste.années
#'
kable(tableau.effectifs, row.names = TRUE, align='c')
#'  
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
        sum(plus.2.ans),
        sum(moins.2.ans),
        sum(moins.1.an),
        sum(moins.six.mois))

#'

if (nrow(Analyse.variations.par.exercice) > 0)
  qplot(factor(Année),
        data = Analyse.variations.par.exercice,
        geom = "bar",
        fill = factor(!plus.2.ans),
        main = "Evolutions entre " %+% début.période.sous.revue %+% " et " %+% fin.période.sous.revue,
        xlab = étiquette.année,
        ylab = "Effectif",
        asp = 4)        +
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
#'Toutes les durées de service sont calculées en multiples de 365 jours.
#'Dans les statistiques de cette section, les élus ne sont pas pris en compte.
#'

# on ne compte pas les élus dans le total (voir 5.6)

année <- début.période.sous.revue

colonnes.sélectionnées <- c("traitement.indiciaire",
                            "rémunération.indemnitaire",
                            "autres.rémunérations",
                            "total.rémunérations",
                            "total.rémunérations.et.remboursements",
                            "Montant.brut.annuel",
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
#'## 2.1 Statistiques de position globales (tous statuts)
#'

masses.premier <- colSums(Analyse.rémunérations.premier.exercice[c("Montant.brut.annuel",
                                                         "total.rémunérations",
                                                         "total.rémunérations.et.remboursements",
                                                         "indemnités.élu",
                                                         "autres.rémunérations")])

#'### Cumuls des rémunérations brutes pour l'exercice `r année`
#'

Tableau(c("Rémunérations brutes",
          "Indemnités d'élus",
          "Autres paiements",
          "Total brut"),
        masses.premier["total.rémunérations"],
        masses.premier["indemnités.élu"],
        masses.premier["autres.rémunérations"],
        sum(masses.premier[c("total.rémunérations", "indemnités.élu", "autres.rémunérations")]))

#'
#'**Définitions :**
#'Tous les items du tableau sont calculés sur le champ Montant de la base Lignes de paie
#'  *Rémunération brutes* : somme du champ *Montant* des rémunérations versées hors élus
#'  *Indemnités d'élus*   : somme du champ *Montant* des rémunérations versées pour les élus
#'  *Autres paiements*    : remboursements de frais, régularisations, etc., non compris dans les deux premiers agrégats
#'  *Total brut*          : somme des trois précédents facteurs
#'

somme.brut.non.élu  <- sum(Bulletins.paie[  Bulletins.paie$Année == année
                                          & ! grepl(expression.rég.élus, Bulletins.paie$Emploi, ignore.case=TRUE)
                                          & ! grepl(expression.rég.élus, Bulletins.paie$Emploi, ignore.case=TRUE),
                                            "Brut"])

delta  <- somme.brut.non.élu - masses.premier["total.rémunérations"]

#'**Tests de cohérence**
#'
#'Somme des rémunérations brutes versées aux personnels (non élus) :
#'
Tableau.vertical2(c("Agrégats",
                    "euros"),
                 c("Bulletins de paie (euros)",
                   "Lignes de paie (euros)",
                   "Différence (euros)"),
                 c(somme.brut.non.élu,
                   masses.premier["total.rémunérations"],
                   delta))


delta2 <-  masses.premier["Montant.brut.annuel"] - masses.premier["total.rémunérations"] - masses.premier["indemnités.élu"]


#'
#'à comparer aux soldes des comptes 641 et 648 du compte de gestion.
#'
#'Somme des rémunérations brutes versées (élus compris) :
#'
Tableau.vertical2(c("Agrégats",
                    "euros"),
                 c("Bulletins de paie (euros)",
                   "Lignes de paie (euros)",
                   "Différence (euros)"),
                 c(masses.premier["Montant.brut.annuel"],
                   masses.premier["total.rémunérations"] + masses.premier["indemnités.élu"],
                   delta2))

#'
#'à comparer aux soldes des comptes 641, 648 et 653 du compte de gestion
#'
#'**Définitions :**
#'  *Bulletins de paie*   : somme du champ *Brut* de la base Bulletins de paie. Le champ *Brut* ne tient pas compte des *Autres paiements* (remboursements de frais, régularisations, etc.) en base de données.
#'  *Lignes de paie*      : somme des lignes de paie correspondantes de la base Lignes de paie sans tenir compte des *Autres paiements*
#'

#'
#'Les résultats sont exprimés en euros.
#'

#'
#'## 2.2 Fonctionnaires titulaires et stagiaires
#'

filtre.fonctionnaire <- function (X) X[ (Statut == "TITULAIRE" | Statut == "STAGIAIRE") & X >0 ]

AR <- Analyse.rémunérations.premier.exercice[Statut == "TITULAIRE" | Statut == "STAGIAIRE", colonnes.sélectionnées]

if (longueur.non.na(filtre.fonctionnaire(total.rémunérations) > 0))
 hist(filtre.fonctionnaire(total.rémunérations)/1000,
     xlab = "En milliers d'euros hors\nindemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 120),
     main = "Rémunération annuelle des fonctionnaires en " %+% année,
     col = "blue",
     nclass = 50)

#'

if (longueur.non.na(filtre.fonctionnaire(rémunération.indemnitaire) > 0))
  hist(filtre.fonctionnaire(rémunération.indemnitaire)/1000,
     xlab = "En milliers d'euros hors\nindemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 70),
     main = "Rémunération indemnitaire annuelle des fonctionnaires en " %+% année,
     col = "blue",
     nclass = 50)

if (longueur.non.na(filtre.fonctionnaire(part.rémunération.indemnitaire) > 0))
  hist(filtre.fonctionnaire(part.rémunération.indemnitaire),
     xlab = "Part des indemnités dans la rémunération en %\n hors indemnités journalières et remboursements",
     ylab = "Effectif",
     main = "Part indemnitaire de la rémunération annuelle des fonctionnaires en " %+% année,
     xlim = c(0,60),
     col = "blue",
     nclass = 60)

#'
#'**Effectif : `r nrow(AR)`**
#'
#'**Tests de cohérence**

if (nrow(AR) > 0) {
   temp <- colSums(AR[ ,c("Montant.brut.annuel", "total.rémunérations", "total.rémunérations.et.remboursements")])
} else {
   temp <- c(0,0) }

#'Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :
#'

Tableau.vertical2(c("Agrégats", "euros"),
                  c("Bulletins de paie (euros)",
                    "Lignes de paie (euros), hors remb.",
                    "Lignes de paie (euros), total.",
                    "Différence (euros)"),
                  c(temp["Montant.brut.annuel"],
                    temp["total.rémunérations"],
                    temp["total.rémunérations.et.remboursements"],
                    temp["Montant.brut.annuel"] - temp["total.rémunérations.et.remboursements"]))

rm(temp)

#'
#'A comparer aux soldes des comptes 6411, 6419 et 648 du conmpte de gestion.
#'

#'
#'### Statistiques de position pour l'exercice `r année`
#'


Résumé(c("Traitement indiciaire",
         étiquette.rém.indemn,
         "Autres rémunérations",
         "Effectif"),
       AR[c("traitement.indiciaire", "rémunération.indemnitaire", "autres.rémunérations")],
       extra = "length")


#'
Résumé(c("Total rémunérations", "Part de la rémunération contractuelle ou indemnitaire", "Effectif"),
       AR[c("total.rémunérations", "part.rémunération.indemnitaire")],
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
           étiquette.rém.indemn,
           "Autres rémunérations"),
         ARA[c("traitement.indiciaire",
             "rémunération.indemnitaire",
             "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
  Résumé(c("Total rémunérations", "Part de la rémunération contractuelle ou indemnitaire"),
          ARA[c( "total.rémunérations", "part.rémunération.indemnitaire")])
}

#'
#'**Effectif : `r nrow(ARA)`**
#'
#'**Catégorie B**
#'

if (fichier.personnels.existe)
{
  Résumé(c("Traitement indiciaire",
           étiquette.rém.indemn,
           "Autres rémunérations"),
         ARB[ c("traitement.indiciaire",
                "rémunération.indemnitaire",
                "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
  Résumé(c("Total rémunérations",
           "Part de la rémunération contractuelle ou indemnitaire"),
         ARB[ c( "total.rémunérations",
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
           étiquette.rém.indemn,
           "Autres rémunérations"),
         ARC[ c("traitement.indiciaire",
                "rémunération.indemnitaire",
                "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
  Résumé(c("Total rémunérations",
           "Part de la rémunération contractuelle ou indemnitaire"),
         ARC[ c( "total.rémunérations",
                 "part.rémunération.indemnitaire") ])
}

#'**Effectif : `r nrow(ARC)`**
#'
#'
#'######      <br>
#'
#'## 2.3 Contractuels, vacataires et stagiaires inclus
#'

temp <- total.rémunérations[indemnités.élu == 0
                            & Statut != "TITULAIRE"
                            & Statut != "STAGIAIRE"
                            & total.rémunérations > 1000] / 1000

if (length(temp > 0))
  hist(temp,
     xlab = "Rémunération en milliers d'euros \n hors indemnités journalières et remboursements",
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
       xlab = "En euros :\n indemnités journalières et remboursements",
       ylab = "Effectif",
       xlim = c(0, 5000),
       main = "Autres rémunérations en " %+% année,
       nclass = 50,
       col = "grey")

#'


AR <- Analyse.rémunérations.premier.exercice[  indemnités.élu == 0
                                              &  Statut != "TITULAIRE"
                                              &  Statut != "STAGIAIRE",
                                              c("rémunération.indemnitaire",
                                                "autres.rémunérations",
                                                "total.rémunérations") ]

#'
Résumé(c(étiquette.rém.indemn,
         "Autres rémunérations",
         "Effectif"),
       AR[c("rémunération.indemnitaire", "autres.rémunérations")],
       extra = "length")

#'

Résumé(c("Total rémunérations",
         "Effectif"),
       AR["total.rémunérations"],
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

masses.dernier <- colSums(Analyse.rémunérations.dernier.exercice[c("Montant.brut.annuel", "total.rémunérations", "indemnités.élu", "autres.rémunérations")])

#'### Cumuls des rémunérations brutes pour l'exercice `r année`
#'

Tableau(c("Rémunérations brutes",
          "Indemnités d'élus",
          "Autres paiements",
          "Total brut"),
        masses.dernier["total.rémunérations"],
        masses.dernier["indemnités.élu"],
        masses.dernier["autres.rémunérations"],
        sum(masses.dernier[c("total.rémunérations", "indemnités.élu", "autres.rémunérations")]))

#'
#'**Définitions :**
#'Tous les items du tableau sont calculés sur le champ Montant de la base Lignes de paie
#'  *Rémunération brutes* : somme du champ *Montant* des rémunérations versées hors élus
#'  *Indemnités d'élus*   : somme du champ *Montant* des rémunérations versées pour les élus
#'  *Autres paiements*    : remboursements de frais, régularisations, etc., non compris dans les deux premiers agrégats
#'  *Total brut*          : somme des trois précédents facteurs
#'

#'
#'Cumuls réalisés sur les lignes de paie. Les indemnités d'élu ne sont pas prises en compte.
#'

somme.brut.non.élu  <- sum(Bulletins.paie[  Bulletins.paie$Année == année
                                          & ! grepl(expression.rég.élus, Bulletins.paie$Emploi, ignore.case=TRUE)
                                          & ! grepl(expression.rég.élus, Bulletins.paie$Service, ignore.case=TRUE),
                                          "Brut"])

             delta  <- somme.brut.non.élu - masses.dernier["total.rémunérations"]

#'**Tests de cohérence**
#'
#'Somme des rémunérations brutes versées aux personnels (non élus) :
#'

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Bulletins de paie (euros)",
                    "Lignes de paie (euros)",
                    "Différence (euros)"),
                  c(somme.brut.non.élu,
                    masses.dernier["total.rémunérations"],
                    delta))

delta2 <-  masses.dernier["Montant.brut.annuel"] - masses.dernier["total.rémunérations"] - masses.dernier["indemnités.élu"]


#'
#'à comparer aux soldes des comptes 641 et 648 du compte de gestion.
#'
#'**Définitions :**
#'  *Bulletins de paie*   : somme du champ *Brut* de la base Bulletins de paie. Le champ *Brut* ne tient pas compte des *Autres paiements* (remboursements de frais, régularisations, etc.) en base de données.
#'  *Lignes de paie*      : somme des lignes de paie correspondantes de la base Lignes de paie sans tenir compte des *Autres paiements*
#'
#'Somme des rémunérations brutes versées (élus compris) :
#'

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Bulletins de paie (euros)",
                    "Lignes de paie (euros)",
                    "Différence (euros)"),
                  c(masses.dernier["Montant.brut.annuel"],
                    masses.dernier["total.rémunérations"] + masses.dernier["indemnités.élu"],
                    delta2))

#'
#'à comparer aux soldes des comptes 641, 648 et 653 du compte de gestion
#'

#'
#'Les résultats sont exprimés en euros.
#'
#'
#'######

#'## 3.2 Fonctionnaires titulaires et stagiaires
#'

if (longueur.non.na(filtre.fonctionnaire(total.rémunérations)) > 0)
  hist(filtre.fonctionnaire(total.rémunérations) / 1000,
     xlab = "En milliers d'euros \n hors indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 120),
     main = "Rémunération annuelle des fonctionnaires en " %+% année,
     col = "blue",
     nclass = 50)

#'
#'

if (longueur.non.na(filtre.fonctionnaire(rémunération.indemnitaire) > 0))
  hist(filtre.fonctionnaire(rémunération.indemnitaire)/1000,
     xlab = "En milliers d'euros\n hors indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 70),
     main = "Rémunération indemnitaire annuelle\ndes fonctionnaires en " %+% année,
     col = "blue",
     nclass = 50)

#'
#'

if (longueur.non.na(filtre.fonctionnaire(part.rémunération.indemnitaire)) > 0)
  hist(filtre.fonctionnaire(part.rémunération.indemnitaire),
     xlab = "Pourcentage des indemnités dans la rémunération\n hors indemnités journalières et remboursements",
     ylab = "Effectif",
     main = "Part indemnitaire de la rémunération annuelle des fonctionnaires en " %+% année,
     xlim = c(0,60),
     col = "blue",
     nclass = 60)

#'
#'**Tests de cohérence**

AR <- Analyse.rémunérations.dernier.exercice[Statut == "TITULAIRE" | Statut == "STAGIAIRE",
                                                                    colonnes.sélectionnées ]

if (nrow(AR) > 0) {
   temp <- colSums(AR[ ,c("Montant.brut.annuel", "total.rémunérations")])
} else {
   temp <- c(0,0) }


#'Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :
#'

Tableau.vertical2(c("Agrégats", "euros"),
                  c("Bulletins de paie (euros)",
                    "Lignes de paie (euros)",
                    "Différence (euros)"),
                  c(temp["Montant.brut.annuel"],
                    temp["total.rémunérations"],
                    temp["Montant.brut.annuel"] - temp["total.rémunérations"]))


rm(temp)
#'
#'A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.
#'

#'
#'### Statistiques de position pour l'exercice `r année`
#'


Résumé(c("Traitement indiciaire",
         étiquette.rém.indemn,
         "Autres rémunérations"),
       AR[c("traitement.indiciaire", "rémunération.indemnitaire", "autres.rémunérations")])
#'

Résumé(c("Total rémunérations",
         "Part de la rémunération contractuelle ou indemnitaire",
         "Effectif"),
       AR[c("total.rémunérations", "part.rémunération.indemnitaire")],
       extra = "length")

#'
#'######
#'
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
            étiquette.rém.indemn,
           "Autres rémunérations"),
         ARA[c("traitement.indiciaire",
               "rémunération.indemnitaire",
               "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
  Résumé(c("Total rémunérations",
           "Part de la rémunération contractuelle ou indemnitaire"),
         ARA[ c( "total.rémunérations",
                 "part.rémunération.indemnitaire")])
}

#'**Effectif : `r nrow(ARA)`**
#'
#'**Catégorie B**
#'

if (fichier.personnels.existe)
{
  Résumé(c("Traitement indiciaire",
           étiquette.rém.indemn,
           "Autres rémunérations"),
         ARB[c("traitement.indiciaire",
               "rémunération.indemnitaire",
               "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
  Résumé(c("Total rémunérations",
           "Part de la rémunération contractuelle ou indemnitaire"),
         ARB[ c( "total.rémunérations",
                 "part.rémunération.indemnitaire")])
}
#'
#'**Effectif : `r nrow(ARB)`**
#'
#'**Catégorie C**
#'
#'

if (fichier.personnels.existe)
{
  Résumé(c("Traitement indiciaire",
           étiquette.rém.indemn,
           "Autres rémunérations"),
         ARC[ c("traitement.indiciaire",
                "rémunération.indemnitaire",
               "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{
  Résumé(c("Total rémunérations",
           "Part de la rémunération contractuelle ou indemnitaire"),
         ARC[ c( "total.rémunérations",
                 "part.rémunération.indemnitaire")])
}

#'
#'**Effectif : `r nrow(ARC)`**
#'
#'######
#'
#'
#'## 3.3 Contractuels, vacataires et stagiaires inclus
#'

temp <- total.rémunérations[   indemnités.élu == 0
                             & Statut != "TITULAIRE"
                             & Statut != "STAGIAIRE"
                             & total.rémunérations > 1000]/1000

if (longueur.non.na(temp) > 0)
  hist(temp,
     xlab = "Rémunération en milliers d'euros\n hors indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 40),
     main = "Rémunération annuelle totale des contractuels en " %+% année,
     col = "red",
     nclass = 50)

#'
#'**Nota:**
#'Ne sont retenues que les rémunérations supérieures à 1 000 euros.
#'Les élus ne sont pas pris en compte.

temp <- positive(autres.rémunérations)

if (longueur.non.na(temp) > 0)
  hist(temp,
     xlab = "En euros :\n indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 5000),
     main = "Autres rémunérations en " %+% année,
     col = "grey")
#'

AR <- Analyse.rémunérations.dernier.exercice[   indemnités.élu == 0
                                              & Statut != "TITULAIRE"
                                              & Statut != "STAGIAIRE",
                                              c("rémunération.indemnitaire",
                                                "autres.rémunérations",
                                                "total.rémunérations") ]

#'
Résumé(c(étiquette.rém.indemn,
         "Autres rémunérations",
         "Effectif"),
       AR[c("rémunération.indemnitaire", "autres.rémunérations")],
       extra = "length")

#'

Résumé(c("Total rémunérations",
         "Effectif"),
       AR["total.rémunérations"],
       extra = "length")

#'
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

Analyse.variations.synthèse.filtrée <- Analyse.variations.synthèse[nb.mois.exercice.début > seuil.troncature
                                                                   & nb.mois.exercice.sortie   > seuil.troncature
                                                                   & statut !=  "AUTRE_STATUT",
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

attach(Bulletins.paie.Lignes.paie, warn.conflicts = FALSE)

NBI.aux.non.titulaires <- Bulletins.paie.Lignes.paie[Statut != "TITULAIRE"
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

# variante : filtre <- regexpr(".*(INFO|PFI|P.F.I).*", toupper(Bulletins.paie.Lignes.paie$Libellé)) et regmatches(Bulletins.paie.Lignes.paie$Libellé, filtre)

filtre <- grep(".*(INFO|PFI|P.F.I).*", Libellé)

personnels.prime.informatique <- Bulletins.paie.Lignes.paie[ filtre,
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

lignes.fonctionnaires.et.vacations <- Bulletins.paie.Lignes.paie[ (Statut == "TITULAIRE" | Statut == "STAGIAIRE")
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

#'

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

    lignes.contractuels.et.vacations <- Bulletins.paie.Lignes.paie[Statut != "TITULAIRE"
                                                                   & Statut != "STAGIAIRE"
                                                                   & Codes.paiement.vacations[Code],
                                                                    c(étiquette.matricule,
                                                                      "Nom", "Prénom",
                                                                      étiquette.code,
                                                                      étiquette.libellé,
                                                                      étiquette.montant)]


   matricules.contractuels.et.vacations <- unique(lignes.contractuels.et.vacations[c("Matricule", "Nom", "Prénom")])

    nombre.contractuels.et.vacations <- nrow(matricules.contractuels.et.vacations)

                     RI.et.vacations <- Bulletins.paie.Lignes.paie[Codes.paiement.indemnitaire[Code] != 0
                                                                   & Matricule %in% matricules.contractuels.et.vacations$Matricule,
                                                                    c(étiquette.matricule,
                                                                      "Statut",
                                                                      étiquette.code,
                                                                      étiquette.libellé,
                                                                      étiquette.montant)]
# Vacations et indiciaire

traitement.et.vacations <- Bulletins.paie.Lignes.paie[Codes.paiement.traitement[Code] != 0
                                                      & Matricule %in% matricules.contractuels.et.vacations$Matricule,
                                                       c(étiquette.matricule,
                                                         "Statut",
                                                         étiquette.code,
                                                         étiquette.libellé,
                                                         étiquette.montant)]

nombre.Lignes.paie.contractuels.et.vacations <- nrow(lignes.contractuels.et.vacations)
nombre.Lignes.paie.RI.et.vacations <- nrow(RI.et.vacations)
nombre.Lignes.paie.traitement.et.vacations <- nrow(traitement.et.vacations)

#'
#'**Contractuels effectuant des vacations (CEV)**
#'
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

  ifts.logical <- grepl(expression.rég.ifts, Bulletins.paie.Lignes.paie$Libellé, ignore.case=TRUE)
  codes.ifts  <- levels(as.factor(Bulletins.paie.Lignes.paie[ifts.logical, étiquette.code]))
  personnels.iat.ifts <- intersect(Bulletins.paie.Lignes.paie[grepl(expression.rég.iat, Libellé, ignore.case=TRUE), clé.fusion[1]],
                                   Bulletins.paie.Lignes.paie[ifts.logical, clé.fusion[1]])
 names(personnels.iat.ifts) <- "Matricules des agents percevant IAT et/ou IFTS sur la période"

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

 lignes.ifts.anormales <- na.omit(Bulletins.paie.Lignes.paie[as.integer(Indice) < 350  & Code %in% codes.ifts,
                                                                  c(clé.fusion,
                                                                    "Statut",
                                                                    étiquette.code,
                                                                     étiquette.libellé,
                                                                     "Indice",
                                                                     étiquette.montant)])

nombre.lignes.ifts.anormales <- nrow(lignes.ifts.anormales)

# IFTS et non tit

ifts.et.contractuel <- Bulletins.paie.Lignes.paie[ Statut != "TITULAIRE"
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


HS.sup.25 <- Bulletins.paie.Lignes.paie[Heures.Sup. >= 25,
                                                 c(étiquette.matricule,
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
                                                   "montant.primes",
                                                   "montant.traitement.indiciaire")]

# version 1 : 48 s
# HS.sup.25 <- merge(HS.sup.25,
#                    ddply(Bulletins.paie.Lignes.paie,
#                          .(Matricule, Année, Mois),
#                          summarise,
#                          "Traitement indiciaire mensuel" = sum(montant.traitement.indiciaire),
#                          .progress = "tk"))

# version optimisée : 0,15 s soit x300


HS.sup.indiciaire.mensuel <- with(HS.sup.25, HS.sup.25[!is.na(montant.traitement.indiciaire) & montant.traitement.indiciaire != 0, c("Matricule", "Année", "Mois", "montant.traitement.indiciaire")])

HS.sup.25 <- with(HS.sup.25,
                  HS.sup.25[! is.na(montant.primes)
                            & montant.primes != 0
                            & ! grepl(".*SMIC.*",
                                     Libellé, ignore.case = TRUE)
                            & grepl(expression.rég.heures.sup,
                                      Libellé, ignore.case = TRUE), ])

HS.sup.25 <- with(HS.sup.25, HS.sup.25[order(Matricule, Année, Mois), c(étiquette.matricule,
                                                                        étiquette.année,
                                                                        "Statut",
                                                                        "Mois",
                                                                        "Libellé",
                                                                        étiquette.code,
                                                                        "Heures",
                                                                        "Heures.Sup.",
                                                                        "Base",
                                                                        "Taux",
                                                                        "Montant")])

# donne un tableau à 3 dimensions [Matricules, Années, Mois] dont les valeurs sont nommées par matricule
# bizarrement le hashage de la variable année se fait par charactère alors que le mois reste entier dans certaines exécutions et pas dana d'autres !
# Tout convertir en as.character() est plus prudent.

temp <- with(HS.sup.indiciaire.mensuel,
             tapply(montant.traitement.indiciaire, list(Matricule, Année, Mois), FUN=sum))

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

matricules.à.identifier <- unique(Bulletins.paie.Lignes.paie[ ,
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
                                                         "total.rémunérations") ]


      rémunérations.élu <- mutate(rémunérations.élu,
                                  total.rémunérations = indemnités.élu +  total.rémunérations)

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
#'[Lien vers la base de données fusionnées des bulletins et lignes de paie](Bases/Paiements/Bulletins.paie.Lignes.paie.csv)
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

detach(Bulletins.paie.Lignes.paie)

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
    "masses.premier",
    "masses.dernier")

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
            "Bulletins.paie.Lignes.paie",
            "Bulletins.paie")



