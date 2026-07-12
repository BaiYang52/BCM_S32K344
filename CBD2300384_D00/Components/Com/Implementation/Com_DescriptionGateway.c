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
/*!        \file  Com_DescriptionGateway.c
 *        \brief  MICROSAR Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/
#define COM_DESCRIPTIONGATEWAY_SOURCE

/**********************************************************************************************************************
 * MISRA JUSTIFICATION
 *********************************************************************************************************************/
/* PRQA  S 2741 EOF */ /* MD_CSL_ReducedToDefine */

/**********************************************************************************************************************
  INCLUDES
**********************************************************************************************************************/
#include "Com.h"
#include "Com_LLIf.h"
#include "Com_Timer.h"
#include "Com_DescriptionGateway.h"
#include "Com_TxModeHdlr.h"
#include "Com_Serializer.h"
#include "Com_CprQueue.h"
#include "SchM_Com.h"
#include "Com_CprQueueDescrGw.h"

/**********************************************************************************************************************
  LOCAL CONSTANT MACROS
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION MACROS
**********************************************************************************************************************/
#if !defined (COM_LOCAL_INLINE)
# define COM_LOCAL_INLINE LOCAL_INLINE
#endif

#if (COM_GWTXPDUDESCRIPTIONINFOUSEDOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_IsGwDestinationDescriptionTypeImmediate
**********************************************************************************************************************/
/*! \brief        This function checks if the type of the gateway description is immediate.
    \details      -
    \pre          -
    \param[in]    DescriptionType - Type of description
    \return       boolean
                    TRUE if type of description is immediate.
                    FALSE otherwise
**********************************************************************************************************************/
# define Com_IsGwDestinationDescriptionTypeImmediate(DescriptionType)       (boolean) ((((DescriptionType) & COM_IMMEDIATE_MASK_TYPEOFGWDESCRIPTIONACCESSINFO) == COM_IMMEDIATE_MASK_TYPEOFGWDESCRIPTIONACCESSINFO) ? TRUE : FALSE)

/**********************************************************************************************************************
  Com_IsGwDestinationDescriptionTypeTriggered
**********************************************************************************************************************/
/*! \brief        This function checks if the description has a TRIGGERED* transferProperty
    \details      -
    \pre          -
    \param[in]    DescriptionType - Type of description
    \return       boolean
                    TRUE if a TRIGGERED* transferProperty is configured
                    FALSE otherwise
**********************************************************************************************************************/
# define Com_IsGwDestinationDescriptionTypeTriggered(DescriptionType)       (((DescriptionType) & COM_TRIGGERED_MASK_TYPEOFGWDESCRIPTIONACCESSINFO) == COM_TRIGGERED_MASK_TYPEOFGWDESCRIPTIONACCESSINFO)

/**********************************************************************************************************************
  Com_IsGwDestinationDescriptionTypeOnChange
**********************************************************************************************************************/
/*! \brief        This function checks if the description has an ONCHANGE* transferProperty
    \details      -
    \pre          -
    \param[in]    DescriptionType - Type of description
    \return       boolean
                    TRUE if an ONCHANGE* transferProperty is configured
                    FALSE otherwise
**********************************************************************************************************************/
# define Com_IsGwDestinationDescriptionTypeOnChange(DescriptionType)        (((DescriptionType) & COM_ONCHANGE_MASK_TYPEOFGWDESCRIPTIONACCESSINFO) == COM_ONCHANGE_MASK_TYPEOFGWDESCRIPTIONACCESSINFO)

/**********************************************************************************************************************
  Com_IsGwDestinationDescriptionTypeWithoutRepetion
**********************************************************************************************************************/
/*! \brief        This function checks if the description has an WITHOUTREPETITION* transferProperty
    \details      -
    \pre          -
    \param[in]    DescriptionType - Type of description
    \return       boolean
                    TRUE if a WITHOUTREPETITION* transferProperty is configured
                    FALSE otherwise
**********************************************************************************************************************/
# define Com_IsGwDestinationDescriptionTypeWithoutRepetion(DescriptionType)  (((DescriptionType) & COM_WITHOUTREP_MASK_TYPEOFGWDESCRIPTIONACCESSINFO) == COM_WITHOUTREP_MASK_TYPEOFGWDESCRIPTIONACCESSINFO)
#endif

/**********************************************************************************************************************
  LOCAL DATA TYPES AND STRUCTURES
**********************************************************************************************************************/
#if (COM_GWTXPDUDESCRIPTIONINFOUSEDOFRXPDUINFO == STD_ON)
typedef enum
{
  COM_TRANSMIT_TRIGGER_NOTFULFILLED,
  COM_TRANSMIT_TRIGGER,
  COM_TRANSMIT_TRIGGERWITHOUTREP
} Com_DescriptionGwTriggerType;
#endif

#if (COM_GWTXPDUDESCRIPTIONINFOUSEDOFRXPDUINFO == STD_ON)
/*!
  \defgroup  Com_IsGwDestinationDescriptionTypeX Bit Flags
  \brief  These bit masks are used to retrieve the desired property out of GwDestinationDescriptionType
  \details  -
    \pre      -
    \{
*/
# define COM_IMMEDIATE_MASK_TYPEOFGWDESCRIPTIONACCESSINFO      8u    /**< bit mask to retrieve the immediate flag */
# define COM_TRIGGERED_MASK_TYPEOFGWDESCRIPTIONACCESSINFO      4u    /**< bit mask to retrieve the triggered flag */
# define COM_ONCHANGE_MASK_TYPEOFGWDESCRIPTIONACCESSINFO       2u    /**< bit mask to retrieve the onchange flag */
# define COM_WITHOUTREP_MASK_TYPEOFGWDESCRIPTIONACCESSINFO     1u    /**< bit mask to retrieve the without repetition flag */
/*!
  \}
*/
#endif

/**********************************************************************************************************************
  LOCAL DATA PROTOTYPES
**********************************************************************************************************************/

/**********************************************************************************************************************
  LOCAL FUNCTION PROTOTYPES
**********************************************************************************************************************/
#define COM_START_SEC_CODE
#include "MemMap.h"    /* PRQA S 5087 */        /* MD_MSR_MemMap */

#if (COM_GWTXPDUDESCRIPTIONINFOUSEDOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_ProcessDestinationDescription
**********************************************************************************************************************/
/*! \brief        This function processes a gateway destination description. More precisely, it copies the configured bits
 *                from the source to destination buffer if the transfer property conditions are satisfied.
    \details      -
    \pre          -
    \param[in]    sourceBuffer         Buffer of rxPdu that is received on gateway description event.
    \param[in]    gwDescrAccessIdx     Index of Com_GwDescriptionAccessInfo, which contains all informations of the description
    \return       Com_DescriptionGwTriggerType
                      COM_TRANSMIT_TRIGGER_NOTFULFILLED  transfer property conditions are not fulfilled
                      COM_TRANSMIT_TRIGGER               transfer property conditions are fulfilled and repetitions
                                                         are required
                      COM_TRANSMIT_TRIGGERWITHOUTREP     transfer property conditions are fulfilled and repetitions
                                                         are not required
    \context      TASK|ISR2
    \trace        CREQ-115135
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(Com_DescriptionGwTriggerType, COM_CODE) Com_DescriptionGw_ProcessDestinationDescription(P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) sourceBuffer,
                                                                                                             Com_GwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfoType gwDescrAccessIdx);

/**********************************************************************************************************************
  Com_DescriptionGw_CopyBuffer
**********************************************************************************************************************/
/*! \brief        This function copies the received content from source to destination buffer.
    \details      -
    \pre          -
    \param[in]    sourceBuffer         Buffer of rxPdu that is received on gateway description event.
    \param[in]    gwDescrAccessIdx     Index of Com_GwDescriptionAccessInfo, which contains all informations of the description
    \return       boolean
                    TRUE                 if content of destination buffer has changed.
                    FALSE                otherwise
    \context      TASK|ISR2
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DescriptionGw_CopyBuffer(P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) sourceBuffer,
                                                                     Com_GwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfoType gwDescrAccessIdx);

# if ((COM_DIRECTOFTXMODETRUE == STD_ON) || (COM_DIRECTOFTXMODEFALSE == STD_ON))
/**********************************************************************************************************************
  Com_DescriptionGw_Transmit
**********************************************************************************************************************/
/*! \brief        This function initiates the transmission of txPdu on gateway description event
    \details      -
    \pre          -
    \param[in]    ComTxPduId              Valid ID of TxPdu that should be sent
    \param[in]    isImmediate             true, if TxPdu should be sent immediately, otherwise transmission will be triggered
                                          in next main function cycle
    \param[in]    withoutRepetitionFlag   true, if no repetitions are required
    \context      TASK|ISR2
 *********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_Transmit(Com_TxPduInfoIterType ComTxPduId, boolean isImmediate, boolean withoutRepetitionFlag);
# endif
#endif

#if (COM_EXISTS_DIRECT_COPYTYPEOFGWDESCRIPTIONACCESSINFO == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_DirectCopy
**********************************************************************************************************************/
/*! \brief            This function directly copies the content from source to destination buffer.
    \details          -
    \param[in]        gwDescrAccessIdx          Gateway description access idx.
    \param[in]        sourceBuffer              Received source buffer referenced by source description.
    \param[in,out]    destinationBuffer         Tx Buffer where the received content is copied to.
    \param[in]        txBufferLength            Length of destinationBuffer
    \return           boolean
                        TRUE   if any bytes of destination have changed
                        FALSE  otherwise
    \pre              Caller has to ensure that sourceBuffer and destinationBuffer point to start byte of source and destination description.
    \context          TASK|ISR2
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DescriptionGw_DirectCopy(Com_GwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfoType gwDescrAccessIdx,
                                                                      P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) sourceBuffer,
                                                                      P2VAR(uint8, AUTOMATIC, COM_VAR_NO_INIT) destinationBuffer, Com_TxBufferLengthOfGwDescriptionAccessInfoType txBufferLength);
#endif

#if (COM_EXISTS_RIGHTSHIFT_COPYTYPEOFGWDESCRIPTIONACCESSINFO == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_RightShiftCopy
**********************************************************************************************************************/
/*! \brief              This function shifts the content of source buffer to the right and copies it to the destination buffer.
    \details            -
    \param[in]          sourceBuffer              Source buffer.
    \param[in,out]      destinationBuffer         Destination buffer.
    \param[in]          destinationBufferSize     Size of Destination Buffer.
    \param[in]          sourceStartBitIndex       Bit index within byte zero of source, where the content starts.
    \param[in]          bitOffset                 Offset to be shifted right.
    \param[in]          bitLength                 Number of bits to be copied.
    \param[in]          isBigEndian               true, if GwDescription is type of BIG_ENDIAN
    \return             boolean
                          TRUE                    if any bytes of destination have changed
                          FALSE                   otherwise
    \pre                For BigEndian description routing, caller has to ensure that sourceBuffer and destinationBuffer
                        point to end byte of source and destination description. For LittleEndian description routing,
                        caller has to ensure that sourceBuffer and destinationBuffer point to start byte of source
                        and destination description.
    \context            TASK|ISR2
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DescriptionGw_RightShiftCopy(P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) sourceBuffer,
                                                                          P2VAR(uint8, AUTOMATIC, COM_VAR_NO_INIT) destinationBuffer,
                                                                          uint16 destinationBufferSize, uint8 sourceStartBitIndex, uint8 bitOffset, uint16 bitLength, boolean isBigEndian);
#endif

#if (COM_EXISTS_LEFTSHIFT_COPYTYPEOFGWDESCRIPTIONACCESSINFO == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_LeftShiftCopy
**********************************************************************************************************************/
/*! \brief            This function shifts the content of source buffer to the left and copies it to the destination buffer.
    \details          -
    \param[in]        sourceBuffer              Source buffer.
    \param[in,out]    destinationBuffer         Destination buffer.
    \param[in]        destinationBufferSize     Size of Destination Buffer.
    \param[in]        sourceStartBitIndex       Bit index within byte zero of source, where the content starts.
    \param[in]        bitOffset                 Offset to be shifted left.
    \param[in]        bitLength                 Number of bits to be copied.
    \param[in]        isBigEndian               true, if GwDescription is type of BIG_ENDIAN
    \return           boolean
                        TRUE                    if any bytes of destination have changed
                        FALSE                   otherwise
    \pre              For BigEndian description routing, caller has to ensure that sourceBuffer and destinationBuffer
                      point to end byte of source and destination description. For LittleEndian description routing,
                      caller has to ensure that sourceBuffer and destinationBuffer point to start byte of source and
                      destination description.
    \context          TASK|ISR2
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DescriptionGw_LeftShiftCopy(P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) sourceBuffer,
                                                                         P2VAR(uint8, AUTOMATIC, COM_VAR_NO_INIT) destinationBuffer,
                                                                         uint16 destinationBufferSize, uint8 sourceStartBitIndex, uint8 bitOffset, uint16 bitLength, boolean isBigEndian);
#endif

#if (COM_RXUBMASKUSEDOFGWDESCRIPTIONACCESSINFO  == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_CheckUB
**********************************************************************************************************************/
/*! \brief        This function evaluates if the source description was updated.
    \details      -
    \pre          -
    \param[in]    gwDescrAccessIdx        Index of RxSigInfo
    \param[in]    PduInfoPtr              contains the SDU buffer and the SDU length.
    \return       boolean
                    TRUE                  if the source description was updated
                    FALSE                 otherwise
    \context      TASK|ISR2
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DescriptionGw_CheckUB(Com_GwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfoType gwDescrAccessIdx,
                                                                               P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr);
#endif

#if (COM_GWEVENTCACHE == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_EnterExclusiveArea
**********************************************************************************************************************/
/*! \brief    This function enters the gw description routing exclusive area.
    \details  -
    \pre      -
    \trace    DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context  TASK
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_EnterExclusiveArea(void);

/**********************************************************************************************************************
  Com_DescriptionGw_ExitExclusiveArea
**********************************************************************************************************************/
/*! \brief    This function exits the gw description routing exclusive area.
    \details  -
    \pre      -
    \param[in]    idx    index of the indirection from the main function route signals to the ISRLock counter
    \trace    DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context  TASK
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_ExitExclusiveArea(Com_GatewayDescriptionProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType idx);

/**********************************************************************************************************************
  Com_DescriptionGw_ThresholdCheck
**********************************************************************************************************************/
/*! \brief    This function checks if the ISR Lock must be opened
    \details  -
    \pre      -
    \param[in]    idx    index of the indirection from the main function route signals to the ISRLock counter
    \trace    DSGN-COM_ISRTHRESHOLD_CONCEPT
    \context  TASK
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_ThresholdCheck(Com_GatewayDescriptionProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType idx);
#endif

#if (COM_GWEVENTCACHE == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_ProcessGwEventCache
**********************************************************************************************************************/
/*! \brief        This function handles the processing of the GwEventCache. 
    \details      -
    \pre          -
    \param[in]    mainFunctionId                           ID of the calling route signals main function.
    \param[in]    gwDescriptionProcessingISRLockCounterIdx index of the indirection from the main function route signals to the ISRLock counter
    \context      TASK
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_ProcessGwEventCache(Com_MainFunctionRouteSignalsStructIterType mainFunctionId, Com_GatewayDescriptionProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType gwDescriptionProcessingISRLockCounterIdx);
#endif

#if(COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_ProcessCrossPartitionQueuesTx
**********************************************************************************************************************/
/*! \brief        This function handles the processing of the cross partition routing queues leading to this partition and initiates the transmission if the conditions of
 *                the configured transfer properties are satisfied.
    \details      -
    \pre          -
    \param[in]    mainFunctionId                           ID of the calling route signals main function.
    \param[in]    gwDescriptionProcessingISRLockCounterIdx index of the indirection from the main function route signals to the ISRLock counter
    \context      TASK
*****************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_ProcessCrossPartitionQueuesTx(Com_MainFunctionRouteSignalsStructIterType mainFunctionId, Com_GatewayDescriptionProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType gwDescriptionProcessingISRLockCounterIdx);
#endif

#if(COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_ProcessCrossPartitionRoutingTx
**********************************************************************************************************************/
/*! \brief        This function handles the tx side of cross partition gateway description events and initiates the transmission if the conditions of
 *                the configured transfer properties are satisfied.
    \details      -
    \pre          -
    \param[in]    routingQueueIndex   Idx of the cross partition routing queue
    \param[in]    readIdx             Index to the position in the queue where the data shall be read
    \context      TASK
*****************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_ProcessCrossPartitionRoutingTx(Com_CprQueueDescriptionRoutingIndType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx);
#endif

#if (COM_CPRQUEUEUSEDOFGWTXPDUDESCRIPTIONINFO == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_ProcessCrossPartitionRoutingRx
**********************************************************************************************************************/
/*! \brief        This function handles the rx side of the cross partition gateway description events which are written into the cross partition routing queue.
    \details      -
    \pre          -
    \param[in]    PduInfoPtr                contains the SDU buffer and the SDU length
    \param[in]    isCalledFromMainFunction  Info if the function is called from the main function or not. Set to true in the main function, set to false in RxIndication
    \param[in]    gwTxPduIdx                index of the GwTxPduDescriptionInfo struct
    \context      TASK
*****************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_ProcessCrossPartitionRoutingRx(P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr, boolean isCalledFromMainFunction, Com_GwTxPduDescriptionInfoEndIdxOfRxPduInfoType gwTxPduIdx);
#endif

#if (COM_GWTXPDUDESCRIPTIONINFOUSEDOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_ProcessPartitionLocalGwDescriptionEvent
**********************************************************************************************************************/
/*! \brief        This function handles the partition local gatway description events and initiates the transmission if the conditions of
 *                the configured transfer properties are satisfied.
    \details      -
    \pre          -
    \param[in]    PduInfoPtr                contains the SDU buffer and the SDU length
    \param[in]    gwTxPduIdx                index of the GwTxPduDescriptionInfo struct
    \context      TASK
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_ProcessPartitionLocalGwDescriptionEvent(P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr, Com_GwTxPduDescriptionInfoEndIdxOfRxPduInfoType gwTxPduIdx);
#endif

/**********************************************************************************************************************
  LOCAL FUNCTIONS
**********************************************************************************************************************/

#if (COM_GWTXPDUDESCRIPTIONINFOUSEDOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_ProcessDestinationDescription
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 ********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(Com_DescriptionGwTriggerType, COM_CODE) Com_DescriptionGw_ProcessDestinationDescription(P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) sourceBuffer,
                                                                                                             Com_GwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfoType gwDescrAccessIdx)
{
  Com_DescriptionGwTriggerType retVal = COM_TRANSMIT_TRIGGER_NOTFULFILLED;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

  Com_TypeOfGwDescriptionAccessInfoType descriptionType = Com_GetTypeOfGwDescriptionAccessInfo(gwDescrAccessIdx);

  boolean triggerCondition = TRUE;

  /* #10 Copy configured bytes into destination txPduBuffers */
  boolean hasContentChanged = Com_DescriptionGw_CopyBuffer(sourceBuffer, gwDescrAccessIdx);       /* SBSW_COM_FCTCALL_WITH_P2CONST */

  if(Com_IsGwDestinationDescriptionTypeOnChange(descriptionType))       /* TRIGGERED ON CHANGE */
  {
    triggerCondition = hasContentChanged;
  }

  /* #20 If transferProperty is not type of ON-CHANGE or content of description has changed */
  if(triggerCondition == TRUE)
  {
    /* #30 Set TRIGGERED and WITHOUTREPETITION FLAG if transfer property is TriggeredWithoutRepetition or TriggeredOnChangeWithoutRepetition */
    if(Com_IsGwDestinationDescriptionTypeWithoutRepetion(descriptionType))
    {
      retVal = COM_TRANSMIT_TRIGGERWITHOUTREP;
    }
    /* #40 otherwise set TRIGGERED FLAG if transfer property is Triggered */
    else if(Com_IsGwDestinationDescriptionTypeTriggered(descriptionType))
    {
      retVal = COM_TRANSMIT_TRIGGER;
    }
    else
    {
      /* #50 otherwise transfer property is assumed to be PENDING */
      retVal = COM_TRANSMIT_TRIGGER_NOTFULFILLED;
    }
  }

# if (COM_TXUBMASKUSEDOFGWDESCRIPTIONACCESSINFO == STD_ON)
  /* #60 If the destination description has a configured UB, set UB in the Tx ComIPdu Buffer. */
  if(Com_IsTxUbMaskUsedOfGwDescriptionAccessInfo(gwDescrAccessIdx) && Com_IsTxBufferUbUsedOfGwDescriptionAccessInfo(gwDescrAccessIdx) && (Com_GetTxUbMaskOfGwDescriptionAccessInfo(gwDescrAccessIdx) == COM_CONSTVALUEUINT8_TXUBMASKOFGWDESCRIPTIONACCESSINFO)) /* COV_COM_CSL04_DESTINATION_ENUM_CHECK */
  {
    Com_TxBufferEndIdxOfTxPduInfoType ubIdx = Com_GetTxBufferUbIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx);
    Com_SetTxBuffer(ubIdx, (Com_GetTxBuffer(ubIdx) | Com_GetConstValueUInt8(Com_GetTxUbMaskIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx))));   /* SBSW_COM_CSL03 */
  }
# endif

  return retVal;
}

 /**********************************************************************************************************************
  Com_DescriptionGw_CopyBuffer
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 ********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DescriptionGw_CopyBuffer(P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) sourceBuffer,
                                                                     Com_GwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfoType gwDescrAccessIdx)
{
  boolean hasContentChanged = FALSE;    /* PRQA S 2981 */ /* MD_MSR_RetVal */

  P2VAR(uint8, AUTOMATIC, COM_VAR_NO_INIT) destinationBuffer;
  boolean isBigEndian;

  uint8 sourceStartBitIndex = Com_GetGwSourceStartBitIndexOfGwDescriptionAccessInfo(gwDescrAccessIdx);
  uint8 bitOffset = Com_GetBitOffsetOfGwDescriptionAccessInfo(gwDescrAccessIdx);

  Com_TxBufferLengthOfGwDescriptionAccessInfoType destinationBufferSize = Com_GetTxBufferLengthOfGwDescriptionAccessInfo(gwDescrAccessIdx);

# if (COM_EXISTS_BIG_ENDIAN_ENDIANNESSOFGWDESCRIPTIONACCESSINFO == STD_ON)
  if(Com_GetEndiannessOfGwDescriptionAccessInfo(gwDescrAccessIdx) == COM_BIG_ENDIAN_ENDIANNESSOFGWDESCRIPTIONACCESSINFO)
  {
    destinationBuffer = Com_GetAddrTxBuffer(Com_GetTxBufferEndIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx) - 1u);
    isBigEndian = TRUE;
  }
  else
# endif
  {
    destinationBuffer = Com_GetAddrTxBuffer(Com_GetTxBufferStartIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx));
    isBigEndian = FALSE;
  }

  switch (Com_GetCopyTypeOfGwDescriptionAccessInfo(gwDescrAccessIdx))
  {     /* #10 If source and destination description are bit aligned respective to their relative start bit position, copy directly the desired bytes. */
# if (COM_EXISTS_DIRECT_COPYTYPEOFGWDESCRIPTIONACCESSINFO == STD_ON)
    case COM_DIRECT_COPYTYPEOFGWDESCRIPTIONACCESSINFO:
    {
      /* Assume direct copy is always little endian. */
      hasContentChanged = Com_DescriptionGw_DirectCopy(gwDescrAccessIdx, sourceBuffer, destinationBuffer, destinationBufferSize);       /* SBSW_COM_GW_DESCRIPTION_COPYBUFFER */
      break;
    }
# endif
# if (COM_EXISTS_RIGHTSHIFT_COPYTYPEOFGWDESCRIPTIONACCESSINFO == STD_ON)
      /* #20 Otherwise if relative start bit index of source is bigger than destination, perform right shift copy operation. */
    case COM_RIGHTSHIFT_COPYTYPEOFGWDESCRIPTIONACCESSINFO:
    {
      hasContentChanged = Com_DescriptionGw_RightShiftCopy(sourceBuffer, destinationBuffer, (uint16) destinationBufferSize, sourceStartBitIndex, bitOffset, Com_GetBitSizeOfGwDescriptionAccessInfo(gwDescrAccessIdx), isBigEndian);    /* SBSW_COM_GW_DESCRIPTION_COPYBUFFER */
      break;
    }
# endif
# if (COM_EXISTS_LEFTSHIFT_COPYTYPEOFGWDESCRIPTIONACCESSINFO == STD_ON)
      /* #30 Otherwise perform left shift copy operation. */
    case COM_LEFTSHIFT_COPYTYPEOFGWDESCRIPTIONACCESSINFO:
    {
      hasContentChanged = Com_DescriptionGw_LeftShiftCopy(sourceBuffer, destinationBuffer, (uint16) destinationBufferSize, sourceStartBitIndex, bitOffset, Com_GetBitSizeOfGwDescriptionAccessInfo(gwDescrAccessIdx), isBigEndian);     /* SBSW_COM_GW_DESCRIPTION_COPYBUFFER */
      break;
    }
# endif
    default:   /* COV_COM_MISRA */
      break;
  }

  COM_DUMMY_STATEMENT(sourceStartBitIndex);     /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  COM_DUMMY_STATEMENT(bitOffset);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  COM_DUMMY_STATEMENT(isBigEndian);     /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */

  return hasContentChanged;
}

# if ((COM_DIRECTOFTXMODETRUE == STD_ON) || (COM_DIRECTOFTXMODEFALSE == STD_ON))
/**********************************************************************************************************************
  Com_DescriptionGw_Transmit
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
 *
 *
 ***********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_Transmit(Com_TxPduInfoIterType ComTxPduId, boolean isImmediate, boolean withoutRepetitionFlag)
{

  /* #10 If txPdu should be processed immediately and minimum delay time has elapsed */
  if((isImmediate == TRUE)
#  if (COM_MINIMUMDELAYOFTXMODEINFO == STD_ON)
     && (Com_GetDelayTimeCnt(ComTxPduId) == 0u)
#  endif
    )   /* COV_COM_IMMEDIATE_DESTINATION_DESCRIPTION */
  {
    /* #20 If TxPdu is active */
    if(Com_IsTxPduGrpActive(ComTxPduId))
    {
      /* #30 If Tx Pdu is in DIRECT or MIXED TxModeMode */
      if(Com_TxModeHdlr_IsDirectTxMode(ComTxPduId) == TRUE)
      {
#  if ((COM_REPCNTOFTXMODETRUE == STD_ON) || (COM_REPCNTOFTXMODEFALSE == STD_ON))
        /* #40 Reload repetition counter and repetition cycle counter, if repetitions are required */
        if(withoutRepetitionFlag == FALSE)
        {
          Com_SetRepCnt(ComTxPduId, Com_TxModeHdlr_GetCurRepCnt(ComTxPduId));   /* SBSW_COM_IDXPARAM_INTERNALFCT_CSL02 */
          Com_SetRepCycleCnt(ComTxPduId, Com_TxModeHdlr_GetCurrentRepetitionPeriod(ComTxPduId));        /* SBSW_COM_IDXPARAM_INTERNALFCT_CSL02 */

#   if (COM_EXISTS_NORMAL_MODEOFTXTOUTINFO == STD_ON)
          /* #42 Reload Tx Deadline Monitoring timer if configured */
          if(0u < Com_GetRepCnt(ComTxPduId))
          {
            /* SPEC-2736884 */
            if(Com_IsTxTOutInfoUsedOfTxPduInfo(ComTxPduId))     /* COV_COM_FEATURE_ALWAYS_TRUE_TX */
            {
              Com_TxTOutInfoIdxOfTxPduInfoType idxTxTOutInfo = Com_GetTxTOutInfoIdxOfTxPduInfo(ComTxPduId);
              Com_SetTxTOutCnt(idxTxTOutInfo, Com_GetFactorOfTxTOutInfo(idxTxTOutInfo));        /* SBSW_COM_CSL02 */
            }
          }
          else
          {
            Com_TxModeHdlr_ReloadTxDlMonCounter(ComTxPduId);
          }
#   endif
        }
        else
#  endif
        {
#  if (COM_EXISTS_NORMAL_MODEOFTXTOUTINFO == STD_ON)
          Com_TxModeHdlr_ReloadTxDlMonCounter(ComTxPduId);
#  endif
        }
        /* #45 Set eventbased trigger Flag if configured */
#  if (COM_DIRECTTRIGGER == STD_ON)
        Com_SetDirectTrigger(ComTxPduId, TRUE); /* SBSW_COM_IDXPARAM_INTERNALFCT_CSL02 */
#  endif

        /* #50 Trigger transmit */
#  if (COM_RETRY_FAILED_TRANSMIT_REQUESTS == STD_ON)
        /* #60 If transmission was not successful and retry failed transmit feature is configured */
        if(Com_TxLLIf_Transmit(ComTxPduId) != E_OK)
        {
          /* #70 Schedule deferred transmission in next main function cycle */
          Com_TxModeHdlr_TriggerDeferredTransmitAndSetTimer(ComTxPduId, withoutRepetitionFlag);
        }
#  else
        (void) Com_TxLLIf_Transmit(ComTxPduId);
#  endif
      }
    }
  }
  /* #80 otherwise trigger deferred transmission in next main function cycle */
  else
  {
    Com_TxModeHdlr_TriggerDeferredTransmitAndSetTimer(ComTxPduId, withoutRepetitionFlag);
  }
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif
#endif

#if (COM_RXUBMASKUSEDOFGWDESCRIPTIONACCESSINFO  == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_CheckUB
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DescriptionGw_CheckUB(Com_GwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfoType gwDescrAccessIdx,
                                                                               P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr)
{
  boolean retVal = TRUE;        /* PRQA S 2981 */ /* MD_MSR_RetVal */
  /* #10 If the source description has an UpdateBit */
  if(Com_IsRxUbMaskUsedOfGwDescriptionAccessInfo(gwDescrAccessIdx) && Com_IsRxUbIdxUsedOfGwDescriptionAccessInfo(gwDescrAccessIdx))  /* COV_COM_FEATURE_ALWAYS_TRUE_UB */
  {
    /* #20 If the configured UpdateBit was not received mark the source description as updated */
    /* <= because a length is compared with an index */
    if(PduInfoPtr->SduLength <= Com_GetRxUbIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx)) 
    {
      /* Update-bit not received */
      retVal = TRUE;
    }
    /* #30 If the UpdateBit is not set, mark the source description as not updated */
    else if((Com_GetRxUbMaskOfGwDescriptionAccessInfo(gwDescrAccessIdx) == COM_CONSTVALUEUINT8_RXUBMASKOFGWDESCRIPTIONACCESSINFO) && (0u == (PduInfoPtr->SduDataPtr[Com_GetRxUbIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx)] & Com_GetConstValueUInt8(Com_GetRxUbMaskIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx)))))       /* COV_COM_CSL04_DESTINATION_ENUM_CHECK */
    {
      /* Update-bit not set */
      retVal = FALSE;
    }
    /* #40 Otherwise mark the source description as updated */
    else
    {
      /* Update-bit set */
      retVal = TRUE;
    }
  }
  return retVal;
}
#endif

#if (COM_EXISTS_DIRECT_COPYTYPEOFGWDESCRIPTIONACCESSINFO == STD_ON)
/**********************************************************************************************************************
   Com_DescriptionGw_DirectCopy
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 ********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DescriptionGw_DirectCopy(Com_GwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfoType gwDescrAccessIdx,
                                                                      P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) sourceBuffer,
                                                                      P2VAR(uint8, AUTOMATIC, COM_VAR_NO_INIT) destinationBuffer, Com_TxBufferLengthOfGwDescriptionAccessInfoType txBufferLength)
{
  boolean destinationHasChanged = FALSE;        /* PRQA S 2981 */ /* MD_MSR_RetVal */

  uint8 byteToBeCopied;

  Com_SourceDescriptionMaskType mask = Com_GetSourceDescriptionMask(Com_GetSourceDescriptionMaskIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx));

  /* Copy configured bytes into destination txPduBuffers and return true if any of the destination bytes has changed its content. */

  byteToBeCopied = (destinationBuffer[0] & COM_CAST2UINT8(~mask)) | (sourceBuffer[0] & mask);

  if(destinationBuffer[0] != byteToBeCopied)
  {
    destinationBuffer[0] = byteToBeCopied;      /* SBSW_COM_CSL03 */
    destinationHasChanged = TRUE;
  }

# if (COM_SOURCEDESCRIPTIONMASKENDMASKUSEDOFGWDESCRIPTIONACCESSINFO == STD_ON)
  if(txBufferLength > 1u)
  {
    if(Com_IsSourceDescriptionMaskEndMaskUsedOfGwDescriptionAccessInfo(gwDescrAccessIdx)) /* COV_COM_CSL03_ISUSED_OPTIONAL_INDIRECTION */
    {
      Com_TxBufferLengthOfGwDescriptionAccessInfoType byteIdx;
      Com_SourceDescriptionMaskType endMask = Com_GetSourceDescriptionMask(Com_GetSourceDescriptionMaskEndMaskIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx));
      /* Intermediate bytes can be copied directly. */
      for(byteIdx = 1u; byteIdx < (txBufferLength - 1u); byteIdx++)
      {
        if(destinationBuffer[byteIdx] != sourceBuffer[byteIdx])
        {
          destinationBuffer[byteIdx] = sourceBuffer[byteIdx];   /* SBSW_COM_CSL03 */
          destinationHasChanged = TRUE;
        }
      }

      /* Last byte has to be masked to avoid overwriting bits that do not belong to gateway description. */
      byteToBeCopied = (destinationBuffer[byteIdx] & COM_CAST2UINT8(~endMask)) | (sourceBuffer[byteIdx] & endMask);

      if(destinationBuffer[byteIdx] != sourceBuffer[byteIdx])
      {
        destinationBuffer[byteIdx] = byteToBeCopied;    /* SBSW_COM_CSL03 */
        destinationHasChanged = TRUE;
      }
    }
  }
# else
  COM_DUMMY_STATEMENT(txBufferLength);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
# endif

  return destinationHasChanged;
}
#endif

#if (COM_EXISTS_RIGHTSHIFT_COPYTYPEOFGWDESCRIPTIONACCESSINFO == STD_ON)
/*********************************************************************************************************************
   Com_DescriptionGw_RightShiftCopy
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 ********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DescriptionGw_RightShiftCopy(P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) sourceBuffer,
                                                                          P2VAR(uint8, AUTOMATIC, COM_VAR_NO_INIT) destinationBuffer,
                                                                          uint16 destinationBufferSize, uint8 sourceStartBitIndex, uint8 bitOffset, uint16 bitLength, boolean isBigEndian)
{

  sint32 i = 1;

  uint16 remainingBits;
  uint8 destinationStartBitIndex = COM_CAST2UINT8(sourceStartBitIndex - bitOffset);

  uint8 sourceEndBitIndex = COM_CAST2UINT8((sourceStartBitIndex + bitLength) - 1u) % 8u;
  uint8 destinationEndBitIndex = COM_CAST2UINT8((destinationStartBitIndex + bitLength) - 1u) % 8u;

  uint8 destinationMask = COM_CAST2UINT8(255u << destinationStartBitIndex);
  uint8 sourceMask_i = COM_CAST2UINT8(255u << sourceStartBitIndex);
  uint8 sourceMask_iP1 = COM_CAST2UINT8(~sourceMask_i);

  uint8 byteToCopy = 0u;        /* PRQA S 2981 */ /* MD_MSR_RetVal */

  boolean destinationHasChanged = FALSE;        /* PRQA S 2981 */ /* MD_MSR_RetVal */

  if((destinationStartBitIndex + bitLength) <= 8u)
  {
    /* Destination description covers single byte. Truncate leading bits of destination mask. */
    destinationMask &= COM_CAST2UINT8(255u >> COM_CAST2UINT8(8u - (destinationStartBitIndex + bitLength)));
    remainingBits = 0u;
  }
  else
  {
    /* Description covers multiple bytes. No truncation of mask is required. */
    uint8 bitToBeCopiedToFirstByte = COM_CAST2UINT8(8u - destinationStartBitIndex);
    remainingBits = (uint16) (bitLength - bitToBeCopiedToFirstByte);
  }

  if((sourceStartBitIndex + bitLength) <= 8u)
  {
    /* Source description covers single byte.  Truncate leading bits of source mask */
    sourceMask_i &= COM_CAST2UINT8(255u >> COM_CAST2UINT8(7u - sourceEndBitIndex));
    byteToCopy = COM_CAST2UINT8((sourceBuffer[0] & sourceMask_i) >> bitOffset);
  }
  else
  {
    /* Source description covers multiple bytes. */
    uint8 bitsOf_i = COM_CAST2UINT8(8u - sourceStartBitIndex);
    uint8 posOf_iP1 = destinationStartBitIndex + bitsOf_i;

    if(bitLength < bitOffset)
    {
      sourceMask_iP1 = COM_CAST2UINT8(~COM_CAST2UINT8(255u << (sourceEndBitIndex + 1u)));
    }

    byteToCopy = COM_CAST2UINT8((sourceBuffer[0] & sourceMask_i) >> bitOffset);
    byteToCopy |= COM_CAST2UINT8((sourceBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] & sourceMask_iP1) << posOf_iP1);
  }

  byteToCopy = (destinationBuffer[0] & COM_CAST2UINT8(~destinationMask)) | (byteToCopy & destinationMask);

  if(destinationBuffer[0] != byteToCopy)
  {
    destinationHasChanged = TRUE;
    destinationBuffer[0] = byteToCopy;  /* SBSW_COM_CSL03 */
  }

  sourceMask_i = COM_CAST2UINT8(255u << bitOffset);
  sourceMask_iP1 = COM_CAST2UINT8(~sourceMask_i);

  while((remainingBits > 7u) && (i < (sint32) destinationBufferSize))   /* COV_COM_GWDESCRIPTION_RTCHECK */
  {
    /* Copy full bytes without truncation. */
    byteToCopy = COM_CAST2UINT8((sourceBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] & sourceMask_i) >> bitOffset);
    byteToCopy |= COM_CAST2UINT8((sourceBuffer[Com_NBITNBYTE_GetEndiannesIndex(i + 1, isBigEndian)] & sourceMask_iP1) << COM_CAST2UINT8(8u - bitOffset));

    if(destinationBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] != byteToCopy)
    {
      destinationHasChanged = TRUE;
      destinationBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] = byteToCopy;  /* SBSW_COM_GWDESCRIPTION_RTCHECK */
    }

    i++;
    remainingBits = (uint16) (remainingBits - 8u);
  }

  if((remainingBits > 0u) && (i < (sint32) destinationBufferSize))      /* COV_COM_GWDESCRIPTION_RTCHECK */
  {     /* if source covers multiple bytes */
    destinationMask = COM_CAST2UINT8(255u >> COM_CAST2UINT8(7u - destinationEndBitIndex));
    if(destinationEndBitIndex > sourceEndBitIndex)
    {
      /* Copy i and i+1 */
      sourceMask_iP1 = COM_CAST2UINT8(~(255u << (sourceEndBitIndex + 1u)));
      byteToCopy = COM_CAST2UINT8((sourceBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] & sourceMask_i) >> bitOffset);
      byteToCopy |= COM_CAST2UINT8((sourceBuffer[Com_NBITNBYTE_GetEndiannesIndex(i + 1, isBigEndian)] & sourceMask_iP1) << COM_CAST2UINT8(8u - bitOffset));
    }
    else
    {
      /* Copy only i */
      byteToCopy = COM_CAST2UINT8((sourceBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] & sourceMask_i) >> bitOffset);
    }

    byteToCopy = (destinationBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] & COM_CAST2UINT8(~destinationMask)) | (byteToCopy & destinationMask);
    if(destinationBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] != byteToCopy)
    {
      destinationHasChanged = TRUE;
      destinationBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] = byteToCopy;  /* SBSW_COM_GWDESCRIPTION_RTCHECK */
    }
  }

  return destinationHasChanged;
}       /* PRQA S 6010, 6060 */ /* MD_MSR_STPTH, MD_MSR_STPAR */
#endif

#if (COM_EXISTS_LEFTSHIFT_COPYTYPEOFGWDESCRIPTIONACCESSINFO == STD_ON)
/*********************************************************************************************************************
   Com_DescriptionGw_LeftShiftCopy
 *********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 ********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(boolean, COM_CODE) Com_DescriptionGw_LeftShiftCopy(P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) sourceBuffer,
                                                                         P2VAR(uint8, AUTOMATIC, COM_VAR_NO_INIT) destinationBuffer,
                                                                         uint16 destinationBufferSize, uint8 sourceStartBitIndex, uint8 bitOffset, uint16 bitLength, boolean isBigEndian)
{
  boolean destinationHasChanged = FALSE;        /* PRQA S 2981 */ /* MD_MSR_RetVal */

  uint8 byteToBeCopied;

  sint32 i = 1;

  uint8 destinationStartBitIndex = sourceStartBitIndex + bitOffset;

  uint8 sourceEndBitIndex = COM_CAST2UINT8((sourceStartBitIndex + bitLength) - 1u) % 8u;
  uint8 destinationEndBitIndex = COM_CAST2UINT8((destinationStartBitIndex + bitLength) - 1u) % 8u;

  uint16 remainingBits = bitLength;

  uint8 snippetMask = COM_CAST2UINT8(255u >> bitOffset);

  uint8 destinationMask = COM_CAST2UINT8(255u << destinationStartBitIndex);

  uint8 sourceMask = COM_CAST2UINT8(255u << sourceStartBitIndex) & COM_CAST2UINT8(255u >> bitOffset);

  if((destinationStartBitIndex + bitLength) <= 8u)
  {
    /* Destination description covers single byte. Destinations mask needs to be truncated. */
    destinationMask &= COM_CAST2UINT8(255u >> COM_CAST2UINT8(7u - destinationEndBitIndex));
    sourceMask = COM_CAST2UINT8(destinationMask >> bitOffset);

    remainingBits = 0u;
  }
  else
  {
    /* Destination description covers multiple bytes. */
    remainingBits = COM_CAST2UINT8(remainingBits - COM_CAST2UINT8(8u - destinationStartBitIndex));
  }

  byteToBeCopied = (destinationBuffer[0] & COM_CAST2UINT8(~destinationMask)) | COM_CAST2UINT8((sourceBuffer[0] & sourceMask) << bitOffset);

  if(*destinationBuffer != byteToBeCopied)
  {
    destinationHasChanged = TRUE;
    *destinationBuffer = byteToBeCopied;        /* SBSW_COM_CSL03 */
  }

  while((remainingBits > 7u) && (i < (sint32) destinationBufferSize))   /* COV_COM_GWDESCRIPTION_RTCHECK */
  {
    /* Copy full bytes to destination description. Fill leading part of destination with trailing bits of i-th source byte and copy trailing part of destination with leading bits of (i-1)-th source byte. */
    byteToBeCopied = COM_CAST2UINT8((sourceBuffer[Com_NBITNBYTE_GetEndiannesIndex(i - 1, isBigEndian)] & COM_CAST2UINT8(~snippetMask)) >> COM_CAST2UINT8(8u - bitOffset));
    byteToBeCopied |= COM_CAST2UINT8((sourceBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] & snippetMask) << bitOffset);

    if(destinationBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] != byteToBeCopied)
    {
      destinationHasChanged = TRUE;
      destinationBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] = byteToBeCopied;      /* SBSW_COM_GWDESCRIPTION_RTCHECK */
    }

    i++;
    remainingBits = COM_CAST2UINT8(remainingBits - 8u);
  }

  if((remainingBits > 0u) && (i < (sint32) destinationBufferSize))      /* COV_COM_GWDESCRIPTION_RTCHECK */
  {     /* if source covers multiple bytes */
    uint8 finalByteMask;
    destinationMask = COM_CAST2UINT8(255u << (destinationEndBitIndex + 1u));

    byteToBeCopied = destinationBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)];
    if(destinationEndBitIndex < sourceEndBitIndex)
    {
      /* Next Byte */
      finalByteMask = COM_CAST2UINT8(COM_CAST2UINT8(255u >> COM_CAST2UINT8(8u - (uint8) remainingBits)) << COM_CAST2UINT8((1u + sourceEndBitIndex) - (uint8) remainingBits));
    }
    else
    {
      /* Same byte */
      uint8 sourceMaske = COM_CAST2UINT8(255u >> COM_CAST2UINT8(7u - sourceEndBitIndex));
      finalByteMask = COM_CAST2UINT8(~snippetMask);
      byteToBeCopied = (byteToBeCopied & destinationMask) | COM_CAST2UINT8((sourceBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] & sourceMaske) << bitOffset);

      destinationMask = COM_CAST2UINT8(255u << bitOffset);
    }
    byteToBeCopied = (byteToBeCopied & destinationMask) | COM_CAST2UINT8((sourceBuffer[Com_NBITNBYTE_GetEndiannesIndex(i - 1, isBigEndian)] & finalByteMask) >> COM_CAST2UINT8(8u - bitOffset));

    if(destinationBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] != byteToBeCopied)
    {
      destinationHasChanged = TRUE;
      destinationBuffer[Com_NBITNBYTE_GetEndiannesIndex(i, isBigEndian)] = byteToBeCopied;      /* SBSW_COM_GWDESCRIPTION_RTCHECK */
    }
  }

  return destinationHasChanged;
}       /* PRQA S 6060 */ /* MD_MSR_STPAR */
#endif

#if (COM_GWEVENTCACHE == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_EnterExclusiveArea
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_EnterExclusiveArea(void)
{
  /* #10 Enter the gw description routing exclusive area */
  SchM_Enter_Com_COM_EXCLUSIVE_AREA_BOTH();
}

/**********************************************************************************************************************
  Com_DescriptionGw_ExitExclusiveArea
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_ExitExclusiveArea(Com_GatewayDescriptionProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType idx)
{
  /* #05 Reload the counter with the threshold value */
  Com_SetGatewayDescriptionProcessingISRLockCounter(idx, Com_GetGatewayDescriptionProcessingISRLockThreshold());        /* SBSW_COM_CSL_VAR_ACCESS */
  /* #10 Exit the gw description routing exclusive area */
  SchM_Exit_Com_COM_EXCLUSIVE_AREA_BOTH();
}

/**********************************************************************************************************************
  Com_DescriptionGw_ThresholdCheck
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_ThresholdCheck(Com_GatewayDescriptionProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType idx)
{
  if(Com_GetGatewayDescriptionProcessingISRLockCounter(idx) > 0u)
  {
    Com_DecGatewayDescriptionProcessingISRLockCounter(idx);     /* SBSW_COM_CSL_VAR_ACCESS */
  }
  else
  {
    Com_DescriptionGw_ExitExclusiveArea(Com_GetGatewayDescriptionProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStruct(idx));
    Com_DescriptionGw_EnterExclusiveArea();
  }
}
#endif

#if (COM_GWEVENTCACHE == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_ProcessGwEventCache
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_ProcessGwEventCache(Com_MainFunctionRouteSignalsStructIterType mainFunctionId, Com_GatewayDescriptionProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType gwDescriptionProcessingISRLockCounterIdx)
{
  /* If the Gw Event Cache is used */
  if(Com_IsGwEventCacheIndexUsedOfMainFunctionRouteSignalsStruct(mainFunctionId))       /* COV_COM_GWEVENTCACHE_OF_MAINFUNCROUTE_USED_TX */
  {
    Com_GwEventCacheIndexIdxOfMainFunctionRouteSignalsStructType gwEventCacheIndexIdx = Com_GetGwEventCacheIndexIdxOfMainFunctionRouteSignalsStruct(mainFunctionId);

    /* Iterate over all ComIPdus inside the gw event cache */
    while(Com_GetGwEventCacheIdxOfGwEventCacheIndex(gwEventCacheIndexIdx) > Com_GetGwEventCacheStartIdxOfMainFunctionRouteSignalsStruct(mainFunctionId))
    {
      Com_RxPduInfoIterType rxPdu;

      Com_DecGwEventCacheIdxOfGwEventCacheIndex(gwEventCacheIndexIdx);  /* SBSW_COM_CSL_VAR_ACCESS */

      rxPdu = (Com_RxPduInfoIterType) Com_GetRxPduInfoIdxOfGwEventCache(Com_GetGwEventCacheIdxOfGwEventCacheIndex(gwEventCacheIndexIdx));

      if((rxPdu < Com_GetSizeOfRxPduInfo()) && Com_IsHandleRxDeferredGwDescriptionUsedOfRxPduInfo(rxPdu))       /* COV_COM_INDEX_IN_BOUNDS_CHECK */
      {
        Com_HandleRxDeferredGwDescriptionType gwDeferredHandleSize = Com_GetHandleRxDeferredGwDescription(Com_GetHandleRxDeferredGwDescriptionIdxOfRxPduInfo(rxPdu));    

        /* If gw flag has not been reset */
        if(gwDeferredHandleSize > 0u)
        {
          PduInfoType PduInfo;
          PduInfo.SduLength = (PduLengthType) (gwDeferredHandleSize - 1u);
          PduInfo.SduDataPtr = Com_GetAddrRxDefPduBuffer(Com_GetRxDefPduBufferStartIdxOfRxPduInfo(rxPdu));

          /* Process gateway description event */
          Com_GwDescriptionEvent(rxPdu, &PduInfo, TRUE); /* SBSW_COM_FCTCALL_STACKPTR */
        }

        /* Reset deferred gw description flag */
        Com_SetHandleRxDeferredGwDescription(Com_GetHandleRxDeferredGwDescriptionIdxOfRxPduInfo(rxPdu), 0);     /* SBSW_COM_CSL03 */
      }
      Com_DescriptionGw_ThresholdCheck(gwDescriptionProcessingISRLockCounterIdx);
    }
  }
  COM_DUMMY_STATEMENT(gwDescriptionProcessingISRLockCounterIdx);   /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
}
#endif

#if(COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_ProcessCrossPartitionQueuesTx
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
**********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_ProcessCrossPartitionQueuesTx(Com_MainFunctionRouteSignalsStructIterType mainFunctionId, Com_GatewayDescriptionProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType gwDescriptionProcessingISRLockCounterIdx)
{
  /* If cross partition routing queues leading to this partition are used */
  if(Com_IsPartitionsTxStructUsedOfMainFunctionRouteSignalsStruct(mainFunctionId) && Com_IsCprQueueDescriptionRoutingIndUsedOfPartitionsTxStruct(Com_GetPartitionsTxStructIdxOfMainFunctionRouteSignalsStruct(mainFunctionId))) /* COV_COM_CROSSPARTITIONQUEUEUSED_TX */
  {
    /* Iterate over cross partition queues leading to this partition */
    Com_CprQueueDescriptionRoutingIndStartIdxOfPartitionsTxStructType routingQueueIndIndex = Com_GetCprQueueDescriptionRoutingIndStartIdxOfPartitionsTxStruct(Com_GetPartitionsTxStructIdxOfMainFunctionRouteSignalsStruct(mainFunctionId));
    for(; routingQueueIndIndex < Com_GetCprQueueDescriptionRoutingIndEndIdxOfPartitionsTxStruct(Com_GetPartitionsTxStructIdxOfMainFunctionRouteSignalsStruct(mainFunctionId));routingQueueIndIndex++)
    {
      Com_CprQueueDescriptionRoutingIndType routingQueueIndex = Com_GetCprQueueDescriptionRoutingInd(routingQueueIndIndex);
      Com_CprQueueBufferReadIdxOfCprQueueType readIdx = 0u;
      Com_CprQueueBufferLengthOfCprQueueType readThreshold = Com_GetCprQueueBufferLengthOfCprQueue(routingQueueIndex);

      /* Reset NumberOfBytesRead */
      Com_CprQueue_ResetNumberOfBytesRead(routingQueueIndex);

      /* While cross partition queue has stored elements, read data from queue */
      while(Com_CprQueue_GetReadIdx(routingQueueIndex, &readIdx) == E_OK)      /* SBSW_COM_FCTCALL_STACKPTR */
      {
        if (Com_CprQueue_ReadFromCprQueueDirectly(readIdx) == COM_CPRQUEUE_HEADERVALUE_DESCRIPTION)  /* COV_COM_DEFENSIVE_PROGRAMMING_CROSSPARTITIONDSCRIPTIONROUTINGQUEUE */
        {
          /* Process cross partition description routing tx side*/
          Com_DescriptionGw_ProcessCrossPartitionRoutingTx(routingQueueIndex, readIdx);       /* SBSW_COM_FCTCALL_STACKPTR */
        
          /* Update the read index */      
          Com_CprQueue_UpdateReadIdx(routingQueueIndex);
        }

        /* Abort processing if number of read bytes increases the the queue size */
        if(Com_GetCprQueueBufferNumberOfBytesReadIdxOfCprQueue(routingQueueIndex) > readThreshold) /* COV_COM_DEFENSIVE_PROGRAMMING_CROSSPARTITIONQUEUE */
        {
          break;
        }

        Com_DescriptionGw_ThresholdCheck(gwDescriptionProcessingISRLockCounterIdx);
      }
    }
  }
  COM_DUMMY_STATEMENT(gwDescriptionProcessingISRLockCounterIdx);   /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
}
#endif

#if (COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_ProcessCrossPartitionRoutingTx
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
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_ProcessCrossPartitionRoutingTx(Com_CprQueueDescriptionRoutingIndType routingQueueIndex, Com_CprQueueBufferReadIdxOfCprQueueType readIdx)
{
  Com_GwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfoType gwDescrAccessIdx = 0u;
  Com_TxPduInfoIterType ComTxPduId = 0u;

  boolean sendTrigger = FALSE;                /* PRQA S 2981 */ /* MD_MSR_RetVal */
  boolean withoutRepetitionFlag = TRUE;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Com_GwTxPduDescriptionInfoIterType gwTxPduIdx = 0u;
        
  PduInfoType sourceBuffer;
  sourceBuffer.SduLength = 0u;
  sourceBuffer.SduDataPtr = NULL_PTR;

  /* Read data from queue */
  Com_CprQueueDescrGw_ReadData(routingQueueIndex, &gwTxPduIdx, &sourceBuffer, readIdx); /* SBSW_COM_FCTCALL_STACKPTR */

  if(gwTxPduIdx < Com_GetSizeOfGwTxPduDescriptionInfo())   /* COV_COM_INDEX_IN_BOUNDS_CHECK */
  {
    gwDescrAccessIdx = Com_GetGwDescriptionAccessInfoStartIdxOfGwTxPduDescriptionInfo(gwTxPduIdx);
    ComTxPduId = Com_GetTxPduInfoIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx);

    /* Process all descriptions that correspond to the same destination txPdu */
    for(; gwDescrAccessIdx < Com_GetGwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfo(gwTxPduIdx); gwDescrAccessIdx++)
    {
      /* Process Description Routing */
      P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) sourceBufferGwDescrAccess = &(sourceBuffer.SduDataPtr[Com_GetSourceStartByteOffsetOfGwDescriptionAccessInfo(gwDescrAccessIdx)]);
      switch (Com_DescriptionGw_ProcessDestinationDescription(sourceBufferGwDescrAccess, gwDescrAccessIdx))     /* SBSW_COM_FCTCALL_WITH_P2CONST */
      {
        case COM_TRANSMIT_TRIGGERWITHOUTREP:
          sendTrigger = TRUE;       /* Set transmit flag if at least one description fulfills the transmit condition */
          break;
        case COM_TRANSMIT_TRIGGER:
          sendTrigger = TRUE;
          withoutRepetitionFlag = FALSE;    /* Set repetition flag if at least one description requires repetitve transmit */
          break;
        default:   /* COV_COM_MISRA */
          break;
      }
    }

# if (COM_GWTIMEOUTINFO == STD_ON)
    /* Handle Gw Timeout if configured */
    Com_GwTout_Event(ComTxPduId);
# endif
# if ((COM_DIRECTOFTXMODETRUE == STD_ON) || (COM_DIRECTOFTXMODEFALSE == STD_ON))
    /* If transmit condition is fulfilled (transmit flag is set) */
    if(sendTrigger == TRUE)
    {
      Com_TypeOfGwDescriptionAccessInfoType typeOfDescription = Com_GetTypeOfGwDescriptionAccessInfo(Com_GetGwDescriptionAccessInfoStartIdxOfGwTxPduDescriptionInfo(gwTxPduIdx));
      /* Trigger the transmission of the PDU */
      Com_DescriptionGw_Transmit(ComTxPduId, Com_IsGwDestinationDescriptionTypeImmediate(typeOfDescription), withoutRepetitionFlag);        /* COV_COM_IMMEDIATE_DESTINATION_DESCRIPTION */
    }
# endif
  }
  
  COM_DUMMY_STATEMENT(withoutRepetitionFlag); /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
  COM_DUMMY_STATEMENT(sendTrigger);   /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
}       /* PRQA S 6080 */ /* MD_MSR_STMIF */
#endif

#if (COM_CPRQUEUEUSEDOFGWTXPDUDESCRIPTIONINFO == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_ProcessCrossPartitionRoutingRx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_ProcessCrossPartitionRoutingRx(P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr, boolean isCalledFromMainFunction, Com_GwTxPduDescriptionInfoEndIdxOfRxPduInfoType gwTxPduIdx)
{
  Com_GwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfoType gwDescrAccessIdx = Com_GetGwDescriptionAccessInfoStartIdxOfGwTxPduDescriptionInfo(gwTxPduIdx);  

  /* Process all descriptions that correspond to the same destination txPdu */
  if(isCalledFromMainFunction) /* COV_COM_DEFENSIVE_PROGRAMMING_CROSSPARTITIONGATEWAY */
  {
    boolean gwDescriptionUpdated = FALSE;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

    for(; gwDescrAccessIdx < Com_GetGwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfo(gwTxPduIdx); gwDescrAccessIdx++)
    {
      /* Check if the source description is fully contained in received buffer */
      if(PduInfoPtr->SduLength >= Com_GetValidDlcOfGwDescriptionAccessInfo(gwDescrAccessIdx))
      {
# if (COM_RXUBMASKUSEDOFGWDESCRIPTIONACCESSINFO == STD_ON)
        /* Check if the source description was updated */
        if(Com_DescriptionGw_CheckUB(gwDescrAccessIdx, PduInfoPtr) == TRUE) /* SBSW_COM_FCTCALL_WITH_P2CONST */
# endif
        {
          gwDescriptionUpdated = TRUE;
        }
      }
    }
    /* If any gateway description belonging to same source and destination Pdu was updated */
    if(gwDescriptionUpdated == TRUE)
    {
      /* Write gateway description into Queue. If write is successfull E_OK is returned, if write isn't successfull E_NOT_OK is returned and DET is called. */
      PduInfoType descriptionroutingInfoPtr;
      descriptionroutingInfoPtr.SduDataPtr = &(PduInfoPtr->SduDataPtr[Com_GetSourceMinStartBytePositionOfGwTxPduDescriptionInfo(gwTxPduIdx)]);
      descriptionroutingInfoPtr.SduLength = Com_GetSourceMaxLengthOfGwTxPduDescriptionInfo(gwTxPduIdx);

      if(E_OK != Com_CprQueueDescrGw_WriteData(Com_GetCprQueueIdxOfGwTxPduDescriptionInfo(gwTxPduIdx), (Com_GwTxPduDescriptionInfoIterType)gwTxPduIdx, &descriptionroutingInfoPtr)) /* SBSW_COM_FCTCALL_STACKPTR */
      {
        Com_LMgt_CallDetError(COMServiceId_CprQueueDescrGw_Write, COM_E_CPRQUEUE_OVERFLOW);
      }
    }
  }

  COM_DUMMY_STATEMENT(gwDescrAccessIdx); /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
}
#endif

#if (COM_GWTXPDUDESCRIPTIONINFOUSEDOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_ProcessPartitionLocalGwDescriptionEvent
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
 *
 *
 **********************************************************************************************************************/
COM_LOCAL_INLINE FUNC(void, COM_CODE) Com_DescriptionGw_ProcessPartitionLocalGwDescriptionEvent(P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr, Com_GwTxPduDescriptionInfoEndIdxOfRxPduInfoType gwTxPduIdx)
{
  Com_GwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfoType gwDescrAccessIdx = Com_GetGwDescriptionAccessInfoStartIdxOfGwTxPduDescriptionInfo(gwTxPduIdx);  
  Com_TxPduInfoIterType ComTxPduId = Com_GetTxPduInfoIdxOfGwDescriptionAccessInfo(gwDescrAccessIdx);

  boolean gwDescriptionUpdated = FALSE;       /* PRQA S 2981 */ /* MD_MSR_RetVal */
  boolean sendTrigger = FALSE;        /* PRQA S 2981 */ /* MD_MSR_RetVal */
  boolean withoutRepetitionFlag = TRUE;       /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* Process all descriptions that correspond to the same destination txPdu */
  for(; gwDescrAccessIdx < Com_GetGwDescriptionAccessInfoEndIdxOfGwTxPduDescriptionInfo(gwTxPduIdx); gwDescrAccessIdx++)
  {
    /* Check if the source description is fully contained in received buffer */
    if(PduInfoPtr->SduLength >= Com_GetValidDlcOfGwDescriptionAccessInfo(gwDescrAccessIdx))
    {
# if (COM_RXUBMASKUSEDOFGWDESCRIPTIONACCESSINFO == STD_ON)
      /* Check if the source description was updated */
      if(Com_DescriptionGw_CheckUB(gwDescrAccessIdx, PduInfoPtr) == TRUE) /* SBSW_COM_FCTCALL_WITH_P2CONST */
# endif
      {
        /* Process Description Routing */
        P2CONST(uint8, AUTOMATIC, COM_APPL_DATA) sourceBuffer = &(PduInfoPtr->SduDataPtr[Com_GetSourceMinStartBytePositionOfGwTxPduDescriptionInfo(gwTxPduIdx)]);
        sourceBuffer = &(sourceBuffer[Com_GetSourceStartByteOffsetOfGwDescriptionAccessInfo(gwDescrAccessIdx)]);
        switch (Com_DescriptionGw_ProcessDestinationDescription(sourceBuffer, gwDescrAccessIdx))     /* SBSW_COM_FCTCALL_WITH_P2CONST */
        {
          case COM_TRANSMIT_TRIGGERWITHOUTREP:
            sendTrigger = TRUE;       /* Set transmit flag if at least one description fulfills the transmit condition */
            break;
          case COM_TRANSMIT_TRIGGER:
            sendTrigger = TRUE;
            withoutRepetitionFlag = FALSE;    /* Set repetition flag if at least one description requires repetitve transmit */
            break;
          default:   /* COV_COM_MISRA */
            break;
        }

        gwDescriptionUpdated = TRUE;
      }
    }
  }

# if (COM_GWTIMEOUTINFO == STD_ON)
  /* If any gateway description belonging to same source and destination Pdu was updated */
  if(gwDescriptionUpdated == TRUE)
  {
    /* Handle Gw Timeout if configured */
    Com_GwTout_Event(ComTxPduId);
  }
# endif
# if ((COM_DIRECTOFTXMODETRUE == STD_ON) || (COM_DIRECTOFTXMODEFALSE == STD_ON))
  /* If transmit condition is fulfilled (transmit flag is set) */
  if(sendTrigger == TRUE)
  {
    Com_TypeOfGwDescriptionAccessInfoType typeOfDescription = Com_GetTypeOfGwDescriptionAccessInfo(Com_GetGwDescriptionAccessInfoStartIdxOfGwTxPduDescriptionInfo(gwTxPduIdx));
    /* Trigger the transmission of the PDU */
    Com_DescriptionGw_Transmit(ComTxPduId, Com_IsGwDestinationDescriptionTypeImmediate(typeOfDescription), withoutRepetitionFlag);        /* COV_COM_IMMEDIATE_DESTINATION_DESCRIPTION */
  }
# endif

  COM_DUMMY_STATEMENT(ComTxPduId); /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
  COM_DUMMY_STATEMENT(withoutRepetitionFlag); /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
  COM_DUMMY_STATEMENT(sendTrigger);   /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
  COM_DUMMY_STATEMENT(gwDescriptionUpdated);  /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
}
#endif

/**********************************************************************************************************************
  GLOBAL FUNCTIONS
**********************************************************************************************************************/

#if (COM_GWEVENTCACHE == STD_ON)
/**********************************************************************************************************************
  Com_DescriptionGw_MainFunction
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, COM_CODE) Com_DescriptionGw_MainFunction(Com_MainFunctionRouteSignalsStructIterType mainFunctionId)
{
  Com_GatewayDescriptionProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStructType gwDescriptionProcessingISRLockCounterIdx = Com_GetGatewayDescriptionProcessingISRLockCounterIdxOfMainFunctionRouteSignalsStruct(mainFunctionId);
    
  Com_DescriptionGw_EnterExclusiveArea();

  /* Process the Gw Event Cache */
  Com_DescriptionGw_ProcessGwEventCache(mainFunctionId, gwDescriptionProcessingISRLockCounterIdx);

# if(COM_CPRQUEUEDESCRIPTIONROUTINGINDUSEDOFPARTITIONSTXSTRUCT == STD_ON)
  /* Process Cross Partition Routing Queues leading to this partition */
  Com_DescriptionGw_ProcessCrossPartitionQueuesTx(mainFunctionId, gwDescriptionProcessingISRLockCounterIdx);
# endif

  Com_DescriptionGw_ExitExclusiveArea(gwDescriptionProcessingISRLockCounterIdx);
} /* PRQA S 6050 */ /* MD_MSR_STCAL */
#endif

#if (COM_GWTXPDUDESCRIPTIONINFOUSEDOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  Com_GwDescriptionEvent
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(void, COM_CODE) Com_GwDescriptionEvent(Com_RxPduInfoIterType rxComPduId, P2CONST(PduInfoType, AUTOMATIC, COM_APPL_DATA) PduInfoPtr, boolean isCalledFromMainFunction)
{
  Com_GwTxPduDescriptionInfoEndIdxOfRxPduInfoType gwTxPduIdx = Com_GetGwTxPduDescriptionInfoStartIdxOfRxPduInfo(rxComPduId);
  /* Iterate over all corresponding destination TxPDUs */
  for(; gwTxPduIdx < Com_GetGwTxPduDescriptionInfoEndIdxOfRxPduInfo(rxComPduId); gwTxPduIdx++)
  {
    /* If the indirection to the CrossPartitionRoutingQueue is used it is a cross partition routing */
# if (COM_CPRQUEUEUSEDOFGWTXPDUDESCRIPTIONINFO == STD_ON)
    if(Com_IsCprQueueUsedOfGwTxPduDescriptionInfo(gwTxPduIdx))
    {
      Com_DescriptionGw_ProcessCrossPartitionRoutingRx(PduInfoPtr, isCalledFromMainFunction, gwTxPduIdx);       /* SBSW_COM_FCTCALL_WITH_P2CONST */
    }
    /* Else process partition local routing */
    else 
# endif
    {
      Com_DescriptionGw_ProcessPartitionLocalGwDescriptionEvent(PduInfoPtr, gwTxPduIdx);       /* SBSW_COM_FCTCALL_WITH_P2CONST */
    }
  }

  COM_DUMMY_STATEMENT(isCalledFromMainFunction); /* PRQA S 1338, 2983, 3112, 2982 */ /* MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_MSR_DummyStmt, MD_Com_2982 */ /* lint -e{438} */
}       /* PRQA S 6030, 6080 */ /* MD_MSR_STMIF */ /* MD_MSR_STCYC */
#endif

#define COM_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/

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
 *  END OF FILE: Com_DescriptionGateway.c
 *********************************************************************************************************************/
