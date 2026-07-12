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
/**        \file  I2c_PrioQueueMgr.h
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
 *  Refer to revision history in I2c.h.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  MULTIPLE INCLUSION PROTECTION
 *********************************************************************************************************************/
#if !defined I2C_PRIOQUEUEMANAGER_H
# define I2C_PRIOQUEUEMANAGER_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "I2c_Cfg.h"

/**********************************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define I2C_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  I2c_QueueMgr_IsEmpty()
 *********************************************************************************************************************/
/*! \brief          This service checks if the requested hwUnit's priority queue is empty.
 *  \details        -
 *  \param[in]      Pointer to the processed HwUnit descriptor.
 *  \return         TRUE:         - The queue is empty.
 *  \param[out]     FALSE:        - The queue is not empty.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(boolean, I2C_CODE) I2c_QueueMgr_IsEmpty
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr
);

/**********************************************************************************************************************
 *  I2c_QueueMgr_PollSeq()
 *********************************************************************************************************************/
/*! \brief          Returns the first entry within the requested HwUnit's priority queue.
 *  \details        This service returns the first entry within the requested HwUnit's priority queue
 *                  and increases the StartIndex by one.
 *  \param[in]      Pointer to the HwUnit descriptor.
 *  \return         Pointer to the requested sequence descriptor.
 *  \return         NULL_PTR:         - There's no entry within the priority queue.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(I2c_ConstSeqDescPtrType, I2C_CODE) I2c_QueueMgr_PollSeq
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr
);

/**********************************************************************************************************************
 *  I2c_QueueMgr_RemoveSeq()
 *********************************************************************************************************************/
/*! \brief          Removes the requested sequence from its corresponding priority queue.
 *  \details        This service determines the corresponding priority queue of the requested sequence
 *                  and removes the sequence from the queue.
 *  \param[in]      Pointer to the processed sequence descriptor.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_QueueMgr_RemoveSeq
(
  I2c_ConstSeqDescPtrType seqDescPtr
);

/**********************************************************************************************************************
 *  I2c_QueueMgr_AddSeq()
 *********************************************************************************************************************/
/*! \brief          Adds the requested sequence to its corresponding queue.
 *  \details        This service adds the requested sequence to its corresponding priority queue, with respect to its
 *                  priority.
 *  \param[in]      Pointer to the processed sequence descriptor.
 *  \return         E_OK              - Sequence has been added to its corresponding priority queue.
 *  \return         E_NOT_OK          - Sequence is already within the priority queue, or the queue is full.
 *  \pre            -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, I2C_CODE) I2c_QueueMgr_AddSeq
(
  I2c_ConstSeqDescPtrType seqDescPtr
);

/**********************************************************************************************************************
 *  I2c_QueueMgr_ResetQueue()
 *********************************************************************************************************************/
/*! \brief          Empties the requested HwUnit's priority queue.
 *  \details        In case a specific configuration is used more than once, it is possible that there are
 *                  entries within a HwUnit's priority queue. These entries have to be deleted.
 *  \param[in]      HwUnit descriptor pointer
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_QueueMgr_ResetQueue
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr
);

# define I2C_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* I2C_PRIOQUEUEMANAGER_H */

/*********************************************************************************************************************
 *  END OF FILE: PrioQueueManager.h
 *********************************************************************************************************************/
