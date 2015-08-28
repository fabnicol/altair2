greaterThan(QT_MAJOR_VERSION, 5)

# Windows : do not use shadow builds
# Note for deployment: mind to place share files into datadir local directory (eg .local/altair-author-gui/...)
# after installing.

# Ensure that library is built before application

CONFIG  += ordered 

#use at least Qt5.1 with g++-4.8 for windows

TEMPLATE = app

QT       += core gui xml widgets 
#webkitwidgets
QT       -= opengl

TARGET = Altair

VPATH = .

#win32:LIBS += -L../win32
#linux:LIBS += -L../linux

DEFINES += DEBUG QT_NO_OPENGL STATIC

win32: DEFINES += LOCAL_BINPATH 

QMAKE_CXXFLAGS += -std=c++11 -march=corei7 -O3
#QMAKE_CXXFLAGS += /Ox /MP

SOURCES += \
    options.cpp \
    mainwindow.cpp \
    fwidgets.cpp \
    fstring.cpp \
    flistframe.cpp \
    altair.cpp 
  

HEADERS  += \
    options.h \
    fwidgets.h \
    fstring.h \
    flistframe.h \
    common.h \
    altair.h \
   


RESOURCES += \
    ../share/altair-gui/altair-gui.qrc 

   






