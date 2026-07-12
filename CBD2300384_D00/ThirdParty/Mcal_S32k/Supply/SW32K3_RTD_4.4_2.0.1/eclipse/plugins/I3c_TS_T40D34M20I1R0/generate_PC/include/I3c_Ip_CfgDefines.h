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

#ifndef I3C_IP_CFGDEFINES_H
#define I3C_IP_CFGDEFINES_H

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
#include "StandardTypes.h"
#include "OsIf.h"
#include "[!"ecu:get('I3c.I3cHeaderName')"!].h"
[!IF "GeneralConfiguration/I3cDevErrorDetect"!][!//
#include "Devassert.h"
[!ENDIF!][!//
[!NOCODE!][!//
[!//INCLUDE "Adc_VersionCheck_Src.m"!][!// TODO: add version checking
[!INCLUDE "I3c_RegOperations.m"!][!//
[!ENDNOCODE!][!//

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define I3C_IP_VENDOR_ID_CFGDEFINES                    43
#define I3C_IP_AR_RELEASE_MAJOR_VERSION_CFGDEFINES     4
#define I3C_IP_AR_RELEASE_MINOR_VERSION_CFGDEFINES     4
#define I3C_IP_AR_RELEASE_REVISION_VERSION_CFGDEFINES  0
#define I3C_IP_SW_MAJOR_VERSION_CFGDEFINES             2
#define I3C_IP_SW_MINOR_VERSION_CFGDEFINES             0
#define I3C_IP_SW_PATCH_VERSION_CFGDEFINES             1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if current file and StandardTypes header file are of the same Autosar version */
#if ((I3C_IP_AR_RELEASE_MAJOR_VERSION_CFGDEFINES     != STD_AR_RELEASE_MAJOR_VERSION) || \
     (I3C_IP_AR_RELEASE_MINOR_VERSION_CFGDEFINES     != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of I3c_Ip_CfgDefines.h and StandardTypes.h are different"
#endif

/* Check if current file and OsIf header file are of the same Autosar version */
#if ((I3C_IP_AR_RELEASE_MAJOR_VERSION_CFGDEFINES     != OSIF_AR_RELEASE_MAJOR_VERSION) || \
     (I3C_IP_AR_RELEASE_MINOR_VERSION_CFGDEFINES     != OSIF_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of I3c_Ip_CfgDefines.h and OsIf.h are different"
#endif
[!IF "GeneralConfiguration/I3cDevErrorDetect"!][!//

/* Check if current file and Devassert.h header file are of the same Autosar version */
#if ((I3C_IP_AR_RELEASE_MAJOR_VERSION_CFGDEFINES != DEVASSERT_AR_RELEASE_MAJOR_VERSION) || \
     (I3C_IP_AR_RELEASE_MINOR_VERSION_CFGDEFINES != DEVASSERT_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of I3c_Ip_CfgDefines.h and Devassert.h are different"
#endif
[!ENDIF!][!//
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */

/*==================================================================================================
*                                            CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/

[!CALL "I3c_Ip_ConfigDef"!][!//

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

#endif /* I3C_IP_CFGDEFINES_H */
