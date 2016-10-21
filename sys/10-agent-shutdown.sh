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

# La clé est reconnue par sa dimension de 64 Go, par son formatage exfat et son caractère de disque USB

cle=/dev/$(lsblk -O | grep ".*exfat.*58,4G.*part.*scsi:usb" | cut -f 2 -d \ )


udisksctl unmount --force --no-user-interaction --block-device  $cle
if test -h ~/Dev/altair/Tests/Exemple/Donnees/R-Altair; then
  rm ~/Dev/altair/Tests/Exemple/Donnees/R-Altair
fi
rm ~/.Xauthority






