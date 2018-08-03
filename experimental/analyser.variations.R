
##---------------------------------------------------------------------------------------------------------------------
#  Analyse des variations
##
##---------------------------------------------------------------------------------------------------------------------
#options(width=120, warn=-1)

library(plyr)

analyser.variations <- function() 
{
  
  julian.date.début.période <- julian(as.Date(paste0("01/01/", début.période.sous.revue), date.format))
  julian.exercice.suivant.premier <- julian(as.Date(paste0("01/01/",(début.période.sous.revue+1)), date.format))
  julian.date.fin.période   <- julian(as.Date(paste0("01/01/", fin.période.sous.revue+1), date.format))
  julian.exercice.dernier <- julian(as.Date(paste0("01/01/",fin.période.sous.revue), date.format))
  
  
  names(Total) <- c(début.période.sous.revue:fin.période.sous.revue)
  
  plot(names(Total),Total,
       ylab="k€",
       las=1,
       xlab="Année",
       main="Rémunération annuelle nette des agents",
       col="red",
       type="b",  
       xlim=c(début.période.sous.revue,fin.période.sous.revue)
  )
  
  Ldp$Matricule<-as.numeric(Ldp$Matricule)
  
  Ldp<-Ldp[order(Ldp$Matricule),]
  
  library(plyr)
  
  calcul.nb.jours <- function(entrée, sortie) 
  {
    
    julian.entrée <- 
      ifelse(entrée == "", 
             julian.date.début.période,
             max(julian.date.début.période, julian(as.Date(entrée, date.format))))
    
    julian.sortie <- 
      ifelse(sortie == "", 
             julian.date.fin.période, 
             min(julian.date.fin.période, julian(as.Date(sortie, date.format))))
    
    return (julian.sortie - julian.entrée)
  }
  
  calcul.nb.jours.dans.exercice.in <- function(entrée) 
  {
    date.entrée <- as.Date(entrée, date.format)
    
    if (entrée == "")
    {
      julian.entrée <-  julian.date.début.période
      julian.fin.exercice <- julian.exercice.suivant.premier
    }
    else
    {
      julian.entrée <- julian(date.entrée)
      if (julian.date.début.période < julian.entrée)
        julian.fin.exercice <- julian(as.Date(paste0("01/01/",as.integer(substr(entrée, 7, 10))+1), date.format))
      else
      {
        julian.fin.exercice <- julian.exercice.suivant.premier
        julian.entrée <- julian.date.début.période
      }
    }
    
    return (julian.fin.exercice - julian.entrée)
    
  }
  
  calcul.nb.jours.dans.exercice.out <- function(sortie) 
  {
    date.sortie <- as.Date(sortie, date.format)
    
    if (sortie == "")
    {
      julian.sortie <-  julian.date.fin.période
      julian.début.exercice <- julian.exercice.dernier
    }
    else
    {
      julian.sortie <- julian(date.sortie)
      if (julian.date.début.période < julian.sortie)
        julian.début.exercice <- julian(as.Date(paste0("01/01/",as.integer(substr(sortie, 7, 10))), date.format))
      else
      {
        julian.début.exercice <- julian.date.début.période
        julian.sortie <- julian.exercice.suivant.premier
      }
    }
    
    return (julian.sortie - julian.début.exercice)
    
  }
  
  calcul.variation <- function(rémunération.début, rémunération.sortie, nb.jours.exercice.début, nb.jours.exercice.sortie, nb.exercices)
  {
    if (nb.exercices > 1)  
      
      return(( rémunération.sortie / rémunération.début   - 1  ) * 100)
    
    else
      
      return (0)
    
  }
  
  Analyse.rémunérations <- ddply(Ldp,
                                 .(Matricule),
                                 summarize,
                                 nb.exercices = length(Net.a.Payer),
                                 entrée = Date.d.entrée[1],
                                 sortie = Date.de.Sortie[nb.exercices],
                                 nb.jours = calcul.nb.jours(entrée, sortie),
                                 nb.jours.exercice.début = calcul.nb.jours.dans.exercice.in(entrée),
                                 nb.jours.exercice.sortie = calcul.nb.jours.dans.exercice.out(sortie),
                                 rémunération.début = Net.a.Payer[1]/nb.jours.exercice.début*365,
                                 rémunération.sortie = Net.a.Payer[nb.exercices]/nb.jours.exercice.sortie*365,
                                 moyenne.rémunération.annuelle.sur.période = sum(Net.a.Payer)*365/nb.jours,
                                 variation.rémunération.jour = calcul.variation(rémunération.début, rémunération.sortie, nb.jours.exercice.début, nb.jours.exercice.sortie, nb.exercices),
                                 variation.moyenne.rémunération.jour = 
                                   ( ( 1 + variation.rémunération.jour / 100 ) ^ (365 / nb.jours) - 1) * 100
  )
  
  Analyse.rémunérations <- mutate(Analyse.rémunérations,
                                  plus.de.2.ans = (nb.jours >= 2*365),
                                  moins.de.2.ans = (nb.jours < 2*365),
                                  moins.de.1.an  = (nb.jours < 365),
                                  moins.de.six.mois = (nb.jours < 365/2))
  
  attach(Analyse.rémunérations, warn.conflicts=FALSE)
  
  nlevels(as.factor(Matricule))
  
  summary(Analyse.rémunérations[ c("plus.de.2.ans",
                                   "moins.de.2.ans",
                                   "moins.de.1.an", 
                                   "moins.de.six.mois")])
  
  
  
  hist(moyenne.rémunération.annuelle.sur.période/1000,
       xlab=paste0("Distribution de la rémunération nette moyenne sur la période ",début.période.sous.revue,"-",fin.période.sous.revue," en k€"),
       ylab="Effectif",
       xlim=c(0, 80),
       main="Rémunération nette moyenne",
       col="blue",
       nclass=100)
  
  
  table(moins.de.2.ans, cut(variation.moyenne.rémunération.jour, breaks=seq(-5,5,by=0.5)))
  
  Analyse.rémunérations.filtrée<-Analyse.rémunérations[ nb.jours.exercice.début > seuil.troncature & nb.jours.exercice.sortie > seuil.troncature & nb.exercices > 1, ]
  
  Stats.Analyse.rémunérations.filtrée<-summary(Analyse.rémunérations.filtrée[-c(1:4)])
  
  print(Stats.Analyse.rémunérations.filtrée)
  
  Analyse.rémunérations.personnels.plus.de.2.ans <- Analyse.rémunérations.filtrée[Analyse.rémunérations.filtrée$plus.de.2.ans,]
  
  Stats.Analyse.rémunérations.personnels.plus.de.2.ans<-summary(Analyse.rémunérations.personnels.plus.de.2.ans[-c(1:4)])
  
  print(Stats.Analyse.rémunérations.personnels.plus.de.2.ans)
  
  
  Analyse.rémunérations.personnels.moins.de.2.ans <- Analyse.rémunérations[moins.de.2.ans,]
  
  Stats.Analyse.rémunérations.personnels.moins.de.2.ans<-summary(Analyse.rémunérations.personnels.moins.de.2.ans)
  
  print(Stats.Analyse.rémunérations.personnels.moins.de.2.ans)
  
  table(Analyse.rémunérations.filtrée$plus.de.2.ans, cut(Analyse.rémunérations.filtrée$variation.moyenne.rémunération.jour, breaks=seq(-10,35,by=0.5)))
  
  hist(Analyse.rémunérations.personnels.plus.de.2.ans$variation.moyenne.rémunération.jour,
       xlab ="Variation annuelle moyenne de la rémunération en %",
       las=1,
       sub  = "pour 41 agents restés plus de deux ans",
       ylab ="Effectifs",
       main ="Distribution de la variation annuelle\nmoyenne de la rémunération des agents en place",
       col ="red",
       nclass=100
  )
  
  
  hist(Analyse.rémunérations.personnels.moins.de.2.ans$variation.moyenne.rémunération.jour,
       xlab ="Variation annuelle moyenne de la rémunération en %",
       xlim=c(-10,30),
       las=1,
       sub  = "pour 30 agents restés moins de deux ans",
       ylab ="Effectifs",
       main ="Distribution de la variation annuelle\nmoyenne de la rémunération des agents restés moins de deux ans",
       col ="red",
       nclass=100
  )
  
  detach(Analyse.rémunérations)
  
  Ldp <- mutate(Ldp,
                plus.de.2.ans = Matricule
                %in%
                  Analyse.rémunérations[Analyse.rémunérations$plus.de.2.ans, étiquette.matricule])
  attach(Ldp, warn=-1)
  
  Ldp.plus.de.2.ans<-Ldp[plus.de.2.ans, ]
  
  plot(levels(as.factor(Ldp.plus.de.2.ans$Année)),
       tapply(Ldp.plus.de.2.ans$Net.a.Payer, Ldp.plus.de.2.ans$Année, sum)/1000,
       ylab="k€",
       las=1,
       xlab="Année",
       main="Rémunération annuelle nette des agents\nrestés plus de deux ans",
       col="red",
       type="b",  
       xlim=c(début.période.sous.revue,fin.période.sous.revue)
  )
  
  
  Ldp.moins.de.2.ans<-Ldp[!plus.de.2.ans, ]
  
  plot(levels(as.factor(Ldp.moins.de.2.ans$Année)),
       tapply(Ldp.moins.de.2.ans$Net.a.Payer, Ldp.moins.de.2.ans$Année, sum)/1000,
       ylab="k€",
       las=1,
       xlab="Année",
       main="Rémunération annuelle nette des agents\n restés moins de deux ans",
       col="blue",
       type="b",  
       xlim=c(début.période.sous.revue,fin.période.sous.revue)
  )
  
  plot(levels(as.factor(Ldp.moins.de.2.ans$Année)),
       sapply(levels(as.factor(Ldp.moins.de.2.ans$Année)) ,function(x) nlevels(as.factor(Ldp.moins.de.2.ans[Ldp.moins.de.2.ans$Année == x, ]$Matricule))),
       ylab="Effectif",
       las=1,
       xlab="Année",
       main="Nombre d'agents\n restés moins de deux ans",
       col="blue",
       type="b",  
       xlim=c(début.période.sous.revue,fin.période.sous.revue)
  )
  
  plot(levels(as.factor(Ldp.plus.de.2.ans$Année)),
       sapply(levels(as.factor(Ldp.plus.de.2.ans$Année)) ,function(x) nlevels(as.factor(Ldp.plus.de.2.ans[Ldp.plus.de.2.ans$Année == x, ]$Matricule))),
       ylab="Effectif",
       las=1,
       xlab="Année",
       main="Nombre d'agents\n restés plus de deux ans",
       col="blue",
       type="b",  
       xlim=c(début.période.sous.revue,fin.période.sous.revue)
  )
  
  plot(levels(as.factor(Année)),
       sapply(levels(as.factor(Année)), function(x) nlevels(as.factor(Ldp[Année == x, ]$Matricule))),
       ylab="Effectif",
       las=1,
       xlab="Année",
       main="Nombre d'agents rémunérés par exercice",
       col="green",
       type="b",  
       xlim=c(début.période.sous.revue,fin.période.sous.revue)
  )
  
  table(plus.de.2.ans, Année)
  
  detach(Ldp)
}