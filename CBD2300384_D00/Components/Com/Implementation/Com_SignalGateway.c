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
/*!        \file  Com_SignalGateway.c
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_SIGNALGATEWAY_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com.h"
#include "Com_Timer.h"
#include "Com_SignalGateway.h"
#include "Com_SignalIf.h"
#include "SchM_Com.h"
#include "Com_CprQueueSigGw.h"
#include "Com_CprQueue.h"

#include "vstdlib.h"

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
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

#if (COM_GWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_EnterExclusiveArea
**********************************************************************************************************************/
/*! \brief        This function enters the gw signal routing exclusive area.
    \details      -
    \pre          -
    \trace        DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context      TASK
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_EnterExclusiveArea(void);
#endif

#if (COM_GWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_ExitExclusiveArea
**********************************************************************************************************************/
/*! \brief        This function exits the gw signal routing exclusive area.
    \details      -
    \pre          -
    \param[in]    idx    index of the indirection from the main function route signals to the ISRLock counter
    \trace        DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context      TASK
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_ExitExclusiveArea(Com_GatewayProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType idx);
#endif

#if (COM_GWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_ThresholdCheck
**********************************************************************************************************************/
/*! \brief        This function checks if the ISR Lock must be opened
    \details      -
    \pre          -
    \param[in]    idx    index of the indirection from the main function route signals to the ISRLock counter
    \trace        DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context      TASK
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_ThresholdCheck(Com_GatewayProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType idx);
#endif

#if (((COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON) || (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON)) && (COM_MINPROCGWBUFFERUSEDOFRXACCESSINFO == STD_OFF) && (COM_MINPROCGWBUFFERARRAYBASEDBUFFERUSEDOFRXACCESSINFO == STD_OFF))
/**********************************************************************************************************************
  Com_SignalGw_GetSourcePtr
**********************************************************************************************************************/
/*! \brief        This function gets the pointer to the source buffer.
    \details      -
    \param[in]    idxRxAccessInfo   Index of RxAccessInfo.
    \param[out]   sourcePtr         Pointer to the source buffer
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_GetSourcePtr(Com_SignalIdType idxRxAccessInfo, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) sourcePtr);
#endif

#if (((COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON) || (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON)) && (COM_MINPROCGWBUFFERUSEDOFRXACCESSINFO == STD_ON) || (COM_MINPROCGWBUFFERARRAYBASEDBUFFERUSEDOFRXACCESSINFO == STD_ON))
/**********************************************************************************************************************
  Com_SignalGw_GetSourcePtrMinimalProcessing
**********************************************************************************************************************/
/*! \brief        This gets the pointer to the source buffer for MinimalProcessing.
    \details      -
    \param[in]    idxRxAccessInfo   Index of RxAccessInfo.
    \param[out]   sourcePtr         Pointer to the source buffer
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_GetSourcePtrMinimalProcessing(Com_SignalIdType idxRxAccessInfo, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) sourcePtr);
#endif

#if (COM_GWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_ProcessGwEvents
**********************************************************************************************************************/
/*! \brief        This function processes the signal gateway events.
    \details      -
    \pre          -
    \param[in]    mainFunctionId  Idx of the main function route signals
    \param[in]    gwProcessingISRlockCounterIdx index of the indirection from the main function route signals to the ISRLock counter
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_ProcessGwEvents(Com_MainFunctionRouteSignalsStructIterType mainFunctionId, Com_GatewayProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType gwProcessingISRlockCounterIdx);
#endif

#if (COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_SignalRoutings
**********************************************************************************************************************/
/*! \brief        This function processes the routing of the passed ComSignal.
    \details      -
    \pre          -
    \param[in]    idxGwInfo  Index of GwInfo
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_SignalRoutings(Com_GwInfoIterType idxGwInfo);
#endif

#if (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_SignalGroupRoutings
**********************************************************************************************************************/
/*! \brief        This function processes the routing of the passed ComSignalGroup
    \details      -
    \pre          -
    \param[in]    idxGwInfo  Index of GwInfo
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_SignalGroupRoutings(Com_GwInfoIterType idxGwInfo);
#endif

#if (COM_CPRQUEUEUSEDOFGWSIGGRPMAPPING == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_SignalGroupRoutings_CrossPartitionRoutingRx
**********************************************************************************************************************/
/*! \brief        This function processes cross partition signal group routings.
    \details      -
    \param[in]    idxGwSigMapping   Index of GwSigMapping.
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_SignalGroupRoutings_CrossPartitionRoutingRx(Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping);
#endif

#if (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_SignalGroupRoutings_PartitionLocal
**********************************************************************************************************************/
/*! \brief        This function processes partition local signal group routings.
    \details      -
    \param[in]    idxGwSigGrpMapping   Index of GwSigMapping.
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_SignalGroupRoutings_PartitionLocal(Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping);
#endif

#if(COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_ProcessCrossPartitionQueuesTx
**********************************************************************************************************************/
/*! \brief        This function processes the cross partition routing queues leading to this partition.
    \details      -
    \pre          -
    \param[in]    mainFunctionId  Idx of the main function route signals
    \param[in]    gwProcessingISRlockCounterIdx index of the indirection from the main function route signals to the ISRLock counter
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_ProcessCrossPartitionQueuesTx(Com_MainFunctionRouteSignalsStructIterType mainFunctionId, Com_GatewayProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType gwProcessingISRlockCounterIdx);
#endif

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
  Com_SignalGw_ProcessCrossPartition_SignalGroupRouting
**********************************************************************************************************************/
/*! \brief        This function processes the cross partition routing for signals on tx side.
    \details      -
    \pre          -
    \param[in]    routingQueueIndex  Idx of the cross partition routing queue.
    \param[in]    readIdx            Index to the position in the queue where the data shall be read.
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_ProcessCrossPartition_SignalGroupRouting(Com_CprQueueSignalRoutingIndType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx);
#endif

#if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
  Com_SignalGw_ProcessCrossPartition_SignalRouting
**********************************************************************************************************************/
/*! \brief        This function processes the cross partition routing for signal routings on tx side.
    \details      -
    \pre          -
    \param[in]    routingQueueIndex  Idx of the cross partition routing queue.
    \param[in]    readIdx            Index to the position in the queue where the data shall be read.
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_ProcessCrossPartition_SignalRouting(Com_CprQueueSignalRoutingIndType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx);
#endif

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/
#if (COM_GWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_EnterExclusiveArea
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_EnterExclusiveArea(void)
{
  /* Enter the gw signal routing exclusive area */
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_BOTH();
}
#endif

#if (COM_GWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_ExitExclusiveArea
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_ExitExclusiveArea(Com_GatewayProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType idx)
{
  /* Reload the counter with the threshold value */
  Com_SetGatewayProcessingISRLockCounter(idx, Com_GetGatewayProcessingISRLockThreshold());      /* SBSW_COM_CSL_VAR_ACCESS */
  /* Exit the gw signal routing exclusive area */
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_BOTH();
}
#endif

#if (COM_GWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_ThresholdCheck
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_ThresholdCheck(Com_GatewayProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType idx)
{
  /* If the threshold counter is greater 0, decrement the counter */
  if(Com_GetGatewayProcessingISRLockCounter(idx) > 0u)  /* COV_COM_THRESHOLD_TX */
  {
    Com_DecGatewayProcessingISRLockCounter(idx);        /* SBSW_COM_CSL_VAR_ACCESS */
  }
  /* Otherwise exit and reenter the gw signal routing exclusive area */
  else
  {
    Com_SignalGw_ExitExclusiveArea(idx);
    Com_SignalGw_EnterExclusiveArea();
  }
}
#endif

#if (((COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON) || (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON)) && (COM_MINPROCGWBUFFERUSEDOFRXACCESSINFO == STD_OFF) && (COM_MINPROCGWBUFFERARRAYBASEDBUFFERUSEDOFRXACCESSINFO == STD_OFF))
/**********************************************************************************************************************
  Com_SignalGw_GetSourcePtr
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_GetSourcePtr(Com_SignalIdType idxRxAccessInfo, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) sourcePtr)
{
  /* Get the pointer to the source buffer, the source buffer of the value depends on the signal ApplType. */
# if (COM_ROUTINGBUFFERUSEDOFRXACCESSINFO == STD_ON)
  if(Com_IsRoutingBufferUsedOfRxAccessInfo(idxRxAccessInfo)) /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
  {
    switch (Com_GetRoutingBufferOfRxAccessInfo(idxRxAccessInfo))
    {
#  if (COM_EXISTS_RXSIGBUFFERUINT8_ROUTINGBUFFEROFRXACCESSINFO == STD_ON)
      case COM_RXSIGBUFFERUINT8_ROUTINGBUFFEROFRXACCESSINFO:   /* COV_COM_APPLTYPEOFRXACCESSINFO_GW_ROUTING */
      {
        sourcePtr->SduDataPtr = Com_GetAddrRxSigBufferUInt8(Com_GetRoutingBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_RXSIGBUFFERSINT8_ROUTINGBUFFEROFRXACCESSINFO == STD_ON)
      case COM_RXSIGBUFFERSINT8_ROUTINGBUFFEROFRXACCESSINFO:   /* COV_COM_APPLTYPEOFRXACCESSINFO_GW_ROUTING */
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrRxSigBufferSInt8(Com_GetRoutingBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_RXSIGBUFFERUINT16_ROUTINGBUFFEROFRXACCESSINFO == STD_ON)
      case COM_RXSIGBUFFERUINT16_ROUTINGBUFFEROFRXACCESSINFO:  /* COV_COM_APPLTYPEOFRXACCESSINFO_GW_ROUTING */
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrRxSigBufferUInt16(Com_GetRoutingBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_RXSIGBUFFERSINT16_ROUTINGBUFFEROFRXACCESSINFO == STD_ON)
      case COM_RXSIGBUFFERSINT16_ROUTINGBUFFEROFRXACCESSINFO:  /* COV_COM_APPLTYPEOFRXACCESSINFO_GW_ROUTING */
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrRxSigBufferSInt16(Com_GetRoutingBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_RXSIGBUFFERUINT32_ROUTINGBUFFEROFRXACCESSINFO == STD_ON)
      case COM_RXSIGBUFFERUINT32_ROUTINGBUFFEROFRXACCESSINFO:  /* COV_COM_APPLTYPEOFRXACCESSINFO_GW_ROUTING */
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrRxSigBufferUInt32(Com_GetRoutingBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_RXSIGBUFFERSINT32_ROUTINGBUFFEROFRXACCESSINFO == STD_ON)
      case COM_RXSIGBUFFERSINT32_ROUTINGBUFFEROFRXACCESSINFO:  /* COV_COM_APPLTYPEOFRXACCESSINFO_GW_ROUTING */
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrRxSigBufferSInt32(Com_GetRoutingBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_RXSIGBUFFERUINT64_ROUTINGBUFFEROFRXACCESSINFO == STD_ON)
      case COM_RXSIGBUFFERUINT64_ROUTINGBUFFEROFRXACCESSINFO:  /* COV_COM_APPLTYPEOFRXACCESSINFO_GW_ROUTING */
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrRxSigBufferUInt64(Com_GetRoutingBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_RXSIGBUFFERSINT64_ROUTINGBUFFEROFRXACCESSINFO == STD_ON)
      case COM_RXSIGBUFFERSINT64_ROUTINGBUFFEROFRXACCESSINFO:  /* COV_COM_APPLTYPEOFRXACCESSINFO_GW_ROUTING */
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrRxSigBufferSInt64(Com_GetRoutingBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_RXSIGBUFFERFLOAT32_ROUTINGBUFFEROFRXACCESSINFO == STD_ON)
      case COM_RXSIGBUFFERFLOAT32_ROUTINGBUFFEROFRXACCESSINFO: /* COV_COM_APPLTYPEOFRXACCESSINFO_GW_ROUTING */
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrRxSigBufferFloat32(Com_GetRoutingBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_RXSIGBUFFERFLOAT64_ROUTINGBUFFEROFRXACCESSINFO == STD_ON)
      case COM_RXSIGBUFFERFLOAT64_ROUTINGBUFFEROFRXACCESSINFO: /* COV_COM_APPLTYPEOFRXACCESSINFO_GW_ROUTING */
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrRxSigBufferFloat64(Com_GetRoutingBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
      default: /* COV_COM_MISRA */
        break;
    }
  }
# endif
# if (COM_RXSIGBUFFERARRAYBASEDROUTINGBUFFERUSEDOFRXACCESSINFO == STD_ON)
  if(Com_IsRxSigBufferArrayBasedRoutingBufferUsedOfRxAccessInfo(idxRxAccessInfo))       /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
  {
    sourcePtr->SduDataPtr = Com_GetAddrRxSigBufferArrayBased(Com_GetRxSigBufferArrayBasedRoutingBufferStartIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
  }

# endif
# if (COM_EXISTS_ZEROBIT_APPLTYPEOFRXACCESSINFO == STD_ON)
  if(Com_GetApplTypeOfRxAccessInfo(idxRxAccessInfo) == COM_ZEROBIT_APPLTYPEOFRXACCESSINFO) /* COV_COM_ZEROBIT */
  {
    sourcePtr->SduDataPtr = NULL_PTR; /* SBSW_COM_WRITEACCESS_STACKPTR */
  }
# endif

  COM_DUMMY_STATEMENT(idxRxAccessInfo);   /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
  COM_DUMMY_STATEMENT(sourcePtr);   /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
}       /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

#if (((COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON) || (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON)) && (COM_MINPROCGWBUFFERUSEDOFRXACCESSINFO == STD_ON) || (COM_MINPROCGWBUFFERARRAYBASEDBUFFERUSEDOFRXACCESSINFO == STD_ON))
/**********************************************************************************************************************
  Com_SignalGw_GetSourcePtrMinimalProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_GetSourcePtrMinimalProcessing(Com_SignalIdType idxRxAccessInfo, P2VAR(PduInfoType, AUTOMATIC, COM_APPL_DATA) sourcePtr)
{
  /* Get the pointer to the source buffer, the source buffer of the value depends on the signal ApplType. */
# if(COM_MINPROCGWBUFFERUSEDOFRXACCESSINFO == STD_ON)
  if(Com_IsMinProcGwBufferUsedOfRxAccessInfo(idxRxAccessInfo)) /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
  {
    switch (Com_GetMinProcGwBufferOfRxAccessInfo(idxRxAccessInfo))
    {
#  if (COM_EXISTS_MINPROCGWBUFFERUINT8_MINPROCGWBUFFEROFRXACCESSINFO == STD_ON)
      case COM_MINPROCGWBUFFERUINT8_MINPROCGWBUFFEROFRXACCESSINFO:
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrMinProcGwBufferUInt8(Com_GetMinProcGwBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_MINPROCGWBUFFERSINT8_MINPROCGWBUFFEROFRXACCESSINFO == STD_ON)
      case COM_MINPROCGWBUFFERSINT8_MINPROCGWBUFFEROFRXACCESSINFO:
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrMinProcGwBufferSInt8(Com_GetMinProcGwBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_MINPROCGWBUFFERUINT16_MINPROCGWBUFFEROFRXACCESSINFO == STD_ON)
      case COM_MINPROCGWBUFFERUINT16_MINPROCGWBUFFEROFRXACCESSINFO:
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrMinProcGwBufferUInt16(Com_GetMinProcGwBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_MINPROCGWBUFFERSINT16_MINPROCGWBUFFEROFRXACCESSINFO == STD_ON)
      case COM_MINPROCGWBUFFERSINT16_MINPROCGWBUFFEROFRXACCESSINFO:
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrMinProcGwBufferSInt16(Com_GetMinProcGwBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_MINPROCGWBUFFERUINT32_MINPROCGWBUFFEROFRXACCESSINFO == STD_ON)
      case COM_MINPROCGWBUFFERUINT32_MINPROCGWBUFFEROFRXACCESSINFO:
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrMinProcGwBufferUInt32(Com_GetMinProcGwBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_MINPROCGWBUFFERSINT32_MINPROCGWBUFFEROFRXACCESSINFO == STD_ON)
      case COM_MINPROCGWBUFFERSINT32_MINPROCGWBUFFEROFRXACCESSINFO:
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrMinProcGwBufferSInt32(Com_GetMinProcGwBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_MINPROCGWBUFFERUINT64_MINPROCGWBUFFEROFRXACCESSINFO == STD_ON)
      case COM_MINPROCGWBUFFERUINT64_MINPROCGWBUFFEROFRXACCESSINFO:
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrMinProcGwBufferUInt64(Com_GetMinProcGwBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_MINPROCGWBUFFERSINT64_MINPROCGWBUFFEROFRXACCESSINFO == STD_ON)
      case COM_MINPROCGWBUFFERSINT64_MINPROCGWBUFFEROFRXACCESSINFO:
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrMinProcGwBufferSInt64(Com_GetMinProcGwBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_MINPROCGWBUFFERFLOAT32_MINPROCGWBUFFEROFRXACCESSINFO == STD_ON)
      case COM_MINPROCGWBUFFERFLOAT32_MINPROCGWBUFFEROFRXACCESSINFO:
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrMinProcGwBufferFloat32(Com_GetMinProcGwBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
#  if (COM_EXISTS_MINPROCGWBUFFERFLOAT64_MINPROCGWBUFFEROFRXACCESSINFO == STD_ON)
      case COM_MINPROCGWBUFFERFLOAT64_MINPROCGWBUFFEROFRXACCESSINFO:
      {
        sourcePtr->SduDataPtr = (SduDataPtrType) Com_GetAddrMinProcGwBufferFloat64(Com_GetMinProcGwBufferIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
        break;
      }
#  endif
      default: /* COV_COM_MISRA */
        break;
    }
  }
# endif
# if (COM_MINPROCGWBUFFERARRAYBASEDBUFFERUSEDOFRXACCESSINFO == STD_ON)
  if(Com_IsMinProcGwBufferArrayBasedBufferUsedOfRxAccessInfo(idxRxAccessInfo))  /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
  {
    sourcePtr->SduDataPtr = Com_GetAddrMinProcGwBufferArrayBased(Com_GetMinProcGwBufferArrayBasedBufferStartIdxOfRxAccessInfo(idxRxAccessInfo)); /* SBSW_COM_WRITEACCESS_STACKPTR */
  }
# endif
# if (COM_EXISTS_ZEROBIT_APPLTYPEOFRXACCESSINFO == STD_ON)
  if(Com_GetApplTypeOfRxAccessInfo(idxRxAccessInfo) == COM_ZEROBIT_APPLTYPEOFRXACCESSINFO) /* COV_COM_ZEROBIT */
  {
    sourcePtr->SduDataPtr = NULL_PTR; /* SBSW_COM_WRITEACCESS_STACKPTR */
  }
# endif

  COM_DUMMY_STATEMENT(idxRxAccessInfo);   /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
  COM_DUMMY_STATEMENT(sourcePtr);   /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
}       /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

#if (COM_GWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_ProcessGwEvents
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_ProcessGwEvents(Com_MainFunctionRouteSignalsStructIterType mainFunctionId, Com_GatewayProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType gwProcessingISRlockCounterIdx)
{
  Com_GwInfoIndIterType gwInfoIndIdx = Com_GetGwInfoIndStartIdxOfMainFunctionRouteSignalsStruct(mainFunctionId);
  /* Iterate over all gateway event flags */
  for(; gwInfoIndIdx < Com_GetGwInfoIndEndIdxOfMainFunctionRouteSignalsStruct(mainFunctionId); gwInfoIndIdx++)
  {
    Com_GwInfoIterType gwInfoIdx = Com_GetGwInfoInd(gwInfoIndIdx);
    /* If a gateway event flag is set */
    if(Com_IsGwEvent(gwInfoIdx))
    {
      Com_SetGwEvent(gwInfoIdx, FALSE);       /* SBSW_COM_CSL02 */

#  if (COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON)
      /* Do normal signal routings */
      Com_SignalGw_SignalRoutings(gwInfoIdx);
#  endif

#  if (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON)
      /* Do normal signal group routings */
      Com_SignalGw_SignalGroupRoutings(gwInfoIdx);
#  endif
    }
    Com_SignalGw_ThresholdCheck(gwProcessingISRlockCounterIdx);
  }
}
#endif

#if (COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_SignalRoutings
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_SignalRoutings(Com_GwInfoIterType idxGwInfo)
{
  /* Iterate over all signal depending gateway mappings of the passed gateway info */
  Com_GwSigMappingEndIdxOfGwInfoType idxGwSigMapping = Com_GetGwSigMappingStartIdxOfGwInfo(idxGwInfo);

  for(; idxGwSigMapping < Com_GetGwSigMappingEndIdxOfGwInfo(idxGwInfo); idxGwSigMapping++)
  {
    PduInfoType sourcePtr;
    Com_TxSigInfoIterType txSigId = (Com_TxSigInfoIterType) Com_GetTxSigInfoIdxOfGwSigMapping(idxGwSigMapping);
    Com_TxAccessInfoIterType txAccessInfo = Com_GetTxAccessInfoIdxOfTxSigInfo(txSigId);
    Com_SignalIdType idxRxAccessInfo = (Com_SignalIdType) Com_GetRxAccessInfoIdxOfGwSigMapping(idxGwSigMapping);
    sourcePtr.SduDataPtr = NULL_PTR;
    sourcePtr.SduLength = Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo);

# if (COM_GWTIMEOUTINFO == STD_ON)
#  if (COM_CPRQUEUEUSEDOFGWSIGMAPPING == STD_ON)
    if(!Com_IsCprQueueUsedOfGwSigMapping(idxGwSigMapping))
#  endif
    {
      /* Start cyclic transmission for periodic I-PDUs with a configured GW Timeout and reload gw Timeout counter */
      Com_GwTout_Event(Com_GetTxPduInfoIdxOfTxSigInfo(txSigId));
    }
# endif
    
    /* Get the pointer to the source buffer */
# if ((COM_MINPROCGWBUFFERUSEDOFRXACCESSINFO == STD_ON) || (COM_MINPROCGWBUFFERARRAYBASEDBUFFERUSEDOFRXACCESSINFO == STD_ON))
    Com_SignalGw_GetSourcePtrMinimalProcessing(idxRxAccessInfo, &sourcePtr);        /* SBSW_COM_FCTCALL_STACKPTR */
# else
    Com_SignalGw_GetSourcePtr(idxRxAccessInfo, &sourcePtr);        /* SBSW_COM_FCTCALL_STACKPTR */
# endif

    /* If the indirection to the CrossPartitionRoutingQueue is used it is a cross partition routing */
# if (COM_CPRQUEUEUSEDOFGWSIGMAPPING == STD_ON)
    if(Com_IsCprQueueUsedOfGwSigMapping(idxGwSigMapping))
    {
      /* Write gateway description into Queue. If write is successful E_OK is returned, if write isn't successful E_NOT_OK is returned and DET is called. */
      if(E_OK != Com_CprQueueSigGw_Write_Signal(Com_GetCprQueueIdxOfGwSigMapping(idxGwSigMapping), idxGwSigMapping, &sourcePtr)) /* SBSW_COM_FCTCALL_STACKPTR */
      {
        Com_LMgt_CallDetError(COMServiceId_CprQueueSigGw_Write_Signal, COM_E_CPRQUEUE_OVERFLOW);
      }
    }
    /* Else process partition local routing */
    else 
# endif
    {
      (void) Com_SendSignal_Processing(txAccessInfo, sourcePtr.SduDataPtr, 0); /* PRQA S 0315 */ /* MD_Com_0314_0315 */ /* SBSW_COM_FCTCALL_WITH_P2CONST */
    }
  }
}
#endif

#if (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_SignalGroupRoutings
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_SignalGroupRoutings(Com_GwInfoIterType idxGwInfo)
{
  /* Iterate over all signal group depending gateway mappings of the passed gateway info */
  Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping = Com_GetGwSigGrpMappingStartIdxOfGwInfo(idxGwInfo); /* PRQA S 2842 */ /* MD_Com_2842 */

  for(; idxGwSigGrpMapping < Com_GetGwSigGrpMappingEndIdxOfGwInfo(idxGwInfo); idxGwSigGrpMapping++)
  {
    /* If the indirection to the CrossPartitionRoutingQueue is used it is a cross partition routing */
# if (COM_CPRQUEUEUSEDOFGWSIGGRPMAPPING == STD_ON)
    if(Com_IsCprQueueUsedOfGwSigGrpMapping(idxGwSigGrpMapping))
    {
      Com_SignalGw_SignalGroupRoutings_CrossPartitionRoutingRx(idxGwSigGrpMapping);
    }
    /* Else process partition local routing */
    else 
# endif
    {
      Com_SignalGw_SignalGroupRoutings_PartitionLocal(idxGwSigGrpMapping);
    }
  }
}       /* PRQA S 6030 */ /* MD_MSR_STCYC */
#endif

#if (COM_CPRQUEUEUSEDOFGWSIGGRPMAPPING == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_SignalGroupRoutings_CrossPartitionRoutingRx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_SignalGroupRoutings_CrossPartitionRoutingRx(Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping)
{
  Com_GwSigGrpMappingIterType idxGwGrpSigMapping = Com_GetGwGrpSigMappingStartIdxOfGwSigGrpMapping(idxGwSigGrpMapping);
  Com_CCprQueueIterType routingQueueIndex = Com_GetCprQueueIdxOfGwSigGrpMapping(idxGwSigGrpMapping);
  Com_CprSignalRoutingTmpBufferIterType bufferIdx = 0u;
  PduInfoType bufferPtr;
  bufferPtr.SduDataPtr = Com_GetAddrCprSignalRoutingTmpBuffer(bufferIdx);
  bufferPtr.SduLength = 0u;

  /* Iterate over all related group signal mappings */
  for(; idxGwGrpSigMapping < Com_GetGwGrpSigMappingEndIdxOfGwSigGrpMapping(idxGwSigGrpMapping); idxGwGrpSigMapping++)
  {
    PduInfoType sourcePtr;
    Com_SignalIdType idxRxAccessInfo = (Com_SignalIdType) Com_GetRxAccessInfoIdxOfGwGrpSigMapping(idxGwGrpSigMapping);
    sourcePtr.SduDataPtr = NULL_PTR;
    sourcePtr.SduLength = Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo);

    /* Get the pointer to the source buffer */
# if ((COM_MINPROCGWBUFFERUSEDOFRXACCESSINFO == STD_ON) || (COM_MINPROCGWBUFFERARRAYBASEDBUFFERUSEDOFRXACCESSINFO == STD_ON))
    Com_SignalGw_GetSourcePtrMinimalProcessing(idxRxAccessInfo, &sourcePtr);        /* SBSW_COM_FCTCALL_STACKPTR */
# else
    Com_SignalGw_GetSourcePtr(idxRxAccessInfo, &sourcePtr);        /* SBSW_COM_FCTCALL_STACKPTR */
# endif

    /* Safety check if group signal fits into tmp buffer */
    if((bufferIdx + sourcePtr.SduLength) > 0u)  /* COV_COM_DEFENSIVE_PROGRAMMING_CROSSPARTITIONSIGNALROUTINGQUEUE */
    {
      if((bufferIdx + sourcePtr.SduLength - 1u) < Com_GetSizeOfCprSignalRoutingTmpBuffer()) /* COV_COM_SILENT_TX */
      {
        /* copy source buffer to tmp buffer */
        VStdMemCpyRamToRam(Com_GetAddrCprSignalRoutingTmpBuffer(bufferIdx), sourcePtr.SduDataPtr, sourcePtr.SduLength); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_COM_MEM_CPY_2RAM */
        bufferIdx += Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo);
        bufferPtr.SduLength += Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo);
      }
    }
  }

  /* Write signal group to the CrossPartitionRoutingQueue. If write is successful E_OK is returned, if write isn't successful E_NOT_OK is returned and DET is called. */
  if(E_OK != Com_CprQueueSigGw_Write_SignalGroup(routingQueueIndex, idxGwSigGrpMapping, &bufferPtr)) /* SBSW_COM_FCTCALL_STACKPTR */
  {
    Com_LMgt_CallDetError(COMServiceId_CprQueueSigGw_Write_SignalGroup, COM_E_CPRQUEUE_OVERFLOW);
  }
}
#endif

#if (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_SignalGroupRoutings_PartitionLocal
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_SignalGroupRoutings_PartitionLocal(Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping)
{
  P2CONST(uint8, AUTOMATIC, COM_APPL_VAR) shdBuffer = NULL_PTR;
  Com_GwGrpSigMappingStartIdxOfGwSigGrpMappingType idxGwGrpSigMapping = Com_GetGwGrpSigMappingStartIdxOfGwSigGrpMapping(idxGwSigGrpMapping);
  Com_SignalGroupIdType txSigGrpId = Com_GetTxSigGrpInfoIdxOfGwSigGrpMapping(idxGwSigGrpMapping);

# if (COM_GWTIMEOUTINFO == STD_ON)
  /* Start cyclic transmission for periodic I-PDUs with a configured GW Timeout and reload gw Timeout counter */
  Com_GwTout_Event(Com_GetTxPduInfoIdxOfTxSigGrpInfo(txSigGrpId));
# endif

  /* Iterate over all related group signal mappings */
  for(; idxGwGrpSigMapping < Com_GetGwGrpSigMappingEndIdxOfGwSigGrpMapping(idxGwSigGrpMapping); idxGwGrpSigMapping++)
  {
    PduInfoType sourcePtr;
    Com_TxGrpSigInfoIterType txSigId = (Com_TxGrpSigInfoIterType) Com_GetTxGrpSigInfoIdxOfGwGrpSigMapping(idxGwGrpSigMapping);
    Com_TxAccessInfoIterType txAccessInfo = Com_GetTxAccessInfoIdxOfTxGrpSigInfo(txSigId);
    Com_SignalIdType idxRxAccessInfo = (Com_SignalIdType) Com_GetRxAccessInfoIdxOfGwGrpSigMapping(idxGwGrpSigMapping);
    sourcePtr.SduDataPtr = NULL_PTR;
    sourcePtr.SduLength = Com_GetSignalLengthInByteOfRxAccessInfo(idxRxAccessInfo);

    /* Get the pointer to the source buffer */
# if ((COM_MINPROCGWBUFFERUSEDOFRXACCESSINFO == STD_ON) || (COM_MINPROCGWBUFFERARRAYBASEDBUFFERUSEDOFRXACCESSINFO == STD_ON))
    Com_SignalGw_GetSourcePtrMinimalProcessing(idxRxAccessInfo, &sourcePtr);        /* SBSW_COM_FCTCALL_STACKPTR */
# else
    Com_SignalGw_GetSourcePtr(idxRxAccessInfo, &sourcePtr);        /* SBSW_COM_FCTCALL_STACKPTR */
# endif
    
    /* Process GroupSignal */
    (void) Com_SendSignal_Processing(txAccessInfo, sourcePtr.SduDataPtr, 0); /* PRQA S 0315 */ /* MD_Com_0314_0315 */ /* SBSW_COM_FCTCALL_WITH_P2CONST */
  }
  /* Call Com_SendSignalGroup for the related signal group with the respective shadowBuffer */
# if (COM_TXBUFFERUSEDOFTXSIGGRPINFO == STD_ON)
  if(Com_IsTxBufferUsedOfTxSigGrpInfo(txSigGrpId))    /* COV_COM_FEATURE_ALWAYS_TRUE_TX */
  {
    shdBuffer = Com_GetAddrTxBuffer(Com_GetTxBufferStartIdxOfTxSigGrpInfo(txSigGrpId));
  }
# endif
  (void) Com_SendSignalGroup_Processing(txSigGrpId, shdBuffer);       /* SBSW_COM_FCTCALL_WITH_P2CONST */
}
#endif

# if(COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_ProcessCrossPartitionQueuesTx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_ProcessCrossPartitionQueuesTx(Com_MainFunctionRouteSignalsStructIterType mainFunctionId, Com_GatewayProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType gwProcessingISRlockCounterIdx)
{
  /* If cross partition routing queues leading to this partition are used */
  if(Com_IsPartitionsTxStructUsedOfMainFunctionRouteSignalsStruct(mainFunctionId) && Com_IsCprQueueSignalRoutingIndUsedOfPartitionsTxStruct(Com_GetPartitionsTxStructIdxOfMainFunctionRouteSignalsStruct(mainFunctionId))) /* COV_COM_CROSSPARTITIONQUEUEUSED_TX */
  {
    /* Iterate over cross partition queues leading to this partition */
    Com_CprQueueSignalRoutingIndStartIdxOfPartitionsTxStructType routingQueueIndIndex = Com_GetCprQueueSignalRoutingIndStartIdxOfPartitionsTxStruct(Com_GetPartitionsTxStructIdxOfMainFunctionRouteSignalsStruct(mainFunctionId));
    for(; routingQueueIndIndex < Com_GetCprQueueSignalRoutingIndEndIdxOfPartitionsTxStruct(Com_GetPartitionsTxStructIdxOfMainFunctionRouteSignalsStruct(mainFunctionId));routingQueueIndIndex++)
    {
      Com_CprQueueBufferReadIdxOfCprQueueType readIdx = 0u;
      Com_CprQueueSignalRoutingIndType routingQueueIndex = Com_GetCprQueueSignalRoutingInd(routingQueueIndIndex);
      Com_CprQueueBufferLengthOfCprQueueType readThreshold = Com_GetCprQueueBufferLengthOfCprQueue(routingQueueIndex);

      /* Reset NumberOfBytesRead */
      Com_CprQueue_ResetNumberOfBytesRead(routingQueueIndex);

      /* While multi partition routing queue has stored elements, read data from queue */
      while(Com_CprQueue_GetReadIdx(routingQueueIndex, &readIdx) == E_OK)      /* SBSW_COM_FCTCALL_STACKPTR */
      {
        switch(Com_CprQueue_ReadFromCprQueueDirectly(readIdx))
        {
# if (COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON)
          /* Process signal routing */
          case COM_CPRQUEUE_HEADERVALUE_SIGNAL:
          {
            Com_SignalGw_ProcessCrossPartition_SignalRouting(routingQueueIndex, readIdx);
            /* Update the read index */      
            Com_CprQueue_UpdateReadIdx(routingQueueIndex);   
            break;
          }
# endif
# if (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON)
          /* Process signal group routing */
          case COM_CPRQUEUE_HEADERVALUE_SIGNALGROUP:
          {
            Com_SignalGw_ProcessCrossPartition_SignalGroupRouting(routingQueueIndex, readIdx);
            /* Update the read index */      
            Com_CprQueue_UpdateReadIdx(routingQueueIndex);   
            break;      
          }
# endif
          default: /* COV_COM_MISRA */
            break;
        }

        /* Abort processing if number of read bytes increases the the queue size */
        if(Com_GetCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex) > readThreshold) /* COV_COM_DEFENSIVE_PROGRAMMING_CROSSPARTITIONQUEUE */
        {
          break;
        }
        
        Com_SignalGw_ThresholdCheck(gwProcessingISRlockCounterIdx);
      }
    }
  }

  COM_DUMMY_STATEMENT(gwProcessingISRlockCounterIdx);   /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
}
# endif

# if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGGRPMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
  Com_SignalGw_ProcessCrossPartition_SignalGroupRouting
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_ProcessCrossPartition_SignalGroupRouting(Com_CprQueueSignalRoutingIndType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx)
{
  P2CONST(uint8, AUTOMATIC, COM_APPL_VAR) shdBuffer = NULL_PTR;
  Com_GwSigGrpMappingEndIdxOfGwInfoType idxGwSigGrpMapping = 0u;

  /* Read SignalGroup Index from Queue */
  Com_CprQueueSigGw_Read_SignalGroup(routingQueueIndex, readIdx, &idxGwSigGrpMapping); /* SBSW_COM_FCTCALL_STACKPTR */

  if(idxGwSigGrpMapping < Com_GetSizeOfGwSigGrpMapping())   /* COV_COM_INDEX_IN_BOUNDS_CHECK */
  {
    Com_TxSigGrpInfoIdxOfGwSigGrpMappingType txSigGrpId = Com_GetTxSigGrpInfoIdxOfGwSigGrpMapping(idxGwSigGrpMapping);
    Com_GwSigGrpMappingIterType idxGwGrpSigMapping = Com_GetGwGrpSigMappingStartIdxOfGwSigGrpMapping(idxGwSigGrpMapping);

# if (COM_GWTIMEOUTINFO == STD_ON)
    /* Start cyclic transmission for periodic I-PDUs with a configured GW Timeout and reload gw Timeout counter */
    Com_GwTout_Event(Com_GetTxPduInfoIdxOfTxSigGrpInfo(txSigGrpId));
# endif
    /* Iterate over all related group signal mappings */
    for(; idxGwGrpSigMapping < Com_GetGwGrpSigMappingEndIdxOfGwSigGrpMapping(idxGwSigGrpMapping); idxGwGrpSigMapping++)
    {
      Com_TxGrpSigInfoIterType txSigId = (Com_TxGrpSigInfoIterType) Com_GetTxGrpSigInfoIdxOfGwGrpSigMapping(idxGwGrpSigMapping);
      Com_TxAccessInfoIterType txAccessInfo = Com_GetTxAccessInfoIdxOfTxGrpSigInfo(txSigId);
      PduInfoType data;
      data.SduLength = 0u;
      data.SduDataPtr = NULL_PTR;

      /* Read GroupSignal from Queue */
      Com_CprQueueSigGw_Read_GroupSignal(routingQueueIndex, idxGwGrpSigMapping, &data); /* SBSW_COM_FCTCALL_STACKPTR */

      /* Process GroupSignal */
      (void) Com_SendSignal_Processing(txAccessInfo, data.SduDataPtr, 0); /* PRQA S 0315 */ /* MD_Com_0314_0315 */ /* SBSW_COM_FCTCALL_WITH_P2CONST */ 
    }

    /* Call Com_SendSignalGroup for the related signal group with the respective shadowBuffer */
# if (COM_TXBUFFERUSEDOFTXSIGGRPINFO == STD_ON)
    if(Com_IsTxBufferUsedOfTxSigGrpInfo(txSigGrpId))    /* COV_COM_FEATURE_ALWAYS_TRUE_TX */
    {
      shdBuffer = Com_GetAddrTxBuffer(Com_GetTxBufferStartIdxOfTxSigGrpInfo(txSigGrpId));
    }
# endif
    (void) Com_SendSignalGroup_Processing(txSigGrpId, shdBuffer);       /* SBSW_COM_FCTCALL_WITH_P2CONST */
  } 
}
#endif

# if((COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON) && (COM_GWSIGMAPPINGENDIDXOFGWINFO == STD_ON))
/**********************************************************************************************************************
  Com_SignalGw_ProcessCrossPartition_SignalRouting
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SignalGw_ProcessCrossPartition_SignalRouting(Com_CprQueueSignalRoutingIndType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx)
{
  Com_GwSigMappingEndIdxOfGwInfoType idxGwSigMapping = 0u;
  PduInfoType data;
  data.SduLength = 0u;
  data.SduDataPtr = NULL_PTR;

  /* Read Signal from Queue */
  Com_CprQueueSigGw_Read_Signal(routingQueueIndex, readIdx, &idxGwSigMapping, &data); /* SBSW_COM_FCTCALL_STACKPTR */
  
  if(idxGwSigMapping < Com_GetSizeOfGwSigMapping())   /* COV_COM_INDEX_IN_BOUNDS_CHECK */
  {
    Com_TxSigInfoIterType txSigId = (Com_TxSigInfoIterType) Com_GetTxSigInfoIdxOfGwSigMapping(idxGwSigMapping);
    Com_TxAccessInfoIterType txAccessInfo = Com_GetTxAccessInfoIdxOfTxSigInfo(txSigId);

# if (COM_GWTIMEOUTINFO == STD_ON)
    /* Start cyclic transmission for periodic I-PDUs with a configured GW Timeout and reload gw Timeout counter */
    Com_GwTout_Event(Com_GetTxPduInfoIdxOfTxSigInfo(txSigId));
# endif
    /* Process Signal */
    (void) Com_SendSignal_Processing(txAccessInfo, data.SduDataPtr, 0); /* PRQA S 0315 */ /* MD_Com_0314_0315 */ /* SBSW_COM_FCTCALL_WITH_P2CONST */
  }
  
}
#endif


/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
#if (COM_GWINFOENDIDXOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_InitRxIpdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SignalGw_InitRxIpdu(Com_RxPduInfoIterType rxPduId)
{
  /* Reset all gateway event flags of the passed ComIPdu */
  Com_GwInfoEndIdxOfRxPduInfoType idxGwInfo = Com_GetGwInfoStartIdxOfRxPduInfo(rxPduId);        /* PRQA S 2842 */ /* MD_Com_2842 */
  for(; idxGwInfo < Com_GetGwInfoEndIdxOfRxPduInfo(rxPduId); idxGwInfo++)
  {
    Com_SetGwEvent(idxGwInfo, FALSE);   /* SBSW_COM_CSL02 */
  }
}
#endif

#if (COM_GWINFOUSEDOFRXSIGINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_SigEvent
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SignalGw_SigEvent(Com_RxSigInfoIterType idxRxSigInfo)
{
  /* If the passed signal is routed, set the related gateway event flag */
  if(Com_IsGwInfoUsedOfRxSigInfo(idxRxSigInfo)) /* COV_COM_FEATURE_ALWAYS_TRUE_TX */
  {
    Com_SetGwEvent(Com_GetGwInfoIdxOfRxSigInfo(idxRxSigInfo), TRUE);    /* SBSW_COM_CSL02_CSL03 */
  }
}
#endif

#if (COM_GWINFOUSEDOFRXSIGGRPINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_SigGrpEvent
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SignalGw_SigGrpEvent(Com_RxSigGrpInfoIterType idxRxSigGrpInfo)
{
  /* If the passed signal group is routed, set the related gateway event flag */
  if(Com_IsGwInfoUsedOfRxSigGrpInfo(idxRxSigGrpInfo))   /* COV_COM_FEATURE_ALWAYS_TRUE_TX */
  {
    Com_SetGwEvent(Com_GetGwInfoIdxOfRxSigGrpInfo(idxRxSigGrpInfo), TRUE);      /* SBSW_COM_CSL02_CSL03 */
  }
}
#endif

#if (COM_GWINFO == STD_ON)
/**********************************************************************************************************************
  Com_SignalGw_MainFunction
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_SignalGw_MainFunction(Com_MainFunctionRouteSignalsStructIterType mainFunctionId)
{
  Com_GatewayProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType gwProcessingISRlockCounterIdx = Com_GetGatewayProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStruct(mainFunctionId);

  Com_SignalGw_EnterExclusiveArea();
  {
    /* Process GwEvents */
    Com_SignalGw_ProcessGwEvents(mainFunctionId, gwProcessingISRlockCounterIdx);

# if(COM_CPRQUEUESIGNALROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
    /* Process Cross Partition Routing Queues leading to this partition */
    Com_SignalGw_ProcessCrossPartitionQueuesTx(mainFunctionId, gwProcessingISRlockCounterIdx);
# endif
  }
  Com_SignalGw_ExitExclusiveArea(gwProcessingISRlockCounterIdx);
}
#endif

#define COM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* *INDENT-OFF* */
/* Justifications for module-specific MISRA deviations:
   MD_Com_0314_0315: Misra Dir 1.1
     Reason:     A void pointer is used because it depends on the configuration of which type the pointer is. It is easier to use a void pointer insead of different functions for different pointer types.
     Risk:       No risk, because the underlying pointer type is known via the configuration.
     Prevention: No prevention necessary.
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
 *  END OF FILE: Com_SignalGateway.c
 *********************************************************************************************************************/
