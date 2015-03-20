!include "Sections.nsh"
!include "MUI2.nsh"
!include "x64.nsh"
!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
!include "StrFunc.nsh"
!include "EnvVarUpdate.nsh"
;!addplugindir "c:\Program Files (x86)\NSIS\Plugins"
  
; Num�ros de version 

; pour une version minimal d�finir minimal par "" sinon "_min"

!define version  "2015.02"
!define VER_MAJOR 2015
!define VER_MINOR 02
!define VER_REVISION 0
!define VER_BUILD  0
!define Rversion_major "3.1"

; autres d�finitions

!define prodname     "Alta�r"
!define prodname.simple "Altair"
!define processeur        core
!define setup        "Alta�r-${version}.win.${processeur}.installer.exe"
!define exemple      "${prodname.simple}\Tests\Exemple"
!define xhl          "${exemple}\Donnees\xhl"
!define Interface.minimal    Interface_win64_min
!define Interface    Interface_win64
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
   
!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
!insertmacro MUI_PAGE_WELCOME  
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

!insertmacro MUI_PAGE_LICENSE "${prodname.simple}\LICENCE"
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_COMPONENTS

!define MUI_FINISHPAGE_TITLE $(title1)
!define MUI_FINISHPAGE_TEXT  $(text1)
!define MUI_FINISHPAGE_RUN 
!define MUI_FINISHPAGE_RUN_NOTCHECKED
!define MUI_FINISHPAGE_RUN_TEXT     "Lire le fichier LISEZ-MOI"
!define MUI_FINISHPAGE_RUN_FUNCTION "Launch_LISEZ"
!define MUI_FINISHPAGE_BUTTON       "Compl�ments"
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


!define MUI_FINISHPAGE_TITLE $(title3)
!define MUI_FINISHPAGE_TEXT  $(text3)
!define MUI_FINISHPAGE_RUN 
!define MUI_FINISHPAGE_RUN_NOTCHECKED
!define MUI_FINISHPAGE_RUN_TEXT     "Installer le d�p�t de code source GIT"
!define MUI_FINISHPAGE_RUN_FUNCTION "install_git"
!define MUI_FINISHPAGE_BUTTON       "Terminer"
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

Icon "${prodname.simple}\${Interface}\${icon}"

RequestExecutionLevel admin
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
  
FunctionEnd
 
 
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

Section
  CreateDirectory  $INSTDIR\${exemple}\Donnees\R-Alta�r
  CreateDirectory  $INSTDIR\${exemple}\Projets
  CreateDirectory  $INSTDIR\${xhl}
  CreateDirectory  $INSTDIR\${prodname.simple}\win
  
  SetDetailsPrint both
  SetOutPath $INSTDIR\${prodname.simple}
  File /r  "${prodname.simple}\Docs" 
  File /r  "${prodname.simple}\Outils" 
  File /r  "${prodname.simple}\.Rproj.user" 
  File     "${prodname.simple}\*.*" 
  
  SetOutPath $INSTDIR\${prodname.simple}\win
  File   "${prodname.simple}\win${processeur}\*.*" 
  
  SetOutPath $INSTDIR\${exemple}
  File /r  ${exemple}\Docs
  File     ${exemple}\*.*
    
  SetOutPath $DOCUMENTS\R\win-library\${Rversion_major}
  File /r  "${prodname.simple}\lib\*.*" 
  
SectionEnd

 Section /o $(AdvancedName) Advanced
    SetOutPath $INSTDIR\${prodname.simple}
    File /r  "${prodname.simple}\${Interface}" 
    File /r  "${prodname.simple}\${RDir}"
    File /r  "${prodname.simple}\${texDir}"
	File /r  "${prodname.simple}\${GitDir}"
    File /r  "${prodname.simple}\${RStudioDir}"
	
	SetOutPath $INSTDIR\${exemple}\Projets
	File  "${exemple}\Projets\anonyme.alt"
	File  "${exemple}\Projets\anonyme2.alt"
	
	SetOutPath $LOCALAPPDATA  
    File /r "${prodname.simple}\Local\RStudio-desktop"
  
    SetOutPath $APPDATA\RStudio  
    File  "${prodname.simple}\Roaming\RStudio\*.*"
	
	${EnvVarUpdate} $0 "PATH" "A" "HKCU" "$INSTDIR\${prodname.simple}\${texDir}\bin\win32"
	${EnvVarUpdate} $0 "PATH" "A" "HKCU" "$INSTDIR\${prodname.simple}\${GitDir}\bin" 
		
	StrCpy $minimal ""

 SectionEnd

 Section   $(MinimaleName) Minimale 
    SetOutPath $INSTDIR\${prodname.simple}
    File /r  "${prodname.simple}\${RDir}"
    File /r  "${prodname.simple}\${Interface.minimal}" 
	SetOutPath $INSTDIR\${exemple}\Projets
    File  "${exemple}\Projets\anonyme.minimal.alt"
	File  "${exemple}\Projets\anonyme2.minimal.alt"
	
    StrCpy $minimal "_min"

	CreateDirectory $INSTDIR\${prodname.simple}\${RStudioDir}\bin\pandoc
	SetOutPath $INSTDIR\${prodname.simple}\${RStudioDir}\bin\pandoc
	File ${prodname.simple}\${RStudioDir}\bin\pandoc\pandoc.exe

 SectionEnd

  Section  $(Sec6Name) sec6
    SetOutPath $INSTDIR\${xhl}
    File /r  ${xhl}\Anonyme
    File /r  ${xhl}\Anonyme2
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
   !insertmacro MUI_DESCRIPTION_TEXT ${sec6} $(DESC_sec6)
 !insertmacro MUI_FUNCTION_DESCRIPTION_END
  
 
Section 

 
 !insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  SetShellVarContext current
  
  SetOutPath       "$INSTDIR\${prodname.simple}\${Interface}$minimal"
  CreateShortCut   "$DESKTOP\${prodname}.lnk" "$INSTDIR\${prodname.simple}\${Interface}$minimal\${prodname}.exe"  "" "$INSTDIR\${prodname.simple}\${Interface}$minimal\${icon}"
  ;WriteRegStr HKLM "${prodname}\Shell\open\command\" "" "$INSTDIR\${prodname.simple}\${Interface}$minimal\${prodname}.exe"
  
  CreateDirectory  "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut   "$SMPROGRAMS\$StartMenuFolder\D�sinstaller.lnk" "$INSTDIR\D�sinstaller.exe" "" "$INSTDIR\D�sinstaller.exe" 0
  CreateShortCut   "$SMPROGRAMS\$StartMenuFolder\${prodname}.lnk" "$INSTDIR\${prodname.simple}\${Interface}$minimal\${prodname}.exe" "" "$INSTDIR\${prodname.simple}\${Interface}$minimal\${icon}" 0
  
  SetDetailsPrint textonly
  DetailPrint "Cr�ation des cl�s d'enregistrement..."
  SetDetailsPrint listonly

  WriteRegStr HKLM "${prodname}\DefaultIcon" "${prodname}" "$INSTDIR\${prodname.simple}\${Interface}\${icon}"
  WriteRegStr HKLM SOFTWARE\${prodname} \
     "Install_Dir" "$INSTDIR"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" \
     "DisplayName" "${prodname} (d�sinstallation)"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" \
     "UninstallString" "$\"$INSTDIR\D�sinstaller.exe$\""
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "NoModify" "1"
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}" "NoRepair" "1"
	  
  SetOutPath $INSTDIR

  ;WriteRegStr HKLM "${REG_UNINST_KEY}" "URLInfoAbout" "http://github.com/fabnicol/altair"
  ;WriteRegStr HKLM "${REG_UNINST_KEY}" "HelpLink" "http://github.com/fabnicol/altair"

  WriteUninstaller "$INSTDIR\D�sinstaller.exe"
 
!insertmacro MUI_STARTMENU_WRITE_END

SectionEnd
 
 Function install_git
   MessageBox MB_YESNO|MB_ICONINFORMATION "Une connexion Internet est n�cessaire. Cliquer sur Oui si elle est activ�e, sur Non pour refuser l'installation. Le code source du d�p�t git sera install� dans $INSTDIR\${prodname.simple}.git" IDYES true IDNO false

   true:
	 DetailPrint "Le code source du d�p�t git sera install� dans $INSTDIR\${prodname.simple}.git"   
  	 StrCpy $git "1"
	 Return
   false:
     DetailPrint "Le code source du d�p�t git ne sera pas install�."
	 StrCpy $git "0"
 FunctionEnd
 
 Function un.install_git
   MessageBox MB_YESNO|MB_ICONINFORMATION "D�sinstallation du d�p�t git ?" IDYES true IDNO false

   true:
     DetailPrint "Le code source du d�p�t git sera d�sinstall�."
     RMDir /r "$INSTDIR\${prodname.simple}.git"
	 Return
   false:	 
     DetailPrint "Le code source du d�p�t git ne sera pas d�sinstall�."
 FunctionEnd
  
Function .onInstSuccess
	${If} $git == "1"
	    ExecWait '"$INSTDIR\${prodname.simple}\git_alta�r.bat"' ; Exec est inad�quat ici.
	${EndIf}
FunctionEnd
  
Function un.onUninstSuccess
  MessageBox MB_OK "D�sinstallation r�ussie."
FunctionEnd

Section "Uninstall"
 
 !insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
 
  DeleteRegKey HKLM "SOFTWARE\${prodname}"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}"	
  DeleteRegKey HKLM "${prodname}\DefaultIcon"
  
  ${un.EnvVarUpdate} $0 "PATH" "R" "HKCU" "$INSTDIR\${prodname.simple}\${texDir}\bin\win32" 
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

  Call  un.install_git
SectionEnd

BrandingText "Alta�r-${version}"

; eof
