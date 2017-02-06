function _copy () {

if test -f "$1"; then
  cp -fv "$1" "$2"
else
  if test -d "$1"; then
    cp -rvf "$1"  "$2"
  fi
fi  
  
}

cd sys
chmod -R +rwx *sh

# obsolète
# sed -i 's/ALL ALL=(ALL) ALL/#ALL ALL=(ALL) ALL/' /etc/sudoers

# recompilation de la bibliothèque altair
if test -f build.altair; then
  R CMD INSTALL --byte-compile  -l  /usr/lib64/R/library/ altair.linux
fi  

# accès des données test
if test ! -d /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl/Anonyme2 ; then
   mkdir -p /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl
   cp -rf /home/Public/xhl/Anonyme2 /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl
   chown -R fab /home/fab/Dev/altair/Tests/Exemple/Donnees/
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
# correction sur .Rproj.user de fab
_copy /home/Public/fab/.Rproj.user /home/fab/Dev/altair
_copy .rstudio-desktop   /home/Public

for i in $(cat /etc/passwd | cut -f1 -d:)
do 

	if test $i != fab; then
		if test -d /home/$i; then
		  cp -vf 'Lien vers une application.desktop' /home/$i/Desktop
		  sed "s/utilisateur/${i}/g" /home/fab/Dev/altair/sys/dolphinrc > temp 
		  cp -vf temp  /home/${i}/.config/dolphinrc
		  # intégration de li'cone dans le menu développement + clic sur projet *.alt
		 _copy Altair_jf.desktop  /home/$i/Desktop/Altaïr.desktop
		fi
	else
		cp -vf 'Lien vers une application.desktop' /home/$i/Desktop
		sed "s/\/home\/utilisateur\/Dev\/altair\/Tests\/Exemple\/Donnees\/xhl\/utilisateur/\/home\/fab\/Dev\/altair\/Tests\/Exemple\/Donnees\/xhl/" /home/fab/Dev/altair/sys/dolphinrc > temp 
		cp -vf temp  /home/fab/.config/dolphinrc
		_copy Altair.desktop  /home/fab/Desktop/Altaïr.desktop		
	fi

	if test -d /home/$i; then
		# intégration de l'icone dans le menu développement + clic sur projet *.alt
         mkdir -p /home/$i/.local/share/applications
		_copy Altair.desktop  /home/$i/.local/share/applications
		_copy mimeapps.list   /home/$i/.config/
		_copy mimeapps.list   /home/$i/.local/share/applications
		_copy mime /home/$i/.local/share/applications
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

# réactualisation du grub
chmod 0755  /etc/init.d/ajuster_m
rc-update add ajuster_m default

# no-op mais souhaitable
chmod -R 0777 /home/jf/.rstudio-desktop

# correction d'un bug sur la version fab de m.sh (réimportation de /home/Public/fab/.Rproj.user à chaque ouverture de session)
cp -vf ./autostart-scripts/m_fab.sh /home/fab/.config/autostart-scripts/m.sh

cd -

 

