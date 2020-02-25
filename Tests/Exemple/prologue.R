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
# prologue 
# doit être encodé en UTF-8 au moins sous Linux. A vérifier sous Windows...pour l'interface graphique
# doit être dans le même répertoire que le programme principal et sa bibliothèque

# dans le cas où l'on ne lance le programme que pour certaines années, il préciser début.période sous revue et fin.période .sous.revue
# Dans ce cas fixer extraire.années en valeur TRUE.
# Sinon le programme travaille sur l'ensemble des années disponibles dans la base : elles sont détectées automatiquement.

# PARAMETRES GLOBAUX BOOLEENS ET ENTIERS



# Toujours utiliser FALSE ou TRUE en plein lettres en raison de l'éloignement avec <- (bug de l'interpréteur pour T et F)

extraire.années                <- FALSE
  début.période.sous.revue     <- 2013
  fin.période.sous.revue       <- 2018

exec.root                      <- if (setOSWindows) ".exe" else ""

éliminer.duplications          <- FALSE
redresser.heures               <- TRUE
enlever.quotites.nulles        <- FALSE
enlever.quotites.na            <- FALSE
écreter.quotites               <- TRUE
générer.codes                  <- FALSE
charger.catégories.personnel   <- TRUE
extraire.population            <- FALSE
charger.bases                  <- TRUE
sauvegarder.bases.analyse      <- TRUE
sauvegarder.bases.origine      <- FALSE
afficher.table.effectifs       <- FALSE
générer.table.élus             <- TRUE
tester.matricules              <- TRUE
analyse.statique.totale        <- FALSE
intégrer.localisation          <- FALSE   # Veut on gardet Budget Employeur Siret Etablissement ?
afficher.cumuls.détaillés.lignes.paie <- FALSE
afficher.table.écarts.sft      <- FALSE
afficher.table.codes           <- FALSE
afficher.table.événements      <- FALSE
analyse.par.catégorie          <- TRUE
test.delta                     <- FALSE
if (! exists("PDF"))       PDF <<- FALSE
keep_md                        <- FALSE
utiliser.cplusplus.sft         <- FALSE
sécuriser.types.sortie         <- TRUE
utiliser.variable.Heures.Sup.  <- FALSE    # faire les tests de seuil IHTS sur la variable Heures.Sup. plutôt que sur la Base ou le  Nb.Unite
noria.sur.base.de.paie         <- FALSE    # calculer effet de noria sur base de paie uniquement (pas de fichier E/S)
sauter.tests.statutaires       <- FALSE
profiler                       <- FALSE
paye.budget.existe             <- FALSE
fichier.personnels.existe      <- TRUE
grades.categories.existe       <- TRUE
logements.existe               <- TRUE
plafonds.ifse.existe           <- TRUE
apparier.sur.trois.clés        <- FALSE
fichiers.temp                  <- FALSE
convertir.latin1               <- FALSE

numéro.tableau                 <- 0        # Numérotation de départ des tableaux
chapitre                       <- 1        # Numérotation de départ des chapitres
seuil.troncature               <- 2        # jours
taux.tolérance.homonymie       <- 2        # en %
quantile.cut                   <- 1        # en %
minimum.positif                <- 0.5
minimum.quotite                <- 0.1
tolérance.variation.quotite    <- 0.05
minimum.actif                  <- 100
minimum.delta                  <- 5
dpi                            <- 300 # points par pouce, précision des graphiques de rapport
fig.width                      <- 7.5
fig.height                     <- 5


# définitions INSEE pour le poste non annexe
# poste non annexe = minimum d'un mois et de 120h à raison d'au moins 1,5 h/jour.

minimum.Nheures.jour.non.annexe <- 1.5
minimum.Nheures.non.annexe      <- 120
minimum.Nmois.non.annexe        <- 1

population_minimale_calcul_médiane  <- 3
tolérance.sft                       <- 1
tolérance.nbi                       <- 2
ratio.memoire.ligne.parallele       <- 2.3

séparateur.liste.entrée     <- ";"
séparateur.décimal.entrée   <- ","
séparateur.liste.sortie     <- ";"
séparateur.décimal.sortie   <- ","

# FICHIERS EN INPUT
# conventions de nommage : les noms et chemins sont en minuscules ;
# les bases commencent par une majuscule. Un nom de fichier est souvent associé
# à une variable commençant par une majuscule et représentant la base (data.frame ou matrice)

liste.exclusions <- NULL

try ({
  chemin.liste.exclusions <- chemin("liste.exclusions.txt")
  if (file.exists(chemin.liste.exclusions))
    liste.exclusions <- read.delim(chemin.liste.exclusions, encoding=encodage.code.source, stringsAsFactors = FALSE)[[1]]
}, silent=TRUE)


nom.fichier.codes.paiement  <- paste0(racine, "codes.csv")
nom.fichier.paie            <- paste0(racine, "Table")
nom.bulletin.paie           <- paste0(racine, "Bulletins")
nom.table                   <- "Table"
nom.bulletins               <- "Bulletins"

# Outils

if (setOSWindows) {
  chemin.dossier.outils <- file.path(currentDir, "Outils")
  iconv <- file.path(chemin.dossier.outils, paste0("iconv", exec.root))
  sed   <- file.path(chemin.dossier.outils, paste0("sed", exec.root))
} else {
  iconv <- "iconv"
  sed   <- "sed"
}


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


# A priori les deux modes de lectures de tables (rapide et standard) lisent aussi bien le Windows ANSI/ISO-8859-1 que
# l'UTF-8 à condition que le Windows ANSI soit encodé par Excel ou l'éditeur de RStudio.

encodage.entrée                <-  "ISO-8859-1"
  # "WINDOWS-1252"
  # "UTF-8"
encodage.entrée.xhl2csv        <-   encodage.entrée
convertir.latin                <-   FALSE

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

expression.rég.heures.sup <- "(?:^|\\s)i\\.?h\\.?t\\.?s|i(?:(?=n)n\\w*|)\\.?\\s+\\bh(?:(?=o)o\\w*|)\\.?\\s+\\bs(?:(?=u)u\\w*|)\\.?\\b|h(?:(?=e)e\\w*|)\\.?\\s+\\b(?:(?=s)s\\w*)\\.?\\b"
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
PROF <<- data.table()

# Les valeurs ci-après seront changées par le "préprocesseur" de l'interface graphique si les valeurs explicites des codes de paye

source("versant.R", encoding = "UTF-8")
# point d'indice majoré

point_inm()

