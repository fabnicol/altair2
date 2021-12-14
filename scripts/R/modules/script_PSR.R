#### 5.6 PSR ####

#'
#'## 5.6 Contrôle de la prime de service et de rendement (PSR)   
#'   

# décret n°2009-1558 du 15 décembre 2009    

#+ psr
# -ingénieurs des ponts, des eaux et des forêts relevant du ministère chargé du développement durable ;
# -ingénieurs des travaux publics de l'Etat ;
# -techniciens supérieurs du développement durable ;
# -conducteur des travaux publics de l'Etat ;
# -experts techniques des services techniques ;
# -dessinateurs de l'équipement ;
# -inspecteurs du permis de conduire et de la sécurité routière ;
# -chargés de recherche relevant du ministère chargé du développement durable ;
# -directeurs de recherche relevant du ministère chargé du développement durable.

résultat_PSR   <- test_prime(prime_PSR, prime_IFTS, Paie_I, Paie_IFTS, Lignes_IFTS, verbeux = afficher.table.effectifs, echo = TRUE)

Lignes_PSR <<- résultat_PSR$Lignes
Paie_PSR <<- résultat_PSR$Paie

#'   
afficher_tableau_cumuls("5.6.1", "PSR/IFTS", tableau_cumuls(résultat_PSR))
#'   
#'   
conditionnel("Lien vers la base de données cumuls psr/ifts", "Bases/Reglementation/personnels.psr.ifts.csv")       
conditionnel("Lien vers la base de données PSR grade non conforme", "Bases/Reglementation/PSR.non.catAB.csv")      
conditionnel("Lien vers la base de données PSR non tit", "Bases/Reglementation/PSR.non.tit.csv")       
#'   

#'  
#'&nbsp;*Tableau 5.6.2 : Valeurs de l'agrégat annuel (PSR ou IFTS) pour les bénéficiaires de la PSR*        
#'          

agrégat_annuel(résultat_PSR, afficher.table.effectifs)

#'   
conditionnel("Lien vers la base de données agrégat PSR-IFTS", "Bases/Remunerations/beneficiaires.PSR.IFTS.csv")    
#'    

#'  
#'&nbsp;*Tableau 5.6.3 : Variations de l'agrégat mensuel moyen (PSR ou IFTS) pour les bénéficiaires de la PSR*   
#'          

évolution_agrégat(résultat_PSR, afficher.table.effectifs)

#'   
conditionnel("Lien vers la base de données variations agrégat PSR-IFTS", "Bases/Remunerations/beneficiaires.PSR.IFTS.Variation.csv")    
#'   

résultat_PSR   <- test_prime(prime_PSR, prime_IAT, Paie_I, Paie_IAT, Lignes_IAT, verbeux = afficher.table.effectifs, echo = FALSE)

#'   
afficher_tableau_cumuls("5.6.4", "PSR/IAT", tableau_cumuls(résultat_PSR))
#'   

#'      
conditionnel("Lien vers la base de données cumuls psr/iat", "Bases/Reglementation/personnels.psr.iat.csv")       
#'   


#'  
#'&nbsp;*Tableau 5.6.5 : Valeurs de l'agrégat annuel (PSR ou IAT) pour les bénéficiaires de la PSR*        
#'          

agrégat_annuel(résultat_PSR, afficher.table.effectifs)

#'   
conditionnel("Lien vers la base de données agrégat PSR-IAT", "Bases/Remunerations/beneficiaires.PSR.IAT.csv")    
#'    

#'  
#'&nbsp;*Tableau 5.6.6 : Variations de l'agrégat mensuel moyen (PSR ou IAT) pour les bénéficiaires de la PSR*   
#'          

évolution_agrégat(résultat_PSR, afficher.table.effectifs)

#'   
conditionnel("Lien vers la base de données variations agrégat PSR-IAT", "Bases/Remunerations/beneficiaires.PSR.IAT.Variation.csv")    
#'   

rm(résultat_PSR)
