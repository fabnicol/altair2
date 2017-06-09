

setOSWindows                <- Sys.info()["sysname"] != "Linux"
racine                      <- ifelse(setOSWindows, "R-Altaïr/", "R-Altair")

#if (! générer.rapport) currentDir <- getwd() else currentDir <- initwd
currentDir <- getwd()
chemin.dossier              <- file.path(currentDir, "Tests/Exemple")
chemin.clé                  <- file.path(chemin.dossier, "Donnees", racine)

reps <- list.dirs(chemin.clé, recursive = FALSE)
reps <- reps[basename(reps) != "Bases" & basename(reps) != "Docs"]
if (length(reps) == 0) reps <- chemin.clé

start_env <- ls()

for (rep in reps) {
  e <<- new.env()
  assign("chemin.cle", rep, envir = e)
  message("************************")
  message("*")
  message(paste("* Analyse du dossier", rep))
  message("*")
  message("************************")
  message(file.path(chemin.dossier, "altair.R"))
  res <- try({
      source(file.path(chemin.dossier, "altair.R"), encoding = "UTF-8", echo = TRUE)
  })
  
  if (inherits(res, "try-error")) {
      message("************************")
      message("*")
      message(paste("*", rep, "N'a pas pu être traité"))
      message("*")
      message("************************")
      setwd(currentDir)
  }
  
  #rm(list=setdiff(ls(), start_env))
}


