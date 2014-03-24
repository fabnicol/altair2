library(compiler)
enableJIT(3)

# générer.codes
# générer.distributions
# générer.tests 
# chemin.dossier
# début.période.sous.revue
# fin.période.sous.revue
# champ.détection.1
# champ.détection.2
# étiquette.matricule
# étiquette.montant
# étiquette.totalgeneral
# seuil.troncature
# date.format
# nbi
# nom.de.fichier.de.paie
# matricule.categorie
# code.prime
# matricule.avantage

##---------------------------------------------------------------------------------------------------------------------
#  Chemins et variables globales du programme
##

# Cette section pourra être modifiée en entrée dans d'autres contextes
# Matricule, Codes, Avantages en nature

générer.codes <- TRUE
générer.distributions <- TRUE
générer.tests <- TRUE

chemin.dossier <- "G:/Equipe NICOL/2013/SIERG/GESTION/2-Travail/OBSERVATIONS/ANALYSES/RH"
matricule.categorie <- "LISTES DES PERSONNES REMUNEREES EN 2012 PAR CATEGORIE ET STATUT.csv"     
code.prime          <- "LISTE DES RUBRIQUES DE TRAITEMENT UTILISEES EN 2012.csv"
#matricule.avantage  <- "LISTE DES AGENTS AYANT BENEFICIE D'AVANTAGE EN NATURE EN 2012.csv"

début.période.sous.revue <- 2008
fin.période.sous.revue   <- 2013
étiquette.matricule <- "Matricule"
étiquette.totalgeneral <- "TOTAL GENERAL"
étiquette.montant <- "Net.a.Payer"
seuil.troncature <- 99
nbi <- "paies-NBI-1"

# Le format est jour/mois/année avec deux chiffres-séparateur-deux chiffres-séparateur-4 chiffres.
#Le séparateur peut être changé en un autre en modifiant le "/" dans fate.format

date.format <- "%d/%m/%Y"

nom.de.fichier.de.paie <- "paies-Bulletins de paye-1"

champ.détection.1<-étiquette.matricule
champ.détection.2<-"Matricule"

# Lignes de paie
# On peut lire jusqu'à 10 fichiers csv qui seront générés au format
#  "chemin dossier + paies-Bulletins de paye-j.csv"

ldp <- paste0(nom.de.fichier.de.paie,début.période.sous.revue:fin.période.sous.revue,".csv")
ldp <- paste0("paies-Lignes de paye-",1:10,".csv")

# Bulletins de paie

bdp <- paste0(nom.de.fichier.de.paie, ".csv")

colonnes.sélectionnées <- c("Matricule", "Statut", "Code", "Mois", "Libellé", "Montant")

##---------------------------------------------------------------------------------------------------------------------

options(width=120, warn=-1)

source(chemin("bibliotheque.fonctions.paie.R"), encoding="UTF-8")

# Programme principal
##
#  Bases
##
# Lignes de paie

ldp <- ldp[file.exists(chemin(ldp))]
nbi <- nbi[file.exists(chemin(nbi))]
bdp <- bdp[file.exists(chemin(bdp))]

Ldp <- Read.csv(ldp)
codes.NBI <- Read.csv(nbi)

# Equivaut à :
# ldp1 <- read.csv2(chemin.ldp1,  blank.lines.skip=TRUE, skip=1)
# ...
# ldp <- rbind(ldp1,ldp2,...)

# Bulletin de paie

Bdp <- read.csv.skip(bdp)

#Matricule.categorie <- read.csv.skip(matricule.categorie)
Code.prime          <- read.csv.skip(code.prime)
#Matricule.avantage  <- read.csv.skip(matricule.avantage)

#suppression des colonnes Nom Prénom redondantes

#Matricule.avantage  <- selectionner.cle.matricule(Matricule.avantage, Matricule.categorie) 
Bdp                 <- selectionner.cle.matricule.mois(Bdp, Ldp)

#fusion matricule | avantage | catégorie par Matricule

Bdp.ldp <- merge(Bdp, Ldp)

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
  
  q("no", 0, FALSE)
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
      Bdp.ldp.vacations)
  }


if (générer.variations == TRUE)
  {
    source(chemin("analyser.variations.R"))
    analyser.variations()
    if (générer.bases == TRUE)  
    {
      sauv.base(Total,
        Stats.Analyse.rémunérations.personnels.plus.de.2.ans
        Stats.Analyse.rémunérations.personnels.moins.de.2.ans
        Analyse.rémunérations
        Analyse.rémunérations.filtrée)
    }
 }


