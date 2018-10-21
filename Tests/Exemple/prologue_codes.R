# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser
# les fichiers de paye produits au format spécifié par l'annexe de la  
# convention-cadre de dématérialisation en vigueur à partir de 2008.
# 
# Ce logiciel est régi par la licence CeCILL soumise au droit français et
# respectant les principes de diffusion des logiciels libres. Vous pouvez
# utiliser, modifier et/ou redistribuer ce programme sous les conditions
# de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
# sur le site "http://www.cecill.info".
# 
# En contrepartie de l'accessibilité au code source et des droits de copie,
# de modification et de redistribution accordés par cette licence, il n'est
# offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
# seule une responsabilité restreinte pèse sur l'auteur du programme, le
# titulaire des droits patrimoniaux et les concédants successifs.
# 
# A cet égard l'attention de l'utilisateur est attirée sur les risques
# associés au chargement, à l'utilisation, à la modification et/ou au
# développement et à la reproduction du logiciel par l'utilisateur étant
# donné sa spécificité de logiciel libre, qui peut le rendre complexe à
# manipuler et qui le réserve donc à des développeurs et des professionnels
# avertis possédant des connaissances informatiques approfondies. Les
# utilisateurs sont donc invités à charger et tester l'adéquation du
# logiciel à leurs besoins dans des conditions permettant d'assurer la
# sécurité de leurs systèmes et ou de leurs données et, plus généralement,
# à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
# 
# Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
# pris connaissance de la licence CeCILL, et que vous en avez accepté les
# termes.
# 
# 

codes.traitement <- c("800.10","010A","010E","011A","011E","011R","300.00 R","314.00 R","315.00 R","010N","011N","010D","011D","075N","315.00","300.00","314.00")
codes.nbi <- c("301.00","301.00 R")
codes.pfi <- c("295N","595N","595A","595R")
codes.pfr <- c("583N","636.01","636.02","636.03","636.01 R","636.02 R","283A","583A","636.03 R")
codes.psr <- c("534.00","534.01","587N","287N","534.00 R","587")
codes.ipf        <- NA
codes.ifts <- c("452.00","596N","451.00","296A","296R","451.00 R","452.00 R","596A","596R")
codes.iat <- c("289N","452.50","589N","289A","289R","452.50 R","589A","589R")
codes.ifse       <- NA
codes.iemp <- c("215N","515N","215A","515A","489.50","489.50 R","489.50M")
codes.iss <- c("581N","459.00","590N","593N","623.60")
codes.ihts <- c("401.00","403.10","402.00","404.10","508N","401.00 R","402.00 R","403.10 R","403.20 R","404.10 R","421.00 R","422.00 R","508A","635N","636N","612A","612N","612R","613N","614A","614N","614R","611A","611N","611R","601A","602A","603A","604A","404.00 R","404.20 R")
codes.vacataires <- c("263N","351.00 R","352.00 R","351.10 R","351.20 R","351.00","352.00")
codes.astreintes <- c("650N","479.18 R","479.20 R","479.21 R","479.23 R")
codes.nas        <- NA
codes.primespecifique <- NA  
codes.ift             <- NA  
codes.primedeservice  <- NA  
codes.primedetechnicite <- NA

script_effectifs               <- TRUE    
script_pyramides               <- TRUE    
script_duréedeservice          <- TRUE    
script_rémunérationsbrutes     <- TRUE    
script_comparaisonsdubrut      <- TRUE    
script_rémunérationsnettes     <- TRUE    
script_rmppetnoria             <- TRUE    
script_évolutiondunet          <- TRUE    
script_NBI                     <- TRUE    
script_PFI                     <- TRUE    
script_vacataires              <- TRUE    
script_NAS                     <- TRUE    
script_IATIFTS                 <- TRUE    
script_PFR                     <- TRUE    
script_PSR                     <- TRUE    
script_IPF                     <- TRUE    
script_RIFSEEP                 <- TRUE    
script_HS                      <- TRUE    
script_astreintes              <- TRUE    
script_élus                    <- TRUE    
script_comptabilité            <- TRUE    
script_annexe                  <- TRUE    
script_SFT                     <- TRUE    
script_retraites               <- TRUE    
script_FPH                     <- TRUE    

séquentiel                     <- TRUE
ouvrir.document                <- TRUE

types.irs  <- c("IR", "S")

controle<-c("REGION OCCITANIE, REGION LRMP, REGION LANGUEDOC ROUSSILLON - ELUS, REGION MIDI-PYRENEES, REGION LRMP - ELUS, REGION LANGUEDOC ROUSSILLON, REGION OCCITANIE - ELUS","20005379100048 - 23340001900029 - 20005379100014 - 23310001500012","REGION OCCITANIE - REGION LRMP - Cons.Reg. Languedoc Roussillon - Site de TOULOUSE - REGION MIDI-PYRENEES - REGION LRMP CESER - REGION LANGUEDOC ROUSSILLON - REGION LRMP CR - REGION OCCITANIE CR - REGION OCCITANIE CESER","Multi budgets, BUDGET ANNEXE, BUDGET PRINCIPAL")  

codes <- data.table(type = c("TRAITEMENT", "NBI", "PFI", "PFR", "PSR", "IPF", "IFTS", "IAT", "IFSE", "IEMP", "ISS", "IHTS", "VAC", "ASTREINTES", "NAS", "PRIME SPECIFIQUE", "IFT", "PRIME DE SERVICE", "PRIME DE TECHNICITE", "IR_S"),
                    valeur = list(
                               codes.traitement,
                               codes.nbi, 
                               codes.pfi,
                               codes.pfr, 
                               codes.psr, 
                               codes.ipf,
                               codes.ifts, 
                               codes.iat,
                               codes.ifse,
                               codes.iemp,
                               codes.iss,
                               codes.ihts, 
                               codes.vacataires,
                               codes.astreintes,
                               codes.nas,
                               codes.primespecifique,
                               codes.ift,
                               codes.primedeservice,
                               codes.primedetechnicite,
                               types.irs),
                               
                    expression = c(expression.rég.traitement,
                                   expression.rég.nbi,
                                   expression.rég.pfi,
                                   expression.rég.pfr, 
                                   expression.rég.psr, 
                                   expression.rég.ipf, 
                                   expression.rég.ifts, 
                                   expression.rég.iat,
                                   expression.rég.ifse,
                                   expression.rég.iemp,
                                   expression.rég.iss,
                                   expression.rég.heures.sup, 
                                   expression.rég.vacataires,
                                   expression.rég.astreintes,
                                   expression.rég.nas,
                                   expression.rég.primespecifique,
                                   expression.rég.ift,
                                   expression.rég.primedeservice,
                                   expression.rég.primedetechnicite,
                                   ""))


setkey(codes, type)