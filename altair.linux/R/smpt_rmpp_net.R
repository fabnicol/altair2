#'@export
evolution_net_hist_1 <- function() {

  calcul_rmpp()
  
  temp <- positive(moyenne.rémunération.annuelle.sur.période) / 1000
  
  essayer({ if (longueur.non.na(temp) > 0)
    hist(temp,
         xlab = "Sur la période " %+% début.période.sous.revue %+% "-" %+% fin.période.sous.revue %+% " en milliers d'euros",
         ylab = "Effectif",
         main = "Rémunération nette moyenne",
         col = "blue",
         nclass = 200)
  }, "Le graphique des rémunérations moyennes n'a pas pu être généré")
  
}

#'@export
evolution_net_hist_2 <- function() {
  

  temp <- na.omit(moyenne.rémunération.annuelle.sur.période[moyenne.rémunération.annuelle.sur.période > minimum.positif
                                                            & (statut == "TITULAIRE"  | statut == "STAGIAIRE")] / 1000)
  
  essayer(label = "+histogramme",
          {  if (longueur.non.na(temp) > 0)
            hist(temp,
                 xlab = "Sur la période "%+% début.période.sous.revue %+% "-" %+% fin.période.sous.revue %+% " en milliers d'euros",
                 ylab = "Effectif",
                 main = "Rémunération nette moyenne des fonctionnaires",
                 col = "blue",
                 nclass = 200)
            
          }, "Le graphique des rémunérations moyennes des fonctionnaires n'a pas pu être généré")
}
