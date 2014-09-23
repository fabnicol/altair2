# prologue
# doit être dans le même répertoire que le programme principal et sa bibliothèque

# dans le cas où l'on ne lance le programme que pour certaines années, il préciser début.période sous revue et fin.période .sous.revue
# Dans ce cas fixer extraire.années en valeur TRUE.
# Sinon le programme travaille sur l'ensemble des années disponibles dans la base : elles sont détectées aurtomatiquement.

# PARAMETRES GLOBAUX BOOLEENS ET ENTIERS

extraire.années      <- TRUE
    début.période.sous.revue    <- 2008
    fin.période.sous.revue      <- 2009


setOSWindows         <- Sys.info()["sysname"] != "Linux"
exec.root             <- ifelse(setOSWindows, ".exe", "")

générer.codes        <- FALSE

paralléliser         <- TRUE
table.rapide         <- TRUE
extraire.population  <- FALSE
fusionner.nom.prénom <- FALSE
charger.bases        <- TRUE
sauvegarder.bases.analyse    <- TRUE
sauvegarder.bases.origine    <- FALSE
générer.table.effectifs <- FALSE
tester.matricules            <- FALSE
tester.lignes.bulletins.mois <- FALSE
corriger.quotité     <- FALSE
comportement.strict  <- TRUE
etp.égale.effectif   <- FALSE

seuil.troncature     <- 3
taux.tolérance.homonymie <- 5  # en %

séparateur.liste <- ","
séparateur.décimal <- "."

# FICHIERS EN INPUT
# conventions de nommage : les noms et chemins sont en minuscules ;
# les bases commencent par une majuscule. Un nom de fichier est souvent associé
# à une variable commençant par une majuscule et représentant la base (data.frame ou matrice)

racine                      <- "metro2008-"
  # "Ville Annecy Paye BP-"
  # "UTF-8.PDC-"
  # "RAG_2009_2012-"
  # "UTF-8.RAG_2009_2012-"
  # "UTF-8.SIERG-"
  # "UTF-8.RAG_2009_2012-"
  # "RAG_2009_2012-"
nom.fichier.codes.paiement  <- paste0(racine, "codes.csv")
fichier.personnels          <- "Catégories des personnels"
nom.fichier.personnels      <- paste0(fichier.personnels, ".csv")
nom.fichier.paie            <- paste0(racine, "Lignes de paye")
nom.bulletin.paie           <- paste0(racine, "Bulletins de paye")

# DOSSIERS
# Attention, si l'on lance la génération de document pour la première fois sans répertoire Bases existant, on aura un write error.
# pour les applications à ergonomie facilitée, prévoir de distribuer le logiciel avec un dossier Bases déjà généré.

chemin.dossier              <- getwd()
chemin.dossier.bases        <- file.path(chemin.dossier, "Bases")
chemin.dossier.outils       <- file.path(chemin.dossier, "..", "..", "Outils")
chemin.dossier.données      <- file.path(chemin.dossier, "Donnees")

# Outils

iconv <- file.path(chemin.dossier.outils, paste0("iconv", exec.root))
sed   <- file.path(chemin.dossier.outils, paste0("sed", exec.root))

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
champ.nir                   <- "Nir"

ifelse(fusionner.nom.prénom,
       clé.fusion <<- c("Nom", "Prénom"),
       clé.fusion <<- étiquette.matricule)

colonnes.requises           <- c(union(clé.fusion, étiquette.matricule),
                                 étiquette.année,
                                 "Mois",
                                 "Statut",
                                 "Brut",
                                 "Net.à.Payer",
                                 "Heures.Sup.",
                                 "Heures",
                                 "Emploi",
                                 champ.nir,
                                 "Temps.de.travail")

bulletins.paie.input <- c("Année", "Mois", "Nom", "Prénom", "Matricule", "Statut", "Emploi", "Service", "Indice", "Net", "Brut", "Net.à.Payer", "Heures.Sup.", "Heures", "Temps.de.travail", "Nir", "Grade")

bulletins.paie.classes.input    <- c("integer", "integer", "character", "character", "character", "character", "character", "character", "character", "numeric", "numeric", "numeric", "numeric", "numeric", "numeric", "character", "character")

bulletins.paie.output <- c(bulletins.paie.input, c("Montant.net.annuel.eqtp", "Montant.brut.annuel", "mois.entrée", "mois.sortie", "nb.jours", "nb.mois"))

bulletins.paie.classes.output <- c(bulletins.paie.classes.input, c("numeric", "integer", "integer", "integer", "integer"))

lignes.paie.input <- c("Année", "Mois", "Matricule", "Année", "Mois", "Matricule", "Libellé", "Code", "Base", "Taux", "Montant")

lignes.paie.input.fallback <- c("Année", "Mois", "Matricule", "Libellé", "Code", "Base", "Taux", "Montant")

lignes.paie.classes.input <- c("integer", "integer", "character", "integer", "integer", "character", "character", "character", "numeric", "numeric", "numeric")

lignes.paie.classes.input.fallback <- c("integer", "integer", "character", "character", "character", "numeric", "numeric", "numeric")

date.format                 <- "%d/%m/%Y"

# ESPACES DE VALEURS LICITES POUR CERTAINS CHAMPS (modalités)

#libellés.élus               <- c("Elu", "Elus", "élu", "élus", "maire", "président", "adjoint au maire", "vice-président", "vice président")
codes.NBI <- c("1012", "101B", "101M", "4652", "4672")

# A priori les deux modes de lectures de tables (rapide et standard) lisent aussi bien le Windows ANSI/ISO-8859-1 que 
# l'UTF-8 à condition que le Windows ANSI soit encodé par Excel ou l'éditeur de RStudio.

encodage.entrée <- "WINDOWS-1252"
  # "WINDOWS-1252"
  # "UTF-8"
  # "ISO-8859-1"
  
encodage.sortie <- ifelse(setOSWindows, "ISO-8859-1", encodage.entrée)

modalité.traitement            <- "TRAITEMENT"     # s'applique aussi aux NBI
modalité.indemnitaire          <- "INDEMNITAIRE"   # hors vacations


modalité.principal.contractuel <- "PRINCIPAL.CONTRACTUEL" # contractuels qui ne sont pas payés par référénece à un traitement indiciaire.
modalité.élu                   <- "ELU"
modalité.vacations             <- "VACATIONS"
modalité.autres                <- "AUTRES"         # notamment les remboursements de frais professionnels, de transport et les restitutions/régularisations


# expressions régulières


expression.rég.heures.sup <- ".*(I.?H.?T|H.?[SC]|\\bI[[:alpha:]]*.?.*\\bH[[:alpha:]]*.?\\b.*T[[:alpha:]]*.?.*|\\bH[[:alpha:]]*.?\\b.*[SC][[:alpha:]]*.?\\b).*"
expression.rég.iat        <- ".*(\\b[Ii].?[Aa].?[Tt]\\b|\\b[Ii][[:alpha:]]*.?\\b.*\\b[Aa][Dd][[:alpha:]]*.?\\b.*[Tt][Ee][Cc][[:alpha:]]*.?\\b).*"
expression.rég.ifts       <- ".*(\\b[Ii].?[Ff].?[Tt].?[Ss]\\b|\\b[Ii][[:alpha:]]*.?\\b.*\\b[Ff][[:alpha:]]*.?\\b.*\\b[Tt][Rr][Aa][Vv][[:alpha:]]*.?\\b.*\\b[Ss][[:alpha:]]*.?\\b).*"
expression.rég.population <- ".*\\bASS(\\b|A).*"
expression.rég.élus       <- ".*([eé]lu[s]?|adj.*maire|pr[eé]sident|cons.*muni.*).*"

