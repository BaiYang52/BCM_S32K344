/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         \file  I2c_TransProc.c
 *        \brief  Submodule to process I2c transmission.
 *
 *      \details  The transmission processor is responsible of processing incoming transmission requests. The submodule
 *                decides if an incoming request to transmit a sequence can be processed at once or has to be queued.
 *                The transmission processor maps the requested sequence to its corresponding channels and processes
 *                one channel's transmission after another. In case several channel's are "chained", address the same
 *                slave and require the same direction, these channel's are transmitted without an interrupt.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#define I2C_SOURCE

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "I2c_TransProc.h"
#include "I2c_PerMgr.h"
#include "I2c_ConfigMgr.h"

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#if !defined (I2C_LOCAL) /* COV_I2C_LOCAL */
# define I2C_LOCAL static
#endif

#if !defined (I2C_LOCAL_INLINE) /* COV_I2C_LOCAL */
# define I2C_LOCAL_INLINE LOCAL_INLINE
#endif

/**********************************************************************************************************************
 * LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#define I2C_START_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*! Stores the timeout handling method */
I2C_LOCAL VAR(I2c_HandlingType, I2C_VAR) I2c_TimeoutHandling = I2C_BURST;

#define I2C_STOP_SEC_VAR_INIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define I2C_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  I2c_TransProcessor_InitSeqTransmission()
 *********************************************************************************************************************/
/*! \brief          Initialize the transmission of a sequence.
 *  \details        This local service prepares all internal parameters for the transmission of the requested sequence.
 *  \param[in]      seqDescPtr         Pointer to the processed sequence descriptor.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_TransProcessor_InitSeqTransmission
(
  I2c_ConstSeqDescPtrType seqDescPtr
);

/**********************************************************************************************************************
 *  I2c_TransProcessor_PollSeqFromQueue()
 *********************************************************************************************************************/
/*! \brief          Polls a sequence from the HwUnit's priority queue and prepares internal parameters.
 *  \details        This local service tries to poll the next sequence from the HwUnit's corresponding
 *                  priority queue and initializes the sequence as currently processed sequence.
 *  \param[in]      hwUnitDescPtr      Pointer to the HwUnit descriptor.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_TransProcessor_PollSeqFromQueue
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr
);

/**********************************************************************************************************************
 *  I2c_TransProcessor_CancelTransmission()
 *********************************************************************************************************************/
/*! \brief          Cancels the HwUnit's ongoing transmission process.
 *  \details        This service cancels the currently processed transmission in case the requested HwUnit is idle.
 *  \param[in]      hwUnitDescPtr      Pointer to the processed HwUnit descriptor.
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_TransProcessor_CancelTransmission
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr
);

/**********************************************************************************************************************
 *  I2c_TransProcessor_SetAllChsResult()
 *********************************************************************************************************************/
/*! \brief          Sets all channels' statuses within the requested sequence's channel list.
 *  \details        -
 *  \param[in]      seqDescPtr         Pointer to the processed sequence descriptor.
 *  \param[in]      status             Channel result to set.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_TransProcessor_SetAllChsStatus
(
  I2c_ConstSeqDescPtrType seqDescPtr,
  I2c_ChannelStatusType status
);

/**********************************************************************************************************************
 *  I2c_TransProcessor_AddChToTransBuf()
 *********************************************************************************************************************/
/*! \brief          Adds a single channel to the transmission buffer.
 *  \details        -
 *  \param[in]      hwUnitDescPtr      Pointer to the processed HwUnit Descriptor.
 *  \param[in]      currentChannelPtr  Pointer to the channel descriptor.
 *  \param[in]      channelPos         Channel's position within the transmission buffer.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_TransProcessor_AddChToTransBuf
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr,
  I2c_ConstChannelDescPtrType currentChannelPtr,
  I2c_ChannelType channelPos
);

/**********************************************************************************************************************
 *  I2c_TransProcessor_ChainNextChannel()
 *********************************************************************************************************************/
/*! \brief          Processes the next channel from the sequence's channel list.
 *  \details        This service determines if the currently processed channel from the sequence's channel list
 *                  can be transmitted or not. If the channel is the first within the transmission buffer or if the
 *                  channel can be chained, this service adds the channel to the transmission buffer.
 *  \param[in]      hwUnitDescPtr      Pointer to the channel descriptor.
 *  \param[in]      channelDescPtr     Pointer to the processed HwUnit Descriptor.
 *  \param[in]      channelPos         Next free position within the transmission buffer.
 *  \return         E_OK - The channel has been added to the transmission buffer.
 *  \return         E_NOT_OK - The channel could not be added to the transmission buffer.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(Std_ReturnType, I2C_CODE) I2c_TransProcessor_ChainNextChannel
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr,
  I2c_ConstChannelDescPtrType channelDescPtr,
  I2c_ChannelType channelPos
);

/**********************************************************************************************************************
 *  I2c_TransProcessor_ProcessChannels()
 *********************************************************************************************************************/
/*! \brief          Processes the next channel(s) of the currently processed sequence.
 *  \details        This service sequentially passes the channels of the currently processed sequence to the
 *                  low-level module. The number of channels that can be transmitted at once depends
 *                  on two factors. First, if a channel's "chained" parameter is enabled. Second, if the
 *                  transmission direction and the target address are the same as the ones of the previous channel.
 *  \param[in]      hwUnitDescPtr      Pointer to the processed HwUnit descriptor.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_TransProcessor_ProcessChannels
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr
);

/**********************************************************************************************************************
 *  I2c_TransProcessor_CheckChaining()
 *********************************************************************************************************************/
/*! \brief          Checks if the requested channel can be chained.
 *  \details        This service checks, if the requested channel's chained parameter is TRUE, if the channel's slave
 *                  address and its transmission direction is equal to the leading channel's slave address and
 *                  transmission direction.
 *  \param[in]      hwUnitDescPtr      Pointer to the HwUnit descriptor.
 *  \param[in]      currentChannel     Pointer to the requested channel descriptor.
 *  \pre            -
 *  \return         TRUE - The requested channel can be chained.
 *  \return         FALSE - The requested channel can not be chained.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(boolean, I2C_CODE) I2c_TransProcessor_CheckChaining
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr,
  I2c_ConstChannelDescPtrType currentChannel
);

/**********************************************************************************************************************
 *  I2c_TransProcessor_HandleErrorResult()
 *********************************************************************************************************************/
/*! \brief          Sets the sequence's result according to the given channel result and terminates
 *                  the transmission.
 *  \details        -
 *  \param[in]      seqDescPtr         Pointer to the sequence descriptor.
 *  \param[in]      channelResult      Channel result.
 *  \pre            -
 *  \context        ISR
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_TransProcessor_HandleErrorResult
(
  I2c_ConstSeqDescPtrType seqDescPtr,
  I2c_ChannelResultType channelResult
);

/**********************************************************************************************************************
 * LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  I2c_TransProcessor_InitSeqTransmission()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
I2C_LOCAL FUNC(void, I2C_CODE) I2c_TransProcessor_InitSeqTransmission
(
  I2c_ConstSeqDescPtrType seqDescPtr
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr = I2c_ConfigMgr_GetHwUnitDescViaSeq(seqDescPtr->SeqId);
  I2c_HardwareProcessorPtrType processedSeqPtr = &hwUnitDescPtr->varHwUnitDescPtr->ProcessedSeq;

  /* ----------- Implementation ------------------------------------------------------------------ */
  /* #10 Set the requested sequence as the currently processed sequence. */
  processedSeqPtr->CurrentSeq = seqDescPtr->SeqId; /* SBSW_I2C_HWREF_WRITE_N_01 */
  processedSeqPtr->SequenceInProcess = TRUE; /* SBSW_I2C_HWREF_WRITE_N_01 */
  processedSeqPtr->CancelProcess = FALSE; /* SBSW_I2C_HWREF_WRITE_N_01 */

  /* #20 Set the sequence's transmission specific parameters to default. */
  seqDescPtr->VarSeqDescPtr->LeadingChannel = 0u; /* SBSW_I2C_SEQ_PTR_WRITE_04 */
  seqDescPtr->VarSeqDescPtr->NextChannel = 0u; /* SBSW_I2C_SEQ_PTR_WRITE_04 */

  /* #30 Configure the HwUnit. */
  I2c_PerMgr_ConfigHwUnit(hwUnitDescPtr->HwUnitId, seqDescPtr->BusCfgPtr->Baudrate, seqDescPtr->BusCfgPtr->BusTimeout);
} /* I2c_TransProcessor_InitSeqTransmission */

/**********************************************************************************************************************
 *  I2c_TransProcessor_PollSeqFromQueue()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
I2C_LOCAL FUNC(void, I2C_CODE) I2c_TransProcessor_PollSeqFromQueue
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  /* #10 Call the Queue Manager's service to poll a sequence from the HwUnit's priority queue. */
  I2c_ConstSeqDescPtrType seqDescPtr = I2c_QueueMgr_PollSeq(hwUnitDescPtr); /* SBSW_I2C_HW_PTR_PASS_N_02 */

  /* ----------- Implementation ------------------------------------------------------------------ */
  /* #20 If the sequence descriptor pointer is valid. */
  if (seqDescPtr != NULL_PTR)
  {
    /* #30 Register the polled sequence at its corresponding HwUnit as the currently processed sequence. */
    I2c_TransProcessor_InitSeqTransmission(seqDescPtr); /* SBSW_I2C_PTR_PASS_N */

    /* #40 Set the sequence's result to PENDING. */
    seqDescPtr->VarSeqDescPtr->SeqResult = I2C_SEQ_PENDING; /* SBSW_I2C_SEQ_PTR_WRITE_02 */
  }

} /* I2c_TransProcessor_PollSeqFromQueue */

/**********************************************************************************************************************
 *  I2c_TransProcessor_CancelTransmission()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
I2C_LOCAL FUNC(void, I2C_CODE) I2c_TransProcessor_CancelTransmission
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  I2c_HardwareProcessorPtrType procSeqPtr = &hwUnitDescPtr->varHwUnitDescPtr->ProcessedSeq;

  /* ----------- Implementation ------------------------------------------------------------------ */
  /* #10 Set internal parameters so that no sequence is currently processed. */
  procSeqPtr->SequenceInProcess = FALSE; /* SBSW_I2C_HWREF_WRITE_R_03 */
  procSeqPtr->CancelProcess = FALSE; /* SBSW_I2C_HWREF_WRITE_R_03 */

} /* I2c_TransProcessor_CancelTransmission */

/**********************************************************************************************************************
 *  I2c_TransProcessor_SetAllChsResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
I2C_LOCAL FUNC(void, I2C_CODE) I2c_TransProcessor_SetAllChsStatus
(
  I2c_ConstSeqDescPtrType seqDescPtr,
  I2c_ChannelStatusType status
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  /* #10 Get the sequence's channel list. */
  I2c_ChannelConstPtrType channelListPtr = seqDescPtr->ChannelListPtr;
  I2c_ChannelIndexType channelIndex;
  I2c_ChannelType channelId;

  /* ----------- Implementation ------------------------------------------------------------------ */
  /* #20 Iterate over the channel list. */
  for (channelIndex = 0; channelIndex < seqDescPtr->NumOfChannels; channelIndex++)
  {
    I2c_ConstChannelDescPtrType channelDescPtr;
    /* #30 Get the current channel ID from the channel list. */
    channelId = channelListPtr[channelIndex];
    channelDescPtr = I2c_ConfigMgr_GetChannelDesc(channelId);

    /* #40 Set the result of the channel with the given channel ID. */
    channelDescPtr->VarChannelDescPtr->Status = status; /* SBSW_I2C_CH_PTR_WRITE_03 */
  }

} /* I2c_TransProcessor_SetAllChsResult */

/**********************************************************************************************************************
 *  I2c_TransProcessor_AddChToTransBuf()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
I2C_LOCAL FUNC(void, I2C_CODE) I2c_TransProcessor_AddChToTransBuf
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr,
  I2c_ConstChannelDescPtrType currentChannelPtr,
  I2c_ChannelType channelPos
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  /* #10 Get the HwUnit's transmission buffer pointer. */
  I2c_TransBufferDescPtrType transBufDescPtr = &(hwUnitDescPtr->varHwUnitDescPtr->TransData.TransBufferDesc[channelPos % I2C_TRANS_BUFFER_SIZE]);

  /* ----------- Implementation ------------------------------------------------------------------ */
  /* #20 If the default value shall be transmitted, set buffer descriptor to default data. */
  if (currentChannelPtr->VarChannelDescPtr->BufPtr == NULL_PTR)
  {
    transBufDescPtr->BufPtr = (I2c_DataPtrType)(&(currentChannelPtr->DefaultData)); /* PRQA S 0311 */ /* MD_I2C_11.8 */ /* SBSW_I2C_TRANSBUF_ARRAY_R_02 */
    transBufDescPtr->SingleValue = TRUE; /* SBSW_I2C_TRANSBUF_ARRAY_R_02 */
  }
  /* #30 Otherwise the channel's buffer pointer shall be referenced. */
  else
  {
    transBufDescPtr->BufPtr = currentChannelPtr->VarChannelDescPtr->BufPtr; /* SBSW_I2C_TRANSBUF_ARRAY_R_02 */
    transBufDescPtr->SingleValue = FALSE; /* SBSW_I2C_TRANSBUF_ARRAY_R_02 */
  }

  /* #40 Set the buffer's data length. */
  transBufDescPtr->DataLength = currentChannelPtr->VarChannelDescPtr->Length; /* SBSW_I2C_TRANSBUF_ARRAY_R_02 */

} /* I2c_TransProcessor_AddChToTransBuf */

/**********************************************************************************************************************
 *  I2c_TransProcessor_ChainNextChannel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
I2C_LOCAL FUNC(Std_ReturnType, I2C_CODE) I2c_TransProcessor_ChainNextChannel
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr,
  I2c_ConstChannelDescPtrType channelDescPtr,
  I2c_ChannelType channelPos
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;

  /* ----------- Implementation ------------------------------------------------------------------ */
  /* #10 If the channel can be chained. */
  if (I2c_TransProcessor_CheckChaining(hwUnitDescPtr, channelDescPtr) == TRUE) /* SBSW_I2C_PTR_PASS_N */
  {
    /* #20 Add the channel to the transmission buffer. */
    I2c_TransProcessor_AddChToTransBuf(hwUnitDescPtr, channelDescPtr, channelPos); /* SBSW_I2C_PTR_PASS_N */

    retVal = E_OK;
  }

  return retVal;

} /* I2c_TransProcessor_ChainNextChannel */

/**********************************************************************************************************************
 *  I2c_TransProcessor_ProcessChannels()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
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
 *
 *
 *
 *
 *
 *
 */
I2C_LOCAL FUNC(void, I2C_CODE) I2c_TransProcessor_ProcessChannels
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  I2c_ConstHardwareProcessorPtrType procSeqPtr = &hwUnitDescPtr->varHwUnitDescPtr->ProcessedSeq;
  I2c_ConstSeqDescPtrType seqDescPtr = I2c_ConfigMgr_GetSeqDesc(procSeqPtr->CurrentSeq);
  I2c_TransDataPtrType transDataPtr = &hwUnitDescPtr->varHwUnitDescPtr->TransData;
  I2c_ConstChannelDescPtrType channelDescPtr;
  boolean continueChaining = TRUE;
  I2c_ChannelType numOfChainedChs = 0;
  I2c_ChannelType channelId;

  /* ----------- Implementation ------------------------------------------------------------------ */
  /* Leading channel */
    /* #10 Request the channel that has to be transmitted next. */
    channelId = seqDescPtr->ChannelListPtr[seqDescPtr->VarSeqDescPtr->NextChannel];
    channelDescPtr = I2c_ConfigMgr_GetChannelDesc(channelId);

    /* #20 Set the channel as the sequence's leading channel. */
    seqDescPtr->VarSeqDescPtr->LeadingChannel = seqDescPtr->VarSeqDescPtr->NextChannel; /* SBSW_I2C_SEQ_PTR_WRITE_03 */
    seqDescPtr->VarSeqDescPtr->NextChannel++; /* SBSW_I2C_SEQ_PTR_WRITE_03 */

    I2c_TransProcessor_AddChToTransBuf(hwUnitDescPtr, channelDescPtr, numOfChainedChs); /* SBSW_I2C_HW_CH_DOUBLE */

    /* #30 Set the channel's status to ACTIVE. */
    channelDescPtr->VarChannelDescPtr->Status = I2C_CH_STATUS_ACTIVE; /* SBSW_I2C_CH_PTR_WRITE_04 */

    /* #40 Increment the number of channels within the transmission buffer. */
    numOfChainedChs++;

    /*
     * #60 Set all transmission specific parameters of the leading channel
     * as the transmission parameters of the transmission buffer.
     */
    transDataPtr->SlaveAddress = channelDescPtr->VarChannelDescPtr->SlaveDevAddr; /* SBSW_I2C_HWREF_WRITE_R_03 */
    transDataPtr->TransDir = channelDescPtr->VarChannelDescPtr->TransDir; /* SBSW_I2C_HWREF_WRITE_R_03 */


  /* Chained channels */
  /* #70 Add channels to the Transmission Buffer, as long as the Transmission Buffer length is not exceeded and
   * as long as further channels can be chained. */
  while (continueChaining == TRUE)
  {
    if(seqDescPtr->VarSeqDescPtr->NextChannel < seqDescPtr->NumOfChannels)
    {
      /* #80 Request the channel that has to be transmitted next. */
      channelId = seqDescPtr->ChannelListPtr[seqDescPtr->VarSeqDescPtr->NextChannel];
      channelDescPtr = I2c_ConfigMgr_GetChannelDesc(channelId);

      /* #90 If the currently processed channel has been added to the transmission buffer. */
      if (I2c_TransProcessor_ChainNextChannel(hwUnitDescPtr, channelDescPtr, numOfChainedChs) == E_OK) /* SBSW_I2C_HW_CH_DOUBLE */
      {
        seqDescPtr->VarSeqDescPtr->NextChannel++; /* SBSW_I2C_SEQ_PTR_WRITE_03 */

        /* #100 Set the channel's status to ACTIVE. */
        channelDescPtr->VarChannelDescPtr->Status = I2C_CH_STATUS_ACTIVE; /* SBSW_I2C_CH_PTR_WRITE_04 */

        /* #110 Increment the number of channels within the transmission buffer. */
        numOfChainedChs++;
      }
      /* #120 Otherwise the channel could not be chained and the bus has to be hold for the next channel transmission. */
      else
      {
        transDataPtr->HoldBus = TRUE; /* SBSW_I2C_HWREF_WRITE_R_03 */
        continueChaining = FALSE;
      }
    }
    else
    {
      /* #130 Otherwise there are no channels left for transmission, the bus can be set free. */
      transDataPtr->HoldBus = FALSE; /* SBSW_I2C_HWREF_WRITE_R_03 */
      continueChaining = FALSE;
    }
  }

  /* #140 Set the counted number of channels within the TransBufferDescriptor. */
  transDataPtr->NumOfChannels = numOfChainedChs; /* SBSW_I2C_HWREF_WRITE_R_03 */

  /* #150 Call the Peripheral Manager to start the channel transmission. */
  I2c_PerMgr_TransData(hwUnitDescPtr->HwUnitId);

} /* I2c_TransProcessor_ProcessChannels */

/**********************************************************************************************************************
 *  I2c_TransProcessor_CheckChaining()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
I2C_LOCAL FUNC(boolean, I2C_CODE) I2c_TransProcessor_CheckChaining
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr,
  I2c_ConstChannelDescPtrType currentChannel
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  boolean retVal = TRUE;
  I2c_ConstTransDataPtrType transDataPtr = &hwUnitDescPtr->varHwUnitDescPtr->TransData;

  /* ----------- Implementation ------------------------------------------------------------------ */
  /* #10 If the current channel can be chained. */
  if (currentChannel->Chained == FALSE)
  {
    retVal = FALSE;
  }

  /* #20 If the current channel's slave address is equal to the leading channel's slave address. */
  if (currentChannel->VarChannelDescPtr->SlaveDevAddr != transDataPtr->SlaveAddress)
  {
    retVal = FALSE;
  }

  /*
   * #30 If the current channel's transmission direction
   * is equal to the leading channel's transmission direction.
   */
  if (currentChannel->VarChannelDescPtr->TransDir != transDataPtr->TransDir)
  {
    retVal = FALSE;
  }

  return retVal;

} /* I2c_TransProcessor_CheckChaining */

/**********************************************************************************************************************
 *  I2c_TransProcessor_HandleErrorResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
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
 *
 *
 */
I2C_LOCAL FUNC(void, I2C_CODE) I2c_TransProcessor_HandleErrorResult
(
  I2c_ConstSeqDescPtrType seqDescPtr,
  I2c_ChannelResultType channelResult
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr = I2c_ConfigMgr_GetHwUnitDescViaSeq(seqDescPtr->SeqId);
  boolean errorFlag = TRUE;

  /* ----------- Implementation ------------------------------------------------------------------ */
  switch(channelResult)
  {
    /* #10 In case of ARBFAIL. */
    case I2C_CH_RESULT_ARBFAIL:

      seqDescPtr->VarSeqDescPtr->SeqResult = I2C_SEQ_FAILED; /* SBSW_I2C_SEQ_PTR_WRITE_03 */
#if(I2C_DEM_ERROR_REPORT == STD_ON)
      Dem_ReportErrorStatus(I2c_ConfigPtr->DemArbitrationLoss, DEM_EVENT_STATUS_FAILED);
#endif

      /* #20 If the Timeout handling is in BURST mode. */
      if(I2c_TimeoutHandling == I2C_BURST)
      {
        I2c_PerMgr_DetachBus(hwUnitDescPtr->HwUnitId);
      }
      /* #30 Otherwise if the Timeout handling is in RECURRENT mode, the HwUnit must free the bus. */
      else
      {
        I2c_PerMgr_FreeBus(hwUnitDescPtr->HwUnitId);
      }
      break;
    /* #40 In case of BUSFAIL. */
    case I2C_CH_RESULT_BUSFAIL:
#if(I2C_DEM_ERROR_REPORT == STD_ON)
      Dem_ReportErrorStatus(I2c_ConfigPtr->DemBusError, DEM_EVENT_STATUS_FAILED);
#endif
      /* #50 Call the PeripheralManager to detach the bus. */
      I2c_PerMgr_DetachBus(hwUnitDescPtr->HwUnitId);
      seqDescPtr->VarSeqDescPtr->SeqResult = I2C_SEQ_FAILED; /* SBSW_I2C_SEQ_PTR_WRITE_03 */
      break;

    /* #60 In case of a NACKFAIL, the sequence result is NACK. */
    default:

      seqDescPtr->VarSeqDescPtr->SeqResult = I2C_SEQ_NACK; /* SBSW_I2C_SEQ_PTR_WRITE_03 */

      /* #70 Call the PeripheralManager to detach the bus. */
      I2c_PerMgr_DetachBus(hwUnitDescPtr->HwUnitId);

      /* #80 If the sequence's parameter NackAsError is disabled, avoid reporting of error. */
      if(seqDescPtr->NackAsError == FALSE)
      {
        errorFlag = FALSE;
      }
#if(I2C_DEM_ERROR_REPORT == STD_ON)
      else
      {
        Dem_ReportErrorStatus(I2c_ConfigPtr->DemAddressError, DEM_EVENT_STATUS_FAILED);
      }
#endif
      break;
  }

  /* #90 Terminate transmission of sequence. */
  I2c_TransProcessor_CancelTransmission(hwUnitDescPtr); /* SBSW_I2C_HW_PTR_PASS_N_01 */

  /* #100 If the errorFlag is set TRUE, report Error notification. */
  if(errorFlag == TRUE)
  {
    /* #110 Notify the user about the sequence's error. */
    if(seqDescPtr->SequenceErrorNotification != NULL_PTR)
    {
      seqDescPtr->SequenceErrorNotification(); /* SBSW_I2C_SEQ_FUNC_CALL */
    }
  }
  /* #120 Otherwise report Success notification. */
  else
  {
    if(seqDescPtr->SequenceEndNotification != NULL_PTR)
    {
      seqDescPtr->SequenceEndNotification(); /* SBSW_I2C_SEQ_FUNC_CALL */
    }
  }

} /* I2c_TransProcessor_HandleErrorResult */

/*********************************************************************************************************************
 * GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  I2c_TransProcessor_ProcessHwUnit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, I2C_CODE) I2c_TransProcessor_ProcessHwUnit
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  I2c_ConstHardwareProcessorPtrType procSeqPtr;

  /* ----------- Implementation ------------------------------------------------------------------ */
  /* #10 If the HwUnit is idle */
  if(I2c_PerMgr_IsHwUnitIdle(hwUnitDescPtr->HwUnitId) == TRUE)
  {
    procSeqPtr = &hwUnitDescPtr->varHwUnitDescPtr->ProcessedSeq;
    /* #20 If any sequence is currently in process. */
    if (procSeqPtr->SequenceInProcess == TRUE)
    {
      /* #30 If the CancelProcess flag is set. */
      if (procSeqPtr->CancelProcess == TRUE)
      {
        /* #40 Cancel the HwUnit's ongoing transmission. */
        I2c_TransProcessor_CancelTransmission(hwUnitDescPtr); /* SBSW_I2C_PTR_PASS_N */

        /* #50 Call the I2c_PerMgr_DetachBus() to free the bus. */
        I2c_PerMgr_DetachBus(hwUnitDescPtr->HwUnitId);
      }
      /* #60 Otherwise transmit the sequence's channel(s) that has/have to be transmitted next. */
      else
      {
        I2c_TransProcessor_ProcessChannels(hwUnitDescPtr); /* SBSW_I2C_PTR_PASS_N */
      }
    }
    /* #70 Otherwise get the next sequence from the HwUnit's priority queue. */
    else
    {
      I2c_TransProcessor_PollSeqFromQueue(hwUnitDescPtr); /* SBSW_I2C_PTR_PASS_N */
    }
  }

} /* I2c_TransProcessor_ProcessHwUnit */

/**********************************************************************************************************************
 *  I2c_TransProcessor_RegisterSeq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, I2C_CODE) I2c_TransProcessor_RegisterSeq
(
  I2c_ConstSeqDescPtrType seqDescPtr
) /* PRQA S 1532 */ /* MD_I2C_1532 */
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  Std_ReturnType retVal;
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr =
          (I2c_ConstHwUnitDescPtrType)I2c_ConfigMgr_GetHwUnitDescViaSeq(seqDescPtr->SeqId);

  /* ----------- Implementation ------------------------------------------------------------------ */
  /* #10 If the sequence's corresponding HwUnit is ready and the HwUnit's queue is empty. */
  if ((I2c_QueueMgr_IsEmpty(hwUnitDescPtr) == TRUE) && /* SBSW_I2C_PTR_PASS_N */
      (hwUnitDescPtr->varHwUnitDescPtr->ProcessedSeq.SequenceInProcess == FALSE))
  {
    /* #20 Initialize the sequence's transmission. */
    I2c_TransProcessor_InitSeqTransmission(seqDescPtr); /* SBSW_I2C_SEQ_PTR_PASS_02 */

    /* #30 Set the sequence's result to PENDING. */
    seqDescPtr->VarSeqDescPtr->SeqResult = I2C_SEQ_PENDING; /* SBSW_I2C_SEQ_PTR_WRITE_02 */

    /* #40 Set all sequence's channels' statuses to PENDING. */
    I2c_TransProcessor_SetAllChsStatus(seqDescPtr, I2C_CH_STATUS_PENDING); /* SBSW_I2C_SEQ_PTR_PASS_02 */

    retVal = E_OK;
  }
  /* #50 Otherwise if the sequence can be enqueued. */
  else if (((hwUnitDescPtr->varHwUnitDescPtr->ProcessedSeq.CurrentSeq) != (seqDescPtr->SeqId)) &&
           (I2c_QueueMgr_AddSeq(seqDescPtr) == E_OK)) /* SBSW_I2C_SEQ_PTR_PASS_02 */ /* PRQA S 3415 */ /* MD_I2C_3415 */
  {
    /* #60 Set the sequence's result to QUEUED. */
    seqDescPtr->VarSeqDescPtr->SeqResult = I2C_SEQ_QUEUED; /* SBSW_I2C_SEQ_PTR_WRITE_02 */

    /* #70 Set all sequence's channels' statuses to PENDING. */
    I2c_TransProcessor_SetAllChsStatus(seqDescPtr, I2C_CH_STATUS_PENDING); /* SBSW_I2C_SEQ_PTR_PASS_02 */
    retVal = E_OK;
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;

} /* I2c_TransProcessor_TransmitSeq */

/**********************************************************************************************************************
 *  I2c_TransProcessor_ReportResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, I2C_CODE) I2c_TransProcessor_ReportResult
(
  I2c_HwUnitType hwUnitId,
  I2c_ChannelResultType channelResult
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr = I2c_ConfigMgr_GetHwUnitDesc(hwUnitId);

  if(hwUnitDescPtr != NULL_PTR)
  {
    /* Number of chained channels that have been transmitted. */
    I2c_ChannelType numOfChainedChs = hwUnitDescPtr->varHwUnitDescPtr->TransData.NumOfChannels;
    I2c_ConstSeqDescPtrType seqDescPtr =
            I2c_ConfigMgr_GetSeqDesc(hwUnitDescPtr->varHwUnitDescPtr->ProcessedSeq.CurrentSeq);
    boolean lastChannel = FALSE; /* Determines if the sequence's last channel has been transmitted. */
    I2c_ChannelIndexType channelIndex;

  /* ----------- Implementation ------------------------------------------------------------------ */
    /* #10 Iterate over the sequence's channel list. */
    for (channelIndex = seqDescPtr->VarSeqDescPtr->LeadingChannel;
            channelIndex < (seqDescPtr->VarSeqDescPtr->LeadingChannel + numOfChainedChs); channelIndex++)
    {
      I2c_ConstChannelDescPtrType channelDescPtr;
      /* #20 If the current channel is the sequence's last channel. */
      if(channelIndex == (seqDescPtr->NumOfChannels - (I2c_ChannelIndexType)1))
      {
        lastChannel = TRUE;
      }

      channelDescPtr = I2c_ConfigMgr_GetChannelDesc(seqDescPtr->ChannelListPtr[channelIndex]);
      /* #30 Set the requested result as the channel's result. */
      channelDescPtr->VarChannelDescPtr->Result =
              channelResult; /* SBSW_I2C_CH_PTR_WRITE_05 */
    }

    /* #40 If the channel result is not OK, the sequence's transmission process is terminated. */
    if((channelResult != I2C_CH_RESULT_OK ))
    {
      I2c_TransProcessor_HandleErrorResult(seqDescPtr, channelResult); /* SBSW_I2C_SEQ_PTR_PASS_01 */

    }
    /* #60 If no error was reported, check if the current channel is the sequence's last channel. */
    else if(lastChannel == TRUE)
    {

      seqDescPtr->VarSeqDescPtr->SeqResult = I2C_SEQ_OK; /* SBSW_I2C_SEQ_PTR_WRITE_03 */
      I2c_TransProcessor_CancelTransmission(hwUnitDescPtr); /* SBSW_I2C_PTR_PASS_N */

      /* #70 Notify the user about the finished transmission process. */
      if(seqDescPtr->SequenceEndNotification != NULL_PTR)
      {
        seqDescPtr->SequenceEndNotification(); /* SBSW_I2C_SEQ_FUNC_CALL */
      }
    }
    else
    {
     /* In case no error has been reported and it's not the last sequence's channel, nothing special has to be done. */
    }

    /* #80 Call the service to continue the transmission. */
    I2c_TransProcessor_ProcessHwUnit(hwUnitDescPtr); /* SBSW_I2C_PTR_PASS_N */
  }
} /* I2c_TransProcessor_ReportResult */

/**********************************************************************************************************************
 *  I2c_TransProcessor_CancelSeq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
FUNC(void, I2C_CODE) I2c_TransProcessor_CancelSeq /* PRQA S 1532 */ /* MD_I2C_1532 */
(
  I2c_ConstSeqDescPtrType seqDescPtr
)
{
  /* ----------- Local variables ----------------------------------------------------------------- */
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr = I2c_ConfigMgr_GetHwUnitDescViaSeq(seqDescPtr->SeqId);
  I2c_HardwareProcessorPtrType procSeqPtr = &hwUnitDescPtr->varHwUnitDescPtr->ProcessedSeq;

  /* ----------- Implementation ------------------------------------------------------------------ */
  /* #10 If the requested sequence is within an ongoing transmission process. */
  if ((procSeqPtr->SequenceInProcess == TRUE) && (procSeqPtr->CurrentSeq == seqDescPtr->SeqId))
  {
    /* #20 Set the cancel flag TRUE. */
    procSeqPtr->CancelProcess = TRUE; /* SBSW_I2C_HWREF_WRITE_N_01 */
  }
  /* #30 Otherwise call the Queue Manager to remove the sequence from the priority queue. */
  else
  {

    I2c_QueueMgr_RemoveSeq(seqDescPtr); /* SBSW_I2C_PTR_PASS_N */
  }

  /* #40 Set the sequence's result to CANCELLED. */
  seqDescPtr->VarSeqDescPtr->SeqResult = I2C_SEQ_CANCELED; /* SBSW_I2C_SEQ_PTR_WRITE_05 */

  /* #50 Set the sequence's channels status to IDLE. */
  I2c_TransProcessor_SetAllChsStatus(seqDescPtr, I2C_CH_STATUS_IDLE); /* SBSW_I2C_SEQ_PTR_PASS_02 */

} /* I2c_TransProcessor_CancelSeq */

/**********************************************************************************************************************
 *  I2c_TransProcessor_SetHandling()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, I2C_CODE) I2c_TransProcessor_SetHandling /* PRQA S 1532 */ /* MD_I2C_1532 */
(
  I2c_HandlingType handling
)
{
  /* ----------- Implementation ------------------------------------------------------------------ */
  I2c_TimeoutHandling = handling;
} /* I2c_TransProcessor_SetHandling */

# define I2C_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*********************************************************************************************************************
 * END OF FILE: I2c_TransProc.c
 *********************************************************************************************************************/
