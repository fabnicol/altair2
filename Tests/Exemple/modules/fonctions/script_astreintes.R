calcul <- function() {

essayer({  
  
  "Paie_astreintes" %a% filtrer_Paie("ASTREINTES", portée = "Mois", indic = TRUE)
  
  libelles.astreintes <- unique(Paie_astreintes[indic == TRUE, .(Code, Libellé)], by = NULL)
  
  Controle_astreintes <- Paie_astreintes[! is.na(NBI) 
                                         & NBI > 0
                                         & indic == TRUE,
                                         .(Matricule, Année, Mois, Catégorie, Emploi, Grade, NBI, Code, Libellé, quotité, Montant)
                                         ][Paie_NBI[ , .(Matricule, Année, Mois, Code, Libellé, Montant)],  
                                           nomatch = 0,
                                           on = .(Matricule, Année, Mois)]  
  
  Controle_astreintes <- Controle_astreintes[Catégorie == "A" 
                                             & grepl("d(?:\\.|ir)\\w*\\s*\\bg(?:\\.|\\w*n\\.?\\w*)\\s*\\b(?:des?)\\s*\\bs\\w.*", 
                                                     paste(Emploi, Grade), 
                                                     perl = TRUE,
                                                     ignore.case = TRUE)]
  
  setnames(Controle_astreintes, c("Code", "Libellé", "Montant"), c("Code.astreinte", "Libellé.astreinte", "Montant.astreinte"))
  setnames(Controle_astreintes, c("i.Code", "i.Libellé", "i.Montant"), c("Code.NBI", "Libellé.NBI", "Montant.NBI"))
  
  nb.agents.NBI.astreintes <- uniqueN(Controle_astreintes$Matricule)
  
  if (nrow(Controle_astreintes)) {
    cat("Des astreintes sont payées à", nb.agents.NBI.astreintes, "personnels bénéficiaires de NBI")
  }
  
  "Cum_astreintes" %a% rbind(Controle_astreintes[, round(sum(Montant.astreinte), 1),
                                              by = "Année"],
                          list("Total", Controle_astreintes[, round(sum(Montant.astreinte), 1)]))
},
"Le contrôle Astreintes-NBI n'a pas pu être réalisé.")

essayer({
  with(Cum_astreintes,
                
                print(Tableau.vertical2(c("Année", "Montant astreintes irrégulières (euros)"),
                                            Année, V1))    
)
}, "Le tableau de contrôle des astreintes n'a pas pu être généré.")
}
