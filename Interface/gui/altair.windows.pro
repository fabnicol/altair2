# L'ensemble des fichiers source doit Ãªtre encodÃ© en UTF-8 sauf celui-ci qui peut l'Ãªtre en Latin-1

greaterThan(QT_MAJOR_VERSION, 5)

# utiliser au moins Qt5 et g++-5.1 sous windows

if (win32|linux) {
  message("SystÃ¨me d'exploitation :  $$(OS)")
} else {
  error("Le systÃ¨me d'exploitation doit Ãªtre soit Windows soit linux")
}

windows {
  GIT_VERSION = $$system(git --version | find \"git version\")
  CXX_VERSION = $$system($$QMAKE_CXX --version | findstr \"[5-9].[0-9]\")
  DEFINES += LOCAL_BINPATH
}

linux {
  GIT_VERSION = $$system(git --version | grep -e \"git version\")
  CXX_VERSION = $$system($$QMAKE_CXX --version | grep -e '[5-9].[0-9]')
}

if (!isEmpty(GIT_VERSION)) {
    message( "Version de git : $$GIT_VERSION" )
} else {
#    error( "Git doit Ãªtre installÃ©" )
}


if (!isEmpty(CXX_VERSION)){
    message( "Version du compilateur : $$CXX_VERSION" )
} else {
    error( "Le compilateur doit Ãªtre GNU g++, dont la version doit Ãªtre au moins 5.1" )
}



CONFIG  += ordered
CONFIG(debug, debug|release) {
  QMAKE_LFLAGS   += -L$$(QTDIR)/bin   # ne devrait pas en principe Ãªtre rajoutÃ© mais...qmake est capricieux !
} else {
  CONFIG += static
}

TEMPLATE = app

QT       += core gui xml widgets
            #webkitwidgets : Ã  dÃ©commenter si STATIC n'est pas dÃ©fini c-dessous. NÃ©cessite libWebKitWidgets

QT       -= opengl
QT       -= openssl

TARGET = Altair

VPATH = .

#DEFINES += MINIMAL

DEFINES += PROGRESSBAR_TIMEOUT=400    # Intervalle d'incrÃ©mentation de la barre de progression en millisecondes
                            # Un timeout plus Ã©levÃ© diminue l'overhead mais rend la la progression moins fluide.
                            # Le minimum (timeout = 0) donne des progressions fluides mais augmente de 10 % la durÃ©e d'exec.

DEFINES += MAXIMUM_CONSOLE_OUTPUT=800   #Pour limiter le volume de lignes de la cosole et Ã©viter des plantages ou ralentissements en cas de verbositÃ© excessive

DEFINES += COMMANDLINE_CONSOLE_OUTPUT          \        # GÃ©nÃ©rer la ligne de commande en console (verbeux)
           GUI_OUTPUT                                   # Balises d'identification des lignes de la console : mode GUI

DEFINES += QT_NO_OPENGL \
           STATIC\                                      # Ã  utiliser pour lancer le navigateur internet par dÃ©faut plustÃ´t qu'un navigateur interne
         #  LOCAL_BINPATH \                             # chemins d'exÃ©cution dÃ©finis par rapport Ã  l'emplacement de l'exÃ©cutable
           REGEX_PARSING_FOR_HEADERS \                  # utiliser les expressions rÃ©guliÃ¨res de c++ (g++ 5.1 au moins)
           USE_RIGHT_CLICK                              # utiliser un clic droit sur les fichiers pour ajouter, supprimer etc.


windows:RC_ICONS = neptune.ico

QMAKE_CXXFLAGS_RELEASE -= -O2

# ajouter -std=gnu++14 si qmake de le fait pas

QMAKE_CXXFLAGS += -O3 -fomit-frame-pointer -fexpensive-optimizations -pipe -m64         # facultatif
#QMAKE_CXXFLAGS += -march=core2
QMAKE_CXXFLAGS += -march=core-avx2

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
    elemParser.cpp \
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
    elemParser.hpp \
    flineframe.hpp \
    version.h


RESOURCES += \
    ../share/altair-gui/altair-gui.qrc

   

DISTFILES += \
    ../../A_FAIRE.txt \
    ../share/altair-gui/images/internet-explorer.png \
    ../share/altair-gui/images/anonymiser.png







