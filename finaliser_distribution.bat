xcopy /I /Y /E C:\Users\Public\Altair\Roaming\RStudio  %USERPROFILE%\AppData\Roaming\RStudio
xcopy /I /Y /E C:\Users\Public\Altair\Local\RStudio-desktop  %USERPROFILE%\AppData\Local\RStudio-desktop
set courant=%~dp0
cd %courant%
%courant%\Git\bin\git.exe rm -f  --cached Tests\Exemple\Donnees\R-Altaïr\Bulletins.csv
%courant%\Git\bin\git.exe rm -rf --cached .Rproj.user
%courant%\Git\bin\git.exe rm -f  --cached lhx\cl