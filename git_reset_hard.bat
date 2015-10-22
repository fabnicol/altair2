@echo off
c:\windows\system32\cmd.exe /C  echo Sauvegarde du code source. Cette sauvegarde peut durer entre 15 et 30 minutes... && Git\bin\git.exe init && Git\bin\git.exe add . && Git\bin\git.exe config --global user.email unknown@unknown.fr && Git\bin\git.exe config --global user.name  "Prénom Nom" && Git\bin\git.exe commit -am "Start"  
