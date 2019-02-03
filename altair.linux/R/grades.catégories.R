#' Correspondance grade-catégorie
#' 
#' Etablit la correspondance entre le grade et la catégorie statutaire à partir d'un tableau importé
#' 
#' @return La \code{data.table grade.categorie} résultant de la lecture du fichier \bold{grades.catégories.csv} sous le répertoire \bold{Données}
#'  classe de références (RefClass).
#' @note Requiert l'utilisation d'une table de jointure importée \bold{grades.catégories.csv} sous le répertoire \bold{Données}.
#' A défaut, tente une association approximative à partir d'expressions rationnelles appliquées aux grades.  
#' @author Fabrice Nicol   
#' @export
#'

correspondance_grade_catégorie <- function() {
    
    essayer(label ="+grade-categorie",
            {  
              "grade.categorie.existe" %a%  file.exists(chemin("grades.categories.csv"))  
              
              if (grade.categorie.existe){
                
                grades.categorie <- fread(chemin("grades.categories.csv"), # Code, Libelle,  Statut, Type, Compte
                                      sep = ";",
                                      encoding   = "Latin-1",
                                      col.names  = c("Grade", "Categorie"),
                                      colClasses = c("character", "character"))  
                
                message("*****")
                message("Importation de la table de correspondance grades-catégories (grades.catégories.csv)")
                message("*****")
                
                
                grades.categorie      <- unique(grades.categorie)
                Paie[ , Categorie := NULL]
                Bulletins.paie[ , Categorie := NULL]
                
                exporter_tableau(grades.categorie, requis = "Categorie", clés = "Grade")
               
              } else {
                
                grades.categorie <- unique(matricules[, .(Grade, Categorie)])
              }
              
              grades.categorie
              }, "L'association du grade et de la catégorie statutaire n'a pas pu être réalisée.")
}

