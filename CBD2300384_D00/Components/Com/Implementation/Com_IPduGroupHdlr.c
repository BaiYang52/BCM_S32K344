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
/*!        \file  Com_IPduGroupHdlr.c
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

#define COM_IPDUGROUPHDLR_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com_LMgt.h"
#include "Com_LLIf.h"
#include "Com_Timer.h"
#include "Com_Buffers.h"
#include "Com_TxModeHdlr.h"
#include "Com_IPduGroupHdlr.h"
#include "Com_Notifications.h"
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

#if (COM_RXTOUTINFO == STD_ON)
/**********************************************************************************************************************
  Com_LMgt_ActivateRxIpduDm
**********************************************************************************************************************/
/*! \brief        This function enables the Deadline Monitoring for the Rx I-Pdu.
    \details      -
    \pre          -
    \param[in]    ComRxToutObjtId  ID of Rx Timeout object.
**********************************************************************************************************************/
# define Com_LMgt_ActivateRxIpduDm(ComRxToutObjtId)        Com_SetRxPduDmState(ComRxToutObjtId, COM_ACTIVE_RXPDUDMSTATE)
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

#if (COM_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_ActivateRxComIPdu
**********************************************************************************************************************/
/*! \brief        This function activates Rx ComIPdu's and optionally initializes the ComIPdu.
    \details      -
    \pre          -
    \param[in]    rxPduId    - ID of Rx ComIPdu
    \param[in]    initialize - True, if belonging buffer shall be initialized
    \context      TASK
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_ActivateRxComIPdu(Com_RxPduInfoIterType rxPduId, boolean initialize);
#endif

#if (COM_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_ActivateTxComIPdu
**********************************************************************************************************************/
/*! \brief        This function activates Tx ComIPdu's and optionally initializes the ComIPdu.
    \details      -
    \pre          -
    \param[in]    ComTxPduId   - ID of Tx ComIPdu
    \param[in]    initialize - True, if belonging buffer shall be initialized
    \context      TASK
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_ActivateTxComIPdu(Com_TxPduInfoIterType ComTxPduId, boolean initialize);
#endif

#if (COM_RXPDUINFOINDUSEDOFIPDUGROUPINFO == STD_ON)
/**********************************************************************************************************************
  Com_SetRxPduGrpStateToStop
**********************************************************************************************************************/
/*! \brief        This function sets the Rx-PDU-Group-State to FALSE, if the state of all referenced IPDU groups is
 *                equal to COM_INACTIVE_IPDUGROUPSTATE
    \details      -
    \pre          -
    \param[in]    subIpduGroupId  ComIPduGroup handle id
    \context      TASK
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SetRxPduGrpStateToStop(Com_IpduGroupIdType subIpduGroupId);
#endif

#if (COM_TXPDUINFOINDUSEDOFIPDUGROUPINFO == STD_ON)
/**********************************************************************************************************************
  Com_SetTxPduGrpStateToStop
**********************************************************************************************************************/
/*! \brief        This function sets the Tx-PDU-Group-State to FALSE, if the state of all referenced IPDU groups is
 *                equal to COM_INACTIVE_IPDUGROUPSTATE
    \details      -
    \pre          -
    \param[in]    subIpduGroupId  ComIPduGroup handle id
    \context      TASK
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SetTxPduGrpStateToStop(Com_IpduGroupIdType subIpduGroupId);
#endif

#if (COM_IPDUGROUPINFO == STD_ON)
/**********************************************************************************************************************
  Com_IpduGroupStopSubGroup
**********************************************************************************************************************/
/*! \brief        Stops a preconfigured I-PDU subgroup.
                  For example, cyclic I-PDUs will be stopped after the call of Com_IpduGroupStop().
    \details      -
    \pre          -
    \param[in]    subIpduGroupId ID of I-PDU subgroup to be stopped
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
    \warning      A call to Com_IpduGroupStopSubGroup shall not be interrupted by another call to Com_IpduGroupStop, Com_EnableReceptionDM,
                  Com_DisableReceptionDM or a call to Com_IpduGroupStart.
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IpduGroupStopSubGroup(Com_IpduGroupIdType subIpduGroupId);
#endif

#if (COM_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_DeactivateRxComIPdu
**********************************************************************************************************************/
/*! \brief        This function deactivates the given Rx ComIPdu
    \details      -
    \pre          -
    \param[in]    rxPduId  ComIPdu  handle id
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DeactivateRxComIPdu(Com_RxPduInfoIterType rxPduId);
#endif

#if (COM_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_DeactivateTxComIPdu
**********************************************************************************************************************/
/*! \brief        This function deactivates the given Tx ComIPdu
    \details      -
    \param[in]    txPduId  ComIPdu  handle id
    \pre          Com_TxIPduGroup_EnterExclusiveArea has been called before
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DeactivateTxComIPdu(Com_TxPduInfoIterType txPduId);
#endif

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

#if (COM_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_ActivateRxComIPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_ActivateRxComIPdu(Com_RxPduInfoIterType rxPduId, boolean initialize)
{
  /* Start Rx I-PDU */
  /* #10 If the ComIPdu should be initialized, initialize the Rx ComIPdu parameters */
  if(initialize == TRUE)        /* COV_COM_UPDATE_IPDU_STATE */
  {
    /* COM614 If a previously stopped I-PDU gets started by the operation specified in COM613 and the parameter initialize is set to true, then the function Com_IpduGroupControl shall (re-)initialize this I-PDU before it is started. */
    Com_LMgt_InitRxIpdu(rxPduId);
  }

# if (COM_RXTPINFOUSEDOFRXPDUINFO == STD_ON)
  /* #20 If the ComIPdu is a TP Pdu initialize the TP Pdu parameters */
  if(Com_GetTypeOfRxPduInfo(rxPduId) == COM_TP_TYPEOFRXPDUINFO) /* COV_COM_FEATURE_ALWAYS_TRUE_TX */
  {
    Com_ResetRxTpConnection(rxPduId);
  }
# endif

# if (COM_EXISTS_DEFERRED_SIGNALPROCESSINGOFRXPDUINFO == STD_ON)
  /* #30 If the Rx ComIPdu is handled deferred, reset the "Handle Deferred" flag */
  if(Com_IsHandleRxPduDeferredUsedOfRxPduInfo(rxPduId)) /* COV_COM_FEATURE_ALWAYS_TRUE_TX */
  {
    Com_SetHandleRxPduDeferred(Com_GetHandleRxPduDeferredIdxOfRxPduInfo(rxPduId), 0);   /* SBSW_COM_CSL03 */
  }
# endif

  /* #40 Set the state of the Rx ComIPdu to active */
  Com_SetRxPduGrpActive(rxPduId, TRUE); /* SBSW_COM_CSL02 */
# if (COM_RXTOUTINFOINDENDIDXOFRXPDUINFO == STD_ON)
  {
    /* #50 Iterate over all related Rx timeout state machines and set them to active */
    /* \trace SPEC-2736840 */
    Com_RxTOutInfoIndStartIdxOfRxPduInfoType idxRxTOutInfoInd = Com_GetRxTOutInfoIndStartIdxOfRxPduInfo(rxPduId);
    for(; idxRxTOutInfoInd < Com_GetRxTOutInfoIndEndIdxOfRxPduInfo(rxPduId); idxRxTOutInfoInd++)
    {
      Com_LMgt_ActivateRxIpduDm(Com_GetRxTOutInfoInd(idxRxTOutInfoInd));        /* SBSW_COM_CSL02_CSL03 */
    }
  }
# endif
}
#endif

#if (COM_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_ActivateTxComIPdu
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
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_ActivateTxComIPdu(Com_TxPduInfoIterType ComTxPduId, boolean initialize)
{
  /* Start Tx I-PDU */
  /* #30 If the ComIPdu should be initialized, initialize the Tx ComIPdu parameters */
  if(initialize == TRUE)        /* COV_COM_NO_IPDUGROUPS */
  {
    /* COM614 If a previously stopped I-PDU gets started by the operation specified in COM613 and the parameter initialize is set to true, then the function Com_IpduGroupControl shall (re-)initialize this I-PDU before it is started. */
    Com_LMgt_InitTxIpdu(ComTxPduId);
  }

# if (COM_TXTPINFOUSEDOFTXPDUINFO == STD_ON)
  if(Com_IsTxTpInfoUsedOfTxPduInfo(ComTxPduId)) /* COV_COM_FEATURE_ALWAYS_TRUE_TX */
  {
    Com_ResetTxTpConnection(ComTxPduId);
  }
# endif

# if (COM_TXTOUTINFOUSEDOFTXPDUINFO == STD_ON)
  /* #40 Start the Tx deadline monitoring */
  /* \trace SPEC-2736840 */
  Com_TxDlMon_IPduGroupStart(ComTxPduId);
# endif
  /* #45 Initialize gateway counter */
# if (COM_GWTIMEOUTINFO == STD_ON)
  Com_GwTout_InitCounter(ComTxPduId);
# endif
# if (COM_CONSTVALUEUINT8UBCLEARMASKENDIDXOFTXPDUINFO == STD_ON)
  /* #50 Clear all Update Bits */
  Com_TxBuffer_ClearUpdateBits(ComTxPduId);
# endif
  /* #60 Set the Tx ComIPdu Group state to active */
  Com_SetTxPduGrpActive(ComTxPduId, TRUE);      /* SBSW_COM_CSL02 */
}
#endif

#if (COM_RXPDUINFOINDUSEDOFIPDUGROUPINFO == STD_ON)
/**********************************************************************************************************************
  Com_SetRxPduGrpStateToStop
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SetRxPduGrpStateToStop(Com_IpduGroupIdType subIpduGroupId)
{
  if(Com_IsPartitionsRxStructUsedOfIPduGroupInfo(subIpduGroupId)) /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
  {
    /* #10 Iterate over all related Rx ComIPdu of the passed ComIPduGroup */
    Com_PartitionsRxStructIdxOfIPduGroupInfoType partitionsIdx = Com_GetPartitionsRxStructIdxOfIPduGroupInfo(subIpduGroupId);
    Com_RxIPduGroupISRLockCounterIdxOfPartitionsRxStructType rxIpduGroupISRLockCounterIdx = Com_GetRxIPduGroupISRLockCounterIdxOfPartitionsRxStruct(partitionsIdx);
    Com_RxPduInfoIndEndIdxOfIPduGroupInfoType rxPduIndId = Com_GetRxPduInfoIndStartIdxOfIPduGroupInfo(subIpduGroupId);

    Com_RxIPduGroup_EnterExclusiveArea();

    for(; rxPduIndId < Com_GetRxPduInfoIndEndIdxOfIPduGroupInfo(subIpduGroupId); rxPduIndId++)
    {
      boolean canBeStopped = TRUE;
      Com_RxPduInfoIndType rxPduId = Com_GetRxPduInfoInd(rxPduIndId);

      /* #20 Iterate over all related ComIPdu Groups */
      Com_IPduGroupInfoOfRxPduInfoIndEndIdxOfRxPduInfoType referencedIPduGroupInd = Com_GetIPduGroupInfoOfRxPduInfoIndStartIdxOfRxPduInfo(rxPduId);       /* PRQA S 2843 */ /* MD_Com_2843_rxPduId */
      for(; referencedIPduGroupInd < Com_GetIPduGroupInfoOfRxPduInfoIndEndIdxOfRxPduInfo(rxPduId); referencedIPduGroupInd++)
      {
        Com_IPduGroupInfoOfRxPduInfoIndType referencedIPduGroup = Com_GetIPduGroupInfoOfRxPduInfoInd(referencedIPduGroupInd);
        /* #30 If at least one related ComIPdu Group is active, the ComIPdu remains active */
        if(Com_GetIPduGroupState(referencedIPduGroup) != COM_INACTIVE_IPDUGROUPSTATE)     /* COV_COM_RX_IPDU_GROUP_ASR3_XF */
        {
          canBeStopped = FALSE;
          break;
        }
      }
      /* #40 If all related ComIPdu Groups are stopped the ComIPdu Group state is set to inactive */
      if(canBeStopped == TRUE)    /* COV_COM_RX_IPDU_GROUP_ASR3_TX */
      {
        Com_DeactivateRxComIPdu(rxPduId);
      }

      Com_IPduGroupHdlr_RxISRThresholdCheck(rxIpduGroupISRLockCounterIdx);
    }

    Com_IPduGroupHdlr_ReloadRxIPduGroupISRLockCounter(rxIpduGroupISRLockCounterIdx);
    Com_RxIPduGroup_ExitExclusiveArea();
  }
}
#endif

#if (COM_TXPDUINFOINDUSEDOFIPDUGROUPINFO == STD_ON)
/**********************************************************************************************************************
  Com_SetTxPduGrpStateToStop
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_SetTxPduGrpStateToStop(Com_IpduGroupIdType subIpduGroupId)
{
  if(Com_IsPartitionsTxStructUsedOfIPduGroupInfo(subIpduGroupId)) /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
  {
    Com_PartitionsTxStructIdxOfIPduGroupInfoType partitionsIdx = Com_GetPartitionsTxStructIdxOfIPduGroupInfo(subIpduGroupId);
    Com_TxIPduGroupISRLockCounterIdxOfPartitionsTxStructType txIpduGroupISRLockCounterIdx = Com_GetTxIPduGroupISRLockCounterIdxOfPartitionsTxStruct(partitionsIdx);
    Com_TxPduInfoIndEndIdxOfIPduGroupInfoType txPduIndId = Com_GetTxPduInfoIndStartIdxOfIPduGroupInfo(subIpduGroupId);    /* PRQA S 2842 */ /* MD_Com_2842 */ /* 2842: Index subIpduGroupId originates from Com_IPduGroupHdlr_Stop(). It is retrieved via an optional indirection by iterating from start to end index, so the optional indirection is always valid */

    Com_TxIPduGroup_EnterExclusiveArea();

    /* #10 Iterate over all related Tx ComIPdu of the passed ComIPduGroup */
    for(; txPduIndId < Com_GetTxPduInfoIndEndIdxOfIPduGroupInfo(subIpduGroupId); txPduIndId++)
    {
      boolean canBeStopped = TRUE;
      Com_TxPduInfoIndType txPduId = Com_GetTxPduInfoInd(txPduIndId);

      /* #20 Iterate over all related ComIPdu Groups */
      Com_IPduGroupInfoOfTxPduInfoIndEndIdxOfTxPduInfoType referencedIPduGroupInd = Com_GetIPduGroupInfoOfTxPduInfoIndStartIdxOfTxPduInfo(txPduId);       /* PRQA S 2843 */ /* MD_Com_2843_txPduId */
      for(; referencedIPduGroupInd < Com_GetIPduGroupInfoOfTxPduInfoIndEndIdxOfTxPduInfo(txPduId); referencedIPduGroupInd++)
      {
        Com_IPduGroupInfoOfTxPduInfoIndType referencedIPduGroup = Com_GetIPduGroupInfoOfTxPduInfoInd(referencedIPduGroupInd);
        /* #30 If at least one related ComIPdu Group is active, the ComIPdu remains active */
        if(Com_GetIPduGroupState(referencedIPduGroup) != COM_INACTIVE_IPDUGROUPSTATE)     /* COV_COM_TX_IPDU_GROUP_ASR3_XF */
        {
          canBeStopped = FALSE;
          break;
        }
      }
      /* #40 If all related ComIPdu Groups are stopped, deactivate TxComIPdu */
      if(canBeStopped == TRUE)    /* COV_COM_TX_IPDU_GROUP_ASR3_TX */
      {
        Com_DeactivateTxComIPdu(txPduId);
      }
      Com_IPduGroupHdlr_TxISRThresholdCheck(txIpduGroupISRLockCounterIdx);
    }

    Com_IPduGroupHdlr_ReloadTxIPduGroupISRLockCounter(txIpduGroupISRLockCounterIdx);
    Com_TxIPduGroup_ExitExclusiveArea();
  }
}
#endif

#if (COM_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_DeactivateRxComIPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DeactivateRxComIPdu(Com_RxPduInfoIterType rxPduId)
{
  /* #10 Set RxPduGrpActivate State to FALSE */
  Com_SetRxPduGrpActive(rxPduId, FALSE);        /* PRQA S 2842 */ /* MD_Com_2842 */ /* SBSW_COM_CSL02 */
# if (COM_RXTOUTINFOINDENDIDXOFRXPDUINFO == STD_ON)
  {
    /* - #20 Deactivate Rx Deadline Monitoring */
    Com_RxTOutInfoIndStartIdxOfRxPduInfoType idxRxTOutInfoInd = Com_GetRxTOutInfoIndStartIdxOfRxPduInfo(rxPduId);       /* PRQA S 2843 */ /* MD_Com_2843_rxPduId */
    for(; idxRxTOutInfoInd < Com_GetRxTOutInfoIndEndIdxOfRxPduInfo(rxPduId); idxRxTOutInfoInd++)        /* PRQA S 2843 */ /* MD_Com_2843_rxPduId */
    {
      Com_LMgt_DeactivateRxIpduDm(Com_GetRxTOutInfoInd(idxRxTOutInfoInd));      /* SBSW_COM_CSL02_CSL03 */
    }
  }
# endif
}
#endif

#if (COM_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_DeactivateTxComIPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DeactivateTxComIPdu(Com_TxPduInfoIterType txPduId)
{
  /* #50 Set the Tx Mode related parameters to the initial value */
  Com_TxModeHdlr_StopTxIpdus(txPduId);
# if (COM_CBKTXERRFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)
  /* #60 If the transmission of a ComIPdu was not confirmed, set WaitForConfirmation flags to the initial value and call any configured error notification */
  if(Com_IsWaitingForConfirmation(txPduId))
  {
    Com_PartitionsTxStructIdxOfTxPduInfoType partitionsIdx = Com_GetPartitionsTxStructIdxOfTxPduInfo(txPduId);
    Com_TxIPduGroupISRLockCounterIdxOfPartitionsTxStructType txIpduGroupISRLockCounterIdx = Com_GetTxIPduGroupISRLockCounterIdxOfPartitionsTxStruct(partitionsIdx);

    Com_IPduGroupHdlr_ReloadTxIPduGroupISRLockCounter(txIpduGroupISRLockCounterIdx);
    Com_TxIPduGroup_ExitExclusiveArea();

    Com_TxNHdlr_StopTxIpdu(txPduId);

    Com_TxIPduGroup_EnterExclusiveArea();
  }
# endif
# if (COM_CBKTXACKDEFFUNCPTRINDUSEDOFTXPDUINFO == STD_ON)
  /* #65 Clear HandleTxPduDeffered flag */
  Com_SetHandleTxPduDeferred(txPduId, FALSE);   /* SBSW_COM_IDXPARAM_INTERNALFCT_CSL02 */
# endif

# if (COM_TXTPINFOUSEDOFTXPDUINFO == STD_ON)
  if(Com_IsTxTpInfoUsedOfTxPduInfo(txPduId))    /* COV_COM_FEATURE_ALWAYS_TRUE_TX */
  {
    Com_ResetTxTpConnection(txPduId);
  }
# endif

  /* #70 Set the Tx ComIPdu Group state to inactive */
  Com_SetTxPduGrpActive(txPduId, FALSE);        /* SBSW_COM_CSL02 */
}
#endif

#if (COM_IPDUGROUPINFO == STD_ON)
/**********************************************************************************************************************
  Com_IpduGroupStopSubGroup
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_IpduGroupStopSubGroup(Com_IpduGroupIdType subIpduGroupId)
{
  /* #10 If the ComIPduGroup is ACTIVE set the ComIPduGroup to INACTIVE and stop its SubIPduGroups */
  if(Com_GetIPduGroupState(subIpduGroupId) != COM_INACTIVE_IPDUGROUPSTATE)
  {
    Com_SetIPduGroupState(subIpduGroupId, COM_INACTIVE_IPDUGROUPSTATE); /* SBSW_COM_CSL02 */
# if (COM_RXPDUINFOINDUSEDOFIPDUGROUPINFO == STD_ON)
    Com_SetRxPduGrpStateToStop(subIpduGroupId);
# endif
# if (COM_TXPDUINFOINDUSEDOFIPDUGROUPINFO == STD_ON)
    Com_SetTxPduGrpStateToStop(subIpduGroupId);
# endif
  }
}
#endif

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/
#if (COM_IPDUGROUPSTATE == STD_ON)
/**********************************************************************************************************************
  Com_IPduGroupHdlr_IPduGroupsInit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *-
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_IPduGroupsInit(void)
{
  Com_SizeOfIPduGroupStateType comIPduGroupId = 0;
  for(; comIPduGroupId < Com_GetSizeOfIPduGroupState(); comIPduGroupId++)
  {
    Com_SetIPduGroupState(comIPduGroupId, COM_INACTIVE_IPDUGROUPSTATE); /* SBSW_COM_CSL01 */
  }
}
#endif

#if (COM_ALWAYSACTIVERXCOMIPDUS == STD_ON)
/**********************************************************************************************************************
  Com_ActivateAlwaysOnRxComIPdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *-
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_ActivateAlwaysOnRxComIPdus(void)
{
  Com_AlwaysActiveRxComIPdusIterType idx;

  /* #10 Iterate over all Rx ComIPdus and activate Rx ComIPdu */
  for(idx = 0; idx < Com_GetSizeOfAlwaysActiveRxComIPdus(); idx++)
  {
    Com_ActivateRxComIPdu(Com_GetRxPduInfoIdxOfAlwaysActiveRxComIPdus(idx), TRUE);
  }
}
#endif

#if (COM_ALWAYSACTIVERXCOMIPDUS == STD_ON)
/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateAlwaysOnRxComIPdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateAlwaysOnRxComIPdus(void)
{
  Com_AlwaysActiveRxComIPdusIterType idx;
  Com_PartitionsAlwaysActiveRxComIPdusStructIterType alwaysActivePartitionsIdx;

  Com_RxIPduGroup_EnterExclusiveArea();
  /* #10 Iterate over all Rx ComIPdus without an assigned ComIPduGroup and deactivate Rx ComIPdu */
  for(idx = 0; idx < Com_GetSizeOfAlwaysActiveRxComIPdus(); idx++)
  {
    Com_PartitionsAlwaysActiveRxComIPdusStructIdxOfAlwaysActiveRxComIPdusType partitionIdx = Com_GetPartitionsAlwaysActiveRxComIPdusStructIdxOfAlwaysActiveRxComIPdus(idx);
    Com_RxIPduGroupISRLockCounterIdxOfPartitionsRxStructType rxIpduGroupISRLockCounterIdx = Com_GetRxIPduGroupISRLockCounterIdxOfPartitionsAlwaysActiveRxComIPdusStruct(partitionIdx);

    Com_DeactivateRxComIPdu(Com_GetRxPduInfoIdxOfAlwaysActiveRxComIPdus(idx));
    Com_IPduGroupHdlr_RxISRThresholdCheck(rxIpduGroupISRLockCounterIdx);
  }

  /* #20 Iterate over all always active partitions of Rx ComIPdus */
  for(alwaysActivePartitionsIdx = 0; alwaysActivePartitionsIdx < Com_GetSizeOfPartitionsAlwaysActiveRxComIPdusStruct(); alwaysActivePartitionsIdx++)
  {
    Com_RxIPduGroupISRLockCounterIdxOfPartitionsRxStructType rxIpduGroupISRLockCounterIdx = Com_GetRxIPduGroupISRLockCounterIdxOfPartitionsAlwaysActiveRxComIPdusStruct(alwaysActivePartitionsIdx);
    Com_IPduGroupHdlr_ReloadRxIPduGroupISRLockCounter(rxIpduGroupISRLockCounterIdx);
  }
  Com_RxIPduGroup_ExitExclusiveArea();
}
#endif

#if (COM_ALWAYSACTIVETXCOMIPDUS == STD_ON)
/**********************************************************************************************************************
  Com_ActivateAlwaysOnTxComIPdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_ActivateAlwaysOnTxComIPdus(void)
{
  Com_AlwaysActiveTxComIPdusIterType idx;

  /* #10 Iterate over all Tx ComIPdus and activate Tx ComIPdu */
  for(idx = 0; idx < Com_GetSizeOfAlwaysActiveTxComIPdus(); idx++)
  {
    Com_ActivateTxComIPdu(Com_GetTxPduInfoIdxOfAlwaysActiveTxComIPdus(idx), TRUE);
  }
}
#endif

#if (COM_ALWAYSACTIVETXCOMIPDUS == STD_ON)
/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateAlwaysOnTxComIPdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateAlwaysOnTxComIPdus(void)
{
  Com_AlwaysActiveTxComIPdusIterType idx;
  Com_PartitionsAlwaysActiveTxComIPdusStructIterType alwaysActivePartitionsIdx;

  Com_TxIPduGroup_EnterExclusiveArea();

  /* #10 Iterate over all Tx ComIPdus without an assigned ComIPduGroup and deactivate Tx ComIPdu */
  for(idx = 0; idx < Com_GetSizeOfAlwaysActiveTxComIPdus(); idx++)
  {
    Com_PartitionsAlwaysActiveTxComIPdusStructIdxOfAlwaysActiveTxComIPdusType partitionIdx = Com_GetPartitionsAlwaysActiveTxComIPdusStructIdxOfAlwaysActiveTxComIPdus(idx);
    Com_TxIPduGroupISRLockCounterIdxOfPartitionsAlwaysActiveTxComIPdusStructType txIpduGroupISRLockCounterIdx =
      Com_GetTxIPduGroupISRLockCounterIdxOfPartitionsAlwaysActiveTxComIPdusStruct(partitionIdx);

    Com_DeactivateTxComIPdu(Com_GetTxPduInfoIdxOfAlwaysActiveTxComIPdus(idx));
    Com_IPduGroupHdlr_TxISRThresholdCheck(txIpduGroupISRLockCounterIdx);
  }

  /* #20 Iterate over all always active partitions of Tx ComIPdus */
  for(alwaysActivePartitionsIdx = 0; alwaysActivePartitionsIdx < Com_GetSizeOfPartitionsAlwaysActiveTxComIPdusStruct(); alwaysActivePartitionsIdx++)
  {
    Com_TxIPduGroupISRLockCounterIdxOfPartitionsAlwaysActiveTxComIPdusStructType txIpduGroupISRLockCounterIdx =
      Com_GetTxIPduGroupISRLockCounterIdxOfPartitionsAlwaysActiveTxComIPdusStruct(alwaysActivePartitionsIdx);
    Com_IPduGroupHdlr_ReloadTxIPduGroupISRLockCounter(txIpduGroupISRLockCounterIdx);
  }
  Com_TxIPduGroup_ExitExclusiveArea();
}
#endif

#if (COM_RXPDUINFOINDUSEDOFIPDUGROUPINFO == STD_ON)
/**********************************************************************************************************************
  Com_IpduGroupStartRx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IpduGroupStartRx(Com_IpduGroupIdType subIpduGroupId, boolean Initialize)
{
  if(Com_IsPartitionsRxStructUsedOfIPduGroupInfo(subIpduGroupId)) /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
  {
    /* #10 Iterate over all related Rx ComIPdus */
    Com_RxPduInfoIndEndIdxOfIPduGroupInfoType rxPduIndId = Com_GetRxPduInfoIndStartIdxOfIPduGroupInfo(subIpduGroupId);
    Com_PartitionsRxStructIdxOfIPduGroupInfoType partitionsIdx = Com_GetPartitionsRxStructIdxOfIPduGroupInfo(subIpduGroupId);
    Com_RxIPduGroupISRLockCounterIdxOfPartitionsRxStructType rxIpduGroupISRLockCounterIdx = Com_GetRxIPduGroupISRLockCounterIdxOfPartitionsRxStruct(partitionsIdx);

    Com_RxIPduGroup_EnterExclusiveArea();

    for(; rxPduIndId < Com_GetRxPduInfoIndEndIdxOfIPduGroupInfo(subIpduGroupId); rxPduIndId++)
    {
      Com_RxPduInfoIndType rxPduId = Com_GetRxPduInfoInd(rxPduIndId);
      /* #20 If the Rx ComIPdu is inactive, activate and optionally initialize the Rx ComIPdu */
      if(!Com_IsRxPduGrpActive(rxPduId))  /* COV_COM_RX_IPDU_GROUP_ASR3_TX */
      {
        /* Start Rx I-PDU */
        Com_ActivateRxComIPdu(rxPduId, Initialize);
      }
      Com_IPduGroupHdlr_RxISRThresholdCheck(rxIpduGroupISRLockCounterIdx);
    }
    Com_IPduGroupHdlr_ReloadRxIPduGroupISRLockCounter(rxIpduGroupISRLockCounterIdx);
    Com_RxIPduGroup_ExitExclusiveArea();
  }  
}
#endif

#if (COM_TXPDUINFOINDUSEDOFIPDUGROUPINFO == STD_ON)
/**********************************************************************************************************************
  Com_IpduGroupStartTx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IpduGroupStartTx(Com_IpduGroupIdType subIpduGroupId, boolean Initialize)
{
  if(Com_IsPartitionsTxStructUsedOfIPduGroupInfo(subIpduGroupId)) /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
  {
    /* #10 Iterate over all related Tx ComIPdus */
    Com_TxPduInfoIndEndIdxOfIPduGroupInfoType txPduIndId = Com_GetTxPduInfoIndStartIdxOfIPduGroupInfo(subIpduGroupId);    /* PRQA S 2842 */ /* MD_Com_2842 */ /* 2842: Index subIpduGroupId originates from Com_IpduGroupStart(). It is retrieved via an optional indirection by iterating from start to end index, so the optional indirection is always valid */
    Com_PartitionsTxStructIdxOfIPduGroupInfoType partitionsIdx = Com_GetPartitionsTxStructIdxOfIPduGroupInfo(subIpduGroupId);
    Com_TxIPduGroupISRLockCounterIdxOfPartitionsTxStructType txIpduGroupISRLockCounterIdx = Com_GetTxIPduGroupISRLockCounterIdxOfPartitionsTxStruct(partitionsIdx);

    Com_TxIPduGroup_EnterExclusiveArea();

    for(; txPduIndId < Com_GetTxPduInfoIndEndIdxOfIPduGroupInfo(subIpduGroupId); txPduIndId++)
    {
      Com_TxPduInfoIndType txPduId = Com_GetTxPduInfoInd(txPduIndId);
      /* #20 If the ComIPdu state is inactive */
      if(!Com_IsTxPduGrpActive(txPduId))  /* COV_COM_TX_IPDU_GROUP_ASR3_TX */ 
      {
        /* #30 Activate and optionally initialize the Tx ComIPdu */
        Com_ActivateTxComIPdu(txPduId, Initialize);
      }

      Com_IPduGroupHdlr_TxISRThresholdCheck(txIpduGroupISRLockCounterIdx);
    }

    Com_IPduGroupHdlr_ReloadTxIPduGroupISRLockCounter(txIpduGroupISRLockCounterIdx);
    Com_TxIPduGroup_ExitExclusiveArea();
  }
}
#endif

#if (COM_IPDUGROUPINFO == STD_ON)
/**********************************************************************************************************************
  Com_IPduGroupHdlr_StopAllIpduGroups
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_StopAllIpduGroups(void)
{
  /* ! \note COM130 only specifies that the inter-ECU communication shall be stopped */
  /* #10 Iterate over all ComIPduGroups and stop them by a call of Com_IpduGroupStop (if ComOptimizedIPduGroupHandling is enabled) */
  Com_IpduGroupIdType comIPduGroupId = 0;
  for(; comIPduGroupId < Com_GetSizeOfIPduGroupInfo(); comIPduGroupId++)
  {
# if(COM_INVALIDHNDOFIPDUGROUPINFO == STD_ON)
    if(!Com_IsInvalidHndOfIPduGroupInfo(comIPduGroupId))
# endif
    {
      Com_IPduGroupHdlr_Stop(comIPduGroupId);
    }
  }
}
#endif

#if (COM_IPDUGROUPINFO == STD_ON)
/**********************************************************************************************************************
  Com_IPduGroupHdlr_Stop
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_Stop(Com_IpduGroupIdType IpduGroupId)
{
  /* #10 Iterate over all related SubIPduGroups and stop them */
  Com_IPduGroupInfoToSubIPduGroupsIndEndIdxOfIPduGroupInfoType subIpduGroupInd = Com_GetIPduGroupInfoToSubIPduGroupsIndStartIdxOfIPduGroupInfo(IpduGroupId);    /* PRQA S 2842 */ /* MD_Com_2842 */ /* 2842: Index IpduGroupId originates from Com_IpduGroupStop() where the index is check via the api pattern or from Com_IPduGroupHdlr_StopAllIpduGroups() where it is retrieved by iterating from 0 to SizeOfIPduGroupInfo and checking the validity. The index is therefore always valid */
  for(; subIpduGroupInd < Com_GetIPduGroupInfoToSubIPduGroupsIndEndIdxOfIPduGroupInfo(IpduGroupId); subIpduGroupInd++)
  {
    Com_IpduGroupIdType subIpduGroupId = (Com_IpduGroupIdType) Com_GetIPduGroupInfoToSubIPduGroupsInd(subIpduGroupInd);
    Com_IpduGroupStopSubGroup(subIpduGroupId);
  }
}
#endif


#if ((COM_RXPDUINFOINDUSEDOFIPDUGROUPINFO == STD_ON) && (COM_RXTOUTINFOINDENDIDXOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  Com_EnableReceptionDMsubGroup
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_EnableReceptionDMsubGroup(Com_IpduGroupIdType subIpduGroupId)
{
  /* #10 If the current deadline monitoring state is DMINACTIVE set the state to DMACTIVE */
  if(Com_GetIPduGroupState(subIpduGroupId) == COM_ACTIVE_DMINACTIVE_IPDUGROUPSTATE)
  {
    Com_SetIPduGroupState(subIpduGroupId, COM_ACTIVE_DMACTIVE_IPDUGROUPSTATE);  /* SBSW_COM_CSL02 */
    {
      /* #20 Iterate over all related Rx ComIPdus */
      Com_RxPduInfoIndEndIdxOfIPduGroupInfoType rxPduIndId = Com_GetRxPduInfoIndStartIdxOfIPduGroupInfo(subIpduGroupId);
      for(; rxPduIndId < Com_GetRxPduInfoIndEndIdxOfIPduGroupInfo(subIpduGroupId); rxPduIndId++)
      {
        Com_RxPduInfoIndType rxPduId = Com_GetRxPduInfoInd(rxPduIndId);
        /* #30 Iterate over all related Rx deadline monitoring state machines */
        Com_RxTOutInfoIndStartIdxOfRxPduInfoType idxRxTOutInfoInd = Com_GetRxTOutInfoIndStartIdxOfRxPduInfo(rxPduId);
        for(; idxRxTOutInfoInd < Com_GetRxTOutInfoIndEndIdxOfRxPduInfo(rxPduId); idxRxTOutInfoInd++)
        {
          Com_RxTOutInfoIndType idxRxTOutInfo = Com_GetRxTOutInfoInd(idxRxTOutInfoInd);
          /* #40 If the Rx deadline monitoring is INACTIVE, activate the Rx deadline monitoring */
          if((Com_GetRxPduDmState(idxRxTOutInfo) & COM_PURE_DM_STATE_MASK_RXPDUDMSTATE) == COM_INACTIVE_RXPDUDMSTATE)   /* COV_COM_IPDU_GROUP_DM_ASR3_TX */
          {
            Com_SetRxTOutCnt(idxRxTOutInfo, Com_GetFirstFactorOfRxTOutInfo(idxRxTOutInfo));     /* SBSW_COM_CSL02 */
            Com_SetRxPduDmState(idxRxTOutInfo, COM_ACTIVE_RXPDUDMSTATE);        /* SBSW_COM_CSL02 */
          }
        }
      }
    }
  }
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if ((COM_RXPDUINFOINDUSEDOFIPDUGROUPINFO == STD_ON) && (COM_RXTOUTINFOINDENDIDXOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  Com_DisableReceptionDMsubGroup
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
FUNC(void, COM_CODE) Com_DisableReceptionDMsubGroup(Com_IpduGroupIdType subIpduGroupId)
{
  /* #10 If the current deadline monitoring state is DMACTIVE set the state to DMINACTIVE */
  if(Com_GetIPduGroupState(subIpduGroupId) == COM_ACTIVE_DMACTIVE_IPDUGROUPSTATE)
  {
    Com_SetIPduGroupState(subIpduGroupId, COM_ACTIVE_DMINACTIVE_IPDUGROUPSTATE);        /* SBSW_COM_CSL02 */
    {
      /* #20 Iterate over all related Rx ComIPdus */
      Com_RxPduInfoIndEndIdxOfIPduGroupInfoType rxPduIndId = Com_GetRxPduInfoIndStartIdxOfIPduGroupInfo(subIpduGroupId);
      for(; rxPduIndId < Com_GetRxPduInfoIndEndIdxOfIPduGroupInfo(subIpduGroupId); rxPduIndId++)
      {
        boolean canBeStopped = TRUE;
        Com_RxPduInfoIndType rxPduId = Com_GetRxPduInfoInd(rxPduIndId);

        /* #30 Iterate over all related ComIPduGroups */
        Com_IPduGroupInfoOfRxPduInfoIndEndIdxOfRxPduInfoType referencedIPduGroupInd = Com_GetIPduGroupInfoOfRxPduInfoIndStartIdxOfRxPduInfo(rxPduId);
        for(; referencedIPduGroupInd < Com_GetIPduGroupInfoOfRxPduInfoIndEndIdxOfRxPduInfo(rxPduId); referencedIPduGroupInd++)
        {
          /* #40 If at least one ComIPduGroup state is DMACTIVE the deadline monitoring states remain active */
          Com_IPduGroupInfoOfRxPduInfoIndType referencedIPduGroup = Com_GetIPduGroupInfoOfRxPduInfoInd(referencedIPduGroupInd);
          if(Com_GetIPduGroupState(referencedIPduGroup) == COM_ACTIVE_DMACTIVE_IPDUGROUPSTATE)  /* COV_COM_RX_IPDU_GROUP_ASR3_XF */
          {
            canBeStopped = FALSE;
            break;
          }
        }
        /* #50 If all ComIPduGroups are in state DMINACTIVE */
        if(canBeStopped == TRUE)        /* COV_COM_RX_IPDU_GROUP_ASR3_TX */
        {
          /* #60 Iterate all Rx deadline monitoring states and deactivate them */
          Com_RxTOutInfoIndStartIdxOfRxPduInfoType idxRxTOutInfoInd = Com_GetRxTOutInfoIndStartIdxOfRxPduInfo(rxPduId); /* PRQA S 2843 */ /* MD_Com_2843_rxPduId */
          for(; idxRxTOutInfoInd < Com_GetRxTOutInfoIndEndIdxOfRxPduInfo(rxPduId); idxRxTOutInfoInd++)  /* PRQA S 2843 */ /* MD_Com_2843_rxPduId */
          {
            Com_LMgt_DeactivateRxIpduDm(Com_GetRxTOutInfoInd(idxRxTOutInfoInd));        /* SBSW_COM_CSL02_CSL03 */
          }
        }
      }
    }
  }
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (COM_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_RxIPduGroup_EnterExclusiveArea
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxIPduGroup_EnterExclusiveArea(void)
{
  /* #10 Enter the rx IPduGroups exclusive area */
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_RX();
}

/**********************************************************************************************************************
  Com_RxIPduGroup_ExitExclusiveArea
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxIPduGroup_ExitExclusiveArea(void)
{
  /* #10 Exit the rx IPduGroups exclusive area */
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_RX();
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_ReloadRxIPduGroupISRLockCounter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_ReloadRxIPduGroupISRLockCounter(Com_RxIPduGroupISRLockCounterIdxOfPartitionsRxStructType idx)
{
  /* #10 Reload the counter with the threshold value */
  Com_SetRxIPduGroupISRLockCounter(idx, Com_GetRxIPduGroupISRLockThreshold());  /* SBSW_COM_CSL_VAR_ACCESS */
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_RxISRThresholdCheck
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_RxISRThresholdCheck(Com_RxIPduGroupISRLockCounterIdxOfPartitionsRxStructType idx)
{
  if(Com_GetRxIPduGroupISRLockCounter(idx) > 0u)        /* COV_COM_THRESHOLD_TX */
  {
    Com_DecRxIPduGroupISRLockCounter(idx);      /* SBSW_COM_CSL_VAR_ACCESS */
  }
  else
  {
    Com_IPduGroupHdlr_ReloadRxIPduGroupISRLockCounter(idx);
    Com_RxIPduGroup_ExitExclusiveArea();
    Com_RxIPduGroup_EnterExclusiveArea();
  }
}
#endif

#if (COM_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_TxIPduGroup_EnterExclusiveArea
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxIPduGroup_EnterExclusiveArea(void)
{
  /* #10 Enter the tx IPduGroups exclusive area */
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_TX();
}

/**********************************************************************************************************************
  Com_TxIPduGroup_ExitExclusiveArea
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxIPduGroup_ExitExclusiveArea(void)
{
  /* #10 Exit the tx IPduGroups exclusive area */
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_TX();
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_ReloadTxIPduGroupISRLockCounter
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_ReloadTxIPduGroupISRLockCounter(Com_TxIPduGroupISRLockCounterIdxOfPartitionsTxStructType idx)
{
  /* #10 Reload the counter with the threshold value */
  Com_SetTxIPduGroupISRLockCounter(idx, Com_GetTxIPduGroupISRLockThreshold());  /* SBSW_COM_CSL_VAR_ACCESS */
}

/**********************************************************************************************************************
  Com_IPduGroupHdlr_TxISRThresholdCheck
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_TxISRThresholdCheck(Com_TxIPduGroupISRLockCounterIdxOfPartitionsTxStructType idx)
{
  if(Com_GetTxIPduGroupISRLockCounter(idx) > 0u)        /* COV_COM_THRESHOLD_TX */
  {
    Com_DecTxIPduGroupISRLockCounter(idx);      /* SBSW_COM_CSL_VAR_ACCESS */
  }
  else
  {
    Com_IPduGroupHdlr_ReloadTxIPduGroupISRLockCounter(idx);
    Com_TxIPduGroup_ExitExclusiveArea();
    Com_TxIPduGroup_EnterExclusiveArea();
  }
}
#endif

#define COM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/
/* *INDENT-OFF* */
/* Justifications for module-specific MISRA deviations:
   MD_Com_2843_rxPduId: MISRA2012-Rule18.1
     Reason:     The code is entirely safe, further investigation: rxPduId is always valid because it is a qualified CSL03-use-case Com_GetRxPduInfoInd.
								 The validity of RxPduInfoInd is checked with the iteration from start < end index.
     Risk:       No risk.
     Prevention: No prevention necessary.
   MD_Com_2843_txPduId: MISRA2012-Rule18.1
     Reason:     The code is entirely safe, further investigation: txPduId is always valid because it is a qualified CSL03-use-case Com_GetTxPduInfoInd.
								 The validity of TxPduInfoInd is checked with the iteration from start < end index.
     Risk:       No risk.
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

/**********************************************************************************************************************
 *  END OF FILE: Com_IPduGroupHdlr.c
 *********************************************************************************************************************/
