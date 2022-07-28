# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, annees 2012 à 2017
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
#'![](icones/altair.png)
#'   
#'   
#'## Logiciel Altaïr `r readLines(file.path(currentDir, "VERSION"))`

# ---
# Encodage obligatoire en UTF-8
# ---

#+ echo = FALSE, warning = FALSE, message = FALSE

# comportement global du programme

# Lorsque l'on n'a que une ou deux annees, mettre étudier.variations à FALSE
# Lorsque l'on n'étudie pas une base Xémélios, mettre étudier.tests.statutaires à FALSE

#+ début

# Importer les données --> bases Paie et Bulletins.paie

importer()
#+ fin_import

message("Importation effectuée...")
# En-tête du rapport
# Les caractéristiques du contrôle sont contenues dans controle[1], controle[2], controle[3], controle[4]
#+ entete

#'
#'### Employeur : `r controle[1]`      
#'### Siret : `r controle[2]`   
#'### Etablissement : `r controle[3]`   
#'### Budget : `r controle[4]`      
cat("**Période sous revue : ", debut.periode.sous.revue, " - ", fin.periode.sous.revue,"**  \n")
cat("**Nombre d'exercices : ", duree.sous.revue, "**  \n")        
#+ licence

#'   
#'   
#'Logiciel sous licence [CeCILL v.2.1](Docs/LICENCE.html)     
#'     
#'`r format(Sys.Date(), "%a %d %b %Y")`      
#'    
#'   
#'   
#'**Avertissements**   
#'    
#'*1. La production des rapports d'analyse nécessite que les données de paye soient continues,
#'autrement dit qu'il n'y ait pas d'année ou de mois manquant dans la série de données
#'disponibles. Lorsque tel est le cas, il convient de réaliser autant de rapports partiels
#'que de séries partielles de données continues.*       
#'    
#'*2. Il est recommandé de renseigner, dans toute la mesure du possible, les codes de paye de l'onglet Codes de l'interface graphique*  &nbsp; [![Notice](icones/Notice.png)](Docs/Notices/fiche_onglet_codes.odt)      
#'   
#'En cas de dysfonctionnement logiciel, veuillez bien signaler les difficultés rencontrées au développeur.    

# Pour sauter une page en html (ou pdf converti de html, faire un h6 soit six dièses dans les Rmd seulement)  
#+ analyse-rémunérations

message("Analyse des rémunérations...")

# Analyser les rémunérations à partir des données importees --> bases Analyse.XXX

source("analyse.rémunérations.R", encoding =  "UTF-8")

colonnes.selectionnees <<- c("traitement.indiciaire",
                            "acomptes",
                            "rémunération.indemnitaire.imposable",
                            "rémunération.indemnitaire.imposable.eqtp",
                            "total.lignes.paie",
                            "Montant.brut.annuel",
                            "Montant.brut.annuel.eqtp",
                            "part.rémunération.indemnitaire",
                            "quotite.moyenne",
                            "Statut",
                            "Grade",
                            "Categorie",
                            "Filtre_actif",
                            "Filtre_annexe",
                            "Matricule")

"colonnes" %a%  c("Matricule",
                  "Nom",
                  "Prenom",
                  "Grade",
                  "Statut",
                  "Code",
                  "Libelle",
                  "Annee",
                  "Mois",
                  "Montant")

##### Prologue indemnitaire #####  

# Description des données indemnitaires

Paie_I <<- Paie[Type == "I" | Type == "A" | Type == "R", 
               .(Nom, 
                 Prenom,
                 Matricule, 
                 Annee, 
                 Mois, 
                 Debut,
                 Fin,
                 Code,
                 Libelle,
                 Montant,
                 Type,
                 Emploi,
                 Grade, 
                 Temps.de.travail,
                 Indice,
                 Statut,
                 Categorie)]

prime_IAT <<- list(nom = "IAT",                     # Nom en majuscules
                  categorie = c("B", "C"),         # restreint aux categories B et C
                  restreint_fonctionnaire = TRUE,  # fonctionnaires
                  expr.reg = NULL,
                  dossier = "Reglementation")      # dossier de bases

prime_IFTS <<- list(nom = "IFTS",                   # Nom en majuscules
                   categorie = c("A", "B"),        # restreint aux categories A et B
                   restreint_fonctionnaire = TRUE, # fonctionnaires
                   dossier = "Reglementation",     # dossier de bases  
                   NAS = "non",                    # logement par NAS
                   expr.reg = NULL,
                   indice  = c("+", 350, "B"))     # supérieur à INM 350 pour categorie B.

prime_PFR <<- list(nom = "PFR",                     # Nom en majuscules
                  categorie = "A",                 # restreint aux categories A
                  restreint_fonctionnaire = TRUE,  # fonctionnaires
                  dossier = "Reglementation",      # dossier de bases
                  expr.reg = "")  

prime_PSR <<- list(nom = "PSR",                     # Nom en majuscules
                  categorie = c("A","B"),          # restreint aux categories A et B
                  restreint_fonctionnaire = TRUE,  # fonctionnaires
                  dossier = "Reglementation",      # dossier de bases
                  expr.reg = ".*(?:ing|tech|d.*g.*s.*t|dessin|biol|phar).*")  # Contrainte sur le grade (expression régulière)

prime_IPF <<- list(nom = "IPF",                     # Nom en majuscules
                  categorie = "A",                 # restreint aux categories A et B
                  restreint_fonctionnaire = TRUE,  # fonctionnaires
                  dossier = "Reglementation",      # dossier de bases
                  expr.reg = ".*(?:ing.*chef).*")  # Contrainte sur le grade (expression régulière)

prime_IFSE <<- list(nom = "IFSE",                   # Nom en majuscules
                   restreint_fonctionnaire = TRUE, # fonctionnaires
                   categorie = c("A", "B", "C"),   # toutes les categories
                   expr.reg = NULL,                   
                   dossier = "Reglementation")     # dossier de bases

prime_ISS <<- list(nom = "ISS",                     # Nom en majuscules
                  categorie = c("A", "B"),         # Techniciens A, B
                  restreint_fonctionnaire = TRUE,  # fonctionnaires
                  expr.reg = NULL,                  
                  dossier = "Reglementation")      # dossier de bases

prime_IEMP <<- list(nom = "IEMP",                   # Nom en majuscules
                   restreint_fonctionnaire = TRUE, # fonctionnaires
                   categorie = c("A", "B", "C"),   # toutes les categories
                   expr.reg = NULL,                   
                   dossier = "Reglementation")     # dossier de bases

prime_PFI <<- list(nom = "PFI",                      # Nom en majuscules
                  restreint_fonctionnaire = TRUE,   # fonctionnaires
                  categorie = c("A", "B", "C"),     # toutes les categories
                  expr.reg = NULL,                  
                  dossier = "Reglementation")       # dossier de bases
#immature

if (!setOSWindows) {
  system('cat /proc/meminfo > $HOME/mem.txt')
  mem <- data.table::fread(file.path(Sys.getenv("HOME"), "mem.txt"),
               sep = ":",
               header = FALSE)[V1 == "MemAvailable", V2]
  mem <- strtoi(unlist(strsplit(mem, " "))[1])
  if (nrow(Paie) * ratio.memoire.ligne.parallele  > mem) {
    "sequentiel" %a% TRUE  # assignation globale nécessaire
    message("Bascule en mode synchrone")
  }
}


options(encoding="UTF-8")
  
opts_knit$set(output.dir=getwd())

generer.partie <- function(script, sequentiel = FALSE) {

                              invisible(lapply(script, function(x) do.call(inserer_script, 
                                                                             as.list(na.omit(c(file.path(chemin.modules, x[1]),
                                                                                             x[-1], clean =  ! keep_md)))))) 
}
                            
if (sequentiel || setOSWindows) {

scripts <-
   list("script_effectifs.R",
        "script_pyramides.R",
        "script_duréedeservice.R",
        "script_rémunérationsbrutes1.R",
        "script_rémunérationsbrutes2.R",
        "script_comparaisonsdubrut.R",
        "script_évolutiondunet.R",
        "script_noria.R",
        "script_comparaisonsdunet.R",
        "script_parité.R",
        "script_NBI.R",
        "script_HS.R", #+
        "script_astreintes.R",
        "script_IATIFTS.R",
        "script_PFR.R",
        "script_PSR.R",
        "script_IPF.R",
        "script_RIFSEEP.R",
        "script_PFI.R", #+
        "script_élus.R",
        "script_comptabilité.R", #+
        "script_SFT.R",
        "script_retraites.R",
        "script_FPH.R",
        "script_annexe.R")

  generer.partie(scripts, sequentiel)
  
} else {
  
  
  # Les groupes sont constitutés pour :
  # a) équilibrer les charges des noeuds
  # b) tenir compte des relations de dépendances entre scripts afin d'éviter les files d'attentes et les mutex
  
  # Les indices donnent l'ordre de numérotation des fichiers  scripts/modulesXX.Rmd, cela permet de réordonner.
  
  group1 <- list(list("script_effectifs.R", "a"),
                 list("script_pyramides.R", "b"),
                 list("script_duréedeservice.R", "c"))

  group2 <- list(list("script_rémunérationsbrutes1.R", "d"),
                 list("script_rémunérationsbrutes2.R", "e"),
                 list("script_comparaisonsdubrut.R", "f"),
                 list("script_évolutiondunet.R", "g"),
                 list("script_noria.R", "h"),
                 list("script_comparaisonsdunet.R", "i"),
                 list("script_parité.R", "j"))

  group3 <- list(list("script_NBI.R", "k"),
                 list("script_HS.R", "l"),
                 list("script_astreintes.R", "m"))

  group4 <- list(list("script_IATIFTS.R", "n"),
                 list("script_PFR.R", "o"),
                 list("script_PSR.R", "p"),
                 list("script_IPF.R", "q"),
                 list("script_RIFSEEP.R", "r"))

  group5 <- list(list("script_PFI.R", "s"),
                 list("script_élus.R", "t"))

  group6 <- list(list("script_comptabilité.R", "u"),
                 list("script_SFT.R", "v"),
                 list("script_retraites.R", "w"),
                 list("script_FPH.R", "x"),
                 list("script_annexe.R", "y"))
  
  library(parallel)
  
  G <- list(group1,
            group2,
            group3, 
            group4,
            group5,
            group6)
  
  
  cluster_mode <-"FORK"
  
  cl <- makeCluster(6, type = cluster_mode)
  clusterApply(cl,
                      G,
                      generer.partie)
  
  stopCluster(cl)
  
}

######### SAUVEGARDES #######

# Attention, les noms des fichiers auxiliaires aux rapports doivent être sans accent (compatibilité Windows)

message("Enregistrement de la pile des bases...")

envir <- environment()

if (sauvegarder.bases.origine)
  sauv.bases(".",
             env = envir,
             "Paie",
             "Bulletins.paie")

if (profiler)
  sauv.bases(".", 
            env = envir, "PROF")

# Copie de la documentation accessoire aux rapports

if (! dir.exists(chemin.dossier.docs)) 
  dir.create(chemin.dossier.docs, recursive = TRUE, mode="0777")

res <- file.copy("Docs", chemin.cle, recursive=TRUE)

if (res) message("Dossier Docs copié dans", chemin.cle) else message("Dossier Docs n'a pas été copié dans", chemin.cle)

# Nettoyage

if (paye.budget.existe)        file.remove(chemin("paye_budget.csv"))
if (fichier.personnels.existe) file.remove(chemin("matricules.csv"))
if (grades.categories.existe)  file.remove(chemin("grades.categories.csv"))
if (logements.existe)          file.remove(chemin("logements.csv"))
if (plafonds.ifse.existe)      file.remove(chemin("plafonds_ifse.csv"))
