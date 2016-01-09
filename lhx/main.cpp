/*  Programme �crit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est �dit�, le pr�sent code doit �tre converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'�tre compil�.
 *  En entr�e d'Altair pr�ciser encodage.entr�e en conformit� avec l'encodage du pr�sent fichier, qui sera celui de la base g�n�r�e.
 */


#include <mutex>
#ifdef __linux__
#include <unistd.h>
#endif
#include <iostream>
#include <vector>
#include <thread>
#include <map>
#include <chrono>
#include <cstring>
#include "validator.hpp"
#include "fonctions_auxiliaires.hpp"
#include "table.hpp"
#include "tags.h"

bool verbeux = true;
typedef std::chrono::high_resolution_clock Clock;
std::ofstream rankFile;
std::string rankFilePath = "";
std::mutex mut;
std::vector<errorLine_t> errorLineStack;


int main(int argc, char **argv)
{
    auto startofprogram = Clock::now();

#if defined _WIN32 | defined _WIN64
    setlocale(LC_ALL, "French_France.1252"); // Windows ne g�re pas UTF-8 en locale
    //std::locale::global(std::locale("French_France.1252"));
#elif defined __linux__
    //setlocale(LC_ALL, "fr_FR.utf8");
   std::locale::global(std::locale("fr_FR.utf8"));
#else
#error "Programme con�u pour Windows ou linux"
#endif

    if (argc < 2)
    {
        std::cerr << ERROR_HTML_TAG "Il faut au moins un fichier � analyser.\n" ;
        return -2;
    }

    LIBXML_TEST_VERSION
            xmlKeepBlanksDefault(0);

    int start = 1;
    std::string type_table = "bulletins";
    bool generer_table = false;
    bool liberer_memoire = true;
    std::vector<std::string> cl;  /* pour les lignes de commandes incluses dans un fichier */
    std::string chemin_base = NOM_BASE + std::string(CSV);
    std::string chemin_bulletins = NOM_BASE_BULLETINS + std::string(CSV);

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
        std::vector<uint16_t>(),             //    std::vector<uint16_t> NLigne;
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
        true,             //    all�ger la base
        1,                 // nbfil
        {}  // besoin de m�moire effectif
    };

    /* Analyse de la ligne de commande */

    std::vector<std::string> commandline_tab;
    commandline_tab.assign(argv, argv + argc);

    while (start < argc)
    {
        if (commandline_tab[start] == "-n")
        {
            info.reduire_consommation_memoire = false;
            if ((info.nbAgentUtilisateur = lire_argument(argc, const_cast<char*>(commandline_tab[start + 1].c_str()))) < 1)
            {
                std::cerr << ERROR_HTML_TAG "Pr�ciser le nombre de bulletins mensuels attendus (majorant du nombre) avec -N xxx .\n";
                exit(-1);
            }
            start += 2;
            continue;

        }
        else if (commandline_tab[start] ==  "-h")
        {
            std::cerr <<  "Usage :  lhx OPTIONS fichiers.xhl" << "\n"
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
                      <<  "-S sans argument        : supprimer la sortie Budget, Employeur, Siret, Etablissement (all�ge les bases)." << "\n"
                      <<  "-q sans argument        : limiter la verbosit�." << "\n"
                      <<  "-f argument obligatoire : la ligne de commande est dans le fichier en argument, chaque �l�ment � la ligne." << "\n";

              #ifdef GENERATE_RANK_SIGNAL

                      std::cerr  <<  "-rank argument optionnel : g�n�rer le fichier du rang de la base de paye en cours dans le fichier.\n";

                     #if defined _WIN32 | defined _WIN64
                      std::cerr  <<  "                           ou � d�faut dans %USERPROFILE%\\AppData\\Altair\\rank.\n";
                     #else
                        #if defined __linux__
                          std::cerr  <<  "                           ou � d�faut dans ~/.local/share/Altair/rank.\n";
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
                std::cerr << ERROR_HTML_TAG "Option -T suivi d'un argument obligatoire (nombre de lignes)." ENDL ;
                exit(-100);
            }

            std::map<std::string, BaseType> hashTable;

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
                int32_t size_read = lire_argument(argc, const_cast<char*>(commandline_tab[start + 1].c_str()));

                if (size_read < 0 || size_read > INT32_MAX -1)
                {
                    std::cerr << ERROR_HTML_TAG "Le nombre de lignes doit �tre compris entre 0 et INT64_MAX" ENDL;
                    exit(-908);
                }
                else
                    info.taille_base = size_read;
            }

            start += 2;
            continue;

        }
        else if (commandline_tab[start] == "-s")
        {
            if (start + 1 == argc)
            {
                std::cerr << ERROR_HTML_TAG "Option -s suivi d'un argument obligatoire (s�parateur de champs)." ENDL;
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
                std::cerr << ERROR_HTML_TAG "Option -d suivi d'un argument obligatoire (s�parateur d�cimal)." ENDL;
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
                std::cerr << ERROR_HTML_TAG "Option -o suivi d'un argument obligatoire (nom de  fichier)." ENDL;
                exit(-100);
            }

            info.chemin_base = commandline_tab[start + 1];

            std::ofstream base;
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
            info.chemin_base = commandline_tab[start + 1] + std::string("/") + std::string(NOM_BASE) ;
            info.chemin_bulletins = commandline_tab[start + 1] + std::string("/") + std::string(NOM_BASE_BULLETINS);
            std::ofstream base;
            base.open(info.chemin_base, std::ofstream::out | std::ofstream::trunc);

            if (! base.good())
            {
                std::cerr << ERROR_HTML_TAG "La base de donn�es "
                          << info.chemin_base  + std::string(CSV) << " ne peut �tre cr��e, v�rifier l'existence du dossier." ENDL ;
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
            std::ofstream base;
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
                std::cerr << STATE_HTML_TAG " Nombre maximum de lignes de paye red�fini � : " << info.nbLigneUtilisateur << ENDL;
            }

            info.reduire_consommation_memoire = false;
            if ((info.nbAgentUtilisateur = lire_argument(argc, const_cast<char*>(commandline_tab[start + 1].c_str()))) < 1)
            {
                std::cerr << ERROR_HTML_TAG "Pr�ciser le nombre de nombre maximum d'agents par mois attendus (majorant du nombre) avec -n xxx" ENDL;
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
                    rankFilePath = std::string(std::getenv("USERPROFILE")) + "/AppData/rank";
                else
                {
                    rankFilePath = commandline_tab[start + 1];
                    hasArg = 1;
                }

                std::ifstream testFile;
                testFile.open(rankFilePath);
                if (testFile.is_open())
                {
                    testFile.close();
                }

                rankFile.open(rankFilePath, std::ios::out| std::ios::trunc);
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
                info.select_siret = false;
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
                std::cerr << ERROR_HTML_TAG "Il manque le fichier de ligne de commande.\n";
                exit(-119);
            }

            std::ifstream f;
            f.open(fichier);
            if (! f.good())
            {
                std::cerr << ERROR_HTML_TAG "Impossible d'ouvrir le fichier " << fichier << ".\n";
                exit(-119);
            }

            std::string ligne;
            if (f.good())
            {
              while(f.rdstate() != std::fstream::eofbit)
              {
                std::getline(f, ligne);

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

                    if (std::string(cl.at(i)) == "-f")
                    {
                        std::cerr << ERROR_HTML_TAG "La ligne de commande -f ne peut pas �tre incluse dans un fichier par -f [risque de boucle infinie].\n";
                    }
                }


                /* on relance ...*/

                start = 1;
                continue;
            }
            //break;
        }
        else if (commandline_tab[start][0] == '-')
        {
            std::cerr << ERROR_HTML_TAG "Option inconnue " << commandline_tab[start] << ENDL;
            exit(-100);
        }
        else break;
    }

    /* Fin de l'analyse de la ligne de commande */

    xmlInitMemory();
    xmlInitParser();

    /* on sait que info.nbfill >= 1 */

    init:

    int nb_total_fichiers = argc - start;

    if (info.nbfil > nb_total_fichiers /2 + 2)
    {
        std::cerr << ERROR_HTML_TAG "Trop de fils pour le nombre de fichiers ; ex�cution avec -j 2" ENDL;
        info.nbfil = 2;
        goto init;
    }

    std::vector<int> nb_fichier_par_fil;

    int remainder = nb_total_fichiers % info.nbfil;

    for (int i = 0; i < info.nbfil; ++i)
    {
         nb_fichier_par_fil.push_back(nb_total_fichiers / info.nbfil + (remainder > 0));
         --remainder;
    }

    /* on r�partir le reste de mani�re �quilibr�e sur les premiers fils */

    std::vector<info_t> Info(info.nbfil);
    std::vector<std::thread> t;

    if (info.nbfil > 1)
    {
        t.resize(info.nbfil);
    }

    if (verbeux)
    {
        std::cerr << PROCESSING_HTML_TAG "Cr�ation de " << info.nbfil << " fils clients." ENDL;
    }

    std::vector<thread_t> v_thread_t(info.nbfil);

    for (int i = 0; i < info.nbfil; ++i)
    {
        Info[i] = info;

        Info[i].threads = &v_thread_t[i];
        Info[i].threads->thread_num = i;

        Info[i].threads->argc = nb_fichier_par_fil[i];

        Info[i].threads->argv = std::vector<std::string>(&commandline_tab[start], &commandline_tab[start + nb_fichier_par_fil[i]]);
        Info[i].threads->in_memory_file =std::vector<std::string>(nb_fichier_par_fil[i]);
        start += nb_fichier_par_fil[i];

        if (Info[i].threads->argv.size() != (unsigned) nb_fichier_par_fil[i])
        {
            std::cerr << ERROR_HTML_TAG "Probl�me issu de l'allocation des threads" << ENDL ;
            exit(-145);
        }

        if (verbeux)
            std::cerr <<  PROCESSING_HTML_TAG " File d'ex�cution i = " << i+1 << "/" << info.nbfil
                      << " Nombre de fichiers dans ce fil : " << nb_fichier_par_fil[i] << ENDL;

        errno = 0;

        /* Lancement des fils d'ex�cution */

        if (info.nbfil > 1)
        {
            std::thread th{decoder_fichier, std::ref(Info[i])};
            t[i] = std::move(th);
        }
        else
        {
            decoder_fichier(std::ref(Info[0]));
        }

        if (errno)
        {
            std::cerr << ENDL << strerror(errno) << ENDL;
        }
    }

    if (verbeux && Info[0].reduire_consommation_memoire)
       std::cerr << ENDL PROCESSING_HTML_TAG "Premier scan des fichiers pour d�terminer les besoins m�moire ... " ENDL;

    if (info.nbfil > 1)
        for (int i = 0; i < info.nbfil; ++i)
        {
            t[i].join ();
        }


    if (Info[0].calculer_maxima)
    {
        calculer_maxima(Info);
    }

    if (! Info[0].chemin_log.empty())
    {
        std::ofstream LOG;
        LOG.open(Info[0].chemin_log, std::ios::app);
        calculer_maxima(Info, &LOG);
        LOG.close();
    }

    xmlCleanupParser();

    auto endofcalculus = Clock::now();

    std::cerr << ENDL << PROCESSING_HTML_TAG "Dur�e de calcul : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endofcalculus - startofprogram).count()
              << " millisecondes" << ENDL;


    if (generer_table)
    {
      std::cerr << ENDL << PROCESSING_HTML_TAG "Exportation des bases de donn�es au format CSV..." << ENDL;
      boucle_ecriture(Info);
    }

    /* R�sum� des erreurs rencontr�es */

    if (errorLineStack.size() > 0)
    {
        std::cerr << WARNING_HTML_TAG "<b>R�capitulatif des erreurs rencontr�es</b>" << ENDL;
        for (const errorLine_t& e :  errorLineStack)
        {
            std::cerr << e.filePath;
            if (e.lineN != -1)
                std::cerr << " -- Ligne n�" << e.lineN << ENDL;
            else
                std::cerr << " -- Ligne inconnue." << ENDL;

        }
    }

    if (! Info[0].chemin_log.empty())
    {
        std::ofstream LOG;
        LOG.open(Info[0].chemin_log, std::ios::app);
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

   if (! liberer_memoire) goto duration;

   if (verbeux)
       std::cerr << ENDL
                     << PROCESSING_HTML_TAG "Lib�ration de la m�moire..."
                     << ENDL;

    /* En cas de probl�me d'allocation m�moire le mieux est encore de ne pas d�sallouer car on ne connait pas exacteemnt l'�tat
     * de la m�moire dynamique */

    for (int i = 0; i < Info[0].nbfil; ++i)
    {
        for (unsigned agent = 0; agent < Info[i].NCumAgent; ++agent)
        {
            for (int j = 0; j < Info[i].Memoire_p_ligne[agent]; ++j)
            {
                if (j != Categorie) xmlFree(Info[i].Table[agent][j]);
            }
        }
    }

duration:

    auto endofprogram = Clock::now();

    std::cerr << ENDL << PROCESSING_HTML_TAG "Dur�e d'ex�cution : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endofprogram - startofprogram).count()
              << " millisecondes" << ENDL;

    if (rankFile.is_open()) rankFile.close();

    return errno;
}

