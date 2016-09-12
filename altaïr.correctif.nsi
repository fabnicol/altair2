!include "Sections.nsh"
!include "MUI2.nsh"
!include "x64.nsh"
!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
!include "StrFunc.nsh"
!include "EnvVarUpdate.nsh"
;!addplugindir "c:\Program Files (x86)\NSIS\Plugins"
  
; Numéros de version 

; pour une version minimal définir minimal par "" sinon "_min"

!define version  "2015.10.correctif 01"
!define VER_MAJOR 2015
!define VER_MINOR 10
!define VER_REVISION 1
!define VER_BUILD  0
!define Rversion_major "3.2"

; autres définitions

!define prodname     "Altaïr"
!define prodname.simple "Altair"
!define processeur       "_core2"
!define setup        "Altaïr-${version}.win$processeur.installer.exe"
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
 
 Section
 StrCpy $minimal "_min"
 SectionEnd

 InstallDir "C:\Users\Public" 
 InstallDirRegKey HKLM "SOFTWARE\${prodname}" "Install_Dir"
   
!insertmacro MUI_PAGE_WELCOME  
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_LANGUAGE "French" 

 LangString wizard1 ${LANG_FRENCH}   "Installation du logiciel Analyse des lignes de traitement, attributions indemnitaires et rémunérations diverses. Appuyer sur suivant pour continuer."
 LangString wizard2 ${LANG_FRENCH}   "Installation du logiciel Altaïr version ${version}"
 LangString title1 ${LANG_FRENCH}    "Lisez-moi"
 LangString text1 ${LANG_FRENCH}     "Altaïr ${version} va être installé dans $INSTDIR. Cliquer sur Compléments pour des précisions sur l'installation."
 LangString title2 ${LANG_FRENCH}    "Installation"
 LangString text2 ${LANG_FRENCH}     "Compléments d'information sur l'installation."
 LangString title3 ${LANG_FRENCH}    "Installation du gestionnaire de versions local"
 LangString text3 ${LANG_FRENCH}     "Un dépôt local peut être installé dans $INSTDIR pour suivre l'évolution du code source en temps réel. Cliquer sur Terminer pour lancer l'installation du logiciel et de ses dépendances."
 LangString Désinstaller ${LANG_FRENCH}     "désinstallation du logiciel ${prodname} "
 LangString completed ${LANG_FRENCH} "Terminé."
 LangString uninst_completed ${LANG_FRENCH} "Désinstallation terminée"
 LangString Sec1Name ${LANG_FRENCH}  "Altaïr"
 LangString DESC_sec1 ${LANG_FRENCH} "Installer Altaïr"
 LangString AdvancedName ${LANG_FRENCH}  "Installation avancée"
 LangString MinimaleName ${LANG_FRENCH}  "Installation minimale"
 LangString Sec6Name ${LANG_FRENCH}  "Exemples"
 
 LangString DESC_Advanced ${LANG_FRENCH} "Installer la version avancée (Altaïr, Git, R, RStudio et Texlive)"
 LangString DESC_Minimale ${LANG_FRENCH} "Installer la version minimale (Altaïr, R)"
 LangString DESC_sec6 ${LANG_FRENCH} "Installer les exemples"
 
 
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
  

BrandingText "Altaïr-${version}"

; eof
