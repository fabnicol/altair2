
#'
#'## 5.5 Contrôle sur les logements par nécessité absolue de service (NAS)       
#'  
#'[![Notice](Notice.png)](Docs/Notices/fiche_NAS.odt)  
#'*Pour que le logiciel puisse contrôler sans risque d'erreur les concessions de logement, il est nécessaire de faire remplir, par les organismes contrôlés le tableau CSV accessible dans le bloc* **Logement** *de l'onglet Extra de l'application graphique, ou bien à ce lien. Voir aussi la notice* &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_tableau_logements.odt)      
#'   

#+ NAS
NAS.non.declares.paye <- NULL
NAS.non.importes <- NULL

if (!is.null(base.logements)) test_avn("NAS", Paie, base.logements)  # base des avantages en nature déclarés en paye pour logt par NAS

#'   
#' **Contrôle sur les logements par NAS**       
#'   

if (is.null(NAS.non.declares.paye)) 
    cat("Pas de logement par NAS déclaré dans le fichier de l'onglet Extra mais non déclaré en base de paye   \n") else cat("Il existe des logements par NAS déclarés dans le fichier de l'onglet Extra mais non déclarés en base de paye  \n")
if (is.null(NAS.non.importes)) 
    cat("Pas de logement par NAS déclaré en base de paye mais non déclaré dans le fichier importé par l'onglet Extra   \n") else cat("Il existe des logements par NAS déclarés en base de paye mais non déclarés dans le fichier importé par l'onglet Extra   \n")

conditionnel("Lien vers la base des logements par NAS non déclarés en base de paye", "Bases/Reglementation/NAS.non.declares.paye.csv")    
conditionnel("Lien vers la base des logements par NAS non importés dans le fichier auxiliaire", "Bases/Reglementation/NAS.non.importes.csv")
conditionnel("Lien vers la base des logements par NAS fusionnée avec la base de paye", "Bases/Reglementation/logements.csv")    
#'       
