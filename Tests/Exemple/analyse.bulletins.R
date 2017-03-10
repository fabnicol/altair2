# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
# produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
# en vigueur à compter de l'année 2008.
# 
# Ce logiciel est régi par la licence CeCILL soumise au droit français et
# respectant les principes de diffusion des logiciels libres. Vous pouvez
# utiliser, modifier et/ou redistribuer ce programme sous les conditions
# de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
# sur le site "http://www.cecill.info".
# 
# En contrepartie de l'accessibilité au code source et des droits de copie,
# de modification et de redistribution accordés par cette licence, il n'est
# offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
# seule une responsabilité restreinte pèse sur l'auteur du programme, le
# titulaire des droits patrimoniaux et les concédants successifs.
# 
# A cet égard l'attention de l'utilisateur est attirée sur les risques
# associés au chargement, à l'utilisation, à la modification et/ou au
# développement et à la reproduction du logiciel par l'utilisateur étant
# donné sa spécificité de logiciel libre, qui peut le rendre complexe à
# manipuler et qui le réserve donc à des développeurs et des professionnels
# avertis possédant des connaissances informatiques approfondies. Les
# utilisateurs sont donc invités à charger et tester l'adéquation du
# logiciel à leurs besoins dans des conditions permettant d'assurer la
# sécurité de leurs systèmes et ou de leurs données et, plus généralement,
# à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
# 
# Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
# pris connaissance de la licence CeCILL, et que vous en avez accepté les
# termes.
# 
# 

produire_pyramides <- function(Filtre_bulletins, titre, versant = "") {

  Bulletins.avant <<-  unique(Bulletins.paie[Année == début.période.sous.revue
                                            & Mois == 12
                                            & Statut != "ELU"
                                            & Filtre_bulletins() == TRUE,
                                            c(clé.fusion, "Nir"), with=FALSE], by = NULL)

  année.après <<- if (versant != "") {
                      max(début.période.sous.revue, min(altair::année_comparaison(versant)$année, fin.période.sous.revue))
                  } else fin.période.sous.revue
  
  Bulletins.après <<-  unique(Bulletins.paie[Année == année.après
                                            & Mois == 12
                                            & Statut != "ELU"
                                            & Filtre_bulletins() == TRUE,
                                            c(clé.fusion, "Nir"), with=FALSE], by = NULL)
  
  avant <<- extraire.nir(Bulletins.avant, début.période.sous.revue)
  après <<- extraire.nir(Bulletins.après, année.après)
  
  titre <<- titre
  
  versant <<- versant

    if (! générer.rapport) {
    
      source("pyramides.R", encoding = encodage.code.source)
    
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


