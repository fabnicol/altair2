# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
# produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
# en vigueur à compter de l'année 2008.
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

codes.traitement <- NA
codes.nbi        <- NA
codes.pfi        <- NA
codes.pfr        <- NA
codes.psr        <- NA
codes.ipf        <- NA
codes.ifts       <- NA
codes.iat        <- NA
codes.ifse       <- NA
codes.iemp       <- NA
codes.iss        <- NA
codes.ihts       <- NA
codes.vacataires <- NA
codes.astreintes <- NA
codes.nas        <- NA
codes.primespecifique <- NA  
codes.ift             <- NA  
codes.primedeservice  <- NA  
codes.primedetechnicite <- NA  
types.irs  <- c("IR", "S")

controle<-c("YYY","YYY","YYY","MULTIBUDGET")  

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