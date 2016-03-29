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

using namespace std;


#ifdef  __GNUG__
#define GCC_INLINE __attribute__((always_inline))
#define GCC_UNUSED __attribute__((unused))
#else
#define GCC_INLINE
#define GCC_UNUSED
#endif


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
    #define DEBUG(X) cerr << "\n" << X << "\n";
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

#ifndef CUTFILE_CHUNK
  #define CUTFILE_CHUNK  15 * 1024 * 1024
#endif


#ifndef AVERAGE_RAM_DENSITY
#define AVERAGE_RAM_DENSITY 1.25
#endif


struct thread_t
{
    int      thread_num = 0;
    vector<pair<string, int>>   argv ;
    #if defined(STRINGSTREAM_PARSING)
        vector<vector<string>> in_memory_file_cut;
        vector<string> in_memory_file ;
    #else
        vector<vector<string>>   argv_cut;
    #endif
    unsigned argc = 1;
};


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

using Mem_management = enum {
                              INDEX_MAX_COLONNNES = 5,    // nombre de type de champ de ligne de paye (Libellé, Code, Taux, Base, ...) moins 1.
                              BESOIN_MEMOIRE_ENTETE = 27,  /* nb d'éléments de l'enum ci-dessous */
                              RETRY = -1,
                              SKIP_FILE = 1,
                              NO_AGENT = -2
                            };

using XML_code = enum {
                        NODE_FOUND,
                        NODE_NOT_FOUND,
                        LINE_MEMORY_EXCEPTION,
                        NO_NEXT_ITEM,
                        PREMIER_FICHIER,
                        FICHIER_SUIVANT_DECOUPE,
                        DERNIER_FICHIER_DECOUPE
                       };

using Entete = enum {
              Annee, Mois, Budget, Employeur, Siret, Etablissement,
              Nom, Prenom, Matricule, NIR, NbEnfants, Statut,
              EmploiMetier, Grade, Echelon, Indice, Code, Description, Service, NBI, QuotiteTrav,
              NbHeureTotal, NbHeureSup, MtBrut, MtNet, MtNetAPayer, Categorie
         };

constexpr const char* Tableau_entete[] = {
                                    "Annee", "Mois", "Budget", "Employeur", "Siret", "Etablissement",
                                    "Nom", "Prenom", "Matricule", "NIR", "NbEnfants", "Statut",
                                    "EmploiMetier", "Grade", "Echelon", "Indice", "Evenement", "Service", "NBI", "QuotiteTrav",
                                    "NbHeureTotal", "NbHeureSup", "MtBrut", "MtNet", "MtNetAPayer" };


class info_t
{
private :
    thread_t mon_thread;

public:
    vector<vector<xmlChar*>> Table = {{}};
    uint64_t nbLigne               = 0;
    vector<uint64_t> taille        = {};
    vector<uint32_t> NAgent        = {};
    uint32_t chunksize             = CUTFILE_CHUNK;
    uint32_t nbAgentUtilisateur    = 0;
    uint32_t NCumAgent             = 0;
    uint32_t NCumAgentXml          = 0;
    uint32_t taille_base           = 0;
    BaseType  type_base            = BaseType::MONOLITHIQUE;
    vector<uint16_t> NLigne        = {};
    thread_t* threads              = &mon_thread;
    string chemin_log              = "";
    string expression_reg_elus     = EXPRESSION_REG_ELUS;
    string chemin_base             = NOM_BASE + string(CSV);
    string chemin_bulletins        = NOM_BASE_BULLETINS + string(CSV);
    uint16_t nbLigneUtilisateur    = MAX_LIGNES_PAYE;
    uint16_t fichier_courant       = 0;
    char decimal                   = ',';
    char separateur                = ';';
    bool reduire_consommation_memoire = true;
    bool drapeau_cont              = true;
    bool calculer_maxima           = false;
    bool generer_rang              = false;
    bool select_siret              = false;
    bool select_echelon            = false;
    bool pretend                   = false;
    bool verifmem                  = false;
    bool decoupage_fichiers_volumineux = true;
#ifdef FGETC_PARSING
    bool preserve_tempfiles        = false;
#endif
    unsigned int  nbfil            = 1;
    vector<int> Memoire_p_ligne    = {};
};


struct xml_commun
{
    xmlChar *annee_fichier = nullptr;
    xmlChar *mois_fichier = nullptr;
    xmlChar *employeur_fichier = nullptr;
    xmlChar *etablissement_fichier = nullptr;
    xmlChar *siret_fichier = nullptr;
    xmlChar *budget_fichier = nullptr;
};

using LineCount = struct {
     int nbLignePaye;
     int memoire_p_ligne_allouee;

};

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


inline xmlNodePtr GCC_INLINE atteindreNoeud(const char * noeud, xmlNodePtr cur)
{
   #if 1
    while (cur && xmlIsBlankNode(cur))
    {
        cur = cur -> next;
    }
   #endif


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
