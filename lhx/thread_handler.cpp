#include "commandline_handler.h"
#include "thread_handler.h"

thread_handler::thread_handler(Commandline& commande, int rang_segment) : nb_fil {commande.get_nb_fil()}
{

    if (verbeux)
        cerr << PROCESSING_HTML_TAG "Cr�ation de " << nb_fil << " fils d'ex�cution." ENDL;

    vector<thread_t> v_thread_t(nb_fil);

    if (nb_fil > 1)
    {
        fils.resize(nb_fil);
    }

    Info.resize(nb_fil);

    vector<int> nb_fichier_par_fil = commande.nb_fichier_par_fil(rang_segment);

    auto input_segment = commande.get_input(rang_segment);
    for (unsigned i = 0; i < nb_fichier_par_fil.size(); ++i)
        cerr << "fil " << i + 1 << " Info[i].threads->argc " << nb_fichier_par_fil.at(i)  << endl;

    for (int i = 0; i < nb_fil; ++i)
    {
        Info[i] = commande.info;
        Info[i].threads = &v_thread_t[i];
        Info[i].threads->thread_num = i;

        /* Nombre de fichiers quad du fil */
        Info[i].threads->argc = nb_fichier_par_fil.at(i);

        /* Fichiers quad du fil */

        Info[i].threads->argv = input_segment.at(i);

        /* Leftovers du segment pr�c�dent */

#ifndef STRINGSTREAM_PARSING
       // if (rang_segment > 0) Info[i].threads->argv_cut = leftovers[rang_segment - 1];
#else
       // if (rang_segment > 0) Info[i].threads->in_memory_file_cut = leftovers[rang_segment - 1];
#endif

        /* Donne des Info[i].threads->argv, Info[i].threads->argv_cut
         * ou des    Info[i].threads->in_memory_file, Info[i].threads->in_memory_file_cut */

        if (verbeux)
            cerr <<  PROCESSING_HTML_TAG "Fil d'ex�cution n�" << i + 1 << "/" << nb_fil
                 << "   Nombre de fichiers dans ce fil : " << Info[i].threads->argc << ENDL;

#ifdef CATCH
       try
        {
#endif
          redecouper(Info[i]);
#ifdef CATCH
        }
        catch(...)
        {
            erreur("Erreur dans le d�coupage des fichiers volumineux");
        }
#endif

        /* Lancement des fils d'ex�cution */

        if (verbeux && commande.is_reduire_consommation_memoire())
            cerr << ENDL PROCESSING_HTML_TAG "Premier scan des fichiers pour d�terminer les besoins m�moire, " << "fil " << i + 1 << ENDL;

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

    /* Fin d'ex�cution des fils et retour au processus ma�tre */

     if (verbeux)
         cerr << ENDL PROCESSING_HTML_TAG "Rassemblement des fils d'ex�cution." ENDL;

     if (commande.get_nb_fil() > 1)
         for (int i = 0; i < commande.get_nb_fil(); ++i)
         {
             fils[i].join();
         }

}


void thread_handler::redecouper_volumineux(info_t& info, quad<string, uint64_t, int, int>& tr)
{

    int fichier_courant = info.fichier_courant;

#if defined(STRINGSTREAM_PARSING)
   // if  (! info.threads->in_memory_file_cut.empty()) return;
    string filest = move(info.threads->in_memory_file[fichier_courant]);
    info.threads->in_memory_file[fichier_courant] = string {};
#else
    if (! info.threads->argv_cut.empty()) return;
    ifstream c(info.threads->argv.at(fichier_courant));
    string filest;
    filest = read_stream_into_string(c);
    info.threads->argv_cut.push_back(vector<string>{});
#endif

    if (verbeux)
        cerr << PROCESSING_HTML_TAG "Fil n�" << info.threads->thread_num + 1 << " Red�coupage du fichier n�" << fichier_courant + 1 << ENDL;

    string document_tag = "", enc = "";
    string::iterator iter = filest.begin();

    while (++iter != filest.end() && *iter != '?') continue;

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

    while (++iter != filest.end())
    {
        if (*iter != 'D') continue;
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
        uint64_t taille = taille_fichier(tr.value);

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

        /* L'utilisation de std::move permet de diminue la consommation m�moire sur les tr�s gros fichiers.
         * Le gain de temps de calcul est toutefois n�gligeable par rapport � une copie. */

        for (unsigned k = 0; k < i - last_pos; ++k)
             filest_cut[L + k] = move(filest[last_pos + k]);

        filest_cut += ((insert_di)? string("\n</DonneesIndiv>") : "")
                      + (string("\n</") + ((r > 1)? "DocumentPaye" : document_tag) + string(">"));

        open_di = (s == "DonneesIndiv");

#if defined(STRINGSTREAM_PARSING) || defined(MMAP_PARSING)
        info.threads->in_memory_file[fichier_courant] = move(filest_cut);
        ++fichier_courant;
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
  info.threads->in_memory_file.clear();
  info.threads->in_memory_file.resize(info.threads->argc);
  info.fichier_courant = 0;

  for (auto &&tr : info.threads->argv)
  {

        #ifdef STRINGSTREAM_PARSING
          ifstream c(tr.value, std::ios::in | std::ios::binary);

          if (! c.good())
           {
             cerr << ERROR_HTML_TAG "Erreur d'ouverture du fichier " << tr.value << ENDL;
             #ifdef STRICT
               throw runtime_error {" Exiting."};
             #endif
             info.threads->in_memory_file.push_back("");
             continue;
           }

          cerr << "FC " << info.fichier_courant << endl;
          cerr << "IMF size " << info.threads->in_memory_file.size() << endl;
          info.threads->in_memory_file[info.fichier_courant] = move(read_stream_into_string(c));
          ++info.fichier_courant;

          c.close();
       #endif

        if (info.decoupage_fichiers_volumineux && tr.elements > 1)
        {
          redecouper_volumineux(info, tr);
        }
   }
}

