!include "Sections.nsh"
!include "MUI2.nsh"
!include "x64.nsh"
!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
!include "StrFunc.nsh"
!include "EnvVarUpdate.nsh"
;!addplugindir "c:\Program Files (x86)\NSIS\Plugins"
  
; Num�ros de version 

; pour une version minimal d�finir minimal par "" sinon "_min"

!define version  "2015.10.correctif 01"
!define VER_MAJOR 2015
!define VER_MINOR 10
!define VER_REVISION 1
!define VER_BUILD  0
!define Rversion_major "3.2"

; autres d�finitions

!define prodname     "Alta�r"
!define prodname.simple "Altair"
!define processeur       "_core2"
!define setup        "Alta�r-${version}.win$processeur.installer.exe"
!define exemple      "${prodname.simple}\Tests\Exemple"
!define xhl          "${exemple}\Donnees\xhl"
!define Interface.minimal    Interface_windows_min
!define Interface    Interface_windows
!define icon         neptune.ico
!define RDir         "R"
!define texDir       "texlive"
!define RStudioDir   "RStudio"
!define GitDir       "Git"
!define startmenu    "$SMPROGRAMS\${prodname}-${version}"
!define D�sinstaller "D�sinstaller.exe"
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
 
 Section
 StrCpy $minimal "_min"
 SectionEnd

 InstallDir "C:\Users\Public" 
 InstallDirRegKey HKLM "SOFTWARE\${prodname}" "Install_Dir"
   
!insertmacro MUI_PAGE_WELCOME  
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_LANGUAGE "French" 

 LangString wizard1 ${LANG_FRENCH}   "Installation du logiciel Analyse des lignes de traitement, attributions indemnitaires et r�mun�rations diverses. Appuyer sur suivant pour continuer."
 LangString wizard2 ${LANG_FRENCH}   "Installation du logiciel Alta�r version ${version}"
 LangString title1 ${LANG_FRENCH}    "Lisez-moi"
 LangString text1 ${LANG_FRENCH}     "Alta�r ${version} va �tre install� dans $INSTDIR. Cliquer sur Compl�ments pour des pr�cisions sur l'installation."
 LangString title2 ${LANG_FRENCH}    "Installation"
 LangString text2 ${LANG_FRENCH}     "Compl�ments d'information sur l'installation."
 LangString title3 ${LANG_FRENCH}    "Installation du gestionnaire de versions local"
 LangString text3 ${LANG_FRENCH}     "Un d�p�t local peut �tre install� dans $INSTDIR pour suivre l'�volution du code source en temps r�el. Cliquer sur Terminer pour lancer l'installation du logiciel et de ses d�pendances."
 LangString D�sinstaller ${LANG_FRENCH}     "d�sinstallation du logiciel ${prodname} "
 LangString completed ${LANG_FRENCH} "Termin�."
 LangString uninst_completed ${LANG_FRENCH} "D�sinstallation termin�e"
 LangString Sec1Name ${LANG_FRENCH}  "Alta�r"
 LangString DESC_sec1 ${LANG_FRENCH} "Installer Alta�r"
 LangString AdvancedName ${LANG_FRENCH}  "Installation avanc�e"
 LangString MinimaleName ${LANG_FRENCH}  "Installation minimale"
 LangString Sec6Name ${LANG_FRENCH}  "Exemples"
 
 LangString DESC_Advanced ${LANG_FRENCH} "Installer la version avanc�e (Alta�r, Git, R, RStudio et Texlive)"
 LangString DESC_Minimale ${LANG_FRENCH} "Installer la version minimale (Alta�r, R)"
 LangString DESC_sec6 ${LANG_FRENCH} "Installer les exemples"
 
 
 LangString Message  ${LANG_FRENCH}  "Appuyer sur Oui pour installer Alta�r"
  
 LicenseLangString myLicenseData ${LANG_FRENCH} "${prodname.simple}\LICENCE"
 LicenseData $(myLicenseData)
 LangString Name ${LANG_FRENCH}  "Alta�r"
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
  StrCpy $1 "Minimale"
  Call .onSelChange
  Call .onSelChange
  
FunctionEnd
 

Section
  
  CreateDirectory $LOCALAPPDATA\${prodname}   
  CreateDirectory $INSTDIR\${prodname.simple}\lhx   
  
  SetDetailsPrint both
  SetOutPath $INSTDIR\${Exemple}
  File   "${Exemple}\*.R" 
  SetOutPath $INSTDIR\${prodname.simple}\win
  File   "${prodname.simple}\win${processeur}\*.*" 
          
SectionEnd

 Section /o $(AdvancedName) Advanced
    SetOutPath $INSTDIR\${prodname.simple}\${Interface}
    File /r  "${prodname.simple}\${Interface}${processeur}\*.*" 
	StrCpy $minimal ""
 SectionEnd

 Section   $(MinimaleName) Minimale 
    SetOutPath $INSTDIR\${prodname.simple}\${Interface.minimal}
    
    File /r  "${prodname.simple}\${Interface.minimal}${processeur}\*.*" 
	
	StrCpy $minimal "_min"

	
 SectionEnd

  
 

Function .onSelChange
  !insertmacro StartRadioButtons $1
    !insertmacro RadioButton ${Advanced}
    !insertmacro RadioButton ${Minimale}
!insertmacro EndRadioButtons
FunctionEnd

 
 !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
   !insertmacro MUI_DESCRIPTION_TEXT ${sec1} $(DESC_sec1)
   !insertmacro MUI_DESCRIPTION_TEXT ${Advanced} $(DESC_Advanced)
   !insertmacro MUI_DESCRIPTION_TEXT ${Minimale} $(DESC_Minimale)
  
 !insertmacro MUI_FUNCTION_DESCRIPTION_END
  

BrandingText "Alta�r-${version}"

; eof
