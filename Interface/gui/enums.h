#ifndef ENUMS_H
#define ENUMS_H
#include <cstdint>

class flags
{
public:
    enum {flush=0xF00};
    enum {importFiles, importNames, typeIn, isEmbedded};

    enum class font : std::uint8_t {boldTitle, regularTitle, italicTitle};

    enum class commandLineType : std::uint8_t {altairCommandLine,
                                               defaultCommandLine=altairCommandLine,
                                               RAltairFiles,
                                               noCommandLine,
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

#ifdef _WIN32
#define CONSTEXPR const
#else
#define CONSTEXPR constexpr
#endif

class interfaceStatus  {
  public:
    
  static CONSTEXPR  std::uint16_t hasUnsavedOptions=0x0;
  static CONSTEXPR std::uint16_t hasSavedOptions=0x1;
  static CONSTEXPR std::uint16_t hasSavedOptionsMask=0xF;
  static CONSTEXPR std::uint16_t tree=0x0010;
  static CONSTEXPR std::uint16_t treeMask=0x00F0;
  static CONSTEXPR std::uint16_t saveTree=0x0100;
  static CONSTEXPR std::uint16_t saveTreeMask=0x0F00;
  static CONSTEXPR std::uint16_t mainTabs=0x1000;
  static CONSTEXPR std::uint16_t tabMask=0x7000;
  static CONSTEXPR std::uint16_t optionTabs=0x2000;
  static CONSTEXPR std::uint16_t keepOptionTabs=0x4000;
  static CONSTEXPR std::uint16_t parseXml=0xF000;
  static CONSTEXPR std::uint16_t parseXmlMask=0xF000;
};

class manager  {
  
public:
    
    static CONSTEXPR std::uint16_t refreshProjectManagerFlag=0x000;
    static CONSTEXPR std::uint16_t refreshProjectXHLZoneMask=0x00F;
    static CONSTEXPR std::uint16_t refreshNBulletinsMask=0x0F0;
    static CONSTEXPR std::uint16_t refreshProjectSystemZoneMask=0xF00;
    static CONSTEXPR std::uint16_t refreshProjectInteractiveMask=0xF000;
    static CONSTEXPR std::uint16_t refreshXHLZone=0x001;
    static CONSTEXPR std::uint16_t refreshNBulletins=0x010;
    static CONSTEXPR std::uint16_t refreshSystemZone=0x100;
    static CONSTEXPR std::uint16_t refreshProjectInteractiveMode=0x1000;
    static CONSTEXPR std::uint16_t refreshAllZones = manager::refreshXHLZone| manager::refreshNBulletins | manager::refreshSystemZone;
};

enum exitCode {exitFailure=EXIT_FAILURE, noAudioFiles=10, isoTooSmall=11, mkisofsSanityCheck=12, shouldLaunchRAltairAlone=13};

#endif // ENUMS_H
