


# personnels
# Analyse.rémunérations[Statut != "ELU"
#         & Filtre_annexe == TRUE
#         & Filtre_actif == TRUE
#         & Année == x,
#         Matricule]


#' Tableau des effectifs.
#'
#' Elabore un tableau des effectifs et équivalents temps plein travaillés par type de personnel et par année.
#'
#' @param période Vecteur des années de la période sous revue
#' @param Bulletins Base des bulletins de paye, comportant pour l'ensemble de la période
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Grade}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotité}}{réel entre 0 et 1}
#'                 \item{\code{nb.mois}}{entier entre 0 et 12}}}
#'           \item{ la variable booléenne :
#'               \describe{\item{\code{permanent}}{12 bulletins sur l'année}}}}.
#' @param Analyse Base des analyses de rémunérations, comptant les variables :
#'        \code{Filtre_actif, Filtre_annexe, Statut, Matricule, Année}
#' @param Analyse Base des analyses de variations de rémunérations, comptant les variables :
#'       \code{temps.complet, est.rmpp, statut, Matricule, Année, permanent}
#' @return Un tableau des effectifs mis en forme de 22 lignes et autant de colonnes numériques que d'années de période, plus une colonne de libellés.
#' @examples
#' effectifs(2010:2015)
#' @export

# Bulletins : "Matricule", "Statut", "permanent", "quotité", "nb.mois", "Grade"
# Analyse.rémunérations : Filtre_actif, Filtre_annexe, Statut, Matricule, Année (+filtres sur lignes)
# Analyse.v : temps.complet, est.rmpp, statut, Matricule, Année, permanent   (+ filtres sur lignes)

effectifs <- function(période, Bulletins = Bulletins.paie,
                      personnels = Analyse.remunerations,
                      Analyse.v = Analyse.variations) {


  eff <- lapply(période,
                function(x) {
                  A <- Bulletins[Année == x,
                                  c("Matricule", "Statut", "permanent", "quotité", "nb.mois", "Grade"), with = FALSE]

                  E <- unique(A[ , .(Matricule, permanent)], by = NULL)
                  ETP <- unique(Bulletins[Année == x,
                                               .(quotité, Matricule, Statut, permanent, Mois, nb.mois)],
                                by = NULL)
                  F <- E[permanent == TRUE, ]

                  G <- unique(A[Statut == "TITULAIRE" | Statut == "STAGIAIRE", .(Matricule, permanent)], by = NULL)

                  H <- G[permanent == TRUE, ]

                  postes.non.titulaires <- unique(A[Statut == "NON_TITULAIRE", Matricule], by = NULL)

                  I <- unique(A[Statut == "ELU", .(Matricule, permanent)],
                              by = NULL)
                  J <- I[permanent == TRUE, ]
                  K <- unique(A[Statut != "TITULAIRE" & Statut != "STAGIAIRE" & Grade == "V", .(Matricule, permanent)],
                              by = NULL)
                  L <- unique(A[Grade == "A", .(Matricule, permanent)],
                              by = NULL)
                  postes.non.actifs <- unique(personnels[Statut != "ELU"
                                                                    & Filtre_actif == FALSE
                                                                    & Année == x,
                                                                    Matricule])
                  postes.actifs.annexes <- unique(personnels[Statut != "ELU"
                                                                        & Filtre_annexe == TRUE
                                                                        & Filtre_actif == TRUE
                                                                        & Année == x,
                                                                        Matricule])
                  postes.actifs.non.annexes <- unique(personnels[Statut != "ELU"
                                                                            & Filtre_annexe == FALSE
                                                                            & Filtre_actif == TRUE
                                                                            & Année == x,
                                                                            Matricule])



                  c(nrow(E),   # Matricules gérés en base
                    nrow(F),   # dont présents 12 mois
                    nrow(G),   # dont fonctionnaires
                    nrow(H),   # dont fonct. présents 12 mois
                    length(postes.non.titulaires),  # dont non titulaires
                    nrow(I),   # dont élus
                    nrow(J),   # dont élus présents 12 mois
                    nrow(K),   # dont vacataires détectés
                    nrow(L),   # dont assistantes maternelles détectées
                    length(postes.non.actifs),      # Postes non actifs
                    length(postes.actifs.annexes),  # Postes actifs annexes
                    length(postes.actifs.non.annexes),  # Postes actifs non annexes
                    ETP[Statut != "ELU" , sum(quotité/nb.mois, na.rm=TRUE)],  # Total ETP/année
                    ETP[Statut != "ELU" , sum(quotité, na.rm=TRUE)] / 12,     # Total ETPT/année
                    ETP[Matricule %chin% unique(Analyse.v[est.rmpp == TRUE    # Total ETPT/année personnes en place
                                                                   & Année == x,
                                                                   Matricule]),
                        sum(quotité, na.rm=TRUE)] / 12,

                    ETP[(Statut == "TITULAIRE" | Statut == "STAGIAIRE")       # Total ETPT/année fonctionnaires
                        & Matricule %chin% unique(Analyse.v[Statut == "TITULAIRE"
                                                                     | Statut == "STAGIAIRE",
                                                                     Matricule]),
                        sum(quotité, na.rm=TRUE)] / 12,
                    ETP[Statut == "TITULAIRE"                                 # Total ETPT/année titulaires à temps complet
                        & permanent == TRUE
                        & Matricule %chin% unique(Analyse.v[permanent == TRUE
                                                                     & statut == "TITULAIRE"
                                                                     & temps.complet == TRUE
                                                                     & Année == x,
                                                                     Matricule]),
                        sum(quotité, na.rm=TRUE)] / 12,

                    ETP[Statut == "NON_TITULAIRE"                             # Total ETPT non titulaires
                        & Matricule %chin% postes.non.titulaires,
                        sum(quotité, na.rm=TRUE)] / 12,

                    ETP[Statut == "AUTRE_STATUT"                              # Total ETPT autre statut
                        & Matricule %chin% unique(personnels[Statut == "AUTRE_STATUT",
                                                                        Matricule]),
                        sum(quotité, na.rm=TRUE)] / 12,

                    ETP[Matricule %chin% postes.non.actifs, sum(quotité, na.rm=TRUE)] / 12,         # Total ETPT postes non actifs
                    ETP[Matricule %chin% postes.actifs.annexes, sum(quotité, na.rm=TRUE)] / 12,     # Total ETPT postes actifs annexes
                    ETP[Matricule %chin% postes.actifs.non.annexes, sum(quotité, na.rm=TRUE)] / 12)	# Total ETPT actif non annexes
                    })


for (i in 1:length(eff)) names(eff[[i]]) <- c("Effectifs",
                                                          "Effectifs_12",
                                                          "Effectifs_12_fonct",
                                                          "Effectifs_12_fonct",
                                                          "Effectifs_nontit",
                                                          "Effectifs_élus",
                                                          "Effectifs_12_élus",
                                                          "Effectifs_vac",
                                                          "Effectifs_am",
                                                          "Effectifs_non.actifs",
                                                          "Effectifs_actifs_annexes",
                                                          "Effectifs_actifs_non.annexes",
                                                          "ETP",
                                                          "ETPT",
                                                          "ETPT_pp",
                                                          "ETPT_fonct",
                                                          "Tit_12_100",
                                                          "ETPT_nontit",
                                                          "ETPT_autre",
                                                          "ETPT_non_actif",
                                                          "ETPT_annexe",
                                                          "ETPT_actif_nonannexe")

effectifs.locale <- lapply(eff,
                           function(x) formatC(x, big.mark = " ", format="f", digits=1, decimal.mark=","))

tableau.effectifs <- as.data.frame(effectifs.locale,
                                   row.names = c("Matricules gérés en base (a)",
                                                 "&nbsp;&nbsp;&nbsp;dont présents 12 mois",
                                                 "&nbsp;&nbsp;&nbsp;dont fonctionnaires (b)",
                                                 "&nbsp;&nbsp;&nbsp;dont fonct. présents 12 mois",
                                                 "&nbsp;&nbsp;&nbsp;dont non titulaires",
                                                 "&nbsp;&nbsp;&nbsp;dont élus",
                                                 "&nbsp;&nbsp;&nbsp;dont élus présents 12 mois",
                                                 "&nbsp;&nbsp;&nbsp;dont vacataires détectés (c)",
                                                 "&nbsp;&nbsp;&nbsp;dont assistantes maternelles détectées (c)",
                                                 "Postes non actifs (g)",
                                                 "Postes actifs annexes (g)",
                                                 "Postes actifs non annexes (g)",
                                                 "Total ETP/année (d)",
                                                 "Total ETPT/année (e)",
                                                 "Total ETPT/année personnes en place (f)(g)",
                                                 "Total ETPT/année fonctionnaires (g)",
                                                 "Total ETPT/année titulaires à temps complet (g)",
                                                 "Total ETPT non titulaires (g)",
                                                 "Total ETPT autre statut",
                                                 "Total ETPT postes non actifs (g)",
                                                 "Total ETPT postes actifs annexes (g)",
                                                 "Total ETPT postes actifs non annexes (g)"))

names(tableau.effectifs) <-  as.character(période)

return(tableau.effectifs)
}

analyse.regexp <- function(Base, regexp, agr) {

      dim <- length(regexp)
  
      if (agr && is.null(regexp)) {
        message("Une expression régulière doit être entrée pour agr = TRUE") 
        return(NULL)
      }
      
   
      for (i in 1:dim)  Base[Statut !=  "ELU", paste0("G", i) :=  grepl(regexp[i], Grade, ignore.case = TRUE, perl = TRUE) * i]
      G <- rowSums(Base[ , paste0("G", 1:dim), with = FALSE])
      Base <- cbind(Base, G)
      Base[G != 0]

}

#' Tableau des EQTP par grade.
#'
#' Elabore un tableau des équivalents temps plein travaillés par grade et par année.
#'
#' @param Bulletins Base des bulletins de paye, comportant pour l'ensemble de la période.
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.
#' @param regexp Vecteur de chaîne de caractères représentant des expressions régulières sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param période Vecteur des années considérées.
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Année}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Grade}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotité}}{réel entre 0 et 1}}}}.
#' @return Un tableau des effectifs mis en forme avec les grades en ligne et autant de colonnes numériques que d'années de période, plus une colonne de libellés.
#' @examples
#' eqtp.grade()
#' @export

# Bulletins : "Matricule", "Statut", "quotité", "Grade"

eqtp.grade <- function(Bulletins = Bulletins.paie, grade = NULL, regexp = NULL, libellés = NULL, agr = FALSE, période = NULL) {

  if (! is.null(libellés) && length(libellés) != length(regexp)) {
      
      message("Le vecteur des libellés doit avoir la même longueur que le vecteur des expressions régulières")
      return(NULL)
  }
  
  T <- analyse.regexp(Bulletins, regexp, agr)
  
  if (agr) {
          
          Gr <- "G"
          message("Agrégation des grades")
          
  } else {
          
          if (is.null(regexp) & ! is.null(grade))   T <- Base[Statut !=  "ELU" & Grade %chin% grade]
          
          Gr <- "Grade" 
  }
  
  tableau.effectifs <- T[Statut != "ELU", .(eqtp.g = sum(quotité, na.rm = TRUE) / 12),
                                                         by=c("Année", Gr)
                                      ][ , eqtp.grade := formatC(eqtp.g, digits=1, format = "f")]
  
  if (! is.null(période)) tableau.effectifs <- tableau.effectifs[Année %in% période]
  
  totaux <- tableau.effectifs[, .(Total=formatC(sum(eqtp.g, na.rm = TRUE), digits=1, format="f")), by=Année]
  totaux <- transpose(data.table(c("Total", totaux$Total)))
  
  if (agr) {
    
    tableau.effectifs <- dcast(tableau.effectifs, G ~ Année, value.var = "eqtp.grade", fill = 0)
    if (! is.null(libellés)) tableau.effectifs$G <- libellés  
    names(tableau.effectifs)[1] <- "Catégorie de Grades"
    
  } else {
   
    tableau.effectifs <- dcast(tableau.effectifs, Grade ~ Année, value.var = "eqtp.grade", fill = 0)  
    
  }
  
  colnames(totaux) <- colnames(tableau.effectifs)
  
  rbind(tableau.effectifs, totaux)
}

#' Tableau des charges de personnel par grade.
#'
#' Elabore un tableau des charges de personnel (coût) par grade et par année.
#'
#' @param Base Base des Paies, comportant pour l'ensemble de la période. Par défaut, base Paie.
#' @param grade Vecteur de grades particuliers. Tous les grades en l'absence de spécification. 
#' @param regexp Vecteur de chaîne de caractères représentant des expressions régulières sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp.
#' @param période Vecteur des années considérées.
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Année}             
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Type}
#'                 \item \code{Grade}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{Montant}}{réel}}}}
#'                 
#' @return Un tableau des charges de personnel (coût employeur) mis en forme avec les grades en ligne et autant de colonnes numériques que d'années de période, plus une colonne de libellés.
#' @examples
#' charges.personnel()
#' @export

charges.personnel <- function(Base = Paie, grade = NULL, regexp = NULL, libellés = NULL, agr = FALSE, période = NULL) {
  
  if (! is.null(libellés) && length(libellés) != length(regexp)) {
      
      message("Le vecteur des libellés doit avoir la même longueur que le vecteur des expressions régulières")
      return(NULL)
  }
  
  T <- analyse.regexp(Base, regexp, agr)
  
  if (agr) {
          
          Gr <- "G"
          message("Agrégation des grades")
          
  } else {
          
          if (is.null(regexp) & ! is.null(grade))   T <- Base[Statut !=  "ELU" & Grade %chin% grade]
          
          Gr <- "Grade" 
  }
  
  A <-T[Statut !=  "ELU", .(Coût = round(sum(Montant[Type != "AV" & Type != "RE" & Type != "D"], na.rm = TRUE))), 
                              keyby=c("Année", Gr)]
  
  if (! is.null(période)) A <- A[Année %in% période]
  
  totaux <- A[, .(Total = sum(Coût, na.rm = TRUE)), by = Année]
  totaux <- transpose(data.table(c("Total", totaux$Total)))
  
  if (agr) {
    
    A <- dcast(A, G ~ Année, value.var = "Coût", fill = 0)
    if (! is.null(libellés)) A$G <- libellés  
    names(A)[1] <- "Catégorie de Grades"
    
  } else {
    
    A <- dcast(A, Grade ~ Année, value.var = "Coût", fill = 0)
  }
  
  colnames(totaux) <- colnames(A)
  rbind(A, totaux)
  
}

#' Tableau des coûts moyens de personnel par grade.
#'
#' Elabore un tableau des charges de personnel (coût) par grade et par année.
#' 
#' @param Base Base des Paies, comportant pour l'ensemble de la période. Par défaut, base Paie.
#' @param grade Vecteur de grades particuliers. Tous les grades en l'absence de spécification.
#' @param regexp Vecteur de chaîne de caractères représentant des expressions régulières sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp.
#' @param période Vecteur des années considérées.
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Année}             
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Type}
#'                 \item \code{Grade}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{Montant}}{réel}}}}
#'                 
#' @return Un tableau des charges moyennes de personnel par EQTP (coût employeur) mis en forme avec les grades en ligne et autant de colonnes numériques que d'années de période, plus une colonne de libellés.
#' @examples
#' charges.eqtp()
#' @export

charges.eqtp <- function(Base = Paie, grade = NULL, regexp = NULL, libellés = NULL, agr = FALSE, période = NULL) {
  
  T <- analyse.regexp(Base, regexp, agr)
    
   if (! is.null(libellés) && length(libellés) != dim) {
      
      message("Le vecteur des libellés doit avoir la même longueur que le vecteur des expressions régulières")
      return(NULL)
    }
  
  if (agr) {
          
          Gr <- "G"
          message("Agrégation des grades")
          
  } else {
          
          if (is.null(regexp) & ! is.null(grade))   T <- Base[Statut !=  "ELU" & Grade %chin% grade]
          
          Gr <- "Grade" 
  }

  
  A <- T[ , .(Coût = round(sum(Montant[Type != "AV" & Type != "RE" & Type != "D"], na.rm = TRUE)),
                               eqtp = sum(quotité[1], na.rm = TRUE) / 12),
                             keyby=c("Année", Gr, "Matricule", "Mois")
           ][ , .(Coût.moyen.cum = sum(Coût, na.rm = TRUE),
                  eqtp.cum = sum(eqtp, na.rm = TRUE)),
                 by = c("Année", Gr)
           ][ , Coût.moyen := if (is.na(eqtp.cum) || is.na(Coût.moyen.cum) || eqtp.cum == 0) 0 else round(Coût.moyen.cum / eqtp.cum), 
                 by = c("Année", Gr)]

  
  
  if (! is.null(période)) A <- A[Année %in% période]
  
  if (agr) {
    
    A <- dcast(A, G ~ Année, value.var = "Coût.moyen", fill = 0)
    if (! is.null(libellés)) A$G <- libellés  
    names(A)[1] <- "Catégorie de Grades"
  } else {
    
    A <- dcast(A, Grade ~ Année, value.var = "Coût.moyen", fill = 0)
  }
  
  A
}


#' Année de comparaison avec les données nationales.
#'
#' Calcule l'année à laquelle la pyramide des âges va être comparée aux données nationales, pour un versant donné de la focntion publique.
#'
#' @param versant Chaîne de caractères parmi "FPT", "FPH", "TIT_FPT", "TIT_FPH", "NONTIT_FPT", "NONTIT_FPH".
#' @return Une liste dont les composantes sont :
#'        \describe{
#'         \item{\code{année}}{l'année de comparaison (entier).}
#'         \item{\code{pyr}}{la base de données nationales correspondante (data.table)}.
#'         }
#' @export
 
année_comparaison <- function(versant) {
  
  p <- NULL
  
  for (a in 2010:fin.période.sous.revue) {
    if (exists(p0 <- "pyr_" %+% a %+% "_" %+% tolower(versant))) {
      année <- a
      p <- p0
    }
  }
  
  # --- sanity checks
  stopifnot(!is.null(p))
  pyr = if (!is.null(p)) get(p) else NULL
  stopifnot(toupper(pyr[1, versant]) == versant)
  stopifnot(pyr[1, année.référence]  == année)
  
  # ---
  
  return(list(année = année, pyr = pyr))
}



# Age fin décembre de l'Année en années révolues
# On trouve quelques valeurs correspondant à des NIr non conventionnels
# 3, 4 : en cours d'immatriculation
# 7, 8 : immatriculation temporaire

#' Traitement du NIR (numéro d'inscription au répertoire des personnes physiques).
#'
#' Extrait la répartition par âge et sexe des individus ayant un NIR.
#'
#' @param Base data.table contenant au moins une variable nommée Nir décrivant le NIR.
#' @param année Année civile à la fin de laquelle est évalué l'âge de l'individu.
#' @return Une base data.table ayant la forme suivante (les bornes d'âge ne sont pas impératives) :
#'        \tabular{ccc}{
#'          age \tab Hommes \tab Femmes \cr
#'          15  \tab   0  \tab    1   \cr
#'          16  \tab   NA \tab    2   \cr
#'          17  \tab   1  \tab    3   \cr
#'          18  \tab  409 \tab    52  \cr
#'          ... \tab  ... \tab ...    \cr
#'          68  \tab 2216 \tab    NA
#'        }
#' @examples
#' extraire.nir(Base, 2012)
#' @export

extraire.nir <- function(Base, année)  {

  Base[ , `:=`(age = année - (as.numeric(substr(Nir, 2, 3)) + 1900),
               sexe = substr(Nir, 1, 1))]

  Base[ , age := ifelse(age > 99, age - 100, age)]

  temp <- Base[age < 69 & age > 14]

  H <- temp[sexe == "1" | sexe == "3" | sexe == "7",  .(Hommes = .N), by = "age"]
  F <- temp[sexe == "2" | sexe == "4" | sexe == "8",  .(Femmes = .N), by = "age"]

  HF <- merge(
    merge(H, F, by = "age", all = TRUE),
    data.table(age = 15:68),
    by = "age",
    all = TRUE)

  HF
}

# frame version, added since rev. 1.4, 4th September 2014.
# (C) Minato Nakazawa <minato-nakazawa@umin.net>

pyramidf <- function(data, Laxis=NULL, Raxis=NULL,
                     frame=c(-1.15, 1.15, -0.05, 1.1),
                     AxisFM="d", AxisBM="", AxisBI=3, Cgap=0.3, Cstep=5, Csize=1,
                     Rlab="Hommes", Llab="Femmes", Clab="Âges", GL=TRUE, Cadj=-0.03,
                     Rcol="cadetblue1", Lcol="thistle1", Ldens=-1, Rdens=-1, main="",
                     linewidth=2, ...) {

  Left <- data$Femmes

  Right <- data$Hommes

  if (ncol(data)==2) { Center <- row.names(data) } else { Center <- data$age }
  if (is.null(Laxis)) { Laxis <- seq(0,ceiling(max(c(Left,Right), na.rm=TRUE)/10)*10,len=5) }
  if (is.null(Raxis)) { Raxis <- Laxis }

  # setting x-y axes

  BX <- c(-1-Cgap/2,1+Cgap/2)
  BY <- c(-0.05,1.1)
  XC <- function(XB) { (XB-BX[1])*(frame[2]-frame[1])/(2+Cgap)+frame[1] }
  YC <- function(YB) { (YB-BY[1])*(frame[4]-frame[3])/1.15+frame[3] }

  # scaling factors

  LL <- max(Laxis)
  LR <- min(Laxis)
  LS <- LL-LR
  LI <- length(Laxis)
  RL <- min(Raxis)
  RR <- max(Raxis)
  RS <- RR-RL
  RI <- length(Raxis)

  # ticks of axis

  segments(XC(-(Laxis-LR)/LS-Cgap/2),YC(-0.01),XC(-(Laxis-LR)/LS-Cgap/2),YC(0.01))
  segments(XC((Raxis-RL)/RS+Cgap/2),YC(-0.01),XC((Raxis-RL)/RS+Cgap/2),YC(0.01))

  # vertical grid lines

  if (GL) {
    segments(XC(-(Laxis-LR)/LS-Cgap/2),YC(0),XC(-(Laxis-LR)/LS-Cgap/2),YC(1),
             lty=3,col="blue")
    segments(XC((Raxis-RL)/RS+Cgap/2),YC(0),XC((Raxis-RL)/RS+Cgap/2),YC(1),
             lty=3,col="blue")
  }

  # axes

  lines(c(XC(-1-Cgap/2),XC(-Cgap/2)),c(YC(0),YC(0)),lty=1)
  lines(c(XC(-Cgap/2),XC(-Cgap/2)),c(YC(0),YC(1)),lty=1)
  lines(c(XC(1+Cgap/2),XC(Cgap/2)),c(YC(0),YC(0)),lty=1)
  lines(c(XC(Cgap/2),XC(Cgap/2)),c(YC(0),YC(1)),lty=1)

  # labels

  text(XC(-0.5-Cgap/2),YC(1),Llab,pos=3)
  text(XC(0.5+Cgap/2),YC(1),Rlab,pos=3)
  text(XC(0),YC(1),Clab,pos=3)
  Ci <- length(Center)
  for (i in 0:(Ci-1)) {
    if ((i%%Cstep)==0) { text(XC(0),YC(i/Ci+Cadj),paste(Center[i+1]),pos=3,cex=Csize) }
  }
  text(XC(-(Laxis-LR)/LS-Cgap/2),YC(rep(0,LI)),
       paste(formatC(Laxis,format=AxisFM,big.mark=AxisBM,big.interval=AxisBI)),pos=1)
  text(XC((Raxis-RL)/RS+Cgap/2),YC(rep(0,RI)),
       paste(formatC(Raxis,format=AxisFM,big.mark=AxisBM,big.interval=AxisBI)),pos=1)

  # main text (above the frame)

  if (length(main)>0) { text(XC(0), YC(1.1), main, pos=3) }

  # draw rectangles

  VB <- 0:(Ci-1)/Ci
  VT <- 1:Ci/Ci
  LeftP <- -(Left-LR)/LS-Cgap/2
  rect(XC(LeftP),YC(VB),XC(rep(-Cgap/2,Ci)),YC(VT),col=Lcol,density=Ldens, lwd=linewidth)
  RightP <- (Right-RL)/RS+Cgap/2
  rect(XC(rep(Cgap/2,Ci)),YC(VB),XC(RightP),YC(VT),col=Rcol,density=Rdens,lwd=linewidth)

  return(list(Raxis, Laxis))
}

#' Pyramide des âges.
#'
#' Elabore une pyramide des âges verticale avec superposition du début et de la fin de la période sous revue.
#'
#' @param Avant data.table/data.frame décrivant la situation en début de période
#'        Cette base doit avoir la forme suivante (bornes d'âges non impératifs):
#'        \tabular{ccc}{
#'          age \tab Hommes \tab Femmes \cr
#'          15  \tab   0  \tab    1   \cr
#'          16  \tab   NA \tab    2   \cr
#'          17  \tab   1  \tab    3   \cr
#'          18  \tab  409 \tab    52  \cr
#'          ... \tab  ... \tab ...    \cr
#'          68  \tab 2216 \tab    NA
#'        }
#'        dans laquelle "age" peut être soit un vecteur de nom de lignes soit une colonne.
#' @param Après data.table/data.frame décrivant la situation en fin de période. Même format que \code{Avant}.
#' @param titre Titre du graphique.
#' @param date.début date du début de la période.
#' @param date.fin date de fin de période.
#' @param versant Si non renseigné, sans effet. Si renseigné par "FPT" (resp. "FPH"), le deuxième argument \code{Après} ne
#'                doit pas être renseigné. Il est automatiquement remplacé par une base de données disponible dans le répertoire \code{data/}
#'                du paquet, correspondant à l'année la plus proche du versant de la fonction publique correspondant. La pyramide superposée représente
#'                celle qu'aurait l'organisme si la distribution de ses âges était celle du versant mentionné de la fonction publique.
#' @param couleur_H couleur utilisée pour représenter les hommes (partie droite de la pyramide). Par défaut \code{darkslateblue}
#' @param couleur_F couleur utilisée pour représenter les femmes (partie gauche de la pyramide). Par défaut \code{firebrick4}
#' @return Une liste de deux vecteurs numériques représentant chacun des axes (gauche puis droit).
#'         Un graphique comprenat une pyramide, une légende et éventuellement un titre.
#' @examples
#' pyramide_ages(df1, NULL, "Pyramide des âges", 2008, 2012, versant = "FPT", comparer = TRUE)
#' @export

pyramide_ages <- function(Avant,
                          Après = NULL,
                          titre = "",
                          date.début = début.période.sous.revue,
                          date.fin = fin.période.sous.revue,
                          versant = "",
                          couleur_H = "darkslateblue",
                          couleur_F = "firebrick4") {

  
  if (versant != "") {

    compar <- année_comparaison(versant)
    année.référence <- compar$année
    pyr <- compar$pyr
    
    if (is.null(pyr)) {
      cat("La comparaison ne peut pas être effectuée.")
      return(0)
    }

    tot <- pyr[ , .(H = sum(Hommes), F = sum(Femmes))]
    
    s.avant <- Avant[ , .(H = sum(Hommes, na.rm=TRUE), F = sum(Femmes, na.rm=TRUE))]

    H.coef.forme <- s.avant$H / tot$H
    F.coef.forme <- s.avant$F / tot$F

    pyr[ , `:=`(Hommes = Hommes * H.coef.forme,
                Femmes = Femmes * F.coef.forme)]

    leg <- ""
    if (grepl("non", versant, ignore.case = TRUE)) leg <- "non-"
    if (grepl("tit", versant, ignore.case = TRUE)) leg <- leg %+% "titulaires "      
    if (grepl("fpt", versant, ignore.case = TRUE)) leg <- leg %+% "FPT"      
    if (grepl("fph", versant, ignore.case = TRUE)) leg <- leg %+% "FPH"      
    
    pyramide_ages(Avant,
                  pyr,
                  "Comparaison avec les données nationales au 31 décembre " %+% année.référence,
                  "organisme " %+% date.fin,
                  paste(leg, année.référence))

    cat("Pour obtenir les effectifs nationaux, multiplier les abscisses des hommes par", formatC(round(1 / H.coef.forme), big.mark = " "),
        "et les abscisses des femmes par", formatC(round(1 / F.coef.forme), big.mark = " "))

    return(0)

  }

  plot(c(0,100), c(0,100), type = "n", frame = FALSE, axes = FALSE, xlab = "", ylab = "",
       main = titre)

  axes <- pyramidf(Avant, frame = c(10, 75, 0, 90), linewidth = 1)

  if (! is.null(Après)) {

    pyramidf(Après, Laxis = axes[[1]], Raxis = axes[[2]], frame = c(10, 75, 0, 90),
             Rcol = couleur_H, Lcol = couleur_F,
             #Lcol="deepskyblue", Rcol = "deeppink",
             Ldens = 7, Rdens = 7)

    legend("right", fill = c("thistle1", "cadetblue1", "firebrick4", "darkslateblue"), density = c(NA, NA, 25, 25),
           legend = c("Femmes " %+% date.début, "Hommes " %+% date.début,
                      "Femmes " %+% date.fin, "Hommes " %+% date.fin), cex = 0.8)
  } else {

    legend("right", fill = c("thistle1", "cadetblue1"), density = c(NA, NA),
           legend = c("Femmes " %+% date.début, "Hommes " %+% date.début), cex = 0.8)
  }

  return(0)

}

