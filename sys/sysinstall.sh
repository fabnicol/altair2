chmod -R +rwx sys/*sh
cp -vf sys/10-agent-shutdown.sh /etc/plasma/shutdown
cp -vf sys/compte_utilisateur.sh /usr/bin
sed -i 's/ALL ALL=(ALL) ALL/#ALL ALL=(ALL) ALL/' /etc/sudoers
if test -d sys/kernel; then
  cp -vf sys/kernel/*   /boot
  grub-mkconfig -o /boot/grub/grub.cfg
  echo "Installation du noyau réalisée."
fi
if test -f sys/build.altair; then
  R CMD INSTALL --byte-compile  -l  /usr/lib64/R/library/ altair.linux
fi  
chmod -R 0777 /home/jf/.rstudio-desktop

# Vue de dossiers par défaut
cp -vf sys/defaults          /usr/share/plasma/shells/org.kde.plasma.desktop/contents/

# Thème, fonds d'écran
cp -vf sys/metadata.desktop  /usr/share/plasma/desktoptheme/default/

if test ! -d /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl/Anonyme2 ; then
   mkdir -p /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl
   cp -rvf /home/Public/xhl/Anonyme2 /home/fab/Dev/altair/Tests/Exemple/Donnees/xhl
   chown -R fab /home/fab/Dev/altair/Tests/Exemple/Donnees/
fi   

if test ! -f /etc/init.d/ajuster_m; then
 cp -f sys/ajuster_m   /etc/init.d
 chmod 0755  /etc/init.d/ajuster_m
 rc-update add ajuster_m default
fi 
 

