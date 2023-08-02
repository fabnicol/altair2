# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
# produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
# en vigueur à compter de l'année 2008.
# 
# Ce logiciel est régi par la licence CeCILL soumise au droit français et
# respectant les principes de diffusion des logiciels libres. Vous pouvez
# utiliser, modifier et/ou redistribuer ce programme sous les conditions
# de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
# sur le site "http://www.cecill.info".
# 
# En contrepartie de l'accessibilité au code source et des droits de copie,
# de modification et de redistribution accordés par cette licence, il n'est
# offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
# seule une responsabilité restreinte pèse sur l'auteur du programme, le
# titulaire des droits patrimoniaux et les concédants successifs.
# 
# A cet égard l'attention de l'utilisateur est attirée sur les risques
# associés au chargement, à l'utilisation, à la modification et/ou au
# développement et à la reproduction du logiciel par l'utilisateur étant
# donné sa spécificité de logiciel libre, qui peut le rendre complexe à
# manipuler et qui le réserve donc à des développeurs et des professionnels
# avertis possédant des connaissances informatiques approfondies. Les
# utilisateurs sont donc invités à charger et tester l'adéquation du
# logiciel à leurs besoins dans des conditions permettant d'assurer la
# sécurité de leurs systèmes et ou de leurs données et, plus généralement,
# à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
# 
# Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
# pris connaissance de la licence CeCILL, et que vous en avez accepté les
# termes.
# 
# 

VERSION_TAG = $$system("C:\\Users\\Public\\Dev\\altair_SDK\\usr\\bin\\cat.exe ..\\..\\VERSION")
DEFINES +=  VERSION=\\\"$$VERSION_TAG\\\"

message("Version :  $$VERSION_TAG")
greaterThan(QT_MAJOR_VERSION, 5)

# ENCODAGE : UTILISER UTF-8 PARTOUT, y compris sur le fichier .pro.

CONFIG  += ordered static 

CONFIG(debug, debug|release) {
  QMAKE_LFLAGS   += -L$$(QTDIR)/bin   # ne devrait pas en principe être rajouté mais...qmake est capricieux !

} else {
  CONFIG += static
  QMAKE_CXXFLAGS += -O3 -fomit-frame-pointer -fexpensive-optimizations -Wall -Wextra -static


}

QMAKE_LFLAGS += -LC:\Users\Public\Dev\altair_SDK\mingw64\bin
QMAKE_LFLAGS +=  -s   -static-libgcc -static-libstdc++ \
C:\Users\Public\Dev\altair_SDK\mingw64\bin\libbz2-1.dll \
C:\Users\Public\Dev\altair_SDK\mingw64\bin\liblzma-5.dll \
C:\Users\Public\Dev\altair_SDK\mingw64\bin\libzstd.dll \
C:\Users\Public\Dev\altair_SDK\mingw64\bin\zlib1.dll \
C:\Users\Public\Dev\altair_SDK\mingw64\bin\libdeflate.dll \
C:\Users\Public\Dev\altair_SDK\mingw64\bin\libtiff-5.dll \
C:\Users\Public\Dev\altair_SDK\mingw64\bin\libjasper-4.dll \
C:\Users\Public\Dev\altair_SDK\mingw64\bin\libjpeg-8.dll \
C:\Users\Public\Dev\altair_SDK\mingw64\bin\liblcms2-2.dll \
C:\Users\Public\Dev\altair_SDK\mingw64\bin\libLerc.dll  \
C:\Users\Public\Dev\altair_SDK\mingw64\bin\libjbig-0.dll \
C:\Users\Public\Dev\altair_SDK\mingw64\bin\libharfbuzz-0.dll

TEMPLATE = app

QT       += core gui xml widgets

QT       -= opengl
QT       -= openssl

TARGET = Altair

VPATH = .
INCLUDEPATH += ../../fwidgets_lib

DEFINES += HAS_CPP17

DEFINES += SCRIPT_DIR=\\\"scripts/R/\\\"  # Dossier des scripts R

DEFINES += MAX_FICHIERS_RECENTS=5     # Nombre maximum de fichiers récents

DEFINES += PROGRESSBAR_TIMEOUT=200    # Intervalle d'incrémentation de la barre de progression en millisecondes
                                      # Un timeout plus élevé diminue l'overhead mais rend la la progression moins fluide.
                                      # Le minimum (timeout = 0) donne des progressions fluides mais augmente de 10 % la durée d'exec.

DEFINES += MAXIMUM_CONSOLE_OUTPUT=800  \ # Pour limiter le volume de lignes de la cosole et éviter des plantages ou ralentissements en cas de verbosité excessive
           MINIMUM_LINE_WIDTH=360      \ # Largeur minimale des lignes de l'onglet d'entrée du dialogue d'options (codes de paye). Détermine la lageur du dialogue
           #WIDTH=1170                   \# Largeur par défaut de l'interface        
           #HEIGHT=700                   \# Hauteur par défaut de l'interface 

DEFINES += COMMANDLINE_CONSOLE_OUTPUT          \        # Générer la ligne de commande en console (verbeux)
           GUI_OUTPUT      \                             # Balises d'identification des lignes de la console : mode GUI
           HAVE_APPLICATION

DEFINES += LOCAL_BINPATH \                              # chemins d'exécution définis par rapport l'emplacement de l'exécutable
           USE_RIGHT_CLICK  \                           # utiliser un clic droit sur les fichiers pour ajouter, supprimer etc.
           #REGEX_ANONYM   \                            # Utiliser en cas de problème avec l'analyse des données sensibles dans l'anonymisation


QMAKE_CXXFLAGS += -std=c++17                           # obligatoire
QMAKE_CXXFLAGS += -march=core-avx2  -pipe -m64         # facultatif


SOURCES += \
    options.cpp \
    mainwindow.cpp \
    altair.cpp \
    common.cpp \
    main.cpp \
    xmlparser.cpp \
    run.cpp \
    matricules.cpp \
    ../../fwidgets_lib/flineframe.cpp \
    ../../fwidgets_lib/flistframe.cpp \
    ../../fwidgets_lib/forms.cpp \
    ../../fwidgets_lib/fstring.cpp \
    ../../fwidgets_lib/fwidgets.cpp \
    ../../fwidgets_lib/fwidgets_lib.cpp \
    ../../fwidgets_lib/tools.cpp \


HEADERS  += \
    options.h \
    common.h \
    altair.h \
    altair.h \
    altair-gui.h \
    gui_enums.h \
    matricules.h \
    sommaire.h \
    ../../fwidgets_lib/custom.h \
    ../../fwidgets_lib/enums.h \
    ../../fwidgets_lib/flineframe.h \
    ../../fwidgets_lib/flistframe.h \
    ../../fwidgets_lib/forms.h \
    ../../fwidgets_lib/fstring.h \
    ../../fwidgets_lib/fwidgets.h \
    ../../fwidgets_lib/fwidgets_lib.h \
    ../../fwidgets_lib/fwidgets_lib_global.h \
    ../../fwidgets_lib/tags.h \
    ../../fwidgets_lib/templates.h \
    ../../fwidgets_lib/tools.h \
    ../../fwidgets_lib/custom.h \
    ../../fwidgets_lib/enums.h \



RESOURCES += \
    ../share/altair-gui/altair-gui.qrc

   

DISTFILES += \
    ../../A_FAIRE.txt \
    ../share/altair-gui/images/internet-web-browser.png







