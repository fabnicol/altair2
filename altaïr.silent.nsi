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

; Num�ros de version 

!define version  "2016.02"
Var processeur 
Var type 
Var interface

; l'installation par d�faut est du type Interface_windows_core2
; les possibilit�s sont donc : 
; Interface_windows_core2    	:   /CPU=core2  /TYPE=avance
; Interface_windows				:   /CPU=corei3 /TYPE=avance
; Interface_windows_min_core2	:   /CPU=core2  /TYPE=min
; Interface_windows_min			:   /CPU=corei3 /TYPE=min

!define VER_MAJOR 2016
!define VER_MINOR 01
!define VER_REVISION 0
!define VER_BUILD  0
!define Rversion_major "3.2"

; autres d�finitions

!define prodname     "Alta�r"
!define prodname.simple "Altair"
!define setup      "Alta�r-${version}.win.installer.exe"
!define exemple      "${prodname.simple}\Tests\Exemple"
!define xhl          "${exemple}\Donnees\xhl"

!define icon         neptune.ico
!define RDir         "R"
!define texDir       "texlive"
!define RStudioDir   "RStudio"
!define GitDir       "Git"
!define startmenu    "$SMPROGRAMS\${prodname}-${version}"
!define D�sinstaller "D�sinstaller.exe"
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

 LangString wizard1 ${LANG_FRENCH}   "Installation du logiciel Analyse des lignes de traitement, attributions indemnitaires et r�mun�rations diverses. Appuyer sur suivant pour continuer."
 LangString wizard2 ${LANG_FRENCH}   "Installation du logiciel Alta�r version ${version}"
 LangString title1 ${LANG_FRENCH}    "Lisez-moi"
 LangString text1 ${LANG_FRENCH}     "Alta�r ${version} va �tre install� dans $INSTDIR. Cliquer sur Compl�ments pour des pr�cisions sur l'installation."
 LangString title2 ${LANG_FRENCH}    "Installation"
 LangString text2 ${LANG_FRENCH}     "Compl�ments d'information sur l'installation."
 LangString D�sinstaller ${LANG_FRENCH}     "d�sinstallation du logiciel ${prodname} "
 LangString completed ${LANG_FRENCH} "Termin�."
 LangString uninst_completed ${LANG_FRENCH} "D�sinstallation termin�e"
 LangString Sec1Name ${LANG_FRENCH}  "Alta�r"

  
 LicenseLangString myLicenseData ${LANG_FRENCH} "${prodname.simple}\LICENCE"
 LicenseData $(myLicenseData)
 LangString Name ${LANG_FRENCH}  "Alta�r"
 Name $(Name)


!insertmacro MUI_PAGE_LICENSE "${prodname.simple}\LICENCE"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS

!define MUI_FINISHPAGE_TITLE $(title1)
!define MUI_FINISHPAGE_TEXT  $(text1)
!define MUI_FINISHPAGE_RUN 
!define MUI_FINISHPAGE_RUN_NOTCHECKED
!define MUI_FINISHPAGE_CANCEL_ENABLED 
!insertmacro MUI_PAGE_FINISH

!define MUI_FINISHPAGE_TITLE $(title2)
!define MUI_FINISHPAGE_TEXT  $(text2)
!define MUI_FINISHPAGE_RUN 
!define MUI_FINISHPAGE_RUN_NOTCHECKED
!define MUI_FINISHPAGE_CANCEL_ENABLED 
!insertmacro MUI_PAGE_FINISH


!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_UNPAGE_INSTFILES

UninstallText  $(uninstall)
CompletedText  $(completed)

;ShowInstDetails show
;ShowUninstDetails show

Caption "Installation de Alta�r ${version}"

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

  Delete $TEMP\spltmp.bmp

  StrCpy $processeur  "core2"  ; ou corei3
  StrCpy $type  "windows"  ; ou corei3; ou windows_min
  
  ${GetParameters} $R0
  ClearErrors
  ${GetOptions} $R0 /CPU= $2
  ${GetOptions} $R0 /TYPE= $3
  
  StrCpy $processeur $2
  StrCpy $type $3
  ${If} $3 == "min"
    StrCpy $4 "Interface_windows_min"
  ${Else}
    StrCpy $4 "Interface_windows"
  ${EndIf}
  
  ${If} $2 == "core2"
    StrCpy $4 "$4_core2" 
  ${EndIf}	
  
FunctionEnd
 

 
Section
  
  CreateDirectory  $INSTDIR\${exemple}\Donnees\R-Alta�r
  CreateDirectory  $INSTDIR\${exemple}\Projets
  CreateDirectory  $INSTDIR\${xhl}
  CreateDirectory  $INSTDIR\${prodname.simple}\lhx
  CreateDirectory  $INSTDIR\${prodname.simple}\win
  CreateDirectory $LOCALAPPDATA\${prodname}  
  FileOpen $8 $LOCALAPPDATA\${prodname}\rank w
  FileClose $8
  
  SetOutPath $INSTDIR\${prodname.simple}\win
	
  ${If} $processeur == "core2"
	  File /r  "${prodname.simple}\win_core2\*.*" 
  ${Else}
	  File /r  "${prodname.simple}\win\*.*" 
  ${EndIf}

  SetOutPath $INSTDIR\${prodname.simple}  
    
  ${If} $type == "min"
    
	File /r  "${prodname.simple}\Docs" 
	File /r  "${prodname.simple}\Outils" 
	File /r  "${prodname.simple}\lib" 
	File     "${prodname.simple}\*.*" 
	File /r  "${prodname.simple}\RStudio-project\.Rproj.user" 
	File /r  "${prodname.simple}\${RDir}"

	${If} $processeur == "core2"
	  File /r  "${prodname.simple}\Interface_windows_min_core2" 
    ${Else}
	  File /r  "${prodname.simple}\Interface_windows_min" 
    ${EndIf}
	
	CreateDirectory $INSTDIR\${prodname.simple}\${RStudioDir}\bin\pandoc
	SetOutPath $INSTDIR\${prodname.simple}\${RStudioDir}\bin\pandoc
	File ${prodname.simple}\${RStudioDir}\bin\pandoc\pandoc.exe
			
  ${Else}  ; "avanc�"

    File /r  "${prodname.simple}\${texDir}"
	File /r  "${prodname.simple}\${GitDir}"
    File /r  "${prodname.simple}\${RStudioDir}"
	
	${If} $processeur == "core2"
	  File /r  "${prodname.simple}\Interface_windows_core2" 
    ${Else}
	  File /r  "${prodname.simple}\Interface_windows" 
    ${EndIf}
	
	SetOutPath $LOCALAPPDATA  
    File /r "${prodname.simple}\Local\RStudio-desktop"
  
    SetOutPath $APPDATA\RStudio  
    File  "${prodname.simple}\Roaming\RStudio\*.*"
	
	${EnvVarUpdate} $0 "PATH" "A" "HKCU" "$INSTDIR\${prodname.simple}\${texDir}\miktex\bin\x64"
	${EnvVarUpdate} $0 "PATH" "A" "HKCU" "$INSTDIR\${prodname.simple}\${GitDir}\bin" 
	
  ${EndIf}
  
  SetOutPath $INSTDIR\${exemple}
  File /r  ${exemple}\Docs
  File     ${exemple}\*.*


  SetOutPath $INSTDIR\${xhl}
  File /r  ${xhl}\Anonyme
  File /r  ${xhl}\Anonyme2

 SectionEnd
  
 
Section 
  
 !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  SetShellVarContext current
  StrCpy $5 "$INSTDIR\${prodname.simple}\$4\gui\x64\${prodname}.exe"  
  StrCpy $6 "$INSTDIR\${prodname.simple}\$4\${icon}"
  
  CreateShortCut   "$DESKTOP\${prodname}.lnk" $5  "" $6
    
  CreateDirectory  "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut   "$SMPROGRAMS\$StartMenuFolder\D�sinstaller.lnk" "$INSTDIR\D�sinstaller.exe" "" "$INSTDIR\D�sinstaller.exe" 0
  
  StrCpy $5 "$INSTDIR\${prodname.simple}\$4\gui\x64\${prodname}.exe"
  StrCpy $6 "$INSTDIR\${prodname.simple}\$4\${icon}"
  
  CreateShortCut   "$SMPROGRAMS\$StartMenuFolder\${prodname}.lnk" $5 "" $6 0
  SetDetailsPrint listonly
	  
  SetOutPath $INSTDIR

  WriteUninstaller "$INSTDIR\D�sinstaller.exe"
 
!insertmacro MUI_STARTMENU_WRITE_END

SectionEnd
 
Section "Uninstall"
 
 !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  
  ${un.EnvVarUpdate} $0 "PATH" "R" "HKCU" "$INSTDIR\${prodname.simple}\${texDir}\miktex\bin\x64" 
  ${un.EnvVarUpdate} $0 "PATH" "R" "HKCU" "$INSTDIR\${prodname.simple}\${GitDir}\bin"  

  Delete "$DESKTOP\${prodname}.lnk"
  Delete "$INSTDIR\D�sinstaller.exe"
  Delete "$INSTDIR\LISEZ-MOI.txt"
  Delete "$INSTDIR\INSTALLATION.txt"
  Delete "$SMPROGRAMS\$StartMenuFolder\*.*"
  RMDir  "$SMPROGRAMS\$StartMenuFolder"

  Delete "$INSTDIR\${prodname.simple}\*.*"
  RMDir /r "$INSTDIR\${prodname.simple}"
  
  RMDir /r "$LOCALAPPDATA\RStudio-desktop"
  RMDir /r "$APPDATA\RStudio"
 
SectionEnd

Function un.onUninstSuccess
    MessageBox  MB_ICONINFORMATION "D�sinstallation r�alis�e."
  FunctionEnd

BrandingText "Alta�r-${version}"

; eof
