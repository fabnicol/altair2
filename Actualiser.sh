#!/bin/bash
echo "Actualisation de la plateforme Altaïr..."
echo "Attention toutes les données personnelles du dossier altair seront effacées."
echo "Veuillez confirmer la poursuite de l'actualisation (Oui/Non)."
echo ""
echo "Oui/Non : "
read reponse
if test x$reponse = xOui
then
  echo "****"  
  echo "* Actualisation du dépôt fab..."
  echo "****"
  cd /home/fab/Dev/altair
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
  
  for i in altair.linux sft Docs Interface lhx Interface_linux linux '*.txt' '*.R' '*.sh' '*.desktop' VERSION LICENCE '*.ico' '*.bmp' '*.png'  postinstall.sh altaïr.Rproj 'Tests/Exemple' 'Tests/Exemple/Docs' sys
  do
    git checkout FETCH_HEAD -- "$i" 
    git add .
  done
  if ! test -d  lhx
  then
     mkdir lhx    
  fi

  if test -f sys/actualiser_sys -a -f sys/sysinstall.sh; then
    sudo --preserve-env /bin/bash sys/sysinstall.sh
    if test $? = 0; then
       echo "***"
       echo "* Actualisation système effectuée"
       echo "***"
       sleep 3
       git rm -f sys/actualiser_sys
    else
       echo "=> Erreur d'actualisation système"
    fi
  fi

  git commit -am "Sauvegarde $(date)"
  
  echo "****"
  echo "* Actualisation du dépôt jf..."
  echo "****"
  cd /home/jf/Dev/altair
  
  sudo chown -R fab /home/jf/Dev/altair/.git
  
  if ! test x$current_origin = x$adresse
  then
    echo "****"
    echo "* Actualisation du dépôt $adresse (ancien $current_origin). Patientez..."
    echo "***"
    git remote remove origin
    git remote add -t master origin $adresse
  fi
  
  git fetch -p -n --depth=1 origin master-jf
  
  for i in altair.linux sft Docs Interface_linux linux '*.txt' '*.R' '*.sh' '*.desktop' VERSION LICENCE '*.ico' '*.bmp' '*.png'  postinstall.sh altaïr.Rproj 'Tests/Exemple/*' 'Tests/Exemple/Docs' 
  do
    git checkout FETCH_HEAD -- "$i" 
    sudo chmod 0770 /home/jf/Dev/altair/Tests/Exemple/*.*
    sudo chmod 0770 /home/jf/Dev/altair/*.*
    git add .
  done
  if ! test -d  lhx
  then
     mkdir lhx    
  fi

  git commit -am "Sauvegarde $(date)"
  
  git gc --prune=now
  
  sudo chown -R jf /home/jf/Dev/altair/.git
  
  cd /home/fab/Dev/altair
  git gc --prune=now
fi


echo "****" 
echo "* Fin de l'opération."
echo "****"
echo "   "
echo "Entrer une touche du clavier pour quitter la console..."
read reponse
./postinstall.sh


  
