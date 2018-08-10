#### 5.15 ASTREINTES ####


#'
#'## `r chapitre`.15 Contrôle des indemnités pour astreintes    
#'  
#'[![Notice](Notice.png)](Docs/Notices/fiche_astreintes.odt)   
#'

#'  
#'&nbsp;*Tableau `r incrément()` : Cumuls irréguliers NBI et astreintes (responsabilité supérieure)*   
#'  

insérer_script(file.path(chemin.modules, "fonctions", "script_astreintes.R"), fonction = "calcul")

#'**Nota**    
#'Vérifier l'adéquation des libellés de paye d'astreinte dans le tableau en lien ci-après.      
#'Définition des fonctions de responsabilité supérieure : décrets du 27 décembre 2001 et du 28 décembre 200    
#'[Lien vers la base des cumuls astreintes/NBI](Bases/Reglementation/Controle_astreintes.csv)   
#'[Lien vers les libellés et codes astreintes](Bases/Reglementation/libelles.astreintes.csv)     
#'   

essayer({ setnames(Paie_astreintes, "indic", "indic_astr")
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
  
  Cum_astreintes_HS_irreg <- rbind(Controle_astreintes_HS_irreg[, .(round(sum(Montant[indic_astr == TRUE]), 1),
                                                                    round(sum(Montant[indic_IHTS == TRUE]), 1)),
                                                                keyby = "Année"],
                                   list("Total",
                                        Controle_astreintes_HS_irreg[indic_astr == TRUE, round(sum(Montant), 1)],
                                        Controle_astreintes_HS_irreg[indic_IHTS == TRUE, round(sum(Montant), 1)]))
  
  
}, 
"Le contrôle du cumul astreintes IHTS n'a pas pu être réalisé")

#'  
#'&nbsp;*Tableau `r incrément()` : Cumuls potentiellement irréguliers IHTS et astreintes*   
#'  

with(Cum_astreintes_HS_irreg,
     
     Tableau.vertical2(c("Année", "Montant astreintes potentiellement irrégulières (euros)", "Montant IHTS correspondantes"),
                       Année, V1, V2)    
     
)

#'**Nota**:     
#'Les cumuls peuvent être réguliers s'il y a eu des interventions non compensées en période d'astreinte.      
#'[Lien vers la base des cumuls astreintes/IHTS](Bases/Reglementation/Controle_astreintes_HS_irreg.csv)   
#'[Lien vers les cumuls annuels](Bases/Reglementation/Cum_astreintes_HS_irreg.csv)   


rm(Base.IHTS)
