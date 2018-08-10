
#'
#'## `r chapitre`.14 Contrôle du supplément familial de traitement 
#'  
#'[![Notice](Notice.png)](Docs/Notices/fiche_SFT.odt)   
## La biblitothèque SFT est à revoir  

if (! utiliser.cplusplus.sft)
{
  source("sft.R", encoding = encodage.code.source)
  sft <- sft_R
  
} else {
  
  library(sft, warn.conflicts = FALSE)
  sft <- function(Nb.Enfants,
                  Indice,
                  Echelon,
                  NBI,
                  Temps.de.travail,
                  Année,
                  Mois) .Call("sft_C",
                              Nb.Enfants,
                              Indice,
                              Echelon,
                              NBI,
                              Temps.de.travail,
                              Année,
                              Mois,
                              PACKAGE="sft")
}

essayer(label = "+Sans enfant",
        
        { Paie.sans.enfant.reduit <- Paie[Type == "S" 
                                          & (is.na(Nb.Enfants) | Nb.Enfants == 0),
                                          .(SFT.versé = sum(Montant, na.rm = TRUE)),
                                          keyby = .(Matricule, Année, Mois)] 
        
        Paie.sans.enfant.reduit <- Paie.sans.enfant.reduit[SFT.versé > 0]
        
        nb.écart.paiements.sft.sans.enfant <- nrow(Paie.sans.enfant.reduit)
        
        if (nb.écart.paiements.sft.sans.enfant > 0){
          
          cat("\nPour les agents n'ayant pas d'enfant signalé en base, il a été détecté ",
              nb.écart.paiements.sft.sans.enfant,
              " bulletin", ifelse(nb.écart.paiements.sft.sans.enfant == 1, "", "s"),
              " de paie présentant un paiement du SFT apparemment anormal.\n", sep="")  
          
          if (afficher.table.écarts.sft)
            kable(Paie.sans.enfant.reduit, row.names = FALSE, align = 'c')
          
        } else {
          
          cat("\nPour les agents n'ayant pas d'enfant signalé en base, il n'a été détecté aucun paiement de SFT.\n")
          
        }
        }, "Le contrôle du SFT pour les agents sans enfant n'a pas pu être réalisé.")

#'  
#'[Lien vers la base des paiements de SFT à agents sans enfant signalé](Bases/Reglementation/Paie.sans.enfant.reduit.csv)
#'  

# Traitement = sum(Montant[Type == "T"], na.rm = TRUE),

essayer(label = "+sft",
        
        {  Paie.enfants.réduit <- Paie[Nb.Enfants > 0 & ! is.na(Nb.Enfants) & ! is.na(Indice) & ! is.na(Heures),
                                       .(SFT.versé = sum(Montant[Type == "S"], na.rm = TRUE), 
                                         Temps.de.travail = Temps.de.travail[1],
                                         Indice = Indice[1],
                                         Echelon = Echelon[1],
                                         NBI = NBI[1],
                                         Nb.Enfants = Nb.Enfants[1]),
                                       keyby = .(Matricule, Année, Mois)]
        
        SFT.controle <- with(Paie.enfants.réduit, 
                             mapply(sft, Nb.Enfants, Indice, Echelon, NBI, Temps.de.travail, Année, Mois, USE.NAMES = FALSE))
        
        Paie.enfants.réduit <- cbind(Paie.enfants.réduit, SFT.controle)
        
        Paie.enfants.réduit[ , delta.SFT := SFT.versé - SFT.controle]
        
        
        ## Attention ne pas intégrer au sein d'un même `:=`(...) deux définitions en coréférence avec if ... else
        # ou alors utiliser ifelse()  [bug de data.table]
        
        # Paie.enfants.réduit[ , ecart := if (SFT.controle > 1) delta / SFT.controle else NA]
        
        # On accepte un tolérance fixée dans prologue.R à tolérance.sft <- 1 euro
        
        controle.sft <- Paie.enfants.réduit[delta.SFT > tolérance.sft,
                                            .(delta.SFT = round(delta.SFT, 2),
                                              SFT.versé,
                                              SFT.controle = round(SFT.controle, 2),
                                              Matricule,
                                              Année,
                                              Mois,
                                              Indice,
                                              NBI,
                                              Temps.de.travail,
                                              Nb.Enfants)]
        
        nb.écart.paiements.sft <- nrow(controle.sft)
        
        if (nb.écart.paiements.sft) setorder(controle.sft, -delta.SFT, Matricule, Année, Mois)
        
        if (nb.écart.paiements.sft > 0){
          
          cat("\nPour les agents ayant au moins un enfant, il a été détecté ",
              nb.écart.paiements.sft,
              " bulletin", ifelse(nb.écart.paiements.sft == 1, "", "s"),
              " de paie présentant un écart de paiement du SFT supérieur à ", tolérance.sft, " euro.\n", sep="")
          
          if (afficher.table.écarts.sft)
            kable(controle.sft, row.names = FALSE, align = 'c')
          
        } else {
          
          cat("\nPour les agents ayant au moins un enfant, il n'a été détecté aucun écart de paiement sur SFT supérieur à ", tolérance.sft, " euro.\n")
          
        }
        }, 
        "Le contrôle de liquidation du SFT pour les agents ayant au moins un enfant n'a pas pu être réalisé.")

#'  
#'[Lien vers la base des écarts de paiement sur SFT](Bases/Reglementation/controle.sft.csv)
#'  

message("Analyse du SFT")

# data.table here overallocates memory hence inefficient !
# Bulletins.paie[Nb.Enfants > 0 , SFT.controle := sft(Nb.Enfants, Indice, Heures, Année, Mois)]
