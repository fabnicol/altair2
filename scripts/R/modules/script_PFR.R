#'
#'## 5.5 Contrôle de la prime de fonctions et de résultats (PFR) 
#'   
#'  
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_PFR.odt)   
#'  
#+ pfr


PFR.plafonds <<- list( admin.g   = 58800, 
                       admin.hc  = 55200,
                       admin     = 49800,
                       attaché.p = 25800,
                       attaché   = 20100)

résultat_PFR  <- test_prime(prime_PFR, prime_IFTS, Paie_I, Paie_IFTS, Lignes_IFTS, verbeux = afficher.table.effectifs, echo = TRUE)

Paie_PFR   <<- résultat_PFR$Paie
Lignes_PFR <<- résultat_PFR$Lignes

#'   
afficher_tableau_cumuls("5.5.1", "PFR/IFTS", tableau_cumuls(résultat_PFR))
#'  
#'      
conditionnel("Lien vers la base de données cumuls pfr/ifts", "Bases/Reglementation/personnels.pfr.ifts.csv")    
conditionnel("Lien vers la base de données PFR non cat.A", "Bases/Reglementation/PFR.non.catA.csv")      
conditionnel("Lien vers la base de données PFR non tit", "Bases/Reglementation/PFR.non.tit.csv")       
#'   

résultat_PFR <<- test_prime(prime_PFR, prime_ISS, Paie_I, verbeux = afficher.table.effectifs, echo = FALSE)

Paie_ISS   <<- résultat_PFR$Paie
Lignes_ISS <<- résultat_PFR$Lignes

#'   
afficher_tableau_cumuls("5.5.2", "PFR/ISS", tableau_cumuls(résultat_PFR))
#'  
#'      
conditionnel("Lien vers la base de données cumuls pfr/iss", "Bases/Reglementation/personnels.pfr.iss.csv")    
#'   

résultat_PFR   <- test_prime(prime_PFR, prime_IEMP, Paie_I, verbeux = afficher.table.effectifs, echo = FALSE)

Paie_IEMP   <<- résultat_PFR$Paie
Lignes_IEMP <<- résultat_PFR$Lignes

#'   
afficher_tableau_cumuls("5.5.3", "PFR/IEMP", tableau_cumuls(résultat_PFR))
#'  
#'      
conditionnel("Lien vers la base de données cumuls pfr/iemp", "Bases/Reglementation/personnels.pfr.iemp.csv")    
#'   


# Plafonds annuels (plafonds mensuels reste à implémenter)
# AG 58 800
# ADTHC 55 200
# ADT   49 800
# D/ATP 25 800
# SM/AT 20 100

#'  
#'&nbsp;*Tableau 5.5.4 : Rappel des plafonds annuels de la PFR*
#'      

Tableau(c("Adm. général", "Adm. HC", "Adm.", "Direct./Attaché princ.", "Secr. mairie/Attaché"),
        sapply(PFR.plafonds, 
               function(x) formatC(x, format = "fg", big.mark = " ")))

e <- c(expression.rég.admin.g, expression.rég.admin.hc, expression.rég.admin, expression.rég.attaché.p, expression.rég.attaché)

test.PFR <- function(i, grade, cumul) { 
  
  grepl(e[i], grade, perl = TRUE, ignore.case = TRUE) & (cumul > PFR.plafonds[[i]]) 
}

test.PFR.all <- function(grade, cumul) any(sapply(1:length(e), function(i) test.PFR(i, grade, cumul)))

cumuls.PFR <- résultat_PFR$Lignes[, .(PFR_annuel = sum(Montant, na.rm = TRUE),
                                      nb.mois = uniqueN(Mois),
                                      Grade = Grade[1]),
                                    by = .(Matricule,Annee)
                                 ][ , PFR_annuel := PFR_annuel * 12 / nb.mois]   # proratisation mensuelle

dépassements.PFR.boolean <- mapply(test.PFR.all, cumuls.PFR$Grade, cumuls.PFR$PFR_annuel, USE.NAMES=FALSE)

dépassements.PFR.plafonds <- data.frame()

if (length(dépassements.PFR.boolean) > 0)
  dépassements.PFR.plafonds <- cumuls.PFR[dépassements.PFR.boolean]

if (nrow(dépassements.PFR.plafonds) > 0) {
  
  cat("\nLes plafonds annuels de la PFR sont dépassés pour ", nrow(dépassements.PFR.plafonds), " cumuls annuels.\n")
  kable(dépassements.PFR.plafonds, align = 'r', row.names = FALSE, format = "simple")
  
} else {
  
  cat("\nLes plafonds annuels de la PFR ne sont pas dépassés.\n")
}

#'  
#'&nbsp;*Tableau 5.5.5 : Valeurs de l'agrégat annuel (PFR ou IFTS) pour les bénéficiaires de la PFR*
#'          

agrégat_annuel(résultat_PFR, afficher.table.effectifs)  

#'   
conditionnel("Lien vers la base de données agrégat PFR-IFTS", "Bases/Remunerations/beneficiaires.PFR.IFTS.csv")    
#'    

#'  
#'&nbsp;*Tableau 5.5.6 : Variations de l'agrégat mensuel moyen (PFR ou IFTS) pour les bénéficiaires de la PFR*
#'          

évolution_agrégat(résultat_PFR, afficher.table.effectifs)

#'   
conditionnel("Lien vers la base de données variations agrégat PFR-IFTS", "Bases/Remunerations/beneficiaires.PFR.IFTS.Variation.csv")    
#'   

rm(résultat_PFR)  
