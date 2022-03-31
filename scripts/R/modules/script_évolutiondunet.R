########### 4. Analyse dynamique des rémunérations ########################

#'
#'# 4. Rémunérations nettes : évolutions sur la periode sous revue
#'   
cat("**Nombre d'exercices: ", duree.sous.revue, "**\n")   
#'   
#'**Périmètre des données**      
#'**Les données présentées dans cette section sont toutes relatives à des rémunérations nettes en équivalent temps plein (EQTP)**           
#'**Les élus, les vacataires et les assistantes maternelles ont été retirés de la population étudiée**       
#'**Seuls sont considérés les postes actifs et non annexes**      
#'    
#'*Nota :*   
#'*Rémunération annualisée en EQTP (Equivalent temps plein) : la rémunération annuelle équivalente pour un temps plein en année pleine est calculée pour chaque agent*   
#'*Chaque agent est rentre ensuite dans la somme avec la pondération correspondant à son temps de travail annuel. *   

########### 4.1 Distribution de la rémunération nette moyenne ###########

#'    
#'## 4.1 Distribution de la rémunération nette moyenne sur la periode    
#+ remuneration-nette

attach(Analyse.variations, warn.conflicts = FALSE)

#+fig.height=6, fig.width=7   
inserer_script(fonction = "evolution_net_hist_1")

#'   

#+fig.height=6, fig.width=7   
inserer_script(fonction = "evolution_net_hist_2")
#'   
conditionnel("Lien vers la base de données synthétique", "Bases/Remunerations/Analyse.variations.csv")     
conditionnel("Lien vers la base de données détaillée par année", "Bases/Remunerations/Analyse.variations.par.exercice.csv")     

########### 4.2  Evolutions des rémunérations nettes ###########

#'## 4.2 Evolutions du SMPT sur la periode sous revue 
#'
#'### 4.2.1 Evolution du SMPT pour l'ensemble des personnels fonctionnaires et non titulaires (hors élus)        
#'
#+ remuneration-nette-evolution

masse.salariale.nette <- rep(0, duree.sous.revue)

# sommation sur les matricules à annee fixe 

#'    
#'**Salaire net moyen par tête (SMPT net) en EQTP, hors élus**         
#'       
#'  
#'&nbsp;*Tableau 4.2.1.1*   
#'    

#+ Salaire-moyen-par-tete    

L <- smpt()
essayer(label = "+SMPT global", if (! is.null(L)) print(L[[1]]) else cat("    \n"),
        "Le salaire moyen par tête n'a pas pu être calculé.")

#'**Distribution et variation sur la periode du salaire moyen net par tête (SMPT net) en EQTP**         
#'**pour les salariés à temps complet**           
#'  
#'&nbsp;*Tableau 4.2.1.2*     
#'    

#+ premiere-derniere-annee

essayer(label = "+distribution SMPT global", print(distribution_smpt(S_ = L[[2]])), "La distribution du salaire moyen par tête n'a pas pu être calculée.")

#'  
cat("*Nota :*  La population retenue est constituée des agents qui ne font pas partie des ", 2*quantile.cut, "centiles extrêmaux\n")   
#'Les élus, vacataires et assistantes maternelles sont retirés du périmètre.   
#'TC :  personnels à temps complet sur toute l'annee            
#'Seuls sont pris en compte les agents ayant connu au moins un mois actif et ayant eu, sur l'annee, des rémunérations non annexes.  
#'[Compléments méthodologiques](Docs/méthodologie.pdf)     
#'      


#'**Comparaisons source INSEE/DGCL**   
#'
#'**Salaires nets annuels moyens en EQTP (hors assistantes maternelles) dans la FPT**   
#'  
#'&nbsp;*Tableau 4.2.1.3*       

#### INSEE/DGCL   ####

message("smpt_net_fpt.csv")

Tableau.data("smpt_net_fpt.csv", 
             c("net (euros)", 2011, 2012,  2013,  2014, 2016, 2017, 2018, 2019))
             
#'   
#'   *Contractuels à partir de 2017
#'   
#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			

#'   
#'**Distribution des salaires nets annuels en EQTP dans la fonction publique territoriale (2011-2016)**   
#' 
#'  
#'&nbsp;*Tableau 4.2.1.4*   
#'    
# Remarque sur le formatage markdown: en raison d'un bug de knitr, utiliser un dièse + apostrophe
# suivi d'un seul blanc juste après la table.
   
message("distribution_smpt_net_fpt.csv")   
#'     
 Tableau.data("distribution_smpt_net_fpt.csv", 
              c("Décile \ euros   ", 2011, 2013, 2014, 2016, 2017, 2018, 2019))
#'    

#'**Distribution des salaires nets annuels en EQTP dans la fonction publique d'Etat (2011-2016)**   
#' 
#'  
#'&nbsp;*Tableau 4.2.1.5*   

message("distribution_smpt_net_fpe.csv")   
#'    

Tableau.data("distribution_smpt_net_fpe.csv", 
              c("Décile \ euros   ", 2011, 2013, 2016, 2018, 2019))
#

#'**Distribution des salaires nets annuels en EQTP dans la fonction publique hospitalière (hôpitaux) (2011-2016)**   

#'  
#'&nbsp;*Tableau 4.2.1.6*   
#'    
message("distribution_smpt_net_fph.csv")   
#'
Tableau.data("distribution_smpt_net_fph.csv", 
              c("Décile \ euros   ", 2011, 2013, 2016, 2017, 2018, 2019))
              
#'    

#'[Source INSEE, onglets Figure3, F1web et F3web - 2011](Docs/ip1486.xls)    
#'[Source INSEE, onglets F V3.1-2, F V3.1-5 - 2013](Docs/vue3_remunerations.xls)  
#'[Source INSEE, onglet v3-2, V3-5 2016](Docs/Vue-Remunerations-2018.xlsx)     
#'[Source INSEE, onglet v3-1, 2020](Docs/Vue3.1_Salaires-FP-2020.xlsx )

#'   
#'### 4.2.2 Evolution du SMPT des fonctionnaires         
#'   
#'##### 4.2.2.1 Toutes categories statutaires          

#'**Salaire net moyen par tête (SMPT net) en EQTP**       
#'    
#'    
#'&nbsp;*Tableau 4.2.2.1.1*   
#'    

L_fonct <- smpt(Liste_statuts = c("TITULAIRE", "STAGIAIRE"))

essayer(label = "+SMPT fonct.",
        if (! is.null(L_fonct)) print(L_fonct[[1]]) else cat("    \n"),
       "Le salaire moyen par tête des fonctionnaires n'a pas pu être calculé.")

#'    
#'##### 4.2.2.2 Par categorie  statutaire        
#'   
#'**Categorie A**  
#'
#'&nbsp;*Tableau 4.2.2.2.1*   
#'    
#' *Les statuts d'emplois fonctionnels sont pris en compte dans le calcul.*
#'
L_cat_A <- smpt(Cat = "A",
                Liste_statuts = c("TITULAIRE", "STAGIAIRE", "EMPLOI_FONC", "EMPLOI_FONCTIONNEL"))

essayer(label = "+SMPT cat A",
        if (! is.null(L_cat_A)) print(L_cat_A[[1]]) else cat("    \n"),
        "Le salaire moyen par tête des fonctionnaires de categorie A n'a pas pu être calculé.")

#'  
#'*Comparaisons nationales*    
#'*FPT categorie A*    
#'    
message("distribution_smpt_net_fpt_cat_A.csv")   
Tableau.data("distribution_smpt_net_fpt_cat_A.csv", 
              c("Décile \ euros   ", 2011, 2013, 2014, 2016, 2018, 2019))

#'   

#'   
#'**Categorie B**  
#'   
#'   
#'&nbsp;*Tableau 4.2.2.2.2*   
#'    
#'  

L_cat_B <- smpt(Cat = "B",
                Liste_statuts = c("TITULAIRE", "STAGIAIRE"))

essayer(label = "+SMPT cat B",
        if (! is.null(L_cat_B)) print(L_cat_B[[1]]) else cat("    \n"),
        "Le salaire moyen par tête des fonctionnaires de categorie B n'a pas pu être calculé.")

#'*Comparaisons nationales*    
#'*FPT categorie B*    
#'    
message("distribution_smpt_net_fpt_cat_B.csv")   
#'  
Tableau.data("distribution_smpt_net_fpt_cat_B.csv", 
              c("Décile \ euros   ", 2011, 2013, 2014, 2016, 2018, 2019))

#'  

#'   
#'**Categorie C**  
#'
#'&nbsp;*Tableau 4.2.2.2.3*    
#'    
#'  

L_cat_C <- smpt(Cat = "C",
                Liste_statuts = c("TITULAIRE", "STAGIAIRE"))

essayer(label = "+SMPT cat C",
        if (! is.null(L_cat_C)) print(L_cat_C[[1]]) else cat("    \n"),
       "Le salaire moyen par tête des fonctionnaires de categorie C n'a pas pu être calculé.")

#'     
#'*Comparaisons nationales*    
#'*FPT categorie C*    
#'   
message("distribution_smpt_net_fpt_cat_C.csv")   
#'    
Tableau.data("distribution_smpt_net_fpt_cat_C.csv", 
              c("Décile \ euros   ", 2011, 2013, 2014, 2016, 2018, 2019))

#'    
#'       
#'### 4.2.3 Distribution et variation sur la periode du SMPT net en EQTP         
#'     

#'       
#'#### 4.2.3.1 Pour l'ensemble des categories statutaires             
#'     

#'**Fonctionnaires**    
#'&nbsp;*Tableau 4.2.3.1.1*   
#'    

essayer(label = "+distribution SMPT fonct.",
        print(distribution_smpt(S_ = L_fonct[[2]])),
        "La distribution du salaire moyen par tête des fonctionnaires n'a pas pu être calculée.")

#'       
#'#### 4.2.3.2 Par categorie statutaire             
#'     

#'**Categorie A**    
#'&nbsp;*Tableau 4.2.3.2.1*   
#'    

essayer(label = "+distribution SMPT cat. A",
        print(distribution_smpt(S_ = L_cat_A[[2]])),
        "La distribution du salaire moyen par tête des fonctionnaires de categorie A n'a pas pu être calculée.")

#'**Categorie B**  
#'&nbsp;*Tableau 4.2.3.2.2*   
#'    

essayer(label = "+distribution SMPT cat. B",
        print(distribution_smpt(S_ = L_cat_B[[2]])),
        "La distribution du salaire moyen par tête des fonctionnaires de categorie B n'a pas pu être calculée.")

#'**Categorie C**  
#'&nbsp;*Tableau 4.2.3.2.3*   
#'    

essayer(label = "+distribution SMPT cat. C",
        print(distribution_smpt(S_ = L_cat_C[[2]])),
        "La distribution du salaire moyen par tête des fonctionnaires de categorie C n'a pas pu être calculée.")

#'    
#'    
conditionnel("Lien vers la base de données", "Bases/Remunerations/Analyse.variations.par.exercice.csv")     
#'   

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
cat("**Un statut explicite doit être renseigné en fin de periode. Des rémunérations doivent être versées à la fois en début et en fin de periode de paiement de l'agent, supérieures à ", minimum.positif, ". Le nombre de jours d'exercice doit être supérieur à ", 2 * seuil.troncature, ".**\n")                 
#'**Ces filtres sont référencés ci-après par les termes "filtres sur RMPP".**          

# Appliquer les filtres maintenant

if (nrow(Anavar.synthese.plus.2.ans) > 0 && duree.sous.revue > 1 ) {
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


#'*Cet histogramme décrit l'évolution de la rémunération moyenne des personnes en place (RMPP), définies comme présentes deux annees entières consécutives avec la même quotité*   
#'*L'évolution de la RMPP permet d'étudier le glissement vieillesse-technicité "positif", à effectifs constants sur deux années*      


#'
#'**Variation individuelle de rémunération nette en EQTP pour les personnels présents sur toute la periode**     
#'
#'&nbsp;*Tableau 4.3.1.1*
#'
if (duree.sous.revue > 1) {
  print(Resume(c("Variation normalisée (%)",
                 "Variation annuelle moyenne normalisée (%)",
                 "Quotité",
                 "Effectif"),
               Anavar.synthese[bitwAnd(indicatrice.periode, masque.présent.sur.periode) == masque.présent.sur.periode
                               & Annee == debut.periode.sous.revue, .(variation.rémunération.normalisée,
                                                                      variation.moyenne.rémunération.normalisée,
                                                                      quotite.moyenne)],
               extra = "length"))
}

##### 4.3.2  RMPP des titulaires et stagiaires     ######   

#'    
#'### 4.3.2 RMPP des titulaires et stagiaires     
#'   


#'
#'**Variation individuelle de rémunération nette en EQTP pour les personnels présents sur toute la periode**   
#'  
#'&nbsp;*Tableau 4.3.2.1*   
#'    

if (duree.sous.revue > 1) {
  print(Resume(c("Variation normalisée (%)",
                 "Variation annuelle moyenne normalisée (%)",
                 "Quotité",
                 "Effectif"),
               Anavar.synthese[statut.fin.annee %in% c("TITULAIRE", "STAGIAIRE")
                               & bitwAnd(indicatrice.periode, masque.présent.sur.periode)
                               ==
                                 masque.présent.sur.periode
                               & Annee == debut.periode.sous.revue,
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
cat("*Personnes en place :* en fonction au moins deux années consécutives avec la même quotité sur la periode", debut.periode.sous.revue, " à ", fin.periode.sous.revue, "\n")    
#'*Variation sur la periode d'activité :* entre l'arrivée et le départ de la personne      
#'*Variation normalisée :* conforme à la définition INSEE (présente en début et en fin de période avec la même quotité)  
#'  
#'**Commentaire**       
cat("Les différences éventuelles constatées entre l'évolution de la RMPP au tableau ", numero.tableau-2, "sont dues soit à l'effet de noria soit à l'effet périmètre.\n")    
#'      

detach(Analyse.variations)

conditionnel("Lien vers la base de données", "Bases/Remunerations/Anavar.synthese.csv")  
