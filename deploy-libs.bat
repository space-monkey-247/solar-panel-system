@ECHO OFF

SETLOCAL
SET sourceDir=%CD%\src
echo sourceDir: "%sourceDir%"

rem c:\Users\Bbuzac\Documents\Arduino\libraries\solar-panel-library\
SET dest_dir=%userprofile%\Documents\Arduino\libraries
echo dest_dir: "%dest_dir%"

rem delete ubidots-mqtt-esp-master
IF EXIST "%dest_dir%" (
	echo Deleting "%dest_dir%\ubidots-mqtt-esp-master" dir. 
	DEL "%dest_dir%\ubidots-mqtt-esp-master" /Q /F /S
)

rem delete solar-panel-library
IF EXIST "%dest_dir%" (
	echo Deleting "%dest_dir%\solar-panel-library" dir. 
	DEL "%dest_dir%\solar-panel-library" /Q /F /S
)

xcopy "%sourceDir%" "%dest_dir%" /Y /F /E

echo/
echo Libs deployed succesfully
echo/


