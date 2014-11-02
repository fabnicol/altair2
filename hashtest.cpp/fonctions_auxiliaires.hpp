#ifndef FONCTIONS_AUXILIAIRES_HPP_INCLUDED
#define FONCTIONS_AUXILIAIRES_HPP_INCLUDED
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "validator.hpp"

int32_t lire_argument(int argc, char* c_str);
int calculer_memoire_requise(info_t* info);
FILE* ouvrir_fichier_base(info_t* info, int i);
char* ecrire_chemin_base(char* chemin_base, int rang_fichier_base);
FILE* ajouter_au_fichier_base(info_t* info, int rang);
void ecrire_entete(info_t* info, FILE* base);
#endif // FONCTIONS_AUXILIAIRES_HPP_INCLUDED
