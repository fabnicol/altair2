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

VERSION_TAG = $$system(cat $HOME/Dev/altair2/VERSION)
DEFINES +=  VERSION=\\\"$$VERSION_TAG\\\"
message("Version :  $$VERSION_TAG")
#QMAKE_CXX = /usr/local/bin/g++
#QMAKE_LINK = /usr/local/bin/g++
greaterThan(QT_MAJOR_VERSION, 5)

# utiliser au moins Qt5 et g++-5.1
# ENCODAGE : UTILISER UTF-8 PARTOUT, y compris sur le fichier .pro.

if (linux) {
  message("Système d'exploitation linux")
}
#QMAKE_CXX=/usr/bin/g++-8.1.0
GIT_VERSION = $$system(git --version | grep -e \"git version\")

if (!isEmpty(GIT_VERSION)) {
    message( "Version de git : $$GIT_VERSION" )
} else {
#    error( "Git doit être installé" )
}

CONFIG  += ordered static 
#QTPLUGIN += qxcb
CONFIG(debug, debug|release) {
  QMAKE_LFLAGS   += -L$$(QTDIR)/bin   # ne devrait pas en principe être rajouté mais...qmake est capricieux !

} else {
  CONFIG += static
  QMAKE_CXXFLAGS += -O3 -fomit-frame-pointer -fexpensive-optimizations -Wall -Wextra -static
  QMAKE_LFLAGS +=  -s   -static-libgcc -static-libstdc++
}


TEMPLATE = app

QT       += core gui xml widgets

QT       -= opengl
QT       -= openssl


TARGET = Altair

VPATH = .
INCLUDEPATH += ../../fwidgets_lib
#windows:LIBS += C:/Users/Public/Dev/altair2/fwidgets_lib/libfwidgets_lib.a
#linux:LIBS += ~/Dev/altair/fwidgets_lib/libfwidgets_lib.a

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
    ../../fwidgets_lib/flineframe.cpp \
    ../../fwidgets_lib/flistframe.cpp \
    ../../fwidgets_lib/forms.cpp \
    ../../fwidgets_lib/fstring.cpp \
    ../../fwidgets_lib/fwidgets.cpp \
    ../../fwidgets_lib/tools.cpp \
    options.cpp \
    mainwindow.cpp \
    altair.cpp \
    common.cpp \
    main.cpp \
    xmlparser.cpp \
    run.cpp \
    matricules.cpp


HEADERS  += \
    ../../fwidgets_lib/custom.h \
    ../../fwidgets_lib/enums.h \
    ../../fwidgets_lib/flineframe.h \
    ../../fwidgets_lib/flistframe.h \
    ../../fwidgets_lib/forms.h \
    ../../fwidgets_lib/fstring.h \
    ../../fwidgets_lib/fwidgets.h \
    ../../fwidgets_lib/tags.h \
    ../../fwidgets_lib/templates.h \
    ../../fwidgets_lib/tools.h \
    options.h \
    common.h \
    altair.h \
    altair.h \
    altair-gui.h \
    gui_enums.h \
    matricules.h \
    sommaire.h


RESOURCES += \
    ../share/altair-gui/altair-gui.qrc


DISTFILES += \
    ../../A_FAIRE.txt \
    ../../VERSION \
    ../share/altair-gui/images/internet-web-browser.png \
    ../share/altair-gui/images/process.png \
    ../share/altair-gui/images/log_file_icon.jpg \
    ../share/altair-gui/images/about.png \
    ../share/altair-gui/images/accessories-text-editor.png \
    ../share/altair-gui/images/altair.png \
    ../share/altair-gui/images/anonymiser.png \
    ../share/altair-gui/images/application-exit.png \
    ../share/altair-gui/images/application-xml.png \
    ../share/altair-gui/images/applyEffectsToOneTrackToggledIcon.png \
    ../share/altair-gui/images/applyEffectsToOneTrackUntoggledIcon.png \
    ../share/altair-gui/images/archive.png \
    ../share/altair-gui/images/arrow-down.png \
    ../share/altair-gui/images/arrow-up.png \
    ../share/altair-gui/images/breakpoint.png \
    ../share/altair-gui/images/burn-deprecated.png \
    ../share/altair-gui/images/clean.png \
    ../share/altair-gui/images/configure-deprecated.png \
    ../share/altair-gui/images/configure-toolbars.png \
    ../share/altair-gui/images/configure-toolbars2.png \
    ../share/altair-gui/images/console.png \
    ../share/altair-gui/images/csv.png \
    ../share/altair-gui/images/data-icon.png \
    ../share/altair-gui/images/directory.png \
    ../share/altair-gui/images/display-output-deprecated.png \
    ../share/altair-gui/images/display-output.png \
    ../share/altair-gui/images/document-close.png \
    ../share/altair-gui/images/document-import.png \
    ../share/altair-gui/images/document-open-folder.png \
    ../share/altair-gui/images/document-open.png \
    ../share/altair-gui/images/document-save-as.png \
    ../share/altair-gui/images/document-save.png \
    ../share/altair-gui/images/edit-clear.png \
    ../share/altair-gui/images/edit-delete.png \
    ../share/altair-gui/images/encode.png \
    ../share/altair-gui/images/error.png \
    ../share/altair-gui/images/export.png \
    ../share/altair-gui/images/extra.png \
    ../share/altair-gui/images/extraire.png \
    ../share/altair-gui/images/folder-new.png \
    ../share/altair-gui/images/gplv3.png \
    ../share/altair-gui/images/help-contents.png \
    ../share/altair-gui/images/include.png \
    ../share/altair-gui/images/information.png \
    ../share/altair-gui/images/internet-explorer.png \
    ../share/altair-gui/images/internet-web-browser.png \
    ../share/altair-gui/images/kpresenter.png \
    ../share/altair-gui/images/list-remove.png \
    ../share/altair-gui/images/manager.png \
    ../share/altair-gui/images/mode_Project.png \
    ../share/altair-gui/images/msg.png \
    ../share/altair-gui/images/parameter.png \
    ../share/altair-gui/images/play.png \
    ../share/altair-gui/images/print.png \
    ../share/altair-gui/images/process-stop.png \
    ../share/altair-gui/images/process.png \
    ../share/altair-gui/images/project-open.png \
    ../share/altair-gui/images/rapport.png \
    ../share/altair-gui/images/restore.png \
    ../share/altair-gui/images/retrieve.png \
    ../share/altair-gui/images/show-maximized.png \
    ../share/altair-gui/images/show-normal.png \
    ../share/altair-gui/images/tab-close-other.png \
    ../share/altair-gui/images/tab-new.png \
    ../share/altair-gui/images/video.png \
    ../share/altair-gui/images/view-list-tree.png \
    ../share/altair-gui/images/view-refresh.png \
    ../share/altair-gui/images/warning.png \
    ../share/altair-gui/images/window-close.png \
    ../share/altair-gui/images/directory.svg \
    ../share/altair-gui/images/npp.ico







