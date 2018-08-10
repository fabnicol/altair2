#### 5.8 PSR ####

#'
#'## `r chapitre`.8 Contrôle de la prime de service et de rendement (PSR)   
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

résultat_PSR   <- test_prime(prime_PSR, prime_IFTS, Paie_I, Paie_IFTS, Lignes_IFTS, afficher.table.effectifs)

Lignes_PSR <- résultat_PSR$Lignes
Paie_PSR <- résultat_PSR$Paie


#'    
#'&nbsp;*Tableau `r incrément()` : Cumul PSR/IFTS*   
#'      

tableau_cumuls(résultat_PSR)

#'      
#'[Lien vers la base de données cumuls psr/ifts](Bases/Reglementation/personnels.psr.ifts.csv)       
#'[Lien vers la base de données PSR grade non conforme](Bases/Reglementation/PSR.non.catAB.csv)      
#'[Lien vers la base de données PSR non tit](Bases/Reglementation/PSR.non.tit.csv)       
#'   

#'  
#'&nbsp;*Tableau `r incrément()` : Valeurs de l'agrégat annuel (PSR ou IFTS) pour les bénéficiaires de la PSR*        
#'          

agrégat_annuel(résultat_PSR, afficher.table.effectifs)

#'   
#'[Lien vers la base de données agrégat PSR-IFTS](Bases/Remunerations/beneficiaires.PSR.IFTS.csv)    
#'    

#'  
#'&nbsp;*Tableau `r incrément()` : Variations de l'agrégat mensuel moyen (PSR ou IFTS) pour les bénéficiaires de la PSR*   
#'          

évolution_agrégat(résultat_PSR, afficher.table.effectifs)

#'   
#'[Lien vers la base de données variations agrégat PSR-IFTS](Bases/Remunerations/beneficiaires.PSR.IFTS.Variation.csv)    
#'   

résultat_PSR   <- test_prime(prime_PSR, prime_IAT, Paie_I, Paie_IAT, Lignes_IAT, afficher.table.effectifs)

#'   
#'    
#'&nbsp;*Tableau `r incrément()` : Cumul PSR/IAT*   
#'      

tableau_cumuls(résultat_PSR)

#'      
#'[Lien vers la base de données cumuls psr/iat](Bases/Reglementation/personnels.psr.iat.csv)       
#'   


#'  
#'&nbsp;*Tableau `r incrément()` : Valeurs de l'agrégat annuel (PSR ou IAT) pour les bénéficiaires de la PSR*        
#'          

agrégat_annuel(résultat_PSR, afficher.table.effectifs)

#'   
#'[Lien vers la base de données agrégat PSR-IAT](Bases/Remunerations/beneficiaires.PSR.IAT.csv)    
#'    

#'  
#'&nbsp;*Tableau `r incrément()` : Variations de l'agrégat mensuel moyen (PSR ou IAT) pour les bénéficiaires de la PSR*   
#'          

évolution_agrégat(résultat_PSR, afficher.table.effectifs)

#'   
#'[Lien vers la base de données variations agrégat PSR-IAT](Bases/Remunerations/beneficiaires.PSR.IAT.Variation.csv)    
#'   

rm(résultat_PSR)
