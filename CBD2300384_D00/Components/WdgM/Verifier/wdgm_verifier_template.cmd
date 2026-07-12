@echo off

:: Set the internal directory
set INT_DIR=..\..\..\..\internal

:: Set the external directory
set EXT_DIR=..\..\..\..\external

:: Set the BSW directory
set BSW_DIR=%EXT_DIR%\BSW

:: Set the path to the xsltproc directory
set XSLTPROC_DIR=%EXT_DIR%\Misc\Wdg\xsltproc

:: Set the path to the Verifier test source directory
set VERIFIER_TESTS_DIR=.\Verifier_Tests

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::                                              User Section:                                                        ::
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:: Set the name of project
set PROJECT_NAME=

:: Set the path to the EcuC-File (*.ecuc.arxml)
set ECUC_FILE=

:: Set the GenData directory
set GENDATA_DIR=

:: Configure here all include paths
::
:: Can be extended as desired
:: Do not forget to extend the variable "ALL_INCLUDES" with the new path
::
set I_PATH_COMMON=-I %BSW_DIR%\Common
set I_PATH_DET=-I %BSW_DIR%\Det
set I_PATH_DEM=-I %BSW_DIR%\Dem
set I_PATH_MCU=-I %BSW_DIR%\Mcu
set I_PATH_OS=-I %BSW_DIR%\Os
set I_PATH_WDGIF=-I %BSW_DIR%\WdgIf 
set I_PATH_WDGM=-I %BSW_DIR%\WdgM 
set I_PATH_GENDATA=-I %GENDATA_DIR%
set I_PATH_GENERAL_RTE_GENDATA=-I %GENDATA_DIR%\Components
set I_PATH_VERIFIER_TESTS_INC=-I %VERIFIER_TESTS_DIR%\inc
set I_PATH_INCLUDE=-I %INT_DIR%\%PROJECT_NAME%\Appl\Include 
set I_PATH_WDGM_VERIFIER=-I .

:: Combine all include paths - do not forget to update next line if path were added
set ALL_INCLUDES=%I_PATH_COMMON% %I_PATH_DET% %I_PATH_DEM% %I_PATH_MCU% %I_PATH_OS% %I_PATH_WDGIF% %I_PATH_WDGM% %I_PATH_GENDATA% %I_PATH_GENERAL_RTE_GENDATA% %I_PATH_VERIFIER_TESTS_INC% %I_PATH_INCLUDE% %I_PATH_WDGM_VERIFIER%

:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::                                             End User Section                                                      ::
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:: Set the name of the verifier report
set VERIFIER_REPORT=.\Verifier_Report_%PROJECT_NAME%.txt

:: Set the Verifier test source directory
set VERIFIER_TESTS_SRC=%VERIFIER_TESTS_DIR%\src

:: Set the used OS generation, either "OsGen6" or "OsGen7"
set OS_GENERATION=

set VERIFIER=Verifier.exe
set VERIFIER_INPUT_FILE=wdgm_verifier_info

if exist %VERIFIER_INPUT_FILE%.c del %VERIFIER_INPUT_FILE%.c
if exist %VERIFIER_INPUT_FILE%.h del %VERIFIER_INPUT_FILE%.h
if exist %VERIFIER% del %VERIFIER%

%XSLTPROC_DIR%\xsltproc.exe verify_wdgm_source_%OS_GENERATION%.xsl %ECUC_FILE% >%VERIFIER_INPUT_FILE%.c
%XSLTPROC_DIR%\xsltproc.exe verify_wdgm_header.xsl %ECUC_FILE% >%VERIFIER_INPUT_FILE%.h

set VERIFIER_SOURCE_LIST=%VERIFIER_TESTS_SRC%\wdgm_verifier.c %VERIFIER_TESTS_SRC%\utilities.c %VERIFIER_TESTS_SRC%\edf_utilities.c %VERIFIER_TESTS_SRC%\block_a_tests.c %VERIFIER_TESTS_SRC%\block_b_tests.c %VERIFIER_TESTS_SRC%\block_c_tests.c

gcc -Wall %VERIFIER_INPUT_FILE%.c %VERIFIER_SOURCE_LIST% %GENDATA_DIR%\WdgM_Cfg.c %ALL_INCLUDES% -o %VERIFIER%

IF %ERRORLEVEL% NEQ 0 GOTO Error

if exist %VERIFIER% %VERIFIER% >%VERIFIER_REPORT%

if exist %VERIFIER_REPORT% start notepad++ %VERIFIER_REPORT%

echo Verifier.exe finished successfully.
echo ----------------------------------------------
echo %VERIFIER_REPORT% should be displayed now.
echo ----------------------------------------------

::if not "%BATCH_MODE%"=="true" pause

GOTO End
:Error
 echo Error during Execution
 pause 
:End