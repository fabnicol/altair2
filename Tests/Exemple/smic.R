# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser
# les fichiers de paye produits au format spécifié par l'annexe de la  
# convention-cadre de dématérialisation en vigueur à partir de 2008.
# 
# Ce logiciel est régi par la licence CeCILL soumise au droit français et
# respectant les principes de diffusion des logiciels libres. Vous pouvez
# utiliser, modifier et/ou redistribuer ce programme sous les conditions
# de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
# sur le site "http://www.cecill.info".
# 
# En contrepartie de l'accessibilité au code source et des droits de copie,
# de modification et de redistribution accordés par cette licence, il n'est
# offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
# seule une responsabilité restreinte pèse sur l'auteur du programme, le
# titulaire des droits patrimoniaux et les concédants successifs.
# 
# A cet égard l'attention de l'utilisateur est attirée sur les risques
# associés au chargement, à l'utilisation, à la modification et/ou au
# développement et à la reproduction du logiciel par l'utilisateur étant
# donné sa spécificité de logiciel libre, qui peut le rendre complexe à
# manipuler et qui le réserve donc à des développeurs et des professionnels
# avertis possédant des connaissances informatiques approfondies. Les
# utilisateurs sont donc invités à charger et tester l'adéquation du
# logiciel à leurs besoins dans des conditions permettant d'assurer la
# sécurité de leurs systèmes et ou de leurs données et, plus généralement,
# à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
# 
# Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
# pris connaissance de la licence CeCILL, et que vous en avez accepté les
# termes.
# 
# mes.
# 
# 

# SMIC mensuel net x 3 utilisé pour la condition alternative sur les périodes non annexes
# un salarié qui au cours de l'année connait une rémunération supérieure à 3 fois le smic mensuel
# est dans une période non annexe, quelle que soit la durée de cette période dans l'année.
# remarque :  - on ne vérifie pas la continuité de cette période
#             - on prend la moyenne annuelle des smic net déduction faite de la csg et de la crds, arrondie à la première décimale

smic.net <- data.table::fread(
  
  # Rajouter ici les valeurs futures sous la forme année,  valeur au début de la chaine de caractères :
  
                   "2018,  3446.9
                    2017,  3447.2 
                    2016,  3424.8
                    2015,  3408.0
                    2014,  3386.1
                    2013,  3361.3
                    2012,  3322.5  
                    2011,  3221.9
                    2010,  3168.7
                    2009,  3132.2
                    2008,  3075.7",
                   
                   col.names = c("Année", "SMIC_NET"))

# actualiser aussi ceci, pour éviter à recalculer :

smic.net.dernière.année.renseignée <- 2018
smic.net.première.année.renseignée <- 2008

# http://www.bdm.insee.fr/ et documents altair/Docs/SMIC_NET.xls 
