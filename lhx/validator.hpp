#ifndef VALIDATOR_HPP_INCLUDED
#define VALIDATOR_HPP_INCLUDED

/* CETTE PAGE DOIT RESTER ENCODEE EN UTF-8 */

#include <string>
#include <cstdlib>
#include <cstdint>
#include <cstdbool>
#include <climits>
#include <clocale>
#include <cerrno>
#include <cmath>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <cinttypes>
#include <vector>

#ifdef  __GNUG__
#define GCC_INLINE //__attribute__((always_inline))
#define GCC_UNUSED __attribute__((__unused__))
#else
#define GCC_INLINE
#define GCC_UNUSED
#endif

typedef struct
{
    int      thread_num;
    char**   argv;
    unsigned argc;
} thread_t;

#define EXPRESSION_REG_ELUS "^maire.*|^pr..?sident.*|^[eé]lus?|.*(?:\\badj.*\\bmaire\\b|\\bv.*\\bpr..?sident\\b|\\bcons.*\\bmuni|\\bcons.*\\bcomm|\\bcons.*\\bd..?l..?gu).*"
#define EXPRESSION_REG_VACATIONS ".*\\bvacat.*|.*\\bvac\\.?\\b.*"  // vac.* peut être vérifié par 'vacances'
#define EXPRESSION_REG_ASSISTANTES_MATERNELLES ".*\\bass.*\\bmater.*"

#define NOM_BASE "Table"
#define NOM_BASE_BULLETINS "Bulletins"
#define CSV  ".csv"

/* Les définitions ci-après doivent être négatives */


#define MONOLITHIQUE -1
#define PAR_ANNEE -2

#define PAR_TRAITEMENT   -3
#define PAR_INDEMNITE_RESIDENCE -4
#define PAR_SFT          -5
#define PAR_AVANTAGE_NATURE -6
#define PAR_INDEMNITE    -7
#define PAR_REM_DIVERSES -8
#define PAR_DEDUCTION    -9
#define PAR_ACOMPTE      -10
#define PAR_RAPPEL       -11
#define PAR_RETENUE      -12
#define PAR_COTISATION   -13

#define TOUTES_CATEGORIES -14



#define INDEX_MAX_CONNNES 5    // nombre de type de champ de ligne de paye (Libellé, Code, Taux, Base, ...) moins 1.
#define BESOIN_MEMOIRE_ENTETE  22  /* nb d'éléments de l'enum ci-dessous */
typedef enum {Annee, Mois, Budget, Etablissement, Siret, Nom, Prenom, Matricule, NIR, NbEnfants,
              Statut, EmploiMetier, Grade, Indice,
              Service, NBI, QuotiteTrav, NbHeureTotal,
              NbHeureSup, MtBrut, MtNet, MtNetAPayer
         } Entete;

constexpr const char* Tableau_entete[] = {"Annee", "Mois", "Budget", "Etablissement", "Siret", "Nom", "Prenom", "Matricule", "NIR", "NbEnfants",
                                    "Statut", "EmploiMetier", "Grade", "Indice",
                                     "Service", "NBI", "QuotiteTrav", "NbHeureTotal",
                                      "NbHeureSup", "MtBrut", "MtNet", "MtNetAPayer"};

    // 18 + lignes


typedef struct
{
    xmlChar*** Table;
    uint64_t nbLigne;
    uint32_t  *NAgent;
    uint32_t nbAgentUtilisateur;
    uint32_t NCumAgent;
    uint32_t NCumAgentXml;
    int32_t  taille_base;
    std::vector<uint16_t> NLigne;
    thread_t* threads;
    std::string chemin_log;
    std::string expression_reg_elus;
    std::string chemin_base;
    std::string chemin_bulletins;
    uint16_t nbLigneUtilisateur;
    uint16_t fichier_courant;
    char decimal;
    char separateur;
    bool reduire_consommation_memoire;
    bool drapeau_cont;
    bool calculer_maxima;
    bool generer_rang;
    bool select_siret;
    int nbfil;
    int* Memoire_p_ligne;
} info_t;

#define RESET true
#define RETRY -1
#define SKIP_FILE 1
#define NO_AGENT -1


#ifndef NA_STRING
 #define NA_STRING  (xmlChar*) "NA"
#endif
#ifndef MAX_LIGNES_PAYE
 #define MAX_LIGNES_PAYE 1000
#endif
#ifndef MAX_NB_AGENTS
 #define MAX_NB_AGENTS 8000
#endif

#ifndef NO_DEBUG
    #define DEBUG(X) std::cerr << "\n" << X << "\n";
    #define AFFICHER_NOEUD(X)       { char msg[50]={0}; \
                                      sprintf(msg, "atteint %s\n", (const char*) X);\
                                      DEBUG(msg) }

    #define NO_DEBUG 0
#else
    #undef NO_DEBUG
    #define NO_DEBUG 1
    #define DEBUG(X)
    #define AFFICHER_NOEUD(X)
#endif
#if !NO_DEBUG
    #ifdef __WIN32__
    #include <Windows.h>
    #endif
#endif

#define NODE_FOUND  0
#define NODE_NOT_FOUND 1
#define LINE_MEMORY_EXCEPTION 2
#define NO_NEXT_ITEM 3

/* pas de contrôle d'existence de noeud : version affaiblie de la macro précédente */



static const char* type_remuneration[]   = {
                                            "TraitBrut",
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
                                            "Commentaire"
                                            };


static const int nbType                  = sizeof(type_remuneration)/sizeof(char*);


static const xmlChar drapeau[][2]  = {{1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0}, {9,0}, {10,0}, {11,0}, {12,0}};



void* decoder_fichier(info_t& tinfo);



#endif // VALIDATOR_HPP_INCLUDED
