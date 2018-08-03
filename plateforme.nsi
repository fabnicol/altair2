!include "Sections.nsh"
!include "MUI2.nsh"
!include "x64.nsh"
!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
!include "StrFunc.nsh"
!include "EnvVarUpdate.nsh"
  
; Numéros de version 

!define version  "2015.10"
!define VER_MAJOR 2015
!define VER_MINOR 02
!define VER_REVISION 0
!define VER_BUILD  0

; autres définitions

!define prodname     "Altaïr-SDK"
!define setup        "${prodname}-${version}.installer.exe"
!define icon         neptune.ico
!define startmenu    "$SMPROGRAMS\${prodname}-${version}"
!define Désinstaller "Désinstaller.exe"
!define notefile     "${prodname}\LISEZ-MOI.txt"
!define qtcreator    qtcreator-3.5.1
!define texDir       "texlive"
!define GitDir       "Git"
!define RToolsDir    "Rtools"

!define REG_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}"
!define MEMENTO_REGISTRY_ROOT HKLM
!define MEMENTO_REGISTRY_KEY "${REG_UNINST_KEY}"

!define MUI_ICON     "${prodname}\${icon}"
!define MUI_WELCOMEFINISHPAGE
!define MUI_WELCOMEPAGE_TEXT  $(wizard1)
!define MUI_WELCOMEPAGE_TITLE $(wizard2)
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${prodname}\${prodname}.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${prodname}\neptune.bmp"
!define MUI_ABORTWARNING
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKCU" 
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Modern UI Test" 
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "Start Menu Folder"

 Var StartMenuFolder
  Var minimal
  
 InstallDir "C:\Users\Public\Dev" 
 InstallDirRegKey HKLM "SOFTWARE\${prodname}" "Install_Dir"
   
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
!insertmacro MUI_PAGE_WELCOME  
!insertmacro MUI_LANGUAGE "French" 

 LangString wizard1 ${LANG_FRENCH}   "Installation de la plateforme de développement (SDK) Altaïr. Appuyer sur suivant pour continuer."
 LangString wizard2 ${LANG_FRENCH}   "Version ${version}"
 LangString title1 ${LANG_FRENCH}    "Lisez-moi"
 LangString text1 ${LANG_FRENCH}     "${prodname} va être installé dans $INSTDIR. Cliquer sur Compléments pour des précisions sur l'installation."
 LangString title2 ${LANG_FRENCH}    "Installation"
 LangString Désinstaller ${LANG_FRENCH}     "Désinstallation de la plateforme ${prodname} "
 LangString completed ${LANG_FRENCH} "Terminé."
 LangString uninst_completed ${LANG_FRENCH} "Désinstallation terminée"
 LangString Sec1Name ${LANG_FRENCH}  "${prodname}"
 LangString DESC_sec1 ${LANG_FRENCH} "Installer Altaïr"
 LangString Message  ${LANG_FRENCH}  "Appuyer sur Oui pour installer ${prodname}"
 LicenseLangString myLicenseData ${LANG_FRENCH} "${prodname}\LICENCE"
 LicenseData $(myLicenseData)
 LangString Name ${LANG_FRENCH}  "Plateforme de développement Altaïr"
 LangString DESC_Advanced ${LANG_FRENCH} "Installer la version avancée (Tous les outils de développement)"
 LangString DESC_Minimale ${LANG_FRENCH} "Installer la version minimale (Outils de compilation d'Altaïr seulement)"
 LangString AdvancedName ${LANG_FRENCH}  "Installation avancée"
 LangString MinimaleName ${LANG_FRENCH}  "Installation minimale"
 
 Name $(Name)

; MUI macros   

!insertmacro MUI_PAGE_LICENSE "${prodname}\LICENCE"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS

!define MUI_FINISHPAGE_TITLE $(title1)
!define MUI_FINISHPAGE_TEXT  $(text1)
!define MUI_FINISHPAGE_RUN 
!define MUI_FINISHPAGE_RUN_NOTCHECKED
!define MUI_FINISHPAGE_RUN_TEXT     "Lire le fichier LISEZ-MOI"
!define MUI_FINISHPAGE_RUN_FUNCTION "Launch_LISEZ"
!define MUI_FINISHPAGE_BUTTON       "Terminer"
!define MUI_FINISHPAGE_CANCEL_ENABLED 
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_INSTFILES

UninstallText  $(uninstall)
CompletedText  $(completed)

;ShowInstDetails show
;ShowUninstDetails show

Caption ""

OutFile "${setup}"

SetDateSave on
SetDatablockOptimize on
CRCCheck on
SilentInstall normal

Icon "${prodname}\${icon}"

RequestExecutionLevel user
AutoCloseWindow false

 Section
 StrCpy $minimal "_min"
 SectionEnd


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
 
 
Function Launch_LISEZ
  SetOutPath $INSTDIR
  File  "${notefile}"  
  Exec '"notepad" "$INSTDIR\LISEZ-MOI.txt"'
FunctionEnd

 
Section  $(Sec1Name) sec1
  SetDetailsPrint both
  CreateDirectory $LOCALAPPDATA\Altaïr
  
  SetOutPath $INSTDIR
  File     "${prodname}\*.*" 
  SetOutPath $INSTDIR
  File /r ${prodname}\altair
      
  SetOutPath $APPDATA
  File /r ${prodname}\altair\Roaming.dev\QtProject
  File /r ${prodname}\altair\Roaming.dev\RStudio
    
  SetOutPath $LOCALAPPDATA
  File /r ${prodname}\altair\Local.dev\RStudio-desktop
  
  ${EnvVarUpdate} $0 "PATH" "A" "HKCU" "$INSTDIR\${prodname}\altair\${texDir}\bin\win32"
  ${EnvVarUpdate} $0 "PATH" "A" "HKCU" "$INSTDIR\${prodname}\altair\${GitDir}\bin" 
  
SectionEnd

 Section /o $(AdvancedName) Advanced

    SetOutPath $INSTDIR
	ExecWait '"$INSTDIR\NSIS.exe"'
	ExecWait '"$INSTDIR\Notepad++.exe"'
	ExecWait '"$INSTDIR\RBuildTools.exe"'
	ExecWait '"$INSTDIR\mingw64-5.2.exe"'
	ExecWait '"$INSTDIR\msys64.exe"'
	ExecWait '"$INSTDIR\Qt5.5-builds.exe"'
	ExecWait '"$INSTDIR\qtcreator-3.5.1.exe"'
    ExecWait '"$INSTDIR\redist.exe"'
	Delete   "$INSTDIR\*.exe"

    ${EnvVarUpdate} $0 "PATH" "A" "HKCU" "$INSTDIR\${prodname}\${RToolsDir}\bin" 	
	
	StrCpy $minimal ""

 SectionEnd

 Section   $(MinimaleName) Minimale 
    SetOutPath $INSTDIR

	ExecWait '"$INSTDIR\mingw64-5.2.exe"'
	ExecWait '"$INSTDIR\qt-5.5.0-x64-mingw52-static-runtime.exe"'
    ExecWait '"$INSTDIR\qt-5.5.0-x64-mingw510r0-seh-rev0.exe"'
	ExecWait '"$INSTDIR\qtcreator-3.5.1.exe"'
	Delete   "$INSTDIR\*.exe"
	
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
 
Section 

 
  !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  SetShellVarContext current
  
  SetOutPath       "$INSTDIR\${qtcreator}"
  CreateShortCut   "$DESKTOP\${prodname}.lnk" "$INSTDIR\${qtcreator}\bin\qtcreator.exe"  "" "$INSTDIR\${icon}"
  WriteRegStr HKLM "${prodname}\Shell\open\command\" "" "$INSTDIR\${qtcreator}\bin\qtcreator.exe"
  
  CreateDirectory  "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut   "$SMPROGRAMS\$StartMenuFolder\Désinstaller.lnk" "$INSTDIR\Désinstaller.exe" "" "$INSTDIR\Désinstaller.exe" 0
    
  SetDetailsPrint textonly
  DetailPrint "Création des clés d'enregistrement..."
  SetDetailsPrint listonly

  WriteRegStr HKLM "${prodname}\DefaultIcon" "${prodname}" "$INSTDIR\${icon}"
  WriteRegStr HKLM SOFTWARE\${prodname} \
     "Install_Dir" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" \
     "DisplayName" "${prodname} (désinstallation)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" \
     "UninstallString" "$\"$INSTDIR\Désinstaller.exe$\""
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "NoModify" "1"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "NoRepair" "1"
	  
  SetOutPath $INSTDIR

  ;WriteRegStr HKLM "${REG_UNINST_KEY}" "URLInfoAbout" "http://github.com/fabnicol/altair"
  ;WriteRegStr HKLM "${REG_UNINST_KEY}" "HelpLink" "http://github.com/fabnicol/altair"

  WriteUninstaller "$INSTDIR\Désinstaller.exe"
 
!insertmacro MUI_STARTMENU_WRITE_END

SectionEnd
 
 
Function .onInstSuccess
  
   SetOutPath $INSTDIR\altair
   ExecWait '"$INSTDIR\altair\git_reset_hard.bat"'
   
FunctionEnd

Section "Uninstall"
 
 !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
 
  DeleteRegKey HKLM "SOFTWARE\${prodname}"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}"	
  DeleteRegKey HKLM "${prodname}\DefaultIcon"
  
  ${un.EnvVarUpdate} $0 "PATH" "R" "HKCU" "$INSTDIR\${prodname}\altair\${texDir}\bin\win32" 
  ${un.EnvVarUpdate} $0 "PATH" "R" "HKCU" "$INSTDIR\${prodname}\altair\${GitDir}\bin"  
  
  ${If} $minimal == ""
  ${un.EnvVarUpdate} $0 "PATH" "R" "HKCU" "$INSTDIR\${prodname}\altair\${RToolsDir}\bin"  
  ${EndIf}
  
  Delete "$DESKTOP\${prodname}.lnk"
  Delete "$INSTDIR\Désinstaller.exe"
  Delete "$INSTDIR\LISEZ-MOI.txt"
  Delete "$SMPROGRAMS\$StartMenuFolder\*.*"
  RMDir  "$SMPROGRAMS\$StartMenuFolder"
  RMDir /r $APPDATA\QtProject
  RMDir /r $APPDATA\RStudio
  RMDir /r $APPDATA\Notepad++
  RMDir /r $LOCALAPPDATA\RStudio-desktop
  RMDir /r $LOCALAPPDATA\Altaïr
  
  Delete "$INSTDIR\*.*"
  RMDir /r "$INSTDIR\"
   
SectionEnd

BrandingText "${prodname}-${version}"

; eof
