@echo off

set path=D:\dev\ui;%path%

set SOURCES=main.cpp
set LINK=opengl32.lib winmm.lib gdi32.lib shell32.lib lib\SDL2.lib lib\SDL2main.lib lib\SDL2_ttf.lib lib\SDL2_image.lib

REM DEBUG
set CFLAGS=/MP /MTd /EHsc /Zi /FC /nologo /W1 /Od

REM RELEASE
REM set CFLAGS=/MP /MT /EHsc /FC /nologo /W1 /O2

REM set SKIP_WARNINGS=/wd"4838"
set BUILD_DIR=/Fdbuild\ /Fobuild\
set EXE=build/ui.exe

cl %CFLAGS% %BUILD_DIR% /Fe%EXE% /Iinclude %SOURCES% /link %LINK% /SUBSYSTEM:CONSOLE

rem IF %ERRORLEVEL% EQU 0 start /wait %EXE%