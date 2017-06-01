

setOSWindows                <- Sys.info()["sysname"] != "Linux"
racine                      <- ifelse(setOSWindows, "R-Altaïr/", "R-Altair")
currentDir                  <- getwd()
chemin.dossier              <- file.path(currentDir, "Tests/Exemple")
chemin.clé                  <- file.path(chemin.dossier, "Donnees", racine)

reps <- list.dirs(chemin.clé, recursive = FALSE)
reps <- reps[basename(reps) != "Bases" & basename(reps) != "Docs"]
if (length(reps) == 0) reps <- chemin.clé
start_env <- ls()

process_reps <- function(rep) {

  chemin.clé <<- rep
  
  message("************************")
  message("*")
  message(paste("* Analyse du dossier", rep))
  message("*")
  message("************************")
  
  res <- try({
  source(file.path(chemin.dossier, "altair.R"), encoding = "UTF-8")
  })
  
  if (inherits(res, "try-error")) {
      message("************************")
      message("*")
      message(paste("*", rep, "N'a pas pu être traité"))
      message("*")
      message("************************")
      setwd(currentDir)
  }
  
  rm(list=setdiff(ls(), start_env))
}

invisible(lapply(reps, process_reps))
