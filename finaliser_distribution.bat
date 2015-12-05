@echo off
echo Actualisation de la copie locale de l'entrepot 
set courant=%~dp0\Altair
cd %courant%
%courant%\Git\bin\git.exe --version
%courant%\Git\bin\git.exe config --global credential.helper.cache  wincred 
%courant%\Git\bin\git.exe config --global credential.helper  wincred 
%courant%\Git\bin\git.exe config --global user.name  "Fabrice Nicol"
%courant%\Git\bin\git.exe config --global user.email   fabrnicol@gmail.com 
%courant%\Git\bin\git.exe init
%courant%\Git\bin\git.exe add -f Git
%courant%\Git\bin\git.exe commit -am "Start" --quiet
%courant%\Git\bin\git.exe clean -d -f -x 
%courant%\Git\bin\git.exe remote add -t distribution origin https://github.com/fabnicol/altair.git
%courant%\Git\bin\git.exe fetch --depth=1 origin distribution
%courant%\Git\bin\git.exe merge --quiet -s recursive -X theirs FETCH_HEAD
xcopy /I /Y /E %courant%\Roaming\RStudio  %USERPROFILE%\AppData\Roaming\RStudio
xcopy /I /Y /E %courant%\Local\RStudio-desktop  %USERPROFILE%\AppData\Local\RStudio-desktop
%courant%\Git\bin\git.exe rm -rf  --cached Tests\Exemple\Donnees\R-Al*
%courant%\Git\bin\git.exe rm -rf --cached .Rproj.user
%courant%\Git\bin\git.exe rm -f  --cached lhx\cl
mkdir %courant%\Tests\Exemple\Donnees\R-Altaïr
mkdir %courant%\lhx
%courant%\Git\bin\git.exe commit --quiet -a  -m "local mods"
%courant%\Git\bin\git.exe config --global http.proxy 
%courant%\Git\bin\git.exe config --global https.proxy 
REM END OF FILE


