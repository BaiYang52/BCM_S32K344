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
*   @file Axbs_Ip_PBcfg.c
*
*   @addtogroup AXBS_IP AXBS IPV Driver
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
==================================================================================================*/
#include "Axbs_Ip_Types.h"
#include "Axbs_Ip_Cfg.h"
#include "Axbs_Ip_Device_Registers.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C                     43
#define RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C      4
#define RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C      4
#define RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C   0
#define RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C              2
#define RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C              0
#define RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Axbs_Ip_Types.h */
#if (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_AXBS_IP_TYPES_VENDOR_ID)
    #error "Axbs_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Axbs_Ip_Types.h have different vendor ids"
#endif
#if ((RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_AXBS_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_AXBS_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_AXBS_IP_TYPES_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Axbs_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Axbs_Ip_Types.h are different"
#endif
#if ((RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_AXBS_IP_TYPES_SW_MAJOR_VERSION) || \
     (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_AXBS_IP_TYPES_SW_MINOR_VERSION) || \
     (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_AXBS_IP_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Axbs_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Axbs_Ip_Types.h are different"
#endif

/* Checks against Axbs_Ip_Cfg.h */
#if (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_AXBS_IP_CFG_VENDOR_ID)
    #error "Axbs_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Axbs_Ip_Cfg.h have different vendor ids"
#endif
#if ((RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_AXBS_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_AXBS_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_AXBS_IP_CFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Axbs_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Axbs_Ip_Cfg.h are different"
#endif
#if ((RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_AXBS_IP_CFG_SW_MAJOR_VERSION) || \
     (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_AXBS_IP_CFG_SW_MINOR_VERSION) || \
     (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_AXBS_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Axbs_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Axbs_Ip_Cfg.h are different"
#endif

/* Checks against Axbs_Ip_Device_Registers.h */
#if (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_AXBS_IP_DEVICE_REGISTERS_VENDOR_ID)
    #error "Axbs_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Axbs_Ip_Device_Registers.h have different vendor ids"
#endif
#if ((RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_AXBS_IP_DEVICE_REGISTERS_AR_RELEASE_MAJOR_VERSION) || \
     (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_AXBS_IP_DEVICE_REGISTERS_AR_RELEASE_MINOR_VERSION) || \
     (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_AXBS_IP_DEVICE_REGISTERS_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Axbs_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Axbs_Ip_Device_Registers.h are different"
#endif
#if ((RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_AXBS_IP_DEVICE_REGISTERS_SW_MAJOR_VERSION) || \
     (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_AXBS_IP_DEVICE_REGISTERS_SW_MINOR_VERSION) || \
     (RM_AXBS_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_AXBS_IP_DEVICE_REGISTERS_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Axbs_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Axbs_Ip_Device_Registers.h are different"
#endif
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
[!IF "RmGeneral/RmCrossbarConfigurable"!]
[!MACRO "AxbsConfigSlave","AxbsConfigType"!]
[!NOCODE!]
[!VAR "crossbarInstanceCount" = "num:i(count(RmConfigSet/Crossbar_Configuration/RmCrossbarInstance/*))"!]
[!VAR "crossbarPortCount" = "0"!]
[!FOR "varInstance" = "1" TO "$crossbarInstanceCount"!][!//
    [!VAR "crossbarPortCount" = "num:i($crossbarPortCount + num:i(count(RmConfigSet/Crossbar_Configuration/RmCrossbarInstance/*[position() = $varInstance]/RmCrossbarHwSlavePort/*)))"!]
[!ENDFOR!]
[!CODE!]
/** @brief Configuration for the crossbar ports */
static const Axbs_Ip_SlavePortConfigType AXBS_Crossbar[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SlavePortConfig[[!"$crossbarPortCount"!]] =
{[!ENDCODE!]
[!VAR "comma" = "1"!][!//
[!VAR "crossbarInstanceCount" = "num:i(count(RmConfigSet/Crossbar_Configuration/RmCrossbarInstance/*))"!]
[!FOR "varInstance" = "1" TO "$crossbarInstanceCount"!][!//

[!SELECT "RmConfigSet/Crossbar_Configuration/RmCrossbarInstance/*[position() = $varInstance]/RmCrossbarHwSlavePort"!]
[!VAR "RmCrossbarInstanceNumber" = "substring-after(node:value(./../RmCrossbarHwInstance),"AXBS_")"!][!//
[!LOOP "./*"!][!//
        [!VAR "RmCrossbarHwPortNumber" = "node:value(./RmSlavePortNumber)"!][!//
        [!VAR "RmParkingCtrl" = "'AXBS_PCTL_PARK_FIELD'"!][!//
        [!IF "$RmCrossbarInstanceNumber = 0"!][!VAR "NumberOfMaster" = "count(ecu:list('Rm.Crossbar.AXBS0.Masters'))"!][!//
        [!ELSE!][!VAR "NumberOfMaster" = "count(ecu:list('Rm.Crossbar.AXBS1.Masters'))"!][!ENDIF!][!//
        [!IF "./RmCrossbarParkingControl = 'LastMaster'"!][!VAR "RmParkingCtrl" = "'AXBS_PCTL_LAST_MASTER'"!][!ENDIF!][!//
        [!IF "./RmCrossbarParkingControl = 'LowPowerPark'"!][!VAR "RmParkingCtrl" = "'AXBS_PCTL_LOW_POWER_PARK'"!][!ENDIF!][!//
[!CODE!]
    /** Configure for slave port [!"$RmCrossbarHwPortNumber"!] of instance AXBS_[!"$RmCrossbarInstanceNumber"!]  */
    {
        /**  Hardware port slave number */
        [!"$RmCrossbarHwPortNumber"!]U,
        /**  Hardware crossbar instance */
        [!"$RmCrossbarInstanceNumber"!]U,[!ENDCODE!]
[!IF "$PRSIsReadOnly = 0"!]
[!CODE!]
        /**  Priority register configuration */
        ([!ENDCODE!]
[!VAR "MaxSlotMasterProgrammable" = "num:i(count(ecu:list(text:replace('Rm.Crossbar.AXBSn.SlotMasterProgrammable','n',$RmCrossbarInstanceNumber))))"!]
[!FOR "Index" = "0" TO "$MaxSlotMasterProgrammable -1"!][!//
[!IF "num:i($Index) < ($NumberOfMaster)"!]
[!CODE!]
            (AXBS_MASTER[!"$Index"!]_MASK32 & ((uint32)AXBS_PRIORITY_[!"node:value(concat(node:path(.),concat("/RmCrossbarPrioMaster",string(num:i($Index)))))"!]_U32 << AXBS_MASTER[!"$Index"!]_SHIFT_U32))[!IF "num:i($Index) < ($MaxSlotMasterProgrammable -1)"!]|  \[!ENDIF!][!/*
            */!][!ENDCODE!]
[!ELSE!]
[!CODE!]
            (AXBS_MASTER[!"$Index"!]_MASK32 & ((uint32)AXBS_PRIORITY_[!"$Index"!]_U32 << AXBS_MASTER[!"$Index"!]_SHIFT_U32))[!IF "num:i($Index) < ($MaxSlotMasterProgrammable -1)"!]|  \[!ENDIF!][!/*
            */!][!ENDCODE!]
[!ENDIF!]
[!ENDFOR!][!//

[!CODE!]
        ),[!ENDCODE!]
[!ENDIF!][!//
[!CODE!]
        /** Control register configuration */
        (
            ( AXBS_LK_BIT_MASK32    & ((uint32)([!IF "./RmCrossbarEnableLock"!]AXBS_LOCK_EN_U32 [!ELSE!]AXBS_LOCK_DIS_U32[!ENDIF!])        << AXBS_LOCK_SHIFT_U32))  \[!/*
            */!][!IF "ecu:get('Rm.Crossbar.HaltLowPrioritySupport') = '1'"!][!//
[!CR!]            |(AXBS_HLP_BIT_MASK32   & ((uint32)([!IF "./RmCrossbarHaltLowPrio"!]AXBS_HLP_LOWEST_U32 [!ELSE!]AXBS_HLP_HIGHEST_U32[!ENDIF!])     << AXBS_HLP_SHIFT_U32 ))  \[!/*
            */!][!ENDIF!][!ENDCODE!]


[!FOR "Index" = "0" TO "$NumberOfMaster -1"!][!//
[!CODE!]
            |(AXBS_HPE[!"$Index"!]_BIT_MASK32  & ((uint32)([!IF "node:value(concat(node:path(.),concat("/RmCrossbarEnablePrioElevM",string(num:i($Index))))) = 'true'"!]AXBS_HPE_EN_U32 [!ELSE!]AXBS_HPE_DIS_U32[!ENDIF!])         << AXBS_HPE[!"$Index"!]_SHIFT_U32))  \[!ENDCODE!]
[!ENDFOR!][!//

[!CODE!]
            |(AXBS_ARB_BITS_MASK32  & ((uint32)[!IF "./RmCrossbarEnableFixedPrio"!](AXBS_ARB_FIXED_U32)      [!ELSE!](AXBS_ARB_ROUND_ROBIN_U32)[!ENDIF!] << AXBS_ARB_SHIFT_U32 ))  \
            |(AXBS_PCTL_BITS_MASK32 & ((uint32)([!"$RmParkingCtrl"!]_U32) << AXBS_PCTL_SHIFT_U32))  \
            |(AXBS_PARK_BITS_MASK32 & ((uint32)(AXBS_MASTER_[!"substring-after(./RmCrossbarParkField ,'Master')"!]_U32)))
        )
    }[!IF "(num:i($comma))<(num:i($crossbarPortCount))"!],[!ENDIF!][!VAR "comma"="$comma + 1"!][!ENDCODE!]
[!ENDLOOP!]
[!ENDSELECT!]
[!ENDFOR!]
[!ENDNOCODE!][!//
[!/* */!]};
[!ENDMACRO!][!// Rm Config Slave
[!ENDIF!]
/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/
[!IF "RmGeneral/RmCrossbarConfigurable"!]
[!VAR "EnableInitAXBS_MGPCR"              = "num:i(ecu:get('Rm.Crossbar.EnableInitAXBS_MGPCR'))"!][!//
[!VAR "PRSIsReadOnly"                     = "num:i(ecu:get('Rm.Crossbar.PRS.IsReadOnly'))"!][!//

#define RM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"
[!CALL "AxbsConfigSlave","AxbsConfigType" = "''"!]
/**
* @brief            Crossbar basic configuration.
* @details
*/
const Axbs_Ip_ConfigType  Axbs_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    /** @brief Number of configured logical ports. */
    [!"$crossbarPortCount"!]U,
[!IF "$EnableInitAXBS_MGPCR = 1"!]    /** @brief The number of Master ports. */
    [!"$MastercrossbarPortCount"!]U,
[!ENDIF!]    /** @brief Pointer to the structure containing slave port configuration */
    &AXBS_Crossbar[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SlavePortConfig
[!IF "$EnableInitAXBS_MGPCR = 1"!]       ,
    /** @brief Pointer to the structure containing master port configuration */
    &AXBS_Crossbar_MasterPortConfig[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ENDIF!]};

#define RM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"
[!ENDIF!]

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

