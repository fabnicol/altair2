greaterThan(QT_MAJOR_VERSION, 5)

# sous linux mieux vaut compiler en statique en raison du nombre de dépendances
# utiliser au moins Qt5.1 et g++-4.9.2 sous windows

CONFIG  += ordered static



TEMPLATE = app

QT       += core gui xml widgets #webkitwidgets

# Compile QT with -static -no-opengl

QT       -= opengl

TARGET = Altair

VPATH = .

DEFINES += STATIC

DEFINES += OVERVALUE_DIRSIZE_SHARE_COEFFICIENT=1.5 \    # Une estimation du ration taille des fichiers xhl/taille des bases csv
           COMMANDLINE_CONSOLE_OUTPUT                   # Générer la ligne de commande en console (verbeux)

DEFINES += QT_NO_OPENGL \
           LOCAL_BINPATH \                              # chemins d'exécution définis par rapport à l'emplacement de l'exécutable
           REGEX_PARSING_FOR_HEADERS \                  # utiliser les expressions régulières de c++ (g++ 5.1 au moins)
           USE_RIGHT_CLICK                              # utiliser un clic droit sur les fichiers pour ajouter, supprimer etc.


windows:RC_ICONS = neptune.ico

QMAKE_CXXFLAGS += -std=gnu++11                         # obligatoire
QMAKE_CXXFLAGS += -march=native -O3 -pipe -m64         # facultatif
if (! debug)
   QMAKE_LFLAGS   += -s

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
    ../../A_FAIRE.txt






