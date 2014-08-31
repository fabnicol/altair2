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
#  on vérifie que chaque code de paie est associé, dans le fichier codes.paiement (par défaut, codes.csv),
#  que à chaque code donné on a associé un et un seul type de rémunération ("INDEMNITAIRE", "TRAITEMENT", etc.)           

if (file.exists(chemin(codes.paiement)))
{
  Codes.paiement <- read.csv.skip(codes.paiement)
  
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
    
    stop("Vérifier le fichier " %+% codes.paiement)
    
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
       names(Bulletins.paie)[! Bulletins.paie.contiennent.colonnes.requises])
  
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
  #'Utiliser les codes : TRAITEMENT, INDEMNITAIRE.OU.CONTRACTUEL, ELU, AUTRES  
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
    et placé le fichier sous le répertoire racine avec le nom " %+% codes.paiement, call.=FALSE)
}

# suppression des colonnes Nom Prénom redondantes

message("Nettoyage des bases.")

sélectionner.clé("Bulletins.paie", "Lignes.paie")

# Technique : les espaces de noms sont pollués par la sélection des clés, il faut les nettoyer

# unname(Bulletins.paie$Nom, force=TRUE)
# unname(Bulletins.paie$Prénom, force=TRUE)
# unname(Lignes.paie$Nom, force=TRUE)
# unname(Lignes.paie$Prénom, force=TRUE)

# # Extraction de vecteurs représentant les codes de paiement par type de code (indemnitaire, traitement, vacations...)
# 
# if (exists("Codes.paiement"))
# {
#   Codes.paiement.indemnitaire <- unique(Codes.paiement[Codes.paiement$Type.rémunération == "INDEMNITAIRE",étiquette.code])
#   Codes.paiement.traitement   <- unique(Codes.paiement[Codes.paiement$Type.rémunération == "TRAITEMENT",étiquette.code])
#   Codes.paiement.élu          <- unique(Codes.paiement[Codes.paiement$Type.rémunération == "ELU",étiquette.code])
#   Codes.paiement.vacations    <- unique(Codes.paiement[Codes.paiement$Type.rémunération == "VACATIONS",étiquette.code])
#   Codes.paiement.autres       <- unique(Codes.paiement[Codes.paiement$Type.rémunération == "AUTRES",étiquette.code])
#   
#   message("Extraction des codes par type de code.")
#   
# } else
#   stop("Charger le fichier de codes de paiement.")
# 
# # Pour assurer une fusion correcte des bulletins et lignes de paie, il importe que les colonnes communes aux deux fichiers soient 
# # exactement celles utilisées pour la clé d'appariement d'une part, et le tri sous chaque clé d'autre part, autrement dit :
# # la clé (Matricule ou (Nom, Prénom) selon le cas) + Année + Mois
# 
# if (! fusionner.nom.prénom) 
#   Bulletins.paie <- subset(Bulletins.paie,
#                            select = setdiff(names(Bulletins.paie), c("Nom", "Prénom")))
# 
# if (! setequal(intersect(names(Lignes.paie), names(Bulletins.paie)), union(c("Mois", "Année"), clé.fusion)))
# {
#   if (fusionner.nom.prénom) {
#     
#     stop("L'appariement ne peut se faire par les clés Nom, Prénom et Mois")
#     
#   } else {
#     
#     stop("L'appariement ne peut se faire par les clés Matricule et Mois")
#   }
#   
# } else {
#   
#   message("Pas de redondance des colonnes des bulletins et lignes de paie : la fusion peut être réalisée.")
# }

# Alternative en cas de difficulté :
#
# Bulletins.paie.Lignes.paie <- do.call(rbind, lapply(début.période.sous.revue:fin.période.sous.revue,
#                                                     function(x) 
#                                                        merge(Bulletins.paie[Bulletins.paie$Année == x, ],
#                                                              Lignes.paie[Lignes.paie$Année == x, ], 
#                                                              by=c(étiquette.matricule, "Mois"))))

# Lorsque les bases sont déjà chargées, ont peu désactiver le rechargement par charger.bases <- FALSE

# if (charger.bases)
# {
#   anavar <- ddply(Bulletins.paie, 
#                   c(étiquette.matricule, étiquette.année),
#                   summarise,
#                   # partie Analyse des variations par exercice #
#                   Montant.net = sum(Net.à.Payer),
#                   Montant.brut = sum(Brut),
#                   Statut = Statut[length(Net.à.Payer)],
#                   mois.entrée = ifelse((minimum <- min(Mois)) != Inf,
#                                        minimum,
#                                        0),
#                   mois.sortie = ifelse((maximum <- max(Mois)) != -Inf,
#                                        maximum,
#                                        0),
#                   nb.jours = calcul.nb.jours.mois(mois.entrée[1], mois.sortie[1], Année[1]),
#                   nb.mois  = mois.sortie[1] - mois.entrée[1] + 1)
#   
#   Bulletins.paie <- merge (Bulletins.paie, anavar)
#   
#   Bulletins.paie.Lignes.paie <- merge(Bulletins.paie,
#                                       Lignes.paie,
#                                       by = c(clé.fusion, "Année", "Mois"))
#   
#   if ( ! exists("Codes.paiement.indemnitaire"))  stop("Pas de fichier des Types de codes [INDEMNITAIRE]")
#   if ( ! exists("Codes.paiement.traitement"))    stop("Pas de fichier des Types de codes [TRAITEMENT]")
#   if ( ! exists("Codes.paiement.élu"))           stop("Pas de fichier des Types de codes [ELU]")
#   if ( ! exists("Codes.paiement.autres"))        stop("Pas de fichier des Types de codes [AUTRES]")
#   
#   Bulletins.paie.Lignes.paie <- mutate(Bulletins.paie.Lignes.paie,
#                                        montant.traitement.indiciaire 
#                                        = Montant*(Code %in% Codes.paiement.traitement),
#                                        montant.rémunération.principale.contractuel =0, # rajouter critère non tit
#                                        #                                         = Montant*(Code %in% codes.paiement[codes.paiement$Type.rémunération 
#                                        #                                                                            == "PRINCIPAL.CONTRACTUEL",étiquette.code]),
#                                        montant.primes 
#                                        = Montant*(Code %in% Codes.paiement.indemnitaire),
#                                        montant.autres.rémunérations 
#                                        = Montant*(Code %in% Codes.paiement.autres),
#                                        montant.indemnité.élu 
#                                        = Montant*(Code %in% Codes.paiement.élu),
#                                        
#                                        ### EQTP  ###
#                                        
#                                        quotité
#                                        =  Temps.de.travail / 100
#                                        * ifelse(corriger.quotité,
#                                                 ifelse(is.na(Taux), 1, Taux),
#                                                 1)
#                                        * (montant.rémunération.principale.contractuel > 0 
#                                           | 
#                                             montant.traitement.indiciaire > 0)
#                                        * nb.mois / 12) 
#   
#   Analyse.rémunérations <- ddply(Bulletins.paie.Lignes.paie,
#                                  c(clé.fusion, étiquette.année),
#                                  summarise,
#                                  
#                                  Nir = Nir[1],
#                                  Montant.net = Montant.net[1],
#                                  Statut = Statut[1],   
#                                  mois.entrée = mois.entrée[1],
#                                  nb.jours = nb.jours[1],
#                                  nb.mois = nb.mois[1],
#                                  mois.sortie = mois.sortie[1],
#                                  Emploi = Emploi[length(Net.à.Payer)],
#                                  traitement.indiciaire = sum(montant.traitement.indiciaire),
#                                  rémunération.principale.contractuel = sum(montant.rémunération.principale.contractuel),
#                                  rémunération.indemnitaire = sum(montant.primes),
#                                  indemnités.élu = sum(montant.indemnité.élu),
#                                  autres.rémunérations = sum(montant.autres.rémunérations),
#                                  total.rémunérations =  traitement.indiciaire                       # le premier et deuxième terme sont exclusifs
#                                  + rémunération.principale.contractuel
#                                  + rémunération.indemnitaire,
#                                  
#                                  quotité = ifelse(length(quotité[quotité > 0]) > 0, 
#                                                   quotité[quotité > 0][1], 
#                                                   0),
#                                  montant.net.eqtp = ifelse(quotité == 0, 0, Montant.net / quotité),
#                                  part.rémunération.indemnitaire = ifelse( (s <- traitement.indiciaire + rémunération.principale.contractuel + rémunération.indemnitaire) == 0, 
#                                                                           0,
#                                                                           (rémunération.indemnitaire + rémunération.principale.contractuel )/ 
#                                                                             s * 100))
#   
#   Analyse.rémunérations <- Analyse.rémunérations[!is.na(Analyse.rémunérations$total.rémunérations), ]
#   
#   
#   if (length (Analyse.rémunérations$quotité[Analyse.rémunérations$quotité > 1]) > 0 & comportement.strict & corriger.quotité) stop("Détection de quotités > 1", call. = FALSE)
#   
#   
#   Analyse.variations.par.exercice <- Analyse.rémunérations[ , c(clé.fusion, étiquette.année,
#                                                                 "montant.net.eqtp", 
#                                                                 "Statut",
#                                                                 "Montant.net",
#                                                                 "nb.jours",
#                                                                 "nb.mois",
#                                                                 "quotité")]
#   
#   Analyse.variations.synthèse <- ddply(Analyse.variations.par.exercice,
#                                        clé.fusion,
#                                        summarise,
#                                        
#                                        Nexercices = length(Année),
#                                        # quotité.exercice.début = quotité[1],
#                                        # quotité.exercice.sortie = quotité[Nexercices],
#                                        nb.mois.exercice.début  = nb.mois[1],
#                                        nb.mois.exercice.sortie = nb.mois[Nexercices],
#                                        total.jours = sum(nb.jours),
#                                        total.mois  = sum(nb.mois),
#                                        montant.net.eqtp.début  = montant.net.eqtp[1],
#                                        montant.net.eqtp.sortie = montant.net.eqtp[Nexercices],
#                                        moyenne.rémunération.annuelle.sur.période = mean(montant.net.eqtp[montant.net.eqtp > 0], na.rm=TRUE),
#                                        variation.rémunération = ifelse(Nexercices > 1 & montant.net.eqtp.début > 0,
#                                                                        montant.net.eqtp.sortie / montant.net.eqtp.début -1,
#                                                                        0),
#                                        variation.moyenne.rémunération = 
#                                          ifelse(total.mois == 0, 
#                                                 0,
#                                                 ( ( 1 + variation.rémunération / 100 ) ^ (12 / total.mois) - 1) * 100),
#                                        plus.2.ans  = (total.mois  >= 2*12),
#                                        moins.2.ans = (total.mois < 2*12),
#                                        moins.1.an  = (total.mois < 12),
#                                        moins.six.mois = (total.mois < 6),
#                                        statut = Statut[1])
#   
#   
#   
#   Analyse.variations.par.exercice <- na.omit(Analyse.variations.par.exercice[ ( Analyse.variations.par.exercice$Année > début.période.sous.revue 
#                                                                                 & Analyse.variations.par.exercice$Année < fin.période.sous.revue) | 
#                                                                                 Analyse.variations.par.exercice$nb.mois > seuil.troncature, ])
#   
#   temp <- Analyse.variations.synthèse[Analyse.variations.synthèse$plus.2.ans, clé.fusion] 
#   
#   trouver.ligne <- function(x, y) anyDuplicated(rbind(x, y)) > 1
#   
#   if (fusionner.nom.prénom) {
#     
#     Analyse.variations.par.exercice <- mutate(Analyse.variations.par.exercice,
#                                               plus.2.ans = trouver.ligne(c(Nom, Prénom), temp))
#   } else {
#     
#     Analyse.variations.par.exercice <- mutate(Analyse.variations.par.exercice,
#                                               plus.2.ans = Matricule %in% temp)
#   }
#   
#   rm(temp)
#   
#   Bulletins.paie.nir.total.hors.élus <- merge(Analyse.rémunérations[  Analyse.rémunérations$Année == fin.période.sous.revue 
#                                                                       & Analyse.rémunérations$indemnités.élu == 0,
#                                                                       c(clé.fusion, champ.nir) ],
#                                               Bulletins.paie[  Bulletins.paie$Année == fin.période.sous.revue
#                                                                & Bulletins.paie$Mois == 12,
#                                                                c(clé.fusion, champ.nir)], 
#                                               by = clé.fusion, all = FALSE)
#   
#   Bulletins.paie.nir.fonctionnaires  <- unique(Bulletins.paie[  Bulletins.paie$Année == fin.période.sous.revue 
#                                                                 & Bulletins.paie$Mois  == 12
#                                                                 & Bulletins.paie$Statut %in% c("TITULAIRE", "STAGIAIRE"),
#                                                                 c(clé.fusion, champ.nir)])
#   
#   Bulletins.paie.nir.total.hors.élus <- Bulletins.paie.nir.total.hors.élus[-3]
#   names(Bulletins.paie.nir.total.hors.élus) <- c(clé.fusion, champ.nir)
#   
#   # Age au 31 décembre de l'exercice dernier.exerciceal de la période sous revue
#   # ne pas oublier [ ,...] ici:
#   
#   extraire.nir <- function(Base) fin.période.sous.revue - (as.numeric(substr(as.character(
#     format(Base[ , champ.nir], scientific = FALSE)),
#     2, 3))
#     + 1900)
#   
#   années.fonctionnaires   <- extraire.nir(Bulletins.paie.nir.fonctionnaires)
#   
#   années.total.hors.élus  <- extraire.nir(Bulletins.paie.nir.total.hors.élus)
# }
