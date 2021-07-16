#' Diagnostic des tables de jointure     
#' 
#' Calcule le nombre minimum de variables nécessaires pour apparier les bases de paye et un tableau de correspondance entre codes de paye et comptabilité
#' 
#' @return NULL si les variables \code{Code, Libelle, Type} suffisent, sinon le tableau des multiplicités associées aux combinaisons de ces variables auxquelles il est impossible d'associer un compte unique.   
#' @export


calculer_indice_complexité <- function() {
  
  
  multiplicité <- Paie[ , Statut2 := Statut
                      ][Statut %in% c("EMPLOI_FONCTIONNEL", "ELU", "AUTRE_STATUT"), Statut2 := "NON_TITULAIRE"
                      ][Statut == "STAGIAIRE", Statut2 := "TITULAIRE"
                      ][ , .(m = uniqueN(Statut2), Statut2, Annee, Mois, Matricule, Grade, Emploi, Nb.Enfants, NBI, Brut, Montant),
                             by = .(Code, Type, Libelle)]
  
  if (n <- nrow(multiplicité[m > 1])) {
    
    cat("La cle d'appariement **Code, Libelle, Type** est insuffisante pour réaliser une jointure correcte entre la base de paye et le tableau de correspondance codes de paye - compte 64.  \n")
    cat("Au mieux, il ne serait possible que d'apparier", q <- (1 - round(n/nrow(Paie), 2)) * 100, " % des lignes de paye avec la comptabilité administrative.  \n")
    if (q < 10) {
      cat("Il est nécessaire d'utiliser une autre cle d'appariement, au minimum **Statut**, éventuellement **Grade** et **Nb.Enfant**.  \n") 
      NULL
    } else {
      
      cat("L'appariement peut être tenté avec **Code, Libelle, Type**, mais les résultats différeront de ", 100 - q , "% de la comptabilité administrative.  \n")
      multiplicité[m == 1][ , c("m", "Statut2") := NULL]
    }
  }
}

#' Apparier la base des lignes de paye et une table de jointure
#' 
#' Ajoute une ou plusieurs colonnes à la base des lignes de paye, étant donnée une table de jointure comportant des cles d'appariement de la base de paye et des vecteurs à apparier.     
#' 
#' La table de jointure doit satisfaire une condition d'unicité de la valeur associée à chaque combinaison de cles.
#' @note   Pour chaque combinaison de valeurs des cles, il doit y avoir une et une seule valeur des colonnes supplémentaires apportees par la table de jointure.
#' @param  table.jointure  Tableau au format \code{data.table} indiquant la correspondance entre des cles appartenant à une \code{data.table} et un ou plusieurs vecteurs à rajouter à cette base.    
#' @param  requis Vecteur des noms des colonnes qui sont attendues dans le tableau de jointure, autre que les cles d'appariement, pour ajout à la base de paye.
#' @param  cles   Vecteur des noms de cles d'appariement. Par défaut, les noms de colonnes communs à la base des lignes de paye et à la table de jointure.
#' @param  calculer.indice.complexité Pour l'appariement avec la comptabilité, vérifier s'il est éventuellement possible d'apparier sur les seules cles \code{Code, Libelle, Type}
#' @note   Effet de bord : Base des lignes de paye \code{Paie} appariée avec la table de jointure.
#' @return Base des lignes de paye \code{Paie} appariée avec la table de jointure restreinte aux variables : \code{Annee, Code, Libelle, Statut, Type} et aux colonnes ajoutées par la table de jointure 
#' @export
#' 
exporter_tableau <- function(table.jointure, requis, cles = intersect(names(table.jointure), names(Paie)), calculer.indice.complexité = FALSE) {
  
  colonnes          <- names(Paie)
  colonnes.jointure <- names(table.jointure)
  colonnes.ajoutées <- setdiff(colonnes.jointure, colonnes)
  nrequis           <- length(requis)
  
  if (nrequis < 1)  {
    cat("Le vecteur **requis** doit contenir au moins un nom de variable.")
    return(NULL)
  }
  
  if (! all(requis %in% colonnes.ajoutées) || ! all(cles %in% colonnes.jointure)) {
    
    cat("Le tableau fourni par l'organisme doit contenir", ifelse(nrequis > 1, "les", "la"), " colonne" %+% ifelse(nrequis > 1, "s", ""),
                                                           paste(colonnes.jointure, collapse = " "), "  \n")
    return(NULL)
  }

  if (calculer.indice.complexité) {
    
    res <- calculer_indice_complexité()
    
    if (is.null(res) && apparier.sur.trois.cles) {
        cles <- c("Code", "Libelle", "Type")
    } 
  }
  
  test <- anyDuplicated(table.jointure[ , ..cles])
    
  if (test > 0) {
    
    TabDupl <- duplicated(table.jointure[ , ..cles])
    table.jointure <- unique(table.jointure[ , ..cles])
    if ("Compte" %in% colonnes.ajoutées) cat("Le tableau fourni par l'organisme (*paye_budget.csv*) associe plus d'un compte à une combinaison donnée des variables cles ",  paste(cles, collapse = " "), " pour les combinaisons de cles suivantes : ", paste(cles, collapse = " "), "  \n")
    cat("L'opération d'appariement ne peut se faire sur ces cles. Elle se fera sur les autres cles au prix d'une perte de données.   \n
        Les agrégats seront donc inférieurs à ceux de la comptabilité.  \n")
    cat("Il est envisageable de récupérer les montants correspondants en examinant manuellement le tableau fourni en lien ci-dessous, correspondant aux cles suivantes:  \n")
    kable(TabDupl)
    # Insérer lien condiditionnel sur TabDupl dans le rapport.
    
  } else {
    
    cat("Le tableau fourni par l'organisme contient des cles d'appariement convenables. Chaque combinaison de valeurs des cles ", paste(cles, collapse = " "), " est associée à une seule valeur de(s) colonne(s) rajoutée(s) par la table d'appariement     \n")
    cat("    \n")    
  }
    
  
  "Paie" %a% merge(table.jointure, Paie, all.y = TRUE, by = cles)
  
  if ("Compte" %in% names(Paie)) cat("Colonne **Compte** ajoutée à la base Paie par jointure.   \n")
  
  cols <- c("Annee", cles, colonnes.ajoutées, "Montant")
  Paie[ , ..cols]
  
}

#' Correspondance paye-budget
#' 
#' Etablit la correspondance entre paye et comptabilité administrative (comptes 64 et 65)
#' 
#' @note Requiert l'utilisation d'une table de jointure importee \bold{paye_budget.csv} encodée en UTF-8 sous le répertoire \bold{Données}.
#' A défaut, tente une association approximative à partir d'expressions rationnelles appliquées aux libellés de paye.  
#' @return La \code{data.table code.libellé} résultant de la lecture du fichier \bold{paye_budget.csv} sous le répertoire \bold{Données}
#' @export
#' 
correspondance_paye_budget <- function() {

 essayer(label ="+comptabilité",
 {  
    "paye.budget.existe" %a%  file.exists(chemin("paye_budget.csv"))  
  
    vect <- c("Code", "Libelle", "Statut", "Type")
  
 
    code.libelle <- fread(chemin("paye_budget.csv"), # Code, Libelle,  Statut, Type, Compte
                          sep = ";",
                          encoding = "UTF-8",
                          col.names  = c(vect, "Compte"),
                          colClasses = c("character", "character", "character", "character", "character"))  
    
    cat("Importation de la table des codes et libellés par compte (paye_budget.csv)...   \n")
    
    code.libelle <- résumer_type(code.libelle)
    
    code.libelle      <- unique(code.libelle)
    
    cumul.lignes.paie <- exporter_tableau(code.libelle, requis = "Compte", cles = vect)
    
  
  setkey(code.libelle, Type, Compte, Statut, Code, Libelle)
  
  cumul.lignes.paie[is.na(Compte) | Compte == "", Compte := "Autres"]
  
  cumul.lignes.paie <- cumul.lignes.paie[ , .(Total = sum(Montant, na.rm = TRUE)), keyby = .(Annee, Compte, Libelle, Code)
                                        ][Total != 0
                                        ][ , Total2  := formatC(Total, big.mark = " ", format = "f", decimal.mark = ",", digits = 2)]
  
  cumul.total.lignes.paie <- cumul.lignes.paie[ , .(`Cumul annuel`= formatC(sum(Total, na.rm = TRUE),
                                                                            big.mark = " ",
                                                                            format = "f",
                                                                            decimal.mark = ",",
                                                                            digits = 2)), 
                                                keyby = .(Annee, Compte)]
  
  setnames(cumul.lignes.paie[ , Total := NULL], "Total2", "Total")
  
  inc <- 1
  
  if (afficher.cumuls.détaillés.lignes.paie) {
    
    L <- split(cumul.lignes.paie, cumul.lignes.paie$Annee)
    
    for (i in 1:duree.sous.revue) {
      
      cat("\nTableau 5.14." %+% inc, " Annee ", debut.periode.sous.revue + i - 1)
      print(kable(L[[i]][, .(Compte, Code, Libelle, Total)], row.names = FALSE, align = 'r'))
      inc <- inc + 1
      
    }
  }
  
  L <- split(cumul.total.lignes.paie, cumul.total.lignes.paie$Annee)
  
  if (exists("L")) {
    for (i in 1:duree.sous.revue) {
       cat("   \nTableau 5.14." %+% inc %+% " Annee ",
                    debut.periode.sous.revue + i - 1)
        
        print(kable(L[[i]][, .(Compte, `Cumul annuel`)], row.names = FALSE, align = 'r'))
        
        inc <- inc + 1
      
    }
  } else cat("Tableaux des correspondances bases de paye-budget : génération impossible.")
  
  sauv.bases("Reglementation", 
              environment(),
              "cumul.lignes.paie",
              "cumul.total.lignes.paie")
  
 }, "La correspondance avec le compte de gestion n'a pas pu être établie.")
  
  "code.libelle" %a% code.libelle 
}

