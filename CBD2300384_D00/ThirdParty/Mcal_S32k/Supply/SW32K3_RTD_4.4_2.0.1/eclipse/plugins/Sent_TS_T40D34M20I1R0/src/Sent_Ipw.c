/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : FLEXIO_SENT_IP
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
*   @file    Sent_Ipw.c
*   @version 2.0.1
*
*   @brief   AUTOSAR Sent - Isolation level file for Sent driver.
*   @details Implementation file for function definition on isolation level between high and low level driver.
*
*   @addtogroup SENT_DRIVER
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
#include "Sent_Ipw.h"
#include "Flexio_Sent_Ip.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/


#define SENT_IPW_VENDOR_ID_C                    43
#define SENT_IPW_AR_RELEASE_MAJOR_VERSION_C     4
#define SENT_IPW_AR_RELEASE_MINOR_VERSION_C     4
#define SENT_IPW_AR_RELEASE_REVISION_VERSION_C  0
#define SENT_IPW_SW_MAJOR_VERSION_C             2
#define SENT_IPW_SW_MINOR_VERSION_C             0
#define SENT_IPW_SW_PATCH_VERSION_C             1

/*==================================================================================================
*                                      FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Sent_Ipw header file are of the same vendor */
#if (SENT_IPW_VENDOR_ID_C != SENT_IPW_VENDOR_ID)
    #error "Sent_Ipw.h and Sent_Ipw.c have different vendor ids"
#endif
/* Check if current file and Sent_Ipw header file are of the same Autosar version */
#if ((SENT_IPW_AR_RELEASE_MAJOR_VERSION_C    != SENT_IPW_AR_RELEASE_MAJOR_VERSION) || \
     (SENT_IPW_AR_RELEASE_MINOR_VERSION_C    != SENT_IPW_AR_RELEASE_MINOR_VERSION) || \
     (SENT_IPW_AR_RELEASE_REVISION_VERSION_C != SENT_IPW_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Sent_Ipw.h and Sent_Ipw.c are different"
#endif
/* Check if current file and Sent_Ipw header file are of the same Software version */
#if ((SENT_IPW_SW_MAJOR_VERSION_C != SENT_IPW_SW_MAJOR_VERSION) || \
     (SENT_IPW_SW_MINOR_VERSION_C != SENT_IPW_SW_MINOR_VERSION) || \
     (SENT_IPW_SW_PATCH_VERSION_C != SENT_IPW_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Sent_Ipw.h and Sent_Ipw.c are different"
#endif

/* Check if current file and Flexio_Sent_Ip header file are of the same vendor */
#if (SENT_IPW_VENDOR_ID_C != FLEXIO_SENT_IP_VENDOR_ID)
   #error "Flexio_Sent_Ip.h and Sent_Ipw.c have different vendor ids"
#endif
/* Check if current file and Flexio_Sent_Ip header file are of the same Autosar version */
#if ((SENT_IPW_AR_RELEASE_MAJOR_VERSION_C    != FLEXIO_SENT_IP_AR_RELEASE_MAJOR_VERSION) || \
     (SENT_IPW_AR_RELEASE_MINOR_VERSION_C    != FLEXIO_SENT_IP_AR_RELEASE_MINOR_VERSION) || \
     (SENT_IPW_AR_RELEASE_REVISION_VERSION_C != FLEXIO_SENT_IP_AR_RELEASE_REVISION_VERSION) \
    )
   #error "AutoSar Version Numbers of Flexio_Sent_Ip.h and Sent_Ipw.c are different"
#endif
/* Check if current file and Flexio_Sent_Ip header file are of the same Software version */
#if ((SENT_IPW_SW_MAJOR_VERSION_C != FLEXIO_SENT_IP_SW_MAJOR_VERSION) || \
     (SENT_IPW_SW_MINOR_VERSION_C != FLEXIO_SENT_IP_SW_MINOR_VERSION) || \
     (SENT_IPW_SW_PATCH_VERSION_C != FLEXIO_SENT_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Flexio_Sent_Ip.h and Sent_Ipw.c are different"
#endif
/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                       LOCAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL VARIABLES
===================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

#define SENT_START_SEC_CODE

#include "Sent_MemMap.h"

/**
* @brief   Initialize a Sent channel.
* @details This function initializes all hardware registers needed to
*          start the Sent functionality on the selected channel.
*
* @param[in] u8Channel  Sent channel to be initialized.
*            pConfigPtr Configuration pointer containing hardware specific settings.
*
* @return            void.
* @am: OK
*  
*/
void Sent_Ipw_ControllerInit(uint8 HwCtrlOffset, const Sent_Ipw_CtrlConfigType *IpwController)
{
    (void)Flexio_Sent_Ip_Init(HwCtrlOffset, IpwController->CtrlConfig);
}

/**
* @brief   De initialize a Sent channel.
* @details This function de initializes the hardware registers of an Sent channel
*
* @param[in] u8Channel      Sent channel to be de initialized.
*            pConfigPtr    Configuration pointer containing hardware specific settings.
*
* @return            void.
* @am:  OK
*/
void Sent_Ipw_ControllerDeInit(uint8 HwCtrlOffset)
{
    Flexio_Sent_Ip_DeInit(HwCtrlOffset);
}

/**
* @brief            This function processing polling of Fast notification.
* @details 
*
* @param[in]        ChannelId: The Id of channel
* @return           E_OK:   Channel request has been accepted.
*                   E_NOT_OK:   Channel request has not been accepted.
*
*/
Std_ReturnType Sent_Ipw_GetFastChannelMsgData(uint8 HwCtrlOffset, uint8 ChannelId)
{
    Std_ReturnType RelVal;

    if(STATUS_FLEXIO_SENT_IP_SUCCESS == (Flexio_Sent_Ip_StatusType)Flexio_Sent_Ip_GetFastChannelMsgData(HwCtrlOffset, ChannelId))
    {
        RelVal = E_OK;
    }
    else
    {
        RelVal = E_NOT_OK;
    }

    return (Std_ReturnType)RelVal;
}

/**
* @brief            This function processing polling of Fast notification.
* @details 
*
* @return           E_OK:   Channel request has been accepted.
*                   E_NOT_OK:   Channel request has not been accepted.
*
*/
Std_ReturnType Sent_Ipw_GetFastMsgData(void)
{
    Std_ReturnType RelVal;

    if(STATUS_FLEXIO_SENT_IP_SUCCESS == (Flexio_Sent_Ip_StatusType)Flexio_Sent_Ip_GetFastMsgData())
    {
        RelVal = E_OK;
    }
    else
    {
        RelVal = E_NOT_OK;
    }

    return (Std_ReturnType)RelVal;
}

/**
* @brief            This function processing polling of Fast notification.
* @details 
*
* @param[in]        ChannelId: The Id of channel
* @return           E_OK:   Channel request has been accepted.
*                   E_NOT_OK:   Channel request has not been accepted.
*
*/
Std_ReturnType Sent_Ipw_GetSerialChannelMsgData(uint8 HwCtrlOffset, uint8 ChannelId)
{
    Std_ReturnType RelVal;

    if(STATUS_FLEXIO_SENT_IP_SUCCESS == (Flexio_Sent_Ip_StatusType)Flexio_Sent_Ip_GetSerialChannelMsgData(HwCtrlOffset, ChannelId))
    {
        RelVal = E_OK;
    }
    else
    {
        RelVal = E_NOT_OK;
    }

    return (Std_ReturnType)RelVal;
}

/**
* @brief            This function processing polling of Fast notification.
* @details 
*
* @return           E_OK:   Channel request has been accepted.
*                   E_NOT_OK:   Channel request has not been accepted.
*
*/
Std_ReturnType Sent_Ipw_GetSerialMsgData(void)
{
    Std_ReturnType RelVal;

    if(STATUS_FLEXIO_SENT_IP_SUCCESS == (Flexio_Sent_Ip_StatusType)Flexio_Sent_Ip_GetSerialMsgData())
    {
        RelVal = E_OK;
    }
    else
    {
        RelVal = E_NOT_OK;
    }

    return (Std_ReturnType)RelVal;
}

#define SENT_STOP_SEC_CODE

#include "Sent_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */
