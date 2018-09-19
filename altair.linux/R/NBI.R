
#' @export
#'
calcul_NBI <- function() {
  
    "Paie_NBI"   %a% filtrer_Paie("NBI", 
                               portée = "Mois", 
                               indic = TRUE)[Type %chin% c("T", "I", "R")]
    
    "lignes_NBI" %a% Paie_NBI[indic == TRUE][ , indic :=  NULL]
    
    "NBI_dec" %a% Paie_NBI[NBI != as.integer(NBI)]
    
    if (nrow(NBI_dec)) {
      
      cat("Des NBI décimales ont été attribuées. Les NBI sont généralement en points entiers.   \n")
       
      nombre.personnels.nbi.dec <- uniqueN(NBI_dec$Matricule)
      
      cat("Il existe ", 
            FR(nombre.personnels.nbi.dec),
            "agent" %s% nombre.personnels.nbi.dec,
            " percevant une NBI en points non entiers.   \n")
      } else {
        cat("Pas de NBI en points non entiers.  \n")
    }  
    
    
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
    
    # La quotite ici considérée est non pas la quotite statistique mais la quotite administrative plus favorable aux temps partiels.
    
    T1 <- lignes_NBI[! is.na(NBI)
                     & NBI != 0,  .(Matricule, 
                                    Nom,
                                    Prenom,
                                    Grade,
                                    Statut,
                                    Annee,
                                    Mois,
                                    Montant,
                                    Debut,
                                    quotite,
                                    NBI,
                                    Type)
                     ][ , `:=` (Annee.rappel = as.numeric(substr(Debut, 0, 4)),
                                Mois.rappel  = as.numeric(substr(Debut, 6, 7))) 
                        ][ , Debut := NULL]
    
    if (nrow(T1) == 0) cat("Aucune NBI n'a été attribuée ou les points de NBI n'ont pas été rencensés en base de paye. ")
    

    T2a <- T1[! is.na(quotite)
              & quotite > 0
              & Type == "R",
              .(nbi.cum.rappels = sum(Montant, na.rm = TRUE)), 
              by= .(Matricule, Annee.rappel, Mois.rappel, Annee, Mois)
              ][Annee.rappel >= début.période.sous.revue 
                & Mois.rappel >=1 
                & Mois.rappel <= 12]
    
    setnames(T2a, "Annee", "Annee.R")
    setnames(T2a, "Mois", "Mois.R")
    setnames(T2a, "Annee.rappel", "Annee")
    setnames(T2a, "Mois.rappel", "Mois")
    
    # Il faut éviter la réduplication de rappels sur le mêm mois qui après jointure causera une réduplication fautive des montants
    
    T2b <- T1[! is.na(quotite)
              & quotite > 0
              & Type != "R", 
              .(nbi.cum.hors.rappels = sum(Montant, na.rm = TRUE),
                quotite = quotite[1],
                nbi.cum.indiciaire = NBI[1]), 
              by= .(Matricule, Annee, Mois)]
    
    T2 <- merge(T2a, T2b, 
                all = TRUE,
                by = c("Matricule", "Annee", "Mois"))[ , adm.quotite := adm(quotite)
                                                       ][is.na(nbi.cum.rappels), nbi.cum.rappels := 0
                                                         ][is.na(nbi.cum.hors.rappels), nbi.cum.hors.rappels := 0]
    
    T2[adm.quotite > 0,  nbi.eqtp.tot := (nbi.cum.rappels + nbi.cum.hors.rappels) / adm.quotite]
    
    "cumuls.nbi" %a% T2[ , .(cumul.annuel.indiciaire = sum(nbi.cum.indiciaire, na.rm = TRUE),
                          cumul.annuel.montants   = sum(nbi.eqtp.tot, na.rm = TRUE)),
                      by = "Annee"]
    
    if (nrow(cumuls.nbi) == 0) cat("Cumuls de NBI nuls. ")
    
    # On somme ensuite par année sur tous les matricules
    # Les cumuls annuels rapportés aux cumuls indiciaires pour l'année ne doivent pas trop s'écarter de la valeur annuelle moyenne du point d'indice
    
    # Techniquement, rajouter un by = .(Annee, Mois) accélère la computation
    
    "lignes.nbi.anormales" %a% T2[nbi.cum.indiciaire > 0 
                               &  adm.quotite > 0    
                               & nbi.eqtp.tot > 0]
    
    cout <- function(x, y, z, Annee, Mois) round(x - y * PointMensuelIM[Annee - 2007, Mois] * z, 1 )
      
    "lignes.nbi.anormales" %a% cbind(lignes.nbi.anormales, cout.nbi.anormale = mapply(cout, 
                                                                        lignes.nbi.anormales[["nbi.eqtp.tot"]],
                                                                        lignes.nbi.anormales[["nbi.cum.indiciaire"]],
                                                                        lignes.nbi.anormales[["adm.quotite"]],
                                                                        lignes.nbi.anormales[["Annee"]],
                                                                        lignes.nbi.anormales[["Mois"]]))
    
    "lignes.nbi.anormales" %a% lignes.nbi.anormales[cout.nbi.anormale > adm.quotite * nbi.cum.indiciaire
                                                   ][ , cout.nbi.anormale, by= .(Annee, Mois)]
    
    "couts.nbi.anormales" %a% lignes.nbi.anormales[ , sum(cout.nbi.anormale, na.rm = TRUE)]
    
    "lignes.nbi.anormales.hors.rappels" %a% T2[nbi.cum.indiciaire > 0 
                                               & nbi.cum.hors.rappels > 0]
    
    "lignes.nbi.anormales.hors.rappels" %a% cbind(lignes.nbi.anormales.hors.rappels, cout.nbi.anormale = mapply(cout, 
                                                                                      lignes.nbi.anormales.hors.rappels[["nbi.eqtp.tot"]],
                                                                                      lignes.nbi.anormales.hors.rappels[["nbi.cum.indiciaire"]],
                                                                                      lignes.nbi.anormales.hors.rappels[["adm.quotite"]],
                                                                                      lignes.nbi.anormales.hors.rappels[["Annee"]],
                                                                                      lignes.nbi.anormales.hors.rappels[["Mois"]]))
    
    "lignes.nbi.anormales.hors.rappels" %a% lignes.nbi.anormales.hors.rappels[cout.nbi.anormale > adm.quotite * nbi.cum.indiciaire
                                            ][ , .(cout.nbi.anormale = nbi.cum.hors.rappels - nbi.cum.indiciaire * PointMensuelIM[Annee - 2007, Mois] * adm.quotite), by= .(Annee, Mois)]
    
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
    
  "lignes.nbi.anormales.mensuel" %a% lignes_NBI[Type != "R", .(Montant.NBI.calculé = NBI[1] * adm(quotite[1]) * PointMensuelIM[Annee - 2007, Mois],
                                                                      Montant.NBI.payé = sum(Montant, na.rm = TRUE)), 
                                                       by = .(Matricule, Annee, Mois)
                                                       ][ , Différence.payé.calculé := Montant.NBI.payé - Montant.NBI.calculé
                                                          ][abs(Différence.payé.calculé) > tolérance.nbi]
  
  "lignes.paie.nbi.anormales.mensuel" %a% Paie_NBI[ , .(Matricule, Nom, Prenom, Grade, Statut, 
                                                     Annee, Mois, Echelon, Categorie, 
                                                     Emploi, Service, Temps.de.travail,
                                                     NBI, Code, Libelle,
                                                     Base, Taux,Type, Montant)
                                                 ][lignes.nbi.anormales.mensuel,
                                                   nomatch = 0,
                                                   on = .(Matricule, Annee, Mois)]
  
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
  
  setkey(Bulletins.paie, Categorie, Matricule, Annee, Mois)
  
  NBI.cat <- Bulletins.paie[! is.na(NBI) & NBI > 0, 
                            .(Matricule,
                              Nom,
                              Prenom,
                              Grade,
                              Statut,
                              Emploi,
                              Categorie,
                              Annee,
                              Mois,
                              NBI,
                              quotite)]
  
  NBI.cat[ , Contrôle := { a <- grepl("d(?:\\.|ir)\\w*\\s*\\bg(?:\\.|\\w*n)\\w*\\s*\\bs\\w*", paste(Emploi, Grade), ignore.case = TRUE, perl = TRUE)
  b <- grepl("d(?:\\.ir)\\w*\\s*\\bg(?:\\.|\\w*n)\\w*\\s*\\ba(?:\\.|d)\\w*", paste(Emploi, Grade), ignore.case = TRUE, perl = TRUE)
  ifelse ((NBI > 20 & Categorie == "C")
          | (NBI > 30 & Categorie == "B")
          | (NBI > 50 
             & Categorie == "A" 
             & ! a 
             & ! b)
          | (NBI > 80 & b & !a)
          | (NBI > 120 & a), "Rouge", "Vert")}]
  
  # Si la quotite est inconnue on la suppose égale à 1 (rare) pour l'évaluation du coût
  
  "NBI.cat.irreg" %a% NBI.cat[Contrôle == "Rouge", 
                           Coût := { a <- adm(quotite)
                           (NBI - ifelse(Categorie == "A", 50, ifelse(Categorie == "B", 30, 20))) *
                             PointMensuelIM[Annee - 2007, Mois] * ifelse(is.na(a), 1, a)}
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

