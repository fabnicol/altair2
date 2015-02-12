# encoder ce script en windows-1252
setwd(file.path(getwd(),"../../../Tests/Exemple"))
source("prologue.R", encoding="UTF-8")
writeLines(iconv(readLines("altaïr.R"), from = "UTF-8", to = "WINDOWS-1252"), "altaïr.ansi.R")
library(knitr)
spin("altaïr.ansi.R")
writeLines(iconv(readLines("altaïr.ansi.md"), from = "WINDOWS-1252", to = "UTF-8"), "altaïr.utf8.md", useBytes=TRUE)

system("C:/Users/Public/Altair/RStudio/bin/pandoc/pandoc.exe altair.utf8.md --to docx --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output altaïr.docx --highlight-style tango") 
shell("start winword altaïr.docx")
