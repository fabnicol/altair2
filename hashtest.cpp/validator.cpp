/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
 */

/* Constantes de compilation pouvant être redéfinies : NA_STRING, MAX_LIGNES_PAYE, MAX_NB_AGENTS, NO_DEBUG, MAX_MEMOIRE_RESERVEE */


#ifdef __cplusplus
extern "C" {
#endif
#include "validator.hpp"

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



static inline bool Bulletin(const int l, const char* tag, const bulletinPtr bulletinIdent, xmlNodePtr* cur)
{
    bool test = (cur != NULL && *cur != NULL && (! xmlStrcmp((*cur)->name,  (const xmlChar*) tag)));
    if (test)
    {
        bulletinIdent->ligne[l] = xmlGetProp(*cur, (const xmlChar *) "V");
        if (bulletinIdent->ligne[l] == NULL) bulletinIdent->ligne[l] = (xmlChar*) "NA";
        xmlChar* s = bulletinIdent->ligne[l];

        /* sanitisation */

        for (int i = 0; i < xmlStrlen(s); i++)
            if (s[i] == ',') s[i] = '_';

        *cur = (*cur)? (*cur)->next: NULL;
    }
    return test;
}

static inline void _Bulletin(const int l, const char* tag, const bulletinPtr bulletinIdent, xmlNodePtr* cur)
{
    if (! Bulletin(l, tag, bulletinIdent, cur))
    {
        if (*cur)
            fprintf(stderr, "Trouvé %s au lieu de %s \n", (*cur)->name, tag);
        else
            fprintf(stderr, "Noeud courant null au stade de la vérification de %s pour le matricule %s \n", tag, bulletinIdent->ligne[Matricule]);

        exit(-32);

    }
}

static inline void substituer_separateur_decimal(const int l, const bulletinPtr bulletinIdent, const char decimal)
{
    xmlChar* s = bulletinIdent->ligne[l];
    for (int i = 0; i < xmlStrlen(s); i++)
        if (s[i] == '.') s[i] = decimal;
//    bulletinIdent->ligne[l] = s;
}

/* optionnel */

static inline void _Bulletin_(const int l , const char* tag, const bulletinPtr bulletinIdent, xmlNodePtr* cur, const char decimal)
{
    if (! Bulletin(l , tag, bulletinIdent, cur))
    {
        bulletinIdent->ligne[l] = xmlCharStrdup("NA");//(xmlChar*) malloc(16*sizeof(xmlChar)); //3
        if (bulletinIdent->ligne[l] == NULL)
        {
            perror("Erreur d'allocation de drapeau II.");
            exit(-64);
        }
        //bulletinIdent->ligne[l][0] = 'N';
        //bulletinIdent->ligne[l][1] = 'A';
        return;
    }
    substituer_separateur_decimal(l, bulletinIdent, decimal);
}

/* obligatoire et avec substitution séparateur décimal */

static inline void Bulletin_(const int l, const char* tag, const bulletinPtr bulletinIdent, xmlNodePtr* cur, const char decimal)
{
    _Bulletin(l, tag, bulletinIdent, cur) ;
    substituer_separateur_decimal(l, bulletinIdent, decimal);
}

static inline int lignePaye(xmlNodePtr cur, info_t* info)
{

  #define bulletinIdent  info->Table[info->agent_courant]

    int l = 18; //info->besoin_memoire_par_ligne;

    int nbLignePaye = 0;
    unsigned int t = 0;
    /* Besoins en mémoire : 18 [champs hors ligne] + nombre de lignes + flags (maximum nbType) */
    while (cur != NULL)
    {
        if (xmlStrcmp(cur->name, (const xmlChar *) type_remuneration[t]))
        {
            t++;
            bulletinIdent->ligne[l] = xmlCharStrdup("A");//(xmlChar*) malloc(16*sizeof(xmlChar)); // 2
            bulletinIdent->ligne[l][0] = t+1;  // +1 pour éviter la confusion avec \0 des chaines vides
            l++;
            if (t == nbType)
            {
                fprintf(stderr, "En excès du nombre de types de lignes de paye autorisé (%d)\n", nbType);
                if (cur) fprintf(stderr, "Type litigieux %s aux alentours du matricule %s \n", cur->name, bulletinIdent->ligne[Matricule]);
                else fprintf(stderr, "%s", "Pointeur noeud courant nul\n");
                exit(-11);
            }
            continue;
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

        _Bulletin(l, "Libelle", bulletinIdent, &cur);
        l++;
        /* Code, obligatoire */
        cur = atteindreNoeud("Code", cur);
        _Bulletin(l, "Code",  bulletinIdent, &cur);

        //if (xmlStrcmp(bulletinIdent->ligne[l], (xmlChar*)NA_STRING))
        nbLignePaye++;
        l++;

        /* Base, si elle existe */

        _Bulletin_(l, "Base", bulletinIdent, &cur, info->decimal);
        l++;

        /* Taux, s'il existe */
        _Bulletin_(l, "Taux", bulletinIdent, &cur, info->decimal);
        l++;

        /* Nombre d'unités, s'il existe */
        _Bulletin_(l, "NbUnite", bulletinIdent, &cur, info->decimal);
        l++;

        /* Montant , obligatoire */
        cur = atteindreNoeud("Mt", cur);

        Bulletin_(l, "Mt", bulletinIdent, &cur, info->decimal);
        l++;

        REMONTER_UN_NIVEAU

        // Lorsque on a épuisé tous les types licites on a nécessairement cur = NULL
    }

#undef bulletinIdent

    return nbLignePaye;
}


#define _BULLETIN(X) _Bulletin(X, #X, bulletinIdent, &cur);

#define BULLETIN_(X)  Bulletin_(X, #X, bulletinIdent, &cur, info->decimal);

#define _BULLETIN_(X)  _Bulletin_(X, #X, bulletinIdent, &cur, info->decimal);

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

    #define bulletinIdent info->Table[info->agent_courant]

    bulletinIdent->ligne[Annee] = info->annee_fichier;
    bulletinIdent->ligne[Mois] = info->mois_fichier;

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

    #undef bulletinIdent

    return ligne;
}

/* agent_total est une variable de contrôle pour info->NCumAgent */

static int32_t  parseFile(info_t* info, int32_t agent_total)
{
    xmlDocPtr doc;
    xmlNodePtr cur = NULL;
    uint16_t agent_du_fichier = 0;

    doc = xmlParseFile(info->threads->argv[info->fichier_courant]);

    if (doc == NULL) return(-1);

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL)
    {
        fprintf(stderr,"document vide\n");
        xmlFreeDoc(doc);
        return -1;
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
            info->nbLigne += ligne_p;
            FILE* log=fopen("/home/fab/log", "a+");
            int calc=0;
            static int max_calc;
            if (info->NLigne[agent_total]-ligne_p)
                //fprintf(log, "\nLigne parsing %5d Ligne preprocessing %5d  Diff %4d\n", ligne_p, info->NLigne[agent_total], info->NLigne[agent_total]-ligne_p);
                {
                    calc = info->NLigne[agent_total]-ligne_p;
                    if (calc)
                    {
                        max_calc=calc;
                        fprintf(log, "diff %d Année %s Mois %s Matricule %s agent_total %d agent_du_fichier %d info->agent_courant %d C %d ligne_p %d\n", calc,
                                  info->annee_fichier,
                                  info->mois_fichier,
                                  info->Table[agent_total]->ligne[Matricule], agent_total, agent_du_fichier, info->agent_courant, info->NLigne[agent_total], ligne_p);
                     //   for (int j=0; 0 < 3; j++) fprintf(log, "Max diff %d Année %s Mois %s Mat %s\n", max_calc, info->annee_fichier, info->mois_fichier, info->Table[info->agent_courant]->ligne[j]);
                    }
                    //exit(-1);
                }
            fclose(log);

            // Ici il est normal que cur = NULL

            cur = cur_save->next;

            AFFICHER_NOEUD(cur->name)

            info->agent_courant++;
            agent_du_fichier++;
            agent_total++;
        }
        cur = cur_save->next;
    }

    printf("Fichier n°%d:\nPopulation du fichier  %s :\n %4d bulletins    Total : %4d bulletins  %4" PRIu64 " lignes cumulées.\n",
           info->fichier_courant,
           info->threads->argv[info->fichier_courant],
           agent_du_fichier, agent_total, info->nbLigne);

    xmlFreeDoc(doc);
    return(agent_total);
    // xmlCleanupParser();
}
#ifdef ECRIRE
#define format_base "%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s\n"

#define args(X, agent, separateur, type, ligne) X[agent]->Annee, separateur, X[agent]->Mois, separateur, X[agent]->Nom, separateur, X[agent]->Prenom, separateur,\
                        X[agent]->Matricule, separateur, X[agent]->Service, separateur, X[agent]->Statut, separateur, X[agent]->QuotiteTrav, separateur,\
                        X[agent]->NbHeureSup, separateur, X[agent]->NbHeureTotal, separateur, X[agent]->Indice, separateur, X[agent]->MtBrut, separateur,\
                        X[agent]->MtNet, separateur, X[agent]->MtNetAPayer, separateur, X[agent]->NBI, separateur, X[agent]->ligne[type][ligne][0], separateur, \
                        X[agent]->ligne[type][ligne][1], separateur, X[agent]->ligne[type][ligne][2], separateur, X[agent]->ligne[type][ligne][3], separateur, X[agent]->ligne[type][ligne][4], separateur,\
                        X[agent]->ligne[type][ligne][5], separateur, type_remuneration_traduit[type], separateur, X[agent]->EmploiMetier, separateur,\
                        X[agent]->Grade, separateur, X[agent]->NIR

#define ECRIRE(...) { fprintf(base, format_base, __VA_ARGS__);  }

inline uint32_t boucle_ecriture(FILE* base,
                                uint32_t nbAgent,
                                bulletinPtr* Table,
                                const char separateur,
                                uint64_t* compteur)
{
    int ligne = 0;
    uint32_t agent = 0;

    for (uint32_t agent = 0; agent < nbAgent; agent++)
    {
        int type = 0;
        while (type < nbType-1)  // Pour chaque type pour lequel il existe un code non NA
        {
            while (xmlStrcmp(Table[agent]->ligne[type][ligne][1], (xmlChar*) NA_STRING))  // Tant qu'il existe un code non NA, càd une ligne de paye correspondante
            {
                // Un peu low-level C, mais beaucoup plus rapide que de coder un fprintf pour chaque item.
                // Gain d'exécution : 30s pour fprintf par item
                //                    22s sur une ligne

                if (ligne == MAX_LIGNES_PAYE) perror("Max lignes de paye atteint !");
                fprintf(base, format_base, args(Table, agent, separateur, type, ligne));
                ligne++;
            }

            *compteur += ligne;
            ligne = 0;
            type++;
        }

    }

    return agent;
}

uint64_t generer_table_bulletins(const char* chemin_base, uint32_t nbAgent, bulletinPtr* Table, const char separateur)
{
    FILE* base = fopen(chemin_base, "wb");
    fseek(base, 0, SEEK_SET);
    if (base == NULL)
    {
        fprintf(stderr, "%s\n", "Impossible d'ouvrir le fichier de sortie.");
        exit(-1000);
    }
    else
        printf("%s\n", "Enregistrement de la base csv");

    ECRIRE(u8"Année",
           separateur,
           "Mois",
           separateur,
           "Nom",
           separateur,
           u8"Prénom",
           separateur,
           "Matricule",
           separateur,
           "Service",
           separateur,
           "Statut",
           separateur,
           u8"Temps.de.travail",
           separateur,
           "Heures.Sup.",
           separateur,
           "Heures",
           separateur,
           "Indice",
           separateur,
           "Brut",
           separateur,
           "Net",
           separateur,
           u8"Net.à.Payer",
           separateur,
           "NBI",
           separateur,
           u8"Libellé",
           separateur,
           "Code",
           separateur,
           "Base",
           separateur,
           "Taux",
           separateur,
           u8"Nb.Unité",
           separateur,
           "Montant",
           separateur,
           "Type",
           separateur,
           "Emploi",
           separateur,
           "Grade",
           separateur,
           "Nir")

    uint64_t compteur=0;

    boucle_ecriture(base, nbAgent, Table, separateur, &compteur);

    fclose(base);
    return compteur;
}

int64_t generer_table_standard(const char* chemin_table, uint32_t nbAgent, bulletinPtr* Table, char separateur)
{



}
#endif

//#define PUTCHAR(X) putchar(X)
#define PUTCHAR(X) X

inline int calculer_memoire_requise(info_t* info)
{
    errno = 0;
    info->NLigne = (uint16_t*) calloc(info->threads->argc, MAX_NB_AGENTS * info->threads->argc * sizeof(uint16_t));  // nm total de bulletins
    info->NAgent = (int32_t*) calloc(info->threads->argc, sizeof(int32_t));
    info->NCumAgent = 0;
    int control=0;
    puts("Premier scan des fichiers pour déterminer les besoins mémoire ... ");
    int max_nbLigne = 30  ;
    char codest[15]={0};
    /* par convention  un agent avec rémunération non renseignées (balise sans fils) a une ligne */
    for (int i = 0; i < info->threads->argc ; i++)
    {
        FILE* c;
        errno = 0;
        c = fopen(info->threads->argv[i], "r");
        //if (c) fseek(c, 0, SEEK_SET);
        //else
        if(c == NULL)  {perror("Ouverture Fichiers."); exit(-120);}// cautious no-op
        if (errno)
        {
            perror("Fichier .xhl");
            exit(-122);
        }
        int d = 0;

        while (d != EOF && (d = fgetc(c), PUTCHAR(d)) != EOF)
        {
            if  (d != '<') continue;
            if  ((d = fgetc(c), PUTCHAR(d)) != 'R') continue;
            if  ((d = fgetc(c), PUTCHAR(d)) != 'e') continue;
            if  ((d = fgetc(c), PUTCHAR(d)) != 'm') continue;
            if  ((d = fgetc(c), PUTCHAR(d)) != 'u') continue;
            if  ((d = fgetc(c), PUTCHAR(d)) != 'n') continue;
            if  ((d = fgetc(c), PUTCHAR(d)) != 'e') continue;
            if  ((d = fgetc(c), PUTCHAR(d)) != 'r') continue;
            if  ((d = fgetc(c), PUTCHAR(d)) != 'a') continue;
            if  ((d = fgetc(c), PUTCHAR(d)) != 't') continue;
            if  ((d = fgetc(c), PUTCHAR(d)) != 'i') continue;
            if  ((d = fgetc(c), PUTCHAR(d)) != 'o') continue;
            if  ((d = fgetc(c), PUTCHAR(d)) != 'n') continue;
            if  ((d = fgetc(c), PUTCHAR(d)) == '/')
            {
                 //if (info->NLigne[info->NCumAgent] == 0) info->NLigne[info->NCumAgent] = 1;
                    info->NAgent[i]++;
                    if (info->NLigne[info->NCumAgent] > max_nbLigne) { max_nbLigne = info->NLigne[info->NCumAgent]; printf("%d\n", max_nbLigne);}
                    info->NLigne[info->NCumAgent]++;
                    info->NCumAgent++;
                    control++;
                    //printf("\nNCumAgent : %d Control : %d info->NLigne : %d \n", info->NCumAgent, control, info->NLigne[info->NCumAgent-1]);
                continue;  // Balise simple vide
            }

            while (d != EOF && (d = fgetc(c), PUTCHAR(d)) != EOF)
            {
                if (d != '<') continue;
                if ((d = fgetc(c), PUTCHAR(d)) != 'C')
                {
                    if (d != '/') continue;
                    else
                        if((d = fgetc(c), PUTCHAR(d)) != 'R')   continue;
                        else
                            if ((d = fgetc(c), PUTCHAR(d)) != 'e')   continue;
                            else
                                if((d = fgetc(c), PUTCHAR(d)) != 'm')   continue;
                                else
                                  if((d = fgetc(c), PUTCHAR(d)) != 'u')    continue;
                                    else
                                      if((d = fgetc(c), PUTCHAR(d)) != 'n')   continue;


                    //if (info->NLigne[info->NCumAgent] == 0) info->NLigne[info->NCumAgent] = 1;
                    info->NAgent[i]++;
                    if (info->NLigne[info->NCumAgent] > max_nbLigne) { max_nbLigne = info->NLigne[info->NCumAgent]; printf("%d\n", max_nbLigne);}
                    info->NCumAgent++;

                    //printf("\nNCumAgent : %d Control : %d info->NLigne : %d \n", info->NCumAgent, control, info->NLigne[info->NCumAgent-1]);
                                        //exit(-1);

                    break;
                }
                else
                {
                    if ((d = fgetc(c), PUTCHAR(d)) != 'o') continue;
                    else
                    {
                        PUTCHAR('*');
                        if ((d = fgetc(c), PUTCHAR(d)) != 'd')   continue;
                        else
                        {
                            PUTCHAR('*');
                            if ((d = fgetc(c), PUTCHAR(d)) != 'e')   continue;
                            else
                            {
                               PUTCHAR('*');
                               if ((d = fgetc(c), PUTCHAR(d)) != ' ')   continue;
                               info->NLigne[info->NCumAgent]++;
                               control++;
                               //printf("%s", fgets(codest, 9, c));
                               PUTCHAR('@');
                            }
                        }
                    }
                }

            }
        }

        info->nbLigne = info->NLigne[info->NCumAgent - 1];
        fclose(c);
    }

    //for (int i=0; i < info->NCumAgent; i++) info->NLigne[i] = max_nbLigne;
    //printf("NCumAgent: %d\n", info->NCumAgent);
    return errno;
}


void decoder_fichier(info_t* info, int32_t* agent_total)
{
    /* environ 6000 bulletins par seconde en processus sumple, et 15000 en multithread ; rajoute 1/3 du temps */

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

    printf("Numbre total de bulletins d'agent : %d  Maximum de lignes par bulletins: %d\n", info->NCumAgent, info->NLigne[0]);

    for (int agent = 0; agent < info->NCumAgent; agent++)
    {
        info->Table[agent] = (bulletinPtr) malloc(sizeof(bulletin));
        if (info->Table[agent] == NULL)
        {
            perror("Mémoire insuffisante");
            exit(-19);
        }

        info->Table[agent]->ligne = (xmlChar**) calloc(((info->reduire_consommation_memoire)?
                                                        info->besoin_memoire_par_ligne + nbType + (info->NLigne[agent])*6
                                                         : MAX_LIGNES_PAYE), sizeof(xmlChar*));
        //free(info->NLigne);
        //free(info->NAgent);
        //info->NLigne[agent]
        if (info->Table[agent]->ligne == NULL)
        {
            perror("Erreur d'allocation de drapeau I.");
            exit(-63);
        }
    }

    for (int i = 0; i < info->threads->argc ; i++)
    {
        info->fichier_courant = i;

        //fprintf(stderr, "Fichier: %s, %d/%d, nbLigne=%" PRIu64 "\n", info->threads->argv[i], i+1, info->threads->argc, info->nbLigne);
        *agent_total = parseFile(info, *agent_total);
    }
}

void* launch(void* info)
{

    info_t* tinfo = ((info_t*)info);

    tinfo->Table = (bulletinPtr*) malloc(tinfo->NCumAgent *  sizeof(bulletinPtr));

    if (tinfo->Table == NULL)
    {
        perror("Mémoire insuffisante");
        exit(-18);
    }

    xmlKeepBlanksDefault(0);
    int32_t agent_total=0;
    decoder_fichier(tinfo, &agent_total);
    return NULL;
    /* controle sur agent_total */
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
    uint32_t nbAgentUtilisateur = MAX_NB_AGENTS;

    char type_table[50]= {0};
    strcpy(type_table, "bulletins");

    char chemin_table[500]= {0};
    strcpy(chemin_table, "Table.csv");
    bool afficher_memoire_reservee = false;
    bool generer_table = false;
    bool liberer_memoire = true;
    bool reduire_consommation_memoire = true;
    char separateur = ',';
    char decimal = '.';
    int nbfil = 0;

    while (start < argc)
    {
        if (! strcmp(argv[start], "-n"))
        {
            reduire_consommation_memoire = false;
            if (argc > 2)
            {
                const char* const c_str = argv[start + 1];
                char *end;
                errno = 0;
                const long sl = strtol(c_str, &end, 10);

                if (end == c_str)
                {
                    fprintf(stderr, "%s: pas un décimal\n", c_str);
                }
                else if ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno)
                {
                    fprintf(stderr, "%s entier excédant la limite des entiers longs\n", c_str);
                }
                else if (sl > UINT16_MAX)
                {
                    fprintf(stderr, "%ld entier excédant la limite des entiers à 16 bits\n", sl);
                }
                else if (sl < 0)
                {
                    fprintf(stderr, "%ld l'entier doit être positif\n", sl);
                }
                else
                {
                    nbAgentUtilisateur  = (uint32_t)sl;
                    start += 2;
                    continue;
                }
                exit(-100);
            }
            else
            {
                fprintf(stderr, "%s\n", "Préciser le nombre de bulletins mensuels attendus (majorant du nombre).");
                return -3;
            }
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
            separateur = argv[start + 1][0];
            if (separateur == '!')
            {
                fprintf(stderr, "%s\n", "Le séparateur ne peut pas être '!'");
                exit(-100);
            }
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
            decimal = argv[start + 1][0];
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
            start += 2;
            continue;
        }
        else if (! strcmp(argv[start], "-j"))
        {
            nbfil = atoi(argv[start +1]);
            if (nbfil > 10 || nbfil < 2)
            {
                perror("Le nombre de fils d'exécution doit être compris entre 2 et 10.");
                exit(-111);
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
    uint64_t memoire_reservee = nbAgentUtilisateur*(argc-start)*sizeof(bulletin);
    if (memoire_reservee > MAX_MEMOIRE_RESERVEE)
    {
        fprintf(stderr, "Quantité de mémoire réservée %" PRIu64 " supérieure au maximum de %" PRIu64 " octets.\nAppliquer le programme sur une partie des fichiers et fusionner les bases en résultant.\n", memoire_reservee, MAX_MEMOIRE_RESERVEE);
        exit(-500);
    }

    if (afficher_memoire_reservee) fprintf(stderr, "Quantité de mémoire réservée %" PRIu64 " octets.\n", memoire_reservee);

    thread_t mon_thread;
    info_t info =
    {
            NULL,             //    bulletinPtr* Table;
            0,                //    uint64_t nbLigne;
            NULL,             //    int32_t  *NAgent;
            nbAgentUtilisateur,          //
            nbAgentUtilisateur*(argc-start),      //    uint32_t NCumAgent;
            NULL,             //    uint16_t *NLigne;
            0,                //    uint16_t fichier_courant;
            0,                //    uint32_t agent courant
            &mon_thread,      //    thread_t threads;
            NULL,             //    xmlChar*  annee_fichier;
            NULL,             //    xmlChar* mois_fichier;
            NULL,             //    char** fichiers;
            decimal,          //    const char decimal;
            separateur,       //    const char separateur;
            reduire_consommation_memoire,  //bool
            BESOIN_MEMOIRE_ENTETE// besoin mémoire minimum hors lecture de lignes : devra être incréméenté
    };


    if (nbfil == 0 || (argc -start < 2))
    {
        info.threads->argc = argc -start;
        info.threads->argv = (char**) malloc((argc -start)* sizeof(char*));
        info.reduire_consommation_memoire = reduire_consommation_memoire;
        for (int j = start; j < argc; j++) info.threads->argv[j-start] = strdup(argv[j]);

        launch((void*) &info);
    }
    else
    {
        xmlInitParser();
        int nbfichier_par_fil = floor((argc - start) / nbfil);
        if (nbfichier_par_fil == 0)
        {
            fprintf(stderr, "%s\n", "Trop de fils pour le nombre de fichiers ; exécution avec -j 2");
            nbfil = 2;
        }

        if ((argc - start) % nbfil) nbfil++;  // on en crée un de plus pour le reste

        pthread_t thread_clients[nbfil];

        info_t* Info = (info_t* ) malloc(nbfil*sizeof(info_t));
        if (Info == NULL) {perror("Allocation de info"); exit(-144); }

        puts("Creation des fils clients.\n");
        for (int i = 0; i < nbfil; i++)
        {
            Info[i] = info; // initialisation d'ensemble
            Info[i].threads->thread_num = i;
            Info[i].reduire_consommation_memoire = reduire_consommation_memoire;
            Info[i].threads->argc = (argc - start < nbfichier_par_fil)? argc - start: nbfichier_par_fil;
            Info[i].threads->argv = (char**) malloc(nbfichier_par_fil * sizeof(char*));

            for (int j = 0; j <  nbfichier_par_fil && start + j < argc; j++)
            {
                Info[i].threads->argv[j] = strdup(argv[start + j]);
            }

            start += nbfichier_par_fil;

            int ret = pthread_create (
                          &thread_clients[i],
                          NULL,
                          launch,
                          &Info[i]
                      );

            if (ret)
            {
                fprintf (stderr, "%s", strerror(ret));
            }
        }

        for (int i = 0; i < nbfil; i++)
        {
            pthread_join (thread_clients [i], NULL);
        }

        xmlCleanupParser();

    }


   // uint64_t nbLigneBase=0;

    if (generer_table)
    {

//        if (! strcmp(type_table, "standard"))
//          //  nbLigneBase = generer_table_standard(chemin_table, nbAgent, Table, separateur);
//        else if (! strcmp(type_table, "bulletins"))
//            //nbLigneBase = generer_table_bulletins(chemin_table, nbAgent, Table, separateur);
//        else
//        {
//            fprintf(stderr, "Type %s inconnu.", type_table);
//            exit(-501);
//        }
    }

//    fprintf(stderr, "Table de %" PRIu64 " lignes générée pour %" PRIu64 "lignes de paie d'origine.\n", nbLigneBase, nbLigne);

#define FREE(X) if (X && xmlStrcmp(X, (xmlChar*)NA_STRING)) xmlFree(X);

    /* libération de la mémoire */
// calculer nbAgent à partir de info->NCumAgent...
    if (liberer_memoire)
        for (int i = 0; i < nbAgentUtilisateur; i++)
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

