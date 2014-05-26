# prologue 
# doit être dans le même répertoire que le programme principal et sa bibliothèque

setOSWindows <- TRUE

fusionner.nom.prénom <- FALSE

    charger.bases <- TRUE
sauvegarder.bases <- FALSE
    générer.codes <- FALSE
tester.matricules <- TRUE
calculer.nb.jours <- TRUE

nom.fichier.paie  <- "Ville Annecy Paye BP-Lignes de paye"
nom.bulletin.paie <- "Ville Annecy Paye BP-Bulletins de paye"

    libellés.élus <- c("ELU", "ELUS", "Elu", "Elus", "élu", "élus")

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
                  champ.nir <- "Nir"
            codes.paiement  <- "codes.csv"

ifelse(fusionner.nom.prénom, 
       clé.fusion <<- c("Nom", "Prénom"),
       clé.fusion <<- étiquette.matricule)

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
