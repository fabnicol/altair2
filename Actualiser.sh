#!/bin/bash
echo "Actualisation de la plateforme Altaïr..."
echo "Attention toutes les données personnelles du dossier altair seront effacées."
echo "Veuillez confirmer la poursuite de l'actualisation (Oui/Non)."
echo ""
echo "Oui/Non : "
read reponse
if test x$reponse = xOui
then
  
  echo "Actualisation du dépôt fab..."
  
  git reset --hard HEAD
  git clean -df
  adresse=$(cat entrepot.txt)
  
  current_origin=$(git remote -v | grep origin | grep fetch | cut -f'2' | cut -f 1 -d' ')
  
  if ! test x$current_origin = x$adresse
  then
    echo "****"
    echo "* Actualisation du dépôt $adresse (ancien $current_origin). Patientez..."
    echo "***"
    git remote remove origin
    git remote add -t master origin $adresse
  fi
  
  git fetch -p -n --depth=1 origin master-jf
  
  for i in Interface lhx Interface_linux linux '*.txt' '*.R' '*.sh' '*.desktop' VERSION LICENCE '*.ico' '*.bmp' '*.png'  postinstall.sh altaïr.Rproj 'Tests/Exemple' 'Tests/Exemple/Docs' 
  do
    git checkout FETCH_HEAD -- "$i" 
    git add .
  done
  if ! test -d  lhx
  then
     mkdir lhx    
  fi

  git commit -am "Sauvegarde $(date)"
  
  echo "Actualisation du dépôt jf..."
  
  cd /home/jf/Dev/altair
  
  if ! test x$current_origin = x$adresse
  then
    echo "****"
    echo "* Actualisation du dépôt $adresse (ancien $current_origin). Patientez..."
    echo "***"
    git remote remove origin
    git remote add -t master origin $adresse
  fi
  
  git fetch -p -n --depth=1 origin master-jf
  
  for i in Interface_linux linux '*.txt' '*.R' '*.sh' '*.desktop' VERSION LICENCE '*.ico' '*.bmp' '*.png'  postinstall.sh altaïr.Rproj 'Tests/Exemple' 'Tests/Exemple/Docs' 
  do
    git checkout FETCH_HEAD -- "$i" 
    git add .
  done
  if ! test -d  lhx
  then
     mkdir lhx    
  fi

  git commit -am "Sauvegarde $(date)"
  
  cd /home/fab/Dev/altair
  
fi
git gc --prune=now
echo "Fin de l'opération."
echo "Entrer une touche du clavier pour quitter la console..."
read reponse
./postinstall.sh


  
