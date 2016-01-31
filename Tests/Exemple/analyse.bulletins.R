
produire_pyramides <- function(Filtre_bulletins, titre) {
  
  Bulletins.avant <<-  unique(Bulletins.paie[Année == début.période.sous.revue
                                            & Mois == 12
                                            & Statut != "ELU"
                                            & Filtre_bulletins() == TRUE,
                                            c(clé.fusion, "Nir"), with=FALSE], by = NULL)
  
  Bulletins.après <<-  unique(Bulletins.paie[Année == fin.période.sous.revue
                                            & Mois == 12
                                            & Statut != "ELU"
                                            & Filtre_bulletins() == TRUE,
                                            c(clé.fusion, "Nir"), with=FALSE], by = NULL)
  
  avant <<- extraire.nir(Bulletins.avant, début.période.sous.revue)
  après <<- extraire.nir(Bulletins.après, fin.période.sous.revue)
  
  titre <<- titre
  
    if (! générer.rapport) {
    
      source("pyramides.R")
    
    } else {
    
      if (setOSWindows)  {                 
      
         cat(knit_child(text = readLines('pyramides.Rmd',
                                      encoding = encodage.code.source),
                     quiet=TRUE), 
             sep = '\n')
      } else {
       
        cat(knit_child(text = readLines('pyramides.utf8.Rmd',
                                        encoding = "UTF-8"),
                       quiet=TRUE), 
            sep = '\n')
      }
    }
  
  ### Uniquement utile pour l'output pdf...docx peut prendre "titre" sans filtrage
  
  stub <- gsub(" ", "-", sub("â", "a", titre)) %+% "_"
  
  nom.fichier.avant <<- stub %+% début.période.sous.revue
  nom.fichier.après <<- stub %+% fin.période.sous.revue
  
  ###
  
  Sauv.base(file.path(chemin.dossier.bases, "Effectifs"),  "avant", nom.fichier.avant)
  Sauv.base(file.path(chemin.dossier.bases, "Effectifs"),  "après", nom.fichier.après)
  
}


