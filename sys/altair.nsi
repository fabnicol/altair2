!include "Sections.nsh"
!include "MUI2.nsh"
!include "x64.nsh"
!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
!include "StrFunc.nsh"

;!addplugindir "c:\Program Files (x86)\NSIS\Plugins"
  
; Numéros de version 



!define version  "2021.06-4"
!define processeur  core
!define VER_MAJOR 2018
!define VER_MINOR 06
!define VER_REVISION 0
!define VER_BUILD  0
!define Rversion_major "4.0.2"

; autres définitions

!define prodname     "Altaïr"
!define prodname.simple "Altair"
!define setup        "Altaïr-${version}.win.${processeur}.installer.exe"
!define exemple      "scripts\R"

!define Interface    Interface_windows
!define icon         neptune.ico
!define RDir         "R-devel"
!define nppDir       "npp"
!define texDir       "texlive2"
!define RStudioDir   "RStudio"
!define startmenu    "$SMPROGRAMS\${prodname}-${version}"
!define Désinstaller "Désinstaller.exe"
!define notefile     "${prodname.simple}\LISEZ-MOI.txt"


!define REG_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}"
!define MEMENTO_REGISTRY_ROOT HKLM
!define MEMENTO_REGISTRY_KEY "${REG_UNINST_KEY}"

!define MUI_ICON     "${prodname.simple}\${exemple}\icones\${icon}"
!define MUI_WELCOMEFINISHPAGE
!define MUI_WELCOMEPAGE_TEXT  $(wizard1)
!define MUI_WELCOMEPAGE_TITLE $(wizard2)
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${prodname.simple}\${exemple}\icones\${prodname}.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${prodname.simple}\${exemple}\icones\neptune.bmp"
!define MUI_ABORTWARNING
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Modern UI Test" 
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

 
 Var StartMenuFolder
 

 InstallDir "C:\Users\Public" 
 InstallDirRegKey HKLM "SOFTWARE\${prodname}" "Install_Dir"
   
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
!insertmacro MUI_PAGE_WELCOME  
!insertmacro MUI_LANGUAGE "French" 

 LangString wizard1 ${LANG_FRENCH}   "Installation du logiciel Analyse des lignes de traitement, attributions indemnitaires et rémunérations diverses. Appuyer sur suivant pour continuer."
 LangString wizard2 ${LANG_FRENCH}   "Installation du logiciel Altaïr version ${version}"
 LangString title1 ${LANG_FRENCH}    "Lisez-moi"
 LangString text1 ${LANG_FRENCH}     "Altaïr ${version} va être installé dans $INSTDIR. "
 LangString title2 ${LANG_FRENCH}    "Installation"
 LangString Désinstaller ${LANG_FRENCH}     "désinstallation du logiciel ${prodname} "
 LangString completed ${LANG_FRENCH} "Terminé."
 LangString uninst_completed ${LANG_FRENCH} "Désinstallation terminée"
 LangString Sec1Name ${LANG_FRENCH}  "Altaïr"
 LangString DESC_sec1 ${LANG_FRENCH} "Installer Altaïr"
 LangString AdvancedName ${LANG_FRENCH}  "Installation avancée"
 
 
 LangString DESC_Advanced ${LANG_FRENCH} "Installer la version avancée (Altaïr, R, RStudio et Texlive)"
  
 
 LangString Message  ${LANG_FRENCH}  "Appuyer sur Oui pour installer Altaïr"
  
 LicenseLangString myLicenseData ${LANG_FRENCH} "${prodname.simple}\LICENCE"
 LicenseData $(myLicenseData)
 LangString Name ${LANG_FRENCH}  "Altaïr"
 Name $(Name)

; MUI macros   

!insertmacro MUI_PAGE_LICENSE "${prodname.simple}\LICENCE"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS

!define MUI_FINISHPAGE_TITLE $(title1)
!define MUI_FINISHPAGE_TEXT  $(text1)
!define MUI_FINISHPAGE_RUN 
!define MUI_FINISHPAGE_RUN_NOTCHECKED
!define MUI_FINISHPAGE_RUN_TEXT     "Lire le fichier LISEZ-MOI"
!define MUI_FINISHPAGE_RUN_FUNCTION "Launch_LISEZ"
!define MUI_FINISHPAGE_BUTTON       "Compléments"
!define MUI_FINISHPAGE_CANCEL_ENABLED 
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_INSTFILES

UninstallText  $(uninstall)
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

Icon "${prodname.simple}\${exemple}\icones\${icon}"

RequestExecutionLevel user
AutoCloseWindow false

Function .onInit
 
  SetOutPath $TEMP	
  File /oname=spltmp.bmp "${prodname.simple}\${exemple}\icones\spltmp.bmp"

  advsplash::show 2300 600 400 -1 $TEMP\spltmp

  Pop $0 ; $0 has '1' if the user closed the splash screen early,
         ; '0' if everything closed normally, and '-1' if some error occurred.

  Delete $TEMP\spltmp.bmp

 
FunctionEnd
 
 
Function Launch_LISEZ
  SetOutPath $INSTDIR
  File  "${notefile}"  
  Exec '"notepad" "$INSTDIR\LISEZ-MOI.txt"'
FunctionEnd


Section
  CreateDirectory  $INSTDIR\${prodname.simple}\${exemple}\Donnees\R-Altaïr
  CreateDirectory  $INSTDIR\${prodname.simple}\${exemple}\Donnees\xhl
  CreateDirectory  $INSTDIR\${prodname.simple}\lhx
  CreateDirectory  $INSTDIR\${prodname.simple}\scripts
  CreateDirectory  $LOCALAPPDATA\${prodname.simple}  
 
  
  SetOutPath       $LOCALAPPDATA\${prodname.simple}
  File /r  "${prodname.simple}\Interface\share\altair-gui\images"
  
  SetDetailsPrint both
  SetOutPath $INSTDIR\${prodname.simple}
  File /r  "${prodname.simple}\Docs" 
  File     "${prodname.simple}\*.txt" 
  File     "${prodname.simple}\*.html" 
  File     "${prodname.simple}\LICENCE" 
  File     "${prodname.simple}\VERSION" 
    
  SetOutPath $INSTDIR\${prodname.simple}
  File /r "${prodname.simple}\windows"
  File /r  "${prodname.simple}\${Interface}" 
  File /r  "${prodname.simple}\${RDir}"
  File /r  "${prodname.simple}\${nppDir}"
  File /r  "${prodname.simple}\${texDir}"
  File /r  "${prodname.simple}\${RStudioDir}"
  File /r "${prodname.simple}\data"; pyramides
  
  SetOutPath $INSTDIR\${prodname.simple}\scripts
  File /r "${prodname.simple}\${exemple}"

SectionEnd

 
 
 !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
   !insertmacro MUI_DESCRIPTION_TEXT ${sec1} $(DESC_sec1)
 !insertmacro MUI_FUNCTION_DESCRIPTION_END
  
 
Section 
 
 !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  SetShellVarContext current
  
  SetOutPath       "$INSTDIR\${prodname.simple}\${Interface}"
  CreateShortCut   "$DESKTOP\${prodname}.lnk" "$INSTDIR\${prodname.simple}\${Interface}\gui\x64\${prodname.simple}.exe"  "" "$INSTDIR\${prodname.simple}\${exemple}\icones\${icon}" "" SW_SHOWMINIMIZED
  WriteRegStr HKLM "${prodname}\Shell\open\command\" "" "$INSTDIR\${prodname.simple}\${Interface}\gui\x64\${prodname.simple}.exe"
  
  CreateDirectory  "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut   "$SMPROGRAMS\$StartMenuFolder\Désinstaller.lnk" "$INSTDIR\Désinstaller.exe" "" "$INSTDIR\Désinstaller.exe" 0
  CreateShortCut   "$SMPROGRAMS\$StartMenuFolder\${prodname}.lnk" "$INSTDIR\${prodname.simple}\${Interface}\gui\x64\${prodname.simple}.exe" "" "$INSTDIR\${prodname.simple}\${exemple}\icones\${icon}" 0
  
  SetDetailsPrint textonly
  DetailPrint "Création des clés d'enregistrement..."
  SetDetailsPrint listonly

  WriteRegStr HKLM "${prodname}\DefaultIcon" "${prodname}" "$INSTDIR\${prodname.simple}\${exemple}\icones\${icon}"
  WriteRegStr HKLM SOFTWARE\${prodname} \
     "Install_Dir" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" \
     "DisplayName" "${prodname} (désinstallation)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" \
     "UninstallString" "$\"$INSTDIR\Désinstaller.exe$\""
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "NoModify" "1"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "NoRepair" "1"
	  
  SetOutPath $INSTDIR

  WriteUninstaller "$INSTDIR\Désinstaller.exe"
 
!insertmacro MUI_STARTMENU_WRITE_END

SectionEnd
 
  
Function un.onUninstSuccess
  MessageBox MB_OK "Désinstallation réussie."
FunctionEnd

Section "Uninstall"
 
 !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
 
  DeleteRegKey HKLM "SOFTWARE\${prodname}"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}"	
  DeleteRegKey HKLM "${prodname}\DefaultIcon"
  
  Delete "$DESKTOP\${prodname}.lnk"
  Delete "$INSTDIR\Désinstaller.exe"
  Delete "$INSTDIR\*.*"
  Delete "$SMPROGRAMS\$StartMenuFolder\*.*"
  RMDir  "$SMPROGRAMS\$StartMenuFolder"

  Delete "$INSTDIR\${prodname.simple}\*.*"
  RMDir /r "$INSTDIR\${prodname.simple}"
  
  RMDir /r "$LOCALAPPDATA\RStudio-desktop"
  RMDir /r "$APPDATA\RStudio"

  
SectionEnd

BrandingText "Altaïr-${version}"

; eof
