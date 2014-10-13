
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
 #define MAX_LIGNES_PAYE 40
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

#define SAUTER_UN_NOEUD cur = (cur)? cur->next: NULL;  //cur = (cur)? cur->next: NULL;

#define Bulletin(X, Y) if (cur != NULL && !xmlStrcmp(cur->name, (const xmlChar *) #Y)) { \
                          bulletinIdent->X = xmlGetProp(cur, (const xmlChar *) "V"); \
                          SAUTER_UN_NOEUD \
                     } else { \
                            bulletinIdent->X = (xmlChar*) NA_STRING; \

#define _SUB_DEC_SEP(X) { xmlChar* s = xmlStrdup(bulletinIdent->X); \
                                            if (s == NULL) exit(EXIT_FAILURE); \
                                            for (int i = 0; i < xmlStrlen(s); i++) if (s[i] == '.') s[i] = decimal; \
                                            xmlFree(bulletinIdent->X); \
                                            bulletinIdent->X = s; }


#define _Bulletin(X, Y)   Bulletin(X, Y) \
                          if (cur) \
                              fprintf(stderr, "Trouvé %s au lieu de " #X "\n", cur->name);   \
                            else \
                              fprintf(stderr, "%s pour %s\n", "Noeud courant null au stade de la vérification de " #X, bulletinIdent->Matricule); \
                              SAUTER_UN_NOEUD \
                          }

#define Bulletin_(X, Y)  _Bulletin(X, Y) \
                          _SUB_DEC_SEP(X)

#define _Bulletin_(X, Y)  if (cur != NULL && !xmlStrcmp(cur->name, (const xmlChar *) #Y)) { \
                             bulletinIdent->X = xmlGetProp(cur, (const xmlChar *) "V"); \
                             SAUTER_UN_NOEUD \
                          } else { \
                             bulletinIdent->X = (xmlChar*) NA_STRING; }


#define _BULLETIN(X)      _Bulletin(X, X)

/* pas de contrôle d'existence de noeud : version affaiblie de la macro précédente */


#define _BULLETIN_SUB_DEC_SEP(X)  _BULLETIN(X) \
                                   _SUB_DEC_SEP(X)

#define _BULLETIN__SUB_DEC_SEP(X)  _Bulletin_(X, X)

#define DESCENDRE_UN_NIVEAU    cur = (cur)? cur->xmlChildrenNode: NULL;  if ((! NO_DEBUG) && cur) fprintf(stderr, "Descente au niveau %s\n", cur->name);  // cur = (cur)? cur-> next: NULL;

#define REMONTER_UN_NIVEAU     cur = (cur)? cur->parent: NULL;   if ((! NO_DEBUG) && cur) fprintf(stderr, "Remontée au niveau %s\n", cur->name); SAUTER_UN_NOEUD

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
    xmlChar ****ligne;  // type de ligne de paye x rang de la ligne x {Libellé, ..., Montant}

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



static inline int lignePaye(xmlNodePtr cur, bulletinPtr bulletinIdent, const char decimal)
{
    int l = 0, nbLignePaye = 0;
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
                if (cur) fprintf(stderr, "Type litigieux %s aux alentours du matricule %s \n", cur->name, bulletinIdent->Matricule);
                else fprintf(stderr, "%s", "Pointeur noeud courant nul\n");
                exit(-11);
            }
            continue;
        }

        if (! xmlStrcmp(cur->name, (const xmlChar*) "Commentaire"))
        {
            return l;
        }

        DESCENDRE_UN_NIVEAU
        /* Libellé, obligatoire */

        cur = atteindreNoeud("Libelle", cur);

        _Bulletin(ligne[t][l][0], Libelle)
        /* Code, obligatoire */
        cur = atteindreNoeud("Code", cur);
        _Bulletin(ligne[t][l][1], Code)

         if (xmlStrcmp(bulletinIdent->ligne[t][l][1], (xmlChar*)NA_STRING))
            nbLignePaye++;

        /* Base, si elle existe */
        _Bulletin_(ligne[t][l][2], Base)
        /* Taux, s'il existe */
        _Bulletin_(ligne[t][l][3], Taux)
        /* Nombre d'unités, s'il existe */
        _Bulletin_(ligne[t][l][4], NbUnite)
        /* Montant , obligatoire */
        cur = atteindreNoeud("Mt", cur);
        Bulletin_(ligne[t][l][5], Mt)

        REMONTER_UN_NIVEAU

        l++;

        if (l == MAX_LIGNES_PAYE)
        {
            fprintf(stderr, "En excès du nombre de lignes de paye autorisé (%d)\n", MAX_LIGNES_PAYE);
            exit(-10);
        }
        // Lorsque on a épuisé tous les types licites on a nécessairement cur = NULL
    }

   return nbLignePaye;
}

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

    for (int i = 0; i < nbType-1; i++)
       for (int j = 0; j < MAX_LIGNES_PAYE; j++)
          for (int k = 0; k < 6; k++)
            bulletinIdent->ligne[i][j][k] = (xmlChar *) NA_STRING;

    bulletinIdent->Annee = annee_fichier;
    bulletinIdent->Mois = mois_fichier;

    DESCENDRE_UN_NIVEAU

    SAUTER_UN_NOEUD
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
    _BULLETIN_SUB_DEC_SEP(QuotiteTrav)

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

    _BULLETIN__SUB_DEC_SEP(NbHeureTotal)
    cur = atteindreNoeud("NbHeureSup", cur);
    _BULLETIN_SUB_DEC_SEP(NbHeureSup)
    _BULLETIN_SUB_DEC_SEP(MtBrut)
    _BULLETIN_SUB_DEC_SEP(MtNet)
    _BULLETIN_SUB_DEC_SEP(MtNetAPayer)

    return ligne;
}

static int32_t  parseFile(const char *filename,  bulletinPtr* Table, const char decimal, uint64_t* ligne)
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

        bulletinPtr bulletinIdent = NULL;
        bulletinIdent = (bulletinPtr) calloc(1, sizeof(bulletin));
        if (bulletinIdent == NULL)
        {
            fprintf(stderr,"Pas assez de mémoire\n");
            return 0;
        }

        *ligne += parseBulletin(cur, decimal, bulletinIdent);

        // Ici il est normal que cur = NULL

        cur = cur_save->next;

        AFFICHER_NOEUD(cur->name)

        Table[agent_total] = bulletinIdent;

        agent_total++;
        agent_du_fichier++;
    }

    cur = cur_save->next;

}
    printf("Population du fichier %s : %4d bulletins    Total : %4d bulletins  %4" PRIu64 " lignes cumulées.\n", filename, agent_du_fichier, agent_total, *ligne);
    xmlFreeDoc(doc);
   // xmlCleanupParser();
    return((int32_t) agent_total);
}

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


int32_t decoder_fichier(char** fichiers, int nbfichier, bulletinPtr* Table, const char decimal, uint64_t nbLigne)
{
  int32_t nbAgent  = 0;
    for (int i = 0; i < nbfichier ; i++)
    {
        fprintf(stderr, "Fichier: %s, %d/%d, nbLigne=%" PRIu64 "\n", fichiers[i], i+1, nbfichier, nbLigne);
        fflush(NULL);
        nbAgent = parseFile(fichiers[i], Table, decimal, &nbLigne);

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
    if (Table == NULL) { perror("Mémoire insuffisante"); exit(-18); }

    for (int i=0; i < nbAgent*tinfo->argc; i++)
    {
        Table[i] = (bulletinPtr) malloc(sizeof(bulletin));
        if (Table[i] == NULL) { perror("Mémoire insuffisante"); exit(-19); }

        Table[i]->ligne = (xmlChar****) malloc((nbType-1)*sizeof(xmlChar***));
        if (Table[i]->ligne == NULL) { perror("Mémoire insuffisante"); exit(-20); }
        for (int j=0; j < nbType-1; j++)
        {
          Table[i]->ligne[j] = (xmlChar***) malloc(MAX_LIGNES_PAYE*sizeof(xmlChar**));
          if (Table[i]->ligne[j] == NULL) { perror("Mémoire insuffisante"); exit(-21); }
          for (int k=0; k < MAX_LIGNES_PAYE; k++)
           {
             Table[i]->ligne[j][k] = (xmlChar**) malloc(6*sizeof(xmlChar*));
             if (Table[i]->ligne[j][k] ==  NULL) { perror("Mémoire insuffisante"); exit(-22); }
           }
        }
    }

    //memset(Table, 0, sizeof(Table));
    //xmlInitParser();
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

