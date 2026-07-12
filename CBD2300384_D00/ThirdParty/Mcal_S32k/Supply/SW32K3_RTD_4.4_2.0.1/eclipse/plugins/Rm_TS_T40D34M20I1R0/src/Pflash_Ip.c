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

/**
*   @file Pflash_Ip.c
*
*   @addtogroup Pflash_Ip Pflash IPV Driver
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
#include "Mcal.h"
#include "Pflash_Ip.h"
#include "Pflash_Ip_Cfg.h"
#include "Pflash_Ip_Cfg_Defines.h"

#ifdef PFLASH_IP_DEV_ERROR_DETECT
#if (PFLASH_IP_DEV_ERROR_DETECT == STD_ON)
    #include "Devassert.h"
#endif
#endif

#ifdef MCAL_ENABLE_USER_MODE_SUPPORT
    #ifdef PFLASH_IP_ENABLE_USER_MODE_SUPPORT
        #ifndef USER_MODE_REG_PROT_ENABLED
        #define USER_MODE_REG_PROT_ENABLED (STD_ON)
        #include "RegLockMacros.h"
        #endif
    #endif
#endif

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_PFLASH_IP_VENDOR_ID_C                        43
#define RM_PFLASH_IP_AR_RELEASE_MAJOR_VERSION_C         4
#define RM_PFLASH_IP_AR_RELEASE_MINOR_VERSION_C         4
#define RM_PFLASH_IP_AR_RELEASE_REVISION_VERSION_C      0
#define RM_PFLASH_IP_SW_MAJOR_VERSION_C                 2
#define RM_PFLASH_IP_SW_MINOR_VERSION_C                 0
#define RM_PFLASH_IP_SW_PATCH_VERSION_C                 1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against PFLASH_Ip.h */
#if (RM_PFLASH_IP_VENDOR_ID_C !=  RM_PFLASH_IP_VENDOR_ID)
    #error "Pflash_Ip.c and Pflash_Ip.h have different vendor ids"
#endif
#if ((RM_PFLASH_IP_AR_RELEASE_MAJOR_VERSION_C    !=  RM_PFLASH_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_PFLASH_IP_AR_RELEASE_MINOR_VERSION_C    !=  RM_PFLASH_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_PFLASH_IP_AR_RELEASE_REVISION_VERSION_C !=  RM_PFLASH_IP_AR_RELEASE_REVISION_VERSION) \
    )
     #error "AUTOSAR Version Numbers of Pflash_Ip.c and Pflash_Ip.h are different"
#endif
#if ((RM_PFLASH_IP_SW_MAJOR_VERSION_C !=  RM_PFLASH_IP_SW_MAJOR_VERSION) || \
     (RM_PFLASH_IP_SW_MINOR_VERSION_C !=  RM_PFLASH_IP_SW_MINOR_VERSION) || \
     (RM_PFLASH_IP_SW_PATCH_VERSION_C !=  RM_PFLASH_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Pflash_Ip.c and Pflash_Ip.h are different"
#endif

/* Checks against Pflash_Ip_Cfg.h */
#if (RM_PFLASH_IP_VENDOR_ID_C !=  RM_PFLASH_IP_CFG_VENDOR_ID)
    #error "Pflash_Ip.c and Pflash_Ip_Cfg.h have different vendor ids"
#endif
#if ((RM_PFLASH_IP_AR_RELEASE_MAJOR_VERSION_C    !=  RM_PFLASH_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (RM_PFLASH_IP_AR_RELEASE_MINOR_VERSION_C    !=  RM_PFLASH_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (RM_PFLASH_IP_AR_RELEASE_REVISION_VERSION_C !=  RM_PFLASH_IP_CFG_AR_RELEASE_REVISION_VERSION) \
    )
     #error "AUTOSAR Version Numbers of Pflash_Ip.c and Pflash_Ip_Cfg.h are different"
#endif
#if ((RM_PFLASH_IP_SW_MAJOR_VERSION_C !=  RM_PFLASH_IP_CFG_SW_MAJOR_VERSION) || \
     (RM_PFLASH_IP_SW_MINOR_VERSION_C !=  RM_PFLASH_IP_CFG_SW_MINOR_VERSION) || \
     (RM_PFLASH_IP_SW_PATCH_VERSION_C !=  RM_PFLASH_IP_CFG_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Pflash_Ip.c and Pflash_Ip_Cfg.h are different"
#endif

/* Checks against Pflash_Ip_Cfg_Defines.h */
#if (RM_PFLASH_IP_VENDOR_ID_C != RM_PFLASH_IP_CFG_DEFINES_VENDOR_ID)
    #error "Pflash_Ip.c and Pflash_Ip_Cfg_Defines.h have different vendor ids"
#endif
#if ((RM_PFLASH_IP_AR_RELEASE_MAJOR_VERSION_C    != RM_PFLASH_IP_CFG_DEFINES_AR_RELEASE_MAJOR_VERSION) || \
     (RM_PFLASH_IP_AR_RELEASE_MINOR_VERSION_C    != RM_PFLASH_IP_CFG_DEFINES_AR_RELEASE_MINOR_VERSION) || \
     (RM_PFLASH_IP_AR_RELEASE_REVISION_VERSION_C != RM_PFLASH_IP_CFG_DEFINES_AR_RELEASE_REVISION_VERSION) \
    )
     #error "AUTOSAR Version Numbers of Pflash_Ip.c and Pflash_Ip_Cfg_Defines.h are different"
#endif
#if ((RM_PFLASH_IP_SW_MAJOR_VERSION_C != RM_PFLASH_IP_CFG_DEFINES_SW_MAJOR_VERSION) || \
     (RM_PFLASH_IP_SW_MINOR_VERSION_C != RM_PFLASH_IP_CFG_DEFINES_SW_MINOR_VERSION) || \
     (RM_PFLASH_IP_SW_PATCH_VERSION_C != RM_PFLASH_IP_CFG_DEFINES_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Pflash_Ip.c and Pflash_Ip_Cfg_Defines.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((RM_PFLASH_IP_AR_RELEASE_MAJOR_VERSION_C != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (RM_PFLASH_IP_AR_RELEASE_MINOR_VERSION_C != MCAL_AR_RELEASE_MINOR_VERSION) \
        )
        #error "AUTOSAR Version Numbers of Pflash_Ip.c and Mcal.h are different"
    #endif
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #ifdef PFLASH_IP_DEV_ERROR_DETECT
    #if (PFLASH_IP_DEV_ERROR_DETECT == STD_ON)
        /* Check if the files Pflash_Ip.c and Devassert.h are of the same version */
        #if ((RM_PFLASH_IP_AR_RELEASE_MAJOR_VERSION_C != DEVASSERT_AR_RELEASE_MAJOR_VERSION) || \
            (RM_PFLASH_IP_AR_RELEASE_MINOR_VERSION_C != DEVASSERT_AR_RELEASE_MINOR_VERSION)     \
            )
            #error "AUTOSAR Version Numbers of Pflash_Ip.c and Devassert.h are different"
        #endif
    #endif
    #endif
#endif
/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#ifdef RM_IP_ENABLE_PFLASH
#if (RM_IP_ENABLE_PFLASH == STD_ON)

#if (PFLASH_IP_DEV_ERROR_DETECT == STD_ON)
    #define PFLASH_IP_CONTROLLERS_COUNT         ((uint32)2U)
#endif

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL CONSTANTS
==================================================================================================*/


/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#define RM_START_SEC_CODE
#include "Rm_MemMap.h"

#ifdef PFLASH_IP_ENABLE_USER_MODE_SUPPORT
/**
 * @brief         Pflash_Ip_SetUserAccessAllowed
 *
 * @details       This function is Reentrant
 *
 * @param[in]     address: Address to allow
 * @return        void
 *
 * @api
 *
 * @pre
 *
 */
void Pflash_Ip_SetUserAccessAllowed(uint32 address);
#endif /* PFLASH_ENABLE_USER_MODE_SUPPORT */

/**
 * @brief         Set a master access protection by Pflash
 *
 * @details       This function is Reentrant
 *
 * @param[in]     u8instance: Pflash instance
 * @param[in]     eMaster: Master access protection field
 * @param[in]     eAccess: Access type
 * @return        void
 *
 * @api
 *
 * @pre
 *
 */
void Pflash_Ip_SetAccessProtection_Privileged(uint8 u8instance, Pflash_Ip_MasterType eMaster, Pflash_Ip_MasterAccessType eAccess);
/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
#ifdef PFLASH_IP_ENABLE_USER_MODE_SUPPORT
/**
 * @brief         Pflash_Ip_SetUserAccessAllowed
 */
void Pflash_Ip_SetUserAccessAllowed(uint32 address)
{
    SET_USER_ACCESS_ALLOWED(address, PFLASH_IP_REG_PROT_SIZE);
}
#endif /* PFLASH_ENABLE_USER_MODE_SUPPORT */

/**
 * @brief         Set a master access protection by Pflash
 */
void Pflash_Ip_SetAccessProtection_Privileged(uint8 u8instance, Pflash_Ip_MasterType eMaster, Pflash_Ip_MasterAccessType eAccess)
{
#if (PFLASH_IP_DEV_ERROR_DETECT == STD_ON)
    /*check the input parameter*/
    DevAssert(u8instance < PFLASH_IP_CONTROLLERS_COUNT);
    DevAssert(eMaster != PFLASH_M3AP);   /* PFLASH_M3AP is reserved */
#endif

    if (u8instance == (uint8)(0U))
    {
        PFLASH_IP_REG_RMW32(PFLASH_IP_PFARP_ADDR32, (uint32)PFLASH_IP_PFAPR_MAP_MASK << ((uint32)eMaster << (uint32)1U), (uint32)eAccess << ((uint32)eMaster << (uint32)1U));
    }
    else
    {
    }
}
/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
 * @brief         Initializes the Pflash Ip
 */
void Pflash_Ip_Init(const Pflash_Ip_ConfigType * pPflashConfig)
{
    uint32 u32counter = 0U;

#if (PFLASH_IP_DEV_ERROR_DETECT == STD_ON)
    /*check the input parameter*/
    DevAssert(pPflashConfig != NULL_PTR);
#endif

#ifdef PFLASH_IP_ENABLE_USER_MODE_SUPPORT
    OsIf_Trusted_Call1param(Pflash_Ip_SetUserAccessAllowed, PFLASH_IP_PFARP_ADDR32);
#endif

    for (u32counter = 0U; u32counter < PFLASH_NUM_CONFIG_U32; u32counter++)
    {
#ifdef PFLASH_IP_ENABLE_USER_MODE_SUPPORT
        OsIf_Trusted_Call3params(Pflash_Ip_SetAccessProtection_Privileged, (&pPflashConfig[u32counter])->u32Pflash_Instance, (&pPflashConfig[u32counter])->Pflash_Map, (&pPflashConfig[u32counter])->Pflash_Access);
#else
        Pflash_Ip_SetAccessProtection_Privileged((&pPflashConfig[u32counter])->u32Pflash_Instance, (&pPflashConfig[u32counter])->Pflash_Map, (&pPflashConfig[u32counter])->Pflash_Access);
#endif
    }
}

/**
 * @brief         Set access protection by Pflash master
 */
void Pflash_Ip_SetAccessProtection(uint8 u8instance, Pflash_Ip_MasterType eMaster, Pflash_Ip_MasterAccessType eAccess)
{
#ifdef PFLASH_IP_ENABLE_USER_MODE_SUPPORT
    OsIf_Trusted_Call3params(Pflash_Ip_SetAccessProtection_Privileged, u8instance, eMaster, eAccess);
#else
    Pflash_Ip_SetAccessProtection_Privileged(u8instance, eMaster, eAccess);
#endif
}

#define RM_STOP_SEC_CODE
#include "Rm_MemMap.h"

#endif /* RM_IP_ENABLE_PFLASH == STD_ON */
#endif /* RM_IP_ENABLE_PFLASH */

#ifdef __cplusplus
}
#endif

/** @} */
