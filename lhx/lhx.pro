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

DEFINES += STRICT

DEFINES += GCC_REGEX \                      # Utiliser les expressions régulières de C++
        NO_DEBUG \                          # ne pas générer de messages de débogage avancé
        TOLERANT_TAG_HIERARCHY \            # ordre des balises : tolérance envers des permutations de même niveau
        FULL_PREALLOCATION_TEST \           # calcul des besoins de mémoire : précision maximale (sinon : moindre)
        PREALLOCATE_ON_HEAP \               # préallouer le vecteur sur le tas pour le tableau des allocations de mémoire (sinon : tableau C sur la pile)
        DECIMAL_NON_EN \                    # compilation pour des séparateurs décimaux différents de '.'
        FGETC_PARSING                       # parcourir les fichiers par ifstream (C++)
#MMAP_PARSING                               # parcourir les fichiers par mappage mémoire (C, unix).


# sous MSVC
# windows : QMAKE_CXXFLAGS =/Ox /MP
# sous MINGW/GCC

QMAKE_CXXFLAGS = -pipe -m64 -std=gnu++11 -march=native -fexceptions -fomit-frame-pointer -O3 -fexpensive-optimizations

linux: INCLUDEPATH += /usr/include/libxml2
windows: INCLUDEPATH += $(PUBLIC)/Dev/mingw64-5.2/include

linux: LIBS = -L/usr/lib/x86_64-linux-gnu -lxml2 -pthread
windows: LIBS = $(PUBLIC)/Dev/mingw64-5.2/lib/libxml2.dll.a
#windows: VPATH += $(USERPROFILE)/Dev/mingw64-5.2.0/bin

SOURCES += \ 
    fonctions_auxiliaires.cpp \
    main.cpp \
    table.cpp \
    validator.cpp

#include(deployment.pri)
#qtcAddDeployment()

HEADERS += \
    fonctions_auxiliaires.hpp \
    table.hpp \
    validator.hpp

