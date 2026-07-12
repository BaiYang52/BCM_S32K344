[!CODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : I3C
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

#ifndef I3C_IP_CFG_H
#define I3C_IP_CFG_H

/**
*   @file
*
*   @addtogroup i3c_ip_config I3c IPL Configuration
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
[!NOCODE!][!//
[!IF "var:defined('postBuildVariant')"!]
[!LOOP "variant:all()"!]
[!CODE!]#include "I3c_Ip_[!"."!]_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDLOOP!]
[!ELSE!]
[!CODE!]#include "I3c_Ip_PBcfg.h"[!CR!][!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!][!//

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define I3C_IP_VENDOR_ID_CFG                       43
#define I3C_IP_AR_RELEASE_MAJOR_VERSION_CFG        4
#define I3C_IP_AR_RELEASE_MINOR_VERSION_CFG        4
#define I3C_IP_AR_RELEASE_REVISION_VERSION_CFG     0
#define I3C_IP_SW_MAJOR_VERSION_CFG                2
#define I3C_IP_SW_MINOR_VERSION_CFG                0
#define I3C_IP_SW_PATCH_VERSION_CFG                1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
/* Check if current file and I3c_Ip_[!"."!]_PBcfg.h file are of the same vendor */
#if (I3C_IP_VENDOR_ID_CFG != I3C_IP_VENDOR_ID_[!"text:toupper(.)"!]_PBCFG)
    #error "I3c_Ip_Cfg.h and I3c_Ip_[!"."!]_PBcfg.h have different vendor IDs"
#endif

/* Check if current file and I3c_Ip_[!"."!]_PBcfg.h file are of the same Autosar version */
#if ((I3C_IP_AR_RELEASE_MAJOR_VERSION_CFG     != I3C_IP_AR_RELEASE_MAJOR_VERSION_[!"text:toupper(.)"!]_PBCFG) || \
     (I3C_IP_AR_RELEASE_MINOR_VERSION_CFG     != I3C_IP_AR_RELEASE_MINOR_VERSION_[!"text:toupper(.)"!]_PBCFG) || \
     (I3C_IP_AR_RELEASE_REVISION_VERSION_CFG  != I3C_IP_AR_RELEASE_REVISION_VERSION_[!"text:toupper(.)"!]_PBCFG))
    #error "AutoSar Version Numbers of I3c_Ip_Cfg.h and I3c_Ip_[!"."!]_PBcfg.h are different"
#endif

/* Check if current file and I3c_Ip_[!"."!]_PBcfg.h file are of the same software version */
#if ((I3C_IP_SW_MAJOR_VERSION_CFG != I3C_IP_SW_MAJOR_VERSION_[!"text:toupper(.)"!]_PBCFG) || \
     (I3C_IP_SW_MINOR_VERSION_CFG != I3C_IP_SW_MINOR_VERSION_[!"text:toupper(.)"!]_PBCFG) || \
     (I3C_IP_SW_PATCH_VERSION_CFG != I3C_IP_SW_PATCH_VERSION_[!"text:toupper(.)"!]_PBCFG))
    #error "Software Version Numbers of I3c_Ip_Cfg.h and I3c_Ip_[!"."!]_PBcfg.h are different"
#endif
[!ENDLOOP!][!//
[!ELSE!][!//
/* Check if current file and I3c_Ip_PBcfg.h file are of the same vendor */
#if (I3C_IP_VENDOR_ID_CFG != I3C_IP_VENDOR_ID_PBCFG)
    #error "I3c_Ip_Cfg.h and I3c_Ip_PBcfg.h have different vendor IDs"
#endif

/* Check if current file and I3c_Ip_PBcfg.h file are of the same Autosar version */
#if ((I3C_IP_AR_RELEASE_MAJOR_VERSION_CFG != I3C_IP_AR_RELEASE_MAJOR_VERSION_PBCFG) || \
     (I3C_IP_AR_RELEASE_MINOR_VERSION_CFG != I3C_IP_AR_RELEASE_MINOR_VERSION_PBCFG) || \
     (I3C_IP_AR_RELEASE_REVISION_VERSION_CFG != I3C_IP_AR_RELEASE_REVISION_VERSION_PBCFG))
    #error "AutoSar Version Numbers of I3c_Ip_Cfg.h and I3c_Ip_PBcfg.h are different"
#endif

/* Check if current file and I3c_Ip_PBcfg.h file are of the same software version */
#if ((I3C_IP_SW_MAJOR_VERSION_CFG != I3C_IP_SW_MAJOR_VERSION_PBCFG) || \
     (I3C_IP_SW_MINOR_VERSION_CFG != I3C_IP_SW_MINOR_VERSION_PBCFG) || \
     (I3C_IP_SW_PATCH_VERSION_CFG != I3C_IP_SW_PATCH_VERSION_PBCFG))
    #error "Software Version Numbers of I3c_Ip_Cfg.h and I3c_Ip_PBcfg.h are different"
#endif
[!ENDIF!][!//

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

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

/** @} */

#endif /* I3C_IP_CFG_H */
[!ENDCODE!][!//