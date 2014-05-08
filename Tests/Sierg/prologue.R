# prologue 
# doit être dans le même répertoire que le programme principal et sa bibliothèque

windows <- TRUE

if (windows)
{ 
    racine <- "C:/"
    encodage.entrée <- "WINDOWS-1252"    
} else 
{
    racine <- "~/"
    encodage.entrée <- "UTF-8"    
}

chemin.dossier <- paste0(racine, "Dev/altair/Tests/Sierg")

ic <- function(x) ifelse(windows, iconv(x, to="UTF-8"), x)

début.période.sous.revue <- 2011
fin.période.sous.revue   <- 2012
étiquette.matricule <- "Matricule"
étiquette.Type.rémunération <- ic("Type rémunération")
étiquette.année <- ic("Année")
étiquette.libellé <- ic("Libellé")
étiquette.rém.indemn <- ic("Rémunération contractuelle ou indemnitaire")
champ.détection.1 <- étiquette.matricule
champ.détection.2 <- "Code"
date.format <- "%d/%m/%Y"