greaterThan(QT_MAJOR_VERSION, 5)

# Penser à spécifier un répertoire d'exécution sous QtCreator pour Windows juste au-dessu de release ou debug

# Windows : do not use shadow builds
# Note for deployment: mind to place share files into datadir local directory (eg .local/altair-author-gui/...)
# after installing.

# Ensure that library is built before application

CONFIG  += ordered
#static

#use at least Qt5.1 with g++-4.8 for windows

TEMPLATE = app

QT       += core gui xml widgets
QT       -= opengl

TARGET = Altair

VPATH = .

#win32:LIBS += -L../win32
#linux:LIBS += -L../linux

DEFINES += DEBUG QT_NO_OPENGL  STATIC  REGEX_PARSING_FOR_HEADERS
#MINIMAL

windows:RC_ICONS = neptune.ico

#linux: DEFINES += LOCAL_BINPATH

linux:   QMAKE_CXXFLAGS += -std=c++11 -march=corei7-avx -O3
#windows: QMAKE_CXXFLAGS += /Ox /MP
windows: QMAKE_CXXFLAGS +=   -std=gnu++11

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
    elemParser.cpp

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
    elemParser.hpp


RESOURCES += \
    ../share/altair-gui/altair-gui.qrc
