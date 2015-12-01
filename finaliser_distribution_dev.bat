set courant=%~dp0
cd %courant%
%courant%\Git\bin\git.exe reset --hard HEAD
%courant%\Git\bin\git.exe clean -d -f
%courant%\Git\bin\git.exe remote add -t distribution-dev origin https:\\github.com\fabnicol\altair.git
%courant%\Git\bin\git.exe fetch origin distribution-dev
%courant%\Git\bin\git.exe merge -s recursive -X theirs FETCH_HEAD


