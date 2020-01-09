@echo off

echo Generating Cappuccino Engine and related projects...
echo.

call Premake\premake5.exe vs2019 --file="%~dp0\premake5.lua"

echo.
echo Complete.
echo.

pause