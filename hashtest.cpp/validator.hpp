#ifndef VALIDATOR_HPP_INCLUDED
#define VALIDATOR_HPP_INCLUDED

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


typedef struct
{
    pthread_t thread_id;
    int       thread_num;
    char** argv;
    int argc;

} thread_t;

#define BESOIN_MEMOIRE_ENTETE  18  /* nb d'éléments de l'enum ci-dessous */
typedef enum {Nom, Prenom, Matricule, Annee, Mois, NIR, Statut, EmploiMetier, Grade, Indice,
          Service, NBI, QuotiteTrav, NbHeureTotal, NbHeureSup, MtBrut, MtNet, MtNetAPayer
         } Entete;

typedef struct bulletin
{
    // 18 + lignes
    xmlChar **ligne;

} bulletin, *bulletinPtr;



typedef struct
{
    bulletinPtr* Table;
    uint64_t nbLigne;
    int32_t  *NAgent;
    uint32_t nbAgentUtilisateur;
    uint32_t NCumAgent;
    uint32_t NCumAgentLibxml2;
    uint16_t *NLigne;
    uint16_t fichier_courant;
    //uint32_t agent_courant;
    thread_t* threads;
    xmlChar*  annee_fichier;
    xmlChar* mois_fichier;
    char** fichiers;
    char decimal;
    char separateur;
    bool reduire_consommation_memoire;
    uint8_t besoin_memoire_par_ligne;
    char* chemin_log;

} info_t;


#define nbType 12
#ifndef NA_STRING
 #define NA_STRING "NA"
#endif
#ifndef MAX_LIGNES_PAYE
 #define MAX_LIGNES_PAYE 600
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
#ifdef __WIN32__
#include <Windows.h>
#endif
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

static const char* type_remuneration[nbType] = {"TraitBrut", "IndemResid", "SupFam", "AvantageNature", "Indemnite", "RemDivers", "Deduction", "Acompte", "Rappel", "Retenue", "Cotisation", "Commentaire"};
static const char* type_remuneration_traduit[nbType] = {"Traitement", u8"Indemnité de résidence", u8"Supplément familial", "Avantage en nature", u8"Indemnité", u8"Autres rémunérations", u8"Déduction", "Acompte", "Rappel", "Retenue", "Cotisation", "Commentaire"};


#endif // VALIDATOR_HPP_INCLUDED
