
try(setwd("Tests/Exemple"), silent = TRUE)
encodage.code.source <- "UTF-8" #"ISO-8859-15"
source("prologue.R", encoding = encodage.code.source)

source(file.path(chemin.dossier, "bibliotheque.fonctions.paie.R"), encoding = encodage.code.source)

.libPaths(union(chemin.dossier.lib, .libPaths()))

installer.paquets(knitr, digest, colorspace, ggplot2, assertthat, yaml, gtools, utils, data.table, rmarkdown)
if (paralléliser) installer.paquets(parallel)
