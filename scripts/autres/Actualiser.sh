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

  # Un peu de ménage

  find /home/fab/Dev/altair/.git -name '*.lock' -delete
  git checkout -f master-jf

  rm -f /home/fab/Dev/altair/sys/install.*

  
  for i in $(cut -d: -f1 /etc/passwd)
    do 
        if test -d /home/$i; then

             if test -d "/home/$i/Dev/altair/scripts/R/altair_files"; then
                rm -rf "/home/$i/Dev/altair/scripts/R/altair_files"
             fi
             if test -d "/home/$i/Dev/altair/scripts/R/Donnees/R-Altair"; then
                rm -rf "/home/$i/Dev/altair/scripts/R/Donnees/R-Altair"
             fi
             chgrp -R users /home/$i/.local/share/applications
        fi
    done
	
  chown -R fab /home/fab/Dev/altair
  
  # Vérification de la version 
  
  rm -f /home/Public/VERSION

  wget   https://gitlabjf.ccomptes.fr/fnicol/master-jf/raw/master-jf/VERSION -O /home/Public/VERSION    --no-check-certificate
  
  date >> /home/Public/version.log

  if  test ! -f /home/Public/VERSION -o ! -s /home/Public/VERSION
  then 
   clear
   echo "*** FICHIER DE VERSION NON TELECHARGE  ***" | tee -a /home/Public/version.log
   echo "***  VERIFIER LA CONNEXION RESEAU      ***" | tee -a /home/Public/version.log
   sleep 5
   return -1
  fi 
  
    version_current=$(cat /home/Public/VERSION)
    version=$(cat /home/fab/Dev/altair/VERSION)
    if test x$version = x$version_current
    then
      clear
      echo "*** Pas de mise à jour ***"  | tee -a /home/Public/version.log
      sleep 3
      return 1
    fi
    
      clear
      echo "**************************************"
      echo "*"                                                               
      echo "*  Actualisation de l'application Altaïr"  | tee -a /home/Public/version.log
      echo "*"
      echo "**************************************"
      echo
      echo "****"  
      echo "* Actualisation du dépôt fab..." | tee -a /home/Public/version.log
      echo "****"


      cd /home/fab/Dev/altair
      git reset --hard HEAD
      git clean -df

      chown -R fab .

      adresse=$(cat entrepot.txt)
  
      current_origin=$(git remote -v | grep origin | grep fetch | cut -f'2' | cut -f 1 -d' ')

      git config user.name "Fabrice Nicol"
      git config user.email "fabrice.nicol@crtc.ccomptes.fr"
      git config --unset http.proxy 
      git config --global --unset http.proxy
      git config http.sslVerify false
      git config credential.helper store
      
      if ! test x$current_origin = x$adresse
      then
        echo "****"
        echo "* Actualisation du dépôt $adresse (ancien $current_origin). Patientez..." | tee -a /home/Public/version.log
        echo "***"
        git remote remove origin
        git remote add -t master origin $adresse
       fi
  
      git fetch -p -n --depth=1 origin master-jf | tee -a /home/Public/version.log
        
        for i in A_FAIRE.txt    data             entrepot.txt  Interface_linux    lib           LICENSE       scripts       TESTS.txt  warning \
clonage        DEPENDANCES.txt  fwidgets_lib  Interface_windows  LICENCE       linux           STATISTIQUES  \ 
COPYRIGHT.txt  Docs             Interface     lhx                LICENCE.html  LISEZ-MOI.txt  renv     sys           VERSION 
        do
            rm -rf "$i"
            git checkout FETCH_HEAD -- "$i" 
        done
        
        
	if ! test -d  lhx
        then
            mkdir lhx    
        fi
	
	git add -f .
        git commit -am "Sauvegarde master-jf $(date)" | tee -a /home/Public/version.log

        if test -f sys/install.sys -a -f sys/sysinstall.sh; then

            /bin/bash sys/sysinstall.sh

            if test $? = 0; then
            echo "***"
                echo "* Actualisation système effectuée"  | tee -a /home/Public/version.log
                echo "***"
                sleep 3
            else
                echo "=> Erreur d'actualisation système"
            fi
        fi

        git commit -am "Sauvegarde release $(date)" | tee -a /home/Public/version.log

	        
        echo "* Actualisation de jf..." | tee -a /home/Public/version.log

        echo "****"
        
  
        chmod 0770 /home/jf/Dev/altair/scripts/R/*.*
        chmod 0770 -R /home/jf/Dev/altair/Interface_linux/gui/x64
        chmod 0770 /home/jf/Dev/altair/*.*
        chgrp -R users /home/jf/Dev/altair
         
        rm -f /home/jf/Dev/altair/.directory
        
        if ! test -d  /home/jf/Dev/altair/lhx
        then
            mkdir /home/jf/Dev/altair/lhx    
             chown jf    /home/jf/Dev/altair/lhx
             chmod -R 0777  /home/jf/Dev/altair/lhx
        fi
        
        cd /home/fab/Dev/altair
        git gc --prune=now

        chown -R fab .
        echo "Droit fab alignés"  | tee -a /home/Public/version.log
				
        echo "Actualisation des documents de référence sur le bureau"  | tee -a /home/Public/version.log

# Liste des utilisateurs

        for i in $(cut -d: -f1 /etc/passwd)
        do 
        if test -d /home/$i; then
             find "/home/$i/Desktop/" -name '*.pptx' -delete
	         find "/home/$i/Desktop/" -name '*.docx' -delete
             cp -f "/home/fab/Dev/altair/Docs/Guide utilisateurs.pptx" /home/$i/Desktop
	         cp -f "/home/fab/Dev/altair/Docs/Documentation du projet.docx" /home/$i/Desktop
             chown -R $i "/home/$i/Dev/altair/scripts/R/Donnees/xhl/$i"
             cp -rvf /home/Public/.rstudio-desktop /home/$i
             chown -R $i /home/$i/.rstudio-desktop
             chgrp -R users /home/$i/.rstudio-desktop
             chown -R $i /home/$i/.config
             chgrp -R users /home/$i/.config
             chown -R $i /home/$i/.local/share/applications
             chgrp -R users /home/$i/.local/share/applications
        fi
        done


chmod +x /home/fab/Dev/altair/scripts/autres/postinstall.sh
/bin/bash /home/fab/Dev/altair/scripts/autres/postinstall.sh 
echo "postinstall"| tee -a /home/Public/version.log

echo "****" 
echo "* Fin de l'opération."
echo "****"
echo "   "
echo "Entrer une touche du clavier pour quitter la console et éteindre le PC..."
read reponse

shutdown -h now

  
