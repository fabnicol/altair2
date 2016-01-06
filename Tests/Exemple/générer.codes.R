générer.base.codes <- function(B) {
  message("Génération de la base des codes de paie et des libellés.")
  
  codes.paiement.généré <<- unique(B[ , c(étiquette.code, étiquette.libellé, "Type"), with = FALSE])
  
  codes.paiement.généré <<- cbind(codes.paiement.généré[mixedorder(codes.paiement.généré$Code), ], character(nrow(codes.paiement.généré)))
  
  names(codes.paiement.généré)[3] <- étiquette.Type.rémunération

  class(codes.paiement.généré) <- "data.frame"
  codes.paiement.généré$Libellé <- iconv(codes.paiement.généré$Libellé, from = "UTF-8", to=encodage.entrée, mark = FALSE)
  nom.fichier.codes <- racine %+% "codes.paiement.généré"
  Sauv.base(chemin.dossier.données, "codes.paiement.généré", nom.fichier.codes)
  
  #'---
  #'
  #'# Tableau des codes de paiement
  #'
  #'##  Renseigner le type de rémunération
  #'
  #'Utiliser les codes : TRAITEMENT, INDEMNITAIRE, ELU, VACATIONS, AUTRES
  #'
  
  kable(codes.paiement.généré, row.names = FALSE)
  
  #'
  
  
  if (file.exists(file.path(chemin.dossier.données, nom.fichier.codes %+% ".csv")))
    message("Génération des codes : voir fichier Données/" %+% nom.fichier.codes %+% ".csv")  else
      message("Les codes n'ont pas été générés.")
  
  
  message("Le programme est arrêté après génération de la base de codes et libellés.
  Relancer Altair après avoir renseigné la troisième colonne
  et placé le fichier sous le répertoire Données avec le nom " %+% nom.fichier.codes.paiement)
  
  if (générer.codes) stop("Codes générés", call. = FALSE, domain = NULL)
  
}
