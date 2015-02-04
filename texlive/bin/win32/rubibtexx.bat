@echo off
bibtex8 -B -c cp1251.csf %1.aux
iconv -f CP1251 -t UTF-8 %1.bbl > tmp.tmp
copy /b tmp.tmp %1.bbl
del tmp.tmp




