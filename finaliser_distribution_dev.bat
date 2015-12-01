@echo off
echo Actualisation de la copie locale de l'entrepot 
set courant=%~dp0
cd %courant%
%courant%\Git\bin\git.exe --version
set /p login="Entrez votre login de session : "
set /p password="Entrez votre mot de passe de session : "
set /p proxy="Entrez le proxy [P pour 10.163.251.190]: "
set /p port="Entrez le port [P pour 8080]: "
if "%proxy%" ==  "P" set proxy=10.163.251.190
if "%port%"  ==  "P" set port=8080
copy /Y %courant%\Git\etc\gitconfig %courant%\Git\etc\gitconfig.save
echo [https] >> %courant%\Git\etc\gitconfig
echo     proxy = %login%:%password%@%proxy%:%port% >> %courant%\Git\etc\gitconfig
%courant%\Git\bin\git.exe reset --hard HEAD
%courant%\Git\bin\git.exe clean -d -f
%courant%\Git\bin\git.exe remote add -t distribution-dev origin https:\\github.com\fabnicol\altair.git
%courant%\Git\bin\git.exe fetch origin distribution-dev
%courant%\Git\bin\git.exe merge -s recursive -X theirs FETCH_HEAD
del %courant%\Git\etc\gitconfig
copy /Y %courant%\Git\etc\gitconfig.save %courant%\Git\etc\gitconfig


