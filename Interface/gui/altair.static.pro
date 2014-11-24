greaterThan(QT_MAJOR_VERSION, 5)

# Windows : do not use shadow builds
# Note for deployment: mind to place share files into datadir local directory (eg .local/altair-author-gui/...)
# after installing.

# Ensure that library is built before application

CONFIG  += ordered static

#use at least Qt5.1 with g++-4.8 for windows

TEMPLATE = app

QT       += core gui xml widgets 

TARGET = altair-gui

VPATH = .

DEFINES += DEBUG STATIC

#linux: DEFINES += LOCAL_BINPATH

QMAKE_CXXFLAGS += -std=c++11 -march=core2 -O3

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
    console.cpp \ 
    probe.cpp

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
    console.h \
    tags.h \
    templates.h \
    altair-gui.h \
    probe.h


RESOURCES += \
    ../share/altair-gui/altair-gui.qrc 

   






