# En-tête du rapport
# Les caractéristiques du contrôle sont contenues dans controle[1], controle[2], controle[3], controle[4]
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
