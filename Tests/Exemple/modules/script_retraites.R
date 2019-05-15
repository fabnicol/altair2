
#'
#'## 5.16 Contrôle des cotisations de retraite     
#'  
#'[![Notice](Notice.png)](Docs/Notices/fiche_retraite.odt)   
#'**Non titulaires**   
#'    


cat("Les non titulaires ne doivent pas cotiser à la CNRACL. ")

# Les benchmark montrent que pour DEUX %chin% successifs, l'on a intérêt à chaîner les grepl plutot qu'à les coordonner
# et allonger les vecteurs de %chin% plutôt qu'à utiliser !
# Ils montrent aussi que l'on n'a pas intérêt contrairement à l'intution à commencer par extraire une matrice
# commune Paie[Type %chin% c("C", "D")] et que les gains liés à setkey sont marginaux.

Cotisations.irreg <- Paie[Type %in% c("C", "D") & Statut %in% c("NON_TITULAIRE", "AUTRE_STATUT", "EMPLOI_AIDE", "", "ELU")
                         ][grepl("C\\.?\\s*N\\.?\\s*R\\.?\\s*A\\.?\\s*C\\.?\\s*L",
                                  Libelle,
                                  ignore.case = TRUE,
                                  perl = TRUE) & Montant > 0,
                            .(Matricule, Annee, Mois, Type, Statut, Libelle, Montant)]

#'  
#'&nbsp;*Tableau 5.16.1 : Cotisations irrégulières à la CNRACL*     
#'   

if (! identical(Cotisations.irreg, logical(0)) & (nlignes <- nrow(Cotisations.irreg)) > 0) {
  
  cat("Des cotisations CNRACL sont versées pour des agents non titulaires :",  nlignes, "ligne" %s% nlignes, "de paye.")
}

#'   
#'       

Tableau(c("Cotisations salarié", "Cotisations employeur"),
        Cotisations.irreg[Type == "D", sum(Montant, na.rm = TRUE)],
        Cotisations.irreg[Type == "C", sum(Montant, na.rm = TRUE)])                   

sauv.bases(file.path(chemin.dossier.bases, "Reglementation"), environment(), "Cotisations.irreg")

#'   
conditionnel("Lien vers la base des cotisations irrégulières", "Bases/Reglementation/Cotisations.irreg.csv")   
#'   

#'    
#'**Titulaires**   
#'    

cat("Les titulaires ne doivent pas cotiser à l'IRCANTEC. ")

Cotisations.irreg.ircantec <- Paie[Statut == "TITULAIRE" & Type %chin% c("C", "D") & grepl("I\\.?\\s*R\\.?\\s*C\\.?\\s*A\\.?\\s*N\\.?\\s*",
                                                                                           Libelle,
                                                                                           ignore.case = TRUE, perl = TRUE) 
                                   & Montant > 0,
                                   .(Matricule, Annee, Mois, Type, Libelle, Montant)]

#'  
#'&nbsp;*Tableau 5.16.2 : Cotisations irrégulières à l'IRCANTEC*     
#'   

if (! identical(Cotisations.irreg.ircantec, logical(0)) & (nlignes <- nrow(Cotisations.irreg.ircantec)) > 0) {
  
  cat("Des cotisations IRCANTEC sont versées pour des agents titulaires :",  nlignes, "ligne" %s% nlignes, "de paye.")
}

#'   
#'       

Tableau(c("Cotisations salarié", "Cotisations employeur"),
        Cotisations.irreg.ircantec[Type == "D", sum(Montant, na.rm = TRUE)],
        Cotisations.irreg.ircantec[Type == "C", sum(Montant, na.rm = TRUE)])    

sauv.bases(file.path(chemin.dossier.bases, "Reglementation"), environment(), "Cotisations.irreg.ircantec")

#'   
conditionnel("Lien vers la base des cotisations irrégulières", "Bases/Reglementation/Cotisations.irreg.ircantec.csv")   
#'   
