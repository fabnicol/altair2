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
  git clean -df
  adresse=$(cat entrepot.txt)
  git remote remove origin
  git remote add -t master origin $adresse
  git fetch -p -n --depth=1 origin master
  for i in R Interface_linux linux '*.txt' '*.R' '*.sh' '*.desktop' VERSION LICENCE '*.ico' '*.bmp' '*.png' .Rproj.user 'Tests/Exemple/*' 'Tests/Exemple/Docs' 
  do
    git checkout FETCH_HEAD -- $i 
  done
  if ! test -d  lhx
  then
     mkdir lhx    
  fi

  git commit -am "Sauvegarde $(date)"

fi

echo "Fin de l'opération."
echo "Entrer une touche du clavier pour quitter la console..."
read reponse


  
