/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : 
*   Dependencies         : none
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
*   @file       Crc_Ip_Cfg.c
*   @implements Crc_Ip_Cfg.c_Artifact
*   @version    2.0.1
*
*   @brief      AUTOSAR Crc Post-Build(PB) configuration file code template.
*   @details    Code template for Post-Build(PB) configuration file generation.
*
*   @addtogroup CRC_CFG
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Crc_Ip_Cfg.h"
[!IF "./CrcGeneral/CrcDmaSupportEnable"!][!//
#include "Dma_Ip_Cfg.h"
[!ENDIF!]
/*=================================================================================================
*                              SOURCE FILE VERSION INFORMATION
=================================================================================================*/
#define CRC_IP_CFG_VENDOR_ID_C                      43
#define CRC_IP_CFG_AR_RELEASE_MAJOR_VERSION_C       4
#define CRC_IP_CFG_AR_RELEASE_MINOR_VERSION_C       4
#define CRC_IP_CFG_AR_RELEASE_REVISION_VERSION_C    0
#define CRC_IP_CFG_SW_MAJOR_VERSION_C               2
#define CRC_IP_CFG_SW_MINOR_VERSION_C               0
#define CRC_IP_CFG_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Crc header file are of the same vendor */
#if (CRC_IP_CFG_VENDOR_ID_C != CRC_IP_CFG_VENDOR_ID)
    #error "CRC_Ip_Cfg.c and CRC_Ip_Cfg.h have different vendor ids"
#endif
/* Check if current file and Crc header file are of the same Autosar version */
#if ((CRC_IP_CFG_AR_RELEASE_MAJOR_VERSION_C    != CRC_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (CRC_IP_CFG_AR_RELEASE_MINOR_VERSION_C    != CRC_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (CRC_IP_CFG_AR_RELEASE_REVISION_VERSION_C != CRC_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CRC_Ip_Cfg.c and CRC_Ip_Cfg.h are different"
#endif
/* Check if current file and Crc header file are of the same Software version */
#if ((CRC_IP_CFG_SW_MAJOR_VERSION_C != CRC_IP_CFG_SW_MAJOR_VERSION) || \
     (CRC_IP_CFG_SW_MINOR_VERSION_C != CRC_IP_CFG_SW_MINOR_VERSION) || \
     (CRC_IP_CFG_SW_PATCH_VERSION_C != CRC_IP_CFG_SW_PATCH_VERSION)    \
    )
    #error "Software Version Numbers of CRC_Ip_Cfg.c and CRC_Ip_Cfg.h are different"
#endif

[!IF "./CrcGeneral/CrcDmaSupportEnable"!][!//
/* Check if current file and Dma_Ip_Cfg.h header file are of the same vendor */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if CDD_Mcl.c file and Mcal header file are of the same Autosar version */
#if ((CRC_IP_CFG_AR_RELEASE_MAJOR_VERSION_C != DMA_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (CRC_IP_CFG_AR_RELEASE_MINOR_VERSION_C != DMA_IP_CFG_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of CRC_Ip_Cfg.c and Dma_Ip_Cfg.h are different"
#endif
#endif
[!ENDIF!]
/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
                                        LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
                                       LOCAL CONSTANTS
==================================================================================================*/
[!/*****************************************************************************/!][!//
[!/* MACRO CrcHarwareInstance                                                  */!][!//
[!/* This Macro to get the Crc instance                                        */!][!//
[!/*****************************************************************************/!][!//
[!MACRO "CrcHarwareInstance"!][!//
[!NOCODE!]
[!IF "text:match(./CrcCalculationType,'CRC_IP_HARDWARE_CALCULATION')"!]
    [!VAR "MacCrcHarwareInstance"="./CrcHardwareConfig/CrcHwInstance"!]
[!ELSE!]
    [!VAR "MacCrcHarwareInstance"="'CRC_HW_INSTANCE_0'"!]
[!ENDIF!]
    [!VAR "OutCrcHarwareInstance"="$MacCrcHarwareInstance"!]
[!ENDNOCODE!][!//
[!"$OutCrcHarwareInstance"!][!//
[!ENDMACRO!][!//
[!//
[!/*****************************************************************************/!][!//
[!/* MACRO CrcHarwareChannel                                                   */!][!//
[!/* This Macro to get the CRC Calculation Method                              */!][!//
[!/*****************************************************************************/!][!//
[!MACRO "CrcHarwareChannel"!][!//
[!NOCODE!]
[!IF "text:match(./CrcCalculationType,'CRC_IP_HARDWARE_CALCULATION')"!]
    [!VAR "MacCrcHwChannel"="./CrcHardwareConfig/CrcHwChannel"!]
[!ELSE!]
    [!VAR "MacCrcHwChannel"="'CRC_HW_CHANNEL_0'"!]
[!ENDIF!]
[!ENDNOCODE!][!//
[!"$MacCrcHwChannel"!][!//
[!ENDMACRO!][!//
[!//
[!/*****************************************************************************/!][!//
[!/* MACRO CrcPolynomialValue                                                  */!][!//
[!/* Get the CRC Polynomial Value                                              */!][!//
[!/*****************************************************************************/!][!//
[!MACRO "CrcPolynomialValue"!][!//
[!NOCODE!]
[!VAR "MacCrcPolynomialValue"="0"!]
[!IF "text:contains(ecu:list('Crc.Protocols.CustomConfig.List'), ./CrcProtocolInfo/CrcProtocolType)"!]
    [!VAR "MacCrcPolynomialValue"="num:hextoint(./CrcProtocolInfo/CrcPolynomialValue)"!]
[!ELSE!]
    [!IF "./CrcProtocolInfo/CrcProtocolType = 'CRC_PROTOCOL_8BIT_SAE_J1850'"!]
        [!VAR "MacCrcPolynomialValue"="num:i(29)"!]
    [!ELSEIF "./CrcProtocolInfo/CrcProtocolType = 'CRC_PROTOCOL_8BIT_H2F'"!]
        [!VAR "MacCrcPolynomialValue"="num:i(47)"!]
    [!ELSEIF "./CrcProtocolInfo/CrcProtocolType = 'CRC_PROTOCOL_16BIT_CCITT_FALSE'"!]
        [!VAR "MacCrcPolynomialValue"="num:i(4129)"!]
    [!ELSEIF "./CrcProtocolInfo/CrcProtocolType = 'CRC_PROTOCOL_16BIT_ARC'"!]
        [!VAR "MacCrcPolynomialValue"="num:i(32773)"!]
    [!ELSEIF "./CrcProtocolInfo/CrcProtocolType = 'CRC_PROTOCOL_32BIT_ETHERNET'"!]
        [!VAR "MacCrcPolynomialValue"="num:i(79764919)"!]
    [!ELSEIF "./CrcProtocolInfo/CrcProtocolType = 'CRC_PROTOCOL_32BIT_E2E_P4'"!]
        [!VAR "MacCrcPolynomialValue"="num:i(4104977171)"!]
    [!ELSEIF "./CrcProtocolInfo/CrcProtocolType = 'CRC_PROTOCOL_64BIT_ECMA'"!]
        [!VAR "MacCrcPolynomialValue"=" num:i('4823603603198064275')"!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!][!//
[!"num:inttohex($MacCrcPolynomialValue)"!][!//
[!ENDMACRO!][!//
[!//
[!/*****************************************************************************/!][!//
[!/* MACRO CrcWriteBitSwap                                                     */!][!//
[!/* This Macro to get the CRC Write Data Swap Type                            */!][!//
[!/*****************************************************************************/!][!//
[!MACRO "CrcWriteBitSwap"!][!//
[!NOCODE!]
[!IF "text:contains(ecu:list('Crc.Protocols.CustomConfig.List'), ./CrcProtocolInfo/CrcProtocolType)"!]
    [!VAR "MacWriteBitSwap" = "text:toupper(./CrcProtocolInfo/CrcWriteBitSwap)"!]
[!ELSE!]
    [!IF "text:contains(ecu:list('Crc.Protocols.SwapBitByte.List'), ./CrcProtocolInfo/CrcProtocolType)"!]
        [!VAR "MacWriteBitSwap" = "'TRUE'"!]
    [!ELSE!]
        [!VAR "MacWriteBitSwap" = "'FALSE'"!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!][!//
[!"$MacWriteBitSwap"!][!//
[!ENDMACRO!][!//
[!//
[!/*****************************************************************************/!][!//
[!/* MACRO CrcWriteByteSwap                                                    */!][!//
[!/* This Macro to get the CRC Write Data Swap Type                            */!][!//
[!/*****************************************************************************/!][!//
[!MACRO "CrcWriteByteSwap"!][!//
[!NOCODE!]
[!IF "text:contains(ecu:list('Crc.Protocols.CustomConfig.List'), ./CrcProtocolInfo/CrcProtocolType)"!]
    [!VAR "MacWriteByteSwap" = "text:toupper(./CrcProtocolInfo/CrcWriteByteSwap)"!]
[!ELSE!]
    [!VAR "MacWriteByteSwap" = "'FALSE'"!]
[!ENDIF!]
[!ENDNOCODE!][!//
[!"$MacWriteByteSwap"!][!//
[!ENDMACRO!][!//
[!//
[!/*****************************************************************************/!][!//
[!/* MACRO CrcReadBitSwap                                                      */!][!//
[!/* This Macro to get the CRC Read Data Swap Type                             */!][!//
[!/*****************************************************************************/!][!//
[!MACRO "CrcReadBitSwap"!][!//
[!NOCODE!]
[!IF "text:contains(ecu:list('Crc.Protocols.CustomConfig.List'), ./CrcProtocolInfo/CrcProtocolType)"!]
    [!VAR "MacReadBitSwap" = "text:toupper(./CrcProtocolInfo/CrcReadBitSwap)"!]
[!ELSE!]
    [!IF "text:contains(ecu:list('Crc.Protocols.SwapBitByte.List'), ./CrcProtocolInfo/CrcProtocolType)"!]
        [!VAR "MacReadBitSwap" = "'TRUE'"!]
    [!ELSE!]
        [!VAR "MacReadBitSwap" = "'FALSE'"!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!][!//
[!"$MacReadBitSwap"!][!//
[!ENDMACRO!][!//
[!//
[!/*****************************************************************************/!][!//
[!/* MACRO CrcReadByteSwap                                                     */!][!//
[!/* This Macro to get the CRC Read Data Swap Type                             */!][!//
[!/*****************************************************************************/!][!//
[!MACRO "CrcReadByteSwap"!][!//
[!NOCODE!]
[!IF "text:contains(ecu:list('Crc.Protocols.CustomConfig.List'), ./CrcProtocolInfo/CrcProtocolType)"!]
    [!VAR "MacReadByteSwap" = "text:toupper(./CrcProtocolInfo/CrcReadByteSwap)"!]
[!ELSE!]
    [!IF "text:contains(ecu:list('Crc.Protocols.SwapBitByte.List'), ./CrcProtocolInfo/CrcProtocolType)"!]
        [!VAR "MacReadByteSwap" = "'TRUE'"!]
    [!ELSE!]
        [!VAR "MacReadByteSwap" = "'FALSE'"!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!][!//
[!"$MacReadByteSwap"!][!//
[!ENDMACRO!][!//
[!//
[!/*****************************************************************************/!][!//
[!/* MACRO CrcInversionEnable                                                  */!][!//
[!/* This Macro to get the CRC XOR Value                                       */!][!//
[!/*****************************************************************************/!][!//
[!MACRO "CrcInversionEnable"!][!//
[!NOCODE!]
[!VAR "MacCrcXORValue"="0"!]
[!IF "text:contains(ecu:list('Crc.Protocols.CustomConfig.List'), ./CrcProtocolInfo/CrcProtocolType)"!]
    [!VAR "MacCrcXORValue" = "text:toupper(./CrcProtocolInfo/CrcInversionEnable)"!]
[!ELSE!]
    [!IF "contains(./CrcProtocolInfo/CrcProtocolType,'16BIT')"!]
        [!VAR "MacCrcXORValue"="'FALSE'"!]
    [!ELSE!]
        [!VAR "MacCrcXORValue"="'TRUE'"!]
    [!ENDIF!]
[!ENDIF!]
[!ENDNOCODE!][!//
[!"$MacCrcXORValue"!][!//
[!ENDMACRO!][!//
[!//
[!/**********************************************************************************************/!][!//
[!/*                                    END OF MACRO DEFINE                                     */!][!//
[!/**********************************************************************************************/!][!//
[!//
[!//


[!VAR "ChannelID" = "num:i(0)"!][!//
[!LOOP "CrcChannelConfig/*"!][!//
#define CRC_START_SEC_VAR_INIT_UNSPECIFIED
#include "Crc_MemMap.h"
/**
* @brief CRC Logic Channel [!"$ChannelID"!] Configuration
**/
static Crc_Ip_LogicChannelConfigType LogicChannelCfg_[!"$ChannelID"!] =
{
    /* Crc_Ip_ProtocolType Protocol         */ [!"./CrcProtocolInfo/CrcProtocolType"!],
    /* uint64  PolynomialValue              */ [!CALL "CrcPolynomialValue"!]U,
    /* boolean WriteBitSwap                 */ (boolean)[!CALL "CrcWriteBitSwap"!],
    /* boolean WriteByteSwap                */ (boolean)[!CALL "CrcWriteByteSwap"!],
    /* boolean ReadBitSwap                  */ (boolean)[!CALL "CrcReadBitSwap"!],
    /* boolean ReadByteSwap                 */ (boolean)[!CALL "CrcReadByteSwap"!],
    /* boolean InverseEnable                */ (boolean)[!CALL "CrcInversionEnable"!]
};
#define CRC_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Crc_MemMap.h"

#define CRC_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Crc_MemMap.h"
/**
* @brief CRC Logic Channel Type [!"$ChannelID"!] Configuration
**/
static const Crc_Ip_LogicChannelType LogicChannelType_[!"$ChannelID"!] =
{
    /* uint8 CalculationType        */ [!"./CrcCalculationType"!],
    /* uint8 HwInstance             */ [!CALL "CrcHarwareInstance"!],
    /* uint8 HwChannel              */ [!CALL "CrcHarwareChannel"!],
[!IF "../../CrcGeneral/CrcDmaSupportEnable"!][!//
[!IF "./CrcHardwareConfig/CrcDmaChannelEnable='true' and ./CrcAutosarSelect = 'NON_AUTOSAR'"!][!//
    /* boolean DmaLogicChannelEnable  */ (boolean)[!"'TRUE'"!],
[!IF "node:exists(./CrcHardwareConfig/CrcDmaLogicChannelName)"!][!//
    /* uint32 DmaLogicChannelName    */ [!"node:ref(./CrcHardwareConfig/CrcDmaLogicChannelName)/dmaLogicChannel_LogicName"!],
[!ELSE!][!//
        [!ERROR!]
        [!CR!] 1.Why you got this error? [!CR!]-> When Dma Channel Enable is enable, You have to configure Dma Channel Reference.
        [!CR!]2. How to resolve this error?
        - Step 1: Add Mcl Component.
        - Step 2: In "General" tab, Enable DMA Support.
        - Step 3: Configure at least one channel for DMA in "Dma Logic Channel" tab.
        - Step 4: In Crc component, Enable "DMA Logic Channel Name" Node and select Dma Channel Reference for it.
        [!ENDERROR!]
[!ENDIF!][!//
[!ELSE!][!//
    /* boolean DmaLogicChannelEnable  */ (boolean)[!"'FALSE'"!],
    /* uint32 DmaLogicChannelName    */ [!"'DMA_LOGIC_CH_0'"!],
[!ENDIF!][!//
[!ENDIF!][!//
    /* LogicChannelConfig            */ &LogicChannelCfg_[!"$ChannelID"!]
};
#define CRC_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Crc_MemMap.h"
[!/* comment */!]
[!VAR "ChannelID"="num:i($ChannelID + 1)"!][!//
[!ENDLOOP!][!//
[!//
[!//
[!/*****************************************************************************/!][!//
[!/* Start Generate list Channel Configuration                                 */!][!//
[!/*****************************************************************************/!][!//
[!VAR "Maxcount" = "num:i(count(CrcChannelConfig/*) - 1)"!][!//

#define CRC_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Crc_MemMap.h"

/**
 * @brief  Crc Ip List Logic Channel Configuration
 * */
static const Crc_Ip_LogicChannelType * const paxLogicChannelConfig[] =
{
[!FOR "ChannelID" = "0" TO "$Maxcount"!][!//
[!IF "$ChannelID < $Maxcount"!][!//
    &LogicChannelType_[!"$ChannelID"!],
[!ELSE!][!//
    &LogicChannelType_[!"$ChannelID"!]
[!ENDIF!][!//
[!ENDFOR!][!//
};

/**
 * @brief  Crc Ip Initialization
 * */
const Crc_Ip_InitType CrcIp_xConfigInit =
{
    /* LogicChannelConfigList */ paxLogicChannelConfig
};

#define CRC_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Crc_MemMap.h"

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */
