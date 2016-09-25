
# SMIC mensuel net x 3 utilisé pour la condition alternative sur les périodes non annexes
# un salarié qui au cours de l'année connait une rémunération supérieure à 3 fois le smic mensuel
# est dans une période non annexe, quelle que soit la durée de cette période dans l'année.
# remarque :  - on ne vérifie pas la continuité de cette période
#             - on prend la moyenne annuelle des smic net déduction faite de la csg et de la crds, arrondie à la première décimale

smic.net <- data.table::fread(
  
  # Rajouter ici les valeurs futures sous la forme année,  valeur au début de la chaine de caractères :
  
                   "2014,  3386.1
                    2013,  3361.3
                    2012,  3322.5  
                    2011,  3221.9
                    2010,  3168.7
                    2009,  3132.2
                    2008,  3075.7",
                   
                   col.names = c("Année", "SMIC_NET"))

# actualiser aussi ceci, pour éviter à recalculer :

smic.net.dernière.année.renseignée <- 2014
smic.net.première.année.renseignée <- 2008

# http://www.bdm.insee.fr/ et documents altair/Docs/SMIC_NET.xls 
