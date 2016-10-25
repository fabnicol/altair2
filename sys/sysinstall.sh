cp -vf sys/10-agent-shutdown.sh /etc/plasma/shutdown
cp -vf sys/m.sh /etc/profile.d
cp -vf compte_utilisateur.sh /usr/bin
sed -i 's/ALL ALL=(ALL) ALL/#ALL ALL=(ALL) ALL/' /etc/sudoers
