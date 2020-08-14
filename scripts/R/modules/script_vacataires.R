#'  
#'## 5.3 Contrôle des vacations horaires pour les fonctionnaires      

# Vacations et statut de fonctionnaire

#'Les fonctionnaires peuvent effectuer des vacations horaires pour leur propre employeur à condition de bénéficier d'une autorisation
#'de cumul d'activité accessoire et que les activités concernées ne fassent pas partie du service normal. Les cumuls détectés ci-dessous
#'se limitent aux cas de vacations horaires détectées. L'existence des pièces justificatives pourra être recherchée.

# ----- Trouver, pour toutes les lignes de paie non-charges, une indication de proportionnalité horaire explicite
#       et retenir les variables de calcul du montant et du statut pour les mois concernés par l'existence d'au moins
#       une de ces lignes horaires, assimilée à un paiement de vacation.


Paie_vac <- filtrer_Paie("VAC", portee = "Mois")

Paie_vac <<- Paie_vac[Type %chin% c("T", "I", "R", "IR", "S")]

# ----- Produire la liste de ces libellés horaires

if (is.na(codes.vacataires)) {
  libellés.horaires <- unique(Paie_vac[grepl(expression.rég.vacataires, Libelle, ignore.case = TRUE, perl = TRUE), Libelle])
} else {
  libellés.horaires <- unique(Paie_vac[Code %chin% codes.vacataires, Libelle])
}

# ----- Vérifier si des fonctionnaires titulaires ou stagiaires bénéficient de vacations horaires et donner les caractéristiques

Paie_vac_fonct <<- Paie_vac[Statut %chin% c("TITULAIRE", "STAGIAIRE"), 
                           .(Nom, Statut, Code, Libelle, Type, Taux, Nb.Unite, Montant, Total.mensuel = sum(Montant, na.rm = TRUE)),
                           by = .(Matricule, Annee, Mois)]

lignes.fonctionnaires.et.vacations <- Paie_vac_fonct[Libelle %chin% libellés.horaires]

matricules.fonctionnaires.et.vacations <- unique(lignes.fonctionnaires.et.vacations[ , .(Matricule, Nom, Statut)])
nombre.fonctionnaires.et.vacations <- length(matricules.fonctionnaires.et.vacations[[1]])

if (nombre.fonctionnaires.et.vacations > 0) {
  cat("Il y a ",
      FR(nombre.fonctionnaires.et.vacations),
      "fonctionnaire(s) effectuant des vacations pour son propre établissement. Les bulletins concernés sont donnés en lien. " )
}  else  {
  cat("Pas de vacation détectée. ")
}

sauv.bases("Reglementation", 
           environment(),
           "lignes.fonctionnaires.et.vacations",
           "matricules.fonctionnaires.et.vacations",
           "Paie_vac_fonct")

conditionnel("Matricules des fonctionnaires concernés", "Bases/Reglementation/matricules.fonctionnaires.et.vacations.csv")       
conditionnel("Lien vers les vacations payees à des fonctionnaires", "Bases/Reglementation/lignes.fonctionnaires.et.vacations.csv")       
conditionnel("Lien vers les bulletins de paie correspondants", "Bases/Reglementation/Paie_vac_fonct.csv")            

####  5.4 CEV ####  

#'
#'## 5.4 Contrôles sur les contractuels effectuant des vacations horaires   
#'   
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_CEV_droit.odt)     

#+ tests-statutaires-vacations-ri

#'     
#'**Attention**    
#'Les contrôles réalisés sur les payes des vacataires nécessitent, le plus souvent, la saisie des codes de paye relatifs aux vacations dans l'onglet Codes de l'interface graphique, en raison du fréquent mauvais renseignement 
#'de ces codes en base de paye.  &nbsp; [![Notice](icones/Notice.png)](Docs/Notices/fiche_onglet_codes.odt)        
#'  

Paie_vac_contr <<- Paie_vac[Statut %chin% c("NON_TITULAIRE",  "AUTRE_STATUT"), 
                           .(Nom, Prenom, Matricule, Service, Statut, Categorie, Grade, Echelon, Libelle, Type,
                             Heures, Heures.Sup., Nb.Enfants, Code, Base, Taux, Nb.Unite,  Montant)]

matricules.contractuels.et.vacations <- unique(Paie_vac_contr[ , .(Matricule, Nom, Statut)])

nombre.contractuels.et.vacations     <- nrow(matricules.contractuels.et.vacations)

RI.et.vacations  <- data.frame(NULL)

if (nombre.contractuels.et.vacations) 
{
  RI.et.vacations <- Paie_vac_contr[Type == "I"]
}

nombre.Lignes.paie.RI.et.vacations <- nrow(RI.et.vacations)

#'  
#'&nbsp;*Tableau 5.4.1 : Contractuels effectuant des vacations horaires (CEV)*  &nbsp; [![Notice](icones/Notice.png)](Docs/Notices/fiche_CEV_horaires.odt)   
#'    

if (exists("nombre.contractuels.et.vacations")) {
  
  Tableau(c("Nombre de CEV",
            "Nombre de lignes indemnitaires payees"),
          nombre.contractuels.et.vacations,
          nombre.Lignes.paie.RI.et.vacations)
}

sauv.bases("Reglementation", 
           environment(),
           "RI.et.vacations",
           "matricules.contractuels.et.vacations",
           "Paie_vac_contr")

#'  
conditionnel("Lien vers les matricules des vacataires", "Bases/Reglementation/matricules.contractuels.et.vacations.csv")   
conditionnel("Lien vers les lignes indemnitaires à vérifier", "Bases/Reglementation/RI.et.vacations.csv")    
conditionnel("Lien vers les bulletins de paye correspondants", "Bases/Reglementation/Paie_vac_contr.csv")   
#'    

essayer({ Paie_vac_sft_ir <- filtrer_Paie("IR_S", 
                                          portee = "Mois", 
                                          Var = "Type", 
                                          Base = Paie_vac)[! Statut %chin% c("TITULAIRE", "STAGIAIRE"), 
                                                           .(Nom, Prenom, Matricule, Service, Statut, Categorie, Grade, Echelon, Libelle, Type,
                                                             Heures, Heures.Sup., Nb.Enfants, Code, Base, Taux, Nb.Unite,  Montant)]  

SFT_IR.et.vacations <- Paie_vac_sft_ir[Type %chin% c("IR", "S")]

matricules.SFT_IR.et.vacations <- unique(SFT_IR.et.vacations[ , .(Matricule, Nom, Statut)])

nombre.SFT_IR.et.vacations     <- nrow(matricules.SFT_IR.et.vacations)
}, "Les tests sur les CEV et les versements de SFT ou d'IR n'ont pas été réalisés. ")


#'  
#'&nbsp;*Tableau 5.4.2 : CEV percevant le supplément familial de traitement ou l'indemnité de résidence*     &nbsp; [![Notice](icones/Notice.png)](Docs/Notices/fiche_CEV_SFT.odt)   
#'    

if (exists("nombre.SFT_IR.et.vacations")) {
  
  Tableau(c("Nombre d'agents concernés",
            "Nombre de lignes de paye SFT/IR"),
          nombre.SFT_IR.et.vacations,
          nrow(SFT_IR.et.vacations))
}

sauv.bases("Reglementation", 
           environment(),
           "matricules.SFT_IR.et.vacations",
           "SFT_IR.et.vacations",
           "Paie_vac_sft_ir")

#'  
conditionnel("Lien vers les matricules concernés", "Bases/Reglementation/matricules.SFT_IR.et.vacations.csv")     
conditionnel("Lien vers les lignes SFT/IR à vérifier", "Bases/Reglementation/SFT_IR.et.vacations.csv")   
conditionnel("Lien vers les bulletins de paye correspondants", "Bases/Reglementation/Paie_vac_sft_ir.csv")    
#'   
