
#'   
#'## 5.17 Primes de la fonction publique hospitalière            
#'    
#'[![Notice](Notice.png)](Docs/Notices/fiche_FPH.odt)    
#'     
#'*Les primes qui suivent ne peuvent être octroyées qu'à des fontionnaires.*    
#'*Les tests portent sur les cas d'attribution à des non-titulaires (et autres statuts)*     
#'      

prime_FPH_test <- function(prime, prime_lit, base, expr = NULL) {
  
  if (VERSANT_FP != "FPH"){
    
    cat("Ce contrôle ne porte pas sur la FPH. Les liens hypertextes ci-dessous seront désactivés.   \n")
    cat("Pour activer le contrôle FPH, cocher la case correspondante de l'onglet **Options > Format** de l'interface graphique  \n")
    return("Non traité.")
  } 
  
  DT <- filtrer_Paie(prime)[ , ..colonnes]
  
  DT <- if (! is.null(expr)) {
    DT[Statut == "NON_TITULAIRE" | Statut == "AUTRE_STATUT" | grepl(expr, Grade, perl = TRUE, ignore.case = TRUE)]
  } else {
    DT[Statut == "NON_TITULAIRE" | Statut == "AUTRE_STATUT"]
  }
  
  if (nombre.personnels.nt <- uniqueN(DT$Matricule)) {
    
    cat("Il existe ", 
        FR(nombre.personnels.nt),
        "agent" %s% nombre.personnels.nt,
        "non titulaire" %s% nombre.personnels.nt, "percevant une", prime_lit)
  }
  
  if (nombre.personnels.nt) {
    
    cat("Coût des anomalies ", 
        DT[ , sum(Montant, na.rm = TRUE)], "euros.")
  }
  
  assign(base,  DT, .GlobalEnv)

  fwrite(get(base, .GlobalEnv), file.path(chemin.dossier.bases, "Reglementation", base %+% ".csv"))
  
  primes <- unique(DT$Libellé)
  
  primes.potentielles <- if (length(primes) == 0) "aucune" else paste(primes, collapse = " ;")
}

#'**Prime spécifique**   

# décret n°88-1083 du 30 novembre 1988

primes.potentielles <- prime_FPH_test("PRIME SPECIFIQUE", "prime spécifique", "personnels.prime.specifique.nt")

#'   
#'Primes spécifiques : `r primes.potentielles`      
#'   

#'   
conditionnel("Lien vers la base de données Prime spécifique NT", "Bases/Reglementation/personnels.prime.specifique.nt.csv")   
#'   

#'**Prime de technicité**   

# décret n°91-870 du 5 septembre 1991 

primes.potentielles <- prime_FPH_test("PRIME DE TECHNICITE", "prime de technicité", "personnels.prime.tech.nt")

#'   
#'Primes de technicité : `r primes.potentielles`    
#'   

#'   
conditionnel("Lien vers la base de données Prime de technicité NT", "Bases/Reglementation/personnels.prime.tech.nt.csv")   
#'   

#'**Indemnité forfaitaire et technique**   

# décret n°2013-102 du 29 janvier 2013  

primes.potentielles <- prime_FPH_test("IFT", "indemnité forfaitaire et technique", "personnels.ift.nt")

#'   
#'Indemnités forfaitaire et technique : `r primes.potentielles`    
#'   

#'   
conditionnel("Lien vers la base de données IFT NT", "Bases/Reglementation/personnels.ift.nt.csv")   
#'   

#'**Prime de service**   

primes.potentielles <- prime_FPH_test("PRIME DE SERVICE", "prime de service", "personnels.ps.nt", expression.rég.médecin)

#'   
#'Primes de service : `r primes.potentielles`    
#'   

#'   
conditionnel("Lien vers la base de données primes de service NT", "Bases/Reglementation/personnels.ps.nt.csv")   
#'   
