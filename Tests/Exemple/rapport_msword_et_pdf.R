# encoder ce script en windows-1252
encodage.code.source <- "ISO-8859-1"
initwd <- getwd()

setwd(file.path(Sys.getenv("R_HOME"), "../Tests/Exemple"))
source("C:/Users/Public/Dev/altair/Tests/Exemple/syspaths.R", encoding=encodage.code.source)
source("C:/Users/Public/Dev/altair/Tests/Exemple/prologue.R", encoding=encodage.code.source)

library_path <- if (setOSWindows) "lib" else "lib_linux"

.libPaths(file.path("C:/Users/Public/Dev/altair", library_path))

writeLines(iconv(readLines("C:/Users/Public/Dev/altair/Tests/Exemple/altair.R"), from = encodage.code.source, to = "WINDOWS-1252"), "C:/Users/Public/Dev/altair/Tests/Exemple/altair.ansi.R")
library(knitr)
spin("C:/Users/Public/Dev/altair/Tests/Exemple/altair.ansi.R")
writeLines(iconv(readLines("altair.ansi.md"), from = "WINDOWS-1252", to = "UTF-8"), "C:/Users/Public/Dev/altair/Tests/Exemple/altair.utf8.md", useBytes=TRUE)

system("C:/Users/Public/Dev/altair/RStudio/bin/pandoc/pandoc.exe C:/Users/Public/Dev/altair/Tests/Exemple/altair.utf8.md --to docx --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output C:/Users/Public/Dev/altair/Tests/Exemple/altaïr.docx --highlight-style tango") 
shell("start winword C:/Users/Public/Dev/altair/Tests/Exemple/altaïr.docx")

system("C:/Users/Public/Dev/altair/RStudio/bin/pandoc/pandoc.exe -V papersize=A4 +RTS -K512m -RTS c:/Users/Public/Dev/altair/Tests/Exemple/altair.utf8.md --to latex --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output altair.pdf --template C:/Users/Public/Dev/altair/lib/rmarkdown/rmd/latex/default.tex --highlight-style tango --latex-engine C:/Users/Public/Dev/altair/texlive/miktex/bin/x64/pdflatex.exe --variable geometry:margin=1in" )
shell("start acrord32.exe C:/Users/Public/Dev/altair/Tests/Exemple/altair.pdf")

setwd(initwd)

