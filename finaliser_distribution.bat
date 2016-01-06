@echo off
echo Actualisation de la copie locale de l'entrepot 
set courant=%~dp0\Altair
cd %courant%
%courant%\Git\bin\git.exe --version
set /p login="Entrez votre login de session : "
set /p password="Entrez votre mot de passe de session : "
set /p proxy="Entrez le proxy [P pour 10.163.251.190]: "
set /p port="Entrez le port [P pour 8080]: "
if "%proxy%" ==  "P" set proxy=10.163.251.190
if "%port%"  ==  "P" set port=8080
 REM %courant%\Git\bin\git.exe config --system http.sslcainfo C:\Users\Public\Dev\altair\Git\mingw64\ssl\certs\ca-bundle.crt
%courant%\Git\bin\git.exe config --global http.proxy http://%login%:%password%@%proxy%:%port% 
%courant%\Git\bin\git.exe config --global https.proxy https://%login%:%password%@%proxy%:%port% 
%courant%\Git\bin\git.exe config --global credential.helper.cache  wincred 
%courant%\Git\bin\git.exe config --global credential.helper  wincred 
%courant%\Git\bin\git.exe config --global user.name  "Fabrice Nicol"
%courant%\Git\bin\git.exe config --global user.email   fabrnicol@gmail.com 
%courant%\Git\bin\git.exe init
xcopy /q /I /Y /E %courant%\Git %~dp0\Git0
%~dp0\Git0\bin\git.exe clean  -d -f -x 
%~dp0\Git0\bin\git.exe remote add -t distribution origin https://github.com/fabnicol/altair.git
%~dp0\Git0\bin\git.exe fetch  --depth=1 origin distribution
echo Importation du code en cours...
%~dp0\Git0\bin\git.exe merge --quiet -s recursive -X theirs FETCH_HEAD
%~dp0\Git0\bin\git.exe branch -u origin/distribution
del /q %~dp0\Git0
xcopy /I /Y /E Roaming\RStudio  %USERPROFILE%\AppData\Roaming\RStudio
xcopy /I /Y /E Local\RStudio-desktop  %USERPROFILE%\AppData\Local\RStudio-desktop
%courant%\Git\bin\git.exe rm -rf --quiet --cached Tests\Exemple\Donnees\R-Al*
del /q lhx\cl
%courant%\Git\bin\git.exe clean  -d -f -x 
%courant%\Git\bin\git.exe rm -rf --quiet --cached .Rproj.user
cd ..
echo Fin.
REM END OF FILE
