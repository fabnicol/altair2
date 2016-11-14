source("prologue_rapport.R", encoding = "ISO-8859-1")

source("corps_rapport_pdf.R", encoding = encodage.code.source)

if (! keep_md) {
  unlink("altair.ansi_pdf", recursive=TRUE)
  unlink("altair.md")
  unlink("altair_files", recursive = TRUE)  
}

setwd(initwd)
