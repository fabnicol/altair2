
#' @export
#'
calcul_NBI <- function() {
  
    "Paie_NBI"   %a% filtrer_Paie("NBI", 
                               portée = "Mois", 
                               indic = TRUE)[Type %chin% c("T", "I", "R")]
    
    "lignes_NBI" %a% Paie_NBI[indic == TRUE][ , indic :=  NULL]
    
    "NBI.aux.non.titulaires" %a% lignes_NBI[! Statut %chin% c("TITULAIRE", "STAGIAIRE") 
                                             & NBI != 0,
                                               c(..colonnes, "NBI")]
    
    if ("nombre.personnels.nbi.nontit" %a% uniqueN(NBI.aux.non.titulaires$Matricule)) {
      cat("Il existe ", 
          FR(nombre.personnels.nbi.nontit),
          "non titulaire" %s% nombre.personnels.nbi.nontit,
          " percevant une NBI.")
    } else {
      cat("Pas de NBI apparemment attribuée à des non-titulaires. ")
    }
    
    # On calcule tout d'abord la somme de points de NBI par matricule et par année
    # On calcule ensuite, sur les traitements et éventuellement (par abus ou erreur de codage) les indemnités, la somme des paiements au titre de la NBI, par matricule et par année
    # Attention ne pas prendre en compte les déductions, retenues et cotisations. On compare en effet les payements bruts de base à la somme des points x valeur du point
    
    # La quotité ici considérée est non pas la quotité statistique mais la quotité administrative plus favorable aux temps partiels.
    
    T1 <- lignes_NBI[! is.na(NBI)
                     & NBI != 0,  .(Matricule, 
                                    Nom,
                                    Prénom,
                                    Grade,
                                    Statut,
                                    Année,
                                    Mois,
                                    Montant,
                                    Début,
                                    quotité,
                                    NBI,
                                    Type)
                     ][ , `:=` (Année.rappel = as.numeric(substr(Début, 0, 4)),
                                Mois.rappel  = as.numeric(substr(Début, 6, 7))) 
                        ][ , Début := NULL]
    
    if (nrow(T1) == 0) cat("Aucune NBI n'a été attribuée ou les points de NBI n'ont pas été rencensés en base de paye. ")
    

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
    
    "cumuls.nbi" %a% T2[ , .(cumul.annuel.indiciaire = sum(nbi.cum.indiciaire, na.rm = TRUE),
                          cumul.annuel.montants   = sum(nbi.eqtp.tot, na.rm = TRUE)),
                      by = "Année"]
    
    if (nrow(cumuls.nbi) == 0) cat("Cumuls de NBI nuls. ")
    
    # On somme ensuite par année sur tous les matricules
    # Les cumuls annuels rapportés aux cumuls indiciaires pour l'année ne doivent pas trop s'écarter de la valeur annuelle moyenne du point d'indice
    
    # Techniquement, rajouter un by = .(Année, Mois) accélère la computation
    
    "lignes.nbi.anormales" %a% T2[nbi.cum.indiciaire > 0 
                               & nbi.eqtp.tot > 0,
                               test := nbi.eqtp.tot/nbi.cum.indiciaire - PointMensuelIM[Année - 2007, Mois],
                               by = .(Année, Mois)
                               ][! is.na(test) & abs(test) > 1
                                 ][ , cout.nbi.anormale := (nbi.eqtp.tot - nbi.cum.indiciaire * PointMensuelIM[Année - 2007, Mois]) * adm.quotité]
    
    "couts.nbi.anormales" %a% lignes.nbi.anormales[ , sum(cout.nbi.anormale, na.rm = TRUE)]
    
    "lignes.nbi.anormales.hors.rappels" %a% T2[nbi.cum.indiciaire > 0 
                                            & nbi.cum.hors.rappels > 0,
                                            test := nbi.cum.hors.rappels/(adm.quotité * nbi.cum.indiciaire) - PointMensuelIM[Année - 2007, Mois], by= .(Année, Mois)
                                            ][! is.na(test) & abs(test) > 1
                                            ][ , cout.nbi.anormale := nbi.cum.hors.rappels - nbi.cum.indiciaire * PointMensuelIM[Année - 2007, Mois] * adm.quotité]
    
    "couts.nbi.anormales.hors.rappels" %a% lignes.nbi.anormales.hors.rappels[ , sum(cout.nbi.anormale, na.rm = TRUE)]
    
    "rappels.nbi" %a% T2a[ , sum(nbi.cum.rappels, na.rm = TRUE)]
    
    sauv.bases(file.path(chemin.dossier.bases, "Reglementation"), 
               environment(),
               "NBI.aux.non.titulaires")
    
    sauv.bases(file.path(chemin.dossier.bases, "Fiabilite"), 
               environment(),
               "lignes.nbi.anormales",
               "lignes.nbi.anormales.hors.rappels",
               "cumuls.nbi")
}

#' @export

proratisation_NBI <- function() {
  
  "montants.nbi.anormales.mensuel" %a% 0
  "lignes.nbi.anormales.mensuel" %a% data.table()
  
  essayer({ 
    
  "lignes.nbi.anormales.mensuel" %a% lignes_NBI[Type != "R", .(Montant.NBI.calculé = NBI[1] * adm(quotité[1]) * PointMensuelIM[Année - 2007, Mois],
                                                                      Montant.NBI.payé = sum(Montant, na.rm = TRUE)), 
                                                       by = .(Matricule, Année, Mois)
                                                       ][ , Différence.payé.calculé := Montant.NBI.payé - Montant.NBI.calculé
                                                          ][abs(Différence.payé.calculé) > tolérance.nbi]
  
  "lignes.paie.nbi.anormales.mensuel" %a% Paie_NBI[ , .(Matricule, Nom, Prénom, Grade, Statut, 
                                                     Année, Mois, Echelon, Catégorie, 
                                                     Emploi, Service, Temps.de.travail,
                                                     NBI, Code, Libellé,
                                                     Base, Taux,Type, Montant)
                                                 ][lignes.nbi.anormales.mensuel,
                                                   nomatch = 0,
                                                   on = .(Matricule, Année, Mois)]
  
  "nb.lignes.anormales.mensuel"    %a% nrow(lignes.nbi.anormales.mensuel)
  "montants.nbi.anormales.mensuel" %a% lignes.nbi.anormales.mensuel[, sum(Différence.payé.calculé, na.rm = TRUE)]
  },
  "La vérification de la proratisation de la NBI n'a pas pu être réalisée. ")
  
  print(Tableau(
    c("Différences > " %+% tolérance.nbi %+% " euro : nombre de lignes",
      "Coût total des différences"),
    nrow(lignes.nbi.anormales.mensuel),
    round(montants.nbi.anormales.mensuel)))
  
  sauv.bases(file.path(chemin.dossier.bases, "Fiabilite"), 
             environment(),
             "lignes.nbi.anormales.mensuel",
             "lignes.paie.nbi.anormales.mensuel")
}

#' @export

catégories_NBI <- function() {
  
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
  
  "NBI.cat.irreg" %a% NBI.cat[Contrôle == "Rouge", 
                           Coût := { a <- adm(quotité)
                           (NBI - ifelse(Catégorie == "A", 50, ifelse(Catégorie == "B", 30, 20))) *
                             PointMensuelIM[Année - 2007, Mois] * ifelse(is.na(a), 1, a)}
                           ][! is.na(Coût)
                             ][ , Contrôle := NULL] 
  
  "nombre.mat.NBI.irrég" %a% NBI.cat.irreg[ , uniqueN(Matricule)]
  
  coût.total <- NBI.cat.irreg[ , sum(Coût, na.rm = TRUE)]
  
  print(Tableau(
    c("Nombre d'agents concernés",
      "Coût total des dépassements"),
    nombre.mat.NBI.irrég,
    round(coût.total, 1)))
  
  sauv.bases(file.path(chemin.dossier.bases, "Reglementation"), 
             .GlobalEnv,
             "NBI.cat.irreg")
  
}

