# encoder ce script en windows-1252
encodage.code.source <- "ISO-8859-1"
initwd <- getwd()
setOSWindows  <- Sys.info()["sysname"] != "Linux"
PDF <<- TRUE

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
                                                                       fig.height = 5,
                                                                       echo = FALSE,
                                                                       warning = FALSE,
                                                                       message = FALSE,
                                                                       results = 'asis')),
                                       keep_md = keep_md, clean_supporting = !keep_md,
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

  
} else {
  
  setwd("Tests/Exemple")
  source("syspaths.R", encoding=encodage.code.source)
  knitr::opts_chunk$set(echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')
  source("prologue.R", encoding=encodage.code.source)
  writeLines(iconv(readLines("altair.R"), from = encodage.code.source, to = "UTF-8"), "altair.utf8.R")  
  
  rmarkdown::render("altair.utf8.R", output_format = "pdf_document", output_file = "altair.pdf")
  
# fallback:
#  system(paste("/usr/bin/pandoc", " -V papersize=A4 +RTS -K512m -RTS altair.utf8.md --to latex --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output altaïr.pdf --template ../../lib_linux/rmarkdown/rmd/latex/default.tex --highlight-style tango --latex-engine pdflatex --variable geometry:margin=1in"))
# 
# des particularités étranges d'encodage font que l'output altaïr.pdf de render n'est pas possible (2016/01)
  
  system("mv altair.pdf altaïr.pdf && /usr/bin/okular altaïr.pdf")
  unlink("altair.utf8_files", recursive = TRUE)
}



setwd(initwd)

