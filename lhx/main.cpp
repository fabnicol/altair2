
/*  Programme écrit par Fabrice NICOL sous licence CECILL 3 */


#include "validator.hpp"
#include "table.hpp"
#include "commandline_handler.h"

#ifndef OVERHEAD
#define OVERHEAD 500
#endif

#ifndef AVERAGE_RAM_DENSITY
#define AVERAGE_RAM_DENSITY 1.25
#endif

using vString = vector<string>;
using vUint64 = vector<uint64_t>;

typedef chrono::high_resolution_clock Clock;

bool generer_table = false;
bool liberer_memoire = true;
bool verbeux = true;

ofstream rankFile;
string rankFilePath = "";
mutex mut;
vector<errorLine_t> errorLineStack;

int produire_segment(const info_t& info, const vString& segment, const vector<uint64_t>& taille_fichiers_segment);
static inline vector<int> repartir_fichiers_par_fil(const info_t& info, const vString& segment);


int main(int argc, char **argv)
{
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

    info_t info;

    /* Analyse de la ligne de commande */

    Commandline_handler command { info, argv, argc };

    /* Fin de l'analyse de la ligne de commande */

    /* on sait que info.nbfil >= 1 */

    vector<uint64_t> taille;

    vString files = command.liste_fichiers();

    #ifdef STRINGSTREAM_PARSING
       vString in_memory_files;
    #endif

    /* Soit la taille totale des fichiers est transmise par --mem soit on la calcule ici,
     * en utilisant taille_fichiers */

    /* Note --mem reste à implémenter */

    if (command.memoire_xhl == 0)
    {
       taille = calculer_taille_fichiers(files);
    }

    /* redécoupage éventuel des fichiers
     * Le gain éventuel du multithreading est faible (-20% à -25% sur le temps d'exécution
     * mais intéressant pour des bases de plus de 5 ML (-4 s à 5 ML, -16s à 20 ML) */

    vector<int> nb_fichier_par_fil = repartir_fichiers_par_fil(info, files);
    vector<info_t> Info(info.nbfil);
    vector<thread> t;

    if (info.nbfil > 1)
    {
        t.resize(info.nbfil);
    }

    vector<thread_t> v_thread_t(info.nbfil);
    vString::const_iterator files_it = files.begin();
    vector<uint64_t>::const_iterator taille_it = taille.begin();

    for (unsigned  i = 0; i < info.nbfil; ++i)
    {

           Info[i] = info;
           Info[i].threads = &v_thread_t[i];
           Info[i].threads->thread_num = i;
           Info[i].threads->argc = nb_fichier_par_fil.at(i);
           Info[i].threads->argv = vString(files_it, files_it + nb_fichier_par_fil[i]);

           Info[i].taille = vector<uint64_t>(taille_it, taille_it + nb_fichier_par_fil[i]);

           files_it   += nb_fichier_par_fil.at(i);
           taille_it  += nb_fichier_par_fil.at(i);

       if (info.nbfil > 1)
       {
           thread th{redecouper, ref(Info[i])};
           t[i] = move(th);
       }
       else
       {
           redecouper(ref(Info[0]));
       }
    }

    if (info.nbfil > 1)
        for (unsigned i = 0; i < info.nbfil; ++i)
        {
            t[i].join();
        }

    /* Il faut alors reverser argc_cut dans argv ... idem pour in_memory */

    #ifndef STRINGSTREAM_PARSING
      files.clear();
    #endif

    for (unsigned i = 0; i < info.nbfil; ++i)
        for (unsigned j = 0; j < Info[i].threads->argc; ++j)
        {
            #if defined(STRINGSTREAM_PARSING)
               if (! Info[i].threads->in_memory_file_cut.at(j).empty())
               {
                  for (string s : Info[i].threads->in_memory_file_cut.at(j))
                    {
                       in_memory_files.emplace_back(s);
                    }
               }
               else
                  in_memory_files.emplace_back(Info[i].threads->in_memory_file.at(j));
            #else
               if (! Info[i].threads->argv_cut.at(j).empty())
               {
                   for (auto &&s : Info[i].threads->argv_cut.at(j))
                     files.emplace_back(s);
               }
               else
                   files.emplace_back(Info[i].threads->argv.at(j));

            #endif
        }

    /* On recalcule les tailles */

    taille.clear();

   #ifdef STRINGSTREAM_PARSING
     taille = calculer_taille_fichiers_memoire(in_memory_files, true);
   #else
     taille = calculer_taille_fichiers(files, true);
   #endif

    /* ajustement représente la part maximum de la mémoire disponible que l'on consacre au processus, compte tenu de la marge sous plafond (overhead) */



    vector<vString> segments;
    vector<vUint64> taille_fichiers_segments;

    taille_it = taille.begin();

    #ifdef STRINGSTREAM_PARSING
        files_it = in_memory_files.begin();
        auto files_it_end = in_memory_files.end();
    #else
        files_it = files.begin();
        auto files_it_end = files.end();
    #endif

    do
    {
        uint64_t cumul_taille_segment = *taille_it;
        vString segment;
        vUint64 taille_fichiers_segment;
        float densite_segment = AVERAGE_RAM_DENSITY;

#ifdef STRINGSTREAM_PARSING
        ++densite_segment;
#endif
#ifndef OFSTREAM_TABLE_OUTPUT
        ++densite_segment;
#endif

        while (cumul_taille_segment * densite_segment < command.memoire_utilisable && files_it != files_it_end)
        {
            segment.emplace_back(*files_it);
            taille_fichiers_segment.push_back(*taille_it);

            ++files_it;
            ++taille_it;
            cumul_taille_segment  += *taille_it;
        }

        segments.emplace_back(segment);
        taille_fichiers_segments.emplace_back(taille_fichiers_segment);

    }
     while (files_it != files_it_end);

    unsigned int segments_size = segments.size();
    if (segments_size > 1)
        cerr << PROCESSING_HTML_TAG << "Les bases en sortie seront analysées en " << segments_size << " segments." ENDL;

    int info_nbfil_defaut = info.nbfil;
    int rang_segment = 0;

    for (auto&& segment : segments)
    {
        unsigned int segment_size = segment.size();

        if (info.nbfil > segment_size)
        {
            cerr << ERROR_HTML_TAG "Trop de fils (" << info.nbfil << ") pour le nombre de fichiers (" << segment_size << "); exécution avec "
                 << segment_size << pluriel(segment_size, " fil") <<"."  ENDL;

            info.nbfil = segment_size;
        }
        else
            info.nbfil = info_nbfil_defaut;

        produire_segment(info, segment, taille_fichiers_segments.at(rang_segment++));
    }

    xmlCleanupParser();

    auto endofprogram = Clock::now();

    cerr << ENDL << PROCESSING_HTML_TAG "Durée d'exécution : "
         << chrono::duration_cast<chrono::milliseconds>(endofprogram - startofprogram).count()
         << " millisecondes" << ENDL;

    if (rankFile.is_open()) rankFile.close();

    return errno;
}

static inline vector<int> repartir_fichiers_par_fil(const info_t& info, const vString& segment)
{
    vector<int> nb_fichier_par_fil;
    int segment_size = segment.size();

    int remainder = segment_size % info.nbfil;

    for (unsigned i = 0; i < info.nbfil; ++i)
    {
        nb_fichier_par_fil.push_back(segment_size / info.nbfil + (remainder > 0));
        --remainder;
    }

    return nb_fichier_par_fil;
}


int produire_segment(const info_t& info, const vString& segment, const vector<uint64_t>& taille_fichiers_segment)
{
    static int nsegment;

    ++nsegment;

    vector<int> nb_fichier_par_fil = repartir_fichiers_par_fil(info, segment);

    /* on répartir le reste de manière équilibrée sur les premiers fils */

    vector<info_t> Info(info.nbfil);
    vector<thread> t;

    if (info.nbfil > 1)
    {
        t.resize(info.nbfil);
    }

    if (verbeux)
    {
        cerr << PROCESSING_HTML_TAG "Création de " << info.nbfil << " fils d'exécution." ENDL;
    }

    vector<thread_t> v_thread_t(info.nbfil);
    vString::const_iterator segment_it = segment.begin();
    vector<uint64_t>::const_iterator taille_fichiers_segment_it = taille_fichiers_segment.begin();

    for (unsigned  i = 0; i < info.nbfil; ++i)
    {
        Info[i] = info;

        Info[i].threads = &v_thread_t[i];
        Info[i].threads->thread_num = i;

        Info[i].threads->argc = nb_fichier_par_fil.at(i);

        for (int k = 0; k < nb_fichier_par_fil.at(i); ++k)
          #ifdef STRINGSTREAM_PARSING
             Info[i].threads->in_memory_file[k] = move(*(segment_it + k));
          #else
             Info[i].threads->argv[k] = move(*(segment_it + k));
          #endif

        Info[i].taille = vector<uint64_t>(taille_fichiers_segment_it, taille_fichiers_segment_it + nb_fichier_par_fil[i]);

        segment_it += nb_fichier_par_fil.at(i);
        taille_fichiers_segment_it += nb_fichier_par_fil.at(i);

        if (verbeux)
            cerr <<  PROCESSING_HTML_TAG "Fil d'exécution n°" << i + 1 << "/" << info.nbfil
                  << "   Nombre de fichiers dans ce fil : " << nb_fichier_par_fil[i] << ENDL;

        errno = 0;

        /* Lancement des fils d'exécution */

        if (verbeux && Info[0].reduire_consommation_memoire)
            cerr << ENDL PROCESSING_HTML_TAG "Premier scan des fichiers pour déterminer les besoins mémoire, " << "fil " << i + 1 << ENDL;

        if (info.nbfil > 1)
        {
            thread th{decoder_fichier, ref(Info[i])};
            t[i] = move(th);
        }
        else
        {
            decoder_fichier(ref(Info[0]));
        }

        if (errno)
        {
            cerr << ENDL << strerror(errno) << ENDL;
        }
    }

    if (verbeux)
        cerr << ENDL PROCESSING_HTML_TAG "Rassemblement des fils d'exécution." ENDL;

    if (info.nbfil > 1)
        for (unsigned i = 0; i < info.nbfil; ++i)
        {
            t[i].join();
        }

    if (info.pretend) return 2;


    if (Info[0].calculer_maxima)
    {
        calculer_maxima(Info);
    }

    if (! Info[0].chemin_log.empty())
    {
        ofstream LOG;
        LOG.open(Info[0].chemin_log, ios::app);
        calculer_maxima(Info, &LOG);
        LOG.close();
    }


    if (generer_table)
    {
        cerr << ENDL << PROCESSING_HTML_TAG "Exportation des bases de données au format CSV..." << ENDL ENDL;
        boucle_ecriture(Info, nsegment);
    }

    /* Résumé des erreurs rencontrées */

    if (errorLineStack.size() > 0)
    {
        cerr << WARNING_HTML_TAG "<b>Récapitulatif des erreurs rencontrées</b>" << ENDL;
        for (const errorLine_t& e :  errorLineStack)
        {
            cerr << e.filePath;
            if (e.lineN != -1)
                cerr << " -- Ligne n°" << e.lineN << ENDL;
            else
                cerr << " -- Ligne inconnue." << ENDL;

        }
    }

    if (! Info[0].chemin_log.empty())
    {
        ofstream LOG;
        LOG.open(Info[0].chemin_log, ios::app);
        if (LOG.good())
            for (const errorLine_t& e :  errorLineStack)
            {
                if (e.lineN != -1)
                    LOG << " -- Ligne n°" << e.lineN << ENDL;
                else
                    LOG << " -- Ligne inconnue." << ENDL;
            }
    }

    generate_rank_signal(-1);
    cerr << " \n";

    /* libération de la mémoire */

    if (! liberer_memoire) return 0;

    if (verbeux)
        cerr << ENDL
             << PROCESSING_HTML_TAG "Libération de la mémoire..."
             << ENDL;

    /* En cas de problème d'allocation mémoire le mieux est encore de ne pas désallouer car on ne connait pas exacteemnt l'état
     * de la mémoire dynamique */

    for (unsigned i = 0; i < Info[0].nbfil; ++i)
    {
        for (unsigned agent = 0; agent < Info[i].NCumAgent; ++agent)
        {
            for (int j = 0; j < Info[i].Memoire_p_ligne[agent]; ++j)
            {
                if (j != Categorie && xmlStrcmp(Info[i].Table[agent][j], (const xmlChar*) "") != 0) xmlFree(Info[i].Table[agent][j]);
            }
        }
    }


    return 1;
}


