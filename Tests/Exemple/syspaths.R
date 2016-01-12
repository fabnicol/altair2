setOSWindows            <- Sys.info()["sysname"] != "Linux"
library_path <- if (setOSWindows) "lib" else "lib_linux"

.libPaths(file.path(getwd(), "..", "..", library_path))

