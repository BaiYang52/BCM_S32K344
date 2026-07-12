/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : ADC_SAR
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

#ifndef ADC_IPW_CFGDEFINES_H
#define ADC_IPW_CFGDEFINES_H

/**
*   @file
*
*   @addtogroup adc_ipw_config Adc Ipw Configuration
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

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/

#define ADC_IPW_VENDOR_ID_CFGDEFINES                     43
#define ADC_IPW_AR_RELEASE_MAJOR_VERSION_CFGDEFINES      4
#define ADC_IPW_AR_RELEASE_MINOR_VERSION_CFGDEFINES      4
#define ADC_IPW_AR_RELEASE_REVISION_VERSION_CFGDEFINES   0
#define ADC_IPW_SW_MAJOR_VERSION_CFGDEFINES              2
#define ADC_IPW_SW_MINOR_VERSION_CFGDEFINES              0
#define ADC_IPW_SW_PATCH_VERSION_CFGDEFINES              1

/*==================================================================================================
*                              DEFINES AND MACROS
==================================================================================================*/
#define ADC_IPW_INVALID_LOGICAL_UNIT_ID     (0xFFU)

#define ADC_IPW_INVALID_DMA_CHANNEL_ID      (0xFFU)

#define ADC_IPW_INVALID_TRANSFER_TYPE       (0xFFU)

#define ADC_IPW_NUM_OF_GROUP_CHN                ([!"ecu:get('Adc.AdcConfigSet.AdcSar.NumGroupChan')"!]U)

/**
 * @brief          Max number of ADC SAR Hw units.
 */
#define ADC_IPW_MAX_HW_UNITS                    ([!"ecu:get('Adc.AdcConfigSet.AdcHwUnit')"!]U)

/**
 * @brief   Define the number of maximum hardware unit configured
 */
#define ADC_IPW_MAX_HW_UNITS_CFG                ([!"num:i(count(AdcConfigSet/AdcHwUnit/*))"!]U)

/**
 * @brief          Max number of CTU Hw units.
 */
#define ADC_IPW_CTU_MAX_HW_UNITS                    (1U)

#define ADC_IPW_CTU_UNIT_USED                   (0U)

/**
* @brief           Bctu trigger mode is supported.
*/
#define ADC_IPW_CTU_TRIGGER_MODE_SUPPORTED      (STD_ON)

[!IF "AutosarExt/AdcEnableCtuTrigAutosarExtApi"!][!//
/**
* @brief           Maximum number of hardware triggers configred for all groups.
*/
#define ADC_IPW_MAX_HARDWARE_TRIGGERS           ([!"num:i(count(AdcConfigSet/AdcHwTrigger/*))"!]U)

[!ENDIF!][!//
/**
* @brief           PDB available/unavailable (STD_ON/STD_OFF).
*/
#define ADC_IPW_PDB_AVAILABLE                   (STD_OFF)

[!IF "AutosarExt/AdcEnableSetChannel"!][!//
/**
* @brief           PDB channel delays available/unavailable (STD_ON/STD_OFF).
*/
#define ADC_IPW_DELAY_AVAILABLE                 (ADC_IPW_PDB_AVAILABLE)

[!ENDIF!][!//
/**
* @brief           BCTU available/unavailable (STD_ON/STD_OFF).
*/
#define ADC_IPW_BCTU_AVAILABLE                  (STD_ON)

/**
* @brief           CTU available/unavailable (STD_ON/STD_OFF).
*/
#define ADC_IPW_CTU_AVAILABLE                   (STD_OFF)

/**
* @brief           Define the number of maximum channels
*/
#define ADC_IPW_MAXIMUM_HW_CHANNELS             ([!"num:i(ecu:get('Adc.AdcConfigSet.AdcSar.MaxHwChanCount'))"!]U)

/**
* @brief           Maximum CTU event configuration.
*/
#define ADC_IPW_MAX_CTU_TRIG_SOURCE_ID      ([!"num:i(ecu:get('AdcMaxCtuTriggerEventId'))"!]U)
[!IF "ecu:get('AdcHasExtTrig') = 'TRUE'"!][!//
#define ADC_IPW_EXT_TRG_SRC                     ([!"num:i(ecu:get('AdcMaxCtuTriggerEventId') + 1)"!]U) /* Normal and injected external trigger */
[!IF "ecu:get('AdcHasAuxExtTrig') = 'TRUE'"!][!//
#define ADC_IPW_AUX_EXT_TRG_SRC                 ([!"num:i(ecu:get('AdcMaxCtuTriggerEventId') + 2)"!]U) /* Auxiliary normal external trigger */
[!ENDIF!][!//
[!ENDIF!][!//

/**
* @brief           ADC hardware is supported CALSTAT register
*/
#define ADC_IPW_CALSTAT_AVAILABLE               (STD_OFF)

[!IF "AutosarExt/AdcEnableDmaTransferMode"!][!//
/**
* @brief          Available resolutions
*/
#define ADC_IPW_AVAILABLE_RESOLUTIONS       {14U, 12U, 10U, 8U}

[!ENDIF!][!//
#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ADC_IPW_CFGDEFINES_H */
