/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  Com_Notifications.c
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_NOTIFICATIONS_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_Notifications.h"
#include "Com_RxProcessing.h"
#include "SchM_Com.h"

/**********************************************************************************************************************
  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION MACROS
**********************************************************************************************************************/
#if !defined (COM_LOCAL_INLINE)
# define COM_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

#if (COM_RXIMMEDIATEFCTPTRCACHE == STD_ON)
/**********************************************************************************************************************
  Com_CacheOrCallRxImmediateCbkFctPtr
**********************************************************************************************************************/
/*! \brief              This function caches the the provided index of function pointer
 *                      in the Rx Immediate function pointer cache. If the cache is full,
 *                      all cached function pointers are called and the cache is cleared.
 *   \details           -
 *   \pre               -
 *   \param[in]         cbkFuncPtrIdx       Index of function pointer that should be cached or called.
 *   \param[in,out]     fctPtrCacheStrctPtr Pointer to struct which contains all information about the cache (size, index, location)
 *   \context           TASK|ISR2
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CacheOrCallRxImmediateCbkFctPtr(Com_RxCbkFuncPtrIterType cbkFuncPtrIdx, P2VAR(FctPtrCacheStrct, AUTOMATIC, COM_APPL_DATA) fctPtrCacheStrctPtr);
#endif

#if (COM_RXDEFERREDFCTPTRCACHE == STD_ON)
/**********************************************************************************************************************
  Com_CacheOrCallRxDeferredCbkFctPtr
**********************************************************************************************************************/
/*! \brief              This function caches the the provided index of function pointer
 *                      in the Rx Deferred function pointer cache. If the cache is full,
 *                      all cached function pointers are called and the cache is cleared.
 *   \details           -
 *   \pre               -
 *   \param[in]         idx                 index of the indirection from the main function rx to the ISRLock counter
 *   \param[in]         cbkFuncPtrIdx       Index of function pointer that should be cached or called.
 *   \param[in,out]     fctPtrCacheStrctPtr Pointer to struct which contains all information about the cache (size, index, location)
 *   \context           TASK|ISR2
 *   \synchronous       TRUE
 *   \reentrant         FALSE
 **********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CacheOrCallRxDeferredCbkFctPtr(Com_RxMainFunctionProcessingISRLockCounterIdxOfMainFunctionRxStructType idx, Com_RxCbkFuncPtrIterType cbkFuncPtrIdx,
                                                        P2VAR(FctPtrCacheStrct, AUTOMATIC, COM_APPL_DATA) fctPtrCacheStrctPtr);
#endif


/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

#if (COM_RXIMMEDIATEFCTPTRCACHE == STD_ON)
/**********************************************************************************************************************
  Com_CacheOrCallRxImmediateCbkFctPtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_CacheOrCallRxImmediateCbkFctPtr(Com_RxCbkFuncPtrIterType cbkFuncPtrIdx, P2VAR(FctPtrCacheStrct, AUTOMATIC, COM_APPL_DATA) fctPtrCacheStrctPtr)
{
  /* #10 If RxImmediateFctPtrCache is not full, cache the function pointer index. */
  if(fctPtrCacheStrctPtr->cacheIndex < fctPtrCacheStrctPtr->cacheSize)  /* COV_COM_CACHEORCALL_IMMEDIATE */
  {
    fctPtrCacheStrctPtr->ptrToCache[fctPtrCacheStrctPtr->cacheIndex] = (uint32) cbkFuncPtrIdx;  /* SBSW_COM_FCTPTR_CACHE_PTR_3 */
    fctPtrCacheStrctPtr->cacheIndex = fctPtrCacheStrctPtr->cacheIndex + 1u;     /* SBSW_COM_FCTPTR_CACHE_PTR_2 */
  }
  /* #20 Otherwise, call all cached function callbacks. */
  else
  {
    SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
    Com_GetRxCbkFuncPtr((Com_RxCbkFuncPtrIterType) cbkFuncPtrIdx) ();   /* SBSW_COM_FCT_PTR_CALL_CSL03 */
    Com_ProcessRxFctPtrCache(fctPtrCacheStrctPtr);      /* SBSW_COM_FCTPTR_CACHE_PTR */
    SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
  }
}
#endif

#if (COM_RXDEFERREDFCTPTRCACHE == STD_ON)
/**********************************************************************************************************************
  Com_CacheOrCallRxDeferredCbkFctPtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CacheOrCallRxDeferredCbkFctPtr(Com_RxMainFunctionProcessingISRLockCounterIdxOfMainFunctionRxStructType idx, Com_RxCbkFuncPtrIterType cbkFuncPtrIdx,
                                                        P2VAR(FctPtrCacheStrct, AUTOMATIC, COM_APPL_DATA) fctPtrCacheStrctPtr)
{
  /* #10 If RxDeferredFctPtrCache is not full, cache the function pointer index. */
  if(fctPtrCacheStrctPtr->cacheIndex < fctPtrCacheStrctPtr->cacheSize)
  {
    fctPtrCacheStrctPtr->ptrToCache[fctPtrCacheStrctPtr->cacheIndex] = (uint32) cbkFuncPtrIdx;  /* SBSW_COM_FCTPTR_CACHE_PTR_3 */
    fctPtrCacheStrctPtr->cacheIndex = fctPtrCacheStrctPtr->cacheIndex + 1u;     /* SBSW_COM_FCTPTR_CACHE_PTR_2 */
  }
  /* #20 Otherwise, call all cached function callbacks. */
  else
  {
    Com_RxProcessing_ExitExclusiveArea_Deferred(idx);
    Com_GetRxCbkFuncPtr((Com_RxCbkFuncPtrIterType) cbkFuncPtrIdx) ();   /* SBSW_COM_FCT_PTR_CALL_CSL03 */
    Com_ProcessRxFctPtrCache(fctPtrCacheStrctPtr);      /* SBSW_COM_FCTPTR_CACHE_PTR */
    Com_RxProcessing_EnterExclusiveArea_Deferred();
  }
}
#endif

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

#if ((COM_CBKTXACKDEFFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)||(COM_CBKTXERRFUNCPTRINDUSEDOFTXPDUINFO == STD_ON))
/**********************************************************************************************************************
  Com_TxNHdlr_InitTxIpdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxNHdlr_InitTxIpdu(Com_TxPduInfoIterType ComPduId)
{
# if (COM_CBKTXACKDEFFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)
  /* #10 Set HandleTxPduDeferred flag to its initial value */
  Com_SetHandleTxPduDeferred(ComPduId, FALSE);  /* SBSW_COM_IDXPARAM_INTERNALFCT_CSL02 */
# endif
# if (COM_CBKTXERRFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)
  /* #20 Set WaitingForConfirmation flag to its initial value */
  Com_SetWaitingForConfirmation(ComPduId, FALSE);       /* SBSW_COM_IDXPARAM_INTERNALFCT_CSL02 */
# endif
}
#endif

#if (COM_CBKTXACKDEFFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_TxNHdlr_MainFunctionTx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxNHdlr_MainFunctionTx(Com_MainFunctionTxStructIterType mainFunctionId)
{
  Com_TxPduInfoIterType ComPduId;
  Com_TxPduInfoOfMainFunctionTxIndIterType txPduInfoIndIdx = Com_GetTxPduInfoOfMainFunctionTxIndStartIdxOfMainFunctionTxStruct(mainFunctionId);

  /* #10 Iterate over all messages and check if a deferred transmission is pending for this message */
  for(; txPduInfoIndIdx < Com_GetTxPduInfoOfMainFunctionTxIndEndIdxOfMainFunctionTxStruct(mainFunctionId); txPduInfoIndIdx++)
  {
    ComPduId = Com_GetTxPduInfoOfMainFunctionTxInd(txPduInfoIndIdx);
    /* #20 If the Handle Deferred flag is set */
    if(Com_IsHandleTxPduDeferred(ComPduId))
    {
      Com_SetHandleTxPduDeferred(ComPduId, FALSE);    /* SBSW_COM_CSL02 */
      {
        /* #30 Call all related notification function of this ComIPdu */
        Com_CbkTxAckDefFuncPtrIndEndIdxOfTxPduInfoType notiIndIndex = Com_GetCbkTxAckDefFuncPtrIndStartIdxOfTxPduInfo(ComPduId);
        for(; notiIndIndex < Com_GetCbkTxAckDefFuncPtrIndEndIdxOfTxPduInfo(ComPduId); notiIndIndex++)
        {
          Com_GetCbkTxAckDefFuncPtr(Com_GetCbkTxAckDefFuncPtrInd(notiIndIndex)) ();   /* SBSW_COM_FCT_PTR_CALL_CSL03 */
        }
      }
    }
  }
}
#endif

#if (COM_CBKTXERRFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_TxNHdlr_StopTxIpdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxNHdlr_StopTxIpdu(Com_TxPduInfoIterType ComPduId)
{
  /* #10 Call all related error notification function of this ComIPdu */
  Com_CbkTxErrFuncPtrIndEndIdxOfTxPduInfoType notiIndIndex = Com_GetCbkTxErrFuncPtrIndStartIdxOfTxPduInfo(ComPduId);
  for(; notiIndIndex < Com_GetCbkTxErrFuncPtrIndEndIdxOfTxPduInfo(ComPduId); notiIndIndex++)
  {
    Com_GetCbkTxErrFuncPtr(Com_GetCbkTxErrFuncPtrInd(notiIndIndex)) (); /* SBSW_COM_FCT_PTR_CALL_CSL03 */
  }
  Com_SetWaitingForConfirmation(ComPduId, FALSE);       /* SBSW_COM_IDXPARAM_INTERNALFCT_CSL02 */
}
#endif

#if ((COM_CBKTXERRFUNCPTRINDUSEDOFTXPDUINFO == STD_ON) || (COM_CBKTXACKIMFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)||(COM_CBKTXACKDEFFUNCPTRINDUSEDOFTXPDUINFO == STD_ON))
/**********************************************************************************************************************
  Com_TxNHdlr_Confirmation
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxNHdlr_Confirmation(Com_TxPduInfoIterType ComPduId)
{
# if (COM_CBKTXERRFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)
  /* #10 Clear notification flag */
  Com_SetWaitingForConfirmation(ComPduId, FALSE);       /* SBSW_COM_IDXPARAM_INTERNALFCT_DET_CSL02 */
# endif

# if (COM_CBKTXACKIMFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)
  {
    /* #20 Call all related notification function of this ComIPdu */
    Com_CbkTxAckImFuncPtrIndEndIdxOfTxPduInfoType notiIndIndex = Com_GetCbkTxAckImFuncPtrIndStartIdxOfTxPduInfo(ComPduId);
    for(; notiIndIndex < Com_GetCbkTxAckImFuncPtrIndEndIdxOfTxPduInfo(ComPduId); notiIndIndex++)
    {
      Com_GetCbkTxAckImFuncPtr(Com_GetCbkTxAckImFuncPtrInd(notiIndIndex)) ();   /* SBSW_COM_FCT_PTR_CALL_CSL03 */
    }
  }
# endif
# if (COM_CBKTXACKDEFFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)
  /* #30 If the notification functions are handled deferred, set the HandleTxPduDeferred flag */
  if(Com_IsCbkTxAckDefFuncPtrIndUsedOfTxPduInfo(ComPduId))      /* COV_COM_FEATURE_ALWAYS_TRUE_TX */
  {
    Com_SetHandleTxPduDeferred(ComPduId, TRUE); /* SBSW_COM_IDXPARAM_INTERNALFCT_DET_CSL02 */
  }
# endif
}
#endif

#if (COM_RXCBKFUNCPTR == STD_ON)
/**********************************************************************************************************************
  Com_CacheOrCallRxCbkFctPtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_CacheOrCallRxCbkFctPtr(Com_RxCbkFuncPtrIterType cbkFuncPtrIdx, Com_RxPduInfoIterType comRxPduInfoIdx, P2VAR(FctPtrCacheStrct, AUTOMATIC, COM_APPL_DATA) fctPtrCacheStrctPtr)
{
# if ((COM_RXDEFERREDFCTPTRCACHE == STD_ON) && (COM_RXIMMEDIATEFCTPTRCACHE == STD_ON))
  /* #10 If callback function pointer should be processed deferred */
  if(Com_IsHandleRxPduDeferredUsedOfRxPduInfo(comRxPduInfoIdx))
  {
# endif
# if (COM_RXDEFERREDFCTPTRCACHE == STD_ON)
    /* #20 Cache index of function ptr in deferred function pointer cache or call all function pointer if cache is full. */
    if(Com_IsMainFunctionRxStructUsedOfRxPduInfo(comRxPduInfoIdx))      /* COV_COM_SILENT_TX */
    {
      Com_RxMainFunctionProcessingISRLockCounterIdxOfMainFunctionRxStructType rxMainFunctionProcessingISRLockCounteridx =
        Com_GetRxMainFunctionProcessingISRLockCounterIdxOfMainFunctionRxStruct(Com_GetMainFunctionRxStructIdxOfRxPduInfo(comRxPduInfoIdx));
      Com_CacheOrCallRxDeferredCbkFctPtr(rxMainFunctionProcessingISRLockCounteridx, cbkFuncPtrIdx, fctPtrCacheStrctPtr);        /* SBSW_COM_FCTPTR_CACHE_PTR */
    }
# endif
# if ((COM_RXDEFERREDFCTPTRCACHE == STD_ON) && (COM_RXIMMEDIATEFCTPTRCACHE == STD_ON))
  }
  /* #30 Otherwise */
  else
# endif
  {
# if (COM_RXIMMEDIATEFCTPTRCACHE == STD_ON)
    /* #40 Cache index of function ptr in immediate function pointer cache or call all function pointer if cache is full. */
    Com_CacheOrCallRxImmediateCbkFctPtr(cbkFuncPtrIdx, fctPtrCacheStrctPtr);    /* SBSW_COM_FCTPTR_CACHE_PTR */
# endif
  }

  COM_DUMMY_STATEMENT(comRxPduInfoIdx); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}
#endif

#define COM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* *INDENT-OFF* */
/* Justifications for module-specific MISRA deviations:

*/

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/
/* SBSW_JUSTIFICATION_BEGIN

SBSW_JUSTIFICATION_END */

/***********************************************************************************************************************
 *  COVERAGE JUSTIFICATIONS
 **********************************************************************************************************************/
/* START_COVERAGE_JUSTIFICATION

END_COVERAGE_JUSTIFICATION */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  END OF FILE: Com_Notifications.c
 *********************************************************************************************************************/
