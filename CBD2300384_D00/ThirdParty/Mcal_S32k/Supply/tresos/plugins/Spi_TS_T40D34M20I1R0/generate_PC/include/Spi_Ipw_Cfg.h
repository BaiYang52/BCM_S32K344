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

#ifndef SPI_IPW_CFG_H
#define SPI_IPW_CFG_H

/**
*   @file    Spi_Ipw_Cfg.h
*   @version 2.0.1
*
*   @brief   AUTOSAR Spi - Spi configuration header file.
*   @details This file is the header containing all the extern declaration for SPI
*            module configuration(s).
*   @addtogroup SPI_DRIVER_CONFIGURATION Spi Driver Configuration
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

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define SPI_IPW_VENDOR_ID_CFG_H                    43
#define SPI_IPW_AR_RELEASE_MAJOR_VERSION_CFG_H     4
#define SPI_IPW_AR_RELEASE_MINOR_VERSION_CFG_H     4
#define SPI_IPW_AR_RELEASE_REVISION_VERSION_CFG_H  0
#define SPI_IPW_SW_MAJOR_VERSION_CFG_H             2
#define SPI_IPW_SW_MINOR_VERSION_CFG_H             0
#define SPI_IPW_SW_PATCH_VERSION_CFG_H             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/
/** @brief HWUnits enabled/disabled */
[!NOCODE!]
[!VAR "SlaveSupport" = "'false'"!]
[!LOOP "SpiGeneral/SpiPhyUnit/*"!][!//
    [!IF "SpiPhyUnitMode = 'SPI_SLAVE'"!]
        [!VAR "SlaveSupport" = "'true'"!]
    [!ENDIF!]
[!ENDLOOP!]
[!CODE!]
#define SPI_IPW_SLAVE_SUPPORT  ([!IF "$SlaveSupport = 'true'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!])
[!ENDCODE!]
[!ENDNOCODE!]

/**
* @brief Defines if transfers are made using DMA or FIFO.
* @details Defines if transfers are made using DMA or FIFO.
*/
#define SPI_IPW_DMA_USED [!//
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
#define SPI_IPW_DUAL_CLOCK_MODE [!//
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
#define SPI_IPW_ENABLE_DMAFASTTRANSFER_SUPPORT   [!IF "(node:value(SpiAutosarExt/SpiEnableDmaFastTransferSupport) = 'true')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Half duplex mdoe enable 
*/
#define SPI_IPW_SPI_HALF_DUPLEX_MODE_SUPPORT   [!IF "(node:value(SpiAutosarExt/SpiHalfDuplexModeSupport) = 'true')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/*
* @brief Flexio SPI supported enable.
* @details Flexio SPI supported enable. 
*/
#define SPI_IPW_SPI_FLEXIO_ENABLE [!//
[!IF "SpiAutosarExt/SpiFlexioEnable"!][!//
    (STD_ON)
[!ELSE!][!//
    (STD_OFF)
[!ENDIF!][!//

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

/** @} */

#endif 

[!ENDCODE!][!//
