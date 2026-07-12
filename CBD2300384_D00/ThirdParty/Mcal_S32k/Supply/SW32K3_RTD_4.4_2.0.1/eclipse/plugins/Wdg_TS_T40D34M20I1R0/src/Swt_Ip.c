/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : Swt
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
*   @addtogroup Swt
*   @{
*/

#ifdef __cplusplus
extern "C"{
#endif


/*=================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
=================================================================================================*/
#include "Swt_Ip.h"
#include "Devassert.h"
#include "SchM_Wdg.h"

#include "OsIf.h"
#include "Swt_Ip_Irq.h"
/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define SWT_IP_VENDOR_ID_C                      43
#define SWT_IP_AR_RELEASE_MAJOR_VERSION_C       4
#define SWT_IP_AR_RELEASE_MINOR_VERSION_C       4
#define SWT_IP_AR_RELEASE_REVISION_VERSION_C    0
#define SWT_IP_SW_MAJOR_VERSION_C               2
#define SWT_IP_SW_MINOR_VERSION_C               0
#define SWT_IP_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/* Check if current file and Swt_Ip header file are of the same vendor */
#if (SWT_IP_VENDOR_ID_C != SWT_IP_VENDOR_ID)
#error "Swt_Ip.c and Swt_Ip.h have different vendor ids"
#endif

/* Check if current file and Swt_Ip header file are of the same Autosar version */
#if ((SWT_IP_AR_RELEASE_MAJOR_VERSION_C     != SWT_IP_AR_RELEASE_MAJOR_VERSION) || \
     (SWT_IP_AR_RELEASE_MINOR_VERSION_C     != SWT_IP_AR_RELEASE_MINOR_VERSION) || \
     (SWT_IP_AR_RELEASE_REVISION_VERSION_C  != SWT_IP_AR_RELEASE_REVISION_VERSION))
#error "AutoSar Version Numbers of Swt_Ip.c and Swt_Ip.h are different"
#endif

/* Check if current file and Swt_Ip header file are of the same software version */
#if ((SWT_IP_SW_MAJOR_VERSION_C != SWT_IP_SW_MAJOR_VERSION) || \
     (SWT_IP_SW_MINOR_VERSION_C != SWT_IP_SW_MINOR_VERSION) || \
     (SWT_IP_SW_PATCH_VERSION_C != SWT_IP_SW_PATCH_VERSION))
#error "Software Version Numbers of Swt_Ip.c and Swt_Ip.h are different"
#endif

/* Check if current file and Swt_Ip_Irq header file are of the same vendor */
#if (SWT_IP_VENDOR_ID_C != SWT_IP_IRQ_VENDOR_ID)
#error "Swt_Ip.c and Swt_Ip_Irq.h have different vendor ids"
#endif

/* Check if current file and Swt_Ip_Irq header file are of the same Autosar version */
#if ((SWT_IP_AR_RELEASE_MAJOR_VERSION_C     != SWT_IP_IRQ_AR_RELEASE_MAJOR_VERSION) || \
     (SWT_IP_AR_RELEASE_MINOR_VERSION_C     != SWT_IP_IRQ_AR_RELEASE_MINOR_VERSION) || \
     (SWT_IP_AR_RELEASE_REVISION_VERSION_C  != SWT_IP_IRQ_AR_RELEASE_REVISION_VERSION))
#error "AutoSar Version Numbers of Swt_Ip.c and Swt_Ip_Irq.h are different"
#endif

/* Check if current file and Swt_Ip_Irq header file are of the same software version */
#if ((SWT_IP_SW_MAJOR_VERSION_C != SWT_IP_IRQ_SW_MAJOR_VERSION) || \
     (SWT_IP_SW_MINOR_VERSION_C != SWT_IP_IRQ_SW_MINOR_VERSION) || \
     (SWT_IP_SW_PATCH_VERSION_C != SWT_IP_IRQ_SW_PATCH_VERSION))
#error "Software Version Numbers of Swt_Ip.c and Swt_Ip_Irq.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and Devassert header file are of the same Autosar version */
    #if ((SWT_IP_AR_RELEASE_MAJOR_VERSION_C    != DEVASSERT_AR_RELEASE_MAJOR_VERSION) || \
        (SWT_IP_AR_RELEASE_MINOR_VERSION_C     != DEVASSERT_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Swt_Ip.c and Devassert.h are different"
    #endif

    /* Check if current file and StandardTypes header file are of the same Autosar version */
    #if ((SWT_IP_AR_RELEASE_MAJOR_VERSION_C    != STD_AR_RELEASE_MAJOR_VERSION) || \
        (SWT_IP_AR_RELEASE_MINOR_VERSION_C     != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Swt_Ip.c and StandardTypes.h are different"
    #endif

    /* Check if current file and StandardTypes header file are of the same Autosar version */
    #if ((SWT_IP_AR_RELEASE_MAJOR_VERSION_C    != SCHM_WDG_AR_RELEASE_MAJOR_VERSION) || \
        (SWT_IP_AR_RELEASE_MINOR_VERSION_C     != SCHM_WDG_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Swt_Ip.c and SchM_Wdg.h are different"
    #endif
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */

/* Check if current file and Osif header file are of the same Autosar version */
#if ((SWT_IP_AR_RELEASE_MAJOR_VERSION_C    != OSIF_AR_RELEASE_MAJOR_VERSION) || \
    (SWT_IP_AR_RELEASE_MINOR_VERSION_C     != OSIF_AR_RELEASE_MINOR_VERSION))
#error "AutoSar Version Numbers of Swt_Ip.c and Osif.h are different"
#endif

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/
#define SWT_IP_MAP(x)  (((uint32)(((uint32)(x)) << SWT_IP_MAP_SHIFT)) & SWT_IP_MAP_MASK_U32)

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

#define WDG_START_SEC_CONST_UNSPECIFIED

#include "Wdg_MemMap.h"

#if SWT_IP_DISCOUNTINUOUS_INSTANCE_IDS == STD_OFF
static SWT_Type * const Swt_Ip_sBase[SWT_INSTANCE_COUNT] = IP_SWT_BASE_PTRS;
#else
static SWT_Type * const Swt_Ip_sBase[SWT_INSTANCE_COUNT] = {IP_SWT_0, IP_SWT_0, IP_SWT_2, IP_SWT_3};
#endif

#define WDG_STOP_SEC_CONST_UNSPECIFIED

#include "Wdg_MemMap.h"

#define WDG_START_SEC_VAR_CLEARED_UNSPECIFIED

#include "Wdg_MemMap.h"

static Swt_CallbackPtrType Swt_Ip_apCallbackPtr[SWT_INSTANCE_COUNT];

#define WDG_STOP_SEC_VAR_CLEARED_UNSPECIFIED

#include "Wdg_MemMap.h"

/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/

#ifdef WDG_ROM
    #if (WDG_ROM == 1U)
        #define WDG_START_SEC_CODE
    #endif
#else
    #if (WDG_RAM == 0U)
        #define WDG_START_SEC_RAMCODE
    #endif
#endif

#include "Wdg_MemMap.h"

static inline boolean Swt_Ip_IsEnable(const SWT_Type * const Base)
{
    /* Checks whether the SWT is enabled */
    return ((Base->CR & SWT_CR_WEN_MASK) != 0U);
}

static inline void Swt_Ip_Enable(SWT_Type * const Base)
{
    /* Enables the SWT */
    Base->CR |= SWT_CR_WEN_MASK;
}

static inline void Swt_Ip_Disable(SWT_Type * const Base)
{
    /* Enables the SWT */
    Base->CR &= ~SWT_CR_WEN_MASK;
}

static  Swt_Ip_ServiceModeType Swt_Ip_GetServiceMode(const SWT_Type * const Base)
{
    Swt_Ip_ServiceModeType Mode;

    /* Gets current service mode */
    switch ((Base->CR & SWT_CR_SMD_MASK) >> SWT_CR_SMD_SHIFT)
    {
#if (SWT_IP_SUPPORT_IACR == STD_ON)
        /* Incremental Address Execution */
        case 3U:
            Mode = SWT_IP_IA_EXE_MODE;
            break;
        /* Fixed Address Execution */
        case 2U:
            Mode = SWT_IP_FA_EXE_MODE;
            break;
#endif            
        /* Keyed Service Sequence */
        case 1U:
            Mode = SWT_IP_KS_SEQ_MODE;
            break;
        /* Fixed Service Sequence */
        default:
            Mode = SWT_IP_FS_SEQ_MODE;
            break;
    }

    return Mode;
}

static Swt_Ip_LockType Swt_Ip_GetLock(const SWT_Type * const Base)
{
    Swt_Ip_LockType LockMode = SWT_IP_UNLOCK;

    /* Get lock mode */
    switch ((Base->CR & (SWT_CR_SLK_MASK | SWT_CR_HLK_MASK)) >> SWT_CR_SLK_SHIFT)
    {
        case 0U:
            LockMode = SWT_IP_UNLOCK;
            break;
        case 1U:
            LockMode = SWT_IP_SOFTLOCK;
            break;
        case 2U:
            LockMode = SWT_IP_HARDLOCK;
            break;
        default:
            LockMode = SWT_IP_HARDLOCK;
            break;
    }

    return LockMode;
}

static void Swt_Ip_Lock(SWT_Type * const Base,
                 Swt_Ip_LockType LockConfig)
{
    /* Configures lock bits */
    switch (LockConfig)
    {
        /* Hard lock */
        case SWT_IP_HARDLOCK:
            Base->CR |= SWT_CR_HLK(1U);
            break;
        /* Soft lock */
        case SWT_IP_SOFTLOCK:
            Base->CR |= SWT_CR_SLK(1U);
            break;
        /* Unlock */
        default:
            /* Do nothing */
            break;
    }
}

static Swt_Ip_StatusType Swt_Ip_Unlock(SWT_Type * const Base)
{
    Swt_Ip_StatusType Ret = SWT_IP_STATUS_SUCCESS;

    uint32 TimeoutTicks = OsIf_MicrosToTicks(SWT_IP_OSIF_TIMEOUT_VAL, SWT_IP_TIMEOUT_METHOD);
    uint32 CurrentTicks = 0u; /* initialize current counter */
    uint32 ElapsedTicks = 0u; /* elapsed will give timeout */

    /* Hard lock */
    if ((Base->CR & SWT_CR_HLK_MASK) != 0U)
    {
        Ret = SWT_IP_STATUS_ERROR;
    }
    else
    {
        /* Soft lock */
        if ((Base->CR & SWT_CR_SLK_MASK) != 0U)
        {
            /* Unlocks sequence */
            Base->SR = SWT_SR_WSC(SWT_IP_UNLOCK_VALUE1_U16);
            Base->SR = SWT_SR_WSC(SWT_IP_UNLOCK_VALUE2_U16);

            CurrentTicks = OsIf_GetCounter(SWT_IP_TIMEOUT_METHOD);

            /* Waits unlock complete */
            while (((Base->CR & SWT_CR_SLK_MASK) != 0U) && (ElapsedTicks < TimeoutTicks))
            {
                ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, SWT_IP_TIMEOUT_METHOD);
            }

            if (ElapsedTicks >= TimeoutTicks)
            {
                /* Soft lock is enabled again to avoid unlock sequence successfully after osif timeout expired */
                Swt_Ip_Lock(Base, SWT_IP_SOFTLOCK);
                Ret = SWT_IP_STATUS_ERROR;
            }
        }
        /* Unlock */
        else
        {
            /* Do nothing */
        }
    }

    return Ret;
}

#if (SWT_IP_SUPPORT_IACR == STD_ON)
static inline void Swt_Ip_WriteIACR(uint32 Address)
{
    static CREG_Type * const Swt_Ip_IARC[CREG_INSTANCE_COUNT] = IP_CREG_BASE_PTRS;

    /* Set SWT specific IAC register value */
    CREG_Type * const Base = Swt_Ip_IARC[0];
    
    Base->SWT_IACR = Address;
}

static void Swt_Ip_IACRConfig(const Swt_Ip_ConfigType * const ConfigPtr)
{
    Swt_Ip_ServiceModeType ServiceMode = ConfigPtr->eServiceMode;
    
     /* Initialize the instruction execution address */
    if ((SWT_IP_FA_EXE_MODE == ServiceMode) || (SWT_IP_IA_EXE_MODE == ServiceMode))
    {
        Swt_Ip_WriteIACR((uint32)&Swt_Ip_Service);
    }
    else
    {
        /* Work-around for NMI not being triggered */
        /* Write a value different from 0x0000000 to SWT_IACR */
        Swt_Ip_WriteIACR(0x00000008U);    
    }
}
#endif /* (SWT_IP_SUPPORT_IACR == STD_ON) */
                                  
static void Swt_Ip_RegisterConfig(SWT_Type * const Base,
                                     const Swt_Ip_ConfigType * const ConfigPtr)
{
    /* Temporary storage for control register value */
    uint32 TempCr = 0x00UL;    
    Swt_Ip_ServiceModeType ServiceMode = ConfigPtr->eServiceMode;
    
    /* Sets control configuration */
    TempCr = (SWT_IP_MAP(ConfigPtr->u8MapEnBitmask)
        | SWT_CR_SMD(ServiceMode)
        | SWT_CR_RIA(ConfigPtr->bEnResetOnInvalidAccess ? 1UL : 0UL)
        | SWT_CR_WND(ConfigPtr->bEnWindow ? 1UL : 0UL)
        | SWT_CR_ITR(ConfigPtr->bEnInterrupt ? 1UL : 0UL)
#if (defined(SWT_IP_HAS_STOP_MODE) && (SWT_IP_HAS_STOP_MODE == 1U))        
        | SWT_CR_STP(ConfigPtr->bEnRunInStopMode? 0UL : 1UL)
#endif    
        | SWT_CR_FRZ(ConfigPtr->bEnRunInDebugMode ? 0UL : 1UL));

    /* Write configuration to the SWT CR register*/
    Base->CR = TempCr;

    /* Clears interrupt flags */

    Base->IR = SWT_IR_TIF_MASK;

    /* Sets timeout value */
    Base->TO = ConfigPtr->u32TimeoutValue;

    /* Sets window value */
    if (ConfigPtr->bEnWindow)
    {
        Base->WN = ConfigPtr->u32WindowValue;
    }
    /* Sets initial service key value */
    if (SWT_IP_KS_SEQ_MODE == ServiceMode)
    {
        Base->SK = SWT_SK_SK(ConfigPtr->u16InitialKey);
    }
}

static Swt_Ip_StatusType Swt_Ip_CheckTimeout(uint32 u32TimeoutValue, uint32 u32WindowValue)
{
    /* Check timeout period is valid */
    Swt_Ip_StatusType Ret = SWT_IP_STATUS_SUCCESS;

    if (u32TimeoutValue < SWT_MIN_VALUE_TIMEOUT_U32)
    {
        Ret = SWT_IP_STATUS_ERROR;
    }

    if ((u32WindowValue != 0U) && (u32TimeoutValue <= u32WindowValue))
    {
        Ret = SWT_IP_STATUS_ERROR;
    }

    return Ret;
}

static inline void Swt_Ip_ServiceCmd(SWT_Type * const Base,
                                        uint16 ServiceKey)
{
    /* Services SWT instance */
    Base->SR = (Base->SR & ~SWT_SR_WSC_MASK) | SWT_SR_WSC(ServiceKey);
}

static inline uint16 Swt_Ip_ServiceKeyGen(const SWT_Type * const Base)
{
    /* Generates the next key used to service the SWT */
    return ((uint16)((((Base->SK & SWT_SK_SK_MASK) >> SWT_SK_SK_SHIFT) * 17U) + 3U));
}

#if (SWT_IP_DEINIT == STD_ON)
#if (SWT_IP_CLEAR_RESET_REQUEST == STD_ON)
static inline boolean Swt_Ip_RequestedReset(const SWT_Type * const Base)
{
    /* Get reset request flag */
    return (((Base->RRR & SWT_RRR_RRF_MASK)>> SWT_RRR_RRF_SHIFT) == 1U);
}
#endif
#endif

#if (SWT_IP_HAS_CLEAR_RESET == 1)
#if (SWT_IP_DEINIT == STD_ON)
static inline void Swt_Ip_ClearRequest(SWT_Type * const Base)
{
    /* Clear reset request flag */
    Base->RRR = SWT_RRR_RRF(1U);
}
#endif
#endif

static inline boolean Swt_Ip_GetIntFlag(const SWT_Type * const Base)
{
    /* Get interrupt flag */
    return ((Base->IR & SWT_IR_TIF_MASK) != 0U);
}

static inline boolean Swt_Ip_IsIntEnabled(const SWT_Type * const Base)
{
    /* Interrupt then reset request */
    return ((Base->CR & SWT_CR_ITR_MASK ) != 0U);
}

static inline void Swt_Ip_ClearIntFlag(SWT_Type * const Base)
{
    /* Clear interrupt flag */
    Base->IR &= SWT_IR_TIF_MASK;
}

/*==================================================================================================
*                                 GLOBAL FUNCTIONS PROTOTYPES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL FUNCTIONS
==================================================================================================*/

/**
* @Description    This function initializes SWT instance.
*
* @implements     Swt_Ip_Init_Activity
*/
Swt_Ip_StatusType Swt_Ip_Init(const uint32 Instance,
                            const Swt_Ip_ConfigType * const ConfigPtr)
{
    SWT_Type * const Base = Swt_Ip_sBase[Instance];
    Swt_Ip_StatusType Ret = SWT_IP_STATUS_SUCCESS;

#if (SWT_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(Instance < SWT_INSTANCE_COUNT);
    DevAssert(ConfigPtr != NULL_PTR);
#endif

    if (Swt_Ip_Unlock(Base) == SWT_IP_STATUS_ERROR)
    {
        Ret = SWT_IP_STATUS_ERROR;
    }
    else
    {
        boolean bSwtEnabled = Swt_Ip_IsEnable(Base);

        if (bSwtEnabled)
        {
            Swt_Ip_Disable(Base);
        }

        /* Verify if the timeout and window values are within accepted range */
        Ret = Swt_Ip_CheckTimeout(ConfigPtr->u32TimeoutValue, ConfigPtr->u32WindowValue);

        if (SWT_IP_STATUS_SUCCESS == Ret)
        {
            /* Configures the SWT instance */
            Swt_Ip_RegisterConfig(Base, ConfigPtr);
            
            #if (SWT_IP_SUPPORT_IACR == STD_ON)
            Swt_Ip_IACRConfig(ConfigPtr);
            #endif

            if (ConfigPtr->bEnInterrupt)
            {
                Swt_Ip_apCallbackPtr[Instance] = ConfigPtr->pfSwtCallback;
            }

            /* Enable the SWT timer */
            Swt_Ip_Enable(Base);

            /* Lock the SWT */
            Swt_Ip_Lock(Base, ConfigPtr->lockConfig);
        }
    }

    return Ret;
}

#if (SWT_IP_DEINIT == STD_ON)
/**
* @Description    This function de-initializes SWT instance.
*
* @implements     Swt_Ip_Deinit_Activity
*/
    Swt_Ip_StatusType Swt_Ip_Deinit(const uint32 Instance)
    {
        SWT_Type * const Base = Swt_Ip_sBase[Instance];
        Swt_Ip_StatusType Ret;

    #if (SWT_IP_DEV_ERROR_DETECT == STD_ON)
        DevAssert(Instance < SWT_INSTANCE_COUNT);
    #endif

        /* Unlocks SWT instance */
        Ret = Swt_Ip_Unlock(Base);

        if (Ret == SWT_IP_STATUS_SUCCESS)
        {
            /* Disable SWT timer */
            Swt_Ip_Disable(Base);

            /* Clears interrupt flag */
            Base->IR = SWT_IR_TIF_MASK;
            /* Resets timeout value */
            Base->TO = SWT_IP_TO_RESET_VALUE_U32;
            /* Resets window value */
            Base->WN = SWT_IP_WN_RESET_VALUE_U32;
            /* Resets service key value */
            Base->SK = SWT_IP_SK_RESET_VALUE_U16;
            /* Resets control register */
            Base->CR = SWT_IP_CR_RESET_VALUE_U32 & (~SWT_CR_WEN_MASK); /* make sure the SWT is disabled after reset */
            
            #if (SWT_IP_SUPPORT_IACR == STD_ON)
            /* Resets instruction address compare register */
            Swt_Ip_WriteIACR(0x00000000U);
            #endif
            
            /* Clear RRF bit in RRR register */
            #if (SWT_IP_HAS_CLEAR_RESET == 1)
            Swt_Ip_ClearRequest(Base);
            #endif
        }

        return Ret;
    }
#endif

/**
* @Description    This function services SWT instance.
*
* @implements     Swt_Ip_Service_Activity
*/
void Swt_Ip_Service(const uint32 Instance)
{
    SWT_Type * const Base = Swt_Ip_sBase[Instance];
    boolean BUnlockSequence = FALSE;

#if (SWT_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(Instance < SWT_INSTANCE_COUNT);
#endif
    
    SchM_Enter_Wdg_WDG_EXCLUSIVE_AREA_09();
    
    Swt_Ip_ServiceModeType ServiceMode = Swt_Ip_GetServiceMode(Base);

    switch (ServiceMode)
    {
        /* Keyed Service Sequence Mode */
        case SWT_IP_KS_SEQ_MODE:
            if (Swt_Ip_ServiceKeyGen(Base) == SWT_IP_UNLOCK_VALUE1_U16)
            {
                BUnlockSequence = TRUE;
            }
            
            Swt_Ip_ServiceCmd(Base, Swt_Ip_ServiceKeyGen(Base));
            
            if (Swt_Ip_ServiceKeyGen(Base) == SWT_IP_UNLOCK_VALUE1_U16)
            {
                BUnlockSequence = TRUE;
            }
            
            Swt_Ip_ServiceCmd(Base, Swt_Ip_ServiceKeyGen(Base));

            if ((SWT_IP_SOFTLOCK == Swt_Ip_GetLock(Base)) && (TRUE == BUnlockSequence))
            {
                /* Unlock key generated. Complete the unlock sequence and lock it again. */
                Swt_Ip_ServiceCmd(Base, SWT_IP_UNLOCK_VALUE2_U16);
                Swt_Ip_Lock(Base, SWT_IP_SOFTLOCK);
            }
            break;
        /* Fixed Service Sequence Mode */
        case SWT_IP_FS_SEQ_MODE:
            Swt_Ip_ServiceCmd(Base, SWT_IP_FIXED_SERVICE_VALUE1_U16);
            Swt_Ip_ServiceCmd(Base, SWT_IP_FIXED_SERVICE_VALUE2_U16);
            break;
        default:
            /* No register write required for servicing the watchdog.
            The watchdog is serviced when executing code at the address loaded into the designated IAC register */
            break;
    }

    SchM_Exit_Wdg_WDG_EXCLUSIVE_AREA_09();
}

/**
* @Description    This function configure SWT instance.
*
* @implements     Swt_Ip_Config_Activity
*/
Swt_Ip_StatusType Swt_Ip_Config(const uint32 Instance,
                              const Swt_Ip_ConfigType * const ConfigPtr)
{
    SWT_Type * const Base = Swt_Ip_sBase[Instance];
    Swt_Ip_StatusType Ret;

#if (SWT_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(Instance < SWT_INSTANCE_COUNT);
    DevAssert(ConfigPtr != NULL_PTR);
#endif

    Ret = Swt_Ip_CheckTimeout(ConfigPtr->u32TimeoutValue, ConfigPtr->u32WindowValue);

    if (SWT_IP_STATUS_SUCCESS == Ret)
    {
        if (SWT_IP_STATUS_ERROR == Swt_Ip_Unlock(Base))
        {
            Ret = SWT_IP_STATUS_ERROR;
        }
        else
        {
            boolean bSwtEnabled = Swt_Ip_IsEnable(Base);

            if (bSwtEnabled)
            {
                Swt_Ip_Disable(Base);
            }

            /* Configures the SWT instance */
            Swt_Ip_RegisterConfig(Base, ConfigPtr);

            /* If the timer was enebled, enable it after configuration is done */
            if (bSwtEnabled)
            {
                Swt_Ip_Enable(Base);
            }
            /* Lock the SWT */
            Swt_Ip_Lock(Base, ConfigPtr->lockConfig);
        }
    }

    return Ret;
}

/**
* @Description    This function sets the timeout value for the SWT instance.
*
* @implements     Swt_Ip_SetTimeout_Activity
*/
Swt_Ip_StatusType Swt_Ip_SetTimeout(const uint32 Instance,
                                    const uint32 TimeoutValue, const uint32 WindowValue)
{
    SWT_Type * const Base = Swt_Ip_sBase[Instance];
    Swt_Ip_LockType LockConfig;
    Swt_Ip_StatusType Ret;

#if (SWT_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(Instance < SWT_INSTANCE_COUNT);
#endif

    LockConfig = Swt_Ip_GetLock(Base);
    Ret = Swt_Ip_CheckTimeout(TimeoutValue, WindowValue);

    if (SWT_IP_STATUS_SUCCESS == Ret)
    {
        if (SWT_IP_STATUS_ERROR == Swt_Ip_Unlock(Base))
        {
            Ret = SWT_IP_STATUS_ERROR;
        }
        else
        {
            /* Stop the SWT timer */
            Swt_Ip_Disable(Base);

            /* Sets timeout value */
            Base->TO = TimeoutValue;

            /* Sets window value */
            if (WindowValue > 0U)
            {
                Base->WN = WindowValue;
                /* Enable Windowed Mode */
                Base->CR |= SWT_CR_WND_MASK;
            }
            else
            {
                Base->CR &= ~SWT_CR_WND_MASK;
            }

            /* Start the SWT timer */
            Swt_Ip_Enable(Base);

            /* Lock the SWT */
            Swt_Ip_Lock(Base, LockConfig);
        }
    }

    return Ret;
}

/**
* @Description    This function starts the timer for the SWT instance.
*
* @implements     Swt_Ip_StartTimer_Activity
*/
Swt_Ip_StatusType Swt_Ip_StartTimer(const uint32 Instance)
{
    SWT_Type * const Base = Swt_Ip_sBase[Instance];
    Swt_Ip_LockType LockConfig;
    Swt_Ip_StatusType Ret;

#if (SWT_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(Instance < SWT_INSTANCE_COUNT);
#endif

    LockConfig = Swt_Ip_GetLock(Base);
    /* Unlocks the SWT */
    SchM_Enter_Wdg_WDG_EXCLUSIVE_AREA_10();
    Ret = Swt_Ip_Unlock(Base);

    if (SWT_IP_STATUS_SUCCESS == Ret)
    {
        /* Start the SWT timer */
        Swt_Ip_Enable(Base);

        Swt_Ip_Lock(Base, LockConfig);
    }
    SchM_Exit_Wdg_WDG_EXCLUSIVE_AREA_10();

    return Ret;
}

/**
* @Description    This function stops the timer for the SWT instance.
*
* @implements     Swt_Ip_StopTimer_Activity
*/
Swt_Ip_StatusType Swt_Ip_StopTimer(const uint32 Instance)
{
    SWT_Type * const Base = Swt_Ip_sBase[Instance];
    Swt_Ip_LockType LockConfig;
    Swt_Ip_StatusType Ret;

#if (SWT_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(Instance < SWT_INSTANCE_COUNT);
#endif

    LockConfig = Swt_Ip_GetLock(Base);
    /* Unlocks the SWT */
    SchM_Enter_Wdg_WDG_EXCLUSIVE_AREA_11();
    Ret = Swt_Ip_Unlock(Base);

    if (SWT_IP_STATUS_SUCCESS == Ret)
    {
        /* Start the SWT timer */
        Swt_Ip_Disable(Base);

        Swt_Ip_Lock(Base, LockConfig);
    }
    SchM_Exit_Wdg_WDG_EXCLUSIVE_AREA_11();

    return Ret;
}

#if (SWT_IP_HAS_CLEAR_RESET == 1)
#if (SWT_IP_DEINIT == STD_ON)
#if (SWT_IP_CLEAR_RESET_REQUEST == STD_ON)
/**
* @Description    This function clears reset request.
*
* @return                             Swt_Ip_StatusType.
* @retval  SWT_IP_STATUS_ERROR        returned if SWT has not requested a reset.
* @retval  SWT_IP_STATUS_SUCCESS      Clear reset request successfully.
* @implements     Swt_Ip_ClearResetRequest_Activity
*/
    Swt_Ip_StatusType Swt_Ip_ClearResetRequest(const uint32 Instance)
    {
        SWT_Type * const Base = Swt_Ip_sBase[Instance];
        Swt_Ip_StatusType Ret = SWT_IP_STATUS_SUCCESS;

        uint32 TimeoutTicks = OsIf_MicrosToTicks(SWT_IP_OSIF_TIMEOUT_VAL, SWT_IP_TIMEOUT_METHOD);
        uint32 CurrentTicks = 0u; /* initialize current counter */
        uint32 ElapsedTicks = 0u; /* elapsed will give timeout */

    #if (SWT_IP_DEV_ERROR_DETECT == STD_ON)
        DevAssert(Instance < SWT_INSTANCE_COUNT);
        
        /* Can the SWT instance be reset without system reset */
        DevAssert((SWT_IP_RRR_SUPPORT & (1UL << Instance)) != 0UL);
    #endif

        if (Swt_Ip_RequestedReset(Base))
        {   /* Reset the SWT instance */
            Swt_Ip_ClearRequest(Base);

            CurrentTicks = OsIf_GetCounter(SWT_IP_TIMEOUT_METHOD);

            /* Wait for the SWT instance to finish reseting */
            while ((Swt_Ip_RequestedReset(Base)) && (ElapsedTicks < TimeoutTicks))
            {
                ElapsedTicks += OsIf_GetElapsed(&CurrentTicks, SWT_IP_TIMEOUT_METHOD);
            }

            if((ElapsedTicks >= TimeoutTicks) && (Swt_Ip_RequestedReset(Base)))
            {
                Ret = SWT_IP_STATUS_TIMEOUT;
            }

        }
        else
        {   /* SWT has not requested a reset */
            Ret = SWT_IP_STATUS_ERROR;
        }

        return Ret;
    }
#endif /* SWT_IP_DEINIT == STD_ON */
#endif /* SWT_IP_DEINIT == STD_ON */
#endif /* SWT_IP_HAS_CLEAR_RESET == 1 */

/**
* @Description    This function handles the swt interrupt request.
*
* @implements     Swt_Ip_IrqHandler_Activity
*/
void Swt_Ip_IrqHandler(uint32 Instance)
{
    SWT_Type * const Base = Swt_Ip_sBase[Instance];

#if (SWT_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(Instance < SWT_INSTANCE_COUNT);
#endif

    if  (Swt_Ip_GetIntFlag(Base))
    {
        Swt_Ip_ClearIntFlag(Base);

        if ((Swt_Ip_apCallbackPtr[Instance] != ((void *)0)) && (Swt_Ip_IsIntEnabled(Base)))
        {
            Swt_Ip_apCallbackPtr[Instance]();
        }
    }
}

#if (SWT_IP_SUPPORT_IACR == STD_ON)
#if (SWT_IP_ENABLE_SET_SERVICE_ADDRESS == STD_ON)
/**
* @Description    Sets the Instruction Address Compare Register for SWT instance.
* @param[in]      Instance The SWT peripheral instance number
* @param[in]      Address The Instruction Execution Address
* @return         Swt_Ip_StatusType
* @retval         SWT_IP_STATUS_SUCCESS     Operation was successful.
* @retval         SWT_IP_STATUS_ERROR       Operation failed due to SWT being started
*                                           or it is not in Incremental mode.
* @implements     Swt_Ip_SetServiceAddress_Activity
*/
Swt_Ip_StatusType Swt_Ip_SetServiceAddress(const uint32 Instance, const uint32 Address)
{
    const SWT_Type * const Base = Swt_Ip_sBase[Instance];
    Swt_Ip_StatusType Ret = SWT_IP_STATUS_SUCCESS;
    
    #if (SWT_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(Instance < SWT_INSTANCE_COUNT);
    #endif
    
    if (Swt_Ip_IsEnable(Base))
    {
        if (Swt_Ip_GetServiceMode(Base) == SWT_IP_IA_EXE_MODE)
        {
             /* Incremental Address Execution */
             /* Service the watchdog by executing code at the address loaded into the designated IAC register */
            Swt_Ip_WriteIACR(Address);
        }
        else
        {
            /* Fixed Address Execution */
            /*  The designated IAC register cannot be updated while the watchdog is enabled */
            Ret = SWT_IP_STATUS_ERROR;
        }
    }
    else
    {
        /* Incremental Address Execution or Fixed Address Execution */
        /* When WDG is disable, the address can be loaded into the designated IAC register */
        Swt_Ip_WriteIACR(Address);
    }

    return Ret;
}
#endif /* (SWT_IP_SUPPORT_IACR == STD_ON) */
#endif /* SWT_IP_ENABLE_SET_SERVICE_ADDRESS == STD_ON */

#ifdef WDG_ROM
    #if (WDG_ROM == 1U)
        #define WDG_STOP_SEC_CODE
    #endif
#else
    #if (WDG_RAM == 0U)
        #define WDG_STOP_SEC_RAMCODE
    #endif
#endif

#include "Wdg_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
