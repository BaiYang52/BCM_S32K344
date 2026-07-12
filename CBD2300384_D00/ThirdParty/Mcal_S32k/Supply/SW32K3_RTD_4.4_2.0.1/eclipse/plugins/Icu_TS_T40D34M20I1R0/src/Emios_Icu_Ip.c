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
#include "SchM_Icu.h"

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    #include "Devassert.h"
#endif

#if (STD_ON == EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT)
    #define USER_MODE_REG_PROT_ENABLED  (EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT)
    #include "RegLockMacros.h"
#endif

/*==================================================================================================
*                                        LOCAL MACROS
==================================================================================================*/
#define EMIOS_ICU_IP_VENDOR_ID_C                       43
#define EMIOS_ICU_IP_AR_RELEASE_MAJOR_VERSION_C        4
#define EMIOS_ICU_IP_AR_RELEASE_MINOR_VERSION_C        4
#define EMIOS_ICU_IP_AR_RELEASE_REVISION_VERSION_C     0
#define EMIOS_ICU_IP_SW_MAJOR_VERSION_C                2
#define EMIOS_ICU_IP_SW_MINOR_VERSION_C                0
#define EMIOS_ICU_IP_SW_PATCH_VERSION_C                1

/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if header file and StandardTypes.h file are of the same Autosar version */
    #if ((EMIOS_ICU_IP_AR_RELEASE_MAJOR_VERSION_C != STD_AR_RELEASE_MAJOR_VERSION) || \
         (EMIOS_ICU_IP_AR_RELEASE_MINOR_VERSION_C != STD_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Emios_Icu_Ip.c and StandardTypes.h are different"
    #endif
    
    /* Check if source file and SchM_Icu.h file are of the same Autosar version */
    #if ((EMIOS_ICU_IP_AR_RELEASE_MAJOR_VERSION_C != SCHM_ICU_AR_RELEASE_MAJOR_VERSION) || \
         (EMIOS_ICU_IP_AR_RELEASE_MINOR_VERSION_C != SCHM_ICU_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Emios_Icu_Ip.c and SchM_Icu.h are different"
    #endif
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if (STD_ON == EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT)
        /* Check if the files Emios_Icu_Ip.c and RegLockMacros.h are of the same version */
        #if ((EMIOS_ICU_IP_AR_RELEASE_MAJOR_VERSION_C != REGLOCKMACROS_AR_RELEASE_MAJOR_VERSION) || \
             (EMIOS_ICU_IP_AR_RELEASE_MINOR_VERSION_C != REGLOCKMACROS_AR_RELEASE_MINOR_VERSION))
            #error "AutoSar Version Numbers of Emios_Icu_Ip.c and RegLockMacros.h are different"
        #endif
    #endif
    
    /* Check if this header file and OsIf.h file are of the same Autosar version */
    #if ((EMIOS_ICU_IP_AR_RELEASE_MAJOR_VERSION != OSIF_AR_RELEASE_MAJOR_VERSION) || \
        (EMIOS_ICU_IP_AR_RELEASE_MINOR_VERSION != OSIF_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Emios_Icu_Ip.h and OsIf.h are different"
    #endif
    
    /* Check if this header file and SchM_Icu.h file are of the same Autosar version */
    #if ((EMIOS_ICU_IP_AR_RELEASE_MAJOR_VERSION != SCHM_ICU_AR_RELEASE_MAJOR_VERSION) || \
        (EMIOS_ICU_IP_AR_RELEASE_MINOR_VERSION != SCHM_ICU_AR_RELEASE_MINOR_VERSION))
        #error "AutoSar Version Numbers of Emios_Icu_Ip.h and SchM_Icu.h are different"
    #endif
#endif

/* Check if source file and ICU header file are of the same vendor */
#if (EMIOS_ICU_IP_VENDOR_ID_C != EMIOS_ICU_IP_VENDOR_ID)
    #error "Emios_Icu_Ip.c and Emios_Icu_Ip.h have different vendor IDs"
#endif
/* Check if source file and Emios_Icu_Ip header file are of the same Autosar version */
#if (  (EMIOS_ICU_IP_AR_RELEASE_MAJOR_VERSION_C != EMIOS_ICU_IP_AR_RELEASE_MAJOR_VERSION) || \
       (EMIOS_ICU_IP_AR_RELEASE_MINOR_VERSION_C != EMIOS_ICU_IP_AR_RELEASE_MINOR_VERSION) || \
       (EMIOS_ICU_IP_AR_RELEASE_REVISION_VERSION_C != EMIOS_ICU_IP_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Emios_Icu_Ip.c and Emios_Icu_Ip.h are different"
#endif
/* Check if source file and Icu_eMIOS header file are of the same Software version */
#if ((EMIOS_ICU_IP_SW_MAJOR_VERSION_C != EMIOS_ICU_IP_SW_MAJOR_VERSION) || \
        (EMIOS_ICU_IP_SW_MINOR_VERSION_C != EMIOS_ICU_IP_SW_MINOR_VERSION) || \
        (EMIOS_ICU_IP_SW_PATCH_VERSION_C != EMIOS_ICU_IP_SW_PATCH_VERSION))
    #error "Software Version Numbers of Emios_Icu_Ip.c and Emios_Icu_Ip.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #if(EMIOS_ICU_IP_DEV_ERROR_DETECT == STD_ON)
        /* Check if this header file and Devassert.h file are of the same Autosar version */
        #if ((EMIOS_ICU_IP_AR_RELEASE_MAJOR_VERSION_C != DEVASSERT_AR_RELEASE_MAJOR_VERSION) || \
            (EMIOS_ICU_IP_AR_RELEASE_MINOR_VERSION_C != DEVASSERT_AR_RELEASE_MINOR_VERSION))
            #error "AutoSar Version Numbers of Emios_Icu_Ip.c and Devassert.h are different"
        #endif
    #endif
#endif
/*==================================================================================================
*                                           LOCAL MACROS
==================================================================================================*/
#if (defined (MCAL_EMIOS_REG_PROT_AVAILABLE) && (STD_ON == MCAL_EMIOS_REG_PROT_AVAILABLE) && (STD_ON == EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT))
    #define Call_Emios_Icu_Ip_SetUserAccessAllowed(BaseAddr) OsIf_Trusted_Call1param(Emios_Icu_Ip_SetUserAccessAllowed,(BaseAddr))
#else
    #define Call_Emios_Icu_Ip_SetUserAccessAllowed(BaseAddr)
#endif

/*==================================================================================================
*                                       LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL VARIABLES
==================================================================================================*/
#define ICU_START_SEC_VAR_CLEARED_BOOLEAN
#include "Icu_MemMap.h"

/* State of initialized EMIOS modules. */
static boolean eMios_Icu_Ip_bInstanceState[EMIOS_ICU_IP_INSTANCE_COUNT];

#define ICU_STOP_SEC_VAR_CLEARED_BOOLEAN
#include "Icu_MemMap.h"

#define ICU_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Icu_MemMap.h"
#if (EMIOS_ICU_IP_EDGE_COUNT_API == STD_ON)
static uint32 eMios_Icu_Ip_u32aEdgeCurrent_Value[EMIOS_ICU_IP_INSTANCE_COUNT][EMIOS_ICU_IP_NUM_OF_CHANNELS];
#endif /* EMIOS_ICU_IP_EDGE_COUNT_API == STD_ON */

#if ((EMIOS_ICU_IP_TIMESTAMP_API == STD_ON)||(EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON))
#if (EMIOS_ICU_IP_OVERFLOW_NOTIFICATION_API == STD_ON)
/**
* @brief      eMios_Icu_Ip_u8NumOvflByCounterBus
* @details    The number of active channels has the overflow notification function using masterbus.
*/
static uint8 eMios_Icu_Ip_u8NumOvflByCounterBus[EMIOS_ICU_IP_INSTANCE_COUNT][EMIOS_ICU_IP_NUM_OF_CHANNELS];
#endif /* EMIOS_ICU_IP_OVERFLOW_NOTIFICATION_API == STD_ON */
#endif /* ((EMIOS_ICU_IP_TIMESTAMP_API == STD_ON)||(EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)) */

#define ICU_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Icu_MemMap.h"
/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/
#define ICU_START_SEC_CONST_UNSPECIFIED
#include "Icu_MemMap.h"
/* Table of base addresses for EMIOS instances. */
eMIOS_Type * const s_emiosBase[] = IP_eMIOS_BASE_PTRS;
#define ICU_STOP_SEC_CONST_UNSPECIFIED
#include "Icu_MemMap.h"
/*==================================================================================================
*                                       GLOBAL VARIABLES
==================================================================================================*/
#define ICU_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Icu_MemMap.h"

#if (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)
eMios_Icu_Ip_MeasStatusType eMios_Icu_Ip_aeInt_Counter[EMIOS_ICU_IP_INSTANCE_COUNT][EMIOS_ICU_IP_NUM_OF_CHANNELS];
uint16 eMios_Icu_Ip_u16aCapturedActivePulseWidth[EMIOS_ICU_IP_INSTANCE_COUNT][EMIOS_ICU_IP_NUM_OF_CHANNELS];
uint16 eMios_Icu_Ip_u16aTimeStart[EMIOS_ICU_IP_INSTANCE_COUNT][EMIOS_ICU_IP_NUM_OF_CHANNELS];
#endif /* EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON */

#if (EMIOS_ICU_IP_TIMESTAMP_API == STD_ON)
uint16 eMios_Icu_Ip_u16aBufferPtr[EMIOS_ICU_IP_INSTANCE_COUNT][EMIOS_ICU_IP_NUM_OF_CHANNELS];
#endif /* EMIOS_ICU_IP_TIMESTAMP_API == STD_ON */

#define ICU_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Icu_MemMap.h"


#define ICU_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Icu_MemMap.h"
eMios_Icu_Ip_ChStateType eMios_Icu_Ip_ChState[EMIOS_ICU_IP_INSTANCE_COUNT][EMIOS_ICU_IP_NUM_OF_CHANNELS];
#define ICU_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Icu_MemMap.h"

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#define ICU_START_SEC_CODE
#include "Icu_MemMap.h"

#if (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)
extern void Emios_Icu_Ip_SignalMeasurementHandler(const uint8 instance, const uint8 hwChannel, boolean bOverflow);
#endif

#if (EMIOS_ICU_IP_DUAL_CLOCK_MODE_API == STD_ON)
/**
 * @brief      Icu driver function that sets the channel prescaler.
 * @details    This function:
 *             - Writes prescaling rate at UCPRE[0:1] bits in EMIOSC[n] register
 *             - Enables channel prescaler by setting UCPREN bit in EMIOSC[n] register
 *
 * @param[in]  u32ChannelPrescaler       - prescaler for the hardware channel
 * @param[in]  hwChannel               - eMIOS encoded hardware channel
 */
static inline void Emios_Icu_Ip_SetPrescaler
(
    const uint8 instance,
    const uint8 hwChannel,
    const uint32 u32ChannelPrescaler
);
#endif  /* EMIOS_ICU_IP_DUAL_CLOCK_MODE_API == STD_ON */

#if ((EMIOS_ICU_IP_TIMESTAMP_API == STD_ON)||(EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON))
#if (EMIOS_ICU_IP_OVERFLOW_NOTIFICATION_API == STD_ON)
/**
* @brief      Icu driver function that enable the masterbus Interrupt.
* @details    This function:
*             - Only uing for the following funtions:
*                   - Emios_Icu_Ip_StartSignalMeasurement: Used for enable masterbuses interrupt that hwChannel uses as an overflow notification.
*                   - Emios_Icu_Ip_StartTimestamp: Used for enable masterbuses interrupt that hwChannel uses as an overflow notification.
*
* @param[in]  hwChannel       - eMIOS encoded hardware channel using master bus
* @api
*/
static inline void Emios_Icu_Ip_EnableMasterBusInterrupt
(
    const uint8 instance,
    const uint8 hwChannel
);

/**
* @brief      Icu driver function that disable the masterbus Interrupt.
* @details    This function:
*             - Only uing for the following funtions:
*                   - Emios_Icu_Ip_StartSignalMeasurement: Used for disable masterbus interrupt that hwChannel uses as an overflow notification.
*                   - Emios_Icu_Ip_StartTimestamp: Used for disable masterbus interrupt that hwChannel uses as an overflow notification.
*
* @param[in]  hwChannel       - eMIOS encoded hardware channel using master bus
* @api
*/
static inline void Emios_Icu_Ip_DisableMasterBusInterrupt
(
    const uint8 instance,
    const uint8 hwChannel
);
#endif /* EMIOS_ICU_IP_OVERFLOW_NOTIFICATION_API == STD_ON */
#endif

/*================================================================================================*/
/**
* @brief      Emios_Icu_Ip_UCSetMode
* @details    This function sets CCR_MODE bitfield, the mode of operation of the Unified Channel
*
* @param[in]  instance  - eMIOS module index
* @param[in]  hwChannel - eMIOS Channel index
* @param[in]  u32Mode  - CCR_MODE bitfield value to be set
* @api
*/
static inline void Emios_Icu_Ip_UCSetMode
(
    const uint8 instance,
    const uint8 hwChannel,
    const uint32 u32Mode
);

/*================================================================================================*/
/**
* @brief   Function to return the channel id of master bus of current channel
* @details Function to return the channel id of master bus of current channel
* @param[in] u8mIdx  - the index of module
* @param[in] u8chIdx - the index of unified channel
* @param[out]  - The id of master bus
*/
/*================================================================================================*/
static inline uint8 Emios_Icu_Ip_GetMasterBus
(
    uint8 hwChannel,
    eMios_Icu_Ip_BusType u32Bus
);

#if (defined(EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT) && (STD_ON == EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT))
#if (defined(MCAL_EMIOS_REG_PROT_AVAILABLE) && (STD_ON == MCAL_EMIOS_REG_PROT_AVAILABLE))
/**
* @brief        Enables EMIOS registers writing in User Mode by configuring REG_PROT
* @details      Sets the UAA (User Access Allowed) bit of the EMIOS IP allowing EMIOS registers writing in User Mode
*
* @param[in]    none
*
* @return       none
*
* @pre          Should be executed in supervisor mode
* @post         none
*
*/
void Emios_Icu_Ip_SetUserAccessAllowed(uint32 EmiosBaseAddr);
#endif /* (defined(MCAL_EMIOS_REG_PROT_AVAILABLE) && (STD_ON == MCAL_EMIOS_REG_PROT_AVAILABLE)) */
#endif /* (defined(EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT) && (STD_ON == EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT)) */


/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
static inline void Emios_Icu_Ip_UCSetMode
(
    const uint8 instance,
    const uint8 hwChannel,
    const uint32 u32Mode
)
{
    SchM_Enter_Icu_ICU_EXCLUSIVE_AREA_48();
    /* Clear MODE bitfield - GPIO mode */
    s_emiosBase[instance]->CH.UC[hwChannel].C &= ~eMIOS_C_MODE_MASK;
    /* Set desired mode */
    s_emiosBase[instance]->CH.UC[hwChannel].C |= (u32Mode & eMIOS_C_MODE_MASK);
    SchM_Exit_Icu_ICU_EXCLUSIVE_AREA_48();
}

static inline uint8 Emios_Icu_Ip_GetMasterBus
(
    uint8 hwChannel,
    eMios_Icu_Ip_BusType u32Bus
)
{
    uint8 u8MasterBusChannelIdx = (uint8)0U;

    if (EMIOS_ICU_BUS_DIVERSE == u32Bus)
    {
        if (EMIOS_ICU_IP_CHANNEL_7 >= hwChannel) /* bus B */
        {
            u8MasterBusChannelIdx = EMIOS_ICU_IP_CHANNEL_0;
        }
        else if (EMIOS_ICU_IP_CHANNEL_15 >= hwChannel) /* Bus C */
        {
            u8MasterBusChannelIdx = EMIOS_ICU_IP_CHANNEL_8;
        }
        else if (EMIOS_ICU_IP_CHANNEL_23 >= hwChannel) /* bus D */
        {
            u8MasterBusChannelIdx = EMIOS_ICU_IP_CHANNEL_16;
        }
        else /* Bus E remaining */
        {
            u8MasterBusChannelIdx = EMIOS_ICU_IP_CHANNEL_24;
        }
    }
    else if (EMIOS_ICU_BUS_A == u32Bus) /* bus A */
    {
        u8MasterBusChannelIdx = EMIOS_ICU_IP_CHANNEL_23;
    }
    else if (EMIOS_ICU_BUS_F == u32Bus) /* bus F remaining */
    {
        u8MasterBusChannelIdx = EMIOS_ICU_IP_CHANNEL_22;
    }
    else
    {
        /*Empty else to fix misra*/
    }
    if(u8MasterBusChannelIdx >= EMIOS_ICU_IP_NUM_OF_CHANNELS)
    {
        u8MasterBusChannelIdx = (uint8)0U;
    }
    return u8MasterBusChannelIdx;
}

#if (EMIOS_ICU_IP_DUAL_CLOCK_MODE_API == STD_ON)
static inline void Emios_Icu_Ip_SetPrescaler
(
    const uint8 instance,
    const uint8 hwChannel,
    const uint32 u32ChannelPrescaler
)
{
    SchM_Enter_Icu_ICU_EXCLUSIVE_AREA_53();

    /* Clear UCPREN bit */
    s_emiosBase[instance]->CH.UC[hwChannel].C &= ~eMIOS_C_UCPREN_MASK;
    /* Write prescaling rate at UCEXTPRE[0:3] bits in EMIOSC2[n] */
    s_emiosBase[instance]->CH.UC[hwChannel].C2 &= ~eMIOS_C2_UCEXTPRE_MASK;

    s_emiosBase[instance]->CH.UC[hwChannel].C2 |= eMIOS_C2_UCEXTPRE(u32ChannelPrescaler);
    /* Enable channel prescaler */
    s_emiosBase[instance]->CH.UC[hwChannel].C |= eMIOS_C_UCPREN_MASK;

    SchM_Exit_Icu_ICU_EXCLUSIVE_AREA_53();
}
#endif  /* EMIOS_ICU_IP_DUAL_CLOCK_MODE_API == STD_ON */

#if ((EMIOS_ICU_IP_TIMESTAMP_API == STD_ON)||(EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON))
#if (EMIOS_ICU_IP_OVERFLOW_NOTIFICATION_API == STD_ON)
static inline void Emios_Icu_Ip_EnableMasterBusInterrupt
(
    const uint8 instance,
    const uint8 hwChannel
)
{
    uint8      nMasterBusHwChannel;

    if (EMIOS_ICU_BUS_INTERNAL_COUNTER != eMios_Icu_Ip_ChState[instance][hwChannel].BusSelected)
    {
        nMasterBusHwChannel = Emios_Icu_Ip_GetMasterBus(hwChannel, eMios_Icu_Ip_ChState[instance][hwChannel].BusSelected);
        if((uint8)0U == eMios_Icu_Ip_u8NumOvflByCounterBus[instance][nMasterBusHwChannel])
        {
            /* Clear pending interrupts */
            s_emiosBase[instance]->CH.UC[hwChannel].S |= (eMIOS_S_OVR_MASK | eMIOS_S_OVFL_MASK | eMIOS_S_FLAG_MASK);
            /* Enable Interrupt for masterbus*/
            Emios_Icu_Ip_EnableInterrupt(instance, nMasterBusHwChannel);
        }
        eMios_Icu_Ip_u8NumOvflByCounterBus[instance][nMasterBusHwChannel]++;
    }
}

static inline void Emios_Icu_Ip_DisableMasterBusInterrupt
(
    const uint8 instance,
    const uint8 hwChannel
)
{
    uint8      nMasterBusHwChannel;

    if (EMIOS_ICU_BUS_INTERNAL_COUNTER != eMios_Icu_Ip_ChState[instance][hwChannel].BusSelected)
    {
        nMasterBusHwChannel = Emios_Icu_Ip_GetMasterBus(hwChannel, eMios_Icu_Ip_ChState[instance][hwChannel].BusSelected);
        eMios_Icu_Ip_u8NumOvflByCounterBus[instance][nMasterBusHwChannel]--;
        if((uint8)0U == eMios_Icu_Ip_u8NumOvflByCounterBus[instance][nMasterBusHwChannel])
        {
            /* Disable Interrupt for masterbus*/
            Emios_Icu_Ip_DisableInterrupt(instance, nMasterBusHwChannel);
        }
    }
}
#endif /* EMIOS_ICU_IP_OVERFLOW_NOTIFICATION_API == STD_ON */
#endif /* (EMIOS_ICU_IP_TIMESTAMP_API == STD_ON)||(EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON) */

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/
#if (defined(EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT) && (STD_ON == EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT))
#if (defined(MCAL_EMIOS_REG_PROT_AVAILABLE) && (STD_ON == MCAL_EMIOS_REG_PROT_AVAILABLE))
void Emios_Icu_Ip_SetUserAccessAllowed(uint32 EmiosBaseAddr)
{
    SET_USER_ACCESS_ALLOWED(EmiosBaseAddr, EMIOS_PROT_MEM_U32);
}
#endif /* (defined(MCAL_EMIOS_REG_PROT_AVAILABLE) && (STD_ON == MCAL_EMIOS_REG_PROT_AVAILABLE)) */
#endif /* (defined(EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT) && (STD_ON == EMIOS_ICU_IP_ENABLE_USER_MODE_SUPPORT)) */


/** @implements Emios_Icu_Ip_EnableInterrupt_Activity */
void Emios_Icu_Ip_EnableInterrupt(uint8 instance, uint8 hwChannel)
{
    /* Clear pending flag */
    s_emiosBase[instance]->CH.UC[hwChannel].S |= eMIOS_S_FLAG_MASK;

    SchM_Enter_Icu_ICU_EXCLUSIVE_AREA_46();
    /* Enable interrupt */
    s_emiosBase[instance]->CH.UC[hwChannel].C |= eMIOS_C_FEN_MASK;
    SchM_Exit_Icu_ICU_EXCLUSIVE_AREA_46();
}

/** @implements Emios_Icu_Ip_DisableInterrupt_Activity */
void Emios_Icu_Ip_DisableInterrupt(uint8 instance, uint8 hwChannel)
{
    SchM_Enter_Icu_ICU_EXCLUSIVE_AREA_47();
    /* Disable interrupt */
    s_emiosBase[instance]->CH.UC[hwChannel].C &= ~eMIOS_C_FEN_MASK;
    SchM_Exit_Icu_ICU_EXCLUSIVE_AREA_47();
}

/* @implements Emios_Icu_Ip_Init_Activity */
eMios_Icu_Ip_StatusType Emios_Icu_Ip_Init(uint8 instance, const eMios_Icu_Ip_ConfigType *userConfig)
{
    uint8                       hwChannel;
    uint32                      u32RegCCR;
    uint8                       u8MasterBusMode[EMIOS_ICU_IP_NUM_OF_CHANNELS];
    uint8                       index;
    uint8                       u8MasterBusChannelIdx = EMIOS_ICU_IP_CB_NONE;
    eMios_Icu_Ip_StatusType     retStatus   = EMIOS_IP_STATUS_SUCCESS;

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(userConfig != NULL_PTR);
#endif 
    if (FALSE == eMios_Icu_Ip_bInstanceState[instance])
    {
        eMios_Icu_Ip_bInstanceState[instance] = TRUE;
        for (index=0U; index < EMIOS_ICU_IP_NUM_OF_CHANNELS; index++)
        {
            eMios_Icu_Ip_ChState[instance][index].channelsInitState = FALSE;
            u8MasterBusMode[index] = EMIOS_ICU_IP_CB_NONE;
        }
        /* Register Protection - Set UAA bit in GCR - allow USER MODE access */
        Call_Emios_Icu_Ip_SetUserAccessAllowed((uint32)s_emiosBase[instance]);

        for (index=0U; index < userConfig->nNumChannels; index++)
        {
            hwChannel   = (*userConfig->pChannelsConfig)[index].hwChannel;
            eMios_Icu_Ip_ChState[instance][hwChannel].channelsInitState = TRUE;
            
            /* Initialize the state for calling user notification - can be HLD or IPL direct user notification */
            eMios_Icu_Ip_ChState[instance][hwChannel].eMiosChannelNotification = (*userConfig->pChannelsConfig)[index].eMiosChannelNotification;
            
            /* Take HLD callback pointer and logic channel and store them in state configuration. */
            eMios_Icu_Ip_ChState[instance][hwChannel].eMiosOverflowNotification = (*userConfig->pChannelsConfig)[index].eMiosOverflowNotification;
            eMios_Icu_Ip_ChState[instance][hwChannel].callback      = (*userConfig->pChannelsConfig)[index].callback;
            eMios_Icu_Ip_ChState[instance][hwChannel].logicChStateCallback = (*userConfig->pChannelsConfig)[index].logicChStateCallback;
            eMios_Icu_Ip_ChState[instance][hwChannel].callbackParam = (*userConfig->pChannelsConfig)[index].callbackParams;
            
            /* Set the event which will generate the interrupt */
            eMios_Icu_Ip_ChState[instance][hwChannel].edgeTrigger   = (*userConfig->pChannelsConfig)[index].edgeAlignement;

            u8MasterBusChannelIdx = (uint8)Emios_Icu_Ip_GetMasterBus(hwChannel, (*userConfig->pChannelsConfig)[index].CntBus);

            /* Clear channel config for hwChannel */
            eMios_Icu_Ip_ChState[instance][hwChannel].channelMode = EMIOS_ICU_MODE_NO_MEASUREMENT;
#if (STD_ON == EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API)
            eMios_Icu_Ip_ChState[instance][hwChannel].measurement = EMIOS_ICU_NO_MEASUREMENT;
            eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aPeriod = (uint16)0U;
            eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aActivePulseWidth = (uint16)0U;
#endif
#if (STD_ON == EMIOS_ICU_IP_TIMESTAMP_API)
            eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBuffer = NULL_PTR;
            eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBufferSize = (uint16)0U;
            eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBufferNotify = (uint16)0U;
            eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aNotifyCount = (uint16)0U;
            eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBufferIndex = (uint16)0U;
            eMios_Icu_Ip_ChState[instance][hwChannel].timestampBufferType = (*userConfig->pChannelsConfig)[index].timestampBufferType;
#endif /* (STD_ON == EMIOS_ICU_IP_TIMESTAMP_API) */
            
            /* Enable EMIOS Channel. */
            Emios_Mcl_Ip_EnableChannel(instance, hwChannel);

            /* Enter GPIO Mode to Configure Channel */
            /* Reset mode for selected channel */
            s_emiosBase[instance]->CH.UC[hwChannel].C &= EMIOS_ICU_IP_CCR_CLEAR_U32;

            /* Initialize channel filter, freeze enable, and bus select and disable the
            * Set Default Edge (included)
            */
            u32RegCCR = eMIOS_C_UCPREN_MASK | eMIOS_C_FCK_MASK;
            u32RegCCR |= eMIOS_C_FREN(((*userConfig->pChannelsConfig)[index].FreezeEn)?1U:0U);
            u32RegCCR |= eMIOS_C_IF((*userConfig->pChannelsConfig)[index].Filter);
            u32RegCCR |= eMIOS_C_BSL((*userConfig->pChannelsConfig)[index].CntBus);

            s_emiosBase[instance]->CH.UC[hwChannel].C |= u32RegCCR;
            
            /* Initialize prescaler value */
            s_emiosBase[instance]->CH.UC[hwChannel].C2 &= ~eMIOS_C2_UCEXTPRE_MASK;
            s_emiosBase[instance]->CH.UC[hwChannel].C2 |= eMIOS_C2_UCEXTPRE((*userConfig->pChannelsConfig)[index].Prescaler);
            
            /* Disable interrupt */
            Emios_Icu_Ip_DisableInterrupt(instance, hwChannel);

            /* Clear pending interrupt flag (and other flags) for the channel */
            s_emiosBase[instance]->CH.UC[hwChannel].S |= (eMIOS_S_OVR_MASK | eMIOS_S_OVFL_MASK | eMIOS_S_FLAG_MASK);

            Emios_Icu_Ip_SetActivation( instance, hwChannel, eMios_Icu_Ip_ChState[instance][hwChannel].edgeTrigger);

            /* Set Config for hwChannel */
            eMios_Icu_Ip_ChState[instance][hwChannel].dmaMode = (*userConfig->pChannelsConfig)[index].chSubMode;
            eMios_Icu_Ip_ChState[instance][hwChannel].operationMode = (*userConfig->pChannelsConfig)[index].ucMode;
            eMios_Icu_Ip_ChState[instance][hwChannel].msWithoutInterrupt = (*userConfig->pChannelsConfig)[index].bWithoutInterrupt;
        #if (STD_ON == EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API)
            eMios_Icu_Ip_ChState[instance][hwChannel].measurement = (*userConfig->pChannelsConfig)[index].measurementMode;
        #endif
            /* Store bus select of hwChannel */
            eMios_Icu_Ip_ChState[instance][hwChannel].BusSelected = (*userConfig->pChannelsConfig)[index].CntBus;

            if (EMIOS_ICU_BUS_INTERNAL_COUNTER != eMios_Icu_Ip_ChState[instance][hwChannel].BusSelected)
            {
                if(EMIOS_ICU_IP_CB_NONE == u8MasterBusMode[u8MasterBusChannelIdx]) /* This master bus have not been configed*/
                {
                    /* store master bus mode and master bus prescaler*/
                    u8MasterBusMode[u8MasterBusChannelIdx] = (uint8)EMIOS_ICU_IP_MCB_INT_CLOCK_U32;
                    /* Enable EMIOS Channel to make registers writeable. */
                    Emios_Mcl_Ip_EnableChannel(instance, hwChannel);
                }
            }
        }

        /* configuration for master bus */
        for (index=0U; index < EMIOS_ICU_IP_NUM_OF_CHANNELS; index++)
        {
            if(EMIOS_ICU_IP_CB_NONE != u8MasterBusMode[index]) /* Master bus is used */
            {
    #if ((EMIOS_ICU_IP_TIMESTAMP_API == STD_ON)||(EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON))
    #if (EMIOS_ICU_IP_OVERFLOW_NOTIFICATION_API == STD_ON)
                /* Reset the number of active channels has the overflow notification function using masterbus. */
                eMios_Icu_Ip_u8NumOvflByCounterBus[instance][index] = 0;
    #endif
    #endif
                /* Set Config for master bus */
                eMios_Icu_Ip_ChState[instance][index].channelMode = EMIOS_ICU_MODE_MASTER_BUS;
            }
        }
    }
    else
    {
        /* Module already initialized - use deinitialize first */
        retStatus = EMIOS_IP_STATUS_ERROR;
    }
    return retStatus;
}

#if (EMIOS_ICU_IP_DEINIT_API == STD_ON)
/* @implements Emios_Icu_Ip_DeinitChannel_Activity */
eMios_Icu_Ip_StatusType Emios_Icu_Ip_Deinit(uint8 instance)
{
    eMios_Icu_Ip_ModeType       nMeasMode;
    uint8                       hwChannel;
    uint8                       u8MasterBusChannelIdx = EMIOS_ICU_IP_CB_NONE;

    eMios_Icu_Ip_StatusType     retStatus   = EMIOS_IP_STATUS_SUCCESS;

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
#endif 

    if (TRUE == eMios_Icu_Ip_bInstanceState[instance])
    {
        eMios_Icu_Ip_bInstanceState[instance] = FALSE;
        
        for (hwChannel=0U; hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS; hwChannel++)
        {
            if (TRUE == eMios_Icu_Ip_ChState[instance][hwChannel].channelsInitState)
            {
                eMios_Icu_Ip_ChState[instance][hwChannel].channelsInitState = FALSE;
                
                nMeasMode = eMios_Icu_Ip_ChState[instance][hwChannel].channelMode;
    
                /* Set all channel registers as after reset */
                s_emiosBase[instance]->CH.UC[hwChannel].C &= EMIOS_ICU_IP_CCR_CLEAR_U32;
                s_emiosBase[instance]->CH.UC[hwChannel].A &= EMIOS_ICU_IP_CCR_CLEAR_U32;

                switch (nMeasMode)
                {
                    case EMIOS_ICU_MODE_SIGNAL_MEASUREMENT:
                    {
                        s_emiosBase[instance]->CH.UC[hwChannel].B &= EMIOS_ICU_IP_CCR_CLEAR_U32;
                    }
                    break;
    
                    case EMIOS_ICU_MODE_EDGE_COUNTER:
                    {
                        s_emiosBase[instance]->CH.UC[hwChannel].CNT &= EMIOS_ICU_IP_CCR_CLEAR_U32;
                    }
                    break;
    
                    default:
                    {
                        /*For Misra Compliance*/
                    }
                    break;
                }
                /* Clear CSR register */
                s_emiosBase[instance]->CH.UC[hwChannel].S |= EMIOS_ICU_IP_CSR_CLEAR_U32;

#if (EMIOS_ICU_IP_EDGE_COUNT_API == STD_ON)
                /* Clear all aEdgeCurrent_Value */
                eMios_Icu_Ip_u32aEdgeCurrent_Value[instance][hwChannel] = (uint32)(0x0U);
#endif /* EMIOS_ICU_IP_EDGE_COUNT_API == STD_ON */

                /* if channel using master bus, so de-init for master bus */
                if (EMIOS_ICU_BUS_INTERNAL_COUNTER != eMios_Icu_Ip_ChState[instance][hwChannel].BusSelected)
                {
                    u8MasterBusChannelIdx = (uint8)Emios_Icu_Ip_GetMasterBus(hwChannel, eMios_Icu_Ip_ChState[instance][hwChannel].BusSelected);
                    if(EMIOS_ICU_MODE_NO_MEASUREMENT != eMios_Icu_Ip_ChState[instance][hwChannel].channelMode)
                    {
                        eMios_Icu_Ip_ChState[instance][u8MasterBusChannelIdx].channelMode = EMIOS_ICU_MODE_NO_MEASUREMENT;
                        
                        /* Disable eMIOS Channel */
                        Emios_Mcl_Ip_DisableChannel(instance, hwChannel);
                    }
                }            
                /* Clear config for hardware channel */
                eMios_Icu_Ip_ChState[instance][hwChannel].operationMode = EMIOS_ICU_UNINIT;
                eMios_Icu_Ip_ChState[instance][hwChannel].BusSelected   = EMIOS_ICU_BUS_INTERNAL_COUNTER;
                eMios_Icu_Ip_ChState[instance][hwChannel].channelMode   = EMIOS_ICU_MODE_NO_MEASUREMENT;
#if (STD_ON == EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API)
                eMios_Icu_Ip_ChState[instance][hwChannel].measurement   = EMIOS_ICU_NO_MEASUREMENT;
#endif
#if (STD_ON == EMIOS_ICU_IP_TIMESTAMP_API)
                eMios_Icu_Ip_ChState[instance][hwChannel].timestampBufferType = EMIOS_ICU_NO_TIMESTAMP;
#endif
            }
        }
    }
    else
    {
        /* Module already de-init - use init first */
        retStatus = EMIOS_IP_STATUS_ERROR;
    }
    return retStatus;
}
#endif

#if (EMIOS_ICU_IP_SET_MODE_API == STD_ON)
void Emios_Icu_Ip_SetSleepMode(uint8 instance, uint8 hwChannel)
{
#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    /* Stop eMIOS channel */
    Emios_Icu_Ip_DisableInterrupt(instance, hwChannel);
}

void Emios_Icu_Ip_SetNormalMode(uint8 instance, uint8 hwChannel)
{
    uint32 u32Value_CCR_FEN;

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    /* u32Value_CCR_FEN will indicate whether the interrupt is enabled or not*/
    u32Value_CCR_FEN  = (s_emiosBase[instance]->CH.UC[hwChannel].C & eMIOS_C_FEN_MASK);
    /* if interrupt is not enabled*/
    if(0U == u32Value_CCR_FEN)
    {
        Emios_Icu_Ip_EnableInterrupt(instance, hwChannel);
    }
}
#endif  /* EMIOS_ICU_IP_SET_MODE_API */

/* @implements Emios_Icu_Ip_SetActivation_Activity */
void Emios_Icu_Ip_SetActivation(uint8 instance, uint8 hwChannel, eMios_Icu_Ip_EdgeType edge)
{
#ifdef EMIOS_ICU_IP_SIGNAL_MEASUREMENT_USES_SAIC_MODE
    uint32 u32RegEmiosCCR;
#endif  /* EMIOS_ICU_IP_SIGNAL_MEASUREMENT_USES_SAIC_MODE */

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    SchM_Enter_Icu_ICU_EXCLUSIVE_AREA_50();
    switch (edge)
    {
#ifdef EMIOS_ICU_IP_SIGNAL_MEASUREMENT_USES_SAIC_MODE
        case EMIOS_OPPOSITE_EDGES:
        {
            u32RegEmiosCCR = s_emiosBase[instance]->CH.UC[hwChannel].C;
            s_emiosBase[instance]->CH.UC[hwChannel].C = (u32RegEmiosCCR ^ eMIOS_C_EDPOL_MASK);
        }
        break;
#endif  /* EMIOS_ICU_IP_SIGNAL_MEASUREMENT_USES_SAIC_MODE */

        case EMIOS_ICU_BOTH_EDGES:
        {
            s_emiosBase[instance]->CH.UC[hwChannel].C |= eMIOS_C_EDSEL_MASK;
        }
        break;

        case EMIOS_ICU_RISING_EDGE:
        {
            s_emiosBase[instance]->CH.UC[hwChannel].C &= ~eMIOS_C_EDSEL_MASK;
            s_emiosBase[instance]->CH.UC[hwChannel].C |= eMIOS_C_EDPOL_MASK;
        }
        break;

        default:
        {
            /* EMIOS_ICU_FALLING_EDGE */
            s_emiosBase[instance]->CH.UC[hwChannel].C &= ~(eMIOS_C_EDSEL_MASK|eMIOS_C_EDPOL_MASK);
        }
        break;
    }
    SchM_Exit_Icu_ICU_EXCLUSIVE_AREA_50();
}

#if (EMIOS_ICU_IP_EDGE_DETECT_API == STD_ON)
/* @implements Emios_Icu_Ip_EnableEdgeDetection_Activity */
void Emios_Icu_Ip_EnableEdgeDetection(uint8 instance, uint8 hwChannel)
{

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    /* Enter GPIO Mode */
    Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_CCR_MODE_GPI_U32);

    /* Enable SAIC mode */
    Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_CCR_MODE_SAIC_U32);

    /* Enable Interrupt */
    Emios_Icu_Ip_EnableInterrupt(instance, hwChannel);

    /* Set Channel Config */
    eMios_Icu_Ip_ChState[instance][hwChannel].channelMode = EMIOS_ICU_MODE_SIGNAL_EDGE_DETECT;

}

/* @implements Emios_Icu_Ip_DisableEdgeDetection_Activity */
void Emios_Icu_Ip_DisableEdgeDetection(uint8 instance, uint8 hwChannel)
{

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    /* Disable interrupt */
    Emios_Icu_Ip_DisableInterrupt(instance, hwChannel);

    /* Enter GPIO Mode */
    Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_CCR_MODE_GPI_U32);

    /* Clear Channel Config */
    eMios_Icu_Ip_ChState[instance][hwChannel].channelMode = EMIOS_ICU_MODE_NO_MEASUREMENT;

}
#endif /* EMIOS_ICU_IP_EDGE_DETECT_API */

/* @implements Emios_Icu_Ip_EnableNotification_Activity */
void Emios_Icu_Ip_EnableNotification(uint8 instance, uint8 hwChannel)
{
#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 

    eMios_Icu_Ip_ChState[instance][hwChannel].notificationEnable =  TRUE;
}

/* @implements Emios_Icu_Ip_DisableNotification_Activity */
void Emios_Icu_Ip_DisableNotification(uint8 instance, uint8 hwChannel)
{
#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 

    eMios_Icu_Ip_ChState[instance][hwChannel].notificationEnable =  FALSE;
}

#if (EMIOS_ICU_IP_TIMESTAMP_API == STD_ON)
/* @implements Emios_Icu_Ip_StartTimestamp_Activity */
void Emios_Icu_Ip_StartTimestamp
(
    uint8 instance,
    uint8 hwChannel,
    uint16 * bufferPtr,
    uint16 bufferSize,
    uint16 notifyInterval
)
{
#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    SchM_Enter_Icu_ICU_EXCLUSIVE_AREA_51();
    eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBuffer = bufferPtr;
    eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBufferSize = bufferSize;
    eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBufferNotify = notifyInterval;
    eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aNotifyCount = (uint16)0U;
    eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBufferIndex = (uint16)0U;
    SchM_Exit_Icu_ICU_EXCLUSIVE_AREA_51();
    
    /* Reset aBufferPtr */
    eMios_Icu_Ip_u16aBufferPtr[instance][hwChannel] = 0U;

    /* Make sure channel is in GPIO mode before switching modes */
    Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_CCR_MODE_GPI_U32);

    /* Clear pending interrupts */
    s_emiosBase[instance]->CH.UC[hwChannel].S |= (eMIOS_S_OVR_MASK | eMIOS_S_OVFL_MASK | eMIOS_S_FLAG_MASK);

    /* Enable SAIC mode */
    Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_CCR_MODE_SAIC_U32);

    if(EMIOS_ICU_MODE_WITH_DMA == eMios_Icu_Ip_ChState[instance][hwChannel].dmaMode)
    {
        SchM_Enter_Icu_ICU_EXCLUSIVE_AREA_51();
        s_emiosBase[instance]->CH.UC[hwChannel].C |= (eMIOS_C_DMA_MASK);
        SchM_Exit_Icu_ICU_EXCLUSIVE_AREA_51();
    }

    /* Set channel Config*/
    eMios_Icu_Ip_ChState[instance][hwChannel].channelMode = EMIOS_ICU_MODE_TIMESTAMP;
    /* Enable Interrupt */
    Emios_Icu_Ip_EnableInterrupt(instance, hwChannel);

#if (EMIOS_ICU_IP_OVERFLOW_NOTIFICATION_API == STD_ON)
    /* Enable Interrupt for masterbus*/
    Emios_Icu_Ip_EnableMasterBusInterrupt(instance ,hwChannel);
#endif
}

/* @implements Emios_Icu_Ip_GetTimestampIndex_Activity */
uint16 Emios_Icu_Ip_GetTimestampIndex
(
    uint8 instance,
    uint8 hwChannel
)
{
    uint16 timestampIndex = 0U;
    
#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    if (NULL_PTR == eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBuffer)
    {
        timestampIndex = 0U;
    }
    else
    {
        timestampIndex = eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aBufferIndex;
    }
    return timestampIndex;
}

/* @implements Emios_Icu_Ip_StopTimestamp_Activity */
void Emios_Icu_Ip_StopTimestamp(uint8 instance, uint8 hwChannel)
{
#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    /* Disable interrupt */
    Emios_Icu_Ip_DisableInterrupt(instance, hwChannel);

    /* Enter GPIO Mode */
    Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_CCR_MODE_GPI_U32);

    /* Clear channel Config */
    eMios_Icu_Ip_ChState[instance][hwChannel].channelMode = EMIOS_ICU_MODE_NO_MEASUREMENT;

#if (EMIOS_ICU_IP_OVERFLOW_NOTIFICATION_API == STD_ON)
    /* Disable Interrupt for masterbus*/
    Emios_Icu_Ip_DisableMasterBusInterrupt(instance, hwChannel);
#endif
}
#endif  /* EMIOS_ICU_IP_TIMESTAMP_API */

#if ((EMIOS_ICU_IP_SIGNALMEASUREMENT_USES_DMA == STD_ON) || (EMIOS_ICU_IP_TIMESTAMP_USES_DMA == STD_ON))
uint32 Emios_Icu_Ip_GetStartAddress
(
    uint8 instance,
    uint8 hwChannel
)
{
#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif
    /*return the value of A register*/
    return (uint32)(&s_emiosBase[instance]->CH.UC[hwChannel].A);
}
#endif


#if (EMIOS_ICU_IP_EDGE_COUNT_API == STD_ON)

/* @implements Emios_Icu_Ip_ResetEdgeCount_Activity */
void Emios_Icu_Ip_ResetEdgeCount
(
    uint8 instance,
    uint8 hwChannel
)
{
    uint32  u32PrevMode;

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    u32PrevMode = (s_emiosBase[instance]->CH.UC[hwChannel].C & eMIOS_C_MODE_MASK);

    /* Disable timer interrupts */
    Emios_Icu_Ip_DisableInterrupt(instance, hwChannel);

    /* Enter GPIO Mode */
    Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_CCR_MODE_GPI_U32);

    /* Reset counter */
    s_emiosBase[instance]->CH.UC[hwChannel].CNT = EMIOS_ICU_IP_INIT_CCNTR_U32;

    /* Max. value for register A. If reached, overflow interrupt */
    s_emiosBase[instance]->CH.UC[hwChannel].A = EMIOS_ICU_IP_INIT_CADR_U32;

    /* Clear pending interrupts */
    s_emiosBase[instance]->CH.UC[hwChannel].S |= (eMIOS_S_OVR_MASK | eMIOS_S_OVFL_MASK | eMIOS_S_FLAG_MASK);

    /* Restore previous mode */
    Emios_Icu_Ip_UCSetMode(instance, hwChannel, u32PrevMode);

    /* Cleare Current Value*/
    eMios_Icu_Ip_u32aEdgeCurrent_Value[instance][hwChannel] = (uint32)0U;
    /* Enable timer interrupts */
    Emios_Icu_Ip_EnableInterrupt(instance, hwChannel);
}

/* @implements Emios_Icu_Ip_EnableEdgeCount_Activity */
void Emios_Icu_Ip_EnableEdgeCount(uint8 instance, uint8 hwChannel)
{
#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    /* Make sure channel is in GPIO mode before switching modes */
    Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_CCR_MODE_GPI_U32);

    /* Set Max. A value */
    s_emiosBase[instance]->CH.UC[hwChannel].A = EMIOS_ICU_IP_INIT_CADR_U32;

    /* Sync. eMIOS counter value. (Needed in case of stopping and re-enabling counting)
     * Note: Setting GPIO mode, the internal counter was cleared and must be restored
     */

    /* When call again affter call Emios_Icu_Ip_DisableEdgeCount*/
    s_emiosBase[instance]->CH.UC[hwChannel].CNT = eMios_Icu_Ip_u32aEdgeCurrent_Value[instance][hwChannel];

    /* Modulus counter mode */
    Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_MCB_EXT_CLOCK_U32);

    /* Clear pending interrupts */
    s_emiosBase[instance]->CH.UC[hwChannel].S |= (eMIOS_S_OVR_MASK | eMIOS_S_OVFL_MASK | eMIOS_S_FLAG_MASK);

    /* Enable interrupt */
    Emios_Icu_Ip_EnableInterrupt(instance, hwChannel);

    /* Set Channel config */
    eMios_Icu_Ip_ChState[instance][hwChannel].channelMode = EMIOS_ICU_MODE_EDGE_COUNTER;


}

/* @implements Emios_Icu_Ip_DisableEdgeCount_Activity */
void Emios_Icu_Ip_DisableEdgeCount
(
    uint8 instance,
    uint8 hwChannel
)
{
#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    /* Disable interrupt */
    Emios_Icu_Ip_DisableInterrupt(instance, hwChannel);

    /* Clear pending interrupts */
    s_emiosBase[instance]->CH.UC[hwChannel].S |= (eMIOS_S_OVR_MASK | eMIOS_S_OVFL_MASK | eMIOS_S_FLAG_MASK);

    /* Save count, before going to GPIO mode (because counter is reset to 0x0000) */
    eMios_Icu_Ip_u32aEdgeCurrent_Value[instance][hwChannel] = s_emiosBase[instance]->CH.UC[hwChannel].CNT;

    /* Disable MCB mode */
    Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_CCR_MODE_GPI_U32);

    eMios_Icu_Ip_ChState[instance][hwChannel].channelMode = EMIOS_ICU_MODE_NO_MEASUREMENT;
}


/* @implements Emios_Icu_Ip_GetEdgeNumbers_Activity */
uint16 Emios_Icu_Ip_GetEdgeNumbers
(
    uint8 instance,
    uint8 hwChannel
)
{
    uint32 u32Mode;
    uint16 u16Result = (uint16)0U;
    u32Mode = (s_emiosBase[instance]->CH.UC[hwChannel].C & eMIOS_C_MODE_MASK);

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    /* Check mode after call Emios_Icu_Ip_DisableEdgeCount */
    if(EMIOS_ICU_IP_CCR_MODE_GPI_U32 == u32Mode)
    {
        u16Result = (uint16)eMios_Icu_Ip_u32aEdgeCurrent_Value[instance][hwChannel];
    }
    else
    {
        u16Result = (uint16)s_emiosBase[instance]->CH.UC[hwChannel].CNT;
    }
    return u16Result;

}
#endif  /* EMIOS_ICU_IP_EDGE_COUNT_API */

#if (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)
/* @implements Emios_Icu_Ip_StartSignalMeasurement_Activity */
void Emios_Icu_Ip_StartSignalMeasurement
(
    uint8 instance,
    uint8 hwChannel
)
{
    eMios_Icu_Ip_UCModeType eMiosOperationMode;

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aPeriod = (uint16)0U;
    eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aActivePulseWidth = (uint16)0U;
    /* Reset capture and timestart of hwChannel */
    eMios_Icu_Ip_u16aCapturedActivePulseWidth[instance][hwChannel] = 0U;
    eMios_Icu_Ip_u16aTimeStart[instance][hwChannel] = 0U;

    /* Disable interrupt */
    Emios_Icu_Ip_DisableInterrupt(instance, hwChannel);

    /* Enter GPIO Mode */
    Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_CCR_MODE_GPI_U32);

    /* Set activation condition */
    Emios_Icu_Ip_SetActivation(instance, hwChannel, eMios_Icu_Ip_ChState[instance][hwChannel].edgeTrigger);

    /* Get operation mode of hardware channel */
    if(EMIOS_ICU_MODE_WITH_DMA == eMios_Icu_Ip_ChState[instance][hwChannel].dmaMode)\
    {
        SchM_Enter_Icu_ICU_EXCLUSIVE_AREA_52();
        s_emiosBase[instance]->CH.UC[hwChannel].C |= (eMIOS_C_DMA_MASK);
        SchM_Exit_Icu_ICU_EXCLUSIVE_AREA_52();
    }
    eMios_Icu_Ip_ChState[instance][hwChannel].channelMode = EMIOS_ICU_MODE_SIGNAL_MEASUREMENT;
    
    eMiosOperationMode = (eMios_Icu_Ip_UCModeType)eMios_Icu_Ip_ChState[instance][hwChannel].operationMode;

    switch (eMiosOperationMode)
    {
        case EMIOS_ICU_IPWM:
            {
                /* Enable IPWM mode */
                Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_CCR_MODE_IPWM_U32);
            }
        break;

        case EMIOS_ICU_IPM:
            {
                /* Enable IPM mode */
                Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_CCR_MODE_IPM_U32);
            }
        break;

        default:
            {
                /* Enable SAIC mode */
                Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_CCR_MODE_SAIC_U32);
            }
        break;
    }
    /* Clear pending interrupt flag (and other flags) for the channel */
    s_emiosBase[instance]->CH.UC[hwChannel].S |= (eMIOS_S_OVR_MASK | eMIOS_S_OVFL_MASK | eMIOS_S_FLAG_MASK);
    /* Clear the counter for signal measurement */
    eMios_Icu_Ip_aeInt_Counter[instance][hwChannel] = EMIOS_ICU_MEASUREMENT_PENDING;
    if (TRUE != eMios_Icu_Ip_ChState[instance][hwChannel].msWithoutInterrupt)
    {
        /* Enable Interrupt */
        Emios_Icu_Ip_EnableInterrupt(instance, hwChannel);
    }

#if (EMIOS_ICU_IP_OVERFLOW_NOTIFICATION_API == STD_ON)
    /* Enable Interrupt for masterbus*/
    Emios_Icu_Ip_EnableMasterBusInterrupt(instance, hwChannel);
#endif
}

/* @implements Emios_Icu_Ip_StopSignalMeasurement_Activity */
void Emios_Icu_Ip_StopSignalMeasurement
(
    uint8 instance,
    uint8 hwChannel
)
{
#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    /* Disable interrupt */
    Emios_Icu_Ip_DisableInterrupt(instance, hwChannel);

    /* Enter GPIO Mode */
    Emios_Icu_Ip_UCSetMode(instance, hwChannel, EMIOS_ICU_IP_CCR_MODE_GPI_U32);

    SchM_Enter_Icu_ICU_EXCLUSIVE_AREA_49();
    /* EDSEL is set to 1 to discard the input in GPIO mode (not to repond the input signal
       in GPIO mode)*/
    /* Disables the flag generation as defined by EDPOL Bit */
    s_emiosBase[instance]->CH.UC[hwChannel].C |= eMIOS_C_EDSEL_MASK;
    SchM_Exit_Icu_ICU_EXCLUSIVE_AREA_49();

    /* Clear pending interrupt flag (and other flags) for the channel */
    s_emiosBase[instance]->CH.UC[hwChannel].S |= (eMIOS_S_OVR_MASK | eMIOS_S_OVFL_MASK | eMIOS_S_FLAG_MASK);
    /* Clear channel config */
    eMios_Icu_Ip_ChState[instance][hwChannel].channelMode = EMIOS_ICU_MODE_NO_MEASUREMENT;
#if (EMIOS_ICU_IP_OVERFLOW_NOTIFICATION_API == STD_ON)
    /* Disable Interrupt for masterbus*/
    Emios_Icu_Ip_DisableMasterBusInterrupt(instance, hwChannel);
#endif
}

/* @implements Emios_Icu_Ip_GetTimeElapsed_Activity */
uint16 Emios_Icu_Ip_GetTimeElapsed
(
    uint8 instance,
    uint8 hwChannel
)
{
    uint16 timeElapsed = (uint16)0U;
    

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    if((EMIOS_ICU_DUTY_CYCLE != eMios_Icu_Ip_ChState[instance][hwChannel].measurement) && \
        (EMIOS_ICU_NO_MEASUREMENT != eMios_Icu_Ip_ChState[instance][hwChannel].measurement))
    {
        if ((eMios_Icu_Ip_MeasType)EMIOS_ICU_PERIOD_TIME == eMios_Icu_Ip_ChState[instance][hwChannel].measurement)
        {
            timeElapsed = eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aPeriod;
            eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aPeriod = (uint16)0U;
        }
        else
        {
            timeElapsed = eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aActivePulseWidth;
            eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aActivePulseWidth = (uint16)0U;
        }
    }
    #if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    else
    {
        DevAssert(TRUE);
    }
    #endif
    return timeElapsed;
}

/* @implements Emios_Icu_Ip_GetDutyCycleValues_Activity */
void Emios_Icu_Ip_GetDutyCycleValues
(
    uint8 instance,
    uint8 hwChannel,
    eMios_Icu_Ip_DutyCycleType*  dutyCycleValues 
)
{

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    if(EMIOS_ICU_DUTY_CYCLE == eMios_Icu_Ip_ChState[instance][hwChannel].measurement)
    {
        if ((uint16)0U != eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aPeriod)
        {
            dutyCycleValues->ActiveTime = (uint16)eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aActivePulseWidth;
            dutyCycleValues->PeriodTime = (uint16)eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aPeriod;
            eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aActivePulseWidth = (uint16)0U;
            eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aPeriod           = (uint16)0U;
        }
        else
        {
            dutyCycleValues->ActiveTime = (uint16)0U;
            dutyCycleValues->PeriodTime = (uint16)0U;
        }
    }
    #if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    else
    {
        DevAssert(TRUE);
    }
    #endif 
}

/**
 * @brief Emios_Icu_Ip_SetPWandPeriod
 */
void Emios_Icu_Ip_SetPWandPeriod(uint8 instance, \
                               uint8 hwChannel, \
                               uint16 activePulseWidth, \
                               uint16 period)
{
    eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aActivePulseWidth = activePulseWidth;
    eMios_Icu_Ip_ChState[instance][hwChannel].eMios_Icu_Ip_aPeriod           = period;
}
#endif  /* EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API */

#if (EMIOS_ICU_IP_GET_INPUT_STATE_API == STD_ON)
boolean Emios_Icu_Ip_GetInputState
(
    uint8 instance,
    uint8 hwChannel
)
{
    boolean bResult = FALSE;
    uint32 u32ValueCCRFEN;
    uint32 u32ValueCSRFLAG;

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    u32ValueCCRFEN  = (s_emiosBase[instance]->CH.UC[hwChannel].C & eMIOS_C_FEN_MASK);
    u32ValueCSRFLAG = (s_emiosBase[instance]->CH.UC[hwChannel].S & eMIOS_S_FLAG_MASK);

    /* Interrupt not enabled, flag bit was set */
    if ( (eMIOS_C_FEN_MASK != u32ValueCCRFEN) && (eMIOS_S_FLAG_MASK == u32ValueCSRFLAG))
    {

        /* Clear pending interrupt */
        s_emiosBase[instance]->CH.UC[hwChannel].S |= eMIOS_S_FLAG_MASK;
        bResult = TRUE;
    }

    return bResult;
}
#endif  /* EMIOS_ICU_IP_GET_INPUT_STATE_API */

#if (EMIOS_ICU_IP_GET_INPUT_LEVEL_API == STD_ON)
/* @implements  Emios_Icu_Ip_GetInputLevel_Activity */
eMios_Icu_Ip_LevelType Emios_Icu_Ip_GetInputLevel
(
    uint8 instance,
    uint8 hwChannel
)
{
#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    return ((0U != ((s_emiosBase[instance]->CH.UC[hwChannel].S & eMIOS_S_UCIN_MASK)>> eMIOS_S_UCIN_SHIFT))? \
            EMIOS_ICU_LEVEL_HIGH : EMIOS_ICU_LEVEL_LOW);
}
#endif /* EMIOS_ICU_IP_GET_INPUT_LEVEL_API == STD_ON */

boolean Emios_Icu_Ip_GetOverflow
(
    uint8 instance,
    uint8 hwChannel
)
{
    uint32 u32RegCSR = (uint32) 0U;

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    u32RegCSR = s_emiosBase[instance]->CH.UC[hwChannel].S;

    /* Clear pending interrupt serviced */
    s_emiosBase[instance]->CH.UC[hwChannel].S |= eMIOS_S_OVFL_MASK;

    return ((eMIOS_S_OVFL_MASK == (u32RegCSR & eMIOS_S_OVFL_MASK)) ? TRUE : FALSE);
}

#if (EMIOS_ICU_IP_DUAL_CLOCK_MODE_API == STD_ON)
void Emios_Icu_Ip_SetClockMode
(
    uint8 instance,
    const eMios_Icu_Ip_ConfigType                   *peMiosIpConfig,
    const eMios_Icu_Ip_ClockModeType                Prescaler
)
{
    /* logical channel */
    uint8   index;
    uint8   hwChannel;
    eMios_Icu_Ip_BusType  nCtrlBus;
    uint8   u8MasterBusChannelIdx;
    uint8   u8MasterBusUse[EMIOS_ICU_IP_NUM_OF_CHANNELS];

#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
#endif 
    for (index=0U; index < (uint8)EMIOS_ICU_IP_NUM_OF_CHANNELS; index++)
    {
        u8MasterBusUse[index] = (uint8)EMIOS_ICU_IP_CB_NONE;
    }

    for (index=0U; index < peMiosIpConfig->nNumChannels; index++)
    {
        hwChannel   = (*peMiosIpConfig->pChannelsConfig)[index].hwChannel;
        nCtrlBus = eMios_Icu_Ip_ChState[instance][hwChannel].BusSelected;

        if (EMIOS_ICU_BUS_INTERNAL_COUNTER == nCtrlBus)
        {
            if (EMIOS_ICU_NORMAL_CLK == Prescaler)
            {
                Emios_Icu_Ip_SetPrescaler(  instance,
                                            hwChannel,
                                            (uint32)(*peMiosIpConfig->pChannelsConfig)[index].Prescaler
                                          );
            }
            else
            {
                Emios_Icu_Ip_SetPrescaler(  instance,
                                            hwChannel,
                                            (uint32)(*peMiosIpConfig->pChannelsConfig)[index].AltPrescaler
                                          );
            }
        }
        else
        {
            u8MasterBusChannelIdx = (uint8)Emios_Icu_Ip_GetMasterBus(hwChannel, nCtrlBus);

            if(u8MasterBusUse[u8MasterBusChannelIdx] == EMIOS_ICU_IP_CB_NONE)
            {
                if (EMIOS_ICU_NORMAL_CLK == Prescaler)
                {
                    Emios_Icu_Ip_SetPrescaler(  instance,
                                            u8MasterBusChannelIdx,
                                            (uint32)(*peMiosIpConfig->pChannelsConfig)[index].Prescaler
                                          );
                }
                else
                {
                    Emios_Icu_Ip_SetPrescaler(  instance,
                                            u8MasterBusChannelIdx,
                                            (uint32)(*peMiosIpConfig->pChannelsConfig)[index].AltPrescaler
                                          );
                }
                u8MasterBusUse[u8MasterBusChannelIdx] = (uint8)1U;
            }
        }
    }
}
#endif  /* EMIOS_ICU_IP_DUAL_CLOCK_MODE_API == STD_ON */

#if ((EMIOS_ICU_IP_CAPTURERGISTER_API == STD_ON) && ((EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON) || (EMIOS_ICU_IP_TIMESTAMP_API == STD_ON)))
/** @implements Emios_Icu_Ip_GetCaptureRegValue_Activity */
uint32 Emios_Icu_Ip_GetCaptureRegValue
(
    uint8 instance,
    uint8 hwChannel
)
{
#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    return s_emiosBase[instance]->CH.UC[hwChannel].A;
}
#endif  /* (EMIOS_ICU_IP_CAPTURERGISTER_API == STD_ON) && ((EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON) || (EMIOS_ICU_IP_TIMESTAMP_API == STD_ON)) */

#if ((EMIOS_ICU_IP_GET_PULSE_WIDTH_API == STD_ON) && (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON))
/* @implements Emios_Icu_Ip_GetPulseWidth_Activity */
void Emios_Icu_Ip_GetPulseWidth
(
    uint8 instance,
    uint8 hwChannel
)
{
#if (STD_ON == EMIOS_ICU_IP_DEV_ERROR_DETECT)
    DevAssert(instance < EMIOS_ICU_IP_INSTANCE_COUNT);
    DevAssert(hwChannel < EMIOS_ICU_IP_NUM_OF_CHANNELS);
#endif 
    Emios_Icu_Ip_SignalMeasurementHandler(instance, hwChannel, FALSE);
}
#endif /* ((EMIOS_ICU_IP_GET_PULSE_WIDTH_API == STD_ON) && (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON))*/

#if ((EMIOS_ICU_IP_VALIDATE_GLOBAL_CALL == STD_ON) && ((EMIOS_ICU_IP_GET_PULSE_WIDTH_API == STD_ON) && (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON)))
Std_ReturnType Emios_Icu_Ip_ValidateSignalMeasureWithoutInterrupt
(
    uint8 instance,
    uint8 hwChannel
)
{
    /*return ICU signal property without using interrupt*/
    return (eMios_Icu_Ip_ChState[instance][hwChannel].msWithoutInterrupt)?(Std_ReturnType)E_OK:(Std_ReturnType)E_NOT_OK;
}
#endif /* ((EMIOS_ICU_IP_GET_PULSE_WIDTH_API == STD_ON) && (EMIOS_ICU_IP_SIGNAL_MEASUREMENT_API == STD_ON))*/

#define ICU_STOP_SEC_CODE
#include "Icu_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
