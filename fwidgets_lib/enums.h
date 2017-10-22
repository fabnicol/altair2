
// Auteur : Fabrice Nicol
// Copyright Fabrice Nicol
// Contributeur :
// Fabrice Nicol, années 2008 à 2012, 2017
// fabrnicol@gmail.com
//
// Ce logiciel est régi par les dispositions du code de la propriété
// intellectuelle. 

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

// Pour l'année 2017, une autorisation d'usage, de modification et de 
// reproduction du présent code est donnée à tout agent employé par les
// juridictions financières. Cette autorisation est temporaire et peut être 
// révoquée.
//
//
#ifndef ENUMS_FWIDGETS_H
#define ENUMS_FWIDGETS_H


#ifndef BUFFER_SIZE
    static constexpr int BUFFER_SIZE = 1500;
#endif

enum exitCode {exitFailure=EXIT_FAILURE, noAudioFiles=10, isoTooSmall=11, mkisofsSanityCheck=12, shouldLaunchRAltairAlone=13};

class flags
{
public:

   enum class flineframe {isFilePath, isDirectoryPath};
   enum {importFiles, importNames, typeIn, isEmbedded};
   enum class colors {yes, no};
   enum class font : std::uint8_t {boldTitle, regularTitle, italicTitle};
   enum class directory : bool {noCheck = false, checkEmptyness = true } ;
   enum class commandLineType : std::uint8_t {
                                                altairCommandLine = 0x01,
                                                defaultCommandLine=altairCommandLine,
                                                RAltairFiles = 0x02,
                                                noCommandLine = 0x03,
                                                commandLinewidgetDepthMask=0xF,
                                                commandLineMask=0xF,
                                             };

   enum class status : std::uint32_t {
       untoggledCommandLine=0x10,
       toggledCommandLine=0x20,
       enabled=0x100,
       disabled=0x200,
       checked=0x1000,
       unchecked=0x2000,
       multimodal=0x3000,
       enabledChecked=0x1100,
       enabledUnchecked=0x2100,
       hasListCommandLine=0x4000,
       defaultStatus=enabled,
       commandLineToggleMask=0xF0,
       enabledMask=0xF00,
       widgetMask=0xF000,
       statusMask=0xFFF0,
       excluded = 0x10000,
       excludeMask = 0xF0000
   };

   friend int operator | (int  x, flags::status y) {return x | static_cast<int>(y);}
   friend flags::status operator & (int  x, flags::status y) {return y & static_cast<flags::status>(x);}
   friend flags::status operator & (flags::status  x, flags::status y) {return static_cast<flags::status>(static_cast<int>(y) & static_cast<int>(x));}
   friend int operator | (flags::commandLineType x, flags::status y) {return static_cast<int>(x) | static_cast<int>(y);}
   friend int operator | (flags::status y, flags::commandLineType x) {return static_cast<int>(x) | static_cast<int>(y);}
   friend int operator & (flags::status y, flags::commandLineType x) {return static_cast<int>(x) & static_cast<int>(y);}
   friend int operator & (flags::commandLineType x, flags::status y) {return static_cast<int>(x) & static_cast<int>(y);}

   static int RAltairRank;

};

enum actionType {Select, OpenFolder, BrowseFile};


#endif // ENUMS_FWIDGETS_H
