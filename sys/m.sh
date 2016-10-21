
# Tester /dev/sdb1, /dev/sdc1, /dev/sdd1

cle=$(blkid | grep exfat | cut -f 1 -d\  | sed 's/://g')


DATA=$HOME/Dev/altair/Tests/Exemple/Donnees

udisksctl mount --no-user-interaction -b $cle

if test -h $DATA/R-Altair; then rm  $DATA/R-Altair; fi	
TARGET=$(find /run/media/$USER -mindepth 1 -maxdepth 1 -type d)
echo "Found TARGET=$TARGET"
if test -d $TARGET; then
    echo "Linking $DATA/R-Altair to $TARGET"
    cd $DATA
    ln -s  -v  $TARGET R-Altair
    cd -
fi




