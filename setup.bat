set PROJECTPATH=%~dp0
set SOURCEDIR=%PROJECTPATH%DemoUEFMIPlugin\Plugins\UEFMI\ThirdParty\fmikit\src\
set DESTDIR=%PROJECTPATH%DemoUEFMIPlugin\Plugins\UEFMI\Source\UEFMI\ThirdParty\fmikit\src\
xcopy "%SOURCEDIR% %DESTDIR%" /y