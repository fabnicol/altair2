
#'
#'## `r chapitre`.10 Contrôle du RIFSEEP (IFSE)  
#'   
#'*Pour tirer pleinement profit de ces fonctionnalités, il est préférable de faire remplir, par les organismes contrôlés le tableau CSV accessible dans le bloc* **IFSE** *de l'onglet Extra de l'application graphique, ou bien à ce lien. Voir aussi la notice* &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_tableau_ifse.odt)      
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


résultat_IFSE   <- test_prime(prime_IFSE, prime_IFTS, Paie_I, Paie_IFTS, Lignes_IFTS, afficher.table.effectifs)

#'   
#'    
#'&nbsp;*Tableau `r incrément()` : Cumul IFSE/IFTS*   
#'      

tableau_cumuls(résultat_IFSE)

#'      
#'[Lien vers la base de données cumuls ifse/ifts](Bases/Reglementation/personnels.ifse.ifts.csv)    
#'[Lien vers la base de données IFSE non tit](Bases/Reglementation/IFSE.non.tit.csv)      
#'[Lien vers la base de données IFSE non cat. A-B-C](Bases/Reglementation/IFSE.non.catABC.csv)              
#'   

#'    
#'&nbsp;*Tableau `r incrément()` : Cumul IFSE/IAT*   
#'      

résultat_IFSE   <- test_prime(prime_IFSE, prime_IAT, Paie_I, Paie_IAT, Lignes_IAT, afficher.table.effectifs)

tableau_cumuls(résultat_IFSE)

#'      
#'      
#'[Lien vers la base de données cumuls ifse/ifts](Bases/Reglementation/personnels.ifse.iat.csv)    
#'   


résultat_IFSE   <- test_prime(prime_IFSE, prime_ISS, Paie_I, verbeux = afficher.table.effectifs)

#'   
#'    
#'&nbsp;*Tableau `r incrément()` : Cumul IFSE/ISS*   
#'      

tableau_cumuls(résultat_IFSE)

#'      
#'      
#'[Lien vers la base de données cumuls ifse/iss](Bases/Reglementation/personnels.ifse.iss.csv)    
#'   

résultat_IFSE   <- test_prime(prime_IFSE, prime_IEMP, Paie_I, verbeux = afficher.table.effectifs)

#'   
#'    
#'&nbsp;*Tableau `r incrément()` : Cumul IFSE/IEMP*   
#'      

tableau_cumuls(résultat_IFSE)

#'      
#'      
#'[Lien vers la base de données cumuls ifse/iemp](Bases/Reglementation/personnels.ifse.iemp.csv)    
#'   

résultat_IFSE   <- test_prime(prime_IFSE, prime_PFI, Paie_I, verbeux = afficher.table.effectifs)

#'   
#'    
#'&nbsp;*Tableau `r incrément()` : Cumul IFSE/PFI*   
#'      

tableau_cumuls(résultat_IFSE)

#'      
#'      
#'[Lien vers la base de données cumuls ifse/pfi](Bases/Reglementation/personnels.ifse.pfi.csv)    
#'   

résultat_IFSE   <- test_prime(prime_IFSE, prime_PSR, Paie_I, Paie_PSR, Lignes_PSR, afficher.table.effectifs)

#'   
#'    
#'&nbsp;*Tableau `r incrément()` : Cumul IFSE/PSR*   
#'      

tableau_cumuls(résultat_IFSE)

#'      
#'      
#'[Lien vers la base de données cumuls ifse/psr](Bases/Reglementation/personnels.ifse.psr.csv)    
#'   


résultat_IFSE   <- test_prime(prime_IFSE, prime_PFR, Paie_I, Paie_PFR, Lignes_PFR, afficher.table.effectifs)
Lignes_IFSE     <- résultat_IFSE$Lignes

#'   
#'    
#'&nbsp;*Tableau `r incrément()` : Cumul IFSE/PFR*   
#'      

tableau_cumuls(résultat_IFSE)

#'      
#'      
#'[Lien vers la base de données cumuls ifse/ifts](Bases/Reglementation/personnels.ifse.pfr.csv)    
#'   

depassements_IFSE <- test_plafonds(base.ifse, Lignes_IFSE, if (logements.existe) logements else NULL)

#'   
#'    
#'&nbsp;*Tableau `r incrément()` : Coûts des dépassements de plafond IFSE*   
#' 
if (plafonds.ifse.existe) {
  Tableau(as.character(période), depassements_IFSE)
} else {
  cat("Pas d'analyse des plafonds IFSE.")
}

#'      
#'[Lien vers les dépassements de plafond IFSE](Bases/Reglementation/bulletins.depassements.ifse.csv)    
#'   


#'  
#'&nbsp;*Tableau `r incrément()` : Valeurs de l'agrégat annuel (IFSE ou PFR) pour les bénéficiaires de l'IFSE*        
#'          

agrégat_annuel(résultat_IFSE, afficher.table.effectifs)

#'  
#'&nbsp;*Tableau `r incrément()` : Variations de l'agrégat mensuel moyen (IFSE ou PFR) pour les bénéficiaires de l'IFSE*   
#'          

évolution_agrégat(résultat_IFSE, afficher.table.effectifs)

#'   
#'[Lien vers la base de données agrégat IFSE-PFR](Bases/Remunerations/beneficiaires.IFSE.PFR.csv)    
#'    
#'   
#'[Lien vers la base de données variations agrégat IFSE-PFR](Bases/Remunerations/beneficiaires.IFSE.PFR.Variation.csv)    
#'   

rm(résultat_IFSE)  


