#!/bin/sh

# run the clean up command
premake4 --file=build.premake4 clean

# build for codeblocks and gmake
premake4 --file=build.premake4 codeblocks
premake4 --file=build.premake4 codelite
premake4 --file=build.premake4 gmake
