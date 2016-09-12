


rendre <- function(fw = fig.width,
                   fh = fig.height,
                   d  = dpi,
                   keep = TRUE,
                   clean = FALSE,
                   to ="docx",
                   from = "markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures",
                   args = c("-V", 
                             "papersize=A4" ), 
                   output_file = "altair.docx") {

          rm(list = ls(), envir = globalenv())
          render_env <- new.env(parent = globalenv())
          
          render("altair.R",
                 encoding = "UTF-8",
                 output_format = output_format(knitr_options(opts_chunk = list(fig.width = fw, 
                                                                               fig.height = fh,
                                                                               dpi = d,
                                                                               echo = FALSE,
                                                                               warning = FALSE,
                                                                               message = FALSE,
                                                                               results = 'asis')),
                                               keep_md = keep, clean_supporting = clean,
                                               pandoc = pandoc_options(to = to,
                                                                       from = from,
                                                                       args = args)),
                 envir = render_env,
                 output_file = output_file)
          
          invisible(render_env)
}


## début de hack ## 

# Hack relativement laid mais très efficace qui évite de refaire tourner render pour la fabrication d'un .md
# en recalculant les mêmes choses afin de produire le nouveau format.
# On utilise le .md produit par keep_md = TRUE plus haut et les images conservées grâce à clean_supporting = FALSE
# Mais il faut réinsérer un nouveau YAML header pour effacer les noms, dates etc. standard
# et aussi effacer un FALSE indésirable produit par render en tête de .md (qui correspond au traitement du premier YAML header du pdf)
# Les autres alternatives ont été essayées et échouent. La production directe de .md par render échoue sur la question
# des liens URL comportant un caractère non-ascii.

# attention encodage UTF-8 obligatoire quelle que soit la plateforme


hack_md <- function() {
  
  con <- file("altair.2.md", open = "a", encoding = "UTF-8")
  
  writeLines(text = "\n---\ntitle: false\nauthor: false\ndate: false\n---\n", con)     
  
  V <- readLines("altair.md", encoding = "UTF-8")[-c(1, 2)]
  
  writeLines(V, con)
  
  close(con)
  file.rename("altair.2.md", "altair.md")
  
  V
}

# A réévaluer en fonction de l'évolution de la bibliothèque rmarkdown et de pandoc

## fin de hack ##

## Les deux fonctions sont appelées successivement ce qui permet d'identifier V
## Le hack est encore pire que le rpécédent puisqu'il n'est justifié que pour Office Writer et en raison d'un bug 
## de celui ci relatif à la résolution des chemins ! Autant dire qu'il faudra surveiller l'évolution du problème

ajuster_chemins_odt <- function(V) {

  # garde en principe inutile
  
  if (! is.null(V))  {
    con <- file("altair.2.md", open = "a", encoding = "UTF-8")
  
    writeLines(text = "\n---\ntitle: false\nauthor: false\ndate: false\n---\n", con)     
    writeLines(gsub("]\\((Docs|Bases)", "]\\(../\\1", V, perl = TRUE), con)
  
    close(con)
    file.rename("altair.2.md", "altair.md")
  }
}


