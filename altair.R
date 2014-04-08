
## ------------------  Prologue  ------------------------ ##

library(compiler)
enableJIT(3)
options(width=120, warn=-1)
source("bibliothèque.altair.R")
installer.paquets(assertthat, plyr, rigoureusement = TRUE)
library(assertthat)
source("classes.R")
source("analyser.distributions.R")

## ------------------------------------------------------ ##

# méthodes auxiliaires de gestion de bases  


base.générateur$methods(

  #' Base::scan.prime
  #'
  #' Cherche l'expression régulière arg0 dans un Libellé de paiment, sans tenir compte de la casse.
  #' voir  \code{\link{base.générateur}}
  #' @param regexp          Expression régulière
  #' @return Renvoie un data.frame contenant les matricules et libellés correspondants, sans doublons
  #' @author Fabrice Nicol
  #' @examples
  #' # Génère les bases .csv pour les années 2009 à 2013 
  #' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
  #' base   <- base.générateur$new(altair)
  #' base$scan.prime(".*PRIME.*")
  #' @export
     
  scan.prime = function(regexp= regexp)
  {
    "scan.prime:  character  ->  data.frame
    
    Cherche l'expression régulière arg0 dans un Libellé de paiment, sans tenir
    compte de la casse et renvoie les matricules et libellés correspondants, sans
    doublons"
    
    unique(
      Global[grep(paste0(".*(", regexp,").*"), étiquette.libellé, ignore.case=TRUE),
           c("Matricule", étiquette.libellé)])
  },

  #' Base::enlever.colonnes.doublons.sauf.1
  #'
  #' Cherche l'expression régulière arg0 dans un Libellé de paiment, sans tenir compte de la casse.
  #' voir  \code{\link{base.générateur}}
  #' @param dans  Basedont sont enlevées des colonnes qui se trouvent également dans la base de référence, sauf pour le premier champ de détection.
  #' @param de    Base de référence. 
  #' @return Renvoie un data.frame contenant la base dans nettoyée des colonnes excédentaires.
  #' @author Fabrice Nicol
  #' @examples
  #' # Génère les bases .csv pour les années 2009 à 2013 
  #' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
  #' base   <- base.générateur$new(altair)
  #' base$enlever.colonnes.doublons.sauf.1(dans = base$Global, de = base$Avantages)
  #' @export
  
  enlever.colonnes.doublons.sauf.1 =  function(dans=Base1, de=Base2) 
  {
    "enlever.colonnes.doublons.sauf.1:  data.frame, data.frame  ->  data.frame
    
    Enlève les colonnes de arg0 qui sont dans arg1 sauf pour la clé de fusion
    champ.détection.1"
    
    subset(Base1, 
           select=c(altair$champ.détection.1, setdiff(names(Base1),names(Base2))))
    
  },
  
  #' Base::enlever.colonnes.doublons.sauf.2
  #'
  #' Cherche l'expression régulière arg0 dans un Libellé de paiment, sans tenir compte de la casse.
  #' voir  \code{\link{base.générateur}}
  #' @param dans  Basedont sont enlevées des colonnes qui se trouvent également dans la base de référence, sauf pour le premier et dans le second champ de détection.
  #' @param de    Base de référence. 
  #' @return Renvoie un data.frame contenant la base dans nettoyée des colonnes excédentaires.
  #' @author Fabrice Nicol
  #' @examples
  #' # Génère les bases .csv pour les années 2009 à 2013 
  #' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
  #' base   <- base.générateur$new(altair)
  #' base$enlever.colonnes.doublons.sauf.2(dans = base$Global, de = base$Avantages)
  #' @export
  
  
  enlever.colonnes.doublons.sauf.2 =  function(Base1, Base2) 
  {
    "enlever.colonnes.doublons.sauf.2:  data.frame, data.frame  ->  data.frame
    
    Enlève les colonnes de arg0 qui sont dans arg1 sauf pour la clé de fusion
    champ.détection.1 et pour la clé auxiliaire champ.détection.2. Version tolérante
    de la fonction précédente"
    
    subset(Base1, 
           select=c(altair$champ.détection.1,altair$champ.détection.2,
                    setdiff(names(Base1),names(Base2))))
    
  },
  
  #' Base::est.code.de.type
  #'
  #' Teste si le code de rémunération est un code de type indemnité, traitement, etc.
  #' @param type  Type de la rémunération ("TRAITEMENT", "VACATION",...)
  #' @return Renvoie un booléen.
  #' @author Fabrice Nicol
  #' @examples
  #' # Génère les bases .csv pour les années 2009 à 2013 
  #' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
  #' base   <- base.générateur$new(altair)
  #' base$enlever.colonnes.doublons.sauf.1(dans = base$Global, de = base$Avantages)
  #' @export
  
  
  est.code.de.type = function(type = x) 
  {
    "est.code.de.type:  character  ->  logical
    
    Teste si la valeur du champ Code de Bdp.ldp est de type arg0 dans Code.prime$Type.rémunération"
    
    Global[, étiquette.code] %in% Codes[Codes[, étiquette.type.rémunération] == x, étiquette.code]
  }
)

#  Méthodes de sauvegarde et de lecture de bases

base.générateur$methods(
  
 
  
  #' Base::sauvegarder1
  #'
  #' Sauvegarde un objet X sous la forme d'une base csv2 sans noms de ligne, en encodage UTF-8
  #' dans le dossier de travail et sous le nom `X.csv2'
  #' 
  #' @param base  expression
  #' @author Fabrice Nicol
  #' @examples
  #' # Génère les bases .csv pour les années 2009 à 2013 
  #' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
  #' base   <- base.générateur$new(altair)
  #' base$read.csv.skip(altair$chemin.lignes())
  
  sauvegarder1 = function(x)  
  {
    "sauv.base:  object  ->  IO(file)
    
    Sauvegarde un objet X sous la forme d'une base csv2 sans noms de ligne, en encodage UTF-8
    dans le dossier de travail et sous le nom `X.csv2'" 
    
    if (vérifier.intégrité(x, poursuivre=TRUE) == TRUE)
       write.csv2(x, paste0(chemin(deparse(substitute(x)), ".csv")), row.names=FALSE, fileEncoding = "UTF-8")
  },
  
  #' Base::sauvegarder
  #'
  #' Sauvegarde plusieurs arguments X, Y, ... sous la forme de bases csv2
  #' sans noms de ligne, en encodage UTF-8, dans le dossier de travail
  #' et sous le nom `X.csv2', `Y.csv2', ... 
  #' voir  \code{\link{Base::sauv.base}}
  #' @param base  expression
  #' @note  Itère sauvegarder1 sur l'ensemble des arguments.
  #' @author Fabrice Nicol
  #' @examples
  #' # Génère les bases .csv pour les années 2009 à 2013 
  #' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
  #' base   <- base.générateur$new(altair)
  #' base$sauvegarder(Global, Lignes)
  #' @export
    
  sauvegarder = function(...) 
  {
    "sauv.bases:  ...  ->  IO(file),...
    
    itère sauv.base sur chacun des arguments de la fonction"
    
    tmp <- as.list(match.call()) 
    tmp[1] <- NULL
    lapply(tmp, sauv.base)
    return(0)
  }
)

#  Méthodes de traitement actif du problème et lanceur des tâches de statistiques

base.générateur$methods(
  
  #' Base::codes
  #'
  #' Génère l'ensemble des codes de rémunération correspondant à la base globale,
  #' à savoir :
  #' Codes.NT      Codes pour les non titulaires
  #' Codes.fonct   Codes pour les  fonctionnaires
  #' Codes.NBI     Codes NBI
  #' Codes         Ensemble des codes
  #' @author Fabrice Nicol
  #' @examples
  #' # Génère les bases .csv pour les années 2009 à 2013 
  #' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
  #' base   <- base.générateur$new(altair)
  #' base$codes()
  #' @export
    
  codes = function()
  {
    
    "Génère l'ensemble des codes de rémunération correspondant à la base globale"
    
    Codes.NT     <<- Global[ ! Global$Statut %in% c("TITULAIRE", "STAGIAIRE"), c("Code", "Libellé")]
    Codes.NT     <<- Codes.NT[ ! duplicated(Codes.NT),]
    Codes.fonct  <<- Global[ Global$Statut %in% c("TITULAIRE", "STAGIAIRE"), c("Code", "Libellé")]
    Codes.fonct  <<- Codes.fonct[!duplicated(Codes.fonct),]
    Codes.NBI    <<- NBI[ ,"NBI"]  
    Codes.NBI    <<- Codes.NBI[!duplicated(Codes.NBI),]
    Codes        <<- rbind(Code.NT, Codes.fonct)
    
    sauvegarder(Codes.NT,
               Codes.fonct,
               Codes.NBI,
               Codes)
    
  },
  
  #' Base::distributions
  #'
  #' Lance l'analyse des distributions de rémunérations.
  #' Effectue les sauvegardes des bases de résultats, si la valeur
  #' du champ générer.bases du générateur altair a la valeur TRUE.
  #' à savoir :
  #' 
  #' Analyse.rémunérations   Analyse globale des distributions de rémunérations
  #' Stats.contractuels      Analyse des distributions pour les contractuels  
  #' Stats.fonctionnaires    Analyse des distributions pour les fonctionnaires  
  #' Masses                  Masses de rémunérations brutes  
  
  #' @author Fabrice Nicol
  #' @examples
  #' # Génère les bases .csv pour les années 2009 à 2013 
  #' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
  #' base   <- base.générateur$new(altair)
  #' base$distributions()
  #' @export
  
  générer.distributions = function()
  {
    source(chemin("analyser.distributions.R"))
    analyser.distributions()
    if (altair$générer.bases == TRUE)  
    {
      sauv.bases(Analyse.rémunérations,
                 Stats.contractuels,
                 Stats.fonctionnaires,
                 Masses)
    }
  },
  
  #' Base::tests
  #'
  #' Lance les tests de cohérence et de régularité.
  #' Si la valeur du champ générer.bases du générateur altair a la valeur TRUE,
  #' effectue les sauvegardes des bases de résultats, à savoir :
  #' 
  #' NBI.aux.non.titulaires             Base des NBI accordées aux non titulaires
  #' HS.sup.25                          Base des heures supplémentaires supérieures à 25 h/mois
  #' personnels.prime.informatique      Base des personnels attributaires d'une prime informatique
  #' ldp.fonctionnaires.et.vacations    Base des fonctionnaires effectuant des vacations
  #' matricules.nontit.et.vacations     Matricules des non titulaires qui effectuent des vacations
  #' vacations                          Base des lignes de paie de vacations
  #' violation.plancher.indiciaire.ifts Base des lignes de paie qui enfreignent le plancher indiciaire des IFTS
  #' RI.et.vacations                    Base des agents qui perçoivent un régime indemnitaire et effectuent des vacations
  #' vacations.concernées               Base des vacations des agents qui ont un régime indemnitaire,
  #' traitement.et.vacations            Base des agents qui perçoivent un traitement indiciaire et effectuent des vacations
  #' ifts.et.non.tit                    Base des non titulaires qui perçoivent des IFTS.
  #' 
  #' @author Fabrice Nicol
  #' @examples
  #' # Génère les bases .csv pour les années 2009 à 2013 
  #' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
  #' base   <- base.générateur$new(altair)
  #' base$tests()
  #' @export
  
  tests = function()
  {
    source(chemin("analyser.tests.R"))
    analyser.tests()
    sauv.bases(
      NBI.aux.non.titulaires,
      HS.sup.25,
      personnels.prime.informatique,
      ldp.fonctionnaires.et.vacations,
      matricules.nontit.et.vacations,
      vacations,
      violation.plancher.indiciaire.ifts,
      RI.et.vacations,
      vacations.concernées,
      traitement.et.vacations,
      ifts.et.non.tit)
  },
  
  #' Base::variations
  #'
  #' Lance l'analyse des variations de rémunérations.
  #' si la valeur du champ générer.bases du générateur altair est TRUE, 
  #' effectue les sauvegardes des bases de résultats, à savoir :
  #' 
  #' Total                                                 
  #' Stats.Analyse.rémunérations.personnels.plus.de.2.ans  
  #' Stats.Analyse.rémunérations.personnels.moins.de.2.ans  
  #' Analyse.rémunérations                                  
  #' Analyse.rémunérations.filtrée                           
  #' 
  #' @author Fabrice Nicol
  #' @examples
  #' # Génère les bases .csv pour les années 2009 à 2013 
  #' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
  #' base   <- base.générateur$new(altair)
  #' base$variations()
  #' @export
   
  générer.variations = function()
  {
    source(chemin("analyser.variations.R"))
    analyser.variations()
    if (altair$générer.bases == TRUE)  
    {
      sauv.bases(Total,
                 Stats.Analyse.rémunérations.personnels.plus.de.2.ans,
                 Stats.Analyse.rémunérations.personnels.moins.de.2.ans,
                 Analyse.rémunérations,
                 Analyse.rémunérations.filtrée)
    }
  }
)

