# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser
# les fichiers de paye produits au format spécifié par l'annexe de la  
# convention-cadre de dématérialisation en vigueur à partir de 2008.
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
# 
# 

PDF <<- TRUE

source("syspaths.R", encoding = "UTF-8")
# empiler d'bord les rmd dans un fichier out.Rmd
# (mettre l'entête dans script effetctifs)

source("altair_start.R", encoding = "UTF-8", echo = TRUE)
# il faudra fair un cat (..., file="out.Rmd", append=TRUE)
# puis rendre out.Rmd

source("rendre.R", encoding = "UTF-8")

# La fonction rendre() permet de générer le rapport pdf

rendre(to = "latex",
       args = c("-V", 
              "papersize=A4",  # taille du papier
              "-V",
              "geometry:top=2cm,bottom=1.5cm,left=2cm,right=1.5cm", # marges
              "-V",
              "urlcolor=cyan", # couleur des liens hypertexte
              "--highlight-style", # facultatif
              "tango"),
       output_file = "altair.tex",
       keep = TRUE)#fichiers.temp)

# Copie sur le sous-dossier de la clé (dossier R-Altair ou R-Altair/nom du dossier/
# en mode distributif)

file.copy("altair.pdf", chemin.clé)

# Lancer le lecteur de PDF à la fin
# Mais ne le faire que s'il ne s'agit pas du mode distributif(+)
# Il serait en effet non ergonomique de lancer la lecture de 50 fichiers PDF

if (ouvrir.document && basename(chemin.clé) == basename(racine)) {
  if (setOSWindows) {
    
    shell(paste("start acrord32.exe",  file.path(chemin.clé, "altair.pdf")))
    
  } else {
          system(paste("okular", file.path(chemin.clé, "altair.pdf")))
  }
}


