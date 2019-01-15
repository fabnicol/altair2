
#' Constrôle des astreintes pour les emplois de responsabilité supérieure
#' 
#' Vérification du non-paiement des astreintes à des emplois de directeur général.
#' 
#' \itemize{
#' \item{Filtre la paye en retenant les astreintes.}
#' \item{Retient les cumuls avec les NBI non nulles pour la catégorie A et un emploi ou un grade de directeur général} 
#' \item{Affiche le nombre d'agents détectés dans ces cas} 
#' \item{Crée la table \code{Controle_astreintes} ayant pour colonnes : \code{"Code.astreinte", "Libelle.astreinte", "Montant.astreinte", "Code.NBI", "Libelle.NBI", "Montant.NBI"}} 
#' \item{Crée la table \code{Cum_astreintes} ayant pour colonne : \code{Montant annuel astreintes}, et son total pour la période.}
#' \item{Sauve les bases \code{Controle_astreintes} et \code{libelles.astreintes} dans le répertoire \emph{Reglementation}}
#' \item{Affiche si besoin la table \code{Cum_astreintes} au format \code{\link{Tableau.vertical2}}}
#' } 
#' 
#' @author Fabrice Nicol   
#' @note Les emplois sont filtrés à l'aide de l'expression rationnelle Perl suivante, sans prise en compte de la casse : \cr
#' \code{d(?:\\.|ir)\\w*\\s*\\bg(?:\\.|\\w*n\\.?\\w*)\\s*\\b(?:des?)\\s*\\bs\\w.*}
#' @export

calcul_astreintes <- function() {

essayer({  
  
  "Paie_astreintes" %a% filtrer_Paie("ASTREINTES", portée = "Mois", indic = TRUE)
  
  libelles.astreintes <- unique(Paie_astreintes[indic == TRUE, .(Code, Libelle)], by = NULL)
  
  "Controle_astreintes" %a% Paie_astreintes[! is.na(NBI) 
                                           & NBI > 0
                                           & indic == TRUE,
                                           .(Matricule, Annee, Mois, Categorie, Emploi, Grade, NBI, Code, Libelle, quotite, Montant)
                                        ][Paie_NBI[ , .(Matricule, Annee, Mois, Code, Libelle, Montant)],  
                                           nomatch = 0,
                                           on = .(Matricule, Annee, Mois)]  
  
  "Controle_astreintes" %a% Controle_astreintes[Categorie == "A" 
                                                   & grepl("d(?:\\.|ir)\\w*\\s*\\bg(?:\\.|\\w*n\\.?\\w*)\\s*\\b(?:des?)\\s*\\bs\\w.*", 
                                                           paste(Emploi, Grade), 
                                                           perl = TRUE,
                                                           ignore.case = TRUE)]
  
  setnames(Controle_astreintes, c("Code", "Libelle", "Montant"), c("Code.astreinte", "Libelle.astreinte", "Montant.astreinte"))
  setnames(Controle_astreintes, c("i.Code", "i.Libelle", "i.Montant"), c("Code.NBI", "Libelle.NBI", "Montant.NBI"))
  
  nb.agents.NBI.astreintes <- uniqueN(Controle_astreintes$Matricule)
  
  if (nrow(Controle_astreintes)) {
    cat("Des astreintes sont payées à", nb.agents.NBI.astreintes, "personnels bénéficiaires de NBI")
  }
  
  "Cum_astreintes" %a% rbind(Controle_astreintes[, .(`Montant annuel astreintes` = round(sum(Montant.astreinte), 1)),
                                                     by = "Annee"],
                            list("Total", Controle_astreintes[, round(sum(Montant.astreinte), 1)]))
  
  sauv.bases(file.path(chemin.dossier.bases, "Reglementation"),
             environment(),
             "Controle_astreintes",
             "libelles.astreintes")
  
},
"Le contrôle Astreintes-NBI n'a pas pu être réalisé.")

essayer({
  with(Cum_astreintes,
                
                print(Tableau.vertical2(c("Annee", "Montant astreintes irrégulières (euros)"),
                                            Annee, `Montant annuel astreintes`))    
)
}, "Le tableau de contrôle des astreintes n'a pas pu être généré.")
}

#' Contrôle du cumul des astreintes et des IHTS
#' 
#' Détection du paiement le même mois d'indemnités d'astreintes et d'IHTS
#' 
#' @author Fabrice Nicol   
#' @export

cumul_astreintes_IHTS <- function() {
  
  essayer({  
  setnames(Paie_astreintes, "indic", "indic_astr")
  setnames(Base.IHTS, "indic", "indic_IHTS")
  
  Controle_astreintes_HS_irreg <- Paie_astreintes[ , .(Matricule, Annee, Mois, Code, Libelle, Type,  Montant, indic_astr) 
                                                        ][Base.IHTS[Type %in% c("I", "A", "R"), 
                                                               .(Matricule, Annee, Mois, Code, Libelle, Type, Montant, indic_IHTS)], 
                                                               nomatch = 0,
                                                              on = .(Matricule, Annee, Mois, Code, Libelle, Type, Montant)
                                                        ][indic_IHTS == TRUE | indic_astr == TRUE]
  
  nb.agents.IHTS.astreintes <- uniqueN(Controle_astreintes_HS_irreg$Matricule)
  
  if (nrow(Controle_astreintes_HS_irreg)) {
    cat("Des astreintes sont payées à", nb.agents.IHTS.astreintes, "personnels bénéficiaires d'IHTS.")
  }
  
  "Cum_astreintes_HS_irreg" %a% rbind(Controle_astreintes_HS_irreg[, .(round(sum(Montant[indic_astr == TRUE]), 1),
                                                                    round(sum(Montant[indic_IHTS == TRUE]), 1)),
                                                                keyby = "Annee"],
                                   list("Total",
                                        Controle_astreintes_HS_irreg[indic_astr == TRUE, round(sum(Montant), 1)],
                                        Controle_astreintes_HS_irreg[indic_IHTS == TRUE, round(sum(Montant), 1)]))
  
  sauv.bases(file.path(chemin.dossier.bases, "Reglementation"),
             environment(),
             "Controle_astreintes_HS_irreg",
             "Cum_astreintes_HS_irreg")
  }, 
  "Le contrôle du cumul astreintes IHTS n'a pas pu être réalisé")
}

