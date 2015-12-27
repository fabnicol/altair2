

# Article L2122-2 Le conseil municipal détermine le nombre des adjoints au maire sans que ce nombre puisse excéder 30 % de l'effectif légal du conseil municipal.
# Article L2122-2-1 Dans les communes de 80 000 habitants et plus, la limite fixée à l'article L. 2122-2 peut donner lieu à dépassement en vue de la création de postes d'adjoints chargés principalement d'un ou plusieurs quartiers, sans toutefois que le nombre de ceux-ci puisse excéder 10 % de l'effectif légal du conseil municipal.


# COMMUNES	                        NOMBRE DES MEMBRES du conseil municipal
# De moins de 100 habitants	        7
# De 100 à 499 habitants	          11
# De 500 à 1 499 habitants	        15
# De 1 500 à 2 499 habitants	      19
# De 2 500 à 3 499 habitants	      23
# De 3 500 à 4 999 habitants	      27
# De 5 000 à 9 999 habitants	      29
# De 10 000 à 19 999 habitants	    33
# De 20 000 à 29 999 habitants	    35
# De 30 000 à 39 999 habitants	    39
# De 40 000 à 49 999 habitants	    43
# De 50 000 à 59 999 habitants	    45
# De 60 000 à 79 999 habitants	    49
# De 80 000 à 99 999 habitants	    53
# De 100 000 à 149 999 habitants	  55
# De 150 000 à 199 999 habitants	  59
# De 200 000 à 249 999 habitants	  61
# De 250 000 à 299 999 habitants	  65
# Et de 300 000 et au-dessus	      69


# Le Code général des collectivités territoriales distingue trois catégories de population : la population municipale, la population comptée à part et la population totale, qui est la somme des deux précédentes (art. R. 2151-1 du CGCT).
# Le chiffre de population auquel il convient de se référer en matière électorale est le dernier chiffre de population municipale authentifié avant l'élection (art. R. 2153-3 du CGCT). La dernière authentification des chiffres du recensement date de la publication des résultats du recensement de 1999 par le décret n° 99-1154 du 29 décembre 1999. En conséquence, il convient de se rapporter à ce décret, éventuellement complété par les résultats des recensements complémentaires authentifiés par des arrêtés publiés au Journal Officiel pour déterminer le chiffre de la population municipale de la commune.

# prendre base insee texte des communes, apparier par nom de com, trouverstatut chef lieux et population 99 (->2008) et 2011 (->2014). ne rete plus qu'a preciser surclassement.

nb.conseillers.mun <- function(population, année, mois) {

  # les trois dernières valeurs sont adhoc pour Lyon, Marseille, Paris ()
   T <- data.table(pop.min=c(1, 100, 500, 1500, 2500, 3500, 5000, 10000, 20000, 30000, 40000, 50000, 60000, 80000, 100000, 150000, 200000, 250000, 300000, 500000, 700000, 1500000),
                        NCM=c(9, 11, 15,  19,   23,   27,   29,   33,    35,    39,    43,    45,    49,    53,    55,     59,     61,     65,     69,     73, 101, 163))

   if (année >= 2014 && mois >= 4) set(T, 1L, 2L, 7)
   
   tail(T[pop.min <= population, NCM], 1)
   
}

nb.adjoints.max <- function(population, année, mois) {
                      coef <- ifelse(population >= 80000, 0.4, 0.3)
                      floor(coef * nb.conseillers.mun(population, année, mois)) 
}


majoration <- 0

if (chef.lieu.dpt) {
  
    majoration <- 0.25
    
  } else if (chef.lieu.arrdt) {
    
    majoration <- 0.2
    
  } else if (chef.lieu.canton) {
    
    majoration <- 0.15
}

# 4 cas de suclassement : station toutistique, population en hausse en raison de travaux d'intéret national, attributaire de DSU sur une des 3 dernieres années
#                         ou ayant au moins une partie de ZUS.

# Article R2123-23 CGCT
# Les majorations d'indemnités de fonction résultant de l'application de l'article L. 2123-22 peuvent s'élever au maximum pour les élus visés à l'article L. 2123-20 :
# 
# 1° Dans les communes chefs-lieux de département, d'arrondissement et de canton respectivement à 25 %, à 20 % et 15 % ;
# 
# 2° Dans les communes sinistrées, à un pourcentage égal au pourcentage d'immeubles sinistrés de la commune. Ce supplément d'indemnité peut se cumuler, le cas échéant, avec les majorations prévues au 1° ci-dessus, mais il doit être calculé d'après le montant de l'indemnité tel qu'il est prévu aux articles L. 2123-20 à L. 2123-24 ;
# 
# 3° Dans les communes mentionnées aux 3° et 4° de l'article L. 2123-22, à 50 % pour les communes dont la population totale est inférieure à 5 000 habitants et à 25 % pour celles dont la population totale est supérieure à ce chiffre. Un arrêté du préfet détermine les communes dans lesquelles les dispositions prévues au 4° de l'article L. 2123-22 sont applicables ;
# 
# 4° Dans les communes mentionnées au 5° de l'article L. 2123-22, les indemnités de fonctions peuvent être votées dans les limites correspondant à l'échelon immédiatement supérieur à celui de la population des communes visé à l'article L. 2123-23.

# Pour les stations et ZUS : 88 alinéa 3 de la loi du n° 84-53 du 26 janvier 1984 

if (station.touristique.ou.TIN == TRUE) {
  
  if (population < 5000) {
    
    majoration <- majoration + 0.5
      
  } else {
    
    majoration <- majoration + 0.25
  }
}
  

#                    MAIRE     ADJOINT
# POPULATION      % IB 1015   % IB 1015

# Moins de 500         17      6.6
# De 500 à 999         31      8.25
# De 1 000 à 3 499     43      16.5
# De 3 500 à 9 999     55      22
# De 10 000 à 19 999   65      27.5  
# De 20 000 à 49 999   90      33
# De 50 000 à 99 999   110     44
# De 100 000 à 199 999 145     66
# +200 000             145     72.5


plafond.indemn.élu <- matrix(c(0.17, 0.31,   0.43,   0.55, 0.65,   0.90, 1.1,  1.45,  1.45,
                              0.066, 0.0825, 0.165,  0.22, 0.275,  0.33, 0.44, 0.66,  0.725), ncol = 2) 

strate <- function(population) {

  strate <- 1

  if (population >= 500 && population < 1000) {
     strate <- 2
  } else if (population >= 1000 && population < 3500) {
     strate <- 3 
  } else if (population >= 3500 && population < 10000) {
     strate <- 4 
  } else if (population >= 10000 && population < 20000) {
     strate <- 5
  } else if (population >= 20000 && population < 50000) {
     strate <- 6
  } else if (population >= 50000 && population < 100000) {
     strate <- 7
  } else if (population >= 100000 && population < 200000) {
     strate <- 8
  } else strate <- 9
  

  strate
}

# La formule ci-dessous est valable tant que IB 1015 == INM 821, ce qui est vrai sur 2008-2015
# rentrer la strate réelle et le statut sous la forme : maire = 1, adjoint = 2
# ex : PlafondsMens(strate(12000), 1)

PlafondsMens   <- function(strate, statut) {
                    # strate surclassée
                    strate_ <- min(9, strate + (surclassement == TRUE))
                    # Les majorations pour communes touristiques et chefs lieux se calculent en fonction de la strate démographique réelle et non pas de la strate surclassée                    
                    return((plafond.indemn.élu[strate, statut] * majoration + plafond.indemn.élu[strate_, statut]) * 821 * PointMensuelIM)
}


