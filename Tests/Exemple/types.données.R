# Il est important de maîtriser le type des données en entrée de lecture de base

# Exemple : si pas de hors échelle lettres dans une base, l'algorithme de lecture de base de data.table et d'inférence de type
# va proposer "numeric" au lieu de "character" pour l'échelon

# Autre exemple : les matricules composés exclusivement de chiffres vont être traités comme des nombres en entrée, alors qu'ils
# sont utilisés comme des chaînes de caractères dans le code

library(bit64)

localisation.classes <- if (intégrer.localisation) c("character", "character", "character", "character") else NULL
échelon.classe <- if (intégrer.échelon) "character" else NULL
rang.classe    <- if (intégrer.rang) "integer" else NULL


colonnes.classes.input    <- c(rang.classe, "integer", "integer",  
                               localisation.classes,
                               "character", "character", "character",
                               "character", "numeric", "character", "numeric", "numeric", "numeric",
                               "integer",  "numeric", "numeric", "numeric", "integer",
                               "character",  "character", "numeric", "numeric", "numeric",
                               "numeric", "character", "character", "character", échelon.classe, "character", "character")

colonnes.bulletins.classes.input <- c(rang.classe, "integer", "integer",
                                      localisation.classes,
                                      "character", "character", "character",
                                      "character", "numeric", "character", "numeric", "numeric", "numeric",
                                      "integer",  "numeric", "numeric", "numeric", "numeric",
                                      "character", "character", "character", échelon.classe, "character", "character")


remplacer_type <- function(v) unlist(sapply(v, function(x) switch(x,
                   "I" = "Indemnité",
                   "R" = "Rappels",
                   "IR"= "Indemnité de résidence",
                   "T" = "Traitement",
                   "AV"= "Avantage en nature",
                   "A" = "Autres rémunérations",
                   "C" = "Cotisations",
                   "D" = "Déductions",
                   "S" = "Supplément familial",
                   "RE"= "Retenues",
                   "C" = "Commentaire")))
