
/*  Programme �crit par Fabrice NICOL sous licence CECILL 3 */


#include <mutex>
#ifdef __linux__
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#endif
#include <iostream>
#include <vector>
#include <thread>
#include <map>
#include <chrono>
#include <cstring>
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

typedef chrono::high_resolution_clock Clock;

bool generer_table = false;
bool liberer_memoire = true;
bool verbeux = true;

ofstream rankFile;
string rankFilePath = "";
mutex mut;
vector<errorLine_t> errorLineStack;
vector<string> commandline_tab;

int produire_segment(const info_t& info, const vector<string>& segment);


int main(int argc, char **argv)
{
    auto startofprogram = Clock::now();

#if defined _WIN32 | defined _WIN64
    setlocale(LC_ALL, "French_France.1252"); // Windows ne g�re pas UTF-8 en locale
    //locale::global(locale("French_France.1252"));
#elif defined __linux__
    //setlocale(LC_ALL, "fr_FR.utf8");
   locale::global(locale("fr_FR.utf8"));
#else
#error "Programme con�u pour Windows ou linux"
#endif

    if (argc < 2)
    {
        cerr << ERROR_HTML_TAG "Il faut au moins un fichier � analyser.\n" ;
        return -2;
    }

    LIBXML_TEST_VERSION
    xmlKeepBlanksDefault(0);

    xmlInitMemory();
    xmlInitParser();

    int start = 1;
    string type_table = "bulletins";
    vector<string> cl;  /* pour les lignes de commandes incluses dans un fichier */
    string chemin_base = NOM_BASE + string(CSV);
    string chemin_bulletins = NOM_BASE_BULLETINS + string(CSV);
    unsigned long long memoire_xhl = 0, memoire_disponible = 0;
    int nsegments = 0;
    float ajustement = 1;
    unsigned long long overhead = OVERHEAD * 1048576ULL;

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
        true,             // r�duire coso m�moire
        true,             // par d�faut lire la balise adjacente
        false,            // calculer les maxima de lignes et d'agents
        false,            // num�roter les lignes
        false,             // ne pas exporter les informations sur l'�tablissement
        false,             // ne pas exporter l'�chelon
        false,             // pretend
        false,             // verifmem
        1,                 // nbfil
        {}  // besoin de m�moire effectif
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
              cerr << ERROR_HTML_TAG "Pr�ciser le nombre de bulletins mensuels attendus (majorant du nombre) avec -N xxx .\n";
              exit(-1);
            }
            start += 2;
            continue;
        }
        else if (commandline_tab[start] ==  "-h")
        {
            cerr <<  "Usage :  lhx OPTIONS fichiers.xhl" << "\n"
                      <<  "OPTIONS :" << "\n"
                      <<  "-n argument obligatoire : nombre maximum de bulletins mensuels attendus [calcul exact par d�faut]" << "\n"
                      <<  "-N argument obligatoire : nombre maximum de lignes de paye attendues [calcul exact par d�faut]" << "\n"
                      <<  "-t argument optionnel   : type de base en sortie, soit 'standard', soit 'bulletins' [d�faut bulletins]." << "\n"
                      <<  "-T argument obligatoire : nombre de lignes maximum par base .csv [d�faut illimit�]. Au plus 999 tables seront g�n�r�es." << "\n"
                      <<  "-T AN                   : g�n�rer une table par ann�e" << "\n"
                      <<  "-T A/AC/AV/C/D/I/IR/RE/S/T : g�n�rer une table pour chaque cat�gorie de ligne : \
                              A r�mun�rations diverse \n \
                              AC acompte \n \
                              AV avantage en nature \n \
                              C cotisation \n \
                              D d�duction \n \
                              I indemnit�s \n \
                              IR indemnit� de r�sidence \n \
                              RE retenue \n \
                              S suppl�ment familial \n \
                              T traitement brut \n \
                              X toutes cat�gories\n" << "\n"
                      <<  "-o argument obligatoire : fichier.csv, chemin complet du fichier de sortie [d�faut 'Table.csv' avec -t]." << "\n"
                      <<  "-D argument obligatoire : r�pertoire complet du fichier de sortie [d�faut '.' avec -t]." << "\n"
                      <<  "-d argument obligatoire : s�parateur d�cimal [d�faut ',' avec -t]." << "\n"
                      <<  "-s argument obligatoire : s�parateur de champs [d�faut ';' avec -t]. Ne pas utiliser '_'." << "\n"
                      <<  "-j argument obligatoire : nombre de fils d'ex�cution (1 � 10)." << "\n"
                      <<  "-l sans argument        : g�n�rer une colonne de num�ros de ligne intitul�e 'R'." << "\n"
                      <<  "-M sans argument        : ne pas lib�rer la m�moire r�serv�e en fin de programme." << "\n"
                      <<  "-m sans argument        : calculer les maxima d'agents et de lignes de paye." << "\n"
                      <<  "-L argument obligatoire : chemin du log d'ex�cution du test de coh�rence entre analyseurs C et XML." << "\n"
                      <<  "-R argument obligatoire : expression r�guli�re pour la recherche des �lus (cod�s : ELU dans le champ Statut." << "\n"
                      <<  "-S sans argument        : exporter les champs Budget, Employeur, Siret, Etablissement." << "\n"
                      <<  "-E sans argument        : exporter le champ Echelon." << "\n"
                      <<  "-q sans argument        : limiter la verbosit�." << "\n"
                      <<  "-f argument obligatoire : la ligne de commande est dans le fichier en argument, chaque �l�ment � la ligne." << "\n"
                      <<  "--xhlmem arg. oblig.    : taille des fichiers � analyser en octets." << "\n"
                      <<  "--memshare arg. oblig.  : Part de la m�moire vive utilis�e, de 0 (toute) � 1." << "\n"
                      <<  "--segments arg. oblig.  : nombre minimum de segments de base." << "\n"
                      <<  "--verifmem              : seulement v�rifier la consommation m�moire.  " << "\n"
                      <<  "--hmarkdown              : aide en format markdown.  " << "\n"
                      <<  "--pretend              : ex�cution sans traitement des fichiers." << "\n";

              #ifdef GENERATE_RANK_SIGNAL
                      cerr  <<  "-rank argument optionnel : g�n�rer le fichier du rang de la base de paye en cours dans le fichier.\n";

                     #if defined _WIN32 | defined _WIN64
                      cerr  <<  "                           ou � d�faut dans %USERPROFILE%\\AppData\\Altair\\rank.\n";
                     #else
                        #if defined __linux__
                          cerr  <<  "                           ou � d�faut dans ~/.local/share/Altair/rank.\n";
                        #endif
                     #endif
              #endif

            exit(0);
        }
        else if (commandline_tab[start] ==  "--hmarkdown")
        {
          cerr <<  "**Usage** :  lhx OPTIONS fichiers.xhl  " << "\n"
                    <<  "**OPTIONS :**  " << "\n"
                    <<  "**-n** *argument obligatoire* : nombre maximum de bulletins mensuels attendus [calcul exact par d�faut]  " << "\n"
                    <<  "**-N** *argument obligatoire* : nombre maximum de lignes de paye attendues [calcul exact par d�faut]  " << "\n"
                    <<  "**-t** *argument optionnel*   : type de base en sortie, soit 'standard', soit 'bulletins' [d�faut bulletins].  " << "\n"
                    <<  "**-T** *argument obligatoire* : nombre de lignes maximum par base .csv [d�faut illimit�]. Au plus 999 tables seront g�n�r�es.  " << "\n"
                    <<  "**-T AN**                   : g�n�rer une table par ann�e  " << "\n"
                    <<  "**-T A/AC/AV/C/D/I/IR/RE/S/T** : g�n�rer une table pour chaque cat�gorie de ligne :    \n\
                            A r�mun�rations diverse  \n \
                            AC acompte  \n \
                            AV avantage en nature  \n \
                            C cotisation  \n \
                            D d�duction  \n \
                            I indemnit�s  \n \
                            IR indemnit� de r�sidence  \n \
                            RE retenue  \n \
                            S suppl�ment familial  \n \
                            T traitement brut  \n \
                            X toutes cat�gories     \n" << "\n"
                    <<  "**-o** *argument obligatoire* : fichier.csv, chemin complet du fichier de sortie [d�faut 'Table.csv' avec -t].  " << "\n"
                    <<  "**-D** *argument obligatoire* : r�pertoire complet du fichier de sortie [d�faut '.' avec -t].  " << "\n"
                    <<  "**-d** *argument obligatoire* : s�parateur d�cimal [d�faut ',' avec -t].  " << "\n"
                    <<  "**-s** *argument obligatoire* : s�parateur de champs [d�faut ';' avec -t]. Ne pas utiliser '_'.  " << "\n"
                    <<  "**-j** *argument obligatoire* : nombre de fils d'ex�cution (1 � 10).  " << "\n"
                    <<  "**-l** *sans argument*        : g�n�rer une colonne de num�ros de ligne intitul�e 'R'.  " << "\n"
                    <<  "**-M** *sans argument*        : ne pas lib�rer la m�moire r�serv�e en fin de programme.   " << "\n"
                    <<  "**-m** *sans argument*        : calculer les maxima d'agents et de lignes de paye.  " << "\n"
                    <<  "**-L** *argument obligatoire* : chemin du log d'ex�cution du test de coh�rence entre analyseurs C et XML.  " << "\n"
                    <<  "**-R** *argument obligatoire* : expression r�guli�re pour la recherche des �lus (cod�s : ELU dans le champ Statut.  " << "\n"
                    <<  "**-S** *sans argument*        : exporter les champs Budget, Employeur, Siret, Etablissement.  " << "\n"
                    <<  "**-E** *sans argument*        : exporter le champ Echelon.  " << "\n"
                    <<  "**-q** *sans argument*        : limiter la verbosit�.  " << "\n"
                    <<  "**-f** *argument obligatoire* : la ligne de commande est dans le fichier en argument, chaque �l�ment � la ligne.  " << "\n"
                    <<  "**--xhlmem** *arg. oblig.*    : taille des fichiers � analyser en octets.  " << "\n"
                    <<  "**--memshare** *arg. oblig.*  : Part de la m�moire vive utilis�e, de 0 (toute) � 1.  " << "\n"
                    <<  "**--segments** *arg. oblig.*  : nombre minimum de segments de base.  " << "\n"
                    <<  "**--pretend**              : ex�cution sans traitement des fichiers.  " << "\n"
                    <<  "**--verifmem**              : seulement v�rifier la consommation m�moire.  " << "\n"
                    <<  "**--hmarkdown**              : aide en format markdown.  " << "\n";

            #ifdef GENERATE_RANK_SIGNAL
                    cerr  <<  "**-rank** *argument optionnel* : g�n�rer le fichier du rang de la base de paye en cours dans le fichier.  \n";

                   #if defined _WIN32 | defined _WIN64
                    cerr  <<  "                           ou � d�faut dans %USERPROFILE%\\AppData\\Altair\\rank.  \n";
                   #else
                      #if defined __linux__
                        cerr  <<  "                           ou � d�faut dans ~/.local/share/Altair/rank.  \n";
                      #endif
                   #endif
            #endif

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
                // On a trouv� un argument num�rique en ligne de commande

                int32_t size_read = lire_argument(argc, const_cast<char*>(commandline_tab[start + 1].c_str()));

                if (size_read < 0 || size_read > INT32_MAX -1)
                {
                    cerr << ERROR_HTML_TAG "Le nombre de lignes doit �tre compris entre 0 et INT64_MAX" ENDL;
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
                cerr << ERROR_HTML_TAG "Option -s suivi d'un argument obligatoire (s�parateur de champs)." ENDL;
                exit(-100);
            }
            info.separateur = commandline_tab[start + 1][0];

            if (info.separateur == '_')
            {
                perror(ERROR_HTML_TAG "Le s�parateur ne doit pas �tre '_'");
                exit(-909);
            }

            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "-d")
        {
            if (start + 1 == argc)
            {
                cerr << ERROR_HTML_TAG "Option -d suivi d'un argument obligatoire (s�parateur d�cimal)." ENDL;
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
                perror(ERROR_HTML_TAG "La base de donn�es ne peut �tre cr��e, v�rifier l'existence du dossier.");
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
                cerr << ERROR_HTML_TAG "La base de donn�es "
                          << info.chemin_base  + string(CSV) << " ne peut �tre cr��e, v�rifier l'existence du dossier." ENDL ;
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
                    perror(ERROR_HTML_TAG "Le nombre de fils d'ex�cution doit �tre compris au moins �gal � 2.");
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
                perror(ERROR_HTML_TAG "Le log ne peut �tre cr��, v�rifier l'existence du dossier.");
                exit(-114);
            }
            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "-N")
        {
            if ((info.nbLigneUtilisateur = lire_argument(argc, const_cast<char*>(commandline_tab[start +1].c_str()))) > 1)
            {
                cerr << STATE_HTML_TAG "Nombre maximum de lignes de paye red�fini � : " << info.nbLigneUtilisateur << ENDL;
            }

            info.reduire_consommation_memoire = false;
            if ((info.nbAgentUtilisateur = lire_argument(argc, const_cast<char*>(commandline_tab[start + 1].c_str()))) < 1)
            {
                cerr << ERROR_HTML_TAG "Pr�ciser le nombre de nombre maximum d'agents par mois attendus (majorant du nombre) avec -n xxx" ENDL;
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
                perror(ERROR_HTML_TAG "Il manque l'expression r�guli�re.");
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
                        cerr << ERROR_HTML_TAG "La ligne de commande -f ne peut pas �tre incluse dans un fichier par -f [risque de boucle infinie].\n";
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
          info.pretend = true;   // pas de d�codage
          ++start;
          continue;
        }
        else if (commandline_tab[start] == "--verifmem")
        {
          info.verifmem = true;  // seulement v�rifier la m�moire
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
                  cerr << ERROR_HTML_TAG "La donn�e de la taille des fichiers en input est erron�e ou au-del� de la limite permise (60 Go)." << ENDL;
                  exit(-199);
                }
          }
          else
          {
                  cerr << ERROR_HTML_TAG "Le plus grand entier non sign� est inf�rieur � 60 * 1048576" << ENDL;
                  exit(-199);
          }
        }
        else if (commandline_tab[start] == "--memshare")
          {
            int part = stoi(commandline_tab[start + 1], nullptr);

            cerr << STATE_HTML_TAG "Part de la m�moire vive utilis�e : " <<  part << " %" ENDL;

            ajustement = (float) part / 100;
            start += 2;
            continue;
          }
        else if (commandline_tab[start] == "--segments ")
        {
           cerr << STATE_HTML_TAG "Les bases seront scind�es en au moins : " << commandline_tab[start + 1] << " segments" << ENDL;

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
        else if (commandline_tab[start] == "--overhead ")
        {
          overhead = stoi(commandline_tab[start + 1], nullptr) * 1048576;
          cerr << STATE_HTML_TAG "Marge de m�moire vive sous plafond : " << commandline_tab[start + 1] << " Mo" << ENDL;
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

    init:

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
                cerr << ERROR_HTML_TAG "La taille du fichier " << commandline_tab.at(i) << " n'a pas pu �tre d�termin�e." ENDL;
            }
        }

        cerr << ENDL STATE_HTML_TAG << "Taille totale des " << count << " fichiers : " << memoire_xhl / 1048576 << " Mo."  ENDL;
        cerr << STATE_HTML_TAG
                  << "M�moire disponible " <<  ((memoire_disponible = getFreeSystemMemory()) / 1048576)
                  << " / " << (getTotalSystemMemory()  / 1048576)
                  << " Mo."  ENDL;
    }

    /* ajustement repr�sente la part maximum de la m�moire disponible que l'on consacre au processus, compte tenu de la marge sous plafond (overhead) */

    unsigned long long memoire_utilisable = floor(ajustement * static_cast<float>(memoire_disponible)) - overhead;
    cerr << STATE_HTML_TAG  << "M�moire utilisable " <<  memoire_utilisable / 1048576   << " Mo."  ENDL;

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
                  cerr << ERROR_HTML_TAG "Impossible de g�n�rer " << nsegments << " segments. Utilisation d'au moins " << nsegments + 1 << " segments." ENDL;
                  ++nsegments;
               }
             else
               {
                  memoire_utilisable = memoire_xhl / (nsegments - 1);
                  break;
               }
         }
    }

    vector<vector<string>> segments;

    auto  taille_it = taille.begin();
    auto  commandline_it = commandline_tab.begin() + start;
    static int loop1, loop2;

    do
    {
        unsigned long long taille_segment = *taille_it;
        vector<string> segment;
        loop1++;

        while (taille_segment * AVERAGE_RAM_DENSITY < memoire_utilisable && commandline_it != commandline_tab.end())
         {
           segment.push_back(*commandline_it);  // ne pas utiliser move;
           ++commandline_it;
           ++taille_it;
           taille_segment  += *taille_it;
           loop2++;
         }

        segments.emplace_back(segment);

    } while (commandline_it != commandline_tab.end());

    int segments_size = segments.size();
    if (segments_size > 1)
        cerr << PROCESSING_HTML_TAG << "Les bases en sortie seront scind�es en " << segments_size << " segments." ENDL;

   bool restart = false;

    for (auto&& segment : segments)
    {
        if (info.nbfil > segment.size() /2 + 2)
        {
            cerr << ERROR_HTML_TAG "Trop de fils pour le nombre de fichiers ; ex�cution avec -j 2" ENDL;
            info.nbfil = 2;
            taille.clear();
            segments.clear();
            memoire_xhl = 0;
            commandline_it = commandline_tab.begin();
            restart = true;
            break;
        }

        produire_segment(info, segment);
    }


    if (restart) goto init;

    xmlCleanupParser();

    auto endofprogram = Clock::now();

    cerr << ENDL << PROCESSING_HTML_TAG "Dur�e d'ex�cution : "
              << chrono::duration_cast<chrono::milliseconds>(endofprogram - startofprogram).count()
              << " millisecondes" << ENDL;

    if (rankFile.is_open()) rankFile.close();

    return errno;
}

int produire_segment(const info_t& info, const vector<string>& segment)
{



    vector<int> nb_fichier_par_fil;
    int segment_size = segment.size();

    int remainder = segment_size % info.nbfil;

    for (unsigned i = 0; i < info.nbfil; ++i)
    {
         nb_fichier_par_fil.push_back(segment_size / info.nbfil + (remainder > 0));
         --remainder;
    }

    /* on r�partir le reste de mani�re �quilibr�e sur les premiers fils */

    vector<info_t> Info(info.nbfil);
    vector<thread> t;

    if (info.nbfil > 1)
    {
        t.resize(info.nbfil);
    }

    if (verbeux)
    {
        cerr << PROCESSING_HTML_TAG "Cr�ation de " << info.nbfil << " fils clients." ENDL;
    }

    vector<thread_t> v_thread_t(info.nbfil);
    vector<string>::const_iterator segment_it = segment.begin();

    for (unsigned  i = 0; i < info.nbfil; ++i)
    {
        Info[i] = info;

        Info[i].threads = &v_thread_t[i];
        Info[i].threads->thread_num = i;

        Info[i].threads->argc = nb_fichier_par_fil.at(i);

        Info[i].threads->argv = vector<string>(segment_it, segment_it + nb_fichier_par_fil[i]);

        Info[i].threads->in_memory_file = vector<string>(nb_fichier_par_fil[i]);
        segment_it += nb_fichier_par_fil.at(i);

        if (Info[i].threads->argv.size() != (unsigned) nb_fichier_par_fil.at(i))
        {
            cerr << ERROR_HTML_TAG "Probl�me issu de l'allocation des threads" << ENDL ;
            exit(-145);
        }

        if (verbeux)
            cerr <<  PROCESSING_HTML_TAG "Fil d'ex�cution i = " << i+1 << "/" << info.nbfil
                 << "   Nombre de fichiers dans ce fil : " << nb_fichier_par_fil[i] << ENDL;

        errno = 0;

        /* Lancement des fils d'ex�cution */

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
       cerr << ENDL PROCESSING_HTML_TAG "Premier scan des fichiers pour d�terminer les besoins m�moire ... " ENDL;

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
      cerr << ENDL << PROCESSING_HTML_TAG "Exportation des bases de donn�es au format CSV..." << ENDL ENDL;
      boucle_ecriture(Info);
    }

    /* R�sum� des erreurs rencontr�es */

    if (errorLineStack.size() > 0)
    {
        cerr << WARNING_HTML_TAG "<b>R�capitulatif des erreurs rencontr�es</b>" << ENDL;
        for (const errorLine_t& e :  errorLineStack)
        {
            cerr << e.filePath;
            if (e.lineN != -1)
                cerr << " -- Ligne n�" << e.lineN << ENDL;
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
                    LOG << " -- Ligne n�" << e.lineN << ENDL;
                else
                    LOG << " -- Ligne inconnue." << ENDL;
            }
    }

    /* lib�ration de la m�moire */

   if (! liberer_memoire) return 0;

   if (verbeux)
       cerr << ENDL
                     << PROCESSING_HTML_TAG "Lib�ration de la m�moire..."
                     << ENDL;

    /* En cas de probl�me d'allocation m�moire le mieux est encore de ne pas d�sallouer car on ne connait pas exacteemnt l'�tat
     * de la m�moire dynamique */

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


