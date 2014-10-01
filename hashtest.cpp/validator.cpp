
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

#define DEBUG(X) fprintf(stderr, "%s\n", X);

#define SAUTER_UN_NOEUD cur = (cur)? cur->next: NULL;  //cur = (cur)? cur->next: NULL;

#define BULLETIN(X) if (cur != NULL && !xmlStrcmp(cur->name, (const xmlChar *) #X)) { \
                          bulletinIdent->X = xmlGetProp(cur, (const xmlChar *) "V"); \
                          SAUTER_UN_NOEUD \
                     } else { \
                            bulletinIdent->X = NULL; \

#define _BULLETIN(X)      BULLETIN(X) \
                            if (cur) \
                              fprintf(stderr, "Trouvé %s au lieu de " #X "\n", cur->name);   \
                            else \
                              fprintf(stderr, "%s\n", "Noeud courant null au stade de la vérification de " #X ); \
                              SAUTER_UN_NOEUD \
                          }


/* pas de contrôle d'existence de noeud : version affaiblie de la macro précédente */

#define _BULLETIN_(X)     BULLETIN(X) }


#define DESCENDRE_UN_NIVEAU    cur = (cur)? cur->xmlChildrenNode: NULL;  // cur = (cur)? cur-> next: NULL;

#define REMONTER_UN_NIVEAU     cur = (cur)? cur->parent: NULL;   SAUTER_UN_NOEUD

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
    xmlChar *ligne[MAX_LIGNES_PAYE][5] = {{NULL}};

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
          char msg[50]={0};
          sprintf(msg, "atteint %s\n", noeud);
          DEBUG(msg)
          break;
      }
    }

    return cur;
}

inline xmlNodePtr  lignePaye(const char* type, xmlNodePtr cur, bulletinPtr bulletinIdent)
{
     static int j;

     while (j < MAX_LIGNES_PAYE && cur != NULL && !xmlStrcmp(cur->name, (const xmlChar *) type))
       {
            DESCENDRE_UN_NIVEAU
                /* Libellé */
                _BULLETIN(ligne[j][0])
                /* Code */
                _BULLETIN(ligne[j][1])
                /* Taux, s'il existe */
                _BULLETIN_(ligne[j][2])
                /* Nombre d'unités, s'il existe */
                _BULLETIN_(ligne[j][3])
                /* Montant */
                _BULLETIN(ligne[j][4])
           REMONTER_UN_NIVEAU
           j++;
       }

       return cur;
}

static xmlNodePtr parseBulletin(xmlNodePtr cur)
{
    DEBUG("Parsage")

    while (cur != NULL)
    {
        cur = atteindreNoeud((const char*) "Agent", cur);
        if (cur != NULL)
        {
              bulletinIdent = (bulletinPtr) calloc(1, sizeof(bulletin));
              if (bulletinIdent == NULL)
                {
                    fprintf(stderr,"Pas assez de mémoire\n");
                    return(NULL);
                }

              DESCENDRE_UN_NIVEAU

                  SAUTER_UN_NOEUD
                  _BULLETIN(Nom)
                  _BULLETIN(Prenom)
                  _BULLETIN(Matricule)
                  _BULLETIN(NIR)
                  SAUTER_UN_NOEUD
                  _BULLETIN(Statut)
                  _BULLETIN(EmploiMetier)
                  _BULLETIN(Grade)
                  SAUTER_UN_NOEUD
                  _BULLETIN(Indice)

             REMONTER_UN_NIVEAU

        } else {

            fprintf(stderr, "Trouvé %s au lieu de \"Agent\"", cur->name);
            return(NULL);
        }

        _BULLETIN(Service)
        _BULLETIN(NBI)
        _BULLETIN(QuotiteTrav)
        SAUTER_UN_NOEUD

        if (!xmlStrcmp(cur->name, (const xmlChar *)"Remuneration"))
        {
            DESCENDRE_UN_NIVEAU

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

            REMONTER_UN_NIVEAU

      }  else  {
          fprintf(stderr, "Trouvé %s au lieu de \"Remuneration\"", cur->name);
          return NULL;
      }

     _BULLETIN(NbHeureTotal)
     _BULLETIN(NbHeureSup)
     _BULLETIN(MtBrut)
     _BULLETIN(MtNet)
     _BULLETIN(MtNetAPayer)

    }

  return(cur);
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

      DESCENDRE_UN_NIVEAU

      cur = parseBulletin(cur);

      if (cur == NULL) return -1;

      REMONTER_UN_NIVEAU

      cur = cur->next;

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

