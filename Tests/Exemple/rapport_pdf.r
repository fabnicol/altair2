# encoder ce script en windows-1252
encodage.code.source <- "ISO-8859-1"
initwd <- getwd()
if (corriger.environnement.système) {
  
  invisible(Sys.setenv(PATH = paste0(Sys.getenv("PATH"), Sys.getenv("R_HOME"), "../texlive/miktex/bin/x64;")))
  
}
setwd(file.path(Sys.getenv("R_HOME"), "../Tests/Exemple"))
source("syspaths.R", encoding=encodage.code.source)
source("prologue.R", encoding=encodage.code.source)

library_path <- if (setOSWindows) "lib" else "lib_linux"

.libPaths(file.path(Sys.getenv("R_HOME"), "..", library_path))

writeLines(iconv(readLines("altair.R"), from = encodage.code.source, to = "WINDOWS-1252"), "altair.ansi.R")
library(knitr)
spin("altair.ansi.R")
writeLines(iconv(readLines("altair.ansi.md"), from = "WINDOWS-1252", to = "UTF-8"), "altair.utf8.md", useBytes=TRUE)

system(paste(file.path(Sys.getenv("R_HOME"), "../RStudio/bin/pandoc/pandoc.exe"), "-V papersize=A4 +RTS -K512m -RTS altair.utf8.md --to latex --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output altair.pdf --template ", file.path(Sys.getenv("R_HOME"), "..", "lib/rmarkdown/rmd/latex/default.tex"), " --highlight-style tango --latex-engine ", file.path(Sys.getenv("R_HOME"), "..", "texlive/miktex/bin/x64/pdflatex.exe"), " --variable geometry:margin=1in" ))
shell("start acrord32.exe altair.pdf")

setwd(initwd)

