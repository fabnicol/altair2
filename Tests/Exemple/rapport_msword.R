# encoder ce script en windows-1252
encodage.code.source <- "ISO-8859-1"
initwd <- getwd()
setOSWindows  <- Sys.info()["sysname"] != "Linux"
PDF <<- FALSE

if (setOSWindows) {
  
  invisible(Sys.setenv(PATH = paste(Sys.getenv("PATH"), file.path(Sys.getenv("R_HOME"), "../texlive/miktex/bin"), file.path(Sys.getenv("R_HOME"), "../RStudio/bin/pandoc"), sep=";")))

  setwd(file.path(Sys.getenv("R_HOME"), "../Tests/Exemple"))
  source("syspaths.R", encoding=encodage.code.source)
  
  source("prologue.R", encoding=encodage.code.source)
  
  library(rmarkdown)
  
  render("altair.R",
         output_format = output_format(knitr_options(opts_chunk = list(fig.width = 7.5, 
                                                                       fig.height = 5,
                                                                       echo = FALSE,
                                                                       warning = FALSE,
                                                                       message = FALSE,
                                                                       results = 'asis')),
                                       keep_md = keep_md, clean_supporting = !keep_md,
                                       pandoc = pandoc_options(to = "docx",
                                                               from = "markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures",
                                                               args=c("-V", 
                                                                      "papersize=A4" ))),
         output_file = "altaïr.docx")
  
  #file.rename("altair.pdf", "altaïr.pdf")
  shell("start winword altaïr.docx")
  
} else {
  setwd("Tests/Exemple")
  source("syspaths.R", encoding = encodage.code.source)
  knitr::opts_chunk$set(echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')
  source("prologue.R", encoding = encodage.code.source)
  writeLines(iconv(readLines("altair.R"), from = encodage.code.source, to = "UTF-8"),
             "altair.utf8.R")
  library(rmarkdown)
  
  rmarkdown::render("altair.utf8.R", clean = FALSE,  html_document(fig_retina = 3))
  # produit altair.utf8.html
  
  system(
    paste(
      "/usr/bin/pandoc",
      "altair.utf8.html  +RTS -K512m -RTS --to odt --from  html --output altaïr.odt --highlight-style tango"
    )
  )
  
# fallback :  
#  system(
#    paste(~/R/x86_64-pc-linux-gnu-library/3.3/
#      "/usr/bin/pandoc",
#      "altair.utf8.knit.md --to odt --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output altaïr.odt --highlight-style tango"
#    )
 # )
  
  system("/usr/bin/lowriter altaïr.odt")
  unlink("altair.utf8_files", recursive = TRUE)  
}


setwd(initwd)
