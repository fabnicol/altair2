# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, annees 2012 à 2017
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

#' Convertir le fichier tex du rapport en pdf
#' @param chemin_pandoc Chemin complet vers l'exécutable pandoc
#' @param infile  le nom de sortie du fichier, par défaut altair.tex
#' @param outfile le nom de sortie du fichier, par défaut altair.pdf
#' @param args Arguments à passer à pandoc comme options de forme
#' @export

tex2pdf <- function(chemin_pandoc, infile = "altair.tex", outfile = "altair.pdf", 
                    args = c("-V", "papersize=A4", "-V", "geometry:top=2cm,bottom=1.5cm,left=2cm,right=1.5cm", "-V", "urlcolor=cyan", "--highlight-style", "tango")) {
 
  system2(chemin_pandoc, c(infile, args, "-o", outfile))
  
  cleanup()
  
  file.copy(outfile, chemin.cle)
  
  cle_outfile <- file.path(chemin.cle, outfile)

  if (ouvrir.document && file.exists(cle_outfile)) {
    if (setOSWindows) {
      
       shell(paste("start acrord32.exe",  cle_outfile))
      
    } else {
      system(paste("okular", file.path(chemin.cle, outfile)))
    }
  }
}

#' Générer le fichier pdf du rapport
#' @param infile  le nom de sortie du fichier, par défaut altair.tex
#' @param outfile le nom de sortie du fichier, par défaut altair.pdf
#' @param args Arguments à passer à pandoc comme options de forme
#' @return Retourne e chemin vers l'exécutable pandoc
#' @export

generer_pdf <- function(infile = "altair.tex", outfile = "altair.pdf", args = c("-V", "papersize=A4", "-V", "geometry:top=2cm,bottom=1.5cm,left=2cm,right=1.5cm", "-V", "urlcolor=cyan", "--highlight-style", "tango")) {

  PDF <<- TRUE
    					
  chemin_pandoc <- rendre(	args = args,
							texfile = infile, 
							outfile = outfile, 
							keep = TRUE)

  tex2pdf(chemin_pandoc, infile, outfile, args)					 
  chemin_pandoc
}

