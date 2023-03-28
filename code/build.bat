@echo off

IF NOT EXIST .\build mkdir .\build
pushd .\build
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
cl -DHANDMADE_WIN32=1 -FC -Zi ..\code\win32_handmade.cpp user32.lib gdi32.lib
popd