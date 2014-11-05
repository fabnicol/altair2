
knitr::opts_chunk$set(fig.width = 7.5, echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')

histogrammes <- function()  {

if (longueur.non.na(filtre.fonctionnaire(Montant.brut.annuel.eqtp) > 0)) 
  hist(filtre.fonctionnaire(Montant.brut.annuel.eqtp)/1000,
       xlab = "En milliers d'euros  (< 40 k€) EQTP\n",
       ylab = "Effectif",
       xlim = c(0, 40),
       main = "Rémunération brute annuelle des fonctionnaires en " %+% année,
       col = "blue",
       breaks = 50)

#'    
#'    
if (longueur.non.na(filtre.fonctionnaire(Montant.brut.annuel.eqtp[Montant.brut.annuel.eqtp > 40000]) > 0)) 
  hist(filtre.fonctionnaire(Montant.brut.annuel.eqtp[Montant.brut.annuel.eqtp > 40000])/1000,
       xlab = "En milliers d'euros (> 40 k€) EQTP\n",
       ylab = "Effectif",
       xlim = c(40, 120),
       main = "Rémunération brute annuelle des fonctionnaires en " %+% année,
       col = "blue",
       breaks = 50)

#'   
#'    
#'    
if (longueur.non.na(filtre.fonctionnaire(rémunération.indemnitaire.imposable.eqtp) > 0))
  hist(filtre.fonctionnaire(rémunération.indemnitaire.imposable.eqtp)/1000,
       xlab = "En milliers d'euros (< 10 k€) EQTP\n",
       ylab = "Effectif",
       xlim = c(0, 10),
       main = "Rémunération indemnitaire imposable annuelle des fonctionnaires en " %+% année,
       col = "blue",
       breaks = 250)
#'   
#'   
#'   
if (filtre.fonctionnaire(rémunération.indemnitaire.imposable.eqtp[rémunération.indemnitaire.imposable.eqtp > 10000]) > 0)
  hist(filtre.fonctionnaire(rémunération.indemnitaire.imposable.eqtp[rémunération.indemnitaire.imposable.eqtp > 10000])/1000,
       xlab = "En milliers d'euros (> 10 k€) EQTP\n",
       ylab = "Effectif",
       xlim = c(10, 60),
       main = "Rémunération indemnitaire imposable annuelle des fonctionnaires en " %+% année,
       col = "blue",
       breaks = 50)
#'   
#'   
#'   

if (longueur.non.na(filtre.fonctionnaire(part.rémunération.indemnitaire) > 0)) 
  hist(filtre.fonctionnaire(part.rémunération.indemnitaire),
       xlab = "Part des indemnités imposables (< 30 %) dans la rémunération brute en %\n",
       ylab = "Effectif",
       main = "Part indemnitaire de la rémunération annuelle des fonctionnaires en " %+% année,
       xlim = c(0,30),
       col = "blue",
       breaks = 60)
#'   
#'   
#'   
if (longueur.non.na(filtre.fonctionnaire(part.rémunération.indemnitaire[part.rémunération.indemnitaire > 30]) > 0)) 
  hist(filtre.fonctionnaire(part.rémunération.indemnitaire[part.rémunération.indemnitaire > 30]),
       xlab = "Part des indemnités imposables (> 30 %)  dans la rémunération brute en %\n",
       ylab = "Effectif",
       main = "Part indemnitaire de la rémunération annuelle des fonctionnaires en " %+% année,
       xlim = c(30,60),
       col = "blue",
       breaks = 800)

}
