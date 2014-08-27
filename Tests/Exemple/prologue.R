# prologue 
# doit être dans le même répertoire que le programme principal et sa bibliothèque

début.période.sous.revue    <- 2011
fin.période.sous.revue      <- 2012

# PARAMETRES GLOBAUX BOOLEENS ET ENTIERS

setOSWindows         <- TRUE
fusionner.nom.prénom <- FALSE
charger.bases        <- TRUE
sauvegarder.bases    <- FALSE
générer.codes        <- FALSE
tester.matricules    <- FALSE
corriger.quotité     <- FALSE
comportement.strict  <- TRUE
seuil.troncature     <- 3
taux.tolérance.homonymie <- 5  # en %

# FICHIERS EN INPUT
# conventions de nommage : les noms et chemins sont en minuscules ;
# les bases commencent par une majuscule. Un nom de fichier est souvent associé
# à une variable commençant par une majuscule et représentant la base (data.frame ou matrice)

racine                      <- "PEV CA-"
nom.fichier.codes.paiement  <- paste0(racine, "codes.csv")
fichier.personnels          <- "Catégories des personnels"
nom.fichier.personnels      <- paste0(fichier.personnels, ".csv")
nom.fichier.paie            <- paste0(racine, "Lignes de paye")
nom.bulletin.paie           <- paste0(racine, "Bulletins de paye")

# DOSSIERS

chemin.dossier              <- getwd()
chemin.dossier.bases        <- paste0(chemin.dossier, "/Bases")
chemin.dossier.données      <- paste0(chemin.dossier, "/Donnees")

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



date.format                 <- "%d/%m/%Y"

# ESPACES DE VALEURS LICITES POUR CERTAINS CHAMPS (modalités)

libellés.élus               <- c("ELU", "ELUS", "Elu", "Elus", "élu", "élus")
codes.NBI <- c("1012", "101B", "101M", "4652", "4672")

if (!setOSWindows)
{
  windows <- (Sys.info()["sysname"] != "Linux")
  
  if (windows)
  { 
    encodage.entrée <- "ISO-8859-1"    
  } else 
  {
    encodage.entrée <- "UTF-8"    
  }
}

modalité.traitement            <- "TRAITEMENT"     # s'applique aussi aux NBI
modalité.indemnitaire          <- "INDEMNITAIRE"   # hors vacations


modalité.principal.contractuel <- "PRINCIPAL.CONTRACTUEL" # contractuels qui ne sont pas payés par référénece à un traitement indiciaire.
modalité.élu                   <- "ELU"
modalité.vacations             <- "VACATIONS"
modalité.autres                <- "AUTRES"         # notamment les remboursements de frais professionnels, de transport et les restitutions/régularisations


# expressions régulières

# heures supplémentaires


expression.rég.heures.sup <- ".*(\\bI[[:alpha:]]*.?\\b.*\\bH[[:alpha:]]*.?\\b.*T[[:alpha:]]*.?.*|\\bH[[:alpha:]]*.?\\b.*(S|C)[[:alpha:]]*.?\\b).*"
expression.rég.iat        <- ".*(\\bI.?A.?T\\b|\\bI[[:alpha:]]*.?\\b.*\\bAd[[:alpha:]]*.?\\b.*Tec[[:alpha:]]*.?\\b).*"
expression.rég.ifts       <- ".*(\\bI.?F.?T.?S\\b|\\bI[[:alpha:]]*.?\\b.*\\bF[[:alpha:]]*.?\\b.*\\bTRAV[[:alpha:]]*.?\\b.*\\bS[[:alpha:]]*.?\\b).*"

