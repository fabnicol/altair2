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
#include <mutex>
#include "tags.h"

typedef struct {
                 const long lineN;
                 std::string filePath;
               } errorLine_t;

std::ostringstream help();
int32_t lire_argument(int argc, char* c_str);
int calculer_memoire_requise( info_t &info);
void ouvrir_fichier_base(const info_t &info, BaseType, std::ofstream& base, int segment);
void ouvrir_fichier_base0(const info_t &info, BaseCategorie,  BaseType type, std::ofstream& base, int segment);
void ecrire_entete_bulletins(const info_t &info, std::ofstream& base);

void ecrire_entete(const info_t &info, std::ofstream& base);
void ecrire_entete0(const info_t &info, std::ofstream& base, const char* entete[], int N);
void ouvrir_fichier_bulletins(const info_t &info, std::ofstream& base, int segment);

off_t taille_fichier(const std::string& filename);
size_t getTotalSystemMemory();
size_t getFreeSystemMemory();
size_t getCurrentRSS( );
size_t getPeakRSS( );

extern std::mutex mut;
extern std::ofstream rankFile;
extern std::string rankFilePath;

std::string getexecpath();

errorLine_t afficher_environnement_xhl(const info_t& info, const xmlNodePtr cur);

void ecrire_log(const info_t& info, std::ofstream& log, int diff);

void calculer_maxima(const std::vector<info_t> &Info, std::ofstream* LOG = nullptr);

#ifdef GENERATE_RANK_SIGNAL

static int rang_global;

inline void reset_rank_signal()
{
    rang_global = 0;
}

inline void GCC_INLINE effacer_char(xmlChar* c)
{
    for (int j = 0; *(c + j) != 0; ++j)
    {
        *(c + j) = *(c + j + 1);
    }
}

inline void  generate_rank_signal()
{
    if (rankFilePath.empty()) return;

    while (! mut.try_lock()) {}
    static int temp_rank;
    do
    {
        rankFile.open(rankFilePath, std::ios::out|std::ios::trunc);
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

    std::lock_guard<std::mutex> lock(mut);
    if (rankFilePath.empty()) return;

        rankFile.open(rankFilePath, std::ios::out|std::ios::trunc);
        if (rankFile.is_open())
        {
           rankFile << progression ;
        }

        rankFile.close();
}

static inline void GCC_UNUSED memory_debug(const std::string& func_tag)
{
#ifdef MEMORY_DEBUG
        std::cerr << STATE_HTML_TAG << func_tag << " : Calcul de la mémoire disponible : " << getFreeSystemMemory() << ENDL;
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
