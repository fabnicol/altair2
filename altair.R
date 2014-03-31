installer.paquets(assertthat, plyr, rigoureusement = TRUE)

library(compiler)
library(assertthat)
enableJIT(3)



##---------------------------------------------------------------------------------------------------------------------

# Programme principal
##

options(width=120, warn=-1)

ldp <- ldp[file.exists(chemin(ldp))]
nbi <- nbi[file.exists(chemin(nbi))]
bdp <- bdp[file.exists(chemin(bdp))]

vérifier.intégrité(ldp, nbi, bdp, code.prime)

Ldp <- Read.csv(ldp)
codes.NBI <- Read.csv(nbi)

# Bulletin de paie

Bdp <- read.csv.skip(bdp)

#Matricule.categorie <- read.csv.skip(matricule.categorie)
Code.prime          <- read.csv.skip(code.prime)
#Matricule.avantage  <- read.csv.skip(matricule.avantage)

#suppression des colonnes Nom Prénom redondantes
#Matricule.avantage  <- selectionner.cle.matricule(Matricule.avantage, Matricule.categorie) 

vérifier.intégrité(Bdp, Ldp, codes.NBI, Code.prime)

Bdp                 <- selectionner.cle.matricule.mois(Bdp, Ldp)

#fusion matricule | avantage | catégorie par Matricule

Bdp.ldp <- merge(Bdp, Ldp)

vérifier.intégrité(Bdp.ldp)

#génération du fichier des codes et libellés en privilégiant le Code

if (générer.codes == TRUE) 
{
  Codes.NT  <- Bdp.ldp[ ! Bdp.ldp$Statut %in% c("TITULAIRE", "STAGIAIRE"), c("Code", "Libellé")]
  Codes.NT <- Codes[!duplicated(Codes.NT),]
  Codes.fonct  <- Bdp.ldp[ Bdp.ldp$Statut %in% c("TITULAIRE", "STAGIAIRE"), c("Code", "Libellé")]
  Codes.fonct  <- Codes[!duplicated(Codes.fonct),]
  Codes <- rbind(Code.NT, Codes.fonct)
  
  Codes.NBI <- NBI[ ,"NBI"]  
  Codes.NBI <- Codes.NBI[!duplicated(Codes.NBI),]
  
  sauv.base(Codes.NT)
  sauv.base(Codes.fonct)
  sauv.base(Codes)
  
  stop("no", 0, FALSE)
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


