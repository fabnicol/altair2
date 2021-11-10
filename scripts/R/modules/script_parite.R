#'  
#' 4.5 Disparités de rémunérations entre les femmes et les hommes
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

kable(T1a, 
      caption = "Rémunérations nettes annuelles en EQTP en euros et effectifs physiques des fonctionnaires et emplois fonctionnels hommes et femmes" 
                %+% "\n\t\tsur la période " %+% periode[1] %+% " - " %+% periode[length(periode)],
      format.args = list(big.mark =' '))

#'  
#'  
kable(T1, 
      caption = "Rémunérations nettes en EQTP en euros et effectifs physiques des fonctionnaires et emplois fonctionnels hommes et femmes"
                 %+%  "\n\t\tpar catégorie statutaire" %+% " sur la période " %+% periode[1] %+% " - " %+% periode[length(periode)], 
      format.args = list(big.mark =' '))
#'    
#'    

kable(T2a, 
      caption = "Rémunérations nettes en EQTP en euros et effectifs physiques des contractuels (hors emplois aidés) hommes et femmes"
                 %+% "\n\t\tsur la période " %+% periode[1] %+% " - " %+% periode[length(periode)],
      format.args = list(big.mark =' '))

#'  
#'  

kable(T2, 
      caption = "Rémunérations nettes en EQTP en euros et effectifs physiques des contractuels (hors emplois aidés) hommes et femmes"
      %+% "\n\t\tpar catégorie statutaire"
      %+% " sur la période " %+% periode[1] %+% " - " %+% periode[length(periode)], 
      format.args = list(big.mark =' '))

#'    
#'    

kable(T3,
      caption = "Ecarts de rémunérations et d'effectifs physiques des femmes par rapport aux hommes en %"
      %+% "\n\t\tChamp des fonctionnaires et emplois fonctionnels"
      %+% " sur la période " %+% periode[1] %+% " - " %+% periode[length(periode)])

#'    
#'    

kable(T4,
      caption = "Ecarts de rémunérations et d'effectifs physiques des femmes par rapport aux hommes en % "
      %+% "\n\t\tChamp des contractuels (hors emplois aidés)"
      %+% " sur la période " %+% periode[1] %+% " - " %+% periode[length(periode)])

# Remplacer Analyse.remunerations par Analyse.rémunérations après avoir intégré le sexe 