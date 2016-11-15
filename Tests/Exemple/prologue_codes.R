codes.nbi <- NA
codes.pfi <- NA
codes.pfr <- NA
codes.ifts <- NA
codes.iat  <- NA
codes.ihts <- NA
codes.vacataires <-NA
codes.elus <- NA

codes <- data.table(valeur = c(codes.nbi, 
                               codes.pfi,
                               codes.pfr, 
                               codes.ifts, 
                               codes.iat,
                               codes.ihts, 
                               codes.vacataires,
                               codes.elus),
                               
                    expression = c(expression.rég.nbi,
                                   expression.rég.pfi,
                                   expression.rég.pfr, 
                                   expression.rég.ifts, 
                                   expression.rég.iat,
                                   expression.rég.heures.sup, 
                                   "",
                                   expression.rég.élus))
