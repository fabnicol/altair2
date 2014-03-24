library(compiler)
enableJIT(3)

# générer.codes
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

source(file.path(chemin.dossier, "bibliothèque.fonctions.paie.R"), encoding="UTF-8")

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


#SETDIFFLDP <- setdiff(Ldp, Ldp1)

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


#Bdp.ldp<-merge(Bdp.ldp, Matricule.categorie, by=c("Nom"), all=TRUE)

# Matricule.categorie <- selectionner.cle.matricule(Matricule.categorie, Bdp.ldp)
# 
# # Fusion étroite
# Base1 <- merge(Matricule.categorie, Bdp.ldp)
# 
# # Fusion  large
# Base2 <- merge(Matricule.categorie, Bdp.ldp, all=TRUE)

##
#  Test de cohérence sur la fusion
##
# 
# # Matricules différents
# matricules.divergents <- setdiff(levels(as.factor(Base2$Matricule)), levels(as.factor(Base1$Matricule)))
# 
# # Base correspondante
# Base3 <- Base2[!as.character(Base2$Matricule) %in% levels(as.factor(Base1$Matricule)),]

est.code.de.type <- function(x) Bdp.ldp$Code %in% Code.prime[Code.prime$Type.rémunération == x, "Code"]

library(plyr)

Bdp.ldp <- mutate(Bdp.ldp,
                  
                  montant.traitement.indiciaire = Montant*(est.code.de.type("TRAITEMENT")),
                  montant.primes = Montant*(est.code.de.type("INDEMNITAIRE.OU.CONTRACTUEL")),
                  montant.autres.rémunérations = Montant*(est.code.de.type("AUTRES")))


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

detach(Analyse.rémunérations)

##
#  Tests réglementaires
##

attach(Bdp.ldp, warn.conflicts=FALSE)

# Tests du type : incompatibilités entre indemnités et statut

# NBI pour non titulaires

NBI.aux.non.titulaires <- Bdp.ldp[ ! Statut %in% c("TITULAIRE","STAGIAIRE") & as.character(Code) %in% codes.NBI, colonnes.sélectionnées]

nombre.de.ldp.NBI.nontit <- nrow(NBI.aux.non.titulaires)

# heures sup >= 25h/mois

HS.sup.25 <- with(Bdp, Bdp[Heures.Sup. >= 25 , c("Matricule", "Statut", "Mois", "Heures.Sup.", "Brut")])

nombre.de.ldp.HS.sup.25 <- nrow(HS.sup.25)

# Prime de fonctions informatiques : pas dans la base de VLB
# on cherche la chaine de char. "INFO" dans les libellés de primes
# variante : filtre <- regexpr(".*(INFO|PFI|P.F.I).*", toupper(Bdp.ldp$Libellé)) et regmatches(Bdp.ldp$Libellé, filtre)

filtre<-grep(".*(INFO|PFI|P.F.I).*", Libellé, ignore.case=TRUE)

personnels.prime.informatique <- Bdp.ldp[ filtre, colonnes.sélectionnées]

primes.informatiques.potentielles<-unique(Libellé[filtre])

nombre.de.ldp.personnels.pfi <- nrow(personnels.prime.informatique)

# Vacations et statut de fonctionnaire

ldp.fonctionnaires.et.vacations <- Bdp.ldp[ Statut %in% c("TITULAIRE", "STAGIAIRE") & est.code.de.type("VACATIONS"), colonnes.sélectionnées]

nombre.de.ldp.fonctionnaires.et.vacations <- nrow(ldp.fonctionnaires.et.vacations)

# Vacations et régime indemnitaire

Bdp.ldp.vacations <- Bdp.ldp[est.code.de.type("VACATIONS"), colonnes.sélectionnées]

matricules.nontit.et.vacations <- unique(Bdp.ldp.vacations[ ! Bdp.ldp.vacations$Statut %in% c("TITULAIRE", "STAGIAIRE"), "Matricule"])

vacations.concernées <- Bdp.ldp.vacations[Matricule %in% matricules.nontit.et.vacations, ]

RI.et.vacations <- Bdp.ldp[ Matricule %in% matricules.nontit.et.vacations & est.code.de.type("INDEMNITAIRE.OU.CONTRACTUEL"), colonnes.sélectionnées]

traitement.et.vacations <- Bdp.ldp[ Matricule %in% matricules.nontit.et.vacations & est.code.de.type("TRAITEMENT"), colonnes.sélectionnées]

nombre.de.ldp.RI.et.vacations <- nrow(RI.et.vacations)
nombre.de.ldp.traitement.et.vacations <- nrow(traitement.et.vacations)

#IFTS et IB >= 380 (IM >= 350)

filtre.iat<-grep(".*(IAT|I.A.T|.*Adm.*Tech).*", Libellé, ignore.case=TRUE)
filtre.ifts<-grep(".*(IFTS|I.F.T.S|.*FORF.*TRAV.*SUPP).*", Libellé, ignore.case=TRUE)
codes.ifts <- unique(Bdp.ldp[filtre.ifts, "Code"])

sélection.matricules <- intersect(!duplicated(Bdp.ldp[ Indice < 350, c("Matricule")]), !duplicated( Bdp.ldp[ filtre.ifts, c("Matricule")]))

Bdp.ldp[Matricule %in% sélection.matricules & Code %in% codes.ifts & (Indice < 350 ), colonnes.sélectionnées]

rm(sélection.matricules)

# IFTS et non tit

ifts.et.non.tit <- Bdp.ldp[Code %in% codes.ifts & !Statut %in% c("TITULAIRE", "STAGIAIRE"), colonnes.sélectionnées]

# Incomptabilités de primes entre elles

#IAT et IFTS

personnels.iat.ifts <- intersect(as.character(Bdp.ldp[ filtre.iat, c("Matricule")]), as.character(Bdp.ldp[ filtre.ifts, c("Matricule")]))

nb.personnels.iat.ifts <- length(personnels.iat.ifts)

# Primes entre elles : 

pretty.print(nombre.de.ldp.NBI.nontit)
pretty.print(nombre.de.ldp.HS.sup.25)
pretty.print(primes.informatiques.potentielles)
pretty.print(nombre.de.ldp.personnels.pfi)
pretty.print(nombre.de.ldp.fonctionnaires.et.vacations)
pretty.print(nombre.de.ldp.RI.et.vacations)
pretty.print(nombre.de.ldp.traitement.et.vacations)
pretty.print(nb.personnels.iat.ifts)

detach(Bdp.ldp)

## ------------------------------------------------------------------------------------------------------------------
#  Sauvegardes : enlever les commentaires en mode opérationnel
##
# 
# sauv.base(list(Analyse.rémunérations,
# Stats.contractuels,
# Stats.fonctionnaires,
# Bdp.ldp,
# #sauv.base(Base1)
# # sauv.base(Base2, "base2.csv")
# # sauv.base(Base3, "base3.csv")
# # sauv.base(matricules.divergents, "matricules.divergents.csv")
# # sauv.base(data.frame(masse.indemnitaire, masse.indiciaire, masse.rémunérations.brutes, ratio.global.masse.indemnitaire),"stats.masse.csv" )
#  NBI.aux.non.titulaires,
# # sauv.base(HS.sup.25, "HS.sup.25.csv")
# # sauv.base(personnels.prime.informatique, "personnels.prime.informatique.csv")
# ldp.fonctionnaires.et.vacations,
# matricules.nontit.et.vacations,
# Bdp.ldp.vacations))

##---------------------------------------------------------------------------------------------------------------------
#  Analyse des variations
##
##---------------------------------------------------------------------------------------------------------------------
#options(width=120, warn=-1)


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

## ------------------------------------------------------------------------------------------------------------------
#  Sauvegardes : enlever les commentaires en mode opérationnel
##

sauv.base(Total, "Total.csv")
write.csv2(Ldp, chemin("ligne.de.paie.csv"))

sauv.base(Stats.Analyse.rémunérations.personnels.plus.de.2.ans, "Stats.Analyse.rémunérations.personnels.plus.de.2.ans.csv")
sauv.base(Stats.Analyse.rémunérations.personnels.moins.de.2.ans, "Stats.Analyse.rémunérations.personnels.moins.de.2.ans.csv")

sauv.base(Analyse.rémunérations, "Stats.Analyse.rémunérations.personnels.csv")
sauv.base(Analyse.rémunérations.filtrée, "Stats.Analyse.rémunération.filtrée.csv")
