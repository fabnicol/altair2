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

/// \file validator.h
/// \author Fabrice Nicol
/// \brief Ce fichier contient notamment la structure info_t qui stocke les données de paye décodées
/// ainsi que la description des bases de sortie BaseType


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
#include "expression_reg_elus.h"
#include "expression_reg_cata.h"

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


/// Nom racine des fichiers Table....csv (Lignes de paye)
static constexpr auto NOM_BASE = "Table";

/// Nom racine des fichiers Bulletins.paie.csv
static constexpr auto NOM_BASE_BULLETINS = "Bulletins";

/// Extension CSV
static constexpr auto CSV = ".csv";

/// Enum des types de base (Bulletins ou Lignes de paye)
enum class BaseCategorie : int
{
    BASE = 0,  ///< Type de base CSV en sortie : Table.csv
    BULLETINS = 1 ///< Type de base CSV en sortie : Bulletins.paie.csv
};
/// Enum des types de base en sortie (tri par catégorie de ligne de paye)

enum class BaseType : int
{
    MONOLITHIQUE = -3, ///< Base complète, en un seul bloc
    PAR_ANNEE = -2, ///< Base par année
    PAR_AGENT = -1, ///< Une base par agent, par mois et par année ("Bulletins de paye")
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
    MAXIMUM_LIGNES = 15,     ///< Taille maximum de la base de paye "Table" en nombre de lignes.
    MAXIMUM_LIGNES_PAR_ANNEE = 16 ///< Taille maximum de la base de paye "Table" en nombre de lignes, cas d'une exportation annuelle.
};

/// Nombre de type de champ de ligne de paye (Libellé, Code, Taux, Base, NbUnite, Montant, DébutPeriode, FinPeriode) moins 1
#define INDEX_MAX_COLONNNES 7

/// Nombre d'éléments de l'énum ci-dessous, correspondant aux champs des bulletins (répétés à chaque ligne de paye)
#define BESOIN_MEMOIRE_ENTETE  34

/// Enum des libellés de balises XML donnant lieu à extraction
typedef enum
{
    Annee, Mois, Budget, Employeur, Siret, Etablissement,
    Civilite, Nom, Prenom, Matricule, Adresse, NIR, RefNomenStatutaire, NbEnfants, Statut,
    EmploiMetier, Grade, Echelon, Indice, CptBancaire, Code, Description, Service, NBI,
    CodeBudget, Taux, MtBudget, QuotiteTrav, // FPH si RepartitionBudget
    NbHeureTotal, NbHeureSup, MtBrut, MtNet, MtNetAPayer, Categorie
} Entete;

/// Tableau des noms de colonnes associés à ces libellés de balises XML
constexpr const char* Tableau_entete[] =
{
    "Année", "Mois", "Budget", "Employeur", "Siret", "Etablissement",
    "Nom", "Prénom", "Matricule", "NIR", "NbEnfants", "Statut",
    "EmploiMetier", "Grade", "Echelon", "Indice", "Evenement", "Service", "NBI", 
    "CodeBudget", "Taux", "MtBudget", "QuotiteTrav",
    "NbHeureTotal", "NbHeureSup", "MtBrut", "MtNet", "MtNetAPayer"
};

/// Structure de stockage de l'information sur les lignes de paye
typedef struct
{

    vector<vector<xmlChar*>> Table;         ///< Contient les données XML à exporter dans le fichier Table
    uint64_t nbLigne;                       ///< Nombre de lignes
    vector<array<uint64_t, 3>> ligne_debut; ///< Ligne de début du bulletin dans le fichier XML
    vector<array<uint64_t, 2>> ligne_fin;   ///< Ligne de fin du bulletin dans le fichier XML
    vector<uint32_t> NAgent;                ///< Nombre d'agents
    uint32_t nbAgentUtilisateur;            ///< Nombre d'agents utilisateurs
    uint32_t NCumAgent;                     ///< Cumul du nombre d'agents pour l'opération de préallocation  calculer_memoire_requise
    uint32_t NCumAgentXml;                  ///< Cumul du nombre d'agents après décodage XML
    uint32_t taille_base;                   ///< Taille de la base
    uint32_t largeur_base;                  ///< Drapeau pour le nombre de colonnes des bases
    BaseType  type_base;                    ///< Type de la base
    vector<uint16_t> NLigne;                ///< Nombre de lignes par agent
    thread_t* threads;                      ///< Structure thread_t permettant de communiquer une partie des données de paye à chaque thread.
    string chemin_log;                      ///< Chemin du log
#ifdef INCLURE_REG_ELUS    
    string expression_reg_elus;             ///< Expression régulière des élus (DEPRECATED)
#endif    
    string chemin_base;                     ///< Chemin des bases CSV de type Table
    string chemin_bulletins;                ///< Chemin des bases CSV de type Bulletins.paie
    string export_mode;                     ///< Type d'exportation (standard, cumulatif, distributif, ...)
    vector<string> chemins_bulletins_extraits; ///< Vecteur des chemins des bulletins de paye XML extraits des bases XHL/XML originales
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
    bool generer_repartition_budget;        ///< Exporter la répartition budgétaire (surtout pertinent pour la FPH)
    bool pretend;                           ///< Ne pas exporter de données
    bool verifmem;                          ///< Vérifier l'état de la mémoire
    bool cdrom;                             ///< Importer les données de paye directement depuis un disque optique
    bool inserer_bom;                       ///< Insérer un BOM UTF-8 dans les tables CSV exportées (vrai par défaut)
    unsigned int  nbfil;                    ///< Nombre de fils d'exécuSTATE_HTML_TAGtion
} info_t;

typedef struct
{
    int nbLignePaye;  ///< Nombre de lignes de paye
    int memoire_p_ligne_allouee; ///< Mémoire à allouer par ligne
} LineCount;

/// Recommencer l'allocation de mémoire
#define RETRY -1

/// Sauter le fichier en cours
#define SKIP_FILE 1

/// Pas d'agent détecté
#define NO_AGENT -1

#ifndef MAX_MEMORY_SHARE
#define MAX_MEMORY_SHARE 0.9
#endif

#ifndef NA_STRING
#ifdef NA_STRING_IS_EMPTY
/// Caractérisation des non-réponses ou variables non renseignées
  #define NA_STRING  (xmlChar*) ""
#else
/// Caractérisation des non-réponses ou variables non renseignées
  #define NA_STRING  (xmlChar*) "NA"
#endif
#endif

#ifndef MAX_LIGNES_PAYE
/// Maximum de lignes de paye par agent par défaut
#define MAX_LIGNES_PAYE 1000
#endif

// MAX_NB_AGENTS détermine le nombre maximal d'agents par mois potentiellement traités

#ifndef MAX_NB_AGENTS
/// Maximum de nombre d'agents par mois par défaut
#define MAX_NB_AGENTS 8000
#endif

/// Nombre de colonnes standard, étendu et maximal (drapeaux) pour les bases en sortie

#define LARGEUR_STD 0
#define LARGEUR_EXT 1
#define LARGEUR_MAX 2

#ifndef NO_DEBUG
/// Fonctions de débogage
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

/// Un noeud libxml2 XmlPtr a été trouvé
#define NODE_FOUND  0

/// Aucun noeud libxml2 XmlPtr n'a été trouvé
#define NODE_NOT_FOUND 1

/// Problème d'allocation de mémoire pour une ligne de paye
#define LINE_MEMORY_EXCEPTION 2

/// Pas de noeud libxml2 suivant au même niveau de profondeur d'arbre
#define NO_NEXT_ITEM 3


/// Tableau décrivant les libellés des balises XML du format XHL de la DGFIP
/// Voir site Xemelios de la DGFIP
static const char* type_remuneration[]   =
{
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


/// nbType donne le nombre d'items du tableau précédent
static constexpr const int nbType                  = sizeof (type_remuneration) / sizeof (char*); // + NA

/// drapeau est un tableau de paires permettant d'isoler en mémoire les balises de type_remuneration lorsqu'elles sont
/// rencontrées dans un fichier XHL/XML. Le nombre d'items est donc nbType

static constexpr const xmlChar drapeau[][2]  = {{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0}, {11, 0}, {12, 0}};

void* decoder_fichier (info_t& tinfo);
void* parse_info (info_t& info);


/// Permet d'atteindre un noeud donné par son libellé de balise XML à partir d'un pointeur XmlNodePtr de libxml2
/// \param noeud Libellé de la balise à atteindre
/// \param cur Noeud libxml2 courant
/// \return Soit le noeud XmlNodePtr correspondant au noeud trouvé, soit nullptr si pas de noeud trouvé.

static inline xmlNodePtr GCC_INLINE atteindreNoeud (const char * noeud, xmlNodePtr cur)
{

#       ifdef DEBUG_ATTEINDRE

    cerr << "[DEBUG] --- Recherche de " << noeud <<  ENDL;
#       endif

    while (cur && xmlIsBlankNode (cur))
        {
            cur = cur -> next;
#       ifdef DEBUG_ATTEINDRE
            cerr << "[DEBUG] Saut de noeud blanc" << ENDL;
#       endif
        }


    while (cur != nullptr && xmlStrcmp (cur->name,  (const xmlChar*) noeud))
        {
#       ifdef DEBUG_ATTEINDRE
            //       cerr << "[DEBUG]      ......" << cur->name <<  ENDL;
#       endif

            cur = cur->next;
        }

    if (cur == nullptr)
        {
            AFFICHER_NOEUD (noeud) // cur->name == noeud
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
