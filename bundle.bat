@ECHO OFF

SET dir=%~dp0
SET buildDir=%dir%build\
SET targetDir=%dir%temp\
SET output=%buildDir%edgesmooth.mzp
SET exclude_file=tmp_exclude.txt


ECHO Checking bundle prerequisites...
CALL :GetZip zip
dir "%zip%" > nul || goto :ZipNotFoundError

ECHO Checking build directory...
dir "%buildDir%" > nul || goto :buildNotFoundError


ECHO.
ECHO Removing old target files and directory...

::Remove target dir if it exists
IF EXIST %targetDir% (
  rmdir /Q /S %targetDir% || goto :error
  ECHO Removed %targetDir%
)

::Remove output file if it exists
IF EXIST %output% ( del /Q /S %output% || goto :error )


::Copy the installer dir to a temporary directory.
ECHO.
ECHO Copying installer to temporary directory...
ECHO ._ >> %exclude_file%
ECHO .DS_Store >> %exclude_file%
ECHO .mzp >> %exclude_file%

xcopy %dir%installer %targetDir% /e /q /EXCLUDE:%exclude_file% || goto :xcopyError

::Copy plugin to temporary directory.
ECHO.
ECHO Copying plugin to temporary directory...
xcopy %buildDir%* %targetDir%plugin\ /e /q /EXCLUDE:%exclude_file% || goto :xcopyError

del %exclude_file%

::Create package from target dir
ECHO.
ECHO Packing mzp...
"%zip%" a -tzip %output% %targetDir%* || goto :error



::Remove target dir
rmdir /Q /S %targetDir% || goto :error
ECHO Removed %targetDir%


ECHO Done.
goto :eof



:GetZip
SET KEY_NAME=HKEY_LOCAL_MACHINE\SOFTWARE\7-Zip
SET KEY_VALUE=Path

FOR /F "tokens=2*" %%A IN ('REG QUERY "%KEY_NAME%" /v %KEY_VALUE% 2^>nul') DO (
   set %~1=%%B7z.exe
)
goto :eof


:ZipNotFoundError
ECHO Could not find 7z.exe.
goto :error

:buildNotFoundError
ECHO Use build_all.bat or build_and_bundle.bat first.
goto :error

:xcopyError
del %exclude_file%
goto :error

:error
ECHO.
ECHO Bundling failed.
PAUSE
EXIT /B %ERRORLEVEL%