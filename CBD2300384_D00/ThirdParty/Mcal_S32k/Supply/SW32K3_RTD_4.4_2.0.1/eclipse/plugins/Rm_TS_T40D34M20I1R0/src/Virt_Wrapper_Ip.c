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
*   @file Virt_Wrapper_Ip.c
*
*   @addtogroup Virt_Wrapper_Ip Virt Wrapper IPV Driver
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
#include "Virt_Wrapper_Ip.h"
#include "Virt_Wrapper_Ip_Cfg.h"

#ifdef VIRT_WRAPPER_IP_DEV_ERROR_DETECT
#if (VIRT_WRAPPER_IP_DEV_ERROR_DETECT == STD_ON)
    #include "Devassert.h"
#endif
#endif

#ifdef MCAL_ENABLE_USER_MODE_SUPPORT
    #ifdef VIRT_WRAPPER_IP_ENABLE_USER_MODE_SUPPORT
        #ifndef USER_MODE_REG_PROT_ENABLED
        #define USER_MODE_REG_PROT_ENABLED (STD_ON)
        #include "RegLockMacros.h"
        #endif
    #endif
#endif

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_VIRT_WRAPPER_IP_VENDOR_ID_C                        43
#define RM_VIRT_WRAPPER_IP_AR_RELEASE_MAJOR_VERSION_C         4
#define RM_VIRT_WRAPPER_IP_AR_RELEASE_MINOR_VERSION_C         4
#define RM_VIRT_WRAPPER_IP_AR_RELEASE_REVISION_VERSION_C      0
#define RM_VIRT_WRAPPER_IP_SW_MAJOR_VERSION_C                 2
#define RM_VIRT_WRAPPER_IP_SW_MINOR_VERSION_C                 0
#define RM_VIRT_WRAPPER_IP_SW_PATCH_VERSION_C                 1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((RM_VIRT_WRAPPER_IP_AR_RELEASE_MAJOR_VERSION_C != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (RM_VIRT_WRAPPER_IP_AR_RELEASE_MINOR_VERSION_C != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Virt_Wrapper_Ip.c and Mcal.h are different"
    #endif
#endif

/* Checks against VIRT_WRAPPER_Ip.h */
#if (RM_VIRT_WRAPPER_IP_VENDOR_ID_C !=  RM_VIRT_WRAPPER_IP_VENDOR_ID)
    #error "Virt_Wrapper_Ip.c and Virt_Wrapper_Ip.h have different vendor ids"
#endif
#if ((RM_VIRT_WRAPPER_IP_AR_RELEASE_MAJOR_VERSION_C    != RM_VIRT_WRAPPER_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_AR_RELEASE_MINOR_VERSION_C    != RM_VIRT_WRAPPER_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_AR_RELEASE_REVISION_VERSION_C != RM_VIRT_WRAPPER_IP_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Virt_Wrapper_Ip.c and Virt_Wrapper_Ip.h are different"
#endif
#if ((RM_VIRT_WRAPPER_IP_SW_MAJOR_VERSION_C != RM_VIRT_WRAPPER_IP_SW_MAJOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_SW_MINOR_VERSION_C != RM_VIRT_WRAPPER_IP_SW_MINOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_SW_PATCH_VERSION_C != RM_VIRT_WRAPPER_IP_SW_PATCH_VERSION))
    #error "Software Version Numbers of Virt_Wrapper_Ip.c and Virt_Wrapper_Ip.h are different"
#endif

/* Checks against Virt_Wrapper_Ip_Cfg.h */
#if (RM_VIRT_WRAPPER_IP_VENDOR_ID_C != RM_VIRT_WRAPPER_IP_CFG_VENDOR_ID)
    #error "Virt_Wrapper_Ip.c and Virt_Wrapper_Ip_Cfg.h have different vendor ids"
#endif
#if ((RM_VIRT_WRAPPER_IP_AR_RELEASE_MAJOR_VERSION_C    != RM_VIRT_WRAPPER_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_AR_RELEASE_MINOR_VERSION_C    != RM_VIRT_WRAPPER_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_AR_RELEASE_REVISION_VERSION_C != RM_VIRT_WRAPPER_IP_CFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Virt_Wrapper_Ip.c and Virt_Wrapper_Ip_Cfg.h are different"
#endif
#if ((RM_VIRT_WRAPPER_IP_SW_MAJOR_VERSION_C != RM_VIRT_WRAPPER_IP_CFG_SW_MAJOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_SW_MINOR_VERSION_C != RM_VIRT_WRAPPER_IP_CFG_SW_MINOR_VERSION) || \
     (RM_VIRT_WRAPPER_IP_SW_PATCH_VERSION_C != RM_VIRT_WRAPPER_IP_CFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Virt_Wrapper_Ip.c and Virt_Wrapper_Ip_Cfg.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    #ifdef VIRT_WRAPPER_IP_DEV_ERROR_DETECT
    #if (VIRT_WRAPPER_IP_DEV_ERROR_DETECT == STD_ON)
        /* Check if the files Virt_Wrapper_Ip.c and Devassert.h are of the same version */
        #if ((RM_VIRT_WRAPPER_IP_AR_RELEASE_MAJOR_VERSION_C != DEVASSERT_AR_RELEASE_MAJOR_VERSION) || \
            (RM_VIRT_WRAPPER_IP_AR_RELEASE_MINOR_VERSION_C != DEVASSERT_AR_RELEASE_MINOR_VERSION)     \
            )
            #error "AUTOSAR Version Numbers of Virt_Wrapper_Ip.c and Devassert.h are different"
        #endif
    #endif
    #endif
#endif

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#ifdef RM_IP_ENABLE_VIRT_WRAPPER
#if (RM_IP_ENABLE_VIRT_WRAPPER == STD_ON)

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/
static VIRT_WRAPPER_Type * aVirtWrapperBasePtrs[VIRT_WRAPPER_BASE_WITH_GAP_CNT] = VIRT_WRAPPER_BASE_WITH_GAP;
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

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
void Virt_Wrapper_Ip_SetRegAccessProtection(uint32 u32Instance, Virt_Wrapper_Ip_AccessType eMirror, uint32 u32Pin, Virt_Wrapper_Ip_SlotType eSlotType);
/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

#define RM_START_SEC_CODE
#include "Rm_MemMap.h"

/**
 * @brief         Virt_Wrapper_Ip_SetMscrAccessProtection 
 *
 * @details       This function is Reentrant
 *
 * @param[in]     pVirt_WrapperConfig: Pointer point the configuration of pads (Suil2 registers) will be protected by Virtual Wrapper
 * @return        void
 *
 * @api
 *
 * @pre
 *
 */

void Virt_Wrapper_Ip_Init(const Virt_Wrapper_Ip_ConfigType * pVirt_WrapperConfig)
{
    uint32 u32Counter = 0U;
    const Virt_Wrapper_Ip_RegConfigType * pTemp;
#if (VIRT_WRAPPER_IP_DEV_ERROR_DETECT == STD_ON)
    /*check the input parameter*/
    DevAssert(pVirt_WrapperConfig != NULL_PTR);
#endif

    for(u32Counter = 0U; u32Counter < pVirt_WrapperConfig->u32ConfigCount; u32Counter++)
    {
        pTemp = &pVirt_WrapperConfig->aRegConfig[u32Counter];
    #ifdef MCAL_ENABLE_USER_MODE_SUPPORT
        #ifdef VIRT_WRAPPER_IP_ENABLE_USER_MODE_SUPPORT
            OsIf_Trusted_Call4params(Virt_Wrapper_Ip_SetRegAccessProtection,pTemp->u32Virt_Wrapper_Instance, pTemp->Virt_Wrapper_Access, pTemp->u32Virt_Wrapper_Pin, pTemp->Virt_Wrapper_Type);
        #else
            Virt_Wrapper_Ip_SetRegAccessProtection(pTemp->u32Virt_Wrapper_Instance, pTemp->Virt_Wrapper_Access, pTemp->u32Virt_Wrapper_Pin, pTemp->Virt_Wrapper_Type);
        #endif
    #else
        Virt_Wrapper_Ip_SetRegAccessProtection(pTemp->u32Virt_Wrapper_Instance, pTemp->Virt_Wrapper_Access, pTemp->u32Virt_Wrapper_Pin, pTemp->Virt_Wrapper_Type);
    #endif
    }
}

/**
 * @brief         Virt_Wrapper_Ip_SetRegAccessProtection 
 *
 * @details       
 *
 * @param[in]     
 * @return        
 *
 * @api
 *
 * @pre
 *
 */

void Virt_Wrapper_Ip_SetRegAccessProtection(uint32 u32Instance, Virt_Wrapper_Ip_AccessType eMirror, uint32 u32Pin, Virt_Wrapper_Ip_SlotType eSlotType)
{
#if (VIRT_WRAPPER_IP_DEV_ERROR_DETECT == STD_ON)
    /*check the input parameter*/
    DevAssert(u32Instance < (uint32)VIRT_WRAPPER_INSTANCE_COUNT);
#endif
    uint32 regOffset;
    uint32 regShift;
    
    regOffset = u32Pin / 16U;
    regShift = (u32Pin % 16U) * 2U;
    
    if(eSlotType == VIRT_WRAPPER_REG_TYPE_A)
    {
        aVirtWrapperBasePtrs[u32Instance]->REG_A[regOffset] &= (uint32)(~(VIRT_WRAPPER_IP_PAD_MASK << regShift));
        aVirtWrapperBasePtrs[u32Instance]->REG_A[regOffset] |= (uint32)((uint32)eMirror << regShift);
    }
    else if(eSlotType == VIRT_WRAPPER_REG_TYPE_B)
    {
        aVirtWrapperBasePtrs[u32Instance]->REG_B[regOffset] &= (uint32)(~(VIRT_WRAPPER_IP_PAD_MASK << regShift));
        aVirtWrapperBasePtrs[u32Instance]->REG_B[regOffset] |= ((uint32)eMirror << regShift);
    }
    else if(eSlotType == VIRT_WRAPPER_REG_TYPE_C)
    {
        aVirtWrapperBasePtrs[u32Instance]->REG_C[regOffset] &= (uint32)(~(VIRT_WRAPPER_IP_PAD_MASK << regShift));
        aVirtWrapperBasePtrs[u32Instance]->REG_C[regOffset] |= ((uint32)eMirror << regShift);
    }
    else if(eSlotType == VIRT_WRAPPER_REG_TYPE_D)
    {
        aVirtWrapperBasePtrs[u32Instance]->REG_D[regOffset] &= (uint32)(~(VIRT_WRAPPER_IP_PAD_MASK << regShift));
        aVirtWrapperBasePtrs[u32Instance]->REG_D[regOffset] |= ((uint32)eMirror << regShift);
    }
    else
    {
        /* Do nothing */
    }
}

#define RM_STOP_SEC_CODE
#include "Rm_MemMap.h"

#endif /* RM_IP_ENABLE_VIRT_WRAPPER == STD_ON */
#endif /* RM_IP_ENABLE_VIRT_WRAPPER */

#ifdef __cplusplus
}
#endif

/** @} */
