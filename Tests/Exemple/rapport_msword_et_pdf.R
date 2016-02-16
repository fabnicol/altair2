# encoder ce script en windows-1252
encodage.code.source <- "ISO-8859-1"
initwd <- getwd()
setOSWindows  <- Sys.info()["sysname"] != "Linux"

# On doit désactiver les sauts de page PDF ou alors filtrer les \newpage par le hack pour produire les docx

PDF <<- FALSE


if (setOSWindows) {
  invisible(Sys.setenv(PATH = paste(Sys.getenv("PATH"), file.path(Sys.getenv("R_HOME"), "../texlive/miktex/bin"), file.path(Sys.getenv("R_HOME"), "../RStudio/bin/pandoc"), sep=";")))
  setwd(file.path(Sys.getenv("R_HOME"), "../Tests/Exemple"))
  source("syspaths.R", encoding=encodage.code.source)
  
  source("prologue.R", encoding=encodage.code.source)
  
  library(rmarkdown)
  # ne pas écrire altaïr.pdf... 
  
  render("altair.R",
         output_format = output_format(knitr_options(opts_chunk = list(fig.width = 7.5, 
                                                                       fig.height = 5,
                                                                       echo = FALSE,
                                                                       warning = FALSE,
                                                                       message = FALSE,
                                                                       results = 'asis')),
                                       keep_md=TRUE, clean_supporting=FALSE,
                                       pandoc = pandoc_options(to = "latex",
                                                               from = "markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures",
                                                               args=c("-V", 
                                                                      "papersize=A4",
                                                                      "-V",
                                                                      "geometry:top=2cm,bottom=1.5cm,left=2cm,right=1.5cm",
                                                                      "--highlight-style",
                                                                      "tango",
                                                                      "--template",
                                                                      "../../R/library/rmarkdown/rmd/latex/default.tex") )),
         output_file = "altair.pdf")

  file.rename("altair.pdf", "altaïr.pdf")
  
  shell("start acrord32.exe altaïr.pdf")
  
  ## début de hack ## 
  
  # Hack relativement laid mais très efficace qui évite de refaire tourner render pour la fabrication d'un .md
  # en recalculant les mêmes choses afin de produire le nouveau format.
  # On utilise le .md produit par keep_md = TRUE plus haut et les images conservées grâce à clean_supporting = FALSE
  # Mais il faut réinsérer un nouveau YAML header pour effacer les noms, dates etc. standard
  # et aussi effacer un FALSE indésirable produit par render en tête de .md (qui correspond au traitement du premier YAML header du pdf)
  # Les autres alternatives ont été essayées et échouent. La production directe de .md par render échoue sur la question
  # des liens URL comportant un caractère non-ascii.
  
  # attention encodage UTF-8 obligatoire quelle que soit la plateforme
  
  con <- file("altair.2.md", open = "a", encoding = "UTF-8")
  
  writeLines(text = "\n---\ntitle: false\nauthor: false\ndate: false\n---\n", con)     
  
  writeLines(readLines("altair.md", encoding = "UTF-8")[-c(1, 2)], con)
  
  close(con)
  file.rename("altair.2.md", "altair.md")
  
  # A réévaluer en fonction de l'évolution de la bibliothèque rmarkdown et de pandoc
  
  ## fin de hack ##
  
  system(
    paste(
      file.path(Sys.getenv("R_HOME"), "../RStudio/bin/pandoc/pandoc.exe"),
      "altair.md +RTS -K512m -RTS --to docx --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output altaïr.docx --highlight-style tango"
    )
  )

  shell("start winword altaïr.docx")
  
  if (! keep_md) {
    unlink("altair.ansi_pdf", recursive=TRUE)
    unlink("altair.md")
  }

} else {
  setwd("Tests/Exemple")
  source("syspaths.R", encoding = encodage.code.source)
  library(knitr)
  opts_chunk$set(echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')
  source("prologue.R", encoding = encodage.code.source)
  writeLines(iconv(readLines("altair.R"), from = encodage.code.source, to = "UTF-8"),
             "altair.utf8.R")
  
  library(rmarkdown)
  #spin("altair.utf8.R", knit=FALSE)
  rmarkdown::knitr_options_html(fig_width=8, fig_height=4, fig_retina=6, keep_md=FALSE)
  rmarkdown::knitr_options_pdf(fig_width=8, fig_height=4, fig_crop=TRUE)
  
  rmarkdown::render("altair.utf8.R", clean = FALSE,  "html_document")
  
  system(paste("/usr/bin/pandoc",
                "altair.utf8.html  +RTS -K512m -RTS --to odt --from  html --output altaïr.odt --highlight-style tango"))
  
  system(paste("/usr/local/bin/wkhtmltopdf", 
               "--page-size A4 --quiet altair.utf8.html altaïr.pdf")) 
  
  system("/usr/bin/lowriter altaïr.odt")

  system("/usr/bin/okular altaïr.pdf")
  unlink("altair.utf8_files", recursive = TRUE)  
}


setwd(initwd)
