#library(compiler)
#enableJIT(3)

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
libellé.variable.code <- "Code.rubrique"

code.traitement <- 1010

# Lignes de paie
# On peut lire jusqu'à 10 fichiers csv qui seront générés au format
#  "chemin dossier + paies-Bulletins de paye-j.csv"

ldp <- paste0("paies-Lignes de paye-",1:10,".csv")

# Bulletins de paie

bdp <- "paies-Bulletins de paye-1.csv"

codes.NBI <- c("1012", "1011", "101B", "101M", "4652", "4672")

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

Ldp <- Read.csv(ldp)

#génération du fichier des codes et libellés en privilégiant le Code

if (générer.codes == TRUE) 
{
  Codes <- Ldp[,c("Code", "Libellé")]
  Codes <- Codes[!duplicated(Codes),]
  q("no", 0, FALSE)
}


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

est.code.de.type <- function(x) Bdp.ldp$Code %in% Code.prime[Code.prime$Type.rémunération == x, libellé.variable.code]

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

filtre<-grep(".*(INFO|PFI|P.F.I).*", Libellé)

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
