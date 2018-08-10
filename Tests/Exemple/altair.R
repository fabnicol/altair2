
encodage.code.source <- "UTF-8" 

source("prologue_source.R", encoding = encodage.code.source)

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
      source(file.path("syspaths.R"), encoding = encodage.code.source)
    
      source(file.path("altair_start.R"), encoding = encodage.code.source, echo = TRUE)
    
  })
  
  if (inherits(res, "try-error")) {
      message("************************")
      message("*")
      message(paste("*", rep, "N'a pas pu être traité"))
      message("*")
      message("************************")
      setwd(currentDir)
  }
  
  if (! debug.code)  rm(list=setdiff(ls(), script_env))
}

if (! debug.code) rm(list = ls())


