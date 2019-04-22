# Copyright Cour des comptes, 2017
# Contributeur :
# Fabrice Nicol, années 2012 à 2017
# fabrice.nicol@crtc.ccomptes.fr
# 
# Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
# produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
# en vigueur à compter de l'année 2008.
# 
# Ce logiciel est régi par la licence CeCILL soumise au droit français et
# respectant les principes de diffusion des logiciels libres. Vous pouvez
# utiliser, modifier et/ou redistribuer ce programme sous les conditions
# de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
# sur le site "http://www.cecill.info".
# 
# En contrepartie de l'accessibilité au code source et des droits de copie,
# de modification et de redistribution accordés par cette licence, il n'est
# offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
# seule une responsabilité restreinte pèse sur l'auteur du programme, le
# titulaire des droits patrimoniaux et les concédants successifs.
# 
# A cet égard l'attention de l'utilisateur est attirée sur les risques
# associés au chargement, à l'utilisation, à la modification et/ou au
# développement et à la reproduction du logiciel par l'utilisateur étant
# donné sa spécificité de logiciel libre, qui peut le rendre complexe à
# manipuler et qui le réserve donc à des développeurs et des professionnels
# avertis possédant des connaissances informatiques approfondies. Les
# utilisateurs sont donc invités à charger et tester l'adéquation du
# logiciel à leurs besoins dans des conditions permettant d'assurer la
# sécurité de leurs systèmes et ou de leurs données et, plus généralement,
# à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
# 
# Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
# pris connaissance de la licence CeCILL, et que vous en avez accepté les
# termes.
# 
# 
greaterThan(QT_MAJOR_VERSION, 5)

# Windows : do not use shadow builds
# Note for deployment: mind to place share files into datadir local directory (eg .local/altair-author-gui/...)
# after installing.

# Ensure that library is built before application

CONFIG  += ordered 

#use at least Qt5.1 with g++-4.8 for windows

TEMPLATE = app

QT       += core gui xml widgets # webkitwidgets
QT       -= opengl

TARGET = Altair

VPATH = .

#win32:LIBS += -L../win32
#linux:LIBS += -L../linux

DEFINES += DEBUG QT_NO_OPENGL LOCAL_BINPATH STATIC



linux: QMAKE_CXXFLAGS += -std=gnu++11 -march=native -O3
windows: QMAKE_CXXFLAGS += /Ox /MP

SOURCES += \
    options.cpp \
    mainwindow.cpp \
    altair.cpp \
    common.cpp \
    main.cpp \
     xmlparser.cpp \
    highlighter.cpp \
    run.cpp

HEADERS  += \
    options.h \
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

   






