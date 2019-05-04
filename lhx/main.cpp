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
#include <experimental/filesystem>
#include <regex>
#include "validator.h"
#include "fonctions_auxiliaires.h"
#include "table.h"
#include "recherche.h"

#ifndef OVERHEAD
#define OVERHEAD 500
#endif

#ifndef AVERAGE_RAM_DENSITY
#define AVERAGE_RAM_DENSITY 1.25
#endif

/// \file    lhx/main.cpp
/// \author  Fabrice Nicol
/// \brief   Ce fichier contient le code relatif à l'analyse de la ligne de commande, au découpage de la liste des fichiers XML
/// en sous-listes (ou segments) déterminées en fonction des capacités de mémoire vive de la plateforme, qui seront analysés successivement,
/// et au lancement des fils d'exécution permettant de traiter des sous-ensembles de ces segments en parallèle. Il contient aussi la fonction
/// permetant de lancer l'extraction des bulletins de paye après décodage complet des bases XML.

using namespace std;

//#ifdef HAS_CPP17
namespace fs = experimental::filesystem;

//#endif

using vString = vector<string>;

typedef chrono::high_resolution_clock Clock;

static bool generer_table = false;
static bool liberer_memoire = true;
bool verbeux = true;

ofstream rankFile;
string rankFilePath = "";
mutex mut;
vector<errorLine_t> errorLineStack;
static vString commandline_tab;
static vector<vString> vbull;
static string repertoire_bulletins;

pair<uint64_t, uint64_t>  produire_segment (info_t& info, const vString& segment);


int main (int argc, char **argv)
{
    auto startofprogram = Clock::now();

#if defined _WIN32 | defined _WIN64
    setlocale (LC_ALL, "French_France.1252"); // Windows ne gère pas UTF-8 en locale
    //locale::global(locale("French_France.1252"));
#elif defined __linux__
    // setlocale(LC_ALL, "fr_FR.ISO-8859-15");
    locale::global (locale ("fr_FR.utf8"));
#else
#error "Programme conçu pour Windows ou linux"
#endif

    if (argc < 2)
        {
            cerr << ERROR_HTML_TAG "Il faut au moins un fichier à analyser.\n" ;
            return -2;
        }

    // Initialisation de libxml2

    LIBXML_TEST_VERSION
    xmlKeepBlanksDefault (0);

    xmlInitMemory();
    xmlInitParser();

    int start = 1;
    string type_table = "bulletins";
    vString cl;  /* pour les lignes de commandes incluses dans un fichier */
    string chemin_base = NOM_BASE + string (CSV);
    string chemin_bulletins = NOM_BASE_BULLETINS + string (CSV);
    unsigned long long memoire_xhl = 0, memoire_disponible = 0;
    int nsegments = 0;
    float ajustement = MAX_MEMORY_SHARE;
    bool exporter_bulletins_par_agent = false;

    thread_t mon_thread;

    // Inititalisation de la structure de partage des données décodées des fichiers XML

    info_t info =
    {
        {{}},             //    bulletinPtr* Table;
        0,                //    uint64_t nbLigne;
        vector<array<uint64_t, 3>>(),                //    ligne_debut
                                vector<array<uint64_t, 2>>(),                //    ligne_fin
        {},               //    int32_t  *NAgent;
        0,                //    uint32_t nbAgentUtilisateur
        0,                //    uint32_t NCumAgent;
        0,                //    uint32_t NCumAgentXml;
        0, // taille base
        BaseType::MONOLITHIQUE,         //    type base
        vector<uint16_t>(),             //    vector<uint16_t> NLigne;
        &mon_thread,      //    thread_t threads;
        "",               //    chemin log
#ifdef INCLURE_REG_ELUS
        EXPRESSION_REG_ELUS,
#endif    
        chemin_base,
        chemin_bulletins,
        "Standard",       // export_mode
        vString(),        // pas d'extraction de bulletins
        vString(),        // n'exclure aucun SIRET
        vString(),        // n'exclure aucun budget
        vString(),        // n'exclure aucun employeur
        MAX_LIGNES_PAYE,  // nbLigneUtilisateur
        0,                // uint16_t fichier_courant
        ',',              // const char decimal
        ';',              // const char separateur
        true,             // réduire consommation mémoire
        true,             // par défaut lire la balise adjacente
        false,            // calculer les maxima de lignes et d'agents
        false,            // numéroter les lignes
        false,            // ne pas générer des bulletins particuliers pour impression
        false,            // ne pas exporter les informations sur l'établissement
        false,            // ne pas exporter l'échelon
        false,            // ne pas exporeter la répartition budgétaire
        false,            // faire semblant d'extraire
        false,            // test mémoire
        false,            // extraction depuis disque optique
        1                 // nombre de fils
    };

    // Analyse de la ligne de commande

    commandline_tab.assign (argv, argv + argc);

    while (start < argc)
        {

            // Nombre maximal de bulletins mensuels attendus pour une allocation manuelle de la mémoire

            if (commandline_tab[start] == "-n")
                {
                    info.reduire_consommation_memoire = false;
                    info.nbAgentUtilisateur
                        = lire_argument (argc, const_cast<char*> (commandline_tab[start + 1].c_str()));

                    if (info.nbAgentUtilisateur < 1)
                        {
                            cerr << ERROR_HTML_TAG "Préciser le nombre de bulletins mensuels attendus (majorant du nombre) avec -n xxx .\n";
                            exit (-1);
                        }

                    start += 2;
                    continue;
                }

            // Aide en ligne

            else if (commandline_tab[start] ==  "-h" || commandline_tab[start] ==  "--help")
                {
                    string out = help().str();
                    out.erase (remove (out.begin(), out.end(), '*'), out.end());
                    cerr << out;
                    exit (0);
                }

            else if (commandline_tab[start] == "-v" || commandline_tab[start] == "--version")
                {
                    cerr << "Altaïr, module LHX version " << VERSION << "\n"
                         << "  Copyright Cour des comptes 2013-2018.\n"
                         << "  Auteur Fabrice Nicol <fabrice.nicol@crtc.ccomptes.fr> 2013-2018.n"
                         << "  Cette application est placée sous licence CeCILL version 2.1.\n  Voir fichier LICENCE ou  LICENCE.html.\n"
                         << "  L'application en ligne de commande LHX est un module du logiciel Altaïr.\n"
                         << "  Elle extrait les données de paye des fichiers au format XHL/XML spécifié\n  par la convention-cadre nationale de dématérialisation.\n\n";
                    exit(0);
                }

            // Génération des bulletins de paye

            else if (commandline_tab[start] ==  "--bulletins")
                {
                    ++start;

                    if (start == argc) break;

                    if (none_of (commandline_tab.begin(),
                                 commandline_tab.end(),
                                 [] (const string & x)
                    {
                        return (x == string ("--dossier-bulletins"));
                        }))
                    {
                        cerr << ERROR_HTML_TAG "Erreur de ligne de commande fatale : "
                             "Il faut une option --dossier-bulletins spécifiant en argument"
                             "le dossier vers lequel les bulletins extraits seront exportés."
                             ENDL;
                        throw;
                    }
                    const string req = commandline_tab[start];
                    vString bull = split (req, ';');
                    generer_table = false;
                    info.generer_bulletins = true;

                    for (const string &v : bull)
                        vbull.emplace_back (split (v, '-')); // vbull est un vecteur de triplets du type {"1025N", "6...9", "2012...2015"}

                    ++start;
                    continue;
                }

            // Paramétrage du dossier de génération des bulletins de paye

            else if (commandline_tab[start] ==  "--dossier-bulletins")
                {
                    ++start;

                    if (start == argc) break;

                    if (none_of (commandline_tab.begin(),
                                 commandline_tab.end(),
                                 [] (const string & x)
                    {
                        return (x == string ("--bulletins"));
                        }))
                    {
                        cerr << ERROR_HTML_TAG "Erreur de ligne de commande fatale : "
                             "Il faut une option --bulletins avec les bulletins à imprimer"
                             "en argument, sur le format matricule-mois...mois-année...année"
                             ENDL;
                        throw;
                    }

                    repertoire_bulletins = commandline_tab[start];

                    // Nettoyage (sinon problèmes d'empilement de données cumulées en CSV)

                    if (fs::exists(repertoire_bulletins))
                    {
                        if (verbeux) cerr << PROCESSING_HTML_TAG << "Nettoyage du dossier " << repertoire_bulletins << ENDL;
                        fs::remove_all(repertoire_bulletins);
                        if (fs::exists(repertoire_bulletins) && verbeux)
                            cerr << WARNING_HTML_TAG << "Le dossier n'a pas pu être nettoyé. Attention aux empilements de données de différents contrôles !" << ENDL;
                    }
                    else
                    {
                        if (verbeux)
                            cerr << PROCESSING_HTML_TAG << "Création du dossier " << repertoire_bulletins << ENDL;
                    }

                    fs::create_directories(repertoire_bulletins);

                    ++start;
                    continue;
                }

            // Génération de la documentation de l'aide en ligne

            else if (commandline_tab[start] ==  "--hmarkdown" || commandline_tab[start] == "--pdf" || commandline_tab[start] == "--html")
                {
                    ostringstream out = help();

                    if (commandline_tab[start] == "--hmarkdown")
                        {
                            cerr << out.str();
                        }
                    else
                        {
                            ofstream help;

                            help.open ("aide.md");
                            
                            help << regex_replace(out.str(), regex("--"), "\\-\\-");
                       
#ifndef __linux__
                            string sep (";");
                            sep[0] = SYSTEM_PATH_SEPARATOR;
                            string exec_dir = getexecpath();
                            string command = string ("PATH=") + string (getenv ("PATH"))
                                             + sep + exec_dir + string ("/../texlive/miktex/bin")
                                             + sep + exec_dir + string ("/../Outils")
                                             + sep + exec_dir + string ("/../RStudio/bin/pandoc");
                            putenv (command.c_str());
#             define exec  ".exe"
#else
#             define exec   ""
#endif

#                           if not (defined(__linux__ )  || defined(__unix__))
                            int res = system ("iconv"  exec  " -t utf-8 -f latin1 -c -s  aide.md > aide.utf8.md");

                            if (res) cerr << "La conversion en UTF-8 par iconv a échoué" ENDL;

#                           endif

                            if (commandline_tab[start] == "--pdf")
                                {
                                    int res = system ("pandoc" exec  " -o aide_lhx.pdf  aide.md");

                                    if (res) cerr << "La conversion en PDF par pandoc a échoué" ENDL;
                                }
                            else if (commandline_tab[start] == "--html")
                                {
                                    int res = 0;
#                                   if not (defined(__linux__ )  || defined(__unix__))
                                    res = system ("pandoc"  exec  " -o aide_lhx.utf8.html  aide.utf8.md");
                                    unlink ("aide_lhx.utf8.html");
                                    res = system ("iconv"  exec  " -f utf-8 -t latin1 -c -s  aide_lhx.utf8.html > aide_lhx.html");
#                                   else
                                    res = system ("pandoc"  exec  " -o aide_lhx.html  aide.md");
                                    res = system ("echo" exec " \"<html>\n<meta charset=\"UTF-8\">\n<body>\n\" > aide_lhx.temp"
                                                  " && cat aide_lhx.html >> aide_lhx.temp"
                                                  " && echo" exec " \"\n</body>\n</html>\" >> aide_lhx.temp"
                                                  " && mv aide_lhx.temp aide_lhx.html");
#                                   endif

                                    if (res) cerr << "La conversion en HTML par pandoc a échoué" ENDL;
                                }

                            help.close();
#                           if not (defined(__linux__ )  || defined(__unix__))
                            unlink ("aide.utf8.md");
#                           endif
                            unlink ("aide.md");
                        }

                    exit (0);
                }

            // Mode à verbosité réduite

            else if (commandline_tab[start] ==  "-q")
                {
                    verbeux = false;
                    ++start;
                    continue;
                }

            // Type de la table en sortie (standard, par année, par catégorie de ligne de paye,....)

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

            // Génération des rangs de ligne de paye dans la table en sortie

            else if (commandline_tab[start] ==  "-l")
                {
                    info.generer_rang = true ;
                    ++start;
                    continue;
                }

            // Type de la table des données de paye en sortie

            else if (commandline_tab[start] == "-T")
                {
                   if (start + 1 == argc)
                        {
                            cerr << ERROR_HTML_TAG "Option -T suivi d'un argument obligatoire (nombre de lignes)." ENDL ;
                            exit (-100);
                        }

                    // Désactiver si --AG est déjà rencontré en ligne de commande (aucun bulletin de paye individuel n'atteindra des plafonds raisonnables en argument de -T)
                    if (info.type_base == BaseType::PAR_AGENT)
                    {
                        start += 2;
                        continue;
                    }

                    map<string, BaseType> hashTable;

                    hashTable["AN"] = BaseType::PAR_ANNEE;
                    hashTable["A"]  = BaseType::PAR_REM_DIVERSES;
                    hashTable["AC"] = BaseType::PAR_ACOMPTE;
                    hashTable["AG"] = BaseType::PAR_AGENT;
                    hashTable["AV"] = BaseType::PAR_AVANTAGE_NATURE;
                    hashTable["C"]  = BaseType::PAR_COTISATION;
                    hashTable["D"]  = BaseType::PAR_DEDUCTION;
                    hashTable["I"]  = BaseType::PAR_INDEMNITE;
                    hashTable["IR"] = BaseType::PAR_INDEMNITE_RESIDENCE;
                    hashTable["S"]  = BaseType::PAR_SFT;
                    hashTable["T"]  = BaseType::PAR_TRAITEMENT;
                    hashTable["R"]  = BaseType::PAR_RAPPEL;
                    hashTable["RE"] = BaseType::PAR_RETENUE;
                    hashTable["X"]  = BaseType::TOUTES_CATEGORIES;

                    if (hashTable.find (commandline_tab[start + 1]) != hashTable.end())
                        {
                            BaseType type = hashTable[commandline_tab[start + 1]];
                            info.type_base = (info.type_base == BaseType::MAXIMUM_LIGNES && type == BaseType::PAR_ANNEE) ? BaseType::MAXIMUM_LIGNES_PAR_ANNEE : type;

                        }
                    else
                        {
                            // On a trouvé un argument numérique en ligne de commande

                            int32_t size_read = lire_argument (argc, const_cast<char*> (commandline_tab[start + 1].c_str()));

                            if (size_read < 0 || size_read > INT32_MAX - 1)
                                {
                                    cerr << ERROR_HTML_TAG "Le nombre de lignes doit être compris entre 0 et INT64_MAX" ENDL;
                                    throw;
                                }
                            else
                                {
                                    info.taille_base = size_read;

                                    info.type_base = (info.type_base == BaseType::PAR_ANNEE) ? BaseType::MAXIMUM_LIGNES_PAR_ANNEE : BaseType::MAXIMUM_LIGNES;

                                    cerr << PARAMETER_HTML_TAG "Bases d'au plus " << size_read << " lignes" ENDL;
                                }
                        }

                    start += 2;
                    continue;
                }
            else if (commandline_tab[start] == "--AG")
                {
                   exporter_bulletins_par_agent = true;
                   cerr << PARAMETER_HTML_TAG "Exportation des lignes de paye : un fichier par agent, mois et année." << ENDL;

                   ++start;
                   continue;
                }
            // Séparateur-délimiteur de champs du format CSV (en général ';' ou ',')

            else if (commandline_tab[start] == "-s")
                {
                    if (start + 1 == argc)
                        {
                            cerr << ERROR_HTML_TAG "Option -s suivi d'un argument obligatoire (séparateur de champs)." ENDL;
                            exit (-100);
                        }

                    info.separateur = commandline_tab[start + 1][0];

                    if (info.separateur == '_')
                        {
                            perror (ERROR_HTML_TAG "Le séparateur ne doit pas être '_'");
                            exit (-909);
                        }

                    start += 2;
                    continue;
                }

            // Séparateur des décimales

            else if (commandline_tab[start] == "-d")
                {
                    if (start + 1 == argc)
                        {
                            cerr << ERROR_HTML_TAG "Option -d suivi d'un argument obligatoire (séparateur décimal)." ENDL;
                            exit (-100);
                        }

                    info.decimal = commandline_tab[start + 1][0];
                    start += 2;
                    continue;
                }

            // Chemin complet de sortie de la base Table

            else if (commandline_tab[start] == "-o")
                {
                    if (start + 1 == argc)
                        {
                            cerr << ERROR_HTML_TAG "Option -o suivi d'un argument obligatoire (nom de  fichier)." ENDL;
                            exit (-100);
                        }

                    info.chemin_base = commandline_tab[start + 1];

                    ofstream base;
                    base.open (info.chemin_base);

                    if (! base.good())
                        {
                            perror (ERROR_HTML_TAG "La base de données ne peut être créée, vérifier l'existence du dossier.");
                            exit (-113);
                        }

                    base.close();
                    unlink (info.chemin_base.c_str());
                    start += 2;
                    continue;
                }

            // Désactivation de la libération de la mémoire en sortie (utile pour déboguer)

            else if (commandline_tab[start] == "-M")
                {
                    liberer_memoire = false;
                    ++start;
                    continue;
                }

            // Calcul des maxima de nombre de bulletins de paye et d'agents par mois

            else if (commandline_tab[start] == "-m")
                {
                    info.calculer_maxima = true;
                    ++start;
                    continue;
                }

            // Chemin complet de sortie du répertoire des données CSV

            else if (commandline_tab[start] == "-D")
                {
                    string path = commandline_tab[start + 1];

                    if (! fs::exists(path))
                        fs::create_directories(path);

                    info.chemin_base = path + string ("/") + string (NOM_BASE) ;
                    info.chemin_bulletins = path + string ("/") + string (NOM_BASE_BULLETINS);
                    ofstream base;

                    base.open (info.chemin_base, ofstream::out | ofstream::trunc);

                    if (! base.good())
                        {
                            cerr << ERROR_HTML_TAG "La base de données "
                                 << info.chemin_base  + string (CSV) << " ne peut être créée, vérifier les droits d'accès au dossier spécifié par -D." ENDL ;
                            throw;
                        }
                    else
                        {
                            // Necessaire sous Windows, no-op sous *.nix
                            base.close();
                            unlink (info.chemin_base.c_str());
                        }

                    start += 2;
                    continue;
                }

            // Nombre de fils d'exécution

            else if (commandline_tab[start] == "-j")
                {
                    if ((info.nbfil = lire_argument (argc, const_cast<char*> (commandline_tab[start + 1].c_str()))) > 0)
                        {
                            if (info.nbfil < 1)
                                {
                                    perror (ERROR_HTML_TAG "Le nombre de fils d'exécution doit être compris au moins égal à 2.");
                                    exit (-111);
                                }
                        }

                    start += 2;
                    continue;
                }

            // Chemin du log

            else if (commandline_tab[start] == "-L")
                {
                    if (argc > start + 2) info.chemin_log = commandline_tab[start + 1];

                    ofstream base;
                    base.open (info.chemin_log);

                    if (! base.good())
                        {
                            perror (ERROR_HTML_TAG "Le log ne peut être créé, vérifier l'existence du dossier.");
                            exit (-114);
                        }

                    start += 2;
                    continue;
                }

            // Nombre maximal de ligne de payes par mois (allocation de mémoire manuelle)

            else if (commandline_tab[start] == "-N")
                {
                    if ((info.nbLigneUtilisateur = lire_argument (argc, const_cast<char*> (commandline_tab[start + 1].c_str()))) > 1)
                        {
                            cerr << STATE_HTML_TAG "Nombre maximum de lignes de paye redéfini à : " << info.nbLigneUtilisateur << ENDL;
                        }
                    else exit (-1);

                    info.reduire_consommation_memoire = false;

                    start += 2;
                    continue;
                }

            // Chemin du fichier contenant le rang d'extraction pour permettre l'affichage d'une barre de progression dans l'interface graphique.
            // Par défaut USERPROFILE/LOCALDATA

#ifdef GENERATE_RANK_SIGNAL
            else if (commandline_tab[start] == "-rank")
                {
                    int hasArg = 0;

                    if (argc > start + 2)
                        {
                            if (commandline_tab[start + 1][0] == '-')
                                rankFilePath = string (getenv (USERPROFILE)) + string ("/") + string (LOCALDATA);

                            else
                                {
                                    rankFilePath = commandline_tab[start + 1];
                                    hasArg = 1;
                                }

                            ifstream testFile;
                            testFile.open (rankFilePath);

                            if (testFile.is_open())
                                {
                                    testFile.close();
                                }

                            rankFile.open (rankFilePath, ios::out | ios::trunc);
                            rankFile << 1 << "\n";
                            rankFile.close();
                        }

                    start += 1 + hasArg;
                    continue;
                }

#endif

            // Générer les Siret

            else if (commandline_tab[start] == "-S")
                {
                    if (argc > start + 2)
                        {
                            info.select_siret = true;
                        }
                    else
                        {
                            exit (-116);
                        }

                    ++start;
                    continue;
                }

            // Générer les échelons

            else if (commandline_tab[start] == "-E")
                {
                    if (argc > start + 1)
                        {
                            info.select_echelon = true;
                        }
                    else
                        {
                            cout << "Il faut au moins un fichier";
                            exit (-116);
                        }

                    ++start;
                    continue;
                }

            // Exclure le siret en argument

            else if (commandline_tab[start] == "--esiret")
                {
                    if (argc > start + 2)
                        {
                            if (commandline_tab[start + 1][0] == '-') throw ("Il manque les SIRET à exclure");

                            do
                                {
                                    info.exclure_siret.push_back (commandline_tab[start + 1]);
                                    ++start;
                                }
                            while (argc > start + 1 && commandline_tab[start][0] != '-');

                        }
                    else
                        {
                            cerr << ERROR_HTML_TAG "Il manque les SIRET à exclure.";
                            throw ("");
                        }

                    continue;
                }

            // Exclure le budget en argument

            else if (commandline_tab[start] == "--ebudget")
                {
                    if (argc > start + 2)
                        {
                            if (commandline_tab[start + 1][0] == '-') throw ("Il manque les budgets à exclure");

                            do
                                {
                                    info.exclure_budget.push_back (commandline_tab[start + 1]);
                                    ++start;
                                }
                            while (argc > start + 1 && commandline_tab[start][0] != '-');

                        }
                    else
                        {
                            cerr << ERROR_HTML_TAG "Il manque les budgets à exclure.";
                            throw ("");
                        }

                    continue;
                }

            // Exclure le budget en argument

            else if (commandline_tab[start] == "--eemployeur")
                {
                    if (argc > start + 2)
                        {
                            if (commandline_tab[start + 1][0] == '-') throw ("Il manque les employeurs à exclure");

                            do
                                {
                                    info.exclure_employeur.push_back (commandline_tab[start + 1]);
                                    ++start;
                                }
                            while (argc > start + 1 && commandline_tab[start][0] != '-');

                        }
                    else
                        {
                            cerr << ERROR_HTML_TAG "Il manque les employeurs à exclure.";
                            throw ("");
                        }

                    continue;
                }

            // Chemin du fichier permettant de lire une ligne de commandes générée par l'interface graphique

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
                            exit (-119);
                        }

                    ifstream f;
                    f.open (fichier);

                    if (! f.good())
                        {
                            cerr << ERROR_HTML_TAG "Impossible d'ouvrir le fichier " << fichier << ".\n";
                            exit (-119);
                        }

                    string ligne;

                    if (f.good())
                        {
                            while (f.rdstate() != fstream::eofbit)
                                {
                                    getline (f, ligne);

                                    if (! ligne.empty())
                                        cl.push_back (ligne);
                                }

                        }

                    f.close();

                    if (! cl.empty())
                        {
                            argc = cl.size() + 1;

                            commandline_tab.resize (argc);

                            for (int i = 0; i < argc - 1; ++i)
                                {

                                    commandline_tab[i + 1] = cl.at (i);

                                    if (string (cl.at (i)) == "-f")
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

            // Ne pas décoder XML

            else if (commandline_tab[start] == "--pretend")
                {
                    info.pretend = true;   // pas de décodage
                    ++start;
                    continue;
                }

            // Seulement vérifier la mémoire

            else if (commandline_tab[start] == "--verifmem")
                {
                    info.verifmem = true;  // seulement vérifier la mémoire
                    ++start;
                    continue;
                }

            // Taille totale de fichiers

            else if (commandline_tab[start] == "--xhlmem")
                {
                    cerr << STATE_HTML_TAG "Taille totale des fichiers : " << commandline_tab[start + 1] << " octets." << ENDL;
                    // Taille des fichiers en ko fournie par l'interface graphique, en octets

                    memoire_xhl = stoull (commandline_tab[start + 1], nullptr);

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
                                    exit (-199);
                                }
                        }
                    else
                        {
                            cerr << ERROR_HTML_TAG "Le plus grand entier non signé est inférieur à 60 * 1048576" << ENDL;
                            exit (-199);
                        }
                }

            // Part de la mémoire vive utilisée

            else if (commandline_tab[start] == "--memshare")
                {
                    int part = stoi (commandline_tab[start + 1], nullptr);

                    cerr << ENDL << PARAMETER_HTML_TAG "Part de la mémoire vive utilisée : " <<  part << " %" ENDL;

                    ajustement = (float) part / 100;
                    start += 2;
                    continue;
                }

            // Nombre de segments pour répartir les fichiers XHL de manière équilibrée entre ces sgments, qui chacun prendront un fil d'exécution successivement

            else if (commandline_tab[start] == "--segments")
                {
                    cerr << PARAMETER_HTML_TAG "Les bases seront analysées en au moins : " << commandline_tab[start + 1] << " segments" << ENDL;

                    // au maximum 99 segments

                    nsegments = stoi (commandline_tab[start + 1], nullptr);

                    if (nsegments > 1 && nsegments < 100)
                        {
                            start += 2;
                            continue;
                        }
                    else
                        {
                            cerr << ERROR_HTML_TAG "Il doit y avoir entre 1 et 99 segments de bases." << ENDL;
                            exit (-208);
                        }
                }

            // Modalité d'exportation (standard, cumulative, distributive,...)

            else if (commandline_tab[start] == "--export")
                {
                    string st = commandline_tab[start + 1];
                    cerr << STATE_HTML_TAG "Modalité d'exportation : " << st << ENDL;

                    if (  st != "'Standard'"
                            && st != "'Cumulative'"
                            && st != "'Distributive'")
                        {
                            cerr << ERROR_HTML_TAG "Modalité d'exportation inconnue : " << st << ENDL;
                            throw;
                        }

                    info.export_mode = st;
                    start += 2;
                    continue;
                }

            // Importation directe d'un disque optique

            else if  (commandline_tab[start] == "--cdrom")
                {
                    info.cdrom = true;
                    ++start;
                    cerr << PARAMETER_HTML_TAG "Modalité disque optique" << ENDL;
                    continue;
                }

            // Exportation de la répartition budgétaire (FPH)

            else if  (commandline_tab[start] == "--repartition-budget")
                {
                    info.generer_repartition_budget = true;
                    ++start;
                    cerr << PARAMETER_HTML_TAG "Exporter la répartition budgétaire" << ENDL;
                    continue;
                }

            // Option inconnue

            else if (commandline_tab[start][0] == '-')
                {
                    cerr << ERROR_HTML_TAG "Option inconnue " << commandline_tab[start] << ENDL;
                    exit (-100);
                }
            else break;
        }

    // Fin de l'analyse de la ligne de commande
    // on sait que info.nbfil >= 1

    vector<unsigned long long> taille;

    // Soit la taille totale des fichiers est transmise par --memshare soit on la calcule ici,
    // en utilisant taille_fichiers

    if (memoire_xhl == 0)
        {
            off_t mem = 0;
            int count = 0;

            for (int i = start; i < argc; ++i)
                {
                    if ((mem = taille_fichier (commandline_tab.at (i))) != -1)
                        {
                            memoire_xhl += static_cast<unsigned long long> (mem);
                            taille.push_back (static_cast<unsigned long long> (mem));
                            ++count;
                        }
                    else
                        {
                            cerr << ERROR_HTML_TAG "La taille du fichier " << commandline_tab.at (i) << " n'a pas pu être déterminée." ENDL;
                            taille.push_back (0);
                            ++count;
                        }
                }

            {
                LOCK_GUARD
                cerr << ENDL STATE_HTML_TAG << "Taille totale des " << count << " fichiers : " << memoire_xhl / 1048576 << " Mo."  ENDL;
                cerr << STATE_HTML_TAG
                     << "Mémoire disponible " <<  ((memoire_disponible = getFreeSystemMemory()) / 1024)
                     << " / " << (getTotalSystemMemory()  / 1048576)
                     << " Mo."  ENDL;

            }
        }

    // ajustement représente la part maximum de la mémoire disponible que l'on consacre au processus, compte tenu de la marge sous plafond (overhead)

    unsigned long long memoire_utilisable = floor (ajustement * static_cast<float> (memoire_disponible));
    cerr << STATE_HTML_TAG  << "Mémoire utilisable " <<  memoire_utilisable / 1024 << " Mo."  ENDL;

    memoire_utilisable *= 1024;

    if (nsegments != 0)
        {
            while (true)
                {
                    if (memoire_xhl % nsegments == 0)
                        {
                            memoire_utilisable = memoire_xhl / nsegments ;
                            break;
                        }
                    else if (memoire_xhl % (nsegments - 1) == 0)
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

    // découpage de la ligne de commande en segments, la taille de chaque segment en mémoire devant être inférieure au plafond fix" par memoire_utilisable
    do
        {
            unsigned long long taille_segment = *taille_it;
            vString segment;
            float densite_segment = AVERAGE_RAM_DENSITY;
#ifdef STRINGSTREAM_PARSING
            ++densite_segment;
#endif
#ifndef OFSTREAM_TABLE_OUTPUT
            ++densite_segment;
#endif

            do
                {
                    taille_segment  += *taille_it;
                    segment.push_back (*commandline_it); // ne pas utiliser move;
                    ++commandline_it;
                    ++taille_it;

                }
            while (taille_it != taille.end()
                    && commandline_it != commandline_tab.end()
                    && taille_segment * densite_segment < memoire_utilisable);

            segments.emplace_back (segment);

        }
    while (commandline_it != commandline_tab.end()
            && taille_it != taille.end());

    unsigned int segments_size = segments.size();

    if (segments_size > 1)
        cerr << PROCESSING_HTML_TAG << "Les bases en sortie seront analysées en " << segments_size << " segments." ENDL;

    int info_nbfil_defaut = info.nbfil;

    pair<uint64_t, uint64_t>lignes = make_pair (0, 0);

    for (auto &&segment : segments)
      {
            unsigned int segment_size = segment.size();

            // S'il y a plus de fils que de fichiers dans le segment il faut corriger le nombre de fils en conséquence (nb de fils = de fichiers)

            if (info.nbfil > segment_size)
                {
                    if (verbeux)
                        cerr << WARNING_HTML_TAG "Réduction du nombre de fils (" << info.nbfil << ") au nombre de fichiers (" << segment_size << ")" ENDL;

                    info.nbfil = segment_size;
                }
            else
                info.nbfil = info_nbfil_defaut;

            // Lancement de la fonction principale

            lignes = produire_segment (info, segment);
        }

    // Extraction des bulletins : on relance lhx mais sur une ligne de commande qui pour sa partie fichiers est remplacée par les bulletins individuels xml extraits
    // Il suffit de substituer le répertoire --dossier-bulletins au répertoire de sortie (-D) habituel
    // Le segment est à présent la collection de chemins de bulletins réalisée plus haut. Un seul segment va suffire, dans la pratique.

    if (info.generer_bulletins && ! info.chemins_bulletins_extraits.empty())
    {

        // réajustement du répertoire de sortie

        string path = repertoire_bulletins + string ("/Bases/");
        fs::create_directories(path);

        // On est maintenant en extraction standard des données CSV, la "génération des bulletins" est déjà faite :

        info.generer_bulletins = false;
        generer_table = true;

        // On exporte éventuellement par agent en fonction ou non de la présence de --AG :

        info.type_base = exporter_bulletins_par_agent ? BaseType::PAR_AGENT : BaseType::MONOLITHIQUE;

        info.chemin_base = path + string (NOM_BASE) ;
        info.chemin_bulletins = path + string (NOM_BASE_BULLETINS);

        unsigned int segment_size = info.chemins_bulletins_extraits.size();

        // S'il y a plus de fils que de fichiers dans le segment il faut corriger le nombre de fils en conséquence (nb de fils = de fichiers)

        if (info.nbfil > segment_size)
            {
                if (verbeux)
                    cerr << WARNING_HTML_TAG "Réduction du nombre de fils (" << info.nbfil << ") au nombre de fichiers (" << segment_size << ")" ENDL;

                info.nbfil = segment_size;
            }
        else
            info.nbfil = info_nbfil_defaut;

        // Lancement de la fonction principale

        lignes = produire_segment(info, info.chemins_bulletins_extraits);
    }

    // Nettoyage du parseur XML
    
#   ifndef TINYXML2
      xmlCleanupParser();
#   endif    

    // Calcul de la durée d'exécution

    auto endofprogram = Clock::now();
    auto duree = chrono::duration_cast<chrono::milliseconds> (endofprogram - startofprogram).count();

    cerr << ENDL << PROCESSING_HTML_TAG "Durée d'exécution : "
         << duree
         << " millisecondes" << ENDL;

    if (generer_table)
        cerr << ENDL << PROCESSING_HTML_TAG "Vitesse d'exécution : "
             << lignes.first / duree
             << " milliers de lignes par seconde" << ENDL;

    if (rankFile.is_open())
        rankFile.close();

    return errno;
}

/// Pour un segment donné (sous-ensemble de bases de paye XML), redécoupe le segment en partie égales ou presque
/// et lance un fil d'exécution pour chacune de ces sous-parties.
/// \param info  Référence vers un structure de type info_t contenant les données d'input (noms de fichiers, paramètres de lignes de commande)
/// \param segment Référence vers un vecteur de chaines de caractères de type string contenant les chemins de bases de paye XML en input
/// \return Nombre de lignes du fichier bulletins de paye et du fichier lignes de paye (Table)

pair<uint64_t, uint64_t> produire_segment (info_t& info, const vString& segment)
{
    static int nsegment;

    ++nsegment;

    vector<int> nb_fichier_par_fil;
    int segment_size = segment.size();

    int remainder = segment_size % info.nbfil;

    for (unsigned i = 0; i < info.nbfil; ++i)
        {
            nb_fichier_par_fil.push_back (segment_size / info.nbfil + (remainder > 0));
            --remainder;
        }

    // on répartir le reste de manière équilibrée sur les premiers fils

    vector<info_t> Info (info.nbfil);
    vector<thread> t;

    if (info.nbfil > 1)
        {
            t.resize (info.nbfil);
        }

    if (verbeux)
        {
            cerr << PROCESSING_HTML_TAG "Création de " << info.nbfil << pluriel (info.nbfil, "fil") << "d'exécution." ENDL;
        }

    vector<thread_t> v_thread_t (info.nbfil);
    vString::const_iterator segment_it = segment.begin();

#ifdef GENERATE_RANK_SIGNAL

    reset_rank_signal();
    generate_rank_signal();

#endif

    for (unsigned  i = 0; i < info.nbfil; ++i)
        {
            Info[i] = info;
            Info[i].threads = &v_thread_t[i];
            Info[i].threads->thread_num = i;
            Info[i].threads->argc = nb_fichier_par_fil.at (i);
            Info[i].threads->argv = vString (segment_it, segment_it + nb_fichier_par_fil[i]);
            Info[i].threads->in_memory_file = vString (nb_fichier_par_fil[i]);
            segment_it += nb_fichier_par_fil.at (i);

            if (Info[i].threads->argv.size() != (unsigned) nb_fichier_par_fil.at (i))
                {
                    cerr << ERROR_HTML_TAG "Problème issu de l'allocation des threads" << ENDL ;
                    throw;
                }

            if (verbeux)
                cerr <<  PROCESSING_HTML_TAG "Fil d'exécution i = " << i + 1 << "/" << info.nbfil
                     << "   Nombre de fichiers dans ce fil : " << nb_fichier_par_fil[i] << ENDL;

            errno = 0;

            // Lancement des fils d'exécution

            if (verbeux && Info[0].reduire_consommation_memoire)
                cerr << ENDL PROCESSING_HTML_TAG
                     "Premier scan des fichiers pour déterminer les besoins mémoire, "
                     << "fil " << i << ENDL;

            if (info.nbfil > 1)
                {
                    if (! info.cdrom)
                        {
                            thread th{decoder_fichier, ref (Info[i])};
                            t[i] = move (th);
                        }
                    else
                        decoder_fichier (ref (Info[i]));
                }
            else
                {
                    decoder_fichier (ref (Info[0]));
                }

            if (errno)
                {
                    cerr << ENDL << strerror (errno) << ENDL;
                }
        }

    if (verbeux)
        cerr << ENDL PROCESSING_HTML_TAG "Rassemblement des fils d'exécution." ENDL;

    if (info.nbfil > 1 && !info.cdrom)
        for (unsigned i = 0; i < info.nbfil; ++i)
            {
                t[i].join ();
            }

    for (unsigned  i = 0; i < info.nbfil; ++i)
        {
            errno = 0;

            // Lancement des fils d'exécution

            if (verbeux)
                cerr << ENDL PROCESSING_HTML_TAG "Analyse XML, " << "fil " << i << ENDL;

            if (info.nbfil > 1)
                {
                    thread th{parse_info, ref (Info[i])};
                    t[i] = move (th);
                }
            else
                {
                    parse_info (ref (Info[0]));
                }
        }

    if (info.nbfil > 1)
        for (unsigned i = 0; i < info.nbfil; ++i)
            {
                t[i].join ();
            }


    if (info.pretend) return make_pair (0, 0);

    if (Info[0].calculer_maxima)
        {
            calculer_maxima (Info);
        }

    if (! Info[0].chemin_log.empty())
        {
            ofstream LOG;
            LOG.open (Info[0].chemin_log, ios::app);
            calculer_maxima (Info, &LOG);
            LOG.close();
        }


    // Edition éventuelle des bulletins

    if (info.generer_bulletins)
    {
        vString chemins_bulletins_extraits;

        for (auto &&v : vbull)
            {
                string annee = v.at (2);
                string mois  = v.at (1);
                string matricule = v.at (0);

                size_t pos;

                if ((pos = annee.find_first_of ('.')) != string::npos)
                    {
                        int an0 = stoi (annee.substr (0, pos));
                        if (an0 < 100 && an0 > 0) an0 += 2000;

                        pos = annee.find_last_of ('.');
                        int an1 = stoi (annee.substr (pos + 1));
                        if (an1 < 100 && an1 > 0) an1 += 2000;

                        for (int an = an0; an <= an1; ++an)
                            {
                                const string annee = to_string (an);

                                vect_concat(chemins_bulletins_extraits,
                                            scan_mois (repertoire_bulletins,
                                                              Info,
                                                              matricule,
                                                              mois,
                                                              annee));

                            }

                    }
                else
                {
                    int an = stoi (annee);
                    if (an < 100 && an > 0)
                    {
                        an += 2000;
                        annee = to_string(an);
                    }

                    vect_concat(chemins_bulletins_extraits,
                                scan_mois (repertoire_bulletins,
                                           Info,
                                           matricule,
                                           mois,
                                           annee));
                }
            }

        static int res;

        res += chemins_bulletins_extraits.size();

        if (verbeux)
        {
            LOCK_GUARD
            if (res)
                {
                   cerr << STATE_HTML_TAG  << res << " bulletin" << (res > 1 ? "s ont " : " a ") << " été extrait" << (res > 1 ? "s." : ".") << ENDL;
                }
            else
                {
                   cerr << WARNING_HTML_TAG "Aucun bulletin n'a été extrait dans le segment " << nsegment << "." << ENDL;
                }
        }

        vect_concat(info.chemins_bulletins_extraits, chemins_bulletins_extraits);
    }

    pair<uint64_t, uint64_t> lignes;

    if (generer_table)
        {
            cerr << ENDL << PROCESSING_HTML_TAG "Exportation des bases de données au format CSV..." << ENDL ENDL;
            lignes = boucle_ecriture (Info, nsegment);
        }

    // Résumé des erreurs rencontrées

    if (errorLineStack.size() > 0)
        {
            cerr << WARNING_HTML_TAG "<b>Récapitulatif des erreurs rencontrées</b>" << ENDL;

            for (const errorLine_t& e :  errorLineStack)
                {
                    cerr << e.pres;

                    if (e.lineN != -1)
                        cerr << " -- Ligne n°" << e.lineN << ENDL;
                    else
                        cerr << " -- Ligne inconnue." << ENDL;

                }
        }

    if (! Info[0].chemin_log.empty())
        {
            ofstream LOG;
            LOG.open (Info[0].chemin_log, ios::app);

            if (LOG.good())
                for (const errorLine_t& e :  errorLineStack)
                    {
                        if (e.lineN != -1)
                            LOG << " -- Ligne n°" << e.lineN << ENDL;
                        else
                            LOG << " -- Ligne inconnue." << ENDL;
                    }
        }

    generate_rank_signal (-1);
    cerr << " \n";

    // libération de la mémoire

    if (! liberer_memoire) return lignes;

    if (verbeux)
        cerr << ENDL
             << PROCESSING_HTML_TAG "Libération de la mémoire..."
             << ENDL;

    // En cas de problème d'allocation mémoire le mieux est encore de ne pas désallouer car on ne connait pas exacteemnt l'état
    // de la mémoire dynamique

    for (unsigned i = 0; i < Info[0].nbfil; ++i)
        {
            for (unsigned agent = 0; agent < Info[i].NCumAgent; ++agent)
                {
                    if (! Info[i].Table[agent].empty())
                        for (xmlChar* u : Info[i].Table[agent])
                            {
                                if (u != NULL) xmlFree (u);
                            }
                }

        }

    if (Info[0].generer_bulletins)
        {
            for (unsigned i = 0; i < Info[0].nbfil; ++i)
                {
                    for (auto &&f : Info[i].threads->in_memory_file)
                        f.clear();
                }
        }

    return lignes;
}



