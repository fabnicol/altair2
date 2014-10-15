/*  Programme écrit par Fabrice NICOL sous licence CECILL 3
 *  Attention : lorsqu'il est édité, le présent code doit être converti soit en UTF-8 soit en ISO-5589-1 (Latin-1)avant d'être compilé.
 *  En entrée d'Altair préciser encodage.entrée en conformité avec l'encodage du présent fichier, qui sera celui de la base générée.
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
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#define nbType 12

#ifndef NA_STRING
#define NA_STRING "NA"
#endif

#ifndef MAX_LIGNES_PAYE
#define MAX_LIGNES_PAYE 350
#endif

#ifndef MAX_NB_AGENTS
#define MAX_NB_AGENTS 1200
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
#if !NO_DEBUG
#include <Windows.h>
#endif

#ifndef MAX_MEMOIRE_RESERVEE
uint64_t MAX_MEMOIRE_RESERVEE = UINT32_MAX;
// = 2^32 = 4 294 967 296 = 4GB
#endif
/* pas de contrôle d'existence de noeud : version affaiblie de la macro précédente */


#define DESCENDRE_UN_NIVEAU    cur = (cur)? cur->xmlChildrenNode: NULL;  if ((! NO_DEBUG) && cur) fprintf(stderr, "Descente au niveau %s\n", cur->name);  // cur = (cur)? cur-> next: NULL;

#define REMONTER_UN_NIVEAU     cur = (cur)? cur->parent: NULL;   if ((! NO_DEBUG) && cur) fprintf(stderr, "Remontée au niveau %s\n", cur->name); cur = (cur)? cur->next: NULL;

/* Traitement Brut : */
/* Indemnité de résidence */
/* Supplément familial */
/* Indemnités */
/* Rémunérations diverses */
/* Déduction */
/* Rappel */
/* Retenue */
/* Cotisation */

const char* type_remuneration[nbType] = {"TraitBrut", "IndemResid", "SupFam", "AvantageNature", "Indemnite", "RemDivers", "Deduction", "Acompte", "Rappel", "Retenue", "Cotisation", "Commentaire"};
const char* type_remuneration_traduit[nbType] = {"Traitement", u8"Indemnité de résidence", u8"Supplément familial", "Avantage en nature", u8"Indemnité", u8"Autres rémunérations", u8"Déduction", "Acompte", "Rappel", "Retenue", "Cotisation", "Commentaire"};


#define Nom 0
#define Prenom 1
#define Matricule 2
#define Annee 3
#define Mois 4
#define NIR 5
#define Statut 6
#define EmploiMetier 7
#define Grade 8
#define Indice 9
#define Service 10
#define NBI 11
#define QuotiteTrav 12
#define NbHeureTotal 13
#define NbHeureSup 14
#define MtBrut 15
#define MtNet 16
#define MtNetAPayer 17

 const char* Tags[] = {"Nom",
               "Prenom",
               "Matricule",
               "Annee",
               "Mois",
               "NIR",
               "Statut",
               "EmploiMetier",
               "Grade",
               "Indice",
               "Service",
               "NBI",
               "QuotiteTrav",
               "NbHeureTotal",
               "NbHeureSup",
               "MtBrut",
               "MtNet",
               "MtNetAPayer"};


xmlChar* annee_fichier = NULL;
xmlChar* mois_fichier = NULL;

typedef struct bulletin
{
    // 18 + lignes
    xmlChar **ligne;

} bulletin, *bulletinPtr;


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

bool test = (*cur != NULL &&!xmlStrcmp((*cur)->name,  (const xmlChar*) tag));

if (test)
  {
    bulletinIdent->ligne[l] = xmlGetProp(*cur, (const xmlChar *) "V");
    xmlChar* s = bulletinIdent->ligne[l];

    /* sanitisation */

    for (int i = 0; i < xmlStrlen(s); i++)
        if (s[i] == separateur) s[i] = '_';

    *cur = (*cur)? (*cur)->next: NULL;
  }

 return test;
}

static inline void _Bulletin(const int l, const char* tag, const bulletinPtr bulletinIdent, xmlNodePtr* cur)
{
    if (Bulletin(const int l, const char* tag, const bulletinPtr bulletinIdent, xmlNodePtr* cur))
    else
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
    if (Bulletin(const int l , const char* tag, const bulletinPtr bulletinIdent, xmlNodePtr* cur))
    else
    {
          bulletinIdent->ligne[l] = (xmlChar*) malloc(3*sizeof(xmlChar));
          if (bulletinIdent->ligne[l] == NULL) { perror("Erreur d'allocation de drapeau II."); exit(-64); }
          bulletinIdent->ligne[l][0] = 'N';
          bulletinIdent->ligne[l][1] = 'A';
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

static inline int lignePaye(xmlNodePtr cur, const bulletinPtr bulletinIdent, const char decimal)
{
    int start = sizeof(Tags)/sizeof(xmlChar*);
    int l = start, nbLignePaye = 0;
    unsigned int t = 0;

    while (cur != NULL)
    {
        if (xmlStrcmp(cur->name, (const xmlChar *) type_remuneration[t]))
        {
            t++;
            bulletinIdent->ligne[l] = (xmlChar*) malloc(2*sizeof(xmlChar));
            bulletinIdent->ligne[l][0] = t+1;  // +1 pour éviter la confusion avec \0 des chaines vides
            l++;

            l = 0;
            if (t == nbType)
            {
                fprintf(stderr, "En excès du nombre de types de lignes de paye autorisé (%d)\n", nbType);
                if (cur) fprintf(stderr, "Type litigieux %s aux alentours du matricule %s \n", cur->name, bulletinIdent->ligne[Matricule]);
                else fprintf(stderr, "%s", "Pointeur noeud courant nul\n");
                exit(-11);
            }
            continue;
        }

        verifier_taille(l);

        if (! xmlStrcmp(cur->name, (const xmlChar*) "Commentaire"))
        {
            return l;
        }

        DESCENDRE_UN_NIVEAU
        /* Libellé, obligatoire */

        cur = atteindreNoeud("Libelle", cur);

        _Bulletin(l, "Libelle", bulletinIdent, &cur);
        l++;
        /* Code, obligatoire */
        cur = atteindreNoeud("Code", cur);
        _Bulletin(l, "Code",  bulletinIdent, &cur);

        if (xmlStrcmp(bulletinIdent->ligne[l], (xmlChar*)NA_STRING))
            nbLignePaye++;

        l++;

        /* Base, si elle existe */

        _Bulletin_(l, "Base", bulletinIdent, &cur, decimal);
        l++;

        /* Taux, s'il existe */
        _Bulletin_(l, "Taux", bulletinIdent, &cur, decimal);
        l++;

        /* Nombre d'unités, s'il existe */
        _Bulletin_(l, "NbUnite", bulletinIdent, &cur, decimal);
        l++;

        /* Montant , obligatoire */
        cur = atteindreNoeud("Mt", cur);

        Bulletin_(l, "Mt", bulletinIdent, &cur, decimal);
        l++;

        REMONTER_UN_NIVEAU

        // Lorsque on a épuisé tous les types licites on a nécessairement cur = NULL
    }

    return nbLignePaye;
}


#define _BULLETIN(X) _Bulletin(X, Tags[X], bulletinIdent, &cur);

#define BULLETIN_(X)  Bulletin_(X, Tags[X], bulletinIdent, &cur, decimal);

#define _BULLETIN_(X)  _Bulletin_(X, Tags[X], bulletinIdent, &cur, decimal);

static uint64_t  parseBulletin(xmlNodePtr cur, const char decimal, bulletinPtr bulletinIdent)
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



    bulletinIdent->ligne[Annee] = annee_fichier;
    bulletinIdent->ligne[Mois] = mois_fichier;

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
    cur = atteindreNoeud("Indice", cur);
    _BULLETIN(Indice)

    cur = cur_save;
    cur = atteindreNoeud("Statut", cur);
    _BULLETIN(Statut)

    cur = cur_save;
    cur = atteindreNoeud("EmploiMetier", cur);

    _BULLETIN(EmploiMetier)

    cur = cur_save;
    cur = atteindreNoeud("Grade", cur);
    _BULLETIN(Grade)

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
            ligne = lignePaye(cur, bulletinIdent, decimal);
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

    return ligne;
}

static int32_t  parseFile(const char *filename,  bulletinPtr* Table, const char decimal, uint64_t* ligne, int32_t agent_total)
{
    xmlDocPtr doc;
    xmlNodePtr cur = NULL;
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
        cur = (cur)? cur->next : NULL;
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

            Sleep(1000);
#endif

            cur = atteindreNoeud("PayeIndivMensuel", cur);
            xmlNodePtr cur_save =  cur;

            DESCENDRE_UN_NIVEAU

            Table[agent_total]->ligne = (xmlChar**) malloc(MAX_LIGNES_PAYE* sizeof(xmlChar*));
            if (Table[agent_total]->ligne == NULL) { perror("Erreur d'allocation de drapeau I."); exit(-63); }

            *ligne += parseBulletin(cur, decimal, Table[agent_total]);

            // Ici il est normal que cur = NULL

            cur = cur_save->next;

            AFFICHER_NOEUD(cur->name)

            //Table[agent_total] = bulletinIdent;

            agent_total++;
            agent_du_fichier++;
        }

        cur = cur_save->next;

    }
    printf("Population du fichier %s : %4d bulletins    Total : %4d bulletins  %4" PRIu64 " lignes cumulées.\n", filename, agent_du_fichier, agent_total, *ligne);
    xmlFreeDoc(doc);
    // xmlCleanupParser();
    return(agent_total);
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

int32_t decoder_fichier(char** fichiers, int nbfichier, bulletinPtr* Table, const char decimal, uint64_t nbLigne)
{
    int32_t nbAgent  = 0;
    for (int i = 0; i < nbfichier ; i++)
    {
        fprintf(stderr, "Fichier: %s, %d/%d, nbLigne=%" PRIu64 "\n", fichiers[i], i+1, nbfichier, nbLigne);
        fflush(NULL);
        nbAgent = parseFile(fichiers[i], Table, decimal, &nbLigne, nbAgent);

        if (nbAgent < 0)
        {
            fprintf(stderr, "Erreur de décodage pour le fichier %s\n", fichiers[i]);
            return -1;
        }

    }
    return nbAgent;
}

typedef struct
{
    pthread_t thread_id;
    int       thread_num;
    char** argv;
    int argc;

} thread_info;

char decimal = '.';
char separateur = ',';
int32_t nbAgent = MAX_NB_AGENTS;

void* launch(void* info)
{
    uint64_t nbLigne = 0;
    thread_info* tinfo = (thread_info*) info;
    bulletinPtr* Table = (bulletinPtr*) malloc(nbAgent*tinfo->argc*sizeof(bulletinPtr));
    if (Table == NULL)
    {
        perror("Mémoire insuffisante");
        exit(-18);
    }

    for (int i=0; i < nbAgent*tinfo->argc; i++)
    {
        Table[i] = (bulletinPtr) malloc(sizeof(bulletin));
        if (Table[i] == NULL)
        {
            perror("Mémoire insuffisante");
            exit(-19);
        }

    }

    // LIBXML_TEST_VERSION
    xmlKeepBlanksDefault(0);

    int32_t nbAgent = decoder_fichier(tinfo->argv, tinfo->argc, Table, decimal, nbLigne);

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

    while (start < argc)
    {
        if (! strcmp(argv[start], "-n"))
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
                    nbAgent  = (uint32_t)sl;
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
            printf("-n nombre de bulletins mensuels attendus [défaut %d].\n", MAX_NB_AGENTS);
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

    uint64_t memoire_reservee = nbAgent*(argc-start)*sizeof(bulletin);
    if (memoire_reservee > MAX_MEMOIRE_RESERVEE)
    {
        fprintf(stderr, "Quantité de mémoire réservée %" PRIu64 " supérieure au maximum de %" PRIu64 " octets.\nAppliquer le programme sur une partie des fichiers et fusionner les bases en résultant.\n", memoire_reservee, MAX_MEMOIRE_RESERVEE);
        exit(-500);
    }

    if (afficher_memoire_reservee) fprintf(stderr, "Quantité de mémoire réservée %" PRIu64 " octets.\n", memoire_reservee);

    if (nbfil == 0 || (argc -start < 2))
    {
        thread_info tinfo;
        tinfo.argc = argc -start;
        tinfo.argv = (char**) malloc((argc -start)* sizeof(char*));
        for (int j=start; j < argc; j++) tinfo.argv[j-start] = strdup(argv[j]);
        launch((void*) &tinfo);
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

        thread_info tinfo[nbfil];

        puts("Creation des fils clients.\n");
        for (int i = 0; i < nbfil; i++)
        {
            tinfo[i].thread_num = i;
            tinfo[i].argc = (argc - start < nbfichier_par_fil)? argc - start: nbfichier_par_fil;
            tinfo[i].argv = (char**) malloc(nbfichier_par_fil * sizeof(char*));

            for (int j = 0; j <  nbfichier_par_fil && start + j < argc; j++)
            {
                tinfo[i].argv[j] = strdup(argv[start + j]);
            }

            start += nbfichier_par_fil;

            int ret = pthread_create (
                          &thread_clients[i],
                          NULL,
                          launch,
                          &tinfo[i]
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


    uint64_t nbLigneBase=0;

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

    if (liberer_memoire)
        for (int i = 0; i < nbAgent; i++)
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

