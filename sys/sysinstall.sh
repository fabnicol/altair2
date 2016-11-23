cp -vf sys/10-agent-shutdown.sh /etc/plasma/shutdown
cp -vf sys/m.sh /etc/profile.d
cp -vf compte_utilisateur.sh /usr/bin
sed -i 's/ALL ALL=(ALL) ALL/#ALL ALL=(ALL) ALL/' /etc/sudoers
if test -d sys/kernel; then
  cp -vf sys/kernel/*   /boot
  grub-mkconfig -o /boot/grub/grub.cfg
  echo "Installation du noyau réalisée."
fi
