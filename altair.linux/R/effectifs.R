


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

effectifs <- function(période, Bulletins = Bulletins.paie,
                       personnels = Analyse.remunerations,
                       Analyse.v = Analyse.variations)  {
  essayer(effectifs_(période, 
                    Bulletins,
                    personnels,
                    Analyse.v), "Les effectifs n'ont pas pu être calculés")
}

# Bulletins : "Matricule", "Statut", "permanent", "quotité", "nb.mois", "Grade"
# Analyse.rémunérations : Filtre_actif, Filtre_annexe, Statut, Matricule, Année (+filtres sur lignes)
# Analyse.v : temps.complet, est.rmpp, statut, Matricule, Année, permanent   (+ filtres sur lignes)

effectifs_ <- function(période, Bulletins = Bulletins.paie,
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

effectifs.locale <- as.data.table(lapply(eff,
                                   function(x) formatC(x, big.mark = " ", format="f", digits=1, decimal.mark=",")))

tableau.effectifs <- cbind(row.names = c("Matricules gérés en base (a)",
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
                                                 "Total ETPT postes actifs non annexes (g)"),
                           effectifs.locale)

setnames(tableau.effectifs, 1, "Effectifs")
for (i in seq_along(période)) setnames(tableau.effectifs, i + 1, as.character(période[i]))

return(tableau.effectifs)
}

analyse.regexp <- function(Base, classe, agr) {

      if (is.null(classe)) {
        
        if (agr) {
          stop("Une expression régulière doit être entrée pour agr = TRUE") 
        } else return(Base)
      }
  
      
      dim <- length(classe)
   
      if (is.list(classe)) {
        
        for (i in 1:dim)  Base[Statut !=  "ELU", paste0("G", i) :=  Grade %chin% classe[[i]] * i]
        
      } else {
      
        for (i in 1:dim)  Base[Statut !=  "ELU", paste0("G", i) :=  grepl(classe[i], Grade, ignore.case = TRUE, perl = TRUE) * i]  
      }
        
      
      G <- rowSums(Base[ , paste0("G", 1:dim), with = FALSE])
      Base <- cbind(Base, G)
      Base[G != 0]

}

#' Tableau des EQTP par grade.
#'
#' Elabore un tableau des équivalents temps plein travaillés par grade et par année.
#'
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la période
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
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.
#' @param service Services. Vecteur de chaînes de caractères exactes. Tous les services en l'absence de spécification.
#' @param classe Liste de vecteurs de grades, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param période Vecteur des années considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param catégorie Catégorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @return Un tableau des effectifs mis en forme avec les grades en ligne et autant de colonnes numériques que d'années de période, plus une colonne de libellés.
#' @examples
#' eqtp.grade()
#' @export

eqtp.grade <- function(Base = Bulletins.paie, 
                       grade = NULL,
                       classe = NULL,
                       service = NULL,
                       libellés = NULL, 
                       agr = FALSE,
                       période = NULL,
                       variation = FALSE,
                       statut = NULL,
                       catégorie = NULL) {

  if (! is.null(libellés) && length(libellés) != length(classe)) {
      
      message("Le vecteur des libellés doit avoir la même longueur que le vecteur des expressions régulières")
      return(NULL)
  }
  
  T <- analyse.regexp(Base, classe, agr)
  
  if (agr) {
          
          Gr <- "G"
          message("Agrégation des grades")
          
  } else {
          
          if (is.null(classe) & ! is.null(grade))   T <- Base[Statut !=  "ELU" & Grade %chin% grade]
          
          Gr <- "Grade" 
  }
  
  T <- T[Statut != "ELU"]
  
  if (is.null(catégorie)) {
        
      if (is.null(statut)) {
        if (is.null(service)) {
          
            tableau.effectifs <- T[ , .(eqtp.g = sum(quotité, na.rm = TRUE) / 12),
                                                                   by=c("Année", Gr)
                                                ][ , eqtp.grade := formatC(eqtp.g, digits=2, format = "f")]
        } else {
          
            tableau.effectifs <- T[Service %chin% service, .(eqtp.g = sum(quotité, na.rm = TRUE) / 12),
                                                                             by=c("Année", Gr)
                                   ][ , eqtp.grade := formatC(eqtp.g, digits=2, format = "f")]
        }
      } else {
        
        if (is.null(service)) {
          
          tableau.effectifs <- T[Statut %chin% statut, .(eqtp.g = sum(quotité, na.rm = TRUE) / 12),
                                 by=c("Année", Gr)
                                 ][ , eqtp.grade := formatC(eqtp.g, digits=2, format = "f")]
        } else {
          
          tableau.effectifs <- T[Statut %chin% statut & Service %chin% service, .(eqtp.g = sum(quotité, na.rm = TRUE) / 12),
                                 by=c("Année", Gr)
                                 ][ , eqtp.grade := formatC(eqtp.g, digits=2, format = "f")]
        }
      }
    
  } else {
    
    if (is.null(statut)) {
      if (is.null(service)) {
        
        tableau.effectifs <- T[Catégorie == catégorie, .(eqtp.g = sum(quotité, na.rm = TRUE) / 12),
                               by=c("Année", Gr)
                               ][ , eqtp.grade := formatC(eqtp.g, digits=2, format = "f")]
      } else {
        
        tableau.effectifs <- T[Service %chin% service & Catégorie == catégorie, .(eqtp.g = sum(quotité, na.rm = TRUE) / 12),
                               by=c("Année", Gr)
                               ][ , eqtp.grade := formatC(eqtp.g, digits=2, format = "f")]
      }
    } else {
      
      if (is.null(service)) {
        
        tableau.effectifs <- T[Statut %chin% statut & Catégorie == catégorie, .(eqtp.g = sum(quotité, na.rm = TRUE) / 12),
                               by=c("Année", Gr)
                               ][ , eqtp.grade := formatC(eqtp.g, digits=2, format = "f")]
      } else {
        
        tableau.effectifs <- T[Statut %chin% statut & Service %chin% service & Catégorie == catégorie, .(eqtp.g = sum(quotité, na.rm = TRUE) / 12),
                               by=c("Année", Gr)
                               ][ , eqtp.grade := formatC(eqtp.g, digits=2, format = "f")]
      }
    }
  }
  
  if (! is.null(période)) tableau.effectifs <- tableau.effectifs[Année %in% période]
  
  totaux <- tableau.effectifs[, .(Total = formatC(sum(eqtp.g, na.rm = TRUE), digits = 2, big.mark = " ", format = "f")), keyby = Année]
  totaux <- transpose(data.table(c("Total", totaux$Total)))
  
  if (nrow(tableau.effectifs) == 0) {
    message("La base des effectifs est vide")
    return(tableau.effectifs) 
  }
  
  if (agr) {
    
    tableau.effectifs <- dcast(tableau.effectifs, G ~ Année, value.var = "eqtp.grade", fill = 0)
    if (! is.null(libellés)) tableau.effectifs$G <- libellés[1:length(tableau.effectifs$G)]
    names(tableau.effectifs)[1] <- "Catégorie de Grades"
    
  } else {
    
    tableau.effectifs <- dcast(tableau.effectifs, Grade ~ Année, value.var = "eqtp.grade", fill = 0)  
    
  }
  colnames(totaux) <- colnames(tableau.effectifs)
  
  tableau.effectifs <- rbind(tableau.effectifs, totaux)
  
  début <- names(tableau.effectifs)[2]
  fin <- names(tableau.effectifs)[ncol(tableau.effectifs)]
  
  h <- function(x)  as.numeric(gsub(",", ".", tableau.effectifs[[x]]))
  
  d <- h(2)
  d <- ifelse(d == 0, NA, d)
  
  if (variation || agr) 
    tableau.effectifs[ , paste0("Variation ", début, "-", fin, " (%)") :=  round((h(ncol(tableau.effectifs))/d - 1)*100, 1)]
  
  tableau.effectifs
}


#' Tableau des charges de personnel par grade.
#'
#' Elabore un tableau des charges de personnel (coût) par grade et par année.
#'
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la période
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
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.
#' @param service Services. Vecteur de chaînes de caractères exactes. Tous les services en l'absence de spécification.
#' @param classe Liste de vecteurs de grades, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param période Vecteur des années considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param catégorie Catégorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @return Un tableau des effectifs mis en forme avec les grades en ligne et autant de colonnes numériques que d'années de période, plus une colonne de libellés.
#' @examples
#' charges.personnel()
#' @export

charges.personnel <- function(Base = Paie, grade = NULL, classe = NULL,  service = NULL, libellés = NULL, agr = FALSE, période = NULL) {
  
  if (! is.null(libellés) && length(libellés) != length(classe)) {
      
      message("Le vecteur des libellés doit avoir la même longueur que le vecteur des expressions rationnelles ou la liste de grades")
      return(NULL)
  }
  
  T <- analyse.regexp(Base, classe, agr)
  
  if (agr) {
          
          Gr <- "G"
          message("Agrégation des grades")
          
  } else {
          
          if (is.null(classe) & ! is.null(grade))   T <- Base[Statut !=  "ELU" & Grade %chin% grade]
          
          Gr <- "Grade" 
  }
  
  if (is.null(service)) {
    
    A <-T[Statut !=  "ELU", 
              .(Coût = round(sum(Montant[Type != "AV" & Type != "RE" & Type != "D" (Type != "R" | ! grepl("cot|ret|d[eé]du|R\\.?D\\.?S|C\\.?S\\.?G\\.?", 
                                                                                                          Libellé,
                                                                                                          ignore.case = TRUE,
                                                                                                          perl = TRUE))], na.rm = TRUE))), 
                  keyby=c("Année", Gr)]
    
  } else {
    
    A <-T[Statut !=  "ELU" & Service %chin% service , 
              .(Coût = round(sum(Montant[Type != "AV" & Type != "RE" & Type != "D" (Type != "R" | ! grepl("cot|ret|d[eé]du|R\\.?D\\.?S|C\\.?S\\.?G\\.?", 
                                                                                                          Libellé,
                                                                                                          ignore.case = TRUE,
                                                                                                          perl = TRUE))], na.rm = TRUE))), 
                  keyby=c("Année", Gr)]
    
    
  }
    
  if (! is.null(période)) A <- A[Année %in% période]
  
  totaux <- A[, .(Total = sum(Coût, na.rm = TRUE)),
                  keyby = Année]
  
  totaux <- transpose(data.table(c("Total", totaux$Total)))
  
  if (nrow(A) == 0) {
    message("Base vide")
    return(A)
  }
  
  if (agr) {
    
    A <- dcast(A, G ~ Année, value.var = "Coût", fill = 0)
    if (! is.null(libellés)) A$G <- libellés  
    names(A)[1] <- "Catégorie de Grades"
    
  } else {
    
    A <- dcast(A, Grade ~ Année, value.var = "Coût", fill = 0)
  }
  
  colnames(totaux) <- colnames(A)
  A <- rbind(A, totaux)
  
  début <- names(A)[2]
  fin <- names(A)[ncol(A)]
  
  d <- as.numeric(A[[2]])
  d <- ifelse(d == 0, NA, d)  
  A[ , paste0("Variation ", début, "-", fin, " (%)") :=  round((as.numeric(A[[ncol(A)]])/d - 1)*100, 1)]
  
}

#' Tableau des coûts moyens de personnel par grade.
#'
#' Elabore un tableau des charges de personnel (coût) par grade et par année.
#' 
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la période
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
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.
#' @param service Services. Vecteur de chaînes de caractères exactes. Tous les services en l'absence de spécification.
#' @param classe Liste de vecteurs de grades, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param période Vecteur des années considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param catégorie Catégorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @param exclure.codes Codes de paye à exclure pour le calcul du coût salarial (vecteur de chaînes de caractères).  
#' @return Un tableau des effectifs mis en forme avec les grades en ligne et autant de colonnes numériques que d'années de période, plus une colonne de libellés.
#' @examples
#' charges.eqtp()
#' @export



charges.eqtp <- function(Base = Paie, 
                         grade = NULL,
                         classe = NULL,
                         service = NULL,
                         libellés = NULL, 
                         agr = FALSE,
                         période = NULL,
                         variation = FALSE,
                         statut = NULL,
                         catégorie = NULL,
                         exclure.codes = NULL,
                         quotité.nulle = FALSE)  {
 
  if (! is.null(libellés) && length(libellés) != length(classe)) {
    
    message("Le vecteur des libellés doit avoir la même longueur que le vecteur des expressions régulières")
    return(NULL)
  }
  
  T <- analyse.regexp(Base, classe, agr)
  
  if (agr) {
          
          Gr <- "G"
          message("Agrégation des grades")
          
  } else {
          
          if (is.null(classe) & ! is.null(grade))   T <- Base[Statut !=  "ELU" & Grade %chin% grade]
          
          Gr <- "Grade" 
  }

  if (quotité.nulle) {
     T <- T[Statut != "ELU" & quotité == 0]  
    } else {
     T <- T[Statut != "ELU" & quotité > 0]
    }
  
  if (! is.null(exclure.codes)) T <- T[! Code %chin% exclure.codes]
  
  if (! is.null(période)) T <- T[Année %in% période]
  
   if (is.null(catégorie)) {
    
    if (is.null(statut)) {
      
      if (! is.null(service)) {
  
            T <- T[Service %chin% service]    
      }
      
    } else {
      
      if (is.null(service)) {
        
               T <- T[Statut %chin% statut] 
      } else {
        
        T <- T[Statut %chin% statut & Service %chin% service] 
      }
      
    }
     
   } else {
     
     if (is.null(statut)) {
       
       if (is.null(service)) {
         
         T <- T[Catégorie == catégorie]    
         
       } else {
         
         T <- T[Catégorie == catégorie & Service %chin% service]    
       }
       
     } else {
       
       if (is.null(service)) {
         
         T <- T[Catégorie == catégorie & Statut %chin% statut] 
         
       } else {
         
         T <- T[Catégorie == catégorie & Statut %chin% statut & Service %chin% service] 
       }
       
     }
   }
  
 # Ni un avantage en nature, ni une retenue, ni une déduction (salarié) ni un rappel de retenue/déduction.csg/crds/avantage nat. mais ok : remb. frais frais
 # On sort les élus
  
  A <- T[ , .(Coût = round(sum(Montant[Type %chin%  c("T", "IR", "S", "I", "A", "C") | (Type == "R" & ! grepl("cot|ret|d[eé]du|R\\.?D\\.?S|C\\.?S\\.?G\\.?|av.*nat", 
                                                                                                          Libellé,
                                                                                                          ignore.case = TRUE,
                                                                                                          perl = TRUE))], na.rm = TRUE)),
                
              eqtp = sum(quotité[1], na.rm = TRUE) / 12),
                    keyby=c("Année", Gr, "Matricule", "Mois")
        ][ , .(Coût.moyen.cum = sum(Coût, na.rm = TRUE),
               eqtp.cum = sum(eqtp, na.rm = TRUE)),
                   keyby = c("Année", Gr)
        ]
  
  if (! quotité.nulle) {
    A <- A[ , Coût.moyen.num := ifelse(is.na(eqtp.cum) | is.na(Coût.moyen.cum) | eqtp.cum == 0, 0, round(Coût.moyen.cum / eqtp.cum)),
                       keyby = c("Année", Gr)
            ][ , Coût.moyen := formatC(round(Coût.moyen.num), big.mark = " ", format = "d")]
    moyenne_ <- A[ , .(Moy.num = round(sum(Coût.moyen.cum, na.rm = TRUE)/sum(eqtp.cum, na.rm = TRUE))), keyby = Année
                   ][ , Moy := formatC(Moy.num, big.mark = " ", format = "d")]
    moyenne <- transpose(data.table(c("Moyenne", moyenne_$Moy)))
    moyenne.num <- transpose(data.table(c("Moyenne", moyenne_$Moy.num)))
    
    
  } else {
    
    A <- A[ , Coût.moyen.num := ifelse(is.na(eqtp.cum) | is.na(Coût.moyen.cum), 0, round(Coût.moyen.cum)),
            keyby = c("Année", Gr)
            ][ , Coût.moyen := formatC(round(Coût.moyen.num), big.mark = " ", format = "d")]
    moyenne_ <- A[ , .(Moy.num = round(sum(Coût.moyen.cum, na.rm = TRUE))), keyby = Année
                   ][ , Moy := formatC(Moy.num, big.mark = " ", format = "d")]
    moyenne <- transpose(data.table(c("Total", moyenne_$Moy)))
    moyenne.num <- transpose(data.table(c("Total", moyenne_$Moy.num)))
    
  }

  if (nrow(A) == 0) {
    message("Base vide")
    return(A)
  }
  
  if (agr) {
    
    B <- dcast(A, G ~ Année, value.var = "Coût.moyen", fill = 0)
    B.num <- dcast(A, G ~ Année, value.var = "Coût.moyen.num", fill = 0)
    if (! is.null(libellés)) B$G <- libellés[1:length(B$G)]
    names(B)[1] <- "Catégorie de Grades"
    
  } else {
    
    B <- dcast(A, Grade ~ Année, value.var = "Coût.moyen", fill = 0)
    B.num <- dcast(A, Grade ~ Année, value.var = "Coût.moyen.num", fill = 0)
  }
  
  colnames(moyenne) <- colnames(B)
  colnames(moyenne.num) <- colnames(B.num)
  
  B <- rbind(B, moyenne)
  B.num <- rbind(B.num, moyenne.num)
  
  début <- names(B)[2]
  fin <- names(B)[ncol(B)]
  
  d <- as.numeric(B.num[[2]])
  d <- ifelse(d == 0, NA, d)
  
  if (variation || agr) 
    B[ , paste0("Variation ", début, "-", fin, " (%)") :=  round((as.numeric(B.num[[ncol(B.num)]])/d - 1)*100, 1)]
  
  B
}


#' Tableau des rémunérations nettes moyennes par grade.
#'
#' Elabore un tableau des rémunérations nettes moyennes du personnel par grade et par année (hors SFT).
#' 
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la période
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
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.
#' @param service Services. Vecteur de chaînes de caractères exactes. Tous les services en l'absence de spécification.
#' @param classe Liste de vecteurs de grades, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param période Vecteur des années considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param catégorie Catégorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @param exclure.codes Codes de paye à exclure pour le calcul du coût salarial (vecteur de chaînes de caractères).  
#' @return Un tableau des effectifs mis en forme avec les grades en ligne et autant de colonnes numériques que d'années de période, plus une colonne de libellés.
#' @examples
#' net.eqtp()
#' @export



net.eqtp <- function(Base = Paie, 
                         grade = NULL,
                         classe = NULL,
                         service = NULL,
                         libellés = NULL, 
                         agr = FALSE,
                         période = NULL,
                         variation = FALSE,
                         statut = NULL,
                         catégorie = NULL,
                         exclure.codes = NULL,
                         quotité.nulle = FALSE)  {
  
  if (! is.null(libellés) && length(libellés) != length(classe)) {
    
    message("Le vecteur des libellés doit avoir la même longueur que le vecteur des expressions régulières")
    return(NULL)
  }
  
  T <- analyse.regexp(Base, classe, agr)
  
  if (agr) {
    
    Gr <- "G"
    message("Agrégation des grades")
    
  } else {
    
    if (is.null(classe) & ! is.null(grade))   T <- Base[Statut !=  "ELU" & Grade %chin% grade]
    
    Gr <- "Grade" 
  }
  
  if (quotité.nulle) {
    T <- T[Statut != "ELU" & quotité == 0]  
  } else {
    T <- T[Statut != "ELU" & quotité > 0]
  }
  
  if (! is.null(exclure.codes)) T <- T[! Code %chin% exclure.codes]
  
  if (! is.null(période)) T <- T[Année %in% période]
  
  if (is.null(catégorie)) {
    
    if (is.null(statut)) {
      
      if (! is.null(service)) {
        
        T <- T[Service %chin% service]    
      }
      
    } else {
      
      if (is.null(service)) {
        
        T <- T[Statut %chin% statut] 
      } else {
        
        T <- T[Statut %chin% statut & Service %chin% service] 
      }
      
    }
    
  } else {
    
    if (is.null(statut)) {
      
      if (is.null(service)) {
        
        T <- T[Catégorie == catégorie]    
        
      } else {
        
        T <- T[Catégorie == catégorie & Service %chin% service]    
      }
      
    } else {
      
      if (is.null(service)) {
        
        T <- T[Catégorie == catégorie & Statut %chin% statut] 
        
      } else {
        
        T <- T[Catégorie == catégorie & Statut %chin% statut & Service %chin% service] 
      }
      
    }
  }
  
  # Ni un avantage en nature, ni une retenue, ni une déduction (salarié) ni un rappel de retenue/déduction.csg/crds/avantage nat. mais ok : remb. frais frais
  # On sort les élus
  
  T[Type == "S" , sft.net := Montant * (1 -ifelse(Statut == "TITULAIRE" | Statut == "STAGIAIRE", 0.9825 * (0.005 + 0.0750) + 0.01 + 0.05, 0.9825 * (0.005 + 0.0750) + 0.01 + 0.0805))]
  
  A <-T[ ,  .(Net = round(sum(Net.à.Payer[1], na.rm = TRUE)),  
                eqtp = sum(quotité[1], na.rm = TRUE) / 12,
                SFT.net = sum(sft.net, na.rm = TRUE)),
                  keyby=c("Année", Gr, "Matricule", "Mois")
          ][ , .(Net.moyen.cum = sum(Net - SFT.net, na.rm = TRUE),
                 eqtp.cum = sum(eqtp, na.rm = TRUE)),
                  keyby = c("Année", Gr)]
  
  if (! quotité.nulle) {
    A <- A[ , Net.moyen.num := ifelse(is.na(eqtp.cum) | is.na(Net.moyen.cum) | eqtp.cum == 0, 0, round(Net.moyen.cum / eqtp.cum)),
            keyby = c("Année", Gr)
            ][ , Net.moyen := formatC(round(Net.moyen.num), big.mark = " ", format = "d")]
    moyenne_ <- A[ , .(Moy.num = round(sum(Net.moyen.cum, na.rm = TRUE)/sum(eqtp.cum, na.rm = TRUE))), keyby = Année
                   ][ , Moy := formatC(Moy.num, big.mark = " ", format = "d")]
    moyenne <- transpose(data.table(c("Moyenne", moyenne_$Moy)))
    moyenne.num <- transpose(data.table(c("Moyenne", moyenne_$Moy.num)))
    
    
  } else {
    
    A <- A[ , Net.moyen.num := ifelse(is.na(eqtp.cum) | is.na(Net.moyen.cum), 0, round(Net.moyen.cum)),
            keyby = c("Année", Gr)
            ][ , Net.moyen := formatC(round(Net.moyen.num), big.mark = " ", format = "d")]
    moyenne_ <- A[ , .(Moy.num = round(sum(Net.moyen.cum, na.rm = TRUE))), keyby = Année
                   ][ , Moy := formatC(Moy.num, big.mark = " ", format = "d")]
    moyenne <- transpose(data.table(c("Total", moyenne_$Moy)))
    moyenne.num <- transpose(data.table(c("Total", moyenne_$Moy.num)))
    
  }
  
  if (nrow(A) == 0) {
    message("Base vide")
    return(A)
  }
  
    
  if (agr) {
    
    B <- dcast(A, G ~ Année, value.var = "Net.moyen", fill = 0)
    B.num <- dcast(A, G ~ Année, value.var = "Net.moyen.num", fill = 0)
    if (! is.null(libellés)) B$G <- libellés[1:length(B$G)]
    names(B)[1] <- "Catégorie de Grades"
    
  } else {
    
    B <- dcast(A, Grade ~ Année, value.var = "Net.moyen", fill = 0)
    B.num <- dcast(A, Grade ~ Année, value.var = "Net.moyen.num", fill = 0)
  }
  
  colnames(moyenne) <- colnames(B)
  colnames(moyenne.num) <- colnames(B.num)
  
  B <- rbind(B, moyenne)
  B.num <- rbind(B.num, moyenne.num)
  
  début <- names(B)[2]
  fin <- names(B)[ncol(B)]
  
  d <- as.numeric(B.num[[2]])
  d <- ifelse(d == 0, NA, d)
  
  if (variation || agr) 
    B[ , paste0("Variation ", début, "-", fin, " (%)") :=  round((as.numeric(B.num[[ncol(B.num)]])/d - 1)*100, 1)]
  
  B
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
  
  for (a in fin.période.sous.revue:2010) {
    if (exists(p0 <- "pyr_" %+% a %+% "_" %+% tolower(versant), envir = .GlobalEnv)) {
      année <- a
      p <- p0
      break
    }
  }
  
  # --- sanity checks
  stopifnot(!is.null(p))
  pyr <- get(p, .GlobalEnv)
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

