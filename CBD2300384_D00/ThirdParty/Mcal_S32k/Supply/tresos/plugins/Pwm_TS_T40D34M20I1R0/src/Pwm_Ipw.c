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
*   @file       Pwm_Ipw.c
*
*   @addtogroup pwm_driver Pwm Driver
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
#include "StandardTypes.h"
#include "Pwm_Ipw.h"
#include "Emios_Pwm_Ip.h"

#if (PWM_FLEXIO_USED == STD_ON)
    #include "Flexio_Pwm_Ip.h"
    #include "Flexio_Pwm_Ip_HwAccess.h"
#endif

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define PWM_IPW_VENDOR_ID_C                     43
#define PWM_IPW_AR_RELEASE_MAJOR_VERSION_C      4
#define PWM_IPW_AR_RELEASE_MINOR_VERSION_C      4
#define PWM_IPW_AR_RELEASE_REVISION_VERSION_C   0
#define PWM_IPW_SW_MAJOR_VERSION_C              2
#define PWM_IPW_SW_MINOR_VERSION_C              0
#define PWM_IPW_SW_PATCH_VERSION_C              1


/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if header file and StandardTypes.h are of the same AUTOSAR version */
    #if ((PWM_IPW_AR_RELEASE_MAJOR_VERSION_C != STD_AR_RELEASE_MAJOR_VERSION) || \
         (PWM_IPW_AR_RELEASE_MINOR_VERSION_C != STD_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Pwm_Ipw.h and StandardTypes.h are different"
    #endif
#endif

/* Check if source file and PWM_Ipw.h file are of the same vendor */
#if (PWM_IPW_VENDOR_ID_C != PWM_IPW_VENDOR_ID)
    #error "Vendor IDs of Pwm_Ipw.c and PWM_Ipw.h are different."
#endif

/* Check if source file and PWM_Ipw.h file are of the same AUTOSAR version */
#if ((PWM_IPW_AR_RELEASE_MAJOR_VERSION_C    != PWM_IPW_AR_RELEASE_MAJOR_VERSION) || \
     (PWM_IPW_AR_RELEASE_MINOR_VERSION_C    != PWM_IPW_AR_RELEASE_MINOR_VERSION) || \
     (PWM_IPW_AR_RELEASE_REVISION_VERSION_C != PWM_IPW_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR version numbers of Pwm_Ipw.c and PWM_Ipw.h are different."
#endif

/* Check if source file and PWM_Ipw.h file are of the same Software version */
#if ((PWM_IPW_SW_MAJOR_VERSION_C != PWM_IPW_SW_MAJOR_VERSION)  || \
     (PWM_IPW_SW_MINOR_VERSION_C != PWM_IPW_SW_MINOR_VERSION)  || \
     (PWM_IPW_SW_PATCH_VERSION_C != PWM_IPW_SW_PATCH_VERSION))
    #error "Software version numbers of Pwm_Ipw.c and PWM_Ipw.h are different."
#endif

/* Check if source file and Emios_Pwm_Ip.h file are of the same vendor */
#if (PWM_IPW_VENDOR_ID_C != EMIOS_PWM_IP_VENDOR_ID)
    #error "Vendor IDs of Pwm_Ipw.c and Emios_Pwm_Ip.h are different."
#endif

/* Check if source file and Emios_Pwm_Ip.h file are of the same AUTOSAR version */
#if ((PWM_IPW_AR_RELEASE_MAJOR_VERSION_C    != EMIOS_PWM_IP_AR_RELEASE_MAJOR_VERSION) || \
     (PWM_IPW_AR_RELEASE_MINOR_VERSION_C    != EMIOS_PWM_IP_AR_RELEASE_MINOR_VERSION) || \
     (PWM_IPW_AR_RELEASE_REVISION_VERSION_C != EMIOS_PWM_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR version numbers of Pwm_Ipw.c and Emios_Pwm_Ip.h are different."
#endif

/* Check if source file and Emios_Pwm_Ip.h file are of the same Software version */
#if ((PWM_IPW_SW_MAJOR_VERSION_C != EMIOS_PWM_IP_SW_MAJOR_VERSION)  || \
     (PWM_IPW_SW_MINOR_VERSION_C != EMIOS_PWM_IP_SW_MINOR_VERSION)  || \
     (PWM_IPW_SW_PATCH_VERSION_C != EMIOS_PWM_IP_SW_PATCH_VERSION))
    #error "Software version numbers of Pwm_Ipw.c and Emios_Pwm_Ip.h are different."
#endif

#if (PWM_FLEXIO_USED == STD_ON)
    /* Check if source file and Flexio_Pwm_Ip.h file are of the same vendor */
    #if (PWM_IPW_VENDOR_ID_C != FLEXIO_PWM_IP_VENDOR_ID)
        #error "Vendor IDs of Pwm_Ipw.c and Flexio_Pwm_Ip.h are different."
    #endif

    /* Check if source file and Flexio_Pwm_Ip.h file are of the same AUTOSAR version */
    #if ((PWM_IPW_AR_RELEASE_MAJOR_VERSION_C    != FLEXIO_PWM_IP_AR_RELEASE_MAJOR_VERSION) || \
         (PWM_IPW_AR_RELEASE_MINOR_VERSION_C    != FLEXIO_PWM_IP_AR_RELEASE_MINOR_VERSION) || \
         (PWM_IPW_AR_RELEASE_REVISION_VERSION_C != FLEXIO_PWM_IP_AR_RELEASE_REVISION_VERSION))
        #error "AUTOSAR version numbers of Pwm_Ipw.c and Flexio_Pwm_Ip.h are different."
    #endif

    /* Check if source file and Flexio_Pwm_Ip.h file are of the same Software version */
    #if ((PWM_IPW_SW_MAJOR_VERSION_C != FLEXIO_PWM_IP_SW_MAJOR_VERSION)  || \
         (PWM_IPW_SW_MINOR_VERSION_C != FLEXIO_PWM_IP_SW_MINOR_VERSION)  || \
         (PWM_IPW_SW_PATCH_VERSION_C != FLEXIO_PWM_IP_SW_PATCH_VERSION))
        #error "Software version numbers of Pwm_Ipw.c and Flexio_Pwm_Ip.h are different."
    #endif
    
    /* Check if source file and Flexio_Pwm_Ip_HwAccess.h file are of the same vendor */
    #if (PWM_IPW_VENDOR_ID_C != FLEXIO_PWM_IP_HWACCESS_VENDOR_ID)
        #error "Vendor IDs of Pwm_Ipw.c and Flexio_Pwm_Ip_HwAccess.h are different."
    #endif

    /* Check if source file and Flexio_Pwm_Ip_HwAccess.h file are of the same AUTOSAR version */
    #if ((PWM_IPW_AR_RELEASE_MAJOR_VERSION_C    != FLEXIO_PWM_IP_HWACCESS_AR_RELEASE_MAJOR_VERSION) || \
         (PWM_IPW_AR_RELEASE_MINOR_VERSION_C    != FLEXIO_PWM_IP_HWACCESS_AR_RELEASE_MINOR_VERSION) || \
         (PWM_IPW_AR_RELEASE_REVISION_VERSION_C != FLEXIO_PWM_IP_HWACCESS_AR_RELEASE_REVISION_VERSION))
        #error "AUTOSAR version numbers of Pwm_Ipw.c and Flexio_Pwm_Ip_HwAccess.h are different."
    #endif

    /* Check if source file and Flexio_Pwm_Ip_HwAccess.h file are of the same Software version */
    #if ((PWM_IPW_SW_MAJOR_VERSION_C != FLEXIO_PWM_IP_HWACCESS_SW_MAJOR_VERSION)  || \
         (PWM_IPW_SW_MINOR_VERSION_C != FLEXIO_PWM_IP_HWACCESS_SW_MINOR_VERSION)  || \
         (PWM_IPW_SW_PATCH_VERSION_C != FLEXIO_PWM_IP_HWACCESS_SW_PATCH_VERSION))
        #error "Software version numbers of Pwm_Ipw.c and Flexio_Pwm_Ip_HwAccess.h are different."
    #endif
#endif

/*==================================================================================================
*                           LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                          LOCAL MACROS
==================================================================================================*/
#if ((PWM_PARAM_CHECK == STD_ON) && ((PWM_SET_PHASE_SHIFT_API == STD_ON) || (PWM_SET_PHASE_SHIFT_NO_UPDATE_API == STD_ON)))
/**
* @brief        Duty cycle 50%
*
*/
#define PWM_HALF_PERIOD_DUTY            ((uint16)0x4000U)
#endif

/*==================================================================================================
*                                         LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                         LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                        GLOBAL VARIABLES
==================================================================================================*/
#define PWM_START_SEC_VAR_CLEARED_16
#include "Pwm_MemMap.h"

#if (PWM_EMIOS_USED == STD_ON)
/** @brief Array to save the duty cycle of the emios channel */
uint16 Pwm_Ipw_aEmiosDutyCycle[EMIOS_PWM_IP_INSTANCE_COUNT][EMIOS_PWM_IP_CHANNEL_COUNT];
#endif

#if (PWM_FLEXIO_USED == STD_ON)
/** @brief Array to save the duty cycle of the flexio channel */
uint16 Pwm_Ipw_aFlexioDutyCycle[FLEXIO_PWM_IP_INSTANCE_COUNT][FLEXIO_PWM_IP_CHANNEL_COUNT];  
#endif

#define PWM_STOP_SEC_VAR_CLEARED_16
#include "Pwm_MemMap.h"
/*==================================================================================================
*                                    LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && (PWM_NOTIFICATION_SUPPORTED == STD_ON))
#if (PWM_EMIOS_USED == STD_ON)
static Std_ReturnType Pwm_Ipw_EmiosValidateNotification(Pwm_EdgeNotificationType       Notification,
                                                        const Pwm_IpwChannelConfigType * const IpConfig);
#endif /* (PWM_EMIOS_USED == STD_ON) */
#endif /* ((PWM_DEV_ERROR_DETECT == STD_ON) && (PWM_NOTIFICATION_SUPPORTED == STD_ON)) */

/*==================================================================================================
*                                         LOCAL FUNCTIONS
==================================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && (PWM_NOTIFICATION_SUPPORTED == STD_ON))
#if (PWM_EMIOS_USED == STD_ON)
static Std_ReturnType Pwm_Ipw_EmiosValidateNotification(Pwm_EdgeNotificationType       Notification,
                                                        const Pwm_IpwChannelConfigType * const IpConfig)
{
    /** @brief  Variable to store the value returned by the function */
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;

    if(
        ((PWM_FALLING_EDGE == Notification) && (EMIOS_PWM_IP_ACTIVE_LOW == ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->OutputPolarity)) ||
        ((PWM_RISING_EDGE == Notification) && (EMIOS_PWM_IP_ACTIVE_HIGH == ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->OutputPolarity))
      )
    {
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
        switch(((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->Mode)
        {
            case EMIOS_PWM_IP_MODE_OPWFMB_FLAG_BOTH:
            case EMIOS_PWM_IP_MODE_OPWMCB_TRAIL_EDGE_FLAG_BOTH:
            case EMIOS_PWM_IP_MODE_OPWMCB_LEAD_EDGE_FLAG_BOTH:
            case EMIOS_PWM_IP_MODE_OPWMB_FLAG_BOTH:
                RetVal = (PWM_BOTH_EDGES == Notification)? (Std_ReturnType)E_OK : (Std_ReturnType)E_NOT_OK;
                break;
            case EMIOS_PWM_IP_MODE_OPWFMB_FLAG:
            case EMIOS_PWM_IP_MODE_OPWMCB_TRAIL_EDGE_FLAG:
            case EMIOS_PWM_IP_MODE_OPWMCB_LEAD_EDGE_FLAG:
            case EMIOS_PWM_IP_MODE_OPWMB_FLAG:
                RetVal = (PWM_BOTH_EDGES == Notification)? (Std_ReturnType)E_NOT_OK : (Std_ReturnType)E_OK;
                break;
            default:
                /* Do Nothing */
                break;
        }
    }
    return RetVal;
}
#endif /* (PWM_EMIOS_USED == STD_ON) */
#endif /* ((PWM_DEV_ERROR_DETECT == STD_ON) && (PWM_NOTIFICATION_SUPPORTED == STD_ON)) */

/*==================================================================================================
*                                        GLOBAL FUNCTIONS
==================================================================================================*/
#define PWM_START_SEC_CODE
#include "Pwm_MemMap.h"

/**
* @brief        Pwm_Ipw_Init
* @details      This function call the supported IP initialization functions.
*
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       void
*
*/
void Pwm_Ipw_Init(const Pwm_IpwChannelConfigType * const IpConfig)
{
    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            if (NULL_PTR != IpConfig->EmiosChConfig)
            {
                /* Call Emios IP initialize function */
                Emios_Pwm_Ip_InitChannel(IpConfig->ChannelInstanceId, (const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig);
                /* Duty cycle of Emios Ip */
                Pwm_Ipw_aEmiosDutyCycle[IpConfig->ChannelInstanceId][((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId] = IpConfig->ChannelDutyCycle;
            }
            break;
#endif
#if (PWM_FLEXIO_USED == STD_ON)
        case PWM_CHANNEL_FLEXIO:
            if (NULL_PTR != IpConfig->FlexioChConfig)
            {
                /* Call Flexio IP initialize function */
                (void)Flexio_Pwm_Ip_InitChannel(IpConfig->ChannelInstanceId, (const Flexio_Pwm_Ip_ChannelConfigType *)IpConfig->FlexioChConfig);
                /* Duty cycle of Flexio Ip */
                Pwm_Ipw_aFlexioDutyCycle[IpConfig->ChannelInstanceId][(IpConfig->FlexioChConfig)->TimerId] = IpConfig->ChannelDutyCycle;
            }
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            break;
        default:
            /* Nothing to do. */
            break;
    }
}

/*===============================================================================================*/
#if (PWM_DE_INIT_API == STD_ON)
/**
* @brief        Pwm_Ipw_DeInit
* @details      This function call the supported IP de-initialization functions.
*
* @param[in]    IpConfig            Pointer to PWM top configuration structure
* @param[in]    ChannelIdleState    The state of the channel output in idle mode
*
* @return       void
*
*/
void Pwm_Ipw_DeInit(const Pwm_IpwChannelConfigType * const IpConfig, Pwm_OutputStateType ChannelIdleState)
{

#if (PWM_EMIOS_USED == STD_ON)
    Emios_Pwm_Ip_OutputStateType EmiosChState = EMIOS_PWM_IP_OUTPUT_STATE_LOW;
    /* Idle state is true when requested level is PWM_HIGH*/
    EmiosChState = (PWM_HIGH == ChannelIdleState) ? EMIOS_PWM_IP_OUTPUT_STATE_HIGH : EMIOS_PWM_IP_OUTPUT_STATE_LOW;
#endif
    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            if (NULL_PTR != IpConfig->EmiosChConfig)
            {
                Emios_Pwm_Ip_DeInitChannel(IpConfig->ChannelInstanceId, ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);
                /* Set output to Idle state */
                Emios_Pwm_Ip_SetOutputState(IpConfig->ChannelInstanceId,
                                           ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                           EmiosChState);
                Pwm_Ipw_aEmiosDutyCycle[IpConfig->ChannelInstanceId][((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId] = 0U;
            }
            break;
#endif
#if (PWM_FLEXIO_USED == STD_ON)
        case PWM_CHANNEL_FLEXIO:
            if (NULL_PTR != IpConfig->FlexioChConfig)
            {
                (void)Flexio_Pwm_Ip_DeInitChannel(IpConfig->ChannelInstanceId, ((const Flexio_Pwm_Ip_ChannelConfigType *)IpConfig->FlexioChConfig)->TimerId);
                /* Set output to Idle state */
                (void)Flexio_Pwm_Ip_ForceOuputLevel(IpConfig->ChannelInstanceId,
                                                    IpConfig->FlexioChConfig->TimerId,
                                                    (PWM_HIGH == ChannelIdleState));
                /* Duty cycle of Flexio Ip */
                Pwm_Ipw_aFlexioDutyCycle[IpConfig->ChannelInstanceId][(IpConfig->FlexioChConfig)->TimerId] = 0U;
            }
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            (void)ChannelIdleState;
            break;
        default:
            /* Nothing to do. */
            (void)ChannelIdleState;
            break;
    }
}
#endif /* PWM_DE_INIT_API */

/*===============================================================================================*/
#if (PWM_SET_DUTY_CYCLE_API == STD_ON)
/**
* @brief        Pwm_Ipw_SetDutyCycle
* @details      This function will call SetDutyCycle function for the corresponding IP of PwmChannel
*
* @param[in]    DutyCycle      Duty cycle value 0x0000 for 0% ... 0x8000 for 100%
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       Std_ReturnType
*               E_NOT_OK        The duty cycle could not be set
*               E_OK            The duty cycle is set to the new value
*
*/
Std_ReturnType Pwm_Ipw_SetDutyCycle(uint16                         DutyCycle,
                                    const Pwm_IpwChannelConfigType * const IpConfig)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
#if (PWM_EMIOS_USED == STD_ON)
    uint16 EmiosChPeriod = 0U;
    uint16 EmiosChDuty = 0U;
#endif

#if (PWM_FLEXIO_USED == STD_ON)
    uint16 FlexioChDuty = 0U;
    uint16 FlexioPeriod = 0U;
#endif

    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            /* Get the period of the chanel  */
            EmiosChPeriod = Emios_Pwm_Ip_GetPeriod(IpConfig->ChannelInstanceId,
                                                  ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);
                                                  
            EmiosChDuty = (uint16)(((uint32)EmiosChPeriod * DutyCycle) / 0x8000U);
            /* Call Emios IP function */
            RetVal = (Std_ReturnType)Emios_Pwm_Ip_SetDutyCycle(IpConfig->ChannelInstanceId,
                                                               ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                                               EmiosChDuty);
            /* Call Emios api for setting the polarity and mode */
            Emios_Pwm_Ip_SetOutputToNormal(IpConfig->ChannelInstanceId,
                                            ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                            DutyCycle,
                                            ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->OutputPolarity,
                                            ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->Mode
                                           );
            Pwm_Ipw_aEmiosDutyCycle[IpConfig->ChannelInstanceId][((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId] = DutyCycle;

            break;
#endif
#if (PWM_FLEXIO_USED == STD_ON)
        case PWM_CHANNEL_FLEXIO:
            FlexioPeriod = Flexio_Pwm_Ip_GetPeriod(IpConfig->ChannelInstanceId,
                                                   IpConfig->FlexioChConfig->TimerId);
            /* Calculate new duty value in ticks */
            FlexioChDuty = (uint16)(((uint32)FlexioPeriod * DutyCycle)/0x8000U);
            /* Call Flexio api for setting the period and duty cycle */
            (void)Flexio_Pwm_Ip_UpdatePeriodDuty(IpConfig->ChannelInstanceId,
                                                 IpConfig->FlexioChConfig->TimerId,
                                                 FlexioPeriod,
                                                 FlexioChDuty);
            /* Duty cycle of Flexio Ip */
            Pwm_Ipw_aFlexioDutyCycle[IpConfig->ChannelInstanceId][(IpConfig->FlexioChConfig)->TimerId] = DutyCycle;
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            break;
        default:
            /* Nothing to do. */
            /* Avoid compiler warning */
            (void) DutyCycle;
            (void) IpConfig;
            break;
    }

    return RetVal;
}
#endif /* PWM_SET_DUTY_CYCLE_API */

/*===============================================================================================*/
#if (PWM_SET_PERIOD_AND_DUTY_API == STD_ON)
/**
* @brief        Pwm_Ipw_SetPeriodAndDuty
* @details      This function will call SetPeriodAndDuty function for the corresponding IP of PwmChannel
*
* @param[in]    Period         Period value for this channel
* @param[in]    DutyCycle      Duty cycle value 0x0000 for 0% ... 0x8000 for 100%
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       Std_ReturnType
*               E_NOT_OK        The period and duty cycle could not be set
*               E_OK            The period and duty cycle is set to the new value
*
*/
Std_ReturnType Pwm_Ipw_SetPeriodAndDuty(Pwm_Ipw_PeriodType              Period,
                                        uint16                          DutyCycle,
                                        const Pwm_IpwChannelConfigType * const IpConfig)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
#if (PWM_EMIOS_USED == STD_ON)
    uint16 EmiosChDuty = 0U;
#endif
#if (PWM_FLEXIO_USED == STD_ON)
    uint16 FlexioChDuty = 0U;
#endif

    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            /* Call Emios api for setting the period */
            Emios_Pwm_Ip_SetPeriod(IpConfig->ChannelInstanceId,
                                   ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                   Period);

            /* Calculate new duty value in ticks */
            EmiosChDuty = (uint16)(((uint32)Period * DutyCycle)/0x8000U);
            /* Call Emios api for setting the duty cycle */
            RetVal = (Std_ReturnType)Emios_Pwm_Ip_SetDutyCycle(IpConfig->ChannelInstanceId,
                                                               ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                                               EmiosChDuty);
            /* Call Emios api for setting the polarity and mode */
            Emios_Pwm_Ip_SetOutputToNormal(IpConfig->ChannelInstanceId,
                                            ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                            DutyCycle,
                                            ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->OutputPolarity,
                                            ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->Mode
                                           );
            Pwm_Ipw_aEmiosDutyCycle[IpConfig->ChannelInstanceId][((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId] = DutyCycle;
            break;
#endif
#if (PWM_FLEXIO_USED == STD_ON)
        case PWM_CHANNEL_FLEXIO:
            /* Calculate new duty value in ticks */
            FlexioChDuty = (uint16)(((uint32)Period * DutyCycle)/0x8000U);
            /* Call Flexio api for setting the period */
            RetVal = (Std_ReturnType)Flexio_Pwm_Ip_UpdatePeriodDuty(IpConfig->ChannelInstanceId,
                                                                    IpConfig->FlexioChConfig->TimerId,
                                                                    Period,
                                                                    FlexioChDuty);
            /* Duty cycle of Flexio Ip */
            Pwm_Ipw_aFlexioDutyCycle[IpConfig->ChannelInstanceId][(IpConfig->FlexioChConfig)->TimerId] = DutyCycle;
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            break;
        default:
            /* Nothing to do. */
            /* Avoid compiler warning */
            (void) Period;
            (void) DutyCycle;
            (void) IpConfig;
            break;
    }

    return RetVal;
}
#endif /* PWM_SET_PERIOD_AND_DUTY_API */

/*===============================================================================================*/
#if (PWM_SET_OUTPUT_TO_IDLE_API == STD_ON)
/**
* @brief        Pwm_Ipw_SetOutputToIdle
* @details      This function will call SetOutputToIdle function for the corresponding IP of PwmChannel
*
* @param[in]    IdleState      The Level that the hw channel must drive on the ouput pin
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       void
*
*/
void Pwm_Ipw_SetOutputToIdle(const Pwm_OutputStateType IdleState, const Pwm_IpwChannelConfigType * const IpConfig)
{
#if (PWM_EMIOS_USED == STD_ON)
    Emios_Pwm_Ip_OutputStateType EmiosChState = EMIOS_PWM_IP_OUTPUT_STATE_LOW;
    /* Idle state is true when requested level is PWM_HIGH*/
    EmiosChState = (PWM_HIGH == IdleState) ? EMIOS_PWM_IP_OUTPUT_STATE_HIGH : EMIOS_PWM_IP_OUTPUT_STATE_LOW;
#endif

    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            Emios_Pwm_Ip_SetOutputState(IpConfig->ChannelInstanceId,
                                       ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                        EmiosChState);
            break;
#endif
#if (PWM_FLEXIO_USED == STD_ON)
        case PWM_CHANNEL_FLEXIO:
            (void)Flexio_Pwm_Ip_ForceOuputLevel(IpConfig->ChannelInstanceId,
                                          IpConfig->FlexioChConfig->TimerId,
                                          (PWM_HIGH == IdleState));
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            (void) IdleState;
            break;
        default:
            /* Nothing to do. */
            /* Avoid compiler warning */
            (void) IdleState;
            break;
    }
}
#endif /* PWM_SET_OUTPUT_TO_IDLE_API */

/*===============================================================================================*/
#if (PWM_GET_OUTPUT_STATE_API == STD_ON)
/**
* @brief        Pwm_Ipw_GetOutputState
* @details      This function will call GetOutputState function for the corresponding IP of PwmChannel
*
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       Pwm_OutputStateType
*
*/
Pwm_OutputStateType Pwm_Ipw_GetOutputState(const Pwm_IpwChannelConfigType * const IpConfig)
{
    /** @brief  Store return value */
    Pwm_OutputStateType RetVal = PWM_LOW;

#if (PWM_EMIOS_USED == STD_ON)
    Emios_Pwm_Ip_OutputStateType EmiosChState = EMIOS_PWM_IP_OUTPUT_STATE_LOW;
#endif
#if (PWM_FLEXIO_USED == STD_ON)
    boolean FlexioChState = FALSE;
#endif

    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
        /* Call Emios IP function */
            EmiosChState = Emios_Pwm_Ip_GetOutputState(IpConfig->ChannelInstanceId,
                                                      ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);

            if (EMIOS_PWM_IP_OUTPUT_STATE_LOW == EmiosChState)
            {
                RetVal = PWM_LOW;
            }
            else
            {
                RetVal = PWM_HIGH;
            }
            break;
#endif
#if (PWM_FLEXIO_USED == STD_ON)
        case PWM_CHANNEL_FLEXIO:
            /* Call Flexio IP function */
            FlexioChState = Flexio_Pwm_Ip_GetOutputState(IpConfig->ChannelInstanceId,
                                                         IpConfig->FlexioChConfig->TimerId);

            if (TRUE == FlexioChState)
            {
                RetVal = PWM_HIGH;
            }
            else
            {
                RetVal = PWM_LOW;
            }
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            break;
        default:
            /* Nothing to do. */
            break;
    }

    return RetVal;
    }
#endif /* PWM_GET_OUTPUT_STATE_API */

/*===============================================================================================*/
#if (PWM_NOTIFICATION_SUPPORTED == STD_ON)
/**
* @brief        Pwm_Ipw_DisableNotification
* @details      This function will call DisableNotification function for the corresponding IP of PwmChannel
*
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       void
*
*/
void Pwm_Ipw_DisableNotification(const Pwm_IpwChannelConfigType * const IpConfig)
{
    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            /* Call Emios IP function */
            if((EMIOS_PWM_IP_MODE_DAOC_FLAG != ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->Mode) &&
               (EMIOS_PWM_IP_MODE_DAOC_FLAG_BOTH != ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->Mode))
            {
                Emios_Pwm_Ip_SetFlagRequest(IpConfig->ChannelInstanceId,
                            ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                            EMIOS_PWM_IP_NOTIFICATION_DISABLED);
            }
            break;
#endif
#if (PWM_FLEXIO_USED == STD_ON)
        case PWM_CHANNEL_FLEXIO:
            /* Call Flexio IP function */
            (void)Flexio_Pwm_Ip_UpdateInterruptMode(IpConfig->ChannelInstanceId,
                                              IpConfig->FlexioChConfig->TimerId,
                                              FLEXIO_PWM_IP_IRQ_DISABLED);
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            break;
        default:
            /* Nothing to do. */
            /* Avoid compiler warning */
            (void) IpConfig;
            break;
    }

}
#endif /* PWM_NOTIFICATION_SUPPORTED */

/*===============================================================================================*/
#if (PWM_NOTIFICATION_SUPPORTED == STD_ON)
/**
* @brief        Pwm_Ipw_EnableNotification
* @details      This function will call EnableNotification function for the corresponding IP of PwmChannel
*
* @param[in]    Notification   Notification edge type to be enabled
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       void
*
*/
void Pwm_Ipw_EnableNotification(Pwm_EdgeNotificationType       Notification,
                                const Pwm_IpwChannelConfigType * const IpConfig)
{
    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            /* Call Emios IP function */
            if(EMIOS_PWM_IP_MODE_GPO != ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->Mode)
            {
                Emios_Pwm_Ip_SetFlagRequest(IpConfig->ChannelInstanceId,
                                           ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                            EMIOS_PWM_IP_INTERRUPT_REQUEST);
            }
            break;
#endif
#if (PWM_FLEXIO_USED == STD_ON)
        case PWM_CHANNEL_FLEXIO:
            /* Call Flexio IP function */
            switch (Notification)
            {
                case PWM_RISING_EDGE:
                    (void)Flexio_Pwm_Ip_UpdateInterruptMode(IpConfig->ChannelInstanceId,
                                                      IpConfig->FlexioChConfig->TimerId,
                                                      FLEXIO_PWM_IP_IRQ_ON_RISING_EDGE);
                    break;
                case PWM_FALLING_EDGE:
                    (void)Flexio_Pwm_Ip_UpdateInterruptMode(IpConfig->ChannelInstanceId,
                                                      IpConfig->FlexioChConfig->TimerId,
                                                      FLEXIO_PWM_IP_IRQ_ON_FALLING_EDGE);
                    break;
                case PWM_BOTH_EDGES:
                    (void)Flexio_Pwm_Ip_UpdateInterruptMode(IpConfig->ChannelInstanceId,
                                                      IpConfig->FlexioChConfig->TimerId,
                                                      FLEXIO_PWM_IP_IRQ_ON_BOTH_EDGES);
                    break;
                default:
                    /* Noting to do */
                    break;
            }
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            break;
        default:
            /* Nothing to do. */
            /* Avoid compiler warning */
            (void)Notification;
            break;
    }
}
#endif /* PWM_NOTIFICATION_SUPPORTED */

/*===============================================================================================*/
#if (PWM_GET_CHANNEL_STATE_API == STD_ON)
/**
* @brief        Pwm_Ipw_GetChannelState
* @details      This function will call GetChannelState function for the corresponding IP of PwmChannel
*
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       uint16
*
*/
uint16 Pwm_Ipw_GetChannelState(const Pwm_IpwChannelConfigType * const IpConfig)
{
    /** @brief  Store return value */
    uint16 DutyCycle = (uint16)0U;

    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            /* Call Emios IP function */
            if(EMIOS_PWM_IP_MODE_GPO != ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->Mode)
            {
                DutyCycle = Pwm_Ipw_aEmiosDutyCycle[IpConfig->ChannelInstanceId][((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId];
            }
            else
            {
                DutyCycle = 0U;
            }

            break;
#endif
#if (PWM_FLEXIO_USED == STD_ON)
        case PWM_CHANNEL_FLEXIO:
            /* Get Duty cycle of Flexio Ip */
            DutyCycle =  Pwm_Ipw_aFlexioDutyCycle[IpConfig->ChannelInstanceId][(IpConfig->FlexioChConfig)->TimerId];
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            break;
        default:
            /* Nothing to do. */
            /* Avoid compiler warning */
            (void) DutyCycle;
            (void) IpConfig;
            break;
    }

    return DutyCycle;
}
#endif /* PWM_GET_CHANNEL_STATE */

/*===============================================================================================*/
#if (PWM_SET_CLOCK_MODE_API == STD_ON)
/**
* @brief        Pwm_Ipw_SetClockMode
* @details      This function will call SetClockMode function of all configured hardware eMIOS modules.
*
* @param[in]    Prescaler      Pre-scaler type
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       void
*
*/
void Pwm_Ipw_SetClockMode(Pwm_PrescalerType              Prescaler,
                          const Pwm_IpwChannelConfigType * const IpConfig)
{
    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            if(PWM_PRIMARY_PRESCALER == Prescaler)
            {
                Emios_Pwm_Ip_SetClockPs(IpConfig->ChannelInstanceId,
                                        ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                        ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->InternalPs
                                        );
            }
            else
            {
                Emios_Pwm_Ip_SetClockPs(IpConfig->ChannelInstanceId,
                                        ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                        ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->InternalPsAlt
                                        );
            }
            break;
#endif
#if (PWM_FLEXIO_USED == STD_ON)
        case PWM_CHANNEL_FLEXIO:
            if(PWM_PRIMARY_PRESCALER == Prescaler)
            {
                (void)Flexio_Pwm_Ip_UpdateClockPrescaler(IpConfig->ChannelInstanceId,
                                                   IpConfig->FlexioChConfig->TimerId,
                                                   IpConfig->FlexioChConfig->Prescaler
                                                  );
            }
            else
            {
                (void)Flexio_Pwm_Ip_UpdateClockPrescaler(IpConfig->ChannelInstanceId,
                                                   IpConfig->FlexioChConfig->TimerId,
                                                   IpConfig->FlexioAlternateClk
                                                  );
            }
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            break;
        default:
            /* Nothing to do. */
            /* Avoid compiler warning */
            (void) IpConfig;
            break;
    }
}
#endif

/*===============================================================================================*/
#if (PWM_SET_COUNTER_BUS_API == STD_ON)
/**
* @brief        Pwm_Ipw_SetCounterBus
* @details      This function will change the counter bus of PWM channels running.
*
* @param[in]    Bus            eMIOS bus to change to
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       void
*
*/
void Pwm_Ipw_SetCounterBus(uint32                         Bus,
                           const Pwm_IpwChannelConfigType * const IpConfig)
{
    if (PWM_CHANNEL_EMIOS == (IpConfig->ChannelType))
    {
#if (PWM_EMIOS_USED == STD_ON)
        if (0x00U == Bus)
        {
            Emios_Pwm_Ip_SetBusSelected(IpConfig->ChannelInstanceId,
                                        ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                        EMIOS_PWM_IP_BUS_A);
        }
        else if (0x01U == Bus)
        {
            Emios_Pwm_Ip_SetBusSelected(IpConfig->ChannelInstanceId,
                                       ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                       EMIOS_PWM_IP_BUS_BCDE);
        }
        else
        {
            Emios_Pwm_Ip_SetBusSelected(IpConfig->ChannelInstanceId,
                                        ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                        EMIOS_PWM_IP_BUS_F);
        }
#endif
    }
    else
    {
        /* This parameter is not used */
        (void)Bus;
    }
}
#endif /* PWM_SET_COUNTER_BUS_API */

/*===============================================================================================*/
#if (PWM_SET_CHANNEL_OUTPUT_API == STD_ON)
/**
* @brief        Pwm_Ipw_SetChannelOutput
* @details      This function is useful to set the state of the PWM pin as requested
*
* @param[in]    State          State of the channel to change to
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       void
*
*/
void Pwm_Ipw_SetChannelOutput(Pwm_StateType                  State,
                              const Pwm_IpwChannelConfigType * const IpConfig)
{
    if (PWM_CHANNEL_EMIOS == (IpConfig->ChannelType))
    {
#if (PWM_EMIOS_USED == STD_ON)
        if ((EMIOS_PWM_IP_MODE_OPWFMB_FLAG == ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->Mode) ||
            (EMIOS_PWM_IP_MODE_OPWFMB_FLAG_BOTH == ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->Mode)
            )
        {
            if(PWM_EMIOS_ACTIVE == State)
            {
                /* Call Emios IP Force match on trailing edge */
                Emios_Pwm_Ip_ForceMatchTrailingEdge(IpConfig->ChannelInstanceId,
                                                ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                                TRUE);
            }
            else if(PWM_EMIOS_INACTIVE == State)
            {
                /* Call Emios IP Force match on leading edge */
                Emios_Pwm_Ip_ForceMatchLeadingEdge(IpConfig->ChannelInstanceId,
                                                ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                                TRUE);
            }
            else
            {
                /* Do nothing */
            }
        }
        else
        {
            if(PWM_EMIOS_ACTIVE == State)
            {
                /* Call Emios IP Force match on leading edge */
                Emios_Pwm_Ip_ForceMatchLeadingEdge(IpConfig->ChannelInstanceId,
                                                ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                                TRUE);
            }
            else if(PWM_EMIOS_INACTIVE == State)
            {
                /* Call Emios IP Force match on trailing edge */
                Emios_Pwm_Ip_ForceMatchTrailingEdge(IpConfig->ChannelInstanceId,
                                                ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                                TRUE);

            }
            else
            {
                /* Do nothing */
            }
        }
#endif
    }
    else
    {
        /* This parameter is not used */
        (void)State;
    }
}
#endif /* PWM_SET_CHANNEL_OUTPUT_API */

/*===============================================================================================*/
#if (PWM_SET_TRIGGER_DELAY_API == STD_ON)
/**
* @brief        Implementation specific function to change the trigger delay
* @details      This function is useful to set the trigger delay in OPWMT mode. If no DET error
*               reported from function Pwm_Ipw_ValidateTriggerMode then the trigger delay for
*               current channel will be set.
*
* @param[in]    TriggerDelay   Delay value for trigger
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       void
*
*/
void Pwm_Ipw_SetTriggerDelay(Pwm_Ipw_PeriodType                 TriggerDelay,
                             const Pwm_IpwChannelConfigType * const IpConfig)
{
    if (PWM_CHANNEL_EMIOS == (IpConfig->ChannelType))
    {
#if (PWM_EMIOS_USED == STD_ON)
        /* Call eMIOS IP function */
        Emios_Pwm_Ip_SetTriggerPlacement(IpConfig->ChannelInstanceId,
                                        ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                        TriggerDelay);
#endif
    }
    else
    {
        /* This parameter is not used */
        (void)TriggerDelay;
    }
}
#endif /* PWM_SET_TRIGGER_DELAY_API */

/*===============================================================================================*/
#if (PWM_SET_CHANNEL_DEAD_TIME_API == STD_ON)
/**
* @brief
* @details      This function is used to update the deadtime at runtime for Pwm channels.
*
* @param[in]    DeadTimeTicks   DeadTime value in ticks
* @param[in]    IpConfig        Pointer to PWM top configuration structure
*
* @return       void
*
*/
void Pwm_Ipw_SetChannelDeadTime(uint16                          DeadTimeTicks,
                                const Pwm_IpwChannelConfigType * const IpConfig)
{
    if (PWM_CHANNEL_EMIOS == (IpConfig->ChannelType))
    {
#if (PWM_EMIOS_USED == STD_ON)
        /* Call eMIOS IP function */
        Emios_Pwm_Ip_SetDeadTime(IpConfig->ChannelInstanceId,
                                 ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                 DeadTimeTicks);
#endif
    }
    else
    {
        /* This parameter is not used */
        (void)DeadTimeTicks;
    }
}
#endif /* PWM_SET_CHANNEL_DEAD_TIME_API */

/*===============================================================================================*/
#if (PWM_SET_DUTY_PHASE_SHIFT_API == STD_ON)
/**
* @brief        This function set phase shift and duty cycle value (as immediate or synchronized base on API parameter SyncUpdate)
*
* @details      Pwm_Ipw_SetDutyPhaseShift allows to set both phase shift and duty cycle value,
*               The phase shift is the offset of the leading edge of the signal in respect to period starting point.
*
* @param[in]    DutyCycle               Pwm duty cycle value 0x0000 for 0% ... 0x8000 for 100%
* @param[in]    PhaseShift              Phase shift value (in ticks)
* @param[in]    IpConfig                Pointer to PWM top configuration structure
* @param[in]    SyncUpdate              Update duty and phases shift value synchronization for channels in given module or not
*                                       TRUE     Set the phase shift and duty cycle value base on the synchronization when calling Pwm_SyncUpdate.
*                                       FALSE    Set phase shift and duty cycle value immediately
*
* @return       void
*
*/
void Pwm_Ipw_SetDutyPhaseShift(uint16                           DutyCycle,
                               uint16                           PhaseShift,
                               const Pwm_IpwChannelConfigType * const IpConfig,
                               boolean                          SyncUpdate)
{

#if (PWM_EMIOS_USED == STD_ON)
    uint16 DutyCycleTicks = 0U;
    uint16 PeriodChTicks = 0U;
    uint32 ChannelMask = 0U;
#endif

    if (PWM_CHANNEL_EMIOS == (IpConfig->ChannelType))
    {
#if (PWM_EMIOS_USED == STD_ON)
        ChannelMask = (uint32)((uint32)1U << (((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId));

        Emios_Pwm_Ip_ComparatorTransferDisable(IpConfig->ChannelInstanceId,
                                               ChannelMask);

        PeriodChTicks = Emios_Pwm_Ip_GetPeriod(IpConfig->ChannelInstanceId,
                                              ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);
        /* Call eMIOS IP function */
        (void)Emios_Pwm_Ip_SetPhaseShift(IpConfig->ChannelInstanceId,
                                   ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                    PhaseShift);

        DutyCycleTicks = (uint16)(((uint32)PeriodChTicks * DutyCycle) >> 15U);

        (void)Emios_Pwm_Ip_SetDutyCycle(IpConfig->ChannelInstanceId,
                                  ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                   DutyCycleTicks);
        if (SyncUpdate == FALSE)
        {
            Emios_Pwm_Ip_ComparatorTransferEnable(IpConfig->ChannelInstanceId,
                                                  ChannelMask);
        }

        Pwm_Ipw_aEmiosDutyCycle[IpConfig->ChannelInstanceId][((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId] = DutyCycle;

#endif
    }
    else
    {
        /* This parameter is not used */
        (void)PhaseShift;
        (void)DutyCycle;
        (void)SyncUpdate;
    }

}
#endif /* PWM_SET_DUTY_PHASE_SHIFT_API */

/*===============================================================================================*/
#if (PWM_SET_PHASE_SHIFT_API == STD_ON)
/**
* @brief        This function set phase shift value and also force duty cycle to 50%. The output will
*               effect immediately after calling this function.
* @details      In order to have Phase-Shifted Full-Bridge controller, Pwm_SetPhaseShift is introduced.
*
*
* @param[in]    Period         Period value for this channel
* @param[in]    PhaseShift     Phase shift value
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       void
*
*/
void Pwm_Ipw_SetPhaseShift(uint16                         Period,
                           uint16                         PhaseShift,
                           const Pwm_IpwChannelConfigType * const IpConfig)
{
#if (PWM_EMIOS_USED == STD_ON)
    uint16 DutyCycleTicks = 0U;
    uint16 PeriodChTicks = 0U;
    uint16 PhaseShiftTicks = 0U;
#endif

    if (PWM_CHANNEL_EMIOS == (IpConfig->ChannelType))
    {
#if (PWM_EMIOS_USED == STD_ON)
        PeriodChTicks = Emios_Pwm_Ip_GetPeriod(IpConfig->ChannelInstanceId,
                                              ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);
        PhaseShiftTicks = (uint16)((uint32)PeriodChTicks * PhaseShift) >> 15U;
        /* Call eMIOS IP function */
        (void)Emios_Pwm_Ip_SetPhaseShift(IpConfig->ChannelInstanceId,
                                   ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                    PhaseShiftTicks);

        DutyCycleTicks = (uint16)((uint32)PeriodChTicks * 0x4000U) >> 15U;

        (void)Emios_Pwm_Ip_SetDutyCycle(IpConfig->ChannelInstanceId,
                                  ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                   DutyCycleTicks);
#endif
    }
    else
    {
        /* This parameter is not used */
        (void)PhaseShift;
        (void)Period;
    }

}
#endif /* PWM_SET_PHASE_SHIFT_API */

/*===============================================================================================*/
#if ((PWM_UPDATE_DUTY_SYNCHRONOUS == STD_ON) && (PWM_SYNC_UPDATE_API == STD_ON))
/**
* @brief        Pwm_Ipw_SyncUpdate
* @details      This function will allow synchronized loading
*               of the value register.
*
* @param[in]    ModuleId        instance ID
*
* @return       void
*
*/
void Pwm_Ipw_SyncUpdate(uint8  ModuleId)
{

    if (ModuleId < PWM_HW_MODULE_EMIOS_NO)
    {
        /* Call eMIOS IP function */
        Emios_Pwm_Ip_SyncUpdate(ModuleId);

    }
    else
    {
        /* Do Nothing */
        /* The feature is not supported for FLEXIO*/
    }
}
#endif /* PWM_UPDATE_DUTY_SYNCHRONOUS && PWM_SYNC_UPDATE_API */

/*===============================================================================================*/
#if ((PWM_UPDATE_DUTY_SYNCHRONOUS == STD_ON) && (PWM_SET_DUTY_CYCLE_NO_UPDATE_API == STD_ON))
/**
* @brief        Pwm_Ipw_SetDutyCycle_NoUpdate
*
* @param[in]    DutyCycle      Duty cycle value 0x0000 for 0% ... 0x8000 for 100%
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       void
*
*/
Std_ReturnType Pwm_Ipw_SetDutyCycle_NoUpdate(uint16                               DutyCycle,
                                             const Pwm_IpwChannelConfigType * const IpConfig)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
#if (PWM_EMIOS_USED == STD_ON)
    uint16 EmiosChPeriod = 0U;
    uint16 EmiosChDuty = 0U;
    uint32 ChannelMask = 0U;
#endif

    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            ChannelMask = (uint32)((uint32)1U << (((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId));

            Emios_Pwm_Ip_ComparatorTransferDisable(IpConfig->ChannelInstanceId,
                                                   ChannelMask);
            /* Get the period of the chanel  */
            EmiosChPeriod = Emios_Pwm_Ip_GetPeriod(IpConfig->ChannelInstanceId,
                                                  ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);

            EmiosChDuty = (uint16)(((uint32)EmiosChPeriod * DutyCycle) / 0x8000U);
            /* Call Emios IP function */
            RetVal = (Std_ReturnType)Emios_Pwm_Ip_SetDutyCycle(IpConfig->ChannelInstanceId,
                                                               ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                                               EmiosChDuty);
            /* Call Emios api for setting the polarity and mode */
            Emios_Pwm_Ip_SetOutputToNormal(IpConfig->ChannelInstanceId,
                                            ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                            DutyCycle,
                                            ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->OutputPolarity,
                                            ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->Mode
                                           );
            Pwm_Ipw_aEmiosDutyCycle[IpConfig->ChannelInstanceId][((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId] = DutyCycle;
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            break;
        default:
            /* Nothing to do. */
            /* Avoid compiler warning */
            (void) DutyCycle;
            (void) IpConfig;
            break;
    }

    return RetVal;

}
#endif /* PWM_UPDATE_DUTY_SYNCHRONOUS && PWM_SET_DUTY_CYCLE_NO_UPDATE_API */

/*===============================================================================================*/
#if ((PWM_UPDATE_DUTY_SYNCHRONOUS == STD_ON) && (PWM_SET_PERIOD_AND_DUTY_NO_UPDATE_API == STD_ON))
/**
* @brief        Pwm_Ipw_SetPeriodAndDuty_NoUpdate
*
* @param[in]    Period         Period value for this channel
* @param[in]    DutyCycle      Duty cycle value 0x0000 for 0% ... 0x8000 for 100%
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       void
*
*/
void Pwm_Ipw_SetPeriodAndDuty_NoUpdate(Pwm_PeriodType                 Period,
                                       uint16                         DutyCycle,
                                       const Pwm_IpwChannelConfigType * const IpConfig)
{
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;
#if (PWM_EMIOS_USED == STD_ON)
    uint16 EmiosChDuty = 0U;
    uint32 ChannelMask = 0U;
#endif

    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            ChannelMask = (uint32)((uint32)1U << (((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId));

            Emios_Pwm_Ip_ComparatorTransferDisable(IpConfig->ChannelInstanceId,
                                                   ChannelMask);
            /* Call Emios api for setting the period */
            Emios_Pwm_Ip_SetPeriod(IpConfig->ChannelInstanceId,
                                   ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                   Period);

            /* Calculate new duty value in ticks */
            EmiosChDuty = (uint16)(((uint32)Period * DutyCycle)/0x8000U);
            /* Call Emios api for setting the duty cycle */
            RetVal = (Std_ReturnType)Emios_Pwm_Ip_SetDutyCycle(IpConfig->ChannelInstanceId,
                                                               ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                                               EmiosChDuty);
            /* Call Emios api for setting the polarity and mode */
            Emios_Pwm_Ip_SetOutputToNormal(IpConfig->ChannelInstanceId,
                                            ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                            DutyCycle,
                                            ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->OutputPolarity,
                                            ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->Mode
                                           );
            Pwm_Ipw_aEmiosDutyCycle[IpConfig->ChannelInstanceId][((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId] = DutyCycle;
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            break;
        default:
            /* Nothing to do. */
            /* Avoid compiler warning */
            (void) Period;
            (void) DutyCycle;
            (void) IpConfig;
            break;
    }

    (void) RetVal;

}
#endif /* PWM_UPDATE_DUTY_SYNCHRONOUS && PWM_SET_PERIOD_AND_DUTY_NO_UPDATE_API */

/*===============================================================================================*/
#if ((PWM_UPDATE_DUTY_SYNCHRONOUS == STD_ON) && (PWM_SET_PHASE_SHIFT_NO_UPDATE_API == STD_ON))
/**
* @brief        This function set phase shift value and also force duty cycle to 50%. The output will take
*               effect after Pwm_SyncUpdate be called.
* @details      In order to have Phase-Shifted Full-Bridge controller, Pwm_SetPhaseShift is introduced.
*
* @param[in]    Period         Period value for this channel
* @param[in]    PhaseShift     Phase shift value
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       void
*
*/
void Pwm_Ipw_SetPhaseShift_NoUpdate(uint16                         Period,
                                    uint16                         PhaseShift,
                                    const Pwm_IpwChannelConfigType * const IpConfig)
{
#if (PWM_EMIOS_USED == STD_ON)
    uint16 DutyCycleTicks = 0U;
    uint16 PeriodChTicks = 0U;
    uint16 PhaseShiftTicks = 0U;
    uint32 ChannelMask = 0U;
#endif

    if (PWM_CHANNEL_EMIOS == (IpConfig->ChannelType))
    {
#if (PWM_EMIOS_USED == STD_ON)
        ChannelMask = (uint32)((uint32)1U << (((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId));

        PeriodChTicks = Emios_Pwm_Ip_GetPeriod(IpConfig->ChannelInstanceId,
                                              ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);
        PhaseShiftTicks = (uint16)((uint32)PeriodChTicks * PhaseShift) >> 15U;

        Emios_Pwm_Ip_ComparatorTransferDisable(IpConfig->ChannelInstanceId,
                                               ChannelMask);
        /* Call eMIOS IP function */
        (void)Emios_Pwm_Ip_SetPhaseShift(IpConfig->ChannelInstanceId,
                                   ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                    PhaseShiftTicks);

        DutyCycleTicks = (uint16)((uint32)PeriodChTicks * 0x4000U) >> 15U;

        (void)Emios_Pwm_Ip_SetDutyCycle(IpConfig->ChannelInstanceId,
                                  ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                   DutyCycleTicks);
#endif
    }
    else
    {
        /* This parameter is not used */
        (void)PhaseShift;
        (void)Period;
    }

}
#endif /* PWM_SET_PHASE_SHIFT_NO_UPDATE_API */

/*===============================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && (PWM_SYNC_UPDATE_API == STD_ON))
/**
* @brief        Pwm_Ipw_ValidateModuleId
* @details      Validate the module id.
*               In order to be valid, the module id should less than number of module that supported
*
* @param[in]    ModuleId        The Id of the ipv module
*
* @return       Std_ReturnType
*               E_NOT_OK        The ModuleId is invalid
*               E_OK            The ModuleId is valid
*
*/     
Std_ReturnType  Pwm_Ipw_ValidateModuleId(uint8 ModuleId)
{
    /** @brief  Variable to store the value returned by the function */
    Std_ReturnType      RetVal = (Std_ReturnType)E_OK;
#if (PWM_EMIOS_USED == STD_ON)    
    if(PWM_HW_MODULE_EMIOS_NO <= ModuleId)
    {
        RetVal = (Std_ReturnType)E_NOT_OK;
    }
    else
    {
        RetVal = (Std_ReturnType)E_OK;
    }
#else
    RetVal = (Std_ReturnType)E_NOT_OK;
    (void)ModuleId;
#endif

    return RetVal;
}
#endif

/*===============================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && (PWM_NOTIFICATION_SUPPORTED == STD_ON))
/**
* @brief        Pwm_Ipw_ValidateNotification
* @details      This function will check validate of edge notification type with polarity
*
* @param[in]    Notification   Notification edge type to check if available
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       Std_ReturnType
*               E_NOT_OK       Notification handler is not valid
*               E_OK           Notification handler is valid
*
*/
Std_ReturnType Pwm_Ipw_ValidateNotification(Pwm_EdgeNotificationType       Notification,
                                            const Pwm_IpwChannelConfigType * const IpConfig)
{
    /** @brief  Variable to store the value returned by the function */
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;

    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            /* Call Emios IP function */
            RetVal = Pwm_Ipw_EmiosValidateNotification(Notification, IpConfig);

            break;
#endif
#if (PWM_FLEXIO_USED == STD_ON)
        case PWM_CHANNEL_FLEXIO:
            /* Call Flexio IP function */
            if(
                ((PWM_FALLING_EDGE == Notification) && (FLEXIO_PWM_IP_IRQ_ON_FALLING_EDGE == IpConfig->FlexioChConfig->IrqMode)) ||
                ((PWM_RISING_EDGE == Notification) && (FLEXIO_PWM_IP_IRQ_ON_RISING_EDGE == IpConfig->FlexioChConfig->IrqMode)) ||
                ((PWM_BOTH_EDGES == Notification) && (FLEXIO_PWM_IP_IRQ_ON_BOTH_EDGES == IpConfig->FlexioChConfig->IrqMode))
              )
            {
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
            else
            {
                RetVal = (Std_ReturnType)E_OK;
            }

            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            break;
        default:
            /* Nothing to do. */
            /* Avoid compiler warning */
            (void)IpConfig;
            (void)Notification;
            break;
    }

    return RetVal;
}
#endif

/*===============================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && (PWM_SET_DUTY_PHASE_SHIFT_API == STD_ON))
/**
* @brief        Pwm_Ipw_ValidateSetPhaseShiftParams
* @details      This function will check phase shift and duty cycle value
*
* @param[in]    PhaseShift   Phase shift value
* @param[in]    DutyCycle    Duty Cycle value
* @param[in]    IpConfig     Pointer to PWM top configuration structure
*
* @return       Std_ReturnType
*               E_NOT_OK    Phase shift or duty cycle value is invalid
*               E_OK        Phase shift or duty cycle is valid
*
*/
Std_ReturnType Pwm_Ipw_ValidateSetDutyPhaseShiftParams(uint16 PhaseShift,
                                                       uint16 DutyCycle,
                                                       const Pwm_IpwChannelConfigType * const IpConfig)
{
    /** @brief  Variable to store the value returned by the function */
    Std_ReturnType RetVal = (Std_ReturnType)E_NOT_OK;

#if (PWM_EMIOS_USED == STD_ON)
    uint16 EmiosPeriod = 0U;
    uint16 EmiosDuty = 0U;
#endif
    if (PWM_CHANNEL_EMIOS == (IpConfig->ChannelType))
    {
#if (PWM_EMIOS_USED == STD_ON)
        /* Call eMIOS IP function */
        EmiosPeriod = Emios_Pwm_Ip_GetPeriod(IpConfig->ChannelInstanceId,
                                            ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);

        EmiosDuty = (uint16)(((uint32)EmiosPeriod * DutyCycle) >> 15U);

        if (EMIOS_PWM_IP_MODE_OPWMT == Emios_Pwm_Ip_GetChannelMode(IpConfig->ChannelInstanceId,
                                                                ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId)
            )
        {
            if(EmiosPeriod >= PhaseShift)
            {
                RetVal = (Std_ReturnType)E_OK;
            }
        }
        else
        {
            if(EmiosPeriod >= (PhaseShift + EmiosDuty))
            {
               RetVal = (Std_ReturnType)E_OK;
            }
        }
#endif
    }
    return RetVal;

}
#endif

/*===============================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && (PWM_SET_TRIGGER_DELAY_API == STD_ON))
/**
* @brief        Pwm_Ipw_ValidateTriggerMode
* @details      This function will check validate operation mode of current channel to use
*               trigger delay feature.
*
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       Std_ReturnType
*               E_NOT_OK        Current channel mode is invalid
*               E_OK            Current channel mode is valid
*
*/
Std_ReturnType Pwm_Ipw_ValidateTriggerMode(const Pwm_IpwChannelConfigType * const IpConfig)
{
    /** @brief  Variable to store the value returned by the function */
    Std_ReturnType RetVal = (Std_ReturnType)E_NOT_OK;

    if (PWM_CHANNEL_EMIOS == (IpConfig->ChannelType))
    {
#if (PWM_EMIOS_USED == STD_ON)
        /* Call eMIOS IP function */
        if (EMIOS_PWM_IP_MODE_OPWMT == Emios_Pwm_Ip_GetChannelMode(IpConfig->ChannelInstanceId,
                                                                ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId)
            )
        {
            RetVal = (Std_ReturnType)E_OK;
        }
        else
        {
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
#endif
    }
    return RetVal;
}
#endif

/*===============================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && ((PWM_SET_DUTY_CYCLE_NO_UPDATE_API == STD_ON) || (PWM_SET_PERIOD_AND_DUTY_NO_UPDATE_API == STD_ON) || (PWM_SET_PHASE_SHIFT_NO_UPDATE_API == STD_ON) || (PWM_SYNC_UPDATE_API == STD_ON)))
/**
* @brief        Pwm_Ipw_ValidateUpdateSynchronous
* @details      This function will check given channel with synchronization supported
*
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       Std_ReturnType
*               E_NOT_OK   synchronization is not supported this channel
*               E_OK       synchronization is supported this channel
*
*/
Std_ReturnType Pwm_Ipw_ValidateUpdateSynchronous(const Pwm_IpwChannelConfigType * const IpConfig)
{

    /** @brief  Variable to store the value returned by the function */
    Std_ReturnType                      RetVal = (Std_ReturnType)E_OK;
    (void)IpConfig;

    return RetVal;

}
#endif /* PWM_DEV_ERROR_DETECT && (PWM_SET_DUTY_CYCLE_NO_UPDATE_API || PWM_SET_PERIOD_AND_DUTY_NO_UPDATE_API) */

/*===============================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && (PWM_SET_COUNTER_BUS_API == STD_ON))
/**
* @brief        Pwm_Ipw_ValidateCounterBus
* @details      This function will validate of counter bus
*
* @param[in]    Bus      Counter bus ID
* @param[in]    IpConfig    Pointer to PWM top configuration structure
*
* @return       Std_ReturnType
*               E_NOT_OK    The selected bus is invalid
*               E_OK        The selected bus is valid
*
*/
Std_ReturnType Pwm_Ipw_ValidateCounterBus(uint32                         Bus,
                                          const Pwm_IpwChannelConfigType * const IpConfig)
{
/** @brief  Variable to store the value returned by the function */
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;

#if (PWM_EMIOS_USED == STD_ON)
    if (NULL_PTR != IpConfig->EmiosChConfig)
    {
        /* Call eMIOS IP function */
        if ((0x00U != Bus) && (0x01U != Bus) && (0x02U == Bus))
        {
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
        if ((EMIOS_PWM_IP_MODE_OPWFMB_FLAG == ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->Mode) ||
            (EMIOS_PWM_IP_MODE_OPWFMB_FLAG_BOTH == ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->Mode))
        {
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
    }
    else
    {
        /* This parameter is not used */
        (void)Bus;
    }
#else
    /* This parameter is not used */
    (void)Bus;
    (void)IpConfig;
#endif

    return RetVal;
}
#endif

/*===============================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && (PWM_SET_CHANNEL_DEAD_TIME_API == STD_ON))
/**
* @brief        Pwm_ValidateDeadTime
* @details      This function will check validate operation mode of current channel to use
*               trigger delay feature.
*
* @param[in]    IpConfig        Pointer to PWM top configuration structure
*
* @return       Std_ReturnType
*               E_NOT_OK        Current channel mode is invalid
*               E_OK            Current channel mode is valid
*
*/
Std_ReturnType Pwm_Ipw_ValidateDeadTime(const Pwm_IpwChannelConfigType * const IpConfig)
{
    /** @brief  Variable to store the value returned by the function */
    Std_ReturnType RetVal = (Std_ReturnType)E_NOT_OK;
#if (PWM_EMIOS_USED == STD_ON)
    Emios_Pwm_Ip_PwmModeType ChannelMode;
#endif

    if (PWM_CHANNEL_EMIOS == (IpConfig->ChannelType))
    {
#if (PWM_EMIOS_USED == STD_ON)
        ChannelMode = Emios_Pwm_Ip_GetChannelMode(IpConfig->ChannelInstanceId,((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);
        /* Call eMIOS IP function */
        if ((EMIOS_PWM_IP_MODE_OPWMCB_TRAIL_EDGE_FLAG == ChannelMode) ||
           (EMIOS_PWM_IP_MODE_OPWMCB_TRAIL_EDGE_FLAG_BOTH == ChannelMode) ||
           (EMIOS_PWM_IP_MODE_OPWMCB_LEAD_EDGE_FLAG == ChannelMode) ||
           (EMIOS_PWM_IP_MODE_OPWMCB_LEAD_EDGE_FLAG_BOTH == ChannelMode)
           )
        {
            RetVal = (Std_ReturnType)E_OK;
        }
        else
        {
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
#endif
    }
    return RetVal;
}
#endif

/*===============================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && (PWM_SET_CHANNEL_DEAD_TIME_API == STD_ON))
/**
* @brief        Pwm_ValidateDeadTime
* @details      This function will check validate operation mode of current channel to use
*               trigger delay feature.
*
* @param[in]    DeadTimeTicks   DeadTime value in ticks
* @param[in]    IpConfig        Pointer to PWM top configuration structure
*
* @return       Std_ReturnType
*               E_NOT_OK        Current channel mode is invalid
*               E_OK            Current channel mode is valid
*
*/
Std_ReturnType Pwm_Ipw_ValidateParamDeadTime(uint16   DeadTimeTicks,
                                             const Pwm_IpwChannelConfigType * const IpConfig)
{
    /** @brief  Variable to store the value returned by the function */
    Std_ReturnType RetVal = (Std_ReturnType)E_NOT_OK;
#if (PWM_EMIOS_USED == STD_ON)
    Emios_Pwm_Ip_PwmModeType ChannelMode;
    uint16 Period = Emios_Pwm_Ip_GetPeriod(IpConfig->ChannelInstanceId,
                                          ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);
    uint16 DutyCycle = Emios_Pwm_Ip_GetDutyCycle(IpConfig->ChannelInstanceId,
                                                ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);
#endif

    if (PWM_CHANNEL_EMIOS == (IpConfig->ChannelType))
    {
#if (PWM_EMIOS_USED == STD_ON)
        ChannelMode = Emios_Pwm_Ip_GetChannelMode(IpConfig->ChannelInstanceId,((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);
        /* Call eMIOS IP function */
        if ((EMIOS_PWM_IP_MODE_OPWMCB_TRAIL_EDGE_FLAG == ChannelMode) ||
            (EMIOS_PWM_IP_MODE_OPWMCB_TRAIL_EDGE_FLAG_BOTH == ChannelMode)
           )
        {
            if(DeadTimeTicks < (DutyCycle >> 1U))
            {
                RetVal = (Std_ReturnType)E_OK;
            }
            else
            {
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
        }
        else
        {
            if(DeadTimeTicks < (Period - DutyCycle))
            {
                RetVal = (Std_ReturnType)E_OK;
            }
            else
            {
                RetVal = (Std_ReturnType)E_NOT_OK;
            }

        }
#endif
    }
    return RetVal;

}
#endif

/*===============================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && (PWM_SET_CHANNEL_OUTPUT_API == STD_ON))
/**
* @brief        Validate the set channel output feature.
*               In order to be valid, the set channel output feature only supported for eMIOS.
*
* @param[in]    State          The state of output
* @param[in]    IpConfig        Pointer to PWM top configuration structure
*
* @return       Std_ReturnType  Validity of notification handler
*                   E_OK            set channel output parameter is valid
*                   E_NOT_OK        set channel output parameter is not valid
*
*/
Std_ReturnType Pwm_Ipw_ValidateSetChannelOutput(Pwm_StateType            State,
                                                const Pwm_IpwChannelConfigType * const IpConfig)
{

    /** @brief  Variable to store the value returned by the function */
    Std_ReturnType RetVal = (Std_ReturnType)E_NOT_OK;

    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            /* Check if state is not included */
            if ((PWM_EMIOS_ACTIVE == State) || (PWM_EMIOS_INACTIVE == State))
            {
                RetVal = (Std_ReturnType)E_OK;
            }
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            (void) State;
            break;
        default:
            /* Nothing to do. */
            /* Avoid compiler warning */
            (void) State;
            break;
    }

    return RetVal;
}
#endif

/*===============================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && ((PWM_SET_PHASE_SHIFT_API == STD_ON) || (PWM_SET_PHASE_SHIFT_NO_UPDATE_API == STD_ON) || (PWM_SET_DUTY_PHASE_SHIFT_API == STD_ON)))
/**
* @brief        Pwm_Ipw_ValidateSetPhaseShift
* @details      This function will check given channel with phase shift supported
*
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       Std_ReturnType
*               E_NOT_OK        Phase shift is not supported this channel
*               E_OK            Phase shift is supported this channel
*
*/
Std_ReturnType Pwm_Ipw_ValidateSetPhaseShift(const Pwm_IpwChannelConfigType * const IpConfig)
{
    /** @brief  Variable to store the value returned by the function */
    Std_ReturnType RetVal = (Std_ReturnType)E_NOT_OK;
#if (PWM_EMIOS_USED == STD_ON)
    Emios_Pwm_Ip_PwmModeType ChannelMode;
#endif

    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            ChannelMode = Emios_Pwm_Ip_GetChannelMode(IpConfig->ChannelInstanceId,((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);
            /* Call eMIOS IP function */
            if ((EMIOS_PWM_IP_MODE_OPWMT == ChannelMode) ||
               (EMIOS_PWM_IP_MODE_OPWMB_FLAG == ChannelMode) ||
               (EMIOS_PWM_IP_MODE_OPWMB_FLAG_BOTH == ChannelMode)
               )
            {
                RetVal = (Std_ReturnType)E_OK;
            }
            else
            {
                RetVal = (Std_ReturnType)E_NOT_OK;
            }
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            break;
        default:
            /* Nothing to do. */
            break;
    }

    return RetVal;
}
#endif /* PWM_DEV_ERROR_DETECT && (PWM_SET_PHASE_SHIFT_API || PWM_SET_PHASE_SHIFT_NO_UPDATE_API) */

/*===============================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && ((PWM_SET_PHASE_SHIFT_API == STD_ON) || (PWM_SET_PHASE_SHIFT_NO_UPDATE_API == STD_ON)))
/**
* @brief        Pwm_Ipw_ValidateSetPhaseShiftParams
* @details      This function will check phase shift value, has to be equal or lower than duty 50%
*
* @param[in]    PhaseShift   Phase shift value
*
* @return       Std_ReturnType
*               E_NOT_OK    Phase shift value is invalid
*               E_OK        Phase shift value is valid
*
*/
Std_ReturnType Pwm_Ipw_ValidateSetPhaseShiftParams(uint16 PhaseShift)
{

    /** @brief  Variable to store the value returned by the function */
    Std_ReturnType                      RetVal = (Std_ReturnType)E_OK;

    if (PWM_HALF_PERIOD_DUTY < PhaseShift)
    {
        RetVal = (Std_ReturnType)E_NOT_OK;
    }

    return RetVal;

}
#endif /* PWM_DEV_ERROR_DETECT && (PWM_SET_PHASE_SHIFT_API || PWM_SET_PHASE_SHIFT_NO_UPDATE_API) */

/*===============================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && (PWM_SET_PERIOD_AND_DUTY_API == STD_ON))
/**
* @brief        Pwm_Ipw_GetMaxPeriodValue
* @details      This function will calculate the maximum period value supported by hardware
*
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       uint32
*
*/
uint32 Pwm_Ipw_GetMaxPeriodValue(const Pwm_IpwChannelConfigType * const IpConfig)
{
    /** @brief  Store return value */
    uint32 RetVal = (uint32)0xFFFFU;

    switch(IpConfig->ChannelType)
    {
#if (PWM_EMIOS_USED == STD_ON)
        case PWM_CHANNEL_EMIOS:
            /* Call eMIOS IP function */
            RetVal = EMIOS_PWM_IP_MAX_CNT_VAL;
            break;
#endif
        case PWM_CHANNEL_NONE:
            /* Nothing to do. No hw channel assigned to the logical channel */
            break;
        default:
            /* Nothing to do. */
            /* Avoid compiler warning */
            (void) IpConfig;
            break;
    }

    return RetVal;
}
#endif

/*===============================================================================================*/
#if ((PWM_DEV_ERROR_DETECT == STD_ON) && (PWM_SET_TRIGGER_DELAY_API == STD_ON))
/**
* @brief        Pwm_Ipw_GetCurrentPeriodValue
* @details      This function will calculate the period value of current channel
*
* @param[in]    IpConfig        Pointer to PWM top configuration structure
*
* @return       Pwm_Ipw_PeriodType  Period value of current channel
*
*/
Pwm_Ipw_PeriodType Pwm_Ipw_GetCurrentPeriodValue(const Pwm_IpwChannelConfigType * const IpConfig)
{
    /** @brief  Variable to store the value returned by the function */
    Pwm_Ipw_PeriodType Period = (Pwm_Ipw_PeriodType)0U;

    if (PWM_CHANNEL_EMIOS ==  (IpConfig->ChannelType))
    {
#if (PWM_EMIOS_USED == STD_ON)
        /* Call eMIOS IP function */
        Period = Emios_Pwm_Ip_GetPeriod(IpConfig->ChannelInstanceId,
                                       ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);
#endif
    }

    return Period;
}
#endif

/*===============================================================================================*/
#if (PWM_POWER_STATE_SUPPORTED == STD_ON)
/**
* @brief        Pwm_Ipw_ValidateIdleState
* @details      This function will call ValidateIdleState function of all configured hardware eMIOS modules
*
* @param[in]    IpConfig        Pointer to PWM top configuration structure
*
* @return       Std_ReturnType
*               E_NOT_OK        The Idle state is not ready
*               E_OK            The Idle state is ready
*
*/
Std_ReturnType Pwm_Ipw_ValidateIdleState(const Pwm_IpwChannelConfigType * const IpConfig)
{
    /** @brief  Variable to store the value returned by the function */
    Std_ReturnType RetVal = (Std_ReturnType)E_OK;

#if (PWM_EMIOS_USED == STD_ON)
    if (NULL_PTR != IpConfig->EmiosChConfig)
    {
        if (EMIOS_PWM_IP_MODE_GPO != Emios_Pwm_Ip_GetChannelMode(IpConfig->ChannelInstanceId,
                                                             ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId)
            )
        {
            RetVal = (Std_ReturnType)E_NOT_OK;
        }
    }
#else
    (void)IpConfig;
#endif

    return RetVal;
}
#endif /* PWM_POWER_STATE_SUPPORTED */

/*===============================================================================================*/
#if (PWM_POWER_STATE_SUPPORTED == STD_ON)
/**
* @brief        Pwm_Ipw_SetPowerState
* @details      This function will call SetPowerState function of all configured hardware eMIOS modules
*
* @param[in]    PowerState     Target power mode
* @param[in]    IpConfig       Pointer to PWM top configuration structure
*
* @return       void
*
*/
void Pwm_Ipw_SetPowerState(Pwm_PowerStateType               PowerState,
                           const Pwm_IpwChannelConfigType * const IpConfig)
{
#if (PWM_EMIOS_USED == STD_ON)
    Emios_Pwm_Ip_PwmModeType ChannelMode;
    uint8 Channel;
#endif
#if (PWM_EMIOS_USED == STD_ON)
    if (NULL_PTR != IpConfig->EmiosChConfig)
    {
        ChannelMode = Emios_Pwm_Ip_GetChannelMode(IpConfig->ChannelInstanceId,
                                                  ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);
        Channel = Emios_Pwm_Ip_GetMasterBusChannel(IpConfig->ChannelInstanceId,
                                                    ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId);
        if((EMIOS_PWM_IP_MODE_OPWMCB_TRAIL_EDGE_FLAG == ChannelMode) ||
           (EMIOS_PWM_IP_MODE_OPWMCB_TRAIL_EDGE_FLAG_BOTH == ChannelMode) ||
           (EMIOS_PWM_IP_MODE_OPWMCB_LEAD_EDGE_FLAG == ChannelMode) ||
           (EMIOS_PWM_IP_MODE_OPWMCB_LEAD_EDGE_FLAG_BOTH == ChannelMode)
          )
        {
            if (PWM_LOW_POWER == PowerState)
            {
                Emios_Pwm_Ip_SetPreEnableClock(IpConfig->ChannelInstanceId,
                                                ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                                FALSE);
            }
            else
            {
                Emios_Pwm_Ip_SetPreEnableClock(IpConfig->ChannelInstanceId,
                                               ((const Emios_Pwm_Ip_ChannelConfigType *)IpConfig->EmiosChConfig)->ChannelId,
                                               TRUE);
            }
        }

        if (PWM_LOW_POWER == PowerState)
        {
            Emios_Pwm_Ip_SetPreEnableClock(IpConfig->ChannelInstanceId,
                                            Channel,
                                            FALSE);
        }
        else
        {
            Emios_Pwm_Ip_SetPreEnableClock(IpConfig->ChannelInstanceId,
                                           Channel,
                                           TRUE);
        }
    }
#endif
}
#endif /* PWM_POWER_STATE_SUPPORTED */

#define PWM_STOP_SEC_CODE
#include "Pwm_MemMap.h"


#ifdef __cplusplus
}
#endif

/** @} */
