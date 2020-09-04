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
# 


if (longueur.non.na(filtre.fonctionnaire(Montant.brut.annuel.eqtp) > 0)) 
  hist(filtre.fonctionnaire(Montant.brut.annuel.eqtp)/1000,
       xlab = "En milliers d'euros EQTP\n",
       ylab = "Effectif",
       xlim = c(0, 40),
       main = "Rémunération brute annuelle des fonctionnaires en " %+% annee,
       col = "blue",
       breaks = 250)

  cat("\n\n")


if (longueur.non.na(filtre.fonctionnaire(Montant.brut.annuel.eqtp[Montant.brut.annuel.eqtp > 40000]) > 0)) 
  hist(filtre.fonctionnaire(Montant.brut.annuel.eqtp[Montant.brut.annuel.eqtp > 40000])/1000,
       xlab = "En milliers d'euros EQTP\n",
       ylab = "Effectif",
       xlim = c(40, 120),
       main = "Rémunération brute annuelle des fonctionnaires en " %+% annee,
       col = "blue",
       breaks = 250)

  cat("\n\n")
if (longueur.non.na(filtre.fonctionnaire(rémunération.indemnitaire.imposable.eqtp[rémunération.indemnitaire.imposable.eqtp < 10000]) > 0))
  hist(filtre.fonctionnaire(rémunération.indemnitaire.imposable.eqtp[rémunération.indemnitaire.imposable.eqtp < 10000])/1000,
       xlab = "En milliers d'euros EQTP\n",
       ylab = "Effectif",
       xlim = c(0, 10),
       main = "Rémunération indemnitaire imposable annuelle des fonctionnaires\n  en " %+% annee,
       col = "blue",
       breaks = 150)
  cat("\n\n")
  if (longueur.non.na(filtre.fonctionnaire(rémunération.indemnitaire.imposable.eqtp[rémunération.indemnitaire.imposable.eqtp > 10000]) > 0))
  hist(filtre.fonctionnaire(rémunération.indemnitaire.imposable.eqtp[rémunération.indemnitaire.imposable.eqtp > 10000])/1000,
       xlab = "En milliers d'euros EQTP\n",
       ylab = "Effectif",
       xlim = c(10, 60),
       main = "Rémunération indemnitaire imposable annuelle\n des fonctionnaires en " %+% annee,
       col = "blue",
       breaks = 150)
  cat("\n\n")
if (longueur.non.na(filtre.fonctionnaire(part.rémunération.indemnitaire[part.rémunération.indemnitaire < 30]) > 0)) 
  hist(filtre.fonctionnaire(part.rémunération.indemnitaire[part.rémunération.indemnitaire < 30]),
       xlab = "Part des indemnités imposables (< 30 %) dans la rémunération brute en %\n",
       ylab = "Effectif",
       main = "Part indemnitaire de la rémunération annuelle\n des fonctionnaires en " %+% annee,
       xlim = c(0,30),
       col = "blue",
       breaks = 250)
  cat("\n\n")
if (longueur.non.na(filtre.fonctionnaire(part.rémunération.indemnitaire[part.rémunération.indemnitaire > 30]) > 0)) 
  hist(filtre.fonctionnaire(part.rémunération.indemnitaire[part.rémunération.indemnitaire > 30]),
       xlab = "Part des indemnités imposables (> 30 %)  dans la rémunération brute en %\n",
       ylab = "Effectif",
       main = "Part indemnitaire de la rémunération annuelle\n des fonctionnaires en " %+% annee,
       xlim = c(30,100),
       col = "blue",
       include.lowest=TRUE,
       breaks = seq.int(20, 100, by=2))
  cat("\n\n")

