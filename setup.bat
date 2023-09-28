set PROJECTPATH=%~dp0
set SOURCEDIR=%PROJECTPATH%FMIPlugin\Plugins\FMIKit\ThirdParty\fmikit\src\
set DESTDIR=%PROJECTPATH%FMIPlugin\Plugins\FMIKit\Source\FMIKit\ThirdParty\fmikit\src\
xcopy "%SOURCEDIR%" "%DESTDIR%" /y
pause