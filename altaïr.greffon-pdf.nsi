!include "Sections.nsh"
!include "MUI2.nsh"
!include "x64.nsh"
!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
!include "StrFunc.nsh"
!include "EnvVarUpdate.nsh"
;!addplugindir "c:\Program Files (x86)\NSIS\Plugins"
; Numéros de version 

!define version  "2015.10.greffon-pdf"
!define VER_MAJOR 2015
!define VER_MINOR 10
!define VER_REVISION 2
!define VER_BUILD  0

; autres définitions

!define prodname     "Altaïr"
!define prodname.simple "Altair"
!define processeur   "_core2"
!define setup        "Altaïr-${version}.win.installer.exe"
!define exemple      "${prodname.simple}\Tests\Exemple"
!define Interface    Interface_windows
!define icon         neptune.ico
!define RDir         "R"
!define texDir       "texlive"
!define RStudioDir   "RStudio"
!define GitDir       "Git"
!define startmenu    "$SMPROGRAMS\${prodname}-${version}"
!define Désinstaller "Désinstaller.exe"
!define notefile     "${prodname.simple}\LISEZ-MOI.txt"
!define installfile  "${prodname.simple}\INSTALLATION.txt"

!define REG_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}"
!define MEMENTO_REGISTRY_ROOT HKLM
!define MEMENTO_REGISTRY_KEY "${REG_UNINST_KEY}"

!define MUI_ICON     "${exemple}\${icon}"
!define MUI_WELCOMEFINISHPAGE
!define MUI_WELCOMEPAGE_TEXT  $(wizard1)
!define MUI_WELCOMEPAGE_TITLE $(wizard2)
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${exemple}\${prodname}.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${exemple}\neptune.bmp"
!define MUI_ABORTWARNING
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Modern UI Test" 
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

 Var git
 Var StartMenuFolder
 Var minimal
  
 InstallDir "C:\Users\Public" 
 InstallDirRegKey HKLM "SOFTWARE\${prodname}" "Install_Dir"
   
!insertmacro MUI_PAGE_WELCOME  
!insertmacro MUI_LANGUAGE "French" 

 LangString wizard1 ${LANG_FRENCH}   "Installation du logiciel Analyse des lignes de traitement, attributions indemnitaires et rémunérations diverses. Appuyer sur suivant pour continuer."
 LangString wizard2 ${LANG_FRENCH}   "Installation du logiciel Altaïr version ${version}"
 LangString title1 ${LANG_FRENCH}    "Lisez-moi"
 LangString text1 ${LANG_FRENCH}     "Altaïr ${version} va être installé dans $INSTDIR. Cliquer sur Compléments pour des précisions sur l'installation."
 LangString title2 ${LANG_FRENCH}    "Installation"
 LangString text2 ${LANG_FRENCH}     "Compléments d'information sur l'installation."
 LangString Désinstaller ${LANG_FRENCH}     "désinstallation du logiciel ${prodname} "
 LangString completed ${LANG_FRENCH} "Terminé."
 LangString uninst_completed ${LANG_FRENCH} "Désinstallation terminée"
 LangString Sec1Name ${LANG_FRENCH}  "Altaïr"
 LangString DESC_sec1 ${LANG_FRENCH} "Installer Altaïr"
   
 LangString Message  ${LANG_FRENCH}  "Appuyer sur Oui pour installer Altaïr"
  
 LicenseLangString myLicenseData ${LANG_FRENCH} "${prodname.simple}\LICENCE"
 LicenseData $(myLicenseData)
 LangString Name ${LANG_FRENCH}  "Altaïr"
 Name $(Name)

; MUI macros   

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_INSTFILES

CompletedText  $(completed)

;ShowInstDetails show
;ShowUninstDetails show
;RequestExecutionLevel user

Caption ""

OutFile "${setup}"

SetDateSave on
SetDatablockOptimize on
CRCCheck on
SilentInstall normal

Icon "${prodname.simple}\${Interface}\${icon}"

RequestExecutionLevel user
AutoCloseWindow false

Function .onInit
 
  SetOutPath $TEMP	
  File /oname=spltmp.bmp "${prodname.simple}\spltmp.bmp"

  advsplash::show 2300 600 400 -1 $TEMP\spltmp

  Pop $0 ; $0 has '1' if the user closed the splash screen early,
         ; '0' if everything closed normally, and '-1' if some error occurred.

  Delete $TEMP\spltmp.bmp
   
FunctionEnd

Section
      
  SetDetailsPrint both
  
  
  SetOutPath $INSTDIR\${prodname.simple}
  File  /r "${prodname.simple}\texlive"
          
SectionEnd

 
BrandingText "Altaïr-${version}"

; eof
