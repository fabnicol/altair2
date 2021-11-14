#'  
#'## 4.6 Disparités de rémunérations entre les femmes et les hommes   
#'  


Analyse.remunerations[Categorie %in% c("A", "B", "C") 
               & ! is.na(Montant.net.annuel.eqtp) 
               & Montant.net.annuel.eqtp > minimum.positif
               & Statut %in% c("TITULAIRE", "STAGIAIRE", "EMPLOI_FONC"), 
                 .(Moyenne = round(mean(Montant.net.annuel.eqtp)), Effectif = .N), 
                     keyby = "Categorie,Sexe"
               ][ , Sexe := ifelse(Sexe == 1, "Homme", "Femme")] -> T1

Analyse.remunerations[! is.na(Montant.net.annuel.eqtp) 
               & Montant.net.annuel.eqtp > minimum.positif
               & Statut %in% c("TITULAIRE", "STAGIAIRE", "EMPLOI_FONC"), 
               .(Moyenne = round(mean(Montant.net.annuel.eqtp)), Effectif = .N), 
               keyby = "Sexe"
][ , Sexe := ifelse(Sexe == 1, "Homme", "Femme")] -> T1a


Analyse.remunerations[Categorie %in% c("A", "B", "C") 
               & !is.na(Montant.net.annuel.eqtp)
               & round(Montant.net.annuel.eqtp) > minimum.positif
               & Statut == "NON_TITULAIRE", 
                 .(Moyenne = mean(Montant.net.annuel.eqtp), Effectif = .N), 
                     keyby = "Categorie,Sexe"
               ][ , Sexe := ifelse(Sexe == 1, "Homme", "Femme")] -> T2

Analyse.remunerations[
               ! is.na(Montant.net.annuel.eqtp)
               & round(Montant.net.annuel.eqtp) > minimum.positif
               & Statut == "NON_TITULAIRE", 
               .(Moyenne = mean(Montant.net.annuel.eqtp), Effectif = .N), 
               keyby = "Sexe"
][ , Sexe := ifelse(Sexe == 1, "Homme", "Femme")] -> T2a

Analyse.remunerations[Categorie %in% c("A", "B", "C")
               & !is.na(Montant.net.annuel.eqtp) 
               & Montant.net.annuel.eqtp > minimum.positif
               & Statut %in% c("TITULAIRE", "STAGIAIRE", "EMPLOI_FONC"),
                 .(Moyenne = round(mean(Montant.net.annuel.eqtp)), Effectif = .N), 
                   keyby = "Categorie,Sexe"
              ][ , .(`Ecart de rémunération` = signif((Moyenne[2] / Moyenne[1] - 1) * 100, 2), 
                     `Ecart d'effectifs` = signif((Effectif[2] / Effectif[1] - 1) * 100, 2)),
                       keyby = "Categorie"
              ] -> T3

Analyse.remunerations[Categorie %in% c("A", "B", "C") 
               & ! is.na(Montant.net.annuel.eqtp) 
               & Montant.net.annuel.eqtp > minimum.positif
               & Statut %in% c("NON_TITULAIRE"), 
                 .(Moyenne = round(mean(Montant.net.annuel.eqtp)), Effectif = .N),
                   keyby = "Categorie,Sexe"
              ][ , .(Ecart.salarial = signif((Moyenne[2] / Moyenne[1] - 1) * 100, 2), 
                     Ecart.effectifs=signif((Effectif[2] / Effectif[1] - 1) * 100, 2)),
                       keyby="Categorie"
              ] -> T4

#'    
#'  

cat("**Rémunérations nettes annuelles en EQTP en euros et effectifs physiques des fonctionnaires et emplois fonctionnels hommes et femmes ",    
"sur la période ", debut.periode.sous.revue, " - ", fin.periode.sous.revue, "**  ")    

#'  
#'&nbsp;*Tableau 4.6.1*  
#'  

kable(T1a, format.args = list(big.mark =' '))

#'  
#'  
cat("**Rémunérations nettes en EQTP en euros et effectifs physiques des fonctionnaires et emplois fonctionnels hommes et femmes",    
"par catégorie statutaire, sur la période", debut.periode.sous.revue, " - ", fin.periode.sous.revue, "**  ")
   
#'  
#'&nbsp;*Tableau 4.6.2*  
#'  

kable(T1, format.args = list(big.mark =' '))

#'    
#'    

cat("**Rémunérations nettes en EQTP en euros et effectifs physiques des contractuels (hors emplois aidés) hommes et femmes sur la période ", 
    debut.periode.sous.revue, " - ", fin.periode.sous.revue, "**  ")

#'  
#'&nbsp;*Tableau 4.6.3*  
#'  

kable(T2a, format.args = list(big.mark =' '))

#'  
cat("**Rémunérations nettes en EQTP en euros et effectifs physiques des contractuels (hors emplois aidés) hommes et femmes",   
"par catégorie statutaire, sur la période ", debut.periode.sous.revue, " - ", fin.periode.sous.revue, "**  ")

#'  


#'  
#'&nbsp;*Tableau 4.6.4*  
#'  

kable(T2, format.args = list(big.mark =' '))


#'    
#'   
cat("**Ecarts de rémunérations et d'effectifs physiques des femmes par rapport aux hommes en % sur la période ", debut.periode.sous.revue, " - ", fin.periode.sous.revue,
"- champ des fonctionnaires et emplois fonctionnels**  ")

#'   
#'  
#'&nbsp;*Tableau 4.6.5*  
#'  


kable(T3)

#'  
#'&nbsp;*Tableau 4.6.6*  
#'  
#'   

cat("**Ecarts de rémunérations et d'effectifs physiques des femmes par rapport aux hommes en % - champ des contractuels (hors emplois aidés) sur la période", 
    debut.periode.sous.revue, " - ", fin.periode.sous.revue, "**    ")

#'   

kable(T4)
