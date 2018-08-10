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
# 
# 

# on prévoit 15 enfants...

# limitations : pas de vérification des cas de divorce etc., ni des cas de cumuls
#               pas de vérification non plus de la licéité des versements à des contractuels exclus par l'article 1er 
#               du décret n°85-1148 du 24 octobre 1985 modifié relatif à la rémunération des personnels civils et militaires
#               de l'Etat, des personnels des collectivités territoriales et des personnels des établissements publics d'hospitalisation. 

# part fixe mensuelle

sft.fixe <- c(un = 2.29, deux = 10.67, trois = 15.24, 15.24 + 4.57 * 1:12)

# part variable en proportion du traitement indiciaire

sft.prop <- c(un = 0, deux = 3, trois = 8, 8 + 6 * 1:12) / 100 

part.proportionnelle.minimale <- outer(PointMensuelIM, sft.prop * 449)


sft_R <- function(x, indice, échelon, nbi, durée, année, mois)   {
  
  if (x > 15) return(-1)
  
  if (is.na(durée) || is.na(x)) return(0)
  
  if (grepl("H.*(E|é).*[A-F]", échelon, perl = TRUE, ignore.case = TRUE)) {
    
    indice <- 717
 
  }
  
  indice <- sum(indice, nbi, na.rm = TRUE)
  
  # "Pour les personnels non rémunérés par un traitement établi en application de l'article 2 précité,
  # l'élément proportionnel est calculé en pourcentage du traitement afférent à l'indice majoré 449 (indice brut 524)." art. 10 bis décretn°85-1148
  
  part.proportionnelle <- (x != 0) * sft.prop[x] * max(449, min(indice, 717)) * PointMensuelIM[année - 2007, mois]  
  
  # on prend en compte les quotités spécifiques de temps partiel
  
  if (durée == 90) {
    coef <- 0.91429   # 32/35 
  } else if (durée == 80) {
    coef <- 0.85714   # 6/7   
  } else coef <- durée/100
  
  if (x != 1) {
    
    valeur <- coef * (part.proportionnelle + sft.fixe[x]) 
    
  } else {
    
    valeur <- coef * part.proportionnelle + sft.fixe[x]
  }
  
  # vérification du plancher des attributions minimales à temps plein
  
  if (x != 1) 
    valeur <- max(valeur, part.proportionnelle.minimale[ , , x][année - 2007, mois] + sft.fixe[x])
  
  
  #if (is.na(valeur)) valeur <- 0
  
  return(valeur)
  
}
