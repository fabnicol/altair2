# encoder ce script en windows-1252
encodage.code.source <- "ISO-8859-1"
initwd <- getwd()

setwd(file.path(Sys.getenv("R_HOME"), "../Tests/Exemple"))
setwd("Tests/Exemple")
source("syspaths.R", encoding=encodage.code.source)
source("prologue.R", encoding=encodage.code.source)

library_path <- if (setOSWindows) "lib" else "lib_linux"

.libPaths(file.path(Sys.getenv("R_HOME"), "..", library_path))

writeLines(iconv(readLines("altair.R"), from = encodage.code.source, to = "WINDOWS-1252"), "altair.ansi.R")
library(knitr)
spin("altair.ansi.R")
writeLines(iconv(readLines("altair.ansi.md"), from = "WINDOWS-1252", to = "UTF-8"), "altair.utf8.md", useBytes=TRUE)
if (setOSWindows) {
  system(paste(file.path(Sys.getenv("R_HOME"), "../RStudio/bin/pandoc/pandoc.exe"), "altair.utf8.md --to docx --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output altaïr.docx --highlight-style tango")) 
  shell("start winword altaïr.docx")
} else {
  system(paste("/usr/bin/pandoc.exe", "altair.utf8.md --to odt --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output altaïr.odt --highlight-style tango")) 
  shell("/usr/bin/lowriter altaïr.odt")
}

setwd(initwd)

