/* ********************************************************************************************************************
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
 *  -------------------------------------------------------------------------------------------------------------------
 */
/*!
 *  \addtogroup Dem_ClearTask
 *  \{
 *  \file       Dem_ClearTask_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of ClearTask subcomponent
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_CLEARTASK_IMPLEMENTATION_H)
#define DEM_CLEARTASK_IMPLEMENTATION_H

/* ********************************************************************************************************************
 *  GLOBAL MISRA JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2880 EOF */ /* MD_DEM_2.1_UNREACHABLE_STATEMENT */
/* PRQA S 2982 EOF */ /* MD_DEM_2982 */
/* PRQA S 2983 EOF */ /* MD_DEM_2983 */
/* PRQA S 2985 EOF */ /* MD_DEM_2985 */
/* PRQA S 2986 EOF */ /* MD_DEM_2986 */
/* PRQA S 2987 EOF */ /* MD_DEM_2987 */
/* PRQA S 2991 EOF */ /* MD_DEM_2991 */
/* PRQA S 2992 EOF */ /* MD_DEM_2992 */
/* PRQA S 2995 EOF */ /* MD_DEM_2995 */
/* PRQA S 2996 EOF */ /* MD_DEM_2996 */
/* PRQA S 0759 EOF */ /* MD_MSR_Union */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_ClearTask_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_DTCSelector_Interface.h"
#include "Dem_ClearDTC_Interface.h"
#include "Dem_ClientAccess_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_CLEARTASK_IMPLEMENTATION_FILENAME "Dem_ClearTask_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

#define DEM_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! State of clear requests */
DEM_LOCAL volatile VAR(uint8, DEM_VAR_NOINIT)    Dem_ClearTask_ClearState;

/*! Flag to indicate write errors during clear */
DEM_LOCAL VAR(boolean, DEM_VAR_NOINIT)           Dem_ClearTask_ClearWriteErrors;                                                 /* PRQA S 3218 */ /* MD_DEM_8.7 */

/*! Flag to indicate that the clear operation is finished */
DEM_LOCAL VAR(boolean, DEM_VAR_NOINIT)           Dem_ClearTask_ClearFinished;

#define DEM_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DEM_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Run-time limitation state for clear requests */
DEM_LOCAL VAR(uint16, DEM_VAR_NOINIT)            Dem_ClearTask_ClearEventIterator;

#define DEM_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DEM_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! ClearRequest data */
DEM_LOCAL VAR(Dem_ClearTask_ClearRequestType, DEM_VAR_NOINIT)  Dem_ClearTask_ClearRequest;

#define DEM_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_ClearTask_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Client_ClearTask_ClearDTCRuntimeLimit_IsEnabled()
 *****************************************************************************/
/*!
 * \brief         Returns if feature ClearDTC Runtime Limit is enabled.
 *
 * \details       Returns if feature ClearDTC Runtime Limit is enabled.
 *
 * \return        True: Feature ClearDTC Runtime Limit is enabled.
 *                False: Feature ClearDTC Runtime Limit is disabled.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClearTask_ClearDTCRuntimeLimit_IsEnabled(
  void
  );

/* ****************************************************************************
* Dem_Client_ClearTask_GlobalClearDtcRuntimeLimit
*****************************************************************************/
/*!
* \brief         Get the clear DTC runtime limit
*
* \details       Get the clear DTC runtime limit
*
* \return        The clear DTC runtime limit
*
* \pre           -
* \config        -
*
* \context       TASK|ISR2
* \synchronous   TRUE
* \reentrant     TRUE
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_GlobalClearDtcRuntimeLimit(
  void
  );

/* ****************************************************************************
 * Dem_ClearTask_GetClearMemoryId()
 *****************************************************************************/
/*!
 * \brief         Get the memory id of the current clear operation.
 *
 * \details       Get the memory id of the current clear operation.
 *
 * \return        The memory id of the current clear operation.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE) 
Dem_ClearTask_GetClearMemoryId(
  void
  );


/* ****************************************************************************
 * Dem_ClearTask_SetClearMemory()
 *****************************************************************************/
/*!
 * \brief         Set the memory id of the current clear operation.
 *
 * \details       Set the memory id of the current clear operation.
 *
 * \param[in]     MemoryId
 *                The new memory id of the current clear operation
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_ClearTask_SetClearMemory(
  CONST(uint16, AUTOMATIC) MemoryId
  );


/* ****************************************************************************
 * Dem_ClearTask_GetClearGroup()
 *****************************************************************************/
/*!
 * \brief         Get the clear group of the current clear operation.
 *
 * \details       Get the clear group of the current clear operation.
 *
 * \return        The clear group of the current clear operation.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE) 
Dem_ClearTask_GetClearGroup(
  void
  );


/* ****************************************************************************
 * Dem_ClearTask_GetClearEvent()
 *****************************************************************************/
/*!
 * \brief         Get the clear event of the current clear operation.
 *
 * \details       Get the clear event of the current clear operation.
 *
 * \return        The clear event of the current clear operation.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE) 
Dem_ClearTask_GetClearEvent(
  void
  );


/* ****************************************************************************
 * Dem_ClearTask_SetClearEvent()
 *****************************************************************************/
/*!
 * \brief         Set the clear event of the current clear operation.
 *
 * \details       Set the clear event of the current clear operation.
 *
 * \param[in]     EventId
 *                The new clear event of the current clear operation
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_ClearTask_SetClearEvent(
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  );


/* ****************************************************************************
 * Dem_ClearTask_GetClearFilter()
 *****************************************************************************/
/*!
 * \brief         Get the clear filter of the current clear operation.
 *
 * \details       Get the clear filter of the current clear operation.
 *
 * \return        The clear code of the current clear operation.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE) 
Dem_ClearTask_GetClearFilter(
  void
  );

/* ****************************************************************************
 * Dem_ClearTask_GetClearNodeIndex()
 *****************************************************************************/
/*!
 * \brief         Get the clear node of the current clear operation.
 *
 * \details       Get the clear node of the current clear operation.
 *
 * \return        The clear node of the current clear operation.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE) 
Dem_ClearTask_GetClearNodeIndex(
  void
  );


#if (DEM_CFG_CLEAR_NV_END_RESPONSE == STD_ON)
/* ****************************************************************************
 * Dem_ClearTask_TestClearWriteErrors()
 *****************************************************************************/
/*!
 * \brief         Test the write errors flag of the current clear operation.
 *
 * \details       Test the write errors flag of the current clear operation.
 *
 * \return        TRUE
 *                The write errors flag was set
 * \return        FALSE
 *                The write errors flag was not set
 *
 * \pre           -
 * \config        DEM_CFG_CLEAR_NV_END_RESPONSE == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE) 
Dem_ClearTask_TestClearWriteErrors(
  void
  );
#endif


#if (DEM_CFG_CLEAR_NV_END_RESPONSE == STD_ON)
/* ****************************************************************************
 * Dem_ClearTask_SetClearWriteErrors()
 *****************************************************************************/
/*!
 * \brief         Set the write errors flag of the current clear operation.
 *
 * \details       Set the write errors flag of the current clear operation.
 *
 * \pre           -
 * \config        DEM_CFG_CLEAR_NV_END_RESPONSE == STD_ON
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_ClearTask_SetClearWriteErrors(
  void
  );
#endif


/* ****************************************************************************
 * Dem_ClearTask_ResetClearWriteErrors()
 *****************************************************************************/
/*!
 * \brief         Reset the write errors flag of the current clear operation.
 *
 * \details       Reset the write errors flag of the current clear operation.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_ClearTask_ResetClearWriteErrors(
  void
  );

/* ****************************************************************************
 * Dem_ClearTask_GetClearState()
 *****************************************************************************/
/*!
 * \brief         Get the status of the current clear operation.
 *
 * \details       Get the status of the current clear operation.
 *
 * \return        The status of the clear operation
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE) 
Dem_ClearTask_GetClearState(
  void
  );

/* ****************************************************************************
 * Dem_Client_ClearTask_ClearingInternalEventsRequired()
 *****************************************************************************/
/*!
 * \brief         Test if the events without DTC need to be cleared while 
 *                clearing the input TranslatedMemoryId
 *
 * \details       Test if the events without DTC need to be cleared while 
 *                clearing the input TranslatedMemoryId
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of the event memory
 *
 * \return        TRUE
 *                The events without DTC need to be cleared
 * \return        FALSE
 *                No events without DTC need to be cleared
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE) 
Dem_Client_ClearTask_ClearingInternalEventsRequired(
  uint16 TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_Client_ClearTask_GetClearInternalEventsMemoryId
 *****************************************************************************/
/*!
 * \brief         Get the id of the event memory, which is referenced to clear 
 *                the events without DTC
 *
 * \details       Get the id of the event memory, which is referenced to clear 
 *                the events without DTC
 *
 * \return        Memory Id of the event memory, which is referenced to clear 
 *                the events without DTC
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_GetClearInternalEventsMemoryId(
  void
  );

/* ****************************************************************************
 * Dem_Client_ClearTask_IsClearInternalEventsSupported
 *****************************************************************************/
/*!
 * \brief         Test if clearing events without DTC is supported
 *
 * \details       Test if clearing events without DTC is supported
 *
 * \return        TRUE
 *                Clearing events without DTC is supported
 * \return        FALSE
 *                Clearing events without DTC is not supported
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClearTask_IsClearInternalEventsSupported(
  void
  );

/* ****************************************************************************
 * Dem_Client_ClearTask_GetFirstInternalEvent
 *****************************************************************************/
/*!
 * \brief         Get the Id of the first event without DTC
 *
 * \details       Get the Id of the first event without DTC
 *
 * \return        The Id of the first event without DTC
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_GetFirstInternalEvent(
  void
  );

/* ****************************************************************************
 * Dem_Client_ClearTask_GetLastInternalEvent
 *****************************************************************************/
/*!
 * \brief         Get the Id of the last event without DTC
 *
 * \details       Get the Id of the last event without DTC
 *
 * \return        The Id of the last event without DTC
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_GetLastInternalEvent(
  void
  );

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE PROPERTIES DEFINTIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Client_ClearTask_ClearDTCRuntimeLimit_IsEnabled()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
*****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClearTask_ClearDTCRuntimeLimit_IsEnabled(
  void
  )
{
  return (boolean)(DEM_CFG_SUPPORT_CLEAR_DTC_RUNTIME_LIMIT == STD_ON);
}

/* ****************************************************************************
% Dem_Client_ClearTask_GlobalClearDtcRuntimeLimit()
*****************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_GlobalClearDtcRuntimeLimit(
void
)
{
#if (DEM_CFG_SUPPORT_CLEAR_DTC_RUNTIME_LIMIT == STD_ON)
  return DEM_CFG_GLOBAL_CLEAR_DTC_RUNTIME_LIMIT;
#else
  return 0x00;
#endif
}

/* ****************************************************************************
 % Dem_ClearTask_GetClearMemoryId()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE) 
Dem_ClearTask_GetClearMemoryId(
  void
  )
{
  return Dem_ClearTask_ClearRequest.ClearMemoryId;
}

/* ****************************************************************************
 % Dem_ClearTask_SetClearMemory()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_ClearTask_SetClearMemory(                                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(uint16, AUTOMATIC) MemoryId
  )
{
  Dem_ClearTask_ClearRequest.ClearMemoryId = MemoryId;
}

/* ****************************************************************************
 % Dem_ClearTask_GetClearGroup()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE) 
Dem_ClearTask_GetClearGroup(
  void
  )
{
  return Dem_ClearTask_ClearRequest.ClearGroup;
}

/* ****************************************************************************
 % Dem_ClearTask_GetClearEvent()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EventIdType, DEM_CODE) 
Dem_ClearTask_GetClearEvent(
  void
  )
{
  return Dem_ClearTask_ClearRequest.ClearEventId;
}

/* ****************************************************************************
 % Dem_ClearTask_SetClearEvent()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_ClearTask_SetClearEvent(                                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  CONST(Dem_EventIdType, AUTOMATIC) EventId
  )
{
  Dem_ClearTask_ClearRequest.ClearEventId = EventId;
}

/* ****************************************************************************
 % Dem_ClearTask_GetClearFilter()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE) 
Dem_ClearTask_GetClearFilter(                                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  return Dem_ClearTask_ClearRequest.ClearFilter;
}

/* ****************************************************************************
 % Dem_ClearTask_GetClearNodeIndex()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE) 
Dem_ClearTask_GetClearNodeIndex(                                                                                                 /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  return Dem_ClearTask_ClearRequest.ClearNode;
}

# if (DEM_CFG_CLEAR_NV_END_RESPONSE == STD_ON)
/* ****************************************************************************
 % Dem_ClearTask_TestClearWriteErrors()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE) 
Dem_ClearTask_TestClearWriteErrors(                                                                                              /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  return Dem_ClearTask_ClearWriteErrors;
}
#endif

# if (DEM_CFG_CLEAR_NV_END_RESPONSE == STD_ON)
/* ****************************************************************************
 % Dem_ClearTask_SetClearWriteErrors()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_ClearTask_SetClearWriteErrors(                                                                                               /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  Dem_ClearTask_ClearWriteErrors = TRUE;
}
#endif

/* ****************************************************************************
 % Dem_ClearTask_ResetClearWriteErrors()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_ClearTask_ResetClearWriteErrors(                                                                                             /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  Dem_ClearTask_ClearWriteErrors = FALSE;
}

/* ****************************************************************************
 % Dem_ClearTask_GetClearState()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE) 
Dem_ClearTask_GetClearState(                                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  void
  )
{
  return Dem_ClearTask_ClearState;
}

/* ****************************************************************************
 % Dem_Client_ClearTask_IsClearInternalEventsSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClearTask_IsClearInternalEventsSupported(
  void
)
{
  return (boolean)(DEM_CFG_CLEAR_EVENTS_WITHOUT_DTC_EVENT_MEMORY_ID != DEM_CFG_MEMORYID_INVALID);
}

/* ****************************************************************************
 % Dem_Client_ClearTask_GetClearInternalEventsMemoryId
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_GetClearInternalEventsMemoryId(
  void
)
{
  return (uint16)(DEM_CFG_CLEAR_EVENTS_WITHOUT_DTC_EVENT_MEMORY_ID);
}

/* ****************************************************************************
 % Dem_Client_ClearTask_ClearingInternalEventsRequired()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE) 
Dem_Client_ClearTask_ClearingInternalEventsRequired(                                                                                    /* PRQA S 3219 */ /* MD_DEM_14.1_ACCESSOR */
  uint16 TranslatedMemoryId
  )
{
  boolean lReturnValue;

  if ((Dem_Client_ClearTask_IsClearInternalEventsSupported() == TRUE)
    && (Dem_Client_ClearTask_GetClearInternalEventsMemoryId() == TranslatedMemoryId)
    && (Dem_ClearTask_GetClearNodeIndex() == Dem_Cfg_GetInvalidJ1939NodeIndex()))
  {
    lReturnValue = TRUE;
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Client_ClearTask_GetFirstInternalEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_GetFirstInternalEvent(
  void
)
{
  return (uint16)(DEM_CFG_GLOBAL_INTERNAL_EVENT_FIRST);
}

/* ****************************************************************************
 % Dem_Client_ClearTask_GetLastInternalEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_GetLastInternalEvent(
  void
)
{
  return (uint16)(DEM_CFG_GLOBAL_INTERNAL_EVENT_LAST);
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"  


/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_ClearTask_PublicProperties
 * \{
 */

/* ****************************************************************************
 % Dem_ClearTask_GetClearClient()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE) 
Dem_ClearTask_GetClearClient(
  void
  )
{
  return Dem_ClearTask_ClearRequest.ClearClient;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_ClearTask_Private Private Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Client_ClearTask_TestClearExternalEventsInProgress
 *****************************************************************************/
/*!
 * \brief         Tests if clearing of external events is in progress.
 *
 * \details       Tests if clearing of external events is in progress.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of the event memory.
 * \return        TRUE
 *                Clearing of external events is in progress.
 * \return        FALSE
 *                clearing of external events is not in progress.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClearTask_TestClearExternalEventsInProgress(
  CONST(uint16, AUTOMATIC)  TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_ClearTask_ProcessClearSingleDtc
 *****************************************************************************/
/*!
 * \brief         Clears a single dtc
 *
 * \details       This function clears a single DTC and manages the related
 *                API state transitions.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ClearTask_ProcessClearSingleDtc(
  void
  );


/* ****************************************************************************
 * Dem_ClearTask_DTCCleared
 *****************************************************************************/
/*!
 * \brief         Clear the DTC and update global state of clear progress.
 *
 * \details       Clear the DTC and update global state of clear progress.
 *
 * \param[in]     EventIterator
 *                Master id of the DTC that shall be cleared.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_ClearTask_DTCCleared(
  CONST(uint16, AUTOMATIC) EventIterator
  );


/* ****************************************************************************
 * Dem_ClearTask_ProcessClearDtcGroup
 *****************************************************************************/
/*!
 * \brief         Process UDS clear dtc group requests
 *
 * \details       The function processes clearing for the events in range
 *                [FirstEventId, LastEventId] (inclusive interval) that match
 *                the ClearDTC parameters.
 *
 * \param[in]     FirstEventId
 *                Numerically smallest eventid belonging to the range
 * \param[in]     LastEventId
 *                Numerically largest eventid belonging to the range
 *
 * \pre           The global clear DTC context must have been initialized
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_ClearTask_ProcessClearDtcGroup(
  CONST(uint16, AUTOMATIC)  FirstEventId,
  CONST(uint16, AUTOMATIC)  LastEventId
  );

#if (DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON)
/* ****************************************************************************
 * Dem_ClearTask_TestDtcMatchesJ1939ClearFilter
 *****************************************************************************/
/*!
 * \brief         Tests if the DTC matches the given clear filter
 *
 * \details       Tests if the DTC corresponding the given EventId matches 
 *                the given clear filter
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 * \param[out]    TRUE: DTC matches the clear filter
 *                FALSE: DTC does not match the clear filter
 *
 * \pre           The global clear DTC context must have been initialized
 * \config        DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_ClearTask_TestDtcMatchesJ1939ClearFilter(
  const Dem_EventIdType EventId
  );
#endif

#if (DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON)
/* ****************************************************************************
 * Dem_ClearTask_ProcessClearJ1939Dtc
 *****************************************************************************/
/*!
 * \brief         Process J1939 clear dtc requests
 *
 * \details       The function processes clearing for the events in range
 *                [FirstEventId, LastEventId] (inclusive interval) that match
 *                the J1939 ClearDTC parameters.
 *
 * \param[in]     FirstEventId
 *                Numerically smallest eventid belonging to the range
 * \param[in]     LastEventId
 *                Numerically largest eventid belonging to the range
 *
 * \pre           The global clear DTC context must have been initialized
 * \config        DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_ClearTask_ProcessClearJ1939Dtc(
  CONST(uint16, AUTOMATIC)  FirstEventId,
  CONST(uint16, AUTOMATIC)  LastEventId
  );
#endif


/* ****************************************************************************
 ! Dem_ClearTask_ProcessClearDtc
 *****************************************************************************/
/*!
 * \brief         Processes clear dtc requests
 *
 * \details       This function processes clear DTC requests. Group clear is
 *                optionally distributed across multiple tasks, single clear
 *                is processed immediately.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ClearTask_ProcessClearDtc(
  void
  );


/* ****************************************************************************
 * Dem_ClearTask_SetClearState()
 *****************************************************************************/
/*!
 * \brief         Set the status of the current clear operation.
 *
 * \details       Set the status of the current clear operation.
 *
 * \param[in]     State
 *                The new status of the current clear operation
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different ClearDTCHandle
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_ClearTask_SetClearState(
  CONST(uint8, AUTOMATIC) State
  );

/* ****************************************************************************
 * Dem_Client_ClearTask_CalculateFirstEventToClear()
 *****************************************************************************/
/*!
 * \brief         Calculate the first event to be cleared.
 *
 * \details       Calculate the first event to be cleared, after considering
 *                the ClearDTC Runtime limit.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of Memory
 * \param[in]     ClearDTCRuntimeLimit
 *                The limit of events which can still be cleared.
 * \return        The first to be cleared event Id.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE) 
Dem_Client_ClearTask_CalculateFirstEventToClear(
  uint16 TranslatedMemoryId,
  uint16 ClearDTCRuntimeLimit
  );

/* ****************************************************************************
 * Dem_Client_ClearTask_CalculateLastEventToClear()
 *****************************************************************************/
/*!
 * \brief         Calculate the last event to be cleared.
 *
 * \details       Calculate the last event to be cleared.
 *
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of Memory 
 * \return        The last to be cleared event Id.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_CalculateLastEventToClear(
  uint16 TranslatedMemoryId
  );

/* ****************************************************************************
 * Dem_Client_ClearTask_CalculateFirstInternalEventToClear()
 *****************************************************************************/
/*!
 * \brief         Calculate the first internal event to be cleared.
 *
 * \details       Calculate the first internal event to be cleared, after considering
 *                the ClearDTC Runtime limit.
 *
 * \param[in]     ClearDTCRuntimeLimit
 *                The limit of events which can still be cleared.
 * \return        The first to be cleared internal event Id.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_CalculateFirstInternalEventToClear(
  uint16 ClearDTCRuntimeLimit
  );

/* ****************************************************************************
 * Dem_Client_ClearTask_CalculateLastInternalEventToClear()
 *****************************************************************************/
/*!
 * \brief         Calculate the last internal event to be cleared.
 *
 * \details       Calculate the last internal event to be cleared.
 *
 * \return        The last internal event id to be cleared.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_CalculateLastInternalEventToClear(
  void
  );

/* ****************************************************************************
 * Dem_Client_ClearTask_UpdateEventClearIterator()
 *****************************************************************************/
/*!
 * \brief         Update the event clear iterator.
 *
 * \details       Update the event clear iterator.
 *
 * \param[in]     FirstClearedEventId
 *                The first cleared event Id.
 * \param[in]     TranslatedMemoryId
 *                Unique identifier of the event memory.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE, for different ClearDTCHandle
  *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE) 
Dem_Client_ClearTask_UpdateEventClearIterator(
  uint16 FirstClearedEventId,
  uint16 TranslatedMemoryId
  );

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Client_ClearTask_TestClearExternalEventsInProgress()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Client_ClearTask_TestClearExternalEventsInProgress(
  CONST(uint16, AUTOMATIC)  TranslatedMemoryId
  )
{
  boolean lRetVal;

  if (Dem_Client_ClearTask_ClearDTCRuntimeLimit_IsEnabled() == TRUE)
  {
    lRetVal = (boolean)(Dem_ClearTask_ClearEventIterator <= Dem_Memory_GetLastEvent(TranslatedMemoryId));
  }
  else
  {
    lRetVal = TRUE;
  }

  return lRetVal;
}

/* ****************************************************************************
 % Dem_ClearTask_SetClearState()
 *****************************************************************************/
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_ClearTask_SetClearState(
  CONST(uint8, AUTOMATIC) State
  )
{
  switch (State)
  {
  case DEM_CLEARTASK_IDLE:
    Dem_ClearTask_ResetClearWriteErrors();
    Dem_Scheduler_DisableTask(Dem_Scheduler_Task_ClearTask);
    break;

  case DEM_CLEARTASK_NEW:
    Dem_Scheduler_EnableTask(Dem_Scheduler_Task_ClearTask);
    break;

  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
    /* Do nothing */
    break;
  }

  Dem_ClearDTC_NotifyClearResult(State);
  Dem_ClearTask_ClearState = (State);
}


/* ****************************************************************************
 % Dem_ClearTask_ProcessClearSingleDtc
 *****************************************************************************/
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ClearTask_ProcessClearSingleDtc(
  void
  )
{
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (Dem_ClearTask_GetClearMemoryId() != Dem_Cfg_EventDestination(Dem_ClearTask_GetClearEvent()))
  {
    Dem_Error_RunTimeCheckFailed(DEM_CLEARTASK_IMPLEMENTATION_FILENAME, __LINE__);                                               /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
#endif
  
  Dem_ClearTask_ClearFinished = Dem_DTC_Cleared(Dem_ClearTask_GetClearEvent());
  
#if (DEM_CFG_NOTIFY_CLEAR_ALLOWED == STD_ON)
  if (Dem_ClearTask_ClearFinished == FALSE)
  {
    Dem_ClearTask_SetClearState(DEM_CLEARTASK_FAILED);
  }
#endif
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_ClearTask_DTCCleared
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE) 
Dem_ClearTask_DTCCleared(
  CONST(uint16, AUTOMATIC) EventIterator
  )
{
#if (DEM_CFG_NOTIFY_CLEAR_ALLOWED == STD_ON)
  boolean lReturnValue;
  if (Dem_ClearTask_GetClearState() == DEM_CLEARTASK_INPROGRESS)
  {
    /*Remember that a suitable DTC was found*/
    Dem_ClearTask_SetClearState(DEM_CLEARTASK_INPROGRESS_DTCFOUND);
  }

  lReturnValue = Dem_DTC_Cleared(EventIterator);

  if ((Dem_ClearTask_GetClearState() == DEM_CLEARTASK_INPROGRESS_DTCFOUND) && (lReturnValue == TRUE))
  {
    /*Remember that a suitable DTC was cleared -> give positive response to clear request*/
    Dem_ClearTask_SetClearState(DEM_CLEARTASK_INPROGRESS_DTCCLEARED);
  }
#else
  (void)Dem_DTC_Cleared(EventIterator);
#endif
}

/* ****************************************************************************
 % Dem_ClearTask_ProcessClearDtcGroup
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_ClearTask_ProcessClearDtcGroup(
  CONST(uint16, AUTOMATIC)  FirstEventId,
  CONST(uint16, AUTOMATIC)  LastEventId
  )
{
  if (FirstEventId != DEM_EVENT_INVALID)
  {
    uint8 lGroupMask;
    uint16 lEventIterator;

    lGroupMask = Dem_ClearTask_GetClearGroup();
    lEventIterator = LastEventId;

    while (FirstEventId <= lEventIterator)
    { /* Event must match the group */
      if (( (lGroupMask == DEM_CFG_GROUPMASK_ALLDTCS)
#if (DEM_CFG_CLEAR_GROUP_DTCS == STD_ON)
          || ((Dem_Cfg_EventGroupMask(lEventIterator) & lGroupMask) != 0u)                                                        
#endif
          )
      /* Do not test for suppression: suppressed DTCs are not exempt from ClearGroupOfDTC */
        && (Dem_DTC_TestDtcAvailableInVariant(lEventIterator) == TRUE)                                                           
        && (Dem_DTC_TestDtcAvailable(lEventIterator) == TRUE)                                                                    
        )

      { /* For combined events, only clear each event once.
           All subevents shall be cleared in context of master event together. */
#if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
        if (lEventIterator == Dem_Cfg_GetMasterEvent(lEventIterator))
#endif
        {
          Dem_ClearTask_DTCCleared(lEventIterator);
        }
      }
      --lEventIterator;
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

#if (DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON)
/* ****************************************************************************
 % Dem_ClearTask_TestDtcMatchesJ1939ClearFilter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_ClearTask_TestDtcMatchesJ1939ClearFilter(
  const Dem_EventIdType EventId
)
{
  boolean lEventMatch;

  switch (Dem_ClearTask_GetClearFilter())
  {
  case DEM_J1939DTC_CLEAR_ACTIVE:
    if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
    {
      /* Clear all DTCs if OBD is supported */
      lEventMatch = TRUE;
    }
    else
    {
      lEventMatch = (Dem_DTC_TestDtcActive(EventId) == TRUE);
    }
    break;

  case DEM_J1939DTC_CLEAR_PREVIOUSLY_ACTIVE:
    lEventMatch = (Dem_DTC_TestDtcPreviouslyActive(EventId) == TRUE);
    break;

  default:
    /* Clear active and previously active DTCs */
    lEventMatch = ((Dem_DTC_TestDtcActive(EventId) == TRUE) || (Dem_DTC_TestDtcPreviouslyActive(EventId) == TRUE));              /* PRQA S 3415 */ /* MD_DEM_13.5_cf */
    break;
  }
  return lEventMatch;
}
#endif
#if (DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON)
/* ****************************************************************************
 % Dem_ClearTask_ProcessClearJ1939Dtc
 *****************************************************************************/
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
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_ClearTask_ProcessClearJ1939Dtc(
  CONST(uint16, AUTOMATIC)  FirstEventId,
  CONST(uint16, AUTOMATIC)  LastEventId
  )
{
  if (FirstEventId != DEM_EVENT_INVALID) 
  {
    uint16 lEventIterator;

    for (lEventIterator = LastEventId;
         FirstEventId <= lEventIterator;
         --lEventIterator)
    { /* Event must be a J1939 related and match the ClearCode */

      /* If OBD is supported, non-J1939 DTCs shall also be removed when clear is requested */
      if ( (Dem_Cfg_IsObdSupportedInVariant() == FALSE)
        && (Dem_Core_DTC_HasJ1939DTC(lEventIterator) == FALSE) )
      { /* only match events that support J1939 */
        continue;                                                                                                                
      }
# if (DEM_CFG_ISCONST_AVAILABLEINVARIANT != STD_ON)
      if (Dem_DTC_TestDtcAvailableInVariant(lEventIterator) == FALSE)
      {
        continue;                                                                                                                
      }
# endif
      if (Dem_DTC_TestDtcAvailable(lEventIterator) == FALSE)
      { /* Event must be connected */
        continue;                                                                                                                
      }
# if (DEM_FEATURE_SUPPORT_EVENTCOMBINATION == STD_ON)
      if (lEventIterator != Dem_Cfg_GetMasterEvent(lEventIterator))
      { /* do not match dependent combined events */
        continue;                                                                                                                
      }
# endif
      if (Dem_ClearTask_TestDtcMatchesJ1939ClearFilter(lEventIterator) == FALSE)
      { /* DTC status needs to match the request */
        continue;                                                                                                                
      }
      if (Dem_ClearTask_GetClearNodeIndex() != Dem_Cfg_GetInvalidJ1939NodeIndex())
      {
        /* If OBD is supported, all events in the memory shall be cleared */
        if ( (Dem_Cfg_IsObdSupportedInVariant() == FALSE)
          && (Dem_Client_ClientAccess_IsEventAssignedToNode(lEventIterator, Dem_ClearTask_GetClearNodeIndex()) == FALSE) )
        {
          continue;
        }
      }
      Dem_ClearTask_DTCCleared(lEventIterator);
    }
  }
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
#endif /* (DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON) */

/* ****************************************************************************
 % Dem_Client_ClearTask_CalculateFirstEventToClear()
 *****************************************************************************/
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
 */
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_CalculateFirstEventToClear(
  uint16 TranslatedMemoryId,
  uint16 ClearDTCRuntimeLimit
  )
{
  uint16 lFirstEventToClear;
  Dem_EventIdType FirstEventId = Dem_Memory_GetFirstEvent(TranslatedMemoryId);

  if (Dem_Client_ClearTask_ClearDTCRuntimeLimit_IsEnabled() == TRUE)
  {
    if ((Dem_ClearTask_ClearEventIterator - FirstEventId) < ClearDTCRuntimeLimit)
    {
      lFirstEventToClear = FirstEventId;
    }
    else
    {
      lFirstEventToClear = (uint16)(Dem_ClearTask_ClearEventIterator - ClearDTCRuntimeLimit + 1u);
    }
  }
  else
  {
    lFirstEventToClear = FirstEventId;
  }

  if ((Dem_Client_ClearTask_ClearingInternalEventsRequired(TranslatedMemoryId) == FALSE) && (lFirstEventToClear == FirstEventId))
  {
    Dem_ClearTask_ClearFinished = TRUE;
  }

  return lFirstEventToClear;
}

/* ****************************************************************************
 % Dem_Client_ClearTask_CalculateLastEventToClear()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_CalculateLastEventToClear(
  uint16 TranslatedMemoryId
  )
{
  Dem_EventIdType LastEventId;

  if (Dem_Client_ClearTask_ClearDTCRuntimeLimit_IsEnabled() == TRUE)
  {
    LastEventId = Dem_ClearTask_ClearEventIterator;
  }
  else
  {
    LastEventId = Dem_Memory_GetLastEvent(TranslatedMemoryId);
  }

  return LastEventId;
}


/* ****************************************************************************
 % Dem_Client_ClearTask_CalculateFirstInternalEventToClear()
 *****************************************************************************/
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
 */
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_CalculateFirstInternalEventToClear(
  uint16 ClearDTCRuntimeLimit
  )
{
  uint16 lFirstEventToClear;
  Dem_EventIdType FirstEventId = Dem_Client_ClearTask_GetFirstInternalEvent();

  if (Dem_Client_ClearTask_ClearDTCRuntimeLimit_IsEnabled() == TRUE)
  {
    if ((Dem_ClearTask_ClearEventIterator - FirstEventId) < ClearDTCRuntimeLimit)
    {
      lFirstEventToClear = FirstEventId;
      Dem_ClearTask_ClearFinished = TRUE;
    }
    else
    {
      lFirstEventToClear = (uint16)(Dem_ClearTask_ClearEventIterator - ClearDTCRuntimeLimit + 1u);
    }
  }
  else
  {
    lFirstEventToClear = FirstEventId;
    Dem_ClearTask_ClearFinished = TRUE;
  }

  return lFirstEventToClear;
}

/* ****************************************************************************
 % Dem_Client_ClearTask_CalculateLastInternalEventToClear()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(uint16, DEM_CODE)
Dem_Client_ClearTask_CalculateLastInternalEventToClear(
  void
  )
{
  Dem_EventIdType LastEventId;

  if (Dem_Client_ClearTask_ClearDTCRuntimeLimit_IsEnabled() == TRUE)
  {
    LastEventId = Dem_ClearTask_ClearEventIterator;
  }
  else
  {
    LastEventId = Dem_Client_ClearTask_GetLastInternalEvent();
  }

  return LastEventId;
}

/* ****************************************************************************
 % Dem_Client_ClearTask_UpdateEventClearIterator()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE) 
Dem_Client_ClearTask_UpdateEventClearIterator(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  uint16 FirstClearedEventId,
  uint16 TranslatedMemoryId
)
{
  Dem_ClearTask_ClearEventIterator = FirstClearedEventId - 1u;

  if (Dem_Client_ClearTask_ClearingInternalEventsRequired(TranslatedMemoryId) == TRUE)
  {
    if (FirstClearedEventId == Dem_Memory_GetFirstEvent(TranslatedMemoryId))
    { /* All events with the memory destination are cleared and update the iterator to started clearing the internal events */
      Dem_ClearTask_ClearEventIterator = Dem_Client_ClearTask_GetLastInternalEvent();
    }
  }
}


/* ****************************************************************************
 % Dem_ClearTask_ProcessClearDtc
 *****************************************************************************/
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
 *
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ClearTask_ProcessClearDtc(
  void
  )
{
  uint16 lTranslatedMemoryId;
#if (DEM_CFG_SUPPORT_CLEAR_DTC_RUNTIME_LIMIT == STD_ON)
  uint16 lNumClearedEvents = 0;
#endif
  lTranslatedMemoryId = Dem_ClearTask_GetClearMemoryId();
  
  Dem_ClearTask_ClearFinished = FALSE;

  if (Dem_ClearTask_GetClearEvent() != DEM_EVENT_INVALID)
  { /* Clear single event */
    Dem_ClearTask_ProcessClearSingleDtc();
  }
  else
  { 
    uint16 lFirstEventToClear;
    uint16 lLastEventToClear;

#if (DEM_CFG_SUPPORT_RESTART_DCY_ON_CLEAR_DTC == STD_ON)
    if ((Dem_Cfg_IsObdSupportedInVariant() == TRUE)
      && (Dem_ClearTask_GetClearGroup() == DEM_CFG_GROUPMASK_ALLDTCS)
# if (DEM_CFG_SUPPORT_CLEAR_DTC_RUNTIME_LIMIT == STD_ON)
      && (Dem_ClearTask_ClearEventIterator == DEM_EVENT_INVALID)
# endif
       )
      {
        Dem_Esm_PreOnClear_AllDtc(lTranslatedMemoryId);
      }
#endif

    /* Clear events assigned to the memory destination */
#if (DEM_CFG_SUPPORT_CLEAR_DTC_RUNTIME_LIMIT == STD_ON)
    if (Dem_ClearTask_ClearEventIterator == DEM_EVENT_INVALID)
    {
      Dem_ClearTask_ClearEventIterator = Dem_Memory_GetLastEvent(lTranslatedMemoryId);
    }
#endif

    /* Clear of Non-Internal events are in progress */
    if (Dem_Client_ClearTask_TestClearExternalEventsInProgress(lTranslatedMemoryId) == TRUE)
    {
      lFirstEventToClear = Dem_Client_ClearTask_CalculateFirstEventToClear(lTranslatedMemoryId, Dem_Client_ClearTask_GlobalClearDtcRuntimeLimit());
      lLastEventToClear = Dem_Client_ClearTask_CalculateLastEventToClear(lTranslatedMemoryId);

#if (DEM_CFG_SUPPORT_CLEAR_DTC_RUNTIME_LIMIT == STD_ON)
      Dem_Client_ClearTask_UpdateEventClearIterator(lFirstEventToClear, lTranslatedMemoryId);
      lNumClearedEvents = lLastEventToClear - lFirstEventToClear + 1u;
#endif

#if (DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON)
      if (Dem_ClearTask_GetClearNodeIndex() != Dem_Cfg_GetInvalidJ1939NodeIndex())
      {
        /* Attention: lEventLowerLimit is numerically smaller than current iterator */
        Dem_ClearTask_ProcessClearJ1939Dtc(lFirstEventToClear, lLastEventToClear);
      }
      else
#endif
      {
        Dem_ClearTask_ProcessClearDtcGroup(lFirstEventToClear, lLastEventToClear);
      }
    }

    /* Clear internal events */
    if (Dem_Client_ClearTask_ClearingInternalEventsRequired(lTranslatedMemoryId) == TRUE)
    {
      uint16 lInternalEventClearLimit = 0x00;
# if (DEM_CFG_SUPPORT_CLEAR_DTC_RUNTIME_LIMIT == STD_ON)
      lInternalEventClearLimit = (Dem_Client_ClearTask_GlobalClearDtcRuntimeLimit() - lNumClearedEvents);
      if (lNumClearedEvents < Dem_Client_ClearTask_GlobalClearDtcRuntimeLimit())
# endif
      {
        lFirstEventToClear = Dem_Client_ClearTask_CalculateFirstInternalEventToClear(lInternalEventClearLimit);
        lLastEventToClear = Dem_Client_ClearTask_CalculateLastInternalEventToClear();

# if (DEM_CFG_SUPPORT_CLEAR_DTC_RUNTIME_LIMIT == STD_ON)
        Dem_Client_ClearTask_UpdateEventClearIterator(lFirstEventToClear, lTranslatedMemoryId);
# endif
        Dem_ClearTask_ProcessClearDtcGroup(lFirstEventToClear, lLastEventToClear);
      }
    }
  }

  if (Dem_ClearTask_ClearFinished == TRUE)
  {
#if (DEM_CFG_SUPPORT_CLEAR_DTC_RUNTIME_LIMIT== STD_ON)
    Dem_ClearTask_ClearEventIterator = DEM_EVENT_INVALID;
#endif
    if (Dem_ClearTask_GetClearState() == DEM_CLEARTASK_INPROGRESS_DTCFOUND)
    {
      /* Suitable DTC found, but none was allowed to be cleared 
         -> clear request failed.*/
      Dem_ClearTask_SetClearState(DEM_CLEARTASK_FAILED);
    }
    else
    {
      /* No suitable DTC was found or suitable DTCs have been cleared successfully 
         -> clear request successful.*/
#if (DEM_CFG_SUPPORT_J1939_CLEAR_DTC == STD_ON)
      if ( (Dem_Cfg_IsObdIIExclusivelySupportedInVariant() == TRUE) 
        || (Dem_ClearTask_GetClearNodeIndex() == Dem_Cfg_GetInvalidJ1939NodeIndex()) )
#endif
      {
        if (Dem_ClearTask_GetClearGroup() == DEM_CFG_GROUPMASK_ALLDTCS)
        {
          Dem_Esm_PostOnClear_AllDtc(lTranslatedMemoryId);
          if (Dem_Cfg_IsPermanentSupportedInVariant() == TRUE)
          {
            Dem_Memory_ConfirmedChronologyResetOverflown();
          }
        }
      }
      Dem_Esm_PostOnClear_Always(lTranslatedMemoryId);

# if (DEM_CFG_USE_NVM == STD_ON) && (DEM_CFG_CLEAR_NV_END_RESPONSE == STD_ON)
      Dem_ClearTask_SetClearState(DEM_CLEARTASK_NV_INPROGRESS);
# else
      Dem_ClearTask_SetClearState(DEM_CLEARTASK_DONE);
# endif
      Dem_Nvm_SetSingleBlockState(Dem_Nvm_GetSingleBlockId(DEM_NVM_BLOCKTYPE_STATUS),
        DEM_NVM_BLOCKSTATE_DIRTYCLEAREDIMMEDIATE);
    }
  }
}                                                                                                                                /* PRQA S 6010, 6050, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCAL, MD_MSR_STCYC, MD_MSR_STMIF */


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */


/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_ClearTask_Public
 * \{
 */


/* ****************************************************************************
 % Dem_ClearTask_Init()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ClearTask_Init(
  void
  )
{
  Dem_ClearTask_SetClearState(DEM_CLEARTASK_IDLE);
#if (DEM_CFG_SUPPORT_CLEAR_DTC_RUNTIME_LIMIT == STD_ON)
  Dem_ClearTask_ClearEventIterator = DEM_EVENT_INVALID;
#endif
  Dem_ClearTask_ClearFinished = TRUE;
}

/* ****************************************************************************
 % Dem_ClearTask_MainFunction()
 *****************************************************************************/
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ClearTask_MainFunction(
  void
  )
{
  switch (Dem_ClearTask_GetClearState())
  {
  case DEM_CLEARTASK_NEW:
    Dem_ClearTask_SetClearState(DEM_CLEARTASK_INPROGRESS);
    /* FALL THROUGH */
  case DEM_CLEARTASK_INPROGRESS:                                                                                                 /* PRQA S 2003 */ /* MD_DEM_15.2 */
  case DEM_CLEARTASK_INPROGRESS_DTCCLEARED:
  case DEM_CLEARTASK_INPROGRESS_DTCFOUND:
    Dem_ClearTask_ProcessClearDtc();
    if (Dem_ClearTask_GetClearState() == DEM_CLEARTASK_DONE)
    {
      Dem_ClearTask_SetClearState(DEM_CLEARTASK_IDLE);
    }
    break;
  
  case DEM_CLEARTASK_DONE:
  case DEM_CLEARTASK_FAILED:
  case DEM_CLEARTASK_NVERROR:
    Dem_ClearTask_SetClearState(DEM_CLEARTASK_IDLE);
    break;

  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
    /* nothing to do */
    break;
  }
}

/* ****************************************************************************
 % Dem_ClearTask_TryNewClear()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Std_ReturnType, DEM_CODE)
Dem_ClearTask_TryNewClear(
  CONST(Dem_ClearTask_ClearRequestConstPtrType, AUTOMATIC)  ClearRequest,
  CONST(Dem_DTCSelector_HandleType, AUTOMATIC)  DTCSelector
  )
{
  Std_ReturnType lReturnValue;
  Dem_ClearDTC_DataType lClearData;

  lReturnValue = E_NOT_OK;
  lClearData.DTCSelector = DTCSelector;
  lClearData.ClearDTCStatus = DEM_CLEARDTC_STATUS_IDLE;
  Dem_ClearDTC_SetClearData(ClearRequest->ClearClient, lClearData);

  if (Dem_ClearTask_GetClearState() == DEM_CLEARTASK_IDLE)
  {
/* >>>> -------------------------------- Enter Critical Section: DcmApi */
    Dem_EnterCritical_DcmApi();

    if (Dem_ClearTask_GetClearState() == DEM_CLEARTASK_IDLE)
    {
      Dem_ClearTask_ClearRequest = *ClearRequest;
      lClearData.ClearDTCStatus = DEM_CLEARDTC_STATUS_PENDING;
      Dem_DTCSelector_LockSelectDTC(DTCSelector);
      Dem_ClearDTC_SetClearData(ClearRequest->ClearClient, lClearData);
      Dem_ClearTask_SetClearState(DEM_CLEARTASK_NEW);
      lReturnValue = E_OK;
    }
    Dem_LeaveCritical_DcmApi();
/* <<<< -------------------------------- Leave Critical Section: DcmApi */
  }

  return lReturnValue;
}

# if (DEM_CFG_USE_NVM == STD_ON) && (DEM_CFG_CLEAR_NV_END_RESPONSE == STD_ON)
/* ****************************************************************************
 % Dem_ClearTask_NotifyClearCompleted()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ClearTask_NotifyClearCompleted(
  void
  )
{
  if (Dem_ClearTask_GetClearState() == DEM_CLEARTASK_NV_INPROGRESS)
  {
    if (Dem_ClearTask_TestClearWriteErrors() == TRUE)
    {
      Dem_ClearTask_SetClearState(DEM_CLEARTASK_NVERROR);
    }
    else
    {
      Dem_ClearTask_SetClearState(DEM_CLEARTASK_DONE);
    }
  }
}
#endif

# if (DEM_CFG_USE_NVM == STD_ON) && (DEM_CFG_CLEAR_NV_END_RESPONSE == STD_ON)
/* ****************************************************************************
 % Dem_ClearTask_NotifyClearWriteError()
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_ClearTask_NotifyClearWriteError(
  void
  )
{
  Dem_ClearTask_SetClearWriteErrors();
}
#endif

#if (DEM_CFG_NOTIFY_CLEAR_ALLOWED == STD_ON)
/* ****************************************************************************
 % Dem_ClearTask_ClearEventAllowed
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_ClearTask_ClearEventAllowed(
CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  boolean lClearAllowed;
  Dem_ClearEventAllowedFPtrType lpClearAllowedFunc;

  lClearAllowed = TRUE;
  lpClearAllowedFunc = Dem_Cfg_EventCbkClearAllowed(EventId);
  if (lpClearAllowedFunc != NULL_PTR)
  {
    if ((*lpClearAllowedFunc)(&lClearAllowed) != E_OK)                                                                           /* SBSW_DEM_CLEARALLOWED_FPTR */
    { /* Make sure a failed call does not influence lDoClear */
      lClearAllowed = TRUE;
    }
  }
  return lClearAllowed;
}
#endif

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_CLEARTASK_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_ClearTask_Implementation.h
 *********************************************************************************************************************/
