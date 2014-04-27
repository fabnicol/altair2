library(compiler)
enableJIT(3)

##---------------------------------------------------------------------------------------------------------------------
#  Chemins et variables globales du programme
##

# Cette section pourra être modifiée en entrée dans d'autres contextes
# Matricule, Codes, Avantages en nature

chemin.dossier      <- "C:/Users/fnicol/Desktop/Bureau/Informatique/VLB"
matricule.categorie <- "LISTES DES PERSONNES REMUNEREES EN 2012 PAR CATEGORIE ET STATUT.csv"     
code.prime          <- "LISTE DES RUBRIQUES DE TRAITEMENT UTILISEES EN 2012.csv"
matricule.avantage  <- "LISTE DES AGENTS AYANT BENEFICIE D'AVANTAGE EN NATURE EN 2012.csv"
code.traitement     <- 1010

# Lignes de paie
# On peut lire jusqu'à 10 fichiers csv qui seront générés au format
#  "chemin dossier + paies-Bulletins de paye-j.csv"

ldp <- paste0("paies-Lignes de paye-",1:10,".csv")

# Bulletins de paie

bdp <- "paies-Bulletins de paye-1.csv"
bdp.nir <- "paies-Bulletins de paye-1.nir.csv"

codes.NBI <- c("1012", "101B", "101M", "4652", "4672")

##---------------------------------------------------------------------------------------------------------------------
options(width=120, warn=-1)
source(file.path(chemin.dossier, "bibliothèque.fonctions.paie.R"), encoding="UTF-8")

# Programme principal
##
#  Bases
##
# Lignes de paie

ldp <- ldp[file.exists(chemin(ldp))]

Ldp<-Read.csv(ldp)

# Equivaut à :
# ldp1 <- read.csv2(chemin.ldp1,  blank.lines.skip=TRUE, skip=1)
# ...
# ldp <- rbind(ldp1,ldp2,...)
# Bulletin de paie

Bdp <- read.csv.skip(bdp)
Bdp.nir <- read.csv.skip(bdp.nir)

nir <- "nir.csv"

NIR <- read.csv.skip(nir)

liste.matricules.fonctionnaires <- unique(Bdp.nir[Bdp.nir$Statut %in% c("TITULAIRE", "STAGIAIRE"), c("Matricule")])
liste.matricules.élus <- unique(Bdp.nir[ Bdp.nir$Service == "ELUS", c("Matricule")])

NIR <- NIR[! NIR$Matricule %in% liste.matricules.élus, ]

NIR2 <- NIR[NIR$Matricule %in% liste.matricules.fonctionnaires, ]

années2 <- substr(as.character(NIR2$Nir), 2, 3)

mois2 <- substr(as.character(NIR2$Nir), 4, 5)

age2 <- 2011-(as.numeric(années2)+1900)-1

hist(age2,
     xlab="Age",
     xlim=c(18,65),
     ylab="Effectif",
     main="Pyramide des âges des fonctionnaires",
     col="blue",
     nclass=50)

summary(age2)

années <- substr(as.character(NIR$Nir), 2, 3)

mois <- substr(as.character(NIR$Nir), 4, 5)

age <- 2011-(as.numeric(années)+1900)-1

hist(age,
     xlab="Age",
     xlim=c(18,65),
     ylab="Effectif",
     main="Pyramide des âges\nEffectif total hors élus",
     col="blue",
     nclass=50)

summary(age)

Matricule.categorie <- read.csv.skip(matricule.categorie)
Code.prime          <- read.csv.skip(code.prime)
#Matricule.avantage  <- read.csv.skip(matricule.avantage)

#suppression des colonnes Nom Prénom redondantes

#Matricule.avantage  <- selectionner.cle.matricule(Matricule.avantage, Matricule.categorie) 
Bdp                 <- selectionner.cle.matricule.mois(Bdp, Ldp)

#fusion matricule | avantage | catégorie par Matricule

Bdp.ldp <- merge(Bdp, Ldp)

Codes <- Bdp.ldp[,c("Code", "Libellé")]
Codes.NT  <- Bdp.ldp[ ! Bdp.ldp$Statut %in% c("TITULAIRE", "STAGIAIRE"), c("Code", "Libellé")]
Codes.NT <- Codes[!duplicated(Codes.NT),]

Matricule.categorie <- selectionner.cle.matricule(Matricule.categorie, Bdp.ldp)

# Fusion étroite
Base1 <- merge(Matricule.categorie, Bdp.ldp)

# Fusion  large
Base2 <- merge(Matricule.categorie, Bdp.ldp, all=TRUE)

##
#  Test de cohérence sur la fusion
##

# Matricules différents
matricules.divergents <- setdiff(levels(as.factor(Base2$Matricule)), levels(as.factor(Base1$Matricule)))

# Base correspondante
Base3 <- Base2[!as.character(Base2$Matricule) %in% levels(as.factor(Base1$Matricule)),]

library(plyr)

Base2 <- mutate(Base2,
       
        montant.traitement.indiciaire = Montant*(Code %in% Code.prime[Code.prime$Type.rémunération == "TRAITEMENT","Code.rubrique"]),
        
        montant.vacations = Montant*(Code %in% Code.prime[  Code.prime$Type.rémunération == "VACATIONS",
                                                           "Code.rubrique"]),
        
        montant.primes.hors.vacations = Montant*(Code %in% Code.prime[ Code.prime$Type.rémunération == "INDEMNITAIRE.OU.CONTRACTUEL",
                                                        "Code.rubrique"]),
        
        montant.autres.rémunérations = Montant*(Code %in% Code.prime[Code.prime$Type.rémunération == "AUTRES","Code.rubrique"])
       )


Analyse.rémunérations <- ddply(Base2,
                             .(Matricule, Statut, Code.catégorie),
                             summarize,
                             traitement.indiciaire                      = sum(montant.traitement.indiciaire),
                             vacations                                  = sum(montant.vacations),
                             rémunération.contractuelle.ou.indemnitaire = sum(montant.primes.hors.vacations)+vacations,
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
#  Fonctionnaires 
##

hist(part.rémunération.contractuelle.ou.indemnitaire[Statut %in% c("TITULAIRE", "STAGIAIRE") & part.rémunération.contractuelle.ou.indemnitaire >0],
   xlab="Pourcentage des indemnités dans la rémunération\n hors politique familiale, indemnités journalières et remboursements",
   ylab="Effectif",
   main="Distribution de la part indemnitaire\n de la rémunération annuelle des fonctionnaires",
   xlim=c(0,60),     
   col="blue",
   breaks=50)


hist(total.rémunérations[Statut %in% c("TITULAIRE", "STAGIAIRE") ]/1000,
   xlab="Distribution de la rémunération en k€\n hors politique familiale, indemnités journalières et remboursements",
   ylab="Effectif",
   xlim=c(0, 120),
   main="Rémunération annuelle totale des fonctionnaires",
   col="blue",
   breaks=100)

rémunération.indemnitaire.tot.fonct <- rémunération.contractuelle.ou.indemnitaire[Statut %in% c("TITULAIRE", "STAGIAIRE") ]

hist(rémunération.indemnitaire.tot.fonct/1000,
     xlab="Distribution des attributions indemnitaires en k€\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 15),
     main="Rémunération indemnitaire\n annuelle des fonctionnaires",
     col="blue",
     breaks=100)

message("Rémunération indemnitaire totale des fonctionnaires")
summary(rémunération.indemnitaire.tot.fonct)

message("Rémunération indemnitaire totale des fonctionnaires ( > 15k€)")
summary(rémunération.indemnitaire.tot.fonct[rémunération.indemnitaire.tot.fonct > 15000])
length(rémunération.indemnitaire.tot.fonct[rémunération.indemnitaire.tot.fonct > 15000])

hist(vacations[vacations > 0]/1000,
     xlab="en k€",
     ylab="Effectif",
     main="Rémunération des vacations",
     col="green",
     breaks=100)

# La moyenne est tirée vers le haut par les outlyers

Stats.fonctionnaires <- summary(Analyse.rémunérations[Statut %in% c("TITULAIRE", "STAGIAIRE"), c("traitement.indiciaire", "rémunération.contractuelle.ou.indemnitaire", "autres.rémunérations", "total.rémunérations", "part.rémunération.contractuelle.ou.indemnitaire")])

masse.indemnitaire            <- sum(rémunération.contractuelle.ou.indemnitaire[ Statut %in% c("TITULAIRE", "STAGIAIRE") ])
masse.indiciaire              <- sum(traitement.indiciaire[ Statut %in% c("TITULAIRE", "STAGIAIRE") ])
masse.rémunérations.brutes    <- sum(total.rémunérations[ Statut %in% c("TITULAIRE", "STAGIAIRE") ])
ratio.global.masse.indemnitaire  <- masse.indemnitaire/(masse.indiciaire+masse.indemnitaire)*100

print(data.frame(masse.indemnitaire, masse.indiciaire, masse.rémunérations.brutes, ratio.global.masse.indemnitaire))

print(Stats.fonctionnaires)

# Fonctionnaires Catégorie A 

hist(part.rémunération.contractuelle.ou.indemnitaire[Statut %in% c("TITULAIRE", "STAGIAIRE") & Code.catégorie == "A" & part.rémunération.contractuelle.ou.indemnitaire >0],
     xlab="Pourcentage des indemnités dans la rémunération\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     main="Distribution de la part indemnitaire\n de la rémunération annuelle des fonctionnaires de catégorie A",
     xlim=c(0,60),     
     col="blue",
     breaks=50)

hist(total.rémunérations[Statut %in% c("TITULAIRE", "STAGIAIRE") & Code.catégorie == "A"]/1000,
     xlab="Distribution de la rémunération en k€\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 120),
     main="Rémunération annuelle totale\n des fonctionnaires de catégorie A",
     col="blue",
     breaks=100)

rémunération.indemnitaire.tot.fonct.A <- rémunération.contractuelle.ou.indemnitaire[Statut %in% c("TITULAIRE", "STAGIAIRE") & Code.catégorie == "A"]

hist(rémunération.indemnitaire.tot.fonct.A/1000,
     xlab="Distribution des attributions indemnitaires en k€\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 15),
     main="Rémunération indemnitaire annuelle\n des fonctionnaires de catégorie A",
     col="blue",
     breaks=100)

message("Rémunération indemnitaire totale des fonctionnaires de catégorie A")
summary(rémunération.indemnitaire.tot.fonct.A)

message("Rémunération indemnitaire totale des fonctionnaires  de catégorie A ( > 15k€)")
summary(rémunération.indemnitaire.tot.fonct.A[rémunération.indemnitaire.tot.fonct.A > 15000])
length(rémunération.indemnitaire.tot.fonct.A[rémunération.indemnitaire.tot.fonct.A > 15000])


# La moyenne est tirée vers le haut par les outlyers

Stats.fonctionnaires.A <- summary(Analyse.rémunérations[Statut %in% c("TITULAIRE", "STAGIAIRE") & Code.catégorie == "A", c("traitement.indiciaire", "rémunération.contractuelle.ou.indemnitaire", "autres.rémunérations", "total.rémunérations", "part.rémunération.contractuelle.ou.indemnitaire")])

masse.indemnitaire.A            <- sum(rémunération.contractuelle.ou.indemnitaire[ Statut %in% c("TITULAIRE", "STAGIAIRE") & Code.catégorie == "A"])
masse.indiciaire.A              <- sum(traitement.indiciaire[ Statut %in% c("TITULAIRE", "STAGIAIRE") & Code.catégorie == "A"])
masse.rémunérations.brutes.A    <- sum(total.rémunérations[ Statut %in% c("TITULAIRE", "STAGIAIRE") & Code.catégorie == "A"])
ratio.global.masse.indemnitaire.A  <- masse.indemnitaire.A/(masse.indiciaire.A + masse.indemnitaire.A)*100

print(data.frame(masse.indemnitaire.A, masse.indiciaire.A, masse.rémunérations.brutes.A, ratio.global.masse.indemnitaire.A))

print(Stats.fonctionnaires.A)


# Fonctionnaires Catégorie C 

hist(part.rémunération.contractuelle.ou.indemnitaire[Statut %in% c("TITULAIRE", "STAGIAIRE") & Code.catégorie == "C" & part.rémunération.contractuelle.ou.indemnitaire >0],
     xlab="Pourcentage des indemnités dans la rémunération\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     main="Distribution de la part indemnitaire\n de la rémunération annuelle des fonctionnaires de catégorie C",
     col="blue",
     breaks=50)

hist(total.rémunérations[Statut %in% c("TITULAIRE", "STAGIAIRE") & Code.catégorie == "C"]/1000,
     xlab="Distribution de la rémunération en k€\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     main="Rémunération annuelle totale\n des fonctionnaires de catégorie C",
     col="blue",
     breaks=50)

rémunération.indemnitaire.tot.fonct.C <- rémunération.contractuelle.ou.indemnitaire[Statut %in% c("TITULAIRE", "STAGIAIRE") & Code.catégorie == "C"]

hist(rémunération.indemnitaire.tot.fonct.C/1000,
     xlab="Distribution des attributions indemnitaires en k€\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     main="Rémunération indemnitaire annuelle\n des fonctionnaires de catégorie C",
     col="blue",
     breaks=50)

message("Rémunération indemnitaire totale des fonctionnaires de catégorie C")
summary(rémunération.indemnitaire.tot.fonct.C)

message("Rémunération indemnitaire totale des fonctionnaires  de catégorie C ( > 15k€)")
summary(rémunération.indemnitaire.tot.fonct.C[rémunération.indemnitaire.tot.fonct.C > 15000])
length(rémunération.indemnitaire.tot.fonct.C[rémunération.indemnitaire.tot.fonct.C > 15000])


# La moyenne est tirée vers le haut par les outlyers

Stats.fonctionnaires.C <- summary(Analyse.rémunérations[Statut %in% c("TITULAIRE", "STAGIAIRE") & Code.catégorie == "C", c("traitement.indiciaire", "rémunération.contractuelle.ou.indemnitaire", "autres.rémunérations", "total.rémunérations", "part.rémunération.contractuelle.ou.indemnitaire")])

masse.indemnitaire.C            <- sum(rémunération.contractuelle.ou.indemnitaire[ Statut %in% c("TITULAIRE", "STAGIAIRE") & Code.catégorie == "C"])
masse.indiciaire.C              <- sum(traitement.indiciaire[ Statut %in% c("TITULAIRE", "STAGIAIRE") & Code.catégorie == "C"])
masse.rémunérations.brutes.C    <- sum(total.rémunérations[ Statut %in% c("TITULAIRE", "STAGIAIRE") & Code.catégorie == "C"])
ratio.global.masse.indemnitaire.C  <- masse.indemnitaire.C/(masse.indiciaire.C + masse.indemnitaire.C)*100

print(data.frame(masse.indemnitaire.C, masse.indiciaire.C, masse.rémunérations.brutes.C, ratio.global.masse.indemnitaire.C))

print(Stats.fonctionnaires.C)

##
#  Contractuels  vacataires et stagiaires inclus
##

hist(total.rémunérations[! Statut %in% c("TITULAIRE", "STAGIAIRE") & total.rémunérations > 1000]/1000,
   xlab="Distribution de la rémunération en k€ (supérieure à 1000 €)\n hors politique familiale, indemnités journalières et remboursements",
   ylab="Effectif",
   xlim=c(0, 40),
   main="Rémunération annuelle totale des contractuels",
   col="red",
   breaks=40)

hist(autres.rémunérations[autres.rémunérations >0],
     xlab="Distribution des autres rémunérations annuelles en € :\n politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 5000),
     main="Autres rémunérations",
     col="grey",
     breaks=40)


Stats.contractuels <- summary(Analyse.rémunérations[! Statut %in% c("TITULAIRE", "STAGIAIRE"), c("rémunération.contractuelle.ou.indemnitaire", "autres.rémunérations", "total.rémunérations")])

print(Stats.contractuels)

detach(Analyse.rémunérations)


##
#  Tests réglementaires
##

attach(Bdp.ldp, warn.conflicts=FALSE)

# NBI pour non titulaires

NBI.aux.non.titulaires <- Bdp.ldp[ ! Statut %in% c("TITULAIRE","STAGIAIRE") & as.character(Code) %in% codes.NBI, c("Matricule", "Statut", "Code", "Libellé", "Mois", "Montant")]

nombre.de.ldp.NBI.nontit <- nrow(NBI.aux.non.titulaires)

# heures sup >= 25h/mois

HS.sup.25 <- with(Bdp, Bdp[Heures.Sup. >= 25 , c("Matricule", "Statut", "Mois", "Heures.Sup.", "Brut")])

nombre.de.ldp.HS.sup.25 <- nrow(HS.sup.25)

# Prime de fonctions informatiques : pas dans la base de VLB
# on cherche la chaine de char. "INFO" dans les libellés de primes

# variante : filtre <- regexpr(".*(INFO|PFI|P.F.I).*", toupper(Bdp.ldp$Libellé)) et regmatches(Bdp.ldp$Libellé, filtre)

filtre<-grep(".*(INFO|PFI|P.F.I).*", Libellé)

personnels.prime.informatique <- Bdp.ldp[ filtre, c("Matricule", "Statut", "Code", "Libellé", "Montant")]

primes.informatiques.potentielles<-unique(Libellé[filtre])
  
nombre.de.ldp.personnels.pfi <- nrow(personnels.prime.informatique)

# Vacations et statut de fonctionnaire

fonctionnaires.et.vacations <- Bdp.ldp[ Statut %in% c("TITULAIRE", "STAGIAIRE") & Code %in% Code.prime[Code.prime$Type.rémunération == "VACATIONS","Code.rubrique"], c("Matricule", "Statut", "Code", "Libellé", "Montant")]

nombre.de.ldp.fonctionnaires.et.vacations <- nrow(fonctionnaires.et.vacations)

# Vacations et régime indemnitaire

matricules.RI.et.vacations <- unique(Bdp.ldp[ ! Statut %in% c("TITULAIRE", "STAGIAIRE") & Code %in% Code.prime[Code.prime$Type.rémunération == "VACATIONS","Code.rubrique"], "Matricule"])

vacations.concernées <- Bdp.ldp[Matricule %in% matricules.RI.et.vacations &  Code %in% Code.prime[Code.prime$Type.rémunération == "VACATIONS","Code.rubrique"], c("Matricule", "Code", "Libellé", "Montant")]

RI.et.vacations <- Bdp.ldp[ Matricule %in% matricules.RI.et.vacations & Code %in% Code.prime[Code.prime$Type.rémunération == "INDEMNITAIRE.OU.CONTRACTUEL","Code.rubrique"], c("Matricule", "Statut", "Code", "Libellé", "Montant")]

# Vacations et indiciaire

traitement.et.vacations <- Bdp.ldp[ Matricule %in% matricules.RI.et.vacations & Code %in% Code.prime[Code.prime$Type.rémunération == "TRAITEMENT","Code.rubrique"], c("Matricule", "Statut", "Code", "Libellé", "Montant")]

nombre.de.ldp.RI.et.vacations <- nrow(RI.et.vacations)
nombre.de.ldp.traitement.et.vacations <- nrow(traitement.et.vacations)

# Incomptabilités de primes entre elles

#IAT et IFTS

filtre.iat<-grep(".*(IAT|I.A.T|.*Adm.*Tech).*", Libellé, ignore.case=TRUE)
filtre.ifts<-grep(".*(IFTS|I.F.T.S|.*FORF.*TRAV.*SUPP).*", Libellé, ignore.case=TRUE)
codes.ifts <- unique(Bdp.ldp[filtre.ifts, "Code"])

length(personnels.iat.ifts <- intersect(as.character(Bdp.ldp[ filtre.iat, c("Matricule")]), as.character(Bdp.ldp[ filtre.ifts, c("Matricule")])))

#IFTS et IB >= 380 (IM >= 350)

df1 <- Bdp.ldp[ Indice < 350, c("Matricule")]
df1 <- df1[!duplicated(df1)]

df2 <- Bdp.ldp[ filtre.ifts, c("Matricule")]
df2 <- df2[!duplicated(df2)]

df3 <- intersect(df1,df2)
Bdp.ldp[Matricule %in% df3 & Code %in% codes.ifts & (Indice < 380 ), c("Matricule", "Statut", "Code", "Libellé", "Indice", "Montant")]

rm(df1, df2, df3)
# IFTS et non tit

ifts.et.non.tit <- Bdp.ldp[Code %in% codes.ifts & !Statut %in% c("TITULAIRE", "STAGIAIRE"), c("Matricule", "Statut", "Code", "Libellé", "Indice", "Montant")]

# IHTS et catégorie < A
with(Base2,
ihts <<- Base2[! Code.catégorie %in% c("B", "C") & substr(Code,1,2) %in% c("19") & ! grepl(" ENS", Libellé), c("Matricule", "Code", "Libellé", "Montant", "Code.catégorie")]
)


pretty.print(nombre.de.ldp.NBI.nontit)
pretty.print(nombre.de.ldp.HS.sup.25)
pretty.print(primes.informatiques.potentielles)
pretty.print(nombre.de.ldp.personnels.pfi)
pretty.print(nombre.de.ldp.fonctionnaires.et.vacations)
pretty.print(nombre.de.ldp.RI.et.vacations)
pretty.print(nombre.de.ldp.traitement.et.vacations)

matricules.a.identifier <- unique(data.frame(Bdp.ldp$Nom, Bdp.ldp$Matricule))
Catégorie <- character(length=nrow(matricules.a.identifier))
matricules.a.identifier <- cbind(matricules.a.identifier, Catégorie)

detach(Bdp.ldp)


## ------------------------------------------------------------------------------------------------------------------
#  Sauvegardes : enlever les commentaires en mode opérationnel
##

 sauv.base(Analyse.rémunérations)
 sauv.base(Stats.contractuels)
 sauv.base(Stats.fonctionnaires)
 sauv.base(Bdp.ldp)
#  sauv.base(Base1)
#  sauv.base(Base2)
#  sauv.base(Base3)
 sauv.base(matricules.divergents)
 masses <-data.frame(masse.indemnitaire, masse.indiciaire, masse.rémunérations.brutes, ratio.global.masse.indemnitaire)
 sauv.base(masses)
 sauv.base(NBI.aux.non.titulaires)
 sauv.base(HS.sup.25)
 sauv.base(personnels.prime.informatique)

 sauv.base(traitement.et.vacations)
 sauv.base(RI.et.vacations)
 sauv.base(vacations.concernées)
 sauv.base(ifts.et.non.tit)
 write.csv2(Codes.NT, chemin("Codes.NT.csv"), row.names=FALSE)
 sauv.base(matricules.a.identifier)