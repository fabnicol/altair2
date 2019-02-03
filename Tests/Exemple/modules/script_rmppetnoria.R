##### 4.3.3 Effet de noria et de variation d'effectifs sur rémunérations moyennes ######

attach(Analyse.variations.par.exercice)

#'   
#'##### Les filtres RMPP appliqués au 4.3.1 et 4.3.2 ne sont plus appliqués. Il peut en résulter des variations significativement différentes de celles calculées précédemment.
#'         

#'
#'### 4.3.3 Effet de noria et de variation d'effectifs sur rémunérations moyennes       
#'     
#'*Les calculs de RMPP sur la première année de la période sous revue sont estimatifs et ne devraient pas être pris en compte pour publication.*      
#'   
#'      
#'**Effet de noria et de variations d'effectifs sur rémunérations nettes moyennes EQTP**   
#'  
#'&nbsp;*Tableau 4.3.3.1*   
#'    
#+ noria-tableau

essayer(label = "+noria nette globale", invisible(noria(champ = "net")),
        "Le calcul de l'effet de noria n'a pas pu être réalisé sur les rémunérations nettes moyennes.")

#'  
#'**Effet de noria et de variations d'effectifs sur rémunérations brutes moyennes EQTP**   
#'  
#'&nbsp;*Tableau 4.3.3.2*   
#'    

essayer(label = "+noria brute globale", invisible(noria(champ = "brut")),
        "Le calcul de l'effet de noria n'a pas pu être réalisé sur les rémunérations brutes moyennes.")

#'  
#'*Note :*        
#'*Effet de noria* : *variation des rémunérations liées au remplacement de salariés sortants par un même nombre d'entrants.*   
#'*Effet des variations d'effectifs (ou de périmètre)* : *variation des rémunérations liées à la différence entre nombre de sortants et d'entrants.*    
#'*Les effets de vacances d'emploi, s'ils existent, sont intégrés dans le calcul de l'effet de noria*     
#'*Voir les exemples traités en bas de page du lien technique suivant*     
#'[Spécifications techniques des tables RMPP et effet de noria](Docs/Notices/noria.html)      
#'[Voir complément méthodologique DGFIP](Docs/Notices/GVT et noria.pdf)        
#'  

##### 4.3.4 Effet de noria et de variation d'effectifs sur rémunérations moyennes des fonctionnaires  ######

#'
#'### 4.3.4 Effet de noria et de variation d'effectifs sur rémunérations moyennes des fonctionnaires           
#'   
#'   
#'**Effet de noria et de variations d'effectifs sur rémunérations nettes moyennes EQTP des fonctionnaires**   
#'  
#'&nbsp;*Tableau 4.3.4.1*   
#'    
#+ noria-tableau2

essayer(invisible(noria(champ = "net", filtre = c("TITULAIRE", "STAGIAIRE"))),
        "Le calcul de l'effet de noria n'a pas pu être réalisé sur les rémunérations nettes moyennes des fonctionnaires.")


#'  
#'**Effet de noria et de variations d'effectifs sur rémunérations brutes moyennes EQTP des fonctionnaires**   
#'  
#'&nbsp;*Tableau 4.3.4.2*   
#'    

essayer(invisible(noria(champ = "brut", filtre = c("TITULAIRE", "STAGIAIRE"))),
        "Le calcul de l'effet de noria n'a pas pu être réalisé sur les rémunérations brutes moyennes des fonctionnaires.")

##### 4.3.5 Effet de noria et de variation d'effectifs sur rémunérations moyennes par catégorie statutaire  ######

#'
#'### `r chapitre`.3.5 Effet de noria et de variation d'effectifs sur rémunérations moyennes par catégorie statutaire             
#'   
#'   
#'**Effet de noria et de variations d'effectifs sur rémunérations nettes moyennes EQTP des fonctionnaires de catégorie A**   
#'  
#'&nbsp;*Tableau 4.3.5.1*   
#'    
#+ noria-tableau3

essayer(invisible(noria(champ = "net", filtre = "A")),
        "Le calcul de l'effet de noria n'a pas pu être réalisé sur les rémunérations nettes moyennes des fonctionnaires de catégorie A.")

#'  
#'**Effet de noria et de variations d'effectifs sur rémunérations brutes moyennes EQTP des fonctionnaires de catégorie A**   
#'  
#'&nbsp;*Tableau 4.3.5.2*   
#'    

essayer(invisible(noria(champ = "brut", filtre = "A")),
        "Le calcul de l'effet de noria n'a pas pu être réalisé sur les rémunérations brutes moyennes des fonctionnaires de catégorie A.")

#'   
#'**Effet de noria et de variations d'effectifs sur rémunérations nettes moyennes EQTP des fonctionnaires de catégorie B**   
#'  
#'&nbsp;*Tableau 4.3.5.3*   
#'    
#+ noria-tableau4

essayer(invisible(noria(champ = "net", filtre = "B")),
        "Le calcul de l'effet de noria n'a pas pu être réalisé sur les rémunérations nettes moyennes des fonctionnaires de catégorie B.")

#'  
#'**Effet de noria et de variations d'effectifs sur rémunérations brutes moyennes EQTP des fonctionnaires de catégorie B**   
#'  
#'&nbsp;*Tableau 4.3.5.4*   
#'    

essayer(invisible(noria(champ = "brut", filtre = "B")),
        "Le calcul de l'effet de noria n'a pas pu être réalisé sur les rémunérations brutes moyennes des fonctionnaires de catégorie B.")

#'   
#'**Effet de noria et de variations d'effectifs sur rémunérations nettes moyennes EQTP des fonctionnaires de catégorie C**   
#'  
#'&nbsp;*Tableau 4.3.5.5*   
#'    
#+ noria-tableau5

essayer(invisible(noria(champ = "net", filtre = "C")),
        "Le calcul de l'effet de noria n'a pas pu être réalisé sur les rémunérations nettes moyennes des fonctionnaires de catégorie C.")

#'  
#'**Effet de noria et de variations d'effectifs sur rémunérations brutes moyennes EQTP des fonctionnaires de catégorie C**   
#'  
#'&nbsp;*Tableau 4.3.5.6*   
#'    

essayer(invisible(noria(champ = "brut", filtre = "C")),
        "Le calcul de l'effet de noria n'a pas pu être réalisé sur les rémunérations brutes moyennes des fonctionnaires de catégorie C.")

#'     

detach(Analyse.variations.par.exercice)
