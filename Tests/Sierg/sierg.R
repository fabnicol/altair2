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
library(plyr)
library(ggplot2)

options(warn=-1, verbose=FALSE, OutDec=",")
compilerOptions <- setCompilerOptions(suppressAll=TRUE)
JITlevel <- enableJIT(3)

knitr::opts_chunk$set(fig.retina=2, echo=FALSE, warning=FALSE, message=FALSE, results='asis')

début.période.sous.revue <- 2011
fin.période.sous.revue   <- 2012
sauvegarder.bases <- TRUE

#'<p class="centered"><b>Exercices `r paste(début.période.sous.revue, "à", fin.période.sous.revue)` </b></p>  
#'<p class="author">Fabrice Nicol</h1>
#'
#+ echo=FALSE
#'`r format(Sys.Date(), "%a %d %b %Y")`  
#'

nombre.exercices <- fin.période.sous.revue - début.période.sous.revue + 1

étudier.variations <- TRUE

étiquette.matricule <- "Matricule"
étiquette.montant <- "Montant"

seuil.troncature <- 99

# Le format est jour/mois/année avec deux chiffres-séparateur-deux chiffres-séparateur-4 chiffres.
#Le séparateur peut être changé en un autre en modifiant le "/" dans fate.format

date.format <- "%d/%m/%Y"

nom.fichier.paie  <- "Lignes de paye"
nom.bulletin.paie <- "Bulletins de paye"

# Cette section pourra être modifiée en entrée dans d'autres contextes
# Matricule, Codes, Avantages en nature 

chemin.dossier <- "C:/Dev/altair/Tests/Sierg"
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

Lignes.paie <- outer(début.période.sous.revue:fin.période.sous.revue, 1:10, paste, nom.fichier.paie, "csv", sep=".")

#/* Bulletins de paie */

Bulletins.paie <- paste(début.période.sous.revue:fin.période.sous.revue, nom.bulletin.paie, "csv", sep=".")

codes.NBI <- c("1012", "101B", "101M", "4652", "4672")

source(file.path(chemin.dossier, "bibliotheque.fonctions.paie.R"), encoding="UTF-8-BOM")

#/* Programme principal
##
#  Bases
##
# Lignes de paie  */

Lignes.paie <- Lignes.paie[file.exists(chemin(Lignes.paie))]

Lignes.paie<-Read.csv(Lignes.paie)

Bulletins.paie <- Read.csv(Bulletins.paie)

# Matricule.catégorie <- read.csv.skip(matricule.catégorie)
Code.prime          <- read.csv.skip(code.prime)
# Matricule.avantage  <- read.csv.skip(matricule.avantage)

# suppression des colonnes Nom Prénom redondantes

Bulletins.paie.dernier.exercice <-  selectionner.cle.matricule.mois(Bulletins.paie[Bulletins.paie$Année == fin.période.sous.revue,], Lignes.paie)

# Matricule.avantage  <- selectionner.cle.matricule(Matricule.avantage, Matricule.catégorie) 

if (!setequal(intersect(names(Lignes.paie), names(Bulletins.paie.dernier.exercice)), c("Mois", étiquette.matricule)))
  stop("L'appariement ne peut se faire par les clés Matricule et Mois")

liste.matricules.fonctionnaires <- unique(Bulletins.paie.dernier.exercice[Bulletins.paie.dernier.exercice$Mois == 12 & Bulletins.paie.dernier.exercice$Statut %in% c("TITULAIRE", "STAGIAIRE"), étiquette.matricule])

liste.matricules.élus   <- unique(Bulletins.paie.dernier.exercice[Bulletins.paie.dernier.exercice$Mois == 12 &  Bulletins.paie.dernier.exercice[champ.détection.élus] == libellé.élus, étiquette.matricule])

Bulletins.paie.nir.total.hors.élus <- Bulletins.paie.dernier.exercice[Bulletins.paie.dernier.exercice$Mois == 12 & ! Bulletins.paie.dernier.exercice$Matricule %in% liste.matricules.élus, champ.nir]

Bulletins.paie.nir.fonctionnaires  <- Bulletins.paie.dernier.exercice[Bulletins.paie.dernier.exercice$Mois == 12 &   Bulletins.paie.dernier.exercice$Matricule %in% liste.matricules.fonctionnaires, champ.nir]

# Age au 31 décembre de l'exercice dernier.exerciceal de la période sous revue

années.fonctionnaires   <-fin.période.sous.revue-(as.numeric(substr(as.character(Bulletins.paie.nir.fonctionnaires), 2, 3)) + 1900)
années.total.hors.élus  <-fin.période.sous.revue-(as.numeric(substr(as.character(Bulletins.paie.nir.total.hors.élus), 2, 3)) + 1900)

#'# 1. Statistiques de population
#'
#'### 1.1 Ensemble des personnels non élus    

hist(années.total.hors.élus,
     xlab=paste("Âge au 31 décembre",fin.période.sous.revue),
     xlim=c(18, 75),
     ylab="Effectif",
     main="Pyramide des âges",
     col="blue",
     nclass=50)

#'

Résumé(paste0("Âge des personnels<br>au 31/12/",fin.période.sous.revue), années.total.hors.élus, align='c')

#'Effectif total: `r length(années.total.hors.élus)`  
#'
#'### 1.2 Ensemble des fonctionnaires stagiaires et titulaires    

hist(années.fonctionnaires,
     xlab=paste("Âge au 31 décembre",fin.période.sous.revue),
     xlim=c(18,68),
     ylab="Effectif",
     main="Pyramide des âges des fonctionnaires",
     col="navy",
     nclass=50)

#'

Résumé(paste0("Âge des personnels<br>au 31/12/",fin.période.sous.revue), années.fonctionnaires, align='c')

#'   
#'**Effectif total: `r length(années.fonctionnaires)`**     
#'
  
Analyse.variations.par.exercice <- ddply(Bulletins.paie, 
             c(étiquette.matricule, "Année"),
             summarise,
             Montant.net=sum(Net.à.Payer),
             Statut.dernier.exercice.exercice=Statut[length(Net.à.Payer)],
             mois.entrée=ifelse((minimum <- min(Mois)) != Inf, minimum, 0),
             mois.sortie=ifelse((maximum <- max(Mois)) != -Inf, maximum, 0),
             nb.jours = calcul.nb.jours.mois(mois.entrée[1], mois.sortie[1], Année[1]))

# Analyse.variations.par.exercice2 <- ddply(Analyse.variations.par.exercice, 
#               étiquette.matricule,
#               summarise,
#               nb.exercices = length(Année))
# 
# Analyse.variations.par.exercice <- merge(Analyse.variations.par.exercice, Analyse.variations.par.exercice2)

Analyse.variations.synthèse <- ddply(Analyse.variations.par.exercice,
                            .(Matricule),
                            summarise,
                            Nexercices=length(Année),
                            nb.jours.exercice.début = nb.jours[1],
                            nb.jours.exercice.sortie = nb.jours[Nexercices],
                            total.jours = sum(nb.jours),
                            rémunération.début = ifelse(nb.jours.exercice.début == 0,
                                                        0,
                                                        Montant.net[1]/nb.jours.exercice.début*365),
                            rémunération.sortie = ifelse(nb.jours.exercice.sortie == 0,
                                                         0,
                                                         Montant.net[Nexercices]/nb.jours.exercice.sortie*365),
                            moyenne.rémunération.annuelle.sur.période = ifelse(total.jours == 0, 0, sum(Montant.net)*365/total.jours),
                            variation.rémunération.jour = calcul.variation(rémunération.début, 
                                                                           rémunération.sortie,
                                                                           nb.jours.exercice.début,
                                                                           nb.jours.exercice.sortie,
                                                                           Nexercices),
                            variation.moyenne.rémunération.jour = ifelse(total.jours == 0, 0,
                              ( ( 1 + variation.rémunération.jour / 100 ) ^ (365 / total.jours) - 1) * 100),
                            plus.2.ans = (total.jours >= 2*365),
                            moins.2.ans = (total.jours < 2*365),
                            moins.1.an  = (total.jours < 365),
                            moins.six.mois = (total.jours < 365/2))

#  On pourrait aussi plus simplement poser 
#  que plus.deux.ans soit défini comme length(Année) >= 2.
#  On préfère une définition à partir de total.jours pour 
#  avoir une définition cohérente sur toutes les durées, y.c infra-annuelles

attach(Analyse.variations.par.exercice, warn=FALSE)

Analyse.variations.par.exercice <- na.omit(Analyse.variations.par.exercice[ nb.jours[1] > seuil.troncature
                                                                 &  nb.jours[length(Année)] > seuil.troncature, ])

Analyse.variations.par.exercice <- mutate(Analyse.variations.par.exercice,
                                          plus.2.ans = Matricule
                                                          %in%
                                                       Analyse.variations.synthèse[Analyse.variations.synthèse$plus.2.ans, étiquette.matricule])


detach(Analyse.variations.par.exercice)

attach(Analyse.variations.synthèse, warn.conflicts=FALSE)

#'
#'### 1.3 Effectifs des personnels par durée de service

Tableau(c("En place",
          "Moins de 2 ans",
          "Moins d'un an", 
          "Moins de six mois"), 
        sum(plus.2.ans),
        sum(moins.2.ans),
        sum(moins.1.an), 
        sum(moins.six.mois))

detach(Analyse.variations.synthèse)

#'
#'**Evolutions entre `r début.période.sous.revue` et `r fin.période.sous.revue` **   
#'

qplot(factor(Année), data=Analyse.variations.par.exercice, geom="bar", fill=factor(!plus.2.ans), xlab="Année", ylab="Effectif" ) + 
  scale_fill_discrete(name="Composition des effectifs",
                      breaks=c(TRUE, FALSE),
                      labels=c("Moins de deux ans", "Plus de deux ans"))
  
#'
#'**Nota:**  
#'Personnels en place : ayant servi au moins 730 jours pendant la période.  
#'Toutes les durées de service sont calculées en multiples de 365 jours.  
#'Dans les statistiques de cette section, les élus ne sont pas pris en compte.
#'

#fusion matricule | avantage | catégorie par Matricule

# peut aisément être modifié ainsi

année <- début.période.sous.revue

########### pour année début #################
#'
#'

Bulletins.paie.Lignes.paie <- merge(Bulletins.paie.dernier.exercice, Lignes.paie[Lignes.paie$Année == année,])

Bulletins.paie.Lignes.paie <- mutate(Bulletins.paie.Lignes.paie,
                                     montant.traitement.indiciaire 
                                     = Montant*(Code %in% Code.prime[Code.prime$Type.rémunération 
                                                                     == "TRAITEMENT","Code.rubrique"]),
                                     montant.primes 
                                     = Montant*(Code %in% Code.prime[Code.prime$Type.rémunération 
                                                                     == "INDEMNITAIRE.OU.CONTRACTUEL","Code.rubrique"]),
                                     montant.autres.rémunérations 
                                     = Montant*(Code %in% Code.prime[Code.prime$Type.rémunération
                                                                     == "AUTRES","Code.rubrique"]))

# Par convention, on retient pour statut et pour service 
# celui en fin de période sous revue (et en fin de période d'activité)

Analyse.rémunérations <- ddply(Bulletins.paie.Lignes.paie,
                               .(Matricule),
                               summarise,
                               Statut=Statut[length(Statut)],
                               Service=Service[length(Service)],
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

#'# 2. Rémunérations : analyse pour l'exercice `r année` 
#'## 2.1 Fonctionnaires titulaires et stagiaires
#+ fig.width=7.1



filtre.fonctionnaire <- function (X) X[ Statut %in% c("TITULAIRE", "STAGIAIRE") & X >0 ]

hist(filtre.fonctionnaire(total.rémunérations)/1000,
     xlab="Distribution de la rémunération en k€ \n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 120),
     main=paste("Rémunération annuelle des fonctionnaires\nExercice", année),
     col="blue",
     nclass=50)

#'  
#+ fig.width=7.1

hist(filtre.fonctionnaire(rémunération.contractuelle.ou.indemnitaire)/1000,
     xlab="Distribution des attributions indemnitaires en k€\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 70),
     main=paste("Rémunération indemnitaire annuelle\ndes fonctionnaires\nExercice", année),
     col="blue",
     nclass=50
)

#'    
#+ fig.width=7.1

hist(filtre.fonctionnaire(part.rémunération.contractuelle.ou.indemnitaire),
     xlab="Pourcentage des indemnités dans la rémunération\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     main=paste("Distribution de la part indemnitaire\n de la rémunération annuelle des fonctionnaires\nExercice", année),
     xlim=c(0,60),     
     col="blue",
     nclass=30)

#/* La moyenne est tirée vers le haut par les outlyers */

masse.indemnitaire            <- sum(rémunération.contractuelle.ou.indemnitaire[ traitement.indiciaire > 0])
masse.indiciaire              <- sum(traitement.indiciaire)
masse.rémunérations.brutes    <- sum(total.rémunérations)
ratio.global.masse.indemnitaire  <- masse.indemnitaire/(masse.indiciaire+masse.indemnitaire)*100

#'### Masses indemnitaires pour l'exercice `r année`
#'
Tableau(c("Masse indiciaire", "Masse indemnitaire"),
        masse.indiciaire, masse.indemnitaire)

#'  

Tableau(c("Masse des rémunérations brutes", "Part de la masse indemnitaire"),
        masse.rémunérations.brutes, ratio.global.masse.indemnitaire)
#'
#'Les résultats sont exprimés en euros.  
#'
#'### Statistiques de position pour l'exercice `r année`  
#'

Résumé(c("Traitement indiciaire",
         "Rémunération contractuelle ou indemnitaire",
         "Autres rémunérations"),
       Analyse.rémunérations[Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                             c("traitement.indiciaire",
                               "rémunération.contractuelle.ou.indemnitaire",
                               "autres.rémunérations") ])
#'

Résumé(c("Total rémunérations", "Part de la rémunération contractuelle ou indemnitaire"),
       Analyse.rémunérations[Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                             c( "total.rémunérations", "part.rémunération.contractuelle.ou.indemnitaire") ])

#'## 2.2 Contractuels, vacataires et stagiaires inclus
#+ fig.width=7.1

hist(total.rémunérations[!Statut %in% c("TITULAIRE", "STAGIAIRE") & total.rémunérations > 1000]/1000,
     xlab="Distribution de la rémunération en k€ (supérieure à 1000 €)\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 40),
     main=paste("Rémunération annuelle totale des contractuels\nExercice", année),
     col="red",
     nclass=50)

#'
#+ fig.width=7.1

hist(positive(autres.rémunérations),
     xlab="Distribution des autres rémunérations annuelles en € :\n politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 5000),
     main=paste("Autres rémunérations\nExercice", année),
     col="grey")

#'### Statistiques de position pour l'exercice `r année`  
#'

Résumé(c("Rémunération contractuelle ou indemnitaire", "Autres rémunérations"),
       Analyse.rémunérations[! Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                             c("rémunération.contractuelle.ou.indemnitaire", "autres.rémunérations")])

#'

Résumé("Total rémunérations",
       Analyse.rémunérations[!Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                             "total.rémunérations"])
#'

detach(Analyse.rémunérations)

########### pour année fin ########################

année <- fin.période.sous.revue

#'
#'

Bulletins.paie.Lignes.paie <- merge(Bulletins.paie.dernier.exercice, Lignes.paie[Lignes.paie$Année == année,])

Bulletins.paie.Lignes.paie <- mutate(Bulletins.paie.Lignes.paie,
                                     montant.traitement.indiciaire 
                                     = Montant*(Code %in% Code.prime[Code.prime$Type.rémunération 
                                                                     == "TRAITEMENT","Code.rubrique"]),
                                     montant.primes 
                                     = Montant*(Code %in% Code.prime[Code.prime$Type.rémunération 
                                                                     == "INDEMNITAIRE.OU.CONTRACTUEL","Code.rubrique"]),
                                     montant.autres.rémunérations 
                                     = Montant*(Code %in% Code.prime[Code.prime$Type.rémunération
                                                                     == "AUTRES","Code.rubrique"]))

# Par convention, on retient pour statut et pour service 
# celui en fin de période sous revue (et en fin de période d'activité)

Analyse.rémunérations <- ddply(Bulletins.paie.Lignes.paie,
                               .(Matricule),
                               summarise,
                               Statut=Statut[length(Statut)],
                               Service=Service[length(Service)],
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

#'# 3. Rémunérations : analyse pour l'exercice `r année` 
#'## 3.1 Fonctionnaires titulaires et stagiaires
#+ fig.width=7.1



filtre.fonctionnaire <- function (X) X[ Statut %in% c("TITULAIRE", "STAGIAIRE") & X >0 ]

hist(filtre.fonctionnaire(total.rémunérations)/1000,
     xlab="Distribution de la rémunération en k€ \n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 120),
     main=paste("Rémunération annuelle des fonctionnaires\nExercice", année),
     col="blue",
     nclass=50)

#'  
#+ fig.width=7.1

hist(filtre.fonctionnaire(rémunération.contractuelle.ou.indemnitaire)/1000,
     xlab="Distribution des attributions indemnitaires en k€\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 70),
     main=paste("Rémunération indemnitaire annuelle\ndes fonctionnaires\nExercice", année),
     col="blue",
     nclass=50
)

#'    
#+ fig.width=7.1

hist(filtre.fonctionnaire(part.rémunération.contractuelle.ou.indemnitaire),
     xlab="Pourcentage des indemnités dans la rémunération\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     main=paste("Distribution de la part indemnitaire\n de la rémunération annuelle des fonctionnaires\nExercice", année),
     xlim=c(0,60),     
     col="blue",
     nclass=30)

#/* La moyenne est tirée vers le haut par les outlyers */

masse.indemnitaire            <- sum(rémunération.contractuelle.ou.indemnitaire[ traitement.indiciaire > 0])
masse.indiciaire              <- sum(traitement.indiciaire)
masse.rémunérations.brutes    <- sum(total.rémunérations)
ratio.global.masse.indemnitaire  <- masse.indemnitaire/(masse.indiciaire+masse.indemnitaire)*100

#'### Masses indemnitaires pour l'exercice `r année`
#'
Tableau(c("Masse indiciaire", "Masse indemnitaire"),
        masse.indiciaire, masse.indemnitaire)

#'  

Tableau(c("Masse des rémunérations brutes", "Part de la masse indemnitaire"),
        masse.rémunérations.brutes, ratio.global.masse.indemnitaire)
#'
#'Les résultats sont exprimés en euros.  
#'
#'### Statistiques de position pour l'exercice `r année`  
#'

Résumé(c("Traitement indiciaire",
         "Rémunération contractuelle ou indemnitaire",
         "Autres rémunérations"),
       Analyse.rémunérations[Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                             c("traitement.indiciaire",
                               "rémunération.contractuelle.ou.indemnitaire",
                               "autres.rémunérations") ])
#'

Résumé(c("Total rémunérations", "Part de la rémunération contractuelle ou indemnitaire"),
       Analyse.rémunérations[Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                             c( "total.rémunérations", "part.rémunération.contractuelle.ou.indemnitaire") ])

#'## 3.2 Contractuels, vacataires et stagiaires inclus
#+ fig.width=7.1

hist(total.rémunérations[!Statut %in% c("TITULAIRE", "STAGIAIRE") & total.rémunérations > 1000]/1000,
     xlab="Distribution de la rémunération en k€ (supérieure à 1000 €)\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 40),
     main=paste("Rémunération annuelle totale des contractuels\nExercice", année),
     col="red",
     nclass=50)

#'
#+ fig.width=7.1

hist(positive(autres.rémunérations),
     xlab="Distribution des autres rémunérations annuelles en € :\n politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 5000),
     main=paste("Autres rémunérations\nExercice", année),
     col="grey")

#'### Statistiques de position pour l'exercice `r année`  
#'

Résumé(c("Rémunération contractuelle ou indemnitaire", "Autres rémunérations"),
       Analyse.rémunérations[! Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                             c("rémunération.contractuelle.ou.indemnitaire", "autres.rémunérations")])

#'

Résumé("Total rémunérations",
       Analyse.rémunérations[!Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                             "total.rémunérations"])
#'

detach(Analyse.rémunérations)

#'
#'# 4. Analyse des rémunérations nettes sur l'ensemble de la période `r début.période.sous.revue` - `r fin.période.sous.revue` 
#'
#'Nombre d'exercices: `r nombre.exercices`  
#'    
#Ici il faudra afdernier.exerciceer l'input des paramètres pour l'interface en utilisatant get(get) par exemple.
#ou alors un processeur

#/* faire renseigner par la structure pour les colonnes début et dernier.exercice.
# A priori on pourrait aussi prendre : Min(Mois) comme début et Max(Mois) comme dernier.exercice en appliquant la quotité. C'est imprécis. 
# Il y a donc tros inputs à solliciter : les débuts / dernier.exercice d'activité, les catégories statutaires, les avantages en nature
# Ces trois champs sont des inputs natifs; ils se font au niveau 
# d'un fichier du type
#  personne : 
#     début/dernier.exercice :  début (date dernier.exercice)
#     catégorie : X (date nouvelle)
#     NAS :  avantage (date suppression)
# par défaut la personne débute au commencement de la période sous revue et a pour catégorie AUTRE et NAS non
# plus la validation de la codification des indemnités (confirmation) */

#' 
#'## 4.1 Rémunération nette moyenne sur la période

attach(Analyse.variations.synthèse)

hist(moyenne.rémunération.annuelle.sur.période[moyenne.rémunération.annuelle.sur.période > 0]/1000,
     xlab=paste0("Distribution de la rémunération nette moyenne sur la période ",début.période.sous.revue,"-",fin.période.sous.revue," en k€"),
     ylab="Effectif",
     main="Rémunération nette moyenne",
     col="blue",
     nclass=100)

#'

Analyse.variations.synthèse.filtrée <- na.omit(Analyse.variations.synthèse[ nb.jours.exercice.début > seuil.troncature 
                                                                          & nb.jours.exercice.sortie   > seuil.troncature
                                                                              , c("rémunération.début" ,
                                                                                  "rémunération.sortie",
                                                                                  "moyenne.rémunération.annuelle.sur.période",
                                                                                  "variation.rémunération.jour",
                                                                                  "variation.moyenne.rémunération.jour", "plus.2.ans")])

Analyse.variations.synthèse.filtrée.plus.2.ans  <- Analyse.variations.synthèse.filtrée[Analyse.variations.synthèse.filtrée$plus.2.ans, -6]
Analyse.variations.synthèse.filtrée.moins.2.ans <- Analyse.variations.synthèse.filtrée[! Analyse.variations.synthèse.filtrée$plus.2.ans, -6]
Analyse.variations.synthèse.filtrée <- Analyse.variations.synthèse.filtrée[, -6]

detach(Analyse.variations.synthèse)

#' 
#'## 4.2 Evolutions des rémunérations nettes sur la période `r début.période.sous.revue` - `r fin.période.sous.revue` 
#'
#'### 4.2.1 Ensemble des personnels
#'

#'

f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[Analyse.variations.par.exercice$Année == x, "Montant.net"])/ 1000, big.mark=" ", digits=5, format="fg")

Tableau.vertical(c("Année", "Rémunération nette totale (k&euro;)"), 
                 début.période.sous.revue:fin.période.sous.revue, 
                 f)


#'


Résumé(   c("Première année",
            "Dernière année",
            "Moyenne sur la période<br>d'activité",
            "Variation sur la période<br>d'activité",
            "Variation annuelle moyenne"),
          Analyse.variations.synthèse.filtrée)

#'
#'**Nota:** Seuls sont pris en compte les personnels en fonction au moins 100 jours la première et la dernière année    
#'
#'### 4.2.2 Personnels en place
#'

hist(Analyse.variations.synthèse.filtrée.plus.2.ans$variation.moyenne.rémunération.jour,
     xlab ="Variation annuelle moyenne\nde la rémunération nette en %",
     las=1,
     xlim=c(-5,30),
     sub  = "pour les personnels en place",
     ylab ="Effectifs",
     main ="Distribution de la variation annuelle\nmoyenne de la rémunération nette des personnels en place",
     col ="red",
     nclass=200)

#'
#'

f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[
                               Analyse.variations.par.exercice$Année == x & Analyse.variations.par.exercice$plus.2.ans,
                               "Montant.net"])/ 1000, big.mark=" ", digits=5, format="fg")

Tableau.vertical(c("Année", "Rémunération nette totale<br>des agents en place (k&euro;)"), 
                 début.période.sous.revue:fin.période.sous.revue, 
                 f)


#'

Résumé(c("Première année",
         "Dernière année",
         "Moyenne sur la période",
         "Variation sur la période",
         "Variation annuelle moyenne"),
         Analyse.variations.synthèse.filtrée.plus.2.ans)
#'
#'### 4.2.3 Personnels en fonction moins de deux ans
#'
#'
hist(Analyse.variations.synthèse.filtrée.moins.2.ans$variation.moyenne.rémunération.jour,
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
#'

f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[
  Analyse.variations.par.exercice$Année == x & ! Analyse.variations.par.exercice$plus.2.ans,
  "Montant.net"])/ 1000, big.mark=" ", digits=5, format="fg")

Tableau.vertical(c("Année", "Rémunération nette totale<br>des agents en fonction moins de deux ans (k&euro;)"), 
                 début.période.sous.revue:fin.période.sous.revue, 
                 f)


#'
#'

Résumé(c("Première année",
         "Dernière année",
         "Moyenne sur la période",
         "Variation sur la période",
         "Variation annuelle moyenne"),
          Analyse.variations.synthèse.filtrée.moins.2.ans)

#'
#'Les résultats sont exprimés en euros.
#'
#'# 5. Tests réglementaires
#'## 5.1 Contrôle des heures supplémentaires, des NBI et primes informatiques
#'

attach(Bulletins.paie.Lignes.paie, warn.conflicts=FALSE)

NBI.aux.non.titulaires <- Bulletins.paie.Lignes.paie[ ! Statut %in% c("TITULAIRE","STAGIAIRE") & as.character(Code) %in% codes.NBI, c(étiquette.matricule, "Statut", "Code", "Libellé", "Mois", étiquette.montant)]

nombre.Lignes.paie.NBI.nontit <- nrow(NBI.aux.non.titulaires)

#/* Prime de fonctions informatiques : pas dans la base de VLB
# on cherche la chaine de char. "INFO" dans les libellés de primes

# variante : filtre <- regexpr(".*(INFO|PFI|P.F.I).*", toupper(Bulletins.paie.Lignes.paie$Libellé)) et regmatches(Bulletins.paie.Lignes.paie$Libellé, filtre)*/

filtre<-grep(".*(INFO|PFI|P.F.I).*", Libellé)

personnels.prime.informatique <- Bulletins.paie.Lignes.paie[ filtre, c(étiquette.matricule, "Statut", "Code", "Libellé", étiquette.montant)]

primes.informatiques.potentielles<-unique(Libellé[filtre])

if  (length(primes.informatiques.potentielles) == 0) 
  primes.informatiques.potentielles <- "aucune"
  
nombre.personnels.pfi <- nrow(personnels.prime.informatique)

#'Primes informatiques potentielles : `r primes.informatiques.potentielles`  

Tableau(
  c("Nombre de lignes NBI pour non titulaires",
    "Nombre de bénéficiaires de PFI"),
  nombre.Lignes.paie.NBI.nontit, 
  nombre.personnels.pfi)

#'**Nota :**  
#'NBI: nouvelle bonification indiciaire  
#'PFI: prime de fonctions informatiques  
#'    
#'## 5.2 Contrôle des vacations pour les fonctionnaires  

# Vacations et statut de fonctionnaire

lignes.fonctionnaires.et.vacations <- Bulletins.paie.Lignes.paie[ Statut %in% c("TITULAIRE", "STAGIAIRE") & Code %in% Code.prime[Code.prime$Type.rémunération == "VACATIONS","Code.rubrique"], c(étiquette.matricule, "Statut", "Code", "Libellé", étiquette.montant)]
matricules.fonctionnaires.et.vacations <- unique(lignes.fonctionnaires.et.vacations$Matricule)
nombre.fonctionnaires.et.vacations <- length(matricules.fonctionnaires.et.vacations)
nombre.Lignes.paie.fonctionnaires.et.vacations <- nrow(lignes.fonctionnaires.et.vacations)

#'

Tableau(
  c("Nombre de FEV",
    "Nombre de lignes de vacations pour FEV"),
  nombre.fonctionnaires.et.vacations,
  nombre.Lignes.paie.fonctionnaires.et.vacations)


#'    
#'**Nota:**  
#'FEV : fonctionnaire effectuant des vacations
#'
#'## 5.3 Contrôles sur les cumuls traitement indiciaire, indemnités et vacations des contractuels

# Vacations et régime indemnitaire

lignes.contractuels.et.vacations <- Bulletins.paie.Lignes.paie[ ! Statut %in% c("TITULAIRE", "STAGIAIRE")  & Code %in% Code.prime[Code.prime$Type.rémunération == "VACATIONS","Code.rubrique"], c(étiquette.matricule, "Code", "Libellé", étiquette.montant)]
matricules.contractuels.et.vacations <- unique(lignes.contractuels.et.vacations$Matricule)
nombre.contractuels.et.vacations <- length(matricules.contractuels.et.vacations)

RI.et.vacations <- Bulletins.paie.Lignes.paie[ Matricule %in% matricules.contractuels.et.vacations & Code %in% Code.prime[Code.prime$Type.rémunération == "INDEMNITAIRE.OU.CONTRACTUEL","Code.rubrique"], c(étiquette.matricule, "Statut", "Code", "Libellé", étiquette.montant)]

# Vacations et indiciaire

traitement.et.vacations <- Bulletins.paie.Lignes.paie[ Matricule %in% matricules.contractuels.et.vacations & Code %in% Code.prime[Code.prime$Type.rémunération == "TRAITEMENT","Code.rubrique"], c(étiquette.matricule, "Statut", "Code", "Libellé", étiquette.montant)]

nombre.Lignes.paie.contractuels.et.vacations <- nrow(lignes.contractuels.et.vacations)
nombre.Lignes.paie.RI.et.vacations <- nrow(RI.et.vacations)
nombre.Lignes.paie.traitement.et.vacations <- nrow(traitement.et.vacations)

#'

Tableau(c("Nombre de CEV",
          "Nombre de lignes CEV",
          "Nombre de lignes indemnitaires pour CEV",
          "Nombre de lignes de traitement pour CEV"),
          nombre.contractuels.et.vacations,
          nombre.Lignes.paie.contractuels.et.vacations,
          nombre.Lignes.paie.RI.et.vacations,
          nombre.Lignes.paie.traitement.et.vacations)

#'    
#'**Nota:**  
#'CEV : contractuel effectuant des vacations

#'
#'## 5.4 Contrôle sur les indemnités IAT et IFTS

#IAT et IFTS

filtre.iat<-grep(".*(IAT|I.A.T|.*Adm.*Tech).*", Libellé, ignore.case=TRUE)
filtre.ifts<-grep(".*(IFTS|I.F.T.S|.*FORF.*TRAV.*SUPP).*", Libellé, ignore.case=TRUE)
codes.ifts <- unique(Bulletins.paie.Lignes.paie[filtre.ifts, "Code"])

nombre.personnels.iat.ifts <- length(personnels.iat.ifts <- intersect(as.character(Bulletins.paie.Lignes.paie[ filtre.iat, c(étiquette.matricule)]), as.character(Bulletins.paie.Lignes.paie[ filtre.ifts, c(étiquette.matricule)])))

#'

Tableau(c("Codes IFTS", "Nombre de personnels percevant IAT et IFTS"), codes.ifts, nombre.personnels.iat.ifts)

#'
#'### Contrôle sur les IFTS pour catégories B et contractuels

#IFTS et IB >= 380 (IM >= 350)

df1 <- Bulletins.paie.Lignes.paie[ Indice < 350, c(étiquette.matricule)]
df1 <- df1[!duplicated(df1)]

df2 <- Bulletins.paie.Lignes.paie[ filtre.ifts, c(étiquette.matricule)]
df2 <- df2[!duplicated(df2)]

df3 <- intersect(df1,df2)

lignes.ifts.anormales <- Bulletins.paie.Lignes.paie[Matricule %in% df3 & Code %in% codes.ifts & (Indice < 380 ), c(étiquette.matricule, "Statut", "Code", "Libellé", "Indice", étiquette.montant)]
nombre.lignes.ifts.anormales <- length(lignes.ifts.anormales)

rm(df1, df2, df3)
# IFTS et non tit

ifts.et.contractuel <- Bulletins.paie.Lignes.paie[Code %in% codes.ifts & ! Statut %in% c("TITULAIRE", "STAGIAIRE"), c(étiquette.matricule, "Statut", "Code", "Libellé", "Indice", étiquette.montant)]
nombres.lignes.ifts.et.contractuel <- length(ifts.et.contractuel)

#'

Tableau(c("Nombre de contractuels percevant des IFTS", "Nombre de lignes IFTS pour IB < 380"), nombres.lignes.ifts.et.contractuel, nombre.lignes.ifts.anormales)

#'    
#'**Nota:**  
#'IB < 380 : fonctionnaire percevant un indice brut inférieur à 380  
#'
#'###### page break
#'
#'## 5.5 Contrôle sur les heures supplémentaires

HS.sup.25 <- Bulletins.paie.Lignes.paie[Heures.Sup. >= 25 , c(étiquette.matricule, "Statut", "Mois", "Heures.Sup.", "Brut")]
nombre.Lignes.paie.HS.sup.25 <- nrow(HS.sup.25)

# with(Base2,
#      ihts <<- Base2[! Code.catégorie %in% c("B", "C") & substr(Code,1,2) %in% c("19") & ! grepl(" ENS", Libellé), c(étiquette.matricule, "Code", "Libellé", étiquette.montant, "Code.catégorie")]
# )

ihts <- character(0)
nombre.ihts.anormales <- length(ihts)
#'
 
Tableau(c("Nombre de lignes HS en excès", "Nombre de lignes IHTS anormales"), nombre.Lignes.paie.HS.sup.25, nombre.ihts.anormales)

#'
#'    
#'**Nota:**  
#'HS en excès : au-delà de 25 heures par mois  
#'IHTS anormales : non attribuées à des fonctionnaires de catégorie B ou C.  
#'
#'****
#'
#'# Annexe

matricules.à.identifier <- unique(data.frame(Bulletins.paie.Lignes.paie$Nom, Bulletins.paie.Lignes.paie$Prénom, Bulletins.paie.Lignes.paie$Matricule))
Catégorie <- character(length=nrow(matricules.à.identifier))
matricules.à.identifier <- cbind(matricules.à.identifier, Catégorie)
names(matricules.à.identifier) <- c("Nom", "Prénom", étiquette.matricule, "Catégorie")

kable(matricules.à.identifier, row.names=FALSE)

#'

detach(Bulletins.paie.Lignes.paie)

#/* ------------------------------------------------------------------------------------------------------------------*/
#  Sauvegardes : enlever les commentaires en mode opérationnel
##

Masses <- data.frame(masse.indemnitaire, masse.indiciaire, masse.rémunérations.brutes, ratio.global.masse.indemnitaire)

if (sauvegarder.bases) 
  sauv.bases("Bases",
    "Analyse.rémunérations",
    "Analyse.variations.par.exercice",
    "Analyse.variations.synthèse",
    "Analyse.variations.synthèse.filtrée.plus.2.ans",
    "Bulletins.paie.nir.total.hors.élus",
    "Bulletins.paie.nir.fonctionnaires",
    "Bulletins.paie.Lignes.paie", 
    "Masses",
    "NBI.aux.non.titulaires",
    "RI.et.vacations",
    "HS.sup.25",
    "personnels.prime.informatique",
    "liste.matricules.fonctionnaires",
    "lignes.contractuels.et.vacations",
    "lignes.fonctionnaires.et.vacations",
    "lignes.ifts.anormales",
    "matricules.contractuels.et.vacations",
    "matricules.fonctionnaires.et.vacations")


