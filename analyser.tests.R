library(plyr)

analyser.tests <- function ()
{
  attach(Bdp.ldp, warn.conflicts=FALSE)
  
  # Tests du type : incompatibilités entre indemnités et statut
  
  # NBI pour non titulaires
  
  NBI.aux.non.titulaires <- Bdp.ldp[ ! Statut %in% c("TITULAIRE","STAGIAIRE") & as.character(Code) %in% codes.NBI, colonnes.sélectionnées]
  
  nombre.de.ldp.NBI.nontit <- nrow(NBI.aux.non.titulaires)
  
  # heures sup >= 25h/mois
  
  HS.sup.25 <- with(Bdp, Bdp[Heures.Sup. >= 25 , c("Matricule", "Statut", "Mois", "Heures.Sup.", "Brut")])
  
  nombre.de.ldp.HS.sup.25 <- nrow(HS.sup.25)
  
  # Prime de fonctions informatiques : pas dans la base de VLB
  # on cherche la chaine de char. "INFO" dans les libellés de primes
  # variante : filtre <- regexpr(".*(INFO|PFI|P.F.I).*", toupper(Bdp.ldp$Libellé)) et regmatches(Bdp.ldp$Libellé, filtre)
  
  filtre<-grep(".*(INFO|PFI|P.F.I).*", Libellé, ignore.case=TRUE)
  
  personnels.prime.informatique <- Bdp.ldp[ filtre, colonnes.sélectionnées]
  
  primes.informatiques.potentielles<-unique(Libellé[filtre])
  
  nombre.de.ldp.personnels.pfi <- nrow(personnels.prime.informatique)
  
  # Vacations et statut de fonctionnaire
  
  ldp.fonctionnaires.et.vacations <- Bdp.ldp[ Statut %in% c("TITULAIRE", "STAGIAIRE") & est.code.de.type("VACATIONS"), colonnes.sélectionnées]
  
  nombre.de.ldp.fonctionnaires.et.vacations <- nrow(ldp.fonctionnaires.et.vacations)
  
  # Vacations et régime indemnitaire
  
  Bdp.ldp.vacations <- Bdp.ldp[est.code.de.type("VACATIONS"), colonnes.sélectionnées]
  
  matricules.nontit.et.vacations <- unique(Bdp.ldp.vacations[ ! Bdp.ldp.vacations$Statut %in% c("TITULAIRE", "STAGIAIRE"), "Matricule"])
  
  vacations.concernées <- Bdp.ldp.vacations[Matricule %in% matricules.nontit.et.vacations, ]
  
  RI.et.vacations <- Bdp.ldp[ Matricule %in% matricules.nontit.et.vacations & est.code.de.type("INDEMNITAIRE.OU.CONTRACTUEL"), colonnes.sélectionnées]
  
  traitement.et.vacations <- Bdp.ldp[ Matricule %in% matricules.nontit.et.vacations & est.code.de.type("TRAITEMENT"), colonnes.sélectionnées]
  
  nombre.de.ldp.RI.et.vacations <- nrow(RI.et.vacations)
  nombre.de.ldp.traitement.et.vacations <- nrow(traitement.et.vacations)
  
  #IFTS et IB >= 380 (IM >= 350)
  
  filtre.iat<-grep(".*(IAT|I.A.T|.*Adm.*Tech).*", Libellé, ignore.case=TRUE)
  filtre.ifts<-grep(".*(IFTS|I.F.T.S|.*FORF.*TRAV.*SUPP).*", Libellé, ignore.case=TRUE)
  codes.ifts <- unique(Bdp.ldp[filtre.ifts, "Code"])
  
  sélection.matricules <- intersect(!duplicated(Bdp.ldp[ Indice < 350, c("Matricule")]), !duplicated( Bdp.ldp[ filtre.ifts, c("Matricule")]))
  
  Bdp.ldp[Matricule %in% sélection.matricules & Code %in% codes.ifts & (Indice < 350 ), colonnes.sélectionnées]
  
  rm(sélection.matricules)
  
  # IFTS et non tit
  
  ifts.et.non.tit <- Bdp.ldp[Code %in% codes.ifts & !Statut %in% c("TITULAIRE", "STAGIAIRE"), colonnes.sélectionnées]
  
  # Incomptabilités de primes entre elles
  
  #IAT et IFTS
  
  personnels.iat.ifts <- intersect(as.character(Bdp.ldp[ filtre.iat, c("Matricule")]), as.character(Bdp.ldp[ filtre.ifts, c("Matricule")]))
  
  nb.personnels.iat.ifts <- length(personnels.iat.ifts)
  
  # Primes entre elles : 
  
  pretty.print(nombre.de.ldp.NBI.nontit)
  pretty.print(nombre.de.ldp.HS.sup.25)
  pretty.print(primes.informatiques.potentielles)
  pretty.print(nombre.de.ldp.personnels.pfi)
  pretty.print(nombre.de.ldp.fonctionnaires.et.vacations)
  pretty.print(nombre.de.ldp.RI.et.vacations)
  pretty.print(nombre.de.ldp.traitement.et.vacations)
  pretty.print(nb.personnels.iat.ifts)
  
  detach(Bdp.ldp)
}
