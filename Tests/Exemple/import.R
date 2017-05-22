# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, ann?es 2012 ? 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant ? extraire et analyser les fichiers de paye
# produits au format sp?cifi? par l'annexe de la convention-cadre nationale de d?mat?rialisation
# en vigueur ? compter de l'ann?e 2008.
# 
# Ce logiciel est r?gi par la licence CeCILL soumise au droit fran?ais et
# respectant les principes de diffusion des logiciels libres. Vous pouvez
# utiliser, modifier et/ou redistribuer ce programme sous les conditions
# de la licence CeCILL telle que diffus?e par le CEA, le CNRS et l'INRIA
# sur le site "http://www.cecill.info".
# 
# En contrepartie de l'accessibilit? au code source et des droits de copie,
# de modification et de redistribution accord?s par cette licence, il n'est
# offert aux utilisateurs qu'une garantie limit?e. Pour les m?mes raisons,
# seule une responsabilit? restreinte p?se sur l'auteur du programme, le
# titulaire des droits patrimoniaux et les conc?dants successifs.
# 
# A cet ?gard l'attention de l'utilisateur est attir?e sur les risques
# associ?s au chargement, ? l'utilisation, ? la modification et/ou au
# d?veloppement et ? la reproduction du logiciel par l'utilisateur ?tant
# donn? sa sp?cificit? de logiciel libre, qui peut le rendre complexe ?
# manipuler et qui le r?serve donc ? des d?veloppeurs et des professionnels
# avertis poss?dant des connaissances informatiques approfondies. Les
# utilisateurs sont donc invit?s ? charger et tester l'ad?quation du
# logiciel ? leurs besoins dans des conditions permettant d'assurer la
# s?curit? de leurs syst?mes et ou de leurs donn?es et, plus g?n?ralement,
# ? l'utiliser et l'exploiter dans les m?mes conditions de s?curit?.
# 
# Le fait que vous puissiez acc?der ? cet en-t?te signifie que vous avez
# pris connaissance de la licence CeCILL, et que vous en avez accept? les
# termes.
# 
# 
# 

library(knitr)
library(ggplot2)

## Pour les versions de data.table < 1.9.5 
##  if (s?parateur.d?cimal.entr?e != ".")
##  stop("Pour les tables import?es par data.table::fread, le s?parateur d?cimal doit ?tre '.'")

if (s?parateur.d?cimal.entr?e == s?parateur.liste.entr?e)
  stop("Le s?parateur d?cimal en entr?e doit ?tre diff?rent du s?parateur de colonnes !")

if (s?parateur.d?cimal.sortie == s?parateur.liste.sortie)
  stop("Le s?parateur d?cimal en sortie doit ?tre diff?rent du s?parateur de colonnes !")

if (sauvegarder.bases.analyse) {
  for (path in c("Remunerations", "Effectifs", "Reglementation", "Fiabilite"))
    dir.create(file.path(chemin.dossier.bases, path), recursive = TRUE, mode="0777")
}

if (sauvegarder.bases.origine)
  dir.create(file.path(chemin.dossier.bases, "Paiements"), recursive = TRUE, mode="0777")

# probl?me temporaire avec l'option fig.retina depuis fin mai 2014

knitr::opts_chunk$set(fig.width = 7.5, echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')

# Contr?le de coh?rence
#  on v?rifie que chaque code de paie est associ?, dans le fichier des codes de paiement (par d?faut, racinecodes.csv),
#  que ? chaque code donn? on a associ? un et un seul type de r?mun?ration ("INDEMNITAIRE", "TRAITEMENT", etc.)
# Pour le mode rapide, convertir les fichiers base en UTF-8 SANS BOM (par exemple, notepad++ apr?s Excel)

 
fichier.personnels.existe <- (charger.cat?gories.personnel == TRUE) & file.exists(chemin("matricules.csv"))
grades.categories.existe <- (charger.cat?gories.personnel == TRUE) & file.exists(chemin("grades.categories.csv"))

base.personnels.cat?gorie <- NULL
base.grades.categories    <- NULL

if (fichier.personnels.existe) {
  base.personnels.cat?gorie <- data.table::fread(chemin("matricules.csv"),
                                                 sep = s?parateur.liste.entr?e,
                                                 header = TRUE,
                                                 colClasses = c("numeric", "character", "character",
                                                                "character", "character", "character", "character"),
                                                 encoding = ifelse(setOSWindows, "Latin-1", "UTF-8"),
                                                 showProgress = FALSE) 

  message("Chargement du fichier des cat?gories statutaires des personnels.")
  if (!is.null(base.personnels.cat?gorie))
    message("Import?.")
  else {
    message("Impossible d'importer les cat?gories.")
    stop(" ")
  }
}

if (grades.categories.existe) {
  base.grades.categories <- data.table::fread(chemin("grades.categories.csv"),
                                                 sep = s?parateur.liste.entr?e,
                                                 header = TRUE,
                                                 colClasses = c("character", "character"),
                                                 encoding = ifelse(setOSWindows, "Latin-1", "UTF-8"),
                                                 showProgress = FALSE) 
  
  message("Chargement du fichier des grades et cat?gories statutaires des personnels.")
  if (!is.null(base.grades.categories))
    message("Import?.")
  else {
    message("Impossible d'importer les grades et cat?gories.")
    stop(" ")
  }
}


fichiers.table <- list.files(chemin.cl?, pattern = nom.table %+% "(-)?[^.]*[.]csv", full.names  = TRUE)
fichiers.bulletins <- list.files(chemin.cl?, pattern = nom.bulletins %+% "(-)?[^.]*[.]csv", full.names  = TRUE)

# Programme principal

# Bases

# Lignes de paie

# On ne retient que les bases ayant pour ann?es au minimum d?but.p?riode.sous.revue
# et au maximum fin.p?riode.sous.revue, qui contiennent toutes les colonnes requises
# pour le contr?le

# Le mode rapide n'est disponible que avec des csv ? s?parateurs virgule
# Il permet d'?conomiser environ 8s par million de ligne lues sur une dizaine de champs

if (! charger.bases) break

T0 <- data.table::fread(fichiers.bulletins[1],
                        sep = s?parateur.liste.entr?e,
                        dec = s?parateur.d?cimal.entr?e,
                        nrows = 0,
                        header = TRUE,
                        #skip = champ.d?tection.1,
                        encoding = ifelse(setOSWindows, "Latin-1", "UTF-8"))

colonnes <- names(T0)

int?grer.rang <- ("R" %chin% colonnes) 
int?grer.?chelon <- ("Echelon" %chin% colonnes) 
int?grer.localisation <- ("Siret" %chin% colonnes)

if (int?grer.rang) message("Int?gration du Rang")
if (int?grer.?chelon) message("Int?gration de l'?chelon")
if (int?grer.localisation) message("Int?gration des donn?es ?tablissement")

source("types.donn?es.R", encoding = encodage.code.source)

importer.bases.via.xhl2csv <- function(base, fichiers, colClasses = colonnes.classes.input) {
  
  res <- try(Read.csv(base,
                      fichiers,
                      colClasses = colClasses,
                      s?parateur.liste = s?parateur.liste.entr?e,
                      s?parateur.d?cimal = s?parateur.d?cimal.entr?e,
                      convertir.encodage = FALSE, #(encodage.entr?e.xhl2csv != "UTF-8"),
                      encodage = encodage.entr?e.xhl2csv,
                      rapide = TRUE),
             silent = FALSE)
  
  if (inherits(res, 'try-error'))
    stop("Probl?me de lecture de la base de la table bulletins-lignes de Paie")

  
  message("Chargement direct des bulletins et lignes de paie")
}

importer.bases.via.xhl2csv("Paie", fichiers.table, colClasses =  colonnes.classes.input)
importer.bases.via.xhl2csv("Bulletins.paie", fichiers.bulletins, colClasses =  colonnes.bulletins.classes.input)

Bulletins.paie[ , Grade := toupper(Grade)]
Paie[ , Grade := toupper(Grade)]
  
  if (! is.null(Paie) && ! is.null(Bulletins.paie)) {
    message("Chargement de la table bulletins-lignes de Paie.")
  } else {
    stop("Impossible de charger les lignes/bulletins de paie.")
  }

  if (!is.null(base.personnels.cat?gorie)) {
  
    message("Remplacement de la cat?gorie par la cat?gorie import?e du fichier matricules.csv sous ", chemin.dossier.donn?es)
    BP <- base.personnels.cat?gorie[ , , keyby = "Ann?e,Nom,Pr?nom,Matricule,Grade,Emploi"]
    vect <- c("Ann?e", "Nom", "Pr?nom", "Matricule", "Grade", "Emploi")
    
    Paie[, Cat?gorie := NULL]
    Bulletins.paie[, Cat?gorie := NULL]
    
    Paie <- merge(Paie[ , , keyby = "Ann?e,Nom,Pr?nom,Matricule,Grade,Emploi"], BP, all = TRUE, by = vect)
    
    Bulletins.paie <- merge(Bulletins.paie[ , , keyby="Ann?e,Nom,Pr?nom,Matricule,Grade,Emploi"], BP, all =TRUE, by = vect)
  } else {
  
    if (!is.null(base.grades.categories)) {
      
      message("Remplacement de la cat?gorie par la cat?gorie import?e du fichier grades.categories.csv sous ", chemin.dossier.donn?es)
      
      Paie[, Cat?gorie := NULL]
      Bulletins.paie[, Cat?gorie := NULL]
      BP <- base.grades.categories[Grade != "V" & Grade != "A", Cat?gorie, keyby = "Grade"]
      BP <- rbindlist(list(BP, data.table("V", "NA")))
      BP <- rbindlist(list(BP, data.table("A", "NA")))
      
      Paie <- merge(Paie[ , , keyby = "Grade"], BP, all = TRUE, by = "Grade")
      
      Bulletins.paie <- merge(Bulletins.paie[ , , keyby="Grade"], BP, all = TRUE, by = "Grade")
    }
  }
  
  setkey(Paie, Matricule, Ann?e, Mois)
  setkey(Bulletins.paie, Matricule, Ann?e, Mois)
  
  # dans le cas o? l'on ne lance le programme que pour certaines ann?es, il pr?ciser d?but.p?riode sous revue et fin.p?riode .sous.revue
  # dans le fichier prologue.R. Sinon le programme travaille sur l'ensemble des ann?es disponibles.
  
  if (extraire.ann?es) {
    
    Paie <- Paie[Ann?e >= d?but.p?riode.sous.revue & Ann?e <= fin.p?riode.sous.revue, ]
    Bulletins.paie <- Bulletins.paie[Ann?e >= d?but.p?riode.sous.revue & Ann?e <= fin.p?riode.sous.revue, ]
    
  } else {
    
    d?but.p?riode.sous.revue <- min(Bulletins.paie[ , Ann?e])
    fin.p?riode.sous.revue   <- max(Bulletins.paie[ , Ann?e])
  }
  
  
  Paie[is.na(Grade),  Grade  := ""]
  Paie[is.na(Statut), Statut := "AUTRE_STATUT"]
  Paie[is.na(NBI),    NBI    := 0]
  Bulletins.paie[is.na(Grade),  Grade  := ""]
  Bulletins.paie[is.na(Statut), Statut := "AUTRE_STATUT"]
  Bulletins.paie[is.na(NBI),    NBI    := 0]


p?riode                 <- d?but.p?riode.sous.revue:fin.p?riode.sous.revue
dur?e.sous.revue        <- fin.p?riode.sous.revue - d?but.p?riode.sous.revue + 1

if (! analyse.statique.totale) {
  
  ann?es.analyse.statique <- c(d?but.p?riode.sous.revue, fin.p?riode.sous.revue)
  
} else {
  
  ann?es.analyse.statique <- p?riode
}



# Le format est jour/mois/ann?e avec deux chiffres-s?parateur-deux chiffres-s?parateur-4 chiffres.
# Le s?parateur peut ?tre chang? en un autre en modifiant le "/" dans date.format

avant.redressement <- 0
apr?s.redressement <- 0

if (?liminer.duplications) {
  avant.redressement <- nrow(Paie)
  duplications.vecteur <- duplicated(Paie, by=NULL)
  duplications.paie <- Paie[duplications.vecteur & Montant != 0]
  Paie <- Paie[! duplications.vecteur] 
  if (sauvegarder.bases.origine)
      sauv.bases(chemin.dossier.bases, "duplications.paie")
  apr?s.redressement <- nrow(Paie)
  
  avant.redressement.bull <- nrow(Bulletins.paie)
  duplications.vecteur   <- duplicated(Bulletins.paie, by=NULL)
  duplications.paie.bull <- Bulletins.paie[duplications.vecteur & Montant != 0]
  Bulletins.paie <- Bulletins.paie[! duplications.vecteur] 
  if (sauvegarder.bases.origine) {
    sauv.bases(chemin.dossier.bases, "duplications.paie")
    sauv.bases(chemin.dossier.bases, "duplications.paie.bull")
  }
  apr?s.redressement.bull <- nrow(Bulletins.paie)
  rm(duplications.vecteur)
  
} 
  

# calcul du temps complet mensuel de r?f?rence en h/mois

quotit?.temps.partiel <- function(temps.de.travail) {
  
  if (x == 90) return(0.91429)  # 32/35 
  if (x == 80) return(0.85714)  # 6/7   
  return(x/100)
  
}

verif.temps.complet <- function() {
  
  # dans certains cas on a presque jamais la variable Heures renseign?e... sauf pour quelques temps partiels
  
  h <- hist(Bulletins.paie[Temps.de.travail == 100, Heures], nclass = 20000, plot = FALSE)
  max.h <- which.max(h$counts)
  
  if (max.h > 1) {
    
    delta <- (h$mids[max.h + 1] - h$mids[max.h - 1])/2
    
    if (is.na(delta)) delta <<- 1 #Pr?somption
    
    nb.heures.temps.complet <<- floor(h$mids[max.h])
    
  } else {
    
    return(TRUE) # pr?somption
  }
  
  valeur <- (abs(nb.heures.temps.complet - 151.67) < 1 + delta)  
  
  if (is.na(valeur))  {
    valeur <- TRUE # pr?somption
  }
  
  return(valeur)
}

message("V?rification de la dur?e l?gale th?orique du travail (1820 h = 35h x 52 semaines soit 151,67 h/mois)")

test.temps.complet <<- verif.temps.complet()

if (test.temps.complet) {

  nb.heures.temps.complet <<- 151.67  #  1820 / 12
  
} else {
  
  nb.heures.temps.complet <<- floor(nb.heures.temps.complet)

}


# si l'on a une coh?rence du calcul des heures de travail par semaine alors peut se baser dessus :

if (redresser.heures) {
  
      # On ne peut pas inf?rer sur quotite Trav (Temps.de.travail) de mani?re g?n?rale
      # Mais on peut exclure les cas dans lesquels Temps de travail est non fiable puis d?duire en inf?rence sur ce qui reste
      # crit?re d'exclusion envisageable pour les stats de r?mun?rations ? quotit?s :
      # Paie[Indice == "" & Type %chin% c("T", "I", "A", "AC") & Heures == 0 | Statut %chin% c("ELU", "v", "A")]
      # sur le reste on peut inf?rer Heures 
      
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
              & Temps.de.travail != 0 & !is.na(Temps.de.travail), `:=`(indic = TRUE,
                                                                        Heures = round(Temps.de.travail * nb.heures.temps.complet / 100, 1))]
             #, times=1)
        
           } else {

          # ----- Pour les tr?s gros fichiers (> 1 ML) , plus rapide que la solution de r?f?rence supra. On gagne 1 s par ML ? partir de 15 ML
          #       0.5 s par ML ? partir de 3 ML. D?pend beaucoup du CPU et de la m?moire. R?sultats sur corei7, DDR4 1333, non v?rifi?s sur corei3.
          #       Gain de 14 s ? 15 s pour un gros fichier de 15 ML.  
           
               message("correction du temps de travail par recherche binaire")
             # 
             #   microbenchmark::microbenchmark({   
             #      Unique <- lapply(Bulletins.paie, unique)
             #       
             #      `%-%`<- function(x, y) setdiff(Unique[[as.character(substitute(x))]], y)
             #       
             #      setkey(Paie, Heures, Statut, Grade) 
             #       
             #      Paie[..(c(0, NA_real_),
             #              Statut %-% "ELU",
             #              Grade  %-% c("V", "A")), 
             #                `:=`(indic1 = TRUE), nomatch=0]  
             #      
             #      # On est oblig? de segmenter la condition en deux pour ?viter une explosion combinatoire
             #      # Pour cela on pose une indicatrice auxiliaire plus tard effac?e
             #      
             #      setkey(Paie, indic1, Indice, Temps.de.travail)
             #      
             #      Paie[..(TRUE,
             #           Indice %-% c(0, NA_real_),
             #           Temps.de.travail %-% c(0, NA_real_)),
             #              `:=`(indic = TRUE, 
             #                   Heures = round(Temps.de.travail * nb.heures.temps.complet / 100, 1)), nomatch=0]
             #       
             #      Paie[, indic1 := NULL]
             # }, times=1)
        
             # alternative par la m?thode duale des indicatrices sur j:   
               
              # microbenchmark::microbenchmark({   
                 
                 setkey(Paie, Heures) 
                 
                 Paie[.(c(0, NA_real_)), indic := ifelse(Statut != "ELU" & Grade  != "V" & Grade  != "A"  & Indice != 0  & is.na(Indice) & Temps.de.travail != 0 & !is.na(Temps.de.travail), TRUE, NA)
                         , nomatch=0]
                 
                 Paie[.(c(0, NA_real_)), Heures := indic * round(Temps.de.travail * nb.heures.temps.complet / 100, 1)]
                 
               
              # }, times=1)
               
               
           }
     }
    
      # -----
      
      Bulletins.paie[(Heures == 0 | is.na(Heures))
                     & Indice != "" & !is.na(Indice) 
                     & Statut != "ELU" & Grade != "V" & Grade!= "A"
                     & Temps.de.travail != 0 & !is.na(Temps.de.travail), 
                     `:=`(indic = TRUE, 
                          Heures = round(Temps.de.travail * nb.heures.temps.complet / 100, 1))]
      
      message("Correction (m?thode 1), compte tenu des temps complets v?rifi?s, sur ", nredressements <<- nrow(Bulletins.paie[indic == TRUE]), " bulletins de paie")
      
} else {
  
  # on pr?sume alors que les traitements sont correctement liquid?s... il faudrait mettre un drapeau sur cette pr?somption  
  system.time(
    Paie[ , indic := (Heures == 0 | is.na(Heures))
               & Indice != "" & !is.na(Indice) 
               & Statut != "ELU" & Grade != "V" & Grade!= "A"
               & Temps.de.travail != 0 & !is.na(Temps.de.travail)
               & Type == "T" & Montant > 0
               & grepl(".*salaire|trait.*", Libell?, perl=TRUE, ignore.case=TRUE)])
    
  # attention ifelse pas if...else
  # La recherche binaire est 20 fois plus rapide que la recherche vscan (gain de 4s par million de lignes sur corei3)
  
  setkey(Paie, Ann?e, Mois, indic)  
  
  for (A in p?riode) {
      for (M in 1:12) {
            a <- PointMensuelIM[A - 2007, M]  
            Paie[list(A, M, TRUE), 
                   Heures := ifelse(Indice == 0, NA, Montant / (Indice * a * 151.67))]
       }
    }    
  
  
    Bulletins.paie <- merge(unique(Paie[ , .(Matricule, 
                                             Ann?e,
                                             Mois,
                                             Service,
                                             Statut,
                                             Heures,
                                             indic)], by=NULL),
                  Bulletins.paie[, Heures := NULL], 
                  by = c("Matricule","Ann?e","Mois","Service", "Statut"))
    
    message("Correction (m?thode 2), compte tenu des temps complets v?rifi?s, sur ", nredressements <<- nrow(Bulletins.paie[indic == TRUE]), " lignes de paie")
   
}

  Paie[ , Filtre_actif := FALSE]

# TODO: ? revoir pour deux causes : le revenu peut ne pas ?tre un traitement et les heures peuvent ?tre nulles pour
# des temps pleins...

  Paie[ , Filtre_actif := any(Montant[Type == "T" & Heures > minimum.positif] > minimum.actif, na.rm = TRUE),
        by="Matricule,Ann?e"]
  
  Paie[ , delta := 0, by="Matricule,Ann?e,Mois"]
  
  Paie[Type %chin% c("I", "T", "S", "IR", "AC","A", "R", "AV") , 
       delta := sum(Montant,  na.rm=TRUE) - Brut,
       by="Matricule,Ann?e,Mois"]

  # R est le rang (0-based) d?cal? d'une unit? (lag 1)
  
  Bulletins.paie[ , `:=`(Sexe = substr(Nir, 1, 1),
                         R    = .I - 1)]
  
  # Attention, NA, pas FALSE
  
  set(Bulletins.paie, 1, "R", NA)
  
  # M?diane des services horaires ? temps complet par emploi et par sexe 
  
  # La variable Heures des ?lus est non fiable et on peut par convention prendre la quotit? 1
  
  # Pour faciliter les comparaisons de quotit? lors du calcul de la RMPP on arrondit les quotit?s au centi?me inf?rieur
  # Lorsque la d?termin?ation de la m?diane par emploi et sexe du nombre d'heures travaill?es ? temps complet n'est pas positive, la quotit? est ind?finie
  # Une quotit? ne peut pas d?passer 1.
  # Les ?lus sont r?put?s travailler ? temps complet.
  
  message("Calcul des quotit?s")
  
  #on va trouver la plupart du temps 151,67...
  # Tableau de r?f?rence des matrices de m?dianes
  # A ce niveau de g?n?ralit?, le filtre actif est inutile, sauf peut-?tre pour de tr?s petits effectifs.
  
  
  M <- Bulletins.paie[(Sexe == "1" | Sexe == "2"), 
                      .(M?diane_Sexe_Statut = median(Heures, na.rm=TRUE)), by="Sexe,Statut"]
  
  Bulletins.paie <- merge(Bulletins.paie,
                          Paie[ , .(Filtre_actif = Filtre_actif[1]),
                                 by="Matricule,Ann?e,Mois"],
                          all.x=TRUE,
                          all.y=FALSE)
  
  Bulletins.paie[ , pop_calcul_m?diane := length(Heures[Temps.de.travail == 100 
                                                        & !is.na(Heures) 
                                                        & Heures > minimum.positif]),
                  by = "Sexe,Emploi"]
  
  # Pour les quotit?s seules les p?riodes actives sont prises en compte
  
  Bulletins.paie[pop_calcul_m?diane > population_minimale_calcul_m?diane 
                 & Filtre_actif == TRUE, 
                    MHeures :=  median(Heures[Temps.de.travail == 100 
                                                & Filtre_actif == TRUE
                                                & Heures > minimum.positif], na.rm = TRUE),
                 by="Sexe,Emploi"]
  
  B <- Bulletins.paie[pop_calcul_m?diane <= population_minimale_calcul_m?diane 
                 | Filtre_actif == FALSE | is.na(Filtre_actif) | is.na(pop_calcul_m?diane), 
                 .(Sexe, Statut)]
                              
  d?faut_m?diane <- function(X, Y)  {
    
    r <- M[Sexe == X
            & Statut == Y,
               M?diane_Sexe_Statut]
    if (length(r) > 0) r[1] else 0
  }
  
  
  B[ , MHeures := d?faut_m?diane(Sexe, Statut), by = 1:NROW(B)] 
  # ou: B[, MHeures := mapply(d?faut_m?diane, B[["Sexe"]], B[["Statut"]], SIMPLIFY=TRUE, USE.NAMES=FALSE)]
  
  
  # L'?cr?tement des quotit?s est une contrainte statistiquement discutable qui permet de "stresser" le mod?le
  # Par d?faut les quotit?s sont ?cr?t?es pour pouvoir par la suite raisonner en d?finissant le temps plein comme quotit? == 1
  
  if (?creter.quotit?s) {
    Bulletins.paie[ , quotit?   :=  ifelse(MHeures < minimum.positif, NA, ifelse(Heures > MHeures, 1, round(Heures/MHeures, digits = 2)))]  
  } else {
    Bulletins.paie[ , quotit?   :=  ifelse(MHeures < minimum.positif, NA, round(Heures/MHeures, digits = 2))]  
  }
  
  Bulletins.paie[Statut == "ELU", `:=`(MHeures = 1,
                                       quotit? = 1)]
  
  message("Quotit?s calcul?es")
  
  Bulletins.paie[ ,   `:=`(Montant.net.eqtp  = ifelse(is.finite(a<-Net.?.Payer/quotit?), a,  NA),
                           Montant.brut.eqtp = ifelse(is.finite(a<-Brut/quotit?), a,  NA))]
  
  Bulletins.paie[ ,   `:=`(Statut.sortie   = Statut[length(Net.?.Payer)],
                           nb.jours        = calcul.nb.jours.mois(Mois, Ann?e[1]),
                           nb.mois         = length(Mois),
                           cumHeures       = sum(Heures, na.rm = TRUE),
                           quotit?.moyenne = sum(quotit?, na.rm = TRUE) / 12),
                 key=c("Matricule", "Ann?e")]
  
  # Indicatrice pour la r?mun?ration moyenne des personnes en place :
  # quotit? ?gale pendant deux ann?es successives contigues, permanence sur 12 mois.
  # nous prenons les moyennes des quotit?s non NA.
  
  Bulletins.paie[ , indicatrice.quotit?.pp := (Matricule[R] == Matricule 
                                               & Ann?e[R]   == Ann?e - 1 
                                               & abs(quotit?.moyenne[R] - quotit?.moyenne) < tol?rance.variation.quotit?
                                               & nb.mois[R] == nb.mois
                                               & nb.mois    == 12)]
  
  Bulletins.paie[ ,   `:=`(Montant.brut.annuel      = sum(Brut, na.rm = TRUE),
                           Montant.brut.annuel.eqtp = 365 / nb.jours * sum(Montant.brut.eqtp , na.rm = TRUE),
                           Montant.net.annuel.eqtp  = 365 / nb.jours * sum(Montant.net.eqtp, na.rm = TRUE),
                           Montant.net.annuel       = sum(Net.?.Payer, na.rm = TRUE),
                           permanent                = nb.jours >= 365,
                           cumHSup                  = sum(Heures.Sup., na.rm = TRUE), 
                           indicatrice.quotit?.pp   = indicatrice.quotit?.pp[1]),
                 key=c("Matricule", "Ann?e")]
  
  message("Indicatrice RMPP calcul?e")
  
  Paie <- merge(unique(Bulletins.paie[ , .(Matricule, 
                                           Ann?e,
                                           Mois,
                                           Service,
                                           Statut,
                                           cumHeures,
                                           quotit?,
                                           quotit?.moyenne,
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
                                           indicatrice.quotit?.pp,
                                           permanent)], by = NULL),
                Paie, 
                by=c("Matricule","Ann?e","Mois","Service", "Statut"))
  
  matricules <- unique(Bulletins.paie[ , .(Ann?e, Nom, Pr?nom, Matricule, Cat?gorie, Grade, Emploi)], by = NULL)
  
  matricules <- matricules[order(Matricule, Ann?e)]
  
  grades.categories <- unique(matricules[ , .(Grade, Cat?gorie)], by = NULL)
  grades.categories <- grades.categories[order(Grade)]
  
  # on essaie de deviner le versant de la FP par l'existence d'agents de service hospitalier
  # on peut d?sactiver ce test par d?sactiver.test.versant.fp <- T dans prologue.R
  
  if (is.na(VERSANT_FP))
      VERSANT_FP <<-  if (grepl("AG.*HOSP", grades.categories$Grade, ignore.case = TRUE)) "FPH" else "FPT"
  
  
