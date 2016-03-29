#ifndef COMMANDLINE_HANDLER_H
#define COMMANDLINE_HANDLER_H
#include <mutex>
#ifdef __linux__
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#endif
#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <map>
#include <chrono>
#include <cstring>
#include <sstream>
#include <cstdlib>
#include <limits.h>
#include <exception>

#include "tags.h"
#include "fonctions_auxiliaires.hpp"
#include "thread_handler.h"

using namespace std;
using vString = vector<string>;
class Commandline;
class thread_handler;

class Analyseur
{
friend class thread_handler;

public:
    Analyseur(Commandline&);
    static vector<errorLine_t>  errorLineStack;
    void lanceur(Commandline&, int nb_segment);
    int produire_segment(Commandline&, int rang_segment);
    static void warning_msg(const char* noeud, const info_t& info, const xmlNodePtr cur);

private:
    void liberer_memoire( Commandline&, thread_handler& );
    static int parseFile(info_t& info);
    static int parseFile(const xmlDocPtr doc, info_t& info, int cont_flag, xml_commun* champ_commun);
    static void* decoder_fichier(info_t& info);
};

class Commandline
{
friend void Analyseur::lanceur(Commandline&, int);
friend int Analyseur::produire_segment(Commandline&, int);
friend class thread_handler;

public:

    uint64_t get_memoire_xhl() { return memoire_xhl; }
    uint64_t get_memoire_disponible() { return memoire_disponible; }
    uint64_t get_memoire_utilisable() { return memoire_utilisable; }
    int get_nsegments() { return nsegments; }
    float get_ajustement() { return ajustement; }
    uint32_t get_chunksize() { return chunksize; }
    vector<uint64_t> get_taille() { return std::move(taille); }
    int get_nb_fil() { return nb_fil; }
    string get_chemin_log() { return info.chemin_log; }

    void set_memoire_xhl(uint64_t m) { memoire_xhl = m; }
    void set_memoire_disponible(uint64_t m) { memoire_disponible = m; }
    void set_memoire_utilisable(uint64_t m) { memoire_utilisable = m; }
    void set_nsegments(int n) { nsegments = n; }
    void set_ajustement(float f) { ajustement = f; }
    void set_chunksize(uint32_t c) { chunksize = c; }
    void set_nbfil(int n) { nb_fil = n; }

    bool is_pretend() { return info.pretend; }
    bool is_generer_table() { return generer_table; }
    bool is_liberer_memoire() { return liberer_memoire; }
    bool is_calculer_maxima() { return calculer_maxima; }
    bool is_reduire_consommation_memoire() { return info.reduire_consommation_memoire; }

    Commandline(char** argv, int argc);

    void calculer_taille_fichiers(const vector<string>& files, bool silent = true);
    void repartir_fichiers();

    int nb_segment() { return input.size();}


    vector<int> nb_fichier_par_fil(int segment)
    {
         int nfichier = get_nb_fichier(segment);
         int remainder = nfichier % nb_fil;
         vector<int> nb_fichier_par_fil;

         for (int  i = 0; i < nb_fil; ++i)
         {
             nb_fichier_par_fil.push_back(nfichier / nb_fil + (remainder > 0));
             --remainder;
         }

         return move(nb_fichier_par_fil);

         // ici il faut repérer les leftovers et les redistribuer entre segments...
    }

    int get_nb_fichier(int segment) { return nb_fichier_par_segment.at(segment); }

    vector<pair<string, int>> get_input(int segment)
    {
        return std::move(input.at(segment));
    }


private:

    bool pretend = false;
    bool generer_table = false;
    bool liberer_memoire = true;
    bool calculer_maxima = false;

    int index_debut_fichiers = 1;
    int nb_fil = 1;
    int nsegments = 0;
    uint32_t chunksize = CUTFILE_CHUNK;
    uint64_t memoire_xhl = 0;
    uint64_t memoire_disponible = 0;
    uint64_t memoire_utilisable = 0;
    vector<int> nb_fichier_par_segment;

    float ajustement = MAX_MEMORY_SHARE;
    vString argv;
    vector<uint64_t> taille;
    vector<vector<pair<string, int>>> input;

    void memoire();

    vector<int> get_nb_fichier()
    {
        vector<int> v;
        int N = nb_segment();
        for (int segment = 0; segment < N; ++segment)
        {
            int n = 0;
            for (auto &&s: input.at(segment)) n += s.second;
            v.push_back(n);
        }

        return std::move(v);
    }

    info_t info;

};

#endif // COMMANDLINE_HANDLER_H
