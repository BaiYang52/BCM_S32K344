/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : LPSPI
*   Dependencies         : 
*
*   Autosar Version      : 4.4.0
*   Autosar Revision     : ASR_REL_4_4_REV_0000
*   Autosar Conf.Variant :
*   SW Version           : 2.0.1
*   Build Version        : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
*   (c) Copyright 2020 - 2022 NXP Semiconductors
*   All Rights Reserved.
*
*   NXP Confidential. This software is owned or controlled by NXP and may only be
*   used strictly in accordance with the applicable license terms. By expressly
*   accepting such terms or by downloading, installing, activating and/or otherwise
*   using the software, you are agreeing that you have read, and that you agree to
*   comply with and are bound by, such license terms. If you do not agree to be
*   bound by the applicable license terms, then you may not retain, install,
*   activate or otherwise use the software.
==================================================================================================*/

/**   
*   @file     Flexio_Spi_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c
*   @version 2.0.1
*
*   @brief   AUTOSAR Spi - Post-Build(PB) configuration file code template.
*   @details Code template for Post-Build(PB) configuration file generation.
*
*   @addtogroup FLEXIO_DRIVER_CONFIGURATION  Flexio_Spi Driver Configuration
*   @{
*/

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Flexio_Spi_Ip.h"

#if (FLEXIO_SPI_IP_ENABLE == STD_ON)
#if (FLEXIO_SPI_IP_DMA_USED == STD_ON)
#include "Dma_Ip.h"
#endif
#endif

/*==================================================================================================
*                                    SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_PBCFG_C                        43
#define FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_PBCFG_C         4
#define FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_PBCFG_C         4
#define FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_PBCFG_C      0
#define FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_PBCFG_C                 2
#define FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_PBCFG_C                 0
#define FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_PBCFG_C                 1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if Flexio_Spi_Ip_[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg source file and Spi configuration header file are of the same vendor */
#if (FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_PBCFG_C != FLEXIO_SPI_IP_VENDOR_ID)
    #error "Flexio_Spi_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Flexio_Spi_Ip.h have different vendor IDs"
#endif

#if ((FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_PBCFG_C    != FLEXIO_SPI_IP_AR_RELEASE_MAJOR_VERSION) || \
     (FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_PBCFG_C    != FLEXIO_SPI_IP_AR_RELEASE_MINOR_VERSION) || \
     (FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_PBCFG_C != FLEXIO_SPI_IP_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Flexio_Spi_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Flexio_Spi_Ip.h are different"
#endif
/* Check if Flexio_Spi_Ip_[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg header file and Spi configuration header file are of the same software version */
#if ((FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_PBCFG_C != FLEXIO_SPI_IP_SW_MAJOR_VERSION) || \
     (FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_PBCFG_C != FLEXIO_SPI_IP_SW_MINOR_VERSION) || \
     (FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_PBCFG_C != FLEXIO_SPI_IP_SW_PATCH_VERSION))
    #error "Software Version Numbers of Flexio_Spi_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Flexio_Spi_Ip.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if (FLEXIO_SPI_IP_ENABLE == STD_ON)
    #if (FLEXIO_SPI_IP_DMA_USED == STD_ON)
        #if ((FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_PBCFG_C != DMA_IP_AR_RELEASE_MAJOR_VERSION) || \
             (FLEXIO_SPI_IP[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_PBCFG_C != DMA_IP_AR_RELEASE_MINOR_VERSION) \
            )
            #error "AutoSar Version Numbers of Flexio_Spi_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.h and Dma_Ip.h are different"
        #endif
    #endif /* (FLEXIO_SPI_IP_DMA_USED == STD_ON) */
    #endif /* (FLEXIO_SPI_IP_ENABLE == STD_ON) */
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */


/*==================================================================================================
*                                        LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
                                       LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/
[!MACRO "FlexioSpiBaudrateCalculator"!][!//
[!NOCODE!]
    [!VAR "FlexioBusClock" = "num:i($BusClockNormal)"!]
    [!IF "$DualClockEnabled = 'true'"!][!//
        [!VAR "FlexioBusClockAlternate" = "num:i($BusClockAlternate)"!]
    [!ENDIF!] 
    [!VAR "Timdec" = "0"!]
    [!VAR "TimdecAlternate" = "0"!]
    [!VAR "ActualBaudrate" = "0"!]
    [!VAR "ActualBaudrateAlternate" = "0"!]
    [!VAR "ErrorValue" = "1000000"!]
    [!VAR "ErrorValueAlternate" = "1000000"!]
    [!VAR "DivBest" = "255"!]
    [!VAR "RatioBest" = "num:i(ecu:list('Spi.SpiFlexioClkDivRatio.List')[1])"!]
    [!FOR "RatioIndex" = "1" TO "count(ecu:list('Spi.SpiFlexioClkDivRatio.List'))"!]
        [!VAR "CurrentRatioValue" = "num:i(ecu:list('Spi.SpiFlexioClkDivRatio.List')[num:i($RatioIndex)])"!]
        [!FOR "DivCount" = "0" TO "255"!]
            [!VAR "CurrentBaudValue" = "$FlexioBusClock div ($CurrentRatioValue * 2 * ($DivCount + 1))"!]
            [!IF "$CurrentBaudValue < SpiBaudrate"!]
                [!VAR "CurrentErrorValue" = "SpiBaudrate - $CurrentBaudValue"!]
            [!ELSE!]
                [!VAR "CurrentErrorValue" = "$CurrentBaudValue - SpiBaudrate"!]
            [!ENDIF!]
            [!IF "$CurrentErrorValue < $ErrorValue"!]
                [!VAR "ErrorValue" = "$CurrentErrorValue"!]
                [!VAR "DivBest" = "$DivCount"!]
                [!VAR "RatioBest" = "$CurrentRatioValue"!]
                [!VAR "ActualBaudrate" = "$CurrentBaudValue"!]
            [!ENDIF!]
            [!IF "$DualClockEnabled = 'true'"!][!//
                [!VAR "CurrentBaudValueAlternate" = "$FlexioBusClockAlternate div ($CurrentRatioValue * 2 * ($DivCount + 1))"!]
                [!IF "$CurrentBaudValueAlternate < SpiBaudrate"!]
                    [!VAR "CurrentErrorValueAlternate" = "SpiBaudrate - $CurrentBaudValueAlternate"!]
                [!ELSE!]
                    [!VAR "CurrentErrorValueAlternate" = "$CurrentBaudValueAlternate - SpiBaudrate"!]
                [!ENDIF!]
                [!IF "$CurrentErrorValueAlternate < $ErrorValueAlternate"!]
                    [!VAR "ErrorValueAlternate" = "$CurrentErrorValueAlternate"!]
                    [!VAR "DivBestAlternate" = "$DivCount"!]
                    [!VAR "RatioBestAlternate" = "$CurrentRatioValue"!]
                    [!VAR "ActualBaudrateAlternate" = "$CurrentBaudValueAlternate"!]
                [!ENDIF!]
            [!ENDIF!]
        [!ENDFOR!]
    [!ENDFOR!]
    
    [!VAR "TIMCMPL_NORMAL" = "($FlexioBusClock div $RatioBest div 2 div SpiBaudrate) - 1"!]
    [!IF "$TIMCMPL_NORMAL > 255"!]
        [!ERROR!] Spi baudrate divider = [!"$TIMCMPL_NORMAL"!], it should be <= 255, please select a lower MCU clock for SpiPhyUnitClockRef node.[!ENDERROR!]
    [!ENDIF!]

    [!IF "$DualClockEnabled = 'true'"!]
        [!VAR "TIMCMPL_ALTERNATE" = "($FlexioBusClockAlternate div $RatioBestAlternate div 2 div SpiBaudrate) - 1"!]
        [!IF "$TIMCMPL_ALTERNATE > 255"!]
            [!ERROR!] Spi baudrate divider = [!"$TIMCMPL_ALTERNATE"!], it should be <= 255, please select a lower MCU clock for SpiPhyUnitAlternateClockRef node.[!ENDERROR!]
        [!ENDIF!]
    [!ENDIF!]    


    [!IF "$RatioBest = num:i(256)"!]
        [!VAR "Timdec" = "5"!]
    [!ELSEIF "$RatioBest = num:i(16)"!]
        [!VAR "Timdec" = "4"!]
    [!ENDIF!]
    
    [!IF "$DualClockEnabled = 'true'"!][!//
        [!IF "$RatioBestAlternate = num:i(256)"!]
            [!VAR "TimdecAlternate" = "5"!]
        [!ELSEIF "$RatioBestAlternate = num:i(16)"!]
            [!VAR "TimdecAlternate" = "4"!]
        [!ENDIF!]
    [!ENDIF!]
[!ENDNOCODE!]
[!ENDMACRO!][!//

[!NOCODE!][!//

[!MACRO "GenerateExternalDeviceInfoMaster"!]
    [!IF "SpiCsContinous = 'TRUE'"!]
        [!VAR "CS_Continuons" = "1"!]
    [!ELSE!] 
        [!VAR "CS_Continuons" = "0"!]
    [!ENDIF!]

  [!IF "SpiDataShiftEdge = 'LEADING'"!][!//
  [!VAR "CPHA" = "1"!]
  [!ELSE!]
  [!VAR "CPHA" = "0"!]
  [!ENDIF!][!// "SpiDataShiftEdge     = 'LEADING'"

  [!IF "SpiShiftClockIdleLevel = 'HIGH'"!][!//
  [!VAR "CPOL" = "1"!]
  [!ELSE!]
  [!VAR "CPOL" = "0"!]
  [!ENDIF!][!// "SpiShiftClockIdleLevel     = 'HIGH'"

  [!IF "SpiCsPolarity = 'HIGH'"!][!//
  [!VAR "CsPolarity" = "1"!]
  [!ELSE!]
  [!VAR "CsPolarity" = "0"!]
  [!ENDIF!][!// "SpiCsPolarity     = 'HIGH'"
[!ENDMACRO!][!//

[!MACRO "GenerateExternalDeviceInfoSlave"!] 
  [!IF "SpiCsContinous = 'TRUE'"!]
      [!VAR "CS_Continuons" = "1"!]
  [!ELSE!] 
      [!VAR "CS_Continuons" = "0"!]
  [!ENDIF!]

  [!IF "SpiDataShiftEdge = 'LEADING'"!][!//
  [!VAR "CPHA" = "1"!]
  [!ELSE!]
  [!VAR "CPHA" = "0"!]
  [!ENDIF!][!// "SpiDataShiftEdge     = 'LEADING'"

  [!IF "SpiShiftClockIdleLevel = 'HIGH'"!][!//
  [!VAR "CPOL" = "1"!]
  [!ELSE!]
  [!VAR "CPOL" = "0"!]
  [!ENDIF!][!// "SpiShiftClockIdleLevel     = 'HIGH'"

  [!IF "SpiCsPolarity = 'HIGH'"!][!//
  [!VAR "CsPolarity" = "1"!]
  [!ELSE!]
  [!VAR "CsPolarity" = "0"!]
  [!ENDIF!][!// "SpiCsPolarity     = 'HIGH'"
[!ENDMACRO!][!//

[!MACRO "Spi_CheckDualClockSupport"!]
    [!VAR "DualClockSupport"= "'false'"!]
    [!LOOP "SpiGeneral/SpiPhyUnit/*"!][!//
        [!IF "(node:exists(SpiPhyUnitAlternateClockRef))"!][!//
            [!VAR "DualClockSupport"= "'true'"!]
        [!ENDIF!][!//
    [!ENDLOOP!][!//

    [!VAR "DualClockEnabled" = "'false'"!][!//
[!ENDMACRO!][!//

[!CALL "Spi_CheckDualClockSupport"!]

[!ENDNOCODE!][!//

/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/
#if (FLEXIO_SPI_IP_ENABLE == STD_ON)
[!NOCODE!][!//
[!VAR "hwUnitIndexFlexiospi"= "0"!]
[!LOOP "SpiGeneral/SpiPhyUnit/*"!]
[!IF "contains(node:value(SpiPhyUnitMapping),'FLEXIO_SPI')"!]
[!VAR "hwUnitIndexFlexiospi"= "$hwUnitIndexFlexiospi + 1"!]
[!ENDIF!]
[!ENDLOOP!]
[!IF "num:i($hwUnitIndexFlexiospi) != 0"!]
    [!CODE!]
#if (FLEXIO_SPI_IP_DMA_USED == STD_ON)
    #define SPI_START_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#else
    #define SPI_START_SEC_VAR_INIT_UNSPECIFIED
#endif
#include "Spi_MemMap.h"
[!ENDCODE!]
[!NOCODE!][!//
[!VAR "NumberOfExternalDeviceFlexio" = "0"!][!//
[!VAR "spiPath" = "node:path(.)"!][!//
[!LOOP "SpiDriver/SpiExternalDevice/*"!][!//
    [!VAR "varSpiHwUnit"="string(substring-after(node:value(SpiHwUnit),'CSIB')+1)"!][!//
    [!IF "contains((substring-before((node:value(concat($spiPath,'/SpiGeneral/SpiPhyUnit/*[',$varSpiHwUnit,']/SpiPhyUnitMapping'))),'_')),'FLEXIO')"!]
        [!VAR "NumberOfExternalDeviceFlexio"= "$NumberOfExternalDeviceFlexio + 1"!]
    [!ENDIF!]
[!ENDLOOP!]
[!IF "num:i($NumberOfExternalDeviceFlexio) != 0"!]
[!CODE!][!//
/* Spi_Ip_DeviceParamsCfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] Device Attribute Configuration of Spi*/
static Flexio_Spi_Ip_DeviceParamsType Flexio_Spi_Ip_DeviceParamsCfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($NumberOfExternalDeviceFlexio)"!]U] =
{
[!ENDCODE!][!//
    [!VAR "ExIndex"="1"!]
    [!LOOP "SpiDriver/SpiExternalDevice/*"!][!//
        [!VAR "varSpiHwUnit"="string(substring-after(node:value(SpiHwUnit),'CSIB')+1)"!][!//
        [!IF "contains((substring-before((node:value(concat($spiPath,'/SpiGeneral/SpiPhyUnit/*[',$varSpiHwUnit,']/SpiPhyUnitMapping'))),'_')),'FLEXIO')"!]
            [!CODE!]
    {
        (uint8)8U, /* Frame size - dummy value */
        (boolean)TRUE, /* Lsb - dummy value */
        (uint32)1U  /* Default Data - dummy value */
    }[!IF "$ExIndex < $NumberOfExternalDeviceFlexio"!],[!ENDIF!][!//
            [!ENDCODE!][!//
            [!VAR "ExIndex"="$ExIndex + 1"!][!//
        [!ENDIF!]
    [!ENDLOOP!][!//
[!CODE!]
};
[!ENDCODE!][!//
[!ENDIF!]
[!ENDNOCODE!][!//
[!CODE!]
#if (FLEXIO_SPI_IP_DMA_USED == STD_ON)
    #define SPI_STOP_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#else
    #define SPI_STOP_SEC_VAR_INIT_UNSPECIFIED
#endif
#include "Spi_MemMap.h"
[!ENDCODE!]
[!ENDIF!][!// num:i($hwUnitIndexFlexiospi) != 0
[!ENDNOCODE!][!//

#define SPI_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Spi_MemMap.h"
[!NOCODE!][!//
[!VAR "ExIndex"="0"!]
[!VAR "spiPath" = "node:path(.)"!][!//
[!LOOP "SpiDriver/SpiExternalDevice/*"!]
[!VAR "varSpiHwUnit"="string(substring-after(node:value(SpiHwUnit),'CSIB')+1)"!]
[!IF "contains((substring-before((node:value(concat($spiPath,'/SpiGeneral/SpiPhyUnit/*[',$varSpiHwUnit,']/SpiPhyUnitMapping'))),'_')),'FLEXIO')"!]
[!CODE!][!//
/* Flexio_Spi_Ip_DeviceAttributes[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] Device Attribute Configuration of Spi*/
const Flexio_Spi_Ip_ExternalDeviceType Flexio_Spi_Ip_DeviceAttributes_[!"@name"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
[!ENDCODE!]
[!IF "node:value(../../../SpiGeneral/SpiPhyUnit/*[number($varSpiHwUnit)]/SpiPhyUnitMode) = 'SPI_MASTER'"!]
    [!SELECT "node:ref(../../../SpiGeneral/SpiPhyUnit/*[number($varSpiHwUnit)]/SpiPhyUnitClockRef)"!][!//
        [!VAR "SpiPeriphFrequency" = "McuClockReferencePointFrequency"!][!//
    [!ENDSELECT!][!//
    [!VAR "BusClockNormal" = "string($SpiPeriphFrequency)"!][!//
    [!IF "(node:exists(../../../SpiGeneral/SpiPhyUnit/*[number($varSpiHwUnit)]/SpiPhyUnitAlternateClockRef))"!][!//
        [!VAR "DualClockEnabled" = "'true'"!][!//
        [!SELECT "node:ref(../../../SpiGeneral/SpiPhyUnit/*[number($varSpiHwUnit)]/SpiPhyUnitAlternateClockRef)"!][!//
            [!VAR "SpiPeriphFrequency" = "McuClockReferencePointFrequency"!][!//
        [!ENDSELECT!][!//
        [!VAR "BusClockAlternate" = "string($SpiPeriphFrequency)"!][!//
    [!ELSE!]
        [!VAR "DualClockEnabled" = "'false'"!][!//
    [!ENDIF!][!//
[!CALL "GenerateExternalDeviceInfoMaster"!]
[!CALL "FlexioSpiBaudrateCalculator"!]
[!CODE!]
   [!"num:i(substring-after(node:value(../../../SpiGeneral/SpiPhyUnit/*[number($varSpiHwUnit)]/SpiPhyUnitMapping),'SPI_'))"!]U,  /* Instance */
   (uint8)FLEXIO_SPI_IP_CPOL_[!"SpiShiftClockIdleLevel"!]_U8,
   (uint8)FLEXIO_SPI_IP_CPHA_[!"SpiDataShiftEdge"!]_U8,

[!ENDCODE!]
    [!IF "$DualClockSupport = 'true'"!][!//
[!CODE!][!//
    {
[!ENDCODE!][!//
    [!ENDIF!][!//
[!CODE!][!//
        (uint32)[!"num:inttohex($DivBest)"!]u, /* Normal Mode */
[!ENDCODE!][!//
    [!IF "$DualClockSupport = 'true'"!][!//
        [!IF "$DualClockEnabled = 'true'"!][!//
[!CODE!][!//
        (uint32)[!"num:inttohex($DivBestAlternate)"!]u /* Alternate Mode*/
[!ENDCODE!][!//
        [!ELSE!][!//
[!CODE!][!//
        (uint32)[!"num:inttohex($DivBest)"!]u  /* Normal Mode */
[!ENDCODE!][!//
        [!ENDIF!][!//
[!CODE!][!//
    },
[!ENDCODE!][!//
    [!ENDIF!][!//

    [!IF "$DualClockSupport = 'true'"!][!//
[!CODE!][!//
    {
[!ENDCODE!][!//
    [!ENDIF!][!//
[!CODE!][!//
        FLEXIO_SPI_IP_TIMCFG_TIMDEC([!"num:i($Timdec)"!]), /* Normal Mode */
[!ENDCODE!][!//
    [!IF "$DualClockSupport = 'true'"!][!//
        [!IF "$DualClockEnabled = 'true'"!][!//
[!CODE!][!//
        FLEXIO_SPI_IP_TIMCFG_TIMDEC([!"num:i($TimdecAlternate)"!]) /* Alternate Mode*/
[!ENDCODE!][!//
        [!ELSE!][!//
[!CODE!][!//
        FLEXIO_SPI_IP_TIMCFG_TIMDEC([!"num:i($Timdec)"!])  /* Normal Mode */
[!ENDCODE!][!//
        [!ENDIF!][!//
[!CODE!][!//
    },
[!ENDCODE!][!//
    [!ENDIF!][!//
[!VAR "CSIB_number"="string(substring-after(node:value(SpiHwUnit),'CSIB')+1)"!]
[!SELECT "(../../../SpiGeneral/SpiPhyUnit/*[num:i($CSIB_number)])"!]
[!CODE!]
   /* Shifter control (SHIFTCTL) for TX */
   FLEXIO_SPI_IP_SHIFTCTL_TIMSEL([!"substring-after(node:value(node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclChannelId),'CHANNEL_')"!]) /* Select TIMER */
 | FLEXIO_SPI_IP_SHIFTCTL_PINSEL([!"substring-after(node:value(node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclPinId),'PIN_')"!]) /* Select PIN */
 | FLEXIO_SPI_IP_SHIFTCTL_PINCFG(3) /* Set is output pin */
 | FLEXIO_SPI_IP_SHIFTCTL_SMOD(2) /* Transmit mode */
 | [!IF "num:i($CPHA) = '1'"!]FLEXIO_SPI_IP_SHIFTCTL_TIMPOL(0)[!ELSE!]FLEXIO_SPI_IP_SHIFTCTL_TIMPOL(1)[!ENDIF!] /* If CPHA = 0, set "Trigger active low" */

,[!IF "num:i($CPHA) = '1'"!]FLEXIO_SPI_IP_SHIFTCFG_SSTOP(2)|FLEXIO_SPI_IP_SHIFTCFG_SSTART(1)[!ELSE!]FLEXIO_SPI_IP_SHIFTCFG_SSTOP(0)|FLEXIO_SPI_IP_SHIFTCFG_SSTART(0)[!ENDIF!]
   ,FLEXIO_SPI_IP_SHIFTCTL_TIMSEL([!"substring-after(node:value(node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclChannelId),'CHANNEL_')"!]) /* Select TIMER */
 | FLEXIO_SPI_IP_SHIFTCTL_PINSEL([!"substring-after(node:value(node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclPinId),'PIN_')"!]) /* Select PIN */
 | FLEXIO_SPI_IP_SHIFTCTL_PINCFG(0) /* Set is input pin */
 | FLEXIO_SPI_IP_SHIFTCTL_SMOD(1) /* Receive mode */
 | [!IF "num:i($CPHA) = '1'"!]FLEXIO_SPI_IP_SHIFTCTL_TIMPOL(1)[!ELSE!]FLEXIO_SPI_IP_SHIFTCTL_TIMPOL(0)[!ENDIF!]/* If CPHA = 0, set "Trigger active low" */
,(uint32)0u /* SHIFTCFG for RX will be updated in runtime => don't care */
,(uint32)0u /* Timer compare for CLK will be updated in runtime*/
[!IF "num:i($CS_Continuons) = 1"!],FLEXIO_SPI_IP_TIMCFG_TIMDIS(3) | FLEXIO_SPI_IP_TIMCFG_DEFAULT_MASTER_VALUE_U32 /* Timer config for CLK with CS continous mode */
[!ELSE!],FLEXIO_SPI_IP_TIMCFG_TIMDIS(2) | FLEXIO_SPI_IP_TIMCFG_DEFAULT_MASTER_VALUE_U32 /* Timer config for CLK with Non CS continous mode */
[!ENDIF!][!//
 | [!IF "$CPOL = 1"!]FLEXIO_SPI_IP_TIMCFG_TIMOUT(0)[!ELSE!]FLEXIO_SPI_IP_TIMCFG_TIMOUT(1)[!ENDIF!] /* CPOL = HIGH then TIMOUT must be 'Timer output is logic one when enabled and is not affected by timer reset'.. */
, FLEXIO_SPI_IP_TIMCTL_PINSEL([!"substring-after(node:value(node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclAddPinId),'PIN_')"!]) /* Select PIN */
 | FLEXIO_SPI_IP_TIMCTL_TRGSRC(1) /* Internal trigger selected */
 | FLEXIO_SPI_IP_TIMCTL_TIMOD(1) /* Dual 8-bit counters baud mode */
 | FLEXIO_SPI_IP_TIMCTL_TRGPOL(1) /* Pin is active low */
 | FLEXIO_SPI_IP_TIMCTL_PINCFG(3) /* Set is output pin */
, FLEXIO_SPI_IP_TIMCMP_CMP(0xFFFFu)
, FLEXIO_SPI_IP_TIMCFG_TIMDIS(1) /* Timer disabled on Timer N-1 disable */
 | FLEXIO_SPI_IP_TIMCFG_TIMENA(1) /* Timer enabled on Timer N-1 enable */
 | FLEXIO_SPI_IP_TIMCFG_TIMRST(2) /* Timer will reset to keep CS asserted following clk control */
, FLEXIO_SPI_IP_TIMCTL_PINSEL([!"substring-after(node:value(node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclAddPinId),'PIN_')"!])/* Select PIN */
 | FLEXIO_SPI_IP_TIMCTL_TIMOD(3) /* Single 16-bit counter mode */
 | [!IF "$CsPolarity = 1"!]FLEXIO_SPI_IP_TIMCTL_PINPOL(0) /* Pin is active high */[!ELSE!]FLEXIO_SPI_IP_TIMCTL_PINPOL(1) /* Pin is active low */[!ENDIF!]
 | FLEXIO_SPI_IP_TIMCTL_PINCFG(3) /* Set is output pin */
,&Flexio_Spi_Ip_DeviceParamsCfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($ExIndex)"!]U]
 [!ENDCODE!][!//
[!ENDSELECT!][!///
[!CODE!][!//
};
[!ENDCODE!][!//
    [!VAR "ExIndex"="$ExIndex + 1"!][!//

[!ELSE!][!// Slave mode
[!CALL "GenerateExternalDeviceInfoSlave"!]
[!CODE!]
   [!"num:i(substring-after(node:value(../../../SpiGeneral/SpiPhyUnit/*[number($varSpiHwUnit)]/SpiPhyUnitMapping),'SPI_'))"!]U,  /* Instance */
   (uint8)FLEXIO_SPI_IP_CPOL_[!"SpiShiftClockIdleLevel"!]_U8,
   (uint8)FLEXIO_SPI_IP_CPHA_[!"SpiDataShiftEdge"!]_U8,
[!ENDCODE!]
        [!IF "$DualClockSupport = 'true'"!]
[!CODE!]
   { /*ClkTimeCmpBaudRate is not used in salve mode */
   (uint32)0u, /* Normal clock */
   (uint32)0u /* Alternate Clock */
   },
   { /* ClkTimeCfgTimDec is not used in slave mode */
   (uint32)0u, /* Normal clock */
   (uint32)0u /* Alternate Clock */
   },
[!ENDCODE!]
        [!ELSE!]
[!CODE!]
   (uint32)0u, /*ClkTimeCmpBaudRate is not used in salve mode - Normal clock */
   (uint32)0u, /*ClkTimeCfgTimDec is not used in slave mode - Normal clock */
[!ENDCODE!]
        [!ENDIF!]
[!VAR "CSIB_number"="string(substring-after(node:value(SpiHwUnit),'CSIB')+1)"!]
[!SELECT "(../../../SpiGeneral/SpiPhyUnit/*[num:i($CSIB_number)])"!]
[!CODE!]
    /* Shifter control (SHIFTCTL) for TX */
   FLEXIO_SPI_IP_SHIFTCTL_TIMSEL([!"substring-after(node:value(node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclChannelId),'CHANNEL_')"!]) /* Select TIMER */
 | FLEXIO_SPI_IP_SHIFTCTL_PINSEL([!"substring-after(node:value(node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclPinId),'PIN_')"!]) /* Select PIN */
 | FLEXIO_SPI_IP_SHIFTCTL_PINCFG(3) /* Set is output pin */
 | FLEXIO_SPI_IP_SHIFTCTL_SMOD(2) /* Transmit mode */
 | [!IF "num:i($CPHA) = '1'"!]FLEXIO_SPI_IP_SHIFTCTL_TIMPOL(0)[!ELSE!]FLEXIO_SPI_IP_SHIFTCTL_TIMPOL(1)[!ENDIF!] /* If CPHA = 0, set "Trigger active low" */
    /* Shifter control (SHIFTCFG) for TX */
,[!IF "num:i($CPHA) = '1'"!]FLEXIO_SPI_IP_SHIFTCFG_SSTART(1)[!ELSE!](uint32)0u[!ENDIF!]
    /* Shifter control (SHIFTCTL) for RX */
   ,FLEXIO_SPI_IP_SHIFTCTL_TIMSEL([!"substring-after(node:value(node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclChannelId),'CHANNEL_')"!]) /* Select TIMER */
 | FLEXIO_SPI_IP_SHIFTCTL_PINSEL([!"substring-after(node:value(node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclPinId),'PIN_')"!]) /* Select PIN */
 | FLEXIO_SPI_IP_SHIFTCTL_PINCFG(0) /* Set is input pin */
 | FLEXIO_SPI_IP_SHIFTCTL_SMOD(1) /* Receive mode */
 | [!IF "num:i($CPHA) = '1'"!]FLEXIO_SPI_IP_SHIFTCTL_TIMPOL(1)[!ELSE!]FLEXIO_SPI_IP_SHIFTCTL_TIMPOL(0)[!ENDIF!]/* If CPHA = 0, set "Trigger active low" */
    /* SHIFTCFG for RX - will be updated in runtime => don't care */
,(uint32)0u
    /* Timer compare (TIMCMP) for CLK - will be updated in runtime */
,(uint32)0u
    /* Timer Configuration (TIMCFG) for CLK - Only 1 timer need to configure */
[!IF "num:i($CPHA) = 1"!],FLEXIO_SPI_IP_TIMCFG_DEFAULT_SLAVE_CPHA1_VALUE[!ELSE!],FLEXIO_SPI_IP_TIMCFG_DEFAULT_SLAVE_CPHA0_VALUE[!ENDIF!]
    /* Timer control (TIMCTL) for CLK - Only 1 timer need to configure */
, FLEXIO_SPI_IP_TIMCTL_TRGSEL([!"num:i((substring-after(node:value(node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclAddPinId),'PIN_')*2))"!]) /* Select PIN for CS input -> 2*N - Pin N input */
 | FLEXIO_SPI_IP_TIMCTL_PINSEL([!"substring-after(node:value(node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclAddPinId),'PIN_')"!]) /* Select PIN for CLK input*/
 | FLEXIO_SPI_IP_TIMCTL_TRGSRC(1) /* Internal trigger selected */
 | FLEXIO_SPI_IP_TIMCTL_TIMOD(3) /* Single 16-bit counter mode */
 | FLEXIO_SPI_IP_TIMCTL_TRGPOL(1) /* Pin is active low */
 | FLEXIO_SPI_IP_TIMCTL_PINCFG(0) /* Set is input pin */
    /* In slave mode, only 1 timer is selected. */
 ,(uint32)0u
 ,(uint32)0u
 ,(uint32)0u
,&Flexio_Spi_Ip_DeviceParamsCfg[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($ExIndex)"!]U]
 [!ENDCODE!][!//
[!ENDSELECT!][!/// 
[!CODE!][!//
};
[!ENDCODE!][!//
    [!VAR "ExIndex"="$ExIndex + 1"!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//

[!VAR "hwUnitIndex" = "0"!]
[!LOOP "SpiGeneral/SpiPhyUnit/*"!][!//
[!IF "contains(node:value(SpiPhyUnitMapping),'FLEXIO_SPI')"!]
[!IF "SpiPhyUnitMode = 'SPI_MASTER'"!][!//
[!VAR "Master" = "1"!]
[!ELSE!]
[!VAR "Master" = "0"!]
[!ENDIF!][!// "SpiPhyUnitMode     = 'SPI_MASTER'"
    
[!CODE!][!//
/* The FLEXIO SPI Hw Unit configuration structures */
const Flexio_Spi_Ip_ConfigType Flexio_Spi_Ip_PhyUnitConfig_[!"@name"!][!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    [!"num:i(substring-after(SpiPhyUnitMapping,'SPI_'))"!]U,  /* Instance */
    #if (FLEXIO_SPI_IP_SLAVE_SUPPORT == STD_ON)
    (boolean)[!IF "$Master = 0"!]TRUE[!ELSE!]FALSE[!ENDIF!],
    #endif
    #if (FLEXIO_SPI_IP_DMA_USED == STD_ON)
[!ENDCODE!][!//
    [!IF "(ecu:get('Spi.SpiDMAPresent') = 'TRUE') and (../../SpiGlobalDmaEnable = 'true') and (../../SpiLevelDelivered >0) and (SpiPhyUnitAsyncUseDma = 'true')"!][!//
[!CODE!][!//
    (boolean)TRUE,
    (uint8)[!"node:value(node:ref(SpiPhyTxDmaChannel)/dmaLogicChannel_LogicName)"!], /* txDmaChannel */
    (uint8)[!"node:value(node:ref(SpiPhyRxDmaChannel)/dmaLogicChannel_LogicName)"!], /* rxDmaChannel */
[!ENDCODE!][!//
    [!ELSE!]
[!CODE!][!//
    (boolean)FALSE,
    (uint8)0U, /* txDmaChannel */
    (uint8)0U, /* rxDmaChannel */
[!ENDCODE!][!//
    [!ENDIF!]
[!CODE!][!//
    #endif  /* (FLEXIO_SPI_IP_DMA_USED == STD_ON) */
    FLEXIO_SPI_IP_POLLING, /* Transfer mode */
    (uint32)8UL, /* Frame size - dummy value */
    (boolean)TRUE, /*Lsb - dummy value */
    (uint32)1U,  /* Default Data - dummy value */
    FLEXIO_SPI_IP_SHIFTER_[!"substring-after(node:value(node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclChannelId),'CHANNEL_')"!]_U8,
    FLEXIO_SPI_IP_SHIFTER_[!"substring-after(node:value(node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclChannelId),'CHANNEL_')"!]_U8,
    FLEXIO_SPI_IP_TIMER_[!"substring-after(node:value(node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclChannelId),'CHANNEL_')"!]_U8,
    FLEXIO_SPI_IP_TIMER_[!"substring-after(node:value(node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclChannelId),'CHANNEL_')"!]_U8,
    (uint8)[!"num:i($hwUnitIndex)"!]U, /* State structure element from the array */
};
[!ENDCODE!][!//
[!VAR "hwUnitIndex"= "$hwUnitIndex + 1"!]
[!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!][!//

#define SPI_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Spi_MemMap.h"

#endif /*(FLEXIO_SPI_IP_ENABLE == STD_ON)*/
/*==================================================================================================
                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
                                       GLOBAL FUNCTIONS
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */
