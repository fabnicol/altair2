#duree.sous.revue <- { temp <- Bulletins.paie[ , Annee, by = Annee] ; temp[1] - temp[.N] +1 }

#' SMPT
#' @param Cat  Catégorie des personnels (A, B, C)
#' @param Liste_statuts Statuts des personnels (ex.: c("STAGIAIRE", "TITULAIRE"))
#' @param type "smpt net", "smpt brut", "rmpp net", "rmpp brut"
#' @return Liste composée du tableau d'évolution du SMPT et table temporaire
#' @export

smpt <- function(Cat = NULL, Liste_statuts = NULL, type =  "smpt net") {

    S_ <- Analyse.variations.par.exercice
    if (! is.null(Cat)) S_ <- S_[Categorie == Cat]
    if (! is.null(Liste_statuts)) S_ <- S_[Statut %in% Liste_statuts]

    S_net.eqtp <- S_[ ,.(moy = weighted.mean(Montant.net.annuel.eqtp,
                                       quotite.moyenne,
                                       na.rm = TRUE)),
                         by = "Annee"]


    S_net.eqtp.100 <- S_[temps.complet.sur.annee == TRUE,
                           .(moy = mean(Montant.net.annuel.eqtp, na.rm = TRUE)),
                           by = "Annee"]

    f <- function(x) prettyNum(S_net.eqtp[Annee == x, moy],
                               big.mark = " ",
                               digits = 1,
                               format = "fg")

    g <- function(x) prettyNum(S_net.eqtp.100[Annee == x, moy],
                               big.mark = " ",
                               digits = 1,
                               format = "fg")

    T <- Tableau.vertical(c("Annee", type %+% " (euros)", type %+% " temps complet (euros)"),
                          if (type == "smpt net") periode else periode[2:get("duree.sous.revue")],           # if...else pas ifelse (dim vecteur)
                          extra = "variation",
                          f,
                          g)

  if (! is.null(T) & T != "") list(T, S_) else NULL
}

#' distribution_smpt
#' @param Cat  Catégorie des personnels (A, B, C)
#' @param Liste_statuts Statuts des personnels (ex.: c("STAGIAIRE", "TITULAIRE"))
#' @param S_ Table temporaire calculée par #smpt
#' @return Quartiles du SMPT
#' @export

distribution_smpt <- function(Cat = NULL, Liste_statuts = NULL, S_ = NULL) {

   if (is.null(S_)) {
    S_ <- Analyse.variations.par.exercice
    if (! is.null(Cat)) S_ <- S_[Categorie == Cat]
    if (! is.null(Liste_statuts)) S_ <- S_[Statut %in% Liste_statuts]
  }

  Resume(c(debut.periode.sous.revue, "Effectif",
           debut.periode.sous.revue %+% " TC", "Effectif",
           fin.periode.sous.revue, "Effectif",
           fin.periode.sous.revue %+% " TC",  "Effectif"),
         list(
             S_[Annee == debut.periode.sous.revue][,
                 .(Montant.net.annuel.eqtp, quotite.moyenne)],
             S_[Annee == debut.periode.sous.revue
                 & temps.complet.sur.annee == TRUE][,
                  .(Montant.net.annuel.eqtp, quotite.moyenne)],
             S_[Annee == fin.periode.sous.revue][,
                 .(Montant.net.annuel.eqtp, quotite.moyenne)],
             S_[Annee == fin.periode.sous.revue
                 & temps.complet.sur.annee == TRUE][,
                 .(Montant.net.annuel.eqtp, quotite.moyenne)]),
         extra = "length",
         id = "distribution_smpt")

  # Pour des raisons très mal comprises, print est ici nécessaire
  # alors qu'il ne l'est pas dans smpt() pour Tableau_vertical ;
  # pourtant les deux fonctions sont basées sur kable()

}

#' RMPP
#' Calcul de la rémunération moyenne des personnes en place
#' @export

calcul_rmpp <- function() {

"Analyse.variations" %a% Analyse.variations[nb.jours > seuil.troncature
                                           & ! is.na(Montant.net.annuel.eqtp)
                                           & Montant.net.annuel.eqtp  > minimum.positif
                                           & ! is.na(Statut)]

    q3 <- quantile(Analyse.variations$variation.rémunération,
                   c(quantile.cut/100, 1 - quantile.cut/100), na.rm=TRUE)

# Filtrage : on enlève les personnels présents depuis moins d'un seuil
# de troncature (ex. 120 jours) dans l'annee et les élus
# (paramètre seuil.troncature)

# Filtrage pour l'étude des variations : on enlève les valeurs manquantes
# des variations, les centiles extrêmaux,
# les rémunérations nettes négatives ou proche de zéro.
# On exige un statut explicite en fin de periode (statut.fin.annee).
# Paramétrable par :
# minimum.positif, quantile.cut

# ici il faut réduire la matrice pour éviter les réduplications pour les Resumes. TODO

"Anavar.synthese" %a% Analyse.variations[total.jours > seuil.troncature
                                      & pris.en.compte == TRUE
                                      & ! is.na(statut.fin.annee)
                                      & ! is.na(variation.rémunération)
                                      & variation.rémunération > q3[[1]]
                                      & variation.rémunération < q3[[2]]]

"Anavar.synthese.plus.2.ans"  %a% Anavar.synthese[! is.na(plus.2.ans) & plus.2.ans == TRUE]
"Anavar.synthese.moins.2.ans" %a% Anavar.synthese[! is.na(plus.2.ans) & plus.2.ans == FALSE]

"masque.présent.sur.periode" %a% bitwShiftL(1, duree.sous.revue - 1)      #  11111..1

sauv.bases("Remunerations", .GlobalEnv, "Anavar.synthese")
}
