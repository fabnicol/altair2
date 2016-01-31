
  
Résumé(c("Âge des personnels <br>au 31/12/" %+% début.période.sous.revue,
         "Effectif",
         "Âge des personnels <br>au 31/12/" %+% fin.période.sous.revue,
         "Effectif"),
       list(Bulletins.avant$age, 
            Bulletins.après$age),
       extra = "length",
       align = 'c')
#' 
#+fig.height=9.7, fig.width=8.4, dpi=300      


if (longueur.non.na(avant) > 0 && longueur.non.na(après) > 0) {
  
  pyramide_ages(avant, après, titre)

  H0 <- avant[ , .(total = sum(total, na.rm = TRUE)), by = floor(age / 5)]
  H1 <- après[ , .(total = sum(total, na.rm = TRUE)), by = floor(age / 5)]
}
#' 

#+fig.height=7, fig.width=10   

if (longueur.non.na(H0$total) > 0 && longueur.non.na(H1$total) > 0) {
  barplot(H1$total - H0$total, 
          names.arg=c("15-20", 
                      "20-25",
                      "25-30",
                      "30-35",
                      "35-40",
                      "40-45",
                      "45-50",
                      "50-55",
                      "55-60",
                      "60-65",
                      "65-70"),
          ylab = "Variation d'effectifs",
          xlab = "Tranche d'âge",
          main = "Evolution des effectifs par tranche d'âge",
          col  = "navy",
          cex.names=0.8)
}
#' 




