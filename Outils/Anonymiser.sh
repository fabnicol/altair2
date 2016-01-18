#!./bash.exe
echo "Création de la liste des fichiers..."
./find.exe . -type f -name *.xhl -fprint filelist

if test -f filelist ; then 
 echo "Conversion des fichiers..."
else
  echo "Impossible de trouver les fichiers xhl" 
  exit -1 
fi

while read -r fichier || [[ -n "$fichier" ]]
do
 ./gawk.exe  -f gawkscript   "$fichier" 2>log.erreurs
done  < filelist

echo "Fin de la conversion..."
while read -r fichier || [[ -n "$fichier" ]]
do
 [[ -f "${fichier}.new" ]] && mv "${fichier}.new" "$fichier" 2>>log.erreurs
done  < filelist

if test  "" == "$(cat log.erreurs)"; then
 echo "Aucune erreur détectée"
else
 echo "Erreurs détectées : " 
 cat log.erreurs 
fi
 






