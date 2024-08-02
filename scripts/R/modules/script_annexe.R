newpage()

#'# Annexe       

#'   
#'## Contrôle des événements de paye   
#'      

E <- Bulletins.paie[ , unique(Evenement)]
Evenements <- unique(trimws(regmatches(E, regexpr("[^-]*", E, perl=TRUE))))
Evenements2 <- unique(trimws(gsub("-", "", 
                                  regmatches(E, regexpr("- .*", E, perl=TRUE)))))

Evenements <- sort(c(Evenements, Evenements2))
rm(E, Evenements2)
if (Evenements[1] == "") Evenements <- Evenements[-1] 
Evenements <- data.table(Evenements = Evenements)

if (afficher.table.événements) {
  kable(Evenements, format = "simple")
}

Evenements.ind <- setkey(Bulletins.paie[Evenement != "" & Evenement != "NA NA", 
                                        .(Evenement,
                                          Matricule,
                                          Nom,
                                          Prenom,
                                          Annee,
                                          Mois,
                                          Grade,
                                          Statut,
                                          Emploi,
                                          Service)],
                         Evenement,
                         Matricule,
                         Annee,
                         Mois)

Evenements.mat <- setcolorder(setkey(data.table::copy(Evenements.ind), 
                                     Matricule,
                                     Annee,
                                     Mois,
                                     Evenement),
                              c("Matricule",
                                "Nom",
                                "Prenom",
                                "Annee",
                                "Mois",
                                "Evenement",
                                "Grade",
                                "Statut",
                                "Emploi",
                                "Service"))

#'  
#'[Lien vers la nomenclature des événements de paye](Bases/Fiabilite/Evenements.csv)     
#'[Tri par type d'évement, agent, année, mois](Bases/Fiabilite/Evenements.ind.csv)     
#'[Tri par agent, année, mois, événement](Bases/Fiabilite/Evenements.mat.csv)     
#'  

#'  
#'## Codes et libellés de paye     
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_individualisation.odt)     
#'         
#'   

# Ce cas peut se produire quand la correspondance budgétaire n'est pas générée d'abord.

if (! exists("code.libelle") || is.null(code.libelle)) code.libelle <- unique(Paie[ , .(Code, Libelle, Statut, Type)][ , Compte := ""])

code.libelle.short <- unique(code.libelle[!is.na(Code), .(Code, Libelle, Type)])
code.libelle.short <- remplacer_type(code.libelle.short)

### Ces lignes de code Copyright Fabrice Nicol septembre 2020
# réordonner

code.libelle.short[, ord := 10]
code.libelle.short[Type == "Traitement",  ord := 1]
code.libelle.short[Type == "Indemnité",  ord := 2]
code.libelle.short[Type == "Indemnité de résidence", ord := 3]
code.libelle.short[Type == "Supplément familial",  ord := 4]
code.libelle.short[Type == "Rappels",  ord := 5]
code.libelle.short[Type == "Autres rémunérations",  ord := 6]
code.libelle.short[Type == "Déductions",  ord := 7]
code.libelle.short[Type == "Cotisations",  ord := 8]
setkey(code.libelle.short, ord)

# coller les libellés et types pour chaque code distinct

code.libelle.short <- code.libelle.short[, ord := NULL
                                       ][, lapply(.SD, function(x) paste(unique(x), collapse = " / ")), 
                                            by= Code, .SDcols=c("Libelle", "Type")]

### Fin du Copyright Fabrice Nicol septembre 2020

code.libelle <- remplacer_type(code.libelle)

setcolorder(code.libelle, c("Code", "Libelle", "Statut", "Type", "Compte"))

if (afficher.table.codes) {
  kable(code.libelle, align="c", format = "simple")
}

# Plusieurs libellés par code
plusieurs_libelles_par_code <- unique(code.libelle[ , .(Code, Libelle, Type)])[, Multiplicité := .N, keyby = Code][Multiplicité > 1]

# Plusieurs codes par libellé
plusieurs_codes_par_libelle <- unique(code.libelle[ , .(Libelle,  Code, Type)])[, Multiplicité := .N, keyby = Libelle][Multiplicité > 1]

# Plusieurs types de ligne par code
plusieurs_types_par_code <- unique(code.libelle[, .(Code, Type)])[ , .(Multiplicité = .N,  Type), keyby = Code][Multiplicité > 1]

# Plusieurs types de ligne par libellé
plusieurs_types_par_libelle <- unique(code.libelle[, .(Libelle, Type)])[ , .(Multiplicité = .N,  Type), keyby = Libelle][Multiplicité > 1]

#'Les tables ci-dessous donnent les correspondances entre codes de paye et libellés de paye   
#'Table à utiliser pour l'onglet Codes du dialogue d'options (**CTRL+T puis icône Codes**)        
#'[Lien vers la table Codes/Libelles pour l'onglet Codes](Bases/Fiabilite/code.libelle.short.csv)       
#'   
#'Table plus complète à utiliser pour la fonctionnalité avancée de mise en correspondance des données comptables et de paye, onglet **Budget**     
#'[Lien vers la table Codes/Libelles pour appariement avec les balances](Bases/Fiabilite/code.libelle.csv)       
#'  

sauv.bases("Fiabilite",
           environment(),
           "plusieurs_libelles_par_code",
           "plusieurs_codes_par_libelle",
           "plusieurs_types_par_code",
           "plusieurs_types_par_libelle")


#'  
#'Certains libellés ou codes de paye peuvent être équivoques et entraîner des erreurs de requête.       
#'Les liens ci-après donnent les codes correspondant à au moins deux libellés distincts, les libellés correspondant à au moins deux codes et les codes ou libellés correspondant à au moins deux types de ligne de paye distincts.           
#'L'association d'un même code à plusieurs libellés de paye peut induire des erreurs d'analyse comptable et financière lorsque les libellés correspondent à des types de ligne de paye distincts.    
#'Ces liens ne sont insérés que si de tels cas de multiplicité sont détectés.       
#'     
conditionnel("Plusieurs libellés par code", "Bases/Fiabilite/plusieurs_libelles_par_code.csv")   
conditionnel("Plusieurs codes par libellé", "Bases/Fiabilite/plusieurs_codes_par_libelle.csv")   
conditionnel("Plusieurs types de ligne par code", "Bases/Fiabilite/plusieurs_types_par_code.csv")   
conditionnel("Plusieurs types de ligne par libellé", "Bases/Fiabilite/plusieurs_types_par_libelle.csv")   
#'   
#'## Fiabilite des heures et des quotités de travail           
#'   

nrow.bull <- nrow(Bulletins.paie)
nrow.bull.heures <- nrow(Bulletins.paie[Heures != 0])
nrow.bull.quotites <- nrow(Bulletins.paie[Temps.de.travail != 0])

if (nrow.bull.heures/nrow.bull  < 0.1) 
  message("Attention moins de 10 % des heures sont renseignées")

if (nrow.bull.quotites/nrow.bull < 0.1)
  message("Attention moins de 10 % des quotités sont renseignées")
#'     
cat("Nombre de bulletins : ", FR(nrow.bull))
#'     
if (redresser.heures) {
  if (nredressements > 0) {
    cat("Les heures de travail ont été redressées avec la méthode ", ifelse(test.temps.complet, "des quotités.\n", "de l'interpolation indiciaire\n")) 
    if (test.temps.complet) {
      cat("La méthode des quotités ne s'applique pas aux élus, vacataires et assistantes maternelles détectés. \n")
      cat("Pour les autres agents, si la quotité de temps de travail est non nulle, la méthode redresse le nombre d'heures réalisées à partir du nombre d'heures normal à temps plein lorsqu'une quotité de temps de travail est aussi indiquée.\n")
      cat("La médiane du nombre d'heures accomplies par les agents à temps complet est retenue comme dénominateur pour le calcul des ETPT. Voir notice méthodologique:")
      conditionnel("Compléments méthodologiques", "Docs/methodologie.pdf")         
    }
  }
} else {
  cat("Les heures de travail n'ont pas été redressées.")
}
#'    
cat(" Nombre de bulletins de paie redressés pour le calcul de la quotité de travail :", FR(nredressements)) 
#'    
cat(" Pourcentage de redressements :", round((nredressements)/nrow.bull*100, 2), "% des bulletins de paie.")
#'  
cat("\nPourcentage d'heures renseignées (après redressement éventuel):", round(nrow.bull.heures/nrow.bull*100, 1), "%")
#'   
cat("\nPourcentage de quotités renseignées :", round(nrow.bull.quotites/nrow.bull*100, 1), "%")
#'   

sauv.bases("Fiabilite",
                       environment(),
                       "Evenements",
                       "Evenements.ind",
                       "Evenements.mat",
                       "code.libelle",
                       "code.libelle.short")

#'    
#'## Tableau des personnels  
#'    
#'   
#'*Pour vérifier que le logiciel déduit correctement les categories statutaires des libellés de grade, il est préférable de faire remplir, par les organismes contrôlés le tableau CSV accessible dans le bloc* **Grade et categorie statutaire** *de l'onglet Extra de l'application graphique, ou bien à ce lien. Voir aussi la notice* &nbsp; [![Notice](icones/Notice.png)](Docs/Notices/fiche_tableau_categories.odt)      
#'   

if (afficher.table.effectifs) {
  kable(grades.categories, row.names = FALSE, format = "simple") 
} 

sauv.bases("Effectifs",
                       environment(),
                       "matricules",
                       "grades.categories")

#'
#'[Lien vers la base des grades et categories](Bases/Effectifs/grades.categories.csv)        
#'   

#'
#'[Lien vers la base des personnels](Bases/Effectifs/matricules.csv)        
#'   
#'      
#'## Bulletins de paye et lignes de paye doublonnés   
#'   

e <- Exporter.tables.sans.doublons()

if (e$Ndup) {
    cat("Il a été trouvé", prettyNum(e$Ndup, big.mark = " "), "bulletins de paye doublonnés.   \n")
    cat("Soit un taux de doublons de", round(e$Ndup / nrow(Bulletins.paie) * 100, 1), "%   \n")
} else {
    cat("Il n'a pas été trouvé de bulletins de paye doublonnés.   \n")
}
  
if (e$Ndup2) {
        cat("Il a été trouvé", prettyNum(e$Ndup2, big.mark = " "), "lignes de paye doublonnées.   \n")
        cat("Soit un taux de doublons de", round(e$Ndup2 / nrow(Paie) * 100, 1), "%   \n")                                                     
}
                                 
conditionnel("Bulletins de paye doublonnés", "Bases/Remunerations/Bulletins.paie.dup.csv")   
conditionnel("Bulletins de paye sans doublon", "Bases/Remunerations/Bulletins.paie.uniq.csv")   
conditionnel("Lignes de paye doublonnées", "Bases/Remunerations/Paie.dup.csv")   
conditionnel("Lignes de paye sans doublon", "Bases/Remunerations/Paie.uniq.csv")   
