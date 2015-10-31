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

#ifndef GUI_TAG_MESSAGES
  #define  STATE_HTML_TAG  "[MSG]"
  #define  PROCESSING_HTML_TAG  "[MSG]"
  #define  ERROR_HTML_TAG  "[ERR]"
  #define  WARNING_HTML_TAG  "[ATT]"
#endif

typedef struct
{
    int      thread_num;
    char**   argv;
    unsigned argc;
} thread_t;

static constexpr auto EXPRESSION_REG_ELUS = "^maire.*|^pr..?sident.*|^[eé]lus?|.*(?:\\badj.*\\bmaire\\b|\\bv.*\\bpr..?sident\\b|\\bcons.*\\bmuni|\\bcons.*\\bcomm|\\bcons.*\\bd..?l..?gu).*",
                      EXPRESSION_REG_VACATIONS = ".*\\bvacat.*|.*\\bvac\\.?\\b.*",                 // vac.* peut être vérifié par 'vacances'
                      EXPRESSION_REG_ASSISTANTES_MATERNELLES = ".*\\bass.*\\bmater.*",
                      NOM_BASE = "Table",
                      NOM_BASE_BULLETINS = "Bulletins",
                      CSV = ".csv";

/* Les définitions ci-après doivent être négatives */

enum class BaseCategorie : int {
                                 BASE = 0,
                                 BULLETINS = 1
                               };

enum class BaseType : int
                  {
                    MONOLITHIQUE        = -1,
                    PAR_ANNEE           = -2,
                    PAR_TRAITEMENT      =  -3,
                    PAR_INDEMNITE_RESIDENCE = -4,
                    PAR_SFT             = -5,
                    PAR_AVANTAGE_NATURE = -6,
                    PAR_INDEMNITE       = -7,
                    PAR_REM_DIVERSES    = -8,
                    PAR_DEDUCTION       = -9,
                    PAR_ACOMPTE         = -10,
                    PAR_RAPPEL          = -11,
                    PAR_RETENUE         = -12,
                    PAR_COTISATION      = -13,
                    PAR_COMMENTAIRE     = -14,
                    TOUTES_CATEGORIES   = -15
                  };

#define INDEX_MAX_CONNNES 5    // nombre de type de champ de ligne de paye (Libellé, Code, Taux, Base, ...) moins 1.
#define BESOIN_MEMOIRE_ENTETE  22  /* nb d'éléments de l'enum ci-dessous */

typedef enum {
              Annee, Mois, Budget, Employeur, Siret,
              Nom, Prenom, Matricule, NIR, NbEnfants,
              Statut, EmploiMetier, Grade, Indice,
              Service, NBI, QuotiteTrav, NbHeureTotal,
              NbHeureSup, MtBrut, MtNet, MtNetAPayer
         } Entete;

constexpr const char* Tableau_entete[] = {"Annee", "Mois", "Budget", "Employeur", "Siret", "Nom", "Prenom", "Matricule", "NIR", "NbEnfants",
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
    uint32_t taille_base;
    BaseType  type_base;
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
    int  nbfil;
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
/* A chaque valeur de drapeau[i][0] doit correspondre un type différent de rémunération type_remuneration[i] */


void* decoder_fichier(info_t& tinfo);



#endif // VALIDATOR_HPP_INCLUDED
