#include "thread_handler.h"
#include "table.hpp"

extern bool verbeux;

Analyseur::Analyseur(Commandline& commande)
{
    int nb_segment = commande.get_nsegments();
    if (verbeux)
    {
        if (nb_segment > 1)
            cerr << PROCESSING_HTML_TAG << "Les bases en sortie seront analysées en " << nb_segment << " segments." ENDL;
        else
            cerr << PROCESSING_HTML_TAG << "Les bases en sortie seront analysées en une seule itération."  ENDL;
    }

    lanceur(commande, nb_segment);
}

void Analyseur::lanceur(Commandline& commande, int nb_segment)
{
    for(int rang_segment = 0; rang_segment < nb_segment; ++rang_segment)
    {
        unsigned nb_fichier = commande.get_nb_fichier(rang_segment);

        if (commande.info.nbfil > nb_fichier)
        {
            cerr << ERROR_HTML_TAG "Segment n°" << rang_segment << " : trop de fils (" << commande.info.nbfil << ") pour le nombre de fichiers (" << nb_fichier << ")" ENDL;
            cerr << ERROR_HTML_TAG "Exécution avec " << nb_fichier << pluriel(nb_fichier, " fil") <<"."  ENDL;

            commande.set_nbfil(nb_fichier);
        }
        else
            commande.set_nbfil(commande.info.nbfil);

        /* Le nombre de fils du segment doit impérativement être ajusté avant de lancer la production */

        produire_segment(commande, rang_segment);
    }
}

thread_handler::thread_handler(Commandline& commande, int rang_segment)
{
    int nb_fil = commande.get_nb_fil();

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
        Info[i].threads->argv = commande.get_input(rang_segment);

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

        redecouper(Info[i]);

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



int Analyseur::produire_segment(Commandline& commande, int rang_segment)
{
    /* Lancement des fils */

    thread_handler gestionnaire_fils { commande, rang_segment };





    if (commande.is_pretend()) return 2;

    /* Gestion des informations de fin d'exécution */

    if (commande.is_calculer_maxima())
    {
        calculer_maxima(gestionnaire_fils.Info);
    }

    if (! commande.get_chemin_log().empty())
    {
        ofstream LOG;
        LOG.open(commande.get_chemin_log(), ios::app);
        calculer_maxima(gestionnaire_fils.Info, &LOG);
        LOG.close();
    }

    /* Résumé des erreurs rencontrées */

    if (errorLineStack.size() > 0)
    {
        cerr << WARNING_HTML_TAG "<b>Récapitulatif des erreurs rencontrées</b>" << ENDL;
        for (const errorLine_t& e :  errorLineStack)
        {
            cerr << e.filePath;
            if (e.lineN != -1)
                cerr << " -- Ligne n°" << e.lineN << ENDL;
            else
                cerr << " -- Ligne inconnue." << ENDL;

        }
    }

    if (! commande.get_chemin_log().empty())
    {
        ofstream LOG;
        LOG.open(commande.get_chemin_log(), ios::app);
        if (LOG.good())
            for (const errorLine_t& e :  errorLineStack)
            {
                if (e.lineN != -1)
                    LOG << " -- Ligne n°" << e.lineN << ENDL;
                else
                    LOG << " -- Ligne inconnue." << ENDL;
            }
    }

    /* Génération des tables CSV */

    if (commande.is_generer_table())
    {
        cerr << ENDL << PROCESSING_HTML_TAG "Exportation des bases de données au format CSV..." << ENDL ENDL;
        boucle_ecriture(gestionnaire_fils.Info, rang_segment);
    }

    /* Signal de fin de barre de progression */

    generate_rank_signal(-1);
    cerr << " \n";

    /* Libération de la mémoire
     * En cas de problème d'allocation mémoire le mieux est encore de ne pas désallouer car on ne connait pas exacteemnt l'état
     * de la mémoire dynamique */

    if (! commande.is_liberer_memoire()) return 0;

    liberer_memoire(commande, gestionnaire_fils);

    return 1;
}

void Analyseur::liberer_memoire(Commandline& commande, thread_handler& t)
{

if (verbeux)
    cerr << ENDL
         << PROCESSING_HTML_TAG "Libération de la mémoire..."
         << ENDL;

    for (int i = 0; i < commande.get_nb_fil(); ++i)
    {
        for (unsigned agent = 0; agent < t.Info.at(i).NCumAgent; ++agent)
        {
            for (int j = 0; j < t.Info.at(i).Memoire_p_ligne[agent]; ++j)
            {
                if (j != Categorie && xmlStrcmp(t.Info.at(i).Table[agent][j], (const xmlChar*) "") != 0)
                    xmlFree(t.Info[i].Table[agent][j]);
            }
        }
    }
}

