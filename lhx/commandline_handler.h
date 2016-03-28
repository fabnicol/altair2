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

using namespace std;
using vString = vector<string>;

class Commandline_handler
{
public:

    uint64_t memoire_xhl = 0;
    uint64_t memoire_disponible = 0;
    uint64_t memoire_utilisable = 0;
    int nsegments = 0;
    float ajustement = MAX_MEMORY_SHARE;
    vString argv;

    Commandline_handler(info_t& , char** argv, int argc);

    vString liste_fichiers()
    {
        return vString(argv.begin() + index_debut_fichiers, argv.end());
    }

private:

    template <typename e = std::runtime_error> void erreur(const string& s = "") { throw e {  string(ERROR_HTML_TAG) + s + string(ENDL) }; }
    int index_debut_fichiers = 1;
    void memoire()
    {
        cerr << STATE_HTML_TAG
             << "Mémoire disponible " <<  ((memoire_disponible = getFreeSystemMemory()) / 1048576)
             << " / " << (getTotalSystemMemory()  / 1048576)
             << " Mo."  ENDL;

        memoire_utilisable = floor(ajustement * static_cast<float>(memoire_disponible));

        cerr << STATE_HTML_TAG  << "Mémoire utilisable " <<  memoire_utilisable / 1048576   << " Mo."  ENDL;

        if (nsegments != 0)
        {
            while (true)
            {
                if (memoire_xhl % nsegments == 0)
                {
                    memoire_utilisable = memoire_xhl / nsegments ;
                    break;
                }
                else
                    if (memoire_xhl % (nsegments - 1) == 0)
                    {
                        cerr << ERROR_HTML_TAG "Impossible de générer " << nsegments
                             << " segments. Utilisation d'au moins " << nsegments + 1 << " segments." ENDL;
                        ++nsegments;
                    }
                    else
                    {
                        memoire_utilisable = memoire_xhl / (nsegments - 1);
                        break;
                    }
            }
        }
    }


};

#endif // COMMANDLINE_HANDLER_H
