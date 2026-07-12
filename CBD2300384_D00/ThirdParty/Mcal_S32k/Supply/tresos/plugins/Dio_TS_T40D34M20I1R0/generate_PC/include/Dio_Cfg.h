[!CODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : SIUL2
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

#ifndef DIO_CFG_H
#define DIO_CFG_H

/**
*   @file Dio_Cfg.h
*   @implements Dio_Cfg.h_Artifact
*
*   @defgroup DIO_CFG Dio Cfg
*   @{
*/

#ifdef __cplusplus
extern "C" {
#endif

/*=================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
=================================================================================================*/
#include "StandardTypes.h"
#include "Siul2_Dio_Ip_Cfg.h"

/*=================================================================================================
*                              SOURCE FILE VERSION INFORMATION
=================================================================================================*/
#define DIO_VENDOR_ID_CFG_H                   43
#define DIO_AR_RELEASE_MAJOR_VERSION_CFG_H    4
#define DIO_AR_RELEASE_MINOR_VERSION_CFG_H    4
#define DIO_AR_RELEASE_REVISION_VERSION_CFG_H 0
#define DIO_SW_MAJOR_VERSION_CFG_H            2
#define DIO_SW_MINOR_VERSION_CFG_H            0
#define DIO_SW_PATCH_VERSION_CFG_H            1

/*=================================================================================================
*                                     FILE VERSION CHECKS
=================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* StandardTypes.h version check start */
    #if ((DIO_AR_RELEASE_MAJOR_VERSION_CFG_H != STD_AR_RELEASE_MAJOR_VERSION) ||   \
        (DIO_AR_RELEASE_MINOR_VERSION_CFG_H != STD_AR_RELEASE_MINOR_VERSION)       \
        )
        #error "AUTOSAR Version Numbers of Dio_Cfg.h and StandardTypes.h are different"
    #endif
    /* StandardTypes.h version check end */
#endif
/*=================================================================================================
*                                          CONSTANTS
=================================================================================================*/
[!SELECT "DioConfig"!][!//
[!INDENT "0"!][!//
[!VAR "numChannelGroupsInConfig" = "count(DioPort/*/DioChannelGroup/*)"!][!//
[!IF "$numChannelGroupsInConfig != 0"!][!//
/**
* @brief      Dio driver Channel Groups configuration switch.
*
*/
#define DIO_CHANNEL_GROUPS_AVAILABLE
[!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDSELECT!][!//
/**
* @brief          Enable or Disable Development Error Detection.
*
* @implements     DIO_DEV_ERROR_DETECT_define
*/
#define DIO_DEV_ERROR_DETECT    ([!IF "DioGeneral/DioDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @brief          Function @p Dio_GetVersionInfo() enable switch.
*
* @implements     DIO_VERSION_INFO_API_define
*/
#define DIO_VERSION_INFO_API    ([!IF "DioGeneral/DioVersionInfoApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @brief          Function @p Dio_FlipChannel() enable switch.
*/
#define DIO_FLIP_CHANNEL_API    ([!IF "DioGeneral/DioFlipChannelApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @brief          Function @p Dio_MaskedWritePort() enable switch.
*/
#define DIO_MASKEDWRITEPORT_API ([!IF "DioGeneral/DioMaskedWritePortApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @brief          Platform specific define stating if mapping of port bits over port pins is reversed.
*/
[!IF "ecu:get('Dio.ReversedMappingOfPortBitsOverPortPins')"!][!//
#define DIO_REVERSED_MAPPING_OF_PORT_BITS_OVER_PORT_PINS    (STD_ON)
[!ELSE!][!//
#define DIO_REVERSED_MAPPING_OF_PORT_BITS_OVER_PORT_PINS    (STD_OFF)
[!ENDIF!][!//

/**
* @brief          Reversed port functionality enable switch.
*
* @implements DIO_REVERSEPORTBITS_define
*/
#define DIO_REVERSEPORTBITS     ([!IF "text:tolower(DioGeneral/DioReversePortBits) != ecu:get('Dio.ReversedMappingOfPortBitsOverPortPins')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])


/**
* @brief          Undefined pins masking enable switch.
*/
#define DIO_READZERO_UNDEFINEDPORTS ([!IF "DioGeneral/DioReadZeroForUndefinedPortPins"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

[!NOCODE!][!//
[!VAR "NumPortsForWrite"="count(ecu:list('Dio.AvailablePortPinsForWrite'))"!][!//
[!VAR "NumPortsForRead"="count(ecu:list('Dio.AvailablePortPinsForRead'))"!][!//
[!IF "$NumPortsForWrite != $NumPortsForRead"!][!//
    [!ERROR!]The number ports in Dio.AvailablePortPinsForWrite and Dio.AvailablePortPinsForRead were not same value in resource file[!ENDERROR!][!//
[!ENDIF!][!//
[!ENDNOCODE!][!//

[!NOCODE!][!//
/**
* @brief          The number of partition on the platform.
*
* @note           Used for channel, port and channel group validation.
*/
[!VAR "maxPartition" = "num:i(0)"!][!//
[!IF "node:value(DioGeneral/DioMulticoreSupport)='true'"!]
    [!IF "node:exists(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition)"!][!//
        [!VAR "maxPartition" = "num:i(count(as:modconf("EcuC")[1]/EcucPartitionCollection/*[1]/EcucPartition/*))"!][!//
    [!ENDIF!][!//
[!ENDIF!][!//
[!IF "$maxPartition = 0"!][!//
    [!VAR "maxPartition" = "num:i(1)"!][!//
[!ENDIF!][!//
#define DIO_MAX_PARTITION              ([!"num:i($maxPartition)"!]U)
[!ENDNOCODE!][!//

/**
* @brief Enable/Disable multiocre function from the driver
*/
#define DIO_MULTICORE_ENABLED          [!IF "(DioGeneral/DioMulticoreSupport)"!](STD_ON)[!ELSE!](STD_OFF)[!ENDIF!]

/**
* @brief          Number of implemented ports.
*
* @note           Used for channel, port and channel group validation.
*/
#define DIO_NUM_PORTS_U16               ((uint16)[!"num:inttohex(count(ecu:list('Dio.AvailablePortPinsForWrite')))"!])

/**
* @brief          The number of partition on the port
*
* @note           Used for port validation.
*/
#define DIO_PORT_PARTITION_U16          ((uint16)[!"num:i(count(ecu:list('Dio.AvailablePortPinsForWrite')))"!]U)

/**
* @brief          Number of channels in a port.
*
* @note           Used for channel, port and channel group validation.
*/
#define DIO_NUM_CHANNELS_PER_PORT_U16   ((uint16)((uint16)sizeof(Dio_PortLevelType) * 0x8U))

/**
* @brief          Number of channels available on the implemented ports.
*
* @note           Used for channel validation.
*/
#if (STD_ON == DIO_DEV_ERROR_DETECT)
    #define DIO_NUM_CHANNELS_U16            ((uint16)[!"num:i(ecu:list('Dio.LastPinAvailable'))"!]U)
#endif


/**
* @brief The number of partition on the channel.
*
* @note           Used for channel validation.
*/
#define DIO_CHANNEL_PARTITION_U16           ((uint16)[!"num:i(16*count(ecu:list('Dio.AvailablePortPinsForWrite')))"!]U)

/**
* @brief          Mask representing no available channels on a port.
*
* @note           Used for channel validation.
*/
#if (STD_ON == DIO_DEV_ERROR_DETECT)
    #define DIO_NO_AVAILABLE_CHANNELS_U16   ((Dio_PortLevelType)0x0U)
#endif

/**
* @brief          Mask representing the maximum valid offset for a channel group.
*
* @note           Used for channel group validation.
*/
#if (STD_ON == DIO_DEV_ERROR_DETECT)
    #define DIO_MAX_VALID_OFFSET_U8         ((uint8)0xFU)
#endif

/**
*   @brief   Enables or disables the access to a hardware register from user mode
*            USER_MODE_SOFT_LOCKING:        All reads to hw registers will be done via REG_PROT, user mode access
*            SUPERVISOR_MODE_SOFT_LOCKING:  Locks the access to the registers only for supervisor mode
*
*   @note    Currently, no register protection mechanism is used for Dio driver.
*/
#define DIO_USER_MODE_SOFT_LOCKING      (STD_OFF)

[!IF "IMPLEMENTATION_CONFIG_VARIANT='VariantPreCompile'"!][!//
/**
* @brief          Dio driver Pre-Compile configuration switch.
*/
#define DIO_PRECOMPILE_SUPPORT

[!ELSEIF "IMPLEMENTATION_CONFIG_VARIANT='VariantLinkTime'"!][!//
/**
* @brief          Dio driver Link-time configuration switch.
*/
#define DIO_LINKTIME_SUPPORT
[!ENDIF!][!//

/**
* @brief Support for User mode.
*        If this parameter has been configured to 'STD_ON', the Dio driver code can be executed from both supervisor and user mode.
*
*/
[!IF "DioGeneral/DioEnableUserModeSupport"!]
#define DIO_ENABLE_USER_MODE_SUPPORT   (STD_ON)

/**
* @brief Support for REG_PROT in SIUL2 IP.
*        If the current platform implements REG_PROT for SIUL2 IP, this parameter will be defined, and will enable REG_PROT configuration for SIUL2 IP in DIO drvier
*/
#define DIO_SIUL2_REG_PROT_AVAILABLE
[!ELSE!]
#define DIO_ENABLE_USER_MODE_SUPPORT   (STD_OFF)
[!ENDIF!]

#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
 #ifdef DIO_ENABLE_USER_MODE_SUPPORT
  #if (STD_ON == DIO_ENABLE_USER_MODE_SUPPORT)
    #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running Dio in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined
  #endif /* (STD_ON == DIO_ENABLE_USER_MODE_SUPPORT) */
 #endif /* ifdef DIO_ENABLE_USER_MODE_SUPPORT*/
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT */

/**
* @brief Number of SIUL2 instances on the platform.
*/
#define DIO_NUM_SIUL2_INSTANCES_U8      ((uint8)[!"num:i(count(ecu:list('Dio.Siul2Instances')))"!])

/**
* @brief List of identifiers for each of the SIUL2 instances on the platform
*/
[!NOCODE!][!//
[!VAR "CurInstanceIdx"="0"!][!//
[!FOR "LoopCounter" = "1" TO "num:i(count(ecu:list('Dio.Siul2Instances')))"!][!//
    [!VAR "CurInstanceName"="ecu:list('Dio.Siul2Instances')[num:i($LoopCounter)]"!][!//
[!CODE!][!//
#define DIO_[!"$CurInstanceName"!]_U8        ((uint8)[!"num:i($CurInstanceIdx)"!])
[!ENDCODE!][!//
    [!VAR "CurInstanceIdx"="$CurInstanceIdx + 1"!][!//
[!ENDFOR!][!//
[!ENDNOCODE!][!//

/*=================================================================================================
*                                      DEFINES AND MACROS
=================================================================================================*/
[!SELECT "DioConfig"!][!//

/**
* @brief          Symbolic name for the configuration Dio_ConfigPC.
*
*/
#define Dio_ConfigPC    (Dio_Config)

/* ========== [!"as:name(.)"!] ========== */

[!VAR "ConfigName"="as:name(.)"!][!//
[!VAR "ChannelGroupdIdx"="0"!][!//
[!LOOP "DioPort/*"!][!/*
    */!][!VAR "PortId" = "DioPortId"!][!//
/* ---------- [!"node:name(.)"!] ---------- */

/**
* @brief          Symbolic name for the port [!"node:name(.)"!].
*
*/
#define DioConf_DioPort_[!"node:name(.)"!]  ((uint8)[!"num:inttohex($PortId,2)"!]U)

[!LOOP "DioChannel/*"!]
[!INDENT "0"!][!//
    [!VAR "PortChannelId" = "DioChannelId"!]
    [!VAR "ChannelId" = "16*$PortId + $PortChannelId"!][!//
    [!IF "(num:i(count(DioGeneral/DioEcucPartitionRef/*)) > 0) and (num:i(count(DioChannelEcucPartitionRef/*)) = 0)"!][!//
        [!ERROR!]The partition of Channel [!"$PortChannelId"!] in port [!"$PortId"!] was not configured[!ENDERROR!]
    [!ENDIF!][!//
/**
* @brief          Symbolic name for the channel [!"node:name(.)"!].
*
*/
#define  DioConf_DioChannel_[!"node:name(.)"!] ((uint16)[!"num:inttohex($ChannelId,4)"!]U)

[!ENDINDENT!][!//
[!ENDLOOP!][!//
[!LOOP "DioChannelGroup/*"!][!//
[!INDENT "0"!][!//
    [!IF "(num:i(count(DioGeneral/DioEcucPartitionRef/*)) > 0) and (0 = num:i(count(DioChannelGroupEcucPartitionRef/*)))"!][!//
        [!ERROR!]The partition of ChannelGroup [!"node:name(.)"!] in port [!"$PortId"!] was not configured[!ENDERROR!]
    [!ENDIF!][!//
/**
* @brief        Symbolic name for the channel group [!"node:name(.)"!].
*
*/

#define DioConf_DioChannelGroupIdentification_[!"node:name(.)"!]    \
                (&[!"$ConfigName"!]_aChannelGroupList[[!"num:dectoint($ChannelGroupdIdx)"!]])[!//

[!VAR "ChannelGroupdIdx" = "$ChannelGroupdIdx + 1"!][!//
[!ENDINDENT!][!//
[!ENDLOOP!][!//
[!ENDLOOP!][!//
[!ENDSELECT!][!//

/*=================================================================================================
*                                             ENUMS
=================================================================================================*/


/*=================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
=================================================================================================*/

/**
* @brief          Type of a DIO port representation.
*
* @implements     Dio_PortType_typedef
*/
typedef uint8 Dio_PortType;

/**
* @brief          Type of a DIO channel representation.
*
* @implements     Dio_ChannelType_typedef
*/
typedef uint16 Dio_ChannelType;

/**
* @brief          Type of a DIO port levels representation.
*
* @implements     Dio_PortLevelType_typedef
*/
typedef uint16 Dio_PortLevelType;

/**
* @brief          Type of a DIO channel levels representation.
*
* @implements     Dio_LevelType_typedef
*/
typedef uint8 Dio_LevelType;

/**
* @brief          Type of a DIO channel group representation.
*
* @implements     Dio_ChannelGroupType_struct
*/
typedef struct
{
    Dio_PortType      port;      /**< @brief Port identifier.  */
    uint8             u8offset;    /**< @brief Bit offset within the port. */
    Dio_PortLevelType mask;      /**< @brief Group mask. */
} Dio_ChannelGroupType;

/**
* @brief          Type of a DIO configuration structure.
*
* @note           In this implementation there is no need for a configuration
*                 structure there is only a dummy field, it is recommended
*                 to initialize this field to zero.
*
* @implements     Dio_ConfigType_struct
*/
typedef struct
{
    uint8 u8NumChannelGroups; /**< @brief Number of channel groups in configuration */
    const Dio_ChannelGroupType * pChannelGroupList;     /**< @brief
                                               Pointer to list of channel groups in configuration */
    const uint32 * pau32Dio_ChannelToPartitionMap;      /**< @brief Pointer to channel to partition mapping */
    const uint32 * pau32Dio_PortToPartitionMap;         /**< @brief Pointer to port to partition mapping */
} Dio_ConfigType;

/*=================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
=================================================================================================*/
#define DIO_START_SEC_CONST_8
#include "Dio_MemMap.h"

/**
* @brief Array of values storing the SIUL2 instance each port on the platform belongs to
*/
extern const uint8 Dio_au8PortSiul2Instance[DIO_NUM_PORTS_U16];

/**
* @brief Array of values storing the offset PORT0 has inside the SIUL2 instance it
*        belongs to
*/
extern const uint8 Dio_au8Port0OffsetInSiul2Instance[DIO_NUM_SIUL2_INSTANCES_U8];

[!SELECT "DioConfig"!][!//
[!IF "../DioGeneral/DioVirtWrapperSupport"!][!//

/**
* @brief Array containing list of mapping channel for virtualization wrapper
*/

extern const uint8 Dio_au8ChannelToVirtWrapperMap[DIO_CHANNEL_PARTITION_U16];
[!ENDIF!][!//
[!ENDSELECT!][!//
[!//

#define DIO_STOP_SEC_CONST_8
#include "Dio_MemMap.h"

#define DIO_START_SEC_CONST_32
#include "Dio_MemMap.h"

/**
* @brief Array containing list of mapping channel for partition
*/
extern const uint32 Dio_au32ChannelToPartitionMap[DIO_CHANNEL_PARTITION_U16];

/**
* @brief Array containing list of mapping port for partition
*/
extern const uint32 Dio_au32PortToPartitionMap[DIO_PORT_PARTITION_U16];

#define DIO_STOP_SEC_CONST_32
#include "Dio_MemMap.h"

#define DIO_START_SEC_CONST_16
#include "Dio_MemMap.h"
[!//
/**
* @brief Array of bitmaps of output pins available per port
*/
extern const Dio_PortLevelType Dio_aAvailablePinsForWrite[DIO_NUM_PORTS_U16];

/**
* @brief Array of bitmaps of input pins available per port
*/
extern const Dio_PortLevelType Dio_aAvailablePinsForRead[DIO_NUM_PORTS_U16];

#define DIO_STOP_SEC_CONST_16
#include "Dio_MemMap.h"
[!//
[!SELECT "DioConfig"!][!//
[!//
[!VAR "numChannelGroupsInConfig" = "count(DioPort/*/DioChannelGroup/*)"!][!//
[!IF "$numChannelGroupsInConfig != 0"!][!//
[!//
#define DIO_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Dio_MemMap.h"
[!//
/**
* @brief          List of channel groups in configuration [!"node:name(.)"!].
*/
extern const Dio_ChannelGroupType [!"as:name(.)"!]_aChannelGroupList[[!"num:dectoint($numChannelGroupsInConfig)"!]];

/**
* @brief          List of channel groups mapping with partition in configuration [!"node:name(.)"!].
*/
extern const Dio_ChannelGroupType * const Dio_pxChannelGroupsListForEachPartition[[!"num:i($maxPartition)"!]][[!"num:i($numChannelGroupsInConfig)"!]];

#define DIO_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Dio_MemMap.h"
[!//
[!//
[!ENDIF!][!//
[!ENDSELECT!][!//
[!//
[!LOOP "DioConfig/*/DioPort/*"!][!/*
    */!][!VAR "crt_name"="node:name(.)"!][!/*
    */!][!VAR "n"="0"!][!/*
    */!][!LOOP "../../DioPort/*"!][!/*
        */!][!IF "$crt_name=node:name(.)"!][!VAR "n"="$n+1"!][!ENDIF!][!/*
    */!][!ENDLOOP!][!/*
    */!][!IF "$n > 1"!][!ERROR "Port name duplicated. Port names must be unique even across configurations"!][!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!LOOP "DioConfig/DioPort/*/DioChannelGroup/*"!][!/*
    */!][!VAR "crt_name"="node:name(.)"!][!/*
    */!][!VAR "n"="0"!][!/*
    */!][!LOOP "../../../../DioPort/*/DioChannelGroup/*"!][!/*
        */!][!IF "$crt_name=node:name(.)"!][!VAR "n"="$n+1"!][!ENDIF!][!/*
    */!][!ENDLOOP!][!/*
    */!][!IF "$n > 1"!][!ERROR "ChannelGroup name duplicated. Channel group names must be unique even across ports and configurations"!][!ENDIF!][!//
[!ENDLOOP!][!//
[!//
[!LOOP "DioConfig/DioPort/*/DioChannel/*"!][!/*
    */!][!VAR "crt_name"="node:name(.)"!][!/*
    */!][!VAR "n"="0"!][!/*
    */!][!LOOP "../../../../DioPort/*/DioChannel/*"!][!/*
        */!][!IF "$crt_name=node:name(.)"!][!VAR "n"="$n+1"!][!ENDIF!][!/*
    */!][!ENDLOOP!][!/*
    */!][!IF "$n > 1"!][!ERROR "Channel name duplicated. Channel names must be unique even across ports and configurations"!][!ENDIF!][!//
[!ENDLOOP!][!//
[!//

/*=================================================================================================
*                                    FUNCTION PROTOTYPES
=================================================================================================*/

#ifdef __cplusplus
}
#endif

#endif  /* DIO_CFG_H */

/** @} */
[!ENDCODE!][!//
