##
#  Fonctions auxiliaires
##

chemin <-  function(fichier) 
  file.path(chemin.dossier, fichier)

scan.prime <- function(texte, Base)
{
  unique(Base[grep(paste0(".*(", texte,").*"), Base$Libellé, ignore.case=TRUE), c("Matricule", "Libellé", "Libellé")])
}


# Trouve le numéro de la ligne à laquelle se situe la liste des noms de variables
# en recherchant soit le mot "Matricule" soit une expression du type "Code..."
# Il faudra déduire ce "skip" du read.csv2 pour récupérer proprement les noms de variable

# Pour cela on scanne les 25 premières lignes de la table une première fois


trouver.valeur.skip <-  function(chemin.table) 
  max(
    sapply(
      read.csv2(chemin.table, nrows=25, fileEncoding="UTF-8-BOM"),
      function(x) 
      {
        m <- match(champ.détection.1, x, nomatch=0 ) 
        if (m == 0)
          m <- pmatch(champ.détection.2, x, nomatch=0, duplicates.ok=FALSE ) 
        return(m)
      }
    ))


selectionner.cle.matricule <-  function(Base1, Base2) 
  subset(Base1, 
         select=c(champ.détection.1, setdiff(names(Base1),names(Base2))))

selectionner.cle.matricule.mois <-  function(Base1, Base2) 
  subset(Base1, 
         select=c(champ.détection.1,"Mois",
                  setdiff(names(Base1),names(Base2))))

read.csv.skip <- function(x) 
{
  chem <- chemin(x)
  read.csv2(chem, skip=trouver.valeur.skip(chem), fileEncoding="UTF-8-BOM")
}


sauv.base <- function(x)  write.csv2(x, paste0(chemin(deparse(substitute(x))), ".csv"), row.names=FALSE, fileEncoding = "UTF-8")

sauv.bases <- function(...) 
{
  tmp <- as.list(match.call()) 
  tmp[1] <- NULL
  lapply(tmp, sauv.base)
  return(0)
}

# Utiliser une assignation globale 
# car la fonction anonyme ne comporte que de variables locales

Read.csv <- function(vect.chemin)   do.call(rbind, lapply(vect.chemin, read.csv.skip))

pretty.print <- function(x) cat(gsub(".", " ",deparse(substitute(x)), fixed=TRUE), "   ", x,"\n")

est.code.de.type <- function(x, Base) Base$Code %in% Code.prime[Code.prime$Type.rémunération == x, "Code"]

Résumé <- function(x,y, align='r',...) 
              {
                 S <- cbind(c("Minimum", "1er quartile", "Médiane", "Moyenne", "3ème quartile", "Maximum"), 
                            prettyNum(sub("[M13].*:", "", summary(x, ...)), big.mark=" "))
                 
                 dimnames(S)[[2]] <- c("Statistique", y)
                 
                 kable(S, row.names=FALSE, align=align)
                 
               }
Tableau <- function(x,...)
{
  T <- t(prettyNum(c(...), big.mark=" "))
  T <- as.data.frame(T)
  names(T) <- x
  kable(T, row.names=FALSE, align="c")
}