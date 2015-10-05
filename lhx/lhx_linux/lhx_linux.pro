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
FGETC_PARSING
#MMAP_PARSING

linux: QMAKE_CXXFLAGS = -pipe -m64 -std=gnu++11 -march=native -fexceptions -fomit-frame-pointer -O3 -fexpensive-optimizations

# sous MSVC
#windows : QMAKE_CXXFLAGS =/Ox /MP
# sous MINGW/GCC

QMAKE_CXXFLAGS = -pipe -m64 -std=gnu++11 -march=native -fexceptions -fomit-frame-pointer -O3 -fexpensive-optimizations

linux: INCLUDEPATH += /usr/include/libxml2
windows: INCLUDEPATH += E:/mingw64/include

linux: LIBS = -L/usr/lib/x86_64-linux-gnu -lxml2 -pthread
windows: LIBS = E:/mingw64/lib/libxml2.dll.a

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

