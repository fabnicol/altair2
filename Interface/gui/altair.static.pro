greaterThan(QT_MAJOR_VERSION, 5)

# Windows : do not use shadow builds
# Note for deployment: mind to place share files into datadir local directory (eg .local/altair-author-gui/...)
# after installing.

# Ensure that library is built before application

CONFIG  += ordered #static

#use at least Qt5.1 with g++-4.8 for windows

TEMPLATE = app

QT       += core gui xml widgets 
# webkitwidgets
# Compile QT with -static -no-opengl
QT       -= opengl

TARGET = Altair

VPATH = .

DEFINES += QT_NO_OPENGL \
           STATIC  \ 
           LOCAL_BINPATH \
           REGEX_PARSING_FOR_HEADERS \
           USE_RIGHT_CLICK

windows:RC_ICONS = neptune.ico

linux: QMAKE_CXXFLAGS += -std=gnu++11 -march=native -O3
windows: QMAKE_CXXFLAGS += /Ox /MP
#windows: QMAKE_CXXFLAGS += -std=gnu++11
linux: QMAKE_LFLAGS += -s

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






