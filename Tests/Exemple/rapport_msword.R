# encoder ce script en windows-1252

source("C:/Users/Public/Dev/altair/Tests/Exemple/prologue.R", encoding="UTF-8")

library_path <- if (setOSWindows) "lib" else "lib_linux"

.libPaths(file.path("C:/Users/Public/Dev/altair", library_path))

writeLines(iconv(readLines("C:/Users/Public/Dev/altair/Tests/Exemple/altair.R"), from = "UTF-8", to = "WINDOWS-1252"), "C:/Users/Public/Dev/altair/Tests/Exemple/altair.ansi.R")
library(knitr)
spin("altair.ansi.R")
writeLines(iconv(readLines("C:/Users/Public/Dev/altair/Tests/Exemple/altair.ansi.md"), from = "WINDOWS-1252", to = "UTF-8"), "C:/Users/Public/Dev/altair/Tests/Exemple/altair.utf8.md", useBytes=TRUE)

system("C:/Users/Public/Dev/altair/RStudio/bin/pandoc/pandoc.exe C:/Users/Public/Dev/altair/Tests/Exemple/altair.utf8.md --to docx --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output C:/Users/Public/Dev/altair/Tests/Exemple/altaïr.docx --highlight-style tango") 
shell("start winword C:/Users/Public/Dev/altair/Tests/Exemple/altaïr.docx")
