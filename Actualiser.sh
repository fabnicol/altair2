#!/bin/bash
echo "Actualisation de la plateforme Altaïr..."
echo "Attention toutes les données personnelles du dossier altair seront effacées."
echo "Veuillez confirmer la poursuite de l'actualisation (Oui/Non)."
echo ""
echo "Oui/Non : "

read reponse

if test x$reponse != xOui -a x$reponse != xoui
then 
  echo "*** L'actualisation a été annulée ***"
  exit 0 
fi  

# Housecleaning

if test -f  /home/fab/Dev/altair/.git/shallow.lock
  then
    echo "Un bloqueur de dépôt git a été détecté (shallow.lock)"
    echo "Effacement de ce bloqueur..."
    rm -f /home/fab/Dev/altair/.git/shallow.lock
fi 

echo "****"  
echo "* Actualisation du dépôt fab..."
echo "****"
cd /home/fab/Dev/altair

find .git -name '*.lock' -delete
git checkout -f newdev
git reset --hard HEAD
git clean -df
adresse=$(cat entrepot.txt)

current_origin=$(git remote -v | grep origin | grep fetch | cut -f'2' | cut -f 1 -d' ')

git config user.name "Fabrice Nicol"
git config user.email "fabrice.nicol@crtc.ccomptes.fr"
git config --unset http.proxy 
git config --unset https.proxy
git config http.sslVerify false
git config https.sslVerify false
git config credential.helper store

git config --global user.name "Fabrice Nicol"
git config --global user.email "fabrice.nicol@crtc.ccomptes.fr"
git config --global --unset http.proxy 
git config --global --unset https.proxy
git config --global http.sslVerify false
git config --global https.sslVerify false
git config --global credential.helper store

if ! test x$current_origin = x$adresse
then
echo "****"
echo "* Actualisation du dépôt $adresse (ancien $current_origin). Patientez..."
echo "***"
git remote remove origin
git remote add -t master origin $adresse
fi

# Remettre à zéro les indicateurs spécifiques d'actualisation

rm -f sys/build.*
rm -f sys/install.*

git fetch -p -n --depth=1 origin newdev

for i in altair.linux sft data Docs Interface fwidgets_lib lhx Interface_linux linux '*.txt' '*.R' '*.sh' '*.desktop' VERSION LICENCE '*.ico' '*.bmp' '*.png'  postinstall.sh altaïr.Rproj 'Tests/Exemple' 'Tests/Exemple/Docs' sys
do
rm -rf "$i"
git checkout FETCH_HEAD -- "$i" 
done

if ! test -d  lhx
then
 mkdir lhx    
fi

git add -f .

git commit -am "Sauvegarde newdev $(date)"

if test -f sys/install.sys -a -f sys/sysinstall.sh; then

sudo --preserve-env /bin/bash sys/sysinstall.sh

if test $? = 0; then
   echo "***"
   echo "* Actualisation système effectuée"
   echo "***"
   sleep 3
else
   echo "=> Erreur d'actualisation système"
fi
fi

git commit -am "Sauvegarde release $(date)"

echo "****"
echo "* Actualisation de jf..."
echo "****"
  
sudo cp -rvf "/home/fab/Dev/altair" /home/jf/Dev
sudo chown -R jf "/home/jf/Dev/altair/$i"


sudo chmod 0770 /home/jf/Dev/altair/Tests/Exemple/*.*
sudo chmod 0770 -R /home/jf/Dev/altair/Interface_linux/gui/x64
sudo chmod 0770 /home/jf/Dev/altair/*.*
sudo chgrp -R users /home/jf/Dev/altair
rm -f /home/jf/Dev/altair/.directory

if ! test -d  /home/jf/Dev/altair/lhx
then
 mkdir /home/jf/Dev/altair/lhx    
 sudo chown jf    /home/jf/Dev/altair/lhx
 sudo chmod -R 0777  /home/jf/Dev/altair/lhx
fi

cd /home/fab/Dev/altair
git gc --prune=now

echo "Actualisation des documents de référence sur le bureau"

# Liste des utilisateurs

for i in $(cut -d: -f1 /etc/passwd)
do 
   if test -d /home/$i; then
     sudo find "/home/$i/Desktop/" -name '*.pptx' -delete
	 sudo find "/home/$i/Desktop/" -name '*.docx' -delete
     sudo cp -f "/home/fab/Dev/altair/Docs/Guide utilisateurs.pptx" /home/$i/Desktop
	 sudo cp -f "/home/fab/Dev/altair/Docs/Documentation du projet.docx" /home/$i/Desktop
     sudo chown -vR $i "/home/$i/Dev/altair/Tests/Exemple/Donnees/xhl/$i"
     sudo cp -rvf /home/Public/.rstudio-desktop /home/$i
     sudo chown -R $i /home/$i/.rstudio-desktop
	 sudo chgrp -R users /home/$i/.rstudio-desktop
	 sudo chown -R $i /home/$i/.config
	 sudo chgrp -R users /home/$i/.config
	 sudo chown -R $i /home/$i/.local/share/applications
	 sudo chgrp -R users /home/$i/.local/share/applications
   fi
done

echo "****" 
echo "* Fin de l'opération."
echo "****"
echo "   "
echo "Entrer une touche du clavier pour quitter la console..."
read reponse
./postinstall.sh


  
