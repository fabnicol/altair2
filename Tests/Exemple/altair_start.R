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
#'---    
#'title: false    
#'author: false     
#'header-includes: \usepackage{graphicx}  
#'date: false    
#'output:   
#'html_document:    
#'css: style.css
#'graphics: yes
#'---     
#'   
#'![Image_Altair](Altair.png)
#'   
#'   
#'## Logiciel Altaïr version `r readLines(file.path(currentDir, "VERSION"))`

# ---
# Encodage obligatoire en UTF-8
# ---

#+ echo = FALSE, warning = TRUE, message = FALSE

# comportement global du programme

# Lorsque l'on n'a que une ou deux années, mettre étudier.variations à FALSE
# Lorsque l'on n'étudie pas une base Xémélios, mettre étudier.tests.statutaires à FALSE

#+ début
  
# Utiliser la compilation JIT

library(compiler, warn.conflicts = FALSE)

invisible(setCompilerOptions(suppressAll = TRUE, optimize = 3))
invisible(enableJIT(3))

# Options générales

# Sourcer la biblio de fonctions auxiliaires
# Appel de la biblio altair, où sont regroupées des fonctions d'analyse des rémunérations et les pyramides

library(knitr, warn.conflicts = FALSE)
knitr::opts_chunk$set(fig.width = 7.5, echo = FALSE, warning = FALSE, message = FALSE, results = 'asis')

# Importer les données --> bases Paie et Bulletins.paie

importer()

# En-tête du rapport
# Les caractéristiques du contrôle sont contenues dans controle[1], controle[2], controle[3], controle[4]

#'
#'### Employeur : `r controle[1]`      
#'### Siret : `r controle[2]`   
#'### Etablissement : `r controle[3]`   
#'### Budget : `r controle[4]`      
#'   
#'En cas de dysfonctionnement logiciel, signaler les difficultés rencontrées à l'aide de la [notice jointe](Docs/MANTIS-suivi des bogues.pdf).    
#'Logiciel sous licence [CeCILL v.2.1](Docs/LICENCE.html)     
#+ echo = FALSE
#'`r format(Sys.Date(), "%a %d %b %Y")`      
#'      
#'Période sous revue : `r début.période.sous.revue` - `r fin.période.sous.revue`    
#'Nombre d'exercices : `r durée.sous.revue`        
#'   
#'   
#'**Avertissements**   
#'    
#'*1. La production des rapports d'analyse nécessite que les données de paye soient continues,
#'autrement dit qu'il n'y ait pas d'année ou de mois manquant dans la série de données
#'disponibles. Lorsque tel est le cas, il convient de réaliser autant de rapports partiels
#'que de séries partielles de données continues.*       
#'    
#'*2. Il est recommandé de renseigner, dans toute la mesure du possible, les codes de paye de l'onglet Codes de l'interface graphique*  &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_onglet_codes.odt)      

# Pour sauter une page en html (ou pdf converti de html, faire un h6 soit six dièses dans les Rmd seulement)  
#+ analyse-rémunérations

# Cette fonction permet de sauter une page dans le PDF ou dans le html (pas dans le docx)

newpage()

# Analyser les rémunérations à partir des données importées --> bases Analyse.XXX

source("analyse.rémunérations.R", encoding = encodage.code.source)

colonnes.sélectionnées <- c("traitement.indiciaire",
                            "acomptes",
                            "rémunération.indemnitaire.imposable",
                            "rémunération.indemnitaire.imposable.eqtp",
                            "total.lignes.paie",
                            "Montant.brut.annuel",
                            "Montant.brut.annuel.eqtp",
                            "part.rémunération.indemnitaire",
                            "quotité.moyenne",
                            "Statut",
                            "Grade",
                            "Catégorie",
                            "Filtre_actif",
                            "Filtre_annexe",
                            "Matricule")

"colonnes" %a%  c("Matricule",
                  "Nom",
                  "Prénom",
                  "Grade",
                  "Statut",
                  "Code",
                  "Libellé",
                  "Année",
                  "Mois",
                  "Montant")

##### Prologue indemnitaire #####  

# Description des données indemnitaires

Paie_I <- Paie[Type == "I" | Type == "A" | Type == "R", 
               .(Nom, 
                 Prénom,
                 Matricule, 
                 Année, 
                 Mois, 
                 Début,
                 Fin,
                 Code,
                 Libellé,
                 Montant,
                 Type,
                 Emploi,
                 Grade, 
                 Temps.de.travail,
                 Indice,
                 Statut,
                 Catégorie)]

prime_IAT <- list(nom = "IAT",                     # Nom en majuscules
                  catégorie = c("B", "C"),         # restreint aux catégories B et C
                  restreint_fonctionnaire = TRUE,  # fonctionnaires
                  dossier = "Reglementation")      # dossier de bases

prime_IFTS <- list(nom = "IFTS",                   # Nom en majuscules
                   catégorie = c("A", "B"),        # restreint aux catégories A et B
                   restreint_fonctionnaire = TRUE, # fonctionnaires
                   dossier = "Reglementation",     # dossier de bases  
                   NAS = "non",                    # logement par NAS
                   indice  = c("+", 350, "B"))     # supérieur à INM 350 pour catégorie B.

prime_PFR <- list(nom = "PFR",                     # Nom en majuscules
                  catégorie = "A",                 # restreint aux catégories A
                  restreint_fonctionnaire = TRUE,  # fonctionnaires
                  dossier = "Reglementation",      # dossier de bases
                  expr.rég = "")  

prime_PSR <- list(nom = "PSR",                     # Nom en majuscules
                  catégorie = c("A","B"),          # restreint aux catégories A et B
                  restreint_fonctionnaire = TRUE,  # fonctionnaires
                  dossier = "Reglementation",      # dossier de bases
                  expr.rég = ".*(?:ing|tech|d.*g.*s.*t|dessin|biol|phar).*")  # Contrainte sur le grade (expression régulière)

prime_IPF <- list(nom = "IPF",                     # Nom en majuscules
                  catégorie = "A",                 # restreint aux catégories A et B
                  restreint_fonctionnaire = TRUE,  # fonctionnaires
                  dossier = "Reglementation",      # dossier de bases
                  expr.rég = ".*(?:ing.*chef).*")  # Contrainte sur le grade (expression régulière)

prime_IFSE <- list(nom = "IFSE",                   # Nom en majuscules
                   restreint_fonctionnaire = TRUE, # fonctionnaires
                   catégorie = c("A", "B", "C"),   # toutes les catégories
                   dossier = "Reglementation")     # dossier de bases

prime_ISS <- list(nom = "ISS",                     # Nom en majuscules
                  catégorie = c("A", "B"),         # Techniciens A, B
                  restreint_fonctionnaire = TRUE,  # fonctionnaires
                  dossier = "Reglementation")      # dossier de bases

prime_IEMP <- list(nom = "IEMP",                   # Nom en majuscules
                   restreint_fonctionnaire = TRUE, # fonctionnaires
                   catégorie = c("A", "B", "C"),   # toutes les catégories
                   dossier = "Reglementation")     # dossier de bases

prime_PFI <- list(nom = "PFI",                      # Nom en majuscules
                  restreint_fonctionnaire = TRUE,   # fonctionnaires
                  catégorie = c("A", "B", "C"),     # toutes les catégories
                  dossier = "Reglementation")       # dossier de bases


scripts <- list(
  "script_effectifs.R",                   #### 1.1 Effectifs          ####
  "script_pyramides.R",                   #### 1.2-5 Pyramides        ####
  "script_durée.R",                       #### 1.6  Effectifs par durée ####
  list("analyse.statique.R",
    index = c(début.période.sous.revue,
              fin.période.sous.revue),
    incrémenter = TRUE),                  #### 2 et 3 Analyse statique des rémunérations ####
  "script_comparaisons_brut.R",           #### 3.4  Comparatif INSEE DGCL ####
  c("script_analyse_dynamique.R",         #### 4. Analyse dynamique des rémunérations ####
    incrémenter = TRUE),
  "script_NBI.R",                         #### 5.1  NBI               ####
  "script_PFI.R",                         #### 5.2  PFI               ####
  "script_vacations.R",                   #### 5.3-5.4 VACATIONS      ####
  "script_NAS.R",                         #### 5.5  NAS               ####
  "script_IAT_IFTS.R",                    #### 5.6  IAT/IFTS          ####  
  "script_PFR.R",                         #### 5.7  PFR               ####
  "script_PSR.R",                         #### 5.8  PSR               ####
  "script_IPF.R",                         #### 5.9  IPF               ####
  "script_RIFSEEP.R",                     #### 5.10 RIFSEEP           ####
  "script_HS.R",                          #### 5.11 HEURES SUP        ####
  "script_élus.R",                        #### 5.12 ELUS              ####
  "script_comptabilité.R",                #### 5.13 COMPTE DE GESTION ####
  "script_SFT.R",                         #### 5.14 SFT               ####
  "script_astreintes.R",                  #### 5.15 ASTREINTES        ####
  "script_retraites.R",                   #### 5.16 RETRAITES         ####
  "script_FPH.R",                         #### 5.17 PRIMES FPH        ####     
  "script_annexe.R"                       #### ANNEXE                 ####
) 

générer.partie <- function(script, type = séquentiel) invisible(lapply(script, function(x) do.call(insérer_script, 
                                                                                          as.list(na.omit(c(file.path(chemin.modules, x[1]),
                                                                                                                      x[-1]))))))
if (séquentiel) {
  
  générer.partie(scripts)
  
} else {
  
  group1 <- list("script_effectifs.R",               
                  "script_pyramides.R",               
                  "script_durée.R",                   
                  "analyse.statique.R",               
                  "script_comparaisons_brut.R")       
  
  group2 <- list("script_rémunérations_net.R",
                  "script_noria.R",                   
                  "script_analyse_dynamique.R")       
  
  group3 <- list("script_NBI.R",                     
                  "script_PFI.R",                     
                  "script_vacations.R",               
                  "script_NAS.R")                     
  
  group4 <- list("script_IAT_IFTS.R",                
                  "script_PFR.R",                     
                  "script_PSR.R",                     
                  "script_IPF.R",                     
                  "script_RIFSEEP.R")                 
  
  group5 <- list("script_HS.R",                      
                  "script_heures_sup.R",              
                  "script_astreintes.R",              
                  "script_élus.R")                    
  
  group6 <- list("script_comptabilité.R",            
                  "script_annexe.R",                  
                  "script_SFT.R",                     
                  "script_retraites.R",               
                  "script_FPH.R")
  library(parallel)
  
  cl <- makeCluster(6, type = "FORK")
  res <- clusterApply(cl, group %+% 1:6, générer.partie, type = ! séquentiel)
  stopCluster(cl)
}

######### SAUVEGARDES #######

# Attention, les noms des fichiers auxiliaires aux rapports doivent être sans accent (compatibilité Windows)

envir <- environment()

if (sauvegarder.bases.analyse) {

  sauv.bases(file.path(chemin.dossier.bases, "Remunerations"),
             env = envir,
             "Analyse.remunerations",
             "Anavar.synthese",
             "Analyse.variations.par.exercice",
             "beneficiaires.IPF.Variation")

  sauv.bases(file.path(chemin.dossier.bases, "Effectifs"),
             env = envir,
             "matricules",
             "grades.categories",
             "tableau.effectifs")

  sauv.bases(file.path(chemin.dossier.bases, "Reglementation"),
             env = envir,
             "personnels.ipf.ifts",
             "codes.ipf",
             "HS.sup.25",
             "Base.IHTS.non.tit",
             "lignes.IHTS",
             "lignes.IHTS.tot",
             "depassement.agent",
             "depassement.agent.annee",
             "Taux.horaires",
             "CumHS",
             "Depassement.seuil.180h",
             "Depassement.seuil.220h",
             "ihts.cat.A",
             "Controle_astreintes",
             "Controle_astreintes_HS_irreg",
             "Cum_astreintes_HS_irreg",
             "libelles.astreintes",
             "IPF.non.catA",
             "IPF.non.tit",
             "lignes.contractuels.et.vacations",
             "lignes.fonctionnaires.et.vacations",
             "Paie_vac_contr",
             "Paie_vac_fonct",
             "Paie_vac_sft_ir",
             "lignes.ifts.anormales",
             "matricules.contractuels.et.vacations",
             "matricules.fonctionnaires.et.vacations",
             "SFT_IR.et.vacations",
             "matricules.SFT_IR.et.vacations",
             "NBI.aux.non.titulaires",
             "NBI.cat.irreg",
             "personnels.prime.informatique",
             "remunerations.elu",
             "RI.et.vacations",
             "traitement.et.vacations",
             "cumul.lignes.paie",
             "cumul.total.lignes.paie",
             "controle.sft",
             "Paie.sans.enfant.reduit",
             "Cotisations.irreg",
             "Cotisations.irreg.ircantec",
             "personnels.ift.nt",
             "personnels.prime.specifique.nt",
             "personnels.prime.tech.nt",
             "personnels.ps.nt")
  
  sauv.bases(file.path(chemin.dossier.bases, "Fiabilite"),
             env = envir,
              "base.heures.nulles.salaire.nonnull",
              "base.quotite.indefinie.salaire.non.nul",
              "lignes.nbi.anormales",
              "lignes.nbi.anormales.hors.rappels",
              "lignes.nbi.anormales.mensuel",
              "lignes.paie.nbi.anormales.mensuel",
              "cumuls.nbi",
              "plusieurs_libelles_par_code",
              "plusieurs_codes_par_libelle",
              "plusieurs_types_par_code",
              "plusieurs_types_par_libelle",
              "code.libelle",
              "Evenements",
              "Evenements.ind",
              "Evenements.mat")
  
  if (test.delta) 
    sauv.bases(file.path(chemin.dossier.bases, "Fiabilite"), 
               env = envir, "Delta")
  
}

if (sauvegarder.bases.origine)
  sauv.bases(file.path(chemin.dossier.bases, "Paiements"),
             env = envir,
             "Paie",
             "Bulletins.paie")

if (profiler)
  sauv.bases(chemin.dossier.bases, 
            env = envir, "PROF")

# Conversion en Latin-1 des bases auxiliaires du rapport, pour une meilleure lecture sous Windows

system2("find", c("Donnees/R-Altair/Bases", "-name", "'*.csv'", "-exec", "iconv -f UTF-8 -t ISO-8859-15 -c -o {}.2  {} \\;", "-exec",  "mv {}.2 {} \\;"))

# Copie de la documentation accessoire aux rapports

if (! dir.exists(chemin.dossier.docs)) 
  dir.create(chemin.dossier.docs, recursive = TRUE, mode="0777")

res <- file.copy("Docs", chemin.clé, recursive=TRUE)

if (res) message("Dossier Docs copié dans", chemin.clé) else message("Dossier Docs n'a pas été copié dans", chemin.clé)

# Nettoyage

if (paye.budget.existe)        file.remove(chemin("paye_budget.csv"))
if (fichier.personnels.existe) file.remove(chemin("matricules.csv"))
if (grades.categories.existe)  file.remove(chemin("grades.categories.csv"))
if (logements.existe)          file.remove(chemin("logements.csv"))
if (plafonds.ifse.existe)      file.remove(chemin("plafonds_ifse.csv"))

setwd(currentDir)

message("Dossier courant : ", getwd())

if (! debug.code)
   rm(list = setdiff(ls(), script_env))
