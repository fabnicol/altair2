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
#if defined(__WIN32__)
#include <windows.h>
#include <psapi.h>

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
#include <stdio.h>
#endif
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


/// \page page_lhx La documentation de la ligne de commande de l'application-noyau lhx
/// \n\n
/// \section sec_docu_lhx Les paramètres de la ligne de commande
/// - <a href="../../linux/aide_lhx.html"><b>Aide au format html</b></a>
/// - <a href="../../linux/aide_lhx.pdf"><b>Aide au format pdf</b></a>
/// - l'aide en ligne de commande peut être appelée par :\code $ lhx -h \endcode
/// \section sec_formats_lhx La génération de la documentation
/// L'aide peut être générée aux formats :
/// - \b PDF, en invoquant : \code $ lhx --pdf \endcode
/// - \b HTML, en invoquant : \code $ lhx --html \endcode
/// - \b markdown, en invoquant : \code $ lhx --hmarkdown \endcode
/// \section sec_inter Les intéractions avec l'interface graphique
/// \subsection sub2_sec_inter En entrée de l'application
/// \subsection sub1_sec_inter En sortie de l'application
/// Un formatage HTML est proposé par défaut.\n
/// Ce formatage peut ne pas être réalisé à condition de recompiler l'application-noyau \b lhx en ne définissant pas la constante de compilation (définie par défaut)
/// \code GUI_TAG_MESSAGES \endcode
/// Si l'application est compilée en utilisant l'outil \b qmake et le fichier de configuration d'extension \em .pro, il suffit de retirer <tt> guiOutput </tt> de la ligne
/// \code CONFIG += console guiOutput \endcode
/// Lorsque le formatage HTML est activé, l'onglet Console de l'interface graphique affiche cette sortie en distinguant les phases de l'exécution par des symboles graphiques.\n 
/// 
/// 
/// 
ostringstream help();


/// Convertit un argument numérique donné en chaîne de caractères
/// \param argc  Nombre d'arguments de la ligne de commande restante
/// \param c_str  Pointeur vers une chaîne de caractères contenant un nombre
/// \return Entier positif de type 32 bits ou -1 si erreur.

int32_t lire_argument (int argc, char* c_str);

/// Calcule la mémoire requise pour l'exécution du programme. Met les fichiers XHL en mémoire dans info.threads->in_memory_file.
/// \param info Structure de type info_t contenant les données formatées
/// \return Retourne errno.

int calculer_memoire_requise ( info_t &info);

/// Ouvre une base de données de type table (bulletins + lignes) en écriture pour un segment d'exécution donné
/// \param info Référence vers une structure de type info_t contenant les données formatées
/// \param type Type d'exportation de base (base monolithique, par indemnité, par année...)
/// \param base Référence vers la base à générer de type \e ofstream
/// \param segment segment d'exécution

void ouvrir_fichier_base (const info_t &info, BaseType type, ofstream& base, int segment);


/// Ouvre une base de données de type table (bulletins + lignes) en écriture pour un segment d'exécution donné
/// \param info Référence vers une structure de type info_t contenant les données formatées
/// \param cat Catégorie de base (Bulletins de paye ou Lignes de paye)
/// \param type Type d'exportation de base (base monolithique, par indemnité, par année...)
/// \param base Référence vers la base à générer de type \e ofstream
/// \param segment segment d'exécution

void ouvrir_fichier_base0 (const info_t &info, BaseCategorie cat,  BaseType type, ofstream& base, int segment);

/// Ecrit les libellés des colonnes des bulletins
/// \param info Référence vers une structure de type info_t contenant les données formatées
/// \param base Référence vers la base à générer de type ofstream

void ecrire_entete_bulletins (const info_t &info, ofstream& base);

/// Ecrit les libellés des colonnes de la table (bulletins + lignes de paye)
/// \param info Référence vers une structure de type info_t contenant les données formatées
/// \param base Référence vers la base à générer de type ofstream

void ecrire_entete (const info_t &info, ofstream& base);

/// Ecrit les libellés des colonnes d'une base quelconque avec un tableau de libellés de taille donnée
/// \param info Référence vers une structure de type info_t contenant les données formatées
/// \param base Référence vers la base à générer de type ofstream
/// \param entete Tableau des libellés de colonne
/// \param N Taille de ce tableau

void ecrire_entete0 (const info_t &info, ofstream& base, const char* entete[], int N);


/// Ouvre une base de données de bulletins en écriture pour un segment d'exécution donné
/// \param info Référence vers une structure de type info_t contenant les données formatées
/// \param base Référence vers la base à générer de type ofstream
/// \param segment segment d'exécution

void ouvrir_fichier_bulletins (const info_t &info, ofstream& base, int segment);


/// Taille du fichier en octets
/// \param filename chemin du fichier
/// \return Taille en octets au format off_t

off_t taille_fichier (const string& filename);

/// Mémoire totale du système
/// \return Mémoire totale en octets au format size_t

size_t getTotalSystemMemory();

/// Mémoire libre du système
/// \return Mémoire libre du système en octets au format size_t

size_t getFreeSystemMemory();


/// Retourne la \e current resident set size (consommation de mémoire physique) mesurée
/// en octets, ou zéro si la valeur ne peut pas être déterminée par ce système
/// d'exploitation.
/// Code adapté de source externe. Voir site internet indiqué pour la documentation
/// Author:  David Robert Nadeau
/// Site:    http://NadeauSoftware.com/
/// License: Creative Commons Attribution 3.0 Unported License
///         http://creativecommons.org/licenses/by/3.0/deed.en_US

size_t getCurrentRSS( );

size_t getPeakRSS( );

extern mutex mut;
extern ofstream rankFile;
extern string rankFilePath;

/// Obtient le répertoire de l'exécution
/// \return Chemin du répertoire d'exécution

string getexecpath();

#ifdef USE_STRING_EXEC
string string_exec (const char* cmd);
#endif


/// Fonction d'affichage de des lignes du fichier XML de paye entourant celle où se pose
/// un problème de conformité des données
/// \param info  table d'informations
/// \param cur   noeud courant
/// \return structure de type errorLine_t contenant la ligne du fichier où apparaît
/// l'erreur ainsi qu'un message comprenant le fichier et le nom de la balise, s'il est
/// analysable. Sinon affiche un message indiquant son absence et retourne pour le numéro
/// de ligne. Retourne NA pour un noeud null.

errorLine_t afficher_environnement_xhl (const info_t& info, const xmlNodePtr cur);

/// Scinde une chaîne de caractères en ses composants séparées par un délimiteur
/// \return vecteur des composants

vector<string> split (const string &s, char delim) ;


/// Produit un journal d'exécution
/// \param info   table d'informations
/// \param log    fichier
/// \param diff   différence entre les analyseurs C et XML
/// \details Contenient les colonnes suivantes, pour chaque ligne de paye :\n
/// <pre> Année, Mois, Matricule, Rang global, Rang dans le fichier, Analseur C, Xml </pre>
/// et la différence entre l'analyseur C et Xml. \n
/// Sépare les colonnes par la chaine " | ".
/// \note Le chemin du journal est donné par info_t::chemin_log

void ecrire_log (const info_t& info, ofstream& log, int diff);

/// Transforme un fichier de type \e std::ifstream en un \e std::string
/// \param in Référence vers le fichier de type ifstream
/// \param alloc Allocateur ({} par défaut)
/// \return Chaîne de caractères de type string

template <typename Allocator = allocator<char>>
string read_stream_into_string (
    ifstream& in,
    Allocator alloc = {});

/// Calcule le maximum de lignes de paye par bulletin de paye d'un agent et
/// le maximum du nombre d'agents par mois
/// \param Info Vecteur de structures info_t, une par fil d'exécution
/// \param LOG  Pointeur vers un fichier de log de type ofstream

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
