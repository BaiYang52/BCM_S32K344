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
*   @file Virt_Wrapper_Ip_PBcfg.c
*
*   @addtogroup Virt_Wrapper_Ip Virt Wrapper IPV Driver
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
#include "Virt_Wrapper_Ip_Types.h"
#include "Virt_Wrapper_Ip_Cfg.h"
#include "Virt_Wrapper_Ip_Device_Registers.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C                     43
#define RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C      4
#define RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C      4
#define RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C   0
#define RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C              2
#define RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C              0
#define RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Virt_Wrapper_Ip_Types.h */
#if (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_VIRT_WRAPPER_IP_TYPES_VENDOR_ID)
    #error "Virt_Wrapper_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Virt_Wrapper_Ip_Types.h have different vendor ids"
#endif
#if ((RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_VIRT_WRAPPER_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_VIRT_WRAPPER_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_VIRT_WRAPPER_IP_TYPES_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Virt_Wrapper_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Virt_Wrapper_Ip_Types.h are different"
#endif
#if ((RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_VIRT_WRAPPER_IP_TYPES_SW_MAJOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_VIRT_WRAPPER_IP_TYPES_SW_MINOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_VIRT_WRAPPER_IP_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Virt_Wrapper_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Virt_Wrapper_Ip_Types.h are different"
#endif

/* Checks against Virt_Wrapper_Ip_Cfg.h */
#if (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_VIRT_WRAPPER_IP_CFG_VENDOR_ID)
    #error "Virt_Wrapper_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Virt_Wrapper_Ip_Cfg.h have different vendor ids"
#endif
#if ((RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_VIRT_WRAPPER_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_VIRT_WRAPPER_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_VIRT_WRAPPER_IP_CFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Virt_Wrapper_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Virt_Wrapper_Ip_Cfg.h are different"
#endif
#if ((RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_VIRT_WRAPPER_IP_CFG_SW_MAJOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_VIRT_WRAPPER_IP_CFG_SW_MINOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_VIRT_WRAPPER_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Virt_Wrapper_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Virt_Wrapper_Ip_Cfg.h are different"
#endif

/* Checks against Virt_Wrapper_Ip_Device_Registers.h */
#if (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_VIRT_WRAPPER_IP_DEVICE_REGISTERS_VENDOR_ID)
    #error "Virt_Wrapper_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Virt_Wrapper_Ip_Device_Registers.h have different vendor ids"
#endif
#if ((RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_VIRT_WRAPPER_IP_DEVICE_REGISTERS_AR_RELEASE_MAJOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_VIRT_WRAPPER_IP_DEVICE_REGISTERS_AR_RELEASE_MINOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_VIRT_WRAPPER_IP_DEVICE_REGISTERS_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Virt_Wrapper_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Virt_Wrapper_Ip_Device_Registers.h are different"
#endif
#if ((RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_VIRT_WRAPPER_IP_DEVICE_REGISTERS_SW_MAJOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_VIRT_WRAPPER_IP_DEVICE_REGISTERS_SW_MINOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_VIRT_WRAPPER_IP_DEVICE_REGISTERS_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Virt_Wrapper_Ip[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Virt_Wrapper_Ip_Device_Registers.h are different"
#endif

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
[!NOCODE!]
[!VAR "numOfConfigs" = "num:i(num:i(count(RmConfigSet/Virt_Wrapper_Configuration/Mscr_Config_List/*)) + num:i(count(RmConfigSet/Virt_Wrapper_Configuration/Imcr_Config_List/*)) + num:i(count(RmConfigSet/Virt_Wrapper_Configuration/Other_Config_List/*)))"!]
[!ENDNOCODE!]
[!CODE!][!//
#define NUM_OF_CONFIGS[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] ([!"$numOfConfigs"!]U)
[!ENDCODE!]
/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/
[!IF "RmGeneral/RmVirtWrapperConfigurable"!]

#define RM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"

const Virt_Wrapper_Ip_RegConfigType Virt_Wrapper_Ip_RegConfig[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][NUM_OF_CONFIGS[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]] =
{
[!NOCODE!]
[!VAR "comma"="1"!]
[!LOOP "RmConfigSet/Virt_Wrapper_Configuration/Mscr_Config_List/*"!]
[!CODE!][!//
[!WS8!]   {
[!ENDCODE!][!//
[!VAR "PAD" = "node:value(./MscrName)"!]
[!VAR "MscrNumber" = "node:value(./MscrNumber)"!]
[!VAR "Siul2Instance" = "substring-after(node:value(./MscrSiul2Instance),'SIUL2_')"!]
[!VAR "Mirror" = "node:value(./MscrMirror)"!]
[!VAR "Index" = "./@index"!]
[!VAR "ActualMirror"="'VIRT_WRAPPER_MAP_PDAC0'"!]
[!LOOP "text:split($Mirror, '_')"!]
    [!IF "starts-with(., 'PDAC')"!]
        [!VAR "ActualMirror" = "concat('VIRT_WRAPPER_MAP_', .)"!]
    [!ENDIF!]
[!ENDLOOP!]
[!CODE!][!WS16!]      /* [!"$PAD"!] */
[!WS16!]      [!"$ActualMirror"!],
[!WS16!]      [!"$Siul2Instance"!]UL,
[!WS16!]      [!"$MscrNumber"!]UL,
[!WS16!]      VIRT_WRAPPER_REG_TYPE_A
[!ENDCODE!]
[!VAR "comma"="$comma + 1"!]
[!CODE!][!//
[!WS8!]   },
[!ENDCODE!][!//
[!ENDLOOP!]
[!LOOP "RmConfigSet/Virt_Wrapper_Configuration/Imcr_Config_List/*"!]
[!CODE!][!//
[!WS8!]   {
[!ENDCODE!][!//
[!VAR "Imcr" = "node:value(./ImcrName)"!]
[!VAR "ImcrNumber" = "node:value(./ImcrNumber)"!]
[!VAR "Siul2Instance" = "substring-after(node:value(./ImcrSiul2Instance),'SIUL2_')"!]
[!VAR "Mirror" = "node:value(./ImcrMirror)"!]
[!VAR "ActualMirror"="'VIRT_WRAPPER_MAP_PDAC0'"!]
[!LOOP "text:split($Mirror, '_')"!]
    [!IF "starts-with(., 'PDAC')"!]
        [!VAR "ActualMirror" = "concat('VIRT_WRAPPER_MAP_', .)"!]
    [!ENDIF!]
[!ENDLOOP!]
[!VAR "Index" = "./@index"!]
[!CODE!][!WS16!]      /* [!"$Imcr"!] */
[!WS16!]      [!"$ActualMirror"!],
[!WS16!]      [!"$Siul2Instance"!]UL,
[!WS16!]      [!"$ImcrNumber"!]UL,
[!WS16!]      VIRT_WRAPPER_REG_TYPE_B
[!ENDCODE!]
[!VAR "comma"="$comma + 1"!]
[!CODE!][!//
[!WS8!]   }[!IF "num:i($comma) <= num:i($numOfConfigs)"!],[!ENDIF!]
[!ENDCODE!][!//
[!ENDLOOP!]
[!LOOP "RmConfigSet/Virt_Wrapper_Configuration/Other_Config_List/*"!]
[!CODE!][!//
[!WS8!]   {
[!ENDCODE!][!//
[!VAR "OtherRegName" = "node:value(./OtherName)"!]
[!VAR "OtherNumber" = "node:value(./OtherNumber)"!]
[!VAR "Siul2Instance" = "substring-after(node:value(./OthersSiul2Instance),'SIUL2_')"!]
[!VAR "Mirror" = "node:value(./OtherMirror)"!]
[!VAR "ActualMirror"="'VIRT_WRAPPER_MAP_PDAC0'"!]
[!LOOP "text:split($Mirror, '_')"!]
    [!IF "starts-with(., 'PDAC')"!]
        [!VAR "ActualMirror" = "concat('VIRT_WRAPPER_MAP_', .)"!]
    [!ENDIF!]
[!ENDLOOP!]
[!VAR "Index" = "./@index"!]
[!CODE!][!WS16!]      /* [!"$OtherRegName"!] */
[!WS16!]      [!"$ActualMirror"!],
[!WS16!]      [!"$Siul2Instance"!]UL,
[!WS16!]      [!"$OtherNumber"!]UL,
[!WS16!]      [!"ecu:list('Rm.VirtWrapper.OthersRegType.List')[position()=$Index+1]"!]
[!ENDCODE!]
[!VAR "comma"="$comma + 1"!]
[!CODE!][!//
[!WS8!]   }[!IF "num:i($comma) <= num:i($numOfConfigs)"!],[!ENDIF!]
[!ENDCODE!][!//
[!ENDLOOP!]
[!ENDNOCODE!][!//
};

const Virt_Wrapper_Ip_ConfigType Virt_Wrapper_Ip_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    NUM_OF_CONFIGS[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
    Virt_Wrapper_Ip_RegConfig[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
};
#define RM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"

[!ENDIF!]
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/


#ifdef __cplusplus
}
#endif

/** @} */

