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

#ifndef MPU_M7_IP_CFG_H
#define MPU_M7_IP_CFG_H

/**
*   @file Mpu_M7_Ip_Cfg.h
*
*   @addtogroup Mpu_M7_Ip Mpu M7 IPV Driver
*   @{
*/


/*==================================================================================================
                                         INCLUDE FILES
==================================================================================================*/
#include "Mpu_M7_Ip_Types.h"
#include "Mpu_M7_Ip_Cfg_Defines.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_MPU_M7_IP_CFG_VENDOR_ID                    43
#define RM_MPU_M7_IP_CFG_AR_RELEASE_MAJOR_VERSION     4
#define RM_MPU_M7_IP_CFG_AR_RELEASE_MINOR_VERSION     4
#define RM_MPU_M7_IP_CFG_AR_RELEASE_REVISION_VERSION  0
#define RM_MPU_M7_IP_CFG_SW_MAJOR_VERSION             2
#define RM_MPU_M7_IP_CFG_SW_MINOR_VERSION             0
#define RM_MPU_M7_IP_CFG_SW_PATCH_VERSION             1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Mpu_M7_Ip_Types.h */
#if (RM_MPU_M7_IP_CFG_VENDOR_ID != RM_MPU_M7_IP_TYPES_VENDOR_ID)
    #error "Mpu_M7_Ip_Cfg.h and Mpu_M7_Ip_Types.h have different vendor ids"
#endif
#if (( RM_MPU_M7_IP_CFG_AR_RELEASE_MAJOR_VERSION    != RM_MPU_M7_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     ( RM_MPU_M7_IP_CFG_AR_RELEASE_MINOR_VERSION    != RM_MPU_M7_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     ( RM_MPU_M7_IP_CFG_AR_RELEASE_REVISION_VERSION != RM_MPU_M7_IP_TYPES_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Mpu_M7_Ip_Cfg.h and Mpu_M7_Ip_Types.h are different"
#endif
#if (( RM_MPU_M7_IP_CFG_SW_MAJOR_VERSION != RM_MPU_M7_IP_TYPES_SW_MAJOR_VERSION) || \
     ( RM_MPU_M7_IP_CFG_SW_MINOR_VERSION != RM_MPU_M7_IP_TYPES_SW_MINOR_VERSION) || \
     ( RM_MPU_M7_IP_CFG_SW_PATCH_VERSION != RM_MPU_M7_IP_TYPES_SW_PATCH_VERSION))
    #error "Software Version Numbers of Mpu_M7_Ip_Cfg.h and Mpu_M7_Ip_Types.h are different"
#endif

/* Checks against Mpu_M7_Ip_Cfg_Defines.h */
#if (RM_MPU_M7_IP_CFG_VENDOR_ID != RM_MPU_M7_IP_CFG_DEFINES_VENDOR_ID)
    #error "Mpu_M7_Ip_Cfg.h and Mpu_M7_Ip_Cfg_Defines.h have different vendor ids"
#endif
#if (( RM_MPU_M7_IP_CFG_AR_RELEASE_MAJOR_VERSION    != RM_MPU_M7_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION) || \
     ( RM_MPU_M7_IP_CFG_AR_RELEASE_MINOR_VERSION    != RM_MPU_M7_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION) || \
     ( RM_MPU_M7_IP_CFG_AR_RELEASE_REVISION_VERSION != RM_MPU_M7_IP_CFG_DEFINES_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Mpu_M7_Ip_Cfg.h and Mpu_M7_Ip_Cfg_Defines.h are different"
#endif
#if (( RM_MPU_M7_IP_CFG_SW_MAJOR_VERSION != RM_MPU_M7_IP_CFG_DEFINES_SW_MAJOR_VERSION) || \
     ( RM_MPU_M7_IP_CFG_SW_MINOR_VERSION != RM_MPU_M7_IP_CFG_DEFINES_SW_MINOR_VERSION) || \
     ( RM_MPU_M7_IP_CFG_SW_PATCH_VERSION != RM_MPU_M7_IP_CFG_DEFINES_SW_PATCH_VERSION))
    #error "Software Version Numbers of Mpu_M7_Ip_Cfg.h and Mpu_M7_Ip_Cfg_Defines.h are different"
#endif

/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/

[!NOCODE!][!//
    [!IF "RmGeneral/RmMpuM7Configurable"!] /* System settings */
[!CODE!][!//
#define RM_IP_ENABLE_MPU_M7                               (STD_ON)
#define MPU_M7_IP_MULTICORE_SUPPORT                       ([!IF "(RmGeneral/RmEnableMultiCore = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

[!SELECT "RmConfigSet/MPU_M7_Configuration/MPU_M7_ModuleConfig"!][!//
#define MPU_M7_IP_NUM_CONFIG_U32                          ([!"num:i(count(./*))"!]U)
[!ENDSELECT!][!//

#define RM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"

/** @brief    Declaration of the configuration structure for Mpu_M7 Ip */
[!IF "RmGeneral/RmEnableMultiCore"!][!//
[!SELECT "RmConfigSet/MPU_M7_Configuration/MPU_M7_ModuleConfig"!][!//
extern const Mpu_M7_Ip_ConfigType * const Mpu_M7_Gather_Config[[!"num:i(count(./*))"!]];
[!ENDSELECT!][!//
[!ELSE!][!//
extern const Mpu_M7_Ip_ConfigType Mpu_M7_Config;
[!ENDIF!][!//

#define RM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"
[!ENDCODE!]
    [!ENDIF!]
[!ENDNOCODE!]

/** @} */

#endif /* MPU_M7_IP_CFG_H */

