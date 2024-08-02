#'
#'## 5.7 Contrôle de l'indemnité de performance et de fonctions (IPF)   
#'   

# décret n°2010-1705 du 30 décembre 2010

#+ ipf

résultat_IPF   <<- test_prime(prime_IPF, prime_IFTS, Paie_I, Paie_IFTS, Lignes_IFTS, verbeux = afficher.table.effectifs, echo = TRUE)

#'   
afficher_tableau_cumuls("5.7.1", "IPF/IFTS", tableau_cumuls(résultat_IPF))
#'  
   
conditionnel("Lien vers la base de données IPF non cat.A", "Bases/Reglementation/IPF.non.catA.csv")      
conditionnel("Lien vers la base de données IPF non tit", "Bases/Reglementation/IPF.non.tit.csv")       
#'   

résultat_IPF   <<- test_prime(prime_IPF, prime_PFR, Paie_I, Paie_PFR, Lignes_PFR, verbeux = afficher.table.effectifs, echo = FALSE)

#'   
afficher_tableau_cumuls("5.7.2", "IPF/PFR", tableau_cumuls(résultat_IPF))
#'  
#'      
conditionnel("Lien vers la base de données cumuls ipf/pfr", "Bases/Remunerations/beneficiaires.IPF.PFR.csv")    
#'   

résultat_IPF   <<- test_prime(prime_IPF, prime_ISS, Paie_I, Paie_ISS, Lignes_ISS, verbeux = afficher.table.effectifs, echo = FALSE)

#'   
afficher_tableau_cumuls("5.7.3", "IPF/ISS", tableau_cumuls(résultat_IPF))
#'  

#'      
#'      
conditionnel("Lien vers la base de données cumuls ipf/iss", "Bases/Remunerations/beneficiaires.IPF.ISS.csv")    
#'   


# Attention keyby = et pas seulement by = !

#'  
#'&nbsp;*Tableau 5.7.4 : Valeurs de l'agrégat annuel (IPF ou IFTS) pour les bénéficiaires de l'IPF*        
#'          

agrégat_annuel(résultat_IPF, afficher.table.effectifs)

#'  
#'&nbsp;*Tableau 5.7.5 : Variations de l'agrégat mensuel moyen (IPF ou IFTS) pour les bénéficiaires de l'IPF*   
#'          

évolution_agrégat(résultat_IPF, afficher.table.effectifs)

#'   
conditionnel("Lien vers la base de données agrégat IPF-IFTS", "Bases/Remunerations/beneficiaires.IPF.IFTS.csv")    
#'    
#'   
#'   

rm(résultat_IPF)  
