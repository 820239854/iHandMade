@echo off

mkdir .\build
pushd .\build
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cl -Zi ..\code\win32_handmade.cpp user32.lib
popd