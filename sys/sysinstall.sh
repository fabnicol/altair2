function _copy () {

if test -f "$1"; then
  cp -fv "$1" "$2"
else
  if test -d "$1"; then
    cp -rvf "$1"  "$2"
  fi
fi  
  
}

chown -R fab .

      

# DEPRECATED
#
if test -f sys/install.modules -a $(uname -r | cut -d '.' -f 2) = 4; then

   echo "Actualisation des modules du T3..."
   echo "Importation de la branche T3"
   git fetch -p -n  --depth=1 origin T3
   git checkout FETCH_HEAD -- sys/modules
   git checkout FETCH_HEAD -- sys/modprobe.d
   
   if test -d sys/modules -a -d sys/modprobe.d; then
   
      echo "Modules importés de la branche T3"
      _copy sys/modules /lib
      _copy sys/modprobe.d /lib
      chown -R root /lib/modules
      chown -R root /lib/modprobe.d
      echo "Actualisation des modules du T3 terminée..."
      
   else
   
      echo "Echec à l'importation des modules de la branche T3"
      
    fi 
fi

# Toute l'actualisation étant en mode git checkout et non pas en git pull
# il faut effacer les résidus qui posent problème


if test -f sys/install.data; then
  git checkout FETCH_HEAD -- data
  cp -rf data /home/jf/Dev/altair
  rm -f sys/Rlibrary/altair/data/*
  rm -f altair.linux/data/*
  rm -f altair/data/*
fi  

# obsolète
# sed -i 's/ALL ALL=(ALL) ALL/#ALL ALL=(ALL) ALL/' /etc/sudoers

if test -f /usr/local/lib64/R/bin/R; then
 R_version=$(/usr/local/lib64/R/bin/R --version | grep "R version" | cut -f 3 -d' ') 
else
 R_version=""
fi

mkdir -p /usr/local/lib64/R/library/

cd /home/fab/Dev/altair/sys
chmod -R +rwx *sh

# recompilation de la bibliothèque altair
if test -f install.Rlibrary; then

  cp -rf Rlibrary/*  /usr/local/lib64/R/library/ 
  echo "*************************************"
  echo "*                                   *"
  echo "* Nouvelle bibliothèque R installée *"
  echo "*                                   *"
  echo "*************************************"
  sleep 2
fi  

cd ..

# Pour recompiler et réinstaller R : insérer R_VERSION avec un nouveau numéro de version
# ou bien un fichier install.R.force
# insérer le fichier install.R 
# insérer un paquet R.tar.xz dans build/


if test -f sys/install.R; then

  
   if test -f sys/install.R.force -o x$R_version != x$(cat sys/R_VERSION) ; then

     echo "Actualisation de R par compilation..."
       
     if test -d sys/build; then

         emerge --unmerge dev-lang/R  
         cd sys/build
         tar xJf R.tar.xz
         ./configure --enable-R-shlib --prefix=/usr/local
         make uninstall
         make -j8
         make install
      
       if test $? = 0; then
     
         echo "*****************************"
         echo "* Compilation de R terminée *"
         echo "*****************************"

         R_version=$(/usr/local/lib64/R/bin/R --version | grep "R version" | cut -f 3 -d' ') 
         echo $R_version > /home/fab/Dev/altair/sys/R_VERSION
         git commit -am "installed R version $R_version"
                
       else
     
         echo "************************************************"
         echo "* La compilation de R version $R_version a échoué *"
         echo "************************************************"
        
       fi
      
       cd /home/fab/Dev/altair     
      
     else
   
       echo "Pas de répertoire de compilation build !"
      
     fi 
   fi  

else
  
  echo "pas d'actualisation de R par compilation..."
  echo "R : **$R_version**"
  echo "fichier de version :  **$(cat sys/R_VERSION)**"
  
  sleep 2
fi  



if test -f sys/install.RStudio; then
  
   if test -f sys/install.RStudio.force -o x$(cat /usr/local/lib64/rstudio/VERSION) != x$(cat sys/RStudio_VERSION) ; then

     echo "Actualisation de RStudio (blob)..."
       
     if test -d sys/build/rstudio; then

         emerge --unmerge sci-mathematics/rstudio 
         cd sys/build/rstudio
         tar xJf rstudio-bin.tar.xz
         tar xJf rstudio-etc.tar.xz
         rm -f *.xz
         rm -rf /usr/local/lib64/rstudio
         mkdir -p /usr/local/lib64/rstudio
         cp -rf * /usr/local/lib64/rstudio
         rm -f /usr/bin/rstudio
         ln -s /usr/local/lib64/rstudio/bin/rstudio /usr/bin/rstudio
               
       if test $? = 0; then
     
         echo "*************************************"
         echo "* Actualisation de RStudio terminée *"
         echo "*************************************"
         
                
       else
     
         echo "************************************************"
         echo "* La nouvelle installation de RStudio a échoué *"
         echo "************************************************"
        
       fi
      
       cd /home/fab/Dev/altair     
      
     else
   
       echo "Pas de répertoire de compilation build !"
      
     fi 
   fi  

else
  
  echo "pas d'actualisation de RStudio par blob..."
  echo "fichier de version :  **$(cat sys/RStudio_VERSION)**"
  
  sleep 2
fi  



if test -f sys/install.packages -a ! -f sys/packages.installed; then

   echo "Actualisation des paquets..."
  
   export PKGDIR="$PWD/sys/packages"
   
   if test -d sys/packages; then
   
      echo "Installation des paquets..."
      emerge -K --nodeps  $(find $PKGDIR -name '*tbz2')
      echo "Installation des paquets terminée..."
      eix-update
      touch sys/packages.installed
      git add -f sys/packages.installed
      git commit -am "packages.installed"

   else
   
      echo "Echec de l'actualisation des paquets."
      
   fi 

  sleep 2
fi  


# recompilation de la bibliothèque altair
if test -f sys/build.altair; then
  rm -rf altair.linux
  rm -rf /usr/local/lib64/R/library/altair
  rm -rf /usr/lib64/R/library/altair
  git checkout FETCH_HEAD -- altair.linux
  if ! test -f /usr/bin/R; then
     ln -s /usr/local/bin/R /usr/bin/R
  fi

  R CMD INSTALL --byte-compile  -l  /usr/local/lib64/R/library/ altair.linux
  echo "*************************************"
  echo "*                                   *"
  echo "* Nouvelle bibliothèque altair      *"
  echo "*                                   *"
  echo "*************************************"
  sleep 2
fi  


if ! test -d  lhx
    then
        mkdir lhx    
    fi

if ! test -d  Tests/Exemple/Donnees/R-Altair
    then
        mkdir -p Tests/Exemple/Donnees/R-Altair    
    fi
    
_copy Interface/share/altair-gui/images /home/fab/.local/share/Altair
    
cd sys

if test -f install.lib64; then
  echo "********************************************"
  echo "*                                          *"
  echo "* Nouvelles bibliothèques lib64 installées *"
  echo "*                                          *"
  echo "********************************************"
  cp -rfd  lib64/*  /opt/lib64
fi



# script exécuté à la fin d'une session plasma (démontage de la clé)
_copy 10-agent-shutdown.sh /etc/plasma/shutdown

# Vue de dossiers par défaut
_copy defaults /usr/share/plasma/shells/org.kde.plasma.desktop/contents/
# Thème, fonds d'écran
_copy metadata.desktop  /usr/share/plasma/desktoptheme/default/
# script m.sh exécuté au début d'une session plasma (montage de la clé)
_copy ajuster_m   /etc/init.d
# correction d'un bug d'accélération 3 D dans le driver intel i 915 (01.2017)
#_copy 10-monitor.conf /etc/X11/xorg.conf.d
# UTF-8 sur Konqueror
_copy konquerorrc /home/Public
chgrp users /home/Public/konquerorrc 
chmod 0770 /home/Public/konquerorrc 



for i in $(cat /etc/passwd | cut -f1 -d:)
do 
	if test -d /home/$i; then
		# intégration de l'icone dans le menu développement + clic sur projet *.alt
		mkdir -p /home/$i/.local/share/applications
		_copy mimeapps.list   /home/$i/.config/
		_copy mimeapps.list   /home/$i/.local/share/applications
		_copy images          /home/$i/.local/share/Altair     
		_copy mime /home/$i/.local/share
		_copy konquerorrc     /home/$i/.kde4/share/config
        _copy 'Lien vers une application.desktop' /home/$i/Desktop		
	   
                
		if test $i != fab; then
    	    if ! test -d /home/$i/Dev/altair/.Rproj.user; then
	           cp -rvf /home/Public/.Rproj.user  /home/$i/Dev/altair
            fi

            sed "s/utilisateur/${i}/g" /home/fab/Dev/altair/sys/dolphinrc > temp 
            # intégration de li'cone dans le menu développement + clic sur projet *.alt
            _copy Altair_jf.desktop  /home/$i/Desktop/Altaïr.desktop
            _copy Altair_jf.desktop  /home/$i/.local/share/applications
             sed "s/utilisateur/$i/g" user-places.xbel > temp2
    	else
    	    if ! test -d /home/fab/Dev/altair/.Rproj.user; then
	           cp -rvf /home/Public/fab/.Rproj.user  /home/fab/Dev/altair
            fi
            sed "s/\/home\/utilisateur\/Dev\/altair\/Tests\/Exemple\/Donnees\/xhl\/utilisateur/\/home\/fab\/Dev\/altair\/Tests\/Exemple\/Donnees\/xhl/" dolphinrc > temp
            sed "s/utilisateur/$i/g" user-places.xbel > temp2
            sed -i "s/\/home\/fab\/Dev\/altair\/Tests\/Exemple\/Donnees\/xhl\/fab/\/home\/fab\/Dev\/altair\/Tests\/Exemple\/Donnees\/xhl/" temp2
            _copy Altair.desktop  /home/fab/Desktop/Altaïr.desktop	
            _copy Altair.desktop  /home/fab/.local/share/applications
			chown -R fab /home/fab/Dev/altair/Tests/Exemple/Donnees
        fi

        chown -R $i      /home/$i/Dev/altair/.Rproj.user
        chgrp -R users   /home/$i/Dev/altair/.Rproj.user
    	chmod -R 0770    /home/$i/Dev/altair/.Rproj.user

        _copy temp2           /home/$i/.local/share/user-places.xbel        
        _copy temp  /home/${i}/.config/dolphinrc
        rm temp
        rm temp2
		
	fi
done


# réactualisation 
_copy  autostart-scripts/m.sh    /etc/init.d 
chmod 0755  /etc/init.d/ajuster_m
rc-update add ajuster_m default
_copy  ajuster_version    /etc/init.d 
chmod 0755  /etc/init.d/ajuster_version
rc-update add ajuster_version default


# no-op mais souhaitable

chmod -R 0750 /home/fab/Dev/altair/Interface_linux/gui/x64/
chmod -R 0750 /home/fab/Dev/altair/linux

# correction d'un bug sur la version fab de m.sh (réimportation de /home/Public/fab/.Rproj.user à chaque ouverture de session)
cp -vf ./autostart-scripts/m_fab.sh /home/fab/.config/autostart-scripts/m.sh

cd /home/fab/Dev/altair

git rev-parse --verify release

if test $? != 0; then

 echo "***"
 echo "*** Création de la branche locale release... ***"
 echo "***"
 
 git checkout --orphan release
 
 if test $? = 0; then
    git fetch --depth=1 -f -p -n $(cat entrepot.txt) release
    git checkout -f FETCH_HEAD
    git branch -D release
    git checkout -b release 
    git add -f .
 fi
 
 sleep 1
 
else

 echo "***"
 echo "*** Actualisation de la branche locale release... ***"
 echo "***"

 git checkout -f release 
 git fetch --depth=1 -f -p -n $(cat entrepot.txt) release
 git checkout -f FETCH_HEAD 
 git branch -D release
 git checkout -b release
 git add -f .
  
fi 
git commit -am "Release $(date)"

mkdir -p Tests/Exemple/Donnees/R-Altair

echo "*** Opérations sur branche release : Terminé ***"
 
 
git checkout -f dev
 
 

echo "Raffraichissement des paramètres éditeur"
rm -rf /home/Public/.Rproj.user
rm -rf /home/Public/.rstudio-desktop
rm -rf /home/Public/fab
rm -rf sys/Public

cp -rf sys/Public /home
chown -R fab /home/Public
chgrp -R users /home/Public
chmod -R 0777 /home/Public
# correction sur .Rproj.user 

cp -rf sys/Public/.rstudio-desktop   /home/fab
cp -rf sys/Public/.Rproj.user        /home/fab/Dev/altair
chmod -R 0777 /home/fab/Dev/altair/.Rproj.user         
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 

 

