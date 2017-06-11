
source("prologue_source.R")

for (rep in reps) {
  
  e <<- new.env()
  assign("chemin.cle", rep, envir = e)
  message("************************")
  message("*")
  message(paste("* Analyse du dossier", rep))
  message("*")
  message("************************")
  
  setwd(chemin.dossier)
  
  res <- try({
      source(file.path("syspaths.R"), encoding = encodage.code.source)
      source(file.path("altair.R"), encoding = "UTF-8", echo = TRUE)
  })
  
  if (inherits(res, "try-error")) {
      message("************************")
      message("*")
      message(paste("*", rep, "N'a pas pu être traité"))
      message("*")
      message("************************")
      setwd(currentDir)
  }
  
  rm(list=setdiff(ls(), script_env))
}

rm(list = ls())


