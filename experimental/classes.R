library(compiler)
source("bibliothèque.altair.R")
installer.paquets(assertthat, plyr, rigoureusement = TRUE)
library(assertthat)
enableJIT(3)

noyau.générateur <- setRefClass(
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

#         if (length(arguments) == 0) 
#             arguments <<- fields()
        
        if (all(unlist(lapply(lapply(arguments, eval, envir=.self), assertthat::not_empty))) != TRUE)
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

#' Classe Altair
#'
#' altair.générateur est le générateur de la classe Altair.
#' Il permet d'importer l'ensemble des données qui 
#' caractériseront les bases de données Altair générées 
#' par \code{\link{base.générateur}}
#' @param champ1          ["Matricule"]    Etiquette du champ qui sera détecté en priorité pour la lecture des bases.
#' @param champ2          ["Mois"]         Etiquette du champ qui sera détecté en second lieu pour la lecture des bases.
#' @param catégorie       ["Categorie"]    Etiquette du champ des catégories statutaires.
#' @param code            ["Code"]         Etiquette du champ des codes de rémunération.
#' @param libellé         ["Libelle"]      Etiquette du champ des libellés de rémunération.
#' @param matricule       ["Matricule"]    Etiquette du champ du matricule d'un agent.
#' @param montant         ["Montant"]      Etiquette du champ du montant de la rémunération.
#' @param statut          ["Statut"]       Etiquette du champ du statut de l'agent (titulaire, stagiaire...)
#' @param totalgénéral    ["Total"]        Etiquette du champ du total des rémunérations brutes pour une année.  
#' @param type            ["Type.rémunération"] Etiquette du champ du type de rémunération (traitement, vacation...)
#' @param autre           ["AUTRES"]       Libelle de la valeur du champ `type' pour les rémunérations non catégorisées.
#' @param élu             ["ELU"]          Libelle utilisé pour identifier le service de l'organisme comme celui des élus.
#' @param nbi             ["NBI"           Libelle de la valeur du champ `type' pour les rémunérations de type NBI. 
#' @param prime           ["INDEMNITAIRE.OU.CONTRACTUEL"]  Libelle de la valeur du champ `type' pour les rémunérations de type indemnitaire des fonctionnaires, hors vacations, ainsi que pour les rémunérations non indexées sur INM des contractuels. 
#' @param stagiaire       ["STAGIAIRE"]    Libelle du statut d'un agent fonctionnaire stagiaire.
#' @param titulaire       ["TITULAIRE"]    Libelle du statut d'un agent fonctionnaire titulaire.
#' @param traitement      ["TRAITEMENT"]   Libelle de la valeur du champ `type' pour les rémunérations indexées sur INM, de fonctionnaire ou de contractuel.
#' @param vacation        ["VACATION"]     Libelle de la valeur du champ `type' pour les rémunérations de vacations.
#' @param date            ["%d/%m/%Y"]     Format de date :  %d pour les jours, %m pour les mois, %Y pour les années.
#' @param début           [2008]           Annee de la première base requêtée.
#' @param fin             [2013]           Annee de la dernière base requêtée. 
#' @param seuil           [100]            Nombre de jours minimum que doit avoir travaillé l'agent en année de début et de fin de requête pour inclure l'agent dans les statistiques de variations de rémunérations.
#' @param bases           [FALSE]          Valeur logique : si vaut TRUE, générer les bases .csv
#' @param codage          [TRUE]           Valeur logique : si vaut TRUE, générer les codes de rémunération.
#' @param décoder         [FALSE]          Valeur logique : si vaut TRUE, importer les données .xhl des fichiers de paie au format XML.
#' @param distributions   [TRUE]           Valeur logique : si vaut TRUE, générer la distribution des rémunérations.
#' @param tests           [TRUE]           Valeur logique : si vaut TRUE, générer les tests statutaires.
#' @param variations      [TRUE]           Valeur logique : si vaut TRUE, générer l'analyse des variations.
#' @param colonnes        [c(...)]         Colonnes de variables à produire dans la base principale en sortie.  
#' @param dossier.travail [getwd()]        Dossier de travail.
#' @param dossier.bases   ["Altair/bases"] Dossier de sortie des bases .csv
#' @param dossier.stats   ["Altair/stats"] Dossier de sortie des statistiques.
#' @param nom.avantages   ["avantages"]    Nom du fichier .csv des avantages en nature.
#' @param nom.base        ["base"]         Nom du fichier .csv de la base principale de sortie.
#' @param nom.bulletins   ["Bulletins de paie"] Nom du fichier .csv de la base des bulletins de paie.
#' @param nom.catégories  ["catégories"]   Nom du fichier .csv de la base des catégories (A, B, C,...) des agents.
#' @param nom.codes       ["codes"]        Nom du fichier .csv des codes de rémunération.
#' @param nom.lignes      ["Lignes de paie"] Nom du fichier .csv des lignes de paie.  
#' @param nom.nbi         ["NBI"]          Nom du fichier .csv des rémunérations de type NBI.
#' @param nom.xhl         ["paie"]         Nom du fichier .xhl ( base XML de paie) 
#' @param verbosité       [0]              Niveau de verbosité (0, 1, 2)
#' @param fusion.intégrale [TRUE]          Fusionner les bases en ajoutant des lignes lorsqu'il y a discordance sur la clé.
#' @return Retourne un instance de classe Altair par la méthode new(...)
#' @note La classe est de type classe de références (RefClass).
#' @author Fabrice Nicol
#' @seealso Link to functions in the same package with
#' \code{\link{base.générateur}}
#' @examples
#' # Génère altair pour les années 2009 à 2013 et enregistre les bases 
#' # .csv en sortie du programme
#' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
#' bases  <- bases.générateur$new(altair)
#' @export


altair.générateur <- setRefClass(
  "Altair",
  contains="Noyau",
  fields=list(
    champ.détection.1         = "character",
    champ.détection.2         = "character",
    étiquette.catégorie       = "character",
    étiquette.code            = "character",
    étiquette.libellé         = "character",
    étiquette.matricule       = "character",
    étiquette.montant         = "character",
    étiquette.statut          = "character",
    étiquette.totalgénéral    = "character",
    étiquette.type.rémunération = "character",
    code.autre                = "character",
    code.élu                  = "character",
    code.nbi                  = "character",
    code.prime.ou.contractuel = "character",
    code.stagiaire            = "character",
    code.titulaire            = "character",
    code.traitement           = "character",
    code.vacation             = "character",
    date.format               = "character",
    début.période.sous.revue  = "numeric",
    fin.période.sous.revue    = "numeric",
    seuil.troncature          = "numeric",
    générer.bases             = "logical",
    générer.codes             = "logical",
    décoder.xhl               = "logical",
    générer.distributions     = "logical",
    générer.tests             = "logical",
    générer.variations        = "logical",
    colonnes.sélectionnées    = "character",
    dossier.travail           = "character",
    dossier.bases             = "character",
    dossier.stats             = "character",
    nom.de.fichier.avantages  = "character",
    nom.de.fichier.base       = "character",
    nom.de.fichier.bulletins  = "character",
    nom.de.fichier.catégories = "character",
    nom.de.fichier.codes      = "character",
    nom.de.fichier.lignes     = "character",
    nom.de.fichier.nbi        = "character",
    nom.de.fichier.xhl        = "character",
    verbosité                 = "numeric",
    fusion.intégrale          = "logical"
    ),
  
  methods=list(
    
    initialize = function(
      champ1                  = "Matricule",
      champ2                  = "Mois",
      catégorie               = "Categorie",
      code                    = "Code",
      libellé                 = "Libelle",
      matricule               = "Matricule",
      montant                 = "Montant",
      statut                  = "Statut",
      totalgénéral            = "Total",
      type                    = "Type.rémunération",
      autre                   = "AUTRES",
      élu                     = "ELU",
      nbi                     = "NBI",
      prime                   = "INDEMNITAIRE.OU.CONTRACTUEL",
      stagiaire               = "STAGIAIRE",
      titulaire               = "TITULAIRE",
      traitement              = "TRAITEMENT",
      vacation                = "VACATION",
      date                    = "%d/%m/%Y",
      début                   =  2008,
      fin                     =  2013,
      seuil                   =  100,
      bases                   =  FALSE,
      codage                  =  TRUE,
      décoder                 =  FALSE,
      distributions           =  TRUE,
      tests                   =  TRUE,
      variations              =  TRUE,
      colonnes                = c("Matricule",
                                  "Statut",
                                  "Code",
                                  "Mois",
                                  "Libelle",
                                  "Montant"),
      dossier.travail         = getwd(),
      dossier.bases           = "bases",
      dossier.stats           = "stats",
      nom.avantages           = "avantages.csv",
      nom.base                = "base.csv",
      nom.bulletins           = "paies-Bulletins de paye-1.csv",
        #"Bulletins de paye",
      nom.catégories          = "catégories.csv",
      nom.codes               = "LISTE DES RUBRIQUES DE TRAITEMENT UTILISEES EN 2012.csv",
      nom.lignes              = "paies-Lignes de paye-1.csv",
        #"Lignes de paye", 
      nom.nbi                 = "NBI.csv",
      nom.xhl                 = "paie",
      verbosité               =  0,
      fusion.intégrale        = TRUE
      )
    {
"Assigne les champs paramètres des fonctions de traitement statistique"
      
      champ.détection.1         <<-    champ1
      champ.détection.2         <<-    champ2
      étiquette.catégorie       <<-    catégorie
      étiquette.code            <<-    code
      étiquette.libellé         <<-    libellé
      étiquette.matricule       <<-    matricule
      étiquette.montant         <<-    montant
      étiquette.statut          <<-    statut
      étiquette.totalgénéral    <<-    totalgénéral 
      étiquette.type.rémunération <<-  type
      code.autre                <<-    autre
      code.élu                  <<-    élu
      code.nbi                  <<-    nbi
      code.prime.ou.contractuel <<-    prime
      code.stagiaire            <<-    stagiaire
      code.titulaire            <<-    titulaire
      code.traitement           <<-    traitement
      code.vacation             <<-    vacation
      date.format               <<-    date
      début.période.sous.revue  <<-    début
      fin.période.sous.revue    <<-    fin
      seuil.troncature          <<-    seuil
      générer.bases             <<-    bases
      générer.codes             <<-    codage
      générer.distributions     <<-    distributions
      générer.tests             <<-    tests
      générer.variations        <<-    variations
      colonnes.sélectionnées    <<-    colonnes
      décoder.xhl               <<-    décoder
      dossier.travail           <<-    dossier.travail
      dossier.bases             <<-    dossier.bases
      dossier.stats             <<-    dossier.stats
      nom.de.fichier.avantages  <<-    nom.avantages
      nom.de.fichier.base       <<-    nom.base
      nom.de.fichier.bulletins  <<-    nom.bulletins
      #nom.de.fichier.bulletins  <<-    nom.bulletins[file.exists(chemin(nom.bulletins))]
      nom.de.fichier.catégories <<-    nom.catégories
      nom.de.fichier.codes      <<-    nom.codes
      nom.de.fichier.lignes     <<-    nom.lignes
      #nom.de.fichier.lignes     <<-    nom.lignes[file.exists(chemin(nom.lignes))]
      nom.de.fichier.nbi        <<-    nom.nbi
      #nom.de.fichier.nbi        <<-    nom.nbi[file.exists(chemin(nom.nbi))]
      nom.de.fichier.xhl        <<-    nom.xhl
      #nom.de.fichier.xhl        <<-    nom.xhl[file.exists(chemin(nom.xhl))]
      verbosité                 <<-    verbosité
      fusion.intégrale          <<-    fusion.intégrale

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

#' Classe Altair: méthode champs()
#'
#' @return Affiche l'ensemble des arguments possibles de la méthode new(...) générateur de la classe et leurs valeur par défaut.
#' @author Fabrice Nicol
#' @examples
#' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
#' altair$champs()
#' @export

    champs = function()
    {
      cat(" champ de détection prioritaire   [ champ1 =", champ.détection.1,"]\n",
          "champ de détection secondaire    [ champ2 =", champ.détection.2, "]\n",
          "champ des catégories             [ catégorie =", champ.détection.2, "]\n",
          "champ des codes                  [ code =", étiquette.code, "]\n",
          "champ des libellés               [ libellé =", étiquette.libellé, "]\n",
          "champ des matricules             [ matricule =", étiquette.matricule, "]\n",
          "champ des montants               [ montant =", étiquette.montant, "]\n",
          "champ des  statuts               [ statut =", étiquette.statut, "]\n",
          "champ du total général           [ totalgénéral =", étiquette.totalgénéral, "]\n",
          "champ du type de rémunération    [ type =", étiquette.type.rémunération, "]\n",
          "libellé des autres primes        [ autre =",  code.autre, "]\n",
          "libellé pour service 'élu'       [ élu =", code.élu, "]\n",
          "libellé des NBI                  [ nbi =", code.nbi, "]\n",
          "libellé des rémunérations\n indemnitaires ou contractuelles  [ prime =", code.prime.ou.contractuel, "]\n",
          "libellé des stagiaires           [ stagiaire =", code.stagiaire, "]\n",
          "libellé des titulaires           [ titulaire =", code.titulaire, "]\n",
          "libellé des  traitements         [ traitement =", code.traitement, "]\n",
          "libellé des vacations            [ vacations =", code.vacation, "]\n",
          "format de date                   [ date =", date.format, "]\n",
          "début de période sous revue      [ début =", début.période.sous.revue, "]\n",
          "fin de la période sous revue     [ fin =", fin.période.sous.revue, "]\n",
          "durée minimum de travail\n première et dernière année       [ durée =", seuil.troncature, "]\n",
          "génération des bases .csv        [ bases =", générer.bases, "]\n",
          "génération des codes de paye     [ codage =", générer.codes, "]\n",
          "génération des distributions     [ distributions =", générer.distributions, "]\n",
          "génération des tests statutaires [ tests =", générer.tests, "]\n",
          "génération des variations        [ variations =", générer.variations, "]\n",
          "colonnes sélectionnées           [ colonnes =", colonnes.sélectionnées, "]\n",          
          "décoder les fichiers .*xhl       [ décoder =", décoder.xhl, "]\n",
          "dossier de travail               [ dossier.travail =", dossier.travail, "]\n",        
          "dossier des bases                [ dossier.bases =", dossier.bases, "]\n",
          "dossier des statistiques         [ dossier.stats =", dossier.stats, "]\n",
          "nom du fichier des avantages     [ nom.avantages =", nom.de.fichier.avantages, "]\n",
          "nom du fichier de base en sortie [ nom.base =", nom.de.fichier.base, "]\n",
          "nom du fichier de\n bulletins de paye                [ nom.bulletins =", nom.de.fichier.bulletins, "]\n",
          "nom du fichier de catégories     [ nom.catégories =", nom.de.fichier.catégories, "]\n",
          "nom du fichier de codes          [ nom.codes =", nom.de.fichier.codes, "]\n",
          "nom du fichier de lignes de paye [ nom.lignes =", nom.de.fichier.lignes, "]\n",
          "nom du fichier de NBI            [ nom.nbi =", nom.de.fichier.nbi, "]\n",
          "nom du fichier de xhl            [ nom.xhl =", nom.de.fichier.xhl, "]\n",
          "verbosité                        [ verbosité =", verbosité, "]\n")
          
      
    }
  )
)


#' Classe Base
#'
#' base.générateur est le générateur de la classe Base.
#' grâce aux données entrées par l'instance de la classe Altair,
#' voir  \code{\link{altair.générateur}}
#' @param altair          [altair.générateur$new()]  Valeurs par défaut des champs de classe Altair
#' @param bulletins       [data.frame(NULL)]         Base .csv des bulletins de paie
#' @param codes           [data.frame(NULL)]         Base .csv des codes de paie
#' @param lignes          [data.frame(NULL)]         Base .csv des lignes de paie 
#' @param avantages       [data.frame(NULL)]         Base .csv des avantages en nature
#' @param catégories      [data.frame(NULL)]         Base .csv des catégories statutaires (A, B, C, ...) des agents
#' @param nbi             [data.frame(NULL)]         Base .csv des rémunérations NBI
#' @return Retourne un instance de classe Base par la méthode new(...)
#' @note La classe est de type classe de références (RefClass).
#' @author Fabrice Nicol
#' @seealso Link to functions in the same package with
#' \code{\link{altair.générateur}}
#' @examples
#' # Génère les bases .csv pour les années 2009 à 2013 
#' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
#' bases  <- bases.générateur$new(altair)
#' @export

base.générateur <- setRefClass(
  "Base",
   contains="Noyau",
   fields=list(
     altair                    = "Altair",
     doss                      = "character",
     Global                    = "data.frame",
     Bulletins                 = "data.frame",
     Codes                     = "data.frame",
     Codes.fonct               = "data.frame",
     Codes.NBI                 = "data.frame",
     Codes.NT                  = "data.frame",
     Lignes                    = "data.frame", 
     Avantages                 = "data.frame",
     Categories                = "data.frame",
     NBI                       = "data.frame"),
  
   methods=list(
    initialize = function(altair) 
      {
        altair <<- altair
        doss   <<- altair$dossier.travail
      }, 
    
    décoder.xhl = function()
     {
      .NotYetImplemented()
     },

    #' Base::trouver.valeur.skip
    #'
    #' Calcule le nombre de lignes qu'il faut sauter à l'importation d'une base .csv
    #' pour pourvoir lire le champ de détection prioritaire 1 ou, s'il n'est pas détecté,
    #' le champ de détection secondaire 2.
    #' 
    #' @param chemin  Chemin complet de la base .csv à importer
    #' @return Renvoie entier.
    #' @note   Pour cela on scanne les 25 premières lignes de la table une première fois.
    #' @author Fabrice Nicol
    #' @examples
    #' # Génère les bases .csv pour les années 2009 à 2013 
    #' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
    #' base   <- base.générateur$new(altair)
    #' base$trouver.valeur.skip(altair$chemin.lignes())
    #' @export
    #' 
    #'   
    
    trouver.valeur.skip =  function(x) 
     {
       "trouver.valeur.skip: character  ->  numeric(1)
    
    Trouve le numéro de la ligne à laquelle se situe la liste des noms de variables
    en recherchant soit le mot `Matricule' soit une expression du type `Code...'
    Il faudra déduire ce `skip' du read.csv2 pour récupérer proprement les noms de variable
    Pour cela on scanne les 25 premières lignes de la table une première fois"
       
       max(
         sapply(
           read.csv2(x, nrows=25),
           function(y) 
           {
             m <- match(altair$champ.détection.1, y, nomatch=0 ) 
             if (m == 0)
               m <- pmatch(altair$champ.détection.2, y, nomatch=0, duplicates.ok=FALSE ) 
             return(m)
           }
         ))
     },


    chemin =  function(fichier) 
    {
      if (!see_if(is.dir(doss)))
      {
        stop("Pas de dossier de travail spécifié")
      }
      file.path(doss, fichier)
    },

    #' Base::read.csv.skip
    #'
    #' Lit une base en sautant les lignes avant les champs de détection automatique
    #' de la première ligne de noms de colonne, par ex. `Matricule'
    #' spécifiés par champ.détection.1 et champ.détection.2
    #' 
    #' @param chemin  Chemin complet de la base .csv à importer
    #' @return Renvoie un data.frame.
    #' @author Fabrice Nicol
    #' @examples
    #' # Génère les bases .csv pour les années 2009 à 2013 
    #' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
    #' base   <- base.générateur$new(altair)
    #' base$read.csv.skip(altair$chemin.lignes())
    #' @export
    #' 
    
   Read.csv = function(vect.chemin)
  {
    "Read.csv: vector(character)  ->   data.frame
    
    Lit un vecteur de chemins et empile verticalement les bases correspondant à ces chemins
    qui résultent d'une importation csv2 par read.csv.skip"
    
    usingMethods(read.csv.skip)
    do.call(rbind, lapply(vect.chemin, read.csv.skip))
  },

  read.csv.skip = function(x) 
  {
    
    "read.csv.skip:  character -> data.frame
    
    Lit une base en sautant les lignes avant les champs de détection automatique
    de la première ligne de noms de colonne, par ex. `Matricule'
    spécifiés par champ.détection.1 et champ.détection.2"
    
    ch <- chemin(x)
    tryCatch(read.csv2(ch, skip=trouver.valeur.skip(ch), fileEncoding="UTF-8"), error = function(e) data.frame(NULL))
  },
  
  #' Base::importer
  #'
  #' Importe les différentes bases .csv2 données en input 
  #' à savoir :
  #' Lignes     Base des lignes de paie
  #' NBI        Base des rémunérations de type NBI
  #' Bulletins  Base des bulletins de paie
  #' Categories Base des catégories statutaires (A, B, C,...)
  #' Codes      Base des codes de paie
  #' Avantages  Base des avantages en nature
  #' 
  #' Fusionne les bases Bulletins et Lignes dans la base Global.
  #' 
  #' @author Fabrice Nicol
  #' @examples
  #' # Génère les bases .csv pour les années 2009 à 2013 
  #' altair <- altair.générateur$new(début=2009, fin=2013, bases=TRUE)
  #' base   <- base.générateur$new(altair)
  #' base$importer()
  #' @export
  
  
  importer = function()
  {
    "Importe les différentes bases .csv2 données en input."
    
    Lignes      <<-  Read.csv(altair$nom.de.fichier.lignes)
    NBI         <<-  Read.csv(altair$nom.de.fichier.nbi)
    Bulletins   <<-  Read.csv(altair$nom.de.fichier.bulletins)
    Categories  <<-  Read.csv(altair$nom.de.fichier.catégories)
    Codes       <<-  Read.csv(altair$nom.de.fichier.codes)
    Avantages   <<-  Read.csv(altair$nom.de.fichier.avantages)
    
    vérifier.intégrité(
      Lignes,
      NBI,
      Bulletins,
      Categories,
      Codes,
      Avantages,
      poursuivre = TRUE)
    
    #suppression des colonnes Nom Prenom redondantes
    
#     Avantages   <<-  selectionner.cle.matricule(Avantages, Categories) 
#     Bulletins   <<-  selectionner.cle.matricule.mois(Bulletins, Lignes)
    
    #fusion matricule | avantage | catégorie par Matricule
    
    Global <<- merge(Bulletins, Lignes, all=altair$fusion.intégrale)
    delta <- nrow(Global) -  nrow(Lignes)

    if ( delta > 0)
      message("La fusion des bases ", altair$nom.de.fichier.bulletins, " et ", altair$nom.de.fichier.lignes, " a augmenté le nombre de lignes de ", delta," lignes. Des erreurs peuvent en résulter pour les statistiques.")

    # un peu par acquis de conscience...
    
    vérifier.intégrité(Global)
  },

lancer = function() 
{
  "Lance l'importantion de toutes les bases"
  message("OK")
  # si altair prévoit un décodage xml, alors lancer ce décodage dans un fichier temp
  # puis attribuer directement base au résultat
  # sinon, lire lignes, bulletins, nbi et fusionner
  # mode in : NBI, Lignes, Bulletins dans le premier cas ; *.xhl dans le second
  # mode out: Global, NBI, Lignes, Bulletins dans les deux cas
  
  altair <<- altair
  
  if (altair$décoder.xhl) 
    décoder.xhl(altair$nom.de.fichier.xhl)
  else  
    importer()
  
 # if (altair$générer.codes == TRUE)          générer.codes()
 # if (altair$générer.tests == TRUE)          générer.tests()
#   if (altair$générer.distributions == TRUE)  générer.distributions()
#   if (altair$générer.variations == TRUE)     générer.variations()
})  

  
)
  
  
  
# à implémenter:
# décoder.xhl(altair$nom.de.fichier.xhl)



