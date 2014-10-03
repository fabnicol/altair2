
/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti en UTF-8 avant d'être compilé.
 *  Prêter attention que certains éditeurs sous Windows convertissent automatiquement sous d'autres
 *  encodages. Il peut en résulter des erreurs de chaînes accentuées.
 *  Note : l'encodage UTF-8 est requis par libxml2
 */

/* Constantes de compilation pouvant être redéfinies : NA_STRING, MAX_LIGNES_PAYE, MAX_NB_AGENTS, NO_DEBUG, MAX_MEMOIRE_RESERVEE */


#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include <locale.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#if !NO_DEBUG
#include <Windows.h>
#endif
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#define nbType 9

#ifndef NA_STRING
 #define NA_STRING "NA"
#endif

#ifndef MAX_LIGNES_PAYE
 #define MAX_LIGNES_PAYE 100
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
#endif

#ifndef MAX_MEMOIRE_RESERVEE
uint64_t MAX_MEMOIRE_RESERVEE = UINT32_MAX;
// = 2^32 = 4 294 967 296 = 4GB
#endif

#define SAUTER_UN_NOEUD cur = (cur)? cur->next: NULL;  //cur = (cur)? cur->next: NULL;

#define Bulletin(X, Y) if (cur != NULL && !xmlStrcmp(cur->name, (const xmlChar *) #Y)) { \
                          bulletinIdent->X = xmlGetProp(cur, (const xmlChar *) "V"); \
                          SAUTER_UN_NOEUD \
                     } else { \
                            bulletinIdent->X = (xmlChar*) NA_STRING; \

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

const char* type_remuneration[nbType] = {"TraitBrut", "IndemResid", "SupFam", "Indemnite", "RemDivers", "Deduction", "Rappel", "Retenue", "Cotisation"};

xmlChar* annee_fichier = NULL;
xmlChar* mois_fichier = NULL;

typedef struct bulletin
{
    xmlChar *Nom;
    xmlChar *Prenom;
    xmlChar *Matricule;
    xmlChar *Annee;
    xmlChar *Mois;
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
    xmlChar *ligne[nbType][MAX_LIGNES_PAYE][6];  // type de ligne de paye x rang de la ligne x {Libellé, ..., Montant}

} bulletin, *bulletinPtr;

bulletinPtr bulletinIdent = NULL;

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

static inline void lignePaye(xmlNodePtr cur, bulletinPtr bulletinIdent)
{
    int l = 0;
    int t = 0;

    while (cur != NULL)
    {
        if (xmlStrcmp(cur->name, (const xmlChar *) type_remuneration[t]))
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
    cur = atteindreNoeud("Agent", cur);
    if (cur != NULL)
    {
        bulletinIdent = (bulletinPtr) calloc(1, sizeof(bulletin));
        if (bulletinIdent == NULL)
        {
            fprintf(stderr,"Pas assez de mémoire\n");
            return;
        }

        for (int i = 0; i < nbType; i++)
            for (int j = 0; j < MAX_LIGNES_PAYE; j++)
              for (int k = 0; k < 6; k++)
               bulletinIdent->ligne[i][j][k] = (xmlChar *) NA_STRING;

        bulletinIdent->Annee = annee_fichier;
        bulletinIdent->Mois = mois_fichier;

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

static int32_t  parseFile(const char *filename,  bulletinPtr* Table)
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

    cur = atteindreNoeud("Annee", cur);

    if (cur != NULL)
        {
            annee_fichier = xmlGetProp(cur, (const xmlChar *) "V");
            SAUTER_UN_NOEUD
        }
        else
        {
           fprintf(stderr, "%s\n", "Année non détectable");
           exit(-502);
        }

    if (cur != NULL)
        {
            mois_fichier = xmlGetProp(cur, (const xmlChar *) "V");
        }
        else
        {
           fprintf(stderr, "%s\n", "Mois non détectable");
           exit(-503);
        }

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

    agent_du_fichier--;
    agent_total--;
    printf("Population du fichier %s : %4d bulletins    Total : %4d bulletins.\n", filename, agent_du_fichier, agent_total);
//    xmlFree(annee_fichier);
//    xmlFree(mois_fichier);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return((int32_t) agent_total);
}


#define ECRIRE(...) { fprintf(table, "%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s\n", __VA_ARGS__);  }

int64_t generer_table_bulletins(const char* chemin_table, uint32_t taille, bulletinPtr* Table, char separateur, char decimal)
{

    FILE* table = fopen(chemin_table, "wb");
    if (table == NULL)
    {
        fprintf(stderr, "%s\n", "Impossible d'ouvrir le fichier de sortie.");
        exit(-1000);
    }
    else printf("%s\n", "Enregistrement de la base csv");

//    ECRIRE(u8"Année")
//    ECRIRE("Mois")
//    ECRIRE("Nom")
//    ECRIRE(u8"Prénom")
//    ECRIRE("Matricule")
//    ECRIRE("Service")
//    ECRIRE("Statut")
//    ECRIRE(u8"Temps de travail")
//    ECRIRE("HeureSup")
//    ECRIRE("Heures")
//    ECRIRE("Indice")
//    ECRIRE("Brut")
//    ECRIRE("Net")
//    ECRIRE(u8"Net à Payer")
//    ECRIRE("NBI")
//    ECRIRE(u8"Libellé")
//    ECRIRE("Code")
//    ECRIRE("Base")
//    ECRIRE("Taux")
//    ECRIRE(u8"Nb. Unité")
//    ECRIRE("Montant")
//    ECRIRE("Type")
//    ECRIRE("Emploi")
//    ECRIRE("Grade")
//    ECRIRE0("NIR")

    int ligne=0;

    int64_t compteur = 0;

    for (uint32_t agent = 0; agent < taille; agent++)
    {
        int type=0;
        while (type < nbType)  // Pour chaque type pour lequel il existe un code non NA
        {
            while (xmlStrcmp(Table[agent]->ligne[type][ligne][1], (xmlChar*) NA_STRING) && ligne < MAX_LIGNES_PAYE)  // Tant qu'il existe un code non NA, càd une ligne de paye correspondante
            {
                // Un peu low-level C, mais 3 fois plus rapide que de coder un fprintf pour chaque item.

         ECRIRE(Table[agent]->Annee,
                separateur,
                Table[agent]->Mois,
                separateur,
                Table[agent]->Nom,
                separateur,
                Table[agent]->Prenom,
                separateur,
                Table[agent]->Matricule,
                separateur,
                Table[agent]->Service,
                separateur,
                Table[agent]->Statut,
                separateur,
                Table[agent]->QuotiteTrav,
                separateur,
                Table[agent]->NbHeureSup,
                separateur,
                Table[agent]->NbHeureTotal,
                separateur,
                Table[agent]->Indice,
                separateur,
                Table[agent]->MtBrut,
                separateur,
                Table[agent]->MtNet,
                separateur,
                Table[agent]->MtNetAPayer,
                separateur,
                Table[agent]->NBI,
                separateur,
                Table[agent]->ligne[type][ligne][0], //"Libellé"
                separateur,
                Table[agent]->ligne[type][ligne][1], //"Code"
                separateur,
                Table[agent]->ligne[type][ligne][2], //"Base"
                separateur,
                Table[agent]->ligne[type][ligne][3], //"Taux"
                separateur,
                Table[agent]->ligne[type][ligne][4], //"Nb. Unité"
                separateur,
                Table[agent]->ligne[type][ligne][5], //"Montant"
                separateur,
                type_remuneration[type],
                separateur,
                Table[agent]->EmploiMetier,
                separateur,
                Table[agent]->Grade,
                separateur,
                Table[agent]->NIR)

                ligne++;
            }
            compteur += ligne;
            ligne=0;
            type++;
        }

    }

    fclose(table);
    return compteur;
}

int64_t generer_table_standard(const char* chemin_table, uint32_t taille, bulletinPtr* Table, char separateur, char decimal)
{



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
    uint16_t nbAgents = MAX_NB_AGENTS;

    int start = 1;

    char type_table[50]= {0};
    strcpy(type_table, "bulletins");
    char decimal = '.';
    char separateur = ',';
    char chemin_table[500]= {0};
    strcpy(chemin_table, "table.csv");
    bool afficher_memoire_reservee = false;
    bool generer_table = false;

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
            printf("-n nombre de bulletins mensuels attendus [défaut %d].\n", MAX_NB_AGENTS);
            printf("%s\n", "-t argument optionnel : type de base en sortie, soit 'standard', soit 'bulletins' [défaut bulletins].");
            printf("%s\n", "-o argument obligatoire : fichier.csv, fichier de sortie [défaut 'table.csv' avec -t].");
            printf("%s\n", "-d argument obligatoire : séparateur décimal [défaut . avec -t].");
            printf("%s\n", "-s argument obligatoire : séparateur de champs [défaut , avec -t]/");
            printf("%s\n", "-m sans argument : mémoire réservée. Estimation de la consommation de mémoire.");
            exit(0);
        }
        else if (!strcmp(argv[start], "-t"))
        {
            generer_table = true;
            if (!strcmp(argv[start + 1], "standard"))
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
        else if (!strcmp(argv[start], "-s"))
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
            strncpy(chemin_table, argv[start + 1], 500*sizeof(char));
            start += 2;
            continue;
        }
        else if (!strcmp(argv[start], "-m"))
        {
            afficher_memoire_reservee = true;
            start +=1;
            continue;
        }
        else if (argv[start][0] == '-')
        {
            fprintf(stderr, "%s\n", "Option inconnue.");
            exit(-100);
        }
        else break;
    }

    bulletinPtr Table[nbAgents*(argc-start)];
    uint64_t memoire_reservee = nbAgents*(argc-start)*sizeof(bulletin);
    if (memoire_reservee > MAX_MEMOIRE_RESERVEE)
    {
        fprintf(stderr, "Quantité de mémoire réservée %" PRIu64 " supérieure au maximum de %" PRIu64 " octets.\nAppliquer le programme sur une partie des fichiers et fusionner les bases en résultant.\n", memoire_reservee, MAX_MEMOIRE_RESERVEE);
        exit(-500);
    }

    if (afficher_memoire_reservee) fprintf(stderr, "Quantité de mémoire réservée %" PRIu64 " octets.\n", memoire_reservee);

    memset(Table, 0, sizeof(Table));

    int32_t taille = 0;

    for (int i = start; i < argc ; i++)
    {
        taille = parseFile(argv[i], Table);

        if (taille < 0)
        {
            fprintf(stderr, "Erreur de décodage pour le fichier %s\n", argv[i]);
            return -1;
        }
    }

    if (generer_table)
    {
        if (!strcmp(type_table, "standard"))
            generer_table_standard(chemin_table, taille, Table, separateur, decimal);
        else if (!strcmp(type_table, "bulletins"))
            generer_table_bulletins(chemin_table, taille, Table, separateur, decimal);
        else
        {
            fprintf(stderr, "Type %s inconnu.", type_table);
            exit(-501);
        }
    }

    return(0);
}

#ifdef __cplusplus
}
#endif

