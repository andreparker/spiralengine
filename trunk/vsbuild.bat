@echo off

if not "%VS90COMNTOOLS%" == "" goto VS90
if not "%VS80COMNTOOLS%" == "" goto VS80
if not "%VS70COMNTOOLS%" == "" goto VS70
goto NO_VS_INSTALLED

:VS90
set path=%path%;%VS90COMNTOOLS%;
goto RUN_BAT

:VS80
set path=%path%;%VS80COMNTOOLS%;
goto RUN_BAT

:VS70
set path=%path%;%VS70COMNTOOLS%;
goto RUN_BAT

:RUN_BAT
call "vsvars32.bat"
if exist "Build\vs2008\Spiral.sln" goto BUILD_2008
if exist "Build\vs2005\Spiral.sln" goto BUILD_2005
if exist "Build\vs2003\Spiral.sln" goto BUILD_2003


:BUILD_2008
MSBuild Build\vs2008\Spiral.sln /t:Rebuild /p:Configuration="Debugvs2008" /p:Platform="Win32" /m
MSBuild Build\vs2008\Spiral.sln /t:Rebuild /p:Configuration="Releasevs2008" /p:Platform="Win32" /m
goto :eof

:BUILD_2005
MSBuild Build\vs2005\Spiral.sln /t:Rebuild /p:Configuration="Debugvs2005" /p:Platform="Win32" /m
MSBuild Build\vs2005\Spiral.sln /t:Rebuild /p:Configuration="Releasevs2005" /p:Platform="Win32" /m
goto :eof

:BUILD_2003
MSBuild Build\vs2003\Spiral.sln /t:Rebuild /p:Configuration="Debugvs2003" /p:Platform="Win32" /m
MSBuild Build\vs2003\Spiral.sln /t:Rebuild /p:Configuration="Releasevs2003" /p:Platform="Win32" /m
goto :eof

:NO_VS_INSTALLED
echo No compatible visual studio installation detected!
