# --- Test Prime de fonctions informatiques
#     Filtre    : filtre expression rationnelle expression.rég.pfi dans Libellé.   

#'  
#'## `r chapitre`.2 Contrôle de la prime de fonctions informatiques (PFI)     
#'   
#'[![Notice](Notice.png)](Docs/Notices/fiche_PFI.odt)   
#'   
Matrice.PFI <- filtrer_Paie("PFI")

personnels.prime.informatique <- Matrice.PFI[ , ..colonnes]

if (nombre.personnels.pfi <- uniqueN(personnels.prime.informatique$Matricule)) {
  
  cat("Il existe ", 
      FR(nombre.personnels.pfi),
      "agent" %s% nombre.personnels.pfi,
      " percevant une PFI.")
}

primes.informatiques.potentielles <- if (nombre.personnels.pfi == 0) "aucune" else paste(unique(personnels.prime.informatique$Libellé), collpase = " ;")

#'Primes informatiques (PFI) : `r primes.informatiques.potentielles`    
#'  

#'   
#'[Lien vers la base de données Primes informatiques](Bases/Reglementation/personnels.prime.informatique.csv)   
#'     
