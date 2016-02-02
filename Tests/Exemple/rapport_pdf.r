# encoder ce script en windows-1252
encodage.code.source <- "ISO-8859-1"
initwd <- getwd()
setOSWindows  <- Sys.info()["sysname"] != "Linux"
PDF <<- TRUE

if (setOSWindows) {
  
  invisible(Sys.setenv(PATH = paste0(Sys.getenv("PATH"), ";", file.path(Sys.getenv("R_HOME"), "../texlive/miktex/bin;"))))
  setwd(file.path(Sys.getenv("R_HOME"), "../Tests/Exemple"))
  source("syspaths.R", encoding=encodage.code.source)
  knitr::opts_chunk$set(fig.width=8, fig.height=4, echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')
  
  source("prologue.R", encoding=encodage.code.source)
  writeLines(iconv(readLines("altair.R"), from = encodage.code.source, to = "WINDOWS-1252"), "altair.ansi.R")
  library(knitr)
  
  spin("altair.ansi.R")
  writeLines(iconv(readLines("altair.ansi.md"), from = "WINDOWS-1252", to = "UTF-8"), "altair.utf8.md", useBytes = TRUE)

  system(paste(file.path(Sys.getenv("R_HOME"), "../RStudio/bin/pandoc/pandoc.exe"), "-V papersize=A4 +RTS -K512m -RTS altair.utf8.md --to latex --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output altaïr.pdf --template ", file.path(Sys.getenv("R_HOME"), "library/rmarkdown/rmd/latex/default.tex"), " --highlight-style tango --latex-engine ", file.path(Sys.getenv("R_HOME"), "..", "texlive/miktex/bin/pdflatex.exe"), " --variable geometry:margin=1in" ))
  shell("start acrord32.exe altaïr.pdf")
  unlink("figure", recursive=TRUE)
  
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

