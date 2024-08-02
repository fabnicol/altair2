
#'
#'## 5.12 Contrôle du supplément familial de traitement
#'  
#'[![Notice](icones/Notice.png)](Docs/Notices/fiche_SFT.odt)   
## La biblitothèque SFT est à revoir  

SFT_sans_enfant()

#'  
conditionnel("Lien vers la base des paiements de SFT à agents sans enfant signalé", "Bases/Reglementation/Paie.sans.enfant.reduit.csv")
#'  

# Traitement = sum(Montant[Type == "T"], na.rm = TRUE),

liquidation_SFT()

#'  
conditionnel("Lien vers la base des écarts de paiement sur SFT", "Bases/Reglementation/controle.sft.csv")
#'  

message("Analyse du SFT")

# data.table here overallocates memory hence inefficient !
# Bulletins.paie[Nb.Enfants > 0 , SFT.controle := sft(Nb.Enfants, Indice, Heures, Annee, Mois)]
