@echo off

call ..\Premake\premake5.exe --file="%~dp0\premake5.lua" vs2019

echo.
echo Project generated.
echo.

pause
