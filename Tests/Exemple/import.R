
library(knitr)
library(ggplot2)
library(assertthat)
library(gtools)
library(data.table)

## Pour les versions de data.table < 1.9.5 
##  if (séparateur.décimal.entrée != ".")
##  stop("Pour les tables importées par data.table::fread, le séparateur décimal doit être '.'")

if (séparateur.décimal.entrée == séparateur.liste.entrée)
  stop("Le séparateur décimal en entrée doit être différent du séparateur de colonnes !")

if (séparateur.décimal.sortie == séparateur.liste.sortie)
  stop("Le séparateur décimal en sortie doit être différent du séparateur de colonnes !")

if (sauvegarder.bases.analyse) {
  for (path in c("Rémunérations", "Effectifs", "Réglementation", "Fiabilité"))
    dir.create(file.path(chemin.dossier.bases, path), recursive = TRUE)
}

if (sauvegarder.bases.origine)
  dir.create(file.path(chemin.dossier.bases, "Paiements"), recursive = TRUE)

# problème temporaire avec l'option fig.retina depuis fin mai 2014

knitr::opts_chunk$set(fig.width = 7.5, echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')

# Contrôle de cohérence
#  on vérifie que chaque code de paie est associé, dans le fichier des codes de paiement (par défaut, racinecodes.csv),
#  que à chaque code donné on a associé un et un seul type de rémunération ("INDEMNITAIRE", "TRAITEMENT", etc.)
# Pour le mode rapide, convertir les fichiers base en UTF-8 SANS BOM (par exemple, notepad++ après Excel)

 
fichier.personnels.existe <- (charger.catégories.personnel == TRUE) & file.exists(chemin(nom.fichier.personnels))

if (fichier.personnels.existe) {
  base.personnels.catégorie <- read.csv.skip(nom.fichier.personnels, séparateur.liste = séparateur.liste.entrée, séparateur.décimal = séparateur.décimal.entrée)
  message("Chargement du fichier des catégories statutaires des personnels.")
}

# Lignes de paie
# On peut lire jusqu'à 50 fichiers csv de lignes de paie qui seront générés au format :

# "chemin dossier + racine-Lignes de Paie-j.csv" où racine est un bref identifiant du type de contrôle (exemple : "c2a-", "PEV-", ...)

lignes.paie <- nom.fichier.paie %+% "-" %+% 1:50 %+% ".csv"
lignes.paie <- lignes.paie[file.exists(chemin(lignes.paie))]

# Bulletins de paie
# On peut lire jusqu'à 10 fichiers csv de bulletins de paie qui seront générés au format :

# "chemin dossier + racine-Bulletins de Paie-j.csv"

bulletins.paie <- nom.bulletin.paie %+% "-" %+% 1:10 %+% ".csv"
bulletins.paie <- bulletins.paie[file.exists(chemin(bulletins.paie))]
nom.table      <- nom.table[file.exists(chemin(nom.table))]

# Programme principal

# Bases

# Lignes de paie

# On ne retient que les bases ayant pour années au minimum début.période.sous.revue
# et au maximum fin.période.sous.revue, qui contiennent toutes les colonnes requises
# pour le contrôle

# Le mode rapide n'est disponible que avec des csv à séparateurs virgule
# Il permet d'économiser environ 8s par million de ligne lues sur une dizaine de champs


importer.bases.via.xhl2csv <- function(base, table = nom.table, colClasses = colonnes.classes.input, colNames =  colonnes.input) {
  
  res <- try(Read.csv(base,
                      table,
                      colClasses = colClasses,
                      colNames = colNames,
                      séparateur.liste = séparateur.liste.entrée,
                      séparateur.décimal = séparateur.décimal.entrée,
                      convertir.encodage = (encodage.entrée.xhl2csv != "UTF-8"),
                      encodage = encodage.entrée.xhl2csv,
                      rapide = TRUE),
             silent = FALSE)
  
  if (inherits(res, 'try-error'))
    stop("Problème de lecture de la base de la table bulletins-lignes de Paie")
  
  if (!is.null(Paie)) {
    message("Chargement de la table bulletins-lignes de Paie.")
  } else {
    stop("Chargement de la table bulletins-lignes de paie en échec.")
  }
  
  message("Chargement direct des bulletins et lignes de paie")
}


if (charger.bases) {
  importer.bases.via.xhl2csv("Paie",colClasses =  colonnes.classes.input)
  importer.bases.via.xhl2csv("Bulletins.paie", nom.bulletins, colClasses =  colonnes.bulletins.classes.input, colNames = colonnes.bulletins.input)
  
  # dans le cas où l'on ne lance le programme que pour certaines années, il préciser début.période sous revue et fin.période .sous.revue
  # dans le fichier prologue.R. Sinon le programme travaille sur l'ensemble des années disponibles.
  
  if (extraire.années) {
    Paie <- Paie[Année >= début.période.sous.revue & Année <= fin.période.sous.revue, ]
    Bulletins.paie <- Bulletins.paie[Année >= début.période.sous.revue & Année <= fin.période.sous.revue, ]
  } else {
    début.période.sous.revue <- min(Paie[[1]])
    fin.période.sous.revue   <- max(Paie[[1]])
  }
  
  
  Paie[is.na(Grade),  Grade  := ""]
  Paie[is.na(Statut), Statut := "AUTRE_STATUT"]
  Paie[is.na(NBI),    NBI    := 0]
  Bulletins.paie[is.na(Grade),  Grade  := ""]
  Bulletins.paie[is.na(Statut), Statut := "AUTRE_STATUT"]
  Bulletins.paie[is.na(NBI),    NBI    := 0]
}

période                 <- début.période.sous.revue:fin.période.sous.revue
durée.sous.revue        <- fin.période.sous.revue - début.période.sous.revue + 1

if (! analyse.statique.totale) {
  
  années.analyse.statique <- c(début.période.sous.revue, fin.période.sous.revue)
  
} else {
  
  années.analyse.statique <- période
}

setkey(Paie, Matricule, Année, Mois)
setkey(Bulletins.paie, Matricule, Année, Mois)


# Le format est jour/mois/année avec deux chiffres-séparateur-deux chiffres-séparateur-4 chiffres.
# Le séparateur peut être changé en un autre en modifiant le "/" dans date.format

avant.redressement <- 0
après.redressement <- 0
  
if (éliminer.duplications) {
  avant.redressement <- nrow(Paie)
  duplications.vecteur <- duplicated(Paie, by=NULL)
  duplications.paie <- Paie[duplications.vecteur]
  Paie <- Paie[! duplications.vecteur] 
  if (sauvegarder.bases.origine)
      sauv.bases(chemin.dossier.bases, "duplications.paie")
  après.redressement <- nrow(Paie)
  
  avant.redressement.bull <- nrow(Bulletins.paie)
  duplications.vecteur   <- duplicated(Bulletins.paie, by=NULL)
  duplications.paie.bull <- Bulletins.paie[duplications.vecteur]
  Bulletins.paie <- Bulletins.paie[! duplications.vecteur] 
  if (sauvegarder.bases.origine) {
    sauv.bases(chemin.dossier.bases, "duplications.paie")
    sauv.bases(chemin.dossier.bases, "duplications.paie.bull")
  }
  après.redressement.bull <- nrow(Bulletins.paie)
  rm(duplications.vecteur)
  
} 
  


# Lors de la PREMIERE utilisation d'Altair, paramétrer générer.codes <- TRUE dans prologue.R
# pour générer les fichier des codes de paiement sous le dossier des bases (par défaut "Données").
# ce fichier est trier par ordre croissant des codes de paiement sur les trois premiers chiffres des codes
# des anomalies peuvent résiduellement apparaître avec des codes contenant des lettres, en général après
# le troisième chiffre du code.
# L'utilisateur devra alors renseigner la colonne étiquette.type.rémunération de ce fichier

if (générer.codes)   {
  source("générer.codes.R", encoding = encodage.code.source)
  générer.base.codes(Paie) 
}

if (charger.bases) {
  
  Paie[ , Filtre_actif := any(Montant[Type == "T" & Heures > minimum.positif] > minimum.actif, na.rm = TRUE), by="Matricule,Année"]
  
  Paie[ , delta := 0, by="Matricule,Année,Mois"]
  
  Paie[Type %chin% c("I", "T", "S", "IR", "AC","A", "R", "AV") , delta := sum(Montant,  na.rm=TRUE) - Brut, by="Matricule,Année,Mois"]
  
  #Bulletins.paie <- unique(Paie[ , .(Matricule, Nom, Année, Mois, Temps.de.travail, Heures,  Statut, Emploi, Grade, Brut, Net.à.Payer, Nir)], by = NULL)
  
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
  
  
  M <- Bulletins.paie[(Sexe == "1" | Sexe == "2") & Heures > minimum.positif, .(Médiane_Sexe_Statut = median(Heures, na.rm=TRUE)), by="Sexe,Statut"]
  
  Bulletins.paie <- merge(Bulletins.paie, Paie[, .(Filtre_actif=Filtre_actif[1]), by="Matricule,Année,Mois"], all.x=TRUE, all.y=FALSE)
  
  Bulletins.paie[ , pop_calcul_médiane := length(Heures[Temps.de.travail == 100 
                                                        & !is.na(Heures) 
                                                        & Heures > minimum.positif]), by = "Sexe,Emploi"]
  
  # Pour les quotités seules les périodes actives sont prises en compte
  
  Bulletins.paie[ , MHeures := ifelse(pop_calcul_médiane > population_minimale_calcul_médiane 
                                      & Filtre_actif == TRUE,
                                      median(Heures[Temps.de.travail == 100 
                                                    & Filtre_actif == TRUE
                                                    & Heures > minimum.positif], na.rm = TRUE),
                                      M[M$Sexe == Bulletins.paie$Sexe
                                        & M$Statut == Bulletins.paie$Statut,
                                        Médiane_Sexe_Statut]),
                 by="Sexe,Emploi"]
  
  # L'écrêtement des quotités est une contrainte statistiquement discutable qui permet de "stresser" le modèle
  # Par défaut les quotités sont écrêtées pour pouvoir par la suite raisonner en définissant le temps plein comme quotité == 1
  
  if (écreter.quotités) {
    Bulletins.paie[ , quotité   :=  ifelse(MHeures < minimum.positif, NA, ifelse(Heures > MHeures, 1, round(Heures/MHeures, digits=2)))]  
  } else {
    Bulletins.paie[ , quotité   :=  ifelse(MHeures < minimum.positif, NA, round(Heures/MHeures, digits=2))]  
  }
  
  Bulletins.paie[Statut == "ELU", `:=`(MHeures = 1,
                                       quotité = 1)]
  
  message("Quotités calculées")
  
  Bulletins.paie[ ,   `:=`(Montant.net.eqtp  = ifelse(is.finite(a<-Net.à.Payer/quotité), a,  NA),
                           Montant.brut.eqtp = ifelse(is.finite(a<-Brut/quotité), a,  NA))]
  
  Bulletins.paie[ ,   `:=`(Statut.sortie   = Statut[length(Net.à.Payer)],
                           nb.jours        = calcul.nb.jours.mois(Mois, Année[1]),
                           nb.mois         = length(Mois),
                           cumHeures       = sum(Heures, na.rm = TRUE),
                           quotité.moyenne = round(mean.default(quotité, na.rm = TRUE), digits = 1)),
                 key=c("Matricule", "Année")]
  
  # Indicatrice pour la rémunération moyenne des personnes en place :
  # quotité égale pendant deux années successives contigues, permanence sur 12 mois.
  # nous prenons les moyennes des quotités non NA.
  
  Bulletins.paie[ , indicatrice.quotité.pp := (Matricule[R] == Matricule 
                                               & Année[R]   == Année - 1 
                                               & quotité.moyenne[R] == quotité.moyenne
                                               & nb.mois[R] == nb.mois
                                               & nb.mois    == 12)]
  
  Bulletins.paie[ ,   `:=`(Montant.brut.annuel      = sum(Brut, na.rm=TRUE),
                           Montant.brut.annuel.eqtp = sum(Montant.brut.eqtp * 365 / nb.jours, na.rm=TRUE),
                           Montant.net.annuel.eqtp  = sum(Montant.net.eqtp * 365 / nb.jours, na.rm=TRUE),
                           Montant.net.annuel       = sum(Net.à.Payer, na.rm=TRUE),
                           permanent                = nb.jours >= 365,
                           cumHSup      = sum(Heures.Sup., na.rm = TRUE), 
                           indicatrice.quotité.pp = indicatrice.quotité.pp[1]),
                 key=c("Matricule", "Année")]
  
  message("Indicatrice RMPP calculée")
  
  # Obsolète
  
  # Bulletins.paie.réduit <- unique(Bulletins.paie[ , .(Matricule, Année, quotité.moyenne)], by = NULL)
  # 
  # Bulletins.paie.réduit <- Bulletins.paie.réduit[ , nb.années := length(Année), by="Matricule"]
  # 
  # indicatrice.quotité <- function(matricule, année)  Bulletins.paie.réduit[Matricule == matricule 
  #                                                                          & Année == année, 
  #                                                                            quotité.moyenne][1] ==  Bulletins.paie[Matricule == matricule
  #                                                                                                                   & (Année == année - 1),
  #                                                                                                                     quotité.moyenne][1]
  #                                                   
  # 
  # Bulletins.paie <- merge(Bulletins.paie, cbind(Bulletins.paie.réduit[ , .(Matricule, Année, nb.années)],
  #                                               indicatrice.quotité.pp = mapply(indicatrice.quotité,
  #                                                              Bulletins.paie.réduit[ , Matricule], 
  #                                                              Bulletins.paie.réduit[ , Année],
  #                                                              USE.NAMES = FALSE)),
  #                         by = c("Matricule", "Année"))
  # 
  # delta<-Bulletins.paie[indic.rmpp != indicatrice.quotité.pp, .(Matricule, Année, Mois, quotité, quotité.moyenne, indic.rmpp, indicatrice.quotité.pp, R)]
  # 
  # sauv.bases(dossier = chemin.dossier.bases, "delta")
  # stop("test")
  
  Paie <- merge(unique(Bulletins.paie[ , c("Matricule", 
                                           "Année",
                                           "Mois",
                                           "Service",
                                           "cumHeures",
                                           "quotité",
                                           "quotité.moyenne",
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
                                           "indicatrice.quotité.pp",
                                           "permanent"), with=FALSE], by=NULL),
                Paie, 
                by=c("Matricule","Année","Mois","Service"))
  
  matricules <- unique(Bulletins.paie[ ,
                                      c("Année",
                                        "Emploi",
                                        "Nom",
                                        "Matricule"), 
                                      with=FALSE], by=NULL)
  
  if (fichier.personnels.existe) {
    matricules <- merge(matricules, base.personnels.catégorie, by = clé.fusion, all=TRUE)
  } else {
    Catégorie <- character(length = nrow(matricules))
    matricules <- cbind(matricules, Catégorie)
  }
  
  matricules <- matricules[order(Matricule,  Année), ]
  
  message("Bulletins de Paie retraités")
  
} # if (charger.bases)
