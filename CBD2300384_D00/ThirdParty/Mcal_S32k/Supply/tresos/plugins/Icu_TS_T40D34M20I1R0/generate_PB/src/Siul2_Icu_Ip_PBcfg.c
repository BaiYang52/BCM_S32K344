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
 *   @file    Siul2_Icu_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c
 *   @version 2.0.1
 *
 *   @brief   AUTOSAR Icu - contains the data exported by the ICU module
 *   @details Contains the information that will be exported by the module, as requested by AUTOSAR.
 *
 *   @addtogroup siul2_icu_ip SIUL2 IPL
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
[!IF "count(IcuSiul2/*) > '0'"!]
    [!LOOP "IcuSiul2/*"!]

        [!CODE!]/*[!CR!][!ENDCODE!]
        [!CODE!] *  @brief    [!"$VARIANT"!] SIUL2 Channels Configuration[!CR!][!ENDCODE!]
        [!CODE!] */[!CR!][!ENDCODE!]
        [!CODE!]const Siul2_Icu_Ip_ChannelConfigType Siul2_Icu_Ip_[!"node:value(./IcuSiul2Instance)"!]_ChannelConfig_[!"$VARIANT"!][[!"num:i(count(IcuSiul2Channels/*))"!]U] =[!CR!]{[!CR!][!ENDCODE!]
        [!VAR "comma" = "num:i(count(IcuSiul2Channels/*))"!]
            [!LOOP "IcuSiul2Channels/*"!]
                [!CODE!][!WS "4"!]/** @brief [!"node:name(.)"!] */[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]/** @brief Siul2 HW Module and Channel used by the Icu channel */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!][!"node:value(IcuSiul2Channel)"!]U,[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]/** @brief Siul2 Digital Filter enable */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(Icu_EXT_ISR_IFERDigitalFilter))"!],[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]/** @brief Siul2 Digital Filter value */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!][!"node:value(Icu_EXT_ISR_IFMCDigitalFilter)"!]U,[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]/** @brief Siul2 request type*/[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]SIUL2_ICU_IRQ,[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]/** @brief Siul2 Edge type*/[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]SIUL2_[!"../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuDefaultStartEdge"!],[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]/** @brief Callback Pointer */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]&Icu_ReportEvents,[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]/** @brief The notification functions */[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!]/** @brief Callback Param1*/[!CR!][!ENDCODE!]
                [!CODE!][!WS "8"!][!"../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuChannelId"!]U[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!]}[!CR!][!ENDCODE!]
                [!IF "(num:i($comma))>1"!][!CODE!][!WS "4"!],[!CR!][!ENDCODE!][!VAR "comma"="$comma - 1"!][!ENDIF!]
            [!ENDLOOP!]
        [!CODE!]};[!CR!][!ENDCODE!]

        [!CODE!]/*[!CR!][!ENDCODE!]
        [!CODE!] *  @brief    [!"$VARIANT"!] SIUL2 Instance Configuration[!CR!][!ENDCODE!]
        [!CODE!] */[!CR!][!ENDCODE!]
        [!CODE!]const Siul2_Icu_Ip_InstanceConfigType Siul2_Icu_Ip_[!"node:value(./IcuSiul2Instance)"!]_InstanceConfig_[!"$VARIANT"!] = [!CR!]{[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]/** @brief Interrupt Filter Clock Prescaler */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!](uint8)[!"num:i(IcuEXT_ISR_InterruptFilterClockPrescaler)"!]U,[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]/** @brief Interrupt Filter Clock Alternate Prescaler  */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!](uint8)[!"num:i(IcuEXT_ISR_AlternateInterruptFilterClockPrescaler)"!]U[!CR!][!ENDCODE!]
        [!CODE!]};[!CR!][!ENDCODE!]
        
        [!CODE!]/*[!CR!][!ENDCODE!]
        [!CODE!] *  @brief    [!"$VARIANT"!] Default SIUL2 IP Configuration[!CR!][!ENDCODE!]
        [!CODE!] */[!CR!][!ENDCODE!]
        [!CODE!]const Siul2_Icu_Ip_ConfigType Siul2_Icu_Ip_[!"node:value(./IcuSiul2Instance)"!]_Config_[!"$VARIANT"!] = [!CR!]{[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]/** @brief Number of Siul2 channels in the Icu configuration */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!](uint8)[!"num:i(count(IcuSiul2Channels/*))"!]U,[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]/** @brief The Siul2 instance configuration */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]&Siul2_Icu_Ip_[!"node:value(./IcuSiul2Instance)"!]_InstanceConfig_[!"$VARIANT"!],[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]/** @brief Pointer to the array of Siul2 channel configurations */[!CR!][!ENDCODE!]
        [!CODE!][!WS "4"!]&Siul2_Icu_Ip_[!"node:value(./IcuSiul2Instance)"!]_ChannelConfig_[!"$VARIANT"!][!CR!][!ENDCODE!]
        [!CODE!]};[!CR!][!ENDCODE!]   
    
    [!ENDLOOP!]
[!ENDIF!]
[!ENDSELECT!]
[!ENDNOCODE!]
[!ENDMACRO!][!// End of Macro ICU_GENERATE_FTM_IP_CONFIGURATION

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
#include "Siul2_Icu_Ip_Types.h"

/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/
#define SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C                    43
#define SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C     4
#define SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C     4
#define SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C  0
#define SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C             2
#define SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C             0
#define SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C             1

/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if header file and StandardTypes.h file are of the same Autosar version */
    #if ((SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != STD_AR_RELEASE_MAJOR_VERSION) || \
         (SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Siul2_Icu_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and StandardTypes.h are different"
    #endif
#endif

/* Check if source file and ICU header file are of the same vendor */
#if (SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != SIUL2_ICU_IP_TYPES_VENDOR_ID)
    #error "Siul2_Icu_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Siul2_Icu_Ip_Types.h have different vendor IDs"
#endif
/* Check if source file and ICU header file are of the same AutoSar version */
#if ((SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C    != SIUL2_ICU_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C    != SIUL2_ICU_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != SIUL2_ICU_IP_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Siul2_Icu_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Siul2_Icu_Ip_Types.h are different"
#endif
/* Check if source file and ICU header file are of the same Software version */
#if ((SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != SIUL2_ICU_IP_TYPES_SW_MAJOR_VERSION) || \
     (SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != SIUL2_ICU_IP_TYPES_SW_MINOR_VERSION) || \
     (SIUL2_ICU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != SIUL2_ICU_IP_TYPES_SW_PATCH_VERSION))
#error "Software Version Numbers of Siul2_Icu_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Siul2_Icu_Ip_Types.h are different"
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
 *                                       GLOBAL FUNCTIONS
 *================================================================================================*/
#define ICU_START_SEC_CODE
#include "Icu_MemMap.h"

/** @brief Signature of report events interrupt function. */
extern void Icu_ReportEvents(uint16 Channel, boolean bOverflow);

/** @brief Signature of wakeup and overflow interrupt function. */
extern void Icu_ReportWakeupAndOverflow(uint16 Channel, boolean bOverflow);

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
