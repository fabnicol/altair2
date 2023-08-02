# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, annees 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
# produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
# en vigueur à compter de l'annee 2008.
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
# Il est important de maîtriser le type des données en entree de lecture de base

# Exemple : si pas de hors échelle lettres dans une base, l'algorithme de lecture de base de data.table et d'inférence de type
# va proposer "numeric" au lieu de "character" pour l'échelon

# Autre exemple : les matricules composés exclusivement de chiffres vont être traités comme des nombres en entree, alors qu'ils
# sont utilisés comme des chaînes de caractères dans le code

library(bit64)

#' Contrôle le type des données d'entrée
#' @export

type.données <- function() {
  
"bulletins.noms"  %a% c("Année", "Mois",
                       "Nom", "Prénom", "Matricule",
                       "Service", "Nb.Enfants", "Statut",
                       "Temps.de.travail", "Heures.Sup.", "Heures",
                       "Indice", "Brut", "Net",
                       "Net.à.Payer","NBI",
                       "Emploi", "Grade", "Evénement",
                       "Echelon", "Catégorie", "Nir")
 
  "lignes.noms"  %a% c("Année", "Mois",
                       "Nom", "Prénom", "Matricule",
                       "Service", "Nb.Enfants", "Statut",
                       "Temps.de.travail", "Heures.Sup.", "Heures",
                       "Indice", "Brut", "Net",
                       "Net.à.Payer", "NBI",
                       "Libellé", "Code", "Base",
                       "Taux", "Nb.Unite", "Montant",
                       "Début","Fin","Type",
                       "Emploi", "Grade", 
                       "Echelon", "Catégorie", "Nir")

"colonnes.classes.input"    %a% c(Année = "integer", Mois = "integer",  
                                  Nom = "character", Prénom = "character", Matricule = "character",
                                  Service = "character", Nb.Enfants = "numeric", Statut = "character", 
                                  Temps.de.travail =  "numeric",  Heures.Sup. = "numeric", Heures = "numeric",
                                  Indice = "integer", Brut = "numeric", Net = "numeric", 
                                  Net.à.Payer = "numeric", NBI = "numeric",
                                  Libellé = "character",  Code = "character", Base = "numeric", 
                                  Taux = "numeric", Nb.Unite = "numeric", Montant = "numeric",
                                  Début =  "character", Fin = "character", Type = "character",
                                  Emploi =  "character", Grade = "character", 
                                  Echelon =  "character", Catégorie = "character", Nir = "character")

"colonnes.bulletins.classes.input" %a% c(Année = "integer", Mois = "integer",  
                                         Nom = "character", Prénom = "character", Matricule = "character",
                                         Service = "character", Nb.Enfants = "numeric", Statut = "character", 
                                         Temps.de.travail =  "numeric",  Heures.Sup. = "numeric", Heures = "numeric",
                                         Indice = "integer", Brut = "numeric", Net = "numeric", 
                                         Net.à.Payer = "numeric", NBI = "numeric",
                                         Emploi = "character", Grade = "character", Evénement = "character",
                                         Echelon = "character", Catégorie = "character", Nir = "character")

"trans" %a% data.table(t(matrix( c("I",  "Indemnité",
                                "AC", "Acompte",
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
  M <- unique(M)
  M <- merge(M, trans2, by = "Type", all.x = TRUE)[, Type := NULL]
  setnames(M, "Type_long", "Type")
}


#' @export

résumer_type <- function(M) {
  trans2 <- data.table::copy(trans)
  setnames(trans2, c("V1", "V2"), c("Type_court", "Type"))
  M <- unique(M)
  M <- merge(M, trans2, by = "Type", all.x = TRUE)[, Type := NULL]
  setnames(M, "Type_court", "Type")
}

