@echo off
setlocal enabledelayedexpansion
set "scriptPath=%~dp0"
cd ../
set "currentDirectory=%CD%"
cmake.exe -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_SYSTEM_NAME=Windows -DCMAKE_SYSTEM_PROCESSOR=x64 -DCMAKE_SYSTEM_VERSION=10 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
pause
endlocal
