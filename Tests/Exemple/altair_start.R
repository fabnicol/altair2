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
#'![Image_Altair](Altair.png)
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
  
# Utiliser la compilation JIT

library(compiler, warn.conflicts = FALSE)

invisible(setCompilerOptions(suppressAll = TRUE, optimize = 3))
invisible(enableJIT(3))

# Options générales

options(warn = -1, verbose = FALSE, OutDec = ",", datatable.verbose = FALSE, datatable.integer64 = "numeric")

# Sourcer la biblio de fonctions auxiliaires

source("bibliotheque.fonctions.paie.R", encoding = "UTF-8")

# Importer les données --> bases Paie et Bulletins.paie

source("import.R", encoding = encodage.code.source)

# En-tête du rapport
# Les caractéristiques du contrôle sont contenues dans controle[1], controle[2], controle[3], controle[4]

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

# Cette fonction permet de sauter une page dans le PDF ou dans le html (pas dans le docx)

newpage()

# Analyser les rémunérations à partir des données importées --> bases Analyse.XXX

source("analyse.rémunérations.R", encoding = encodage.code.source)

########### 1.1 Effectifs ########################

# Cette fonction incrémente le chapitre du rapport


#'# 1. Statistiques de population
#'

#+ pyramides-des-âges


# Appel de la biblio altair, où sont regroupées des fonctions d'analyse des rémunérations et les pyramides

library("altair", lib.loc=c("/usr/lib64/R/library", "/usr/local/lib64/R/library"))

e <- new.env()

fichiers.pyr <- list.files(path= file.path(currentDir, "data"), pattern = "*.csv", full.names = TRUE)

# Lecture des fichiers de référence des pyramides (fichiers listés dans fichiers.pyr), comportant les statistiques INSEE

for (f in fichiers.pyr) {
  base <- basename(f)
  assign(substr(base, 1, attr(regexec("(.*)\\.csv", base)[[1]], "match.length")[2]),
         fread(f, sep = ";", header = TRUE, encoding = "Latin-1", dec = ",", colClasses = c("integer", "numeric", "numeric", "integer", "character")),
         envir = .GlobalEnv)
}

# local = TRUE permet de conserver l'environnement e en sourçant

source("analyse.bulletins.R", local = TRUE, encoding = encodage.code.source)

########### 1.1 Pyramides ########################

#'## 1.1 Pyramide des âges, ensemble des personnels

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
   
########### 1.2 Pyramides fonctionnaires ########################

#'
#'## 1.2 Pyramide des âges des fonctionnaires  
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

########### 1.3 Pyramides non Tit ########################

#'## 1.3 Pyramide des âges, personnels non titulaires   

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

########### 1.4 Pyramides Autres statut ########################

#'## 1.4 Pyramide des âges, autres statuts


#' 
#+fig.height=8, fig.width=7
Filtre_bulletins <<- setdiff(unique(Bulletins.paie$Statut), c("TITULAIRE", "NON_TITULAIRE", "STAGIAIRE")) 

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

########### 1.5 Effectifs par durée ########################

#'## 1.5 Effectifs des personnels par durée de service
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




newpage()

#'
########### 2. TESTS STATUTAIRES ########################
#'
#'   
#'**Dans cette partie, l'ensemble de la base de paie est étudié.**  
#'Les agents non actifs ou dont le poste est annexe sont réintroduits dans le périmètre.   

#### 2.1 NBI ####

#'# 2. Tests réglementaires   
#'## 2.1 Contrôle des nouvelles bonifications indiciaires (NBI) 

#+ tests-statutaires-nbi
#'   
#'Il est conseillé, pour ce test, de saisir les codes de NBI dans l'onglet Codes de l'interface graphique  &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_onglet_codes.odt)     
#'A défaut, des lignes de paye de rappels de cotisations sur NBI peuvent être agrégés, dans certains cas, aux rappels de rémunération brute.   
#'       

# --- Test NBI accordée aux non titulaires
#     Filtre    : Statut != "TITULAIRE" & Statut != "STAGIAIRE" & NBI != 0  grepl(expression.rég.nbi, Libellé)

colonnes <-  c("Matricule",
               "Nom",
               "Prénom",
               "Grade",
               "Statut",
               "Code",
               "Libellé",
               "Année",
               "Mois",
               "Montant")

Paie_NBI   <- filtrer_Paie("NBI", 
                           portée = "Mois", 
                           indic = TRUE)[Type %chin% c("T", "I", "R")]

lignes_NBI <- Paie_NBI[indic == TRUE][ , indic :=  NULL]

NBI.aux.non.titulaires <- lignes_NBI[Statut != "TITULAIRE" 
                                     & Statut != "STAGIAIRE" 
                                     & NBI != 0,
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
#'[Lien vers la base de données NBI aux non titulaires](Bases/Reglementation/NBI.aux.non.titulaires.csv) &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_NBI_nt.odt)       
#'     

# On calcule tout d'abord la somme de points de NBI par matricule et par année
# On calcule ensuite, sur les traitements et éventuellement (par abus ou erreur de codage) les indemnités, la somme des paiements au titre de la NBI, par matricule et par année
# Attention ne pas prendre en compte les déductions, retenues et cotisations. On compare en effet les payements bruts de base à la somme des points x valeur du point

# La quotité ici considérée est non pas la quotité statistique mais la quotité administrative plus favorable aux temps partiels.


T1 <- lignes_NBI[! is.na(NBI)
                & NBI != 0,  c("Matricule", 
                               "Nom",
                               "Prénom",
                               "Grade",
                               "Statut",
                               "Année",
                               "Mois",
                               "Montant",
                               "Début",
                               "quotité",
                               "NBI",
                               "Type"),
                                with = FALSE
                ][ , `:=` (Année.rappel = as.numeric(substr(Début, 0, 4)),
                           Mois.rappel  = as.numeric(substr(Début, 6, 7))) 
                ][ , Début := NULL]
               
if (nrow(T1) == 0) cat("Aucune NBI n'a été attribuée ou les points de NBI n'ont pas été rencensés en base de paye. ")

#nbi.cum.hors.rappels = sum(Montant[Type != "R" | (Année.rappel == Année & Mois.rappel == Mois)],
#                                                          na.rm = TRUE),

T2a <- T1[! is.na(quotité)
          & quotité > 0
          & Type == "R",
                .(nbi.cum.rappels = sum(Montant, na.rm = TRUE)), 
                 by= .(Matricule, Année.rappel, Mois.rappel, Année, Mois)
         ][Année.rappel >= début.période.sous.revue 
                 & Mois.rappel >=1 
                 & Mois.rappel <= 12]

setnames(T2a, "Année", "Année.R")
setnames(T2a, "Mois", "Mois.R")
setnames(T2a, "Année.rappel", "Année")
setnames(T2a, "Mois.rappel", "Mois")

# Il faut éviter la réduplication de rappels sur le mêm mois qui après jointure causera une réduplication fautive des montants

T2b <- T1[! is.na(quotité)
           & quotité > 0
           & Type != "R", 
               .(nbi.cum.hors.rappels = sum(Montant, na.rm = TRUE),
                 quotité = quotité[1],
                 nbi.cum.indiciaire = NBI[1]), 
                 by= .(Matricule, Année, Mois)]

T2 <- merge(T2a, T2b, 
               all = TRUE,
               by = c("Matricule", "Année", "Mois"))[ , adm.quotité := adm(quotité)
                                                   ][is.na(nbi.cum.rappels), nbi.cum.rappels := 0
                                                   ][is.na(nbi.cum.hors.rappels), nbi.cum.hors.rappels := 0]

T2[adm.quotité > 0,  nbi.eqtp.tot := (nbi.cum.rappels + nbi.cum.hors.rappels) / adm.quotité]

cumuls.nbi <- T2[ , .(cumul.annuel.indiciaire = sum(nbi.cum.indiciaire, na.rm = TRUE),
                      cumul.annuel.montants   = sum(nbi.eqtp.tot, na.rm = TRUE)),
                    keyby="Année"]

if (nrow(cumuls.nbi) == 0) cat("Cumuls de NBI nuls. ")

# On somme ensuite par année sur tous les matricules
# Les cumuls annuels rapportés aux cumuls indiciaires pour l'année ne doivent pas trop s'écarter de la valeur annuelle moyenne du point d'indice

# Techniquement, rajouter un by = .(Année, Mois) accélère la computation

lignes.nbi.anormales <- T2[nbi.cum.indiciaire > 0 
                            & nbi.eqtp.tot > 0,
                              test := nbi.eqtp.tot/nbi.cum.indiciaire - PointMensuelIM[Année - 2007, Mois], by= .(Année, Mois)
                          ][! is.na(test) & abs(test) > 1
                          ][ , cout.nbi.anormale := (nbi.eqtp.tot - nbi.cum.indiciaire * PointMensuelIM[Année - 2007, Mois]) * adm.quotité]

couts.nbi.anormales <- lignes.nbi.anormales[ , sum(cout.nbi.anormale, na.rm = TRUE)]

lignes.nbi.anormales.hors.rappels <- T2[nbi.cum.indiciaire > 0 
                            & nbi.cum.hors.rappels > 0,
                              test := nbi.cum.hors.rappels/(adm.quotité * nbi.cum.indiciaire) - PointMensuelIM[Année - 2007, Mois], by= .(Année, Mois)
                          ][! is.na(test) & abs(test) > 1
                          ][ , cout.nbi.anormale := nbi.cum.hors.rappels - nbi.cum.indiciaire * PointMensuelIM[Année - 2007, Mois] * adm.quotité]

couts.nbi.anormales.hors.rappels <- lignes.nbi.anormales.hors.rappels[ , sum(cout.nbi.anormale, na.rm = TRUE)]

rappels.nbi <- T2a[ , sum(nbi.cum.rappels, na.rm = TRUE)]

#'  
#'&nbsp;*Tableau `r incrément()` : Contrôle de liquidation de la NBI*     &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_NBI_liq.odt)       
#'    

Tableau(
  c("Lignes de NBI concernées",
    "Coûts correspondants",
    "Rappels (montants bruts)"),
  nrow(lignes.nbi.anormales),
  round(couts.nbi.anormales, 1),
  round(rappels.nbi, 1))

#'  
#'&nbsp;*Tableau `r incrément()` : Contrôle de liquidation de la NBI, hors rappels*     &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_NBI_liq.odt)       
#'    

Tableau(
  c("Lignes de NBI concernées (hors rappels)",
    "Coûts correspondants"),
  nrow(lignes.nbi.anormales.hors.rappels),
  round(couts.nbi.anormales.hors.rappels, 1))

#'       
#'[Lien vers la base de données des anomalies de NBI](Bases/Fiabilite/lignes.nbi.anormales.csv)     
#'[Lien vers la base de données des anomalies de NBI hors rappels](Bases/Fiabilite/lignes.nbi.anormales.hors.rappels.csv)          
#'   
#'**Nota :**   
#'*Est considéré comme anomalie manifeste un total annuel de rémunérations NBI correspondant à un point d'indice net mensuel inférieur à la moyenne de l'année moins 1 euro ou supérieur à cette moyenne plus 1 euro.*    
#'*Les rappels ne sont pas pris en compte dans les montants versés. Certains écarts peuvent être régularisés en les prenant en compte*     
#'  
#'    
#'&nbsp;*Tableau `r incrément()` : Contrôle global de la liquidation des NBI*     &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_NBI_glob.odt)       
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
#'[Lien vers la base de données des cumuls annuels de NBI](Bases/Fiabilite/cumuls.nbi.csv)   
#'   

# --- Test Proratisation NBI

#'  
#'&nbsp;*Tableau `r incrément()` : Contrôle de proratisation/liquidation de la NBI*     &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_NBI_prorat.odt)           
#'  

# Calcul plus exact de liquidation, attention à exclure les rappels

montants.nbi.anormales.mensuel <- 0
lignes.nbi.anormales.mensuel <- data.table()

essayer(
{  
  lignes.nbi.anormales.mensuel <- lignes_NBI[Type != "R", .(Montant.NBI.calculé = NBI[1] * adm(quotité[1]) * PointMensuelIM[Année - 2007, Mois],
                                                            Montant.NBI.payé = sum(Montant, na.rm = TRUE)), 
                                                            keyby = "Matricule,Année,Mois"
                                       ][ , Différence.payé.calculé := Montant.NBI.payé - Montant.NBI.calculé
                                       ][abs(Différence.payé.calculé) > tolérance.nbi]
  
  lignes.paie.nbi.anormales.mensuel <- merge(Paie_NBI[, .(Matricule, Nom, Prénom, Grade, Statut, 
                                                            Année, Mois, Echelon, Catégorie, 
                                                            Emploi, Service, Temps.de.travail,
                                                            NBI, Code, Libellé,
                                                            Base, Taux,Type, Montant)],
                                                  lignes.nbi.anormales.mensuel,
                                                  by = c("Matricule", "Année", "Mois"))
  
  nb.lignes.anormales.mensuel    <- nrow(lignes.nbi.anormales.mensuel)
  montants.nbi.anormales.mensuel <- lignes.nbi.anormales.mensuel[, sum(Différence.payé.calculé, na.rm = TRUE)]
},
"La vérification de la proratisation de la NBI n'a pas pu être réalisée. ")

Tableau(
  c("Différences > " %+% tolérance.nbi %+% " euro : nombre de lignes",
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

NBI.cat <- Bulletins.paie[! is.na(NBI) & NBI > 0, 
                            .(Matricule,
                              Nom,
                              Prénom,
                              Grade,
                              Statut,
                              Emploi,
                              Catégorie,
                              Année,
                              Mois,
                              NBI,
                              quotité)]

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
                        ][! is.na(Coût)
                        ][ , Contrôle := NULL] 
  
nombre.mat.NBI.irrég <- NBI.cat.irreg[ , uniqueN(Matricule)]
coût.total <- NBI.cat.irreg[ , sum(Coût, na.rm = TRUE)]

#'  
#'&nbsp;*Tableau `r incrément()` : Contrôle d'attribution de NBI par catégorie statutaire*  &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_plafonds_NBI.odt)           
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



#### 2.2 PFI ####

# --- Test Prime de fonctions informatiques
#     Filtre    : filtre expression rationnelle expression.rég.pfi dans Libellé.   

#'  
#'## 2.2 Contrôle de la prime de fonctions informatiques (PFI)   

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



#### 2.3 VACATIONS ####
#'  
#'## 2.3 Contrôle des vacations horaires pour les fonctionnaires      

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

####  2.4 CEV ####  
  
#'
#'## 2.4 Contrôles sur les contractuels effectuant des vacations horaires    


#+ tests-statutaires-vacations-ri
#'     
#'**Attention**    
#'Les contrôles réalisés sur les payes des vacataires nécessitent, le plus souvent, la saisie des codes de paye relatifs aux vacations dans l'onglet Codes de l'interface graphique, en raison du fréquent mauvais renseignement 
#'de ces codes en base de paye.  &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_onglet_codes.odt)        
#'  

  Paie_vac_contr <- Paie_vac[Statut %chin% c("NON_TITULAIRE",  "AUTRE_STATUT"), 
                                 .(Nom, Prénom, Matricule, Service, Statut, Catégorie, Grade, Echelon, Libellé, Type,
                                   Heures, Heures.Sup., Nb.Enfants, Code, Base, Taux, Nb.Unité,  Montant)]
                                           
  matricules.contractuels.et.vacations <- unique(Paie_vac_contr[ , .(Matricule, Nom, Statut)], by=NULL)

  nombre.contractuels.et.vacations     <- nrow(matricules.contractuels.et.vacations)
    
  RI.et.vacations  <- data.frame(NULL)

  if (nombre.contractuels.et.vacations) 
  {
     RI.et.vacations <- Paie_vac_contr[Type == "I"]
  }

  nombre.Lignes.paie.RI.et.vacations <- nrow(RI.et.vacations)

#'  
#'&nbsp;*Tableau `r incrément()` : Contractuels effectuant des vacations horaires (CEV)*  &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_CEV_horaires.odt)   
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

  
  essayer({ 
    
  Paie_vac_sft_ir <- filtrer_Paie("IR_S", 
                                  portée = "Mois", 
                                  Var = "Type", 
                                  Base = Paie_vac)[! Statut %chin% c("TITULAIRE", "STAGIAIRE"), 
                               .(Nom, Prénom, Matricule, Service, Statut, Catégorie, Grade, Echelon, Libellé, Type,
                                 Heures, Heures.Sup., Nb.Enfants, Code, Base, Taux, Nb.Unité,  Montant)]  

  SFT_IR.et.vacations <- Paie_vac_sft_ir[Type %chin% c("IR", "S")]
  
  matricules.SFT_IR.et.vacations <- unique(SFT_IR.et.vacations[ , .(Matricule, Nom, Statut)], by=NULL)
  
  nombre.SFT_IR.et.vacations     <- nrow(matricules.SFT_IR.et.vacations)
  }, "Les tests sur les CEV et les versements de SFT ou d'IR n'ont pas été réalisés. ")
  

#'  
#'&nbsp;*Tableau `r incrément()` : CEV percevant le supplément familial de traitement ou l'indemnité de résidence*     &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_CEV_SFT.odt)   
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
  
##### Prologue indemnitaire #####  

# Description des données indemnitaires
  
  Paie_I <- Paie[Type == "I" | Type == "A" | Type == "R", 
                 .(Nom, 
                   Prénom,
                   Matricule, 
                   Année, 
                   Mois, 
                   Début,
                   Fin,
                   Code,
                   Libellé,
                   Montant,
                   Type,
                   Emploi,
                   Grade, 
                   Temps.de.travail,
                   Indice,
                   Statut,
                   Catégorie)]
  
  prime_IAT <- list(nom = "IAT",                     # Nom en majuscules
                    catégorie = c("B", "C"),         # restreint aux catégories B et C
                    restreint_fonctionnaire = TRUE,  # fonctionnaires
                    dossier = "Reglementation")      # dossier de bases
  
  prime_IFTS <- list(nom = "IFTS",                   # Nom en majuscules
                     catégorie = c("A", "B"),        # restreint aux catégories A et B
                     restreint_fonctionnaire = TRUE, # fonctionnaires
                     dossier = "Reglementation",     # dossier de bases  
                     NAS = "non",                    # logement par NAS
                     indice  = c("+", 350, "B"))     # supérieur à INM 350 pour catégorie B.
  
  prime_PFR <- list(nom = "PFR",                     # Nom en majuscules
                    catégorie = "A",                 # restreint aux catégories A
                    restreint_fonctionnaire = TRUE,  # fonctionnaires
                    dossier = "Reglementation",      # dossier de bases
                    expr.rég = "")  
  
  prime_PSR <- list(nom = "PSR",                     # Nom en majuscules
                    catégorie = c("A","B"),          # restreint aux catégories A et B
                    restreint_fonctionnaire = TRUE,  # fonctionnaires
                    dossier = "Reglementation",      # dossier de bases
                    expr.rég = ".*(?:ing|tech|d.*g.*s.*t|dessin|biol|phar).*")  # Contrainte sur le grade (expression régulière)
  
  prime_IPF <- list(nom = "IPF",                     # Nom en majuscules
                    catégorie = "A",                 # restreint aux catégories A et B
                    restreint_fonctionnaire = TRUE,  # fonctionnaires
                    dossier = "Reglementation",      # dossier de bases
                    expr.rég = ".*(?:ing.*chef).*")  # Contrainte sur le grade (expression régulière)
  
  prime_IFSE <- list(nom = "IFSE",                   # Nom en majuscules
                     restreint_fonctionnaire = TRUE, # fonctionnaires
                     catégorie = c("A", "B", "C"),   # toutes les catégories
                     dossier = "Reglementation")     # dossier de bases
  
  prime_ISS <- list(nom = "ISS",                     # Nom en majuscules
                    catégorie = c("A", "B"),         # Techniciens A, B
                    restreint_fonctionnaire = TRUE,  # fonctionnaires
                    dossier = "Reglementation")      # dossier de bases
  
  prime_IEMP <- list(nom = "IEMP",                   # Nom en majuscules
                     restreint_fonctionnaire = TRUE, # fonctionnaires
                     catégorie = c("A", "B", "C"),   # toutes les catégories
                     dossier = "Reglementation")     # dossier de bases
  
  prime_PFI <- list(nom = "PFI",                      # Nom en majuscules
                    restreint_fonctionnaire = TRUE,   # fonctionnaires
                    catégorie = c("A", "B", "C"),     # toutes les catégories
                    dossier = "Reglementation")       # dossier de bases
  
  
#### 2.5 IAT/IFTS ####  
  
#'
#'## 2.5 Contrôle sur les indemnités IAT et IFTS      

#+ IAT-et-IFTS


résultat_IAT_IFTS <- test_prime(prime_IAT,
                                prime_B = prime_IFTS,
                                Paie_I,
                                verbeux = afficher.table.effectifs)

Paie_IAT    <- résultat_IAT_IFTS$Paie
Paie_IFTS   <- résultat_IAT_IFTS$Paie_B
Lignes_IAT  <- résultat_IAT_IFTS$Lignes
Lignes_IFTS <- résultat_IAT_IFTS$Lignes_B

#'   
#'   
#'### Contrôle sur les IAT pour catégories B C et non-titulaires      
#'   
#'   
#'[Lien vers la base de données IAT aux non-titulaires](Bases/Reglementation/IAT.non.tit.csv)    
#'[Lien vers la base de données IAT non cat B-C](Bases/Reglementation/IAT.non.catBC.csv)   
#'     
#'     
#'### Contrôle sur les IFTS pour catégories B et non-titulaires      

#IFTS et IB >= 380 (IM >= 350)
#'  
#'   
#'[Lien vers la base de données IFTS à des catég. B INM inférieur à 350](Bases/Reglementation/IFTS.indice.anormal.csv)         
#'[Lien vers la base de données IFTS aux non-titulaires](Bases/Reglementation/IFTS.non.tit.csv)   
#'[Lien vers la base de données IFTS non cat A-B](Bases/Reglementation/IFTS.non.catAB.csv)    
#' 
#'  
#'&nbsp;*Tableau `r incrément()` : Cumul IAT/IFTS*   
#'      

tableau_cumuls(résultat_IAT_IFTS)

#'[Lien vers la base de données cumuls IAT/IFTS](Bases/Reglementation/personnels.iat.ifts.csv)          

#'     
#'### Contrôle sur le cumul logement par NAS et IFTS
#'      

#'  
#'&nbsp;*Tableau `r incrément()` : Cumul logement par NAS/IFTS*   
#'      

tableau_NAS(résultat_IAT_IFTS)

#'[Lien vers la base de données cumuls NAS/IFTS](Bases/Reglementation/cumul.IFTS.NAS.csv)          

rm(résultat_IAT_IFTS)  

#### 2.6 PFR ####

#'
#'## 2.6 Contrôle de la prime de fonctions et de résultats (PFR)   
#'   

#+ pfr

#'    
#'&nbsp;*Tableau `r incrément()` : Cumul PFR/IFTS*   
#'      

résultat_PFR  <- test_prime(prime_PFR, prime_IFTS, Paie_I, Paie_IFTS, Lignes_IFTS, afficher.table.effectifs)

Paie_PFR <- résultat_PFR$Paie
Lignes_PFR <- résultat_PFR$Lignes

#'  
#'&nbsp;*Tableau `r incrément()` : Cumuls PFR/IFTS*   
#'      

tableau_cumuls(résultat_PFR)

#'      
#'[Lien vers la base de données cumuls pfr/ifts](Bases/Reglementation/personnels.pfr.ifts.csv)    
#'[Lien vers la base de données PFR non cat.A](Bases/Reglementation/PFR.non.catA.csv)      
#'[Lien vers la base de données PFR non tit](Bases/Reglementation/PFR.non.tit.csv)       
#'   

résultat_PFR   <- test_prime(prime_PFR, prime_ISS, Paie_I, verbeux = afficher.table.effectifs)

Paie_ISS   <- résultat_PFR$Paie
Lignes_ISS <- résultat_PFR$Lignes

#'    
#'&nbsp;*Tableau `r incrément()` : Cumul PFR/ISS*   
#'      

tableau_cumuls(résultat_PFR)

#'      
#'      
#'[Lien vers la base de données cumuls pfr/iss](Bases/Reglementation/personnels.pfr.iss.csv)    
#'   

résultat_PFR   <- test_prime(prime_PFR, prime_IEMP, Paie_I, verbeux = afficher.table.effectifs)

Paie_IEMP   <- résultat_PFR$Paie
Lignes_IEMP <- résultat_PFR$Lignes

#'    
#'&nbsp;*Tableau `r incrément()` : Cumul PFR/IEMP*   
#'      

tableau_cumuls(résultat_PFR)

#'      
#'      
#'[Lien vers la base de données cumuls pfr/iemp](Bases/Reglementation/personnels.pfr.iemp.csv)    
#'   


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
  
  cumuls.PFR <- résultat_PFR$Lignes[, .(PFR_annuel = sum(Montant, na.rm = TRUE),
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
    
    cat("\nLes plafonds annuels de la PFR ne sont pas dépassés.\n")
  }

#'  
#'&nbsp;*Tableau `r incrément()` : Valeurs de l'agrégat annuel (PFR ou IFTS) pour les bénéficiaires de la PFR*        
#'          

  agrégat_annuel(résultat_PFR, afficher.table.effectifs)  
  
#'   
#'[Lien vers la base de données agrégat PFR-IFTS](Bases/Remunerations/beneficiaires.PFR.IFTS.csv)    
#'    
  
#'  
#'&nbsp;*Tableau `r incrément()` : Variations de l'agrégat mensuel moyen (PFR ou IFTS) pour les bénéficiaires de la PFR*   
#'          
  
  évolution_agrégat(résultat_PFR, afficher.table.effectifs)

#'   
#'[Lien vers la base de données variations agrégat PFR-IFTS](Bases/Remunerations/beneficiaires.PFR.IFTS.Variation.csv)    
#'   

rm(résultat_PFR)  


#### 2.7 PSR ####

#'
#'## 2.7 Contrôle de la prime de service et de rendement (PSR)   
#'   

# décret n°2009-1558 du 15 décembre 2009    
  
#+ psr


  
# -ingénieurs des ponts, des eaux et des forêts relevant du ministère chargé du développement durable ;
# -ingénieurs des travaux publics de l'Etat ;
# -techniciens supérieurs du développement durable ;
# -conducteur des travaux publics de l'Etat ;
# -experts techniques des services techniques ;
# -dessinateurs de l'équipement ;
# -inspecteurs du permis de conduire et de la sécurité routière ;
# -chargés de recherche relevant du ministère chargé du développement durable ;
# -directeurs de recherche relevant du ministère chargé du développement durable.
  
résultat_PSR   <- test_prime(prime_PSR, prime_IFTS, Paie_I, Paie_IFTS, Lignes_IFTS, afficher.table.effectifs)

Lignes_PSR <- résultat_PSR$Lignes
Paie_PSR <- résultat_PSR$Paie


#'    
#'&nbsp;*Tableau `r incrément()` : Cumul PSR/IFTS*   
#'      

tableau_cumuls(résultat_PSR)

#'      
#'[Lien vers la base de données cumuls psr/ifts](Bases/Reglementation/personnels.psr.ifts.csv)       
#'[Lien vers la base de données PSR grade non conforme](Bases/Reglementation/PSR.non.catAB.csv)      
#'[Lien vers la base de données PSR non tit](Bases/Reglementation/PSR.non.tit.csv)       
#'   

#'  
#'&nbsp;*Tableau `r incrément()` : Valeurs de l'agrégat annuel (PSR ou IFTS) pour les bénéficiaires de la PSR*        
#'          

agrégat_annuel(résultat_PSR, afficher.table.effectifs)

#'   
#'[Lien vers la base de données agrégat PSR-IFTS](Bases/Remunerations/beneficiaires.PSR.IFTS.csv)    
#'    

#'  
#'&nbsp;*Tableau `r incrément()` : Variations de l'agrégat mensuel moyen (PSR ou IFTS) pour les bénéficiaires de la PSR*   
#'          

évolution_agrégat(résultat_PSR, afficher.table.effectifs)

#'   
#'[Lien vers la base de données variations agrégat PSR-IFTS](Bases/Remunerations/beneficiaires.PSR.IFTS.Variation.csv)    
#'   

résultat_PSR   <- test_prime(prime_PSR, prime_IAT, Paie_I, Paie_IAT, Lignes_IAT, afficher.table.effectifs)

#'   
#'    
#'&nbsp;*Tableau `r incrément()` : Cumul PSR/IAT*   
#'      

tableau_cumuls(résultat_PSR)

#'      
#'[Lien vers la base de données cumuls psr/iat](Bases/Reglementation/personnels.psr.iat.csv)       
#'   


#'  
#'&nbsp;*Tableau `r incrément()` : Valeurs de l'agrégat annuel (PSR ou IAT) pour les bénéficiaires de la PSR*        
#'          

agrégat_annuel(résultat_PSR, afficher.table.effectifs)

#'   
#'[Lien vers la base de données agrégat PSR-IAT](Bases/Remunerations/beneficiaires.PSR.IAT.csv)    
#'    

#'  
#'&nbsp;*Tableau `r incrément()` : Variations de l'agrégat mensuel moyen (PSR ou IAT) pour les bénéficiaires de la PSR*   
#'          

évolution_agrégat(résultat_PSR, afficher.table.effectifs)

#'   
#'[Lien vers la base de données variations agrégat PSR-IAT](Bases/Remunerations/beneficiaires.PSR.IAT.Variation.csv)    
#'   

rm(résultat_PSR)
   

#### 2.8 IPF ####

#'
#'## 2.8 Contrôle de l'indemnité de performance et de fonctions (IPF)   
#'   

# décret n°2010-1705 du 30 décembre 2010
  
#+ ipf

résultat_IPF   <- test_prime(prime_IPF, prime_IFTS, Paie_I, Paie_IFTS, Lignes_IFTS, afficher.table.effectifs)

#'   
#'    
#'&nbsp;*Tableau `r incrément()` : Cumul IPF/IFTS*   
#'      

tableau_cumuls(résultat_IPF)

#'      
#'      
#'[Lien vers la base de données cumuls ipf/ifts](Bases/Reglementation/personnels.ipf.ifts.csv)    
#'[Lien vers la base de données IPF non cat.A](Bases/Reglementation/IPF.non.catA.csv)      
#'[Lien vers la base de données IPF non tit](Bases/Reglementation/IPF.non.tit.csv)       
#'   

résultat_IPF   <- test_prime(prime_IPF, prime_PFR, Paie_I, Paie_PFR, Lignes_PFR, verbeux = afficher.table.effectifs)

#'    
#'&nbsp;*Tableau `r incrément()` : Cumul IPF/PFR*   
#'      

tableau_cumuls(résultat_IPF)

#'      
#'      
#'[Lien vers la base de données cumuls ipf/pfr](Bases/Reglementation/personnels.ipf.pfr.csv)    
#'   

résultat_IPF   <- test_prime(prime_IPF, prime_ISS, Paie_I, Paie_ISS, Lignes_ISS, verbeux = afficher.table.effectifs)

#'    
#'&nbsp;*Tableau `r incrément()` : Cumul IPF/ISS*   
#'      

tableau_cumuls(résultat_IPF)

#'      
#'      
#'[Lien vers la base de données cumuls ipf/iss](Bases/Reglementation/personnels.ipf.iss.csv)    
#'   


# Attention keyby = et pas seulement by = !

#'  
#'&nbsp;*Tableau `r incrément()` : Valeurs de l'agrégat annuel (IPF ou IFTS) pour les bénéficiaires de l'IPF*        
#'          

agrégat_annuel(résultat_IPF, afficher.table.effectifs)

#'  
#'&nbsp;*Tableau `r incrément()` : Variations de l'agrégat mensuel moyen (IPF ou IFTS) pour les bénéficiaires de l'IPF*   
#'          

évolution_agrégat(résultat_IPF, afficher.table.effectifs)

#'   
#'[Lien vers la base de données agrégat IPF-IFTS](Bases/Remunerations/beneficiaires.IPF.IFTS.csv)    
#'    
#'   
#'[Lien vers la base de données variations agrégat IPF-IFTS](Bases/Remunerations/beneficiaires.IPF.IFTS.Variation.csv)    
#'   

rm(résultat_IPF)  
  
  
#### 2.9 HEURES SUP ####
#'    
#'## 2.9 Contrôle sur les heures supplémentaires

#'    
#'## `r chapitre`.11 Contrôle sur les heures supplémentaires    &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_IHTS.odt)   

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
              "NBI",
              "Libellé",
              étiquette.code,
              "Heures",
              "Heures.Sup.",
              "Temps.de.travail", 
              "quotité.moyenne.orig",
              "Base",
              "Nb.Unité",
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

lignes.IHTS <- Base.IHTS[indic == TRUE][ , indic := NULL]

Base.IHTS.non.tit <- lignes.IHTS[Statut != "TITULAIRE" & Statut != "STAGIAIRE"]

lignes.IHTS.rappels <- lignes.IHTS[Type == "R" & Montant != 0
         ][ , `:=`(ihts.cum.rappels = sum(Montant[Année.rappel == Année & Mois.rappel <= Mois], na.rm = TRUE),
                   nihts.cum.rappels = ifelse((a <- sum(abs(Base[Année.rappel == Année & Mois.rappel <= Mois]) * sign(Montant), na.rm = TRUE)) == 0, sum(abs(Nb.Unité[Année.rappel == Année & Mois.rappel <= Mois])  * sign(Montant), na.rm = TRUE), a),
                   ihts.cum.rappels.ant = sum(Montant[Année.rappel < Année], na.rm = TRUE),
                   nihts.cum.rappels.ant = ifelse((a <- sum(abs(Base[Année.rappel < Année]) * sign(Montant), na.rm = TRUE)) == 0, sum(abs(Nb.Unité[Année.rappel < Année]) * sign(Montant), na.rm = TRUE), a)), 
                 by= .(Matricule, Année.rappel, Mois.rappel)
         ][Année.rappel >= début.période.sous.revue 
                 & Mois.rappel >=1 
                 & Mois.rappel <= 12
         ][ , .(Matricule, Année, Mois, quotité, quotité.moyenne, Année.rappel, Mois.rappel, ihts.cum.rappels, nihts.cum.rappels, ihts.cum.rappels.ant, nihts.cum.rappels.ant)]

setnames(lignes.IHTS.rappels, "Année", "Année.R")
setnames(lignes.IHTS.rappels, "Mois", "Mois.R")
setnames(lignes.IHTS.rappels, "Année.rappel", "Année")
setnames(lignes.IHTS.rappels, "Mois.rappel", "Mois")

lignes.IHTS.hors.rappels <- lignes.IHTS[Type != "R" & Montant != 0, 
                                       .(ihts.cum.hors.rappels = sum(Montant, na.rm = TRUE),
                                         nihts.cum.hors.rappels = ifelse((a <- sum(abs(Base) * sign(Montant), na.rm = TRUE)) == 0, sum(abs(Nb.Unité) * sign(Montant), na.rm = TRUE), a),
                                         quotité.moyenne),
                                           by= .(Matricule, Année, Mois)]

lignes.IHTS.tot <- merge(lignes.IHTS.rappels, lignes.IHTS.hors.rappels, 
                         all = TRUE,
                         by = c("Matricule", "Année", "Mois", "quotité.moyenne"))[is.na(ihts.cum.rappels), ihts.cum.rappels := 0
                     ][is.na(ihts.cum.hors.rappels), ihts.cum.hors.rappels := 0
                     ][is.na(nihts.cum.rappels), nihts.cum.rappels := 0
                     ][is.na(nihts.cum.hors.rappels), nihts.cum.hors.rappels := 0
                     ][is.na(ihts.cum.rappels), ihts.cum.rappels := 0
                     ][is.na(ihts.cum.rappels.ant), ihts.cum.rappels.ant := 0
                     ][is.na(nihts.cum.rappels.ant), nihts.cum.rappels.ant := 0]

lignes.IHTS.tot[ ,  `:=`(ihts.tot = ihts.cum.rappels + ihts.cum.hors.rappels + ihts.cum.rappels.ant,
                         nihts.tot = nihts.cum.rappels + nihts.cum.hors.rappels + nihts.cum.rappels.ant)]

essayer(
{
  
  if ((N.IHTS.non.tit <<- uniqueN(Base.IHTS.non.tit$Matricule)) > 0) {
    
    cat(N.IHTS.non.tit, "attributaire" %s% N.IHTS.non.tit, " des IHTS sont des non-titulaires. Vérifier l'existence d'une délibération le prévoyant expressément. ")

  } else {
    
    cat("Tous les attributaires des IHTS sont titulaires ou stagiaires. ")
  }
  
# Taux.horaires donne des sommes annuelles pour les traitements et IHTS, afin de calculer les taux maxima "seuils"
# on prend en compte la NBI pour le calcul du taux (réponse ministérielle 23 mai 2006)   
  
  Taux.horaires <- Base.IHTS[ ,.(`IR` = sum(Montant[Type == "IR"], na.rm = TRUE),
                                  Indice = Indice[1],
                                  NBI = NBI[1],
                                  Heures.Sup. = Heures.Sup.[1]), # ajouter NBI proratisée !
                                         by = .(Matricule, Année, Mois, quotité)]
  
  Taux.horaires <- merge(Taux.horaires, lignes.IHTS.tot, by=c("Matricule", "Année", "Mois", "quotité"))
  
  setkey(Taux.horaires, Année, Mois)
  Taux.horaires[ , `Traitement indiciaire annuel et IR` := IR * 12 + (Indice + NBI) * PointIM[Année - 2007, Mois]
                              
                            ][ , `Taux horaire` := `Traitement indiciaire annuel et IR` / 1820 ]
  
  # Pour les temps partiels et les heures complémentaires, pas de sur-rémunération
  
  Taux.horaires[ ,  `:=` (`Taux horaire inf.14 H` = `Taux horaire`,
                          `Taux horaire sup.14 H` = `Taux horaire`,   
                          `Taux horaire nuit`     = `Taux horaire`,     
                          `Taux horaire dim. j.f.`= `Taux horaire`)]
                 
  Taux.horaires[quotité >= 0.98,  `:=`(`Taux horaire inf.14 H` = `Taux horaire` * 1.25,
                                   `Taux horaire sup.14 H` = `Taux horaire` * 1.27,   
                                   `Taux horaire nuit`     = `Taux horaire` * 2,     
                                   `Taux horaire dim. j.f.`= `Taux horaire` * 5/3)]             
                
  Taux.horaires[ ,   `:=` (Max = nihts.tot * `Taux horaire nuit`,
                           Min = nihts.tot * `Taux horaire inf.14 H`)
                        
                ][ ,  `:=`(Indice = NULL,
                                           IR = NULL)]   
  
 
},
  "La base des taux horaires d'heures supplémentaires n'a pas pu être générée. ")

# On considère le taux horaire maximum de nuit et la somme des IHTS 

 
#'     
#'&nbsp;*Tableau `r incrément()` : Paiements au-delà des seuils de liquidation pour l'exercice* &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_liquidation_IHTS.odt)      
#'    

# On considère le taux horaire maximum de nuit et la somme des IHTS et on teste su la somme des IHTS est supérieures à ce que donnerait l'application du taux de nuit

essayer(
{
  depassement <- Taux.horaires[ihts.tot > Max, uniqueN(Matricule)]
  depassement.agent <- Taux.horaires[ihts.tot > Max, 
                           .(`Coût en euros` = -Max + ihts.tot,
                             Matricule,
                             Max,
                             ihts.tot), keyby = Année]
  
  depassement.agent.annee <- depassement.agent[ , .(`Coût en euros` = sum(`Coût en euros`, na.rm = TRUE),
                                                     `Nombre d'agents` = uniqueN(Matricule)), keyby = Année]
  
  if (depassement) {
    
    cat("Il y a", depassement, "agent" %+% ifelse(depassement, "s", ""), "qui perçoivent davantage que le maximum d'IHTS pouvant être liquidé au titre du mois.") 
  }
    
},
"Le tableau des dépassements de coûts n'a pas pu être généré. ")

  with(depassement.agent.annee,
  
    Tableau.vertical2(c("Année", "Coût en euros", "Nombre d'agents"),
                         Année, digits = 0, `Coût en euros`, `Nombre d'agents`))         


#'     
#'[Lien vers la base de données des IHTS aux non-titulaires](Bases/Reglementation/Base.IHTS.non.tit.csv)           
#'[Lien vers le tableau des dépassements individuels des seuils de liquidation](Bases/Reglementation/depassement.agent.annee.csv)         
#'[Lien vers la base de données dépassements individuels des seuils de liquidation](Bases/Reglementation/depassement.agent.csv)         
#'[Lien vers la base de données calcul des taux horaires individuels](Bases/Reglementation/Taux.horaires.csv)        
#'       

#'*Le cumul des heures supplémentaires déclarées (colonne Heures.Sup. des bases) est, par année, comparé au cumul des bases de liquidation IHTS, pour l'année et en régularisation l'année suivante au titre du même exercice*    
#'*Le volume d'heures supplémentaires déclarées et non liquidées sous forme d'IHTS peut correspondre à d'autres régimes d'heures supplémentaires (enseignants, élections) ou à des heures supplémentaires non effectuées ou sous-déclarées*   
#'*Des différences importantes peuvent indiquer une mauvaise fiabilité des déclarations d'heures supplémentaires et/ou des bases de liquidation IHTS*             
#'           
#'       
#'&nbsp;*Tableau `r incrément()` : Cumuls d'heures supplémentaires déclarées et des IHTS payées, en nombre d'heures*     
#'    

essayer(
{
  CumHS <- Bulletins.paie[ , .(toths = sum(Heures.Sup., na.rm = TRUE)), keyby = Année]
  
  # Certaines bases de données indiquent le nombre d'heures sup dans la variable Base et d'autres dans la variable Nb.Unité
  # En principe un rappel concerne un mois antérieur. Mais de nombreuses BD ont des rappels du même mois...
  
  CumBaseIHTS <- unique(lignes.IHTS.tot[, .(Matricule, Année, Mois, quotité, quotité.moyenne, nihts.tot, nihts.cum.hors.rappels, nihts.cum.rappels, nihts.cum.rappels.ant)], by = NULL)

  TotBaseIHTS <- CumBaseIHTS[ , .(totihts = sum(nihts.tot),
                   totihts.hors.rappels = sum(nihts.cum.hors.rappels),
                   totihts.rappels = sum(nihts.cum.rappels),
                   totihts.rappels.ant = sum(nihts.cum.rappels.ant)),
                           keyby = Année]
  
  CumHS <- merge(CumHS, TotBaseIHTS, all = TRUE, by = "Année")
},
"La base des cumuls d'IHTS par année, des régularisations et des IHTS apparemment non liquidées n'a pas pu être générée. ")

  with(CumHS,
  
    Tableau.vertical2(c("Année N", "Cumul HS N", "Cumul IHTS N", "dont du mois", "dont rappels N", "dont payés N+1"),
                        Année,             toths,    totihts,   totihts.hors.rappels, totihts.rappels,  totihts.rappels.ant)
  
  )
#'    
#'    
  
  if ((l <- length(v <- CumHS[toths < totihts, Année])) > 0) {
    cat("Le nombre d'heures supplémentaires déclarées pour ")     
    cat(v, sep = ", ")
    cat(" est incohérent avec le nombre d'heures IHTS payées au titre de", ifelse(l > 1, "ces", "cet"), ifelse(l > 1, "exercices. ","exercice. "))
  }
  
#'    
#'[Lien vers les données du tableau](Bases/Reglementation/CumHS.csv)     
#'[Lien vers les cumuls IHTS par matricule](Bases/Reglementation/lignes.IHTS.tot.csv)       
#'[Lien vers les lignes IHTS](Bases/Reglementation/lignes.IHTS.csv)              
#'   

Depassement.seuil.180h <- data.table()
Dépassement.seuil.220h <- data.table()

nb.agents.dépassement <- 0
nb.agents.dépassement.220h <- 0


if (utiliser.variable.Heures.Sup.) {
  
  Depassement.seuil.180h <- Bulletins.paie[ , Nihts.tot := sum(Heures.Sup., na.rm = TRUE),
                                                        keyby = .(Matricule, Année)
                                                 ][ Nihts.tot > 180 * quotité.moyenne, 
                                                  .(Matricule, 
                                                    Année,
                                                    quotité.moyenne,
                                                    Nihts.tot,                        
                                                    Emploi,
                                                    Grade,
                                                    Service)]
} else {
  
  Depassement.seuil.180h <- merge(CumBaseIHTS[ , .(Nihts.tot = sum(nihts.tot), quotité.moyenne), by = .(Matricule, Année)
                                             ][Nihts.tot > 180 * quotité.moyenne, 
                                                .(Matricule, 
                                                  Année,
                                                  quotité.moyenne,
                                                  Nihts.tot)],
                                                 Bulletins.paie[Mois == 12 , .(
                                                                  Matricule,
                                                                  Année,
                                                                  Emploi,
                                                                  Grade,
                                                                  Service)], all.x = TRUE)

}


nb.agents.dépassement <- uniqueN(Depassement.seuil.180h$Matricule)

if  (nb.agents.dépassement)  {
  if (VERSANT_FP == "FPH") {  
        cat("Le seuil de 180 heures supplémentaires maximum est dépassé par ", 
            FR(nb.agents.dépassement), " agents.\n")
  } 
  
  Depassement.seuil.220h <- Depassement.seuil.180h[Nihts.tot > 220 * quotité.moyenne]
  
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

if (utiliser.variable.Heures.Sup.) {
  cat ("Les cumuls d'IHTS sont déterminés à partir de la variable Heures.Sup. ")
   HS.sup.25 <- lignes.IHTS[Heures.Sup. > seuil.HS * quotité]
} else {
   cat ("Les cumuls d'IHTS sont déterminés à partir des paiements de l'année, rappels compris, et des rappels payés l'année suivante. ")      
   HS.sup.25 <- CumBaseIHTS[nihts.tot > seuil.HS * quotité]
}

nombre.Lignes.paie.HS.sup.25 <- nrow(HS.sup.25)

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
#'Dans les tableaux en lien les grades, emplois et service sont ceux connus en fin d'année.    


#### 2.10 ELUS ####

#' 
#'## 2.10 Contrôle sur les indemnités des élus
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


#### 2.11 SFT ####

#'
#'## 2.11 Contrôle du supplément familial de traitement   
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
Paie.sans.enfant.reduit <- Paie[Type == "S" 
                                & (is.na(Nb.Enfants) | Nb.Enfants == 0),
                                 .(SFT.versé = sum(Montant, na.rm = TRUE)),
                                      keyby = "Matricule,Année,Mois"] 

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
    
#### 2.12 ASTREINTES ####

#'
#'## 2.12 Contrôle des astreintes
#'  


essayer({
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
},
"Le contrôle Astreintes-NBI n'a pas pu être réalisé.")

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

essayer({
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


}, 
"Le contrôle du cumul astreintes IHTS n'a pas pu être réalisé")

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

  
#### 2.13 RETRAITES ####

#'
#'## 2.13 Contrôle des cotisations de retraite    
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


#### 2.14 PRIMES FPH ####     

#'   
#'## 2.14 Primes de la fonction publique hospitalière          
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
#'Indemnités forfaitaire et technique : `r primes.ift.potentielles`    
#'   

#'   
#'[Lien vers la base de données IFT NT](Bases/Reglementation/personnels.ift.nt.csv)   
#'   

#'**Prime de service**   

primes.ps.potentielles <- ""
personnels.ps.nt <- NULL

if (VERSANT_FP == "FPH") {
  Paie_ps <- filtrer_Paie("PRIME DE SERVICE")
  
  personnels.ps.nt <- Paie_ps[Statut == "NON_TITULAIRE" | Statut == "AUTRE_STATUT" | grepl(expression.rég.médecin, Grade, perl = TRUE, ignore.case = TRUE), ..colonnes]
    
  if (nombre.personnels.ps.nt <- uniqueN(personnels.ps.nt)) {
    
    cat("Il existe ", 
        FR(nombre.personnels.ps.nt),
        "agent" %s% nombre.personnels.ps.nt,
        "non titulaire" %s% nombre.personnels.ps.nt, "percevant une prime de service.")
  }
}

#'    
#'    

if (VERSANT_FP == "FPH") {
  if (nombre.personnels.ps.nt) {
    
    cat("Coût des anomalies ", 
        personnels.ps.nt[ , sum(Montant, na.rm = TRUE)], "euros.")
  }
  
  primes.ps <- unique(Paie_ps$Libellé)
  
  primes.ps.potentielles <- if (length(primes.ps) == 0) "aucune" else paste(primes.ps, collapse = " ;")
}

#'   
#'Primes de service : `r primes.ps.potentielles`    
#'   

#'   
#'[Lien vers la base de données primes de service NT](Bases/Reglementation/personnels.ps.nt.csv)   
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

code.libelle <- unique(Paie[Montant != 0, .(Code, Libellé), by = "Type"][ , Libellé := toupper(Libellé)], by = NULL)
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
             "Depassement.seuil.180h",
             "Depassement.seuil.220h",
             "ifts.et.contractuel",
             "ihts.cat.A",
             "Controle_astreintes",
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
              "lignes.nbi.anormales.hors.rappels",
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

if (! debug.code)
   rm(list = setdiff(ls(), script_env))
