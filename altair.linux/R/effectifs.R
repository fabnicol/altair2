
# personnels
# Analyse.rémunérations[Statut != "ELU"
#         & Filtre_annexe == TRUE
#         & Filtre_actif == TRUE
#         & Annee == x,
#         Matricule]


#' Tableau des effectifs.
#'
#' Elabore un tableau des effectifs et équivalents temps plein travaillés par type de personnel et par annee.
#'
#' @param periode Vecteur des annees de la periode sous revue
#' @param Bulletins Base des bulletins de paye, comportant pour l'ensemble de la periode
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Grade}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite}}{réel entre 0 et 1}
#'                 \item{\code{nb.mois}}{entier entre 0 et 12}}}
#'           \item{ la variable booléenne :
#'               \describe{\item{\code{permanent}}{12 bulletins sur l'annee}}}}.
#' @param Analyse Base des analyses de rémunérations, comptant les variables :
#'        \code{Filtre_actif, Filtre_annexe, Statut, Matricule, Annee}
#' @param Analyse Base des analyses de variations de rémunérations, comptant les variables :
#'       \code{temps.complet, est.rmpp, statut, Matricule, Annee, permanent}
#' @return Un tableau des effectifs mis en forme de 22 lignes et autant de colonnes numériques que d'annees de periode, plus une colonne de libellés.
#' @examples
#' effectifs(2010:2015)
#' @export

effectifs <- function(periode, Bulletins = Bulletins.paie,
                       personnels = Analyse.remunerations,
                       Analyse.v = Analyse.variations)  {
  essayer(label = "+effectifs", effectifs_(periode, 
                    Bulletins,
                    personnels,
                    Analyse.v), "Les effectifs n'ont pas pu être calcules")
}

# Bulletins : "Matricule", "Statut", "permanent", "quotite", "nb.mois", "Grade"
# Analyse.rémunérations : Filtre_actif, Filtre_annexe, Statut, Matricule, Annee (+filtres sur lignes)
# Analyse.v : temps.complet, est.rmpp, statut, Matricule, Annee, permanent   (+ filtres sur lignes)

effectifs_ <- function(periode, Bulletins = Bulletins.paie,
                      personnels = Analyse.remunerations,
                      Analyse.v = Analyse.variations) {

  eff <- lapply(periode,
                function(x) {
                  A <- Bulletins[Annee == x,
                                  .(Matricule, Statut, permanent, quotite, nb.mois, Grade)]

                  E <- unique(A[ , .(Matricule, permanent)])
                  ETP <- unique(Bulletins[Annee == x & Statut != "ELU",
                                               .(quotite, Matricule, Statut, permanent, Mois, nb.mois)])
                  F <- E[permanent == TRUE, ]

                  G <- unique(A[Statut == "TITULAIRE" | Statut == "STAGIAIRE", .(Matricule, permanent)])

                  H <- G[permanent == TRUE, ]

                  postes.non.titulaires <- unique(A[Statut == "NON_TITULAIRE", Matricule])

                  I <- unique(A[Statut == "ELU", .(Matricule, permanent)])
                  J <- I[permanent == TRUE, ]
                  K <- unique(A[Statut != "TITULAIRE" & Statut != "STAGIAIRE" & Grade == "V", .(Matricule, permanent)])
                  L <- unique(A[Grade == "A", .(Matricule, permanent)])
                  postes.non.actifs <- unique(personnels[Statut != "ELU"
                                                                    & Filtre_actif == FALSE
                                                                    & Annee == x,
                                                                    Matricule])
                  postes.actifs.annexes <- unique(personnels[Statut != "ELU"
                                                                        & Filtre_annexe == TRUE
                                                                        & Filtre_actif == TRUE
                                                                        & Annee == x,
                                                                        Matricule])
                  postes.actifs.non.annexes <- unique(personnels[Statut != "ELU"
                                                                            & Filtre_annexe == FALSE
                                                                            & Filtre_actif == TRUE
                                                                            & Annee == x,
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
                    ETP[Mois == 12, sum(quotite, na.rm=TRUE)],  # Total ETP/annee
                    ETP[ , sum(quotite, na.rm=TRUE)] / 12,     # Total ETPT/annee
                    ETP[Matricule %chin% unique(Analyse.v[est.rmpp == TRUE    # Total ETPT/annee personnes en place
                                                                   & Annee == x,
                                                                   Matricule]),
                        sum(quotite, na.rm=TRUE)] / 12,

                    ETP[(Statut == "TITULAIRE" | Statut == "STAGIAIRE")       # Total ETPT/annee fonctionnaires
                        & Matricule %chin% unique(Analyse.v[Statut == "TITULAIRE"
                                                                     | Statut == "STAGIAIRE",
                                                                     Matricule]),
                        sum(quotite, na.rm=TRUE)] / 12,
                    ETP[Statut == "TITULAIRE"                                 # Total ETPT/annee titulaires à temps complet
                        & permanent == TRUE
                        & Matricule %chin% unique(Analyse.v[permanent == TRUE
                                                                     & statut == "TITULAIRE"
                                                                     & temps.complet == TRUE
                                                                     & Annee == x,
                                                                     Matricule]),
                        sum(quotite, na.rm=TRUE)] / 12,

                    ETP[Statut == "NON_TITULAIRE"                             # Total ETPT non titulaires
                        & Matricule %chin% postes.non.titulaires,
                        sum(quotite, na.rm=TRUE)] / 12,

                    ETP[Statut == "AUTRE_STATUT"                              # Total ETPT autre statut
                        & Matricule %chin% unique(personnels[Statut == "AUTRE_STATUT",
                                                                        Matricule]),
                        sum(quotite, na.rm=TRUE)] / 12,

                    ETP[Matricule %chin% postes.non.actifs, sum(quotite, na.rm=TRUE)] / 12,         # Total ETPT postes non actifs
                    ETP[Matricule %chin% postes.actifs.annexes, sum(quotite, na.rm=TRUE)] / 12,     # Total ETPT postes actifs annexes
                    ETP[Matricule %chin% postes.actifs.non.annexes, sum(quotite, na.rm=TRUE)] / 12)	# Total ETPT actif non annexes
                    
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
                                                 "Total ETP au 31/12 (d)",
                                                 "Total ETPT/annee (e)",
                                                 "Total ETPT/annee personnes en place (f)(g)",
                                                 "Total ETPT/annee fonctionnaires (g)",
                                                 "Total ETPT/annee titulaires à temps complet (g)",
                                                 "Total ETPT non titulaires (g)",
                                                 "Total ETPT autre statut",
                                                 "Total ETPT postes non actifs (g)",
                                                 "Total ETPT postes actifs annexes (g)",
                                                 "Total ETPT postes actifs non annexes (g)"),
                           effectifs.locale)

setnames(tableau.effectifs, 1, "Effectifs")
for (i in seq_along(periode)) setnames(tableau.effectifs, i + 1, as.character(periode[i]))

return(tableau.effectifs)
}



formater <- function(A, variation,  agr, somme = FALSE, round = TRUE, type = "G", libellés) {

  if (nrow(A) == 0 || is.null(A)) {
    message("Base vide")
    return(A)
  }
  
  
  if (type == "G") {
      if (agr) {
        
        B <- dcast(A, G ~ Annee, value.var = "VAR", fill = 0)
        if (! is.null(libellés)) B$G <- libellés[1:length(B$G)]
        names(B)[1] <- "Catégorie de Grades"
        
      } else {
        
        B <- dcast(A, Grade ~ Annee, value.var = "VAR", fill = 0)
      }
    
   } else {
     if (type == "E") {
      if (agr) {
        
        B <- dcast(A, G ~ Annee, value.var = "VAR", fill = 0)
        if (! is.null(libellés)) B$E <- libellés[1:length(B$E)]
        names(B)[1] <- "Catégorie d'Emplois"
        
      } else {
        
        B <- dcast(A, Emploi ~ Annee, value.var = "VAR", fill = 0)
      }
     } else {
       if (agr) {
         
         B <- dcast(A, G ~ Annee, value.var = "VAR", fill = 0)
         if (! is.null(libellés)) B$E <- libellés[1:length(B$E)]
         names(B)[1] <- "Catégorie statutaires"
         
       } else {
         
         B <- dcast(A, Categorie ~ Annee, value.var = "VAR", fill = 0)
       }
     }
    }
  
    if (somme) {
      BL <- B[,-1][, lapply(.SD, sum, na.rm = TRUE)]
      BL  <- cbind(0, round(BL, 1)) 
      B <- rbind(B, BL, use.names = FALSE)
      
    } else {
      
      # On ne fait la moyenne que sur les valeurs non nulles
      
      Moyenne <- A[VAR != 0  , .(Moy.num = weighted.mean(VAR, eqtp.cum, na.rm = TRUE)), keyby = Annee]
      
      moyenne.num  <- transpose(data.table(c(0, Moyenne[, Moy.num[1], by = Annee][[2]])))
      
      setnames(moyenne.num, c(names(B)[1], as.character(Moyenne$Annee)))
               
      B <- rbind(B, moyenne.num, fill = TRUE)
    }
    
    
    if (round) {
      tab <- cbind(B[[1]], B[, lapply(.SD, function(x) round(as.numeric(x), 1)), .SDcols = -1])
      
      setnames(tab, 1, names(B)[1])
    } else tab <- B
    
    
    if (variation) {
      
      début <- names(B)[2]
      fin   <- names(B)[ncol(B)]
      tab$V <-  round((B[[fin]]/B[[début]] - 1) * 100, 1)
      
      setnames(tab, ncol(tab), paste0("Variation ", début, "-", fin))
      
    }
    
    set(tab, nrow(tab), 1, ifelse(somme, "Total", "Moyenne"))
    
  return(tab)
}

formater2 <- function(A, variation, fichier, groupe, agr, somme = FALSE, round = TRUE) {
  
   if (nrow(A) == 0 || is.null(A)) {
    message("Base vide")
    return(A)
   }
  
  essayer({
    A[ , {
       
        fwrite(formater(.SD, variation,  agr, somme, round, type = "G"),
               paste0(fichier, gsub("/", "-", get(groupe)), ".csv"),
               bom = TRUE,
               sep = ";",
               dec = ",")
      
    }, by = groupe]
  }, "Could not save " %+% paste0(fichier, gsub("/", "-", get(groupe)), ".csv") )
  
  return(NULL)
}


#' Tableau des EQTP par grade.
#'
#' Elabore un tableau des équivalents temps plein travaillés par grade et par annee.
#'
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la periode
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Annee}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Grade}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite}}{réel entre 0 et 1}}}}.
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.
#' @param service Services. Vecteur de chaînes de caractères exactes. Tous les services en l'absence de spécification.
#' @param classe Liste de vecteurs de grades, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param periode Vecteur des annees considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param categorie Categorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @param quotite.nulle [défaut FALSE] Si TRUE, lorsque la quotité est nulle, faire comme si elle valait 1. A n'utiliser que dans le cas de défauts de bases qui utilisent 0 pour les quotités de temps plein (attesté mais rare).
#' @return Un tableau des effectifs par grade mis en forme avec les grades en ligne et autant de colonnes numériques que d'annees de periode, plus une colonne de libellés.
#' @examples
#' eqtp.grade()
#' @export

eqtp.grade <- function(Base = Bulletins.paie, 
                       grade = NULL,
                       emploi = NULL,
                       classe = NULL,
                       service = NULL,
                       libellés = NULL, 
                       agr = FALSE,
                       periode = NULL,
                       variation = FALSE,
                       statut = NULL,
                       categorie = NULL,
                       exclure.codes = NULL,
                       quotite.nulle = FALSE) {

 
  T <- filtrer.base(Base, grade, emploi, classe, service, libellés, agr, periode, statut, categorie, exclure.codes, quotite.nulle, type = "G")
  
  if (is.null(T) || nrow(T) == 0) return(NULL)
  
  T <- T [, .(VAR = sum(quotite, na.rm = TRUE) / 12), by = c("Annee",  groupage(type = "G", agr))]
  
  formater(T, variation, agr, somme = TRUE, round = TRUE, type = "G", libellés)
}


#' Tableau des EQTP par emploi.
#'
#' Elabore un tableau des équivalents temps plein travaillés par emploi et par annee.
#'
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la periode
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Annee}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Emploi}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite}}{réel entre 0 et 1}}}}.
#' @param emploi Emploi particulier. Tous les emplois en l'absence de spécification.
#' @param service Services. Vecteur de chaînes de caractères exactes. Tous les services en l'absence de spécification.
#' @param classe Liste de vecteurs d'emplois, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les emplois. Tous les emplois en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations d'emplois par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param periode Vecteur des annees considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param categorie Categorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @param quotite.nulle [défaut FALSE] Si TRUE, lorsque la quotité est nulle, faire comme si elle valait 1. A n'utiliser que dans le cas de défauts de bases qui utilisent 0 pour les quotités de temps plein (attesté mais rare).
#' @return Un tableau des effectifs par type d'emploi mis en forme avec les emplois en ligne et autant de colonnes numériques que d'annees de periode, plus une colonne de libellés.
#' @examples
#' eqtp.emploi()
#' @export

eqtp.emploi <- function(Base = Bulletins.paie, 
                       emploi = NULL,
                       classe = NULL,
                       service = NULL,
                       libellés = NULL, 
                       agr = FALSE,
                       periode = NULL,
                       variation = FALSE,
                       statut = NULL,
                       categorie = NULL, 
                       exclure.codes = NULL,
                       quotite.nulle = FALSE) {
  
  T <- filtrer.base(Base, grade, emploi, classe, service, libellés, agr, periode, statut, categorie, exclure.codes, quotite.nulle, type = "E")
  
  if (is.null(T) || nrow(T) == 0) return(NULL)
  
  T <- T [, .(VAR = sum(quotite, na.rm = TRUE) / 12), by = c("Annee",  groupage(type = "E", agr))]
  
  formater(T, variation, agr, somme = TRUE, round = TRUE, type = "E", libellés)
 
}


#' Tableau des EQTP par grade et service.
#'
#' Elabore un tableau des équivalents temps plein travaillés par grade, service et par annee.
#'
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la periode
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Annee}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Grade}
#'                 \item \code{Service}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite}}{réel entre 0 et 1}}}}.
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.
#' @param classe Liste de vecteurs de grades, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param periode Vecteur des annees considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param categorie Categorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @return Un tableau des effectifs par grade et service mis en forme avec les grades en ligne et autant de colonnes numériques que d'annees de periode, plus une colonne de libellés.
#' @examples
#' eqtp.grade.serv()
#' @export

eqtp.grade.serv <- function(Base = Bulletins.paie, 
                       grade = NULL,
                       classe = NULL,
                       libellés = NULL, 
                       agr = FALSE,
                       periode = NULL,
                       variation = FALSE,
                       statut = NULL,
                       categorie = NULL,
                       exclure.codes = NULL,
                       quotite.nulle = FALSE) {
  
  T <- filtrer.base(Base, grade, emploi, classe, service, libellés, agr, periode, statut, categorie, exclure.codes, quotite.nulle, type = "G")
  
  if (is.null(T) || nrow(T) == 0) return(NULL)

  curD <- getwd()
  
  setwd(file.path(chemin.dossier.bases, "Effectifs"))
  
  formater2(T [ , .(VAR = sum(quotite, na.rm = TRUE) / 12), by = c("Annee",  groupage(type, agr), "Service")],
            variation,
            fichier = "effectifs.serv.", 
            groupe = "Service", 
            agr, 
            somme = TRUE,
            round  = TRUE)
  
  setwd(curD)
}

#' Tableau des EQTP par grade et categorie.
#'
#' Elabore un tableau des équivalents temps plein travaillés par grade et categorie.
#'
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la periode
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Annee}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Grade}
#'                 \item \code{Service}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite}}{réel entre 0 et 1}}}}.
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.
#' @param classe Liste de vecteurs de grades, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param periode Vecteur des annees considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @return Un tableau des effectifs par grade et categorie mis en forme avec les grades en ligne et autant de colonnes numériques que d'annees de periode, plus une colonne de libellés.
#' @examples
#' eqtp.grade.cat()
#' @export

eqtp.grade.cat <- function(Base = Bulletins.paie, 
                            grade = NULL,
                            classe = NULL,
                            libellés = NULL, 
                            agr = FALSE,
                            periode = NULL,
                            variation = FALSE,
                            statut = NULL,
                            exclure.codes = NULL,
                            quotite.nulle = FALSE) {
  
  T <- filtrer.base(Base, grade, emploi, classe, service, libellés, agr, periode, statut, categorie, exclure.codes, quotite.nulle, type = "C")
  
  if (is.null(T) || nrow(T) == 0) return(NULL)
  
  curD <- getwd()
  
  setwd(file.path(chemin.dossier.bases, "Effectifs"))
  
  formater2(T [ , .(VAR = sum(quotite, na.rm = TRUE) / 12), by = c("Annee",  groupage(type, agr), "Categorie")], 
            variation, 
            fichier = "effectifs.cat.", 
            groupe = "Categorie",
            agr,
            somme = TRUE,
            round = TRUE)
  
  setwd(curD)
}


#' Tableau des coûts moyens par agent et par grade.
#'
#' Elabore un tableau des charges de personnel (coût) par grade et par annee.
#' 
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la periode
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Annee}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Grade}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite}}{réel entre 0 et 1}}}}.
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.
#' @param emploi Emploi particulier. Tous les emplois en l'absence de spécification.
#' @param service Services. Vecteur de chaînes de caractères exactes. Tous les services en l'absence de spécification.
#' @param classe Liste de vecteurs de grades, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param periode Vecteur des annees considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param categorie Categorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @param exclure.codes Codes de paye à exclure pour le calcul du coût salarial (vecteur de chaînes de caractères).  
#' @param quotite.nulle [défaut FALSE] Si TRUE, lorsque la quotité est nulle, faire comme si elle valait 1. A n'utiliser que dans le cas de défauts de bases qui utilisent 0 pour les quotités de temps plein (attesté mais rare).
#' @return Un tableau des coûts moyen par agent et par grade mis en forme avec les grades en ligne et autant de colonnes numériques que d'annees de periode, plus une colonne de libellés.
#' @examples
#' charges.eqtp()
#' @export


charges.eqtp <- function(Base = Paie, 
                         grade = NULL,
                         emploi = NULL,
                         classe = NULL,
                         service = NULL,
                         libellés = NULL, 
                         agr = FALSE,
                         periode = NULL,
                         variation = FALSE,
                         statut = NULL,
                         categorie = NULL,
                         exclure.codes = NULL,
                         quotite.nulle = FALSE,
                         type = "G")  {
 
 formater(calcul.charges(filtrer.base(Base,
                                      grade,
                                      emploi,
                                      classe,
                                      service,
                                      libellés,
                                      agr,
                                      periode,
                                      statut,
                                      categorie,
                                      exclure.codes,
                                      quotite.nulle,
                                      type),
                         var,
                         quotite.nulle,
                         groupage(type, agr)),
          variation,
          agr,
          somme = FALSE,
          round = TRUE,
          type = type,
          libellés)
}


#' Tableau des coûts moyens de personnel par type emploi.
#'
#' Elabore un tableau des charges de personnel (coût) par emploi et par annee.
#' 
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la periode
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Annee}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Emploi}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite}}{réel entre 0 et 1}}}}.
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.
#' @param emploi Emploi particulier. Tous les emplois en l'absence de spécification.
#' @param service Services. Vecteur de chaînes de caractères exactes. Tous les services en l'absence de spécification.
#' @param classe Liste de vecteurs d'emplois, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les emplois. Tous les emplois en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les emplois décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de emplois par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param periode Vecteur des annees considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param categorie Categorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @param exclure.codes Codes de paye à exclure pour le calcul du coût salarial (vecteur de chaînes de caractères).  
#' @param quotite.nulle [défaut FALSE] Si TRUE, lorsque la quotité est nulle, faire comme si elle valait 1. A n'utiliser que dans le cas de défauts de bases qui utilisent 0 pour les quotités de temps plein (attesté mais rare).
#' @return Un tableau des coûts moyens par type d'emploi mis en forme avec les emplois en ligne et autant de colonnes numériques que d'annees de periode, plus une colonne de libellés.
#' @examples
#' charges.eqtp.emploi()
#' @export



charges.eqtp.emploi <- function(Base = Paie, 
                         garde = NULL,
                         emploi = NULL,
                         classe = NULL,
                         service = NULL,
                         libellés = NULL, 
                         agr = FALSE,
                         periode = NULL,
                         variation = FALSE,
                         statut = NULL,
                         categorie = NULL,
                         exclure.codes = NULL,
                         quotite.nulle = FALSE)  {
   
  charges.eqtp(Base, grade, emploi, classe, service, libellés, agr, periode, variation, statut, categorie, exclure.codes, quotite.nulle, type = "E")
}

#' Tableau des coûts moyens de personnel par grade et service.
#'
#' Elabore un tableau des charges moyennes de personnel (coût) par grade et par annee, pour chaque service et donne la moyenne pondérée.
#' 
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la periode
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Annee}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Grade}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite}}{réel entre 0 et 1}}}}.
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.
#' @param classe Liste de vecteurs de grades, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param service Services. Vecteur de chaînes de caractères exactes. Tous les services en l'absence de spécification. 
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param periode Vecteur des annees considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param categorie Catégorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @param exclure.codes Codes de paye à exclure pour le calcul du coût salarial (vecteur de chaînes de caractères).  
#' @param quotite.nulle [défaut FALSE] Si TRUE, lorsque la quotité est nulle, faire comme si elle valait 1. A n'utiliser que dans le cas de défauts de bases qui utilisent 0 pour les quotités de temps plein (attesté mais rare).
#' @return Un tableau des coûts chargés moyens par grade et service mis en forme avec les grades en ligne et autant de colonnes numériques que d'annees de periode, plus une colonne de libellés.
#' @examples
#' charges.eqtp.serv()
#' @export

charges.eqtp.serv <- function(Base = Paie, 
                          grade = NULL,
                          classe = NULL,
                          service = NULL,
                          libellés = NULL, 
                          agr = FALSE,
                          periode = NULL,
                          variation = FALSE,
                          statut = NULL,
                          categorie = NULL,
                          exclure.codes = NULL,
                          quotite.nulle = FALSE)  {
  
  curD <- getwd()
  
  setwd(file.path(chemin.dossier.bases, "Remunerations"))
  
  formater2(calcul.charges(filtrer.base(Base,
                                        grade,
                                        emploi,
                                        classe,
                                        service,
                                        libellés,
                                        agr,
                                        periode,
                                        statut,
                                        categorie,
                                        exclure.codes,
                                        quotite.nulle,
                                        type),
                           var,
                           quotite.nulle,
                           c(groupage(type, agr), "Service")),
            variation,
            fichier = "charges.serv.",
            groupe = "Service",
            agr,
            somme = FALSE,
            round = TRUE)
    
  setwd(curD)
}


filtrer.base <- function(Base, grade, emploi, classe, service, libellés, agr, periode, statut, categorie, exclure.codes, quotite.nulle, type) {
  
  if (is.null(classe)) {
    
    if (agr) {
      stop("Une expression régulière doit être entree pour agr = TRUE") 
    } else return(Base)
  }
  
  if (!is.null(grade)) Base <- Base[Grade   %in% grade]
  
  if (!is.null(emploi)) Base <- Base[Emploi %in% emploi]
  
  dim <- length(classe)
  
  if (is.list(classe)) {
    
    if (type == "G") {
      for (i in 1:dim)  Base[ , paste0("G", i) :=  Grade %chin% classe[[i]] * i] 
    } else {
      if (type == "E") {
         for (i in 1:dim)  Base[ , paste0("G", i) :=  Emploi %chin% classe[[i]] * i] 
      } else {
         for (i in 1:dim)  Base[ , paste0("G", i) :=  Categorie %chin% classe[[i]] * i] 
      }
    }
    
  } else {
    if (is.vector(classe)) {
      if (type == "G") {
        for (i in 1:dim)  Base[ , paste0("G", i) :=  grepl(classe[i], Grade, ignore.case = TRUE, perl = TRUE) * i]  
      } else {
        if (type == "E") {
           for (i in 1:dim)  Base[ , paste0("G", i) :=  grepl(classe[i], Emploi, ignore.case = TRUE, perl = TRUE) * i]  
        } else {
           for (i in 1:dim)  Base[ , paste0("G", i) :=  grepl(classe[i], Categorie, ignore.case = TRUE, perl = TRUE) * i]  
        }
      }
    }
  }
  
  if ("G1" %in% names(Base)) {
    
    G <- rowSums(Base[ , paste0("G", 1:dim), with = FALSE], na.rm = TRUE)
    Base <- cbind(Base, G)
    Base <- Base[G != 0]
  }
  
  
  if (quotite.nulle) {
    Base <- Base[quotite == 0]  
  } else {
    Base <- Base[quotite > 0]
  }
  
  if (! is.null(exclure.codes)) Base <- Base[! Code %chin% exclure.codes]
  
  if (! is.null(periode)) Base <- Base[Annee %in% periode]
  
  if (is.null(categorie)) {
    
    if (is.null(statut)) {
      
      if (! is.null(service)) {
        
        Base <- Base[Service %chin% service]    
      }
      
    } else {
      
      if (is.null(service)) {
        
        Base <- Base[Statut %chin% statut] 
      } else {
        
        Base <- Base[Statut %chin% statut & Service %chin% service] 
      }
      
    }
    
  } else {
    
    if (is.null(statut)) {
      
      if (is.null(service)) {
        
        Base <- Base[Categorie == categorie]    
        
      } else {
        
        Base <- Base[Categorie == categorie & Service %chin% service]    
      }
      
    } else {
      
      if (is.null(service)) {
        
        Base <- Base[Categorie == categorie & Statut %chin% statut] 
        
      } else {
        
        Base <- Base[Categorie == categorie & Statut %chin% statut & Service %chin% service] 
     }
    }
  }

  Base
}

groupage <- function(type, agr) {

  if (agr) {

    Gr <- type


  } else {

    if (type == "E") {

      Gr <- "Emploi"
    } else {

      Gr <- "Grade"
    }

    Gr
  }
}



calcul.rémunération <- function(T, var, quotite.nulle, Gr) {
  
  if (is.null(T)) return(NULL)
  
  A <- T[ ,  .(Rem = sum(get(var)[1], na.rm = TRUE),  
               eqtp = sum(quotite[1], na.rm = TRUE)),
                  keyby = c("Matricule", "Annee", "Mois", Gr)
        ][ ,  .(Rem.moyen.cum = sum(Rem, na.rm = TRUE),
               eqtp.cum = sum(eqtp, na.rm = TRUE) / 12),
                  keyby = c("Annee", Gr)]  
  
  if (! quotite.nulle) {
    
    A[ , VAR := ifelse(is.na(eqtp.cum) | is.na(Rem.moyen.cum) | eqtp.cum == 0, 0, round(Rem.moyen.cum / eqtp.cum)),
       keyby = c("Annee", Gr)]
    
  } else {
    
    A[ , VAR := ifelse(is.na(eqtp.cum) | is.na(Rem.moyen.cum), 0, round(Rem.moyen.cum)),
       keyby = c("Annee", Gr)]
    
  }
}


calcul.charges <- function(T, var, quotite.nulle, Gr) {

  if (is.null(T)) return(NULL)
  
  A <- T[ , .(Cout = sum(Montant[Type == "C" | (Type == "R" & grepl("cot.*(?:emp|pat).*", 
                                                                    Libelle,
                                                                    ignore.case = TRUE,
                                                                    perl = TRUE))]) + Brut[1],
              
              eqtp = sum(quotite[1], na.rm = TRUE)),
          
                 keyby=c("Matricule", "Annee", "Mois", Gr)
          
        ][ , .(Cout.moyen.cum = sum(Cout, na.rm = TRUE),
               eqtp.cum = sum(eqtp, na.rm = TRUE) / 12),
           
                 keyby = c("Annee", Gr)]
  
  if (! quotite.nulle) {
    
    A <- A[ , VAR := ifelse(is.na(eqtp.cum) | is.na(Cout.moyen.cum) | eqtp.cum == 0, 0, round(Cout.moyen.cum / eqtp.cum)),
            
                 keyby = c("Annee", Gr)]
    
  } else {
    
    A <- A[ , VAR := ifelse(is.na(eqtp.cum) | is.na(Cout.moyen.cum), 0, round(Cout.moyen.cum)),
            
                 keyby = c("Annee", Gr)]
  }
}

#' Tableau des rémunérations nettes moyennes par grade.
#'
#' Elabore un tableau des rémunérations nettes moyennes du personnel par grade et par annee (hors SFT).
#' 
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la periode
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Annee}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Grade}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite}}{réel entre 0 et 1}}}}.
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.
#' @param emploi Emploi particulier. Tous les emplois en l'absence de spécification.
#' @param service Services. Vecteur de chaînes de caractères exactes. Tous les services en l'absence de spécification.
#' @param classe Liste de vecteurs de grades, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param periode Vecteur des annees considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param categorie Categorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @param exclure.codes Codes de paye à exclure pour le calcul du coût salarial (vecteur de chaînes de caractères).  
#' @param quotite.nulle [défaut FALSE] Si TRUE, lorsque la quotité est nulle, faire comme si elle valait 1. A n'utiliser que dans le cas de défauts de bases qui utilisent 0 pour les quotités de temps plein (attesté mais rare).
#' @return Un tableau des rémunérations nettes par grade mis en forme avec les grades en ligne et autant de colonnes numériques que d'annees de periode, plus une colonne de libellés.
#' @examples
#' net.eqtp()
#' @export



net.eqtp <- function(Base = Paie, 
                         grade = NULL,
                         emploi = NULL,
                         classe = NULL,
                         service = NULL,
                         libellés = NULL, 
                         agr = FALSE,
                         periode = NULL,
                         variation = FALSE,
                         statut = NULL,
                         categorie = NULL,
                         exclure.codes = NULL,
                         exclure.sft = FALSE,
                         quotite.nulle = FALSE,
                         type = "G")  {
  
  formater(calcul.rémunération(filtrer.base(Base,
                                            grade,
                                            emploi,
                                            classe,
                                            service,
                                            libellés,
                                            agr,
                                            periode,
                                            statut,
                                            categorie,
                                            exclure.codes,
                                            quotite.nulle,
                                            type),
                               "Net.a.Payer",
                               quotite.nulle,
                               groupage(type, agr)),
           variation, 
           agr,
           type,
           somme = FALSE,
           round = TRUE,
           libellés)
}


#' Tableau des rémunérations nettes moyennes par grade et service.
#'
#' Elabore un tableau des rémunérations nettes moyennes du personnel par grade et par annee, par service.
#' 
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la periode
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Annee}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Grade}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite}}{réel entre 0 et 1}}}}.
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.
#' @param emploi Emploi particulier. Tous les emplois en l'absence de spécification.
#' @param classe Liste de vecteurs de grades, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param service Service particulier. Tous les services par défaut.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param periode Vecteur des annees considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param categorie Categorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @param exclure.codes Codes de paye à exclure pour le calcul du coût salarial (vecteur de chaînes de caractères).  
#' @param quotite.nulle [défaut FALSE] Si TRUE, lorsque la quotité est nulle, faire comme si elle valait 1. A n'utiliser que dans le cas de défauts de bases qui utilisent 0 pour les quotités de temps plein (attesté mais rare).
#' @return Un tableau des rémunérations nettes par grade mis en forme avec les grades en ligne et autant de colonnes numériques que d'annees de periode, plus une colonne de libellés.
#' @examples
#' net.eqtp.serv()
#' @export


net.eqtp.serv <- function(Base = Paie, 
                     grade = NULL,
                     emploi = NULL,
                     classe = NULL,
                     service = NULL,
                     libellés = NULL, 
                     agr = FALSE,
                     periode = NULL,
                     variation = FALSE,
                     statut = NULL,
                     categorie = NULL,
                     exclure.codes = NULL,
                     quotite.nulle = FALSE,
                     type = "G")  {
  
  curD <- getwd()
  
  setwd(file.path(chemin.dossier.bases, "Remunerations"))
  
  formater2(calcul.rémunération(filtrer.base(Base, 
                                             grade,
                                             emploi,
                                             classe,
                                             service,
                                             libellés,
                                             agr,
                                             periode,
                                             statut,
                                             categorie,
                                             exclure.codes,
                                             quotite.nulle,
                                             type),
                                "Net.a.Payer",
                                quotite.nulle,
                                c(groupage(type, agr), "Service")),
            variation,
            fichier = "net.serv.",
            groupe  = "Service",
            agr,
            somme   = FALSE,
            round = TRUE)
  
  setwd(curD)
}


#' Tableau des rémunérations nettes moyennes par emploi.
#'
#' Elabore un tableau des rémunérations nettes moyennes du personnel par emploi et par annee (hors SFT).
#' 
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la periode
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Annee}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Emploi}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite}}{réel entre 0 et 1}}}}.
#' @param emploi Emploi particulier. Tous les emplois en l'absence de spécification.
#' @param service Services. Vecteur de chaînes de caractères exactes. Tous les services en l'absence de spécification.
#' @param classe Liste de vecteurs d'emplois, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les emplois. Tous les emplois en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les emplois décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations d'emplois par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param periode Vecteur des annees considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param categorie Categorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @param exclure.codes Codes de paye à exclure pour le calcul du coût salarial (vecteur de chaînes de caractères).  
#' @param exclure.sft Exclure le SFT du calcul
#' @param quotite.nulle [défaut FALSE] Si TRUE, lorsque la quotité est nulle, faire comme si elle valait 1. A n'utiliser que dans le cas de défauts de bases qui utilisent 0 pour les quotités de temps plein (attesté mais rare).
#' @return Un tableau des rémunérations nettes moyennes par emploi mis en forme avec les emplois en ligne et autant de colonnes numériques que d'annees de periode, plus une colonne de libellés.
#' @examples
#' net.eqtp.emploi()
#' @export



net.eqtp.emploi <- function(Base = Paie, 
                     grade = NULL,                            
                     emploi = NULL,
                     classe = NULL,
                     service = NULL,
                     libellés = NULL, 
                     agr = FALSE,
                     periode = NULL,
                     variation = FALSE,
                     statut = NULL,
                     categorie = NULL,
                     exclure.codes = NULL,
                     exclure.sft = FALSE,
                     quotite.nulle = FALSE)  {
  
  net.eqtp(Base, grade, emploi, classe, service, libellés, agr, periode, variation, statut, categorie, exclure.codes, exclure.sft, quotite.nulle, type = "E")
}



#' Tableau des rémunérations brutes moyennes par grade.
#'
#' Elabore un tableau des rémunérations brutes moyennes du personnel par grade et par annee.
#' 
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la periode
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Annee}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Grade}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite}}{réel entre 0 et 1}}}}.
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.
#' @param emploi Emploi particulier. Tous les emplois en l'absence de spécification.
#' @param service Services. Vecteur de chaînes de caractères exactes. Tous les services en l'absence de spécification.
#' @param classe Liste de vecteurs de grades, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les grades. Tous les grades en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les grades décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations de grades par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param periode Vecteur des annees considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param categorie Categorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @param exclure.codes Codes de paye à exclure pour le calcul du coût salarial (vecteur de chaînes de caractères).  
#' @param quotite.nulle [défaut FALSE] Si TRUE, lorsque la quotité est nulle, faire comme si elle valait 1. A n'utiliser que dans le cas de défauts de bases qui utilisent 0 pour les quotités de temps plein (attesté mais rare).
#' @param type "E" (par emploi) ou "G" (par grade, défaut).
#' @return Un tableau des rémunérations brutes moyennes mis en forme avec les grades en ligne et autant de colonnes numériques que d'annees de periode, plus une colonne de libellés.
#' @examples
#' brut.eqtp()
#' @export



brut.eqtp <- function(Base = Paie, 
                     grade = NULL,
                     emploi = NULL,
                     classe = NULL,
                     service = NULL,
                     libellés = NULL, 
                     agr = FALSE,
                     periode = NULL,
                     variation = FALSE,
                     statut = NULL,
                     categorie = NULL,
                     exclure.codes = NULL,
                     quotite.nulle = FALSE,
                     type = "G")  {
  

  formater(calcul.rémunération(filtrer.base(Base,
                                            grade,
                                            emploi,
                                            classe,
                                            service,
                                            libellés,
                                            agr,
                                            periode,
                                            statut,
                                            categorie,
                                            exclure.codes,
                                            quotite.nulle,
                                            type),
                               "Brut",
                               quotite.nulle,
                               groupage(type, agr)),
           variation, 
           agr,
           type,
           somme = FALSE,
           round = TRUE,
           libellés)
}


#' Tableau des rémunérations brutes moyennes par emploi.
#'
#' Elabore un tableau des rémunérations brutes moyennes du personnel par emploi et par annee.
#' 
#' @param Base Base des bulletins de paye, comportant pour l'ensemble de la periode
#'        \enumerate{
#'          \item{ les variables charactère suivantes :
#'             \itemize{
#'                 \item \code{Annee}
#'                 \item \code{Matricule}
#'                 \item \code{Statut}
#'                 \item \code{Emploi}}}
#'           \item{ les variables numériques :
#'               \describe{
#'                 \item{\code{quotite}}{réel entre 0 et 1}}}}.
#' @param grade Grade particulier. Tous les grades en l'absence de spécification.            
#' @param emploi Emploi particulier. Tous les emplois en l'absence de spécification.
#' @param service Services. Vecteur de chaînes de caractères exactes. Tous les services en l'absence de spécification.
#' @param classe Liste de vecteurs d'emplois, chaque vecteur représentant une classe aggrégée, ou bien vecteur de chaîne de caractères représentant des expressions rationnelles sur les emplois. Tous les emplois en l'absence de spécification. La casse est ignorée pour les expressions rationnelles.
#' @param agr Booléen (défaut FALSE). Si TRUE, l'expression régulière précédente conduit à agréger les emplois décrits par le vecteur d'expressions régulières précédent : une ligne par composante du vecteur.
#' @param libellés  Vecteur de libellés des agrégations d'emplois par expression régulière. Doit avoir la même dimension que le vecteur de regexp. 
#' @param periode Vecteur des annees considérées.
#' @param variation Booléen Insérer une colonne des variations (défaut FALSE).
#' @param statut Restreindre le tableau au vecteur des statuts en paramètres. Expressions exactes. Tous statuts par défaut.
#' @param categorie Categorie statutaire (vecteur de lettres parmi 'A', 'B', 'C'). Par défaut A, B, C ou indéterminée.  
#' @param exclure.codes Codes de paye à exclure pour le calcul du coût salarial (vecteur de chaînes de caractères).  
#' @param quotite.nulle [défaut FALSE] Si TRUE, lorsque la quotité est nulle, faire comme si elle valait 1. A n'utiliser que dans le cas de défauts de bases qui utilisent 0 pour les quotités de temps plein (attesté mais rare).
#' @return Un tableau des rémunérations brutes moyennes par emploi mis en forme avec les emplois en ligne et autant de colonnes numériques que d'annees de periode, plus une colonne de libellés.
#' @examples
#' brut.eqtp.emploi()
#' @export



brut.eqtp.emploi <- function(Base = Paie, 
                            grade = NULL, 
                            emploi = NULL,
                            classe = NULL,
                            service = NULL,
                            libellés = NULL, 
                            agr = FALSE,
                            periode = NULL,
                            variation = FALSE,
                            statut = NULL,
                            categorie = NULL,
                            exclure.codes = NULL,
                            quotite.nulle = FALSE)  {
  
  brut.eqtp(Base, grade, emploi, classe, service, libellés, agr, periode, variation, statut, categorie, exclure.codes, quotite.nulle, type = "E")
}


#' Annee de comparaison avec les données nationales.
#'
#' Calcule l'annee à laquelle la pyramide des âges va être comparée aux données nationales, pour un versant donné de la focntion publique.
#'
#' @param versant Chaîne de caractères parmi "FPT", "FPH", "TIT_FPT", "TIT_FPH", "NONTIT_FPT", "NONTIT_FPH".
#' @return Une liste dont les composantes sont :
#'        \describe{
#'         \item{\code{annee}}{l'annee de comparaison (entier).}
#'         \item{\code{pyr}}{la base de données nationales correspondante (data.table)}.
#'         }
#' @export
 
annee_comparaison <- function(versant) {
  
  p <- NULL
  
  for (a in fin.periode.sous.revue:2010) {
    if (exists(p0 <- "pyr_" %+% a %+% "_" %+% tolower(versant), envir = .GlobalEnv)) {
      annee <- a
      p <- p0
      break
    }
  }
  
  # --- sanity checks
  stopifnot(!is.null(p))
  pyr <- get(p, .GlobalEnv)
  stopifnot(toupper(pyr[1, versant]) == versant)
  stopifnot(pyr[1, annee.reference]  == annee)
  
  # ---
  
  return(list(annee = annee, pyr = pyr))
}



# Age fin décembre de l'Annee en annees révolues
# On trouve quelques valeurs correspondant à des NIr non conventionnels
# 3, 4 : en cours d'immatriculation
# 7, 8 : immatriculation temporaire

#' Traitement du NIR (numero d'inscription au répertoire des personnes physiques).
#'
#' Extrait la répartition par âge et sexe des individus ayant un NIR.
#'
#' @param Base data.table contenant au moins une variable nommée Nir décrivant le NIR.
#' @param annee Annee civile à la fin de laquelle est évalué l'âge de l'individu.
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

extraire.nir <- function(Base, annee)  {

  Base[ , `:=`(age = annee - (as.numeric(substr(Nir, 2, 3)) + 1900),
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

