#'---
#'title: false
#'author: false
#'date: false
#'output:
#'html_document:
#'css: style.css
#'---

#'<p class = "centered"><img src = "altair.png" /></p>
#'<p class = "title">RH Sierg</p> 
#'

#+ echo = FALSE, warning = FALSE, message = FALSE

# comportement global du programme

sauvegarder.bases <- TRUE

# Lorsque l'on n'a que une ou deux années, mettre étudier.variations à FALSE
# Lorsque l'on n'étudie pas une base Xémélios, mettre étudier.tests.statutaires à FALSE

library(compiler)

options(warn = -1, verbose = FALSE, OutDe = ",", encoding = "UTF-8")

try(setwd("Tests/Sierg"))

source("prologue.R", chdir=TRUE)

compilerOptions <- setCompilerOptions(suppressAll = TRUE)
JITlevel <- enableJIT(3)

source(file.path(chemin.dossier, "bibliotheque.fonctions.paie.R"))

base.personnels.catégorie <- data.frame(NULL)

installer.paquets(knitr, plyr, ggplot2, assertthat, yaml)

library(knitr)
library(plyr)
library(ggplot2)
library(assertthat)

knitr::opts_chunk$set(fig.retina = 2, fig.width = 7.5, echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')

dir.create(chemin("Bases"), recursive = TRUE)

#'<p class = "centered"><b>Exercices `r paste(début.période.sous.revue, "à", fin.période.sous.revue)` </b></p>  
#'<p class = "author">Fabrice Nicol</h1>
#'
#+ echo = FALSE
#'`r format(Sys.Date(), "%a %d %b %Y")`  
#'

nombre.exercices <- fin.période.sous.revue - début.période.sous.revue + 1

étiquette.montant <- "Montant"
seuil.troncature <- 99

# Le format est jour/mois/année avec deux chiffres-séparateur-deux chiffres-séparateur-4 chiffres.
#Le séparateur peut être changé en un autre en modifiant le "/" dans fate.format

nom.fichier.paie  <- "Lignes de paye"
nom.bulletin.paie <- "Bulletins de paye"

# Cette section pourra être modifiée en entrée dans d'autres contextes
# Matricule, Codes, Avantages en nature 

champ.détection.élus <- "Service"
champ.nir <- "Nir"
libellé.élus <- "Elus"
   
codes.paiement          <- "LISTE DES RUBRIQUES DE TRAITEMENT UTILISEES EN 2012.csv"
#matricule.avantage  <- "LISTE DES AGENTS AYANT BENEFICIE D'AVANTAGE EN NATURE EN 2012.csv"
code.traitement <- 1010

fichier.personnels <- "Catégories des personnels"
nom.fichier.personnels <- paste0(fichier.personnels, ".csv")
fichier.personnels.existe <- file.exists(chemin(nom.fichier.personnels))
if (fichier.personnels.existe)
  base.personnels.catégorie = read.csv.skip(nom.fichier.personnels)

#/* Lignes de paie
# On peut lire jusqu'à 10 fichiers csv qui seront générés au format
#  "chemin dossier + paies-Bulletins de paye-j.csv" */

Lignes.paie <- outer(début.période.sous.revue:fin.période.sous.revue, 1:10, paste, nom.fichier.paie, "csv", sep=".")

#/* Bulletins de paie */

Bulletins.paie <- paste(début.période.sous.revue:fin.période.sous.revue, nom.bulletin.paie, "csv", sep=".")

codes.NBI <- c("1012", "101B", "101M", "4652", "4672")


#/* Programme principal
##
#  Bases
##
# Lignes de paie  */

Lignes.paie <- Lignes.paie[file.exists(chemin(Lignes.paie))]

Lignes.paie<-Read.csv(Lignes.paie)

Bulletins.paie <- Read.csv(Bulletins.paie)

codes.paiement          <- read.csv.skip(codes.paiement)
# Matricule.avantage  <- read.csv.skip(matricule.avantage)

# suppression des colonnes Nom Prénom redondantes

Bulletins.paie.premier.exercice <-  selectionner.cle.matricule.mois(Bulletins.paie[Bulletins.paie$Année == début.période.sous.revue,], Lignes.paie)

Bulletins.paie.dernier.exercice <-  selectionner.cle.matricule.mois(Bulletins.paie[Bulletins.paie$Année == fin.période.sous.revue,], Lignes.paie)

# Matricule.avantage  <- selectionner.cle.matricule(Matricule.avantage, Matricule.catégorie) 

if (!setequal(intersect(names(Lignes.paie), names(Bulletins.paie.dernier.exercice)), c("Mois", étiquette.matricule)))
  stop("L'appariement ne peut se faire par les clés Matricule et Mois")

liste.matricules.fonctionnaires <- unique(Bulletins.paie.dernier.exercice[Bulletins.paie.dernier.exercice$Mois == 12 & Bulletins.paie.dernier.exercice$Statut %in% c("TITULAIRE", "STAGIAIRE"), étiquette.matricule])

liste.matricules.élus   <- unique(Bulletins.paie.dernier.exercice[Bulletins.paie.dernier.exercice$Mois == 12 &  Bulletins.paie.dernier.exercice[champ.détection.élus] == libellé.élus, étiquette.matricule])

Bulletins.paie.nir.total.hors.élus <- Bulletins.paie.dernier.exercice[Bulletins.paie.dernier.exercice$Mois == 12 & ! Bulletins.paie.dernier.exercice$Matricule %in% liste.matricules.élus, c(étiquette.matricule,champ.nir)]

Bulletins.paie.nir.fonctionnaires  <- Bulletins.paie.dernier.exercice[Bulletins.paie.dernier.exercice$Mois == 12 &   Bulletins.paie.dernier.exercice$Matricule %in% liste.matricules.fonctionnaires, c(étiquette.matricule, champ.nir)]

# Age au 31 décembre de l'exercice dernier.exerciceal de la période sous revue

années.fonctionnaires   <-fin.période.sous.revue-(as.numeric(substr(as.character(Bulletins.paie.nir.fonctionnaires[,champ.nir]), 2, 3)) + 1900)
années.total.hors.élus  <-fin.période.sous.revue-(as.numeric(substr(as.character(Bulletins.paie.nir.total.hors.élus[,champ.nir]), 2, 3)) + 1900)

########### Démographie ########################

#'# 1. Statistiques de population
#'
#'### 1.1 Ensemble des personnels non élus    


hist(années.total.hors.élus,
     xlab = paste("Âge au 31 décembre",fin.période.sous.revue),
     xlim = c(18, 75),
     ylab = "Effectif",
     main = "Pyramide des âges",
     col = "blue",
     nclass = 50)

#'
#'[Lien vers la base des âges](Bases/Bulletins.paie.nir.total.hors.élus.csv)
#'

Résumé(paste0("Âge des personnels <br>au 31/12/",fin.période.sous.revue), années.total.hors.élus, align = 'c')

#'Effectif total: `r length(années.total.hors.élus)`  
#'
#'######      <br>
#'
#'
#'### 1.2 Ensemble des fonctionnaires stagiaires et titulaires    

hist(années.fonctionnaires,
     xlab = paste("Âge au 31 décembre",fin.période.sous.revue),
     xlim = c(18,68),
     ylab = "Effectif",
     main = "Pyramide des âges des fonctionnaires",
     col = "navy",
     nclass = 50)

#'
#'[Lien vers la base des âges](Bases/Bulletins.paie.nir.fonctionnaires.csv)
#'

Résumé(paste0("Âge des personnels <br>au 31/12/",fin.période.sous.revue), années.fonctionnaires, align = 'c')

#'   
#'**Effectif total: `r length(années.fonctionnaires)`**     
#'
  
Analyse.variations.par.exercice <- ddply(Bulletins.paie, 
             c(étiquette.matricule, étiquette.année),
             summarise,
             Montant.net = sum(Net.à.Payer),
             Statut = Statut[length(Net.à.Payer)],
             mois.entrée = ifelse((minimum <- min(Mois)) != Inf, minimum, 0),
             mois.sortie = ifelse((maximum <- max(Mois)) != -Inf, maximum, 0),
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
                            Nexercices = length(Année),
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
                            moins.six.mois = (total.jours < 365/2),
                            statut = Statut[1])

####  On pourrait aussi plus simplement poser  ###
#  que plus.deux.ans soit défini comme length(Année) >= 2.
#  On préfère une définition à partir de total.jours pour 
#  avoir une définition cohérente sur toutes les durées, y.c infra-annuelles

attach(Analyse.variations.par.exercice, warn = FALSE)

Analyse.variations.par.exercice <- na.omit(Analyse.variations.par.exercice[ nb.jours[1] > seuil.troncature
                                                                 &  nb.jours[length(Année)] > seuil.troncature, ])

Analyse.variations.par.exercice <- mutate(Analyse.variations.par.exercice,
                                          plus.2.ans = Matricule
                                                          %in%
                                                       Analyse.variations.synthèse[Analyse.variations.synthèse$plus.2.ans, étiquette.matricule])


detach(Analyse.variations.par.exercice)

attach(Analyse.variations.synthèse, warn.conflicts = FALSE)

#'
#'<!-- BREAK -->
#'
#'
#'### 1.3 Effectifs des personnels par durée de service
#'
#'**Personnels en fonction des exercices `r début.période.sous.revue` à `r fin.période.sous.revue` inclus :**  
#'

Tableau(c("Plus de 2 ans",
          "Moins de 2 ans",
          "Moins d'un an", 
          "Moins de six mois"), 
        sum(plus.2.ans),
        sum(moins.2.ans),
        sum(moins.1.an), 
        sum(moins.six.mois))

#'

detach(Analyse.variations.synthèse)

qplot(factor(Année), 
      data = Analyse.variations.par.exercice,
      geom = "bar",
      fill = factor(!plus.2.ans),
      main = paste("Evolutions entre", début.période.sous.revue,"et", fin.période.sous.revue),
      xlab = étiquette.année,
      ylab = "Effectif",
      asp = 4) + 
  scale_fill_discrete(name = "Personnels en fonction",
                      breaks = c(TRUE, FALSE),
                      labels = c("Moins de deux ans", "Plus de deux ans"))
  
#'
#'**Nota:**  
#'Personnels en place : ayant servi au moins 730 jours pendant la période.  
#'Toutes les durées de service sont calculées en multiples de 365 jours.  
#'Dans les statistiques de cette section, les élus ne sont pas pris en compte.
#'

#fusion matricule | avantage | catégorie par Matricule

# peut aisément être modifié ainsi

année <- début.période.sous.revue

colonnes.sélectionnées <- c("traitement.indiciaire",
                            ic("rémunération.contractuelle.ou.indemnitaire"),
                            ic("autres.rémunérations"), 
                            ic("total.rémunérations"),
                            ic("part.rémunération.contractuelle.ou.indemnitaire"),
                            étiquette.matricule)



# pour année début #

Bulletins.paie.Lignes.paie <- merge(Bulletins.paie.premier.exercice, Lignes.paie[Lignes.paie$Année == année,])

Bulletins.paie.Lignes.paie <- mutate(Bulletins.paie.Lignes.paie,
                                     montant.traitement.indiciaire 
                                     = Montant*(Code %in% codes.paiement[codes.paiement$Type.rémunération 
                                                                     == "TRAITEMENT","Code.rubrique"]),
                                     montant.primes 
                                     = Montant*(Code %in% codes.paiement[codes.paiement$Type.rémunération 
                                                                     == "INDEMNITAIRE.OU.CONTRACTUEL","Code.rubrique"]),
                                     montant.autres.rémunérations 
                                     = Montant*(Code %in% codes.paiement[codes.paiement$Type.rémunération
                                                                     == "AUTRES","Code.rubrique"]),
                                     montant.indemnité.élu 
                                     = Montant*(Code %in% codes.paiement[codes.paiement$Type.rémunération
                                                                     == "ELU","Code.rubrique"]))


###########  Analyse des rémunérations  début de période###################
#  ATTENTION : les années doivent être SUCCESSIVES                        #

Analyse.rémunérations <- ddply(Bulletins.paie.Lignes.paie,
                               .(Matricule),
                               summarise,
                               Statut = Statut[année - début.période.sous.revue + 1],
                               Service = Service[année - début.période.sous.revue + 1],
                               traitement.indiciaire = sum(montant.traitement.indiciaire),
                               rémunération.contractuelle.ou.indemnitaire = sum(montant.primes),
                               indemnités.élu                             = sum(montant.indemnité.élu),
                               autres.rémunérations                       = sum(montant.autres.rémunérations),
                               total.rémunérations                        = traitement.indiciaire 
                               + rémunération.contractuelle.ou.indemnitaire 
                               + autres.rémunérations
                               + indemnités.élu,
                               part.rémunération.contractuelle.ou.indemnitaire = 
                                 ifelse(traitement.indiciaire + rémunération.contractuelle.ou.indemnitaire == 0, 0,
                                        rémunération.contractuelle.ou.indemnitaire /
                                          (traitement.indiciaire + rémunération.contractuelle.ou.indemnitaire)*100))

if (fichier.personnels.existe)
{
  Analyse.rémunérations <- merge(Analyse.rémunérations, base.personnels.catégorie, by = étiquette.matricule, all = FALSE)
  colonnes.sélectionnées <- c(colonnes.sélectionnées, "Catégorie") 
}

attach(Analyse.rémunérations, warn.conflicts = FALSE)

########### Analyse statique premier exercice ########################
#'# 2. Rémunérations brutes : analyse pour l'exercice `r année` 
#'## 2.1 Fonctionnaires titulaires et stagiaires
#'



filtre.fonctionnaire <- function (X) X[ Statut %in% c("TITULAIRE", "STAGIAIRE") & X >0 ]

hist(filtre.fonctionnaire(total.rémunérations)/1000,
     xlab = "En milliers d'euros hors politique familiale\nindemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 120),
     main = paste("Rémunération annuelle des fonctionnaires en", année),
     col = "blue",
     nclass = 50)

#'    
#'##         
#'    
hist(filtre.fonctionnaire(rémunération.contractuelle.ou.indemnitaire)/1000,
     xlab = "En milliers d'euros hors politique familiale\nindemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 70),
     main = paste("Rémunération indemnitaire annuelle des fonctionnaires en", année),
     col = "blue",
     nclass = 50
)

#'
#'<!-- BREAK -->
#'
       

hist(filtre.fonctionnaire(part.rémunération.contractuelle.ou.indemnitaire),
     xlab = "Part des indemnités dans la rémunération en %\n hors politique familiale, indemnités journalières et remboursements",
     ylab = "Effectif",
     main = paste("Part indemnitaire de la rémunération annuelle des fonctionnaires en", année),
     xlim = c(0,60),     
     col = "blue",
     nclass = 30)

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
#'<!-- BREAK -->
#'

Tableau(c("Masse des rémunérations brutes", "Part de la masse indemnitaire"),
        masse.rémunérations.brutes, ratio.global.masse.indemnitaire)

#'

df <- data.frame(masse.indemnitaire, masse.indiciaire, masse.rémunérations.brutes, ratio.global.masse.indemnitaire)

Sauv.base(chemin("Bases"), "df", paste0("Masses.", année))

#'
#'[Lien vers la base de données](Bases/`r paste0("Masses.", année, ".csv")`)    
#'        
#'Les résultats sont exprimés en euros.  
#'  
#'
#'
#'######      <br>
#'   
#'### Statistiques de position pour l'exercice `r année`  
#'

AR <- Analyse.rémunérations[Statut %in% c("TITULAIRE", "STAGIAIRE"), colonnes.sélectionnées]

Résumé(c("Traitement indiciaire",
         étiquette.rém.indemn,
         "Autres rémunérations"),  AR[1:3])

#'    
#'
Résumé(c("Total rémunérations", "Part de la rémunération contractuelle ou indemnitaire"), AR[4:5])

#'     
#'**Effectif : `r nrow(AR)`**     
#'
#'<!-- BREAK -->
#'

#'### Statistiques de position par catégorie pour l'exercice `r année` 
#'

ARA <- AR[AR$Catégorie == 'A', ]
ARB <- AR[AR$Catégorie == 'B', ]
ARC <- AR[AR$Catégorie == 'C', ]

#'**Catégorie A**     
#'
#'     


if (fichier.personnels.existe)
{  
  Résumé(c("Traitement indiciaire",
           étiquette.rém.indemn,
           "Autres rémunérations"), ARA[c("traitement.indiciaire",
                                          "rémunération.contractuelle.ou.indemnitaire",
                                          "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{  
  Résumé(c("Total rémunérations", "Part de la rémunération contractuelle ou indemnitaire"),
          ARA[c( "total.rémunérations", "part.rémunération.contractuelle.ou.indemnitaire")])
}
#'    
#'**Effectif : `r nrow(ARA)`**          
#'    
#'**Catégorie B**     
#'     

if (fichier.personnels.existe)
{  
  Résumé(c("Traitement indiciaire",
           étiquette.rém.indemn,
           "Autres rémunérations"), 
         ARB[ c("traitement.indiciaire",
                "rémunération.contractuelle.ou.indemnitaire",
                "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{  
  Résumé(c("Total rémunérations", "Part de la rémunération contractuelle ou indemnitaire"),
         ARB[ c( "total.rémunérations", "part.rémunération.contractuelle.ou.indemnitaire")])
}

#'     
#'**Effectif : `r nrow(ARB)`**     
#'    
#'**Catégorie C**     
#'
#'     

if (fichier.personnels.existe)
{      
  Résumé(c("Traitement indiciaire",
           étiquette.rém.indemn,
           "Autres rémunérations"), 
         ARC[ c("traitement.indiciaire",
                "rémunération.contractuelle.ou.indemnitaire",
                "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{  
  Résumé(c("Total rémunérations", "Part de la rémunération contractuelle ou indemnitaire"),
         ARC[ c( "total.rémunérations", "part.rémunération.contractuelle.ou.indemnitaire") ])
}

#'**Effectif : `r nrow(ARC)`**     
#'    
#'    
#'######      <br>
#'
#'## 2.2 Contractuels, vacataires et stagiaires inclus
#'

hist(total.rémunérations[! Matricule %in% liste.matricules.élus & !Statut %in% c("TITULAIRE", "STAGIAIRE") & total.rémunérations > 1000]/1000,
     xlab = "Rémunération en milliers d'euros \n hors politique familiale, indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 40),
     main = paste("Rémunération annuelle totale des contractuels en", année),
     col = "red",
     nclass = 50)
#'
#'**Nota:**   
#'Ne sont retenues que les rémunérations supérieures à 1 000 €.  
#'Les élus ne sont pas pris en compte.   
#'

hist(positive(autres.rémunérations),
     xlab = "En euros :\n politique familiale, indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 5000),
     main = paste("Autres rémunérations en", année),
     col = "grey")

#'   

Résumé(c(étiquette.rém.indemn, "Autres rémunérations"),
       Analyse.rémunérations[ ! Matricule %in% liste.matricules.élus & ! Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                             c(ic("rémunération.contractuelle.ou.indemnitaire"), ic("autres.rémunérations"))])
#'

Résumé("Total rémunérations",
       Analyse.rémunérations[ ! Matricule %in% liste.matricules.élus & !Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                             "total.rémunérations"])

nom.base.analyse <- paste0("Analyse.rémunérations.", année)

if (sauvegarder.bases)
  Sauv.base(chemin("Bases"), ic("Analyse.rémunérations"), nom.base.analyse)

#'
#'[Lien vers la base de données](Bases/`r nom.base.analyse`.csv) d'analyse des rémunérations pour `r année`
#'

detach(Analyse.rémunérations)

# pour année fin #

année <- fin.période.sous.revue

#'
#'

Bulletins.paie.Lignes.paie <- merge(Bulletins.paie.dernier.exercice, Lignes.paie[Lignes.paie$Année == année,])

Bulletins.paie.Lignes.paie <- mutate(Bulletins.paie.Lignes.paie,
                                     montant.traitement.indiciaire 
                                     = Montant*(Code %in% codes.paiement[codes.paiement$Type.rémunération 
                                                                     == "TRAITEMENT","Code.rubrique"]),
                                     montant.primes 
                                     = Montant*(Code %in% codes.paiement[codes.paiement$Type.rémunération 
                                                                     == "INDEMNITAIRE.OU.CONTRACTUEL","Code.rubrique"]),
                                     montant.autres.rémunérations 
                                     = Montant*(Code %in% codes.paiement[codes.paiement$Type.rémunération
                                                                     == "AUTRES","Code.rubrique"]),
                                     montant.indemnité.élu 
                                     = Montant*(Code %in% codes.paiement[codes.paiement$Type.rémunération
                                                                     == "ELU","Code.rubrique"]))

###########  Analyse des rémunérations  fin de période###################
#  ATTENTION : les années doivent être SUCCESSIVES                      #


Analyse.rémunérations <- ddply(Bulletins.paie.Lignes.paie,
                               .(Matricule),
                               summarise,
                               Statut = Statut[année - début.période.sous.revue + 1],
                               Service = Service[année - début.période.sous.revue + 1],
                               traitement.indiciaire = sum(montant.traitement.indiciaire),
                               rémunération.contractuelle.ou.indemnitaire = sum(montant.primes),
                               indemnités.élu                             = sum(montant.indemnité.élu),
                               autres.rémunérations                       = sum(montant.autres.rémunérations),
                               total.rémunérations                        = traitement.indiciaire 
                               + rémunération.contractuelle.ou.indemnitaire 
                               + autres.rémunérations
                               + indemnités.élu,
                               part.rémunération.contractuelle.ou.indemnitaire = 
                                 ifelse(traitement.indiciaire + rémunération.contractuelle.ou.indemnitaire == 0, 0,
                                        rémunération.contractuelle.ou.indemnitaire /
                                          (traitement.indiciaire + rémunération.contractuelle.ou.indemnitaire)*100))


if (fichier.personnels.existe)
{
  Analyse.rémunérations <- merge(Analyse.rémunérations, base.personnels.catégorie, by = étiquette.matricule, all = FALSE)
}

attach(Analyse.rémunérations, warn.conflicts = FALSE)

########### Analyse statique dernier exercice ########################

#'# 3. Rémunérations brutes : analyse pour l'exercice `r année` 
#'## 3.1 Fonctionnaires titulaires et stagiaires
#'

filtre.fonctionnaire <- function (X) X[ Statut %in% c("TITULAIRE", "STAGIAIRE") & X >0 ]

hist(filtre.fonctionnaire(total.rémunérations)/1000,
     xlab = "En milliers d'euros \n hors politique familiale, indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 120),
     main = paste("Rémunération annuelle des fonctionnaires en", année),
     col = "blue",
     nclass = 50)

#'  
#'

hist(filtre.fonctionnaire(rémunération.contractuelle.ou.indemnitaire)/1000,
     xlab = "En milliers d'euros\n hors politique familiale, indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 70),
     main = paste("Rémunération indemnitaire annuelle\ndes fonctionnaires en", année),
     col = "blue",
     nclass = 50
)

#'    
#'

hist(filtre.fonctionnaire(part.rémunération.contractuelle.ou.indemnitaire),
     xlab = "Pourcentage des indemnités dans la rémunération\n hors politique familiale, indemnités journalières et remboursements",
     ylab = "Effectif",
     main = paste("Part indemnitaire de la rémunération annuelle des fonctionnaires en", année),
     xlim = c(0,60),     
     col = "blue",
     nclass = 30)

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

df <- data.frame(masse.indemnitaire, masse.indiciaire, masse.rémunérations.brutes, ratio.global.masse.indemnitaire)
Sauv.base(chemin("Bases"), "df", paste0("Masses.", année))

#'[Lien vers la base de données](Bases/`r paste0("Masses.", année, ".csv")` )   
#'
#'Les résultats sont exprimés en euros.  
#'
#'
#'######      
#'
#'### Statistiques de position pour l'exercice `r année`  
#'

AR <- Analyse.rémunérations[Statut %in% c("TITULAIRE", "STAGIAIRE"), colonnes.sélectionnées ]

Résumé(c("Traitement indiciaire",
         étiquette.rém.indemn,
         "Autres rémunérations"),  AR[1:3])
#'

Résumé(c("Total rémunérations", "Part de la rémunération contractuelle ou indemnitaire"), AR[4:5])
       
#'**Effectif : `r nrow(AR)`**     
#'   
#'######      
#'
#'### Statistiques de position par catégorie pour l'exercice `r année` 
#'

ARA <- AR[AR$Catégorie == 'A', ]
ARB <- AR[AR$Catégorie == 'B', ]
ARC <- AR[AR$Catégorie == 'C', ]

#'**Catégorie A**     
#'
#'     

if (fichier.personnels.existe)
{  
  Résumé(c("Traitement indiciaire",
                étiquette.rém.indemn,
                 "Autres rémunérations"),
         ARA[c("traitement.indiciaire",
               "rémunération.contractuelle.ou.indemnitaire",
               "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{  
  Résumé(c("Total rémunérations", "Part de la rémunération contractuelle ou indemnitaire"),
         ARA[ c( "total.rémunérations", "part.rémunération.contractuelle.ou.indemnitaire")])
}

#'**Effectif : `r nrow(ARA)`**          
#'    
#'**Catégorie B**     
#'     

if (fichier.personnels.existe)
{  
  Résumé(c("Traitement indiciaire",
           étiquette.rém.indemn,
           "Autres rémunérations"),
         ARB[c("traitement.indiciaire",
               "rémunération.contractuelle.ou.indemnitaire",
               "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{  
  Résumé(c("Total rémunérations", "Part de la rémunération contractuelle ou indemnitaire"),
         ARB[ c( "total.rémunérations", "part.rémunération.contractuelle.ou.indemnitaire")])
}
#'
#'**Effectif : `r nrow(ARB)`**          
#'
#'**Catégorie C**     
#'
#'     

if (fichier.personnels.existe)
{      
  Résumé(c("Traitement indiciaire",
           étiquette.rém.indemn,
           "Autres rémunérations"),
         ARC[ c("traitement.indiciaire",
                "rémunération.contractuelle.ou.indemnitaire",
               "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{  
  Résumé(c("Total rémunérations", "Part de la rémunération contractuelle ou indemnitaire"),
         ARC[ c( "total.rémunérations", "part.rémunération.contractuelle.ou.indemnitaire")])
}
#'  
#'**Effectif : `r nrow(ARC)`**     
#'
#'######      
#'
#'
#'## 3.2 Contractuels, vacataires et stagiaires inclus
#'

hist(total.rémunérations[ ! Matricule %in% liste.matricules.élus & ! Statut %in% c("TITULAIRE", "STAGIAIRE") & total.rémunérations > 1000]/1000,
     xlab = "Rémunération en milliers d'euros\n hors politique familiale, indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 40),
     main = paste("Rémunération annuelle totale des contractuels en", année),
     col = "red",
     nclass = 50)

#'
#'**Nota:**   
#'Ne sont retenues que les rémunérations supérieures à 1 000 €.  
#'Les élus ne sont pas pris en compte.   

hist(positive(autres.rémunérations),
     xlab = "En euros :\n politique familiale, indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 5000),
     main = paste("Autres rémunérations en", année),
     col = "grey")
#'    

Résumé(c(étiquette.rém.indemn, "Autres rémunérations"),
       Analyse.rémunérations[ ! Matricule %in% liste.matricules.élus & ! Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                             c(ic("rémunération.contractuelle.ou.indemnitaire"), ic("autres.rémunérations"))])

#'

AR <- Analyse.rémunérations[ ! Matricule %in% liste.matricules.élus & ! Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                       "total.rémunérations"]

Résumé("Total rémunérations",   AR)
#'   
#'**Effectif : `r nrow(AR)`**     
#'   

detach(Analyse.rémunérations)

nom.base.analyse <- paste0("Analyse.rémunérations.", année)

if (sauvegarder.bases)
  Sauv.base(chemin("Bases"), ic("Analyse.rémunérations"), nom.base.analyse)

#'
#'[Lien vers la base de données](Bases/`r nom.base.analyse`.csv) d'analyse des rémunérations pour `r année`
#'
#'# 
#'

########### Analyse dynamique ########################
#'
#'# 4. Rémunérations nettes : évolutions sur la période `r début.période.sous.revue` - `r fin.période.sous.revue` 
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

hist(positive(moyenne.rémunération.annuelle.sur.période)/1000,
     xlab = paste0("Sur la période ",début.période.sous.revue,"-",fin.période.sous.revue," en milliers d'euros"),
     ylab = "Effectif",
     main = "Rémunération nette moyenne",
     col = "blue",
     nclass = 100)
#'  

hist(moyenne.rémunération.annuelle.sur.période[moyenne.rémunération.annuelle.sur.période >0 & (statut == "TITULAIRE"  || statut == "STAGIAIRE")]/1000,
xlab = paste0("Sur la période ",début.période.sous.revue,"-",fin.période.sous.revue," en milliers d'euros"),
ylab = "Effectif",
main = "Rémunération nette moyenne des fonctionnaires",
col = "blue",
nclass = 100)

#'
#'[Lien vers la base de données](Bases/Analyse.variations.synthèse.csv)
#'
#'**Nota:** La rémunération nette perçue est rapportée au cumul des jours d'activité.  

Analyse.variations.synthèse.filtrée <- na.omit(Analyse.variations.synthèse[ nb.jours.exercice.début > seuil.troncature 
                                                                          & nb.jours.exercice.sortie   > seuil.troncature
                                                                        #  &  statut %in% c("TITULAIRE", "STAGIAIRE")
                                                                          &  statut !=  "AUTRE_STATUT"
                                                                              , c(ic("rémunération.début"),
                                                                                  ic("rémunération.sortie"),
                                                                                  ic("moyenne.rémunération.annuelle.sur.période"),
                                                                                  ic("variation.rémunération.jour"),
                                                                                  ic("variation.moyenne.rémunération.jour"), 
                                                                                  "plus.2.ans",
                                                                                  étiquette.matricule)])

Analyse.variations.synthèse.filtrée.plus.2.ans  <- Analyse.variations.synthèse.filtrée[Analyse.variations.synthèse.filtrée$plus.2.ans, ]
Analyse.variations.synthèse.filtrée.moins.2.ans <- Analyse.variations.synthèse.filtrée[! Analyse.variations.synthèse.filtrée$plus.2.ans, ]


detach(Analyse.variations.synthèse)
#'  
#'## 4.2 Evolutions des rémunérations nettes sur la période `r début.période.sous.revue` - `r fin.période.sous.revue` 
#'
#'### 4.2.1 Ensemble des personnels fonctionnaires et non titulaires
#'

#'
f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[Analyse.variations.par.exercice$Année == x, "Montant.net"])/ 1000, big.mark = " ", digits = 5, format = "fg")

Tableau.vertical(c(étiquette.année, "Rémunération nette totale (k&euro;)"), 
                 début.période.sous.revue:fin.période.sous.revue, 
                 f)


#'
#'[Lien vers la base de données](Bases/Analyse.variations.par.exercice.csv)
#'
#'######   
#'

Résumé(   c("Première année",
            "Dernière année",
            "Moyenne sur la période <br>d'activité"),
             Analyse.variations.synthèse.filtrée[1:3])

#'

Résumé(   c("Variation sur la période <br>d'activité",
            "Variation annuelle moyenne"),
          Analyse.variations.synthèse.filtrée[4:5])

#'
#'**Effectif : `r nrow(Analyse.variations.synthèse.filtrée[étiquette.matricule])`**     
#'
#'[Lien vers la base de données](Bases/Analyse.variations.synthèse.filtrée.csv)
#'
#'### 4.2.2 Personnels fonctionnaires et non titulaires en place
#'

hist(Analyse.variations.synthèse.filtrée.plus.2.ans$variation.moyenne.rémunération.jour,
     xlab ="Variation annuelle moyenne en %",
     las = 1,
     xlim = c(-5,30),
     ylab ="Effectifs",
     main ="Rémunération nette des personnels en place",
     col ="blue",
     nclass = 200)

#'
#'

f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[
                               Analyse.variations.par.exercice$Année == x & Analyse.variations.par.exercice$plus.2.ans,
                               "Montant.net"])/ 1000, big.mark = " ", digits = 5, format = "fg")

Tableau.vertical(c(étiquette.année, "Rémunération nette totale <br>des agents en place (k&euro;)"), 
                 début.période.sous.revue:fin.période.sous.revue, 
                 f)


#'

Résumé(   c("Première année",
            "Dernière année",
            "Moyenne sur la période <br>d'activité"),
          Analyse.variations.synthèse.filtrée.plus.2.ans[1:3])

#'

Résumé(   c("Variation sur la période <br>d'activité",
            "Variation annuelle moyenne"),
          Analyse.variations.synthèse.filtrée.plus.2.ans[4:5])
#'     
#'**Effectif :** `r nrow(Analyse.variations.synthèse.filtrée.plus.2.ans[étiquette.matricule])`    
#'     
#'[Lien vers la base de données](Bases/Analyse.variations.synthèse.filtrée.plus.2.ans.csv)
#'     
#'**Nota**    
#'Personnels en place : en fonction au moins 730 jours sur la période `r début.période.sous.revue` à `r fin.période.sous.revue`     
#'
#'######   
#'
#'### 4.2.3 Personnels fonctionnaires et non titulaires en fonction moins de deux ans
#'
#'
hist(Analyse.variations.synthèse.filtrée.moins.2.ans$variation.moyenne.rémunération.jour,
     xlab ="Variation annuelle moyenne en %",
     xlim = c(-10,30),
     las = 1,
     ylab ="Effectifs",
     main ="Rémunération nette des personnels en fonction moins de deux ans",
     col ="turquoise",
     nclass = 100
)

#'
#'##            
#'

f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[
  Analyse.variations.par.exercice$Année == x & ! Analyse.variations.par.exercice$plus.2.ans,
  "Montant.net"])/ 1000, big.mark = " ", digits = 5, format = "fg")

Tableau.vertical(c(étiquette.année, "Rémunération nette totale <br>des agents en fonction moins de deux ans (k&euro;)"), 
                 début.période.sous.revue:fin.période.sous.revue, 
                 f)

#'
#'

Résumé(   c("Première année",
            "Dernière année",
            "Moyenne sur la période <br>d'activité"),
          Analyse.variations.synthèse.filtrée.moins.2.ans[1:3])

#'

Résumé(   c("Variation sur la période <br>d'activité",
            "Variation annuelle moyenne"),
          Analyse.variations.synthèse.filtrée.moins.2.ans[4:5])
#'
#'     
#'**Effectif :** `r nrow(Analyse.variations.synthèse.filtrée.moins.2.ans[étiquette.matricule])`    
#'     

#'
########### Tests statutaires ########################
#'
#'[Lien vers la base de données](Bases/Analyse.variations.synthèse.filtrée.moins.2.ans.csv)
#'
#'**Nota:** Au 4.2 seuls sont pris en compte les personnels en fonction au moins 100 jours la première et la dernière année    
#'Ne sont pas pris en compte les élus, stagiaires et apprentis ou assimilés.    
#'
#'Les résultats sont exprimés en euros.
#'
#'# 5. Tests réglementaires
#'## 5.1 Contrôle des heures supplémentaires, des NBI et primes informatiques
#'

attach(Bulletins.paie.Lignes.paie, warn.conflicts = FALSE)

NBI.aux.non.titulaires <- Bulletins.paie.Lignes.paie[ ! Statut %in% c("TITULAIRE","STAGIAIRE") 
                                                      & as.character(Code) %in% codes.NBI,
                                                      c(étiquette.matricule,
                                                        "Statut",
                                                        "Code",
                                                        étiquette.libellé,
                                                        "Mois",
                                                        étiquette.montant)]

nombre.Lignes.paie.NBI.nontit <- nrow(NBI.aux.non.titulaires)

#/* Prime de fonctions informatiques : pas dans la base de VLB
# on cherche la chaine de char. "INFO" dans les libellés de primes

# variante : filtre <- regexpr(".*(INFO|PFI|P.F.I).*", toupper(Bulletins.paie.Lignes.paie$Libellé)) et regmatches(Bulletins.paie.Lignes.paie$Libellé, filtre)*/

filtre<-grep(".*(INFO|PFI|P.F.I).*", Libellé)

personnels.prime.informatique <- Bulletins.paie.Lignes.paie[ filtre,
                                                             c(étiquette.matricule,
                                                               "Statut",
                                                               "Code",
                                                               étiquette.libellé,
                                                               étiquette.montant)]

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

#'
#'[Lien vers la base de données NBI aux non titulaires](Bases/NBI.aux.non.titulaires.csv)  
#'[Lien vers la base de données Primes informatiques](Bases/personnels.prime.informatique.csv)  
#'   
#'**Nota :**  
#'NBI: nouvelle bonification indiciaire  
#'PFI: prime de fonctions informatiques  
#'    
#'## 5.2 Contrôle des vacations pour les fonctionnaires  

# Vacations et statut de fonctionnaire

lignes.fonctionnaires.et.vacations <- Bulletins.paie.Lignes.paie[ Statut %in% c("TITULAIRE", "STAGIAIRE")
                                                                  & Code %in% codes.paiement[codes.paiement[ic("Type.rémunération")] == "VACATIONS","Code.rubrique"],
                                                                                             c(étiquette.matricule,
                                                                                               "Statut",
                                                                                               "Code",
                                                                                               étiquette.libellé,
                                                                                               étiquette.montant)]
                                                                  
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
#'[Lien vers la base de données Matricules des FEV](Bases/matricules.fonctionnaires.et.vacations.csv)  
#'[Lien vers la base de données Lignes de vacations de FEV](Bases/lignes.fonctionnaires.et.vacations.csv)  
#'   
#'**Nota:**  
#'FEV : fonctionnaire effectuant des vacations
#'
#'## 5.3 Contrôles sur les cumuls traitement indiciaire, indemnités et vacations des contractuels

# Vacations et régime indemnitaire

lignes.contractuels.et.vacations <- Bulletins.paie.Lignes.paie[ ! Statut %in% c("TITULAIRE", "STAGIAIRE")  & Code %in% codes.paiement[codes.paiement$Type.rémunération == "VACATIONS","Code.rubrique"], c(étiquette.matricule, "Code", étiquette.libellé, étiquette.montant)]
matricules.contractuels.et.vacations <- unique(lignes.contractuels.et.vacations$Matricule)
nombre.contractuels.et.vacations <- length(matricules.contractuels.et.vacations)

RI.et.vacations <- Bulletins.paie.Lignes.paie[ Matricule %in% matricules.contractuels.et.vacations & Code %in% codes.paiement[codes.paiement$Type.rémunération == "INDEMNITAIRE.OU.CONTRACTUEL","Code.rubrique"], c(étiquette.matricule, "Statut", "Code", étiquette.libellé, étiquette.montant)]


# Vacations et indiciaire

traitement.et.vacations <- Bulletins.paie.Lignes.paie[ Matricule %in% matricules.contractuels.et.vacations & Code %in% codes.paiement[codes.paiement$Type.rémunération == "TRAITEMENT","Code.rubrique"], c(étiquette.matricule, "Statut", "Code", étiquette.libellé, étiquette.montant)]

nombre.Lignes.paie.contractuels.et.vacations <- nrow(lignes.contractuels.et.vacations)
nombre.Lignes.paie.RI.et.vacations <- nrow(RI.et.vacations)
nombre.Lignes.paie.traitement.et.vacations <- nrow(traitement.et.vacations)

#'

#'**Contractuels effectuant des vacations (CEV)**  
#'  
Tableau(c("Nombre de CEV",
          "Nombre de lignes",
          "Nombre de lignes indemnitaires",
          "Nombre de lignes de traitement"),
          nombre.contractuels.et.vacations,
          nombre.Lignes.paie.contractuels.et.vacations,
          nombre.Lignes.paie.RI.et.vacations,
          nombre.Lignes.paie.traitement.et.vacations)

#'
#'[Lien vers la base de données Matricules des CEV](Bases/matricules.contractuels.et.vacations.csv)  
#'[Lien vers la base de données Lignes de paie de CEV](Bases/RI.et.vacations.csv)  
#'[Lien vers la base de données Lignes de traitement indiciaire pour CEV](Bases/traitement.et.vacations.csv)  
#'    
#'     
#'     
#'## 5.4 Contrôle sur les indemnités IAT et IFTS

#IAT et IFTS

filtre.iat<-grep(".*(IAT|I.A.T|.*Adm.*Tech).*", Libellé, ignore.case = TRUE)
filtre.ifts<-grep(".*(IFTS|I.F.T.S|.*FORF.*TRAV.*SUPP).*", Libellé, ignore.case = TRUE)
codes.ifts <- unique(Bulletins.paie.Lignes.paie[filtre.ifts, "Code"])

personnels.iat.ifts <- intersect(as.character(Bulletins.paie.Lignes.paie[ filtre.iat, c(étiquette.matricule)]),
                                 as.character(Bulletins.paie.Lignes.paie[ filtre.ifts, c(étiquette.matricule)]))

nombre.personnels.iat.ifts <- length(personnels.iat.ifts)

#'

Tableau(c("Codes IFTS", "Nombre de personnels percevant IAT et IFTS"), paste0(codes.ifts, collapse = " "), nombre.personnels.iat.ifts)

#'
#'[Codes IFTS retenus](Bases/codes.ifts.csv)    
#'[Lien vers la base de données](Bases/personnels.iat.ifts.csv)    
#'
#'### Contrôle sur les IFTS pour catégories B et contractuels

#IFTS et IB >= 380 (IM >= 350)

df1 <- Bulletins.paie.Lignes.paie[ Indice < 350, c(étiquette.matricule)]
df1 <- df1[!duplicated(df1)]

df2 <- Bulletins.paie.Lignes.paie[ filtre.ifts, c(étiquette.matricule)]
df2 <- df2[!duplicated(df2)]

df3 <- intersect(df1,df2)

lignes.ifts.anormales <- Bulletins.paie.Lignes.paie[Matricule %in% df3 & Code %in% codes.ifts & (Indice < 380 ), c(étiquette.matricule, "Statut", "Code", étiquette.libellé, "Indice", étiquette.montant)]
nombre.lignes.ifts.anormales <- nrow(lignes.ifts.anormales)

rm(df1, df2, df3)
# IFTS et non tit

ifts.et.contractuel <- Bulletins.paie.Lignes.paie[Code %in% codes.ifts & ! Statut %in% c("TITULAIRE", "STAGIAIRE"), c(étiquette.matricule, "Statut", "Code", étiquette.libellé, "Indice", étiquette.montant)]
nombres.lignes.ifts.et.contractuel <- nrow(ifts.et.contractuel)

#'

Tableau(c("Nombre de contractuels percevant des IFTS", "Nombre de lignes IFTS pour IB < 380"), nombres.lignes.ifts.et.contractuel, nombre.lignes.ifts.anormales)

#'
#'[Lien vers la base de données Lignes IFTS pour contractuels](Bases/ifts.et.contractuel.csv)  
#'[Lien vers la base de données Lignes IFTS pour IB < 380](Bases/lignes.ifts.anormales.csv)  
#'       
#'**Nota:**  
#'IB < 380 : fonctionnaire percevant un indice brut inférieur à 380  
#'
#'######  
#'
#'## 5.5 Contrôle sur les heures supplémentaires

HS.sup.25 <- Bulletins.paie.Lignes.paie[Heures.Sup. >= 25 , c(étiquette.matricule, étiquette.année, "Mois", "Statut", "Heures.Sup.", "Brut")]
nombre.Lignes.paie.HS.sup.25 <- nrow(HS.sup.25)

# with(Base2,
#      ihts.anormales <<- Base2[! Code.catégorie %in% c("B", "C") & substr(Code,1,2) %in% c("19") & ! grepl(" ENS", Libellé), c(étiquette.matricule, "Code", étiquette.libellé, étiquette.montant, "Code.catégorie")]
# )

ihts.anormales <- data.frame(NULL)
nombre.ihts.anormales <- nrow(ihts.anormales)
#'
 
Tableau(c("Nombre de lignes HS en excès", "Nombre de lignes IHTS anormales"), nombre.Lignes.paie.HS.sup.25, nombre.ihts.anormales)

#'
#'[Lien vers la base de données Heures suplémentaires en excès](Bases/HS.sup.25.csv)  
#'[Lien vers la base de données IHTS anormales](Bases/ihts.anormales.csv)  
#'       
#'**Nota:**  
#'HS en excès : au-delà de 25 heures par mois  
#'IHTS anormales : non attribuées à des fonctionnaires de catégorie B ou C.  
#'
#'#     
#'   
#'## 5.6 Contrôle sur les indemnités des élus
#'     

matricules.à.identifier <- unique(Bulletins.paie[, c(étiquette.année, "Service", "Grade", "Nom",  étiquette.matricule)])

Catégorie <- character(length = nrow(matricules.à.identifier))
matricules.à.identifier <- cbind(matricules.à.identifier, Catégorie)
matricules.à.identifier <- matricules.à.identifier[order(matricules.à.identifier$Matricule, matricules.à.identifier$Année),]

rémunérations.élus <- Analyse.rémunérations[ Analyse.rémunérations$indemnités.élu > 0,
                                             c("Matricule",
                                               ic("indemnités.élu"),
                                               ic("autres.rémunérations"),
                                               ic("total.rémunérations")) ]

rémunérations.élus <- merge(unique(matricules.à.identifier[c("Nom",  étiquette.matricule)]),
                            rémunérations.élus,
                            all.y = TRUE,
                            all.x = FALSE)

names(rémunérations.élus) <- c("Nom", "Matricule", "Indemnités d'élu (€)", "Autres rémunérations (€)", "Total (€)")
#'
kable(rémunérations.élus, row.names = FALSE)
#'     

Sauv.base(chemin("Bases"), ic("matricules.à.identifier"), fichier.personnels)

#'[Lien vers la base de données Rémunérations des élus](Bases/rémunérations.élus.csv)  
#'     

#'# Annexe

#'## Liens complémentaires
#'     
#'[Lien vers la base de données fusionnées des bulletins et lignes de paie](Bases/Bulletins.paie.Lignes.paie.csv)
#'      
#'[Lien vers le tableau des codes de paiement](Bases/codes.paiement.csv)
#'
#'[Lien vers le fichier des personnels](Bases/Catégories des personnels.csv)
#'

with( Lignes.paie,
      
    codes.paiement <<- unique(Lignes.paie[  Montant > 0 
                                              & nchar(as.character(Code)) == 4 
                                              & as.numeric(substr(Code, 1,1)) < 5,
                                              c("Code", étiquette.libellé)]))

codes.paiement <- cbind(codes.paiement[order(substr(codes.paiement$Code,1,3)), ],
                        étiquette.type.rémunération = character(nrow(codes.paiement)))

#'---
#'   
#'# Tableau des codes de paiement
#'
#'##  Renseigner le type de rémunération
#'
#'Utiliser les codes : TRAITEMENT, INDEMNITAIRE.OU.CONTRACTUEL, ELU, AUTRES  
#'  
#'  
kable(codes.paiement, row.names = FALSE)
#'                             
#'
#'<!-- BREAK -->
#'
#'                             
#'# Tableau des personnels : renseigner la catégorie  
#'
#'Utiliser les codes : A, B, C, ELU, AUTRES    
#'
#'En cas de changement de catégorie en cours de période, utiliser la catégorie AUTRES   
#'Cela peut conduire à modifier manuellement le fichier Catégories des personnels.csv
#'
kable(matricules.à.identifier, row.names = FALSE)
#'

detach(Bulletins.paie.Lignes.paie)

#/* ------------------------------------------------------------------------------------------------------------------*/
#  Sauvegardes : enlever les commentaires en mode opérationnel
##


if (sauvegarder.bases) 
  sauv.bases("Bases",
    "Analyse.variations.par.exercice",
    "Analyse.variations.synthèse",
    "Analyse.variations.synthèse.filtrée",
    "Analyse.variations.synthèse.filtrée.plus.2.ans",
    "Analyse.variations.synthèse.filtrée.moins.2.ans",
    "Bulletins.paie.nir.total.hors.élus",
    "Bulletins.paie.nir.fonctionnaires",
    "Bulletins.paie.Lignes.paie", 
    "NBI.aux.non.titulaires",
    "RI.et.vacations",
    "HS.sup.25",
    "ihts.anormales",
    "personnels.prime.informatique",
    "liste.matricules.fonctionnaires",
    "lignes.contractuels.et.vacations",
    "lignes.fonctionnaires.et.vacations",
    "lignes.ifts.anormales",
    "personnels.iat.ifts",
    "codes.ifts",
    "ifts.et.contractuel",
    "traitement.et.vacations",
    "matricules.contractuels.et.vacations",
    "matricules.fonctionnaires.et.vacations",
    "rémunérations.élus",
    "codes.paiement")




