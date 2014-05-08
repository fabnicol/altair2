# prologue 
# doit être dans le même répertoire que le programme principal et sa bibliothèque

windows <- FALSE

if (windows)
{ 
    racine <- "C:/"
    encodage.entrée <- "CP-1252"    
}
else
{
    racine <- "~/"
    encodage.entrée <- "UTF-8"    
}

chemin.dossier <- paste0(racine, "Dev/altair/Tests/Sierg")

début.période.sous.revue <- 2011
fin.période.sous.revue   <- 2012
étiquette.matricule <- "Matricule"
champ.détection.1 <- étiquette.matricule
champ.détection.2 <- "Code"
date.format <- "%d/%m/%Y"