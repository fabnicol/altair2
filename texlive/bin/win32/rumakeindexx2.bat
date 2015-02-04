@echo off
iconv -f UTF-8 -t CP1251 %1.idx > tmp.tmp
copy /b tmp.tmp %1.idx
del tmp.tmp
makeindex %1.idx -t %1.ilg  -s %2.ist -o %1.ind
iconv -f CP1251 -t UTF-8 %1.ind > tmp.tmp
copy /b tmp.tmp %1.ind
del tmp.tmp
