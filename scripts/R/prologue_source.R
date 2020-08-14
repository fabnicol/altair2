# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser
# les fichiers de paye produits au format spécifié par l'annexe de la 
# convention-cadre de dématérialisation en vigueur à partir de 2008.
# 
# Ce logiciel est régi par la licence CeCILL soumise au droit français et
# respectant les principes de diffusion des logiciels libres. Vous pouvez
# utiliser, modifier et/ou redistribuer ce programme sous les conditions
# de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
# sur le site "http://www.cecill.info".
# 
# En contrepartie de l'accessibilité au code source et des droits de copie,
# de modification et de redistribution accordés par cette licence, il n'est
# offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
# seule une responsabilité restreinte pèse sur l'auteur du programme, le
# titulaire des droits patrimoniaux et les concédants successifs.
# 
# A cet égard l'attention de l'utilisateur est attirée sur les risques
# associés au chargement, à l'utilisation, à la modification et/ou au
# développement et à la reproduction du logiciel par l'utilisateur étant
# donné sa spécificité de logiciel libre, qui peut le rendre complexe à
# manipuler et qui le réserve donc à des développeurs et des professionnels
# avertis possédant des connaissances informatiques approfondies. Les
# utilisateurs sont donc invités à charger et tester l'adéquation du
# logiciel à leurs besoins dans des conditions permettant d'assurer la
# sécurité de leurs systèmes et ou de leurs données et, plus généralement,
# à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
# 
# Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
# pris connaissance de la licence CeCILL, et que vous en avez accepté les
# termes.
# 
# 

debug.code                  <- TRUE
setOSWindows                <- Sys.info()["sysname"] != "Linux"
racine                      <- ifelse(setOSWindows, "R-Altaïr/", "R-Altair")
currentDir                  <- getwd()
chemin.dossier              <- file.path(currentDir, "scripts/R")
chemin.modules              <- file.path(chemin.dossier, "modules")
chemin.cle.racine           <- file.path(chemin.dossier, "Donnees", racine)
generer.rapport             <- TRUE

reps <- list.dirs(chemin.cle.racine, recursive = FALSE)
reps <- reps[basename(reps) != "Bases" & basename(reps) != "Docs"]
if (length(reps) == 0) reps <- chemin.cle.racine


setwd(chemin.dossier)
library(data.table, warn.conflicts = FALSE)

if (setOSWindows) {
   if (basename(currentDir) == "Exemple") 
       .libPaths(file.path(currentDir, "..", "..", "R-devel/library"))
     

   library("altair")
} else {
  library("altair", lib.loc=c("/usr/local/lib64/R/library",
                              "/usr/local/lib/R/library",
                              "/usr/lib64/R/library",
                              "/usr/lib/R/library",
                              "/usr/local/lib64/R/site-library",
                              "/usr/local/lib/R/site-library",
                              "/usr/lib64/R/site-library",
                              "/usr/lib/R/site-library"))
}

options(warn = -1, verbose = FALSE, OutDec = ",", datatable.verbose = FALSE, datatable.integer64 = "numeric")

source("prologue.R", encoding = "UTF-8")

script_env <- ls()
script_env <- c(script_env, "script_env")
