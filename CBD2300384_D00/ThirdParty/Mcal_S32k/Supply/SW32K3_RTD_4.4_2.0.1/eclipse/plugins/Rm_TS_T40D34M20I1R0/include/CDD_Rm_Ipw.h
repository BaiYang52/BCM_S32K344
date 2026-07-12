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
#ifndef CDD_RM_IPW_H
#define CDD_RM_IPW_H

/**
*   @file CDD_Rm_Ipw.h
*
*   @addtogroup RM_MODULE RM Driver
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
#include "CDD_Rm_Ipw_Cfg.h"

#if (RM_IPW_ENABLE_XRDC == STD_ON)
    #include "Xrdc_Ip.h"
#endif

#if (RM_IPW_ENABLE_SEMA42 == STD_ON)
    #include "Sema42_Ip.h"
#endif

#if (RM_IPW_ENABLE_MPU_M7 == STD_ON)
    #include "Mpu_M7_Ip.h"
#endif

#if (RM_IPW_ENABLE_PFLASH == STD_ON)
    #include "Pflash_Ip.h"
#endif

#if (RM_IPW_ENABLE_AXBS == STD_ON)
    #include "Axbs_Ip.h"
#endif

#if (RM_IPW_ENABLE_XBIC == STD_ON)
    #include "Xbic_Ip.h"
#endif

#if (RM_IPW_ENABLE_VIRT_WRAPPER == STD_ON)
    #include "Virt_Wrapper_Ip.h"
#endif

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_IPW_VENDOR_ID                    43
#define RM_IPW_AR_RELEASE_MAJOR_VERSION     4
#define RM_IPW_AR_RELEASE_MINOR_VERSION     4
#define RM_IPW_AR_RELEASE_REVISION_VERSION  0
#define RM_IPW_SW_MAJOR_VERSION             2
#define RM_IPW_SW_MINOR_VERSION             0
#define RM_IPW_SW_PATCH_VERSION             1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against CDD_Rm_Ipw_Cfg.h */
#if (RM_IPW_VENDOR_ID != RM_IPW_CFG_VENDOR_ID)
    #error "CDD_Rm_Ipw.h and CDD_Rm_Ipw_Cfg.h have different vendor ids"
#endif
#if ((RM_IPW_AR_RELEASE_MAJOR_VERSION    != RM_IPW_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_AR_RELEASE_MINOR_VERSION    != RM_IPW_CFG_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_AR_RELEASE_REVISION_VERSION != RM_IPW_CFG_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw.h and CDD_Rm_Ipw_Cfg.h are different"
#endif
#if ((RM_IPW_SW_MAJOR_VERSION != RM_IPW_CFG_SW_MAJOR_VERSION) || \
     (RM_IPW_SW_MINOR_VERSION != RM_IPW_CFG_SW_MINOR_VERSION) || \
     (RM_IPW_SW_PATCH_VERSION != RM_IPW_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw.h and CDD_Rm_Ipw_Cfg.h are different"
#endif

#if (RM_IPW_ENABLE_XRDC == STD_ON)
/* Checks against Xrdc_Ip.h */
#if (RM_IPW_VENDOR_ID != RM_XRDC_IP_VENDOR_ID)
    #error "CDD_Rm_Ipw.h and Xrdc_Ip.h have different vendor ids"
#endif
#if ((RM_IPW_AR_RELEASE_MAJOR_VERSION    != RM_XRDC_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_AR_RELEASE_MINOR_VERSION    != RM_XRDC_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_AR_RELEASE_REVISION_VERSION != RM_XRDC_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw.h and Xrdc_Ip.h are different"
#endif
#if ((RM_IPW_SW_MAJOR_VERSION != RM_XRDC_IP_SW_MAJOR_VERSION) || \
     (RM_IPW_SW_MINOR_VERSION != RM_XRDC_IP_SW_MINOR_VERSION) || \
     (RM_IPW_SW_PATCH_VERSION != RM_XRDC_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw.h and Xrdc_Ip.h are different"
#endif
#endif

#if (RM_IPW_ENABLE_SEMA42 == STD_ON)
/* Checks against Sema42_Ip.h */
#if (RM_IPW_VENDOR_ID != RM_SEMA42_IP_VENDOR_ID)
    #error "CDD_Rm_Ipw.h and Sema42_Ip.h have different vendor ids"
#endif
#if ((RM_IPW_AR_RELEASE_MAJOR_VERSION    != RM_SEMA42_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_AR_RELEASE_MINOR_VERSION    != RM_SEMA42_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_AR_RELEASE_REVISION_VERSION != RM_SEMA42_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw.h and Sema42_Ip.h are different"
#endif
#if ((RM_IPW_SW_MAJOR_VERSION != RM_SEMA42_IP_SW_MAJOR_VERSION) || \
     (RM_IPW_SW_MINOR_VERSION != RM_SEMA42_IP_SW_MINOR_VERSION) || \
     (RM_IPW_SW_PATCH_VERSION != RM_SEMA42_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw.h and Sema42_Ip.h are different"
#endif
#endif

#if (RM_IPW_ENABLE_MPU_M7 == STD_ON)
/* Checks against Mpu_M7_Ip.h */
#if (RM_IPW_VENDOR_ID != RM_MPU_M7_IP_VENDOR_ID)
    #error "CDD_Rm_Ipw.h and Mpu_M7_Ip.h have different vendor ids"
#endif
#if ((RM_IPW_AR_RELEASE_MAJOR_VERSION    != RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_AR_RELEASE_MINOR_VERSION    != RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_AR_RELEASE_REVISION_VERSION != RM_MPU_M7_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw.h and Mpu_M7_Ip.h are different"
#endif
#if ((RM_IPW_SW_MAJOR_VERSION != RM_MPU_M7_IP_SW_MAJOR_VERSION) || \
     (RM_IPW_SW_MINOR_VERSION != RM_MPU_M7_IP_SW_MINOR_VERSION) || \
     (RM_IPW_SW_PATCH_VERSION != RM_MPU_M7_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw.h and Mpu_M7_Ip.h are different"
#endif
#endif

#if (RM_IPW_ENABLE_PFLASH == STD_ON)
/* Checks against Pflash_Ip.h */
#if (RM_IPW_VENDOR_ID != RM_PFLASH_IP_VENDOR_ID)
    #error "CDD_Rm_Ipw.h and Pflash_Ip.h have different vendor ids"
#endif
#if ((RM_IPW_AR_RELEASE_MAJOR_VERSION    != RM_PFLASH_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_AR_RELEASE_MINOR_VERSION    != RM_PFLASH_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_AR_RELEASE_REVISION_VERSION != RM_PFLASH_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw.h and Pflash_Ip.h are different"
#endif
#if ((RM_IPW_SW_MAJOR_VERSION != RM_PFLASH_IP_SW_MAJOR_VERSION) || \
     (RM_IPW_SW_MINOR_VERSION != RM_PFLASH_IP_SW_MINOR_VERSION) || \
     (RM_IPW_SW_PATCH_VERSION != RM_PFLASH_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw.h and Pflash_Ip.h are different"
#endif
#endif

#if (RM_IPW_ENABLE_AXBS == STD_ON)
/* Checks against Axbs_Ip.h */
#if (RM_IPW_VENDOR_ID != RM_AXBS_IP_VENDOR_ID)
    #error "CDD_Rm_Ipw.h and Axbs_Ip.h have different vendor ids"
#endif
#if ((RM_IPW_AR_RELEASE_MAJOR_VERSION    != RM_AXBS_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_AR_RELEASE_MINOR_VERSION    != RM_AXBS_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_AR_RELEASE_REVISION_VERSION != RM_AXBS_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw.h and Axbs_Ip.h are different"
#endif
#if ((RM_IPW_SW_MAJOR_VERSION != RM_AXBS_IP_SW_MAJOR_VERSION) || \
     (RM_IPW_SW_MINOR_VERSION != RM_AXBS_IP_SW_MINOR_VERSION) || \
     (RM_IPW_SW_PATCH_VERSION != RM_AXBS_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw.h and Axbs_Ip.h are different"
#endif
#endif

#if (RM_IPW_ENABLE_XBIC == STD_ON)
/* Checks against Xbic_Ip.h */
#if (RM_IPW_VENDOR_ID != RM_XBIC_IP_VENDOR_ID)
    #error "CDD_Rm_Ipw.h and Xbic_Ip.h have different vendor ids"
#endif
#if ((RM_IPW_AR_RELEASE_MAJOR_VERSION    != RM_XBIC_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_AR_RELEASE_MINOR_VERSION    != RM_XBIC_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_AR_RELEASE_REVISION_VERSION != RM_XBIC_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw.h and Xbic_Ip.h are different"
#endif
#if ((RM_IPW_SW_MAJOR_VERSION != RM_XBIC_IP_SW_MAJOR_VERSION) || \
     (RM_IPW_SW_MINOR_VERSION != RM_XBIC_IP_SW_MINOR_VERSION) || \
     (RM_IPW_SW_PATCH_VERSION != RM_XBIC_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw.h and Xbic_Ip.h are different"
#endif
#endif

#if (RM_IPW_ENABLE_VIRT_WRAPPER == STD_ON)
/* Checks against Virt_Wrapper_Ip.h */
#if (RM_IPW_VENDOR_ID != RM_VIRT_WRAPPER_IP_VENDOR_ID)
    #error "CDD_Rm_Ipw.h and Virt_Wrapper_Ip.h have different vendor ids"
#endif
#if ((RM_IPW_AR_RELEASE_MAJOR_VERSION    != RM_VIRT_WRAPPER_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_IPW_AR_RELEASE_MINOR_VERSION    != RM_VIRT_WRAPPER_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_IPW_AR_RELEASE_REVISION_VERSION != RM_VIRT_WRAPPER_IP_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of CDD_Rm_Ipw.h and Virt_Wrapper_Ip.h are different"
#endif
#if ((RM_IPW_SW_MAJOR_VERSION != RM_VIRT_WRAPPER_IP_SW_MAJOR_VERSION) || \
     (RM_IPW_SW_MINOR_VERSION != RM_VIRT_WRAPPER_IP_SW_MINOR_VERSION) || \
     (RM_IPW_SW_PATCH_VERSION != RM_VIRT_WRAPPER_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of CDD_Rm_Ipw.h and Virt_Wrapper_Ip.h are different"
#endif
#endif

/*==================================================================================================
                                           CONSTANTS
==================================================================================================*/

/*==================================================================================================
                                       DEFINES AND MACROS
==================================================================================================*/

#if (RM_IPW_ENABLE_XRDC == STD_ON)
#define RM_XRDC_UNKNOWN_DOMAIN                (XRDC_UNKNOWN_DOMAIN)

#endif

#if (RM_IPW_ENABLE_SEMA42 == STD_ON)
#define RM_SEMAPHORE_FREE                     (SEMA42_IP_UNLOCKED_GATE)

#define RM_SEMAPHORE_UNKNOWN_DOMAINID         (SEMA42_IP_UNKNOWN_DOMAINID)
#endif

/*==================================================================================================
                                             ENUMS
==================================================================================================*/

/*==================================================================================================
                                 STRUCTURES AND OTHER TYPEDEFS
==================================================================================================*/
#if (RM_IPW_ENABLE_XRDC == STD_ON)
/** Enumeration listing secure attributes of a XRDC master */
typedef Xrdc_Ip_SecureAttributeType   Rm_XrdcSecureAttributeType;

/** Structure used to retrieve information violation details and the domain ID where violation occured. */
typedef Xrdc_Ip_DomainIDErrStatusType Rm_XrdcDomainIDErrStatusType;

/** Master core type, use for PID feature*/
typedef Xrdc_Ip_MasterCoreType        Rm_XrdcMasterCoreType;

/** Domain ID */
typedef uint32                        Rm_DomainIdType;

typedef Xrdc_Ip_InstanceConfigType Rm_XrdcInstanceConfigType;
#endif

#if (RM_IPW_ENABLE_SEMA42 == STD_ON)
/** Channel Type for Sema42 */
typedef uint8                         Rm_Sema42_ChannelType;

/** Core Type for Sema42 */
typedef uint8                         Rm_Sema42_CoreType;

/** Status Type for Sema42 */
typedef Sema42_Ip_StatusType          Rm_Sema42_StatusType;
#endif

#if (RM_IPW_ENABLE_MPU_M7 == STD_ON)
/** Configuration structure containing the region configuration for Mpu M7 */
typedef Mpu_M7_Ip_RegionConfigType    Rm_Mpu_M7_RegionConfigType;

/** Enumeration listing access permisions for Mpu M7 */
typedef Mpu_M7_Ip_AccessRightsType    Rm_Mpu_M7_AccessRightsType;

/** Enumeration listing the possible access errors for Mpu M7 */
typedef Mpu_M7_Ip_ErrorDetailsType    Rm_Mpu_M7_ErrorDetailsType;
#endif

#if (RM_IPW_ENABLE_XBIC == STD_ON)
/** Structure for error informations of Xbic Ip. */
typedef Xbic_Ip_ErrorInfoType         Rm_XbicErrorInfoType;
#endif
/*==================================================================================================
                                 GLOBAL VARIABLE DECLARATIONS
==================================================================================================*/

/*==================================================================================================
                                     FUNCTION PROTOTYPES
==================================================================================================*/
#define RM_START_SEC_CODE
#include "Rm_MemMap.h"

#if (RM_IPW_ENABLE_XRDC == STD_ON)
void Rm_Ipw_Xrdc_Init(Rm_Ipw_ConfigType const * pIpwConfig);

void Rm_Ipw_Xrdc_InstanceInit(Rm_XrdcInstanceConfigType const * pXrdcInstanceConfig);

void Rm_Ipw_Xrdc_SetProcessID(Rm_XrdcMasterCoreType MasterCore, uint8 u8Pid, Rm_XrdcSecureAttributeType SecureAttr);

void Rm_Ipw_Xrdc_GetDomainIDErrorStatus(uint32 u32Instance, Rm_XrdcDomainIDErrStatusType * pXrdcDomainIDErrorStatus);

uint32 Rm_Ipw_Xrdc_GetDomainID(uint32 u32Instance);

#endif

#if (RM_IPW_ENABLE_SEMA42 == STD_ON)

void Rm_Ipw_Sema4_Init(Rm_Ipw_ConfigType const * pIpwConfig);

Std_ReturnType Rm_Ipw_Sema4_LockGate(const uint8 u8LogicChannel, uint8 u8DomainId);

Std_ReturnType Rm_Ipw_Sema4_UnlockGate(const uint8 u8LogicChannel);

void Rm_Ipw_Sema4_ResetGate(const uint8 u8LogicChannel);

void Rm_Ipw_Sema4_ResetAllGates(void);

uint8 Rm_Ipw_Sema4_GetGateLocker(const uint8 u8LogicChannel);

uint32 Rm_Ipw_SemaphoreGetDomainID(Rm_Sema42_ChannelType ChannelNumber);
#endif

#if (RM_IPW_ENABLE_MPU_M7 == STD_ON)
void Rm_Ipw_Mpu_M7_Init(Rm_Ipw_ConfigType const * pIpwConfig);

void Rm_Ipw_Mpu_M7_SetRegionConfig(uint8 u8RegionNum, const Rm_Mpu_M7_RegionConfigType * const pUserConfigPtr);

void Rm_Ipw_Mpu_M7_EnableRegion(uint8 u8RegionNum, boolean bEnable);

void Rm_Ipw_Mpu_M7_SetAccessRight(uint8 u8RegionNum, Rm_Mpu_M7_AccessRightsType eRights);

boolean Rm_Ipw_Mpu_M7_GetErrorDetails(Rm_Mpu_M7_ErrorDetailsType * pErrorDetails);

#if (RM_IPW_MPU_MULTICORE_SUPPORT == STD_ON)
void Rm_Ipw_Mpu_M7_MulticoreInit(void);
#endif
#endif

#if (RM_IPW_ENABLE_PFLASH == STD_ON)
void Rm_Ipw_Pflash_Init(Rm_Ipw_ConfigType const * pIpwConfig);
#endif

#if (RM_IPW_ENABLE_AXBS == STD_ON)
void Rm_Ipw_Axbs_Init(Rm_Ipw_ConfigType const * pIpwConfig);
#endif

#if (RM_IPW_ENABLE_VIRT_WRAPPER == STD_ON)
void Rm_Ipw_Virt_Wrapper_Init(Rm_Ipw_ConfigType const * pIpwConfig);
#endif

#if (RM_IPW_ENABLE_XBIC == STD_ON)
void Rm_Ipw_Xbic_Init(Rm_Ipw_ConfigType const * pIpwConfig);

void Rm_Ipw_Xbic_EnableMasterFeedbackCheck(uint32 xbicInstance, uint8 masterPort, boolean bFeedbackCheckEnable);

void Rm_Ipw_Xbic_EnableSlaveEDCCheck(uint32 xbicInstance, uint8 slavePort, boolean bEDCCheckEnable);

void Rm_Ipw_Xbic_FeedbackCheckAndEDCCheckDisable(void);

void Rm_Ipw_Xbic_GetErrorStatus(uint32 xbicInstance, Rm_XbicErrorInfoType * pErrorInfo);

void Rm_Ipw_Xbic_ErrorInjection(uint32 xbicInstance, uint8 slaveTarget, uint8 masterTargetID, uint8 synDecode);

void Rm_Ipw_Xbic_ErrorInjectionDisable(uint32 xbicInstance);
#endif

#define RM_STOP_SEC_CODE
#include "Rm_MemMap.h"


#ifdef __cplusplus
}
#endif

/** @} */

#endif /* CDD_RM_IPW_H */

