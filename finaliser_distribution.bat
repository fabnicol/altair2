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
%courant%\Git\bin\git.exe config --global http.proxy http://%login%:%password%@%proxy%:%port% 
%courant%\Git\bin\git.exe config --global https.proxy https://%login%:%password%@%proxy%:%port% 
%courant%\Git\bin\git.exe config --global credential.helper.cache  wincred 
%courant%\Git\bin\git.exe config --global credential.helper  wincred 
%courant%\Git\bin\git.exe config --global user.name  "Fabrice Nicol"
%courant%\Git\bin\git.exe config --global user.email   fabrnicol@gmail.com 
%courant%\Git\bin\git.exe init
xcopy /q /I /Y /E Git  Git0
%courant%\Git0\bin\git.exe add Git0
%courant%\Git0\bin\git.exe clean  -d -f -x 
%courant%\Git0\bin\git.exe remote add -t distribution-dev origin https://github.com/fabnicol/altair.git
%courant%\Git0\bin\git.exe fetch  --depth=1 origin distribution
%courant%\Git0\bin\git.exe branch -u origin/distribution
%courant%\Git0\bin\git.exe merge --quiet -s recursive -X theirs FETCH_HEAD
xcopy /I /Y /E Roaming.dev\RStudio  %USERPROFILE%\AppData\Roaming\RStudio
xcopy /I /Y /E Local.dev\RStudio-desktop  %USERPROFILE%\AppData\Local\RStudio-desktop
%courant%\Git\bin\git.exe rm -rf --cached Tests\Exemple\Donnees\R-Al*
%courant%\Git\bin\git.exe rm -rf --cached Git0
%courant%\Git\bin\git.exe branch -m distribution
del lhx\cl
%courant%\Git\bin\git.exe clean  -d -f -x 
%courant%\Git\bin\git.exe commit --quiet -a  -m "actualisation Dist--cleanup"
REM END OF FILE


