set PROJECTPATH=%~dp0
set SOURCEDIR=%PROJECTPATH%ThirdParty\fmikit\src\
set DESTDIR=%PROJECTPATH%Source\UEFMI\ThirdParty\fmikit\src\
xcopy "%SOURCEDIR%" "%DESTDIR%" /y