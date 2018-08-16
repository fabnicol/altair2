
#'@export
#'
calcul_HS <- function() {
  
  ft <- filtre("TRAITEMENT")
  
  colonnes <- c("Matricule",
                "Année",
                "Mois",
                "Statut",
                "Indice",
                "NBI",
                "Libellé",
                "Code",
                "Heures",
                "Heures.Sup.",
                "Temps.de.travail", 
                "quotité.moyenne.orig",
                "Base",
                "Nb.Unité",
                "Taux",
                "Montant",
                "Début",
                "Fin",
                "Type",
                "Service",
                "Emploi",
                "Grade")
  
  "Base.IHTS" %a% filtrer_Paie("IHTS",
                            Base = Paie[Type %chin% c("I", "T", "R", "IR", "A"), ..colonnes],
                            portée = "Mois",
                            indic = TRUE)[ , `:=` (Année.rappel = as.numeric(substr(Début, 0, 4)),
                                                   Mois.rappel  = as.numeric(substr(Début, 6, 7))) ]
  
  lignes.IHTS <- Base.IHTS[indic == TRUE][ , indic := NULL]
  
  Base.IHTS.non.tit <- lignes.IHTS[Statut != "TITULAIRE" & Statut != "STAGIAIRE"]
  
  lignes.IHTS.rappels <- lignes.IHTS[Type == "R" & Montant != 0
                                    ][ , `:=`(ihts.cum.rappels = sum(Montant[Année.rappel == Année & Mois.rappel <= Mois], na.rm = TRUE),
                                               nihts.cum.rappels = ifelse((a <- sum(abs(Base[Année.rappel == Année & Mois.rappel <= Mois]) * sign(Montant), na.rm = TRUE)) == 0,
                                                                          sum(abs(Nb.Unité[Année.rappel == Année & Mois.rappel   <= Mois]) * sign(Montant), na.rm = TRUE),
                                                                          a),
                                               ihts.cum.rappels.ant = sum(Montant[Année.rappel < Année], na.rm = TRUE),
                                               nihts.cum.rappels.ant = ifelse((a <- sum(abs(Base[Année.rappel < Année]) * sign(Montant), na.rm = TRUE)) == 0, 
                                                                              sum(abs(Nb.Unité[Année.rappel   < Année]) * sign(Montant), na.rm = TRUE),
                                                                              a)), 
                                              by = .(Matricule, Année.rappel, Mois.rappel)
                                     ][Année.rappel >= début.période.sous.revue 
                                          & Mois.rappel >=1 
                                          & Mois.rappel <= 12
                                     ][ , .(Matricule, Année, Mois, quotité, quotité.moyenne, Année.rappel,
                                            Mois.rappel, ihts.cum.rappels, nihts.cum.rappels, 
                                            ihts.cum.rappels.ant, nihts.cum.rappels.ant)]
  
  setnames(lignes.IHTS.rappels, "Année", "Année.R")
  setnames(lignes.IHTS.rappels, "Mois", "Mois.R")
  setnames(lignes.IHTS.rappels, "Année.rappel", "Année")
  setnames(lignes.IHTS.rappels, "Mois.rappel", "Mois")
  
  lignes.IHTS.hors.rappels <- lignes.IHTS[Type != "R" & Montant != 0, 
                                          .(ihts.cum.hors.rappels = sum(Montant, na.rm = TRUE),
                                            nihts.cum.hors.rappels = ifelse((a <- sum(abs(Base) * sign(Montant), na.rm = TRUE)) == 0, 
                                                                            sum(abs(Nb.Unité) * sign(Montant), na.rm = TRUE),
                                                                            a),
                                            quotité.moyenne),
                                            by = .(Matricule, Année, Mois)]
  
  "lignes.IHTS.tot" %a% merge(lignes.IHTS.rappels, lignes.IHTS.hors.rappels, 
                           all = TRUE,
                             by = c("Matricule", "Année", "Mois", "quotité.moyenne"))[is.na(ihts.cum.rappels), ihts.cum.rappels := 0
                                                                                   ][is.na(ihts.cum.hors.rappels), ihts.cum.hors.rappels := 0
                                                                                   ][is.na(nihts.cum.rappels), nihts.cum.rappels := 0
                                                                                   ][is.na(nihts.cum.hors.rappels), nihts.cum.hors.rappels := 0
                                                                                   ][is.na(ihts.cum.rappels), ihts.cum.rappels := 0
                                                                                   ][is.na(ihts.cum.rappels.ant), ihts.cum.rappels.ant := 0
                                                                                   ][is.na(nihts.cum.rappels.ant), nihts.cum.rappels.ant := 0]
  
  lignes.IHTS.tot[ ,  `:=`(ihts.tot = ihts.cum.rappels + ihts.cum.hors.rappels + ihts.cum.rappels.ant,
                           nihts.tot = nihts.cum.rappels + nihts.cum.hors.rappels + nihts.cum.rappels.ant)]
  
  essayer({  
    
    if ((N.IHTS.non.tit <<- uniqueN(Base.IHTS.non.tit$Matricule)) > 0) {
    
    cat(N.IHTS.non.tit, "attributaire" %s% N.IHTS.non.tit, " des IHTS sont des non-titulaires.   \n**Vérifier l'existence d'une délibération** le prévoyant expressément.   \n")
    
    } else {
    
    cat("Tous les attributaires des IHTS sont titulaires ou stagiaires.   \n")
    }
    
    # Taux.horaires donne des sommes annuelles pour les traitements et IHTS, afin de calculer les taux maxima "seuils"
    # on prend en compte la NBI pour le calcul du taux (réponse ministérielle 23 mai 2006)   
    
    vect <- c("Matricule", "Année", "Mois", "quotité")
    
    "Taux.horaires" %a% Base.IHTS[ ,.(IR = sum(Montant[Type == "IR"], na.rm = TRUE),
                                   Indice = Indice[1],
                                   NBI = NBI[1],
                                   Heures.Sup. = Heures.Sup.[1]), # ajouter NBI proratisée !
                                      by = vect]
    
    "Taux.horaires" %a% Taux.horaires[lignes.IHTS.tot, nomatch = 0, on = vect]
    
    setkey(Taux.horaires, Année, Mois)
    
    Taux.horaires[ , `Traitement indiciaire annuel et IR` := IR * 12 + (Indice + NBI) * PointIM[Année - 2007, Mois]
                 ][ , `Taux horaire` := `Traitement indiciaire annuel et IR` / 1820 ]
    
    # Pour les temps partiels et les heures complémentaires, pas de sur-rémunération
    
    Taux.horaires[ ,  `:=` (`Taux horaire inf.14 H` = `Taux horaire`,
                            `Taux horaire sup.14 H` = `Taux horaire`,   
                            `Taux horaire nuit`     = `Taux horaire`,     
                            `Taux horaire dim. j.f.`= `Taux horaire`)]
    
    Taux.horaires[quotité >= 0.98,  `:=`(`Taux horaire inf.14 H` = `Taux horaire` * 1.25,
                                         `Taux horaire sup.14 H` = `Taux horaire` * 1.27,   
                                         `Taux horaire nuit`     = `Taux horaire` * 2,     
                                         `Taux horaire dim. j.f.`= `Taux horaire` * 5/3)]             
    
    Taux.horaires[ ,   `:=` (Max = nihts.tot * `Taux horaire nuit`,
                             Min = nihts.tot * `Taux horaire inf.14 H`)
                   
                   ][ ,  `:=`(Indice = NULL,
                              IR = NULL)]   
    
    sauv.bases(file.path(chemin.dossier.bases, "Reglementation"), 
               environment(), 
               "lignes.IHTS",
               "lignes.IHTS.tot",
               "Base.IHTS.non.tit",
               "Taux.horaires")

  },
  "La base des taux horaires d'heures supplémentaires n'a pas pu être générée.   \n")
}
  
#'@export  

dépassements_HS <- function() {
  
  essayer({  depassement <- Taux.horaires[ihts.tot > Max, uniqueN(Matricule)]
  
  depassement.agent <- Taux.horaires[ihts.tot > Max, 
                                     .(`Coût en euros` = -Max + ihts.tot,
                                       Matricule,
                                       Max,
                                       ihts.tot), keyby = Année]
  
  depassement.agent.annee <- depassement.agent[ , .(`Coût en euros` = sum(`Coût en euros`, na.rm = TRUE),
                                                    `Nombre d'agents` = uniqueN(Matricule)), keyby = Année]
  
  if (depassement) {
    
    cat("Il y a", depassement, "agent" %+% ifelse(depassement, "s", ""), "qui perçoivent davantage que le maximum d'IHTS pouvant être liquidé au titre du mois.  \n") 
  }
  
  with(depassement.agent.annee,
       
       print(Tableau.vertical2(c("Année", "Coût en euros", "Nombre d'agents"),
                            Année, digits = 0, `Coût en euros`, `Nombre d'agents`)))   
  
  sauv.bases(file.path(chemin.dossier.bases, "Reglementation"),
             env = new.env(),
             "depassement.agent",
             "depassement.agent.annee")
  
  },
  "Le tableau des dépassements de coûts n'a pas pu être généré.   \n")
  
}

#'@export

cumuls_HS <- function() {
  
  essayer({ 
    
  "CumHS" %a% Bulletins.paie[ , .(toths = sum(Heures.Sup., na.rm = TRUE)), keyby = Année]
  
  # Certaines bases de données indiquent le nombre d'heures sup dans la variable Base et d'autres dans la variable Nb.Unité
  # En principe un rappel concerne un mois antérieur. Mais de nombreuses BD ont des rappels du même mois...
  
  "CumBaseIHTS" %a% unique(lignes.IHTS.tot[, .(Matricule, Année, Mois, quotité, quotité.moyenne, nihts.tot, nihts.cum.hors.rappels, nihts.cum.rappels, nihts.cum.rappels.ant)])
  
  TotBaseIHTS <- CumBaseIHTS[ , .(totihts = sum(nihts.tot),
                                  totihts.hors.rappels = sum(nihts.cum.hors.rappels),
                                  totihts.rappels = sum(nihts.cum.rappels),
                                  totihts.rappels.ant = sum(nihts.cum.rappels.ant)),
                                     keyby = Année]
  
  "CumHS" %a% merge(CumHS, TotBaseIHTS, all = TRUE, by = "Année")
  },
  "La base des cumuls d'IHTS par année, des régularisations et des IHTS apparemment non liquidées n'a pas pu être générée.   \n")
  
  with(CumHS,
       
       print(Tableau.vertical2(c("Année N", "Cumul HS N", "Cumul IHTS N", "dont du mois", "dont rappels N", "dont payés N+1"),
                         Année,             toths,    totihts,   totihts.hors.rappels, totihts.rappels,  totihts.rappels.ant))
       
  )
  
  if ((l <- length(v <- CumHS[toths < totihts, Année])) > 0) {
    cat("  \n", "  \n", 
        "Le nombre d'heures supplémentaires déclarées pour ")     
    cat(v, sep = ", ")
    cat(" est incohérent avec le nombre d'heures IHTS payées au titre de", ifelse(l > 1, "ces", "cet"), ifelse(l > 1, "exercices. ","exercice.   \n"))
  }
  
  sauvebase("CumHS", "CumHS", "Reglementation", environment())
}

#'@export

plafonds_HS <- function() {
  
  Depassement.seuil.180h     <- data.table()
  Dépassement.seuil.220h     <- data.table()
  nb.agents.dépassement      <- 0
  nb.agents.dépassement.220h <- 0

  if (utiliser.variable.Heures.Sup.) {
    
    Depassement.seuil.180h <- Bulletins.paie[ , Nihts.tot := sum(Heures.Sup., na.rm = TRUE),
                                              keyby = .(Matricule, Année)
                                              ][ Nihts.tot > 180 * quotité.moyenne, 
                                                 .(Matricule, 
                                                   Année,
                                                   quotité.moyenne,
                                                   Nihts.tot,                        
                                                   Emploi,
                                                   Grade,
                                                   Service)]
  } else {
    
    Depassement.seuil.180h <- Bulletins.paie[Mois == 12 , .(
      Matricule,
      Année,
      Emploi,
      Grade,
      Service)
      ][CumBaseIHTS[ , .(Nihts.tot = sum(nihts.tot), quotité.moyenne),
                     by = .(Matricule, Année)
                     ][Nihts.tot > (180 * quotité.moyenne), 
                       .(Matricule, 
                         Année,
                         quotité.moyenne,
                         Nihts.tot)],
          on = .(Matricule, Année)]
    
  }

  nb.agents.dépassement <- uniqueN(Depassement.seuil.180h$Matricule)
  
  if  (nb.agents.dépassement)  {
    if (VERSANT_FP == "FPH") {  
      cat("Le seuil de 180 heures supplémentaires maximum est dépassé par ", 
          FR(nb.agents.dépassement), " agents.   \n")
    } 
    
    Depassement.seuil.220h <- Depassement.seuil.180h[Nihts.tot > 220 * quotité.moyenne]
    
    nb.agents.dépassement.220h <- uniqueN(Depassement.seuil.220h$Matricule) 
    
    if (VERSANT_FP == "FPH" && nb.agents.dépassement.220h) {  
      cat(" Le seuil de 220 heures supplémentaires maximum est dépassé par ",
          FR(nb.agents.dépassement.220h), 
          " agents.  \n")
    }
  }

  seuil.HS <- switch (VERSANT_FP, 
                      FPH = 15,
                      FPT = 25)
  
  if (utiliser.variable.Heures.Sup.) {
    cat ("Les cumuls d'IHTS sont déterminés à partir de la variable Heures.Sup. ")
    "HS.sup.25" %a% lignes.IHTS[Heures.Sup. > seuil.HS * quotité]
    
  } else {
    
    cat ("Les cumuls d'IHTS sont déterminés à partir des paiements de l'année, rappels compris, et des rappels payés l'année suivante.   \n")      
    "HS.sup.25" %a% CumBaseIHTS[nihts.tot > seuil.HS * quotité]
  }
  
  nombre.Lignes.paie.HS.sup.25 <- nrow(HS.sup.25)
  
  ihts.cat.A <- filtrer_Paie("IHTS")[Montant != 0 
                                     & Catégorie == "A"
                                     & Type %chin% c("R", "I", "T", "A"),
                                     .(Matricule, Année, Mois, Statut, Grade, Heures.Sup., Libellé, Code, Type, Montant)]
  
  nombre.ihts.cat.A <- nrow(ihts.cat.A) 
  
  print(Tableau(c("Nombre de lignes HS en excès", "Nombre de lignes IHTS cat. A"),
             nombre.Lignes.paie.HS.sup.25,   nombre.ihts.cat.A))

  sauv.bases(file.path(chemin.dossier.bases, "Reglementation"),
             environment(),
             "HS.sup.25",
             "Depassement.seuil.180h",
             "Depassement.seuil.220h",
             "ihts.cat.A")
  sauvebase("HS.sup.25", "HS.sup.25", "Reglementation", environment())
  message("Heures sup controlées")  
}
