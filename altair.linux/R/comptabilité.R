#' Diagnostic des tables de jointure     
#' 
#' Calcule le nombre minimum de variables nécessaires pour apparier les bases de paye et un tableau de correspondance entre codes de paye et comptabilité
#' 
#' @return NULL si les variables \code{Code, Libellé, Type} suffisent, sinon le tableau des multiplicités associées aux combinaisons de ces variables auxquelles il est impossible d'associer un compte unique.   
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
      cat("Il est nécessaire d'utiliser une autre clé d'appariement, au minimum **Statut**, éventuellement **Grade** et **Nb.Enfant**.  \n") 
      NULL
    } else {
      
      cat("L'appariement peut être tenté avec **Code, Libellé, Type**, mais les résultats différeront de ", 100 - q , "% de la comptabilité administrative.  \n")
      multiplicité[m == 1][ , c(m, Statut2) := NULL]
    }
  }
}

#' Apparier la base des lignes de paye et une table de jointure
#' 
#' Ajoute une ou plusieurs colonnes à la base des lignes de paye, étant donnée une table de jointure comportant des clés d'appariement de la base de paye et des vecteurs à apparier.     
#' 
#' La table de jointure doit satisfaire une condition d'unicité de la valeur associée à chaque combinaison de clés.
#' @note   Pour chaque combinaison de valeurs des clés, il doit y avoir une et une seule valeur des colonnes supplémentaires apportées par la table de jointure.
#' @param  table.jointure  Tableau au format \code{data.table} indiquant la correspondance entre des clés appartenant à une \code{data.table} et un ou plusieurs vecteurs à rajouter à cette base.    
#' @param  requis Vecteur des noms des colonnes qui sont attendues dans le tableau de jointure, autre que les clés d'appariement, pour ajout à la base de paye.
#' @param  clés   Vecteur des noms de clés d'appariement. Par défaut, les noms de colonnes communs à la base des lignes de paye et à la table de jointure.
#' @param  calculer.indice.complexité Pour l'appariement avec la comptabilité, vérifier s'il est éventuellement possible d'apparier sur les seules clés \code{Code, Libellé, Type}
#' @note   Effet de bord : Base des lignes de paye \code{Paie} appariée avec la table de jointure.
#' @return Base des lignes de paye \code{Paie} appariée avec la table de jointure restreinte aux variables : \code{Année, Code, Libellé, Statut, Type} et aux colonnes ajoutées par la table de jointure 
#' @export
#' 
exporter_tableau <- function(table.jointure, requis, clés = intersect(names(table.jointure), names(Paie)), calculer.indice.complexité = FALSE) {
  
  colonnes <- names(Paie)
  colonnes.jointure <- names(table.jointure)
  colonnes.ajoutées <- setdiff(colonnes.jointure, colonnes)
  nrequis <- length(requis)
  if (nrequis < 1)  {
    cat("Le vecteur **requis** doit contenir au moins un nom de variable.")
    return(NULL)
  }
  
  if (! all(requis %in% colonnes.ajoutées) || ! all(clés %in% colonnes.jointure)) {
    cat("Le tableau fourni par l'organisme doit contenir", ifelse(requis > 1, "les", "la"), " colonne" %+% ifelse(nrequis > 1, "s", ""),
                                                           paste(colonnes.jointure, collapse = " "), "  \n")
    return(NULL)
  }

  if (calculer.indice.complexité) {
    res <- calculer_indice_complexité()
    if (is.null(res) && apparier.sur.trois.clés) {
        clés <- c("Code", "Libellé", "Type")
    } 
  }
  
  test <- anyDuplicated(table.jointure[ , ..clés])
    
  if (test > 0) {
    
    TabDupl <- duplicated(table.jointure[ , ..clés])
    table.jointure <- unique(table.jointure[ , ..clés])
    if ("Compte" %in% colonnes.ajoutées) cat("Le tableau fourni par l'organisme (*paye_budget.csv*) associe plus d'un compte à une combinaison donnée des variables clés ",  paste(clés, collapse = " "), " pour les combinaisons de clés suivantes : ", paste(clés, collapse = " "), "  \n")
    cat("L'opération d'appariement ne peut se faire sur ces clés. Elle se fera sur les autres clés au prix d'une perte de données.   \n
        Les agrégats seront donc inférieurs à ceux de la comptabilité.  \n")
    cat("Il est envisageable de récupérer les montants correspondants en examinant manuellement le tableau fourni en lien ci-dessous, correspondant aux clés suivantes:  \n")
    kable(TabDupl)
    # Insérer lien condiditionnel sur TabDupl dans le rapport.
    
  } else {
    
    cat("Le tableau fourni par l'organisme contient des clés d'appariement convenables. Chaque combinaison de valeurs des clés ", paste(clés, collapse = " "), " est associée à une seule valeur de(s) colonne(s) rajoutée(s) par la table d'appariement  \n")
  }
    
  "Paie" %a% table.jointure[Paie, on = clés]
  
  cols <- c("Année", "Code", "Libellé", "Statut", "Type", colonnes.ajoutées)
  Paie[ , ..cols]
  
}

#' Correspondance paye-budget
#' 
#' Etablit la correspondance entre paye et comptabilité administrative (comptes 64 et 65)
#' 
#' @note Requiert l'utilisation d'une table de jointure importée \bold{paye_budget.csv} sous le répertoire \bold{Données}.
#' A défaut, tente une association approximative à partir d'expressions rationnelles appliquées aux libellés de paye.  
#' @return La \code{data.table code.libellé} résultant de la lecture du fichier \bold{paye_budget.csv} sous le répertoire \bold{Données}
#' @export
#' 
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
    
    cumul.lignes.paie <- exporter_tableau(code.libelle, requis = "Compte", clés = c("Code", "Libellé", "Type", "Statut"))
    
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
  
  cumul.lignes.paie <- cumul.lignes.paie[ , .(Total = sum(Montant, na.rm = TRUE)), keyby = .(Année, Compte, Libellé, Code)
                                        ][Total != 0
                                        ][ , Total2  := formatC(Total, big.mark = " ", format = "f", decimal.mark = ",", digits = 2)]
  
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

