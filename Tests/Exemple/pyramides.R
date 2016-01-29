

  
Résumé(c("Âge des personnels <br>au 31/12/" %+% début.période.sous.revue,
         "Effectif",
         "Âge des personnels <br>au 31/12/" %+% fin.période.sous.revue,
         "Effectif"),
       list(avant, 
            après),
       extra = "length",
       align = 'c')
#'  
if (longueur.non.na(avant) > 0) {
  
  hist(avant,
       xlab = "Âge au 31 décembre " %+% début.période.sous.revue,
       ylab = "Effectif",
       main = titre,
       col = "blue",
       breaks = 16:68)
  
  H0 <- hist(avant, breaks = seq.int(15, 70, by = 5), plot = FALSE)
  
}

#'  
if (longueur.non.na(après) > 0) {
  
  hist(après,
       xlab = "Âge au 31 décembre " %+% fin.période.sous.revue,
       ylab = "Effectif",
       main =  titre,
       col = "blue",
       breaks = 16:68)
  
  H1 <- hist(après, breaks = seq.int(15, 70, by = 5), plot = FALSE)
}

barplot(H1$count - H0$count, names.arg=c("15-20", "20-25", "25-30", "30-35", "35-40", "40-45", "45-50", "50-55", "55-60", "60-65", "65-70"), ylab="Variation d'effectifs", xlab = "Tranche d'âge", main= "Evolution des effectifs par tranche d'âge", cex.names=0.8)


