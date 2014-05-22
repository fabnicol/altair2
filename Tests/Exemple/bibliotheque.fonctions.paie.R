##
#  Fonctions auxiliaires
##

chemin <-  function(fichier) 
  file.path(chemin.dossier.données, fichier)

scan.prime <- function(texte, Base)
{
  unique(Base[grep(paste0(".*(", texte,").*"), Base$Libellé, ignore.case = TRUE), c("Matricule", "Libellé", "Libellé")])
}


# Trouve le numéro de la ligne à laquelle se situe la liste des noms de variables
# en recherchant soit le mot "Matricule" soit une expression du type "Code..."
# Il faudra déduire ce "skip" du read.csv2 pour récupérer proprement les noms de variable

# Pour cela on scanne les 25 premières lignes de la table une première fois


trouver.valeur.skip <-  function(chemin.table, encodage) 
  max(
    sapply(
      read.csv2(chemin.table, nrows = 25, fileEncoding = encodage),
      function(x) 
      {
        m <- match(champ.détection.1, x, nomatch = 0 ) 
        if (m == 0)
          m <- pmatch(champ.détection.2, x, nomatch = 0, duplicates.ok = FALSE ) 
        return(m)
      }
    ))


selectionner.cle.matricule <-  function(Base1, Base2) 
  subset(Base1, 
         select = c(champ.détection.1, setdiff(names(Base1),names(Base2))))

selectionner.cle.matricule.mois <-  function(Base1, Base2) 
  subset(Base1, 
         select = c(champ.détection.1,"Mois","Année",
                  setdiff(names(Base1),names(Base2))))

read.csv.skip <- function(x, encodage = encodage.entrée) 
{
  chem <- chemin(x)
  T <- read.csv2(chem, skip = trouver.valeur.skip(chem, encodage), fileEncoding = encodage)
  if (encodage.entrée != "UTF-8")
     names(T) <- iconv(names(T), to="UTF-8")
  return(T)
}

Sauv.base <- function(chemin.dossier, nom, nom.sauv, encodage = encodage.entrée)
{
  message("Sauvegarde de ", nom)
  write.csv2(get(nom), paste0(chemin.dossier, "/", iconv(nom.sauv, to = encodage.entrée), ".csv"), 
             row.names = FALSE, fileEncoding = encodage)
  
}
 
sauv.bases <- function(dossier, ...) 
{
   
  if (!see_if(is.dir(dossier)))
  {
    stop("Pas de dossier de travail spécifié")
  }
  
  tmp <- as.list(match.call()) 
  tmp[1] <- NULL
 
  message("Dans le dossier ", dossier," :")
  invisible(lapply(tmp[-1], function(x) Sauv.base(dossier, x, x)))
}

# Utiliser une assignation globale 
# car la fonction anonyme ne comporte que de variables locales

Read.csv <- function(Base, vect.chemin, charger = charger.bases)   if (charger.bases) Base <- do.call(rbind, lapply(vect.chemin, read.csv.skip))

pretty.print <- function(x) cat(gsub(".", " ",deparse(substitute(x)), fixed = TRUE), "   ", x,"\n")

est.code.de.type <- function(x, Base) Base$Code %in% Code.prime[Code.prime$Type.rémunération == x, "Code"]

Résumé <- function(x,y, align = 'r',...) 
              {
                 S <- cbind(c("Minimum", "1er quartile", "Médiane", "Moyenne", "3ème quartile", "Maximum"), 
                            prettyNum(sub("[M13].*:", "", summary(y, ...)), big.mark = " "))
                 
                 dimnames(S)[[2]] <- c("Statistique", x)
                 
                 kable(S, row.names = FALSE, align = align)
                 
               }
Tableau <- function(x, ...)
{
  V <- c(...)
  if ("sep.milliers" %in% names(V))
  {
    sep.milliers <- V["sep.milliers"]
    V$sep.milliers <- NULL
  }  else
  sep.milliers <- " "
  
  T <- t(prettyNum(V, big.mark = sep.milliers))
  T <- as.data.frame(T)
  names(T) <- x
  kable(T, row.names = FALSE, align = "c")
}

Tableau.vertical <- function(colnames, rownames, f)
{
  T <- data.frame(rownames,   sapply(rownames, f))
  
  names(T) <- colnames
  
  kable(T, row.names = FALSE, align = "c")
}

Tableau.vertical2 <- function(colnames, données.col1, données.col2)
{

  T <- data.frame(données.col1, prettyNum(données.col2, big.mark=" "))
  
  names(T) <- colnames
  
  kable(T, row.names = FALSE, align = "c")
}


  julian.date.début.période <- julian(as.Date(paste0("01/01/", début.période.sous.revue), date.format))
  julian.exercice.suivant.premier <- julian(as.Date(paste0("01/01/",(début.période.sous.revue+1)), date.format))
  julian.date.fin.période   <- julian(as.Date(paste0("01/01/", fin.période.sous.revue+1), date.format))
  julian.exercice.dernier <- julian(as.Date(paste0("01/01/",fin.période.sous.revue), date.format))

calcul.nb.jours <- function(entrée, sortie) 
{
  
  julian.entrée <- 
    ifelse(entrée == "", 
           julian.date.début.période,
           max(julian.date.début.période, julian(as.Date(entrée, date.format))))
  
  julian.sortie <- 
    ifelse(sortie == "", 
           julian.date.fin.période, 
           min(julian.date.fin.période, julian(as.Date(sortie, date.format))))
  
  return (julian.sortie - julian.entrée)
}

calcul.nb.jours.mois <- function(mois.entrée, mois.sortie, année)
{
  if (mois.sortie < mois.entrée) return(0);
  
  if (mois.sortie == 12) 
  {
     année.sortie <- année +1
     mois.sortie = 1
  }
  else
  {
    année.sortie <- année
    mois.sortie <- mois.sortie + 1
  }
  
   as.numeric(as.Date(paste0("01", 
                                  formatC(mois.sortie, width = 2, flag = "0"),
                                  année.sortie),
                      "%d%m%Y")
              - as.Date(paste0("01",
                                   formatC(mois.entrée, width = 2, flag = "0"),
                                   année),
                            "%d%m%Y"))
}

calcul.nb.jours.dans.exercice.in <- function(entrée) 
{
  date.entrée <- as.Date(entrée, date.format)
  
  if (entrée == "")
  {
    julian.entrée <-  julian.date.début.période
    julian.fin.exercice <- julian.exercice.suivant.premier
  }
  else
  {
    julian.entrée <- julian(date.entrée)
    if (julian.date.début.période < julian.entrée)
      julian.fin.exercice <- julian(as.Date(paste0("01/01/",as.integer(substr(entrée, 7, 10))+1), date.format))
    else
    {
      julian.fin.exercice <- julian.exercice.suivant.premier
      julian.entrée <- julian.date.début.période
    }
  }
  
  return (julian.fin.exercice - julian.entrée)
  
}

calcul.nb.jours.dans.exercice.out <- function(sortie) 
{
  date.sortie <- as.Date(sortie, date.format)
  
  if (sortie == "")
  {
    julian.sortie <-  julian.date.fin.période
    julian.début.exercice <- julian.exercice.dernier
  }
  else
  {
    julian.sortie <- julian(date.sortie)
    if (julian.date.début.période < julian.sortie)
      julian.début.exercice <- julian(as.Date(paste0("01/01/",as.integer(substr(sortie, 7, 10))), date.format))
    else
    {
      julian.début.exercice <- julian.date.début.période
      julian.sortie <- julian.exercice.suivant.premier
    }
  }
  
  return (julian.sortie - julian.début.exercice)
  
}

calcul.variation <- function(rémunération.début, rémunération.sortie, nb.jours.exercice.début, nb.jours.exercice.sortie, nb.exercices)
{
  if (nb.exercices > 1)  
    
    return(ifelse(rémunération.début == 0, 0, ( rémunération.sortie / rémunération.début   - 1  ) * 100))
  
  else
    
    return (0)
  
}

positive <- function(X) X[ X > 0]


installer.paquet <- function(paquet, rigoureusement = FALSE) 
{
  if (missing(paquet))  return(NA_character_)
  Paquet <- deparse(paquet)
  if (length(find.package(Paquet, quiet = TRUE)) == 0)
  {
    install.packages(Paquet, repos="http://cran.univ-lyon1.fr/")
    if (length(find.package(Paquet, quiet = TRUE)) !=0 )
    {
      message(Paquet, " a été installé.")
      return(invisible(1))
    }
    else
    {
      message(Paquet, " n'a pas été installé.")
      if (rigoureusement == TRUE) 
      {
        message("Arrêt: le paquet ", Paquet, " n'a pas pu être installé.")
        stop("Fin")
      }
      return(invisible(0))
    }
  }
  else
    message(Paquet, " est déjà installé.")
  return(invisible(0))
}

installer.paquets <- function(..., rigoureusement = FALSE) 
{
  tmp <- as.list(match.call()) 
  tmp[1] <- NULL
  if (!missing(rigoureusement)) tmp[length(tmp)] <- NULL
  if (length(tmp) == 0) return(0)
  
  invisible(do.call(sum, lapply(tmp, function(x) installer.paquet(x, rigoureusement))))
}



