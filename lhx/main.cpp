
/*  Programme écrit par Fabrice NICOL sous licence CECILL 3 */


#include "validator.hpp"
#include "table.hpp"
#include "commandline_handler.h"


typedef chrono::high_resolution_clock Clock;

bool verbeux = true;

ofstream rankFile;
string rankFilePath = "";
mutex mut;

//static inline vector<int> repartir_fichiers_par_fil(const info_t& info, const vString& segment);

int main(int argc, char **argv)
{
    errno = 0;
    auto startofprogram = Clock::now();

#if defined _WIN32 | defined _WIN64
    setlocale(LC_ALL, "French_France.1252"); // Windows ne gère pas UTF-8 en locale
    //locale::global(locale("French_France.1252"));
#elif defined __linux__
    //setlocale(LC_ALL, "fr_FR.utf8");
    locale::global(locale("fr_FR.utf8"));
#else
#error "Programme conçu pour Windows ou linux"
#endif

    if (argc < 2)
    {
        cerr << ERROR_HTML_TAG "Il faut au moins un fichier à analyser.\n" ;
        return -2;
    }

    LIBXML_TEST_VERSION
            xmlKeepBlanksDefault(0);

    xmlInitMemory();
    xmlInitParser();

    /* Analyse de la ligne de commande */

    Commandline commande { argv, argc };

    /* Fin de l'analyse de la ligne de commande */

    commande.repartir_fichiers();

    /* ajustement représente la part maximum de la mémoire disponible que l'on consacre au processus, compte tenu de la marge sous plafond (overhead) */

    Analyseur analyse { commande };

    xmlCleanupParser();

    auto endofprogram = Clock::now();

    cerr << ENDL << PROCESSING_HTML_TAG "Durée d'exécution : "
         << chrono::duration_cast<chrono::milliseconds>(endofprogram - startofprogram).count()
         << " millisecondes" << ENDL;

    if (rankFile.is_open()) rankFile.close();

    return errno;
}
