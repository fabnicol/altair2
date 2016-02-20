
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


sft_R <- function(x, indice, nbi, durée, année, mois)   {
  
  if (x > 15) return(-1)
  
  if (is.na(durée) || is.na(x) || is.na(indice)) return(0)
  
  if (grepl("H.*(E|é).*[A-F]", indice, perl = TRUE, ignore.case = TRUE)) {
    
    indice <- 717
    
  } else {
    
    indice <- as.numeric(indice)
  }
  
  indice <- sum(indice, nbi, na.rm = TRUE)
  
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
