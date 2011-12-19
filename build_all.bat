@ECHO OFF

SET Configs=(max2010 max2011 max2012)
SET Platforms=(win32 x64)
SET Project=plugin/EdgeSmooth.vcxproj


CALL :GetMsBuild MSBuild 4.0
dir "%MSBuild%" > nul || goto :Error "Could not find MSBuild.exe


FOR %%C IN %Configs% DO (
   FOR %%P IN %Platforms% DO (
      ECHO.
      ECHO.
      ECHO Building %%C ^| %%P
      %MSBuild% %Project% /nologo /v:m /p:Configuration=%%C /p:Platform=%%P || goto :error
   )
)



:: Usage: CALL :GetMSBuild variable_to_set dot_net_version
:GetMSBuild
SET KEY_NAME=HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\MSBuild\ToolsVersions\
SET KEY_VALUE=MSBuildToolsPath

FOR /F "tokens=2*" %%A IN ('REG QUERY "%KEY_NAME%%2" /v %KEY_VALUE% 2^>nul') DO (
   set %~1=%%BMSBuild.exe
)
goto :eof

:Error
ECHO %~1
PAUSE
EXIT /B %ERRORLEVEL%