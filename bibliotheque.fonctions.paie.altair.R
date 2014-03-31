##
#  Fonctions auxiliaires (globales)
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

pretty.print <- function(x) cat(gsub(".", " ",deparse(substitute(x)), fixed=TRUE), "   ", x,"\n")

