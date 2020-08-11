#'
#'## 5.6 Contrôle sur les indemnités IAT et IFTS 
#'   
#'[![Notice](Notice.png)](Docs/Notices/fiche_IAT_IFTS.odt)   
#'   
#+ IAT-et-IFTS


résultat_IAT_IFTS <- test_prime(prime = prime_IAT,
                                prime_B = prime_IFTS,
                                Paie_I = Paie_I,
                                verbeux = afficher.table.effectifs)

Paie_IAT    <<- résultat_IAT_IFTS$Paie
Paie_IFTS   <<- résultat_IAT_IFTS$Paie_B
Lignes_IAT  <<- résultat_IAT_IFTS$Lignes
Lignes_IFTS <<- résultat_IAT_IFTS$Lignes_B

#'   
#'   
#'### Contrôle sur les IAT pour categories B C et non-titulaires      
#'   
#'   
conditionnel("Lien vers la base de données IAT aux non-titulaires", "Bases/Reglementation/IAT.non.tit.csv")    
conditionnel("Lien vers la base de données IAT non cat B-C", "Bases/Reglementation/IAT.non.catBC.csv")   
#'     
#'     
#'### Contrôle sur les IFTS pour categories B et non-titulaires      

#IFTS et IB >= 380 (IM >= 350)
#'  
#'   
conditionnel("Lien vers la base de données IFTS à des catég. B INM inférieur à 350", "Bases/Reglementation/IFTS.indice.anormal.csv")         
conditionnel("Lien vers la base de données IFTS aux non-titulaires", "Bases/Reglementation/IFTS.non.tit.csv")   
conditionnel("Lien vers la base de données IFTS non cat A-B", "Bases/Reglementation/IFTS.non.catAB.csv")    
#' 
#'  
#'&nbsp;*Tableau 5.6.1 : Cumul IAT/IFTS*   
#'      

tableau_cumuls(résultat_IAT_IFTS)

conditionnel("Lien vers la base de données cumuls IAT/IFTS", "Bases/Reglementation/personnels.iat.ifts.csv")          

#'     
#'### Contrôle sur le cumul logement par NAS et IFTS
#'      

#'  
#'&nbsp;*Tableau 5.6.2 : Cumul logement par NAS/IFTS*   
#'      


tableau_NAS(résultat_IAT_IFTS)

if (is.null(résultat_IAT_IFTS$NAS)) cat( "     \n", "La base des logements par NAS n'est pas détectée.   \nLe test de compatibilité de la prime avec les concessions de logements ne sera pas réalisé.")

conditionnel("Lien vers la base de données cumuls NAS/IFTS", "Bases/Reglementation/cumul.IFTS.NAS.csv")          

rm(résultat_IAT_IFTS)  
