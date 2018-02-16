#!/bin/bash

if test x$1 == xh
then
	echo "USAGE: export [option] [branche]"
	echo "options:"
	echo R  'fichiers R (*.R, *.Rmd)'
	echo I  "développement de l'interface graphique"
	echo L  "développement du moteur d'extraction XML"
	echo B  "fichiers binaires"
	echo dist "fichiers de distribution : répertoires %APPDATA/{Local,Roaming}, .Rproj.user et fichiers racines"
	echo a  "Toutes les catégories précédentes"
	echo branches:
	echo "Par défaut les banches distribution et distribution-dev sont concernées."
	echo "Préciser la seule branche concernée sinon."
fi

files=""

if test x$1 == xR -o xa
then 
	files+=$(ls Tests/Exemple/*.{R,Rmd})
else
	if test x$1 == xI -o xa
	then
	  if test -d Interface
	  then
		files+="Interface/gui Interface/share" 
	  fi
	elif test x$1 == xL -o xa
	then
	   if test -d lhx
	   then
		 files+=$(ls lhx/*.{c,h,cpp,hpp,pro,pro.user})   
	   fi
	elif test x$1 == xB -o xa
	then 
	   files+="Interface_windows Interface_windows_min win"
	elif test x$1 == dist -o xa
	then 
	  cp -rf .Rproject RStudio-project
	  if test -d Interface
	  then
		cp -rf C:\Users\fnicol\AppData\Local\RStudio-project Local.dev
		cp -rf C:\Users\fnicol\AppData\Roaming\Qtproject Roaming.dev
		cp -rf C:\Users\fnicol\AppData\Roaming\RStudio Roaming.dev
		files+="Local.dev Roaming.dev RStudio-project"
	  else 	
		cp -rf C:\Users\fnicol\AppData\Local\RStudio-project Local
		cp -rf C:\Users\fnicol\AppData\Roaming\RStudio Roaming
		files+="Local Roaming RStudio-project"
	  fi	
	  files+=" $(find . -maxdepth=1 -type=f)"
	fi
fi

echo "Exportation de $files ..."

if test x$2 == x
then 
   branches="distribution distribution-dev"
else
   branches=$2
fi

for B in $branches
do
  git checkout $B
  echo "Exportation par cherry-pick vers $B ..."
  git rev-list --reverse -n 1  distribution-dev -- $files | git cherry-pick -X theirs --strategy recursive --stdin
done

git checkout distribution-dev
echo "Terminé."


  
