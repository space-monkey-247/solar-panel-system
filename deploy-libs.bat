@ECHO OFF

SETLOCAL
SET sourceDir=%CD%\src
echo sourceDir: "%sourceDir%"

rem c:\Program Files\Arduino\libraries\
rem c:\Program Files (x86)\
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

IF EXIST "%dest_dir%" (
	xcopy "%sourceDir%" "%dest_dir%" /Y /F /E
)

echo/
echo Libs deployed succesfully
echo/


