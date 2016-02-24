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
#define GCC_INLINE __attribute__((always_inline))
#define GCC_UNUSED __attribute__((__unused__))
#else
#define GCC_INLINE
#define GCC_UNUSED
#endif


typedef struct
{
    int      thread_num;
    std::vector<std::string>   argv;
    std::vector<std::string> in_memory_file;
    unsigned argc;
} thread_t;

static constexpr auto EXPRESSION_REG_ELUS = "^maire.*|^pr..?sident.*|^[e�]lus?|^(?:adj.*\\bmaire\\b|vi.*\\bpr..?sident\\b|cons.*\\bmuni|cons.*\\bcomm|cons.*\\bd..?l..?gu).*",
  EXPRESSION_REG_VACATIONS = ".*\\bvacat.*|.*\\bvac\\.?\\b.*",                 // vac.* peut �tre v�rifi� par 'vacances'
  EXPRESSION_REG_ASSISTANTES_MATERNELLES = ".*\\bass.*\\bmater.*",
  EXPRESSION_REG_ADJOINTS = "\\W*(?:adj.*(?:adm|ani|tech|pat)|op[e�].*(a\\.?p\\.?s\\.?|act)|aux.*(pu[e�]r|soin)|gard(ien|.*ch)|brigadier|receveur|sapeur|capor|sous.*off).*",
  EXPRESSION_REG_AGENTS = "\\W*(?:(?:agent|agt\\.?).*(?:soc|ma[�i]|poli|p\\.?m\\.?|sp[e�]|pat)|(?:agent|agt\\.?)?.*atsem).*",

 /* Attention il ne faut pas autre chose que \\W* car sinon on peut avoir confusion entre cons[eiller].* et [agent].*cons[ervation].*   */
 /* cons = conseiller ou conservateur souvent abr�g� cons., mai speut �tre aussi conservation */

  EXPRESSION_REG_CAT_A = "\\W*(?:adminis|attach|biol|biblio|infi.*(?:cad.*san|soi)|cap.*t|com.*t|.*colon|cons\\.?|d\\S*\\.?\\s*g\\S*\\.?|direct|ing[e�]n|m[e�]de|pharm|prof|psy|puer|pu�r|sage.*f|secr.*mai[v[e�]t[e�]r]|infirm\\S*\\s.*(?:soi|enc.*s|C\\.?S\\.?)).*",

 /* A cause du cas probl�matique des infirmiers, ex B recat�goris�s en A, il faut d'abord tester A puis si �chec B */

  EXPRESSION_REG_CAT_B = "\\W*(?:r[e�]dac|tech|anim|[e�]duc|ass\\.?.*(?:ens|cons|pat|bib|socio.*[e�]d|m[�e]d.*t)|monit|contr[o�].*t(?:er|ra)|chef.*p.*m|lieut[^c\\s]*\\b|infirm|r[e�][e�]duc).*",

/* Les d�finitions ci-apr�s doivent �tre n�gatives */  NOM_BASE = "Table",
  NOM_BASE_BULLETINS = "Bulletins",
  CSV = ".csv";


enum class BaseCategorie : int {
                                 BASE = 0,
                                 BULLETINS = 1
                               };

enum class BaseType : int
                  {
                    MONOLITHIQUE = -1,
                    PAR_ANNEE = 0,
                    PAR_TRAITEMENT = 1,
                    PAR_INDEMNITE_RESIDENCE = 2,
                    PAR_SFT = 3,
                    PAR_AVANTAGE_NATURE = 4,
                    PAR_INDEMNITE = 5,
                    PAR_REM_DIVERSES = 6,
                    PAR_DEDUCTION = 7,
                    PAR_ACOMPTE = 8,
                    PAR_RAPPEL = 9,
                    PAR_RETENUE = 10,
                    PAR_COTISATION = 11,
                    PAR_COMMENTAIRE = 12,
                    TOUTES_CATEGORIES = 13,
                    MAXIMUM_LIGNES = 14,
                    MAXIMUM_LIGNES_PAR_ANNEE = 15
                  };

#define INDEX_MAX_COLONNNES 5    // nombre de type de champ de ligne de paye (Libell�, Code, Taux, Base, ...) moins 1.
#define BESOIN_MEMOIRE_ENTETE  25  /* nb d'�l�ments de l'enum ci-dessous */

typedef enum {
              Annee, Mois, Budget, Employeur, Siret, Etablissement,
              Nom, Prenom, Matricule, NIR, NbEnfants, Statut,
              EmploiMetier, Grade, Echelon, Indice, Service, NBI, QuotiteTrav,
              NbHeureTotal, NbHeureSup, MtBrut, MtNet, MtNetAPayer, Categorie
         } Entete;

constexpr const char* Tableau_entete[] = {
                                    "Annee", "Mois", "Budget", "Employeur", "Siret", "Etablissement",
                                    "Nom", "Prenom", "Matricule", "NIR", "NbEnfants", "Statut",
                                    "EmploiMetier", "Grade", "Echelon", "Indice", "Service", "NBI", "QuotiteTrav",
                                    "NbHeureTotal", "NbHeureSup", "MtBrut", "MtNet", "MtNetAPayer" };


typedef struct
{

    std::vector<std::vector<xmlChar*>> Table;
    uint64_t nbLigne;
    std::vector<uint32_t> NAgent;
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
    bool select_echelon;
    unsigned int  nbfil;
    std::vector<int> Memoire_p_ligne;
} info_t;

typedef struct {
     int nbLignePaye;
     int memoire_p_ligne_allouee;

} LineCount;


#define RETRY -1
#define SKIP_FILE 1
#define NO_AGENT -1


#ifndef NA_STRING
 #define NA_STRING  (xmlChar*) "NA"
#endif
#ifndef MAX_LIGNES_PAYE
 #define MAX_LIGNES_PAYE 1000
#endif

// MAX_NB_AGENTS d�termine le nombre maximal d'agents par mois potentiellement trait�s

#ifndef MAX_NB_AGENTS
 #define MAX_NB_AGENTS 50000
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

/* pas de contr�le d'existence de noeud : version affaiblie de la macro pr�c�dente */



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
/* A chaque valeur de drapeau[i][0] doit correspondre un type diff�rent de r�mun�ration type_remuneration[i] */


void* decoder_fichier(info_t& tinfo);


inline xmlNodePtr GCC_INLINE atteindreNoeud(const char * noeud, xmlNodePtr cur, int normalJump = 0)
{
   #if 1
    while (cur && xmlIsBlankNode(cur))
    {
        cur = cur -> next;
    }
   #endif

    for (int i = 0; i < normalJump; ++i)
           cur = cur->next;

    while (cur != nullptr && xmlStrcmp(cur->name,  (const xmlChar*) noeud))
    {
           cur = cur->next;
    }

      if (cur == nullptr)
      {
          AFFICHER_NOEUD(noeud)  // cur->name == noeud
      }

     return cur;  // soit un pointer vers le bon noeud, soit nullptr
}



#endif // VALIDATOR_HPP_INCLUDED
