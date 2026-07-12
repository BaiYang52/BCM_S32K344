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
/*! \addtogroup Dem_Completion
 *  \{
 *  \file       Dem_Completion_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_COMPLETION_IMPLEMENTATION_H)
#define DEM_COMPLETION_IMPLEMENTATION_H

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
/* PRQA S 3415 EOF */ /* MD_DEM_13.5_cf */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own unit header */
/* ------------------------------------------------------------------------- */
#include "Dem_Completion_Interface.h"

                                                    /* Used unit API */
/* ------------------------------------------------------------------------- */
#include "Dem_FaultMemory_Interface.h"
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_Support_Interface.h"
#include "Dem_Client_Interface.h"

#include "Dem_ReadinessIF_Interface.h"

                                                   /* unit callbacks*/
/* ------------------------------------------------------------------------- */

/* ********************************************************************************************************************
 *  UNIT CONSTANT MACROS
 *********************************************************************************************************************/

  /*! Filename declaration */
#define DEM_COMPLETION_IMPLEMENTATION_FILENAME "Dem_Completion_Implementation.h"

 /* ********************************************************************************************************************
 *  UNIT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT DATA
 *********************************************************************************************************************/


#define DEM_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
 /*! Pending updates of readiness groups */
DEM_LOCAL volatile VAR(uint32, DEM_VAR_NOINIT)     Dem_Completion_UpdateFlag;
#define DEM_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */



/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
 
/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Completion_PrivateProperties Private Properties
 * \{
 */

/* ****************************************************************************
 * Dem_Readiness_Completion_IsCompressionMisfireReportedAsComplete
 *****************************************************************************/
/*!
 * \brief         Tests if misfire monitoring shall always be reported as
 *                complete for compression ignition engines.
 *
 * \details       Tests if misfire monitoring shall always be reported as
 *                complete for compression ignition engines.
 *
 * \return        TRUE
 *                Misfire monitoring shall always be reported as complete
 *                for compression ignition engines.
 * \return        FALSE
 *                Misfire monitor completion is dynamically calculated for
 *                compression ignition engines.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_IsCompressionMisfireReportedAsComplete(
  void
);

/* ****************************************************************************
 * Dem_Readiness_Completion_IsFuelSystemReportedAsComplete
 *****************************************************************************/
/*!
 * \brief         Tests if fuel system monitoring shall always be reported as
 *                complete.
 *
 * \details       Tests if fuel system monitoring shall always be reported as
 *                complete.
 *
 * \return        TRUE
 *                Fuel system monitoring shall always be reported as complete
 *                complete.
 * \return        FALSE
 *                Fuel system monitor completion is dynamically calculated.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_IsFuelSystemReportedAsComplete(
  void
);

/* ****************************************************************************
 * Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Set the update flag for Readiness Completion Calculation
 *
 * \details       Set the update flag for Readiness Completion Calculation,
 *                on the Dem MainFunction.
 *
 * \param[in]     UpdateFlag
 *                Value to be set
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag(
  uint32 UpdateFlag
);

/* ****************************************************************************
 * Dem_Readiness_Completion_GetFullReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Get the global update flags for readiness completion calculation.
 *
 * \details       Get the global update flags for readiness completion calculation.
 *                The flags indicate, if an update of a readiness group
 *                completion state is necessary during the main function.
 *
 * \return        Readiness completion update flags
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_GetFullReadinessGroupUpdateFlag(
  void
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Readiness_Completion_IsCompressionMisfireReportedAsComplete
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_IsCompressionMisfireReportedAsComplete(
  void
)
{
  return DEM_CFG_PROCESS_COMPRESSION_MISFIRE_ALWAYS_COMPLETE == STD_ON;
}

/* ****************************************************************************
 % Dem_Readiness_Completion_IsFuelSystemReportedAsComplete
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_IsFuelSystemReportedAsComplete(
  void
)
{
  return DEM_CFG_PROCESS_FUELSYSTEM_ALWAYS_COMPLETE == STD_ON;
}

/* ****************************************************************************
 % Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag(
  uint32 UpdateFlag
)
{
  uint32 lOldValue;
  uint32 lNewValue;

  do
  {
    lOldValue = Dem_Completion_UpdateFlag;
    lNewValue = UpdateFlag;
  } while (Dem_Com_SyncCompareAndSwap(&Dem_Completion_UpdateFlag, lOldValue, lNewValue) == FALSE);                               /* SBSW_DEM_CALL_SYNC_COMPARE_AND_SWAP */
}

/* ****************************************************************************
 % Dem_Readiness_Completion_GetFullReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_GetFullReadinessGroupUpdateFlag(
  void
)
{
  return Dem_Completion_UpdateFlag;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_Completion_PublicProperties
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Completion_Private Private Methods
 * \{
 */

/* ****************************************************************************
 * Dem_Readiness_Completion_ResetFullReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Reset the update flags for readiness completion calculation.
 *
 * \details       Reset the update flags for readiness completion calculation,
 *                and return the previous values.
 *
 * \return        Update flag values before the reset
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_ResetFullReadinessGroupUpdateFlag(
  void
  );

/* ****************************************************************************
 * Dem_Readiness_Completion_SetReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Marks the Readiness Completion of a given readiness group to
 *                be updated.
 *
 * \details       Marks the Readiness Completion of a given readiness group to
 *                be updated.
 *
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetReadinessGroupUpdateFlag(
  uint8 ReadinessGroup
);

/* ****************************************************************************
 * Dem_Readiness_Completion_GetReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Tests if Readiness Completion of the given readiness group has
 *                to be updated
 *
 * \details       Tests if Readiness Completion of the given readiness group has
 *                to be updated
 *
 * \param[in]     State
 *                Current readiness state as 32 bit bitmap
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \return        TRUE
 *                Readiness Completion of given readiness group has to be updated
 * \return        FALSE
 *                Readiness Completion of given readiness group doesn't have to
 *                be updated
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_GetReadinessGroupUpdateFlag(
  uint32 State,
  uint8 ReadinessGroup
);

/* ****************************************************************************
 * Dem_Readiness_Completion_SetReadinessGroupCompletedState
 *****************************************************************************/
/*!
 * \brief         Sets the stored completion value of the given readiness 
 *                group to the given value.
 *
 * \details       Sets the stored completion value of the given readiness 
 *                group to the given value.
 *
 * \param[in]     Completed
 *                TRUE    Readiness group is set to completed
 *                FALSE   Readiness group is set to not completed
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetReadinessGroupCompletedState(
  boolean Completed,
  uint8 ReadinessGroup);

/* ****************************************************************************
 * Dem_Readiness_Completion_GetCompletionMaskAndPositionForPIDs
 *****************************************************************************/
/*!
 * \brief         Provides Byte Mask and Byte Position of completion state for
 *                a readiness group in PID01 or PID41 Data Block.
 *
 * \details       Provides Byte Mask and Byte Position of completion state for
 *                a readiness group in PID01 or PID41 Data Block.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[out]    ByteMask
 *                Byte Mask of completion state for the corresponding PID01 or
 *                PID41 Data Block
 * \param[out]    BytePos
 *                Byte Position of completion state of the corresponding PID01
 *                or PID41 Data Block
 *
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_GetCompletionMaskAndPositionForPIDs(
  uint8  ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
);

/* ****************************************************************************
 * Dem_Readiness_Completion_GetCompletionMaskAndPositionForDM05
 *****************************************************************************/
/*!
 * \brief         Provides Byte Mask and Byte Position of status bits for
 *                a Readiness Group in DM 05 Data Block.
 *
 * \details       Provides Byte Mask and Byte Position of status bits for
 *                a Readiness Group in DM 05 Data Block.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[out]    ByteMask
 *                Byte Mask for the corresponding DM 05 Data Block
 * \param[out]    BytePos
 *                Byte Position of the corresponding DM 05 Data Block
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Completion_GetCompletionMaskAndPositionForDM05(
  uint8  ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
);

/* ****************************************************************************
 * Dem_Readiness_Completion_TestStaticallyCalculatedCompletionStates
 *****************************************************************************/
/*!
 * \brief         Function returns if the completeness for readiness group is
 *                statically calculated
 *
 * \details       Function returns if the completeness for readiness group is
 *                statically calculated
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, PID41, DID F501, DM05.
 *
 * \return        TRUE
 *                Completeness for readiness group is statically calculated
 * \return        FALSE
 *                Completeness for readiness group is dynamically calculated
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_TestStaticallyCalculatedCompletionStates(
  uint8  ReadinessGroup,
  uint8  DataContext
);

/* ****************************************************************************
 * Dem_Readiness_Completion_CheckEventPreventsCompletion
 *****************************************************************************/
/*!
 * \brief         Check if event prevents its assigned readiness group from 
 *                being completed.
 *
 * \details       If the event prevents its assigned readiness group from
 *                being completed, the flag for updating the readiness completion
 *                is reset for this readiness group.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 * \param[in]     OldReadinessUpdateFlag
 *                Old update flag values of all readiness groups
 *
 * \return        The new update flag values of all readiness groups
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_CheckEventPreventsCompletion(
  Dem_EventIdType EventId,
  uint32 OldReadinessUpdateFlag
);

/* ****************************************************************************
 * Dem_Readiness_Completion_CheckReadinessRecalculationNeeded
 *****************************************************************************/
/*!
 * \brief         Check if recalcuation of event's readiness group is needed.
 *
 * \details       Check if recalcuation of event's readiness group is needed
 *                and reset update flag for this readiness group, if no 
 *                recalculation is required.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 * \param[in,out] ReadinessUpdateFlag
 *                Update flag values of all readiness groups
 *
 * \return        TRUE
 *                Recalcuation of event's readiness group is needed.
 * \return        FALSE
 *                No recalcuation of event's readiness group is needed.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_CheckReadinessRecalculationNeeded(
  Dem_EventIdType EventId,
  CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) ReadinessUpdateFlag
);

/* ****************************************************************************
 * Dem_Readiness_Completion_SetAllMarkedReadinessGroupsCompleted
 *****************************************************************************/
/*!
 * \brief         Set all marked readiness groups to completed.
 *
 * \details       Set all readiness groups that are marked to be recalculated
 *                to completed.
 *
 * \param[in]     ReadinessUpdateFlag
 *                Update flag values of all readiness groups
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetAllMarkedReadinessGroupsCompleted(
  uint32 ReadinessUpdateFlag
);

/* ****************************************************************************
 * Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * \brief         Removes the mark for updating the readiness completion of the
 *                given readiness group.
 *
 * \details       Removes the mark for updating the readiness completion of the
 *                given readiness group.
 *
 * \param[in]     State
 *                Current readiness state as 32 bit bitmap
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \return        New readiness state
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag(
  uint32 State,
  uint8 ReadinessGroup
);

/* ****************************************************************************
 * Dem_Readiness_Completion_TestReadinessGroupCompletedState
 *****************************************************************************/
/*!
 * \brief         Checks if readiness group is completed.
 *
 * \details       Checks the stored completion value of the given readiness
 *                group.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 *
 * \return        TRUE
 *                Readiness group is completed
 * \return        FALSE
 *                Readiness group is not completed
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_TestReadinessGroupCompletedState(
  uint8 ReadinessGroup
);

/* ****************************************************************************
 * Dem_Readiness_Completion_SetCalculatedCompletionStates
 *****************************************************************************/
/*!
 * \brief         Set calculated completion states for all readiness groups
 *                and respective data context.
 *
 * \details       Set calculated completion states for all readiness groups
 *                and respective data context.
 *
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked
 *                Currently considered: PID01, DID F501, DM05.
 *
 * \param[out]    CompletionStates
 *                Destination buffer with completion bits set for supported
 *                readiness groups and respective data context.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetCalculatedCompletionStates(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Completion_TestReadinessGroupCompletedStateInMask
 *****************************************************************************/
/*!
 * \brief         Checks if readiness group is completed in the given completion
 *                mask buffer
 *
 * \details       Checks if readiness group is completed in the given completion
 *                mask buffer
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[in]     CompletionMaskBuffer
 *                Buffer containing the completion mask for all readiness groups
 *
 * \return        TRUE
 *                Readiness group is completed
 * \return        FALSE
 *                Readiness group is not completed
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_TestReadinessGroupCompletedStateInMask(
  uint8  ReadinessGroup,
  CONST(Dem_Data_ConstDestinationBufferPtrType, AUTOMATIC)  CompletionMaskBuffer
);

/* ********************************************************************************************************************
 *  UNIT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Readiness_Completion_ResetFullReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_ResetFullReadinessGroupUpdateFlag(
  void
)
{
  uint32 lOldValue;
  uint32 lNewValue;

  do
  {
    lOldValue = Dem_Completion_UpdateFlag;
    lNewValue = 0u;
  } while (Dem_Com_SyncCompareAndSwap(&Dem_Completion_UpdateFlag, lOldValue, lNewValue) == FALSE);                               /* SBSW_DEM_CALL_SYNC_COMPARE_AND_SWAP */

  /* Old value always contains the value before it was cleared */
  return lOldValue;
}

/* ****************************************************************************
 % Dem_Readiness_Completion_SetReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetReadinessGroupUpdateFlag(
  uint8 ReadinessGroup
)
{
  if (ReadinessGroup != DEM_CFG_READINESS_NONE)
  {
    Dem_Com_SyncSetBitInBitfield(&Dem_Completion_UpdateFlag, DEM_CFG_READINESS_GROUP_WORDSIZE, ReadinessGroup);                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  }
}

/* ****************************************************************************
 % Dem_Readiness_Completion_GetReadinessGroupUpdateFlag
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
Dem_Readiness_Completion_GetReadinessGroupUpdateFlag(
  uint32 State,
  uint8 ReadinessGroup
)
{
  boolean lReturnValue;
  lReturnValue = FALSE;
  if (ReadinessGroup != DEM_CFG_READINESS_NONE)
  {
    if ((State & (uint32)((uint32)1u << (ReadinessGroup))) != 0u)
    {
      lReturnValue = TRUE;
    }
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Readiness_Completion_SetReadinessGroupCompletedState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetReadinessGroupCompletedState(
  boolean Completed,
  uint8 ReadinessGroup
)
{
  uint8 lReadinessGroup;
  lReadinessGroup = ReadinessGroup;

  if (lReadinessGroup != DEM_CFG_READINESS_NONE)
  {
#if (DEM_FEATURE_NEED_READINESS_CALCULATION == STD_ON)
    if (Completed == TRUE)
    {
      Dem_Com_SetBitInBitfield(&Dem_Cfg_StatusData.ReadinessGroupCompletedStates, DEM_CFG_READINESS_GROUP_WORDSIZE, ReadinessGroup);  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      Dem_Com_ResetBitInBitfield(&Dem_Cfg_StatusData.ReadinessGroupCompletedStates, DEM_CFG_READINESS_GROUP_WORDSIZE, ReadinessGroup); /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
#endif
  }
  DEM_IGNORE_UNUSED_ARGUMENT(Completed)                                                                                          /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
}

/* ****************************************************************************
 % Dem_Readiness_Completion_GetCompletionMaskAndPositionForPIDs
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_GetCompletionMaskAndPositionForPIDs(
  uint8  ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
)
{
  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    *ByteMask = DEM_READINESS_PID_COMPLETION_MISF_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_MISF_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_FLSYS:
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    *ByteMask = DEM_READINESS_PID_COMPLETION_FLSYS_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_FLSYS_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    *ByteMask = DEM_READINESS_PID_COMPLETION_CMPRCMPT_MASK;                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_CMPRCMPT_BYTE;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CAT:
    *ByteMask = DEM_READINESS_PID_COMPLETION_CAT_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_CAT_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HCCAT:
    *ByteMask = DEM_READINESS_PID_COMPLETION_HCCAT_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_HCCAT_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HTCAT:
    *ByteMask = DEM_READINESS_PID_COMPLETION_HTCAT_MASK;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_HTCAT_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_NOXCAT:
    *ByteMask = DEM_READINESS_PID_COMPLETION_NOXCAT_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_NOXCAT_BYTE;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EVAP:
    *ByteMask = DEM_READINESS_PID_COMPLETION_EVAP_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_EVAP_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_SECAIR:
    *ByteMask = DEM_READINESS_PID_COMPLETION_SECAIR_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_SECAIR_BYTE;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    *ByteMask = DEM_READINESS_PID_COMPLETION_BOOSTPR_MASK;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_BOOSTPR_BYTE;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_O2SENS:
    *ByteMask = DEM_READINESS_PID_COMPLETION_O2SENS_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_O2SENS_BYTE;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGSENS:
    if (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE)
    {
      *ByteMask = DEM_READINESS_PID_COMPLETION_EGSENS_MASK;                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *BytePos = DEM_READINESS_PID_COMPLETION_EGSENS_BYTE;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      *ByteMask = DEM_READINESS_PID_COMPLETION_NONE_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *ByteMask = DEM_READINESS_PID_COMPLETION_NONE_BYTE;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    break;
  case DEM_CFG_READINESS_O2SENSHT:
    *ByteMask = DEM_READINESS_PID_COMPLETION_O2SENSHT_MASK;                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_O2SENSHT_BYTE;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_PMFLT:
    if (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE)
    {
      *ByteMask = DEM_READINESS_PID_COMPLETION_PMFLT_MASK;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *BytePos = DEM_READINESS_PID_COMPLETION_PMFLT_BYTE;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    else
    {
      *ByteMask = DEM_READINESS_PID_COMPLETION_NONE_MASK;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
      *ByteMask = DEM_READINESS_PID_COMPLETION_NONE_BYTE;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    }
    break;
  case DEM_CFG_READINESS_EGR:
    *ByteMask = DEM_READINESS_PID_COMPLETION_EGR_MASK;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_EGR_BYTE;                                                                            /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  default:
    *ByteMask = DEM_READINESS_PID_COMPLETION_NONE_MASK;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_PID_COMPLETION_NONE_BYTE;                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  }
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */

/* ****************************************************************************
 % Dem_Readiness_Completion_GetCompletionMaskAndPositionForDM05
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Completion_GetCompletionMaskAndPositionForDM05(
  uint8  ReadinessGroup,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  ByteMask,
  CONSTP2VAR(uint8, AUTOMATIC, AUTOMATIC)  BytePos
)
{
  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_MISF:
    *ByteMask = DEM_READINESS_DM05_COMPLETENESS_MISF_MASK;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_MISF_BYTE;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_FLSYS:
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    *ByteMask = DEM_READINESS_DM05_COMPLETENESS_FLSYS_MASK;                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_FLSYS_BYTE;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    *ByteMask = DEM_READINESS_DM05_COMPLETENESS_CMPRCMPT_MASK;                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_CMPRCMPT_BYTE;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CAT:
    *ByteMask = DEM_READINESS_DM05_CAT_MASK;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_COMPLETENESS_CAT_BYTE;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HTCAT:
    *ByteMask = DEM_READINESS_DM05_HTCAT_MASK;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_COMPLETENESS_HTCAT_BYTE;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EVAP:
    *ByteMask = DEM_READINESS_DM05_EVAP_MASK;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_COMPLETENESS_EVAP_BYTE;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_SECAIR:
    *ByteMask = DEM_READINESS_DM05_SECAIR_MASK;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_COMPLETENESS_SECAIR_BYTE;                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_AC:
    *ByteMask = DEM_READINESS_DM05_AC_MASK;                                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_COMPLETENESS_AC_BYTE;                                                                          /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGSENS:
    *ByteMask = DEM_READINESS_DM05_EGSENS_MASK;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_COMPLETENESS_EGSENS_BYTE;                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGHSENS:
    *ByteMask = DEM_READINESS_DM05_EGHSENS_MASK;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_COMPLETENESS_EGHSENS_BYTE;                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_EGR:
  case DEM_CFG_READINESS_VVT:
    *ByteMask = DEM_READINESS_DM05_EGRVVT_MASK;                                                                                  /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_COMPLETENESS_EGRVVT_BYTE;                                                                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_CSAS:
    *ByteMask = DEM_READINESS_DM05_CSAS_MASK;                                                                                    /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_COMPLETENESS_CSAS_BYTE;                                                                        /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_BOOSTPR:
    *ByteMask = DEM_READINESS_DM05_BOOSTPR_MASK;                                                                                 /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_COMPLETENESS_BOOSTPR_BYTE;                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_PMFLT:
    *ByteMask = DEM_READINESS_DM05_PMFLT_MASK;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_COMPLETENESS_PMFLT_BYTE;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_NOXCAT:
  case DEM_CFG_READINESS_NOXADSORB:
    *ByteMask = DEM_READINESS_DM05_NOX_MASK;                                                                                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_COMPLETENESS_NOX_BYTE;                                                                         /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  case DEM_CFG_READINESS_HCCAT:
    *ByteMask = DEM_READINESS_DM05_HCCAT_MASK;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = DEM_READINESS_DM05_COMPLETENESS_HCCAT_BYTE;                                                                       /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  default:
    /* invalid readiness group */
    *ByteMask = 0u;                                                                                                              /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    *BytePos = 0u;                                                                                                               /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
    break;
  }
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */

/* ****************************************************************************
 % Dem_Readiness_Completion_TestStaticallyCalculatedCompletionStates
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
DEM_LOCAL FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_TestStaticallyCalculatedCompletionStates(
  uint8  ReadinessGroup,
  uint8  DataContext
)
{
  boolean lReturnValue;
  boolean lSparkEngineSupported;

  lSparkEngineSupported = (boolean)(DEM_CFG_SUPPORT_IGNITION_SPARK == STD_ON);

  switch (ReadinessGroup)
  {
  case DEM_CFG_READINESS_FLSYS:
    lReturnValue = TRUE;
    break;
  case DEM_CFG_READINESS_CMPRCMPT:
    if ((Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInDidF501() == TRUE)
      && (DataContext == DEM_READINESS_DATACONTEXT_DIDF501))
    {
      lReturnValue = TRUE;
    }
    else if ((Dem_GlobalDiagnostics_Readiness_IsComprehensiveComponentsReportedAsCompleteInPID41() == TRUE)
      && (DataContext == DEM_READINESS_DATACONTEXT_PID41))
    {
      lReturnValue = TRUE;
    }
    else
    {
      lReturnValue = FALSE;
    }
    break;
  case DEM_CFG_READINESS_MISF:
    if (lSparkEngineSupported == TRUE)
    {
      lReturnValue = TRUE;
    }
    else
    {
      if (Dem_Readiness_Completion_IsCompressionMisfireReportedAsComplete() == TRUE)
      {
        lReturnValue = TRUE;
      }
      else
      {
        lReturnValue = FALSE;
      }
    }
    break;
  case DEM_CFG_READINESS_FLSYS_NONCONT:
    if (Dem_Readiness_Completion_IsFuelSystemReportedAsComplete() == TRUE)
    {
      lReturnValue = TRUE;
    }
    else
    {
      lReturnValue = FALSE;
    }
    break;
  default:
    lReturnValue = FALSE;
    break;
  }

  return lReturnValue;
}


/* ****************************************************************************
 % Dem_Readiness_Completion_CheckEventPreventsCompletion
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
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_CheckEventPreventsCompletion(
  Dem_EventIdType EventId,
  uint32 OldReadinessUpdateFlag
)
{
  uint32 lNewReadinessUpdateFlag;

  lNewReadinessUpdateFlag = OldReadinessUpdateFlag;
  /* If event is available */
  if (Dem_Event_TestEventUnavailable(EventId) == FALSE)
  {
    Dem_UDSStatus_Type lEventStatus;
    Dem_DTC_ExtendedStatusType lExtStatus;
    lEventStatus = Dem_Event_GetUDSStatus(EventId);
    lExtStatus = Dem_Event_GetExtendedStatus(EventId);

    /* If event is pending or event was not tested passed since last clear */
    if ( (Dem_Event_ExtendedStatus_GetTPSLC(lExtStatus) == FALSE)
      || (Dem_UDSStatus_Test_PDTC(lEventStatus) == TRUE) )
    {
      /* Reset flag of this readiness group  */
      lNewReadinessUpdateFlag = Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag(lNewReadinessUpdateFlag, Dem_Core_Event_GetEventReadinessGroup(EventId));
    }
  }
  return lNewReadinessUpdateFlag;
}

/* ****************************************************************************
 % Dem_Readiness_Completion_CheckReadinessRecalculationNeeded
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_CheckReadinessRecalculationNeeded(
  Dem_EventIdType EventId,
  CONSTP2VAR(uint32, AUTOMATIC, AUTOMATIC) ReadinessUpdateFlag
)
{
  uint8 lReadinessGroup;
  uint32 lReadinessUpdateFlag;
  boolean lRecalculationNeeded;

  lReadinessGroup = Dem_Core_Event_GetEventReadinessGroup(EventId);
  lReadinessUpdateFlag = *ReadinessUpdateFlag;

  /* If update flag for event readiness group is not set no recalcuation of readiness group is needed */
  if (Dem_Readiness_Completion_GetReadinessGroupUpdateFlag(lReadinessUpdateFlag, lReadinessGroup) == FALSE)
  {
    lRecalculationNeeded = FALSE;
  }
  /* data context 'DID F501' is representant for data contexts PID01, DM05 and DID F501 as from these data contexts
     there is only a special handling for DID F501 in called function */
  else if (Dem_Readiness_Completion_TestStaticallyCalculatedCompletionStates(lReadinessGroup, DEM_READINESS_DATACONTEXT_DIDF501) == TRUE)
  {
    /* Reset flag for this group, no recalcuation of readiness group is needed */
    lReadinessUpdateFlag = Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag(lReadinessUpdateFlag, lReadinessGroup);
    lRecalculationNeeded = FALSE;
  }
  /* If event readiness group is already completed  */
  else if (Dem_Readiness_Completion_TestReadinessGroupCompletedState(lReadinessGroup) == TRUE)
  {
    /* Reset flag for this group, no recalcuation of readiness group is needed */
    lReadinessUpdateFlag = Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag(lReadinessUpdateFlag, lReadinessGroup);
    lRecalculationNeeded = FALSE;
  }
  else
  {
    lRecalculationNeeded = TRUE;
  }

  *ReadinessUpdateFlag = lReadinessUpdateFlag;                                                                                   /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  return lRecalculationNeeded;
}

/* ****************************************************************************
 % Dem_Readiness_Completion_SetAllMarkedReadinessGroupsCompleted
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetAllMarkedReadinessGroupsCompleted(
  uint32 ReadinessUpdateFlag
)
{
  uint32 lReadinessUpdateFlag;
  uint8 lReadinessGroupIter;

  lReadinessUpdateFlag = ReadinessUpdateFlag;

  /* Start with first OBD readiness group */
  lReadinessGroupIter = 1U;

  /* For all flags that are still set change the related readiness group state to completed.
  Second loop exit criteria required to break out of loop in case statically calculated
  readiness groups with no referenced events are indicated in Dem_Completion_UpdateFlag */
  while ( (lReadinessUpdateFlag != 0u) && (lReadinessGroupIter < DEM_CFG_READINESS_GROUP_SIZE) )
  {
    if (Dem_Readiness_Completion_GetReadinessGroupUpdateFlag(lReadinessUpdateFlag, lReadinessGroupIter) == TRUE)
    {
      Dem_Readiness_Completion_SetReadinessGroupCompletedState(TRUE, lReadinessGroupIter);                                       /* SBSW_DEM_POINTER_FORWARD_GLOBAL */

      /* Reset flag of this readiness group */
      lReadinessUpdateFlag = Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag(lReadinessUpdateFlag, lReadinessGroupIter);
    }

    lReadinessGroupIter++;
  }
}

/* ****************************************************************************
 % Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint32, DEM_CODE)
Dem_Readiness_Completion_ResetReadinessGroupUpdateFlag(
  uint32 State,
  uint8 ReadinessGroup
)
{
  uint32 lReturnValue;
  lReturnValue = State;
  if (ReadinessGroup != DEM_CFG_READINESS_NONE)
  {
    lReturnValue &= ~((uint32)((uint32)1U << (uint32)ReadinessGroup));
  }
  return lReturnValue;
}

/* ****************************************************************************
 % Dem_Readiness_Completion_TestReadinessGroupCompletedState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_TestReadinessGroupCompletedState(
  uint8 ReadinessGroup
)
{
  boolean lReturnValue;
#if (DEM_FEATURE_NEED_READINESS_CALCULATION == STD_ON)
  lReturnValue = Dem_Com_TestBitInBitfield(&Dem_Cfg_StatusData.ReadinessGroupCompletedStates, DEM_CFG_READINESS_GROUP_WORDSIZE, ReadinessGroup); /* SBSW_DEM_POINTER_FORWARD_STACK */
#else
  DEM_IGNORE_UNUSED_ARGUMENT(ReadinessGroup)                                                                                     /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  lReturnValue = FALSE;
#endif
  return lReturnValue;
}                                                                                                                                /* PRQA S 6030 */ /* MD_MSR_STCYC */

/* ****************************************************************************
 % Dem_Readiness_Completion_SetCalculatedCompletionStates
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetCalculatedCompletionStates(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  uint8 lReadinessGroup;
  for (lReadinessGroup = 1u; lReadinessGroup < DEM_CFG_READINESS_GROUP_SIZE; lReadinessGroup++)
  {
    if (Dem_Readiness_Completion_TestReadinessGroupCompletedState(lReadinessGroup) == TRUE)
    {
      Dem_Readiness_Completion_SetReadinessGroupCompleted(lReadinessGroup,                                                       /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
        TRUE, DataContext, DestinationBuffer);
    }
  }
}

/* ****************************************************************************
 % Dem_Readiness_Completion_TestReadinessGroupCompletedStateInMask
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Readiness_Completion_TestReadinessGroupCompletedStateInMask(
  uint8  ReadinessGroup,
  CONST(Dem_Data_ConstDestinationBufferPtrType, AUTOMATIC)  CompletionMaskBuffer
)
{
  boolean lReturnValue;
  uint8 lByteMask;
  uint8 lBytePos;

  lByteMask = 0u;
  lBytePos = 0u;

  Dem_Readiness_Completion_GetCompletionMaskAndPositionForPIDs(ReadinessGroup, &lByteMask, &lBytePos);                           /* SBSW_DEM_POINTER_FORWARD_STACK */

  if ((CompletionMaskBuffer->Buffer[lBytePos] & lByteMask) == 0u)
  {
    lReturnValue = TRUE;
  }
  else
  {
    lReturnValue = FALSE;
  }

  return lReturnValue;
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \addtogroup Dem_Completion_Public
 * \{
 */

/* ****************************************************************************
 %  Dem_Readiness_Completion_ProcessUpdateFlag
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_ProcessUpdateFlag(
  void
)
{
  if ( (Dem_Cfg_IsObdSupportedInVariant() == TRUE) && (Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported() == TRUE) )
  {
    uint32 lReadinessUpdateFlag;
    
    /* Get and reset all update flags for every readiness group to recalculate monitor completion state*/
    lReadinessUpdateFlag = Dem_Readiness_Completion_ResetFullReadinessGroupUpdateFlag();
    
    /* If a flag of a readiness group is set  */
    if (lReadinessUpdateFlag != 0u)
    {
      Dem_EventIdType lEventId;

      /* Iterate over all primary events */
      for (lEventId = Dem_MemState_MemoryGetFirstEvent(DEM_CFG_MEMORYID_PRIMARY);
           lEventId <= Dem_MemState_MemoryGetLastEvent(DEM_CFG_MEMORYID_PRIMARY);
           lEventId++)
      {
        /* If all flags are reset stop iterating events */
        if (lReadinessUpdateFlag == 0u)
        {
          break;
        }

        if (Dem_Readiness_Completion_CheckReadinessRecalculationNeeded(lEventId, &lReadinessUpdateFlag) == TRUE)                 /* SBSW_DEM_POINTER_FORWARD_STACK */
        {
          lReadinessUpdateFlag = Dem_Readiness_Completion_CheckEventPreventsCompletion(lEventId, lReadinessUpdateFlag);
        }
      }
      
      Dem_Readiness_Completion_SetAllMarkedReadinessGroupsCompleted(lReadinessUpdateFlag);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Readiness_Completion_SetStaticCompletionStates
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetStaticCompletionStates(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer                                                         /* PRQA S 3673 */ /* MD_DEM_8.13_fp */
)
{
#if (DEM_CFG_SUPPORT_MAJOR_MONITORS == STD_ON)
  uint8 lStaticallyCompletedGroups;
  switch (DataContext)
  {
  case DEM_READINESS_DATACONTEXT_PID01:
    lStaticallyCompletedGroups = DestinationBuffer->Buffer[DEM_READINESS_PID01_BYTEB] & (uint8)Dem_Cfg_GetGlobalPID01AlwaysCompletedMaskByteB();

    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID01_BYTEB, lStaticallyCompletedGroups);            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;
  case DEM_READINESS_DATACONTEXT_DIDF501:
    lStaticallyCompletedGroups = DestinationBuffer->Buffer[DEM_READINESS_DIDF501_BYTEB] & (uint8)Dem_Cfg_GetGlobalPIDF501AlwaysCompletedMaskByteB();

    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_DIDF501_BYTEB, lStaticallyCompletedGroups);          /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;
  case DEM_READINESS_DATACONTEXT_DM05:
    lStaticallyCompletedGroups = DestinationBuffer->Buffer[DEM_READINESS_DM05_BYTE4] & (uint8)Dem_Cfg_GetGlobalDM05AlwaysCompletedMaskByte4();

    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_DM05_BYTE4, lStaticallyCompletedGroups);             /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;
  case DEM_READINESS_DATACONTEXT_PID41:
    /* Set reserved completed bits to 0 */
    if (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE)
    {
      lStaticallyCompletedGroups = (DestinationBuffer->Buffer[DEM_READINESS_PID41_BYTED] & DEM_READINESS_PID41_COMPLETION_COMPRESSION_BYTED);
    }
    else
    {
      lStaticallyCompletedGroups = (DestinationBuffer->Buffer[DEM_READINESS_PID41_BYTED] & DEM_READINESS_PID41_COMPLETION_SPARK_BYTED);
    }
    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID41_BYTED, lStaticallyCompletedGroups);            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

    lStaticallyCompletedGroups = (DestinationBuffer->Buffer[DEM_READINESS_PID41_BYTEB]
      & (uint8)Dem_Cfg_GetGlobalPID41AlwaysCompletedMaskByteB());

    Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, DEM_READINESS_PID41_BYTEB, lStaticallyCompletedGroups);            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
}
#else
  DEM_IGNORE_UNUSED_ARGUMENT(DataContext)                                                                                        /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(DestinationBuffer)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}

/* ****************************************************************************
 % Dem_Readiness_Completion_SetCompletionStates
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetCompletionStates(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  Dem_Readiness_Completion_SetCalculatedCompletionStates(DataContext, DestinationBuffer);                                        /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */

  Dem_Readiness_Completion_SetStaticCompletionStates(DataContext, DestinationBuffer);                                            /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
}

/* ****************************************************************************
 % Dem_Readiness_Completion_ResetReadinessGroupCompletedStates
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_ResetReadinessGroupCompletedStates(
  void
)
{
#if (DEM_FEATURE_NEED_READINESS_CALCULATION == STD_ON)
  Dem_Cfg_StatusData.ReadinessGroupCompletedStates = 0u;
#endif
}

/* ****************************************************************************
 % Dem_Readiness_Completion_ConsiderEventStatusForCompletionStates
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
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_ConsiderEventStatusForCompletionStates(
  Dem_EventIdType EventId,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  CompletionMaskBuffer,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  uint8 lReadinessGroup;
  lReadinessGroup = Dem_Core_Event_GetEventReadinessGroup(EventId);

  if ( (lReadinessGroup != DEM_CFG_READINESS_NONE)
    && (Dem_Event_TestEventUnavailable(EventId) == FALSE) )
  {
    if (Dem_Readiness_Completion_TestStaticallyCalculatedCompletionStates(lReadinessGroup, DEM_READINESS_DATACONTEXT_PID41) == FALSE)
    {
      if (Dem_Readiness_Completion_TestReadinessGroupCompletedStateInMask(lReadinessGroup, CompletionMaskBuffer) == FALSE)       /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
      {
        Dem_UDSStatus_Type lEventStatus;
        lEventStatus = Dem_Event_GetUDSStatus(EventId);

        if (Dem_UDSStatus_Test_TFTOC(lEventStatus) == TRUE)
        {
          Dem_Readiness_Completion_SetReadinessGroupCompleted(lReadinessGroup,                                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
            TRUE, DEM_READINESS_DATACONTEXT_PID41, CompletionMaskBuffer);
        }
        else if (Dem_UDSStatus_Test_TNCTOC(lEventStatus) == TRUE)
        {
          Dem_Readiness_Completion_SetReadinessGroupCompleted(lReadinessGroup,                                                   /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
            FALSE, DEM_READINESS_DATACONTEXT_PID41, DestinationBuffer);
        }
        else
        {
          /* do nothing */
        }
      }
    }
  }
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Readiness_Completion_SetReadinessGroupCompleted
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetReadinessGroupCompleted(
  uint8 ReadinessGroup,
  boolean Completed,
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
)
{
  uint8 lByteMask;
  uint8 lBytePos;

  lByteMask = 0u;
  lBytePos = 0u;
  switch (DataContext)
  {
  case DEM_READINESS_DATACONTEXT_PID01:
  case DEM_READINESS_DATACONTEXT_PID41:
    Dem_Readiness_Completion_GetCompletionMaskAndPositionForPIDs(ReadinessGroup, &lByteMask, &lBytePos);                         /* SBSW_DEM_POINTER_FORWARD_STACK */
    break;
  case DEM_READINESS_DATACONTEXT_DIDF501:
  {
    Dem_Readiness_Support_GetSupportMaskAndPositionForDIDF501(ReadinessGroup, &lByteMask, &lBytePos);                            /* SBSW_DEM_POINTER_FORWARD_STACK */

    /* Not Completed Bits of a Readiness Group in F501 Format are always 4 bits to the left, of the supported bit*/
    lByteMask = (uint8)((uint8)lByteMask << 4u);
  }
  break;
  case DEM_READINESS_DATACONTEXT_DM05:
    Dem_Readiness_Completion_GetCompletionMaskAndPositionForDM05(ReadinessGroup, &lByteMask, &lBytePos);                         /* SBSW_DEM_POINTER_FORWARD_STACK */
    break;
  default:                                                                                                                       /* PRQA S 2016 */ /* MD_MSR_EmptyClause */ /* COV_DEM_MISRA_BRANCH */
    break;
  }
  if (lByteMask != 0u)
  {
    if (Completed == TRUE)
    {
      Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, lBytePos, DestinationBuffer->Buffer[lBytePos] & ((uint8)~lByteMask)); /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
    else
    {
      Dem_Data_WriteDestinationBufferAtIndex(DestinationBuffer, lBytePos, DestinationBuffer->Buffer[lBytePos] | lByteMask);      /* SBSW_DEM_POINTER_FORWARD_ARGUMENT */
    }
  }
}

/* ****************************************************************************
 % Dem_Readiness_Completion_UpdateCompletionStatesAfterClear
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_UpdateCompletionStatesAfterClear(
  void
)
{
  if (Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported() == TRUE)
  {
    /* all readiness groups are set to not complete */
    Dem_Readiness_Completion_ResetReadinessGroupCompletedStates();

    /* set flag for all readiness group supported by engine type to recalculate monitor state -> not supported groups will be
       set to N/A = completed*/
       /* although event states are reset on clear recalculation is necessary as availability of events (independent from clear
          request) has to be considered for completion states */
    if (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE)
    {
      Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag(DEM_READINESS_INITIALIZE_UPDATE_COMPRESSION);
    }
    else
    {
      Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag(DEM_READINESS_INITIALIZE_UPDATE_SPARK);
    }
  }
}

/* ****************************************************************************
 % Dem_Readiness_Completion_UpdateCompletionStatus
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_UpdateCompletionStatus(
  uint8  ReadinessGroup,
  boolean Complete
)
{
  if ( (Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported() == TRUE)
    && (Dem_Readiness_Completion_TestStaticallyCalculatedCompletionStates(ReadinessGroup, DEM_READINESS_DATACONTEXT_DIDF501) == FALSE))
  {
    Dem_Readiness_Completion_SetReadinessGroupCompletedState(Complete, ReadinessGroup);
  }
}

/* ****************************************************************************
 % Dem_Readiness_Completion_TriggerCompletionRecalculation
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_TriggerCompletionRecalculation(
  uint8  ReadinessGroup
)
{
  if (Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported() == TRUE)
  {
    if (Dem_Readiness_Completion_GetReadinessGroupUpdateFlag(Dem_Readiness_Completion_GetFullReadinessGroupUpdateFlag(), ReadinessGroup) == FALSE)
    {
      Dem_Readiness_Completion_SetReadinessGroupUpdateFlag(ReadinessGroup);
    }
  }
}

/* ****************************************************************************
 % Dem_Readiness_Completion_TriggerCompletionRecalculationAll
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_TriggerCompletionRecalculationAll(
  void
)
{
  if (Dem_GlobalDiagnostics_Readiness_IsReadinessCalculationSupported() == TRUE)
  {
    /* set flag for all readiness group supported by engine type to recalculate monitor state -> not supported groups will be set to N/A = completed*/
    if (Dem_GlobalDiagnostics_Readiness_IsMotorTypeCompressionSupported() == TRUE)
    {
      Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag(DEM_READINESS_INITIALIZE_UPDATE_COMPRESSION);
    }
    else
    {
      Dem_Readiness_Completion_SetFullReadinessGroupUpdateFlag(DEM_READINESS_INITIALIZE_UPDATE_SPARK);
    }
    Dem_Scheduler_EnableTask(Dem_Scheduler_Task_ReadinessCompletion);
  }
}

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_COMPLETION_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Completion_Implementation.h
 *********************************************************************************************************************/
