@echo off

rem ########################################################################################################################
rem # File Name  : set_makesupport_dir.bat
rem # Description: This script sets the variable MAKESUPPORT_DIR to location where this file resists.
rem #              This file can be sourced from anywhere.
rem #                                                                                                                     
rem #              There exists also a bash variant set_makesupport_dir.sh
rem #                                                                                                                     
rem #              This file MUST NOT BE CHANGED on project integration
rem #                                                                                                                     
rem # Project    : Vector PES Buildenvironment
rem # Module     : GlobalMake 4.0
rem #                                                                                                                     
rem #-----------------------------------------------------------------------------------------------------------------------
rem # COPYRIGHT
rem #-----------------------------------------------------------------------------------------------------------------------
rem # Copyright (c) 2018 by Vector Informatik GmbH.                                                  All rights reserved. 
rem #                                                                                                                     
rem #-----------------------------------------------------------------------------------------------------------------------
rem # AUTHOR IDENTITY
rem #-----------------------------------------------------------------------------------------------------------------------
rem # Name                          Initials      Company
rem # ----------------------------  ------------  --------------------------------------------------------------------------
rem # Clemens von Mann              vircvo        Vector Informatik GmbH
rem # Claudia Buhl                  vircbl        Vector Informatik GmbH
rem #-----------------------------------------------------------------------------------------------------------------------
rem # REVISION HISTORY                                                                                                    
rem #-----------------------------------------------------------------------------------------------------------------------
rem # Version   Date        Author  Description
rem # --------  ----------  ------  ----------------------------------------------------------------------------------------
rem # 04.00.00  2018-09-26  vircvo  Initial Release
rem # 04.00.02  2018-10-15  vircbl  Modified header
rem ########################################################################################################################

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
set MAKESUPPORT_DIR=%~dp0
echo set MAKESUPPORT_DIR=%MAKESUPPORT_DIR%
echo.
