# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, annees 2012 à 2017
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
# 

#' @export
#' 
point_inm <- function() {

  an <- as.numeric(format(Sys.time(), "%Y"))
  
  if (debut.periode.sous.revue < 2008) {
    
    cat("Ce programme ne peut pas traiter les périodes sous revue commençant avant le 1er janvier 2008. Ces années seront écartées.")
    "extraire.annees" %a% TRUE
    "debut.periode.sous.revue" %a% 2008
  }
    
  # Période couverte : 2008-2021 inclus
  
  "PointIM" %a% matrix(c(
54.4113,             # 2008 01
54.4113,             # 2008 02
rep.int(54.6834, 7), # 2008 3-9 
rep.int(54.8475, 9), # 2008 10-12 et 2009 1-6
rep.int(55.1217, 3), # 2009 6-9
rep.int(55.2871, 9), # 2009 10-12 et 2010 1-6
rep.int(55.5635, 72),# 2010 7 -> 2016 6  (6 annees de blocage)
rep.int(55.8969, 6), # 2016 7 -> 2016 12
55.8969, # 2017 
rep.int(56.2323,11), # 2017
rep.int(56.2323,12), # 2018  
rep.int(56.2323,12), # 2019 
rep.int(56.2323,12), # 2020 
rep.int(56.2323,12), # 2021 
rep.int(56.2323, 6), # 2022 1-6 
rep.int(58.20036,6)  # 2022 7-12 
),

# inserer ici la valeur courante du mpoint d'indice annuel
# par exemple pour 56 € par point/an :
# rep.int(56, 12)
# si passage à 56.5 au 1er juillet:
# rep.int(56, 6)
# rep.int(56.5, 6)

ncol = 12, 
byrow = TRUE)
  
  limite_inm <- 2008 + nrow(PointIM)
  
  if (an >= limite_inm) {
    
    cat("AVERTISSEMENT : Il faut actualiser la matrice PointIM du fichier point.im.R comme indiqué en commentaires en rajoutant la valeur annuelle du point d'indice net pour les annees après", limite_inm)
    
    if (fin.periode.sous.revue >= limite_inm) {
      
      cat("ERREUR : La periode sous revue dépasse la valeur limite du tableau en référence du point d'indice net. Actualiser ce tableau (fichier point.im.R) et relancer", limite_inm - 1)
      if (fin.periode.sous.revue > limite_inm) {
        cat("Il faut rajouter la periode", limite_inm, " - ",  fin.periode.sous.revue)
      } else {
        cat("Il faut rajouter l'année", limite_inm)
      }
      
      cat("Pour continuer à s'exécuter le programme va écarter les années ci-dessus mentionnées...")
      
      "extraire.annees" %a% TRUE
      "fin.periode.sous.revue" %a% limite_inm - 1
    }
  }
  
  "PointMensuelIM" %a% (PointIM / 12)
  
  "PointMensuelIMMoyen" %a% apply(PointMensuelIM, 1, mean)
  
  "valeur.point.inm.pivot" %a% round(PointMensuelIMMoyen, 1)     
  
  # annee >= 2008 et annee <= 2016
  
  # pour avoir la valeur du point d'indice retrancher 2007 à l'annee :
  #   PointIM[annee - 2007, mois]
 
}
