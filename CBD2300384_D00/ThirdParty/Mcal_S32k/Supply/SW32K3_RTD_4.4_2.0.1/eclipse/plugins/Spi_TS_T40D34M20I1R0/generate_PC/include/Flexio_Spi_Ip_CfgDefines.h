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

#ifndef FLEXIO_SPI_IP_CFG_DEFINES_H
#define FLEXIO_SPI_IP_CFG_DEFINES_H

/**
*   @file     Flexio_Spi_Ip_CfgDefines.h
*   @version 2.0.1
*
*   @brief   AUTOSAR Spi - Spi configuration header file.
*   @details This file is the header containing all the necessary information for SPI
*            module configuration(s).
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

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define FLEXIO_SPI_IP_CFG_DEFINES_VENDOR_ID                    43
#define FLEXIO_SPI_IP_CFG_DEFINES_MODULE_ID                    83
#define FLEXIO_SPI_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION     4
#define FLEXIO_SPI_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION     4
#define FLEXIO_SPI_IP_CFG_DEFINES_AR_RELEASE_REVISION_VERSION  0
#define FLEXIO_SPI_IP_CFG_DEFINES_SW_MAJOR_VERSION             2
#define FLEXIO_SPI_IP_CFG_DEFINES_SW_MINOR_VERSION             0
#define FLEXIO_SPI_IP_CFG_DEFINES_SW_PATCH_VERSION             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/



[!NOCODE!][!//
[!CODE!][!WS "0"!]/* Macros that indicate FLEXIO channels used by SPI */[!CR!][!ENDCODE!][!//
[!LOOP "SpiGeneral/SpiPhyUnit/*"!]
  [!IF "contains(node:value(SpiPhyUnitMapping),'FLEXIO_SPI')"!]
    [!CODE!][!WS "0"!]#ifndef [!"node:ref(SpiFlexioTxAndClkChannelsConfig)/../../FlexioMclInstances"!]_CH_[!"num:i(text:split((node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#define [!"node:ref(SpiFlexioTxAndClkChannelsConfig)/../../FlexioMclInstances"!]_CH_[!"num:i(text:split((node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#else[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#error "[!"node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclChannelId"!] cannot be used by SPI driver to configure as TX and CLK channels. Channel locked by another driver!"[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#endif[!CR!][!CR!][!ENDCODE!][!//

    [!CODE!][!WS "0"!]#ifndef [!"node:ref(SpiFlexioRxAndCsChannelsConfig)/../../FlexioMclInstances"!]_CH_[!"num:i(text:split((node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#define [!"node:ref(SpiFlexioRxAndCsChannelsConfig)/../../FlexioMclInstances"!]_CH_[!"num:i(text:split((node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#else[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#error "[!"node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclChannelId"!] cannot be used by SPI driver to configure as RX and CS channels. Channel locked by another driver!"[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#endif[!CR!][!CR!][!ENDCODE!][!//

 [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!][!//

[!NOCODE!][!//

[!CODE!][!WS "0"!]/* Macros that indicate FLEXIO pins used by SPI */[!CR!][!ENDCODE!][!//
[!LOOP "SpiGeneral/SpiPhyUnit/*"!]
  [!IF "contains(node:value(SpiPhyUnitMapping),'FLEXIO_SPI')"!]
    [!CODE!][!WS "0"!]#ifndef [!"node:ref(SpiFlexioTxAndClkChannelsConfig)/../../FlexioMclInstances"!]_PIN_[!"num:i(text:split((node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclPinId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#define [!"node:ref(SpiFlexioTxAndClkChannelsConfig)/../../FlexioMclInstances"!]_PIN_[!"num:i(text:split((node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclPinId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#else[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#error "[!"node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclPinId"!] cannot be used by SPI driver to configure for TX channel. PIN locked by another driver!"[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#endif[!CR!][!CR!][!ENDCODE!][!//

    [!CODE!][!WS "0"!]#ifndef [!"node:ref(SpiFlexioRxAndCsChannelsConfig)/../../FlexioMclInstances"!]_PIN_[!"num:i(text:split((node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclPinId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#define [!"node:ref(SpiFlexioRxAndCsChannelsConfig)/../../FlexioMclInstances"!]_PIN_[!"num:i(text:split((node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclPinId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#else[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#error "[!"node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclPinId"!] cannot be used by SPI driver to configure for RX channel. PIN locked by another driver!"[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#endif[!CR!][!CR!][!ENDCODE!][!//
    
    [!CODE!][!WS "0"!]#ifndef [!"node:ref(SpiFlexioTxAndClkChannelsConfig)/../../FlexioMclInstances"!]_PIN_[!"num:i(text:split((node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclAddPinId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#define [!"node:ref(SpiFlexioTxAndClkChannelsConfig)/../../FlexioMclInstances"!]_PIN_[!"num:i(text:split((node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclAddPinId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#else[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#error "[!"node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclAddPinId"!] cannot be used by SPI driver to configure for Clk channel. PIN locked by another driver!"[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#endif[!CR!][!CR!][!ENDCODE!][!//
    
    [!CODE!][!WS "0"!]#ifndef [!"node:ref(SpiFlexioRxAndCsChannelsConfig)/../../FlexioMclInstances"!]_PIN_[!"num:i(text:split((node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclAddPinId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#define [!"node:ref(SpiFlexioRxAndCsChannelsConfig)/../../FlexioMclInstances"!]_PIN_[!"num:i(text:split((node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclAddPinId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#else[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#error "[!"node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclAddPinId"!] cannot be used by SPI driver to configure as for channel. PIN locked by another driver!"[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#endif[!CR!][!CR!][!ENDCODE!][!//
 [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!][!//

[!NOCODE!][!//
[!CODE!][!WS "0"!]/* Macros that indicate FLEXIO channels used by SPI */[!CR!][!ENDCODE!][!//
[!LOOP "SpiGeneral/SpiPhyUnit/*"!]
  [!IF "contains(node:value(SpiPhyUnitMapping),'FLEXIO_SPI')"!]
    [!CODE!][!WS "0"!]#ifdef [!"node:ref(SpiFlexioTxAndClkChannelsConfig)/../../FlexioMclInstances"!]_CH_[!"num:i(text:split((node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#define SPI_CHANNEL_[!"num:i(text:split((node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#else[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#error "[!"node:ref(SpiFlexioTxAndClkChannelsConfig)/FlexioMclChannelId"!] cannot be used by SPI driver to configure as TX and CLK channels. Channel locked by another driver!"[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#endif[!CR!][!CR!][!ENDCODE!][!//

    [!CODE!][!WS "0"!]#ifdef [!"node:ref(SpiFlexioRxAndCsChannelsConfig)/../../FlexioMclInstances"!]_CH_[!"num:i(text:split((node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#define SPI_CHANNEL_[!"num:i(text:split((node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclChannelId),'_')[2])"!]_USED[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#else[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "4"!]#error "[!"node:ref(SpiFlexioRxAndCsChannelsConfig)/FlexioMclChannelId"!] cannot be used by SPI driver to configure as RX and CS channels. Channel locked by another driver!"[!CR!][!ENDCODE!][!//
    [!CODE!][!WS "0"!]#endif[!CR!][!CR!][!ENDCODE!][!//
 [!ENDIF!]
[!ENDLOOP!]
[!ENDNOCODE!][!//

/*==================================================================================================
*                                              ENUMS
==================================================================================================*/

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                  GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                       FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif /* FLEXIO_SPI_IP_CFG_DEFINES_H */
