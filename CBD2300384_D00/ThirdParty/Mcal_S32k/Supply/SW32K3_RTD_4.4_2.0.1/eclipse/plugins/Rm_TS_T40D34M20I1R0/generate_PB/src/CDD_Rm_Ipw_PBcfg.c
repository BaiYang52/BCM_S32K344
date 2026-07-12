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
*   @file CDD_Rm_Ipw_PBcfg.c
*
*   @addtogroup RM_MODULE RM Driver
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
#include "Mcal.h"
#include "Xrdc_Ip.h"
#include "Mpu_M7_Ip.h"
#include "Pflash_Ip.h"
#include "Axbs_Ip.h"
#include "Virt_Wrapper_Ip.h"
#include "Xbic_Ip.h"
#include "CDD_Rm_Ipw_Cfg.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C                     43
#define RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C      4
#define RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C      4
#define RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C   0
#define RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C              2
#define RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C              0
#define RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C              1
/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against CDD_Rm_Ipw_Cfg.h */
#if (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_IPW_CFG_VENDOR_ID)
    #error "CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and CDD_Rm_Ipw_Cfg.h have different vendor ids"
#endif
#if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_IPW_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_IPW_CFG_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_IPW_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and CDD_Rm_Ipw_Cfg.h are different"
#endif
#if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_IPW_CFG_SW_MAJOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_IPW_CFG_SW_MINOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_IPW_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and CDD_Rm_Ipw_Cfg.h are different"
#endif

/* Checks against Xrdc_Ip.h */
#if (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_XRDC_IP_VENDOR_ID)
    #error "CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xrdc_Ip.h have different vendor ids"
#endif
#if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_XRDC_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_XRDC_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_XRDC_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xrdc_Ip.h are different"
#endif
#if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_XRDC_IP_SW_MAJOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_XRDC_IP_SW_MINOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_XRDC_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xrdc_Ip.h are different"
#endif

/* Checks against Mpu_M7_Ip.h */
#if (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_MPU_M7_IP_VENDOR_ID)
    #error "CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Mpu_M7_Ip.h have different vendor ids"
#endif
#if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_MPU_M7_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Mpu_M7_Ip.h are different"
#endif
#if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_MPU_M7_IP_SW_MAJOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_MPU_M7_IP_SW_MINOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_MPU_M7_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Mpu_M7_Ip.h are different"
#endif

/* Checks against Pflash_Ip.h */
#if (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_PFLASH_IP_VENDOR_ID)
    #error "CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Pflash_Ip.h have different vendor ids"
#endif
#if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_PFLASH_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_PFLASH_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_PFLASH_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Pflash_Ip.h are different"
#endif
#if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_PFLASH_IP_SW_MAJOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_PFLASH_IP_SW_MINOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_PFLASH_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Pflash_Ip.h are different"
#endif

/* Checks against Axbs_Ip.h */
#if (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_AXBS_IP_VENDOR_ID)
    #error "CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Axbs_Ip.h have different vendor ids"
#endif
#if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_AXBS_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_AXBS_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_AXBS_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Axbs_Ip.h are different"
#endif
#if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_AXBS_IP_SW_MAJOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_AXBS_IP_SW_MINOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_AXBS_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Axbs_Ip.h are different"
#endif

/* Checks against Virt_Wrapper_Ip.h */
#if (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_VIRT_WRAPPER_IP_VENDOR_ID)
    #error "CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Virt_Wrapper_Ip.h have different vendor ids"
#endif
#if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_VIRT_WRAPPER_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_VIRT_WRAPPER_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_VIRT_WRAPPER_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Virt_Wrapper_Ip.h are different"
#endif
#if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_VIRT_WRAPPER_IP_SW_MAJOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_VIRT_WRAPPER_IP_SW_MINOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_VIRT_WRAPPER_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Virt_Wrapper_Ip.h are different"
#endif

/* Checks against Xbic_Ip.h */
#if (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_VENDOR_ID_C != RM_XBIC_IP_VENDOR_ID)
    #error "CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xbic_Ip.h have different vendor ids"
#endif
#if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C    != RM_XBIC_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C    != RM_XBIC_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_REVISION_VERSION_C != RM_XBIC_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xbic_Ip.h are different"
#endif
#if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MAJOR_VERSION_C != RM_XBIC_IP_SW_MAJOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_MINOR_VERSION_C != RM_XBIC_IP_SW_MINOR_VERSION) || \
     (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_SW_PATCH_VERSION_C != RM_XBIC_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Xbic_Ip.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MAJOR_VERSION_C != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (RM_IPW_PBCFG[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_AR_RELEASE_MINOR_VERSION_C != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of CDD_Rm_Ipw[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]_PBcfg.c and Mcal.h are different"
    #endif
#endif
/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/


/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
#define RM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"

[!IF "RmGeneral/RmEnableSema42Support"!][!//
[!NOCODE!][!//
[!VAR "Rm_Sema42ChannelCount" = "count(RmConfigSet/Sema42_ModuleConfig/Sema42LogicChannelConfiguration/*)"!][!//

[!CODE!][!//
/**
 * @brief       Sema42 Logic Channel Configuration Array
 */
static const Rm_Ipw_Sema42_LogicChannelConfigType Rm_Ipw_Sema42_LogicChannelArray[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][[!"num:i($Rm_Sema42ChannelCount)"!]U] =
{
[!LOOP "RmConfigSet/Sema42_ModuleConfig/Sema42LogicChannelConfiguration/*"!][!//
[!VAR "Rm_Sema42ChannelCount" = "$Rm_Sema42ChannelCount - 1"!][!//
    {
        /* Hardware Instance  */ [!"(./Sema42HardwareInstance)"!],
        /* Hardware Channel   */ [!"substring-after(./Sema42HardwareChannel ,'')"!]U
    }[!IF "$Rm_Sema42ChannelCount != 0"!],[!ENDIF!]
[!ENDLOOP!][!//
};
[!ENDCODE!][!//

[!VAR "Rm_Sema42ChannelCount" = "count(RmConfigSet/Sema42_ModuleConfig/Sema42LogicChannelConfiguration/*)"!][!//
[!CODE!][!//
static const Rm_Ipw_Sema42_ConfigType Rm_Ipw_Sema42_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
    Rm_Ipw_Sema42_LogicChannelArray[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
};

[!ENDCODE!][!//
[!ENDNOCODE!][!//
[!ENDIF!][!//

const Rm_Ipw_ConfigType Rm_Ipw_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!] =
{
[!IF "RmGeneral/RmEnableXRDCSupport"!][!//
    &Xrdc_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
[!ELSE!][!//
    NULL_PTR,
[!ENDIF!][!//
[!IF "RmGeneral/RmEnableSema42Support"!][!//
    &Rm_Ipw_Sema42_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
[!ELSE!][!//
    NULL_PTR,
[!ENDIF!][!//
[!IF "RmGeneral/RmMpuM7Configurable"!][!/*
*/!][!IF "RmGeneral/RmEnableMultiCore"!][!//
    NULL_PTR,
[!ELSE!]    &Mpu_M7_Config,
[!ENDIF!][!/*
*/!][!ELSE!][!//
    NULL_PTR,
[!ENDIF!][!//
[!IF "RmGeneral/RmPflashConfigurable"!][!//
    &Pflash_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!][0],
[!ELSE!][!//
    NULL_PTR,
[!ENDIF!][!//
[!IF "RmGeneral/RmCrossbarConfigurable"!][!//
    &Axbs_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
[!ELSE!][!//
    NULL_PTR,
[!ENDIF!][!//
[!IF "RmGeneral/RmVirtWrapperConfigurable"!][!//
    &Virt_Wrapper_Ip_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!],
[!ELSE!][!//
    NULL_PTR,
[!ENDIF!][!//
[!IF "RmGeneral/RmXbicConfigurable"!][!//
    &Xbic_Config[!IF "var:defined('postBuildVariant')"!]_[!"$postBuildVariant"!][!ENDIF!]
[!ELSE!][!//
    NULL_PTR
[!ENDIF!][!//
};



#define RM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Rm_MemMap.h"

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
