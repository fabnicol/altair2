#include <fstream>
#include "validator.hpp"

vector<long long>  recherche(info_t &info, const xmlChar* annee, const xmlChar* mois, const xmlChar* matricule);
const string extraire_lignes(const info_t& info, const array<uint64_t, 3>& debut, const array<uint64_t, 2>& fin);
bool bulletin_paye(const string& chemin_repertoire, vector<info_t> &Info, const string& matricule, const string& mois, const string& annee);
