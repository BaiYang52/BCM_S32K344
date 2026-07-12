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

/**
*   @file
*
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
#include "Adc_Sar_Ip.h"
#include "Adc_Sar_Ip_Irq.h"
#include "Adc_Sar_Ip_HwAccess.h"
#include "Adc_Sar_Ip_TrustedFunctions.h"
#include "Reg_eSys.h"
#include "SchM_Adc.h"

#if defined(ADC_SAR_IP_DEV_ERROR_DETECT)
#include "OsIf.h"

/* local definitions on the limits of the clock frequency */
#define ADC_CLOCK_FREQ_MAX_RUNTIME     (80000000U)
#define ADC_CLOCK_FREQ_MAX_CALIBRATION (40000000U)

#endif /* defined(ADC_SAR_IP_DEV_ERROR_DETECT) */

#if (STD_ON == ADC_SAR_IP_ENABLE_USER_MODE_SUPPORT)
    /* USER_MODE_REG_PROT_ENABLED is defined for Base sanity check in RegLockMacros.h */
    #define USER_MODE_REG_PROT_ENABLED      (ADC_SAR_IP_ENABLE_USER_MODE_SUPPORT)
    #include "RegLockMacros.h"
#endif /* (STD_ON == ADC_SAR_IP_ENABLE_USER_MODE_SUPPORT) */

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ADC_SAR_IP_VENDOR_ID_C                      43
#define ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_C       4
#define ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_C       4
#define ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_C    0
#define ADC_SAR_IP_SW_MAJOR_VERSION_C               2
#define ADC_SAR_IP_SW_MINOR_VERSION_C               0
#define ADC_SAR_IP_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if Adc_Sar_Ip.c file and Adc_Sar_Ip.h file are of the same vendor */
#if (ADC_SAR_IP_VENDOR_ID_C != ADC_SAR_IP_VENDOR_ID)
    #error "Adc_Sar_Ip.c and Adc_Sar_Ip.h have different vendor ids"
#endif

/* Check if Adc_Sar_Ip.c file and Adc_Sar_Ip.h file are of the same Autosar version */
#if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_C != ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION) || \
     (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_C != ADC_SAR_IP_AR_RELEASE_MINOR_VERSION) || \
     (ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_C != ADC_SAR_IP_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Adc_Sar_Ip.c and Adc_Sar_Ip.h are different"
#endif

/* Check if Adc_Sar_Ip.c file and Adc_Sar_Ip.h file are of the same Software version */
#if ((ADC_SAR_IP_SW_MAJOR_VERSION_C != ADC_SAR_IP_SW_MAJOR_VERSION) || \
     (ADC_SAR_IP_SW_MINOR_VERSION_C != ADC_SAR_IP_SW_MINOR_VERSION) || \
     (ADC_SAR_IP_SW_PATCH_VERSION_C != ADC_SAR_IP_SW_PATCH_VERSION) \
    )
  #error "Software Version Numbers of Adc_Sar_Ip.c and Adc_Sar_Ip.h are different"
#endif

/* Check if Adc_Sar_Ip.c file and Adc_Sar_Ip_Irq.h file are of the same vendor */
#if (ADC_SAR_IP_VENDOR_ID_C != ADC_SAR_IP_VENDOR_ID_IRQ)
    #error "Adc_Sar_Ip.c and Adc_Sar_Ip_Irq.h have different vendor ids"
#endif

/* Check if Adc_Sar_Ip.c file and Adc_Sar_Ip_Irq.h file are of the same Autosar version */
#if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_C != ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_IRQ) || \
     (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_C != ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_IRQ) || \
     (ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_C != ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_IRQ) \
    )
    #error "AutoSar Version Numbers of Adc_Sar_Ip.c and Adc_Sar_Ip_Irq.h are different"
#endif

/* Check if Adc_Sar_Ip.c file and Adc_Sar_Ip_Irq.h file are of the same Software version */
#if ((ADC_SAR_IP_SW_MAJOR_VERSION_C != ADC_SAR_IP_SW_MAJOR_VERSION_IRQ) || \
     (ADC_SAR_IP_SW_MINOR_VERSION_C != ADC_SAR_IP_SW_MINOR_VERSION_IRQ) || \
     (ADC_SAR_IP_SW_PATCH_VERSION_C != ADC_SAR_IP_SW_PATCH_VERSION_IRQ) \
    )
  #error "Software Version Numbers of Adc_Sar_Ip.c and Adc_Sar_Ip_Irq.h are different"
#endif

/* Check if Adc_Sar_Ip.c file and Adc_Sar_Ip_HwAccess.h file are of the same vendor */
#if (ADC_SAR_IP_VENDOR_ID_C != ADC_SAR_IP_VENDOR_ID_HWACCESS)
    #error "Adc_Sar_Ip.c and Adc_Sar_Ip_HwAccess.h have different vendor ids"
#endif

/* Check if Adc_Sar_Ip.c file and Adc_Sar_Ip_HwAccess.h file are of the same Autosar version */
#if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_C != ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_HWACCESS) || \
     (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_C != ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_HWACCESS) || \
     (ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_C != ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_HWACCESS) \
    )
    #error "AutoSar Version Numbers of Adc_Sar_Ip.c and Adc_Sar_Ip_HwAccess.h are different"
#endif

/* Check if Adc_Sar_Ip.c file and Adc_Sar_Ip_HwAccess.h file are of the same Software version */
#if ((ADC_SAR_IP_SW_MAJOR_VERSION_C != ADC_SAR_IP_SW_MAJOR_VERSION_HWACCESS) || \
     (ADC_SAR_IP_SW_MINOR_VERSION_C != ADC_SAR_IP_SW_MINOR_VERSION_HWACCESS) || \
     (ADC_SAR_IP_SW_PATCH_VERSION_C != ADC_SAR_IP_SW_PATCH_VERSION_HWACCESS) \
    )
  #error "Software Version Numbers of Adc_Sar_Ip.c and Adc_Sar_Ip_HwAccess.h are different"
#endif

/* Check if Adc_Sar_Ip.c file and Adc_Sar_Ip_TrustedFunctions.h file are of the same vendor */
#if (ADC_SAR_IP_VENDOR_ID_C != ADC_SAR_IP_VENDOR_ID_TRUSTEDFUNCTIONS)
    #error "Adc_Sar_Ip.c and Adc_Sar_Ip_TrustedFunctions.h have different vendor ids"
#endif

/* Check if Adc_Sar_Ip.c file and Adc_Sar_Ip_TrustedFunctions.h file are of the same Autosar version */
#if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_C != ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_TRUSTEDFUNCTIONS) || \
     (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_C != ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_TRUSTEDFUNCTIONS) || \
     (ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_C != ADC_SAR_IP_AR_RELEASE_REVISION_VERSION_TRUSTEDFUNCTIONS) \
    )
    #error "AutoSar Version Numbers of Adc_Sar_Ip.c and Adc_Sar_Ip_TrustedFunctions.h are different"
#endif

/* Check if Adc_Sar_Ip.c file and Adc_Sar_Ip_TrustedFunctions.h file are of the same Software version */
#if ((ADC_SAR_IP_SW_MAJOR_VERSION_C != ADC_SAR_IP_SW_MAJOR_VERSION_TRUSTEDFUNCTIONS) || \
     (ADC_SAR_IP_SW_MINOR_VERSION_C != ADC_SAR_IP_SW_MINOR_VERSION_TRUSTEDFUNCTIONS) || \
     (ADC_SAR_IP_SW_PATCH_VERSION_C != ADC_SAR_IP_SW_PATCH_VERSION_TRUSTEDFUNCTIONS) \
    )
  #error "Software Version Numbers of Adc_Sar_Ip.c and Adc_Sar_Ip_TrustedFunctions.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if Adc_Sar_Ip.c file and Reg_eSys.h file are of the same Autosar version */
#if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_C != REG_ESYS_AR_RELEASE_MAJOR_VERSION) || \
     (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_C != REG_ESYS_AR_RELEASE_MINOR_VERSION)    \
    )
    #error "AutoSar Version Numbers of Adc_Sar_Ip.c and Reg_eSys.h are different"
#endif

#if defined(ADC_SAR_IP_DEV_ERROR_DETECT)
    /* Check if Adc_Sar_Ip.c file and OsIf.h file are of the same Autosar version */
    #if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_C != OSIF_AR_RELEASE_MAJOR_VERSION) || \
         (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_C != OSIF_AR_RELEASE_MINOR_VERSION)    \
        )
        #error "AutoSar Version Numbers of Adc_Sar_Ip.c and OsIf.h are different"
    #endif
#endif /* defined(ADC_SAR_IP_DEV_ERROR_DETECT) */

#if (STD_ON == ADC_SAR_IP_ENABLE_USER_MODE_SUPPORT)
    /* Checks against RegLockMacros.h */
    #if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_C != REGLOCKMACROS_AR_RELEASE_MAJOR_VERSION) || \
         (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_C != REGLOCKMACROS_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Adc_Sar_Ip.c and RegLockMacros.h are different"
    #endif
#endif /* (STD_ON == ADC_SAR_IP_ENABLE_USER_MODE_SUPPORT) */

/* Check if Adc_Sar_Ip.c file and SchM_Adc.h file are of the same version */
#if ((ADC_SAR_IP_AR_RELEASE_MAJOR_VERSION_C != SCHM_ADC_AR_RELEASE_MAJOR_VERSION) || \
     (ADC_SAR_IP_AR_RELEASE_MINOR_VERSION_C != SCHM_ADC_AR_RELEASE_MINOR_VERSION) \
    )
    #error "AutoSar Version Numbers of Adc_Sar_Ip.c and SchM_Adc.h are different"
#endif
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */

/*******************************************************************************
 * Pre-check
 ******************************************************************************/
#ifndef MCAL_ENABLE_USER_MODE_SUPPORT
    #if (STD_ON == ADC_SAR_IP_ENABLE_USER_MODE_SUPPORT)
        #error MCAL_ENABLE_USER_MODE_SUPPORT is not enabled. For running ADC in user mode the MCAL_ENABLE_USER_MODE_SUPPORT needs to be defined
    #endif /* (STD_ON == ADC_ENABLE_USER_MODE_SUPPORT) */
#endif /* ifndef MCAL_ENABLE_USER_MODE_SUPPORT*/

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
/* ADC_SAR_IP_INST_HAS_REG_<bitwidth> macros. Checks the availability of a
    register index based on the availability of the ADC channels
    for the particular u32Instance on the platform.
    <bitwidth> represents the width of the bitfield associated with each
    ADC channel in the register series.
*/
#define ADC_SAR_IP_INST_HAS_REG_4(inst,regIdx)  ((Adc_Sar_Ip_au32AdcChanBitmap[inst][(regIdx) / 4u] & (0xFFUL << (((regIdx) % 4u) * 8u))) != 0u)
#define ADC_SAR_IP_INST_HAS_REG_32(inst,regIdx) ((Adc_Sar_Ip_au32AdcChanBitmap[inst][(regIdx) / 32u] & (0x01UL << ((regIdx) % 32u))) != 0u)

/* <bitwidth> = 4. Four bits in the register correspond to one ADC channel */
#define ADC_SAR_IP_INST_HAS_CWSELRn(inst,x) (ADC_SAR_IP_INST_HAS_REG_4(inst,x))

/* <bitwidth> = 32. The entire 4 byte register is associated with one ADC channel */
#define ADC_SAR_IP_INST_HAS_CDRn(inst,n) (ADC_SAR_IP_INST_HAS_REG_32(inst,n))
#define ADC_SAR_IP_INST_HAS_CHANn(inst,n) (ADC_SAR_IP_INST_HAS_REG_32(inst,n))

#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */

#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE

#ifdef ADC_DSDR_DSD
#define ADC_SAR_IP_INST_HAS_DSDR(inst) ((Adc_Sar_Ip_au32AdcFeatureBitmap[inst] & (1UL << 0u)) != 0u)
#endif /* ADC_DSDR_DSD */
#define ADC_SAR_IP_INST_HAS_PSCR(inst) ((Adc_Sar_Ip_au32AdcFeatureBitmap[inst] & (1UL << 1u)) != 0u)
#define ADC_SAR_IP_INST_HAS_CTU(inst) ((Adc_Sar_Ip_au32AdcFeatureBitmap[inst] & (1UL << 2u)) != 0u)
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_FEATURE */

/* Macros for watchdog registers to help decode and compose bit mask for the functionality */
#define ADC_SAR_IP_WDG_REG_MASK_HIGH(registerIdx) ((uint32)((uint32)1UL << (((registerIdx) * 2u) + 1u)))
#define ADC_SAR_IP_WDG_REG_MASK_LOW(registerIdx)  ((uint32)((uint32)1UL << ((registerIdx) * 2u)))
/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
#define ADC_START_SEC_CONST_UNSPECIFIED
#include "Adc_MemMap.h"

/* Table of pBase addresses for ADC instances. */
static ADC_Type * const Adc_Sar_Ip_apxAdcBase[ADC_SAR_IP_INSTANCE_COUNT] = IP_ADC_BASE_PTRS;
#if FEATURE_ADC_HAS_TEMPSENSE_CHN
#if (ADC_SAR_IP_TEMPSENSE_ENABLED == STD_ON)
static TEMPSENSE_Type * const Adc_Sar_Ip_apxTempsenseBase[TEMPSENSE_INSTANCE_COUNT] = IP_TEMPSENSE_BASE_PTRS;
#endif /* (ADC_SAR_IP_TEMPSENSE_ENABLED == STD_ON) */
#endif

#define ADC_STOP_SEC_CONST_UNSPECIFIED
#include "Adc_MemMap.h"

#define ADC_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Adc_MemMap.h"
/* Global state structure */
static Adc_Sar_Ip_StateStructType Adc_Sar_Ip_axAdcSarState[ADC_SAR_IP_INSTANCE_COUNT];

#define ADC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Adc_MemMap.h"

#define ADC_START_SEC_CONST_32
#include "Adc_MemMap.h"

static const uint8 Adc_Sar_Ip_au8AdcGroupCount[ADC_SAR_IP_INSTANCE_COUNT] = FEATURE_ADC_MAX_GROUP_COUNT;

#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
static const uint32 Adc_Sar_Ip_au32AdcChanBitmap[ADC_SAR_IP_INSTANCE_COUNT][ADC_SAR_IP_NUM_GROUP_CHAN] = FEATURE_ADC_CHN_AVAIL_BITMAP;
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */

#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
static const uint32 Adc_Sar_Ip_au32AdcFeatureBitmap[ADC_SAR_IP_INSTANCE_COUNT] = FEATURE_ADC_FEAT_AVAIL_BITMAP;
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_FEATURE */

#define ADC_STOP_SEC_CONST_32
#include "Adc_MemMap.h"

/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   GLOBAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#define ADC_START_SEC_CODE
#include "Adc_MemMap.h"

#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
static inline uint8 Adc_Sar_GetResolution(const uint32 Instance);
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */

static inline uint16 Adc_Sar_GetMaskedResult(const uint32 Instance,
                                             const uint32 Cdr);
static inline uint32 Adc_Sar_GetMsrFlags(const uint32 Instance);
static inline uint32 Adc_Sar_GetIsrFlags(const uint32 Instance);
#if ADC_SAR_IP_HAS_ADCLKSEL
static inline uint32 Adc_Sar_GetADCLKSELValue(Adc_Sar_Ip_ClockSelType ClockSel);
#endif /* ADC_SAR_IP_HAS_ADCLKSEL */
static inline uint32 Adc_Sar_CollectMcrMasks(const uint32 Instance,
                                             const Adc_Sar_Ip_ConfigType * const Config,
                                             uint32 InitialMcr);
static inline void Adc_Sar_ConfigChannels(const uint32 Instance,
                                          const Adc_Sar_Ip_ChanConfigType * ChannelConfigsPtr,
                                          uint8 NumChannels);
static inline Adc_Sar_Ip_StatusType Adc_Sar_CheckSelfTestProgress(const uint32 Instance);
static uint32 Adc_Sar_GetConvResults(const uint32 Instance,
                                     const Adc_Sar_Ip_ConvChainType ChainType,
                                     uint16 * const ResultsRaw,
                                     Adc_Sar_Ip_ChanResultType * const ResultsStruct,
                                     const uint32 Length);

static void Adc_Sar_ResetWdog(const uint32 Instance);

#if FEATURE_ADC_HAS_CLOCK_DIVIDER
static inline void Adc_Sar_EnableClkDiv(const uint32 Instance,
                                        const boolean ClkDivEnable);
#endif /* FEATURE_ADC_HAS_CLOCK_DIVIDER */

#if ( \
     (STD_ON == ADC_SAR_IP_ECH_ENABLED) || (STD_ON == ADC_SAR_IP_JECH_ENABLED) || \
     ((FEATURE_ADC_HAS_CTU == 1U) && (STD_ON == ADC_SAR_IP_EOCTU_ENABLED)) \
    )
static inline void Adc_Sar_CheckAndCallNotification(const uint32 Instance,
                                                    void (*Callback)(void));
#endif /* (STD_ON == ADC_SAR_IP_ECH_ENABLED) .. */

#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
static inline void Adc_Sar_CheckAndCallEocNotification(uint32 Instance,
                                                       uint16 ChanIdx,
                                                       uint16 VectAdr,
                                                       const boolean ExecCallback);
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */

#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
static inline void Adc_Sar_CheckAndCallWorrNotification(uint32 Instance,
                                                        uint16 ChanIdx,
                                                        uint16 VectAdr);
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */

static inline void Adc_CheckAndCallAllChannelNotification(const uint32 Instance);

#if (FEATURE_ADC_HAS_INJ_EXT_TRIGGER || FEATURE_ADC_HAS_EXT_TRIGGER)
static inline void Adc_Sar_ConfigExternalTrigger(const uint32 Instance,
                                                 const Adc_Sar_Ip_ExtTriggerEdgeType TriggerEdge,
                                                 const uint32 TrgEdgeSetMask,
                                                 const uint32 TrgEdgeClrMask,
                                                 const uint32 TrigSrcMask);
#endif /* (FEATURE_ADC_HAS_INJ_EXT_TRIGGER || FEATURE_ADC_HAS_EXT_TRIGGER) */

#if FEATURE_ADC_HAS_TEMPSENSE_CHN
#if (ADC_SAR_IP_TEMPSENSE_ENABLED == STD_ON)
static inline uint32 Adc_Sar_TempsenseConvFp2Int(const uint32 FloatingPoint);
static inline uint16 Adc_Sar_TempsenseConvInt2Fp(const sint32 SignedInteger);
#endif /* (ADC_SAR_IP_TEMPSENSE_ENABLED == STD_ON) */
#endif /* FEATURE_ADC_HAS_TEMPSENSE_CHN */

static inline void Adc_Sar_SetNormalChain(const uint32 Instance,
                                          const uint32 * const Mask);
static inline void Adc_Sar_SetInjectedChain(const uint32 Instance,
                                            const uint32 * const Mask);

/* The function is used to reduce complexity for parent function */
static inline uint32 Adc_Sar_GetValueFromBool(const boolean bVal,
                                              const uint32 TrueVal,
                                              const uint32 FalseVal);

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_GetResolution
 * Description   : Returns Adc Sar resolution for conversion data
 *
 *END*************************************************************************/
static inline uint8 Adc_Sar_GetResolution(const uint32 Instance)
{
    uint8 Resolution;
    static const uint8 ResolutionArray[] = {14U, 12U, 10U, 8U}; /* maps each register bit value to resolution bit number */
    const ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];
    uint32 Calbistreg = AdcBasePtr->CALBISTREG;
    /* ResolutionArray can be used only for 2 bits resolutions. On S32K3 platforms only 2 bits are used so "& 3U" operation is needed */
    uint8 ResolutionBits = (uint8)(((Calbistreg & ADC_CALBISTREG_RESN_MASK) >> ADC_CALBISTREG_RESN_SHIFT) & 3U);

    Resolution = ResolutionArray[ResolutionBits];

    return Resolution;
}
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */

/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_GetMaskedResult
 * Description   : Returns result masked accordingly with alignment
 *
 *END*************************************************************************/
static inline uint16 Adc_Sar_GetMaskedResult(const uint32 Instance,
                                             const uint32 Cdr)
{
    uint32 CdrMask = ADC_CDR_CDATA_MASK;
    uint16 Result;
#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
    uint8 Resolution;
#else
    uint8 Resolution = ADC_SAR_IP_MAX_RESOLUTION;
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */

#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
    if (FALSE == Adc_Sar_Ip_axAdcSarState[Instance].BypassResolution)
    {
        Resolution = Adc_Sar_GetResolution(Instance);
    }
    else
    {
        Resolution = ADC_SAR_IP_RESULT_RESOLUTION;
    }
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */

    /* If the result is left aligned, adjust register mask accordingly */
    if (Adc_Sar_Ip_axAdcSarState[Instance].DataAlign == ADC_SAR_IP_DATA_ALIGNED_LEFT)
    {
        /* Assumption: the width of the register is less than 16 */
        CdrMask = (uint32)ADC_CDR_CDATA_MASK << (16u - Resolution);
#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
        Result = ((uint16)(Cdr & CdrMask)) >> ADC_CDR_CDATA_SHIFT;
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */
    }
#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
    else
    {
        CdrMask = ((uint32)ADC_CDR_CDATA_MASK >> 1u) & ((uint32)ADC_CDR_CDATA_MASK << (ADC_SAR_IP_RESULT_RESOLUTION - Resolution));
        Result = ((uint16)(Cdr & CdrMask)) >> (ADC_SAR_IP_RESULT_RESOLUTION - Resolution + ADC_CDR_CDATA_SHIFT);
    }
#else
    Result = ((uint16)(Cdr & CdrMask)) >> ADC_CDR_CDATA_SHIFT;
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */
    return Result;
}


/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_GetMsrFlags
 * Description   : Returns the flag status from MSR register
 *
 *END*************************************************************************/
static inline uint32 Adc_Sar_GetMsrFlags(const uint32 Instance)
{
    const ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];
    uint32 Msr = AdcBasePtr->MSR;
    uint32 Flags = 0U;

#if FEATURE_ADC_HAS_CALIBRATION
    Flags |= ((Msr & ADC_MSR_CALIBRTD_MASK) != 0U) ? ADC_SAR_IP_STATUS_FLAG_CALIBRATED : 0U;
#endif /* FEATURE_ADC_HAS_CALIBRATION */
    Flags |= ((Msr & ADC_MSR_NSTART_MASK) != 0U) ? ADC_SAR_IP_STATUS_FLAG_NORMAL_STARTED : 0U;
    Flags |= ((Msr & ADC_MSR_JABORT_MASK) != 0U) ? ADC_SAR_IP_STATUS_FLAG_INJECTED_ABORTED : 0U;
    Flags |= ((Msr & ADC_MSR_JSTART_MASK) != 0U) ? ADC_SAR_IP_STATUS_FLAG_INJECTED_STARTED : 0U;
#if FEATURE_ADC_HAS_CTU
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    if (ADC_SAR_IP_INST_HAS_CTU(Instance))
    {
#endif /*FEATURE_ADC_BAD_ACCESS_PROT_FEATURE*/
        Flags |= ((Msr & ADC_MSR_CTUSTART_MASK) != 0U) ? ADC_SAR_IP_STATUS_FLAG_CTU_STARTED : 0U;
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    }
#endif /*FEATURE_ADC_BAD_ACCESS_PROT_FEATURE*/
#endif /* FEATURE_ADC_HAS_CTU */
    Flags |= ((Msr & ADC_MSR_ACKO_MASK) != 0U) ? ADC_SAR_IP_STATUS_FLAG_AUTOCLOCKOFF : 0U;

    return Flags;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_GetIsrFlags
 * Description   : Returns the flag status from ISR register
 *
 *END*************************************************************************/
static inline uint32 Adc_Sar_GetIsrFlags(const uint32 Instance)
{
    const ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];
    uint32 Isr = AdcBasePtr->ISR;
    uint32 Flags = 0U;

    Flags |= ((Isr & ADC_ISR_EOC_MASK) != 0U) ? ADC_SAR_IP_NOTIF_FLAG_NORMAL_EOC : 0U;
    Flags |= ((Isr & ADC_ISR_ECH_MASK) != 0U) ? ADC_SAR_IP_NOTIF_FLAG_NORMAL_ENDCHAIN : 0U;
    Flags |= ((Isr & ADC_ISR_JEOC_MASK) != 0U) ? ADC_SAR_IP_NOTIF_FLAG_INJECTED_EOC : 0U;
    Flags |= ((Isr & ADC_ISR_JECH_MASK) != 0U) ? ADC_SAR_IP_NOTIF_FLAG_INJECTED_ENDCHAIN : 0U;
#if FEATURE_ADC_HAS_CTU
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    if (ADC_SAR_IP_INST_HAS_CTU(Instance))
    {
#endif /*FEATURE_ADC_BAD_ACCESS_PROT_FEATURE*/
        Flags |= ((Isr & ADC_ISR_EOCTU_MASK) != 0U) ? ADC_SAR_IP_NOTIF_FLAG_CTU_EOC : 0U;
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    }
#endif /*FEATURE_ADC_BAD_ACCESS_PROT_FEATURE*/
#endif /* FEATURE_ADC_HAS_CTU */

    return Flags;
}

#if ADC_SAR_IP_HAS_ADCLKSEL
/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_GetADCLKSELValue
 * Description   : This function returns the value that ADCLKSEL field should
 * have based on the input parameter.
 *
 *END*************************************************************************/
static inline uint32 Adc_Sar_GetADCLKSELValue(Adc_Sar_Ip_ClockSelType ClockSel)
{
#if FEATURE_ADC_HAS_CLKSEL_EXTENDED
    return ADC_MCR_ADCLKSEL(ClockSel);
#else
    uint32 Temp = 0u;
    switch (ClockSel)
    {
        case ADC_SAR_IP_CLK_FULL_BUS:
            /* Full bus clock when MCR[ADCLKSEL] = 1 */
            Temp = ADC_MCR_ADCLKSEL(1U);
            break;
        case ADC_SAR_IP_CLK_HALF_BUS:
            /* Half bus clock when MCR[ADCLKSEL] = 0 */
            break;
        default:
            /* no-op */
            break;
    }
    return Temp;
#endif /* FEATURE_ADC_HAS_CLKSEL_EXTENDED */
}
#endif /* ADC_SAR_IP_HAS_ADCLKSEL */

/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_CollectMcrMasks
 * Description   : The function collects the mask for MCR register from provided
 * structure. The funcion is used to reduce the complexity of Adc_Sar_Ip_Init()
 *
 *END*************************************************************************/
static inline uint32 Adc_Sar_CollectMcrMasks(const uint32 Instance,
                                             const Adc_Sar_Ip_ConfigType * const Config,
                                             uint32 InitialMcr)
{
    uint32 Mcr = InitialMcr;

    Mcr &= ~(ADC_MCR_MODE_MASK);
    Mcr |= ADC_MCR_MODE(Config->ConvMode);
#if ADC_SAR_IP_HAS_ADCLKSEL
    Mcr &= ~(ADC_MCR_ADCLKSEL_MASK);
    Mcr |= Adc_Sar_GetADCLKSELValue(Config->ClkSelect);
#endif /* ADC_SAR_IP_HAS_ADCLKSEL */
    Mcr &= ~(ADC_MCR_ACKO_MASK);
    Mcr |= Adc_Sar_GetValueFromBool(Config->AutoClockOff, ADC_MCR_ACKO_MASK, 0U);
    Mcr &= ~(ADC_MCR_OWREN_MASK);
    Mcr |= Adc_Sar_GetValueFromBool(Config->OverwriteEnable, ADC_MCR_OWREN_MASK, 0U);
    Mcr &= ~(ADC_MCR_WLSIDE_MASK);
    Mcr |= ADC_MCR_WLSIDE(Config->DataAlign);

#if FEATURE_ADC_HAS_CTU
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    if (ADC_SAR_IP_INST_HAS_CTU(Instance))
    {
#endif /*FEATURE_ADC_BAD_ACCESS_PROT_FEATURE*/
        switch (Config->CtuMode)
        {
            case ADC_SAR_IP_CTU_MODE_CONTROL:
                /* Already in CTU Control Mode CTU_MODE = 0 */
                Mcr &= ~(ADC_MCR_CTUEN_MASK);
                Mcr |= ADC_MCR_CTUEN(1U); /* Enable CTU */
                break;
#if FEATURE_ADC_HAS_CTU_TRIGGER_MODE
            case ADC_SAR_IP_CTU_MODE_TRIGGER:
                Mcr &= ~(ADC_MCR_CTUEN_MASK | ADC_MCR_CTU_MODE_MASK);
                Mcr |= ADC_MCR_CTU_MODE(1U); /* Set CTU to Trigger Mode CTU_MODE = 1 */
                Mcr |= ADC_MCR_CTUEN(1U); /* Enable CTU */
                break;
#endif /* FEATURE_ADC_HAS_CTU_TRIGGER_MODE */
            case ADC_SAR_IP_CTU_MODE_DISABLED:
                /* CTU is disabled (CTUEN = 0 and CTU_MODE = 0) */
                /* Pass through */
            default:
                ; /* no-op */
                break;
        }
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    }
#endif /*FEATURE_ADC_BAD_ACCESS_PROT_FEATURE*/
#endif /* FEATURE_ADC_HAS_CTU */
#if FEATURE_ADC_HAS_INJ_EXT_TRIGGER
    switch (Config->InjectedEdge)
    {
        case ADC_SAR_IP_EXT_TRIG_EDGE_FALLING:
            /* Already on falling edge JEDGE = 0 */
            Mcr &= ~(ADC_MCR_JTRGEN_MASK);
            Mcr |= ADC_MCR_JTRGEN_MASK; /* enable Injected trigger */
            break;
        case ADC_SAR_IP_EXT_TRIG_EDGE_RISING:
            /* Enable Injected trigger and set to rising edge JEDGE = 1 */
            Mcr &= ~(ADC_MCR_JEDGE_MASK | ADC_MCR_JTRGEN_MASK);
            Mcr |= (ADC_MCR_JEDGE_MASK | ADC_MCR_JTRGEN_MASK);
            break;
        case ADC_SAR_IP_EXT_TRIG_EDGE_DISABLED:
            /* Already set to injected trigger disabled (JTRGEN = 0 and JEDGE = 0) */
        default:
            ; /* no-op */
            break;
    }
#endif /* FEATURE_ADC_HAS_INJ_EXT_TRIGGER */

#if FEATURE_ADC_HAS_EXT_TRIGGER
    /* Get normal trigger masks */
    Mcr &= ~(ADC_MCR_EDGE_MASK);
    Mcr |= (Config->ExtTrigger == ADC_SAR_IP_EXT_TRIG_EDGE_RISING) ? (ADC_MCR_EDGE(1U)) : 0U;
    if (Config->ExtTrigger != ADC_SAR_IP_EXT_TRIG_EDGE_DISABLED)
    {
        Mcr &= ~(ADC_MCR_TRGEN_MASK);
        Mcr |= Adc_Sar_GetValueFromBool(Config->NormalExtTrgEn, ADC_MCR_TRGEN_MASK, 0U);
#if FEATURE_ADC_HAS_AUX_EXT_TRIGGER
        Mcr &= ~(ADC_MCR_XSTRTEN_MASK);
        Mcr |= Adc_Sar_GetValueFromBool(Config->NormalAuxExtTrgEn, ADC_MCR_XSTRTEN_MASK, 0U);
#endif /* FEATURE_ADC_HAS_AUX_EXT_TRIGGER */
    }
#endif /* FEATURE_ADC_HAS_EXT_TRIGGER */

#if FEATURE_ADC_HAS_AVERAGING
    Mcr &= ~(ADC_MCR_AVGEN_MASK);
    Mcr |= Adc_Sar_GetValueFromBool(Config->AvgEn, ADC_MCR_AVGEN_MASK, 0U);
    Mcr &= ~(ADC_MCR_AVGS_MASK);
    Mcr |= ADC_MCR_AVGS(Config->AvgSel);
#else
#if FEATURE_ADC_HAS_CALIBRATION
    /* On platforms that use AVGEN only for calibration, reset to default value */
    Mcr |= ADC_MCR_AVGEN_MASK;
#endif /* FEATURE_ADC_HAS_CALIBRATION */
#endif /* FEATURE_ADC_HAS_AVERAGING */

#if FEATURE_ADC_HAS_CALIBRATION
#if ADC_SAR_IP_CALIBRATION_USES_MCR
    /* Reset NRSMPL to default value */
    Mcr &= ~(ADC_MCR_NRSMPL_MASK);
    Mcr |= ADC_MCR_NRSMPL_MASK;
#endif /* ADC_SAR_IP_CALIBRATION_USES_MCR */
#endif /* FEATURE_ADC_HAS_CALIBRATION */

    return Mcr;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_ConfigChannels
 * Description   : Configure a list of channels by enabling channel notifications
 * and setting presampling.
 *END*************************************************************************/
static inline void Adc_Sar_ConfigChannels(const uint32 Instance,
                                          const Adc_Sar_Ip_ChanConfigType * ChannelConfigsPtr,
                                          uint8 NumChannels)
{
    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];
    uint8 Index;
    uint32 VectAdr;
    uint32 VectBit;
    uint32 CimrMask[ADC_SAR_IP_NUM_GROUP_CHAN];
    uint32 CwenrMask[ADC_SAR_IP_NUM_GROUP_CHAN];
    uint32 DmarMask[ADC_SAR_IP_NUM_GROUP_CHAN];
#if FEATURE_ADC_HAS_PRESAMPLING
    uint32 PsrMask[ADC_SAR_IP_NUM_GROUP_CHAN];
#endif /* FEATURE_ADC_HAS_PRESAMPLING */
    const Adc_Sar_Ip_ChanConfigType * ChnConfig;

    for (Index = 0U; Index < Adc_Sar_Ip_au8AdcGroupCount[Instance]; Index++)
    {
        CimrMask[Index]  = 0UL;
        CwenrMask[Index] = 0UL;
        DmarMask[Index]  = 0UL;
#if FEATURE_ADC_HAS_PRESAMPLING
        PsrMask[Index]   = 0UL;
#endif /* FEATURE_ADC_HAS_PRESAMPLING */
    }

    for (Index = 0U; Index < NumChannels; Index++)
    {
        ChnConfig = &(ChannelConfigsPtr[Index]);
#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
        /* Each CWSELR register contains 8 watchdog selections according to 8 channels
           Each watchdog selection possibly needs maximum 4 bits for setting
           This is to calculate the CWSELR register index and position of channel on that register */
        VectAdr = (uint32)ChnConfig->ChanIndex / 8U;
        VectBit = (uint32)ChnConfig->ChanIndex % 8U;
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
        if (ADC_SAR_IP_INST_HAS_CWSELRn(Instance, VectAdr))
        {
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
            Adc_Sar_WriteChannelMapping(AdcBasePtr, VectAdr, VectBit, ChnConfig->WdgThreshRegIndex);
            /* Store threshold register index for configured channel */
            Adc_Sar_Ip_axAdcSarState[Instance].ChanWdgThresholdIndex[ChnConfig->ChanIndex] = ChnConfig->WdgThreshRegIndex;
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
        }
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */

        VectAdr = ADC_SAR_IP_CHAN_2_VECT((uint32)ChnConfig->ChanIndex);
        VectBit = ADC_SAR_IP_CHAN_2_BIT((uint32)ChnConfig->ChanIndex);
        /* Collect bit-mask for WDG, EOC, DMA and Presample */
#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
        CimrMask[VectAdr]  |= Adc_Sar_GetValueFromBool(ChnConfig->EndOfConvNotification, (1UL << VectBit), 0UL);
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */
#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
        CwenrMask[VectAdr] |= Adc_Sar_GetValueFromBool(ChnConfig->WdgNotificationEn,       (1UL << VectBit), 0UL);
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */
        DmarMask[VectAdr]  |= Adc_Sar_GetValueFromBool(ChnConfig->EndOfConvDmaEnable,    (1UL << VectBit), 0UL);
#if FEATURE_ADC_HAS_PRESAMPLING
        PsrMask[VectAdr]   |= Adc_Sar_GetValueFromBool(ChnConfig->PresamplingEnable,     (1UL << VectBit), 0UL);
#endif /* FEATURE_ADC_HAS_PRESAMPLING */
    }

    /* Enable WDG, EOC, DMA and Presample */
    for (Index = 0U; Index < Adc_Sar_Ip_au8AdcGroupCount[Instance]; Index++)
    {
        CIMR(AdcBasePtr, Index) = CimrMask[Index];
        CWENR(AdcBasePtr, Index) = CwenrMask[Index];
        DMAR(AdcBasePtr, Index) = DmarMask[Index];
#if FEATURE_ADC_HAS_PRESAMPLING
        PSR(AdcBasePtr, Index) = PsrMask[Index];
#endif /* FEATURE_ADC_HAS_PRESAMPLING */
    }
}

/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_CheckSelfTestProgress
 * Description   : Configures watchdog threshold with provided array of configuration
 *
 *END*************************************************************************/
static inline Adc_Sar_Ip_StatusType Adc_Sar_CheckSelfTestProgress(const uint32 Instance)
{
    const ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];
    Adc_Sar_Ip_StatusType Status = ADC_SAR_IP_STATUS_SUCCESS;
#if FEATURE_ADC_HAS_SELFTEST_STCR1
    uint32 Reg = 0U;
#endif /* FEATURE_ADC_HAS_SELFTEST_STCR1 */
    uint32 TimeoutTicks = OsIf_MicrosToTicks(ADC_SAR_IP_TIMEOUT_VAL, ADC_SAR_IP_TIMEOUT_TYPE);
    uint32 CurrentTicks = OsIf_GetCounter(ADC_SAR_IP_TIMEOUT_TYPE);
    uint32 ElapsedTicks;
    uint8 Index;

    /*  Each for loop checks the conversion of all steps in self-test S algorithm
    *   ForLoop(0): while (AlgS-start-1) ->S0 -> S1 -> S2 -> while (Alg-end-1) -> C0 to C11 ->
    *   ForLoop(1): while (AlgS-start-2) ->S0 -> S1 -> S2 -> while (Alg-end-2)
    */
    for (Index = 0U; Index < 2U; Index++)
    {
        /* Wait until Alg S has started*/
        ElapsedTicks = 0U;
        while (((AdcBasePtr->MSR & ADC_MSR_SELF_TEST_S_MASK) != ADC_MSR_SELF_TEST_S_MASK) && (ElapsedTicks < TimeoutTicks))
        {
            ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, ADC_SAR_IP_TIMEOUT_TYPE);
        }
        if (ElapsedTicks >= TimeoutTicks)
        {
            Status = ADC_SAR_IP_STATUS_TIMEOUT;
        }
        if (Status == ADC_SAR_IP_STATUS_SUCCESS)
        {
            /* Wait until Alg S has finished*/
            ElapsedTicks = 0U;
            while (((AdcBasePtr->MSR & ADC_MSR_SELF_TEST_S_MASK) == ADC_MSR_SELF_TEST_S_MASK) && (ElapsedTicks < TimeoutTicks))
            {
                ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, ADC_SAR_IP_TIMEOUT_TYPE);
            }
            if (ElapsedTicks >= TimeoutTicks)
            {
                Status = ADC_SAR_IP_STATUS_TIMEOUT;
            }
        }
        if (Status == ADC_SAR_IP_STATUS_TIMEOUT)
        {
            /* Break the loop if timeout occured in order to increase performance */
            break;
        }
    }
#if FEATURE_ADC_HAS_SELFTEST_STCR1
    if (Status != ADC_SAR_IP_STATUS_TIMEOUT)
    {
        Reg |= ADC_STSR1_ERR_S0_MASK | ADC_STSR1_ERR_S1_MASK | ADC_STSR1_ERR_S2_MASK | ADC_STSR1_ERR_C_MASK;

        if ((AdcBasePtr->STSR1 & Reg) != 0UL)
        {
            Status = ADC_SAR_IP_STATUS_ERROR;
        }
    }
#endif /* FEATURE_ADC_HAS_SELFTEST_STCR1 */

    return Status;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_GetConvResults
 * Description   : Reads the conversion results to output arrays
 * Params:
 *  - Instance : the ADC Instance to read
 *  - ChainType : the conversion chain (Normal, Injected or CTU)
 *  - ResultsRaw : an uint16 array to write only conversion data
 *  - ResultsStruct : an Adc_Sar_Ip_ChanResultType array to write detailed information
                      about each conversion result
 *  - length : the maximum size of ResultsRaw and ResultsStruct
 *
 * ResultsRaw or ResultsStruct can be NULL, but not both, as the function will
 * have nowhere to write the results.
 *
 *END*************************************************************************/
static uint32 Adc_Sar_GetConvResults(const uint32 Instance,
                                     const Adc_Sar_Ip_ConvChainType ChainType,
                                     uint16 * const ResultsRaw,
                                     Adc_Sar_Ip_ChanResultType * const ResultsStruct,
                                     const uint32 Length)
{
    uint32 Index = 0u;
    boolean LengthExceeded = FALSE;
    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];
    uint32 VectAdr;
    uint32 VectBit;
    uint8 ChnIdx;
    uint32 Cdr;
    const uint16 AdcChanCount[ADC_SAR_IP_INSTANCE_COUNT][ADC_SAR_IP_NUM_GROUP_CHAN] = FEATURE_ADC_MAX_CHN_COUNT;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    /* ResultsRaw and ResultsStruct cannot be both NULL */
    DevAssert((ResultsRaw != NULL_PTR) || (ResultsStruct != NULL_PTR));
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    /* Go through each channel group */
    for (VectAdr = 0u; VectAdr < Adc_Sar_Ip_au8AdcGroupCount[Instance]; VectAdr++)
    {
        /* go through each bit in the group, check if there is a completed conversion */
        for (VectBit = 0u; VectBit < AdcChanCount[Instance][VectAdr]; VectBit++)
        {
            ChnIdx = (uint8)((VectAdr * 32u) + VectBit);
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
            if (!(ADC_SAR_IP_INST_HAS_CDRn(Instance, ChnIdx)))
            {
                continue; /* skip if the CDR register is not available */
            }
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
            Cdr = CDR(AdcBasePtr, ChnIdx);
            if ((ADC_CDR_RESULT((uint32)ChainType) | ADC_CDR_VALID_MASK) == (Cdr & (ADC_CDR_RESULT_MASK | ADC_CDR_VALID_MASK)))
            {
                /* if the result type matches the one request by ChainType
                   and the data is valid, write to the output array(s)
                */
                if (ResultsRaw != NULL_PTR)
                {
                    ResultsRaw[Index] = Adc_Sar_GetMaskedResult(Instance, Cdr);
                }
                if (ResultsStruct != NULL_PTR)
                {
                    ResultsStruct[Index].ConvData = Adc_Sar_GetMaskedResult(Instance, Cdr);
                    ResultsStruct[Index].AdcChnIdx = ChnIdx;
                    ResultsStruct[Index].ValidFlag = ((Cdr & ADC_CDR_VALID_MASK) != 0U) ? TRUE : FALSE;
                    ResultsStruct[Index].OverWrittenFlag = ((Cdr & ADC_CDR_OVERW_MASK) != 0U) ? TRUE : FALSE;
                }
                /* increment the current Index and reset the CEOCFR flag */
                CEOCFR(AdcBasePtr, VectAdr) = ((uint32)1UL << VectBit); /* w1c bit */
                Index++;
                if (Index >= Length)
                {
                    /* We have filled the output buffer, exit the loop.
                     * Data may still exist in the result registers, which won't be reached.
                     */
                    LengthExceeded = TRUE;
                    break;
                }
            }
        }
        if (LengthExceeded)
        {
            break;
        }
    }
    return Index;
}

/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_ResetWdog
 * Description   : Reset and disable the Analog Watchdog feature
 *
 *END*************************************************************************/
static void Adc_Sar_ResetWdog(const uint32 Instance)
{
    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];
    uint8 Index;

    for (Index = 0u; Index < Adc_Sar_Ip_au8AdcGroupCount[Instance]; Index++)
    {
        CWENR(AdcBasePtr, Index) = 0u;
        AWORR(AdcBasePtr, Index) = 0xFFFFFFFFu; /* w1c bits */
    }

    for (Index = 0u; Index < ADC_SAR_IP_THRHLR_COUNT; Index++)
    {
        Adc_Sar_WriteThresholds(AdcBasePtr, Index, 0xFFFFu, 0u);
    }

    for (Index = 0u; Index < ADC_SAR_IP_CWSELR_COUNT; Index++)
    {
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
        if (!(ADC_SAR_IP_INST_HAS_CWSELRn(Instance, Index)))
        {
            continue; /* skip register if it's not available */
        }
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
        Adc_Sar_ResetWdogCWSELR(AdcBasePtr, Index);
    }
}

#if FEATURE_ADC_HAS_CLOCK_DIVIDER
/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_EnableClkDiv
 * Description   : Configure clock divider
 *
 *END*************************************************************************/
static inline void Adc_Sar_EnableClkDiv(const uint32 Instance,
                                        const boolean ClkDivEnable)
{
    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];

    /* Enable clock divider */
    if (ClkDivEnable)
    {
        AdcBasePtr->CLKDIV_CTRL |= ADC_CLKDIV_CTRL_CLKDIV_ENABLE(1U);
    }
    else
    {
        AdcBasePtr->CLKDIV_CTRL &= ~(ADC_CLKDIV_CTRL_CLKDIV_ENABLE_MASK);
    }
}
#endif /* FEATURE_ADC_HAS_CLOCK_DIVIDER */

#if ( \
     (STD_ON == ADC_SAR_IP_ECH_ENABLED) || (STD_ON == ADC_SAR_IP_JECH_ENABLED) || \
     ((FEATURE_ADC_HAS_CTU == 1U) && (STD_ON == ADC_SAR_IP_EOCTU_ENABLED)) \
    )
/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_CheckAndCallNotification
 * Description   : Checks if the given callback is not NULL then calls it
 *
 *END*************************************************************************/
static inline void Adc_Sar_CheckAndCallNotification(const uint32 Instance,
                                                    void (*Callback)(void))
{
    if ((Callback != NULL_PTR) && (Adc_Sar_Ip_axAdcSarState[Instance].InitStatus))
    {
        Callback();
    }
}
#endif /* (STD_ON == ADC_SAR_IP_ECH_ENABLED) .. */

#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_CheckAndCallEocNotification
 * Description   : Checks if the given channel interrupt was triggered, clears
 *                 the interrupt, check if it is not spurious, checks that the
 *                 callback is not null and calls it if ExecCallback is set to true.
 *
 *END*************************************************************************/
static inline void Adc_Sar_CheckAndCallEocNotification(uint32 Instance,
                                                       uint16 ChanIdx,
                                                       uint16 VectAdr,
                                                       const boolean ExecCallback)
{
    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];
    const uint32 Mask = (uint32)1UL << ADC_SAR_IP_CHAN_2_BIT(ChanIdx);
    const uint32 Ceocfr = (CEOCFR(AdcBasePtr, VectAdr) & Mask);
    const uint32 Cimr   = (CIMR(AdcBasePtr, VectAdr) & Mask);

    if (Ceocfr != 0U)
    {
        /* Clears flag by w1c */
        CEOCFR(AdcBasePtr, VectAdr) = Mask;
        /*
         * Check if the callback is not null, if the interrupt was spurious
         * and if the driver was initialized.
         */
        if ((Cimr != 0U) && (ExecCallback) && (Adc_Sar_Ip_axAdcSarState[Instance].InitStatus) &&
            (Adc_Sar_Ip_axAdcSarState[Instance].EndOfConvNotification != NULL_PTR))
        {
            Adc_Sar_Ip_axAdcSarState[Instance].EndOfConvNotification(ChanIdx);
        }
    }
}
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */

#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_CheckAndCallWorrNotification
 * Description   : Checks if the given channel interrupt was triggered, clears
 *                 the interrupt, check if it is not spurious, checks that the
 *                 callback is not null and calls it if ExecCallback is set to true.
 *
 *END*************************************************************************/
static inline void Adc_Sar_CheckAndCallWorrNotification(uint32 Instance,
                                                        uint16 ChanIdx,
                                                        uint16 VectAdr)
{
    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];
    const uint32 Mask = (uint32)1UL << ADC_SAR_IP_CHAN_2_BIT(ChanIdx);
    const uint32 Aworr = (AWORR(AdcBasePtr, VectAdr) & Mask);
    const uint32 Cwenr = (CWENR(AdcBasePtr, VectAdr) & Mask);
    const uint32 Wtisr = AdcBasePtr->WTISR;
    const uint32 Wtimr = AdcBasePtr->WTIMR;
    uint8 WdgFlags = 0U;
    uint32 ThresholdIndex;

    if (Aworr != 0U)
    {
        /* Clears flag by w1c */
        AWORR(AdcBasePtr, VectAdr) = Mask;
        /*
         * Check if the callback is not null, if the interrupt was spurious
         * and if the driver was initialized.
         */
        if ((Cwenr != 0U) && (Adc_Sar_Ip_axAdcSarState[Instance].InitStatus) &&
            (Adc_Sar_Ip_axAdcSarState[Instance].WdgOutOfRangeNotification != NULL_PTR))
        {
            /* Get the mapping threshold register index corresponding to the configured channel */
            ThresholdIndex = Adc_Sar_Ip_axAdcSarState[Instance].ChanWdgThresholdIndex[ChanIdx];

            /* Check if the high/low flags were not set spuriously. */
            if ((Wtimr & (ADC_SAR_IP_WDG_REG_MASK_LOW(ThresholdIndex))) != 0u)
            {
                WdgFlags |= ((Wtisr & ADC_SAR_IP_WDG_REG_MASK_LOW(ThresholdIndex)) != 0u) ? ADC_SAR_IP_WDG_LOW_FLAG : 0u;
            }
            if ((Wtimr & (ADC_SAR_IP_WDG_REG_MASK_HIGH(ThresholdIndex))) != 0u)
            {
                WdgFlags |= ((Wtisr & ADC_SAR_IP_WDG_REG_MASK_HIGH(ThresholdIndex)) != 0u) ? ADC_SAR_IP_WDG_HIGH_FLAG : 0u;
            }
            if (WdgFlags != 0u)
            {
                Adc_Sar_Ip_axAdcSarState[Instance].WdgOutOfRangeNotification(ChanIdx, WdgFlags);
            }
        }
    }
}
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */

/*LOCAL FUNCTION*********************************************************************
 *
 * Function Name : Adc_CheckAndCallAllChannelNotification
 * Description   : Check for all watchdog interrupts and for all individual channel interrupts
 * if it was detected. And then call the appropriate notification
 *
 *END*********************************************************************************/
static inline void Adc_CheckAndCallAllChannelNotification(const uint32 Instance)
{
    uint16 VectAdr;
    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];
#if ((STD_ON == ADC_SAR_IP_WDG_ENABLED) || (STD_ON == ADC_SAR_IP_EOC_ENABLED))
    uint16 PhyChan;
    uint16 ChanCnt;
    const uint16 AdcChanCount[ADC_SAR_IP_INSTANCE_COUNT][ADC_SAR_IP_NUM_GROUP_CHAN] = FEATURE_ADC_MAX_CHN_COUNT;
#endif /* ((STD_ON == ADC_SAR_IP_WDG_ENABLED) || (STD_ON == ADC_SAR_IP_EOC_ENABLED)) */
#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
    boolean ChannelTriggered = FALSE;

    /* Check if the interrupt was spurious */
    if ((AdcBasePtr->IMR & (ADC_IMR_MSKEOC_MASK | ADC_IMR_MSKJEOC_MASK)) != 0U)
    {
        ChannelTriggered = TRUE;
    }
#else
    uint32 TempValue;
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */

    for (VectAdr = 0U; VectAdr < Adc_Sar_Ip_au8AdcGroupCount[Instance]; VectAdr++)
    {
#if (STD_OFF == ADC_SAR_IP_EOC_ENABLED)
        TempValue = CEOCFR(AdcBasePtr, VectAdr);
        if (TempValue != 0U)
        {
            /* Clears spurious flag by w1c when EoC interrupt is not enabled */
            CEOCFR(AdcBasePtr, VectAdr) = TempValue;
        }
#endif /* (STD_OFF == ADC_SAR_IP_EOC_ENABLED) */

#if ((STD_ON == ADC_SAR_IP_WDG_ENABLED) || (STD_ON == ADC_SAR_IP_EOC_ENABLED))
        for (ChanCnt = 0U; ChanCnt < AdcChanCount[Instance][VectAdr]; ChanCnt++)
        {
            PhyChan = (ChanCnt + (uint16)(ADC_SAR_IP_HW_REG_SIZE * VectAdr));

#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
            /* Check end of conversion interrupt */
            Adc_Sar_CheckAndCallEocNotification(Instance, PhyChan, VectAdr, ChannelTriggered);
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */

#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
            /* Check watchdog out of range interrupt */
            Adc_Sar_CheckAndCallWorrNotification(Instance, PhyChan, VectAdr);
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */
        }
#endif /* ((STD_ON == ADC_SAR_IP_WDG_ENABLED) || (STD_ON == ADC_SAR_IP_EOC_ENABLED)) */
    }
}

#if (FEATURE_ADC_HAS_INJ_EXT_TRIGGER || FEATURE_ADC_HAS_EXT_TRIGGER)
/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_ConfigExternalTrigger
* Description   : This function configures the external trigger.
*
* END**************************************************************************/
static inline void Adc_Sar_ConfigExternalTrigger(const uint32 Instance,
                                                 const Adc_Sar_Ip_ExtTriggerEdgeType TriggerEdge,
                                                 const uint32 TrgEdgeSetMask,
                                                 const uint32 TrgEdgeClrMask,
                                                 const uint32 TrigSrcMask)
{
    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_21();

    switch (TriggerEdge)
    {
        case ADC_SAR_IP_EXT_TRIG_EDGE_FALLING:
            AdcBasePtr->MCR &= ~TrgEdgeSetMask;  /* set to falling edge 0 */
            AdcBasePtr->MCR |= TrigSrcMask;    /* enable external trigger */
            break;
        case ADC_SAR_IP_EXT_TRIG_EDGE_RISING:
            /* enable external trigger and set to rising edge EDGE = 1 */
            AdcBasePtr->MCR |= (TrgEdgeSetMask | TrigSrcMask);
            break;
        case ADC_SAR_IP_EXT_TRIG_EDGE_DISABLED:
            /* disable external trigger and reset edge field */
            AdcBasePtr->MCR &= ~(TrgEdgeClrMask | TrigSrcMask);
            break;
        default:
            ; /* no-op */
            break;
    }

    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_21();
}
#endif /* (FEATURE_ADC_HAS_INJ_EXT_TRIGGER || FEATURE_ADC_HAS_EXT_TRIGGER) */

#if FEATURE_ADC_HAS_TEMPSENSE_CHN
#if (ADC_SAR_IP_TEMPSENSE_ENABLED == STD_ON)
/*FUNCTION**********************************************************************
 *
 * Function Name : Adc_Sar_TempsenseConvFp2Int
 * Description   : The function converts the number in signed fixed point format
 * (1,11,4) to the integer number multipled with ADC_SAR_IP_TEMPSENSE_MULTIPLIER to avoid
 * rounding the decimal part.
 *
 *END**************************************************************************/
static inline uint32 Adc_Sar_TempsenseConvFp2Int(const uint32 FloatingPoint)
{
    uint32 RetVal;

    /* The integer and decimal part multiple with ADC_SAR_IP_TEMPSENSE_MULTIPLIER to avoid rounding the decimal part */
    RetVal  = ((FloatingPoint & ADC_SAR_IP_TEMPSENSE_INTEGER_MASK) >> ADC_SAR_IP_TEMPSENSE_INTEGER_SHIFT) * ADC_SAR_IP_TEMPSENSE_MULTIPLIER;
    RetVal += ((FloatingPoint & ADC_SAR_IP_TEMPSENSE_DECIMAL_MASK) * ADC_SAR_IP_TEMPSENSE_MULTIPLIER) / (ADC_SAR_IP_TEMPSENSE_DECIMAL_MASK + 1U);

    return RetVal;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : Adc_Sar_TempsenseConvInt2Fp
 * Description   : The function converts the number in integer number to signed fixed
 * point format (1,11,4). The integer number parameter is multipled with ADC_SAR_IP_TEMPSENSE_MULTIPLIER
 *
 *END**************************************************************************/
static inline uint16 Adc_Sar_TempsenseConvInt2Fp(const sint32 SignedInteger)
{
    uint16 RetVal;
    uint32 Temp;

    if (SignedInteger < 0)
    {
        RetVal = ADC_SAR_IP_TEMPSENSE_SIGN_MASK;
        Temp = (uint32)(- SignedInteger);
    }
    else
    {
        RetVal = 0U;
        Temp = (uint32)SignedInteger;
    }

    /* Calculates the integer part */
    RetVal |= (uint16)(((Temp / ADC_SAR_IP_TEMPSENSE_MULTIPLIER) << ADC_SAR_IP_TEMPSENSE_INTEGER_SHIFT) & ADC_SAR_IP_TEMPSENSE_INTEGER_MASK);
    /* Calculates the decimal part */
    RetVal |= (uint16)(((Temp % ADC_SAR_IP_TEMPSENSE_MULTIPLIER) * (ADC_SAR_IP_TEMPSENSE_DECIMAL_MASK + 1U)) / ADC_SAR_IP_TEMPSENSE_MULTIPLIER);

    return RetVal;
}
#endif /* (ADC_SAR_IP_TEMPSENSE_ENABLED == STD_ON) */
#endif /* FEATURE_ADC_HAS_TEMPSENSE_CHN */

/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_Ip_IRQHandler
 * Description   : Handles Adc Sar interrupts
 *
 * @implements     Adc_Sar_Ip_IRQHandler_Activity
 *END*************************************************************************/
void Adc_Sar_Ip_IRQHandler(const uint32 Instance)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];

/* Check which interrupt was triggered, clear it and call the associated callback */
#if FEATURE_ADC_HAS_CTU
    if ((AdcBasePtr->ISR & ADC_ISR_EOCTU_MASK) != 0U)
    {
        AdcBasePtr->ISR = ADC_ISR_EOCTU_MASK;
        /* Check if the interrupt was spurious */
    #if (STD_ON == ADC_SAR_IP_EOCTU_ENABLED)
        if ((AdcBasePtr->IMR & ADC_IMR_MSKEOCTU_MASK) != 0U)
        {
            Adc_Sar_CheckAndCallNotification(Instance, Adc_Sar_Ip_axAdcSarState[Instance].EndOfCtuConversionNotification);
        }
    #endif /* (STD_ON == ADC_SAR_IP_EOCTU_ENABLED) */
    }
#endif /* FEATURE_ADC_HAS_CTU */
    if ((AdcBasePtr->ISR & ADC_ISR_ECH_MASK) != 0U)
    {
        AdcBasePtr->ISR = ADC_ISR_ECH_MASK;
        /* Check if the interrupt was spurious */
    #if (STD_ON == ADC_SAR_IP_ECH_ENABLED)
        if ((AdcBasePtr->IMR & ADC_IMR_MSKECH_MASK) != 0U)
        {
            Adc_Sar_CheckAndCallNotification(Instance, Adc_Sar_Ip_axAdcSarState[Instance].EndOfNormalChainNotification);
        }
    #endif /* (STD_ON == ADC_SAR_IP_ECH_ENABLED) */
    }
    if ((AdcBasePtr->ISR & ADC_ISR_JECH_MASK) != 0U)
    {
        AdcBasePtr->ISR = ADC_ISR_JECH_MASK;
        /* Check if the interrupt was spurious */
    #if (STD_ON == ADC_SAR_IP_JECH_ENABLED)
        if ((AdcBasePtr->IMR & ADC_IMR_MSKJECH_MASK)!= 0U)
        {
            Adc_Sar_CheckAndCallNotification(Instance, Adc_Sar_Ip_axAdcSarState[Instance].EndOfInjectedChainNotification);
        }
    #endif /* (STD_ON == ADC_SAR_IP_JECH_ENABLED) */
    }

    /* Clear EOC/JEOC interrupt */
    if ((AdcBasePtr->ISR & ADC_ISR_EOC_MASK) != 0U)
    {
        AdcBasePtr->ISR = ADC_ISR_EOC_MASK;
    }
    if ((AdcBasePtr->ISR & ADC_ISR_JEOC_MASK) != 0U)
    {
        AdcBasePtr->ISR = ADC_ISR_JEOC_MASK;
    }

    Adc_CheckAndCallAllChannelNotification(Instance);

    /* Clear all active watchdog interrupt flags */
    AdcBasePtr->WTISR = ADC_SAR_IP_WTISR_CLEAR_MASK;
}

/*FUNCTION**********************************************************************
 *
 * Function Name : Adc_Sar_SetNormalChain
 * Description   : This function configures the ADC Normal Chain.
 *
 *END**************************************************************************/
static inline void Adc_Sar_SetNormalChain(const uint32 Instance,
                                          const uint32 * const Mask)
{
    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];
    uint8 Index;

    for (Index = 0U; Index < Adc_Sar_Ip_au8AdcGroupCount[Instance]; Index++)
    {
        NCMR(AdcBasePtr, Index) = Mask[Index];
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : Adc_Sar_SetInjectedChain
 * Description   : This function configures the ADC Injected Chain.
 *
 *END**************************************************************************/
static inline void Adc_Sar_SetInjectedChain(const uint32 Instance,
                                            const uint32 * const Mask)
{
    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[Instance];
    uint8 Index;

    for (Index = 0U; Index < Adc_Sar_Ip_au8AdcGroupCount[Instance]; Index++)
    {
        JCMR(AdcBasePtr, Index) = Mask[Index];
    }
}

/*FUNCTION**********************************************************************
 *
 * Function Name : Adc_Sar_GetValueFromBool
 * Description   : The function is used to reduce complexity for parent function
 *
 *END**************************************************************************/
static inline uint32 Adc_Sar_GetValueFromBool(const boolean bVal,
                                              const uint32 TrueVal,
                                              const uint32 FalseVal)
{
    return ((TRUE == bVal) ? TrueVal : FalseVal);
}
/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_Init
* Description   : This function initializes the ADC_SAR module by configuring all
* available features.
*
* @implements      Adc_Sar_Ip_Init_Activity
* END**************************************************************************/
Adc_Sar_Ip_StatusType Adc_Sar_Ip_Init(const uint32 u32Instance,
                                      const Adc_Sar_Ip_ConfigType * const pConfig)
{
    Adc_Sar_Ip_StatusType Status = ADC_SAR_IP_STATUS_SUCCESS;
#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
    uint8 Index = 0u;
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(pConfig != NULL_PTR);
#if FEATURE_ADC_HAS_CTU
#if (FEATURE_ADC_HAS_CTU_TRIGGER_MODE == 0u)
    /* test that the feature is enabled only on platforms that have it */
    DevAssert(pConfig->CtuMode != ADC_SAR_IP_CTU_MODE_TRIGGER);
#endif /* FEATURE_ADC_HAS_CTU */
#endif /* (FEATURE_ADC_HAS_CTU_TRIGGER_MODE == 0u) */
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

#if (STD_ON == ADC_SAR_IP_ENABLE_USER_MODE_SUPPORT)
    /* Set the UAA bit in REG_PROT to make the instance accessible in user mode */
    OsIf_Trusted_Call1param(Adc_Sar_Ip_SetUserAccessAllowed, AdcBasePtr);
#endif

    /* Stop any conversions, if any */
    (void)Adc_Sar_Ip_AbortChain(u32Instance, TRUE, FALSE);

    /* Clear CTUEN to allow Powerdown operation to succeed. */
#if defined(ADC_MCR_CTUEN_MASK)
    AdcBasePtr->MCR &= ~ADC_MCR_CTUEN_MASK;
#endif /* ADC_MCR_CTUEN_MASK */

    Status = Adc_Sar_Ip_Powerdown(u32Instance);
    if (ADC_SAR_IP_STATUS_SUCCESS == Status)
    {
        /* Collects the mask and writes MCR mask */
        AdcBasePtr->MCR = Adc_Sar_CollectMcrMasks(u32Instance, pConfig, AdcBasePtr->MCR);

        Adc_Sar_Ip_axAdcSarState[u32Instance].DataAlign = pConfig->DataAlign;

        Status = Adc_Sar_Ip_Powerup(u32Instance);
        if (ADC_SAR_IP_STATUS_SUCCESS == Status)
        {
            if (ADC_SAR_IP_CTU_MODE_TRIGGER == pConfig->CtuMode)
            {
                /* Workaround to enable CTU_MODE because it cannot be written in power-down mode in SAF85 */
                AdcBasePtr->MCR |= ADC_MCR_CTU_MODE(1U);
            }
#if FEATURE_ADC_HAS_HIGH_SPEED_ENABLE
            /* Enables high speed conversion or calibration */
            Adc_Sar_EnableHighSpeed(AdcBasePtr, pConfig->HighSpeedConvEn);
#endif /* FEATURE_ADC_HAS_HIGH_SPEED_ENABLE */

#if FEATURE_ADC_HAS_CONVERSION_TIMING
            /* Set the sample times for each channnel group */
            Adc_Sar_Ip_SetSampleTimes(u32Instance, pConfig->SampleTimeArr);
#endif /* FEATURE_ADC_HAS_CONVERSION_TIMING */

            AdcBasePtr->PDEDR = ADC_PDEDR_PDED(pConfig->PowerDownDelay);
#if FEATURE_ADC_HAS_CLOCK_DIVIDER
            /* Configure clock divider */
            Adc_Sar_EnableClkDiv(u32Instance, pConfig->ClkDivEnable);
#endif /* FEATURE_ADC_HAS_CLOCK_DIVIDER */

#ifdef ADC_DSDR_DSD
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
            if (ADC_SAR_IP_INST_HAS_DSDR(u32Instance))
            {
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_FEATURE */
#if FEATURE_ADC_SAR_DECODE_DELAY
                AdcBasePtr->DSDR = ADC_DSDR_DSD(pConfig->DecodeDelay);
#endif /* FEATURE_ADC_SAR_DECODE_DELAY */
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
            }
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_FEATURE */
#endif /* ADC_DSDR_DSD */

            /* Configure chain conversions */
            Adc_Sar_SetNormalChain(u32Instance, pConfig->ChanMaskNormal);
            Adc_Sar_SetInjectedChain(u32Instance, pConfig->ChanMaskInjected);

#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
            /* Configure resolution */
            Adc_Sar_Ip_SetResolution(u32Instance, pConfig->AdcResolution);
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */

            /* Configure WDG */
#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
            if ((pConfig->WdgThresholds != NULL_PTR) && (pConfig->NumWdgThresholds > 0u))
            {
                for (Index = 0U; Index < pConfig->NumWdgThresholds; Index++)
                {
                    Adc_Sar_Ip_SetWdgThreshold(u32Instance, pConfig->WdgThresholds[Index].WdgIndex, &pConfig->WdgThresholds[Index]);
                }
            }
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */

            /* Configure DMA for channels */
            AdcBasePtr->DMAE &= ~(ADC_DMAE_DMAEN_MASK | ADC_DMAE_DCLR_MASK);
            if (pConfig->DmaEnable)
            {
                AdcBasePtr->DMAE |= (ADC_DMAE_DMAEN(1U) | ADC_DMAE_DCLR((uint32)pConfig->DmaClearSource));
            }

            /* Configure per channel notifications and presampling */
            if ((pConfig->ChannelConfigsPtr != NULL_PTR) && (pConfig->NumChannels > 0u))
            {
                Adc_Sar_ConfigChannels(u32Instance, pConfig->ChannelConfigsPtr, pConfig->NumChannels);
            }

#if FEATURE_ADC_HAS_PRESAMPLING
            /* Configure presampling sources */
            AdcBasePtr->PSCR &= ~(ADC_PSCR_PRECONV_MASK);
            AdcBasePtr->PSCR |= ADC_PSCR_PRECONV(pConfig->BypassSampling ? 1u : 0u);

            Adc_Sar_Ip_SetPresamplingSource(u32Instance, ADC_SAR_IP_CHAN_GROUP_0, pConfig->PresamplingSourceArr[0u]);
            Adc_Sar_Ip_SetPresamplingSource(u32Instance, ADC_SAR_IP_CHAN_GROUP_1, pConfig->PresamplingSourceArr[1u]);
#if ADC_SAR_IP_NUM_GROUP_CHAN > 2u
            Adc_Sar_Ip_SetPresamplingSource(u32Instance, ADC_SAR_IP_CHAN_GROUP_2, pConfig->PresamplingSourceArr[2u]);
#endif /* ADC_SAR_IP_NUM_GROUP_CHAN > 2u */
#endif /* FEATURE_ADC_HAS_PRESAMPLING */

            /* Add notification callbacks to the state structure */
#if (STD_ON == ADC_SAR_IP_ECH_ENABLED)
            Adc_Sar_Ip_axAdcSarState[u32Instance].EndOfNormalChainNotification = pConfig->EndOfNormalChainNotification;
#endif /* (STD_ON == ADC_SAR_IP_ECH_ENABLED) */
#if (STD_ON == ADC_SAR_IP_JECH_ENABLED)
            Adc_Sar_Ip_axAdcSarState[u32Instance].EndOfInjectedChainNotification = pConfig->EndOfInjectedChainNotification;
#endif /* (STD_ON == ADC_SAR_IP_JECH_ENABLED) */
#if FEATURE_ADC_HAS_CTU
#if (STD_ON == ADC_SAR_IP_EOCTU_ENABLED)
            Adc_Sar_Ip_axAdcSarState[u32Instance].EndOfCtuConversionNotification = pConfig->EndOfCtuConversionNotification;
#endif /* (STD_ON == ADC_SAR_IP_EOCTU_ENABLED) */
#endif /* FEATURE_ADC_HAS_CTU */
#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
            Adc_Sar_Ip_axAdcSarState[u32Instance].EndOfConvNotification = pConfig->EndOfConvNotification;
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */
#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
            Adc_Sar_Ip_axAdcSarState[u32Instance].WdgOutOfRangeNotification = pConfig->WdgOutOfRangeNotification;
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */
#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
            Adc_Sar_Ip_axAdcSarState[u32Instance].BypassResolution = pConfig->BypassResolution;
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */
            Adc_Sar_Ip_axAdcSarState[u32Instance].CalibrationClkSelect = pConfig->CalibrationClkSelect;

            /* Configure user gain and offset */
            AdcBasePtr->ADC_USER_OFFSET_GAIN_REG = ADC_USER_OFFSET(pConfig->UsrOffset) | ADC_USER_GAIN(pConfig->UsrGain);

            /* Mark that the driver was initialized */
            Adc_Sar_Ip_axAdcSarState[u32Instance].InitStatus = TRUE;
        }
    }

    return Status;
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_Deinit
* Description   : This function writes all the internal ADC registers with
*  their Reference Manual reset values.
*
* @implements      Adc_Sar_Ip_DeInit_Activity
* END**************************************************************************/
Adc_Sar_Ip_StatusType Adc_Sar_Ip_Deinit(const uint32 u32Instance)
{
    uint32 Index;
    Adc_Sar_Ip_ConfigType DefaultConfig;
    Adc_Sar_Ip_StatusType Status;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    DefaultConfig.ConvMode = ADC_SAR_IP_CONV_MODE_ONESHOT;
    DefaultConfig.ClkSelect = ADC_SAR_IP_CLK_FULL_BUS;
    DefaultConfig.CalibrationClkSelect = ADC_SAR_IP_CLK_HALF_BUS;
#if FEATURE_ADC_HAS_HIGH_SPEED_ENABLE
    DefaultConfig.HighSpeedConvEn = FALSE;
#endif /* FEATURE_ADC_HAS_HIGH_SPEED_ENABLE */
#if FEATURE_ADC_HAS_CTU
    DefaultConfig.CtuMode = ADC_SAR_IP_CTU_MODE_DISABLED;
#endif /* FEATURE_ADC_HAS_CTU */
#if FEATURE_ADC_HAS_INJ_EXT_TRIGGER
    DefaultConfig.InjectedEdge = ADC_SAR_IP_EXT_TRIG_EDGE_DISABLED;
#endif /* FEATURE_ADC_HAS_INJ_EXT_TRIGGER */
#if FEATURE_ADC_HAS_EXT_TRIGGER
    DefaultConfig.ExtTrigger = ADC_SAR_IP_EXT_TRIG_EDGE_DISABLED;
    DefaultConfig.NormalExtTrgEn = FALSE;
#if FEATURE_ADC_HAS_AUX_EXT_TRIGGER
    DefaultConfig.NormalAuxExtTrgEn = FALSE;
#endif /* FEATURE_ADC_HAS_AUX_EXT_TRIGGER */
#endif /* FEATURE_ADC_HAS_EXT_TRIGGER */
#if (FEATURE_ADC_HAS_CONVERSION_TIMING || FEATURE_ADC_HAS_PRESAMPLING)
    for (Index = 0u; Index < Adc_Sar_Ip_au8AdcGroupCount[u32Instance]; Index++)
    {
#if FEATURE_ADC_HAS_CONVERSION_TIMING
        DefaultConfig.SampleTimeArr[Index] = ADC_SAR_IP_DEF_SAMPLE_TIME;
#endif /* FEATURE_ADC_HAS_CONVERSION_TIMING */
#if FEATURE_ADC_HAS_PRESAMPLING
#if (ADC_PSCR_PREVAL0_WIDTH > 1u)
        DefaultConfig.PresamplingSourceArr[Index] = ADC_SAR_IP_PRESAMPLE_DVDD;
#else
        DefaultConfig.PresamplingSourceArr[Index] = ADC_SAR_IP_PRESAMPLE_VREFL;
#endif /* (ADC_PSCR_PREVAL0_WIDTH > 1u) */
#endif /* FEATURE_ADC_HAS_PRESAMPLING */
    }
#endif /* (FEATURE_ADC_HAS_CONVERSION_TIMING || FEATURE_ADC_HAS_PRESAMPLING) */
#if FEATURE_ADC_HAS_PRESAMPLING
    DefaultConfig.BypassSampling = FALSE;
#endif /* FEATURE_ADC_HAS_PRESAMPLING */
    DefaultConfig.AutoClockOff = FALSE;
    DefaultConfig.OverwriteEnable = FALSE;
    DefaultConfig.DataAlign = ADC_SAR_IP_DATA_ALIGNED_RIGHT;
#if FEATURE_ADC_SAR_DECODE_DELAY
    DefaultConfig.DecodeDelay = 0u;
#endif /* FEATURE_ADC_SAR_DECODE_DELAY */
    DefaultConfig.PowerDownDelay = 0u;
#if FEATURE_ADC_HAS_CLOCK_DIVIDER
    DefaultConfig.ClkDivEnable = FALSE;
#endif /* FEATURE_ADC_HAS_CLOCK_DIVIDER */
#if FEATURE_ADC_HAS_AVERAGING
    DefaultConfig.AvgEn = FALSE;
    DefaultConfig.AvgSel = ADC_SAR_IP_AVG_4_CONV;
#endif /* FEATURE_ADC_HAS_AVERAGING */
    DefaultConfig.UsrOffset = 0u;
    DefaultConfig.UsrGain = 0u;
    DefaultConfig.DmaEnable = FALSE;
    DefaultConfig.DmaClearSource = ADC_SAR_IP_DMA_REQ_CLEAR_ON_ACK;
    DefaultConfig.NumChannels = 0u;
    DefaultConfig.ChannelConfigsPtr = NULL_PTR;
#if (STD_ON == ADC_SAR_IP_ECH_ENABLED)
    DefaultConfig.EndOfNormalChainNotification = NULL_PTR;
#endif /* (STD_ON == ADC_SAR_IP_ECH_ENABLED) */
#if (STD_ON == ADC_SAR_IP_JECH_ENABLED)
    DefaultConfig.EndOfInjectedChainNotification = NULL_PTR;
#endif /* (STD_ON == ADC_SAR_IP_JECH_ENABLED) */
#if FEATURE_ADC_HAS_CTU
#if (STD_ON == ADC_SAR_IP_EOCTU_ENABLED)
    DefaultConfig.EndOfCtuConversionNotification = NULL_PTR;
#endif /* (STD_ON == ADC_SAR_IP_EOCTU_ENABLED) */
#endif /* FEATURE_ADC_HAS_CTU */

#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
    DefaultConfig.EndOfConvNotification = NULL_PTR;
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */
#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
    DefaultConfig.NumWdgThresholds = 0u;
    DefaultConfig.WdgThresholds = NULL_PTR;
    DefaultConfig.WdgOutOfRangeNotification = NULL_PTR;
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */

    Status = Adc_Sar_Ip_Init(u32Instance, &DefaultConfig);
    if (ADC_SAR_IP_STATUS_SUCCESS == Status)
    {
        /* Deinit channel configuration */
        for (Index = 0u; Index < Adc_Sar_Ip_au8AdcGroupCount[u32Instance]; Index++)
        {
            CIMR(AdcBasePtr, Index) = 0u;
            CEOCFR(AdcBasePtr, Index) = 0xFFFFFFFFu; /* w1c bits */
            DMAR(AdcBasePtr, Index) = 0u;
#if FEATURE_ADC_HAS_PRESAMPLING
            PSR(AdcBasePtr, Index) = 0u;
#endif /* FEATURE_ADC_HAS_PRESAMPLING */
        }

        /* Deinit watchdog interrupts */
        Adc_Sar_ResetWdog(u32Instance);
        AdcBasePtr->WTIMR = 0u;

        Adc_Sar_Ip_ClearStatusFlags(u32Instance, ADC_SAR_IP_NOTIF_FLAG_ALL);
#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
        /* Set default resolution */
        Adc_Sar_Ip_SetResolution(u32Instance, ADC_SAR_IP_RESOLUTION_12);
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */

        /* Mark that the driver was deinitialized */
        Adc_Sar_Ip_axAdcSarState[u32Instance].InitStatus = FALSE;

        Status = Adc_Sar_Ip_Powerdown(u32Instance);

#if (STD_ON == ADC_SAR_IP_ENABLE_USER_MODE_SUPPORT)
        /* Clear the UAA bit in REG_PROT to reset the elevation requirement */
        OsIf_Trusted_Call1param(Adc_Sar_Ip_ClrUserAccessAllowed, AdcBasePtr);
#endif
    }

    return Status;
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_ChainConfig
* Description   : This function configures the ADC Normal and Injected Chains
*  with the options provided in the structure.
*
* @implements      Adc_Sar_Ip_ChainConfig_Activity
* END**************************************************************************/
void Adc_Sar_Ip_ChainConfig(const uint32 u32Instance,
                            const Adc_Sar_Ip_ChansIdxMaskType * const pChansIdxMask,
                            const Adc_Sar_Ip_ConvChainType pChainType)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(pChansIdxMask != NULL_PTR);
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
    uint8 i;
    /* Enabling unavailable channels is forbidden */
    for (i = 0U; i < ADC_SAR_IP_NUM_GROUP_CHAN; i++)
    {
        DevAssert((pChansIdxMask->ChanMaskArr[i] & (~Adc_Sar_Ip_au32AdcChanBitmap[u32Instance][i])) == 0u);
    }
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    switch (pChainType)
    {
        case ADC_SAR_IP_CONV_CHAIN_NORMAL:
            Adc_Sar_SetNormalChain(u32Instance, pChansIdxMask->ChanMaskArr);
            break;
        case ADC_SAR_IP_CONV_CHAIN_INJECTED:
            Adc_Sar_SetInjectedChain(u32Instance, pChansIdxMask->ChanMaskArr);
            break;
        default:
            ; /* no-op */
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
            DevAssert(FALSE);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */
            break;
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_EnableChannel
* Description   : This function enables a channel in a conversion chain (
*  ADC_SAR_IP_CONV_CHAIN_NORMAL or ADC_SAR_IP_CONV_CHAIN_INJECTED).
*
* @implements      Adc_Sar_Ip_EnableChannel_Activity
* END**************************************************************************/
void Adc_Sar_Ip_EnableChannel(const uint32 u32Instance,
                              const Adc_Sar_Ip_ConvChainType pChainType,
                              const uint32 u32ChnIdx)
{
    uint32 VectAdr;
    uint32 VectBit;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(u32ChnIdx < ADC_SAR_IP_CDR_COUNT);
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
    DevAssert(ADC_SAR_IP_INST_HAS_CHANn(u32Instance, u32ChnIdx));
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    VectAdr = ADC_SAR_IP_CHAN_2_VECT(u32ChnIdx);
    VectBit = ADC_SAR_IP_CHAN_2_BIT(u32ChnIdx);

    switch (pChainType)
    {
        case ADC_SAR_IP_CONV_CHAIN_NORMAL:
            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_23();
            NCMR(AdcBasePtr, VectAdr) |= (1UL << VectBit);
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_23();
            break;
        case ADC_SAR_IP_CONV_CHAIN_INJECTED:
            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_26();
            JCMR(AdcBasePtr, VectAdr) |= (1UL << VectBit);
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_26();
            break;
        default:
            ; /* no-op */
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
            DevAssert(FALSE);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */
            break;
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_DisableChannel
* Description   : This function disables a channel from a conversion chain (ADC_SAR_IP_CONV_CHAIN_NORMAL
*  or ADC_SAR_IP_CONV_CHAIN_INJECTED).
*
* @implements      Adc_Sar_Ip_DisableChannel_Activity
* END**************************************************************************/
void Adc_Sar_Ip_DisableChannel(const uint32 u32Instance,
                               const Adc_Sar_Ip_ConvChainType pChainType,
                               const uint32 u32ChnIdx)
{
    uint32 VectAdr;
    uint32 VectBit;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(u32ChnIdx < ADC_SAR_IP_CDR_COUNT);
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
    DevAssert(ADC_SAR_IP_INST_HAS_CHANn(u32Instance, u32ChnIdx));
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    VectAdr = ADC_SAR_IP_CHAN_2_VECT(u32ChnIdx);
    VectBit = ADC_SAR_IP_CHAN_2_BIT(u32ChnIdx);

    switch (pChainType)
    {
        case ADC_SAR_IP_CONV_CHAIN_NORMAL:
            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_24();
            NCMR(AdcBasePtr, VectAdr) &= ~(1UL << VectBit);
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_24();
            break;
        case ADC_SAR_IP_CONV_CHAIN_INJECTED:
            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_27();
            JCMR(AdcBasePtr, VectAdr) &= ~(1UL << VectBit);
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_27();
            break;
        default:
            ; /* no-op */
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
            DevAssert(FALSE);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */
            break;
    }
}

#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_SetResolution
* Description   : This function sets the conversion resolution (number of bits per conversion data)
*
* @implements      Adc_Sar_Ip_SetResolution_Activity
* END**************************************************************************/
void Adc_Sar_Ip_SetResolution(const uint32 u32Instance,
                              const Adc_Sar_Ip_Resolution eResolution)
{
    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];
    uint32 Calbistreg;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    /* The selected ADC should be in IDLE state */
    DevAssert((AdcBasePtr->MSR & ADC_MSR_ADCSTATUS_MASK) == ADC_MSR_ADCSTATUS(ADC_SAR_IP_MSR_ADCSTATUS_IDLE));
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_37();
    Calbistreg = AdcBasePtr->CALBISTREG;
    /* Clear the bits and set resolution value */
    Calbistreg &= ~(ADC_CALBISTREG_RESN_MASK);
    Calbistreg |= ADC_CALBISTREG_RESN(eResolution);
    AdcBasePtr->CALBISTREG = Calbistreg;
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_37();
}
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_StartConversion
* Description   : This function starts a conversion chain (ADC_SAR_IP_CONV_CHAIN_NORMAL
*  or ADC_SAR_IP_CONV_CHAIN_INJECTED).
*
* @implements      Adc_Sar_Ip_StartConversion_Activity
* END**************************************************************************/
void Adc_Sar_Ip_StartConversion(const uint32 u32Instance,
                                const Adc_Sar_Ip_ConvChainType pChainType)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_10();
    switch (pChainType)
    {
        case ADC_SAR_IP_CONV_CHAIN_NORMAL:
            AdcBasePtr->MCR |= ADC_MCR_NSTART(1U);
            break;
        case ADC_SAR_IP_CONV_CHAIN_INJECTED:
            AdcBasePtr->MCR |= ADC_MCR_JSTART(1U);
            break;
        default:
            ; /* no-op */
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
            DevAssert(FALSE);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */
            break;
    }
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_10();
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_GetStatusFlags
* Description   : This function returns the status flags of the ADC.
*
* @implements      Adc_Sar_Ip_GetStatusFlags_Activity
* END**************************************************************************/
uint32 Adc_Sar_Ip_GetStatusFlags(const uint32 u32Instance)
{
    uint32 Flags = 0U;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    /* Returns the flag status from MSR register */
    Flags |= Adc_Sar_GetMsrFlags(u32Instance);
    /* Returns the flag status from ISR register */
    Flags |= Adc_Sar_GetIsrFlags(u32Instance);

    return Flags;
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_ClearStatusFlags
* Description   : This function clears the status flags that are set to '1' in
*  the mask.
*
* @implements      Adc_Sar_Ip_ClearStatusFlags_Activity
* END**************************************************************************/
void Adc_Sar_Ip_ClearStatusFlags(const uint32 u32Instance,
                                 const uint32 u32Mask)
{
    uint32 IsrFlags = 0U;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert((u32Mask & (~ADC_SAR_IP_NOTIF_FLAG_ALL)) == 0UL);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    IsrFlags |= ((u32Mask & ADC_SAR_IP_NOTIF_FLAG_NORMAL_EOC) != 0U) ? ADC_ISR_EOC(1U) : 0U;
    IsrFlags |= ((u32Mask & ADC_SAR_IP_NOTIF_FLAG_NORMAL_ENDCHAIN) != 0U) ? ADC_ISR_ECH(1U) : 0U;
    IsrFlags |= ((u32Mask & ADC_SAR_IP_NOTIF_FLAG_INJECTED_EOC) != 0U) ? ADC_ISR_JEOC(1U) : 0U;
    IsrFlags |= ((u32Mask & ADC_SAR_IP_NOTIF_FLAG_INJECTED_ENDCHAIN) != 0U) ? ADC_ISR_JECH(1U) : 0U;
#if FEATURE_ADC_HAS_CTU
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    if (ADC_SAR_IP_INST_HAS_CTU(u32Instance))
    {
#endif /*FEATURE_ADC_BAD_ACCESS_PROT_FEATURE*/
        IsrFlags |= ((u32Mask & ADC_SAR_IP_NOTIF_FLAG_CTU_EOC) != 0U) ? ADC_ISR_EOCTU(1U) : 0U;
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    }
#endif /*FEATURE_ADC_BAD_ACCESS_PROT_FEATURE*/
#endif /* FEATURE_ADC_HAS_CTU */

    /* Write-1-to-clear bits in ISR register */
    AdcBasePtr->ISR = IsrFlags;
#ifdef FEATURE_CPU_HAS_WRITE_BUFFER
    (void) AdcBasePtr->ISR;
#endif /* FEATURE_CPU_HAS_WRITE_BUFFER */
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_SelfTest
* Description   : This function executes a self test on the ADC instance.
*
* @implements      Adc_Sar_Ip_SelfTest_Activity
* END**************************************************************************/
Adc_Sar_Ip_StatusType Adc_Sar_Ip_SelfTest(const uint32 u32Instance)
{
    Adc_Sar_Ip_StatusType Status = ADC_SAR_IP_STATUS_SUCCESS;
#if FEATURE_ADC_HAS_SELFTEST_STCR1
    uint32 Reg;
#endif /* FEATURE_ADC_HAS_SELFTEST_STCR1 */
    uint32 MsrStatus;
    uint32 TimeoutTicks = OsIf_MicrosToTicks(ADC_SAR_IP_TIMEOUT_VAL, ADC_SAR_IP_TIMEOUT_TYPE);
    uint32 CurrentTicks = OsIf_GetCounter(ADC_SAR_IP_TIMEOUT_TYPE);
    uint32 ElapsedTicks = 0U;
#if (FEATURE_ADC_SELFTEST_FULL_CLK && ADC_SAR_IP_HAS_ADCLKSEL)
    uint32 AdcClkSel = 0u;
#endif /* (FEATURE_ADC_SELFTEST_FULL_CLK && ADC_SAR_IP_HAS_ADCLKSEL) */

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

#if FEATURE_ADC_SELFTEST_FULL_CLK
    /* Enter into power-down Mode */
    Status = Adc_Sar_Ip_Powerdown(u32Instance);
    if (ADC_SAR_IP_STATUS_SUCCESS == Status)
    {
#if ADC_SAR_IP_HAS_ADCLKSEL
        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_11();
        /* Save the current state of ADCLKSEL in order to restore it */
        AdcClkSel = (AdcBasePtr->MCR & ADC_MCR_ADCLKSEL_MASK);
        /* ADC self-test should be run with MCR[ADCLKSE] bit set to 1 */
        AdcBasePtr->MCR &= ~(ADC_MCR_ADCLKSEL_MASK);
        AdcBasePtr->MCR |= ADC_MCR_ADCLKSEL(1U);
        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_11();
#endif /* ADC_SAR_IP_HAS_ADCLKSEL */

        /* Exit into power-down Mode */
        Status = Adc_Sar_Ip_Powerup(u32Instance);
        if (ADC_SAR_IP_STATUS_SUCCESS == Status)
        {
#endif /* FEATURE_ADC_SELFTEST_FULL_CLK */
            /* 1. Program NCMR0 to select channels to be converted for normal conversion.*/
            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_25();
            NCMR(AdcBasePtr, 0u) |= ADC_NCMR_CH0(0x1UL);
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_25();

            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_11();
            /* Self-test will only run with CPU mode so clear CTU enable */
#if defined(ADC_MCR_CTUEN_MASK)
            AdcBasePtr->MCR &= ~ADC_MCR_CTUEN_MASK;
#endif /* ADC_MCR_CTUEN_MASK */

            /* 2. Program MCR[MODE] = 1 to select Scan mode. ADC_ONE_SHOT_MODE_U32 ADCDIG_CONTINUOUS_MODE_U32*/
            AdcBasePtr->MCR |= ADC_MCR_MODE(0x1UL);
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_11();
#if FEATURE_ADC_HAS_SELFTEST_STCR1
            /* 3. Program sampling duration values in STCR1[INPSAMPx].*/
            Reg = 0u;
            Reg |= ADC_STCR1_INPSAMP_S(0x10UL) | ADC_STCR1_INPSAMP_C(0x10UL);
            AdcBasePtr->STCR1 = Reg;
#endif /* FEATURE_ADC_HAS_SELFTEST_STCR1 */

            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_35();
#if FEATURE_ADC_HAS_SELFTEST_STCR3
            /* 4. Select the self-testing algorithm in STCR3[ALG]. */
            AdcBasePtr->STCR3 |= ADC_STCR3_ALG(0x3UL);
#endif /* FEATURE_ADC_HAS_SELFTEST_STCR3 */

            /* 5. Enable the self-testing channel by setting STCR2[EN]. */
            AdcBasePtr->STCR2 |= ADC_STCR2_EN(0x1UL);
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_35();

            /* 6. Start the normal conversion by setting MCR[NSTART]. */
            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_11();
            AdcBasePtr->MCR |= ADC_MCR_NSTART(0x1UL);
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_11();

            /*  Check that all algorithms have executed at least once. */
            Status = Adc_Sar_CheckSelfTestProgress(u32Instance);

            /* Stop the conversion */
            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_11();
            AdcBasePtr->MCR &= ~(ADC_MCR_NSTART(0x1UL));
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_11();

            if (ADC_SAR_IP_STATUS_SUCCESS == Status)
            {
                /* Read ADC status */
                MsrStatus = (AdcBasePtr->MSR & ADC_MSR_ADCSTATUS_MASK) >> ADC_MSR_ADCSTATUS_SHIFT;
                /* Wait for ADC changes to the expected Idle mode, The bitfield MSR[ADCSTATUS] should be checked to Idle state when MCR[NSTART] is written to 0 */
                ElapsedTicks = 0U;
                while ((MsrStatus != ADC_MSR_ADCSTATUS(ADC_SAR_IP_MSR_ADCSTATUS_IDLE)) && (ElapsedTicks < TimeoutTicks))
                {
                    MsrStatus = (AdcBasePtr->MSR & ADC_MSR_ADCSTATUS_MASK) >> ADC_MSR_ADCSTATUS_SHIFT;
                    ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, ADC_SAR_IP_TIMEOUT_TYPE);
                }
            }

            /* Disable Self-test */
            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_35();
            AdcBasePtr->STCR2 &= ~(ADC_STCR2_EN(0x1UL));
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_35();

#if FEATURE_ADC_SELFTEST_FULL_CLK
            /* Enter into power-down Mode */
            Status = Adc_Sar_Ip_Powerdown(u32Instance);
        }
#if ADC_SAR_IP_HAS_ADCLKSEL
        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_11();
        /* Restore the state of ADCLKSEL */
        AdcBasePtr->MCR &= ~(ADC_MCR_ADCLKSEL_MASK);
        AdcBasePtr->MCR |= AdcClkSel;
        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_11();
#endif /* ADC_SAR_IP_HAS_ADCLKSEL */

        if (ADC_SAR_IP_STATUS_SUCCESS == Status)
        {
            /* Exit into power-down Mode */
            Status = Adc_Sar_Ip_Powerup(u32Instance);
            if (ADC_SAR_IP_STATUS_SUCCESS == Status)
#endif /* FEATURE_ADC_SELFTEST_FULL_CLK */
            {
                if (ElapsedTicks >= TimeoutTicks)
                {
                    Status = ADC_SAR_IP_STATUS_TIMEOUT;
                }
            }
#if FEATURE_ADC_SELFTEST_FULL_CLK
        }
    }
#endif /* FEATURE_ADC_SELFTEST_FULL_CLK */

    return Status;
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_GetConvDataToArray
* Description   : This function reads the conversion result values for a conversion chain
*  (ADC_SAR_IP_CONV_CHAIN_NORMAL, ADC_SAR_IP_CONV_CHAIN_INJECTED or ADC_SAR_IP_CONV_CHAIN_CTU).
*
* @implements      Adc_Sar_Ip_GetConvDataToArray_Activity
* END**************************************************************************/
uint32 Adc_Sar_Ip_GetConvDataToArray(const uint32 u32Instance,
                                     const Adc_Sar_Ip_ConvChainType pChainType,
                                     const uint32 u32Length,
                                     uint16 * const pResults)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */
    return Adc_Sar_GetConvResults(u32Instance, pChainType, pResults, NULL_PTR, u32Length);
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_GetConvResultsToArray
* Description   : This function gets the conversion results for the selected
*  Conversion Chain. It follows the same algorithm as Adc_Sar_Ip_GetConvDataToArray,
*  but will copy some extra information to the output.
*
* @implements      Adc_Sar_Ip_GetConvResultsToArray_Activity
* END**************************************************************************/
uint32 Adc_Sar_Ip_GetConvResultsToArray(const uint32 u32Instance,
                                        const Adc_Sar_Ip_ConvChainType pChainType,
                                        const uint32 u32Length,
                                        Adc_Sar_Ip_ChanResultType* const pResults)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */
    return Adc_Sar_GetConvResults(u32Instance, pChainType, NULL_PTR, pResults, u32Length);
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_GetConvData
* Description   : This function returns the result of the conversion for
* a single channel
*
* @implements      Adc_Sar_Ip_GetConvData_Activity
* END**************************************************************************/
uint16 Adc_Sar_Ip_GetConvData(const uint32 u32Instance,
                              const uint32 u32ChnIdx)
{
    uint16 Result = 0U;
    uint32 VectAdr;
    uint32 VectBit;
    uint32 Cdr;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(u32ChnIdx < ADC_SAR_IP_CDR_COUNT);
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
    DevAssert(ADC_SAR_IP_INST_HAS_CHANn(u32Instance, u32ChnIdx));
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    VectAdr = ADC_SAR_IP_CHAN_2_VECT(u32ChnIdx);
    VectBit = ADC_SAR_IP_CHAN_2_BIT(u32ChnIdx);
    Cdr = CDR(AdcBasePtr, u32ChnIdx);
    /* check if the conversion data is valid */
    if (((Cdr & ADC_CDR_VALID_MASK) != 0U))
    {
        /* the data is correct, store the result in and clear the flag */
        Result = Adc_Sar_GetMaskedResult(u32Instance, Cdr);
        CEOCFR(AdcBasePtr, VectAdr) = ((uint32)1UL << VectBit); /* w1c bit */
    }

    return Result;
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_GetConvResult
* Description   : This function returns the result and the status of
* the conversion for a single channel
*
* @implements      Adc_Sar_Ip_GetConvResult_Activity
* END**************************************************************************/
void Adc_Sar_Ip_GetConvResult(const uint32 u32Instance,
                              const uint32 u32ChnIdx,
                              const Adc_Sar_Ip_ConvChainType pChainType,
                              Adc_Sar_Ip_ChanResultType * const pResult)
{
    uint32 VectAdr;
    uint32 VectBit;
    uint32 Cdr;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(u32ChnIdx < ADC_SAR_IP_CDR_COUNT);
    DevAssert(pResult != NULL_PTR);
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
    DevAssert(ADC_SAR_IP_INST_HAS_CHANn(u32Instance, u32ChnIdx));
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    VectAdr = ADC_SAR_IP_CHAN_2_VECT(u32ChnIdx);
    VectBit = ADC_SAR_IP_CHAN_2_BIT(u32ChnIdx);

    pResult->ConvData = 0u;
    pResult->AdcChnIdx = (uint8)u32ChnIdx;
    pResult->ValidFlag = FALSE;
    pResult->OverWrittenFlag = FALSE;

    Cdr = CDR(AdcBasePtr, u32ChnIdx);
    /* check if the result type matches the one request by pChainType */
    if (ADC_CDR_RESULT((uint32)pChainType) == (Cdr & ADC_CDR_RESULT_MASK))
    {
        pResult->ValidFlag = ((Cdr & ADC_CDR_VALID_MASK) != 0U) ? TRUE : FALSE;
        pResult->OverWrittenFlag = (1U == ((Cdr & ADC_CDR_OVERW_MASK) >> ADC_CDR_OVERW_SHIFT));
        pResult->ConvData = Adc_Sar_GetMaskedResult(u32Instance, Cdr);
    }
    CEOCFR(AdcBasePtr, VectAdr) = ((uint32)1UL << VectBit); /* w1c bit */
}

#if FEATURE_ADC_HAS_CALIBRATION
/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_DoCalibration
* Description   : This functions executes a calibration sequence. It is recommended
*  to run this sequence before using the ADC converter. The maximum clock frequency
*  for the calibration is 40 MHz, this function sets the ADCLKSEL bit resulting in
*  a maximum input clock frequency of 80 MHz.
*
* @implements      Adc_Sar_Ip_DoCalibration_Activity
* END**************************************************************************/
Adc_Sar_Ip_StatusType Adc_Sar_Ip_DoCalibration(const uint32 u32Instance)
{
    Adc_Sar_Ip_StatusType Status = ADC_SAR_IP_STATUS_SUCCESS;
    Adc_Sar_Ip_StatusType CalStatus = ADC_SAR_IP_STATUS_SUCCESS;
    uint32 TimeoutTicks = OsIf_MicrosToTicks(ADC_SAR_IP_TIMEOUT_VAL, ADC_SAR_IP_TIMEOUT_TYPE);
    uint32 CurrentTicks = OsIf_GetCounter(ADC_SAR_IP_TIMEOUT_TYPE);
    uint32 ElapsedTicks = 0u;
    uint32 AdcClkSel = 0u;
    uint32 Mcr;
#if !ADC_SAR_IP_CALIBRATION_USES_MCR
    uint32 Calbistreg;
#endif

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    /* Set CLKSEL to CalibrationClkSelect value */
    Status = Adc_Sar_Ip_Powerdown(u32Instance);
    if (ADC_SAR_IP_STATUS_SUCCESS == Status)
    {
#if ADC_SAR_IP_HAS_ADCLKSEL
        /* Save the current state of ADCLKSEL in order to restore it */
        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_12();
        Mcr = AdcBasePtr->MCR;
        AdcClkSel = (Mcr & ADC_MCR_ADCLKSEL_MASK);
        Mcr &= ~(ADC_MCR_ADCLKSEL_MASK);
        Mcr |= Adc_Sar_GetADCLKSELValue(Adc_Sar_Ip_axAdcSarState[u32Instance].CalibrationClkSelect);
        AdcBasePtr->MCR = Mcr;
        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_12();
#endif /* ADC_SAR_IP_HAS_ADCLKSEL */

        Status = Adc_Sar_Ip_Powerup(u32Instance);
        if (ADC_SAR_IP_STATUS_SUCCESS == Status)
        {
#if ADC_SAR_IP_CALIBRATION_USES_MCR
            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_12();
            /* Clear the bits and set to calibration values */
            Mcr = AdcBasePtr->MCR;
            Mcr &= ~(ADC_MCR_TSAMP_MASK | ADC_MCR_NRSMPL_MASK | ADC_MCR_AVGEN_MASK);
            Mcr |= ADC_MCR_NRSMPL(0x03u) | ADC_MCR_AVGEN(1U);
            AdcBasePtr->MCR = Mcr;

            /* Start calibration */
            AdcBasePtr->MCR |= ADC_MCR_CALSTART(1U);
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_12();

            /* Wait for calibration to finish */
            while (((AdcBasePtr->MSR & ADC_MSR_CALBUSY_MASK) != 0U) && (ElapsedTicks < TimeoutTicks))
            {
                ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, ADC_SAR_IP_TIMEOUT_TYPE);
            }

            if (ElapsedTicks >= TimeoutTicks)
            {
                CalStatus = ADC_SAR_IP_STATUS_TIMEOUT;
            }
            else if ((AdcBasePtr->MSR & ADC_MSR_CALFAIL_MASK) != 0U)
            {
                /* If the calibration failed, return error */
                CalStatus = ADC_SAR_IP_STATUS_ERROR;
            }
            else
            {
                ; /* Empty else branch to avoid MISRA */
            }
#else
            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_36();
            Calbistreg = AdcBasePtr->CALBISTREG;
            /* clear the bits and set to calibration values */
            Calbistreg &= ~(ADC_CALBISTREG_TSAMP_MASK | ADC_CALBISTREG_NR_SMPL_MASK | ADC_CALBISTREG_AVG_EN_MASK | ADC_CALBISTREG_TEST_EN_MASK);
            Calbistreg |= ADC_CALBISTREG_NR_SMPL(0x03u) | ADC_CALBISTREG_AVG_EN(1U);
            AdcBasePtr->CALBISTREG = Calbistreg;
            /* clear the calibration failed before a new calibration */
            AdcBasePtr->CALBISTREG |= ADC_CALBISTREG_TEST_FAIL_MASK;
            /* start calibration */
            AdcBasePtr->CALBISTREG |= ADC_CALBISTREG_TEST_EN(1U);
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_36();

            /* Wait for calibration to finish */
            ElapsedTicks = 0U;
            while (((AdcBasePtr->CALBISTREG & ADC_CALBISTREG_C_T_BUSY_MASK) != 0U) && (ElapsedTicks < TimeoutTicks))
            {
                ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, ADC_SAR_IP_TIMEOUT_TYPE);
            }
            if (ElapsedTicks >= TimeoutTicks)
            {
                CalStatus = ADC_SAR_IP_STATUS_TIMEOUT;
            }
            else if ((AdcBasePtr->CALBISTREG & ADC_CALBISTREG_TEST_FAIL_MASK) != 0U)
            {
                /* If the calibration failed, return error */
                CalStatus = ADC_SAR_IP_STATUS_ERROR;
            }
            else
            {
                ; /* Empty else branch to avoid MISRA */
            }
#endif /* ADC_SAR_IP_CALIBRATION_USES_MCR */

            /* Restore the state of ADCLKSEL */
            Status = Adc_Sar_Ip_Powerdown(u32Instance);
        }

#if ADC_SAR_IP_HAS_ADCLKSEL
        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_12();
        Mcr = AdcBasePtr->MCR;
        Mcr &= ~(ADC_MCR_ADCLKSEL_MASK);
        Mcr |= AdcClkSel;
        AdcBasePtr->MCR = Mcr;
        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_12();
#endif /* ADC_SAR_IP_HAS_ADCLKSEL */

        if (ADC_SAR_IP_STATUS_SUCCESS == Status)
        {
            Status = Adc_Sar_Ip_Powerup(u32Instance);

            if (ADC_SAR_IP_STATUS_SUCCESS != CalStatus)
            {
                Status = CalStatus;
            }
        }
    }

    return Status;
}
#endif /* FEATURE_ADC_HAS_CALIBRATION */

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_Powerup
* Description   : This function enables the ADC module (by clearing the Powerdown
*  bit).
*
* @implements      Adc_Sar_Ip_Powerup_Activity
* END**************************************************************************/
Adc_Sar_Ip_StatusType Adc_Sar_Ip_Powerup(const uint32 u32Instance)
{
    Adc_Sar_Ip_StatusType ReturnStatus = ADC_SAR_IP_STATUS_SUCCESS;

#if FEATURE_ADC_HAS_BANDGAP_STATUS
    uint32 BandGapStatus = 0u;
#else
    uint32 ExpectedStatus = ADC_MSR_ADCSTATUS(ADC_SAR_IP_MSR_ADCSTATUS_IDLE);  /* expected status is IDLE */
    uint32 MsrStatus;
#endif /* FEATURE_ADC_HAS_BANDGAP_STATUS */
    uint32 TimeoutTicks = OsIf_MicrosToTicks(ADC_SAR_IP_TIMEOUT_VAL, ADC_SAR_IP_TIMEOUT_TYPE);
    uint32 CurrentTicks = OsIf_GetCounter(ADC_SAR_IP_TIMEOUT_TYPE);
    uint32 ElapsedTicks = 0u;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

#if FEATURE_ADC_HAS_BANDGAP_STATUS
    BandGapStatus = AdcBasePtr->GPADC_READOUT & ADC_GPADC_READOUT_BG_OK_MASK;
#else
    MsrStatus = AdcBasePtr->MSR & ADC_MSR_ADCSTATUS_MASK;
#endif /* FEATURE_ADC_HAS_BANDGAP_STATUS */

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_13();
    Adc_Sar_Powerup(AdcBasePtr);
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_13();

    /* Wait for the device to power up */
#if FEATURE_ADC_HAS_BANDGAP_STATUS
    while ((0U == BandGapStatus) && (ElapsedTicks < TimeoutTicks))
    {
        BandGapStatus = AdcBasePtr->GPADC_READOUT & ADC_GPADC_READOUT_BG_OK_MASK;
        ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, ADC_SAR_IP_TIMEOUT_TYPE);
    }
#else
    while ((MsrStatus != ExpectedStatus) && (ElapsedTicks < TimeoutTicks))
    {
        MsrStatus = AdcBasePtr->MSR & ADC_MSR_ADCSTATUS_MASK;
        ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, ADC_SAR_IP_TIMEOUT_TYPE);
    }
#endif /* FEATURE_ADC_HAS_BANDGAP_STATUS */

    if (ElapsedTicks >= TimeoutTicks)
    {
        ReturnStatus = ADC_SAR_IP_STATUS_TIMEOUT;
    }
#if FEATURE_ADC_HAS_BANDGAP_STATUS
    else
    {
        /* Wait for 20us after BandGap is OK */
        TimeoutTicks = OsIf_MicrosToTicks(ADC_SAR_IP_BANDGAP_STABLE_INTERVAL, ADC_SAR_IP_TIMEOUT_TYPE);
        CurrentTicks = OsIf_GetCounter(ADC_SAR_IP_TIMEOUT_TYPE);
        ElapsedTicks = 0u;
        while (ElapsedTicks < TimeoutTicks)
        {
            ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, ADC_SAR_IP_TIMEOUT_TYPE);
        }
    }
#endif /* FEATURE_ADC_HAS_BANDGAP_STATUS */

    return ReturnStatus;
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_Powerdown
* Description   : This function disables the ADC module (by setting the Powerdown
*  bit).
*
* @implements      Adc_Sar_Ip_Powerdown_Activity
* END**************************************************************************/
Adc_Sar_Ip_StatusType Adc_Sar_Ip_Powerdown(const uint32 u32Instance)
{
    Adc_Sar_Ip_StatusType ReturnStatus = ADC_SAR_IP_STATUS_SUCCESS;

#if FEATURE_ADC_HAS_BANDGAP_STATUS
    uint32 BandGapStatus = 0u;
#else
    uint32 ExpectedStatus = ADC_MSR_ADCSTATUS(ADC_SAR_IP_MSR_ADCSTATUS_POWER_DOWN);  /* expected status is powered down */
    uint32 MsrStatus;
#endif /* FEATURE_ADC_HAS_BANDGAP_STATUS */
    uint32 TimeoutTicks = OsIf_MicrosToTicks(ADC_SAR_IP_TIMEOUT_VAL, ADC_SAR_IP_TIMEOUT_TYPE);
    uint32 CurrentTicks = OsIf_GetCounter(ADC_SAR_IP_TIMEOUT_TYPE);
    uint32 ElapsedTicks = 0u;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

#if FEATURE_ADC_HAS_BANDGAP_STATUS
    BandGapStatus = AdcBasePtr->GPADC_READOUT & ADC_GPADC_READOUT_BG_OK_MASK;
#else
    MsrStatus = AdcBasePtr->MSR & ADC_MSR_ADCSTATUS_MASK;
#endif /* FEATURE_ADC_HAS_BANDGAP_STATUS */

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_14();
    Adc_Sar_Powerdown(AdcBasePtr);
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_14();

    /* Wait for the device to power down */
#if FEATURE_ADC_HAS_BANDGAP_STATUS
    while ((1U == BandGapStatus) && (ElapsedTicks < TimeoutTicks))
    {
        BandGapStatus = AdcBasePtr->GPADC_READOUT & ADC_GPADC_READOUT_BG_OK_MASK;
        ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, ADC_SAR_IP_TIMEOUT_TYPE);
    }
#else
    while ((MsrStatus != ExpectedStatus) && (ElapsedTicks < TimeoutTicks))
    {
        MsrStatus = AdcBasePtr->MSR & ADC_MSR_ADCSTATUS_MASK;
        ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, ADC_SAR_IP_TIMEOUT_TYPE);
    }
#endif /* FEATURE_ADC_HAS_BANDGAP_STATUS */

    if (ElapsedTicks >= TimeoutTicks)
    {
        ReturnStatus = ADC_SAR_IP_STATUS_TIMEOUT;
    }

    return ReturnStatus;
}

#if ( \
     (STD_ON == ADC_SAR_IP_ECH_ENABLED) || (STD_ON == ADC_SAR_IP_JECH_ENABLED) || \
     (STD_ON == ADC_SAR_IP_EOC_ENABLED) || \
     ((FEATURE_ADC_HAS_CTU == 1U) && (STD_ON == ADC_SAR_IP_EOCTU_ENABLED)) \
    )
/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_EnableNotifications
* Description   : This function enables the ADC notifications set to '1' in the
*  mask parameter.
*
* @implements      Adc_Sar_Ip_EnableNotifications_Activity
* END**************************************************************************/
void Adc_Sar_Ip_EnableNotifications(const uint32 u32Instance,
                                    const uint32 u32NotificationMask)
{
    uint32 ImrFlags = 0U;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert((u32NotificationMask & (~ADC_SAR_IP_NOTIF_FLAG_ALL)) == 0UL);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];
#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
    ImrFlags |= ((u32NotificationMask & ADC_SAR_IP_NOTIF_FLAG_NORMAL_EOC) != 0U) ? ADC_IMR_MSKEOC(1U) : 0U;
    ImrFlags |= ((u32NotificationMask & ADC_SAR_IP_NOTIF_FLAG_INJECTED_EOC) != 0U) ? ADC_IMR_MSKJEOC(1U) : 0U;
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */
#if (STD_ON == ADC_SAR_IP_ECH_ENABLED)
    ImrFlags |= ((u32NotificationMask & ADC_SAR_IP_NOTIF_FLAG_NORMAL_ENDCHAIN) != 0U) ? ADC_IMR_MSKECH(1U) : 0U;
#endif /* (STD_ON == ADC_SAR_IP_ECH_ENABLED) */
#if (STD_ON == ADC_SAR_IP_JECH_ENABLED)
    ImrFlags |= ((u32NotificationMask & ADC_SAR_IP_NOTIF_FLAG_INJECTED_ENDCHAIN) != 0U) ? ADC_IMR_MSKJECH(1U) : 0U;
#endif /* (STD_ON == ADC_SAR_IP_JECH_ENABLED) */
#if FEATURE_ADC_HAS_CTU
#if (STD_ON == ADC_SAR_IP_EOCTU_ENABLED)
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    if (ADC_SAR_IP_INST_HAS_CTU(u32Instance))
    {
#endif /*FEATURE_ADC_BAD_ACCESS_PROT_FEATURE*/
        ImrFlags |= ((u32NotificationMask & ADC_SAR_IP_NOTIF_FLAG_CTU_EOC) != 0U) ? ADC_IMR_MSKEOCTU(1U) : 0U;
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    }
#endif /*FEATURE_ADC_BAD_ACCESS_PROT_FEATURE*/
#endif /* (STD_ON == ADC_SAR_IP_EOCTU_ENABLED) */
#endif /* FEATURE_ADC_HAS_CTU */
    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_33();
    AdcBasePtr->IMR |= ImrFlags;
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_33();
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_DisableNotifications
* Description   : This function disables the ADC notifications set to '1' in the
*  mask parameter.
*
* @implements      Adc_Sar_Ip_DisableNotifications_Activity
* END**************************************************************************/
void Adc_Sar_Ip_DisableNotifications(const uint32 u32Instance,
                                     const uint32 u32NotificationMask)
{
    uint32 ImrFlags = 0U;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert((u32NotificationMask & (~ADC_SAR_IP_NOTIF_FLAG_ALL)) == 0UL);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
    ImrFlags |= ((u32NotificationMask & ADC_SAR_IP_NOTIF_FLAG_NORMAL_EOC) != 0U) ? ADC_IMR_MSKEOC(1U) : 0U;
    ImrFlags |= ((u32NotificationMask & ADC_SAR_IP_NOTIF_FLAG_INJECTED_EOC) != 0U) ? ADC_IMR_MSKJEOC(1U) : 0U;
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */
#if (STD_ON == ADC_SAR_IP_ECH_ENABLED)
    ImrFlags |= ((u32NotificationMask & ADC_SAR_IP_NOTIF_FLAG_NORMAL_ENDCHAIN) != 0U) ? ADC_IMR_MSKECH(1U) : 0U;
#endif /* (STD_ON == ADC_SAR_IP_ECH_ENABLED) */
#if (STD_ON == ADC_SAR_IP_JECH_ENABLED)
    ImrFlags |= ((u32NotificationMask & ADC_SAR_IP_NOTIF_FLAG_INJECTED_ENDCHAIN) != 0U) ? ADC_IMR_MSKJECH(1U) : 0U;
#endif /* (STD_ON == ADC_SAR_IP_JECH_ENABLED) */
#if FEATURE_ADC_HAS_CTU
#if (STD_ON == ADC_SAR_IP_EOCTU_ENABLED)
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    if (ADC_SAR_IP_INST_HAS_CTU(u32Instance))
    {
#endif /*FEATURE_ADC_BAD_ACCESS_PROT_FEATURE*/
        ImrFlags |= ((u32NotificationMask & ADC_SAR_IP_NOTIF_FLAG_CTU_EOC) != 0U) ? ADC_IMR_MSKEOCTU(1U) : 0U;
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    }
#endif /*FEATURE_ADC_BAD_ACCESS_PROT_FEATURE*/
#endif /* (STD_ON == ADC_SAR_IP_EOCTU_ENABLED) */
#endif /* FEATURE_ADC_HAS_CTU */
    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_34();
    AdcBasePtr->IMR &= ~(ImrFlags);
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_34();
}
#endif /* (STD_ON == ADC_SAR_IP_ECH_ENABLED) .. */

#if ((STD_ON == ADC_SAR_IP_WDG_ENABLED) || (STD_ON == ADC_SAR_IP_EOC_ENABLED))
/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_EnableChannelNotifications
* Description   : This function enables End-of-Conversion and watchdog
* interrupts generation for a single channel.
*
* @implements      Adc_Sar_Ip_EnableChannelNotifications_Activity
* END**************************************************************************/
void Adc_Sar_Ip_EnableChannelNotifications(const uint32 u32Instance,
                                           const uint32 u32ChnIdx,
                                           const uint32 u32Mask)
{
    uint32 VectAdr;
    uint32 VectBit;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(u32ChnIdx < ADC_SAR_IP_CDR_COUNT);
    DevAssert((u32Mask & ~(ADC_SAR_IP_CHAN_NOTIF_FLAG_ALL)) == 0UL);
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
    DevAssert(ADC_SAR_IP_INST_HAS_CHANn(u32Instance, u32ChnIdx));
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    VectAdr = ADC_SAR_IP_CHAN_2_VECT(u32ChnIdx);
    VectBit = ADC_SAR_IP_CHAN_2_BIT(u32ChnIdx);

#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
    if ((u32Mask & ADC_SAR_IP_CHAN_NOTIF_EOC) != 0u)
    {
        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_30();
        CIMR(AdcBasePtr, VectAdr) |= 1UL << VectBit;
        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_30();
    }
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */

#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
    if ((u32Mask & ADC_SAR_IP_CHAN_NOTIF_WDG) != 0u)
    {
        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_28();
        CWENR(AdcBasePtr, VectAdr) |= 1UL << VectBit;
        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_28();
    }
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_DisableChannelNotifications
* Description   : This function disables End-of-Conversion and watchdog
* interrupts generation for a single channel.
*
* @implements      Adc_Sar_Ip_DisableChannelNotifications_Activity
* END**************************************************************************/
void Adc_Sar_Ip_DisableChannelNotifications(const uint32 u32Instance,
                                            const uint32 u32ChnIdx,
                                            const uint32 u32Mask)
{
    uint32 VectAdr;
    uint32 VectBit;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(u32ChnIdx < ADC_SAR_IP_CDR_COUNT);
    DevAssert((u32Mask & ~(ADC_SAR_IP_CHAN_NOTIF_FLAG_ALL)) == 0UL);
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
    DevAssert(ADC_SAR_IP_INST_HAS_CHANn(u32Instance, u32ChnIdx));
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    VectAdr = ADC_SAR_IP_CHAN_2_VECT(u32ChnIdx);
    VectBit = ADC_SAR_IP_CHAN_2_BIT(u32ChnIdx);

#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
    if ((u32Mask & ADC_SAR_IP_CHAN_NOTIF_EOC) != 0u)
    {
        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_31();
        CIMR(AdcBasePtr, VectAdr) &= ~(1UL << VectBit);
        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_31();
    }
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */

#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
    if ((u32Mask & ADC_SAR_IP_CHAN_NOTIF_WDG) != 0u)
    {
        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_29();
        CWENR(AdcBasePtr, VectAdr) &= ~(1UL << VectBit);
        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_29();
    }
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */
}
#endif /* ((STD_ON == ADC_SAR_IP_WDG_ENABLED) || (STD_ON == ADC_SAR_IP_EOC_ENABLED)) */

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_SetClockMode
* Description   : This function initializes the ADC clock configuration.
*
* @implements      Adc_Sar_Ip_SetClockMode_Activity
* END**************************************************************************/
Adc_Sar_Ip_StatusType Adc_Sar_Ip_SetClockMode(const uint32 u32Instance,
                                              const Adc_Sar_Ip_ClockConfigType * const pConfig)
{
    Adc_Sar_Ip_StatusType Status = ADC_SAR_IP_STATUS_SUCCESS;
#if ADC_SAR_IP_HAS_ADCLKSEL
    uint32 Mcr = 0u;
#endif /* ADC_SAR_IP_HAS_ADCLKSEL */

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(pConfig != NULL_PTR);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

#if ADC_SAR_IP_HAS_ADCLKSEL
    /* Read MCR value and clear values that will be replaced */
    Mcr |= Adc_Sar_GetADCLKSELValue(pConfig->ClkSelect);
#endif /* ADC_SAR_IP_HAS_ADCLKSEL */
    Status = Adc_Sar_Ip_Powerdown(u32Instance);
    if (ADC_SAR_IP_STATUS_SUCCESS == Status)
    {
#if ADC_SAR_IP_HAS_ADCLKSEL
        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_15();
        AdcBasePtr->MCR = (AdcBasePtr->MCR & ~(ADC_MCR_ADCLKSEL_MASK)) | Mcr;
        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_15();
#endif /* ADC_SAR_IP_HAS_ADCLKSEL */

        Status = Adc_Sar_Ip_Powerup(u32Instance);
        if (ADC_SAR_IP_STATUS_SUCCESS == Status)
        {
#if FEATURE_ADC_HAS_HIGH_SPEED_ENABLE
            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_51();
            /* Enables high speed conversion or calibration */
            Adc_Sar_EnableHighSpeed(AdcBasePtr, pConfig->HighSpeedConvEn);
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_51();
#endif /* FEATURE_ADC_HAS_HIGH_SPEED_ENABLE */

#if FEATURE_ADC_HAS_AVERAGING
            Adc_Sar_Ip_SetAveraging(u32Instance, pConfig->AvgEn, pConfig->AvgSel);
#endif /* FEATURE_ADC_HAS_AVERAGING */

            /* Set powerdown delay */
            AdcBasePtr->PDEDR = ADC_PDEDR_PDED(pConfig->PowerDownDelay);
#if FEATURE_ADC_HAS_CONVERSION_TIMING
            /* Set sample times */
            Adc_Sar_Ip_SetSampleTimes(u32Instance, pConfig->SampleTimeArr);
#endif /* FEATURE_ADC_HAS_CONVERSION_TIMING */
        }
    }

    return Status;
}

#if FEATURE_ADC_HAS_CONVERSION_TIMING
/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_SetSampleTimes
* Description   : This function sets the sample times for each channnel group.
*
* @implements      Adc_Sar_Ip_SetSampleTimes_Activity
* END**************************************************************************/
void Adc_Sar_Ip_SetSampleTimes(const uint32 u32Instance,
                               const uint8 * const aSampleTimes)
{
    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];
    uint8 Index;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    for (Index = 0u; Index < Adc_Sar_Ip_au8AdcGroupCount[u32Instance]; Index++)
    {
        /* Set sample times */
        CTR(AdcBasePtr, Index) = ADC_CTR_INPSAMP(aSampleTimes[Index]);
    }
}
#endif /* FEATURE_ADC_HAS_CONVERSION_TIMING */

#if FEATURE_ADC_HAS_AVERAGING
/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_SetAveraging
* Description   : This function sets the sample times for each channnel group.
*
* @implements      Adc_Sar_Ip_SetAveraging_Activity
* END**************************************************************************/
void Adc_Sar_Ip_SetAveraging(const uint32 u32Instance,
                             const boolean bAvgEn,
                             const Adc_Sar_Ip_AvgSelectType eAvgSel)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_16();
    /* Clear old values */
    AdcBasePtr->MCR &= ~(ADC_MCR_AVGEN_MASK | ADC_MCR_AVGS_MASK);

    /* Write new values */
    AdcBasePtr->MCR |= ADC_MCR_AVGEN(bAvgEn ? 1U : 0U) | ADC_MCR_AVGS(eAvgSel);
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_16();
}
#endif /* FEATURE_ADC_HAS_AVERAGING */

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_AbortConversion
* Description   : This function aborts an ongoing conversion.
*
* @implements      Adc_Sar_Ip_AbortConversion_Activity
* END**************************************************************************/
void Adc_Sar_Ip_AbortConversion(const uint32 u32Instance)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */
    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_17();
    AdcBasePtr->MCR |= ADC_MCR_ABORT(1U);
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_17();
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_AbortChain
* Description   : This function aborts an ongoing chain of conversions.
*
* @implements      Adc_Sar_Ip_AbortChain_Activity
* END**************************************************************************/
Adc_Sar_Ip_StatusType Adc_Sar_Ip_AbortChain(const uint32 u32Instance,
                                            const boolean bBlocking,
                                            const boolean bAllowRestart)
{
    uint32 MsrStatus;
    uint32 TimeoutTicks = OsIf_MicrosToTicks(ADC_SAR_IP_TIMEOUT_VAL, ADC_SAR_IP_TIMEOUT_TYPE);
    uint32 CurrentTicks = OsIf_GetCounter(ADC_SAR_IP_TIMEOUT_TYPE);
    uint32 ElapsedTicks = 0u;
    Adc_Sar_Ip_StatusType ReturnStatus = ADC_SAR_IP_STATUS_SUCCESS;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_18();
    /* The behavior of the ADC depends on MCR[MODE] (One-Shot/Scan Operation modes).
    If Scan Operation mode is disabled, MSR[NSTART] is automatically reset together with MCR[ABORTCHAIN].
    Otherwise, in Scan Operation mode, a new chain is started. Therefore MCR[NSTART] should be cleared
    before abort the current chain of conversions. */
    if (!bAllowRestart)
    {
        AdcBasePtr->MCR &= ~(ADC_MCR_NSTART(1U));
    }
    AdcBasePtr->MCR |= ADC_MCR_ABORTCHAIN(1U);
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_18();

    if (bBlocking)
    {
        /* Wait for the ongoing conversion to stop */
        MsrStatus = AdcBasePtr->MSR & ADC_MSR_NSTART_MASK;
        while ((MsrStatus == ADC_MSR_NSTART_MASK) && (ElapsedTicks < TimeoutTicks))
        {
            MsrStatus = AdcBasePtr->MSR & ADC_MSR_NSTART_MASK;
            ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, ADC_SAR_IP_TIMEOUT_TYPE);
        }
        if (ElapsedTicks >= TimeoutTicks)
        {
            ReturnStatus = ADC_SAR_IP_STATUS_TIMEOUT;
        }
#if !FEATURE_ADC_SAR_W1C_ABORT
        else
        {
            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_18();
            /* Clear the ABORT_CHAIN bit if set, otherwise the next chain will be ABORTED without any End of Chain Interrupt */
            if ((AdcBasePtr->MCR & ADC_MCR_ABORTCHAIN_MASK) != 0u)
            {
                AdcBasePtr->MCR &= ~(ADC_MCR_ABORTCHAIN(1U));
            }
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_18();
        }
#endif /* FEATURE_ADC_SAR_W1C_ABORT */
    }

    return ReturnStatus;
}

#if FEATURE_ADC_HAS_PRESAMPLING
/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_SetPresamplingSource
* Description   : This function configures the Presampling Source for a channel group (0-31, 32-63, 64 -95)
*
* @implements      Adc_Sar_Ip_SetPresamplingSource_Activity
* END**************************************************************************/
void Adc_Sar_Ip_SetPresamplingSource(const uint32 u32Instance,
                                     const Adc_Sar_Ip_ChanGroupType pChanGroup,
                                     const Adc_Sar_Ip_PresamplingSourceType pPresampleSource)
{
    uint32 Pscr;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    if (ADC_SAR_IP_INST_HAS_PSCR(u32Instance))
    {
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_FEATURE */
        ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_38();

        Pscr = AdcBasePtr->PSCR;

        switch (pChanGroup)
        {
#ifdef ADC_PSCR_PREVAL0
            case ADC_SAR_IP_CHAN_GROUP_0:
                Pscr &= ~(ADC_PSCR_PREVAL0_MASK);
                Pscr |= ADC_PSCR_PREVAL0((uint32)pPresampleSource);
                break;
#endif /* ADC_PSCR_PREVAL0 */
#ifdef ADC_PSCR_PREVAL1
            case ADC_SAR_IP_CHAN_GROUP_1:
                Pscr &= ~(ADC_PSCR_PREVAL1_MASK);
                Pscr |= ADC_PSCR_PREVAL1((uint32)pPresampleSource);
                break;
#endif /* ADC_PSCR_PREVAL1 */
#ifdef ADC_PSCR_PREVAL2
            case ADC_SAR_IP_CHAN_GROUP_2:
                Pscr &= ~(ADC_PSCR_PREVAL2_MASK);
                Pscr |= ADC_PSCR_PREVAL2((uint32)pPresampleSource);
                break;
#endif /* ADC_PSCR_PREVAL2 */
            default:
                ; /* no-op */
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
                /* Not supported */
                DevAssert(FALSE);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */
                break;
        }

        AdcBasePtr->PSCR = Pscr;

        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_38();
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    }
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_FEATURE */
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_EnableChannelPresampling
* Description   : This function enables the Presampling on one channel of the ADC.
*
* @implements      Adc_Sar_Ip_EnableChannelPresampling_Activity
* END**************************************************************************/
void Adc_Sar_Ip_EnableChannelPresampling(const uint32 u32Instance,
                                         const uint32 u32ChnIdx)
{
    uint32 VectAdr;
    uint32 VectBit;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(u32ChnIdx < ADC_SAR_IP_CDR_COUNT);
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
    DevAssert(ADC_SAR_IP_INST_HAS_CHANn(u32Instance, u32ChnIdx));
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    if (ADC_SAR_IP_INST_HAS_PSCR(u32Instance))
    {
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_FEATURE */
        ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

        VectAdr = ADC_SAR_IP_CHAN_2_VECT(u32ChnIdx);
        VectBit = ADC_SAR_IP_CHAN_2_BIT(u32ChnIdx);

        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_41();
        PSR(AdcBasePtr, VectAdr) |= 1UL << VectBit;
        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_41();

#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    }
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_FEATURE */
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_DisableChannelPresampling
* Description   : This function disables the Presampling on one channel of the ADC.
*
* @implements      Adc_Sar_Ip_DisableChannelPresampling_Activity
* END**************************************************************************/
void Adc_Sar_Ip_DisableChannelPresampling(const uint32 u32Instance,
                                          const uint32 u32ChnIdx)
{
    uint32 VectAdr;
    uint32 VectBit;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(u32ChnIdx < ADC_SAR_IP_CDR_COUNT);
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
    DevAssert(ADC_SAR_IP_INST_HAS_CHANn(u32Instance, u32ChnIdx));
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    if (ADC_SAR_IP_INST_HAS_PSCR(u32Instance))
    {
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_FEATURE */
        ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

        VectAdr = ADC_SAR_IP_CHAN_2_VECT(u32ChnIdx);
        VectBit = ADC_SAR_IP_CHAN_2_BIT(u32ChnIdx);

        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_42();
        PSR(AdcBasePtr, VectAdr) &= ~(1UL << VectBit);
        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_42();

#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    }
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_FEATURE */
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_EnablePresampleConversion
* Description   : This function enables bypass of the Sampling Phase, resulting in a conversion
* of the presampled data. This is available only for channels that have presampling
* enabled.
*
* @implements      Adc_Sar_Ip_EnablePresampleConversion_Activity
* END**************************************************************************/
void Adc_Sar_Ip_EnablePresampleConversion(const uint32 u32Instance)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    if (ADC_SAR_IP_INST_HAS_PSCR(u32Instance))
    {
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_FEATURE */
        ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_39();
        AdcBasePtr->PSCR |= ADC_PSCR_PRECONV(1U);
        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_39();

#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    }
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_FEATURE */
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_DisablePresampleConversion
* Description   : This function disables Sampling Phase bypass.
*
* @implements      Adc_Sar_Ip_DisablePresampleConversion_Activity
* END**************************************************************************/
void Adc_Sar_Ip_DisablePresampleConversion(const uint32 u32Instance)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    if (ADC_SAR_IP_INST_HAS_PSCR(u32Instance))
    {
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_FEATURE */
        ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_40();
        AdcBasePtr->PSCR &= ~(ADC_PSCR_PRECONV(1U));
        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_40();

#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
    }
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_FEATURE */
}
#endif /* FEATURE_ADC_HAS_PRESAMPLING */

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_EnableDma
* Description   : This function enables requests to DMA from ADC
*
* @implements      Adc_Sar_Ip_EnableDma_Activity
* END**************************************************************************/
void Adc_Sar_Ip_EnableDma(const uint32 u32Instance)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_43();
    AdcBasePtr->DMAE |= ADC_DMAE_DMAEN(1U);
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_43();
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_DisableDma
* Description   : This function disables requests to DMA from ADC
*
* @implements      Adc_Sar_Ip_DisableDma_Activity
* END**************************************************************************/
void Adc_Sar_Ip_DisableDma(const uint32 u32Instance)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_44();
    AdcBasePtr->DMAE &= ~(ADC_DMAE_DMAEN(1U));
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_44();
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_EnableChannelDma
* Description   : This function enables DMA requests triggered by End of Conversion event from
*  a selected channel.
*
* @implements      Adc_Sar_Ip_EnableChannelDma_Activity
* END**************************************************************************/
void Adc_Sar_Ip_EnableChannelDma(const uint32 u32Instance,
                                 const uint32 u32ChnIdx)
{
    uint32 VectAdr;
    uint32 VectBit;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(u32ChnIdx < ADC_SAR_IP_CDR_COUNT);
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
    DevAssert(ADC_SAR_IP_INST_HAS_CHANn(u32Instance, u32ChnIdx));
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    VectAdr = ADC_SAR_IP_CHAN_2_VECT(u32ChnIdx);
    VectBit = ADC_SAR_IP_CHAN_2_BIT(u32ChnIdx);

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_46();
    DMAR(AdcBasePtr, VectAdr) |= 1UL << VectBit;
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_46();
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_DisableChannelDma
* Description   : This function disables DMA requests triggered by End of Conversion event from
*  a selected channel.
*
* @implements      Adc_Sar_Ip_DisableChannelDma_Activity
* END**************************************************************************/
void Adc_Sar_Ip_DisableChannelDma(const uint32 u32Instance,
                                  const uint32 u32ChnIdx)
{
    uint32 VectAdr;
    uint32 VectBit;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(u32ChnIdx < ADC_SAR_IP_CDR_COUNT);
#if FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL
    DevAssert(ADC_SAR_IP_INST_HAS_CHANn(u32Instance, u32ChnIdx));
#endif /* FEATURE_ADC_BAD_ACCESS_PROT_CHANNEL */
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    VectAdr = ADC_SAR_IP_CHAN_2_VECT(u32ChnIdx);
    VectBit = ADC_SAR_IP_CHAN_2_BIT(u32ChnIdx);

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_47();
    DMAR(AdcBasePtr, VectAdr) &= ~(1UL << VectBit);
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_47();
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_DisableChannelDmaAll
* Description   : This function disables DMA requests triggered by End of Conversion event from
*  all channels.
*
* @implements      Adc_Sar_Ip_DisableChannelDmaAll_Activity
* END**************************************************************************/
void Adc_Sar_Ip_DisableChannelDmaAll(const uint32 u32Instance)
{
    uint8 Index;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    for (Index = 0U; Index < Adc_Sar_Ip_au8AdcGroupCount[u32Instance]; Index++)
    {
        SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_48();
        DMAR(AdcBasePtr, Index) &= ~(Adc_Sar_Ip_au32AdcChanBitmap[u32Instance][Index]);
        SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_48();
    }
}

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_SetDmaClearSource
* Description   : This function selects the DMA Request Flag Clear Source.
*
* @implements      Adc_Sar_Ip_SetDmaClearSource_Activity
* END**************************************************************************/
void Adc_Sar_Ip_SetDmaClearSource(const uint32 u32Instance,
                                  const Adc_Sar_Ip_ClearSourceType pDmaClear)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_45();
    AdcBasePtr->DMAE &= ~(ADC_DMAE_DCLR_MASK);
    AdcBasePtr->DMAE |= ADC_DMAE_DCLR((uint32)pDmaClear);
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_45();
}

#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_SetWdgThreshold
* Description   : This function sets the watchdog thresholds for a certain register.
*
* @implements      Adc_Sar_Ip_SetWdgThreshold_Activity
* END**************************************************************************/
void Adc_Sar_Ip_SetWdgThreshold(const uint32 u32Instance,
                                const uint8 u8RegisterIdx,
                                const Adc_Sar_Ip_WdgThresholdType * const pThresholdValues)
{
#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
    uint8 Shift;
    uint16 HighThreshold;
    uint16 LowThreshold;
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */
    uint32 Wtimr;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(pThresholdValues != NULL_PTR);
    DevAssert(u8RegisterIdx < ADC_SAR_IP_THRHLR_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
    if (FALSE == Adc_Sar_Ip_axAdcSarState[u32Instance].BypassResolution)
    {
        /* Calculate the shift of the threshold value to get the number of bits
         * needed by the hardware unit. */
        Shift = ADC_SAR_IP_RESULT_RESOLUTION - Adc_Sar_GetResolution(u32Instance);
        /* Shift the threshold to obtain the value to be written in the threshold register. */
        HighThreshold = (pThresholdValues->HighThreshold) << Shift;
        LowThreshold = (pThresholdValues->LowThreshold) << Shift;

        /* If the bits of the high threshold value that are ignored due to
         * resolution remain 0, this will trigger interrupts when the conversion
         * value is equal to the high threshold value because the ignored bits
         * of the conversion result will be more than 0, so the high threshold
         * value will need to have the ignored bits set to 1. */
        HighThreshold |= (((uint16)1U << Shift) - 1U);

        Adc_Sar_WriteThresholds(AdcBasePtr, u8RegisterIdx, HighThreshold, LowThreshold);
    }
    else
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */
    {
        Adc_Sar_WriteThresholds(AdcBasePtr, u8RegisterIdx, pThresholdValues->HighThreshold, pThresholdValues->LowThreshold);
    }

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_32();
    Wtimr = AdcBasePtr->WTIMR;
    if (pThresholdValues->HighThresholdIntEn)
    {
        Wtimr |= (ADC_SAR_IP_WDG_REG_MASK_HIGH(u8RegisterIdx));
    }
    else
    {
        Wtimr &= (~ADC_SAR_IP_WDG_REG_MASK_HIGH(u8RegisterIdx));
    }
    if (pThresholdValues->LowThresholdIntEn)
    {
        Wtimr |= (ADC_SAR_IP_WDG_REG_MASK_LOW(u8RegisterIdx));
    }
    else
    {
        Wtimr &= (~ADC_SAR_IP_WDG_REG_MASK_LOW(u8RegisterIdx));
    }
    AdcBasePtr->WTIMR = Wtimr;
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_32();
}
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */

/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_SetConversionMode
* Description   : This function configures the used conversion mode.
* Note: The ADC instance must be in IDLE state in order to change this.
*
* @implements      Adc_Sar_Ip_SetConversionMode_Activity
* END**************************************************************************/
void Adc_Sar_Ip_SetConversionMode(const uint32 u32Instance,
                                const Adc_Sar_Ip_ConvModeType eConvMode)
{
    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
    /* The selected ADC should be in IDLE state */
    DevAssert((AdcBasePtr->MSR & ADC_MSR_ADCSTATUS_MASK) == ADC_MSR_ADCSTATUS(ADC_SAR_IP_MSR_ADCSTATUS_IDLE));
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    /* Edit MODE */
    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_19();
    switch (eConvMode)
    {
        case ADC_SAR_IP_CONV_MODE_ONESHOT:
            AdcBasePtr->MCR &= ~(ADC_MCR_MODE(1u)); /* MODE = 0 */
            break;
        case ADC_SAR_IP_CONV_MODE_SCAN:
            AdcBasePtr->MCR |= ADC_MCR_MODE(1u); /* MODE = 1 */
            break;
        default:
            ; /* no-op */
            break;
    }
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_19();
}

#if FEATURE_ADC_HAS_CTU
/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_SetCtuMode
* Description   : This function configures the CTU mode and enable.
*
* @implements      Adc_Sar_Ip_SetCtuMode_Activity
* END**************************************************************************/
Adc_Sar_Ip_StatusType Adc_Sar_Ip_SetCtuMode(const uint32 u32Instance,
                                            const Adc_Sar_Ip_CtuModeType eCtuMode)
{
    Adc_Sar_Ip_StatusType Status = ADC_SAR_IP_STATUS_SUCCESS;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    ADC_Type * const AdcBasePtr = Adc_Sar_Ip_apxAdcBase[u32Instance];

    /* Clear CTUEN to allow Powerdown operation to succeed. */
    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_20();
    AdcBasePtr->MCR &= ~ADC_MCR_CTUEN_MASK;
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_20();

    /* Power down */
    Status = Adc_Sar_Ip_Powerdown(u32Instance);
    if (ADC_SAR_IP_STATUS_SUCCESS == Status)
    {
        /* Configure CTU */
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
        if (ADC_SAR_IP_INST_HAS_CTU(u32Instance))
        {
#endif /*FEATURE_ADC_BAD_ACCESS_PROT_FEATURE*/
            SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_20();
            switch (eCtuMode)
            {
                case ADC_SAR_IP_CTU_MODE_CONTROL:
                    AdcBasePtr->MCR &= ~(ADC_MCR_CTU_MODE(1U)); /* CTU_MODE = 0 */
                    AdcBasePtr->MCR |= ADC_MCR_CTUEN(1U); /* CTUEN = 1 */
                    break;
#if FEATURE_ADC_HAS_CTU_TRIGGER_MODE
                case ADC_SAR_IP_CTU_MODE_TRIGGER:
                    /* Set CTU_MODE = 1 and CTUEN = 1 */
                    AdcBasePtr->MCR |= (ADC_MCR_CTU_MODE(1U) | ADC_MCR_CTUEN(1U));
                    break;
#endif /* FEATURE_ADC_HAS_CTU_TRIGGER_MODE */
                    /* Set CTU_MODE = 0 and CTUEN = 0 */
                case ADC_SAR_IP_CTU_MODE_DISABLED:
                    AdcBasePtr->MCR &= ~(ADC_MCR_CTU_MODE(1U) | ADC_MCR_CTUEN(1U));
                    break;
                default:
                    ; /* no-op */
                    break;
            }
            SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_20();
#if FEATURE_ADC_BAD_ACCESS_PROT_FEATURE
        }
#endif /*FEATURE_ADC_BAD_ACCESS_PROT_FEATURE*/

        /* Power up */
        Status = Adc_Sar_Ip_Powerup(u32Instance);
    }

    return Status;
}
#endif /* FEATURE_ADC_HAS_CTU */

#if (FEATURE_ADC_HAS_INJ_EXT_TRIGGER || FEATURE_ADC_HAS_EXT_TRIGGER)
/*FUNCTION**********************************************************************
*
* Function Name : Adc_Sar_Ip_SetExternalTrigger
* Description   : This function configures the external trigger.
*
* @implements      Adc_Sar_Ip_SetExternalTrigger_Activity
* END**************************************************************************/
void Adc_Sar_Ip_SetExternalTrigger(const uint32 u32Instance,
                                   const Adc_Sar_Ip_ExtTriggerEdgeType eTriggerEdge,
                                   const Adc_Sar_Ip_ExtTriggerSourceType eTrggerSrc)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < ADC_SAR_IP_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    /* Switch between injected and normal external trigger */
    switch (eTrggerSrc)
    {
#if FEATURE_ADC_HAS_EXT_TRIGGER
        case ADC_SAR_IP_NORMAL_EXT_TRIG:
            Adc_Sar_ConfigExternalTrigger(u32Instance, eTriggerEdge, ADC_MCR_EDGE_MASK, 0U, ADC_MCR_TRGEN_MASK);
            break;
#if FEATURE_ADC_HAS_AUX_EXT_TRIGGER
        case ADC_SAR_IP_AUX_NORMAL_EXT_TRIG:
            Adc_Sar_ConfigExternalTrigger(u32Instance, eTriggerEdge, ADC_MCR_EDGE_MASK, 0U, ADC_MCR_XSTRTEN_MASK);
            break;
        case ADC_SAR_IP_ALL_NORMAL_EXT_TRIG:
            Adc_Sar_ConfigExternalTrigger(u32Instance, eTriggerEdge, ADC_MCR_EDGE_MASK, ADC_MCR_EDGE_MASK, (ADC_MCR_TRGEN_MASK | ADC_MCR_XSTRTEN_MASK));
            break;
#endif /* FEATURE_ADC_HAS_AUX_EXT_TRIGGER */
#endif /* FEATURE_ADC_HAS_EXT_TRIGGER */
#if FEATURE_ADC_HAS_INJ_EXT_TRIGGER
        case ADC_SAR_IP_INJECTED_EXT_TRIG:
            Adc_Sar_ConfigExternalTrigger(u32Instance, eTriggerEdge, ADC_MCR_JEDGE_MASK, ADC_MCR_JEDGE_MASK, ADC_MCR_JTRGEN_MASK);
            break;
#endif /* FEATURE_ADC_HAS_INJ_EXT_TRIGGER */
        default:
            ; /* no-op */
            break;
    }
}
#endif /* (FEATURE_ADC_HAS_INJ_EXT_TRIGGER || FEATURE_ADC_HAS_EXT_TRIGGER) */

#if FEATURE_ADC_HAS_TEMPSENSE_CHN
#if (ADC_SAR_IP_TEMPSENSE_ENABLED == STD_ON)
/*FUNCTION**********************************************************************
 *
 * Function Name : Adc_Sar_Ip_TempSenseCalculateTemp
 * Description   : This function calculates the temperature on chip from the provided
 * parameter.
 *
 * @implements      Adc_Sar_Ip_TempSenseCalculateTemp_Activity
 * END**************************************************************************/
uint16 Adc_Sar_Ip_TempSenseCalculateTemp(const uint32 u32AdcInstance,
                                         const uint32 u32TempInstance,
                                         const uint16 u16TempSenseAdcConvData)
{
    uint32 Vets;
    uint32 Vref;
    uint32 TempValue;
    sint32 Tets;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32TempInstance < TEMPSENSE_INSTANCE_COUNT);
    DevAssert(AdcVoltageRefs[u32AdcInstance] <= 0x58U); /* Vref should be less than 5.5V */
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    const TEMPSENSE_Type * const TempSenseBasePtr = Adc_Sar_Ip_apxTempsenseBase[u32TempInstance];
    uint32 Tca0 = TempSenseBasePtr->TCA0;
    uint32 Tca1 = TempSenseBasePtr->TCA1;
    uint32 Tca2 = TempSenseBasePtr->TCA2;

    if (AdcVoltageRefs[u32AdcInstance] > ADC_SAR_IP_TEMPSENSE_VOLTAGE_SUPPLY)
    {
        Vref = Adc_Sar_TempsenseConvFp2Int(ADC_SAR_IP_TEMPSENSE_VOLTAGE_SUPPLY);
    }
    else
    {
        Vref = Adc_Sar_TempsenseConvFp2Int(AdcVoltageRefs[u32AdcInstance]);
    }

    if ((Tca0 & ADC_SAR_IP_TEMPSENSE_SIGN_MASK) != 0U)
    {
        Tets = - (sint32)(Adc_Sar_TempsenseConvFp2Int(Tca0));
    }
    else
    {
        Tets = (sint32)(Adc_Sar_TempsenseConvFp2Int(Tca0));
    }

    Vets = (uint32)((Vref * u16TempSenseAdcConvData) / ADC_SAR_IP_TEMPSENSE_RESOLUTION_12B);
    TempValue = (Adc_Sar_TempsenseConvFp2Int(Tca1) * Vets) / ADC_SAR_IP_TEMPSENSE_MULTIPLIER;

    if ((Tca1 & ADC_SAR_IP_TEMPSENSE_SIGN_MASK) != 0U)
    {
        Tets -= (sint32)(TempValue);
    }
    else
    {
        Tets += (sint32)(TempValue);
    }

    Vets = (Vets * Vets) / ADC_SAR_IP_TEMPSENSE_MULTIPLIER;
    TempValue = (Adc_Sar_TempsenseConvFp2Int(Tca2) * Vets) / ADC_SAR_IP_TEMPSENSE_MULTIPLIER;

    if ((Tca2 & ADC_SAR_IP_TEMPSENSE_SIGN_MASK) != 0U)
    {
        Tets -= (sint32)(TempValue);
    }
    else
    {
        Tets += (sint32)(TempValue);
    }

    return Adc_Sar_TempsenseConvInt2Fp(Tets);
}


/*FUNCTION**********************************************************************
 *
 * Function Name : Adc_Sar_Ip_TempSenseGetTemp
 * Description   : This function starts a normal software conversion on tempsense
 * channel and calculates the temperature on chip from the data conversion
 * parameter. Tempsense calculations are done using 12bits operators, so it is
 * recommended for Adc to run on minimum resolution of 12-bit.
 *
 * @implements      Adc_Sar_Ip_TempSenseGetTemp_Activity
 * END**************************************************************************/
Adc_Sar_Ip_StatusType Adc_Sar_Ip_TempSenseGetTemp(const uint32 u32AdcInstance,
                                                  const uint32 u32TempInstance,
                                                  uint16 * const pTempSenseVal)
{
    Adc_Sar_Ip_StatusType ReturnStatus = ADC_SAR_IP_STATUS_SUCCESS;
    uint32 TimeoutTicks = OsIf_MicrosToTicks(ADC_SAR_IP_TIMEOUT_VAL, ADC_SAR_IP_TIMEOUT_TYPE);
    uint32 CurrentTicks = OsIf_GetCounter(ADC_SAR_IP_TIMEOUT_TYPE);
    uint32 ElapsedTicks = 0u;
    uint32 ExpectedStatus;
    uint32 ConvFlagStatus;
    uint32 Cdr;
    uint16 Result;
#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
    uint8 Resolution;
#else
    uint8 Resolution = ADC_SAR_IP_MAX_RESOLUTION;
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */
    uint32 VectAdr;
    uint32 VectBit;
    ADC_Type * const AdcBasePtr  = Adc_Sar_Ip_apxAdcBase[u32AdcInstance];
#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
    uint32 Cimr[ADC_SAR_IP_NUM_GROUP_CHAN];
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */
#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
    uint32 Cwenr[ADC_SAR_IP_NUM_GROUP_CHAN];
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */
    const uint32 Imr = AdcBasePtr->IMR;
    const uint32 Wtimr = AdcBasePtr->WTIMR;

#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32AdcInstance < ADC_SAR_IP_INSTANCE_COUNT);
    DevAssert(u32TempInstance < TEMPSENSE_INSTANCE_COUNT);
    DevAssert(AdcVoltageRefs[u32AdcInstance] <= 0x58U); /* Vref should be less than 5.5V */
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */

    /* Store the state of control interrupt registers and disable them,
     * Since this function cannt work when any ISR enabled */
    AdcBasePtr->IMR = 0UL;
    AdcBasePtr->WTIMR = 0UL;
#if ((STD_ON == ADC_SAR_IP_WDG_ENABLED) || (STD_ON == ADC_SAR_IP_EOC_ENABLED))
    for (VectAdr = 0U; VectAdr < Adc_Sar_Ip_au8AdcGroupCount[u32AdcInstance]; VectAdr++)
    {
#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
        Cimr[VectAdr] = CIMR(AdcBasePtr, VectAdr);
        CIMR(AdcBasePtr, VectAdr) = 0UL;
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */

#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
        Cwenr[VectAdr] = CWENR(AdcBasePtr, VectAdr);
        CWENR(AdcBasePtr, VectAdr) = 0UL;
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */
    }
#endif /* ((STD_ON == ADC_SAR_IP_WDG_ENABLED) || (STD_ON == ADC_SAR_IP_EOC_ENABLED)) */

    VectAdr = ADC_SAR_IP_CHAN_2_VECT(ADC_SAR_IP_TEMPSENSE_CHANNEL);
    VectBit = ADC_SAR_IP_CHAN_2_BIT(ADC_SAR_IP_TEMPSENSE_CHANNEL);
    /* Enable tempsense channel */
    NCMR(AdcBasePtr, VectAdr) |= (1UL << VectBit);
    /* One-shot mode */
    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_22();
    AdcBasePtr->MCR &= ~(ADC_MCR_MODE(1U));
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_22();

    /* Start normal software conversion */
    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_22();
    AdcBasePtr->MCR |= ADC_MCR_NSTART(1U);
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_22();

    /* Wait for the conversion to be finished */
    ExpectedStatus = (1UL << VectBit);
    ConvFlagStatus = CEOCFR(AdcBasePtr, VectAdr) & (1UL << VectBit);
    while ((ConvFlagStatus != ExpectedStatus) && (ElapsedTicks < TimeoutTicks))
    {
        ConvFlagStatus = CEOCFR(AdcBasePtr, VectAdr) & (1UL << VectBit);
        ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, ADC_SAR_IP_TIMEOUT_TYPE);
    }
    if (ElapsedTicks >= TimeoutTicks)
    {
        ReturnStatus = ADC_SAR_IP_STATUS_TIMEOUT;
    }

    if (ReturnStatus == ADC_SAR_IP_STATUS_SUCCESS)
    {
        Cdr = CDR(AdcBasePtr, (uint32)ADC_SAR_IP_TEMPSENSE_CHANNEL);
        if (((Cdr & ADC_CDR_VALID_MASK) != 0U))
        {
#if (ADC_SAR_IP_SET_RESOLUTION == STD_ON)
            if (FALSE == Adc_Sar_Ip_axAdcSarState[u32AdcInstance].BypassResolution)
            {
                Resolution = Adc_Sar_GetResolution(u32AdcInstance);
            }
            else
            {
                Resolution = ADC_SAR_IP_RESULT_RESOLUTION;
            }
#endif /* (ADC_SAR_IP_SET_RESOLUTION == STD_ON) */
            Result = Adc_Sar_GetMaskedResult(u32AdcInstance, Cdr);
            /* the data is correct, store the result in 12bit resolution and clear the flag */
            if (Adc_Sar_Ip_axAdcSarState[u32AdcInstance].DataAlign == ADC_SAR_IP_DATA_ALIGNED_LEFT)
            {
                /* Assumption: the width of the register is less than 16 */
                Result = Result >> (16u - Resolution);
            }

            Result = (Resolution > ADC_SAR_IP_TEMPSENSE_RESOLUTION) ? \
            (Result >> (Resolution - ADC_SAR_IP_TEMPSENSE_RESOLUTION)) : \
            (Result << (ADC_SAR_IP_TEMPSENSE_RESOLUTION - Resolution));

            CEOCFR(AdcBasePtr, VectAdr) = ((uint32)1UL << VectBit); /* w1c bit */

            *pTempSenseVal = Adc_Sar_Ip_TempSenseCalculateTemp(u32AdcInstance, u32TempInstance, Result);
        }
    }

    /* Disable tempsense channel */
    NCMR(AdcBasePtr, VectAdr) &= ~(1UL << VectBit);

    /* Re-store control interrupt registers */
    AdcBasePtr->IMR = Imr;
    AdcBasePtr->WTIMR = Wtimr;
#if ((STD_ON == ADC_SAR_IP_WDG_ENABLED) || (STD_ON == ADC_SAR_IP_EOC_ENABLED))
    for (VectAdr = 0U; VectAdr < Adc_Sar_Ip_au8AdcGroupCount[u32AdcInstance]; VectAdr++)
    {
#if (STD_ON == ADC_SAR_IP_EOC_ENABLED)
        CIMR(AdcBasePtr, VectAdr) = Cimr[VectAdr];
#endif /* (STD_ON == ADC_SAR_IP_EOC_ENABLED) */

#if (STD_ON == ADC_SAR_IP_WDG_ENABLED)
        CWENR(AdcBasePtr, VectAdr) = Cwenr[VectAdr];
#endif /* (STD_ON == ADC_SAR_IP_WDG_ENABLED) */
    }
#endif /* ((STD_ON == ADC_SAR_IP_WDG_ENABLED) || (STD_ON == ADC_SAR_IP_EOC_ENABLED)) */

    return ReturnStatus;
}


/*FUNCTION**********************************************************************
 *
 * Function Name : Adc_Sar_Ip_TempSenseEnable
 * Description   : This function enables the temperature sensor.
 *
 * @implements      Adc_Sar_Ip_TempSenseEnable_Activity
 * END**************************************************************************/
void Adc_Sar_Ip_TempSenseEnable(const uint32 u32Instance)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < TEMPSENSE_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */
    TEMPSENSE_Type * const TempSenseBasePtr = Adc_Sar_Ip_apxTempsenseBase[u32Instance];

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_49();
    TempSenseBasePtr->ETSCTL |= TEMPSENSE_ETSCTL_ETS_EN_MASK;
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_49();
}

/*FUNCTION**********************************************************************
 *
 * Function Name : Adc_Sar_Ip_TempSenseDisable
 * Description   : This function disables the temperature sensor.
 *
 * @implements      Adc_Sar_Ip_TempSenseDisable_Activity
 * END**************************************************************************/
void Adc_Sar_Ip_TempSenseDisable(const uint32 u32Instance)
{
#if (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(u32Instance < TEMPSENSE_INSTANCE_COUNT);
#endif /* (ADC_SAR_IP_DEV_ERROR_DETECT == STD_ON) */
    TEMPSENSE_Type * const TempSenseBasePtr = Adc_Sar_Ip_apxTempsenseBase[u32Instance];

    SchM_Enter_Adc_ADC_EXCLUSIVE_AREA_50();
    TempSenseBasePtr->ETSCTL &= (~TEMPSENSE_ETSCTL_ETS_EN_MASK);
    SchM_Exit_Adc_ADC_EXCLUSIVE_AREA_50();
}
#endif /* (ADC_SAR_IP_TEMPSENSE_ENABLED == STD_ON) */
#endif /* FEATURE_ADC_HAS_TEMPSENSE_CHN */

/*FUNCTION*********************************************************************
 *
 * Function Name : Adc_Sar_Ip_GetDataAddress
 * Description   : Returns the address of the specified CDR register
 * @implements      Adc_Sar_Ip_GetDataAddress_Activity
 *END*************************************************************************/
uint32 Adc_Sar_Ip_GetDataAddress(uint32 u32Instance,
                                 uint32 u32ChannelIndex)
{
    return (uint32)&(CDR(Adc_Sar_Ip_apxAdcBase[u32Instance], u32ChannelIndex));
}

#if (STD_ON == ADC_SAR_IP_ENABLE_USER_MODE_SUPPORT)
/*FUNCTION**********************************************************************
 *
 * Function Name : Adc_Sar_Ip_SetUserAccessAllowed
 * Description   : Sets the UAA bit in REG_PROT to make the instance accessible in user mode.
 *
 *END**************************************************************************/
void Adc_Sar_Ip_SetUserAccessAllowed(const ADC_Type * const AdcBasePtr)
{
    SET_USER_ACCESS_ALLOWED((uint32)AdcBasePtr, SAR_ADC_PROT_MEM_U32);
}

/*FUNCTION**********************************************************************
 *
 * Function Name : Adc_Sar_Ip_ClrUserAccessAllowed
 * Description   : Clears the UAA bit in REG_PROT to make the instance inaccessible in user mode.
 *
 *END**************************************************************************/
void Adc_Sar_Ip_ClrUserAccessAllowed(const ADC_Type * const AdcBasePtr)
{
    CLR_USER_ACCESS_ALLOWED((uint32)AdcBasePtr, SAR_ADC_PROT_MEM_U32);
}
#endif /* (STD_ON == ADC_SAR_IP_ENABLE_USER_MODE_SUPPORT) */

#define ADC_STOP_SEC_CODE
#include "Adc_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
