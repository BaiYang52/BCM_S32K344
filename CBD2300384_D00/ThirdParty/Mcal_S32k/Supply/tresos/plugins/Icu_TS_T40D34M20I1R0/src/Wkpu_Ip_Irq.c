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
 *     @file
 *
 *     @addtogroup wkpu_icu_ip WKPU IPL
 *     @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                          INCLUDE FILES
*  1) system and project includes
*  2) needed interfaces from external units
*  3) internal and external interfaces from this unit
==================================================================================================*/
#include "StandardTypes.h"
#include "Wkpu_Ip.h"
#include "Wkpu_Ip_Irq.h"

#include "SchM_Icu.h"
/*==================================================================================================
*                                         LOCAL MACROS
==================================================================================================*/

#define WKPU_IP_IRQ_VENDOR_ID_C         43

#define WKPU_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C     4
#define WKPU_IP_IRQ_AR_RELEASE_MINOR_VERSION_C     4
#define WKPU_IP_IRQ_AR_RELEASE_REVISION_VERSION_C  0

#define WKPU_IP_IRQ_SW_MAJOR_VERSION_C  2
#define WKPU_IP_IRQ_SW_MINOR_VERSION_C  0
#define WKPU_IP_IRQ_SW_PATCH_VERSION_C  1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if header file and StandardTypes.h file are of the same Autosar version */
    #if ((WKPU_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C != STD_AR_RELEASE_MAJOR_VERSION) || \
         (WKPU_IP_IRQ_AR_RELEASE_MINOR_VERSION_C != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Wkpu_Ip_Irq.c and StandardTypes.h are different"
    #endif
    /* Check if this header file and SchM_Icu.h file are of the same Autosar version */
    #if ((WKPU_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C != SCHM_ICU_AR_RELEASE_MAJOR_VERSION) || \
        (WKPU_IP_IRQ_AR_RELEASE_MINOR_VERSION_C != SCHM_ICU_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Wkpu_Ip_Irq.c and SchM_Icu.h are different"
    #endif
#endif

/* Check if source file and ICU header file are of the same vendor */
#if (WKPU_IP_IRQ_VENDOR_ID_C != WKPU_IP_VENDOR_ID)
    #error "Wkpu_Ip_Irq.c and Wkpu_Ip.h have different vendor IDs"
#endif
/* Check if source file and ICU header file are of the same AutoSar version */
#if ((WKPU_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C  != WKPU_IP_AR_RELEASE_MAJOR_VERSION) || \
     (WKPU_IP_IRQ_AR_RELEASE_MINOR_VERSION_C  != WKPU_IP_AR_RELEASE_MINOR_VERSION) || \
     (WKPU_IP_IRQ_AR_RELEASE_REVISION_VERSION_C   != WKPU_IP_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Wkpu_Ip_Irq.c and Wkpu_Ip.h are different"
#endif
/* Check if source file and ICU header file are of the same Software version */
#if ((WKPU_IP_IRQ_SW_MAJOR_VERSION_C  != WKPU_IP_SW_MAJOR_VERSION) || \
     (WKPU_IP_IRQ_SW_MINOR_VERSION_C  != WKPU_IP_SW_MINOR_VERSION) || \
     (WKPU_IP_IRQ_SW_PATCH_VERSION_C  != WKPU_IP_SW_PATCH_VERSION))
#error "Software Version Numbers of Wkpu_Ip_Irq.c and Wkpu_Ip.h are different"
#endif

/* Check if source file and ICU header file are of the same vendor */
#if (WKPU_IP_IRQ_VENDOR_ID_C != WKPU_IP_IRQ_VENDOR_ID)
    #error "Wkpu_Ip_Irq.c and Wkpu_Ip_Irq.h have different vendor IDs"
#endif
/* Check if source file and ICU header file are of the same AutoSar version */
#if ((WKPU_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C  != WKPU_IP_IRQ_AR_RELEASE_MAJOR_VERSION) || \
     (WKPU_IP_IRQ_AR_RELEASE_MINOR_VERSION_C  != WKPU_IP_IRQ_AR_RELEASE_MINOR_VERSION) || \
     (WKPU_IP_IRQ_AR_RELEASE_REVISION_VERSION_C   != WKPU_IP_IRQ_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Wkpu_Ip_Irq.c and Wkpu_Ip_Irq.h are different"
#endif
/* Check if source file and ICU header file are of the same Software version */
#if ((WKPU_IP_IRQ_SW_MAJOR_VERSION_C  != WKPU_IP_IRQ_SW_MAJOR_VERSION) || \
     (WKPU_IP_IRQ_SW_MINOR_VERSION_C  != WKPU_IP_IRQ_SW_MINOR_VERSION) || \
     (WKPU_IP_IRQ_SW_PATCH_VERSION_C  != WKPU_IP_IRQ_SW_PATCH_VERSION))
#error "Software Version Numbers of Wkpu_Ip_Irq.c and Wkpu_Ip_Irq.h are different"
#endif

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/
#ifndef WKPU_IP_DERIVATIVE_NOT_SUPPORT_WKPU

#define ICU_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Icu_MemMap.h"
/* Table of initilized Wkpu channels */
#if !defined(WKPU_DERIVATIVE_SUPPORT_ONLY_NMI)
/* Table of initilized WKPU channels */
extern Wkpu_Ip_State Wkpu_Ip_u32ChState[WKPU_IP_NUM_OF_CHANNELS];
#endif /* WKPU_DERIVATIVE_SUPPORT_ONLY_NMI */
#define ICU_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Icu_MemMap.h"
/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#define ICU_START_SEC_CODE
#include "Icu_MemMap.h"

#if !defined(WKPU_DERIVATIVE_SUPPORT_ONLY_NMI)

#if ((defined(ICU_WKPU_SINGLE_INTERRUPT) && (ICU_WKPU_SINGLE_INTERRUPT == STD_ON)) || \
    ((defined ICU_WKPU_CH_0_ISR_USED) || (defined ICU_WKPU_CH_1_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_2_ISR_USED) || (defined ICU_WKPU_CH_3_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_4_ISR_USED) || (defined ICU_WKPU_CH_5_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_6_ISR_USED) || (defined ICU_WKPU_CH_7_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_8_ISR_USED) || (defined ICU_WKPU_CH_9_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_10_ISR_USED) || (defined ICU_WKPU_CH_11_ISR_USED) ||  \
     (defined ICU_WKPU_CH_12_ISR_USED) || (defined ICU_WKPU_CH_13_ISR_USED) ||  \
     (defined ICU_WKPU_CH_14_ISR_USED) || (defined ICU_WKPU_CH_15_ISR_USED) ||  \
     (defined ICU_WKPU_CH_16_ISR_USED) || (defined ICU_WKPU_CH_17_ISR_USED) ||  \
     (defined ICU_WKPU_CH_18_ISR_USED) || (defined ICU_WKPU_CH_19_ISR_USED) ||  \
     (defined ICU_WKPU_CH_20_ISR_USED) || (defined ICU_WKPU_CH_21_ISR_USED) ||  \
     (defined ICU_WKPU_CH_22_ISR_USED) || (defined ICU_WKPU_CH_23_ISR_USED) ||  \
     (defined ICU_WKPU_CH_24_ISR_USED) || (defined ICU_WKPU_CH_25_ISR_USED) ||  \
     (defined ICU_WKPU_CH_26_ISR_USED) || (defined ICU_WKPU_CH_27_ISR_USED) ||  \
     (defined ICU_WKPU_CH_28_ISR_USED) || (defined ICU_WKPU_CH_29_ISR_USED) ||  \
     (defined ICU_WKPU_CH_30_ISR_USED) || (defined ICU_WKPU_CH_31_ISR_USED)))
static inline void Wkpu_Ip_Callback(uint8 hwChannel);
#endif
#endif /* WKPU_DERIVATIVE_SUPPORT_ONLY_NMI */

#if (defined(ICU_WKPU_SINGLE_INTERRUPT) && (ICU_WKPU_SINGLE_INTERRUPT == STD_ON))

static inline void Wkpu_Icu_Ip_ProcessSingleInterrupt(void);

#else

#if ((defined ICU_WKPU_CH_0_ISR_USED) || (defined ICU_WKPU_CH_1_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_2_ISR_USED) || (defined ICU_WKPU_CH_3_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_4_ISR_USED) || (defined ICU_WKPU_CH_5_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_6_ISR_USED) || (defined ICU_WKPU_CH_7_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_8_ISR_USED) || (defined ICU_WKPU_CH_9_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_10_ISR_USED) || (defined ICU_WKPU_CH_11_ISR_USED) ||  \
     (defined ICU_WKPU_CH_12_ISR_USED) || (defined ICU_WKPU_CH_13_ISR_USED) ||  \
     (defined ICU_WKPU_CH_14_ISR_USED) || (defined ICU_WKPU_CH_15_ISR_USED) ||  \
     (defined ICU_WKPU_CH_16_ISR_USED) || (defined ICU_WKPU_CH_17_ISR_USED) ||  \
     (defined ICU_WKPU_CH_18_ISR_USED) || (defined ICU_WKPU_CH_19_ISR_USED) ||  \
     (defined ICU_WKPU_CH_20_ISR_USED) || (defined ICU_WKPU_CH_21_ISR_USED) ||  \
     (defined ICU_WKPU_CH_22_ISR_USED) || (defined ICU_WKPU_CH_23_ISR_USED) ||  \
     (defined ICU_WKPU_CH_24_ISR_USED) || (defined ICU_WKPU_CH_25_ISR_USED) ||  \
     (defined ICU_WKPU_CH_26_ISR_USED) || (defined ICU_WKPU_CH_27_ISR_USED) ||  \
     (defined ICU_WKPU_CH_28_ISR_USED) || (defined ICU_WKPU_CH_29_ISR_USED) ||  \
     (defined ICU_WKPU_CH_30_ISR_USED) || (defined ICU_WKPU_CH_31_ISR_USED))

void Wkpu_Icu_Ip_ProcessInterrupt(uint8 u8FirstHwChannel);

#endif
#endif /*defined ICU_WKPU_SINGLE_INTERRUPT*/


/*==================================================================================================
*                                        LOCAL FUNCTIONS
==================================================================================================*/

#if (defined(ICU_WKPU_SINGLE_INTERRUPT) && (ICU_WKPU_SINGLE_INTERRUPT == STD_ON))
/*==================================================================================================*/
/**
* @brief      Wkpu_Icu_Ip_ProcessSingleInterrupt
* @details
*
* @param[in]      u8FirstHwChannel - first IRQ HW Channel from the HW group
*
* @retval None
* @implements Wkpu_Ip_ProcessSingleInterrupt_Activity
*/
static inline void Wkpu_Icu_Ip_ProcessSingleInterrupt(void)
{
    uint8  u8WkpuChannel    = 0x0U;
    uint32 u32ChannelMask   = 0x1U;
    uint32 u32reg_WKPU_WISR = Wkpu_Ip_pBase[0U]->WISR;
    uint32 u32reg_WKPU_WIER = Wkpu_Ip_pBase[0U]->IRER;

    /* Clear pending interrupt serviced */
    Wkpu_Ip_pBase[0U]->WISR = u32reg_WKPU_WISR;
    
    /* Select which channels will be serviced - only the enabled irq ones*/
    u32reg_WKPU_WISR &= u32reg_WKPU_WIER;

    for (u8WkpuChannel = 0; u8WkpuChannel < (uint32)WKPU_IP_NUM_OF_CHANNELS_IN_ONE_REG; u8WkpuChannel++)
    {
        if (0x0U != (u32reg_WKPU_WISR & u32ChannelMask))
        {
            Wkpu_Ip_Callback(u8WkpuChannel);
        }
        u32ChannelMask <<= (uint32)1U;
    }

#ifdef WKPU_IP_64_CH_USED
    /*Get  WKPU32 - WKPU63 interrupt status flag*/
    u8WkpuChannel = 0x0U;
    u32ChannelMask   = 0x1U;
    u32reg_WKPU_WISR = Wkpu_Ip_pBase[0U]->WISR_64;
    u32reg_WKPU_WIER = Wkpu_Ip_pBase[0U]->IRER_64;
    
    /* Clear pending interrupt serviced */
    Wkpu_Ip_pBase[0U]->WISR_64 = u32reg_WKPU_WISR;
    /* Select which channels will be serviced - only the enabled irq ones*/
    u32reg_WKPU_WISR &= u32reg_WKPU_WIER;

    for (u8WkpuChannel = 0; u8WkpuChannel < (uint32)WKPU_IP_NUM_OF_CHANNELS_IN_ONE_REG; u8WkpuChannel++)
    {
        if (0x0U != (u32reg_WKPU_WISR & u32ChannelMask))
        {
            Wkpu_Ip_Callback(u8WkpuChannel+ 32U);
        }
        u32ChannelMask <<= (uint32)1U;
    }
#endif
}

#else

#if ((defined ICU_WKPU_CH_0_ISR_USED) || (defined ICU_WKPU_CH_1_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_2_ISR_USED) || (defined ICU_WKPU_CH_3_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_4_ISR_USED) || (defined ICU_WKPU_CH_5_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_6_ISR_USED) || (defined ICU_WKPU_CH_7_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_8_ISR_USED) || (defined ICU_WKPU_CH_9_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_10_ISR_USED) || (defined ICU_WKPU_CH_11_ISR_USED) ||  \
     (defined ICU_WKPU_CH_12_ISR_USED) || (defined ICU_WKPU_CH_13_ISR_USED) ||  \
     (defined ICU_WKPU_CH_14_ISR_USED) || (defined ICU_WKPU_CH_15_ISR_USED) ||  \
     (defined ICU_WKPU_CH_16_ISR_USED) || (defined ICU_WKPU_CH_17_ISR_USED) ||  \
     (defined ICU_WKPU_CH_18_ISR_USED) || (defined ICU_WKPU_CH_19_ISR_USED) ||  \
     (defined ICU_WKPU_CH_20_ISR_USED) || (defined ICU_WKPU_CH_21_ISR_USED) ||  \
     (defined ICU_WKPU_CH_22_ISR_USED) || (defined ICU_WKPU_CH_23_ISR_USED) ||  \
     (defined ICU_WKPU_CH_24_ISR_USED) || (defined ICU_WKPU_CH_25_ISR_USED) ||  \
     (defined ICU_WKPU_CH_26_ISR_USED) || (defined ICU_WKPU_CH_27_ISR_USED) ||  \
     (defined ICU_WKPU_CH_28_ISR_USED) || (defined ICU_WKPU_CH_29_ISR_USED) ||  \
     (defined ICU_WKPU_CH_30_ISR_USED) || (defined ICU_WKPU_CH_31_ISR_USED))
/*==================================================================================================*/
/**
* @brief      Wkpu_Icu_Ip_ProcessInterrupt
* @details
*
* @param[in]      u8FirstHwChannel - first IRQ HW Channel from the HW group
*
* @retval None
* @implements Wkpu_Ip_ProcessInterrupt_Activity
*/
void Wkpu_Icu_Ip_ProcessInterrupt(uint8 FirstHwChannel)
{
    uint8  u8WkpuChannel    = FirstHwChannel;
    uint32 u32ChannelMask   = 1U << FirstHwChannel;
    uint32 u32RegMask       = 0xFFU;
    uint32 u32reg_WKPU_WISR = Wkpu_Ip_pBase[0U]->WISR;
    uint32 u32reg_WKPU_WIER = Wkpu_Ip_pBase[0U]->IRER;

    /* Clear pending interrupt serviced */
    Wkpu_Ip_pBase[0U]->WISR = (u32reg_WKPU_WISR & (u32RegMask << FirstHwChannel));
    
    /* Select which channels will be serviced - only the enabled irq ones*/
    u32reg_WKPU_WISR = u32reg_WKPU_WISR & (u32RegMask << FirstHwChannel) & u32reg_WKPU_WIER;

    for (u8WkpuChannel = FirstHwChannel; u8WkpuChannel < (FirstHwChannel + 8U); u8WkpuChannel++)
    {
        if (0x0U != (u32reg_WKPU_WISR & u32ChannelMask))
        {
            Wkpu_Ip_Callback(u8WkpuChannel);
        }
        u32ChannelMask <<= (uint32)1U;
    }
}
#endif
#endif /*defined ICU_WKPU_SINGLE_INTERRUPT*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/

#if (defined(ICU_WKPU_SINGLE_INTERRUPT) && (ICU_WKPU_SINGLE_INTERRUPT == STD_ON))
/*==================================================================================================*/
/**
* @brief     Interrupt handler for WKPU channels : 0 to 31
* @details   Process the interrupt of WKPU channels 0 to 31 for platforms with only one interrupt line
*
* @remarks   This will be defined only if any of WKPU channels 0 to 31 are configured
*/
/*==================================================================================================*/
ISR(WKPU_EXT_IRQ_SINGLE_ISR)
{
    Wkpu_Icu_Ip_ProcessSingleInterrupt();
}
#else

#if ((defined ICU_WKPU_CH_0_ISR_USED) || (defined ICU_WKPU_CH_1_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_2_ISR_USED) || (defined ICU_WKPU_CH_3_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_4_ISR_USED) || (defined ICU_WKPU_CH_5_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_6_ISR_USED) || (defined ICU_WKPU_CH_7_ISR_USED))
/*==================================================================================================*/
/**
* @brief     Interrupt handler for WKPU channels : 0 to 7
* @details   Process the interrupt of WKPU channels 0 to 7
*
* @remarks   This will be defined only if any of WKPU channels 0 to 7 is configured
*/
/*==================================================================================================*/
ISR(WKPU_EXT_IRQ_0_7_ISR)
{
    Wkpu_Icu_Ip_ProcessInterrupt(0U);
}
#endif  /* WKPU IRQ 0 - 7 */


#if ((defined ICU_WKPU_CH_8_ISR_USED) || (defined ICU_WKPU_CH_9_ISR_USED)   ||  \
     (defined ICU_WKPU_CH_10_ISR_USED) || (defined ICU_WKPU_CH_11_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_12_ISR_USED) || (defined ICU_WKPU_CH_13_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_14_ISR_USED) || (defined ICU_WKPU_CH_15_ISR_USED))
/*==================================================================================================*/
/**
* @brief     Interrupt handler for WKPU channels : 8 to 15
* @details   Process the interrupt of WKPU channels 8 to 15
*
* @remarks   This will be defined only if any of WKPU channels 8 to 15 is configured
*/
/*==================================================================================================*/
ISR(WKPU_EXT_IRQ_8_15_ISR)
{
    Wkpu_Icu_Ip_ProcessInterrupt(ICU_WKPU_8);
}
#endif  /* WKPU IRQ 8 - 15 */


#if ((defined ICU_WKPU_CH_16_ISR_USED) || (defined ICU_WKPU_CH_17_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_18_ISR_USED) || (defined ICU_WKPU_CH_19_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_20_ISR_USED) || (defined ICU_WKPU_CH_21_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_22_ISR_USED) || (defined ICU_WKPU_CH_23_ISR_USED))
/*==================================================================================================*/
/**
* @brief     Interrupt handler for WKPU channels : 16 to 23
* @details   Process the interrupt of WKPU channels 16 to 23
*
* @remarks   This will be defined only if any of WKPU channels 16 to 23 is configured
*/
/*==================================================================================================*/
ISR(WKPU_EXT_IRQ_16_23_ISR)
{
    Wkpu_Icu_Ip_ProcessInterrupt(ICU_WKPU_16);
}
#endif   /* WKPU IRQ 16 - 23 */


#if ((defined ICU_WKPU_CH_24_ISR_USED) || (defined ICU_WKPU_CH_25_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_26_ISR_USED) || (defined ICU_WKPU_CH_27_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_28_ISR_USED) || (defined ICU_WKPU_CH_29_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_30_ISR_USED) || (defined ICU_WKPU_CH_31_ISR_USED))
/*==================================================================================================*/
/**
* @brief     Interrupt handler for WKPU channels : 24 to 31
* @details   Process the interrupt of WKPU channels 24 to 31
*
* @remarks   This will be defined only if any of WKPU channels 24 to 31 is configured
*/
/*==================================================================================================*/
ISR(WKPU_EXT_IRQ_24_31_ISR)
{
    Wkpu_Icu_Ip_ProcessInterrupt(ICU_WKPU_24);
}
#endif   /* WKPU IRQ 24 - 31 */
#endif   /* defined ICU_WKPU_SINGLE_INTERRUPT */

#if !defined(WKPU_DERIVATIVE_SUPPORT_ONLY_NMI)

#if ((defined(ICU_WKPU_SINGLE_INTERRUPT) && (ICU_WKPU_SINGLE_INTERRUPT == STD_ON)) || \
    ((defined ICU_WKPU_CH_0_ISR_USED) || (defined ICU_WKPU_CH_1_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_2_ISR_USED) || (defined ICU_WKPU_CH_3_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_4_ISR_USED) || (defined ICU_WKPU_CH_5_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_6_ISR_USED) || (defined ICU_WKPU_CH_7_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_8_ISR_USED) || (defined ICU_WKPU_CH_9_ISR_USED)  ||  \
     (defined ICU_WKPU_CH_10_ISR_USED) || (defined ICU_WKPU_CH_11_ISR_USED) ||  \
     (defined ICU_WKPU_CH_12_ISR_USED) || (defined ICU_WKPU_CH_13_ISR_USED) ||  \
     (defined ICU_WKPU_CH_14_ISR_USED) || (defined ICU_WKPU_CH_15_ISR_USED) ||  \
     (defined ICU_WKPU_CH_16_ISR_USED) || (defined ICU_WKPU_CH_17_ISR_USED) ||  \
     (defined ICU_WKPU_CH_18_ISR_USED) || (defined ICU_WKPU_CH_19_ISR_USED) ||  \
     (defined ICU_WKPU_CH_20_ISR_USED) || (defined ICU_WKPU_CH_21_ISR_USED) ||  \
     (defined ICU_WKPU_CH_22_ISR_USED) || (defined ICU_WKPU_CH_23_ISR_USED) ||  \
     (defined ICU_WKPU_CH_24_ISR_USED) || (defined ICU_WKPU_CH_25_ISR_USED) ||  \
     (defined ICU_WKPU_CH_26_ISR_USED) || (defined ICU_WKPU_CH_27_ISR_USED) ||  \
     (defined ICU_WKPU_CH_28_ISR_USED) || (defined ICU_WKPU_CH_29_ISR_USED) ||  \
     (defined ICU_WKPU_CH_30_ISR_USED) || (defined ICU_WKPU_CH_31_ISR_USED)))
static inline void Wkpu_Ip_Callback(uint8 hwChannel)
{
    if(((boolean)TRUE == Wkpu_Ip_u32ChState[hwChannel].chInit) && \
       (NULL_PTR != Wkpu_Ip_u32ChState[hwChannel].callback))
    {
        Wkpu_Ip_u32ChState[hwChannel].callback(Wkpu_Ip_u32ChState[hwChannel].callbackParam, FALSE);
    }
    else
    {
        /* Calling Notification for the IPL channel */
        if (((boolean)TRUE == Wkpu_Ip_u32ChState[hwChannel].chInit) && \
            (NULL_PTR != Wkpu_Ip_u32ChState[(uint8)hwChannel].WkpuChannelNotification) && \
            ((boolean)TRUE == Wkpu_Ip_u32ChState[hwChannel].notificationEnable))
        {
            Wkpu_Ip_u32ChState[(uint8)hwChannel].WkpuChannelNotification();
        }
    }
}
#endif
#endif /* WKPU_DERIVATIVE_SUPPORT_ONLY_NMI */

#define ICU_STOP_SEC_CODE
#include "Icu_MemMap.h"

#endif /* WKPU_IP_DERIVATIVE_NOT_SUPPORT_WKPU */

#ifdef __cplusplus
}
#endif

/** @} */

