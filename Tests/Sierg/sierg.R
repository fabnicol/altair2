#'---
#'title: false
#'author: false
#'date: false
#'output:
#'html_document:
#'css: style.css
#'---

#'<p class="centered"><img src="altair.png" /></p>
#'<p class="title">RH Sierg</h1>
#'

#+ echo=FALSE
library(compiler)
library(knitr)
knitr::opts_chunk$set(fig.retina=2, echo=FALSE, warning=FALSE, message=FALSE, results='asis')

exercice <- 2012

#'<p class="centered"><b>Exercice `r exercice` </b></p>  
#'<p class="author">Fabrice Nicol</h1>
#'
#+ echo=FALSE
#'`r format(Sys.Date(), "%a %d %b %Y")`  
#'

options(warn=-1, verbose=FALSE, OutDec=",")
compilerOptions <- setCompilerOptions(suppressAll=TRUE)
JITlevel <- enableJIT(3)


début.période.sous.revue <- 2011
fin.période.sous.revue   <- 2012
exercice <- 2012
nombre.exercices <- fin.période.sous.revue - début.période.sous.revue + 1

étudier.variations <- TRUE

étiquette.matricule <- "Matricule"
étiquette.montant <- "Montant"

seuil.troncature <- 99

# Le format est jour/mois/année avec deux chiffres-séparateur-deux chiffres-séparateur-4 chiffres.
#Le séparateur peut être changé en un autre en modifiant le "/" dans fate.format

date.format <- "%d/%m/%Y"

nom.de.fichier.de.paie  <- "Lignes de paye"
nom.de.bulletin.de.paie <- "Bulletins de paye"

# Cette section pourra être modifiée en entrée dans d'autres contextes
# Matricule, Codes, Avantages en nature 

chemin.dossier <- "~/Dev/Altair/altair/Tests/Sierg"
champ.détection.1 <- étiquette.matricule
champ.détection.2 <- "Code"
champ.détection.élus <- "Service"
champ.nir <- "Nir"
libellé.élus <- "Elus"

matricule.catégorie <- "LISTES DES PERSONNES REMUNEREES EN 2012 PAR CATEGORIE ET STATUT.csv"     
code.prime          <- "LISTE DES RUBRIQUES DE TRAITEMENT UTILISEES EN 2012.csv"
matricule.avantage  <- "LISTE DES AGENTS AYANT BENEFICIE D'AVANTAGE EN NATURE EN 2012.csv"
code.traitement <- 1010

#/* Lignes de paie
# On peut lire jusqu'à 10 fichiers csv qui seront générés au format
#  "chemin dossier + paies-Bulletins de paye-j.csv" */

ldp <- outer(début.période.sous.revue:fin.période.sous.revue, 1:10, paste, nom.de.fichier.de.paie, "csv", sep=".")

#/* Bulletins de paie */

bdp <- paste(début.période.sous.revue:fin.période.sous.revue, nom.de.bulletin.de.paie, "csv", sep=".")

codes.NBI <- c("1012", "101B", "101M", "4652", "4672")

source(file.path(chemin.dossier, "bibliothèque.fonctions.paie.R"), encoding="UTF-8-BOM")

#/* Programme principal
##
#  Bases
##
# Lignes de paie  */

ldp <- ldp[file.exists(chemin(ldp))]

Ldp<-Read.csv(ldp)

Bdp <- Read.csv(bdp)

#Matricule.catégorie <- read.csv.skip(matricule.catégorie)
Code.prime          <- read.csv.skip(code.prime)
#Matricule.avantage  <- read.csv.skip(matricule.avantage)

#suppression des colonnes Nom Prénom redondantes


Bdp.fin=Bdp[Bdp$Année == fin.période.sous.revue,]

#Matricule.avantage  <- selectionner.cle.matricule(Matricule.avantage, Matricule.catégorie) 
Bdp2                 <- selectionner.cle.matricule.mois(Bdp.fin, Ldp)

if (!setequal(intersect(names(Ldp), names(Bdp2)), c("Mois", étiquette.matricule)))
  stop("L'appariement ne peut se faire par les clés Matricule et Mois")

liste.matricules.fonctionnaires <- unique(Bdp.fin[Bdp.fin$Mois == 12 & Bdp.fin$Statut %in% c("TITULAIRE", "STAGIAIRE"), étiquette.matricule])
liste.matricules.élus   <- unique(Bdp.fin[Bdp.fin$Mois == 12 &  Bdp.fin[champ.détection.élus] == libellé.élus, étiquette.matricule])

Bdp.nir.total.hors.élus <- Bdp.fin[Bdp.fin$Mois == 12 & ! Bdp.fin$Matricule %in% liste.matricules.élus, champ.nir]

Bdp.nir.fonctionnaires  <- Bdp.fin[Bdp.fin$Mois == 12 &   Bdp.fin$Matricule %in% liste.matricules.fonctionnaires, champ.nir]

années.fonctionnaires   <- exercice-1-(as.numeric(substr(as.character(Bdp.nir.fonctionnaires), 2, 3)) + 1900)
années.total.hors.élus  <- exercice-1-(as.numeric(substr(as.character(Bdp.nir.total.hors.élus), 2, 3)) + 1900)

#'# 1. Statistiques de population
#'
#'### 1.1 Ensemble des personnels non élus    

hist(années.total.hors.élus,
     xlab="Âge",
     xlim=c(18, 75),
     ylab="Effectif",
     main="Pyramide des âges",
     col="blue",
     nclass=50)

#'

Résumé(années.total.hors.élus, "Âge des personnels", align='c')

#'Effectif total: `r length(années.total.hors.élus)`  
#'
#'### 1.2 Ensemble des fonctionnaires stagiaires et titulaires    

hist(années.fonctionnaires,
     xlab="Âge",
     xlim=c(18,68),
     ylab="Effectif",
     main="Pyramide des âges des fonctionnaires",
     col="navy",
     nclass=50)

#'

Résumé(années.fonctionnaires, "Âge des fonctionnaires", align='c')

#'   
#'Effectif total: `r length(années.fonctionnaires)`   
#'  
#'**Nota :** Dans les statistiques de cette section, les élus ne sont pas pris en compte.
#'Pyramides au 31 décembre `r exercice`  
#'
  
Fdp <- ddply(Bdp, 
             c(étiquette.matricule, "Année"),
             summarize,
             Montant=sum(Net.à.Payer),
             Statut.fin.exercice=Statut[length(Net.à.Payer)],
             entrée="1/1/2011",
             sortie="31/12/2012")

Fdp2 <- ddply(Fdp, étiquette.matricule, summarise, nb.exercices = length(Année))

Fdp <- merge(Fdp, Fdp2)

Analyse.variations <- ddply(Fdp,
                            .(Matricule),
                            summarise,
                            Nexercices=nb.exercices[1],
                            nb.jours = calcul.nb.jours(entrée[1], sortie[Nexercices]),
                            nb.jours.exercice.début = calcul.nb.jours.dans.exercice.in(entrée[1]),
                            nb.jours.exercice.sortie = calcul.nb.jours.dans.exercice.out(sortie[Nexercices]),
                            rémunération.début = ifelse(nb.jours.exercice.début == 0, 0, Montant[1]/nb.jours.exercice.début*365),
                            rémunération.sortie = ifelse(nb.jours.exercice.sortie == 0, 0, Montant[Nexercices]/nb.jours.exercice.sortie*365),
                            moyenne.rémunération.annuelle.sur.période = ifelse(nb.jours == 0, 0, sum(Montant)*365/nb.jours),
                            variation.rémunération.jour = calcul.variation(rémunération.début, rémunération.sortie, nb.jours.exercice.début, nb.jours.exercice.sortie, nb.exercices),
                            variation.moyenne.rémunération.jour = ifelse(nb.jours == 0, 0,
                              ( ( 1 + variation.rémunération.jour / 100 ) ^ (365 / nb.jours) - 1) * 100))

Analyse.variations <- mutate(Analyse.variations,
                             plus.de.2.ans = (nb.jours >= 2*365),
                             moins.de.2.ans = (nb.jours < 2*365),
                             moins.de.1.an  = (nb.jours < 365),
                             moins.de.six.mois = (nb.jours < 365/2))


Fdp <- mutate(Fdp,
              plus.de.2.ans = Matricule
              %in%
                Analyse.variations[Analyse.variations$plus.de.2.ans, étiquette.matricule])

attach(Fdp, warn=-1)

Fdp.plus.de.2.ans<-Fdp[plus.de.2.ans, ]


attach(Analyse.variations, warn.conflicts=FALSE)

#'
### 1.3 Effectifs des personnels par durée de service

Tableau(c("En place",
          "Moins de 2 ans",
          "Moins d'un an", 
          "Moins de six mois"), 
        sum(plus.de.2.ans),
        sum(moins.de.2.ans),
        sum(moins.de.1.an), 
        sum(moins.de.six.mois))

#'
#'**Evolutions entre `r début.période.sous.revue` et `r fin.période.sous.revue` **   
#'
# plot(levels(as.factor(Fdp.moins.de.2.ans$Année)),
#      sapply(levels(as.factor(Fdp.moins.de.2.ans$Année)) ,function(x) nlevels(as.factor(Fdp.moins.de.2.ans[Fdp.moins.de.2.ans$Année == x, ]$Matricule))),
#      ylab="Effectif",
#      las=1,
#      xlab="Année",
#      main="Nombre d'agents\n restés moins de deux ans",
#      col="blue",
#      type="b",  
#      xlim=c(début.période.sous.revue,fin.période.sous.revue)
# )

plot(levels(as.factor(Fdp.plus.de.2.ans$Année)),
     sapply(levels(as.factor(Fdp.plus.de.2.ans$Année)) ,function(x) nlevels(as.factor(Fdp.plus.de.2.ans[Fdp.plus.de.2.ans$Année == x, ]$Matricule))),
     ylab="Effectif",
     las=1,
     xlab="Année",
     main="Nombre d'agents\n restés plus de deux ans",
     col="blue",
     type="b",  
     xlim=c(début.période.sous.revue,fin.période.sous.revue))

plot(c(début.période.sous.revue:fin.période.sous.revue, début.période.sous.revue:fin.période.sous.revue),
     c(sapply(levels(as.factor(Année)), function(x) nlevels(as.factor(Fdp[Année == x, ]$Matricule))),
       sapply(levels(as.factor(Fdp.plus.de.2.ans$Année)) ,function(x) nlevels(as.factor(Fdp.plus.de.2.ans[Fdp.plus.de.2.ans$Année == x, ]$Matricule)))),
     ylab="Effectif",
     las=1,
     xlab="Année",
     main="Nombre d'agents",
     col=c("green", "blue"),
     type="p")


#'
#'**Nota:**  
#'Personnels en place : ayant servi au moins 730 jours pendant la période.  
#'Toutes les durées de service sont calculées en multiples de 365 jours.
#'

detach(Analyse.variations)

#fusion matricule | avantage | catégorie par Matricule

Bdp.ldp <- merge(Bdp2, Ldp[Ldp$Année == fin.période.sous.revue,])

library(plyr)

Bdp.ldp2 <- mutate(Bdp.ldp,
       
        montant.traitement.indiciaire = Montant*(Code %in% Code.prime[Code.prime$Type.rémunération == "TRAITEMENT","Code.rubrique"]),
        montant.primes = Montant*(Code %in% Code.prime[Code.prime$Type.rémunération == "INDEMNITAIRE.OU.CONTRACTUEL","Code.rubrique"]),
        montant.autres.rémunérations = Montant*(Code %in% Code.prime[Code.prime$Type.rémunération == "AUTRES","Code.rubrique"]))

Analyse.rémunérations <- ddply(Bdp.ldp2,
                             c(étiquette.matricule, "Statut", "Service", "Année"),
                             summarize,
                             traitement.indiciaire = sum(montant.traitement.indiciaire),
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

#'# 2. Rémunérations : analyse pour l'exercice `r fin.période.sous.revue` 
#'## 2.1 Fonctionnaires titulaires et stagiaires
#+ fig.width=7.1

filtre.fonctionnaire <- function (X) X[ Statut %in% c("TITULAIRE", "STAGIAIRE") & X >0 ]

hist(filtre.fonctionnaire(total.rémunérations)/1000,
   xlab="Distribution de la rémunération en k€ \n hors politique familiale, indemnités journalières et remboursements",
   ylab="Effectif",
   xlim=c(0, 120),
   main="Rémunération annuelle des fonctionnaires",
   col="blue",
   nclass=50
)

#'  
#+ fig.width=7.1

hist(filtre.fonctionnaire(rémunération.contractuelle.ou.indemnitaire)/1000,
     xlab="Distribution des attributions indemnitaires en k€\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 70),
     main="Rémunération indemnitaire annuelle\ndes fonctionnaires",
     col="blue",
     nclass=50
)

#'    
#+ fig.width=7.1

hist(filtre.fonctionnaire(part.rémunération.contractuelle.ou.indemnitaire),
     xlab="Pourcentage des indemnités dans la rémunération\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     main="Distribution de la part indemnitaire\n de la rémunération annuelle des fonctionnaires",
     xlim=c(0,60),     
     col="blue",
     nclass=30)

#/* La moyenne est tirée vers le haut par les outlyers */

masse.indemnitaire            <- sum(rémunération.contractuelle.ou.indemnitaire[ traitement.indiciaire > 0])
masse.indiciaire              <- sum(traitement.indiciaire)
masse.rémunérations.brutes    <- sum(total.rémunérations)
ratio.global.masse.indemnitaire  <- masse.indemnitaire/(masse.indiciaire+masse.indemnitaire)*100

#'### Masses indemnitaires
#'Les résultats sont exprimés en euros.

 Tableau(c("Masse indiciaire", "Masse indemnitaire"),
                  masse.indiciaire, masse.indemnitaire)
  
#'  

 Tableau(c("Masse des rémunérations brutes", "Part de la masse indemnitaire"),
                   masse.rémunérations.brutes, ratio.global.masse.indemnitaire)

#'### Statistiques de position

#/*print(xtable(Stats.fonctionnaires), type="html", include.rownames=FALSE)*/

 Résumé(Analyse.rémunérations[Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                              c("traitement.indiciaire",
                                "rémunération.contractuelle.ou.indemnitaire",
                                "autres.rémunérations") ],
                               c("Traitement indiciaire",
                                 "Rémunération contractuelle ou indemnitaire",
                                 "Autres rémunérations"))

#'

 Résumé(Analyse.rémunérations[Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                              c( "total.rémunérations",
                                 "part.rémunération.contractuelle.ou.indemnitaire") ],
                              c("Total rémunérations",
                                "Part de la rémunération contractuelle ou indemnitaire"))

#'## 2.2 Contractuels, vacataires et stagiaires inclus
#+ fig.width=7.1

hist(total.rémunérations[!Statut %in% c("TITULAIRE", "STAGIAIRE") & total.rémunérations > 1000]/1000,
   xlab="Distribution de la rémunération en k€ (supérieure à 1000 €)\n hors politique familiale, indemnités journalières et remboursements",
   ylab="Effectif",
   xlim=c(0, 40),
   main="Rémunération annuelle totale des contractuels",
   col="red",
   nclass=50)

#'
#+ fig.width=7.1

hist(positive(autres.rémunérations),
     xlab="Distribution des autres rémunérations annuelles en € :\n politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 5000),
     main="Autres rémunérations",
     col="grey")

#'### Statistiques de position

  Résumé(Analyse.rémunérations[! Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                                                    c("rémunération.contractuelle.ou.indemnitaire", "autres.rémunérations")],
                               c("Rémunération contractuelle ou indemnitaire",
                                  "Autres rémunérations"))

#'

  Résumé(Analyse.rémunérations[!Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                                                   "total.rémunérations"],
                                                   "Total rémunérations")
detach(Analyse.rémunérations)

#'
#'# 3. Analyse des rémunérations sur l'ensemble de la période `r début.période.sous.revue` - `r fin.période.sous.revue` 
#'
#'Nombre d'exercices: `r nombre.exercices`  
#'    
#Ici il faudra affiner l'input des paramètres pour l'interface en utilisatant get(get) par exemple.
#ou alors un processeur

#/* faire renseigner par la structure pour les colonnes début et fin.
# A priori on pourrait aussi prendre : Min(Mois) comme début et Max(Mois) comme fin en appliquant la quotité. C'est imprécis. 
# Il y a donc tros inputs à solliciter : les débuts / fin d'activité, les catégories statutaires, les avantages en nature
# Ces trois champs sont des inputs natifs; ils se font au niveau 
# d'un fichier du type
#  personne : 
#     début/fin :  début (date fin)
#     catégorie : X (date nouvelle)
#     NAS :  avantage (date suppression)
# par défaut la personne débute au commencement de la période sous revue et a pour catégorie AUTRE et NAS non
# plus la validation de la codification des indemnités (confirmation) */

#' 
#'## 3.1 Rémunération moyenne sur la période

attach(Analyse.variations)

hist(moyenne.rémunération.annuelle.sur.période[moyenne.rémunération.annuelle.sur.période > 0]/1000,
     xlab=paste0("Distribution de la rémunération nette moyenne sur la période ",début.période.sous.revue,"-",fin.période.sous.revue," en k€"),
     ylab="Effectif",
     main="Rémunération nette moyenne",
     col="blue",
     nclass=100)

#'

Analyse.variations.filtrée <- Analyse.variations[ nb.jours.exercice.début > seuil.troncature & nb.jours.exercice.sortie > seuil.troncature & nb.exercices > 1, ]

Analyse.variations.filtrée2 <- na.omit(Analyse.variations.filtrée[10:14])

#' 
#'## 3.2 Evolutions des rémunérations sur la période `r début.période.sous.revue` - `r fin.période.sous.revue` 
#'
#'### 3.2.1 Ensemble des personnels
#'

Résumé(
    Analyse.variations.filtrée2,
          c("Première année",
            "Dernière année",
            "Moyenne sur la période",
            "Variation sur la période",
            "Variation annuelle moyenne"))

#'

Analyse.variations.personnels.plus.de.2.ans <- na.omit(Analyse.variations.filtrée[Analyse.variations.filtrée$plus.de.2.ans, 10:14])

#'
#'### 3.2.2 Personnels en place
#'



hist(Analyse.variations.personnels.plus.de.2.ans$variation.moyenne.rémunération.jour,
     xlab ="Variation annuelle moyenne de la rémunération en %",
     las=1,
     xlim=c(-5,30),
     sub  = "pour les personnels en place",
     ylab ="Effectifs",
     main ="Distribution de la variation annuelle\nmoyenne de la rémunération des personnels en place",
     col ="red",
     nclass=200)

#'

#'
plot(levels(as.factor(Fdp.plus.de.2.ans$Année)),
     tapply(Fdp.plus.de.2.ans$Montant, Fdp.plus.de.2.ans$Année, sum)/1000,
     ylab="k€",
     las=1,
     xlab="Année",
     main="Rémunération annuelle nette des agents\nrestés plus de deux ans",
     col="red",
     type="b",  
     xlim=c(début.période.sous.revue,fin.période.sous.revue)
)
#'

Résumé(Analyse.variations.personnels.plus.de.2.ans, 
       c("Première année",
         "Dernière année",
         "Moyenne sur la période",
         "Variation sur la période",
         "Variation annuelle moyenne"))
#'
#'### 3.2.2 Personnels en fonction moins de deux ans
#'


Analyse.variations.personnels.moins.de.2.ans <- na.omit(Analyse.variations.filtrée[Analyse.variations.filtrée$moins.de.2.ans, 10:14])

#'
hist(Analyse.variations.personnels.moins.de.2.ans$variation.moyenne.rémunération.jour,
     xlab ="Variation annuelle moyenne de la rémunération en %",
     xlim=c(-10,30),
     las=1,
     sub  = "pour 30 agents restés moins de deux ans",
     ylab ="Effectifs",
     main ="Distribution de la variation annuelle\nmoyenne de la rémunération des agents restés moins de deux ans",
     col ="red",
     nclass=100
)
#'

Fdp.moins.de.2.ans<-Fdp[!plus.de.2.ans, ]

plot(levels(as.factor(Fdp.moins.de.2.ans$Année)),
     tapply(Fdp.moins.de.2.ans$Montant, Fdp.moins.de.2.ans$Année, sum)/1000,
     ylab="k€",
     las=1,
     xlab="Année",
     main="Rémunération annuelle nette des agents\n restés moins de deux ans",
     col="blue",
     type="b",  
     xlim=c(début.période.sous.revue,fin.période.sous.revue)
)

#'
#'

Résumé(Analyse.variations.personnels.moins.de.2.ans, 
       c("Première année",
         "Dernière année",
         "Moyenne sur la période",
         "Variation sur la période",
         "Variation annuelle moyenne"))

#'
detach(Analyse.variations)

#'Les résultats sont exprimés en euros.
#'
#'# 4. Tests réglementaires
#'## 4.1 Contrôle des heures supplémentaires, des NBI et primes informatiques
#'

attach(Bdp.ldp, warn.conflicts=FALSE)

NBI.aux.non.titulaires <- Bdp.ldp[ ! Statut %in% c("TITULAIRE","STAGIAIRE") & as.character(Code) %in% codes.NBI, c(étiquette.matricule, "Statut", "Code", "Libellé", "Mois", étiquette.montant)]

nombre.de.ldp.NBI.nontit <- nrow(NBI.aux.non.titulaires)

#/* Prime de fonctions informatiques : pas dans la base de VLB
# on cherche la chaine de char. "INFO" dans les libellés de primes

# variante : filtre <- regexpr(".*(INFO|PFI|P.F.I).*", toupper(Bdp.ldp$Libellé)) et regmatches(Bdp.ldp$Libellé, filtre)*/

filtre<-grep(".*(INFO|PFI|P.F.I).*", Libellé)

personnels.prime.informatique <- Bdp.ldp[ filtre, c(étiquette.matricule, "Statut", "Code", "Libellé", étiquette.montant)]

primes.informatiques.potentielles<-unique(Libellé[filtre])

if  (length(primes.informatiques.potentielles) == 0) 
  primes.informatiques.potentielles <- "aucune"
  
nombre.de.personnels.pfi <- nrow(personnels.prime.informatique)

#'Primes informatiques potentielles : `r primes.informatiques.potentielles`  

Tableau(
  c("Nombre de lignes NBI pour non titulaires",
    "Nombre de bénéficiaires de PFI"),
  nombre.de.ldp.NBI.nontit, 
  nombre.de.personnels.pfi)

#'**Nota :**  
#'NBI: nouvelle bonification indiciaire  
#'PFI: prime de fonctions informatiques  
#'    
#'## 4.2 Contrôle des vacations pour les fonctionnaires  

# Vacations et statut de fonctionnaire

lignes.fonctionnaires.et.vacations <- Bdp.ldp[ Statut %in% c("TITULAIRE", "STAGIAIRE") & Code %in% Code.prime[Code.prime$Type.rémunération == "VACATIONS","Code.rubrique"], c(étiquette.matricule, "Statut", "Code", "Libellé", étiquette.montant)]
matricules.fonctionnaires.et.vacations <- unique(lignes.fonctionnaires.et.vacations$Matricule)
nombre.fonctionnaires.et.vacations <- length(matricules.fonctionnaires.et.vacations)
nombre.ldp.fonctionnaires.et.vacations <- nrow(lignes.fonctionnaires.et.vacations)

#'

Tableau(
  c("Nombre de FEV",
    "Nombre de lignes de vacations pour FEV"),
  nombre.fonctionnaires.et.vacations,
  nombre.ldp.fonctionnaires.et.vacations)


#'    
#'**Nota:**  
#'FEV : fonctionnaire effectuant des vacations
#'
#'## 4.3 Contrôles sur les cumuls traitement indiciaire, indemnités et vacations des contractuels

# Vacations et régime indemnitaire

lignes.contractuels.et.vacations <- Bdp.ldp[ ! Statut %in% c("TITULAIRE", "STAGIAIRE")  & Code %in% Code.prime[Code.prime$Type.rémunération == "VACATIONS","Code.rubrique"], c(étiquette.matricule, "Code", "Libellé", étiquette.montant)]
matricules.contractuels.et.vacations <- unique(lignes.contractuels.et.vacations$Matricule)
nombre.contractuels.et.vacations <- length(matricules.contractuels.et.vacations)

RI.et.vacations <- Bdp.ldp[ Matricule %in% matricules.contractuels.et.vacations & Code %in% Code.prime[Code.prime$Type.rémunération == "INDEMNITAIRE.OU.CONTRACTUEL","Code.rubrique"], c(étiquette.matricule, "Statut", "Code", "Libellé", étiquette.montant)]

# Vacations et indiciaire

traitement.et.vacations <- Bdp.ldp[ Matricule %in% matricules.contractuels.et.vacations & Code %in% Code.prime[Code.prime$Type.rémunération == "TRAITEMENT","Code.rubrique"], c(étiquette.matricule, "Statut", "Code", "Libellé", étiquette.montant)]

nombre.ldp.contractuels.et.vacations <- nrow(lignes.contractuels.et.vacations)
nombre.ldp.RI.et.vacations <- nrow(RI.et.vacations)
nombre.ldp.traitement.et.vacations <- nrow(traitement.et.vacations)

#'

Tableau(c("Nombre de CEV",
          "Nombre de lignes CEV",
          "Nombre de lignes indemnitaires pour CEV",
          "Nombre de lignes de traitement pour CEV"),
          nombre.contractuels.et.vacations,
          nombre.ldp.contractuels.et.vacations,
          nombre.ldp.RI.et.vacations,
          nombre.ldp.traitement.et.vacations)

#'    
#'**Nota:**  
#'CEV : contractuel effectuant des vacations

#'
#'## 4.4 Contrôle sur les indemnités IAT et IFTS

#IAT et IFTS

filtre.iat<-grep(".*(IAT|I.A.T|.*Adm.*Tech).*", Libellé, ignore.case=TRUE)
filtre.ifts<-grep(".*(IFTS|I.F.T.S|.*FORF.*TRAV.*SUPP).*", Libellé, ignore.case=TRUE)
codes.ifts <- unique(Bdp.ldp[filtre.ifts, "Code"])

nombre.personnels.iat.ifts <- length(personnels.iat.ifts <- intersect(as.character(Bdp.ldp[ filtre.iat, c(étiquette.matricule)]), as.character(Bdp.ldp[ filtre.ifts, c(étiquette.matricule)])))

#'

Tableau(c("Codes IFTS", "Nombre de personnels percevant IAT et IFTS"), codes.ifts, nombre.personnels.iat.ifts)

#'
#'### Contrôle sur les IFTS pour catégories B et contractuels

#IFTS et IB >= 380 (IM >= 350)

df1 <- Bdp.ldp[ Indice < 350, c(étiquette.matricule)]
df1 <- df1[!duplicated(df1)]

df2 <- Bdp.ldp[ filtre.ifts, c(étiquette.matricule)]
df2 <- df2[!duplicated(df2)]

df3 <- intersect(df1,df2)

lignes.ifts.anormales <- Bdp.ldp[Matricule %in% df3 & Code %in% codes.ifts & (Indice < 380 ), c(étiquette.matricule, "Statut", "Code", "Libellé", "Indice", étiquette.montant)]
nombre.lignes.ifts.anormales <- length(lignes.ifts.anormales)

rm(df1, df2, df3)
# IFTS et non tit

ifts.et.contractuel <- Bdp.ldp[Code %in% codes.ifts & ! Statut %in% c("TITULAIRE", "STAGIAIRE"), c(étiquette.matricule, "Statut", "Code", "Libellé", "Indice", étiquette.montant)]
nombres.lignes.ifts.et.contractuel <- length(ifts.et.contractuel)

#'

Tableau(c("Nombre de contractuels percevant des IFTS", "Nombre de lignes IFTS pour IB < 380"), nombres.lignes.ifts.et.contractuel, nombre.lignes.ifts.anormales)

#'    
#'**Nota:**  
#'IB < 380 : fonctionnaire percevant un indice brut inférieur à 380  
#'
#'###### page break
#'
#'## 4.5 Contrôle sur les heures supplémentaires

HS.sup.25 <- Bdp.ldp[Heures.Sup. >= 25 , c(étiquette.matricule, "Statut", "Mois", "Heures.Sup.", "Brut")]
nombre.ldp.HS.sup.25 <- nrow(HS.sup.25)

# with(Base2,
#      ihts <<- Base2[! Code.catégorie %in% c("B", "C") & substr(Code,1,2) %in% c("19") & ! grepl(" ENS", Libellé), c(étiquette.matricule, "Code", "Libellé", étiquette.montant, "Code.catégorie")]
# )

ihts <- character(0)
nombre.ihts.anormales <- length(ihts)
#'
 
Tableau(c("Nombre de lignes HS en excès", "Nombre de lignes IHTS anormales"), nombre.ldp.HS.sup.25, nombre.ihts.anormales)

#'
#'    
#'**Nota:**  
#'HS en excès : au-delà de 25 heures par mois  
#'IHTS anormales : non attribuées à des fonctionnaires de catégorie B ou C.  
#'
#'****
#'
#'# Annexe

matricules.à.identifier <- unique(data.frame(Bdp.ldp$Nom, Bdp.ldp$Prénom, Bdp.ldp$Matricule))
Catégorie <- character(length=nrow(matricules.à.identifier))
matricules.à.identifier <- cbind(matricules.à.identifier, Catégorie)
names(matricules.à.identifier) <- c("Nom", "Prénom", étiquette.matricule, "Catégorie")

kable(matricules.à.identifier, row.names=FALSE)

#'

detach(Bdp.ldp)

#/* ------------------------------------------------------------------------------------------------------------------
#  Sauvegardes : enlever les commentaires en mode opérationnel
##

# sauv.base(Analyse.rémunérations, "analyse.rémunérations.csv")
# sauv.base(Stats.contractuels, "stats.contractuels.csv")
# sauv.base(Stats.fonctionnaires, "stats.fonctionnaires.csv")
# sauv.base(Bdp.ldp, "bdp.ldp.csv")
# sauv.base(Base1, "base1.csv")
# sauv.base(Base2, "base2.csv")
# sauv.base(Base3, "base3.csv")
# sauv.base(matricules.divergents, "matricules.divergents.csv")
# sauv.base(data.frame(masse.indemnitaire, masse.indiciaire, masse.rémunérations.brutes, ratio.global.masse.indemnitaire),"stats.masse.csv" )
# sauv.base(NBI.aux.non.titulaires)
# sauv.base(HS.sup.25, "HS.sup.25.csv")
# sauv.base(personnels.prime.informatique, "personnels.prime.informatique.csv") 
#sauv.base(liste.matricules.fonctionnaires)
#sauv.base(liste.matricules.)*/

