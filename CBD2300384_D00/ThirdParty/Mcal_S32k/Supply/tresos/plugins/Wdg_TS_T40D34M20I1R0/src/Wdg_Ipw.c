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
*   @addtogroup Wdg
*   @{
*/

/*==================================================================================================
*                                        INCLUDE FILES
* 1) system and project includes
* 2) needed interfaces from external units
* 3) internal and external interfaces from this unit
==================================================================================================*/

#include "Wdg_Ipw.h"

/*==================================================================================================
*                                 SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define WDG_IPW_VENDOR_ID_C                      43
#define WDG_IPW_AR_RELEASE_MAJOR_VERSION_C       4
#define WDG_IPW_AR_RELEASE_MINOR_VERSION_C       4
#define WDG_IPW_AR_RELEASE_REVISION_VERSION_C    0
#define WDG_IPW_SW_MAJOR_VERSION_C               2
#define WDG_IPW_SW_MINOR_VERSION_C               0
#define WDG_IPW_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                       FILE VERSION CHECKS
==================================================================================================*/

/* Check if current file and Wdg_Ipw header file are of the same vendor */
#if (WDG_IPW_VENDOR_ID_C != WDG_IPW_VENDOR_ID)
    #error "Wdg_Ipw.c and Wdg_Ipw.h have different vendor ids"
#endif

#if ((WDG_IPW_AR_RELEASE_MAJOR_VERSION_C     != WDG_IPW_AR_RELEASE_MAJOR_VERSION) || \
     (WDG_IPW_AR_RELEASE_MINOR_VERSION_C     != WDG_IPW_AR_RELEASE_MINOR_VERSION) || \
     (WDG_IPW_AR_RELEASE_REVISION_VERSION_C  != WDG_IPW_AR_RELEASE_REVISION_VERSION))
#error "AutoSar Version Numbers of Wdg_Ipw.c and Wdg_Ipw.h are different"
#endif

#if ((WDG_IPW_SW_MAJOR_VERSION_C != WDG_IPW_SW_MAJOR_VERSION) || \
     (WDG_IPW_SW_MINOR_VERSION_C != WDG_IPW_SW_MINOR_VERSION) || \
     (WDG_IPW_SW_PATCH_VERSION_C != WDG_IPW_SW_PATCH_VERSION))
#error "Software Version Numbers of Wdg_Ipw.c and Wdg_Ipw.h are different"
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

Std_ReturnType Wdg_Ipw_Init(Wdg_Ipw_InstanceType Instance, const Wdg_Ipw_ConfigType * const IpwConfig)
{
    Std_ReturnType Ret = (Std_ReturnType) E_OK;

    /* Initialize Wdg module */
    if (Swt_Ip_Init((uint32)Instance, IpwConfig->aSwtConfig) != SWT_IP_STATUS_SUCCESS)
    {
        Ret = (Std_ReturnType) E_NOT_OK;
    }

    return Ret;
}

Std_ReturnType Wdg_Ipw_SetMode(Wdg_Ipw_InstanceType Instance, const Wdg_Ipw_ConfigType * const IpwConfig)
{
    Std_ReturnType Ret = (Std_ReturnType) E_OK;

    /* Reconfigure Wdg module */
    if (Swt_Ip_Config((uint32)Instance, IpwConfig->aSwtConfig) != SWT_IP_STATUS_SUCCESS)
    {
        Ret = (Std_ReturnType) E_NOT_OK;
    }

    return Ret;
}

Std_ReturnType Wdg_Ipw_StartTimer(Wdg_Ipw_InstanceType Instance)
{
    Std_ReturnType Ret = (Std_ReturnType) E_OK;

    /* Starts the timer for the Wdg module */
    if (Swt_Ip_StartTimer((uint32)Instance) != SWT_IP_STATUS_SUCCESS)
    {
        Ret = (Std_ReturnType) E_NOT_OK;
    }

    return Ret;
}

#if (WDG_IPW_DEINIT == STD_ON)
    Std_ReturnType Wdg_Ipw_StopTimer(Wdg_Ipw_InstanceType Instance)
    {
        Std_ReturnType Ret = (Std_ReturnType) E_OK;

        /* Stops the timer for the Wdg module */
        if (Swt_Ip_StopTimer((uint32)Instance) != SWT_IP_STATUS_SUCCESS)
        {
            Ret = (Std_ReturnType) E_NOT_OK;
        }

        return Ret;
    }

    Std_ReturnType Wdg_Ipw_Deinit(Wdg_Ipw_InstanceType Instance)
    {
        Std_ReturnType Ret = (Std_ReturnType) E_OK;

        /* De-initializes Wdg module */
        if (Swt_Ip_Deinit((uint32)Instance) != SWT_IP_STATUS_SUCCESS)
        {
            Ret = (Std_ReturnType) E_NOT_OK;
        }

        return Ret;
    }
#endif

void Wdg_Ipw_Service(Wdg_Ipw_InstanceType Instance)
{
    /* Services Wdg module */
    Swt_Ip_Service((uint32)Instance);
}

#if (WDG_IPW_DEINIT == STD_ON)
#if (WDG_IPW_CLEAR_RESET_REQUEST == STD_ON)
    Wdg_Ipw_StatusType Wdg_Ipw_ClearResetRequest(Wdg_Ipw_InstanceType Instance)
    {
        Wdg_Ipw_StatusType Ret      = WDG_IPW_STATUS_SUCCESS;
        Swt_Ip_StatusType RetClrReq = SWT_IP_STATUS_SUCCESS;

        /* Clears reset request */
        RetClrReq =  Swt_Ip_ClearResetRequest((uint32)Instance);

        if (SWT_IP_STATUS_SUCCESS == RetClrReq)
        {
            Ret = WDG_IPW_STATUS_SUCCESS;
        }
        else 
        {
            if (SWT_IP_STATUS_TIMEOUT == RetClrReq)
            {
                Ret = WDG_IPW_STATUS_TIMEOUT;
            }
            else
            {
                Ret = WDG_IPW_STATUS_ERROR;
            }
        }

        return Ret;
    }
#endif
#endif

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

/** @} */
