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
 *   @file          Wkpu_Ip_PBcfg.c
 *   @version       2.0.1
 *
 *   @brief   AUTOSAR Icu - contains the data exported by the Icu module
 *   @details Contains the information that will be exported by the module, as requested by Autosar.
 *
 *   @addtogroup wkpu_icu_ip WKPU IPL
 *   @{
 */
 
[!NOCODE!]
/*==================================================================================================
 *                              GENERATION MACRO DEFINTION 
 *================================================================================================*/
[!// MACRO for creating the IP configuration pointer(s)
[!MACRO "ICU_GENERATE_IP_CONFIGURATION","VARIANT"="'PB'"!]
[!NOCODE!]   
[!SELECT "IcuConfigSet"!][!//
    [!IF "count(IcuWkpu/*) > '0' and not(starts-with(ecu:get('Icu.Derivative'),'S32R'))"!]
        [!LOOP "IcuWkpu/*"!][!// Implementation is supporting only 1 instance of WKPU
            [!IF "num:i(count(IcuWkpuChannels/*)) > '0'"!]
                [!VAR "IrqChannelsAvailable" = "'true'"!]
                [!CODE!]/** @brief Variant: [!"$VARIANT"!] Default WKPU Configuration. */[!CR!][!ENDCODE!]
                [!CODE!]const Wkpu_Ip_ChannelConfigType Wkpu_Ip_ChannelConfig_[!"$VARIANT"!][[!"num:i(count(IcuWkpuChannels/*))"!]U] = [!CR!]{[!CR!][!ENDCODE!]
                [!VAR "comma" = "num:i(count(IcuWkpuChannels/*))"!]
                [!LOOP "IcuWkpuChannels/*"!]
                    [!CODE!][!WS "4"!]/** @brief [!"node:name(.)"!] */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief Wkpu HW Channel used by the Icu channel */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!][!"node:value(IcuWkpuChannel)"!]U,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief Wkpu HW Channel Filter enable */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(Icu_EXT_ISR_WIFERDigitalFilter))"!],[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief Wkpu HW Channel Pullup enable */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(IcuWKPU_ISR_WIPUER))"!],[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief Wkpu Default Start Edge */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!][!"text:replace((../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuDefaultStartEdge),'ICU_','WKPU_IP_')"!],[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief Wkpu Channel Callback */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]&Icu_ReportEvents,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief The notification functions */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]NULL_PTR,[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief Wkpu Callback Param1 */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!][!"(../../../../../IcuConfigSet/IcuChannel/*[as:path(node:ref(./IcuChannelRef)) = as:path(node:current())]/IcuChannelId)"!]U[!CR!][!ENDCODE!]
                    [!CODE!][!WS "4"!]}[!ENDCODE!]
                    [!IF "(num:i($comma))>1"!][!CODE!][!WS "4"!],[!CR!][!ENDCODE!][!VAR "comma"="$comma - 1"!][!ENDIF!]
                [!ENDLOOP!]
                [!CODE!][!CR!]};[!CR!][!ENDCODE!]
            [!ENDIF!]
            [!IF "$IrqChannelsAvailable = 'true' or $NMIChannelsAvailable = 'true'"!]
                [!CODE!][!CR!]/** @brief Variant: [!"$VARIANT"!] WKPU IP Configuration. */[!CR!][!ENDCODE!]
                [!CODE!]const Wkpu_Ip_IrqConfigType Wkpu_Ip_Config_[!"$VARIANT"!] =[!CR!]{[!CR!][!ENDCODE!]
              [!IF "$NMIChannelsAvailable = 'true'"!]
                [!CODE!]#if (defined (WKPU_IP_NMI_API) && (STD_ON == WKPU_IP_NMI_API))[!CR!][!ENDCODE!][!//
                [!CODE!][!WS "4"!]/** @brief Number of Wkpu NMI channels in the Icu configuration */[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!](uint8)[!"num:i(count(IcuWkpuNMIConfiguration/*))"!]U,[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!]/** @brief Pointer to the array of Wkpu enabled Icu channel configurations */[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!]&Wkpu_Ip_NMIConfig_[!"$VARIANT"!],[!CR!][!ENDCODE!]
                [!CODE!]#endif[!CR!][!ENDCODE!][!//
              [!ELSE!]
                [!CODE!]#if (defined (WKPU_IP_NMI_API) && (STD_ON == WKPU_IP_NMI_API))[!CR!][!ENDCODE!][!//
                [!CODE!][!WS "4"!]/** @brief Number of Wkpu channels in the Icu configuration */[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!](uint8)0U,[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!]/** @brief Pointer to the array of Wkpu enabled Icu channel configurations */[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!]NULL_PTR,[!CR!][!ENDCODE!]
                [!CODE!]#endif[!CR!][!ENDCODE!][!//
              [!ENDIF!]
              
              [!IF "$IrqChannelsAvailable = 'true'"!]
                [!CODE!][!WS "4"!]/** @brief Number of Wkpu channels in the Icu configuration */[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!](uint8)[!"num:i(count(IcuWkpuChannels/*))"!]U,[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!]/** @brief Pointer to the array of Wkpu enabled Icu channel configurations */[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!]&Wkpu_Ip_ChannelConfig_[!"$VARIANT"!][!CR!][!ENDCODE!]
              [!ELSE!]
                [!CODE!][!WS "4"!]/** @brief Number of Wkpu channels in the Icu configuration */[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!](uint8)0U,[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!]/** @brief Pointer to the array of Wkpu enabled Icu channel configurations */[!CR!][!ENDCODE!]
                [!CODE!][!WS "4"!]NULL_PTR[!CR!][!ENDCODE!]
              [!ENDIF!]
                [!CODE!]};[!CR!][!ENDCODE!]
            [!ENDIF!]
        [!ENDLOOP!]
    [!ENDIF!]
[!ENDSELECT!]
[!ENDNOCODE!]
[!ENDMACRO!][!// End of Macro ICU_GENERATE_WKPU_IP_CONFIGURATION

[!// MACRO for creating the NMI configuration pointer(s)
[!MACRO "ICU_GENERATE_NMI_CONFIGURATION","VARIANT"="'PB'"!]
[!NOCODE!]   
[!SELECT "IcuConfigSet"!][!//
    [!IF "count(IcuWkpu/*) > '0'"!]
        [!LOOP "IcuWkpu/*"!]
            [!IF "count(IcuWkpuNMIConfiguration/*) > '0'"!]
                [!VAR "NMIChannelsAvailable" = "'true'"!]
                [!CODE!][!CR!][!ENDCODE!][!//
                [!CODE!]/** @brief Variant: [!"$VARIANT"!] NMI Configuration. */[!CR!][!ENDCODE!]
                [!CODE!]Wkpu_Ip_NmiCfgType Wkpu_Ip_NMIConfig_[!"$VARIANT"!][[!"num:i(count(IcuWkpuNMIConfiguration/*))"!]U] = [!CR!]{[!CR!][!ENDCODE!]
                [!VAR "comma" = "num:i(count(IcuWkpuNMIConfiguration/*))"!]
                [!LOOP "IcuWkpuNMIConfiguration/*"!]
                    [!CODE!][!WS "4"!]/** @brief [!"node:name(.)"!] */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "4"!]{[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief WKPU core source */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]WKPU_CORE[!"node:value(NMICoreSource)"!],[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief NMI destination source */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]WKPU_IP_[!"node:value(DestinationSourceSelect)"!],[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief NMI request enable */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(WakeupRequestEnable))"!],[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief NMI filter enable */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(FilterEnable))"!],[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief NMI edge events */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]WKPU_IP_[!"node:value(NMIEdgeEvents)"!],[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!]/** @brief NMI configuration lock register */[!CR!][!ENDCODE!]
                    [!CODE!][!WS "8"!](boolean)[!"text:toupper(node:value(LockRegister))"!][!CR!][!ENDCODE!]
                    [!CODE!][!WS "4"!]}[!ENDCODE!]
                    [!IF "(num:i($comma))>1"!][!CODE!][!WS "0"!],[!CR!][!ENDCODE!][!VAR "comma"="$comma - 1"!][!ENDIF!]
                [!ENDLOOP!]
                [!CODE!][!CR!]};[!CR!][!ENDCODE!]
            [!ENDIF!]
        [!ENDLOOP!]
    [!ENDIF!]
[!ENDSELECT!]
[!ENDNOCODE!]
[!ENDMACRO!][!// End of Macro ICU_GENERATE_NMI_CONFIGURATION

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
#include "Wkpu_Ip_Types.h"

/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *================================================================================================*/
#define WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C                    43
#define WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C     4
#define WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C     4
#define WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C  0
#define WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C             2
#define WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C             0
#define WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C             1

/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *================================================================================================*/
/* Check if header file and StandardTypes.h file are of the same Autosar version */
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if ((WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != STD_AR_RELEASE_MAJOR_VERSION) || \
         (WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Wkpu_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and StandardTypes.h are different"
    #endif
#endif

/* Check if source file and ICU header file are of the same vendor */
#if (WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_VENDOR_ID_C != WKPU_IP_TYPES_VENDOR_ID)
    #error "Wkpu_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Wkpu_Ip_Types.h have different vendor IDs"
#endif
/* Check if source file and ICU header file are of the same AutoSar version */
#if ((WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MAJOR_VERSION_C != WKPU_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_MINOR_VERSION_C != WKPU_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_AR_RELEASE_REVISION_VERSION_C != WKPU_IP_TYPES_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Wkpu_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Wkpu_Ip_Types.h are different"
#endif
/* Check if source file and ICU header file are of the same Software version */
#if ((WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MAJOR_VERSION_C != WKPU_IP_TYPES_SW_MAJOR_VERSION) || \
     (WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_MINOR_VERSION_C != WKPU_IP_TYPES_SW_MINOR_VERSION) || \
     (WKPU_IP_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBCFG_SW_PATCH_VERSION_C != WKPU_IP_TYPES_SW_PATCH_VERSION))
#error "Software Version Numbers of Wkpu_Ip_[!IF "var:defined('postBuildVariant')"!][!"$postBuildVariant"!]_[!ENDIF!]PBcfg.c and Wkpu_Ip_Types.h are different"
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
#ifndef WKPU_IP_DERIVATIVE_NOT_SUPPORT_WKPU

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
[!NOCODE!][!//
[!SELECT "IcuConfigSet"!][!//
[!IF "count(IcuWkpu/*) > '0'"!][!//
[!CODE!]
#define ICU_START_SEC_CONFIG_DATA_UNSPECIFIED
[!ENDCODE!][!//
[!CODE!]#include "Icu_MemMap.h"[!CR!][!ENDCODE!][!//
[!ENDIF!][!//
[!ENDSELECT!]
[!ENDNOCODE!][!//

[!NOCODE!][!//
[!IF "var:defined('postBuildVariant')"!][!//
    [!VAR "PBCfg_Variant" = "concat('PB_', $postBuildVariant)"!][!//
[!ELSE!][!//
    [!VAR "PBCfg_Variant" = "'PB'"!][!//
[!ENDIF!][!//

[!VAR "IrqChannelsAvailable" = "'false'"!]
[!VAR "NMIChannelsAvailable" = "'false'"!]
[!CALL "ICU_GENERATE_NMI_CONFIGURATION","VARIANT"="$PBCfg_Variant"!][!//
[!CALL "ICU_GENERATE_IP_CONFIGURATION","VARIANT"="$PBCfg_Variant"!][!//

[!ENDNOCODE!][!//

[!NOCODE!][!//
[!SELECT "IcuConfigSet"!][!//
[!IF "count(IcuWkpu/*) > '0'"!][!//
[!CODE!]
#define ICU_STOP_SEC_CONFIG_DATA_UNSPECIFIED
[!ENDCODE!][!//
[!CODE!]#include "Icu_MemMap.h"[!ENDCODE!][!//
[!ENDIF!][!//
[!ENDSELECT!]
[!ENDNOCODE!][!//

#endif /* WKPU_IP_DERIVATIVE_NOT_SUPPORT_WKPU */
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
[!ENDCODE!][!//
