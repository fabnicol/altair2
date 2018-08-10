
#' 
#'## `r chapitre`.12 Contrôle sur les indemnités des élus
#'   


remunerations.elu <- Analyse.remunerations[ indemnités.élu > minimum.positif,
                                            .(Matricule,
                                              Année,
                                              Emploi,
                                              indemnités.élu,
                                              acomptes,
                                              rémunération.indemnitaire.imposable)]

remunerations.elu <- remunerations.elu[ , rémunération.indemnitaire.imposable := indemnités.élu +  rémunération.indemnitaire.imposable]

remunerations.elu <- unique(matricules[ , .(Nom,  Matricule)])[remunerations.elu, on = "Matricule"]

names(remunerations.elu) <- c("Matricule",
                              "Nom",
                              "Année",
                              "Emploi",
                              "Indemnités ",
                              "Autres ",
                              "Total ")

remunerations.elu <- na.omit(remunerations.elu)

#'   
if (générer.table.élus)   {
  
  if (nrow(remunerations.elu) > 0) {
    
    kable(remunerations.elu, row.names = FALSE)
    
  } else {
    
    cat("Tableau des indemnités d'élu : pas de données.")
  } 
} else {
  
  cat("Tableau des indemnités d'élu : non générée.")
}


#'[Lien vers la base de données Rémunérations des élus](Bases/Reglementation/remunerations.elu.csv)
#'
