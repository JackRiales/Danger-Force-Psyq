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
SET BMPDIR=bmp
SET TIMDIR=tim

REM Tim-to-header conversion
SET HEADERTOOL=bin2h
SET TIMHEADERFOLDER="imageh"

REM TODO(Jack): Sounds

REM Head to the meta folder
PUSHD ..\meta

REM Delete and remake the folders
IF EXIST %BMPDIR% RD /S /Q %BMPDIR%
IF EXIST %TIMDIR% RD /S /Q %TIMDIR%
IF EXIST %TIMHEADERFOLDER% RD /S /Q %TIMHEADERFOLDER%
MKDIR %BMPDIR%
MKDIR %TIMDIR%
MKDIR %TIMHEADERFOLDER%

REM Convert all PNG's to BMPs
REM Requires IMAGEMAGICK, any version should do so long as it's in your path
ECHO Converting PNG workfiles to BMPs...
FOR %%I IN (*.png) DO %IMAGEMAGICK% %%I %IMARGS% %BMPDIR%\%%~nI.bmp

REM Convert those BMPs to TIMs (redirecting annoying output)
REM NOTE!! This uses a modified version of BMP2TIM
REM That version can be found at this link: http://www.psxdev.net/forum/viewtopic.php?f=69&t=780
REM Replace your EXE file in psyq with his and this will work.
ECHO Converting Bitmaps to Playstation TIM files...
FOR %%I IN (%BMPDIR%\*.bmp) DO bmp2tim %%I %TIMDIR%\%%~nI.tim 24 -org=320,256 -mpink ON > nul

REM Search out and compile .TIM files into headers using bin2h
REM My version of bin2h is here: https://www.deadnode.org/sw/bin2h/
ECHO Converting TIM files to binary C headers...
FOR %%I IN (%TIMDIR%\*.tim) DO %HEADERTOOL% %%~nI < %%I > %TIMHEADERFOLDER%\%%~nI.h

REM Cleanup some stuff
ECHO Cleaning up...
RD /S /Q %BMPDIR%
RD /S /Q %TIMDIR%

REM Come back
POPD

REM Move the image headers to the code folder for inclusion in the program
ECHO Moving image headers to the code directory...
MOVE %META%\%TIMHEADERFOLDER% %CODE%
RD /S /Q %META%\%TIMHEADERFOLDER%

ECHO Compiled meta folder successfully.
pause
