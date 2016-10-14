

DATA=$HOME/Dev/altair/Tests/Exemple/Donnees
#udisksctl unmount --no-user-interaction --force /dev/sdc1
udisksctl mount --no-user-interaction -b /dev/sdc1

   if test -h $DATA/R-Altair; then rm -f $DATA/R-Altair; fi	
   TARGET=$(find /run/media/$USER -mindepth 1 -maxdepth 1 -type d)
   echo "Found TARGET=$TARGET"
   if test -d $TARGET; then
      echo "Linking $DATA/R-Altair to $TARGET"
      cd $DATA
      ln -s  -v  $TARGET R-Altair
      cd -
   fi




