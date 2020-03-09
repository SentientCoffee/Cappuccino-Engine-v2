@echo off

echo Starting shader check...

for /f %%f in ('dir /s /b /a-d "C:\Repos\CappuccinoEngine-v2\Cappuccino\Assets\Shaders"') do @if exist "%%f" Externals\glslang\bin\glslangValidator %%f
for /f %%f in ('dir /s /b /a-d "C:\Repos\CappuccinoEngine-v2\Sandbox\Assets\Shaders"') do @if exist "%%f" Externals\glslang\bin\glslangValidator %%f

echo Finished checking shaders.