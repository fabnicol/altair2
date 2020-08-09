
#'## 1.6 Effectifs des personnels par durée de service
#'
cat("**Personnels en fonction (hors élus) des exercices ", début.période.sous.revue, " à ", fin.période.sous.revue," inclus :**\n")
#'

#'  
#'&nbsp;*Tableau 1.6.1*   
#'    

#+ effectifs-par-durée

Tableau(c("Plus de 2 ans",
          "Moins de 2 ans",
          "Moins d'un an",
          "Moins de six mois"),
        sum(Analyse.variations$plus.2.ans, na.rm=TRUE),
        sum(Analyse.variations$moins.2.ans, na.rm=TRUE),
        sum(Analyse.variations$moins.1.an, na.rm=TRUE),
        sum(Analyse.variations$moins.six.mois, na.rm=TRUE))


#'

if (nrow(Analyse.variations) > 0)
  ggplot2::qplot(factor(Annee),
                 data = Analyse.variations,
                 geom = "bar",
                 fill = factor(!Analyse.variations$plus.2.ans),
                 main = "Evolutions entre " %+% début.période.sous.revue %+% " et " %+% fin.période.sous.revue,
                 xlab = "Annee",
                 ylab = "Effectif",
                 asp = 1.4)        +
  ggplot2::scale_fill_discrete(name = "Personnels (non élus) en fonction",
                               breaks = c(TRUE, FALSE),
                               labels = c("Moins de deux ans", "Plus de deux ans"))



#'
#'**Effectifs (hors élus)**   
#+ tableau-effectifs

effectifs.var <- lapply(période,
                        function(x) {
                          
                          E <- unique(Analyse.variations[Annee == x , .(Matricule, plus.2.ans)], by=NULL)
                          F <- E[plus.2.ans == TRUE]
                          tot <- nrow(E)
                          plus.2.ans <- nrow(F)
                          résultat <- c(plus.2.ans, tot - plus.2.ans, tot)
                          rm(E, F, tot, plus.2.ans)
                          résultat
                        })

effectifs.var.locale <- lapply(effectifs.var, function(x) formatC(x, big.mark = " ", format="f", digits=0, decimal.mark=","))

tableau.effectifs.var <- as.data.frame(effectifs.var.locale, row.names = c("Plus de deux ans", "Moins de deux ans", "Total"))

names(tableau.effectifs.var) <- as.character(période)
#'  
#'&nbsp;*Tableau 1.6.2*   
#'    

kable(tableau.effectifs.var, row.names = TRUE, align='c')
#'
#'
#'**Nota :**
#'*Personnels en place : ayant servi au moins deux années consécutives pendant la période.*     
#'*Plus/moins de deux ans : plus/mois de 730 jours sur la période sous revue.*   
#'


#+ lancement-analyse-statique
