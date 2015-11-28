
if (setOSWindows) {
rtoolspath <- "RBuildTools/3.3/bin"

wd <- getwd()
if (grepl("altair$", wd, ignore.case=TRUE) == FALSE)
{
  stop("le répertoire de travail doit être .../altair")  
}

.libPaths("lib")

localPath <- Sys.getenv("PATH")

library(devtools)

# No need to use find_rtools() (broken or just useless here)

if (grepl(rtoolspath, localPath, ignore.case=TRUE) == FALSE) {

  Sys.setenv(PATH = paste(localPath, file.path(wd, "..", rtoolspath), sep=";"))
}  

install_github("Rdatatable/data.table", build_vignettes = FALSE, configure.args = c("CFLAGS=-O3", "-march=native"))


# Restoring previous system path to avoid cluttering

Sys.setenv(PATH = localPath)

} else {

.libPaths("lib_linux")
library(devtools)
install_github("Rdatatable/data.table", configure.args = c("CFLAGS=-O3", "-march=native"))
}
