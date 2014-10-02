


#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <Windows.h>

#define nbType 9

#ifndef MAX_LIGNES_PAYE
#define MAX_LIGNES_PAYE 300
#endif

#ifndef MAX_NB_AGENTS
#define MAX_NB_AGENTS 2000
#endif

#ifndef NO_DEBUG
#define DEBUG(X) fprintf(stderr, "%s\n", X);
#define AFFICHER_NOEUD(X)       { char msg[50]={0}; \
                                  sprintf(msg, "atteint %s\n", X);\
                                  DEBUG(msg) }

#define NO_DEBUG 0
#else
#define NO_DEBUG 1
#define DEBUG(X)
#define AFFICHER_NOEUD(X)
#endif // NO_DEBUG

#define SAUTER_UN_NOEUD cur = (cur)? cur->next: NULL;  //cur = (cur)? cur->next: NULL;

#define Bulletin(X, Y) if (cur != NULL && !xmlStrcmp(cur->name, (const xmlChar *) #Y)) { \
                          bulletinIdent->X = xmlGetProp(cur, (const xmlChar *) "V"); \
                          SAUTER_UN_NOEUD \
                     } else { \
                            bulletinIdent->X = NULL; \

#define _Bulletin(X, Y)   Bulletin(X, Y) \
                          if (cur) \
                              fprintf(stderr, "Trouvé %s au lieu de " #X "\n", cur->name);   \
                            else \
                              fprintf(stderr, "%s\n", "Noeud courant null au stade de la vérification de " #X ); \
                              SAUTER_UN_NOEUD \
                          }

#define _BULLETIN(X)      _Bulletin(X, X)

/* pas de contrôle d'existence de noeud : version affaiblie de la macro précédente */

#define _Bulletin_(X, Y)     Bulletin(X, Y) }

#define DESCENDRE_UN_NIVEAU    cur = (cur)? cur->xmlChildrenNode: NULL;  if ((!NO_DEBUG) && cur) fprintf(stderr, "Descente au niveau %s\n", cur->name);  // cur = (cur)? cur-> next: NULL;

#define REMONTER_UN_NIVEAU     cur = (cur)? cur->parent: NULL;   if ((!NO_DEBUG) && cur) fprintf(stderr, "Remontée au niveau %s\n", cur->name); SAUTER_UN_NOEUD


/* Traitement Brut : */
/* Indemnité de résidence */
/* Supplément familial */
/* Indemnités */
/* Rémunérations diverses */
/* Déduction */
/* Rappel */
/* Retenue */
/* Cotisation */

const char* type[nbType] = {"TraitBrut", "IndemResid", "SupFam", "Indemnite", "RemDivers", "Deduction", "Rappel", "Retenue", "Cotisation"};

typedef struct bulletin
{
    xmlChar *Nom;
    xmlChar *Prenom;
    xmlChar *Matricule;
    xmlChar *NIR;
    xmlChar *Statut;
    xmlChar *EmploiMetier;
    xmlChar *Grade;
    xmlChar *Indice;
    xmlChar *Service;
    xmlChar *NBI;
    xmlChar *QuotiteTrav;
    xmlChar *NbHeureTotal;
    xmlChar *NbHeureSup;
    xmlChar *MtBrut;
    xmlChar *MtNet;
    xmlChar *MtNetAPayer;
    xmlChar *ligne[nbType][MAX_LIGNES_PAYE][6] = {{{NULL}}};  // type de ligne de paye x rang de la ligne x {Libellé, ..., Montant}

} bulletin, *bulletinPtr;

bulletinPtr bulletinIdent = NULL;

inline xmlNodePtr atteindreNoeud(const char* noeud, xmlNodePtr cur)
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

inline void lignePaye(xmlNodePtr cur, bulletinPtr bulletinIdent)
{
    int l = 0;
    int t = 0;

    while (cur != NULL)
    {
        if (xmlStrcmp(cur->name, (const xmlChar *) type[t]))
        {
            t++;
            l = 0;
            if (t == nbType)
            {
                fprintf(stderr, "En excès du nombre de types de lignes de paye autorisé (%d)\n", nbType);
                fprintf(stderr, "Type litigieux : %s\n", cur->name);
                exit(-11);
            }
            continue;
        }

        DESCENDRE_UN_NIVEAU
        /* Libellé, obligatoire */

        cur = atteindreNoeud("Libelle", cur);

        _Bulletin(ligne[t][l][0], Libelle)
        /* Code, obligatoire */
        cur = atteindreNoeud("Code", cur);
        _Bulletin(ligne[t][l][1], Code)
        /* Base, si elle existe */
        _Bulletin_(ligne[t][l][2], Base)
        /* Taux, s'il existe */
        _Bulletin_(ligne[t][l][3], Taux)
        /* Nombre d'unités, s'il existe */
        _Bulletin_(ligne[t][l][4], NbUnite)
        /* Montant , obligatoire */
        cur = atteindreNoeud("Mt", cur);
        _Bulletin(ligne[t][l][5], Mt)

        REMONTER_UN_NIVEAU
        l++;
        if (l == MAX_LIGNES_PAYE)
        {
            fprintf(stderr, "En excès du nombre de lignes de paye autorisé (%d)\n", MAX_LIGNES_PAYE);
            exit(-10);
        }

        // Lorsque on a épuisé tous les types licites on a nécessairement cur = NULL
    }
}

static void  parseBulletin(xmlNodePtr cur)
{
    DEBUG("Parsage")

    if (cur == NULL) return;
    cur = atteindreNoeud((const char*) "Agent", cur);
    if (cur != NULL)
    {
        bulletinIdent = (bulletinPtr) calloc(1, sizeof(bulletin));
        if (bulletinIdent == NULL)
        {
            fprintf(stderr,"Pas assez de mémoire\n");
            return;
        }

        DESCENDRE_UN_NIVEAU

        SAUTER_UN_NOEUD
        _BULLETIN(Nom)
        _BULLETIN(Prenom)
        _BULLETIN(Matricule)
        _BULLETIN(NIR)
        SAUTER_UN_NOEUD
        SAUTER_UN_NOEUD
        _BULLETIN(Statut)
        _BULLETIN(EmploiMetier)
        _BULLETIN(Grade)
        SAUTER_UN_NOEUD
        _BULLETIN(Indice)

        REMONTER_UN_NIVEAU
    }
    else
    {
        fprintf(stderr, "%s\n", "Impossible d'atteindre \"Agent\"");
        return;
    }

    cur = atteindreNoeud("Service", cur);
    _BULLETIN(Service)
    _BULLETIN(NBI)
    _BULLETIN(QuotiteTrav)
    SAUTER_UN_NOEUD

    if (cur && !xmlStrcmp(cur->name, (const xmlChar *)"Remuneration"))
    {
        xmlNodePtr cur_save = cur;

        DESCENDRE_UN_NIVEAU

        lignePaye(cur, bulletinIdent);
        cur = cur_save->next;
    }
    else
    {
        fprintf(stderr, "Trouvé %s au lieu de \"Remuneration\"", cur->name);
        exit(-4);
    }

    _BULLETIN(NbHeureTotal)
    cur = atteindreNoeud("NbHeureSup", cur);
    _BULLETIN(NbHeureSup)
    _BULLETIN(MtBrut)
    _BULLETIN(MtNet)
    _BULLETIN(MtNetAPayer)
}


static int  parseFile(const char *filename, uint16_t nbAgents, bulletinPtr* Table)
{
    xmlDocPtr doc;
    xmlNodePtr cur = NULL;
    static uint16_t agent_total;
    uint16_t agent_du_fichier = 0;

    doc = xmlParseFile(filename);

    if (doc == NULL) return(-1);

    cur = xmlDocGetRootElement(doc);

    if (cur == NULL)
    {
        fprintf(stderr,"document vide\n");
        xmlFreeDoc(doc);
        return -1;
    }

    DESCENDRE_UN_NIVEAU

    cur = atteindreNoeud("DonneesIndiv", cur);

    DESCENDRE_UN_NIVEAU

    while(cur != NULL)
    {
#if !NO_DEBUG
        char msg[50] = { 0};
        sprintf(msg, "Paye n°%d\n", agent_du_fichier);

        DEBUG(msg);

        Sleep(1000);
#endif

        cur = atteindreNoeud("PayeIndivMensuel", cur);
        xmlNodePtr cur_save =  cur;

        DESCENDRE_UN_NIVEAU

        parseBulletin(cur);
        // Ici il est normal que cur = NULL

        cur = cur_save->next;

        AFFICHER_NOEUD(cur->name)

        Table[agent_total++] = bulletinIdent;
        agent_du_fichier++;
    }

    printf("Population du fichier %s : %4d bulletins    Total : %4d bulletins.\n", filename, agent_du_fichier, agent_total);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return(0);
}


int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "%s\n", "Il faut au moins un fichier à analyser.");
        return -2;
    }

    LIBXML_TEST_VERSION
    xmlKeepBlanksDefault(0);
    uint16_t nbAgents = MAX_NB_AGENTS;

    int start = 1;

    char* type_table;
    strcpy(type_table, "standard");
    char decimal = '.';
    char separateur = ',';
    char* chemin_table;
    strcpy(chemin_table, "table.csv");

    while (start < argc)
    {
         if (!strcmp(argv[start], "-n"))
            {
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
                        nbAgents  = (uint16_t)sl;
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
        else if (!strcmp(argv[start], "-h"))
            {
                printf("%s\n", "Usage :  xhl2csv OPTIONS fichiers.xhl");
                puts("OPTIONS :");
                printf("-n nombre de bulletins mensuels attendus [défaut %d]\n", MAX_NB_AGENTS);
                printf("%s\n", "-t type de base en sortie, soit 'standard', soit 'bulletins' [défaut standard]");
                printf("%s\n", "-o fichier.csv, fichier de sortie [défaut table.csv]");
                printf("%s\n", "-d séparateur décimal [défaut .]");
                printf("%s\n", "-s séparateur de champs [défaut ,]");
                exit(0);
            }
        else if (!strcmp(argv[start], "-t"))
        {
            if (start + 1 == argc)
            {
                fprintf(stderr, "%s\n", "Option -t suivi d'un argument obligatoire, standard ou bulletins.");
                exit(-100);
            }

            if (!strcmp(argv[start + 1], "standard") || !strcmp(argv[start + 1], "bulletins"))
            {
                type_table = argv[start + 1];
                start += 2;
                continue;
            }
            else
            {
                fprintf(stderr, "%s\n", "Préciser le type de la table, standard ou bulletins.");
                exit(-100);
            }
        }
       else if (!strcmp(argv[start], "-s"))
        {
            if (start + 1 == argc)
            {
                fprintf(stderr, "%s\n", "Option -s suivi d'un argument obligatoire (séparateur de champs).");
                exit(-100);
            }

                separateur = argv[start + 1][0];
                start += 2;
                continue;
        }
       else if (!strcmp(argv[start], "-d"))
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
       else if (!strcmp(argv[start], "-o"))
        {
            if (start + 1 == argc)
            {
                fprintf(stderr, "%s\n", "Option -o suivi d'un argument obligatoire (nom de  fichier).");
                exit(-100);
            }

                chemin_table = argv[start + 1];
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

    bulletinPtr Table[nbAgents*(argc-1)];
    memset(Table, 0, nbAgents*(argc-1)*sizeof(bulletinPtr));

    for (int i = start; i < argc ; i++)
    {
        int ret = parseFile(argv[i], nbAgents, Table);

        if (ret == -1)
        {
            fprintf(stderr, "Erreur de décodage pour le fichier %s\n", argv[i]);
            return -1;
        }
    }

    return(0);
}

#ifdef __cplusplus
}
#endif

