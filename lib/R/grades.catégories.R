#' Correspondance grade-categorie
#' 
#' Etablit la correspondance entre le grade et la catégorie statutaire à partir d'un tableau importé
#' 
#' @note Requiert l'utilisation d'une table de jointure importée \bold{grades.categories.csv} encodée en UTF-8 sous le répertoire \bold{Données}.
#' A défaut, tente une association approximative à partir d'expressions rationnelles appliquées aux grades.  
#' @return La \code{data.table grade.categorie} résultant de la lecture du fichier \bold{grades.categories.csv} sous le répertoire \bold{Données}
#' @export
#'

correspondance_grade_categorie <- function() {
    
    essayer(label ="+grade-categorie",
            {  
              "grade.categorie.existe" %a%  file.exists(chemin("grades.categories.csv"))  
              
              if (grade.categorie.existe){
                
                grades.categorie <- fread(chemin("grades.categories.csv"), # Code, Libelle,  Statut, Type, Compte
                                      sep = ";",
                                      encoding   = "UTF-8",
                                      col.names  = c("Grade", "Categorie"),
                                      colClasses = c("character", "character"))  
                
                message("*****")
                message("Importation de la table de correspondance grades-catégories (grades.categories.csv)")
                message("*****")
                
                
                grades.categorie      <- unique(grades.categorie)
                Paie[ , Categorie := NULL]
                Bulletins.paie[ , Categorie := NULL]
                
                exporter_tableau(grades.categorie, requis = "Categorie", cles = "Grade")
               
              } else {
                
                grades.categorie <- unique(matricules[, .(Grade, Categorie)])
              }
              
              grades.categorie
              }, "L'association du grade et de la catégorie statutaire n'a pas pu être réalisée.")
}

