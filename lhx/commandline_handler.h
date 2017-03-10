// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
// produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
// en vigueur à compter de l'année 2008.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions
// de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
// sur le site "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie,
// de modification et de redistribution accordés par cette licence, il n'est
// offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
// seule une responsabilité restreinte pèse sur l'auteur du programme, le
// titulaire des droits patrimoniaux et les concédants successifs.
//
// A cet égard l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant
// donné sa spécificité de logiciel libre, qui peut le rendre complexe à
// manipuler et qui le réserve donc à des développeurs et des professionnels
// avertis possédant des connaissances informatiques approfondies. Les
// utilisateurs sont donc invités à charger et tester l'adéquation du
// logiciel à leurs besoins dans des conditions permettant d'assurer la
// sécurité de leurs systèmes et ou de leurs données et, plus généralement,
// à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de la licence CeCILL, et que vous en avez accepté les
// termes.
//
//
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
#include "analyseur.h"
#include "templates.h"

using namespace std;
using vString = vector<string>;

extern bool verbeux;



class Commandline
{
friend class thread_handler;
friend class Analyseur;

public:

    uint64_t get_memoire_xhl() { return memoire_xhl; }
    uint64_t get_memoire_disponible() { return memoire_disponible; }
    uint64_t get_memoire_utilisable() { return memoire_utilisable; }

    float get_ajustement() { return ajustement; }
    uint32_t get_chunksize() { return chunksize; }
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
    Commandline() {}
    ~Commandline() { cerr << "Sortie de la classe Commandline."; }

    //void calculer_taille_fichiers(const vector<quad<string, uint64_t, int, int>>& files, bool silent = true);

    void repartir_fichiers();

    int nb_segment() { return input.size();}


    vector<int> nb_fichier_par_fil(int segment)
    {
         int nfichier = get_nb_fichier(segment);
         cerr << "SEGMENT n� " << segment + 1 << endl;
         cerr << "nfichier " << nfichier << endl;
         cerr << "nb_fil " << nb_fil << endl;

         int remainder = nfichier % nb_fil;
         vector<int> nb_fichier_par_fil;

         for (int  i = 0; i < nb_fil; ++i)
         {
             nb_fichier_par_fil.push_back(nfichier / nb_fil + (remainder > 0));
             --remainder;
         }


         return move(nb_fichier_par_fil);

         // ici il faut rep�rer les leftovers et les redistribuer entre segments...
    }

    int get_nb_fichier(int segment) { return nb_fichier_par_segment.at(segment); }


    vector<vector<quad<>>> get_input(int segment)
    {
        return std::move(input[segment]);
    }

    void print_repartion()
    {
        int i = 0, j = 0, k =0;
        for (auto && s : input)
            for (auto && f : s)
                for (quad<> p : f)
                    cerr << "segment " << i++ << " fil " << j++ << " fichier " << k++ << " : " << p.value << ", " << "index " << p.elements <<"\n";
    }

    void print() { int i = 0; for (auto &&s : argv) cerr << "argv[" << i++ << "]=" << s.value << " " << s.elements << "\n"; }

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
    uint64_t memoire_utilisable_par_fil = 0;

    vector<uint64_t> taille_segment;
    vector<int> nb_fichier_par_segment;


    float ajustement = MAX_MEMORY_SHARE;
    vector<quad<>> argv;
    vector<vector<vector<quad<>>>> input;
    vector<vector<quad<>>> input_par_segment;

    void memoire();
#if 0
    template<typename T=string, typename U=uint64_t, typename V=int, typename W=int> somme(const vector<quad<T, U, V, W>>& v)
    {
        U acc = 0;
        for (auto &&s : v)
        {
          if (s.elements == 1)
              acc += s.size;
          else
          {
              acc += (s.elements - 1) * info.chunksize + s.size % chunksize;
          }
        }
        return acc;
    }
#endif
    info_t info;

    bool allouer_fil(const int fil,
                     vector<quad<>>::iterator& iter_fichier);

};


#endif // COMMANDLINE_HANDLER_H
