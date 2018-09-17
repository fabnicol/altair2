library(plyr)

analyser.tests <- function ()
{
  attach(Base, warn.conflicts=FALSE)
  
  # Tests du type : incompatibilités entre indemnités et statut
  
  # NBI pour non titulaires
  
  NBI.aux.non.titulaires <- Global[ ! parse(text=altair$étiquette.statut) %in% c(altair$code.titulaire, altair$code.stagiaire) & as.character(Code) %in% Codes.NBI, altair$colonnes.sélectionnées]
  
  nombre.de.ldp.NBI.nontit <- nrow(NBI.aux.non.titulaires)
  
  # heures sup >= 25h/mois
  
  HS.sup.25 <- with(Bulletins, Bulletins[Heures.Sup. >= 25 , c("Matricule", "Statut", "Mois", "Heures.Sup.", "Brut")])
  
  nombre.de.ldp.HS.sup.25 <- nrow(HS.sup.25)
  
  # Prime de fonctions informatiques : pas dans la base de VLB
  # on cherche la chaine de char. "INFO" dans les libellés de primes
  # variante : filtre <- regexpr(".*(INFO|PFI|P.F.I).*", toupper(Global$Libelle)) et regmatches(Global$Libelle, filtre)
  
  filtre<-grep(".*(INFO|PFI|P.F.I).*", Libelle, ignore.case=TRUE)
  
  personnels.prime.informatique <- Global[ filtre, colonnes.sélectionnées]
  
  primes.informatiques.potentielles<-unique(Libelle[filtre])
  
  nombre.de.ldp.personnels.pfi <- nrow(personnels.prime.informatique)
  
  # Vacations et statut de fonctionnaire
  
  ldp.fonctionnaires.et.vacations <- Global[ parse(text=altair$étiquette.statut) %in% c("TITULAIRE", "STAGIAIRE") & est.code.de.type(code.vacation), colonnes.sélectionnées]
  
  nombre.de.ldp.fonctionnaires.et.vacations <- nrow(ldp.fonctionnaires.et.vacations)
  
  # Vacations et régime indemnitaire
  
  Global.vacations <- Global[est.code.de.type(code.vacation), colonnes.sélectionnées]
  
  matricules.nontit.et.vacations <- unique(Base.vacations[ ! Base.vacations$Statut %in% c("TITULAIRE", "STAGIAIRE"), "Matricule"])
  
  vacations.concernées <- Base.vacations[Matricule %in% matricules.nontit.et.vacations, ]
  
  RI.et.vacations <- Global[ Matricule %in% matricules.nontit.et.vacations & est.code.de.type(code.prime.ou.contractuel), colonnes.sélectionnées]
  
  # Vacations et indiciaire
  
  traitement.et.vacations <- Global[ Matricule %in% matricules.nontit.et.vacations & est.code.de.type(code.traitement), colonnes.sélectionnées]
  
  nombre.de.ldp.RI.et.vacations <- nrow(RI.et.vacations)
  nombre.de.ldp.traitement.et.vacations <- nrow(traitement.et.vacations)
  
  #IFTS et IB >= 380 (IM >= 350)
  
  filtre.iat<-grep(".*(IAT|I.A.T|.*Adm.*Tech).*", Libelle, ignore.case=TRUE)
  filtre.ifts<-grep(".*(IFTS|I.F.T.S|.*FORF.*TRAV.*SUPP).*", Libelle, ignore.case=TRUE)
  codes.ifts <- unique(Global[filtre.ifts, "Code"])
  
  sélection.matricules <- intersect(!duplicated(Global[ Indice < 350, c("Matricule")]), !duplicated(Global[ filtre.ifts, c("Matricule")]))
  
  violation.plancher.indiciaire.ifts <- Global[Matricule %in% sélection.matricules & Code %in% codes.ifts & (Indice < 350 ), colonnes.sélectionnées]
  
  rm(sélection.matricules)
  
  # IFTS et non tit
  
  ifts.et.non.tit <- Global[Code %in% codes.ifts & !parse(text=altair$étiquette.statut) %in% c("TITULAIRE", "STAGIAIRE"), colonnes.sélectionnées]
  
  # Incomptabilités de primes entre elles
  
  #IAT et IFTS
  
  personnels.iat.ifts <- intersect(as.character(Global[ filtre.iat, c("Matricule")]), as.character(Global[ filtre.ifts, c("Matricule")]))
  
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
  
  detach(Global)
}
