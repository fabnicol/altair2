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

#library(zeallot)

`%a%` <- function(x, y) assign(x, y, inherits = TRUE)

importer.bases.via.xhl2csv <- function(base, fichiers, colClasses = colonnes.classes.input) {
  
  res <- try(Read.csv(base,
                      fichiers,
                      colClasses = colClasses,
                      séparateur.liste = séparateur.liste.entrée,
                      séparateur.décimal = séparateur.décimal.entrée,
                      convertir.encodage = FALSE, #(encodage.entrée.xhl2csv != "UTF-8"),
                      encodage = encodage.entrée.xhl2csv,
                      rapide = TRUE),
             silent = FALSE)
  
  if (inherits(res, 'try-error'))
    stop("Problème de lecture de la base de la table bulletins-lignes de Paie")
  
  
  message("Chargement direct des bulletins et lignes de paie")
}

quotité.temps.partiel <- function(temps.de.travail) {
  
  if (x == 90) return(0.91429)  # 32/35 
  if (x == 80) return(0.85714)  # 6/7   
  return(x/100)
  
}

verif.temps.complet <- function() {
  
  # dans certains cas on a presque jamais la variable Heures renseignée... sauf pour quelques temps partiels
  
  h <- hist(Bulletins.paie[Temps.de.travail == 100, Heures], nclass = 20000, plot = FALSE)
  max.h <- which.max(h$counts)
  
  if (max.h > 1) {
    
    delta <- (h$mids[max.h + 1] - h$mids[max.h - 1])/2
    
    if (is.na(delta)) delta <- 1 #Présomption
    
    nb.heures.temps.complet <- floor(h$mids[max.h])
    
  } else {
    
    return(c(1, 151.67)) # présomption
  }
  
  valeur <- (abs(nb.heures.temps.complet - 151.67) < 1 + delta)  
  
  if (is.na(valeur))  {
    valeur <- 1 # présomption
  }
  
  return(c(valeur, nb.heures.temps.complet))
}

#' Calcul des quotités
#' @param Bulletins.paie Fichier des bulletins de paye
#' @param Paie Base des lignes de paye
#' @export

quotités <- function() {
  
  Bulletins.paie <- Paie[ , .(Filtre_actif), by=.(Matricule, Année, Mois), mult = "first"
                        ][Bulletins.paie,  on = .(Matricule, Année, Mois)]
  
  Bulletins.paie[ , pop_calcul_médiane := length(Heures[Temps.de.travail == 100 
                                                        & !is.na(Heures) 
                                                        & Heures > minimum.positif]),
                  by = .(Sexe, Emploi)]
  
  # Pour les quotités seules les périodes actives sont prises en compte
  
  Bulletins.paie[pop_calcul_médiane > population_minimale_calcul_médiane 
                 & Filtre_actif == TRUE, 
                   MHeures :=  median(Heures[Temps.de.travail == 100 
                                             & Filtre_actif == TRUE
                                             & Heures > minimum.positif], na.rm = TRUE),
                     by = .(Sexe, Emploi)]
  
  # L'écrêtement des quotités est une contrainte statistiquement discutable qui permet de "stresser" le modèle
  # Par défaut les quotités sont écrêtées pour pouvoir par la suite raisonner en définissant le temps plein comme quotité == 1
  
  if (écreter.quotités) {
    
    Bulletins.paie[ , quotité   :=  ifelse(MHeures < minimum.positif, NA, ifelse(Heures > MHeures, 1, round(Heures/MHeures, digits = 2)))]  
    
  } else {
    
    Bulletins.paie[ , quotité   :=  ifelse(MHeures < minimum.positif, NA, round(Heures/MHeures, digits = 2))]  
  }
  
  Bulletins.paie[Statut == "ELU", `:=`(MHeures = 1,
                                       quotité = 1)]
  
  message("Quotités calculées")
  
  return(Bulletins.paie)
}

#' Calculer la rémunération nette et brute en EQTP
#' @param DT  data.table (Bulletins.paie)
#' @export

rémunérations_eqtp <- function(DT) {
  
  # DT[ ,   Montant.net.eqtp  := Net.à.Payer / quotité]
  # DT[is.na(Montant.net.eqtp) | ! is.finite(Montant.net.eqtp),   Montant.net.eqtp  := 0]
  # 
  # DT[ ,   Montant.brut.eqtp  := Brut / quotité]
  # DT[is.na(Montant.brut.eqtp) | ! is.finite(Montant.brut.eqtp),   Montant.brut.eqtp  := 0]
  
  DT[ , Montant.brut.eqtp := Brut]
  DT[ , Montant.net.eqtp  := Net.à.Payer]
  
  DT[ ,   `:=`(Statut.sortie   = Statut[length(Net.à.Payer)],
                           nb.jours        = calcul.nb.jours.mois(Mois, Année[1]),
                           nb.mois         = length(Mois),
                           cumHeures       = sum(Heures, na.rm = TRUE),
                           quotité.moyenne = sum(quotité, na.rm = TRUE) / 12,
                           quotité.moyenne.orig = sum(Temps.de.travail, na.rm = TRUE) / 1200),
                  
                      key = .(Matricule, Année)]
  
  # Indicatrice pour la rémunération moyenne des personnes en place :
  # quotité égale pendant deux années successives contigues, permanence sur 12 mois.
  # nous prenons les moyennes des quotités non NA.
  
  DT[ , indicatrice.quotité.pp := (Matricule[R] == Matricule 
                                               & Année[R]   == Année - 1 
                                               & abs(quotité.moyenne[R] - quotité.moyenne) < tolérance.variation.quotité
                                               & nb.mois[R] == nb.mois
                                               & nb.mois    == 12)]
  
  DT[ ,   `:=`(Montant.brut.annuel      = sum(Brut, na.rm = TRUE),
                           Montant.brut.annuel.eqtp = 365 / nb.jours * sum(Montant.brut.eqtp , na.rm = TRUE),
                           Montant.net.annuel.eqtp  = 365 / nb.jours * sum(Montant.net.eqtp, na.rm = TRUE),
                           Montant.net.annuel       = sum(Net.à.Payer, na.rm = TRUE),
                           permanent                = nb.jours >= 365,
                           indicatrice.quotité.pp   = indicatrice.quotité.pp[1]),
                  
                  key = .(Matricule, Année)]
  
  message("Rémunérations EQTP calculées")
  
  # DT est modifié par référence
  
  return(DT)
}

#' Importer les données
#' @export

importer <- function() {

    # Il importe que de ne pas confondre le séparateur décimal et le séparateur de champ CSV
    
    if (séparateur.décimal.entrée == séparateur.liste.entrée)
      stop("Le séparateur décimal en entrée doit être différent du séparateur de colonnes !")
    
    if (séparateur.décimal.sortie == séparateur.liste.sortie)
      stop("Le séparateur décimal en sortie doit être différent du séparateur de colonnes !")
    
    # Création des répertoires des fichiers CSV en lien dans les rapports. Attention éviter les caractères non ASCII (bug de knitr)  
    # On peut désactiver les sorties CSV en fixant   sauvegarder.bases.analyse à FALSE [défaut TRUE]
    
    if (sauvegarder.bases.analyse) {
      for (path in c("Remunerations", "Effectifs", "Reglementation", "Fiabilite"))
        dir.create(file.path(chemin.dossier.bases, path), recursive = TRUE, mode="0777")
    }
    
    # Les bases Table et Bulletins augmentées de quelques colonnes auxiliaires et légèrement retraitées peuvent
    # être à nouveau exportées dans Paiements. Généralement inutile, sauf aux fins de débogage. Pour cela
    # fixer sauvegarder.bases.origine à TRUE [défaut FALSE]
    
    if (sauvegarder.bases.origine)
      dir.create(file.path(chemin.dossier.bases, "Paiements"), recursive = TRUE, mode="0777")
    
    "fichier.personnels.existe" %a% (charger.catégories.personnel == TRUE & file.exists(chemin("matricules.csv")))
    "grades.categories.existe" %a%  (charger.catégories.personnel == TRUE & file.exists(chemin("grades.categories.csv")))
    "logements.existe" %a%     file.exists(chemin("logements.csv"))
    "plafonds.ifse.existe" %a% file.exists(chemin("plafonds_ifse.csv"))
    
    base.personnels.catégorie <- NULL
    base.grades.categories    <- NULL
    base.logements <- NULL
    
    if (fichier.personnels.existe) {
      base.personnels.catégorie <- data.table::fread(chemin("matricules.csv"),
                                                     sep = séparateur.liste.entrée,
                                                     header = TRUE,
                                                     colClasses = c("numeric", "character", "character",
                                                                    "character", "character", "character", "character"),
                                                     encoding = ifelse(setOSWindows, "Latin-1", "UTF-8"),
                                                     showProgress = FALSE) 
    
      message("Chargement du fichier des catégories statutaires des personnels.")
      if (!is.null(base.personnels.catégorie))
        message("Importé.")
      else {
        message("Impossible d'importer les catégories.")
        stop(" ")
      }
    }
    
    if (grades.categories.existe) {
      base.grades.categories <- data.table::fread(chemin("grades.categories.csv"),
                                                     sep = séparateur.liste.entrée,
                                                     header = TRUE,
                                                     colClasses = c("character", "character"),
                                                     encoding = "Latin-1",
                                                     showProgress = FALSE) 
      
      message("Chargement du fichier des grades et catégories statutaires des personnels.")
      if (!is.null(base.grades.categories))
        message("Importé.")
      else {
        message("Impossible d'importer les grades et catégories.")
        stop(" ")
      }
    }
    
    if (logements.existe) {
      base.logements <- data.table::fread(chemin("logements.csv"),
                                                  sep = séparateur.liste.entrée,
                                                  header = TRUE,
                                                  colClasses = c("character", "integer", "integer", "character"),
                                                  encoding = "Latin-1",
                                                  showProgress = FALSE) 
      
      message("Chargement du fichier des concessions de logement des personnels.")
      
      if (!is.null(base.logements))
        message("Importé.")
      else {
        message("Impossible d'importer les concessions de logement.")
        stop(" ")
      }
    }
    
    if (plafonds.ifse.existe) {
      base.ifse <- data.table::fread(chemin("plafonds_ifse.csv"),
                                          sep = séparateur.liste.entrée,
                                          header = TRUE,
                                          colClasses = c("character", "character", "character", "numeric"),  # Grade, Groupe, Logement, Plafond
                                          encoding = "Latin-1",
                                          showProgress = FALSE) 
      
      message("Chargement du fichier des plafonds d'IFSE.")
      
      if (!is.null(base.ifse))
        message("Importé.")
      else {
        message("Impossible d'importer les plafonds d'IFSE.")
        stop(" ")
      }
    } else {
     
       base.ifse <- NULL
       
    }
    
    fichiers.table     <- list.files(chemin.clé, pattern = nom.table %+% "(-)?[^.]*[.]csv",     full.names  = TRUE)
    fichiers.bulletins <- list.files(chemin.clé, pattern = nom.bulletins %+% "(-)?[^.]*[.]csv", full.names  = TRUE)
    
    # Programme principal
    
    # Bases
    
    # Lignes de paie
    
    # On ne retient que les bases ayant pour années au minimum début.période.sous.revue
    # et au maximum fin.période.sous.revue, qui contiennent toutes les colonnes requises
    # pour le contrôle
    
    # Le mode rapide n'est disponible que avec des csv à séparateurs virgule
    # Il permet d'économiser environ 8s par million de ligne lues sur une dizaine de champs
    
    if (! charger.bases) return(c(0, 0))
    
    colonnes <- names(data.table::fread(fichiers.bulletins[1],
                            sep = séparateur.liste.entrée,
                            dec = séparateur.décimal.entrée,
                            nrows = 0,
                            header = TRUE,
                            #skip = champ.détection.1,
                            encoding = ifelse(setOSWindows, "Latin-1", "UTF-8")))
        
    type.données(colonnes)
    
    importer.bases.via.xhl2csv("Paie", fichiers.table, colClasses =  colonnes.classes.input)
    importer.bases.via.xhl2csv("Bulletins.paie", fichiers.bulletins, colClasses =  colonnes.bulletins.classes.input)
    
    Bulletins.paie[ , Grade := toupper(Grade)]
    Paie[ , Grade := toupper(Grade)]
      
      if (! is.null(Paie) && ! is.null(Bulletins.paie)) {
        message("Chargement de la table bulletins-lignes de Paie.")
      } else {
        stop("Impossible de charger les lignes/bulletins de paie.")
      }
    
      if (! is.null(base.personnels.catégorie)) {
      
        message("Remplacement de la catégorie par la catégorie importée du fichier matricules.csv sous ", chemin.dossier.données)
        vect <- c("Année", "Nom", "Prénom", "Matricule", "Grade", "Emploi")
        
        Paie[, Catégorie := NULL]
        Bulletins.paie[, Catégorie := NULL]
        
        #BP <- base.personnels.catégorie[ , , keyby = vect]
        #Paie <- merge(Paie[ , , keyby = vect], BP, all = TRUE, by = vect)
        #Bulletins.paie  <- merge(Bulletins.paie[ , , keyby = vect], BP, all = TRUE, by = vect)
        
        Paie <- base.personnels.catégorie[Paie, on = vect]
        Bulletins.paie <- base.personnels.catégorie[Bulletins.paie, on = vect]
        
      } else {
      
        if (! is.null(base.grades.categories)) {
          
          message("Remplacement de la catégorie par la catégorie importée du fichier grades.categories.csv sous ", chemin.dossier.données)
          
          Paie[, Catégorie := NULL]
          Bulletins.paie[, Catégorie := NULL]
          BP <- base.grades.categories[Grade != "V" & Grade != "A", Catégorie, keyby = "Grade"]
          BP <- rbindlist(list(BP, data.table("V", "NA")))
          BP <- rbindlist(list(BP, data.table("A", "NA")))
          
          Paie <- BP[Paie, on = "Grade"]
          Bulletins.paie <- BP[Bulletins.paie, on = "Grade"]
        }
      }
      
      setkey(Paie, Matricule, Année, Mois)
      setkey(Bulletins.paie, Matricule, Année, Mois)
      
      # dans le cas où l'on ne lance le programme que pour certaines années, il préciser début.période sous revue et fin.période .sous.revue
      # dans le fichier prologue.R. Sinon le programme travaille sur l'ensemble des années disponibles.
      
      if (extraire.années) {
        
         Paie <- Paie[Année >= début.période.sous.revue & Année <= fin.période.sous.revue]
         Bulletins.paie <- Bulletins.paie[Année >= début.période.sous.revue & Année <= fin.période.sous.revue]
        
      } else {
        
        "début.période.sous.revue" %a% min(Bulletins.paie[ , Année])
        "fin.période.sous.revue" %a% max(Bulletins.paie[ , Année])
      }
      
    Paie[is.na(Grade),  Grade  := ""]
    Paie[is.na(Statut), Statut := "AUTRE_STATUT"]
    Paie[is.na(NBI),    NBI    := 0]
    Bulletins.paie[is.na(Grade),  Grade  := ""]
    Bulletins.paie[is.na(Statut), Statut := "AUTRE_STATUT"]
    Bulletins.paie[is.na(NBI),    NBI    := 0]
    
    "période" %a% début.période.sous.revue:fin.période.sous.revue
    "durée.sous.revue" %a% (fin.période.sous.revue - début.période.sous.revue + 1)
    "nb.années" %a% uniqueN(Bulletins.paie$Année)
    
    if (durée.sous.revue != nb.années) {
      
      cat("Les exercices doivent être consécutifs. Or il y a", nb.années, "exercices, dans un intervalle de", durée.sous.revue, "années.")
      stop("Sélectionner des exercices consécutifs. Fin du programme.")
    }
    
    if (! analyse.statique.totale) {
      
      "années.analyse.statique" %a% c(début.période.sous.revue, fin.période.sous.revue)
      
    } else {
      
      "années.analyse.statique" %a% période
    }
    
    # Le format est jour/mois/année avec deux chiffres-séparateur-deux chiffres-séparateur-4 chiffres.
    # Le séparateur peut être changé en un autre en modifiant le "/" dans date.format
    
    "avant.redressement" %a% 0
    "après.redressement" %a% 0
    
    if (éliminer.duplications) {
      
      "avant.redressement" %a% nrow(Paie)
      
      duplications.vecteur <- duplicated(Paie, by=NULL)
      duplications.paie <- Paie[duplications.vecteur & Montant != 0]
      
      Paie <- Paie[! duplications.vecteur]
      
      if (sauvegarder.bases.origine)
          sauv.bases(chemin.dossier.bases, 
                      env = environment(),
                      "duplications.paie")
          
      "après.redressement" %a% nrow(Paie)
      
      avant.redressement.bull <- nrow(Bulletins.paie)
      duplications.vecteur    <- duplicated(Bulletins.paie, by=NULL)
      duplications.paie.bull  <- Bulletins.paie[duplications.vecteur & Montant != 0]
      
      Bulletins.paie <- Bulletins.paie[! duplications.vecteur] 
      
      if (sauvegarder.bases.origine) {
        sauv.bases(chemin.dossier.bases, 
          env = environment(),
          "duplications.paie")
          
        sauv.bases(chemin.dossier.bases,
          env = environment(),
          "duplications.paie.bull")
      }
      
      après.redressement.bull <- nrow(Bulletins.paie)
    } 
    
    
    message("Vérification de la durée légale théorique du travail (1820 h = 35h x 52 semaines soit 151,67 h/mois)")
    
    "test.temps.complet" %a% verif.temps.complet()[1]
    "nb.heures.temps.complet" %a% verif.temps.complet()[2]
    
    if (test.temps.complet) {
    
      "nb.heures.temps.complet" %a% 151.67  #  1820 / 12
      
    } else {
      
      "nb.heures.temps.complet" %a% floor(nb.heures.temps.complet)
    
    }
  
    # si l'on a une cohérence du calcul des heures de travail par semaine alors peut se baser dessus :
    
    if (redresser.heures) {
      
          # On ne peut pas inférer sur quotite Trav (Temps.de.travail) de manière générale
          # Mais on peut exclure les cas dans lesquels Temps de travail est non fiable puis déduire en inférence sur ce qui reste
          # critère d'exclusion envisageable pour les stats de rémunérations à quotités :
          # Paie[Indice == "" & Type %chin% c("T", "I", "A", "AC") & Heures == 0 | Statut %chin% c("ELU", "v", "A")]
          # sur le reste on peut inférer Heures 
          
          setnames(Paie, "Heures", "Heures.orig")
          setnames(Bulletins.paie, "Heures", "Heures.orig")
          Paie[ , Heures := Heures.orig]
          Bulletins.paie[ , Heures := Heures.orig]
         
         if (test.temps.complet) {
        
               if (nrow(Paie) < 1e6) {
                 
                 #microbenchmark::  microbenchmark(A <- 
                 Paie[(Heures == 0 | is.na(Heures))
                        & Indice != 0 & !is.na(Indice)
                        & Statut != "ELU" & Grade != "V" & Grade!= "A"
                        & Temps.de.travail != 0 
                        & !is.na(Temps.de.travail), 
                            indic := TRUE]
                 
                 # "plonking"
                 
                 Paie[indic == TRUE , 
                        Heures := round(Temps.de.travail * nb.heures.temps.complet / 100, 1)]
                 
                 #, times=1)
            
               } else {
    
              # ----- Pour les très gros fichiers (> 1 ML) , plus rapide que la solution de référence supra. On gagne 1 s par ML à partir de 15 ML
              #       0.5 s par ML à partir de 3 ML. Dépend beaucoup du CPU et de la mémoire. Résultats sur corei7, DDR4 1333, non vérifiés sur corei3.
              #       Gain de 14 s à 15 s pour un gros fichier de 15 ML.  
               
                   message("correction du temps de travail par recherche binaire")
      
                     setkey(Paie, Heures) 
                     
                     Paie[.(c(0, NA_real_)), 
                              indic := ifelse(Statut != "ELU" 
                                              & Grade  != "V" 
                                              & Grade  != "A"  
                                              & Indice != 0  
                                              & is.na(Indice) 
                                              & Temps.de.travail != 0 
                                              & !is.na(Temps.de.travail),
                                              TRUE,
                                              NA),
                               nomatch=0]
                     
                     Paie[.(c(0, NA_real_)),
                           Heures := indic * round(Temps.de.travail * nb.heures.temps.complet / 100, 1)]
                   
               }
         }
        
          # -----
          
          Bulletins.paie[(Heures == 0 | is.na(Heures))
                           & Indice != "" & !is.na(Indice) 
                           & Statut != "ELU" & Grade != "V" & Grade!= "A"
                           & Temps.de.travail != 0 & !is.na(Temps.de.travail), 
                              indic := TRUE]
          
          # plonking
          
          Bulletins.paie[indic == TRUE , 
                           Heures := round(Temps.de.travail * nb.heures.temps.complet / 100, 1)][]
          
          message("Correction (méthode 1), compte tenu des temps complets vérifiés, sur ",
                  nredressements <<- nrow(Bulletins.paie[indic == TRUE]), " bulletins de paie")
          
    } else {
      
      # on présume alors que les traitements sont correctement liquidés... il faudrait mettre un drapeau sur cette présomption  
    
        Paie[ , indic := (Heures == 0 | is.na(Heures))
                         & Indice != "" & !is.na(Indice) 
                         & Statut != "ELU" & Grade != "V" & Grade!= "A"
                         & Temps.de.travail != 0 & !is.na(Temps.de.travail)
                         & Type == "T" & Montant > 0
                         & grepl(".*salaire|trait.*", Libellé, perl=TRUE, ignore.case=TRUE)]
        
      # attention ifelse pas if...else
      # La recherche binaire est 20 fois plus rapide que la recherche vscan (gain de 4s par million de lignes sur corei3)
      
      setkey(Paie, Année, Mois, indic)  
      
      for (A in période) {
          for (M in 1:12) {
                a <- PointMensuelIM[A - 2007, M]  
                Paie[list(A, M, TRUE), 
                       Heures := ifelse(Indice == 0, NA, Montant / (Indice * a * 151.67))]
           }
        }    
      
      
        Bulletins.paie <- Paie[ , .(Matricule, 
                                     Année,
                                     Mois,
                                     Service,
                                     Statut,
                                     Heures,
                                     indic)
                              ][Bulletins.paie[ , Heures := NULL], 
                                       on = .(Matricule, Année, Mois, Service, Statut)]
        
        message("Correction (méthode 2), compte tenu des temps complets vérifiés, sur ", 
                nredressements <<- nrow(Bulletins.paie[indic == TRUE]), " lignes de paie")
       
    }
    
      Paie[ , Filtre_actif := FALSE]
    
    # TODO: à revoir pour deux causes : le revenu peut ne pas être un traitement et les heures peuvent être nulles pour
    # des temps pleins...
    
      Paie[ , 
             Filtre_actif := any(Montant[Type == "T" & Heures > minimum.positif] > minimum.actif, na.rm = TRUE),
               by = .(Matricule, Année)]
      
      Paie[ , delta := 0, by = .(Matricule, Année, Mois)]
      
      Paie[Type %chin% c("I", "T", "S", "IR", "AC","A", "R", "AV") , 
             delta := sum(Montant,  na.rm=TRUE) - Brut,
               by = .(Matricule, Année, Mois)]
    
      # R est le rang (0-based) décalé d'une unité (lag 1)
      
      Bulletins.paie[ , `:=`(Sexe = substr(Nir, 1, 1),
                             R    = .I - 1)]
      
      # Attention, NA, pas FALSE
      
      set(Bulletins.paie, 1, "R", NA)
      
      # Médiane des services horaires à temps complet par emploi et par sexe 
      
      # La variable Heures des élus est non fiable et on peut par convention prendre la quotité 1
      
      # Pour faciliter les comparaisons de quotité lors du calcul de la RMPP on arrondit les quotités au centième inférieur
      # Lorsque la déterminéation de la médiane par emploi et sexe du nombre d'heures travaillées à temps complet n'est pas positive, la quotité est indéfinie
      # Une quotité ne peut pas dépasser 1.
      # Les élus sont réputés travailler à temps complet.
      
      message("Calcul des quotités")
      
      #on va trouver la plupart du temps 151,67...
      # Tableau de référence des matrices de médianes
      # A ce niveau de généralité, le filtre actif est inutile, sauf peut-être pour de très petits effectifs.
      # M <- Bulletins.paie[(Sexe == "1" | Sexe == "2"), 
      #                     .(Médiane_Sexe_Statut = median(Heures, na.rm=TRUE)), by="Sexe,Statut"]
      
      # Paie n'est pas modifié
      
      Bulletins.paie <- rémunérations_eqtp(quotités())

      # Housecleaning
      
      Bulletins.paie[ , indic := NULL]
      
      vect <- c("Matricule", "Année", "Mois", "Service", "Statut")
      
      Paie <- Paie[unique(Bulletins.paie[ , .(Matricule, 
                                   Année,
                                   Mois,
                                   Service,
                                   Statut,
                                   cumHeures,
                                   quotité,
                                   quotité.moyenne,
                                   quotité.moyenne.orig,
                                   Montant.net.eqtp,
                                   Montant.brut.eqtp,
                                   Montant.brut.annuel,
                                   Montant.brut.annuel.eqtp,
                                   Montant.net.annuel,
                                   Montant.net.annuel.eqtp,
                                   Statut.sortie,
                                   Sexe,
                                   nb.jours,
                                   nb.mois,
                                   indicatrice.quotité.pp,
                                   permanent)], by = vect),
                             on = vect
                           ][ , indic := NULL]
      
      matricules <- unique(Bulletins.paie[ , .(Année, Nom, Prénom, Matricule, Catégorie, Grade, Emploi)], by = NULL)
      
      "matricules" %a% matricules[order(Matricule, Année)]
      
      grades.categories <- unique(matricules[ , .(Grade, Catégorie)], by = NULL)
      
      "grades.categories" %a% grades.categories[order(Grade)]
      
      # on essaie de deviner le versant de la FP par l'existence d'agents de service hospitalier
      # on peut désactiver ce test par désactiver.test.versant.fp <- T dans prologue.R
      
      if (is.na(VERSANT_FP))
         "VERSANT_FP" %a%  (if (grepl("AG.*HOSP", grades.categories$Grade, ignore.case = TRUE)) "FPH" else "FPT")
      
      "Paie" %a% Paie
      "Bulletins.paie" %a% Bulletins.paie
      "base.ifse" %a% base.ifse
      "base.personnels.catégorie" %a% base.personnels.catégorie
      "base.grades.categories" %a% base.grades.categories
      "base.logements" %a% base.logements
  
}
