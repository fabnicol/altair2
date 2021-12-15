
#'   
#'## 4.4 Rémunérations nettes par grade, emploi et service         
#'   

setwd(file.path(chemin.dossier.bases, "Remunerations"))
      
net.grades   <<- net.eqtp(variation = TRUE)
net.emplois  <<- net.eqtp.emploi(variation = TRUE)
net.services <<- net.eqtp.serv(variation = TRUE)

Sauv.base("Remunerations", "net.emplois") 
Sauv.base("Remunerations", "net.grades")
Sauv.base("Remunerations", "net.services") 

csvfiles  <- list.files(".", pattern = "^net.serv..*.csv")
zip("net.services.zip", csvfiles)
invisible(file.remove(csvfiles))

setwd(currentDir)

#'   
conditionnel("Rémunérations nettes par grade", "Bases/Remunerations/net.grades.csv")      
#'  
conditionnel("Rémunérations nettes par emploi", "Bases/Remunerations/net.emplois.csv")  
#'   
conditionnel("Rémunérations nettes par service", "Bases/Remunerations/net.services.zip")  
#'  
#'*Note : les moyennes des tableaux sont pondérées en EQTP. Les rémunérations nettes sont calculees en retranchant le supplément familial de traitement.*      
#'    
#'  
#'
#'
#'## 4.5 Comparaisons avec la situation nationale des rémunérations   
#'  
#'**Évolution en euros courants du SMPT et de la RMPP dans la FPT (en % et euros courants)**    

#'  
#'&nbsp;*Tableau 4.5.1*   
#'    
message("smpt_rmpp_brut_net.csv")
#' 
Tableau.data("smpt_rmpp_brut_net.csv",
             c(" Année      ", 2009:2019))

#'*Source : fichier général de l'État (FGE), DADS, SIASP, Insee, Drees. Traitement Insee, Drees, DGCL*    
#'Hors assistants maternels et familiaux, y compris bénéficiaires de contrats aidés.   
#'Lecture : en 2014, le SMPT brut en EQTP a augmenté de 1,7 %
#'SMPT : Salaire moyen par tête en EQTP.   
#'RMPP : Agents présents 24 mois consécutifs chez le même employeur avec la même quotité de travail.   
#'Lecture : en 2014, la rémunération nette en EQTP des agents présents deux annees consécutives en 2012 et 2013 avec la même quotité a augmenté de 2,7 %   
#'  
#'**Salaires nets annuels et évolution moyenne type de collectivité en euros courants  EQTP**    
#'   
#'  
#'&nbsp;*Tableau 4.5.2*   
#'    

message("organismes_smpt_net.csv")   
#'   
Tableau.data("organismes_smpt_net.csv", 
            c("Organisme SMPT net", 2013:2019))

#'      
#'**RMPP nette (salariés présents deux années de suite avec la même quotité) en EQTP**        
#'  
#'&nbsp;*Tableau 4.5.3*   
#'    
message("organismes_rmpp_net.csv")
#'   
Tableau.data("organismes_rmpp_net.csv",
            c("Organisme RMPP nette",  2014, "2014-2015", "2015-2016", "2016-2017", "2017-2018"))
      
#'       
#'   
#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			
#'Conversion en euros courants, calcul CRC.  La métropole de Lyon est classée avec les départements   

#'[Source RAEFP 2016 données 2014](Docs/RA_2016.pdf)      
#'[Source RAEFP 2017 données 2015](Docs/RA_2017.pdf)      
#'[Source RAEFP 2018 données 2016](Docs/RA_2018.pdf)   
#'[Source RAEFP 2019 données 2017](Docs/RA_2019.pdf)   
#'[Source RAEFP 2020 données 2018](Docs/RA_2020.pdf)   
#'[Source RAEFP 2021 données 2019](Docs/RA_2021.pdf)   
   
incrementer.chapitre()

newpage()
