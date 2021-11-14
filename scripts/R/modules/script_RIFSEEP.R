
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

résultat_IFSE   <- test_prime(prime_IFSE, prime_IFTS, Paie_I, Paie_IFTS, Lignes_IFTS, verbeux = afficher.table.effectifs)

#'   
  
if (! is.null(résultat_IFSE)) {
  
  e <- new.env()
  e <- tableau_cumuls(résultat_IFSE, e)
  
  if (e$res) {
      cat("&nbsp;*Tableau 5.8.1 : Cumul IFSE/IFTS*    ", "\n")

      print(e$tableau)
  }
}

#'      
conditionnel("Lien vers la base de données cumuls ifse/ifts", "Bases/Reglementation/personnels.ifse.ifts.csv")    
conditionnel("Lien vers la base de données IFSE non tit", "Bases/Reglementation/IFSE.non.tit.csv")      
conditionnel("Lien vers la base de données IFSE non cat. A-B-C", "Bases/Reglementation/IFSE.non.catABC.csv")              
#'   

#'    

if (! exists("Paie_IAT")) Paie_IAT <- NULL

résultat_IFSE   <- test_prime(prime_IFSE, prime_IAT, Paie_I, Paie_IAT, Lignes_IAT, verbeux = afficher.table.effectifs, echo = FALSE)

if (! is.null(résultat_IFSE)) {
  
  e <- new.env()
  e <- tableau_cumuls(résultat_IFSE, e)
  
  if (e$res) {
    cat("&nbsp;*Tableau 5.8.2 : Cumul IFSE/IAT*   ", "\n")
    print(e$tableau)
  }
}

#'      
#'      
conditionnel("Lien vers la base de données cumuls ifse/ifts", "Bases/Reglementation/personnels.ifse.iat.csv")    
#'   

if (! exists("Paie_ISS")) Paie_ISS <- NULL

résultat_IFSE   <- test_prime(prime_IFSE, prime_ISS, Paie_I, verbeux = afficher.table.effectifs, echo = FALSE)

#'   

if (! is.null(résultat_IFSE)) {
  
  e <- new.env()
  e <- tableau_cumuls(résultat_IFSE, e)
  
  if (e$res) {
    cat("&nbsp;*Tableau 5.8.3 : Cumul IFSE/ISS*   ", "\n")
    print(e$tableau)
  }
}

#'      
#'      
conditionnel("Lien vers la base de données cumuls ifse/iss", "Bases/Reglementation/personnels.ifse.iss.csv")    
#'   

if (! exists("Paie_IEMP")) Paie_IEMP <- NULL

résultat_IFSE   <- test_prime(prime_IFSE, prime_IEMP, Paie_I, verbeux = afficher.table.effectifs, echo = FALSE)

#'   

if (! is.null(résultat_IFSE)) {
  
  e <- new.env()
  e <- tableau_cumuls(résultat_IFSE, e)
  
  if (e$res) {
    cat("&nbsp;*Tableau 5.8.4 : Cumul IFSE/IEMP*   ", "\n")
    print(e$tableau)
  }
}

#'      
#'      
conditionnel("Lien vers la base de données cumuls ifse/iemp", "Bases/Reglementation/personnels.ifse.iemp.csv")    
#'   

if (! exists("Paie_PFI")) Paie_PFI <- NULL

résultat_IFSE   <- test_prime(prime_IFSE, prime_PFI, Paie_I, verbeux = afficher.table.effectifs, echo = FALSE)

#'    

if (! is.null(résultat_IFSE)) {
  
  e <- new.env()
  e <- tableau_cumuls(résultat_IFSE, e)
  
  if (e$res) {
    cat("&nbsp;*Tableau 5.8.5 : Cumul IFSE/PFI*   ", "\n")
    print(e$tableau)
  }
}

#'      
conditionnel("Lien vers la base de données cumuls ifse/pfi", "Bases/Reglementation/personnels.ifse.pfi.csv")    
#'   

if (! exists("Paie_PSR")) Paie_PSR <- NULL

résultat_IFSE   <- test_prime(prime_IFSE, prime_PSR, Paie_I, Paie_PSR, Lignes_PSR, verbeux = afficher.table.effectifs, echo = FALSE)

if (! is.null(résultat_IFSE)) {
  
  e <- new.env()
  e <- tableau_cumuls(résultat_IFSE, e)
  
  if (e$res) {
    cat("&nbsp;*Tableau 5.8.6 : Cumul IFSE/PSR*   ", "\n")
    print(e$tableau)
  }
}

#'      
#'      
conditionnel("Lien vers la base de données cumuls ifse/psr", "Bases/Reglementation/personnels.ifse.psr.csv")    
#'   

if (! exists("Paie_PFR")) Paie_PFR <- NULL

résultat_IFSE   <- test_prime(prime_IFSE, prime_PFR, Paie_I, Paie_PFR, Lignes_PFR, verbeux = afficher.table.effectifs, echo = FALSE)
Lignes_IFSE     <<- résultat_IFSE$Lignes

#'   

if (! is.null(résultat_IFSE)) {
  
  e <- new.env()
  e <- tableau_cumuls(résultat_IFSE, e)
  
  if (e$res) {
    cat("&nbsp;*Tableau 5.8.7 : Cumul IFSE/PFR*   ", "\n")
    print(e$tableau)
  }
}

#'      
conditionnel("Lien vers la base de données cumuls ifse/ifts", "Bases/Reglementation/personnels.ifse.pfr.csv")    
#'   
#'  

if (afficher.table.effectifs) {

   cat("&nbsp;*Tableau 5.8.8 : Valeurs de l'agrégat annuel (IFSE ou PFR) pour les bénéficiaires de l'IFSE*  ")        

   agrégat_annuel(résultat_IFSE, verbeux = afficher.table.effectifs)

   cat("\n", 
   " &nbsp;*Tableau 5.8.9 : Variations de l'agrégat mensuel moyen (IFSE ou PFR) pour les bénéficiaires de l'IFSE*   ",
   "\n")

   évolution_agrégat(résultat_IFSE, verbeux = afficher.table.effectifs)
}

#'   
conditionnel("Lien vers la base de données agrégat IFSE-PFR", "Bases/Remunerations/beneficiaires.IFSE.PFR.csv")    
#'    
#'   
conditionnel("Lien vers la base de données variations agrégat IFSE-PFR", "Bases/Remunerations/beneficiaires.IFSE.PFR.Variation.csv")    
#'   

rm(résultat_IFSE)  


