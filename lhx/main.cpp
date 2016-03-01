
/*  Programme écrit par Fabrice NICOL sous licence CECILL 3 */


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
#include "validator.hpp"
#include "fonctions_auxiliaires.hpp"
#include "table.hpp"
#include "tags.h"

#ifndef OVERHEAD
  #define OVERHEAD 500
#endif

#ifndef AVERAGE_RAM_DENSITY
  #define AVERAGE_RAM_DENSITY 1.25
#endif

using namespace std;
using vString = vector<string>;

typedef chrono::high_resolution_clock Clock;

bool generer_table = false;
bool liberer_memoire = true;
bool verbeux = true;

ofstream rankFile;
string rankFilePath = "";
mutex mut;
vector<errorLine_t> errorLineStack;
vString commandline_tab;

int produire_segment(const info_t& info, const vString& segment, pair<uint64_t, uint32_t> &nlignes);


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

    int start = 1;
    string type_table = "bulletins";
    vString cl;  /* pour les lignes de commandes incluses dans un fichier */
    string chemin_base = NOM_BASE + string(CSV);
    string chemin_bulletins = NOM_BASE_BULLETINS + string(CSV);
    unsigned long long memoire_xhl = 0, memoire_disponible = 0;
    int nsegments = 0;
    float ajustement = MAX_MEMORY_SHARE;

    thread_t mon_thread;

    info_t info =
    {
        {{}},             //    bulletinPtr* Table;
        0,                //    uint64_t nbLigne;
        {},             //    int32_t  *NAgent;
        0,                //    uint32_t nbAgentUtilisateur
        0,                //    uint32_t NCumAgent;
        0,                //    uint32_t NCumAgentXml;
        0, // taille base
        BaseType::MONOLITHIQUE,                //    type base
        vector<uint16_t>(),             //    vector<uint16_t> NLigne;
        &mon_thread,      //    thread_t threads;
        "",             //    chemin log
        EXPRESSION_REG_ELUS,
        chemin_base,
        chemin_bulletins,
        MAX_LIGNES_PAYE,  // nbLigneUtilisateur
        0,                //    uint16_t fichier_courant
        ',',              //    const char decimal;
        ';',              //    const char separateur;
        true,             // réduire coso mémoire
        true,             // par défaut lire la balise adjacente
        false,            // calculer les maxima de lignes et d'agents
        false,            // numéroter les lignes
        false,             // ne pas exporter les informations sur l'établissement
        false,             // ne pas exporter l'échelon
        false,             // pretend
        false,             // verifmem
        1,                 // nbfil
        {}  // besoin de mémoire effectif
    };

    /* Analyse de la ligne de commande */

    commandline_tab.assign(argv, argv + argc);

    while (start < argc)
    {
      if (commandline_tab[start] == "-n")
       {
         info.reduire_consommation_memoire = false;
         info.nbAgentUtilisateur
           = lire_argument (argc, const_cast<char*>(commandline_tab[start + 1].c_str()));
         if (info.nbAgentUtilisateur < 1)
            {
              cerr << ERROR_HTML_TAG "Préciser le nombre de bulletins mensuels attendus (majorant du nombre) avec -N xxx .\n";
              exit(-1);
            }
            start += 2;
            continue;
        }
        else if (commandline_tab[start] ==  "-h")
        {
            string out = move(help()).str();
            out.erase(remove(out.begin(), out.end(), '*'), out.end());
            cerr << out;
            exit(0);
        }
        else if (commandline_tab[start] ==  "--hmarkdown" || commandline_tab[start] == "--pdf" || commandline_tab[start] == "--html")
        {
          ostringstream out = std::move(help());
          if (commandline_tab[start] == "--hmarkdown")
          {
             cerr << out.str();
          }
          else
          {
             ofstream help;

              help.open("aide.md");

              help << out.str();
              string sep(":");
              sep[0] = SYSTEM_PATH_SEPARATOR;
              string exec_dir = getexecpath();
#ifndef __linux__
              string command = string("PATH=") + string(getenv("PATH"))
                                 + sep + exec_dir + string("/../texlive/miktex/bin")
                                 + sep + exec_dir + string("/../Outils")
                                 + sep + exec_dir + string("/../RStudio/bin/pandoc");
              putenv(command.c_str());
#endif

              system("iconv.exe -t utf-8 -f latin1 -c -s  aide.md > aide.utf8.md");
              if (commandline_tab[start] == "--pdf")
              {
                 system("pandoc.exe -o aide_lhx.pdf  aide.utf8.md");
              }
              else
              if (commandline_tab[start] == "--html")
              {
                 system("pandoc.exe -o aide_lhx.utf8.html  aide.utf8.md");
                 system("iconv.exe -f utf-8 -t latin1 -c -s  aide_lhx.utf8.html > aide_lhx.html");
                 unlink("aide_lhx.utf8.html");
              }
              help.close();
              unlink("aide.utf8.md");
              unlink("aide.md");
          }

          exit(0);
        }
        else if (commandline_tab[start] ==  "-q")
        {
            verbeux = false;
            ++start;
            continue;
        }
        else if (commandline_tab[start] == "-t")
        {
            generer_table = true;
            if (commandline_tab[start + 1] == "standard")
            {
                type_table = commandline_tab[start + 1];
                start += 2;
                continue;
            }
            else
            {
                ++start;
                continue;
            }
        }
        else if (commandline_tab[start] ==  "-l")
        {
            info.generer_rang = true ;
            ++start;
            continue;
        }
        else if (commandline_tab[start] == "-T")
        {
            if (start + 1 == argc)
            {
                cerr << ERROR_HTML_TAG "Option -T suivi d'un argument obligatoire (nombre de lignes)." ENDL ;
                exit(-100);
            }

            map<string, BaseType> hashTable;

            hashTable["AN"] = BaseType::PAR_ANNEE;
            hashTable["A"]  = BaseType::PAR_REM_DIVERSES;
            hashTable["AC"] = BaseType::PAR_ACOMPTE;
            hashTable["AV"] = BaseType::PAR_AVANTAGE_NATURE;
            hashTable["C"]  = BaseType::PAR_COTISATION;
            hashTable["D"]  = BaseType::PAR_DEDUCTION;
            hashTable["I"]  = BaseType::PAR_INDEMNITE;
            hashTable["IR"] = BaseType::PAR_INDEMNITE_RESIDENCE;
            hashTable["S"]  = BaseType::PAR_SFT;
            hashTable["T"]  = BaseType::PAR_TRAITEMENT;
            hashTable["R"]  = BaseType::PAR_RAPPEL;
            hashTable["X"]  = BaseType::TOUTES_CATEGORIES;

            if (hashTable.find(commandline_tab[start + 1]) != hashTable.end())
            {
                info.type_base = hashTable[commandline_tab[start + 1]];
            }
            else
            {
                // On a trouvé un argument numérique en ligne de commande

                int32_t size_read = lire_argument(argc, const_cast<char*>(commandline_tab[start + 1].c_str()));

                if (size_read < 0 || size_read > INT32_MAX -1)
                {
                    cerr << ERROR_HTML_TAG "Le nombre de lignes doit être compris entre 0 et INT64_MAX" ENDL;
                    exit(-908);
                }
                else
                {
                    info.taille_base = size_read;

                    info.type_base = (info.type_base == BaseType::PAR_ANNEE)? BaseType::MAXIMUM_LIGNES_PAR_ANNEE : BaseType::MAXIMUM_LIGNES;

                    cerr << PARAMETER_HTML_TAG "Bases d'au plus " << size_read << " lignes" ENDL;
                }
            }

            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "-s")
        {
            if (start + 1 == argc)
            {
                cerr << ERROR_HTML_TAG "Option -s suivi d'un argument obligatoire (séparateur de champs)." ENDL;
                exit(-100);
            }
            info.separateur = commandline_tab[start + 1][0];

            if (info.separateur == '_')
            {
                perror(ERROR_HTML_TAG "Le séparateur ne doit pas être '_'");
                exit(-909);
            }

            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "-d")
        {
            if (start + 1 == argc)
            {
                cerr << ERROR_HTML_TAG "Option -d suivi d'un argument obligatoire (séparateur décimal)." ENDL;
                exit(-100);
            }
            info.decimal = commandline_tab[start + 1][0];
            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "-o")
        {
            if (start + 1 == argc)
            {
                cerr << ERROR_HTML_TAG "Option -o suivi d'un argument obligatoire (nom de  fichier)." ENDL;
                exit(-100);
            }

            info.chemin_base = commandline_tab[start + 1];

            ofstream base;
            base.open(info.chemin_base);
            if (! base.good())
            {
                perror(ERROR_HTML_TAG "La base de données ne peut être créée, vérifier l'existence du dossier.");
                exit(-113);
            }

            base.close();
            unlink(info.chemin_base.c_str());
            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "-M")
        {
            liberer_memoire = false;
            ++start;
            continue;
        }
        else if (commandline_tab[start] == "-m")
        {
            info.calculer_maxima = true;
            ++start;
            continue;
        }
        else if (commandline_tab[start] == "-D")
        {
            info.chemin_base = commandline_tab[start + 1] + string("/") + string(NOM_BASE) ;
            info.chemin_bulletins = commandline_tab[start + 1] + string("/") + string(NOM_BASE_BULLETINS);
            ofstream base;
            base.open(info.chemin_base, ofstream::out | ofstream::trunc);

            if (! base.good())
            {
                cerr << ERROR_HTML_TAG "La base de données "
                          << info.chemin_base  + string(CSV) << " ne peut être créée, vérifier l'existence du dossier." ENDL ;
                exit(-113);
            }
            else
            {
                // Necessaire sous Windows, no-op sous *.nix
                base.close();
                unlink(info.chemin_base.c_str());
            }

            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "-j")
        {
            if ((info.nbfil = lire_argument(argc, const_cast<char*>(commandline_tab[start +1].c_str()))) > 0)
            {
                if (info.nbfil < 1)
                {
                    perror(ERROR_HTML_TAG "Le nombre de fils d'exécution doit être compris au moins égal à 2.");
                    exit(-111);
                }
            }
            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "-L")
        {
            if (argc > start +2) info.chemin_log = commandline_tab[start + 1];
            ofstream base;
            base.open(info.chemin_log);
            if (! base.good())
            {
                perror(ERROR_HTML_TAG "Le log ne peut être créé, vérifier l'existence du dossier.");
                exit(-114);
            }
            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "-N")
        {
            if ((info.nbLigneUtilisateur = lire_argument(argc, const_cast<char*>(commandline_tab[start +1].c_str()))) > 1)
            {
                cerr << STATE_HTML_TAG "Nombre maximum de lignes de paye redéfini à : " << info.nbLigneUtilisateur << ENDL;
            }

            info.reduire_consommation_memoire = false;
            if ((info.nbAgentUtilisateur = lire_argument(argc, const_cast<char*>(commandline_tab[start + 1].c_str()))) < 1)
            {
                cerr << ERROR_HTML_TAG "Préciser le nombre de nombre maximum d'agents par mois attendus (majorant du nombre) avec -n xxx" ENDL;
                exit(-1);
            }

            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "-R")
        {
            if (argc > start +2)
            {
                info.expression_reg_elus = commandline_tab[start + 1];
            }
            else
            {
                perror(ERROR_HTML_TAG "Il manque l'expression régulière.");
                exit(-115);
            }
            start += 2;
            continue;
        }
#ifdef GENERATE_RANK_SIGNAL
        else if (commandline_tab[start] == "-rank")
        {
            int hasArg = 0;
            if (argc > start +2)
            {
                if (commandline_tab[start + 1][0] == '-')
                    rankFilePath = string(getenv("USERPROFILE")) + "/AppData/rank";
                else
                {
                    rankFilePath = commandline_tab[start + 1];
                    hasArg = 1;
                }

                ifstream testFile;
                testFile.open(rankFilePath);
                if (testFile.is_open())
                {
                    testFile.close();
                }

                rankFile.open(rankFilePath, ios::out| ios::trunc);
                rankFile << 1 << "\n";
                rankFile.close();
            }

            start += 1 + hasArg;
            continue;
        }
#endif

        else if (commandline_tab[start] == "-S")
        {
            if (argc > start +2)
            {
                info.select_siret = true;
            }
            else
            {
                exit(-116);
            }
            ++start;
            continue;
        }
        else if (commandline_tab[start] == "-E")
        {
            if (argc > start +2)
            {
                info.select_echelon = true;
            }
            else
            {
                exit(-116);
            }
            ++start;
            continue;
        }
        else if (commandline_tab[start] == "-f")
        {
            const char* fichier;
            if (argc >= start + 2)
            {
                fichier = commandline_tab[start + 1].c_str();
            }
            else
            {
                cerr << ERROR_HTML_TAG "Il manque le fichier de ligne de commande.\n";
                exit(-119);
            }

            ifstream f;
            f.open(fichier);
            if (! f.good())
            {
                cerr << ERROR_HTML_TAG "Impossible d'ouvrir le fichier " << fichier << ".\n";
                exit(-119);
            }

            string ligne;
            if (f.good())
            {
              while(f.rdstate() != fstream::eofbit)
              {
                getline(f, ligne);

                if (! ligne.empty())
                       cl.push_back(ligne);
              }

            }

            f.close();
            if (! cl.empty())
            {
                argc = cl.size() + 1;

                commandline_tab.resize(argc);

                for (int i = 0; i < argc - 1; ++i)
                {

                    commandline_tab[i + 1] = cl.at(i);

                    if (string(cl.at(i)) == "-f")
                    {
                        cerr << ERROR_HTML_TAG "La ligne de commande -f ne peut pas être incluse dans un fichier par -f [risque de boucle infinie].\n";
                    }
                }


                /* on relance ...*/

                start = 1;
                continue;
            }
            //break;
        }
        else if (commandline_tab[start] == "--pretend")
        {
          info.pretend = true;   // pas de décodage
          ++start;
          continue;
        }
        else if (commandline_tab[start] == "--verifmem")
        {
          info.verifmem = true;  // seulement vérifier la mémoire
          ++start;
          continue;
        }
        else if (commandline_tab[start] == "--xhlmem")
        {
          cerr << STATE_HTML_TAG "Taille totale des fichiers : " << commandline_tab[start + 1] << " octets." << ENDL;
          // Taille des fichiers en ko fournie par l'interface graphique, en octets
          
          memoire_xhl = stoull(commandline_tab[start + 1], nullptr);
          if (60000ULL * 1048576ULL < ULLONG_MAX)
          {
              if (memoire_xhl > 1ULL || memoire_xhl < (60000ULL * 1048576ULL))
                {
                  start += 2;
                  continue;
                }
              else
                {
                  cerr << ERROR_HTML_TAG "La donnée de la taille des fichiers en input est erronée ou au-delà de la limite permise (60 Go)." << ENDL;
                  exit(-199);
                }
          }
          else
          {
                  cerr << ERROR_HTML_TAG "Le plus grand entier non signé est inférieur à 60 * 1048576" << ENDL;
                  exit(-199);
          }
        }
        else if (commandline_tab[start] == "--memshare")
          {
            int part = stoi(commandline_tab[start + 1], nullptr);

            cerr << STATE_HTML_TAG "Part de la mémoire vive utilisée : " <<  part << " %" ENDL;

            ajustement = (float) part / 100;
            start += 2;
            continue;
          }
        else if (commandline_tab[start] == "--segments ")
        {
           cerr << STATE_HTML_TAG "Les bases seront scindées en au moins : " << commandline_tab[start + 1] << " segments" << ENDL;

           // au maximum 99 segments

           nsegments = stoi(commandline_tab[start + 1], nullptr);
           if (nsegments > 1 || nsegments < 100)
             {
               start += 2;
               continue;
             }
           else
             {
               cerr << ERROR_HTML_TAG "Il doit y avoir entre 1 et 99 segments de bases." << ENDL;
               exit(-208);
             }
        }

        else if (commandline_tab[start][0] == '-')
        {
          cerr << ERROR_HTML_TAG "Option inconnue " << commandline_tab[start] << ENDL;
          exit(-100);
        }
        else break;
    }

    /* Fin de l'analyse de la ligne de commande */

    //struct stat st;
    //stat(info.threads->argv[i].c_str(), &st);
    //const size_t file_size =  st.st_size;
    

    /* on sait que info.nbfil >= 1 */

    vector<unsigned long long> taille;

    /* Soit la taille totale des fichiers est transmise par --mem soit on la calcule ici,
     * en utilisant taille_fichiers */

    if (memoire_xhl == 0)
    {
        off_t mem = 0;
        int count = 0;

        for (int i = start; i < argc; ++i)
        {
            if ((mem = taille_fichier(commandline_tab.at(i))) != -1)
            {
                memoire_xhl += static_cast<unsigned long long>(mem);
                taille.push_back(static_cast<unsigned long long>(mem));
                ++count;
            }
            else
            {
                cerr << ERROR_HTML_TAG "La taille du fichier " << commandline_tab.at(i) << " n'a pas pu être déterminée." ENDL;
            }
        }

        cerr << ENDL STATE_HTML_TAG << "Taille totale des " << count << " fichiers : " << memoire_xhl / 1048576 << " Mo."  ENDL;
        cerr << STATE_HTML_TAG
                  << "Mémoire disponible " <<  ((memoire_disponible = getFreeSystemMemory()) / 1048576)
                  << " / " << (getTotalSystemMemory()  / 1048576)
                  << " Mo."  ENDL;
    }

    /* ajustement représente la part maximum de la mémoire disponible que l'on consacre au processus, compte tenu de la marge sous plafond (overhead) */

    unsigned long long memoire_utilisable = floor(ajustement * static_cast<float>(memoire_disponible));
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
                  cerr << ERROR_HTML_TAG "Impossible de générer " << nsegments << " segments. Utilisation d'au moins " << nsegments + 1 << " segments." ENDL;
                  ++nsegments;
               }
             else
               {
                  memoire_utilisable = memoire_xhl / (nsegments - 1);
                  break;
               }
         }
    }

    vector<vString> segments;

    auto  taille_it = taille.begin();
    auto  commandline_it = commandline_tab.begin() + start;

    do
    {
        unsigned long long taille_segment = *taille_it;
        vString segment;
        float densite_segment=AVERAGE_RAM_DENSITY;
        #ifdef STRINGSTREAM_PARSING
          ++densite_segment;
        #endif
        #ifndef OFSTREAM_TABLE_OUTPUT
          ++densite_segment;
        #endif

        while (taille_segment * densite_segment < memoire_utilisable && commandline_it != commandline_tab.end())
         {
           segment.push_back(*commandline_it);  // ne pas utiliser move;
           ++commandline_it;
           ++taille_it;
           taille_segment  += *taille_it;
         }

        segments.emplace_back(segment);

    } while (commandline_it != commandline_tab.end());

    unsigned int segments_size = segments.size();
    if (segments_size > 1)
        cerr << PROCESSING_HTML_TAG << "Les bases en sortie seront scindées en " << segments_size << " segments." ENDL;

   int info_nbfil_defaut = info.nbfil;

   pair<uint64_t, uint32_t> nlignes;

   for (auto&& segment : segments)
   {
        unsigned int segment_size = segment.size();

        if (info.nbfil > segment_size)
        {
            cerr << ERROR_HTML_TAG "Trop de fils (" << info.nbfil << ") pour le nombre de fichiers (" << segment_size << "); exécution avec " << segment_size << "fils." ENDL;

            info.nbfil = segment_size;
        }
        else
            info.nbfil = info_nbfil_defaut;

        produire_segment(info, segment, nlignes);
    }

    xmlCleanupParser();

    if (generer_table && segments_size > 1)
    {
      cerr << ENDL << STATE_HTML_TAG "Cumul des lignes de paye : " << nlignes.first << ENDL;
      cerr << ENDL << STATE_HTML_TAG "Cumul des bulletins de paye : " << nlignes.second << ENDL;
    }

    auto endofprogram = Clock::now();

    cerr << ENDL << PROCESSING_HTML_TAG "Durée d'exécution : "
         << chrono::duration_cast<chrono::milliseconds>(endofprogram - startofprogram).count()
         << " millisecondes" << ENDL;

    if (rankFile.is_open()) rankFile.close();

    return errno;
}

int produire_segment(const info_t& info, const vString& segment, pair<uint64_t, uint32_t>& p)
{
    static int nsegment;

    ++nsegment;

    vector<int> nb_fichier_par_fil;
    int segment_size = segment.size();

    int remainder = segment_size % info.nbfil;

    for (unsigned i = 0; i < info.nbfil; ++i)
    {
         nb_fichier_par_fil.push_back(segment_size / info.nbfil + (remainder > 0));
         --remainder;
    }

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

    for (unsigned  i = 0; i < info.nbfil; ++i)
    {
        Info[i] = info;

        Info[i].threads = &v_thread_t[i];
        Info[i].threads->thread_num = i;

        Info[i].threads->argc = nb_fichier_par_fil.at(i);

        Info[i].threads->argv = vString(segment_it, segment_it + nb_fichier_par_fil[i]);

        Info[i].threads->in_memory_file = vString(nb_fichier_par_fil[i]);
        segment_it += nb_fichier_par_fil.at(i);

        if (Info[i].threads->argv.size() != (unsigned) nb_fichier_par_fil.at(i))
        {
            cerr << ERROR_HTML_TAG "Problème issu de l'allocation des threads" << ENDL ;
            exit(-145);
        }

        if (verbeux)
            cerr <<  PROCESSING_HTML_TAG "Fil d'exécution i = " << i+1 << "/" << info.nbfil
                 << "   Nombre de fichiers dans ce fil : " << nb_fichier_par_fil[i] << ENDL;

        errno = 0;

        /* Lancement des fils d'exécution */

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

    if (verbeux && Info[0].reduire_consommation_memoire)
       cerr << ENDL PROCESSING_HTML_TAG "Premier scan des fichiers pour déterminer les besoins mémoire ... " ENDL;

    if (info.nbfil > 1)
        for (unsigned i = 0; i < info.nbfil; ++i)
        {
            t[i].join ();
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
      pair<uint64_t, uint32_t> res = boucle_ecriture(Info, nsegment);
      p.first += res.first;
      p.second += res.second;
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

   generate_rank_signal(0);
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
                if (j != Categorie) xmlFree(Info[i].Table[agent][j]);
            }
        }
    }


    return 1;
}


