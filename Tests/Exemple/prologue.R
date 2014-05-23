# prologue 
# doit être dans le même répertoire que le programme principal et sa bibliothèque

windows <- (Sys.info()["sysname"] != "Linux")

if (windows)
{ 
    encodage.entrée <- "ISO-8859-1"    
} else 
{
    encodage.entrée <- "UTF-8"    
}


charger.bases <- TRUE
sauvegarder.bases <- TRUE
générer.codes <- FALSE

nom.fichier.paie  <- "PEV CA-Lignes de paye"
nom.bulletin.paie <- "PEV CA-Bulletins de paye"

chemin.dossier <- getwd()

chemin.dossier.bases <- paste0(chemin.dossier, "/Bases")

chemin.dossier.données <- paste0(chemin.dossier, "/Donnees")

  
début.période.sous.revue <- 2010
fin.période.sous.revue   <- 2011
étiquette.matricule <- "Matricule"
étiquette.Type.rémunération <- "Type rémunération"
étiquette.année <- "Année"
étiquette.libellé <- "Libellé"
étiquette.rém.indemn <- "Rémunération contractuelle ou indemnitaire"
champ.détection.1 <- étiquette.matricule
champ.détection.2 <- "Code"
date.format <- "%d/%m/%Y"