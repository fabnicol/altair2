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
    code.stagiaire            = "character",
    code.titulaire            = "character",
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
      stagiaire               = "STAGIAIRE",
      titulaire               = "TITULAIRE",
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
      statut                  = "Statut",
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
      étiquette.statut          <<-    statut
      étiquette.totalgénéral    <<-    totalgénéral 
      étiquette.type.rémunération <<-  type
      fin.période.sous.revue    <<-    fin
      générer.bases             <<-    bases
      générer.codes             <<-    codage
      générer.distributions     <<-    distributions
      générer.tests             <<-    tests
      nom.de.fichier.avantages  <<-    nom.avantages
      nom.de.fichier.base       <<-    nom.base
      nom.de.fichier.bulletins  <<-    nom.bulletins[file.exists(chemin(nom.bulletins))]
      nom.de.fichier.catégories <<-    nom.catégories
      nom.de.fichier.codes      <<-    nom.codes
      nom.de.fichier.lignes     <<-    nom.lignes[file.exists(chemin(nom.lignes))]
      nom.de.fichier.nbi        <<-    nom.nbi[file.exists(chemin(nom.nbi))]
      nom.de.fichier.xhl        <<-    nom.xhl[file.exists(chemin(nom.xhl))]
      seuil.troncature          <<-    seuil
      verbosité                 <<-    verbosité
      # rapport de lecture des paramètres d'entrée
      # l'interface externe a été simplifiée par rapport aux noms de chams internes
      # pour éviter trop de verbosité à l'usage tout en gardant des noms de champs précis
      
      if (verbosité > 1) champs()
      
      #on ne garde que les noms de fichier qui correspondent à des fichiers du système

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
          "libellé des stagiaires           [ stagiaire =", code.stagiaire, "]\n",
          "libellé des titulaires           [ titulaire =", code.titulaire, "]\n",
          "libellé des  traitements         [ traitement =", code.traitement, "]\n",
          "libellé des vacations            [ vacations =", code.vacation, "]\n",
          "colonnes sélectionnées           [ colonnes =", colonnes.sélectionnées, "]\n",
          "format de date                   [ date =", date.format, "]\n",
          "début de période sous revue      [ début =", début.période.sous.revue, "]\n",
          "décoder les fichiers .*xhl       [ décoder =", décoder.xhl, "]\n",
          "dossier des bases                [ dossier.bases =", dossier.bases, "]\n",
          "dossier des statistiques         [ dossier.stats =", dossier.stats, "]\n",
          "champ des codes                  [ code =", étiquette.code, "]\n",
          "champ des libellés               [ libellé =", étiquette.libellé, "]\n",
          "champ des matricules             [ matricule =", étiquette.matricule, "]\n",
          "champ des montants               [ montant =", étiquette.montant, "]\n",
          "champ des  statuts               [ statut =", étiquette.statut, "]\n",
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
    

