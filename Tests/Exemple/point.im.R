
an <- as.numeric(format(Sys.time(), "%Y"))

if (début.période.sous.revue < 2008) {
  
  cat("Ce programme ne peut pas traiter les périodes sous revue commançant avant le 1er janvier 2008. Ces années seront écartées.")
  extraire.années <- TRUE
  début.période.sous.revue <- 2008
}
  

PointIM <- matrix(c(
                    54.4113,             # 2008 01
                    54.4113,             # 2008 02
                    rep.int(54.6834, 7), # 2008 3-9 
                    rep.int(54.8475, 9), # 2008 10-12 et 2009 1-6
                    rep.int(55.1217, 3), # 2009 6-9
                    rep.int(55.2871, 9), # 2009 10-12 et 2010 1-6
                    rep.int(55.5635, 72),# 2010 7 -> 2016 6  (6 années de blocage)
       				rep.int(55.8969, 6),# 2016 7 -> 2016 12
       				55.8969,
       				rep.int(56.2323,11)),
            				
            				# insérer ici la valeur courante du mpoint d'indice annuel
            				# par exemple pour 56 € par point/an :
            				# rep.int(56, 12)
            				# si passage à 56.5 au 1er juillet:
            				# rep.int(56, 6)
            				# rep.int(56.5, 6)
            				
                    ncol = 12, 
            				byrow = TRUE)

limite_inm <- 2008 + nrow(PointIM)

if (an >= limite_inm) {
  
  cat("AVERTISSEMENT : Il faut actualiser la matrice PointIM du fichier point.im.R comme indiqué en commentaires en rajoutant la valeur annuelle du point d'indice net pour les années après", limite_inm)
  
  if (fin.période.sous.revue >= limite_inm) {
    
    cat("ERREUR : La période sous revue dépasse la valeur limite du tableau en référence du point d'indice net. Actualiser ce tableau (fichier point.im.R) et relancer", limite_inm - 1)
    if (fin.période.sous.revue > limite_inm) {
      cat("Il faut rajouter la période", limite_inm, " - ",  fin.période.sous.revue)
    } else {
      cat("Il faut rajouter l'année", limite_inm)
    }
    
    cat("Pour continuer à s'exécuter le programme va écarter les années ci-dessus mentionnées...")
    
    extraire.années <- TRUE
    fin.période.sous.revue <- limite_inm - 1
  }
}

PointMensuelIM <- PointIM / 12

PointMensuelIMMoyen <- apply(PointMensuelIM, 1, mean)

valeur.point.inm.pivot <- round(PointMensuelIMMoyen, 1)     

# année >= 2008 et année <= 2016

# pour avoir la valeur du point d'indice retrancher 2007 à l'année :
#   PointIM[année - 2007, mois]

# 1/01/2015	55.5635	
# 1/01/2014	55.5635	
# 1/01/2013	55.5635	
# 1/02/2012	55.5635	
# 1/07/2011	55.5635	
# 1/07/2010	55.5635	
# 1/10/2009	55.2871	
# 1/07/2009	55.1217	
# 1/10/2008	54.8475	
# 1/03/2008	54.6834	
# 1/02/2007	54.4113	
