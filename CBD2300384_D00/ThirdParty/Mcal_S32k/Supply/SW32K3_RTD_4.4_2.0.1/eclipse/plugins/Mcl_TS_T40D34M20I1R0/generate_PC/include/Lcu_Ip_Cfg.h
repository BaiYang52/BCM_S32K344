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
#ifndef LCU_IP_CFG_H_
#define LCU_IP_CFG_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*******************************************************************************
 * Included files
 ******************************************************************************/
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
#include "Lcu_Ip_[!"."!]_PBcfg.h"
[!ENDLOOP!][!//
[!ELSE!][!//
#include "Lcu_Ip_PBcfg.h"
[!ENDIF!][!//
[!IF "node:value(./MclGeneral/MclLcuConfig/MclEnableLcu)"!][!//
[!INCLUDE "Lcu_Ip_RegOperations.m"!][!//
[!ENDIF!][!//

/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define LCU_IP_CFG_VENDOR_ID                       43
#define LCU_IP_CFG_AR_RELEASE_MAJOR_VERSION        4
#define LCU_IP_CFG_AR_RELEASE_MINOR_VERSION        4
#define LCU_IP_CFG_AR_RELEASE_REVISION_VERSION     0
#define LCU_IP_CFG_SW_MAJOR_VERSION                2
#define LCU_IP_CFG_SW_MINOR_VERSION                0
#define LCU_IP_CFG_SW_PATCH_VERSION                1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
/* Check if header file and Lcu_Ip_[!"."!]_PBcfg.h file are of the same vendor */
#if (LCU_IP_CFG_VENDOR_ID != LCU_IP_[!"."!]_PBCFG_VENDOR_ID)
    #error "Lcu_Ip_Cfg.h and Lcu_Ip_[!"."!]_PBcfg.h have different vendor ids"
#endif

/* Check if header file and Lcu_Ip_[!"."!]_PBcfg.h file are of the same Autosar version */
#if ((LCU_IP_CFG_AR_RELEASE_MAJOR_VERSION != LCU_IP_[!"."!]_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (LCU_IP_CFG_AR_RELEASE_MINOR_VERSION != LCU_IP_[!"."!]_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (LCU_IP_CFG_AR_RELEASE_REVISION_VERSION != LCU_IP_[!"."!]_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Lcu_Ip_Cfg.h and Lcu_Ip_[!"."!]_PBcfg.h are different"
#endif

/* Check if header file and Lcu_Ip_[!"."!]_PBcfg.h file are of the same Software version */
#if ((LCU_IP_CFG_SW_MAJOR_VERSION != LCU_IP_[!"."!]_PBCFG_SW_MAJOR_VERSION) || \
     (LCU_IP_CFG_SW_MINOR_VERSION != LCU_IP_[!"."!]_PBCFG_SW_MINOR_VERSION) || \
     (LCU_IP_CFG_SW_PATCH_VERSION != LCU_IP_[!"."!]_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Lcu_Ip_Cfg.h and Lcu_Ip_[!"."!]_PBcfg.h are different"
#endif
[!ENDLOOP!][!//
[!ELSE!][!//
/* Check if header file and Lcu_Ip_PBcfg.h file are of the same vendor */
#if (LCU_IP_CFG_VENDOR_ID != LCU_IP_PBCFG_VENDOR_ID)
    #error "Lcu_Ip_Cfg.h and Lcu_Ip_PBcfg.h have different vendor ids"
#endif

/* Check if header file and Lcu_Ip_PBcfg.h file are of the same Autosar version */
#if ((LCU_IP_CFG_AR_RELEASE_MAJOR_VERSION != LCU_IP_PBCFG_AR_RELEASE_MAJOR_VERSION) || \
     (LCU_IP_CFG_AR_RELEASE_MINOR_VERSION != LCU_IP_PBCFG_AR_RELEASE_MINOR_VERSION) || \
     (LCU_IP_CFG_AR_RELEASE_REVISION_VERSION != LCU_IP_PBCFG_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Lcu_Ip_Cfg.h and Lcu_Ip_PBcfg.h are different"
#endif

/* Check if header file and Lcu_Ip_PBcfg.h file are of the same Software version */
#if ((LCU_IP_CFG_SW_MAJOR_VERSION != LCU_IP_PBCFG_SW_MAJOR_VERSION) || \
     (LCU_IP_CFG_SW_MINOR_VERSION != LCU_IP_PBCFG_SW_MINOR_VERSION) || \
     (LCU_IP_CFG_SW_PATCH_VERSION != LCU_IP_PBCFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Lcu_Ip_Cfg.h and Lcu_Ip_PBcfg.h are different"
#endif
[!ENDIF!][!//
[!IF "node:value(./MclGeneral/MclLcuConfig/MclEnableLcu)"!][!//

/*===============================================================================================
                                       DEFINES AND MACROS
===============================================================================================*/
[!IF "node:value(./MclGeneral/MclEnableUserModeSupport)"!][!//
/* Support USER MODE */
#define MCL_LCU_REG_PROT_AVAILABLE
[!ENDIF!][!//

/* Number Of Configured Logic Instances */
#define LCU_IP_NOF_CFG_LOGIC_INSTANCES   ((uint32)[!CALL "Lcu_Ip_GetNrOfLogicInstances"!]U)[!//

[!LOOP "./MclConfig/lcuConfiguration/*/lcuInstanceCfg/*"!]
/* Logic Instance [!"@index"!] */
#define [!"node:value(lcuLogicInstance_LogicName)"!]             ((uint32)[!"@index"!]U)
[!ENDLOOP!][!//


/* Number Of Configured Logic Inputs */
#define LCU_IP_NOF_CFG_LOGIC_INPUTS      ((uint32)[!CALL "Lcu_Ip_GetNrOfLogicInputs"!]U)[!//

[!LOOP "./MclConfig/lcuConfiguration/*/mclLcuInputConfiguration/*"!]
/* Logic Input [!"@index"!] */
#define [!"node:value(lcuLogicInput_LogicName)"!]                ((uint32)[!"@index"!]U)
[!ENDLOOP!][!//


/* Number Of Configured Logic Outputs */
#define LCU_IP_NOF_CFG_LOGIC_OUTPUTS     ((uint32)[!CALL "Lcu_Ip_GetNrOfLogicOutputs"!]U)[!//

[!LOOP "./MclConfig/lcuConfiguration/*/mclLcuOutputConfiguration/*"!]
/* Logic Input [!"@index"!] */
#define [!"node:value(lcuLogicOutput_LogicName)"!]               ((uint32)[!"@index"!]U)

[!ENDLOOP!][!//

#define MCL_START_SEC_CONFIG_DATA_UNSPECIFIED
/* @violates @ref LCU_IP_CFG_H_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

/* LCU Initialization Structure */
[!IF "var:defined('postBuildVariant')"!][!//
[!LOOP "variant:all()"!][!//
extern const Lcu_Ip_InitType Lcu_Ip_xLcuInitPB_[!"."!];
[!ENDLOOP!][!//
[!ELSE!][!//
extern const Lcu_Ip_InitType Lcu_Ip_xLcuInitPB;
[!ENDIF!][!//

/* LCU Input De-Initialization Structure */
extern const Lcu_Ip_LogicInputConfigType Lcu_Ip_xLogicInputResetConfig;

/* LCU Output De-Initialization Structure */
extern const Lcu_Ip_LogicOutputConfigType Lcu_Ip_xLogicOutputResetConfig;

#define MCL_STOP_SEC_CONFIG_DATA_UNSPECIFIED
/* @violates @ref LCU_IP_CFG_H_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

#define MCL_START_SEC_CODE
/* @violates @ref LCU_IP_CFG_H_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"

void Lcu0_Ip_IRQHandler(void);

void Lcu1_Ip_IRQHandler(void);

#define MCL_STOP_SEC_CODE
/* @violates @ref LCU_IP_CFG_H_REF_1 MISRA 2012 Required Directive 4.10, Precautions shall be taken in order to prevent the contents of a header file being included more than once. */
#include "Mcl_MemMap.h"
[!ENDIF!][!//

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef LCU_IP_CFG_H_ */

/*==================================================================================================
 *                                        END OF FILE
==================================================================================================*/
