# L'ensemble des fichiers source doit être encodé en UTF-8 sauf celui-ci qui peut l'être en Latin-1

DEFINES += VERSION=\\\"16.09\\\"  \
           CORE2=\\\"\\\"

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

QT       += core gui xml widgets
            #webkitwidgets : Ã  décommenter si STATIC n'est pas défini c-dessous. Nécessite libWebKitWidgets

QT       -= opengl
QT       -= openssl

TARGET = Altair

VPATH = .

#DEFINES += MINIMAL

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
           USE_RIGHT_CLICK                              # utiliser un clic droit sur les fichiers pour ajouter, supprimer etc.


windows:RC_ICONS = neptune.ico

QMAKE_CXXFLAGS_RELEASE -= -O2

# ajouter -std=gnu++14 si qmake de le fait pas

QMAKE_CXXFLAGS += -march=core-avx2 -std=gnu++14
#QMAKE_CXXFLAGS += -march=native -mtune=native -std=gnu++14

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







