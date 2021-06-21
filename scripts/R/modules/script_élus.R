
#' 
#'## 5.12 Contrôle sur les indemnités des élus
#'   


remunerations.elu <- Analyse.remunerations[ indemnités.élu > minimum.positif,
                                            .(Matricule,
                                              Annee,
                                              Emploi,
                                              indemnités.élu,
                                              acomptes,
                                              rémunération.indemnitaire.imposable)]

remunerations.elu <- unique(matricules[ , .(Nom,  Matricule)])[remunerations.elu, on = "Matricule"]

names(remunerations.elu) <- c("Matricule",
                              "Nom",
                              "Annee",
                              "Emploi",
                              "Total paye ",
                              "Autres ",
                              "Total indemnités ")

remunerations.elu <- na.omit(remunerations.elu)

#'   
if (generer.table.élus)   {
  
  if (nrow(remunerations.elu) > 0) {
    
    kable(remunerations.elu, row.names = FALSE)
    
  } else {
    
    cat("Tableau des indemnités d'élu : pas de données.")
  } 
} else {
  
  cat("Tableau des indemnités d'élu : non générée.")
}

sauv.bases("Reglementation", environment(), "remunerations.elu")

conditionnel("Lien vers la base de données Rémunérations des élus", "Bases/Reglementation/remunerations.elu.csv")
#'
