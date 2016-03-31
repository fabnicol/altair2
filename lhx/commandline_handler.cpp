#include "commandline_handler.h"

extern bool verbeux, liberer_memoire, generer_table;

Commandline::Commandline(char** argv, int argc)
try
{
    int start = 0;
    string type_table = "bulletins";
    vString cl;  /* pour les lignes de commandes incluses dans un fichier */
    vector<string> commandline_tab;

    for (int i = 1; i < argc; ++i) { commandline_tab.emplace_back(argv[i]);   }

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
            while(f.rdstate() != fstream::eofbit)
            {
                getline(f, ligne);

                if (! ligne.empty())
                    cl.emplace_back(ligne);
            }

            f.close();
            if (! cl.empty())
            {
                argc = cl.size();

                commandline_tab.resize(argc);

                for (int i = 0; i < argc; ++i)
                {
                   if (string(cl.at(i)) == "-f")
                    {
                        erreur("La ligne de commande -f ne peut pas être incluse dans un fichier par -f [risque de boucle infinie].");
                    }

                    commandline_tab[i] = std::move(cl[i]);
                }

                /* on relance ...*/

                start = 0;

                continue;
            }

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

    vString::const_iterator iter = commandline_tab.begin() + start;
    try
    {
        while (iter != commandline_tab.end())
        {
            cerr << *iter;
            this->argv.push_back(make_pair(*iter++, 1));
        }
    }
    catch(...)
    {
        cerr << "Erreur sur argv" ENDL;
        print();
    }

    if (memoire_xhl == 0)
    {
       try { calculer_taille_fichiers(this->argv); }
       catch (...) { cerr << "Erreur sur la taille des fichiers" ENDL; }
    }

    chunksize = info.chunksize;

    nb_fil = info.nbfil;

    memoire();
}
catch(...)
{
  cerr << ERROR_HTML_TAG "Le programme s'est terminé en raison d'erreurs sur la ligne de commande" ENDL;
}

/* A distribuer par fil ! */


void Commandline::repartir_fichiers()
{
     vector<pair<uint64_t, int>>::iterator iter_taille  = taille.begin();
     vector<pair<string, int>>::iterator iter_fichier = argv.begin();

     float densite_xhl_mem = AVERAGE_RAM_DENSITY;

#ifdef STRINGSTREAM_PARSING
        ++densite_xhl_mem;
#endif
#ifndef OFSTREAM_TABLE_OUTPUT
        ++densite_xhl_mem;
#endif

//(1 + rang_segment / SEGMENT_DIVISION_RATE)
     int rang_segment = 0;

     while (iter_taille != taille.end() && iter_fichier != argv.end())
     {
         cerr << "\n-------------- SEGMENT n°"  << ++rang_segment <<"\n\n";

         uint64_t memoire_utilisable_par_fil = min(memoire_utilisable, static_cast<uint64_t>(floor(somme(taille) / nb_fil) + 1.5*chunksize));

         cerr << STATE_HTML_TAG "\nMémoire utilisée par fil : " << memoire_utilisable_par_fil / (1024 * 1024) << " Mo" ENDL;

         vector<vector<pair<string, int>>> input_par_segment;

         vector<pair<string, int>> leftover;
         vector<pair<uint64_t, int>> leftover_taille;

         for (int fil = 0; fil < nb_fil; ++fil)
         {
             vector<pair<string, int>> input_par_segment_par_fil;
             uint64_t taille_segment_par_fil = 0;
             cerr << "\n------- FIL n°"  << fil +1 <<"\n";

             while (iter_taille != taille.end() && iter_fichier != argv.end())
             {
                 uint64_t incr;
                 int nb_fichier = 0;
                 int nb_decoupe;
                 bool out_of_memory = false;

                 if (iter_fichier->second == 1)
                     nb_decoupe = (iter_taille->first <= chunksize) ? 1 : static_cast<int>(iter_taille->first / chunksize) + 1;
                 else
                 {
                     nb_decoupe = iter_fichier->second;
                     cerr << "\n### Récupération des leftovers du segment précédent : " << iter_fichier->first << "  " << iter_fichier->second << "\n";
                 }

                 for (int k = 1; k <= nb_decoupe; ++k)
                 {
                   if (nb_decoupe == 1)
                     incr = iter_taille->first;
                   else
                   if (k != nb_decoupe)
                     incr = chunksize;
                   else
                     incr = iter_taille->first % chunksize;

                   cerr << "fil " << fil +1<< " iteration " << k << "/" << nb_decoupe << " TSF " << taille_segment_par_fil << " incr " << incr << ENDL;

                   if ((taille_segment_par_fil + incr) /* * densite_xhl_mem */ < memoire_utilisable_par_fil)
                   {
                       taille_segment_par_fil += incr;
                       ++nb_fichier;
                       cerr << "--> nb fichier "<< nb_fichier << ENDL;
                   }
                   else
                   {
                     out_of_memory = true;
                     break;
                   }
                 }

                 cerr << "->nouvelle paire " << iter_fichier->first << " " << nb_fichier << ENDL;
                 if (nb_fichier)
                 {
                     iter_fichier->second = nb_fichier;
                     input_par_segment_par_fil.emplace_back(*iter_fichier);
                 }
                 if (out_of_memory || iter_fichier == argv.end())
                 {
                     cerr << "\n=== nouveau segment_par_fil : " << "\n";
                     for (auto&& s: input_par_segment_par_fil) cerr << s.first << "\n";
                     input_par_segment.emplace_back(input_par_segment_par_fil);

                     if (nb_fichier < nb_decoupe)
                     {
                         cerr << "\n===--- leftover de segment_par_fil : " << iter_fichier->first << " indice " << nb_decoupe - nb_fichier << "\n";

                         leftover.emplace_back(make_pair(iter_fichier->first, nb_decoupe - nb_fichier));
                         leftover_taille.emplace_back(make_pair(iter_taille->first, nb_decoupe - nb_fichier));
                     }
                     break;
                 }

                 ++iter_taille;
                 ++iter_fichier;
             }
        }

        input.emplace_back(input_par_segment);
        vector<pair<string, int>> argv2(iter_fichier, argv.end());
        argv.swap(argv2);
        argv.insert(argv.begin(), leftover.begin(), leftover.end());
        vector<pair<uint64_t, int>> taille2(iter_taille, taille.end());
        taille.swap(taille2);
        taille.insert(taille.begin(), leftover_taille.begin(), leftover_taille.end());
        leftover.clear();
        leftover_taille.clear();
        iter_fichier = argv.begin();
        iter_taille  = taille.begin();
     }

      nb_fichier_par_segment = get_nb_fichier();
}


void Commandline::calculer_taille_fichiers(const vector<pair<string, int>>& files, bool silent)
{
    off_t mem = 0;
    int count = 0;
    uint64_t memoire_xhl = 0;

    for (auto && s : files)
    {
        if ((mem = taille_fichier(s.first)) != -1)
        {
            memoire_xhl += static_cast<uint64_t>(mem);
            taille.push_back(make_pair(mem, 1));
            ++count;
        }
        else
        {
            cerr << ERROR_HTML_TAG "La taille du fichier " << s.first << " n'a pas pu être déterminée." ENDL;
            cerr << STATE_HTML_TAG "Utilisation de la taille par défaut" ENDL;
            taille.push_back(make_pair(CUTFILE_CHUNK, 1));
        }
    }

    if (! silent)
       cerr << ENDL STATE_HTML_TAG << "Taille totale des " << count << " fichiers : " << memoire_xhl / 1048576 << " Mo."  ENDL;
}


 void Commandline::memoire()
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

