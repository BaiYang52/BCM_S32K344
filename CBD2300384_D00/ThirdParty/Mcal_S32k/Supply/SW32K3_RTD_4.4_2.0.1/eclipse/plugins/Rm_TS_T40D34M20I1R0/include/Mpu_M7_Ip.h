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
#ifndef MPU_M7_IP_H
#define MPU_M7_IP_H

/**
*   @file Mpu_M7_Ip.h
*
*   @addtogroup Mpu_M7_Ip Mpu M7 IPV Driver
*   @{
*/


/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "Mpu_M7_Ip_Cfg.h"
#include "Mpu_M7_Ip_Cfg_Defines.h"
#include "Mpu_M7_Ip_DeviceRegisters.h"
#include "Mpu_M7_Ip_Types.h"
#include "Devassert.h"
#include "BasicTypes.h"
#include "Mcal.h"

#ifdef MPU_M7_ENABLE_CLEAR_CACHE
#if (MPU_M7_ENABLE_CLEAR_CACHE == STD_ON)
#include "Cache_Ip.h"
#endif
#endif

#ifdef MPU_M7_IP_ENABLE_USER_MODE_SUPPORT
    #include "OsIf.h"
#endif

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_MPU_M7_IP_VENDOR_ID                      43
#define RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION       4
#define RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION       4
#define RM_MPU_M7_IP_AR_RELEASE_REVISION_VERSION    0
#define RM_MPU_M7_IP_SW_MAJOR_VERSION               2
#define RM_MPU_M7_IP_SW_MINOR_VERSION               0
#define RM_MPU_M7_IP_SW_PATCH_VERSION               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Mpu_M7_Ip_Types.h */
#if (RM_MPU_M7_IP_VENDOR_ID != RM_MPU_M7_IP_TYPES_VENDOR_ID)
    #error "Mpu_M7_Ip.h and Mpu_M7_Ip_Types.h have different vendor ids"
#endif
#if ((RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION    != RM_MPU_M7_IP_TYPES_AR_RELEASE_MAJOR_VERSION) || \
     (RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION    != RM_MPU_M7_IP_TYPES_AR_RELEASE_MINOR_VERSION) || \
     (RM_MPU_M7_IP_AR_RELEASE_REVISION_VERSION != RM_MPU_M7_IP_TYPES_AR_RELEASE_REVISION_VERSION) \
    )
     #error "AUTOSAR Version Numbers of Mpu_M7_Ip.h and Mpu_M7_Ip_Types.h are different"
#endif
#if ((RM_MPU_M7_IP_SW_MAJOR_VERSION != RM_MPU_M7_IP_TYPES_SW_MAJOR_VERSION) || \
     (RM_MPU_M7_IP_SW_MINOR_VERSION != RM_MPU_M7_IP_TYPES_SW_MINOR_VERSION) || \
     (RM_MPU_M7_IP_SW_PATCH_VERSION != RM_MPU_M7_IP_TYPES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Mpu_M7_Ip.h and Mpu_M7_Ip_Types.h are different"
#endif

/* Checks against Mpu_M7_Ip_DeviceRegisters.h */
#if (RM_MPU_M7_IP_VENDOR_ID != RM_MPU_M7_IP_DEVICE_REGISTERS_VENDOR_ID)
    #error "Mpu_M7_Ip.h and Mpu_M7_Ip_DeviceRegisters.h have different vendor ids"
#endif
#if ((RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION    != RM_MPU_M7_IP_DEVICE_REGISTERS_AR_RELEASE_MAJOR_VERSION) || \
     (RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION    != RM_MPU_M7_IP_DEVICE_REGISTERS_AR_RELEASE_MINOR_VERSION) || \
     (RM_MPU_M7_IP_AR_RELEASE_REVISION_VERSION != RM_MPU_M7_IP_DEVICE_REGISTERS_AR_RELEASE_REVISION_VERSION) \
    )
     #error "AUTOSAR Version Numbers of Mpu_M7_Ip.h and Mpu_M7_Ip_DeviceRegisters.h are different"
#endif
#if ((RM_MPU_M7_IP_SW_MAJOR_VERSION != RM_MPU_M7_IP_DEVICE_REGISTERS_SW_MAJOR_VERSION) || \
     (RM_MPU_M7_IP_SW_MINOR_VERSION != RM_MPU_M7_IP_DEVICE_REGISTERS_SW_MINOR_VERSION) || \
     (RM_MPU_M7_IP_SW_PATCH_VERSION != RM_MPU_M7_IP_DEVICE_REGISTERS_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Mpu_M7_Ip.h and Mpu_M7_Ip_DeviceRegisters.h are different"
#endif

/* Checks against Mpu_M7_Ip_Cfg.h */
#if (RM_MPU_M7_IP_VENDOR_ID != RM_MPU_M7_IP_CFG_VENDOR_ID)
    #error "Mpu_M7_Ip.h and Mpu_M7_Ip_Cfg.h have different vendor ids"
#endif
#if ((RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION    != RM_MPU_M7_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION    != RM_MPU_M7_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (RM_MPU_M7_IP_AR_RELEASE_REVISION_VERSION != RM_MPU_M7_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
     #error "AUTOSAR Version Numbers of Mpu_M7_Ip.h and Mpu_M7_Ip_Cfg.h are different"
#endif
#if ((RM_MPU_M7_IP_SW_MAJOR_VERSION != RM_MPU_M7_IP_CFG_SW_MAJOR_VERSION) || \
     (RM_MPU_M7_IP_SW_MINOR_VERSION != RM_MPU_M7_IP_CFG_SW_MINOR_VERSION) || \
     (RM_MPU_M7_IP_SW_PATCH_VERSION != RM_MPU_M7_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Mpu_M7_Ip.h and Mpu_M7_Ip_Cfg.h are different"
#endif

/* Checks against Mpu_M7_Ip_Cfg_Defines.h */
#if (RM_MPU_M7_IP_VENDOR_ID != RM_MPU_M7_IP_CFG_DEFINES_VENDOR_ID)
    #error "Mpu_M7_Ip.h and Mpu_M7_Ip_Cfg_Defines.h have different vendor ids"
#endif
#if ((RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION    != RM_MPU_M7_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION) || \
     (RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION    != RM_MPU_M7_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION) || \
     (RM_MPU_M7_IP_AR_RELEASE_REVISION_VERSION != RM_MPU_M7_IP_CFG_DEFINES_AR_RELEASE_REVISION_VERSION) \
    )
     #error "AUTOSAR Version Numbers of Mpu_M7_Ip.h and Mpu_M7_Ip_Cfg_Defines.h are different"
#endif
#if ((RM_MPU_M7_IP_SW_MAJOR_VERSION != RM_MPU_M7_IP_CFG_DEFINES_SW_MAJOR_VERSION) || \
     (RM_MPU_M7_IP_SW_MINOR_VERSION != RM_MPU_M7_IP_CFG_DEFINES_SW_MINOR_VERSION) || \
     (RM_MPU_M7_IP_SW_PATCH_VERSION != RM_MPU_M7_IP_CFG_DEFINES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Mpu_M7_Ip.h and Mpu_M7_Ip_Cfg_Defines.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Check if the files Mpu_M7_Ip.h and Devassert.h are of the same version */
    #if ((RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION != DEVASSERT_AR_RELEASE_MAJOR_VERSION) || \
        (RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION != DEVASSERT_AR_RELEASE_MINOR_VERSION)     \
        )
        #error "AUTOSAR Version Numbers of Mpu_M7_Ip.h and Devassert.h are different"
    #endif

    /* Checks against Mcal.h */
    #if ((RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION != MCAL_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AUTOSAR Version Numbers of Mpu_M7_Ip.h and Mcal.h are different"
    #endif

    #ifdef MPU_M7_IP_ENABLE_USER_MODE_SUPPORT
        /* Checks against OsIf.h */
        #if ((RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION != OSIF_AR_RELEASE_MAJOR_VERSION) || \
             (RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION != OSIF_AR_RELEASE_MINOR_VERSION) \
            )
            #error "AUTOSAR Version Numbers of Mpu_M7_Ip.h and OsIf.h are different"
        #endif
    #endif

#ifdef MPU_M7_ENABLE_CLEAR_CACHE
#if (MPU_M7_ENABLE_CLEAR_CACHE == STD_ON)
    /* Check if current file and Cache_Ip.h header file are of the same Autosar version */
    #if ((RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION != CACHE_IP_AR_RELEASE_MAJOR_VERSION) || \
         (RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION != CACHE_IP_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AutoSar Version Numbers of Mpu_M7_Ip.h and Cache_Ip.h are different"
    #endif
#endif
#endif

#endif
/*==================================================================================================
*                                      FUNCTION PROTOTYPES
==================================================================================================*/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

#ifdef  RM_IP_ENABLE_MPU_M7
#if  (RM_IP_ENABLE_MPU_M7 == STD_ON)

#define RM_START_SEC_CODE
#include "Rm_MemMap.h"

/**
 * @brief         Initializes the Memory Protection Unit general parameters and
 *                region configurations
 *
 * @details       This function is non-reentrant
 *
 * @param[in]     pConfig: pointer to configuration structure for MPU M7.
 * @return        void
 *
 * @api
 *
 * @pre
 *
 * */
void Mpu_M7_Ip_Init(const Mpu_M7_Ip_ConfigType * pConfig);

/**
 * @brief         Configures the region selected by u8RegionNum with the data from pUserConfigPtr
 *
 * @details       This function is Reentrant
 *
 * @param[in]     u8RegionNum      Region number
 * @param[in]     pUserConfigPtr   Pointer to the region configuration structure for MPU M7.
 * @return        void
 *
 * @api
 *
 * @pre
 *
 * */
void Mpu_M7_Ip_SetRegionConfig(uint8 u8RegionNum, const Mpu_M7_Ip_RegionConfigType * const pUserConfigPtr);

/**
 * @brief         Disables the module and resets all region configurations
 *
 * @details       This function is Reentrant
 *
 * @return        void
 *
 * @api
 *
 * @pre
 * */
void Mpu_M7_Ip_Deinit(void);

/**
 * @brief         Enables or disabled a specific region
 *
 * @details       This function is Reentrant
 *
 * @param[in]     u8RegionNum : Region to be modified
 * @param[in]     bEnable  : Specifies wheter the region is enabled or disabled
 * @return        void
 *
 * @api
 *
 * @pre
 *
 * */
void Mpu_M7_Ip_EnableRegion(uint8 u8RegionNum, boolean bEnable);

/**
 * @brief         Modify the access rights for a specific region
 *
 * @details       This function is Reentrant
 *
 * @param[in]     u8RegionNum : Region to be modified
 * @param[in]     eRights     : Specifies the new access rights
 * @return        void
 *
 * @api
 *
 * @pre
 *
 * */
void Mpu_M7_Ip_SetAccessRight(uint8 u8RegionNum, Mpu_M7_Ip_AccessRightsType eRights);

/**
 * @brief         Retrieve error details
 *
 * @details       This function is Reentrant
 *
 * @param[out]    pErrorDetails : Storage where the data will be saved
 *
 * @return        boolean
 * @retval        TRUE       if an error was present
 * @retval        FALSE      otherwise
 *
 * @api
 *
 * @pre
 *
 * */
boolean Mpu_M7_Ip_GetErrorDetails(Mpu_M7_Ip_ErrorDetailsType * pErrorDetails);

#if (MPU_M7_IP_MULTICORE_SUPPORT == STD_ON)
/**
 * @brief         Initializes the Memory Protection Unit general parameters and
 *                region configurations
 *
 * @details       This function is non-reentrant
 *
 * @api
 *
 * @pre
 *
 * */
void Mpu_M7_Ip_MulticoreInit(void);
#endif

#define RM_STOP_SEC_CODE
#include "Rm_MemMap.h"

#endif
#endif /* RM_IP_ENABLE_MPU_M7*/

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/** @} */

#endif /* MPU_M7_IP_H */

