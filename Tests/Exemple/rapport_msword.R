source("prologue_rapport.R", encoding = "ISO-8859-1")
source ("rendre.R", encoding = encodage.code.source)

envir <- rendre()

file.rename("altair.docx", "altaïr.docx")

ajuster_chemins_odt(hack_md())

system(
 paste(
   ifelse(setOSWindows, file.path(Sys.getenv("R_HOME"), "../RStudio/bin/pandoc/pandoc.exe"), "/usr/bin/pandoc"),
   "altair.md +RTS -K512m -RTS --to",
   "odt",
   "--from markdown+autolink_bare_uris+ascii_identifiers+tex_math_single_backslash-implicit_figures --highlight-style tango --output",
   "altaïr.odt")
)

if (! keep_md) {
    #unlink("altair.ansi_pdf", recursive=TRUE)
    unlink("altair.md")
    unlink("altair_files", recursive = TRUE)  
}

if (setOSWindows) {
  
  system("copy /Y altaïr.odt altaïr.docx Donnees/R-Altaïr")
  
} else {
  
  system("cp -rf Docs Donnees/R-Altair")
  system("cp -f altaïr.odt altaïr.docx Donnees/R-Altair")
}


if (setOSWindows) {
  
  shell("start winword Donnees/R-Altaïr/altaïr.docx")
  
} else {
  
  system("lowriter Donnees/R-Altair/altaïr.odt")
  
}

setwd(initwd)
