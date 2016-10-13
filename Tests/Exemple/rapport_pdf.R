source("prologue_rapport.R", encoding = "ISO-8859-1")

source("corps_rapport_pdf.R", encoding = encodage.code.source)

if (! keep_md) {
  unlink("altair.ansi_pdf", recursive=TRUE)
  unlink("altair.md")
  unlink("altair_files", recursive = TRUE)  
}


if (! setOSWindows) {
    system("cp -rf Docs Donnees/R-Altair")
    system("cp -f  altaïr.pdf Donnees/R-Altair")
    system("cp -rf Bases Donnees/R-Altair")
}


setwd(initwd)
