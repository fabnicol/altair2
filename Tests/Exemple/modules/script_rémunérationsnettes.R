#'    
#'## 4.1 Distribution de la rémunération nette moyenne sur la période    
#+ remuneration-nette


attach(Analyse.variations, warn.conflicts = FALSE)

calcul_rmpp()

temp <- positive(moyenne.rémunération.annuelle.sur.période) / 1000

essayer({ if (longueur.non.na(temp) > 0)
  hist(temp,
       xlab = "Sur la période " %+% début.période.sous.revue %+% "-" %+% fin.période.sous.revue %+% " en milliers d'euros",
       ylab = "Effectif",
       main = "Rémunération nette moyenne",
       col = "blue",
       nclass = 200)
}, "Le graphique des rémunérations moyennes n'a pas pu être généré")

#'    
#+ fig.height=4.5   

temp <- na.omit(moyenne.rémunération.annuelle.sur.période[moyenne.rémunération.annuelle.sur.période > minimum.positif
                                                          & (statut == "TITULAIRE"  | statut == "STAGIAIRE")] / 1000)

essayer(label = "+histogramme",
        {  if (longueur.non.na(temp) > 0)
          hist(temp,
               xlab = "Sur la période "%+% début.période.sous.revue %+% "-" %+% fin.période.sous.revue %+% " en milliers d'euros",
               ylab = "Effectif",
               main = "Rémunération nette moyenne des fonctionnaires",
               col = "blue",
               nclass = 200)
          
        }, "Le graphique des rémunérations moyennes des fonctionnaires n'a pas pu être généré")

#'   
conditionnel("Lien vers la base de données synthétique", "Bases/Remunerations/Analyse.variations.csv")     
conditionnel("Lien vers la base de données détaillée par année", "Bases/Remunerations/Analyse.variations.par.exercice.csv")     

########### 4.2  Evolutions des rémunérations nettes ###########

#'## 4.2 Evolutions du SMPT sur la période `r début.période.sous.revue` - `r fin.période.sous.revue`   
#'
#'### 4.2.1 Evolution du SMPT pour l'ensemble des personnels fonctionnaires et non titulaires (hors élus)        
#'
#+ remuneration-nette-evolution

masse.salariale.nette <- rep(0, durée.sous.revue)

# sommation sur les matricules à année fixe 

#'    
#'**Salaire net moyen par tête (SMPT net) en EQTP, hors élus**         
#'       
#'  
#'&nbsp;*Tableau 4.2.1.1*   
#'    

#+ Salaire-moyen-par-tete    

essayer(label = "+SMPT global", print(smpt(Filtre_neutre)),     "Le salaire moyen par tête n'a pas pu être calculé.")    

#'**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         
#'**pour les salariés à temps complet**           
#'  
#'&nbsp;*Tableau 4.2.1.2*     
#'    

#+ premiere-derniere-annee

essayer(label = "+distribution SMPT global", print(distribution_smpt(Filtre_neutre)), "La distribution du salaire moyen par tête n'a pas pu être calculée.")

#'  
#'*Nota :*  La population retenue est constituée des agents qui ne font pas partie des `r 2*quantile.cut` centiles extrêmaux   
#'Les élus, vacataires et assistantes maternelles sont retirés du périmètre.   
#'TC :  personnels à temps complet sur toute l'année            
#'Seuls sont pris en compte les agents ayant connu au moins un mois actif et ayant eu, sur l'année, des rémunérations non annexes.  
#'[Compléments méthodologiques](Docs/méthodologie.pdf)     
#'      


#'**Comparaisons source INSEE/DGCL**   
#'
#'**Salaires nets annuels moyens 2011 à 2013 en EQTP (hors assistantes maternelles)**   
#'  
#'&nbsp;*Tableau 4.2.1.3*       

#### INSEE/DGCL VARIATIONS  ####
#'  
#'  |  net (euros)    | 2011    | 2012   |  2013  |  2014  |     
#'  |-----------------|--------:|-------:|-------:|-------:|   
#'  |    Ensemble     |  21 876 | 22 176 | 22 224 | 22 524 |  
#'  |   Titulaires    |  22 632 | 22 920 | 22 920 | 23 424 |  
#'  | Autres salariés |  18 864 |  NA    |  NA    | 18 732 |   
#' 
#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			

#'   
#'**Distribution des salaires nets annuels en EQTP dans la fonction publique territoriale (2011-2014)**   
#' 
#'  
#'&nbsp;*Tableau 4.2.1.4*   
#'    
# Remarque sur le formatage markdown: en raison d'un bug de knitr, utiliser un dièse + apostrophe
# suivi d'un seul blanc juste après la table.

#'  
#' | Décile \ euros   | 2011     | 2013   | 2014   |  
#' |------------------|----------|--------|--------|   
#' |    D1            | 15 288   | 15 600 | 15 768 |   
#' |    D2            | 16 512   | 16 860 | 17 124 |   
#' |    D3            | 17 508   | 17 844 | 18 156 |   
#' |    D4            | 18 480   | 18 816 | 19 164 |  
#' |    D5 (médiane)  | 19 632   | 19 908 | 20 256 |    
#' |    D6            | 21 012   | 21 300 | 21 648 |  
#' |    D7            | 22 860   | 23 160 | 23 496 |  
#' |    D8            | 25 596   | 25 956 | 26 292 |  
#' |    D9            | 30 876   | 31 272 | 31 596 |  
#' |    Moyenne       | 21 876   | 22 212 | 22 524 |  
#'    

#'**Distribution des salaires nets annuels en EQTP dans la fonction publique d'Etat (2011-2013)**   
#' 
#'  
#'&nbsp;*Tableau 4.2.1.5*   
#'    

#'    
#' | Décile \ euros   | 2011     | 2013   |   
#' |------------------|----------|--------|
#' |    D1            | 17 496   | 18 012 |  
#' |    D2            | 20 916   | 21 348 |    
#' |    D3            | 23 052   | 23 376 |  
#' |    D4            | 24 912   | 25 248 |  
#' |    D5 (médiane)  | 26 832   | 27 120 |    
#' |    D6            | 28 944   | 29 220 |    
#' |    D7            | 31 632   | 31 968 |    
#' |    D8            | 35 592   | 35 964 |    
#' |    D9            | 42 456   | 42 780 |
#' | Moyenne          | 29 208   | 29 628 |  
#' 


#'**Distribution des salaires nets annuels en EQTP dans la fonction publique hospitalière (hôpitaux) (2011-2013)**   

#'  
#'&nbsp;*Tableau 4.2.1.6*   
#'    

#'    
#' | Décile \ euros   | 2011     | 2013   |   
#' |------------------|----------|--------|
#' |    D1            | 16 584   | 17 016 |  
#' |    D2            | 18 168   | 18 492 |    
#' |    D3            | 19 620   | 19 872 |  
#' |    D4            | 21 048   | 21 192 |  
#' |    D5 (médiane)  | 22 596   | 22 656 |    
#' |    D6            | 24 504   | 24 516 |    
#' |    D7            | 27 216   | 27 252 |    
#' |    D8            | 30 996   | 31 176 |    
#' |    D9            | 37 812   | 38 100 |    
#' |  Moyenne         | 26 496   | 26 916 |  
#' 

#'[Source INSEE, onglets Figure3, F1web et F3web - 2011](Docs/ip1486.xls)    
#'[Source INSEE, onglets F V3.1-2, F V3.1-5 - 2013](Docs/vue3_remunerations.xls)     
#'[Source INSEE 2016](Docs/insee-premiere1616.pdf)    

#'   
#'### 4.2.2 Evolution du SMPT des fonctionnaires         
#'   
#'##### 4.2.2.1 Toutes catégories statutaires          

#'**Salaire net moyen par tête (SMPT net) en EQTP**       
#'    
#'    
#'&nbsp;*Tableau 4.2.2.1.1*   
#'    

essayer(label = "+SMPT fonct.", print(smpt(Filtre_fonctionnaire)), "Le salaire moyen par tête des fonctionnaires n'a pas pu être calculé.")

#'    
#'##### 4.2.2.2 Par catégorie  statutaire        
#'   
#'**Categorie A**  
#'
#'&nbsp;*Tableau 4.2.2.2.1*   
#'    
#'  


essayer(label = "+SMPT cat A", print(smpt(Filtre_cat_A)), "Le salaire moyen par tête des fonctionnaires de catégorie A n'a pas pu être calculé.")  

#'  
#'*Comparaisons nationales*    
#'*FPT catégorie A*    
#'            
#' | Décile \ euros   | 2011     | 2013   |   2014   |  
#' |------------------|----------|--------|----------|   
#' |    D1            | 26 040   | 26 340 |  26 460  |   
#' |    D2            | 28 992   |        |          |   
#' |    D3            | 31 272   |        |          |   
#' |    D4            | 33 468   |        |          |   
#' |    D5 (médiane)  | 35 820   | 36 312 |  36 580  |    
#' |    D6            | 38 664   |        |          |
#' |    D7            | 42 276   |        |          |    
#' |    D8            | 47 124   |        |          |
#' |    D9            | 54 840   | 55 032 |  55 440  |       
#' |  Moyenne         | 38 700   | 39 120 |  39 360  |  
#' 

#'   
#'**Categorie B**  
#'   
#'   
#'&nbsp;*Tableau 4.2.2.2.2*   
#'    
#'  

essayer(label = "+SMPT cat B", print(smpt(Filtre_cat_B)),          "Le salaire moyen par tête des fonctionnaires de catégorie B n'a pas pu être calculé.")  

#'*Comparaisons nationales*    
#'*FPT catégorie B*    
#'    
#' | Décile \ euros   | 2011     | 2013   |  2014   |    
#' |------------------|----------|--------|---------|   
#' |    D1            | 20 580   | 20 964 |  21 108 |   
#' |    D2            | 22 272   |        |         |   
#' |    D3            | 23 652   |        |         |   
#' |    D4            | 24 960   |        |         |   
#' |    D5 (médiane)  | 26 244   | 26 820 |  27 000 |     
#' |    D6            | 27 636   |        |         |   
#' |    D7            | 29 160   |        |         |    
#' |    D8            | 30 984   |        |         |   
#' |    D9            | 33 804   | 34 224 | 34 344  |   
#' |  Moyenne         | 26 940   | 27 408 | 27 588  |  
#' 

#'   
#'**Categorie C**  
#'
#'&nbsp;*Tableau 4.2.2.2.3*    
#'    
#'  

essayer(label = "+SMPT cat C", print(smpt(Filtre_cat_C)),       "Le salaire moyen par tête des fonctionnaires de catégorie C n'a pas pu être calculé.")    

#'     
#'*Comparaisons nationales*    
#'*FPT catégorie C*    
#'    
#' | Décile \ euros   | 2011     | 2013   |  2014  | 
#' |------------------|----------|--------|--------|
#' |    D1            | 15 972   |  16 296| 16 632 |
#' |    D2            | 16 896   |        |        |
#' |    D3            | 17 652   |        |        |
#' |    D4            | 18 360   |        |        |
#' |    D5 (médiane)  | 19 164   |  19 464| 19 884 |
#' |    D6            | 20 100   |        |        |
#' |    D7            | 21 216   |        |        |
#' |    D8            | 22 680   |        |        |
#' |    D9            | 24 996   |  25 176| 25 608 |
#' |    Moyenne       | 20 016   |  20 268| 20 676 |
#' 



#'       
#'### 4.2.3 Distribution et variation sur la période du SMPT net en EQTP         
#'     

#'       
#'#### 4.2.3.1 Pour l'ensemble des catégories statutaires             
#'     

#'**Fonctionnaires**    
#'&nbsp;*Tableau 4.2.3.1.1*   
#'    

essayer(label = "+distribution SMPT fonct.", print(distribution_smpt(Filtre_fonctionnaire)), "La distribution du salaire moyen par tête des fonctionnaires n'a pas pu être calculée.")

#'       
#'#### 4.2.3.2 Par catégorie statutaire             
#'     

#'**Categorie A**    
#'&nbsp;*Tableau 4.2.3.2.1*   
#'    

essayer(label = "+distribution SMPT cat. A", print(distribution_smpt(Filtre_cat_A)), "La distribution du salaire moyen par tête des fonctionnaires de catégorie A n'a pas pu être calculée.")

#'**Categorie B**  
#'&nbsp;*Tableau 4.2.3.2.2*   
#'    

essayer(label = "+distribution SMPT cat. B", print(distribution_smpt(Filtre_cat_B)), "La distribution du salaire moyen par tête des fonctionnaires de catégorie B n'a pas pu être calculée.")

#'**Categorie C**  
#'&nbsp;*Tableau 4.2.3.2.3*   
#'    

essayer(label = "+distribution SMPT cat. C", print(distribution_smpt(Filtre_cat_C)), "La distribution du salaire moyen par tête des fonctionnaires de catégorie C n'a pas pu être calculée.")

conditionnel("Lien vers la base de données", "Bases/Remunerations/Analyse.variations.par.exercice.csv")     

########### 4.3  RMPP et effet de noria ###########  

#'## 4.3 Rémunération moyenne des personnes en place (RMPP) et effet de noria
#'


##### 4.3.1 RMPP de l'ensemble des personnels titulaires et non-titulaires  ####


#'
#'### 4.3.1 RMPP de l'ensemble des personnels titulaires et non-titulaires   
#'   

#'### Application de filtres sur les données        
#'     
#'**Afin d'apprécier la sensibilité des résultats à la qualité ou aux valeurs extrêmes des données, le filtrage suivant est à présent appliqué.**    
#'**Sont retirés les valeurs manquantes des variations, les centiles extrêmaux, les rémunérations nettes négatives (rappels) ou proche de zéro.**     
#'**Un statut explicite doit être renseigné en fin de période. Des rémunérations doivent être versées à la fois en début et en fin de période de paiement de l'agent, supérieures à `r minimum.positif`. Le nombre de jours d'exercice doit être supérieur à `r 2 * seuil.troncature` .**                 
#'**Ces filtres sont référencés ci-après par les termes "filtres sur RMPP".**          

# Appliquer les filtres maintenant

  if (nrow(Anavar.synthese.plus.2.ans) > 0 && durée.sous.revue > 1 ) {
    hist(Anavar.synthese.plus.2.ans$variation.moyenne.rémunération,
         xlab ="Variation annuelle moyenne en %",
         las = 1,
         xlim = c(-5,30),
         ylab ="Effectifs",
         main ="Rémunération nette des personnes en place",
         col ="blue",
         nclass=1000,
         xaxt = 'n')
    
    axis(side=1, at=seq(-5,30, 1), labels=seq(-5,30,1), lwd=2)
  }
    

#'*Cet histogramme décrit l'évolution de la rémunération moyenne des personnes en place (RMPP), définies comme présentes deux années entières consécutives avec la même quotite*   
#'*L'évolution de la RMPP permet d'étudier le glissement vieillesse-technicité "positif", à effectifs constants sur deux années*      


#'
#'**Variation individuelle de rémunération nette en EQTP pour les personnels présents sur toute la période**     
#'
#'&nbsp;*Tableau 4.3.1.1*
#'
if (durée.sous.revue > 1) {
  print(Résumé(c("Variation normalisée (%)",
           "Variation annuelle moyenne normalisée (%)",
           "Quotité",
           "Effectif"),
         Anavar.synthese[bitwAnd(indicatrice.période, masque.présent.sur.période) == masque.présent.sur.période
                         & Annee == début.période.sous.revue, .(variation.rémunération.normalisée,
                                                                variation.moyenne.rémunération.normalisée,
                                                                quotite.moyenne)],
         extra = "length"))
}

##### 4.3.2  RMPP des titulaires et stagiaires     ######   

#'    
#'### 4.3.2 RMPP des titulaires et stagiaires     
#'   


#'
#'**Variation individuelle de rémunération nette en EQTP pour les personnels présents sur toute la période**   
#'  
#'&nbsp;*Tableau 4.3.2.1*   
#'    

if (durée.sous.revue > 1) {
  print(Résumé(c("Variation normalisée (%)",
           "Variation annuelle moyenne normalisée (%)",
           "Quotité",
           "Effectif"),
         Anavar.synthese[statut %in% c("TITULAIRE", "STAGIAIRE")
                         & bitwAnd(indicatrice.période, masque.présent.sur.période)
                         ==
                           masque.présent.sur.période
                         & Annee == début.période.sous.revue,
                         .(variation.rémunération.normalisée, 
                           variation.moyenne.rémunération.normalisée,
                           quotite.moyenne)],
         extra = "length"))
}

#'
#'
conditionnel("Lien vers la base de données", "Bases/Remunerations/Anavar.synthese.csv")
#'
#'**Nota**   
#'*Personnes en place :* en fonction au moins deux années consécutives avec la même quotite sur la période `r début.période.sous.revue` à `r fin.période.sous.revue`    
#'*Variation sur la période d'activité :* entre l'arrivée et le départ de la personne      
#'*Variation normalisée :* conforme à la définition INSEE (présente en début et en fin de période avec la même quotite)  
#'  
#'**Commentaire**       
#'Les différences éventuelles constatées entre l'évolution de la RMPP au tableau `r numéro.tableau-2` sont dues soit à l'effet de noria soit à l'effet périmètre.    
#'      

detach(Analyse.variations)
