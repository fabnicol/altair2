/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */

/* Constantes de compilation pouvant être redéfinies : NA_STRING, MAX_LIGNES_PAYE, MAX_NB_AGENTS, NO_DEBUG, MAX_MEMOIRE_RESERVEE */


#ifdef __cplusplus
extern "C" {
#endif
#include "validator.hpp"
#include "fonctions_auxiliaires.hpp"
#include "table.hpp"

static inline xmlNodePtr atteindreNoeud(const char* noeud, xmlNodePtr cur)
{
    while (cur && xmlIsBlankNode(cur))
    {
        cur = cur -> next;
    }

    while (cur != NULL)
    {
        if (xmlStrcmp(cur->name, (const xmlChar *) noeud))
        {
            cur = cur->next;
        }
        else
        {
            AFFICHER_NOEUD(noeud)
            break;
        }
    }
    return cur;
}

static inline void  verifier_taille(const int l)
{
    if (l == (MAX_LIGNES_PAYE - 6))
    {
        fprintf(stderr, "En excès du nombre de lignes de paye autorisé (%d)\n", MAX_LIGNES_PAYE);
        exit(-10);
    }
}

/* obligatoire */

#define ligne_l  info->Table[info->NCumAgentLibxml2][l]

static inline bool Bulletin(const char* tag, xmlNodePtr* cur, int l, info_t* info)
{
    bool test = (cur != NULL && *cur != NULL && (! xmlStrcmp((*cur)->name,  (const xmlChar*) tag)));

    if (test)
    {
        ligne_l = xmlGetProp(*cur, (const xmlChar *) "V");

        if (ligne_l == NULL) ligne_l = (xmlChar*) "NA"; //ligne_l = xmlCharStrdup("NA");

        /* sanitisation */

        for (int i = 0; i < xmlStrlen(ligne_l); i++)
            if (ligne_l[i] == info->separateur) ligne_l[i] = '_';

        *cur = (*cur)? (*cur)->next: NULL;
    }
    return test;
}

static inline void _Bulletin(const char* tag, xmlNodePtr* cur,  int l, info_t* info)
{
    if (! Bulletin(tag, cur, l, info))
    {
        if (*cur)
            fprintf(stderr, "Trouvé %s au lieu de %s \n", (*cur)->name, tag);
        else
            fprintf(stderr, "Noeud courant null au stade de la vérification de %s\n", tag);

        exit(-32);
    }
}

static inline void substituer_separateur_decimal(xmlChar* ligne, const char decimal)
{
    for (int i = 0; i < xmlStrlen(ligne); i++)
        if (ligne[i] == '.') ligne[i] = decimal;
}

/* optionnel */

static inline void _Bulletin_(const char* tag, xmlNodePtr* cur,  int l, info_t* info)
{
    if (! Bulletin(tag, cur, l,  info))
    {
        //(xmlChar*) malloc(16*sizeof(xmlChar)); //3

        ligne_l = (xmlChar*) "NA";

        //if ((ligne_l = xmlCharStrdup("NA")) == NULL)
//        {
//            perror("Erreur d'allocation de drapeau II");
//            exit(-64);
//        }
        //bulletinIdent->ligne[l][0] = 'N';
        //bulletinIdent->ligne[l][1] = 'A';
        return;
    }

    if (info->decimal != '.') substituer_separateur_decimal(ligne_l, info->decimal);
}

/* obligatoire et avec substitution séparateur décimal */

static inline void Bulletin_(const char* tag, xmlNodePtr* cur, int l, info_t* info)
{
    _Bulletin(tag, cur, l, info) ;
    if (info->decimal != '.')  substituer_separateur_decimal(ligne_l, info->decimal);
}

static inline int lignePaye(xmlNodePtr cur, info_t* info)
{
    int l = info->besoin_memoire_par_ligne;

    int nbLignePaye = 0;
    unsigned int t = 0;
    /* Besoins en mémoire : 18 [champs hors ligne] + nombre de lignes + flags (maximum nbType) */
    while (cur != NULL)
    {
        bool new_type = false;

        while (xmlStrcmp(cur->name, (const xmlChar *) type_remuneration[t]))
        {
            t++;
            if (t == nbType)
            {
                fprintf(stderr, "En excès du nombre de types de lignes de paye autorisé (%d)\n", nbType);
                if (cur) fprintf(stderr, "Type litigieux %s aux alentours du matricule %s \n",
                                     cur->name,
                                     info->Table[info->NCumAgentLibxml2][Matricule]);
                else fprintf(stderr, "%s", "Pointeur noeud courant nul\n");
                exit(-11);
            }

            new_type = true;
        }

        if (new_type && t > 0 && t < nbType)
        {
            //(xmlChar*) malloc(16*sizeof(xmlChar)); // 2
            ligne_l = (xmlChar*) drapeau[t];  // +1 pour éviter la confusion avec \0 des chaines vides
            l++;
        }

        // verifier_taille(l);

        if (! xmlStrcmp(cur->name, (const xmlChar*) "Commentaire"))
        {
            cur=cur->next;
            continue;
        }

        DESCENDRE_UN_NIVEAU
        /* Libellé, obligatoire */

        cur = atteindreNoeud("Libelle", cur);

        _Bulletin("Libelle", &cur, l, info);
        l++;
        /* Code, obligatoire */
        cur = atteindreNoeud("Code", cur);
        _Bulletin("Code", &cur, l, info);

        //if (xmlStrcmp(bulletinIdent->ligne[l], (xmlChar*)NA_STRING))
        nbLignePaye++;
        l++;

        /* Base, si elle existe */

        _Bulletin_("Base", &cur,  l, info);
        l++;

        /* Taux, s'il existe */
        _Bulletin_("Taux", &cur, l, info);
        l++;

        /* Nombre d'unités, s'il existe */
        _Bulletin_("NbUnite", &cur, l, info);
        l++;

        /* Montant , obligatoire */
        cur = atteindreNoeud("Mt", cur);

        Bulletin_("Mt", &cur, l, info);
        l++;

        REMONTER_UN_NIVEAU

        // Lorsque on a épuisé tous les types licites on a nécessairement cur = NULL
    }

#undef ligne_l

    return nbLignePaye;
}

#define _BULLETIN(X) _Bulletin(#X, &cur, X, info);

#define BULLETIN_(X)  Bulletin_(#X, &cur, X, info);

#define _BULLETIN_(X)  _Bulletin_(#X, &cur, X, info);

static uint64_t  parseBulletin(xmlNodePtr cur, info_t* info)
{
    DEBUG("Parsage")
    int ligne = 0;

    if (cur == NULL) return 0;
    cur = atteindreNoeud("Agent", cur);

    if (cur == NULL)
    {
        fprintf(stderr, "%s\n", "Impossible d'atteindre \"Agent\"");
        return 0;
    }

#define bulletinIdent info->Table[info->NCumAgentLibxml2]

    bulletinIdent[Annee] = info->annee_fichier;
    bulletinIdent[Mois]  = info->mois_fichier;

    DESCENDRE_UN_NIVEAU

    cur = (cur)? cur->next : NULL;
    _BULLETIN(Nom)

    cur = cur->parent;
    cur = cur->xmlChildrenNode;
    xmlNodePtr cur_save = cur;
    cur = atteindreNoeud("Prenom", cur);

    _BULLETIN(Prenom)
    _BULLETIN(Matricule)
    _BULLETIN(NIR)

    cur = cur_save;
    cur = atteindreNoeud("Statut", cur);
    _BULLETIN(Statut)

    cur = cur_save;
    cur = atteindreNoeud("EmploiMetier", cur);
    _BULLETIN(EmploiMetier)

    cur = cur_save;
    cur = atteindreNoeud("Grade", cur);
    _BULLETIN(Grade)

    cur = cur_save;
    cur = atteindreNoeud("Indice", cur);
    _BULLETIN(Indice)

    REMONTER_UN_NIVEAU

    cur = atteindreNoeud("Service", cur);
    _BULLETIN(Service)

    cur = cur->parent;
    cur = cur->xmlChildrenNode;
    cur_save = cur;
    cur = atteindreNoeud("NBI", cur);

    _BULLETIN(NBI)

    cur = cur_save;
    cur = atteindreNoeud("QuotiteTrav", cur);

    /* obligatoire, substitution du sparateur décimal */
    BULLETIN_(QuotiteTrav)

    cur = atteindreNoeud("Remuneration", cur);

    if (cur)
    {
        xmlNodePtr cur_save = cur;

        if (xmlChildElementCount(cur))
        {
            DESCENDRE_UN_NIVEAU
            ligne = lignePaye(cur, info);
        }
        else
        {
            // Rémuneration tag vide
            ligne = 1 ;
            for (int k=0; k < 6; k++) bulletinIdent[info->besoin_memoire_par_ligne + k]=(xmlChar*) "NA";
        }

        cur = cur_save->next;
    }
    else
    {
        perror("Rémunération introuvable.");
        exit(-4);
    }

    /* non obligatoire , substitution du sparateur décimal */

    _BULLETIN_(NbHeureTotal)
    cur = atteindreNoeud("NbHeureSup", cur);

    /* obligatoire, substitution du sparateur décimal */
    BULLETIN_(NbHeureSup)
    BULLETIN_(MtBrut)
    BULLETIN_(MtNet)
    BULLETIN_(MtNetAPayer)

    return ligne;
}


#undef bulletinIdent
/* agent_total est une variable de contrôle pour info->NCumAgent */

static void parseFile(info_t* info)
{
    xmlDocPtr doc;
    xmlNodePtr cur = NULL;
    uint16_t agent_du_fichier = 0;

    doc = xmlParseFile(info->threads->argv[info->fichier_courant]);

    if (doc == NULL) return;

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL)
    {
        fprintf(stderr,"document vide\n");
        xmlFreeDoc(doc);
    }

    DESCENDRE_UN_NIVEAU

    cur = atteindreNoeud("Annee", cur);

    if (cur != NULL)
    {
        info->annee_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        cur = (cur)? cur->next : NULL;
    }
    else
    {
        fprintf(stderr, "%s\n", "Année non détectable");
        exit(-502);
    }

    if (cur != NULL)
    {
        info->mois_fichier = xmlGetProp(cur, (const xmlChar *) "V");
    }
    else
    {
        fprintf(stderr, "%s\n", "Mois non détectable");
        exit(-503);
    }

    cur = atteindreNoeud("DonneesIndiv", cur);

    while(! xmlStrcmp(cur->name, (const xmlChar*) "DonneesIndiv"))
    {
        xmlNodePtr cur_save = cur;

        DESCENDRE_UN_NIVEAU

        while(cur != NULL)
        {
#if !NO_DEBUG
            char msg[50] = { 0};
            sprintf(msg, "Paye n°%d\n", agent_du_fichier);

            DEBUG(msg);

            //sleep(1000);
#endif

            cur = atteindreNoeud("PayeIndivMensuel", cur);
            xmlNodePtr cur_save =  cur;

            DESCENDRE_UN_NIVEAU

            int32_t ligne_p=parseBulletin(cur, info);

            if (info->reduire_consommation_memoire)
            {
                if (ligne_p != info->NLigne[info->NCumAgentLibxml2] && info->chemin_log == NULL)
                {
                    fprintf(stderr, "Incohérence des décomptes de lignes entre le contrôle C : %d et l'analyse Libxml2 : %d\nPour l'agent %s, Année %s Mois %s\n", info->NLigne[info->NCumAgentLibxml2], ligne_p, info->Table[info->NCumAgentLibxml2][Matricule], info->Table[info->NCumAgentLibxml2][Annee], info->Table[info->NCumAgentLibxml2][Mois]);
                    exit(-1278);
                }
            }
            else info->NLigne[info->NCumAgentLibxml2] = ligne_p;

            info->nbLigne += ligne_p;

            if (info->chemin_log)
            {
                FILE* log=fopen(info->chemin_log, "a+");
                int diff = info->NLigne[info->NCumAgentLibxml2]-ligne_p;
                if (!diff)
                {
                    fprintf(log, "Année %s Mois %s Matricule %s Rang global agent %d Rang agent du fichier %d Analyseur C, ligne n° %d Analyseur Xml, ligne n° %d Différence %d\n",
                            info->annee_fichier,
                            info->mois_fichier,
                            info->Table[info->NCumAgentLibxml2][Matricule],
                            info->NCumAgentLibxml2,
                            agent_du_fichier,
                            info->NLigne[info->NCumAgentLibxml2],
                            ligne_p,
                            diff);
                }

                fclose(log);
            }
            // Ici il est normal que cur = NULL

            cur = cur_save->next;

            AFFICHER_NOEUD(cur->name)

            agent_du_fichier++;
            info->NCumAgentLibxml2++;
        }
        cur = cur_save->next;
    }

    printf("Fichier n°%d:\nPopulation du fichier  %s :\n %4d bulletins    Total : %4d bulletins  %4" PRIu64 " lignes cumulées.\n",
           info->fichier_courant,
           info->threads->argv[info->fichier_courant],
           agent_du_fichier, info->NCumAgentLibxml2, info->nbLigne);

     xmlFreeDoc(doc);

    // xmlCleanupParser();
}


void* decoder_fichier(void* tinfo)
{
    /* environ 6000 bulletins par seconde en processus sumple, et 15000 en multithread ; rajoute 1/3 du temps */
    info_t* info = (info_t*) tinfo;

    xmlKeepBlanksDefault(0);

    if (info->reduire_consommation_memoire)
    {
        int err = calculer_memoire_requise(info);
        if (err)
        {
            perror("Calcul de la mémoire requise");
            exit(-1001);
        }
    }
    else
    {
        info->NCumAgent = info->nbAgentUtilisateur * info->threads->argc;
        info->NLigne = (uint16_t*) calloc(info->threads->argc, info->nbAgentUtilisateur* sizeof(uint16_t));
        info->NAgent = (int32_t*) calloc(info->threads->argc, sizeof(int32_t));
    }


    info->Table = (xmlChar***) malloc(info->NCumAgent *  sizeof(xmlChar**));

    if (info->Table == NULL)
    {
        perror("Mémoire insuffisante");
        exit(-18);
    }

    for (unsigned agent = 0; agent < info->NCumAgent; agent++)
    {
        info->Table[agent] = (xmlChar**) calloc(((info->reduire_consommation_memoire)?
                                                info->besoin_memoire_par_ligne + nbType + (info->NLigne[agent])*6
                                                : info->besoin_memoire_par_ligne + nbType + MAX_LIGNES_PAYE*6), sizeof(xmlChar*));
        if (info->Table[agent] == NULL)
        {
            perror("Erreur d'allocation de drapeau I.");
            exit(-63);
        }
    }

    for (int i = 0; i < info->threads->argc ; i++)
    {
        info->fichier_courant = i;

        parseFile(info);
    }

    return NULL;
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
    bool afficher_memoire_reservee = false;
    bool generer_table = false;
    bool liberer_memoire = true;
    int nbfil = 0;

    thread_t mon_thread;
    info_t info =
    {
        NULL,             //    bulletinPtr* Table;
        0,                //    uint64_t nbLigne;
        NULL,             //    int32_t  *NAgent;
        0,          //
        0,      //    uint32_t NCumAgent;
        0,                //    uint32_t agent courant
        NULL,             //    uint16_t *NLigne;
        0,                //    uint16_t fichier_courant;
        &mon_thread,      //    thread_t threads;
        NULL,             //    xmlChar*  annee_fichier;
        NULL,             //    xmlChar* mois_fichier;
        NULL,             //    char** fichiers;
        '.',          //    const char decimal;
        ',',       //    const char separateur;
        true,  //bool
        BESOIN_MEMOIRE_ENTETE,// besoin mémoire minimum hors lecture de lignes : devra être incréméenté,
        NULL

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
            printf("%s\n", "-n nombre de bulletins mensuels attendus [calcul exact par défaut]");
            printf("%s\n", "-t argument optionnel : type de base en sortie, soit 'standard', soit 'bulletins' [défaut bulletins].");
            printf("%s\n", "-o argument obligatoire : fichier.csv, chemin complet du fichier de sortie [défaut 'Table.csv' avec -t].");
            printf("%s\n", "-D argument obligatoire : répertoire complet du fichier de sortie [défaut '.' avec -t].");
            printf("%s\n", "-d argument obligatoire : séparateur décimal [défaut . avec -t].");
            printf("%s\n", "-s argument obligatoire : séparateur de champs [défaut , avec -t]/");
            printf("%s\n", "-m sans argument : mémoire réservée. Estimation de la consommation de mémoire.");
            printf("%s\n", "-j argument obligatoire : nombre de fils d'exécution (maximum 10).");
            printf("%s\n", "-M sans argument : ne pas libérer la mémoire réservée en fin de programme.");
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
        else if (! strcmp(argv[start], "-m"))
        {
            afficher_memoire_reservee = true;
            start++;
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
            info.chemin_log = (char*) calloc(500, sizeof(char));
            if (info.chemin_log == NULL)
            {
                perror("Chemin log");
                exit(-853);
            }
            strncpy(info.chemin_log, argv[start + 1], 500*sizeof(char));
            if (NULL == fopen(info.chemin_log, "w"))
            {
                perror("Le log ne peut être créé, vérifier l'existence du dossier.");
                exit(-114);
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

//    if (memoire_reservee > MAX_MEMOIRE_RESERVEE)
//    {
//        fprintf(stderr, "Quantité de mémoire réservée %" PRIu64 " supérieure au maximum de %" PRIu64 " octets.\nAppliquer le programme sur une partie des fichiers et fusionner les bases en résultant.\n", memoire_reservee, MAX_MEMOIRE_RESERVEE);
//        exit(-500);
//    }

//    if (afficher_memoire_reservee) fprintf(stderr, "Quantité de mémoire réservée %" PRIu64 " octets.\n", memoire_reservee);
        xmlInitParser();
    if (nbfil == 0 || (argc -start < 2))
    {

        info.threads->argc = argc -start;
        info.threads->argv = (char**) malloc((argc -start)* sizeof(char*));
        for (int j = start; j < argc; j++) info.threads->argv[j-start] = strdup(argv[j]);

        decoder_fichier((void*) &info);

        if (info.reduire_consommation_memoire && info.NCumAgent != info.NCumAgentLibxml2)
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

        info_t* Info = (info_t* ) malloc(nbfil*sizeof(info_t));
        if (Info == NULL)
        {
            perror("Allocation de info");
            exit(-144);
        }

        puts("Creation des fils clients.\n");

        for (int i = 0; i < nbfil; i++)
        {
            //Info[i] = info; // initialisation d'ensemble

            Info[i].nbAgentUtilisateur = info.nbAgentUtilisateur;
            Info[i].decimal = info.decimal;
            Info[i].separateur = info.separateur;
            Info[i].reduire_consommation_memoire = info.reduire_consommation_memoire;
            Info[i].besoin_memoire_par_ligne = info.besoin_memoire_par_ligne;
            if (info.chemin_log) Info[i].chemin_log = strdup(info.chemin_log);
            Info[i].threads = (thread_t*) malloc(sizeof(thread_t));
            if (Info[i].threads == NULL)
            {
                perror("Allocation de threads");
                exit(-145);
            }
            Info[i].threads->thread_num = i;
            Info[i].threads->argc = (argc - start < nbfichier_par_fil)? argc - start: nbfichier_par_fil;

            printf("\nThread i=%d/%d Info[i].threads->argc=%d\n", i, nbfil, Info[i].threads->argc);

            Info[i].threads->argv = (char**) malloc(nbfichier_par_fil * sizeof(char*));
            if (Info[i].threads->argv == NULL)
            {
                perror("Allocation de threads");
                exit(-145);
            }

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

        info.NCumAgent = 0;
        info.NCumAgentLibxml2 = 0;
        info.nbLigne = 0;
        int agent_cumul = 0;

        for (int i = 0; i < nbfil; i++)
        {
            info.NCumAgentLibxml2 += Info[i].NCumAgentLibxml2;
            info.NCumAgent += Info[i].NCumAgent;
            info.nbLigne   += Info[i].nbLigne;
        }

        info.NLigne = (uint16_t*) calloc(info.NCumAgentLibxml2, sizeof(uint16_t));
        info.Table = (xmlChar***) malloc(info.NCumAgentLibxml2*sizeof(xmlChar**));

       for (int i = 0; i < nbfil; i++)
       {
        for(unsigned agent = 0; agent < Info[i].NCumAgentLibxml2; agent++)
            {
                info.NLigne[agent_cumul] = Info[i].NLigne[agent];
                info.Table[agent_cumul] =  (xmlChar**) malloc(Info[i].NLigne[agent]*sizeof(xmlChar*));
                for (int l=0; l < Info[i].NLigne[agent]; l++)
                {
                    info.Table[agent_cumul][l] = Info[i].Table[agent][l];
                }
                agent_cumul++;
            }
       }

    }

xmlCleanupParser();

    uint64_t nbLigneBase=0;

    if (generer_table)
    {

        if (! strcmp(type_table, "standard"))
            nbLigneBase = generer_table_standard(chemin_table, &info);
        else if (! strcmp(type_table, "bulletins"))
            nbLigneBase = generer_table_bulletins(chemin_table, &info);
        else
        {
            fprintf(stderr, "Type %s inconnu.", type_table);
            exit(-501);
        }
    }

    fprintf(stderr, "Table de %" PRIu64 " lignes générée pour %" PRIu64 "lignes de paie d'origine.\n", nbLigneBase, info.nbLigne);

#define FREE(X) if (X && xmlStrcmp(X, (xmlChar*)NA_STRING)) xmlFree(X);

    /* libération de la mémoire */
// calculer nbAgent à partir de info->NCumAgent...
    if (liberer_memoire)
        for (unsigned i = 0; i < info.nbAgentUtilisateur; i++)
        {

//        FREE(Table[i]->Nom)
//        FREE(Table[i]->Prenom)
//        FREE(Table[i]->Matricule)
//        FREE(Table[i]->Annee)
//        FREE(Table[i]->Mois)
//        FREE(Table[i]->NIR)
//        FREE(Table[i]->Statut)
//        FREE(Table[i]->EmploiMetier)
//        FREE(Table[i]->Grade)
//        FREE(Table[i]->Indice)
//        FREE(Table[i]->Service)
//        FREE(Table[i]->NBI)
//        FREE(Table[i]->QuotiteTrav)

//        FREE(Table[i]->NbHeureTotal)
//        FREE(Table[i]->NbHeureSup)
//        FREE(Table[i]->MtBrut)
//        FREE(Table[i]->MtNet)
//        FREE(Table[i]->MtNetAPayer)
//        for (int l = 0; l < nbType-1; l++)
//             for (int j = 0; j < MAX_LIGNES_PAYE; j++)
//                  for (int k = 0; k < 6; k++)
//                     FREE(Table[i]->ligne[l][j][k])

        }

    return 0;

}
#ifdef __cplusplus
}
#endif

