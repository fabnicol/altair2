# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
# produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
# en vigueur à compter de l'année 2008.
# 
# Ce logiciel est régi par la licence CeCILL soumise au droit français et
# respectant les principes de diffusion des logiciels libres. Vous pouvez
# utiliser, modifier et/ou redistribuer ce programme sous les conditions
# de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
# sur le site "http://www.cecill.info".
# 
# En contrepartie de l'accessibilité au code source et des droits de copie,
# de modification et de redistribution accordés par cette licence, il n'est
# offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
# seule une responsabilité restreinte pèse sur l'auteur du programme, le
# titulaire des droits patrimoniaux et les concédants successifs.
# 
# A cet égard l'attention de l'utilisateur est attirée sur les risques
# associés au chargement, à l'utilisation, à la modification et/ou au
# développement et à la reproduction du logiciel par l'utilisateur étant
# donné sa spécificité de logiciel libre, qui peut le rendre complexe à
# manipuler et qui le réserve donc à des développeurs et des professionnels
# avertis possédant des connaissances informatiques approfondies. Les
# utilisateurs sont donc invités à charger et tester l'adéquation du
# logiciel à leurs besoins dans des conditions permettant d'assurer la
# sécurité de leurs systèmes et ou de leurs données et, plus généralement,
# à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
# 
# Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
# pris connaissance de la licence CeCILL, et que vous en avez accepté les
# termes.
# 
# 
#'---    
#'title: false    
#'author: false    
#'date: false    
#'output:   
#'html_document:    
#'css: style.css
#'---     
#'   
#'![Image_Altair](altair.png)
#'   
#'   
#'## Logiciel Altaïr version `r readLines(file.path(currentDir, "VERSION"))`

# ---
# Encodage obligatoire en UTF-8
# ---

#+ echo = FALSE, warning = TRUE, message = FALSE

# comportement global du programme

# Lorsque l'on n'a que une ou deux années, mettre étudier.variations à FALSE
# Lorsque l'on n'étudie pas une base Xémélios, mettre étudier.tests.statutaires à FALSE

#+ début
  
  
library(compiler, warn.conflicts = FALSE)

invisible(setCompilerOptions(suppressAll = TRUE, optimize = 3))
invisible(enableJIT(0))

options(warn = -1, verbose = FALSE, OutDec = ",", datatable.verbose = FALSE, datatable.integer64 = "numeric")

source("bibliotheque.fonctions.paie.R", encoding = encodage.code.source)

source("import.R", encoding = encodage.code.source)

#'
#'### Employeur : `r controle[1]`      
#'### Siret : `r controle[2]`   
#'### Etablissement : `r controle[3]`   
#'### Budget : `r controle[4]`      
#'En cas de dysfonctionnement logiciel, signaler les difficultés rencontrées à l'aide de la [notice jointe](Docs/MANTIS-suivi des bogues.pdf).    
#'Logiciel sous licence [CeCILL v.2.1](Docs/LICENCE.html)     
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
#'## `r chapitre`.1 Effectifs

#+ effectifs

message("Démographie...")

# Rappel Analyse.variations.par.exercice comprend uniquement les actifs non annexes non assist. mat., non vacataires, non élus.

library(altair, warn.conflicts = FALSE)
tableau.effectifs <- effectifs(période, Bulletins.paie, Analyse.remunerations, Analyse.variations)

#'  
#  
#'&nbsp;*Tableau `r incrément()` : Effectifs*   
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
#'*(g) Postes actifs et non annexes :* voir [Compléments méthodologiques](Docs/méthodologie.pdf)    
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
#'[Lien vers la base des effectifs](Bases/Effectifs/tableau.effectifs.csv)
#'
#'

#+ pyramides-des-âges

message("Statistiques de démographie réalisées.")

e<-new.env()

# local = TRUE permet de conserver l'environnement e en sourçant

source("analyse.bulletins.R", local = TRUE, encoding = encodage.code.source)

newpage()

########### 1.2 Pyramides ########################


#'## `r chapitre`.2 Pyramide des âges, ensemble des personnels

#' 
#+fig.height=8, fig.width=7


  
essayer(produire_pyramides(NULL, 
                           "Pyramide des âges des personnels",
                           versant = VERSANT_FP),
        "La pyramide des âges de l'ensemble des personnels n'a pas pu être générée.")


newpage()

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

print(e$res)


#'  
#'[Lien vers la base des âges - début de période](Bases/Effectifs/`r e$nom.fichier.avant`.csv)  
#'  
#'[Lien vers la base des âges - fin de période](Bases/Effectifs/`r e$nom.fichier.après`.csv)  
#' 


newpage()
   
########### 1.3 Pyramides fonctionnaires ########################

#'
#'## `r chapitre`.3 Pyramide des âges des fonctionnaires  


#' 
#+fig.height=8, fig.width=7
essayer(produire_pyramides(c("TITULAIRE", "STAGIAIRE"), 
                           "Pyramide des âges des fonctionnaires",
                           versant = "TIT_" %+% VERSANT_FP),
      "La pyramide des âges des fonctionnaires n'a pas pu être générée.")

newpage()

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

print(e$res)

#'  
#'[Lien vers la base des âges - début de période](Bases/Effectifs/`r e$nom.fichier.avant`.csv)  
#'  
#'[Lien vers la base des âges - fin de période](Bases/Effectifs/`r e$nom.fichier.après`.csv)  
#'   

newpage()

########### 1.4 Pyramides non Tit ########################

#'## `r chapitre`.4 Pyramide des âges, personnels non titulaires   

#+fig.height=8, fig.width=7
essayer(produire_pyramides(c("NON_TITULAIRE"), "Pyramide des âges des non titulaires", 
                           versant = "NONTIT_" %+% VERSANT_FP),
        "La pyramide des âges des non titulaires n'a pas pu être générée." )

newpage()

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

print(e$res)


#'  
#'[Lien vers la base des âges - début de période](Bases/Effectifs/`r e$nom.fichier.avant`.csv)  
#'  
#'[Lien vers la base des âges - fin de période](Bases/Effectifs/`r e$nom.fichier.après`.csv)  
#'   

newpage()

########### 1.5 Pyramides Autres statut ########################

#'## `r chapitre`.5 Pyramide des âges, autres statuts


#' 
#+fig.height=8, fig.width=7
Filtre_bulletins <- setdiff(unique(Bulletins.paie$Statut), c("TITULAIRE", "NON_TITULAIRE", "STAGIAIRE")) 

essayer(produire_pyramides(Filtre_bulletins,
                           "Pyramide des âges des autres personnels"),
        "La pyramide des âges des autres personnels n'a pas pu être générée.")

newpage()

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

print(e$res)

#'  
#'[Lien vers la base des âges - début de période](Bases/Effectifs/`r e$nom.fichier.avant`.csv)  
#'  
#'[Lien vers la base des âges - fin de période](Bases/Effectifs/`r e$nom.fichier.après`.csv)  
#' 
#'  
#'*Source des comparaisons avec les données nationales*      
#'         
#'Rapport annuel sur l'état de la fonction publique pour 2016      
#'[Pyramide 2013 FPH](Docs/insee_pyramide_fph_2013.csv)   
#'[Pyramide 2013 FPT](Docs/insee_pyramide_fpt_2013.csv)     


#'*Toutes les pyramides des âges sont établies au 31 décembre de l'année considérée.*   
#'*Les élus ne sont pas compris dans le périmètre statistique.*     
	   
newpage()

########### 1.6 Effectifs par durée ########################

#'## `r chapitre`.6 Effectifs des personnels par durée de service
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
                            "acomptes",
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

                   source('analyse.statique.R', encoding = "UTF-8") 
                   
                 } else {
                   if (setOSWindows)  {                 
                      cat(knit_child(text = readLines(file.path(chemin.dossier,'analyse.statique.Rmd'), encoding = encodage.code.source), quiet = TRUE), sep = '\n')
                   } else {
                     cat(knit_child(text = readLines(file.path(chemin.dossier,'analyse.statique.utf8.Rmd'), encoding = "UTF-8"), quiet = TRUE), sep = '\n')
                   }
                 }
               }))

#'  
#'[Lien vers la base des rémunérations](Bases/Remunerations/Analyse.remunerations.csv)  
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


#'**Eléments de la rémunération brute pour les titulaires de la fonction publique territoriale**      
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
#'[Source INSEE](Docs/ip1486.xls)    
#'[Source DGCL](Docs/Vue3_1_Remunerations.xls)    
#'[Source RAEFP 2015](Docs/RA_2015.pdf)   
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

attach(Analyse.variations, warn.conflicts = FALSE)

temp <- positive(moyenne.rémunération.annuelle.sur.période) / 1000

essayer({
  if (longueur.non.na(temp) > 0)
    hist(temp,
         xlab = "Sur la période " %+% début.période.sous.revue %+% "-" %+% fin.période.sous.revue %+% " en milliers d'euros",
         ylab = "Effectif",
         main = "Rémunération nette moyenne",
         col = "blue",
         nclass = 200)
}, "Le graphique des rémunérations moyennes n'a pas pu être généré")

#'    
#+ fig.height=4.5   

temp <- na.omit(moyenne.rémunération.annuelle.sur.période[moyenne.rémunération.annuelle.sur.période > minimum.positif
                                                          & (statut == "TITULAIRE"  | statut == "STAGIAIRE")] / 1000)

essayer({
if (longueur.non.na(temp) > 0)
  hist(temp,
       xlab = "Sur la période "%+% début.période.sous.revue %+% "-" %+% fin.période.sous.revue %+% " en milliers d'euros",
       ylab = "Effectif",
       main = "Rémunération nette moyenne des fonctionnaires",
       col = "blue",
       nclass = 200)

}, "Le graphique des rémunérations moyennes des fonctionnaires n'a pas pu être généré")

#'   
#'[Lien vers la base de données synthétique](Bases/Remunerations/Analyse.variations.par.exercice.csv)
#'[Lien vers la base de données détaillée par année](Bases/Remunerations/Analyse.variations.par.exercice.csv)  

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

essayer(smpt(Filtre_neutre),     "Le salaire moyen par tête n'a pas pu être calculé.")

#'   
#+ Effet-de-noria-ensemble
#source("noria.R", encoding = encodage.code.source)
#noria()

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

essayer(distribution_smpt(Filtre_neutre),        "La distribution du salaire moyen par tête n'a pas pu être calculée.")

#'  
#'*Nota :*  La population retenue est constituée des agents qui ne font pas partie des `r 2*quantile.cut` centiles extrêmaux   
#'Les élus, vacataires et assistantes maternelles sont retirés du périmètre.   
#'TC :  personnels à temps complet sur toute l'année            
#'Seuls sont pris en compte les agents ayant connu au moins un mois actif et ayant eu, sur l'année, des rémunérations non annexes.  
#'[Compléments méthodologiques](Docs/méthodologie.pdf)     
#'      


#'**Comparaisons source INSEE/DGCL**   
#'
#'**Salaires nets annuels moyens 2011 à 2013 en EQTP (hors assistantes maternelles)**   
#'  
#'&nbsp;*Tableau `r incrément()`*       

#### INSEE/DGCL VARIATIONS  ####
#'  
#'  |  net (euros)    | 2011    | 2012   |  2013  |  2014  |     
#'  |-----------------|--------:|-------:|-------:|-------:|   
#'  |    Ensemble     |  21 876 | 22 176 | 22 224 | 22 524 |  
#'  |   Titulaires    |  22 632 | 22 920 | 22 920 | 23 424 |  
#'  | Autres salariés |  18 864 |  NA    |  NA    | 18 732 |   
#' 
#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			

#'   
#'**Distribution des salaires nets annuels en EQTP dans la fonction publique territoriale (2011-2014)**   
#' 
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    
# Remarque sur le formatage markdown: en raison d'un bug de knitr, utiliser un dièse + apostrophe
# suivi d'un seul blanc juste après la table.

#'  
#' | Décile \ euros   | 2011     | 2013   | 2014   |  
#' |------------------|----------|--------|--------|   
#' |    D1            | 15 288   | 15 600 | 15 768 | 
#' |    D2            | 16 512   | 16 860 | 17 124 |   
#' |    D3            | 17 508   | 17 844 | 18 156 | 
#' |    D4            | 18 480   | 18 816 | 19 164 | 
#' |    D5 (médiane)  | 19 632   | 19 908 | 20 256 |    
#' |    D6            | 21 012   | 21 300 | 21 648 |  
#' |    D7            | 22 860   | 23 160 | 23 496 |  
#' |    D8            | 25 596   | 25 956 | 26 292 |  
#' |    D9            | 30 876   | 31 272 | 31 596 |  
#' |    Moyenne       | 21 876   | 22 212 | 22 524 |  
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
    
#'[Source INSEE, onglets Figure3, F1web et F3web - 2011](Docs/ip1486.xls)    
#'[Source INSEE, onglets F V3.1-2, F V3.1-5 - 2013](Docs/vue3_remunerations.xls)     
#'[Source INSEE 2016](Docs/insee-premiere1616.pdf)    
 
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

essayer(smpt(Filtre_fonctionnaire), "Le salaire moyen par tête des fonctionnaires n'a pas pu être calculé.")

Filtre_cat_A <- function()   (Statut == "TITULAIRE"  | Statut == "STAGIAIRE")  & (Catégorie == "A")
Filtre_cat_B <- function()   (Statut == "TITULAIRE"  | Statut == "STAGIAIRE")  & (Catégorie == "B") 
Filtre_cat_C <- function()   (Statut == "TITULAIRE"  | Statut == "STAGIAIRE")  & (Catégorie == "C") 

#'   
#'**Catégorie A**  
#'
#'&nbsp;*Tableau `r incrément()`*   
#'    
#'  


essayer(smpt(Filtre_cat_A), "Le salaire moyen par tête des fonctionnaires de catégorie A n'a pas pu être calculé.")  

#'**Distribution des salaires nets annuels en EQTP dans la fonction publique territoriale par catégorie (2011-2013)**   
#'  
#'*Comparaisons nationales*    
#'    
#' | Décile \ euros   | 2011     | 2013   |   2014   |  
#' |------------------|----------|--------|----------|   
#' |    D1            | 26 040   | 26 340 |  26 460  |   
#' |    D2            | 28 992   |        |          |   
#' |    D3            | 31 272   |        |          |   
#' |    D4            | 33 468   |        |          |   
#' |    D5 (médiane)  | 35 820   | 36 312 |  36 580  |    
#' |    D6            | 38 664   |        |          |
#' |    D7            | 42 276   |        |          |    
#' |    D8            | 47 124   |        |          |
#' |    D9            | 54 840   | 55 032 |  55 440  |       
#' |  Moyenne         | 38 700   | 39 120 |  39 360  |  
#' 

#'   
#'**Catégorie B**  
#'
#'&nbsp;*Tableau `r incrément()`*   
#'    
#'  

essayer(smpt(Filtre_cat_B),          "Le salaire moyen par tête des fonctionnaires de catégorie B n'a pas pu être calculé.")  


#'*Comparaisons nationales*    
#'    
#' | Décile \ euros   | 2011     | 2013   |  2014   |    
#' |------------------|----------|--------|---------|   
#' |    D1            | 20 580   | 20 964 |  21 108 |   
#' |    D2            | 22 272   |        |         |   
#' |    D3            | 23 652   |        |         |   
#' |    D4            | 24 960   |        |         |   
#' |    D5 (médiane)  | 26 244   | 26 820 |  27 000 |     
#' |    D6            | 27 636   |        |         |   
#' |    D7            | 29 160   |        |         |    
#' |    D8            | 30 984   |        |         |   
#' |    D9            | 33 804   | 34 224 | 34 344  |   
#' |  Moyenne         | 26 940   | 27 408 | 27 588  |  
#' 

#'   
#'**Catégorie C**  
#'
#'&nbsp;*Tableau `r incrément()`*    
#'    
#'  

essayer(smpt(Filtre_cat_C),       "Le salaire moyen par tête des fonctionnaires de catégorie C n'a pas pu être calculé.")    

#'     
#'*Comparaisons nationales*    
#' 
#' | Décile \ euros   | 2011     | 2013   |  2014  | 
#' |------------------|----------|--------|--------|
#' |    D1            | 15 972   |  16 296| 16 632 |
#' |    D2            | 16 896   |        |        |
#' |    D3            | 17 652   |        |        |
#' |    D4            | 18 360   |        |        |
#' |    D5 (médiane)  | 19 164   |  19 464| 19 884 |
#' |    D6            | 20 100   |        |        |
#' |    D7            | 21 216   |        |        |
#' |    D8            | 22 680   |        |        |
#' |    D9            | 24 996   |  25 176| 25 608 |
#' |    Moyenne       | 20 016   |  20 268| 20 676 |
#' 



#'   
#'**Effet de noria sur salaires nets et taux de remplacements**       
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


#'     
#'*MS N-1 : masse salariale nette de l'année n-1.*   
#'       
#'**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         


#'**Fonctionnaires**    
#'&nbsp;*Tableau `r incrément()`*   
#'    

essayer(distribution_smpt(Filtre_fonctionnaire), "La distribution du salaire moyen par tête des fonctionnaires n'a pas pu être calculée.")

#'**Catégorie A**    
#'&nbsp;*Tableau `r incrément()`*   
#'    

essayer(distribution_smpt(Filtre_cat_A), "La distribution du salaire moyen par tête des fonctionnaires de catégorie A n'a pas pu être calculée.")

#'**Catégorie B**  
#'&nbsp;*Tableau `r incrément()`*   
#'    

essayer(distribution_smpt(Filtre_cat_B), "La distribution du salaire moyen par tête des fonctionnaires de catégorie B n'a pas pu être calculée.")

#'**Catégorie C**  
#'&nbsp;*Tableau `r incrément()`*   
#'    

essayer(distribution_smpt(Filtre_cat_C), "La distribution du salaire moyen par tête des fonctionnaires de catégorie C n'a pas pu être calculée.")

#'[Lien vers la base de données](Bases/Remunerations/Analyse.variations.par.exercice.csv)     

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

Anavar.synthese <- Analyse.variations[total.jours > 2 * seuil.troncature
                                                           & pris.en.compte == TRUE
                                                           & ! is.na(statut)   
                                                           & ! is.na(variation.rémunération) 
                                                           & variation.rémunération > q3[[1]]
                                                           & variation.rémunération < q3[[2]]]

Anavar.synthese.plus.2.ans  <- Anavar.synthese[! is.na(plus.2.ans) & plus.2.ans == TRUE]
Anavar.synthese.moins.2.ans <- Anavar.synthese[! is.na(plus.2.ans) & plus.2.ans == FALSE]

#Analyse.variations.par.exercice <- Analyse.variations.par.exercice[Nexercices > 1]

if (nrow(Anavar.synthese.plus.2.ans) > 0 && durée.sous.revue > 1 ) {
  hist(Anavar.synthese.plus.2.ans$variation.moyenne.rémunération,
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
   essayer(smpt(Filtre_rmpp, type = "RMPP nette"), "La rémunération moyenne des personnels en place n'a pas pu être calculée.") 
  } else  {
   cat("RMPP calculable uniquement si la période sous revue est au moins égale à 2 ans.")
  }

#'    
#'**Distribution et variation sur la période de la rémunération nette des personnes en place**                
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

# La légère différence de pérmètre entre Anavar.synthese et Analyse.variations.par.exercice tient au filtrage des quantiles
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
         list(Anavar.synthese[bitwAnd(indicatrice.période, masque.rmpp.début.période) == masque.rmpp.début.période 
                                          & Année == début.période.sous.revue + 1, 
                                             .(Montant.net.annuel.eqtp.début, quotité.moyenne)],
              Anavar.synthese[indicatrice.période >= masque.rmpp.fin.période
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
           Anavar.synthese[bitwAnd(indicatrice.période, masque.présent.début.fin) 
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
#        Anavar.synthese[indicatrice.période == masque.présent.sur.période, variation.rémunération.normalisée],
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
    essayer(smpt(Filtre_rmpp_fonctionnaire, type = "RMPP nette"), "La RMPP nette des fonctionnaires n'a pas pu être calculée")

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
         list(Anavar.synthese[(statut == "TITULAIRE" | statut == "STAGIAIRE")
                                           & bitwAnd(indicatrice.période, masque.rmpp.début.période) == masque.rmpp.début.période
                                           & Année == début.période.sous.revue + 1,  
                                          .(Montant.net.annuel.eqtp.début, quotité.moyenne)],
              Anavar.synthese[(statut == "TITULAIRE" | statut == "STAGIAIRE")
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
         Anavar.synthese[(statut == "TITULAIRE" | statut == "STAGIAIRE")
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
#'[Lien vers la base de données](Bases/Remunerations/Anavar.synthese.csv)
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

#'[Lien vers la base de données](Bases/Remunerations/Anavar.synthese.csv)
#'
#'
#'### `r chapitre`.4 Comparaisons avec la situation nationale des rémunérations   
#'  
#'**Évolution en euros courants du SMPT et de la RMPP dans la FPT (en % et euros courants)**    

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

#' 
#'| Année      | 2009 | 2010 | 2011 | 2012 | 2013 | 2014 | 
#'|:----------:|-----:|-----:|-----:|-----:|-----:|-----:|
#'| SMPT brut  | 2,5  |  1,3 |  1,5 |  1,7 |  1,1 |  1,7 |
#'| SMPT net   | 3,0  |  1,4 |  1,3 |  1,4 |  0,8 |  1,3 |
#'| RMPP brute | 3,3  |  2,5 |  2,5 |  2,7 |  1,9 |  3,0 |
#'| RMPP nette | 3,3  |  2,5 |  2,3 |  2,4 |  1,6 |  2,7 |
#' 

#'*Source : fichier général de l'État (FGE), DADS, SIASP, Insee, Drees. Traitement Insee, Drees, DGCL*    
#'Hors assistants maternels et familiaux, y compris bénéficiaires de contrats aidés.   
#'Lecture : en 2014, le SMPT brut en EQTP a augmenté de 1,7 %
#'SMPT : Salaire moyen par tête en EQTP.   
#'RMPP : Agents présents 24 mois consécutifs chez le même employeur avec la même quotité de travail.   
#'Lecture : en 2014, la rémunération nette en EQTP des agents présents deux années consécutives en 2012 et 2013 avec la même quotité a augmenté de 2,7 %   
#'  
#'**Salaires nets annuels et évolution moyenne type de collectivité en euros courants  EQTP**    
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


#' 
#'|  Organisme   SMPT net     |  2011  | 2012     |    2013 |  2014  | 2007-2011 (%) | 2011-2014 (%) | 
#'|:-------------------------:|-------:|---------:|--------:|-------:|----:|-----:|       
#'| Communes                  | 20 784 |  21 120  | 21 096  | 21 444 | 2,5 |  3,2 | 
#'| CCAS et caisses des écoles| 19 415 |  19 716  | 19 788  | 20 124 | 2,4 |  3,7 |
#'| EPCI à fiscalité propre   | 22 882 |  23 088  | 23 184  | 23 412 | 3,1 |  2,3 |
#'| Autres structures intercommunales |   21 299 | 21 684 | 21 828 | 22 140 | 3,0 | 3,9  |
#'|   Départements            | 24 487 |  24 744  | 24 852  | 25 068 | 3,9 |  2,4 |
#'|   SDIS                    | 29 811 |  29 940  | 30 180  | 30 480 | 3,4 |  2,2 |
#'|  Régions                  | 22 432 |  22 836  | 23 004  | 23 484 | 3,8 |  4,7 |
#'| Autres collectivités locales  | 24 680  | 24 696  | 24 828  | 25 032 | 3,2 | 1,4 |
#'| Ensemble (moyenne)        | 21 873 | 22 176   | 22 212  | 22 524 | 2,9 |  3,0 |
#' 

#'**RMPP nette 2014 (salariés présents en 2013 et 2014 avec la même quotité) en EQTP**        
#'    
#' 
#'|  Organisme      RMPP net          |  2014  | 
#'|:---------------------------------:|-------:|
#'| Communes                          | 22 524 |
#'| CCAS et caisses des écoles        | 21 420 |
#'| EPCI à fiscalité propre           | 24 864 |
#'| Autres structures intercommunales | 23 988 |
#'|   Départements                    | 25 932 |
#'|   SDIS                            | 31 032 |
#'|  Régions                          | 24 240 |
#'| Autres collectivités locales      | 21 873 |
#'|  Ensemble (moyenne)               | 23 760 |
#' 
#'   
#'  
#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			
#'Conversion en euros courants, calcul CRC.  

#'[Source RAEFP 2013 données 2011](Docs/RA_2013.pdf)   
#'[Source RAEFP 2014 données 2012](Docs/RA_2014.pdf)     
#'[Source RAEFP 2015 données 2013](Docs/RA_2015.pdf)     
#'[Source RAEFP 2016 données 2014](Docs/RA_2016.pdf)      
#'[Source INSEE 2016](Docs/insee-premiere1616.pdf)      

incrémenter.chapitre()

newpage()

#'
########### 5. TESTS STATUTAIRES ########################
#'
#'   
#'**Dans cette partie, l'ensemble de la base de paie est étudié.**  
#'Les agents non actifs ou dont le poste est annexe sont réintroduits dans le périmètre.   

#### 5.1 NBI ####

#'# `r chapitre`. Tests réglementaires   
#'## `r chapitre`.1 Contrôle des nouvelles bonifications indiciaires (NBI) 

#+ tests-statutaires-nbi

# --- Test NBI accordée aux non titulaires
#     Filtre    : Statut != "TITULAIRE" & Statut != "STAGIAIRE" & NBI != 0  grepl(expression.rég.nbi, Libellé)

colonnes <-  c("Matricule",
               "Statut",
               "Code",
               "Libellé",
               "Année",
               "Mois",
               "Montant")

Paie_NBI <- filtrer_Paie("NBI")[Type %chin% c("T", "I", "R")]

NBI.aux.non.titulaires <- Paie_NBI[Statut != "TITULAIRE" & Statut != "STAGIAIRE" & NBI != 0,
                                         c(colonnes, "NBI"),
                                             with = FALSE] 
           
if (nombre.personnels.nbi.nontit <- uniqueN(NBI.aux.non.titulaires$Matricule)) {
    cat("Il existe ", 
      FR(nombre.personnels.nbi.nontit),
      "non titulaire" %s% nombre.personnels.nbi.nontit,
      " percevant une NBI.")
} else {
    cat("Pas de NBI apparemment attribuée à des non-titulaires. ")
}


#'   
#'[Lien vers la base de données NBI aux non titulaires](Bases/Reglementation/NBI.aux.non.titulaires.csv)   
#'   

# On calcule tout d'abord la somme de points de NBI par matricule et par année

T1 <- Bulletins.paie[! is.na(quotité) & quotité > 0, 
                        .(nbi.cumul.indiciaire = sum(NBI, na.rm = TRUE)),
                            by="Matricule,Année"
                    ][nbi.cumul.indiciaire > 0] 

if (nrow(T1) == 0) cat("Aucune NBI n'a été attribuée. ")

# On calcule ensuite, sur les traitements et éventuellement les indemnités, la somme des paiements au titre de la NBI, par matricule et par année
# Attention ne pas prendre en compte les déductions, retenues et cotisations, ni les rappels. On compare en effet les payements bruts de base à la somme des points x valeur du point

# La quotité ici considérée est non pas la quotité statistique mais la quotité admistrative plus favorable aux temps partiels.

adm <- function(quotité) ifelse(quotité == 0.8,  6/7, ifelse (quotité == 0.9,  32/35, quotité))

T2 <- Paie_NBI[Type %chin% c("T", "I")
                  & ! is.na(NBI)
                  & ! is.na(quotité)
                  & quotité > 0
                  & NBI != 0, 
                      nbi.cumul.montants := sum(Montant / adm(quotité), na.rm=TRUE), 
                           by="Matricule,Année"][nbi.cumul.montants != 0]

T <- merge(T1, 
           T2[ , .(Matricule, Année, Mois, nbi.cumul.montants, Catégorie)], 
           by=c("Matricule", "Année"))

# On somme ensuite par année sur tous les matricules

cumuls.nbi <- T[ , .(cumul.annuel.indiciaire = sum(nbi.cumul.indiciaire, na.rm = TRUE),
                     cumul.annuel.montants   = sum(nbi.cumul.montants, na.rm = TRUE)),
                       keyby="Année"]

if (nrow(cumuls.nbi) == 0) cat("Cumuls de NBI nuls. ")

# Les cumuls annuels rapportés cumuls indiciaires pour l'année ne doivent pas trop s'écarter de la valeur annuelle moyenne du point d'indice

lignes.nbi.anormales <- T[abs(abs(nbi.cumul.montants/nbi.cumul.indiciaire) - valeur.point.inm.pivot[Année - 2007]) > 1, 
                            .(Matricule, Année, nbi.cumul.indiciaire, nbi.cumul.montants)]

montants.nbi.anormales <- sum(lignes.nbi.anormales$nbi.cumul.montants, na.rm = TRUE)

#'  
#'&nbsp;*Tableau `r incrément()` : Rémunérations de NBI globalement anormales*    
#'    

Tableau(
  c("Rémunérations de NBI globalement anormales",
    "Montants correspondants"),
  nrow(lignes.nbi.anormales),
  round(montants.nbi.anormales, 1))

#'       
#'[Lien vers la base de données NBI anormales](Bases/Fiabilite/lignes.nbi.anormales.csv)     
#'   
#'**Nota :**   
#'*Est considéré comme manifestement anormal un total annuel de rémunérations NBI correspondant à un point d'indice net mensuel inférieur à la moyenne de l'année moins 1 euro ou supérieur à cette moyenne plus 1 euro.*    
#'*Les rappels ne sont pas pris en compte dans les montants versés. Certains écarts peuvent être régularisés en les prenant en compte*     
#'  
#'    
#'&nbsp;*Tableau `r incrément()` : Contrôle global de la liquidation des NBI*    
#'    

with(cumuls.nbi,

Tableau.vertical2(c("Année", "Cumuls des NBI", "Montants versés (a)", "Point d'INM apparent", "Point d'INM moyen", "Contrôle"), 
                  Année, 
                  cumul.annuel.indiciaire,
                  cumul.annuel.montants,
                  a <- cumul.annuel.montants/cumul.annuel.indiciaire,
                  b <- PointMensuelIMMoyen[Année - 2007],
                  ifelse(abs(b - a) > 0.3, "Rouge", ifelse(abs(b - a) > 0.15, "Orange", "Vert")))

)

#'   
#'*(a) Hors rappels sur rémunérations*   
#'  

#'   
#'[Lien vers la base de données des cumuls annuels de NBI](Bases/Fiabilite/cumuls.nbi.csv)   
#'   



# --- Test Proratisation NBI

#'  
#'&nbsp;*Tableau `r incrément()` : Contrôle de proratisation/liquidation de la NBI*        
#'  

# Calcul plus exact de liquidation

montants.nbi.anormales.mensuel <- 0
lignes.nbi.anormales.mensuel <- data.table()

essayer(
{  
  lignes.nbi.anormales.mensuel <- T2[ , .(Montant.NBI.calculé = NBI[1] * adm(quotité[1]) * PointMensuelIM[Année - 2007, Mois],
                                          Montant.NBI.payé = sum(Montant, na.rm = TRUE)), 
                                             keyby="Matricule,Année,Mois"
                                       ][ , Différence.payé.calculé := Montant.NBI.payé - Montant.NBI.calculé
                                       ][abs(Différence.payé.calculé) > tolérance.nbi]
  
  
  lignes.paie.nbi.anormales.mensuel <- merge(Paie[Type == "T" | Type == "I", .(Matricule, Année, Mois, Statut, 
                                                                                Grade, Echelon, Catégorie, 
                                                                                Emploi, Service, quotité,
                                                                                NBI, Code, Libellé,
                                                                                Base, Taux,Type, Montant)],
                                                  lignes.nbi.anormales.mensuel,
                                                  by=c("Matricule", "Année", "Mois"))
  
  nb.lignes.anormales.mensuel    <- nrow(lignes.nbi.anormales.mensuel)
  montants.nbi.anormales.mensuel <- lignes.nbi.anormales.mensuel[, sum(Différence.payé.calculé)]
},
"La vérification de la proratisation de la NBI n'a pas pu être réalisée. ")

Tableau(
  c("Différences > 1 euro : nombre de lignes",
    "Coût total des différences"),
  nrow(lignes.nbi.anormales.mensuel),
  round(montants.nbi.anormales.mensuel))

#'   
#'[Lien vers les bulletins anormaux du contrôle de proratisation/liquidation de la NBI](Bases/Fiabilite/lignes.nbi.anormales.mensuel.csv)   
#'   
#'   
#'[Lien vers les lignes de paye du contrôle de proratisation/liquidation de la NBI](Bases/Fiabilite/lignes.paie.nbi.anormales.mensuel.csv)   
#'   

# --- Test Catégorie statutaire et points de NBI

setkey(Bulletins.paie, Catégorie, Matricule, Année, Mois)

NBI.cat <- Bulletins.paie[! is.na(NBI) & NBI > 0, .(Matricule, Année, Mois, Catégorie, NBI, quotité, Emploi, Grade)]

NBI.cat[ , Contrôle := { a <- grepl("d(?:\\.|ir)\\w*\\s*\\bg(?:\\.|\\w*n)\\w*\\s*\\bs\\w*", paste(Emploi, Grade), ignore.case = TRUE, perl = TRUE)
                         b <- grepl("d(?:\\.ir)\\w*\\s*\\bg(?:\\.|\\w*n)\\w*\\s*\\ba(?:\\.|d)\\w*", paste(Emploi, Grade), ignore.case = TRUE, perl = TRUE)
                         ifelse ((NBI > 20 & Catégorie == "C")
                                 | (NBI > 30 & Catégorie == "B")
                                 | (NBI > 50 
                                    & Catégorie == "A" 
                                    & ! a 
                                    & ! b)
                                 | (NBI > 80 & b & !a)
                                 | (NBI > 120 & a), "Rouge", "Vert")}]

# Si la quotité est inconnue on la suppose égale à 1 (rare) pour l'évaluation du coût

NBI.cat.irreg <- NBI.cat[Contrôle == "Rouge", 
                             Coût := { a <- adm(quotité)
                                      (NBI - ifelse(Catégorie == "A", 50, ifelse(Catégorie == "B", 30, 20))) *
                                            PointMensuelIM[Année - 2007, Mois] * ifelse(is.na(a), 1, a)}
                        ][! is.na(Coût)][ , Contrôle := NULL] 
  
nombre.mat.NBI.irrég <- NBI.cat.irreg[ , uniqueN(Matricule)]
coût.total <- NBI.cat.irreg[ , sum(Coût, na.rm = TRUE)]

#'  
#'&nbsp;*Tableau `r incrément()` : Contrôle d'attribution de NBI par catégorie statutaire*        
#'  

Tableau(
  c("Nombre d'agents concernés",
    "Coût total des dépassements"),
  nombre.mat.NBI.irrég,
  round(coût.total, 1))

#'   
#'**Nota :**   
#'Coût annuel calculé pour la quotité de travail observée, limité aux dépassements des maxima ci-après.   
#'Dépassements de NBI :     
#' - plus de 50 points pour la catégorie A;   
#' - plus de 30 points pour la catégorie B;   
#' - plus de 20 points pour la catégorie C.     
#'Directeurs généraux adjoints : plus de 80 points.   
#'Directeurs généraux adjoints : plus de 120 points.           
#'  

#'   
#'[Lien vers les NBI dépassant les seuils par catégorie statutaire](Bases/Reglementation/NBI.cat.irreg.csv)   
#'   

rm(T, T1, T2, NBI.cat, NBI.cat.irrég)

#### 5.2 PFI ####

# --- Test Prime de fonctions informatiques
#     Filtre    : filtre expression rationnelle expression.rég.pfi dans Libellé.   

#'  
#'## `r chapitre`.2 Contrôle de la prime de fonctions informatiques (PFI)   

Matrice.PFI <- filtrer_Paie("PFI")

personnels.prime.informatique <- Matrice.PFI[ , ..colonnes]
  
if (nombre.personnels.pfi <- uniqueN(personnels.prime.informatique$Matricule)) {
  
  cat("Il existe ", 
      FR(nombre.personnels.pfi),
      "agent" %s% nombre.personnels.pfi,
      " percevant une PFI.")
}

primes.informatiques.potentielles <- if (nombre.personnels.pfi == 0) "aucune" else paste(unique(personnels.prime.informatique$Libellé), collpase = " ;")

#'Primes informatiques (PFI) : `r primes.informatiques.potentielles`    
#'  

#'   
#'[Lien vers la base de données Primes informatiques](Bases/Reglementation/personnels.prime.informatique.csv)   
#'   



#### 5.3 VACATIONS ####
#'  
#'## `r chapitre`.3 Contrôle des vacations horaires pour les fonctionnaires      

# Vacations et statut de fonctionnaire

#'Les fonctionnaires peuvent effectuer des vacations horaires pour leur propre employeur à condition de bénéficier d'une autorisation
#'de cumul d'activité accessoire et que les activités concernées ne fassent pas partie du service normal. Les cumuls détectés ci-dessous
#'se limitent aux cas de vacations horaires détectées. L'existence des pièces justificatives pourra être recherchée.

# ----- Trouver, pour toutes les lignes de paie non-charges, une indication de proportionnalité horaire explicite
#       et retenir les variables de calcul du montant et du statut pour les mois concernés par l'existence d'au moins
#       une de ces lignes horaires, assimilée à un paiement de vacation.

  
   Paie_vac <- filtrer_Paie("VAC", portée = "Mois")

   Paie_vac <- Paie_vac[Type %chin% c("T", "I", "R", "IR", "S")]

# ----- Produire la liste de ces libellés horaires

   if (is.na(codes.vacataires)) {
      libellés.horaires <- unique(Paie_vac[grepl(expression.rég.vacataires, Libellé, ignore.case = TRUE, perl = TRUE), Libellé])
   } else {
      libellés.horaires <- unique(Paie_vac[Code %chin% codes.vacataires, Libellé])
   }
   

# ----- Vérifier si des fonctionnaires titulaires ou stagiaires bénéficient de vacations horaires et donner les caractéristiques

Paie_vac_fonct <- Paie_vac[Statut %chin% c("TITULAIRE", "STAGIAIRE"), 
                                               .(Nom, Statut, Code, Libellé, Type, Taux, Nb.Unité, Montant, Total.mensuel = sum(Montant, na.rm = TRUE)),
                           by = .(Matricule, Année, Mois)]

lignes.fonctionnaires.et.vacations <- Paie_vac_fonct[Libellé %chin% libellés.horaires]

matricules.fonctionnaires.et.vacations <- unique(lignes.fonctionnaires.et.vacations[ , .(Matricule, Nom, Statut)], by = NULL)
nombre.fonctionnaires.et.vacations <- length(matricules.fonctionnaires.et.vacations[[1]])

if (nombre.fonctionnaires.et.vacations > 0) {
  cat("Il y a ",
      FR(nombre.fonctionnaires.et.vacations),
      "fonctionnaire(s) effectuant des vacations pour son propre établissement. Les bulletins concernés sont donnés en lien. " )
}  else  {
  cat("Pas de vacation détectée. ")
}

#'[Matricules des fonctionnaires concernés](Bases/Reglementation/matricules.fonctionnaires.et.vacations.csv)       
#'[Lien vers les vacations payées à des fonctionnaires](Bases/Reglementation/lignes.fonctionnaires.et.vacations.csv)       
#'[Lien vers les bulletins de paie correspondants](Bases/Reglementation/Paie_vac_fonct.csv)            

####  5.4 CEV ####  
  
#'
#'## `r chapitre`.4 Contrôles sur les contractuels effectuant des vacations horaires    

#+ tests-statutaires-vacations-ri

#'Les vacataires rémunérés à la vacation horaire n'ont, en principe, pas accès au régime indemnitaire dont bénéficient les titulaires et non-titulaires 
#'sauf si l'assemblée délibérante a explicitement prévu de déterminer le taux des vacations horaires par référence à ces régimes.
#'Les vacataires bénéficiant d'une référence de type indemnitaire perçoivent ainsi parfois des lignes de rémunération
#'identifiées, par abus de codage en base de paye, comme indemnitaires (*catégorie "Indemnite" des bases XML et type "I" des bases CSV*).      
#'Cette situation n'est régulière que s'il s'agit d'un ajustement des formules de calcul du taux horaire par référence à un régime indemnitaire, dont l'application *stricto sensu* serait irrégulière.
#'L'existence, l'applicabilité, et les termes de la délibération correspondante pourront être vérifiés, notamment les formules de calcul mettant en oeuvre cette référence.     
#'Par ailleurs, des vacations peuvent être effectuées par des contractuels de l'établissement sur autorisation de cumul d'activité accessoire, à condition d'avoir
#'obtenu cette autorisation. Le régime indemnitaire dont ils bénéficient pour leur activité principale ne s'étend pas en principe
#'à l'activité accessoire. Une délibération peut toutefois prévoir, dans ce cas également, un ajustement du taux de calcul des vacations par référence à un régime indemnitaire.      

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
#'&nbsp;*Tableau `r incrément()` : Contractuels effectuant des vacations horaires (CEV)*   
#'    
  
if (exists("nombre.contractuels.et.vacations")) {
  
  Tableau(c("Nombre de CEV",
            "Nombre de lignes indemnitaires payées"),
            nombre.contractuels.et.vacations,
            nombre.Lignes.paie.RI.et.vacations)
}
  
#'  
#'[Lien vers les matricules des vacataires](Bases/Reglementation/matricules.contractuels.et.vacations.csv)   
#'[Lien vers les lignes indemnitaires à vérifier](Bases/Reglementation/RI.et.vacations.csv)    
#'[Lien vers les bulletins de paye correspondants](Bases/Reglementation/Paie_vac_contr.csv)   
#'    

#'Les contractuels vacataires rémunérés sur prestation horaire n'ont pas accès au SFT ni à l'indemnité de résidence, contrairement aux contractuels
#'de droit public dont les rémunérations sont calculées sur une base indiciaire. 
#'Les non-titulaires sur contrat effectuant des vacations à titre accessoire pour leur propre employeur ne peuvent bénéficier de paiements
#'complémentaires de SFT ou d'indemnité de résidence au titre de ces activités accessoires.     
  
  essayer({ 
    
  Paie_vac_sft_ir <- filtrer_Paie("IR_S", portée = "Mois", Var = "Type", Base = Paie_vac)[! Statut %chin% c("TITULAIRE", "STAGIAIRE")]  

  SFT_IR.et.vacations <- Paie_vac_sft_ir[Type %chin% c("IR", "S")]
  
  matricules.SFT_IR.et.vacations <- unique(SFT_IR.et.vacations[ , .(Matricule, Nom, Statut)], by=NULL)
  
  nombre.SFT_IR.et.vacations     <- nrow(matricules.SFT_IR.et.vacations)
  }, "Les tests sur les CEV et les versements de SFT ou d'IR n'ont pas été réalisés. ")
  

#'  
#'&nbsp;*Tableau `r incrément()` : CEV percevant le supplément familial de traitement ou l'indemnité de résidence*   
#'    
  
  if (exists("nombre.SFT_IR.et.vacations")) {
    
    Tableau(c("Nombre d'agents concernés",
              "Nombre de lignes de paye SFT/IR"),
            nombre.SFT_IR.et.vacations,
            nrow(SFT_IR.et.vacations))
  }
  
#'  
#'[Lien vers les matricules concernés](Bases/Reglementation/matricules.SFT_IR.et.vacations.csv)     
#'[Lien vers les lignes SFT/IR à vérifier](Bases/Reglementation/SFT_IR.et.vacations.csv)   
#'[Lien vers les bulletins de paye correspondants](Bases/Reglementation/Paie_vac_sft_ir.csv)    
#'   
  
#### 5.5 IAT/IFTS ####  
  
#'
#'## `r chapitre`.5 Contrôle sur les indemnités IAT et IFTS      

#+ IAT-et-IFTS

résultat.ifts.manquant <- FALSE
résultat.iat.manquant  <- FALSE

Paie_I <- Paie[Type == "I" | Type == "A" | Type == "R", 
                 .(Nom, 
                   Matricule, 
                   Année, 
                   Mois, 
                   Code,
                   Libellé,
                   Montant,
                   Type,
                   Emploi,
                   Grade, 
                   Indice,
                   Statut,
                   Catégorie)]


  
Paie_IFTS <- filtrer_Paie("IFTS", portée = "Mois", Base = Paie_I, indic = TRUE)
Paie_IAT  <- filtrer_Paie("IAT", portée = "Mois", Base = Paie_I, indic = TRUE)
Lignes_IFTS <- Paie_IFTS[indic == TRUE][ , indic := NULL]
Lignes_IAT  <- Paie_IAT[indic == TRUE][ , indic := NULL]
Lignes_IAT[ , IAT := TRUE]

if (is.na(codes.ifts)) {          
 codes.ifts  <- list("codes IFTS" = unique(Lignes_IFTS$Code))

  if (length(codes.ifts) == 0) {
    cat("Il n'a pas été possible d'identifier les IFTS par méthode heuristique. Renseigner les codes de paye correspondants dans l'interface graphique. ")
    résultat.ifts.manquant <- TRUE
  }
}

if (is.na(codes.iat)) {          
  codes.iat  <- list("codes IAT" = unique(filtrer_Paie("IAT", Base = Paie_IAT)$Code))
  if (length(codes.iat) == 0) {
    cat("Il n'a pas été possible d'identifier les IAT par méthode heuristique. Renseigner les codes de paye correspondants dans l'interface graphique. ")
    résultat.iat.manquant <- TRUE
  }
}


Paie_IAT.non.tit <- Paie_IAT[Statut != "TITULAIRE" & Statut != "STAGIAIRE" & indic == TRUE]

if ((N.IAT.non.tit <<- uniqueN(Paie_IAT.non.tit$Matricule)) > 0) {
  
  cat(N.IAT.non.tit, "attributaire" %s% N.IAT.non.tit, " de l'IAT sont des non-titulaires. Vérifier l'existence d'une délibération le prévoyant expressément. ")
  
} else {
  
  cat("Tous les attributaires de l'IAT sont titulaires ou stagiaires. ")
}

Paie_IAT.irreg <- Paie_IAT[(Statut == "TITULAIRE" | Statut == "STAGIAIRE") & ( Catégorie == "A" | (Catégorie == "B" & Indice > 350)) & indic == TRUE]

if ((N.IAT.irreg <<- uniqueN(Paie_IAT.irreg$Matricule)) > 0) {
  
  cat(N.IAT.irreg, "attributaire" %s% N.IAT.irreg, " de l'IAT sont des titulaires non éligibles (cat. A ou B d'IB > 380). ")
  
} else {
  
  cat("Tous les attributaires de l'IAT titulaires sont de catégorie C ou B d'IB <= 380. ")
}

nombre.agents.cumulant.iat.ifts <- 0

setnames(Paie_IAT, "indic", "indic_IAT")
setnames(Paie_IFTS, "indic", "indic_IFTS")

if (! résultat.ifts.manquant && ! résultat.iat.manquant) {

  personnels.iat.ifts <- merge(Paie_IAT,
                               Paie_IFTS)[
                                 ,.(Nom,	Matricule,	Année,	Mois,	Code,
                                    Libellé,	Montant,	Type,	Emploi,	Grade,
                                    Indice,	Statut,	Catégorie, indic_IAT, indic_IFTS)
                                 ][indic_IAT == TRUE | indic_IFTS == TRUE]
  
  # on exclut les rappels !
  
  nombre.mois.cumuls <- uniqueN(personnels.iat.ifts[ , .(Matricule, Année, Mois)], 
                                    by = NULL)
  
  nombre.agents.cumulant.iat.ifts <- uniqueN(personnels.iat.ifts$Matricule)
  
  personnels.iat.ifts <- personnels.iat.ifts[order(Année, Mois, Matricule)]
}

#'
#'  
#'&nbsp;*Tableau `r incrément()` : Cumul IAT/IFTS*   
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
#'[Lien vers la base de données iat à des titulaires non éligibles](Bases/Reglementation/Paie_IAT.irreg.csv)         
#'[Lien vers la base de données iat aux non-titulaires](Bases/Reglementation/Paie_IAT.non.tit.csv)            
#'[Codes IFTS retenus](Bases/Reglementation/codes.ifts.csv)      
#'[Lien vers la base de données cumuls iat/ifts](Bases/Reglementation/personnels.iat.ifts.csv)      
#'     
#'     
#'### Contrôle sur les IFTS pour catégories B et non-titulaires      

#IFTS et IB >= 380 (IM >= 350)
#'  
if (! résultat.ifts.manquant) {
    lignes.ifts.anormales <- na.omit(Lignes_IFTS[Indice < 350
                                                  & Catégorie != "A",     # Le pied de corps Attaché est en INM 349
                                                    c(clé.fusion,
                                                      étiquette.année,
                                                      "Mois",
                                                      "Statut",
                                                      "Grade",
                                                      "Catégorie",
                                                      étiquette.code,
                                                      étiquette.libellé,
                                                      "Indice",
                                                      étiquette.montant), 
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
  
  ifts.et.contractuel <- Lignes_IFTS[ Statut != "TITULAIRE"
                                      & Statut != "STAGIAIRE",
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

if (! résultat.ifts.manquant && nombre.lignes.ifts.et.contractuel > 0) {
 
  cat("Les IFTS ne peuvent être attribuées à des non-tituliares que si uen délibration prévoit un tableau d'assimilation. ") 
}

#'     
#'    
#'&nbsp;*Tableau `r incrément()` : IFTS et non-titulaires*     
#'    

if (! résultat.ifts.manquant) {
   Tableau(c("Nombre de lignes de paie de non-titulaires percevant des IFTS", 
             "Nombre de lignes IFTS pour IB < 380"),
           nombre.lignes.ifts.et.contractuel,
           nombre.lignes.ifts.anormales)
}

#'
#'[Lien vers la base de données Lignes IFTS pour contractuels](Bases/Reglementation/ifts.et.contractuel.csv)    
#'[Lien vers la base de données Lignes IFTS pour IB < 380](Bases/Reglementation/lignes.ifts.anormales.csv)     
#'
#'**Nota :**
#'IB < 380 : fonctionnaire percevant un indice brut inférieur à 380
#'

#### 5.6 PFR ####

#'
#'## `r chapitre`.6 Contrôle de la prime de fonctions et de résultats (PFR)   
#'   

#+ pfr

résultat.pfr.manquant <- FALSE
nombre.agents.cumulant.pfr.ifts <- 0

# L'expression régulière capte la PFR et la PR 
# Le cumul de la PR et de l'IFTS est régulier, de même que celui de la PR et de la PFR
# le cumul de la PFR et de l'IFTS est irrrégulier

Paie_PFR <- filtrer_Paie("PFR", portée = "Mois", Base = Paie_I, indic = TRUE)
Lignes_PFR <- Paie_PFR[indic == TRUE][ , indic := NULL]

PFR.non.tit  <- Lignes_PFR[Statut != "TITULAIRE" & Statut != "STAGIAIRE"]
PFR.non.catA <- Lignes_PFR[Catégorie != "A"]

if ((N.PFR.non.tit <<- uniqueN(PFR.non.tit$Matricule)) > 0) {
  
  cat(N.PFR.non.tit, "attributaire" %s% N.PFR.non.tit, " de la PFR sont des non-titulaires. ")
  kable(PFR.non.tit, align = 'r', row.names = FALSE)
  
} else {
  
  cat("Tous les attributaires de la PFR sont titulaires ou stagiaires.")
}


if ((N.PFR.non.catA <<- uniqueN(PFR.non.catA$Matricule)) > 0) {
  
  cat(N.PFR.non.catA, "attributaires de la PFR ne sont pas identifiés en catégorie A. ")
  kable(PFR.non.catA, align = 'r', row.names = FALSE)
  
} else {
  
  cat("Tous les attributaires de la PFR sont identifiés en catégorie A. ")
}

if (is.na(codes.pfr)) {
   codes.pfr  <- list("codes PFR" = unique(Lignes_PFR$Code))

  if (length(codes.pfr) == 0) {
    cat("Il n'a pas été possible d'identifier la PFR par méthode heuristique. Renseigner les codes de paye correspondants dans l'interface graphique. ")
    résultat.pfr.manquant <- TRUE
  }
}

setnames(Paie_PFR, "indic", "indic_PFR")

if (! résultat.ifts.manquant && ! résultat.pfr.manquant) {
  
  # on exclut les rappels !
  
  personnels.pfr.ifts <- merge(Paie_PFR, Paie_IFTS)[ ,.(Nom,	Matricule,	Année,	Mois,	Code,
                                                        Libellé,	Montant,	Type,	Emploi,	Grade,
                                                        Indice,	Statut,	Catégorie, indic_IFTS, indic_PFR)
                                                   ][indic_PFR == TRUE | indic_IFTS == TRUE]

  nombre.mois.cumuls <- uniqueN(personnels.pfr.ifts[ , .(Matricule, Année, Mois)], by = NULL)
  
  nombre.agents.cumulant.pfr.ifts <- uniqueN(personnels.pfr.ifts$Matricule)
  
  personnels.pfr.ifts <- personnels.pfr.ifts[order(Année, Mois, Matricule)]
}

#'   
#'    
#'&nbsp;*Tableau `r incrément()` : Cumul PFR/IFTS*   
#'      
if (length(codes.pfr) < 6) {
  
  Tableau(c("Codes PFR", "Agents cumulant PFR et IFTS"),
          sep.milliers = "",
          paste(unlist(codes.pfr), collapse = " "),
          nombre.agents.cumulant.pfr.ifts)
  
} else {
  
  cat("Codes PFR : ", paste(unlist(codes.pfr), collapse = " "))
  
}

#'     
#'     

cat("Nombre d'agents cumulant PFR et IFTS : ", nombre.agents.cumulant.pfr.ifts)

#'      
#'      
#'[Lien vers la base de données cumuls pfr/ifts](Bases/Reglementation/personnels.pfr.ifts.csv)    
#'[Lien vers la base de données PFR non cat.A](Bases/Reglementation/PFR.non.catA.csv)      
#'[Lien vers la base de données PFR non tit](Bases/Reglementation/PFR.non.tit.csv)       
#'   

# Attention keyby = et pas seulement by = !

Lignes_PFR[ , PFR := TRUE]
Lignes_IFTS[ , IFTS := TRUE]

beneficiaires.PFR <- merge(Lignes_PFR, Lignes_IFTS, all = TRUE)

beneficiaires.PFR[ , Régime := if (all(is.na(PFR))) { if (any(IFTS)) "I" else NA } else { if (all(is.na(IFTS))) "P" else "C" },
                     by = .(Matricule, Année, Mois)][ , `:=`(PFR = NULL, 
                                                             IFTS = NULL)]

matricules.PFR <- unique(Lignes_PFR$Matricule)

beneficiaires.PFR <- beneficiaires.PFR[Matricule %chin% matricules.PFR,
                  .(Agrégat = sum(Montant, na.rm = TRUE),
                    Régime = { c <- uniqueN(Mois[Régime == "C"])
                    
                               if (c == 0) {  
                                 
                                 "IFTS " %+% uniqueN(Mois[Régime == "I"]) %+% " mois-PFR " %+% uniqueN(Mois[Régime == "P"]) %+% " mois"
                                 
                               } else {
                                 
                                 "IFTS " %+% uniqueN(Mois[Régime == "I"]) %+% " mois-PFR " %+% uniqueN(Mois[Régime == "P"]) %+% " mois" %+% "-Cumul " %+% c %+% " mois"
                               }},
                    nb.mois = uniqueN(Mois),
                    Grade = Grade[1]),
                        keyby= .(Matricule, Année)]
                         

# Plafonds annuels (plafonds mensuels reste à implémenter)
# AG 58 800
# ADTHC 55 200
# ADT   49 800
# D/ATP 25 800
# SM/AT 20 100

#'  
#'&nbsp;*Tableau `r incrément()` : Rappel des plafonds annuels de la PFR*   
#'      
  
Tableau(c("Adm. général", "Adm. HC", "Adm.", "Direct./Attaché princ.", "Secr. mairie/Attaché"),
        sapply(PFR.plafonds, 
               function(x) formatC(x, format = "fg", big.mark = " ")))
   
e <- c(expression.rég.admin.g, expression.rég.admin.hc, expression.rég.admin, expression.rég.attaché.p, expression.rég.attaché)
  
test.PFR <- function(i, grade, cumul) { 
  
  grepl(e[i], grade, perl = TRUE, ignore.case = TRUE) & (cumul > PFR.plafonds[[i]]) 
}

  test.PFR.all <- function(grade, cumul) any(sapply(1:length(e), function(i) test.PFR(i, grade, cumul)))
  
  cumuls.PFR <- Lignes_PFR[, .(PFR_annuel = sum(Montant, na.rm = TRUE),
                               nb.mois = uniqueN(Mois),
                               Grade = Grade[1]),
                                  by=.(Matricule,Année)][ , PFR_annuel := PFR_annuel * 12 / nb.mois]   # proratisation mensuelle
  
  dépassements.PFR.boolean <- mapply(test.PFR.all, cumuls.PFR$Grade, cumuls.PFR$PFR_annuel, USE.NAMES=FALSE)

  dépassements.PFR.plafonds <- data.frame()
  
  if (length(dépassements.PFR.boolean) > 0)
    dépassements.PFR.plafonds <- cumuls.PFR[dépassements.PFR.boolean]
  
  if (nrow(dépassements.PFR.plafonds) > 0) {
    
    cat("\nLes plafonds annuels de la PFR sont dépassés pour ", nrow(dépassements.PFR.plafonds), " cumuls annuels.\n")
    kable(dépassements.PFR.plafonds, align = 'r', row.names = FALSE)
    
  } else {
    
    cat("\nLes plafonds annuels de la PFR de sont pas dépassés.\n")
  }
    
  beneficiaires.PFR.Variation <- beneficiaires.PFR[ , 
                                    { 
                                       L <- length(Année)
                                       q <- Agrégat[L]/Agrégat[1] * nb.mois[1]/nb.mois[L]                   
                                       .(Années = paste(Année, collapse = ", "), 
                                         `Variation (%)` = round((q - 1) * 100, 1),
                                         `Moyenne géométrique annuelle(%)` = round((q^(1/(L - 1)) - 1) * 100, 1)) 
                                    }, by="Matricule"]
  
  beneficiaires.PFR.Variation <- beneficiaires.PFR.Variation[`Variation (%)` != 0.00]

#'  
#'&nbsp;*Tableau `r incrément()` : Valeurs de l'agrégat annuel (PFR ou IFTS) pour les bénéficiaires de la PFR*        
#'          

  if (nrow(beneficiaires.PFR)) {
    
    beneficiaires.PFR$Agrégat <- formatC(beneficiaires.PFR$Agrégat, big.mark = " ", format="fg")
    
    kable(beneficiaires.PFR, align = 'r', row.names = FALSE)
    
  } else {
    cat("\nAucun bénéficiaire de la PFR détecté.\n")
  }
  
#'  
#'&nbsp;*Tableau `r incrément()` : Variations de l'agrégat mensuel moyen (PFR ou IFTS) pour les bénéficiaires de la PFR*   
#'          
  if (nrow(beneficiaires.PFR.Variation)) {
    
    kable(beneficiaires.PFR.Variation, align = 'r', row.names = FALSE)
    
  } else {
    cat("\nAucun tableau de variation.\n")
  }
  
#'   
#'[Lien vers la base de données agrégat PFR-IFTS](Bases/Remunerations/beneficiaires.PFR.csv)    
#'    
#'   
#'[Lien vers la base de données variations agrégat PFR-IFTS](Bases/Remunerations/beneficiaires.PFR.Variation.csv)    
#'   
#'[Références juridiques](Docs/Notices/PFR.pdf)   

#### 5.7 PSR ####

#'
#'## `r chapitre`.7 Contrôle de la prime de service et de rendement (PSR)   
#'   

# décret n°2009-1558 du 15 décembre 2009    
  
#+ psr

résultat.psr.manquant <- FALSE
nombre.agents.cumulant.pfr.ifts <- 0
nombre.agents.cumulant.pfr.iat <- 0

# L'expression régulière capte la PSR, non cumulable avec l'IFTS et l'IAT 

Paie_PSR <- filtrer_Paie("PSR", portée = "Mois", Base = Paie_I, indic = TRUE)
Lignes_PSR <- Paie_PSR[indic == TRUE][ , indic := NULL]

PSR.non.tit  <- Lignes_PSR[Statut != "TITULAIRE" & Statut != "STAGIAIRE"]
PSR.non.catAB <- Lignes_PSR[Catégorie == "C" | is.na(Catégorie) | grepl(".*(?:ing|tech|d.*g.*s.*t|v..?t|biol|phar).*", Grade, ignore.case = TRUE, perl = TRUE) == FALSE]

if ((N.PSR.non.tit <<- uniqueN(PSR.non.tit$Matricule)) > 0) {
  
  cat(N.PSR.non.tit, "attributaire" %s% N.PSR.non.tit, " de la PSR", ifelse(N.PSR.non.tit > 1, "sont", "est"), "non-titulaire" %s% N.PSR.non.tit %+%". ")

} else {
  
  cat("Tous les attributaires de la PSR sont titulaires ou stagiaires.")
}

if ((N.PSR.non.catAB <<- uniqueN(PSR.non.catAB$Matricule)) > 0) {
  
  cat(N.PSR.non.catAB, "attributaires de la PSR ne sont pas identifiés en catégorie A ou B comme ingénieur, vétérinaire, biologiste, pharmacien ou technicien. ")

} else {
  
  cat("Tous les attributaires de la PSR sont identifiés en catégorie A ou B comme ingénieur, vétérinaire, biologiste, pharmacien ou technicien. ")
}

if (is.na(codes.psr)) {
  
  codes.psr  <- list("codes PSR" = unique(Lignes_PSR$Code))

  if (length(codes.psr) == 0) {
    cat("Il n'a pas été possible d'identifier la PSR par méthode heuristique. Renseigner les codes de paye correspondants dans l'interface graphique. ")
    résultat.psr.manquant <- TRUE
  }
}

setnames(Paie_PSR, "indic", "indic_PSR")

if (! résultat.ifts.manquant && ! résultat.psr.manquant) {
  
  # on exclut les rappels !
 
  personnels.psr.ifts <- merge(Paie_PSR, Paie_IFTS)[ ,.(Nom,	Matricule,	Année,	Mois,	Code,
                                                        Libellé,	Montant,	Type,	Emploi,	Grade,
                                                        Indice,	Statut,	Catégorie, indic_IFTS, indic_PSR)
                                                   ][indic_PSR == TRUE | indic_IFTS == TRUE]

  nombre.mois.cumuls <- uniqueN(personnels.psr.ifts[ , .(Matricule, Année, Mois)], by = NULL)
  
  nombre.agents.cumulant.psr.ifts <- uniqueN(personnels.psr.ifts$Matricule)
  
  personnels.psr.ifts <- personnels.psr.ifts[order(Année, Mois, Matricule)]
}

if (! résultat.iat.manquant && ! résultat.psr.manquant) {
  
  # on exclut les rappels !
  
  personnels.psr.iat <- merge(Paie_PSR, Paie_IAT)[ ,.(Nom,	Matricule,	Année,	Mois,	Code,
                                                        Libellé,	Montant,	Type,	Emploi,	Grade,
                                                        Indice,	Statut,	Catégorie, indic_IAT, indic_PSR)
                                                   ][indic_PSR == TRUE | indic_IAT == TRUE]

  nombre.mois.cumuls <- uniqueN(personnels.psr.iat[ , .(Matricule, Année, Mois)], by = NULL)
  
  nombre.agents.cumulant.psr.iat <- uniqueN(personnels.psr.iat$Matricule)
  
  personnels.psr.iat <- personnels.psr.iat[order(Année, Mois, Matricule)]
}

#'   
#'    
#'&nbsp;*Tableau `r incrément()` : Cumul PSR/IFTS*   
#'      
if (length(codes.psr) < 6) {
  
  Tableau(c("Codes PSR", "Agents cumulant PSR et IFTS", "Agents cumulant PSR et IAT"),
          sep.milliers = "",
          paste(unlist(codes.psr), collapse = " "),
          nombre.agents.cumulant.psr.ifts,
          nombre.agents.cumulant.psr.iat)
  
} else {

    Tableau(c("Agents cumulant PSR et IFTS", "Agents cumulant PSR et IAT"),
          sep.milliers = " ",
          nombre.agents.cumulant.psr.ifts,
          nombre.agents.cumulant.psr.iat)
  
  cat("Codes PSR : ", paste(unlist(codes.pfr), collapse = " "))
}

#'      
#'      
#'[Lien vers la base de données cumuls psr/ifts](Bases/Reglementation/personnels.psr.ifts.csv)       
#'[Lien vers la base de données cumuls psr/iat](Bases/Reglementation/personnels.psr.iat.csv)         
#'[Lien vers la base de données PSR non cat.A ou B](Bases/Reglementation/PSR.non.catAB.csv)      
#'[Lien vers la base de données PSR non tit](Bases/Reglementation/PSR.non.tit.csv)       
#'   

# Attention keyby = et pas seulement by = !

Lignes_PSR[ , PSR := TRUE]

beneficiaires.PSR <- merge(merge(Lignes_PSR, Lignes_IFTS, all = TRUE), Lignes_IAT, all = TRUE)

beneficiaires.PSR[ , Régime := if (all(is.na(PSR))) { 
                                     if (!is.na(IFTS) && any(IFTS)) "I" else if (! is.na(IAT) && any(IAT)) "A" else NA 
                               } else {
                                     if (all(is.na(IFTS)) && all(is.na(IAT))) "P" else  "C"
                               },
                     by = .(Matricule, Année, Mois)][ , `:=`(PSR = NULL, 
                                                             IAT = NULL,  
                                                             IFTS = NULL)]

matricules.PSR <- unique(Lignes_PSR$Matricule)

beneficiaires.PSR <- beneficiaires.PSR[Matricule %chin% matricules.PSR,
                  .(Agrégat = sum(Montant, na.rm = TRUE),
                    Régime = { c <- uniqueN(Mois[Régime == "C"])
                    
                               if (c == 0) {  
                                 
                                 "IFTS " %+% uniqueN(Mois[Régime == "I"]) %+% " -" %+% "IAT " %+% uniqueN(Mois[Régime == "A"]) %+% " -PSR " %+% uniqueN(Mois[Régime == "P"]) %+% " mois"
                                 
                               } else {
                                 
                                 "IFTS " %+% uniqueN(Mois[Régime == "I"]) %+% " -" %+% "IAT " %+% uniqueN(Mois[Régime == "A"]) %+% " -PSR " %+% uniqueN(Mois[Régime == "P"]) %+% " -Cumul " %+% c %+% " mois"
                               }},
                    nb.mois = uniqueN(Mois),
                    Grade = Grade[1]),
                        keyby= .(Matricule, Année)]
 
  beneficiaires.PSR.Variation <- beneficiaires.PSR[ , 
                                    { 
                                       L <- length(Année)
                                       q <- Agrégat[L]/Agrégat[1] * nb.mois[1]/nb.mois[L]                   
                                       .(Années = paste(Année, collapse = ", "), 
                                         `Variation (%)` = round((q - 1) * 100, 1),
                                         `Moyenne géométrique annuelle(%)` = round((q^(1/(L - 1)) - 1) * 100, 1)) 
                                    }, by="Matricule"]
  
  beneficiaires.PSR.Variation <- beneficiaires.PSR.Variation[`Variation (%)` != 0.00]

#'  
#'&nbsp;*Tableau `r incrément()` : Valeurs de l'agrégat annuel (PSR ou IFTS ou  IAT) pour les bénéficiaires de la PSR*        
#'          

  if (nrow(beneficiaires.PSR)) {
    
    beneficiaires.PSR$Agrégat <- formatC(beneficiaires.PSR$Agrégat, big.mark = " ", format="fg")
    
    kable(beneficiaires.PSR, align = 'r', row.names = FALSE)
    
  } else {
    cat("\nAucun bénéficiaire de la PSR détecté.\n")
  }
  
#'  
#'&nbsp;*Tableau `r incrément()` : Variations de l'agrégat mensuel moyen (PSR ou IFTS ou IAT) pour les bénéficiaires de la PSR*   
#'          
  if (nrow(beneficiaires.PSR.Variation)) {
    
    kable(beneficiaires.PSR.Variation, align = 'r', row.names = FALSE)
    
  } else {
    
    cat("\nAucun tableau de variation.\n")
  }
  
#'   
#'[Lien vers la base de données agrégat PSR-IAT-IFTS](Bases/Remunerations/beneficiaires.PSR.csv)    
#'    
#'   
#'[Lien vers la base de données variations agrégat PSR-IAT-IFTS](Bases/Remunerations/beneficiaires.PSR.Variation.csv)    
#'   

#### 5.8 IPF ####

#'
#'## `r chapitre`.8 Contrôle de l'indemnité de performance et de fonctions (IPF)   
#'   

# décret n°2010-1705 du 30 décembre 2010
  
#+ ipf

résultat.ipf.manquant <- FALSE
nombre.agents.cumulant.ipf.ifts <- 0

# L'expression régulière capte l'IPF
# le cumul de l'IPF et de l'IFTS est irrrégulier

Paie_IPF <- filtrer_Paie("IPF", portée = "Mois", Base = Paie_I, indic = TRUE)
Lignes_IPF <- Paie_IPF[indic == TRUE][ , indic := NULL]

IPF.non.tit  <- Lignes_IPF[Statut != "TITULAIRE" & Statut != "STAGIAIRE"]
IPF.non.catA <- Lignes_IPF[Catégorie != "A" | grepl("ing.*ch", Grade, ignore.case = TRUE) == FALSE]

if ((N.IPF.non.tit <<- uniqueN(IPF.non.tit$Matricule)) > 0) {
  
  cat(N.IPF.non.tit, "attributaire" %s% N.IPF.non.tit, " de l'IPF", ifelse(N.IPF.non.tit > 1, "sont", "est"), "non-titulaire" %s% N.IPF.non.tit, ". ")

} else {
  
  cat("Tous les attributaires de l'IPF sont titulaires ou stagiaires.")
}

if ((N.IPF.non.catA <<- uniqueN(IPF.non.catA$Matricule)) > 0) {
  
  cat(N.IPF.non.catA, "attributaires de l'IPF ne sont pas identifiés en catégorie A comme ingénieur en chef. ")

} else {
  
  cat("Tous les attributaires de l'IPF sont identifiés en catégorie A. ")
}

if (is.na(codes.ipf)) {
   codes.pfr  <- list("codes IPF" = unique(Lignes_IPF$Code))

  if (length(codes.ipf) == 0) {
    cat("Il n'a pas été possible d'identifier l'IPF par méthode heuristique. Renseigner les codes de paye correspondants dans l'interface graphique. ")
    résultat.ipf.manquant <- TRUE
  }
}

setnames(Paie_IPF, "indic", "indic_IPF")

if (! résultat.ifts.manquant && ! résultat.ipf.manquant) {
  
  # on exclut les rappels !
  
  personnels.ipf.ifts <- merge(Paie_IPF, Paie_IFTS)[ ,.(Nom,	Matricule,	Année,	Mois,	Code,
                                                        Libellé,	Montant,	Type,	Emploi,	Grade,
                                                        Indice,	Statut,	Catégorie, indic_IFTS, indic_IPF)
                                                   ][indic_IPF == TRUE | indic_IFTS == TRUE]

  nombre.mois.cumuls <- uniqueN(personnels.ipf.ifts[ , .(Matricule, Année, Mois)], by = NULL)
  
  nombre.agents.cumulant.ipf.ifts <- uniqueN(personnels.ipf.ifts$Matricule)
  
  personnels.ipf.ifts <- personnels.ipf.ifts[order(Année, Mois, Matricule)]
}

#'   
#'    
#'&nbsp;*Tableau `r incrément()` : Cumul IPF/IFTS*   
#'      

if (length(codes.ipf) < 6) {
  
  Tableau(c("Codes IPF", "Agents cumulant IPF et IFTS"),
          sep.milliers = "",
          paste(unlist(codes.ipf), collapse = " "),
          nombre.agents.cumulant.ipf.ifts)
  
} else {
  
  cat("Codes PFR : ", paste(unlist(codes.ipf), collapse = " "))
  
}

#'     
#'     

cat("Nombre d'agents cumulant IPF et IFTS : ", nombre.agents.cumulant.ipf.ifts)

#'      
#'      
#'[Lien vers la base de données cumuls ipf/ifts](Bases/Reglementation/personnels.ipf.ifts.csv)    
#'[Lien vers la base de données IPF non cat.A](Bases/Reglementation/IPF.non.catA.csv)      
#'[Lien vers la base de données IPF non tit](Bases/Reglementation/IPF.non.tit.csv)       
#'   

# Attention keyby = et pas seulement by = !

Lignes_IPF[ , IPF := TRUE]

beneficiaires.IPF <- merge(Lignes_IPF, Lignes_IFTS, all = TRUE)

beneficiaires.IPF[ , Régime := if (all(is.na(IPF))) { if (any(IFTS)) "I" else NA } else { if (all(is.na(IFTS))) "P" else "C" },
                     by = .(Matricule, Année, Mois)][ , `:=`(IPF = NULL, 
                                                             IFTS = NULL)]

matricules.IPF <- unique(Lignes_IPF$Matricule)

beneficiaires.IPF <- beneficiaires.IPF[Matricule %chin% matricules.IPF,
                  .(Agrégat = sum(Montant, na.rm = TRUE),
                    Régime = { c <- uniqueN(Mois[Régime == "C"])
                    
                               if (c == 0) {  
                                 
                                 "IFTS " %+% uniqueN(Mois[Régime == "I"]) %+% " mois-IPF " %+% uniqueN(Mois[Régime == "P"]) %+% " mois"
                                 
                               } else {
                                 
                                 "IFTS " %+% uniqueN(Mois[Régime == "I"]) %+% " mois-IPF " %+% uniqueN(Mois[Régime == "P"]) %+% " mois" %+% "-Cumul " %+% c %+% " mois"
                               }},
                    nb.mois = uniqueN(Mois),
                    Grade = Grade[1]),
                        keyby= .(Matricule, Année)]
                         

  beneficiaires.IPF.Variation <- beneficiaires.IPF[ , 
                                    { 
                                       L <- length(Année)
                                       q <- Agrégat[L]/Agrégat[1] * nb.mois[1]/nb.mois[L]                   
                                       .(Années = paste(Année, collapse = ", "), 
                                         `Variation (%)` = round((q - 1) * 100, 1),
                                         `Moyenne géométrique annuelle(%)` = round((q^(1/(L - 1)) - 1) * 100, 1)) 
                                    }, by="Matricule"]
  
  beneficiaires.IPF.Variation <- beneficiaires.IPF.Variation[`Variation (%)` != 0.00]

#'  
#'&nbsp;*Tableau `r incrément()` : Valeurs de l'agrégat annuel (IPF ou IFTS) pour les bénéficiaires de l'IPF*        
#'          

  if (nrow(beneficiaires.IPF)) {
    
    beneficiaires.IPF$Agrégat <- formatC(beneficiaires.IPF$Agrégat, big.mark = " ", format="fg")
    
    kable(beneficiaires.IPF, align = 'r', row.names = FALSE)
    
  } else {
    cat("\nAucun bénéficiaire de l'IPF détecté.\n")
  }
  
#'  
#'&nbsp;*Tableau `r incrément()` : Variations de l'agrégat mensuel moyen (IPF ou IFTS) pour les bénéficiaires de l'IPF*   
#'          
  if (nrow(beneficiaires.IPF.Variation)) {
    
    kable(beneficiaires.IPF.Variation, align = 'r', row.names = FALSE)
    
  } else {
    cat("\nAucun tableau de variation.\n")
  }
  
#'   
#'[Lien vers la base de données agrégat IPF-IFTS](Bases/Remunerations/beneficiaires.IPF.csv)    
#'    
#'   
#'[Lien vers la base de données variations agrégat IPF-IFTS](Bases/Remunerations/beneficiaires.IPF.Variation.csv)    
#'   
  
  
#### 5.9 HEURES SUP ####
#'    
#'## `r chapitre`.9 Contrôle sur les heures supplémentaires

# Sont repérées comme heures supplémentaires ou complémentaires les heures dont le libellé obéissent à
# l'expression régulière expression.rég.heures.sup donnée par le fichier prologue.R

# Vérification des seuils annuels :


ft <- filtre("TRAITEMENT")

# if (! is.na(ft)) {
#   
#   corriger_T <- function(x, y) {
#     ifelse(x != "T",
#            x,
#            ifelse(y %chin% ft, "T", "NT"))   # pour des raisons non comprises if...else ne fonctionne pas !
#   }
#   
#   Paie[ ,  Type_cor := corriger_T(Type, Code)]  
#    
# } else {
# 
#   corriger_T <- function(x, z) {
#     ifelse(x != "T",
#            x,
#           ifelse(grepl(expression.rég.traitement, z, ignore.case = TRUE, perl = TRUE) 
#               | grepl(expression.rég.nbi, z, ignore.case = TRUE, perl = TRUE) , "T", "NT"))
#   }
#   
#   Paie[ ,  Type_cor := corriger_T(Type, Libellé)]
# }

colonnes <- c(étiquette.matricule,
              étiquette.année,
              "Mois",
              "Statut",
              "Indice",
              "Libellé",
              étiquette.code,
              "Heures",
              "Heures.Sup.",
              "Base",
              "Taux",
              "Montant",
              "Début",
              "Fin",
              "Type",
              "Service",
              "Emploi",
              "Grade")


Base.IHTS <- filtrer_Paie("IHTS",
                          Base = Paie[Type %chin% c("I", "T", "R", "IR", "A"), ..colonnes],
                          portée = "Mois",
                          indic = TRUE)[ , `:=` (Année.rappel = as.numeric(substr(Début, 0, 4)),
                                                 Mois.rappel  = as.numeric(substr(Début, 6, 7))) ]

Base.IHTS.non.tit <- Base.IHTS[Statut != "TITULAIRE" & Statut != "STAGIAIRE" & indic == TRUE]

essayer(
{
  
  if ((N.IHTS.non.tit <<- uniqueN(Base.IHTS.non.tit$Matricule)) > 0) {
    
    cat(N.IHTS.non.tit, "attributaire" %s% N.IHTS.non.tit, " des IHTS sont des non-titulaires. Vérifier l'existence d'une délibération le prévoyant expressément. ")

  } else {
    
    cat("Tous les attributaires des IHTS sont titulaires ou stagiaires. ")
  }
  
  Taux.horaires <- Base.IHTS[ ,.(`IR` = sum(Montant[Type == "IR"], na.rm = TRUE),
                                  IHTS.hors.rappels = sum(Montant[indic == TRUE 
                                                                  & (Type != "R" | (Année.rappel == Année & Mois.rappel == Mois))],
                                                          na.rm = TRUE),
                                  IHTS.rappels = sum(Montant[indic == TRUE
                                                             & Type == "R"
                                                             & ((Année.rappel == Année & Mois.rappel < Mois) | Année.rappel < Année) ],
                                                     na.rm = TRUE),
                                  IHTS.rappels.année.préc = sum(Montant[indic == TRUE 
                                                                        & Type == "R" 
                                                                        & Année.rappel < Année], 
                                                                na.rm = TRUE), 
                                  Indice = Indice[1],
                                  Heures.Sup. = Heures.Sup.[1]), # ajouter NBI proratisée !
                                         by = .(Matricule, Année, Mois)
    
                            ][ , `Traitement indiciaire annuel et IR` := IR * 12 + Indice * PointIM[Année - 2007, Mois]
                              
                            ][ , `Taux horaire` := `Traitement indiciaire annuel et IR` / 1820 
                              
                            ][ ,  `:=` (`Taux horaire inf.14 H` = `Taux horaire` * 1.25,
                                        `Taux horaire sup.14 H` = `Taux horaire` * 1.27,   
                                        `Taux horaire nuit`     = `Taux horaire` * 2,     
                                        `Taux horaire dim. j.f.`= `Taux horaire` * 5/3)  
                              
                            ][ ,   `:=` (Max = Heures.Sup. * `Taux horaire nuit`,
                                         Min = Heures.Sup. * `Taux horaire inf.14 H`)
                              
                            ][ ,  `:=`(Indice = NULL,
                                           IR = NULL)]   
  
  Taux.horaires <-  merge(Base.IHTS, Taux.horaires)[, .(Matricule, Année, Mois, Indice,
                                                         `Traitement indiciaire annuel et IR`,
                                                         `Taux horaire`, Max, Min, 
                                                          IHTS.hors.rappels, IHTS.rappels, IHTS.rappels.année.préc)]
},
  "La base des taux horaires d'heures supplémentaires n'a pas pu être générée. ")

essayer(
{
  Controle.HS <- Taux.horaires[ , .(Matricule, Année, Mois, Max, Min, IHTS.hors.rappels, IHTS.rappels)
                              ][! duplicated(Taux.horaires)  ,
                                    .(Tot.Max  = sum(Max, na.rm = TRUE),
                                      Tot.Min  = sum(Min, na.rm = TRUE),
                                       Tot.IHTS = sum(IHTS.hors.rappels + IHTS.rappels, na.rm = TRUE)), by = "Matricule,Année"]
},
  "La base des cumuls d'IHTS par matricule et année n'a pas pu être générée. ")

#'*Le seuil maximal de liquidation d'IHTS pour l'exercice est déterminé comme égal au nombres d'heures supplémentaires déclarées multipliées par le taux horaire des IHTS de nuit.*    
#'*Les IHTS retenues sont celles qui ont été liquidées sur une base de liquidation, en paiements directs sur l'exercice ou en rappels.*    
#'     
#'     
#'&nbsp;*Tableau `r incrément()` : Paiements au-delà des seuils de liquidation pour l'exercice*   
#'    

essayer(
{

depassement <- Controle.HS[Tot.IHTS > Tot.Max, uniqueN(Matricule)]
depassement.agent <- Controle.HS[Tot.IHTS > Tot.Max, .(Matricule, Tot.Max, Tot.IHTS), keyby = Année]

if (depassement) {
  
  cat("Il y a", depassement, "agent" %+% ifelse(depassement, "s", ""), "qui perçoivent davantage que le maximum d'IHTS pouvant être liquidé au titre de l'exercice.") 
}
  Controle.HS <- Controle.HS[Tot.IHTS > Tot.Max,
                         .(V1 = -sum(Tot.Max) + sum(Tot.IHTS),
                           V2 = uniqueN(Matricule)),
                              keyby=Année]
},
"Le tableau des dépassements de coûts n'a pas pu être généré. ")

  with(Controle.HS,
  
    Tableau.vertical2(c("Année", "Coût en euros", "Nombre d'agents"),
                         Année, digits = 0, V1, V2))         


#'     
#'[Lien vers la base de données des IHTS aux non-titulaires](Bases/Reglementation/Base.IHTS.non.tit.csv)           
#'[Lien vers la base de données dépassements des seuils de liquidation](Bases/Reglementation/Controle.HS.csv)     
#'[Lien vers la base de données dépassements individuels des seuils de liquidation](Bases/Reglementation/depassement.agent.csv)     
#'[Lien vers la base de données calcul des taux horaires individuels](Bases/Reglementation/Taux.horaires.csv)    
#'       


  
#'*Le cumul des heures supplémentaires déclarées (colonne Heures.Sup. des bases) est, par année, comparé au cumul des bases de liquidation IHTS, pour l'année et en régularisation de l'année antérieure*    
#'*Le volume d'heures supplémentaires déclarées et non liquidées sous forme d'IHTS peut correspondre à d'autres régimes d'heures supplémentaires (enseignants, élections) ou à des heures supplémentaires non effectuées (différence positive) ou sous-déclarées (différence négative)*   
#'*Des différences supérieures aux volumes moyens des autres régimes d'heures supplémentaires indiquent une mauvaise fiabilité des déclarations d'heures supplémentaires et/ou des bases de liquidation IHTS*             
#'           
#'       
#'&nbsp;*Tableau `r incrément()` : Cumuls d'heures supplémentaires déclarées, liquidées et régularisée de l'année précédente, en heures*     
#'    

essayer(
{
  CumHS <- Bulletins.paie[, .(V1 = sum(Heures.Sup., na.rm = TRUE)), by = Année]
  
  CumBaseIHTS <- Base.IHTS[indic == TRUE , .(V2 = sum(Base[Type != "R" | (Année.rappel == Année & Mois.rappel < Mois)], na.rm = TRUE),
                                             V3 = sum(Base[Année.rappel < Année], na.rm = TRUE)),
                           by = Année]
  
  CumHS <- merge(CumHS, CumBaseIHTS, all = TRUE)[, Différence := V1 - V2 - shift(V3, type = "lead", fill = 0)]

},
"La base des cumuls d'IHTS par année, des régularisations et des IHTS apparemment non liquidées n'a pas pu être générée. ")

  with(CumHS,
  
    Tableau.vertical2(c("Année", "Cumul heures sup", "Cumul Base IHTS année", "Cumul Régul.N-1", "heures sup sans base de liquidation IHTS"),
                        Année,    V1,                 V2,                      V3,                Différence)
  
  )
  
  
#'
#'[Lien vers la base des cumuls de nombre d'heures IHTS liquidées](Bases/Reglementation/CumHS.csv)     
#'  

Depassement.seuil.180h <- data.table()
Dépassement.seuil.220h <- data.table()

nb.agents.dépassement <- 0
nb.agents.dépassement.220h <- 0

Depassement.seuil.180h <- Bulletins.paie[ , `Cumul heures sup` := sum(Heures.Sup., na.rm = TRUE),
                                                      keyby = .(Matricule, Année)
                                               ][ `Cumul heures sup` > 180, 
                                                .(Matricule, 
                                                  Année,
                                                  `Cumul heures sup`,                        
                                                  Emploi,
                                                  Grade,
                                                  Service)]

nb.agents.dépassement <- uniqueN(Depassement.seuil.180h$Matricule)

if  (nb.agents.dépassement)  {
  if (VERSANT_FP == "FPH") {  
        cat("Le seuil de 180 heures supplémentaires maximum est dépassé par ", 
            FR(nb.agents.dépassement), " agents.\n")
  } 
  
  Depassement.seuil.220h <- Depassement.seuil.180h[`Cumul heures sup` > 220]
  
  nb.agents.dépassement.220h <- uniqueN(Depassement.seuil.220h$Matricule) 
  
  if (VERSANT_FP == "FPH" && nb.agents.dépassement.220h) {  
     cat(" Le seuil de 220 heures supplémentaires maximum est dépassé par ",
                                       FR(nb.agents.dépassement.220h), 
                                       " agents.\n")
  }
}


seuil.HS <- switch (VERSANT_FP, 
                        FPH = 15,
                        FPT = 25)

HS.sup.25 <- Base.IHTS[indic == TRUE & Heures.Sup. > seuil.HS]

setnames(HS.sup.25, "indic", "IHTS identifiée")

nombre.Lignes.paie.HS.sup.25 <- nrow(HS.sup.25[`IHTS identifiée` == TRUE])

ihts.cat.A <- filtrer_Paie("IHTS")[Montant != 0 
                                   & Catégorie == "A"
                                   & Type %chin% c("R", "I", "T", "A"),
                                          .(Matricule, Année, Mois, Statut, Grade, Heures.Sup., Libellé, Code, Type, Montant)]

nombre.ihts.cat.A <- nrow(ihts.cat.A) 

message("Heures sup controlées")

#'
#'  
#'&nbsp;*Tableau `r incrément()` : Heures supplémentaires au-delà des seuils*   
#'    

essayer({
  Tableau(c("Nombre de lignes HS en excès", "Nombre de lignes IHTS cat. A"),
             nombre.Lignes.paie.HS.sup.25,   nombre.ihts.cat.A)
}, "Le tableau des heures supplémentaires et IHTS anormales n'a pas pu être généré.")

#'
#'[Lien vers la base de données Heures supplémentaires en excès du seuil de 15h (FPH) ou de 25h/mois (FPT)](Bases/Reglementation/HS.sup.25.csv)     
#'[Lien vers la base de données cumuls en excès des seuils annuels de 180 h (FPH)](Bases/Reglementation/Depassement.seuil.180h.csv)    
#'[Lien vers la base de données cumuls en excès des seuils annuels de 220 h (FPH)](Bases/Reglementation/Depassement.seuil.220h.csv)   
#'[Lien vers la base de données IHTS versées à des fonctionnaires de cat. A](Bases/Reglementation/ihts.cat.A.csv)      
#'
#'**Nota :**   
#'HS en excès : au-delà de 25 heures par mois dans la FPT et 15 heures par mois dans la FPH, sauf pour certains emplois (18,3 heures par mois)     
#'IHTS cat.A : attribuées à des fonctionnaires ou non-titulaires de catégorie A ou assimilés.     
#'[Références juridiques en lien ](Docs/IHTS.pdf)   

#### 5.10 ELUS ####

#' 
#'## `r chapitre`.10 Contrôle sur les indemnités des élus
#'   


remunerations.elu <- Analyse.remunerations[ indemnités.élu > minimum.positif,
                                            c(clé.fusion,
                                              "Année",
                                              "Emploi",
                                              "indemnités.élu",
                                              "acomptes",
                                              "rémunération.indemnitaire.imposable"),
                                            with=FALSE ]

remunerations.elu <- remunerations.elu[ , rémunération.indemnitaire.imposable := indemnités.élu +  rémunération.indemnitaire.imposable]

remunerations.elu <- merge(unique(matricules[ , .(Nom,  Matricule)], by=NULL),
                             remunerations.elu,
                             by = étiquette.matricule,
                             all.y = TRUE,
                             all.x = FALSE)

names(remunerations.elu) <- c(union(clé.fusion, "Nom"),
                              "Année",
                              "Emploi",
                              "Indemnités ",
                              "Autres ",
                              "Total ")

remunerations.elu <- na.omit(remunerations.elu)

#'   
if (générer.table.élus)   {
  
    if (nrow(remunerations.elu) > 0) {
      
      kable(remunerations.elu, row.names = FALSE)
      
    } else {
  
      cat("Tableau des indemnités d'élu : pas de données.")
    } 
} else {

   cat("Tableau des indemnités d'élu : non générée.")
}


#'[Lien vers la base de données Rémunérations des élus](Bases/Reglementation/remunerations.elu.csv)
#'

#### 5.11 COMPTE DE GESTION ####

#'## `r chapitre`.11 Lien avec le compte de gestion
 

cumul.lignes.paie <- Paie[Type %chin% c("T", "I", "R", "IR", "S", "A", "AC") , 
                        .(Total = sum(Montant, na.rm = TRUE)), keyby="Année,Type,Libellé,Code"]

cumul.lignes.paie <- cumul.lignes.paie[Total != 0]

cumul.lignes.paie$Type <- remplacer_type(cumul.lignes.paie$Type)
                   
cumul.lignes.paie <- cumul.lignes.paie[ , Total2  := formatC(Total, big.mark = " ", format = "f", decimal.mark = ",", digits = 2)]


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
#'[Lien vers la base détaillée des cumuls des lignes de paie](Bases/Reglementation/cumul.lignes.paie.csv)
#'  
#'[Lien vers la base agrégée des cumuls des lignes de paie](Bases/Reglementation/cumul.total.lignes.paie.csv)
#'  

#'  
#'*Avertissement : les rappels comprennent également les rappels de cotisations et déductions diverses.*    
#'   

#### 5.12 SFT ####

#'
#'## `r chapitre`.12 Contrôle du supplément familial de traitement   
#'  

## La biblitothèque SFT est à revoir

if (! utiliser.cplusplus.sft)
{
   source("sft.R", encoding = encodage.code.source)
   sft <- sft_R
   
} else {
  
  library(sft, warn.conflicts = FALSE)
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

essayer({
Paie.sans.enfant.reduit <- Paie[Type == "S" & (is.na(Nb.Enfants) | Nb.Enfants == 0) , .(SFT.versé = sum(Montant, na.rm = TRUE)), keyby = "Matricule,Année,Mois"] 

Paie.sans.enfant.reduit <- Paie.sans.enfant.reduit[SFT.versé > 0]

nb.écart.paiements.sft.sans.enfant <- nrow(Paie.sans.enfant.reduit)

if (nb.écart.paiements.sft.sans.enfant > 0){
  
  cat("\nPour les agents n'ayant pas d'enfant signalé en base, il a été détecté ",
      nb.écart.paiements.sft.sans.enfant,
      " bulletin", ifelse(nb.écart.paiements.sft.sans.enfant == 1, "", "s"),
      " de paie présentant un paiement du SFT apparemment anormal.\n", sep="")  
  
  if (afficher.table.écarts.sft)
    kable(Paie.sans.enfant.reduit, row.names = FALSE, align = 'c')
  
} else {
  
  cat("\nPour les agents n'ayant pas d'enfant signalé en base, il n'a été détecté aucun paiement de SFT.\n")
  
}
}, "Le contrôle du SFT pour les agents sans enfant n'a pas pu être réalisé.")

#'  
#'[Lien vers la base des paiements de SFT à agents sans enfant signalé](Bases/Reglementation/Paie.sans.enfant.reduit.csv)
#'  

# Traitement = sum(Montant[Type == "T"], na.rm = TRUE),

essayer({
  Paie.enfants.réduit <- Paie[! is.na(Nb.Enfants) & Nb.Enfants > 0 & ! is.na(Indice) & ! is.na(Heures),
                              .(SFT.versé = sum(Montant[Type == "S"], na.rm = TRUE), 
                                Temps.de.travail = Temps.de.travail[1],
                                Indice = Indice[1],
                                Echelon = Echelon[1],
                                NBI = NBI[1],
                                Nb.Enfants = Nb.Enfants[1]),
                              keyby="Matricule,Année,Mois"]
    

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
}, 
  "Le contrôle de liquidation du SFT pour les agents ayant au moins un enfant n'a pas pu être réalisé.")

#'  
#'[Lien vers la base des écarts de paiement sur SFT](Bases/Reglementation/controle.sft.csv)
#'  

message("Analyse du SFT")

# data.table here overallocates memory hence inefficient !
# Bulletins.paie[Nb.Enfants > 0 , SFT.controle := sft(Nb.Enfants, Indice, Heures, Année, Mois)]
    
#### 5.13 ASTREINTES ####

#'
#'## `r chapitre`.13 Contrôle des astreintes
#'  

Paie_astreintes <- filtrer_Paie("ASTREINTES", portée = "Mois", indic = TRUE)

libelles.astreintes <- unique(Paie_astreintes[indic == TRUE, .(Code, Libellé)], by = NULL)

Controle_astreintes <- merge(Paie_astreintes[! is.na(NBI) 
                                             & NBI > 0
                                             & indic == TRUE,
                                                  .(Matricule, Année, Mois, Catégorie, Emploi, Grade, NBI, Code, Libellé, quotité, Montant)],
                              Paie_NBI[,.(Matricule, Année, Mois, Code, Libellé, Montant)],
                              by = c("Matricule", "Année", "Mois"))  

Controle_astreintes <- Controle_astreintes[Catégorie == "A" & grepl("d(?:\\.|ir)\\w*\\s*\\bg(?:\\.|\\w*n\\.?\\w*)\\s*\\b(?:des?)\\s*\\bs\\w.*", paste(Emploi, Grade), perl = TRUE, ignore.case = TRUE)]
setnames(Controle_astreintes, c("Code.x", "Libellé.x", "Montant.x"), c("Code.astreinte", "Libellé.astreinte", "Montant.astreinte"))
setnames(Controle_astreintes, c("Code.y", "Libellé.y", "Montant.y"), c("Code.NBI", "Libellé.NBI", "Montant.NBI"))

nb.agents.NBI.astreintes <- uniqueN(Controle_astreintes$Matricule)

if (nrow(Controle_astreintes)) {
 cat("Des astreintes sont payées à", nb.agents.NBI.astreintes, "personnels bénéficiaires de NBI")
}

Cum_astreintes <- rbind(Controle_astreintes[, round(sum(Montant.astreinte), 1),
                                                  by = "Année"],
                                            list("Total", Controle_astreintes[, round(sum(Montant.astreinte), 1)]))

#'  
#'&nbsp;*Tableau `r incrément()` : Cumuls irréguliers NBI et astreintes (responsabilité supérieure)*   
#'  

essayer({
with(Cum_astreintes,
  
Tableau.vertical2(c("Année", "Montant astreintes irrégulières (euros)"),
                  Année, V1)    

)
}, "Le tableau de contrôle des astreintes n'a pas pu être généré.")

#'**Nota**    
#'Vérifier l'adéquation des libellés de paye d'astreinte dans le tableau en lien ci-après.      
#'Définition des fonctions de responsabilité supérieure : décrets du 27 décembre 2001 et du 28 décembre 200    
#'[Lien vers la base des cumuls astreintes/NBI](Bases/Reglementation/Controle_astreintes.csv)
#'[Lien vers les libellés et codes astreintes](Bases/Reglementation/libelles.astreintes.csv)     
#'   


setnames(Paie_astreintes, "indic", "indic_astr")
setnames(Base.IHTS, "indic", "indic_IHTS")

Controle_astreintes_HS_irreg <- merge(Paie_astreintes[ , .(Matricule, Année, Mois, Code, Libellé, Type,  Montant, indic_astr) ], 
                              Base.IHTS[Type %chin% c("I", "A", "R"), .(Matricule, Année, Mois, Code, Libellé, Type, Montant, indic_IHTS)], 
                              by=c("Matricule", "Année", "Mois", "Code", "Libellé", "Type", "Montant")
                              )[indic_IHTS == TRUE | indic_astr == TRUE]

nb.agents.IHTS.astreintes <- uniqueN(Controle_astreintes_HS_irreg$Matricule)

if (nrow(Controle_astreintes_HS_irreg)) {
 cat("Des astreintes sont payées à", nb.agents.IHTS.astreintes, "personnels bénéficiaires d'IHTS.")
}

Cum_astreintes_HS_irreg <- rbind(Controle_astreintes_HS_irreg[, .(round(sum(Montant[indic_astr == TRUE]), 1),
                                                                  round(sum(Montant[indic_IHTS == TRUE]), 1)),
                                                                  keyby = "Année"],
                                list("Total",
                                     Controle_astreintes_HS_irreg[indic_astr == TRUE, round(sum(Montant), 1)],
                                     Controle_astreintes_HS_irreg[indic_IHTS == TRUE, round(sum(Montant), 1)]))

#'  
#'&nbsp;*Tableau `r incrément()` : Cumuls potentiellement irréguliers IHTS et astreintes*   
#'  

with(Cum_astreintes_HS_irreg,
  
Tableau.vertical2(c("Année", "Montant astreintes potentiellement irrégulières (euros)", "Montant IHTS correspondantes"),
                  Année, V1, V2)    

)

#'**Nota**:     
#'Les cumuls peuvent être réguliers s'il y a eu des interventions non compensées en période d'astreinte.      
#'[Lien vers la base des cumuls astreintes/IHTS](Bases/Reglementation/Controle_astreintes_HS_irreg.csv)   
#'[Lien vers les cumuls annuels](Bases/Reglementation/Cum_astreintes_HS_irreg.csv)   


rm(Base.IHTS)
  
#### 5.14 RETRAITES ####

#'
#'## `r chapitre`.14 Contrôle des cotisations de retraite    
#'  

#'**Non titulaires**   
#'    
#'     


cat("Les non titulaires ne doivent pas cotiser à la CNRACL. ")
    
Cotisations.irreg <- Paie[(Type == "D"  | Type == "C") 
                           & Statut != "TITULAIRE" 
                           & Statut != "STAGIAIRE" 
                           & grepl("C\\.?\\s*N\\.?\\s*R\\.?\\s*A\\.?\\s*C\\.?\\s*L",
                                   Libellé,
                                   ignore.case = TRUE,
                                   perl = TRUE) 
                           & Montant > 0,
                               .(Matricule, Année, Mois, Type, Libellé, Montant)]

#'  
#'&nbsp;*Tableau `r incrément()` : Cotisations irrégulières à la CNRACL*     
#'   

if (! identical(Cotisations.irreg, logical(0)) & (nlignes <- nrow(Cotisations.irreg)) > 0) {
 
  cat("Des cotisations CNRACL sont versées pour des agents non titulaires :",  nlignes, "ligne" %s% nlignes, "de paye.")
}

#'   
#'       
  

Tableau(c("Cotisations salarié", "Cotisations employeur"),
           Cotisations.irreg[Type == "D", sum(Montant, na.rm = TRUE)], Cotisations.irreg[Type == "C", sum(Montant, na.rm = TRUE)])                   


#'   
#'[Lien vers la base des cotisations irrégulières](Bases/Reglementation/Cotisations.irreg.csv)   
#'   

#'    
#'**Titulaires**   
#'    

cat("Les titulaires ne doivent pas cotiser à l'IRCANTEC. ")

Cotisations.irreg.ircantec <- Paie[(Type == "D"  | Type == "C") 
                                   & Statut == "TITULAIRE" 
                                   & grepl("I\\.?\\s*R\\.?\\s*C\\.?\\s*A\\.?\\s*N\\.?\\s*",
                                           Libellé,
                                           ignore.case = TRUE, perl = TRUE) 
                                   & Montant > 0,
                                       .(Matricule, Année, Mois, Type, Libellé, Montant)]

#'  
#'&nbsp;*Tableau `r incrément()` : Cotisations irrégulières à l'IRCANTEC*     
#'   

if (! identical(Cotisations.irreg.ircantec, logical(0)) & (nlignes <- nrow(Cotisations.irreg.ircantec)) > 0) {
 
  cat("Des cotisations IRCANTEC sont versées pour des agents titulaires :",  nlignes, "ligne" %s% nlignes, "de paye.")
}

#'   
#'       
  
Tableau(c("Cotisations salarié", "Cotisations employeur"),
           Cotisations.irreg.ircantec[Type == "D", sum(Montant, na.rm = TRUE)], Cotisations.irreg.ircantec[Type == "C", sum(Montant, na.rm = TRUE)])    

#'   
#'[Lien vers la base des cotisations irrégulières](Bases/Reglementation/Cotisations.irreg.ircantec.csv)   
#'   

#### 5.15 PRIMES FPH ####     

#'   
#'## `r chapitre`.15 Primes de la fonction publique hospitalière          
#'    
#'     
#'*Les primes qui suivent ne peuvent être octroyées qu'à des fontionnaires.*    
#'*Les tests portent sur les cas d'attribution à des non-titulaires (et autres statuts)*     
#'      

#'**Prime spécifique**   

# décret n°88-1083 du 30 novembre 1988

primes.spécifiques.potentielles <- ""
personnels.prime.specifique.nt <- NULL

if (VERSANT_FP == "FPH") {
  
  Paie_pspec <- filtrer_Paie("PRIME SPECIFIQUE")
  
  personnels.prime.specifique.nt <- Paie_pspec[Statut != "TITULAIRE" & Statut != "STAGIAIRE" , ..colonnes]
    
  if (nombre.personnels.pspec.nt <- uniqueN(personnels.prime.specifique.nt$Matricule)) {
    
    cat("Il existe ", 
        FR(nombre.personnels.pspec.nt),
        "agent" %s% nombre.personnels.pspec.nt,
        " non titulaire" %s% nombre.personnels.pspec.nt, "percevant une prime spécifique.")
  }
}

#'     
#'     

if (VERSANT_FP == "FPH") {
  if (nombre.personnels.pspec.nt) {
    
    cat("Coût des anomalies ", 
        personnels.prime.specifique.nt[ , sum(Montant, na.rm = TRUE)], "euros.")
  }
  
  primes.spécifiques <- unique(Paie_pspec$Libellé)
  
  primes.spécifiques.potentielles <- if (length(primes.spécifiques) == 0) "aucune" else paste(primes.spécifiques, collapse = " ;")
}

#'   
#'Primes spécifiques : `r primes.spécifiques.potentielles`    
#'   

#'   
#'[Lien vers la base de données Prime spécifique NT](Bases/Reglementation/personnels.prime.specifique.nt.csv)   
#'   

#'**Prime de technicité**   

# décret n°91-870 du 5 septembre 1991 

primes.tech.potentielles <- ""
personnels.prime.tech.nt <- NULL

if (VERSANT_FP == "FPH") {
  Paie_tech <- filtrer_Paie("PRIME DE TECHNICITE")
  
  personnels.prime.tech.nt <- Paie_tech[Statut != "TITULAIRE" & Statut != "STAGIAIRE", ..colonnes]
    
  if (nombre.personnels.tech.nt <- uniqueN(personnels.prime.tech.nt$Matricule)) {
    
    cat("Il existe ", 
        FR(nombre.personnels.tech.nt),
        "agent" %s% nombre.personnels.tech.nt,
        "non titulaire"  %s% nombre.personnels.tech.nt, 
         "percevant une prime de technicité.")
  }
}

#'    
#'    

if (VERSANT_FP == "FPH") {
  if (nombre.personnels.tech.nt) {
    
    cat("Coût des anomalies ", 
        personnels.prime.tech.nt[ , sum(Montant, na.rm = TRUE)], "euros.")
  }
  
  primes.tech <- unique(Paie_tech$Libellé)   
  
  primes.tech.potentielles <- if (length(primes.tech) == 0) "aucune" else paste(primes.tech, collapse = " ;")    
}

#'   
#'Primes de technicité : `r primes.tech.potentielles`    
#'   

#'   
#'[Lien vers la base de données Prime de technicité NT](Bases/Reglementation/personnels.prime.tech.nt.csv)   
#'   

#'**Indemnité forfaitaire et technique**   

# décret n°2013-102 du 29 janvier 2013  

primes.ift.potentielles <- ""
personnels.ift.nt <- NULL

if (VERSANT_FP == "FPH") {
  Paie_ift <- filtrer_Paie("IFT")
  
  personnels.ift.nt <- Paie_ift[Statut != "TITULAIRE" & Statut != "STAGIAIRE", ..colonnes]
    
  if (nombre.personnels.ift.nt <- uniqueN(personnels.ift.nt)) {
    
    cat("Il existe ", 
        FR(nombre.personnels.ift.nt),
        "agent" %s% nombre.personnels.ift.nt,
        "non titulaire" %s% nombre.personnels.ift.nt, "percevant une indemnité forfaitaire et technique.")
  }
}

#'    
#'    

if (VERSANT_FP == "FPH") {
  if (nombre.personnels.ift.nt) {
    
    cat("Coût des anomalies ", 
        personnels.ift.nt[ , sum(Montant, na.rm = TRUE)], "euros.")
  }
  
  primes.ift <- unique(Paie_ift$Libellé)
  
  primes.ift.potentielles <- if (length(primes.ift) == 0) "aucune" else paste(primes.ift, collapse = " ;")
}

#'   
#'Indemnité forfaitaire et technique : `r primes.ift.potentielles`    
#'   

#'   
#'[Lien vers la base de données IFT NT](Bases/Reglementation/personnels.ift.nt.csv)   
#'   

#'**Prime de service**   

primes.ift.potentielles <- 0
personnels.ps.nt <- NULL

if (VERSANT_FP == "FPH") {
  Paie_ps <- filtrer_Paie("PRIME DE SERVICE")
  
  personnels.ps.nt <- Paie_ps[Statut == "NON_TITULAIRE" | Statut == "AUTRE_STATUT" | grepl(expression.rég.médecin, Grade, perl = TRUE, ignore.case = TRUE), ..colonnes]
    
  if (nombre.personnels.ps.nt <- uniqueN(personnels.ps.nt)) {
    
    cat("Il existe ", 
        FR(nombre.personnels.ps.nt),
        "agent" %s% nombre.personnels.ift.nt,
        "non titulaire" %s% nombre.personnels.ift.nt, "percevant une indemnité forfaitaire et technique.")
  }
}

#'    
#'    

if (VERSANT_FP == "FPH") {
  if (nombre.personnels.ift.nt) {
    
    cat("Coût des anomalies ", 
        personnels.ps.nt[ , sum(Montant, na.rm = TRUE)], "euros.")
  }
  
  primes.ift <- unique(Paie_ift$Libellé)
  
  primes.ift.potentielles <- if (length(primes.ift) == 0) "aucune" else paste(primes.ift, collapse = " ;")
}

#'   
#'Indemnité forfaitaire et technique : `r primes.ift.potentielles`    
#'   

#'   
#'[Lien vers la base de données IFT NT](Bases/Reglementation/personnels.ps.nt.csv)   
#'   

#### ANNEXE ####

newpage()

#'# Annexe       

#'   
#'## Contrôle des événements de paye   
#'      

E <- Bulletins.paie[ , unique(Evenement)]
Evenements <- unique(trimws(regmatches(E, regexpr("[^-]*", E, perl=TRUE))))
Evenements2 <- unique(trimws(gsub("-", "", 
                                  regmatches(E, regexpr("- .*", E, perl=TRUE)))))

Evenements <- sort(c(Evenements, Evenements2))
rm(E, Evenements2)
if (Evenements[1] == "") Evenements <- Evenements[-1] 
Evenements <- data.table(Evénements = Evenements)

if (afficher.table.événements) {
    kable(Evenements)
}

Evenements.ind <- setkey(Bulletins.paie[Evenement != "" & Evenement != "NA NA", 
                                       .(Evenement,
                                         Matricule,
                                         Nom,
                                         Prénom,
                                         Année,
                                         Mois,
                                         Grade,
                                         Emploi,
                                         Service)],
                                       Evenement,
                                       Matricule,
                                       Année,
                                       Mois)

Evenements.mat <- setcolorder(setkey(copy(Evenements.ind), 
                                    Matricule,
                                    Année,
                                    Mois,
                                    Evenement),
                             c("Matricule",
                               "Nom",
                               "Prénom",
                               "Année",
                               "Mois",
                               "Evenement",
                               "Grade",
                               "Emploi",
                               "Service"))

#'  
#'[Lien vers la nomenclature des événements de paye](Bases/Fiabilite/Evenements.csv)     
#'[Tri par type d'évement, agent, année, mois](Bases/Fiabilite/Evenements.ind.csv)     
#'[Tri par agent, année, mois, évenement](Bases/Fiabilite/Evenements.mat.csv)     
#'  

#'  
#'## Codes et libellés de paye   
#'         

code.libelle <- unique(Paie[Montant != 0, .(Code, Libellé), by = "Type"], by = NULL)
code.libelle$Type <- remplacer_type(code.libelle$Type)

setcolorder(code.libelle, c("Code", "Libellé", "Type"))
if (afficher.table.codes) {
   kable(code.libelle, align="c")
}

#'Certains libellés ou codes de paye peuvent être équivoques et entraîner des erreurs de requête.       
#'Les liens ci-après donnent les codes correspondant à au moins deux libellés distincts, les libellés correspondant à au moins deux codes et les codes ou libellés correspondant à au moins deux types de ligne de paye distincts.           
#'L'équivocité des codes est particulièrement préjudiciable lorsque les libellés correspondent à des types de ligne de paye distincts.    
#'
cl1 <- code.libelle[ , .(Multiplicité = .N, Libellé, Type), by = "Code"][Multiplicité > 1]
cl2 <- code.libelle[ , .(Multiplicité = .N,  Code, Type), by = "Libellé"][Multiplicité > 1]

cl3 <- unique(code.libelle[, .(Code, Type)], by = NULL)[ , .(Multiplicité = .N,  Type), by = "Code"][Multiplicité > 1]
cl4 <- unique(code.libelle[, .(Libellé, Type)], by = NULL)[ , .(Multiplicité = .N,  Type), by = "Libellé"][Multiplicité > 1]

#'   
#'[Lien vers la table Codes/Libellés](Bases/Fiabilite/code.libelle.csv)       
#'[Plusieurs libellés par code](Bases/Fiabilite/cl1.csv)   
#'[Plusieurs codes par libellé](Bases/Fiabilite/cl2.csv)   
#'[Plusieurs types de ligne par code](Bases/Fiabilite/cl3.csv)   
#'[Plusieurs types de ligne par libellé](Bases/Fiabilite/cl4.csv)           
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
base.quotite.indefinie.salaire.non.nul <- Bulletins.paie[MHeures == 0 & (Net.à.Payer != 0 | Brut != 0)]

nligne.base.heures.nulles.salaire.nonnull     <- nrow(base.heures.nulles.salaire.nonnull)
nligne.base.quotite.indefinie.salaire.non.nul <- nrow(base.quotite.indefinie.salaire.non.nul)
#'  
if (nligne.base.heures.nulles.salaire.nonnull)
   cat("Nombre de bulletins de paie de salaires versés pour un champ Heures = 0 : ", FR(n <<- nligne.base.heures.nulles.salaire.nonnull), "[", round(n/nrow.bull * 100, 1), "%]")
#'   
if (nligne.base.quotite.indefinie.salaire.non.nul)
   cat("\nNombre de bulletins de paie de salaires versés pour une quotité de travail indéfinie : ", FR(nligne.base.heures.nulles.salaire.nonnull))
#'   
#'[Lien vers la base de données des salaires versés pour Heures=0](Bases/Fiabilite/base.heures.nulles.salaire.nonnull.csv)   
#'[Lien vers la base de données des salaires versés à quotité indéfinie](Bases/Fiabilite/base.quotite.indefinie.salaire.non.nul.csv)   
#'
#'## Tableau des personnels  
#'
#'
if (afficher.table.effectifs) {
  kable(grades.categories, row.names = FALSE) 
} 

#'
#'[Lien vers la base des grades et catégories](Bases/Effectifs/grades.categories.csv)        
#'   

#'
#'[Lien vers la base des personnels](Bases/Effectifs/matricules.csv)        
#'   


#'
#'## Divergences lignes-bulletins de paie     
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
#'[Divergences lignes-bulletins de paie](Bases/Fiabilite/Delta.csv)     
#'   


######### SAUVEGARDES #######

# La suppression des accents est regrettablement motivée par les incompatibilités de la conversion pdf sous Windows.

envir <- environment()

if (sauvegarder.bases.analyse) {

  
  sauv.bases(file.path(chemin.dossier.bases, "Remunerations"),
             env = envir,
             "Analyse.remunerations",
             "Anavar.synthese",
             "Analyse.variations.par.exercice",
             "beneficiaires.PFR",
             "beneficiaires.PSR",
             "beneficiaires.IPF",
             "beneficiaires.PFR.Variation",
             "beneficiaires.PSR.Variation",
             "beneficiaires.IPF.Variation")

  sauv.bases(file.path(chemin.dossier.bases, "Effectifs"),
             env = envir,
             "matricules",
             "grades.categories",
             "tableau.effectifs")

  sauv.bases(file.path(chemin.dossier.bases, "Reglementation"),
             env = envir,
             "personnels.iat.ifts",
             "Paie_IAT.non.tit",
             "Paie_IAT.irreg",  
             "codes.ifts",
             "personnels.pfr.ifts",
             "personnels.ipf.ifts",
             "personnels.psr.ifts",
             "personnels.psr.iat",
             "codes.pfr",
             "codes.psr",
             "codes.ipf",
             "HS.sup.25",
             "Base.IHTS.non.tit",
             "Controle.HS",
             "depassement.agent",
             "Taux.horaires",
             "CumHS",
             "Depassement.seuil.180h",
             "Depassement.seuil.220h",
             "ifts.et.contractuel",
             "ihts.cat.A",
             "Controle_astreintes.csv",
             "Controle_astreintes_HS_irreg",
             "Cum_astreintes_HS_irreg",
             "libelles.astreintes",
             "PFR.non.catA",
             "PSR.non.catAB",
             "IPF.non.catA",
             "PFR.non.tit",
             "PSR.non.tit",
             "IPF.non.tit",
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
             "NBI.cat.irreg",
             "personnels.prime.informatique",
             "personnels.iat.ifts",
             "remunerations.elu",
             "RI.et.vacations",
             "traitement.et.vacations",
             "cumul.lignes.paie",
             "cumul.total.lignes.paie",
             "controle.sft",
             "Paie.sans.enfant.reduit",
             "Cotisations.irreg",
             "Cotisations.irreg.ircantec",
             "personnels.ift.nt",
             "personnels.prime.specifique.nt",
             "personnels.prime.tech.nt",
             "personnels.ps.nt")
  
  sauv.bases(file.path(chemin.dossier.bases, "Fiabilite"),
             env = envir,
              "base.heures.nulles.salaire.nonnull",
              "base.quotite.indefinie.salaire.non.nul",
              "lignes.nbi.anormales",
              "lignes.nbi.anormales.mensuel",
              "lignes.paie.nbi.anormales.mensuel",
              "cumuls.nbi",
              "cl1",
              "cl2",
              "cl3",
              "cl4",
              "code.libelle",
              "Evenements",
              "Evenements.ind",
              "Evenements.mat")
  
  if (test.delta) 
    sauv.bases(file.path(chemin.dossier.bases, "Fiabilite"), env = envir, "Delta")
  
}

if (sauvegarder.bases.origine)
  sauv.bases(file.path(chemin.dossier.bases, "Paiements"),
             env = envir,
             "Paie",
             "Bulletins.paie")


if (! dir.exists(chemin.dossier.docs)) 
   dir.create(chemin.dossier.docs, recursive = TRUE, mode="0777")

res <- file.copy("Docs", chemin.clé, recursive=TRUE)

if (res) message("Dossier Docs copié dans", chemin.clé) else message("Dossier Docs n'a pas été copié dans", chemin.clé)

setwd(currentDir)

message("Dossier courant : ", getwd())

rm(list = setdiff(ls(), script_env))
