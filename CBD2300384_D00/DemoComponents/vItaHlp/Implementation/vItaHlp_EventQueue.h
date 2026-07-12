/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2019 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  vItaHlp_EventQueue.h
 *        \brief  Service API declaration for the vItaHlp event queue.
 *
 *      \details  -
 *
 *    \attention  Please note:
 *                The demo and example programs only show special aspects of the software. With regard to the fact
 *                that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *                expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/

#if !defined (VITAHLP_EVENTQUEUE_H)
# define VITAHLP_EVENTQUEUE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  vItaHlp_EventQueue_Init()
 *********************************************************************************************************************/
/*! \brief       Initializes the event queue by resetting all variables and data to default values.
 *  \details     -
 *  \pre         -
 *  \context     TASK|ISR
 *  \synchronous TRUE
 *  \reentrant   TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EventQueue_Init(void);

/**********************************************************************************************************************
 *  vItaHlp_EventQueue_Enqueue()
 *********************************************************************************************************************/
/*! \brief       Fills the given event data to the queue.
 *  \details     If an event can not be stored in queue, because it is full, an internal overflow bit is set.
 *               This bit can be requested by the corresponding service API.
 *  \param[in]   componentIdentifier      Identifier of the requested component.
 *  \param[in]   serviceIdentifier        Identifier of the requested service.
 *  \param[in]   payloadEventBuffer       Pointer which contains the payload content for an event.
 *                                        Length of the buffer is required to be 6 and must be guaranteed by caller.
 *  \pre         Event queue is initialized by calling the appropriate init function.
 *  \context     TASK|ISR
 *  \synchronous TRUE
 *  \reentrant   TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EventQueue_Enqueue(
  VAR(uint8, AUTOMATIC) componentIdentifier,
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadEventBuffer);

/**********************************************************************************************************************
 *  vItaHlp_EventQueue_Dequeue()
 *********************************************************************************************************************/
/*! \brief       Returns the next event of the queue if it is not empty.
 *  \details     -
 *  \param[out]  eventPtr Contains the content of the next queue element
 *  \param[out]  overflowCounter Contains a counter how often the queue gets overflowed.
 *               The corresponding counter value is resetted after calling this function.
 *  \return      TRUE: content of eventPtr and overflowCounter is valid
 *               FALSE: content of eventPtr is not modified, queue is empty
 *  \pre         Event queue is initialized by calling the appropriate init function.
 *  \context     TASK|ISR
 *  \synchronous TRUE
 *  \reentrant   TRUE
 *********************************************************************************************************************/
FUNC(boolean, VITAHLP_CODE) vItaHlp_EventQueue_Dequeue(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) eventPtr,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) overflowCounter);

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* VITAHLP_EVENTQUEUE_H */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_EventQueue.h
 *********************************************************************************************************************/
