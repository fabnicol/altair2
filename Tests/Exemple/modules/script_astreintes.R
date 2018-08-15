#### 5.12 ASTREINTES ####


#'
#'## 5.12 Contrôle des indemnités pour astreintes    
#'  
#'[![Notice](Notice.png)](Docs/Notices/fiche_astreintes.odt)   
#'

#'  
#'&nbsp;*Tableau 5.12.1 : Cumuls irréguliers NBI et astreintes (responsabilité supérieure)*   
#'  

insérer_script(fonction = "calcul_astreintes")

#'**Nota**    
#'Vérifier l'adéquation des libellés de paye d'astreinte dans le tableau en lien ci-après.      
#'Définition des fonctions de responsabilité supérieure : décrets du 27 décembre 2001 et du 28 décembre 200    

conditionnel("Lien vers la base des cumuls astreintes/NBI", "Bases/Reglementation/Controle_astreintes.csv")   

conditionnel("Lien vers les libellés et codes astreintes", "Bases/Reglementation/libelles.astreintes.csv")     
#'   

insérer_script(fonction = "cumul_astreintes_IHTS")
  
#'  
#'&nbsp;*Tableau 5.12.2 : Cumuls potentiellement irréguliers IHTS et astreintes*   
#'  

with(Cum_astreintes_HS_irreg,
     
     print(Tableau.vertical2(c("Année", "Montant astreintes potentiellement irrégulières (euros)", "Montant IHTS correspondantes"),
                                Année, V1, V2))    
     
)

#'**Nota**:     
#'Les cumuls peuvent être réguliers s'il y a eu des interventions non compensées en période d'astreinte.      
conditionnel("Lien vers la base des cumuls astreintes/IHTS", "Bases/Reglementation/Controle_astreintes_HS_irreg.csv")
conditionnel("Lien vers les cumuls annuels", "Bases/Reglementation/Cum_astreintes_HS_irreg.csv")


rm(Base.IHTS)
