#' Correspondance grade-catégorie
#' 
#' Etablit la correspondance entre le grade et la catégorie statutaire à partir d'un tableau importé
#' 
#' @note Requiert l'utilisation d'une table de jointure importée \bold{grades.catégories.csv} sous le répertoire \bold{Données}.
#' A défaut, tente une association approximative à partir d'expressions rationnelles appliquées aux grades.  
#' @return La \code{data.table grade.categorie} résultant de la lecture du fichier \bold{grades.catégories.csv} sous le répertoire \bold{Données}
#' @export
#'

correspondance_grade_catégorie <- function() {
    
    essayer(label ="+grade-categorie",
            {  
              "grade.categorie.existe" %a%  file.exists(chemin("grades.catégories.csv"))  
              
              vect <- c("Grade")
              
              if (grade.categorie.existe){
                
                grade.categorie <- fread(chemin("grades.catégories.csv"), # Code, Libelle,  Statut, Type, Compte
                                      sep = ";",
                                      encoding   = "Latin-1",
                                      col.names  = c(vect, "Categorie"),
                                      colClasses = c("character", "character"))  
                
                message("*****")
                message("Importation de la table de correspondance grades-catégories (grades.catégories.csv)")
                message("*****")
                
                
                grade.categorie      <- unique(grade.categorie)
                
                exporter_tableau(grade.categorie, requis = "Categorie", clé = "Grade")
               
              } else {
                
                grade.categorie <- unique(matricules[, .(Grade, Categorie)])
              }
              
              grade.categorie
              }, "L'association du grade et de la catégorie statutaire n'a pas pu être réalisée.")
}
