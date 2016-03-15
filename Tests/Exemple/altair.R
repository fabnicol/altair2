      
#'---    
#'title: false    
#'author: false    
#'date: false    
#'output:   
#'html_document:    
#'css: style.css
#'---     
#'   
#'![Image_Altair](Altair.png)
#'   
#'   
#'# Démonstrateur Altaïr version 16.03 

#+ echo = FALSE, warning = TRUE, message = FALSE

# comportement global du programme

# Lorsque l'on n'a que une ou deux années, mettre étudier.variations à FALSE
# Lorsque l'on n'étudie pas une base Xémélios, mettre étudier.tests.statutaires à FALSE

#+ début

library(compiler)
invisible(setCompilerOptions(suppressAll = TRUE))
invisible(enableJIT(3))

options(warn = -1, verbose = FALSE, OutDec = ",", datatable.verbose = FALSE, datatable.integer64 = "numeric")

encodage.code.source <- "ISO-8859-1"

currentDir              <- getwd()
générer.rapport         <- ! grepl("altair", basename(currentDir), ignore.case = TRUE) 


# dans cet ordre

try(setwd("Tests/Exemple"), silent = TRUE)

source("syspaths.R", encoding = encodage.code.source)
source("prologue.R", encoding = encodage.code.source)

if (corriger.environnement.système) {
  
  invisible(Sys.setenv(PATH = paste0(Sys.getenv("PATH"), "c:\\Users\\Public\\Dev\\altair\\texlive\\miktex\\bin;")))
  
}


source(file.path(chemin.dossier, "bibliotheque.fonctions.paie.R"), encoding = encodage.code.source)
source("import.R", encoding = encodage.code.source)

#'
#'<p class = "centered"><b>Exercices `r paste(début.période.sous.revue, "à", fin.période.sous.revue)` </b></p>
#'<p class = "author">Fabrice Nicol</h1>
#'
#+ echo = FALSE
#'`r format(Sys.Date(), "%a %d %b %Y")`      
#'      
#'Période sous revue : `r début.période.sous.revue` - `r fin.période.sous.revue`    
#'Nombre d'exercices : `r durée.sous.revue`        
#'   

# Pour sauter une page en html (ou pdf converti de html, faire un h6 soit six dièses dans les Rmd seulement)  

#+ analyse-rémunérations

newpage()

source("analyse.rémunérations.R", encoding = encodage.code.source)

########### 1.1 Effectifs ########################

incrémenter.chapitre()

#'# `r chapitre`. Statistiques de population
#'
#'### `r chapitre`.1 Effectifs

#+ effectifs

message("Démographie...")

# Rappel Analyse.variations.par.exercice comprend uniquement les actifs non annexes non assist. mat., non vacataires, non élus.

library(altair)
tableau.effectifs <- effectifs(période, Bulletins.paie, Analyse.rémunérations, Analyse.variations)

#'  
#  
#'&nbsp;*Tableau `r incrément()`*   
#            
kable(tableau.effectifs, row.names = TRUE, align='c')

#'    
#'**Nota:**   
#'*(a) Nombre de matricules distincts ayant eu au moins un bulletin de paie dans l'année, en fonction ou non. Tous ces personnels ne sont pas en fonction : sont inclus des régularisations, des personnels hors position d'activité ou des ayants droit (reversion, etc.)*   
#'*(b) Titulaires ou stagiaires*   
#'*(c) Sur la base des libellés d'emploi et des libellés de lignes de paie. La détection peut être lacunaire*   
#'*(d) ETP  : Equivalent temps plein = rémunération . quotité*  
#'*(e) ETPT : Equivalent temps plein travaillé = ETP . 12/nombre de mois travaillés dans l'année*  
#'*(f) Personnes en place : présentes en N et N-1 avec la même quotité, postes actifs et non annexes uniquement.*     
#'*(g) Postes actifs et non annexes :* voir [Compléments méthodologiques](`r currentDir`/Docs/méthodologie.pdf)    
#'*&nbsp;&nbsp;&nbsp;Un poste actif est défini par au moins un bulletin de paie comportant un traitement positif pour un volume d'heures de travail mensuel non nul.*             
#'*&nbsp;&nbsp;&nbsp;Un poste non annexe est défini comme la conjonction de critères horaires et de revenu sur une année. La période minimale de référence est le mois.*   
#'*Les dix dernières lignes du tableau sont calculées en ne tenant pas compte des élus.*    

#+ durée-du-travail

cat("\nLa durée du travail prise en compte dans la base de données est de ", nb.heures.temps.complet, " h par mois.\n")  
if (nb.heures.temps.complet > 1.1 * 151.67 || nb.heures.temps.complet < 0.9 * 151.67)  {
  semaine.de.travail <<- nb.heures.temps.complet * 12 / 52
  
  cat("\nAttention !\nLe temps de travail hebdomadaire s'écarte significativement de la durée légale : ", 
      round(semaine.de.travail,1), " h par semaine.\n")
}

#'      
#'   
#'[Lien vers la base des effectifs](`r currentDir`/Bases/Effectifs/tableau.effectifs.csv)
#'
#'

#+ pyramides-des-âges

message("Statistiques de démographie réalisées.")

source("analyse.bulletins.R", encoding = encodage.code.source)

newpage()

########### 1.2 Pyramides ########################


#'### `r chapitre`.2 Pyramide des âges, ensemble des personnels

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

comparaison.nationale <<- TRUE
produire_pyramides(function() TRUE, "Pyramide des âges des personnels")
comparaison.nationale <<- FALSE

#'  
#'[Lien vers la base des âges - début de période](`r currentDir`/Bases/Effectifs/`r nom.fichier.avant`.csv)  
#'  
#'[Lien vers la base des âges - fin de période](`r currentDir`/Bases/Effectifs/`r nom.fichier.après`.csv)  
#' 


newpage()
   
########### 1.3 Pyramides fonctionnaires ########################

#'
#'### `r chapitre`.3 Pyramide des âges des fonctionnaires  

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Filtre_bulletins <- function() Bulletins.paie$Statut == "TITULAIRE" |  Bulletins.paie$Statut == "STAGIAIRE"

produire_pyramides(Filtre_bulletins, "Pyramide des âges des fonctionnaires")


#'  
#'[Lien vers la base des âges - début de période](`r currentDir`/Bases/Effectifs/`r nom.fichier.avant`.csv)  
#'  
#'[Lien vers la base des âges - fin de période](`r currentDir`/Bases/Effectifs/`r nom.fichier.après`.csv)  
#'   

newpage()

########### 1.4 Pyramides non Tit ########################

#'### `r chapitre`.4 Pyramide des âges, personnels non titulaires   

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Filtre_bulletins <- function() Bulletins.paie$Statut == "NON_TITULAIRE"

produire_pyramides(Filtre_bulletins, "Pyramide des âges des non titulaires")

#'  
#'[Lien vers la base des âges - début de période](`r currentDir`/Bases/Effectifs/`r nom.fichier.avant`.csv)  
#'  
#'[Lien vers la base des âges - fin de période](`r currentDir`/Bases/Effectifs/`r nom.fichier.après`.csv)  
#'   

newpage()

########### 1.6 Pyramides Autres statut ########################

#'### `r chapitre`.5 Pyramide des âges, autres statuts

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


Filtre_bulletins <- function() { return(Bulletins.paie$Statut != "TITULAIRE" 
                                 & Bulletins.paie$Statut != "NON_TITULAIRE" 
                                 & Bulletins.paie$Statut != "STAGIAIRE") }

produire_pyramides(Filtre_bulletins, "Pyramide des âges des autres personnels")


#'  
#'[Lien vers la base des âges - début de période](`r currentDir`/Bases/Effectifs/`r nom.fichier.avant`.csv)  
#'  
#'[Lien vers la base des âges - fin de période](`r currentDir`/Bases/Effectifs/`r nom.fichier.après`.csv)  
#' 

#'*Toutes les pyramides des âges sont établies au 31 décembre de l'année considérée.*   
#'*Les élus ne sont pas compris dans le périmètre statistique.*     
	   
newpage()

########### 1.6 Effectifs par durée ########################

#'### `r chapitre`.6 Effectifs des personnels par durée de service
#'
#'**Personnels en fonction (hors élus) des exercices `r début.période.sous.revue` à `r fin.période.sous.revue` inclus :**
#'

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

#+ effectifs-par-durée

Tableau(c("Plus de 2 ans",
          "Moins de 2 ans",
          "Moins d'un an",
          "Moins de six mois"),
        sum(Analyse.variations$plus.2.ans, na.rm=TRUE),
        sum(Analyse.variations$moins.2.ans, na.rm=TRUE),
        sum(Analyse.variations$moins.1.an, na.rm=TRUE),
        sum(Analyse.variations$moins.six.mois, na.rm=TRUE))


#'

if (nrow(Analyse.variations) > 0)
  qplot(factor(Année),
        data = Analyse.variations,
        geom = "bar",
        fill = factor(!Analyse.variations$plus.2.ans),
        main = "Evolutions entre " %+% début.période.sous.revue %+% " et " %+% fin.période.sous.revue,
        xlab = étiquette.année,
        ylab = "Effectif",
        asp = 1.4)        +
  scale_fill_discrete(name = "Personnels (non élus) en fonction",
                      breaks = c(TRUE, FALSE),
                      labels = c("Moins de deux ans", "Plus de deux ans"))



#'
#'**Effectifs (hors élus)**   
#+ tableau-effectifs

effectifs.var <- lapply(période,
                        function(x) {

                          E <- unique(Analyse.variations[Année == x , .(Matricule, plus.2.ans)], by=NULL)
                          F <- E[plus.2.ans == TRUE]
                          tot <- nrow(E)
                          plus.2.ans <- nrow(F)
                          résultat <- c(plus.2.ans, tot - plus.2.ans, tot)
                          rm(E, F, tot, plus.2.ans)
                          résultat
                        })

effectifs.var.locale <- lapply(effectifs.var, function(x) formatC(x, big.mark = " ", format="f", digits=0, decimal.mark=","))

tableau.effectifs.var <- as.data.frame(effectifs.var.locale, row.names = c("Plus de deux ans", "Moins de deux ans", "Total"))

names(tableau.effectifs.var) <- as.character(période)
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

kable(tableau.effectifs.var, row.names = TRUE, align='c')
#'
#'
#'**Nota :**
#'*Personnels en place : ayant servi au moins deux années consécutives pendant la période.*     
#'*Plus/moins de deux ans : plus/mois de 730 jours sur la période sous revue.*   
#'

colonnes.sélectionnées <- c("traitement.indiciaire",
                            "autres.rémunérations",
                            "rémunération.indemnitaire.imposable",
                            "rémunération.indemnitaire.imposable.eqtp",
                            "total.lignes.paie",
                            "Montant.brut.annuel",
                            "Montant.brut.annuel.eqtp",
                            "part.rémunération.indemnitaire",
                            "quotité.moyenne",
                            "Statut",
                            "Grade",
                            "Catégorie",
                            "Filtre_actif",
                            "Filtre_annexe",
                            clé.fusion)


########### Analyse statique des rémunérations (fichier Rmd) ########################
newpage()

#+ lancement-analyse-statique

invisible(lapply(années.analyse.statique, function(x) {
                 année <<- x
                 incrémenter.chapitre()
                 if (! générer.rapport) {
                   
                   source('analyse.statique.R', encoding = encodage.code.source) 
                   
                 } else {
                   if (setOSWindows)  {                 
                      cat(knit_child(text = readLines(file.path(chemin.dossier,'analyse.statique.Rmd'), encoding = encodage.code.source), quiet=TRUE), sep = '\n')
                   } else {
                     cat(knit_child(text = readLines(file.path(chemin.dossier,'analyse.statique.utf8.Rmd'), encoding = "UTF-8"), quiet=TRUE), sep = '\n')
                   }
                 }
               }))

#'  
#'[Lien vers la base des rémunérations](`r currentDir`/Bases/Remunerations/Analyse.rémunérations.csv)  
#'   

newpage()


########### Comparatif INSEE DGCL ###############################
#'   
#'## `r chapitre`.4 Comparaisons source INSEE/DGCL   
#'   
#'*Salaires annnuels moyens 2011 en EQTP (hors assistantes maternelles)*   

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

#+ comparaison-insee1

Tableau.vertical2(c("Agrégat (euros)", "Salaires bruts 2011", "Salaires bruts 2012", "Salaires bruts 2013"),
                  c("Ensemble", "Titulaires", "Autres salariés"),
                  12 * c(2159, 2223, 1903),
                  12 * c(2195, 2259, NA),
                  12 * c(2218, 2287, 2030))


#'**Eléments de la rémunération brute pour les titulaires de la FPT entre 2010 et 2012**      
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(
  c("Rém. annuelles", "2010", "Primes", "2011", "Primes", "2012", "Primes", "2013", "Primes"),
  c("Salaire brut",
    "Traitement brut",
    "Primes et rémunérations annexes",
    "y compris IR et SFT"),
  c(26305, 20350,	"", 5955),
  c("", "22,6 %", "", "" ),
  c(26660, 20562, "", 6098),
  c("", "22,9 %", "", "" ),
  c(12*2259, 12*1727, "", 12*532),
  c("", "23,6 %", "", "" ),
  c(12*2287, 12*1755, "", 12*532),
  c("", "23,6 %", "", "" ))
#'   
#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*   												
#'*Les primes sont cumulées au supplément familial de traitement (SFT) et à l'indemnité de résidence (IR). Le cumul est rapporté à la rémunération brute totale.*    
#'[Source INSEE](http://www.insee.fr/fr/ffc/ipweb/ip1486/ip1486.xls)    
#'[Source DGCL](http://www.fonction-publique.gouv.fr/files/files/statistiques/rapports_annuels/2012-2013/xls/Vue3_1_Remunerations.xls)    
#'[Source DGFIP PLF 2015](http://www.performance-publique.budget.gouv.fr/sites/performance_publique/files/farandole/ressources/2015/pap/pdf/jaunes/jaune2015_fonction_publique.pdf)   
#'   

incrémenter.chapitre()

newpage()

########### 4. Analyse dynamique des rémunérations ########################
#'
#'# `r chapitre`. Rémunérations nettes : évolutions sur la période `r début.période.sous.revue` - `r fin.période.sous.revue`    
#'
#'Nombre d'exercices: `r durée.sous.revue`   
#'  
#'**Les données présentées dans cette section sont toutes relatives à des rémunérations nettes en équivalent temps plein (EQTP)**   
#'Les élus, les vacataires et les assistantes maternelles ont été retirés de la population étudiée       
#'Seuls sont considérés les postes actifs et non annexes   
#'    
#'*Nota :*   
#'*EQTP = Equivalent temps plein  = 12 . moyenne du ratio rémunération / quotité*    

########### 4.1 Distribution de la rémunération nette moyenne ###########

#'    
#'## `r chapitre`.1 Distribution de la rémunération nette moyenne sur la période    
#+ remuneration-nette

Analyse.variations <- Analyse.variations[nb.jours > seuil.troncature
                                               & ! is.na(Montant.net.annuel.eqtp)
                                               & Montant.net.annuel.eqtp  > minimum.positif 
                                               & ! is.na(Statut)] 

attach(Analyse.variations)

temp <- positive(moyenne.rémunération.annuelle.sur.période) / 1000

if (longueur.non.na(temp) > 0)
  hist(temp,
       xlab = "Sur la période " %+% début.période.sous.revue %+% "-" %+% fin.période.sous.revue %+% " en milliers d'euros",
       ylab = "Effectif",
       main = "Rémunération nette moyenne",
       col = "blue",
       nclass = 200)

#'    
#+ fig.height=4.5   

temp <- na.omit(moyenne.rémunération.annuelle.sur.période[moyenne.rémunération.annuelle.sur.période > minimum.positif
                                                          & (statut == "TITULAIRE"  | statut == "STAGIAIRE")] / 1000)

if (longueur.non.na(temp) > 0)
  hist(temp,
       xlab = "Sur la période "%+% début.période.sous.revue %+% "-" %+% fin.période.sous.revue %+% " en milliers d'euros",
       ylab = "Effectif",
       main = "Rémunération nette moyenne des fonctionnaires",
       col = "blue",
       nclass = 200)



#'   
#'[Lien vers la base de données synthétique](`r currentDir`/Bases/Remunerations/Analyse.variations.par.exercice.csv)
#'[Lien vers la base de données détaillée par année](`r currentDir`/Bases/Remunerations/Analyse.variations.par.exercice.csv)  

########### 4.2  Evolutions des rémunérations nettes ###########

#'## `r chapitre`.2 Evolutions des rémunérations nettes sur la période `r début.période.sous.revue` - `r fin.période.sous.revue`   
#'
#'### `r chapitre`.2.1 Ensemble des personnels fonctionnaires et non titulaires (hors élus)
#'
#+ remuneration-nette-evolution

masse.salariale.nette <- rep(0, durée.sous.revue)

# sommation sur les matricules à année fixe 

#'    
#'**Salaire net moyen par tête (SMPT net) en EQTP, hors élus**         
#'       
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

#+ Salaire-moyen-par-tete    


smpt <- function(Filtre, type =  "smpt net") {
  
  S_net.eqtp <- Analyse.variations[Filtre() == TRUE,
                                   .(moy = weighted.mean(Montant.net.annuel.eqtp, quotité.moyenne, na.rm = TRUE)),
                                   by = "Année"]
  
  S_net.eqtp.100 <- Analyse.variations[Filtre() == TRUE & temps.complet == TRUE & permanent == TRUE,
                                       .(moy = weighted.mean(Montant.net.annuel.eqtp, quotité.moyenne, na.rm = TRUE)),
                                       by = "Année"]
  
 
  f <- function(x) prettyNum(S_net.eqtp[Année == x, moy],
                               big.mark = " ",
                               digits = 1,
                               format = "fg")
  
  g <- function(x) prettyNum(S_net.eqtp.100[Année == x, moy],
                             big.mark = " ",
                             digits = 1,
                             format = "fg")
  
  T <- Tableau.vertical(c(étiquette.année, type %+% " (euros)", type %+% " temps complet (euros)"),
                         if (type == "smpt net") période else période[2:durée.sous.revue],           # if...else pas ifelse (dim vecteur)
                         extra = "variation",
                         f,
                         g)
  
  if (T != "")  print(T)
  
  cat("\n\n")  
}

distribution_smpt <- function(Filtre) {
  
  print(Résumé(c(début.période.sous.revue, "Effectif",
                 début.période.sous.revue %+% " TC", "Effectif",
                 fin.période.sous.revue, "Effectif",
                 fin.période.sous.revue %+% " TC",  "Effectif"),
         list(
           Analyse.variations[Année == début.période.sous.revue
                              & Filtre() == TRUE,
                              .(Montant.net.annuel.eqtp, quotité.moyenne)],   
           Analyse.variations[Année == début.période.sous.revue
                              & Filtre() == TRUE
                              & permanent == TRUE
                              & temps.complet == TRUE,
                              .(Montant.net.annuel.eqtp, quotité.moyenne)],
           Analyse.variations[Année == fin.période.sous.revue 
                              & Filtre() == TRUE,
                              .(Montant.net.annuel.eqtp, quotité.moyenne)],
           Analyse.variations[Année == fin.période.sous.revue 
                              & Filtre() == TRUE
                              & permanent == TRUE
                              & temps.complet == TRUE,
                              .(Montant.net.annuel.eqtp, quotité.moyenne)]),
         extra = "length"))

# Pour des raisons très mal comprises, print est ici nécessaire alors qu'il ne l'est pas dans smpt() pour Tableau_vertical ;
# pourtant les deux fonctions sont basées sur kable()

}

Filtre_neutre <- function() TRUE

smpt(Filtre_neutre)

#'   
#+ Effet-de-noria-ensemble
source("noria.R", encoding = encodage.code.source)
noria()

#'   
#'**Effet de noria sur salaires nets et taux de remplacements**       
#'   
#'**Effet de noria** : *différence entre la rémunération annuelle des entrants à l'année N et des sortants à l'année N-1*.  
#'*Usuellement calculée sur les rémunérations brutes, ici sur les rémunérations nettes EQTP*  
#'*afin d'apprécier l'impact de cet effet sur l'évolution des rémunérations nette moyennes calculée au tableau précédent.*               
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    
#+ noria-tableau

# B

#'
#'*MS N-1 : masse salariale nette de l'année n-1.*   

#'**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         
#'**pour les salariés à temps complet**           
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

#+ premiere-derniere-annee

distribution_smpt(Filtre_neutre)

#'  
#'*Nota :*  La population retenue est constituée des agents qui ne font pas partie des `r 2*quantile.cut` centiles extrêmaux   
#'Les élus, vacataires et assistantes maternelles sont retirés du périmètre.   
#'TC :  personnels à temps complet sur toute l'année            
#'Seuls sont pris en compte les agents ayant connu au moins un mois actif et ayant eu, sur l'année, des rémunérations non annexes.  
#'[Compléments méthodologiques](`r currentDir`/Docs/méthodologie.pdf)     
#'      


#'**Comparaisons source INSEE/DGCL**   
#'
#'**Salaires annuels moyens 2011 à 2013 en EQTP (hors assistantes maternelles)**   
#'  
#'&nbsp;*Tableau `r incrément()`*       

#### INSEE/DGCL VARIATIONS  ####
#'  
#'  |  Agrégat (euros)| Salaires nets 2011 | Salaires nets 2012 | Salaires nets 2013 |      
#'  |-----------------|----------:|---------:|----------:|   
#'  |    Ensemble     |  21 876,0 | 22 176,0 |  22 224,0 |   
#'  |   Titulaires    |  22 632,0 | 22 920,0 |  22 920,0 |   
#'  | Autres salariés |  18 864,0 |  NA      |     NA    |   
#' 
#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			

#'   
#'**Distribution des salaires nets annuels en EQTP dans la fonction publique territoriale (2011-2013)**   
#' 
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    
# Remarque sur le formatage markdown: en raison d'un bug de knitr, utiliser un dièse + apostrophe
# suivi d'un seul blanc juste après la table.

#'  
#' | Décile \ euros   | 2011     | 2013   |   
#' |------------------|----------|--------|   
#' |    D1            | 15 288   | 15 600 |  
#' |    D2            | 16 512   | 16 860 |    
#' |    D3            | 17 508   | 17 844 |  
#' |    D4            | 18 480   | 18 816 |  
#' |    D5 (médiane)  | 19 632   | 19 908 |    
#' |    D6            | 21 012   | 21 300 |    
#' |    D7            | 22 860   | 23 160 |    
#' |    D8            | 25 596   | 25 956 |    
#' |    D9            | 30 876   | 31 272 |    
#' |    Moyenne       | 21 876   | 22 212 |    
#' 
 
#'**Distribution des salaires nets annuels en EQTP dans la fonction publique d'Etat (2011-2013)**   
#' 
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    
 
#'    
#' | Décile \ euros   | 2011     | 2013   |   
#' |------------------|----------|--------|
#' |    D1            | 17 496   | 18 012 |  
#' |    D2            | 20 916   | 21 348 |    
#' |    D3            | 23 052   | 23 376 |  
#' |    D4            | 24 912   | 25 248 |  
#' |    D5 (médiane)  | 26 832   | 27 120 |    
#' |    D6            | 28 944   | 29 220 |    
#' |    D7            | 31 632   | 31 968 |    
#' |    D8            | 35 592   | 35 964 |    
#' |    D9            | 42 456   | 42 780 |
#' | Moyenne          | 29 208   | 29 628 |  
#' 

            
#'**Distribution des salaires nets annuels en EQTP dans la fonction publique hospitalière (hôpitaux) (2011-2013)**   
 
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

#'    
#' | Décile \ euros   | 2011     | 2013   |   
#' |------------------|----------|--------|
#' |    D1            | 16 584   | 17 016 |  
#' |    D2            | 18 168   | 18 492 |    
#' |    D3            | 19 620   | 19 872 |  
#' |    D4            | 21 048   | 21 192 |  
#' |    D5 (médiane)  | 22 596   | 22 656 |    
#' |    D6            | 24 504   | 24 516 |    
#' |    D7            | 27 216   | 27 252 |    
#' |    D8            | 30 996   | 31 176 |    
#' |    D9            | 37 812   | 38 100 |    
#' |  Moyenne         | 26 496   | 26 916 |  
#' 

#'[Source INSEE, onglets Figure3, F1web et F3web - 2011](`r currentDir`/Docs/ip1486.xls)   
#'[Source INSEE, onglets F V3.1-2, F V3.1-5 - 2013](`r currentDir`/Docs/vue3_remunerations.xls)   

#'   
#'### `r chapitre`.2.2 Fonctionnaires      
#'   
#'**Titulaires et stagiaires**      

#'**Salaire net moyen par tête (SMPT net) en EQTP**       
#'**Ensemble**  
#'    
#'&nbsp;*Tableau `r incrément()`*   
#'    

Filtre_fonctionnaire <- function() Statut == "TITULAIRE" | Statut == "STAGIAIRE"

smpt(Filtre_fonctionnaire)

Filtre_cat_A <- function()   (Statut == "TITULAIRE"  | Statut == "STAGIAIRE")  & (Catégorie == "A")
Filtre_cat_B <- function()   (Statut == "TITULAIRE"  | Statut == "STAGIAIRE")  & (Catégorie == "B") 
Filtre_cat_C <- function()   (Statut == "TITULAIRE"  | Statut == "STAGIAIRE")  & (Catégorie == "C") 

#'   
#'**Catégorie A**  
#'
#'&nbsp;*Tableau `r incrément()`*   
#'    
#'  


smpt(Filtre_cat_A)  

#'**Distribution des salaires nets annuels en EQTP dans la fonction publique territoriale par catégorie (2011-2013)**   
#'  
#'*Comparaisons nationales*    
#'    
#' | Décile \ euros   | 2011     | 2013   |   
#' |------------------|----------|--------|
#' |    D1            | 26 040   | 26 340 |  
#' |    D2            | 28 992   |        |    
#' |    D3            | 31 272   |        |  
#' |    D4            | 33 468   |        |  
#' |    D5 (médiane)  | 35 820   | 36 312 |    
#' |    D6            | 38 664   |        |    
#' |    D7            | 42 276   |        |    
#' |    D8            | 47 124   |        |    
#' |    D9            | 54 840   | 55 032 |    
#' |  Moyenne         | 38 700   | 39 120 |       
#' 

#'   
#'**Catégorie B**  
#'
#'&nbsp;*Tableau `r incrément()`*   
#'    
#'  

smpt(Filtre_cat_B)  


#'*Comparaisons nationales*    
#'    
#' | Décile \ euros   | 2011     | 2013   |  
#' |------------------|----------|--------|
#' |    D1            | 20 580   | 20 964 |  
#' |    D2            | 22 272   |        |    
#' |    D3            | 23 652   |        |  
#' |    D4            | 24 960   |        |  
#' |    D5 (médiane)  | 26 244   | 26 820 |    
#' |    D6            | 27 636   |        |    
#' |    D7            | 29 160   |        |    
#' |    D8            | 30 984   |        |    
#' |    D9            | 33 804   | 34 224 |    
#' |  Moyenne         | 26 940   | 27 408 |   
#' 

#'   
#'**Catégorie C**  
#'
#'&nbsp;*Tableau `r incrément()`*    
#'    
#'  

smpt(Filtre_cat_C)    

#'     
#'*Comparaisons nationales*    
#'    
#' | Décile \ euros   | 2011     | 2013   |    
#' |------------------|----------|--------|
#' |    D1            | 15 972   |  16 296|  
#' |    D2            | 16 896   |        |    
#' |    D3            | 17 652   |        |  
#' |    D4            | 18 360   |        |  
#' |    D5 (médiane)  | 19 164   |  19 464|    
#' |    D6            | 20 100   |        |    
#' |    D7            | 21 216   |        |    
#' |    D8            | 22 680   |        |    
#' |    D9            | 24 996   |  25 176|    
#' |    Moyenne       | 20 016   |  20 268|  
#' 

# C 

#'   
#'**Effet de noria sur salaires nets et taux de remplacements**       
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

# D


#'     
#'*MS N-1 : masse salariale nette de l'année n-1.*   
#'       
#'**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         


#'**Fonctionnaires**    
#'&nbsp;*Tableau `r incrément()`*   
#'    

distribution_smpt(Filtre_fonctionnaire)

#'**Catégorie A**    
#'&nbsp;*Tableau `r incrément()`*   
#'    

distribution_smpt(Filtre_cat_A)

#'**Catégorie B**  
#'&nbsp;*Tableau `r incrément()`*   
#'    

distribution_smpt(Filtre_cat_B)

#'**Catégorie C**  
#'&nbsp;*Tableau `r incrément()`*   
#'    

distribution_smpt(Filtre_cat_C)

#'[Lien vers la base de données](`r currentDir`/Bases/Remunerations/Analyse.variations.par.exercice.csv)     

########### 4.3  GVT ###########  

#'## `r chapitre`.3 Glissement vieillesse-technicité (GVT)   
#'
#'### `r chapitre`.3.1 Ensemble des personnels   
#'   
#'*Cette section est consacrée à la rémunération moyenne des personnes en place (RMPP), définies comme présentes deux années entières consécutives avec la même quotité*   
#'*L'évolution de la RMPP permet d'étudier le glissement vieillesse-technicité "positif", à effectifs constants sur deux années*      
#'*Le GVT positif est dû aux mesures statutaires et individuelles, à l'avancement et aux changements d'activité*  


# Appliquer les filtres maintenant

q3 <- quantile(Analyse.variations$variation.rémunération, c(quantile.cut/100, 1 - quantile.cut/100), na.rm=TRUE)

# Filtrage : on enlève les personnels présents depuis moins d'un seuil de troncature (ex. 120 jours) dans l'année et les élus
# (paramètre seuil.troncature) 

# Filtrage pour l'étude des variations : on enlève les valeurs manquantes des variations, les centiles extrêmaux,
# les rémunérations nettes négatives ou proche de zéro. On exige un statut explicite en fin de période.
# Paramétrable par :
# minimum.positif, quantile.cut 

# ici il faut réduire la matrice pour éviter les réduplications pour les Résumés. TODO

Analyse.variations.synthèse <- Analyse.variations[total.jours > 2 * seuil.troncature
                                                           & pris.en.compte == TRUE
                                                           & ! is.na(statut)   
                                                           & ! is.na(variation.rémunération) 
                                                           & variation.rémunération > q3[[1]]
                                                           & variation.rémunération < q3[[2]]]

Analyse.variations.synthèse.plus.2.ans  <- Analyse.variations.synthèse[! is.na(plus.2.ans) & plus.2.ans == TRUE]
Analyse.variations.synthèse.moins.2.ans <- Analyse.variations.synthèse[! is.na(plus.2.ans) & plus.2.ans == FALSE]

#Analyse.variations.par.exercice <- Analyse.variations.par.exercice[Nexercices > 1]

if (nrow(Analyse.variations.synthèse.plus.2.ans) > 0 && durée.sous.revue > 1 ) {
  hist(Analyse.variations.synthèse.plus.2.ans$variation.moyenne.rémunération,
       xlab ="Variation annuelle moyenne en %",
       las = 1,
       xlim = c(-5,30),
       ylab ="Effectifs",
       main ="Rémunération nette des personnes en place",
       col ="blue",
       nclass=1000,
       xaxt = 'n')

try(axis(side=1, at=seq(-5,30, 1), labels=seq(-5,30,1), lwd=2))

Filtre_rmpp <- function() (est.rmpp == TRUE)
}

#'   
#'**Evolution de la RMPP nette en EQTP**     
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

if (durée.sous.revue > 1) {
   smpt(Filtre_rmpp, type = "RMPP nette") 
  } else  {
   cat("RMPP calculable uniquement si la période sous revue est au moins égale à 2 ans.")
  }

#'    
#'**Distribution et variation sur la période de la rémunération nette des personnes en place**                
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

# La légère différence de pérmètre entre Analyse.variations.synthèse et Analyse.variations.par.exercice tient au filtrage des quantiles
# extrêmaux et des valeurs manquantes des variations


masque.rmpp.fin.période    <- bitwShiftL(3, durée.sous.revue - 2)      #  11{0,1}...{0,1}
masque.rmpp.début.période  <- 3                                        #  {0,1}...{0,1}11
masque.présent.début.fin   <- bitwShiftL(1, durée.sous.revue - 1) + 1  #  10000..1
masque.présent.sur.période <- bitwShiftL(1, durée.sous.revue) - 1       #  11111..1

if (durée.sous.revue > 1) {

  Résumé(c(paste(début.période.sous.revue, début.période.sous.revue + 1, sep = "-"),
           "Effectif",
           paste(fin.période.sous.revue - 1, fin.période.sous.revue, sep = "-"),
           "Effectif"),
         list(Analyse.variations.synthèse[bitwAnd(indicatrice.période, masque.rmpp.début.période) == masque.rmpp.début.période 
                                          & Année == début.période.sous.revue + 1, 
                                             .(Montant.net.annuel.eqtp.début, quotité.moyenne)],
              Analyse.variations.synthèse[indicatrice.période >= masque.rmpp.fin.période
                                          & Année == fin.période.sous.revue, 
                                          .(Montant.net.annuel.eqtp.sortie, quotité.moyenne)]),
          extra = "length")
  
} else  {
  cat("Distribution de la RMPP calculable uniquement si la période sous revue est au moins égale à 2 ans.")
}

#'*RMPP en `r début.période.sous.revue + 1` des personnes en place en `r début.période.sous.revue`-`r début.période.sous.revue + 1`*     
#'*RMPP en `r fin.période.sous.revue` des personnes en place en `r fin.période.sous.revue - 1`-`r fin.période.sous.revue `*         
#'
#'*Variation individuelle de rémunération nette en EQTP pour les personnels présents la première et la dernière année*   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

if (durée.sous.revue > 1) {
  Résumé(c("Variation normalisée (%)",
           "Variation annuelle moyenne normalisée (%)",
           "Quotité",
           "Effectif"),
           Analyse.variations.synthèse[bitwAnd(indicatrice.période, masque.présent.début.fin) 
                                          == 
                                       masque.présent.début.fin
                                       & Année == début.période.sous.revue,
                                         .(variation.rémunération.normalisée,
                                           variation.moyenne.rémunération.normalisée,
                                           quotité.moyenne)],
         extra = "length")
}


# #'
# #'*Variation individuelle de rémunération nette en EQTP pour les personnels présents sur toute la période*   
# #'  
# #'&nbsp;*Tableau `r incrément()`*   
# #'  
# 
# Résumé("Variation normalisée (%)",
#         # "Variation annuelle moyenne normalisée (%)",
#          "Effectif"),
#        Analyse.variations.synthèse[indicatrice.période == masque.présent.sur.période, variation.rémunération.normalisée],
#        extra = "length")

#'    
#'### `r chapitre`.3.2 Titulaires et stagiaires     
#'   

Filtre_rmpp_fonctionnaire <- function () Filtre_fonctionnaire() & (est.rmpp == TRUE)

#'   
#'**Evolution de la RMPP nette en EQTP**     
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    
if (durée.sous.revue > 1)
    smpt(Filtre_rmpp_fonctionnaire, type = "RMPP nette")

#'    
#'**Distribution et variation sur la période de la rémunération nette des fonctionnaires en place**                
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

#'  
if (durée.sous.revue > 1) {
  
  Résumé(c(paste(début.période.sous.revue, début.période.sous.revue + 1, sep="-"),
           "Effectif",
           paste(fin.période.sous.revue - 1, fin.période.sous.revue, sep="-"),
           "Effectif"),
         list(Analyse.variations.synthèse[(statut == "TITULAIRE" | statut == "STAGIAIRE")
                                           & bitwAnd(indicatrice.période, masque.rmpp.début.période) == masque.rmpp.début.période
                                           & Année == début.période.sous.revue + 1,  
                                          .(Montant.net.annuel.eqtp.début, quotité.moyenne)],
              Analyse.variations.synthèse[(statut == "TITULAIRE" | statut == "STAGIAIRE")
                                          & indicatrice.période >= masque.rmpp.fin.période
                                          & Année == fin.période.sous.revue, 
                                          .(Montant.net.annuel.eqtp.sortie, quotité.moyenne)]),
         extra = "length")
  
}

#'*RMPP en `r début.période.sous.revue + 1` des personnes en place en `r début.période.sous.revue`-`r début.période.sous.revue + 1`*     
#'*RMPP en `r fin.période.sous.revue` des personnes en place en `r fin.période.sous.revue - 1`-`r fin.période.sous.revue `*         
#'

#'
#'*Variation individuelle de rémunération nette en EQTP pour les personnels présents la première et la dernière année*   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

if (durée.sous.revue > 1) {
  Résumé(c("Variation normalisée (%)",
           "Variation annuelle moyenne normalisée (%)",
           "Quotité",
           "Effectif"),
         Analyse.variations.synthèse[(statut == "TITULAIRE" | statut == "STAGIAIRE")
                                     & bitwAnd(indicatrice.période, masque.présent.début.fin)
                                        ==
                                       masque.présent.début.fin
                                     & Année == début.période.sous.revue,
                                       .(variation.rémunération.normalisée, 
                                         variation.moyenne.rémunération.normalisée,
                                         quotité.moyenne)],
         extra = "length")
}


#'
#'
#'[Lien vers la base de données](`r currentDir`/Bases/Remunerations/Analyse.variations.synthèse.csv)
#'
#'**Nota**   
#'*Personnes en place :* en fonction au moins deux années consécutives sur la période `r début.période.sous.revue` à `r fin.période.sous.revue`    
#'*Variation sur la période d'activité :* entre l'arrivée et le départ de la personne      
#'*Variation normalisée :* conforme à la définition INSEE (présente en début et en fin de période avec la même quotité)  
#'  
#'**Commentaire**       
#'Les différences éventuelles constatées entre l'évolution de la RMPP au tableau `r numéro.tableau-2` sont dues soit à l'effet de noria soit à l'effet périmètre.    
#'      

####### 4.4 Comparaisons nationales ####  

#'[Lien vers la base de données](`r currentDir`/Bases/Remunerations/Analyse.variations.synthèse.csv)
#'
#'
#'### `r chapitre`.4 Comparaisons avec la situation nationale des rémunérations   
#'  
#'**Évolution en euros courants du SMPT et de la RMPP dans la FPT (en % et euros courants)**    

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

#' 
#'| Année  |2008-09|2009-10|2010-11|2011-12|2012-13|2008-12|Moy. 2008-12|Méd. 2007-11|  
#'|:-------:|-----:|------:|-----:|----:|------:|------:|-----:|-----:|  
#'| SMPT brut  | 2,5 | 1,3 | 1,5 | 1,7 | 1,1 | 7,2 | 1,8 |  |  
#'|  SMPT net  | 3,0 | 1,4 | 1,3 | 1,4 | 0,8 | 7,3 | 1,8 | 13,4 |  
#'| RMPP brute | 3,3 | 2,5 | 2,5 | 2,7 | 1,9 | 11,5 | 2,8 |     |  
#'| RMPP nette | 3,3 | 2,5 | 2,3 | 2,4 | 1,6 | 10,9 | 2,6 |    |   
#' 

#'*Source : fichier général de l'État (FGE), DADS, SIASP, Insee, Drees. Traitement Insee, Drees, DGCL*    
#'Hors assistants maternels et familiaux, y compris bénéficiaires de contrats aidés.   
#'SMPT : Salaire moyen par tête en EQTP.   
#'RMPP : Agents présents 24 mois consécutifs chez le même employeur avec la même quotité de travail.  
#'Moyenne des variations géométriques annuelles pour les agents du champ.  
#'La dernière colonne présente la médiane des augmentations du SMPT net pour les agents présents en 2007 et 2011.   
#'  
#'**Salaires nets annuels et évolution moyenne type de collectivité en euros courants  EQTP**    
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


#' 
#'|  Collectivité  | SMPT net 2011 | SMPT net 2012 | SMPT net 2013 | Moy. 2007-2011 (%) |   
#'|:------------:|-----------:|-----------:|-----------:|---------:|       
#'|Communes                   |   20 784,0    |   21 120,0    |   21 096,0    | 2,5 |  
#'|CCAS et caisses des écoles |   19 415,0    |   19 716,0    |   19 788,0    | 2,4 |  
#'| EPCI à fiscalité propre   |   22 882,0    |   23 088,0    |   23 184,0    | 3,1 |  
#'| Autres structures intercommunales |   21 299,0    |   21 684,0    |   21 828,0    | 3,0 |  
#'|   Départements            |   24 487,0    |   24 744,0    |   24 852,0    | 3,9 |  
#'|   SDIS                    |   29 811,0    |   29 940,0    |   30 180,0    | 3,4 |  
#'|  Régions                  |   22 432,0    |   22 836,0    |   23 004,0    | 3,8 |  
#'| Autres collectivités locales  |   24 680,0    |   24 696,0    |   24 828,0    | 3,2 |  
#'|  Ensemble (moyenne)       |   21 873,0    |   22 176,0    |   22 212,0    | 2,9 |  
#' 

#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			
#'Conversion en euros courants, calcul CRC.  
#'[Source INSEE données 2011 obsolètes](http://www.insee.fr/fr/ffc/ipweb/ip1486/ip1486.xls)   
#'[Source DGAFP](http://infos.emploipublic.fr/dossiers/la-fonction-publique-en-chiffres/la-fonction-publique-en-chiffre-2013/non-defini-08/apm-71444/)  
#'[Source PLF 2014 données 2011 révisées p.151](http://www.fonction-publique.gouv.fr/files/files/statistiques/jaunes/jaune2014_FP.pdf)   
#'[Source PLF 2015 données 2012 p.130](http://www.performance-publique.budget.gouv.fr/sites/performance_publique/files/farandole/ressources/2015/pap/pdf/jaunes/jaune2015_fonction_publique.pdf)   
#'    

incrémenter.chapitre()

newpage()

#'
########### 5. TESTS STATUTAIRES ########################
#'

#### 5.1 NBI ####

#'# `r chapitre`. Tests réglementaires   
#'## `r chapitre`.1 Contrôle des NBI et primes informatiques   
#'   
#'**Dans cette partie, l'ensemble de la base de paie est étudié.**  
#'Les agents non actifs ou dont le poste est annexe sont réintroduits dans le périmètre.   

#+ tests-statutaires-nbi

if (N <- uniqueN(Paie[Statut != "TITULAIRE"
                            & Statut != "STAGIAIRE"
                            & NBI != 0, 
                              Matricule]))
  cat("Il existe ", FR(N), "non titulaire", ifelse(N>1, "s", ""), " percevant une NBI.")

setkey(Paie, Statut)
Paie[c("NON_TITULAIRE", "EMPLOI_AIDE"),
           indic2 := NBI != 0 & grepl(expression.rég.nbi, Libellé, ignore.case=TRUE, perl=TRUE)]
 
NBI.aux.non.titulaires <- Paie[indic2 == TRUE, c(étiquette.matricule,
                                                  "Statut",
                                                  étiquette.code,
                                                  étiquette.libellé,
                                                  étiquette.année,
                                                  "Mois",
                                                  "NBI",
                                                  étiquette.montant),
                                                   with=FALSE] 

Paie[,indic2 := NULL]

nombre.Lignes.paie.NBI.nontit <- nrow(NBI.aux.non.titulaires)

# Prime de fonctions informatiques : pas dans la base de VLB
# on cherche la chaine de char. "INFO" dans les libellés de primes

# variante : filtre <- regexpr(".*(INFO|PFI|P.F.I).*", toupper(Paie$Libellé)) et regmatches(Paie$Libellé, filtre)

personnels.prime.informatique <- Paie[grepl(expression.rég.pfi, Libellé, ignore.case=TRUE, perl=TRUE) == TRUE,
                                         .(Matricule,
                                           Année,
                                           Mois,
                                           Statut,
                                           Code,
                                           Libellé,
                                           Montant)]

nombre.personnels.pfi <- nrow(personnels.prime.informatique)

primes.informatiques.potentielles <- if (nombre.personnels.pfi == 0) "aucune" else unique(personnels.prime.informatique$Libellé)

#'Primes informatiques : `r primes.informatiques.potentielles`
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau(
  c("Nombre de lignes NBI pour non titulaires",
    "Nombre de bénéficiaires de PFI"),
  nombre.Lignes.paie.NBI.nontit,
  nombre.personnels.pfi)

#'   
#'[Lien vers la base de données NBI aux non titulaires](`r currentDir`/Bases/Reglementation/NBI.aux.non.titulaires.csv)   
#'[Lien vers la base de données Primes informatiques](`r currentDir`/Bases/Reglementation/personnels.prime.informatique.csv)   
#'   
#'**Nota :**   
#'NBI: nouvelle bonification indiciaire   
#'PFI: prime de fonctions informatiques   
#'

T1 <- Bulletins.paie[ , .(nbi.cumul.indiciaire=sum(NBI, na.rm = TRUE)), by="Matricule,Année"] 
T1 <- T1[nbi.cumul.indiciaire > 0] 

T2 <- Paie[grepl(expression.rég.nbi, Libellé, perl=TRUE, ignore.case=TRUE) == TRUE 
           & Type %chin% c("T", "I")
           & NBI != 0,
           .(nbi.cumul.montants = sum(Montant, na.rm=TRUE)), keyby="Matricule,Année"]

T2 <- T2[nbi.cumul.montants != 0]

T <- merge(T1, T2, by=c("Matricule", "Année"))

cumuls.nbi <- T[, .(cumul.annuel.indiciaire = sum(nbi.cumul.indiciaire, na.rm = TRUE),
      cumul.annuel.montants = sum(nbi.cumul.montants, na.rm = TRUE)), keyby="Année"]

T <- T[, ratio := nbi.cumul.montants/nbi.cumul.indiciaire]

T <- T[, nbi.anormale := (abs(ratio) < 4 | abs(ratio) > 6)]

lignes.nbi.anormales <- T[nbi.anormale == TRUE, .(Matricule, Année, nbi.cumul.indiciaire, nbi.cumul.montants)]

montants.nbi.anormales <- sum(lignes.nbi.anormales$nbi.cumul.montants, na.rm = TRUE)

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau(
  c("Rémunérations de NBI anormales, par agent et par exercice",
    "Montants correspondants"),
  nrow(lignes.nbi.anormales),
  formatC(montants.nbi.anormales, big.mark = " ", format="f", digits=0))

rm(T, T1, T2)
#'   
#'[Lien vers la base de données NBI anormales](`r currentDir`/Bases/Fiabilite/lignes.nbi.anormales.csv)   
#'   
#'**Nota :**   
#'*Est considéré comme manifestement anormal un total annuel de rémunérations NBI correspondant à un point d'indice net mensuel inférieur à 4 euros ou supérieur à 6 euros.*    
#'*Les rappels ne sont pas pris en compte dans les montants versés. Certains écarts peuvent être régularisés en les prenant en compte*     
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    
attach(cumuls.nbi)

Tableau.vertical2(c("Année", "Cumuls des NBI", "Montants versés (a)", "Point d'INM apparent", "Point d'INM moyen", "Contrôle"), 
                  Année, 
                  cumul.annuel.indiciaire,
                  cumul.annuel.montants,
                  a <- cumul.annuel.montants/cumul.annuel.indiciaire,
                  b <- PointMensuelIMMoyen[Année - 2007],
                  ifelse(abs(b - a) > 0.3, "Rouge", ifelse(abs(b - a) > 0.15, "Orange", "Vert")))
#'
#'*(a) Hors rappels sur rémunérations*   
#'

detach(cumuls.nbi)

#### 5.2 VACATIONS ####
#'   
#'[Lien vers la base de données des cumuls annuels de NBI](`r currentDir`/Bases/Fiabilite/cumuls.nbi.csv)   
#'   

#'  
#'## `r chapitre`.2 Contrôle des vacations horaires pour les fonctionnaires      

# Vacations et statut de fonctionnaire

#+ tests-statutaires-vacations

#'Les fonctionnaires peuvent effectuer des vacations horaires pour leur propre employeur à condition de bénéficier d'une autorisation
#'de cumul d'activité accessoire et que les activités concernées ne fassent pas partie du service normal. Les cumuls détectés ci-dessous
#'se limitent aux cas de vacations horaires détectées. L'existence des pièces justificatives pourra être recherchée.

# ----- Trouver, pour toutes les lignes de paie non-charges, une indication de proportionnalité horaire explicite
#       et retenir les variables de calcul du montant et du statut pour les mois concernés par l'existence d'au moins
#       une de ces lignes horaires, assimilée à un paiement de vacation.


Paie_vac <- Paie[Type %chin% c("T", "I", "R", "IR", "S"),
                   .(indic_hor = any(grepl("HOR", Libellé, ignore.case = TRUE)), Nom, Statut, Code, Libellé, Type, Taux, Nb.Unité, Montant),
                   by=.(Matricule, Année, Mois)
                 ][indic_hor == TRUE,
                 ][ , indic_hor := NULL]


# ----- Produire la liste de ces libellés horaires

libellés.horaires <- unique(Paie_vac[grepl("HOR", Libellé, ignore.case = TRUE) == TRUE, Libellé])

# ----- Vérifier si des fonctionnaires titulaires ou stagiaires bénéficient de vacations horaires et donner les caractéristiques

Paie_vac_fonct <- Paie_vac[Statut %chin% c("TITULAIRE", "STAGIAIRE"), 
                                               .(Nom, Statut, Code, Libellé, Type, Taux, Nb.Unité, Montant, Total.mensuel = sum(Montant, na.rm = TRUE)), by = .(Matricule, Année, Mois)]

lignes.fonctionnaires.et.vacations <- Paie_vac_fonct[Libellé %chin% libellés.horaires]

matricules.fonctionnaires.et.vacations <- unique(lignes.fonctionnaires.et.vacations[ , .(Matricule, Nom, Statut)], by = NULL)
nombre.fonctionnaires.et.vacations <- length(matricules.fonctionnaires.et.vacations[[1]])

if (nombre.fonctionnaires.et.vacations > 0) {
  cat("Il y a ",
      FR(nombre.fonctionnaires.et.vacations),
      "fonctionnaire(s) effectuant des vacations pour son propre établissement. Les bulletins concernés sont donnés en lien." )
}  else  {
  cat("Pas de vacation détectée.")
}

#'[Matricules des fonctionnaires concernés](`r currentDir`/Bases/Reglementation/matricules.fonctionnaires.et.vacations.csv)       
#'[Lien vers les vacations payées à des fonctionnaires](`r currentDir`/Bases/Reglementation/lignes.fonctionnaires.et.vacations.csv)       
#'[Lien vers les bulletins de paie correspondants](`r currentDir`/Bases/Reglementation/Paie_vac_fonct.csv)            

####  5.3 CEV ####  
  
#'
#'## `r chapitre`.3 Contrôles sur les contractuels effectuant des vacations horaires    

#+ tests-statutaires-vacations-ri

#'Les vacataires rémunérés à la vacation horaire n'ont, en principe, pas accès au régime indemnitaire dont bénéficient les titulaires et non-titulaires 
#'sauf si l'assemblée délibérante a explicitement prévu de déterminer le taux des vacations horaires par référence à ces régimes.
#'Les vacataires bénéficiant d'un régime indemnitaire de ce type perçoivent des lignes de rémunération indemnitaires 
#'identifiées en base de paye (*catégorie "Indemnite" des bases XML et type "I" des bases CSV*).      
#'L'existence et l'applicabilité de la délibération correspondante pourra être recherchée.     
#'Les vacations effectuées par des contractuels de l'établissement sur autorisation de cumul d'activité accessoire doivent
#'avoir obtenu cette autorisation. Le régime indemnitaire dont ils bénéficient pour leur activité principale ne s'étend pas de plein droit
#'à l'activité accessoire, en l'absence de délibération le prévoyant au titre de référence de calcul.

  Paie_vac_contr <- Paie_vac[Statut %chin% c("NON_TITULAIRE",  "AUTRE_STATUT")]
                                           
  matricules.contractuels.et.vacations <- unique(Paie_vac_contr[ , .(Matricule, Nom, Statut)], by=NULL)

  nombre.contractuels.et.vacations     <- nrow(matricules.contractuels.et.vacations)
    
  RI.et.vacations         <- data.frame(NULL)

  if (nombre.contractuels.et.vacations) 
  {
     RI.et.vacations <- Paie_vac_contr[Type == "I"]
  }

  nombre.Lignes.paie.RI.et.vacations           <- nrow(RI.et.vacations)

#'
#'**Contractuels effectuant des vacations horaires (CEV)**
#'

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    
  
if (exists("nombre.contractuels.et.vacations")) {
  
  Tableau(c("Nombre de CEV",
            "Nombre de lignes indemnitaires payées"),
            nombre.contractuels.et.vacations,
            nombre.Lignes.paie.RI.et.vacations)
}
  
#'  
#'[Lien vers les matricules des vacataires](`r currentDir`/Bases/Reglementation/matricules.contractuels.et.vacations.csv)   
#'[Lien vers la lignes indemnitaires à vérifier](`r currentDir`/Bases/Reglementation/RI.et.vacations.csv)    
#'[Lien vers les bulletins de paye correspondants](`r currentDir`/Bases/Reglementation/Paie_vac_contr.csv)   
#'    

#'Les contractuels vacataires rémunérés sur prestation horaire n'ont pas accès au SFT ni à l'indemnité de résidence, contrairement aux contractuels
#'de droit public dont les rémunérations sont calculées sur une base indiciaire. 
#'Les non-titulaires sur contrat effectuant des vacations à titre accessoire pour leur propre employeur ne peuvent bénéficier de paiements
#'complémentaires de SFT ou d'indemnité de résidence au titre de ces activités accessoires.     
  
  Paie_vac_sft_ir <- Paie_vac[! Statut %chin% c("TITULAIRE", "STAGIAIRE"), 
                                 indic_s := any(Type %chin% c("IR", "S")),
                                 by = .(Matricule, Année, Mois)
                             ][indic_s == TRUE,
                             ][ ,indic_s := NULL]
  
  SFT_IR.et.vacations <- Paie_vac_sft_ir[Type %chin% c("IR", "S")]
  
  matricules.SFT_IR.et.vacations <- unique(SFT_IR.et.vacations[ , .(Matricule, Nom, Statut)], by=NULL)
  
  nombre.SFT_IR.et.vacations     <- nrow(matricules.SFT_IR.et.vacations)
  
  
#'
#'**CEV percevant le supplément familial de traitement ou l'indemnité de résidence**      
#'
  
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    
  
  if (exists("nombre.SFT_IR.et.vacations")) {
    
    Tableau(c("Nombre d'agents concernés",
              "Nombre de lignes de paye SFT/IR"),
            nombre.SFT_IR.et.vacations,
            nrow(SFT_IR.et.vacations))
  }
  
#'  
#'[Lien vers les matricules concernés](`r currentDir`/Bases/Reglementation/matricules.SFT_IR.et.vacations.csv)     
#'[Lien vers les lignes SFT/IR à vérifier](`r currentDir`/Bases/Reglementation/SFT_IR.et.vacations.csv)   
#'[Lien vers les bulletins de paye correspondants](`r currentDir`/Bases/Reglementation/Paie_vac_sft_ir.csv)    
#'   
  
#### 5.4 IAT/IFTS ####  
  
#'
#'## `r chapitre`.4 Contrôle sur les indemnités IAT et IFTS      

#+ IAT-et-IFTS

résultat.ifts.manquant <- FALSE
résultat.iat.manquant  <- FALSE

Paie <- Paie[ , `:=`(ifts.logical = grepl(expression.rég.ifts, Paie$Libellé, ignore.case=TRUE, perl=TRUE),
                     iat.logical  = grepl(expression.rég.iat, Paie$Libellé, ignore.case=TRUE, perl=TRUE))]


codes.ifts  <- list("codes IFTS" = unique(Paie[ifts.logical == TRUE][ , Code]))

if (length(codes.ifts) == 0) {
  cat("Il n'a pas été possible d'identifier les IFTS par expression régulière.")
  résultat.ifts.manquant <- TRUE
}

if (! any(Paie$iat.logical)) {
  cat("Il n'a pas été possible d'identifier les IAT par expression régulière.")
  résultat.iat.manquant <- TRUE
}

nombre.agents.cumulant.iat.ifts <- 0

if (! résultat.ifts.manquant && ! résultat.iat.manquant) {
  
  Paie[ , cumul.iat.ifts := any(ifts.logical[Type == "I"]) & any(iat.logical[Type == "I"]), by="Matricule,Année,Mois"]
  
  # on exclut les rappels !
  
  personnels.iat.ifts <- Paie[cumul.iat.ifts == TRUE
                              & (ifts.logical == TRUE | iat.logical == TRUE),
                                .(Matricule, Année, Mois, Code, Libellé, Montant, Type, Emploi, Grade, Service)]
  
  nombre.mois.cumuls <- uniqueN(personnels.iat.ifts[ , .(Matricule, Année, Mois)], 
                                    by = NULL)
  
  nombre.agents.cumulant.iat.ifts <- uniqueN(personnels.iat.ifts$Matricule)
  
  personnels.iat.ifts <- personnels.iat.ifts[order(Année, Mois, Matricule)]
}

#'
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'      
if (nombre.agents.cumulant.iat.ifts) {
  if (length(codes.ifts) < 10) {
    Tableau(c("Codes IFTS", " "),
            sep.milliers = "",
            paste(unlist(codes.ifts), collapse=" "), " ")
  } else {
    
    cat ("Nombre de personnels percevant IAT et IFTS : ", paste(unlist(codes.ifts), collapse=" "), "\n")
  }
}

# Pour de mystérieuses raisons liées à Tableau() il faut répéter la condition.

if (nombre.agents.cumulant.iat.ifts) {
  
  Tableau(c("Nombre de personnels percevant IAT et IFTS", " "), nombre.agents.cumulant.iat.ifts, " ")
       
} else {
  cat("Tests IAT/IFTS sans résultat positif.")
}

#'   
#'[Codes IFTS retenus](`r currentDir`/Bases/Reglementation/codes.ifts.csv)   
#'[Lien vers la base de données cumuls iat/ifts](`r currentDir`/Bases/Reglementation/personnels.iat.ifts.csv)    
#'
#'### Contrôle sur les IFTS pour catégories B et contractuels

#IFTS et IB >= 380 (IM >= 350)
#'  
if (! résultat.ifts.manquant) {
    lignes.ifts.anormales <- na.omit(Paie[as.integer(Indice) < 350
                                          & Catégorie != "A"
                                          & ifts.logical == TRUE,
                                            c(clé.fusion,
                                              étiquette.année,
                                              "Mois",
                                              "Statut",
                                              "Catégorie",
                                              étiquette.code,
                                              étiquette.libellé,
                                              "Indice",
                                              étiquette.montant,
                                              "Service"), 
                                            with=FALSE])
} else {

    lignes.ifts.anormales <- NULL
    cat("Il n'a pas été possible de déterminer les lignes IFTS anormales faute d'indentification des libellés IFTS.")
}
#'  
nombre.lignes.ifts.anormales <- nrow(lignes.ifts.anormales)

#+ IFTS-et-non-tit

ifts.et.contractuel <- NULL 

if (! résultat.ifts.manquant) {
  
  ifts.et.contractuel <- Paie[ Statut != "TITULAIRE"
                              & Statut != "STAGIAIRE"
                              & ifts.logical == TRUE,
                               c(étiquette.matricule,
                                 étiquette.année,
                                 "Mois",
                                 "Statut",
                                 "Catégorie",
                                 étiquette.code,
                                 étiquette.libellé,
                                 "Indice",
                                 étiquette.montant),
                               with=FALSE]
  }

nombre.lignes.ifts.et.contractuel <- nrow(ifts.et.contractuel)

#'
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

if (! résultat.ifts.manquant) {
   Tableau(c("Nombre de lignes de paie de contractuels percevant des IFTS", "Nombre de lignes IFTS pour IB < 380"), nombre.lignes.ifts.et.contractuel, nombre.lignes.ifts.anormales)
}

#'
#'[Lien vers la base de données Lignes IFTS pour contractuels](`r currentDir`/Bases/Reglementation/ifts.et.contractuel.csv)    
#'[Lien vers la base de données Lignes IFTS pour IB < 380](`r currentDir`/Bases/Reglementation/lignes.ifts.anormales.csv)     
#'
#'**Nota :**
#'IB < 380 : fonctionnaire percevant un indice brut inférieur à 380
#'

#### 5.5 PFR ####

#'
#'## `r chapitre`.5 Contrôle de la prime de fonctions et de résultats (PFR)   
#'   

#+ pfr

résultat.pfr.manquant <- FALSE
nombre.agents.cumulant.pfr.ifts <- 0

# L'expression régulière capte la PFR et la PR 
# Le cumul de la PR et de l'IFTS est régulier, de même que celui de la PR et de la PFR
# le cumul de la PFR et de l'IFTS est irrrégulier

Paie_I <- Paie[Type == "I", .(Matricule, Année, Mois, Code, Libellé, Montant, Type, Emploi, Grade, Catégorie)]

Paie_I[ , pfr.logical := grepl(expression.rég.pfr, Libellé, ignore.case = TRUE, perl = TRUE)]

PFR.non.catA <- Paie_I[Catégorie != "A", ][pfr.logical == TRUE]

if ((N.PFR.non.catA <<- nrow(PFR.non.catA)) > 0) {
  cat(N.PFR.non.catA, "attributaires de la PFR ne sont pas identifiés en catégorie A.")
  kable(PFR.non.catA, align = 'r', row.names = FALSE)
  
} else {
  cat("Tous les attributaires de la PFR sont identifiés en catégorie A.")
}

codes.pfr  <- list("codes PFR" = unique(Paie_I[pfr.logical == TRUE, Code]))

if (length(codes.pfr) == 0) {
  cat("Il n'a pas été possible d'identifier la PFR par expression régulière.")
  résultat.pfr.manquant <- TRUE
}

if (! résultat.ifts.manquant && ! résultat.pfr.manquant) {
  
  Paie_I[ , cumul.pfr.ifts := any(pfr.logical) & any(ifts.logical), 
              by="Matricule,Année,Mois"]

  # on exclut les rappels !
  
  personnels.pfr.ifts <- Paie_I[cumul.pfr.ifts == TRUE][pfr.logical == TRUE | ifts.logical == TRUE]

  nombre.mois.cumuls <- uniqueN(personnels.pfr.ifts[ , .(Matricule, Année, Mois)], by = NULL)
  
  nombre.agents.cumulant.pfr.ifts <- uniqueN(personnels.pfr.ifts$Matricule)
  
  personnels.pfr.ifts <- personnels.pfr.ifts[order(Année, Mois, Matricule)]
  
  rm(Paie_I)
}

#'
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'      
if (length(codes.pfr) < 6) {
  Tableau(c("Codes PFR", "Agents cumulant PFR et IFTS"),
          sep.milliers = "",
          paste(unlist(codes.pfr), collapse = " "),
          nombre.agents.cumulant.pfr.ifts)
} else {
  
  cat("Codes PFR : ", paste(unlist(codes.pfr), collapse = " "), "\n")
  
}

if (length(codes.pfr) > 5) {
  Tableau("Agents cumulant PFR et IFTS",
          nombre.agents.cumulant.pfr.ifts)
}

#'   
#'[Lien vers la base de données cumuls pfr/ifts](`r currentDir`/Bases/Reglementation/personnels.pfr.ifts.csv)    
#'

  # P <- Paie[Code %chin% union(unlist(codes.pfr), unlist(codes.ifts)),
  #           .(Attrib.PFR = any(pfr.logical), Cumul.PFR.IFTS = sum(Montant, na.rm = TRUE), Grade = Grade[1]), 
  #           by="Nom,Matricule,Année"]
  # 
  # P.any <- P[, .(attrib.any = any(Attrib.PFR)), by="Nom,Matricule"]
  # 
  # P <- merge(P, P.any, by=c("Nom", "Matricule"))
  # P <- P[attrib.any == TRUE]
  # 
  # bénéficiaires.PFR <- P[, attrib.any := NULL]
  # bénéficiaires.PFR <- P[, Attrib.PFR := NULL]
  # rm(P)

Paie_I[pfr.logical == TRUE | ifts.logical == TRUE][, .(Attrib.PFR = any(pfr.logical),
                                                       Cumul.PFR.IFTS = sum(Montant, na.rm = TRUE),
                                                       Grade = Grade[1]),
                                                     by= .(Nom, Matricule, Année)
                                                 ][Attrib.PFR == TRUE,
                                                 ][ , Attrib.PFR := NULL]

  
# Plafonds annuels (plafonds mensuels reste à implémenter)
# AG 58 800
# ADTHC 55 200
# ADT   49 800
# D/ATP 25 800
# SM/AT 20 100

#'  
#'&nbsp;*Tableau `r incrément()` : rappel des plafonds annuels de la PFR*   
#'      
  
Tableau(c("Adm. général", "Adm. HC", "Adm.", "Direct./Attaché princ.", "Secr. mairie/Attaché"),
        sapply(PFR.plafonds <<- list( admin.g = 58800, admin.hc = 55200, admin = 49800, attaché.p = 25800, attaché = 20100), 
               function(x) formatC(x, format = "fg", big.mark = " ")))
   
  
e <- c(expression.rég.admin.g, expression.rég.admin.hc, expression.rég.admin, expression.rég.attaché.p, expression.rég.attaché)
  
test.PFR <- function(i, grade, cumul) { grepl(e[i], grade, perl = TRUE, ignore.case = TRUE) & (cumul > PFR.plafonds[[i]]) }
  test.PFR.all <- function(grade, cumul) any(sapply(1:length(e), function(i) test.PFR(i, grade, cumul)))
  
  dépassements.PFR.boolean <- mapply(test.PFR.all, bénéficiaires.PFR$Grade, bénéficiaires.PFR$Cumul.PFR.IFTS, USE.NAMES=FALSE)

  dépassements.PFR.plafonds <- data.frame()
  
  if (length(dépassements.PFR.boolean) > 0)
    dépassements.PFR.plafonds <- bénéficiaires.PFR[dépassements.PFR.boolean]
  
  if (nrow(dépassements.PFR.plafonds) > 0) {
    
    cat("\nLes plafonds annuels de la PFR sont dépassés pour ", nrow(dépassements.PFR.plafonds), " cumuls annuels.\n")
    kable(dépassements.PFR.plafonds, align = 'r', row.names = FALSE)
  } else {
    cat("\nLes plafonds annuels de la PFR de sont pas dépassés.\n")
  }
    
  bénéficiaires.PFR.Variation <- bénéficiaires.PFR[ , .(Années = paste(Année, collapse=", "), 
                                  `Variation (%)`= round((Cumul.PFR.IFTS[length(Année)]/Cumul.PFR.IFTS[1] - 1) * 100, 1),
                                   `Moyenne géométrique annuelle(%)`= round(((Cumul.PFR.IFTS[length(Année)]/Cumul.PFR.IFTS[1])^(1/(length(Année) - 1)) - 1) * 100, 1)),
                                   by="Nom,Matricule"]
  
  bénéficiaires.PFR.Variation <- bénéficiaires.PFR.Variation[`Variation (%)` != 0.00]

#'  
#'&nbsp;*Tableau `r incrément()`* : Valeurs de l'agrégat (PFR ou IFTS) pour les bénéficiaires de la PFR   
#'          

  if (nrow(bénéficiaires.PFR)) {
    bénéficiaires.PFR$Cumul.PFR.IFTS <- formatC(bénéficiaires.PFR$Cumul.PFR.IFTS, big.mark = " ", format="fg")
    setnames(bénéficiaires.PFR, "Cumul.PFR.IFTS", "Cumul PFR ou IFTS")
    kable(bénéficiaires.PFR, align = 'r', row.names = FALSE)
  } else {
    cat("\nAucun bénéficiaire de la PFR détecté.\n")
  }
  
#'  
#'&nbsp;*Tableau `r incrément()`* : Variations de l'agrégat (PFR ou IFTS) pour les bénéficiaires de la PFR
#'          
  if (nrow(bénéficiaires.PFR.Variation)) {
    kable(bénéficiaires.PFR.Variation, align = 'r', row.names = FALSE)
  } else {
    cat("\nAucun tableau de variation.\n")
  }
  
        

#'   
#'[Lien vers la base de données agrégat PFR-IFTS](`r currentDir`/Bases/Remunerations/bénéficiaires.PFR.csv)    
#'
#'   
#'[Lien vers la base de données variations agrégat PFR-IFTS](`r currentDir`/Bases/Remunerations/bénéficiaires.PFR.Variation.csv)    
#'

  
#### 5.6 HEURES SUP ####
#'
#'## `r chapitre`.6 Contrôle sur les heures supplémentaires

# Sont repérées comme heures supplémentaires ou complémentaires les heures dont le libellé obéissent à
# l'expression régulière expression.rég.heures.sup donnée par le fichier prologue.R

# Vérification des seuils annuels :

Dépassement.seuil.180h <- unique(Bulletins.paie[cumHSup > 180, 
                                                  .(Matricule, Année, "Cumul heures sup" = cumHSup, Emploi, Grade, Service)])
  
nb.agents.dépassement  <- uniqueN(Dépassement.seuil.180h$Matricule)

if  (nb.agents.dépassement)  {
  cat("Le seuil de 180 heures supplémentaires maximum est dépassé par ", FR(nb.agents.dépassement), " agents.\n")
  Dépassement.seuil.220h <- Dépassement.seuil.180h["Cumul heures sup" > 220]
  nb.agents.dépassement.220h <- uniqueN(Dépassement.seuil.220h$Matricule) 
  
  if  (nb.agents.dépassement.220h) cat(" Le seuil de 220 heures supplémentaires maximum est dépassé par ", FR(nb.agents.dépassement.220h), " agents.\n") 
}

colonnes <- c(étiquette.matricule,
              étiquette.année,
              "Mois",
              "Statut",
              "Libellé",
              étiquette.code,
              "Heures",
              "Heures.Sup.",
              "Base",
              "Taux",
              "Montant",
              "Type",
              "Service",
              "Emploi",
              "Grade")

HS.sup.25 <- Paie[Heures.Sup. > 25, colonnes, with=FALSE]

#setkey(HS.sup.25, Type)

HS.sup.indiciaire.mensuel <- HS.sup.25[Type == "T", .(Matricule, Année, Mois, Montant)]

HS.sup.25 <-  HS.sup.25[Type %chin% c("I", "T", "R", "S", "IR", "A")
                          & ! grepl(".*SMIC.*",
                                    Libellé, 
                                    ignore.case = TRUE)
                          & grepl(expression.rég.heures.sup,
                                  Libellé,
                                  ignore.case = TRUE,
                                  perl=TRUE), ]

HS.sup.25 <- HS.sup.25[order(Matricule, Année, Mois), ]

# La méthode data.table est beaucoup plus efficiente

traitement.indiciaire.mensuel <- HS.sup.indiciaire.mensuel[ ,  sum(Montant, na.rm = TRUE), 
                                                               by="Matricule,Année,Mois"]
names(traitement.indiciaire.mensuel)[4] <- "Traitement indiciaire mensuel" 
  
HS.sup.25 <- merge(HS.sup.25, traitement.indiciaire.mensuel, by=c("Matricule", "Année", "Mois"))

HS.sup.25 <- merge(HS.sup.25, Analyse.rémunérations[ , .(Matricule, Année, traitement.indiciaire)], 
                                                       by=c("Matricule", "Année"))

HS.sup.25 <- unique(HS.sup.25, by=NULL)

names(HS.sup.25) <- sub("traitement.indiciaire", "Traitement indiciaire annuel", names(HS.sup.25))

nombre.Lignes.paie.HS.sup.25 <- nrow(HS.sup.25)

ihts.anormales <- Paie[grepl(expression.rég.heures.sup,
                                    Libellé,
                                    ignore.case = TRUE,
                                    perl=TRUE)
                       & Montant != 0
                       & Catégorie == "A"
                       & Type %chin% c("R", "I", "T", "A"),
                       .(Matricule, Année, Mois, Statut, Grade, Heures.Sup., Libellé, Code, Type, Montant)]


nombre.ihts.anormales <- nrow(ihts.anormales) 

if (! is.null(HS.sup.25)) message("Heures sup controlées")
#'
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau(c("Nombre de lignes HS en excès", "Nombre de lignes IHTS anormales"), nombre.Lignes.paie.HS.sup.25, nombre.ihts.anormales)

#'
#'[Lien vers la base de données Heures supplémentaires en excès du seuil de 25h/mois](`r currentDir`/Bases/Reglementation/HS.sup.25.csv)     
#'[Lien vers la base de données cumuls en excès des seuils annuels](`r currentDir`/Bases/Reglementation/Dépassement.seuil.180h.csv)    
#'[Lien vers la base de données IHTS anormales](`r currentDir`/Bases/Reglementation/ihts.anormales.csv)      
#'
#'**Nota :**   
#'HS en excès : au-delà de 25 heures par mois     
#'IHTS anormales : attribuées à des fonctionnaires ou non-titulaires de catégorie A ou assimilés.     

#### 5.7 ELUS ####

#' 
#'## `r chapitre`.7 Contrôle sur les indemnités des élus
#'   

rémunérations.élu <- Analyse.rémunérations[ indemnités.élu > minimum.positif,
                                            c(clé.fusion,
                                              "Année",
                                              "Emploi",
                                              "indemnités.élu",
                                              "autres.rémunérations",
                                              "rémunération.indemnitaire.imposable"),
                                            with=FALSE ]

rémunérations.élu[ , rémunération.indemnitaire.imposable := indemnités.élu +  rémunération.indemnitaire.imposable]

rémunérations.élu <- merge(unique(matricules[ , .(Nom,  Matricule)], by=NULL),
                             rémunérations.élu,
                             by = étiquette.matricule,
                             all.y = TRUE,
                             all.x = FALSE)

names(rémunérations.élu) <- c(union(clé.fusion, "Nom"),
                              "Année",
                              "Emploi",
                              "Indemnités ",
                              "Autres ",
                              "Total ")

rémunérations.élu <- na.omit(rémunérations.élu)

#'   
if (générer.table.élus)   {
  
    if (nrow(rémunérations.élu) > 0) {
      
      kable(rémunérations.élu, row.names = FALSE)
      
    } else {
  
      cat("Tableau des indemnités d'élu : pas de données.")
    } 
} else {

   cat("Tableau des indemnités d'élu : non générée.")
}


#'[Lien vers la base de données Rémunérations des élus](`r currentDir`/Bases/Reglementation/rémunérations.élu.csv)
#'

#### 5.8 COMPTE DE GESTION ####

#'## `r chapitre`.8 Lien avec le compte de gestion
 

cumul.lignes.paie <- Paie[Type %chin% c("T", "I", "R", "IR", "S", "A", "AC") , 
                        .(Total = sum(Montant, na.rm = TRUE)), keyby="Année,Type,Libellé,Code"]

cumul.lignes.paie <- cumul.lignes.paie[Total != 0]

cumul.lignes.paie[  Type == "R",
                   Type := "Rappels"
               ][
                   Type == "T", 
                   Type := "Traitements"
               ][
                   Type == "I", 
                   Type := "Indemnités"
               ][
                   Type == "IR",
                   Type := "Indem. Résidence"
               ][ 
                   Type == "S",
                   Type := "Supplément familial"
               ][
                   Type == "A",
                   Type := "Rém. diverses"
               ][
                   Type == "AC", 
                   Type := "Acomptes"
                   
               ][ , Total2  := formatC(Total, big.mark = " ", format = "f", decimal.mark = ",", digits = 2)]


cumul.total.lignes.paie <- cumul.lignes.paie[ , .(`Cumul annuel`= formatC(sum(Total, na.rm = TRUE), big.mark = " ", format = "f", decimal.mark = ",", digits = 2)), 
                                                keyby = "Année,Type"]

setnames(cumul.lignes.paie[ , Total := NULL], "Total2", "Total")

L <- split(cumul.lignes.paie, cumul.lignes.paie$Année)

  

if (afficher.cumuls.détaillés.lignes.paie) {
  for (i in 1:durée.sous.revue) {
    cat("\nTableau ", incrément(), " Année ", début.période.sous.revue + i - 1)
    print(kable(L[[i]][, .(Catégorie = Type, Code, Libellé, Total)], row.names = FALSE, align = 'r'))
    incrément()
  }
}

L <- split(cumul.total.lignes.paie, cumul.total.lignes.paie$Année)

#'  
#'Cumul des lignes de paie par exercice et catégorie de ligne de paie   
#'  


for (i in 1:durée.sous.revue) {
  cat("\nTableau ", incrément(), " Année ", début.période.sous.revue + i - 1)
  print(kable(L[[i]][, .(Catégorie = Type, `Cumul annuel`)], row.names = FALSE, align = 'r'))

}

rm(L)

#'  
#'[Lien vers la base détaillée des cumuls des lignes de paie](`r currentDir`/Bases/Reglementation/cumul.lignes.paie.csv)
#'  
#'[Lien vers la base agrégée des cumuls des lignes de paie](`r currentDir`/Bases/Reglementation/cumul.total.lignes.paie.csv)
#'  

#'  
#'*Avertissement : les rappels comprennent également les rappels de cotisations et déductions diverses.*    
#'   

#### 5.9 SFT ####

#'
#'## `r chapitre`.9 Contrôle du supplément familial de traitement   
#'  

if (! utiliser.cplusplus.sft)
{
   source("sft.R", encoding = encodage.code.source)
   sft <- sft_R
   
} else {
  
  library(sft)
  sft <- function(Nb.Enfants,
                  Indice,
                  Echelon,
                  NBI,
                  Temps.de.travail,
                  Année,
                  Mois) .Call("sft_C",
                              Nb.Enfants,
                              Indice,
                              Echelon,
                              NBI,
                              Temps.de.travail,
                              Année,
                              Mois,
                              PACKAGE="sft")

}

Paie.sans.enfant.réduit <- Paie[is.na(Nb.Enfants) | Nb.Enfants == 0 , .(SFT.versé = sum(Montant[Type == "S"], na.rm = TRUE)), keyby = "Matricule,Année,Mois"] 

Paie.sans.enfant.réduit <- Paie.sans.enfant.réduit[SFT.versé > 0, ]

nb.écart.paiements.sft.sans.enfant <- nrow(Paie.sans.enfant.réduit)

if (nb.écart.paiements.sft.sans.enfant > 0){
  
  cat("\nPour les agents n'ayant pas d'enfant signalé en base, il a été détecté ",
      nb.écart.paiements.sft.sans.enfant,
      " bulletin", ifelse(nb.écart.paiements.sft.sans.enfant == 1, "", "s"),
      " de paie présentant un paiement du SFT apparemment anormal.\n", sep="")  
  
  if (afficher.table.écarts.sft)
    kable(Paie.sans.enfant.réduit, row.names = FALSE, align = 'c')
  
} else {
  
  cat("\nPour les agents n'ayant pas d'enfant signalé en base, il n'a été détecté aucun paiement de SFT.\n")
  
}

#'  
#'[Lien vers la base des paiements de SFT à agents sans enfant signalé](`r currentDir`/Bases/Reglementation/Paie.sans.enfant.réduit.csv)
#'  

if (!intégrer.échelon) {
  
  cat("Le contrôle du SFT pour les fonctionnaires hors échelle indiciaire requiert l'exportation de la variable échelon dans la base. Ce contrôle n'a pas pu être effectué.")
}

  if (!intégrer.échelon) {
    
    # pseudo échelon
    Paie.enfants.réduit <- Paie[!is.na(Nb.Enfants) & Nb.Enfants > 0 & !is.na(Indice) & !is.na(Heures),
                                .(SFT.versé = sum(Montant[Type == "S"], na.rm = TRUE), 
                                  #Traitement = sum(Montant[Type == "T"], na.rm = TRUE),
                                  Temps.de.travail = Temps.de.travail[1],
                                  Indice = Indice[1],
                                  Echelon = "",
                                  NBI = NBI[1],
                                  Nb.Enfants = Nb.Enfants[1]),
                                keyby="Matricule,Année,Mois"]
    
    
  } else {
    
    Paie.enfants.réduit <- Paie[!is.na(Nb.Enfants) & Nb.Enfants > 0 & !is.na(Indice) & !is.na(Heures),
                                .(SFT.versé = sum(Montant[Type == "S"], na.rm = TRUE), 
                                  #Traitement = sum(Montant[Type == "T"], na.rm = TRUE),
                                  Temps.de.travail = Temps.de.travail[1],
                                  Indice = Indice[1],
                                  Echelon = Echelon[1],
                                  NBI = NBI[1],
                                  Nb.Enfants = Nb.Enfants[1]),
                                keyby="Matricule,Année,Mois"]
    
  }
  
  SFT.controle <- with(Paie.enfants.réduit, 
                         mapply(sft, Nb.Enfants, Indice, Echelon, NBI, Temps.de.travail, Année, Mois, USE.NAMES = FALSE))
  
  Paie.enfants.réduit <- cbind(Paie.enfants.réduit, SFT.controle)
  
  Paie.enfants.réduit[ , delta.SFT := SFT.versé - SFT.controle]


## Attention ne pas intégrer au sein d'un même `:=`(...) deux définitions en coréférence avec if ... else 
# ou alors utiliser ifelse()  [bug de data.table]

# Paie.enfants.réduit[ , ecart := if (SFT.controle > 1) delta / SFT.controle else NA]

# On accepte un tolérance fixée dans prologue.R à tolérance.sft <- 1 euro

  controle.sft <- Paie.enfants.réduit[delta.SFT > tolérance.sft, 
                                        .(delta.SFT = round(delta.SFT, 2),
                                         SFT.versé,
                                         SFT.controle = round(SFT.controle, 2),
                                         Matricule, 
                                         Année,
                                         Mois,
                                         Indice,
                                         NBI,
                                         Temps.de.travail,
                                         Nb.Enfants)]

  nb.écart.paiements.sft <- nrow(controle.sft)
  
  if (nb.écart.paiements.sft) setorder(controle.sft, -delta.SFT, Matricule, Année, Mois)
  
  if (nb.écart.paiements.sft > 0){
      
    cat("\nPour les agents ayant au moins un enfant, il a été détecté ",
        nb.écart.paiements.sft,
        " bulletin", ifelse(nb.écart.paiements.sft == 1, "", "s"),
        " de paie présentant un écart de paiement du SFT supérieur à ", tolérance.sft, " euro.\n", sep="")  
  
    if (afficher.table.écarts.sft)
       kable(controle.sft, row.names = FALSE, align = 'c')
      
  } else {
    
    cat("\nPour les agents ayant au moins un enfant, il n'a été détecté aucun écart de paiement sur SFT supérieur à ", tolérance.sft, " euro.\n")
        
  }

#'  
#'[Lien vers la base des écarts de paiement sur SFT](`r currentDir`/Bases/Reglementation/controle.sft.csv)
#'  

message("Analyse du SFT")

# data.table here overallocates memory hence inefficient !
# Bulletins.paie[Nb.Enfants > 0 , SFT.controle := sft(Nb.Enfants, Indice, Heures, Année, Mois)]
       
#### ANNEXE ####
newpage()

#'# Annexe
#'## Liens complémentaires

#'  
#'## Codes et libellés de paye   
#'         

code_libellé <- unique(Paie[Montant != 0, .(Code, Libellé), by = "Type"], by = NULL)
code_libellé$Type <- sapply(code_libellé$Type,  function(x) switch(x,
                               "I" = "Indemnité",
                               "R" = "Rappels",
                               "IR"= "Indemnité de résidence",
                               "T" = "Traitement",
                               "AV"= "Avantage en nature",
                               "A" = "Autres rémunérations",
                               "C" = "Cotisations",
                               "D" = "Déductions",
                               "S" = "Supplément familial",
                               "RE"= "Retenues",
                               "C" = "Commentaire"))


setcolorder(code_libellé, c("Code", "Libellé", "Type"))
if (afficher.table.codes) {
   kable(code_libellé, align="c")
}

#'Certains libellés ou codes de paye peuvent être équivoques et entraîner des erreurs de requête.       
#'Les liens ci-après donnent les codes correspondant à au moins deux libellés distincts, les libellés correspondant à au moins deux codes et les codes ou libellés correspondant à au moins deux types de ligne de paye distincts.           
#'L'équivocité des codes est particulièrement préjudiciable lorsque les libellés correspondent à des types de ligne de paye distincts.    
#'
cl1 <- code_libellé[ , .(Multiplicité = .N, Libellé, Type), by = "Code"][Multiplicité > 1]
cl2 <- code_libellé[ , .(Multiplicité = .N,  Code, Type), by = "Libellé"][Multiplicité > 1]

cl3 <- unique(code_libellé[, .(Code, Type)], by = NULL)[ , .(Multiplicité = .N,  Type), by = "Code"][Multiplicité > 1]
cl4 <- unique(code_libellé[, .(Libellé, Type)], by = NULL)[ , .(Multiplicité = .N,  Type), by = "Libellé"][Multiplicité > 1]

#'   
#'[Lien vers la table Codes/Libellés](`r currentDir`/Bases/Fiabilite/code_libellé.csv)       
#'[Plusieurs libellés par code](`r currentDir`/Bases/Fiabilite/cl1.csv)   
#'[Plusieurs codes par libellé](`r currentDir`/Bases/Fiabilite/cl2.csv)   
#'[Plusieurs types de ligne par code](`r currentDir`/Bases/Fiabilite/cl3.csv)   
#'[Plusieurs types de ligne par libellé](`r currentDir`/Bases/Fiabilite/cl4.csv)           
#'   

#'  
#'## Doublons                
#'

if (éliminer.duplications) {
  if (après.redressement != avant.redressement) {
        
    cat("Retraitement de la base de lignes de paie : ")
  
  } else {
    cat("Aucune duplication de ligne de paie n'a été détectée. ")
  }
 
} else {
  
    if (anyDuplicated(Paie) || anyDuplicated(Bulletins.paie)) {
      cat("Attention : Altaïr a détecté des lignes dupliquées alors qu'aucun retraitement des lignes dupliquées n'est prévu par défaut.")
    } else {
      cat("Aucune duplication de ligne n'a été détectée. ")
    }
}

#'  
if (après.redressement != avant.redressement)
  cat("Elimination de ", FR(avant.redressement - après.redressement), " lignes dupliquées")
#'  
#'## Fiabilite des heures et des quotités de travail           
#'   


nrow.bull <- nrow(Bulletins.paie)
nrow.bull.heures <- nrow(Bulletins.paie[Heures != 0])
nrow.bull.quotités <- nrow(Bulletins.paie[Temps.de.travail != 0])

if (nrow.bull.heures/nrow.bull  < 0.1) 
  message("Attention moins de 10 % des heures sont renseignées")

if (nrow.bull.quotités/nrow.bull < 0.1)
  message("Attention moins de 10 % des quotités sont renseignées")
#'     
cat("Nombre de bulletins : ", FR(nrow.bull))
#'     
if (redresser.heures) {
  if (nredressements > 0) {
    cat("Les heures de travail ont été redressées avec la méthode ", ifelse(test.temps.complet, "des quotités.\n", "de l'interpolation indiciaire\n")) 
  }
} else {
   cat("Les heures de travail n'ont pas été redressées.")
}
#'    
cat(" Nombre de bulletins de paie redressés :", FR(nredressements)) 
#'    
cat(" Pourcentage de redressements :", round((nredressements)/nrow.bull*100, 2), "% des bulletins de paie.")
#'  
cat("\nPourcentage d'heures renseignées (après redressement éventuel):", round(nrow.bull.heures/nrow.bull*100, 1), "%")
#'   
cat("\nPourcentage de quotités renseignées :", round(nrow.bull.quotités/nrow.bull*100, 1), "%")
#'   
cat("\nNombre de bulletins à heures et quotités : ", n <- nrow(Bulletins.paie[Heures != 0 & Temps.de.travail != 0]), "[", round(n/nrow.bull*100, 1), "%]")
#'   
cat("\nNombre de bulletins à heures sans quotités : ", n <- nrow(Bulletins.paie[Heures != 0 & Temps.de.travail == 0]), "[", round(n/nrow.bull*100, 1), "%]")
#'   
cat("\nNombre de bulletins à quotités sans heures : ", n <- nrow(Bulletins.paie[Heures == 0 & Temps.de.travail != 0]), "[", round(n/nrow.bull*100, 1), "%]")
#'   
cat("\nNombre de bulletins apparemment inactifs : ", n <- nrow(Bulletins.paie[(Heures == 0 | is.na(Heures)) & (Temps.de.travail == 0 | is.na(Temps.de.travail))]), "[", round(n/nrow.bull*100, 1), "%]")  
#'   
base.heures.nulles.salaire.nonnull     <- Bulletins.paie[Heures == 0  & (Net.à.Payer != 0 | Brut != 0)]
base.quotité.indéfinie.salaire.nonnull <- Bulletins.paie[MHeures == 0 & (Net.à.Payer != 0 | Brut != 0)]

nligne.base.heures.nulles.salaire.nonnull     <- nrow(base.heures.nulles.salaire.nonnull)
nligne.base.quotité.indéfinie.salaire.nonnull <- nrow(base.quotité.indéfinie.salaire.nonnull)
#'  
if (nligne.base.heures.nulles.salaire.nonnull)
   cat("Nombre de bulletins de paie de salaires versés pour un champ Heures = 0 : ", FR(n <<- nligne.base.heures.nulles.salaire.nonnull), "[", round(n/nrow.bull * 100, 1), "%]")
#'   
if (nligne.base.quotité.indéfinie.salaire.nonnull)
   cat("\nNombre de bulletins de paie de salaires versés pour une quotité de travail indéfinie : ", FR(nligne.base.heures.nulles.salaire.nonnull))
#'   
#'[Lien vers la base de données des salaires versés pour Heures=0](`r currentDir`/Bases/Fiabilite/base.heures.nulles.salaire.nonnull.csv)   
#'[Lien vers la base de données des salaires versés à quotité indéfinie](`r currentDir`/Bases/Fiabilite/base.quotité.indéfinie.salaire.nonnull.csv)   
#'
#'# Tableau des personnels  
#'
#'
if (afficher.table.effectifs) {
  kable(grades.catégories, row.names = FALSE) 
} 

#'
#'[Lien vers la base des grades et catégories](`r currentDir`/Bases/Effectifs/grades.catégories.csv)        
#'   

#'
#'[Lien vers la base des personnels](`r currentDir`/Bases/Effectifs/matricules.csv)        
#'   


#'
#'# Divergences lignes-bulletins de paie     
#'   
#'*Pour exclure certains codes de paie de l'analyse, renseigner le fichier liste.exclusions.txt*  
#'   

if (test.delta) {
  if (!is.null(liste.exclusions))
    message("Une liste de codes exclus pour la vérification de la concordance lignes-bulletins de paie a été jointe sous ", getwd())
    cat("   ")
    source("delta.R", encoding=encodage.code.source)
} else {
  cat("Base de vérification des écarts lignes de paie-bulletins de paie non générée.")
}

  
#'   
#'[Divergences lignes-bulletins de paie](`r currentDir`/Bases/Fiabilite/Delta.csv)     
#'   


######### SAUVEGARDES #######

if (sauvegarder.bases.analyse) {

  sauv.bases(file.path(chemin.dossier.bases, "Remunerations"),
             "Analyse.rémunérations",
             "Analyse.variations.synthèse",
             "Analyse.variations.par.exercice",
             "masses.premier.personnels",
             "masses.premier.élus",
             "masses.dernier.personnels",
             "masses.dernier.élus",
             "bénéficiaires.PFR",
             "bénéficiaires.PFR.Variation")

  sauv.bases(file.path(chemin.dossier.bases, "Effectifs"),
             "matricules",
             "grades.catégories",
             "tableau.effectifs")

  sauv.bases(file.path(chemin.dossier.bases, "Reglementation"),
             "personnels.iat.ifts",
             "codes.ifts",
             "personnels.pfr.ifts",
             "codes.pfr",
             "HS.sup.25",
             "Dépassement.seuil.180h",
             "ifts.et.contractuel",
             "ihts.anormales",
             "lignes.contractuels.et.vacations",
             "lignes.fonctionnaires.et.vacations",
             "Paie_vac_contr",
             "Paie_vac_fonct",
             "Paie_vac_sft_ir",
             "lignes.ifts.anormales",
             "matricules.contractuels.et.vacations",
             "matricules.fonctionnaires.et.vacations",
             "SFT_IR.et.vacations",
             "matricules.SFT_IR.et.vacations",
             "NBI.aux.non.titulaires",
             "personnels.prime.informatique",
             "personnels.iat.ifts",
             "rémunérations.élu",
             "RI.et.vacations",
             "traitement.et.vacations",
             "cumul.lignes.paie",
             "cumul.total.lignes.paie",
             "controle.sft",
             "Paie.sans.enfant.réduit")
  
  sauv.bases(file.path(chemin.dossier.bases, "Fiabilite"),
              "base.heures.nulles.salaire.nonnull",
              "base.quotité.indéfinie.salaire.nonnull",
              "lignes.nbi.anormales",
              "cumuls.nbi",
              "cl1",
              "cl2",
              "cl3",
              "cl4",
              "code_libellé")
  
  if (test.delta) 
    sauv.bases(file.path(chemin.dossier.bases, "Fiabilite"), "Delta")
  
}

if (sauvegarder.bases.origine)
  sauv.bases(file.path(chemin.dossier.bases, "Paiements"),
             "Paie",
             "Bulletins.paie")


if (! générer.rapport)
   setwd(currentDir)

message(getwd())

