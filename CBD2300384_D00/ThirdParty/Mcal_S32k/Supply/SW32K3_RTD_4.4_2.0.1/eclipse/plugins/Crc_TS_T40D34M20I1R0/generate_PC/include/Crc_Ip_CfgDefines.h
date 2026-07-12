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

/* Prevention from multiple including the same header */
#ifndef CRC_IP_CFG_DEFINE_H
#define CRC_IP_CFG_DEFINE_H

/**
*   @file       Crc_Ip_CfgDefines.h
*   @implements Crc_Ip_CfgDefines.h_Artifact
*   @version    2.0.1
*
*   @brief      AUTOSAR Crc Post-Build(PB) configuration file code template.
*   @details    Code template for Post-Build(PB) configuration file generation.
*
*   @defgroup   CRC_CFG CRC Configuration
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
#include "StandardTypes.h"
[!NOCODE!][!// Include specific header file
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!VAR "DerivativeName" = "text:toupper(substring-before(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative,'_'))"!]
    [!IF "contains($DerivativeName, 'S32G2')"!][!//
        [!CODE!][!WS "0"!]#include "S32G274A_CRC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains($DerivativeName, 'S32G3')"!][!//
        [!CODE!][!WS "0"!]#include "S32G399A_CRC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains($DerivativeName, 'S32R45')"!][!//
        [!CODE!][!WS "0"!]#include "S32R45_CRC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains($DerivativeName, 'S32R41')"!][!//
        [!CODE!][!WS "0"!]#include "S32R41_CRC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains($DerivativeName, 'SAF85')"!][!//
        [!CODE!][!WS "0"!]#include "SAF85XX_CRC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains($DerivativeName, 'S32K1')"!][!//
        [!CODE!][!WS "0"!]#include "[!"$DerivativeName"!]_CRC.h"[!CR!][!ENDCODE!][!//
    [!ELSEIF "contains($DerivativeName, 'S32K3')"!][!//
        [!IF "contains($DerivativeName, 'S32K341')"!]
            [!CODE!][!WS "0"!]#include "S32K342_CRC.h"[!CR!][!ENDCODE!][!//
        [!ELSE!]
            [!CODE!][!WS "0"!]#include "[!"$DerivativeName"!]_CRC.h"[!CR!][!ENDCODE!][!//
        [!ENDIF!]
    [!ENDIF!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

[!VAR "CheckCRC64"="num:i(0)"!][!//
[!LOOP "CrcChannelConfig/*"!][!//
[!IF "(node:value('./CrcProtocolInfo/CrcProtocolType') = 'CRC_PROTOCOL_64BIT_ECMA') or (node:value('./CrcProtocolInfo/CrcProtocolType') = 'CRC_PROTOCOL_64BIT_CUSTOM')"!][!//
[!VAR "CheckCRC64"="num:i($CheckCRC64 + 1)"!][!//]
[!ENDIF!][!//
[!ENDLOOP!][!//

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define CRC_IP_CFG_DEFINES_VENDOR_ID                       43
#define CRC_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION        4
#define CRC_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION        4
#define CRC_IP_CFG_DEFINES_AR_RELEASE_REVISION_VERSION     0
#define CRC_IP_CFG_DEFINES_SW_MAJOR_VERSION                2
#define CRC_IP_CFG_DEFINES_SW_MINOR_VERSION                0
#define CRC_IP_CFG_DEFINES_SW_PATCH_VERSION                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if Crc_Ip_CfgDefines.h file and StandardTypes.h header file are of the same Autosar version */
#if ((CRC_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
     (CRC_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION) \
    )
    #error "AutoSar Version Numbers of Crc_Ip_CfgDefines.h file and StandardTypes.h are different"
#endif
#endif
/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!VAR "DerivativeName" = "text:toupper(substring-before(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative,'_'))"!]
        [!IF "contains($DerivativeName, 'S32G3')"!][!//
[!CODE!]
#define IP_CRC_BASE           IP_CRC_0_BASE
#define IP_CRC                IP_CRC_0

#define CRC_IP_HARDWARE_16BIT_CCITT_FALSE_POLYG   (uint8)0U
#define CRC_IP_HARDWARE_32BIT_ETHERNET_POLYG      (uint8)1U
#define CRC_IP_HARDWARE_8BIT_SAE_J1850_POLYG      (uint8)2U
#define CRC_IP_HARDWARE_8BIT_H2F_POLYG            (uint8)3U

#define CRC_IP_HARDWARE_16BIT_OUTP_MASK          0xFFFFU
#define CRC_IP_HARDWARE_8BIT_OUTP_MASK           0x00FFU
[!ENDCODE!]
        [!ELSEIF "contains($DerivativeName, 'S32G2')"!][!//
[!CODE!]
#define CRC_IP_HARDWARE_16BIT_CCITT_FALSE_POLYG   (uint8)0U
#define CRC_IP_HARDWARE_32BIT_ETHERNET_POLYG      (uint8)1U
#define CRC_IP_HARDWARE_8BIT_SAE_J1850_POLYG      (uint8)2U
#define CRC_IP_HARDWARE_8BIT_H2F_POLYG            (uint8)3U

#define CRC_IP_HARDWARE_16BIT_OUTP_MASK          0xFFFFU
#define CRC_IP_HARDWARE_8BIT_OUTP_MASK           0x00FFU
[!ENDCODE!]
        [!ELSEIF "contains($DerivativeName, 'S32R45')"!][!//
[!CODE!]
#define CRC_IP_HARDWARE_16BIT_CCITT_FALSE_POLYG   (uint8)0U
#define CRC_IP_HARDWARE_32BIT_ETHERNET_POLYG      (uint8)1U
#define CRC_IP_HARDWARE_8BIT_SAE_J1850_POLYG      (uint8)2U
#define CRC_IP_HARDWARE_8BIT_H2F_POLYG            (uint8)3U

#define CRC_IP_HARDWARE_16BIT_OUTP_MASK          0xFFFFU
#define CRC_IP_HARDWARE_8BIT_OUTP_MASK           0x00FFU
[!ENDCODE!]
        [!ELSEIF "contains($DerivativeName, 'S32R41')"!][!//
[!CODE!]
#define IP_CRC_BASE           IP_CRC_0_BASE
#define IP_CRC                IP_CRC_0

#define CRC_IP_HARDWARE_16BIT_CCITT_FALSE_POLYG  (uint8)0U
#define CRC_IP_HARDWARE_32BIT_ETHERNET_POLYG     (uint8)1U
#define CRC_IP_HARDWARE_8BIT_SAE_J1850_POLYG     (uint8)2U

#define CRC_IP_HARDWARE_16BIT_OUTP_MASK          0xFFFFU
#define CRC_IP_HARDWARE_8BIT_OUTP_MASK           0x00FFU
[!ENDCODE!]
        [!ELSEIF "contains($DerivativeName, 'SAF85')"!][!//
[!CODE!]
#define IP_CRC_BASE           IP_CRC_0_BASE
#define IP_CRC                IP_CRC_0

#define CRC_IP_HARDWARE_16BIT_CCITT_FALSE_POLYG  (uint8)0U
#define CRC_IP_HARDWARE_32BIT_ETHERNET_POLYG     (uint8)1U
#define CRC_IP_HARDWARE_8BIT_SAE_J1850_POLYG     (uint8)2U

#define CRC_IP_HARDWARE_16BIT_OUTP_MASK          0xFFFFU
#define CRC_IP_HARDWARE_8BIT_OUTP_MASK           0x00FFU
[!ENDCODE!]
        [!ENDIF!][!//
[!ENDIF!][!//


/*! @brief  Enables or disables PRE-COMPILE SUPPORT. */
#define CRC_PRECOMPILE_SUPPORT               [!IF "IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile'"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/*! @brief  Error detect. */
#define CRC_DET_IS_AVAILABLE                 [!IF "CrcGeneral/CrcDetectError"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_DEV_ERROR_DETECT              [!IF "CrcGeneral/CrcDetectError"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/*! @brief  Enables or disables User Mode use Register Protect. */
#define CRC_ENABLE_USER_MODE_SUPPORT         [!IF "CrcGeneral/CrcEnableUserModeSupport"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #if (STD_ON == CRC_ENABLE_USER_MODE_SUPPORT)
        #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running CRC in user mode, the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined
    #endif /* (STD_ON == CRC_ENABLE_USER_MODE_SUPPORT) */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */

/*! @brief  DMA Supported. */
#define CRC_IP_DMA_IS_AVAILABLE              [!IF "CrcGeneral/CrcDmaSupportEnable"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/*! @brief  Enables or disables Multicore feature. */
#define CRC_MULTICORE_SUPPORT_ENABLED        [!IF "(./CrcGeneral/CrcMultiCoreEnable = 'true') and (ecu:get('Crc.Hardware.NumOfCoreSupport') > 1)"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/*! @brief  Enables or disables get Version Info. */
#define CRC_VERSION_INFO_API_ENABLE          [!IF "CrcGeneral/CrcVersionInfoApi"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/*! @brief  Number of configured partitions. */
#define CRC_CORE_NOF_PARTITION               ([!IF "(./CrcGeneral/CrcMultiCoreEnable = 'true') and (ecu:get('Crc.Hardware.NumOfCoreSupport') > 1)"!][!"num:i(count(as:modconf("EcuC")[1]/EcucHardware/*[1]/EcucCoreDefinition/*))"!][!ELSE!]1[!ENDIF!]U)

/*! @brief  Core Default. */
#define CRC_CORE_DEFAULT                     (0U)

/*! @brief  Error detect. */
#define CRC_HW_INSTANCE_0                    ((uint8)0U)

/*! @brief  Hardware channel support. */
[!VAR "ChannelIndex" = "num:i(0)"!][!//
[!LOOP "ecu:list('Crc.Hardware.Channels.List')"!][!//
#define [!"."!]                        ((uint8)[!"$ChannelIndex"!]U)
[!VAR "ChannelIndex"="num:i($ChannelIndex + 1)"!][!//
[!ENDLOOP!][!//

[!VAR "ChannelCount" = "num:i(count(ecu:list('Crc.Hardware.Channels.List')))"!][!//
/*! @brief  Number of channel support. */
#ifndef CRC_IP_CHANNEL_COUNT_U8
#define CRC_IP_CHANNEL_COUNT_U8              ((uint8)[!"$ChannelCount"!]U)
#endif

/*! @brief  Detect Channel Invalid. */
#define CRC_IP_CHANNEL_INVALID_U32           ((uint32)0xFFFFFFFFU)

/*! @brief  Hardware Features. */
#define CRC_IP_HARDWARE_IS_AVAILABLE                      [!IF "ecu:get('Crc.Hardware.IsAvailable')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_HARDWARE_PROTOCOL_IS_AVAILABLE             [!IF "ecu:get('Crc.Hardware.Protocol.IsAvailable')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_HARDWARE_WIDTH_IS_AVAILABLE                [!IF "ecu:get('Crc.Hardware.Width.IsAvailable')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_HARDWARE_POLYNOM_IS_AVAILABLE              [!IF "ecu:get('Crc.Hardware.Polynom.IsAvailable')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_HARDWARE_INITIAL_SEED_VALUE_IS_AVAILABLE   [!IF "ecu:get('Crc.Hardware.InitialSeedValue.IsAvailable')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_HARDWARE_WRITE_BIT_SWAP_IS_AVAILABLE       [!IF "ecu:get('Crc.Hardware.Write.BitSwap.IsAvailable')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_HARDWARE_WRITE_BYTE_SWAP_IS_AVAILABLE      [!IF "ecu:get('Crc.Hardware.Write.ByteSwap.IsAvailable')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_HARDWARE_READ_BIT_SWAP_IS_AVAILABLE        [!IF "ecu:get('Crc.Hardware.Read.BitSwap.IsAvailable')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_HARDWARE_READ_BYTE_SWAP_IS_AVAILABLE       [!IF "ecu:get('Crc.Hardware.Read.ByteSwap.IsAvailable')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_HARDWARE_INVERSE_BIT_IS_AVAILABLE          [!IF "ecu:get('Crc.Hardware.InverseBit.IsAvailable')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/*! @brief  Supported Hardware Protocols. */
#define CRC_IP_8BIT_SAE_J1850_HARDWARE_SUPPORTED          [!IF "contains(ecu:get('Crc.Protocols.HardwareSupport.List'),'CRC_PROTOCOL_8BIT_SAE_J1850')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_8BIT_H2F_HARDWARE_SUPPORTED                [!IF "contains(ecu:get('Crc.Protocols.HardwareSupport.List'),'CRC_PROTOCOL_8BIT_H2F')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_16BIT_CCITT_FALSE_HARDWARE_SUPPORTED       [!IF "contains(ecu:get('Crc.Protocols.HardwareSupport.List'),'CRC_PROTOCOL_16BIT_CCITT_FALSE')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_16BIT_ARC_HARDWARE_SUPPORTED               [!IF "contains(ecu:get('Crc.Protocols.HardwareSupport.List'),'CRC_PROTOCOL_16BIT_ARC')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_32BIT_ETHERNET_HARDWARE_SUPPORTED          [!IF "contains(ecu:get('Crc.Protocols.HardwareSupport.List'),'CRC_PROTOCOL_32BIT_ETHERNET')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_32BIT_E2E_P4_HARDWARE_SUPPORTED            [!IF "contains(ecu:get('Crc.Protocols.HardwareSupport.List'),'CRC_PROTOCOL_32BIT_E2E_P4')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_64BIT_ECMA_HARDWARE_SUPPORTED              [!IF "contains(ecu:get('Crc.Protocols.HardwareSupport.List'),'CRC_PROTOCOL_64BIT_ECMA')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

#define CRC_IP_8BIT_CUSTOM_HARDWARE_SUPPORTED             [!IF "contains(ecu:get('Crc.Protocols.HardwareSupport.List'),'CRC_PROTOCOL_8BIT_CUSTOM')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_16BIT_CUSTOM_HARDWARE_SUPPORTED            [!IF "contains(ecu:get('Crc.Protocols.HardwareSupport.List'),'CRC_PROTOCOL_16BIT_CUSTOM')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_32BIT_CUSTOM_HARDWARE_SUPPORTED            [!IF "contains(ecu:get('Crc.Protocols.HardwareSupport.List'),'CRC_PROTOCOL_32BIT_CUSTOM')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]
#define CRC_IP_64BIT_CUSTOM_HARDWARE_SUPPORTED            [!IF "contains(ecu:get('Crc.Protocols.HardwareSupport.List'),'CRC_PROTOCOL_64BIT_CUSTOM')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

#define CRC_IP_WRITE_BIT_SWAP_INVERETED                   [!IF "ecu:get('Crc.Hardware.WriteBitSwapInverted')"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CRC_IP_CFG_DEFINE_H */
