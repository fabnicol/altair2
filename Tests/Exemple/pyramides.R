
  
Résumé(c("Âge des personnels <br>au 31/12/" %+% début.période.sous.revue,
         "Effectif",
         "Âge des personnels <br>au 31/12/" %+% fin.période.sous.revue,
         "Effectif"),
       list(Bulletins.avant[ , age], 
            Bulletins.après[ , age]),
       extra = "length",
       align = 'c',
       type = "standard")
#' 
#+fig.height=9.7, fig.width=8.4


if (longueur.non.na(avant) > 0 && longueur.non.na(après) > 0) {
  
  pyramide_ages(avant, après, titre)

  if (comparaison.nationale) {
    pyr_fph_2013 <- data.table::fread("Docs/insee_pyramide_fph_2013.csv", dec = ",")
    pyr_fpt_2013 <- data.table::fread("Docs/insee_pyramide_fpt_2013.csv", dec = ",")
    
    tot_fph_2013 <- pyr_fph_2013[ , .(H = sum(Hommes), F = sum(Femmes))]
    tot_fpt_2013 <- pyr_fpt_2013[ , .(H = sum(Hommes), F = sum(Femmes))]
    
    s.après <- après[ , .(H = sum(Hommes, na.rm=TRUE), F = sum(Femmes, na.rm=TRUE))]
    
    H.coef.forme.fph <- s.après$H / tot_fph_2013$H
    F.coef.forme.fph <- s.après$F / tot_fph_2013$F
    H.coef.forme.fpt <- s.après$H / tot_fpt_2013$H
    F.coef.forme.fpt <- s.après$F / tot_fpt_2013$F
    
    
    pyr_fph_2013[ , `:=`(Hommes = Hommes * H.coef.forme.fph,
                         Femmes = Femmes * F.coef.forme.fph)]
    
    pyr_fpt_2013[ , `:=`(Hommes = Hommes * H.coef.forme.fpt,
                         Femmes = Femmes * F.coef.forme.fpt)]  
    newpage()
    
    pyramide_ages(après, pyr_fpt_2013, "Comparaison avec les données nationales du 31 décembre 2013 [FPT]",
                  "organisme " %+% fin.période.sous.revue,
                  "PFT 2013")
    
    cat("Pour obtenir les effectifs nationaux, multiplier les abscisses des hommes par", round(1 / H.coef.forme.fpt * 1000), 
        " et les abscisses des femmes par", round(1 / F.coef.forme.fpt * 1000))
    
    newpage()
    
    pyramide_ages(après, pyr_fph_2013, "Comparaison avec les données nationales du 31 décembre 2013 [FPH]", 
                  "organisme " %+% fin.période.sous.revue,
                  "PFH 2013")
    
    cat("Pour obtenir les effectifs nationaux, multiplier les abscisses des hommes par ", round(1 / H.coef.forme.fph * 1000),
        " et les abscisses des femmes par", round(1 / F.coef.forme.fph * 1000) )
    
    newline()
  }
  
    
  H0 <- avant[ , .(Hommes = sum(Hommes, na.rm = TRUE), 
                   Femmes = sum(Femmes, na.rm = TRUE)),
                 by = floor(age / 5)][ 
               , Total := Hommes + Femmes]
 
  H1 <- après[ , .(Hommes = sum(Hommes, na.rm = TRUE), 
                   Femmes = sum(Femmes, na.rm = TRUE)),
               by = floor(age / 5)][ 
                 , Total := Hommes + Femmes]
  
  H <- H1 - H0
  
}
#'  
#'*Source des comparaisons avec les données nationales*      
#'         
#'Rapport annuel sur l'état de la fonction publique pour 2016      
#'[Pyramide 2013 FPH](`r currentDir`/Docs/insee_pyramide_fph_2013.csv)   
#'[Pyramide 2013 FPT](`r currentDir`/Docs/insee_pyramide_fpt_2013.csv)     

#' 
#+fig.height=9.7, fig.width=8.4 

newpage()

if (longueur.non.na(H$Total) > 0) {
  
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
  

}
#' 




