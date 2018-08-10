
#'   
#'## `r chapitre`.4 Comparaisons source INSEE/DGCL   
#'   
#'*Salaires annnuels moyens 2011 en EQTP (hors assistantes maternelles)*   

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

#+ comparaison-insee1

Tableau.vertical2(c("Agrégat (euros)", "Salaires bruts 2011", "Salaires bruts 2012", "Salaires bruts 2013"),
                  c("Ensemble", "Titulaires", "Autres salariés"),
                  12 * c(2159, 2223, 1903),
                  12 * c(2195, 2259, NA),
                  12 * c(2218, 2287, 2030))


#'**Eléments de la rémunération brute pour les titulaires de la fonction publique territoriale**      
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(
  c("Rém. annuelles", "2010", "Primes", "2011", "Primes", "2012", "Primes", "2013", "Primes"),
  c("Salaire brut",
    "Traitement brut",
    "Primes et rémunérations annexes",
    "y compris IR et SFT"),
  c(26305, 20350,	"", 5955),
  c("", "22,6 %", "", "" ),
  c(26660, 20562, "", 6098),
  c("", "22,9 %", "", "" ),
  c(12*2259, 12*1727, "", 12*532),
  c("", "23,6 %", "", "" ),
  c(12*2287, 12*1755, "", 12*532),
  c("", "23,6 %", "", "" ))
#'   
#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*   												
#'*Les primes sont cumulées au supplément familial de traitement (SFT) et à l'indemnité de résidence (IR). Le cumul est rapporté à la rémunération brute totale.*    
#'[Source INSEE](Docs/ip1486.xls)    
#'[Source DGCL](Docs/Vue3_1_Remunerations.xls)    
#'[Source RAEFP 2015](Docs/RA_2015.pdf)   
#'   
