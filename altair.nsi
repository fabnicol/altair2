!include "Sections.nsh"
!include "MUI2.nsh"
!include "x64.nsh"
!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
!include "StrFunc.nsh"
!include "EnvVarUpdate.nsh"
  
; Numéros de version 

!define version  "2015.02"
!define VER_MAJOR 2015
!define VER_MINOR 02
!define VER_REVISION 0
!define VER_BUILD  0
!define Rversion_minor "2"
!define Rversion_major "3.1"
!define Rversion     ${Rversion_major}.${Rversion_minor}
!define RStudio_version "0.99.179"
!define Miktex_version  "2.9"

; autres définitions

!define prodname     "Altair"
!define nbits        celeron
!define setup        "Altaïr-${version}.win.${nbits}.installer.exe"
!define exemple      "${prodname}\Tests\Exemple"
!define xhl          "${exemple}\Donnees\xhl"
!define icon         neptune.ico
!define RStudio      "RStudio-${RStudio_version}.exe" 
!define R            "R-${Rversion}-win.exe"
!define RDir         "R-${Rversion}"
!define MiktexDir    "Miktex ${Miktex_version}"
!define RStudioDir   "RStudio"
!define Miktex       "setup-2.9.4503-x64.exe"
!define startmenu    "$SMPROGRAMS\${prodname}-${version}"
!define Désinstaller "Désinstaller.exe"
!define notefile     "${prodname}\LISEZ-MOI.txt"
!define installfile  "${prodname}\INSTALLATION.txt"
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

 RequestExecutionLevel admin
 InstallDirRegKey HKLM "SOFTWARE\${prodname}" ""
 
 Var StartMenuFolder

!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Modern UI Test" 
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"
  
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
!insertmacro MUI_PAGE_WELCOME  
!insertmacro MUI_LANGUAGE "French" 

 LangString wizard1 ${LANG_FRENCH}   "Installation du logiciel Analyse des lignes de traitement, attributions indemnitaires et rémunérations diverses. Appuyer sur suivant pour continuer."
 LangString wizard2 ${LANG_FRENCH}   "Installation du logiciel Altaïr version ${version}"
 LangString title1 ${LANG_FRENCH}    "Lisez-moi"
 LangString text1 ${LANG_FRENCH}     "Altaïr ${version} va être installé dans $INSTDIR. Cliquer sur Compléments pour des précisions sur l'installation."
 LangString title2 ${LANG_FRENCH}    "Installation"
 LangString text2 ${LANG_FRENCH}     "Compléments d'information sur l'installation. Cliquer sur Terminer pour lancer l'installation du logiciel et de ses dépendances."
 LangString Désinstaller ${LANG_FRENCH}     "désinstallation du logiciel ${prodname} "
 LangString completed ${LANG_FRENCH} "Terminé."
 LangString uninst_completed ${LANG_FRENCH} "Désinstallation terminée"
 LangString Sec1Name ${LANG_FRENCH}  "Altaïr"
 LangString DESC_sec1 ${LANG_FRENCH} "Installer Altaïr"
 LangString AdvancedName ${LANG_FRENCH}  "Installation avancée"
 LangString MinimaleName ${LANG_FRENCH}  "Installation minimale"
 LangString Sec6Name ${LANG_FRENCH}  "Exemples"
 ;LangString Sec5Name ${LANG_FRENCH} "Git"
 LangString DESC_Advanced ${LANG_FRENCH} "Installer la version avancée (Altaïr, R, RStudio et MiKTeX)"
 LangString DESC_Minimale ${LANG_FRENCH} "Installer la version minimale (Altaïr, R)"
 
 ;LangString DESC_sec5 ${LANG_FRENCH} "Installer le gestionnaire de versions GIT"
 LangString DESC_sec6 ${LANG_FRENCH} "Installer les exemples"
 
 
 LangString Message  ${LANG_FRENCH}  "Appuyer sur Oui pour installer Altaïr"
  
 LicenseLangString myLicenseData ${LANG_FRENCH} "${prodname}\LICENCE"
 LicenseData $(myLicenseData)
 LangString Name ${LANG_FRENCH}  "Altaïr"
 Name $(Name)

; MUI macros   

!insertmacro MUI_PAGE_LICENSE "${prodname}\LICENCE"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS

!define MUI_FINISHPAGE_TITLE $(title1)
!define MUI_FINISHPAGE_TEXT  $(text1)
!define MUI_FINISHPAGE_RUN 
!define MUI_FINISHPAGE_RUN_TEXT     "Lire le fichier LISEZ-MOI"
!define MUI_FINISHPAGE_RUN_FUNCTION "Launch_LISEZ"
!define MUI_FINISHPAGE_BUTTON       "Compléments"
!define MUI_FINISHPAGE_CANCEL_ENABLED 
!insertmacro MUI_PAGE_FINISH

!define MUI_FINISHPAGE_TITLE $(title2)
!define MUI_FINISHPAGE_TEXT  $(text2)
!define MUI_FINISHPAGE_RUN 
!define MUI_FINISHPAGE_RUN_TEXT     "Lire le fichier INSTALLATION"
!define MUI_FINISHPAGE_RUN_FUNCTION "Launch_INSTALLATION"
!define MUI_FINISHPAGE_BUTTON       "Terminer"
!define MUI_FINISHPAGE_CANCEL_ENABLED 
!insertmacro MUI_PAGE_FINISH


Section
MessageBox MB_YESNO|MB_ICONINFORMATION $(Message)  IDNO Fin IDYES OK
Fin:  Delete "$INSTDIR\${prodname}\*.*"
      RMDir /r "$INSTDIR\${prodname}"
      Abort
OK:
SectionEnd


Section
  CreateDirectory  $INSTDIR\${exemple}\Donnees\R-Altair
  CreateDirectory  $INSTDIR\${xhl}
  
  SetOutPath $INSTDIR\${prodname}
  File /r  "${prodname}\Interface_win64" 
  File /r  "${prodname}\Docs" 
  File /r  "${prodname}\Outils" 
  File /r  "${prodname}\win.${nbits}" 
  File /r  "${prodname}\.Rproj.user" 
  File     "${prodname}\*.*" 
  
  SetOutPath $INSTDIR\${exemple}
  File /r  ${exemple}\Docs
  File /r  ${exemple}\Projets
  File     ${exemple}\Altair.bmp     ${exemple}\Altair.ico      ${exemple}\*.R         ${exemple}\*.Rmd 
  File     ${exemple}\neptune.512.ico ${exemple}\neptune.bmp ${exemple}\neptune.ico      ${exemple}\style.css
  
  SetOutPath $DOCUMENTS\R\win-library\${Rversion_major}
  File /r  "${prodname}\lib\*.*" 
  
  
SectionEnd

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_INSTFILES

UninstallText  $(uninstall)
CompletedText  $(completed)

ShowInstDetails show
ShowUninstDetails show
RequestExecutionLevel user

Caption ""

OutFile "${setup}"

SetDateSave on
SetDatablockOptimize on
CRCCheck on
SilentInstall normal

InstallDir "C:\Users\Public"
InstallDirRegKey HKLM "${regkey}" ""

Icon "${prodname}\Interface_win64\${icon}"

RequestExecutionLevel user
AutoCloseWindow false
ShowInstDetails show


Function .onInit
 
  SetOutPath $TEMP	
  File /oname=spltmp.bmp "${prodname}\spltmp.bmp"

  advsplash::show 2300 600 400 -1 $TEMP\spltmp

  Pop $0 ; $0 has '1' if the user closed the splash screen early,
         ; '0' if everything closed normally, and '-1' if some error occurred.
	 

  Delete $TEMP\spltmp.bmp
  
  StrCpy $1 "Minimale"
  Call .onSelChange
  
FunctionEnd

; Functions
 
 
Function Launch_LISEZ
  SetOutPath $INSTDIR
  File  "${notefile}"  
  Exec '"notepad" "$INSTDIR\LISEZ-MOI.txt"'
FunctionEnd

Function Launch_INSTALLATION
  SetOutPath $INSTDIR
  File "${installfile}"  
  Exec '"notepad" "$INSTDIR\INSTALLATION.txt"'
FunctionEnd

Section -post 

 !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  SetShellVarContext current
  SetOutPath       "$INSTDIR\${prodname}\Interface_win64"
  CreateShortCut   "$DESKTOP\${prodname}.lnk" "$INSTDIR\${prodname}\Interface_win64\${prodname}.exe"  "" "$INSTDIR\${prodname}\Interface_win64\${icon}"
  WriteRegStr HKLM "${prodname}\Shell\open\command\" "" "$INSTDIR\${prodname}\Interface_win64\${prodname}.exe"
  
  CreateDirectory  "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut   "$SMPROGRAMS\$StartMenuFolder\Désinstaller.lnk" "$INSTDIR\Désinstaller.exe" "" "$INSTDIR\Désinstaller.exe" 0
  CreateShortCut   "$SMPROGRAMS\$StartMenuFolder\${prodname}.lnk" "$INSTDIR\${prodname}\Interface_win64\${prodname}.exe" "" "$INSTDIR\${prodname}\Interface_win64\${icon}" 0
  
 ; WriteRegStr HKLM "${prodname}\DefaultIcon" "${prodname}" "$INSTDIR\${prodname}\Interface_w${nbits}\${icon}"
 
 ; WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "DisplayName" "${prodname} (désinstallation)"
 ; WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "UninstallString" "$\"$INSTDIR\Désinstaller.exe$\""
    
  SetDetailsPrint textonly
  DetailPrint "Création des clés d'enregistrement..."
  SetDetailsPrint listonly

  SetOutPath $INSTDIR

  WriteRegDword HKLM "SOFTWARE\${prodname}" "VersionMajor" "${VER_MAJOR}"
  WriteRegDword HKLM "SOFTWARE\${prodname}" "VersionMinor" "${VER_MINOR}"
  WriteRegDword HKLM "SOFTWARE\${prodname}" "VersionRevision" "${VER_REVISION}"
  WriteRegDword HKLM "SOFTWARE\${prodname}" "VersionBuild" "${VER_BUILD}"
  
  WriteRegStr HKLM "SOFTWARE\${prodname}" "Install_Dir" "$INSTDIR"

  WriteRegStr HKLM   "${REG_UNINST_KEY}" "DisplayName" "${prodname}"
  WriteRegStr HKLM   "${REG_UNINST_KEY}" "DisplayIcon" "$INSTDIR\${prodname}\Interface_win64\${icon}"
  WriteRegStr HKLM   "${REG_UNINST_KEY}" "DisplayVersion" "${version}"
  WriteRegDWORD HKLM "${REG_UNINST_KEY}" "NoModify" "1"
  WriteRegDWORD HKLM "${REG_UNINST_KEY}" "NoRepair" "1"
  WriteRegExpandStr HKLM "${REG_UNINST_KEY}" "UninstallString" "$\"$INSTDIR\Désinstaller.exe$\""
  WriteRegExpandStr HKLM "${REG_UNINST_KEY}" "InstallLocation" "$INSTDIR"

  WriteRegDWORD HKLM "${REG_UNINST_KEY}" "VersionMajor" "${VER_MAJOR}"
  WriteRegDWORD HKLM "${REG_UNINST_KEY}" "VersionMinor" "${VER_MINOR}"
  ;WriteRegStr HKLM "${REG_UNINST_KEY}" "URLInfoAbout" "http://github.com/fabnicol/altair"
  ;WriteRegStr HKLM "${REG_UNINST_KEY}" "HelpLink" "http://github.com/fabnicol/altair"

  WriteUninstaller "$INSTDIR\Désinstaller.exe"
  
  SetDetailsPrint both
 
!insertmacro MUI_STARTMENU_WRITE_END

SectionEnd


 Section /o $(AdvancedName) Advanced
    SetOutPath $INSTDIR\${prodname}
    File /r  "${prodname}\${RDir}"
    File /r  "${prodname}\${MiktexDir}"
	${EnvVarUpdate} $0 "PATH" "A" "HKCU" "$INSTDIR\${prodname}\${MiktexDir}\miktex\bin\x64" ; appends to the user path
    File /r  "${prodname}\${RStudioDir}"
	SetOutPath $LOCALAPPDATA  
    File /r "${prodname}\Local\RStudio-desktop"
    File /r "${prodname}\Local\MiKTeX"
  
    SetOutPath $APPDATA\RStudio  
    File  "${prodname}\Roaming\RStudio\*.*"
  
    SetOutPath $APPDATA
    File /r "${prodname}\Roaming\MiKTeX"
  
	; This is important to have $APPDATA variable
	; point to ProgramData folder
	; instead of current user's Roaming folder
	
    SetShellVarContext all
	SetOutPath $APPDATA
	File /r "${prodname}\ProgramData\MiKTeX"
	SetShellVarContext current
 SectionEnd

 Section   $(MinimaleName) Minimale 
    SetOutPath $INSTDIR\${prodname}
    File /r  "${prodname}\${RDir}"
 SectionEnd

  Section  $(Sec6Name) sec6
    SetOutPath $INSTDIR\${xhl}
   ; File /r  ${xhl}\Anonyme
   ; File /r  ${xhl}\Anonyme2
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
  ; !insertmacro MUI_DESCRIPTION_TEXT ${sec4} $(DESC_sec4)
  ; !insertmacro MUI_DESCRIPTION_TEXT ${sec5} $(DESC_sec5)
   !insertmacro MUI_DESCRIPTION_TEXT ${sec6} $(DESC_sec6)
  ; !insertmacro MUI_DESCRIPTION_TEXT ${sec7} $(DESC_sec7)
 !insertmacro MUI_FUNCTION_DESCRIPTION_END

Function .onInstSuccess
  MessageBox MB_OK "Installation réussie de Altaïr-${version}."
FunctionEnd
 
 
Function un.onUninstSuccess
  MessageBox MB_OK "Désinstallation réussie."
FunctionEnd

Section "Uninstall"
 
 !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
 
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\${prodname}"
  DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}"
  
  ${un.EnvVarUpdate} $0 "PATH" "R" "HKCU" "$INSTDIR\${prodname}\${MiktexDir}\miktex\bin\x64" ; appends to the user path

  Delete "$DESKTOP\${prodname}.lnk"
  Delete "$INSTDIR\Désinstaller.exe"
  Delete "$INSTDIR\LISEZ-MOI.txt"
  Delete "$INSTDIR\INSTALLATION.txt"
  Delete "$SMPROGRAMS\$StartMenuFolder\*.*"
  RMDir  "$SMPROGRAMS\$StartMenuFolder"

  Delete "$INSTDIR\${prodname}\*.*"
  RMDir /r "$INSTDIR\${prodname}"
  
  SetShellVarContext all
  RMDir /r "$APPDATA\MiKTeX"
  
  SetShellVarContext current
  
  RMDir /r "$LOCALAPPDATA\RStudio-desktop"
  RMDir /r "$LOCALAPPDATA\MiKTeX"
  RMDir /r "$APPDATA\RStudio"
  RMDir /r "$APPDATA\MiKTeX"
  
SectionEnd

BrandingText "Altaïr-${version}"

; eof
