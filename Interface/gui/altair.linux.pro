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

VERSION_TAG = $$system(cat ../../VERSION)
DEFINES +=  VERSION=\\\"$$VERSION_TAG\\\"
message("Version :  $$VERSION_TAG")
#QMAKE_CXX = /usr/bin/g++-6.3.0
greaterThan(QT_MAJOR_VERSION, 5)

# utiliser au moins Qt5 et g++-5.1
# ENCODAGE : UTILISER UTF-8 PARTOUT, y compris sur le fichier .pro.

if (linux) {
  message("Système d'exploitation linux")
} else {
  error("Le système d'exploitation doit être linux")
}

GIT_VERSION = $$system(git --version | grep -e \"git version\")
CXX_VERSION = $$system($$QMAKE_CXX --version | grep -e '[5-9].[0-9]')

if (!isEmpty(GIT_VERSION)) {
    message( "Version de git : $$GIT_VERSION" )
} else {
#    error( "Git doit être installé" )
}


if (!isEmpty(CXX_VERSION)){
    message( "Version du compilateur : $$CXX_VERSION" )
} else {
    error( "Le compilateur doit être GNU g++, dont la version doit être au moins 5.1" )
}

QMAKE_CFLAGS =

CONFIG  += ordered static
CONFIG(debug, debug|release) {
  QMAKE_LFLAGS   += -L$$(QTDIR)/bin   # ne devrait pas en principe être rajouté mais...qmake est capricieux !

} else {
  CONFIG += static
  QMAKE_CXXFLAGS += -O3 -fomit-frame-pointer -fexpensive-optimizations
  QMAKE_LFLAGS += -s -licui18n -licuuc -licudata
}

TEMPLATE = app

QT       += core gui xml widgets printsupport
            #webkitwidgets :   décommenter si STATIC n'est pas défini c-dessous. Nécessite libWebKitWidgets

QT       -= opengl
QT       -= openssl

TARGET = Altair

VPATH = .

DEFINES += PROGRESSBAR_TIMEOUT=200    # Intervalle d'incrémentation de la barre de progression en millisecondes
                            # Un timeout plus élevé diminue l'overhead mais rend la la progression moins fluide.
                            # Le minimum (timeout = 0) donne des progressions fluides mais augmente de 10 % la durée d'exec.

DEFINES += MAXIMUM_CONSOLE_OUTPUT=800  \ # Pour limiter le volume de lignes de la cosole et éviter des plantages ou ralentissements en cas de verbosité excessive
           MINIMUM_LINE_WIDTH=360      \ # Largeur minimale des lignes de l'onglet d'entrée du dialogue d'options (codes de paye). Détermine la lageur du dialogue
           #WIDTH=1170                   \# Largeur par défaut de l'interface        
           #HEIGHT=700                   \# Hauteur par défaut de l'interface 

DEFINES += COMMANDLINE_CONSOLE_OUTPUT          \        # Générer la ligne de commande en console (verbeux)
           GUI_OUTPUT                                   # Balises d'identification des lignes de la console : mode GUI

DEFINES += QT_NO_OPENGL \
           STATIC\                                      # utiliser pour lancer le navigateur internet par défaut plustôt qu'un navigateur interne
           LOCAL_BINPATH \                              # chemins d'exécution définis par rapport l'emplacement de l'exécutable
           USE_RIGHT_CLICK  \                           # utiliser un clic droit sur les fichiers pour ajouter, supprimer etc.
           #REGEX_ANONYM   \                            # Utiliser en cas de problème avec l'analyse des données sensibles dans l'anonymisation
           #INSERT_DIRPAGE   \                          # Utiliser un dialogue de configuration des répertoires          
           #INSERT_MAXN                                 # Insérer des champs pour -n et -N dans le dialogue d'options 


QMAKE_CXXFLAGS += -std=gnu++11                         # obligatoire
QMAKE_CXXFLAGS += -march=core-avx2  -pipe -m64         # facultatif


SOURCES += \
    options.cpp \
    mainwindow.cpp \
    fwidgets.cpp \
    fstring.cpp \
    flistframe.cpp \
    altair.cpp \
    common.cpp \
    forms.cpp \
    main.cpp \
    browser.cpp \
    xmlparser.cpp \
    highlighter.cpp \
    run.cpp \
    flineframe.cpp

HEADERS  += \
    options.h \
    fwidgets.h \
    fstring.h \
    flistframe.h \
    common.h \
    altair.h \
    enums.h \
    forms.h \
    browser.h \
    altair.h \
    highlighter.h \
    tags.h \
    templates.h \
    altair-gui.h \
    flineframe.hpp \
    version.h


RESOURCES += \
    ../share/altair-gui/altair-gui.qrc

   

DISTFILES += \
    ../../A_FAIRE.txt \
    ../share/altair-gui/images/internet-web-browser.png







