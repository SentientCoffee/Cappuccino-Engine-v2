@echo off

echo Starting shader check...

for /f "usebackq tokens=*" %%f in (`dir /s /b /a-d "Cappuccino\Assets\Shaders" ^| findstr /ile ".vert .frag .geom"`) do @if exist "%%f" Externals\glslang\bin\glslangValidator "%%f"
for /f "usebackq tokens=*" %%f in (`dir /s /b /a-d "Sandbox\Assets\Shaders" ^| findstr /ile ".vert .frag .geom"`) do @if exist "%%f" Externals\glslang\bin\glslangValidator "%%f"

echo Finished checking shaders.