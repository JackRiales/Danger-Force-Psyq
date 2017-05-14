@ECHO off
REM Meta compile script
REM Takes all the TIM and XA, XN files from the meta folder (asset folder)
REM and compiles them properly to what they need to be compiled as,
REM whether that be tim headers for images (.h) or otherwise.
REM
REM For now, only compiles TIMs into headers

SET META=..\meta
SET CODE=..\code

REM PNG-to-bmp conversion
REM and bmp-to-tim conversion
SET BGCOLOR="#FF00FF"
SET IMAGEMAGICK=convert
SET IMARGS=-background %BGCOLOR% -flatten
SET BMP2TIM=bmp2tim
SET TIMDIR=tim

REM Tim-to-header conversion
SET HEADERTOOL=bin2h
SET TIMHEADERFOLDER="imageh"

REM TODO(Jack): Sounds

REM Head to the meta folder
PUSHD ..\meta

REM Delete and remake the folders
IF EXIST %TIMDIR% RD /S /Q %TIMDIR%
IF EXIST %TIMHEADERFOLDER% RD /S /Q %TIMHEADERFOLDER%
MKDIR %TIMDIR%
MKDIR %TIMHEADERFOLDER%

REM Convert all images to TIMs (redirecting annoying output)
REM NOTE!! This uses a special img2tim program, found here: https://github.com/Lameguy64/img2tim
ECHO Converting Bitmaps to Playstation TIM files...
FOR %%I IN (*.png) DO img2tim -usealpha -bpp 16 %%I -o %TIMDIR%\%%~nI.tim > nul

REM Search out and compile .TIM files into headers using bin2h
REM My version of bin2h is here: https://www.deadnode.org/sw/bin2h/
ECHO Converting TIM files to binary C headers...
FOR %%I IN (%TIMDIR%\*.tim) DO %HEADERTOOL% %%~nI < %%I > %TIMHEADERFOLDER%\%%~nI.h

REM Cleanup some stuff
ECHO Cleaning up...
REM RD /S /Q %TIMDIR%

REM Come back
POPD

REM Move the image headers to the code folder for inclusion in the program
ECHO Moving image headers to the code directory...
IF EXIST %CODE%\%TIMHEADERFOLDER% RD /S /Q %CODE%\%TIMHEADERFOLDER%
MOVE %META%\%TIMHEADERFOLDER% %CODE%

ECHO Compiled meta folder successfully.

CALL createassetheader.bat
ECHO Wrote asset header

pause
