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

#ifndef ADC_SAR_IP_HW_ACCESS_H
#define ADC_SAR_IP_HW_ACCESS_H

/**
*   @file
*
*   @internal
*   @addtogroup adc_sar_ip Adc Sar IPL
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
#include "Adc_Sar_Ip_CfgDefines.h"

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
#include "Devassert.h"
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ADC_SAR_IP_VENDOR_ID_HWACCESS                      43
#define ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_HWACCESS       4
#define ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_HWACCESS       4
#define ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_HWACCESS    0
#define ADC_SAR_IP_SW_MAJOR_VERSION_HWACCESS               2
#define ADC_SAR_IP_SW_MINOR_VERSION_HWACCESS               0
#define ADC_SAR_IP_SW_PATCH_VERSION_HWACCESS               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if Adc_Sar_Ip_HwAccess.h file and Adc_Sar_Ip_CfgDefines.h file are of the same vendor */
#if (ADC_SAR_IP_VENDOR_ID_HWACCESS != ADC_SAR_IP_VENDOR_ID_CFGDEFINES)
    #error "Adc_Sar_Ip_HwAccess.h and Adc_Sar_Ip_CfgDefines.h have different vendor ids"
#endif

/* Check if Adc_Sar_Ip_HwAccess.h file and Adc_Sar_Ip_CfgDefines.h file are of the same Autosar version */
#if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_HWACCESS != ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_CFGDEFINES) || \
     (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_HWACCESS != ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_CFGDEFINES) || \
     (ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_HWACCESS != ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_CFGDEFINES) \
    )
    #error "AutoSar Version Numbers of Adc_Sar_Ip_HwAccess.h and Adc_Sar_Ip_CfgDefines.h are different"
#endif

/* Check if Adc_Sar_Ip_HwAccess.h file and Adc_Sar_Ip_CfgDefines.h file are of the same Software version */
#if ((ADC_SAR_IP_SW_MAJOR_VERSION_HWACCESS != ADC_SAR_IP_SW_MAJOR_VERSION_CFGDEFINES) || \
     (ADC_SAR_IP_SW_MINOR_VERSION_HWACCESS != ADC_SAR_IP_SW_MINOR_VERSION_CFGDEFINES) || \
     (ADC_SAR_IP_SW_PATCH_VERSION_HWACCESS != ADC_SAR_IP_SW_PATCH_VERSION_CFGDEFINES) \
    )
  #error "Software Version Numbers of Adc_Sar_Ip_HwAccess.h and Adc_Sar_Ip_CfgDefines.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if Adc_Sar_Ip_HwAccess.h file and StandardTypes.h file are of the same Autosar version */
#if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_HWACCESS != STD_AR_RELEASE_MAJOR_VERSION) || \
     (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_HWACCESS != STD_AR_RELEASE_MINOR_VERSION)    \
    )
    #error "AutoSar Version Numbers of Adc_Sar_Ip_HwAccess.h and StandardTypes.h are different"
#endif

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
/* Check if Adc_Sar_Ip_HwAccess.h and Devassert.h are of the same Autosar version */
#if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_HWACCESS != DEVASSERT_AR_RELEASE_MAJOR_VERSION) || \
     (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_HWACCESS != DEVASSERT_AR_RELEASE_MINOR_VERSION) \
    )
    #error "AutoSar Version Numbers of Adc_Sar_Ip_HwAccess.h and Devassert.h are different"
#endif
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
/* Define the hardware register size when calculating bit positions */
#define ADC_SAR_IP_HW_REG_SIZE (32u)

/* Definitions to compute bit positions from channel index */
#define ADC_SAR_IP_CHAN_2_VECT(CHNIDX) ((CHNIDX) / ADC_SAR_IP_HW_REG_SIZE)
#define ADC_SAR_IP_CHAN_2_BIT(CHNIDX)  ((CHNIDX) % ADC_SAR_IP_HW_REG_SIZE)
/*==================================================================================================
                                     FUNCTION DEFINITIONS
==================================================================================================*/
#define ADC_START_SEC_CODE
#include "Adc_MemMap.h"

/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_Powerup
 * Description   : Turning on power to the analog portion of ADC
 *
 *END*************************************************************************/
static inline void Adc_Sar_Powerup(ADC_Type * const Base)
{
    Base->MCR &= ~(ADC_MCR_PWDN(1u));
}

/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_Powerdown
 * Description   : Turning off power to the analog portion of ADC
 *
 *END*************************************************************************/
static inline void Adc_Sar_Powerdown(ADC_Type * const Base)
{
    Base->MCR |= ADC_MCR_PWDN(1u);
}

#if FEATURE_ADC_HAS_HIGH_SPEED_ENABLE
/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_EnableHighSpeed
 * Description   : Enable a high-speed calibration or a high-speed conversion
 *
 *END*************************************************************************/
static inline void Adc_Sar_EnableHighSpeed(ADC_Type * const Base,
                                           boolean Enable)
{
    uint32 Amsio = Base->AMSIO;
    Amsio &= ~(ADC_AMSIO_HSEN_MASK);
    Amsio |= ADC_AMSIO_HSEN(Enable ? 3u : 0u);
    Base->AMSIO = Amsio;
}
#endif /* FEATURE_ADC_HAS_HIGH_SPEED_ENABLE */

/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_WriteThresholds
 * Description   : Write Watchdog low, high thresholds for a wdog register
 *
 *END*************************************************************************/
static inline void Adc_Sar_WriteThresholds(ADC_Type * const Base,
                                           const uint8 RegisterNumber,
                                           const uint16 HighThreshold,
                                           const uint16 LowThreshold)
{
#if ADC_SAR_IP_HAS_THRHLR_ARRAY
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(RegisterNumber < ADC_SAR_IP_THRHLR_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */
    uint32 Value = ADC_THRHLR_THRH(HighThreshold) |
                     ADC_THRHLR_THRL(LowThreshold);
    Base->THRHLR[RegisterNumber] = Value;

#else /* ADC_SAR_IP_HAS_THRHLR_ARRAY == 0 */

    uint32 Value = ADC_THRHLR0_THRH(HighThreshold) |
                     ADC_THRHLR0_THRL(LowThreshold);
    switch (RegisterNumber)
    {
        case 0:
            Base->THRHLR0 = Value;
            break;
        case 1:
            Base->THRHLR1 = Value;
            break;
        case 2:
            Base->THRHLR2 = Value;
            break;
        case 3:
            Base->THRHLR3 = Value;
            break;
        case 4:
            Base->THRHLR4 = Value;
            break;
        case 5:
            Base->THRHLR5 = Value;
            break;
#if (ADC_SAR_IP_THRHLR_COUNT > 6u)
        case 6:
            Base->THRHLR6 = Value;
            break;
        case 7:
            Base->THRHLR7 = Value;
            break;
#if (ADC_SAR_IP_THRHLR_COUNT > 8u)
        case 8:
            Base->THRHLR8 = Value;
            break;
        case 9:
            Base->THRHLR9 = Value;
            break;
        case 10:
            Base->THRHLR10 = Value;
            break;
        case 11:
            Base->THRHLR11 = Value;
            break;
        case 12:
            Base->THRHLR12 = Value;
            break;
        case 13:
            Base->THRHLR13 = Value;
            break;
        case 14:
            Base->THRHLR14 = Value;
            break;
        case 15:
            Base->THRHLR15 = Value;
            break;
#endif /* (ADC_SAR_IP_THRHLR_COUNT > 8u) */
#endif /* (ADC_SAR_IP_THRHLR_COUNT > 6u) */
        default:
            ; /* no-op */
            break;
    }
#endif /* ADC_SAR_IP_HAS_THRHLR_ARRAY */
}

#if (ADC_SAR_IP_HAS_CWSELR_UNROLLED == 1u)
/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_GetChannelWatchdogAddress
 * Description   : Returns the address of the specified CWSELR register
 *
 *END*************************************************************************/
static inline volatile uint32 * Adc_Sar_GetChannelWatchdogAddress(ADC_Type * const Base,
                                                                  const uint32 RegisterNumber)
{
    volatile uint32 * CwselrAddr = NULL_PTR;

    switch (RegisterNumber)
    {
#if ADC_SAR_IP_HAS_CWSELR0
        case 0u:
            CwselrAddr = &(Base->CWSELR0);
            break;
#endif /* ADC_SAR_IP_HAS_CWSELR0 */
#if ADC_SAR_IP_HAS_CWSELR1
        case 1u:
            CwselrAddr = &(Base->CWSELR1);
            break;
#endif /* ADC_SAR_IP_HAS_CWSELR1 */
#if ADC_SAR_IP_HAS_CWSELR2
        case 2u:
            CwselrAddr = &(Base->CWSELR2);
            break;
#endif /* ADC_SAR_IP_HAS_CWSELR2 */
#if ADC_SAR_IP_HAS_CWSELR3
        case 3u:
            CwselrAddr = &(Base->CWSELR3);
            break;
#endif /* ADC_SAR_IP_HAS_CWSELR3 */
#if ADC_SAR_IP_HAS_CWSELR4
        case 4u:
            CwselrAddr = &(Base->CWSELR4);
            break;
#endif /* ADC_SAR_IP_HAS_CWSELR4 */
#if ADC_SAR_IP_HAS_CWSELR5
        case 5u:
            CwselrAddr = &(Base->CWSELR5);
            break;
#endif /* ADC_SAR_IP_HAS_CWSELR5 */
#if ADC_SAR_IP_HAS_CWSELR6
        case 6u:
            CwselrAddr = &(Base->CWSELR6);
            break;
#endif /* ADC_SAR_IP_HAS_CWSELR6 */
#if ADC_SAR_IP_HAS_CWSELR7
        case 7u:
            CwselrAddr = &(Base->CWSELR7);
            break;
#endif /* ADC_SAR_IP_HAS_CWSELR7 */
#if ADC_SAR_IP_HAS_CWSELR8
        case 8u:
            CwselrAddr = &(Base->CWSELR8);
            break;
#endif /* ADC_SAR_IP_HAS_CWSELR8 */
#if ADC_SAR_IP_HAS_CWSELR9
        case 9u:
            CwselrAddr = &(Base->CWSELR9);
            break;
#endif /* ADC_SAR_IP_HAS_CWSELR9 */
#if ADC_SAR_IP_HAS_CWSELR10
        case 10u:
            CwselrAddr = &(Base->CWSELR10);
            break;
#endif /* ADC_SAR_IP_HAS_CWSELR10 */
#if ADC_SAR_IP_HAS_CWSELR11
        case 11u:
            CwselrAddr = &(Base->CWSELR11);
            break;
#endif /* ADC_SAR_IP_HAS_CWSELR11 */
        default:
            ; /* the RegisterNumber does not exist */
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
            DevAssert(FALSE);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */
            break;
    }

    return CwselrAddr;
}
#endif /* ADC_SAR_IP_HAS_CWSELR_UNROLLED == 1 */

#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_WriteChannelMapping
 * Description   :
 *
 *END*************************************************************************/
static inline void Adc_Sar_WriteChannelMapping(ADC_Type * const Base,
                                               const uint32 RegisterNumber,
                                               const uint32 FieldPosition,
                                               const uint32 Value)
{
    uint32 CwselrVal;
    uint32 CwselrMask;
    uint32 CwselrShift;
#if (ADC_SAR_IP_HAS_CWSELR_UNROLLED == 1u)
    volatile uint32 * Cwselr = Adc_Sar_GetChannelWatchdogAddress(Base, RegisterNumber);

    /* Each CWSELR register contains 8 watchdog selections according to 8 channels
       Each watchdog selection possibly needs maximum 4 bits for setting */
    CwselrShift = FieldPosition * ADC_CWSELR0_WSEL_CH1_SHIFT;
    CwselrMask = (uint32)ADC_CWSELR0_WSEL_CH0_MASK << CwselrShift;
    CwselrVal  = (*Cwselr) & (~CwselrMask);
    CwselrVal |= ADC_CWSELR0_WSEL_CH0(Value) << CwselrShift;
    *Cwselr      = CwselrVal;
#else /* ADC_SAR_IP_HAS_CWSELR_UNROLLED == 0 */
    CwselrShift = FieldPosition * ADC_CWSELR_WSEL_CH1_SHIFT;
    CwselrMask = (uint32)ADC_CWSELR_WSEL_CH0_MASK << CwselrShift;
    CwselrVal  = CWSELR(Base, RegisterNumber) & (~CwselrMask);
    CwselrVal |= ADC_CWSELR_WSEL_CH0(Value) << CwselrShift;
    CWSELR(Base, RegisterNumber) = CwselrVal;
#endif /* (ADC_SAR_IP_HAS_CWSELR_UNROLLED == 1u) */
}
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */

static inline void Adc_Sar_ResetWdogCWSELR(ADC_Type * const Base, const uint8 RegisterNumber)
{
#if (ADC_SAR_IP_HAS_CWSELR_UNROLLED == 1u)
    volatile uint32 * Cwselr = Adc_Sar_GetChannelWatchdogAddress(Base, (uint32)RegisterNumber);
    *Cwselr = 0U;
#else /* ADC_SAR_IP_HAS_CWSELR_UNROLLED == 0 */
    CWSELR(Base, RegisterNumber) = 0u;
#endif /* (ADC_SAR_IP_HAS_CWSELR_UNROLLED == 1u) */
}

#define ADC_STOP_SEC_CODE
#include "Adc_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* ADC_SAR_IP_HW_ACCESS_H */
