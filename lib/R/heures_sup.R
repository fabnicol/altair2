# WIDTH: 90 char.

#'@export
#'
calcul_HS <- function() {

  ft <- filtre("TRAITEMENT")

  colonnes <- c("Matricule",
                "Annee",
                "Mois",
                "Statut",
                "Indice",
                "NBI",
                "Libelle",
                "Code",
                "Heures",
                "Heures.Sup.",
                "Temps.de.travail",
                "quotite.moyenne.orig",
                "Base",
                "Nb.Unite",
                "Taux",
                "Montant",
                "Debut",
                "Fin",
                "Type",
                "Service",
                "Emploi",
                "Grade")

  # Sélectionner dans les lignes de paye les
  # indémnités, les traitements, les rappels, les indemnités de résidence et
  # les autres rémunérations ; identifier l'année et le mois de référence (rappel)

  "Base.IHTS" %a% filtrer_Paie("IHTS",
                               Base = Paie[Type %chin% c("I", "T", "R", "IR", "A"),
                                           ..colonnes],
                               portee = "Mois",
                               indic = TRUE)[, `:=` (Annee.rappel =
                                                       as.numeric(substr(Debut, 0, 4)),
                                                     Mois.rappel  =
                                                       as.numeric(substr(Debut, 6, 7)))]

  # Ne retenir que les IHTS grâce à l'indicatrice indic

  lignes.IHTS <- Base.IHTS[indic == TRUE][ , indic := NULL]

  Base.IHTS.non.tit <- lignes.IHTS[Statut != "TITULAIRE" & Statut != "STAGIAIRE"]

  # Pour les rappels de paiement identifiés comme tels par Type
  # 1. Filtrer les résultats pour ne retenir que les périodes de référence intérieures
  #    à la période sous revue (années et mois des payes courantes de la base)
  # 2. Sommer tous les paiements en rappel postérieurs à un mois donné pris
  #    comme référence de l'année en cours, par matricule -> ihts.cum.rappels
  # 3. Sommer tous les paiements en rappel intervenus dans les années suivantes
  # 4. Les nombres d'heures de rappel, selon les données, sont obtenues par la variable
  #    Base ou la variable Nb.Unite, l'une ou l'autre étant nulle.
  #    Compte tenu du signe des rappels (-, trop-perçu ou + paiement),
  #    qui n'est pas reflété par ces deux variables, ou de manière aléatoire, il faut
  #    prendre leur valeur absolue et utiliser le signe du montant. Leur somme algébrique
  #    pour l'ensemble des mois de rappel de l'année en cours donne le nombre d'heures
  #    supplémentaires payées, par matricule, pour l'année en cours
  #    au titre du mois donné en référence (Mois.rappel) -> nihts.cum.rappels
  # 5. Idem pour les paiements postérieurs à l'année en cours, par matricule
  #    -> nihts.cum.rappels.ant

  lignes.IHTS.rappels <- lignes.IHTS[Type == "R" &
    Montant != 0 &
    Annee.rappel >= debut.periode.sous.revue &
    Mois.rappel >=1 &
    Mois.rappel <= 12
  ][, `:=`(ihts.cum.rappels =
             sum(Montant[Annee.rappel == Annee &
                           Mois.rappel <= Mois], na.rm = TRUE),
           nihts.cum.rappels =
             ifelse((a <- sum(abs(Base[Annee.rappel == Annee &
                                         Mois.rappel <= Mois]) *
                                sign(Montant), na.rm = TRUE)) == 0,
                    sum(abs(Nb.Unite[Annee.rappel == Annee &
                                       Mois.rappel <= Mois]) *
                          sign(Montant), na.rm = TRUE),
                    a),
           ihts.cum.rappels.ant =
             sum(Montant[Annee.rappel < Annee], na.rm = TRUE),
           nihts.cum.rappels.ant =
             ifelse((a <- sum(abs(Base[Annee.rappel < Annee]) *
                                sign(Montant),
                              na.rm = TRUE)) == 0,
                    sum(abs(Nb.Unite[Annee.rappel   < Annee]) *
                          sign(Montant), na.rm = TRUE),
                    a)),
    by = .(Matricule, Annee.rappel, Mois.rappel)
  ][ , .(Matricule, Annee, Mois, quotite, quotite.moyenne, Annee.rappel,
         Mois.rappel, ihts.cum.rappels, nihts.cum.rappels,
         ihts.cum.rappels.ant, nihts.cum.rappels.ant)]

  setnames(lignes.IHTS.rappels, "Annee", "Annee.R")
  setnames(lignes.IHTS.rappels, "Mois", "Mois.R")
  setnames(lignes.IHTS.rappels, "Annee.rappel", "Annee")
  setnames(lignes.IHTS.rappels, "Mois.rappel", "Mois")

  lignes.IHTS.hors.rappels <- lignes.IHTS[Type != "R" & Montant != 0,
                                          .(ihts.cum.hors.rappels =
                                              sum(Montant, na.rm = TRUE),
                                            nihts.cum.hors.rappels =
                                              ifelse((a <- sum(abs(Base) *
                                                                 sign(Montant),
                                                               na.rm = TRUE)) == 0,
                                                     sum(abs(Nb.Unite) * sign(Montant),
                                                         na.rm = TRUE),
                                                     a),
                                            quotite,
                                            quotite.moyenne),
                                          by = .(Matricule, Annee, Mois)]

  "lignes.IHTS.tot" %a%
    merge(lignes.IHTS.rappels,
          lignes.IHTS.hors.rappels,
          all = TRUE,
          by = c("Matricule", "Annee", "Mois",
                 "quotite", "quotite.moyenne"))[
                   is.na(ihts.cum.rappels), ihts.cum.rappels := 0
                 ][is.na(ihts.cum.hors.rappels), ihts.cum.hors.rappels := 0
                 ][is.na(nihts.cum.rappels), nihts.cum.rappels := 0
                 ][is.na(nihts.cum.hors.rappels), nihts.cum.hors.rappels := 0
                 ][is.na(ihts.cum.rappels), ihts.cum.rappels := 0
                 ][is.na(ihts.cum.rappels.ant), ihts.cum.rappels.ant := 0
                 ][is.na(nihts.cum.rappels.ant), nihts.cum.rappels.ant := 0]

  lignes.IHTS.tot[ ,  `:=`(ihts.tot  = ihts.cum.rappels +
                             ihts.cum.hors.rappels +
                             ihts.cum.rappels.ant,
                           nihts.tot = nihts.cum.rappels +
                             nihts.cum.hors.rappels +
                             nihts.cum.rappels.ant)]

  essayer({

    if ((N.IHTS.non.tit <<- uniqueN(Base.IHTS.non.tit$Matricule)) > 0) {

      cat(N.IHTS.non.tit,
          "attributaire" %s% N.IHTS.non.tit,
          " des IHTS sont des non-titulaires.   \n",
          "**Vérifier l'existence d'une délibération** le prévoyant expressément.   \n")

    } else {

      cat("Tous les attributaires des IHTS sont titulaires ou stagiaires.   \n")
    }

    # Taux.horaires donne des sommes annuelles pour les traitements et IHTS,
    # afin de calculer les taux maxima "seuils"
    # on prend en compte la NBI pour le calcul du taux
    # (réponse ministérielle 23 mai 2006)

    vect <- c("Matricule", "Annee", "Mois", "quotite")

    "Taux.horaires" %a% Base.IHTS[ ,.(IR = sum(Montant[Type == "IR"],
                                               na.rm = TRUE),
                                      Indice = Indice[1],
                                      NBI = NBI[1],
                                      # ajouter NBI proratisée !
                                      Heures.Sup. = Heures.Sup.[1]),
                                   by = vect]

    "Taux.horaires" %a% Taux.horaires[lignes.IHTS.tot, nomatch = 0, on = vect]

    setkey(Taux.horaires, Annee, Mois)

    f <- function(IR, Indice, NBI, Annee, Mois) {
      IR * 12 + (Indice + NBI) * PointIM[Annee - 2007, Mois]
    }

    Taux.horaires$`Traitement indiciaire annuel et IR` <<-
      mapply(f,
             Taux.horaires$IR,
             Taux.horaires$Indice,
             Taux.horaires$NBI,
             Taux.horaires$Annee,
             Taux.horaires$Mois,
             USE.NAMES = FALSE)

    Taux.horaires[ , `Taux horaire` := `Traitement indiciaire annuel et IR` / 1820 ]

    # Pour les temps partiels et les heures complémentaires, pas de sur-rémunération

    Taux.horaires[ ,  `:=` (`Taux horaire inf.14 H` = `Taux horaire`,
                            `Taux horaire sup.14 H` = `Taux horaire`,
                            `Taux horaire nuit`     = `Taux horaire`,
                            `Taux horaire dim. j.f.`= `Taux horaire`)]

    Taux.horaires[quotite >= 0.98,  `:=`(`Taux horaire inf.14 H` = `Taux horaire` * 1.25,
                                         `Taux horaire sup.14 H` = `Taux horaire` * 1.27,
                                         `Taux horaire nuit`     = `Taux horaire` * 2,
                                         `Taux horaire dim. j.f.`= `Taux horaire` * 5/3)]

    # Pour évaluer les dépassements, il faut prendre le nombre d'heures sup. payées
    # dans le mois et non pas le nombre d'heures effectuées et éventuellement payées
    # au cours des mois qui suivent [BUGFIX]

    Taux.horaires[ ,   `:=` (Max = Heures.Sup. * `Taux horaire nuit`,
                             Min = Heures.Sup. * `Taux horaire inf.14 H`)

    ][ ,  `:=`(Indice = NULL,
               IR = NULL)]

    sauv.bases("Reglementation",
               environment(),
               "lignes.IHTS",
               "lignes.IHTS.tot",
               "Base.IHTS.non.tit",
               "Taux.horaires")
  },
  "La base des taux horaires d'heures supplémentaires n'a pas pu être générée.  \n")
}

#'@export

dépassements_HS <- function() {

  essayer({  depassement <- Taux.horaires[ihts.tot > Max, uniqueN(Matricule)]

  depassement.agent <- Taux.horaires[ihts.tot > Max,
                                     .(`Coût en euros` = -Max + ihts.tot,
                                       Matricule,
                                       Max,
                                       ihts.tot),
                                       keyby = Annee]

  depassement.agent.annee <-
    depassement.agent[ ,
                        .(`Coût en euros` = sum(`Coût en euros`, na.rm = TRUE),
                          `Nombre d'agents` = uniqueN(Matricule)),
                          keyby = Annee]

  if (depassement) {

    cat("Il y a", depassement, "agent" %+% ifelse(depassement, "s", ""),
        "qui perçoivent davantage que le maximum d'IHTS pouvant",
        "être liquidé au titre du mois.  \n")

    with(depassement.agent.annee,

         print(Tableau.vertical2(c("Annee", "Coût en euros", "Nombre d'agents"),
                                 Annee, digits = 0, `Coût en euros`, `Nombre d'agents`)))
  } else {
    cat("Pas de dépassement détecté du maximum d'IHTS pouvant",
        "être liquidé au titre du mois.   \n")
  }

  sauv.bases("Reglementation",
             env = new.env(),
             "depassement.agent",
             "depassement.agent.annee")

  },
  "Le tableau des dépassements de coûts n'a pas pu être généré.   \n")
}

#'@export

cumuls_HS <- function() {

  "CumHS" %a% NULL

  essayer({

    "CumHS" %a% Bulletins.paie[ , .(toths = sum(Heures.Sup., na.rm = TRUE)),
                                    keyby = Annee]

    # Certaines bases de données indiquent le nombre d'heures sup dans la variable Base
    # et d'autres dans la variable Nb.Unite
    # En principe un rappel concerne un mois antérieur. Mais de nombreuses BD ont des
    # rappels du même mois...

    "CumBaseIHTS" %a% unique(lignes.IHTS.tot[, .(Matricule,
                                                 Annee,
                                                 Mois,
                                                 quotite,
                                                 quotite.moyenne,
                                                 nihts.tot,
                                                 nihts.cum.hors.rappels,
                                                 nihts.cum.rappels,
                                                 nihts.cum.rappels.ant)])

    TotBaseIHTS <- CumBaseIHTS[ , .(totihts = sum(nihts.tot),
                                    totihts.hors.rappels = sum(nihts.cum.hors.rappels),
                                    totihts.rappels = sum(nihts.cum.rappels),
                                    totihts.rappels.ant = sum(nihts.cum.rappels.ant)),
                                keyby = Annee]

    "CumHS" %a% merge(CumHS, TotBaseIHTS, all = TRUE, by = "Annee")
  },
  "La base des cumuls d'IHTS par année, des régularisations" %+%
    "et des IHTS apparemment non liquidées n'a pas pu être générée.   \n")

  if (!is.null(CumHS)) {

    with(CumHS,

         print(Tableau.vertical2(c("Annee N",
                                   "Cumul HS N",
                                   "Cumul IHTS N",
                                   "dont du mois",
                                   "dont rappels N",
                                   "dont payes N+1"),
                                 Annee,
                                 toths,
                                 totihts,
                                 totihts.hors.rappels,
                                 totihts.rappels,
                                 totihts.rappels.ant))
    )

    if ((l <- length(v <- CumHS[toths < totihts, Annee])) > 0) {
      cat("  \n", "  \n",
          "Le nombre d'heures supplémentaires déclarées pour ")
      cat(v, sep = ", ")
      cat(" est incohérent avec le nombre d'heures IHTS payees au titre de",
          ifelse(l > 1, "ces", "cet"),
          ifelse(l > 1, "exercices. ","exercice.   \n"))
    }

    sauvebase("CumHS", "CumHS", "Reglementation")

  } else {
    cat("La base des cumuls d'heures supplémentaires n'a pas pu être générée.   \n")
  }
}

#'@export

plafonds_HS <- function() {

  Depassement.seuil.180h     <- data.table()
  Dépassement.seuil.220h     <- data.table()
  nb.agents.dépassement      <- 0
  nb.agents.dépassement.220h <- 0

  if (utiliser.variable.Heures.Sup.) {

    Depassement.seuil.180h <-
      unique(Bulletins.paie[ ,
                               Nihts.tot := sum(Heures.Sup., na.rm = TRUE),
                               keyby = .(Matricule, Annee)
    ][Nihts.tot > 180 * quotite.moyenne,
      .(Matricule,
        Annee,
        quotite.moyenne,
        Nihts.tot,
        Emploi,
        Grade,
        Service)])

  } else {

    Depassement.seuil.180h <- unique(Bulletins.paie[Mois == 12 , .(
      Matricule,
      Annee,
      Emploi,
      Grade,
      Service)
    ][CumBaseIHTS[ , .(Nihts.tot = sum(nihts.tot), quotite.moyenne),
                   by = .(Matricule, Annee)
    ][Nihts.tot > (180 * quotite.moyenne),
      .(Matricule,
        Annee,
        quotite.moyenne,
        Nihts.tot)],
    on = .(Matricule, Annee)])

  }

  nb.agents.dépassement <- uniqueN(Depassement.seuil.180h$Matricule)

  if  (nb.agents.dépassement)  {
    if (VERSANT_FP == "FPH") {
      cat("Le seuil de 180 heures supplémentaires maximum est dépassé par ",
          FR(nb.agents.dépassement), " agents.   \n")
    }

    Depassement.seuil.220h <- Depassement.seuil.180h[Nihts.tot > 220 * quotite.moyenne]

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
    "HS.sup.25" %a% lignes.IHTS[Heures.Sup. > seuil.HS * quotite]

  } else {

    cat ("Les cumuls d'IHTS sont déterminés à partir des paiements de l'année,",
         "rappels compris, et des rappels payés l'année suivante.   \n")
    "HS.sup.25" %a% CumBaseIHTS[nihts.tot > seuil.HS * quotite]
  }

  nombre.Lignes.paie.HS.sup.25 <- nrow(HS.sup.25)

  # Attention régime spécial FPH
  # IHTS payables à :
  # -cadre de santé ;
  # -infirmier anesthésiste ;
  # -infirmier de bloc opératoire ;
  # -infirmière puéricultrice ;
  # -infirmier ;
  # -orthophoniste ;
  # -orthoptiste ;
  # -diététicien ;
  # -ergothérapeute ;
  # -masseur-kinésithérapeute ;
  # -psychomotricien ;
  # -pédicure-podologue ;
  # -aide-soignant (y compris aide médico-psychologique et auxiliaire de puériculture) ;
  # -psychologue ;
  # -technicien de laboratoire ;
  # -préparateur en pharmacie ;
  # -manipulateur d'électroradiologie médicale.
  # Personnels sages-femmes :
  # -sage-femme cadre ;
  # -sage-femme.
  # Personnels administratifs :
  # -adjoint des cadres administratifs ;
  # -secrétaire médical ;
  # -adjoint administratif hospitalier ;
  # -permanencier auxiliaire de régulation médicale ;
  # -standardiste.
  # Personnels techniques :
  # -adjoint technique ;
  # -dessinateur.
  # Personnels ouvriers :
  # -contremaître ;
  # -maître ouvrier ;
  # -conducteur ambulancier ;
  # -chef de garage ;
  # -agent technique d'entretien.
  # Personnels socio-éducatif :
  # -cadre socio-éducatif ;
  # -animateur ;
  # -éducateur technique spécialisé ;
  # -éducateur de jeunes enfants ;
  # -moniteur-éducateur ;
  # -moniteur d'atelier ;
  # -assistant socio-éducatif ;
  # -conseiller en économie sociale et familiale.
  # Les personnels de l'informatique et de l'organisation recrutés en application de
  # l'article 8 de la loi n° 86-33 du 9 janvier 1986 sont éligibles aux indemnités
  # horaires pour travaux supplémentaires.

  # prochainement les corps suivants vont basculer en cat 1:
  # Décret n° 2018-731 du 21 août 2018
  # portant dispositions statutaires communes à certains
  # corps de catégorie A de la fonction publique hospitalière à caractère socio-éducatif
  # Sont classés dans la catégorie A de la fonction publique hospitalière prévue
  # à l'article 13 de la loi du 13 juillet 1983 susvisée, les corps des personnels
  # socio-éducatifs des établissements mentionnés à l'article 2 de la loi
  # du 9 janvier 1986 susvisée, ci-dessous énumérés :
  # 1° Le corps des conseillers en économie sociale et familiale ;
  # 2° Le corps des éducateurs techniques spécialisés ;
  # 3° Le corps des éducateurs de jeunes enfants ;
  # 4° Le corps des assistants socio-éducatifs.

  reg.exceptions <-
    "(infirm|cadre.*sant|sage.*fem|pu..?ric|ergot|orthop|di..?t..?ti|\
psycho|cadre.*soc.*du|\
cons.*cono.*f|..?ducat.*(spec|enf)|assist.*soc.*du).*"

  ihts.cat.A <- filtrer_Paie("IHTS")[Montant != 0 &
                                       Categorie == "A" &
                                       ! grepl(reg.exceptions,
                                               Grade,
                                               ignore.case = TRUE,
                                               perl = TRUE) &
                                       Type %in% c("R", "I", "T", "A"),
                                     .(Matricule, Annee, Mois, Statut, Grade,
                                       Heures.Sup., Libelle, Code, Type, Montant)]

  nombre.ihts.cat.A <- nrow(ihts.cat.A)

  print(Tableau(c("Nombre de lignes HS en excès",
                  "Nombre de lignes IHTS cat. A"),
                nombre.Lignes.paie.HS.sup.25,
                nombre.ihts.cat.A))

  sauv.bases("Reglementation",
             environment(),
             "HS.sup.25",
             "Depassement.seuil.180h",
             "Depassement.seuil.220h",
             "ihts.cat.A")

  message("Heures sup contrôlées")
}
