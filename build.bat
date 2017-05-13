@ECHO off

if exist "build\" rd /q /s "build\" 
mkdir build

cls
ccpsx -O3 -Xo$80010000 -DPXFINDER -DDEBUG -Iextlib extlib\pxfinder\pxfinder.obj code\main.c -obuild\main.cpe,build\main.sym,build\mem.map
cd build
cpe2x main.cpe
cd ..
pause
