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

# Trouver le chemin de pandoc
find.pandoc <- function() {
  
  if (setOSWindows) {
     Sys.setenv(PATH = Sys.getenv("PATH")  %+%  ";C:\\Users\\Public\\Dev\\altair\\texlive2\\texmfs\\install\\miktex\\bin\\x64")
     repert <-  "C:\\Users\\Public\\Dev\\altair\\RStudio\\bin\\pandoc"
  } else {
     repert <- c("/usr/bin", "/usr/bin/pandoc", "/usr/local/bin", "/usr/local/bin/pandoc")
  }
  
  
  for (folder in repert) {
    p <- file.path(folder, "pandoc" %+% exec.root)
    if (file_test("-f", p)) return(p) 
  }
  
  return("")
}



#' Générer le fichier latex du rapport, puis le pdf ou le docx et l'odt
#' @param fw Largeur des figures
#' @param fh Hauteur des figures
#' @param d  Précision des graphiques en dpi
#' @param keep valeur booléenne : garder les fichiers intermédiaires md (par défaut la variable globale keep_md)
#' @param clean valeur booléenne : garder les autres fichiers intermédiaires (par défaut FALSE)
#' @param to Format de sortie (par défaut latex)
#' @param from Format d'entrée (par défaut une variété de markdown)
#' @param args Options à passer à pandoc
#' @param texfile Nom du fichier latex de sortie (altair.tex par défaut)
#' @param outfile Nom du premier fichier de sortie (pdf ou docx)
#' @param outfile2 Nom du deuxième fichier de sortie optionnel (par défaut odt si précisé)
#' @export

rendre <- function(fw = fig.width,
                   fh = fig.height,
                   d  = dpi,
                   keep = keep_md,
                   clean = FALSE,
                   to ="latex",
                   from = "markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures",
                   args = c("-V", 
                             "papersize=A4"), 
				   texfile = "altair.tex",			 
                   outfile = "altair.pdf",
				   outfile2 = "altair.odt") {

          rm(list = ls(), envir = globalenv())
          render_env <- new.env(parent = globalenv())
          essayer({         
			  render("altair_start.R",
					 encoding = encodage.code.source,
					 output_format = output_format(knitr_options(opts_chunk = list(fig.width = fw, 
																				   fig.height = fh,
																				   dpi = d,
																				   echo = FALSE,
																				   warning = FALSE,
																				   message = FALSE,
																				   results = 'asis')),
												   keep_md = TRUE, clean_supporting = clean,
												   pandoc = pandoc_options(to = "latex",
																		   from = from,
																		   args = args)),
					 envir = render_env,
					 output_file = outfile)
					 }, "Conversion pandoc imparfaite")
    
		 chemin_pandoc <- find.pandoc()            

		 if (chemin_pandoc != "") {          
			  if (to == "docx") {
					 generer_docx_odt(chemin_pandoc, texfile, outfile, outfile2)
			  } 

		 } else {
			cat("Impossible de trouver pandoc et de generer le rapport.")
		 }
		 if (! keep) file.remove(outfile)
				  
     chemin_pandoc
     
}
