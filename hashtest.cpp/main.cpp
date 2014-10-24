/*  Programme �crit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est �dit�, le pr�sent code doit �tre converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'�tre compil�.
 *  En entr�e d'Altair pr�ciser encodage.entr�e en conformit� avec l'encodage du pr�sent fichier, qui sera celui de la base g�n�r�e.
 */


//#ifdef __cplusplus
//extern "C" {
//#endif

#include "validator.hpp"
#include "fonctions_auxiliaires.hpp"
#include "table.hpp"

int main(int argc, char **argv)
{

#ifdef _WIN32
    setlocale(LC_ALL, "French_France");  // Windows ne g�re pas UTF-8 en locale
#else
    setlocale(LC_ALL, "fr_FR.UTF-8");
#endif

    if (argc < 2)
    {
        fprintf(stderr, "%s\n", "Il faut au moins un fichier � analyser.");
        return -2;
    }

    LIBXML_TEST_VERSION
    xmlKeepBlanksDefault(0);

    int start = 1;
    char type_table[50]= {0};
    strcpy(type_table, "bulletins");

    char chemin_table[500]= {0};
    strcpy(chemin_table, "Table.csv");
    bool generer_table = false;
    bool liberer_memoire = true;
    int nbfil = 0;

    thread_t mon_thread;
    info_t info =
    {
        NULL,             //    bulletinPtr* Table;
        0,                //    uint64_t nbLigne;
        NULL,             //    int32_t  *NAgent;
        0,                //    uint32_t nbAgentUtilisateur
        0,                //    uint32_t NCumAgent;
        0,                //    uint32_t NCumAgentLibxml2;
        NULL,             //    uint16_t *NLigne;
        &mon_thread,      //    thread_t threads;
        NULL,             //    chemin log
        (char*) EXPRESSION_REG_ELUS,
        0,                //    uint16_t fichier_courant
        '.',              //    const char decimal;
        ',',              //    const char separateur;
        true,             //bool
        true,             // par d�faut lire la balise adjacente
        BESOIN_MEMOIRE_ENTETE,// besoin m�moire minimum hors lecture de lignes : devra �tre incr�m�ent�,
        1                 // nbfil
    };

    while (start < argc)
    {
        if (! strcmp(argv[start], "-n"))
        {
            info.reduire_consommation_memoire = false;
            if ((info.nbAgentUtilisateur = lire_argument(argc, argv[start + 1])) < 1)
            {
                fprintf(stderr, "%s\n", "Pr�ciser le nombre de bulletins mensuels attendus (majorant du nombre).");
                exit(-1);
            }
            start += 2;
            continue;

        }
        else if (! strcmp(argv[start], "-h"))
        {
            printf("%s\n", "Usage :  xhl2csv OPTIONS fichiers.xhl");
            puts("OPTIONS :");
            printf("%s\n", "-n nombre de bulletins mensuels attendus [calcul exact par d�faut]");
            printf("%s\n", "-t argument optionnel : type de base en sortie, soit 'standard', soit 'bulletins' [d�faut bulletins].");
            printf("%s\n", "-o argument obligatoire : fichier.csv, chemin complet du fichier de sortie [d�faut 'Table.csv' avec -t].");
            printf("%s\n", "-D argument obligatoire : r�pertoire complet du fichier de sortie [d�faut '.' avec -t].");
            printf("%s\n", "-d argument obligatoire : s�parateur d�cimal [d�faut . avec -t].");
            printf("%s\n", "-s argument obligatoire : s�parateur de champs [d�faut , avec -t]/");
            printf("%s\n", "-j argument obligatoire : nombre de fils d'ex�cution (maximum 10).");
            printf("%s\n", "-M sans argument : ne pas lib�rer la m�moire r�serv�e en fin de programme.");
            printf("%s\n", "-R argument obligatoire : expression r�guli�re pour la recherche des �lus (cod�s : ELU dans le champ Statut.");
            exit(0);
        }
        else if (! strcmp(argv[start], "-t"))
        {
            generer_table = true;
            if (! strcmp(argv[start + 1], "standard"))
            {
                strncpy(type_table, argv[start + 1], 50*sizeof(char));
                start += 2;
                continue;
            }
            else
            {
                start++;
                continue;
            }
        }
        else if (! strcmp(argv[start], "-s"))
        {
            if (start + 1 == argc)
            {
                fprintf(stderr, "%s\n", "Option -s suivi d'un argument obligatoire (s�parateur de champs).");
                exit(-100);
            }
            info.separateur = argv[start + 1][0];

            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-d"))
        {
            if (start + 1 == argc)
            {
                fprintf(stderr, "%s\n", "Option -d suivi d'un argument obligatoire (s�parateur d�cimal).");
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
                fprintf(stderr, "%s\n", "Option -o suivi d'un argument obligatoire (nom de  fichier).");
                exit(-100);
            }
            strncpy(chemin_table, argv[start + 1], 500*sizeof(char));
            if (NULL == fopen(chemin_table, "w"))
            {
                perror("La base de donn�es ne peut �tre cr��e, v�rifier l'existence du dossier.");
                exit(-113);
            }
            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-M"))
        {
            liberer_memoire = false;
            start++;
            continue;
        }
        else if (! strcmp(argv[start], "-D"))
        {
            snprintf(chemin_table, 500*sizeof(char), "%s/Table.csv", argv[start + 1]);
            if (NULL == fopen(chemin_table, "w"))
            {
                perror("La base de donn�es ne peut �tre cr��e, v�rifier l'existence du dossier.");
                exit(-113);
            }

            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-j"))
        {
            if ((nbfil = lire_argument(argc, argv[start +1])) > 0)
            {
                if (nbfil > 10 || nbfil < 2)
                {
                    perror("Le nombre de fils d'ex�cution doit �tre compris entre 2 et 10.");
                    exit(-111);
                }
            }
            start += 2;
            continue;

        }
        else if (! strcmp(argv[start], "-L"))
        {
            if (argc > start +2) info.chemin_log = strdup(argv[start + 1]);
            if (NULL == fopen(info.chemin_log, "w"))
            {
                perror("Le log ne peut �tre cr��, v�rifier l'existence du dossier.");
                exit(-114);
            }
            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-R"))
        {
            if (argc > start +2) info.expression_reg_elus = strdup(argv[start + 1]);
            else
            {
                perror("Il manque l'expression r�guli�re.");
                exit(-115);
            }
            start += 2;
            continue;
        }
        else if (argv[start][0] == '-')
        {
            fprintf(stderr, "%s\n", "Option inconnue.");
            exit(-100);
        }
        else break;
    }

    xmlInitMemory();
    xmlInitParser();
    info_t* Info;
    printf("Besoin de m�moire requise minimum par bulletin : %d x sizeof(xmlChar*)\n", info.minimum_memoire_p_ligne);

    if (nbfil == 0 || (argc -start < 2))
    {
        info.nbfil=1;
        info.threads->argc = argc -start;
        info.threads->argv = (char**) malloc((argc -start)* sizeof(char*));
        for (int j = start; j < argc; j++) info.threads->argv[j-start] = argv[j];

        Info =&info;

        decoder_fichier((void*) Info);

        if (info.reduire_consommation_memoire && info.NCumAgent != info.NCumAgentLibxml2)
        {
            fprintf(stderr, "%s\n", "Incoh�rence des cumuls de nombre d'agents");
            exit(-123);
        }
        else
        {
            fprintf(stderr, "%s\n", "Coh�rence des cumuls de nombre d'agents v�rifi�e.");
        }


    }
    else
    {
        int nbfichier_par_fil = floor((argc - start) / nbfil);
        if (nbfichier_par_fil == 0)
        {
            fprintf(stderr, "%s\n", "Trop de fils pour le nombre de fichiers ; ex�cution avec -j 2");
            nbfil = 2;
        }

        if ((argc - start) % nbfil) nbfil++;  // on en cr�e un de plus pour le reste

        pthread_t thread_clients[nbfil];

        // Allocation dynamique n�cessaire (� expliquer)

        Info = (info_t* ) malloc(nbfil*sizeof(info_t));
        if (Info == NULL)
        {
            perror("Allocation de info");
            exit(-144);
        }

        puts("Creation des fils clients.\n");

        for (int i = 0; i < nbfil; i++)
        {
            Info[i].nbfil =nbfil;
            Info[i].nbAgentUtilisateur = info.nbAgentUtilisateur;
            Info[i].decimal = info.decimal;
            Info[i].separateur = info.separateur;
            Info[i].reduire_consommation_memoire = info.reduire_consommation_memoire;
            Info[i].minimum_memoire_p_ligne = info.minimum_memoire_p_ligne;
            if (info.chemin_log) Info[i].chemin_log = strdup(info.chemin_log);
            if (info.expression_reg_elus) Info[i].expression_reg_elus = strdup(info.expression_reg_elus);
            //thread_t thr;
            Info[i].threads = (thread_t *) malloc(sizeof(thread_t));
            Info[i].threads->thread_num = i;
            Info[i].threads->argc = (argc - start < nbfichier_par_fil)? argc - start: nbfichier_par_fil;

            printf("Thread i=%d/%d Nombre de fichiers : %d\n", i+1, nbfil, Info[i].threads->argc);

            Info[i].threads->argv = (char**) malloc(nbfichier_par_fil * sizeof(char*));
            if (Info[i].threads->argv == NULL)
            {
                perror("Allocation de threads");
                exit(-145);
            }

            for (int j = 0; j <  nbfichier_par_fil && start + j < argc; j++)
            {
                Info[i].threads->argv[j] = argv[start + j];
            }

            start += nbfichier_par_fil;

            int ret = pthread_create(
                          &thread_clients[i],
                          NULL,
                          decoder_fichier,
                          (void*) &Info[i]);

            if (ret)
            {
                fprintf (stderr, "%s", strerror(ret));
            }
        }

        for (int i = 0; i < nbfil; i++)
        {
            pthread_join (thread_clients[i], NULL);
        }
    }

    xmlCleanupParser();

    uint64_t nbLigneBase=0;

    if (generer_table)
    {
        if (! strcmp(type_table, "standard"))
            nbLigneBase = generer_table_standard(chemin_table, Info);
        else if (! strcmp(type_table, "bulletins"))
            nbLigneBase = generer_table_bulletins(chemin_table, Info);
        else
        {
            fprintf(stderr, "Type %s inconnu.", type_table);
            exit(-501);
        }
        fprintf(stderr, "Table de %" PRIu64 " lignes g�n�r�e pour %" PRIu64 "lignes de paie d'origine.\n", nbLigneBase, Info[0].nbLigne);
    }


    /* lib�ration de la m�moire */

    if (! liberer_memoire) return 0;

    for (int i = 0; i < Info[0].nbfil; i++)
    {
      for (unsigned agent = 0; agent < Info[i].NCumAgent; agent++)
      {

       int utilisation_memoire = ((Info[i].reduire_consommation_memoire)?
                                                  Info[i].minimum_memoire_p_ligne + nbType + (Info[i].NLigne[agent])*6
                                                : Info[i].minimum_memoire_p_ligne + nbType + MAX_LIGNES_PAYE*6);

       for (int j = 0; j < utilisation_memoire; j++)
         if (Info[i].Table[agent][j]) xmlFree(Info[i].Table[agent][j]);

       xmlFree(Info[i].Table[agent]);
      }
      free(Info[i].NLigne);
      free(Info[i].NAgent);
      free(Info[i].threads->argv);
      xmlFree(Info[i].Table);

      if (Info[i].chemin_log) xmlFree(Info[i].chemin_log);
      if (Info[i].expression_reg_elus) xmlFree(Info[i].expression_reg_elus);

      if (Info[0].nbfil > 1)
      {
        free(Info[i].threads);
      }

    }

    if (Info[0].nbfil > 1) free(Info);

    return 0;

}

//
//#ifdef __cplusplus
//}
//#endif // __cplusplus
