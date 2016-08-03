greaterThan(QT_MAJOR_VERSION, 5)

# Windows : do not use shadow builds
# Note for deployment: mind to place share files into datadir local directory (eg .local/altair-author-gui/...)
# after installing.

# Ensure that library is built before application

CONFIG  += ordered static

#use at least Qt5.1 with g++-4.8 for windows

TEMPLATE = app

QT       += core gui xml widgets
QT       -= opengl

TARGET = Altair

VPATH = .

#win32:LIBS += -L../win32
#linux:LIBS += -L../linux

DEFINES +=  QT_NO_OPENGL  STATIC MINIMAL LOCAL_BINPATH

windows:RC_ICONS = neptune.ico
windows: QMAKE_CXXFLAGS += /Ox /MP
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
    tags.h \
    templates.h \
    altair-gui.h \
    probe.h


RESOURCES += \
    ../share/altair-gui/altair-gui.qrc

   






