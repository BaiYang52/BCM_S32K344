/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : LPI2C
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
*   @file
*
*   @addtogroup I2C_DRIVER_CONFIGURATION I2c Driver Configurations
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
#include "CDD_I2c.h"

#if (I2C_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
#include "Dem.h"
#endif /* (I2C_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF) */

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define I2C_CFG_VENDOR_ID_C                     43

#define I2C_CFG_AR_RELEASE_MAJOR_VERSION_C      4

#define I2C_CFG_AR_RELEASE_MINOR_VERSION_C      4

#define I2C_CFG_AR_RELEASE_REVISION_VERSION_C   0
#define I2C_CFG_SW_MAJOR_VERSION_C              2
#define I2C_CFG_SW_MINOR_VERSION_C              0
#define I2C_CFG_SW_PATCH_VERSION_C              1


/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
/**
* @brief            The I2c module's implementer shall avoid the integration of incompatible files.
*/
/* Check if current file and I2c header file are of the same vendor */
#if (I2C_CFG_VENDOR_ID_C != I2C_VENDOR_ID)
    #error "CDD_I2c_Cfg.c and CDD_I2c.h have different vendor ids"
#endif

/* Check if current file and I2c header file are of the same Autosar version */
#if ((I2C_CFG_AR_RELEASE_MAJOR_VERSION_C != I2C_AR_RELEASE_MAJOR_VERSION) ||    \
     (I2C_CFG_AR_RELEASE_MINOR_VERSION_C != I2C_AR_RELEASE_MINOR_VERSION) ||    \
     (I2C_CFG_AR_RELEASE_REVISION_VERSION_C != I2C_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of CDD_I2c_Cfg.c and CDD_I2c.h are different"
#endif

/* Check if current file and I2c header file are of the same Software version */
#if ((I2C_CFG_SW_MAJOR_VERSION_C != I2C_SW_MAJOR_VERSION) || \
     (I2C_CFG_SW_MINOR_VERSION_C != I2C_SW_MINOR_VERSION) || \
     (I2C_CFG_SW_PATCH_VERSION_C != I2C_SW_PATCH_VERSION)    \
    )
    #error "Software Version Numbers of CDD_I2c_Cfg.c and CDD_I2c.h are different"
#endif

/* Check if current file and DEM header file are of the same version */
#if (I2C_DISABLE_DEM_REPORT_ERROR_STATUS == STD_OFF)
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((I2C_CFG_AR_RELEASE_MAJOR_VERSION_C != DEM_AR_RELEASE_MAJOR_VERSION) || \
         (I2C_CFG_AR_RELEASE_MINOR_VERSION_C != DEM_AR_RELEASE_MINOR_VERSION)    \
        )
        #error "AutoSar Version Numbers of CDD_I2c_Cfg.c and Dem.h are different"
    #endif
#endif
#endif

/*==================================================================================================
                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
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
[!NOCODE!][!//
[!INCLUDE "CDD_I2c_RegOperations.m"!][!//
[!ENDNOCODE!][!//

#define I2C_START_SEC_CONFIG_DATA_UNSPECIFIED

#include "I2c_MemMap.h"

[!CALL "I2c_Dem_Config", "Type" = "'PC'"!]

#define I2C_STOP_SEC_CONFIG_DATA_UNSPECIFIED

#include "I2c_MemMap.h"

/*==================================================================================================
                                       GLOBAL VARIABLES
==================================================================================================*/

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
