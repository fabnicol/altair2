#'   
#'## 5.8 Contrôle du RIFSEEP (IFSE)  
#'    
#'*Pour tirer pleinement profit de ces fonctionnalités, il est préférable de faire remplir, par les organismes contrôlés le tableau CSV accessible dans le bloc* **IFSE** *de l'onglet Extra de l'application graphique, ou bien à ce lien. Voir aussi la notice* &nbsp; [![Notice](icones/Notice.png)](Docs/Notices/fiche_tableau_ifse.odt)      
#'   

# article 88 de la Loi 2016-483 du 20 avril 2016
# Décret n°2014-513 du 20 mai 2014 portant création de la RIFSEEP

# non cumuleable entre autres avec :
# - l'indemnité forfaitaire pour travaux supplémentaires
# - la prime de service et de rendement
# - l'indemnité de fonctions et de résultats
# - la prime de fonctions informatiques
# - l'indemnité d'administration et de technicité
# - l'indemnité d'exercice de mission des préfectures
# - l'indemnité spécifique de service (I.S.S.)

#+ rifseep

if (! exists("Paie_IFTS")) Paie_IFTS <- NULL

résultat_IFSE   <- test_prime(prime_IFSE, prime_IFTS, Paie_I, Paie_IFTS, Lignes_IFTS, verbeux = afficher.table.effectifs, echo = TRUE)

#'   
afficher_tableau_cumuls("5.8.1", "IFSE/IFTS", tableau_cumuls(résultat_IFSE))
#'      
conditionnel("Lien vers la base de données cumuls ifse/ifts", "Bases/Remunerations/beneficiaires.IFSE.IFTS.csv")
#'   
#'    

if (! exists("Paie_IAT")) Paie_IAT <- NULL

résultat_IFSE   <- test_prime(prime_IFSE, prime_IAT, Paie_I, Paie_IAT, Lignes_IAT, verbeux = afficher.table.effectifs, echo = FALSE)

#'      
afficher_tableau_cumuls("5.8.2", "IFSE/IAT", tableau_cumuls(résultat_IFSE))
#'      
#'   
conditionnel("Lien vers la base de données cumuls ifse/iat", "Bases/Remunerations/beneficiaires.IFSE.IAT.csv")    
#'   

if (! exists("Paie_ISS")) Paie_ISS <- NULL

résultat_IFSE   <- test_prime(prime_IFSE, prime_ISS, Paie_I, verbeux = afficher.table.effectifs, echo = FALSE)

#'   
#'   
afficher_tableau_cumuls("5.8.3", "IFSE/ISS", tableau_cumuls(résultat_IFSE))
#'      
#'      
conditionnel("Lien vers la base de données cumuls ifse/iss", "Bases/Remunerations/beneficiaires.IFSE.ISS.csv")    
#'   

if (! exists("Paie_IEMP")) Paie_IEMP <- NULL

résultat_IFSE   <- test_prime(prime_IFSE, prime_IEMP, Paie_I, verbeux = afficher.table.effectifs, echo = FALSE)

#'   
afficher_tableau_cumuls("5.8.4", "IFSE/IEMP", tableau_cumuls(résultat_IFSE))
#'      
#'      
conditionnel("Lien vers la base de données cumuls ifse/iemp", "Bases/Remunerations/beneficiaires.IFSE.IEMP.csv")    
#'   

if (! exists("Paie_PFI")) Paie_PFI <- NULL

résultat_IFSE   <- test_prime(prime_IFSE, prime_PFI, Paie_I, verbeux = afficher.table.effectifs, echo = FALSE)

#'   
afficher_tableau_cumuls("5.8.5", "IFSE/PFI", tableau_cumuls(résultat_IFSE))
#'      
#'      
conditionnel("Lien vers la base de données cumuls ifse/pfi", "Bases/Remunerations/beneficiaires.IFSE.PFI.csv")    
#'   

if (! exists("Paie_PSR")) Paie_PSR <- NULL

résultat_IFSE   <- test_prime(prime_IFSE, prime_PSR, Paie_I, Paie_PSR, Lignes_PSR, verbeux = afficher.table.effectifs, echo = FALSE)

#'   
afficher_tableau_cumuls("5.8.6", "IFSE/PSR", tableau_cumuls(résultat_IFSE))
#'      
#'      
conditionnel("Lien vers la base de données cumuls ifse/psr", "Bases/Remunerations/beneficiaires.IFSE.PSR.csv")    
#'   

if (! exists("Paie_PFR")) Paie_PFR <- NULL

résultat_IFSE   <- test_prime(prime_IFSE, prime_PFR, Paie_I, Paie_PFR, Lignes_PFR, verbeux = afficher.table.effectifs, echo = FALSE)
Lignes_IFSE     <<- résultat_IFSE$Lignes

#'   
afficher_tableau_cumuls("5.8.7", "IFSE/PFR", tableau_cumuls(résultat_IFSE))
#'      
#'      

conditionnel("Lien vers la base de données IFSE non tit", "Bases/Reglementation/IFSE.non.tit.csv")      
conditionnel("Lien vers la base de données IFSE non cat. A-B-C", "Bases/Reglementation/IFSE.non.catABC.csv")              
#'   
#'    
#'   
conditionnel("Lien vers la base de données agrégat IFSE-PFR", "Bases/Remunerations/beneficiaires.IFSE.PFR.csv")    
#'    
#'   
conditionnel("Lien vers la base de données variations agrégat IFSE-PFR", "Bases/Remunerations/beneficiaires.IFSE.PFR.Variation.csv")    
#'   

rm(résultat_IFSE)  


