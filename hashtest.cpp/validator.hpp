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

#define BESOIN_MEMOIRE_ENTETE  16  /* nb d'éléments de l'enum ci-dessous */
typedef enum {Nom, Prenom, Matricule, NIR, Statut, EmploiMetier, Grade, Indice,
          Service, NBI, QuotiteTrav, NbHeureTotal, NbHeureSup, MtBrut, MtNet, MtNetAPayer
         } Entete;

    // 18 + lignes


typedef struct
{
    xmlChar*** Table;
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
    char decimal;
    char separateur;
    bool reduire_consommation_memoire;
    uint8_t besoin_memoire_par_ligne;
    char* chemin_log;
    int nbfil;
} info_t;



#ifndef NA_STRING
 #define NA_STRING "NA"
#endif
#ifndef MAX_LIGNES_PAYE
 #define MAX_LIGNES_PAYE 300
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

/* pas de contrôle d'existence de noeud : version affaiblie de la macro précédente */


#define DESCENDRE_UN_NIVEAU    cur = (cur)? cur->xmlChildrenNode: NULL;  if ((! NO_DEBUG) && cur) fprintf(stderr, "Descente au niveau %s\n", cur->name);  // cur = (cur)? cur-> next: NULL;

#define REMONTER_UN_NIVEAU     cur = (cur)? cur->parent: NULL;   if ((! NO_DEBUG) && cur) fprintf(stderr, "Remontée au niveau %s\n", cur->name); cur = (cur)? cur->next: NULL;

static const char* type_remuneration[]   = {"TraitBrut",
                                            "IndemResid",
                                            "SupFam",
                                            "AvantageNature",
                                            "Indemnite",
                                            "RemDivers",
                                            "Deduction",
                                            "Acompte",
                                            "Rappel",
                                            "Retenue",
                                            "Cotisation",
                                            "Commentaire"};

static const char* type_remuneration_traduit[] = {"Traitement",
                                                   u8"Indemnité de résidence",
                                                   u8"Supplément familial",
                                                   "Avantage en nature",
                                                   u8"Indemnité",
                                                   u8"Autres rémunérations",
                                                   u8"Déduction",
                                                   "Acompte",
                                                   "Rappel",
                                                   "Retenue",
                                                   "Cotisation",
                                                   "Commentaire"};

static const int nbType                  = sizeof(type_remuneration)/sizeof(char*);
static const xmlChar drapeau[nbType][2]  = {{1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0}, {9,0}, {10,0}, {11,0}, {12,0}};

static const char* entete_char[]={u8"Année", "Mois", "Nom", u8"Prénom", "Matricule", "Service", "Statut", u8"Temps.de.travail",
                                  "Heures.Sup.", "Heures", "Indice", "Brut", "Net", u8"Net.à.Payer", "NBI", u8"Libellé", "Code",
                                  "Base", "Taux", u8"Nb.Unité", "Montant", "Type", "Emploi", "Grade", "Nir"};

void* decoder_fichier(void* tinfo);

#endif // VALIDATOR_HPP_INCLUDED
