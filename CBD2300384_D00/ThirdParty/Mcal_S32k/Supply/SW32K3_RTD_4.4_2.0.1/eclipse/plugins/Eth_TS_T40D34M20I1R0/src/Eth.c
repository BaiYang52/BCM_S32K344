/*==================================================================================================
*   Project              : RTD AUTOSAR 4.4
*   Platform             : CORTEXM
*   Peripheral           : GMAC
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
*   @file
*
*   @addtogroup ETH_DRIVER Ethernet Driver
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
#include "Eth.h"
#include "Eth_Internal.h"
#include "Eth_Ipw.h"

#include "EthIf_Cbk.h"

#include "Det.h"
#if (STD_ON == ETH_DEM_EVENT_DETECT)
#include "Dem.h"
#endif

#include "SchM_Eth.h"

/*==================================================================================================
*                              SOURCE FILE VERSION INFORMATION
==================================================================================================*/
#define ETH_VENDOR_ID_C                      43
#define ETH_MODULE_ID_C                      88
#define ETH_AR_RELEASE_MAJOR_VERSION_C       4
#define ETH_AR_RELEASE_MINOR_VERSION_C       4
#define ETH_AR_RELEASE_REVISION_VERSION_C    0
#define ETH_SW_MAJOR_VERSION_C               2
#define ETH_SW_MINOR_VERSION_C               0
#define ETH_SW_PATCH_VERSION_C               1

/*==================================================================================================
*                                     FILE VERSION CHECKS
==================================================================================================*/
/* Checks against Eth.h */
#if (ETH_VENDOR_ID_C != ETH_VENDOR_ID)
    #error "Eth.c and Eth.h have different vendor ids"
#endif
#if ((ETH_AR_RELEASE_MAJOR_VERSION_C    != ETH_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_AR_RELEASE_MINOR_VERSION_C    != ETH_AR_RELEASE_MINOR_VERSION) || \
     (ETH_AR_RELEASE_REVISION_VERSION_C != ETH_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Eth.c and Eth.h are different"
#endif
#if ((ETH_SW_MAJOR_VERSION_C != ETH_SW_MAJOR_VERSION) || \
     (ETH_SW_MINOR_VERSION_C != ETH_SW_MINOR_VERSION) || \
     (ETH_SW_PATCH_VERSION_C != ETH_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Eth.c and Eth.h are different"
#endif

/* Checks against Eth_Internal.h */
#if (ETH_VENDOR_ID_C != ETH_INTERNAL_VENDOR_ID)
    #error "Eth.c and Eth_Internal.h have different vendor ids"
#endif
#if ((ETH_AR_RELEASE_MAJOR_VERSION_C    != ETH_INTERNAL_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_AR_RELEASE_MINOR_VERSION_C    != ETH_INTERNAL_AR_RELEASE_MINOR_VERSION) || \
     (ETH_AR_RELEASE_REVISION_VERSION_C != ETH_INTERNAL_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Eth.c and Eth_Internal.h are different"
#endif
#if ((ETH_SW_MAJOR_VERSION_C != ETH_INTERNAL_SW_MAJOR_VERSION) || \
     (ETH_SW_MINOR_VERSION_C != ETH_INTERNAL_SW_MINOR_VERSION) || \
     (ETH_SW_PATCH_VERSION_C != ETH_INTERNAL_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Eth.c and Eth_Internal.h are different"
#endif

/* Checks against Eth_Ipw.h */
#if (ETH_VENDOR_ID_C != ETH_IPW_VENDOR_ID)
    #error "Eth.c and Eth_Ipw.h have different vendor ids"
#endif
#if ((ETH_AR_RELEASE_MAJOR_VERSION_C    != ETH_IPW_AR_RELEASE_MAJOR_VERSION) || \
     (ETH_AR_RELEASE_MINOR_VERSION_C    != ETH_IPW_AR_RELEASE_MINOR_VERSION) || \
     (ETH_AR_RELEASE_REVISION_VERSION_C != ETH_IPW_AR_RELEASE_REVISION_VERSION))
    #error "AUTOSAR Version Numbers of Eth.c and Eth_Ipw.h are different"
#endif
#if ((ETH_SW_MAJOR_VERSION_C != ETH_IPW_SW_MAJOR_VERSION) || \
     (ETH_SW_MINOR_VERSION_C != ETH_IPW_SW_MINOR_VERSION) || \
     (ETH_SW_PATCH_VERSION_C != ETH_IPW_SW_PATCH_VERSION) \
    )
    #error "Software Version Numbers of Eth.c and Eth_Ipw.h are different"
#endif

#ifndef DISABLE_MCAL_INTERMODULE_ASR_CHECK
    /* Checks against EthIf_Cbk.h */
    #if ((ETH_AR_RELEASE_MAJOR_VERSION_C != ETHIF_AR_RELEASE_MAJOR_VERSION) || \
         (ETH_AR_RELEASE_MINOR_VERSION_C != ETHIF_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Eth.c and EthIf_Cbk.h are different"
    #endif

    /* Checks against Det.h */
    #if ((ETH_AR_RELEASE_MAJOR_VERSION_C != DET_AR_RELEASE_MAJOR_VERSION) || \
         (ETH_AR_RELEASE_MINOR_VERSION_C != DET_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Eth.c and Det.h are different"
    #endif
#if (STD_ON == ETH_DEM_EVENT_DETECT)
    /* Checks against Dem.h */
    #if ((ETH_AR_RELEASE_MAJOR_VERSION_C != DEM_AR_RELEASE_MAJOR_VERSION) || \
         (ETH_AR_RELEASE_MINOR_VERSION_C != DEM_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Eth.c and Dem.h are different"
    #endif
#endif

    /* Checks against SchM_Eth.h */
    #if ((ETH_AR_RELEASE_MAJOR_VERSION_C != SCHM_ETH_AR_RELEASE_MAJOR_VERSION) || \
         (ETH_AR_RELEASE_MINOR_VERSION_C != SCHM_ETH_AR_RELEASE_MINOR_VERSION))
        #error "AUTOSAR Version Numbers of Eth.c and SchM_Eth.h are different"
    #endif
#endif

/*==================================================================================================
*                          LOCAL TYPEDEFS (STRUCTURES, UNIONS, ENUMS)
==================================================================================================*/
typedef struct Eth_axTxBufferIdxMapType
{
    uint8 FifoIdx;
    uint8* pu8BufferData;
    boolean bTxConfirmation;
    struct Eth_axTxBufferIdxMapType *pNextBuffer;

} Eth_axTxBufferIdxMapType;

typedef struct
{
    Eth_axTxBufferIdxMapType *pQueueHead;
    Eth_axTxBufferIdxMapType *pQueueTail;

} Eth_QueueInfo;

/*==================================================================================================
*                                       LOCAL MACROS
==================================================================================================*/
#define ETH_FRAME_MACDST_OFFSET     (0U)
#define ETH_FRAME_MACSRC_OFFSET     (6U)
#define ETH_FRAME_ETHTYPE_OFFSET    (12U)
#define ETH_FRAME_PAYLOAD_OFFSET    (14U)

#define ETH_FRAME_MACDST_LENGTH     (6U)
#define ETH_FRAME_MACSRC_LENGTH     (6U)
#define ETH_FRAME_ETHTYPE_LENGTH    (2U)   
#define ETH_FRAME_HEADER_LENGTH     (ETH_FRAME_MACDST_LENGTH + ETH_FRAME_MACSRC_LENGTH + ETH_FRAME_ETHTYPE_LENGTH)

#define ETH_BUFFER_INDEX_UNUSED     (0U)


#if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Little Endian - Convert to network order (Big Endian) */
    #define Eth_Local_Macro_Htons(u16Value)       (((((uint16)(u16Value) & 0xFFU)) << 8U) | (((uint16)(u16Value) & 0xFF00U) >> 8U))
#else
    /* Big Endian - Already in network order - Nothing to be done */
    #define Eth_Local_Macro_Htons(u16Value)       ((u16Value))
#endif


#if STD_ON == ETH_MULTICORE_SUPPORT
    #define Eth_GetCoreID()        (OsIf_GetCoreID())
#else
    #define Eth_GetCoreID()        (0U)
#endif

/*==================================================================================================
*                                      LOCAL CONSTANTS
==================================================================================================*/

/*==================================================================================================
*                                      LOCAL VARIABLES
==================================================================================================*/
#define ETH_START_SEC_VAR_CLEARED_UNSPECIFIED
#include "Eth_MemMap.h"


/**
* @brief          Pointers to the configuration structures.
* @details        Used to store the configuration pointers for later use.
*/
static const Eth_ConfigType *Eth_apxInternalCfg[ETH_MAX_COREIDX_SUPPORTED];

/**
* @brief         Map between buffer indices and transmission-specific information.
* @details       Used to realize the map function f(BufIdx) = (FifoIdx, Buffer, TxConfirmation)
*/
static Eth_axTxBufferIdxMapType Eth_axTxBufferIdxMap[ETH_MAX_CTRLIDX_SUPPORTED][ETH_MAX_TXFIFO_SUPPORTED * ETH_MAX_TXBUFF_SUPPORTED];

/**
* @brief         Queues of transmission requests for each Tx FIFO
* @details       Keeps track of the transmission requests enqueued by "Eth_Transmit".
*/
static Eth_QueueInfo Eth_axTransmissionRequests[ETH_MAX_CTRLIDX_SUPPORTED][ETH_MAX_TXFIFO_SUPPORTED];

/**
* @brief         Mode of the ETH controllers
* @details       Stores the last known mode of the ETH controller.
*                Used in "Eth_MainFunction" to track mode changes and report them back to EthIf.
*                Only "Eth_SetControllerMode" can trigger mode changes.
*/
static Eth_ModeType Eth_axCtrlMode[ETH_MAX_CTRLIDX_SUPPORTED];

#if STD_ON == ETH_DEV_ERROR_DETECT
/**
* @brief         State of the ETH controllers
* @details       Used to store state of the Ethernet controller driver i.e Eth
*                module. It is initialized to ETH_STATE_UNINIT
*/
static Eth_StateType Eth_axCtrlState[ETH_MAX_CTRLIDX_SUPPORTED];
#endif

#define ETH_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include "Eth_MemMap.h"

/*==================================================================================================
                                       GLOBAL CONSTANTS
==================================================================================================*/
#if (ETH_PRECOMPILE_SUPPORT == STD_ON)

#define ETH_START_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"


#if STD_ON == ETH_MULTICORE_SUPPORT
    extern const Eth_ConfigType * const Eth_apxPredefinedConfig[ETH_MAX_COREIDX_SUPPORTED];
#else
    extern const Eth_ConfigType Eth_xPredefinedConfig;
#endif


#define ETH_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include "Eth_MemMap.h"

#endif /* #if (ETH_PRECOMPILE_SUPPORT == STD_ON) */

/*==================================================================================================
*                                      GLOBAL VARIABLES
==================================================================================================*/

/*==================================================================================================
*                                   LOCAL FUNCTION PROTOTYPES
==================================================================================================*/
#define ETH_START_SEC_CODE
#include "Eth_MemMap.h"

static inline boolean Eth_Local_IsBroadcastFrame(const uint8 *PhyAddr);
static inline void Eth_Local_Memcpy(uint8 *Dst, const uint8 *Src, uint8 BytesNum);
#if (STD_ON == ETH_DEM_EVENT_DETECT)
static void Eth_Local_CheckDemStatus( \
                            const uint8 CtrlIdx, \
                            const uint32 DemConfig, \
                            const Eth_DemErrorIdType DemErrorId, \
                            const Dem_EventIdType DemId
                            );
#endif
#if STD_ON == ETH_DEV_ERROR_DETECT
static Std_ReturnType Eth_Local_CheckEthHLDEntry( \
                               uint32 CoreId, \
                               const uint8 CtrlIdx, \
                               uint8 EthServiceID \
                                );
#endif
static void Eth_Local_InitController( \
                                const uint8 CtrlIdx, \
                                const Eth_CtrlCfgType *Eth_Controller \
                                );
/*==================================================================================================
*                                       LOCAL FUNCTIONS
==================================================================================================*/
static inline boolean Eth_Local_IsBroadcastFrame(const uint8 *PhyAddr)
{
    return ((PhyAddr[0U] == 0xFFU) && (PhyAddr[1U] == 0xFFU) && (PhyAddr[2U] == 0xFFU) && (PhyAddr[3U] == 0xFFU) && (PhyAddr[4U] == 0xFFU) && (PhyAddr[5U] == 0xFFU));
}

static inline void Eth_Local_Memcpy(uint8 *Dst, const uint8 *Src, uint8 BytesNum)
{
    uint8 TempVar = BytesNum;
    
    /* Start copy data*/
    while (TempVar > 0U)
    {
        TempVar--;
        Dst[TempVar] = Src[TempVar];
    }
}
#if (STD_ON == ETH_DEM_EVENT_DETECT)
static void Eth_Local_CheckDemStatus( \
                            const uint8 CtrlIdx, \
                            const uint32 DemConfig, \
                            const Eth_DemErrorIdType DemErrorId, \
                            const Dem_EventIdType DemId
                            )
{
    /* Check Dem event ON or OFF */
    if ((uint32)STD_ON == DemConfig)
    {
        if (Eth_Ipw_ErrorDetected(CtrlIdx, DemErrorId))
        {
            (void)Dem_SetEventStatus(DemId, DEM_EVENT_STATUS_PREFAILED);
        }
        else
        {
            (void)Dem_SetEventStatus(DemId, DEM_EVENT_STATUS_PREPASSED);
        }
    }        
}
#endif
#if STD_ON == ETH_DEV_ERROR_DETECT
static Std_ReturnType Eth_Local_CheckEthHLDEntry( \
                                           uint32 CoreId, \
                                           const uint8 CtrlIdx, \
                                           uint8 EthServiceID \
                                            )
{
    Std_ReturnType CheckStatus = (Std_ReturnType)E_OK;
    
  #if STD_ON == ETH_MULTICORE_SUPPORT
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        CheckStatus = (Std_ReturnType)E_NOT_OK;
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, EthServiceID, ETH_E_UNINIT);
        
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        CheckStatus = (Std_ReturnType)E_NOT_OK;
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, EthServiceID, ETH_E_INV_CTRL_IDX);
    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        CheckStatus = (Std_ReturnType)E_NOT_OK;
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, EthServiceID, ETH_E_UNINIT);
    }
    else
    {
        /* Prevent misra */
    }
    
    return CheckStatus;
} 
#endif

static void Eth_Local_InitController( \
                                    const uint8 CtrlIdx, \
                                    const Eth_CtrlCfgType *Eth_Controller \
                                    )
{
    if (NULL_PTR != Eth_Controller) 
    {
    #if STD_ON == ETH_DEV_ERROR_DETECT
        Eth_axCtrlState[CtrlIdx] = ETH_STATE_UNINIT;
    #endif
        Eth_axCtrlMode[CtrlIdx]  = ETH_MODE_DOWN;

        /*  Check whether the controller is available */
        if ((boolean)TRUE == Eth_Ipw_CheckAccessToController(CtrlIdx))                    
        {
            if ((Std_ReturnType)E_OK == Eth_Ipw_ConfigureController(CtrlIdx, Eth_Controller->Eth_pIpwCtrlConfig, Eth_Controller->Eth_pClockConfig))
            {
                #if STD_ON == ETH_DEV_ERROR_DETECT
                    Eth_axCtrlState[CtrlIdx] = ETH_STATE_INIT; 
                #endif
                
                #if(STD_ON == ETH_DEM_EVENT_DETECT)
                    if((uint32)STD_ON == (uint32)(Eth_Controller->Eth_DemEventsList.ETH_E_ACCESS_Cfg.state))
                    {
                        (void)Dem_SetEventStatus((Dem_EventIdType)(Eth_Controller->Eth_DemEventsList.ETH_E_ACCESS_Cfg.id), DEM_EVENT_STATUS_PREPASSED);
                    }
                #endif
            }
       
        }
        else
        {
        #if(STD_ON == ETH_DEM_EVENT_DETECT)
            if((uint32)STD_ON == (uint32)(Eth_Controller->Eth_DemEventsList.ETH_E_ACCESS_Cfg.state))
            {
                (void)Dem_SetEventStatus((Dem_EventIdType)(Eth_Controller->Eth_DemEventsList.ETH_E_ACCESS_Cfg.id), DEM_EVENT_STATUS_PREFAILED);
            }
        #endif
        }
    }
}
/*==================================================================================================
*                                       GLOBAL FUNCTIONS
==================================================================================================*/

/*================================================================================================*/
void Eth_Init(const Eth_ConfigType *CfgPtr)
{
    const Eth_CtrlCfgType *Eth_Controller;
    uint32 CoreId = Eth_GetCoreID();
    uint8 CtrlIdx;

#if (STD_OFF == ETH_PRECOMPILE_SUPPORT)
  #if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR == CfgPtr)
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_INIT, ETH_E_INIT_FAILED);
    }
    #if (STD_ON == ETH_MULTICORE_SUPPORT)
    else if (CoreId != CfgPtr->PartitionCoreId)
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_INIT, ETH_E_PARAM_CONFIG);
    }
    #endif
    else
    {
  #endif
        Eth_apxInternalCfg[CoreId] = CfgPtr;
#else /* ETH_PRECOMPILE_SUPPORT == STD_ON */
  #if (STD_ON == ETH_DEV_ERROR_DETECT)
    if (NULL_PTR != CfgPtr)
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_INIT, ETH_E_INIT_FAILED);
    }
    #if (STD_ON == ETH_MULTICORE_SUPPORT)
    else if ((NULL_PTR == Eth_apxPredefinedConfig[CoreId]) || (CoreId != Eth_apxPredefinedConfig[CoreId]->PartitionCoreId))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_INIT, ETH_E_PARAM_CONFIG);
    }
    #endif
    else
    {
  #endif
      #if (STD_ON == ETH_MULTICORE_SUPPORT)
        Eth_apxInternalCfg[CoreId] = Eth_apxPredefinedConfig[CoreId];
      #else
        Eth_apxInternalCfg[CoreId] = &Eth_xPredefinedConfig;
      #endif
        (void)CfgPtr;
#endif /* (STD_ON != ETH_PRECOMPILE_SUPPORT) */

        for (CtrlIdx = 0U; CtrlIdx < ETH_MAX_CTRLIDX_SUPPORTED; ++CtrlIdx)
        { 
            Eth_Controller = Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx];
            
            Eth_Local_InitController(CtrlIdx, Eth_Controller);

        }
        #ifdef MCAL_ENABLE_FAULT_INJECTION
            MCAL_FAULT_INJECTION_POINT(ETH_CHANGE_STATE_FIP_1);
        #endif
#if (STD_ON == ETH_DEV_ERROR_DETECT)
    }
#endif
}

/*================================================================================================*/
Std_ReturnType Eth_SetControllerMode(uint8 CtrlIdx, Eth_ModeType CtrlMode)
{
    Eth_axTxBufferIdxMapType *Iter;
    Std_ReturnType FunctionSuccess = (Std_ReturnType)E_NOT_OK;
    const Eth_CtrlCfgType *Eth_Controller;
    uint32 CoreId = Eth_GetCoreID();
    uint8 FifoIdx;

#if STD_ON == ETH_DEV_ERROR_DETECT
    if (0U == Eth_Local_CheckEthHLDEntry(CoreId, CtrlIdx, ETH_SID_SETCONTROLLERMODE))
    {
#endif /* ETH_DEV_ERROR_DETECT  */
        Eth_Controller = Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx];

        /*  Check whether the controller is available */
        if((boolean)TRUE == Eth_Ipw_CheckAccessToController(CtrlIdx))
        {
            if(ETH_MODE_ACTIVE == CtrlMode)
            {
                FunctionSuccess = Eth_Ipw_EnableController(CtrlIdx);
            }
            else
            {
                /* Clear pending transmission requests */
                for (FifoIdx = 0U; FifoIdx < Eth_Controller->Eth_pEgressConfig->FifoCount; ++FifoIdx)
                {
                    for (Iter = Eth_axTransmissionRequests[CtrlIdx][FifoIdx].pQueueHead; Iter != NULL_PTR; Iter = Iter->pNextBuffer)
                    {
                        /* Clear buffer allocation */
                        Iter->pu8BufferData = NULL_PTR;
                    }

                    /* Mark queue as empty */
                    Eth_axTransmissionRequests[CtrlIdx][FifoIdx].pQueueHead = NULL_PTR;
                }
                
                FunctionSuccess = Eth_Ipw_DisableController(CtrlIdx);
            }

            /* Yes, the controller is available, report as passed */
            #if(STD_ON == ETH_DEM_EVENT_DETECT)
            if((uint32)STD_ON == (uint32)(Eth_Controller->Eth_DemEventsList.ETH_E_ACCESS_Cfg.state))
            {
            
                (void)Dem_SetEventStatus((Dem_EventIdType)(Eth_Controller->Eth_DemEventsList.ETH_E_ACCESS_Cfg.id), DEM_EVENT_STATUS_PREPASSED);
            }
            #endif
        }
        else
        {
            /* No, the controller is not available, report an error */
            #if(STD_ON == ETH_DEM_EVENT_DETECT)
            if((uint32)STD_ON == (uint32)(Eth_Controller->Eth_DemEventsList.ETH_E_ACCESS_Cfg.state))
            {
                (void)Dem_SetEventStatus((Dem_EventIdType)(Eth_Controller->Eth_DemEventsList.ETH_E_ACCESS_Cfg.id), DEM_EVENT_STATUS_PREFAILED);
            }
            #endif
        }
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */

    return FunctionSuccess;
}

/*================================================================================================*/
Std_ReturnType Eth_GetControllerMode( \
                            uint8 CtrlIdx, \
                            Eth_ModeType *CtrlModePtr \
                                    )
{
    Std_ReturnType FunctionSuccess = (Std_ReturnType)E_NOT_OK;

#if STD_ON == ETH_DEV_ERROR_DETECT
    uint32 CoreId = Eth_GetCoreID();

#if STD_ON == ETH_MULTICORE_SUPPORT
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
#else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
#endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETCONTROLLERMODE, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETCONTROLLERMODE, ETH_E_INV_CTRL_IDX);
    }
    else if (NULL_PTR == CtrlModePtr)
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETCONTROLLERMODE, ETH_E_PARAM_POINTER);
    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETCONTROLLERMODE, ETH_E_UNINIT);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */
        *CtrlModePtr = Eth_Ipw_GetControllerMode(CtrlIdx);
        FunctionSuccess = (Std_ReturnType)E_OK;             
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */

    return FunctionSuccess;
}
  
/*================================================================================================*/
void Eth_GetPhysAddr(uint8 CtrlIdx, uint8 *PhysAddrPtr)
{
#if STD_ON == ETH_DEV_ERROR_DETECT
    uint32 CoreId = Eth_GetCoreID();

  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETPHYSADDR, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETPHYSADDR, ETH_E_INV_CTRL_IDX);
    }
    else if (NULL_PTR == PhysAddrPtr)
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETPHYSADDR, ETH_E_PARAM_POINTER);

    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETPHYSADDR, ETH_E_UNINIT);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */
        Eth_Ipw_GetPhysicalAddress(CtrlIdx, PhysAddrPtr);
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */
}

/*================================================================================================*/
void Eth_SetPhysAddr(uint8 CtrlIdx, const uint8 *PhysAddrPtr)
{   
#if STD_ON == ETH_DEV_ERROR_DETECT
    uint32 CoreId = Eth_GetCoreID();

  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_SETPHYSADDR, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_SETPHYSADDR, ETH_E_INV_CTRL_IDX);
    }
    else if (NULL_PTR == PhysAddrPtr)
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_SETPHYSADDR, ETH_E_PARAM_POINTER);

    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_SETPHYSADDR, ETH_E_UNINIT);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */
        Eth_Ipw_SetPhysicalAddress(CtrlIdx, PhysAddrPtr);               
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */
}


#if STD_ON == ETH_UPDATE_PHYS_ADDR_FILTER_API
/*================================================================================================*/
Std_ReturnType Eth_UpdatePhysAddrFilter ( \
                                uint8 CtrlIdx, \
                                const uint8 *PhysAddrPtr, \
                                Eth_FilterActionType Action \
                                        )
{
    Std_ReturnType ReturnStatus = (Std_ReturnType)E_NOT_OK;

#if STD_ON == ETH_DEV_ERROR_DETECT
    uint32 CoreId = Eth_GetCoreID();

  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_UPDATEADDRFILTER, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_UPDATEADDRFILTER, ETH_E_INV_CTRL_IDX);
    }
    else if (NULL_PTR == PhysAddrPtr)
    {           
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_UPDATEADDRFILTER, ETH_E_PARAM_POINTER);

    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_UPDATEADDRFILTER, ETH_E_UNINIT);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */

        ReturnStatus = Eth_Ipw_UpdatePhysAddrFilter(CtrlIdx, PhysAddrPtr, Action);
       
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */

    return ReturnStatus;
}

#endif /* STD_ON == ETH_UPDATE_PHYS_ADDR_FILTER_API */


#if STD_ON == ETH_MDIO_CLAUSE22_API
/*================================================================================================*/
Std_ReturnType Eth_WriteMii ( \
                               uint8 CtrlIdx, \
                               uint8 TrcvIdx, \
                               uint8 RegIdx, \
                               uint16 RegVal \
                            )
{
    Std_ReturnType ReturnStatus = (Std_ReturnType)E_NOT_OK;
    const Eth_CtrlCfgType *Eth_Controller;
    uint32 CoreId = Eth_GetCoreID();

#if STD_ON == ETH_DEV_ERROR_DETECT
  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_WRITEMII, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_WRITEMII, ETH_E_INV_CTRL_IDX);
    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_WRITEMII, ETH_E_UNINIT);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */

        if ((boolean)TRUE == Eth_Ipw_WriteMii(CtrlIdx, TrcvIdx, RegIdx, RegVal))
        {
            Eth_Controller = Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx];
            Eth_Controller->Eth_EthTrcvDriverFunctionList.writeMiiIndicationFunction(CtrlIdx, TrcvIdx, RegIdx);
            ReturnStatus = (Std_ReturnType)E_OK;
        }           
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */

    return ReturnStatus;
}

/*================================================================================================*/
Std_ReturnType Eth_ReadMii  ( \
                            uint8 CtrlIdx, \
                            uint8 TrcvIdx, \
                            uint8 RegIdx, \
                            uint16 *RegValPtr \
                            )
{
    Std_ReturnType ReturnStatus = (Std_ReturnType)E_NOT_OK;
    const Eth_CtrlCfgType *Eth_Controller;
    uint32 CoreId = Eth_GetCoreID();

#if STD_ON == ETH_DEV_ERROR_DETECT
    if (0U == Eth_Local_CheckEthHLDEntry(CoreId, CtrlIdx, ETH_SID_READMII))
    {
        if (NULL_PTR == RegValPtr)
        {
            (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_READMII, ETH_E_PARAM_POINTER);
        }
        else
        {
#endif /* ETH_DEV_ERROR_DETECT  */
            if ((boolean)TRUE == Eth_Ipw_ReadMii(CtrlIdx, TrcvIdx, RegIdx, RegValPtr))
            {
                Eth_Controller = Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx];
                Eth_Controller->Eth_EthTrcvDriverFunctionList.readMiiIndicationFunction(CtrlIdx, TrcvIdx, RegIdx, *RegValPtr);
                ReturnStatus = (Std_ReturnType)E_OK;
            }         
#if STD_ON == ETH_DEV_ERROR_DETECT
        }
    }
#endif /* ETH_DEV_ERROR_DETECT  */

    return ReturnStatus;
}
#endif /* ETH_MDIO_CLAUSE22_API */

/*================================================================================================*/
#if STD_ON == ETH_MDIO_CLAUSE45_API
Std_ReturnType Eth_ReadMmd ( \
                              uint8 CtrlIdx,\
                              uint8 TrcvIdx, \
                              uint8 Mmd, \
                              uint16 RegAddress,\
                              uint16 *RegValPtr
                           )
{
    Std_ReturnType ReturnStatus = (Std_ReturnType)E_NOT_OK;

#if STD_ON == ETH_DEV_ERROR_DETECT   
    uint32 CoreId = Eth_GetCoreID();

  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_READMMD, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_READMMD, ETH_E_INV_CTRL_IDX);
    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_READMMD, ETH_E_UNINIT);
    }
    else if (NULL_PTR == RegValPtr)
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_READMMD, ETH_E_PARAM_POINTER);
    }    
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */
        if ((boolean)TRUE == Eth_Ipw_ReadMmd(CtrlIdx, TrcvIdx, Mmd, RegAddress, RegValPtr))
        {
            ReturnStatus = E_OK;
        }
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */  

    return ReturnStatus;
}

/*================================================================================================*/
Std_ReturnType Eth_WriteMmd ( \
                                uint8 CtrlIdx,\
                                uint8 TrcvIdx, \
                                uint8 Mmd, \
                                uint16 RegAddress,\
                                uint16 RegVal
                            )
{
    Std_ReturnType ReturnStatus = (Std_ReturnType)E_NOT_OK;
    
#if STD_ON == ETH_DEV_ERROR_DETECT    
    uint32 CoreId = Eth_GetCoreID();

  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_WRITEMMD, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_WRITEMMD, ETH_E_INV_CTRL_IDX);
    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_WRITEMMD, ETH_E_UNINIT);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */
        if ((boolean)TRUE == Eth_Ipw_WriteMmd(CtrlIdx, TrcvIdx, Mmd, RegAddress, RegVal))
        {   
            ReturnStatus = E_OK;
        }
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */  

    return ReturnStatus;
}
#endif /* ETH_MDIO_CLAUSE45_API */

#if STD_ON == ETH_GET_COUNTER_API
/*================================================================================================*/
Std_ReturnType Eth_GetCounterValues ( \
                                        uint8 CtrlIdx, \
                                        Eth_CounterType *CounterPtr \
                                    )
{
    Std_ReturnType ReturnStatus = (Std_ReturnType) E_NOT_OK;
 
#if STD_ON == ETH_DEV_ERROR_DETECT
    uint32 CoreId = Eth_GetCoreID();

  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETCOUNTERVALUE, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETCOUNTERVALUE, ETH_E_INV_CTRL_IDX);
    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETCOUNTERVALUE, ETH_E_UNINIT);
    }
    else if (NULL_PTR == CounterPtr)
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETCOUNTERVALUE, ETH_E_PARAM_POINTER);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */
        ReturnStatus = Eth_Ipw_GetCounterValues(CtrlIdx, CounterPtr);
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */

    return ReturnStatus;
}
#endif /* ETH_GET_COUNTER_API */

#if STD_ON == ETH_GET_RXSTATS_API
/*================================================================================================*/
Std_ReturnType Eth_GetRxStats ( \
                                uint8 CtrlIdx, \
                                Eth_RxStatsType *RxStats \
                              )
{
    Std_ReturnType ReturnStatus = (Std_ReturnType)E_NOT_OK;
 
#if STD_ON == ETH_DEV_ERROR_DETECT
    uint32 CoreId = Eth_GetCoreID();

  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETRXSTATS, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETRXSTATS, ETH_E_INV_CTRL_IDX);
    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETRXSTATS, ETH_E_UNINIT);
    }
    else if(NULL_PTR == RxStats)
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETRXSTATS, ETH_E_PARAM_POINTER);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */
        ReturnStatus = Eth_Ipw_GetRxStats(CtrlIdx, RxStats);
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */

    return ReturnStatus;
}
#endif /*ETH_GET_RXSTATS_API*/

#if STD_ON == ETH_GET_TXSTATS_API
/*================================================================================================*/
Std_ReturnType Eth_GetTxStats ( \
                                uint8 CtrlIdx, \
                                Eth_TxStatsType *TxStats \
                              )
{
    Std_ReturnType ReturnStatus = (Std_ReturnType) E_NOT_OK;

#if STD_ON == ETH_DEV_ERROR_DETECT
    uint32 CoreId = Eth_GetCoreID();

  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETTXSTATS, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETTXSTATS, ETH_E_INV_CTRL_IDX);
    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETTXSTATS, ETH_E_UNINIT);
    }
    else if(NULL_PTR == TxStats)
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETTXSTATS, ETH_E_PARAM_POINTER);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */
        ReturnStatus = Eth_Ipw_GetTxStats(CtrlIdx, TxStats);
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */

    return ReturnStatus;
}
#endif /*ETH_GET_TXSTATS_API*/

#if STD_ON == ETH_GET_TXERROR_COUNTER_API
/*================================================================================================*/
Std_ReturnType Eth_GetTxErrorCounterValues ( \
                                uint8 CtrlIdx, \
                                Eth_TxErrorCounterValuesType *TxErrorCounterValues \
                                           )
{
    Std_ReturnType ReturnStatus = (Std_ReturnType) E_NOT_OK;

#if STD_ON == ETH_DEV_ERROR_DETECT
    uint32 CoreId = Eth_GetCoreID();

  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETTXERRORCOUNTERVALUE, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETTXERRORCOUNTERVALUE, ETH_E_INV_CTRL_IDX);
    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETTXERRORCOUNTERVALUE, ETH_E_UNINIT);
    }
    else if (NULL_PTR == TxErrorCounterValues)
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETTXERRORCOUNTERVALUE, ETH_E_PARAM_POINTER);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */
        ReturnStatus = Eth_Ipw_GetTxErrorCounterValues(CtrlIdx, TxErrorCounterValues);
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */

    return ReturnStatus;
}
#endif /*ETH_GET_TXERROR_COUNTER_API*/

#if STD_ON == ETH_GLOBAL_TIME_API
/*================================================================================================*/
Std_ReturnType Eth_SetGlobalTime ( \
                    uint8 CtrlIdx, \
                    const Eth_TimeStampType * TimeStampPtr \
                        )
{
    Std_ReturnType ReturnStatus = (Std_ReturnType)E_NOT_OK;
    const Eth_CtrlCfgType *Eth_Controller;
    uint32 CoreId = Eth_GetCoreID();
    
#if STD_ON == ETH_DEV_ERROR_DETECT
  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_SETGLOBALTIME, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_SETGLOBALTIME, ETH_E_INV_CTRL_IDX);
    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_SETGLOBALTIME, ETH_E_UNINIT);
    }
    else if (NULL_PTR == TimeStampPtr)
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_SETGLOBALTIME, ETH_E_PARAM_POINTER);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */ 
        Eth_Controller = Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx];
        
        Eth_Ipw_SetGlobalTime(CtrlIdx, Eth_Controller->Eth_pClockConfig, TimeStampPtr);
        ReturnStatus = (Std_ReturnType)E_OK;
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */
    
    return ReturnStatus;
}
/*================================================================================================*/
Std_ReturnType Eth_SetCorrectionTime ( \
                            uint8 CtrlIdx, \
                            const Eth_TimeIntDiffType *TimeOffsetPtr, \
                            const Eth_RateRatioType *RateRatioPtr \
                                )
{
    Std_ReturnType ReturnStatus = (Std_ReturnType)E_NOT_OK;
    const Eth_CtrlCfgType *Eth_Controller;
    uint32 CoreId = Eth_GetCoreID();
    
#if STD_ON == ETH_DEV_ERROR_DETECT
  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_SETCORRECTIONTIME, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_SETCORRECTIONTIME, ETH_E_INV_CTRL_IDX);
    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_SETCORRECTIONTIME, ETH_E_UNINIT);
    }
    else if ((NULL_PTR == TimeOffsetPtr) || (NULL_PTR == RateRatioPtr))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_SETCORRECTIONTIME, ETH_E_PARAM_POINTER);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */
        Eth_Controller = Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx];

        ReturnStatus = Eth_Ipw_SetCorrectionTime(CtrlIdx, Eth_Controller->Eth_pClockConfig, TimeOffsetPtr, RateRatioPtr);
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */

    return ReturnStatus;
}

/*================================================================================================*/
Std_ReturnType Eth_GetCurrentTime  ( \
                                uint8 CtrlIdx, \
                                Eth_TimeStampQualType *TimeQualPtr, \
                                Eth_TimeStampType *TimeStampPtr \
                                   )
{
    Std_ReturnType ReturnStatus = (Std_ReturnType)E_NOT_OK;
    
#if STD_ON == ETH_DEV_ERROR_DETECT
    uint32 CoreId = Eth_GetCoreID();

  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETCURRENTTIME, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETCURRENTTIME, ETH_E_INV_CTRL_IDX);
    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETCURRENTTIME, ETH_E_UNINIT);
    }
    else if ((NULL_PTR == TimeQualPtr) || (NULL_PTR == TimeStampPtr))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETCURRENTTIME, ETH_E_PARAM_POINTER);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */

        *TimeQualPtr = ETH_UNCERTAIN;
        ReturnStatus = Eth_Ipw_GetCurrentTime(CtrlIdx, TimeQualPtr, TimeStampPtr);
        
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */

    return ReturnStatus;
}

/*================================================================================================*/
void Eth_EnableEgressTimeStamp  ( \
                                uint8 CtrlIdx, \
                                Eth_BufIdxType BufIdx \
                                )
{
#if STD_ON == ETH_DEV_ERROR_DETECT
    uint32 CoreId = Eth_GetCoreID();
    
  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_ENABLEEGRESSTIMESTAMP, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_ENABLEEGRESSTIMESTAMP, ETH_E_INV_CTRL_IDX);
    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_ENABLEEGRESSTIMESTAMP, ETH_E_UNINIT);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT  */    
        Eth_Ipw_EnableEgressTimeStamp(CtrlIdx, BufIdx);
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT  */
}

/*================================================================================================*/
Std_ReturnType Eth_GetEgressTimeStamp ( \
                                uint8 CtrlIdx, \
                                Eth_BufIdxType BufIdx, \
                                Eth_TimeStampQualType *TimeQualPtr, \
                                Eth_TimeStampType *TimeStampPtr \
                                      )
{
    Std_ReturnType ReturnStatus = (Std_ReturnType)E_NOT_OK;
    uint8 FifoIdx;
    
#if STD_ON == ETH_DEV_ERROR_DETECT
    uint32 CoreId = Eth_GetCoreID();
    
    if (0U == Eth_Local_CheckEthHLDEntry(CoreId, CtrlIdx, ETH_SID_GETEGRESSTIMESTAMP))
    {
        if ((NULL_PTR == TimeQualPtr) || (NULL_PTR == TimeStampPtr))
        {
            (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETEGRESSTIMESTAMP, ETH_E_PARAM_POINTER);
        }
        else
        {
#endif /* ETH_DEV_ERROR_DETECT  */      
            *TimeQualPtr = ETH_UNCERTAIN;
            FifoIdx    = Eth_axTxBufferIdxMap[CtrlIdx][BufIdx].FifoIdx;
            Eth_Ipw_GetEgressTimeStamp(CtrlIdx, FifoIdx, TimeQualPtr, TimeStampPtr);

            if (ETH_VALID == *TimeQualPtr)
            {
                ReturnStatus = (Std_ReturnType)E_OK;
            }
#if STD_ON == ETH_DEV_ERROR_DETECT
        }
    }
#endif /* ETH_DEV_ERROR_DETECT  */

    return ReturnStatus;

}

/*================================================================================================*/
Std_ReturnType Eth_GetIngressTimeStamp ( \
                                uint8 CtrlIdx, \
                                const Eth_DataType *DataPtr, \
                                Eth_TimeStampQualType* TimeQualPtr, \
                                Eth_TimeStampType* TimeStampPtr \
                                       )

{
    const Eth_CtrlCfgType *Eth_Controller = NULL_PTR;
    const uint8 *FrameData;
    uint8 FifoIdx;
    Std_ReturnType ReturnStatus = (Std_ReturnType)E_NOT_OK;
    uint32 CoreId = Eth_GetCoreID();
    
#if STD_ON == ETH_DEV_ERROR_DETECT
    if (0U == Eth_Local_CheckEthHLDEntry(CoreId, CtrlIdx, ETH_SID_GETINGRESSTIMESTAMP))
    {
        if ((NULL_PTR == DataPtr) || (NULL_PTR == TimeQualPtr) || (NULL_PTR == TimeStampPtr))
        {
            (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETINGRESSTIMESTAMP, ETH_E_PARAM_POINTER);
        }
        else
        {
#endif /* ETH_DEV_ERROR_DETECT  */
            /* First perform conversion from Eth_DataType to uint8 */
            FrameData = (const uint8 *)DataPtr;
            /* Move the pointer back to the position of the frame header (note the index sign) */
            FrameData = (const uint8 *)(FrameData - (const uint8 *)ETH_FRAME_PAYLOAD_OFFSET);
            
            Eth_Controller = Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx];
            
            *TimeQualPtr = ETH_UNCERTAIN;
            
            for ( \
                 FifoIdx = 0U; \
                 (FifoIdx < Eth_Controller->Eth_pIngressConfig->FifoCount) && (*TimeQualPtr == ETH_UNCERTAIN); \
                 ++FifoIdx \
                )
            {
                Eth_Ipw_GetIngressTimeStamp(CtrlIdx, FifoIdx, FrameData, TimeQualPtr, TimeStampPtr);
            }

            if (ETH_VALID == *TimeQualPtr)
            {
                ReturnStatus = (Std_ReturnType)E_OK;
            }
#if STD_ON == ETH_DEV_ERROR_DETECT
        }
    }
#endif /* ETH_DEV_ERROR_DETECT  */

    return ReturnStatus;
}
#endif /* ETH_GLOBAL_TIME_API */

/*================================================================================================*/
BufReq_ReturnType Eth_ProvideTxBuffer( \
                            uint8 CtrlIdx, \
                            uint8 Priority, \
                            Eth_BufIdxType *BufIdxPtr, \
                            uint8 **BufPtr, \
                            uint16 *LenBytePtr \
                                     )
{
    BufReq_ReturnType FunctionSuccess = BUFREQ_E_NOT_OK;
    const Eth_CtrlCfgType *Eth_Controller = NULL_PTR;
    uint32 CoreId = Eth_GetCoreID();
    uint8 FifoIdx;

    uint8 *FrameBuffer;
    uint16 FrameLength;

#if (STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API)
    uint8 *DataMgmt;
    uint16 ManagementInfoLength;
    uint16 DummyLength;
#endif

#if STD_ON == ETH_DEV_ERROR_DETECT
    if (0U == Eth_Local_CheckEthHLDEntry(CoreId, CtrlIdx, ETH_SID_PROVIDETXBUFFER))
    {
        if ((NULL_PTR == BufIdxPtr) || (NULL_PTR == BufPtr) || (NULL_PTR == LenBytePtr))
        {
            (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_PROVIDETXBUFFER, ETH_E_PARAM_POINTER);
        }
        else
        {
#endif /* ETH_DEV_ERROR_DETECT  */
            Eth_Controller = Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx];

            /* Convert VLAN PCP to Tx FIFO index */
            FifoIdx = Eth_Controller->Eth_pEgressConfig->VlanPcpToFifoIdx[Priority];

        #if (STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API)
            ManagementInfoLength = *LenBytePtr;
            Eth_Controller->Eth_EthSwtDriverFunctionList.TxAdaptBufferLengthFunction(LenBytePtr);
            ManagementInfoLength = *LenBytePtr - ManagementInfoLength;
        #endif

            /* Request the desired length */
            FrameLength     = *LenBytePtr + ETH_FRAME_HEADER_LENGTH;
            /* Warning: Function "Eth_Ipw_ProvideTxBuffer" is not reentrant */
            FunctionSuccess   = Eth_Ipw_ProvideTxBuffer(CtrlIdx, FifoIdx, BufIdxPtr, &FrameBuffer, &FrameLength);
            /* Report back the granted length */
            *LenBytePtr = FrameLength - ETH_FRAME_HEADER_LENGTH;

            if (BUFREQ_OK == FunctionSuccess)
            {
                Eth_axTxBufferIdxMap[CtrlIdx][*BufIdxPtr].FifoIdx     = FifoIdx;
                Eth_axTxBufferIdxMap[CtrlIdx][*BufIdxPtr].pu8BufferData = FrameBuffer;

            #if (STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API)
                DataMgmt    = &FrameBuffer[ETH_FRAME_ETHTYPE_OFFSET];
                DummyLength = *LenBytePtr - ManagementInfoLength;
                (void) Eth_Controller->Eth_EthSwtDriverFunctionList.TxPrepareFrameFunction(CtrlIdx, *BufIdxPtr, &DataMgmt, &DummyLength);
                *BufPtr = &DataMgmt[ETH_FRAME_ETHTYPE_LENGTH];
            #else
                *BufPtr = &FrameBuffer[ETH_FRAME_PAYLOAD_OFFSET];
            #endif
            }
#if STD_ON == ETH_DEV_ERROR_DETECT
        }
    }
#endif /* ETH_DEV_ERROR_DETECT  */

    return FunctionSuccess;
}

/*================================================================================================*/
Std_ReturnType Eth_TxTimeAwareShaperInit(uint8 CtrlIdx)
{
    const Eth_CtrlCfgType *Eth_Controller;
    uint32 CoreId = Eth_GetCoreID();
    Std_ReturnType FunctionSuccess = (Std_ReturnType)E_NOT_OK;
    
#if STD_ON == ETH_DEV_ERROR_DETECT
  #if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId >= ETH_MAX_COREIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]))
  #else
    if (NULL_PTR == Eth_apxInternalCfg[CoreId])
  #endif
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_TXTIMEAWARESHAPER, ETH_E_UNINIT);
    }
    else if ((CtrlIdx >= ETH_MAX_CTRLIDX_SUPPORTED) || (NULL_PTR == Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]))
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_TXTIMEAWARESHAPER, ETH_E_INV_CTRL_IDX);
    }
    else if (ETH_STATE_INIT != Eth_axCtrlState[CtrlIdx])
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_TXTIMEAWARESHAPER, ETH_E_UNINIT);
    }
    else
    {
#endif
        Eth_Controller = Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx];
        FunctionSuccess = Eth_Ipw_TxTimeAwareShaperInit(CtrlIdx, Eth_Controller->Eth_pIpwCtrlConfig);
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif
    
    return FunctionSuccess;
}

/*================================================================================================*/
Std_ReturnType Eth_Transmit(
                           uint8 CtrlIdx,
                           Eth_BufIdxType BufIdx,
                           Eth_FrameType FrameType,
                           boolean TxConfirmation,
                           uint16 LenByte,
                           const uint8 *PhysAddrPtr
                            )
{
    Std_ReturnType FunctionSuccess = (Std_ReturnType)E_NOT_OK;
    uint8 FifoIdx;
    uint8 PhysSrcAddr[ETH_FRAME_MACSRC_LENGTH];
    uint8 *FrameBuffer;
    uint16 FrameLength;
    uint16 ChFrameType;

#if (STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API)
    uint8 *DataMgmt;
    const Eth_CtrlCfgType *Eth_Controller;
#endif

#if (STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API) || (STD_ON == ETH_DEV_ERROR_DETECT)
    uint32 CoreId = Eth_GetCoreID();
#endif

#if STD_ON == ETH_DEV_ERROR_DETECT   
    if ((Std_ReturnType)E_OK == Eth_Local_CheckEthHLDEntry(CoreId, CtrlIdx, ETH_SID_TRANSMIT))
    {
        if (ETH_MODE_ACTIVE != Eth_Ipw_GetControllerMode(CtrlIdx))
        {
            FunctionSuccess  = (Std_ReturnType)E_NOT_OK;
            (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_TRANSMIT, ETH_E_INV_MODE);
        }
        else if (NULL_PTR == PhysAddrPtr)
        {
            FunctionSuccess  = (Std_ReturnType)E_NOT_OK;
            (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_TRANSMIT, ETH_E_PARAM_POINTER);
        }
        else if
            (
                (BufIdx >= Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]->Eth_pEgressConfig->BufferCount) ||
                (NULL_PTR == Eth_axTxBufferIdxMap[CtrlIdx][BufIdx].pu8BufferData)
            )
        {
            FunctionSuccess  = (Std_ReturnType)E_NOT_OK;
            (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_TRANSMIT, ETH_E_INV_PARAM);
        }
        else
        {
#endif /* ETH_DEV_ERROR_DETECT  */
            /* Convert buffer index to Tx FIFO index */
            FifoIdx = Eth_axTxBufferIdxMap[CtrlIdx][BufIdx].FifoIdx;

            /* Convert buffer index to buffer address pointer */
            FrameBuffer = Eth_axTxBufferIdxMap[CtrlIdx][BufIdx].pu8BufferData;

            /* Configure Tx confirmation status */
            Eth_axTxBufferIdxMap[CtrlIdx][BufIdx].bTxConfirmation = TxConfirmation;

            /* Insert ethernet header (DST + SRC + TYPE) into the buffer */
            Eth_Local_Memcpy(&FrameBuffer[ETH_FRAME_MACDST_OFFSET], PhysAddrPtr, ETH_FRAME_MACDST_LENGTH);

            Eth_Ipw_GetPhysicalAddress(CtrlIdx, PhysSrcAddr);
            Eth_Local_Memcpy(&FrameBuffer[ETH_FRAME_MACSRC_OFFSET], PhysSrcAddr, ETH_FRAME_MACSRC_LENGTH);

            ChFrameType = Eth_Local_Macro_Htons(FrameType);
            Eth_Local_Memcpy(&FrameBuffer[ETH_FRAME_ETHTYPE_OFFSET], (uint8*) &ChFrameType, ETH_FRAME_ETHTYPE_LENGTH);

            /* Enqueue transmission request in the given Tx FIFO */
            if (NULL_PTR == Eth_axTransmissionRequests[CtrlIdx][FifoIdx].pQueueHead)
            {
                Eth_axTransmissionRequests[CtrlIdx][FifoIdx].pQueueHead = &Eth_axTxBufferIdxMap[CtrlIdx][BufIdx];
                Eth_axTransmissionRequests[CtrlIdx][FifoIdx].pQueueTail = &Eth_axTxBufferIdxMap[CtrlIdx][BufIdx];
            }
            else
            {
                Eth_axTransmissionRequests[CtrlIdx][FifoIdx].pQueueTail->pNextBuffer = &Eth_axTxBufferIdxMap[CtrlIdx][BufIdx];
                Eth_axTransmissionRequests[CtrlIdx][FifoIdx].pQueueTail = &Eth_axTxBufferIdxMap[CtrlIdx][BufIdx];
            }
            
            Eth_axTransmissionRequests[CtrlIdx][FifoIdx].pQueueTail->pNextBuffer = NULL_PTR;
            
        #if (STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API)
            DataMgmt = &FrameBuffer[ETH_FRAME_ETHTYPE_OFFSET];

            Eth_Controller = Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx];
            
            if ((Std_ReturnType)E_OK == Eth_Controller->Eth_EthSwtDriverFunctionList.TxProcessFrameFunction(CtrlIdx, BufIdx, &DataMgmt, &LenByte))
            {
                if ((Std_ReturnType)E_OK == Eth_Controller->Eth_EthSwtDriverFunctionList.TxFinishedIndicationFunction(CtrlIdx, BufIdx))
                {
        #endif
                    FrameLength = LenByte + ETH_FRAME_HEADER_LENGTH;
                    FunctionSuccess = Eth_Ipw_Transmit(CtrlIdx, FifoIdx, FrameBuffer, FrameLength);
        #if (STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API)
                }
            }
        #endif
        
#if STD_ON == ETH_DEV_ERROR_DETECT
        }
    }
#endif /* ETH_DEV_ERROR_DETECT  */
    
    return FunctionSuccess; 
}


/*================================================================================================*/
void Eth_ReportTransmission(
                                const uint8 CtrlIdx, \
                                const uint8 FifoIdx \
                           )
{
    Eth_axTxBufferIdxMapType *Iter;
    Eth_BufIdxType BufIdx;
    boolean FrameHasError;
    Std_ReturnType TxResult;
    Std_ReturnType TxStatus = (Std_ReturnType)E_OK;
    
    if (FifoIdx < ETH_MAX_TXFIFO_SUPPORTED)
    {
        /* Check all buffers previously queued for transmission */
        Iter = Eth_axTransmissionRequests[CtrlIdx][FifoIdx].pQueueHead;
        
        while ((Iter != NULL_PTR) && (TxStatus == (Std_ReturnType)E_OK))
        {
            TxStatus = Eth_Ipw_GetTransmissionStatus(CtrlIdx, FifoIdx, Iter->pu8BufferData, &FrameHasError);

            if ((Std_ReturnType)E_OK == TxStatus)
            {
                if (Iter->bTxConfirmation)
                {
                    BufIdx   = (Eth_BufIdxType)((sint32)(Iter - Eth_axTxBufferIdxMap[CtrlIdx]));
                    TxResult = (((boolean)FALSE == FrameHasError)? ((Std_ReturnType)E_OK) : ((Std_ReturnType)E_NOT_OK));
                    EthIf_TxConfirmation(CtrlIdx, BufIdx, TxResult);
                }

                /* Clear buffer allocation and dequeue transmission request */
                Iter->pu8BufferData = NULL_PTR;
                Eth_axTransmissionRequests[CtrlIdx][FifoIdx].pQueueHead = Iter->pNextBuffer;
            }
            
            Iter = Iter->pNextBuffer;
        }
    }
}

/*================================================================================================*/
void Eth_Receive(uint8 CtrlIdx, \
                 uint8 FifoIdx,  \
                 Eth_RxStatusType *RxStatusPtr   \
                )   
{
    uint8 *FrameData;
    uint16 FrameLength;
    boolean FrameHasError;

    Eth_FrameType FrameType;
    boolean IsBroadcast;
    const uint8 *MacSrcAddr;
    const Eth_DataType *Payload;
    uint16 PayloadLength;

#if (STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API)
    boolean IsMgmtFrameOnly;
    uint8 *DataMgmt;
    const Eth_CtrlCfgType *Eth_Controller;
#endif

#if (STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API) || (STD_ON == ETH_DEV_ERROR_DETECT)
    uint32 CoreId = Eth_GetCoreID();
#endif

#if STD_ON == ETH_DEV_ERROR_DETECT
    if (0U == Eth_Local_CheckEthHLDEntry(CoreId, CtrlIdx, ETH_SID_RECEIVE))
    {
        if(ETH_MODE_ACTIVE != Eth_Ipw_GetControllerMode(CtrlIdx))
        {
            (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_RECEIVE, ETH_E_INV_MODE);
        }
        else
        {
#endif /* ETH_DEV_ERROR_DETECT  */
            *RxStatusPtr = Eth_Ipw_ReceiveFrame(CtrlIdx, FifoIdx, &FrameData, &FrameLength, &FrameHasError);

            if ((ETH_NOT_RECEIVED != *RxStatusPtr) && ((boolean)FALSE == FrameHasError))
            {
                /* Get Frame Type */
                FrameType  = FrameData[ETH_FRAME_ETHTYPE_OFFSET + 1U];
                FrameType |= ((uint16)FrameData[ETH_FRAME_ETHTYPE_OFFSET] << 8U);

                /* Check Broadcast */
                IsBroadcast = Eth_Local_IsBroadcastFrame(&FrameData[ETH_FRAME_MACDST_OFFSET]);

                /* Get MAC Source Address */
                MacSrcAddr = &FrameData[ETH_FRAME_MACSRC_OFFSET];

                /* Get Payload */
                Payload = &FrameData[ETH_FRAME_PAYLOAD_OFFSET];

            #if (STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API)
                DataMgmt = &FrameData[ETH_FRAME_ETHTYPE_OFFSET];
                Eth_Controller = Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx];
                /* There shouldn't be a buffer index concept for RX side */
                (void)Eth_Controller->Eth_EthSwtDriverFunctionList.RxProcessFrameFunction(CtrlIdx, ETH_BUFFER_INDEX_UNUSED, &DataMgmt, &FrameLength, &IsMgmtFrameOnly);
                if ((boolean)FALSE == IsMgmtFrameOnly)
                {
            #endif
                    /* Get Payload Length */
                    /* We assume CRC and padding have been stripped so that FrameLength = DST + SRC + (VLAN_TAG) + ETHTYPE + PAYLOAD */    
                    PayloadLength = FrameLength - ETH_FRAME_HEADER_LENGTH;

                    EthIf_RxIndication(CtrlIdx, FrameType, IsBroadcast, MacSrcAddr, Payload, PayloadLength);
            #if (STD_ON == ETH_SWT_MANAGEMENT_SUPPORT_API)
                }

                (void)Eth_Controller->Eth_EthSwtDriverFunctionList.RxFinishedIndicationFunction(CtrlIdx, ETH_BUFFER_INDEX_UNUSED);
            #endif

                Eth_Ipw_ReleaseResources(CtrlIdx, FifoIdx);
            }
#if STD_ON == ETH_DEV_ERROR_DETECT
        }
    }
#endif /* ETH_DEV_ERROR_DETECT  */
}

/*================================================================================================*/
void Eth_TxConfirmation(uint8 CtrlIdx)
{
    const Eth_CtrlCfgType *Eth_Controller = NULL_PTR;
    uint32 CoreId = Eth_GetCoreID();
    uint8 FifoIdx;

#if STD_ON == ETH_DEV_ERROR_DETECT
    if (0U == Eth_Local_CheckEthHLDEntry(CoreId, CtrlIdx, ETH_SID_TXCONFIRMATION))
    {
        if (ETH_MODE_ACTIVE != Eth_Ipw_GetControllerMode(CtrlIdx))
        {
            (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_TXCONFIRMATION, ETH_E_INV_MODE);
        }
        else
        {
#endif /* ETH_DEV_ERROR_DETECT  */
            Eth_Controller = Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx];

            for (FifoIdx = 0U; FifoIdx < Eth_Controller->Eth_pEgressConfig->FifoCount; ++FifoIdx)
            {
                Eth_ReportTransmission(CtrlIdx, FifoIdx);
            }         
#if STD_ON == ETH_DEV_ERROR_DETECT
        }
    }
#endif /* ETH_DEV_ERROR_DETECT  */
}

#if STD_ON == ETH_VERSION_INFO_API
/*================================================================================================*/
void Eth_GetVersionInfo(Std_VersionInfoType *VersionInfoPtr) 
{
#if STD_ON == ETH_DEV_ERROR_DETECT
    if (NULL_PTR == VersionInfoPtr)
    {
        (void)Det_ReportError(ETH_MODULE_ID, ETH_DRIVER_INSTANCE, ETH_SID_GETVERSIONINFO, ETH_E_PARAM_POINTER);
    }
    else
    {
#endif /* ETH_DEV_ERROR_DETECT */
        VersionInfoPtr->moduleID = (uint16)ETH_MODULE_ID;
        VersionInfoPtr->vendorID = (uint16)ETH_VENDOR_ID;
        VersionInfoPtr->sw_major_version = (uint8)ETH_SW_MAJOR_VERSION;
        VersionInfoPtr->sw_minor_version = (uint8)ETH_SW_MINOR_VERSION;
        VersionInfoPtr->sw_patch_version = (uint8)ETH_SW_PATCH_VERSION;
#if STD_ON == ETH_DEV_ERROR_DETECT
    }
#endif /* ETH_DEV_ERROR_DETECT */
}
#endif /* ETH_VERSION_INFO_API */

/*================================================================================================*/
/**
* @brief         The function checks for controller errors and lost frames. Used for polling state changes.
* @api
* implements    Eth_MainFunction_Activity 
*/
void Eth_MainFunction(void)
{
    Eth_ModeType CurrentMode;
    uint8 CtrlIdx;
    uint32 CoreId = Eth_GetCoreID();
#if (STD_ON == ETH_DEM_EVENT_DETECT)
    const Eth_DemEventsType *Eth_CtrlDemEvents;
#endif
   
#if (STD_ON == ETH_MULTICORE_SUPPORT)
    if ((CoreId < ETH_MAX_COREIDX_SUPPORTED) && (NULL_PTR != Eth_apxInternalCfg[CoreId]))
#else
    if (NULL_PTR != Eth_apxInternalCfg[CoreId])
#endif
    {
        for (CtrlIdx = 0U; CtrlIdx < ETH_MAX_COREIDX_SUPPORTED; ++CtrlIdx)
        {
            if (NULL_PTR != Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx])
            {
                CurrentMode = Eth_Ipw_GetControllerMode(CtrlIdx);
                if (CurrentMode != Eth_axCtrlMode[CtrlIdx])
                {
                    Eth_axCtrlMode[CtrlIdx] = CurrentMode;
                    EthIf_CtrlModeIndication(CtrlIdx, CurrentMode);
                }

#if (STD_ON == ETH_DEM_EVENT_DETECT)
                Eth_CtrlDemEvents = &(Eth_apxInternalCfg[CoreId]->Eth_apCtrlConfig[CtrlIdx]->Eth_DemEventsList);

                if ((boolean)FALSE == Eth_Ipw_CheckAccessToController(CtrlIdx))
                {
                    if ((uint32)STD_ON == (uint32)(Eth_CtrlDemEvents->ETH_E_ACCESS_Cfg.state))
                    {
                        (void)Dem_SetEventStatus((Dem_EventIdType)(Eth_CtrlDemEvents->ETH_E_ACCESS_Cfg.id), DEM_EVENT_STATUS_PREFAILED);
                    }
                }
                else
                {
                    if ((uint32)STD_ON == (uint32)(Eth_CtrlDemEvents->ETH_E_ACCESS_Cfg.state))
                    {
                       (void)Dem_SetEventStatus((Dem_EventIdType)(Eth_CtrlDemEvents->ETH_E_ACCESS_Cfg.id), DEM_EVENT_STATUS_PREPASSED);
                    }
                    
                    Eth_Local_CheckDemStatus(CtrlIdx, (uint32)Eth_CtrlDemEvents->ETH_E_RX_FRAMES_LOST_Cfg.state, ETH_E_RX_FRAMES_LOST, (Dem_EventIdType)(Eth_CtrlDemEvents->ETH_E_RX_FRAMES_LOST_Cfg.id));
                    
                    Eth_Local_CheckDemStatus(CtrlIdx, (uint32)Eth_CtrlDemEvents->ETH_E_CRC_Cfg.state, ETH_E_CRC, (Dem_EventIdType)(Eth_CtrlDemEvents->ETH_E_CRC_Cfg.id));
                    
                    Eth_Local_CheckDemStatus(CtrlIdx, (uint32)Eth_CtrlDemEvents->ETH_E_UNDERSIZEFRAME_Cfg.state, ETH_E_UNDERSIZEFRAME, (Dem_EventIdType)(Eth_CtrlDemEvents->ETH_E_UNDERSIZEFRAME_Cfg.id));
                    
                    Eth_Local_CheckDemStatus(CtrlIdx, (uint32)Eth_CtrlDemEvents->ETH_E_OVERSIZEFRAME_Cfg.state, ETH_E_OVERSIZEFRAME, (Dem_EventIdType)(Eth_CtrlDemEvents->ETH_E_OVERSIZEFRAME_Cfg.id));
                    
                    Eth_Local_CheckDemStatus(CtrlIdx, (uint32)Eth_CtrlDemEvents->ETH_E_ALIGNMENT_Cfg.state, ETH_E_ALIGNMENT, (Dem_EventIdType)(Eth_CtrlDemEvents->ETH_E_ALIGNMENT_Cfg.id));
                    
                    Eth_Local_CheckDemStatus(CtrlIdx, (uint32)Eth_CtrlDemEvents->ETH_E_SINGLECOLLISION_Cfg.state, ETH_E_SINGLECOLLISION, (Dem_EventIdType)(Eth_CtrlDemEvents->ETH_E_SINGLECOLLISION_Cfg.id));
                    
                    Eth_Local_CheckDemStatus(CtrlIdx, (uint32)Eth_CtrlDemEvents->ETH_E_MULTIPLECOLLISION_Cfg.state, ETH_E_MULTIPLECOLLISION, (Dem_EventIdType)(Eth_CtrlDemEvents->ETH_E_MULTIPLECOLLISION_Cfg.id));
                    
                    Eth_Local_CheckDemStatus(CtrlIdx, (uint32)Eth_CtrlDemEvents->ETH_E_LATECOLLISION_Cfg.state, ETH_E_LATECOLLISION, (Dem_EventIdType)(Eth_CtrlDemEvents->ETH_E_LATECOLLISION_Cfg.id));
                }
#endif /* ETH_DEM_EVENT_DETECT */
            }
        }
    }
}

#define ETH_STOP_SEC_CODE
#include "Eth_MemMap.h"


#ifdef __cplusplus
}
#endif

/** @} */
