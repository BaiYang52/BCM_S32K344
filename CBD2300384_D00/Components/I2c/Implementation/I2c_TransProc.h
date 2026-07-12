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
 *         \file  I2c_TransProc.h
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
 *  Refer to revision history in I2c.h.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  MULTIPLE INCLUSION PROTECTION
 *********************************************************************************************************************/
#if !defined I2C_TRANSPROCESSOR_H
# define I2C_TRANSPROCESSOR_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
# include "I2c_PrioQueueMgr.h"

/**********************************************************************************************************************
 * GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define I2C_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

/**********************************************************************************************************************
 *  I2c_TransProcessor_ProcessHwUnit()
 *********************************************************************************************************************/
/*! \brief          Processes the requested HwUnit.
 *  \details        This service processes the requested HwUnit. This service checks if the requested HwUnit is idle.
 *                  In case a sequence is registered at the HwUnit to be transmitted, the transmission of the next channel
 *                  is initialized. In case there's no sequence to be transmitted, this service polls a sequence from
 *                  the HwUnit's prioQueue.
 *  \param[in]      Pointer to the HwUnit descriptor
 *  \pre            -
 *  \context        TASK|ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_TransProcessor_ProcessHwUnit
(
  I2c_ConstHwUnitDescPtrType hwUnitDescPtr
);

/**********************************************************************************************************************
 *  I2c_TransProcessor_RegisterSeq()
 *********************************************************************************************************************/
/*! \brief          Registers the requested sequence for transmission.
 *  \details        The Transmission Processor checks if the sequence's corresponding HwUnit is either idle or busy.
 *                  In case the HwUnit is idle, the sequence is registered for transmission. In case the HwUnit is busy, the
 *                  sequence is queued within the HwUnit's priority queue.
 *  \param[in]      Pointer to the sequence descriptor.
 *  \pre            -
 *  \return         E_OK:         - The sequence is either registered at the HwUnit for transmission, or queued in the priority queue.
 *  \return         E_NOT_OK:     - The HwUnit is busy and the priority queue's limit is already reached, so the sequence can not
 *                  queued.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(Std_ReturnType, I2C_CODE) I2c_TransProcessor_RegisterSeq
(
  I2c_ConstSeqDescPtrType seqDescPtr
);

/**********************************************************************************************************************
 *  I2c_TransProcessor_ReportResult()
 *********************************************************************************************************************/
/*! \brief          This notification is called by the Low Level module in case a transmission is finished.
 *  \details        In case an ongoing transmission is finished, this service is called by the Low Level module to
 *                  report the channel's current result.
 *  \param[in]      HwUnit ID
 *  \param[in]      Channel result
 *  \pre            -
 *  \context        ISR
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_TransProcessor_ReportResult
(
  I2c_HwUnitType hwUnitId,
  I2c_ChannelResultType channelResult
);

/**********************************************************************************************************************
 *  I2c_TransProcessor_CancelSequence()
 *********************************************************************************************************************/
/*! \brief          This service cancels the requested sequence's transmission.
 *  \details        In case the sequence is already within a transmission process, the process is aborted as soon as
 *                  the sequence's ongoing channel transmission is finished. In case the sequence is within the
 *                  the priority queue, the queue entry is deleted.
 *  \param[in]      Pointer to the sequence descriptor.
 *  \pre          -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_TransProcessor_CancelSeq
(
  I2c_ConstSeqDescPtrType seqDescPtr
);

/**********************************************************************************************************************
 *  I2c_TransProcessor_SetHandling()
 *********************************************************************************************************************/
/*! \brief          Sets the type of timeout handling.
 *  \details        There are two different ways to handle a time out.
 *                  In BURST mode the bus is detached.
 *                  In RECURRENT mode the driver transmits a specific signal
 *                  to free the bus.
 *  \param[in]      Handling type
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
FUNC(void, I2C_CODE) I2c_TransProcessor_SetHandling
(
  I2c_HandlingType handling
);

# define I2C_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* I2C_TRANSPROCESSOR_H */

/*********************************************************************************************************************
 *  END OF FILE: I2c_TransProcessor.h
 *********************************************************************************************************************/
