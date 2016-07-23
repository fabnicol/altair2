setOSWindows            <- Sys.info()["sysname"] != "Linux"
if (setOSWindows) {
   if (basename(current) == "Exemple") 
    .libPaths(file.path(current, "..", "..", library_path))
}


