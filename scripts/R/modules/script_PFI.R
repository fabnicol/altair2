# --- Test Prime de fonctions informatiques
#     Filtre    : filtre expression rationnelle expression.rég.pfi dans Libelle.   

#'  
#'## 5.9 Contrôle de la prime de fonctions informatiques (PFI)     
#'   
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_PFI.odt)   
#'   
Matrice.PFI <- filtrer_Paie("PFI")

personnels.prime.informatique <- Matrice.PFI[ , ..colonnes]

if (nombre.personnels.pfi <- uniqueN(personnels.prime.informatique$Matricule)) {
  
  cat("Il existe ", 
      FR(nombre.personnels.pfi),
      "agent" %s% nombre.personnels.pfi,
      " percevant une PFI.")
}

if (nombre.personnels.pfi == 0) {
  primes.informatiques.potentielles <- "aucune"
  } else {
    primes.informatiques.potentielles <- paste(unique(personnels.prime.informatique$Libelle), collapse = " ;")
  }

sauv.bases("Reglementation", environment(), "personnels.prime.informatique")

cat(primes.informatiques.potentielles) 
#'  
#'  
conditionnel("Lien vers la base de données PFI", "Bases/Reglementation/personnels.prime.informatique.csv")   
#'  
