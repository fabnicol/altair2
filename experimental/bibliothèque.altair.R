
#if (!exists("dossier.travail", envir=.GlobalEnv) || nchar(dossier.travail) == 0)  dossier.travail <- getwd()

##
#  Fonctions auxiliaires (globales)
##

dossier.travail <- "/home/fab/Dev/Altair/altair"

#  chemin:  string  ->  string
#
#  préfixe le chemin du dossier de travail au chemin de arg0

chemin <-  function(fichier) 
{
  if (!see_if(is.dir(dossier.travail)))
  {
     stop("Pas de dossier de travail spécifié")
  }
      file.path(dossier.travail, fichier)
}

#  scan.prime:  string  ->  data.frame
#
#  Cherche l'expression régulière arg0 dans un Libelle de paiment, sans tenir
#  compte de la casse et renvoie les matricules et libellés correspondants, sans
#  doublons

pretty.print <- function(x) cat(gsub(".", " ",deparse(substitute(x)), fixed=TRUE), "   ", x,"\n")

installer.paquet <- function(paquet, rigoureusement = FALSE) 
  {
   if (missing(paquet))  return(NA_character_)
   Paquet <- deparse(paquet)
   if (length(find.package(Paquet, quiet=TRUE)) == 0)
   {
       install.packages(Paquet)
       if (length(find.package(Paquet, quiet=TRUE)) !=0 )
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

