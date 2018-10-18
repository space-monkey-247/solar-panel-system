@ECHO OFF

SETLOCAL
SET sourceDir=%CD%\src
echo sourceDir: "%sourceDir%"

rem c:\Users\Bbuzac\Documents\Arduino\libraries\solar-panel-master\
SET dest_dir=%userprofile%\Documents\Arduino\libraries
echo dest_dir: "%dest_dir%"

rem delete ubidots-mqtt-esp-master
IF EXIST "%dest_dir%" (
	echo Deleting "%dest_dir%\ubidots-mqtt-esp-master" dir. 
	DEL "%dest_dir%\ubidots-mqtt-esp-master" /Q /F /S
)

rem delete solar-panel-master
IF EXIST "%dest_dir%" (
	echo Deleting "%dest_dir%\solar-panel-master" dir. 
	DEL "%dest_dir%\solar-panel-master" /Q /F /S
)

xcopy "%sourceDir%" "%dest_dir%" /Y /F /E


rem c:\Program Files\Arduino\libraries\
SET dest_dir=c:\Program Files\Arduino\libraries
echo dest_dir: "%dest_dir%"

rem delete ubidots-mqtt-esp-master
IF EXIST "%dest_dir%" (
	echo Deleting "%dest_dir%\ubidots-mqtt-esp-master" dir. 
	DEL "%dest_dir%\ubidots-mqtt-esp-master" /Q /F /S
)

rem delete solar-panel-master
IF EXIST "%dest_dir%" (
	echo Deleting "%dest_dir%\solar-panel-master" dir. 
	DEL "%dest_dir%\solar-panel-master" /Q /F /S
)

xcopy "%sourceDir%" "%dest_dir%" /Y /F /E

echo/
echo Libs deployed succesfully
echo/


