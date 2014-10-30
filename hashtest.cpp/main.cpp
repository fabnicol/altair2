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

        for (uint32_t agent = 0; agent < Info[i].NCumAgentXml; agent++)
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
    bool generer_table = false;
    bool liberer_memoire = true;

    char chemin_base[500]={0};
    sprintf(chemin_base, "%s%s", CHEMIN_BASE, CSV);

    thread_t mon_thread;

    info_t info =
    {
        NULL,             //    bulletinPtr* Table;
        0,                //    uint64_t nbLigne;
        NULL,             //    int32_t  *NAgent;
        0,                //    uint32_t nbAgentUtilisateur
        0,                //    uint32_t NCumAgent;
        0,                //    uint32_t NCumAgentXml;
        MONOLITHIQUE,                //    taille base : non limitée par défaut
        NULL,             //    uint16_t *NLigne;
        &mon_thread,      //    thread_t threads;
        NULL,             //    chemin log
        (char*) strdup(EXPRESSION_REG_ELUS),
        (char*) strdup(chemin_base),
        MAX_LIGNES_PAYE,  // nbLigneUtilisateur
        0,                //    uint16_t fichier_courant
        '.',              //    const char decimal;
        ',',              //    const char separateur;
        true,             // réduire coso mémoire
        true,             // par défaut lire la balise adjacente
        false,            // calculer les maxima de lignes et d'agents
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
            printf("%s\n", "-n argument obligatoire : nombre maximum de bulletins mensuels attendus [calcul exact par défaut]");
            printf("%s\n", "-N argument obligatoire : nombre maximum de lignes de paye attendues [calcul exact par défaut]");
            printf("%s\n", "-t argument optionnel   : type de base en sortie, soit 'standard', soit 'bulletins' [défaut bulletins].");
            printf("%s\n", "-T argument obligatoire : nombre de lignes maximum par base .csv [défaut illimité]. Au plus 999 tables seront générées.");
            printf("%s\n", "-T AN                    : générer une table par année");
            printf("%s\n", "-T A/AC/AV/C/D/I/IR/RE/S/T : générer une table pour chaque catégorie de ligne : \
                          A rémunérations diverse \n \
                          AC acompte \n \
                          AV avantage en nature \n \
                          C cotisation \n \
                          D déduction \n \
                          I indemnités \n \
                          IR indemnité de résidence \n \
                          RE retenue \n \
                          S supplément familial \n \
                          T traitement brut \n \
                          X toutes catégories\n");
            printf("%s\n", "-o argument obligatoire : fichier.csv, chemin complet du fichier de sortie [défaut 'Table.csv' avec -t].");
            printf("%s\n", "-D argument obligatoire : répertoire complet du fichier de sortie [défaut '.' avec -t].");
            printf("%s\n", "-d argument obligatoire : séparateur décimal [défaut . avec -t].");
            printf("%s\n", "-s argument obligatoire : séparateur de champs [défaut , avec -t]/");
            printf("%s\n", "-j argument obligatoire : nombre de fils d'exécution (1 à 10).");
            printf("%s\n", "-M sans argument        : ne pas libérer la mémoire réservée en fin de programme.");
            printf("%s\n", "-m sans argument        : calculer les maxima d'agents et de lignes de paye.");
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
        else if (! strcmp(argv[start], "-T"))
        {
           if (start + 1 == argc)
            {
                fprintf(stderr, "%s\n", "Option -T suivi d'un argument obligatoire (nombre de lignes).");
                exit(-100);
            }

            if (! strcmp(argv[start + 1], "AN"))
            {
                info.taille_base = PAR_ANNEE;
            }
            else
            if (! strcmp(argv[start + 1], "A"))
            {
                info.taille_base = PAR_REM_DIVERSES ;
            }
            else
            if (! strcmp(argv[start + 1], "AC"))
            {
                info.taille_base = PAR_ACOMPTE ;
            }
            else
            if (! strcmp(argv[start + 1], "AV"))
            {
                info.taille_base = PAR_AVANTAGE_NATURE ;
            }
            else
            if (! strcmp(argv[start + 1], "C"))
            {
                info.taille_base = PAR_COTISATION ;
            }
            else
            if (! strcmp(argv[start + 1], "D"))
            {
                info.taille_base = PAR_DEDUCTION ;
            }
            else
            if (! strcmp(argv[start + 1], "I"))
            {
                info.taille_base = PAR_INDEMNITE ;
            }
            else
            if (! strcmp(argv[start + 1], "IR"))
            {
                info.taille_base = PAR_INDEMNITE_RESIDENCE ;
            }
            else
            if (! strcmp(argv[start + 1], "RE"))
            {
                info.taille_base = PAR_RETENUE ;
            }
            else
            if (! strcmp(argv[start + 1], "S"))
            {
                info.taille_base = PAR_SFT ;
            }
            else
            if (! strcmp(argv[start + 1], "T"))
            {
                info.taille_base = PAR_TRAITEMENT ;
            }
            else
            if (! strcmp(argv[start + 1], "R"))
            {
                info.taille_base = PAR_RAPPEL ;
            }
            else
            if (! strcmp(argv[start + 1], "X"))
            {
                info.taille_base = TOUTES_CATEGORIES ;
            }
            else
            if ((info.taille_base = lire_argument(argc, argv[start +1])) < 0 || info.taille_base > INT32_MAX -1)
            {
                    perror("Le nombre de lignes doit être compris entre 0 et INT64_MAX");
                    exit(-908);
            }
            start += 2;
            continue;
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
            free(info.chemin_base);
            strncpy(info.chemin_base, argv[start + 1], 500*sizeof(char));
            if (NULL == fopen(info.chemin_base, "w"))
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
            snprintf(info.chemin_base, strlen(info.chemin_base), "%s/%s%s", argv[start + 1], CHEMIN_BASE, CSV);

            if (NULL == fopen(info.chemin_base, "w"))
            {
                perror("La base de données ne peut être créée, vérifier l'existence du dossier.");
                exit(-113);
            }

            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-j"))
        {
            if ((info.nbfil = lire_argument(argc, argv[start +1])) > 0)
            {
                if (info.nbfil > 10 || info.nbfil < 1)
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

    if (info.nbfil == 1 || (argc -start < 2))
    {
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
        int nbfichier_par_fil = floor((argc - start) / info.nbfil);
        if (nbfichier_par_fil == 0)
        {
            fprintf(stderr, "%s\n", "Trop de fils pour le nombre de fichiers ; exécution avec -j 2");
            info.nbfil = 2;
        }

        if ((argc - start) % info.nbfil) info.nbfil++;  // on en crée un de plus pour le reste

        pthread_t thread_clients[info.nbfil];

        // Allocation dynamique nécessaire (à expliquer)

        Info = (info_t* ) malloc(info.nbfil*sizeof(info_t));
        if (Info == NULL)
        {
            perror("Allocation de info");
            exit(-144);
        }

        puts("Creation des fils clients.\n");

        for (int i = 0; i < info.nbfil; i++)
        {

            Info[i].nbLigne = 0;
            Info[i].NAgent = NULL;
            Info[i].nbAgentUtilisateur = info.nbAgentUtilisateur;
            Info[i].NCumAgent = 0;
            Info[i].NCumAgentXml = 0;
            Info[i].taille_base = info.taille_base;
            Info[i].chemin_base = info.chemin_base;
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
            if (info.chemin_base)
            {
                Info[i].chemin_base = strdup(info.chemin_base);
            }
            Info[i].nbLigneUtilisateur = info.nbLigneUtilisateur;
            Info[i].fichier_courant = 0;
            Info[i].decimal = info.decimal;
            Info[i].separateur = info.separateur;
            Info[i].reduire_consommation_memoire = info.reduire_consommation_memoire;
            Info[i].drapeau_cont = true;
            Info[i].calculer_maxima = info.calculer_maxima;
            Info[i].minimum_memoire_p_ligne = info.minimum_memoire_p_ligne;
            Info[i].nbfil = info.nbfil;

            Info[i].threads->argv = (char**) malloc(nbfichier_par_fil * sizeof(char*));
            if (Info[i].threads->argv == NULL)
            {
                perror("Allocation de threads");
                exit(-145);
            }
            printf("Thread i=%d/%d Nombre de fichiers : %d\n", i+1, info.nbfil, Info[i].threads->argc);
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

        for (int i = 0; i < info.nbfil; i++)
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
        printf("\nMaximum de lignes : %d  \n", maxima[1]);
        printf("\nMaximum d'agents  : %d  \n\n", maxima[0]);
      }
    }

    if (Info[0].chemin_log)
    {
      if (maxima == NULL) maxima = calculer_maxima(Info);
      FILE* LOG = fopen(Info[0].chemin_log, "a");
      if (LOG && maxima)
        {
            fprintf(LOG, "\nMaximum de lignes : %d  \n", maxima[1]);
            fprintf(LOG, "\nMaximum d'agent   : %d  \n", maxima[0]);
            fclose(LOG);
        }
    }

    xmlCleanupParser();

    if (generer_table)
    {
        if (! strcmp(type_table, "standard"))
            generer_table_standard(Info[0].chemin_base, Info);
        else if (! strcmp(type_table, "bulletins"))
              boucle_ecriture(Info);
        else
        {
            fprintf(stderr, "Type %s inconnu.", type_table);
            exit(-501);
        }
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
            free(Info[i].chemin_log);
        if (Info[i].expression_reg_elus)
            free(Info[i].expression_reg_elus);
        if (Info[i].chemin_base)
            free(Info[i].chemin_base);

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
