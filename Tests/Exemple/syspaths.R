setOSWindows            <- Sys.info()["sysname"] != "Linux"
current <- getwd()
if (setOSWindows) {
   if (basename(current) == "Exemple") 
    .libPaths(file.path(current, "..", "..", "R/library"))
}


