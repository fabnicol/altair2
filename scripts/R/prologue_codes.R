"VERSANT_FP" %a% "FPT"
"codes.traitement" %a% c("010A","012A","011A","014A","010R","012R","010N","184N","186N","010D","012N","011N","014N","282N","281N","236N","283N","180N","285N","011D","256N","182N","284N","231N","288N","256A","282A","280A","281A","291A","283A","284A","288A")
"codes.nbi" %a% c("002A")
"codes.pfi" %a% c("536N")
"codes.pfr" %a% c("771N","772N","775N","776N","771A","772A","775A","776A")
"codes.psr" %a% c("530N","530A")
"codes.ipf" %a% c("773N","774N","773A","774A")
"codes.ifts"       %a% NA
"codes.iat"        %a% NA
"codes.ifse" %a% c("5000N","5007N","2010N","2000N","2020N","2007N","5015N","2050A","2018N","2050N","5000A","2000A","2010A","5007A","2007A","5033A","5015A","2018A")
"codes.iemp"       %a% NA
"codes.iss" %a% c("523N","559N","5012N","5011N","221N","224N","2011N","5014N","2012N","5013N","2014N","221A","504A","5012A","289A","306A","315A","307A","559A","313A","224A","289N","315N","307N","306N","313N")
"codes.ihts" %a% c("531N","531A","613A","601N","604N","603N","602N","607N","606N","611N","614N","613N","617N","612N","616N","601A","604A","603A","602A","607A","606A","611A","614A")
"codes.vacataires" %a% c("013N","612A")
"codes.astreintes" %a% c("578N","581N","584N","586N","583N","263N","264N","266N","202N","207N","265N","518N","218N","219N","574N","243N","219A","545N","608N","212N","202A","207A","230N","222N","217N","213A","578A","518A","574A","586A","583A","263A","264A","266A","582N","585N","213N","585A")
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

"controle" %a% c("CONSEIL DEPARTEMENTAL DES HAUTES-PYRENEES, ELU, ASE65, MAISON DEPT DES PERSONNES HANDICAPEES","22650001500350 - 22650001500012 - CONSEIL DEPARTEMENTAL DES HAUTES-PYRENEES - 22650001500111 - 13000029200016","TRANSPORT - CONSEIL DEPARTEMENTAL - 22650001500012 - ASSISTANTE FAMILIALE - M.D.P.H - ELU","BUDGET TRANSPORT, BUDGET PRINCIPAL, Budget MDPH, Multi budgets")  

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