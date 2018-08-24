
#' @export


calculer_indice_complexité <- function() {
  
  
  multiplicité <- Paie[ , Statut2 := Statut
                      ][Statut %in% c("EMPLOI_FONCTIONNEL", "ELU", "AUTRE_STATUT"), Statut2 := "NON_TITULAIRE"
                      ][Statut == "STAGIAIRE", Statut2 := "TITULAIRE"
                      ][ , .(m = uniqueN(Statut2), Statut2, Année, Mois, Matricule, Grade, Emploi, Nb.Enfants, NBI, Brut, Montant),
                             by = .(Code, Type, Libellé)]
  
  if (n <- nrow(multiplicité[m > 1])) {
    
    cat("La clé d'appariement **Code, Libellé, Type** est insuffisante pour réaliser une jointure correcte entre la base de paye et le tableau de correspondance codes de paye - compte 64.  \n")
    cat("Au mieux, il ne serait possible que d'apparier", q <- (1 - round(n/nrow(Paie), 2)) * 100, " % des lignes de paye avec la comptabilité administrative.  \n")
    if (q < 10) {
      cat("Il est nécessaire d'utiliser une autre clé d'appariement, au minimum **Statut**, éventuellement **Grade** et **Nb.Enfant**") 
      NULL
    } else {
      
      cat("L'appariement peut être tenté avec **Code, Libellé, Type**, mais les résultats différeront de ", 100 - q , "% de la comptabilité administrative.")
      multiplicité[m == 1][ , c(m, Statut2) := NULL]
    }
    
  }
}

#' @export
exporter_tableau <- function(DT) {
  
  colonnes <- names(Paie)
  
  if (! "Compte" %chin% colonnes) {
    cat("Le tableau fourni par l'organisme (*paye_budget.csv*) doit contenir la colonne **Compte**")
    return(NULL)
  }
  
  res <- calculer_indice_complexité()
  
  if (is.null(res) && apparier.sur.trois.clés) {
    
    clés <- c("Code", "Libellé", "Type")
    
  } else {
    
    clés <- intersect(names(DT), colonnes)
  }
  
  if (length(setdiff(c("Code", "Libellé", "Type"), clés))) {
    
    cat("Les clés d'appariement doivent comporter au moins les colonnes **Code**, **Libellé** et **Type**  \n")
    return(NULL)
  } 
    
    test <- anyDuplicated(DT[ , ..clés])
    
  if (test > 0) {
    
    TabDupl <- duplicated(DT[ , ..clés])
    DT <- unique(DT[ , ..clés])
    cat("Le tableau fourni par l'organisme (*paye_budget.csv*) contient des clés dupliquées pour les variables suivantes : ", paste(clés, collapse = " "), "  \n")
    cat("L'opération d'appariement ne peut se faire sur ces clés. Elle se fera sur les autres clés au prix d'une perte de données financières.   \n
        Les agrégats seront donc inférieurs à ceux de la comptabilité.  \n")
    cat("Il est envisageable de récupérer les montants correspondants en examinant manuellement le tableau fourni en lien ci-dessous, correspondant aux clés suivantes:  \n")
    kable(TabDupl)
    # Insérer lien condiditionnel sur TabDupl dans le rapport.
    
  } else {
    
    cat("Le tableau fourni par l'organisme (*paye_budget.csv*) contient des clés d'appariement convenables.  \n")
  }
    
   "Paie" %a% DT[Paie, on = clés]
    
   Paie[ , .(Année, Code, Libellé, Statut, Type, Montant)]
  
}

#' @export
correspondance_paye_budget <- function() {

 essayer(label ="+comptabilité",
 {  
  "paye.budget.existe" %a%  file.exists(chemin("paye_budget.csv"))  
  
  vect <- c("Code", "Libellé", "Statut", "Type")
  
  if (paye.budget.existe){
    
    code.libelle <- fread(chemin("paye_budget.csv"), # Code, Libellé,  Statut, Type, Compte
                          sep = ";",
                          encoding   = "Latin-1",
                          col.names  = c(vect, "Compte"),
                          colClasses = c("character", "character", "character", "character", "character"))  
    
    message("*****")
    message("Importation de la table des codes et libellés par compte (paye_budget.csv)")
    message("*****")
    
    code.libelle <- résumer_type(code.libelle)
    
    code.libelle      <- unique(code.libelle)
    
    cumul.lignes.paie <- exporter_tableau(code.libelle)
    
  } else {
    
    # Ne pas prendre les capitales ni simplifier les libellés
    
    code.libelle <- unique(Paie[Montant != 0, .(Code, Libellé, Statut), by = "Type"])
    
    # Note : des traitements et NBI sont parfois improprement codés comme indemnités.
    
    code.libelle[Type %in% c("T", "I", "R", "AC") & grepl(expression.rég.traitement, Libellé, ignore.case = TRUE, perl = TRUE),
                 `:=`(Compte.tit    = "64111",
                      Compte.nontit = "64131")]
    
    code.libelle[Type == "IR" | Type == "S" | (Type %in% c("T", "I", "R") & grepl(expression.rég.nbi, Libellé, ignore.case = TRUE, perl = TRUE)),
                 `:=`(Compte.tit    = "64112",
                      Compte.nontit = "64132")]
    
    code.libelle[grepl("(?:ind|prim).*(?:pr[e,é]avis|licen)", Libellé, ignore.case = TRUE, perl = TRUE), 
                 `:=`(Compte.tit    = "64116",
                      Compte.nontit = "64136")]
    
    code.libelle[Statut == "EMPLOI_AIDE"
                 & Type %in% c("T", "I", "R", "AC"), 
                 `:=`(Compte.nontit = "64116")]
    
    code.libelle[is.na(Compte.tit) 
                 & Statut != "ELU"
                 & ! Type %in% c("D", "C", "RE", "CO") 
                 & (Type == "I" | grepl("(?:prim|indem)", Libellé, ignore.case = TRUE, perl = TRUE)), 
                 `:=`(Compte.tit    = "64118",
                      Compte.nontit = "64138")]
    
    code.libelle[ , Compte := ifelse(Statut == "TITULAIRE" | Statut == "STAGIAIRE", Compte.tit, Compte.nontit)
                  ][ , Compte.tit := NULL
                  ][ , Compte.nontit := NULL]
    
    cumul.lignes.paie <- code.libelle[Paie[ , .(Année, Code, Libellé, Statut, Type, Montant)], on = vect]
    
    
  }
  
  setkey(code.libelle, Type, Compte, Statut, Code, Libellé)
  
  cumul.lignes.paie[is.na(Compte) | Compte == "", Compte := "Autres"]
  
  cumul.lignes.paie <- cumul.lignes.paie[ , .(Total = sum(Montant, na.rm = TRUE)), keyby = .(Année, Compte, Libellé, Code)][Total != 0]
  
  cumul.lignes.paie <- cumul.lignes.paie[ , Total2  := formatC(Total, big.mark = " ", format = "f", decimal.mark = ",", digits = 2)]
  
  cumul.total.lignes.paie <- cumul.lignes.paie[ , .(`Cumul annuel`= formatC(sum(Total, na.rm = TRUE),
                                                                            big.mark = " ",
                                                                            format = "f",
                                                                            decimal.mark = ",",
                                                                            digits = 2)), 
                                                keyby = .(Année, Compte)]
  
  setnames(cumul.lignes.paie[ , Total := NULL], "Total2", "Total")
  
  inc <- 1
  
  if (afficher.cumuls.détaillés.lignes.paie) {
    
    L <- split(cumul.lignes.paie, cumul.lignes.paie$Année)
    
    for (i in 1:durée.sous.revue) {
      
      cat("\nTableau 5.14." %+% inc, " Année ", début.période.sous.revue + i - 1)
      print(kable(L[[i]][, .(Compte, Code, Libellé, Total)], row.names = FALSE, align = 'r'))
      inc <- inc + 1
      
    }
  }
  
  L <- split(cumul.total.lignes.paie, cumul.total.lignes.paie$Année)
  
  if (exists("L")) {
    for (i in 1:durée.sous.revue) {
       cat("   \nTableau 5.14." %+% inc %+% " Année ",
                    début.période.sous.revue + i - 1)
        
        print(kable(L[[i]][, .(Compte, `Cumul annuel`)], row.names = FALSE, align = 'r'))
        
        inc <- inc + 1
      
    }
  } else cat("Tableaux des correspondances bases de paye-budget : génération impossible.")
  
  sauv.bases(file.path(chemin.dossier.bases, "Reglementation"), 
              environment(),
              "cumul.lignes.paie",
              "cumul.total.lignes.paie")
  
 }, "La correspondance avec le compte de gestion n'a pas pu être établie.")
  
  "code.libelle" %a% code.libelle 
}

