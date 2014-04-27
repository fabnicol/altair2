#'---
#'title: "RH Sierg"
#'author: "Fabrice Nicol"
#'date: false
#'output:
#'html_document:
#'css: style.css
#'---



#+ echo=FALSE
exercice <- 2012


#'Exercice `r exercice`  
#'`r format(Sys.Date(), "%a %d %b %Y")`  

#+ echo=FALSE

library(compiler)
library(knitr)

options(warn=-1, verbose=FALSE, OutDec=",")
compilerOptions <- setCompilerOptions(suppressAll=TRUE)
JITlevel <- enableJIT(3)

#/*---------------------------------------------------------------------------------------------------------------------
#  Chemins et variables globales du programme
##

# Cette section pourra être modifiée en entrée dans d'autres contextes
# Matricule, Codes, Avantages en nature */

chemin.dossier <- "~/Dev/altair2/Reports/Sierg"
champ.détection.1 <- "Matricule"
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

ldp <- paste0(paste0(exercice,"-Lignes de paye-"),1:10,".csv")

#/* Bulletins de paie */

bdp <- paste0(exercice, "-Bulletins de paye-1.csv")

codes.NBI <- c("1012", "101B", "101M", "4652", "4672")

source(file.path(chemin.dossier, "bibliothèque.fonctions.paie.R"), encoding="UTF-8-BOM")

#/* Programme principal
##
#  Bases
##
# Lignes de paie  */

ldp <- ldp[file.exists(chemin(ldp))]

Ldp<-Read.csv(ldp)

#/* Equivaut à :
# ldp1 <- read.csv2(chemin.ldp1,  blank.lines.skip=TRUE, skip=1)
# ...
# ldp <- rbind(ldp1,ldp2,...)
# Bulletin de paie  */

Bdp <- read.csv.skip(bdp)

#Matricule.catégorie <- read.csv.skip(matricule.catégorie)
Code.prime          <- read.csv.skip(code.prime)
#Matricule.avantage  <- read.csv.skip(matricule.avantage)

#suppression des colonnes Nom Prénom redondantes

#Matricule.avantage  <- selectionner.cle.matricule(Matricule.avantage, Matricule.catégorie) 
Bdp                 <- selectionner.cle.matricule.mois(Bdp, Ldp)

if (!setequal(intersect(names(Ldp), names(Bdp)), c("Mois", "Matricule")))
  stop("L'appariement ne peut se faire par les clés Matricule et Mois")


liste.matricules.fonctionnaires <- unique(Bdp[Bdp$Mois == 12 & Bdp$Statut %in% c("TITULAIRE", "STAGIAIRE"), "Matricule"])
liste.matricules.élus   <- unique(Bdp[Bdp$Mois == 12 &  Bdp[champ.détection.élus] == libellé.élus, "Matricule"])

Bdp.nir.total.hors.élus <- Bdp[Bdp$Mois == 12 & ! Bdp$Matricule %in% liste.matricules.élus, champ.nir]

Bdp.nir.fonctionnaires  <- Bdp[Bdp$Mois == 12 &   Bdp$Matricule %in% liste.matricules.fonctionnaires, champ.nir]

années.fonctionnaires   <- exercice-1-(as.numeric(substr(as.character(Bdp.nir.fonctionnaires), 2, 3)) + 1900)

années.total.hors.élus  <- exercice-1-(as.numeric(substr(as.character(Bdp.nir.total.hors.élus), 2, 3)) + 1900)

#'# 1. Statistiques de population
#'**Ensemble des personnels non élus**  
#+ echo=FALSE, fig.retina=2

hist(années.total.hors.élus,
     xlab="Âge",
     xlim=c(18, 75),
     ylab="Effectif",
     main="Pyramide des âges",
     col="blue",
     nclass=50)

#'
#+ echo=FALSE, results='asis'

Résumé(années.total.hors.élus, "Âge des personnels", align='c')

#'Effectif total: `r length(années.total.hors.élus)`  
#'
#'**Ensemble des fonctionnaires stagiaires et titulaires**  

#'  
#+ echo=FALSE, fig.retina=2

hist(années.fonctionnaires,
     xlab="Âge",
     xlim=c(18,68),
     ylab="Effectif",
     main="Pyramide des âges des fonctionnaires",
     col="navy",
     nclass=50)

#'
#+ echo=FALSE, results='asis'

Résumé(années.fonctionnaires, "Âge des fonctionnaires", align='c')

#'   
#'Effectif total: `r length(années.fonctionnaires)`   
#'  
#'**Nota :** Dans les statistiques de cette section, les élus ne sont pas pris en compte.
#'Pyramides au 31 décembre `r exercice`  
#+ echo=FALSE

#fusion matricule | avantage | catégorie par Matricule

Bdp.ldp <- merge(Bdp, Ldp)

library(plyr)

Bdp.ldp2 <- mutate(Bdp.ldp,
       
        montant.traitement.indiciaire = Montant*(Code %in% Code.prime[Code.prime$Type.rémunération == "TRAITEMENT","Code.rubrique"]),
        montant.primes = Montant*(Code %in% Code.prime[Code.prime$Type.rémunération == "INDEMNITAIRE.OU.CONTRACTUEL","Code.rubrique"]),
        montant.autres.rémunérations = Montant*(Code %in% Code.prime[Code.prime$Type.rémunération == "AUTRES","Code.rubrique"]),
        warn=-1
       )


Analyse.rémunérations <- ddply(Bdp.ldp2,
                             c("Matricule", "Service"),
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

#'# 2. Rémunérations 
#'## 2.1 Fonctionnaires titulaires et stagiaires
#+ echo=FALSE, fig.retina=2, fig.width=7.1

hist(total.rémunérations[traitement.indiciaire > 0 ]/1000,
   xlab="Distribution de la rémunération en k€ \n hors politique familiale, indemnités journalières et remboursements",
   ylab="Effectif",
   xlim=c(0, 120),
   main="Rémunération annuelle des fonctionnaires",
   col="blue",
   nclass=50
)

#'  
#+ echo=FALSE, fig.retina=2, fig.width=7.1

hist(rémunération.contractuelle.ou.indemnitaire[traitement.indiciaire > 0 ]/1000,
     xlab="Distribution des attributions indemnitaires en k€\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 70),
     main="Rémunération indemnitaire annuelle\ndes fonctionnaires",
     col="blue",
     nclass=50
)

#'    
#+ echo=FALSE, fig.retina=2, fig.width=7.1

hist(part.rémunération.contractuelle.ou.indemnitaire[traitement.indiciaire > 0 & part.rémunération.contractuelle.ou.indemnitaire >0],
     xlab="Pourcentage des indemnités dans la rémunération\n hors politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     main="Distribution de la part indemnitaire\n de la rémunération annuelle des fonctionnaires",
     xlim=c(0,60),     
     col="blue",
     nclass=30
)

#/* La moyenne est tirée vers le haut par les outlyers */


masse.indemnitaire            <- sum(rémunération.contractuelle.ou.indemnitaire[ traitement.indiciaire > 0])
masse.indiciaire              <- sum(traitement.indiciaire)
masse.rémunérations.brutes    <- sum(total.rémunérations)
ratio.global.masse.indemnitaire  <- masse.indemnitaire/(masse.indiciaire+masse.indemnitaire)*100

#'### Masses indemnitaires
#'Les résultats sont exprimés en euros.
#+ echo=FALSE, results='asis'

 Tableau(c("Masse indiciaire", "Masse indemnitaire"),
                  masse.indiciaire, masse.indemnitaire)
  
#'  
#+ echo=FALSE, results='asis'

 Tableau(c("Masse des rémunérations brutes", "Part de la masse indemnitaire"),
                   masse.rémunérations.brutes, ratio.global.masse.indemnitaire)


#'### Statistiques de position
#+ echo=FALSE, results='asis'

#/*print(xtable(Stats.fonctionnaires), type="html", include.rownames=FALSE)*/

 Résumé(Analyse.rémunérations[traitement.indiciaire > 0, 
                              c("traitement.indiciaire",
                                "rémunération.contractuelle.ou.indemnitaire",
                                "autres.rémunérations") ],
                               c("Traitement indiciaire",
                                 "Rémunération contractuelle ou indemnitaire",
                                 "Autres rémunérations"))

#'
#+ echo=FALSE, results='asis'

 Résumé(Analyse.rémunérations[traitement.indiciaire > 0, 
                              c( "total.rémunérations",
                                 "part.rémunération.contractuelle.ou.indemnitaire") ],
                              c("Total rémunérations",
                                "Part de la rémunération contractuelle ou indemnitaire"))


#'## 2.2 Contractuels, vacataires et stagiaires inclus
#+ echo=FALSE, fig.retina=2, fig.width=7.1

hist(total.rémunérations[traitement.indiciaire == 0 & total.rémunérations > 1000]/1000,
   xlab="Distribution de la rémunération en k€ (supérieure à 1000 €)\n hors politique familiale, indemnités journalières et remboursements",
   ylab="Effectif",
   xlim=c(0, 40),
   main="Rémunération annuelle totale des contractuels",
   col="red")

#'
#+ echo=FALSE, fig.retina=2, fig.width=7.1

hist(autres.rémunérations[autres.rémunérations >0],
     xlab="Distribution des autres rémunérations annuelles en € :\n politique familiale, indemnités journalières et remboursements",
     ylab="Effectif",
     xlim=c(0, 5000),
     main="Autres rémunérations",
     col="grey")

#'### Statistiques de position
#+ echo=FALSE, results='asis'

  Résumé(Analyse.rémunérations[traitement.indiciaire == 0, 
                                                    c("rémunération.contractuelle.ou.indemnitaire", "autres.rémunérations")],
                               c("Rémunération contractuelle ou indemnitaire",
                                  "Autres rémunérations"))

#'
#+ echo=FALSE, results='asis'

  Résumé(Analyse.rémunérations[traitement.indiciaire == 0, 
                                                   "total.rémunérations"],
                                                   "Total rémunérations")
detach(Analyse.rémunérations)

#'Les résultats sont exprimés en euros.
#'
#'# 3. Tests réglementaires
#'### Contrôle des heures supplémentaires, des NBI et primes informatiques
#'
#+ echo=FALSE

attach(Bdp.ldp, warn.conflicts=FALSE)

NBI.aux.non.titulaires <- Bdp.ldp[ ! Statut %in% c("TITULAIRE","STAGIAIRE") & as.character(Code) %in% codes.NBI, c("Matricule", "Statut", "Code", "Libellé", "Mois", "Montant")]

nombre.de.ldp.NBI.nontit <- nrow(NBI.aux.non.titulaires)

HS.sup.25 <- Bdp.ldp[Heures.Sup. >= 25 , c("Matricule", "Statut", "Mois", "Heures.Sup.", "Brut")]

nombre.de.ldp.HS.sup.25 <- nrow(HS.sup.25)

#/* Prime de fonctions informatiques : pas dans la base de VLB
# on cherche la chaine de char. "INFO" dans les libellés de primes

# variante : filtre <- regexpr(".*(INFO|PFI|P.F.I).*", toupper(Bdp.ldp$Libellé)) et regmatches(Bdp.ldp$Libellé, filtre)*/

filtre<-grep(".*(INFO|PFI|P.F.I).*", Libellé)

personnels.prime.informatique <- Bdp.ldp[ filtre, c("Matricule", "Statut", "Code", "Libellé", "Montant")]

primes.informatiques.potentielles<-unique(Libellé[filtre])

if  (length(primes.informatiques.potentielles) == 0) 
  primes.informatiques.potentielles <- "aucune"
  
nombre.de.personnels.pfi <- nrow(personnels.prime.informatique)


#'Primes informatiques potentielles : `r primes.informatiques.potentielles`
#+ echo=FALSE, results='asis'

Tableau(
  c("Nombre de lignes NBI pour non titulaires",
    "Nombre de lignes HS >= 25",
    "Nombre de bénéficiaires de PFI"),
  nombre.de.ldp.NBI.nontit, 
  nombre.de.ldp.HS.sup.25,
  nombre.de.personnels.pfi)

#+ echo=FALSE
detach(Bdp.ldp)

#'### Nota :
#'NBI: nouvelle bonification indiciaire  
#'HS: heures supplémentaires  
#'PFI: prime de fonctions informatiques  


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

