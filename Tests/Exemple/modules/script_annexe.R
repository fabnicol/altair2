
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
  kable(Evenements)
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

Evenements.mat <- setcolorder(setkey(copy(Evenements.ind), 
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
#'[Tri par agent, année, mois, évenement](Bases/Fiabilite/Evenements.mat.csv)     
#'  

#'  
#'## Codes et libellés de paye     
#'[![Notice](Notice.png)](Docs/Notices/fiche_individualisation.odt)     
#'         
#'   

code.libelle.short <- unique(code.libelle[, .(Code, Libelle)])

code.libelle <- remplacer_type(code.libelle)

setcolorder(code.libelle, c("Code", "Libelle", "Statut", "Type", "Compte"))

if (afficher.table.codes) {
  kable(code.libelle, align="c")
}

#'Certains libellés ou codes de paye peuvent être équivoques et entraîner des erreurs de requête.       
#'Les liens ci-après donnent les codes correspondant à au moins deux libellés distincts, les libellés correspondant à au moins deux codes et les codes ou libellés correspondant à au moins deux types de ligne de paye distincts.           
#'L'association d'un même code à plusieurs libellés de paye peut induire des erreurs d'analyse comptable et financière lorsque les libellés correspondent à des types de ligne de paye distincts.    
#'

# Plusieurs libellés par code
plusieurs_libelles_par_code <- unique(code.libelle[ , .(Code, Libelle, Type)], by = NULL)[, Multiplicité := .N, keyby = Code][Multiplicité > 1]

# Plusieurs codes par libellé
plusieurs_codes_par_libelle <- unique(code.libelle[ , .(Libelle,  Code, Type)], by = NULL)[, Multiplicité := .N, keyby = Libelle][Multiplicité > 1]

# Plusieurs types de ligne par code
plusieurs_types_par_code <- unique(code.libelle[, .(Code, Type)], by = NULL)[ , .(Multiplicité = .N,  Type), keyby = Code][Multiplicité > 1]

# Plusieurs types de ligne par libellé
plusieurs_types_par_libelle <- unique(code.libelle[, .(Libelle, Type)], by = NULL)[ , .(Multiplicité = .N,  Type), keyby = Libelle][Multiplicité > 1]

#'   
#'[Lien vers la table Codes/Libelles](Bases/Fiabilite/code.libelle.short.csv)       
#'[Lien vers la table Codes/Libelles](Bases/Fiabilite/code.libelle.csv)       
conditionnel("Plusieurs libellés par code", "Bases/Fiabilite/plusieurs_libelles_par_code.csv")   
conditionnel("Plusieurs codes par libellé", "Bases/Fiabilite/plusieurs_codes_par_libelle.csv")   
conditionnel("Plusieurs types de ligne par code", "Bases/Fiabilite/plusieurs_types_par_code.csv")   
conditionnel("Plusieurs types de ligne par libellé", "Bases/Fiabilite/plusieurs_types_par_libelle.csv")   
#'   

#'  
#'## Doublons                
#'

if (éliminer.duplications) {
  if (après.redressement != avant.redressement) {
    
    cat("Retraitement de la base de lignes de paie : ")
    
  } else {
    cat("Aucune duplication de ligne de paie n'a été détectée. ")
  }
  
} else {
  
  if (anyDuplicated(Paie) || anyDuplicated(Bulletins.paie)) {
    cat("Attention : Altaïr a détecté des lignes dupliquées alors qu'aucun retraitement des lignes dupliquées n'est prévu par défaut.")
  } else {
    cat("Aucune duplication de ligne n'a été détectée. ")
  }
}

#'  
if (après.redressement != avant.redressement)
  cat("Elimination de ", FR(avant.redressement - après.redressement), " lignes dupliquées")
#'  
#'## Fiabilite des heures et des quotites de travail           
#'   

nrow.bull <- nrow(Bulletins.paie)
nrow.bull.heures <- nrow(Bulletins.paie[Heures != 0])
nrow.bull.quotites <- nrow(Bulletins.paie[Temps.de.travail != 0])

if (nrow.bull.heures/nrow.bull  < 0.1) 
  message("Attention moins de 10 % des heures sont renseignées")

if (nrow.bull.quotites/nrow.bull < 0.1)
  message("Attention moins de 10 % des quotites sont renseignées")
#'     
cat("Nombre de bulletins : ", FR(nrow.bull))
#'     
if (redresser.heures) {
  if (nredressements > 0) {
    cat("Les heures de travail ont été redressées avec la méthode ", ifelse(test.temps.complet, "des quotites.\n", "de l'interpolation indiciaire\n")) 
  }
} else {
  cat("Les heures de travail n'ont pas été redressées.")
}
#'    
cat(" Nombre de bulletins de paie redressés :", FR(nredressements)) 
#'    
cat(" Pourcentage de redressements :", round((nredressements)/nrow.bull*100, 2), "% des bulletins de paie.")
#'  
cat("\nPourcentage d'heures renseignées (après redressement éventuel):", round(nrow.bull.heures/nrow.bull*100, 1), "%")
#'   
cat("\nPourcentage de quotites renseignées :", round(nrow.bull.quotites/nrow.bull*100, 1), "%")
#'   
cat("\nNombre de bulletins à heures et quotites : ", n <- nrow(Bulletins.paie[Heures != 0 & Temps.de.travail != 0]), "[", round(n/nrow.bull*100, 1), "%]")
#'   
cat("\nNombre de bulletins à heures sans quotites : ", n <- nrow(Bulletins.paie[Heures != 0 & Temps.de.travail == 0]), "[", round(n/nrow.bull*100, 1), "%]")
#'   
cat("\nNombre de bulletins à quotites sans heures : ", n <- nrow(Bulletins.paie[Heures == 0 & Temps.de.travail != 0]), "[", round(n/nrow.bull*100, 1), "%]")
#'   
cat("\nNombre de bulletins apparemment inactifs : ", n <- nrow(Bulletins.paie[(Heures == 0 | is.na(Heures)) & (Temps.de.travail == 0 | is.na(Temps.de.travail))]), "[", round(n/nrow.bull*100, 1), "%]")  
#'   
base.heures.nulles.salaire.nonnull     <- Bulletins.paie[Heures == 0  & (Net.a.Payer != 0 | Brut != 0)]
base.quotite.indefinie.salaire.non.nul <- Bulletins.paie[MHeures == 0 & (Net.a.Payer != 0 | Brut != 0)]

nligne.base.heures.nulles.salaire.nonnull     <- nrow(base.heures.nulles.salaire.nonnull)
nligne.base.quotite.indefinie.salaire.non.nul <- nrow(base.quotite.indefinie.salaire.non.nul)
#'  
if (nligne.base.heures.nulles.salaire.nonnull)
  cat("Nombre de bulletins de paie de salaires versés pour un champ Heures = 0 : ", FR(n <<- nligne.base.heures.nulles.salaire.nonnull), "[", round(n/nrow.bull * 100, 1), "%]")
#'   

sauv.bases(file.path(chemin.dossier.bases, "Fiabilite"),
                       environment(),
                       "Evenements",
                       "Evenements.ind",
                       "Evenements.mat",
                       "code.libelle",
                       "code.libelle.short",
                       "plusieurs_libelles_par_code",
                       "plusieurs_codes_par_libelle",
                       "plusieurs_types_par_code",
                       "plusieurs_types_par_libelle",
                       "base.heures.nulles.salaire.nonnull",
                       "base.quotite.indefinie.salaire.non.nul")

if (nligne.base.quotite.indefinie.salaire.non.nul)
  cat("\nNombre de bulletins de paie de salaires versés pour une quotite de travail indéfinie : ", FR(nligne.base.heures.nulles.salaire.nonnull))
#'   
conditionnel("Lien vers la base de données des salaires versés pour Heures=0", "Bases/Fiabilite/base.heures.nulles.salaire.nonnull.csv")   
conditionnel("Lien vers la base de données des salaires versés à quotite indéfinie", "Bases/Fiabilite/base.quotite.indefinie.salaire.non.nul.csv")   
#'    
#'## Tableau des personnels  
#'    
#'   
#'*Pour vérifier que le logiciel déduit correctement les catégories statutaires des libellés de grade, il est préférable de faire remplir, par les organismes contrôlés le tableau CSV accessible dans le bloc* **Grade et catégorie statutaire** *de l'onglet Extra de l'application graphique, ou bien à ce lien. Voir aussi la notice* &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_tableau_categories.odt)      
#'   

if (afficher.table.effectifs) {
  kable(grades.categories, row.names = FALSE) 
} 

sauv.bases(file.path(chemin.dossier.bases, "Effectifs"),
                       environment(),
                       "matricules",
                       "grades.categories")

#'
#'[Lien vers la base des grades et catégories](Bases/Effectifs/grades.categories.csv)        
#'   

#'
#'[Lien vers la base des personnels](Bases/Effectifs/matricules.csv)        
#'   


#'
#'## Divergences lignes-bulletins de paie     
#'   
#'*Pour exclure certains codes de paie de l'analyse, renseigner le fichier liste.exclusions.txt*  
#'   
if (! séquentiel) {
  cat("La recherche des différences entre lignes et bulletins de paye n'est activée qu'en mode séquentiel (non parallèle)")
} else {  
  if (test.delta) {
    if (!is.null(liste.exclusions))
      message("Une liste de codes exclus pour la vérification de la concordance lignes-bulletins de paie a été jointe sous ", getwd())
    cat("   ")
    source("delta.R", encoding=encodage.code.source)
    sauv.bases(file.path(chemin.dossier.bases, "Fiabilite"),
                            environment(),
                            "Delta")
  } else {
    cat("Base de vérification des écarts lignes de paie-bulletins de paie non générée.")
  }
}

#'   
conditionnel("Divergences lignes-bulletins de paie", "Bases/Fiabilite/Delta.csv")     
#'   
