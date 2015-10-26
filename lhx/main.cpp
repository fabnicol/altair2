/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
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

typedef std::chrono::high_resolution_clock Clock;
std::ofstream rankFile;
char* rankFilePath;
std::mutex mut;

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
    setlocale(LC_NUMERIC, "French_France.1252"); // Windows ne gère pas UTF-8 en locale
#elif defined __linux__
    setlocale(LC_NUMERIC, "fr_FR.utf8");
#else
#error "Programme conçu pour Windows ou linux"
#endif

    if (argc < 2)
    {
        std::cerr << "Erreur : Il faut au moins un fichier à analyser.\n" ;
        return -2;
    }

    LIBXML_TEST_VERSION
            xmlKeepBlanksDefault(0);

    int start = 1;
    std::string type_table = "bulletins";
    bool generer_table = false;
    bool liberer_memoire = true;

    std::string chemin_base = NOM_BASE CSV;
    std::string chemin_bulletins = NOM_BASE_BULLETINS CSV;

    thread_t mon_thread;

    info_t info =
    {
        nullptr,             //    bulletinPtr* Table;
        0,                //    uint64_t nbLigne;
        nullptr,             //    int32_t  *NAgent;
        0,                //    uint32_t nbAgentUtilisateur
        0,                //    uint32_t NCumAgent;
        0,                //    uint32_t NCumAgentXml;
        MONOLITHIQUE,                //    taille base : non limitée par défaut
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
        true,             // réduire coso mémoire
        true,             // par défaut lire la balise adjacente
        false,            // calculer les maxima de lignes et d'agents
        false,            // numéroter les lignes
        true,             //    alléger la base
        BESOIN_MEMOIRE_ENTETE,// besoin mémoire minimum hors lecture de lignes : devra être incrémenté,
        info.minimum_memoire_p_ligne  // chaque agent a au moins BESOIN_MEMOIRE_ENTETE champs du bulletins de paye en colonnes
                                     // sans la table ces champs sont répétés à chaque ligne de paye.
        + nbType // espace pour les drapeaux de séparation des champs (taille de type_remuneration). Nécessaire pour l'algorithme
        + (MAX_LIGNES_PAYE)*(INDEX_MAX_CONNNES + 1),   // nombre de lignes de paye x nombre maximum de types de balises distincts de lignes de paye
                                                       // soit N+1 pour les écritures du type Var(l+i), i=0,...,N dans ECRIRE_LIGNE_l_COMMUN
        1                 // nbfil
    };

    while (start < argc)
    {
        if (! strcmp(argv[start], "-n"))
        {
            info.reduire_consommation_memoire = false;
            if ((info.nbAgentUtilisateur = lire_argument(argc, argv[start + 1])) < 1)
            {
                std::cerr << "Erreur : Préciser le nombre de bulletins mensuels attendus (majorant du nombre) avec -N xxx .\n";
                exit(-1);
            }
            start += 2;
            continue;

        }
        else if (! strcmp(argv[start], "-h"))
        {
            std::cout <<  "Usage :  lhx OPTIONS fichiers.xhl" << "\n"
                      <<  "OPTIONS :" << "\n"
                      <<  "-n argument obligatoire : nombre maximum de bulletins mensuels attendus [calcul exact par défaut]" << "\n"
                      <<  "-N argument obligatoire : nombre maximum de lignes de paye attendues [calcul exact par défaut]" << "\n"
                      <<  "-t argument optionnel   : type de base en sortie, soit 'standard', soit 'bulletins' [défaut bulletins]." << "\n"
                      <<  "-T argument obligatoire : nombre de lignes maximum par base .csv [défaut illimité]. Au plus 999 tables seront générées." << "\n"
                      <<  "-T AN                    : générer une table par année" << "\n"
                      <<  "-T A/AC/AV/C/D/I/IR/RE/S/T : générer une table pour chaque catégorie de ligne : \
                              A rémunérations diverse \n \
                              AC acompte \n \
                              AV avantage en nature \n \
                              C cotisation \n \
                              D déduction \n \
                              I indemnités \n \
                              IR indemnité de résidence \n \
                              RE retenue \n \
                              S supplément familial \n \
                              T traitement brut \n \
                              X toutes catégories\n" << "\n"
                      <<  "-o argument obligatoire : fichier.csv, chemin complet du fichier de sortie [défaut 'Table.csv' avec -t]." << "\n"
                      <<  "-D argument obligatoire : répertoire complet du fichier de sortie [défaut '.' avec -t]." << "\n"
                      <<  "-d argument obligatoire : séparateur décimal [défaut ',' avec -t]." << "\n"
                      <<  "-s argument obligatoire : séparateur de champs [défaut ';' avec -t]. Ne pas utiliser '_'." << "\n"
                      <<  "-j argument obligatoire : nombre de fils d'exécution (1 à 10)." << "\n"
                      <<  "-l sans argument        : générer une colonne de numéros de ligne intitulée 'R'." << "\n"
                      <<  "-M sans argument        : ne pas libérer la mémoire réservée en fin de programme." << "\n"
                      <<  "-m sans argument        : calculer les maxima d'agents et de lignes de paye." << "\n"
                      <<  "-L argument obligatoire : chemin du log d'exécution du test de cohérence entre analyseurs C et XML." << "\n"
                      <<  "-R argument obligatoire : expression régulière pour la recherche des élus (codés : ELU dans le champ Statut." << "\n"
                      <<  "-S sans argument        : supprimer la sortie Budget, Etablissement, Siret (allège les bases)." << "\n";
            exit(0);
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
                std::cerr << "Erreur : Option -T suivi d'un argument obligatoire (nombre de lignes).\n" ;
                exit(-100);
            }

            std::map<std::string, int> hashTable;

            hashTable["AN"] = PAR_ANNEE;
            hashTable["A"]  = PAR_REM_DIVERSES;
            hashTable["AC"] = PAR_ACOMPTE;
            hashTable["AV"] = PAR_AVANTAGE_NATURE;
            hashTable["C"]  = PAR_COTISATION;
            hashTable["D"]  = PAR_DEDUCTION;
            hashTable["I"]  = PAR_INDEMNITE;
            hashTable["IR"] = PAR_INDEMNITE_RESIDENCE;
            hashTable["S"]  = PAR_SFT;
            hashTable["T"]  = PAR_TRAITEMENT;
            hashTable["R"]  = PAR_RAPPEL;
            hashTable["X"]  = TOUTES_CATEGORIES;

            if (hashTable.find(argv[start + 1]) != hashTable.end())
            {
                info.taille_base = hashTable[argv[start + 1]];

            }
            else
                if ((info.taille_base = lire_argument(argc, argv[start + 1])) < 0 || info.taille_base > INT32_MAX -1)
                                {
                                    perror("Erreur : Le nombre de lignes doit être compris entre 0 et INT64_MAX");
                                    exit(-908);
                                }
            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-s"))
        {
            if (start + 1 == argc)
            {
                std::cerr << "Erreur : Option -s suivi d'un argument obligatoire (séparateur de champs).\n";
                exit(-100);
            }
            info.separateur = argv[start + 1][0];

            if (info.separateur == '_')
            {
                perror("Erreur : Le séparateur ne doit pas être '_'");
                exit(-909);
            }

            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-g"))  // no-op pour l'interface graphique
        {
            if (start + 1 == argc)
            {
                std::cerr << "Erreur : Option -g suivi d'un argument obligatoire.\n";
                exit(-100);
            }

            start += 1;
            continue;
        }
        else if (! strcmp(argv[start], "-d"))
        {
            if (start + 1 == argc)
            {
                std::cerr << "Erreur : Option -d suivi d'un argument obligatoire (séparateur décimal).\n";
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
                std::cerr << "Erreur : Option -o suivi d'un argument obligatoire (nom de  fichier).\n";
                exit(-100);
            }

            info.chemin_base = argv[start + 1];

            std::ofstream base;
            base.open(info.chemin_base);
            if (! base.good())
            {
                perror("Erreur : La base de données ne peut être créée, vérifier l'existence du dossier.");
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
            info.chemin_base = argv[start + 1] + std::string("/" NOM_BASE CSV);
            info.chemin_bulletins = argv[start + 1] + std::string("/" NOM_BASE_BULLETINS CSV);
            std::ofstream base;
            base.open(info.chemin_base);

            if (! base.good())
            {
                std::cerr << "Erreur : La base de données "
                          << info.chemin_base << " ne peut être créée, vérifier l'existence du dossier.\n" ;
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
                    perror("Erreur : Le nombre de fils d'exécution doit être compris au moins égal à 2.");
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
                perror("Erreur : Le log ne peut être créé, vérifier l'existence du dossier.");
                exit(-114);
            }
            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-N"))
        {
            if ((info.nbLigneUtilisateur = lire_argument(argc, argv[start +1])) > 1)
            {
                std::cerr << "[MSG] Nombre maximum de lignes de paye redéfini à : " << info.nbLigneUtilisateur << "\n";
            }

            info.reduire_consommation_memoire = false;
            if ((info.nbAgentUtilisateur = lire_argument(argc, argv[start + 1])) < 1)
            {
                std::cerr << "Erreur : Préciser le nombre de nombre maximum d'agents par mois attendus (majorant du nombre) avec -n xxx\n";
                exit(-1);
            }

            start += 2;
            info.memoire_p_ligne = info.minimum_memoire_p_ligne  // chaque agent a au moins BESOIN_MEMOIRE_ENTETE champs du bulletins de paye en colonnes
                                                                                  // sans la table ces champs sont répétés à chaque ligne de paye.
                                        + nbType // espace pour les drapeaux de séparation des champs (taille de type_remuneration). Nécessaire pour l'algorithme
                                        + (info.nbLigneUtilisateur)*(INDEX_MAX_CONNNES + 1);   // nombre de lignes de paye x nombre maximum de types de balises distincts de lignes de paye
                                                                                                        // soit N+1 pour les écritures du type Var(l+i), i=0,...,N dans ECRIRE_LIGNE_l_COMMUN
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
                perror("Erreur : Il manque l'expression régulière.");
                exit(-115);
            }
            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-rank"))
        {
            if (argc > start +2)
            {
                std::ifstream testFile;
                testFile.open(argv[start + 1]);
                if (testFile.is_open())
                {
                    testFile.close();
                    remove(argv[start + 1]);
                }

                rankFilePath = argv[start + 1];
                rankFile.open(rankFilePath, std::ios::out| std::ios::trunc);
                rankFile << 1 << "\n";
                rankFile.close();

            }

            start += 2;
            continue;
        }

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
            std::cerr << "Erreur : Option inconnue " << argv[start] << "\n";
            exit(-100);
        }
        else break;
    }

    xmlInitMemory();
    xmlInitParser();

    int nbfichier_par_fil = (int) (argc - start) / info.nbfil;
    if (nbfichier_par_fil == 0)
    {
        std::cerr << "Erreur : Trop de fils pour le nombre de fichiers ; exécution avec -j 2\n";
        info.nbfil = 2;
    }

    if ((argc - start) % info.nbfil) ++info.nbfil;  // on en crée un de plus pour le reste

    std::vector<info_t> Info(info.nbfil);
    std::vector<std::thread> t;

    if (info.nbfil > 1)
    {
        t.resize(info.nbfil);
    }

    std::cerr << "\n[INF] Création des fils clients.\n";

    for (int i = 0; i < info.nbfil; ++i)
    {
        Info[i] = info;

        Info[i].threads = new thread_t;
        Info[i].threads->thread_num = i;
        Info[i].threads->argc = (argc - start < nbfichier_par_fil)? argc - start: nbfichier_par_fil;
        Info[i].threads->argv = new char*[nbfichier_par_fil];


        if (Info[i].threads->argv == nullptr)
        {
            perror("Erreur : Allocation de threads");
            exit(-145);
        }

        int shift = 0;
        for (int j = start; j <  nbfichier_par_fil + start && j + shift < argc; ++j)
        {
            if (! strcmp(argv[j + shift], "-g"))
            {
                Info[i].threads->argc--;
                ++shift;
            }
            Info[i].threads->argv[j - start] = argv[j + shift];
        }

        std::cerr << "\n\nThread i=" << i+1 << "/" << info.nbfil
                  << "\n\nNombre de fichiers : " << Info[i].threads->argc << "\n";

        start += nbfichier_par_fil;

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
            std::cerr << "\n" << strerror(errno) << "\n";
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
            std::cerr <<  "\n[MSG] Maximum de lignes : " << maxima[1] << "\n"
                       << "\n[MSG] Maximum d'agents  : " << maxima[0] << "\n";
        }
    }

    if (maxima == nullptr) maxima = calculer_maxima(Info);

#if 0
    std::ofstream LOG;
    LOG.open(Info[0].chemin_log, std::ios::app);
    if (LOG.good() && maxima)
    {
        LOG << "\n[MSG] Maximum de lignes : " << maxima[1] << "\n";
        LOG << "\n[MSG] Maximum d'agent   : " << maxima[0] << "\n";
        LOG.close();
    }
#endif

    xmlCleanupParser();

    auto endofcalculus = Clock::now();

    std::cerr << "\n[INF] Durée de calcul : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endofcalculus - startofprogram).count()
              << " millisecondes" << "\n";


    if (generer_table)
    {
        boucle_ecriture(Info);
    }

    /* libération de la mémoire */

    int valeur_de_retour = 0;
    if (! liberer_memoire) goto duration;

    std::cerr << "\n[INF] Libération de la mémoire...\n";

    for (int i = 0; i < Info[0].nbfil; ++i)
    {
        for (unsigned agent = 0; agent < Info[i].NCumAgent; ++agent)
        {
            int utilisation_memoire =  Info[i].minimum_memoire_p_ligne + nbType + Info[i].NLigne[agent]*(INDEX_MAX_CONNNES + 1);

            for (int j = 0; j < utilisation_memoire; ++j)
                if (Info[i].Table[agent][j])
                    if (Info[i].Table[agent][j] != nullptr)
                        xmlFree(Info[i].Table[agent][j]);

            delete [] (Info[i].Table[agent]);
        }

        delete [] (Info[i].NAgent);
        delete [] (Info[i].threads->argv);

        delete [] (Info[i].Table);

        if (Info[0].nbfil > 1)
        {
            delete [] (Info[i].threads);
        }
    }

    valeur_de_retour = (maxima)? 2 * maxima[0] + 3 * maxima[1]: 0;

    if (maxima) delete [] (maxima);

    duration:

    auto endofprogram = Clock::now();

    std::cerr << "\n[INF] Durée d'exécution : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(endofprogram - startofprogram).count()
              << " millisecondes" << "\n";

    if (rankFile.is_open()) rankFile.close();

    return valeur_de_retour;
}

