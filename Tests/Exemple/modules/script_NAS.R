
#'
#'## `r chapitre`.5 Contrôle sur les logements par nécessité absolue de service (NAS)       
#'  
#'[![Notice](Notice.png)](Docs/Notices/fiche_NAS.odt)  
#'*Pour que le logiciel puisse contrôler sans risque d'erreur les concessions de logement, il est préférable de faire remplir, par les organismes contrôlés le tableau CSV accessible dans le bloc* **Logement** *de l'onglet Extra de l'application graphique, ou bien à ce lien. Voir aussi la notice* &nbsp; [![Notice](Notice.png)](Docs/Notices/fiche_tableau_logements.odt)      
#'   

#+ NAS

base.logements <- test_avn("NAS", Paie, logements = base.logements)  # base des avantages en nature déclarés en paye pour logt par NAS

#'   
#'   
#'### Contrôle sur les logements par NAS    
#'   
#'   
#'[Lien vers la base des logements par NAS non déclarés en base de paye](Bases/Reglementation/NAS.non.declares.paye.csv)    
#'[Lien vers la base des logements par NAS non importés dans le fichier auxiliaire](Bases/Reglementation/NAS.non.importes.csv)    
#'       
