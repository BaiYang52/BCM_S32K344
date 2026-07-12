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
 *     @file       File with source code used to implement ICU driver functionality on EMIOS module.
 *     @details    This file contains the source code for all functions which are using EMIOS module.
 *     @addtogroup emios_icu_ip EMIOS IPL
 *     @{
 */

#ifdef __cplusplus
extern "C"{
#endif

/*==================================================================================================
*                                         INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/
#include "Emios_Icu_Ip.h"
#include "Emios_Icu_Ip_Irq.h"
#include "SchM_Icu.h"

#if (EMIOS_ICU_IP_DEV_ERROR_DETECT == STD_ON)
    #include "Devassert.h"
#endif

/*==================================================================================================
*                                        LOCAL MACROS
==================================================================================================*/
#define EMIOS_ICU_IP_IRQ_VENDOR_ID_C                       43
#define EMIOS_ICU_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C        4
#define EMIOS_ICU_IP_IRQ_AR_RELEASE_MINOR_VERSION_C        4
#define EMIOS_ICU_IP_IRQ_AR_RELEASE_REVISION_VERSION_C     0
#define EMIOS_ICU_IP_IRQ_SW_MAJOR_VERSION_C                2
#define EMIOS_ICU_IP_IRQ_SW_MINOR_VERSION_C                0
#define EMIOS_ICU_IP_IRQ_SW_PATCH_VERSION_C                1

/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
#if (EMIOS_ICU_IP_IRQ_VENDOR_ID_C != EMIOS_ICU_IP_IRQ_VENDOR_ID)
    #error "Emios_Icu_Ip_Irq.c and Emios_Icu_Ip_Irq.h have different vendor ids"
#endif

/* Check if  header file and Emios_Icu_Ip_Irq.h file are of the same Autosar version */
#if ((EMIOS_ICU_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C != EMIOS_ICU_IP_IRQ_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_AR_RELEASE_MINOR_VERSION_C != EMIOS_ICU_IP_IRQ_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_AR_RELEASE_REVISION_VERSION_C != EMIOS_ICU_IP_IRQ_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Emios_Icu_Ip_Irq.c and Emios_Icu_Ip_Irq.h are different"
#endif

/* Check if header file and Emios_Icu_Ip_Irq.h file are of the same Software version */
#if ((EMIOS_ICU_IP_IRQ_SW_MAJOR_VERSION_C != EMIOS_ICU_IP_IRQ_SW_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_SW_MINOR_VERSION_C != EMIOS_ICU_IP_IRQ_SW_MINOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_SW_PATCH_VERSION_C != EMIOS_ICU_IP_IRQ_SW_PATCH_VERSION))
    #error "Software Version Numbers of Emios_Icu_Ip_Irq.c and Emios_Icu_Ip_Irq.h are different"
#endif

#if (EMIOS_ICU_IP_IRQ_VENDOR_ID_C != EMIOS_ICU_IP_VENDOR_ID)
    #error "Emios_Icu_Ip_Irq.c and Emios_Icu_Ip.h have different vendor ids"
#endif

/* Check if  header file and Emios_Icu_Ip.h file are of the same Autosar version */
#if ((EMIOS_ICU_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C != EMIOS_ICU_IP_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_AR_RELEASE_MINOR_VERSION_C != EMIOS_ICU_IP_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_AR_RELEASE_REVISION_VERSION_C != EMIOS_ICU_IP_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Emios_Icu_Ip_Irq.c and Emios_Icu_Ip.h are different"
#endif

/* Check if header file and Emios_Icu_Ip.h file are of the same Software version */
#if ((EMIOS_ICU_IP_IRQ_SW_MAJOR_VERSION_C != EMIOS_ICU_IP_SW_MAJOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_SW_MINOR_VERSION_C != EMIOS_ICU_IP_SW_MINOR_VERSION) || \
     (EMIOS_ICU_IP_IRQ_SW_PATCH_VERSION_C != EMIOS_ICU_IP_SW_PATCH_VERSION))
    #error "Software Version Numbers of Emios_Icu_Ip_Irq.c and Emios_Icu_Ip.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if this header file and SchM_Icu.h file are of the same Autosar version */
    #if ((EMIOS_ICU_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C != SCHM_ICU_AR_RELEASE_MAJOR_VERSION) || \
        (EMIOS_ICU_IP_IRQ_AR_RELEASE_MINOR_VERSION_C != SCHM_ICU_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Emios_Icu_Ip_Irq.c and SchM_Icu.h are different"
    #endif
    
    #if(EMIOS_ICU_IP_DEV_ERROR_DETECT == STD_ON)
        /* Check if this header file and Devassert.h file are of the same Autosar version */
        #if ((EMIOS_ICU_IP_IRQ_AR_RELEASE_MAJOR_VERSION_C != DEVASSERT_AR_RELEASE_MAJOR_VERSION) || \
             (EMIOS_ICU_IP_IRQ_AR_RELEASE_MINOR_VERSION_C != DEVASSERT_AR_RELEASE_MINOR_VERSION))
            #error "AutoSar Version Numbers of Emios_Icu_Ip_Irq.c and Devassert.h are different"
        #endif
    #endif
#endif
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL VARIABLES
==================================================================================================*/
#define ICU_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Icu_MemMap.h"
#if (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)
extern eMios_Icu_Ip_MeasStatusType eMios_Icu_Ip_aeInt_Counter[EMIOS_ICU_IP_INSTANCE_COUNT][EMIOS_ICU_IP_NUM_OF_CHANNELS];
extern uint16 eMios_Icu_Ip_u16aCapturedActivePulseWidth[EMIOS_ICU_IP_INSTANCE_COUNT][EMIOS_ICU_IP_NUM_OF_CHANNELS];
extern uint16 eMios_Icu_Ip_u16aTimeStart[EMIOS_ICU_IP_INSTANCE_COUNT][EMIOS_ICU_IP_NUM_OF_CHANNELS];
#endif /* EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON */
#if (EMIOS_ICU_IP_TIMESTAMP_API == STD_ON)
extern uint16 eMios_Icu_Ip_u16aBufferPtr[EMIOS_ICU_IP_INSTANCE_COUNT][EMIOS_ICU_IP_NUM_OF_CHANNELS];
#endif /* EMIOS_ICU_IP_TIMESTAMP_API == STD_ON */
#define ICU_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Icu_MemMap.h"

#define ICU_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Icu_MemMap.h"
extern eMios_Icu_Ip_ChStateType eMios_Icu_Ip_ChState[EMIOS_ICU_IP_INSTANCE_COUNT][EMIOS_ICU_IP_NUM_OF_CHANNELS];
#define ICU_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Icu_MemMap.h"

#define ICU_START_SEC_CONST_UNSPECIFIED
#include "Icu_MemMap.h"

extern eMIOS_Type * const s_emiosBase[];

#define ICU_STOP_SEC_CONST_UNSPECIFIED
#include "Icu_MemMap.h"
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#define ICU_START_SEC_CODE
#include "Icu_MemMap.h"
/**
 * @brief 
 * 
 * @param instance 
 * @param channel 
 * @param bOverflow 
 */
static inline void Emios_Icu_Ip_ReportEvents(uint8 instance, uint8 hwChannel, boolean bOverflow);

/**
 * @brief 
 * 
 * @param instance 
 * @param channel 
 * @param bOverflow 
 */
static inline void Emios_Icu_Ip_ReportOverflow(uint8 instance, uint8 hwChannel, boolean bOverflow);

#if (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)
/**
* @brief      Emios_Icu_Ip_GetCaptureRegB
* @details    This function returns the register B of the given eMIOS Unified Channel
* @param[in]  hwChannel  - eMIOS encoded hardware channel
* @return     uint32 - Captured value of the register B
*
* @api
*/
static inline uint32 Emios_Icu_Ip_GetCaptureRegB
(
    const uint8 instance,
    const uint8 hwChannel
);
#endif /* EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON */

#if (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)
/**
* @brief      Emios_Icu_Ip_GetCaptureRegA
* @details    This function returns the register A of the given eMIOS Unified Channel
* @param[in]  hwChannel  - eMIOS encoded hardware channel
* @return     uint32 - Captured value of the register A
*
* @api
*/
static inline uint32 Emios_Icu_Ip_GetCaptureRegA
(
    const uint8 instance,
    const uint8 hwChannel
);
#endif /* (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON) */

/**
* @brief      Emios_Icu_Ip_ClearStatusFlags
* @details    Clear the flags set only for selected interrupts that are enabled
*
* @param[in]      hwChannel    - eMIOS Hardware Channel
* @param[in]      u32BitMask   - flags to be cleared
*
* @return void
*
**/
static inline void Emios_Icu_Ip_ClearStatusFlags
(
    const uint8 instance,
    const uint8 hwChannel,
    uint32 u32BitMask
);

#if (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)
/**
* @brief      Icu driver function that checks the overflow flag
* @details    This function returns clock mode of channel
*
* @param[in]  hwChannel - hardware channel eMIOS
*
* @return     uint32
*
* @api
*/
static inline uint32 Emios_Icu_Ip_GetChannelClockMode
(
    const uint8 instance,
    const uint8 hwChannel
);

static inline void Emios_Icu_Ip_SignalMeasurementStore(uint8  instance,
                                                       uint8  hwChannel,
                                                       uint16 activePulseWidth,
                                                       uint16 period,
                                                       boolean bOverflow);
                                                       
/**
 @brief      This function is used to read the period values for channels
 @details    This function returns the Counter Value based on the configuration
 @param[in]  hwChannel - hardware channel eMIOS
 @pre        Icu_Init must be called before.
*/
static inline uint32 Emios_Icu_Ip_ReadCounterBus
(
    const uint8 instance,
    const uint8 hwChannel
);

static inline void Emios_Icu_Ip_SignalMeasurementWithIPWMMode
(
    const uint8 instance,
    const uint8 hwChannel,
    boolean bOverflow
);

static inline void Emios_Icu_Ip_SignalMeasurementWithIPMMode
(
    const uint8 instance,
    const uint8 hwChannel,
    boolean bOverflow
);

#ifdef EMIOS_ICU_IP_SIGNAL_MEASUREMENT_USES_SAIC_MODE
static inline void Emios_Icu_Ip_SignalMeasurementWithSAICMode
(
    const uint8 instance,
    const uint8 hwChannel,
    boolean bOverflow
);
#endif /* EMIOS_ICU_IP_SIGNAL_MEASUREMENT_USES_SAIC_MODE */

#endif /* EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON */

/**
* @brief      Icu driver function that handles the masterbus interrupt of eMIOS channel.
* @details    This function:
*              - Report overflow to notify if necessary
*
* @param[in]  hwChannel - eMIOS hardware channel
*
*/
static inline eMios_Icu_Ip_BusType Emios_Icu_Ip_ValidateChannelUsedMasterbus
(
    const uint8 instance,
    const uint8 hwChannel
);

/**
* @brief      Emios_Icu_Ip_GetStatusFlags
* @details    Returns the flags set only for the enabled interrupts
*
* @param[in]   hwChannel - eMIOS Hardware Channel
*
* @return      uint32 - Flags set for enabled interrupts
**/
static inline uint32 Emios_Icu_Ip_GetStatusFlags
(
    const uint8 instance,
    const uint8 hwChannel
);

/**
* @brief      Icu driver function that handles the masterbus interrupt of eMIOS channel.
* @details    This function:
*              - Report overflow to notify if necessary
*
* @param[in]  hwChannel - eMIOS hardware channel
*/
static inline void Emios_Icu_Ip_ProcessMasterBusInterrupt
(
    const uint8 instance,
    const uint8 hwChannel
);

#if (STD_ON == EMIOS_ICU_IP_TIMESTAMP_API )
/**
 * @brief      Icu driver function that handles the timestamp measurement type interrupt.
 * @details    This service is  called when an  interrupt is recognized  as a Timestamp
 *             Measurement type.
 *
 * @param[in]  hwChannel - The index of ICU channel for current configuration structure
 * @param[in]  bOverflow        Parameter that indicates the source of report is an overflow
 * @implements Emios_Icu_Ip_TimestampHandler_Activity
*/
static void Emios_Icu_Ip_TimestampHandler(uint8        instance,
                                          uint8        hwChannel,
                                          const uint16 *bufferPtr,
                                          boolean      bOverflow);
#endif /* STD_ON == EMIOS_ICU_IP_TIMESTAMP_API */                                 

#ifdef EMIOS_ICU_IP_SIGNAL_MEASUREMENT_USES_SAIC_MODE
extern void Emios_Icu_Ip_SetActivation
(
    uint8 instance,
    uint8 hwChannel,
    eMios_Icu_Ip_EdgeType edge
);
#endif
/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
#if (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)
static inline uint32 Emios_Icu_Ip_GetCaptureRegB
(
    const uint8 instance,
    const uint8 hwChannel
)
{
    /*return the value of shadow register*/
    return s_emiosBase[instance]->CH.UC[hwChannel].B;
}
#endif

#if (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)
static inline uint32 Emios_Icu_Ip_GetCaptureRegA
(
    const uint8 instance,
    const uint8 hwChannel
)
{
#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif
    /*get the value of A shadow register*/
    return s_emiosBase[instance]->CH.UC[hwChannel].A;
}
#endif /* (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON) */

static inline void Emios_Icu_Ip_ClearStatusFlags
(
    const uint8 instance,
    const uint8 hwChannel,
    uint32 u32BitMask
)
{
    s_emiosBase[instance]->CH.UC[hwChannel].S |= (u32BitMask & (eMIOS_S_OVR_MASK | eMIOS_S_OVFL_MASK | eMIOS_S_FLAG_MASK));
}

#if (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)
static inline uint32 Emios_Icu_Ip_GetChannelClockMode
(
    const uint8 instance,
    const uint8 hwChannel
)
{
    /*return the value of mode selection*/
    return (s_emiosBase[instance]->CH.UC[hwChannel].C & eMIOS_C_MODE_MASK);
}
  
static inline void Emios_Icu_Ip_SignalMeasurementStore(uint8  instance,
                                                       uint8  hwChannel,
                                                       uint16 activePulseWidth,
                                                       uint16 period,
                                                       boolean bOverflow)
{
    /* Saving measurement data into state variable for later usage */
    eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aActivePulseWidth = activePulseWidth;
    eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aPeriod = period;

    
    /* Calling HLD logical channel status changer */
    if(NULL_PTR != eMios_Icu_Ip_ChState[instance][hwChannel].logicChStateCallback)
    {
        eMios_Icu_Ip_ChState[instance][hwChannel].logicChStateCallback(eMios_Icu_Ip_ChState[instance][hwChannel].callbackParam, (1U << 1), TRUE);
    }
    
    /* Calling HLD for reporting wakeup and overflow */
    Emios_Icu_Ip_ReportOverflow(instance, hwChannel, bOverflow);
}

static inline uint32 Emios_Icu_Ip_ReadCounterBus
(
    const uint8 instance,
    const uint8 hwChannel
)
{
    uint32 u32Period = (uint32)0u;
    uint32 u32BusChannelClockMode = (uint32)0u;

    /* Get eMiosBus of hardware channel */
    eMios_Icu_Ip_BusType u32ChannelEmiosBus = eMios_Icu_Ip_ChState[instance][hwChannel].BusSelected;

    switch (u32ChannelEmiosBus)
    {
        case EMIOS_ICU_BUS_INTERNAL_COUNTER:
        {
            u32Period = EMIOS_ICU_IP_COUNTER_MASK_U16;
        }
        break;

        case EMIOS_ICU_BUS_DIVERSE:
        {
            if (EMIOS_ICU_IP_CHANNEL_7 >= hwChannel)
            {
                u32BusChannelClockMode = Emios_Icu_Ip_GetChannelClockMode(instance, EMIOS_ICU_IP_CHANNEL_0);
                /* eMIOS Counter Bus_B Channel (EMIOS_ICU_IP_CHANNEL_0) is in Modulous counter buffer or
                Modulous counter mode */
                if (EMIOS_ICU_IP_MCB_INT_CLOCK_U32 == u32BusChannelClockMode)
                {
                    u32Period= (uint32)Emios_Icu_Ip_GetCaptureRegA(instance, EMIOS_ICU_IP_CHANNEL_0);
                }
                else
                {
                    u32Period= (uint32)Emios_Icu_Ip_GetCaptureRegB(instance, EMIOS_ICU_IP_CHANNEL_0);
                }
            }

            /* eMIOS Channel configured is using Counter Bus_C */
            else if (EMIOS_ICU_IP_CHANNEL_15 >= hwChannel)
            {
                u32BusChannelClockMode = Emios_Icu_Ip_GetChannelClockMode(instance, EMIOS_ICU_IP_CHANNEL_8);
                /* eMIOS Counter Bus_C Channel (EMIOS_ICU_IP_CHANNEL_8) is in Modulous counter buffer or
                 Modulous counter mode */
                if (EMIOS_ICU_IP_MCB_INT_CLOCK_U32 == u32BusChannelClockMode)
                {
                    u32Period= (uint32)Emios_Icu_Ip_GetCaptureRegA(instance, EMIOS_ICU_IP_CHANNEL_8);
                }
                else
                {
                    u32Period= (uint32)Emios_Icu_Ip_GetCaptureRegB(instance, EMIOS_ICU_IP_CHANNEL_8);
                }
            }

            /* eMIOS Channel configured is using Counter Bus_D */
            else if (EMIOS_ICU_IP_CHANNEL_23 >= hwChannel )
            {
                u32BusChannelClockMode = Emios_Icu_Ip_GetChannelClockMode(instance, EMIOS_ICU_IP_CHANNEL_16);
                /* eMIOS Counter Bus_D Channel (EMIOS_ICU_IP_CHANNEL_16) is in Modulous counter buffer or
                 Modulous counter mode */
                if (EMIOS_ICU_IP_MCB_INT_CLOCK_U32 == u32BusChannelClockMode)
                {
                    u32Period= (uint32)Emios_Icu_Ip_GetCaptureRegA(instance, EMIOS_ICU_IP_CHANNEL_16);
                }
                else
                {
                    u32Period= (uint32)Emios_Icu_Ip_GetCaptureRegB(instance, EMIOS_ICU_IP_CHANNEL_16);
                }
            }

#if (STD_ON == EMIOS_ICU_IP_CHANNEL_24_USED)
            /* eMIOS Channel configured is using Counter Bus_E */
            else if (EMIOS_ICU_IP_CHANNEL_31  >= hwChannel)
            {
                u32BusChannelClockMode = Emios_Icu_Ip_GetChannelClockMode(instance, EMIOS_ICU_IP_CHANNEL_24);
                /* eMIOS Counter Bus_E Channel (EMIOS_ICU_IP_CHANNEL_24) is in Modulous counter buffer or
                 Modulous counter mode */
                if (EMIOS_ICU_IP_MCB_INT_CLOCK_U32 == u32BusChannelClockMode)
                {
                    u32Period= (uint32)Emios_Icu_Ip_GetCaptureRegA(instance, EMIOS_ICU_IP_CHANNEL_24);
                }
                else
                {
                    u32Period= (uint32)Emios_Icu_Ip_GetCaptureRegB(instance, EMIOS_ICU_IP_CHANNEL_24);
                }
            }
#endif
            else
            {
                /* Do Nothing */
            }
        }
        break;

        case EMIOS_ICU_BUS_A:
        {
            /* Get clock mode */
            u32BusChannelClockMode = Emios_Icu_Ip_GetChannelClockMode(instance, EMIOS_ICU_IP_CHANNEL_23);

            if(EMIOS_ICU_IP_MCB_INT_CLOCK_U32 == u32BusChannelClockMode)
            {
                u32Period  = (uint32) Emios_Icu_Ip_GetCaptureRegA(instance, EMIOS_ICU_IP_CHANNEL_23);
            }
            else
            {
                u32Period  = (uint32) Emios_Icu_Ip_GetCaptureRegB(instance, EMIOS_ICU_IP_CHANNEL_23);
            }
        }
        break;

        case EMIOS_ICU_BUS_F:
        {
            /* Get clock mode of bus F */
            u32BusChannelClockMode = Emios_Icu_Ip_GetChannelClockMode(instance, EMIOS_ICU_IP_CHANNEL_22);

            if(EMIOS_ICU_IP_MCB_INT_CLOCK_U32 == u32BusChannelClockMode)
            {
                u32Period  = (uint32) Emios_Icu_Ip_GetCaptureRegA(instance, EMIOS_ICU_IP_CHANNEL_22);
            }
            else
            {
                u32Period  = (uint32) Emios_Icu_Ip_GetCaptureRegB(instance, EMIOS_ICU_IP_CHANNEL_22);
            }
        }
        break;

        default:
        {
            /* Rule 16.1 requires switch well-formed */
        }
        break;
    }

    /* return u32Period */
    return (u32Period);
}

static inline void Emios_Icu_Ip_SignalMeasurementWithIPWMMode
(
    const uint8 instance,
    const uint8 hwChannel,
    boolean bOverflow
)
{
    uint16 activePulseWidth;
    uint16 IcuPeriod;
    uint16 Bus_Period;
    eMios_Icu_Ip_MeasType nMeasurement_property = eMios_Icu_Ip_ChState[instance][hwChannel].measurement;
    
    uint16 IcuTempA = (uint16)Emios_Icu_Ip_GetCaptureRegA(instance, hwChannel);
    uint16 IcuTempB = (uint16)Emios_Icu_Ip_GetCaptureRegB(instance, hwChannel);

    if ((EMIOS_ICU_HIGH_TIME == nMeasurement_property) || \
        (EMIOS_ICU_LOW_TIME == nMeasurement_property))
    {
        if (IcuTempA > IcuTempB)
        {
            activePulseWidth = (uint16)(IcuTempA - IcuTempB);
        }
        else /*Counter overflow Case*/
        {
            Bus_Period = (uint16)Emios_Icu_Ip_ReadCounterBus(instance, hwChannel);
            activePulseWidth = (uint16)(IcuTempA + ((uint32)Bus_Period - IcuTempB) + 1U);
        }

        Emios_Icu_Ip_SignalMeasurementStore(instance, hwChannel, activePulseWidth, 0U, bOverflow);
    }
    /* Duty cycle */
    else
    {
        /* Check if a complete signal was acquired */
        if (EMIOS_ICU_MEASUREMENT_PENDING == eMios_Icu_Ip_aeInt_Counter[instance][hwChannel])
        {
            /* Mark that the complete signal was acquired */
            eMios_Icu_Ip_aeInt_Counter[instance][hwChannel] = EMIOS_ICU_MEASUREMENT_DUTY;
            /* Report event and posible overflow to HLD or IPL user overflow notification */
            Emios_Icu_Ip_ReportOverflow (instance, hwChannel, bOverflow);
        }
        else
        {
            /* Compute Period of the signal */
            if (IcuTempB > eMios_Icu_Ip_u16aTimeStart[instance][hwChannel])
            {
                IcuPeriod = IcuTempB - eMios_Icu_Ip_u16aTimeStart[instance][hwChannel];
            }
            else     /*Counter overflow Case */
            {
                Bus_Period = (uint16)Emios_Icu_Ip_ReadCounterBus(instance, hwChannel);
                IcuPeriod = (uint16)\
                (IcuTempB + ((uint32)Bus_Period - eMios_Icu_Ip_u16aTimeStart[instance][hwChannel]) + 1U);
            }
            activePulseWidth = eMios_Icu_Ip_u16aCapturedActivePulseWidth[instance][hwChannel];

            if (EMIOS_ICU_DUTY_CYCLE == nMeasurement_property)
            {
                Emios_Icu_Ip_SignalMeasurementStore(instance, hwChannel, activePulseWidth, IcuPeriod, bOverflow);
            }
            else if (EMIOS_ICU_PERIOD_TIME == nMeasurement_property)
            {
                Emios_Icu_Ip_SignalMeasurementStore(instance, hwChannel, 0U, IcuPeriod, bOverflow);
             }
            else
            {
                /* Nothing to do */
            }
        }
    }

    if (IcuTempA > IcuTempB)
    {
        eMios_Icu_Ip_u16aCapturedActivePulseWidth[instance][hwChannel] = IcuTempA - IcuTempB;
    }
    else /*Counter overflow Case */
    {
        Bus_Period = (uint16)Emios_Icu_Ip_ReadCounterBus(instance, hwChannel);
        eMios_Icu_Ip_u16aCapturedActivePulseWidth[instance][hwChannel] = (uint16)(IcuTempA + ((uint32)Bus_Period - IcuTempB) + 1U);
    }
    eMios_Icu_Ip_u16aTimeStart[instance][hwChannel] = (uint16)IcuTempB;
}

static inline void Emios_Icu_Ip_SignalMeasurementWithIPMMode
(
    const uint8 instance,
    const uint8 hwChannel,
    boolean bOverflow
)
{
    uint16 IcuPeriod;
    uint16 Bus_Period;
    uint16 IcuTempA = (uint16)Emios_Icu_Ip_GetCaptureRegA(instance, hwChannel);
    uint16 IcuTempB = (uint16)Emios_Icu_Ip_GetCaptureRegB(instance, hwChannel);

    if (IcuTempA > IcuTempB)
    {
        IcuPeriod = IcuTempA - IcuTempB;
    }
    else     /*Counter overflow Case*/
    {
        Bus_Period = (uint16)Emios_Icu_Ip_ReadCounterBus(instance, hwChannel);
        IcuPeriod = (uint16)(IcuTempA + ((uint32)Bus_Period - IcuTempB) + 1U);
    }
    Emios_Icu_Ip_SignalMeasurementStore(instance, hwChannel, 0U, IcuPeriod, bOverflow);
}

#ifdef EMIOS_ICU_IP_SIGNAL_MEASUREMENT_USES_SAIC_MODE
static inline void Emios_Icu_Ip_SignalMeasurementWithSAICMode
(
    const uint8 instance,
    const uint8 hwChannel,
    boolean bOverflow
)
{
    uint16 activePulseWidth;
    uint16 IcuPeriod;
    uint16 Bus_Period;
    eMios_Icu_Ip_MeasType nMeasurement_property = eMios_Icu_Ip_ChState[instance][hwChannel].measurement;
    uint16 IcuTempA = (uint16)Emios_Icu_Ip_GetCaptureRegA(instance, hwChannel);

#ifdef EMIOS_ICU_IP_SIGNAL_MEASUREMENT_USES_SAIC_MODE
    uint16 Previous_Value;
    uint16 Pulse_Width;
#endif /* EMIOS_ICU_IP_SIGNAL_MEASUREMENT_USES_SAIC_MODE */
 
    Emios_Icu_Ip_SetActivation (instance, hwChannel, EMIOS_OPPOSITE_EDGES);

    if (EMIOS_ICU_MEASUREMENT_PENDING == eMios_Icu_Ip_aeInt_Counter[instance][hwChannel])
    {
        /* store the first value */
        eMios_Icu_Ip_u16aTimeStart[instance][hwChannel] = IcuTempA;
        eMios_Icu_Ip_aeInt_Counter[instance][hwChannel] = EMIOS_ICU_MEASUREMENT_DUTY;
    }
    else
    {
        Previous_Value = eMios_Icu_Ip_u16aTimeStart[instance][hwChannel];
        /* if first value is greater than the second value */
        if (IcuTempA < Previous_Value)
        {
            Bus_Period = (uint16)Emios_Icu_Ip_ReadCounterBus(instance, hwChannel);
            Pulse_Width = (Bus_Period - Previous_Value) + IcuTempA + 1U;
        }
        else
        {
            Pulse_Width = IcuTempA - Previous_Value;
        }

        /* HIGH TIME or LOW TIME measurement */
        if ((EMIOS_ICU_HIGH_TIME == nMeasurement_property) ||   \
            (EMIOS_ICU_LOW_TIME == nMeasurement_property)
           )
        {
            activePulseWidth = Pulse_Width;
            /* clear to measure next LOW/HIGH pulse */
            eMios_Icu_Ip_aeInt_Counter[instance][hwChannel] = EMIOS_ICU_MEASUREMENT_PENDING;
            Emios_Icu_Ip_SignalMeasurementStore(instance, hwChannel, activePulseWidth, (uint16)0U, bOverflow);
        }
        /* Duty Cycle */
        else
        {
            /* DUTYCYCLE or PERIOD measurement */
            if (EMIOS_ICU_MEASUREMENT_DUTY == eMios_Icu_Ip_aeInt_Counter[instance][hwChannel])
            {
                eMios_Icu_Ip_u16aCapturedActivePulseWidth[instance][hwChannel] = Pulse_Width;
                eMios_Icu_Ip_aeInt_Counter[instance][hwChannel] = EMIOS_ICU_MEASUREMENT_PERIOD;
                if(eMios_Icu_Ip_ChState[instance][hwChannel].callback != NULL_PTR)
                {
                    eMios_Icu_Ip_ChState[instance][hwChannel].callback(eMios_Icu_Ip_ChState[instance][hwChannel].callbackParam, bOverflow);
                }
            }
            else
            {
                /* eMios_Icu_Ip_aeInt_Counter is for period */
                IcuPeriod = eMios_Icu_Ip_u16aCapturedActivePulseWidth[instance][hwChannel] + Pulse_Width;
                activePulseWidth = eMios_Icu_Ip_u16aCapturedActivePulseWidth[instance][hwChannel];
                
                /* set to Duty to find active pulse width next time */
                eMios_Icu_Ip_aeInt_Counter[instance][hwChannel] = EMIOS_ICU_MEASUREMENT_DUTY;
                if (EMIOS_ICU_DUTY_CYCLE == nMeasurement_property)
                {
                    Emios_Icu_Ip_SignalMeasurementStore(instance, hwChannel, activePulseWidth, IcuPeriod, bOverflow);
                }
                else if (EMIOS_ICU_PERIOD_TIME == nMeasurement_property)
                {
                    Emios_Icu_Ip_SignalMeasurementStore(instance, hwChannel, (uint16)0U, IcuPeriod, bOverflow);
                }
                else
                {
                    /**/
                }
            }
            /* store for next time */
            eMios_Icu_Ip_u16aTimeStart[instance][hwChannel] = IcuTempA;
        }
    }
}
#endif
#endif

static inline eMios_Icu_Ip_BusType Emios_Icu_Ip_ValidateChannelUsedMasterbus
(
    const uint8 instance,
    const uint8 hwChannel
)
{
    eMios_Icu_Ip_BusType busResult = (eMios_Icu_Ip_BusType)EMIOS_ICU_BUS_INTERNAL_COUNTER;
    
    /*change bus if it is not the mode of no measurement*/
    if(EMIOS_ICU_MODE_NO_MEASUREMENT != eMios_Icu_Ip_ChState[instance][hwChannel].channelMode)
    {
        busResult = (eMios_Icu_Ip_BusType)eMios_Icu_Ip_ChState[instance][hwChannel].BusSelected;
    }
    return busResult;
}

#if (STD_ON == EMIOS_ICU_IP_TIMESTAMP_API )
static void Emios_Icu_Ip_TimestampHandler(uint8        instance,
                                          uint8        hwChannel,
                                          const uint16 *bufferPtr,
                                          boolean      bOverflow)
{
    Emios_Icu_Ip_ReportOverflow(instance, hwChannel, bOverflow);
    
    eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBuffer[eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBufferIndex] = bufferPtr[0U];
    eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBufferIndex++;

    if (eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBufferIndex >= eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBufferSize)
    {
        if(EMIOS_ICU_CIRCULAR_BUFFER == eMios_Icu_Ip_ChState[instance][hwChannel].timestampBufferType)
        {
            eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBufferIndex = 0U;
        }
        else
        {
            Emios_Icu_Ip_StopTimestamp(instance, hwChannel);
            if (NULL_PTR != eMios_Icu_Ip_ChState[instance][hwChannel].logicChStateCallback)
            {
                eMios_Icu_Ip_ChState[instance][hwChannel].logicChStateCallback(eMios_Icu_Ip_ChState[instance][hwChannel].callbackParam, (1U << 3), FALSE);
            } 

        }
    }

    if (0U != eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBufferNotify)
    {
        eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aNotifyCount++;
        if (eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aNotifyCount >= eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBufferNotify)
        {
            eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aNotifyCount = 0U;
            
            /* Call User Notification Function for IPL and HLD */
            if ((NULL_PTR != eMios_Icu_Ip_ChState[instance][hwChannel].eMiosChannelNotification) && \
                ((boolean)TRUE == eMios_Icu_Ip_ChState[instance][hwChannel].notificationEnable))
            {
                eMios_Icu_Ip_ChState[instance][hwChannel].eMiosChannelNotification();
            }
        }
    }
}
#endif  /* EMIOS_ICU_IP_TIMESTAMP_API */

static inline void Emios_Icu_Ip_ProcessMasterBusInterrupt
(
    const uint8 instance,
    const uint8 hwChannel
)
{
    uint8 nCounter;
    boolean bOverflowUsingMasterbus = FALSE;
    eMios_Icu_Ip_ModeType nMeasMode;
    eMios_Icu_Ip_BusType BusSelected;

    for(nCounter=0U; nCounter < EMIOS_ICU_IP_NUM_OF_CHANNELS; nCounter++)
    {
        bOverflowUsingMasterbus = FALSE;
        BusSelected = Emios_Icu_Ip_ValidateChannelUsedMasterbus(instance, nCounter);
        switch(BusSelected)
        {
            case EMIOS_ICU_BUS_DIVERSE:
            {
                if
                (
                    ((hwChannel % ((uint8)8U)) == 0U) && \
                    (nCounter <= (hwChannel + ((uint8)7U))) && (nCounter >= (hwChannel + ((uint8)1U)))
                )
                {
                    bOverflowUsingMasterbus = TRUE;
                }
            }
            break;
            case EMIOS_ICU_BUS_A:
            {
                if(EMIOS_ICU_IP_CHANNEL_23 == hwChannel)
                {
                    bOverflowUsingMasterbus = TRUE;
                }
            }
            break;
            case EMIOS_ICU_BUS_F:
            {
                if(EMIOS_ICU_IP_CHANNEL_22 == hwChannel)
                {
                    bOverflowUsingMasterbus = TRUE;
                }
            }
            break;
            default:
            {
                /* Do nothing. */
            }
            break;
        }
        if(TRUE == bOverflowUsingMasterbus)
        {
            nMeasMode = eMios_Icu_Ip_ChState[instance][nCounter].channelMode;
            if((EMIOS_ICU_MODE_TIMESTAMP == nMeasMode) || (EMIOS_ICU_MODE_SIGNAL_MEASUREMENT == nMeasMode))
            {
                Emios_Icu_Ip_ReportOverflow(instance, nCounter, TRUE);
            }
        }
    }
}

static inline uint32 Emios_Icu_Ip_GetStatusFlags
(
    const uint8 instance,
    const uint8 hwChannel
)
{
    /*return the value of status*/
    return s_emiosBase[instance]->CH.UC[hwChannel].S ;
}


static inline void Emios_Icu_Ip_ReportEvents(uint8 instance, uint8 hwChannel, boolean bOverflow)
{  
    /* Calling HLD Report Events for the logical channel */
    if(eMios_Icu_Ip_ChState[instance][hwChannel].callback != NULL_PTR)
    {
        eMios_Icu_Ip_ChState[instance][hwChannel].callback(eMios_Icu_Ip_ChState[instance][hwChannel].callbackParam, bOverflow);
    }
    else
    {
        /* Calling Notification for the IPL channel */
        if ((NULL_PTR != eMios_Icu_Ip_ChState[instance][hwChannel].eMiosChannelNotification) && \
           ((boolean)TRUE == eMios_Icu_Ip_ChState[instance][hwChannel].notificationEnable))
        {
            eMios_Icu_Ip_ChState[instance][hwChannel].eMiosChannelNotification();
        }
        
        /* Calling User Overflow Notification for the IPL channel */
        if ( bOverflow && (NULL_PTR != eMios_Icu_Ip_ChState[instance][hwChannel].eMiosOverflowNotification))
        {
            eMios_Icu_Ip_ChState[instance][hwChannel].eMiosOverflowNotification();
        }
    }
}


static inline void Emios_Icu_Ip_ReportOverflow(uint8 instance, uint8 hwChannel, boolean bOverflow)
{
    /* Calling HLD Report wakeup and overflow for the logical channel */
    if(eMios_Icu_Ip_ChState[instance][hwChannel].callback != NULL_PTR)
    {
        eMios_Icu_Ip_ChState[instance][hwChannel].callback(eMios_Icu_Ip_ChState[instance][hwChannel].callbackParam, bOverflow);
    }
    else
    {
        /* Calling User Overflow Notification for the IPL channel */
        if ((NULL_PTR != eMios_Icu_Ip_ChState[instance][hwChannel].eMiosOverflowNotification) && bOverflow)
        {
            eMios_Icu_Ip_ChState[instance][hwChannel].eMiosOverflowNotification();
        }
    }
}

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/

#if (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)
void Emios_Icu_Ip_SignalMeasurementHandler
(
    const uint8 instance,
    const uint8 hwChannel,
    boolean bOverflow
)
{
    eMios_Icu_Ip_UCModeType eMios_OperationMode;

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 

    eMios_OperationMode = eMios_Icu_Ip_ChState[instance][hwChannel].operationMode;
    switch(eMios_OperationMode)
    { 
        case EMIOS_ICU_IPWM :
        {
            Emios_Icu_Ip_SignalMeasurementWithIPWMMode(instance, hwChannel, bOverflow);
        }
        break;
        
        case EMIOS_ICU_IPM :
        {
            Emios_Icu_Ip_SignalMeasurementWithIPMMode(instance, hwChannel, bOverflow);
        }
        break;
#ifdef EMIOS_ICU_IP_SIGNAL_MEASUREMENT_USES_SAIC_MODE
        case EMIOS_ICU_SAIC :
        {
            Emios_Icu_Ip_SignalMeasurementWithSAICMode(instance, hwChannel, bOverflow);
        }
        break;
#endif /* EMIOS_ICU_IP_SIGNAL_MEASUREMENT_USES_SAIC_MODE */
        default:
        {
                /* Do nothing. */
        }
        break;
    }
}
#endif  /* EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API */

/* @implements Emios_Icu_Ip_IrqHandler_Activity */
void Emios_Icu_Ip_IrqHandler(uint8 instance, uint8 channel)
{
    eMios_Icu_Ip_ModeType nMeasMode;
    uint32      u32RegCSR;
    uint32      u32RegCCR;

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(channel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif
    
    boolean     bOverflow = FALSE;
    
    /* Read UC Status Register */
    u32RegCSR = Emios_Icu_Ip_GetStatusFlags(instance, channel);
    u32RegCCR = s_emiosBase[instance]->CH.UC[channel].C;
    
    /* Note: what if this channel is Counter Bus -> MCL must clear the flags. */
    Emios_Icu_Ip_ClearStatusFlags(instance, channel, u32RegCSR);

    /* MCL common ISR checks for spurios interrupts already */
    if(EMIOS_ICU_MODE_NO_MEASUREMENT != eMios_Icu_Ip_ChState[instance][channel].channelMode)
    {
        if(((0U == (channel & (uint8)EMIOS_ICU_IP_CB_DIVERSE)) || (EMIOS_ICU_IP_CHANNEL_22 == channel) || (EMIOS_ICU_IP_CHANNEL_23 == channel)) \
            && \
            (EMIOS_ICU_IP_MCB_INT_CLOCK_U32 == (u32RegCCR & eMIOS_C_MODE_MASK)))
        {
            Emios_Icu_Ip_ProcessMasterBusInterrupt(instance, channel);
        }
        else
        {
#if (EMIOS_ICU_IP_OVERFLOW_NOTIFICATION_API == STD_OFF)
            bOverflow = ((eMIOS_S_OVFL_MASK == (u32RegCSR & eMIOS_S_OVFL_MASK)) ? TRUE : FALSE);
#endif
            nMeasMode = eMios_Icu_Ip_ChState[instance][channel].channelMode;
            switch (nMeasMode)
            {
                case EMIOS_ICU_MODE_SIGNAL_EDGE_DETECT:
                {
                    Emios_Icu_Ip_ReportEvents (instance, channel, bOverflow);
                }
                break;
#if (EMIOS_ICU_IP_TIMESTAMP_API == STD_ON)
                case EMIOS_ICU_MODE_TIMESTAMP:
                {
                    if(EMIOS_ICU_MODE_WITHOUT_DMA == eMios_Icu_Ip_ChState[instance][channel].dmaMode)
                    {
                        /* Copy the Counter value in the Timestamp Buffer. */
                        eMios_Icu_Ip_u16aBufferPtr[instance][channel] = (uint16)s_emiosBase[instance]->CH.UC[channel].A;

                        /* Call timestamp handler. */
                        Emios_Icu_Ip_TimestampHandler(instance, channel, &eMios_Icu_Ip_u16aBufferPtr[instance][channel], bOverflow);
                    }
#if (EMIOS_ICU_IP_TIMESTAMP_USES_DMA == STD_ON)
                    else
                    {
                        /* Note: - look if the existing EMIOSs hardware need this branch. */
                        if(eMios_Icu_Ip_ChState[instance][channel].callback != NULL_PTR)
                        {
                            eMios_Icu_Ip_ChState[instance][channel].callback(eMios_Icu_Ip_ChState[instance][channel].callbackParam, bOverflow);
                        }
                    }
#endif
                }
                break;
#endif /* EMIOS_ICU_IP_TIMESTAMP_API == STD_ON */

#if (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)
                case EMIOS_ICU_MODE_SIGNAL_MEASUREMENT:
                {
                    Emios_Icu_Ip_SignalMeasurementHandler(instance, channel, bOverflow);
                }
                break;
#endif
#if (EMIOS_ICU_IP_EDGE_COUNT_API == STD_ON)
                case EMIOS_ICU_MODE_EDGE_COUNTER:
                {
                    /* Calling HLD Report Wakeup and Overflow for the logical channel */
                    /* Note: - actual version of emios does not have IRQ for overflow, in some configuration this will not get executed */
                    Emios_Icu_Ip_ReportOverflow(instance, channel, bOverflow);
                }
                break;
#endif
                default:
                {
                    /* Rule 16.1 requires switch well-formed */
                }
                break;
            }
        }
    }
}

#define ICU_STOP_SEC_CODE
#include "Icu_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
