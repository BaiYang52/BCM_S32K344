@echo off

rem ########################################################################################################################
rem     EXAMPLE CODE ONLY
rem     --------------------------------------------------------------------------------------------------------------------
rem     This Example Code is only intended for illustrating an example of a possible BSW integration and BSW configuration.
rem     The Example Code has not passed any quality control measures and may be incomplete. The Example Code is neither
rem     intended nor qualified for use in series production. The Example Code as well as any of its modifications and/or
rem     implementations must be tested with diligent care and must comply with all quality requirem ents which are necessary
rem     according to the state of the art before their use.
rem ########################################################################################################################

rem ---------------------------------------------------
rem 
rem   This script set the mandarory path to cygwin 
rem   executables.
rem 
rem ---------------------------------------------------

set PATH=%~dp0cygwin_root\cmd;%PATH%
mkdir %~dp0cygwin_root\tmp 2> NUL
set MAKESUPPORT_DIR=%~dp0
set MAKEFLAGS=-j%NUMBER_OF_PROCESSORS% -Otarget

echo.
echo     1) Added '%~dp0cygwin_root\cmd' to %%PATH%%.
echo     2) Created '/tmp' for cygwin in '%~dp0cygwin_root\tmp'
echo     3) Set MAKESUPPORT_DIR to %MAKESUPPORT_DIR%
echo     4) Set MAKEFLAGS to %MAKEFLAGS%
echo.

title "MakeSupport"

cmd /K
