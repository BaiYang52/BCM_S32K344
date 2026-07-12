/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Emios Flexio
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
*   @file       Emios_Pwm_Ip_Irq.c
*
*   @addtogroup emios_pwm_ip Emios Pwm IPL
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*==================================================================================================
*                                          INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Emios_Pwm_Ip.h"
#include "Emios_Pwm_Ip_HwAccess.h"
#include "Emios_Pwm_Ip_Irq.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define EMIOS_PWM_IP_IRQ_VENDOR_ID_C                      43
#define EMIOS_PWM_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C       4
#define EMIOS_PWM_IP_IRQ_AR_RELEASE_MINOR_VERSION_C       4
#define EMIOS_PWM_IP_IRQ_AR_RELEASE_REVISION_VERSION_C    0
#define EMIOS_PWM_IP_IRQ_SW_MAJOR_VERSION_C               2
#define EMIOS_PWM_IP_IRQ_SW_MINOR_VERSION_C               0
#define EMIOS_PWM_IP_IRQ_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
/* Check if source file and Emios_Pwm_Ip.h file are of the same vendor */
#if (EMIOS_PWM_IP_IRQ_VENDOR_ID_C != EMIOS_PWM_IP_VENDOR_ID)
    #error "Vendor IDs of Emios_Pwm_Ip_Irq.c and Emios_Pwm_Ip.h are different."
#endif

/* Check if source file and Emios_Pwm_Ip.h file are of the same AUTOSAR version */
#if ((EMIOS_PWM_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C    != EMIOS_PWM_IP_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_PWM_IP_IRQ_AR_RELEASE_MINOR_VERSION_C    != EMIOS_PWM_IP_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_PWM_IP_IRQ_AR_RELEASE_REVISION_VERSION_C != EMIOS_PWM_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR version numbers of Emios_Pwm_Ip_Irq.c and Emios_Pwm_Ip.h are different."
#endif

/* Check if source file and Emios_Pwm_Ip.h file are of the same Software version */
#if ((EMIOS_PWM_IP_IRQ_SW_MAJOR_VERSION_C != EMIOS_PWM_IP_SW_MAJOR_VERSION)  || \
     (EMIOS_PWM_IP_IRQ_SW_MINOR_VERSION_C != EMIOS_PWM_IP_SW_MINOR_VERSION)  || \
     (EMIOS_PWM_IP_IRQ_SW_PATCH_VERSION_C != EMIOS_PWM_IP_SW_PATCH_VERSION))
    #error "Software version numbers of Emios_Pwm_Ip_Irq.c and Emios_Pwm_Ip.h are different."
#endif

/* Check if source file and Emios_Pwm_Ip_HwAccess.h file are of the same vendor */
#if (EMIOS_PWM_IP_IRQ_VENDOR_ID_C != EMIOS_PWM_IP_HWACCESS_VENDOR_ID)
    #error "Vendor IDs of Emios_Pwm_Ip_Irq.c and Emios_Pwm_Ip_HwAccess.h are different."
#endif

/* Check if source file and Emios_Pwm_Ip_HwAccess.h file are of the same AUTOSAR version */
#if ((EMIOS_PWM_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C    != EMIOS_PWM_IP_HWACCESS_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_PWM_IP_IRQ_AR_RELEASE_MINOR_VERSION_C    != EMIOS_PWM_IP_HWACCESS_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_PWM_IP_IRQ_AR_RELEASE_REVISION_VERSION_C != EMIOS_PWM_IP_HWACCESS_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR version numbers of Emios_Pwm_Ip_Irq.c and Emios_Pwm_Ip_HwAccess.h are different."
#endif

/* Check if source file and Emios_Pwm_Ip_HwAccess.h file are of the same Software version */
#if ((EMIOS_PWM_IP_IRQ_SW_MAJOR_VERSION_C != EMIOS_PWM_IP_HWACCESS_SW_MAJOR_VERSION)  || \
     (EMIOS_PWM_IP_IRQ_SW_MINOR_VERSION_C != EMIOS_PWM_IP_HWACCESS_SW_MINOR_VERSION)  || \
     (EMIOS_PWM_IP_IRQ_SW_PATCH_VERSION_C != EMIOS_PWM_IP_HWACCESS_SW_PATCH_VERSION))
    #error "Software version numbers of Emios_Pwm_Ip_Irq.c and Emios_Pwm_Ip_HwAccess.h are different."
#endif

/* Check if source file and Emios_Pwm_Ip_Irq.h file are of the same vendor */
#if (EMIOS_PWM_IP_IRQ_VENDOR_ID_C != EMIOS_PWM_IP_IRQ_VENDOR_ID)
    #error "Vendor IDs of Emios_Pwm_Ip_Irq.c and Emios_Pwm_Ip_Irq.h are different."
#endif

/* Check if source file and Emios_Pwm_Ip_Irq.h file are of the same AUTOSAR version */
#if ((EMIOS_PWM_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C    != EMIOS_PWM_IP_IRQ_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_PWM_IP_IRQ_AR_RELEASE_MINOR_VERSION_C    != EMIOS_PWM_IP_IRQ_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_PWM_IP_IRQ_AR_RELEASE_REVISION_VERSION_C != EMIOS_PWM_IP_IRQ_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR version numbers of Emios_Pwm_Ip_Irq.c and Emios_Pwm_Ip_Irq.h are different."
#endif

/* Check if source file and Emios_Pwm_Ip_Irq.h file are of the same Software version */
#if ((EMIOS_PWM_IP_IRQ_SW_MAJOR_VERSION_C != EMIOS_PWM_IP_IRQ_SW_MAJOR_VERSION)  || \
     (EMIOS_PWM_IP_IRQ_SW_MINOR_VERSION_C != EMIOS_PWM_IP_IRQ_SW_MINOR_VERSION)  || \
     (EMIOS_PWM_IP_IRQ_SW_PATCH_VERSION_C != EMIOS_PWM_IP_IRQ_SW_PATCH_VERSION))
    #error "Software version numbers of Emios_Pwm_Ip_Irq.c and Emios_Pwm_Ip_Irq.h are different."
#endif

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/
#define PWM_START_SEC_CONST_UNSPECIFIED
#include "Pwm_MemMap.h"

/** @brief Array with base addresses for Emios instances available on platform */
extern Emios_Pwm_Ip_HwAddrType *const Emios_Pwm_Ip_aBasePtr[EMIOS_PWM_IP_INSTANCE_COUNT];

#define PWM_STOP_SEC_CONST_UNSPECIFIED
#include "Pwm_MemMap.h"

#define PWM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Pwm_MemMap.h"

/** @brief Array with notification handlers for all configurable channels */
extern Emios_Pwm_Ip_NotificationType const *Emios_Pwm_Ip_aNotificationPtr[EMIOS_PWM_IP_INSTANCE_COUNT][EMIOS_PWM_IP_CHANNEL_COUNT];

#define PWM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Pwm_MemMap.h"

#define PWM_START_SEC_VAR_INIT_UNSPECIFIED
#include "Pwm_MemMap.h"

/* Array with current pwm modes for each Emios Channel */
extern Emios_Pwm_Ip_PwmModeType Emios_Pwm_Ip_aCurrentModes[EMIOS_PWM_IP_INSTANCE_COUNT][EMIOS_PWM_IP_CHANNEL_COUNT];

#define PWM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "Pwm_MemMap.h"

#define PWM_START_SEC_VAR_CLEARED_16
#include "Pwm_MemMap.h"

/** @brief Array with period for all channels */
extern uint16 Emios_Pwm_Ip_aPeriod[EMIOS_PWM_IP_INSTANCE_COUNT][EMIOS_PWM_IP_CHANNEL_COUNT];

/** @brief Array with duty cycle for all channels */
extern uint16 Emios_Pwm_Ip_aDutyCycle[EMIOS_PWM_IP_INSTANCE_COUNT][EMIOS_PWM_IP_CHANNEL_COUNT];

#define PWM_STOP_SEC_VAR_CLEARED_16
#include "Pwm_MemMap.h"

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/


/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
#define PWM_START_SEC_CODE
#include "Pwm_MemMap.h"

static void Emios_Pwm_Ip_IrqDaocHandler(uint8 Instance, uint8 Channel)
{

    uint16 DaocDuty = 0U;
    uint16 DaocRegA = 0U;
    uint16 DaocRegB = 0U;
    uint16 CounterMax = Emios_Pwm_Ip_GetUCRegA(Emios_Pwm_Ip_aBasePtr[Instance], Emios_Pwm_Ip_GetMasterBusChannel(Instance, Channel));

    Emios_Pwm_Ip_PolarityType Polarity = Emios_Pwm_Ip_GetEdgePolarity(Emios_Pwm_Ip_aBasePtr[Instance], Channel);
    boolean OutputPin = Emios_Pwm_Ip_GetOutputPinState(Emios_Pwm_Ip_aBasePtr[Instance], Channel);

    if (EMIOS_PWM_IP_MODE_DAOC_FLAG == Emios_Pwm_Ip_aCurrentModes[Instance][Channel])
    {
        DaocDuty = Emios_Pwm_Ip_aPeriod[Instance][Channel] - Emios_Pwm_Ip_aDutyCycle[Instance][Channel];
        DaocRegA = ((DaocDuty + Emios_Pwm_Ip_GetUCRegB(Emios_Pwm_Ip_aBasePtr[Instance], Channel)) % CounterMax);

        Emios_Pwm_Ip_SetUCRegA(Emios_Pwm_Ip_aBasePtr[Instance], Channel, (DaocRegA == 0U)? CounterMax : DaocRegA);

        DaocRegB = (Emios_Pwm_Ip_aPeriod[Instance][Channel] + Emios_Pwm_Ip_GetUCRegB(Emios_Pwm_Ip_aBasePtr[Instance], Channel)) % CounterMax;
        Emios_Pwm_Ip_SetUCRegB(Emios_Pwm_Ip_aBasePtr[Instance], Channel, (DaocRegB == 0U)? CounterMax : DaocRegB);

    }
    else if (EMIOS_PWM_IP_MODE_DAOC_FLAG_BOTH == Emios_Pwm_Ip_aCurrentModes[Instance][Channel])
    {
        if (((EMIOS_PWM_IP_ACTIVE_HIGH == Polarity) && (FALSE == OutputPin)) ||
            ((EMIOS_PWM_IP_ACTIVE_LOW == Polarity) && (TRUE == OutputPin)))
        {
            DaocRegA = (Emios_Pwm_Ip_aPeriod[Instance][Channel] + Emios_Pwm_Ip_GetUCRegA(Emios_Pwm_Ip_aBasePtr[Instance], Channel)) % CounterMax;
            Emios_Pwm_Ip_SetUCRegA(Emios_Pwm_Ip_aBasePtr[Instance], Channel, (DaocRegA == 0U)? CounterMax : DaocRegA);      
        }
        else
        {
            DaocRegB = (Emios_Pwm_Ip_aDutyCycle[Instance][Channel] + Emios_Pwm_Ip_GetUCRegA(Emios_Pwm_Ip_aBasePtr[Instance], Channel)) % CounterMax;
            Emios_Pwm_Ip_SetUCRegB(Emios_Pwm_Ip_aBasePtr[Instance], Channel, (DaocRegB == 0U)? CounterMax : DaocRegB);
        }
    }
    else
    {
        /* Do nothing */
    }
    (void)DaocDuty;
    (void)DaocRegA;
    (void)DaocRegB;
    (void)Polarity;
    (void)OutputPin;
}

/**
* @brief          Interrupt handler for Emios Pwm channels.
* @details        Interrupt handler that clears the flags and calls the user notification function.
*
* @param[in]      Instance    Emios Instance id on which the interrupt occured.
* @param[in]      Channel     Channel id within the emios Instance that triggered the interrupt.
* @implements Emios_Pwm_Ip_IrqHandler_Activity
*/
void Emios_Pwm_Ip_IrqHandler(uint8 Instance, uint8 Channel)
{
    /* Clear Interrupt flag */
    Emios_Pwm_Ip_ClearFlagEvent(Emios_Pwm_Ip_aBasePtr[Instance], Channel);

    Emios_Pwm_Ip_IrqDaocHandler(Instance, Channel);

    if (NULL_PTR != Emios_Pwm_Ip_aNotificationPtr[Instance][Channel]->CbFunction)
    {
        /* Call the user notification callback with the provided parameter */
        (Emios_Pwm_Ip_aNotificationPtr[Instance][Channel]->CbFunction)(Emios_Pwm_Ip_aNotificationPtr[Instance][Channel]->CbParameter);
    }
}

#define PWM_STOP_SEC_CODE
#include "Pwm_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
