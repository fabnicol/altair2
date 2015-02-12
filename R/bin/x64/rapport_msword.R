# encoder ce script en windows-1252
setwd(file.path(getwd(),"../../../Tests/Exemple"))
source("prologue.R", encoding="UTF-8")
writeLines(iconv(readLines("altair.R"), from = "UTF-8", to = "WINDOWS-1252"), "altair.ansi.R")
library(knitr)
spin("altair.ansi.R")
writeLines(iconv(readLines("altair.ansi.md"), from = "WINDOWS-1252", to = "UTF-8"), "altair.utf8.md", useBytes=TRUE)

system("C:/Users/Public/Altair/RStudio/bin/pandoc/pandoc.exe altair.utf8.md --to docx --from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --output altaïr.docx --highlight-style tango") 
shell("start winword altaïr.docx")
