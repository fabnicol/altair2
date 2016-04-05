#include "commandline_handler.h"
#include "thread_handler.h"
#include "validator.hpp"

thread_handler::thread_handler(Commandline& commande, int rang_segment)
    : nb_fil {commande.get_nb_fil()},
      is_liberer_memoire {commande.is_liberer_memoire()}
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
        Info[i].threads->rang_segment = rang_segment;

        /* Nombre de fichiers quad du fil */
        Info[i].threads->argc = nb_fichier_par_fil.at(i);
#if 0
        ofstream log("log", ios_base::app);
        log << "thread " << i << "/" << nb_fil <<  " rang_sement " << rang_segment << " Info[i].threads->argc " << Info[i].threads->argc << endl;
#endif
        /* Fichiers quad du fil */

        Info[i].threads->argv = input_segment.at(i);

        /* Leftovers du segment pr�c�dent */

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

     if (nb_fil > 1)
         for (int i = 0; i < nb_fil; ++i)
         {
             fils[i].join();
         }
}

thread_handler::~thread_handler()
{
    if (is_liberer_memoire)
    {
        if (verbeux)
            cerr << ENDL
                 << PROCESSING_HTML_TAG "Lib�ration de la m�moire..."
                 << ENDL;

            for (int i = 0; i < nb_fil; ++i)
            {
                for (unsigned agent = 0; agent < Info.at(i).NCumAgent; ++agent)
                {
                    for (int j = 0; j < Info.at(i).Memoire_p_ligne[agent]; ++j)
                    {
                        if (j != Categorie && xmlStrcmp(Info.at(i).Table[agent][j], (const xmlChar*) "") != 0)
                            xmlFree(Info[i].Table[agent][j]);
                    }
                }
            }
    }
}

void thread_handler::redecouper_volumineux(info_t& info, quad<>& tr)
{

    string fichier_courant = tr.value;
    int rang_segment = info.threads->rang_segment;

#if defined(STRINGSTREAM_PARSING)
   // if  (! info.threads->in_memory_file_cut.empty()) return;
    string filest = move(info.threads->in_memory_file[fichier_courant][rang_segment][0]);
    info.threads->in_memory_file[fichier_courant][rang_segment].clear();
#else
    if (! info.threads->argv_cut.empty()) return;
    ifstream c(info.threads->argv.at(fichier_courant));
    string filest;
    filest = read_stream_into_string(c);
    info.threads->argv_cut.push_back(vector<string>{});
#endif

    if (verbeux)
        cerr << PROCESSING_HTML_TAG " Fil n�" << info.threads->thread_num + 1 << " Red�coupage du fichier n�" << fichier_courant  << ENDL;

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
    int element = 0;

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
        if (element < info.hash_size[fichier_courant][rang_segment][info.threads->thread_num])
            info.threads->in_memory_file[fichier_courant][rang_segment].emplace_back(filest_cut);
        else
        {
          element = 0;
          //while (info.threads->in_memory_file[fichier_courant][rang_segment].size() == 0)
          ++rang_segment;

          info.threads->in_memory_file[fichier_courant][rang_segment].emplace_back(filest_cut);
        }
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

  for (auto &&tr : info.threads->argv)
  {
      #ifdef STRINGSTREAM_PARSING

      ofstream log("log", ios_base::app);

#if 0
      log << endl << "info.threads->argv " << tr.value << " elements: " << tr.elements
          << " size " << tr.size/(1024*1024) << " taille " << taille_fichier(tr.value) << " status " << tr.status << endl;
#endif

      if (tr.status == FIRST_CUT || tr.status == NO_LEFTOVER)
      {
          ifstream c(tr.value, std::ios::in | std::ios::binary);

          if (! c.good())
           {
             cerr << ERROR_HTML_TAG "Erreur d'ouverture du fichier " << tr.value << ENDL;
             #ifdef STRICT
               throw runtime_error {" Exiting."};
             #endif
             info.threads->in_memory_file[tr.value][info.threads->rang_segment].push_back("");
             continue;
           }

          info.threads->in_memory_file[tr.value][info.threads->rang_segment].emplace_back(read_stream_into_string(c));

          c.close();
          if (info.decoupage_fichiers_volumineux && tr.elements > 1)
          {
            redecouper_volumineux(info, tr);
          }
      }


     #endif


   }
}

