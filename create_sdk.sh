export PS1='$'
echo "Début de la création du SDK..."
if test -e Altaïr-SDK
then
	echo "Effacement du répertoire SDK"
	rm -rf Altaïr-SDK
	echo "Effacement terminé"
fi
mkdir -p Altaïr-SDK/altair
var=0
for term in LICENCE dlls Docs Git Interface Interface_windows Interface_windows_min lib Local Outils R R64 Roaming RStudio RTools win  ".Rproj.user"
do
	((var++))
	echo -ne "$var/20: copie de $term en cours...               "\\r
	cp -rf $term Altaïr-SDK/altair
done
echo "20/20 : copie de lhx"
mkdir Altaïr-SDK/altair/lhx
cp lhx/*.* Altaïr-SDK/altair/lhx
cp lhx/LICENCE Altaïr-SDK/altair/lhx
echo "Sous répertoires copiés hormis les exemples"
cp -rf  *.*  Altaïr-SDK/altair
echo "Fichiers racine copiés"
mkdir -p Altaïr-SDK/altair/Tests/Exemple/Donnees/xhl
cp -rf Tests/Exemple/Docs  Tests/Exemple/Projets  Altaïr-SDK/altair/Tests/Exemple
cp Tests/Exemple/*.* Altaïr-SDK/altair/Tests/Exemple
cp -rf Tests/Exemple/Donnees/xhl/Anonyme Tests/Exemple/Donnees/xhl/Anonyme2 Altaïr-SDK/altair/Tests/Exemple/Donnees/xhl
mkdir Altaïr-SDK/altair/Tests/Exemple/Donnees/R-Altaïr 
echo "Exemples copiés"
cp Paquets/*  Altaïr-SDK
echo "Paquets copiés"
echo "Terminé"
