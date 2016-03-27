#ifndef TABLE_HPP_INCLUDED
#define TABLE_HPP_INCLUDED
#include "validator.hpp"
#include "tags.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

void generer_table_bulletins(info_t* Info);
int64_t generer_table_standard(const char* chemin_table, vector<info_t> &info);
extern bool verbeux;
pair<uint64_t, uint32_t>  boucle_ecriture(vector<info_t> &info, int segment);

#if defined(__WIN32__) && defined(USE_ICONV)
static inline void convertir(const string& filepath)
{

    if (verbeux) cerr << PROCESSING_HTML_TAG "Conversion au format Latin-1..."  ENDL;

    system((string("set PATH=%PATH%; C:/Users/Public/Dev/altair/Outils && CALL iconv.exe -c -s -f UTF-8 -t ISO-8859-1 " )
                + filepath + ".temp" + CSV
                + string(" > ")
                + filepath + CSV + " 2>null").c_str());

    if (verbeux) cerr << PROCESSING_HTML_TAG "Conversion terminée"  ENDL;
}
#endif

#endif // TABLE_HPP_INCLUDED
