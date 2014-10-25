/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */


//#ifdef __cplusplus
//extern "C" {
//#endif

#include "validator.hpp"
#include "fonctions_auxiliaires.hpp"
#include "table.hpp"

inline const uint32_t* calculer_maxima(const info_t* Info)
{
    static int once;

    uint32_t* maximum= (uint32_t*) calloc(2, 4);

    if (once || maximum == NULL) return NULL;  // no-op)

    for (int i = 0; i < Info[0].nbfil; i++)
    {
        for (unsigned j = 0; j < Info[i].threads->argc; j++)
            if (Info[i].NAgent[j] > maximum[0])
               maximum[0] = Info[i].NAgent[j];

        for (uint32_t agent=0; agent < Info[i].NCumAgentXml; agent++)
            if (Info[i].NLigne[agent] > maximum[1])
                maximum[1] = Info[i].NLigne[agent];
    }

    once++;
    return(maximum);
}

int main(int argc, char **argv)
{

#ifdef _WIN32
    setlocale(LC_ALL, "French_France");  // Windows ne gère pas UTF-8 en locale
#else
    setlocale(LC_ALL, "fr_FR.UTF-8");
#endif

    if (argc < 2)
    {
        fprintf(stderr, "%s\n", "Il faut au moins un fichier à analyser.");
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
        0,                //    uint32_t NCumAgentXml;
        NULL,             //    uint16_t *NLigne;
        &mon_thread,      //    thread_t threads;
        NULL,             //    chemin log
        (char*) strdup(EXPRESSION_REG_ELUS),
        MAX_LIGNES_PAYE,  // nbLigneUtilisateur
        0,                //    uint16_t fichier_courant
        '.',              //    const char decimal;
        ',',              //    const char separateur;
        true,             // réduire coso mémoire
        true,             // par défaut lire la balise adjacente
        false,             // calculer les maxima de lignes et d'agents
        BESOIN_MEMOIRE_ENTETE,// besoin mémoire minimum hors lecture de lignes : devra être incréméenté,
        1                 // nbfil
    };

    while (start < argc)
    {
        if (! strcmp(argv[start], "-n"))
        {
            info.reduire_consommation_memoire = false;
            if ((info.nbAgentUtilisateur = lire_argument(argc, argv[start + 1])) < 1)
            {
                fprintf(stderr, "%s\n", "Préciser le nombre de bulletins mensuels attendus (majorant du nombre).");
                exit(-1);
            }
            start += 2;
            continue;

        }
        else if (! strcmp(argv[start], "-h"))
        {
            printf("%s\n", "Usage :  xhl2csv OPTIONS fichiers.xhl");
            puts("OPTIONS :");
            printf("%s\n", "-n nombre maximum de bulletins mensuels attendus [calcul exact par défaut]");
            printf("%s\n", "-N nombre maximum de lignes de paye attendues [calcul exact par défaut]");
            printf("%s\n", "-t argument optionnel : type de base en sortie, soit 'standard', soit 'bulletins' [défaut bulletins].");
            printf("%s\n", "-o argument obligatoire : fichier.csv, chemin complet du fichier de sortie [défaut 'Table.csv' avec -t].");
            printf("%s\n", "-D argument obligatoire : répertoire complet du fichier de sortie [défaut '.' avec -t].");
            printf("%s\n", "-d argument obligatoire : séparateur décimal [défaut . avec -t].");
            printf("%s\n", "-s argument obligatoire : séparateur de champs [défaut , avec -t]/");
            printf("%s\n", "-j argument obligatoire : nombre de fils d'exécution (maximum 10).");
            printf("%s\n", "-M sans argument : ne pas libérer la mémoire réservée en fin de programme.");
            printf("%s\n", "-m sans argument : calculer les maxima d'agents et de lignes de paye.");
            printf("%s\n", "-L argument obligatoire : chemin du log d'exécution du test de cohérence entre analyseurs C et XML.");
            printf("%s\n", "-R argument obligatoire : expression régulière pour la recherche des élus (codés : ELU dans le champ Statut.");
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
                fprintf(stderr, "%s\n", "Option -s suivi d'un argument obligatoire (séparateur de champs).");
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
                fprintf(stderr, "%s\n", "Option -d suivi d'un argument obligatoire (séparateur décimal).");
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
                perror("La base de données ne peut être créée, vérifier l'existence du dossier.");
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
        else if (! strcmp(argv[start], "-m"))
        {
            info.calculer_maxima = true;
            start++;
            continue;
        }
        else if (! strcmp(argv[start], "-D"))
        {
            snprintf(chemin_table, 500*sizeof(char), "%s/Table.csv", argv[start + 1]);
            if (NULL == fopen(chemin_table, "w"))
            {
                perror("La base de données ne peut être créée, vérifier l'existence du dossier.");
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
                    perror("Le nombre de fils d'exécution doit être compris entre 2 et 10.");
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
                perror("Le log ne peut être créé, vérifier l'existence du dossier.");
                exit(-114);
            }
            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-N"))
        {
            if ((info.nbLigneUtilisateur = lire_argument(argc, argv[start +1])) > 1)
            {
               fprintf(stderr, "Nombre maximum de lignes de paye redéfini à %d\n", info.nbLigneUtilisateur);
            }

            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-R"))
        {
            if (argc > start +2)
            {
                free(info.expression_reg_elus);
                info.expression_reg_elus = strdup(argv[start + 1]);
            }
            else
            {
                perror("Il manque l'expression régulière.");
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
    printf("Besoin de mémoire requise minimum par bulletin : %d x sizeof(xmlChar*)\n", info.minimum_memoire_p_ligne);

    if (nbfil == 0 || (argc -start < 2))
    {
        info.nbfil=1;
        info.threads->argc = argc -start;
        info.threads->argv = (char**) malloc((argc -start)* sizeof(char*));
        for (int j = start; j < argc; j++) info.threads->argv[j-start] = argv[j];

        Info =&info;

        decoder_fichier((void*) Info);

        if (info.reduire_consommation_memoire && info.NCumAgent != info.NCumAgentXml)
        {
            fprintf(stderr, "%s\n", "Incohérence des cumuls de nombre d'agents");
            exit(-123);
        }
        else
        {
            fprintf(stderr, "%s\n", "Cohérence des cumuls de nombre d'agents vérifiée.");
        }


    }
    else
    {
        int nbfichier_par_fil = floor((argc - start) / nbfil);
        if (nbfichier_par_fil == 0)
        {
            fprintf(stderr, "%s\n", "Trop de fils pour le nombre de fichiers ; exécution avec -j 2");
            nbfil = 2;
        }

        if ((argc - start) % nbfil) nbfil++;  // on en crée un de plus pour le reste

        pthread_t thread_clients[nbfil];

        // Allocation dynamique nécessaire (à expliquer)

        Info = (info_t* ) malloc(nbfil*sizeof(info_t));
        if (Info == NULL)
        {
            perror("Allocation de info");
            exit(-144);
        }

        puts("Creation des fils clients.\n");

        for (int i = 0; i < nbfil; i++)
        {

            Info[i].nbLigne = 0;
            Info[i].NAgent = NULL;
            Info[i].nbAgentUtilisateur = info.nbAgentUtilisateur;
            Info[i].NCumAgent = 0;
            Info[i].NCumAgentXml = 0;
            Info[i].NLigne = NULL;

            Info[i].threads = (thread_t *) malloc(sizeof(thread_t));
            Info[i].threads->thread_num = i;
            Info[i].threads->argc = (argc - start < nbfichier_par_fil)? argc - start: nbfichier_par_fil;

            if (info.chemin_log)
            {
                Info[i].chemin_log = strdup(info.chemin_log);
            }
            if (info.expression_reg_elus)
            {
                Info[i].expression_reg_elus = strdup(info.expression_reg_elus);
            }
            //thread_t thr;

            Info[i].nbLigneUtilisateur = info.nbLigneUtilisateur;
            Info[i].fichier_courant = 0;
            Info[i].decimal = info.decimal;
            Info[i].separateur = info.separateur;
            Info[i].reduire_consommation_memoire = info.reduire_consommation_memoire;
            Info[i].drapeau_cont = true;
            Info[i].calculer_maxima = info.calculer_maxima;
            Info[i].minimum_memoire_p_ligne = info.minimum_memoire_p_ligne;
            Info[i].nbfil = nbfil;

            Info[i].threads->argv = (char**) malloc(nbfichier_par_fil * sizeof(char*));
            if (Info[i].threads->argv == NULL)
            {
                perror("Allocation de threads");
                exit(-145);
            }
            printf("Thread i=%d/%d Nombre de fichiers : %d\n", i+1, nbfil, Info[i].threads->argc);
            for (int j = 0; j <  nbfichier_par_fil && start + j < argc; j++)
            {
                Info[i].threads->argv[j] = strdup(argv[start + j]);
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

        free(info.chemin_log);
        free(info.expression_reg_elus);
    }

    const uint32_t*   maxima = NULL;

    if (Info[0].calculer_maxima)
    {
      maxima = calculer_maxima(Info);
      if (maxima)
      {
        printf("\nMaximum N.lignes Analyseur : %d  \n", maxima[0]);
        printf("\nMaximum N.agents Analyseur : %d  \n", maxima[1]);
      }
    }

    if (Info[0].chemin_log)
    {
      maxima = calculer_maxima(Info);
      FILE* LOG = fopen(Info[0].chemin_log, "a");
      if (LOG && maxima)
        {
            fprintf(LOG, "\nMaximum N.lignes Analyseur : %d  \n", maxima[0]);
            fprintf(LOG, "\nMaximum N.agents Analyseur : %d  \n", maxima[1]);
            fclose(LOG);
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
        fprintf(stderr, "Table de %" PRIu64 " lignes générée pour %" PRIu64 " lignes de paie d'origine.\n", nbLigneBase, Info[0].nbLigne);
    }

    /* libération de la mémoire */

    if (! liberer_memoire) return 0;

    for (int i = 0; i < Info[0].nbfil; i++)
    {
        for (unsigned agent = 0; agent < Info[i].NCumAgent; agent++)
        {
            int utilisation_memoire = ((Info[i].reduire_consommation_memoire)?
                                         Info[i].minimum_memoire_p_ligne + nbType + Info[i].NLigne[agent]*6
                                       : Info[i].minimum_memoire_p_ligne + nbType + Info[i].nbLigneUtilisateur*6);

            for (int j = 0; j < utilisation_memoire; j++)
                if (Info[i].Table[agent][j])
                    xmlFree(Info[i].Table[agent][j]);

            xmlFree(Info[i].Table[agent]);
        }

        free(Info[i].NLigne);
        free(Info[i].NAgent);
        free(Info[i].threads->argv);

        xmlFree(Info[i].Table);

        if (Info[i].chemin_log)
            xmlFree(Info[i].chemin_log);
        if (Info[i].expression_reg_elus)
            xmlFree(Info[i].expression_reg_elus);

        if (Info[0].nbfil > 1)
        {
            free(Info[i].threads);
        }
    }

    if (maxima) free((uint32_t*) maxima);

    if (Info[0].nbfil > 1) free(Info);

    return 0;
}

//
//#ifdef __cplusplus
//}
//#endif // __cplusplus
