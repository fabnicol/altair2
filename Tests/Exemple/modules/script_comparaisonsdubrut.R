
#'   
#'## 3.4 Comparaisons source INSEE/DGCL   
#'   
#'*Salaires annnuels bruts moyens 2011-2016 en EQTP (hors assistantes maternelles)*   

#'  
#'&nbsp;*Tableau 3.4.1*   
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
#'&nbsp;*Tableau 3.4.2*   
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
