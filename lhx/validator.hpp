// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser les fichiers de paye
// produits au format spécifié par l'annexe de la convention-cadre nationale de dématérialisation
// en vigueur à compter de l'année 2008.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions
// de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
// sur le site "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie,
// de modification et de redistribution accordés par cette licence, il n'est
// offert aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons,
// seule une responsabilité restreinte pèse sur l'auteur du programme, le
// titulaire des droits patrimoniaux et les concédants successifs.
//
// A cet égard l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant
// donné sa spécificité de logiciel libre, qui peut le rendre complexe à
// manipuler et qui le réserve donc à des développeurs et des professionnels
// avertis possédant des connaissances informatiques approfondies. Les
// utilisateurs sont donc invités à charger et tester l'adéquation du
// logiciel à leurs besoins dans des conditions permettant d'assurer la
// sécurité de leurs systèmes et ou de leurs données et, plus généralement,
// à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de la licence CeCILL, et que vous en avez accepté les
// termes.
//
//
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

/// Macro tendant à forcer l'inlining sous GCC
#define GCC_INLINE __attribute__((always_inline))

/// Macro indiquant un argument non utilisé
#define GCC_UNUSED __attribute__((__unused__))

using namespace std;

/// Structure permettant de transmettre à chaque fil d'exécution une partie des données de paye
typedef struct
{
    int      thread_num;  ///< Rang du fil d'exécution
    vector<string>   argv;  ///< Vecteur de noms de fichiers de paye
    vector<string> in_memory_file; ///< Vecteur empilant en mémoire des fichiers de paye
    unsigned argc; ///< Rang du fichier de paye
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

// Les ergothérapeutes ayant changé de catégorie, il faut tester leur indice pour déterminer dans quelle catégorie les classer
// Attention problème d'entretien en cas d'attribution de points uniformes !

static constexpr const array<int, 22> indices_ergo = {353,
                                                      367,
                                                      386,
                                                      394,
                                                      406,
                                                      407,
                                                      424,
                                                      428,
                                                      444,
                                                      461,
                                                      464,
                                                      487,
                                                      487,
                                                      492,
                                                      510,
                                                      513,
                                                      533,
                                                      553,
                                                      556,
                                                      570,
                                                      582,
                                                      608 };

/// Expression régulière tendant à capturer les vacataires
static constexpr auto EXPRESSION_REG_VACATIONS = ".*\\bvacat.*|.*\\bvac\\.?\\b.*";                 // vac.* peut être vérifié par 'vacances'
  
/// Expression régulière tendant à capturer les assistantes maternelles
static constexpr auto EXPRESSION_REG_ASSISTANTES_MATERNELLES = ".*\\bass.*\\bmater.*";
        
/// Expression régulière tendant à capturer les agents de catégorie C
static constexpr auto   EXPRESSION_REG_AGENTS = "\\W*(?:A\\.?S\\.?\\b|A\\.?A\\.?\\b|A\\.?E\\.?Q\\.?\\b|A\\.?A\\.?H\\.?\\b|A\\.?S\\.?H\\.?Q\\.?|O\\.?P\\.?Q\\.?|\
(?:agent|agt\\.?).*(?:ser.*ho|soc|ma[îi]|poli|p\\.?m\\.?|pat|ent.*\\b(?:qu|sp))|ch.*pol.*mun|\
(?:agent|agt\\.?)?.*atsem|aide.*(?:soi|pha)|aumonier|cond.*amb|dessin|.*ouv(?:rier|.*prof)).*";

 /* Attention il ne faut pas autre chose que \\W* car sinon on peut avoir confusion entre cons[eiller].* et [agent].*cons[ervation].*   */
 /* cons = conseiller ou conservateur souvent abrégé cons., mais peut être aussi conservation */

/// Expression régulière tendant à capturer les officiers subalternes de catégorie B
#define OFFICIER_SUB     "(?:sous-?|\\b)lieut[^c]*\\b|major"

/// Expression régulière tendant à capturer les agents officiers (subalternes et supérieurs) de catégorie A
#define OFFICIER         "lieut.*col|capit|com.*d.*t|colon|g.?.?n.?.?ral"

/// Expression régulière tendant à capturer les agents de catégorie A
static constexpr auto   EXPRESSION_REG_CAT_A = "\\W*\
(?:adminis|a.*\\bh.*\\bu|c.*\\b(?:cl|tr).*\\bu|attach|biol|biblio|cad.*(?:\\bsoc.*ed|\\bsan)|" OFFICIER "|cons\\.?|d\\S*\\.?\\s*g\\S*\\.?|\
dir(?:ect|.*\\bet.*b|.*\\bsoi)|ingen|mede|ma.t.*conf|prat.*hos|pharm|ped.*p.*c.*\\bs|prep.*c.*\\bs|prof|psy.*(?:l|m.*c.*\\bs)|puer.*cad.*sa|puericultr|sage.*f|secr.*mai|v[eé]t[eé]r|\
i\\.?a\\.?d\\.?e\\.?|i\\.?b\\.?o\\.?d\\.?e\\.?|I\\.?S\\.?G\\.?(?:\\b|S)|int.*(?:med|phar|od)|infi?r?m?i?.*(?:\\b(?!i)|anes|bloc|i\\.?a\\.?d\\.?|i\\.?b\\.?o\\.?d\\.?|s\\.?\\s*\\bg\\.?|soi|enc.*s)|\
radiophys|(?:tec.*l|mass.*kin|diet|inf|manip).*\\bc(?:\\.|a).*\\bs).*";

 // A cause du cas problématique des infirmiers et diététiciens, ex B recatégorisés en A, il faut d'abord tester A puis si échec B

/// Expression régulière tendant à capturer les agents de catégorie C
static constexpr auto   EXPRESSION_REG_CAT_B = "\\W*\
(?:redac|tech|T\\.?S\\.?H\\.?|anim|educ|a\\.?\\s?s\\.?\\s?(?:e|\\s)|ast\\.?|assi?s?t?\\.?.*(?:spec|ens|cons|pat|bib|social|soc.*ed|med.*t|med.*adm)|monit|contro.*t(?:er|ra)|\
ch.*(?:s.*po|S.*P.*M).*|I\\.?D\\.?E\\.?|inf.*\\bi\\.?d\\.?e|reeduc|adj.*cadr|analyst|diet|prep.*ph|ped.*po|programmeu|orthop|mass.*kin|manip|secr.*med|\\ba\\.?m\\.?a\\.?\\b|" OFFICIER_SUB ").*";

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
 * LIEUTENANT 
 */
        
/// Expression régulière tendant à capturer les agents de type ergothérapeute
static constexpr auto EXPRESSION_REG_ERGO = "\\W*ergo.*";

/// Nom racine des fichiers Table....csv (Lignes de paye)
static constexpr auto NOM_BASE = "Table";

/// Nom racine des fichiers Bulletins.paie.csv
static constexpr auto NOM_BASE_BULLETINS = "Bulletins";

/// Extension CSV
static constexpr auto CSV = ".csv";

/// Enum des types de base (Bulletins ou Lignes de paye)
enum class BaseCategorie : int {
                                 BASE = 0,  ///< Type de base CSV en sortie : Table.csv
                                 BULLETINS = 1 ///< Type de base CSV en sortie : Bulletins.paie.csv
                               };
/// Enum des types de base en sortie (tri par catégorie de ligne de paye)
enum class BaseType : int
                  {
                    MONOLITHIQUE = -1, ///< Base complète, en un seul bloc
                    PAR_ANNEE = 0, ///< Base par année
                    PAR_TRAITEMENT = 1, ///< Base par type de ligne de paye "Traitement"
                    PAR_INDEMNITE_RESIDENCE = 2, ///< Base par type de ligne de paye "Indemnité de résidence"
                    PAR_SFT = 3, ///< Base par type de ligne de paye "Supplément familial de traitement"
                    PAR_AVANTAGE_NATURE = 4, ///< Base par type de ligne de paye "Avantage en nature"
                    PAR_INDEMNITE = 5, ///< Base par type de ligne de paye "Indemnité"
                    PAR_REM_DIVERSES = 6, ///< Base par type de ligne de paye "Rémunérations diverses"
                    PAR_DEDUCTION = 7,  ///< Base par type de ligne de paye "Déduction"
                    PAR_ACOMPTE = 8, ///< Base par type de ligne de paye "Acompte"
                    PAR_RAPPEL = 9, ///< Base par type de ligne de paye "Rappel"
                    PAR_RETENUE = 10, ///< Base par type de ligne de paye "Retenue"
                    PAR_COTISATION = 11, ///< Base par type de ligne de paye "Cotisation"
                    PAR_COMMENTAIRE = 12, ///< Base par type de ligne de paye "Commentaire"
                    NA = 13,
                    TOUTES_CATEGORIES = 14, ///< Base par type de ligne de paye "Toutes catégories"
                    MAXIMUM_LIGNES = 15,
                    MAXIMUM_LIGNES_PAR_ANNEE = 16
                  };

/// Nombre de type de champ de ligne de paye (Libellé, Code, Taux, Base, NbUnite, Montant, DebutPeriode, FinPeriode) moins 1
#define INDEX_MAX_COLONNNES 7

/// Nombre d'éléments de l'énum ci-dessous, correspondant aux champs des bulletins (répétés à chaque ligne de paye)
#define BESOIN_MEMOIRE_ENTETE  27

/// Enum des libellés de balises XML donnant lieu à extraction
typedef enum {
              Annee, Mois, Budget, Employeur, Siret, Etablissement,
              Nom, Prenom, Matricule, NIR, NbEnfants, Statut,
              EmploiMetier, Grade, Echelon, Indice, Code, Description, Service, NBI, QuotiteTrav,
              NbHeureTotal, NbHeureSup, MtBrut, MtNet, MtNetAPayer, Categorie
         } Entete;

/// Tableau des noms de colonnes associés à ces libellés de balises XML
constexpr const char* Tableau_entete[] = {
                                    "Annee", "Mois", "Budget", "Employeur", "Siret", "Etablissement",
                                    "Nom", "Prenom", "Matricule", "NIR", "NbEnfants", "Statut",
                                    "EmploiMetier", "Grade", "Echelon", "Indice", "Evenement", "Service", "NBI", "QuotiteTrav",
                                    "NbHeureTotal", "NbHeureSup", "MtBrut", "MtNet", "MtNetAPayer" };

/// Structure de stockage de l'information sur les lignes de paye
typedef struct
{

    vector<vector<xmlChar*>> Table;         ///< Contient les données XML à exporter dans le fichier Table
    uint64_t nbLigne;                       ///< Nombre de lignes
    vector<array<uint64_t, 3>> ligne_debut; ///< Ligne de début du bulletin dans le fichier XML
    vector<array<uint64_t, 2>> ligne_fin;   ///< Ligne de fin du bulletin dans le fichier XML
    vector<uint32_t> NAgent;                ///< Nombre d'agents
    uint32_t nbAgentUtilisateur;            ///< Nombre d'agents utilisateurs
    uint32_t NCumAgent;                     ///< Cumul du nombre d'agents pour l'opération de préallocation \ref calculer_memoire_requise
    uint32_t NCumAgentXml;                  ///< Cumul du nombre d'agents après décodage XML
    uint32_t taille_base;                   ///< Taille de la base
    BaseType  type_base;                    ///< Type de la base
    vector<uint16_t> NLigne;                ///< Nombre de lignes par agent
    thread_t* threads;                      ///< Structure thread_t permettant de communiquer une partie des données de paye à chaque thread.
    string chemin_log;                      ///< Chemin du log
    string expression_reg_elus;             ///< Expression régulière des élus (DEPRECATED)
    string chemin_base;                     ///< Chemin des bases CSV de type Table
    string chemin_bulletins;                ///< Chemin des bases CSV de type Bulletins.paie
    string export_mode;                     ///< Type d'exportation (standard, cumulatif, distributif, ...)
    vector<string> exclure_siret;           ///< Ne pas exporter le Siret
    vector<string> exclure_budget;          ///< Ne pas exporter le Budget
    vector<string> exclure_employeur;       ///< Ne pas exporter l'Employeur
    uint16_t nbLigneUtilisateur;            ///< Ne pas exporter le nombre de lignes utilisateur
    uint16_t fichier_courant;               ///< Rang du fichier courant
    char decimal;                           ///< Séparateur décimal
    char separateur;                        ///< Sépareteur de champ CSV
    bool reduire_consommation_memoire;      ///< Prédimensionner l'allocation mémoire des données (défaut)
    bool drapeau_cont;                      ///< TRUE (défaut) : continuer à analyser les données XML le plus possible en cas de léger défaut
    bool calculer_maxima;                   ///< Calculer les maxima de nombre de lignes de paye par agent et de nombre d'agents par mois
    bool generer_rang;                      ///< Générer un index dans un fichier temporaire permettant de raffraîchier une barre de progression d'interface graphique
    bool generer_bulletins;                 ///< Générer des bulletins de paye particuliers
    bool select_siret;                      ///< Sélectionner un SIRET particulier
    bool select_echelon;                    ///< Sélectionne run échelon particulier
    bool pretend;                           ///< Ne pas exporter de données
    bool verifmem;                          ///< Vérifier l'état de la mémoire
    bool cdrom;                             ///< Importer les données de paye directement depuis un disque optique
    unsigned int  nbfil;                    ///< Nombre de fils d'exécution
} info_t;

typedef struct {
     int nbLignePaye;  ///< Nombre de lignes de paye
     int memoire_p_ligne_allouee; ///< Mémoire à allouer par ligne
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
