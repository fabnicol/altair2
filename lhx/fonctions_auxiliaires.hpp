#ifndef FONCTIONS_AUXILIAIRES_HPP_INCLUDED
#define FONCTIONS_AUXILIAIRES_HPP_INCLUDED

#define _CRT_SECURE_NO_WARNINGS
#include "validator.hpp"

#define BASE 0
#define BULLETINS 1
#include <iostream>
#include <fstream>
#include <vector>
#include <mutex>

int32_t lire_argument(int argc, char* c_str);
int calculer_memoire_requise( info_t &info);
void ouvrir_fichier_base(const info_t &info, int i, std::ofstream& base);
void ouvrir_fichier_base0(const info_t &info, int rang, int type, std::ofstream& base);
//char* ecrire_chemin_base(const char* chemin_base, int rang_fichier_base);
void ecrire_entete_bulletins(const info_t &info, std::ofstream& base);

void ecrire_entete(const info_t &info, std::ofstream& base);
void ecrire_entete0(const info_t &info, std::ofstream& base, const char* entete[], int N);
void ouvrir_fichier_bulletins(const info_t &info, std::ofstream& base);

extern std::mutex mut;
extern std::ofstream rankFile;
extern char* rankFilePath;

inline void GCC_INLINE  generate_rank_signal()
{
        static int temp_rank;

        ++temp_rank;

        if (mut.try_lock())
        {
            rankFile.open(rankFilePath, std::ios::out|std::ios::trunc);
            if (rankFile.is_open())
            {
               rankFile << temp_rank ;
            }

            rankFile.close();
        }

        mut.unlock();
}

#endif // FONCTIONS_AUXILIAIRES_HPP_INCLUDED
