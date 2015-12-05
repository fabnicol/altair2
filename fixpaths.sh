#!/bin/bash
# fix paths, this file must be run from within source directory

CUR=$(echo $PWD | sed -e 's/\/c/C:/' -e 's/\//\\/g')

echo "Correction des sources à partir du dossier $CUR"

find qtbase -type f -exec grep -Iq . {} \; -and -print0 | xargs -0 sed -i 's/(C:.*qt-everywhere-opensource-src-5.5.0)/$CUR/g'
