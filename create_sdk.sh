echo "Début de la création du SDK..."
mkdir -p Altaïr-SDK/altair
cp -rf dlls Docs Git Interface Interface_windows Interface_windows_min lib Local Outils R R64 Roaming RStudio RTools Tests/Exemple/Docs Tests/Exemple/Donnees/xhl/Anonyme Tests/Exemple/Donnees/xhl/Anonyme2 Tests/Exemple/Donnees/R-Altaïr  Tests/Exemple/Projets   .Rproj.user *.* Licence Altaïr-SDK/altair
mkdir Altaïr-SDK/altair/lhx
cp lhx/*.* Altaïr-SDK/altair/lhx
cp lhx/LICENCE Altaïr-SDK/altair/lhx
mkdir -p Altaïr-SDK/altair/Tests/Exemple
cp Tests/Exemple/*.* Altaïr-SDK/altair/Tests/Exemple
cp Paquets/*.*  Altaïr-SDK
echo "Terminé"
