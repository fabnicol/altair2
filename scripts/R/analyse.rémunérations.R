# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, annees 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser
# les fichiers de paye produits au format spécifié par l'annexe de la  
# convention-cadre de dématérialisation en vigueur à partir de 2008.
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

message("Calcul des rémunérations...")

smic_net    <<- smic.net()

periode.hors.données.smic <- FALSE

if (debut.periode.sous.revue < smic.net.première.annee.renseignée 
    || fin.periode.sous.revue > smic.net.dernière.annee.renseignée) {
  
message("Attention la periode sous revue n'est pas incluse dans la base de données du smic net mensuel.
Actualiser le fichier smic.R dans le dossier scripts/R")

  periode.hors.données.smic <- TRUE  
}

# Si la periode déborde de la periode des données du smic, on prend la moins mauvaise solution qui consiste à retenir la borne la plus proche.
# On ne renvoie donc jamais logical(0) mais toujours un booléen.

smic.net.inf <- smic_net[Annee == smic.net.première.annee.renseignée, SMIC_NET]
smic.net.sup <- smic_net[Annee == smic.net.dernière.annee.renseignée, SMIC_NET]


# cle.fusion = Matricule, en principe (mais pourrait être NIR)
# Sommation : on passe du niveau infra-annuel (détails au mois de niveau Paie) au niveau de la periode (détail de niveau annee)

#+ Table-Analyse.remunerations

Analyse.remunerations <- Paie[ , .(Nir          = Nir[1],
                                   Montant.net.annuel = Montant.net.annuel[1],
                                   Montant.net.annuel.eqtp  = Montant.net.annuel.eqtp[1],
                                   Montant.brut.annuel = Montant.brut.annuel[1],
                                   Montant.brut.annuel.eqtp = Montant.brut.annuel.eqtp[1],
                                   cumHeures    = cumHeures[1],
                                   Statut       = Statut[1],
                                   nb.jours     = nb.jours[1],
                                   nb.mois      = nb.mois[1],
                                   annee_entiere = annee_entiere[1],
                                   ind.quotite  = indicatrice.quotite.pp[1],
                                   Filtre_actif = Filtre_actif[1],
                                   quotite.moyenne = quotite.moyenne[1],
                                   Emploi       = Emploi[1],
                                   Grade        = Grade[1],
                                   Categorie    = Categorie[1],
                                   temps.complet.sur.annee = all(quotite == 1),
                                   Service      = Service[1],
                                   traitement.indiciaire   = sum(Montant[Type == "T"], na.rm = TRUE),
                                   sft          = sum(Montant[Type == "S"], na.rm = TRUE),
                                   indemnité.résidence = sum(Montant[Type == "IR"], na.rm = TRUE),
                                   indemnités   = sum(Montant[Type == "I"], na.rm = TRUE),
                                   cotisations  = sum(Montant[Type == "C"], na.rm = TRUE),
                                   rappels  = sum(Montant[Type == "R"], na.rm = TRUE),
                                   retenues  = sum(Montant[Type == "RE"], na.rm = TRUE),
                                   rémunérations.diverses = sum(Montant[Type == "A"], na.rm = TRUE),
                                   acomptes   = sum(Montant[Type == "AC"], na.rm = TRUE),
                                   rémunération.vacataire = sum(Montant[Type == "VAC"], na.rm = TRUE)), 
                              by = .(Matricule, Annee)]


                                                    
# soit le nombre de jours, avec un nombre d'heure supérieur à 120 à raison d'une heure trente par jour en moyenne
# soit la rémunération totale annuelle gagnée est supérieure à 3 fois le smic (Vérifier_non_annexe)

Analyse.remunerations[ , Filtre_annexe :=  (nb.jours < minimum.Njours.non.annexe
                                                | cumHeures < minimum.Nheures.non.annexe 
                                                | cumHeures / nb.jours < minimum.Nheures.jour.non.annexe)]

#+ Table-Analyse.remunerations-Filtre-annexe

if (periode.hors.données.smic) {
  
  Analyse.remunerations[ 
    ((Annee < smic.net.première.annee.renseignée & Montant.net.annuel < smic.net.inf)
     | (Annee > smic.net.dernière.annee.renseignée & Montant.net.annuel < smic.net.sup)
     | (Annee >= smic.net.première.annee.renseignée 
        & Annee <= smic.net.première.annee.renseignée 
        & Montant.net.annuel < smic_net[Annee - smic.net.première.annee.renseignée + 1, SMIC_NET])),
             Filtre_annexe := TRUE]
                         
} else {
  
  Analyse.remunerations[Montant.net.annuel < smic_net[smic.net.dernière.annee.renseignée - Annee + 1, SMIC_NET],  Filtre_annexe := TRUE]
}
  

Analyse.remunerations[ , rémunération.indemnitaire.imposable := indemnités + sft + indemnité.résidence + rémunérations.diverses]

Analyse.remunerations[ ,
                      `:=`(rémunération.indemnitaire.imposable.eqtp = ifelse(is.finite(q <- Montant.brut.annuel.eqtp / Montant.brut.annuel),
                                                                             q * rémunération.indemnitaire.imposable,
                                                                             NA),
                           
                           total.lignes.paie =  traitement.indiciaire + sft + indemnité.résidence + indemnités + acomptes + rappels,
                           
                           part.rémunération.indemnitaire =  ifelse(is.finite(q <- rémunération.indemnitaire.imposable/Montant.brut.annuel),
                                                                    pmin(q, 1) * 100,
                                                                    NA))]

Analyse.remunerations[ , indemnités.élu := ifelse(Statut == "ELU", total.lignes.paie, 0)]

# Pour analyser les rémunérations, on ne retient que les enregistrements pour lesquels elle est calculable.
# Il ne faudra donc pas utiliser cette table par exemple pour évaluer les effectifs

Analyse.remunerations <- Analyse.remunerations[! is.na(Montant.brut.annuel)]

message("Analyse des rémunérations réalisée.")

# On retire les assistantes maternelles (Grade A), les vacataires (Grade V) les élus les inactifs et les postes annexes

#+ Table-Analyse.variations.par.exercice

Analyse.variations.par.exercice <- Analyse.remunerations[Grade != "A"  
                                                         & Grade != "V" 
                                                         & Statut != "ELU"
                                                         & Filtre_actif == TRUE
                                                         & Filtre_annexe == FALSE,
                                                           .(Matricule, 
                                                             Annee,
                                                             Nir,
                                                             Montant.net.annuel.eqtp,
                                                             Montant.brut.annuel.eqtp,
                                                             rémunération.indemnitaire.imposable.eqtp,
                                                             Statut,
                                                             Grade,
                                                             Categorie,
                                                             nb.jours,
                                                             temps.complet.sur.annee,
                                                             ind.quotite,
                                                             quotite.moyenne,
                                                             annee_entiere)]

# indicatrice binaire annee
# Ex: si Annee = debut.periode.sous.revue + 3, indicatrice.annee = 1 << 3 soit le binaire 1000 = 8 ou encore 2^3
# l'indicatrice d'annee sera utilisée pour l'analyse du GVT 

Analyse.variations.par.exercice[ , indicatrice.annee := bitwShiftL(1, Annee - debut.periode.sous.revue) ]

# <!-- Prologue : enlever.quotites.na, enlever.quotites.nulles (défaut : FALSE)

# On ne retire les quotités nulles et NA que pour l'analyse dynamique de la partie 4 
# On retire également les Heures nulles na et les Heures < seuil.heures

if (enlever.quotites.na) {
  Analyse.variations.par.exercice <- Analyse.variations.par.exercice[! is.na(quotite.moyenne)]
}

if (enlever.quotites.nulles) {
  Analyse.variations.par.exercice <- Analyse.variations.par.exercice[quotite.moyenne > minimum.quotite]
}

#      Prologue -->

# l'indicatrice de periode est la signature de la présence de l'agent sur la periode sous revue :
# elle s'obtient en sommant les indicatrices annee

# 000001 + 000010 + 010000  = 010011  soit une présence les deux premières années et l'avant-dernière.
# indicatrices d'annee = 1, 2 et 16 soit somme de 19 

# Pour cette matrice on retient le statut en fin de periode
# sont considérés comme temps complets ou permanents seulement ceux qui le sont sur l'ensemble de la periode 

#+ Table-Analyse.variations

Analyse.variations <- Analyse.variations.par.exercice[ ,
                                                       .(Annee,
                                                         ind.quotite,
                                                         nb.jours,
                                                         Nexercices = length(Annee),
                                                         Statut,
                                                         Categorie,
                                                         statut.fin.annee = Statut[length(Annee)],
                                                         total.jours = sum(nb.jours, na.rm = TRUE),
                                                         indicatrice.periode = sum(indicatrice.annee),
                                                         quotite.moyenne,
                                                         Montant.net.annuel.eqtp,
                                                         Montant.net.annuel.eqtp.début  = Montant.net.annuel.eqtp[1],
                                                         Montant.net.annuel.eqtp.sortie = Montant.net.annuel.eqtp[length(Annee)],
                                                         periode_entiere = all(annee_entiere),
                                                         temps.complet.sur.periode = all(temps.complet.sur.annee), # sur toute la période
                                                         moyenne.rémunération.annuelle.sur.periode =
                                                           sum(Montant.net.annuel.eqtp, na.rm = TRUE) / length(Annee[!is.na(Montant.net.annuel.eqtp) 
                                                                                                                     & Montant.net.annuel.eqtp > minimum.positif])),
                                                       by = Matricule]

## Important pour la validité de est.rmpp si les années ne sortent pas bien triées de lhx ! ##

setkey(Analyse.variations, Annee)

Analyse.variations[ ,  pris.en.compte := ! is.na(Montant.net.annuel.eqtp.début)
                                    & ! is.na(Montant.net.annuel.eqtp.sortie)
                                    & Montant.net.annuel.eqtp.début  > minimum.positif 
                                    & Montant.net.annuel.eqtp.sortie > minimum.positif ]

Analyse.variations[ ,  variation.rémunération := ifelse(pris.en.compte,
                                                 (Montant.net.annuel.eqtp.sortie / Montant.net.annuel.eqtp.début - 1) * 100,
                                                  NA)]

# La variation de rémunération normalisée se définit comme celle qui correspond à des agents présents en début et en fin d'exercice

Analyse.variations[ , `:=`(variation.moyenne.rémunération = ifelse(pris.en.compte,
                                                                   ((variation.rémunération /100 + 1)^(1 / (Nexercices - 1)) - 1) * 100,
                                                                   NA),
                               
                           variation.rémunération.normalisée = ifelse(duree.sous.revue == Nexercices,   variation.rémunération,  NA))]


Analyse.variations[ ,                                                                 
                            `:=`(variation.moyenne.rémunération.normalisée = ifelse(! is.na(variation.rémunération.normalisée),  
                                                                                  variation.moyenne.rémunération,
                                                                                  NA),
                                 plus.2.ans  = (total.jours  >= 730),  
                                 moins.2.ans = (total.jours < 730),
                                 moins.1.an  = (total.jours < 365),
                                 moins.six.mois = (total.jours < 183))]


# On retranche 1 unité en décalant l'annee en cours pour calculer l'indicatrice de l'annee antérieure
# soit 2^(Annee - 1 - debut.periode.sous.revue) ou bitwShiftL(1, Annee - 1 - debut.periode.sous.revue)
# que l'on compare avec l'indicatrice de periode par un AND binaire (bitwAnd)

# L'opération donne donc 0 si la personne n'a pas été présente l'annee précédente, sinon donne un entier positif
# en combinant avec & : 0 & TRUE = FALSE, 0 & FALSE = FALSE, 4 & TRUE = TRUE, 4 & FALSE = FALSE

Analyse.variations[ , est.rmpp :=  (Annee != debut.periode.sous.revue  
                                        & ! is.na(ind.quotite)
                                        & ind.quotite == TRUE
                                        & bitwAnd(bitwShiftL(1, Annee - 1 - debut.periode.sous.revue),
                                                  indicatrice.periode) != 0)]

message("Analyse des variations réalisée.")

message("Analyse démographique réalisée.")

if (!is.null(Paie) & !is.null(Analyse.remunerations) & !is.null(Analyse.variations.par.exercice))
  message("Statistiques de synthèse réalisées")

sauv.bases("Remunerations", 
           environment(),
           "Analyse.remunerations",
           "Analyse.variations.par.exercice",
           "Analyse.variations")
