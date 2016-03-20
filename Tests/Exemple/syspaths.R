setOSWindows            <- Sys.info()["sysname"] != "Linux"
if (! setOSWindows) {
  library_path <- "lib_linux"
  current <- getwd()
  if (basename(current) == "altair") {
    .libPaths(file.path(current,  library_path))
  } else if (basename(current) == "Exemple") 
    .libPaths(file.path(current, "..", "..", library_path))
}


