
knitr::opts_chunk$set(fig.width = 7.5, echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')



if (longueur.non.na(filtre.fonctionnaire(Montant.brut.annuel.eqtp) > 0)) 
  hist(filtre.fonctionnaire(Montant.brut.annuel.eqtp)/1000,
       xlab = "En milliers d'euros EQTP\n",
       ylab = "Effectif",
       xlim = c(0, 40),
       main = "Rémunération brute annuelle des fonctionnaires en " %+% année,
       col = "blue",
       breaks = 250)

  cat("\n\n")


if (longueur.non.na(filtre.fonctionnaire(Montant.brut.annuel.eqtp[Montant.brut.annuel.eqtp > 40000]) > 0)) 
  hist(filtre.fonctionnaire(Montant.brut.annuel.eqtp[Montant.brut.annuel.eqtp > 40000])/1000,
       xlab = "En milliers d'euros EQTP\n",
       ylab = "Effectif",
       xlim = c(40, 120),
       main = "Rémunération brute annuelle des fonctionnaires en " %+% année,
       col = "blue",
       breaks = 250)

  cat("\n\n")
if (longueur.non.na(filtre.fonctionnaire(rémunération.indemnitaire.imposable.eqtp[rémunération.indemnitaire.imposable.eqtp < 10000]) > 0))
  hist(filtre.fonctionnaire(rémunération.indemnitaire.imposable.eqtp[rémunération.indemnitaire.imposable.eqtp < 10000])/1000,
       xlab = "En milliers d'euros EQTP\n",
       ylab = "Effectif",
       xlim = c(0, 10),
       main = "Rémunération indemnitaire imposable annuelle des fonctionnaires\n  en " %+% année,
       col = "blue",
       breaks = 150)
  cat("\n\n")
  if (longueur.non.na(filtre.fonctionnaire(rémunération.indemnitaire.imposable.eqtp[rémunération.indemnitaire.imposable.eqtp > 10000]) > 0))
  hist(filtre.fonctionnaire(rémunération.indemnitaire.imposable.eqtp[rémunération.indemnitaire.imposable.eqtp > 10000])/1000,
       xlab = "En milliers d'euros EQTP\n",
       ylab = "Effectif",
       xlim = c(10, 60),
       main = "Rémunération indemnitaire imposable annuelle\n des fonctionnaires en " %+% année,
       col = "blue",
       breaks = 150)
  cat("\n\n")
if (longueur.non.na(filtre.fonctionnaire(part.rémunération.indemnitaire[part.rémunération.indemnitaire < 30]) > 0)) 
  hist(filtre.fonctionnaire(part.rémunération.indemnitaire[part.rémunération.indemnitaire < 30]),
       xlab = "Part des indemnités imposables (< 30 %) dans la rémunération brute en %\n",
       ylab = "Effectif",
       main = "Part indemnitaire de la rémunération annuelle\n des fonctionnaires en " %+% année,
       xlim = c(0,30),
       col = "blue",
       breaks = 250)
  cat("\n\n")
if (longueur.non.na(filtre.fonctionnaire(part.rémunération.indemnitaire[part.rémunération.indemnitaire > 30]) > 0)) 
  hist(filtre.fonctionnaire(part.rémunération.indemnitaire[part.rémunération.indemnitaire > 30]),
       xlab = "Part des indemnités imposables (> 30 %)  dans la rémunération brute en %\n",
       ylab = "Effectif",
       main = "Part indemnitaire de la rémunération annuelle\n des fonctionnaires en " %+% année,
       xlim = c(30,100),
       col = "blue",
       include.lowest=TRUE,
       breaks = seq.int(20, 100, by=2))
  cat("\n\n")

