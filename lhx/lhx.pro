TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
VPATH = .

DEFINES += __GNUC_EXTENSION \
           _REENTRANT \
           _GNU_SOURCE \
           __STDC_LIMIT_MACROS \
           __STDC_FORMAT_MACROS

#DEFINES += STRICT

DEFINES += NO_REGEX \#GCC_REGEX \                      # Utiliser les expressions régulières de C++
        NO_DEBUG \                          # ne pas générer de messages de débogage avancé
#        TOLERANT_TAG_HIERARCHY \            # ordre des balises : tolérance envers des permutations de même niveau
        FULL_PREALLOCATION_TEST \           # calcul des besoins de mémoire : précision maximale (sinon : moindre)
        PREALLOCATE_ON_HEAP \               # préallouer le vecteur sur le tas pour le tableau des allocations de mémoire (sinon : tableau C sur la pile)
        DECIMAL_NON_EN \                    # compilation pour des séparateurs décimaux différents de '.'
        FGETC_PARSING                       # parcourir les fichiers par ifstream (C++)
#MMAP_PARSING                               # parcourir les fichiers par mappage mémoire (C, unix).

DEVROOT = $$PWD/../..

# Insérer ici le nom du répertoire contenant dans include/ et lib/ les dépendances système
# Ce compilateur doit être adjacent aux sources sous Windows

COMPILER_DIR = mingw64-5.2

# Options de compilation

# sous MSVC
# windows : QMAKE_CXXFLAGS =/Ox /MP
# sous MINGW/GCC

QMAKE_CXXFLAGS = -pipe -m64 -std=gnu++11 -march=native -fno-inline -O3 -fexpensive-optimizations -pg  #-fomit-frame-pointer

linux: INCLUDEPATH += /usr/include/libxml2
windows: INCLUDEPATH += $$DEVROOT/$$COMPILER_DIR/include

linux: LIBS = -L/usr/lib/lib64 -L/usr/lib/x86_64-linux-gnu -lxml2 -pthread
windows: LIBS = -L$$DEVROOT/$$COMPILER_DIR/lib -lxml2.dll -pthread -pg

#windows: QMAKE_LFLAGS += -s

SOURCES += \ 
    fonctions_auxiliaires.cpp \
    main.cpp \
    table.cpp \
    validator.cpp

HEADERS += \
    fonctions_auxiliaires.hpp \
    table.hpp \
    validator.hpp

