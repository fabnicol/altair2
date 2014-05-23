#----------------------------------------------------------------------------------------------------------------------
##
#  Fonctions auxiliaires
##

# Trouve le numéro de la ligne à laquelle se situe la liste des noms de variables
# en recherchant soit le mot "Matricule" soit une expression du type "Code..."
# Il faudra déduire ce "skip" du read.csv2 pour récupérer proprement les noms de variable

# Pour cela on scanne les 25 premières lignes de la table une première fois


scan.prime <- function(texte)
{
	unique(Bdp.ldp[grep(paste0(".*(", texte,").*"), Bdp.ldp$Libellé), c("Matricule", "Libellé", "Libellé")])
}

trouver.valeur.skip <-  function(chemin.table) 
  max(
    sapply(
      read.csv2(chemin.table, nrows=25),
      function(x) 
      {
        m <- match("Matricule", x, nomatch=0 ) 
        if (m == 0)
          m <- pmatch("Code", x, nomatch=0, duplicates.ok=FALSE ) 
        return(m)
      }
    ))


selectionner.cle.matricule <-  function(Base1, Base2) 
  subset(Base1, 
         select=c("Matricule", setdiff(names(Base1),names(Base2))))

selectionner.cle.matricule.mois <-  function(Base1, Base2) 
  subset(Base1, 
         select=c("Matricule","Mois",
                  setdiff(names(Base1),names(Base2))))

chemin <-  function(fichier) 
  file.path(chemin.dossier, fichier)

read.csv.skip <- function(x) 
{
  chem <- chemin(x)
  read.csv2(chem, skip=trouver.valeur.skip(chem))
}

sauv.base <- function(x) write.csv2(x, paste0(chemin(deparse(substitute(x))), ".csv"), row.names=FALSE)


# Utiliser une assignation globale 
# car la fonction anonyme ne comporte que de variables locales

Read.csv <- function(vect.chemin) 
{
  Read.csv.result <- data.frame(NULL)
  Vectorize(function(x) Read.csv.result <<- rbind(Read.csv.result, read.csv.skip(x)))(vect.chemin)
  Read.csv.result
}

pretty.print <- function(x) cat(gsub(".", " ",deparse(substitute(x)), fixed=TRUE), "   ", x,"\n")


#-----------------------------------------------------------------------------------------------------------------
