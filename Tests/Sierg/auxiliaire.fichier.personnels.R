
#INSTRUCTIONS : exécuter une première fois le programme principal
#Renseigner la catégorie de spersonnels en veillant à indiquer AUTRES pour le changements de catégorie
#Renseigner la catégorie une seule fois par matricule
#Exécuter le présent programme
#Vérifier l'absence d'erreur

# prologue 

chemin.dossier <- "~/Dev/altair/Tests/Sierg"
début.période.sous.revue <- 2011
fin.période.sous.revue   <- 2012
étiquette.matricule <- "Matricule"
champ.détection.1 <- étiquette.matricule
champ.détection.2 <- "Code"

# programme principal 
  
date.format <- "%d/%m/%Y"
source(file.path(chemin.dossier, "bibliotheque.fonctions.paie.R"), encoding="UTF-8-BOM")
chemin.fichier.personnels <- "Catégories des personnels.csv"
fichier.personnels.tmp <- read.csv.skip(chemin.fichier.personnels)
fichier.personnels <- unique(fichier.personnels.tmp[, étiquette.matricule])
longueur <- length(fichier.personnels)
fichier.personnels <- merge(fichier.personnels, fichier.personnels.tmp[, "Catégorie"], all=TRUE)

if (!see_if(nrow(fichier.personnels) != longueur))
{
  stop("Erreur d'appariement : il doit y avoir un matricule avec deux catégories différentes")
}

message("Appariement réussi")

write.csv2(fichier.personnels, chemin(chemin.fichier.personnels), row.names=FALSE)