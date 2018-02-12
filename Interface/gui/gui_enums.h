// Copyright Cour des comptes, 2017
// Contributeur :
// Fabrice Nicol, années 2012 à 2017
// fabrice.nicol@crtc.ccomptes.fr
//
// Ce logiciel est un programme informatique servant à extraire et analyser
// les fichiers de paye produits au format spécifié par l'annexe de la
// convention-cadre nationale de dématérialisation en vigueur à compter de
// l'année 2008.
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
// /////////////////////////////////////////////////////////////////////////


/// \file gui_enums.h
/// \author Fabrice Nicol
/// \brief Code de constantes et de classes regroupant des constantes

#ifndef GUI_ENUMS_H
#define GUI_ENUMS_H
#include <cstdint>

#ifndef STEP_UP
static constexpr const char* STEP_UP  =  "/../../";
#endif

/// Largeur minimale des lignes de l'onglet d'entrée du dialogue d'options (codes de paye). Détermine la largeur du dialogue

#ifndef MINIMUM_LINE_WIDTH
#  define MINIMUM_LINE_WIDTH 350
#endif

/// Nombre maximum de lignes d'avertissement par erreur de ligne de paye

#ifndef WARNING_LIMIT
#  define WARNING_LIMIT 5
#endif

/// Maximum du nombre de lignes de la sortie console si l'option de configuration "Limiter la sortie de la console" est cochée.

#ifndef MAXIMUM_CONSOLE_OUTPUT
#  define MAXIMUM_CONSOLE_OUTPUT 800
#endif

#ifndef TYPE_LOOP_LIMIT
#  define TYPE_LOOP_LIMIT 50
#endif

#ifndef SCRIPT_DIR

/// Répertoire principal des scripts R

#  define SCRIPT_DIR "Tests/Exemple/"
#endif

#ifndef DONNEES

///Répertoire d'insertion des répertoires  AltairDir et  DONNEES_XHL

#  define DONNEES  SCRIPT_DIR "Donnees/"
#endif

#ifndef DONNEES_XHL
#  define DONNEES_XHL  DONNEES "xhl"
#endif

/// Répertoire d'exportation des données CSV et des rapports d'analyse

#ifndef AltairDir
#  ifdef __linux__
#    define AltairDir  "R-Altair"
#  else
#    define AltairDir  "R-Altaïr"
#  endif
#endif

#define DONNEES_SORTIE  DONNEES AltairDir

/// Caractérise l'état d'actualisation de l'interface et les processus de raffraichissement
/// des différentes parties de l'interface.

class interfaceStatus
{
public:

    static constexpr std::uint16_t hasUnsavedOptions = 0x0;   ///< Caractérise des nouveaux paramètres non sauvegardés dans le projet \b .alt
    static constexpr std::uint16_t hasSavedOptions = 0x1;     ///< Caractérise des nouveaux paramètres sauvegardés dans le projet \b .alt
    static constexpr std::uint16_t hasSavedOptionsMask = 0xF; ///< Masque des nouveaux paramètres sauvegardés dans le projet \b .alt
    static constexpr std::uint16_t tree = 0x0010;             ///< Gestionnaire de projets à raffraichir à la première occasion
    static constexpr std::uint16_t treeMask = 0x00F0;         ///< Masque correspondant à  tree
    static constexpr std::uint16_t saveTree = 0x0100;         ///< Ouvrir le fichier projet pour le modifier
    static constexpr std::uint16_t saveTreeMask = 0x0F00;     ///< Masque correspondant à  saveTree
    static constexpr std::uint16_t mainTabs = 0x1000;         ///< Permet d'autoriser l'actualisation des valeurs de l'onglet central à partir du décodage du projet \b .alt (balise \code <XML> \endcode)
    static constexpr std::uint16_t tabMask = 0x7000;          ///< Masque correspondant à  mainTabs
    static constexpr std::uint16_t optionTabs = 0x2000;       ///< Permet d'autoriser l'actualisation des valeurs du dialogue d'options à partir du décodage du projet \b .alt
    static constexpr std::uint16_t keepOptionTabs = 0x4000;   ///< Force l'enregostrement des éléments de statut de l'interface non encore enregistrés dans un projet \b .alt
    static constexpr std::uint16_t parseXml = 0xF000;         ///< Autorise le décodage (parsing) du projet \b .alt
    static constexpr std::uint16_t parseXmlMask = 0xF000;     ///< Masque de interfaceStatus::parseXml
};

/// Décrit la portée des actualisations à opérer sur le gestionnaire de projets et le fichier projet \b .alt

class manager
{
public:

    static constexpr std::uint16_t refreshProjectInteractiveMask = 0xF000;  ///< Masque du comportament par défaut
    static constexpr std::uint16_t refreshProjectInteractiveMode = 0x1000;  ///< Défaut
    static constexpr std::uint16_t refreshXHLZone = 0x001;                  ///< Raffraîchir la zone de données XHL
    static constexpr std::uint16_t refreshSystemZone = 0x100;               ///< Raffraîchir la zone de données paramètres de l'interface
    static constexpr std::uint16_t refreshAllZones  =  manager::refreshXHLZone | manager::refreshSystemZone; ///< Tout raffraichir
};



#endif // GUI_ENUMS_H
