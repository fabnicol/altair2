#durée.sous.revue <- { temp <- Bulletins.paie[ , Année, by = Année] ; temp[1] - temp[.N] +1 }

#' SMPT
#' @param Filtre  Filtre fonctionnel booléen décrivant une sélection sur les lignes (caractéristiques des personnels)
#' @param type "smpt net", "smpt brut", "rmpp net", "rmpp brut"
#' @return Tableau d'évolution du SMPT
#' @export

smpt <- function(Filtre, type =  "smpt net") {
  
  S_net.eqtp <- Analyse.variations[Filtre() == TRUE,
                                   .(moy = weighted.mean(Montant.net.annuel.eqtp, quotité.moyenne, na.rm = TRUE)),
                                   by = "Année"]
  
  S_net.eqtp.100 <- Analyse.variations[Filtre() == TRUE & temps.complet == TRUE & permanent == TRUE,
                                       .(moy = weighted.mean(Montant.net.annuel.eqtp, quotité.moyenne, na.rm = TRUE)),
                                       by = "Année"]
  
  
  f <- function(x) prettyNum(S_net.eqtp[Année == x, moy],
                             big.mark = " ",
                             digits = 1,
                             format = "fg")
  
  g <- function(x) prettyNum(S_net.eqtp.100[Année == x, moy],
                             big.mark = " ",
                             digits = 1,
                             format = "fg")
  
  T <- Tableau.vertical(c("Année", type %+% " (euros)", type %+% " temps complet (euros)"),
                        if (type == "smpt net") période else période[2:get("durée.sous.revue")],           # if...else pas ifelse (dim vecteur)
                        extra = "variation",
                        f,
                        g)
  
  if (! is.null(T) & T != "") T else  cat("    \n")  
}

#' distribution_smpt
#' @param Filtre  Filtre fonctionnel booléen décrivant une sélection sur les lignes (caractéristiques des personnels)
#' @return Quartiles du SMPT
#' @export

distribution_smpt <- function(Filtre) {
  
  Résumé(c(début.période.sous.revue, "Effectif",
           début.période.sous.revue %+% " TC", "Effectif",
           fin.période.sous.revue, "Effectif",
           fin.période.sous.revue %+% " TC",  "Effectif"),
         list(
           Analyse.variations[Année == début.période.sous.revue
                              & Filtre() == TRUE,
                              .(Montant.net.annuel.eqtp, quotité.moyenne)],   
           Analyse.variations[Année == début.période.sous.revue
                              & Filtre() == TRUE
                              & permanent == TRUE
                              & temps.complet == TRUE,
                              .(Montant.net.annuel.eqtp, quotité.moyenne)],
           Analyse.variations[Année == fin.période.sous.revue 
                              & Filtre() == TRUE,
                              .(Montant.net.annuel.eqtp, quotité.moyenne)],
           Analyse.variations[Année == fin.période.sous.revue 
                              & Filtre() == TRUE
                              & permanent == TRUE
                              & temps.complet == TRUE,
                              .(Montant.net.annuel.eqtp, quotité.moyenne)]),
         extra = "length")
  
  # Pour des raisons très mal comprises, print est ici nécessaire alors qu'il ne l'est pas dans smpt() pour Tableau_vertical ;
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
  
q3 <- quantile(Analyse.variations$variation.rémunération, c(quantile.cut/100, 1 - quantile.cut/100), na.rm=TRUE)

# Filtrage : on enlève les personnels présents depuis moins d'un seuil de troncature (ex. 120 jours) dans l'année et les élus
# (paramètre seuil.troncature) 

# Filtrage pour l'étude des variations : on enlève les valeurs manquantes des variations, les centiles extrêmaux,
# les rémunérations nettes négatives ou proche de zéro. On exige un statut explicite en fin de période.
# Paramétrable par :
# minimum.positif, quantile.cut 

# ici il faut réduire la matrice pour éviter les réduplications pour les Résumés. TODO

"Anavar.synthese" %a% Analyse.variations[total.jours > seuil.troncature
                                      & pris.en.compte == TRUE
                                      & ! is.na(statut)   
                                      & ! is.na(variation.rémunération) 
                                      & variation.rémunération > q3[[1]]
                                      & variation.rémunération < q3[[2]]]

"Anavar.synthese.plus.2.ans"  %a% Anavar.synthese[! is.na(plus.2.ans) & plus.2.ans == TRUE]
"Anavar.synthese.moins.2.ans" %a% Anavar.synthese[! is.na(plus.2.ans) & plus.2.ans == FALSE]

# Filtre neutre (tous les personnels sont sélectionnés)
"Filtre_neutre" %a% function() TRUE   

# Filtre fonctionnaires (tous les fonctionnaires sont sélectionnés et eux seuls)
"Filtre_fonctionnaire" %a% function() Statut == "TITULAIRE" | Statut == "STAGIAIRE"

# Filtre catégorie A
"Filtre_cat_A" %a%  function()   (Statut == "TITULAIRE"  | Statut == "STAGIAIRE")  & (Catégorie == "A")

# Filtre catégorie B
"Filtre_cat_B" %a% function()   (Statut == "TITULAIRE"  | Statut == "STAGIAIRE")  & (Catégorie == "B") 

# Filtre catégorie C
"Filtre_cat_C" %a% function()   (Statut == "TITULAIRE"  | Statut == "STAGIAIRE")  & (Catégorie == "C") 


"Filtre_rmpp"  %a% function() (est.rmpp == TRUE)
"Filtre_rmpp_fonctionnaire" %a% function () Filtre_fonctionnaire() & (est.rmpp == TRUE)
"masque.présent.sur.période" %a% bitwShiftL(1, durée.sous.revue - 1)      #  11111..1

sauv.bases(file.path(chemin.dossier.bases, "Remunerations"), .GlobalEnv, "Anavar.synthese")
}

