#include "commandline_handler.h"

extern bool verbeux, liberer_memoire, generer_table;

Commandline_handler::Commandline_handler(info_t& info, char** argv, int argc)
try
{
    int start = 1;
    string type_table = "bulletins";
    vString cl;  /* pour les lignes de commandes incluses dans un fichier */
    vector<string> commandline_tab(argc);
    for (int i = 0; i < argc; ++i) commandline_tab.emplace_back(argv[i]);

    while (start < argc)
    {
        if (commandline_tab[start] == "-n")
        {
            info.reduire_consommation_memoire = false;
            info.nbAgentUtilisateur
                    = lire_argument (argc, const_cast<char*>(commandline_tab[start + 1].c_str()));
            if (info.nbAgentUtilisateur < 1)
            {
                erreur("Préciser le nombre de bulletins mensuels attendus (majorant du nombre) avec -N xxx .");
            }
            start += 2;
            continue;
        }
        else if (commandline_tab[start] ==  "-h")
        {
            string out = move(help()).str();
            out.erase(remove(out.begin(), out.end(), '*'), out.end());
            cerr << out;
            return;
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

           return;
        }
        else if (commandline_tab[start] ==  "--entier")
        {
            info.decoupage_fichiers_volumineux = false;
            ++start;
            continue;
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
                erreur("Option -T suivi d'un argument obligatoire (nombre de lignes).");
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
            hashTable["RE"] = BaseType::PAR_RETENUE;
            hashTable["X"]  = BaseType::TOUTES_CATEGORIES;

            if (hashTable.find(commandline_tab[start + 1]) != hashTable.end())
            {
                BaseType type = hashTable[commandline_tab[start + 1]];
                info.type_base = (info.type_base == BaseType::MAXIMUM_LIGNES && type == BaseType::PAR_ANNEE)? BaseType::MAXIMUM_LIGNES_PAR_ANNEE : type;

            }
            else
            {
                // On a trouvé un argument numérique en ligne de commande

                int32_t size_read = lire_argument(argc, const_cast<char*>(commandline_tab[start + 1].c_str()));

                if (size_read < 0 || size_read > INT32_MAX -1)
                {
                    erreur("Le nombre de lignes doit être compris entre 0 et INT64_MAX");
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
                erreur("Option -s suivi d'un argument obligatoire (séparateur de champs).");
            }
            info.separateur = commandline_tab[start + 1][0];

            if (info.separateur == '_')
            {
                erreur("Le séparateur ne doit pas être '_'");
            }

            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "-d")
        {
            if (start + 1 == argc)
            {
                erreur("Option -d suivi d'un argument obligatoire (séparateur décimal).");
            }
            info.decimal = commandline_tab[start + 1][0];
            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "-o")
        {
            if (start + 1 == argc)
            {
                erreur("Option -o suivi d'un argument obligatoire (nom de  fichier).");
            }

            info.chemin_base = commandline_tab[start + 1];

            ofstream base;
            base.open(info.chemin_base);
            if (! base.good())
            {
                erreur<ios_base::failure>("La base de données ne peut être créée, vérifier l'existence du dossier.");
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
                erreur<ios_base::failure>(string("La base de données ")
                                         + info.chemin_base  + string(CSV)
                                         + string(" ne peut être créée, vérifier l'existence du dossier."));

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
                    erreur("Le nombre de fils d'exécution doit être au moins égal à 1.");
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
                erreur<ios_base::failure>("Le log ne peut être créé, vérifier l'existence du dossier.");
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
                erreur("Préciser le nombre de nombre maximum d'agents par mois attendus (majorant du nombre) avec -n xxx");
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
                erreur("Il manque l'expression régulière.");
            }
            start += 2;
            continue;
        }
#ifdef GENERATE_RANK_SIGNAL
        else if (commandline_tab[start] == "--rank")
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
            info.select_siret = true;
            ++start;
            continue;
        }
        else if (commandline_tab[start] == "-E")
        {
            if (argc > start + 1)
            {
                info.select_echelon = true;
            }
            else
            {
                erreur("Il faut au moins un fichier");
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
                erreur("Il manque le fichier de ligne de commande.");
            }

            ifstream f;
            f.open(fichier);
            if (! f.good())
            {
                erreur<ios_base::failure>("Impossible d'ouvrir le fichier " + string(fichier));
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
                        erreur("La ligne de commande -f ne peut pas être incluse dans un fichier par -f [risque de boucle infinie].");
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
#ifdef FGETC_PARSING
        else if (commandline_tab[start] == "--preserve-tempfiles")
        {
            info.preserve_tempfiles = true;   // garder les fichiers remporaires
            ++start;
            continue;
        }
#endif
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
                    erreur("La donnée de la taille des fichiers en input est erronée ou au-delà de la limite permise (60 Go).");
                }
            }
            else
            {
                erreur("Le plus grand entier non signé est inférieur à 60 * 1048576");
            }
        }
        else if (commandline_tab[start] == "--memshare")
        {
            int part = stoi(commandline_tab[start + 1], nullptr);
            if (verbeux)
            cerr << STATE_HTML_TAG "Part de la mémoire vive utilisée : " <<  part << " %" ENDL;

            ajustement = (float) part / 100;
            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "--chunksize")
        {
            int chunksize = stoi(commandline_tab[start + 1], nullptr);
            if (verbeux)
            cerr << STATE_HTML_TAG "Taille unitaire des découpes de fichiers volumineux : " <<  chunksize << " Mo" ENDL;

            info.chunksize = chunksize * 1024 * 1024;

            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "--segments ")
        {
            cerr << STATE_HTML_TAG "Les bases seront analysées en au moins : " << commandline_tab[start + 1] << " segments" << ENDL;

            // au maximum 99 segments

            nsegments = stoi(commandline_tab[start + 1], nullptr);
            if (nsegments > 1 || nsegments < 100)
            {
                start += 2;
                continue;
            }
            else
            {
                erreur("Il doit y avoir entre 1 et 99 segments de bases.");
            }
        }

        else if (commandline_tab[start][0] == '-')
        {
            erreur("Option inconnue " + commandline_tab[start]);
        }
        else break;
    }

    index_debut_fichiers  = start;
    this->argv = std::move(commandline_tab);
    memoire();
}
catch(...) {
 cerr << ERROR_HTML_TAG "Le programme s'est terminé en raison d'erreurs sur la ligne de commande" ENDL;
}
