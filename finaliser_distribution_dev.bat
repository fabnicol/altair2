@echo off
echo Actualisation de la copie locale de l'entrepot 
set courant=%~dp0\altair
cd %courant%
%courant%\Git\bin\git.exe --version
%courant%\Git\bin\git.exe reset --hard HEAD
set /p login="Entrez votre login de session : "
set /p password="Entrez votre mot de passe de session : "
set /p proxy="Entrez le proxy [P pour 10.163.251.190]: "
set /p port="Entrez le port [P pour 8080]: "
if "%proxy%" ==  "P" set proxy=10.163.251.190
if "%port%"  ==  "P" set port=8080
%courant%\Git\bin\git.exe config --system http.sslcainfo C:\Users\Public\Dev\altair\Git\mingw64\ssl\certs\ca-bundle.crt
%courant%\Git\bin\git.exe config --global http.proxy http://%login%:%password%@%proxy%:%port% 
%courant%\Git\bin\git.exe config --global https.proxy https://%login%:%password%@%proxy%:%port% 
%courant%\Git\bin\git.exe config --global credential.helper.cache  wincred 
%courant%\Git\bin\git.exe config --global credential.helper  wincred 
%courant%\Git\bin\git.exe config --global user.name  "Fabrice Nicol"
%courant%\Git\bin\git.exe config --global user.email   fabrnicol@gmail.com 
%courant%\Git\bin\git.exe clean -d -f -x 
%courant%\Git\bin\git.exe remote add -t distribution-dev origin https://github.com/fabnicol/altair.git
%courant%\Git\bin\git.exe fetch --depth=1 origin distribution-dev
%courant%\Git\bin\git.exe branch --set-upstream-to=origin/distribution
%courant%\Git\bin\git.exe merge --quiet -s recursive -X theirs FETCH_HEAD
xcopy /I /Y /E %courant%\Roaming.dev\RStudio  %USERPROFILE%\AppData\Roaming\RStudio
xcopy /I /Y /E %courant%\Local.dev\RStudio-desktop  %USERPROFILE%\AppData\Local\RStudio-desktop
%courant%\Git\bin\git.exe rm -rf  --cached Tests\Exemple\Donnees\R-Al*
%courant%\Git\bin\git.exe rm -rf --cached .Rproj.user
%courant%\Git\bin\git.exe rm -f  --cached lhx\cl
%courant%\Git\bin\git.exe commit --quiet -a  -m "local mods"
REM END OF FILE


