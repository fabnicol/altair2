# Encodage : CP-1252

greaterThan(QT_MAJOR_VERSION, 5)

if (win32|linux) {
  message("SystÃ¨me d'exploitation :  $$(OS)")
} else {
  error("Le systÃ¨me d'exploitation doit Ãªtre soit Windows soit linux")
}

#windows {
#  GIT_VERSION = $$system(git --version | find \"git version\")
#  CXX_VERSION = $$system($$QMAKE_CXX --version | findstr \"5.[0-9]\")
#}

linux {
  GIT_VERSION = $$system(git --version | grep -e \'git version\')
  CXX_VERSION = $$system($$QMAKE_CXX --version | grep -e '5.[0-9]')
}


if (!isEmpty(GIT_VERSION)) {
    message( "Version de git : $$GIT_VERSION" )
} else {
  #  error( "Git doit Ãªtre installÃ©" )
}


#if (!isEmpty(CXX_VERSION)){
#    message( "Version du compilateur : $$CXX_VERSION" )
#} else {
#    error( "Le compilateur doit Ãªtre GNU g++, dont la version doit Ãªtre au moins 5.1" )
#}

# Pour une sortie en mode console pure, enlever guiOutput ci-dessous. Sinon l'output console prÃ©pare
# l'input de la "console de l'interface graphique"

CONFIG += console guiOutput
CONFIG -= app_bundle
CONFIG -= qt

CONFIG(guiOutput) {
  DEFINES +=  GUI_TAG_MESSAGES                   # dÃ©finir pour que les sorties des messages soient formatÃ©es pour que l'interface graphique les transforme en icÃ´ne.
}

QMAKE_CXXFLAGS_RELEASE -= -O2

CONFIG(debug, debug|release) {

QMAKE_CXXFLAGS += -O1

} else {

QMAKE_LFLAGS += -s
QMAKE_CXXFLAGS += -O3 -fexpensive-optimizations -fomit-frame-pointer
}

VPATH = .

TEMPLATE = app


DEFINES +=  WARNING_LIMIT=5  \         # nombre maximum d'avertissement par fichier
            MAX_NB_AGENTS=30000 \      # nombre maximum de bulletins par mois
            MAX_LIGNES_PAYE=100 \      # nombre maximum de lignes de paye par bulletin
            TYPE_LOOP_LIMIT=10 \       # nombre de "rembobinages des drapeaux de catÃ©gories (voir ligne_paye.cpp,
            MAX_STRING_LENGTH=200 \    # taille maximum des strings pour la conversion latin-1
            MAX_MEMORY_SHARE=0.5  \    # part maximum de la mÃ©moire vive disponible consommÃ©e par dÃ©faut (si --memshare n'est pas prÃ©cisÃ©)
            AVERAGE_RAM_DENSITY=15 \    # constante empirique dÃ©terminant la quantitÃ© de mÃ©moire nÃ©cessitÃ©e par 1 unitÃ© de mÃ©moire de fichier xhl en cours de traitement.
            USERPROFILE=\\\"HOME\\\" \         # pour la barre de progrÃ¨s. Windows: "USERPROFILE"
            #MEMORY_DEBUG  \
            LOCALDATA=\\\"/AppData/rank\\\"    # Windows

DEFINES += __GNUC_EXTENSION \
           _REENTRANT \
           _GNU_SOURCE \
           __STDC_LIMIT_MACROS \
           __STDC_FORMAT_MACROS \

                                            # DEFINES += STRICT  pour un validateur qui retourne Ã  la premiÃ¨re erreur.
DEFINES += \#NO_REGEX \                     # Pas d'analyse du texte par expression rÃ©guliÃ¨re
        GCC_REGEX \                         # Utiliser les expressions rÃ©guliÃ¨res de C++. Attention dÃ©sactiver cette valeur casse les analyse sous R.
        WAIT_FOR_LOCK  \                    # insiter jusqu'Ã  acquÃ©rir les mutex dans les fils d'exÃ©cution. Peut entraÃ®ner des "output freeze" en cas de forte
                        \                   # charge I/O.
        NO_DEBUG \                          # ne pas gÃ©nÃ©rer de messages de dÃ©bogage avancÃ©
#       USE_ICONV \                         # pour Windows uniquement, si l'on n'est pas satisfait du hack de pseudo-conversion UTF-8 vers Latin-1
                   \                        # alors on peut utiliser iconv pour une conversion plus propre.
#       TOLERANT_TAG_HIERARCHY \            # ordre des balises : tolÃ©rance envers des permutations de mÃªme niveau
        FULL_PREALLOCATION_TEST \           # calcul des besoins de mÃ©moire : prÃ©cision maximale (sinon : moindre)
        PREALLOCATE_ON_HEAP \               # prÃ©allouer le vecteur sur le tas pour le tableau des allocations de mÃ©moire (sinon : tableau C sur la pile)
        DECIMAL_NON_EN \                    # compilation pour des sÃ©parateurs dÃ©cimaux diffÃ©rents de '.'
        GENERATE_RANK_SIGNAL \              # chaque fois qu'un fichier est traitÃ©, un signal externe est Ã©mis (rang dans un fichier rank sous AppData\Local\Altair).
                             \              # n'est utile que lorsqu'une interface graphique est connectÃ©e. peut ralentir l'application de 1 Ã  5 %.
        FGETC_PARSING    \                  # parcourir les fichiers par ifstream (C++)
#       STRINGSTREAM_PARSING  \             # mise en mÃ©moire vive des fichiers de paye par ostringstream (plus de mÃ©moire vive ; accÃ©lÃ¨re beaucoup le 1er traitement sous Windows)
#       MMAP_PARSING           \            # parcourir les fichiers par mappage mÃ©moire (C, unix uniquement, aucun avantage Ã©vident).
        OFSTREAM_TABLE_OUTPUT               # enregistrer les lignes de paye ligne Ã  ligne sur la base. Plus robuste et moins de mÃ©moire mais plus lent sous Windows

# Nota : dÃ©finir au moins un des suivants : STRINGSTREAM_PARSING MMAP_PARSING FGETC_PARSING
# Sous windows la meilleure configuration de premiÃ¨re exÃ©cution est STRINGSTREAM_PARSING pour un disque non-SSD seulement.
# STRINGSTREAM_PARSING est beaucoup moins performant pour les SSD ou quand les volumes sont grands.
# Ne pas dÃ©finit OFSTREAM_TABLE_OUTPUT fait gagner environ 25 % Ã  30 % de temps d'exÃ©cution sous Windows.
# Pour Ã©conomiser de la mÃ©moire prÃ©fÃ©rer FGETC_PARSING et dÃ©finir OFSTREAM_TABLE_OUTPUT

DEVROOT = $$PWD/../..

# InsÃ©rer ici le nom du rÃ©pertoire contenant dans include/ et lib/ les dÃ©pendances systÃ¨me
# Ce compilateur doit Ãªtre adjacent aux sources sous Windows

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS += -pipe -m64 -std=gnu++14
#QMAKE_CXXFLAGS += -march=core2
QMAKE_CXXFLAGS += -march=core-avx2

# Sous linux penser Ã  installer libxml2-dev. Ceci n'est pas testÃ©.


INCLUDEPATH += ../Interface/gui

windows {

  COMPILER_DIR = x86_64-6.1.0-release-posix-seh-rt_v5-rev0
  DEFINES += SYSTEM_PATH_SEPARATOR=\"\';\'\"
  INCLUDEPATH += $$DEVROOT/$$COMPILER_DIR/include
  LIBS = -L$$DEVROOT/$$COMPILER_DIR/lib -lxml2.dll -pthread $$(SYSTEMROOT)/System32/psapi.dll
  HEADERS += entete-latin1.hpp

} else {

  DEFINES += SYSTEM_PATH_SEPARATOR=\"\":\"\"
  INCLUDEPATH += /usr/include/libxml2
  LIBS = -L/usr/lib/lib64 -L/usr/lib/x86_64-linux-gnu -lxml2 -pthread
  HEADERS += entete.hpp
}

SOURCES += \ 
    fonctions_auxiliaires.cpp \
    main.cpp \
    table.cpp \
    validator.cpp \
    ligne_paye.cpp

HEADERS += \
    fonctions_auxiliaires.hpp \
    table.hpp \
    validator.hpp \
    ligne_paye.hpp \
    ../Interface/gui/tags.h

