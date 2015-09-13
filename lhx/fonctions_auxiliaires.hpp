#ifndef FONCTIONS_AUXILIAIRES_HPP_INCLUDED
#define FONCTIONS_AUXILIAIRES_HPP_INCLUDED
//#include <limits.h>
//#include <errno.h>
//#include <stdio.h>
//#include <inttypes.h>
//#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS
#include "validator.hpp"

#define BASE 0
#define BULLETINS 1

int32_t lire_argument(int argc, char* c_str);
int calculer_memoire_requise(info_t* info);
FILE* ouvrir_fichier_base(info_t* info, int i);
char* ecrire_chemin_base(const char* chemin_base, int rang_fichier_base);
void ecrire_entete_bulletins(info_t* info, FILE* base);
//FILE* ouvrir_fichier_base_append(info_t* info, int rang);
void ecrire_entete(info_t* info, FILE* base);
void ecrire_entete0(info_t* info, FILE* base, const char* entete[], int N);
FILE* ouvrir_fichier_bulletins(info_t* info);
FILE* ouvrir_fichier_base0(info_t* info, int rang, int type);

#endif // FONCTIONS_AUXILIAIRES_HPP_INCLUDED
