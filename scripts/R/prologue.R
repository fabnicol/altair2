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
  Sys.setenv(LD_LIBRARY_PATH=path.libreoffice %+% sep_syspaths %+% Sys.getenv("LD_LIBRARY_PATH"))
}



# PARAMETRES GLOBAUX BOOLEENS ET ENTIERS

# Toujours utiliser FALSE ou TRUE en plein lettres en raison de l'éloignement avec <- (bug de l'interpréteur pour T et F)


extraire.annees                <- FALSE
  debut.periode.sous.revue     <- 2013
  fin.periode.sous.revue       <- 2018

exec.root                      <- if (setOSWindows) ".exe" else ""

éliminer.duplications          <- FALSE
redresser.heures               <- TRUE
enlever.quotites.nulles        <- FALSE
enlever.quotites.na            <- FALSE
écreter.quotites               <- TRUE
generer.codes                  <- FALSE
charger.categories.personnel   <- TRUE
extraire.population            <- FALSE
charger.bases                  <- TRUE
sauvegarder.bases.analyse      <- TRUE
sauvegarder.bases.origine      <- FALSE
afficher.table.effectifs       <- FALSE
generer.table.élus             <- TRUE
tester.matricules              <- TRUE
analyse.statique.totale        <- FALSE
intégrer.localisation          <- FALSE   # Veut on gardet Budget Employeur Siret Etablissement ?
afficher.cumuls.détaillés.lignes.paie <- FALSE
afficher.table.écarts.sft      <- FALSE
afficher.table.codes           <- FALSE
afficher.table.événements      <- FALSE
analyse.par.categorie          <- TRUE
test.delta                     <- FALSE
if (! exists("PDF"))       PDF <<- FALSE
keep_md                        <- TRUE
utiliser.cplusplus.sft         <- FALSE
utiliser.variable.Heures.Sup.  <- FALSE    # faire les tests de seuil IHTS sur la variable Heures.Sup. plutôt que sur la Base ou le  Nb.Unite
noria.sur.base.de.paie         <- FALSE    # calculer effet de noria sur base de paie uniquement (pas de fichier E/S)
sauter.tests.statutaires       <- FALSE
profiler                       <- FALSE
paye.budget.existe             <- FALSE
fichier.personnels.existe      <- TRUE
grades.categories.existe       <- TRUE
logements.existe               <- TRUE
plafonds.ifse.existe           <- TRUE
apparier.sur.trois.cles        <- FALSE
fichiers.temp                  <- FALSE

encodage.code.source           <- "UTF-8"

numero.tableau                 <- 0        # Numérotation de départ des tableaux
chapitre                       <- 1        # Numérotation de départ des chapitres
seuil.troncature               <- 2        # jours
taux.tolerance.homonymie       <- 2        # en %
quantile.cut                   <- 1        # en %
minimum.positif                <- 0.5
minimum.quotite                <- 0.05
tolerance.variation.quotite    <- 0.05
minimum.actif                  <- 100
minimum.delta                  <- 5
dpi                            <- 300 # points par pouce, précision des graphiques de rapport
fig.width                      <- 7.5
fig.height                     <- 5


# définitions INSEE pour le poste non annexe
# poste non annexe = minimum de 30 jours et de 120h à raison d'au moins 1,5 h/jour.

reference.nb.heures.par.mois    <<- 151.61 # = 1820 /12
minimum.Nheures.jour.non.annexe <- 1.5
minimum.Nheures.non.annexe      <- 120
minimum.Njours.non.annexe        <- 30

population_minimale_calcul_médiane  <- 3
tolerance.sft                       <- 1
tolerance.nbi                       <- 2
ratio.memoire.ligne.parallele       <- 2.3

separateur.liste.entree     <- ";"
separateur.decimal.entree   <- ","
separateur.liste.sortie     <- ";"
separateur.decimal.sortie   <- ","

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

# ESPACES DE VALEURS LICITES POUR CERTAINS CHAMPS (modalités)


########## Problématique ##############

# expressions régulières

#\\bxyz veut aussi dire cde.xyz(...), à éviter

# --- NBI
# Schémas recherchés :
#    nbi, n.b.., n(o*).? *b(o*).? *i(nd*).?
# ex: NBI, N.B.I, NOUV. BON. IND., N BON IND
# ex: PFI, P.F.I, PR. FONCT. INFO., IND F INF

expression.rég.traitement     <- "(?:\\s|^)trait.*\\b"
expression.rég.nbi        <- "(?:\\s|^)nbi|(?:n(?:(?=o)o\\w*|)\\.?\\s*\\b)?b(?:(?=o)o\\w*|)\\.?\\s*\\bi(?:(?=n)nd\\w*|)\\.?\\b"
expression.rég.astreintes <- "astr.*"
expression.rég.nas        <- "log.*(:?n\\.?\\s*\\ba\\.?\\s*\\bs|n..?c.*ser)"

# --- FPH

expression.rég.primespecifique <- "pr\\.?\\w*\\b\\s*\\bsp..?ci.*"
expression.rég.ift <- "(?:\\bind\\.?\\w*\\s*forf\\.?\\w*\\s*(?:et\\s*|)te.*)"
expression.rég.primedeservice <- "\\bpr\\.?\\w*\\b\\s*(?:de\\s*)\\bs\\w+v"
expression.rég.primedetechnicite <- "\\bpr\\.?\\w*\\b\\s*\\b(?:de\\s*|)tech.*"
expression.rég.médecin <- "(?:\\bm..?d(?:\\.|ecin)\\b\\s+|prat\\.?\\w*\\b\\s*\\bhos).*"

# --- Prime de fonctions informatiques
# Schémas recherchés :
#    pfi, p.f.i., p(r*).? *f(o*).? *inf(o*).?, i(nd*).? *f(o*).? *inf(o*).?
# ex: PFI, P.F.I, PR. FONCT. INFO., IND F INF

expression.rég.pfi        <- "(?:^|\\s)pfi|(?:p(?:(?=r)r\\w*|)|i(?:(?=n)nd\\w*|))\\.?\\s*(?:\\bf(?:(?=o)o\\w*|)\\.?\\s*)?\\binf(?:(?=o)o\\w*|)\\.?\\b"
expression.rég.vacataires <- "HOR"

expression.rég.heures.sup <- "(?:^|\\s)iht?[s]|i(?:(?=n)n\\w*|)\\.?\\s*\\bh(?:(?=o)o\\w*|)\\.?.*\\bs(?:(?=u)u\\w*|)\\.?\\b|h(?:(?=e)e\\w*|)\\.?\\s*\\b(?:(?=s)s\\w*)\\.?\\b"
expression.rég.iat        <- "(?:^|\\s)i\\.?a\\.?t|i(?:(?=n)n\\w*|)\\.?\\s*\\ba(?:(?=d)d\\w*|)\\.?\\s*\\b(?:et\\s*\\b)?t(?:(?=e)e\\w*|)\\.?\\b"
expression.rég.ifts       <- "(?:^|\\s)i\\.?f\\.?t\\.?s|i(?:(?=n)n\\w*|)\\.?\\s*\\bf\\w*\\.?\\s*\\bt(?:(?=r)r\\w*|)\\.?\\s*\\bs(?:(?=u)u\\w*|)\\.?\\b"
expression.rég.pfr        <- "(?:^|\\s)p\\.?f\\.?r|(?:p(?:(?=r)r\\w*|))\\.?\\s*\\bfo?\\w*\\.?.*\\s+.*\\br\\w*s\\w*\\.?\\s*\\b"
expression.rég.iss        <- "(?:^|\\s)i\\.?s\\.?s|(?:i(?:(?=n)n\\w*|))\\.?\\s*\\bsu?\\w*\\.?.*\\s+.*\\bsp?\\w*\\.?\\s*\\b"
expression.rég.ifse       <- "(?:^|\\s)i\\.?f\\.?s\\.?e|(?:i(?:(?=n)n\\w*|))\\.?\\s*\\bfo?\\w*\\.?.*\\s+.*\\bs\\w*\\.?\\s*\\be.*"
expression.rég.iemp       <- "(?:^|\\s)i\\.?e\\.?m\\.?p|(?:i(?:(?=n)n\\w*|))\\.?\\s*\\bex?\\w*\\.?.*\\s+.*\\bm\\w*\\.?\\s*\\bpr.*"
expression.rég.psr        <- "(?:^|\\s)p\\.?s\\.?r|(?:p(?:(?=r)r\\w*|))\\.?\\s*\\bse?\\w*\\.?.*\\s+.*\\br\\w*n\\w*\\.?\\s*\\b"
expression.rég.ipf        <- "(?:^|\\s)i\\.?p\\.?f|(?:i(?:(?=n)n\\w*|))\\.?\\s*\\bpe?\\w*\\.?.*\\s+.*\\bf\\w*n\\w*\\.?\\s*\\b"

expression.rég.population <- "\\bASS(\\b|A).*"
expression.rég.élus       <- "ind.*[é,e]lu"

expression.rég.attaché    <- "(?:^|\\s)att\\w*\\.?\\b|se\\w*\\.?\\bma\\w*\\.?\\b"
expression.rég.attaché.p  <- "(?:^|\\s)att\\w*\\.?\\bpr\\w*\\.?\\b|dir\\w*\\.?\\b"
expression.rég.admin      <- "(?:^|\\s)adm\\w*\\.?\\b"
expression.rég.admin.hc   <- "(?:^|\\s)adm\\w*\\.?\\bh\\w*\\.?\\s?c\\w*\\.?\\b"
expression.rég.admin.g    <- "(?:^|\\s)adm\\w*\\.?\\b\\s?g\\w*\\.?\\b"

# Trace de profilage.
library(data.table, warn.conflicts = FALSE)
PROF <<- data.table()

# point d'indice majoré

point_inm()

