#include "commandline_handler.h"

extern bool verbeux, generer_table;

Commandline::Commandline(char** argv, int argc)
#ifdef CATCH
try
#endif
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
                erreur("Pr�ciser le nombre de bulletins mensuels attendus (majorant du nombre) avec -N xxx .");
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
                // On a trouv� un argument num�rique en ligne de commande

                int32_t size_read = lire_argument(argc, const_cast<char*>(commandline_tab[start + 1].c_str()));

                if (size_read < 0 || size_read > INT32_MAX -1)
                {
                    erreur("Le nombre de lignes doit �tre compris entre 0 et INT64_MAX");
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
                erreur("Option -s suivi d'un argument obligatoire (s�parateur de champs).");
            }
            info.separateur = commandline_tab[start + 1][0];

            if (info.separateur == '_')
            {
                erreur("Le s�parateur ne doit pas �tre '_'");
            }

            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "-d")
        {
            if (start + 1 == argc)
            {
                erreur("Option -d suivi d'un argument obligatoire (s�parateur d�cimal).");
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
                erreur<ios_base::failure>("La base de donn�es ne peut �tre cr��e, v�rifier l'existence du dossier.");
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
                erreur<ios_base::failure>(string("La base de donn�es ")
                                          + info.chemin_base  + string(CSV)
                                          + string(" ne peut �tre cr��e, v�rifier l'existence du dossier."));

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
                    erreur("Le nombre de fils d'ex�cution doit �tre au moins �gal � 1.");
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
                erreur<ios_base::failure>("Le log ne peut �tre cr��, v�rifier l'existence du dossier.");
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
                erreur("Pr�ciser le nombre de nombre maximum d'agents par mois attendus (majorant du nombre) avec -n xxx");
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
                        erreur("La ligne de commande -f ne peut pas �tre incluse dans un fichier par -f [risque de boucle infinie].");
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
            info.pretend = true;   // pas de d�codage
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
                    erreur("La donn�e de la taille des fichiers en input est erron�e ou au-del� de la limite permise (60 Go).");
                }
            }
            else
            {
                erreur("Le plus grand entier non sign� est inf�rieur � 60 * 1048576");
            }
        }
        else if (commandline_tab[start] == "--memshare")
        {
            int part = stoi(commandline_tab[start + 1], nullptr);
            if (verbeux)
                cerr << STATE_HTML_TAG "Part de la m�moire vive utilis�e : " <<  part << " %" ENDL;

            ajustement = (float) part / 100;
            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "--chunksize")
        {
            int chunksize = stoi(commandline_tab[start + 1], nullptr);
            if (verbeux)
                cerr << STATE_HTML_TAG "Taille unitaire des d�coupes de fichiers volumineux : " <<  chunksize << " Mo" ENDL;

            info.chunksize = chunksize * 1024 * 1024;

            start += 2;
            continue;
        }
        else if (commandline_tab[start] == "--segments ")
        {
            cerr << STATE_HTML_TAG "Les bases seront analys�es en au moins : " << commandline_tab[start + 1] << " segments" << ENDL;

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

#ifdef CATCH
    try
    {
#endif
        while (iter != commandline_tab.end())
        {
            this->argv.push_back(quad<> {*iter, taille_fichier(*iter), 1, NO_LEFTOVER});
            iter++;
        }
#ifdef CATCH
    }
    catch(...)
    {
        cerr << "Erreur sur argv" ENDL;
        print();
    }
#endif
#if 0
    if (memoire_xhl == 0)
    {
        try { calculer_taille_fichiers(this->argv); }
        catch (...) { cerr << "Erreur sur la taille des fichiers" ENDL; }
    }
#endif
    chunksize = info.chunksize;

    nb_fil = info.nbfil;

    memoire();

}
#ifdef CATCH
catch(...)
{
  cerr << ERROR_HTML_TAG "Le programme s'est termin� en raison d'erreurs sur la ligne de commande" ENDL;
}
#endif
/* A distribuer par fil ! */

#ifndef REP_DEBUG
#define debug(X)
#else
#define debug(X) X
#endif

/* dans chaque input_par_segment[fil] on a un quad qui pr�cise :
 *   - le chemin (value),
 *   - le nombre de d'�l�ments de taille chunksize � int�grer dans le fil du segment (elements)
 *   - la taille r�siduelle apr�s d�duction des �l�ments de la taille initiale (size)
 *   - le statut (status) : premi�re d�coupe : FIRST_CUT, d�coupe interm�diaire MEDIUM_CUT ou terminale LEFTOVER*/

/* Structure des donn�es :
 *   quad { value : nom du fichier et chemin : string;
 *          size     : en octets, taille initiale ou r�siduelle  : uint64_t ;
 *          elements : nombre de d�coupages du fichier ma�tre dans le segment en cours : int ;
 *          status   : LEFTOVER, NO_LEFTOVER, INCOMPLETE : int } ; */

bool Commandline::allouer_fil(const int fil,
                              vector<quad<>>::iterator& iter_fichier)
{
    float densite_xhl_mem = AVERAGE_RAM_DENSITY;

    #ifdef STRINGSTREAM_PARSING
      ++densite_xhl_mem;
    #endif
    #ifndef OFSTREAM_TABLE_OUTPUT
      ++densite_xhl_mem;
    #endif

    uint64_t incr;
    bool in_memory = true;

    int nb_decoupe = (iter_fichier->size <= chunksize) ? 1 : static_cast<int>(iter_fichier->size / chunksize) + 1;
#if 0
    ofstream log("log", ios_base::app);
    if (iter_fichier->status != NO_LEFTOVER)
    {
        log << "\n### R�cup�ration des leftovers de segments pr�c�dents : " << iter_fichier->value << "  " << iter_fichier->elements << "\n";
    }
#endif

    int k;

    /* Ajouter des �l�ments du quad *iter_fichier (soit au max. nb_decoupe) tant que l'incr�ment incr
     * correspondant un �l�ment, accumul� dans taille_segment[fil], ne d�passe par la m�moire utilisable/AVERAGE_RAM_DENSITY */

    for (k = 0; k < nb_decoupe; ++k)
    {
        if (nb_decoupe == 1)
            incr = iter_fichier->size;
        else
        if (k != nb_decoupe)
            incr = chunksize;
        else
            incr = iter_fichier->size % chunksize;

        if ((taille_segment.at(fil) + incr) * densite_xhl_mem < memoire_utilisable_par_fil)
        {
            taille_segment[fil] += incr;
        }
        else
        {
            in_memory = false;
            break;
        }

        iter_fichier->size -= incr;
    }

    /* Lorsque le crit�re de test n'est pas satisfait, k n'est pas incr�ment� et est donc au plus nb_decoupe - 1
     * Que le crit�re soit satisfait ou pas, le nombre d'�l�ments du fichier soit iter_fichier->elements vaut au plus k */

    iter_fichier->elements =  k ;

#if 0
    log << endl << iter_fichier->value << "  " <<  " k " << k << " iter_fichier->size " << iter_fichier->size << endl;
    log << "->nouvelle paire " << iter_fichier->value << " " << iter_fichier->elements << ENDL;
    log << "-->leftover : " << nb_decoupe - iter_fichier->elements << ENDL;
#endif

    /* Si on prend un fichier entier :
     *   si on le d�coupe et si tout rentre en m�moire, on ne fait rien
     *   si on le d�coupe si si une partie ne rentre pas en m�moire, on accorde le statut FIRST_CUT
     * Si on prend un fichier d�j� d�coup� dans un segment pr�c�dent :
     *   si tous les �lements rentrent en m�moire, c'est la fin de l'op�ration d'allocation :statut LEFTOVER
     *   si certains �l�ments ne rentrent toujours pas en m�moire, il faudra encore au moins un segment
     *   cette situation a priori rare a le statut MEDIUM_CUT */

    if (iter_fichier->status == NO_LEFTOVER)
    {
       if (iter_fichier->elements > 1
            &&
           in_memory == false)

                  iter_fichier->status =  FIRST_CUT;
    }
    else
    {
        if (in_memory == true)
                   iter_fichier->status = LEFTOVER;
        else
         if (in_memory == false)
                   iter_fichier->status = MEDIUM_CUT;

    }

    /* on retient un quad dans l'ensemble des quads du fil pour le segment donn� (input_par_segment[fil])
     * d�s lors que au moins un �l�ment du quad est allou� dans le segment en cours */

    if (k) input_par_segment[fil].push_back(*iter_fichier);

    /* on retourne le bool�en d�crivant la situation fichier entier (ou pas) dans le segment en cours */

    return in_memory;
}


/* Algorithme  de r�partition
 *
 * On alloue un fichier � un fil du segment en cours, �ventuellement de mani�re partielle.
 * Si l'allocation est incompl�te et porte sur un nombre donn� d'�l�ments, essaie d'allouer le reste au segment suivant.
 *
 * Les allocations de fichiers aux fils se font de mani�re cyclique sur les fils : si le quad n est allou� au fil i
 * alors on alloue en principe le quad n + 1 au fil i + 1 modulo nb_fil.
 * On arr�te l'allocation du segment lorsque on n'arrive plus � allouer int�gralement aucun nouveau fichier sur aucun fil.
 */

void Commandline::repartir_fichiers()
{
    int rang_segment = 0;

    memoire_utilisable_par_fil = memoire_utilisable / nb_fil;

#if 0
    ofstream log("log", ios_base::app);
#endif

    vector<quad<>>::iterator iter_fichier = argv.begin();
    vector<quad<>> leftover;

    while (iter_fichier != argv.end())
    {

#if 0
        log << "\n-------------- SEGMENT n�"  << rang_segment + 1 <<"\n\n";
        log << STATE_HTML_TAG "\nM�moire utilis�e par fil : " << memoire_utilisable_par_fil / (1024 * 1024) << " Mo" ENDL;
#endif
        /* D�but d'un nouveau segment : on alloue de l'espace pour stocker les quads de nb_fil
         * et de m�me pour le stockage des tailles de fichier. On nettoie le vecteur de stockage de
         * quads qui permet de communiquer les "restes non allou�s" du segment pr�c�dent */

        leftover.clear();
        input_par_segment.clear();
        input_par_segment.resize(nb_fil);
        taille_segment.clear();
        taille_segment.resize(nb_fil);

        int fil = 0;
        int s_allouable = 0;
        bool allouable = true;
        int nb_fichier = 0;  // stocke le nombre total d'�l�ments de quad pour le segment en cours

        while (iter_fichier != argv.end())
        {
            allouable = allouer_fil(fil, iter_fichier);
            s_allouable += (allouable == true);

            /* � ins�rer apr�s l'appel de allouer_fil */

            nb_fichier += iter_fichier->elements;

            /* si le quad n'est pas enti�rement allouable dans le segment et le fil en cours
             * alors on r�serve le quad pour le segment suivant */

            if (! allouable)
            {
              leftover.push_back(*iter_fichier);
            }

            /* Une table de hashage hash_size sur le chemin des fichiers et l'index du segment associe un
             * le nombre d'�l�ments du fichier int�gr�s au fil en cours (sous la forme d'un vecteur
             * de nombre d'�l�ments index� par le fil en cours) */

            if (iter_fichier->elements) info.hash_size[iter_fichier->value][rang_segment][fil] = iter_fichier->elements;

            ++iter_fichier;

            /* crt�re de sortie du segment : */

            ++fil;

            if (nb_fil == fil)
            {
                if (! s_allouable) break;
                fil = 0;
                s_allouable = 0;
            }
        }

        /* bilan segment en cours et pr�paration du suivant */

        nb_fichier_par_segment.push_back(nb_fichier);  // comprendre nombre d'�l�ments de quad

        /* le vecteur input contient toutes les donn�es par strates de segment, fil et quad
         * sauf les nombres d'�l�ments */

        input.push_back(input_par_segment);

        /* Le leftover du segment en cours est rajout� en t�te de liste des fichiers � allouer au segment suivant */

        vector<quad<>> argv2(iter_fichier, argv.end());  // le reste � analyser
        argv.clear();
        argv = move(argv2);

        if (! leftover.empty())
            argv.insert(argv.begin(), leftover.begin(), leftover.end());  // on pourrait utiliser une deque pour argv

        iter_fichier = argv.begin();

        ++rang_segment;
   }
}

#if 0
void Commandline::calculer_taille_fichiers(const vector<quad<>>& files, bool silent)
{
    off_t mem = 0;
    int count = 0;
    uint64_t memoire_xhl = 0;

    for (auto && s : files)
    {
        if ((mem = taille_fichier(s.value)) != -1)
        {
            memoire_xhl += static_cast<uint64_t>(mem);
            taille.push_back(quad<uint64_t, uint64_t, int, int> {static_cast<uint64_t>(mem), 1, 0});
            ++count;
        }
        else
        {
            cerr << ERROR_HTML_TAG "La taille du fichier " << s.value << " n'a pas pu �tre d�termin�e." ENDL;
            cerr << STATE_HTML_TAG "Utilisation de la taille par d�faut" ENDL;
            taille.push_back(quad<uint64_t, uint64_t, int, int> {CUTFILE_CHUNK, 1, 0});
        }
    }

    if (! silent)
        cerr << ENDL STATE_HTML_TAG << "Taille totale des " << count << " fichiers : " << memoire_xhl / 1048576 << " Mo."  ENDL;
}
#endif

void Commandline::memoire()
{
    cerr << STATE_HTML_TAG
         << "M�moire disponible " <<  ((memoire_disponible = getFreeSystemMemory()) / 1048576)
         << " / " << (getTotalSystemMemory()  / 1048576)
         << " Mo."  ENDL;

    memoire_utilisable = floor(ajustement * static_cast<float>(memoire_disponible));

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
                    cerr << ERROR_HTML_TAG "Impossible de g�n�rer " << nsegments
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

