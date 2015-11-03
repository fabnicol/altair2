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

static inline const uint32_t* calculer_maxima(const std::vector<info_t> &Info)
{
    static int once;

    uint32_t* maximum= new uint32_t[2] {0, 0};

    if (once || maximum == nullptr) return nullptr;  // no-op)

    for (int i = 0; i < Info[0].nbfil; ++i)
    {
        for (unsigned j = 0; j < Info[i].threads->argc; ++j)
            if (Info[i].NAgent[j] > maximum[0])
                maximum[0] = Info[i].NAgent[j];

        for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; ++agent)
            if (Info[i].NLigne[agent] > maximum[1])
                maximum[1] = Info[i].NLigne[agent];
    }

    ++once;
    return(maximum);
}

int main(int argc, char **argv)
{
    auto startofprogram = Clock::now();

#if defined _WIN32 | defined _WIN64
    setlocale(LC_NUMERIC, "French_France.1252"); // Windows ne g�re pas UTF-8 en locale
#elif defined __linux__
    setlocale(LC_NUMERIC, "fr_FR.utf8");
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

    std::string chemin_base = NOM_BASE + std::string(CSV);
    std::string chemin_bulletins = NOM_BASE_BULLETINS + std::string(CSV);

    thread_t mon_thread;

    info_t info =
    {
        nullptr,             //    bulletinPtr* Table;
        0,                //    uint64_t nbLigne;
        nullptr,             //    int32_t  *NAgent;
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
        nullptr  // besoin de m�moire effectif
    };

    /* Analyse de la ligne de commande */

    while (start < argc)
    {
        if (! strcmp(argv[start], "-n"))
        {
            info.reduire_consommation_memoire = false;
            if ((info.nbAgentUtilisateur = lire_argument(argc, argv[start + 1])) < 1)
            {
                std::cerr << ERROR_HTML_TAG "Pr�ciser le nombre de bulletins mensuels attendus (majorant du nombre) avec -N xxx .\n";
                exit(-1);
            }
            start += 2;
            continue;

        }
        else if (! strcmp(argv[start], "-h"))
        {
            std::cout <<  "Usage :  lhx OPTIONS fichiers.xhl" << "\n"
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
                      <<  "-S sans argument        : supprimer la sortie Budget, Etablissement, Siret (all�ge les bases)." << "\n"
                      <<  "-q sans argument        : limiter la verbosit�." << "\n";

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
        else if (! strcmp(argv[start], "-q"))
        {
            verbeux = false;
            ++start;
            continue;
        }
        else if (! strcmp(argv[start], "-t"))
        {
            generer_table = true;
            if (! strcmp(argv[start + 1], "standard"))
            {
                type_table = argv[start + 1];
                start += 2;
                continue;
            }
            else
            {
                ++start;
                continue;
            }
        }
        else if (! strcmp(argv[start], "-l"))
        {
            info.generer_rang = true ;
            ++start;
            continue;
        }
        else if (! strcmp(argv[start], "-T"))
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

            if (hashTable.find(argv[start + 1]) != hashTable.end())
            {
                info.type_base = hashTable[argv[start + 1]];
            }
            else
            {
                int32_t size_read = lire_argument(argc, argv[start + 1]);

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
        else if (! strcmp(argv[start], "-s"))
        {
            if (start + 1 == argc)
            {
                std::cerr << ERROR_HTML_TAG "Option -s suivi d'un argument obligatoire (s�parateur de champs)." ENDL;
                exit(-100);
            }
            info.separateur = argv[start + 1][0];

            if (info.separateur == '_')
            {
                perror(ERROR_HTML_TAG "Le s�parateur ne doit pas �tre '_'");
                exit(-909);
            }

            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-g"))  // no-op pour l'interface graphique
        {
            if (start + 1 == argc)
            {
                std::cerr << ERROR_HTML_TAG "Option -g suivi d'un argument obligatoire." ENDL;
                exit(-100);
            }

            start += 1;
            continue;
        }
        else if (! strcmp(argv[start], "-d"))
        {
            if (start + 1 == argc)
            {
                std::cerr << ERROR_HTML_TAG "Option -d suivi d'un argument obligatoire (s�parateur d�cimal)." ENDL;
                exit(-100);
            }
            info.decimal = argv[start + 1][0];
            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-o"))
        {
            if (start + 1 == argc)
            {
                std::cerr << ERROR_HTML_TAG "Option -o suivi d'un argument obligatoire (nom de  fichier)." ENDL;
                exit(-100);
            }

            info.chemin_base = argv[start + 1];

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
        else if (! strcmp(argv[start], "-M"))
        {
            liberer_memoire = false;
            ++start;
            continue;
        }
        else if (! strcmp(argv[start], "-m"))
        {
            info.calculer_maxima = true;
            ++start;
            continue;
        }
        else if (! strcmp(argv[start], "-D"))
        {
            info.chemin_base = argv[start + 1] + std::string("/") + std::string(NOM_BASE);
            info.chemin_bulletins = argv[start + 1] + std::string("/") + std::string(NOM_BASE_BULLETINS);
            std::ofstream base;
            base.open(info.chemin_base);

            if (! base.good())
            {
                std::cerr << ERROR_HTML_TAG "La base de donn�es "
                          << info.chemin_base << " ne peut �tre cr��e, v�rifier l'existence du dossier." ENDL ;
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
        else if (! strcmp(argv[start], "-j"))
        {
            if ((info.nbfil = lire_argument(argc, argv[start +1])) > 0)
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
        else if (! strcmp(argv[start], "-L"))
        {
            if (argc > start +2) info.chemin_log = argv[start + 1];
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
        else if (! strcmp(argv[start], "-N"))
        {
            if ((info.nbLigneUtilisateur = lire_argument(argc, argv[start +1])) > 1)
            {
                std::cerr << STATE_HTML_TAG " Nombre maximum de lignes de paye red�fini � : " << info.nbLigneUtilisateur << ENDL;
            }

            info.reduire_consommation_memoire = false;
            if ((info.nbAgentUtilisateur = lire_argument(argc, argv[start + 1])) < 1)
            {
                std::cerr << ERROR_HTML_TAG "Pr�ciser le nombre de nombre maximum d'agents par mois attendus (majorant du nombre) avec -n xxx" ENDL;
                exit(-1);
            }

            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-R"))
        {
            if (argc > start +2)
            {
                info.expression_reg_elus = argv[start + 1];
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
        else if (! strcmp(argv[start], "-rank"))
        {
            int hasArg = 0;
            if (argc > start +2)
            {
                if (argv[start + 1][0] == '-')
                    rankFilePath = std::string(std::getenv("USERPROFILE")) + "/AppData/rank";
                else
                {
                    rankFilePath = argv[start + 1];
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

        else if (! strcmp(argv[start], "-S"))
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
        else if (argv[start][0] == '-')
        {
            std::cerr << ERROR_HTML_TAG "Option inconnue " << argv[start] << ENDL;
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
         nb_fichier_par_fil.emplace_back(nb_total_fichiers / info.nbfil + (remainder > 0));
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


    if (verbeux && Info[0].reduire_consommation_memoire)
       std::cerr << ENDL PROCESSING_HTML_TAG "Premier scan des fichiers pour d�terminer les besoins m�moire ... " ENDL;

    for (int i = 0; i < info.nbfil; ++i)
    {
        Info[i] = info;
        Info[i].threads = new thread_t;
        Info[i].threads->thread_num = i;

        Info[i].threads->argc = nb_fichier_par_fil[i];
        Info[i].threads->argv = new char*[nb_fichier_par_fil[i]]();

        if (Info[i].threads->argv == nullptr)
        {
            perror(ERROR_HTML_TAG "Probl�me issu de l'allocation des threads");
            exit(-145);
        }

        for (int j = start; j <  nb_fichier_par_fil[i] + start; ++j)
        {
            Info[i].threads->argv[j - start] = argv[j];
        }

        if (verbeux)
            std::cerr <<  PROCESSING_HTML_TAG "Thread i=" << i+1 << "/" << info.nbfil
                      << " Nombre de fichiers : " << nb_fichier_par_fil[i] << ENDL;

        start += nb_fichier_par_fil[i];

        errno = 0;

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

    if (info.nbfil > 1)
        for (int i = 0; i < info.nbfil; ++i)
        {
            t[i].join ();
        }

    const uint32_t*   maxima = nullptr;

    if (Info[0].calculer_maxima)
    {
        maxima = calculer_maxima(Info);
        if (maxima)
        {
            std::cerr <<  STATE_HTML_TAG "Maximum de lignes : " << maxima[1] << ENDL
                       << STATE_HTML_TAG "Maximum d'agents  : " << maxima[0] << ENDL;
        }
    }

    if (maxima == nullptr) maxima = calculer_maxima(Info);

    if (! Info[0].chemin_log.empty())
    {
        std::ofstream LOG;
        LOG.open(Info[0].chemin_log, std::ios::app);
        if (LOG.good() && maxima)
        {
            LOG << STATE_HTML_TAG "Maximum de lignes : " << maxima[1] << "\n";
            LOG << STATE_HTML_TAG "Maximum d'agent   : " << maxima[0] << "\n";
            LOG.close();
        }
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
        std::cerr << WARNING_HTML_TAG "<g>R�capitulatif des erreurs rencontr�es</g>" << ENDL;
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

    int valeur_de_retour = 0;
    if (! liberer_memoire) goto duration;

   if (verbeux)  std::cerr << ENDL << PROCESSING_HTML_TAG "Lib�ration de la m�moire..." << ENDL;

    /* En cas de probl�me d'allocation m�moire le mieux est encore de ne pas d�sallouer car on ne connait pas exacteemnt l'�tat
     * de la m�moire dynamique */

    for (int i = 0; i < Info[0].nbfil; ++i)
    {
        for (unsigned agent = 0; agent < Info[i].NCumAgent; ++agent)
        {

            for (int j = 0; j < Info[i].Memoire_p_ligne[agent]; ++j)
                if (Info[i].Table[agent][j] != nullptr)
                {}//delete [] (Info[i].Table[agent][j]);

            //delete [] (Info[i].Table[agent]);
        }

        delete [] (Info[i].NAgent);
        delete [] (Info[i].threads->argv);
        delete [] (Info[i].Table);
        delete [] (Info[i].Memoire_p_ligne);

        if (Info[0].nbfil > 1)
        {
            delete [] (Info[i].threads);
        }
    }

    valeur_de_retour = (maxima)? 2 * maxima[0] + 3 * maxima[1]: 0;

    if (maxima) delete [] (maxima);

duration:

    auto endofprogram = Clock::now();

    std::cerr << ENDL << PROCESSING_HTML_TAG "Dur�e d'ex�cution : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endofprogram - startofprogram).count()
              << " millisecondes" << ENDL;

    if (rankFile.is_open()) rankFile.close();

    return valeur_de_retour;
}

