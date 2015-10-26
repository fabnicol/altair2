#ifndef TABLE_HPP_INCLUDED
#define TABLE_HPP_INCLUDED
#include "validator.hpp"
#include <vector>


void generer_table_bulletins(info_t* Info);
int64_t generer_table_standard(const char* chemin_table, std::vector<info_t> &info);

void boucle_ecriture(std::vector<info_t> &info);

#endif // TABLE_HPP_INCLUDED
