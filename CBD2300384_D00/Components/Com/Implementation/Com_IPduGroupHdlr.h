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
/*!        \file  Com_IPduGroupHdlr.h
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#if !defined (COM_IPDUGROUPHDLR_H)
# define COM_IPDUGROUPHDLR_H

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
# include "Com_Lcfg.h"

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION MACROS
**********************************************************************************************************************/
# if (COM_RXTOUTINFO == STD_ON)

/**********************************************************************************************************************
  Com_LMgt_DeactivateRxIpduDm
**********************************************************************************************************************/
/*! \brief        This function disables the Deadline Monitoring for the Rx I-Pdu.
                  To avoid data inconsistency ensure the Deadline Monitoring is
                  enabled for the Rx I-Pdu by calling Com_LMgt_RxIpduDmActive().
    \details      -
    \pre          -
    \param[in]    ComRxToutObjtId  ID of Rx Timeout object.
    \context      TASK
    \synchronous  TRUE
    \reentrant    TRUE, for different handles
**********************************************************************************************************************/
#  define Com_LMgt_DeactivateRxIpduDm(ComRxToutObjtId)  Com_SetRxPduDmState((ComRxToutObjtId), COM_INACTIVE_RXPDUDMSTATE)

# endif

/**********************************************************************************************************************
  GLOBAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL DATA PROTOTYPES (Com)
**********************************************************************************************************************/

/**********************************************************************************************************************
  GLOBAL FUNCTION PROTOTYPES
**********************************************************************************************************************/

# define COM_START_SEC_CODE
# include "MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

# if (COM_IPDUGROUPSTATE == STD_ON)
/**********************************************************************************************************************
  Com_IPduGroupHdlr_IPduGroupsInit
**********************************************************************************************************************/
/*! \brief        This function iterates over all ComIPduGroups and sets them to INACTIVE
    \details      -
    \pre          -
    \trace        SPEC-2736784, SPEC-2736788
    \context      TASK
    \synchronous  FALSE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_IPduGroupsInit(void);
# endif

# if (COM_ALWAYSACTIVERXCOMIPDUS == STD_ON)
/**********************************************************************************************************************
  Com_ActivateAlwaysOnRxComIPdus
**********************************************************************************************************************/
/*! \brief        This function activates Rx ComIPdus without an assigned ComIPduGroup
    \details      -
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_ActivateAlwaysOnRxComIPdus(void);
# endif

# if (COM_ALWAYSACTIVERXCOMIPDUS == STD_ON)
/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateAlwaysOnRxComIPdus
**********************************************************************************************************************/
/*! \brief        This function deactivates Rx ComIPdus without an assigned ComIPduGroup
    \details      -
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateAlwaysOnRxComIPdus(void);
# endif

# if (COM_ALWAYSACTIVETXCOMIPDUS == STD_ON)
/**********************************************************************************************************************
  Com_ActivateAlwaysOnTxComIPdus
**********************************************************************************************************************/
/*! \brief        This function activates Tx ComIPdus without an assigned ComIPduGroup
    \details      -
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_ActivateAlwaysOnTxComIPdus(void);
# endif

# if (COM_ALWAYSACTIVETXCOMIPDUS == STD_ON)
/**********************************************************************************************************************
  Com_IPduGroupHdlr_DeactivateAlwaysOnTxComIPdus
**********************************************************************************************************************/
/*! \brief        This function deactivates Tx ComIPdus without an assigned ComIPduGroup
    \details      -
    \pre          -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_DeactivateAlwaysOnTxComIPdus(void);
# endif

# if (COM_RXPDUINFOINDUSEDOFIPDUGROUPINFO == STD_ON)
/**********************************************************************************************************************
  Com_IpduGroupStartRx
**********************************************************************************************************************/
/*! \brief        Starts a preconfigured I-PDU Rx-subgroup.
    \details      -
    \pre          -
    \param[in]    subIpduGroupId ID of I-PDU subgroup to be started
    \param[in]    Initialize  Flag to request initialization of the data in the I-PDUs of this I-PDU group
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
    \warning      A call to Com_IpduGroupStartRx shall not be interrupted by another call to Com_IpduGroupStart, Com_EnableReceptionDM,
                  Com_DisableReceptionDM or a call to Com_IpduGroupStop.
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IpduGroupStartRx(Com_IpduGroupIdType subIpduGroupId, boolean Initialize);
# endif

# if (COM_TXPDUINFOINDUSEDOFIPDUGROUPINFO == STD_ON)
/**********************************************************************************************************************
  Com_IpduGroupStartTx
**********************************************************************************************************************/
/*! \brief        Starts a preconfigured I-PDU Tx-subgroup.
    \details      -
    \pre          -
    \param[in]    subIpduGroupId ID of I-PDU subgroup to be started
    \param[in]    Initialize  Flag to request initialization of the data in the I-PDUs of this I-PDU group
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
    \warning      A call to Com_IpduGroupStartTx shall not be interrupted by another call to Com_IpduGroupStart, Com_EnableReceptionDM,
                  Com_DisableReceptionDM or a call to Com_IpduGroupStop.
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IpduGroupStartTx(Com_IpduGroupIdType subIpduGroupId, boolean Initialize);
# endif

# if (COM_IPDUGROUPINFO == STD_ON)
/**********************************************************************************************************************
  Com_IPduGroupHdlr_StopAllIpduGroups
**********************************************************************************************************************/
/*! \brief        Iterates over all ComIPduGroups and stops them by a call of Com_IpduGroupStop (if ComOptimizedIPduGroupHandling is enabled)
    \details      -
    \pre          -
    \param[in]    -
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_StopAllIpduGroups(void);
# endif

# if (COM_IPDUGROUPINFO == STD_ON)
/**********************************************************************************************************************
  Com_IPduGroupHdlr_Stop
**********************************************************************************************************************/
/*! \brief        Iterates over all related SubIPduGroups and stops them
    \details      -
    \pre          -
    \param[in]    IpduGroupId ID of I-PDU group to be stopped
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
    \warning      It is only allowed to call this function after checking
                  the validity of IpduGroupId against Com_GetSizeOfIPduGroupInfo and Com_IsInvalidHndOfIPduGroupInfo
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_Stop(Com_IpduGroupIdType IpduGroupId);
# endif

# if ((COM_RXPDUINFOINDUSEDOFIPDUGROUPINFO == STD_ON) && (COM_RXTOUTINFOINDENDIDXOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  Com_EnableReceptionDMsubGroup
**********************************************************************************************************************/
/*! \brief        Enables the reception deadline monitoring for the I-PDUs within the given I-PDU subgroup.
    \details      -
    \pre          -
    \param[in]    subIpduGroupId ID of I-PDU subgroup where reception DM shall be enabled.
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
    \warning      A call to Com_EnableReceptionDMsubGroup shall not be interrupted by another call to Com_EnableReceptionDM,
                  Com_IpduGroupStop, Com_DisableReceptionDM or a call to Com_IpduGroupStart.
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_EnableReceptionDMsubGroup(Com_IpduGroupIdType subIpduGroupId);

/**********************************************************************************************************************
  Com_DisableReceptionDMsubGroup
**********************************************************************************************************************/
/*! \brief        Disables the reception deadline monitoring for the I-PDUs within the given I-PDU subgroup.
    \details      -
    \pre          -
    \param[in]    subIpduGroupId ID of I-PDU subgroup where reception DM shall be disabled.
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
    \warning      A call to Com_DisableReceptionDMsubGroup shall not be interrupted by another call to Com_DisableReceptionDM, Com_IpduGroupStop,
                  Com_EnableReceptionDM or a call to Com_IpduGroupStart.
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_DisableReceptionDMsubGroup(Com_IpduGroupIdType subIpduGroupId);
# endif

# if (COM_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_RxIPduGroup_EnterExclusiveArea
**********************************************************************************************************************/
/*! \brief        This function enters the rx IPduGroups exclusive area.
    \details      -
    \pre          -
    \trace        DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxIPduGroup_EnterExclusiveArea(void);

/**********************************************************************************************************************
  Com_RxIPduGroup_ExitExclusiveArea
**********************************************************************************************************************/
/*! \brief        This function exits the rx IPduGroups exclusive area.
    \details      -
    \pre          -
    \trace        DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_RxIPduGroup_ExitExclusiveArea(void);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_ReloadRxIPduGroupISRLockCounter
**********************************************************************************************************************/
/*! \brief        This function reloads the RxIPduGroupISRLockCounter.
    \details      -
    \pre          -
    \param[in]    idx    index of the indirection from the partitions rx struct to the ISRLock counter
    \trace        DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_ReloadRxIPduGroupISRLockCounter(Com_RxIPduGroupISRLockCounterIdxOfPartitionsRxStructType idx);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_RxISRThresholdCheck
**********************************************************************************************************************/
/*! \brief        This function checks if the ISR Lock must be opened
    \details      -
    \pre          -
    \param[in]    idx    index of the indirection from the partitions rx struct to the ISRLock counter
    \trace        DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_RxISRThresholdCheck(Com_RxIPduGroupISRLockCounterIdxOfPartitionsRxStructType idx);
# endif

# if (COM_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_TxIPduGroup_EnterExclusiveArea
**********************************************************************************************************************/
/*! \brief        This function enters the tx IPduGroups exclusive area.
    \details      -
    \pre          -
    \trace        DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxIPduGroup_EnterExclusiveArea(void);

/**********************************************************************************************************************
  Com_TxIPduGroup_ExitExclusiveArea
**********************************************************************************************************************/
/*! \brief        This function exits the tx IPduGroups exclusive area.
    \details      -
    \pre          -
    \trace        DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_TxIPduGroup_ExitExclusiveArea(void);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_ReloadTxIPduGroupISRLockCounter
**********************************************************************************************************************/
/*! \brief        This function reloads the TxIPduGroupISRLockCounter.
    \details      -
    \pre          -
    \param[in]    idx    index of the indirection from the partitions rx struct to the ISRLock counter
    \trace        DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_ReloadTxIPduGroupISRLockCounter(Com_TxIPduGroupISRLockCounterIdxOfPartitionsTxStructType idx);

/**********************************************************************************************************************
  Com_IPduGroupHdlr_TxISRThresholdCheck
**********************************************************************************************************************/
/*! \brief        This function checks if the ISR Lock must be opened
    \details      -
    \pre          -
    \param[in]    idx    index of the indirection from the partitions tx struct to the ISRLock counter
    \trace        DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context      TASK
    \synchronous  TRUE
    \reentrant    FALSE
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_IPduGroupHdlr_TxISRThresholdCheck(Com_TxIPduGroupISRLockCounterIdxOfPartitionsTxStructType idx);
# endif

# define COM_STOP_SEC_CODE
# include "MemMap.h"    /* PRQA S 5087 */       /* MD_MSR_MemMap */

#endif /* COM_IPDUGROUPHDLR_H */
/**********************************************************************************************************************
  END OF FILE: Com_IPduGroupHdlr.h
**********************************************************************************************************************/
