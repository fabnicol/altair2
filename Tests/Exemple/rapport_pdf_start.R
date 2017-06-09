
setOSWindows                <- Sys.info()["sysname"] != "Linux"
racine                      <- ifelse(setOSWindows, "R-Altaïr/", "R-Altair")

chemin.clé.racine                  <- file.path(getwd(), "Tests", "Exemple", "Donnees", racine)

reps <- list.dirs(chemin.clé.racine, recursive = FALSE)
reps <- reps[basename(reps) != "Bases" & basename(reps) != "Docs"]
if (length(reps) == 0) reps <- chemin.clé.racine

start_env <- ls()
start_env <- c(start_env, "start_env")

for (rep in reps) {
  e <<- new.env()
  assign("chemin.cle", rep, envir = e)
  message("************************")
  message("*")
  message(paste("* Analyse du dossier", rep))
  message("*")
  message("************************")
  script_rapport <- file.path("Tests", "Exemple", "rapport_pdf.R")
  
  res <- try({
      source(script_rapport, encoding = "ISO-8859-1", echo = TRUE)
  })
  
  if (inherits(res, "try-error")) {
      message("************************")
      message("*")
      message(paste("*", rep, "N'a pas pu être traité"))
      message("*")
      message("************************")
      setwd(currentDir)
  }
  
  rm(list = setdiff(ls(), start_env))
}


