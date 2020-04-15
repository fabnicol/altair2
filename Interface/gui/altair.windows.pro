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
# L'ensemble des fichiers source doit être encodé en UTF-8 sauf celui-ci qui peut l'être en Latin-1

DEFINES +=  CORE2=\\\"\\\"
VERSION_TAG = $$system(type C:\Users\Public\Dev\altair\VERSION)
DEFINES +=  VERSION=\\\"$$VERSION_TAG\\\"
message("Version :  $$VERSION_TAG")
greaterThan(QT_MAJOR_VERSION, 5)

# utiliser au moins Qt5 et g++-5.1 sous windows

if (win32|linux) {
  message("Système d'exploitation :  $$(OS)")
} else {
  error("Le système d'exploitation doit être soit Windows soit linux")
}

windows {
  GIT_VERSION = $$system(git --version | find \"git version\")
  CXX_VERSION = $$system($$QMAKE_CXX --version | findstr \"[5-9].[0-9]\")
  DEFINES += LOCAL_BINPATH
}

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



CONFIG  += ordered
CONFIG(debug, debug|release) {
  QMAKE_LFLAGS   +=  -L$$(QTDIR)/bin   # ne devrait pas en principe être rajouté mais...qmake est capricieux !

} else {
  CONFIG += static
  QMAKE_CXXFLAGS += -O3 -fomit-frame-pointer -fexpensive-optimizations -pipe -m64         # facultatif
  QMAKE_LFLAGS += -s

}

TEMPLATE = app

QT       += core gui xml widgets printsupport
            #webkitwidgets : à  décommenter si STATIC n'est pas défini c-dessous. Nécessite libWebKitWidgets

QT       -= opengl
QT       -= openssl

TARGET = Altair

VPATH = .

#DEFINES += MINIMAL

DEFINES += MAX_FICHIERS_RECENTS=5     # Nombre de fichiers récents maximum

DEFINES += PROGRESSBAR_TIMEOUT=400    # Intervalle d'incrémentation de la barre de progression en millisecondes
                                      # Un timeout plus élevé diminue l'overhead mais rend la la progression moins fluide.
                                      # Le minimum (timeout = 0) donne des progressions fluides mais augmente de 10 % la durée d'exec.

DEFINES += MAXIMUM_CONSOLE_OUTPUT=800   #Pour limiter le volume de lignes de la cosole et éviter des plantages ou ralentissements en cas de verbosité excessive

DEFINES += COMMANDLINE_CONSOLE_OUTPUT          \        # Générer la ligne de commande en console (verbeux)
           GUI_OUTPUT                                   # Balises d'identification des lignes de la console : mode GUI

DEFINES += QT_NO_OPENGL \
           STATIC\                                      # Ã  utiliser pour lancer le navigateur internet par défaut plustÃ´t qu'un navigateur interne
         #  LOCAL_BINPATH \                             # chemins d'exécution définis par rapport Ã  l'emplacement de l'exécutable
           REGEX_PARSING_FOR_HEADERS \                  # utiliser les expressions régulières de c++ (g++ 5.1 au moins)
           USE_RIGHT_CLICK \                            # utiliser un clic droit sur les fichiers pour ajouter, supprimer etc.
         # USE_AVERT                                    # DEPRECATED : avertissement au montage CDROM
           HAVE_APPLICATION

DEFINES += SCRIPT_DIR=\\\"Tests/Exemple/\\\"  # Dossier des scripts R

windows:RC_ICONS = neptune.ico
INCLUDEPATH += C:/Users/Public/Dev/altair2/fwidgets_lib
QMAKE_CXXFLAGS_RELEASE -= -O2

# ajouter -std=gnu++14 si qmake de le fait pas

QMAKE_CXXFLAGS += -march=core-avx2 -std=gnu++14
#QMAKE_CXXFLAGS += -march=native -mtune=native -std=gnu++14
LIBS += C:/Users/Public/Dev/altair2/fwidgets_lib/libfwidgets_lib.a

DEFINES += HAS_CPP17

SOURCES += \
    options.cpp \
    mainwindow.cpp \
    altair.cpp \
    common.cpp \
    main.cpp \
    xmlparser.cpp \
    highlighter.cpp \
    run.cpp \
    matricules.cpp


HEADERS  += \
    options.h \
    common.h \
    altair.h \
    altair.h \
    highlighter.h \
    altair-gui.h \
    gui_enums.h \
    matricules.h \
    sommaire.h

RESOURCES += \
    ../share/altair-gui/altair-gui.qrc

   

DISTFILES += \
    ../../A_FAIRE.txt \
    ../share/altair-gui/images/internet-web-browser.png







