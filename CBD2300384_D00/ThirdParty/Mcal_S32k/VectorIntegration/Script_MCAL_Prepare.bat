@echo off
rem ###############################################################################
rem # File Name  : Script_MCAL_Prepare.bat                                        #
rem # Description: Batch file for Mcal to proceed necessary actions after         #
rem #              installation of the Mcal package                               #
rem #                                                                             #
rem #                                                                             #
rem #-----------------------------------------------------------------------------#
rem #               C O P Y R I G H T                                             #
rem #-----------------------------------------------------------------------------#
rem # Copyright (c) 2022 by Vector Informatik GmbH.  All rights reserved.         #
rem #                                                                             #
rem #-----------------------------------------------------------------------------#
rem #               A U T H O R   I D E N T I T Y                                 #
rem #-----------------------------------------------------------------------------#
rem # Initials     Name                      Company                              #
rem # --------     ---------------------     -------------------------------------#
rem # Refer to MIPconfig.xml                                                      #
rem #-----------------------------------------------------------------------------#
rem #               R E V I S I O N   H I S T O R Y                               #
rem #-----------------------------------------------------------------------------#
rem # Date         Version  Sign    Description                                   #
rem # ----------   -------  ------  ----------------------------------------------#
rem # Refer to MIPconfig.xml                                                      #
rem #-----------------------------------------------------------------------------#
rem ###############################################################################

title Script_MCAL_prepare.bat
echo.
echo ***************Script by Vector Informatik GmbH***************
echo.

set DERIVATIVE=
set MCAL=
set ID=
set PATH=

if "%1"==""           goto description
if "%1"=="--prepare"  goto prepare
if "%1"=="--undo"     goto undo
if "%1"=="--auto" (
  if "%2"=="" (
    echo ERROR: No derivative provided by option --auto, script will exit without any action!
	goto end
  )
  set DERIVATIVE=%2
  
  rem check if third parameter exists
  if not "%3"=="" (
   set MCAL=%3
  )
  
  goto prepare
)

if "%1"=="--replace" (
  if "%2"=="" (
    echo ERROR: Missing parameter. Please use -help for more information!
	goto end
  )
  set ID=%2
  
  if "%3"=="" (
    echo ERROR: Missing parameter. Please use -help for more information!
	goto end
  )
  set PATH=%3
  
  if "%4"=="" (
    echo ERROR: Missing parameter. Please use -help for more information!
	goto end
  )
  set DERIVATIVE=%4
  goto replace
)

if "%1"=="--copy" (
  if "%2"=="" (
    echo ERROR: Missing parameter. Please use -help for more information!
	goto end
  )
  set ID=%2
  
  if "%3"=="" (
    echo ERROR: Missing parameter. Please use -help for more information!
	goto end
  )
  set PATH=%3
  
  if "%4"=="" (
    echo ERROR: Missing parameter. Please use -help for more information!
	goto end
  )
  set DERIVATIVE=%4
  goto copy
)

if "%1"=="--ignore" (
  if "%2"=="" (
    echo ERROR: Missing parameter. Please use -help for more information!
	goto end
  )
  set ID=%2
  
  if "%3"=="" (
    echo ERROR: Missing parameter. Please use -help for more information!
	goto end
  )
  set DERIVATIVE=%3
  goto ignore
)

rem ELSE Description
:description
echo This File starts the so called 3rdPartyMcalIntegrationHelper which prepares the MCAL for further use.
echo.
echo Options:
echo    --auto:     Prepares the MCAL for usage without any user interaction needed. 
echo 		    Copies plugins, renames file duplicated, copies BSMWD files, ....
echo                Needs as second input parameter the derivative number (file fragment, see MIPconfig.xml).
echo    --undo:     Inverts the steps from previous integration.
echo    --prepare:  Opens the Helper Tool in GUI mode.
echo    --replace:  Replaces an MCAL package from the Supply folder with a new package specifying its path and id.
echo    --copy:     Copies an MCAL package into the Supply folder by specifying its path and id.
echo    --ignore:   Ignores an MCAL package from Supply folder by specifying its id.
goto end

:prepare
rem #####################################################################################################
rem # Description of command line options for 3rdPartyMcalIntegrationHelper:
rem #----------------------------------------------------------------------------------------------------
rem # MANDATORY
rem #   - NONE
rem #----------------------------------------------------------------------------------------------------
rem # OPTIONAL
rem #   -f:"<pathToConfigFile>"
rem #      Path to a valid configuration file which shall be used.
rem #      If not specified the default file (ThirdParty\Mcal_XYZ\VectorIntegration\MIPconfig.xml will be used.
rem #   -b:"<pathToThisFile>"
rem #      Complete path of this batch file (including file name)
rem #      Only needed if the tool shall manipulate this file e.g. by writing the variable MIH_EXITCODE (see below).
rem #   -mode:<Prepare|UndoPrepare|Remove>
rem #      Prepare:     Execute immediately all steps needed to use the MCAL without any user input.
rem #                   This options needs the parameter 'derivative' (see below) if derivative specific actions 
rem #                   must be executed (e.g. copying of BSMWD files)
rem #      UndoPrepare: Invert the steps done during 'prepare' immediately without any user input
rem #      Remove:      Remove the MCAL packages from the SIP immediately without any user input
rem #                   ATTENTION: This actions deletes the configured packages (within MIPconfig.xml) on hard drive!
rem #   -derivative:<FileFragement>
rem #      Use this parameter to specify the BSWMD file name fragment which identifies the derivative you want to use.
rem #      Hint: you may have a look in the file MIPconfig.xml (Bswmd->Derivatives) for possible values.
rem #####################################################################################################
if "%DERIVATIVE%"=="" (
call ..\..\McalIntegrationHelper\3rdPartyMcalIntegrationHelper.exe
) else (
	if "%MCAL%"=="" (
		call ..\..\McalIntegrationHelper\3rdPartyMcalIntegrationHelper.exe -b:"%~dpnx0" -mode:Prepare -derivative:%DERIVATIVE%
	) else (
		call ..\..\McalIntegrationHelper\3rdPartyMcalIntegrationHelper.exe -b:"%~dpnx0" -mode:Prepare -derivative:%DERIVATIVE% -mcal:%MCAL%
	)
)
goto end

:undo
call ..\..\McalIntegrationHelper\3rdPartyMcalIntegrationHelper.exe -b:"%~dpnx0" -mode:UndoPrepare
goto end

:replace
call ..\..\McalIntegrationHelper\3rdPartyMcalIntegrationHelper.exe -b:"%~dpnx0" -mode:Replace -id:%ID% -path:%PATH% -derivative:%DERIVATIVE%
goto end

:copy
call ..\..\McalIntegrationHelper\3rdPartyMcalIntegrationHelper.exe -b:"%~dpnx0" -mode:Copy -id:%ID% -path:%PATH% -derivative:%DERIVATIVE%
goto end

:ignore
call ..\..\McalIntegrationHelper\3rdPartyMcalIntegrationHelper.exe -b:"%~dpnx0" -mode:Ignore -id:%ID% -derivative:%DERIVATIVE%
goto end

rem   MIH_EXITCODE will be set by 3rdPartyMcalIntegrationHelper
rem   OK (default), CANCEL, ERROR
set MIH_EXITCODE=OK
:end

