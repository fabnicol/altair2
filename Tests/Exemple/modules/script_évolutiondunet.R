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
#'*EQTP = Equivalent temps plein  = 12 . moyenne du ratio rémunération / quotité*    

########### 4.1 Distribution de la rémunération nette moyenne ###########

insérer_script(file.path(chemin.modules, "script_rémunérationsnettes.R"))

#'   
#+ Effet-de-noria-ensemble

####### 4.3.3 Effet de noria ####

insérer_script(file.path(chemin.modules, "script_rmppetnoria.R"))

####### 4.4 Comparaisons nationales ####  

#'[Lien vers la base de données](Bases/Remunerations/Anavar.synthese.csv)
#'
#'
#'### 4.4 Comparaisons avec la situation nationale des rémunérations   
#'  
#'**Évolution en euros courants du SMPT et de la RMPP dans la FPT (en % et euros courants)**    

#'  
#'&nbsp;*Tableau 4.4.1*   
#'    

#' 
#'| Année      | 2009 | 2010 | 2011 | 2012 | 2013 | 2014 | 
#'|:----------:|-----:|-----:|-----:|-----:|-----:|-----:|
#'| SMPT brut  | 2,5  |  1,3 |  1,5 |  1,7 |  1,1 |  1,7 |
#'| SMPT net   | 3,0  |  1,4 |  1,3 |  1,4 |  0,8 |  1,3 |
#'| RMPP brute | 3,3  |  2,5 |  2,5 |  2,7 |  1,9 |  3,0 |
#'| RMPP nette | 3,3  |  2,5 |  2,3 |  2,4 |  1,6 |  2,7 |
#' 

#'*Source : fichier général de l'État (FGE), DADS, SIASP, Insee, Drees. Traitement Insee, Drees, DGCL*    
#'Hors assistants maternels et familiaux, y compris bénéficiaires de contrats aidés.   
#'Lecture : en 2014, le SMPT brut en EQTP a augmenté de 1,7 %
#'SMPT : Salaire moyen par tête en EQTP.   
#'RMPP : Agents présents 24 mois consécutifs chez le même employeur avec la même quotité de travail.   
#'Lecture : en 2014, la rémunération nette en EQTP des agents présents deux années consécutives en 2012 et 2013 avec la même quotité a augmenté de 2,7 %   
#'  
#'**Salaires nets annuels et évolution moyenne type de collectivité en euros courants  EQTP**    
#'   
#'  
#'&nbsp;*Tableau 4.4.2*   
#'    

#' 
#'|  Organisme   SMPT net     |  2011  | 2012     |    2013 |  2014  | 2007-2011 (%) | 2011-2014 (%) | 
#'|:-------------------------:|-------:|---------:|--------:|-------:|----:|-----:|       
#'| Communes                  | 20 784 |  21 120  | 21 096  | 21 444 | 2,5 |  3,2 | 
#'| CCAS et caisses des écoles| 19 415 |  19 716  | 19 788  | 20 124 | 2,4 |  3,7 |
#'| EPCI à fiscalité propre   | 22 882 |  23 088  | 23 184  | 23 412 | 3,1 |  2,3 |
#'| Autres structures intercommunales |   21 299 | 21 684 | 21 828 | 22 140 | 3,0 | 3,9  |
#'|   Départements            | 24 487 |  24 744  | 24 852  | 25 068 | 3,9 |  2,4 |
#'|   SDIS                    | 29 811 |  29 940  | 30 180  | 30 480 | 3,4 |  2,2 |
#'|  Régions                  | 22 432 |  22 836  | 23 004  | 23 484 | 3,8 |  4,7 |
#'| Autres collectivités locales  | 24 680  | 24 696  | 24 828  | 25 032 | 3,2 | 1,4 |
#'| Ensemble (moyenne)        | 21 873 | 22 176   | 22 212  | 22 524 | 2,9 |  3,0 |
#' 

#'**RMPP nette 2014 (salariés présents en 2013 et 2014 avec la même quotité) en EQTP**        
#'    
#' 
#'|  Organisme      RMPP net          |  2014  | 
#'|:---------------------------------:|-------:|
#'| Communes                          | 22 524 |
#'| CCAS et caisses des écoles        | 21 420 |
#'| EPCI à fiscalité propre           | 24 864 |
#'| Autres structures intercommunales | 23 988 |
#'|   Départements                    | 25 932 |
#'|   SDIS                            | 31 032 |
#'|  Régions                          | 24 240 |
#'| Autres collectivités locales      | 21 873 |
#'|  Ensemble (moyenne)               | 23 760 |
#' 
#'   
#'  
#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			
#'Conversion en euros courants, calcul CRC.  


#'[Source RAEFP 2016 données 2014](Docs/RA_2016.pdf)      
#'[Source INSEE 2016](Docs/insee-premiere1616.pdf)      

incrémenter.chapitre()

newpage()
