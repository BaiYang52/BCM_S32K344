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

#ifndef WDG_IPW_H
#define WDG_IPW_H

/**
* @file
*
* @addtogroup  Wdg
* @{
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

#include "Swt_Ip.h"
#include "Wdg_Ipw_Types.h"
#include "StandardTypes.h"

/*==================================================================================================
*                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define WDG_IPW_VENDOR_ID                    43
#define WDG_IPW_AR_RELEASE_MAJOR_VERSION     4
#define WDG_IPW_AR_RELEASE_MINOR_VERSION     4
#define WDG_IPW_AR_RELEASE_REVISION_VERSION  0
#define WDG_IPW_SW_MAJOR_VERSION             2
#define WDG_IPW_SW_MINOR_VERSION             0
#define WDG_IPW_SW_PATCH_VERSION             1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Wdg_Ipw_Types header file are of the same vendor */
#if (WDG_IPW_VENDOR_ID != WDG_IPW_TYPES_VENDOR_ID)
#error "Wdg_ipw.h and Wdg_Ipw_Types.h have different vendor ids"
#endif

/* Check if current file and Wdg_Ipw_Types header file are of the same Autosar version */
#if ((WDG_IPW_AR_RELEASE_MAJOR_VERSION     != WDG_IPW_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (WDG_IPW_AR_RELEASE_MINOR_VERSION     != WDG_IPW_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (WDG_IPW_AR_RELEASE_REVISION_VERSION  != WDG_IPW_TYPES_AR_RELEASE_REVISION_VERSION))
#error "AutoSar Version Numbers of Wdg_ipw.h and Wdg_Ipw_Types.h are different"
#endif

/* Check if current file and Wdg_Ipw_Types header file are of the same software version */
#if ((WDG_IPW_SW_MAJOR_VERSION != WDG_IPW_TYPES_SW_MAJOR_VERSION) || \
     (WDG_IPW_SW_MINOR_VERSION != WDG_IPW_TYPES_SW_MINOR_VERSION) || \
     (WDG_IPW_SW_PATCH_VERSION != WDG_IPW_TYPES_SW_PATCH_VERSION))
#error "Software Version Numbers of Wdg_ipw.h and Wdg_Ipw_Types.h are different"
#endif

/* Check if current file and Swt_Ip header file are of the same vendor */
#if (WDG_IPW_VENDOR_ID != SWT_IP_VENDOR_ID)
#error "Wdg_Ipw.h and Swt_Ip.h have different vendor ids"
#endif

/* Check if current file and Swt_Ip header file are of the same Autosar version */
#if ((WDG_IPW_AR_RELEASE_MAJOR_VERSION     != SWT_IP_AR_RELEASE_MAJOR_VERSION) || \
     (WDG_IPW_AR_RELEASE_MINOR_VERSION     != SWT_IP_AR_RELEASE_MINOR_VERSION) || \
     (WDG_IPW_AR_RELEASE_REVISION_VERSION  != SWT_IP_AR_RELEASE_REVISION_VERSION))
#error "AutoSar Version Numbers of Wdg_Ipw.h and Swt_Ip.h are different"
#endif

/* Check if current file and Swt_Ip header file are of the same software version */
#if ((WDG_IPW_SW_MAJOR_VERSION != SWT_IP_SW_MAJOR_VERSION) || \
     (WDG_IPW_SW_MINOR_VERSION != SWT_IP_SW_MINOR_VERSION) || \
     (WDG_IPW_SW_PATCH_VERSION != SWT_IP_SW_PATCH_VERSION))
#error "Software Version Numbers of Wdg_Ipw.h and Swt_Ip.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if current file and StandardTypes header file are of the same Autosar version */
    #if ((WDG_IPW_AR_RELEASE_MAJOR_VERSION    != STD_AR_RELEASE_MAJOR_VERSION) || \
        (WDG_IPW_AR_RELEASE_MINOR_VERSION     != STD_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Swt_Ip_Cfg_Defines.h and StandardTypes.h are different"
    #endif
#endif /* DISABLE_MCAL_INTERMODULE_ASR_CHECK */

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                            ENUMS
==================================================================================================*/


/*==================================================================================================
*                                STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/


/*==================================================================================================
*                                GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/


/*==================================================================================================
*                                    FUNCTION PROTOTYPES
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

Std_ReturnType Wdg_Ipw_Init(Wdg_Ipw_InstanceType Instance, const Wdg_Ipw_ConfigType * const IpwConfig);

Std_ReturnType Wdg_Ipw_SetMode(Wdg_Ipw_InstanceType Instance, const Wdg_Ipw_ConfigType * const IpwConfig);

Std_ReturnType Wdg_Ipw_StartTimer(Wdg_Ipw_InstanceType Instance);

#if (WDG_IPW_DEINIT == STD_ON)
    Std_ReturnType Wdg_Ipw_StopTimer(Wdg_Ipw_InstanceType Instance);
    
    Std_ReturnType Wdg_Ipw_Deinit(Wdg_Ipw_InstanceType Instance);
#endif

void Wdg_Ipw_Service(Wdg_Ipw_InstanceType Instance);

#if (WDG_IPW_DEINIT == STD_ON)
#if (WDG_IPW_CLEAR_RESET_REQUEST == STD_ON)
    Wdg_Ipw_StatusType Wdg_Ipw_ClearResetRequest(Wdg_Ipw_InstanceType Instance);
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

#ifdef __cplusplus
}
#endif

/** @} */

#endif /*WDG_IPW_H*/

