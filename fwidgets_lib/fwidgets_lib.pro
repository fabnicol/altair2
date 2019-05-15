
#-------------------------------------------------
#
# Project created by QtCreator 2017-09-15T15:18:29
#
#-------------------------------------------------


VERSION_TAG = $$system(cat ../VERSION)
DEFINES +=  VERSION=\\\"$$VERSION_TAG\\\"
message("Version :  $$VERSION_TAG")
QMAKE_CXX = /usr/bin/g++-8.2.0
#QMAKE_LINK = /usr/bin/g++-5
greaterThan(QT_MAJOR_VERSION, 5)

# utiliser au moins Qt5 et g++-5.1
# ENCODAGE : UTILISER UTF-8 PARTOUT, y compris sur le fichier .pro.

if (linux) {
  message("Système d'exploitation linux")
} else {
  error("Le système d'exploitation doit être linux")
}

GIT_VERSION = $$system(git --version | grep -e \"git version\")
CXX_VERSION = $$system($$QMAKE_CXX --version | grep -e '[5-9].[0-9]')

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



QT       += widgets
QT       -= opengl
QT       -= openssl


TARGET = fwidgets_lib
TEMPLATE = lib
CONFIG += staticlib
DEFINES += FWIDGETS_LIB_LIBRARY
DEFINES += PROGRESSBAR_TIMEOUT=200
DEFINES += HAVE_APPLICATION GUI_OUTPUT
# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
           LOCAL_BINPATH \                              # chemins d'exécution définis par rapport l'emplacement de l'exécutable
           USE_RIGHT_CLICK  \                           # utiliser un clic droit sur les fichiers pour ajouter, supprimer etc.

QMAKE_LFLAGS += -static-libgcc -static-libstdc++
QMAKE_CXXFLAGS += -std=gnu++17  -O3 -fomit-frame-pointer -fexpensive-optimizations                        # obligatoire
QMAKE_CXXFLAGS += -march=core-avx2  -pipe -m64 -std=c++17        # facultatif
LIBS += -lstdc++fs

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    flineframe.cpp \
    forms.cpp \
    fstring.cpp \
    fwidgets.cpp \
    tools.cpp \
    flistframe.cpp

HEADERS += \
        fwidgets_lib_global.h \ 
    flineframe.h \
    flistframe.h \
    forms.h \
    fstring.h \
    enums.h \
    fwidgets.h \
    tags.h \
    tools.h \
    templates.h \
    custom.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES +=
