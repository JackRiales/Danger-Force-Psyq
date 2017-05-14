@ECHO off
REM Meta compile script
REM Takes all the TIM and XA, XN files from the meta folder (asset folder)
REM and compiles them properly to what they need to be compiled as,
REM whether that be tim headers for images (.h) or otherwise.
REM
REM For now, only compiles TIMs into headers

SET META=..\meta
SET CODE=..\code
SET TIMDIR=tim

REM TODO(Jack): Sounds

REM Head to the meta folder
PUSHD ..\meta

REM Delete and remake the folders
IF EXIST %TIMDIR% RD /S /Q %TIMDIR%
MKDIR %TIMDIR%

REM Convert all images to TIMs (redirecting annoying output)
REM NOTE!! This uses a special img2tim program, found here: https://github.com/Lameguy64/img2tim
ECHO Converting Bitmaps to Playstation TIM files...
FOR %%I IN (*.png) DO img2tim -usealpha -bpp 16 %%I -o %TIMDIR%\%%~nI.tim > nul

REM Cleanup some stuff
ECHO Cleaning up...

REM Come back
POPD

REM Move the image headers to the code folder for inclusion in the program
ECHO Moving image headers to the code directory...
IF EXIST %CODE%\%TIMDIR% RD /S /Q %CODE%\%TIMDIR%
MOVE %META%\%TIMDIR% %CODE%

ECHO Compiled meta folder successfully.

pause
