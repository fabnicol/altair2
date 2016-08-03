#ifndef ELEMPARSER_HPP
#define ELEMPARSER_HPP
#ifndef REGEX_PARSING_FOR_HEADERS
//extern "C" {
struct Header {
        bool test;
        char annee[5];
        char mois[3];
        char siret[15];
} ;

#ifndef MAX_COUNT
  #define MAX_COUNT 1100
#endif
#define NB_CHAR_SAUT_ENTETE 140
#define SHIFT_MOIS_ANNEE 5 // 8
#define SHIFT_SIRET_MOIS 300 // 840
struct Header* elem_parser(const char*);
//}
#endif
#endif // ELEMPARSER_HPP
