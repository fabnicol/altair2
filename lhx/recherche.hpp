#include <fstream>
#include "validator.hpp"

vector<long long>  recherche(info_t &info, const xmlChar* annee, const xmlChar* mois, const xmlChar* matricule);
const string extraire_lignes(info_t info, uint64_t debut, uint64_t fin);
bool bulletin_paye(const string& chemin_repertoire, vector<info_t> &Info, const string& matricule, const string& mois, const string& annee);
