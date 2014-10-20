#ifndef TABLE_HPP_INCLUDED
#define TABLE_HPP_INCLUDED
#include "validator.hpp"
uint64_t generer_table_bulletins(const char* chemin_base, info_t* Info);
int64_t generer_table_standard(const char* chemin_table, info_t* info);
inline uint64_t boucle_ecriture(FILE* base, info_t* Info);

#endif // TABLE_HPP_INCLUDED
