@echo off

rem ---------------------------------------------------
rem 
rem   This script sets the mandatory path to cygwin 
rem   executables.
rem 
rem   This file should be called inside a cmd.exe shell.
rem 
rem   Use
rem       <path>/set_cygwin_path.bat
rem 
rem ---------------------------------------------------

rem ########################################################################################################################
rem     EXAMPLE CODE ONLY
rem     --------------------------------------------------------------------------------------------------------------------
rem     This Example Code is only intended for illustrating an example of a possible BSW integration and BSW configuration.
rem     The Example Code has not passed any quality control measures and may be incomplete. The Example Code is neither
rem     intended nor qualified for use in series production. The Example Code as well as any of its modifications and/or
rem     implementations must be tested with diligent care and must comply with all quality requirem ents which are necessary
rem     according to the state of the art before their use.
rem ########################################################################################################################

echo.
rem Add '~dp0cygwin_root\cmd' to PATH
set PATH=%~dp0cygwin_root\cmd;%PATH%
rem Create '/tmp' for cygwin in '~dp0cygwin_root\tmp'
mkdir %~dp0cygwin_root\tmp 2> NUL
