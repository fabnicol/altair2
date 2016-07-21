#!/bin/bash
cd ~/Dev/altair

echo "Attention toutes les données récentes du dossier altair seront effacées."
echo "Veuillez confirmer la poursuite de l'actualisation (Oui/Non)."
echo ""
echo "Oui/Non : "
read reponse
if test x$reponse = xOui; then
  
   echo "Lancement de la récupération..."
   git reset --hard HEAD
     
fi
echo "Opération terminée."
echo "Taper une touche pour quitter la console."
read key
