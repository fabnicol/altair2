
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
date.format <- "%d/%m/%Y"

# programme principal 

source(file.path(chemin.dossier, "bibliotheque.fonctions.paie.R"), encoding="UTF-8-BOM")
chemin.fichier.personnels <- "Catégories des personnels.csv"
valeurs.catégories <- c('A', 'B', 'C', 'AUTRES', 'ELU')
invisible(file.copy(chemin(chemin.fichier.personnels), chemin("Catégories des personnels.tmp.csv"), overwrite=TRUE))

fichier.personnels.tmp <- read.csv.skip(chemin.fichier.personnels)
fichier.personnels <- unique(fichier.personnels.tmp[, étiquette.matricule])

colonne.catégories <- na.omit(fichier.personnels.tmp[fichier.personnels.tmp$Catégorie %in% valeurs.catégories, "Catégorie"])

if (see_if(length(colonne.catégories) != length(fichier.personnels)))
{
  stop("Erreur d'appariement : il doit y avoir un matricule avec deux catégories différentes")
}

fichier.personnels        <- data.frame(fichier.personnels, colonne.catégories)
names(fichier.personnels) <- c( étiquette.matricule, "Catégorie")

message("Appariement réussi")

write.csv2(fichier.personnels, chemin(chemin.fichier.personnels), row.names=FALSE)
