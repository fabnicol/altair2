# Plafonds d'IFTS par catégorie de grades ou d'emplois

# année >= 2008 et année <= 2015

MontantMoyenIFTS_CAT1 <- matrix(c(
  1440.67, 1440.67,
  rep.int(1447.87, 7),
  rep.int(1452.22, 9),
  rep.int(1459.46, 3),
  rep.int(1463.86, 9),
  rep.int(1471.18, 66)), 
  ncol = 12, byrow = TRUE)

MontantMoyenIFTS_CAT2 <- matrix(c(
  1056.36, 1056.36,
  rep.int(1061.64, 7),
  rep.int(1064.83, 9),
  rep.int(1070.15, 3),
  rep.int(1073.37, 9),
  rep.int(1078.73, 66)), 
  ncol = 12, byrow = TRUE)

MontantMoyenIFTS_CAT3 <- matrix(c(
  840.04, 840.04,
  rep.int(844.24, 7),
  rep.int(846.77, 9),
  rep.int(851.00, 3),
  rep.int(853.56, 9),
  rep.int(857.82, 66)), 
  ncol = 12, byrow = TRUE)

# 01/01/2008  01/03/2008   01/10/2008 01/07/2009 01/10/2009  01/07/10    15/05/2014
#  1440.67      1447.87       1452.22    1459.46    1463.86     1471.18     1471.18 # "1re catégorie"
#  1056.36      1061.64       1064.83    1070.15    1073.37     1078.73     1078.73 # "2e catégorie"    
#  840.04       844.24        846.77     851.00     853.56      857.82      857.82  # "3e catégorie"
#  NA           NA            NA         NA         NA          NA          970     # "4e catégorie"

# décret de référence :  Décret n° 2002-63 du 14 janvier 2002 relatif à l'indemnité forfaitaire pour travaux supplémentaires des services déconcentrés

#Les attributions individuelles ne peuvent excéder huit fois le montant moyen annuel attaché à la catégorie à laquelle appartient l'agent.

# Avant le 15 mai 2014 (arrêté du 14 janvier 2002 fixant les montants moyens annuels de l'indemnité forfaitaire pour travaux supplémentaires des services déconcentrés)

# définition des catégories

  # 1re catégorie : fonctionnaires de catégorie A appartenant à un grade dont l'indice brut terminal est supérieur à celui de l'indice brut terminal du premier grade du corps des attachés d'administration centrale.
  # 2e catégorie : fonctionnaires de catégorie A appartenant à un grade dont l'indice brut terminal est au plus égal à l'indice brut terminal du premier grade du corps des attachés d'administration centrale.
  # 3e catégorie : fonctionnaires de catégorie B.

# A partir du 15 mai 2014 (arrêté du 12 mai 2014 fixant les montants moyens annuels de l'indemnité forfaitaire pour travaux supplémentaires des services déconcentrés)

  # 1re catégorie : fonctionnaires de catégorie A appartenant à un grade dont l'indice brut terminal est supérieur à celui de l'indice brut terminal du premier grade du corps des attachés d'administration de l'Etat ;
  # 2e catégorie : fonctionnaires de catégorie A appartenant à un grade dont l'indice brut terminal est au plus égal à l'indice brut terminal du premier grade du corps des attachés d'administration de l'Etat ;
  # 3e catégorie : fonctionnaires de catégorie B ;
  # 4e catégorie : fonctionnaires de catégorie A dont l'indice brut terminal du dernier grade est égal à l'indice brut 700 jusqu'au 30 juin 2015 et à l'indice brut 730 à compter du 1er juillet 2015.

# concrètement, avant le 15 mai 2014 :

# 1e catégorie : Directeur, Attaché principal,administrateur
#                Professeur d'enseignement artistique hors classe chargé de direction exclusivement 
#                Professeur d'enseignement artistique de classe normale chargé de direction exclusivement
# 3e catégorie : tous les B avec INM >= 350
# 2e catégorie : autres A

#depuis le 15 mai 2014, les informiers en soins généraux peuvent être comptés en 4e catégorie
