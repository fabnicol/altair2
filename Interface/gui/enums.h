#ifndef ENUMS_H
#define ENUMS_H
#include <cstdint>
#include "version.h"


#ifndef STEP_UP
    static constexpr const char* STEP_UP = "/../../";
#endif

#ifndef BUFFER_SIZE
    static constexpr int BUFFER_SIZE = 1500;
#endif

#ifndef MINIMUM_LINE_WIDTH
#  define MINIMUM_LINE_WIDTH 350
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

    enum class status : std::uint16_t {
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
        statusMask=0xFFF0
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



class interfaceStatus  {
  public:
    
  static constexpr  std::uint16_t hasUnsavedOptions=0x0;
  static constexpr std::uint16_t hasSavedOptions=0x1;
  static constexpr std::uint16_t hasSavedOptionsMask=0xF;
  static constexpr std::uint16_t tree=0x0010;
  static constexpr std::uint16_t treeMask=0x00F0;
  static constexpr std::uint16_t saveTree=0x0100;
  static constexpr std::uint16_t saveTreeMask=0x0F00;
  static constexpr std::uint16_t mainTabs=0x1000;
  static constexpr std::uint16_t tabMask=0x7000;
  static constexpr std::uint16_t optionTabs=0x2000;
  static constexpr std::uint16_t keepOptionTabs=0x4000;
  static constexpr std::uint16_t parseXml=0xF000;
  static constexpr std::uint16_t parseXmlMask=0xF000;
};

class manager  {
  
public:
    
    static constexpr std::uint16_t refreshProjectManagerFlag=0x000;
    static constexpr std::uint16_t refreshProjectXHLZoneMask=0x00F;
    static constexpr std::uint16_t refreshNBulletinsMask=0x0F0;
    static constexpr std::uint16_t refreshProjectSystemZoneMask=0xF00;
    static constexpr std::uint16_t refreshProjectInteractiveMask=0xF000;
    static constexpr std::uint16_t refreshXHLZone=0x001;
    static constexpr std::uint16_t refreshNBulletins=0x010;
    static constexpr std::uint16_t refreshSystemZone=0x100;
    static constexpr std::uint16_t refreshProjectInteractiveMode=0x1000;
    static constexpr std::uint16_t refreshAllZones = manager::refreshXHLZone| manager::refreshNBulletins | manager::refreshSystemZone;
};



#endif // ENUMS_H
