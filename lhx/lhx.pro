greaterThan(QT_MAJOR_VERSION, 5)

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
  #  error( "Git doit �tre install�" )
}


if (!isEmpty(CXX_VERSION)){
    message( "Version du compilateur : $$CXX_VERSION" )
} else {
    error( "Le compilateur doit �tre GNU g++, dont la version doit �tre au moins 5.1" )
}


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

DEFINES += NO_REGEX \#GCC_REGEX \                      # Utiliser les expressions r�guli�res de C++
        NO_DEBUG \                          # ne pas g�n�rer de messages de d�bogage avanc�
#        TOLERANT_TAG_HIERARCHY \            # ordre des balises : tol�rance envers des permutations de m�me niveau
        FULL_PREALLOCATION_TEST \           # calcul des besoins de m�moire : pr�cision maximale (sinon : moindre)
        PREALLOCATE_ON_HEAP \               # pr�allouer le vecteur sur le tas pour le tableau des allocations de m�moire (sinon : tableau C sur la pile)
        DECIMAL_NON_EN \                    # compilation pour des s�parateurs d�cimaux diff�rents de '.'
        FGETC_PARSING                       # parcourir les fichiers par ifstream (C++)
#MMAP_PARSING                               # parcourir les fichiers par mappage m�moire (C, unix).

DEVROOT = $$PWD/../..

# Ins�rer ici le nom du r�pertoire contenant dans include/ et lib/ les d�pendances syst�me
# Ce compilateur doit �tre adjacent aux sources sous Windows

QMAKE_CXXFLAGS = -pipe -m64 -std=gnu++11 -march=native -fno-inline -fexceptions -frtti  -O3 -fexpensive-optimizations -fomit-frame-pointer

# Sous linux penser � installer libxml2-dev. Ceci n'est pas test�.

windows {

  COMPILER_DIR = mingw64-5.2
  INCLUDEPATH += $$DEVROOT/$$COMPILER_DIR/include
  LIBS = -L$$DEVROOT/$$COMPILER_DIR/lib -lxml2.dll -pthread
  release: QMAKE_LFLAGS += -s

} else {
  INCLUDEPATH += /usr/include/libxml2
  LIBS = -L/usr/lib/lib64 -L/usr/lib/x86_64-linux-gnu -lxml2 -pthread
}

SOURCES += \ 
    fonctions_auxiliaires.cpp \
    main.cpp \
    table.cpp \
    validator.cpp

HEADERS += \
    fonctions_auxiliaires.hpp \
    table.hpp \
    validator.hpp

