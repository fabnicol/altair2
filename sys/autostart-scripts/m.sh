
# La clé est reconnue par sa dimension de 64 Go, par son formatage exfat et son caractère de disque USB

cle=/dev/$(lsblk -O | grep -E ".*exfat.*[0-9,.]{2,5}G.*part.*scsi:usb" | cut -f 3 -d \ )

DATA=$HOME/Dev/altair/Tests/Exemple/Donnees

udisksctl mount --no-user-interaction -b $cle

if test -h $DATA/R-Altair; then rm  $DATA/R-Altair; fi	
TARGET=$(find /run/media/$USER -mindepth 1 -maxdepth 1 -type d)
echo "Found TARGET=$TARGET"
if test -d "$TARGET"; then
    echo "Linking $DATA/R-Altair to $TARGET"
    cd $DATA
    ln -s  -v  $TARGET R-Altair
    cd -
fi

cp -f  /home/Public/konquerorrc  $HOME/.kde4/share/config
cp -rf /home/Public/.Rproj.user  $HOME/Dev/altair
