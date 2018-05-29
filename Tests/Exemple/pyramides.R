# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
# produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
# en vigueur à compter de l'année 2008.
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
  

pyramides <- function(Bulletins.début.psr, 
                      Bulletins.fin.psr, 
                      ages.début.psr, 
                      ages.fin.psr,
                      titre,
                      versant,
                      envir = .GlobalEnv) {

e$res <- Résumé(c("Âge des personnels <br>au 31/12/" %+% début.période.sous.revue,
         "Effectif",
         "Âge des personnels <br>au 31/12/" %+% fin.période.sous.revue,
         "Effectif"),
       list(Bulletins.début.psr[ , age], 
            Bulletins.fin.psr[ , age]),
       extra = "length",
       align = 'c',
       type = "standard")



if (longueur.non.na(ages.début.psr) == 0) {
  cat("La pyramide des âges de début de période ne peut être produite.")
  ages.début.psr <- ages.fin.psr
} 

if (longueur.non.na(ages.fin.psr) == 0) {
  cat("La pyramide des âges de fin de période ne peut être produite.")
  ages.fin.psr <- ages.début.psr
}

#  ----- On peut forcer le versant de la FP en renseignant versant
#        sinon détection automatique par VERSANT_FP
 

if (longueur.non.na(ages.début.psr) > 0 || longueur.non.na(ages.fin.psr) > 0) {
  
  if (! is.null(ages.fin.psr)) {
    
    pyramide_ages(ages.début.psr, ages.fin.psr, titre) 

    if (versant != "")  {  
          pyramide_ages(ages.fin.psr, date.fin = fin.période.sous.revue, versant = versant)
    } 
    
  } else {
    
    pyramide_ages(après)
    
  }

  H0 <- ages.début.psr[ , .(Hommes = sum(Hommes, na.rm = TRUE), 
                           Femmes = sum(Femmes, na.rm = TRUE)),
                            by = floor(age / 5)][
                        , Total := Hommes + Femmes]
 
  H1 <- ages.fin.psr[ , .(Hommes = sum(Hommes, na.rm = TRUE), 
                          Femmes = sum(Femmes, na.rm = TRUE)),
                          by = floor(age / 5)][ 
                      , Total := Hommes + Femmes]
  
  H <- H1 - H0
    
} 


newpage()

if  (exists("H") && ! identical(ages.fin.psr, ages.début.psr) && longueur.non.na(H$Total) > 0) {
  
  # la valeur y du plot serait plutôt c(-3,20) pour une sortie R pure. On privilégie le formatage Rmd à c(-1, 20)
  
  plot(c(min(H$Total), max(H$Total)), c(-1, 20), type = "n", frame = FALSE, axes = FALSE, xlab = "", ylab = "",
       main = "Evolution des effectifs par tranche d'âge")
  
  barplot(H$Total, 
          width = 1.5,
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
          xlab = "Variation d'effectifs",
          ylab = "Tranche d'âge",
          xlim = c(min(H$Total), max(H$Total)),
          xpd  = FALSE,
          col  = "lightgreen",
          horiz = TRUE,
          add = TRUE,
          cex.names = 0.8)
  

  barplot(H$Femmes, 
          width=1.5,
          col  = "deeppink",
          horiz = TRUE,
          density = 10,
          xlim = c(min(H$Total), max(H$Total)),
          xpd  = FALSE,
          add = TRUE)
  
  legend("bottomleft", fill=c("lightgreen", "deeppink"), density=c(NA, 20),
         legend=c("Total " %+% début.période.sous.revue %+% "-" %+% fin.période.sous.revue,
                  "    dont Femmes "), cex = 0.8)
  

} else {
  
  cat("Le graphique des variations d'effectifs par tranche d'âge ne peut être produit.")
}
}




