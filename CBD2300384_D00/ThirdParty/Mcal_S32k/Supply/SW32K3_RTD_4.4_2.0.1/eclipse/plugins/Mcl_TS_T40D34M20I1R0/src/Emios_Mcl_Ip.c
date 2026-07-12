/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : DMA,CACHE,TRGMUX,LCU,EMIOS,FLEXIO
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
*   @file    Emios_Mcl_Ip.c
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Mcl - Emios Common driver source file.
*   @details
*
*   @addtogroup EMIOS_IP_DRIVER EMIOS IP Driver
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
==================================================================================================*/
#include "Emios_Mcl_Ip.h"

/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define EMIOS_MCL_IP_VENDOR_ID_C                       43
#define EMIOS_MCL_IP_AR_RELEASE_MAJOR_VERSION_C        4
#define EMIOS_MCL_IP_AR_RELEASE_MINOR_VERSION_C        4
#define EMIOS_MCL_IP_AR_RELEASE_REVISION_VERSION_C     0
#define EMIOS_MCL_IP_SW_MAJOR_VERSION_C                2
#define EMIOS_MCL_IP_SW_MINOR_VERSION_C                0
#define EMIOS_MCL_IP_SW_PATCH_VERSION_C                1
/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
#if (EMIOS_MCL_IP_VENDOR_ID_C != EMIOS_MCL_IP_VENDOR_ID)
    #error "Emios_Mcl_Ip.c and Emios_Mcl_Ip.h have different vendor ids"
#endif

/* Check if  source file and Emios_Mcl_Ip.h file are of the same Autosar version */
#if ((EMIOS_MCL_IP_AR_RELEASE_MAJOR_VERSION_C != EMIOS_MCL_IP_AR_RELEASE_MAJOR_VERSION) || \
     (EMIOS_MCL_IP_AR_RELEASE_MINOR_VERSION_C != EMIOS_MCL_IP_AR_RELEASE_MINOR_VERSION) || \
     (EMIOS_MCL_IP_AR_RELEASE_REVISION_VERSION_C != EMIOS_MCL_IP_AR_RELEASE_REVISION_VERSION))
    #error "AutoSar Version Numbers of Emios_Mcl_Ip.c and Emios_Mcl_Ip.h are different"
#endif

/* Check if source file and Emios_Mcl_Ip.h file are of the same Software version */
#if ((EMIOS_MCL_IP_SW_MAJOR_VERSION_C != EMIOS_MCL_IP_SW_MAJOR_VERSION) || \
     (EMIOS_MCL_IP_SW_MINOR_VERSION_C != EMIOS_MCL_IP_SW_MINOR_VERSION) || \
     (EMIOS_MCL_IP_SW_PATCH_VERSION_C != EMIOS_MCL_IP_SW_PATCH_VERSION))
    #error "Software Version Numbers of Emios_Mcl_Ip.c and Emios_Mcl_Ip.h are different"
#endif
/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/
#define MCL_START_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#include "Mcl_MemMap.h"

eMIOS_Type* Emios_Ip_paxBase[eMIOS_INSTANCE_COUNT] = IP_eMIOS_BASE_PTRS;

#define MCL_STOP_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
#include "Mcl_MemMap.h"

#if (EMIOS_IP_MULTICORE_IS_AVAILABLE == STD_ON)

    #define MCL_START_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
    #include "Mcl_MemMap.h"

    static Emios_Ip_InstStateType Emios_Ip_axIpIsInitialized[eMIOS_INSTANCE_COUNT] =
    {
        {(boolean)FALSE, (uint8)255}
    };

    #define MCL_STOP_SEC_VAR_INIT_UNSPECIFIED_NO_CACHEABLE
    #include "Mcl_MemMap.h"

#else

    #define MCL_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
    #include "Mcl_MemMap.h"

    static Emios_Ip_InstStateType Emios_Ip_axIpIsInitialized[eMIOS_INSTANCE_COUNT];

    #define MCL_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
    #include "Mcl_MemMap.h"

#endif


#define MCL_START_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Mcl_MemMap.h"

static Emios_Ip_ChStateType Emios_Ip_axChState[eMIOS_INSTANCE_COUNT][eMIOS_CH_UC_UC_COUNT];

#define MCL_STOP_SEC_VAR_CLEARED_UNSPECIFIED_NO_CACHEABLE
#include "Mcl_MemMap.h"

#define MCL_START_SEC_VAR_CLEARED_16
#include "Mcl_MemMap.h"

/* Array to store the period master bus */
static uint16 Emios_Ip_ChPeriodMasterBus[eMIOS_INSTANCE_COUNT][eMIOS_CH_UC_UC_COUNT];

#define MCL_STOP_SEC_VAR_CLEARED_16
#include "Mcl_MemMap.h"

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/
#define MCL_START_SEC_CODE
#include "Mcl_MemMap.h"

/** @implements Emios_Mcl_Ip_Init_Activity */
Emios_Ip_CommonStatusType Emios_Mcl_Ip_Init(uint8 Instance, const Emios_Mcl_Ip_ConfigType* const ConfigPtr)
{
#if (STD_ON == EMIOS_MCL_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < eMIOS_INSTANCE_COUNT);
    DevAssert(ConfigPtr != NULL_PTR);
#endif

    uint8                     CurrentChannel;
    eMIOS_Type*               Base   = Emios_Ip_paxBase[Instance];
    Emios_Ip_CommonStatusType Status = EMIOS_IP_COMMON_STATUS_SUCCESS;

#if (STD_ON == EMIOS_IP_MULTICORE_IS_AVAILABLE)
    uint8 coreID = (uint8)OsIf_GetCoreID();
    if ( MULTICORE_INIT_CORE == coreID )
    {
#endif /* STD_ON == EMIOS_IP_MULTICORE_IS_AVAILABLE */

        if (Emios_Ip_axIpIsInitialized[Instance].instanceInitState == TRUE)
        {
            Status = EMIOS_IP_COMMON_STATUS_FAIL;
        }
        else
        {
            /* Initialization of EMIOS instance specific parameters. */
            Base->MCR &= ~eMIOS_MCR_GPREN_MASK;
            Base->MCR = eMIOS_MCR_GPRE(ConfigPtr->emiosGlobalConfig->clkDivVal) | eMIOS_MCR_FRZ(((uint32)ConfigPtr->emiosGlobalConfig->allowDebugMode)) | \
                        eMIOS_MCR_GTBE(ConfigPtr->emiosGlobalConfig->enableGlobalTimeBase);

            /* Initialization of EMIOS channel specific parameters. */
            for (CurrentChannel = 0; CurrentChannel < ConfigPtr->channelsNumber; CurrentChannel++)
            {
                if ((*ConfigPtr->masterBusConfig)[CurrentChannel].allowDebugMode)
                {
                    Base->CH.UC[(*ConfigPtr->masterBusConfig)[CurrentChannel].hwChannel].C |= eMIOS_C_FREN_MASK;
                }

                /* Set Cn UCPRE to divider. */
                Base->CH.UC[(*ConfigPtr->masterBusConfig)[CurrentChannel].hwChannel].C2 |= eMIOS_C2_UCEXTPRE((*ConfigPtr->masterBusConfig)[CurrentChannel].masterBusPrescaler);

                switch ((*ConfigPtr->masterBusConfig)[CurrentChannel].masterMode)
                {
                case EMIOS_IP_MC_UP_COUNTER_START:
                    Base->CH.UC[(*ConfigPtr->masterBusConfig)[CurrentChannel].hwChannel].A = (*ConfigPtr->masterBusConfig)[CurrentChannel].defaultPeriod;
                    break;
                case EMIOS_IP_MC_UP_COUNTER_END:
                    Base->CH.UC[(*ConfigPtr->masterBusConfig)[CurrentChannel].hwChannel].A = (*ConfigPtr->masterBusConfig)[CurrentChannel].defaultPeriod;
                    break;
                case EMIOS_IP_MC_UP_DOWN_COUNTER:
                    Base->CH.UC[(*ConfigPtr->masterBusConfig)[CurrentChannel].hwChannel].B = 0;
                    Base->CH.UC[(*ConfigPtr->masterBusConfig)[CurrentChannel].hwChannel].A = (*ConfigPtr->masterBusConfig)[CurrentChannel].defaultPeriod;
                    break;
                case EMIOS_IP_MCB_UP_COUNTER:
                    Base->CH.UC[(*ConfigPtr->masterBusConfig)[CurrentChannel].hwChannel].A = (*ConfigPtr->masterBusConfig)[CurrentChannel].defaultPeriod;
                    break;
                case EMIOS_IP_MCB_UP_DOWN_COUNTER:
                    Base->CH.UC[(*ConfigPtr->masterBusConfig)[CurrentChannel].hwChannel].A = (*ConfigPtr->masterBusConfig)[CurrentChannel].defaultPeriod;
                    break;
                default:
                    /* Do nothing. */
                    break;
                }

                /* Write CNT register with start offset value. */
                Base->CH.UC[(*ConfigPtr->masterBusConfig)[CurrentChannel].hwChannel].CNT = (*ConfigPtr->masterBusConfig)[CurrentChannel].offsetStartValue;

                /* Set mode Cn_MODE = mode. */
                Base->CH.UC[(*ConfigPtr->masterBusConfig)[CurrentChannel].hwChannel].C |= eMIOS_C_MODE((*ConfigPtr->masterBusConfig)[CurrentChannel].masterMode);

                Base->CH.UC[(*ConfigPtr->masterBusConfig)[CurrentChannel].hwChannel].C |= eMIOS_C_UCPREN(1U);

                /* Save current state of channel. */
                Emios_Ip_axChState[Instance][(*ConfigPtr->masterBusConfig)[CurrentChannel].hwChannel].counterMode      = (*ConfigPtr->masterBusConfig)[CurrentChannel].masterMode;
                Emios_Ip_axChState[Instance][(*ConfigPtr->masterBusConfig)[CurrentChannel].hwChannel].channelInitState = TRUE;
                /* Save the period of master bus channel. */
                Emios_Ip_ChPeriodMasterBus[Instance][(*ConfigPtr->masterBusConfig)[CurrentChannel].hwChannel] = (uint16)(*ConfigPtr->masterBusConfig)[CurrentChannel].defaultPeriod;
            }

            /* Save current state of the instance. */
            Emios_Ip_axIpIsInitialized[Instance].instanceInitState = TRUE;
#if (STD_ON == EMIOS_IP_MULTICORE_IS_AVAILABLE)
            Emios_Ip_axIpIsInitialized[Instance].runCore           = ConfigPtr->instanceCoreNumber;
#endif
        }

        Base->MCR |= eMIOS_MCR_GPREN_MASK;

#if (STD_ON == EMIOS_IP_MULTICORE_IS_AVAILABLE)
    }
    else
    {
        Status = EMIOS_IP_COMMON_STATUS_WRONG_CORE;
    }
#endif /* STD_ON == EMIOS_IP_MULTICORE_IS_AVAILABLE */

    return Status;
}

/** @implements Emios_Mcl_Ip_EnableChannel_Activity */
void Emios_Mcl_Ip_EnableChannel(uint8 Instance, uint8 HwChannel)
{
#if (STD_ON == EMIOS_MCL_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < eMIOS_INSTANCE_COUNT);
    DevAssert(HwChannel < eMIOS_CH_UC_UC_COUNT);
#endif

    eMIOS_Type* Base = Emios_Ip_paxBase[Instance];
    Base->UCDIS &= ~(uint32)((uint32)1 << ((uint32)HwChannel));
}

/** @implements Emios_Mcl_Ip_DisableChannel_Activity */
void Emios_Mcl_Ip_DisableChannel(uint8 Instance, uint8 HwChannel)
{
#if (STD_ON == EMIOS_MCL_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < eMIOS_INSTANCE_COUNT);
    DevAssert(HwChannel < eMIOS_CH_UC_UC_COUNT);
#endif

    eMIOS_Type* Base = Emios_Ip_paxBase[Instance];
    Base->UCDIS |= (uint32)((uint32)1 << ((uint32)HwChannel));
}

/** @implements Emios_Mcl_Ip_ComparatorTransferEnable_Activity */
void Emios_Mcl_Ip_ComparatorTransferEnable(uint8 Instance, uint32 ChannelMask)
{
#if (STD_ON == EMIOS_MCL_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < eMIOS_INSTANCE_COUNT);
    DevAssert(ChannelMask < EMIOS_CHANNELMASK_MAXVAL);
#endif

    eMIOS_Type* Base = Emios_Ip_paxBase[Instance];
    /* Update enable. */
    Base->OUDIS &= ~(ChannelMask);
}

/** @implements Emios_Mcl_Ip_ComparatorTransferDisable_Activity */
void Emios_Mcl_Ip_ComparatorTransferDisable(uint8 Instance, uint32 ChannelMask)
{
#if (STD_ON == EMIOS_MCL_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < eMIOS_INSTANCE_COUNT);
    DevAssert(ChannelMask < EMIOS_CHANNELMASK_MAXVAL);
#endif

    eMIOS_Type* Base = Emios_Ip_paxBase[Instance];
    /* Disable channel output. */
    Base->OUDIS |= ChannelMask;
}

/** @implements Emios_Mcl_Ip_Deinit_Activity */
Emios_Ip_CommonStatusType Emios_Mcl_Ip_Deinit(uint8 Instance)
{
#if (STD_ON == EMIOS_MCL_IP_DEV_ERROR_DETECT)
    DevAssert(Instance < eMIOS_INSTANCE_COUNT);
#endif

    uint8                     CurrentChannel;
    Emios_Ip_CommonStatusType Status = EMIOS_IP_COMMON_STATUS_SUCCESS;
    eMIOS_Type*               Base   = Emios_Ip_paxBase[Instance];

#if (STD_ON == EMIOS_IP_MULTICORE_IS_AVAILABLE)
    uint8 coreID = (uint8)OsIf_GetCoreID();
    if ( MULTICORE_INIT_CORE == coreID )
    {
#endif /* STD_ON == EMIOS_IP_MULTICORE_IS_AVAILABLE */
        if (Emios_Ip_axIpIsInitialized[Instance].instanceInitState == FALSE)
        {
            Status = EMIOS_IP_COMMON_STATUS_FAIL;
        }
        else
        {
            Base->MCR &= ~eMIOS_MCR_GPREN_MASK;
            Base->MCR = eMIOS_MCR_GPRE(0U) | eMIOS_MCR_FRZ(0U) | eMIOS_MCR_GTBE(0U);

            for (CurrentChannel = 0; CurrentChannel < eMIOS_CH_UC_UC_COUNT; CurrentChannel++)
            {
                if(Emios_Ip_axChState[Instance][CurrentChannel].channelInitState == TRUE)
                {
                    /* Disable channel pre-scaler (reset default) */
                    Emios_Ip_paxBase[Instance]->CH.UC[CurrentChannel].C = 0U;
                    /* Reset An and Bn registers */
                    Emios_Ip_paxBase[Instance]->CH.UC[CurrentChannel].A = 0UL;
                    Emios_Ip_paxBase[Instance]->CH.UC[CurrentChannel].B = 0UL;

                    Emios_Ip_paxBase[Instance]->UCDIS |= (uint32)((uint32)0 << ((uint32)CurrentChannel));
                    /* Clear the period of master bus channel. */
                    Emios_Ip_ChPeriodMasterBus[Instance][CurrentChannel] = (uint16)0U;
                    Emios_Ip_axChState[Instance][CurrentChannel].channelInitState = FALSE;
                }
            }
            Emios_Ip_axIpIsInitialized[Instance].instanceInitState = FALSE;
        }
#if (STD_ON == EMIOS_IP_MULTICORE_IS_AVAILABLE)
    }
    else
    {
        Status = EMIOS_IP_COMMON_STATUS_WRONG_CORE;
    }
#endif /* STD_ON == EMIOS_IP_MULTICORE_IS_AVAILABLE */

    return Status;
}

/** @implements Emios_Mcl_Ip_SetReloadInterval_Activity */
void Emios_Mcl_Ip_SetReloadInterval(uint8 HwInstance, uint8 HwChannel, uint8 Interval)
{
#if (STD_ON == EMIOS_MCL_IP_DEV_ERROR_DETECT)
    DevAssert((uint8)31 > Interval);
    DevAssert(HwInstance < eMIOS_INSTANCE_COUNT);
    DevAssert(HwChannel < eMIOS_CH_UC_UC_COUNT);
#endif
    /* Specifies the delay interval, in counter bus reload events, between each assertion
     * of AS1-BS1 reload in MC and MCB modes. */
    Emios_Ip_paxBase[HwInstance]->CH.UC[HwChannel].C2 = eMIOS_C2_UCRELDEL_INT(Interval);
}

/** @implements Emios_Mcl_Ip_ValidateChannel_Activity */
boolean Emios_Mcl_Ip_ValidateChannel(uint8 HwInstance, uint8 HwChannel)
{
#if (STD_ON == EMIOS_MCL_IP_DEV_ERROR_DETECT)
    /* Check input parameter*/
    DevAssert(HwInstance < eMIOS_INSTANCE_COUNT);
    DevAssert(HwChannel < eMIOS_CH_UC_UC_COUNT);
#endif
    boolean Valid = FALSE;

    if (TRUE == Emios_Ip_axChState[HwInstance][HwChannel].channelInitState)
    {
        Valid = TRUE;
    }
    return Valid;
}

/** @implements Emios_Mcl_Ip_SetCounterBusPeriod_Activity */
Emios_Ip_CommonStatusType Emios_Mcl_Ip_SetCounterBusPeriod(uint8 HwInstance, uint8 HwChannel, uint16 Period)
{
#if (STD_ON == EMIOS_MCL_IP_DEV_ERROR_DETECT)
    DevAssert((uint16)65535 > Period);
    DevAssert(HwInstance < eMIOS_INSTANCE_COUNT);
    DevAssert(HwChannel < eMIOS_CH_UC_UC_COUNT);
#endif
    Emios_Ip_CommonStatusType Status = EMIOS_IP_COMMON_STATUS_FAIL;

    if (( (Emios_Ip_axChState[HwInstance][HwChannel].counterMode == EMIOS_IP_MCB_UP_COUNTER) || \
          (Emios_Ip_axChState[HwInstance][HwChannel].counterMode == EMIOS_IP_MCB_UP_DOWN_COUNTER) ) && \
          (Period < 1U) )
    {
        Status = EMIOS_IP_COMMON_STATUS_FAIL;
    }
    else
    {
        Emios_Ip_paxBase[HwInstance]->CH.UC[HwChannel].A = Period;
        /* Store the new period */
        Emios_Ip_ChPeriodMasterBus[HwInstance][HwChannel] = Period;
        Status = EMIOS_IP_COMMON_STATUS_SUCCESS;
    }
    return Status;
}


/** @implements Emios_Mcl_Ip_GetCounterBusPeriod_Activity */
uint16 Emios_Mcl_Ip_GetCounterBusPeriod(uint8 Instance, uint8 Channel)
{
#if (EMIOS_MCL_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(Instance < eMIOS_INSTANCE_COUNT);
    DevAssert(Channel < eMIOS_CH_UC_UC_COUNT);
#endif
    /* Get period of counter bus channels. */
    uint16 PeriodCounterBus = Emios_Ip_ChPeriodMasterBus[Instance][Channel];

    return PeriodCounterBus;
}

#if (STD_ON == EMIOS_IP_MULTICORE_IS_AVAILABLE)
/** @implements Emios_Mcl_Ip_ValidateMultiCoreInit_Activity */
boolean Emios_Mcl_Ip_ValidateMultiCoreInit(uint8 HwInstance)
{
    boolean Valid = FALSE;
    uint8 CoreId  = (uint8)OsIf_GetCoreID();

    if (Emios_Ip_axIpIsInitialized[HwInstance].runCore == CoreId)
    {
        Valid = TRUE;
    }

    return Valid;
}
#endif

#define MCL_STOP_SEC_CODE
#include "Mcl_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
