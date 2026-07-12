[!CODE!][!//
[!AUTOSPACING!][!//
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
 *   @file    Icu_DmaNotification.c
 *   @version 2.0.1
 *
 *   @brief   AUTOSAR Icu - contains the data exported by the Icu module
 *   @details Contains the information that will be exported by the module, as requested by Autosar.
 *
 *   @addtogroup icu Icu Driver
 *   @{
 */
 
#ifdef __cplusplus
extern "C"{
#endif

 /*==================================================================================================
 *                                         INCLUDE FILES
 * 1) system and project includes
 * 2) needed interfaces from external units
 * 3) internal and external interfaces from this unit
 *==================================================================================================*/
#include "Icu.h"

/*==================================================================================================
 *                              SOURCE FILE VERSION INFORMATION
 *==================================================================================================*/
#define ICU_VENDOR_ID_DMANOTIF_C                    43
#define ICU_AR_RELEASE_MAJOR_VERSION_DMANOTIF_C     4
#define ICU_AR_RELEASE_MINOR_VERSION_DMANOTIF_C     4
#define ICU_AR_RELEASE_REVISION_VERSION_DMANOTIF_C  0
#define ICU_SW_MAJOR_VERSION_DMANOTIF_C             2
#define ICU_SW_MINOR_VERSION_DMANOTIF_C             0
#define ICU_SW_PATCH_VERSION_DMANOTIF_C             1

/*==================================================================================================
 *                                      FILE VERSION CHECKS
 *==================================================================================================*/
/* Check Icu_DmaNotification.c against Icu.h file versions */
#if (ICU_VENDOR_ID_DMANOTIF_C != ICU_VENDOR_ID)
    #error "Icu_DmaNotification.c and Icu.h have different vendor IDs!"
#endif

#if ((ICU_AR_RELEASE_MAJOR_VERSION_DMANOTIF_C != ICU_AR_RELEASE_MAJOR_VERSION) || \
     (ICU_AR_RELEASE_MINOR_VERSION_DMANOTIF_C != ICU_AR_RELEASE_MINOR_VERSION) || \
     (ICU_AR_RELEASE_REVISION_VERSION_DMANOTIF_C != ICU_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Icu_DmaNotification.c and Icu.h are different"
#endif

#if ((ICU_SW_MAJOR_VERSION_DMANOTIF_C != ICU_SW_MAJOR_VERSION) || \
     (ICU_SW_MINOR_VERSION_DMANOTIF_C != ICU_SW_MINOR_VERSION) || \
     (ICU_SW_PATCH_VERSION_DMANOTIF_C != ICU_SW_PATCH_VERSION))
    #error "Software Version Numbers of Icu_DmaNotification.c  and Icu.h are different"
#endif

/* Check Icu_DmaNotification.c against Icu_Irq.h file versions */
#if (ICU_VENDOR_ID_DMANOTIF_C != ICU_IRQ_VENDOR_ID)
    #error "Icu_DmaNotification.c and Icu_Irq.h have different vendor IDs"
#endif

#if ((ICU_AR_RELEASE_MAJOR_VERSION_DMANOTIF_C != ICU_IRQ_AR_RELEASE_MAJOR_VERSION) || \
     (ICU_AR_RELEASE_MINOR_VERSION_DMANOTIF_C != ICU_IRQ_AR_RELEASE_MINOR_VERSION) || \
     (ICU_AR_RELEASE_REVISION_VERSION_DMANOTIF_C != ICU_IRQ_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Icu_DmaNotification.c and Icu_Irq.h are different"
#endif

#if ((ICU_SW_MAJOR_VERSION_DMANOTIF_C != ICU_IRQ_SW_MAJOR_VERSION) || \
     (ICU_SW_MINOR_VERSION_DMANOTIF_C != ICU_IRQ_SW_MINOR_VERSION) || \
     (ICU_SW_PATCH_VERSION_DMANOTIF_C != ICU_IRQ_SW_PATCH_VERSION))
    #error "Software Version Numbers of Icu_DmaNotification.c  and Icu_Irq.h are different"
#endif

/*==================================================================================================
 *                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
 *==================================================================================================*/

/*==================================================================================================
 *                                       GLOBAL FUNCTIONS
 *==================================================================================================*/
#define ICU_START_SEC_CODE
#include "Icu_MemMap.h"

#if ((ICU_SIGNALMEASUREMENT_USES_DMA == STD_ON) || (ICU_TIMESTAMP_USES_DMA == STD_ON))
[!NOCODE!][!//
    [!VAR "OuterLoopCounter" = "0"!][!//
    [!VAR "InnerLoopCounter" = "0"!][!//
    [!VAR "found" = "'false'"!][!//
    [!SELECT "IcuConfigSet"!][!//
        [!LOOP "./IcuChannel/*"!][!//
        [!IF "IcuDMAChannelEnable='true'"!][!//
            [!VAR "Matchcounter" = "0"!][!//
            [!IF "node:exists(IcuDMAChannelRef)"!][!//
                [!VAR "OuterLoopCounter" = "$OuterLoopCounter + 1"!][!//
                [!VAR "InnerLoopCounter" = "0"!][!//
                [!VAR "ChRef" = "IcuDMAChannelRef"!][!//
                [!LOOP "../../IcuChannel/*"!][!//All channels
                    [!IF "IcuDMAChannelEnable='true' and node:exists(IcuDMAChannelRef)"!][!//
                        [!VAR "InnerLoopCounter" = "$InnerLoopCounter + 1"!][!//
                        [!IF "$OuterLoopCounter >= $InnerLoopCounter"!][!//
                            [!IF "($ChRef = IcuDMAChannelRef)"!][!//
                                [!VAR "Matchcounter" = "$Matchcounter + 1"!][!//
                            [!ENDIF!][!//
                        [!ENDIF!][!//
                    [!ENDIF!][!//
                [!ENDLOOP!][!//
            [!ENDIF!][!//
            [!IF "$Matchcounter = 1"!][!//
                [!IF "$found = 'false'"!][!//
                    [!VAR "found" = "'true'"!][!//
                [!ENDIF!][!//
                [!IF "IcuMeasurementMode = 'ICU_MODE_TIMESTAMP'"!][!//
                    [!CODE!][!WS "0"!]/** @brief DMA notification handlers for [!"node:name(.)"!]. */[!CR!][!ENDCODE!][!//
                    [!CODE!][!WS "0"!]extern void [!"node:ref(IcuDMAChannelRef)/dmaLogicChannel_InterruptCallback"!](void)[!CR!][!ENDCODE!][!//
                    [!CODE!][!WS "0"!]{[!CR!][!ENDCODE!][!//
                    [!CODE!][!WS "4"!]Icu_TimestampDmaProcessing([!"node:name(.)"!]);[!CR!][!ENDCODE!][!//
                    [!CODE!][!WS "0"!][!CR!]}[!CR!][!ENDCODE!][!//
                [!ENDIF!][!//
                [!IF "IcuMeasurementMode = 'ICU_MODE_SIGNAL_MEASUREMENT'"!][!//
                    [!CODE!][!WS "0"!]/** @brief DMA notification handlers for [!"node:name(.)"!]. */[!CR!][!ENDCODE!][!//
                    [!CODE!][!WS "0"!]extern void [!"node:ref(IcuDMAChannelRef)/dmaLogicChannel_InterruptCallback"!](void)[!CR!][!ENDCODE!][!//
                    [!CODE!][!WS "0"!]{[!CR!][!ENDCODE!][!//
                    [!CODE!][!WS "4"!]Icu_SignalMeasurementDmaProcessing([!"node:name(.)"!]);[!CR!][!ENDCODE!][!//
                    [!CODE!][!WS "0"!][!CR!]}[!CR!][!ENDCODE!][!//
                [!ENDIF!][!//
             [!ENDIF!][!//
        [!ENDIF!][!//
        [!ENDLOOP!][!//
    [!ENDSELECT!][!//
[!ENDNOCODE!][!//
#endif /* ((ICU_SIGNALMEASUREMENT_USES_DMA == STD_ON) || (ICU_TIMESTAMP_USES_DMA == STD_ON)) */

#define ICU_STOP_SEC_CODE
#include "Icu_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
[!ENDCODE!][!//
