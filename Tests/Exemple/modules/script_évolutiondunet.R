########### 4. Analyse dynamique des rémunérations ########################
#'
#'# 4. Rémunérations nettes : évolutions sur la période `r début.période.sous.revue` - `r fin.période.sous.revue`    
#'
#'Nombre d'exercices: `r durée.sous.revue`   
#'
#'**Périmètre des données**      
#'**Les données présentées dans cette section sont toutes relatives à des rémunérations nettes en équivalent temps plein (EQTP)**           
#'**Les élus, les vacataires et les assistantes maternelles ont été retirés de la population étudiée**       
#'**Seuls sont considérés les postes actifs et non annexes**      
#'    
#'*Nota :*   
#'*EQTP = Equivalent temps plein  = 12 . moyenne du ratio rémunération / quotite*    

########### 4.1 Distribution de la rémunération nette moyenne ###########

insérer_script(file.path(chemin.modules, "script_rémunérationsnettes.R"))

#'   
#+ Effet-de-noria-ensemble

####### 4.3.3 Effet de noria ####

insérer_script(file.path(chemin.modules, "script_rmppetnoria.R"))

####### 4.4 Comparaisons nationales ####  

conditionnel("Lien vers la base de données", "Bases/Remunerations/Anavar.synthese.csv")
#'
#'
#'### 4.4 Comparaisons avec la situation nationale des rémunérations   
#'  
#'**Évolution en euros courants du SMPT et de la RMPP dans la FPT (en % et euros courants)**    

#'  
#'&nbsp;*Tableau 4.4.1*   
#'    

#' 
#'| Annee      | 2009 | 2010 | 2011 | 2012 | 2013 | 2014 | 2015 | 2016  |   
#'|:----------:|-----:|-----:|-----:|-----:|-----:|-----:|-----:|------:|   
#'| SMPT brut  | 2,5  |  1,3 |  1,5 |  1,7 |  1,1 |  1,7 |  1,2 | 0,9   |   
#'| SMPT net   | 3,0  |  1,4 |  1,3 |  1,4 |  0,8 |  1,3 |  0,8 | 0,6   |   
#'| RMPP brute | 3,3  |  2,5 |  2,5 |  2,7 |  1,9 |  3,0 |  2,1 | 1,7   |  
#'| RMPP nette | 3,3  |  2,5 |  2,3 |  2,4 |  1,6 |  2,7 |  1,7 | 1,3   |  
#'    

#'*Source : fichier général de l'État (FGE), DADS, SIASP, Insee, Drees. Traitement Insee, Drees, DGCL*    
#'Hors assistants maternels et familiaux, y compris bénéficiaires de contrats aidés.   
#'Lecture : en 2014, le SMPT brut en EQTP a augmenté de 1,7 %
#'SMPT : Salaire moyen par tête en EQTP.   
#'RMPP : Agents présents 24 mois consécutifs chez le même employeur avec la même quotite de travail.   
#'Lecture : en 2014, la rémunération nette en EQTP des agents présents deux années consécutives en 2012 et 2013 avec la même quotite a augmenté de 2,7 %   
#'  
#'**Salaires nets annuels et évolution moyenne type de collectivité en euros courants  EQTP**    
#'   
#'  
#'&nbsp;*Tableau 4.4.2*   
#'    

#' 
#'|  Organisme   SMPT net     |  2011  | 2012     |    2013 |  2014  |  2015 |  2016    |
#'|:-------------------------:|-------:|---------:|--------:|-------:|------:|---------:|
#'| Communes                  | 20 784 |  21 120  | 21 096  | 21 444 |  21 552  | 21 632  | 
#'| CCAS et caisses des écoles| 19 415 |  19 716  | 19 788  | 20 124 |  20 232  | 20 370  | 
#'| EPCI à fiscalité propre   | 22 882 |  23 088  | 23 184  | 23 412 |  23 424  | 23 754  |
#'| Autres structures intercommunales |   21 299 | 21 684 | 21 828 | 22 140 | 22 332   | 22 517 |
#'|   Départements            | 24 487 |  24 744  | 24 852  | 25 068 |  25 344  | 25 391 |
#'|   SDIS                    | 29 811 |  29 940  | 30 180  | 30 480 |  30 912  | 31 147 |
#'|  Régions                  | 22 432 |  22 836  | 23 004  | 23 484 |  23 808  | 24 284 |
#'| Autres collectivités locales  | 24 680  | 24 696  | 24 828  | 25 032 | 25 368   | 25 456  |
#'| Ensemble (moyenne)        | 21 873 | 22 176   | 22 212  | 22 524 |  22 692  | 22 819  |
#' 

#'**RMPP nette (salariés présents deux années de suite avec la même quotite) en EQTP**        
#'    
#' 
#'|  Organisme      RMPP net          |  2014  | 2014-2015 (%)  | 2015-2016 (%) |
#'|:---------------------------------:|-------:|------:|------: |
#'| Communes                          | 22 524 |  1,5 | 1,1     |
#'| CCAS et caisses des écoles        | 21 420 |  1,6 | 1,1     |
#'| EPCI à fiscalité propre           | 24 864 |  1,9 | 1,6     |
#'| Autres structures intercommunales | 23 988 |  2,1 | 1,7     |
#'|   Départements                    | 25 932 |  1,9 | 1,3     |
#'|   SDIS                            | 31 032 |  2,6 | 1,5     |
#'|  Régions                          | 24 240 |  2,1 | 1,3     |
#'| Autres collectivités locales      | 21 873 |  2,0 | 1,7     |
#'|  Ensemble (moyenne)               | 23 760 |  1,7 | 1,3     | 
#'     
#'   
#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			
#'Conversion en euros courants, calcul CRC.  La métropole de Lyon est classée avec les départements   


#'[Source RAEFP 2016 données 2014](Docs/RA_2016.pdf)      
#'[Source RAEFP 2017 données 2015](Docs/RA_2017.pdf)      
#'[Source RAEFP 2018 données 2016](Docs/RA_2018.pdf)  
#'[Source INSEE 2016](Docs/insee-premiere1616.pdf)      

incrémenter.chapitre()

newpage()
