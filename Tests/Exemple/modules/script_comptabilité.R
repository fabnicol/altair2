#' 
#'## 5.14 Lien avec le compte de gestion    
#'

#'   
#'*Pour tirer pleinement profit de ces fonctionnalités, il est préférable de faire remplir, par les organismes contrôlés le tableau CSV accessible dans le bloc* **Bugdet** *de l'onglet Extra de l'application graphique, ou bien à ce lien. Voir aussi la notice* &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_tableau_budget.odt)      
#'   

Paie[, Compte := NULL]  

correspondance_paye_budget()

#'  
conditionnel("Lien vers la base détaillée des cumuls des lignes de paie", "Bases/Reglementation/cumul.lignes.paie.csv")
#'  
conditionnel("Lien vers la base agrégée des cumuls des lignes de paie", "Bases/Reglementation/cumul.total.lignes.paie.csv")
#'  

#'  
#'*Avertissement : les rappels comprennent également les rappels de cotisations et déductions diverses.*    
#'   
