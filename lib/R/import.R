# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, annees 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
#
# Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
# produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
# en vigueur à compter de l'annee 2008.
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

#library(zeallot)

#' Assignation dans l'environnement d'appel
#' @param x Chaîne de caractères entre guillemets du nom de la variable assignée (gauche), comme pour \code{assign}
#' @param y Valeur. Variable assignante (droite)
#' @examples "x" %a% median(1:100); cat(x)
#' @export
#'
`%a%` <- function(x, y) assign(x, y, inherits = TRUE, envir = .GlobalEnv)


convertir.accents <- function(V) {
  for (x in V) {
    for (y in names(x)) setnames(x, y, gsub("à", "a", gsub("é", "e", gsub("è", "e", y))))
  }
}


importer.bases.via.xhl2csv <- function(base, fichiers, colClasses, select) {

  res <- try(Read.csv(base,
                      fichiers,
                      colClasses = colClasses,
                      select = select,
                      separateur.liste = separateur.liste.entree,
                      separateur.decimal = separateur.decimal.entree),
             silent = FALSE)

  if (inherits(res, 'try-error'))
    stop("Problème de lecture de la base de la table bulletins-lignes de Paie")


  message("Chargement direct des bulletins et lignes de paie")
}

quotite.temps.partiel <- function(temps.de.travail) {

  if (x == 90) return(0.91429)  # 32/35
  if (x == 80) return(0.85714)  # 6/7
  return(x/100)

}

verif.temps.complet <- function() {

  # dans certains cas on a presque jamais la variable Heures renseignée... sauf pour quelques temps partiels
  # des bases (peu fréquentes) sont attestées dans lesquelles on a Heures à zéro pour tous les titulaires à temps complet !
  # il s'agit d'un test pour vérifier s'il faut redresser la colonne Heures en conséquence
  # dans ce cas valeur est à 0
  # par ailleurs on recalcule la base de référence mensuelle du temps complet qui n'est pas nécessairement 151.67 empiriquement à partir du mode de la distribution
  # du nombres d'heures à temps complet
  # on n'utilisera plus loin ce résultat que si valeur est à 0 (c'est rare)
  # il s'agit d'une vérification globale et non par sous-groupe de grade etc.

  B <- Bulletins.paie[Temps.de.travail == 100, Heures]

  if (length(B) == 0) {
    cat("Aucune quotité à temps complet. Problème de qualité probable de la base de paye.  \n")
    cat("En l'absence de quotité positive, la liquidation des payes n'est pas vérifiable.  \n")
    cat("Pour permettre la poursuite du traitement, les payes sont supposées toutes à temps complet.   \n")
    cat("Attention ce redressement peut entraîner des erreurs d'analyse dans la suite du rapport.   \n")
    Bulletins.paie[ , Temps.de.travail := 100]
    B <- Bulletins.paie[Temps.de.travail == 100, Heures]
  }

  h <- hist(B, nclass = 20000, plot = FALSE)
  max.h <- which.max(h$counts)

  if (max.h > 1) {

    delta <- (h$mids[max.h + 1] - h$mids[max.h - 1])/2

    if (is.na(delta)) delta <- 1 #Présomption

    nb.heures.temps.complet <- floor(h$mids[max.h])

  } else {

    return(c(1, reference.nb.heures.par.mois)) # présomption
  }

  valeur <- (abs(nb.heures.temps.complet - reference.nb.heures.par.mois) < 1 + delta)  # booléen, 0 ou 1

  if (is.na(valeur))  {
    valeur <- 1 # présomption
  }

  return(c(valeur, nb.heures.temps.complet))
}

#' Calcul des quotites
#' @param Bulletins.paie Fichier des bulletins de paye
#' @param Paie Base des lignes de paye
#' @export

quotites <- function() {

  Bulletins.paie <- merge(Bulletins.paie,
                          Paie[ , .(Filtre_actif = Filtre_actif[1]),
                                by = c("Matricule", "Annee", "Mois")],
                          all.x = TRUE,
                          all.y = FALSE)

  Bulletins.paie[ , pop_calcul_médiane := length(Heures[Temps.de.travail == 100
                                                        & !is.na(Heures)
                                                        & Heures > minimum.positif]),
                  by = .(Emploi)]

  # Pour les quotites seules les periodes actives sont prises en compte

  Bulletins.paie[pop_calcul_médiane > population_minimale_calcul_médiane
                 & Filtre_actif == TRUE,
                   MHeures :=  median(Heures[Temps.de.travail == 100
                                             & Filtre_actif == TRUE
                                             & Heures > minimum.positif], na.rm = TRUE),
                     by = .(Emploi)]

  # L'écrêtement des quotites est une contrainte statistiquement discutable qui permet de "stresser" le modèle
  # Par défaut les quotites sont écrêtées pour pouvoir par la suite raisonner en définissant le temps plein comme quotite == 1

  if (écreter.quotites) {

    Bulletins.paie[ , quotite   :=  ifelse(MHeures < minimum.positif, NA, ifelse(Heures > MHeures, 1, round(Heures/MHeures, digits = 2)))]

  } else {

    Bulletins.paie[ , quotite   :=  ifelse(MHeures < minimum.positif, NA, round(Heures/MHeures, digits = 2))]
  }

  Bulletins.paie[Statut == "ELU", `:=`(MHeures = 1,
                                       quotite = 1)]

  message("Quotités calculees")

  return(Bulletins.paie)
}

#' Calculer la rémunération nette et brute en EQTP
#' @param DT  data.table (Bulletins.paie)
#' @export

rémunérations_eqtp <- function(DT) {

  # Pour éviter les problèmes de qualité résultant de la divergence à l'infini d'une division par un dénominateur tendant vers 0
  # on impose un minimum de quotité pour le calcul des EQTP. A défaut, on utilise la rémunération constatée.
  # ceci pour éviter les difficultés liées aux données brutes
  # ne concerne qu'un nombre réduit de cas, sauf si les quotités sont souvent abusivement nulles

  DT[  ,   Montant.net.eqtp  := Net.a.Payer]
  DT[quotite > minimum.quotite,   Montant.net.eqtp  := Net.a.Payer / quotite]

  DT[  ,   Montant.brut.eqtp  := Brut]
  DT[quotite > minimum.quotite,   Montant.brut.eqtp  := Brut / quotite]

  DT[ ,   `:=`(Statut.sortie   = Statut[length(Montant.net.eqtp)],
               nb.jours        = sum(calcul.nb.jours.mois(Mois, Annee[1]), na.rm = TRUE),
               nb.mois         = length(Mois),
               cumHeures       = sum(Heures, na.rm = TRUE),
               quotite.moyenne = sum(quotite, na.rm = TRUE) / 12,
               quotite.moyenne.orig = sum(Temps.de.travail, na.rm = TRUE) / 1200),

                      keyby = .(Matricule, Annee)]

  # Analyser les différences entre quotite.moyenne et quotite.moyenne.orig permettrait de localiser les cas
  # dans lesquels la quotité "théorique" (Temps.de.travail) mensuelle est fausse ou représentatif d'une réalité
  # contractuelle/statutaire sans lien avec le service fait mensuel.

  # Indicatrice pour la rémunération moyenne des personnes en place :
  # quotite égale pendant deux annees successives contigues, permanence sur 12 mois.
  # nous prenons les moyennes des quotites non NA.

  DT[ , indicatrice.quotite.pp := (Matricule[R] == Matricule
                                               & Annee[R]   == Annee - 1
                                               & abs(quotite.moyenne[R] - quotite.moyenne) < tolerance.variation.quotite
                                               & nb.mois[R] == nb.mois
                                               & nb.mois    == 12)]

  DT[ ,   `:=`(Montant.brut.annuel      = sum(Brut, na.rm = TRUE),
                           Montant.brut.annuel.eqtp = 365 / nb.jours * sum(Montant.brut.eqtp , na.rm = TRUE),
                           Montant.net.annuel.eqtp  = 365 / nb.jours * sum(Montant.net.eqtp, na.rm = TRUE),
                           Montant.net.annuel       = sum(Net.a.Payer, na.rm = TRUE),
                           annee_entiere            = nb.jours >= 365,
                           indicatrice.quotite.pp   = indicatrice.quotite.pp[1]),

                  keyby = .(Matricule, Annee)]

  message("Rémunérations EQTP calculees")

  # DT est modifié par référence

  return(DT)
}

#' Importer la base externe des correspondance ente matricules, categories et grades
#' @export

importer_matricules <- function() {

  if (fichier.personnels.existe) {

    base.personnels.categorie <- data.table::fread(chemin("matricules.csv"),
                                                   sep = separateur.liste.entree,
                                                   header = TRUE,
                                                   colClasses = c("numeric", "character", "character",
                                                                  "character", "character", "character", "character"),
                                                   encoding = "UTF-8",
                                                   showProgress = FALSE)

    message("Chargement du fichier des categories statutaires des personnels.")
    if (!is.null(base.personnels.categorie))
      message("Importé.")
    else {
      message("Impossible d'importer les categories.")
      stop(" ")
    }
  } else {
    base.personnels.categorie <- NULL
  }

  base.personnels.categorie
}

# Reprise du code C++

identifier.personnels <- function() {

  # maire
  MAIRE     <-  "maire"
  # président
  PRESIDENT <- "pr..?sident"
  #élu
  ELUS      <-  "[eé]lus?"
  # adjoint au maire
  ADJOINT_MAIRE     <-  ".*adj.*\\bmaire\\b"
  # vice-président
  VICE_PRESIDENT    <- ".*vi.*\\bpr..?sident\\b"
  # conseiller municipal
  CONSEILLER_MUNIC  <-"cons.*\\bmuni"
  # conseiller communautaire
  CONSEILLER_COMMUN <- "cons.*\\bcomm"
  # conseiller délégué
  CONSEILLER_DELEGUE <- "(?:cons.*\\bd\\S*|d..?..?gu)"
  OU        <-  "|"
  SOIT      <-  "(?:"
  FIN_SOIT  <-  ")"
  ETC       <-  ".*"
  AUCUN_MOT <- "\\W*"
  DEBUT     <- "^"

  EXPRESSION_REG_ELUS <- DEBUT %+%
                         SOIT %+%
                            MAIRE %+%
                         OU %+% PRESIDENT %+%
                         OU %+% ELUS %+%
                         OU %+% ADJOINT_MAIRE %+%
                         OU %+% VICE_PRESIDENT %+%
                         OU %+% CONSEILLER_MUNIC %+%
                         OU %+% CONSEILLER_COMMUN %+%
                         OU %+% CONSEILLER_DELEGUE %+%
                         FIN_SOIT %+%
                         ETC

  Bulletins.paie[grepl(pattern = EXPRESSION_REG_ELUS, Emploi, perl = TRUE, ignore.case = TRUE), `:=`(Statut = "ELU",
                                                                                                     Categorie = NA)]


  Paie[grepl(pattern = EXPRESSION_REG_ELUS, Emploi, perl = TRUE, ignore.case = TRUE), `:=`(Statut = "ELU",
                                                                                                     Categorie = NA)]
  EXPRESSION_REG_VACATIONS <- ".*\\bvacat.*|.*\\bvac\\.?\\b.*"
  EXPRESSION_REG_ASSMAT    <- ".*\\bass.*\\bmat.*"

  Bulletins.paie[grepl(pattern = EXPRESSION_REG_VACATIONS, Emploi, perl = TRUE, ignore.case = TRUE), Grade := "V"]

  Bulletins.paie[grepl(pattern = EXPRESSION_REG_ASSMAT, Emploi, perl = TRUE, ignore.case = TRUE), Grade := "A"]
}



#' Importer la base des logements de fonction (encodage UTF-8)
#' @export
#'
importer_base_logements <- function() {
  if (logements.existe) {
    base.logements <- data.table::fread(chemin("logements.csv"),
                                    sep = separateur.liste.entree,
                                    header = TRUE,
                                    colClasses = c("character", "integer", "integer", "character"),
                                    encoding = "UTF-8",
                                    showProgress = FALSE)

    message("Chargement du fichier des concessions de logement des personnels.")

    if (!is.null(base.logements))
    message("Importé.")
    else {
    message("Impossible d'importer les concessions de logement.")
    stop(" ")
    }
  } else {
    base.logements <- NULL
  }

  return(base.logements)
}

#' Importer le base externe IFSE (encodage UTF-8)
#' @export
#'

importer_base_ifse <- function() {

  if (plafonds.ifse.existe) {

    base.ifse <- data.table::fread(chemin("plafonds_ifse.csv"),
                                   sep = separateur.liste.entree,
                                   header = TRUE,
                                   colClasses = c("character", "character", "character", "numeric"),  # Grade, Groupe, Logement, Plafond
                                   encoding = "UTF-8",
                                   showProgress = FALSE)

    message("Chargement du fichier des plafonds d'IFSE.")

    if (!is.null(base.ifse)) {

      message("Importé.")

    } else {

      message("Impossible d'importer les plafonds d'IFSE.")
      stop(" ")
    }

  } else {

    base.ifse <- NULL
  }

  return(base.ifse)
}

Extraire.annees <- function() {

  if (extraire.annees) {

    "Paie" %a% Paie[Annee >= debut.periode.sous.revue & Annee <= fin.periode.sous.revue]
    "Bulletins.paie" %a% Bulletins.paie[Annee >= debut.periode.sous.revue & Annee <= fin.periode.sous.revue]

  } else {

    "debut.periode.sous.revue" %a% min(Bulletins.paie[ , Annee], na.rm = TRUE)
    "fin.periode.sous.revue" %a% max(Bulletins.paie[ , Annee], na.rm = TRUE)
  }
}

#' Eliminer les doublons en mémoire pour les calculs
#' @return e   Environnement contenant les champs \code{Ndup}, nombre de bulletins doublonnés 
#'             et \code{Ndup2} (nombre de lignes doublonnées)
#' @export
#'
Eliminer.duplications <- function() {
 
    vect.duplicated <- duplicated(Bulletins.paie, by = c("Nom", "Prenom", "Matricule", "Annee", 
                                                        "Mois", "Brut", "Net.a.Payer"))
                                                        
    e = new.env()
    e$Ndup  <- 0
    e$Ndup2 <- 0
    Paie.dup <- NULL
    Paie.uniq <- NULL
    Bulletins.paie.dup <- NULL
    Bulletins.paie.uniq <- NULL
    
    e$Ndup <- length(vect.duplicated)
    
    if (e$Ndup == 0) return(e)
    
    cols <- c("Nom", "Prenom", "Matricule", "Annee", 
                "Mois", "Brut", "Net.a.Payer", "Code", 
                "Type", "Libelle", "Montant")

    if ("Ordre" %in% names(Paie))      cols <- c(cols, "Ordre")
    if ("CodeCaisse" %in% names(Paie)) cols <- c(cols, "CodeCaisse")

    vect.duplicated.cols <- duplicated(Paie, by = cols)
    e$Ndup2 <- length(vect.duplicated.cols)

    "Bulletins.paie" %a%  Bulletins.paie[! vect.duplicated]
    
    if (e$Ndup2) {
    
        "Paie" %a% Paie[! vect.duplicated.cols] 
    } 
    
    e
}

#' Eliminer les doublons dans les tables exportées sous Bases/Remunerations
#' @return e   Environnement contenant les champs \code{Ndup}, nombre de bulletins doublonnés 
#'             et \code{Ndup2} (nombre de lignes doublonnées)
#' @note Les bases de doublons sont Paie.dup et Bulletins.paie.dup, les bases filtrées sont
#'       Bulletins.paie.uniq et Paie.uniq 
#' @export
#'

Exporter.tables.sans.doublons <- function() {
        
    # Il faut réimporter à nouveau
    
    message("Réimportation des bases")
    importer(normaliser.colonnes = FALSE)
    
    # dans quelques rares cas les bases peuvent diverger à cause des fusions
    # intervenues dans importer(), il faut donc recalculser les vecteurs de duplication
    
    vect.duplicated <- duplicated(Bulletins.paie, by = c("Nom", "Prénom", "Matricule", "Année", 
                                                         "Mois", "Brut", "Net.à.Payer"))

    e = new.env()
    e$Ndup  <- 0
    e$Ndup2 <- 0
    Paie.dup <- NULL
    Paie.uniq <- NULL
    Bulletins.paie.dup <- NULL
    Bulletins.paie.uniq <- NULL
    
    e$Ndup <- length(vect.duplicated)
    
    if (e$Ndup == 0) return(e)
    
    cols <- c("Nom", "Prénom", "Matricule", "Année", 
              "Mois", "Brut", "Net.à.Payer", "Code", 
              "Type", "Libellé", "Montant")
                
    if ("Ordre" %in% names(Paie))      cols <- c(cols, "Ordre")
    if ("CodeCaisse" %in% names(Paie)) cols <- c(cols, "CodeCaisse")
       
    vect.duplicated.cols <- duplicated(Paie, by = cols)
    
    e$Ndup2 <- length(vect.duplicated.cols)
    
    Bulletins.paie.dup <-  Bulletins.paie[vect.duplicated]
    Bulletins.paie.uniq <- Bulletins.paie[! vect.duplicated]

    if (e$Ndup2) {
        Paie.dup  <- Paie[vect.duplicated.cols]
        Paie.uniq <- Paie[! vect.duplicated.cols] 
        sauv.bases("Remunerations",
                environment(),
                "Paie.dup",
                "Paie.uniq")
    }

    sauv.bases("Remunerations",
                environment(),
                "Bulletins.paie.dup",
                "Bulletins.paie.uniq")
        
    e
}

#' Redresser les heures de travail (variable \code{Heures}) en tenant compte des traitements.
#' @export

Redresser.heures <- function() {

    "nredressements" %a% 0

    if (redresser.heures) {

    # On ne peut pas inférer sur quotite Trav (Temps.de.travail) de manière générale
	  # car il s'agit d'une quotité de temps de travail théorique, ou contractuelle, et non effectivement constatée en service fait.
    # Mais on peut exclure les cas dans lesquels les heures sont non renseignées, alors que soit la quotité théorique soit le traitement l'est,
    # pour un paiement non nul et un indice connu
    # (cette dernière condition afin d'éviter le redresser à tort des vacations à la tâche ou forfaitaire)
	  # On part donc du principe que le service fait est régulier.
	  # contrairement à l'approche juridicationnelle qui part du nombre d'heures comme élément de liquidation

      setnames(Paie, "Heures", "Heures.orig")
      setnames(Bulletins.paie, "Heures", "Heures.orig")
      Paie[ , Heures := Heures.orig]
      Bulletins.paie[ , Heures := Heures.orig]

      if (test.temps.complet) {

		  Bulletins.paie[(Heures == 0 | is.na(Heures))
						 & Indice != ""
						 & !is.na(Indice)
						 & Statut != "ELU"
						 & Grade  != "V"
						 & Grade  != "A"
						 & Temps.de.travail != 0
						 & !is.na(Temps.de.travail),
							Heures := {"nredressements" %a% .N
										round(Temps.de.travail * nb.heures.temps.complet / 100, 1)}]

      # la solution de merge ne donne pas de meilleurs résultats et est moins fiable

      Paie[(Heures == 0 | is.na(Heures))
                       & Indice != ""
                       & !is.na(Indice)
                       & Statut != "ELU"
                       & Grade  != "V"
                       & Grade  != "A"
                       & Temps.de.travail != 0
                       & !is.na(Temps.de.travail),
                       Heures :=  round(Temps.de.travail * nb.heures.temps.complet / 100, 1)]

      }

      message("Correction (méthode 1), compte tenu des temps complets vérifiés, sur ",
              nredressements, " bulletins de paie")

  } else {

	# on fait la même hypothèse que précédemment, mais on utilise dans ce cas le traitement et l'indice pour redresser
	# le nombre d'heures, pas la quotité théorique.

      Paie[ , indic := (Heures == 0 | is.na(Heures))
                        & Indice != ""
                        & !is.na(Indice)
                        & Statut != "ELU"
            						& Grade != "V"
            						& Grade!= "A"
                        & Temps.de.travail != 0
            						& !is.na(Temps.de.travail)
                        & Type == "T"
            						& Montant > 0
                        & grepl(".*salaire|trait.*", Libelle, perl=TRUE, ignore.case=TRUE)
		      ][ , indic := any(indic), by = .(Matricule, Annee, Mois)]

      # attention ifelse pas if...else
      # La recherche binaire est 20 fois plus rapide que la recherche vscan (gain de 4s par million de lignes sur corei3)

      setkey(Paie, Annee, Mois, indic)

      for (A in periode) {
        for (M in 1:12) {
          a <- PointMensuelIM[A - 2007, M]
          Paie[list(A, M, TRUE),
               Heures := ifelse(Indice == 0, NA, Montant / (Indice * a * reference.nb.heures.par.mois))]
        }
      }

	  Bulletins.paie[ , Heures := NULL]

    "Bulletins.paie" %a% unique(Paie[ , c(names(Bulletins.paie),  "indic")])

    "nredressements" %a% nrow(Bulletins.paie[indic == TRUE])

	  Bulletins.paie[, indic := NULL]

    message("Correction (méthode 2), compte tenu des temps complets vérifiés, sur ",
              nredressements, " lignes de paie")
  }
}

#' Importer les données
#' @param normaliser.colonnes  Enlever les accents des noms de colonnes
#' @note Cette option vise à circonvenir un ancien bug de la bibliothèque \code{data.table}, désormais apparemment corrigé (2021)
#' @export

importer <- function(normaliser.colonnes = TRUE) {

  essayer(label = "+importer", importer_(normaliser.colonnes), "L'importation des données n'a pas pu être réalisée", abort = TRUE)

}

importer_ <- function(normaliser.colonnes = TRUE) {

  # Il importe que de ne pas confondre le separateur decimal et le separateur de champ CSV
  "duree.sous.revue" %a% 1

  if (separateur.decimal.entree == separateur.liste.entree)
    stop("Le separateur decimal en entree doit être différent du separateur de colonnes !")

  if (separateur.decimal.sortie == separateur.liste.sortie)
    stop("Le separateur decimal en sortie doit être différent du separateur de colonnes !")

  # Création des répertoires des fichiers CSV en lien dans les rapports. Attention éviter les caractères non ASCII (bug de knitr)
  # On peut désactiver les sorties CSV en fixant   sauvegarder.bases.analyse à FALSE [défaut TRUE]

  if (sauvegarder.bases.analyse) {
    for (path in c("Remunerations", "Effectifs", "Reglementation", "Fiabilite"))
      dir.create(file.path(chemin.dossier.bases, path), recursive = TRUE, mode = "0777")
  }

  # Les bases Table et Bulletins augmentées de quelques colonnes auxiliaires et légèrement retraitées peuvent
  # être à nouveau exportees dans Paiements. Généralement inutile, sauf aux fins de débogage. Pour cela
  # fixer sauvegarder.bases.origine à TRUE [défaut FALSE]

  if (sauvegarder.bases.origine)
    dir.create(file.path(chemin.dossier.bases, "Paiements"), recursive = TRUE, mode = "0777")

  "fichier.personnels.existe" %a% (charger.categories.personnel == TRUE & file.exists(chemin("matricules.csv")))
  "grades.categories.existe" %a%  (charger.categories.personnel == TRUE & file.exists(chemin("grades.categories.csv")))

  "logements.existe" %a%     file.exists(chemin("logements.csv"))
  "plafonds.ifse.existe" %a% file.exists(chemin("plafonds_ifse.csv"))

  base.personnels.categorie <- importer_matricules()

  base.logements            <- importer_base_logements()
  base.ifse                 <- importer_base_ifse()

  fichiers.table     <- list.files(chemin.cle, pattern = nom.table %+% "(-)?[^.]*[.]csv",     full.names  = TRUE)
  fichiers.bulletins <- list.files(chemin.cle, pattern = nom.bulletins %+% "(-)?[^.]*[.]csv", full.names  = TRUE)

  # Programme principal

  # Bases

  # Lignes de paie

  # On ne retient que les bases ayant pour annees au minimum debut.periode.sous.revue
  # et au maximum fin.periode.sous.revue, qui contiennent toutes les colonnes requises
  # pour le contrôle

  # Le mode rapide n'est disponible que avec des csv à separateurs virgule
  # Il permet d'économiser environ 8s par million de ligne lues sur une dizaine de champs

  if (! charger.bases) return(c(0, 0))

  type.données()

  importer.bases.via.xhl2csv("Paie", fichiers.table, colClasses =  colonnes.classes.input, select = lignes.noms)
  importer.bases.via.xhl2csv("Bulletins.paie", fichiers.bulletins, colClasses =  colonnes.bulletins.classes.input, select = bulletins.noms)
  
  Bulletins.paie[ , Grade := toupper(Grade)]
  Paie[ , Grade := toupper(Grade)]

  if (! is.null(Paie) && ! is.null(Bulletins.paie)) {

    message("Chargement de la table bulletins-lignes de Paie.")

  } else {

    stop("Impossible de charger les lignes/bulletins de paie.")
  }

  if (normaliser.colonnes) {
  
    convertir.accents(list(Paie, Bulletins.paie)) 
    setkey(Paie, Matricule, Annee, Mois)
    setkey(Bulletins.paie, Matricule, Annee, Mois)
  }  else  {
  
    setkey(Paie, Matricule, Année, Mois)
    setkey(Bulletins.paie, Matricule, Année, Mois)
    return("")
  }
  
  if (! is.null(base.personnels.categorie)) {

    message("Remplacement de la catégorie par la categorie importee du fichier matricules.csv sous ", chemin.dossier.donnees)
    vect <- c("Annee", "Nom", "Prenom", "Matricule", "Grade", "Emploi")
    
    BP <- unique(base.personnels.categorie[ , , keyby = vect])

    Paie[ , Categorie := NULL]
    Bulletins.paie[, Categorie := NULL]

    Paie <- merge(Paie[ , , keyby = vect], BP, all = TRUE, by = vect)

    Bulletins.paie  <- merge(Bulletins.paie[ , , keyby = vect], BP, all = TRUE, by = vect)
  }
  
  
  
  # dans le cas où l'on ne lance le programme que pour certaines annees, il préciser debut.periode.sous.revue et fin.periode .sous.revue
  # dans le fichier prologue.R. Sinon le programme travaille sur l'ensemble des annees disponibles.

  Extraire.annees()

  # Elus, vacataire, assistantes maternelles
  identifier.personnels()

  Paie[is.na(Grade),  Grade  := ""]
  Paie[is.na(Statut), Statut := "AUTRE_STATUT"]
  Paie[is.na(NBI),    NBI    := 0]
  Bulletins.paie[is.na(Grade),  Grade  := ""]
  Bulletins.paie[is.na(Statut), Statut := "AUTRE_STATUT"]
  Bulletins.paie[is.na(NBI),    NBI    := 0]
  Bulletins.paie[grepl("CONTRA", Grade, fixed = TRUE), Statut := "NON_TITULAIRE"]
  Paie[grepl("CONTRA", Grade, fixed = TRUE), Statut := "NON_TITULAIRE"]

  "periode" %a% debut.periode.sous.revue:fin.periode.sous.revue
  "duree.sous.revue" %a% (fin.periode.sous.revue - debut.periode.sous.revue + 1)
  "nb.annees" %a% uniqueN(Bulletins.paie[! is.na(Annee), Annee])

  if (duree.sous.revue != nb.annees) {

    cat("Les exercices doivent être consécutifs. Or il y a", nb.annees, "exercices, dans un intervalle de", duree.sous.revue, "années.")
    stop("Sélectionner des exercices consécutifs. Fin du programme.")
  }

  "annees.analyse.statique" %a% ifelse(analyse.statique.totale, periode, c(debut.periode.sous.revue, fin.periode.sous.revue))

  # Le format est jour/mois/annee avec deux chiffres-separateur-deux chiffres-separateur-4 chiffres.
  # Le separateur peut être changé en un autre en modifiant le "/" dans date.format

  "avant.redressement" %a% 0
  "après.redressement" %a% 0

  # Par défaut on n'élimine pas les doublons car il peut y avoir bcp de "faux doublons"
  # voir paramètre dans prologue.R

  if (éliminer.duplications) Eliminer.duplications()
  
  message("Vérification de la durée légale théorique du travail (1820 h = 35h x 52 semaines soit 151,67 h/mois)")

  res <- verif.temps.complet()

  "test.temps.complet" %a% res[1]
  "nb.heures.temps.complet" %a% res[2]

  "nb.heures.temps.complet" %a% ifelse(test.temps.complet, reference.nb.heures.par.mois,   #  1820 / 12
                                                           floor(nb.heures.temps.complet))

  # lorsque les heures mentionnées sont incohérentes avec les quotités théoriques (Temps.de.travail), redresser Heures:

  Redresser.heures()

  Paie[ , Filtre_actif := FALSE]

  # TODO: à revoir pour deux causes : le revenu peut ne pas être un traitement et les heures peuvent être nulles pour
  # des temps pleins...

  Paie[ ,
        Filtre_actif := any(Montant[Type == "T" & Heures > minimum.positif] > minimum.actif, na.rm = TRUE),
        by = .(Matricule, Annee)]

  # R est le rang (0-based) décalé d'une unité (lag 1)

  Bulletins.paie[ , `:=`(Sexe = substr(Nir, 1, 1),
                         R    = .I - 1)]

  # Attention, NA, pas FALSE

  set(Bulletins.paie, 1L, "R", NA)

  # Médiane des services horaires à temps complet par emploi et par sexe

  # La variable Heures des élus est non fiable et on peut par convention prendre la quotite 1

  # Pour faciliter les comparaisons de quotite lors du calcul de la RMPP on arrondit les quotites au centième inférieur
  # Lorsque la déterminéation de la médiane par emploi et sexe du nombre d'heures travaillées à temps complet n'est pas positive, la quotite est indéfinie
  # Une quotite ne peut pas dépasser 1.
  # Les élus sont réputés travailler à temps complet.

  message("Calcul des quotites")
  message("cle.fusion")
  #on va trouver la plupart du temps 151,67...
  # Tableau de référence des matrices de médianes
  # A ce niveau de généralité, le filtre actif est inutile, sauf peut-être pour de très petits effectifs.
  # M <- Bulletins.paie[(Sexe == "1" | Sexe == "2"),
  #                     .(Médiane_Sexe_Statut = median(Heures, na.rm=TRUE)), by="Sexe,Statut"]

  # Paie n'est pas modifié

  Bulletins.paie <- rémunérations_eqtp(quotites())

  # Housecleaning
  # La clé d'appariement entre Paie et Bulletins.paie est 
  # .(Nom, Prenom, Matricule, Annee, Mois, Emploi, Service, Statut, Grade, Nb.Enfants, Temps.de.travail, Heures.Sup., Heures, Indice, NBI, MtBrut, MtNet)
    
  cle.fusion <- c("Nom", "Prenom", "Matricule", "Annee", "Mois", "Emploi", "Service", "Statut", "Grade", "Nb.Enfants", "Temps.de.travail",
                  "Heures.Sup.", "Heures.orig", "Indice", "NBI", "Brut", "Net")

  cols.fusion <- c(cle.fusion, 
                    "cumHeures",
                    "quotite",
                    "quotite.moyenne",
                    "quotite.moyenne.orig",
                    "Montant.net.eqtp",
                    "Montant.brut.eqtp",
                    "Montant.brut.annuel",
                    "Montant.brut.annuel.eqtp",
                    "Montant.net.annuel",
                    "Montant.net.annuel.eqtp",
                    "Statut.sortie",
                    "Sexe",
                    "nb.jours",
                    "nb.mois",
                    "indicatrice.quotite.pp",
                    "annee_entiere")
  
  Paie <- merge(unique(Bulletins.paie[ , ..cols.fusion]),
                Paie,
                by = cle.fusion)

  matricules <- unique(Bulletins.paie[ , .(Annee, Nom, Prenom, Matricule, Categorie, Grade, Emploi)])

  "matricules" %a% matricules[order(Matricule, Annee)]

  # on essaie de deviner le versant de la FP

  if (is.na(VERSANT_FP))
    "VERSANT_FP" %a%  (if (grepl("AG.*HOSP", grades.categories$Grade, ignore.case = TRUE)) "FPH" else "FPT")

  "Paie" %a% Paie
  "Bulletins.paie" %a% Bulletins.paie
  "base.ifse" %a% base.ifse
  "base.personnels.categorie" %a% base.personnels.categorie
  "base.logements" %a% base.logements
  "grades.categories" %a% correspondance_grade_categorie()
}


