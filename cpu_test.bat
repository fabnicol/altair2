@echo off
for /f "tokens=2 delims==" %%A in ('wmic cpu get name /value') do @(echo "%%A") | ((echo "%%A" | find /I  "core2") || (echo "%%A" | find /I "celeron"))  >nul && (echo 1 > cpu_type) || (echo 0 > cpu_type)

REM puis lire cpu_type

