
# prime$nom
# prime$catégorie
# prime$restreint_fonctionnaire
# prime$prime_B  : prime contre laquelle comparer A
# si non null Paie_B doit avoir indic_B


test <- function(prime, Paie_I, Paie_B = NULL, Lignes_B = NULL, verbeux = FALSE) {

# Initialisation
  
Paie_A <- NULL 
Lignes_A <- NULL 
personnels.A.B <- NULL 
A.non.cat <- NULL 
nombre.mois.cumuls <- 0 
nombre.agents.cumulant.A.B <- 0
  
ident_prime <- prime$nom

# prime A testée contre prime B
  
résultat.manquant <- FALSE
nombre.agents.cumulant.A.B <- 0

essayer({
          Paie_A   <- filtrer_Paie(ident_prime, portée = "Mois", Base = Paie_I, indic = TRUE)
          Lignes_A <- Paie_A[indic == TRUE
                            ][ , indic := NULL]
}, 
"Le filtrage des idemnités " %+% ident_prime %+% " n'a pas pu être réalisé. ")

# Questions de compatibilité statutaire

essayer({
  
if (prime$restreint_fonctionnaire) {
   
   A.non.tit  <- Lignes_A[Statut != "TITULAIRE" & Statut != "STAGIAIRE"]
  
   if ((N.A.non.tit <<- uniqueN(A.non.tit$Matricule)) > 0) {
     
     cat(N.A.non.tit, "attributaire" %s% N.A.non.tit, ident_prime, "sont des non-titulaires. ")
     if (verbeux) print(kable(A.non.tit, align = 'r', row.names = FALSE))
     
   } else {
     
     cat("Tous les attributaires de", ident_prime, "sont titulaires ou stagiaires. ")
   }
}

if (prime$catégorie != ""){
  
  if (prime$expr.rég != "") {
    
    A.non.cat <- Lignes_A[! Catégorie %chin% prime$catégorie | ! grepl(prime$expr.rég, Grade, ignore.case = TRUE, perl = TRUE)] 
    
  } else {
    
    A.non.cat <- Lignes_A[! Catégorie %chin% prime$catégorie]
  }
  
  if ((N.A.non.cat <<- uniqueN(A.non.cat$Matricule)) > 0) {
    
    cat(N.A.non.cat, "attributaires de", ident_prime, "ne sont pas identifiés en catégorie", prime$catégorie,". ")
    if (verbeux)  print(kable(A.non.cat, align = 'r', row.names = FALSE))
    
  } else {
    
    cat("Tous les attributaires de", ident_prime, "sont identifiés en catégorie", prime$catégorie, ". ")
  }
  
} else {
  
  if (prime$expr.rég != "") {
    
    A.non.cat <- Lignes_A[! grepl(prime$expr.rég, Grade, ignore.case = TRUE, perl = TRUE)] 
    if ((N.A.non.cat <<- uniqueN(A.non.cat$Matricule)) > 0) {
      
      cat(N.A.non.cat, "attributaires de", ident_prime, "ne sont pas identifiés comme relevant de grades conformes. ")
      
      if (verbeux)  print(kable(A.non.cat, align = 'r', row.names = FALSE))
      
    }
  
  } else {
    
    cat("La détection des incompatibilités statutaires n'a pas pu être réalisée. ")
  }
}
}, "La détection des incompatibilités statutaires n'a pas pu être réalisée. ")


essayer({
  
  
K <- "codes." %+% tolower(ident_prime)

if (exists(K) && is.na(get(K))) {
  
  assign(K, list(K = unique(Lignes_A$Code)))
  
  L <- length(get(K))
  
  if (L == 0) {
    cat("Il n'a pas été possible d'identifier", 
        ident_prime,
        "par méthode heuristique. Renseigner les codes de paye correspondants dans l'interface graphique. ")
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
                        ][ , indic_B := NULL
                             , with = FALSE]
    
    nombre.mois.cumuls <- uniqueN(personnels.A.B[ , .(Matricule, Année, Mois)], by = NULL)
    
    nombre.agents.cumulant.A.B <- uniqueN(personnels.A.B$Matricule)
    
    setkey(personnels.A.B, Matricule,Année, Mois)
  
}
},
  "La détection des cumuls d'indemnités " %+% ident_prime %+% " et " %+% prime$prime_B %+% " n'a pas pu être réalisée. ")


essayer({
  
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
}, "Le tableau des cumuls ne peut pas être généré. ")

env <- environment()

sauvebase <- function(x, y, z) {
  Sauv.base(file.path(chemin.dossier.bases, z),
  x,
  y,
  FALSE,
  sep = ";",
  dec = ",",
  environment = env)
}

essayer({
  sauvebase("personnels.A.B", "personnels." %+% tolower(ident_prime) %+% "." %+% tolower(prime$prime_B), prime$dossier)
  sauvebase("A.non.cat", ident_prime %+% ".non.cat" %+% paste0("", prime$catégorie, collapse = ""), prime$dossier)
  sauvebase("A.non.tit", ident_prime %+% ".non.tit", prime$dossier)
}, "Pas de sauvegarde des fichiers auxiliaires.")

Lignes_A[ , A_ := TRUE]

beneficiaires.A <- merge(Lignes_A, Lignes_B, all = TRUE)

beneficiaires.A[ , Régime := if (all(is.na(A_))) { if (any(get(prime$prime_B))) "I" else NA } else { if (all(is.na(get(prime$prime_B)))) "P" else "C" },
                   by = .(Matricule, Année, Mois)][ , `:=`(A_ = NULL, 
                                                           B_ = NULL)]

matricules.A <- unique(Lignes_A$Matricule)

beneficiaires.A <- beneficiaires.A[Matricule %chin% matricules.A,
                                       .(Agrégat = sum(Montant, na.rm = TRUE),
                                         c = uniqueN(Mois[Régime == "C"]),
                                         nb.mois = uniqueN(Mois),
                                         Grade = Grade[1],
                                         Mois,
                                         Régime),
                                       keyby= .(Matricule, Année),
                                  ][ , 
                                      .(Agrégat,
                                        c,
                                        Grade,
                                        nb.mois,
                                        Régime = {
                                       
                                       prime$prime_B %+% " " %+% uniqueN(Mois[Régime == "I"]) %+% " mois-" %+% ident_prime %+% " " %+% uniqueN(Mois[Régime == "P"]) %+% " mois" %+% "-Cumul " %+% c %+% " mois"
                                     }),
                                      keyby= .(Matricule, Année)]

beneficiaires.A <- unique(beneficiaires.A)

beneficiaires.A.Variation <- beneficiaires.A[ , 
                                                { 
                                                  L <- length(Année)
                                                  q <- Agrégat[L]/Agrégat[1] * nb.mois[1]/nb.mois[L]                   
                                                  .(Années = paste(Année, collapse = ", "), 
                                                    `Variation (%)` = round((q - 1) * 100, 1),
                                                    `Moyenne géométrique annuelle(%)` = round((q^(1/(L - 1)) - 1) * 100, 1)) 
                                                }, by = "Matricule"]

beneficiaires.A.Variation <- beneficiaires.A.Variation[`Variation (%)` != 0.00]

essayer({
  sauvebase("beneficiaires.A", "beneficiaires." %+% ident_prime %+% "." %+% prime$prime_B, "Remunerations")
  sauvebase("beneficiaires.A.Variation", "beneficiaires." %+% ident_prime %+% "." %+% prime$prime_B %+% ".Variation", "Remunerations")
}, "Pas de sauvegarde des fichiers auxiliaires. ")

list(Paie = Paie_A, 
     Lignes = Lignes_A, 
     personnels = personnels.A.B, 
     non.cat = A.non.cat, 
     mois = nombre.mois.cumuls, 
     N = nombre.agents.cumulant.A.B,
     cumuls = beneficiaires.A,
     variations = beneficiaires.A.Variation,
     matricules = matricules.A)
}

tableau_bénéficiaires <- function(résultats) {

  essayer({
  res <- résultats$cumuls[, .(Matricule, Année, nb.mois, Grade, Agrégat)]
  
  if (afficher.table.effectifs) {
    if (nrow(res)) {
      
      res$Agrégat <- formatC(res$Agrégat, big.mark = " ", format="fg")
      
      kable(res, align = 'r', row.names = FALSE)
      
    } else {
      cat("\nAucun bénéficiaire détecté.\n")
    }
  }
  }, "Impossible d'afficher le tableau des bénéficiaires. ")
  
}

tableau_bénéficiaires_variation <- function(résultats) {
  
  essayer({
  res <- résultats$variations
  
  if (afficher.table.effectifs) {
    if (nrow(res)) {
      
      kable(res, align = 'r', row.names = FALSE)
      
    } else {
      
      cat("\nAucun tableau de variation.\n")
    }
  }
  }, "Impossible d'afficher le tableau des variations. ")
  
}


tableau_cumuls <- function(résultats) {
  
  res <- résultats$cumuls
  
  if (nrow(res[c != 0])) {
    
    kable(res[c != 0, .(Matricule, Année, Grade, Régime)])
    
  } else cat("Pas de cumuls.")
}

