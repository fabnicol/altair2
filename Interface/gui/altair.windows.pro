greaterThan(QT_MAJOR_VERSION, 5)
# Mettre Git\bin dans le PATH syst�matiquement
# utiliser au moins Qt5 et g++-5.1 sous windows

if (win32|linux) {
  message("Syst�me d'exploitation :  $$(OS)")
} else {
  error("Le syst�me d'exploitation doit �tre soit Windows soit linux")
}

windows {
  GIT_VERSION = $$system(git --version | find \"git version\")
  CXX_VERSION = $$system($$QMAKE_CXX --version | findstr \"5.[0-9]\")
}

linux {
  GIT_VERSION = $$system(git --version | grep -e 'git version')
  CXX_VERSION = $$system($$QMAKE_CXX --version | grep -e '5.[0-9]')
}

if (!isEmpty(GIT_VERSION)) {
    message( "Version de git : $$GIT_VERSION" )
} else {
#    error( "Git doit �tre install�" )
}


if (!isEmpty(CXX_VERSION)){
    message( "Version du compilateur : $$CXX_VERSION" )
} else {
    error( "Le compilateur doit �tre GNU g++, dont la version doit �tre au moins 5.1" )
}



CONFIG  += ordered
CONFIG(debug, debug|release) {
  QMAKE_LFLAGS   += -L$$(QTDIR)/bin   # ne devrait pas en principe �tre rajout� mais...qmake est capricieux !
} else {
  CONFIG += static
}

TEMPLATE = app

QT       += core gui xml widgets
            #webkitwidgets : � d�commenter si STATIC n'est pas d�fini c-dessous. N�cessite libWebKitWidgets

QT       -= opengl
QT       -= openssl

TARGET = Altair

VPATH = .

DEFINES += PROGRESSBAR_TIMEOUT=200    # Intervalle d'incr�mentation de la barre de progression en millisecondes
                            # Un timeout plus �lev� diminue l'overhead mais rend la la progression moins fluide.
                            # Le minimum (timeout = 0) donne des progressions fluides mais augmente de 10 % la dur�e d'exec.

DEFINES += MAXIMUM_CONSOLE_OUTPUT=800   #Pour limiter le volume de lignes de la cosole et �viter des plantages ou ralentissements en cas de verbosit� excessive

DEFINES += COMMANDLINE_CONSOLE_OUTPUT          \        # G�n�rer la ligne de commande en console (verbeux)
           GUI_OUTPUT                                   # Balises d'identification des lignes de la console : mode GUI

DEFINES += QT_NO_OPENGL \
           STATIC\                                      # � utiliser pour lancer le navigateur internet par d�faut plust�t qu'un navigateur interne
           LOCAL_BINPATH \                              # chemins d'ex�cution d�finis par rapport � l'emplacement de l'ex�cutable
           REGEX_PARSING_FOR_HEADERS \                  # utiliser les expressions r�guli�res de c++ (g++ 5.1 au moins)
           USE_RIGHT_CLICK                              # utiliser un clic droit sur les fichiers pour ajouter, supprimer etc.


windows:RC_ICONS = neptune.ico

QMAKE_CXXFLAGS += -std=gnu++11                         # obligatoire
QMAKE_CXXFLAGS += -march=native -O3 -fomit-frame-pointer -fexpensive-optimizations -pipe -m64         # facultatif


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







