 Spiral Game Engine:

 To build for visual studio 2005/2008 download premake from : http://industriousone.com/premake
 Edit buildscript to point to the directory you have premake installed. Run buildscript

 Running Simple App:
 Working Directory needs to be set to Spiral or directory of this readme
 
 Needed Sdks:
 - Boost-C++ (bjam to build needed libs)
 - wxWidgets (for Console App) Note: Build wxWidgets as static libs


 Note: for console to work correctly, it needs to be renamed to Console inside the generate bin folder inside your ide-folder-name
 example: Spiral/Build/bin/Releasevs2008/Console_relvs2008.exe renamed to Spiral/Console.exe