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
# Encodage : CP-1252

greaterThan(QT_MAJOR_VERSION, 5)


if (linux) {
  message("Système d'exploitation :  $$(OS)")

QMAKE_CXX=/usr/bin/g++
GIT_VERSION = $$system(git --version | grep -e \'git version\')
CXX_VERSION = $$system($$QMAKE_CXX --version | grep -e '[5-9].[0-9]')

if (!isEmpty(GIT_VERSION)) {
    message( "Version de git : $$GIT_VERSION" )
} else {
    error( "Git doit être installé" )
}


if (!isEmpty(CXX_VERSION)){
    message( "Version du compilateur : $$CXX_VERSION" )
    VERSION_8 = $$system($$QMAKE_CXX --version | grep -e '8.')
    if (! isEmpty(VERSION_8)) {
       message( "Utilisation de C++17")
    } else {
       message( "Utilisation de C++11")
   }

} else {
    error( "Le compilateur doit être GNU g++, dont la version doit être au moins 5.1" )
}
}

# Pour une sortie en mode console pure, enlever guiOutput ci-dessous. Sinon l'output console prépare
# l'input de la "console de l'interface graphique"

CONFIG += console guiOutput
CONFIG -= app_bundle
CONFIG -= qt

CONFIG(guiOutput) {
  DEFINES +=  GUI_TAG_MESSAGES                   # définir pour que les sorties des messages soient formatées pour que l'interface graphique les transforme en icône.
}

CONFIG(debug, debug|release) {

    QMAKE_CXXFLAGS = -O0 

} else {

    QMAKE_LFLAGS += -s
    QMAKE_CXXFLAGS = -O3 -fexpensive-optimizations 
}

VPATH = .

TEMPLATE = app

VERSION_TAG = $$system(cat ../VERSION)
DEFINES +=  VERSION=\\\"$$VERSION_TAG\\\"

message("Version :  $$VERSION_TAG")

DEFINES +=  WARNING_LIMIT=5  \         # nombre maximum d'avertissement par fichier
            MAX_NB_AGENTS=80000 \      # nombre maximum de bulletins par mois
            MAX_LIGNES_PAYE=3000 \     # nombre maximum de lignes de paye par bulletin
            TYPE_LOOP_LIMIT=50 \       # nombre de "rembobinages des drapeaux de catégories (voir ligne_paye.cpp,
            MAX_STRING_LENGTH=200 \    # taille maximum des strings pour la conversion latin-1
            MAX_MEMORY_SHARE=0.5  \    # part maximum de la mémoire vive disponible consommée par défaut (si --memshare n'est pas précisé)
            AVERAGE_RAM_DENSITY=12 \    # constante empirique déterminant la quantité de mémoire nécessitée par 1 unité de mémoire de fichier xhl en cours de traitement.
            #MEMORY_DEBUG \            # ajouter d ela verbosité
            USERPROFILE=\\\"HOME\\\" \         # pour la barre de progrès. Windows: "USERPROFILE"
            #LOCALDATA=\\\"/AppData/rank\\\" \   # Windows
            LOCALDATA=\\\".local/share/Altair/rank\\\"


DEFINES += __GNUC_EXTENSION \
           _REENTRANT \
           _GNU_SOURCE \
           __STDC_LIMIT_MACROS \
           __STDC_FORMAT_MACROS \
           SYSTEM_PATH_SEPARATOR=\"\';\'\" \
#          USE_STRING_EXEC  \
#          DEBUG_ATTEINDRE

                                            # DEFINES += STRICT  pour un validateur qui retourne à la première erreur.
DEFINES += \
#        INCLURE_REG_ELUS \                  # Les élus indentifiés par expression régulière se voient assigner le Statut ELU et le grade NA. Peut poser des problèmes de comparabilité avec Xemelios.  
#        INCLURE_REG_VACATAIRES \            # Les vacataires identifiés se voient assigner le Grade "V". Peu fiable.
#        INCLURE_REG_ASSMAT \                # Les assistantes maternelles identifiées se voient assigner le Grade "A". Peu fiable.
        WAIT_FOR_LOCK  \                    # insister jusqu'à acquérir les mutex dans les fils d'exécution. Peut entraîner des "output freeze" en cas de forte
                        \                   # charge I/O.
        NO_DEBUG \                          # ne pas générer de messages de débogage avancé
#       CONVERTIR_LATIN_1    \              # pour la compatiilité Windows
#       USE_ICONV \                         # pour la compatibilité Windows, si l'on n'est pas satisfait du hack de pseudo-conversion UTF-8 vers Latin-1
                   \                        # alors on peut utiliser iconv pour une conversion plus propre.
#       TOLERANT_TAG_HIERARCHY \            # ordre des balises : tolérance envers des permutations de même niveau
        FULL_PREALLOCATION_TEST \           # calcul des besoins de mémoire : précision maximale (sinon : moindre)
        PREALLOCATE_ON_HEAP \               # préallouer le vecteur sur le tas pour le tableau des allocations de mémoire (sinon : tableau C sur la pile)
        DECIMAL_NON_EN \                    # compilation pour des séparateurs décimaux différents de '.'
        GENERATE_RANK_SIGNAL \              # chaque fois qu'un fichier est traité, un signal externe est émis (rang dans un fichier rank sous AppData\Local\Altair).
                             \              # n'est utile que lorsqu'une interface graphique est connectée. peut ralentir l'application de 1 à 5 %.
#       FGETC_PARSING    \                  # parcourir les fichiers par ifstream (C++)
        STRINGSTREAM_PARSING  \             # mise en mémoire vive des fichiers de paye par ostringstream (plus de mémoire vive ; accélère beaucoup le 1er traitement sous Windows)
#       MMAP_PARSING           \            # parcourir les fichiers par mappage mémoire (C/C++, Unix uniquement, changer la directive sou Windows).
#       OFSTREAM_TABLE_OUTPUT  \            # enregistrer les lignes de paye ligne à ligne sur la base. Plus robuste et moins de mémoire mais plus lent sous Windows
        LHX_NO_REDEFINE \
#       TINYXML2                            # Utiliser tinyxml2 et pas libxml2  



# un seulement des trois  FGETC_PARSING, MMAP_PARSING ou STRINGSTREAM_PARSING doit être spécifié.

DEVROOT = $$PWD/../..

# Insérer ici le nom du répertoire contenant dans include/ et lib/ les dépendances système
# Ce compilateur doit être adjacent aux sources sous Windows


DEFINES += HAS_CPP17

QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS +=  -m64 -std=c++17
QMAKE_CXXFLAGS += -march=core-avx2 -Wextra
#QMAKE_CXXFLAGS += -march=core2
# Sous linux penser à installer libxml2-dev. Ceci n'est pas testé.

INCLUDEPATH += ../Interface/gui ../fwidgets_lib
windows:INCLUDEPATH += C:/msys64/mingw64/include/libxml2
linux:INCLUDEPATH += /usr/include/libxml2

linux:QMAKE_LFLAGS += -L/usr/lib/x86_64-linux-gnu/ -L/usr/lib/ -L/usr/lib64

windows:LIBS =   -lstdc++fs -static-libgcc -static-libstdc++ -pthread  C:/Users/Public/Dev/altair2/lhx/libxml2.dll.a   -lz -llzma -liconv -LC:\msys64\mingw64\bin
linux:LIBS =   -lstdc++fs -static-libgcc -static-libstdc++ -pthread  /usr/lib/x86_64-linux-gnu/libxml2.a   -lz -llzma -licuuc#-liconv

SOURCES += \
    fonctions_auxiliaires.cpp \
    main.cpp \
    table.cpp \
    validator.cpp \
    ligne_paye.cpp \
    recherche.cpp

contains(DEFINES, TINYXML2) {
  SOURCES += tinyxml2.cpp
}

HEADERS += \
    entete.h \
    table.h \
    ../Interface/gui/tags.h \
    expression_reg_adjoints.h \
    expression_reg_cata.h \
    expression_reg_commun.h \
    expression_reg_elus.h \
    filenames_windows.h \
    fonctions_auxiliaires.h \
    ligne_paye.h \
    recherche.h \
    validator.h \
    xmlconv.h

contains(DEFINES, TINYXML2) {
  HEADERS += tinyxml2.h xmlconv.h
}
