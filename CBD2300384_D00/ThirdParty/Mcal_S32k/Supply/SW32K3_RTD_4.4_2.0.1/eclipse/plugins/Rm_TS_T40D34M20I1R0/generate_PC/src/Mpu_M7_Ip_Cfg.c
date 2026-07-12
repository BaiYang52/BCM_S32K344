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
*   @file Mpu_M7_Ip_Cfg.c
*
*   @addtogroup Mpu_M7_Ip Mpu M7 IPV Driver
*   @{
*/



#ifdef __cplusplus
extern "C"
{
#endif


/*==================================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Mpu_M7_Ip_Cfg.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_MPU_M7_IP_CFG_VENDOR_ID_C                     43
#define RM_MPU_M7_IP_CFG_AR_RELEASE_MAJOR_VERSION_C      4
#define RM_MPU_M7_IP_CFG_AR_RELEASE_MINOR_VERSION_C      4
#define RM_MPU_M7_IP_CFG_AR_RELEASE_REVISION_VERSION_C   0
#define RM_MPU_M7_IP_CFG_SW_MAJOR_VERSION_C              2
#define RM_MPU_M7_IP_CFG_SW_MINOR_VERSION_C              0
#define RM_MPU_M7_IP_CFG_SW_PATCH_VERSION_C              1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Mpu_M7_Ip_Cfg.h */
#if (RM_MPU_M7_IP_CFG_VENDOR_ID_C != RM_MPU_M7_IP_CFG_VENDOR_ID)
    #error "Mpu_M7_Ip_Cfg.c and Mpu_M7_Ip_Cfg.h have different vendor ids"
#endif
#if ((RM_MPU_M7_IP_CFG_AR_RELEASE_MAJOR_VERSION_C    != RM_MPU_M7_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (RM_MPU_M7_IP_CFG_AR_RELEASE_MINOR_VERSION_C    != RM_MPU_M7_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (RM_MPU_M7_IP_CFG_AR_RELEASE_REVISION_VERSION_C != RM_MPU_M7_IP_CFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Mpu_M7_Ip_Cfg.h and Mpu_M7_Ip_Cfg.h are different"
#endif
#if ((RM_MPU_M7_IP_CFG_SW_MAJOR_VERSION_C != RM_MPU_M7_IP_CFG_SW_MAJOR_VERSION) || \
     (RM_MPU_M7_IP_CFG_SW_MINOR_VERSION_C != RM_MPU_M7_IP_CFG_SW_MINOR_VERSION) || \
     (RM_MPU_M7_IP_CFG_SW_PATCH_VERSION_C != RM_MPU_M7_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Mpu_M7_Ip_Cfg.c and Mpu_M7_Ip_Cfg.h are different"
#endif
/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
                                        LOCAL MACROS
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

/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/
[!NOCODE!]
    [!IF "RmGeneral/RmMpuM7Configurable"!] /* System settings */
[!CODE!][!//
#define RM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"

[!IF "RmGeneral/RmEnableMultiCore"!][!//
[!LOOP "RmConfigSet/MPU_M7_Configuration/MPU_M7_ModuleConfig/*"!][!//
static const Mpu_M7_Ip_RegionConfigType [!"@name"!]_RegionConfig[[!"num:i(count(node:current()/RegionConfig/*))"!]] =
{
[!VAR "comma" = "num:i(1)"!][!//
[!VAR "numberofConfig" = "(num:i(count(node:current()/RegionConfig/*)))"!][!//
[!LOOP "node:current()/RegionConfig/*"!][!//
    /* Region Configuration [!"node:current()/@index"!] */
    {
        [!"node:current()/RegionNumber"!]U,
        [!"node:current()/StartAddress"!]UL,
        [!"node:current()/EndAddress"!]UL,
        [!"node:current()/MemoryType"!],
        [!"node:current()/AccessRights"!],
        [!"node:current()/OuterCachePolicy"!],
        [!"node:current()/InnerCachePolicy"!],
        [!"node:current()/SubregionMask"!]U,
        ([!IF "node:current()/Shareable"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!])
    }[!IF "(num:i($comma))< $numberofConfig"!],[!ENDIF!][!CR!][!VAR "comma"="$comma + 1"!][!//
[!ENDLOOP!][!//
};
[!ENDLOOP!]
[!SELECT "RmConfigSet/MPU_M7_Configuration/MPU_M7_ModuleConfig"!][!//
[!LOOP "./*"!][!//
static const Mpu_M7_Ip_ConfigType [!"@name"!] =
{
        [!"./PhysicalCoreID"!],
        ([!IF "node:current()/DefaultMapEnable"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!]),
        ([!IF "node:current()/RunInHFNMIEnable"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!]),
        ([!IF "node:current()/MemManageInterruptEnable"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!]),
        [!"num:i(count(node:current()/RegionConfig/*))"!],
        [!"@name"!]_RegionConfig
};
[!ENDLOOP!]
[!ENDSELECT!][!//
[!SELECT "RmConfigSet/MPU_M7_Configuration/MPU_M7_ModuleConfig"!][!//
[!VAR "comma" = "num:i(1)"!][!//
[!VAR "numberofConfig" = "(num:i(count(./*)))"!][!//
const Mpu_M7_Ip_ConfigType * const Mpu_M7_Gather_Config[[!"num:i(count(./*))"!]] =
{
[!LOOP "./*"!][!//
    &[!"@name"!][!IF "(num:i($comma))< $numberofConfig"!],[!ENDIF!][!CR!][!VAR "comma"="$comma + 1"!][!//
[!ENDLOOP!][!//
};
[!ENDSELECT!][!//
[!ELSE!][!//
[!//
[!SELECT "RmConfigSet/MPU_M7_Configuration/MPU_M7_ModuleConfig/*[1]"!][!//
static const Mpu_M7_Ip_RegionConfigType [!"@name"!]_RegionConfig[[!"num:i(count(node:current()/RegionConfig/*))"!]] =
{
[!VAR "comma" = "num:i(1)"!][!//
[!VAR "numberofConfig" = "(num:i(count(node:current()/RegionConfig/*)))"!][!//
[!LOOP "node:current()/RegionConfig/*"!][!//
    /* Region Configuration [!"node:current()/@index"!] */
    {
        [!"node:current()/RegionNumber"!]U,
        [!"node:current()/StartAddress"!]UL,
        [!"node:current()/EndAddress"!]UL,
        [!"node:current()/MemoryType"!],
        [!"node:current()/AccessRights"!],
        [!"node:current()/OuterCachePolicy"!],
        [!"node:current()/InnerCachePolicy"!],
        [!"node:current()/SubregionMask"!]U,
        ([!IF "node:current()/Shareable"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!])
    }[!IF "(num:i($comma))< $numberofConfig"!],[!ENDIF!][!CR!][!VAR "comma"="$comma + 1"!][!//
[!ENDLOOP!][!//
};

const Mpu_M7_Ip_ConfigType Mpu_M7_Config =
{
        [!"PhysicalCoreID"!],
        ([!IF "DefaultMapEnable"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!]),
        ([!IF "RunInHFNMIEnable"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!]),
        ([!IF "MemManageInterruptEnable"!](boolean)TRUE[!ELSE!](boolean)FALSE[!ENDIF!]),
        [!"num:i(count(node:current()/RegionConfig/*))"!],
        [!"@name"!]_RegionConfig
};
[!ENDSELECT!][!//
[!ENDIF!][!//

#define RM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"
[!ENDCODE!]
    [!ENDIF!]
[!ENDNOCODE!]
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

