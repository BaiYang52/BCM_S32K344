/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : C40_IP IPV_QSPI
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

#ifndef QSPI_IP_CONTROLLER_H
#define QSPI_IP_CONTROLLER_H

/**
*   @file Qspi_Ip_Controller.h
*
*   @addtogroup IPV_QSPI QSPI IPV Driver
*   @{
*/

/* implements Qspi_Ip_Controller.h_Artifact */

#ifdef __cplusplus
extern "C"{
#endif

#include "Qspi_Ip_Types.h"
#include "Qspi_Ip_Cfg.h"
#include "Qspi_Ip_Common.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define QSPI_IP_CONTROLLER_VENDOR_ID_H                       43
#define QSPI_IP_CONTROLLER_AR_RELEASE_MAJOR_VERSION_H        4
#define QSPI_IP_CONTROLLER_AR_RELEASE_MINOR_VERSION_H        4
#define QSPI_IP_CONTROLLER_AR_RELEASE_REVISION_VERSION_H     0
#define QSPI_IP_CONTROLLER_SW_MAJOR_VERSION_H                2
#define QSPI_IP_CONTROLLER_SW_MINOR_VERSION_H                0
#define QSPI_IP_CONTROLLER_SW_PATCH_VERSION_H                1
/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Check if current file and Qspi_Ip_Types.h header file are of the same vendor */
#if (QSPI_IP_CONTROLLER_VENDOR_ID_H != FLS_QSPI_TYPES_VENDOR_ID)
    #error "Qspi_Ip_Controller.h and Qspi_Ip_Types.h have different vendor ids"
#endif
/* Check if current file and Qspi_Ip_Types.h header file are of the same Autosar version */
#if ((QSPI_IP_CONTROLLER_AR_RELEASE_MAJOR_VERSION_H    != FLS_QSPI_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (QSPI_IP_CONTROLLER_AR_RELEASE_MINOR_VERSION_H    != FLS_QSPI_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (QSPI_IP_CONTROLLER_AR_RELEASE_REVISION_VERSION_H != FLS_QSPI_TYPES_AR_RELEASE_REVISION_VERSION) \
    )
    #error "AutoSar Version Numbers of Qspi_Ip_Controller.h and Qspi_Ip_Types.h are different"
#endif
/* Check if current file and Qspi_Ip_Types.h header file are of the same Software version */
#if ((QSPI_IP_CONTROLLER_SW_MAJOR_VERSION_H != FLS_QSPI_TYPES_SW_MAJOR_VERSION) || \
     (QSPI_IP_CONTROLLER_SW_MINOR_VERSION_H != FLS_QSPI_TYPES_SW_MINOR_VERSION) || \
     (QSPI_IP_CONTROLLER_SW_PATCH_VERSION_H != FLS_QSPI_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Qspi_Ip_Controller.h and Qspi_Ip_Types.h are different"
#endif

/* Check if current file and Qspi_Ip_Cfg.h header file are of the same vendor */
#if (QSPI_IP_CONTROLLER_VENDOR_ID_H != QSPI_IP_VENDOR_ID_CFG)
    #error "Qspi_Ip_Controller.h and Qspi_Ip_Cfg.h have different vendor ids"
#endif
/* Check if current file and Qspi_Ip_Cfg.h header file are of the same Autosar version */
#if ((QSPI_IP_CONTROLLER_AR_RELEASE_MAJOR_VERSION_H    != QSPI_IP_AR_RELEASE_MAJOR_VERSION_CFG) || \
     (QSPI_IP_CONTROLLER_AR_RELEASE_MINOR_VERSION_H    != QSPI_IP_AR_RELEASE_MINOR_VERSION_CFG) || \
     (QSPI_IP_CONTROLLER_AR_RELEASE_REVISION_VERSION_H != QSPI_IP_AR_RELEASE_REVISION_VERSION_CFG) \
    )
    #error "AutoSar Version Numbers of Qspi_Ip_Controller.h and Qspi_Ip_Cfg.h are different"
#endif
/* Check if current file and Qspi_Ip_Cfg.h header file are of the same Software version */
#if ((QSPI_IP_CONTROLLER_SW_MAJOR_VERSION_H != QSPI_IP_SW_MAJOR_VERSION_CFG) || \
     (QSPI_IP_CONTROLLER_SW_MINOR_VERSION_H != QSPI_IP_SW_MINOR_VERSION_CFG) || \
     (QSPI_IP_CONTROLLER_SW_PATCH_VERSION_H != QSPI_IP_SW_PATCH_VERSION_CFG) \
    )
    #error "Software Version Numbers of Qspi_Ip_Controller.h and Qspi_Ip_Cfg.h are different"
#endif

/* Check if current file and Qspi_Ip_Common.h header file are of the same vendor */
#if (QSPI_IP_CONTROLLER_VENDOR_ID_H != QSPI_IP_COMMON_VENDOR_ID_H)
    #error "Qspi_Ip_Controller.h and Qspi_Ip_Common.h have different vendor ids"
#endif
/* Check if current file and Qspi_Ip_Common.h header file are of the same Autosar version */
#if ((QSPI_IP_CONTROLLER_AR_RELEASE_MAJOR_VERSION_H    != QSPI_IP_COMMON_AR_RELEASE_MAJOR_VERSION_H) || \
     (QSPI_IP_CONTROLLER_AR_RELEASE_MINOR_VERSION_H    != QSPI_IP_COMMON_AR_RELEASE_MINOR_VERSION_H) || \
     (QSPI_IP_CONTROLLER_AR_RELEASE_REVISION_VERSION_H != QSPI_IP_COMMON_AR_RELEASE_REVISION_VERSION_H) \
    )
    #error "AutoSar Version Numbers of Qspi_Ip_Controller.h and Qspi_Ip_Common.h are different"
#endif
/* Check if current file and Qspi_Ip_Common.h header file are of the same Software version */
#if ((QSPI_IP_CONTROLLER_SW_MAJOR_VERSION_H != QSPI_IP_COMMON_SW_MAJOR_VERSION_H) || \
     (QSPI_IP_CONTROLLER_SW_MINOR_VERSION_H != QSPI_IP_COMMON_SW_MINOR_VERSION_H) || \
     (QSPI_IP_CONTROLLER_SW_PATCH_VERSION_H != QSPI_IP_COMMON_SW_PATCH_VERSION_H) \
    )
    #error "Software Version Numbers of Qspi_Ip_Controller.h and Qspi_Ip_Common.h are different"
#endif

#if (QSPI_IP_MEM_INSTANCE_COUNT > 0)

/*******************************************************************************
 * Enumerations.
 ******************************************************************************/

/*******************************************************************************
* Definitions
******************************************************************************/

#define FLS_START_SEC_CONST_UNSPECIFIED
#include "Fls_MemMap.h"

extern QuadSPI_Type * const Qspi_Ip_BaseAddress[];

#define FLS_STOP_SEC_CONST_UNSPECIFIED
#include "Fls_MemMap.h"


#define FLS_START_SEC_VAR_CLEARED_8
#include "Fls_MemMap.h"

/* The padding bytes information to handle unaligned read/write operation for QuadSPI instances */
extern uint8 Qspi_Ip_MemoryPadding[QuadSPI_INSTANCE_COUNT];

#define FLS_STOP_SEC_VAR_CLEARED_8
#include "Fls_MemMap.h"


/*******************************************************************************
 * API
 ******************************************************************************/
/*!
 * @name QuadSPI Driver
 * @{
 */


#define FLS_START_SEC_CODE
#include "Fls_MemMap.h"



/*!
 * @brief Configures LUT commands
 *
 * This function configures a pair of LUT commands in the specified LUT register.
 * LUT sequences start at index multiple of 4 and can have up to 8 commands
 *
 * @param instance     QuadSPI peripheral instance number
 * @param lut          Index in LUT table
 * @param operation0   First operation
 * @param operation1   Second operation
 * Implements      Qspi_Ip_SetLut_Activity
 */
void Qspi_Ip_SetLut(uint32 instance,
                    uint8 lut,
                    Qspi_Ip_InstrOpType operation0,
                    Qspi_Ip_InstrOpType operation1
                   );

/*!
 * @brief Sets sequence ID for AHB operations
 *
 * @param instance     QuadSPI peripheral instance number
 * @param seqID        Sequence ID in LUT for read operation
 * Implements   Qspi_Ip_SetAhbSeqId_Activity
 */
void Qspi_Ip_SetAhbSeqId_Privileged(uint32 instance,
                                    uint8 seqID
                                   );

/*!
 * @brief Returns the physical base address of a flash device
 *
 * This function returns the physical base address of a flash device, depending on the QSPI connection.
 * The controller must be initialized prior to calling this function.
 *
 * @param instance        QuadSPI peripheral instance number
 * @param connectionType  Connection of the flash device to QSPI
 */
uint32 Qspi_Ip_GetBaseAdress(uint32 instance,
                             Qspi_Ip_ConnectionType connectionType
                            );

/*!
 * @brief Launches a simple IP command
 *
 * @param instance     QuadSPI peripheral instance number
 * @param lut          Index of LUT register
 * @param addr         Address of the target serial flash
 * @return    Error or success status returned by API
 */
Qspi_Ip_StatusType Qspi_Ip_IpCommand(uint32 instance,
                                     uint8 lut,
                                     uint32 addr
                                    );


/*!
 * @brief Launches an IP read command
 *
 * This function can launch a read command in 3 modes:
 * - normal read (dataRead != NULL_PTR): Data is read from serial flash and placed in the buffer
 * - verify (dataRead == NULL_PTR, dataCmp != NULL_PTR): Data is read from serial flash and compared to the reference buffer
 * - blank check (dataRead == NULL_PTR, dataCmp == NULL_PTR): Data is read from serial flash and compared to 0xFF
 * Only normal read mode can use DMA.
 *
 * @param instance       QuadSPI peripheral instance number
 * @param lut            Index of LUT register
 * @param addr           Start address for read operation in serial flash
 * @param dataRead       Buffer where to store read data
 * @param dataCmp        Buffer to be compared to read data
 * @param size           Size of data buffer
 * @return    Error or success status returned by API
 */
Qspi_Ip_StatusType Qspi_Ip_IpRead(uint32 instance,
                                  uint8 lut,
                                  uint32 addr,
                                  uint8 * dataRead,
                                  const uint8 * dataCmp,
                                  uint32 size
                                 );


/*!
 * @brief Launches an IP write command
 *
 * @param instance       QuadSPI peripheral instance number
 * @param lut            Index of LUT register
 * @param addr           Start address for write operation in serial flash
 * @param data           Data to be programmed in flash
 * @param size           Size of data buffer
 * @return    Error or success status returned by API
 */
Qspi_Ip_StatusType Qspi_Ip_IpWrite(uint32 instance,
                                   uint8 lut,
                                   uint32 addr,
                                   const uint8 * data,
                                   uint32 size
                                  );


#define FLS_STOP_SEC_CODE
#include "Fls_MemMap.h"

#endif /* (QSPI_IP_MEM_INSTANCE_COUNT > 0) */

#ifdef __cplusplus
}
#endif

/** @} */

#endif /* QSPI_IP_CONTROLLER_H */
