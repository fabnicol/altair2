setOSWindows            <- Sys.info()["sysname"] != "Linux"

if (setOSWindows) {

  source("Tests/Exemple/rtoolspath.R")
  
  call_rtools()

  install_github("Rdatatable/data.table", build_vignettes = FALSE, configure.args = c("CFLAGS=-O3", "-march=native"))

  restore_path()


} else {

.libPaths("lib_linux")
library(devtools)
install_github("Rdatatable/data.table", configure.args = c("CFLAGS=-O3", "-march=native"))
}
