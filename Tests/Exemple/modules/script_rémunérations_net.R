#'    
#'## `r chapitre`.1 Distribution de la rémunération nette moyenne sur la période    
#+ remuneration-nette

Analyse.variations <- Analyse.variations[nb.jours > seuil.troncature
                                         & ! is.na(Montant.net.annuel.eqtp)
                                         & Montant.net.annuel.eqtp  > minimum.positif 
                                         & ! is.na(Statut)] 

attach(Analyse.variations, warn.conflicts = FALSE)

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
#'[Lien vers la base de données synthétique](Bases/Remunerations/Analyse.variations.par.exercice.csv)     
#'[Lien vers la base de données détaillée par année](Bases/Remunerations/Analyse.variations.par.exercice.csv)     

########### 4.2  Evolutions des rémunérations nettes ###########

#'## `r chapitre`.2 Evolutions du SMPT sur la période `r début.période.sous.revue` - `r fin.période.sous.revue`   
#'
#'### `r chapitre`.2.1 Evolution du SMPT pour l'ensemble des personnels fonctionnaires et non titulaires (hors élus)        
#'
#+ remuneration-nette-evolution

masse.salariale.nette <- rep(0, durée.sous.revue)

# sommation sur les matricules à année fixe 

#'    
#'**Salaire net moyen par tête (SMPT net) en EQTP, hors élus**         
#'       
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

#+ Salaire-moyen-par-tete    


smpt <- function(Filtre, type =  "smpt net") {
  
  S_net.eqtp <- Analyse.variations[Filtre() == TRUE,
                                   .(moy = weighted.mean(Montant.net.annuel.eqtp, quotité.moyenne, na.rm = TRUE)),
                                   by = "Année"]
  
  S_net.eqtp.100 <- Analyse.variations[Filtre() == TRUE & temps.complet == TRUE & permanent == TRUE,
                                       .(moy = weighted.mean(Montant.net.annuel.eqtp, quotité.moyenne, na.rm = TRUE)),
                                       by = "Année"]
  
  
  f <- function(x) prettyNum(S_net.eqtp[Année == x, moy],
                             big.mark = " ",
                             digits = 1,
                             format = "fg")
  
  g <- function(x) prettyNum(S_net.eqtp.100[Année == x, moy],
                             big.mark = " ",
                             digits = 1,
                             format = "fg")
  
  T <- Tableau.vertical(c("Année", type %+% " (euros)", type %+% " temps complet (euros)"),
                        if (type == "smpt net") période else période[2:durée.sous.revue],           # if...else pas ifelse (dim vecteur)
                        extra = "variation",
                        f,
                        g)
  
  #if (! is.null(T) & T != "") 
  try(print(T))
  
  cat("\n\n")  
}

distribution_smpt <- function(Filtre) {
  
  print(Résumé(c(début.période.sous.revue, "Effectif",
                 début.période.sous.revue %+% " TC", "Effectif",
                 fin.période.sous.revue, "Effectif",
                 fin.période.sous.revue %+% " TC",  "Effectif"),
               list(
                 Analyse.variations[Année == début.période.sous.revue
                                    & Filtre() == TRUE,
                                    .(Montant.net.annuel.eqtp, quotité.moyenne)],   
                 Analyse.variations[Année == début.période.sous.revue
                                    & Filtre() == TRUE
                                    & permanent == TRUE
                                    & temps.complet == TRUE,
                                    .(Montant.net.annuel.eqtp, quotité.moyenne)],
                 Analyse.variations[Année == fin.période.sous.revue 
                                    & Filtre() == TRUE,
                                    .(Montant.net.annuel.eqtp, quotité.moyenne)],
                 Analyse.variations[Année == fin.période.sous.revue 
                                    & Filtre() == TRUE
                                    & permanent == TRUE
                                    & temps.complet == TRUE,
                                    .(Montant.net.annuel.eqtp, quotité.moyenne)]),
               extra = "length"))
  
  # Pour des raisons très mal comprises, print est ici nécessaire alors qu'il ne l'est pas dans smpt() pour Tableau_vertical ;
  # pourtant les deux fonctions sont basées sur kable()
  
}

Filtre_neutre <<- function() TRUE   

essayer(label = "+SMPT global", smpt(Filtre_neutre),     "Le salaire moyen par tête n'a pas pu être calculé.")    

#'**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         
#'**pour les salariés à temps complet**           
#'  
#'&nbsp;*Tableau `r incrément()`*     
#'    

#+ premiere-derniere-annee

essayer(label = "+distribution SMPT global", distribution_smpt(Filtre_neutre), "La distribution du salaire moyen par tête n'a pas pu être calculée.")

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
#'&nbsp;*Tableau `r incrément()`*       

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
#'&nbsp;*Tableau `r incrément()`*   
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
#'&nbsp;*Tableau `r incrément()`*   
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
#'&nbsp;*Tableau `r incrément()`*   
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
#'### `r chapitre`.2.2 Evolution du SMPT des fonctionnaires         
#'   
#'##### `r chapitre`.2.2.1 Toutes catégories statutaires          

#'**Salaire net moyen par tête (SMPT net) en EQTP**       
#'    
#'    
#'&nbsp;*Tableau `r incrément()`*   
#'    

Filtre_fonctionnaire <<- function() Statut == "TITULAIRE" | Statut == "STAGIAIRE"

essayer(label = "+SMPT fonct.", smpt(Filtre_fonctionnaire), "Le salaire moyen par tête des fonctionnaires n'a pas pu être calculé.")

Filtre_cat_A <<- function()   (Statut == "TITULAIRE"  | Statut == "STAGIAIRE")  & (Catégorie == "A")
Filtre_cat_B <<- function()   (Statut == "TITULAIRE"  | Statut == "STAGIAIRE")  & (Catégorie == "B") 
Filtre_cat_C <<- function()   (Statut == "TITULAIRE"  | Statut == "STAGIAIRE")  & (Catégorie == "C") 

#'    
#'##### `r chapitre`.2.2.2 Par catégorie  statutaire        
#'   
#'**Catégorie A**  
#'
#'&nbsp;*Tableau `r incrément()`*   
#'    
#'  


essayer(label = "+SMPT cat A", smpt(Filtre_cat_A), "Le salaire moyen par tête des fonctionnaires de catégorie A n'a pas pu être calculé.")  

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
#'**Catégorie B**  
#'   
#'   
#'&nbsp;*Tableau `r incrément()`*   
#'    
#'  

essayer(label = "+SMPT cat B", smpt(Filtre_cat_B),          "Le salaire moyen par tête des fonctionnaires de catégorie B n'a pas pu être calculé.")  

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
#'**Catégorie C**  
#'
#'&nbsp;*Tableau `r incrément()`*    
#'    
#'  

essayer(label = "+SMPT cat C", smpt(Filtre_cat_C),       "Le salaire moyen par tête des fonctionnaires de catégorie C n'a pas pu être calculé.")    

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
#'### `r chapitre`.2.3 Distribution et variation sur la période du SMPT net en EQTP         
#'     

#'       
#'#### `r chapitre`.2.3.1 Pour l'ensemble des catégories statutaires             
#'     

#'**Fonctionnaires**    
#'&nbsp;*Tableau `r incrément()`*   
#'    

essayer(label = "+distribution SMPT fonct.", distribution_smpt(Filtre_fonctionnaire), "La distribution du salaire moyen par tête des fonctionnaires n'a pas pu être calculée.")

#'       
#'#### `r chapitre`.2.3.2 Par catégorie statutaire             
#'     

#'**Catégorie A**    
#'&nbsp;*Tableau `r incrément()`*   
#'    

essayer(label = "+distribution SMPT cat. A", distribution_smpt(Filtre_cat_A), "La distribution du salaire moyen par tête des fonctionnaires de catégorie A n'a pas pu être calculée.")

#'**Catégorie B**  
#'&nbsp;*Tableau `r incrément()`*   
#'    

essayer(label = "+distribution SMPT cat. B",distribution_smpt(Filtre_cat_B), "La distribution du salaire moyen par tête des fonctionnaires de catégorie B n'a pas pu être calculée.")

#'**Catégorie C**  
#'&nbsp;*Tableau `r incrément()`*   
#'    

essayer(label = "+distribution SMPT cat. C", distribution_smpt(Filtre_cat_C), "La distribution du salaire moyen par tête des fonctionnaires de catégorie C n'a pas pu être calculée.")

#'[Lien vers la base de données](Bases/Remunerations/Analyse.variations.par.exercice.csv)     

########### 4.3  RMPP et effet de noria ###########  

#'## `r chapitre`.3 Rémunération moyenne des personnes en place (RMPP) et effet de noria
#'


##### 4.3.1 RMPP de l'ensemble des personnels titulaires et non-titulaires  ####

#'### Application de filtres sur les données        
#'     
#'**Afin d'apprécier la sensibilité des résultats à la qualité ou aux valeurs extrêmes des données, le filtrage suivant est à présent appliqué.**    
#'**Sont retirés les valeurs manquantes des variations, les centiles extrêmaux, les rémunérations nettes négatives (rappels) ou proche de zéro.**     
#'**Un statut explicite doit être renseigné en fin de période. Des rémunérations doivent être versées à la fois en début et en fin de période de paiement de l'agent, supérieures à `r minimum.positif`. Le nombre de jours d'exercice doit être supérieur à `r 2 * seuil.troncature` .**                 
#'**Ces filtres sont référencés ci-après par les termes "filtres sur RMPP".**          
#'
#'### `r chapitre`.3.1 RMPP de l'ensemble des personnels titulaires et non-titulaires   
#'   
#'*Cette section est consacrée à la rémunération moyenne des personnes en place (RMPP), définies comme présentes deux années entières consécutives avec la même quotité*   
#'*L'évolution de la RMPP permet d'étudier le glissement vieillesse-technicité "positif", à effectifs constants sur deux années*      

# Appliquer les filtres maintenant


essayer(label = "+RMPP nette globale", {
  
  q3 <- quantile(Analyse.variations$variation.rémunération, c(quantile.cut/100, 1 - quantile.cut/100), na.rm=TRUE)
  
  # Filtrage : on enlève les personnels présents depuis moins d'un seuil de troncature (ex. 120 jours) dans l'année et les élus
  # (paramètre seuil.troncature) 
  
  # Filtrage pour l'étude des variations : on enlève les valeurs manquantes des variations, les centiles extrêmaux,
  # les rémunérations nettes négatives ou proche de zéro. On exige un statut explicite en fin de période.
  # Paramétrable par :
  # minimum.positif, quantile.cut 
  
  # ici il faut réduire la matrice pour éviter les réduplications pour les Résumés. TODO
  
  Anavar.synthese <- Analyse.variations[total.jours > seuil.troncature
                                        & pris.en.compte == TRUE
                                        & ! is.na(statut)   
                                        & ! is.na(variation.rémunération) 
                                        & variation.rémunération > q3[[1]]
                                        & variation.rémunération < q3[[2]]]
  
  Anavar.synthese.plus.2.ans  <- Anavar.synthese[! is.na(plus.2.ans) & plus.2.ans == TRUE]
  Anavar.synthese.moins.2.ans <- Anavar.synthese[! is.na(plus.2.ans) & plus.2.ans == FALSE]
  
  #Analyse.variations.par.exercice <- Analyse.variations.par.exercice[Nexercices > 1]
  
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
    
    try(axis(side=1, at=seq(-5,30, 1), labels=seq(-5,30,1), lwd=2))
    
    Filtre_rmpp <<- function() (est.rmpp == TRUE)
  }
}, "La RMPP nette globale n'a pas pu être calculée.")

# #'   
# #'**Evolution de la RMPP nette en EQTP**     
# #'   
# #'#### La RMPP est évaluée sans filtrage particulier sur les données dans le tableau suivant      
# #'  
# #'&nbsp;*Tableau `r incrément()`*   
# #'    
# 
# if (durée.sous.revue > 1) {
#    essayer(smpt(Filtre_rmpp, type = "RMPP nette"), "La rémunération moyenne des personnels en place n'a pas pu être calculée.") 
#   } else  {
#    cat("RMPP calculable uniquement si la période sous revue est au moins égale à 2 ans.")
#   }
# 
#  
# #'    
# #'**Distribution et variation sur la période de la rémunération nette des personnes en place**                   
# #'  
# #'&nbsp;*Tableau `r incrément()`*     
# #'    
# 
# # La légère différence de périmètre entre Anavar.synthese et Analyse.variations tient au filtrage des quantiles
# # extrêmaux et des valeurs manquantes des variations


#masque.rmpp.fin.période    <- bitwShiftL(3, durée.sous.revue - 2)      #  11{0,1}...{0,1}
#masque.rmpp.début.période  <- 3                                        #  {0,1}...{0,1}11
masque.présent.début.fin   <- bitwShiftL(1, durée.sous.revue - 1) + 1  #  10000..1
masque.présent.sur.période <- bitwShiftL(1, durée.sous.revue) - 1      #  11111..1

# if (durée.sous.revue > 1) {
# 
#   Résumé(c(paste(début.période.sous.revue, début.période.sous.revue + 1, sep = "-"),
#            "Effectif",
#            paste(fin.période.sous.revue - 1, fin.période.sous.revue, sep = "-"),
#            "Effectif"),
#          list(Anavar.synthese[bitwAnd(indicatrice.période, masque.rmpp.début.période) == masque.rmpp.début.période 
#                                           & Année == début.période.sous.revue + 1, 
#                                              .(Montant.net.annuel.eqtp.début, quotité.moyenne)],
#               Anavar.synthese[indicatrice.période >= masque.rmpp.fin.période
#                                           & Année == fin.période.sous.revue, 
#                                           .(Montant.net.annuel.eqtp.sortie, quotité.moyenne)]),
#           extra = "length")
#   
# } else  {
#   cat("Distribution de la RMPP calculable uniquement si la période sous revue est au moins égale à 2 ans.")
# }

# #'*Note :*    
# #'*RMPP en `r début.période.sous.revue + 1` des personnes en place en `r début.période.sous.revue`-`r début.période.sous.revue + 1`*     
# #'*RMPP en `r fin.période.sous.revue` des personnes en place en `r fin.période.sous.revue - 1`-`r fin.période.sous.revue `*         
#'    
#'    
#'**Variation individuelle de rémunération nette en EQTP pour les personnels présents la première et la dernière année**   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

if (durée.sous.revue > 1) {
  Résumé(c("Variation normalisée (%)",
           "Variation annuelle moyenne normalisée (%)",
           "Quotité",
           "Effectif"),
         Anavar.synthese[bitwAnd(indicatrice.période, masque.présent.début.fin) 
                         == 
                           masque.présent.début.fin
                         & Année == début.période.sous.revue,
                         .(variation.rémunération.normalisée,
                           variation.moyenne.rémunération.normalisée,
                           quotité.moyenne)],
         extra = "length")
}


#'
#'**Variation individuelle de rémunération nette en EQTP pour les personnels présents sur toute la période**     
#'
#'&nbsp;*Tableau `r incrément()`*
#'

Résumé(c("Variation normalisée (%)",
         "Variation annuelle moyenne normalisée (%)",
         "quotité moyenne",
         "Effectif"),
       Anavar.synthese[indicatrice.période == masque.présent.sur.période
                       & Année == début.période.sous.revue, .(variation.rémunération.normalisée,
                                                              variation.moyenne.rémunération.normalisée,
                                                              quotité.moyenne)],
       extra = "length")


##### 4.3.2  RMPP des titulaires et stagiaires     ######   

#'    
#'### `r chapitre`.3.2 RMPP des titulaires et stagiaires     
#'   

Filtre_rmpp_fonctionnaire <<- function () Filtre_fonctionnaire() & (est.rmpp == TRUE)

# #'   
# #'**Evolution de la RMPP nette en EQTP**     
# #'   
# #'  
# #'&nbsp;*Tableau `r incrément()`*   
# #'    
# if (durée.sous.revue > 1)
#     essayer(smpt(Filtre_rmpp_fonctionnaire, type = "RMPP nette"), 
#             "La RMPP nette des fonctionnaires n'a pas pu être calculée")
# 
# #'    
# #'**Distribution et variation sur la période de la rémunération nette des fonctionnaires en place**                
# #'  
# #'#### Les filtres sur RMPP sont à présent appliqués (voir 4.3.1)      
# #'  
# #'&nbsp;*Tableau `r incrément()`*     
# #'    
# #'  
# if (durée.sous.revue > 1) {
#   
#   Résumé(c(paste(début.période.sous.revue, début.période.sous.revue + 1, sep="-"),
#            "Effectif",
#            paste(fin.période.sous.revue - 1, fin.période.sous.revue, sep="-"),
#            "Effectif"),
#          list(Anavar.synthese[(statut == "TITULAIRE" | statut == "STAGIAIRE")
#                                            & bitwAnd(indicatrice.période, masque.rmpp.début.période) == masque.rmpp.début.période
#                                            & Année == début.période.sous.revue + 1,  
#                                           .(Montant.net.annuel.eqtp.début, quotité.moyenne)],
#               Anavar.synthese[(statut == "TITULAIRE" | statut == "STAGIAIRE")
#                                           & indicatrice.période >= masque.rmpp.fin.période
#                                           & Année == fin.période.sous.revue, 
#                                           .(Montant.net.annuel.eqtp.sortie, quotité.moyenne)]),
#          extra = "length")
#   
# }

# #'*Note :*   
# #'*RMPP en `r début.période.sous.revue + 1` des personnes en place en `r début.période.sous.revue`-`r début.période.sous.revue + 1`*     
# #'*RMPP en `r fin.période.sous.revue` des personnes en place en `r fin.période.sous.revue - 1`-`r fin.période.sous.revue `*         
# #'   

#'
#'**Variation individuelle de rémunération nette en EQTP pour les personnels présents la première et la dernière année**   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

if (durée.sous.revue > 1) {
  Résumé(c("Variation normalisée (%)",
           "Variation annuelle moyenne normalisée (%)",
           "Quotité",
           "Effectif"),
         Anavar.synthese[(statut == "TITULAIRE" | statut == "STAGIAIRE")
                         & bitwAnd(indicatrice.période, masque.présent.début.fin)
                         ==
                           masque.présent.début.fin
                         & Année == début.période.sous.revue,
                         .(variation.rémunération.normalisée, 
                           variation.moyenne.rémunération.normalisée,
                           quotité.moyenne)],
         extra = "length")
}


#'
#'
#'[Lien vers la base de données](Bases/Remunerations/Anavar.synthese.csv)
#'
#'**Nota**   
#'*Personnes en place :* en fonction au moins deux années consécutives avec la même quotité sur la période `r début.période.sous.revue` à `r fin.période.sous.revue`    
#'*Variation sur la période d'activité :* entre l'arrivée et le départ de la personne      
#'*Variation normalisée :* conforme à la définition INSEE (présente en début et en fin de période avec la même quotité)  
#'  
#'**Commentaire**       
#'Les différences éventuelles constatées entre l'évolution de la RMPP au tableau `r numéro.tableau-2` sont dues soit à l'effet de noria soit à l'effet périmètre.    
#'      

detach(Analyse.variations)
