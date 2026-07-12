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
/**        \file  vItaHlp_EventQueue.c
 *        \brief  Implements service API to use the vItaHlp event queue.
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

#define VITAHLP_EVENTQUEUE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vItaHlp_EventQueue.h"
#include "vItaHlp.h"
#include "vItaHlp_TestCom_Cfg.h"
#include "vstdlib.h"
#include "SchM_vItaHlp.h"

#if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Size of Event Buffer List (max. 255) */
# define VITAHLP_EVENTQUEUE_QUEUE_SIZE                                                                  (128u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
# if !defined (VITAHLP_LOCAL)
#  define VITAHLP_LOCAL static
# endif

# if !defined (VITAHLP_LOCAL_INLINE)
#  define VITAHLP_LOCAL_INLINE LOCAL_INLINE
# endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define VITAHLP_START_SEC_GLOBALSHARED_VAR_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* Contains number of events which are stored in the event queue */
VAR(uint8, VITAHLP_VAR_NOINIT_8BIT) vItaHlp_EventQueue_NumberOfStoredEvents;
/* Contains the index of the event, which may be sent at next time */
VAR(uint8, VITAHLP_VAR_NOINIT_8BIT) vItaHlp_EventQueue_ReadIndex;
/* Contains the index of the event, which was added recently */
VAR(uint8, VITAHLP_VAR_NOINIT_8BIT) vItaHlp_EventQueue_WriteIndex;
/* Contains queued events which can be processed in the event worker */
VAR(uint8, VITAHLP_VAR_NOINIT_8BIT)
  vItaHlp_EventQueue_Queue[VITAHLP_EVENTQUEUE_QUEUE_SIZE][VITAHLP_TESTCOMMUNICATION_MESSAGE_LENGTH];
/* Contains information if an overflow occurred (event queue full while request of storing a new event) */
VAR(uint8, VITAHLP_VAR_NOINIT_8BIT) vItaHlp_EventQueue_OverflowCounter;
# define VITAHLP_STOP_SEC_GLOBALSHARED_VAR_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  vItaHlp_EventQueue_IsEmpty()
 *********************************************************************************************************************/
/*! \brief       Evaluates if the event queue is empty or not.
 *  \details     -
 *  \return      TRUE: queue is empty
 *               FALSE: queue is not empty
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \note        ATTENTION: Local function must be executed in context of an exclusive area.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_EventQueue_IsEmpty(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_EventQueue_IsEmpty()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_EventQueue_IsEmpty(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isEmpty = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  if (vItaHlp_EventQueue_NumberOfStoredEvents == 0u)
  {
    isEmpty = TRUE;
  }
  return isEmpty;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_EventQueue_Init()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EventQueue_Init(void)
{
  vItaHlp_EventQueue_NumberOfStoredEvents = 0u;
  vItaHlp_EventQueue_ReadIndex = 0u;
  vItaHlp_EventQueue_WriteIndex = 0u;
  vItaHlp_EventQueue_OverflowCounter = 0u;
}

/**********************************************************************************************************************
 *  vItaHlp_EventQueue_Enqueue()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EventQueue_Enqueue(
  VAR(uint8, AUTOMATIC) componentIdentifier,
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadEventBuffer)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
# if(VITAHLP_TESTCOM_USE_SPINLOCK == STD_ON)
  (void)GetSpinlock(VItaHlp_Os_Spinlock);       /* No notification, if this API call fails */
# else
  SchM_Enter_vItaHlp_VITAHLP_EVENTQUEUE_EXCLUSIVE_AREA();
# endif
  if (vItaHlp_EventQueue_NumberOfStoredEvents < VITAHLP_EVENTQUEUE_QUEUE_SIZE)
  {
    /* Add event to Event Queue */
    vItaHlp_EventQueue_Queue[vItaHlp_EventQueue_WriteIndex][0u] = componentIdentifier;
    vItaHlp_EventQueue_Queue[vItaHlp_EventQueue_WriteIndex][1u] = serviceIdentifier;
    VStdLib_MemCpy(&vItaHlp_EventQueue_Queue[vItaHlp_EventQueue_WriteIndex][2u],
      payloadEventBuffer, VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH);

    /* Set the next index value to be added */
    if (vItaHlp_EventQueue_WriteIndex == VITAHLP_EVENTQUEUE_QUEUE_SIZE - 1u)
    {
      vItaHlp_EventQueue_WriteIndex = 0u;  /* Wrap around */
    }
    else
    {
      vItaHlp_EventQueue_WriteIndex++;
    }
    vItaHlp_EventQueue_NumberOfStoredEvents++;
  }
  else
  {
    /* Increment overflow counter until maximum value of datatype is reached */
    if (vItaHlp_EventQueue_OverflowCounter < 0xFFu)
    {
      vItaHlp_EventQueue_OverflowCounter++;
    }
  }
# if(VITAHLP_TESTCOM_USE_SPINLOCK == STD_ON)
  (void)ReleaseSpinlock(VItaHlp_Os_Spinlock);       /* No notification, if this API call fails */
# else
  SchM_Exit_vItaHlp_VITAHLP_EVENTQUEUE_EXCLUSIVE_AREA();
# endif
}

/**********************************************************************************************************************
 *  vItaHlp_EventQueue_Dequeue()
 *********************************************************************************************************************/
FUNC(boolean, VITAHLP_CODE) vItaHlp_EventQueue_Dequeue(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) eventPtr,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) overflowCounter)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean itemAvailable;
  uint8_least byteNumber = 0u;

  /* ----- Implementation ----------------------------------------------- */
# if(VITAHLP_TESTCOM_USE_SPINLOCK == STD_ON)
  (void)GetSpinlock(VItaHlp_Os_Spinlock);       /* No notification, if this API call fails */
# else
  SchM_Enter_vItaHlp_VITAHLP_EVENTQUEUE_EXCLUSIVE_AREA();
# endif
  itemAvailable = !vItaHlp_EventQueue_IsEmpty();
  if (itemAvailable == TRUE)
  {
    for (byteNumber = 0u; byteNumber < VITAHLP_TESTCOMMUNICATION_MESSAGE_LENGTH; byteNumber++)
    {
      eventPtr[byteNumber] = vItaHlp_EventQueue_Queue[vItaHlp_EventQueue_ReadIndex][byteNumber];
    }

    /* Set index for next event to read */
    if (vItaHlp_EventQueue_ReadIndex == VITAHLP_EVENTQUEUE_QUEUE_SIZE - 1u)
    {
      vItaHlp_EventQueue_ReadIndex = 0u;
    }
    else
    {
      vItaHlp_EventQueue_ReadIndex++;
    }
    /* Update total number of events in queue */
    vItaHlp_EventQueue_NumberOfStoredEvents--;
    /* Set the overflow counter value as return value and set it back to default (zero) */
    overflowCounter[0u] = vItaHlp_EventQueue_OverflowCounter;
    vItaHlp_EventQueue_OverflowCounter = 0u;
  }
# if(VITAHLP_TESTCOM_USE_SPINLOCK == STD_ON)
  (void)ReleaseSpinlock(VItaHlp_Os_Spinlock);       /* No notification, if this API call fails */
# else
  SchM_Exit_vItaHlp_VITAHLP_EVENTQUEUE_EXCLUSIVE_AREA();
# endif
  return itemAvailable;
}

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_EventQueue.c
 *********************************************************************************************************************/
