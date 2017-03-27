function _copy () {

if test -f "$1"; then
  cp -fv "$1" "$2"
else
  if test -d "$1"; then
    cp -rvf "$1"  "$2"
  fi
fi  
  
}


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

cd sys
chmod -R +rwx *sh

# obsolète
# sed -i 's/ALL ALL=(ALL) ALL/#ALL ALL=(ALL) ALL/' /etc/sudoers

# recompilation de la bibliothèque altair
if test -f build.altair; then
  R CMD INSTALL --byte-compile  -l  /usr/lib64/R/library/ altair.linux
fi  

# création du dossier Bulletins sous jf

mkdir -p /home/jf/Dev/altair/Tests/Exemple/Donnees/Bulletins
chgrp -R users /home/jf/Dev/altair/Tests/Exemple/Donnees/Bulletins
chmod -R 0770 /home/jf/Dev/altair/Tests/Exemple/Donnees/Bulletins
             
# accès des données test
if test ! -d /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl/Anonyme2 ; then
   mkdir -p /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl
   cp -rf /home/Public/xhl/Anonyme2 /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl
fi   

# script exécuté à la fin d'une session plasma (démontage de la clé)
_copy 10-agent-shutdown.sh /etc/plasma/shutdown
_copy compte_utilisateur.sh /usr/bin
# Vue de dossiers par défaut
_copy defaults /usr/share/plasma/shells/org.kde.plasma.desktop/contents/
# Thème, fonds d'écran
_copy metadata.desktop  /usr/share/plasma/desktoptheme/default/
# script m.sh exécuté au début d'une session plasma (montage de la clé)
_copy ajuster_m   /etc/init.d
# correction d'un bug d'accélération 3 D dans le driver intel i 915 (01.2017)
_copy 10-monitor.conf /etc/X11/xorg.conf.d
# UTF-8 sur Konqueror
_copy konquerorrc /home/Public
chown jf /home/Public/konquerorrc 
chgrp users /home/Public/konquerorrc 
chmod 0770 /home/Public/konquerorrc 

# correction sur .Rproj.user 
_copy /home/Public/fab/.Rproj.user /home/fab/Dev/altair
_copy .rstudio-desktop   /home/Public
  
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

# if test -f localgitconfig -a -f .gitconfig; then
    # echo '**********************'
    # echo '* Actualisation de GIT'
    # echo '**********************'
    # echo
    # cp -vf localgitconfig  /home/fab/Dev/altair/.git/config
    # cp -vf .gitconfig      /home/fab/.gitconfig
# fi

# réactualisation 
_copy  autostart-scripts/m.sh    /etc/init.d 
chmod 0755  /etc/init.d/ajuster_m
rc-update add ajuster_m default
_copy  ajuster_version    /etc/init.d 
chmod 0755  /etc/init.d/ajuster_version
rc-update add ajuster_version default


# no-op mais souhaitable
chmod -R 0777 /home/jf/.rstudio-desktop

# correction d'un bug sur la version fab de m.sh (réimportation de /home/Public/fab/.Rproj.user à chaque ouverture de session)
cp -vf ./autostart-scripts/m_fab.sh /home/fab/.config/autostart-scripts/m.sh
git config --global --unset http.proxy
cd -

 

