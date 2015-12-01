#ifndef TABLE_HPP_INCLUDED
#define TABLE_HPP_INCLUDED
#include "validator.hpp"
#include "tags.h"
#include <vector>
#include <string>
#include <iostream>

void generer_table_bulletins(info_t* Info);
int64_t generer_table_standard(const char* chemin_table, std::vector<info_t> &info);
extern bool verbeux;
void boucle_ecriture(std::vector<info_t> &info);

#if defined(__WIN32__) && defined(USE_ICONV)
static inline void convertir(const std::string& filepath)
{

    if (verbeux) std::cerr << PROCESSING_HTML_TAG "Conversion au format Latin-1..."  ENDL;

    std::system((std::string("set PATH=%PATH%; C:/Users/Public/Dev/altair/Outils && CALL iconv.exe -c -s -f UTF-8 -t ISO-8859-1 " )
                + filepath + ".temp" + CSV
                + std::string(" > ")
                + filepath + CSV + " 2>null").c_str());

    if (verbeux) std::cerr << PROCESSING_HTML_TAG "Conversion terminée"  ENDL;
}
#endif

#endif // TABLE_HPP_INCLUDED
