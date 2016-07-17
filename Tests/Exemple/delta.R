
## calcul de la variable delta ##

if (!is.null(liste.exclusions)) {

  message("Filtrage de la base Delta en retirant les codes exclus...") 
  cat("Codes exclus : ", liste.exclusions)  
  cat("  ")
  # On présume que si delta != 0 et si un code exclu se trouve dans la liste des codes de paiement du mois
  # alors ce seul code doit expliquer l'écart delta.
  # Ce n'est pas toujours vrai mais on peut se contenter de cette approximation
  
  Delta <- Paie[abs(delta) > minimum.delta, .SD[! any(Code %chin% liste.exclusions), 
                                                   .(Statut,
                                                     Emploi,
                                                     Catégorie,
                                                     Service,
                                                     Type,
                                                     Code,
                                                     Libellé,
                                                     Montant,
                                                     Brut,
                                                     delta)], by="Matricule,Année,Mois"]
  message("Filtrage terminé.") 
  
} else {
  
   message("Filtrage de la base Delta sans retirer de codes...") 
  
   Delta <- Paie[abs(delta) > minimum.positif, .(Matricule, 
                                                 Année,
                                                 Mois, 
                                                 Statut,
                                                 Emploi,
                                                 Catégorie,
                                                 Service,
                                                 Type,
                                                 Code,
                                                 Libellé,
                                                 Montant,
                                                 Brut,
                                                 delta), by="Matricule,Année,Mois"]
   message("Filtrage terminé.") 
}

#'
