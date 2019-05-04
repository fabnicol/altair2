#'
#'## 3.4 Rémunérations brutes par grade et par emploi   
#'   

brut.eqtp <- brut.eqtp(variation = TRUE)
brut.eqtp.emploi <- brut.eqtp.emploi(variation = TRUE)   
sauv.bases(file.path(chemin.dossier.bases, "Remunerations"), 
           environment(), "brut.eqtp.emploi")
sauv.bases(file.path(chemin.dossier.bases, "Remunerations"), 
           environment(), "brut.eqtp")


#'   
conditionnel("Rémunérations brutes par grade", "Bases/Remunerations/brut.eqtp.csv")      
#'    
conditionnel("Rémunérations brutes par emploi", "Bases/Remunerations/brut.eqtp.emploi.csv")  
#'   
#'   
#'## 3.5 Comparaisons source INSEE/DGCL   
#'   
#'*Salaires annnuels bruts moyens 2011-2016 en EQTP (hors assistantes maternelles)*   

#'  
#'&nbsp;*Tableau 3.5.1*   
#'    

#+ comparaison-insee1

Tableau.vertical2(c("Agrégat (euros)", "2011", "2012", "2013", "2014", "2015", "2016"),
                  c("Ensemble", "Titulaires", "Autres salariés"),
                  digits = 0,
                  12 * c(2159, 2223, 1903),
                  12 * c(2195, 2259, NA),
                  12 * c(2218, 2287, 2030),
                  12 * c(2237, 2337, 2042),
                  12 * c(2282, 2372, 2058),
                  12 * c(2303, 2397, 2069))

#'**Eléments de la rémunération brute pour les titulaires de la fonction publique territoriale**      
#'   
#'  
#'&nbsp;*Tableau 3.5.2*   
#'    

Tableau.vertical2(
  c("Rém. annuelles",  "2011", "Primes", "2012", "Primes", "2013", "Primes"),
  c("Salaire brut",
    "Traitement brut",
    "Primes et rémunérations annexes",
    "y compris IR et SFT"),
  digits = 0,
  c(26660, 20562, 0, 6098),
  c("", "22,9 %", "", "" ),
  12 * c(2259, 1727, 0, 532),
  c("", "23,6 %", "", "" ),
  12 * c(2287, 1755, 0, 532),
  c("", "23,6 %", "", "" )
  )
#'      
#'      

Tableau.vertical2(
  c("Rém. annuelles", "2014", "Primes", "2015", "Primes", "2016", "Primes"),
  c("Salaire brut",
    "Traitement brut",
    "Primes et rémunérations annexes",
    "y compris IR et SFT"),
  digits = 0,
  12 * c(2337, 1788, 0, 549),
  c("", "23,5 %", "", "" ),
  12 * c(2372, 1818, 0, 554),
  c("", "23,4 %", "", "" ),
  12 * c(2397, 1842, 0, 555),
  c("", "23,2 %", "", "" )
)


#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*   												
#'*Les primes sont cumulées au supplément familial de traitement (SFT) et à l'indemnité de résidence (IR). Le cumul est rapporté à la rémunération brute totale.*    
#'[Source INSEE](Docs/ip1486.xls)    
#'[Source DGCL](Docs/Vue3_Remuneration_2017.xlsx)    
#'[Source DGCL](Docs/Vue-Remunerations-2018.xlsx)   
#'[Source RAEFP 2015](Docs/RA_2015.pdf)   
#'[Source RAEFP 2016](Docs/RA_2016.pdf)   
#'[Source RAEFP 2017](Docs/RA_2017.pdf)    
#'[Source RAEFP 2018](Docs/RA_2018.pdf)    
#'   
#'   
#'## 3.6 Coût chargé  
#'
#'**Les liens ci-après renvoient vers des tableaux présentant le coût moyen chargé par agent**  
#'  
#'
cout.eqtp <- charges.eqtp(variation = TRUE)
cout.eqtp.emploi <- charges.eqtp.emploi(variation = TRUE)   
cout.eqtp.serv <- charges.eqtp.serv(variation = TRUE)

sauv.bases(file.path(chemin.dossier.bases, "Remunerations"), 
           environment(), "cout.eqtp.emploi")
sauv.bases(file.path(chemin.dossier.bases, "Remunerations"), 
           environment(), "cout.eqtp")

setwd(file.path(chemin.dossier.bases, "Remunerations"))
csvfiles  <- list.files(".", pattern = "^charges.serv..*.csv")
system2(file.path(currentDir, "linux/utf82latin1"), system.quote(csvfiles), stderr = NULL, stdout = NULL)
zip("cout.eqtp.services.zip", csvfiles)
invisible(file.remove(csvfiles))
setwd(currentDir)

#'   
conditionnel("Coût moyen chargé par grade", "Bases/Remunerations/cout.eqtp.csv")      
#'    
conditionnel("Coût moyen chargé par emploi", "Bases/Remunerations/cout.eqtp.emploi.csv")    
#'   
#'    
conditionnel("Coût moyen chargé par grade et service", "Bases/Remunerations/cout.eqtp.services.zip")    
#'   

#'          
