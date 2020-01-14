TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += static
QMAKE_CXXFLAGS += -O3 -fomit-frame-pointer -fexpensive-optimizations -Wall -Wextra -static
QMAKE_LFLAGS +=  -s   -static-libgcc -static-libstdc++
DEFINES += MMAP_PARSING
SOURCES +=   utf82latin1.cpp

