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
 *  \defgroup   Dem_Completion Completion
 *  \{
 *  \file       Dem_Completion_Interface.h
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

#if !defined (DEM_COMPLETION_INTERFACE_H)
#define DEM_COMPLETION_INTERFACE_H

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
#include "Dem_Completion_Types.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PUBLIC PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Completion_PublicProperties Public Properties
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

/* ********************************************************************************************************************
 *  UNIT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
/*!
 * \defgroup Dem_Completion_Public Public Methods
 * \{
 */

 /* ****************************************************************************
 *  Dem_Readiness_Completion_ProcessUpdateFlag
 *****************************************************************************/
 /*!
 * \brief         Calculates Readiness Completion.
 *
 * \details       Calculates Readiness Completion.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
 Dem_Readiness_Completion_ProcessUpdateFlag(
  void
);

/* ****************************************************************************
 * Dem_Readiness_Completion_SetStaticCompletionStates
 *****************************************************************************/
/*!
 * \brief         Set static completion states for respective data context.
 *
 * \details       Set static completion states for respective data context
 *                from generated code.
 *
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, PID41, DID F501, DM05.
 * \param[out]    CompletionStates
 *                Destination buffer with completion bits set for supported
 *                readiness groups and respective data context.
 *
 * \pre           Event availability is not supported in configuration.
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetStaticCompletionStates(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Completion_SetCompletionStates
 *****************************************************************************/
/*!
 * \brief         Set completion states for respective data context.
 *
 * \details       Set completion states for respective data context.
 *
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, DID F501, DM05.
 *
 * \param[out]    DestinationBuffer
 *                Destination buffer with completion bits set for respective
 *                data context.
 *
 * \pre           Completion bits are calculated and not statically set
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetCompletionStates(
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Completion_ResetReadinessGroupCompletedStates
 *****************************************************************************/
/*!
 * \brief        Resets globally the stored completion values of all readiness
 *               groups.
 *
 * \details      Resets globally the stored completion values of all readiness
 *               groups.
 *
 *
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_ResetReadinessGroupCompletedStates(
  void
);

/* ****************************************************************************
 * Dem_Readiness_Completion_ConsiderEventStatusForCompletionStates
 *****************************************************************************/
/*!
 * \brief         Consider event status for calculation of completion state of
 *                the event's readiness group.
 *
 * \details       Set completion state for event's readiness group in buffers
 *                according to event status (event status can lead to completion
 *                or prevent it).
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 * \param[in,out] CompletionMaskBuffer
 *                Buffer containing temporary completion bit values.
 * \param[in,out] DestinationBuffer
 *                Destination buffer with completion bits set.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_ConsiderEventStatusForCompletionStates(
  Dem_EventIdType EventId,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  CompletionMaskBuffer,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Completion_SetReadinessGroupCompleted
 *****************************************************************************/
/*!
 * \brief         Set the readiness group completed or not completed in the
 *                given buffer for the respective data context.
 *
 * \details       Set the readiness group completed or not completed in the
 *                given buffer for the respective data context.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[in]     Completed
 *                TRUE    Readiness group is set to completed
 *                FALSE   Readiness group is set to not completed
 * \param[in]     DataContext
 *                Data Context (which DID, PID etc.) for which the function
 *                is invoked.
 *                Currently considered: PID01, PID41, DID F501, DM05.
 * \param[in,out] DestinationBuffer
 *                Destination buffer
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_Readiness_Completion_SetReadinessGroupCompleted(
  uint8 ReadinessGroup,
  boolean Completed,
  uint8 DataContext,
  CONST(Dem_Data_DestinationBufferPtrType, AUTOMATIC)  DestinationBuffer
);

/* ****************************************************************************
 * Dem_Readiness_Completion_UpdateCompletionStatesAfterClear
 *****************************************************************************/
/*!
 * \brief         Update completion states after clear request.
 *
 * \details       Update completion states after clear request.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_UpdateCompletionStatesAfterClear(
  void
);

/* ****************************************************************************
 * Dem_Readiness_Completion_UpdateCompletionStatus
 *****************************************************************************/
/*!
 * \brief         Update completion status of event's readiness group.
 *
 * \details       Update completion status of event's readiness group.
 *
 * \param[in]     ReadinessGroup
 *                Readiness Group Identifier
 * \param[in]     Completed
 *                TRUE    Readiness group shall be set to completed.
 *                FALSE   Readiness group shall be set to not completed.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_UpdateCompletionStatus(
  uint8  ReadinessGroup,
  boolean Complete
);

/* ****************************************************************************
 * Dem_Readiness_Completion_TriggerCompletionRecalculation
 *****************************************************************************/
/*!
 * \brief         Trigger recalculation of completion status of event's
 *                readiness group.
 *
 * \details       Trigger recalculation of completion status of event's
 *                readiness group. Recalculation is deferred to main function.
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
Dem_Readiness_Completion_TriggerCompletionRecalculation(
  uint8  ReadinessGroup
);

/* ****************************************************************************
 * Dem_Readiness_Completion_TriggerCompletionRecalculationAll
 *****************************************************************************/
/*!
 * \brief         Trigger recalculation of completion states of all readiness
 *                groups.
 *
 * \details       Trigger recalculation of completion states of all readiness
 *                groups.
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Readiness_Completion_TriggerCompletionRecalculationAll(
  void
);

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */

#endif /* DEM_COMPLETION_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Completion_Interface.h
 *********************************************************************************************************************/
