/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  I2c_PrioQueueMgr.c
 *        \brief  Submodule to manage the priority queue of sequences.
 *
 *      \details  The Priority Queue Manager is responsible of the global queue containing all sequences that are ready
 *                to be processed. The manager can insert sequences according to their priority and return the
 *                sequence with the highest priority.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to reversion history in I2c.h.
 *********************************************************************************************************************/

#define I2C_PRIO_QUEUE_MGR_C

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "I2c_PrioQueueMgr.h"
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

/*************************************************************************************************
 * LOCAL FUNCTION PROTOTYPES
 *************************************************************************************************/
# define I2C_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  I2c_QueueMgr_GetRange()
 *********************************************************************************************************************/
/*! \brief          This service determines the range from a given start index to a given end index
 *                  within a requested queue.
 *  \details        This service presumes that at least one entry is within the priority queue.
 *  \param[in]      Start index
 *  \param[in]      End index
 *  \return         Number of entries between start index and end index.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL_INLINE FUNC(I2c_PrioQueueLengthType, I2C_CODE) I2c_QueueMgr_GetRange
(
  I2c_PrioQueueLengthType rotStartIndex,
  I2c_PrioQueueLengthType rotEndIndex
);

/**********************************************************************************************************************
 *  I2c_QueueMgr_RotateRight()
 *********************************************************************************************************************/
/*! \brief          This service rotates all entries of a priority queue within a given range to the right by 1.
 *  \details        This service rotates all entries of the requested priority queue from the given start index
 *                  to the queue's end index to the right by the value 1.
 *  \param[in]      PrioQueueDesc             - Pointer to the priority queue descriptor.
 *  \param[in]      Rotation start index      - Start index of the range that has to be rotated.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(void, I2C_CODE) I2c_QueueMgr_RotateRight
(
  I2c_PrioQueueDescPtrType prioQueueDesc,
  I2c_PrioQueueLengthType rotStartIndex,
  I2c_PrioQueueLengthType rotEndIndex
);

/**********************************************************************************************************************
 *  I2c_QueueMgr_IncreaseStartIndex()
 *********************************************************************************************************************/
/*! \brief          Increases the queue's start index by 1.
 *  \details        Increases the queue's start index by 1. In case the index reaches the end of the queue,
 *                  it begins from 0.
 *  \param[in]      Pointer to the requested priority queue descriptor.
 *  \param[out]     none
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL_INLINE FUNC(void, I2C_CODE) I2c_QueueMgr_IncrementStartIndex
(
  I2c_PrioQueueDescPtrType queueDescPtr
);

/**********************************************************************************************************************
 *  I2c_QueueMgr_IncrementEndIndex()
 *********************************************************************************************************************/
/*! \brief        Increases the queue's end index by 1.
 *  \details      Increases the queue's end index by 1. In case the index reaches the end of the queue,
 *                it begins from 0.
 *  \param[in]    Pointer to the requested priority queue descriptor.
 *  \param[out]   none
 *  \pre          -
 *  \context      TASK
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *********************************************************************************************************************/
I2C_LOCAL_INLINE FUNC(void, I2C_CODE) I2c_QueueMgr_IncrementEndIndex
(
  I2c_PrioQueueDescPtrType queueDescPtr
);

/**********************************************************************************************************************
 *  I2c_QueueMgr_CheckQueueEntries()
 *********************************************************************************************************************/
/*! \brief          This service decides if the requested sequence can be added to the priority queue.
 *  \details        This service determines if the requested sequence is already within the priority queue
 *                  and if the priority queue has still an entry left.
 *  \param[in]      Pointer to the sequence descriptor pointer.
 *  \return         E_OK            - The requested sequence can be added to the priority queue.
 *  \return         E_NOT_OK        - Either the requested sequence is already queued, or the priority queue is
 *                                  completely filled.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
I2C_LOCAL FUNC(Std_ReturnType, I2C_CODE) I2c_QueueMgr_CheckQueueEntries
(
  I2c_ConstSeqDescPtrType seqDescPtr
);

/*************************************************************************************************
 * LOCAL FUNCTIONS
 *************************************************************************************************/
/**********************************************************************************************************************
 *  I2c_QueueMgr_GetRange()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
I2C_LOCAL_INLINE FUNC(I2c_PrioQueueLengthType, I2C_CODE) I2c_QueueMgr_GetRange
(
  I2c_PrioQueueLengthType rotStartIndex,
  I2c_PrioQueueLengthType rotEndIndex
)
{
  /* -------------- Local variables -------------------------------------------------------------------------------- */
  I2c_PrioQueueLengthType queueIndex;

  /* #10 The range starts from 1 with respect to the end index. */
  I2c_PrioQueueLengthType range = 1;

  /* -------------- Implementation --------------------------------------------------------------------------------- */
  /* #20 Iterate over the priority queue while counting the number of entries. */
  for (queueIndex = rotStartIndex; (I2c_PrioQueueLengthType)(queueIndex % (I2c_PrioQueueLengthType)(I2C_PRIO_QUEUE_BUFFER_SIZE)) != rotEndIndex;
          queueIndex++)
  {
    range++;
  }

  return range;
} /* I2c_QueueMgr_GetRange */

/**********************************************************************************************************************
 *  I2c_QueueMgr_RotateRight()
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
I2C_LOCAL FUNC(void, I2C_CODE) I2c_QueueMgr_RotateRight
(
  I2c_PrioQueueDescPtrType prioQueueDesc,
  I2c_PrioQueueLengthType rotStartIndex,
  I2c_PrioQueueLengthType rotEndIndex
)
{
  /* -------------- Local variables -------------------------------------------------------------------------------- */
  I2c_PrioQueueLengthType queueIndex;

  /* The local buffer to temporarily store queue entries that will be overwritten. */
  I2c_PrioQueueLengthType localReadBuffer;

  /*
   * The local write buffer stores the value that is written to the priority queue next. Beginning
   * with the rotation start index the value at the queue's end index is written to the entry of
   * the rotation start index.
   */
  /* #10 Temporarily store the value at the queue end index in the local write buffer. */
  I2c_PrioQueueLengthType localWriteBuffer = prioQueueDesc->PrioQueue[rotEndIndex % (I2c_PrioQueueLengthType)I2C_PRIO_QUEUE_BUFFER_SIZE];

  /* #20 Get the number of entries from the rotation start index to the end index. */
  I2c_PrioQueueLengthType range = I2c_QueueMgr_GetRange(rotStartIndex, rotEndIndex);
  I2c_PrioQueueLengthType rotIndex = rotStartIndex;

  /* -------------- Implementation --------------------------------------------------------------------------------- */
  /* #30 Iterate over the priority queue range-times. */
  for(queueIndex = 0; queueIndex < range; queueIndex++)
  {
    /* #40 Store the value at the current queue index entry in the local read buffer. */
    localReadBuffer = prioQueueDesc->PrioQueue[rotIndex % (I2c_PrioQueueLengthType)I2C_PRIO_QUEUE_BUFFER_SIZE];

    /* #50 Overwrite the entry at the current position with the value stored in the local write buffer. */
    prioQueueDesc->PrioQueue[rotIndex % (I2c_PrioQueueLengthType)I2C_PRIO_QUEUE_BUFFER_SIZE] = localWriteBuffer; /* SBSW_I2C_ARRAY_MOD_R */

    /* #60 Copy the value from the local read buffer to the local write buffer. */
    localWriteBuffer = localReadBuffer;

    /* #70 Increment the rotIndex. */
    rotIndex++;
  }
} /* I2c_QueueMgr_RotateRight */

/**********************************************************************************************************************
 *  I2c_QueueMgr_IncreaseStartIndex()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
I2C_LOCAL_INLINE FUNC(void, I2C_CODE) I2c_QueueMgr_IncrementStartIndex
(
  I2c_PrioQueueDescPtrType queueDescPtr
)
{
  /* -------------- Implementation --------------------------------------------------------------------------------- */
  /* #10 Increase the queue's start index. In case the end of the queue is reached, the start index begins from 0. */
  queueDescPtr->StartIndex++; /* SBSW_I2C_HWREF_WRITE_R_03 */

  /* #20 If the start index reaches the queue's end. Then the start index begins from 0. */
  if (queueDescPtr->StartIndex >= (I2c_PrioQueueLengthType)I2C_PRIO_QUEUE_BUFFER_SIZE)
  {
    queueDescPtr->StartIndex = 0; /* SBSW_I2C_HWREF_WRITE_R_03 */
  }
} /* I2c_QueueMgr_IncreaseStartIndex */

/**********************************************************************************************************************
 *  I2c_QueueMgr_IncrementEndIndex()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
I2C_LOCAL_INLINE FUNC(void, I2C_CODE) I2c_QueueMgr_IncrementEndIndex
(
  I2c_PrioQueueDescPtrType queueDescPtr
)
{
  /* -------------- Implementation --------------------------------------------------------------------------------- */
  /* #10 Increase the queue's end index. In case the end of the queue is reached, the end index begins from 0. */
  queueDescPtr->EndIndex++; /* SBSW_I2C_HWREF_WRITE_R_03 */

  /* #20 If the end index reaches the queue's end. Then the end index begins from 0. */
  if (queueDescPtr->EndIndex >= (I2c_PrioQueueLengthType)I2C_PRIO_QUEUE_BUFFER_SIZE)
  {
    queueDescPtr->EndIndex = 0; /* SBSW_I2C_HWREF_WRITE_R_03 */
  }
} /* I2c_QueueMgr_IncrementEndIndex */

/**********************************************************************************************************************
 *  I2c_QueueMgr_CheckQueueEntries()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
I2C_LOCAL FUNC(Std_ReturnType, I2C_CODE) I2c_QueueMgr_CheckQueueEntries
(
  I2c_ConstSeqDescPtrType seqDescPtr
)
{
  /* -------------- Local variables -------------------------------------------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  I2c_PrioQueueLengthType numOfEntries = 0;
  I2c_PrioQueueLengthType queueIndex;
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr = I2c_ConfigMgr_GetHwUnitDescViaSeq(seqDescPtr->SeqId);

  /* 10# Get a pointer to the sequence's corresponding priority queue. */
  I2c_ConstPrioQueueDescPtrType queueDescPtr = &(hwUnitDescPtr->varHwUnitDescPtr->PrioQueueDesc);

  /* -------------- Implementation --------------------------------------------------------------------------------- */
  /* #20 Iterate over the priority queue, count the entries and check if the given sequence is already within the queue. */
  for (queueIndex = queueDescPtr->StartIndex; (queueIndex % (I2c_PrioQueueLengthType)I2C_PRIO_QUEUE_BUFFER_SIZE) != queueDescPtr->EndIndex; queueIndex++)
  {
    /* #30 Increment the number of entries. */
    numOfEntries++;

    /* #40 If the current entry is equal to the given sequence's ID, return E_NOT_OK. */
    if (queueDescPtr->PrioQueue[queueIndex % (I2c_PrioQueueLengthType)I2C_PRIO_QUEUE_BUFFER_SIZE] == seqDescPtr->SeqId)
    {
      retVal = E_NOT_OK;
    }
  }

  /* #50 If the priority queue is already completely filled, return E_NOT_OK. */
  if (numOfEntries >= I2C_PRIO_QUEUE_DEPTH)
  {
    retVal = E_NOT_OK;
  }

  return retVal;
} /* I2c_QueueMgr_CheckQueueEntries */

/**********************************************************************************************************************
 * GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  I2c_QueueMgr_IsEmpty()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(boolean, I2C_CODE) I2c_QueueMgr_IsEmpty
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr
)
{
  /* -------------- Local variables -------------------------------------------------------------------------------- */
I2c_ConstPrioQueueDescPtrType queueDescPtr = &hwUnitDescPtr->varHwUnitDescPtr->PrioQueueDesc;

  /* -------------- Implementation --------------------------------------------------------------------------------- */
  return (queueDescPtr->StartIndex == queueDescPtr->EndIndex) ? TRUE : FALSE;
} /* I2c_QueueMgr_IsEmpty */

/**********************************************************************************************************************
 *  I2c_QueueMgr_PollSeq()
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
FUNC(I2c_ConstSeqDescPtrType, I2C_CODE) I2c_QueueMgr_PollSeq /* PRQA S 1532 */ /* MD_I2C_1532 */
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr
)
{
  /* -------------- Local variables -------------------------------------------------------------------------------- */
  I2c_ConstSeqDescPtrType retVal = NULL_PTR;

  /* -------------- Implementation --------------------------------------------------------------------------------- */
  /* #10 If the queue is not empty. */
  if (I2c_QueueMgr_IsEmpty(hwUnitDescPtr) == FALSE) /* SBSW_I2C_HW_PTR_PASS_N_02 */
  {
    /* #20 Get the HwUnit specific priority queue. */
    I2c_PrioQueueDescPtrType queueDescPtr = &hwUnitDescPtr->varHwUnitDescPtr->PrioQueueDesc;

    /* #30 Get the sequence ID with the highest priority out of the priority queue. */
    I2c_SequenceType sequenceId = queueDescPtr->PrioQueue[queueDescPtr->StartIndex];

    /* #40 Get a pointer to the sequence descriptor. */
    retVal = I2c_ConfigMgr_GetSeqDesc(sequenceId);

    /* #50 Increment the queue's start index. */
    I2c_QueueMgr_IncrementStartIndex(queueDescPtr); /* SBSW_I2C_HWREF_PASS_R_02 */
  }

  return retVal;
} /* I2c_QueueMgr_PollSeq */

/**********************************************************************************************************************
 *  I2c_QueueMgr_RemoveSeq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, I2C_CODE) I2c_QueueMgr_RemoveSeq /* PRQA S 1532 */ /* MD_I2C_1532 */
(
  I2c_ConstSeqDescPtrType seqDescPtr
)
{
  /* -------------- Local variables -------------------------------------------------------------------------------- */
  I2c_PrioQueueLengthType queueIndex;

  /* #10 Get a pointer to the sequence's corresponding priority queue. */
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr = I2c_ConfigMgr_GetHwUnitDescViaSeq(seqDescPtr->SeqId);
  I2c_PrioQueueDescPtrType queueDescPtr = &hwUnitDescPtr->varHwUnitDescPtr->PrioQueueDesc;

  /* -------------- Implementation --------------------------------------------------------------------------------- */
  /* #20 Iterate over the priority queue until the requested sequence is found. */
  for (queueIndex = queueDescPtr->StartIndex;
          (queueIndex % (I2c_PrioQueueLengthType)I2C_PRIO_QUEUE_BUFFER_SIZE) != queueDescPtr->EndIndex; queueIndex++)
  {
    if (queueDescPtr->PrioQueue[queueIndex % (I2c_PrioQueueLengthType)I2C_PRIO_QUEUE_BUFFER_SIZE] == seqDescPtr->SeqId)
    {
      /* #30 Rotate all queue's entries from the queue's start index to the entry where the sequence has been found. */
      I2c_QueueMgr_RotateRight(queueDescPtr, queueDescPtr->StartIndex, /* SBSW_I2C_HWREF_PASS_R_01 */
              (queueIndex % (I2c_PrioQueueLengthType)I2C_PRIO_QUEUE_BUFFER_SIZE));

      /* #40 Increment the queue's start index. */
      I2c_QueueMgr_IncrementStartIndex(queueDescPtr); /* SBSW_I2C_HWREF_PASS_R_01 */
    }
  }
} /* I2c_QueueMgr_RemoveSeq */

/**********************************************************************************************************************
 *  I2c_QueueMgr_AddSeq()
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
FUNC(Std_ReturnType, I2C_CODE) I2c_QueueMgr_AddSeq /* PRQA S 1532 */ /* MD_I2C_1532 */
(
  I2c_ConstSeqDescPtrType seqDescPtr
)
{
  /* -------------- Local variables -------------------------------------------------------------------------------- */
  Std_ReturnType retVal = E_NOT_OK;
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr;
  I2c_PrioQueueLengthType queueIndex;
  I2c_PrioQueueDescPtrType queueDescPtr;
  I2c_SeqPrioType sequencePrio;
  boolean posFound = FALSE;

  /* -------------- Implementation --------------------------------------------------------------------------------- */
  /* #10 Check if the requested sequence can be added to its corresponding priority queue. */
  if (I2c_QueueMgr_CheckQueueEntries(seqDescPtr) == E_OK) /* SBSW_I2C_SEQ_PTR_PASS_02 */
  {
    retVal = E_OK;
    hwUnitDescPtr = I2c_ConfigMgr_GetHwUnitDescViaSeq(seqDescPtr->SeqId);

    queueDescPtr = &hwUnitDescPtr->varHwUnitDescPtr->PrioQueueDesc;
    sequencePrio = seqDescPtr->SequencePrio;

    /* #20 Add the requested sequence ID to the queue's end index. */
    queueDescPtr->PrioQueue[queueDescPtr->EndIndex] = seqDescPtr->SeqId; /* SBSW_I2C_ARRAY_ENDINDEX */

    /*
     * #30 Iterate over the priority queue until an entry is found with an
     * priority smaller than the requested sequence's priority.
     */
    for (queueIndex = queueDescPtr->StartIndex;
            ((queueIndex % (I2c_PrioQueueLengthType)I2C_PRIO_QUEUE_BUFFER_SIZE) != queueDescPtr->EndIndex) && (posFound == FALSE); queueIndex++)
    {
      I2c_SequenceType compareSeqId = queueDescPtr->PrioQueue[queueIndex % (I2c_PrioQueueLengthType)I2C_PRIO_QUEUE_BUFFER_SIZE];
      /*
       * #40 If a proper entry has been found, perform a value rotation
       * for all entries between the current index and the queue's end index.
       */
      I2c_ConstSeqDescPtrType constSeqDescPtr =I2c_ConfigMgr_GetSeqDesc(compareSeqId);

      if (sequencePrio > constSeqDescPtr->SequencePrio)
      {
        posFound = TRUE;

        I2c_QueueMgr_RotateRight(queueDescPtr,
                (queueIndex % (I2c_PrioQueueLengthType)I2C_PRIO_QUEUE_BUFFER_SIZE), queueDescPtr->EndIndex); /* SBSW_I2C_HWREF_PASS_R_01 */
      }
    }

    /* #50 Increment the end index. */
    I2c_QueueMgr_IncrementEndIndex(queueDescPtr); /* SBSW_I2C_HWREF_PASS_R_01 */
  }

  return retVal;
} /* I2c_QueueMgr_AddSeq */


/**********************************************************************************************************************
 *  I2c_QueueMgr_ResetQueue()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, I2C_CODE) I2c_QueueMgr_ResetQueue /* PRQA S 1532 */ /* MD_I2C_1532 */
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr
)
{
  /* -------------- Local variables -------------------------------------------------------------------------------- */
  /* #10 Get the HwUnit's priority queue descriptor and set the queue's start index and end index to 0. */
  I2c_PrioQueueDescPtrType prioQueueDescPtr = &hwUnitDescPtr->varHwUnitDescPtr->PrioQueueDesc;

  /* -------------- Implementation --------------------------------------------------------------------------------- */
  prioQueueDescPtr->StartIndex = 0u; /* SBSW_I2C_HWREF_WRITE_R_02 */
  prioQueueDescPtr->EndIndex = 0u; /* SBSW_I2C_HWREF_WRITE_R_02 */
} /* I2c_QueueMgr_ResetQueue */

# define I2C_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/*********************************************************************************************************************
 * END OF FILE: I2c_PrioQueueManager.c
 *********************************************************************************************************************/
