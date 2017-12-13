// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
// produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
// en vigueur à compter de l'année 2008.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions
// de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
// sur le site "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie,
// de modification et de redistribution accordés par cette licence, il n'est
// offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
// seule une responsabilité restreinte pèse sur l'auteur du programme, le
// titulaire des droits patrimoniaux et les concédants successifs.
//
// A cet égard l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant
// donné sa spécificité de logiciel libre, qui peut le rendre complexe à
// manipuler et qui le réserve donc à des développeurs et des professionnels
// avertis possédant des connaissances informatiques approfondies. Les
// utilisateurs sont donc invités à charger et tester l'adéquation du
// logiciel à leurs besoins dans des conditions permettant d'assurer la
// sécurité de leurs systèmes et ou de leurs données et, plus généralement,
// à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de la licence CeCILL, et que vous en avez accepté les
// termes.
//

/// \file fonctions_auxiliaires.h
/// \author Fabrice Nicol
/// \brief Diverses fonctions auxiliaires en inlining


#ifndef FONCTIONS_AUXILIAIRES_HPP_INCLUDED
#define FONCTIONS_AUXILIAIRES_HPP_INCLUDED

#define _CRT_SECURE_NO_WARNINGS

#include "validator.h"
#include <libxml/parser.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <mutex>
#include <cstring>
#include <cstdint>
#include <iomanip>
#include <cstdio>
#include <iterator>
#include <sys/stat.h>
#include <memory>
#include <stdexcept>
#include <string>
#include <memory>
#include <stdexcept>
#include <array>

using namespace std;

#include "tags.h"

/// Bloqueur de fils d'exécutions concurrents pour l'accès à une console ou à un même fichier.
#define LOCK_GUARD  lock_guard<mutex> guard(mut);

/// Structure encapsulant les messages d'erreurs sur décodage XML
typedef struct
{
    const long lineN; ///< Numéro de ligne XML
    string filePath;  ///< Chemin du fichier XML
    string pres;      ///< Message d eprésentation à afficher
} errorLine_t;


ostringstream help();

int32_t lire_argument (int argc, char* c_str);

int calculer_memoire_requise ( info_t &info);
void ouvrir_fichier_base (const info_t &info, BaseType, ofstream& base, int segment);
void ouvrir_fichier_base0 (const info_t &info, BaseCategorie,  BaseType type, ofstream& base, int segment);
void ecrire_entete_bulletins (const info_t &info, ofstream& base);

void ecrire_entete (const info_t &info, ofstream& base);
void ecrire_entete0 (const info_t &info, ofstream& base, const char* entete[], int N);
void ouvrir_fichier_bulletins (const info_t &info, ofstream& base, int segment);

off_t taille_fichier (const string& filename);
size_t getTotalSystemMemory();
size_t getFreeSystemMemory();
size_t getCurrentRSS( );
size_t getPeakRSS( );

extern mutex mut;
extern ofstream rankFile;
extern string rankFilePath;

string getexecpath();

#ifdef USE_STRING_EXEC
string string_exec (const char* cmd);
#endif

errorLine_t afficher_environnement_xhl (const info_t& info, const xmlNodePtr cur);

vector<string> split (const string &s, char delim) ;

void ecrire_log (const info_t& info, ofstream& log, int diff);

void calculer_maxima (const vector<info_t> &Info, ofstream* LOG = nullptr);

#ifdef GENERATE_RANK_SIGNAL

/// Rang global de la progression de l'extraction.\n
/// Utilisé pour la barre de progression de l'interface graphique

static int rang_global;

/// Remet à zéro le rang global
inline void reset_rank_signal()
{
    rang_global = 0;
}

/// Efface le premier caractère d'une chaîne et translate la chaîne d'un caractère vers la gauche
/// \param c chaine de caractères libXml2 à modifier par pointeur

inline void GCC_INLINE effacer_char (xmlChar* c)
{
    for (int j = 0; * (c + j) != 0 && * (c + j + 1) != 0; ++j)
        {
            * (c + j) = * (c + j + 1);
        }
}

/// Incrémente le rang de la progression de la barre de progrès.\n
/// \note Est en principe thread-safe, mais peut causer des ralentissements en raison du \n
///  bloquage des fils concurrents

inline void  generate_rank_signal()
{
    if (rankFilePath.empty()) return;

    while (! mut.try_lock()) {}

    static int temp_rank;

    do
        {
            rankFile.open (rankFilePath, ios::out | ios::trunc);

            if (rankFile.is_open())
                {
                    if (rang_global)
                        rang_global = temp_rank;

                    rankFile << ++rang_global ;
                    temp_rank = rang_global;
                }

            rankFile.close();


            mut.unlock();

        }
    while (false);

}

/// Rajoute un s au pluriel de Y si X est non unique

#define pluriel(X, Y)  ((X > 1)? " " Y "s ": " " Y " ")

/// Actualise le rang de la progression de la barre de progrès.
/// \param progression indice d'actualisation
/// \note Thread-safe.

inline void generate_rank_signal (int progression)
{
    LOCK_GUARD

    if (rankFilePath.empty()) return;

    rankFile.open (rankFilePath, ios::out | ios::trunc);

    if (rankFile.is_open())
        {
            rankFile << progression ;
        }

    rankFile.close();
}

/// Débogage de la mémoire
/// \param func_tag Chaîne de caractères donnant un libellé à afficher.
/// \note Thread-safe. N'est activé que si la constante MEMORY_DEBUG est définie.

static inline void  memory_debug (GCC_UNUSED const string& func_tag)
{
#ifdef MEMORY_DEBUG
    LOCK_GUARD
    cerr << STATE_HTML_TAG << func_tag << " : Calcul de la mémoire disponible : " << getFreeSystemMemory() << ENDL;
#endif
}

#endif
#endif // FONCTIONS_AUXILIAIRES_HPP_INCLUDED
