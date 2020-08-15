


#'
#'## 5.7 Contrôle de l'indemnité de performance et de fonctions (IPF)   
#'   

# décret n°2010-1705 du 30 décembre 2010

#+ ipf

résultat_IPF   <<- test_prime(prime_IPF, prime_IFTS, Paie_I, Paie_IFTS, Lignes_IFTS, afficher.table.effectifs)

#'   
#'    
#'&nbsp;*Tableau 5.7.1 : Cumul IPF/IFTS*   
#'      

tableau_cumuls(résultat_IPF)

#'      
#'      
conditionnel("Lien vers la base de données cumuls ipf/ifts", "Bases/Reglementation/personnels.ipf.ifts.csv")    
conditionnel("Lien vers la base de données IPF non cat.A", "Bases/Reglementation/IPF.non.catA.csv")      
conditionnel("Lien vers la base de données IPF non tit", "Bases/Reglementation/IPF.non.tit.csv")       
#'   

résultat_IPF   <<- test_prime(prime_IPF, prime_PFR, Paie_I, Paie_PFR, Lignes_PFR, verbeux = afficher.table.effectifs)

#'    
#'&nbsp;*Tableau 5.7.2 : Cumul IPF/PFR*   
#'      

tableau_cumuls(résultat_IPF)

#'      
#'      
conditionnel("Lien vers la base de données cumuls ipf/pfr", "Bases/Reglementation/personnels.ipf.pfr.csv")    
#'   

résultat_IPF   <<- test_prime(prime_IPF, prime_ISS, Paie_I, Paie_ISS, Lignes_ISS, verbeux = afficher.table.effectifs)

#'    
#'&nbsp;*Tableau 5.7.3 : Cumul IPF/ISS*   
#'      

tableau_cumuls(résultat_IPF)

#'      
#'      
conditionnel("Lien vers la base de données cumuls ipf/iss", "Bases/Reglementation/personnels.ipf.iss.csv")    
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
conditionnel("Lien vers la base de données variations agrégat IPF-IFTS", "Bases/Remunerations/beneficiaires.IPF.IFTS.Variation.csv")    
#'   

rm(résultat_IPF)  
