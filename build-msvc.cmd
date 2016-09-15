@echo off

setlocal ENABLEDELAYEDEXPANSION

rem Process the command line arguments.
set flag_batch=0
set msvc_version=14.0
for %%a in (%*) do (
    set flag="%%a"
    if "!flag!"=="/batch" set flag_batch=1
    if "!flag:~0,9!"=="/version:" set version="!arg:~7!"
)

rem Build Picol with MSVC.
echo on
call "C:\Program Files (x86)\Microsoft Visual Studio %msvc_version%\VC\vcvarsall.bat"
echo on
nmake /F Makefile.nmake
@if "%flag_batch%"=="0" pause
