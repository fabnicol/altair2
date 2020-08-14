#!/bin/sh
#
# This file is executed at Plasma shutdown.
# Uncomment the following lines to kill the agents
# that were started at session startup.

# gnupg 2.0.x
#if [ -n "${GPG_AGENT_INFO}" ]; then
#	kill $(echo ${GPG_AGENT_INFO} | cut -d':' -f 2) >/dev/null 2>&1
#fi

# gnupg 2.1.x
#gpgconf --kill gpg-agent >/dev/null 2>&1

#if [ -n "${SSH_AGENT_PID}" ]; then
#	eval "$(ssh-agent -s -k)"
#fi

# Tester /dev/sdb1, /dev/sdc1, /dev/sdd1

# La clé est reconnue par sa dimension de XYZTU Go, par son formatage exfat et son caractère de disque USB

cle=/dev/$(lsblk -O | grep -E ".*exfat.*[0-9,.]{2,5}G.*part.*scsi:usb" | cut -f 3 -d \ )


udisksctl unmount --force --no-user-interaction --block-device  $cle

# Il faut effacer le lien symbolique pour éviter des problème de conflits potentiels de clés si les clés changent entre deux connexions
# Il sera recréé à la prochaine connection

if test -h ~/Dev/altair/scripts/R/Donnees/R-Altair; then
  rm ~/Dev/altair/scripts/R/Donnees/R-Altair
else

#  Il faut effacer les donssiers "durs" qui pourraient avoir été créés en l'absence de clé. En effet la création d'un lien symbolique pourrait échouer "par dessus"

  if test -d ~/Dev/altair/scripts/R/Donnees/R-Altair; then
    rm -rf ~/Dev/altair/scripts/R/Donnees/R-Altair
  fi
fi

# Probablement inutile mais des cas de problèmes sont rapportés dans les forums
rm ~/.Xauthority


# .Rproj.user doit être raffraichi à chaque connection pour actualiser les droits d'accès. Sinon RStudio démarerra sans les panneaux 
# de scripts ajustés par défaut
# L'expérience montre que ceci est difficulement réalisable par chown/chmod L'utilisation d'une recopie depui /home/Public à la connexions
# est empiriquement testés comme à la fois plus simple et plus sûre. 

echo "Effacement de : $HOME/Dev/altair/.Rproj.user" >> /home/Public/log 
rm -rf $HOME/Dev/altair/.Rproj.user
if test -d $HOME/Dev/altair/.Rproj.user; then
  echo "Effacement incomplet" >> /home/Public/log
fi



