library(compiler)
source("bibliothèque.altair.R")
enableJIT(3)

noyau <- setRefClass(
  "Noyau",
  fields=list(arguments = "list"),
  methods=list(
    vérifier.intégrité = function(..., poursuivre=FALSE) 
    {
      "vérifier.intégrité:  ..., poursuivre=FALSE  ->  IO(console|exec)
      poursuivre=FALSE       ->  IO(console|exec)
      
      Vérifie si aucune dimension n'est nulle dans les  objets en argument.
 Avec argument, si au moins un objet a au moins une dimension nulle, alors :
   - si poursuivre == FALSE :  terminer la session
   - sinon continuer en laissant un message d'erreur.
      Sans argument, opère comme avec l'ensemble de ses champs en argument."
        
        arguments <<- as.list(match.call()) 
        arguments[1] <<- NULL
        if  (!missing(poursuivre))  arguments[length(arguments)] <<- NULL

        if (length(arguments) == 0) 
            arguments <<- fields()
        
        if (all(unlist(lapply(lapply(arguments, eval, envir=.GlobalEnv), assertthat::not_empty))) != TRUE)
          {
            message("Un des objets :")
            cat(unlist(lapply(arguments, deparse)))
            message("est de dimension nulle")
            if (poursuivre == FALSE)
              stop("Fin de la session.")
            
            return(invisible(FALSE))
          }
        else
          return(invisible(TRUE))
      }
  )
)



altair.générateur <- setRefClass(
  "Altair",
  contains="Noyau",
  fields=list(
    champ.détection.1         = "character",
    champ.détection.2         = "character",
    code.autre                = "character",
    code.nbi                  = "character",
    code.prime.ou.contractuel = "character",
    code.traitement           = "character",
    code.vacation             = "character",
    colonnes.sélectionnées    = "character",    
    date.format               = "character",
    début.période.sous.revue  = "numeric",
    décoder.xhl               = "logical",
    dossier.bases             = "character",
    dossier.stats             = "character",
    étiquette.code            = "character",
    étiquette.libellé         = "character",
    étiquette.matricule       = "character",
    étiquette.montant         = "character",
    étiquette.totalgénéral    = "character",
    étiquette.type.rémunération = "character",
    fin.période.sous.revue    = "numeric",
    générer.bases             = "logical",
    générer.codes             = "logical",
    générer.distributions     = "logical",
    générer.tests             = "logical",
    nom.de.fichier.avantages  = "character",
    nom.de.fichier.base       = "character",
    nom.de.fichier.bulletins  = "character",
    nom.de.fichier.catégories = "character",
    nom.de.fichier.codes      = "character",
    nom.de.fichier.lignes     = "character",
    nom.de.fichier.nbi        = "character",
    seuil.troncature          = "numeric",
    verbosité                 = "numeric"
    ),
  
  methods=list(
    initialize = function(
      champ1                  = "Matricule",
      champ2                  = "Mois",
      autre                   = "AUTRES",
      nbi                     = "NBI",
      prime                   = "INDEMNITAIRE.OU.CONTRACTUEL",
      traitement              = "TRAITEMENT",
      vacation                = "VACATION",
      colonnes                = c("Matricule",
                                  "Statut",
                                  "Code",
                                  "Mois",
                                  "Libellé",
                                  "Montant"),
      date                    = "%d/%m/%Y",
      début                   =  2008,
      décoder                 = FALSE,
      dossier.bases           = "Altair/bases",
      dossier.stats           = "Altair/stats",
      code                    = "Code",
      libellé                 = "Libellé",
      matricule               = "Matricule",
      montant                 = "Montant",
      totalgénéral            = "Total",
      type                    = "Type.rémunération",
      fin                     =  2013,
      bases                   =  FALSE,
      codage                  =  TRUE,
      distributions           =  TRUE,
      tests                   =  TRUE,
      nom.avantages           = "avantages",
      nom.base                = "base.csv",
      nom.bulletins           = "Bulletins de paye",
      nom.catégories          = "catégories",
      nom.codes               = "codes.csv",
      nom.lignes              = "Lignes de paye", 
      nom.nbi                 = "NBI",
      seuil                   =  100,
      verbosité               =  0
      )
    {
"Assigne les champs paramètres des fonctions de traitement statistique"
      
      champ.détection.1         <<-    champ1
      champ.détection.2         <<-    champ2
      code.autre                <<-    autre
      code.nbi                  <<-    nbi
      code.prime.ou.contractuel <<-    prime
      code.traitement           <<-    traitement
      code.vacation             <<-    vacation
      colonnes.sélectionnées    <<-    colonnes
      date.format               <<-    date
      début.période.sous.revue  <<-    début
      décoder.xhl               <<-    décoder
      dossier.bases             <<-    dossier.bases
      dossier.stats             <<-    dossier.stats
      étiquette.code            <<-    code
      étiquette.libellé         <<-    libellé
      étiquette.matricule       <<-    matricule
      étiquette.montant         <<-    montant
      étiquette.totalgénéral    <<-    totalgénéral 
      étiquette.type.rémunération <<-  type
      fin.période.sous.revue    <<-    fin
      générer.bases             <<-    bases
      générer.codes             <<-    codage
      générer.distributions     <<-    distributions
      générer.tests             <<-    tests
      nom.de.fichier.avantages  <<-    nom.avantages
      nom.de.fichier.base       <<-    nom.base
      nom.de.fichier.bulletins  <<-    nom.bulletins
      nom.de.fichier.catégories <<-    nom.catégories
      nom.de.fichier.codes      <<-    nom.codes
      nom.de.fichier.lignes     <<-    nom.lignes
      nom.de.fichier.nbi        <<-    nom.nbi
      nom.de.fichier.xhl        <<-    nom.xhl
      seuil.troncature          <<-    seuil
      verbosité                 <<-    verbosité
      # rapport de lecture des paramètres d'entrée
      # l'interface externe a été simplifiée par rapport aux noms de chams internes
      # pour éviter trop de verbosité à l'usage tout en gardant des noms de champs précis
      
      if (verbosité > 1) champs()
      
      #on ne garde que les noms de fichier qui correspondent à des fichiers du système

      nom.de.fichier.bulletins <- nom.de.fichier.bulletins[file.exists(chemin(nom.de.fichier.bulletins))]
      nom.de.fichier.lignes    <- nom.de.fichier.lignes[file.exists(chemin(nom.de.fichier.lignes))]
      nom.de.fichier.nbi       <- nom.de.fichier.nbi[file.exists(chemin(nom.de.fichier.nbi))]

      vérifier.intégrité(nom.de.fichier.bulletins,
                   nom.de.fichier.lignes,
                   nom.de.fichier.nbi,
                   nom.de.fichier.codes)
    
    },
    
    champs = function()
    {
      cat(" champ de détection prioritaire   [ champ1 =", champ.détection.1,"]\n",
          "champ de détection secondaire    [ champ2 =", champ.détection.2, "]\n",
          "libellé des autres primes        [ autre =",  code.autre, "]\n",
          "libellé des NBI                  [ nbi =", code.nbi, "]\n",
          "libellé des rémunérations\n indemnitaires ou contractuelles  [ prime =", code.prime.ou.contractuel, "]\n",
          "libellé des  traitements         [ traitement =", code.traitement, "]\n",
          "libellé des vacations            [ vacations =", code.vacation, "]\n",
          "colonnes sélectionnées           [ colonnes =", colonnes.sélectionnées, "]\n",
          "format de date                   [ date =", date.format, "]\n",
          "début de période sous revue      [ début =", début.période.sous.revue, "]\n",
          "décoder les fichiers .*xhl       [ décoder =", décoder.xhl, "]\n",
          "dossier des bases                [ dossier.bases =", dossier.bases, "]\n",
          "dossier des statistiques         [ dossier.stats =", dossier.stats, "]\n",
          "champ des codes                  [ code =", étiquette.code, "]\n",
          "champ des libellés               [ libellé =] ", étiquette.libellé, "]\n",
          "champ des matricules             [ matricule =", étiquette.matricule, "]\n",
          "champ des montants               [ montant =", étiquette.montant, "]\n",
          "champ du total général           [ totalgénéral =", étiquette.totalgénéral, "]\n",
          "champ du type de rémunération    [ type =", étiquette.type.rémunération, "]\n",
          "fin de la période sous revue     [ fin =", fin.période.sous.revue, "]\n",
          "génération des bases .csv        [ bases =", générer.bases, "]\n",
          "génération des codes de paye     [ codage =", générer.codes, "]\n",
          "génération des distributions     [ distributions =", générer.distributions, "]\n",
          "génération des tests statutaires [ tests =", générer.tests, "]\n",
          "nom du fichier des avantages     [ nom.avantages =", nom.de.fichier.avantages, "]\n",
          "nom du fichier de base en sortie [ nom.base =", nom.de.fichier.base, "]\n",
          "nom du fichier de\n bulletins de paye                [ nom.bulletins =", nom.de.fichier.bulletins, "]\n",
          "nom du fichier de catégories     [ nom.catégories =", nom.de.fichier.catégories, "]\n",
          "nom du fichier de codes          [ nom.codes =", nom.de.fichier.codes, "]\n",
          "nom du fichier de lignes de paye [ nom.lignes =", nom.de.fichier.lignes, "]\n",
          "nom du fichier de NBI            [ nom.nbi =", nom.de.fichier.nbi, "]\n",
          "nom du fichier de xhl            [ nom.xhl =", nom.de.fichier.xhl, "]\n",
          "durée minimum de travail\n première et dernière année       [ durée =", seuil.troncature, "]\n")
      
    }
  )
)

base.générateur <- setRefClass(
  
  "Base",
   contains="Noyau",
   fields=list(
     Base                      = "data.frame",
     Bulletins                 = "data.frame",
     Codes                     = "data.frame",
     Codes.fonct               = "data.frame",
     Codes.NBI                 = "data.frame",
     Codes.NT                  = "data.frame",
     Lignes                    = "data.frame", 
     Avantages                 = "data.frame",
     Catégories                = "data.frame",
     NBI                       = "data.frame"),
  
   methods=list(
    initialize = function(
        altair                  = altair.générateur$new(),  # valeurs par défaut des champs de classe Altair
        bulletins               = data.frame(NULL),
        codes                   = data.frame(NULL),
        lignes                  = data.frame(NULL), 
        avantages               = data.frame(NULL), 
        catégories              = data.frame(NULL), 
        nbi                     = data.frame(NULL))
    {
      # si altair prévoit un décodage xml, alors lancer ce décodage dans un fichier temp
      # puis attribuer directement base au résultat
      # sinon, lire lignes, bulletins, nbi et fusionner
      # mode in : NBI, Lignes, Bulletins dans le premier cas ; *.xhl dans le second
      # mode out: Base, NBI, Lignes, Bulletins dans les deux cas
    
      if (altair$décoder.xhl) décoder.xhl(altair$nom.de.fichier.xhl)
      else  
         importer.bases()
      
      if (altair$générer.codes == TRUE)          générer.codes()
      if (altair$générer.tests == TRUE)          générer.tests()
      if (altair$générer.distributions == TRUE)  générer.distributions()
      if (altair$générer.variations == TRUE)     générer.variations()
    }))
    
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

