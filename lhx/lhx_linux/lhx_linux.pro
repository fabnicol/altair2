TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
VPATH = .
DEFINES += GCC_REGEX \
	__GNUC_EXTENSION \
	_REENTRANT \
	_GNU_SOURCE \
	__STDC_LIMIT_MACROS \
	__STDC_FORMAT_MACROS \
	NO_DEBUG \
	TOLERANT \

QMAKE_CXXFLAGS = -pipe -m64 -std=gnu++11 -march=native -fexceptions -fomit-frame-pointer -O3 -fexpensive-optimizations  

INCLUDEPATH += /usr/include/libxml2
LIBS = -L/usr/lib/x86_64-linux-gnu -lxml2 -pthread

SOURCES += \ 
    ../fonctions_auxiliaires.cpp \
    ../main.cpp \
    ../table.cpp \
    ../validator.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    ../fonctions_auxiliaires.hpp \
    ../table.hpp \
    ../validator.hpp

