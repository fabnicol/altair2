git checkout -f master-jf
git commit -am"Sauvegarde master-jf $(date)"
chwon -R fab .
sleep 2
cp -vf Actualiser.sh ../
sleep 1
echo "****** Terminé ******* "
sleep 1
