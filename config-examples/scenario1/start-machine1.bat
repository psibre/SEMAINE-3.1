rem Which ActiveMQ to connect to:
set CMS_URL=tcp://192.168.1.1:61616

rem Locations of installed software:
rem %~dp0 is expanded pathname of the current script
set SEMAINEDIR=%~dp0%
set ACTIVEMQDIR="%SEMAINEDIR%apache-activemq-5.3.0"
set JAVADIR="%SEMAINEDIR%java"
set GRETADIR="%SEMAINEDIR%Greta"

rem ACTIVEMQ: start and wait for it to be started
if exist %ACTIVEMQDIR% (
  echo.Trying to start ActiveMQ from %ACTIVEMQDIR%...
  set SUNJMX=-Dcom.sun.management.jmxremote.port=1099 -Dcom.sun.management.jmxremote.authenticate=false -Dcom.sun.management.jmxremote.ssl=false
  start "activemq" /MIN /D %ACTIVEMQDIR%\bin activemq-admin.bat start
  :loop
  %GRETADIR%\utils\WAIT.exe 1
  for /f "delims=" %%a in ('netstat -an ^| findstr "61616"') do @set ACTIVEMQ_READY=%%a 
  if "%ACTIVEMQ_READY%"=="" (
    echo.Waiting for ActiveMQ...
    goto loop
  ) else (
    echo.ActiveMQ seems to be ready.
  )
)

rem Java components
if exist %JAVADIR% (
  echo.Starting Java components
  start "speech2face" /MIN /D "%SEMAINEDIR%" bin\semaine-speech2face.bat
)

rem GRETA components
if exist %GRETADIR% (
  echo.Starting Greta components
  start /MIN /D %GRETADIR% Active_ActionSelection.exe
  start /MIN /D %GRETADIR% Active_BehaviorPlanner.exe
  start /MIN /D %GRETADIR% Active_BehaviorPlanner.exe -p
  start /MIN /D %GRETADIR% Active_BehaviorRealizer.exe
  start /MIN /D %GRETADIR% Active_BehaviorRealizer.exe -p
  start /MIN /D %GRETADIR% Active_ListenerIntentPlanner.exe
  start /D %GRETADIR% Active_Player.exe
)


