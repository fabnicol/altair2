# Encodage : CP-1252

greaterThan(QT_MAJOR_VERSION, 5)

if (win32|linux) {
  message("Syst�me d'exploitation :  $$(OS)")
} else {
  error("Le syst�me d'exploitation doit �tre soit Windows soit linux")
}

windows {
  GIT_VERSION = $$system(git --version | find \"git version\")
  CXX_VERSION = $$system($$QMAKE_CXX --version | findstr \"5.[0-9]\")
}

linux {
  GIT_VERSION = $$system(git --version | grep -e \'git version\')
  CXX_VERSION = $$system($$QMAKE_CXX --version | grep -e '5.[0-9]')
}


if (!isEmpty(GIT_VERSION)) {
    message( "Version de git : $$GIT_VERSION" )
} else {
  #  error( "Git doit �tre install�" )
}


if (!isEmpty(CXX_VERSION)){
    message( "Version du compilateur : $$CXX_VERSION" )
} else {
    error( "Le compilateur doit �tre GNU g++, dont la version doit �tre au moins 5.1" )
}

# Pour une sortie en mode console pure, enlever guiOutput ci-dessous. Sinon l'output console pr�pare
# l'input de la "console de l'interface graphique"

CONFIG += console guiOutput
CONFIG -= app_bundle
CONFIG -= qt


DEFINES +=  GUI_TAG_MESSAGES                   # d�finir pour que les sorties des messages soient format�es pour que l'interface graphique les transforme en ic�ne.
#DEFINES += COLOR_CONSOLE

QMAKE_CXXFLAGS_RELEASE -= -O2

CONFIG(debug, debug|release) {

QMAKE_CXXFLAGS += -O0

} else {

QMAKE_LFLAGS += -s
QMAKE_CXXFLAGS += -O3 -fexpensive-optimizations -fomit-frame-pointer
}

VPATH = .

TEMPLATE = app


DEFINES +=  WARNING_LIMIT=5  \         # nombre maximum d'avertissement par fichier
            MAX_NB_AGENTS=30000 \      # nombre maximum de bulletins par mois
            MAX_LIGNES_PAYE=100 \      # nombre maximum de lignes de paye par bulletin
            TYPE_LOOP_LIMIT=10 \       # nombre de "rembobinages des drapeaux de cat�gories (voir ligne_paye.cpp,
            MAX_STRING_LENGTH=200 \    # taille maximum des strings pour la conversion latin-1
            MAX_MEMORY_SHARE=0.5  \    # part maximum de la m�moire vive disponible consomm�e par d�faut (si --memshare n'est pas pr�cis�)
            SEGMENT_DIVISION_RATE=2 \
            AVERAGE_RAM_DENSITY=3 \    # constante empirique d�terminant la quantit� de m�moire n�cessit�e par 1 unit� de m�moire de fichier xhl en cours de traitement.
           # MEMORY_DEBUG

DEFINES += __GNUC_EXTENSION \
           _REENTRANT \
           _GNU_SOURCE \
           __STDC_LIMIT_MACROS \
           __STDC_FORMAT_MACROS

                                            # DEFINES += STRICT  pour un validateur qui retourne � la premi�re erreur.
DEFINES += \#NO_REGEX \                     # Pas d'analyse du texte par expression r�guli�re
        GCC_REGEX \                         # Utiliser les expressions r�guli�res de C++. Attention d�sactiver cette valeur casse les analyse sous R.
        WAIT_FOR_LOCK  \                    # insiter jusqu'� acqu�rir les mutex dans les fils d'ex�cution. Peut entra�ner des "output freeze" en cas de forte
                        \                   # charge I/O.
        NO_DEBUG \                          # ne pas g�n�rer de messages de d�bogage avanc�
#       USE_ICONV \                         # pour Windows uniquement, si l'on n'est pas satisfait du hack de pseudo-conversion UTF-8 vers Latin-1
                   \                        # alors on peut utiliser iconv pour une conversion plus propre.
#       TOLERANT_TAG_HIERARCHY \            # ordre des balises : tol�rance envers des permutations de m�me niveau
        FULL_PREALLOCATION_TEST \           # calcul des besoins de m�moire : pr�cision maximale (sinon : moindre)
        PREALLOCATE_ON_HEAP \               # pr�allouer le vecteur sur le tas pour le tableau des allocations de m�moire (sinon : tableau C sur la pile)
        DECIMAL_NON_EN \                    # compilation pour des s�parateurs d�cimaux diff�rents de '.'
        GENERATE_RANK_SIGNAL \              # chaque fois qu'un fichier est trait�, un signal externe est �mis (rang dans un fichier rank sous AppData\Local\Altair).
                             \              # n'est utile que lorsqu'une interface graphique est connect�e. peut ralentir l'application de 1 � 5 %.
#        FGETC_PARSING    \                 # parcourir les fichiers par ifstream (C++)
        STRINGSTREAM_PARSING  \             # mise en m�moire vive des fichiers de paye par ostringstream (plus de m�moire vive ; acc�l�re beaucoup le 1er traitement sous Windows)
#       MMAP_PARSING           \            # parcourir les fichiers par mappage m�moire (C, unix uniquement, aucun avantage �vident).
#       OFSTREAM_TABLE_OUTPUT               # enregistrer les lignes de paye ligne � ligne sur la base. Plus robuste et moins de m�moire mais plus lent sous Windows

# Nota : d�finir au moins un des suivants : STRINGSTREAM_PARSING MMAP_PARSING FGETC_PARSING
# Sous windows la meilleure configuration de premi�re ex�cution est STRINGSTREAM_PARSING pour un disque non-SSD seulement.
# STRINGSTREAM_PARSING est beaucoup moins performant pour les SSD ou quand les volumes sont grands.
# Ne pas d�finir OFSTREAM_TABLE_OUTPUT fait gagner environ 25 % � 30 % de temps d'ex�cution sous Windows.
# Pour �conomiser de la m�moire pr�f�rer FGETC_PARSING et d�finir OFSTREAM_TABLE_OUTPUT

DEVROOT = $$PWD/../..

# Ins�rer ici le nom du r�pertoire contenant dans include/ et lib/ les d�pendances syst�me
# Ce compilateur doit �tre adjacent aux sources sous Windows

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS += -pipe -m64 -std=gnu++14 -Wunused-function -pedantic -Wextra
#QMAKE_CXXFLAGS += -march=core2
QMAKE_CXXFLAGS += -march=core-avx2

# Sous linux penser � installer libxml2-dev. Ceci n'est pas test�.


INCLUDEPATH += ../Interface/gui

windows {

  COMPILER_DIR = mingw64-5.2
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
    commandline_handler.cpp \
    thread_handler.cpp \
    analyseur.cpp

HEADERS += \
    fonctions_auxiliaires.hpp \
    table.hpp \
    validator.hpp \
    ligne_paye.hpp \
    ../Interface/gui/tags.h \
    commandline_handler.h \
    thread_handler.h \
    analyseur.h \
    templates.h

