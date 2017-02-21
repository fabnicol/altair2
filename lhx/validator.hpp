#ifndef VALIDATOR_HPP_INCLUDED
#define VALIDATOR_HPP_INCLUDED

/* CETTE PAGE DOIT RESTER ENCODEE EN UTF8 */

#include <string>
#include <cstdlib>
#include <cstdint>
#include <cstdbool>
#include <climits>
#include <clocale>
#include <cerrno>
#include <cmath>
#include <cstring>
#include <cctype>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <cinttypes>
#include <vector>
#include <array>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include "tags.h"

#define GCC_INLINE __attribute__((always_inline))
#define GCC_UNUSED __attribute__((__unused__))

using namespace std;

typedef struct
{
    int      thread_num;
    vector<string>   argv;
    vector<string> in_memory_file;
    unsigned argc;
} thread_t;

/*
Décret n° 2011-746 du 27 juin 2011 portant statuts particuliers des corps des personnels de rééducation de la catégorie B de la fonction publique hospitalière
Sont classés dans la catégorie B les corps des personnels de rééducation des établissements mentionnés à l'article 2 de la loi du 9 janvier 1986 susvisée, ci-dessous énumérés :
1° Le corps des pédicures-podologues ;
2° Le corps des masseurs-kinésithérapeutes ;
3° Le corps des ergothérapeutes ;
4° Le corps des psychomotriciens ;
5° Le corps des orthophonistes ;
6° Le corps des orthoptistes ;
7° Le corps des diététiciens.  */

/* Passage en A des ergothérapeutes
Décret n° 2015-1048 du 21 août 2015 portant dispositions statutaires relatives aux ergothérapeutes de la fonction publique hospitalière  */

/* Nouveau corps, classe sup
 *       IB  IN
 * 1	449	394	1 an	1 824,34 €
   2	465	407	2 ans	1 884,53 €
   3	491	424	2 ans	1 963,25 €
   4	517	444	2 ans	2 055,85 €
   5	546	464	2 ans	2 148,46 €
   6	577	487	3 ans	2 254,96 €
   7	607	510	3 ans	2 361,45 €
   8	637	533	4 ans	2 467,95 €
   9	667	556	4 ans	2 574,45 €
   10	701	582	4 ans	2 694,83 €
   11	736	608	-	2 815,22 €

Corps : Ergothérapeute (nouveau corps au 01.09.2015), classe N
Categorie : A


1	385	353	1 an	1 634,50 €
2	408	367	2 ans	1 699,32 €
3	438	386	3 ans	1 787,30 €
4	464	406	3 ans	1 879,90 €
5	497	428	3 ans	1 981,77 €
6	542	461	3 ans	2 134,57 €
7	582	492	3 ans	2 278,11 €
8	611	513	4 ans	2 375,34 €
9	637	533	4 ans	2 467,95 €
10	663	553	4 ans	2 560,56 €
11	685	570	-	2 639 */

/* ergothérapeutes : tester sur l'indice {353, 367, 386, 394, 406, 407, 424, 428, 444, 461, 464, 487, 487, 492, 510, 513, 533, 553, 556, 570, 582, 608 }
 * de la nouvelle grille A. On a vérifié qu'il n'y a pas de superposition avec l'ancienne */

/* Les infirmières FPH sont passées en A en 2012 sauf un corps en extinction IDE; situation à évaluer pour l'éducation nationale sur les situations d'extinction */

#include "expression_reg_elus.hpp"
#include "expression_reg_adjoints.hpp"

static constexpr const array<int, 22> indices_ergo = {353, 367, 386, 394, 406, 407, 424, 428, 444, 461, 464, 487, 487, 492, 510, 513, 533, 553, 556, 570, 582, 608 };


static constexpr auto EXPRESSION_REG_VACATIONS = ".*\\bvacat.*|.*\\bvac\\.?\\b.*";                 // vac.* peut être vérifié par 'vacances'
  
static constexpr auto EXPRESSION_REG_ASSISTANTES_MATERNELLES = ".*\\bass.*\\bmater.*";
        
static constexpr auto   EXPRESSION_REG_AGENTS = "\\W*(?:A\\.?S\\.?\\b|A\\.?A\\.?\\b|A\\.?E\\.?Q\\.?\\b|A\\.?A\\.?H\\.?\\b|A\\.?S\\.?H\\.?Q\\.?|O\\.?P\\.?Q\\.?|\
(?:agent|agt\\.?).*(?:ser.*ho|soc|ma[îi]|poli|p\\.?m\\.?|pat|ent.*\\b(?:qu|sp))|\
(?:agent|agt\\.?)?.*atsem|aide.*(?:soi|pha)|aumonier|cond.*amb|dessin|.*ouv(?:rier|.*prof)).*",

 /* Attention il ne faut pas autre chose que \\W* car sinon on peut avoir confusion entre cons[eiller].* et [agent].*cons[ervation].*   */
 /* cons = conseiller ou conservateur souvent abrégé cons., mais peut être aussi conservation */

  EXPRESSION_REG_CAT_A = "\\W*\
(?:adminis|a.*\\bh.*\\bu|c.*\\b(?:cl|tr).*\\bu|attach|biol|biblio|cad.*(?:\\bsoc.*ed|\\bsan)|cap.*t|com.*t|.*colon|cons\\.?|d\\S*\\.?\\s*g\\S*\\.?|\
dir(?:ect|.*\\bet.*b|.*\\bsoi)|ingen|mede|ma.t.*conf|prat.*hos|pharm|ped.*p.*c.*\\bs|prep.*c.*\\bs|prof|psy.*(?:l|m.*c.*\\bs)|puer.*cad.*sa|puericultr|sage.*f|secr.*mai[veter]|\
i\\.?a\\.?d\\.?e\\.?|i\\.?b\\.?o\\.?d\\.?e\\.?|I\\.?S\\.?G\\.?(?:\\b|S)|int.*(?:med|phar|od)|infi?r?m?i?.*(?:\\b(?!i)|anes|bloc|i\\.?a\\.?d\\.?|i\\.?b\\.?o\\.?d\\.?|s\\.?\\s*\\bg\\.?|soi|enc.*s)|\
radiophys|(?:tec.*l|mass.*kin|diet|inf|manip).*\\bc(?:\\.|a).*\\bs).*",

 /* A cause du cas problématique des infirmiers et diététiciens, ex B recatégorisés en A, il faut d'abord tester A puis si échec B */

  EXPRESSION_REG_CAT_B = "\\W*\
(?:redac|tech|T\\.?S\\.?H\\.?|anim|educ|a\\.?\\s?s\\.?\\s?(?:e|\\s)|ast\\.?|assi?s?t?\\.?.*(?:spec|ens|cons|pat|bib|social|soc.*ed|med.*t|med.*adm)|monit|contro.*t(?:er|ra)|\
chef.*p.*m|lieut[^c\\s]*\\b|I\\.?D\\.?E\\.?|inf.*\\bi\\.?d\\.?e|reeduc|adj.*cadr|analyst|diet|prep.*ph|ped.*po|programmeu|orthop|mass.*kin|manip|secr.*med|\\ba\\.?m\\.?a\\.?\\b).*",
/*
 * REDACTEUR
 * TECHNICIEN
 * TECHNICIEN_SUP_HOSP
 * ANIMATEUR
 * AS
 * EDUCATEUR
 * ASSISTANT
 * SPECIALISE
 * ENSEIGNEMENT
 * CONSERVATION
 * PATRIMOINE
 * BIBLIOTHEQUES
 * SOCIO_EDUC
 * MEDICO_TECH
 * MEDICO_ADM
 * MONITEUR
 * CONTROLEUR
 * CHEF_POLICE_MUNIC ?
 * LIEUTENANT ?
 * 
 * 
 * 
 */
        
        
  EXPRESSION_REG_ERGO = "\\W*ergo.*",

  NOM_BASE = "Table",
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

#define INDEX_MAX_COLONNNES 5    // nombre de type de champ de ligne de paye (Libellé, Code, Taux, Base, ...) moins 1.
#define BESOIN_MEMOIRE_ENTETE  27  /* nb d'éléments de l'enum ci-dessous */

typedef enum {
              Annee, Mois, Budget, Employeur, Siret, Etablissement,
              Nom, Prenom, Matricule, NIR, NbEnfants, Statut,
              EmploiMetier, Grade, Echelon, Indice, Code, Description, Service, NBI, QuotiteTrav,
              NbHeureTotal, NbHeureSup, MtBrut, MtNet, MtNetAPayer, Categorie
         } Entete;

constexpr const char* Tableau_entete[] = {
                                    "Annee", "Mois", "Budget", "Employeur", "Siret", "Etablissement",
                                    "Nom", "Prenom", "Matricule", "NIR", "NbEnfants", "Statut",
                                    "EmploiMetier", "Grade", "Echelon", "Indice", "Evenement", "Service", "NBI", "QuotiteTrav",
                                    "NbHeureTotal", "NbHeureSup", "MtBrut", "MtNet", "MtNetAPayer" };


typedef struct
{

    vector<vector<xmlChar*>> Table;
    uint64_t nbLigne;
    vector<array<uint64_t, 3>> ligne_debut;
    vector<array<uint64_t, 2>> ligne_fin;
    vector<uint32_t> NAgent;
    uint32_t nbAgentUtilisateur;
    uint32_t NCumAgent;
    uint32_t NCumAgentXml;
    uint32_t taille_base;
    BaseType  type_base;
    vector<uint16_t> NLigne;
    thread_t* threads;
    string chemin_log;
    string expression_reg_elus;
    string chemin_base;
    string chemin_bulletins;
    string export_mode;
    uint16_t nbLigneUtilisateur;
    uint16_t fichier_courant;
    char decimal;
    char separateur;
    bool reduire_consommation_memoire;
    bool drapeau_cont;
    bool calculer_maxima;
    bool generer_rang;
    bool generer_bulletins;
    bool select_siret;
    bool select_echelon;
    bool pretend;
    bool verifmem;
    bool cdrom;
    unsigned int  nbfil;
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
void* parse_info(info_t& info);

inline xmlNodePtr GCC_INLINE atteindreNoeud(const char * noeud, xmlNodePtr cur)
{

#       ifdef DEBUG_ATTEINDRE   

          cerr << "[DEBUG] --- Recherche de " << noeud <<  ENDL;
#       endif          
    
    while (cur && xmlIsBlankNode(cur))
    {
        cur = cur -> next;
#       ifdef DEBUG_ATTEINDRE
          cerr << "[DEBUG] Saut de noeud blanc" << ENDL;
#       endif            
    }


    while (cur != nullptr && xmlStrcmp(cur->name,  (const xmlChar*) noeud))
    {
#       ifdef DEBUG_ATTEINDRE   
      //       cerr << "[DEBUG]      ......" << cur->name <<  ENDL;
#       endif          
        
        cur = cur->next;
    }

      if (cur == nullptr)
      {
          AFFICHER_NOEUD(noeud)  // cur->name == noeud
      }
#     ifdef DEBUG_ATTEINDRE   
        else
        {
          cerr << "[DEBUG] !!! Trouvé " << cur->name <<  ENDL;
        }
#     endif          
      

     return cur;  // soit un pointer vers le bon noeud, soit nullptr
}



#endif // VALIDATOR_HPP_INCLUDED
