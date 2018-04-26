
# prime$nom
# prime$catégorie
# prime$restreint_fonctionnaire
# prime$prime_B  : prime contre laquelle comparer A
# si non null Paie_B doit avoir indic_B


test <- function(prime, Paie_I, Paie_B = NULL, verbeux) {

ident_prime <- prime$nom

# prime A testée contre prime B
  
résultat.manquant <- FALSE
nombre.agents.cumulant.A.B <- 0

Paie_A   <- filtrer_Paie(ident_prime, portée = "Mois", Base = Paie_I, indic = TRUE)
Lignes_A <- Paie_A[indic == TRUE][ , indic := NULL]

if (prime$restreint_fonctionnaire) {
   
   A.non.tit  <- Lignes_A[Statut != "TITULAIRE" & Statut != "STAGIAIRE"]
  
   if ((N.A.non.tit <<- uniqueN(A.non.tit$Matricule)) > 0) {
     
     cat(N.A.non.tit, "attributaire" %s% N.A.non.tit, ident_prime, "sont des non-titulaires.")
     if (verbeux) print(kable(A.non.tit, align = 'r', row.names = FALSE))
     
   } else {
     
     cat("Tous les attributaires de", ident_prime, "sont titulaires ou stagiaires.")
   }
}

if (prime$catégorie != ""){
   
  A.non.cat <- Lignes_A[Catégorie != prime$catégorie]
  
  if ((N.A.non.cat <<- uniqueN(A.non.cat$Matricule)) > 0) {
    
    cat(N.A.non.cat, "attributaires de", ident_prime, "ne sont pas identifiés en catégorie", prime$catégorie)
    if (verbeux)  print(kable(A.non.cat, align = 'r', row.names = FALSE))
    
  } else {
    
    cat("Tous les attributaires de", ident_prime, "sont identifiés en catégorie", prime$catégorie)
  }
}

K <- "codes." %+% tolower(ident_prime)

if (exists(K) && is.na(get(K))) {
  
  assign(K, list(K = unique(Lignes_A$Code)))

  L <- length(get(K))
  
  if (L == 0) {
    cat("Il n'a pas été possible d'identifier", ident_prime, "par méthode heuristique. Renseigner les codes de paye correspondants dans l'interface graphique.")
    résultat.manquant <- TRUE
  }
}


if (! is.null(Paie_B) && ! résultat.manquant) {
  
  # on exclut les rappels !
  
  indic_B <- "indic_"  %+% prime$prime_B
  
  période.fusion <- merge(Paie_A[indic == TRUE],
                          Paie_B[get(indic_B) == TRUE],
                          by = c("Nom", "Prénom", "Matricule",
                                 "Année", "Mois", "Emploi", "Grade",
                                 "Indice", "Statut",
                                 "Catégorie"))[ , .(Matricule, Année, Mois)]

  période.fusion <- unique(période.fusion)
  
  A_ <- merge(Paie_A, période.fusion)
  B_ <- merge(Paie_B, période.fusion)
  B_$indic <- A_$indic
  
  personnels.A.B <- B_[indic == TRUE | get(indic_B) == TRUE
                      ][ , indic := NULL
                      ][ , indic_B := NULL, with = FALSE]
  
  nombre.mois.cumuls <- uniqueN(personnels.A.B[ , .(Matricule, Année, Mois)], by = NULL)
  
  nombre.agents.cumulant.A.B <- uniqueN(personnels.A.B$Matricule)
  
  personnels.A.B <- personnels.A.B[order(Année, Mois, Matricule)]
}

L <- length(get(K))

if (L < 6) {
  
  print(Tableau(c("Codes " %+% ident_prime,
                  "Agents cumulant " %+% ident_prime %+% " et " %+% prime$prime_B),
          sep.milliers = "",
          paste(unlist(get(K)), collapse = " "),
          nombre.agents.cumulant.A.B))
  
} else {
  
  cat("Codes " %+% ident_prime %+% " : ", paste(unlist(get(K)), collapse = " "))
  
}

env <- environment()

sauvebase <- function(x, y) {
  Sauv.base(file.path(chemin.dossier.bases, "Reglementation"),
  x,
  y,
  FALSE,
  sep = ";",
  dec = ",",
  environment = env)
}

sauvebase("personnels.A.B" , "personnels.pfr.ifts")
sauvebase("A.non.cat" , "PFR.non.catA")
sauvebase("A.non.tit" , "PFR.non.tit")

list(Paie = Paie_A, 
     Lignes = Lignes_A, 
     personnels = personnels.A.B, 
     non.cat = A.non.cat, 
     mois = nombre.mois.cumuls, 
     N = nombre.agents.cumulant.A.B)

}
