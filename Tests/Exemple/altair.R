
source("prologue_source.R", encoding = "UTF-8")

for (rep in reps) {
  
  env <<- new.env()
  assign("chemin.cle", rep, envir = env)
  message("************************")
  message("*")
  message(paste("* Analyse du dossier", rep))
  message("*")
  message("************************")
  
  setwd(chemin.dossier)
  
  res <- try({
      source(file.path("syspaths.R"), encoding = "UTF-8")
    
      source(file.path("altair_start.R"), encoding = "UTF-8", echo = TRUE)
    
  })
  
  if (inherits(res, "try-error")) {
      message("************************")
      message("*")
      message(paste("*", rep, "N'a pas pu être traité"))
      message("*")
      message("************************")
  
  }
  
  setwd(currentDir)
  
  message("Dossier courant : ", getwd())
  
  if (! debug.code)  rm(list=setdiff(ls(), script_env))
}

if (! debug.code) rm(list = ls())


