@echo off
set SDLPATH=%LibSDL2MSVC%
cl /nologo user32.lib /I%SDLPATH%\include %SDLPATH%\lib\x64\SDL2main.lib %SDLPATH%\lib\x64\SDL2.lib src\*.c /Fe:bin\out.exe
copy /Y %SDLPATH%\lib\x64\SDL2.dll bin
echo -I > compile_flags.txt
echo %SDLPATH%\include >> compile_flags.txt
del *.obj