
#'---
#'title: false
#'author: false
#'date: false
#'output:
#'html_document:
#'css: style.css
#'---
#'   
#'![Image_Altair](../../Altaïr.png)
#'   
#'   
#'# Démonstrateur Altaïr version 15.02   

#+ echo = FALSE, warning = TRUE, message = FALSE

# comportement global du programme

# Lorsque l'on n'a que une ou deux années, mettre étudier.variations à FALSE
# Lorsque l'on n'étudie pas une base Xémélios, mettre étudier.tests.statutaires à FALSE

library(compiler)
invisible(setCompilerOptions(suppressAll = TRUE))
invisible(enableJIT(1))

options(warn = -1, verbose = FALSE, OutDec = ",", datatable.verbose = FALSE)

encodage.code.source <- "UTF-8" #"ISO-8859-15"

# dans cet ordre

try(setwd("Tests/Exemple"), silent = TRUE)

source("prologue.R", encoding = encodage.code.source)
source(file.path(chemin.dossier, "bibliotheque.fonctions.paie.R"), encoding = encodage.code.source)
source("import.R", encoding = encodage.code.source)

#'
#'<p class = "centered"><b>Exercices `r paste(début.période.sous.revue, "à", fin.période.sous.revue)` </b></p>
#'<p class = "author">Fabrice Nicol</h1>
#'
#+ echo = FALSE
#'`r format(Sys.Date(), "%a %d %b %Y")`
#'
###########  Analyse des rémunérations  ########

source("analyse.rémunérations.R", encoding = encodage.code.source)

########### Démographie ########################

incrémenter.chapitre()

#'# `r chapitre`. Statistiques de population
#'
#'### `r chapitre`.1 Effectifs

liste.années <- as.character(période)

# Rappel Analyse.variations.par.exercice comprend uniquement les actifs non annexes non assist. mat., non vacataires, non élus.

effectifs <- lapply(période,
                    function(x) {
                      A <- Bulletins.paie[Année == x, 
                                          .(Matricule, Statut, permanent, quotité, nb.mois, Grade)]
                      
                      E <- unique(A[ , .(Matricule, permanent)], by = NULL)
                      ETP <- unique(Bulletins.paie[Année == x, 
                                                   .(quotité, Matricule, Statut, permanent, Mois, nb.mois)],
                                    by = NULL)
                      F <- E[permanent == TRUE, ]
                      G <- unique(A[Statut == "TITULAIRE" | Statut == "STAGIAIRE", .(Matricule, permanent)],
                                  by = NULL)
                      H <- G[permanent == TRUE, ]
                      I <- unique(A[Statut == "ELU", .(Matricule, permanent)],
                                  by = NULL)
                      J <- I[permanent == TRUE, ]
                      K <- unique(A[Statut != "TITULAIRE" & Statut != "STAGIAIRE" & Grade == "V", .(Matricule, permanent)],
                                  by = NULL)
                      L <- unique(A[Grade == "A", .(Matricule, permanent)],
                                  by = NULL)
                      postes.non.actifs <- unique(Analyse.rémunérations[Statut != "ELU"
                                                                        & Filtre_actif == FALSE
                                                                        & Année == x,
                                                                          Matricule])
                      postes.annexes <- unique(Analyse.rémunérations[Statut != "ELU"
                                                                     & Filtre_non_annexe == FALSE
                                                                     & Année == x,
                                                                       Matricule])
                      postes.actifs.non.annexes <- unique(Analyse.rémunérations[Statut != "ELU"
                                                                                & Filtre_actif_non_annexe == TRUE
                                                                                & Année == x,
                                                                                  Matricule])
                      postes.non.titulaires <- unique(Analyse.variations.par.exercice[Statut == "NON_TITULAIRE" 
                                                                                      & Année ==x, 
                                                                                        Matricule])
                      
                      c(nrow(E), 
                        nrow(F),
                        nrow(G),
                        nrow(H),
                        length(postes.non.titulaires),
                        nrow(I),
                        nrow(J),
                        nrow(K),
                        nrow(L),
                        length(postes.non.actifs),
                        length(postes.annexes),
                        length(postes.actifs.non.annexes),
                        ETP[Statut != "ELU" , sum(quotité/nb.mois, na.rm=TRUE)],
                        ETP[Statut != "ELU" , sum(quotité, na.rm=TRUE)] / 12,
                        ETP[Matricule %chin% unique(Analyse.variations.par.exercice[est.rmpp == TRUE
                                                                                    & Année == x,
                                                                                    Matricule]),
                            sum(quotité, na.rm=TRUE)] / 12,
                        ETP[(Statut == "TITULAIRE" | Statut == "STAGIAIRE")
                            & Matricule %chin% unique(Analyse.variations.par.exercice[Statut == "TITULAIRE"
                                                                                      | Statut == "STAGIAIRE",
                                                                                         Matricule]),
                              sum(quotité, na.rm=TRUE)] / 12,
                        ETP[Statut == "TITULAIRE"
                            & permanent == TRUE 
                            & Matricule %chin% unique(Analyse.variations.par.exercice[permanent == TRUE
                                                                             & Statut == "TITULAIRE"
                                                                             & temps.complet == TRUE
                                                                             & Année == x,
                                                                             Matricule]),
                              sum(quotité, na.rm=TRUE)] / 12,
                        ETP[Statut == "NON_TITULAIRE" 
                            & Matricule %chin% postes.non.titulaires,  
                              sum(quotité, na.rm=TRUE)] / 12,
                        ETP[Statut == "AUTRE_STATUT"  
                            & Matricule %chin% unique(Analyse.rémunérations[Statut == "AUTRE_STATUT",
                                                                                    Matricule]),
                              sum(quotité, na.rm=TRUE)] / 12,
            						ETP[Matricule %chin% postes.non.actifs, sum(quotité, na.rm=TRUE)] / 12,
            						ETP[Matricule %chin% postes.annexes, sum(quotité, na.rm=TRUE)] / 12,
                        ETP[Matricule %chin% postes.actifs.non.annexes, sum(quotité, na.rm=TRUE)] / 12)							
                     })

for (i in 1:length(effectifs)) names(effectifs[[i]]) <- c("Effectifs", 
                                                          "Effectifs_12",
                                                          "Effectifs_12_fonct",
                                                          "Effectifs_12_fonct",
                                                          "Effectifs_nontit",
                                                          "Effectifs_élus",
                                                          "Effectifs_12_élus",
                                                          "Effectifs_vac",
                                                          "Effectifs_am",
                                                          "Effectifs_non.actifs",
                                                          "Effectifs_annexes",
                                                          "Effectifs_actifs_non.annexes",
                                                          "ETP",
                                                          "ETPT",
                                                          "ETPT_pp", 
                                                          "ETPT_fonct",
                                                          "Tit_12_100",
                                                          "ETPT_nontit", 
                                                          "ETPT_autre",
                                                          "ETPT_non_actif",
                                                          "ETPT_annexe",
                                                          "ETPT_actif_nonannexe")
 
effectifs.locale <- lapply(effectifs, 
                           function(x) formatC(x, big.mark = " ", format="f", digits=1, decimal.mark=","))

tableau.effectifs <- as.data.frame(effectifs.locale,
                                   row.names = c("Total effectifs (a)",
                                                 "&nbsp;&nbsp;&nbsp;dont présents 12 mois",
                                                 "&nbsp;&nbsp;&nbsp;dont fonctionnaires (b)",
                                                 "&nbsp;&nbsp;&nbsp;dont fonct. présents 12 mois",
                                                 "&nbsp;&nbsp;&nbsp;dont non titulaires",
                                                 "&nbsp;&nbsp;&nbsp;dont élus",
                                                 "&nbsp;&nbsp;&nbsp;dont élus présents 12 mois",
                                                 "&nbsp;&nbsp;&nbsp;dont vacataires détectés (c)",
                                                 "&nbsp;&nbsp;&nbsp;dont assistantes maternelles détectées (c)",
                                                 "Postes non actifs (g)",
                                                 "Postes annexes (g)",
                                                 "Postes actifs non annexes (g)",
                                                 "Total ETP/année (d)",
                                                 "Total ETPT/année (e)",
                                                 "Total ETPT/année personnes en place (f)(g)",
                                                 "Total ETPT/année fonctionnaires (g)",
                                                 "Total ETPT/année titulaires à temps complet (g)",
                                                 "Total ETPT non titulaires (g)",
                                                 "Total ETPT autre statut",
                                                 "Total ETPT postes non actifs (g)",
                                                 "Total ETPT postes annexes (g)",
                                                 "Total ETPT postes actifs non annexes (g)"))

names(tableau.effectifs) <- liste.années
names(effectifs) <- liste.années
#'  
#  
#'&nbsp;*Tableau `r incrément()`*   
#            
kable(tableau.effectifs, row.names = TRUE, align='c')
#'
#'**Nota:**   
#'*(a) Nombre de matricules distincts ayant eu au moins un bulletin de paie dans l'année, en fonction ou non.Peut correspondre à des régularisations, des personnels hors position d'activité ou des ayants droit (reversion, etc.)*   
#'*(b) Titulaires ou stagiaires*   
#'*(c) Sur la base des libellés d'emploi et des libellés de lignes de paye. La détection peut être lacunaire*   
#'*(d) ETP  : Equivalent temps plein = rémunération . quotité*  
#'*(e) ETPT : Equivalent temps plein travaillé = ETP . 12/nombre de mois travaillés dans l'année*  
#'*(f) Personnes en place : présentes en N et N-1 avec la même quotité, postes actifs et non annexes uniquement.*     
#'*(g) Postes actifs et non annexes :* voir [Compléments méthodologiques](Docs/méthodologie.pdf)    
#'*&nbsp;&nbsp;&nbsp;Un poste actif est défini par au moins un bulletin de paie comportant un traitement positif pour un volume d'heures de travail mensuel non nul.*             
#'*&nbsp;&nbsp;&nbsp;Un poste non annexe est défini comme la conjonction de critères horaires et de revenu sur une année. La période minimale de référence est le mois.*   
#'*Les dix dernières lignes du tableau sont calculées en ne tenant pas compte des élus.*      
#'   
#'[Lien vers la base des effectifs](Bases/Effectifs/tableau.effectifs.csv)
#'
#'
message("Statistiques de démographie réalisées.")

#'### `r chapitre`.2 Pyramide des âges, personnels non élus

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    
Résumé(c("Âge des personnels <br>au 31/12/" %+% début.période.sous.revue,
         "Effectif"),
       années.total.hors.élus.début,
       extra = "length",
       align = 'c')
#'  
if (longueur.non.na(années.total.hors.élus.début) > 0)
  hist(années.total.hors.élus.début,
       xlab = "Âge au 31 décembre " %+% début.période.sous.revue,
       xlim = c(18, 75),
       ylab = "Effectif",
       main = "Pyramide des âges des personnels (non élus)",
       col = "blue",
       nclass = 50)

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Âge des personnels <br>au 31/12/" %+% fin.période.sous.revue,
         "Effectif"),
       années.total.hors.élus,
       extra = "length",
       align = 'c')

#'  
#'  
if (longueur.non.na(années.total.hors.élus) > 0)
  hist(années.total.hors.élus,
       xlab = "Âge au 31 décembre " %+% fin.période.sous.revue,
       xlim = c(18, 75),
       ylab = "Effectif",
       main = "Pyramide des âges des personnels (non élus)",
       col = "blue",
       nclass = 50)

#'
#'### `r chapitre`.3 Pyramide des âges, personnels non titulaires


#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Âge des personnels non titulaires<br>au 31/12/" %+% début.période.sous.revue,
         "Effectif"),
       années.total.nontit.début,
       extra = "length",
       align = 'c')

#'  
if (longueur.non.na(années.total.nontit.début) > 0)
  hist(années.total.nontit.début,
       xlab = "Âge au 31 décembre " %+% début.période.sous.revue,
       xlim = c(18, 75),
       ylab = "Effectif",
       main = "Pyramide des âges des personnels non titulaires",
       col = "blue",
       nclass = 50)
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Âge des personnels non titulaires<br>au 31/12/" %+% fin.période.sous.revue,
         "Effectif"),
       années.total.nontit,
       extra = "length",
       align = 'c')

	   
if (longueur.non.na(années.total.nontit) > 0)
  hist(années.total.nontit,
       xlab = "Âge au 31 décembre " %+% fin.période.sous.revue,
       xlim = c(18, 75),
       ylab = "Effectif",
       main = "Pyramide des âges des personnels non titulaires",
       col = "blue",
       nclass = 50)
	   
	   
#'  
#'[Lien vers la base des âges](Bases/Effectifs/Bulletins.paie.nir.nontit.csv)  
#'  
#'
#'
#'### `r chapitre`.4 Pyramide des âges, personnels fonctionnaires stagiaires et titulaires

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Âge des personnels <br>au 31/12/" %+% début.période.sous.revue,
         "Effectif"),
       années.fonctionnaires.début,
       extra = "length",
       align = 'c')

#'  
#'

if (longueur.non.na(années.fonctionnaires.début) > 0)
  hist(années.fonctionnaires.début,
       xlab = "Âge au 31 décembre " %+% début.période.sous.revue,
       xlim = c(18,68),
       ylab = "Effectif",
       main = "Pyramide des âges des fonctionnaires",
       col = "navy",
       nclass = 50)

#'  

#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Âge des personnels <br>au 31/12/" %+% fin.période.sous.revue,
         "Effectif"),
       années.fonctionnaires,
       extra = "length",
       align = 'c')

#'

if (longueur.non.na(années.fonctionnaires) > 0)
  hist(années.fonctionnaires,
       xlab = "Âge au 31 décembre " %+% fin.période.sous.revue,
       xlim = c(18,68),
       ylab = "Effectif",
       main = "Pyramide des âges des fonctionnaires",
       col = "navy",
       nclass = 50)

#'  
#'[Lien vers la base des âges](Bases/Effectifs/Bulletins.paie.nir.fonctionnaires.csv)  
#'  

#'### `r chapitre`.5 Pyramide des âges, personnels permanents (titulaires, stagiaires et non titulaires)

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Âge des personnels <br>au 31/12/" %+% début.période.sous.revue,
         "Effectif"),
       années.total.permanents.début,
       extra = "length",
       align = 'c')

#'  

if (longueur.non.na(années.total.permanents.début) > 0)
  hist(années.total.permanents.début,
       xlab = "Âge au 31 décembre " %+% début.période.sous.revue,
       xlim = c(18,68),
       ylab = "Effectif",
       main = "Pyramide des âges des permanents",
       col = "navy",
       nclass = 50)

#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Âge des personnels <br>au 31/12/" %+% fin.période.sous.revue,
         "Effectif"),
       années.total.permanents,
       extra = "length",
       align = 'c')

#'

if (longueur.non.na(années.total.permanents) > 0)
  hist(années.total.permanents,
       xlab = "Âge au 31 décembre " %+% fin.période.sous.revue,
       xlim = c(18,68),
       ylab = "Effectif",
       main = "Pyramide des âges des permanents",
       col = "navy",
       nclass = 50)

#'  
#'[Lien vers la base des âges](Bases/Effectifs/Bulletins.paie.nir.permanents.csv)  
#'  

#'### `r chapitre`.6 Effectifs des personnels par durée de service
#'
#'**Personnels en fonction (hors élus) des exercices `r début.période.sous.revue` à `r fin.période.sous.revue` inclus :**
#'

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    



Tableau(c("Plus de 2 ans",
          "Moins de 2 ans",
          "Moins d'un an",
          "Moins de six mois"),
        sum(Analyse.variations.synthèse$plus.2.ans, na.rm=TRUE),
        sum(Analyse.variations.synthèse$moins.2.ans, na.rm=TRUE),
        sum(Analyse.variations.synthèse$moins.1.an, na.rm=TRUE),
        sum(Analyse.variations.synthèse$moins.six.mois, na.rm=TRUE))


#'

if (nrow(Analyse.variations.par.exercice) > 0)
  qplot(factor(Année),
        data = Analyse.variations.par.exercice,
        geom = "bar",
        fill = factor(!Analyse.variations.par.exercice$plus.2.ans),
        main = "Evolutions entre " %+% début.période.sous.revue %+% " et " %+% fin.période.sous.revue,
        xlab = étiquette.année,
        ylab = "Effectif",
        asp = 1.4)        +
  scale_fill_discrete(name = "Personnels (non élus) en fonction",
                      breaks = c(TRUE, FALSE),
                      labels = c("Moins de deux ans", "Plus de deux ans"))



#'
#'**Effectifs (hors élus)**   
#'

effectifs.var <- lapply(période,
                        function(x) {

                          E <- unique(Analyse.variations.par.exercice[Année == x , .(Matricule, plus.2.ans)], by=NULL)
                          F <- E[plus.2.ans == TRUE]
                          tot <- nrow(E)
                          plus.2.ans <- nrow(F)
                          résultat <- c(plus.2.ans, tot - plus.2.ans, tot)
                          rm(E, F, tot, plus.2.ans)
                          résultat
                        })

effectifs.var.locale <- lapply(effectifs.var, function(x) formatC(x, big.mark = " ", format="f", digits=0, decimal.mark=","))

tableau.effectifs.var <- as.data.frame(effectifs.var.locale, row.names = c("Plus de deux ans", "Moins de deux ans", "Total"))

names(tableau.effectifs.var) <- liste.années
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
                            "Statut",
                            "Grade",
                            "Filtre_actif_non_annexe",
                            clé.fusion)


########### Analyse statique ########################

invisible(lapply(années.analyse.statique, function(x) {
                 année <<- x
                 incrémenter.chapitre()
                 if (! produire.rapport) {
                   
                   source('analyse.statique.R', encoding = encodage.code.source) 
                   
                 } else {
                                      
                   cat(knit_child(text = readLines(file.path(chemin.dossier,'analyse.statique.Rmd'), encoding = encodage.code.source), quiet=TRUE), sep = '\n')
                 }
               }))

#'  
#'[Lien vers la base des rémunérations](Bases/Rémunérations/Analyse.rémunérations.csv)  
#'   

########### INSEE DGCL ###############################
#'   
#'## `r chapitre`.4 Comparaisons source INSEE/DGCL   
#'   
#'*Salaires annnuels moyens 2011 en EQTP (hors assistantes maternelles)*   

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Agrégat", "Salaires bruts 2011 (&euro;)", "Salaires bruts 2012 (&euro;)"),
                  c("Ensemble", "Titulaires", "Autres salariés"),
                  12 * c(2159, 2223, 1903),
                  12 * c(2195, 2259, NA))

#'   
#'**Eléments de la rémunération brute pour les titulaires de la FPT entre 2010 et 2012**      
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(
  c("Rém. annuelles", "2010 (&euro;)", "Primes (%)", "2011 (&euro;)", "Primes (%)", "2012 (&euro;)", "Primes (%)"),
  c("Salaire brut",
    "Traitement brut",
    "Primes et rémunérations annexes",
    "y compris IR et SFT"),
  c(26305, 20350,	"", 5955),
  c("", 22.6, "", "" ),
  c(26660, 20562, "", 6098),
  c("", 22.9, "", "" ),
  c(12*2259, 12*1727, "", 12*532),
  c("", 23.6, "", "" ))
#'   
#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*   												
#'*Les primes sont cumulées au supplément familial de traitement (SFT) et à l'indemnité de résidence (IR). Le cumul est rapporté à la rémunération brute totale.*    
#'[Source INSEE](http://www.insee.fr/fr/ffc/ipweb/ip1486/ip1486.xls)    
#'[Source DGCL](http://www.fonction-publique.gouv.fr/files/files/statistiques/rapports_annuels/2012-2013/xls/Vue3_1_Remunerations.xls)    
#'[Source DGFIP PLF 2015](http://www.performance-publique.budget.gouv.fr/sites/performance_publique/files/farandole/ressources/2015/pap/pdf/jaunes/jaune2015_fonction_publique.pdf)   
#'   

incrémenter.chapitre()

########### Analyse dynamique ########################
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
#'    
#'## `r chapitre`.1 Distribution de la rémunération nette moyenne sur la période    

Analyse.variations.par.exercice <- Analyse.variations.par.exercice[nb.jours > seuil.troncature
                                                                   & ! is.na(Montant.net.annuel.eqtp)
                                                                   & Montant.net.annuel.eqtp  > minimum.positif 
                                                                   & ! is.na(Statut)] 

attach(Analyse.variations.synthèse)

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

detach(Analyse.variations.synthèse)

#'
#'[Lien vers la base de données synthétique](Bases/Rémunérations/Analyse.variations.synthèse.csv)
#'[Lien vers la base de données détaillée par année](Bases/Rémunérations/Analyse.variations.par.exercice.csv)
#'
#'## `r chapitre`.2 Evolutions des rémunérations nettes sur la période `r début.période.sous.revue` - `r fin.période.sous.revue`   
#'
#'### `r chapitre`.2.1 Ensemble des personnels fonctionnaires et non titulaires (hors élus)
#'
#'


masse.salariale.nette <- rep(0, durée.sous.revue)

f <- function(x) prettyNum(masse.salariale.nette[x - début.période.sous.revue + 1] <<- 
                             sum(Analyse.variations.par.exercice[Année == x, 
                                                               Montant.net.annuel.eqtp],
                               na.rm = TRUE) / 1000,
                           big.mark = " ",
                           digits = 5,
                           format = "fg")

g <- function(x) prettyNum(mean.default(Analyse.variations.par.exercice[Année == x, 
                                                               Montant.net.annuel.eqtp],
                               na.rm = TRUE),
                           big.mark = " ",
                           digits = 1,
                           format = "fg")
#'    
#'**Salaire net moyen par tête (SMPT net) en EQTP, hors élus**         
#'       
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical(c(étiquette.année, "Rém. nette totale (k&euro;)", "SMPT net (&euro;)"),
                 période,
                 extra = "variation",
                 f,
                 g)

entrants <- function(x)   {
  
  A <- setdiff(Analyse.variations.par.exercice[Année == x, Matricule], 
               Analyse.variations.par.exercice[Année == x -1, Matricule])
  

  B <- unique(Bulletins.paie[Année == x 
                             & Matricule %chin% A, 
                               .(Année, quotité, Matricule, Mois, Statut)], by = NULL)

  eqtp.agent <- B[ , sum(quotité, na.rm=TRUE)] / 12
  eqtp.fonct <- B[Statut == "TITULAIRE" | Statut == "STAGIAIRE", sum(quotité, na.rm=TRUE)] / 12

  list(A, eqtp.agent, eqtp.fonct)
}

sortants <- function(x)   {
  
  A <- setdiff(Analyse.variations.par.exercice[Année == x-1, Matricule], 
               Analyse.variations.par.exercice[Année == x, Matricule])
    
  B <- unique(Bulletins.paie[Année == x - 1
                             & Matricule %chin% A,
                               .(Année, quotité, Matricule, Mois, Statut)], by = NULL)
  
  eqtp.agent <- B[ , sum(quotité, na.rm=TRUE)] / 12
  eqtp.fonct <- B[Statut == "TITULAIRE" | Statut == "STAGIAIRE", sum(quotité, na.rm=TRUE)] / 12
  
  list(A, eqtp.agent, eqtp.fonct)
}

s <- list(0)
e <- list(0)
noria <- rep(0, durée.sous.revue)
remplacements <- rep(0, durée.sous.revue)

f <- function(x) {
  y <- x - début.période.sous.revue
  
  s[[y]] <<- sortants(x)
  e[[y]] <<- entrants(x)
  
  noria[y] <<- mean.default(Analyse.variations.par.exercice[Année == x 
                                                            & Matricule %chin% e[[y]][[1]], 
                                                              Montant.net.annuel.eqtp],
                    na.rm = TRUE) - mean.default(Analyse.variations.par.exercice[Année == x- 1 
                                                                                 & Matricule %chin% s[[y]][[1]], 
                                                                                     Montant.net.annuel.eqtp],
                                                 na.rm = TRUE)
  
  prettyNum(noria[y],
            big.mark = " ",
            digits = 5,
            format = "fg")
}

g <- function(x) {
  
  y <- x - début.période.sous.revue

  remplacements[y] <<- min(e[[y]][[2]], s[[y]][[2]], na.rm=TRUE)
  
  prettyNum(noria[y] * remplacements[y] / (masse.salariale.nette[y] * 10),
                           big.mark = " ",
                           digits = 3,
                           format = "fg")
}
#'   
#'**Effet de noria sur salaires nets et taux de remplacements**       
#'   
#'**Effet de noria** : *différence entre la rémunération annuelle des entrants à l'année N et des sortants à l'année N-1*.  
#'*Usuellement calculée sur les rémunérations brutes, ici sur les rémunérations nettes EQTP*  
#'*afin d'apprécier l'impact de cet effet sur l'évolution des rémunérations nette moyennes calculée au tableau précédent.*               
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    
if (durée.sous.revue > 1) {
Tableau.vertical(c(étiquette.année,  "Noria EQTP (&euro;)", "En % de la MS N-1", "Remplacements EQTP", "Taux de remplacements (%)"),
                 période[2:durée.sous.revue],
                 extra = "no",
                 f,
                 g,
                 function(x) prettyNum(remplacements[x - début.période.sous.revue], 
                                       digits=0,
                                       format="f"),
                 function(x) prettyNum(remplacements[x - début.période.sous.revue] / effectifs[[as.character(x)]]["ETPT"] * 100,
                                       digits=2,
                                       format="f"))
} else {
  cat("L'effet de noria ne peut être calculé que pour des durées sous revue supérieures à un exercice.")
}

#'
#'*MS N-1 : masse salariale nette de l'année n-1.*   
#'       
#'**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         
#'       
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé("Première année",
       Analyse.variations.par.exercice[Année == début.période.sous.revue, Montant.net.annuel.eqtp])

Résumé("Dernière année",
       Analyse.variations.par.exercice[Année == fin.période.sous.revue, Montant.net.annuel.eqtp])


#'  
#'*Nota :*  La population retenue est constituée des agents qui :   
#'&nbsp;&nbsp;- ne font pas partie des `r 2*quantile.cut` centiles extrêmaux   
#'&nbsp;&nbsp;- sont au moins présents `r seuil.troncature` jour(s) la première et la dernière année d'activité  
#'Les élus, vacataires et assistantes maternelles sont retirés du périmètre.   
#'Seuls sont pris en compte les agents ayant connu au moins un mois actif et ayant eu, sur l'année, des rémunérations non annexes.  
#'[Compléments méthodologiques](Docs/méthodologie.pdf)     
#'      
#'**Comparaisons source INSEE/DGCL**   
#'
#'**Salaires annuels moyens 2011 et 2012 en EQTP (hors assistantes maternelles)**   
#'  
#'&nbsp;*Tableau `r incrément()`*       

  
Tableau.vertical2(c("Agrégat",  "Salaires nets 2011 (&euro;)", "Salaires nets 2012 (&euro;)"),
                  c("Ensemble", "Titulaires", "Autres salariés"),
                  12*c(1823, 1886, 1572),
                  12*c(1848, 1910, NA))

#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			


matrice.déciles <- t(matrix(12*c(1458, 1274, 1382, 1170, 1743, 1376, 1514, 1305, 1921, 1459, 1635, 1428, 2076, 1540, 1754,
                                 1559, 2236, 1636, 1883, 1712, 2412, 1751, 2042, 1902, 2636, 1905, 2268, 2156, 2966, 2133,
                                 2583, 2569, 3538, 2573, 3151, 3400),  ncol = 9))


#'**Distribution des salaires nets annuels en EQTP dans la fonction publique par versant en 2011**   
#' 
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


Tableau.vertical2(c("Décile (k&euro;)", "FPE", "FPT", "FPH", "Secteur privé"),
                  paste0("D", 1:9),
                  matrice.déciles[,1],
                  matrice.déciles[,2],
                  matrice.déciles[,3],
                  matrice.déciles[,4])


matrice.déciles.cat <- matrix(12*c(2170,2416,2606,2789,2985,3222,3523,3927,4570,1823,
                                    1715,1856,1971,2080,2187,2303,2430,2582,2817,3225,
                                    1331,1408,1471,1530,1597,1675,1768,1890,2083,2244,
                                    1135,1195,1252,1307,1364,1436,1540,1732,2243,1668), nrow = 10)



#'**Distribution des salaires nets annuels en EQTP dans la fonction publique territoriale par catégorie en 2011**   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Décile (k&euro;)", "Catégorie A", "Catégorie B", "Catégorie C", "Autres salariés"),
                  c(paste0("D", 1:9), "Moyenne"),
                  matrice.déciles.cat[,1],
                  matrice.déciles.cat[,2],
                  matrice.déciles.cat[,3],
                  matrice.déciles.cat[,4])

#'[Source INSEE, onglets Figure3, F1web et F3web](http://www.insee.fr/fr/ffc/ipweb/ip1486/ip1486.xls)   
#'   
#'[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)
#'   

#'### `r chapitre`.2.2 Fonctionnaires
#'
#'**Titulaires et stagiaires**      

f <- function(x) {

  masse.salariale.nette[x - début.période.sous.revue + 1] <<-  sum(Analyse.variations.par.exercice[Année == x
                                                                                                   & (Statut == "TITULAIRE" | Statut == "STAGIAIRE"), 
                                                                                                      Montant.net.annuel.eqtp],
                                                                na.rm = TRUE) / 1000

 prettyNum(masse.salariale.nette[x - début.période.sous.revue + 1],
                           big.mark = " ",
                           digits = 5,
                           format = "fg")
}

g <- function(x) prettyNum(mean.default(Analyse.variations.par.exercice[Année == x 
                                                                        & (Statut == "TITULAIRE" | Statut == "STAGIAIRE"), 
                                                                         Montant.net.annuel.eqtp],
                           na.rm = TRUE),
                   big.mark = " ",
                   digits = 1,
                   format = "fg")


#'**Salaire net moyen par tête (SMPT net) en EQTP**       
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical(c(étiquette.année, "Rém. nette totale (k&euro;)", "SMPT net en EQTP (&euro;)"),
                 période,
                 extra = "variation",
                 f,
                 g)

#'
f <- function(x) {
  y <- x - début.période.sous.revue
  
  noria[y] <<- sum(Analyse.variations.par.exercice[Année == x 
                                                   & (Statut == "TITULAIRE" | Statut == "STAGIAIRE") 
                                                   & Matricule %chin% e[[y]][[1]], 
                                                     Montant.net.annuel.eqtp],
                   na.rm = TRUE) / e[[y]][[3]] -  sum(Analyse.variations.par.exercice[Année == x- 1
                                                                                      & (Statut == "TITULAIRE" | Statut == "STAGIAIRE") 
                                                                                      & Matricule %chin% s[[y]][[1]],
                                                                                        Montant.net.annuel.eqtp],
                                                  na.rm = TRUE) / s[[y]][[3]]
  
  prettyNum(noria[y],
            big.mark = " ",
            digits = 5,
            format = "fg")
}

g <- function(x) {
  
  y <- x - début.période.sous.revue

  remplacements[y] <<- min(e[[y]][[3]], s[[y]][[3]], na.rm=TRUE)
  
  prettyNum(noria[y] * remplacements[y] / (masse.salariale.nette[y] * 10),
            big.mark = " ",
            digits = 3,
            format = "fg")
}

#'   
#'**Effet de noria sur salaires nets et taux de remplacements**       
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

if (durée.sous.revue > 1) {
Tableau.vertical(c(étiquette.année,  "Noria EQTP (&euro;)", "En % de la  MSN N-1", "Remplacements EQTP", "Taux de remplacements (%)"),
                 période[2:length(période)],
                 extra = "no",
                 f,
                 g,
                 function(x) prettyNum(remplacements[x - début.période.sous.revue], digits=0, format="f"),
                 function(x) prettyNum(remplacements[x - début.période.sous.revue]/ effectifs[[as.character(x)]]["ETPT_fonct"] * 100, digits=2, format="f"))
} else {
  cat("L'effet de noria ne peut être calculé que pour des durées sous revue supérieures à un exercice.")
}
#'     
#'*MS N-1 : masse salariale nette de l'année n-1.*   
#'       
#'**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) en EQTP**         
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


Résumé("Première année",
       Analyse.variations.par.exercice[Année == début.période.sous.revue
                                       & (Statut == "TITULAIRE" | Statut == "STAGIAIRE"),
                                           Montant.net.annuel.eqtp])

Résumé("Dernière année",
       Analyse.variations.par.exercice[Année == fin.période.sous.revue 
                                       & (Statut == "TITULAIRE" | Statut == "STAGIAIRE"),
                                         Montant.net.annuel.eqtp])


#'    
f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[Année == x 
                                                               & Statut == "TITULAIRE"
                                                               & temps.complet == TRUE & permanent == TRUE, 
                                                                 Montant.net.annuel.eqtp],
                               na.rm = TRUE) / 1000,
                           big.mark = " ",
                           digits = 5,
                           format = "fg")

g <- function(x) prettyNum(mean.default(Analyse.variations.par.exercice[Année == x
                                                               & Statut == "TITULAIRE"
                                                               & temps.complet == TRUE & permanent == TRUE, 
                                                                 Montant.net.annuel.eqtp],
                                        na.rm = TRUE),
                           big.mark = " ",
                           digits = 1,
                           format = "fg")

#'   
#'**Evolution du SMPT net des titulaires à temps complet**     
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical(c(étiquette.année, "Rémunération nette totale (k&euro;)", "SMPT (&euro;)"),
                 période,
                 extra = "variation",
                 f,
                 g)

#'    
#'**Distribution et variation sur la période du salaire moyen net par tête (SMPT net) des titulaires à temps complet**         
#'       
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


Résumé("Première année",
       Analyse.variations.par.exercice[Année == début.période.sous.revue
                                       & Statut == "TITULAIRE" 
                                       & temps.complet == TRUE
                                       & permanent == TRUE,
                                           Montant.net.annuel.eqtp])

#'   
Résumé("Dernière année",
       Analyse.variations.par.exercice[Année == fin.période.sous.revue
                                       & Statut == "TITULAIRE" 
                                       & temps.complet == TRUE
                                       & permanent == TRUE,
                                         Montant.net.annuel.eqtp])


#'
#'[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)
#'
#'

#'## `r chapitre`.3 Glissement vieillesse-technicité (GVT)   
#'
#'### `r chapitre`.3.1 Ensemble des personnels   
#'   
#'*Cette section est consacrée à la rémunération moyenne des personnes en place (RMPP), définies comme présentes deux années entières consécutives avec la même quotité*   
#'*L'évolution de la RMPP permet d'étudier le glissement vieillesse-technicité "positif", à effectifs constants sur deux années*      
#'*Le GVT positif est dû aux mesures statutaires et individuelles, à l'avancement et aux changements d'activité*  


# Appliquer les filtres maintenant

q3 <- quantile(Analyse.variations.synthèse$variation.rémunération, c(quantile.cut/100, 1 - quantile.cut/100), na.rm=TRUE)

# Filtrage : on enlève les personnels présents depuis moins d'un seuil de troncature (ex. 120 jours) dans l'année et les élus
# (paramètre seuil.troncature) 

# Filtrage pour l'étude des variations : on enlève les valeurs manquantes des variations, les centiles extrêmaux,
# les rémunérations nettes négatives ou proche de zéro. On exige un statut explicite en fin de période.
# Paramétrable par :
# minimum.positif, quantile.cut 


Analyse.variations.synthèse <- Analyse.variations.synthèse[total.jours > 2 * seuil.troncature
                                                           & pris.en.compte == TRUE
                                                           & ! is.na(statut)   
                                                           & ! is.na(variation.rémunération) 
                                                           & variation.rémunération > q3[[1]]
                                                           & variation.rémunération < q3[[2]]]

Analyse.variations.synthèse.plus.2.ans  <- Analyse.variations.synthèse[! is.na(plus.2.ans) & plus.2.ans == TRUE]
Analyse.variations.synthèse.moins.2.ans <- Analyse.variations.synthèse[! is.na(plus.2.ans) & plus.2.ans == FALSE]

#Analyse.variations.par.exercice <- Analyse.variations.par.exercice[Nexercices > 1]


if (nrow(Analyse.variations.synthèse.plus.2.ans) > 0)
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

#'
#'

f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[Année == x
                                                               & est.rmpp == TRUE,
                                                                 Montant.net.annuel.eqtp],
                               na.rm = TRUE)/ 1000,
                           big.mark = " ",
                           digits = 5,
                           format = "fg")

g <- function(x) prettyNum(mean.default(Analyse.variations.par.exercice[Année == x 
                                                                        & est.rmpp == TRUE,
                                                                          Montant.net.annuel.eqtp],
                               na.rm = TRUE) ,
                           big.mark = " ",
                           digits = 1,
                           format = "fg")
#'   
#'**Evolution de la RMPP nette en EQTP**     
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical(c(étiquette.année,
                   "Rémunération nette totale (k&euro;)",
                   "RMPP nette (k&euro;)"),
                 période[2:durée.sous.revue],
                 extra = "variation",
                 f,
                 g)


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
masque.présent.sur.période <- bitwShiftL(1, durée.sous.revue) -1       #  11111..1

#'  
Résumé(c("Première année",
         "Effectif"),
       Analyse.variations.synthèse[bitwAnd(indicatrice.période, masque.rmpp.début.période) == masque.rmpp.début.période, 
                                           Montant.net.annuel.eqtp.début],
       extra = "length")
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Dernière année",
         "Effectif"),
        Analyse.variations.synthèse[indicatrice.période >= masque.rmpp.fin.période, Montant.net.annuel.eqtp.sortie],
        extra = "length")
#'
#'*Variation individuelle de rémunération nette en EQTP pour les personnels présents la première et la dernière année*   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Variation normalisée (%)",
         "Variation annuelle moyenne normalisée (%)",
         "Effectif"),
         Analyse.variations.synthèse[bitwAnd(indicatrice.période, masque.présent.début.fin) 
                                        == 
                                     masque.présent.début.fin,
                                       .(variation.rémunération.normalisée,
                                         variation.moyenne.rémunération.normalisée)],
       extra = "length")

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

f <- function(x) prettyNum(sum(Analyse.variations.par.exercice[Année == x
                                                               & est.rmpp == TRUE
                                                               & (Statut == "TITULAIRE" | Statut == "STAGIAIRE"),
                                                                 Montant.net.annuel.eqtp],
                               na.rm = TRUE)/ 1000,
                           big.mark = " ",
                           digits = 5,
                           format = "fg")

g <- function(x) prettyNum(mean.default(Analyse.variations.par.exercice[Année == x 
                                                                        & est.rmpp == TRUE
                                                                        & (Statut == "TITULAIRE" | Statut == "STAGIAIRE"),
                                                                          Montant.net.annuel.eqtp],
                                        na.rm = TRUE) ,
                           big.mark = " ",
                           digits = 1,
                           format = "fg")
#'   
#'**Evolution de la RMPP nette en EQTP**     
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    


Tableau.vertical(c(étiquette.année,
                   "Rémunération nette totale (k&euro;)",
                   "RMPP nette (k&euro;)"),
                 période[2:durée.sous.revue],
                 extra = "variation",
                 f,
                 g)

#'    
#'**Distribution et variation sur la période de la rémunération nette des fonctionnaires en place**                
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

#'  
Résumé(c("Première année",
         "Effectif"),
       Analyse.variations.synthèse[(statut == "TITULAIRE" | statut == "STAGIAIRE")
                                   & bitwAnd(indicatrice.période, masque.rmpp.début.période) == masque.rmpp.début.période, 
                                     Montant.net.annuel.eqtp.début],
       extra = "length")
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Dernière année",
         "Effectif"),
       Analyse.variations.synthèse[statut == "TITULAIRE" | statut == "STAGIAIRE"
                                   & indicatrice.période >= masque.rmpp.fin.période, 
                                     Montant.net.annuel.eqtp.sortie],
       extra = "length")
#'
#'*Variation individuelle de rémunération nette en EQTP pour les personnels présents la première et la dernière année*   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Résumé(c("Variation normalisée (%)",
         "Variation annuelle moyenne normalisée (%)",
         "Effectif"),
       Analyse.variations.synthèse[(statut == "TITULAIRE" | statut == "STAGIAIRE")
                                   & bitwAnd(indicatrice.période, masque.présent.début.fin)
                                      ==
                                     masque.présent.début.fin,
                                     .(variation.rémunération.normalisée,  variation.moyenne.rémunération.normalisée)],
       extra = "length")


#'
#'
#'[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)
#'
#'**Nota**   
#'*Personnes en place :* en fonction au moins deux années consécutives sur la période `r début.période.sous.revue` à `r fin.période.sous.revue`    
#'*Variation sur la période d'activité :* entre l'arrivée et le départ de la personne      
#'*Variation normalisée :* conforme à la définition INSEE (présente en début et en fin de période avec la même quotité)  
#'  
#'**Commentaire**       
#'Les différences éventuelles constatées entre l'évolution de la RMPP au tableau `r numéro.tableau-2` sont dues soit à l'effet de noria soit à l'effet périmètre.    
#'      

#'[Lien vers la base de données](Bases/Rémunérations/Analyse.variations.synthèse.csv)
#'
#'
#'### `r chapitre`.4 Comparaisons avec la situation nationale des rémunérations   
#'  
#'**Évolution en euros courants du SMPT et de la RMPP dans la FPT (en % et euros courants)**    

#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Année", "2008-2009", "2009-2010", "2010-2011", "2011-1012", "2008-2012", "Moy. 2008-2012", "Médiane 2007-2011"),
                  c("SMPT brut", "SMPT net", "RMPP brute", "RMPP nette"),         
                  t(data.frame(c("2,5", "1,3", "1,5", "1,7", "7,2", "1,8", ""),
                  c("3,0", "1,4", "1,3", "1,4", "7,3", "1,8", "13,4"),
                  c("3,3", "2,5", "2,5", "2,7", "11,5", "2,8", ""),
                  c("3,3", "2,5", "2,3", "2,4", "10,9", "2,6", ""))))


#'*Source : fichier général de l'État (FGE), DADS, SIASP, Insee, Drees. Traitement Insee, Drees, DGCL*    
#'Hors assistants maternels et familiaux, y compris bénéficiaires de contrats aidés.   
#'SMPT : Salaire moyen par tête en EQTP.   
#'RMPP : Agents présents 24 mois consécutifs chez le même employeur avec la même quotité de travail.  
#'Moyenne des variations géométriques annuelles pour les agents du champ.  
#'La dernière colonne présente la médiane des augmentations du SMPT net pour les agents présents en 2007 et 2011.   
#'  
#'**Salaires nets annuels et évolution moyenne type de collectivité en &euro; courants  EQTP**    
#'   
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau.vertical2(c("Type de collectivité", "SMPT net 2011 (&euro;)", "SMPT net 2012 (&euro;)", "Évolution annuelle moy. 2007-2011 (%)"),
  c("Communes",
    "CCAS et caisses des écoles",
    "EPCI à fiscalité propre",
    "Autres structures intercommunales",
    "Départements",
    "SDIS",
    "Régions",		 
    "Autres collectivités locales",	 
    "Ensemble (moyenne)"),	
   c(20784, 19415, 22882, 21299, 24487, 29811, 22432, 24680, 21873),
  12*c(1760, 1643, 1924, 1807, 2062, 2495, 1903,  2058, 1848),
   c("2,5", "2,4", "3,1", "3,0", "3,9", "3,4", "3,8", "3,2", "2,9"))

#'
#'*Champ : France. Salariés en équivalent-temps plein (EQTP) des collectivités territoriales (y compris bénéficiaires de contrats aidés, hors assistantes maternelles).*     			
#'Conversion en euros courants, calcul CRC.  
#'[Source INSEE données 2011 obsolètes](http://www.insee.fr/fr/ffc/ipweb/ip1486/ip1486.xls)   
#'[Source DGAFP](http://infos.emploipublic.fr/dossiers/la-fonction-publique-en-chiffres/la-fonction-publique-en-chiffre-2013/non-defini-08/apm-71444/)  
#'[Source PLF 2014 données 2011 révisées p.151](http://www.fonction-publique.gouv.fr/files/files/statistiques/jaunes/jaune2014_FP.pdf)   
#'[Source PLF 2015 données 2012 p.130](http://www.performance-publique.budget.gouv.fr/sites/performance_publique/files/farandole/ressources/2015/pap/pdf/jaunes/jaune2015_fonction_publique.pdf)   
#'    

incrémenter.chapitre()


#'
########### Tests statutaires ########################
#'

#'# `r chapitre`. Tests réglementaires   
#'## `r chapitre`.1 Contrôle des heures supplémentaires, des NBI et primes informatiques   
#'   
#'**Dans cette partie, l'ensemble de la base de paye est étudié.**  
#'Les agents non actifs ou dont le poste est annexe sont réintroduits dans le périmètre.   

if (N <- uniqueN(Paie[Statut != "TITULAIRE"
                            & Statut != "STAGIAIRE"
                            & NBI != 0, 
                              Matricule]))
  cat("Il existe ", N, "non titulaire", ifelse(N>1, "s", ""), " percevant une NBI.")

NBI.aux.non.titulaires <- Paie[Statut != "TITULAIRE"
                               & Statut != "STAGIAIRE"
                               & NBI != 0
                               & grepl(expression.rég.nbi, Libellé, ignore.case=TRUE, perl=TRUE),
                                 c(étiquette.matricule,
                                   "Statut",
                                   étiquette.code,
                                   étiquette.libellé,
                                   étiquette.année,
                                   "Mois",
                                   "NBI",
                                   étiquette.montant),
                                 with=FALSE]

nombre.Lignes.paie.NBI.nontit <- nrow(NBI.aux.non.titulaires)

# Prime de fonctions informatiques : pas dans la base de VLB
# on cherche la chaine de char. "INFO" dans les libellés de primes

# variante : filtre <- regexpr(".*(INFO|PFI|P.F.I).*", toupper(Paie$Libellé)) et regmatches(Paie$Libellé, filtre)

attach(Paie, warn.conflicts=FALSE)
filtre <- grep(expression.rég.pfi, Libellé, ignore.case=TRUE, perl=TRUE)

personnels.prime.informatique <- Paie[ filtre,
                                         c(étiquette.matricule,
                                           étiquette.année,
                                           "Mois",
                                           "Statut",
                                           étiquette.code,
                                           étiquette.libellé,
                                           étiquette.montant),
                                           with=FALSE]

primes.informatiques.potentielles <- unique(Libellé[filtre], by=NULL)

if  (length(primes.informatiques.potentielles) == 0)
  primes.informatiques.potentielles <- "aucune"

nombre.personnels.pfi <- nrow(personnels.prime.informatique)

detach(Paie)
#'Primes informatiques potentielles : `r primes.informatiques.potentielles`
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau(
  c("Nombre de lignes NBI pour non titulaires",
    "Nombre de bénéficiaires de PFI"),
  nombre.Lignes.paie.NBI.nontit,
  nombre.personnels.pfi)

#'   
#'[Lien vers la base de données NBI aux non titulaires](Bases/Réglementation/NBI.aux.non.titulaires.csv)   
#'[Lien vers la base de données Primes informatiques](Bases/Réglementation/personnels.prime.informatique.csv)   
#'   
#'**Nota :**   
#'NBI: nouvelle bonification indiciaire   
#'PFI: prime de fonctions informatiques   
#'  
#'## `r chapitre`.2 Contrôle des vacations pour les fonctionnaires

# Vacations et statut de fonctionnaire

  lignes.fonctionnaires.et.vacations <- Paie[(Statut == "TITULAIRE" | Statut == "STAGIAIRE") & Grade == "V",
                                                c(étiquette.matricule,
                                                  "Nom", "Prénom",
                                                  "Statut",
                                                  étiquette.code,
                                                  étiquette.libellé,
                                                  étiquette.montant),
                                                 with=FALSE]
   
  matricules.fonctionnaires.et.vacations <- unique(lignes.fonctionnaires.et.vacations[ , .(Matricule, Nom, Prénom)], by=NULL)
  nombre.fonctionnaires.et.vacations <- nrow(matricules.fonctionnaires.et.vacations)
#'

if (! is.null(nombre.fonctionnaires.et.vacations)) {
  cat("Il y a ",
      nombre.fonctionnaires.et.vacations,
      "fonctionnaire(s) effectuant des vacations pour son propre établissement. Les bulletins concernés sont donnés en lien." )
}  else  {
  cat("Pas de vacation détectée.")
}


#'
#'[Lien vers les matricules des fonctionnaires concernés](Bases/Réglementation/matricules.fonctionnaires.et.vacations.csv)
#'[Lien vers les bulletins de paye correspondants](Bases/Réglementation/lignes.fonctionnaires.et.vacations.csv)
#'
#'## `r chapitre`.3 Contrôles sur les cumuls traitement indiciaire, indemnités et vacations des contractuels

# Vacations et régime indemnitaire

  lignes.contractuels.et.vacations <- Paie[Statut != "TITULAIRE"
                                           & Statut != "STAGIAIRE"
                                           & Grade == "V",
                                             c(étiquette.matricule,
                                               "Nom", 
                                               "Prénom",
                                               étiquette.code,
                                               étiquette.libellé,
                                               étiquette.montant),
                                             with=FALSE]

  matricules.contractuels.et.vacations <- unique(lignes.contractuels.et.vacations[ , .(Matricule, Nom, Prénom)], by=NULL)

  nombre.contractuels.et.vacations     <- nrow(matricules.contractuels.et.vacations)
    
  RI.et.vacations         <- data.frame(NULL)
  traitement.et.vacations <- data.frame(NULL)

 if (nombre.contractuels.et.vacations) 
  {
     RI.et.vacations <- Paie[Type == "I"
                             & Matricule %chin% matricules.contractuels.et.vacations$Matricule,
                               c(étiquette.matricule,
                                 "Statut",
                                 étiquette.code,
                                 "Type",
                                 étiquette.libellé,
                                 étiquette.montant), 
                               with=FALSE]
  
  # Vacations et indiciaire
  
    traitement.et.vacations <- Paie[Type == "T" 
                                    & Matricule %chin% matricules.contractuels.et.vacations$Matricule,
                                      c(étiquette.matricule,
                                        "Statut",
                                        étiquette.code,
                                        "Type",
                                        étiquette.libellé,
                                        étiquette.montant),
                                      with=FALSE]
  }

  nombre.Lignes.paie.contractuels.et.vacations <- nrow(lignes.contractuels.et.vacations)
  nombre.Lignes.paie.RI.et.vacations           <- nrow(RI.et.vacations)
  nombre.Lignes.paie.traitement.et.vacations   <- nrow(traitement.et.vacations)

#'
#'**Contractuels effectuant des vacations (CEV)**
#'


if (exists("nombre.contractuels.et.vacations")) {
  #'  
  #'&nbsp;*Tableau `r incrément()`*   
  #'    
  
  Tableau(c("Nombre de CEV",
            "Nombre de lignes",
            "Nombre de lignes indemnitaires",
            "Nombre de lignes de traitement"),
            nombre.contractuels.et.vacations,
            nombre.Lignes.paie.contractuels.et.vacations,
            nombre.Lignes.paie.RI.et.vacations,
            nombre.Lignes.paie.traitement.et.vacations)
}
#'  
#'[Lien vers le bulletins des CEV](Bases/Réglementation/lignes.contractuels.et.vacations.csv)   
#'[Lien vers la base de données Matricules des CEV](Bases/Réglementation/matricules.contractuels.et.vacations.csv)  
#'[Lien vers la base de données Cumul régime indemnitaire et vacations de CEV](Bases/Réglementation/RI.et.vacations.csv)  
#'[Lien vers la base de données Lignes de traitement indiciaire pour CEV](Bases/Réglementation/traitement.et.vacations.csv)  
#'  
#'
#'
#'## `r chapitre`.4 Contrôle sur les indemnités IAT et IFTS

#IAT et IFTS

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
  
  nombre.mois.cumuls <- nrow(unique(personnels.iat.ifts[ , .(Matricule, Année, Mois)], 
                                    by = NULL))
  
  nombre.agents.cumulant.iat.ifts <- uniqueN(personnels.iat.ifts$Matricule)
  
  personnels.iat.ifts <- personnels.iat.ifts[order(Année, Mois, Matricule)]
}

#'
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'      
if (nombre.agents.cumulant.iat.ifts) {
  Tableau(c("Codes IFTS", "Nombre de personnels percevant IAT et IFTS"),
          sep.milliers = "",
          paste(unlist(codes.ifts), collapse=" "),
          nombre.agents.cumulant.iat.ifts)
} else {
  cat("Tests IAT/IFTS sans résultat positif.")
}

#'   
#'[Codes IFTS retenus](Bases/Réglementation/codes.ifts.csv)   
#'[Lien vers la base de données cumuls iat/ifts](Bases/Réglementation/personnels.iat.ifts.csv)    
#'
#'### Contrôle sur les IFTS pour catégories B et contractuels

#IFTS et IB >= 380 (IM >= 350)
#'  
if (! résultat.ifts.manquant) {
    lignes.ifts.anormales <- na.omit(Paie[as.integer(Indice) < 350   
                                          & ifts.logical == TRUE,
                                            c(clé.fusion,
                                              étiquette.année,
                                              "Mois",
                                              "Statut",
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

# IFTS et non tit

ifts.et.contractuel <- NULL 

if (! résultat.ifts.manquant) {
  
  ifts.et.contractuel <- Paie[ Statut != "TITULAIRE"
                              & Statut != "STAGIAIRE"
                              & ifts.logical == TRUE,
                               c(étiquette.matricule,
                                 étiquette.année,
                                 "Mois",
                                 "Statut",
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
   Tableau(c("Nombre de lignes de paye de contractuels percevant des IFTS", "Nombre de lignes IFTS pour IB < 380"), nombre.lignes.ifts.et.contractuel, nombre.lignes.ifts.anormales)
}

#'
#'[Lien vers la base de données Lignes IFTS pour contractuels](Bases/Réglementation/ifts.et.contractuel.csv)    
#'[Lien vers la base de données Lignes IFTS pour IB < 380](Bases/Réglementation/lignes.ifts.anormales.csv)     
#'
#'**Nota :**
#'IB < 380 : fonctionnaire percevant un indice brut inférieur à 380
#'

#'
#'## `r chapitre`.5 Contrôle de la prime de fonctions et de résultats (PFR) et de la prime de responsabilité (PR)     
#'   
résultat.pfr.manquant <- FALSE
nombre.agents.cumulant.pfr.ifts <- 0

# L'expression régulière capte la PFR et la PR 
# Le cumul de la PR et de l'IFTS est régulier, de même que celui de la PR et de la PFR
# le cumul de la PFR et de l'IFTS est irrrégulier

Paie[ , pfr.logical := grepl(expression.rég.pfr, Paie$Libellé, ignore.case=TRUE, perl=TRUE)]

codes.pfr  <- list( "codes PFR" = unique(Paie[pfr.logical == TRUE][ , Code]))


if (length(codes.pfr) == 0) {
  cat("Il n'a pas été possible d'identifier la PFR par expression régulière.")
  résultat.pfr.manquant <- TRUE
}


if (! résultat.ifts.manquant && ! résultat.pfr.manquant) {
  
  Paie[ , cumul.pfr.ifts := (any(pfr.logical[Type == "I"]) 
                               & any(ifts.logical[Type == "I"])), 
         by="Matricule,Année,Mois"]

  # on exclut les rappels !
  
  personnels.pfr.ifts <- Paie[cumul.pfr.ifts == TRUE 
                              & Type == "I"
                              & (pfr.logical == TRUE | ifts.logical == TRUE),
                              .(Matricule, Année, Mois, Code, Libellé, Montant, Type, Emploi, Grade, Service)]
  
  nombre.mois.cumuls <- nrow(unique(personnels.pfr.ifts[ , .(Matricule, Année, Mois)], by = NULL))
  
  nombre.agents.cumulant.pfr.ifts <- uniqueN(personnels.pfr.ifts$Matricule)
  
  personnels.pfr.ifts <- personnels.pfr.ifts[order(Année, Mois, Matricule)]
}

#'
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'      
if (nombre.agents.cumulant.pfr.ifts) {
  Tableau(c("Codes IFTS", "Nombre de personnels percevant PFR/PR et IFTS"),
          sep.milliers = "",
          paste(unlist(codes.pfr), collapse = " "),
          nombre.agents.cumulant.pfr.ifts)
} else {
  cat("Tests PFR/IFTS sans résultat positif.")
}

#'   
#'[Codes PFR retenus](Bases/Réglementation/codes.pfr.csv)   
#'[Lien vers la base de données cumuls pfr/ifts](Bases/Réglementation/personnels.pfr.ifts.csv)    
#'
#'
#'## `r chapitre`.6 Contrôle sur les heures supplémentaires

# Sont repérées comme heures supplémentaires ou complémentaires les heures dont le libellé obéissent à
# l'expression régulière expression.rég.heures.sup donnée par le fichier prologue.R

# Vérification des seuils annuels :

Dépassement.seuil.180h <- unique(Bulletins.paie[cumHSup > 180, 
                                                  .(Matricule, Année, "Cumul heures sup" = cumHSup, Emploi, Grade, Service)])
nb.agents.dépassement  <- uniqueN(Dépassement.seuil.180h$Matricule)

if  (nb.agents.dépassement)  {
  cat("Le seuil de 180 heures supplémentaires maximum est dépassé par ", nb.agents.dépassement, " agents.\n")
  Dépassement.seuil.220h <- Dépassement.seuil.180h["Cumul heures sup" > 220]
  nb.agents.dépassement.220h <- uniqueN(Dépassement.seuil.220h$Matricule) 
  
  if  (nb.agents.dépassement.220h) cat(" Le seuil de 220 heures supplémentaires maximum est dépassé par ", nb.agents.dépassement.220h, " agents.\n") 
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
  
HS.sup.25 <- merge(HS.sup.25, traitement.indiciaire.mensuel)

HS.sup.25 <- merge(HS.sup.25, Analyse.rémunérations[ , .(Matricule, Année, traitement.indiciaire)], 
                                                       by=c("Matricule", "Année"))

HS.sup.25 <- unique(HS.sup.25, by=NULL)

names(HS.sup.25) <- sub("traitement.indiciaire", "Traitement indiciaire annuel", names(HS.sup.25))

nombre.Lignes.paie.HS.sup.25 <- nrow(HS.sup.25)

ihts.anormales <- data.frame(NULL)

if (fichier.personnels.existe)
  nombre.ihts.anormales <- nrow(ihts.anormales) else nombre.ihts.anormales <- NA

if (! is.null(HS.sup.25)) message("Heures sup controlées")
#'
#'  
#'&nbsp;*Tableau `r incrément()`*   
#'    

Tableau(c("Nombre de lignes HS en excès", "Nombre de lignes IHTS anormales"), nombre.Lignes.paie.HS.sup.25, nombre.ihts.anormales)

#'
#'[Lien vers la base de données Heures supplémentaires en excès du seuil de 25h/mois: matricules](Bases/Réglementation/HS.sup.25.csv)     
#'[Lien vers la base de données cumuls en excès des seuils annuels](Bases/Réglementation/Dépassement.seuil.180h.csv)    
#'[Lien vers la base de données IHTS anormales](Bases/Réglementation/ihts.anormales.csv)      
#'
#'**Nota :**
#'HS en excès : au-delà de 25 heures par mois
#'IHTS anormales : non attribuées à des fonctionnaires de catégorie B ou C.


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
if (générer.table.élus)
{
    if (nrow(rémunérations.élu) > 0)
      kable(rémunérations.élu, row.names = FALSE)
} else {
  
   cat("Tableau des indemnités d'élu : non générée.")
}

#'   
if (sauvegarder.bases.analyse)
  Sauv.base(file.path(chemin.dossier.bases, "Effectifs"),
            "matricules",
            fichier.personnels)

#'[Lien vers la base de données Rémunérations des élus](Bases/Réglementation/rémunérations.élu.csv)
#'
#'# Annexe
#'## Liens complémentaires
#'
#'[Lien vers la base de données des bulletins et lignes de paie](Bases/Paiements/Bulletins.paie.csv)
#'[Lien vers la base de données fusionnées des bulletins et lignes de paie](Bases/Paiements/Paie.csv)
#'
#'
#'[Lien vers le fichier des personnels](Bases/Effectifs/Catégories des personnels.csv)
#'  
#'## Fiabilité du traitement statistique  
#'### Eliminations des doublons  
#'  
if (après.redressement != avant.redressement) {
      
  cat("Retraitement de la base : ")

} else {
  cat("Aucune duplication de ligne détectée. ")
}

#'  
if (après.redressement != avant.redressement)
  cat("Elimination de ", avant.redressement - après.redressement, " lignes dupliquées")

#'   
base.heures.nulles.salaire.nonnull     <- Bulletins.paie[Heures == 0  & (Net.à.Payer != 0 | Brut != 0)]
base.quotité.indéfinie.salaire.nonnull <- Bulletins.paie[MHeures == 0 & (Net.à.Payer != 0 | Brut != 0)]

nligne.base.heures.nulles.salaire.nonnull     <- nrow(base.heures.nulles.salaire.nonnull)
nligne.base.quotité.indéfinie.salaire.nonnull <- nrow(base.quotité.indéfinie.salaire.nonnull)
#'  
if (nligne.base.heures.nulles.salaire.nonnull)
   cat("Nombre de bulletins de paye de salaires (net ou brut) versés pour un champ Heures = 0 : ", nligne.base.heures.nulles.salaire.nonnull)
#'   
if (nligne.base.quotité.indéfinie.salaire.nonnull)
   cat("\nNombre de bulletins de paye de salaires (net ou brut) versés pour une quotité de travail indéfinie : ", nligne.base.heures.nulles.salaire.nonnull)
#'   
#'[Lien vers la base de données des salaires versés pour Heures=0](Bases/Fiabilité/base.heures.nulles.salaire.nonnull.csv)   
#'[Lien vers la base de données des salaires versés à quotité indéfinie](Bases/Fiabilité/base.quotité.indéfinie.salaire.nonnull.csv)   
#'
#'# Tableau des personnels : renseigner la catégorie
#'
#'Utiliser les codes : A, B, C, ELU, AUTRES
#'
#'En cas de changement de catégorie en cours de période, utiliser la catégorie AUTRES
#'Cela peut conduire à modifier manuellement le fichier Catégories des personnels.csv
#'
if (générer.table.effectifs) {
  kable(matricules, row.names = FALSE) 
} else  {
  cat("Non généré  [anonymisation]")
}

# ------------------------------------------------------------------------------------------------------------------
#  Sauvegardes : enlever les commentaires en mode opérationnel
##

if (sauvegarder.bases.analyse) {

  sauv.bases(file.path(chemin.dossier.bases, "Rémunérations"),
             "Analyse.rémunérations",
             "Analyse.variations.synthèse",
             "Analyse.variations.par.exercice",
             "masses.premier.personnels",
             "masses.premier.élus",
             "masses.dernier.personnels",
             "masses.dernier.élus")

  sauv.bases(file.path(chemin.dossier.bases, "Effectifs"),
             "Bulletins.paie.nir.total.hors.élus",
             "Bulletins.paie.nir.fonctionnaires",
             "Bulletins.paie.nir.nontit",
             "Bulletins.paie.nir.permanents",
             "tableau.effectifs")

  sauv.bases(file.path(chemin.dossier.bases, "Réglementation"),
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
             "lignes.ifts.anormales",
             "matricules.contractuels.et.vacations",
             "matricules.fonctionnaires.et.vacations",
             "NBI.aux.non.titulaires",
             "personnels.prime.informatique",
             "personnels.iat.ifts",
             "rémunérations.élu",
             "RI.et.vacations",
             "traitement.et.vacations")
  
  sauv.bases(file.path(chemin.dossier.bases, "Fiabilité"),
              "base.heures.nulles.salaire.nonnull",
              "base.quotité.indéfinie.salaire.nonnull")
  
}

if (sauvegarder.bases.origine)
  sauv.bases(file.path(chemin.dossier.bases, "Paiements"),
             "Paie",
             "Bulletins.paie")



