# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, annees 2012 à 2017
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
# prologue 
# doit être encodé en UTF-8 au moins sous Linux. 
# doit être dans le même répertoire que le programme principal et sa bibliothèque

# dans le cas où l'on ne lance le programme que pour certaines annees, il préciser debut.periode.sous.revue et fin.periode .sous.revue
# Dans ce cas fixer extraire.annees en valeur TRUE.
# Sinon le programme travaille sur l'ensemble des annees disponibles dans la base : elles sont détectées automatiquement.

# OPTIONS GLOBALES ET DUBUGGAGE

debug.code                     <- FALSE

options(warn = ifelse(debug.code, 1, -1), verbose = debug.code, OutDec = ",", datatable.verbose = debug.code, datatable.integer64 = "numeric")
options(knitr.duplicate.label = "allow", encoding="UTF-8")
knitr::opts_chunk$set(fig.width = 7.5,  echo = debug.code, warning = debug.code, message = debug.code, results = 'asis', encoding = "UTF-8")

library(compiler, warn.conflicts = FALSE)

invisible(setCompilerOptions(suppressAll = TRUE, optimize = 3))
invisible(enableJIT(3))

library(knitr, warn.conflicts = FALSE)

# CHEMINS ET ENVIRONNEMENT

setOSWindows                <- Sys.info()["sysname"] != "Linux"
racine                      <- ifelse(setOSWindows, "R-Altaïr/", "R-Altair")

chemin.modules              <- file.path(chemin.dossier, "modules")
chemin.cle.racine           <- file.path(chemin.dossier, "Donnees", racine)

reps <- list.dirs(chemin.cle.racine, recursive = FALSE)
reps <- reps[basename(reps) != "Bases" & basename(reps) != "Docs"]
if (length(reps) == 0) reps <- chemin.cle.racine

if (setOSWindows) {

  invisible(Sys.setenv(PATH = paste(Sys.getenv("PATH"), 
                                    file.path(Sys.getenv("R_HOME"),
                                              "../texlive2/texmfs/install/miktex/bin/x64"),
                                    file.path(Sys.getenv("R_HOME"), "../RStudio/bin/pandoc"),
                                    sep=";")))
  
  if (basename(currentDir) == "R") 
    .libPaths(file.path(currentDir, "..", "..", "R-devel/library"))
  
  user <- Sys.getenv("USERNAME")
  sep_syspaths <- ";"
  
} else {
  path.libreoffice <- "/usr/lib/libreoffice/program" 
  user <-  Sys.getenv("USER")
  sep_syspaths <- ":"
  Sys.setenv(LD_LIBRARY_PATH = path.libreoffice %+% sep_syspaths %+% Sys.getenv("LD_LIBRARY_PATH"))
}

# PARAMETRES GLOBAUX NUMERIQUES OU BOOLEENS
# Toujours utiliser FALSE ou TRUE en plein lettres en raison de l'éloignement avec <- (bug de l'interpréteur pour T et F)

extraire.annees                <- FALSE
  debut.periode.sous.revue     <- 2015
  fin.periode.sous.revue       <- 2021

exec.root                      <- if (setOSWindows) ".exe" else ""

# Affichage dans le rapport
afficher.cumuls.détaillés.lignes.paie <- FALSE
afficher.table.écarts.sft      <- FALSE
afficher.table.codes           <- FALSE
afficher.table.événements      <- FALSE
afficher.table.effectifs       <- FALSE
generer.table.élus             <- TRUE
numero.tableau                 <- 0        # Numérotation de départ des tableaux
chapitre                       <- 1        # Numérotation de départ des chapitres
dpi                            <- 300 # points par pouce, précision des graphiques de rapport
fig.width                      <- 7.5
fig.height                     <- 5

# Paramétrage des algorithmes sur les bases
éliminer.duplications <- FALSE
utiliser.variable.Heures.Sup.  <- FALSE    # faire les tests de seuil IHTS sur la variable Heures.Sup. plutôt que sur la Base ou le  Nb.Unite
noria.sur.base.de.paie         <- FALSE    # calculer effet de noria sur base de paie uniquement (pas de fichier E/S)

# Paramétrage des algorithmes sur les heures et rémunérations
redresser.heures               <- TRUE
enlever.quotites.nulles        <- FALSE
enlever.quotites.na            <- FALSE
    # Si TRUE, des quotités > 1 seront ramenées à 1.
écreter.quotites               <- TRUE
analyse.statique.totale        <- FALSE
seuil.troncature               <- 2        # jours
taux.tolerance.homonymie       <- 2        # en %
quantile.cut                   <- 1        # en %
minimum.positif                <- 0.5
minimum.quotite                <- 0.05
tolerance.variation.quotite    <- 0.05
minimum.actif                  <- 100
minimum.delta                  <- 5
    # définitions INSEE pour le poste non annexe
    # poste non annexe = minimum de 30 jours et de 120h à raison d'au moins 1,5 h/jour.
reference.nb.heures.par.mois    <<- 151.61 # = 1820 /12
minimum.Nheures.jour.non.annexe <- 1.5
minimum.Nheures.non.annexe      <- 120
minimum.Njours.non.annexe        <- 30
population_minimale_calcul_médiane  <- 3
tolerance.sft                       <- 1
tolerance.nbi                       <- 2

# Paramétrage de l'input
charger.bases                  <- TRUE
charger.categories.personnel   <- TRUE
logements.existe               <- TRUE
plafonds.ifse.existe           <- TRUE
tester.matricules              <- TRUE
fichier.personnels.existe      <- TRUE
grades.categories.existe       <- TRUE
apparier.sur.trois.cles        <- FALSE
encodage.code.source           <- "UTF-8"
separateur.liste.entree     <- ";"
separateur.decimal.entree   <- ","

# Paramétrage de l'output
extraire.population            <- FALSE
keep_md                        <- TRUE
intégrer.localisation          <- FALSE   # Veut on gardet Budget Employeur Siret Etablissement ?
test.delta                     <- FALSE
analyse.par.categorie          <- TRUE
generer.codes                  <- FALSE
sauvegarder.bases.analyse      <- TRUE
sauvegarder.bases.origine      <- FALSE
fichiers.temp                  <- FALSE
if (! exists("PDF"))       PDF <<- FALSE
sauter.tests.statutaires       <- FALSE
separateur.liste.sortie     <- ";"
separateur.decimal.sortie   <- ","

# Paramétrage d'exécution des scripts
profiler                       <- FALSE
    # Trace de profilage.
if (profiler) {
    library(data.table, warn.conflicts = FALSE)
    PROF <<- data.table()
}
ratio.memoire.ligne.parallele  <- 2.3

# FICHIERS EN INPUT
# conventions de nommage : les noms et chemins sont en minuscules ;
# les bases commencent par une majuscule. Un nom de fichier est souvent associé
# à une variable commençant par une majuscule et représentant la base (data.frame ou matrice)

liste.exclusions <- NULL

try ({
  chemin.liste.exclusions <- chemin("liste.exclusions.txt")
  if (file.exists(chemin.liste.exclusions))
    liste.exclusions <- read.delim(chemin.liste.exclusions, encoding="UTF-8", stringsAsFactors = FALSE)[[1]]
}, silent=TRUE)


nom.fichier.codes.paiement  <- paste0(racine, "codes.csv")
nom.fichier.paie            <- paste0(racine, "Table")
nom.bulletin.paie           <- paste0(racine, "Bulletins")
nom.table                   <- "Table"
nom.bulletins               <- "Bulletins"

# FORMATS

colonnes.requises           <- c("Matricule",
                                 "Annee",
                                 "Mois",
                                 "Statut",
                                 "Brut",
                                 "Net.a.Payer",
                                 "Heures.Sup.",
                                 "Heures",
                                 "Emploi",
                                 "Nir",
                                 "Temps.de.travail")

date.format                    <- "%d/%m/%Y"

# Expressions régulières

source("expressions.regulieres.R")

# Point d'indice majoré

point_inm()
