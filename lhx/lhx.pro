greaterThan(QT_MAJOR_VERSION, 5)

# Pour une sortie en mode console pure, enlever guiOutput ci-dessous. Sinon l'output console pr�pare
# l'input de la "console de l'interface graphique"

CONFIG += console guiOutput
CONFIG -= app_bundle
CONFIG -= qt
VPATH = .

TEMPLATE = app

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
  GIT_VERSION = $$system(git --version | grep -e \'git version\')
  CXX_VERSION = $$system($$QMAKE_CXX --version | grep -e '5.[0-9]')
}


if (!isEmpty(GIT_VERSION)) {
    message( "Version de git : $$GIT_VERSION" )
} else {
  #  error( "Git doit �tre install�" )
}


if (!isEmpty(CXX_VERSION)){
    message( "Version du compilateur : $$CXX_VERSION" )
} else {
    error( "Le compilateur doit �tre GNU g++, dont la version doit �tre au moins 5.1" )
}


CONFIG(guiOutput) {
  DEFINES +=  GUI_TAG_MESSAGES                   # d�finir pour que les sorties des messages soient format�es pour que l'interface graphique les transforme en ic�ne.
}

DEFINES +=  WARNING_LIMIT=5   # nombre maximum d'avertissement par fichier

DEFINES += __GNUC_EXTENSION \
           _REENTRANT \
           _GNU_SOURCE \
           __STDC_LIMIT_MACROS \
           __STDC_FORMAT_MACROS

                                            # DEFINES += STRICT  pour un validateur qui retourne � la premi�re erreur.
DEFINES += \#NO_REGEX \                       # Pas d'analyse du texte par expression r�guli�re
        GCC_REGEX \                        # Utiliser les expressions r�guli�res de C++
        WAIT_FOR_LOCK  \                    # insiter jusqu'� acqu�rir les mutex dans les fils d'ex�cution. Peut entra�ner des "output freeze" en cas de forte
                        \                   # charge I/O.
        NO_DEBUG \                          # ne pas g�n�rer de messages de d�bogage avanc�
#        TOLERANT_TAG_HIERARCHY \           # ordre des balises : tol�rance envers des permutations de m�me niveau
        FULL_PREALLOCATION_TEST \           # calcul des besoins de m�moire : pr�cision maximale (sinon : moindre)
        PREALLOCATE_ON_HEAP \               # pr�allouer le vecteur sur le tas pour le tableau des allocations de m�moire (sinon : tableau C sur la pile)
        DECIMAL_NON_EN \                    # compilation pour des s�parateurs d�cimaux diff�rents de '.'
        GENERATE_RANK_SIGNAL \              # chaque fois qu'un fichier est trait�, un signal externe est �mis (rang dans un fichier rank sous AppData\Local\Altair).
                             \              # n'est utile que lorsqu'une interface graphique est connect�e. peut ralentir l'application de 1 � 5 %.
        FGETC_PARSING                       # parcourir les fichiers par ifstream (C++)
#MMAP_PARSING                               # parcourir les fichiers par mappage m�moire (C, unix).



DEVROOT = $$PWD/../..

# Ins�rer ici le nom du r�pertoire contenant dans include/ et lib/ les d�pendances syst�me
# Ce compilateur doit �tre adjacent aux sources sous Windows

QMAKE_CXXFLAGS = -pipe -m64 -std=gnu++14 -march=native

# Sous linux penser � installer libxml2-dev. Ceci n'est pas test�.

CONFIG(debug) {
    QMAKE_CXXFLAGS +=  -O1
} else {
    QMAKE_LFLAGS += -s
    QMAKE_CXXFLAGS += -O3 -fexpensive-optimizations -fomit-frame-pointer
}

INCLUDEPATH += ../Interface/gui

windows {

  COMPILER_DIR = mingw64-5.2
  INCLUDEPATH += $$DEVROOT/$$COMPILER_DIR/include
  LIBS = -L$$DEVROOT/$$COMPILER_DIR/lib -lxml2.dll -pthread


} else {
  INCLUDEPATH += /usr/include/libxml2
  LIBS = -L/usr/lib/lib64 -L/usr/lib/x86_64-linux-gnu -lxml2 -pthread
}

SOURCES += \ 
    fonctions_auxiliaires.cpp \
    main.cpp \
    table.cpp \
    validator.cpp \
    ligne_paye.cpp

HEADERS += \
    fonctions_auxiliaires.hpp \
    table.hpp \
    validator.hpp \
    ligne_paye.hpp \
    ../Interface/gui/tags.h \
    entete.hpp

