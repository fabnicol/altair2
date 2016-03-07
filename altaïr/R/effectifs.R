



#' Tableau des effectifs.
#' 
#' Elabore un tableau des effectifs et équivalents temps plein travaillés par type de personnel et par année.
#' 
#' @param période Vecteur des années de la période sous revue
#' @param Bulletins Base des bulletins de paye
#' @param Analyse Base des analyses de rémunérations
#' @param Analyse Base des analyses de variations de rémunérations
#' @return Un tableau des effectifs mis en forme.
#' @examples
#' effectifs(2010:2015)
#' @export
effectifs <- function(période, Bulletins = Bulletins.paie, 
                      Analyse = Analyse.rémunérations,
                      Analyse.vpe = Analyse.variations.par.exercice,
                      Analyse.v = Analyse.variations) {
  
  
  eff <- lapply(période,
                    function(x) {
                      A <- Bulletins[Année == x, 
                                      .(Matricule, Statut, permanent, quotité, nb.mois, Grade)]
                      
                      E <- unique(A[ , .(Matricule, permanent)], by = NULL)
                      ETP <- unique(Bulletins[Année == x, 
                                                   .(quotité, Matricule, Statut, permanent, Mois, nb.mois)],
                                    by = NULL)
                      F <- E[permanent == TRUE, ]
                      G <- unique(A[Statut == "TITULAIRE" | Statut == "STAGIAIRE", .(Matricule, permanent)],
                                  by = NULL)
                      H <- G[permanent == TRUE, ]
                      I <- unique(A[Statut == "ELU", .(Matricule, permanent)],
                                  by = NULL)
                      J <- I[permanent == TRUE, ]
                      K <- unique(A[Statut != "TITULAIRE" & Statut != "STAGIAIRE" & Grade == "V", .(Matricule, permanent)],
                                  by = NULL)
                      L <- unique(A[Grade == "A", .(Matricule, permanent)],
                                  by = NULL)
                      postes.non.actifs <- unique(Analyse[Statut != "ELU"
                                                                        & Filtre_actif == FALSE
                                                                        & Année == x,
                                                                        Matricule])
                      postes.actifs.annexes <- unique(Analyse[Statut != "ELU"
                                                                            & Filtre_annexe == TRUE
                                                                            & Filtre_actif == TRUE
                                                                            & Année == x,
                                                                            Matricule])
                      postes.actifs.non.annexes <- unique(Analyse[Statut != "ELU"
                                                                                & Filtre_annexe == FALSE
                                                                                & Filtre_actif == TRUE
                                                                                & Année == x,
                                                                                Matricule])
                      postes.non.titulaires <- unique(Analyse.vpe[Statut == "NON_TITULAIRE" 
                                                                                      & Année ==x, 
                                                                                      Matricule])
                      
                      c(nrow(E), 
                        nrow(F),
                        nrow(G),
                        nrow(H),
                        length(postes.non.titulaires),
                        nrow(I),
                        nrow(J),
                        nrow(K),
                        nrow(L),
                        length(postes.non.actifs),
                        length(postes.actifs.annexes),
                        length(postes.actifs.non.annexes),
                        ETP[Statut != "ELU" , sum(quotité/nb.mois, na.rm=TRUE)],                            # ETP      
                        ETP[Statut != "ELU" , sum(quotité, na.rm=TRUE)] / 12,                               # ETPT 
                        ETP[Matricule %chin% unique(Analyse.v[est.rmpp == TRUE                     # ETPT_pp              
                                                                       & Année == x,
                                                                       Matricule]),
                            sum(quotité, na.rm=TRUE)] / 12,
                        ETP[(Statut == "TITULAIRE" | Statut == "STAGIAIRE")                                 # ETPT_fonct
                            & Matricule %chin% unique(Analyse.v[Statut == "TITULAIRE"
                                                                         | Statut == "STAGIAIRE",
                                                                         Matricule]),
                            sum(quotité, na.rm=TRUE)] / 12,
                        ETP[Statut == "TITULAIRE"                                                           # Tit_12_100
                            & permanent == TRUE 
                            & Matricule %chin% unique(Analyse.v[permanent == TRUE
                                                                         & statut == "TITULAIRE"
                                                                         & temps.complet == TRUE
                                                                         & Année == x,
                                                                         Matricule]),
                            sum(quotité, na.rm=TRUE)] / 12,                                                
                        
                        
                        ETP[Statut == "NON_TITULAIRE"                                                       # ETPT_nontit
                            & Matricule %chin% postes.non.titulaires,  
                            sum(quotité, na.rm=TRUE)] / 12,
                        ETP[Statut == "AUTRE_STATUT"                                                        # ETPT_autre
                            & Matricule %chin% unique(Analyse.rémunérations[Statut == "AUTRE_STATUT",
                                                                            Matricule]),
                            sum(quotité, na.rm=TRUE)] / 12,
                        ETP[Matricule %chin% postes.non.actifs, sum(quotité, na.rm=TRUE)] / 12,             # ETPT_non_actif 
                        ETP[Matricule %chin% postes.actifs.annexes, sum(quotité, na.rm=TRUE)] / 12,         # ETPT_annexe
                        ETP[Matricule %chin% postes.actifs.non.annexes, sum(quotité, na.rm=TRUE)] / 12)			# ETPT_actif_nonannexe				
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


