
#'@export
calcul_astreintes <- function() {

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
  
  sauv.bases(file.path(chemin.dossier.bases, "Reglementation"),
             environment(),
             "Controle_astreintes",
             "libelles.astreintes")
  
},
"Le contrôle Astreintes-NBI n'a pas pu être réalisé.")

essayer({
  with(Cum_astreintes,
                
                print(Tableau.vertical2(c("Année", "Montant astreintes irrégulières (euros)"),
                                            Année, V1))    
)
}, "Le tableau de contrôle des astreintes n'a pas pu être généré.")
}

#'@export
cumul_astreintes_IHTS <- function() {
  
  essayer({  
  setnames(Paie_astreintes, "indic", "indic_astr")
  setnames(Base.IHTS, "indic", "indic_IHTS")
  
  Controle_astreintes_HS_irreg <- Paie_astreintes[ , .(Matricule, Année, Mois, Code, Libellé, Type,  Montant, indic_astr) 
                                                        ][Base.IHTS[Type %in% c("I", "A", "R"), 
                                                               .(Matricule, Année, Mois, Code, Libellé, Type, Montant, indic_IHTS)], 
                                                               nomatch = 0,
                                                              on = .(Matricule, Année, Mois, Code, Libellé, Type, Montant)
                                                        ][indic_IHTS == TRUE | indic_astr == TRUE]
  
  nb.agents.IHTS.astreintes <- uniqueN(Controle_astreintes_HS_irreg$Matricule)
  
  if (nrow(Controle_astreintes_HS_irreg)) {
    cat("Des astreintes sont payées à", nb.agents.IHTS.astreintes, "personnels bénéficiaires d'IHTS.")
  }
  
  "Cum_astreintes_HS_irreg" %a% rbind(Controle_astreintes_HS_irreg[, .(round(sum(Montant[indic_astr == TRUE]), 1),
                                                                    round(sum(Montant[indic_IHTS == TRUE]), 1)),
                                                                keyby = "Année"],
                                   list("Total",
                                        Controle_astreintes_HS_irreg[indic_astr == TRUE, round(sum(Montant), 1)],
                                        Controle_astreintes_HS_irreg[indic_IHTS == TRUE, round(sum(Montant), 1)]))
  
  sauv.bases(file.path(chemin.dossier.bases, "Reglementation"),
             environment(),
             "Controle_astreintes_HS_irreg",
             "Cum_astreintes_HS_irreg")
  }, 
  "Le contrôle du cumul astreintes IHTS n'a pas pu être réalisé")
}

