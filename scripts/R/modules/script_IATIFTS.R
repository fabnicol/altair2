#'
#'## 5.4 Contrôle sur les indemnités IAT et IFTS 
#'   
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_IAT_IFTS.odt)   
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
#'&nbsp;*Tableau 5.4.1 : Cumul IAT/IFTS*   
#'      

tableau_cumuls(résultat_IAT_IFTS)

conditionnel("Lien vers la base de données cumuls IAT/IFTS", "Bases/Reglementation/personnels.iat.ifts.csv")          

rm(résultat_IAT_IFTS)  
