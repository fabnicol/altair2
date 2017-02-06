

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Avert
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS = -Os -march=core-avx2
QMAKE_LFLAGS += -s
RESOURCES += \
    avert.qrc \
    avert.qrc

DISTFILES += \
    ../../share/altair-gui/images/altair.png
