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
*   @file Mpu_M7_Ip.c
*
*   @addtogroup Mpu_M7_Ip Mpu M7 IPV Driver
*   @{
*/

#ifdef __cplusplus
extern "C"
{
#endif

/*==================================================================================================
*                                        INCLUDE FILES
==================================================================================================*/
#include "Mpu_M7_Ip.h"

#ifdef MPU_M7_IP_DEV_ERROR_DETECT
#if(MPU_M7_IP_DEV_ERROR_DETECT == STD_ON)
    #include "Devassert.h"
#endif
#endif

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define RM_MPU_M7_IP_VENDOR_ID_C                      43
#define RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION_C       4
#define RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION_C       4
#define RM_MPU_M7_IP_AR_RELEASE_REVISION_VERSION_C    0
#define RM_MPU_M7_IP_SW_MAJOR_VERSION_C               2
#define RM_MPU_M7_IP_SW_MINOR_VERSION_C               0
#define RM_MPU_M7_IP_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Mpu_M7_Ip.h */
#if (RM_MPU_M7_IP_VENDOR_ID_C !=  RM_MPU_M7_IP_VENDOR_ID)
    #error "Mpu_M7_Ip.c and Mpu_M7_Ip.h have different vendor ids"
#endif
#if ((RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION_C    !=  RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION) || \
     (RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION_C    !=  RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION) || \
     (RM_MPU_M7_IP_AR_RELEASE_REVISION_VERSION_C !=  RM_MPU_M7_IP_AR_RELEASE_REVISION_VERSION) \
    )
     #error "AUTOSAR Version Numbers of Mpu_M7_Ip.c and Mpu_M7_Ip.h are different"
#endif
#if ((RM_MPU_M7_IP_SW_MAJOR_VERSION_C !=  RM_MPU_M7_IP_SW_MAJOR_VERSION) || \
     (RM_MPU_M7_IP_SW_MINOR_VERSION_C !=  RM_MPU_M7_IP_SW_MINOR_VERSION) || \
     (RM_MPU_M7_IP_SW_PATCH_VERSION_C !=  RM_MPU_M7_IP_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Mpu_M7_Ip.c and Mpu_M7_Ip.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
#ifdef MPU_M7_IP_DEV_ERROR_DETECT
#if(MPU_M7_IP_DEV_ERROR_DETECT == STD_ON)
    /* Check if the files Mpu_M7_Ip.c and Devassert.h are of the same version */
    #if ((RM_MPU_M7_IP_AR_RELEASE_MAJOR_VERSION_C != DEVASSERT_AR_RELEASE_MAJOR_VERSION) || \
        (RM_MPU_M7_IP_AR_RELEASE_MINOR_VERSION_C != DEVASSERT_AR_RELEASE_MINOR_VERSION)     \
        )
        #error "AUTOSAR Version Numbers of Mpu_M7_Ip.c and Devassert.h are different"
    #endif
#endif
#endif
#endif
/*==================================================================================================
*                                       LOCAL VARIABLES
==================================================================================================*/
#ifdef  RM_IP_ENABLE_MPU_M7
#if  (RM_IP_ENABLE_MPU_M7 == STD_ON)

#ifdef MPU_M7_ENABLE_CLEAR_CACHE
#if (MPU_M7_ENABLE_CLEAR_CACHE == STD_ON)
#define RM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Rm_MemMap.h"

static boolean bCacheClearedFromInit = FALSE;

#define RM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Rm_MemMap.h"
#endif
#endif

#define RM_START_SEC_CODE
#include "Rm_MemMap.h"
/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/

void Mpu_M7_Ip_Init_Privileged(const Mpu_M7_Ip_ConfigType * pConfig);

void Mpu_M7_Ip_SetRegionConfig_Privileged(uint8 u8RegionNum,
                                          const Mpu_M7_Ip_RegionConfigType * const pUserConfigPtr
                                         );

void Mpu_M7_Ip_Deinit_Privileged(void);

void Mpu_M7_Ip_EnableRegion_Privileged(uint8 u8RegionNum, boolean bEnable);

void Mpu_M7_Ip_SetAccessRight_Privileged(uint8 u8RegionNum, Mpu_M7_Ip_AccessRightsType eRights);

static uint8 Mpu_M7_Ip_GetDRegion(S32_MPU_Type const * const base);

static uint32 Mpu_M7_Ip_CalculateRegionSize(uint32 u32StartAddr, uint32 u32EndAddr);

static void Mpu_M7_Ip_SetMemoryType(uint32 * pRegionAttr, Mpu_M7_Ip_MemoryType eType);

static void Mpu_M7_Ip_SetCachePolicies(uint32 * pRegionAttr,
                                       Mpu_M7_Ip_CachePolicyType eInnerPolicy,
                                       Mpu_M7_Ip_CachePolicyType eOuterPolicy
                                      );

static void Mpu_M7_Ip_ComputeAccessRights(uint32 * pRegionAttr, Mpu_M7_Ip_AccessRightsType eAccessRights);

void Mpu_M7_Ip_GetErrorRegisters(uint32 * pMmfsr, uint32 * pAddress);

#if (MPU_M7_IP_MULTICORE_SUPPORT == STD_ON)

void Mpu_M7_Ip_MulticoreInit_Privileged(void);

static uint8 Mpu_M7_GetCoreId(void);
#endif /* #if (MPU_M7_IP_MULTICORE_SUPPORT == STD_ON) */

/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
/*!
 *  @brief Get Memory regions
 *
 *  @param[in] base The MPU peripheral base address.
 *  @return Number of regions supported by MPU ,if 0 MPU is not supported by core.
 *
 */
static uint8 Mpu_M7_Ip_GetDRegion(S32_MPU_Type const * const base)
{
    return (uint8)((base->TYPE & S32_MPU_TYPE_DREGION_MASK)>>S32_MPU_TYPE_DREGION_SHIFT) & 0xFFU;
}

/*!
 * @brief Calculate log2(n) size for a specific region
 *
 * @param[in] u32StartAddr Region Start address
 * @param[in] u32EndAddr   Region End address
 *
 * @return Region size as a power of 2
 */
static uint32 Mpu_M7_Ip_CalculateRegionSize(uint32 u32StartAddr, uint32 u32EndAddr)
{
    uint32 finalSize = 0UL;
    uint32 u32Size = 0UL;

    u32Size = u32EndAddr - u32StartAddr;
    while (u32Size > 0U)
    {
        finalSize++;
        u32Size >>= 1U;
    }

    return finalSize;
}

/*!
 * @brief Configure memory type
 *
 * @param[out] pRegionAttr RASR register where to save the attributes
 * @param[in]  eType       Memory type
 *
 * @return void
 */
static void Mpu_M7_Ip_SetMemoryType(uint32 * pRegionAttr, Mpu_M7_Ip_MemoryType eType)
{
    /* 7-6, 5-3, 2, 1, 0 bit S always clear to not influence previous setting
     *      Res, TEX, S, C, B */
    switch(eType)
    {
        case MPU_M7_MEM_STRONG_ORDER:
            /* do nothing - 0b00000000,  MPU_M7_MEMORY_TYPE_STRONG_ORDER  */
            break;
        case MPU_M7_MEM_DEVICE_SHARED:
            *pRegionAttr |= ((uint32)0x01U << S32_MPU_A_RASR_B_SHIFT);/*0b00000001,  MPU_M7_MEMORY_TYPE_DEVICE_SHARED */
            break;
        case MPU_M7_MEM_NORMAL_IO_WR_THROUGH:
            *pRegionAttr |= ((uint32)0x02U << S32_MPU_A_RASR_B_SHIFT);/*0b00000010,  MPU_M7_MEMORY_TYPE_NORMAL_IN_OUT_WRITE_THROUGH */
            break;
        case MPU_M7_MEM_NORMAL_IO_WR_BACK1:
            *pRegionAttr |= ((uint32)0x03U << S32_MPU_A_RASR_B_SHIFT);/*0b00000011,  MPU_M7_MEMORY_TYPE_NORMAL_IN_OUT_WRITE_BACK1 */
            break;
        case MPU_M7_MEM_NORMAL_IO_NO_CACHE:
            *pRegionAttr |= ((uint32)0x08U << S32_MPU_A_RASR_B_SHIFT);/*0b00001000,  MPU_M7_MEMORY_TYPE_NORMAL_IN_OUT_NO_CACHE */
            break;
        case MPU_M7_MEM_NORMAL_IO_WR_BACK2:
            *pRegionAttr |= ((uint32)0x0BU << S32_MPU_A_RASR_B_SHIFT);/*0b00001011,  MPU_M7_MEMORY_TYPE_NORMAL_IN_OUT_WRITE_BACK2 */
            break;
        case MPU_M7_MEM_DEVICE_NOSHARE:
            *pRegionAttr |= ((uint32)0x10U << S32_MPU_A_RASR_B_SHIFT);/*0b00010000,  MPU_M7_MEMORY_TYPE_DEVICE_NOSHARE */
            break;
        case MPU_M7_MEM_NORMAL_CACHEABLE:
            *pRegionAttr |= ((uint32)0x20U << S32_MPU_A_RASR_B_SHIFT);/*0b00100000   MPU_M7_MEMORY_TYPE_NORMAL_CACHEABLE */
            break;
        default:
            /* fix misra rule 16.4*/
            break;
    }

}

/*!
 * @brief Set inner and outer cache policies
 *
 * @param[out] pRegionAttr   RASR register where to save the attributes
 * @param[in]  eInnerPolicy  Inner cache policy
 * @param[in]  eOuterPolicy  Outer cache policy
 *
 * @return void
 */
static void Mpu_M7_Ip_SetCachePolicies(uint32 * pRegionAttr,
                                       Mpu_M7_Ip_CachePolicyType eInnerPolicy,
                                       Mpu_M7_Ip_CachePolicyType eOuterPolicy
                                      )
{
    *pRegionAttr |= (S32_MPU_RASR_C((uint32)eInnerPolicy >> 1UL) | S32_MPU_RASR_B((uint32)eInnerPolicy));
    *pRegionAttr |= (S32_MPU_RASR_TEX(0x4UL | (uint32)eOuterPolicy));
}

/*!
 * @brief Set access rights
 *
 * @param[out] pRegionAttr   RASR register where to save the attributes
 * @param[in]  eAccessRights Region access rights
 *
 * @return void
 */
static void Mpu_M7_Ip_ComputeAccessRights(uint32 * pRegionAttr, Mpu_M7_Ip_AccessRightsType eAccessRights)
{
    *pRegionAttr &= ~(S32_MPU_RASR_AP_MASK | S32_MPU_RASR_XN_MASK);
    *pRegionAttr |= S32_MPU_RASR_AP(eAccessRights);
    if (((uint8)eAccessRights & (uint8)MPU_M7_IP_EXECUTE_RIGHT_MASK) == (uint8)0U)
    {
        *pRegionAttr |= S32_MPU_RASR_XN(1UL);
    }
    else
    {
        *pRegionAttr &= ~S32_MPU_RASR_XN(1UL);
    }

}

/*!
 * @brief Retrieve fault information from SCB
 *
 * @param[out] pMmfsr   SCB->MMFSR register contents
 * @param[out] pAddress Address of the violation
 *
 * @return void
 */
void Mpu_M7_Ip_GetErrorRegisters(uint32 * pMmfsr, uint32 * pAddress)
{
    *pMmfsr = S32_SCB->CFSR & (uint32)MPU_M7_IP_SCB_CFSR_MMFSR_MASK_WITHOUT_VALID;
    if (((S32_SCB->CFSR & (uint32)S32_SCB_CFSR_MMFSR_MMARVALID_MASK) >> (uint32)S32_SCB_CFSR_MMFSR_MMARVALID_SHIFT) == (uint32)1UL)
    {
        *pAddress = (uint32)S32_SCB->MMFAR;
    }
    else
    {
        *pAddress = 0xFFFFFFFFUL;
    }
}

#if (MPU_M7_IP_MULTICORE_SUPPORT == STD_ON)
static uint8 Mpu_M7_GetCoreId(void)
{
    return (uint8)(IP_MSCM->CPXNUM & MSCM_CPXNUM_CPN_MASK);
}
#endif /* #if (MPU_M7_IP_MULTICORE_SUPPORT == STD_ON) */

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
 */
void Mpu_M7_Ip_Init_Privileged(const Mpu_M7_Ip_ConfigType * pConfig)
{
    uint8 region;
    const Mpu_M7_Ip_RegionConfigType * regionCfg;
    S32_MPU_Type * base = S32_MPU;

#if (MPU_M7_IP_DEV_ERROR_DETECT == STD_ON)
    /* Check the input parameters */
    DevAssert(pConfig != NULL_PTR);
    DevAssert(pConfig->pRegionConfigArr != NULL_PTR);
    DevAssert(pConfig->u8RegionCnt > 0U);
    DevAssert(pConfig->u8RegionCnt <= Mpu_M7_Ip_GetDRegion(base));
#endif

/*Clear cache if configured */
#ifdef MPU_M7_ENABLE_CLEAR_CACHE
#if (MPU_M7_ENABLE_CLEAR_CACHE == STD_ON)
    bCacheClearedFromInit = TRUE;

    /*disable cache*/
    Cache_Ip_Disable(CACHE_IP_ALL);

    for (region = 0U; region < pConfig->u8RegionCnt; region++)
    {
        regionCfg = &pConfig->pRegionConfigArr[region];

        if(regionCfg->eMemType == MPU_M7_MEM_NORMAL_CACHEABLE)
        {
            /*clear cache*/
            (void)Cache_Ip_CleanByAddr(CACHE_IP_ALL, TRUE, (uint32)regionCfg->u32StartAddr, ((uint32)(regionCfg->u32EndAddr - regionCfg->u32StartAddr + 1)));
        }
    }
#endif
#endif

    /* Reset the peripheral to the default settings */
    Mpu_M7_Ip_Deinit();

    /* Configure MPU general settings */
    if (pConfig->bDefaultMapEn == TRUE)
    {
        base->CTRL |=S32_MPU_CTRL_PRIVDEFENA_MASK;
    }
    else
    {
        base->CTRL &= (~((uint32)S32_MPU_CTRL_PRIVDEFENA_MASK));
    }

    if (pConfig->bEnableRunHFNMI == TRUE)
    {
        base->CTRL |=S32_MPU_CTRL_HFNMIENA_MASK;
    }
    else
    {
        base->CTRL &= (~((uint32)S32_MPU_CTRL_HFNMIENA_MASK));
    }

    if (pConfig->bEnMemManageInterrupt == TRUE)
    {
        S32_SCB->SHCSR |= S32_SCB_SHCSR_MEMFAULTENA_MASK;
    }
    else
    {
        S32_SCB->SHCSR &= ~S32_SCB_SHCSR_MEMFAULTENA_MASK;
    }

    for (region = 0U; region < pConfig->u8RegionCnt; region++)
    {
        regionCfg = &pConfig->pRegionConfigArr[region];
        Mpu_M7_Ip_SetRegionConfig_Privileged((pConfig->pRegionConfigArr[region]).u8RegionNum, regionCfg);
    }

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

    base->CTRL |= S32_MPU_CTRL_ENABLE(1U);

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

#ifdef MPU_M7_ENABLE_CLEAR_CACHE
#if (MPU_M7_ENABLE_CLEAR_CACHE == STD_ON)
    for (region = 0U; region < pConfig->u8RegionCnt; region++)
    {
        regionCfg = &pConfig->pRegionConfigArr[region];

        if(regionCfg->eMemType == MPU_M7_MEM_NORMAL_CACHEABLE)
        {
            /*clear cache*/
            (void)Cache_Ip_CleanByAddr(CACHE_IP_ALL, TRUE, (uint32)regionCfg->u32StartAddr, ((uint32)(regionCfg->u32EndAddr - regionCfg->u32StartAddr + 1)));
        }
    }

    /*enable cache*/
    Cache_Ip_Enable(CACHE_IP_ALL);

    bCacheClearedFromInit = TRUE;
#endif
#endif
}

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
void Mpu_M7_Ip_MulticoreInit_Privileged(void)
{
    uint32 u32counter = 0U;
    const Mpu_M7_Ip_ConfigType * pMpuCfg;
    for (u32counter = 0U; u32counter < MPU_M7_IP_NUM_CONFIG_U32; u32counter++)
    {
        pMpuCfg = Mpu_M7_Gather_Config[u32counter];
        if (Mpu_M7_GetCoreId() == pMpuCfg->ConfigCore)
        {
            Mpu_M7_Ip_Init_Privileged(Mpu_M7_Gather_Config[u32counter]);
        }
    }
}
#endif /* #if (MPU_M7_IP_MULTICORE_SUPPORT == STD_ON) */

/**
 * @brief         Configures the region selected by u8RegionNum with the data from pUserConfigPtr
 *
 * @details       This function is Reentrant
 *
 * @param[in]     u8RegionNum:    region number
 * @param[in]     pUserConfigPtr: pointer to the region configuration structure for MPU M7.
 * @return        void
 *
 * @api
 *
 * @pre
 *
 */
void Mpu_M7_Ip_SetRegionConfig_Privileged(uint8 u8RegionNum,
                                          const Mpu_M7_Ip_RegionConfigType * const pUserConfigPtr
                                         )
{
    S32_MPU_Type * base = S32_MPU;
    uint32 regionSize;
    uint32 regionAttributes = 0UL;

#if (MPU_M7_IP_DEV_ERROR_DETECT == STD_ON)
    /* Check if the region size is at least the minimum supported */
    DevAssert(u8RegionNum < Mpu_M7_Ip_GetDRegion(base));
    DevAssert(pUserConfigPtr != NULL_PTR);
    DevAssert((pUserConfigPtr->u32EndAddr - pUserConfigPtr->u32StartAddr) >= MPU_M7_IP_MIN_REGION_SIZE);
#endif

#ifdef MPU_M7_ENABLE_CLEAR_CACHE
#if (MPU_M7_ENABLE_CLEAR_CACHE == STD_ON)
    if(bCacheClearedFromInit == FALSE)
    {
        /*disable cache*/
        Cache_Ip_Disable(CACHE_IP_ALL);

        if(pUserConfigPtr->eMemType == MPU_M7_MEM_NORMAL_CACHEABLE)
        {
            /*clear cache*/
            (void)Cache_Ip_CleanByAddr(CACHE_IP_ALL, TRUE, (uint32)pUserConfigPtr->u32StartAddr, ((uint32)(pUserConfigPtr->u32EndAddr - pUserConfigPtr->u32StartAddr + 1)));
        }
    }
#endif
#endif

    /* Calculate log2(region) */
    regionSize = Mpu_M7_Ip_CalculateRegionSize(pUserConfigPtr->u32StartAddr, pUserConfigPtr->u32EndAddr);

#if (MPU_M7_IP_DEV_ERROR_DETECT == STD_ON)
    /* Check if the region start address is aligned to the region's size */
    DevAssert((pUserConfigPtr->u32StartAddr & ((1UL << regionSize) - 1UL)) == 0UL);
#endif

    /* Set the region number */
    base->RNR = u8RegionNum;
    /* Set the region base address */
    base->RBAR = pUserConfigPtr->u32StartAddr;
    /* Set the region size */
    regionAttributes |= S32_MPU_RASR_SIZE(regionSize - 1UL);

    /* Configure sub region disable mask if the size of the region allows it */
    if ((pUserConfigPtr->u32EndAddr - pUserConfigPtr->u32StartAddr) >= (uint32)MPU_M7_IP_MIN_REGION_SIZE_WITH_SUBREGION)
    {
        regionAttributes |= S32_MPU_RASR_SRD(pUserConfigPtr->u8SubRegMask);
    }
    else
    {
        regionAttributes |= S32_MPU_RASR_SRD(0U);
    }

    /* Configure shareability */
    if (pUserConfigPtr->bShareable == TRUE)
    {
        regionAttributes |=S32_MPU_RASR_S_MASK;
    }
    else
    {
        regionAttributes &= (~((uint32)S32_MPU_RASR_S_MASK));
    }

    /* Set memory attributes and cacheability options */
    if (pUserConfigPtr->eMemType != MPU_M7_MEM_NORMAL_CACHEABLE)
    {
        Mpu_M7_Ip_SetMemoryType(&regionAttributes, pUserConfigPtr->eMemType);
    }
    else
    {
        Mpu_M7_Ip_SetCachePolicies(&regionAttributes, pUserConfigPtr->eInnerCachePolicy, pUserConfigPtr->eOuterCachePolicy);
    }

    /* Compute access rights */
    Mpu_M7_Ip_ComputeAccessRights(&regionAttributes, pUserConfigPtr->eAccessRight);

    /* Enable Region */
    regionAttributes |= S32_MPU_RASR_ENABLE(1UL);

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

    /* Write final values */
    base->RASR = regionAttributes;

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

#ifdef MPU_M7_ENABLE_CLEAR_CACHE
#if (MPU_M7_ENABLE_CLEAR_CACHE == STD_ON)
    if(bCacheClearedFromInit == FALSE)
    {
        if(pUserConfigPtr->eMemType == MPU_M7_MEM_NORMAL_CACHEABLE)
        {
            /*clear cache*/
            (void)Cache_Ip_CleanByAddr(CACHE_IP_ALL, TRUE, (uint32)pUserConfigPtr->u32StartAddr, ((uint32)(pUserConfigPtr->u32EndAddr - pUserConfigPtr->u32StartAddr + 1)));
        }

        /*disable cache*/
        Cache_Ip_Enable(CACHE_IP_ALL);
    }
#endif
#endif
}

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
 */
void Mpu_M7_Ip_Deinit_Privileged(void)
{
    uint8 region;
    S32_MPU_Type * base = S32_MPU;

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

    base->CTRL &= ~(S32_MPU_CTRL_PRIVDEFENA_MASK | S32_MPU_CTRL_HFNMIENA_MASK | S32_MPU_CTRL_ENABLE_MASK);

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

    for (region = 0U; region < Mpu_M7_Ip_GetDRegion(base); region++)
    {
        base->RNR = (uint32)region;
        base->RASR = 0UL;
        base->RBAR = 0UL;
    }
}

/**
 * @brief         Enables or disabled a specific region
 *
 * @details       This function is Reentrant
 *
 * @param[in]     u8RegionNum : Region to be modified
 * @param[in]     bEnable  : Specifies whether the region is enabled or disabled
 * @return        void
 *
 * @api
 *
 * @pre
 *
 */
void Mpu_M7_Ip_EnableRegion_Privileged(uint8 u8RegionNum, boolean bEnable)
{
    S32_MPU_Type * base = S32_MPU;

#if (MPU_M7_IP_DEV_ERROR_DETECT == STD_ON)
    /* Check the input parameters */
    DevAssert(u8RegionNum < Mpu_M7_Ip_GetDRegion(base));
#endif

    base->RNR = (uint32)u8RegionNum;
    if (bEnable == TRUE)
    {
        MCAL_DATA_SYNC_BARRIER();
        MCAL_INSTRUCTION_SYNC_BARRIER();

        base->RASR |= S32_MPU_RASR_ENABLE(1UL);

        MCAL_DATA_SYNC_BARRIER();
        MCAL_INSTRUCTION_SYNC_BARRIER();
    }
    else
    {
        MCAL_DATA_SYNC_BARRIER();
        MCAL_INSTRUCTION_SYNC_BARRIER();

        base->RASR &= ~S32_MPU_RASR_ENABLE(1UL);

        MCAL_DATA_SYNC_BARRIER();
        MCAL_INSTRUCTION_SYNC_BARRIER();
    }
}

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
 */
void Mpu_M7_Ip_SetAccessRight_Privileged(uint8 u8RegionNum, Mpu_M7_Ip_AccessRightsType eRights)
{
    uint32 regionAttributes;
    S32_MPU_Type * base = S32_MPU;

#if (MPU_M7_IP_DEV_ERROR_DETECT == STD_ON)
    /* Check the input parameters */
    DevAssert(u8RegionNum < Mpu_M7_Ip_GetDRegion(base));
#endif

    base->RNR = (uint32)u8RegionNum;
    regionAttributes = base->RASR;
    Mpu_M7_Ip_ComputeAccessRights(&regionAttributes, eRights);

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

    base->RASR = regionAttributes;

    MCAL_DATA_SYNC_BARRIER();
    MCAL_INSTRUCTION_SYNC_BARRIER();

}

/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/**
 * @brief         Initializes the Memory Protection Unit general parameters and region configurations
 * @implements    Mpu_M7_Ip_Init_Activity
 */
void Mpu_M7_Ip_Init(const Mpu_M7_Ip_ConfigType * pConfig)
{
#ifdef MPU_M7_IP_ENABLE_USER_MODE_SUPPORT
    OsIf_Trusted_Call1param(Mpu_M7_Ip_Init_Privileged, pConfig);
#else
    Mpu_M7_Ip_Init_Privileged(pConfig);
#endif
}

/**
 * @brief         Configures the region selected by u8RegionNum with the data from pUserConfigPtr
 * @implements    Mpu_M7_Ip_SetRegionConfig_Activity
 */
void Mpu_M7_Ip_SetRegionConfig(uint8 u8RegionNum, const Mpu_M7_Ip_RegionConfigType * const pUserConfigPtr)
{
#ifdef MPU_M7_IP_ENABLE_USER_MODE_SUPPORT
    OsIf_Trusted_Call2params(Mpu_M7_Ip_SetRegionConfig_Privileged, u8RegionNum, pUserConfigPtr);
#else
    Mpu_M7_Ip_SetRegionConfig_Privileged(u8RegionNum, pUserConfigPtr);
#endif
}

/**
 * @brief         Disables the module and resets all region configurations
 * @implements    Mpu_M7_Ip_Deinit_Activity
 */
void Mpu_M7_Ip_Deinit(void)
{
#ifdef MPU_M7_IP_ENABLE_USER_MODE_SUPPORT
    OsIf_Trusted_Call(Mpu_M7_Ip_Deinit_Privileged);
#else
    Mpu_M7_Ip_Deinit_Privileged();
#endif
}

/**
 * @brief         Enables or disabled a specific region
 * @implements    Mpu_M7_Ip_EnableRegion_Activity
 */
void Mpu_M7_Ip_EnableRegion(uint8 u8RegionNum, boolean bEnable)
{
#ifdef MPU_M7_IP_ENABLE_USER_MODE_SUPPORT
    OsIf_Trusted_Call2params(Mpu_M7_Ip_EnableRegion_Privileged, u8RegionNum, bEnable);
#else
    Mpu_M7_Ip_EnableRegion_Privileged(u8RegionNum, bEnable);
#endif
}

/**
 * @brief         Modify the access rights for a specific region
 * @implements    Mpu_M7_Ip_SetAccessRight_Activity
 */
void Mpu_M7_Ip_SetAccessRight(uint8 u8RegionNum, Mpu_M7_Ip_AccessRightsType eRights)
{
#ifdef MPU_M7_IP_ENABLE_USER_MODE_SUPPORT
    OsIf_Trusted_Call2params(Mpu_M7_Ip_SetAccessRight_Privileged, u8RegionNum, eRights);
#else
    Mpu_M7_Ip_SetAccessRight_Privileged(u8RegionNum, eRights);
#endif
}

/**
 * @brief         Retrieve error details
 * @implements    Mpu_M7_Ip_GetErrorDetails_Activity
 */
boolean Mpu_M7_Ip_GetErrorDetails(Mpu_M7_Ip_ErrorDetailsType * pErrorDetails)
{
    boolean result = FALSE;
    uint32  mmfsr;
    uint32  mmfsrCopy;
    uint32  errorAddress;
    uint8   errorCount = 0U;

#if (MPU_M7_IP_DEV_ERROR_DETECT == STD_ON)
    DevAssert(pErrorDetails != NULL_PTR);
#endif

#ifdef MPU_M7_IP_ENABLE_USER_MODE_SUPPORT
    OsIf_Trusted_Call2params(Mpu_M7_Ip_GetErrorRegisters, &mmfsr, &errorAddress);
#else
    Mpu_M7_Ip_GetErrorRegisters(&mmfsr, &errorAddress);
#endif
    mmfsrCopy = mmfsr;
    while (mmfsr != 0U)
    {
        errorCount += (uint8)(mmfsr & 1UL);
        mmfsr >>= 1UL;
    }

    if (errorCount > 1U)
    {
        pErrorDetails->eType = MPU_M7_MEMMAN_ERROR_MULTIPLE;
    }
    else
    {
        mmfsr = mmfsrCopy;
        switch(mmfsr)
        {
            case (1U << S32_SCB_CFSR_MMFSR_IACCVIOL_SHIFT):
                pErrorDetails->eType = MPU_M7_MEMMAN_ERROR_INSTRUCTION_ACCESS;
                break;
            case (1U << S32_SCB_CFSR_MMFSR_DACCVIOL_SHIFT):
                pErrorDetails->eType = MPU_M7_MEMMAN_ERROR_DATA_ACCESS;
                break;
            case (1U << S32_SCB_CFSR_MMFSR_MUNSTKERR_SHIFT):
                pErrorDetails->eType = MPU_M7_MEMMAN_ERROR_UNSTACK;
                break;
            case (1U << S32_SCB_CFSR_MMFSR_MSTKERR_SHIFT):
                pErrorDetails->eType = MPU_M7_MEMMAN_ERROR_STACK;
                break;
            case (1U << S32_SCB_CFSR_MMFSR_MLSPERR_SHIFT):
                pErrorDetails->eType = MPU_M7_MEMMAN_ERROR_FLOATING_POINT;
                break;
            default:
                pErrorDetails->eType = MPU_M7_MEMMAN_ERROR_NONE;
                break;
        }
    }

    pErrorDetails->u32Address = errorAddress;

    if (errorCount > 0U)
    {
        result = TRUE;
    }

    return result;
}

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
void Mpu_M7_Ip_MulticoreInit(void)
{
#ifdef MPU_M7_IP_ENABLE_USER_MODE_SUPPORT
    OsIf_Trusted_Call(Mpu_M7_Ip_MulticoreInit_Privileged);
#else
    Mpu_M7_Ip_MulticoreInit_Privileged();
#endif
}

#endif /* MPU_M7_IP_MULTICORE_SUPPORT == STD_ON */

#define RM_STOP_SEC_CODE
#include "Rm_MemMap.h"

#endif
#endif  /* RM_IP_ENABLE_MPU_M7*/

#ifdef __cplusplus
}
#endif

/** @} */
