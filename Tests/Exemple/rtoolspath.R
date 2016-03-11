
call_rtools <- function() {

  rtoolspath <- "Rtools/bin"
  rtoolspath2 <- "Rtools/mingw_64/bin"
  
  wd <- getwd()
  if (grepl("altair$", wd, ignore.case=TRUE) == FALSE)
  {
    stop("le répertoire de travail doit être .../altair")  
  }
  
  #.libPaths("lib")
  
  localPath <<- Sys.getenv("PATH")
  
  library(devtools)
  
  # No need to use find_rtools() (broken or just useless here)
  
  if (grepl(rtoolspath, localPath, ignore.case=TRUE) == FALSE) {
    
    Sys.setenv(PATH = paste(localPath, file.path(wd, "..", rtoolspath), file.path(wd, "..", rtoolspath2), file.path(wd, "Outils"), sep=";"))
  }  
  
  
}


restore_path <- function() {
  
  # Restoring previous system path to avoid cluttering

  Sys.setenv(PATH = localPath)
  
}

