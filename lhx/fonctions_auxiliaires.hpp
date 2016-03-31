#ifndef FONCTIONS_AUXILIAIRES_HPP_INCLUDED
#define FONCTIONS_AUXILIAIRES_HPP_INCLUDED

#define _CRT_SECURE_NO_WARNINGS
#include "validator.hpp"
#include <libxml/parser.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <mutex>
#include <cstring>
#include <cstdint>
#include <iomanip>
#include <iterator>
#include <sys/stat.h>

using namespace std;

#include "tags.h"

typedef struct {
                 const long lineN;
                 string filePath;
               } errorLine_t;

ostringstream help();
int32_t lire_argument(int argc, char* c_str);
int calculer_memoire_requise(info_t &info);
void ouvrir_fichier_base(const info_t &info, BaseType, ofstream& base, int segment);
void ouvrir_fichier_base0(const info_t &info, BaseCategorie,  BaseType type, ofstream& base, int segment);
void ecrire_entete_bulletins(const info_t &info, ofstream& base);

void ecrire_entete(const info_t &info, ofstream& base);
void ecrire_entete0(const info_t &info, ofstream& base, const char* entete[], int N);
void ouvrir_fichier_bulletins(const info_t &info, ofstream& base, int segment);

off_t taille_fichier(const string& filename);
vector<uint64_t> calculer_taille_fichiers(const vector<string>& files, bool silent = false);
vector<uint64_t> calculer_taille_fichiers_memoire(const vector<string>& in_memory_files, bool silent);
size_t getTotalSystemMemory();
size_t getFreeSystemMemory();
size_t getCurrentRSS( );
size_t getPeakRSS( );

extern mutex mut;
extern ofstream rankFile;
extern string rankFilePath;

string getexecpath();

errorLine_t afficher_environnement_xhl(const info_t& info, const xmlNodePtr cur);

void ecrire_log(const info_t& info, ofstream& log, int diff);

static string msg_erreur(const string& s = "")
{
    return string(ENDL ERROR_HTML_TAG) + s + string(ENDL);
}

template <typename e = std::runtime_error> void erreur(const string& s = "")
{
    throw e {  msg_erreur(s) };
}

void calculer_maxima(const vector<info_t> &Info, ofstream* LOG = nullptr);

#ifdef GENERATE_RANK_SIGNAL

static int rang_global;

inline void reset_rank_signal()
{
    rang_global = 0;
}

inline void  generate_rank_signal()
{
    if (rankFilePath.empty()) return;

    while (! mut.try_lock()) {}
    static int temp_rank;
    do
    {
        rankFile.open(rankFilePath, ios::out|ios::trunc);
        if (rankFile.is_open())
        {
            if (rang_global)
                rang_global = temp_rank;
            rankFile << ++rang_global ;
            temp_rank =rang_global;
        }

        rankFile.close();


        mut.unlock();

    } while(false);

}

#define pluriel(X, Y)  ((X > 1)? Y "s": Y)


inline void generate_rank_signal(int progression)
{
    if (rankFilePath.empty()) return;

        rankFile.open(rankFilePath, ios::out|ios::trunc);
        if (rankFile.is_open())
        {
           rankFile << progression ;
        }

        rankFile.close();
}

template <typename Allocator = allocator<char>>
inline string read_stream_into_string(
        ifstream& in,
        Allocator alloc = {})
{
    basic_ostringstream<char, char_traits<char>, Allocator>
            ss(basic_string<char, char_traits<char>, Allocator>(move(alloc)));

    if (!(ss << in.rdbuf()))
        throw ios_base::failure{"[ERR] Erreur d'allocation de lecture de fichier.\n"};

    return ss.str();
}

static inline void memory_debug(const string& func_tag GCC_UNUSED)
{
#ifdef MEMORY_DEBUG
   cerr << STATE_HTML_TAG << func_tag << " : Calcul de la m�moire disponible : " << getFreeSystemMemory() / (1024 * 1024) << " Mo"  ENDL;
#else
#endif
}

#if 0
static inline xmlChar* GCC_INLINE  UTF8toISO8859_1(const unsigned char* in)
{

    const char *encoding = "ISO-8859-1";
    unsigned char* out;
    int ret, size, out_size, temp;
    xmlCharEncodingHandlerPtr handler;

    size = (int) strlen((const char*) in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char*) malloc((size_t)out_size);

    if (out)
    {
        handler = xmlFindCharEncodingHandler(encoding);

        if (!handler)
        {
            free(out);
            out = NULL;
        }

         temp = size-1;
         ret = handler->input(out, &out_size, in, &temp);
         if (ret || temp - size + 1)
         {
            free(out);
            out = NULL;
         } else {
            out = (unsigned char*) realloc(out,out_size+1);
            out[out_size]=0; /*null terminating out*/

         }
     }

    return ((xmlChar*) out);
}
#endif

#endif

#endif // FONCTIONS_AUXILIAIRES_HPP_INCLUDED
