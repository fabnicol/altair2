library(plyr)

analyser.distributions <- function () 
{
  
  Bdp.ldp <- mutate(Bdp.ldp,
                    
                    montant.traitement.indiciaire = Montant*(est.code.de.type(code.traitement)),
                    montant.primes = Montant*(est.code.de.type(code.prime.ou.contractuel)),
                    montant.autres.rémunérations = Montant*(est.code.de.type(code.autres)))
  
  
  Analyse.rémunérations <- ddply(Bdp.ldp,
                                 .(Matricule, Service),
                                 summarize,
                                 #                             catégorie = Code.catégorie[1],
                                 traitement.indiciaire                      = sum(montant.traitement.indiciaire),
                                 rémunération.contractuelle.ou.indemnitaire = sum(montant.primes),
                                 autres.rémunérations                       = sum(montant.autres.rémunérations),
                                 total.rémunérations                        = traitement.indiciaire 
                                 + rémunération.contractuelle.ou.indemnitaire 
                                 + autres.rémunérations,
                                 part.rémunération.contractuelle.ou.indemnitaire = 
                                   ifelse(traitement.indiciaire + rémunération.contractuelle.ou.indemnitaire == 0, 0,
                                          rémunération.contractuelle.ou.indemnitaire /
                                            (traitement.indiciaire + rémunération.contractuelle.ou.indemnitaire)*100))
  
  attach(Analyse.rémunérations, warn.conflicts=FALSE)
  
  ##
  #  Stats globales
  ##
  
  Stats <- summary(Analyse.rémunérations[total.rémunérations > 0, c("traitement.indiciaire", "rémunération.contractuelle.ou.indemnitaire", "autres.rémunérations", "total.rémunérations", "part.rémunération.contractuelle.ou.indemnitaire")])
  
  masse.indemnitaire            <- sum(rémunération.contractuelle.ou.indemnitaire[ total.rémunérations > 0])
  masse.indiciaire              <- sum(traitement.indiciaire[ total.rémunérations > 0])
  masse.rémunérations.brutes    <- sum(total.rémunérations[ total.rémunérations > 0])
  
  print(data.frame(masse.indemnitaire, masse.indiciaire, masse.rémunérations.brutes))
  
  cat("masse totale rémunérations brutes hors élus : ", rem.brute.hors.élus <- sum(Bdp[Bdp$Service != "Elus",]$Brut), "\n")
  cat("masse totale rémunérations brutes élus      : ", rem.brute.élus      <- sum(Bdp[Bdp$Service == "Elus",]$Brut), "\n")
  cat("masse totale rémunérations brutes           : ", rem.brute           <- rem.brute.élus + rem.brute.hors.élus, "\n")
  
  cat("masse salariale brutes : ", sum(Bdp.ldp$Montant), "\n")
  print(Stats)
  
  
  # Stats.C <- summary(Analyse.rémunérations[total.rémunérations > 0 & catégorie == "C", c("traitement.indiciaire", "rémunération.contractuelle.ou.indemnitaire", "autres.rémunérations", "total.rémunérations", "part.rémunération.contractuelle.ou.indemnitaire")])
  # 
  # masse.indemnitaire.C            <- sum(rémunération.contractuelle.ou.indemnitaire[ total.rémunérations > 0 & catégorie == "C" & ! is.na(rémunération.contractuelle.ou.indemnitaire) & ! is.na(catégorie)])
  # masse.indiciaire.C              <- sum(traitement.indiciaire[ total.rémunérations > 0 & catégorie == "C" & ! is.na(traitement.indiciaire) & ! is.na(catégorie)])
  # masse.rémunérations.brutes.C    <- sum(total.rémunérations[ total.rémunérations > 0 & catégorie == "C" & ! is.na(total.rémunérations) & ! is.na(catégorie)])
  # 
  # print(data.frame(masse.indemnitaire.C, masse.indiciaire.C, masse.rémunérations.brutes.C))
  # 
  # print(Stats.C)
  
  
  
  ##
  #  Fonctionnaires (repérés par traitement indemnitaire > 0) : fonctionnaires stagiaires inclus (à vérif.)
  ##
  
  par(las=1,
      lab=c(15,5,6))
  
  mesure <- part.rémunération.contractuelle.ou.indemnitaire[traitement.indiciaire > 0 & part.rémunération.contractuelle.ou.indemnitaire >0]
  
  hist(mesure,
       xlab="Pourcentage des indemnités dans la rémunération\n hors politique familiale, indemnités journalières et remboursements",
       ylab="Effectif",
       main="Distribution de la part indemnitaire\n de la rémunération annuelle des fonctionnaires",
       #xlim=c(min(mesure),max(mesure)+3),     
       col="blue",
       nclass=50
  )
  
  
  
  hist(total.rémunérations[traitement.indiciaire > 0 ]/1000,
       xlab="Distribution de la rémunération en k€\n hors politique familiale, indemnités journalières et remboursements",
       ylab="Effectif",
       xlim=c(0, 125),
       main="Rémunération annuelle totale des fonctionnaires",
       col="blue",
       nclass=50
  )
  
  hist(rémunération.contractuelle.ou.indemnitaire[traitement.indiciaire > 0 ]/1000,
       xlab="Distribution des attributions indemnitaires en k€\n hors politique familiale, indemnités journalières et remboursements",
       ylab="Effectif",
       xlim=c(0, 45),
       main="Rémunération indemnitaire annuelle des fonctionnaires",
       col="blue",
       breaks=50
  )
  
  # La moyenne est tirée vers le haut par les outlyers
  
  Stats.fonctionnaires <- summary(Analyse.rémunérations[traitement.indiciaire > 0, c("traitement.indiciaire", "rémunération.contractuelle.ou.indemnitaire", "autres.rémunérations", "total.rémunérations", "part.rémunération.contractuelle.ou.indemnitaire")])
  
  masse.indemnitaire            <- sum(rémunération.contractuelle.ou.indemnitaire[ traitement.indiciaire > 0])
  masse.indiciaire              <- sum(traitement.indiciaire)
  masse.rémunérations.brutes    <- sum(total.rémunérations)
  ratio.global.masse.indemnitaire  <- masse.indemnitaire/(masse.indiciaire+masse.indemnitaire)*100
  
  print(data.frame(masse.indemnitaire, masse.indiciaire, masse.rémunérations.brutes, ratio.global.masse.indemnitaire))
  
  print(Stats.fonctionnaires)
  
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
  
}
