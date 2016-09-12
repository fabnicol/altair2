Altaïr (version de développement)
======
 
 Analyse des lignes de traitement, des attributions indemnitaires et autres rémunérations.
 
 Logiciel de traitement statistique des bases de données de paie générées par le logiciel
 Xémélios entretenu par la DGFIP (direction générale des finances publiques du ministère
 des finances français: http://xemelios.org). 
 
 Licence GPL v3 jointe, en anglais et en français.
 

dépendances
===========

 L'interface graphique et l'extracteur de données de paye XML en fichiers CSV sont écrits
 en langage C++. L'analyse des bases de données CSV est réalisée par une collection de scripts
 en langage R, avec notamment pour dépendances internes les paquets inclus sous R/library
 pour la version Windows (les dépendances sont générées différemment sous linux).
    
 Les paquets système suivants sont notamment requis :
    gcc/g++ et suite de compilation, minimum 5.1 (actuellement 6.1)
    Qt5 (actuellement 5.7)

Ces paquets sont disponibles dans les autres dépôts du groupe Altaïr, en téléchargement ouvert.

