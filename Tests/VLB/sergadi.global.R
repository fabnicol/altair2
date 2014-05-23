#library(compiler)
#enableJIT(3)

##---------------------------------------------------------------------------------------------------------------------
#  Chemins et variables globales du programme
##

# Cette section pourra être modifiée en entrée dans d'autres contextes
# Matricule, Codes, Avantages en nature

chemin.dossier <- "C:/Documents and Settings/fnicol/Bureau/Informatique/VLB"


bdp <- "paies-Bulletins de paye-1.csv"

##---------------------------------------------------------------------------------------------------------------------
options(width=120, warn=-1)
source(file.path(chemin.dossier, "bibliothèque.fonctions.paie.R"))

# Programme principal

##
#  Bases
##

# Lignes de paie


Bdp <- read.csv.skip(bdp)


attach(Analyse.rémunérations, warn.conflicts=FALSE)

##
#  Fonctionnaires (repérés par traitement indemnitaire > 0) : fonctionnaires stagiaires inclus (à vérif.)
##




