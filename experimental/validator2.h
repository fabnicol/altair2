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
#include <cinttypes>
#include <vector>
#include <libxml/parser.h>

#define GCC_INLINE __attribute__((always_inline))
#define GCC_UNUSED __attribute__((__unused__))


typedef struct
{
    int      thread_num;
    std::vector<std::string>   argv;
    std::vector<std::string> in_memory_file;
    unsigned argc;
} thread_t;


static constexpr auto EXPRESSION_REG_ELUS = "^maire.*|^pr..?sident.*|^[eé]lus?|^(?:adj.*\\bmaire\\b|vi.*\\bpr..?sident\\b|cons.*\\bmuni|cons.*\\bcomm|cons.*\\bd..?l..?gu).*",

  EXPRESSION_REG_VACATIONS = ".*\\bvacat.*|.*\\bvac\\.?\\b.*",                 // vac.* peut être vérifié par 'vacances'
  EXPRESSION_REG_ASSISTANTES_MATERNELLES = ".*\\bass.*\\bmater.*",
  EXPRESSION_REG_ADJOINTS = "\\W*(?:adj.*(?:adm|ani|tech|pat)|op[eé].*(a\\.?p\\.?s\\.?|act)|aux.*(pu[eé]r|soin)|gard(ien|.*ch)|brigadier|receveur|sapeur|capor|sous.*off).*",
  EXPRESSION_REG_AGENTS = "\\W*(?:(?:agent|agt\\.?).*(?:soc|ma[îi]|poli|p\\.?m\\.?|sp[eé]|pat)|(?:agent|agt\\.?)?.*atsem).*",

 /* Attention il ne faut pas autre chose que \\W* car sinon on peut avoir confusion entre cons[eiller].* et [agent].*cons[ervation].*   */
 /* cons = conseiller ou conservateur souvent abrégé cons., mai speut être aussi conservation */

  EXPRESSION_REG_CAT_A = "\\W*(?:adminis|attach|biol|biblio|infi.*(?:cad.*san|soi)|cap.*t|com.*t|.*colon|cons\\.?|d\\S*\\.?\\s*g\\S*\\.?|direct|ing[eé]n|m[eé]de|pharm|prof|psy|puer|puér|sage.*f|secr.*mai[v[eé]t[eé]r]|infirm\\S*\\s.*(?:soi|enc.*s|C\\.?S\\.?)).*",

 /* A cause du cas problématique des infirmiers, ex B recatégorisés en A, il faut d'abord tester A puis si échec B */

  EXPRESSION_REG_CAT_B = "\\W*(?:r[eé]dac|tech|anim|[eé]duc|ass\\.?.*(?:ens|cons|pat|bib|socio.*[eé]d|m[ée]d.*t)|monit|contr[oô].*t(?:er|ra)|chef.*p.*m|lieut[^c\\s]*\\b|infirm|r[eé][eé]duc).*",

/* Les définitions ci-après doivent être négatives */  NOM_BASE = "Table",
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
                    NA = 13,
                    TOUTES_CATEGORIES = 14,
                    MAXIMUM_LIGNES = 15,
                    MAXIMUM_LIGNES_PAR_ANNEE = 16
                  };

#define INDEX_MAX_COLONNNES 5    // nombre de type de champ de ligne de paye (Libelle, Code, Taux, Base, ...) moins 1.
#define BESOIN_MEMOIRE_ENTETE  27  /* nb d'éléments de l'enum ci-dessous */

typedef enum {
              Annee, Mois, Budget, Employeur, Siret, Etablissement,
              Nom, Prenom, Matricule, NIR, NbEnfants, Statut,
              EmploiMetier, Grade, Echelon, Indice, Code, Description, Service, NBI, QuotiteTrav,
              NbHeureTotal, NbHeureSup, MtBrut, MtNet, MtNetAPayer, Categorie
         } Entete;

enum { V };

constexpr const char* Tableau_entete[] = {
                                    "Annee", "Mois", "Budget", "Employeur", "Siret", "Etablissement",
                                    "Nom", "Prenom", "Matricule", "NIR", "NbEnfants", "Statut",
                                    "EmploiMetier", "Grade", "Echelon", "Indice", "Evenement", "Service", "NBI", "QuotiteTrav",
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
    bool pretend;
    bool verifmem;
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

#ifndef MAX_MEMORY_SHARE
#define MAX_MEMORY_SHARE 0.9
#endif

#ifndef NA_STRING
 #define NA_STRING  (xmlChar*) "NA"
#endif
#ifndef MAX_LIGNES_PAYE
 #define MAX_LIGNES_PAYE 1000
#endif

// MAX_NB_AGENTS détermine le nombre maximal d'agents par mois potentiellement traités

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


static const int nbType                  = sizeof(type_remuneration)/sizeof(char*);  // + NA


static const xmlChar drapeau[][2]  = {{1,0}, {2,0}, {3,0}, {4,0}, {5,0}, {6,0}, {7,0}, {8,0}, {9,0}, {10,0}, {11,0}, {12,0}};
/* A chaque valeur de drapeau[i][0] doit correspondre un type différent de rémunération type_remuneration[i] */


void* decoder_fichier(info_t& tinfo);

using namespace std;
#ifndef LIBXML2
    using xmlNode = pair<string, uint32_t>;
    using xmlNodePtr = vector<xmlNode>::const_iterator;
    using xmlDocPtr = vector<xmlNode>;
    typedef string xmlChar ;
    #define NULLPTR string::end();
    class Xml
    {
      public:
        static vector<pair<string, uint32_t>> V;
        Xml();
    };

    inline xmlNodePtr GCC_INLINE atteindreNoeud(const int n, xmlNodePtr iter0)
    {
        xmlNodePtr iter = iter0;

        while (iter != v.end() && iter->second != n)
        {
               ++iter;
        }

       return iter;
    }

    /*
      L'index uint32_t est ainsi défini :
         8 premiers bits : libellé du noeud
         1 bit : flag nouvelle ligne de paie
         1 bit : flag nouvel agent
         2 bits : libres
         20 bits (max 1 006 200 mots soit environ 5 Mo soit environ 500 agents) : dernier index conservé
    */

    uint8_t tag(const xmlNode& x)
    {
        return x.second & 0xFF;
    }

    /* La valeur des tags est la suivante :

      {0, 1} : si le tag a une valeur associée : tag & 0x01
      2..0xFE : tag lui même : tag >> 1
      0xFF : tag "V", valeur associée au tag précédent

     */

    bool n_paie(const xmlNode& x)
    {
        return (bool) x.second & 0x0100;
    }
    bool n_agent(const xmlNode& x)
    {
        return (bool) x.second & 0x0200;
    }
    uint32_t prec(const xmlNode& x)
    {
        return x.second & 0xFFFFF000 >> 12;
    }

    string valeur(const xmlNode& x)
    {
        return x.first;
    }

    void tag(const xmlNode& x, int y)
    {
        x.second = (x.second & 0xFFFFFF00) + (y & 0xFF);
    }

    void n_paie(const xmlNode& x, bool y)
    {
           x.second = (x.second & 0xFFFFF000) + ( y << 8) ;
    }

    void n_agent(const xmlNode& x, bool y)
    {
           x.second = (x.second  & 0xFFFFF000) + (y << 9);
    }

    void prec(const xmlNode& x, uint32_t y)
    {
        return x.second = (x.second & 0x00000FFF) + (y << 12);  // y encodé sur les 20 premiers bits seulement sinon tronqué en silence
    }

    string xmlGetProp(const xmlNodePtr& iter)
    {
        while (iter != string::end() && tag(*iter) != 0xFF) ++iter;
        return valeur(*iter);
    }

#else
    #define NULLPTR nullptr;

inline xmlNodePtr GCC_INLINE atteindreNoeud_(const char * noeud, xmlNodePtr cur, int normalJump = 0)
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

#define atteindreNoeud(X, Y) atteindreNoeud_(#X, Y)

xmlChar* xmlGetProp(const xmlNodePtr x) { return xmlNodePtr(x, (const xmlChar *) "V");

#endif




#endif // VALIDATOR_HPP_INCLUDED
