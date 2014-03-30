##
#  Fonctions auxiliaires
##


#  chemin:  string  ->  string
#
#  préfixe le chemin du dossier de travail au chemin de arg0

chemin <-  function(fichier) 
  file.path(dossier.travail, fichier)

#  scan.prime:  string  ->  data.frame
#
#  Cherche l'expression régulière arg0 dans un Libellé de paiment, sans tenir
#  compte de la casse et renvoie les matricules et libellés correspondants, sans
#  doublons

scan.prime <- function(texte)
{
  unique(Bdp.ldp[grep(paste0(".*(", texte,").*"), Bdp.ldp$Libellé, ignore.case=TRUE), c("Matricule", "Libellé", "Libellé")])
}

#  trouver.valeur.skip: string  ->  numeric(1)
#
#  Trouve le numéro de la ligne à laquelle se situe la liste des noms de variables
#  en recherchant soit le mot "Matricule" soit une expression du type "Code..."
#  Il faudra déduire ce "skip" du read.csv2 pour récupérer proprement les noms de variable
#  Pour cela on scanne les 25 premières lignes de la table une première fois

trouver.valeur.skip <-  function(chemin.table) 
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


#  enlever.colonnes.doublons.sauf.1:  data.frame, data.frame  ->  data.frame
#
#  Enlève les colonnes de arg0 qui sont dans arg1 sauf pour la clé de fusion
#  champ.détection.1

enlever.colonnes.doublons.sauf.1 <-  function(Base1, Base2) 
  subset(Base1, 
         select=c(champ.detection.1, setdiff(names(Base1),names(Base2))))

#  enlever.colonnes.doublons.sauf.2:  data.frame, data.frame  ->  data.frame
#
#  Enlève les colonnes de arg0 qui sont dans arg1 sauf pour la clé de fusion
#  champ.détection.1 et pour la clé auxiliaire champ.détection.2. Version tolérante
#  de la fonction précédente

enlever.colonnes.doublons.sauf.2 <-  function(Base1, Base2) 
  subset(Base1, 
         select=c(champ.detection.1,champ.detection.2,
                  setdiff(names(Base1),names(Base2))))


#  read.csv.skip:  string -> data.frame
#
#  Lit une base en sautant les lignes avant les champs de détection automatique
#  de la première ligne de noms de colonne, par ex. "Matricule"
#  spécifiés par champ.détection.1 et champ.détection.2

read.csv.skip <- function(x) 
{
  chem <- chemin(x)
  read.csv2(chem, skip=trouver.valeur.skip(chem), fileEncoding="UTF-8")
}

#  sauv.base:  object  ->  IO(file)
#
#  Sauvegarde un objet X sous la forme d'une base csv2 sans noms de ligne, en encodage UTF-8
#  dans le dossier de travail et sous le nom "X.csv2" 

sauv.base<- function(x)  
  {
    if (vérifier.intégrité(x, poursuivre=TRUE) == TRUE)
        write.csv2(x, paste0(chemin(deparse(substitue(x)), ".csv")), row.names=FALSE, fileEncoding = "UTF-8")
  }

#  sauv.bases:  ...  ->  IO(file),...
#
#  itère sauv.base sur chacun des arguments de la fonction

sauv.bases <- function(...) 
{
  tmp <- as.list(match.call()) 
  tmp[1] <- NULL
  lapply(tmp, sauv.base)
  return(0)
}

#  Read.csv: vector(string)  ->   data.frame
#
#  Lit un vecteur de chemins et empile verticalement les bases correspondant à ces chemins
#  qui résultent d'une importation csv2 par read.csv.skip
  
Read.csv <- function(vect.chemin)   do.call(rbind, lapply(vect.chemin, read.csv.skip))

#  pretty.print: string  ->  IO(console)
#
#  Affiche le nom d'unobjet et sa valeur en enlevant les points de la chaine de caractères du nom

pretty.print <- function(x) cat(gsub(".", " ",deparse(substitute(x)), fixed=TRUE), "   ", x,"\n")

#  est.code.de.type:  string  ->  bool
#
#  teste si la valeur du champ Code de Bdp.ldp est de type arg0 dans Code.prime$Type.rémunération
#

est.code.de.type <- function(x) Bdp.ldp$Code %in% Code.prime[Code.prime$Type.rémunération == x, "Code"]

#  vérifier.intégrité:  ..., poursuivre=FALSE  ->  IO(console|exec)
#
#  Vérifie si aucune dimension n'est nulle dans les  objets en argument
#  Si au moins un objet a au moins une dimension nulle, alors :
#    - si poursuivre == FALSE :  terminer la session
#    - sinon continuer en laissant un message d'erreur.


vérifier.intégrité <- function(..., poursuivre=FALSE) 
{
  tmp <- as.list(match.call()) 
  tmp[1] <- NULL
  
  if (all(lapply(lapply(tmp, eval, envir=.GlobalEnv), assertthat::not_empty)) != TRUE)
  {
    message("Un des objets :")
    cat(unlist(lapply(tmp, deparse)))
    message("est de dimension nulle")
    if (formals()$poursuivre == FALSE)
       stop("Fin de la session.")
    
    return(FALSE)
  }
  
  return(TRUE)
}
