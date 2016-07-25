encodage.code.source <- "ISO-8859-1"
initwd <- getwd()
setOSWindows  <- Sys.info()["sysname"] != "Linux"

# On doit désactiver les sauts de page PDF ou alors filtrer les \newpage par le hack pour produire les docx

PDF <<- FALSE


if (setOSWindows) {
  invisible(Sys.setenv(PATH = paste(Sys.getenv("PATH"), file.path(Sys.getenv("R_HOME"), "../texlive/miktex/bin"), file.path(Sys.getenv("R_HOME"), "../RStudio/bin/pandoc"), sep=";")))
  setwd(file.path(Sys.getenv("R_HOME"), "../Tests/Exemple"))
} else {
  
  setwd("Tests/Exemple")
}

source("syspaths.R", encoding = encodage.code.source)

source("prologue.R", encoding = encodage.code.source)

library(rmarkdown)
# ne pas écrire altaïr.pdf... 
