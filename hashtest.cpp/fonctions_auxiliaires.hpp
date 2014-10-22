#ifndef FONCTIONS_AUXILIAIRES_HPP_INCLUDED
#define FONCTIONS_AUXILIAIRES_HPP_INCLUDED
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "validator.hpp"

int lire_argument(int argc, const char* const c_str);
int calculer_memoire_requise(info_t* info);
#endif // FONCTIONS_AUXILIAIRES_HPP_INCLUDED
