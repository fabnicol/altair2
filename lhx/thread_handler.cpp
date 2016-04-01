#include "commandline_handler.h"
#include "thread_handler.h"

thread_handler::thread_handler(Commandline& commande, int rang_segment) : nb_fil {commande.get_nb_fil()}
{

    if (verbeux)
        cerr << PROCESSING_HTML_TAG "Création de " << nb_fil << " fils d'exécution." ENDL;

    vector<thread_t> v_thread_t(nb_fil);

    if (nb_fil > 1)
    {
        fils.resize(nb_fil);
    }

    Info.resize(nb_fil);

    vector<int> nb_fichier_par_fil = commande.nb_fichier_par_fil(rang_segment);

    for (int i = 0; i < nb_fil; ++i)
    {
        Info[i] = commande.info;
        Info[i].threads = &v_thread_t[i];
        Info[i].threads->thread_num = i;
        Info[i].threads->argc = nb_fichier_par_fil.at(i);
        Info[i].threads->argv = commande.get_input(rang_segment).at(i);

        /* Leftovers du segment précédent */

#ifndef STRINGSTREAM_PARSING
       // if (rang_segment > 0) Info[i].threads->argv_cut = leftovers[rang_segment - 1];
#else
       // if (rang_segment > 0) Info[i].threads->in_memory_file_cut = leftovers[rang_segment - 1];
#endif

        /* Donne des Info[i].threads->argv, Info[i].threads->argv_cut
         * ou des    Info[i].threads->in_memory_file, Info[i].threads->in_memory_file_cut */

        if (verbeux)
            cerr <<  PROCESSING_HTML_TAG "Fil d'exécution n°" << i + 1 << "/" << nb_fil
                  << "   Nombre de fichiers dans ce fil : " << Info[i].threads->argc << ENDL;

       try
        {
          redecouper(Info[i]);
        }
        catch(...)
        {
            erreur("Erreur dans le découpage des fichiers volumineux");
        }

        /* Lancement des fils d'exécution */

        if (verbeux && commande.is_reduire_consommation_memoire())
            cerr << ENDL PROCESSING_HTML_TAG "Premier scan des fichiers pour déterminer les besoins mémoire, " << "fil " << i + 1 << ENDL;

        if (nb_fil > 1)
        {
            thread th{Analyseur::decoder_fichier, ref(Info[i])};
            fils[i] = move(th);
        }
        else
        {
            Analyseur::decoder_fichier(ref(Info[0]));
        }
    }

    /* Fin d'exécution des fils et retour au processus maître */

     if (verbeux)
         cerr << ENDL PROCESSING_HTML_TAG "Rassemblement des fils d'exécution." ENDL;

     if (commande.get_nb_fil() > 1)
         for (int i = 0; i < commande.get_nb_fil(); ++i)
         {
             fils[i].join();
         }

}


void thread_handler::redecouper_volumineux(info_t& info)
{

    int fichier_courant = info.fichier_courant;

#if defined(STRINGSTREAM_PARSING)
    if  (! info.threads->in_memory_file_cut.empty()) return;
    string filest = std::move(info.threads->in_memory_file.at(fichier_courant));
    info.threads->in_memory_file_cut.push_back(vector<string>{});
#else
    if (! info.threads->argv_cut.empty()) return;
    ifstream c(info.threads->argv.at(fichier_courant));
    string filest;
    filest = read_stream_into_string(c);
    info.threads->argv_cut.push_back(vector<string>{});
#endif

/* taille est vide */

    uint64_t taille = info.taille.at(fichier_courant);

    if (taille < info.chunksize) return;

    if (verbeux)
        cerr << PROCESSING_HTML_TAG "Fil n°" << info.threads->thread_num + 1 << " Redécoupage du fichier n°" << fichier_courant + 1 << ENDL;

    string document_tag = "", enc = "";
    string::iterator iter = filest.begin();

    while (*++iter != '?' && iter != filest.end()) continue;

    while (++iter != filest.end())
    {
        if (*iter == '?') break;
        if (*iter != 'e') continue;
        string::iterator iter2 = iter;
        //encoding="ISO-8859-1"
        if (*++iter2 != 'n' || *++iter2 != 'c') continue;
        while (*++iter2 != '?') continue;

        enc = string(iter, iter2);
        if (enc.find("encoding") == string::npos)
            enc = "";
        break;
    }

    while (iter != filest.end())
    {
        if (*++iter != 'D') continue;
        ++iter;
        const string s = string(iter, iter + 11);
        // <...:DocumentPaye>
        if (s != "ocumentPaye") continue;
        string::iterator iter2 = iter;
        while (*--iter2 != '<' ) if (iter2 == filest.begin()) break;
        document_tag = string(iter2 + 1, iter + 11);
        break;
    }

    int r = 0;

    uint64_t i = 0, last_pos = 0;

    bool open_di = true;

    while (filest.at(i) != '\0')
    {
        ++r;
        string s = "";
        bool end_loop = false;
        i += info.chunksize;

        if (i < taille)
        {

            while (filest.at(i) != '\0')
            {
                while (filest.at(++i) != '<') continue;
                if (filest.at(++i) != '/') continue;

                if (filest.at(++i) != 'P') continue;

                s = filest.substr(i, 16);

                if (s == "PayeIndivMensuel")
                {
                    break;
                }
                // </PayeIndivMensuel>
            }
        }
        else
        {
            i = taille - 16;
            while (i > last_pos)
            {
                while (filest.at(--i) != 'P') continue;
                if (filest.at(--i) != '/') continue;

                if (filest.at(--i) != '<') continue;

                s = filest.substr(i + 2, 16);

                if (s == "PayeIndivMensuel")
                {
                    break;
                }
            }

            // Il peut ne rien y avoir

            end_loop = true;
        }

        i += 16;

        while (filest.at(++i) != '<') continue;

        s = filest.substr(i + 1 , 13);
        bool insert_di = (s != "/DonneesIndiv");
        if (! insert_di) i += 14;

        if (filest.at(i) != '<') while (filest.at(++i) != '<') continue;

        s = filest.substr(i + 1 , 12);

        string header = (r > 1)? (string("<?xml version=\"1.0\" ") + enc + string("?>")
                                  + string("\n<DocumentPaye>")
                                  + ((! open_di)? "\n<DonneesIndiv>" : "")) : string("");

        string filest_cut = header;
        unsigned L = filest_cut.length();
        filest_cut.resize(L + i - last_pos);

        /* L'utilisation de std::move permet de diminue la consommation mémoire sur les très gros fichiers.
         * Le gain de temps de calcul est toutefois négligeable par rapport à une copie. */

        for (unsigned k = 0; k < i - last_pos; ++k)
             filest_cut[L + k] = move(filest[last_pos + k]);

        filest_cut += ((insert_di)? string("\n</DonneesIndiv>") : "")
                      + (string("\n</") + ((r > 1)? "DocumentPaye" : document_tag) + string(">"));

        open_di = (s == "DonneesIndiv");

#if defined(STRINGSTREAM_PARSING) || defined(MMAP_PARSING)
        info.threads->in_memory_file_cut[fichier_courant].emplace_back(filest_cut);
#else
        string filecut_path = info.threads->argv.at(fichier_courant) +"_" + to_string(r) + ".xhl";
        ofstream filecut(filecut_path);
        filecut << filest_cut;
        filecut.close();
        info.threads->argv_cut[fichier_courant].emplace_back(filecut_path);
#endif

        if (end_loop) break;

        last_pos = i;
    }
}

void thread_handler::redecouper(info_t& info)
{
    for (auto &&p: info.threads->argv)
    {
#ifdef STRINGSTREAM_PARSING

        ifstream c(p.value);

        if (! c.good())
         {
             cerr << ERROR_HTML_TAG "Erreur d'ouverture du fichier " << p.value << ENDL;
#ifdef STRICT
             throw runtime_error {" Exiting."};
#endif
             info.threads->in_memory_file.push_back("");
             continue;
         }

        info.threads->in_memory_file.emplace_back(read_stream_into_string(c));

        c.close();

#endif

        if (info.decoupage_fichiers_volumineux)
        {
          redecouper_volumineux(info);
        }

        ++info.fichier_courant;
   }
}

