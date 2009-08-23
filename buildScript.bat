@echo off

rem point global path to premake
rem change this to where premake is stored
set path=%path%;"d:\code\tools\bin"

rem premake4 --help
premake4 --file=build.premake4 clean
echo Building vs 2005 project files..
premake4 --file=build.premake4 vs2005
echo Building vs 2008 project files..
premake4 --file=build.premake4 vs2008