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

#ifndef PFLASH_IP_TYPES_H
#define PFLASH_IP_TYPES_H

/**
*   @file Pflash_Ip_Types.h
*
*   @addtogroup Pflash_Ip Pflash IPV Driver
*   @{
*/


#ifdef __cplusplus
extern "C"{
#endif

/*===============================================================================================
                                         INCLUDE FILES
 1) system and project includes
 2) needed interfaces from external units
 3) internal and external interfaces from this unit
===============================================================================================*/
#include "Mcal.h"

/*===============================================================================================
                               SOURCE FILE VERSION INFORMATION
===============================================================================================*/
#define RM_PFLASH_IP_TYPES_VENDOR_ID                        43
#define RM_PFLASH_IP_TYPES_AR_RELEASE_MAJOR_VERSION         4
#define RM_PFLASH_IP_TYPES_AR_RELEASE_MINOR_VERSION         4
#define RM_PFLASH_IP_TYPES_AR_RELEASE_REVISION_VERSION      0
#define RM_PFLASH_IP_TYPES_SW_MAJOR_VERSION                 2
#define RM_PFLASH_IP_TYPES_SW_MINOR_VERSION                 0
#define RM_PFLASH_IP_TYPES_SW_PATCH_VERSION                 1

/*===============================================================================================
                                      FILE VERSION CHECKS
===============================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((RM_PFLASH_IP_TYPES_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (RM_PFLASH_IP_TYPES_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Pflash_Ip_Types.h and Mcal.h are different"
    #endif
#endif

/*===============================================================================================
                                           CONSTANTS
===============================================================================================*/


/*===============================================================================================
                                       DEFINES AND MACROS
===============================================================================================*/
/**
* @brief          Enumeration listing Pflash master access protection field
*/
typedef enum
{
    PFLASH_M15AP=0,
    PFLASH_M14AP,
    PFLASH_M13AP,
    PFLASH_M12AP,
    PFLASH_M11AP,
    PFLASH_M10AP,
    PFLASH_M9AP,
    PFLASH_M8AP,
    PFLASH_M7AP,
    PFLASH_M6AP,
    PFLASH_M5AP,
    PFLASH_M4AP,
    PFLASH_M3AP,
    PFLASH_M2AP,
    PFLASH_M1AP,
    PFLASH_M0AP
} Pflash_Ip_MasterType;

/**
* @brief          Enumeration listing Pflash access type
*/
typedef enum
{
    PFLASH_READ_NOT_ALLOWED=0,
    PFLASH_READ_ALLOWED=1
} Pflash_Ip_MasterAccessType;

/*==================================================================================================
*                                  STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
/**
* @brief          Structure containing the configuration of the Pflash Ip
*/
typedef struct
{
    const uint8                     u32Pflash_Instance;  /*!< Pflash instance */
    Pflash_Ip_MasterType            Pflash_Map;          /*!< Pflash master access protection field */
    Pflash_Ip_MasterAccessType      Pflash_Access;       /*!< Pflash access type */
} Pflash_Ip_ConfigType;

/*===============================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
===============================================================================================*/

/*===============================================================================================
*                                     FUNCTION PROTOTYPES
===============================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* PFLASH_IP_TYPES_H */

