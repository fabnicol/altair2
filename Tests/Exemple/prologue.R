# prologue
# doit être dans le même répertoire que le programme principal et sa bibliothèque

# dans le cas où l'on ne lance le programme que pour certaines années, il préciser début.période sous revue et fin.période .sous.revue
# Dans ce cas fixer extraire.années en valeur TRUE.
# Sinon le programme travaille sur l'ensemble des années disponibles dans la base : elles sont détectées automatiquement.

# PARAMETRES GLOBAUX BOOLEENS ET ENTIERS


racine                      <- ifelse(setOSWindows, "R-Altaïr/", "R-Altair/")
# "Ville Annecy Paye BP-"
# "UTF-8.PDC-"
# "RAG_2009_2012-"
# "UTF-8.RAG_2009_2012-"
# "UTF-8.SIERG-"
# "UTF-8.RAG_2009_2012-"
# "RAG_2009_2012-"

extraire.années      <- F
  début.période.sous.revue    <- 2013
  fin.période.sous.revue      <- 2014

exec.root               <- if (setOSWindows) ".exe" else ""

éliminer.duplications   <- F
redresser.heures        <- TRUE
enlever.quotités.nulles <- F
enlever.quotités.na     <- F
écreter.quotités        <- T
générer.codes           <- FALSE
charger.catégories.personnel <- TRUE
extraire.population     <- FALSE
charger.bases           <- T
sauvegarder.bases.analyse    <- T
sauvegarder.bases.origine    <- F
afficher.table.effectifs      <- F
générer.table.élus           <- T
tester.matricules            <- TRUE
analyse.statique.totale      <- F
corriger.environnement.système <- (setOSWindows == TRUE)
intégrer.localisation <- FALSE   # Veut on gardet Budget Employeur Siret Etablissement ?
afficher.cumuls.détaillés.lignes.paie <- FALSE
afficher.table.écarts.sft <- FALSE
analyse.par.catégorie     <- TRUE
test.delta                <- TRUE


seuil.troncature         <- 1 # jours
taux.tolérance.homonymie <- 2  # en %
quantile.cut             <- 1  # en %
minimum.positif          <- 0.5
minimum.quotité          <- 0.1
minimum.actif            <- 100
minimum.delta            <- 5

population_minimale_calcul_médiane  <- 3
tolérance.sft <- 1

séparateur.liste.entrée     <- ";"
séparateur.décimal.entrée   <- ","
séparateur.liste.sortie     <- ";"
séparateur.décimal.sortie   <- ","

# FICHIERS EN INPUT
# conventions de nommage : les noms et chemins sont en minuscules ;
# les bases commencent par une majuscule. Un nom de fichier est souvent associé
# à une variable commençant par une majuscule et représentant la base (data.frame ou matrice)

liste.exclusions <- NULL

try ({
  chemin.liste.exclusions <- chemin("liste.exclusions.txt")
  if (file.exists(chemin.liste.exclusions))
    liste.exclusions <- read.delim(chemin.liste.exclusions, encoding=encodage.code.source, stringsAsFactors = FALSE)[[1]]
}, silent=TRUE)


nom.fichier.codes.paiement  <- paste0(racine, "codes.csv")
nom.fichier.paie            <- paste0(racine, "Lignes de paye")
nom.bulletin.paie           <- paste0(racine, "Bulletins de paye")
nom.table                   <- paste0(racine, "Table.csv")
nom.bulletins               <- paste0(racine, "Bulletins.csv")

# DOSSIERS
# Attention, si l'on lance la génération de document pour la première fois sans répertoire Bases existant, on aura un write error.
# pour les applications à ergonomie facilitée, prévoir de distribuer le logiciel avec un dossier Bases déjà généré.

chemin.dossier              <- getwd()
chemin.dossier.bases        <- file.path(chemin.dossier, "Bases")
chemin.dossier.outils       <- file.path(chemin.dossier, "..", "..", "Outils")
chemin.dossier.lib          <- file.path(chemin.dossier, "..", "..", "lib")
chemin.dossier.données      <- file.path(chemin.dossier, "Donnees")

# Outils

if (setOSWindows) {
  iconv <- file.path(chemin.dossier.outils, paste0("iconv", exec.root))
  sed   <- file.path(chemin.dossier.outils, paste0("sed", exec.root))
} else {
  iconv <- "iconv"
  sed   <- "sed"
}


# ETIQUETTES ET FORMATS

étiquette.matricule         <- "Matricule"
étiquette.Type.rémunération <- "Type rémunération"
étiquette.année             <- "Année"
étiquette.libellé           <- "Libellé"
étiquette.montant           <- "Montant"
étiquette.code              <- "Code"
étiquette.rém.indemn        <- "Rémunération contractuelle ou indemnitaire"
champ.détection.1           <- étiquette.matricule
champ.détection.2           <- "Code"

clé.fusion <- étiquette.matricule

colonnes.requises           <- c(union(clé.fusion, étiquette.matricule),
                                 étiquette.année,
                                 "Mois",
                                 "Statut",
                                 "Brut",
                                 "Net.à.Payer",
                                 "Heures.Sup.",
                                 "Heures",
                                 "Emploi",
                                 "Nir",
                                 "Temps.de.travail")

localisation <- if (intégrer.localisation) c("Budget", "Employeur", "Siret", "Etablissement") else NULL
localisation.classes <- if (intégrer.localisation) c("character", "character", "character", "character") else NULL

colonnes.input <- c("Année", "Mois", 
                    localisation,
                    "Nom", "Prénom", "Matricule",
                    "Service", "NbEnfants", "Statut", "Temps.de.travail", "Heures.Sup.", "Heures",
                    "Indice", "Brut", "Net", "Net.à.Payer", "NBI",
                    "Libellé", "Code", "Base", "Taux", "Nb.Unité",
                    "Montant", "Type", "Emploi", "Grade", "Catégorie", "Nir")

colonnes.classes.input    <- c("integer", "integer",  
                               localisation.classes,
                               "character", "character", "character",
                               "character", "numeric", "character", "numeric", "numeric", "numeric",
                               "character",  "numeric", "numeric", "numeric", "numeric",
                               "character",  "character", "numeric", "numeric", "numeric",
                               "numeric", "character", "character", "character", "character", "character")

colonnes.bulletins.input <- c("Année", "Mois", 
                              localisation,
                              "Nom", "Prénom", "Matricule",
                              "Service", "NbEnfants", "Statut", "Temps.de.travail", "Heures.Sup.", "Heures",
                              "Indice", "Brut", "Net", "Net.à.Payer", "NBI",
                              "Emploi", "Grade", "Catégorie", "Nir")

colonnes.bulletins.classes.input <- c("integer", "integer",
                                      localisation.classes,
                                      "character", "character", "character",
                                      "character", "numeric", "character", "numeric", "numeric", "numeric",
                                      "character",  "numeric", "numeric", "numeric", "numeric",
                                      "character", "character", "character", "character")

date.format                      <- "%d/%m/%Y"

# ESPACES DE VALEURS LICITES POUR CERTAINS CHAMPS (modalités)

#libellés.élus               <- c("Elu", "Elus", "élu", "élus", "maire", "président", "adjoint au maire", "vice-président", "vice président")

########## Problématique ##############
#codes.NBI <- c("1012", "101B", "101M", "4652", "4672")


# A priori les deux modes de lectures de tables (rapide et standard) lisent aussi bien le Windows ANSI/ISO-8859-1 que
# l'UTF-8 à condition que le Windows ANSI soit encodé par Excel ou l'éditeur de RStudio.

encodage.entrée <-  "ISO-8859-1"
  # "WINDOWS-1252"
  # "UTF-8"
  
encodage.entrée.xhl2csv <-  "ISO-8859-1"

encodage.sortie <- "ISO-8859-15"

modalité.traitement            <- "TRAITEMENT"     # s'applique aussi aux NBI
modalité.indemnitaire          <- "INDEMNITAIRE"   # hors vacations


modalité.principal.contractuel <- "PRINCIPAL.CONTRACTUEL" # contractuels qui ne sont pas payés par référénece à un traitement indiciaire.
modalité.élu                   <- "ELU"
modalité.vacations             <- "VACATIONS"
modalité.autres                <- "AUTRES"         # notamment les remboursements de frais professionnels, de transport et les restitutions/régularisations


# expressions régulières

#\\bxyz veut aussi dire cde.xyz(...), à éviter

expression.rég.heures.sup <- "(?:^|\\s)iht?[sc]|i(?:(?=n)n\\w*|)\\.?\\s*\\bh(?:(?=o)o\\w*|)\\.?.*\\bs(?:(?=u)u\\w*|)\\.?\\b|h(?:(?=e)e\\w*|)\\.?\\s*\\b(?:(?=s)s\\w*|(?:(?=c)c\\w*|))\\.?\\b"
expression.rég.iat        <- "(?:^|\\s)iat|i(?:(?=n)n\\w*|)\\.?\\s*\\ba(?:(?=d)d\\w*|)\\.?\\s*\\b(?:et\\s*\\b)?t(?:(?=e)e\\w*|)\\.?\\b"
expression.rég.ifts       <- "(?:^|\\s)ifts|i(?:(?=n)n\\w*|)\\.?\\s*\\bf\\w*\\.?\\s*\\bt(?:(?=r)r\\w*|)\\.?\\s*\\bs(?:(?=u)u\\w*|)\\.?\\b"
expression.rég.pfr        <- "(?:^|\\s)pfr|p(?:(?=r)r\\w*|)\\.?\\s*\\bf(?:(?=o)o\\w*|)\\.?\\s*(?:et|)\\s*\\br(?:.?.?su\\w*|)\\.?\\s*\\b"
expression.rég.pfi        <- "(?:^|\\s)pfi|p(?:(?=r)r\\w*|)\\.?\\s*\\bf(?:(?=o)o\\w*|)\\.?\\s*\\bi(?:(?=n)n\\w*|)\\.?\\b"
expression.rég.population <- "\\bASS(\\b|A).*"
expression.rég.élus       <- "maire|pr[eé]sident|.*([eé]lu[s]?|adj.*maire|v\\w*.*\\s+pr[eé]sident|cons\\w*.*\\s+muni|cons\\w*.*\\s+commun)"
expression.rég.nbi        <- "(?:\\s|^)nbi|(?:n(?:(?=o)o\\w*|)\\.?\\s*\\b)?b(?:(?=o)o\\w*|)\\.?\\s*\\bi(?:(?=n)n\\w*|)\\.?\\b"
expression.rég.attaché    <- "(?:^|\\s)att\\w*\\.?\\b|se\\w*\\.?\\bma\\w*\\.?\\b"
expression.rég.attaché.p  <- "(?:^|\\s)att\\w*\\.?\\bpr\\w*\\.?\\b|dir\\w*\\.?\\b"
expression.rég.admin      <- "(?:^|\\s)adm\\w*\\.?\\b"
expression.rég.admin.hc   <- "(?:^|\\s)adm\\w*\\.?\\bh\\w*\\.?\\s?c\\w*\\.?\\b"
expression.rég.admin.g    <- "(?:^|\\s)adm\\w*\\.?\\b\\s?g\\w*\\.?\\b"

# point d'indice majoré
source("point.im.R")
