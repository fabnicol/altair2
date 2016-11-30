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
  sudo R CMD INSTALL --byte-compile  -l  /usr/lib64/R/library/ altair.linux
fi  
chmod -R 0777 /home/jf/.rstudio-desktop
chmod -R 0777 /home/jf/Dev/altair/.Rproj.user

for compte_login in {1..8}; do
   cp -vrf /home/fab/Dev/altair/sys/autostart-scripts   /home/$compte_login/.config
done
