
source("smic.R", encoding = encodage.code.source)

période.hors.données.smic <- FALSE

if (début.période.sous.revue < smic.net.première.année.renseignée 
    || fin.période.sous.revue > smic.net.dernière.année.renseignée) {
  
message("Attention la période sous revue n'est pas incluse dans la base de données du smic net mensuel.
Actualiser le fichier smic.R dans le dossier Tests/Exemple")

  période.hors.données.smic <- TRUE  
}

# Si la période déborde de la période des données du smic, on prend la moins mauvaise solution qui consiste à retenir la borne la plus proche.
# On ne renvoie donc jamais logical(0) mais toujours un booléen.

Vérifier_non_annexe <- function(montant, année) {
                                   if (période.hors.données.smic) {
                                     if (année < smic.net.première.année.renseignée)
                                        return(montant > smic.net[Année == smic.net.première.année.renseignée, SMIC_NET])
                                     else if (année > smic.net.dernière.année.renseignée)   
                                        return(montant > smic.net[Année == smic.net.dernière.année.renseignée, SMIC_NET])
                                   } else {
                                     return(montant > smic.net[Année == année, SMIC_NET]) 
                                   }
}


Analyse.rémunérations <- Paie[ , .(Nir          = Nir[1],
                                   Montant.net.annuel = Montant.net.annuel[1],
                                   Montant.net.annuel.eqtp  = Montant.net.annuel.eqtp[1],
                                   Montant.brut.annuel = Montant.brut.annuel[1],
                                   Montant.brut.annuel.eqtp = Montant.brut.annuel.eqtp[1],
                                   cumHeures    = cumHeures[1],
                                   Statut       = Statut[1],
                                   nb.jours     = nb.jours[1],
                                   nb.mois      = nb.mois[1],
                                   permanent    = permanent[1],
                                   ind.quotité  = indicatrice.quotité.pp[1],
                                   Filtre_actif = Filtre_actif[1],
                                   quotité.moyenne = quotité.moyenne[1],
                                   Emploi       = Emploi[1],
                                   Grade        = Grade[1],
                                   Catégorie    = Catégorie[1],
                                   temps.complet = all(quotité == 1),
                                   Service      = Service[1],
                                   traitement.indiciaire   = sum(Montant[Type == "T"], na.rm = TRUE),
                                   sft          = sum(Montant[Type == "S"], na.rm = TRUE),
                                   indemnité.résidence = sum(Montant[Type == "IR"], na.rm = TRUE),
                                   indemnités   = sum(Montant[Type == "I"], na.rm = TRUE),
                                   rémunérations.diverses = sum(Montant[Type == "A"], na.rm = TRUE),
                                   autres.rémunérations   = sum(Montant[Type == "AC" | Type == "A" ], na.rm = TRUE),
                                   rémunération.vacataire = sum(Montant[Type == "VAC"], na.rm = TRUE)),  
                              by = c(clé.fusion, étiquette.année)]

# soit le nombre de mois est supérieur à 1, avec un nombre d'heure supérieur à 120 à raison d'une heure trente par jour en moyenne
# soit la rémunération totale annuelle gagnée est supérieure à 3 fois le smic (Vérifier_non_annexe)

with(Analyse.rémunérations,
     
  Filtre_non_annexe <- mapply(Vérifier_non_annexe,
                                Montant.net.annuel,
                                Année,
                                USE.NAMES = FALSE,
                                SIMPLIFY = TRUE) |
                                    (nb.mois > minimum.Nmois.non.annexe 
                                     & cumHeures > minimum.Nheures.non.annexe 
                                     & cumHeures / nb.jours > minimum.Nheures.jour.non.annexe)
)

Analyse.rémunérations[ , `:=`(rémunération.indemnitaire.imposable = indemnités + sft + indemnité.résidence + rémunérations.diverses,
                              Filtre_actif_non_annexe = (Filtre_actif == TRUE & Filtre_non_annexe == TRUE))]

#Montant.brut.annuel - sft - indemnité.résidence - traitement.indiciaire

Analyse.rémunérations[ ,
                      `:=`(rémunération.indemnitaire.imposable.eqtp = ifelse(is.finite(q <- Montant.brut.annuel.eqtp/Montant.brut.annuel), 
                                                                             q * rémunération.indemnitaire.imposable,
                                                                             NA),
                           
                           total.lignes.paie =  traitement.indiciaire + sft + indemnité.résidence + indemnités + autres.rémunérations,
                           
                           part.rémunération.indemnitaire =  ifelse(is.finite(q <- rémunération.indemnitaire.imposable/Montant.brut.annuel),
                                                                    pmin(q, 1) * 100,
                                                                    NA))]

Analyse.rémunérations[ , indemnités.élu := ifelse(Statut == "ELU", total.lignes.paie, 0)]

Analyse.rémunérations <- Analyse.rémunérations[! is.na(Montant.brut.annuel)]

message("Analyse des rémunérations réalisée.")

Analyse.variations.par.exercice <- Analyse.rémunérations[Grade != "A"  
                                                         & Grade != "V" 
                                                         & Statut != "ELU"
                                                         & Filtre_actif_non_annexe == TRUE,
                                                         c(clé.fusion, étiquette.année,
                                                           "Montant.net.annuel.eqtp",
                                                           "Montant.brut.annuel.eqtp",
                                                           "rémunération.indemnitaire.imposable.eqtp",
                                                           "Statut",
                                                           "Grade",
                                                           "Catégorie",
                                                           "nb.jours",
                                                           "temps.complet",
                                                           "ind.quotité",
                                                           "quotité.moyenne",
                                                           "permanent"), with=FALSE]

Analyse.variations.par.exercice[ , indicatrice.année := bitwShiftL(1, Année - début.période.sous.revue) ]


# On ne retire les quotités nulles et NA que pour l'analyse dynamique de la partie 4 
# On retire également les Heures nulles na et les Heures < seuil.heures

if (enlever.quotités.na) {
  Analyse.variations.par.exercice <- Analyse.variations.par.exercice[! is.na(quotité.moyenne)]
}

if (enlever.quotités.nulles) {
  Analyse.variations.par.exercice <- Analyse.variations.par.exercice[quotité.moyenne > minimum.quotité]
}

Analyse.variations.synthèse <- Analyse.variations.par.exercice[ ,
                                                               .(Nexercices = length(Année),
                                                                 statut = Statut[length(Année)],
                                                                 total.jours = sum(nb.jours, na.rm = TRUE),
                                                                 indicatrice.période = sum(indicatrice.année),
                                                                 Montant.net.annuel.eqtp.début  = Montant.net.annuel.eqtp[1],
                                                                 Montant.net.annuel.eqtp.sortie = Montant.net.annuel.eqtp[length(Année)],
                                                                 permanent = all(permanent),
                                                                 temps.complet = all(temps.complet),
                                                                 moyenne.rémunération.annuelle.sur.période =
                                                                   sum(Montant.net.annuel.eqtp, na.rm = TRUE)/length(Année[!is.na(Montant.net.annuel.eqtp) 
                                                                                                                           & Montant.net.annuel.eqtp > minimum.positif])),
                                                               by = clé.fusion]

Analyse.variations.synthèse[ ,  pris.en.compte := ! is.na(Montant.net.annuel.eqtp.début)
                            & ! is.na(Montant.net.annuel.eqtp.sortie)
                            & Montant.net.annuel.eqtp.début  > minimum.positif 
                            & Montant.net.annuel.eqtp.sortie > minimum.positif ]

Analyse.variations.synthèse[ ,  variation.rémunération := ifelse(pris.en.compte,
                                                                 (Montant.net.annuel.eqtp.sortie / Montant.net.annuel.eqtp.début - 1)*100,
                                                                 NA)]

Analyse.variations.synthèse[ ,                                            
                            `:=`(variation.moyenne.rémunération = ifelse(pris.en.compte,
                                                                         ((variation.rémunération/100 + 1)^(1 / (Nexercices - 1)) - 1) * 100,
                                                                         NA),
                                 
                                 variation.rémunération.normalisée = ifelse(durée.sous.revue == Nexercices,
                                                                            variation.rémunération,
                                                                            NA))]


Analyse.variations.synthèse[ ,                                                                 
                            `:=`(variation.moyenne.rémunération.normalisée = ifelse(!is.na(variation.rémunération.normalisée),
                                                                                    variation.moyenne.rémunération,
                                                                                    NA),
                                 plus.2.ans  = (total.jours  >= 730),  
                                 moins.2.ans = (total.jours < 730),
                                 moins.1.an  = (total.jours < 365),
                                 moins.six.mois = (total.jours < 183))]

# Note : sous environnement knitr/spin, data.table parvient mal à identifier les noms locaux,
# ce qui ne pose pas de problème en environnement standard. Il faut donc rajouter le préfixe de base dans ce cas.


temp2 <- Analyse.variations.synthèse[ , .(Matricule, indicatrice.période, pris.en.compte, Nexercices, plus.2.ans)]


Analyse.variations.par.exercice <- merge(Analyse.variations.par.exercice, temp2, by="Matricule")

rm(temp2)

Analyse.variations.par.exercice[ , est.rmpp :=  Année != début.période.sous.revue  
                                & ! is.na(ind.quotité)
                                &  ind.quotité == TRUE
                                & bitwAnd(bitwShiftL(1, Année - 1 - début.période.sous.revue),
                                          indicatrice.période) != 0]


Analyse.variations.synthèse.plus.2.ans  <- data.frame(NULL)
Analyse.variations.synthèse.moins.2.ans <- data.frame(NULL)

message("Analyse des variations réalisée.")

message("Analyse démographique réalisée.")

if (!is.null(Paie) & !is.null(Analyse.rémunérations)
    & !is.null(Analyse.variations.synthèse) & !is.null(Analyse.variations.par.exercice))
  message("Statistiques de synthèse réalisées")
message("Démographie...")
