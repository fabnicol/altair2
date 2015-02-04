@echo off

:init

  rem Avoid clobbering anyone else's variables

  setlocal

  rem Safety precaution against awkward paths

  cd /d "%~dp0"
  
:main

  call :set_lua
  if ERRORLEVEL 1 (
    goto :end
  )
  
  set BASENAME=%~n0
  set DIRNAME=%~dp0
  
  set LUASCRIPT=%DIRNAME%\lua2dox.lua %BASENAME%

  if not [%*] == [] (
    echo %*
  )
  %EXE% %LUASCRIPT% %*

  goto :end
  
:set_lua

  call :exe-search texlua
  
  if not defined EXE (
    call :exe-search lua
  )
  
  if not defined EXE (
    echo No Lua interpreter available
    exit /b 1
  )
  
  goto :EOF
  
:exe-search

  set PATHCOPY=%PATH%

:exe-search-loop

  rem Search for a binary

  if defined EXE goto :EOF

  for /f "delims=; tokens=1,2*" %%I in ("%PATHCOPY%") do (
    if exist "%%I\%1.exe" (
      set EXE=%1
    )
    set PATHCOPY=%%J;%%K
  )
  if not "%PATHCOPY%" == ";" goto :exe-search-loop

  exit /b 1

  goto :EOF

:end