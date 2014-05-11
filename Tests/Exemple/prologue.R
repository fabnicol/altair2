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

chemin.dossier <- getwd()


# ne pas utiliser ifelse
if (windows) 
{
  ic <<- function(x) iconv(x, to="UTF-8") 
} else
  ic <<- x
  
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