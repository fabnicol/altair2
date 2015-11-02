#ifndef FONCTIONS_AUXILIAIRES_HPP_INCLUDED
#define FONCTIONS_AUXILIAIRES_HPP_INCLUDED

#define _CRT_SECURE_NO_WARNINGS
#include "validator.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>


typedef struct {
                 const long lineN;
                 std::string filePath;
               } errorLine_t;


int32_t lire_argument(int argc, char* c_str);
int calculer_memoire_requise( info_t &info);
void ouvrir_fichier_base(const info_t &info, BaseType, std::ofstream& base);
void ouvrir_fichier_base0(const info_t &info, BaseCategorie,  BaseType type, std::ofstream& base);
void ecrire_entete_bulletins(const info_t &info, std::ofstream& base);

void ecrire_entete(const info_t &info, std::ofstream& base);
void ecrire_entete0(const info_t &info, std::ofstream& base, const char* entete[], int N);
void ouvrir_fichier_bulletins(const info_t &info, std::ofstream& base);

extern std::mutex mut;
extern std::ofstream rankFile;
extern std::string rankFilePath;

std::string getexecpath();

errorLine_t afficher_environnement_xhl(const info_t& info, const xmlNodePtr cur);

void ecrire_log(const info_t& info, std::ofstream& log, int diff);

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
    //static int temp_rank;
    do
    {
        rankFile.open(rankFilePath, std::ios::out|std::ios::trunc);
        if (rankFile.is_open())
        {
           rankFile << ++rang_global ;
           //rang_global = temp_rank;
        }

        rankFile.close();


        mut.unlock();

    } while(false);

}


inline void generate_rank_signal(int progression)
{
    if (rankFilePath.empty()) return;

        rankFile.open(rankFilePath, std::ios::out|std::ios::trunc);
        if (rankFile.is_open())
        {
           rankFile << progression ;
        }

        rankFile.close();
}

#endif

#endif // FONCTIONS_AUXILIAIRES_HPP_INCLUDED
