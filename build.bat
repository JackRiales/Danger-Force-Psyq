@ECHO off

SET CFLAGS=-O3 -Xo$80100000 -Iextlib
REM SET CFLAGS=%CFLAGS% -DPXFINDER
SET CFLAGS=%CFLAGS% -DDEBUG
SET SOURCES=code\main.c
REM SET SOURCES=%SOURCES% extlib\pxfinder\pxfinder.obj 

if exist "build\" rd /q /s "build\" 
mkdir build

cls
ccpsx %CFLAGS% %SOURCES% -obuild\main.cpe,build\main.sym,build\mem.map
cd build
if exist main.cpe cpe2x main.cpe
if exist main.exe ren main.exe main.psx
cd ..
pause
