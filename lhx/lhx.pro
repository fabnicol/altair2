# Encodage : CP-1252

greaterThan(QT_MAJOR_VERSION, 5)

if (win32|linux) {
  message("Système d'exploitation :  $$(OS)")
} else {
  error("Le système d'exploitation doit être soit Windows soit linux")
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
  #  error( "Git doit être installé" )
}


if (!isEmpty(CXX_VERSION)){
    message( "Version du compilateur : $$CXX_VERSION" )
} else {
    error( "Le compilateur doit être GNU g++, dont la version doit être au moins 5.1" )
}

# Pour une sortie en mode console pure, enlever guiOutput ci-dessous. Sinon l'output console prépare
# l'input de la "console de l'interface graphique"

CONFIG += console guiOutput
CONFIG -= app_bundle
CONFIG -= qt


DEFINES +=  GUI_TAG_MESSAGES                   # définir pour que les sorties des messages soient formatées pour que l'interface graphique les transforme en icône.
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
            TYPE_LOOP_LIMIT=10 \       # nombre de "rembobinages des drapeaux de catégories (voir ligne_paye.cpp,
            MAX_STRING_LENGTH=200 \    # taille maximum des strings pour la conversion latin-1
            MAX_MEMORY_SHARE=0.5  \    # part maximum de la mémoire vive disponible consommée par défaut (si --memshare n'est pas précisé)
            SEGMENT_DIVISION_RATE=2 \
            AVERAGE_RAM_DENSITY=3 \    # constante empirique déterminant la quantité de mémoire nécessitée par 1 unité de mémoire de fichier xhl en cours de traitement.
           # MEMORY_DEBUG

DEFINES += __GNUC_EXTENSION \
           _REENTRANT \
           _GNU_SOURCE \
           __STDC_LIMIT_MACROS \
           __STDC_FORMAT_MACROS

                                            # DEFINES += STRICT  pour un validateur qui retourne à la première erreur.
DEFINES += \#NO_REGEX \                     # Pas d'analyse du texte par expression régulière
        GCC_REGEX \                         # Utiliser les expressions régulières de C++. Attention désactiver cette valeur casse les analyse sous R.
        WAIT_FOR_LOCK  \                    # insiter jusqu'à acquérir les mutex dans les fils d'exécution. Peut entraîner des "output freeze" en cas de forte
                        \                   # charge I/O.
        NO_DEBUG \                          # ne pas générer de messages de débogage avancé
#       USE_ICONV \                         # pour Windows uniquement, si l'on n'est pas satisfait du hack de pseudo-conversion UTF-8 vers Latin-1
                   \                        # alors on peut utiliser iconv pour une conversion plus propre.
#       TOLERANT_TAG_HIERARCHY \            # ordre des balises : tolérance envers des permutations de même niveau
        FULL_PREALLOCATION_TEST \           # calcul des besoins de mémoire : précision maximale (sinon : moindre)
        PREALLOCATE_ON_HEAP \               # préallouer le vecteur sur le tas pour le tableau des allocations de mémoire (sinon : tableau C sur la pile)
        DECIMAL_NON_EN \                    # compilation pour des séparateurs décimaux différents de '.'
        GENERATE_RANK_SIGNAL \              # chaque fois qu'un fichier est traité, un signal externe est émis (rang dans un fichier rank sous AppData\Local\Altair).
                             \              # n'est utile que lorsqu'une interface graphique est connectée. peut ralentir l'application de 1 à 5 %.
#        FGETC_PARSING    \                 # parcourir les fichiers par ifstream (C++)
        STRINGSTREAM_PARSING  \             # mise en mémoire vive des fichiers de paye par ostringstream (plus de mémoire vive ; accélère beaucoup le 1er traitement sous Windows)
#       MMAP_PARSING           \            # parcourir les fichiers par mappage mémoire (C, unix uniquement, aucun avantage évident).
#       OFSTREAM_TABLE_OUTPUT               # enregistrer les lignes de paye ligne à ligne sur la base. Plus robuste et moins de mémoire mais plus lent sous Windows

# Nota : définir au moins un des suivants : STRINGSTREAM_PARSING MMAP_PARSING FGETC_PARSING
# Sous windows la meilleure configuration de première exécution est STRINGSTREAM_PARSING pour un disque non-SSD seulement.
# STRINGSTREAM_PARSING est beaucoup moins performant pour les SSD ou quand les volumes sont grands.
# Ne pas définir OFSTREAM_TABLE_OUTPUT fait gagner environ 25 % à 30 % de temps d'exécution sous Windows.
# Pour économiser de la mémoire préférer FGETC_PARSING et définir OFSTREAM_TABLE_OUTPUT

DEVROOT = $$PWD/../..

# Insérer ici le nom du répertoire contenant dans include/ et lib/ les dépendances système
# Ce compilateur doit être adjacent aux sources sous Windows

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS += -pipe -m64 -std=gnu++14 -Wunused-function -pedantic -Wextra
#QMAKE_CXXFLAGS += -march=core2
QMAKE_CXXFLAGS += -march=core-avx2

# Sous linux penser à installer libxml2-dev. Ceci n'est pas testé.


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

