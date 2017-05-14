@ECHO off

REM This takes in the names of the asset header files located in imageh
REM and writes a header file including all of them.

SET CODEDIR=..\code
SET IMAGEDIR=imageh
SET HEADERNAME=df_images.h

PUSHD %CODEDIR%
TYPE nul > %HEADERNAME%
ECHO #ifndef %HEADERNAME% >> %HEADERNAME%
ECHO #define %HEADERNAME% >> %HEADERNAME%
FOR %%I IN (%IMAGEDIR%\*.h) DO ECHO #include "%%I" >> %HEADERNAME%
ECHO #endif >> %HEADERNAME%
POPD
