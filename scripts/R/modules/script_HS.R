#'    
#'## 5.11 Contrôle sur les heures supplémentaires       
#'  
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_IHTS.odt)
# Sont repérées comme heures supplémentaires ou complémentaires les heures dont le libellé obéissent à
# l'expression régulière expression.rég.heures.sup donnée par le fichier prologue.R
#'    
#'     
#'&nbsp;*Tableau 5.11.1 : Paiements au-delà des seuils de liquidation pour l'exercice* 
#'    

# On considère le taux horaire maximum de nuit et la somme des IHTS et on teste su la somme des IHTS est supérieures à ce que donnerait l'application du taux de nuit

# Vérification des seuils annuels :

inserer_script(fonction = c("calcul_HS", "dépassements_HS"))

# On considère le taux horaire maximum de nuit et la somme des IHTS 

#'  
#'&nbsp; [![Notice](icones/Notice.png)](Docs/Notices/fiche_liquidation_IHTS.odt)      
#' 
#'     
conditionnel("Lien vers la base de données des IHTS aux non-titulaires", "Bases/Reglementation/Base.IHTS.non.tit.csv")           
conditionnel("Lien vers le tableau des dépassements individuels des seuils de liquidation", "Bases/Reglementation/depassement.agent.annee.csv")         
conditionnel("Lien vers la base de données dépassements individuels des seuils de liquidation", "Bases/Reglementation/depassement.agent.csv")         
conditionnel("Lien vers la base de données calcul des taux horaires individuels", "Bases/Reglementation/Taux.horaires.csv")        
#'       
#'     
#'*Le cumul des heures supplémentaires déclarées (colonne Heures.Sup. des bases) est, par année, comparé au cumul des bases de liquidation IHTS, pour l'année et en régularisation l'année suivante au titre du même exercice*    
#'*Le volume d'heures supplémentaires déclarées et non liquidées sous forme d'IHTS peut correspondre à d'autres régimes d'heures supplémentaires (enseignants, élections) ou à des heures supplémentaires non effectuées ou sous-déclarées*   
#'*Des différences importantes peuvent indiquer une mauvaise fiabilité des déclarations d'heures supplémentaires et/ou des bases de liquidation IHTS*             
#'           
#'       
#'&nbsp;*Tableau 5.11.2 : Cumuls d'heures supplémentaires déclarées et des IHTS payees, en nombre d'heures*     
#'    

inserer_script(fonction = "cumuls_HS")

#'    
conditionnel("Lien vers les données du tableau", "Bases/Reglementation/CumHS.csv")     
conditionnel("Lien vers les cumuls IHTS par matricule", "Bases/Reglementation/lignes.IHTS.tot.csv")       
conditionnel("Lien vers les lignes IHTS", "Bases/Reglementation/lignes.IHTS.csv")              
#'   
#'  
#'  
#'&nbsp;*Tableau 5.11.3 : Heures supplémentaires au-delà des seuils*   
#'    

inserer_script(fonction = "plafonds_HS")

#'
conditionnel("Lien vers la base de données Heures supplémentaires en excès du seuil de 15h (FPH) ou de 25h/mois (FPT)", "Bases/Reglementation/HS.sup.25.csv")     
conditionnel("Lien vers la base de données cumuls en excès des seuils annuels de 180 h (FPH)", "Bases/Reglementation/Depassement.seuil.180h.csv")    
conditionnel("Lien vers la base de données cumuls en excès des seuils annuels de 220 h (FPH)", "Bases/Reglementation/Depassement.seuil.220h.csv")   
conditionnel("Lien vers la base de données IHTS versées à des fonctionnaires de cat. A", "Bases/Reglementation/ihts.cat.A.csv")      

#'   
#'**Nota :**   
#'HS en excès : au-delà de 25 heures par mois dans la FPT et 15 heures par mois dans la FPH, sauf pour certains emplois (18,3 heures par mois)     
#'IHTS cat.A : attribuées à des fonctionnaires ou non-titulaires de categorie A ou assimilés.    
#'Dans les tableaux en lien les grades, emplois et service sont ceux connus en fin d'annee.    
