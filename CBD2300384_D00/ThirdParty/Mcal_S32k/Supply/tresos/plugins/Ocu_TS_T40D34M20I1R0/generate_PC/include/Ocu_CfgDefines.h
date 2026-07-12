/*==================================================================================================
* Project : RTD AUTOSAR 4.4
* Platform : CORTEXM
* Peripheral : eMios
* Dependencies : none
*
* Autosar Version : 4.4.0
* Autosar Revision : ASR_REL_4_4_REV_0000
* Autosar Conf.Variant :
* SW Version : 2.0.1
* Build Version : S32K3_RTD_2_0_1_D2207_ASR_REL_4_4_REV_0000_20220707
*
* (c) Copyright 2020 - 2022 NXP Semiconductors
* All Rights Reserved.
*
* NXP Confidential. This software is owned or controlled by NXP and may only be
* used strictly in accordance with the applicable license terms. By expressly
* accepting such terms or by downloading, installing, activating and/or otherwise
* using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms. If you do not agree to be
* bound by the applicable license terms, then you may not retain, install,
* activate or otherwise use the software.
==================================================================================================*/

#ifndef OCU_CFGDEFINES_H
#define OCU_CFGDEFINES_H
/**
*   @file           Ocu_CfgDefines.h
*
*   @addtogroup     ocu_driver Ocu Driver
*   @brief          Ocu HLD Precompile defines header file.
*   @details        Precompile defines.
*
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

#include "StandardTypes.h"

/*==================================================================================================
*                               HEADER FILE VERSION INFORMATION
==================================================================================================*/
#define OCU_CFGDEFINES_VENDOR_ID                       43
#define OCU_CFGDEFINES_AR_RELEASE_MAJOR_VERSION        4
#define OCU_CFGDEFINES_AR_RELEASE_MINOR_VERSION        4
#define OCU_CFGDEFINES_AR_RELEASE_REVISION_VERSION     0
#define OCU_CFGDEFINES_SW_MAJOR_VERSION                2
#define OCU_CFGDEFINES_SW_MINOR_VERSION                0
#define OCU_CFGDEFINES_SW_PATCH_VERSION                1
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if this header file and StandardTypes.h file are of the same Autosar version */
    #if ((OCU_CFGDEFINES_AR_RELEASE_MAJOR_VERSION != STD_AR_RELEASE_MAJOR_VERSION) || \
        (OCU_CFGDEFINES_AR_RELEASE_MINOR_VERSION != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Ocu_CfgDefines.h and StandardTypes.h are different"
    #endif
#endif

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

[!NOCODE!]
[!IF "(IMPLEMENTATION_CONFIG_VARIANT = 'VariantPreCompile') and (variant:size() <= 1)"!]
[!CODE!]#define OCU_PRECOMPILE_SUPPORT      (STD_ON)[!ENDCODE!]
[!ELSE!]
[!CODE!]#define OCU_PRECOMPILE_SUPPORT      (STD_OFF)[!ENDCODE!]
[!ENDIF!]
[!ENDNOCODE!]

/** @brief   Switch to indicate if Ocu_GetVersionInfo API is supported */
#define OCU_VERSION_INFO_API    ([!IF "OcuConfigurationOfOptionalApis/OcuVersionInfoApi"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

#ifndef OCU_CONF_CHANNELS_PB
/** @brief  Number of configured Ocu channels */
#define OCU_CONF_CHANNELS_PB    ([!"num:i(count(OcuConfigSet/OcuChannel/*))"!]U)
#endif

#ifndef OCU_DOWNCOUNTING_SUPPORED
/**
* @brief        Switch to indicate that the down counting is supported
*               For current implementation, this feature does not support
*/
#define OCU_DOWNCOUNTING_SUPPORED   (STD_OFF)
#endif
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

#ifdef __cplusplus
}
#endif

/** @} */
#endif  /* OCU_CFGDEFINES_H */
