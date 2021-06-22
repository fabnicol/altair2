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
     Sys.setenv(PATH = Sys.getenv("PATH")  %+%  ";C:\\Users\\Public\\Dev\\altair\\texlive2\\texmfs\\install\\miktex\\bin\\x64"  %+%  ";C:\\Users\\Public\\Altair\\texlive2\\texmfs\\install\\miktex\\bin\\x64")
     repert <-  c("C:\\Users\\Public\\Dev\\altair\\RStudio\\bin\\pandoc", "C:\\Users\\Public\\Altair\\RStudio\\bin\\pandoc")
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
#' @param filename Racine du nom du fichier de sortie (altair par défaut)
#' @param verbose Par défaut, 0. Si fixé à 1, 2, 3, augmente la verbosité progressivement.
#' @param sync Booléen. Décrit le caractère synchone ou asynchrone de la génération des éléments de rapport. Géré par le système selon le paramètre global #sequentiel
#' @return Retourne le vecteur des chemins des fichiers .tex temporaires
#' @export

rendre <- function(fw = fig.width,
                   fh = fig.height,
                   d  = dpi,
                   keep = keep_md,
                   clean = FALSE,
                   to ="latex",
                   from = "markdown-auto_identifiers+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures",
                   args = c("-M", "lang=fr", "-V", "papersize=A4", "-V", "geometry:top=2cm,bottom=1.5cm,left=2cm,right=1.5cm", "-V", "urlcolor=cyan", "--highlight-style", "tango"),
                   filename = "altair",
        				   verbose = 0,
				           sync = sequentiel) {

          rm(list = ls(), envir = globalenv())
          essayer({
                    knitr::opts_chunk$set(echo = (verbose >= 1), warning = (verbose >= 2), message = (verbose >= 3))
                    assign("chemin_pandoc", find.pandoc(), envir = .GlobalEnv)

            			  render("altair_start.R",
            					 encoding = encodage.code.source,
            					 output_format = output_format(knitr_options(opts_chunk = list(fig.width = fw,
            																				   fig.height = fh,
            																				   dpi = d,
            																				   echo = (verbose >= 1),
            																				   warning = (verbose >= 2),
            																				   message = (verbose >= 3),
            																				   results = 'asis')),
            												   keep_md = keep, clean_supporting = clean,
            												   pandoc = pandoc_options(to = "latex",
            																		   from = from,
            																		   args = args)),
            					 envir = .GlobalEnv,
            					 output_file = filename %+% ".pdf")
            					 }, "Conversion pandoc imparfaite")

        texfile <-  filename %+% ".tex"

        if (! sync)   texfile <- c(texfile, list.files(chemin.dossier, "modules.*\\.tex"))

        pandoc <- get("chemin_pandoc", envir = .GlobalEnv)

    		 if (pandoc != "") {

    		   for (type in to) {
      		   if (type == "docx") {

      		       outfile <-  filename %+% ".docx"
      		       outfile2 <- filename %+% ".odt"
      		       assign("PDF", FALSE, envir = .GlobalEnv)
      					 generer_docx_odt(infile = texfile, outfile, outfile2)

      			  } else {

      			     outfile <-  filename %+% ".pdf"
      			     assign("PDF", TRUE, envir = .GlobalEnv)
      			     tex2pdf(texfile, outfile, args, keep = (length(to) > 1 || keep))

      			  }
    		   }

    		 } else {
    			stop("Impossible de trouver pandoc et de generer le rapport.")
    		 }

    		 if (! keep) file.remove(outfile)
        texfile
}
