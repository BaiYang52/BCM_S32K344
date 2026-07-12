[!CODE!][!//
/**
*   @file    Os_cfg.h
*   @version 2.0.1
*
*   @brief   AUTOSAR Os - Project configuration.
*   @details Header file describing all Autosar MCAL Os_cfg present in the current project.
*
*   @addtogroup BASE_COMPONENT
*   @{
*/
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : generic
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
==================================================================================================*/
/*==================================================================================================
==================================================================================================*/
/**
* @file        Os_cfg.h
*/
/** 
* @page misra_violations MISRA-C:2004 violations
*
* @section [global]
*          Violates MISRA 2004 Required Rule 5.1, Identifiers (internal and external) shall not rely 
*          on the significance of more than 31 characters. The used compilers use more than 31 chars for
*          identifiers.
*/

#ifndef OS_CFG_H
#define OS_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
/** 
* @file        Os_cfg.h
* @brief Include Standard types & defines
*/
#include "Std_Types.h"

/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/

/** 
@{
* @brief Parameters that shall be published within the Os_cfg header file.
*       The integration of incompatible files shall be avoided.
*/
#define OS_CFG_VENDOR_ID                       43
#define OS_CFG_MODULE_ID                       0x01
#define OS_CFG_AR_RELEASE_MAJOR_VERSION_H      4
#define OS_CFG_AR_RELEASE_MINOR_VERSION_H      4
#define OS_CFG_AR_RELEASE_REVISION_VERSION_H   0
#define OS_CFG_SW_MAJOR_VERSION_H              2
#define OS_CFG_SW_MINOR_VERSION_H              0
#define OS_CFG_SW_PATCH_VERSION_H              1
/**@}*/
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if source file and Std_Types.h header file are of the same Autosar version */
    #if ((OS_CFG_AR_RELEASE_MAJOR_VERSION_H != STD_AR_RELEASE_MAJOR_VERSION) || \
         (OS_CFG_AR_RELEASE_MINOR_VERSION_H != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Os_cfg.h and Std_Types.h are different"
    #endif
#endif

/*==================================================================================================
*                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       DEFINES AND MACROS
==================================================================================================*/
[!LOOP "OsCounter/*"!][!//
    #define [!"@name"!] [!WS!] [!"@index"!][!CR!][!//
[!IF "node:exists(OsSecondsPerTick)"!][!//
    #define OS_TICKS2NS_[!"@name"!](x)[!WS!][!"(num:i(OsSecondsPerTick * 1000000000))"!]U [!CR!][!//    
    #define OS_TICKS2US_[!"@name"!](x)[!WS!][!"(num:i(OsSecondsPerTick * 1000000))"!]U [!CR!][!//
[!ELSE!][!//
    #define OS_TICKS2NS_[!"@name"!](x)[!WS!][!"num:i(10000)"!]U [!CR!][!//
    #define OS_TICKS2US_[!"@name"!](x)[!WS!][!"num:i(10)"!]U [!CR!][!//
[!ENDIF!][!//
[!ENDLOOP!]

/*==================================================================================================
*                                             ENUMS
==================================================================================================*/

/*==================================================================================================
*                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/*==================================================================================================
*                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
*                                     FUNCTION PROTOTYPES
==================================================================================================*/


#ifdef __cplusplus
}
#endif

#endif /* OS_CFG_H */

/** @} */

[!ENDCODE!][!//

