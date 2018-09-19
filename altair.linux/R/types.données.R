# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
# produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
# en vigueur à compter de l'année 2008.
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
# Il est important de maîtriser le type des données en entrée de lecture de base

# Exemple : si pas de hors échelle lettres dans une base, l'algorithme de lecture de base de data.table et d'inférence de type
# va proposer "numeric" au lieu de "character" pour l'échelon

# Autre exemple : les matricules composés exclusivement de chiffres vont être traités comme des nombres en entrée, alors qu'ils
# sont utilisés comme des chaînes de caractères dans le code

library(bit64)

#' Contrôle le type des données d'entrée
#' @param colonnes Vecteur des noms de colonnes de la table globale
#' @export

type.données <- function(colonnes) {
  
"intégrer.rang" %a% ("R" %chin% colonnes) 
"intégrer.échelon" %a% ("Echelon" %chin% colonnes)
"intégrer.localisation" %a% ("Siret" %chin% colonnes)

if (intégrer.rang) message("Intégration du Rang")
if (intégrer.échelon) message("Intégration de l'échelon")
if (intégrer.localisation) message("Intégration des données établissement")

localisation.classes <- if (intégrer.localisation) c("character", "character", "character", "character") else NULL
échelon.classe <- if (intégrer.échelon) "character" else NULL
rang.classe    <- if (intégrer.rang) "integer" else NULL


"colonnes.classes.input"    %a% c(rang.classe, "integer", "integer",  
                               localisation.classes,
                               "character", "character", "character",
                               "character", "numeric", "character", "numeric", "numeric", "numeric",
                               "integer",  "numeric", "numeric", "numeric", "numeric",
                               "character",  "character", "numeric", "numeric", "numeric",
                               "numeric", "character", "character", "character", "character", "character", échelon.classe, "character", "character")

"colonnes.bulletins.classes.input" %a% c(rang.classe, "integer", "integer",
                                      localisation.classes,
                                      "character", "character", "character",
                                      "character", "numeric", "character", "numeric", "numeric", "numeric",
                                      "integer",  "numeric", "numeric", "numeric", "numeric",
                                      "character", "character", "character", échelon.classe, "character", "character")


"trans" %a% data.table(t(matrix( c("I",  "Indemnité",
                                "R" , "Rappels",
                                "IR", "Indemnité de résidence",
                                "T" , "Traitement",
                                "AV", "Avantage en nature",
                                "A" , "Autres rémunérations",
                                "C" , "Cotisations",
                                "D" , "Déductions",
                                "S" , "Supplément familial",
                                "RE", "Retenues",
                                "CO" , "Commentaire"), nrow = 2)))

}

#' @export

remplacer_type <- function(M) {
  trans2 <- data.table::copy(trans)
  setnames(trans2, c("V1", "V2"), c("Type", "Type_long"))
  M <- unique(M, by = NULL)
  M <- merge(M, trans2, by = "Type", all.x = TRUE)[, Type := NULL]
  setnames(M, "Type_long", "Type")
}


#' @export

résumer_type <- function(M) {
  trans2 <- data.table::copy(trans)
  setnames(trans2, c("V1", "V2"), c("Type_court", "Type"))
  M <- unique(M, by = NULL)
  M <- merge(M, trans2, by = "Type", all.x = TRUE)[, Type := NULL]
  setnames(M, "Type_court", "Type")
}

