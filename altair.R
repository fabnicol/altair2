
## ------------------  Prologue  ------------------------ ##

library(compiler)
enableJIT(3)
options(width=120, warn=-1)
source("bibliothèque.altair.R")
installer.paquets(assertthat, plyr, rigoureusement = TRUE)
library(assertthat)
source("classes.R")

## ------------------------------------------------------ ##

# méthodes auxiliaires de gestion de bases  

base.générateur$methods(
  
  scan.prime = function(regexp)
  {
    "scan.prime:  character  ->  data.frame
    
    Cherche l'expression régulière arg0 dans un Libellé de paiment, sans tenir
    compte de la casse et renvoie les matricules et libellés correspondants, sans
    doublons"
    
    unique(
      Base[grep(paste0(".*(", regexp,").*"), étiquette.libellé, ignore.case=TRUE),
           c("Matricule", étiquette.libellé)])
  },
  
  enlever.colonnes.doublons.sauf.1 =  function(Base1, Base2) 
  {
    "enlever.colonnes.doublons.sauf.1:  data.frame, data.frame  ->  data.frame
    
    Enlève les colonnes de arg0 qui sont dans arg1 sauf pour la clé de fusion
    champ.détection.1"
    
    subset(Base1, 
           select=c(champ.détection.1, setdiff(names(Base1),names(Base2))))
    
  },
  
  enlever.colonnes.doublons.sauf.2 =  function(Base1, Base2) 
  {
    "enlever.colonnes.doublons.sauf.2:  data.frame, data.frame  ->  data.frame
    
    Enlève les colonnes de arg0 qui sont dans arg1 sauf pour la clé de fusion
    champ.détection.1 et pour la clé auxiliaire champ.détection.2. Version tolérante
    de la fonction précédente"
    
    subset(Base1, 
           select=c(champ.détection.1,champ.détection.2,
                    setdiff(names(Base1),names(Base2))))
    
  },
  
  est.code.de.type = function(x) 
  {
    "est.code.de.type:  character  ->  logical
    
    Teste si la valeur du champ Code de Bdp.ldp est de type arg0 dans Code.prime$Type.rémunération"
    
    Base[, étiquette.code] %in% Codes[Codes[, étiquette.type.rémunération] == x, étiquette.code]
  }
)

#  Méthodes de sauvegarde et de lecture de bases

base.générateur$methods(
  
  trouver.valeur.skip =  function(chemin.table) 
  {
    "trouver.valeur.skip: character  ->  numeric(1)
    
    Trouve le numéro de la ligne à laquelle se situe la liste des noms de variables
    en recherchant soit le mot `Matricule' soit une expression du type `Code...'
    Il faudra déduire ce `skip' du read.csv2 pour récupérer proprement les noms de variable
    Pour cela on scanne les 25 premières lignes de la table une première fois"
    
    max(
      sapply(
        read.csv2(chemin.table, nrows=25),
        function(x) 
        {
          m <- match(champ.détection.1, x, nomatch=0 ) 
          if (m == 0)
            m <- pmatch(champ.détection.2, x, nomatch=0, duplicates.ok=FALSE ) 
          return(m)
        }
      ))
  },
  
  read.csv.skip = function(x) 
  {
    
    "read.csv.skip:  character -> data.frame
    
    Lit une base en sautant les lignes avant les champs de détection automatique
    de la première ligne de noms de colonne, par ex. `Matricule'
    spécifiés par champ.détection.1 et champ.détection.2"
    
    chem <- chemin(x)
    read.csv2(chem, skip=trouver.valeur.skip(chem), fileEncoding="UTF-8")
  },
  
  sauv.base = function(x)  
  {
    "sauv.base:  object  ->  IO(file)
    
    Sauvegarde un objet X sous la forme d'une base csv2 sans noms de ligne, en encodage UTF-8
    dans le dossier de travail et sous le nom `X.csv2'" 
    
    if (vérifier.intégrité(x, poursuivre=TRUE) == TRUE)
      write.csv2(x, paste0(chemin(deparse(substitute(x)), ".csv")), row.names=FALSE, fileEncoding = "UTF-8")
  },
  
  sauv.bases = function(...) 
  {
    "sauv.bases:  ...  ->  IO(file),...
    
    itère sauv.base sur chacun des arguments de la fonction"
    
    tmp <- as.list(match.call()) 
    tmp[1] <- NULL
    lapply(tmp, sauv.base)
    return(0)
  },
  
  Read.csv = function(vect.chemin)
  {
    "Read.csv: vector(character)  ->   data.frame
    
    Lit un vecteur de chemins et empile verticalement les bases correspondant à ces chemins
    qui résultent d'une importation csv2 par read.csv.skip"
    
    do.call(rbind, lapply(vect.chemin, read.csv.skip))
  }
)

#  Méthodes de traitement actif du problème et lanceur des tâches de statistiques

base.générateur$methods(
  
  importer.bases = function()
  {
    vérifier.intégrité(
      Lignes      <<-  Read.csv(altair$nom.de.fichier.lignes),
      NBI         <<-  Read.csv(altair$nom.de.fichier.nbi),
      Bulletins   <<-  Read.csv(altair$nom.de.fichier.bulletins),
      Catégories  <<-  Read.csv(altair$nom.de.fichier.catégories),
      Codes       <<-  Read.csv(altair$nom.de.fichier.codes),
      Avantages   <<-  Read.csv(altair$nom.de.fichier.avantages))
    
    #suppression des colonnes Nom Prénom redondantes
    
    Avantages   <<-  selectionner.cle.matricule(Avantages, Categories) 
    Bulletins   <<-  selectionner.cle.matricule.mois(Bulletins, Lignes)
    
    #fusion matricule | avantage | catégorie par Matricule
    
    Base <<- merge(Bulletins, Lignes)
    
    # un peu par acquis de conscience...
    
    vérifier.intégrité(Base)
  },
  
  générer.codes = function()
  {
    Codes.NT     <<- Base[ ! Base$Statut %in% c("TITULAIRE", "STAGIAIRE"), c("Code", "Libellé")]
    Codes.NT     <<- Codes.NT[ ! duplicated(Codes.NT),]
    Codes.fonct  <<- Base[ Bdp.ldp$Statut %in% c("TITULAIRE", "STAGIAIRE"), c("Code", "Libellé")]
    Codes.fonct  <<- Codes.fonct[!duplicated(Codes.fonct),]
    Codes.NBI    <<- NBI[ ,"NBI"]  
    Codes.NBI    <<- Codes.NBI[!duplicated(Codes.NBI),]
    Codes        <<- rbind(Code.NT, Codes.fonct)
    
    sauv.bases(Codes.NT,
               Codes.fonct,
               Codes.NBI,
               Codes)
    
  },
  
  générer.distributions = function()
  {
    source(chemin("analyser.distributions.R"))
    analyser.distributions()
    if (altair$générer.bases == TRUE)  
    {
      sauv.bases(Analyse.rémunérations,
                 Stats.contractuels,
                 Stats.fonctionnaires,
                 Base,
                 masses)
    }
  },
  
  générer.tests = function()
  {
    source(chemin("analyser.tests.R"))
    analyser.tests()
    sauv.bases(
      NBI.aux.non.titulaires,
      HS.sup.25,
      personnels.prime.informatique,
      ldp.fonctionnaires.et.vacations,
      matricules.nontit.et.vacations,
      Bdp.ldp.vacations,
      violation.plancher.indiciaire.ifts,
      RI.et.vacations,
      vacations.concernées,
      traitement.et.vacations,
      ifts.et.non.tit)
  },
  
  générer.variations = function()
  {
    source(chemin("analyser.variations.R"))
    analyser.variations()
    if (générer.bases == TRUE)  
    {
      sauv.bases(Total,
                 Stats.Analyse.rémunérations.personnels.plus.de.2.ans,
                 Stats.Analyse.rémunérations.personnels.moins.de.2.ans,
                 Analyse.rémunérations,
                 Analyse.rémunérations.filtrée)
    }
  }
)

