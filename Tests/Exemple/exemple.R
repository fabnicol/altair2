#'---
#'title: false
#'author: false
#'date: false
#'output:
#'html_document:
#'css: style.css
#'---

#'<p class = "centered"><img src = "Altair.bmp" /></p>
#'<p class = "title">RH Exemple</p> 
#'

#+ echo = FALSE, warning = TRUE, message = FALSE

# comportement global du programme

# Lorsque l'on n'a que une ou deux années, mettre étudier.variations à FALSE
# Lorsque l'on n'étudie pas une base Xémélios, mettre étudier.tests.statutaires à FALSE

library(compiler)

options(warn = -1, verbose = FALSE, OutDec = ",")
encodage.entrée <- "ISO-8859-1"

# dans cet ordre

try(setwd("Tests/Exemple"), silent = TRUE)

source("prologue.R", encoding = encodage.entrée)

compilerOptions <- setCompilerOptions(suppressAll = TRUE)
JITlevel        <- enableJIT(2)

source(file.path(chemin.dossier, "bibliotheque.fonctions.paie.R"), encoding = encodage.entrée)

installer.paquets(knitr, plyr, ggplot2, assertthat, yaml)

library(knitr)
library(plyr)
library(ggplot2)
library(assertthat)

# problème temporaire avec l'option fig.retina depuis fin mai 2014

knitr::opts_chunk$set(fig.width = 7.5, echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')

dir.create(chemin.dossier.bases, recursive = TRUE)

#'<p class = "centered"><b>Exercices `r paste(début.période.sous.revue, "à", fin.période.sous.revue)` </b></p>  
#'<p class = "author">Fabrice Nicol</h1>
#'
#+ echo = FALSE
#'`r format(Sys.Date(), "%a %d %b %Y")`  
#'

nombre.exercices <- fin.période.sous.revue - début.période.sous.revue + 1

# Le format est jour/mois/année avec deux chiffres-séparateur-deux chiffres-séparateur-4 chiffres.
# Le séparateur peut être changé en un autre en modifiant le "/" dans date.format

# Cette section pourra être modifiée en entrée dans d'autres contextes
# Matricule, Codes

# Contrôle de cohérence
#  on vérifie que chaque code de paie est associé, dans le fichier des codes de paiement (par défaut, racinecodes.csv),
#  que à chaque code donné on a associé un et un seul type de rémunération ("INDEMNITAIRE", "TRAITEMENT", etc.)           

if (file.exists(chemin(nom.fichier.codes.paiement)))
{
  Codes.paiement <- read.csv.skip(nom.fichier.codes.paiement)

  if (nlevels(as.factor(Codes.paiement$Code)) != nrow(unique(Codes.paiement[ , c(étiquette.code, "Type.rémunération")])))
  {
    message("Davantage de types de rémunérations que de codes distincts : incohérence de la base de codes.")
    
    V <- tapply(Codes.paiement$Type.rémunération, Codes.paiement$Code, function(x) length(unique(x))) 
    V <- V[V > 1]
    
    print(unique(merge(data.frame(Code = names(V),
                                  "Nombre de types de rémunérations distincts" = V,
                                   row.names=NULL),
                       Codes.paiement[Codes.paiement$Code %in% names(V), 
                                      c(étiquette.code, "Type.rémunération")],
                       by = étiquette.code,
                       all=TRUE)))
    
    stop("Vérifier le fichier " %+% nom.fichier.codes.paiement)
    
  }
  
  message("Chargement des codes de paiement.")
}

  
  fichier.personnels.existe <- file.exists(chemin(nom.fichier.personnels))

  if (fichier.personnels.existe)
  {
   base.personnels.catégorie <- read.csv.skip(nom.fichier.personnels)
   message("Chargement du fichier des catégories statutaires des personnels.")
  }

# Lignes de paie
# On peut lire jusqu'à 50 fichiers csv de lignes de paie qui seront générés au format :

# "chemin dossier + racine-Lignes de paye-j.csv" où racine est un bref identifiant du type de contrôle (exemple : "c2a", "PEV", ...)

lignes.paie <- nom.fichier.paie %+% "-" %+% 1:50 %+% ".csv"
lignes.paie <- lignes.paie[file.exists(chemin(lignes.paie))]

# Bulletins de paie 
# On peut lire jusqu'à 10 fichiers csv de bulletins de paie qui seront générés au format :

# "chemin dossier + racine-Bulletins de paye-j.csv" 

bulletins.paie <- nom.bulletin.paie %+% "-" %+% 1:10 %+% ".csv"
bulletins.paie <- bulletins.paie[file.exists(chemin(bulletins.paie))]

# Programme principal

# Bases

# Lignes de paie 

# On ne retient que les bases ayant pour années au minimum début.période.sous.revue 
# et au maximum fin.période.sous.revue, qui contiennent toutes les colonnes requises
# pour le contrôle

Read.csv("Lignes.paie", lignes.paie)

if (!is.null(Lignes.paie)) message("Chargement des lignes de paie.") else stop("Chargement des lignes de paie en échec.")

Read.csv("Bulletins.paie", bulletins.paie)

if (!is.null(Bulletins.paie)) message("Chargement des bulletins de paie.") else stop("Chargement des bulletins de paie en échec.")

Bulletins.paie <- Bulletins.paie[  Bulletins.paie$Année >= début.période.sous.revue 
                                 & Bulletins.paie$Année <= fin.période.sous.revue, ]
Lignes.paie    <- Lignes.paie[  Lignes.paie$Année >= début.période.sous.revue 
                              & Lignes.paie$Année <= fin.période.sous.revue, ]

Bulletins.paie.contiennent.colonnes.requises <- colonnes.requises %in% names(Bulletins.paie)

if (! all(Bulletins.paie.contiennent.colonnes.requises)) {
  
  stop("Il manque les colonnes suivantes au(x) fichier(s) Bulletins de paie :",
       colonnes.requises[! Bulletins.paie.contiennent.colonnes.requises])
  
} else {
  
  message("Contrôle des noms de colonne des bulletins de paie : normal.")
}

if (tester.matricules)  tester.homogeneite.matricules(Lignes.paie)

# Lors de la PREMIERE utilisation d'Altair, paramétrer générer.codes <- TRUE dans prologue.R
# pour générer les fichier des codes de paiement sous le dossier des bases (par défaut "Bases").
# ce fichier est trier par ordre croissant des codes de paiement sur les trois premiers chiffres des codes
# des anomalies peuvent résiduellement apparaître avec des codes contenant des lettres, en général après
# le troisième chiffre du code.
# L'utilisateur devra alors renseigner la colonne étiquette.type.rémunération de ce fichier

if (générer.codes) {
  
  message("Génération de la base des codes de paie et des libellés.")
  
  with( Lignes.paie,
        codes.paiement.généré <<- unique(Lignes.paie[  Montant > 0,
                                                       c(étiquette.code, étiquette.libellé)]))
  
  codes.paiement.généré <- cbind(codes.paiement.généré[order(substr(as.character(codes.paiement.généré$Code), 1, 3)), ],
                                 character(nrow(codes.paiement.généré)))
  
  names(codes.paiement.généré)[3] <- étiquette.Type.rémunération
  sauv.bases(chemin.dossier.bases, "codes.paiement.généré")
    
  #'---
  #'   
  #'# Tableau des codes de paiement
  #'
  #'##  Renseigner le type de rémunération
  #'
  #'Utiliser les codes : TRAITEMENT, INDEMNITAIRE, ELU, VACATIONS, AUTRES  
  #'  
  #'  
  kable(codes.paiement.généré, row.names = FALSE)
  #'                             
    
  
  if (file.exists(file.path(chemin.dossier.bases, "codes.paiement.généré.csv")))
    message("Génération des codes : voir fichier Bases/codes.paiement.généré.csv")
  else
    message("Les codes n'ont pas été générés.")
  
  stop(
" Le programme est arrêté après génération de la base de codes et libellés. 
  Relancer Altair après avoir renseigné la troisième colonne 
  et placé le fichier sous le répertoire racine avec le nom " %+% nom.fichier.codes.paiement, call.=FALSE)
}

# suppression des colonnes Nom Prénom redondantes

message("Nettoyage des bases.")

sélectionner.clé("Bulletins.paie", "Lignes.paie")

# Technique : les espaces de noms sont pollués par la sélection des clés, il faut les nettoyer

# unname(Bulletins.paie$Nom) devrait marcher mais cause une génération de tableau sous RMarkdown, probablement un bug.
# utiliser attr à la place.

attr(Bulletins.paie$Nom, "names") <- NULL
attr(Bulletins.paie$Prénom, "names") <- NULL
attr(Lignes.paie$Nom, "names") <- NULL
attr(Lignes.paie$Prénom, "names") <- NULL

# Extraction de vecteurs représentant les codes de paiement par type de code (indemnitaire, traitement, vacations...)

if (exists("Codes.paiement"))
{
   Codes.paiement.indemnitaire <- unique(Codes.paiement[Codes.paiement$Type.rémunération == modalité.indemnitaire, étiquette.code])
   Codes.paiement.principal.contractuel  <- unique(Codes.paiement[Codes.paiement$Type.rémunération == modalité.principal.contractuel,
                                                                  étiquette.code])
   Codes.paiement.vacataire    <- unique(Codes.paiement[Codes.paiement$Type.rémunération == modalité.vacations, étiquette.code])
   Codes.paiement.traitement   <- unique(Codes.paiement[Codes.paiement$Type.rémunération == modalité.traitement, étiquette.code])
   Codes.paiement.élu          <- unique(Codes.paiement[Codes.paiement$Type.rémunération == modalité.élu, étiquette.code])
   Codes.paiement.vacations    <- unique(Codes.paiement[Codes.paiement$Type.rémunération == modalité.vacations, étiquette.code])
   Codes.paiement.autres       <- unique(Codes.paiement[Codes.paiement$Type.rémunération == modalité.autres, étiquette.code])
   
   message("Extraction des codes par type de code.")
   
} else
  stop("Charger le fichier de codes de paiement.")

# Pour assurer une fusion correcte des bulletins et lignes de paie, il importe que les colonnes communes aux deux fichiers soient 
# exactement celles utilisées pour la clé d'appariement d'une part, et le tri sous chaque clé d'autre part, autrement dit :
# la clé (Matricule ou (Nom, Prénom) selon le cas) + Année + Mois

  if (! fusionner.nom.prénom) 
    Bulletins.paie <- subset(Bulletins.paie,
                             select = setdiff(names(Bulletins.paie), c("Nom", "Prénom")))
    
  if (! setequal(intersect(names(Lignes.paie), names(Bulletins.paie)), union(c("Mois", "Année"), clé.fusion)))
  {
   if (fusionner.nom.prénom) {
     
     stop("L'appariement ne peut se faire par les clés Nom, Prénom et Mois")
     
   } else {
     
     stop("L'appariement ne peut se faire par les clés Matricule et Mois")
   }
   
  } else {
   
    message("Pas de redondance des colonnes des bulletins et lignes de paie : la fusion peut être réalisée.")
  }

# Alternative en cas de difficulté :
#
# Bulletins.paie.Lignes.paie <- do.call(rbind, lapply(début.période.sous.revue:fin.période.sous.revue,
#                                                     function(x) 
#                                                        merge(Bulletins.paie[Bulletins.paie$Année == x, ],
#                                                              Lignes.paie[Lignes.paie$Année == x, ], 
#                                                              by=c(étiquette.matricule, "Mois"))))

# Lorsque les bases sont déjà chargées, ont peu désactiver le rechargement par charger.bases <- FALSE

if (charger.bases)
{
  anavar <- ddply(Bulletins.paie, 
                  c(étiquette.matricule, étiquette.année),
                  summarise,
                  # partie Analyse des variations par exercice #
                  Montant.net = sum(Net.à.Payer),
                  
                  # En principe la colonne Brut ne tient pas compte des remboursements d efrais ou des régularisations
                  Montant.brut = sum(Brut),
                  Statut = Statut[length(Net.à.Payer)],
                  mois.entrée = ifelse((minimum <- min(Mois)) != Inf,
                                       minimum,
                                       0),
                  mois.sortie = ifelse((maximum <- max(Mois)) != -Inf,
                                       maximum,
                                       0),
                  nb.jours = calcul.nb.jours.mois(mois.entrée[1], mois.sortie[1], Année[1]),
                  nb.mois  = mois.sortie[1] - mois.entrée[1] + 1)
  
  Bulletins.paie <- merge (Bulletins.paie, anavar)
  
  Bulletins.paie.Lignes.paie <- merge(Bulletins.paie,
                                      Lignes.paie,
                                      by = c(clé.fusion, "Année", "Mois"))

  if ( ! exists("Codes.paiement.indemnitaire"))  stop("Pas de fichier des Types de codes [INDEMNITAIRE]")
  if ( ! exists("Codes.paiement.principal.contractuel"))  stop("Pas de fichier des Types de codes [PRINCIPAL.CONTRACTUEL]")
  if ( ! exists("Codes.paiement.vacataire"))  stop("Pas de fichier des Types de codes [VACATAIRE]")
  if ( ! exists("Codes.paiement.traitement"))    stop("Pas de fichier des Types de codes [TRAITEMENT]")
  if ( ! exists("Codes.paiement.élu"))           stop("Pas de fichier des Types de codes [ELU]")
  if ( ! exists("Codes.paiement.autres"))        stop("Pas de fichier des Types de codes [AUTRES]")
    
  if (extraire.population) {
    
    Bulletins.paie.Lignes.paie <- Bulletins.paie.Lignes.paie[grepl(expression.rég.population, Bulletins.paie.Lignes.paie$Service, ignore.case=TRUE), ]
    Bulletins.paie             <- Bulletins.paie[grepl(expression.rég.population, Bulletins.paie$Service, ignore.case=TRUE), ]
  }
    
  
  Bulletins.paie.Lignes.paie <- mutate(Bulletins.paie.Lignes.paie,
                                       montant.traitement.indiciaire 
                                        = Montant*(Code %in% Codes.paiement.traitement),
                                       montant.rémunération.principale.contractuel 
                                        = Montant*(Code %in% Codes.paiement.principal.contractuel),
                                       montant.rémunération.vacataire 
                                        = Montant*(Code %in% Codes.paiement.vacataire),
                                       montant.primes 
                                        = Montant*(Code %in% Codes.paiement.indemnitaire),
                                       montant.autres.rémunérations 
                                        = Montant*(Code %in% Codes.paiement.autres),
                                       montant.indemnité.élu 
                                        = Montant*(Code %in% Codes.paiement.élu),

                                       ### EQTP  ###
                                       
                                       quotité
                                       =  Temps.de.travail / 100
                                       * ifelse(corriger.quotité,
                                                ifelse(is.na(Taux), 1, Taux),
                                                1)
                                       * (montant.rémunération.principale.contractuel > 0 
                                            | 
                                          montant.traitement.indiciaire > 0)
                                       * nb.mois / 12) 
  
  Analyse.rémunérations <- ddply(Bulletins.paie.Lignes.paie,
                                 c(clé.fusion, étiquette.année),
                                 summarise,
                                 
                                 Nir          = Nir[1],
                                 Montant.net  = Montant.net[1],
                                 Montant.brut = Montant.brut[1],
                                 Statut       = Statut[1],   
                                 mois.entrée  = mois.entrée[1],
                                 nb.jours     = nb.jours[1],
                                 nb.mois      = nb.mois[1],
                                 mois.sortie  = mois.sortie[1],
                                 Emploi       = Emploi[length(Net.à.Payer)],
                                 traitement.indiciaire               = sum(montant.traitement.indiciaire),
                                 rémunération.principale.contractuel = sum(montant.rémunération.principale.contractuel),
                                 rémunération.vacataire              = sum(montant.rémunération.vacataire),
                                 rémunération.indemnitaire           = sum(montant.primes),
                                 indemnités.élu                      = sum(montant.indemnité.élu),
                                 autres.rémunérations                = sum(montant.autres.rémunérations),
                                 
                                 # on ne considère que les rémunérations brutes (sans prise en compte des remboursements de frais aux salariés ou des régularisations)
                                 # pour être en homogénéïté avec la colonne Brut/Montant.brut
                                 
                                 total.rémunérations                 =  traitement.indiciaire                       # le premier et deuxième terme sont exclusifs
                                                                      + rémunération.principale.contractuel
                                                                      + rémunération.vacataire
                                                                      + rémunération.indemnitaire,
                                 
                                 # ici on ajoute les remboursements de frais professionnels (autres.rémunérations) et on enlève les régularisations (détachements..., màd...)
                                 
                                 total.rémunérations.et.remboursements =  total.rémunérations 
                                                                        + autres.rémunérations 
                                                                        + indemnités.élu,
                                 
                                 quotité = ifelse(length(quotité[quotité > 0]) > 0, 
                                                  quotité[quotité > 0][1], 
                                                  0),
                                 montant.net.eqtp = ifelse(quotité == 0, 0, Montant.net / quotité),
                                 part.rémunération.indemnitaire = ifelse( (s <- traitement.indiciaire + rémunération.principale.contractuel + rémunération.indemnitaire) == 0, 
                                                                          0,
                                                                          (rémunération.indemnitaire + rémunération.principale.contractuel )/ 
                                                                            s * 100))
  
  Analyse.rémunérations <- Analyse.rémunérations[!is.na(Analyse.rémunérations$total.rémunérations), ]
  

  if (length (Analyse.rémunérations$quotité[Analyse.rémunérations$quotité > 1]) > 0 & comportement.strict & corriger.quotité) stop("Détection de quotités > 1", call. = FALSE)


  Analyse.variations.par.exercice <- Analyse.rémunérations[ , c(clé.fusion, étiquette.année,
                                                                "montant.net.eqtp", 
                                                                "Statut",
                                                                "Montant.net",
                                                                "nb.jours",
                                                                "nb.mois",
                                                                "quotité")]
    
  Analyse.variations.synthèse <- ddply(Analyse.variations.par.exercice,
                                       clé.fusion,
                                       summarise,
                                       
                                       Nexercices = length(Année),
                                       # quotité.exercice.début = quotité[1],
                                       # quotité.exercice.sortie = quotité[Nexercices],
                                       nb.mois.exercice.début  = nb.mois[1],
                                       nb.mois.exercice.sortie = nb.mois[Nexercices],
                                       total.jours = sum(nb.jours),
                                       total.mois  = sum(nb.mois),
                                       montant.net.eqtp.début  = montant.net.eqtp[1],
                                       montant.net.eqtp.sortie = montant.net.eqtp[Nexercices],
                                       moyenne.rémunération.annuelle.sur.période = mean(montant.net.eqtp[montant.net.eqtp > 0], na.rm=TRUE),
                                       variation.rémunération = ifelse(Nexercices > 1 & montant.net.eqtp.début > 0,
                                                                       montant.net.eqtp.sortie / montant.net.eqtp.début -1,
                                                                       0),
                                       variation.moyenne.rémunération = 
                                         ifelse(total.mois == 0, 
                                                 0,
                                                ( ( 1 + variation.rémunération / 100 ) ^ (12 / total.mois) - 1) * 100),
                                       plus.2.ans  = (total.mois  >= 2*12),
                                       moins.2.ans = (total.mois < 2*12),
                                       moins.1.an  = (total.mois < 12),
                                       moins.six.mois = (total.mois < 6),
                                       statut = Statut[1])
  
  
  
  Analyse.variations.par.exercice <- na.omit(Analyse.variations.par.exercice[ ( Analyse.variations.par.exercice$Année > début.période.sous.revue 
                                                                               & Analyse.variations.par.exercice$Année < fin.période.sous.revue) | 
                                                                               Analyse.variations.par.exercice$nb.mois > seuil.troncature, ])
    
  temp <- Analyse.variations.synthèse[Analyse.variations.synthèse$plus.2.ans, clé.fusion] 
  
  trouver.ligne <- function(x, y) anyDuplicated(rbind(x, y)) > 1

  if (fusionner.nom.prénom) {
    
    Analyse.variations.par.exercice <- mutate(Analyse.variations.par.exercice,
                                            plus.2.ans = trouver.ligne(c(Nom, Prénom), temp))
  } else {
    
    Analyse.variations.par.exercice <- mutate(Analyse.variations.par.exercice,
                                            plus.2.ans = Matricule %in% temp)
  }
  
  rm(temp)
    
  Bulletins.paie.nir.total.hors.élus <- merge(Analyse.rémunérations[  Analyse.rémunérations$Année == fin.période.sous.revue 
                                                                    & Analyse.rémunérations$indemnités.élu == 0,
                                                                      c(clé.fusion, champ.nir) ],
                                              Bulletins.paie[  Bulletins.paie$Année == fin.période.sous.revue
                                                             & Bulletins.paie$Mois == 12,
                                                             c(clé.fusion, champ.nir)], 
                                              by = clé.fusion, all = FALSE)
  
  Bulletins.paie.nir.fonctionnaires  <- unique(Bulletins.paie[  Bulletins.paie$Année == fin.période.sous.revue 
                                                              & Bulletins.paie$Mois  == 12
                                                              & Bulletins.paie$Statut %in% c("TITULAIRE", "STAGIAIRE"),
                                                              c(clé.fusion, champ.nir)])
  
  Bulletins.paie.nir.total.hors.élus <- Bulletins.paie.nir.total.hors.élus[-3]
  names(Bulletins.paie.nir.total.hors.élus) <- c(clé.fusion, champ.nir)
  
  # Age au 31 décembre de l'exercice dernier.exerciceal de la période sous revue
  # ne pas oublier [ ,...] ici:
  
  extraire.nir <- function(Base) fin.période.sous.revue - (as.numeric(substr(as.character(
                                                            format(Base[ , champ.nir], scientific = FALSE)),
                                                      2, 3))
                                  + 1900)

  années.fonctionnaires   <- extraire.nir(Bulletins.paie.nir.fonctionnaires)

  années.total.hors.élus  <- extraire.nir(Bulletins.paie.nir.total.hors.élus)
}

########### Démographie ########################

#'# 1. Statistiques de population
#'
#'### 1.1 Ensemble des personnels non élus    

if (longueur.non.na(années.total.hors.élus) > 0)
   hist(années.total.hors.élus,
     xlab = "Âge au 31 décembre " %+% fin.période.sous.revue,
     xlim = c(18, 75),
     ylab = "Effectif",
     main = "Pyramide des âges",
     col = "blue",
     nclass = 50)

#'
#'[Lien vers la base des âges](Bases/Bulletins.paie.nir.total.hors.élus.csv)
#'

Résumé("Âge des personnels <br>au 31/12/" %+% fin.période.sous.revue, années.total.hors.élus, align = 'c')

#'Effectif total: `r length(années.total.hors.élus)`  
#'
#'######      <br>
#'
#'
#'### 1.2 Ensemble des fonctionnaires stagiaires et titulaires    

if (longueur.non.na(années.fonctionnaires) > 0)
 hist(années.fonctionnaires,
     xlab = "Âge au 31 décembre " %+% fin.période.sous.revue,
     xlim = c(18,68),
     ylab = "Effectif",
     main = "Pyramide des âges des fonctionnaires",
     col = "navy",
     nclass = 50)

#'
#'[Lien vers la base des âges](Bases/Bulletins.paie.nir.fonctionnaires.csv)
#'

Résumé("Âge des personnels <br>au 31/12/" %+% fin.période.sous.revue,
       années.fonctionnaires,
       align = 'c')

#'   
#'**Effectif total: `r length(années.fonctionnaires)`**     
#'

#'
#'### 1.3 Effectifs des personnels par durée de service
#'
#'**Personnels en fonction des exercices `r début.période.sous.revue` à `r fin.période.sous.revue` inclus :**  
#'
attach(Analyse.variations.synthèse)

Tableau(c("Plus de 2 ans",
          "Moins de 2 ans",
          "Moins d'un an", 
          "Moins de six mois"), 
        sum(plus.2.ans),
        sum(moins.2.ans),
        sum(moins.1.an), 
        sum(moins.six.mois))

#'

if (nrow(Analyse.variations.par.exercice) > 0)
  qplot(factor(Année), 
        data = Analyse.variations.par.exercice,
        geom = "bar",
        fill = factor(!plus.2.ans),
        main = "Evolutions entre " %+% début.période.sous.revue %+% " et " %+% fin.période.sous.revue,
        xlab = étiquette.année,
        ylab = "Effectif",
        asp = 4)        + 
  scale_fill_discrete(name = "Personnels en fonction",
                      breaks = c(TRUE, FALSE),
                      labels = c("Moins de deux ans", "Plus de deux ans"))

detach(Analyse.variations.synthèse)

#'
#'**Nota:**  
#'Personnels en place : ayant servi au moins 730 jours pendant la période.  
#'Toutes les durées de service sont calculées en multiples de 365 jours.  
#'Dans les statistiques de cette section, les élus ne sont pas pris en compte.
#'


# on ne compte pas les élus dans le total (voir 5.6)

année <- début.période.sous.revue

colonnes.sélectionnées <- c("traitement.indiciaire",
                            "rémunération.indemnitaire",
                            "autres.rémunérations", 
                            "total.rémunérations",
                            "total.rémunérations.et.remboursements",
                            "Montant.brut",
                            "part.rémunération.indemnitaire",
                            clé.fusion)



# pour année début #


###########  Analyse des rémunérations  début de période###################
#  ATTENTION : les années doivent être SUCCESSIVES                        #


Analyse.rémunérations.premier.exercice <- Analyse.rémunérations[Analyse.rémunérations$Année == année, ]
  
if (fichier.personnels.existe)
{
  Analyse.rémunérations.premier.exercice <- merge(Analyse.rémunérations.premier.exercice, 
                                                  base.personnels.catégorie, 
                                                  by = clé.fusion, all = FALSE)
  
  colonnes.sélectionnées <- c(colonnes.sélectionnées, "Catégorie") 
}

attach(Analyse.rémunérations.premier.exercice, warn.conflicts = FALSE)

########### Analyse statique premier exercice ########################
#'# 2. Rémunérations brutes : analyse pour l'exercice `r année` 
#'
#'## 2.1 Statistiques de position globales (tous statuts)       
#'

temp <- colSums(Analyse.rémunérations.premier.exercice[c("Montant.brut", 
                                                         "total.rémunérations",
                                                         "indemnités.élu",
                                                         "autres.rémunérations")])

#'### Cumuls des rémunérations brutes pour l'exercice `r année`
#'

Tableau(c("Rémunérations brutes", 
          "Indemnités d'élus", 
          "Autres paiements",
          "Total brut"),
        temp["total.rémunérations"],
        temp["indemnités.élu"],
        temp["autres.rémunérations"],
        sum(temp[c("total.rémunérations", "indemnités.élu", "autres.rémunérations")]))

#'   
#'**Définitions :**   
#'Tous les items du tableau sont calculés sur le champ Montant de la base Lignes de paie  
#'  *Rémunération brutes* : somme du champ *Montant* des rémunérations versées hors élus  
#'  *Indemnités d'élus*   : somme du champ *Montant* des rémunérations versées pour les élus   
#'  *Autres paiements*    : remboursements de frais, régularisations, etc., non compris dans les deux premiers agrégats    
#'  *Total brut*          : somme des trois précédents facteurs   
#'

somme.brut.non.élu  <- sum(Bulletins.paie[  Bulletins.paie$Année == année 
                                          & ! Bulletins.paie$Emploi %in% libellés.élus
                                          & ! Bulletins.paie$Service %in% libellés.élus, 
                                            "Brut"])

delta  <- somme.brut.non.élu - temp["total.rémunérations"]               

#'**Tests de cohérence**  
#'
#'Somme des rémunérations brutes versées aux personnels (non élus) :   
#'
Tableau.vertical2(c("Agrégats",
                    "euros"),
                 c("Bulletins de paie (euros)",
                   "Lignes de paie (euros)",
                   "Différence (euros)"),
                 c(somme.brut.non.élu,
                   temp["total.rémunérations"],
                   delta))


delta2 <-  temp["Montant.brut"] - temp["total.rémunérations"] - temp["indemnités.élu"]


#'     
#'à comparer aux soldes des comptes 641 et 648 du compte de gestion.   
#'
#'Somme des rémunérations brutes versées (élus compris) :   
#'
Tableau.vertical2(c("Agrégats",
                    "euros"),
                 c("Bulletins de paie (euros)",
                   "Lignes de paie (euros)",
                   "Différence (euros)"),
                 c(temp["Montant.brut"],
                   temp["total.rémunérations"] + temp["indemnités.élu"],
                   delta2))

#'  
#'à comparer aux soldes des comptes 641, 648 et 653 du compte de gestion   
#'    
#'**Définitions :**   
#'  *Bulletins de paie*   : somme du champ *Brut* de la base Bulletins de paie. Le champ *Brut* ne tient pas compte des *Autres paiements* (remboursements de frais, régularisations, etc.) en base de données.  
#'  *Lignes de paie*      : somme des lignes de paie correspondantes de la base Lignes de paie sans tenir compte des *Autres paiements*   
#'

Sauv.base(chemin.dossier.bases, "temp", "Masses." %+% année)

rm(temp)

#'
#'[Lien vers la base de données](Bases/`r paste0("Masses.", année, ".csv")`)    
#'        
#'Les résultats sont exprimés en euros.  
#'  

#'
#'## 2.2 Fonctionnaires titulaires et stagiaires
#'

filtre.fonctionnaire <- function (X) X[ Statut %in% c("TITULAIRE", "STAGIAIRE") & X >0 ]

AR <- Analyse.rémunérations.premier.exercice[Statut %in% c("TITULAIRE", "STAGIAIRE"), colonnes.sélectionnées]

if (longueur.non.na(filtre.fonctionnaire(total.rémunérations) > 0))
 hist(filtre.fonctionnaire(total.rémunérations)/1000,
     xlab = "En milliers d'euros hors\nindemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 120),
     main = "Rémunération annuelle des fonctionnaires en " %+% année,
     col = "blue",
     nclass = 50)

#'    

if (longueur.non.na(filtre.fonctionnaire(rémunération.indemnitaire) > 0))
  hist(filtre.fonctionnaire(rémunération.indemnitaire)/1000,
     xlab = "En milliers d'euros hors\nindemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 70),
     main = "Rémunération indemnitaire annuelle des fonctionnaires en " %+% année,
     col = "blue",
     nclass = 50)

if (longueur.non.na(filtre.fonctionnaire(part.rémunération.indemnitaire) > 0))
  hist(filtre.fonctionnaire(part.rémunération.indemnitaire),
     xlab = "Part des indemnités dans la rémunération en %\n hors indemnités journalières et remboursements",
     ylab = "Effectif",
     main = "Part indemnitaire de la rémunération annuelle des fonctionnaires en " %+% année,
     xlim = c(0,60),     
     col = "blue",
     nclass = 30)

#'     
#'**Effectif : `r nrow(AR)`**     
#'
#'**Tests de cohérence**  

if (nrow(AR) > 0) {
   temp <- colSums(AR[ ,c("Montant.brut", "total.rémunérations")])
} else {
   temp <- c(0,0) }

#'Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :   
#'

Tableau.vertical2(c("Agrégats", "euros"), 
                  c("Bulletins de paie (euros)",  
                    "Lignes de paie (euros)",
                    "Différence (euros)"),
                  c(temp["Montant.brut"],
                    temp["total.rémunérations"],
                    temp["Montant.brut"] - temp["total.rémunérations"])) 

rm(temp)

#' 
#'A comparer aux soldes des comptes 6411, 6419 et 648 du conmpte de gestion.     
#'

#'   
#'### Statistiques de position pour l'exercice `r année`  
#'


Résumé(c("Traitement indiciaire",
         étiquette.rém.indemn,
         "Autres rémunérations"), 
       AR[c("traitement.indiciaire", "rémunération.indemnitaire", "autres.rémunérations")])

#'    
#'
Résumé(c("Total rémunérations", "Part de la rémunération contractuelle ou indemnitaire"),
       AR[c("total.rémunérations", "part.rémunération.indemnitaire")])



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
             "rémunération.indemnitaire",
             "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{  
  Résumé(c("Total rémunérations", "Part de la rémunération contractuelle ou indemnitaire"),
          ARA[c( "total.rémunérations", "part.rémunération.indemnitaire")])
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
                "rémunération.indemnitaire",
                "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{  
  Résumé(c("Total rémunérations", 
           "Part de la rémunération contractuelle ou indemnitaire"),
         ARB[ c( "total.rémunérations",
                 "part.rémunération.indemnitaire")])
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
                "rémunération.indemnitaire",
                "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{  
  Résumé(c("Total rémunérations", 
           "Part de la rémunération contractuelle ou indemnitaire"),
         ARC[ c( "total.rémunérations",
                 "part.rémunération.indemnitaire") ])
}

#'**Effectif : `r nrow(ARC)`**     
#'    
#'    
#'######      <br>
#'
#'## 2.3 Contractuels, vacataires et stagiaires inclus
#'

temp <- total.rémunérations[ indemnités.élu == 0 
                             & ! Statut %in% c("TITULAIRE", "STAGIAIRE")
                             & total.rémunérations > 1000] / 1000

if (length(temp > 0))
  hist(temp,
     xlab = "Rémunération en milliers d'euros \n hors indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 40),
     main = "Rémunération annuelle totale des contractuels en " %+% année,
     col = "red",
     nclass = 50)
#'
#'**Nota:**   
#'Ne sont retenues que les rémunérations supérieures à 1 000 euros.  
#'Les élus ne sont pas pris en compte.   
#'

temp <- positive(autres.rémunérations)

if (length(temp))
  hist(temp,
       xlab = "En euros :\n indemnités journalières et remboursements",
       ylab = "Effectif",
       xlim = c(0, 5000),
       main = "Autres rémunérations en " %+% année,
       nclass = 50,
       col = "grey")

#'   
#'    

AR <- Analyse.rémunérations.premier.exercice[  indemnités.élu == 0 
                                              & ! Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                                              c("rémunération.indemnitaire", 
                                                "autres.rémunérations",
                                                "total.rémunérations") ]

#'
Résumé(c(étiquette.rém.indemn, 
         "Autres rémunérations"),
       AR[c("rémunération.indemnitaire", "autres.rémunérations")])

#'

Résumé("Total rémunérations",   AR["total.rémunérations"])
#'   
#'**Effectif : `r nrow(AR)`**     
#'   

nom.base.analyse <- "Analyse.rémunérations.premier.exercice"

if (sauvegarder.bases)
  Sauv.base(chemin.dossier.bases, "Analyse.rémunérations.premier.exercice", nom.base.analyse)

detach(Analyse.rémunérations.premier.exercice)

# pour année fin #

année <- fin.période.sous.revue

#'

Bulletins.paie.Lignes.paie.dernier.exercice <-  Bulletins.paie.Lignes.paie[Bulletins.paie.Lignes.paie$Année == fin.période.sous.revue, ]

Bulletins.paie.Lignes.paie.dernier.exercice <- mutate(Bulletins.paie.Lignes.paie.dernier.exercice,
                                     montant.traitement.indiciaire 
                                       = Montant*(Code %in% Codes.paiement.traitement),
                                     montant.primes 
                                       = Montant*(Code %in% Codes.paiement.indemnitaire),
                                     montant.autres.rémunérations 
                                       = Montant*(Code %in% Codes.paiement.autres),
                                     montant.indemnité.élu 
                                       = Montant*(Code %in% Codes.paiement.élu))

###########  Analyse des rémunérations  fin de période###################
#  ATTENTION : les années doivent être SUCCESSIVES                      #


Analyse.rémunérations.dernier.exercice <- Analyse.rémunérations[Analyse.rémunérations$Année == année, ]

if (fichier.personnels.existe)
{
  Analyse.rémunérations.dernier.exercice <- merge(Analyse.rémunérations.dernier.exercice, base.personnels.catégorie, by = clé.fusion, all = FALSE)
}

attach(Analyse.rémunérations.dernier.exercice, warn.conflicts = FALSE)

########### Analyse statique dernier exercice ########################

#'# 3. Rémunérations brutes : analyse pour l'exercice `r année` 
#'
#'## 3.1 Statistiques de position globales (tous statuts)       
#'

temp <- colSums(Analyse.rémunérations.dernier.exercice[c("Montant.brut", "total.rémunérations", "indemnités.élu", "autres.rémunérations")])

#'### Cumuls des rémunérations brutes pour l'exercice `r année`
#'

Tableau(c("Rémunérations brutes", 
          "Indemnités d'élus", 
          "Autres paiements",
          "Total brut"),
        temp["total.rémunérations"],
        temp["indemnités.élu"],
        temp["autres.rémunérations"],
        sum(temp[c("total.rémunérations", "indemnités.élu", "autres.rémunérations")]))

#'   
#'**Définitions :**   
#'Tous les items du tableau sont calculés sur le champ Montant de la base Lignes de paie  
#'  *Rémunération brutes* : somme du champ *Montant* des rémunérations versées hors élus  
#'  *Indemnités d'élus*   : somme du champ *Montant* des rémunérations versées pour les élus   
#'  *Autres paiements*    : remboursements de frais, régularisations, etc., non compris dans les deux premiers agrégats    
#'  *Total brut*          : somme des trois précédents facteurs   
#'

#'
#'Cumuls réalisés sur les lignes de paie. Les indemnités d'élu ne sont pas prises en compte.  
#'

somme.brut.non.élu  <- sum(Bulletins.paie[  Bulletins.paie$Année == année 
                                          & ! Bulletins.paie$Emploi %in% libellés.élus
                                          & ! Bulletins.paie$Service %in% libellés.élus, 
                                          "Brut"])

             delta  <- somme.brut.non.élu - temp["total.rémunérations"]

#'**Tests de cohérence**  
#'
#'Somme des rémunérations brutes versées aux personnels (non élus) :   
#'

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Bulletins de paie (euros)",
                    "Lignes de paie (euros)",
                    "Différence (euros)"),
                  c(somme.brut.non.élu,
                    temp["total.rémunérations"],
                    delta))

delta2 <-  temp["Montant.brut"] - temp["total.rémunérations"] - temp["indemnités.élu"]


#'     
#'à comparer aux soldes des comptes 641 et 648 du compte de gestion.   
#'    
#'**Définitions :**   
#'  *Bulletins de paie*   : somme du champ *Brut* de la base Bulletins de paie. Le champ *Brut* ne tient pas compte des *Autres paiements* (remboursements de frais, régularisations, etc.) en base de données.  
#'  *Lignes de paie*      : somme des lignes de paie correspondantes de la base Lignes de paie sans tenir compte des *Autres paiements*   
#'
#'Somme des rémunérations brutes versées (élus compris) :   
#'

Tableau.vertical2(c("Agrégats",
                    "euros"),
                  c("Bulletins de paie (euros)",
                    "Lignes de paie (euros)",
                    "Différence (euros)"),
                  c(temp["Montant.brut"],
                    temp["total.rémunérations"] + temp["indemnités.élu"],
                    delta2))

#'  
#'à comparer aux soldes des comptes 641, 648 et 653 du compte de gestion   
#'

Sauv.base(chemin.dossier.bases, "temp", "Masses." %+% année)

rm(temp)

#'
#'[Lien vers la base de données](Bases/`r paste0("Masses.", année, ".csv")` )   
#'
#'Les résultats sont exprimés en euros.  
#'
#'
#'######      

#'## 3.2 Fonctionnaires titulaires et stagiaires
#'

if (longueur.non.na(filtre.fonctionnaire(total.rémunérations)) > 0)
  hist(filtre.fonctionnaire(total.rémunérations) / 1000,
     xlab = "En milliers d'euros \n hors indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 120),
     main = "Rémunération annuelle des fonctionnaires en " %+% année,
     col = "blue",
     nclass = 50)

#'  
#'

if (longueur.non.na(filtre.fonctionnaire(rémunération.indemnitaire) > 0))
  hist(filtre.fonctionnaire(rémunération.indemnitaire)/1000,
     xlab = "En milliers d'euros\n hors indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 70),
     main = "Rémunération indemnitaire annuelle\ndes fonctionnaires en " %+% année,
     col = "blue",
     nclass = 50)

#'    
#'

if (longueur.non.na(filtre.fonctionnaire(part.rémunération.indemnitaire)) > 0)
  hist(filtre.fonctionnaire(part.rémunération.indemnitaire),
     xlab = "Pourcentage des indemnités dans la rémunération\n hors indemnités journalières et remboursements",
     ylab = "Effectif",
     main = "Part indemnitaire de la rémunération annuelle des fonctionnaires en " %+% année,
     xlim = c(0,60),     
     col = "blue",
     nclass = 30)

#'
#'**Tests de cohérence**  

AR <- Analyse.rémunérations.dernier.exercice[Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                                                                    colonnes.sélectionnées ]
        
if (nrow(AR) > 0) {          
   temp <- colSums(AR[ ,c("Montant.brut", "total.rémunérations")])
} else {
   temp <- c(0,0) }
        

#'Somme des rémunérations brutes versées aux personnels titulaires et stagiaires :   
#'

Tableau.vertical2(c("Agrégats", "euros"), 
                  c("Bulletins de paie (euros)",  
                    "Lignes de paie (euros)",
                    "Différence (euros)"),
                  c(temp["Montant.brut"],
                    temp["total.rémunérations"],
                    temp["Montant.brut"] - temp["total.rémunérations"])) 


rm(temp)
#' 
#'A comparer aux soldes des comptes 6411, 6419 et 648 du compte de gestion.     
#'

#'
#'### Statistiques de position pour l'exercice `r année`  
#'


Résumé(c("Traitement indiciaire",
         étiquette.rém.indemn,
         "Autres rémunérations"), 
       AR[c("traitement.indiciaire", "rémunération.indemnitaire", "autres.rémunérations")])
#'

Résumé(c("Total rémunérations", 
         "Part de la rémunération contractuelle ou indemnitaire"), 
       AR[c("total.rémunérations", "part.rémunération.indemnitaire")])
       
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
               "rémunération.indemnitaire",
               "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{  
  Résumé(c("Total rémunérations",
           "Part de la rémunération contractuelle ou indemnitaire"),
         ARA[ c( "total.rémunérations",
                 "part.rémunération.indemnitaire")])
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
               "rémunération.indemnitaire",
               "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{  
  Résumé(c("Total rémunérations", 
           "Part de la rémunération contractuelle ou indemnitaire"),
         ARB[ c( "total.rémunérations",
                 "part.rémunération.indemnitaire")])
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
                "rémunération.indemnitaire",
               "autres.rémunérations")])
} else
  cat("Pas de statistique en l'absence de fichier des catégories.\n")

#'

if (fichier.personnels.existe)
{  
  Résumé(c("Total rémunérations",
           "Part de la rémunération contractuelle ou indemnitaire"),
         ARC[ c( "total.rémunérations",
                 "part.rémunération.indemnitaire")])
}

#'  
#'**Effectif : `r nrow(ARC)`**     
#'
#'######      
#'
#'
#'## 3.3 Contractuels, vacataires et stagiaires inclus
#'

temp <- total.rémunérations[   indemnités.élu == 0
                             & ! Statut %in% c("TITULAIRE", "STAGIAIRE")
                             & total.rémunérations > 1000]/1000

if (longueur.non.na(temp) > 0)
  hist(temp,
     xlab = "Rémunération en milliers d'euros\n hors indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 40),
     main = "Rémunération annuelle totale des contractuels en " %+% année,
     col = "red",
     nclass = 50)

#'
#'**Nota:**   
#'Ne sont retenues que les rémunérations supérieures à 1 000 euros.  
#'Les élus ne sont pas pris en compte.   

temp <- positive(autres.rémunérations)

if (longueur.non.na(temp) > 0)
  hist(temp,
     xlab = "En euros :\n indemnités journalières et remboursements",
     ylab = "Effectif",
     xlim = c(0, 5000),
     main = "Autres rémunérations en " %+% année,
     col = "grey")
#'    

AR <- Analyse.rémunérations.dernier.exercice[   indemnités.élu == 0 
                                              & ! Statut %in% c("TITULAIRE", "STAGIAIRE"), 
                                              c("rémunération.indemnitaire",
                                                "autres.rémunérations",
                                                "total.rémunérations") ]

#'
Résumé(c(étiquette.rém.indemn, "Autres rémunérations"), AR[c("rémunération.indemnitaire", "autres.rémunérations")])

#'

Résumé("Total rémunérations",   AR["total.rémunérations"])
#'   
#'**Effectif : `r nrow(AR)`**     
#'   

detach(Analyse.rémunérations.dernier.exercice)


#'
#'[Lien vers la base de données](Bases/Analyse.rémunérations.csv) d'analyse des rémunérations
#'


########### Analyse dynamique ########################
#'
#'# 4. Rémunérations nettes : évolutions sur la période `r début.période.sous.revue` - `r fin.période.sous.revue` 
#'
#'Nombre d'exercices: `r nombre.exercices`  
#'    
#'## 4.1 Rémunération nette moyenne sur la période

attach(Analyse.variations.synthèse)

temp <- positive(moyenne.rémunération.annuelle.sur.période) / 1000

if (longueur.non.na(temp) > 0)
  hist(temp,
     xlab = "Sur la période " %+% début.période.sous.revue %+% "-" %+% fin.période.sous.revue %+% " en milliers d'euros",
     ylab = "Effectif",
     main = "Rémunération nette moyenne",
     col = "blue",
     nclass = 100)

#'  
#+ fig.height=4.5  

temp <- na.omit(moyenne.rémunération.annuelle.sur.période[  moyenne.rémunération.annuelle.sur.période > 0 
                                                  & (statut == "TITULAIRE"  | statut == "STAGIAIRE")] / 1000)

if (longueur.non.na(temp) > 0)
  hist(temp,
     xlab = "Sur la période "%+% début.période.sous.revue %+% "-" %+% fin.période.sous.revue %+% " en milliers d'euros",
     ylab = "Effectif",
     main = "Rémunération nette moyenne des fonctionnaires",
     col = "blue",
     nclass = 100)

#'
#'[Lien vers la base de données](Bases/Analyse.variations.synthèse.csv)
#'
#'**Nota:** La rémunération nette perçue est rapportée au cumul des jours d'activité.  

Analyse.variations.synthèse.filtrée <- na.omit(Analyse.variations.synthèse[ nb.mois.exercice.début > seuil.troncature 
                                                                            & nb.mois.exercice.sortie   > seuil.troncature
                                                                            #  &  statut %in% c("TITULAIRE", "STAGIAIRE")
                                                                            &  statut !=  "AUTRE_STATUT",
                                                                            c("montant.net.eqtp.début",
                                                                              "montant.net.eqtp.sortie",
                                                                              "moyenne.rémunération.annuelle.sur.période",
                                                                              "variation.rémunération",
                                                                              "variation.moyenne.rémunération", 
                                                                              "plus.2.ans",
                                                                              clé.fusion)])

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
#'**Effectif : `r nrow(Analyse.variations.synthèse.filtrée[clé.fusion])`**     
#'
#'[Lien vers la base de données](Bases/Analyse.variations.synthèse.filtrée.csv)
#'
#'### 4.2.2 Personnels fonctionnaires et non titulaires en place
#'

if (nrow(Analyse.variations.synthèse.filtrée.plus.2.ans) > 0)
  hist(Analyse.variations.synthèse.filtrée.plus.2.ans$variation.moyenne.rémunération,
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
#'**Effectif :** `r nrow(Analyse.variations.synthèse.filtrée.plus.2.ans[clé.fusion])`    
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

if (nrow(Analyse.variations.synthèse.filtrée.moins.2.ans) > 0)
  hist(Analyse.variations.synthèse.filtrée.moins.2.ans$variation.moyenne.rémunération,
       xlab ="Variation annuelle moyenne en %",
       xlim = c(-10,30),
       las = 1,
       ylab ="Effectifs",
       main ="Rémunération nette des personnels en fonction moins de deux ans",
       col ="turquoise",
       nclass = 100)

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
#'**Effectif :** `r nrow(Analyse.variations.synthèse.filtrée.moins.2.ans[clé.fusion])`    
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
                                                        étiquette.code,
                                                        étiquette.libellé,
                                                        "Mois",
                                                        étiquette.montant)]

nombre.Lignes.paie.NBI.nontit <- nrow(NBI.aux.non.titulaires)

# Prime de fonctions informatiques : pas dans la base de VLB
# on cherche la chaine de char. "INFO" dans les libellés de primes

# variante : filtre <- regexpr(".*(INFO|PFI|P.F.I).*", toupper(Bulletins.paie.Lignes.paie$Libellé)) et regmatches(Bulletins.paie.Lignes.paie$Libellé, filtre)

filtre <- grep(".*(INFO|PFI|P.F.I).*", Libellé)

personnels.prime.informatique <- Bulletins.paie.Lignes.paie[ filtre,
                                                             c(étiquette.matricule,
                                                               "Statut",
                                                               étiquette.code,
                                                               étiquette.libellé,
                                                               étiquette.montant)]

primes.informatiques.potentielles <- unique(Libellé[filtre])

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
#'#       
#'    
#'## 5.2 Contrôle des vacations pour les fonctionnaires  

# Vacations et statut de fonctionnaire

lignes.fonctionnaires.et.vacations <- Bulletins.paie.Lignes.paie[ Statut %in% c("TITULAIRE", "STAGIAIRE")
                                                                  & Code %in% Codes.paiement.vacations,
                                                                 c(étiquette.matricule,
                                                                   "Statut",
                                                                   étiquette.code,
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

    lignes.contractuels.et.vacations <- Bulletins.paie.Lignes.paie[   ! Statut %in% c("TITULAIRE", "STAGIAIRE")  
                                                                    & Code %in% Codes.paiement.vacations,
                                                                    c(étiquette.matricule,
                                                                      étiquette.code,
                                                                      étiquette.libellé,
                                                                      étiquette.montant)]

matricules.contractuels.et.vacations <- unique(lignes.contractuels.et.vacations$Matricule)
    nombre.contractuels.et.vacations <- length(matricules.contractuels.et.vacations)
                     RI.et.vacations <- Bulletins.paie.Lignes.paie[   Matricule %in% matricules.contractuels.et.vacations
                                                                    & Code %in% Codes.paiement.indemnitaire,
                                                                    c(étiquette.matricule,
                                                                      "Statut", 
                                                                      étiquette.code,
                                                                      étiquette.libellé,
                                                                      étiquette.montant)]
# Vacations et indiciaire

traitement.et.vacations <- Bulletins.paie.Lignes.paie[   Matricule %in% matricules.contractuels.et.vacations 
                                                       & Code %in% Codes.paiement.traitement,
                                                       c(étiquette.matricule,
                                                         "Statut",
                                                         étiquette.code,
                                                         étiquette.libellé,
                                                         étiquette.montant)]

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

                filtre.iat  <- grep(expression.rég.iat, Libellé, ignore.case = TRUE)
                filtre.ifts <- grep(expression.rég.ifts, Libellé, ignore.case = TRUE)
                codes.ifts  <- unique(Bulletins.paie.Lignes.paie[filtre.ifts, étiquette.code])
        
        personnels.iat.ifts <- intersect(as.character(Bulletins.paie.Lignes.paie[ filtre.iat, clé.fusion[1]]),
                                         as.character(Bulletins.paie.Lignes.paie[ filtre.ifts, clé.fusion[1]]))

 nombre.personnels.iat.ifts <- length(personnels.iat.ifts)

#'

Tableau(c("Codes IFTS", "Nombre de personnels percevant IAT et IFTS"),
        sep.milliers = "",
        paste0(codes.ifts, collapse = " "),
        nombre.personnels.iat.ifts)

#'
#'[Codes IFTS retenus](Bases/codes.ifts.csv)    
#'[Lien vers la base de données](Bases/personnels.iat.ifts.csv)    
#'
#'### Contrôle sur les IFTS pour catégories B et contractuels

#IFTS et IB >= 380 (IM >= 350)

 lignes.ifts.anormales <- na.omit(Bulletins.paie.Lignes.paie[Indice < 350  & Code %in% codes.ifts,
                                                                  c(clé.fusion,
                                                                    "Statut",
                                                                    étiquette.code,
                                                                     étiquette.libellé,
                                                                     "Indice",
                                                                     étiquette.montant)])

nombre.lignes.ifts.anormales <- nrow(lignes.ifts.anormales)

# IFTS et non tit

ifts.et.contractuel <- Bulletins.paie.Lignes.paie[Code %in% codes.ifts 
                                                  & ! Statut %in% c("TITULAIRE", "STAGIAIRE"),
                                                      c(étiquette.matricule,
                                                        "Statut",
                                                        étiquette.code,
                                                        étiquette.libellé,
                                                        "Indice",
                                                        étiquette.montant)]

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

HS.sup.25.matricules.mois <- unique(Bulletins.paie.Lignes.paie[Heures.Sup. >= 25, 
                                    c(étiquette.matricule,
                                      étiquette.année,
                                      "Mois",
                                      "Statut",
                                      "Heures.Sup.")])

# Sont repérées comme heures supplémentaires ou complémentaires les heures dont le libellé obéissent à
# l'expression régulière expression.rég.heures.sup donnée par le fichier prologue.R
 

HS.sup.25.montants <- Bulletins.paie.Lignes.paie[Heures.Sup. >= 25
                                                 & ! Code %in% Codes.paiement.traitement
                                                 & ! is.na(Libellé)
                                                 & ! is.na(Code)
                                                 & ! grepl(".*SMIC.*",
                                                         Libellé, ignore.case = TRUE)
                                                 & as.numeric(substr(Code,1,2)) < 50
                                                 & grepl(expression.rég.heures.sup,
                                                         Libellé, ignore.case = TRUE),
                                                 c(étiquette.matricule,
                                                   étiquette.année,
                                                   "Mois",
                                                   "Libellé",
                                                   étiquette.code,
                                                   "Heures",
                                                   "Base",
                                                   "Taux",
                                                   "Montant")]

HS.sup.25 <- merge(HS.sup.25.matricules.mois, HS.sup.25.montants, all = TRUE)

T <- aggregate(Bulletins.paie.Lignes.paie$montant.traitement.indiciaire,
               by = list(Matricule, Année, Mois),
               FUN = sum)
names(T) <- c(étiquette.matricule, "Année", "Mois", "montant.traitement.indiciaire")

HS.sup.25 <- merge(HS.sup.25, T)
                   
HS.sup.25 <- merge(HS.sup.25, Analyse.rémunérations[ , c(étiquette.matricule, étiquette.année, "traitement.indiciaire")])

L <-  length(names(HS.sup.25))

names(HS.sup.25)[L]   <- "Traitement indiciaire annuel"
names(HS.sup.25)[L-1]	<- "Traitement indiciaire mensuel" 

rm(HS.sup.25.matricules.mois, HS.sup.25.montants, L)

nombre.Lignes.paie.HS.sup.25 <- nrow(HS.sup.25)

# with(Base2,
#      ihts.anormales <<- Base2[! Code.catégorie %in% c("B", "C") & substr(Code,1,2) %in% c("19") & ! grepl(" ENS", Libellé), c(étiquette.matricule, étiquette.code, étiquette.libellé, étiquette.montant, "Code.catégorie")]
# )

       ihts.anormales <- data.frame(NULL)
nombre.ihts.anormales <- nrow(ihts.anormales)
#'
 
Tableau(c("Nombre de lignes HS en excès", "Nombre de lignes IHTS anormales"), nombre.Lignes.paie.HS.sup.25, nombre.ihts.anormales)

#'
#'[Lien vers la base de données Heures suplémentaires en excès : matricules](Bases/HS.sup.25.csv)  
#'[Lien vers la base de données IHTS anormales](Bases/ihts.anormales.csv)  
#'       
#'**Nota:**  
#'HS en excès : au-delà de 25 heures par mois  
#'IHTS anormales : non attribuées à des fonctionnaires de catégorie B ou C.  
#'
#'## 5.6 Contrôle sur les indemnités des élus
#'     

matricules.à.identifier <- unique(Bulletins.paie.Lignes.paie[ , 
                                                               c(étiquette.année,
                                                                 "Emploi",
                                                                 "Nom",
                                                                 étiquette.matricule)])

if (fichier.personnels.existe) {
  matricules.à.identifier <- merge(matricules.à.identifier, base.personnels.catégorie, by = clé.fusion, all=TRUE)
} else {
  Catégorie <- character(length = nrow(matricules.à.identifier))
  matricules.à.identifier <- cbind(matricules.à.identifier, Catégorie)
}


matricules.à.identifier <- matricules.à.identifier[order(matricules.à.identifier$Matricule,
                                                         matricules.à.identifier$Année),]


      rémunérations.élu <- Analyse.rémunérations[ Analyse.rémunérations$indemnités.élu > 0,
                                                       c(clé.fusion,
                                                         "Année",
                                                         "indemnités.élu",
                                                         "autres.rémunérations",
                                                         "total.rémunérations") ]


      rémunérations.élu <- mutate(rémunérations.élu,
                                  total.rémunérations = indemnités.élu +  total.rémunérations)

if (!fusionner.nom.prénom)
       rémunérations.élu <- merge(unique(matricules.à.identifier[c("Nom",  étiquette.matricule)]),
                                  rémunérations.élu,
                                  by = étiquette.matricule,
                                  all.y = TRUE,
                                  all.x = FALSE)

names(rémunérations.élu) <- c(union(clé.fusion, "Nom"), 
                              "Année",
                              "Indemnités d'élu (euros)",
                              "Autres rémunérations (euros)",
                              "Total (euros)")

       rémunérations.élu <- na.omit(rémunérations.élu)

#'   
if (nrow(rémunérations.élu) >0)
    kable(rémunérations.élu, row.names = FALSE)

#'

   Sauv.base(chemin.dossier.bases, 
             "matricules.à.identifier",
             fichier.personnels)

#'[Lien vers la base de données Rémunérations des élus](Bases/rémunérations.élu.csv)  
#'     

#'# Annexe

#'## Liens complémentaires
#'     
#'[Lien vers la base de données fusionnées des bulletins et lignes de paie](Bases/Bulletins.paie.Lignes.paie.csv)
#'      
#'[Lien vers le tableau des codes de paiement](Bases/nom.fichier.codes.paiement.csv)
#'
#'[Lien vers le fichier des personnels](Bases/Catégories des personnels.csv)
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

# ------------------------------------------------------------------------------------------------------------------
#  Sauvegardes : enlever les commentaires en mode opérationnel
##

length(unique(Bulletins.paie$Service))

if (sauvegarder.bases) 
  sauv.bases(chemin.dossier.bases,
    "Analyse.rémunérations",
    "Bulletins.paie.nir.total.hors.élus",
    "Bulletins.paie.nir.fonctionnaires",
    "Bulletins.paie.Lignes.paie", 
    "Bulletins.paie",
    "NBI.aux.non.titulaires",
    "RI.et.vacations",
    "HS.sup.25",
    "ihts.anormales",
    "personnels.prime.informatique",
    "lignes.contractuels.et.vacations",
    "lignes.fonctionnaires.et.vacations",
    "lignes.ifts.anormales",
    "personnels.iat.ifts",
    "codes.ifts",
    "ifts.et.contractuel",
    "traitement.et.vacations",
    "matricules.contractuels.et.vacations",
    "matricules.fonctionnaires.et.vacations",
    "rémunérations.élu")




