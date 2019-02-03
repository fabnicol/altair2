
#' Vérification des payes de NBI, compte tenu des rappels et des quotités   
#' 
#' 
#' @md
#' @details 
#' \tabular{ll}{
#' lignes_NBI  \tab  Sélectionne les lignes de paye de NBI \cr    
#' NBI_dec \tab Détecte les payes dont le nombre de points de NBI n'est pas entier    \cr        
#' NBI.aux.non.titulaires  \tab Détecte les NBI versées aux non titulaires    \cr
#' cumuls.nbi \tab  Cumule les points de NBI par année}   
#' Détecte les rappels dont les années ou les mois sont inconnus. Dans de tels cas, suppose que l'année ou le mois manquant est l'année ou le mois en cours. \cr \cr       
#' **Variables globales** \cr
#' Affiche     \cr
#' Sauvegarde  \cr \cr   
#' **Principales formules**   \cr
#' \preformatted{
#'  nbi.eqtp.tot            = (nbi.cum.rappels + nbi.cum.hors.rappels) / adm.quotite
#'  cumul.annuel.indiciaire = sum(nbi.cum.indiciaire, na.rm = TRUE)
#'  cumul.annuel.montants   = sum(nbi.eqtp.tot, na.rm = TRUE)
#' }
#' @seealso proratisation_NBI catégories_NBI
#' @export 
#'

calcul_NBI <- function() {
  
    "Paie_NBI"   %a% filtrer_Paie("NBI", 
                               portée = "Mois", 
                               indic = TRUE)[Type %in% c("T", "I", "R")]
    
    "lignes_NBI" %a% Paie_NBI[indic == TRUE][ , indic :=  NULL]
    
    "rappels.nbi" %a% 0
    "couts.nbi.anormales" %a% 0
    "couts.nbi.anormales.hors.rappels" %a% 0
    "montants.nbi.anormales.mensuel" %a% 0
    "lignes.nbi.anormales.hors.rappels" %a% data.table()
    "lignes.nbi.anormales" %a% data.table()
    "lignes.nbi.anormales.hors.rappels" %a% data.table()
    "cumuls.nbi" %a% data.table()

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
    
    if (nrow(T1) == 0) {
      cat("Aucune NBI n'a été attribuée ou les points de NBI n'ont pas été rencensés en base de paye. ")
      return("")
    }
    
    # On présume que en l'absence d'année ou de mois de rappel, il faut prendre l'année ou le mois en cours.
    
    nb.annee.rappel.inconnue <- length(T1[! is.na(quotite)
                                     & quotite > 0
                                     & Type == "R"
                                     & (is.na(Annee.rappel) | Annee.rappel < début.période.sous.revue), Annee.rappel])
    
    nb.mois.rappel.inconnu <- length(T1[! is.na(quotite)
                                          & quotite > 0
                                          & Type == "R"
                                          & (is.na(Mois.rappel) | Mois.rappel < 1 | Mois.rappel > 12), Mois.rappel])
    
    if (nb.annee.rappel.inconnue > 0) {
      cat("Le champ Année de rattachement du rappel n'est pas renseigné en base (défaut de qualité) pour ", nb.annee.rappel.inconnue, " ligne(s) de rappels.  \n")
      cat("L'année de rattachement du rappel est présumée être celle en cours.  \n")
    }
    
    if (nb.mois.rappel.inconnu > 0) {
      cat("Le champ Mois de rappel n'est pas renseigné en base (défaut de qualité) pour ",  nb.mois.rappel.inconnu, " ligne(s) de rappel.")
      cat("Le mois de rattachement du rappel est présumé être celui du mois en cours.  \n")
    }
    
    if (nb.annee.rappel.inconnue > 0) {
    
        T2a <- T1[! is.na(quotite)
                & quotite > 0
                & Type == "R",
                .(nbi.cum.rappels = sum(Montant, na.rm = TRUE), 
                  quotite = quotite[1],
                  nbi.cum.indiciaire = NBI[1]), 
                by= .(Matricule, Annee.rappel, Mois.rappel, Annee, Mois)
                ][, Annee.rappel := Annee]
        
        if (nb.mois.rappel.inconnu >0) {
          T2a[ , Mois.rappel := Mois]
        }
        
    } else {
      
      if (nb.mois.rappel.inconnu >0) {
        
        T2a <- T1[! is.na(quotite)
                  & quotite > 0
                  & Type == "R",
                  .(nbi.cum.rappels = sum(Montant, na.rm = TRUE), 
                    quotite = quotite[1],
                    nbi.cum.indiciaire = NBI[1]), 
                  by= .(Matricule, Annee.rappel, Mois.rappel, Annee, Mois)
                  ][, Mois.rappel := Mois]
      } else {
    
    
        
    T2a <- T1[! is.na(quotite)
              & quotite > 0
              & Type == "R",
              .(nbi.cum.rappels = sum(Montant, na.rm = TRUE), 
                quotite = quotite[1],
                nbi.cum.indiciaire = NBI[1]), 
              by= .(Matricule, Annee.rappel, Mois.rappel, Annee, Mois)
              ][Annee.rappel >= début.période.sous.revue 
                & Mois.rappel >=1 
                & Mois.rappel <= 12]
      }
    }
    
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
    
    if (is.null(T2b)) {
      
      cat("Toutes les NBI payéees sont des rappels ou présentent des défauts de renseignement de la quotité.")
      T2 <- T2a[ , adm.quotite := adm(quotite)
               ][is.na(nbi.cum.rappels), nbi.cum.rappels := 0
               ][is.na(nbi.cum.hors.rappels), nbi.cum.hors.rappels := 0]
      
    } else {
    
      T2 <- merge(T2a, T2b, 
                  all = TRUE,
                  by = c("Matricule", "Annee", "Mois", "quotite", "nbi.cum.indiciaire"))[ , adm.quotite := adm(quotite)
                                                       ][is.na(nbi.cum.rappels), nbi.cum.rappels := 0
                                                       ][is.na(nbi.cum.hors.rappels), nbi.cum.hors.rappels := 0]
    }
    
    T2[adm.quotite > 0,  nbi.eqtp.tot := (nbi.cum.rappels + nbi.cum.hors.rappels) / adm.quotite]
    
    if (is.null(T2)) return("")
    
    "cumuls.nbi" %a% T2[ , .(cumul.annuel.indiciaire = sum(nbi.cum.indiciaire, na.rm = TRUE),
                             cumul.annuel.montants   = sum(nbi.eqtp.tot, na.rm = TRUE)),
                               keyby = "Annee"]
    
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
    
 
    if (! is.null(T2) && nrow(T2) >  0)
    "lignes.nbi.anormales.hors.rappels" %a% T2[nbi.cum.indiciaire > 0 
                                               & nbi.cum.hors.rappels > 0]
    
    "lignes.nbi.anormales.hors.rappels" %a% cbind(lignes.nbi.anormales.hors.rappels, cout.nbi.anormale = mapply(cout, 
                                                                                      lignes.nbi.anormales.hors.rappels[["nbi.eqtp.tot"]],
                                                                                      lignes.nbi.anormales.hors.rappels[["nbi.cum.indiciaire"]],
                                                                                      lignes.nbi.anormales.hors.rappels[["adm.quotite"]],
                                                                                      lignes.nbi.anormales.hors.rappels[["Annee"]],
                                                                                      lignes.nbi.anormales.hors.rappels[["Mois"]]))
    
    if (! is.null(lignes.nbi.anormales.hors.rappels) && nrow(lignes.nbi.anormales.hors.rappels) > 0) {
      
      "lignes.nbi.anormales.hors.rappels" %a% lignes.nbi.anormales.hors.rappels[cout.nbi.anormale > adm.quotite * nbi.cum.indiciaire
                                              ][ , cout.nbi.anormale, by= .(Annee, Mois)]
    }
    

    if (! is.null(lignes.nbi.anormales.hors.rappels) && nrow(lignes.nbi.anormales.hors.rappels) > 0) { 
      
       "couts.nbi.anormales.hors.rappels" %a% lignes.nbi.anormales.hors.rappels[ , sum(cout.nbi.anormale, na.rm = TRUE)]
    }
    
    if (! is.null(T2a) && nrow(T2a) >  0)
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
  "nb.lignes.anormales.mensuel" %a% 0
  
  essayer({ 
    
  "lignes.nbi.anormales.mensuel" %a% lignes_NBI[Type != "R"]
  
  if (! is.null(lignes.nbi.anormales.mensuel)) {    
    "lignes.nbi.anormales.mensuel" %a% lignes_NBI[, .(Montant.NBI.calculé = round(NBI[1] * adm(quotite[1]) * PointMensuelIM[Annee - 2007, Mois], 2),
                                                                        Montant.NBI.payé = sum(Montant, na.rm = TRUE)), 
                                                    by = .(Matricule, Annee, Mois)
                                                 ][ , Différence.payé.calculé := round(Montant.NBI.payé - Montant.NBI.calculé, 1)
                                                 ][abs(Différence.payé.calculé) > tolérance.nbi]
  }
  
  "lignes.paie.nbi.anormales.mensuel" %a% Paie_NBI[ , .(Matricule, Nom, Prenom, Grade, Statut, 
                                                     Annee, Mois, Echelon, Categorie, 
                                                     Emploi, Service, Temps.de.travail,
                                                     NBI, Code, Libelle,
                                                     Base, Taux,Type, Montant)
                                                 ][lignes.nbi.anormales.mensuel,
                                                   nomatch = 0,
                                                   on = .(Matricule, Annee, Mois)]
  
  "nb.lignes.anormales.mensuel"    %a% nrow(lignes.nbi.anormales.mensuel)
  
  if (! is.null(lignes.nbi.anormales.mensuel))     
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
  "NBI.cat.irreg" %a% NULL
  "nombre.mat.NBI.irrég" %a% NULL
  "coût.total" %a% 0
  
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
                               round((NBI - ifelse(Categorie == "A", 50, ifelse(Categorie == "B", 30, 20))) *
                                 PointMensuelIM[Annee - 2007, Mois] * ifelse(is.na(a), 1, a), 2)}
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

