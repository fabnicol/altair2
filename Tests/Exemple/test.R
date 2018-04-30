
# prime$nom
# prime$catégorie
# prime$restreint_fonctionnaire
# prime$prime_B  : prime contre laquelle comparer A
# si non null Paie_B doit avoir indic_B

sauvebase <- function(x, y, z, env) {
  Sauv.base(file.path(chemin.dossier.bases, z),
            x,
            y,
            FALSE,
            sep = ";",
            dec = ",",
            environment = env)
}

test2 <- function(prime, prime_B, Paie_I, verbeux = FALSE) {

  Paie_B <- NULL 
  Lignes_B <- NULL 
  résultat <- analyser(prime_B, Paie_I, verbeux)
  test(prime, Paie_I, résultat$Paye, résultat$Lignes, verbeux)
}


tableau_cumuls <- function(résultat) {
  
  if (nrow(résultat$cumuls[c != 0])) {
    kable(résultat$cumuls[c != 0, .(Matricule, Année, Grade, Régime)])
  } else cat("Pas de cumuls.")
  
}

agrégat_annuel<- function(résultat, verbeux) {
  
  beneficiaires <- résultat$cumuls[, .(Matricule, Année, nb.mois, Grade, Agrégat)]
  
  if (verbeux) {
    
    if (nrow(beneficiaires)) {
      
      beneficiaires$Agrégat <- formatC(beneficiaires$Agrégat, big.mark = " ", format="fg")
      
      kable(beneficiaires, align = 'r', row.names = FALSE)
      
    } else {
      cat("\nAucun bénéficiaire détecté.\n")
    }
  }
  
}


évolution_agrégat <- function(résultat, verbeux) {
  
  if (verbeux) {
    
    if (nrow(résultat$variations)) {
      
      kable(résultat$variations, align = 'r', row.names = FALSE)
      
    } else {
      cat("\nAucun tableau de variation.\n")
    }
  }
}


analyser <- function(prime, Paie_I,verbeux) {
  
  Paie_A <- NULL
  Lignes_A <- NULL
  résultat.manquant <- FALSE
  lignes.indice.anormal <- NULL
  
  essayer({
    
    Paie_A   <- filtrer_Paie(prime$nom, 
                             portée = "Mois",
                             Base = Paie_I,
                             indic = TRUE)
    
    Lignes_A <- Paie_A[indic == TRUE
                      ][ , indic := NULL]
  }, 
    "Le filtrage des idemnités " %+% prime$nom %+% " n'a pas pu être réalisé. ")
  
  if (! is.null(prime$indice)) {
    
    essayer({
      
      lignes.indice.anormal <- if (prime$indice[1] == "+"){
        
        Lignes_A[Indice < prime$indice[2]] } else  { Lignes_A[Indice >= prime$indice[2]]
      } 
    
      if (length(prime$indice) >= 3 && prime$indice[3] %chin% c("A", "B", "C")) {
        
        lignes.indice.anormal <-lignes.indice.anormal[Catégorie %chin% prime$indice[3]]
          
      } 
          
      nr <- nrow(lignes.indice.anormal)
      
      if (! is.null(nr) && nr > 0) {
        cat(nr,
            "attributaires de",
            prime$nom,
            "ne satisfont pas au critère de la borne indiciaire (INM", prime$indice[2] %+% "). " )
      } else {
        cat("Les attributaires de",
            prime$nom,
            "satisfont tous au critère de la borne indiciaire (INM", prime$indice[2] %+% "). ") 
      }
      
    }, 
      "La vérification de la borne indiciaire n'a pas pu êre réalisée. ")
  }
  
  # Questions de compatibilité statutaire
  
  essayer({
    
    if (prime$restreint_fonctionnaire) {
      
      A.non.tit  <- Lignes_A[Statut != "TITULAIRE" & Statut != "STAGIAIRE"]
      
      if ((N.A.non.tit <- uniqueN(A.non.tit$Matricule)) > 0) {
        
        cat(N.A.non.tit,
            "attributaire" %s% N.A.non.tit,
            prime$nom,
            "sont des non-titulaires. ")
        
        if (verbeux) print(kable(A.non.tit,
                                 align = 'r',
                                 row.names = FALSE))
        
      } else {
        
        cat("Tous les attributaires de",
            prime$nom,
            "sont titulaires ou stagiaires. ")
      }
    }
    
    if (!is.null(prime$catégorie)){
      
      if (! is.null(prime$expr.rég)) {
        
        A.non.cat <- Lignes_A[! Catégorie %chin% prime$catégorie 
                              | ! grepl(prime$expr.rég, Grade, ignore.case = TRUE, perl = TRUE)] 
        
      } else {
        
        A.non.cat <- Lignes_A[! Catégorie %chin% prime$catégorie]
      }
      
      if ((N.A.non.cat <<- uniqueN(A.non.cat$Matricule)) > 0) {
        
        cat(N.A.non.cat, 
            "attributaires de",
            prime$nom,
            "ne sont pas identifiés en catégorie",
            prime$catégorie,
            ". ")
        
        if (verbeux)  print(kable(A.non.cat, align = 'r', row.names = FALSE))
        
      } else {
        
        cat("Tous les attributaires de",
            prime$nom,
            "sont identifiés en catégorie",
            prime$catégorie, ". ")
      }
      
    } else {
      
      if (! is.null(prime$expr.rég)) {
        
        A.non.cat <- Lignes_A[! grepl(prime$expr.rég, Grade, ignore.case = TRUE, perl = TRUE)] 
        
        if ((N.A.non.cat <<- uniqueN(A.non.cat$Matricule)) > 0) {
          
          cat(N.A.non.cat,
              "attributaires de",
              prime$nom,
              "ne sont pas identifiés comme relevant de grades conformes. ")
          
          if (verbeux)  print(kable(A.non.cat, align = 'r', row.names = FALSE))
          
        }
        
      } else {
        A.non.cat <- NULL
        cat("La détection des incompatibilités statutaires n'a pas pu être réalisée. ")
      }
    }
  },
    "La détection des incompatibilités statutaires n'a pas pu être réalisée. ")
  
  essayer({
    
    K <- "codes." %+% tolower(prime$nom)
    
    assign(K, list(K = unique(Lignes_A$Code)))
      
    L <- length(get(K))
      
    if (L == 0) {
      cat("Il n'a pas été possible d'identifier", 
          prime$nom,
          "par méthode heuristique. Renseigner les codes de paye correspondants dans l'interface graphique. ")
 
      résultat.manquant <- TRUE

    }
    }, "Pas de génération des codes " %+% prime$nom %+% ". ")
  
  env <- environment()
  
  sauvebase("A.non.cat", prime$nom %+% ".non.cat" %+% paste0("", prime$catégorie, collapse = ""), prime$dossier, env)
  sauvebase("A.non.tit", prime$nom %+% ".non.tit", prime$dossier, env)
  if (! is.null(prime$indice)) {
    sauvebase("lignes.indice.anormal", prime$nom %+% ".indice.anormal", prime$dossier, env)
  }
  
  list(Paye = Paie_A, Lignes = Lignes_A, K = get(K), manquant = résultat.manquant)
}


test <- function(prime, Paie_I, Paie_B = NULL, Lignes_B = NULL, verbeux = FALSE) {

# Initialisation
  
Paie_A <- NULL 
Lignes_A <- NULL 
personnels.A.B <- NULL 
A.non.cat <- NULL 
lignes.indice.anormal <- NULL
nombre.mois.cumuls <- 0 
nombre.agents.cumulant.A.B <- 0
ident_prime <- prime$nom

nombre.agents.cumulant.A.B <- 0

résultat <- analyser(prime, Paie_I, verbeux)

Paie_A   <- résultat$Paye
Lignes_A <- résultat$Lignes
K        <- résultat$K
résultat.manquant <- résultat$manquant

essayer({  
if (! is.null(Paie_B) && ! résultat.manquant) {
  
  # on exclut les rappels !
  
    indic_B <- "indic_"  %+% prime$prime_B
    
    NAMES <- names(Paie_B)
    
    if (! indic_B %chin% NAMES && "indic" %chin% NAMES) setnames(Paie_B, "indic", indic_B)
    
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
  
  L <- length(K)
    
  if (L < 6) {
    
    print(Tableau(c("Codes " %+% ident_prime,
                    "Agents cumulant " %+% ident_prime %+% " et " %+% prime$prime_B),
            sep.milliers = "",
            paste(unlist(K), collapse = " "),
            nombre.agents.cumulant.A.B))
    
  } else {
    
    cat("Codes " %+% ident_prime %+% " : ", paste(unlist(K), collapse = " "))
    
  }
}, "Le tableau des cumuls ne peut pas être généré. ")


essayer({
  sauvebase("personnels.A.B", "personnels." %+% tolower(ident_prime) %+% "." %+% tolower(prime$prime_B), prime$dossier, environment())
  
}, "Pas de sauvegarde des fichiers auxiliaires.")

indic <- "indic_"  %+% prime$nom

Lignes_A[ , indic := TRUE, with = FALSE]
Lignes_B[ , indic_B := TRUE, with = FALSE]

beneficiaires.A <- merge(Lignes_A, Lignes_B, all = TRUE)

beneficiaires.A[ , Régime := if (all(is.na(get(indic)))) { if (any(get(indic_B))) "I" else NA } else { if (all(is.na(get(indic_B)))) "P" else "C" },
                   by = .(Matricule, Année, Mois)][ , indic := NULL, with = FALSE][ , indic_B := NULL, with = FALSE]

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

env <- environment()

essayer({
  sauvebase("beneficiaires.A", "beneficiaires." %+% ident_prime %+% "." %+% prime$prime_B, "Remunerations", env)
  sauvebase("beneficiaires.A.Variation", "beneficiaires." %+% ident_prime %+% "." %+% prime$prime_B %+% ".Variation", "Remunerations", env)
}, "Pas de sauvegarde des fichiers auxiliaires. ")

list(Paie = Paie_A, 
     Lignes = Lignes_A, 
     Paie_B = Paie_B,
     Lignes_B = Lignes_B,
     personnels = personnels.A.B, 
     non.cat = A.non.cat, 
     mois = nombre.mois.cumuls, 
     N = nombre.agents.cumulant.A.B,
     cumuls = beneficiaires.A,
     variations = beneficiaires.A.Variation,
     matricules = matricules.A,
     indices = lignes.indice.anormal,
     manquant = résultat.manquant)
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

