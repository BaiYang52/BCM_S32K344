/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : FLEXIO
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

#ifndef UART_CFG_H
#define UART_CFG_H

/**
*   @file
*
*   @defgroup uart_driver UART Driver
*   @addtogroup  uart_driver UART Driver
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
#include "Mcal.h"
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!]#include "Uart_[!"."!]_PBcfg.h"
[!ENDLOOP!]
[!ELSE!][!//
#include "Uart_PBcfg.h"[!ENDIF!][!//
[!NOCODE!][!//
[!INCLUDE "Uart_VersionCheck_Inc.m"!][!//
[!ENDNOCODE!][!//
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define UART_CFG_VENDOR_ID                    43
#define UART_CFG_AR_RELEASE_MAJOR_VERSION     4
#define UART_CFG_AR_RELEASE_MINOR_VERSION     4
#define UART_CFG_AR_RELEASE_REVISION_VERSION  0
#define UART_CFG_SW_MAJOR_VERSION             2
#define UART_CFG_SW_MINOR_VERSION             0
#define UART_CFG_SW_PATCH_VERSION             1
/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!][!//
/* Checks against Uart_[!"."!]_PBcfg.h */
#if (UART_CFG_VENDOR_ID != UART_PBCFG_[!"."!]_VENDOR_ID)
    #error "Uart_Cfg.h and Uart_[!"."!]_PBcfg.h have different vendor ids"
#endif
#if ((UART_CFG_AR_RELEASE_MAJOR_VERSION    != UART_PBCFG_[!"."!]_AR_RELEASE_MAJOR_VERSION) || \
     (UART_CFG_AR_RELEASE_MINOR_VERSION    != UART_PBCFG_[!"."!]_AR_RELEASE_MINOR_VERSION) || \
     (UART_CFG_AR_RELEASE_REVISION_VERSION != UART_PBCFG_[!"."!]_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Uart_Cfg.h and Uart_[!"."!]_PBcfg.h are different"
#endif
#if ((UART_CFG_SW_MAJOR_VERSION != UART_PBCFG_[!"."!]_SW_MAJOR_VERSION) || \
     (UART_CFG_SW_MINOR_VERSION != UART_PBCFG_[!"."!]_SW_MINOR_VERSION) || \
     (UART_CFG_SW_PATCH_VERSION != UART_PBCFG_[!"."!]_SW_PATCH_VERSION))
    #error "Software Version Numbers of Uart_Cfg.h and Uart_[!"."!]_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!][!//
/* Checks against Uart_PBcfg.h */
#if (UART_CFG_VENDOR_ID != UART_PBCFG_VENDOR_ID)
    #error "Uart_Cfg.h and Uart_PBcfg.h have different vendor ids"
#endif
#if ((UART_CFG_AR_RELEASE_MAJOR_VERSION    != UART_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (UART_CFG_AR_RELEASE_MINOR_VERSION    != UART_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (UART_CFG_AR_RELEASE_REVISION_VERSION != UART_PBCFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Uart_Cfg.h and Uart_PBcfg.h are different"
#endif
#if ((UART_CFG_SW_MAJOR_VERSION != UART_PBCFG_SW_MAJOR_VERSION) || \
     (UART_CFG_SW_MINOR_VERSION != UART_PBCFG_SW_MINOR_VERSION) || \
     (UART_CFG_SW_PATCH_VERSION != UART_PBCFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Uart_Cfg.h and Uart_PBcfg.h are different"
#endif
[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((UART_CFG_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (UART_CFG_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Uart_Cfg.h and Mcal.h are different"
    #endif
#endif
/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
[!IF "not(IMPLEMENTATION_CONFIG_VARIANT !='VariantPostBuild' and (variant:size()<=1))"!][!//
#define UART_CONFIG_EXT \
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
    UART_CONFIG_[!"."!]_PB \
[!ENDLOOP!][!//
[!ELSE!][!//
    UART_CONFIG_PB
[!ENDIF!]
[!ENDIF!]
/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                    FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                  EXTERNAL CONSTANTS
==================================================================================================*/

#ifdef __cplusplus
}
#endif
/** @} */
#endif /* UART_CFG_H */
