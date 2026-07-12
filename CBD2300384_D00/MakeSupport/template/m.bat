@echo off

rem *****************************************************************************
rem * This is a template batch file. The path might be adapted
rem *****************************************************************************

rem *****************************************************************************
rem * MAKESUPPORT_DIR has to be set to MakeSupport root directory
rem * There MUST NOT be any blanks between the '=' sign and the path string
rem * Example: 
rem *     set MAKESUPPORT_DIR=..\..\MakeSupport
rem *****************************************************************************

set MAKESUPPORT_DIR=$$MAKESUPPORT_DIR

rem Check if SWCP was used to set MAKESUPPORT_DIR
rem   If not, its value is set to ""
set VAR_NAME=MAKESUPPORT_DIR
if %MAKESUPPORT_DIR% == $$%VAR_NAME% set MAKESUPPORT_DIR=

rem *****************************************************************************
rem * DO NOT EDIT ANYTHING BELOW THIS
rem *
rem * %~dp0 is the location where this file is started from.
rem *****************************************************************************

if "%MAKESUPPORT_DIR% "==" "   goto ErrorNotSet
if not exist %MAKESUPPORT_DIR% goto ErrorWrongPath

set PATH_OLD=%PATH%
call %~dp0%MAKESUPPORT_DIR%\set_cygwin_path.bat

rem Replace GNU make help with MakeSupport help when 'm -h' is executed
if ["%~1"]==["-h"] (
	make help
	goto Skip
)

rem *****************************************************************************
rem * Run make and store its return value afterwards.
rem * The return value is passed to the caller of m.bat at the end of the file.
rem *****************************************************************************
make -Otarget %*

:Skip

SET GNU_MAKE_RETURN_CODE=%ERRORLEVEL%
set PATH=%PATH_OLD%
set PATH_OLD=

if "%GNU_MAKE_RETURN_CODE%" NEQ "0" goto ErrorGnuMake
set RETURN_CODE=0

goto End

:ErrorNotSet
set RETURN_CODE=1
echo ********************************************************************************
echo Error: MAKESUPPORT_DIR has to be set to MakeSupport\cygwin_root\cmd directory!
echo        Please correct setting in this batch file and try again!
echo ********************************************************************************
goto End

:ErrorWrongPath
set RETURN_CODE=1
echo ********************************************************************************
echo Error: %MAKESUPPORT_DIR% does not exist
echo        Please correct setting in this batch file and try again!
echo ********************************************************************************
goto End

:ErrorGnuMake
set RETURN_CODE=1
echo ********************************************************************************
echo Error: GNU make returned code "%GNU_MAKE_RETURN_CODE%"
echo ********************************************************************************

:End
set MAKESUPPORT_DIR=
exit /b %RETURN_CODE%
