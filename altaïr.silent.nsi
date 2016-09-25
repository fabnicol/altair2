SetCompress auto
!define DATA

!addincludedir  "${NSISDIR}\Include"

!ifdef NSIS_UNICODE
	!addplugindir "${NSISDIR}\Plugins\Release_Unicode"
!else
	!addplugindir "${NSISDIR}\Plugins\Release_ANSI"
!endif

!include "Sections.nsh"
!include "MUI2.nsh"
!include "x64.nsh"
!include "${NSISDIR}\Contrib\Modern UI\System.nsh"
!include "StrFunc.nsh"
!include "EnvVarUpdate.nsh"
!include "LogicLib.nsh"
!include "CPUFeatures.nsh"
 
;!addplugindir "c:\Program Files (x86)\NSIS\Plugins"
!include FileFunc.nsh
!insertmacro GetParameters
!insertmacro GetOptions

; Num�ros de version 

!define version  "2016.07"
Var processeur 
Var type 

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

 Var StartMenuFolder
 InstallDir "C:\Users\Public" 
 InstallDirRegKey HKLM "SOFTWARE\${prodname}" "Install_Dir"
  
     
;!insertmacro MUI_PAGE_STARTMENU Application $StartMenuFolder
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
SilentInstall normal

RequestExecutionLevel user
AutoCloseWindow true
Icon "${prodname.simple}\Interface_windows\${icon}"  

ShowInstDetails show

Function .onInit
 
  SetOutPath $TEMP	
  File /oname=spltmp.bmp "${prodname.simple}\spltmp.bmp"

  advsplash::show 2300 600 400 -1 $TEMP\spltmp

  Delete $TEMP\spltmp.bmp

  StrCpy $processeur  ""  ; 
  StrCpy $type  ""  ; 
  
  ${GetParameters} $R0
  ClearErrors
  ${GetOptions} $R0 /CPU= $2
  ${GetOptions} $R0 /TYPE= $3
  
  ${CPUFeatures.CheckFeature} "AVX2" $7 
  
FunctionEnd


Section
  
  SetOutPath $INSTDIR\${prodname.simple}  
  File /oname=cpu_test.vbs "${prodname.simple}\cpu_test.vbs"
  
  ; besoin d'execwait sinon le test prend trop de temps pour que la valeur r�sultat soit r�cup�r�e
  ; VBScript �vite d'avoir un petit-fils en batch, qui ne serait pas g�r� par ExecWait
  ; lancer batch directement causerait le lancement d'une console inutile 
  
  ;ExecWait '"$SYSDIR\wscript.exe" //Nologo //B "$INSTDIR\${prodname.simple}\cpu_test.vbs"'
    
  ;FileOpen $8 $INSTDIR\${prodname.simple}\cpu_check r
  ;FileRead $8 $9  
  ;FileClose $8
    
  CreateDirectory  $INSTDIR\${exemple}\Donnees\R-Alta�r
  CreateDirectory  $INSTDIR\${exemple}\Projets
  CreateDirectory  $INSTDIR\${xhl}
  CreateDirectory  $INSTDIR\${prodname.simple}\lhx
  CreateDirectory  $INSTDIR\${prodname.simple}\win
  CreateDirectory $LOCALAPPDATA\${prodname}  
  FileOpen $8 $LOCALAPPDATA\${prodname}\rank w
  FileClose $8
  
  StrCpy $processeur $2
  StrCpy $type $3
  
  ${If}  "$processeur" == ""
	  ${If} "$7" == "no"
		 StrCpy $processeur "core2" 
	  ${EndIf}
  ${EndIf}	  
  
  SetOutPath $INSTDIR\${prodname.simple}  
  
  !ifdef DATA
   ; File /r  "${prodname.simple}\Docs" 
   ; File /r  "${prodname.simple}\Outils" 
    File /r  "${prodname.simple}\lib" 
    File     "${prodname.simple}\*.*" 
    File /r  "${prodname.simple}\RStudio-project\.Rproj.user" 
    File /r  "${prodname.simple}\${RDir}"
  !endif
 
  
  SetOutPath $INSTDIR\${prodname.simple}
  
  ${If} "$processeur" == "core2"  

   ; ${If} "$type" == "min"  
	
    ;  File /r  "${prodname.simple}\Interface_windows_min_core2" 
    
	; ${Else}

      File /r  "${prodname.simple}\Interface_windows_core2" 
	  
    ;${EndIf}

	SetOutPath $INSTDIR\${prodname.simple}\win
	File  "${prodname.simple}\win_core2\*.*"   	
	  
  ${Else}	

	;	${If} "$type" == "min"
		  
	;	  File /r  "${prodname.simple}\Interface_windows_min" 

	;	${Else}

		  File /r  "${prodname.simple}\Interface_windows" 
	  
	;	${EndIf}
	
    SetOutPath $INSTDIR\${prodname.simple}\win	
	File  "${prodname.simple}\win\*.*"   	

  ${EndIf}	
  
  ;${If} "$type" == "min"
  
;	CreateDirectory $INSTDIR\${prodname.simple}\${RStudioDir}\bin\pandoc
;	SetOutPath $INSTDIR\${prodname.simple}\${RStudioDir}\bin\pandoc
;	File ${prodname.simple}\${RStudioDir}\bin\pandoc\pandoc.exe
			
 ; ${Else}  ; "avanc�"
   
    SetOutPath $INSTDIR\${prodname.simple}
   !ifdef DATA  
   ;  File /r  "${prodname.simple}\${texDir}"
   ;  File /r  "${prodname.simple}\${GitDir}"
     File /r  "${prodname.simple}\${RStudioDir}"
   !endif
	
	SetOutPath $LOCALAPPDATA  
    File /r "${prodname.simple}\Local\RStudio-desktop"
  
    SetOutPath $APPDATA\RStudio  
    File  "${prodname.simple}\Roaming\RStudio\*.*"
	
	${EnvVarUpdate} $0 "PATH" "A" "HKCU" "$INSTDIR\${prodname.simple}\${texDir}\miktex\bin\x64"
	${EnvVarUpdate} $0 "PATH" "A" "HKCU" "$INSTDIR\${prodname.simple}\${GitDir}\bin" 
	
 ; ${EndIf}
    
  !ifdef DATA
    SetOutPath $INSTDIR\${exemple}
    File /r  ${exemple}\Docs
    File     ${exemple}\*.*

    SetOutPath $INSTDIR\${xhl}
    File /r  ${xhl}\Anonyme
    File /r  ${xhl}\Anonyme2
  !endif

 SectionEnd

Section 
  
 ;!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  SetShellVarContext current
  
  ${If} "$processeur" == "core2"  

    ; ${If} "$type" == "min"  

  ; StrCpy $5 "$INSTDIR\${prodname.simple}\Interface_windows_min_core2\gui\x64\${prodname}.exe"  
  ; StrCpy $6 "$INSTDIR\${prodname.simple}\Interface_windows_min_core2\${icon}"
	
    
	; ${Else}

  StrCpy $5 "$INSTDIR\${prodname.simple}\Interface_windows_core2\gui\x64\${prodname}.exe"  
  StrCpy $6 "$INSTDIR\${prodname.simple}\Interface_windows_core2\${icon}"
  
  
   ; ${EndIf}

	  
  ${Else}	

		; ${If} "$type" == "min"
		  
  ; StrCpy $5 "$INSTDIR\${prodname.simple}\Interface_windows_min\gui\x64\${prodname}.exe"  
  ; StrCpy $6 "$INSTDIR\${prodname.simple}\Interface_windows_min\${icon}"
	

		; ${Else}

  StrCpy $5 "$INSTDIR\${prodname.simple}\Interface_windows\gui\x64\${prodname}.exe"  
  StrCpy $6 "$INSTDIR\${prodname.simple}\Interface_windows\${icon}"
	

		;${EndIf}
	    
  ${EndIf}	
  
  
  CreateShortCut   "$DESKTOP\${prodname}.lnk" $5  "" $6
    
  CreateDirectory  "$SMPROGRAMS\$StartMenuFolder"
  CreateShortCut   "$SMPROGRAMS\$StartMenuFolder\D�sinstaller.lnk" "$INSTDIR\D�sinstaller.exe" "" "$INSTDIR\D�sinstaller.exe" 0
  
  
  CreateShortCut   "$SMPROGRAMS\$StartMenuFolder\${prodname}.lnk" $5 "" $6 0
  SetDetailsPrint listonly
	  
  SetOutPath $INSTDIR

  WriteUninstaller "$INSTDIR\D�sinstaller.exe"
 
;!insertmacro MUI_STARTMENU_WRITE_END

SectionEnd
 
Section "Uninstall"
 
 ;!insertmacro MUI_STARTMENU_GETFOLDER Application $StartMenuFolder
  
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
;    MessageBox  MB_ICONINFORMATION "D�sinstallation r�alis�e."
  FunctionEnd

BrandingText "Alta�r-${version}"

; eof
