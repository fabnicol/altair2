"VERSANT_FP" %a% "FPT"
"codes.traitement" %a% NA
"codes.nbi"        %a% NA
"codes.pfi"        %a% NA
"codes.pfr"        %a% NA
"codes.psr"        %a% NA
"codes.ipf"        %a% NA
"codes.ifts"       %a% NA
"codes.iat"        %a% NA
"codes.ifse"       %a% NA
"codes.iemp"       %a% NA
"codes.iss"        %a% NA
"codes.ihts"       %a% NA
"codes.vacataires" %a% NA
"codes.astreintes" %a% NA
"codes.nas"        %a% NA
"codes.primespecifique" %a% NA  
"codes.ift"             %a% NA  
"codes.primedeservice"  %a% NA  
"codes.primedetechnicite" %a% NA

"script_effectifs" %a% TRUE    
"script_pyramides" %a% TRUE    
"script_duréedeservice" %a% TRUE
"script_rémunérationsbrutes" %a% TRUE
"script_comparaisonsdubrut" %a% TRUE    
"script_rémunérationsnettes" %a% TRUE    
"script_rmpp" %a% TRUE
"script_évolutiondunet" %a% TRUE
"script_comparaisonsdunet" %a% TRUE
"script_parité" %a% TRUE
"script_noria" %a% TRUE
"script_NBI" %a% TRUE    
"script_PFI" %a% TRUE    
"script_IATIFTS" %a% TRUE    
"script_PFR" %a% TRUE    
"script_PSR" %a% TRUE    
"script_IPF" %a% TRUE    
"script_RIFSEEP" %a% TRUE    
"script_HS" %a% TRUE    
"script_astreintes" %a% TRUE    
"script_élus" %a% TRUE    
"script_comptabilité" %a% TRUE    
"script_SFT" %a% TRUE    
"script_retraites" %a% TRUE    
"script_FPH" %a% TRUE    
"script_annexe" %a% TRUE


# White space indent

if (script_rémunérationsbrutes) {
    "script_rémunérationsbrutes1" %a% TRUE
    "script_rémunérationsbrutes2" %a% TRUE
} else {
   "script_rémunérationsbrutes1" %a% FALSE
   "script_rémunérationsbrutes2" %a% FALSE
}


"sequentiel" %a% FALSE
"ouvrir.document" %a% TRUE
"paye.budget.existe" %a% FALSE

"types.irs"  %a% c("IR", "S")

"controle" %a% c("CONSEIL DEPARTEMENTAL DES HAUTES-PYRENEES, ASE65","22650001500012 - 22650001500111 - 22650001500350","CONSEIL DEPARTEMENTAL - ASSISTANTE FAMILIALE - ELU - TRANSPORT","BUDGET PRINCIPAL, BUDGET TRANSPORT")  

"codes" %a% data.table(type = c("TRAITEMENT", "NBI", "PFI", "PFR", "PSR", "IPF", "IFTS", "IAT", "IFSE", "IEMP", "ISS", "IHTS", "VAC", "ASTREINTES", "NAS", "PRIME SPECIFIQUE", "IFT", "PRIME DE SERVICE", "PRIME DE TECHNICITE", "IR_S"),
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