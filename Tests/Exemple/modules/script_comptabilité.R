#' 
#'## `r chapitre`.13 Lien avec le compte de gestion    
#'

#'   
#'*Pour tirer pleinement profit de ces fonctionnalités, il est préférable de faire remplir, par les organismes contrôlés le tableau CSV accessible dans le bloc* **Bugdet** *de l'onglet Extra de l'application graphique, ou bien à ce lien. Voir aussi la notice* &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_tableau_budget.odt)      
#'   


essayer({ paye.budget.existe <-  file.exists(chemin("paye_budget.csv"))  

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
  cumul.lignes.paie <-  code.libelle[Paie[, .(Année, Code, Libellé, Statut, Type, Montant)],  on = vect]
  
} else {
  
  # Ne pas prendre les capitales ni simplifier les libellés
  
  code.libelle <- unique(Paie[Montant != 0, .(Code, Libellé, Statut), by = "Type"], by = NULL)
  
  # Note : des traitements et NBI sont parfois improprement codés comme indemnités.
  
  code.libelle[Type %chin% c("T", "I", "R", "AC") & grepl(expression.rég.traitement, Libellé, ignore.case = TRUE, perl = TRUE),
               `:=`(Compte.tit    = "64111",
                    Compte.nontit = "64131")]
  
  code.libelle[Type == "IR" | Type == "S" | (Type %chin% c("T", "I", "R") & grepl(expression.rég.nbi, Libellé, ignore.case = TRUE, perl = TRUE)),
               `:=`(Compte.tit    = "64112",
                    Compte.nontit = "64132")]
  
  code.libelle[grepl("(?:ind|prim).*(?:pr[e,é]avis|licen)", Libellé, ignore.case = TRUE, perl = TRUE), 
               `:=`(Compte.tit    = "64116",
                    Compte.nontit = "64136")]
  
  code.libelle[is.na(Compte.tit) 
               & Statut != "ELU"
               & ! Type %chin% c("D", "C", "RE", "CO") 
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

if (afficher.cumuls.détaillés.lignes.paie) {
  
  L <- split(cumul.lignes.paie, cumul.lignes.paie$Année)
  
  for (i in 1:durée.sous.revue) {
    
    cat("\nTableau ", incrément(), " Année ", début.période.sous.revue + i - 1)
    print(kable(L[[i]][, .(Compte, Code, Libellé, Total)], row.names = FALSE, align = 'r'))
    incrément()
    
  }
}

L <- split(cumul.total.lignes.paie, cumul.total.lignes.paie$Année)

}, "La correspondance avec le compte de gestion n'a pas pu être établie.")


if (exists("L")) {
  for (i in 1:durée.sous.revue) {
    essayer({ cat("\nTableau ",
                  incrément(),
                  " Année ",
                  début.période.sous.revue + i - 1)
      
      print(kable(L[[i]][, .(Compte, `Cumul annuel`)], row.names = FALSE, align = 'r'))
    }, "Tableaux des correspondances bases de paye-budget : génération impossible pour l'année " %+% (début.période.sous.revue + i - 1))
  }
} else cat("Tableaux des correspondances bases de paye-budget : génération impossible.")

rm(L)

#'  
#'[Lien vers la base détaillée des cumuls des lignes de paie](Bases/Reglementation/cumul.lignes.paie.csv)
#'  
#'[Lien vers la base agrégée des cumuls des lignes de paie](Bases/Reglementation/cumul.total.lignes.paie.csv)
#'  

#'  
#'*Avertissement : les rappels comprennent également les rappels de cotisations et déductions diverses.*    
#'   
