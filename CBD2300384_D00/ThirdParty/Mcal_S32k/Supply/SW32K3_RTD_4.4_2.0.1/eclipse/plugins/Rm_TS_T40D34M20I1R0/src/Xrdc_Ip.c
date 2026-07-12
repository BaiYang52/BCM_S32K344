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
*   @file Xrdc_Ip.c
*
*   @addtogroup XRDC_IP XRDC IPV Driver
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
#include "Xrdc_Ip.h"
#include "Xrdc_Ip_Cfg.h"


#ifdef XRDC_IP_DEV_ERROR_DETECT
#if(XRDC_IP_DEV_ERROR_DETECT == STD_ON)
    #include "Devassert.h"
#endif
#endif
/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_XRDC_IP_VENDOR_ID_C                        43
#define RM_XRDC_IP_AR_RELEASE_MAJOR_VERSION_C         4
#define RM_XRDC_IP_AR_RELEASE_MINOR_VERSION_C         4
#define RM_XRDC_IP_AR_RELEASE_REVISION_VERSION_C      0
#define RM_XRDC_IP_SW_MAJOR_VERSION_C                 2
#define RM_XRDC_IP_SW_MINOR_VERSION_C                 0
#define RM_XRDC_IP_SW_PATCH_VERSION_C                 1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Xrdc_Ip.h */
#if (RM_XRDC_IP_VENDOR_ID_C !=  RM_XRDC_IP_VENDOR_ID)
    #error "Xrdc_Ip.c and Xrdc_Ip.h have different vendor ids"
#endif
#if ((RM_XRDC_IP_AR_RELEASE_MAJOR_VERSION_C    !=  RM_XRDC_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_XRDC_IP_AR_RELEASE_MINOR_VERSION_C    !=  RM_XRDC_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_XRDC_IP_AR_RELEASE_REVISION_VERSION_C !=  RM_XRDC_IP_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Xrdc_Ip.c and Xrdc_Ip.h are different"
#endif
#if ((RM_XRDC_IP_SW_MAJOR_VERSION_C !=  RM_XRDC_IP_SW_MAJOR_VERSION) || \
     (RM_XRDC_IP_SW_MINOR_VERSION_C !=  RM_XRDC_IP_SW_MINOR_VERSION) || \
     (RM_XRDC_IP_SW_PATCH_VERSION_C !=  RM_XRDC_IP_SW_PATCH_VERSION))
    #error "Software Version Numbers of Xrdc_Ip.c and Xrdc_Ip.h are different"
#endif

/* Checks against Xrdc_Ip_Cfg.h */
#if (RM_XRDC_IP_VENDOR_ID_C != RM_XRDC_IP_CFG_VENDOR_ID)
    #error "Xrdc_Ip.c and Xrdc_Ip_Cfg.h have different vendor ids"
#endif
#if ((RM_XRDC_IP_AR_RELEASE_MAJOR_VERSION_C    != RM_XRDC_IP_CFG_AR_RELEASE_MAJOR_VERSION) || \
     (RM_XRDC_IP_AR_RELEASE_MINOR_VERSION_C    != RM_XRDC_IP_CFG_AR_RELEASE_MINOR_VERSION) || \
     (RM_XRDC_IP_AR_RELEASE_REVISION_VERSION_C != RM_XRDC_IP_CFG_AR_RELEASE_REVISION_VERSION))
     #error "AUTOSAR Version Numbers of Xrdc_Ip.c and Xrdc_Ip_Cfg.h are different"
#endif
#if ((RM_XRDC_IP_SW_MAJOR_VERSION_C != RM_XRDC_IP_CFG_SW_MAJOR_VERSION) || \
     (RM_XRDC_IP_SW_MINOR_VERSION_C != RM_XRDC_IP_CFG_SW_MINOR_VERSION) || \
     (RM_XRDC_IP_SW_PATCH_VERSION_C != RM_XRDC_IP_CFG_SW_PATCH_VERSION))
    #error "Software Version Numbers of Xrdc_Ip.c and Xrdc_Ip_Cfg.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against Mcal.h */
    #if ((RM_XRDC_IP_AR_RELEASE_MAJOR_VERSION_C != MCAL_AR_RELEASE_MAJOR_VERSION) || \
         (RM_XRDC_IP_AR_RELEASE_MINOR_VERSION_C != MCAL_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Xrdc_Ip.c and Mcal.h are different"
    #endif

    
    #ifdef XRDC_IP_DEV_ERROR_DETECT
    #if(XRDC_IP_DEV_ERROR_DETECT == STD_ON)

        /* Check if the files Xrdc_Ip.c and Devassert.h are of the same version */
        #if ((RM_XRDC_IP_AR_RELEASE_MAJOR_VERSION_C != DEVASSERT_AR_RELEASE_MAJOR_VERSION) || \
            (RM_XRDC_IP_AR_RELEASE_MINOR_VERSION_C != DEVASSERT_AR_RELEASE_MINOR_VERSION)     \
            )
            #error "AUTOSAR Version Numbers of Xrdc_Ip.c and Devassert.h are different"
        #endif
    #endif
    #endif
#endif
/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/


/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/
#ifdef RM_IP_ENABLE_XRDC
#if (RM_IP_ENABLE_XRDC == STD_ON)


#define RM_START_SEC_CONST_32
#include "Rm_MemMap.h"

static const uint32 Xrdc_Ip_InstanceAddress[XRDC_COUNT] = XRDC_BASE_ADDRS;

#define RM_STOP_SEC_CONST_32
#include "Rm_MemMap.h"

#if(XRDC_IP_REGISTER_LOCK == STD_ON)

#define RM_START_SEC_CONST_8
#include "Rm_MemMap.h"
XRDC_IP_CFG_INFO_8

static const uint8 Xrdc_Num_PerList[XRDC_COUNT] = XRDC_NUM_PER_INSTANCE;
#define RM_STOP_SEC_CONST_8
#include "Rm_MemMap.h"

#define RM_START_SEC_CONST_16
#include "Rm_MemMap.h"
XRDC_IP_CFG_INFO_16

#define RM_STOP_SEC_CONST_16
#include "Rm_MemMap.h"

#define RM_START_SEC_CONST_32
#include "Rm_MemMap.h"

static const uint8 * const Xrdc_Master[XRDC_COUNT] = XRDC_MASTER_INSTANCE;
static const uint8 * const Xrdc_MrcInstances[XRDC_COUNT] = XRDC_MRC_INSTANCE;
static const uint16 * const Xrdc_Perlist[XRDC_COUNT] = XRDC_PER_INSTANCE;

#define RM_STOP_SEC_CONST_32
#include "Rm_MemMap.h"

#endif/*XRDC_IP_REGISTER_LOCK == STD_ON*/


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

static void Xrdc_Memory_Config_Descriptor(uint32 u32Instance, Xrdc_Ip_MemConfigType const * pXrdcMemDescriptor);

static void Xrdc_Peripheral_Access_Config(uint32 u32Instance, Xrdc_Ip_PeripheralConfigType const * pXrdcPerDescriptor);

static void Xrdc_Domain_Init(uint32 u32Instance, Xrdc_Ip_DomainConfigType const * pDomainConfig);

static Xrdc_Ip_ErrorAttributeType Xrdc_Ip_GetErrAttribute(uint32 u32ErrorAttribute);

static uint32 Xrdc_Get_Priviledge_Attribute(Xrdc_Ip_PriviledgedAttributeType eXrdcPriviledge);

static uint32 Xrdc_Get_Secure_Attribute(Xrdc_Ip_SecureAttributeType eXrdcSecure);

#if(XRDC_IP_REGISTER_LOCK == STD_ON)
static void Xrdc_LockRegisters(void);
#endif/*XRDC_IP_REGISTER_LOCK == STD_ON*/

void Xrdc_Ip_Init_Privileged(Xrdc_Ip_ConfigType const * pXrdcConfig);

uint32 Xrdc_Ip_GetDomainID_Privileged(uint32 u32Instance);

void Xrdc_Ip_GetDomainIDErrorStatus_Privileged(uint32 u32Instance, Xrdc_Ip_DomainIDErrStatusType * pXrdcDomainIDErrorStatus);

void Xrdc_Ip_InstanceInit_Privileged(Xrdc_Ip_InstanceConfigType const * pXrdcConfig);

void Xrdc_Ip_SetProcessID_Privileged(Xrdc_Ip_MasterCoreType MasterCore, uint8 u8Pid, Xrdc_Ip_SecureAttributeType eSecureAttr);

#if (XRDC_IP_DEV_ERROR_DETECT == STD_ON)
static inline boolean Xrdc_Ip_ValidateMastercore(Xrdc_Ip_MasterCoreType MasterCore);
static inline boolean Xrdc_Ip_ValidateSecureAttribute(Xrdc_Ip_SecureAttributeType eSecureAttr);
#endif
/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
#define RM_START_SEC_CODE
#include "Rm_MemMap.h"

/**
* @brief        Xrdc_Memory_Config_Descriptor
* @details
*
* @param[in]
*
* @param[out]
*
* @return
*
* @api
*
*/
static void Xrdc_Memory_Config_Descriptor(uint32 u32Instance, Xrdc_Ip_MemConfigType const * pXrdcMemDescriptor )
{
#if (XRDC_IP_DEV_ERROR_DETECT == STD_ON)
    /* Check the input parameters */
    DevAssert(pXrdcMemDescriptor != NULL_PTR);
    DevAssert(u32Instance < XRDC_COUNT);
#endif
    /* Clear Valid bit of descriptor before starting writing new descriptor */
    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

    XRDC_REG_RMW32(XRDC_MRGD_W_ADDR32(u32Instance, XRDC_WORD3, \
        (((uint32)pXrdcMemDescriptor->u32XrdcMrcInstance << 4UL) + (uint32)pXrdcMemDescriptor->u32XrdcMrcRegionDescriptor)), XRDC_MDA_VALID_MASK,XRDC_MDA_DESCRIPTOR_DISABLE);

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();
    /* Define start address bit35:5 according to RM */
    XRDC_REG_WRITE32(XRDC_MRGD_W_ADDR32(u32Instance, XRDC_WORD0, \
        ((pXrdcMemDescriptor->u32XrdcMrcInstance << 4UL) + pXrdcMemDescriptor->u32XrdcMrcRegionDescriptor)), (uint32)(pXrdcMemDescriptor->u32XrdcStartAddress)|(uint32)0x1UL);

    /* Define end address bit35:5 according to RM*/
    XRDC_REG_WRITE32(XRDC_MRGD_W_ADDR32(u32Instance, XRDC_WORD1, \
        ((pXrdcMemDescriptor->u32XrdcMrcInstance << 4UL) + pXrdcMemDescriptor->u32XrdcMrcRegionDescriptor)), (uint32)(pXrdcMemDescriptor->u32XrdcEndAddress)|(uint32)0x1UL);

    /* Set semaphore details */
    XRDC_REG_WRITE32(XRDC_MRGD_W_ADDR32(u32Instance, XRDC_WORD2, \
        ((pXrdcMemDescriptor->u32XrdcMrcInstance << 4UL) + pXrdcMemDescriptor->u32XrdcMrcRegionDescriptor)), pXrdcMemDescriptor->u32XrdcSema4Enable | ((pXrdcMemDescriptor->u32XrdcSema4Number<<XRDC_SNUM_SHIFT_U32) & XRDC_SNUM_MASK) | pXrdcMemDescriptor->u32XrdcMemPolicy);

    /* Enable Descriptor*/
    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();
    XRDC_REG_RMW32(XRDC_MRGD_W_ADDR32(u32Instance, XRDC_WORD3, \
        ((pXrdcMemDescriptor->u32XrdcMrcInstance << 4UL) + pXrdcMemDescriptor->u32XrdcMrcRegionDescriptor)), XRDC_MDA_VALID_MASK | XRDC_MDA_DACP_MASK, XRDC_MDA_DESCRIPTOR_ENABLE | pXrdcMemDescriptor->u32XrdcMemPolicy1);

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

}


/**
* @brief        Xrdc_Peripheral_Access_Config
* @details
*
* @param[in]
*
* @param[out]
*
* @return
*
* @api
*
*/
static void Xrdc_Peripheral_Access_Config(uint32 u32Instance, Xrdc_Ip_PeripheralConfigType const * pXrdcPerDescriptor)
{
#if (XRDC_IP_DEV_ERROR_DETECT == STD_ON)
    /* Check the input parameters */
    DevAssert(pXrdcPerDescriptor != NULL_PTR);
    DevAssert(u32Instance < XRDC_COUNT);
#endif
    /* Clear Valid bit of descriptor before starting writing new descriptor */
    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

    XRDC_REG_RMW32(XRDC_PDAC_W1_ADDR32(u32Instance, pXrdcPerDescriptor->u32XrdcPdacInstance), XRDC_PDAC_W1_VALID_MASK, XRDC_PDAC_W1_DESCRIPTOR_DISABLE);

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

    XRDC_REG_WRITE32(XRDC_PDAC_W0_ADDR32(u32Instance, pXrdcPerDescriptor->u32XrdcPdacInstance), pXrdcPerDescriptor->u32XrdcSema4Enable | ((pXrdcPerDescriptor->u32XrdcSema4Number<<XRDC_SNUM_SHIFT_U32) & XRDC_SNUM_MASK) | pXrdcPerDescriptor->u32XrdcPerPolicy);

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

    XRDC_REG_RMW32(XRDC_PDAC_W1_ADDR32(u32Instance, pXrdcPerDescriptor->u32XrdcPdacInstance), XRDC_PDAC_W1_VALID_MASK | XRDC_PDAC_W1_DACP_MASK, XRDC_PDAC_W1_DESCRIPTOR_ENABLE | pXrdcPerDescriptor->u32XrdcPerPolicy1);

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();
}


/**
* @brief        Xrdc_Domain_Init
* @details
*
* @param[in]
*
* @param[out]
*
* @return
*
* @api
*
*/
static void Xrdc_Domain_Init(uint32 u32Instance, Xrdc_Ip_DomainConfigType const * pDomainConfig)
{
    uint32 u32SecureAtt;
    uint32 u32UserAtt;

#if (XRDC_IP_DEV_ERROR_DETECT == STD_ON)
    /* Check the input parameters */
    DevAssert(pDomainConfig != NULL_PTR);
    DevAssert(u32Instance < XRDC_COUNT);
#endif
    if (pDomainConfig->XrdcCoreMdacInstance == XRDC_CORE_MASTER)
    {
        MCAL_DATA_SYNC_BARRIER();
        MCAL_INSTRUCTION_SYNC_BARRIER();

        XRDC_REG_WRITE32(XRDC_MDA_W_DFMT0_ADDR32(u32Instance, (uint32)pDomainConfig->u32XrdcWordDescriptor, (uint32)pDomainConfig->XrdcMdacInstance),(uint32)((uint32)(pDomainConfig->u32XrdcDomainID) & XRDC_MDA_DOMAIN_ID_MASK)| \
        (uint32)(XRDC_MDA_DID_FIELD_USED)| \
        ((uint32)pDomainConfig->u32XrdcProcessIDEnable)| \
        ((uint32)pDomainConfig->u32XrdcProcessID << (uint32)XRDC_MDA_PID_SHIFT_U32)| \
        ((uint32)pDomainConfig->u32XrdcProcessIDMask << (uint32)XRDC_MDA_PIDM_SHIFT_U32)| \
        (uint32)XRDC_MDA_DESCRIPTOR_ENABLE \
        );

        MCAL_DATA_SYNC_BARRIER();
        MCAL_INSTRUCTION_SYNC_BARRIER();

    }
    else if (pDomainConfig->XrdcCoreMdacInstance == XRDC_NONCORE_MASTER)
    {
        u32SecureAtt = Xrdc_Get_Secure_Attribute(pDomainConfig->eXrdcSecureOutput);
        u32UserAtt = Xrdc_Get_Priviledge_Attribute(pDomainConfig->eXrdcPriviledgeOutput);

        MCAL_DATA_SYNC_BARRIER();
        MCAL_INSTRUCTION_SYNC_BARRIER();
        XRDC_REG_WRITE32(XRDC_MDA_W_DFMT1_ADDR32(u32Instance, (uint32)pDomainConfig->XrdcMdacInstance), (uint32)((uint32)(pDomainConfig->u32XrdcDomainID) & XRDC_MDA_DOMAIN_ID_MASK) | \
                                                                                                                           (uint32)((uint32)(pDomainConfig->u32XrdcDIDBypassDisable) & XRDC_MDA_DID_BYPASS_MASK) | \
                                                                                                                           (uint32)(u32UserAtt & XRDC_MDA_PA_ATTR_MASK) | \
                                                                                                                           (uint32)(u32SecureAtt & XRDC_MDA_SA_ATTR_MASK) | \
                                                                                                                           (uint32)(XRDC_MDA_MASTER_NONCORE_FORMAT | XRDC_MDA_DESCRIPTOR_ENABLE) \
                        );
        MCAL_DATA_SYNC_BARRIER();
        MCAL_INSTRUCTION_SYNC_BARRIER();

    }
    else
    {
        /* Do nothing */
    }
}

/**
* @brief        Xrdc_Ip_GetErrAttribute
* @details
*
* @param[in]
*
* @param[out]
*
* @return
*
* @api
*
*/
static Xrdc_Ip_ErrorAttributeType Xrdc_Ip_GetErrAttribute(uint32 u32ErrorAttribute)
{
    Xrdc_Ip_ErrorAttributeType errorStatus =  XRDC_IP_NO_ERRATTRIBUTE;

    switch (u32ErrorAttribute)
    {
        case (uint32)0U :
            errorStatus = XRDC_IP_SECUUSER_INST;
            break;
        case (uint32)1U :
            errorStatus = XRDC_IP_SECUUSER_DATA;
            break;
        case (uint32)2U :
            errorStatus = XRDC_IP_SECUPRIV_INST_ACCESS;
            break;
        case (uint32)3U :
            errorStatus = XRDC_IP_SECUPRIV_DATA_ACCESS;
            break;
        case (uint32)4U :
            errorStatus = XRDC_IP_NONSECUUSER_INST;
            break;
        case (uint32)5U :
            errorStatus = XRDC_IP_NONSECUUSER_DATA;
            break;
        case (uint32)6U :
            errorStatus = XRDC_IP_NONSECUREPRIV_INST;
            break;
        case (uint32)7U :
            errorStatus = XRDC_IP_NONSECUREPRIV_DATA;
            break;
        default:
        /* fix misra rule 16.4*/
            break;
    }

  return errorStatus;
}

/**
* @brief        Xrdc_Get_Priviledge_Attribute
* @details
*
* @param[in]
*
* @param[out]
*
* @return
*
* @api
*
*/
static uint32 Xrdc_Get_Priviledge_Attribute(Xrdc_Ip_PriviledgedAttributeType eXrdcPriviledge)
{
    uint32 Ret;

    if (eXrdcPriviledge == XRDC_USER_MODE)
    {
        Ret = XRDC_MDA_PA_USERMODE_ATTR_USED;
    }
    else if (eXrdcPriviledge == XRDC_PRIVILEDGE_MODE)
    {
        Ret = XRDC_MDA_PA_PRIVILEGE_ATTR_USED;
    }
    else
    {
        Ret = XRDC_MDA_PA_BUS_MASTER_ATTR_DIRECTLY_USED;
    }

    return Ret;
}
/**
* @brief        Xrdc_Get_Secure_Attribute
* @details
*
* @param[in]
*
* @param[out]
*
* @return
*
* @api
*
*/
static uint32 Xrdc_Get_Secure_Attribute(Xrdc_Ip_SecureAttributeType eXrdcSecure)
{
    uint32 Ret;
     if (eXrdcSecure == XRDC_NON_SECURE)
    {
        Ret = XRDC_MDA_SA_NON_SECURE_ATTR_USED;
    }
    else if (eXrdcSecure == XRDC_SECURE)
    {
        Ret = XRDC_MDA_SA_SECURE_ATTR_USED;
    }
    else
    {
        Ret = XRDC_MDA_SA_BUS_MASTER_ATTR_DIRECTLY_USED;
    }

    return Ret;
}

#if(XRDC_IP_REGISTER_LOCK == STD_ON)
/**
 * @brief         Lock all configuration register of XRDC until the next reset
 *
 * @details
 *
 * @param[in]     u32Instance:     XRDC Instance number
 * @return        void
 *
 * @api
 *
 * @pre
 *
 */
static void Xrdc_LockRegisters(void)
{
    uint32 u32Instance =0U;
    uint32 u32Counter = 0U;
    uint32 u32wordCounter = 0U;
    uint32 u32numMaster = 0U;
    uint32 u32numMrc =0U;
    uint32 u32numDescriptor = 0U;

    for(u32Instance = 0U; u32Instance < XRDC_COUNT; u32Instance++)
    {
        MCAL_DATA_SYNC_BARRIER();
        MCAL_INSTRUCTION_SYNC_BARRIER();
        /*Lock XRDC_CR Register*/
        XRDC_REG_RMW32(XRDC_CR_ADDR32(u32Instance), XRDC_CR_LK1_MASK, XRDC_CR_LOCK);
        
        MCAL_DATA_SYNC_BARRIER();
        MCAL_INSTRUCTION_SYNC_BARRIER();
        /*Loop all masters*/
        {
            u32numMaster = (XRDC_REG_READ32(XRDC_HWCFG0_ADDR32(u32Instance)) & XRDC_HWCFG0_NMSTR_MASK) >> XRDC_HWCFG0_NMSTR_SHIFT;
            for(u32Counter = 0U; u32Counter <= u32numMaster; u32Counter++)
            {
                u32numDescriptor = (uint32)((uint32)(XRDC_REG_READ8(XRDC_MDACFG_ADDR_ADDR32(u32Instance,Xrdc_Master[u32Instance][u32Counter])) & (uint32)XRDC_MDACFG_NMDAR_MASK)>> (uint32)XRDC_MDACFG_NMDAR_SHIFT);
                for(u32wordCounter = 0; u32wordCounter < u32numDescriptor; u32wordCounter++)
                {      
                    MCAL_DATA_SYNC_BARRIER();
                    MCAL_INSTRUCTION_SYNC_BARRIER();
                    /*Lock XRDC_MDA_DFMT Registers*/
                    XRDC_REG_RMW32(XRDC_MDA_W_DFMT0_ADDR32(u32Instance, u32wordCounter, Xrdc_Master[u32Instance][u32Counter]), XRDC_MDA_LK1_MASK, XRDC_MDA_LK1_LOCK);
                    
                    MCAL_DATA_SYNC_BARRIER();
                    MCAL_INSTRUCTION_SYNC_BARRIER();
                }
                /*Lock PID register if master is core*/
                if((uint8)((XRDC_REG_READ8(XRDC_MDACFG_ADDR_ADDR32(u32Instance,Xrdc_Master[u32Instance][u32Counter])) & XRDC_MDACFG_NCM_MASK) >> XRDC_MDACFG_NCM_SHIFT) == (uint8)XRDC_CORE_MASTER)
                {
                    MCAL_DATA_SYNC_BARRIER();
                    MCAL_INSTRUCTION_SYNC_BARRIER();
                    
                    XRDC_REG_RMW32(XRDC_PID_ADDR32(((u32Instance << 8)|(uint32)Xrdc_Master[u32Instance][u32Counter])), XRDC_PID_LK2_MASK, (uint32)XRDC_IP_PID_REG_LOCK << XRDC_PID_LK2_SHIFT);
                    
                    MCAL_DATA_SYNC_BARRIER();
                    MCAL_INSTRUCTION_SYNC_BARRIER();
                }
            }
        }
        /*Loop all PDACs*/
        for(u32Counter =0U; u32Counter < Xrdc_Num_PerList[u32Instance]; u32Counter++)
        {
            MCAL_DATA_SYNC_BARRIER();
            MCAL_INSTRUCTION_SYNC_BARRIER();
            /*Lock XRDC_PDAC Register*/
            XRDC_REG_RMW32(XRDC_PDAC_W1_ADDR32(u32Instance, Xrdc_Perlist[u32Instance][u32Counter]), XRDC_PDAC_W1_LK2_MASK, XRDC_PDAC_W1_LOCK);
        
            MCAL_DATA_SYNC_BARRIER();
            MCAL_INSTRUCTION_SYNC_BARRIER();
        }
        /*Loop all MRCs*/
        {
            u32numMrc = (XRDC_REG_READ32(XRDC_HWCFG0_ADDR32(u32Instance)) & XRDC_HWCFG0_NMRC_MASK) >> XRDC_HWCFG0_NMRC_SHIFT;
            for(u32Counter =0U; u32Counter <= u32numMrc; u32Counter++)
            {
                u32numDescriptor = (uint32)((uint32)(XRDC_REG_READ8(XRDC_MRCFG_ADDR_ADDR32(u32Instance,Xrdc_MrcInstances[u32Instance][u32Counter])) & (uint32)XRDC_MRCFG_NMRGD_MASK) >> (uint32)XRDC_MRCFG_NMRGD_SHIFT);
                for(u32wordCounter =0U; u32wordCounter < u32numDescriptor; u32wordCounter++)
                {
                    MCAL_DATA_SYNC_BARRIER();
                    MCAL_INSTRUCTION_SYNC_BARRIER();
                    /*Lock XRDC_MRGD Register*/
                    XRDC_REG_RMW32(XRDC_MRGD_W_ADDR32(u32Instance, XRDC_WORD3, (((uint32)(Xrdc_MrcInstances[u32Instance][u32Counter]) << 4UL) +  u32wordCounter)), XRDC_XRDC_MRGD_W3_LK2_MASK, XRDC_MRGD_W3_LOCK);
                    
                    MCAL_DATA_SYNC_BARRIER();
                    MCAL_INSTRUCTION_SYNC_BARRIER();
                }
            }
        }
    }
}
#endif/*XRDC_IP_REGISTER_LOCK == STD_ON*/

/**
 * @brief         Get domain ID of the current bus master making the transaction request
 *
 * @details
 *
 * @param[in]     u32Instance:     XRDC Instance number
 * @return        uint32
 * @retval        DID of the bus master making the current transaction request
 *
 * @api
 *
 * @pre
 *
 */
uint32 Xrdc_Ip_GetDomainID_Privileged(uint32 u32Instance)
{
    uint32 u32Value;

#if (XRDC_IP_DEV_ERROR_DETECT == STD_ON)
    /* Check the input parameter */
    DevAssert(u32Instance < XRDC_COUNT);
#endif

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

    u32Value = XRDC_REG_READ32(XRDC_HWCFG1_ADDR32(u32Instance));

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

    return u32Value;
}

/**
* @brief        Initializes Extended Resource Domain Controller parameters
*
* @details      
*
* @param[in]    pXrdcConfig:     Pointer for configuration Xrdc
*
* @return       void
*
* @api
*
*/
void Xrdc_Ip_Init_Privileged(Xrdc_Ip_ConfigType const * pXrdcConfig)
{
    uint32 u32counter = 0U;
    uint32 u32InstanceCounter = 0U;
    const Xrdc_Ip_InstanceConfigType * pTemp = NULL_PTR;

#if (XRDC_IP_DEV_ERROR_DETECT == STD_ON)
    /* Check the input parameters */
    DevAssert(pXrdcConfig != NULL_PTR);
    DevAssert(pXrdcConfig->u32InstanceConfigCnt > 0UL);
    DevAssert(pXrdcConfig->pInstanceConfig != NULL_PTR);
#endif

    for (u32counter = 0; u32counter < pXrdcConfig->u32InstanceInUsedCnt; u32counter++)
    {
        /* Disable XRDC module */
        XRDC_REG_RMW32(XRDC_CR_ADDR32(pXrdcConfig->pInstanceInUsed[u32counter]),(uint32)XRDC_CR_GVLD_MASK, XRDC_CR_GVLD_DISABLE);
    }

    for(u32InstanceCounter = 0; u32InstanceCounter < pXrdcConfig->u32InstanceConfigCnt; u32InstanceCounter++)
    {
        pTemp = pXrdcConfig->pInstanceConfig[u32InstanceCounter];
        /* Configure all memory descriptors */
        for (u32counter = 0U; u32counter < pTemp->u32MemoryConfigCnt; u32counter++)
        {
           Xrdc_Memory_Config_Descriptor(pTemp->u32XrdcInstance, &pTemp->pMemoryDesConfig[u32counter]);
        }

        /* Configure all peripheral descriptors */
        for (u32counter = 0U; u32counter < pTemp->u32PeripheralConfigCnt; u32counter++)
        {
           Xrdc_Peripheral_Access_Config(pTemp->u32XrdcInstance, &pTemp->pPeripheralDescriptorConfig[u32counter]);
        }

        /* Init all domains*/
        for (u32counter = 0U; u32counter < pTemp->u32DomainConfigCnt; u32counter++)
        {
            Xrdc_Domain_Init(pTemp->u32XrdcInstance, &pTemp->pDomainConfig[u32counter]);
        }
    }

    for (u32counter = 0; u32counter < pXrdcConfig->u32InstanceInUsedCnt; u32counter++)
    {
        /* TODO: Add EA here */
        /* Flush pipelines */
        MCAL_DATA_SYNC_BARRIER();
        MCAL_INSTRUCTION_SYNC_BARRIER();

        /* Enable XRDC module */
        XRDC_REG_RMW32(XRDC_CR_ADDR32(pXrdcConfig->pInstanceInUsed[u32counter]), (uint32)XRDC_CR_GVLD_MASK, XRDC_CR_GVLD_ENABLE);

        MCAL_DATA_SYNC_BARRIER();
        MCAL_INSTRUCTION_SYNC_BARRIER();
    }
#if(XRDC_IP_REGISTER_LOCK == STD_ON)
    /* Lock XRDC Registers*/
    Xrdc_LockRegisters();
#endif/*XRDC_IP_REGISTER_LOCK == STD_ON*/
}
#if (XRDC_IP_DEV_ERROR_DETECT == STD_ON)
/**
* @brief        Chech if parameter MasterCore is valid
*
* @details      
*
* @param[in]    MasterCore:     MasterCore id
*
* @return       True if MasterCore is valid, False otherwise
*
* @api
*
*/
static inline boolean Xrdc_Ip_ValidateMastercore(Xrdc_Ip_MasterCoreType MasterCore)
{
    boolean ReturnValue;

    /*check the parameter*/
    switch (MasterCore)
    {
        case XRDC_CORE_M7_0:
        case XRDC_CORE_HSE:
        case XRDC_CORE_M7_1:
            ReturnValue = TRUE;
            break;
        default:
            /*error case: wrong master core id*/
            ReturnValue = FALSE;
            break;
    }
    return ReturnValue;
}

/**
* @brief        Chech if parameter eSecureAttr is valid
*
* @details      
*
* @param[in]    eSecureAttr:     eSecureAttr
*
* @return       True if eSecureAttr is valid, False otherwise
*
* @api
*
*/
static inline boolean Xrdc_Ip_ValidateSecureAttribute(Xrdc_Ip_SecureAttributeType eSecureAttr)
{
    boolean ReturnValue;

    /*check the parameter*/
    switch (eSecureAttr)
    {
        case XRDC_SECURE:
        case XRDC_NON_SECURE:
        case XRDC_INPUT_FROM_BUS_MASTER:
            ReturnValue = TRUE;
            break;
        default:
            /*error case: wrong secure attribute type */
            ReturnValue = FALSE;
            break;
    }
    return ReturnValue;
}
#endif
/**
* @brief         Assign Process ID value to a core master 
* @details
*
* @param[in]     MasterCore:      Index of configured core master
* @param[in]     u8Pid:             Process ID value
* @param[in]     eSecureAttr:       Secure attribute of configured core master
*
* @return        void
*
* @api
*
*/
void Xrdc_Ip_SetProcessID_Privileged(Xrdc_Ip_MasterCoreType MasterCore, uint8 u8Pid, Xrdc_Ip_SecureAttributeType eSecureAttr)
{
#if (XRDC_IP_DEV_ERROR_DETECT == STD_ON)
    if(u8Pid >= XRDC_PID_MAX_VALUE_U8)
    {
        DevAssert(FALSE);
    }
    else if(FALSE == Xrdc_Ip_ValidateMastercore(MasterCore))
    {
        DevAssert(FALSE);
    }
    else if(FALSE == Xrdc_Ip_ValidateSecureAttribute(eSecureAttr))
    {
        DevAssert(FALSE);
    }
    else
#endif
    { 
        if (XRDC_PID_HAS_BUILDIN_PID((uint32)MasterCore) == 0U)
        {
            if (eSecureAttr == XRDC_NON_SECURE)
            {
                MCAL_DATA_SYNC_BARRIER();
                MCAL_INSTRUCTION_SYNC_BARRIER();
                SchM_Enter_Rm_RM_EXCLUSIVE_AREA_00();
                XRDC_REG_RMW32(XRDC_PID_ADDR32((uint32)MasterCore), XRDC_PID_FIELD_MASK, ((((uint32)u8Pid) & XRDC_PID_NONSECURE_FIELD_MASK) | ((uint32)0x20)));
                SchM_Exit_Rm_RM_EXCLUSIVE_AREA_00();
                MCAL_DATA_SYNC_BARRIER();
                MCAL_INSTRUCTION_SYNC_BARRIER();
            }
            else
            {
                MCAL_DATA_SYNC_BARRIER();
                MCAL_INSTRUCTION_SYNC_BARRIER();
                SchM_Enter_Rm_RM_EXCLUSIVE_AREA_00();
                XRDC_REG_RMW32(XRDC_PID_ADDR32((uint32)MasterCore), XRDC_PID_FIELD_MASK, ((((uint32)u8Pid) & XRDC_PID_NONSECURE_FIELD_MASK)| ((uint32)0x00)));
                SchM_Exit_Rm_RM_EXCLUSIVE_AREA_00();
                MCAL_DATA_SYNC_BARRIER();
                MCAL_INSTRUCTION_SYNC_BARRIER();
            }
        }
    }
}



/**
* @brief        Initializes Extended Resource Domain Controller Instance parameters
*
* @details      
*
* @param[in]    pXrdcConfig:     Pointer for configuration Xrdc
*
* @return       void
*
* @api
*
*/

void Xrdc_Ip_InstanceInit_Privileged(Xrdc_Ip_InstanceConfigType const * pXrdcConfig)
{
    uint32 u32counter = 0U;

#if (XRDC_IP_DEV_ERROR_DETECT == STD_ON)
    /* Check the input parameters */
    DevAssert(pXrdcConfig != NULL_PTR);
#endif

    /* TODO: Add EA here */
    /* Flush pipelines */
    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();
    /* Disable XRDC module */
    XRDC_REG_RMW32(XRDC_CR_ADDR32(pXrdcConfig->u32XrdcInstance),(uint32)XRDC_CR_GVLD_MASK, XRDC_CR_GVLD_DISABLE);
    
    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();
    
    /* Configure all memory descriptors */
    for (u32counter = 0U; u32counter < pXrdcConfig->u32MemoryConfigCnt; u32counter++)
    {
       Xrdc_Memory_Config_Descriptor(pXrdcConfig->u32XrdcInstance, &pXrdcConfig->pMemoryDesConfig[u32counter]);
    }

    /* Configure all peripheral descriptors */
    for (u32counter = 0U; u32counter < pXrdcConfig->u32PeripheralConfigCnt; u32counter++)
    {
       Xrdc_Peripheral_Access_Config(pXrdcConfig->u32XrdcInstance, &pXrdcConfig->pPeripheralDescriptorConfig[u32counter]);
    }

    /* Init all domains*/
    for (u32counter = 0U; u32counter < pXrdcConfig->u32DomainConfigCnt; u32counter++)
    {
        Xrdc_Domain_Init(pXrdcConfig->u32XrdcInstance, &pXrdcConfig->pDomainConfig[u32counter]);
    }

    /* TODO: Add EA here */
    /* Flush pipelines */
    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();
    /* Disable XRDC module */
    XRDC_REG_RMW32(XRDC_CR_ADDR32(pXrdcConfig->u32XrdcInstance),(uint32)XRDC_CR_GVLD_MASK, XRDC_CR_GVLD_ENABLE);
    
    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();
}

/**
* @brief        Xrdc_Ip_GetDetailError
* @details      This function implement errata workaround for safety measure of all platform
*
* @param[in]    u32Instance                  Instance number of XRDC
* @param[in]    u8SubInstance               Instance number of MCR or PAC
* @param[out]   pXrdcDomainIDErrorStatus    Pointer get error status
*
* @param[out]
*
* @return
*
* @api
**/
static void Xrdc_Ip_GetDetailError(uint32 u32Instance, uint8 u8SubInstance, Xrdc_Ip_DomainIDErrStatusType * pXrdcDomainIDErrorStatus)
{
    uint32 u32ErrorAcces;
    uint32 u32ErrorState;
    uint32 u32ErrorStatus;
    volatile uint32 u32DomainIDAccess;

    /* Get error in DERR_W1_n */
    u32ErrorStatus = XRDC_REG_READ32(XRDC_DERR_W_ADDR32(u32Instance,XRDC_WORD1,(uint32)u8SubInstance));

    /* Get error state */
    u32ErrorState = (u32ErrorStatus & XRDC_ERROR_STATE_MASK_U32) >> XRDC_ERROR_STATE_SHIFT_U32;
    if (u32ErrorState < 2U)
    {
        (pXrdcDomainIDErrorStatus->ErrorStatus)[u8SubInstance].ErrState = XRDC_IP_NO_ACCESS;
    }
    else if (u32ErrorState == 2U)
    {
        (pXrdcDomainIDErrorStatus->ErrorStatus)[u8SubInstance].ErrState = XRDC_IP_SINGLE_ACCESS;
    }
    else
    {
        (pXrdcDomainIDErrorStatus->ErrorStatus)[u8SubInstance].ErrState = XRDC_IP_MULTIPLE_ACCESS;
    }

    /* Check if an access violation has occurred one (single) or more times (multiple) */
    if (u32ErrorState >= 2U)
    {
        MCAL_FAULT_INJECTION_POINT(T_XRDC_INJ);

        /* Get DomainId error */
        u32DomainIDAccess = XRDC_REG_READ32(XRDC_HWCFG1_ADDR32(u32Instance));
        if (XRDC_NUMBER_OF_DOMAINID > u32DomainIDAccess)
        {
            pXrdcDomainIDErrorStatus->DomainIDAccessError = (Xrdc_Ip_DomainIDType)u32DomainIDAccess;
        }
        else
        {
            pXrdcDomainIDErrorStatus->DomainIDAccessError = XRDC_UNKNOWN_DOMAIN;
        }
    
        /* Get address error */
        (pXrdcDomainIDErrorStatus->ErrorStatus)[u8SubInstance].u32AddError = XRDC_REG_READ32(XRDC_DERR_W_ADDR32(u32Instance,XRDC_WORD0,(uint32)u8SubInstance));
    
        /* Get address error remain */
        (pXrdcDomainIDErrorStatus->ErrorStatus)[u8SubInstance].u32AddErrorRemain = XRDC_REG_READ32(XRDC_DERR_W_ADDR32(u32Instance,XRDC_WORD2,(uint32)u8SubInstance));
    
        /* Get Error Port */
        (pXrdcDomainIDErrorStatus->ErrorStatus)[u8SubInstance].u32ErrPort = (u32ErrorStatus & XRDC_ERROR_PORT_MASK_U32) >> XRDC_ERROR_PORT_SHIFT_U32;
    
        /* Get Error Access Read or Write */
        u32ErrorAcces = ((u32ErrorStatus & XRDC_ERROR_ACCESS_MASK_U32) >> XRDC_ERROR_ACCESS_SHIFT_U32);
        if (u32ErrorAcces == 0U)
        {
            (pXrdcDomainIDErrorStatus->ErrorStatus)[u8SubInstance].ErrAccess = XRDC_IP_READ_ACCESS;
        }
        else if (u32ErrorAcces == 1U)
        {
            (pXrdcDomainIDErrorStatus->ErrorStatus)[u8SubInstance].ErrAccess = XRDC_IP_WRITE_ACCESS;
        }
        else
        {
            (pXrdcDomainIDErrorStatus->ErrorStatus)[u8SubInstance].ErrAccess = XRDC_IP_NOERR_ACCESS;
        }

        /* Get Error Attributes */
        (pXrdcDomainIDErrorStatus->ErrorStatus)[u8SubInstance].ErrAttribute = Xrdc_Ip_GetErrAttribute(((u32ErrorStatus & XRDC_ERROR_ATTRIBUTE_MASK_U32) >> XRDC_ERROR_ATTRIBUTE_SHIFT_U32)); 
    }
}
/**
* @brief        Retrieve error details
* @details      This function implement errata workaround for safety measure of all platform
*
* @param[in]    u8Instance:                 XRDC Instance number
* @param[out]   pXrdcDomainIDErrorStatus:   Pointer getting error status
*
* @param[out]
*
* @return       void
*
* @api
*
*/
void Xrdc_Ip_GetDomainIDErrorStatus_Privileged(uint32 u32Instance, Xrdc_Ip_DomainIDErrStatusType * pXrdcDomainIDErrorStatus)
{
    uint8  u8counter;
    uint8  u8PACInstanceNumber;
    uint8  u8MCRInstanceNumber;

#if (XRDC_IP_DEV_ERROR_DETECT == STD_ON)
    /* Check the input parameters */
    DevAssert(u32Instance < XRDC_COUNT);
    DevAssert(pXrdcDomainIDErrorStatus != NULL_PTR);
#endif
    u8MCRInstanceNumber = (uint8)(((XRDC_REG_READ32(XRDC_HWCFG0_ADDR32(u32Instance)) & XRDC_HWCFG0_NMRC_MASK) >> XRDC_HWCFG0_NMRC_SHIFT) +1U);
    {
        /*Check MCR error*/
        for (u8counter = 0U; u8counter < u8MCRInstanceNumber; u8counter++)
        {
            Xrdc_Ip_GetDetailError(u32Instance, u8counter, pXrdcDomainIDErrorStatus);
            /* Rearm Error Capture register */
            XRDC_REG_WRITE32(XRDC_DERR_W_ADDR32(u32Instance, XRDC_WORD3, (uint32)u8counter), XRDC_ERROR_RECR_U32);

            MCAL_DATA_SYNC_BARRIER();
            MCAL_INSTRUCTION_SYNC_BARRIER();
        }
    }

    u8PACInstanceNumber = (uint8)(((XRDC_REG_READ32(XRDC_HWCFG0_ADDR32(u32Instance)) & XRDC_HWCFG0_NPAC_MASK) >> XRDC_HWCFG0_NPAC_SHIFT) +1U);
    /*Check PAC error*/
    for (u8counter = 0; u8counter < u8PACInstanceNumber; u8counter++)
    {
        Xrdc_Ip_GetDetailError(u32Instance, u8counter + 16U, pXrdcDomainIDErrorStatus);
        /* Rearm Error Capture register */
        XRDC_REG_WRITE32(XRDC_DERR_W_ADDR32(u32Instance, XRDC_WORD3, (uint32)u8counter + 16U), XRDC_ERROR_RECR_U32);

        MCAL_DATA_SYNC_BARRIER();
        MCAL_INSTRUCTION_SYNC_BARRIER();
    }
}
/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
* @brief        Xrdc_Ip_Init
* @details
*
* @param[in]   pXrdcConfig    Pointer for configuration Xrdc
*
* @param[out]
*
* @return
*
* @api
*
*/
/* @implements    Xrdc_Ip_Init_Activity */
void Xrdc_Ip_Init(Xrdc_Ip_ConfigType const * pXrdcConfig)
{
#ifdef XRDC_IP_ENABLE_USER_MODE_SUPPORT
    OsIf_Trusted_Call1param(Xrdc_Ip_Init_Privileged, pXrdcConfig);
#else
    Xrdc_Ip_Init_Privileged(pXrdcConfig);
#endif
}

/**
* @brief        Xrdc_Ip_InstanceInit
* @details
*
* @param[in]   pXrdcInstanceConfig    Pointer for configuration Xrdc
*
* @param[out]
*
* @return
*
* @api
*
*/
void Xrdc_Ip_InstanceInit(Xrdc_Ip_InstanceConfigType const * pXrdcInstanceConfig)
{
#ifdef XRDC_IP_ENABLE_USER_MODE_SUPPORT
    OsIf_Trusted_Call1param(Xrdc_Ip_InstanceInit_Privileged, pXrdcInstanceConfig);
#else
    Xrdc_Ip_InstanceInit_Privileged(pXrdcInstanceConfig);
#endif
}

/**
 * @brief         Xrdc_Ip_GetDomainID
 *
 * @details
 *
 * @param[in]    u32Instance                 Instance number
 * @return
 *
 * @api
 *
 * @pre
 *
 */
/* @implements    Xrdc_Ip_GetDomainID_Activity */
uint32 Xrdc_Ip_GetDomainID(uint32 u32Instance)
{
    uint32 ReturnValue = (uint32)XRDC_UNKNOWN_DOMAIN;
#if (XRDC_IP_DEV_ERROR_DETECT == STD_ON)
    /* Check the input parameters */
    if(u32Instance >= XRDC_COUNT)
    {
        DevAssert(FALSE);
    }
    else
#endif
    {
#ifdef XRDC_IP_ENABLE_USER_MODE_SUPPORT
        ReturnValue = OsIf_Trusted_Call_Return1param(Xrdc_Ip_GetDomainID_Privileged, u32Instance);
#else
        ReturnValue = Xrdc_Ip_GetDomainID_Privileged(u32Instance);
#endif
    }
    if(ReturnValue >= XRDC_NUMBER_OF_DOMAINID)
    {
        ReturnValue = (uint32)XRDC_UNKNOWN_DOMAIN;
    }
    return ReturnValue;
}
/**
* @brief        Xrdc_Ip_SetProcessID
* @details
*
* @param[in]     MasterCore
* @param[in]     u8Pid
* @param[in]     eSecureAttr
*
* @return
*
* @api
*
*/
/* @implements    Xrdc_Ip_SetProcessID_Activity */
void Xrdc_Ip_SetProcessID(Xrdc_Ip_MasterCoreType MasterCore, uint8 u8Pid, Xrdc_Ip_SecureAttributeType eSecureAttr)
{
#if (XRDC_IP_DEV_ERROR_DETECT == STD_ON)
#ifdef XRDC_IP_PID_REG_LOCK 
#if (XRDC_IP_PID_REG_LOCK == XRDC_PID_FULLY_LOCKED)
    DevAssert(FALSE);
#endif
#endif/*XRDC_IP_PID_REG_LOCK*/
    if(u8Pid >= XRDC_PID_MAX_VALUE_U8)
    {
        DevAssert(FALSE);
    }
    else if(FALSE == Xrdc_Ip_ValidateMastercore(MasterCore))
    {
        DevAssert(FALSE);
    }
    else if(FALSE == Xrdc_Ip_ValidateSecureAttribute(eSecureAttr))
    {
        DevAssert(FALSE);
    }
    else
#endif
    {
#ifdef XRDC_IP_ENABLE_USER_MODE_SUPPORT
        OsIf_Trusted_Call3params(Xrdc_Ip_SetProcessID_Privileged, MasterCore, u8Pid, eSecureAttr);
#else
        Xrdc_Ip_SetProcessID_Privileged(MasterCore, u8Pid, eSecureAttr);
#endif
    }
}


/**
* @brief        Xrdc_Ip_GetDomainIDErrorStatus
* @details      This function implement errata workaround for safety measure of all platform
*
* @param[in]    u32Instance                 Instance number
* @param[out]   pXrdcDomainIDErrorStatus    Pointer get error status
*
* @param[out]
*
* @return
*
* @api
*
*/
/* @implements    Xrdc_Ip_GetDomainIDErrorStatus_Activity */
void Xrdc_Ip_GetDomainIDErrorStatus(uint32 u32Instance, Xrdc_Ip_DomainIDErrStatusType * pXrdcDomainIDErrorStatus)
{
#ifdef XRDC_IP_ENABLE_USER_MODE_SUPPORT
    OsIf_Trusted_Call2params(Xrdc_Ip_GetDomainIDErrorStatus_Privileged, u32Instance, pXrdcDomainIDErrorStatus);
#else
    Xrdc_Ip_GetDomainIDErrorStatus_Privileged(u32Instance, pXrdcDomainIDErrorStatus);
#endif
}

#define RM_STOP_SEC_CODE
#include "Rm_MemMap.h"

#endif
#endif

#ifdef __cplusplus
}
#endif

/** @} */
