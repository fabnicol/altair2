
!include "MUI2.nsh"
!include "x64.nsh"

!define version  "2014.5"
!define prodname "Altair"
!define setup    "${prodname}-${version}.win32.installer.exe"
!define srcdir   "."
!define altair   "${srcdir}\Dev\altair"
!define images   "${altair}\Tests\Exemple"
!define icon     "neptune.ico"
!define regkey   "Software\${prodname}-${version}"
!define uninstkey "Software\Microsoft\Windows\CurrentVersion\Uninstall\${prodname}-${version}"
!define startmenu "$SMPROGRAMS\${prodname}-${version}"
!define uninstaller "uninstall.exe"
!define notefile  "${altair}\LISEZ-MOI.txt"
!define MUI_ICON  "${images}\${icon}"
!define MUI_WELCOMEFINISHPAGE
!define MUI_WELCOMEPAGE_TEXT  $(wizard1)
!define MUI_WELCOMEPAGE_TITLE $(wizard2)
!define MUI_HEADERIMAGE
!define MUI_HEADERIMAGE_BITMAP "${images}\${prodname}.bmp"
!define MUI_WELCOMEFINISHPAGE_BITMAP "${images}\neptune.bmp"
!define MUI_ABORTWARNING
!insertmacro MUI_PAGE_WELCOME  ; 
!insertmacro MUI_LANGUAGE "French" 

 LangString  wizard1 ${LANG_FRENCH}  "Installation du logiciel Analyse des lignes de traitement, attributions indemnitaires et autres rémunérations. Appuyer sur suivant pour continuer."
 LangString  wizard2 ${LANG_FRENCH}  "Installation du logiciel ${prodname} version ${version}"
 LangString title1 ${LANG_FRENCH}   "Installation terminée"
 LangString title1 ${LANG_FRENCH}  "Installation terminée"
 LangString text1 ${LANG_FRENCH}  "${prodname} ${version} a été installé dans $INSTDIR"
 LangString uninstall ${LANG_FRENCH}     "désinstallation du logiciel ${prodname} "
 LangString completed ${LANG_FRENCH}    "Installation terminée"
 LangString Sec1Name ${LANG_FRENCH} "Altair"
 LangString DESC_sec1 ${LANG_FRENCH} "Installer Altair"
 LangString Sec2Name ${LANG_FRENCH} "R"
 LangString Sec3Name ${LANG_FRENCH} "RStudio"
 LangString Sec4Name ${LANG_FRENCH} "MikTex"
 LangString Sec5Name ${LANG_FRENCH} "Git"
 LangString DESC_sec2 ${LANG_FRENCH} "Installer le langage R"
 LangString DESC_sec3 ${LANG_FRENCH} "Installer l'interface RStudio"
 LangString DESC_sec4 ${LANG_FRENCH} "Installer le système MikTex (création de documents pdf)"
 LangString DESC_sec5 ${LANG_FRENCH} "Installer le gestionnaire de versions GIT"

 
 LangString Message  ${LANG_FRENCH} "Appuyer sur Oui pour installer ${prodname}"
  
 LicenseLangString myLicenseData ${LANG_FRENCH} "${altair}\LICENCE"
 LicenseData $(myLicenseData)
  LangString Name ${LANG_FRENCH}  "${prodname}"
 Name $(Name)

; MUI macros   

!define MUI_FINISHPAGE_TITLE $(title1)
!define MUI_FINISHPAGE_TEXT  $(text1)
!define MUI_FINISHPAGE_RUN 
!define MUI_FINISHPAGE_RUN_TEXT "Lire le fichier LISEZ-MOI"
!define MUI_FINISHPAGE_RUN_FUNCTION "Launch_${prodname}"
!define MUI_FINISHPAGE_BUTTON  "Terminer"
!define MUI_FINISHPAGE_CANCEL_ENABLED 

!insertmacro MUI_PAGE_LICENSE "${altair}\LICENCE"
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
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


AutoCloseWindow false
ShowInstDetails show


Function .onInit
  SetOutPath $TEMP	
  File /oname=spltmp.bmp "spltmp.bmp"

  advsplash::show 1300 600 400 -1 $TEMP\spltmp

  Pop $0 ; $0 has '1' if the user closed the splash screen early,
         ; '0' if everything closed normally, and '-1' if some error occurred.

  Delete $TEMP\spltmp.bmp

FunctionEnd


Function Launch_${prodname}
  Exec '"notepad" "$INSTDIR\${notefile}"'
FunctionEnd

Section
MessageBox MB_YESNO|MB_ICONINFORMATION $(Message)  IDNO Fin IDYES End
Fin: Abort
End:

CreateDirectory "${startmenu}"

SectionEnd


Section  ; allways done
   SetOutPath $INSTDIR
  
   CreateShortCut "${startmenu}\${prodname}.lnk" "$INSTDIR\RStudio\bin\rstudio.exe"
  
   WriteRegStr HKCR "${prodname}\Shell\open\command\" "" "$INSTDIR\RStudio\bin\rstudio.exe"

   CreateShortCut "${startmenu}\${uninstaller}.lnk" "$INSTDIR\${uninstaller}"
  
  WriteRegStr HKLM "${regkey}" "Install_Dir" "$INSTDIR"
 
  WriteRegStr HKLM "${uninstkey}" "DisplayName" "${prodname} (uninstall only)"
  WriteRegStr HKLM "${uninstkey}" "UninstallString" '"$INSTDIR\${uninstaller}"'
  SetOutPath $INSTDIR

  WriteUninstaller "${uninstaller}"

  SetOutPath $INSTDIR ; for working directory
  File /r  "${srcdir}\Dev"  
  File  R-3.1.0-win.exe RStudio-0.98.831.exe basic-miktex-2.9.5105.exe Git-1.9.2-preview20140411.exe

SectionEnd


 Section  $(Sec2Name) sec2 
   SetOutPath $INSTDIR 
   ExecShell "" R-3.1.0-win.exe
   ;ExecWait  'cmd.exe /C "start /B /MAX /WAIT R-3.1.0-win.exe"'
 SectionEnd

 Section  $(Sec3Name) sec3 
   SetOutPath $INSTDIR 
   ExecShell "" RStudio-0.98.831.exe
   ;ExecWait  'cmd.exe /C "start /B /MAX /WAIT RStudio-0.98.831.exe"'
 SectionEnd

 Section  $(Sec4Name) sec4
  SetOutPath $INSTDIR 
  ExecShell "" basic-miktex-2.9.5105.exe
  ;ExecWait  'cmd.exe /C "start /B /MAX /WAIT basic-miktex-2.9.5105.exe"'
 SectionEnd
 
 Section  $(Sec5Name) sec5
  SetOutPath $INSTDIR 
  ExecShell "" Git-1.9.2-preview20140411.exe
  ;ExecWait  'cmd.exe /C "start /B /MAX /WAIT Git-1.9.2-preview20140411.exe"' 
 SectionEnd

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${sec1} $(DESC_sec1)
  !insertmacro MUI_DESCRIPTION_TEXT ${sec2} $(DESC_sec2)
  !insertmacro MUI_DESCRIPTION_TEXT ${sec3} $(DESC_sec3)
  !insertmacro MUI_DESCRIPTION_TEXT ${sec4} $(DESC_sec4)
  !insertmacro MUI_DESCRIPTION_TEXT ${sec5} $(DESC_sec5)
!insertmacro MUI_FUNCTION_DESCRIPTION_END

Section "Uninstall"

  DeleteRegKey HKLM "${uninstkey}"
  DeleteRegKey HKLM "${regkey}"
    
  Delete "${startmenu}\*.*"
  Delete "${startmenu}"

  Delete "$INSTDIR\*.*"
  RMDir /r "$INSTDIR"
 
SectionEnd

BrandingText "${prodname}.${version}"

; eof
