/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : DMA,CACHE,TRGMUX,LCU,EMIOS,FLEXIO
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

/* Prevention from multiple including the same header */
#ifndef S32K3XX_LCU_IP_FEATURES_H_
#define S32K3XX_LCU_IP_FEATURES_H_

#ifdef __cplusplus
extern "C"
{
#endif

/**
*   @file    Lcu_Ip_Features.h
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Mcl - LCU IP driver header file.
*   @details
*
*   @addtogroup LCU_IP_DRIVER LCU IP Driver
*   @{
*/

#include "Mcal.h"
[!INDENT "0"!][!//
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!VAR "DerivativeName" = "text:toupper(substring-before(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative,'_'))"!]
    [!IF "contains($DerivativeName, 'S32K3')"!][!//
        [!IF "contains($DerivativeName, 'S32K341')"!][!//
            [!CODE!][!WS "0"!]#include "S32K342_LCU.h"[!CR!][!ENDCODE!][!//
        [!ELSE!][!//
            [!CODE!][!WS "0"!]#include "[!"$DerivativeName"!]_LCU.h"[!CR!][!ENDCODE!][!//
        [!ENDIF!][!//
    [!ENDIF!][!//
[!ELSE!][!//
    [!ERROR "The Resource plugin or ResourceSubderivative field is not available for header file inclusion."!][!//
[!ENDIF!][!//
[!ENDINDENT!][!//

/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define LCU_IP_FEATURES_VENDOR_ID                       43
#define LCU_IP_FEATURES_AR_RELEASE_MAJOR_VERSION        4
#define LCU_IP_FEATURES_AR_RELEASE_MINOR_VERSION        4
#define LCU_IP_FEATURES_AR_RELEASE_REVISION_VERSION     0
#define LCU_IP_FEATURES_SW_MAJOR_VERSION                2
#define LCU_IP_FEATURES_SW_MINOR_VERSION                0
#define LCU_IP_FEATURES_SW_PATCH_VERSION                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if header file and Mcal header file are of the same Autosar version */
#if ((LCU_IP_FEATURES_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
     (LCU_IP_FEATURES_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Lcu_Ip_Features.h and Mcal.h are different"
#endif
#endif

/*==================================================================================================
                                            DEFINES
==================================================================================================*/

/** Number of input/output per LC */
#define LCU_NUM_INPUT_PER_LC                        4U
#define LCU_NUM_OUTPUT_PER_LC                       4U

/*-------------------------------------------------------------------/
/                       INSTANCE RESET VALUE                         /
/-------------------------------------------------------------------*/
#define FORCE_FILTER_RESET_VALUE                    ((uint8)(0U))
#define FORCE_POLARITY_RESET_VALUE                  ((uint8)(0U))
#define COMBEN_RESET_VALUE                          ((uint8)(0U))
#define SW_SYNC_SEL_RESET_VALUE                     ((uint8)(0U))

/*-------------------------------------------------------------------/
/                   LUT AND FORCE MASK FOR INTERRUPT                 /
/-------------------------------------------------------------------*/
#define LUT_STS_EVENT_MASK                          ((uint32)(0x01U))
#define FORCE_STS_EVENT_MASK                        ((uint32)(0x0100U))
#define LUT_INT_ENABLE_MASK                         ((uint32)(0x01U))
#define FORCE_INT_ENABLE_MASK                       ((uint32)(0x10000U))
/** @} */

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef S32K3XX_LCU_IP_FEATURES_H_ */

/*==================================================================================================
 *                                        END OF FILE
==================================================================================================*/
