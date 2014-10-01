
/**
 * section: xmlReader
 * synopsis: Parse and validate an XML file with an xmlReader
 * purpose: Demonstrate the use of xmlReaderForFile() to parse an XML file
 *          validating the content in the process and activating options
 *          like entities substitution, and DTD attributes defaulting.
 *          (Note that the XMLReader functions require libxml2 version later
 *          than 2.6.)
 * usage: reader2 <valid_xml_filename>
 * test: reader2 test2.xml > reader1.tmp && diff reader1.tmp $(srcdir)/reader1.res
 * author: Daniel Veillard
 * copy: see Copyright for the status of this software.
 */


#ifdef __cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

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
    xmlChar *ligne[MAX_LIGNES_PAYE][6] = {{NULL}};

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

inline xmlNodePtr  lignePaye(const char* type, xmlNodePtr cur, bulletinPtr bulletinIdent)
{
    static int j;
        fprintf(stderr, "LP---%s, j=%d, cur is null=%d, \n", type, j, (cur==NULL));
        if (cur != NULL && xmlStrcmp(cur->name, (const xmlChar *) type))
        {
            fprintf(stderr, "%s %s but %s\n", "cur->name is not of type", type, cur->name);
            cur=cur->next;
            if (cur != NULL && xmlStrcmp(cur->name, (const xmlChar *) type))
             {
                fprintf(stderr, "%s %s but %s\n", "cur->name is not of type", type, cur->name);
                cur=cur->next;
                // exit(0);
             } else
                if (cur) fprintf(stderr, "%s %s \n", "cur->name is NOW of type", type);

           // exit(0);
        }
    while (j < MAX_LIGNES_PAYE && cur != NULL && !xmlStrcmp(cur->name, (const xmlChar *) type))
    {
        DESCENDRE_UN_NIVEAU
        /* Libellé, obligatoire */

        cur = atteindreNoeud("Libelle", cur);
        fprintf(stderr, "---%s\n", cur->name);

        _Bulletin(ligne[j][0], Libelle)
        /* Code, obligatoire */
        cur = atteindreNoeud("Code", cur);
        _Bulletin(ligne[j][1], Code)
        /* Base, si elle existe */
        _Bulletin_(ligne[j][2], Base)
        /* Taux, s'il existe */
        _Bulletin_(ligne[j][3], Taux)
        /* Nombre d'unités, s'il existe */
        _Bulletin_(ligne[j][4], NbUnite)
        /* Montant , obligatoire */
        cur = atteindreNoeud("Mt", cur);
        _Bulletin(ligne[j][5], Mt)
        REMONTER_UN_NIVEAU
        j++;
    }

    return cur;
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

        // certains noeuds <Remuneration/> sont vides

        if (cur)
        {

            /* Traitement Brut */

            cur = lignePaye((const char*) "TraitBrut", cur, bulletinIdent);

            /* Supplément familial */

            cur = lignePaye((const char*)  "SupFam", cur, bulletinIdent);

            /* Indemnités */

            cur = lignePaye((const char*)  "Indemnite", cur, bulletinIdent);

            /* Déduction */

            cur = lignePaye((const char*) "Deduction", cur, bulletinIdent);

            /* Rappel */

            cur = lignePaye((const char*) "Rappel", cur, bulletinIdent);

            /* Retenue */

            cur = lignePaye("Retenue", cur, bulletinIdent);

            /* Cotisation */

            cur = lignePaye("Cotisation", cur, bulletinIdent);

            // A présent cur= NULL est normal

        }

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


static int  parseFile(const char *filename, uint16_t nbAgents)
{
    xmlDocPtr doc;
    xmlNodePtr cur = NULL;
    static uint16_t agent_total;
    uint16_t agent_du_fichier = 0;

    bulletinPtr Table[nbAgents];
    memset(Table, 0, nbAgents*sizeof(bulletinPtr));

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

    printf("Population du fichier %s : %4d agents    Total : %4d agents.\n", filename, agent_du_fichier, agent_total);

    xmlFreeDoc(doc);
    return(0);
}


int main(int argc, char **argv)
{
    LIBXML_TEST_VERSION
    xmlKeepBlanksDefault(0);
    uint16_t nbAgents = MAX_NB_AGENTS;
    int start = 1;

    if (argc < 2)
    {
        fprintf(stderr, "%s\n", "Il faut au moins un fichier à analyser.");
        return -2;
    }

    if (!strcmp(argv[1], "-n"))
    {
        if (argc > 2)
        {
            nbAgents = atoi(argv[2]);
            start += 2;
        }
        else
        {
            fprintf(stderr, "%s\n", "Préciser le nombre d'agents approximativement à 20 % près.");
            return -3;
        }
    }

    for (int i = start; i < argc ; i++)
    {
        int ret = parseFile(argv[i], nbAgents);

        if (ret == -1)
        {
            fprintf( stderr, "Erreur de décodage pour le fichier %s\n", argv[i]);
            return -1;
        }
    }
    /* Clean up everything else before quitting. */
    xmlCleanupParser();
    return(0);
}

#ifdef __cplusplus
}
#endif

