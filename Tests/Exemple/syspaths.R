setOSWindows            <- Sys.info()["sysname"] != "Linux"
if (! setOSWindows) {
  library_path <- "lib_linux"
  .libPaths(file.path(getwd(), "..", "..", library_path))
}

