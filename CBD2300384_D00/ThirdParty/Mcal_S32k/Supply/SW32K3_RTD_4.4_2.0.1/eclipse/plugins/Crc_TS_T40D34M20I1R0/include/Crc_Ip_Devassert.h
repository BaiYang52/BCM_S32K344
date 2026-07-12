/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : 
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

/* Prevention from multiple including the same header */
#ifndef CRC_IP_DEVASSERT_H
#define CRC_IP_DEVASSERT_H

/**
*   @file    Crc_Ip_Devassert.h
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Crc - Crc Ip driver header file.
*   @details
*
*   @addtogroup CRC_IP_DRIVER
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
#include "Mcal.h"
#include "Crc_Ip_Types.h"

/*==================================================================================================
                               SOURCE FILE VERSION INFORMATION
==================================================================================================*/
/**
* @brief Parameters that shall be published within the standard types header file and also in the
*        module's description file
*/
#define CRC_IP_DEVASSERT_VENDOR_ID                     43
#define CRC_IP_DEVASSERT_AR_RELEASE_MAJOR_VERSION      4
#define CRC_IP_DEVASSERT_AR_RELEASE_MINOR_VERSION      4
#define CRC_IP_DEVASSERT_AR_RELEASE_REVISION_VERSION   0
#define CRC_IP_DEVASSERT_SW_MAJOR_VERSION              2
#define CRC_IP_DEVASSERT_SW_MINOR_VERSION              0
#define CRC_IP_DEVASSERT_SW_PATCH_VERSION              1

/*==================================================================================================
                                      FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
/* Check if source file and Mcal header file are of the same Autosar version */
#if ((CRC_IP_DEVASSERT_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
    (CRC_IP_DEVASSERT_AR_RELEASE_MINOR_VERSION  != MCAL_AR_RELEASE_MINOR_VERSION))
    #error "AutoSar Version Numbers of Crc_Ip_DevAssert.h and Mcal header are different"
#endif
#endif

/* Check if source file and Crc header file are of the same vendor */
#if (CRC_IP_DEVASSERT_VENDOR_ID != CRC_IP_TYPES_VENDOR_ID)
    #error "Crc_Ip_DevAssert.h and Crc_Ip_Types have different vendor ids"
#endif

/* Check if source file and Crc header file are of the same Autosar version */
#if ((CRC_IP_DEVASSERT_AR_RELEASE_MAJOR_VERSION    != CRC_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (CRC_IP_DEVASSERT_AR_RELEASE_MINOR_VERSION    != CRC_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (CRC_IP_DEVASSERT_AR_RELEASE_REVISION_VERSION != CRC_IP_TYPES_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Crc_Ip_DevAssert.h and Crc_Ip_Types are different"
#endif

/* Check if source file and Crc header file are of the same Software version */
#if ((CRC_IP_DEVASSERT_SW_MAJOR_VERSION != CRC_IP_TYPES_SW_MAJOR_VERSION) || \
     (CRC_IP_DEVASSERT_SW_MINOR_VERSION != CRC_IP_TYPES_SW_MINOR_VERSION) || \
     (CRC_IP_DEVASSERT_SW_PATCH_VERSION != CRC_IP_TYPES_SW_PATCH_VERSION)    \
    )
    #error "Software Version Numbers of Crc_Ip_DevAssert.h and Crc_Ip_Types are different"
#endif

/*==================================================================================================
                                           TYPEDEF
==================================================================================================*/
#define CRC_START_SEC_CODE
#include "Crc_MemMap.h"

#if (STD_ON == CRC_IP_DEV_ERROR_DETECT)
    /* Implement default assert macro */
    static inline void Crc_Ip_DevAssert(volatile boolean x)
    {
        if (x) { } else { for (;;) {} }
    }
    #define CRC_IP_DEV_ASSERT(x) Crc_Ip_DevAssert(x)
#else
    /* Assert macro does nothing */
    #define CRC_IP_DEV_ASSERT(x) ((void)0)
#endif /* #if (STD_ON == CRC_IP_DEV_ERROR_DETECT) */

#define CRC_STOP_SEC_CODE
#include "Crc_MemMap.h"

#ifdef __cplusplus
}
#endif

/** @} */

#endif  /* #ifndef CRC_IP_DEVASSERT_H */
