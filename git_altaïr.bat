@echo off
echo Installation de la copie locale de l'entrepot 
echo Gestionnaire de version sous c:\Users\Public\Altair\Git
set courant=%~dp0
%courant%\Git\bin\git.exe --version
cd %courant%
cd ..
set /p login="Entrez votre login de session : "
set /p password="Entrez votre mot de passe de session : "
set /p proxy="Entrez le proxy [P pour 10.163.251.190]: "
set /p port="Entrez le port [P pour 8080]: "
if "%proxy%" ==  "P" set proxy=10.163.251.190
if "%port%"  ==  "P" set port=8080
echo [https] >> %courant%\Git\etc\gitconfig
echo     proxy = %login%:%password%@%proxy%:%port% >> %courant%\Git\etc\gitconfig
echo Clonage en cours...
%courant%\Git\bin\git.exe clone --depth 1 --single-branch --branch distribution https://github.com/fabnicol/altair.git altair.git
cd altair.git
%courant%\Git\bin\git.exe checkout distribution
echo Fin de l'installation.
xcopy /I /Y /E C:\Users\Public\Altair\Roaming\RStudio  %USERPROFILE%\AppData\Roaming\RStudio
xcopy /I /Y /E C:\Users\Public\Altair\Local\RStudio-desktop  %USERPROFILE%\AppData\Local\RStudio-desktop
cd %courant%
