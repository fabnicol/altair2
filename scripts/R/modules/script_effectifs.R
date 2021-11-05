

#'# 1. Statistiques de population
#'
#'## 1.1 Effectifs

#+ effectifs

message("Démographie...")

# Rappel Analyse.variations.par.exercice comprend uniquement les actifs non annexes non assist. mat., non vacataires, non élus.


# Calcul du tableau des effectifs, qui nécessite les deux bases d'analyse (mensuelle, Analyse.remunerations et annuelle Analyse.variations)
# Utilisation de la fonction effectifs() de la biblio altair.

tableau.effectifs         <<- effectifs(periode, Bulletins.paie, Analyse.remunerations, Analyse.variations)

tableau.effectifs.grades  <<- eqtp.grade(variation = TRUE)

tableau.effectifs.emplois <<- eqtp.emploi(variation = TRUE)

#'  
#  
#'&nbsp;*Tableau 1.1.1 : Effectifs*   
#            

# Mise en forme sous tableau aligné en central par la fonction knitr::kable
# Note: il faut préciser l'argument format = latex avec les versions récentes de knitr et pandoc

if (class(tableau.effectifs) != "try-error") {
  
  kable(tableau.effectifs, row.names = TRUE, 
        format = "simple", align = c('l', rep('r', nb.annees)), digits = 1, 
        format.args = list(big.mark = ' ', decimal.mark = ','))
} else {
  cat("Problème sérieux de qualité des données sur les effectifs ou problème logiciel.")
}

#'    
#'**Nota:**   
#'*(a) Nombre de matricules distincts ayant eu au moins un bulletin de paie dans l'année, en fonction ou non. Tous ces personnels ne sont pas en fonction : sont inclus des régularisations, des personnels hors position d'activité ou des ayants droit (reversion, etc.)*   
#'*(b) Titulaires ou stagiaires*   
#'*(c) Sur la base des libellés d'emploi et des libellés de lignes de paie. La détection peut être lacunaire.*   
#'*(d) ETP  : la quotité est retenue au mois de décembre. Un mi-temps sur 6 mois compte 0,5.*  
#'*(e) ETPT : Equivalent temps plein travaillé = somme des quotités mensuelles divisée par 12. Un mi-temps sur 6 mois compte 0,25.*  
#'*(f) Personnes en place : présentes en N et N-1 avec la même quotité, postes actifs et non annexes uniquement.*     
#'*(g) Postes actifs et non annexes :* voir [Compléments méthodologiques](Docs/méthodologie.pdf)    
#'*&nbsp;&nbsp;&nbsp;Un poste actif est défini par au moins un bulletin de paie comportant un traitement positif pour un volume d'heures de travail mensuel non nul.*             
#'*&nbsp;&nbsp;&nbsp;Un poste non annexe est défini comme la conjonction de critères horaires et de revenu sur une année. La periode minimale de référence est le mois.*   
#'*Les dix dernières lignes du tableau sont calculées en ne tenant pas compte des élus.*    

#+ duree-du-travail

cat("\nLa durée du travail prise en compte dans la base de données est de ", nb.heures.temps.complet, " h par mois.\n")  

if (nb.heures.temps.complet > 1.1 * reference.nb.heures.par.mois || nb.heures.temps.complet < 0.9 * reference.nb.heures.par.mois)  {
  semaine.de.travail <<- nb.heures.temps.complet * 12 / 52
  
  cat("\nAttention !\nLe temps de travail hebdomadaire s'écarte significativement de la durée légale : ", 
      round(semaine.de.travail,1), " h par semaine.\n")
}

setwd(file.path(chemin.dossier.bases, "Effectifs"))

Sauv.base("Effectifs", "tableau.effectifs")
Sauv.base("Effectifs", "tableau.effectifs.grades")
Sauv.base("Effectifs", "tableau.effectifs.emplois")

eqtp.grade.serv(variation = TRUE)

eqtp.grade.cat(variation = TRUE)

csvfiles <- list.files(".", pattern = "^effectifs.*csv")

zip("tableau.effectifs.services.zip",   list.files(".", pattern = "effectifs.serv..*.csv"))
zip("tableau.effectifs.categories.zip", list.files(".", pattern = "effectifs.cat..*.csv"))

invisible(file.remove(csvfiles))

#'      
 
conditionnel("Lien vers la base des effectifs", "Bases/Effectifs/tableau.effectifs.csv")  
conditionnel("Lien vers la base des effectifs en ETPT par grade", "Bases/Effectifs/tableau.effectifs.grades.csv")  
conditionnel("Lien vers la base des effectifs en ETPT par emploi", "Bases/Effectifs/tableau.effectifs.emplois.csv")  
conditionnel("Lien vers la base des effectifs en ETPT par grade et service", "Bases/Effectifs/tableau.effectifs.services.zip")  
conditionnel("Lien vers la base des effectifs en ETPT par grade et categorie", "Bases/Effectifs/tableau.effectifs.categories.zip")  

#'   
#'
setwd(currentDir)

#+ pyramides-des-âges

message("Statistiques de démographie réalisées.")


fichiers.pyr <- list.files(path= file.path(currentDir, "data"), pattern = "*.csv", full.names = TRUE)

# Lecture des fichiers de référence des pyramides (fichiers listés dans fichiers.pyr), comportant les statistiques INSEE

essayer(label = "+fichiers pyr", 
        {  for (f in fichiers.pyr) {
          base <- basename(f)
          assign(substr(base, 1, attr(regexec("(.*)\\.csv", base)[[1]], "match.length")[2]),
                 fread(f, 
                       sep = ";",
                       header = TRUE,
                       encoding = "UTF-8",
                       dec = ",",
                       colClasses = c("integer", "numeric", "numeric", "integer", "character")),
                 envir = .GlobalEnv)
          
        }}, "La lecture des fichiers de référence des pyramides (fichiers listés dans fichiers.pyr), comportant les statistiques INSEE, a échoué.")

setwd(chemin.dossier)
