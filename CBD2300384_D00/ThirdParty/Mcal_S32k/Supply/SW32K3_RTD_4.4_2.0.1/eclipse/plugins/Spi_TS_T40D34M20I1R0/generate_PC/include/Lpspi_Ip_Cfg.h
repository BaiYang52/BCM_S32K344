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
[!CODE!][!//

#ifndef LPSPI_IP_CFG_H
#define LPSPI_IP_CFG_H

/**
*   @file    Lpspi_Ip_Cfg.h
*   @version 2.0.1
*
*   @brief   AUTOSAR Spi - Spi configuration header file.
*   @details This file is the header containing all the necessary information for SPI
*            module configuration(s).
*   @addtogroup LPSPI_DRIVER_CONFIGURATION Lpspi Driver Configuration
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
* 4) user callback header files
==================================================================================================*/
#include "Mcal.h"
#include "OsIf.h"
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!][!//
#include "Lpspi_Ip_[!"."!]_PBcfg.h"
[!ENDLOOP!][!//
[!ELSE!]
#include "Lpspi_Ip_PBcfg.h"
[!ENDIF!]
#include "[!"ecu:get('Spi.SpiHeaderNameLPSPI')"!].h"
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define LPSPI_IP_VENDOR_ID_CFG                       43
#define LPSPI_IP_AR_RELEASE_MAJOR_VERSION_CFG        4
#define LPSPI_IP_AR_RELEASE_MINOR_VERSION_CFG        4
#define LPSPI_IP_AR_RELEASE_REVISION_VERSION_CFG     0
#define LPSPI_IP_SW_MAJOR_VERSION_CFG                2
#define LPSPI_IP_SW_MINOR_VERSION_CFG                0
#define LPSPI_IP_SW_PATCH_VERSION_CFG                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and Mcal header file are of the same Autosar version */
    #if ((LPSPI_IP_AR_RELEASE_MAJOR_VERSION_CFG != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (LPSPI_IP_AR_RELEASE_MINOR_VERSION_CFG != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Lpspi_Ip_Cfg.h and Mcal.h are different"
    #endif

    /* Check if current file and OsIf header file are of the same Autosar version */
    #if ((LPSPI_IP_AR_RELEASE_MAJOR_VERSION_CFG != OSIF_AR_RELEASE_MAJOR_VERSION) || \
         (LPSPI_IP_AR_RELEASE_MINOR_VERSION_CFG != OSIF_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Lpspi_Ip_Cfg.h and OsIf.h are different"
    #endif
#endif

[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
/* Check if Lpspi_Ip_[!"."!]_PBcfg header file and Spi configuration header file are of the same vendor */
#if (LPSPI_IP_[!"."!]_VENDOR_ID_PBCFG != LPSPI_IP_VENDOR_ID_CFG)
    #error "Lpspi_Ip_[!"."!]_PBcfg.h and Lpspi_Ip_Cfg.h have different vendor IDs"
#endif
    /* Check if Lpspi_Ip_[!"."!]_PBcfg header file and Spi  configuration header file are of the same Autosar version */
#if ((LPSPI_IP_[!"."!]_AR_RELEASE_MAJOR_VERSION_PBCFG != LPSPI_IP_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (LPSPI_IP_[!"."!]_AR_RELEASE_MINOR_VERSION_PBCFG != LPSPI_IP_AR_RELEASE_MINOR_VERSION_CFG) || \
     (LPSPI_IP_[!"."!]_AR_RELEASE_REVISION_VERSION_PBCFG != LPSPI_IP_AR_RELEASE_REVISION_VERSION_CFG))
#error "AutoSar Version Numbers of Lpspi_Ip_[!"."!]_PBcfg.h and Lpspi_Ip_Cfg.h are different"
#endif
/* Check if Lpspi_Ip_[!"."!]_PBcfg header file and Spi configuration header file are of the same software version */
#if ((LPSPI_IP_[!"."!]_SW_MAJOR_VERSION_PBCFG != LPSPI_IP_SW_MAJOR_VERSION_CFG) || \
     (LPSPI_IP_[!"."!]_SW_MINOR_VERSION_PBCFG != LPSPI_IP_SW_MINOR_VERSION_CFG) || \
     (LPSPI_IP_[!"."!]_SW_PATCH_VERSION_PBCFG != LPSPI_IP_SW_PATCH_VERSION_CFG))
#error "Software Version Numbers of Lpspi_Ip_[!"."!]_PBcfg.h and Lpspi_Ip_Cfg.h are different"
#endif
[!ENDLOOP!]

[!ELSE!]
/* Check if Lpspi_Ip_PBcfg header file and Spi configuration header file are of the same vendor */
#if (LPSPI_IP_VENDOR_ID_PBCFG != LPSPI_IP_VENDOR_ID_CFG)
    #error "Lpspi_Ip_PBcfg.h and Lpspi_Ip_Cfg.h have different vendor IDs"
#endif
    /* Check if Lpspi_Ip_PBcfg header file and Spi  configuration header file are of the same Autosar version */
#if ((LPSPI_IP_AR_RELEASE_MAJOR_VERSION_PBCFG != LPSPI_IP_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (LPSPI_IP_AR_RELEASE_MINOR_VERSION_PBCFG != LPSPI_IP_AR_RELEASE_MINOR_VERSION_CFG) || \
     (LPSPI_IP_AR_RELEASE_REVISION_VERSION_PBCFG != LPSPI_IP_AR_RELEASE_REVISION_VERSION_CFG))
#error "AutoSar Version Numbers of Lpspi_Ip_PBcfg.h and Lpspi_Ip_Cfg.h are different"
#endif
/* Check if Lpspi_Ip_PBcfg header file and Spi configuration header file are of the same software version */
#if ((LPSPI_IP_SW_MAJOR_VERSION_PBCFG != LPSPI_IP_SW_MAJOR_VERSION_CFG) || \
     (LPSPI_IP_SW_MINOR_VERSION_PBCFG != LPSPI_IP_SW_MINOR_VERSION_CFG) || \
     (LPSPI_IP_SW_PATCH_VERSION_PBCFG != LPSPI_IP_SW_PATCH_VERSION_CFG))
#error "Software Version Numbers of Lpspi_Ip_PBcfg.h and Lpspi_Ip_Cfg.h are different"
#endif
[!ENDIF!]
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/
#define LPSPI_IP_CONFIG_EXT \
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
[!WS8!]LPSPI_IP_CONFIG_[!"."!]_PB \
[!ENDLOOP!][!//
[!ELSE!][!//
[!WS8!]LPSPI_IP_CONFIG_PB
[!ENDIF!]

[!NOCODE!][!//
[!VAR "HWtypeLPspi" = "0"!]
[!LOOP "SpiGeneral/SpiPhyUnit/*"!]
[!IF "contains(node:value(SpiPhyUnitMapping),'LPSPI')"!]
    [!VAR "HWtypeLPspi" = "$HWtypeLPspi + 1"!]
[!ENDIF!]
[!ENDLOOP!]
[!IF "num:i($HWtypeLPspi) = 0"!]
    [!VAR "HWtypeLPspi" = "1"!]
[!ENDIF!]
[!ENDNOCODE!][!//
/** @brief Total number of SpiPhyUnit configured. */
#define LPSPI_IP_NUMBER_OF_INSTANCES           ([!"num:i($HWtypeLPspi)"!]U)

/** @brief Number of FIFO entries supported */
#define LPSPI_IP_FIFO_SIZE_U8   ((uint8)[!"ecu:get('Spi.SpiFIFOSize')"!]U)

/** @brief HWUnits enabled/disabled */
[!NOCODE!]
[!VAR "SlaveSupport" = "'false'"!]
[!LOOP "SpiGeneral/SpiPhyUnit/*"!][!//
[!IF "contains(node:value(SpiPhyUnitMapping),'LPSPI')"!]
    [!VAR "unit"="substring-after(SpiPhyUnitMapping,'_')"!]
    [!CODE!]
#define LPSPI_IP_[!"$unit"!]_ENABLED  (STD_ON)
    [!ENDCODE!]
    [!IF "SpiPhyUnitMode = 'SPI_SLAVE'"!]
        [!VAR "SlaveSupport" = "'true'"!]
    [!ENDIF!]
[!ENDIF!]
[!ENDLOOP!]
[!CODE!]
#define LPSPI_IP_SLAVE_SUPPORT  ([!IF "$SlaveSupport = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!])
[!ENDCODE!]
[!ENDNOCODE!]

/**
* @brief Defines if transfers are made using DMA or FIFO.
* @details Defines if transfers are made using DMA or FIFO.
*/
#define LPSPI_IP_DMA_USED [!//
[!IF "ecu:get('Spi.SpiDMAPresent') = 'TRUE' and SpiGeneral/SpiGlobalDmaEnable = 'true'"!][!//
    (STD_ON)
[!ELSE!][!//
    (STD_OFF)
[!ENDIF!][!//

/**
* @brief If enabled, allows dual MCU clock configuration settings.
* @details If enabled, allows dual MCU clock configuration settings.
*/
[!NOCODE!][!//
[!VAR "DualClockSupport"= "'false'"!]
[!LOOP "SpiGeneral/SpiPhyUnit/*"!][!//
    [!IF "(node:exists(SpiPhyUnitAlternateClockRef))"!][!//
        [!VAR "DualClockSupport"= "'true'"!]
    [!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDNOCODE!][!//
#define LPSPI_IP_DUAL_CLOCK_MODE [!//
[!IF "$DualClockSupport = 'true'"!][!//
 (STD_ON)
[!ELSE!][!//
 (STD_OFF)
[!ENDIF!][!//

/**
* @brief          Enable Dma Fast transfer support.
* @details        When SpiAutosarExt/SpiEnableDmaFastTransferSupport = TRUE,
*                 the SPI driver can be supported to transfer multiple Jobs, Channels and CPU used only for processing end of Sequence transfer.
*/
#define LPSPI_IP_ENABLE_DMAFASTTRANSFER_SUPPORT   [!IF "(node:value(SpiAutosarExt/SpiEnableDmaFastTransferSupport) = 'true')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Enable User Mode Support.
* @details        When SpiAutosarExt/SpiEnableUserModeSupport = TRUE,
*                 the SPI driver can be executed from both supervisor and user mode.
*
* @api
*/
#define LPSPI_IP_ENABLE_USER_MODE_SUPPORT   [!IF "(ecu:get('Spi.SpiRegProtectAvailable') = 'TRUE') and (node:value(SpiAutosarExt/SpiEnableUserModeSupport) = 'true')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
 #ifdef LPSPI_IP_ENABLE_USER_MODE_SUPPORT
  #if (STD_ON == LPSPI_IP_ENABLE_USER_MODE_SUPPORT)
   #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running  Spi in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined
  #endif /* (STD_ON == LPSPI_IP_ENABLE_USER_MODE_SUPPORT) */
 #endif /* ifdef MCAL_ENABLE_USER_MODE_SUPPORT */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT*/

/**
* @brief SPI registers is controlled by REG_PROT IP.
*/
#define LPSPI_IP_REG_PROT_AVAILABLE   [!IF "ecu:get('Spi.SpiRegProtectAvailable') = 'TRUE'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

#if (STD_ON == LPSPI_IP_REG_PROT_AVAILABLE)
/**
* @brief The protection size
*/
    #define LPSPI_IP_PROT_MEM_U32                           ((uint32)[!"ecu:get('Spi.SpiRegProtectMem')"!]U)
#endif

/**
* @brief Maximum of frame size supported.
*/
#define LPSPI_IP_FRAMESIZE_MAX_U8                              ((uint8)[!"ecu:get('Spi.SpiFrameSizeMax')"!]U)

/**
* @brief Minimum of frame size supported.
*/
#define LPSPI_IP_FRAMESIZE_MIN_U8                              ((uint8)[!"ecu:get('Spi.SpiFrameSizeMin')"!]U)

/**
* @brief Define Timeout Method.
* @details Define a certain timeout method from OsIf will be used in the driver.
*/
#define LPSPI_IP_TIMEOUT_METHOD       [!//
[!IF "SpiGeneral/SpiTimeoutMethod = 'OSIF_COUNTER_DUMMY'"!][!//
    (OSIF_COUNTER_DUMMY)
[!ELSEIF "SpiGeneral/SpiTimeoutMethod = 'OSIF_COUNTER_SYSTEM'"!][!//
    (OSIF_COUNTER_SYSTEM)
[!ELSEIF "SpiGeneral/SpiTimeoutMethod = 'OSIF_COUNTER_CUSTOM'"!][!//
    (OSIF_COUNTER_CUSTOM)
[!ENDIF!][!//

/**
* @brief Switches ON or OFF for the detection and reporting of development errors(API parameter checking) at IP level.
*/
/** @implements LPSPI_IP_DEV_ERROR_DETECT_define  */
#define LPSPI_IP_DEV_ERROR_DETECT     (STD_OFF)

/**
* @brief Half duplex supported.
*/
/** @implements LPSPI_IP_HALF_DUPLEX_MODE_SUPPORT_define  */
#define LPSPI_IP_HALF_DUPLEX_MODE_SUPPORT     [!IF "(node:value(SpiAutosarExt/SpiHalfDuplexModeSupport) = 'true')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief Defines the "Number of Loops" timeout.
* @details Defines the "Number of Loops" timeout used Spi_AsyncTransmitHalfDuplex in half duplex mode
*         during the wait on TX transmission to complete before start next channel that in receive mode or latest channel in job.
*        One timeout unit means that no TX or RX was executed(the IF statements are returning FALSE).
*/
#define LPSPI_IP_HALF_DUPLEX_TIMEOUT_COUNTER       ((uint32)([!"num:i(SpiGeneral/SpiTransmitTimeout)"!]))

/*==================================================================================================
 *                                     DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
*                                            ENUMS
==================================================================================================*/


/*==================================================================================================
*                               STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/


/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif
#endif 

/** @} */
[!ENDCODE!][!//
