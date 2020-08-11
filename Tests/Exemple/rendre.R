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

chemin_pandoc <- find.pandoc()

rendre <- function(fw = fig.width,
                   fh = fig.height,
                   d  = dpi,
                   keep = keep_md,
                   clean = FALSE,
                   to ="docx",
                   from = "markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures",
                   args = c("-V", 
                             "papersize=A4" ), 
                   output_file = "altair.docx") {

          rm(list = ls(), envir = globalenv())
          render_env <- new.env(parent = globalenv())
          
          altair_start <- ifelse(to == "docx", "temp.R", "altair_start.R")
          
          if (to == "docx") {
            if (file.exists("temp.R")) file.remove("temp.R")
            V <- readLines("altair_start.R", encoding = encodage.code.source)
            V <- gsub("![Notice](Notice.png)", "Notice", V, fixed = TRUE, useBytes = TRUE)
            
            writeLines(V,  "temp.R")
          }
          
          render(altair_start,
                 encoding = encodage.code.source,
                 output_format = output_format(knitr_options(opts_chunk = list(fig.width = fw, 
                                                                               fig.height = fh,
                                                                               dpi = d,
                                                                               echo = FALSE,
                                                                               warning = FALSE,
                                                                               message = FALSE,
                                                                               results = 'asis')),
                                               keep_md = TRUE, clean_supporting = clean,
                                               pandoc = pandoc_options(to = to,
                                                                       from = from,
                                                                       args = args)),
                 envir = render_env,
                 output_file = output_file)
          
          if (to == "docx") {
            file.remove("temp.R") 
          } else {
            if (chemin_pandoc != "") {
               if (to == "latex") {
                  cat(chemin_pandoc, output_file, "-o", "altair.pdf", args)
                  system2(chemin_pandoc, c(output_file, "-o", "altair.pdf", args))
               }
            } else {
              cat("Impossible de trouver pandoc et de generer le pdf.")
            }
            if (! keep) file.remove(output_file)
          }
          
          invisible(render_env)
}


## début de hack ## 

# Hack relativement laid mais très efficace qui évite de refaire tourner render pour la fabrication d'un .md
# en recalculant les mêmes choses afin de produire le nouveau format.
# On utilise le .md produit par keep_md = TRUE plus haut et les images conservées grâce à clean_supporting = FALSE
# Mais il faut réinserer un nouveau YAML header pour effacer les noms, dates etc. standard
# et aussi effacer un FALSE indésirable produit par render en tête de .md (qui correspond au traitement du premier YAML header du pdf)
# Les autres alternatives ont été essayées et échouent. La production directe de .md par render échoue sur la question
# des liens URL comportant un caractère non-ascii.

# attention encodage UTF-8 obligatoire quelle que soit la plateforme


hack_md <- function() {
  
  con <- file("altair.2.md", open = "a", encoding = encodage.code.source)
  
  writeLines(text = "\n---\ntitle: false\nauthor: false\ndate: false\n---\n", con)     
  
  V <- readLines("altair.md", encoding = encodage.code.source)[-c(1, 2)]
  
  writeLines(V, con)
  
  close(con)
  file.rename("altair.2.md", "altair.md")
  
  V
}

# A réévaluer en fonction de l'évolution de la bibliothèque rmarkdown et de pandoc

## fin de hack ##

## Les deux fonctions sont appelées successivement ce qui permet d'identifier V
## Le hack est encore pire que le précédent puisqu'il n'est justifié que pour Office Writer et en raison d'un bug 
## de celui ci relatif à la résolution des chemins ! Autant dire qu'il faudra surveiller l'évolution du problème

ajuster_chemins_odt <- function(V) {

  # garde en principe inutile
  
  if (! is.null(V))  {
    con <- file("altair.2.md", open = "a", encoding = encodage.code.source)
  
    writeLines(text = "\n---\ntitle: false\nauthor: false\ndate: false\n---\n", con)   
 
    writeLines(gsub("]\\((Docs|Bases)", "]\\(../\\1", V, perl = TRUE), con)
  
    close(con)
    file.rename("altair.2.md", "altair.md")
  }
}


