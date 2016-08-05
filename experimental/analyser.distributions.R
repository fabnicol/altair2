
library(plyr)

base.générateur$methods(
  
  analyser.distributions = function () 
  {
    Global <- mutate(Global,
                     
                     montant.traitement.indiciaire = get(altair$étiquette.montant)*(est.code.de.type(altair$code.traitement)),
                     montant.primes = get(altair$étiquette.montant)*(est.code.de.type(altair$code.prime.ou.contractuel)),
                     montant.autres.rémunérations = get(altair$étiquette.montant)*(est.code.de.type(altair$code.autre)))
    
    Analyse.rémunérations <- ddply(Global,
                                   c(altair$étiquette.matricule, altair$étiquette.catégorie, altair$étiquette$statut),
                                   summarize,
                                   traitement.indiciaire                      = sum(montant.traitement.indiciaire),
                                   rémunération.contractuelle.ou.indemnitaire = sum(montant.primes),
                                   autres.rémunérations                       = sum(montant.autres.rémunérations),
                                   total.rémunérations                        = traitement.indiciaire 
                                                                                 + rémunération.contractuelle.ou.indemnitaire 
                                                                                 + autres.rémunérations,
                                   part.rémunération.contractuelle.ou.indemnitaire = 
                                     ifelse(traitement.indiciaire + rémunération.contractuelle.ou.indemnitaire == 0, 0,
                                            rémunération.contractuelle.ou.indemnitaire /
                                              (traitement.indiciaire + rémunération.contractuelle.ou.indemnitaire)*100),
                                   .parallel = TRUE, progress = "text")
    
    attach(Analyse.rémunérations, warn.conflicts=FALSE)
    
    Analyse.rémunérations <- subset(Analyse.rémunérations,  
                                    total.rémunérations > 0 
                                    & ! is.na(get( altair$étiquette.catégorie)))
    
    Liste.de.bases <- c(Analyse.rémunérations, lapply(levels(Catégories[, altair$étiquette.catégories]), 
                                                      function(x) subset(Analyse.rémunérations, get(altair$étiquette.catégorie) == x)))
    
    # Il faut impérativement que la catégorie soit renseignée pour chaque agent
    # Des problèmes risquent de se poser en cas de changement de catégorie en cours d'année.
    # On peut prendre la convention de la cétgorie au 31 /12.
    # Pour les non fonctionnaires on peut prendre un libellé ad hoc.
    
    summary.analyse.rémunérations <- function(X)
    {
      Stats <- summary(X[  
        , c(altair$étiquette.catégorie, 
            "traitement.indiciaire",
            "rémunération.contractuelle.ou.indemnitaire",
            "autres.rémunérations",
            "total.rémunérations",
            "part.rémunération.contractuelle.ou.indemnitaire")])
      
      masse.indemnitaire            <- sum(rémunération.contractuelle.ou.indemnitaire)
      masse.indiciaire              <- sum(traitement.indiciaire)
      masse.rémunérations.brutes    <- sum(total.rémunérations)
      
      print(data.frame(masse.indemnitaire, masse.indiciaire, masse.rémunérations.brutes))
      
      cat("masse totale rémunérations brutes hors élus : ", rem.brute.hors.élus <- sum(Bulletins[Bulletins$Service != altair$libellé.élus,]$Brut), "\n")
      cat("masse totale rémunérations brutes élus      : ", rem.brute.élus      <- sum(Bulletins[Bulletins$Service == altair$libellé.élus,]$Brut), "\n")
      cat("masse totale rémunérations brutes           : ", rem.brute           <- rem.brute.élus + rem.brute.hors.élus, "\n")
      
      with(Lignes, cat("masse salariale brute : ", sum(get(étiquette.montant)), "\n"))

      print(Stats)
    } 


lapply(Liste.de.bases, summary.analyse.rémunérations)

##
#  Fonctionnaires 
##

par(las=1, lab=c(15,5,6))

histogrammes <- function(condition, nom.statut, color)
{
  mesure <- part.rémunération.contractuelle.ou.indemnitaire[  get(altair$étiquette.statut) %in% ensemble.statut 
                                                            & traitement.indiciaire > 0 
                                                            & part.rémunération.contractuelle.ou.indemnitaire > 0 ]

  hist(mesure,
       xlab="Pourcentage des indemnités dans la rémunération\n hors politique familiale, indemnités journalières et remboursements",
       ylab="Effectif",
       main=paste("Distribution de la part indemnitaire\n de la rémunération annuelle des", nom.statut),
       #xlim=c(min(mesure),max(mesure)+3),     
       col=color,
       nclass=50)
  
  hist(total.rémunérations[traitement.indiciaire > 0 ]/1000,
       xlab="Distribution de la rémunération en k€\n hors politique familiale, indemnités journalières et remboursements",
       ylab="Effectif",
       xlim=c(0, 125),
       main=paste("Rémunération annuelle totale des", nom.statut),
       col=color,
       nclass=50)
  
  hist(rémunération.contractuelle.ou.indemnitaire[traitement.indiciaire > 0 ]/1000,
       xlab="Distribution des indemnités en k€\n hors politique familiale, indemnités journalières et remboursements",
       ylab="Effectif",
       xlim=c(0, 45),
       main=paste("Rémunération indemnitaire annuelle des", nom.statut),
       col=color,
       breaks=50)
  
  # La moyenne est tirée vers le haut par les outlyers
  
  Stats <- summary(Analyse.rémunérations[   substitute(condition)
                                          & traitement.indiciaire > 0
                                          & part.rémunération.contractuelle.ou.indemnitaire > 0,
                                         c("traitement.indiciaire",
                                           "rémunération.contractuelle.ou.indemnitaire",
                                           "autres.rémunérations",
                                           "total.rémunérations",
                                           "part.rémunération.contractuelle.ou.indemnitaire")])
  
  masse.indemnitaire            <- sum(rémunération.contractuelle.ou.indemnitaire[ traitement.indiciaire > 0])
  masse.indiciaire              <- sum(traitement.indiciaire)
  masse.rémunérations.brutes    <- sum(total.rémunérations)
  ratio.global.masse.indemnitaire  <- masse.indemnitaire/(masse.indiciaire+masse.indemnitaire)*100
  
  print(data.frame(masse.indemnitaire, masse.indiciaire, masse.rémunérations.brutes, ratio.global.masse.indemnitaire))
  
  print(Stats)
}

histogrammes(quote(get(altair$étiquette.statut) %in% c(altair$libellé.titulaire, altair$libellé.stagiaire)), "fonctionnaires", "blue")
histogrammes(c(altair$libellé., altair$libellé.stagiaire), "non titulaires", "red")
histogrammes(c(altair$libellé.titulaire, altair$libellé.stagiaire), "fonctionnaires", "blue")

##
#  Contractuels (repérés par traitement indemnitaire == 0) : vacataires et stagiaires incluse
##

hist(total.rémunérations[traitement.indiciaire == 0 ]/1000,
     xlab="Distribution de la rémunération en k€",
     ylab="Effectif",
     xlim=c(0, 4),
     main="Rémunération annuelle totale des contractuels,\n étudiants stagiaires et vacataires",
     col="red",
     breaks=40
)

hist(autres.rémunérations[autres.rémunérations >0],
     xlab="Distribution des autres rémunérations annuelles en € :\n politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 5000),
     main="Autres rémunérations",
     col="grey",
     breaks=50
)


Stats.contractuels <- summary(Analyse.rémunérations[traitement.indiciaire == 0, c("rémunération.contractuelle.ou.indemnitaire", "autres.rémunérations", "total.rémunérations")])

print(Stats.contractuels)

masses <- data.frame(masse.indemnitaire, masse.indiciaire, masse.rémunérations.brutes, ratio.global.masse.indemnitaire)

detach(Analyse.rémunérations)

  })
