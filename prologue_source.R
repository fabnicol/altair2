debug.code                  <- TRUE
setOSWindows                <- Sys.info()["sysname"] != "Linux"
racine                      <- ifelse(setOSWindows, "R-Altaïr/", "R-Altair")
currentDir                  <- getwd()
chemin.dossier              <- file.path(currentDir, "Tests/Exemple")
chemin.clé.racine           <- file.path(chemin.dossier, "Donnees", racine)
générer.rapport             <- FALSE

reps <- list.dirs(chemin.clé.racine, recursive = FALSE)
reps <- reps[basename(reps) != "Bases" & basename(reps) != "Docs"]
if (length(reps) == 0) reps <- chemin.clé.racine

encodage.code.source <- "ISO-8859-1"

setwd(chemin.dossier)

if (setOSWindows) {
   if (basename(currentDir) == "Exemple") 
       .libPaths(file.path(currentDir, "..", "..", "R/library"))
  
   invisible(Sys.setenv(PATH = paste0(Sys.getenv("PATH"), "c:\\Users\\Public\\Dev\\altair\\texlive\\miktex\\bin;")))
}

library(data.table, warn.conflicts = FALSE)

source("prologue.R", encoding = "UTF-8")

script_env <- ls()
script_env <- c(script_env, "script_env")
