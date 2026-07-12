/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 *  \ingroup     Os_Kernel
 *  \defgroup    Os_Barrier Barrier
 *  \file        Os_BarrierInt.h
 *  \brief       Provides synchronization barriers for OS internal (and later on also external) use.
 *  \details
 *
 *  \{
 *
 *  \brief       This module provides the internal barrier management interface.
 *  \details     --no details--
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os.h.
 **********************************************************************************************************************/

#ifndef OS_BARRIERINT_H
# define OS_BARRIERINT_H
                                                                                                                         /* PRQA S 0388 EOF */ /* MD_MSR_Dir1.1 */
/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Barrier_Types.h"
# include "Os_Task_Types.h"

/* Os kernel module dependencies */
# include "Os_Cfg.h"
# include "Os_Common_Types.h"
# include "Os_SpinlockInt.h"
# include "Os_ErrorInt.h"

/* Os hal dependencies */
# include "Os_Hal_CompilerInt.h"




/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
typedef uint8 Os_BarrierAttendeeIdxType;
typedef uint8_least Os_BarrierAttendeeIdx_IteratorType;

typedef uint8 Os_BarrierCounterType;

typedef P2CONST(Os_BarrierBaseConfigType, AUTOMATIC, OS_VAR_NOINIT) Os_BarrierAttendeeRefType;

typedef enum
{
  /*! The attendee is detached from the barrier and not considered during synchronization. */
  OS_BARRIERATTENDEESTATE_DETACHED,

  /*! The attendee is considered during synchronization. */
  OS_BARRIERATTENDEESTATE_ATTACHED
}Os_BarrierAttendeeStateType;


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/
/*! Dynamic data to a barrier. */
struct Os_BarrierBaseType_Tag
{
  /*! Defines whether this attendee is considered during synchronization. */
  Os_BarrierAttendeeStateType AttendeeState;

  /*! Counts the number of cores which have not reached the barrier until now. */
  Os_BarrierCounterType Counter;
};


/*! Static data to a barrier. */
struct Os_BarrierBaseConfigType_Tag
{
  /*! The dynamic data of a barrier. */
  P2VAR(Os_BarrierBaseType volatile, AUTOMATIC, OS_VAR_NOINIT) Dyn;

  /*! List of references to the dynamic data of all attendees assigned to this barrier (including this attendee). */
  P2CONST(Os_BarrierAttendeeRefType, AUTOMATIC, OS_VAR_NOINIT) AttendeeRefs;

  /*! The number of attendees, assigned to the barrier including this. */
  Os_BarrierAttendeeIdxType AttendeeCount;

  /*! The reference to the participating task of this barrier */
  P2CONST(Os_TaskConfigType, AUTOMATIC, OS_VAR_NOINIT) TaskPtr;

};


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define OS_START_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */
/***********************************************************************************************************************
 *  Os_BarrierInit()
 **********************************************************************************************************************/
/*! \brief          Initializes the barrier (Init-Step2).
 *  \details        Attendee is not attached to the barrier.
 *
 *  \param[in,out]  Barrier       The barrier to be initialized. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different objects.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 *  \pre            The counter value of each other initialized attendee is '0'.
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_BarrierInit
(
  P2CONST(Os_BarrierBaseConfigType, AUTOMATIC, OS_VAR_NOINIT) Barrier
);


/***********************************************************************************************************************
 *  Os_BarrierAttach()
 **********************************************************************************************************************/
/*! \brief          Tells the barrier that the caller has to be synchronized at the next barrier.
 *  \details        --no details--
 *
 *  \param[in,out]  Barrier   The barrier to synchronize on. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different objects.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 *  \pre            Attendee is detached from the barrier.
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_BarrierAttach
(
  P2CONST(Os_BarrierBaseConfigType, AUTOMATIC, OS_VAR_NOINIT) Barrier
);


/***********************************************************************************************************************
 *  Os_BarrierAttachUserBarrier()
 **********************************************************************************************************************/
/*! \brief          Tells the user configured barrier that the caller has to be synchronized at the next barrier.
 *  \details        --no details--
 *
 *  \param[in,out]  Barrier   The barrier to synchronize on. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different objects.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 *  \pre            Os_BarrierInit() was called before.
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_BarrierAttachUserBarrier
(
  P2CONST(Os_BarrierBaseConfigType, AUTOMATIC, OS_VAR_NOINIT) Barrier
);


/***********************************************************************************************************************
 *  Os_BarrierDetach()
 **********************************************************************************************************************/
/*! \brief          Tells the barrier that the caller does not have to be synchronized or is already waiting for
 *                  becoming synchronous.
 *  \details        --no details--
 *
 *  \param[in,out]  Barrier   The barrier to skip. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE for different objects.
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 *  \pre            Attendee is attached to the barrier.
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_BarrierDetach
(
  P2CONST(Os_BarrierBaseConfigType, AUTOMATIC, OS_VAR_NOINIT) Barrier
);


/***********************************************************************************************************************
 *  Os_BarrierSynchronizeInternal()
 **********************************************************************************************************************/
/*! \brief          Enters the barrier and waits until all attendees are synchronized.
 *  \details        -- no details --
 *
 *  \param[in,out]  Barrier   The synchronization barrier. Parameter must not be NULL.
 *
 *  \context        OS_INTERNAL
 *
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *
 *  \pre            Concurrent access to given object is prevented by caller.
 **********************************************************************************************************************/
FUNC(void, OS_CODE) Os_BarrierSynchronizeInternal
(
  P2CONST(Os_BarrierBaseConfigType, AUTOMATIC, OS_VAR_NOINIT) Barrier
);


/***********************************************************************************************************************
 *  Os_Api_BarrierSynchronize()
 **********************************************************************************************************************/
/*! \brief        Synchronize tasks at the given barrier.
 *  \details      For further details see /ref Os_BarrierSynchronize().
 *
 *  \param[in]    BarrierID         See /ref Os_BarrierSynchronize().
 *
 *  \return       OS_STATUS_OK                       No error.
 *                OS_STATUS_ID_1                     (EXTENDED status:) Invalid BarrierID.
 *                OS_STATUS_CALLEVEL                 (EXTENDED status:) Called from invalid context.
 *                OS_STATUS_NO_BARRIER_PARTICIPANT   (EXTENDED status:) The given barrier is not configured for
 *                                                                      the local core.
 *                                                   (EXTENDED status:) Task is not configured to participate
 *                                                                      the barrier.
 *
 *  \context      TASK
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          Pre-Condition: None
 *
 *  \trace        CREQ-208524
 **********************************************************************************************************************/
FUNC(Os_StatusType, OS_CODE) Os_Api_BarrierSynchronize(Os_BarrierIdType BarrierID);


/***********************************************************************************************************************
 *  Os_BarrierIsEnabled()
 **********************************************************************************************************************/
/*! \brief        Returns whether user configured barriers are enabled (!0) or not (0).
 *  \details        --no details--
 *
 *  \retval       !0  Barrier enabled.
 *  \retval       0   Barrier disabled.
 *
 *  \context      OS_INTERNAL
 *
 *  \reentrant    TRUE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_FUNC_ATTRIBUTE_DECLARATION(OS_LOCAL_INLINE Os_StdReturnType, OS_CODE, OS_ALWAYS_INLINE, Os_BarrierIsEnabled, (void));


/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/


# define OS_STOP_SEC_CODE
# include "Os_MemMap_OsCode.h"                                                                                          /* PRQA S 5087 */ /* MD_MSR_MemMap */




#endif /* OS_BARRIERINT_H */

/* module specific MISRA deviations:
 */

/*!
 * \}
 */
/***********************************************************************************************************************
 *  END OF FILE: Os_BarrierInt.h
 **********************************************************************************************************************/
