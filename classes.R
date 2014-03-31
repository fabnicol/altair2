
altair.générateur <- setRefClass(
  "Altair",
  contains ="Coeur",
  fields=list(
    Base                      = "data.frame",
    bulletins.de.paie         = "data.frame",
    champ.détection.1         = "character",
    champ.détection.2         = "character",
    code.autre                = "character",
    code.nbi                  = "character",
    code.prime.ou.contractuel = "character",
    code.traitement           = "character",
    code.vacation             = "character",
    Codes                     = "data.frame",
    colonnes.sélectionnées    = "character",    
    date.format               = "character",
    début.période.sous.revue  = "numeric",
    décoder.xhl               = "logical",
    dossier.travail           = "character",
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
    lignes.de.paie            = "data.frame", 
    matricule.avantage        = "character",
    matricule.categorie       = "character",
    nom.de.fichier.nbi        = "character",
    nom.de.fichier.paie       = "character",
    nom.de.fichier.primes     = "character",
    seuil.troncature          = "numeric"
    ),
  
  methods=list(
    initialize = function(
      bulletins               = data.frame(NULL),
      champ1                  = "Matricule",
      champ2                  = "Mois",
      autre                   = "AUTRES",
      nbi                     = "NBI",
      prime                   = "INDEMNITAIRE.OU.CONTRACTUEL",
      traitement              = "TRAITEMENT",
      vacation                = "VACATION",
      Codes                   = data.frame(NULL),
      colonnes                = c("Matricule",
                                  "Statut",
                                  "Code",
                                  "Mois",
                                  "Libellé",
                                  "Montant"),
      date                    = "%d/%m/%Y",
      début                   =  2008,
      décoder                 = FALSE,
      dossier                 = "Altair",
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
      lignes                  =  data.frame(NULL),
      fichier.avantages       =  "avantages.csv",
      fichier.categories      =  "catégories.csv",
      fichier.nbi             =  "paies-NBI-1",
      fichier.paie            =  "paies-Bulletins de paye-1",
      fichier.primes          =  "codes.csv",
      seuil                   =  100
      )
    {
      "Assigne les champs paramètres des fonctions de traitement statistique"
      
      bulletins.de.paie         <<-    bulletins
      champ.détection.1         <<-    champ1
      champ.détection.2         <<-    champ2
      code.autre                <<-    autre
      code.nbi                  <<-    nbi
      code.prime.ou.contractuel <<-    prime
      code.traitement           <<-    traitement
      code.vacation             <<-    vacation
      Codes                     <<-    Codes
      colonnes.sélectionnées    <<-    colonnes
      date.format               <<-    date
      début.période.sous.revue  <<-    début
      décoder.xhl               <<-    décoder
      dossier.travail           <<-    dossier
      dossier.bases             <<-    dossier.bases
      dossier.stats             <<     dossier.stats
      étiquette.code            <<-    codage
      étiquette.libellé         <<-    libellé,
      étiquette.matricule       <<-    matricule
      étiquette.montant         <<-    montant
      étiquette.totalgénéral    <<-    totalgénéral 
      étiquette.type.rémunération <<-  type
      fin.période.sous.revue    <<-    fin
      générer.bases             <<-    bases
      générer.codes             <<-    codage
      générer.distributions     <<-    distributions
      générer.tests             <<-    tests
      lignes.de.paie            <<-    ifelse(length(lignes) == 0
      matricule.avantage        <<-    avantage
      matricule.categorie       <<-    categorie
      nom.de.fichier.nbi        <<-    fichier.nbi
      nom.de.fichier.paie       <<-    fichier.paie
      nom.de.fichier.primes     <<-    fichier.primes
      seuil.troncature          <<-    seuil
    },
    
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
      
        teste si la valeur du champ Code de Bdp.ldp est de type arg0 dans Code.prime$Type.rémunération"

       Base[, étiquette.code] %in% Codes[Codes[, étiquette.type.rémunération] == x, étiquette.code]
    }
      
    
    )
  )


coeur <- setRefClass(
  "Coeur",
   fields=list(base,
               vecteur),
  
  methods=list(
    initialize(base = data.frame(NULL), vecteur = rep())
    vérifier.intégrité = function(..., poursuivre=FALSE) 
    {
      "vérifier.intégrité:  ..., poursuivre=FALSE  ->  IO(console|exec)
                            poursuivre=FALSE       ->  IO(console|exec)
      
       Vérifie si aucune dimension n'est nulle dans les  objets en argument.
       Avec argument, si au moins un objet a au moins une dimension nulle, alors :
         - si poursuivre == FALSE :  terminer la session
         - sinon continuer en laissant un message d'erreur.
       Sans argument, opère comme avec l'ensemble de ses champs en argument."
      
      tmp <- as.list(match.call()) 
      tmp[1] <- NULL
      if (length(tmp) == 0) tmp <- fields()
      if (all(lapply(lapply(tmp, eval, envir=.GlobalEnv), assertthat::not_empty)) != TRUE)
      {
        message("Un des objets :")
        cat(unlist(lapply(tmp, deparse)))
        message("est de dimension nulle")
        if (formals()$poursuivre == FALSE)
          stop("Fin de la session.")
        
        return(FALSE)
      }
    },
    
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
        write.csv2(x, paste0(chemin(deparse(substitue(x)), ".csv")), row.names=FALSE, fileEncoding = "UTF-8")
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
)
  
  return(TRUE)
}
