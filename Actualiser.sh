#!/bin/bash
cd ~/Dev/altair
echo "Actualisation de la plateforme Altaïr..."
echo "Attention toutes les données personnelles du dossier altair seront effacées."
echo "Veuillez confirmer la poursuite de l'actualisation (Oui/Non)."
echo ""
echo "Oui/Non : "
read reponse
if test x$reponse = xOui; then
   
  git reset --hard HEAD
  git clean -dfx
  adresse=$(cat entrepot.txt)
  git remote remove origin
  git remote add -t master origin $adresse
  git pull -X theirs -s recursive origin master
  rm -rf altair altair.linux sft lhx Interface Docs Tests/Exemple/Donnees
  mkdir lhx
  git commit -m "Sauvegarde $(date)"

fi

echo "Fin de l'opération."
echo "Entrer une touche du clavier pour quitter la console..."
read reponse


  
