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

#ifndef CRC_TYPES_H
#define CRC_TYPES_H

/**
*   @file    Crc_Types.h
*
*   @version 2.0.1
*
*   @brief   AUTOSAR Crc - Crc Ip driver source file.
*   @details
*
*   @addtogroup CRC_HLD_DRIVER
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

#include "Crc_Ip_Types.h"

/*=================================================================================================
*                              SOURCE FILE VERSION INFORMATION
=================================================================================================*/
/**
* @brief Parameters that shall be published within the Crc driver header file and also in the
*        module's description file
*/
#define CRC_TYPES_VENDOR_ID                      43
#define CRC_TYPES_AR_RELEASE_MAJOR_VERSION       4
#define CRC_TYPES_AR_RELEASE_MINOR_VERSION       4
#define CRC_TYPES_AR_RELEASE_REVISION_VERSION    0
#define CRC_TYPES_SW_MAJOR_VERSION               2
#define CRC_TYPES_SW_MINOR_VERSION               0
#define CRC_TYPES_SW_PATCH_VERSION               1

/*=================================================================================================
*                                     FILE VERSION CHECKS
=================================================================================================*/
/* Check if the files Crc_Types.h and Crc_Ip_Types.h are of the same version */
#if (CRC_TYPES_VENDOR_ID != CRC_IP_TYPES_VENDOR_ID)
    #error "Crc_Types.h and Crc_Ip_Types.h have different vendor ids"
#endif

/* Check if Crc_Types.h and Crc_Ip_Types.h are of the same Autosar version */
#if ((CRC_TYPES_AR_RELEASE_MAJOR_VERSION    != CRC_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (CRC_TYPES_AR_RELEASE_MINOR_VERSION    != CRC_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (CRC_TYPES_AR_RELEASE_REVISION_VERSION != CRC_IP_TYPES_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Crc_Types.h and Crc_Ip_Types.h are different"
#endif

/* Check if Crc_Types.h and Crc_Ip_Types.h are of the same Software version */
#if ((CRC_TYPES_SW_MAJOR_VERSION != CRC_IP_TYPES_SW_MAJOR_VERSION) || \
     (CRC_TYPES_SW_MINOR_VERSION != CRC_IP_TYPES_SW_MINOR_VERSION) || \
     (CRC_TYPES_SW_PATCH_VERSION != CRC_IP_TYPES_SW_PATCH_VERSION)    \
    )
    #error "Software Version Numbers of Crc_Types.h and Crc_Ip_Types.h are different"
#endif
/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/

/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/

/**
* @brief         CRC Partition Configuration Type.
* @details       The Channel is identified by the following Partition
*/
typedef struct
{
    const uint32 ChannelMax;
    const uint32 * ChannelList;
} Crc_PartitionType;

/**
* @brief          Initialization data for the CRC driver.
* @details        A pointer to such a structure is provided to the CRC initialization routines for
*                 configuration.
* @implements     Crc_InitType_structure
*/
typedef struct
{
    const Crc_Ip_InitType *const pxCrcIpConfigInit;
} Crc_InitType;

/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CRC_TYPES_H */
