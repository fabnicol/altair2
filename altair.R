
## ------------------  Prologue  ------------------------ ##

library(compiler)
enableJIT(3)
options(width=120, warn=-1)
source("bibliothèque.altair.R")
installer.paquets(assertthat, plyr, rigoureusement = TRUE)
library(assertthat)
source("classes.R")

## ------------------------------------------------------ ##

##
#  Programme principal
##

#génération du fichier des codes et libellés en privilégiant le Code

if (générer.codes == TRUE) 
  {
    Codes.NT     <<- Base[ ! Base$Statut %in% c("TITULAIRE", "STAGIAIRE"), c("Code", "Libellé")]
    Codes.NT     <<- Codes.NT[ ! duplicated(Codes.NT),]
    Codes.fonct  <<- Base[ Bdp.ldp$Statut %in% c("TITULAIRE", "STAGIAIRE"), c("Code", "Libellé")]
    Codes.fonct  <<- Codes.fonct[!duplicated(Codes.fonct),]
    Codes.NBI    <<- NBI[ ,"NBI"]  
    Codes.NBI    <<- Codes.NBI[!duplicated(Codes.NBI),]
    Codes        <<- rbind(Code.NT, Codes.fonct)
    
    sauv.base(Codes.NT, Codes.fonct, Codes.NBI, Codes)
  
  }

if (générer.distributions == TRUE)  
  {
    source(chemin("analyser.distributions.R"))
    analyser.distributions()
    if (générer.bases == TRUE)  
    {
      sauv.base(Analyse.rémunérations,
                Stats.contractuels,
                Stats.fonctionnaires,
                Bdp.ldp,
                masses)
    }
  }

##
#  Tests réglementaires
##

if (générer.tests == TRUE)  
  {
    source(chemin("analyser.tests.R"))
    analyser.tests()
    sauv.base(
      NBI.aux.non.titulaires,
      HS.sup.25,
      personnels.prime.informatique,
      ldp.fonctionnaires.et.vacations,
      matricules.nontit.et.vacations,
      Bdp.ldp.vacations,
      violation.plancher.indiciaire.ifts,
      RI.et.vacations,
      vacations.concernées,
      traitement.et.vacations,
      ifts.et.non.tit)
  }


if (générer.variations == TRUE)
  {
    source(chemin("analyser.variations.R"))
    analyser.variations()
    if (générer.bases == TRUE)  
    {
      sauv.base(Total,
        Stats.Analyse.rémunérations.personnels.plus.de.2.ans,
        Stats.Analyse.rémunérations.personnels.moins.de.2.ans,
        Analyse.rémunérations,
        Analyse.rémunérations.filtrée)
    }
  }
