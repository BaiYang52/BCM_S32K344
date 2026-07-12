/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : SIUL2
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

#ifndef SIUL2_DIO_IP_CFG_H
#define SIUL2_DIO_IP_CFG_H

/**
*   @file Siul2_Dio_Ip_Cfg.h
*   @implements Siul2_Dio_Ip_Cfg.h_Artifact
*
*   @addtogroup DIO_CFG
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
[!INDENT "0"!][!//
[!IF "node:exists(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative)"!][!//
    [!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k344')"!][!//
#include "S32K344_SIUL2.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k324')"!][!//
#include "S32K324_SIUL2.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k314')"!][!//
#include "S32K314_SIUL2.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k312')"!][!//
#include "S32K312_SIUL2.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k342')"!][!//
#include "S32K342_SIUL2.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k341')"!][!//
#include "S32K342_SIUL2.h"
    [!ELSEIF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k322')"!][!//
#include "S32K322_SIUL2.h"
    [!ENDIF!][!//
[!ELSE!][!//
    [!ERROR "The Resource plugin or ResourceSubderivative field is not available for header file inclusion."!][!//
[!ENDIF!][!//
[!ENDINDENT!][!//

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define SIUL2_DIO_IP_VENDOR_ID_CFG_H                       43
#define SIUL2_DIO_IP_AR_RELEASE_MAJOR_VERSION_CFG_H        4
#define SIUL2_DIO_IP_AR_RELEASE_MINOR_VERSION_CFG_H        4
#define SIUL2_DIO_IP_AR_RELEASE_REVISION_VERSION_CFG_H     0
#define SIUL2_DIO_IP_SW_MAJOR_VERSION_CFG_H                2
#define SIUL2_DIO_IP_SW_MINOR_VERSION_CFG_H                0
#define SIUL2_DIO_IP_SW_PATCH_VERSION_CFG_H                1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/

/*==================================================================================================
*                                          CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      DEFINES AND MACROS
==================================================================================================*/
[!IF "DioGeneral/DioVirtWrapperSupport"!][!//
[!CODE!][!//
/* Base address for SIUL2_VIRWRAPPER_PDACx */
#define DIO_SIUL2_VIRTWRAPPER_PDAC0    (0x40290000U)
#define DIO_SIUL2_VIRTWRAPPER_PDAC1    (0x40298000U)
#define DIO_SIUL2_VIRTWRAPPER_PDAC2    (0x402A0000U)
[!ENDCODE!][!//
[!ENDIF!][!//

/* Pre-processor switch to enable/disable VirtWrapper support */
#define DIO_VIRTWRAPPER_SUPPORT         ([!IF "DioGeneral/DioVirtWrapperSupport"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/**
* @brief          Pre-processor switch to enable/disable development error detection for Hse Ip API
*
* @implements     Siul2_Dio_Ip_ErrorDetect_define
*/
#define SIUL2_DIO_IP_DEV_ERROR_DETECT   ([!IF "DioGeneral/SIUL2DioIPDevErrorDetect"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!])

/* GPIO - Peripheral instance base addresses */
/** Peripheral PTA base address */
#define PTA_L_HALF ((Siul2_Dio_Ip_GpioType *)(&(IP_SIUL2->PGPDO0)))
#define PTA_H_HALF ((Siul2_Dio_Ip_GpioType *)(&(IP_SIUL2->PGPDO1)))
/** Peripheral PTB base address */
#define PTB_L_HALF ((Siul2_Dio_Ip_GpioType *)(&(IP_SIUL2->PGPDO2)))
#define PTB_H_HALF ((Siul2_Dio_Ip_GpioType *)(&(IP_SIUL2->PGPDO3)))
/** Peripheral PTC base address */
#define PTC_L_HALF ((Siul2_Dio_Ip_GpioType *)(&(IP_SIUL2->PGPDO4)))
#define PTC_H_HALF ((Siul2_Dio_Ip_GpioType *)(&(IP_SIUL2->PGPDO5)))
/** Peripheral PTD base address */
#define PTD_L_HALF ((Siul2_Dio_Ip_GpioType *)(&(IP_SIUL2->PGPDO6)))
#define PTD_H_HALF ((Siul2_Dio_Ip_GpioType *)(&(IP_SIUL2->PGPDO7)))
/** Peripheral PTE base address */
#define PTE_L_HALF ((Siul2_Dio_Ip_GpioType *)(&(IP_SIUL2->PGPDO8)))
#define PTE_H_HALF ((Siul2_Dio_Ip_GpioType *)(&(IP_SIUL2->PGPDO9)))
[!INDENT "0"!][!//
[!IF "contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k314') or contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k324') or contains(as:modconf("Resource")[1]/ResourceGeneral/ResourceSubderivative, 's32k344')"!][!//
    /** Peripheral PTF base address */
    #define PTF_L_HALF ((Siul2_Dio_Ip_GpioType *)(&(IP_SIUL2->PGPDO10)))
    #define PTF_H_HALF ((Siul2_Dio_Ip_GpioType *)(&(IP_SIUL2->PGPDO11)))
    /** Peripheral PTG base address */
    #define PTG_L_HALF ((Siul2_Dio_Ip_GpioType *)(&(IP_SIUL2->PGPDO12)))
    #define PTG_H_HALF ((Siul2_Dio_Ip_GpioType *)(&(IP_SIUL2->PGPDO13)))
[!ENDIF!][!//
[!ENDINDENT!][!//
/*==================================================================================================
*                                             ENUMS
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


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* SIUL2_DIO_IP_CFG_H */
