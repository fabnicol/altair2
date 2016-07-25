
PointIM <- matrix(c(
                    54.4113, 54.4113,
                    rep.int(54.6834, 7),
                    rep.int(54.8475, 9),
                    rep.int(55.1217, 3),
                    rep.int(55.2871, 9),
                    rep.int(55.5635, 72),
            				rep.int(55.8969, 6)), 
                    ncol = 12, byrow = TRUE)

PointMensuelIM <- PointIM / 12

PointMensuelIMMoyen <- apply(PointMensuelIM, 1, mean)

# année >= 2008 et année <= 2015

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
