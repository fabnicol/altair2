greaterThan(QT_MAJOR_VERSION, 5)
# Mettre Git\bin dans le PATH systématiquement
# utiliser au moins Qt5 et g++-5.1 sous windows

if (win32|linux) {
  message("Système d'exploitation :  $$(OS)")
} else {
  error("Le système d'exploitation doit être soit Windows soit linux")
}

windows {
  GIT_VERSION = $$system(git --version | find \"git version\")
  CXX_VERSION = $$system($$QMAKE_CXX --version | findstr \"5.[0-9]\")
  DEFINES += LOCAL_BINPATH
}

linux {
  GIT_VERSION = $$system(git --version | grep -e \"git version\")
  CXX_VERSION = $$system($$QMAKE_CXX --version | grep -e '5.[0-9]')
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

QT       += core gui xml widgets
            #webkitwidgets : �  décommenter si STATIC n'est pas défini c-dessous. Nécessite libWebKitWidgets

QT       -= opengl
QT       -= openssl

TARGET = Altair

VPATH = .

DEFINES += PROGRESSBAR_TIMEOUT=200    # Intervalle d'incrémentation de la barre de progression en millisecondes
                            # Un timeout plus élevé diminue l'overhead mais rend la la progression moins fluide.
                            # Le minimum (timeout = 0) donne des progressions fluides mais augmente de 10 % la durée d'exec.

DEFINES += MAXIMUM_CONSOLE_OUTPUT=800   #Pour limiter le volume de lignes de la cosole et éviter des plantages ou ralentissements en cas de verbosité excessive

DEFINES += COMMANDLINE_CONSOLE_OUTPUT          \        # Générer la ligne de commande en console (verbeux)
           GUI_OUTPUT                                   # Balises d'identification des lignes de la console : mode GUI

DEFINES += QT_NO_OPENGL \
           STATIC\                                      # �  utiliser pour lancer le navigateur internet par défaut plustôt qu'un navigateur interne
           LOCAL_BINPATH \                              # chemins d'exécution définis par rapport �  l'emplacement de l'exécutable
           REGEX_PARSING_FOR_HEADERS \                  # utiliser les expressions régulières de c++ (g++ 5.1 au moins)
           USE_RIGHT_CLICK                              # utiliser un clic droit sur les fichiers pour ajouter, supprimer etc.


windows:RC_ICONS = neptune.ico

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
    flineframe.hpp


RESOURCES += \
    ../share/altair-gui/altair-gui.qrc

   

DISTFILES += \
    ../../A_FAIRE.txt \







