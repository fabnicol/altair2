
// Auteur : Fabrice Nicol
// Copyright Fabrice Nicol
// Contributeur :
// Fabrice Nicol, années 2008 à 2012, 2017
// fabrnicol@gmail.com
//
// Ce logiciel est régi par les dispositions du code de la propriété
// intellectuelle (CPI).

// L'auteur se réserve le droit d'exploitation du présent logiciel, 
// et notamment de reproduire et de modifier le logiciel, conformément aux 
// dispositions de l'article L.122-6 du même code. L'auteur se réserve le droit
// de modifier et de corriger les erreurs du logiciel, conformément aux termes 
// du deuxième alinéa du I de l'article L.122-6-1 du même code.

// Le présent logiciel doit être regardé comme un "Module Externe", pour 
// l'application des stipulations de la licence CeCILL, lorsque le code source, 
// ou une bibliothèque compilée à partir du présent code, sont utilisés pour 
// compiler tout logiciel faisant usage du présent code.

// Tout fichier contenant le présent code, ou des versions antérieures du 
// présent code, et faisant apparaître des termes contraires aux stipulations
// précédentes doit être regardé comme faisant partie d'actes préparatoires à
// l'élaboration du présent logiciel et non comme le logiciel lui-même, ou 
// comme des erreurs matérielles dans la copie des licences. Seules les 
// stipulations précédentes doivent être regardées comme s'appliquant au 
// présent logiciel.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
// pris connaissance de ces stipulations et que vous en avez accepté les
// termes.

// Sans préjudice des dispositions du CPI, une autorisation d'usage et de
// reproduction du présent code est donnée à tout agent employé par les
// juridictions financières pour l'exercice de leurs fonctions publiques.
// Le code ainsi mis à disposition ne peut être transmis à d'autres utilisateurs.
//

/// \file enums.h
/// \author Fabrice Nicol
/// \date Janvier 2010
/// \brief Enumérations et constantes permettant de paramétrer les fwidgets

#ifndef ENUMS_FWIDGETS_H
#define ENUMS_FWIDGETS_H

#ifndef BUFFER_SIZE
    static constexpr int BUFFER_SIZE = 1500;
#endif

enum exitCode {exitFailure=EXIT_FAILURE, shouldLaunchRAltairAlone=13};

class flags
{
public:

   enum class flineframe {isFilePath,     ///< Une ligne d'input qui est  un chemin de fichier
                          isDirectoryPath ///< Une ligne d'input qui est un chemin de répertoire
                         };

   enum {importFiles,  ///< Importer des fichiers
         importNames,  ///< Importer des libellés
         typeIn};      ///< Information à saisir

   enum class colors {yes, ///< Coloriser (palette fixe colorée
                      no}; ///< Ne pas coloriser

   enum class font : std::uint8_t {boldTitle,     ///< Police en gras
                                   regularTitle,  ///< Police normale
                                   italicTitle};  ///< Police en italiques

   enum class directory : bool {noCheck = false,       ///< Ne pas vérifier si le répertoire est vide
                                checkEmptyness = true  ///< Vérifier si le répertoire est videv
                               } ;

   enum class commandLineType : std::uint8_t {
                                                coreApplicationCommandLine = 0x01,               ///< Ligne de commande de l'application noyau externe0
                                                defaultCommandLine = coreApplicationCommandLine, ///< Par défaut
                                                noCommandLine = 0x03,                            ///< Pas de ligne de commande assicoée au fwidget
                                                commandLineMask = 0xF                           ///< Masque permettant de récupérer commandLineType sur un multi-octets
                                             };

   enum class status : std::uint32_t {
       enabled = 0x100,              ///< Activer le composant fonctionnelpar défaut
       disabled = 0x200,             ///< Désactiver composant fonctionnelpar défaut
       checked = 0x1000,             ///< Cocher par défaut
       unchecked = 0x2000,           ///< Décocher par défaut
       multimodal = 0x3000,          ///< composant multi-modes
       enabledChecked = 0x1100,      ///< Raccourci pour enabled & checked
       enabledUnchecked = 0x2100,    ///< Raccourci pour enabled & unchecked
       hasListCommandLine = 0x4000,  ///< Profondeur au moins 2
       defaultStatus = enabled,      ///< Statut par défaut
       excluded = 0x10000,           ///< Exclure le composant fonctionnelde la liste Abstract::abstractWidgetList
       enabledMask = 0xF00,          ///< Masque permettant de récupérer enabled
       widgetMask = 0xF000,          ///< Masque permettant de récupérer checked
       statusMask = 0xFFF0,          ///< Masque permettant de récupérer checked et enabled
       excludeMask = 0xF0000         ///< Masque permettant de récupérer excluded
   };

   /// Plongement de l'opérateur | du type int et du type flag::status dans les entiers int
   friend int operator | (int  x, flags::status y) {return x | static_cast<int>(y);}

   /// Plongement de l'opérateur & du type int et du type flag::status dans les entiers int
   friend flags::status operator & (int  x, flags::status y) {return y & static_cast<flags::status>(x);}

   /// Plongement de l'opérateur & du type flag::status dans les entiers int
   friend flags::status operator & (flags::status  x, flags::status y) {return static_cast<flags::status>(static_cast<int>(y) & static_cast<int>(x));}

   /// Plongement de l'opérateur | des types flags::commandLineType et flag::status dans les entiers int
   friend int operator | (flags::commandLineType x, flags::status y) {return static_cast<int>(x) | static_cast<int>(y);}

   /// Plongement de l'opérateur | du type flag::status et du type flags::commandLineType dans les entiers int
   friend int operator | (flags::status y, flags::commandLineType x) {return static_cast<int>(x) | static_cast<int>(y);}

   /// Plongement de l'opérateur & du type flag::status et du type flags::commandLineType dans les entiers int
   friend int operator & (flags::status y, flags::commandLineType x) {return static_cast<int>(x) & static_cast<int>(y);}

   /// Plongement de l'opérateur & du type flags::commandLineType et du type flag::status dans les entiers int
   friend int operator & (flags::commandLineType x, flags::status y) {return static_cast<int>(x) & static_cast<int>(y);}
};

enum actionType {Select,     ///< Sélectionner un répertoire ou un fichier
                 OpenFolder, ///< Ouvrir un répertoire
                 BrowseFile  ///< OUvrir un fichier
                };


#endif // ENUMS_FWIDGETS_H
