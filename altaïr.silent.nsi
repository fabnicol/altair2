!include "Sections.nsh"
!include "MUI2.nsh"
!include "x64.nsh"
!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
!include "StrFunc.nsh"
!include "EnvVarUpdate.nsh"
;!addplugindir "c:\Program Files (x86)\NSIS\Plugins"
!include FileFunc.nsh
!insertmacro GetParameters
!insertmacro GetOptions

; Numéros de version 

!define version  "2016.01"
Var processeur 
Var type 
Var Interface

; l'installation par défaut est du type Interface_windows_core2
; les possibilités sont donc : 
; Interface_windows_core2    	:   /CPU=core2  /TYPE=avance
; Interface_windows				:   /CPU=corei3 /TYPE=avance
; Interface_windows_min_core2	:   /CPU=core2  /TYPE=min
; Interface_windows_min			:   /CPU=corei3 /TYPE=min

!define VER_MAJOR 2016
!define VER_MINOR 01
!define VER_REVISION 0
!define VER_BUILD  0
!define Rversion_major "3.2"

; autres définitions

!define prodname     "Altaïr"
!define prodname.simple "Altair"
!define setup        "Altaïr-${version}.win.${processeur}.installer.exe"
!define exemple      "${prodname.simple}\Tests\Exemple"
!define xhl          "${exemple}\Donnees\xhl"

!define icon         neptune.ico
!define RDir         "R"
!define texDir       "texlive"
!define RStudioDir   "RStudio"
!define GitDir       "Git"
!define startmenu    "$SMPROGRAMS\${prodname}-${version}"
!define Désinstaller "Désinstaller.exe"
!define notefile     "${prodname.simple}\LISEZ-MOI.txt"
!define installfile  "${prodname.simple}\INSTALLATION.txt"


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

 Var StartMenuFolder
 InstallDir "C:\Users\Public" 
 InstallDirRegKey HKLM "SOFTWARE\${prodname}" "Install_Dir"
   
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
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

  
 LicenseLangString myLicenseData ${LANG_FRENCH} "${prodname.simple}\LICENCE"
 LicenseData $(myLicenseData)
 LangString Name ${LANG_FRENCH}  "Altaïr"
 Name $(Name)


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

!define MUI_FINISHPAGE_TITLE $(title2)
!define MUI_FINISHPAGE_TEXT  $(text2)
!define MUI_FINISHPAGE_RUN 
!define MUI_FINISHPAGE_RUN_NOTCHECKED
!define MUI_FINISHPAGE_RUN_TEXT     "Lire le fichier INSTALLATION"
!define MUI_FINISHPAGE_RUN_FUNCTION "Launch_INSTALLATION"
!define MUI_FINISHPAGE_BUTTON       "Suite"
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
SilentInstall silent

RequestExecutionLevel user
AutoCloseWindow true
Icon "${prodname.simple}\Interface_windows\${icon}"  

Function .onInit
 
  SetOutPath $TEMP	
  File /oname=spltmp.bmp "${prodname.simple}\spltmp.bmp"

  advsplash::show 2300 600 400 -1 $TEMP\spltmp

  Pop $0 ; $0 has '1' if the user closed the splash screen early,
         ; '0' if everything closed normally, and '-1' if some error occurred.

  Delete $TEMP\spltmp.bmp

  StrCpy $processeur  "core2"  ; ou corei3
  StrCpy $type  "windows"  ; ou corei3; ou windows_min
  
  ${GetParameters} $R0
  ClearErrors
  ${GetOptions} $R0 /CPU= $2
  ${GetOptions} $R0 /TYPE= $3
  
  StrCpy $processeur $2
  StrCpy $type $3
  StrCpy $Interface  "Interface_${type}_${processeur}"  
  
  ${If} $2 == "core2"
    StrCpy $Interface "Interface_"$3"_core2"
  ${Else}	
    StrCpy $Interface "Interface_"$3
  ${Endif}   
  
FunctionEnd
 

 
Section
  
  CreateDirectory  $INSTDIR\${exemple}\Donnees\R-Altaïr
  CreateDirectory  $INSTDIR\${exemple}\Projets
  CreateDirectory  $INSTDIR\${xhl}
  CreateDirectory  $INSTDIR\${prodname.simple}\lib
  CreateDirectory  $INSTDIR\${prodname.simple}\win
  CreateDirectory  $INSTDIR\${prodname.simple}\lhx
  CreateDirectory $LOCALAPPDATA\${prodname}  
  
  SetDetailsPrint both
  SetOutPath $INSTDIR\${prodname.simple}
  File /r  "${prodname.simple}\Docs" 
  File /r  "${prodname.simple}\Outils" 
  File /r  "${prodname.simple}\lib" 
  File     "${prodname.simple}\*.*" 
  File /r  "${prodname.simple}\RStudio-project\.Rproj.user" 
  
  SetOutPath $INSTDIR\${prodname.simple}\win
  File   "${prodname.simple}\win\*.*" 
  
  SetOutPath $INSTDIR\${exemple}
  File /r  ${exemple}\Docs
  File     ${exemple}\*.*

  ; C:\Users\XXX\AppData\Altaïr  
  
  ReserveFile /r  "${prodname.simple}\Interface_windows" 
  ReserveFile /r  "${prodname.simple}\Interface_windows_core2" 

${If} $type == "avance"
    SetOutPath $INSTDIR\${prodname.simple}
	${If} $processeur == "core2"
      File /r  "${prodname.simple}\Interface_windows_core2" 
	${Else}
	  File /r  "${prodname.simple}\Interface_windows" 
	${EndIf}
    File /r  "${prodname.simple}\${RDir}"
    File /r  "${prodname.simple}\${texDir}"
	File /r  "${prodname.simple}\${GitDir}"
    File /r  "${prodname.simple}\${RStudioDir}"
	
	SetOutPath $LOCALAPPDATA  
    File /r "${prodname.simple}\Local\RStudio-desktop"
  
    SetOutPath $APPDATA\RStudio  
    File  "${prodname.simple}\Roaming\RStudio\*.*"
	
	${EnvVarUpdate} $0 "PATH" "A" "HKCU" "$INSTDIR\${prodname.simple}\${texDir}\miktex\bin\x64"
	${EnvVarUpdate} $0 "PATH" "A" "HKCU" "$INSTDIR\${prodname.simple}\${GitDir}\bin" 
		
${Else}
 
    SetOutPath $INSTDIR\${prodname.simple}
    File /r  "${prodname.simple}\${RDir}"
	${If} $processeur == "core2"
      File /r  "${prodname.simple}\Interface_windows_min_core2" 
	${Else}
	  File /r  "${prodname.simple}\Interface_windows_min" 
	${EndIf}

	CreateDirectory $INSTDIR\${prodname.simple}\${RStudioDir}\bin\pandoc
	SetOutPath $INSTDIR\${prodname.simple}\${RStudioDir}\bin\pandoc
	;File ${prodname.simple}\${RStudioDir}\bin\pandoc\pandoc.exe
	
${EndIf}

    SetOutPath $INSTDIR\${xhl}
    File /r  ${xhl}\Anonyme
    File /r  ${xhl}\Anonyme2

 SectionEnd
  
 
Section 
 
 !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  SetShellVarContext current
  
  SetOutPath       "$INSTDIR\${prodname.simple}\$Interface"
  CreateShortCut   "$DESKTOP\${prodname}.lnk" "$INSTDIR\${prodname.simple}\$Interface\gui\x64\${prodname}.exe"  "" "$INSTDIR\${prodname.simple}\$Interface\${icon}"
    
  CreateDirectory  "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut   "$SMPROGRAMS\$StartMenuFolder\Désinstaller.lnk" "$INSTDIR\Désinstaller.exe" "" "$INSTDIR\Désinstaller.exe" 0
  CreateShortCut   "$SMPROGRAMS\$StartMenuFolder\${prodname}.lnk" "$INSTDIR\${prodname.simple}\$Interface\gui\x64\${prodname}.exe" "" "$INSTDIR\${prodname.simple}\$Interface\${icon}" 0
  SetDetailsPrint listonly
	  
  SetOutPath $INSTDIR

  WriteUninstaller "$INSTDIR\Désinstaller.exe"
 
!insertmacro MUI_STARTMENU_WRITE_END

SectionEnd
 
Section "Uninstall"
 
 !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  
  ${un.EnvVarUpdate} $0 "PATH" "R" "HKCU" "$INSTDIR\${prodname.simple}\${texDir}\miktex\bin\x64" 
  ${un.EnvVarUpdate} $0 "PATH" "R" "HKCU" "$INSTDIR\${prodname.simple}\${GitDir}\bin"  

  Delete "$DESKTOP\${prodname}.lnk"
  Delete "$INSTDIR\Désinstaller.exe"
  Delete "$INSTDIR\LISEZ-MOI.txt"
  Delete "$INSTDIR\INSTALLATION.txt"
  Delete "$SMPROGRAMS\$StartMenuFolder\*.*"
  RMDir  "$SMPROGRAMS\$StartMenuFolder"

  Delete "$INSTDIR\${prodname.simple}\*.*"
  RMDir /r "$INSTDIR\${prodname.simple}"
  
  RMDir /r "$LOCALAPPDATA\RStudio-desktop"
  RMDir /r "$APPDATA\RStudio"
 
SectionEnd

BrandingText "Altaïr-${version}"

; eof
