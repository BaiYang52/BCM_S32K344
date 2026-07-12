[!AUTOSPACING!][!//
[!CODE!][!//
/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Emios Siul2 Wkpu LpCmp
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
 *   @file    Cmp_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c
 *   @version 2.0.1
 *
 *   @brief   AUTOSAR Icu - contains the data exported by the ICU module
 *   @details Contains the information that will be exported by the module, as requested by AUTOSAR.
 *
 *   @addtogroup cmp_ip CMP IPL
 *   @{
 */
 
[!NOCODE!]
/*==================================================================================================
 *                              GENERATION MACRO DEFINTION 
 *================================================================================================*/

[!// MACRO for creating the IP configuration structure(s)
[!MACRO "ICU_GENERATE_IP_CONFIGURATION","VARIANT"="'PB'"!][!//
[!NOCODE!]
    
[!SELECT "./IcuConfigSet"!]
[!IF "count(IcuLpCmp/*) > '0'"!]

    [!VAR "comma" = "num:i(count(IcuCmp/*))"!]
    
    [!LOOP "IcuLpCmp/*"!]
      [!CODE!]/*[!CR!][!ENDCODE!]
      [!CODE!] *  @brief    [!"$VARIANT"!] CMP [!"node:name(.)"!] Configuration[!CR!][!ENDCODE!]
      [!CODE!] */[!CR!][!ENDCODE!]
      [!CODE!]const Cmp_Ip_ConfigType Cmp_Ip_[!"node:value(IcuCmpInstanceNumber)"!]_Config_[!"$VARIANT"!] = [!CR!]{[!CR!][!ENDCODE!]

        [!CODE!][!WS "4"!]/** @brief CMP HW Module used by the Icu channel */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!][!"node:value(IcuCmpInstanceNumber)"!]U,[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]/** @brief CMP Comparator parameters */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief Comparator Functional Mode */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuCmp/IcuCmpFunctionalMode)"!],[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief Comparator Hysteresis */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuCmp/IcuCmpHysteresisLevel)"!],[!CR!][!ENDCODE!]        
        [!CODE!][!WS "8"!]/** @brief Comparator Offset Level */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuCmp/IcuCmpOffsetLevel)"!],[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief Comparator Enable Pin Output */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(IcuCmp/IcuCmpEnablePinOutput))"!],[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief Comparator Enable Inverter */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(IcuCmp/IcuCmpEnableInverter))"!],[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief Comparator Enable Comparator Invert */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(IcuCmp/IcuCmpEnableComparatorInvert))"!],[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief Comparator Enable High Power */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(IcuCmp/IcuCmpEnableHighPowerMode))"!],[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief Comparator Sample Period */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuCmp/IcuCmpFilterSamplePeriod)"!]U,[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief Comparator Sample Count */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuCmp/IcuCmpFilterSampleCount)"!]U,[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief Comparator Enable Dma */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(IcuCmp/IcuCmpEnableDma))"!],[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief Comparator Negative Input muxing */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuCmp/IcuCmpNegativeInputMux)"!],[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief Comparator Positive Input muxing */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuCmp/IcuCmpPositiveInputMux)"!],[!CR!][!ENDCODE!]

        [!CODE!][!WS "8"!]/** @brief Comparator Trigger Edge type*/[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"text:replace((../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuDefaultStartEdge),'ICU_','CMP_IP_INTTRIG_')"!],[!CR!][!ENDCODE!]
        
        [!IF "node:exists(IcuCmp/IcuCmpWindowCloseOutputOverwrite)"!]
            [!CODE!][!WS "8"!]/** @brief Comparator Close Output Overwrite*/[!CR!][!ENDCODE!]
            [!CODE!][!WS "8"!][!"node:value(IcuCmp/IcuCmpWindowCloseOutputOverwrite)"!],[!CR!][!ENDCODE!]
        [!ENDIF!]
        
        [!IF "node:exists(IcuCmp/IcuCmpWindowCloseEvent)"!]
            [!CODE!][!WS "8"!]/** @brief Comparator Close Output Event */[!CR!][!ENDCODE!]
            [!CODE!][!WS "8"!][!"node:value(IcuCmp/IcuCmpWindowCloseEvent)"!],[!CR!][!ENDCODE!]
        [!ENDIF!]
        
        [!IF "node:exists(IcuCmp/IcuCmpEnableInStop)"!]
            [!CODE!][!WS "8"!]/** @brief Comparator Enable InStop */[!CR!][!ENDCODE!]
            [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(IcuCmp/IcuCmpEnableInStop))"!],[!CR!][!ENDCODE!]
        [!ENDIF!]
        
        [!CODE!][!WS "8"!]/** @brief Comparator Output Select */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuCmp/IcuCmpOutputSelect)"!][!CR!][!ENDCODE!]

        [!CODE!][!WS "4"!]},[!CR!][!ENDCODE!]
        
        [!CODE!][!WS "4"!]/** @brief CMP DAC parameters */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief CMP DAC Voltage Level */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuDac/IcuDacVoltageLevel)"!]U,[!CR!][!ENDCODE!]       
        [!CODE!][!WS "8"!]/** @brief CMP Dac Voltage Reference */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuDac/IcuDacVoltageRefSource)"!],[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief CMP Dac Power State */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuDac/IcuDacPowerState)"!][!CR!][!ENDCODE!]        
        [!CODE!][!WS "4"!]},[!CR!][!ENDCODE!]

        [!CODE!][!WS "4"!]/** @brief CMP Triger parameters */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief CMP Trigger Round Robin Channel Mask */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuTrigger/IcuTrgRoundRobinEnChannelMask)"!]U,[!CR!][!ENDCODE!]

        [!IF "node:exists(IcuTrigger/IcuTrgPrepgmStateChannelMask)"!]
            [!CODE!][!WS "8"!]/** @brief CMP Trigger Round Robin Preprog State Channel Mask */[!CR!][!ENDCODE!]
            [!CODE!][!WS "8"!][!"node:value(IcuTrigger/IcuTrgPrepgmStateChannelMask)"!]U,[!CR!][!ENDCODE!]  
        [!ENDIF!]
        
        [!CODE!][!WS "8"!]/** @brief CMP Trigger Delay Value */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuTrigger/IcuTrgInitDelayValue)"!]U,[!CR!][!ENDCODE!]       
        [!CODE!][!WS "8"!]/** @brief CMP Trigger Sample Delay */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuTrigger/IcuTrgSampleDelay)"!],[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief CMP Trigger Fixed Channel */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuTrigger/IcuTrgFixedChannel)"!],[!CR!][!ENDCODE!]  
        [!CODE!][!WS "8"!]/** @brief CMP Trigger Fixed Port */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!][!"node:value(IcuTrigger/IcuTrgFixedPort)"!],[!CR!][!ENDCODE!]  
        [!CODE!][!WS "8"!]/** @brief CMP Trigger Enable Round Robin interrupt */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(IcuTrigger/IcuTrgEnableRoundRobinInterrupt))"!],[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!]/** @brief CMP Trigger Enable Round Robin */[!CR!][!ENDCODE!]
        [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(IcuTrigger/IcuTrgEnableRoundRobin))"!][!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]},[!CR!][!ENDCODE!]

        [!CODE!][!WS "4"!]/** @brief Trigger Callback Pointer */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]&Icu_ReportEvents,[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]/** @brief Comparator Callback Pointer */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]&Icu_ReportEvents,[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]/** @brief Callback Param1*/[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!][!"../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuChannelId"!]U[!CR!][!ENDCODE!]
        
      [!CODE!]};[!CR!][!ENDCODE!]
    [!ENDLOOP!]
[!ENDIF!]
[!ENDSELECT!]

[!ENDNOCODE!]
[!ENDMACRO!][!// End of Macro 

[!ENDNOCODE!][!//

#ifdef __cplusplus
extern "C"{
#endif

 /*==================================================================================================
 *                                         INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
 *================================================================================================*/
#include "StandardTypes.h"
#include "Cmp_Ip_Types.h"

/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/
#define CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C                    43
#define CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C     4
#define CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C     4
#define CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C  0
#define CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C             2
#define CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C             0
#define CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C             1

/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if header file and StandardTypes.h file are of the same Autosar version */
    #if ((CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != STD_AR_RELEASE_MAJOR_VERSION) || \
         (CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Cmp_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and StandardTypes.h are different"
    #endif
#endif

/* Check if source file and ICU header file are of the same vendor */
#if (CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != CMP_IP_TYPES_VENDOR_ID)
    #error "Cmp_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Cmp_Ip_Types.h have different vendor IDs"
#endif
/* Check if source file and ICU header file are of the same AutoSar version */
#if ((CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C    != CMP_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C    != CMP_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != CMP_IP_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Cmp_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Cmp_Ip_Types.h are different"
#endif
/* Check if source file and ICU header file are of the same Software version */
#if ((CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != CMP_IP_TYPES_SW_MAJOR_VERSION) || \
     (CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != CMP_IP_TYPES_SW_MINOR_VERSION) || \
     (CMP_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != CMP_IP_TYPES_SW_PATCH_VERSION))
#error "Software Version Numbers of Cmp_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Cmp_Ip_Types.h are different"
#endif

/*==================================================================================================
 *                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 *================================================================================================*/

/*==================================================================================================
 *                                       LOCAL MACROS
 *================================================================================================*/

/*==================================================================================================
 *                                      LOCAL CONSTANTS
 *================================================================================================*/

/*==================================================================================================
 *                                       LOCAL VARIABLES
 *================================================================================================*/
 
/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/
#define ICU_START_SEC_CODE
#include "Icu_MemMap.h"

/** @brief Signature of report events interrupt function. */
extern void Icu_ReportEvents(uint16 Channel, boolean bOverflow);

#define ICU_STOP_SEC_CODE
#include "Icu_MemMap.h"
/*==================================================================================================
 *                                       GLOBAL CONSTANTS
 *================================================================================================*/
#define ICU_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Icu_MemMap.h" 
[!NOCODE!]
[!IF "var:defined('postBuildVariant')"!]
    [!VAR "PBCfg_Variant" = "concat('PB_', $postBuildVariant)"!]
[!ELSE!]
    [!VAR "PBCfg_Variant" = "'PB'"!]
[!ENDIF!]

[!CALL "ICU_GENERATE_IP_CONFIGURATION","VARIANT"="$PBCfg_Variant"!]

[!ENDNOCODE!]
#define ICU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Icu_MemMap.h" 

/*==================================================================================================
 *                                       GLOBAL VARIABLES
 *================================================================================================*/

/*==================================================================================================
 *                                   LOCAL FUNCTION PROTOTYPES
 *================================================================================================*/

/*==================================================================================================
 *                                       LOCAL FUNCTIONS
 *================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */
[!ENDCODE!]
