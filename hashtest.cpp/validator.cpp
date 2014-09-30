
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
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#ifndef MAX_LIGNES_PAYE
 #define MAX_LIGNES_PAYE 300
#endif

#endif // MAX_LIGNES_PAYE

#define SAUTER_UN_NOEUD cur = (cur)? cur->next: NULL;  cur = (cur)? cur->next: NULL;

#define BULLETIN(X) if (cur != NULL && !xmlStrcmp(cur->name, (const xmlChar *) #X)) { \
                          bulletinIdent->##X = xmlGetProp(cur, "V"); \
                          SAUTER_UN_NOEUD \
                     } else { \
                            bulletinIdent->##X = NULL; \

#define _BULLETIN(X)      BULLETIN(X) \
                            if (cur) \
                              fprintf(stderr, "Trouvé %s au lieu de " #X "\n", cur->name);   \
                            else \
                              fprintf(stderr, "%s\n", "Noeud courant null au stade de la vérification de " #X ); \
                              SAUTER_UN_NOEUD \
                          }


/* pas de contrôle d'existence de noeud : version affaiblie de la macro précédente */

#define _BULLETIN_(X)     BULLETIN(X) }


#define DESCENDRE_UN_NIVEAU    cur = (cur)? cur->xmlChildrenNode: NULL;   cur = (cur)? cur-> next: NULL;

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
    xmlChar *Indice;
    xmlChar *QuotiteTrav;
    xmlChar *NbHeureTotal;
    xmlChar *NbHeureSup;
    xmlChar *MtBrut;
    xmlChar *MtNet;
    xmlChar *MtNetAPayer;
    xmlChar *ligne[MAX_LIGNES_PAYE][5] = {{NULL}};

} bulletin, *bulletinPtr;

inline xmlNodePtr  lignePaye(char* type, xmlNodePtr cur, bulletinPtr bulletinIdent)
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
    bulletinPtr bulletinIdent = NULL;

    while (cur != NULL)
    {
        cur = atteindreNoeud("Agent", cur);
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

                cur = lignePaye("TraitBrut", cur, bulletinIdent);

                /* Supplément familial */

                cur = lignePaye("SupFam", cur, bulletinIdent);

                /* Indemnités */

                cur = lignePaye("Indemnite", cur, bulletinIdent);

                /* Déduction */

                cur = lignePaye("Deduction", cur, bulletinIdent);

                /* Rappel */

                cur = lignePaye("Rappel", cur, bulletinIdent);

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

inline xmlNodePtr atteindreNoeud(char* noeud, xmlNodePtr cur)
{
    while (cur && xmlIsBlankNode(cur))
    {
        cur = cur -> next;
    }

    while (cur != NULL)
    {
     if (xmlStrcmp(cur->name, (const xmlChar *) noeud)
      {
        cur = cur->next;
      }
    }

    return cur;
}

static xmlNodePtr  parseFile(char *filename)
{
    xmlDocPtr doc;
    xmlNodePtr cur = NULL;
    cur = xmlDocGetRootElement(doc);

    if (cur == NULL)
    {
        fprintf(stderr,"document vide\n");
        xmlFreeDoc(doc);
        return NULL;
    }

    DESCENDRE_UN_NIVEAU

    cur = atteindreNoeud("DonneesIndiv");

    DESCENDRE_UN_NIVEAU

    while(cur != NULL)
    {
      cur = atteindreNoeud("PayeIndivMensuel");

      DESCENDRE_UN_NIVEAU

      cur = parseBulletin(cur)

      REMONTER_UN_NIVEAU

      cur = cur->next;
    }

    return(cur);
}

static void handleGjob(gJobPtr cur)
{
    int i;

    /*
     * Do whatever you want and free the structure.
     */
    printf("%d Jobs registered\n", cur->nbJobs);
    for (i = 0; i < cur->nbJobs; i++) printJob(cur->jobs[i]);
}

int main(int argc, char **argv)
{
    int i;
    gJobPtr cur;

    LIBXML_TEST_VERSION
    xmlKeepBlanksDefault(0);

    for (i = 1; i < argc ; i++)
    {
        cur = parseGjobFile(argv[i]);

        if ( cur )
            handleGjob(cur);
        else
            fprintf( stderr, "Error parsing file '%s'\n", argv[i]);

    }

    /* Clean up everything else before quitting. */
    xmlCleanupParser();

    return(0);
}

#ifdef __cplusplus
}
#endif

