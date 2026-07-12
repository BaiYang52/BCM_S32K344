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
/*! \addtogroup Dem_EnableCondition
 *  \{
 *  \file       Dem_EnableCondition_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Implementation of the EnableCondition subcomponent
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_ENABLECONDITION_IMPLEMENTATION_H)
#define DEM_ENABLECONDITION_IMPLEMENTATION_H

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
#include "Dem_EnableCondition_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_Satellite_Interface.h"
#include "Dem_DiagnosticObserver_Interface.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_ENABLECONDITION_IMPLEMENTATION_FILENAME "Dem_EnableCondition_Implementation.h"

/* ********************************************************************************************************************
 *  UNIT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_19.1 */
 /*!
 * \defgroup Dem_EnableCondition_PrivateProperties Private Properties
 * \{
 */

 
 /* ****************************************************************************
 * Dem_Conditions_EnableCondition_IsEnableConditionSupported
 *****************************************************************************/
 /*!
 * \brief         Tests if enable conditions are supported.
 *
 * \details       Tests if enable conditions are supported.
 *                ControlDTCSetting is used by the DCM and is implemented
 *                as an enable condition, therefore DCM support also leads
 *                to enable condition support.
 *
 * \return        TRUE
 *                Enable conditions are supported.
 * \return        FALSE
 *                Enable conditions are not supported.
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Conditions_EnableCondition_IsEnableConditionSupported(
  void
);

/* ****************************************************************************
 * Dem_Conditions_EnableCondition_IsNotifyEnableConditionSupported
 *****************************************************************************/
/*!
 * \brief         Tests if enable condition notifications are supported.
 *
 * \details       Tests if enable condition notifications are supported.
 *
 * \return        TRUE
 *                Enable condition notifications are supported.
 * \return        FALSE
 *                Enable condition notifications are not supported.
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Conditions_EnableCondition_IsNotifyEnableConditionSupported(
  void
);

/* ****************************************************************************
 * Dem_EnableCondition_GroupState_TestActive
 *****************************************************************************/
/*!
 * \brief         Test if the enable condition group is active.
 *
 * \details       Test fi the enable condition group is active.
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        TRUE
 *                Enable condition group is active
 *
 * \return        FALSE
 *                Enable condition group is inactive
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_EnableCondition_GroupState_TestActive(Dem_EnableCondition_GroupState_StateType State);

/* ****************************************************************************
 * Dem_EnableCondition_GroupState_SetActive
 *****************************************************************************/
/*!
 * \brief         Set the active state of an enable condition group
 *
 * \details       Set the active state of an enable condition group
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        Modified enable condition state
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_SetActive(Dem_EnableCondition_GroupState_StateType State);

/* ****************************************************************************
 * Dem_EnableCondition_GroupState_ResetActive
 *****************************************************************************/
/*!
 * \brief         Reset the active state of an enable condition group
 *
 * \details       Reset the active state of an enable condition group
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        Modified enable condition state
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_ResetActive(Dem_EnableCondition_GroupState_StateType State);

/* ****************************************************************************
 * Dem_EnableCondition_GroupState_TestQueueDeactivation
 *****************************************************************************/
/*!
 * \brief         Test, if the state to deactivate the enable condition group is 
 *                queued.
 *
 * \details       Test, if the state to deactivate the enable condition group is 
 *                queued.
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        TRUE
 *
 * \return        FALSE
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_EnableCondition_GroupState_TestQueueDeactivation(Dem_EnableCondition_GroupState_StateType State);

/* ****************************************************************************
 * Dem_EnableCondition_GroupState_SetQueueDeactivation
 *****************************************************************************/
/*!
 * \brief         Plan to activate the enable condition group in the next main function.
 *
 * \details       Plan to activate the enable condition group in the next main function.
 *                By setting a flag in the enable condition state.
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        Modified enable condition state
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_SetQueueDeactivation(Dem_EnableCondition_GroupState_StateType State);

/* ****************************************************************************
 * Dem_EnableCondition_GroupState_ResetQueueDeactivation
 *****************************************************************************/
/*!
 * \brief         Remove the planned deactivation of the enable condition group state.
 *
 * \details       Remove the planned deactivation of the enable condition group state.
 *                By resetting a flag in the enable condition state.
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        Modified enable condition state
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_ResetQueueDeactivation(Dem_EnableCondition_GroupState_StateType State);


/* ****************************************************************************
 * Dem_EnableCondition_GroupState_TestQueueActivation
 *****************************************************************************/
/*!
 * \brief         Test, if the state to activate the enable condition group is 
 *                queued.
 *
 * \details       Test, if the state to activate the enable condition group is 
 *                queued.
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        TRUE
 *
 * \return        FALSE
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_EnableCondition_GroupState_TestQueueActivation(Dem_EnableCondition_GroupState_StateType State);

/* ****************************************************************************
 * Dem_EnableCondition_GroupState_SetQueueActivation
 *****************************************************************************/
/*!
 * \brief         Plan to activate the enable condition group in the next main function.
 *
 * \details       Test the 'queued activation' state of an enable condition group
 *                By setting a flag in the enable condition state.
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        Modified enable condition state
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_SetQueueActivation(Dem_EnableCondition_GroupState_StateType State);

/* ****************************************************************************
 * Dem_EnableCondition_GroupState_ResetQueueActivation
 *****************************************************************************/
/*!
 * \brief         Remove the planned activation of the enable condition group state.
 *
 * \details       Remove the planned activation of the enable condition group state.
 *                By resetting a flag in the enable condition state.
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        Modified enable condition state
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_ResetQueueActivation(Dem_EnableCondition_GroupState_StateType State);

/* ****************************************************************************
 * Dem_EnableCondition_GroupState_TestProcessDeactivationTransition
 *****************************************************************************/
/*!
 * \brief         Test the 'process deactivation transition' state
 *
 * \details       -
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        TRUE
 *
 * \return        FALSE
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_EnableCondition_GroupState_TestProcessDeactivationTransition(Dem_EnableCondition_GroupState_StateType State);

/* ****************************************************************************
 * Dem_EnableCondition_GroupState_SetProcessDeactivationTransition
 *****************************************************************************/
/*!
 * \brief         Set the 'process deactivation transition' state
 *
 * \details       -
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        Modified enable condition state
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_SetProcessDeactivationTransition(Dem_EnableCondition_GroupState_StateType State);

/* ****************************************************************************
 * Dem_EnableCondition_GroupState_ResetProcessDeactivationTransition
 *****************************************************************************/
/*!
 * \brief         Reset the 'process deactivation transition' state
 *
 * \details       -
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        Modified enable condition state
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_ResetProcessDeactivationTransition(Dem_EnableCondition_GroupState_StateType State);

/* ****************************************************************************
 * Dem_EnableCondition_GroupState_TestProcessActivationTransition
 *****************************************************************************/
/*!
 * \brief         Test the 'process activation transition' state
 *
 * \details       -
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        TRUE
 *
 * \return        FALSE
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_EnableCondition_GroupState_TestProcessActivationTransition(Dem_EnableCondition_GroupState_StateType State);

/* ****************************************************************************
 * Dem_EnableCondition_GroupState_SetProcessActivationTransition
 *****************************************************************************/
/*!
 * \brief         Set the 'process activation transition' state
 *
 * \details       -
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        Modified enable condition state
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_SetProcessActivationTransition(Dem_EnableCondition_GroupState_StateType State);

/* ****************************************************************************
 * Dem_EnableCondition_GroupState_ResetProcessActivationTransition
 *****************************************************************************/
/*!
 * \brief         Reset the 'process activation transition' state
 *
 * \details       -
 *
 * \param[in]     State
 *                Input enable condition state
 *
 * \return        Modified enable condition state
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_ResetProcessActivationTransition(Dem_EnableCondition_GroupState_StateType State);


 /* ********************************************************************************************************************
 *  UNIT PRIVATE PROPERTIES DEFINITIONS
 *********************************************************************************************************************/

 /* ****************************************************************************
 % Dem_Conditions_EnableCondition_IsEnableConditionSupported
 *****************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Conditions_EnableCondition_IsEnableConditionSupported(
  void
)
{
  return (DEM_CFG_SUPPORT_ENABLE_CONDITIONS == STD_ON) || (DEM_CFG_SUPPORT_DCM == STD_ON);
}




/* ****************************************************************************
 % Dem_Conditions_EnableCondition_IsNotifyEnableConditionSupported
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Conditions_EnableCondition_IsNotifyEnableConditionSupported(
  void
)
{
  return DEM_CFG_NOTIFY_ENABLECONDITION == STD_ON;
}


/* ****************************************************************************
 % Dem_EnableCondition_GroupState_TestActive
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_EnableCondition_GroupState_TestActive(Dem_EnableCondition_GroupState_StateType State)
{
  return  (((State) & DEM_ENABLECONDITION_ACTIVE_STATE) != 0u);
}

/* ****************************************************************************
 % Dem_EnableCondition_GroupState_SetActive
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_SetActive(Dem_EnableCondition_GroupState_StateType State)
{
  return  ((uint8)((State) | DEM_ENABLECONDITION_ACTIVE_STATE));
}

/* ****************************************************************************
 % Dem_EnableCondition_GroupState_ResetActive
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_ResetActive(Dem_EnableCondition_GroupState_StateType State)
{
  return  ((uint8)((State) & DEM_ENABLECONDITION_ACTIVE_STATE_MASK));
}

/* ****************************************************************************
 % Dem_EnableCondition_GroupState_TestQueueDeactivation
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_EnableCondition_GroupState_TestQueueDeactivation(Dem_EnableCondition_GroupState_StateType State)
{
  return  (((State) & DEM_ENABLECONDITION_QUEUED_DEACTIVATION_STATE) != 0u);
}

/* ****************************************************************************
 % Dem_EnableCondition_GroupState_SetQueueDeactivation
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_SetQueueDeactivation(Dem_EnableCondition_GroupState_StateType State)
{
  return  ((uint8)((State) | DEM_ENABLECONDITION_QUEUED_DEACTIVATION_STATE));
}

/* ****************************************************************************
 % Dem_EnableCondition_GroupState_ResetQueueDeactivation
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_ResetQueueDeactivation(Dem_EnableCondition_GroupState_StateType State)
{
  return  ((uint8)((State) & DEM_ENABLECONDITION_QUEUED_DEACTIVATION_STATE_MASK));
}

/* ****************************************************************************
 % Dem_EnableCondition_GroupState_TestQueueActivation
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_EnableCondition_GroupState_TestQueueActivation(Dem_EnableCondition_GroupState_StateType State)
{
  return  (((State) & DEM_ENABLECONDITION_QUEUED_ACTIVATION_STATE) != 0u);
}

/* ****************************************************************************
 % Dem_EnableCondition_GroupState_SetQueueActivation
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_SetQueueActivation(Dem_EnableCondition_GroupState_StateType State)
{
  return  ((uint8)((State) | DEM_ENABLECONDITION_QUEUED_ACTIVATION_STATE));
}

/* ****************************************************************************
 % Dem_EnableCondition_GroupState_ResetQueueActivation
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_ResetQueueActivation(Dem_EnableCondition_GroupState_StateType State)
{
  return  ((uint8)((State) & DEM_ENABLECONDITION_QUEUED_ACTIVATION_STATE_MASK));
}

/* ****************************************************************************
 % Dem_EnableCondition_GroupState_TestProcessDeactivationTransition
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_EnableCondition_GroupState_TestProcessDeactivationTransition(Dem_EnableCondition_GroupState_StateType State)
{
  return  (((State) & DEM_ENABLECONDITION_PROCESS_DEACTIVATION_TRANSITION_STATE) != 0u);
}

/* ****************************************************************************
 % Dem_EnableCondition_GroupState_SetProcessDeactivationTransition
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_SetProcessDeactivationTransition(Dem_EnableCondition_GroupState_StateType State)
{
  return  ((uint8)((State) | DEM_ENABLECONDITION_PROCESS_DEACTIVATION_TRANSITION_STATE));
}

/* ****************************************************************************
 % Dem_EnableCondition_GroupState_ResetProcessDeactivationTransition
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_ResetProcessDeactivationTransition(Dem_EnableCondition_GroupState_StateType State)
{
  return  ((uint8)((State) & DEM_ENABLECONDITION_PROCESS_DEACTIVATION_TRANSITION_STATE_MASK));
}

/* ****************************************************************************
 % Dem_EnableCondition_GroupState_TestProcessActivationTransition
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_EnableCondition_GroupState_TestProcessActivationTransition(Dem_EnableCondition_GroupState_StateType State)
{
  return  (((State) & DEM_ENABLECONDITION_PROCESS_ACTIVATION_TRANSITION_STATE) != 0u);
}

/* ****************************************************************************
 % Dem_EnableCondition_GroupState_SetProcessActivationTransition
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_SetProcessActivationTransition(Dem_EnableCondition_GroupState_StateType State)
{
  return  ((uint8)((State) | DEM_ENABLECONDITION_PROCESS_ACTIVATION_TRANSITION_STATE));
}

/* ****************************************************************************
 % Dem_EnableCondition_GroupState_ResetProcessActivationTransition
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_GroupState_StateType, DEM_CODE)
Dem_EnableCondition_GroupState_ResetProcessActivationTransition(Dem_EnableCondition_GroupState_StateType State)
{
  return  ((uint8)((State) & DEM_ENABLECONDITION_PROCESS_ACTIVATION_TRANSITION_STATE_MASK));
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
 * \addtogroup Dem_EnableCondition_PublicProperties
 * \{
 */


/* ****************************************************************************
 % Dem_Conditions_EnableCondition_GetGlobalEnableConditionCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Conditions_EnableCondition_GetGlobalEnableConditionCount(
  void
  )
{
  return DEM_CFG_GLOBAL_ENABLE_COND_COUNT;
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
 * \defgroup Dem_EnableCondition_Private Private Methods
 * \{
 */

 /* ****************************************************************************
 * Dem_Condition_EnableCondition_GetGlobalGroupCount
 *****************************************************************************/
 /*!
 * \brief         Get the number of configured enable condition groups
 *
 * \details       Get the number of configured enable condition groups
 *
 * \return        The number of configured enable condition groups
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Condition_EnableCondition_GetGlobalGroupCount(
  void
);



/* ****************************************************************************
 * Dem_Conditions_EnableCondition_GetEnableConditionInitValue
 *****************************************************************************/
/*!
 * \brief         Get the initial value of enable condition
 *
 * \details       Get the initial value of enable condition
 *
 * \param[in]     EnableCondition
 *                Unique handle of the enable condition.
 *
 * \return        TRUE
 *                The initial value of enable condition is TRUE
 * \return        FALSE
 *                The initial value of enable condition is FALSE
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Conditions_EnableCondition_GetEnableConditionInitValue(
  CONST(uint8, AUTOMATIC) EnableCondition
  );




/* ****************************************************************************
 * Dem_EnableCondition_Group_GetState
 *****************************************************************************/
/*!
 * \brief         Get current internal state of enable condition group
 *
 * \details       Get current internal state of enable condition group
 *
 * \param[in]     EnableConditionGroupId

 *                Unique handle of the enable condition group
 *
 * \return        Current activation state of the enable condition group
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_EnableCondition_Group_GetState(
  CONST(Dem_EnableCondition_Group_IdentifierType, AUTOMATIC)  EnableConditionGroupId

  );


/* ****************************************************************************
 * Dem_EnableCondition_Group_GetNotFulfilledCount
 *****************************************************************************/
/*!
 * \brief         Get current internal counter of not fulfilled enable conditions
 *
 * \details       Get current internal counter of not fulfilled enable conditions
 *
 * \param[in]     EnableConditionGroupId

 *                Unique handle of the enable condition group
 *
 * \return        Current number of deactivated enable conditions currently deactivating the 
 *                group.
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_EnableCondition_Group_GetNotFulfilledCount(
  CONST(Dem_EnableCondition_Group_IdentifierType, AUTOMATIC)  EnableConditionGroupId
  );


/* ****************************************************************************
 * Dem_Conditions_EnableCondition_SetInternallyFulfilled
 *****************************************************************************/
/*!
 * \brief         Set current activation status of enable condition
 *
 * \details       Set current activation status of enable condition
 *
 * \param[in]     EnableConditionID
 *                Unique handle of the enable condition
 * \param[in]     ActiveStatus
 *                New activation state of the enable condition
 *
 * \pre           -
 * 
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Conditions_EnableCondition_SetInternallyFulfilled(
  CONST(uint8, AUTOMATIC)  EnableConditionID,
  CONST(boolean, AUTOMATIC)  ActiveStatus
  );


/* ****************************************************************************
 * Dem_EnableCondition_Group_SetState
 *****************************************************************************/
/*!
 * \brief         Set current internal state of enable condition group
 *
 * \details       Set current internal state of enable condition group
 *
 * \param[in]     EnableConditionGroupId

 *                Unique handle of the enable condition group
 * \param[in]     State
 *                New internal state of the enable condition group
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_SetState(
  CONST(Dem_EnableCondition_Group_IdentifierType, AUTOMATIC)  EnableConditionGroupId,
  CONST(uint8, AUTOMATIC)  State
  );


/* ****************************************************************************
 * Dem_EnableCondition_Group_SetCounter
 *****************************************************************************/
/*!
 * \brief         Set current internal counter of deactivated enable condition group
 *
 * \details       Set current internal counter of deactivated enable condition group
 *
 * \param[in]     EnableConditionGroupId
 *                Unique handle of the enable condition group
 * \param[in]     Counter
 *                New internal counter of the deactivated enable condition group
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_SetCounter(
  CONST(Dem_EnableCondition_Group_IdentifierType, AUTOMATIC)  EnableConditionGroupId,
  CONST(uint8, AUTOMATIC)  Counter
  );

/* ****************************************************************************
 * Dem_EnableCondition_Group_ConditionNotFulfilled
 *****************************************************************************/
/*!
 * \brief         Set current internal counter of enable condition group
 *
 * \details       Set current internal counter of enable condition group
 *
 * \param[in]     EnableConditionGroupId

 *                 Unique handle of the enable condition group
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_ConditionNotFulfilled(
  CONST(Dem_EnableCondition_Group_IdentifierType, AUTOMATIC)  EnableConditionGroupId

);

/* ****************************************************************************
 * Dem_EnableCondition_Group_ConditionFulfilled
 *****************************************************************************/
/*!
 * \brief         Set current internal counter of enable condition group
 *
 * \details       Set current internal counter of enable condition group
 *
 * \param[in]     EnableConditionGroupId

 *                 Unique handle of the enable condition group
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_ConditionFulfilled(
  CONST(Dem_EnableCondition_Group_IdentifierType, AUTOMATIC)  EnableConditionGroupId

);

/* ****************************************************************************
 * Dem_Conditions_EnableCondition_TaskFunction
 *****************************************************************************/
/*!
 * \brief         Processes event specific changes of enable condition states.
 *
 * \details       This function resets or freezes debouncing of an event, based
 *                on the changes of the enable condition states since this
 *                function was called last. When the event is re-enabled, this
 *                function also calls the init monitor notifications.
 *
 * \param[in]     EventId
 *                Unique handle of the Event.
 *                The EventId must be in range [1..Dem_Cfg_GlobalEventCount()[.
 *
 * \pre           Can only be called within Critical Section StateManager
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Conditions_EnableCondition_TaskFunction(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );      


/* ****************************************************************************
 * Dem_EnableCondition_Group_IterExists
 *****************************************************************************/
/*!
 * \brief         Test if the iterator points to a valid element
 *
 * \details       Test if the iterator points to a valid element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 * 
 * \return        TRUE
 *                Dem_EnableCondition_Group_GetIter() will return a valid element
 *                on this iterator
 * \return        FALSE
 *                Dem_EnableCondition_Group_GetIter() is undefined on this iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_EnableCondition_Group_IterExists(
  CONSTP2CONST(Dem_Cfg_EventEnableConditionGroupIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );


/* ****************************************************************************
 * Dem_EnableCondition_Group_GetIter
 *****************************************************************************/
/*!
 * \brief         Get the element pointed to by the iterator
 *
 * \details       Get the element pointed to by the iterator
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \return        Handle to Enable condition group descriptor table
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_Group_IdentifierType, DEM_CODE)
Dem_EnableCondition_Group_GetIter(
  CONSTP2CONST(Dem_Cfg_EventEnableConditionGroupIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );


/* ****************************************************************************
 * Dem_EnableCondition_Group_InitIter
 *****************************************************************************/
/*!
 * \brief         Init the iterator
 *
 * \details       Initializes the iterator to enumerate enable condition groups
 *                referencing given enable condition
 *
 * \param[in]     EnableConditionID
 *                Unique handle of the enable condition.
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_InitIter(
  CONST(uint8, AUTOMATIC)  EnableConditionID,
  CONSTP2VAR(Dem_Cfg_EventEnableConditionGroupIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );


/* ****************************************************************************
 * Dem_EnableCondition_Group_NextIter
 *****************************************************************************/
/*!
 * \brief         Move the iterator to the next element
 *
 * \details       Move the iterator to the next element
 *
 * \param[in]     IterPtr
 *                Pointer to the iterator
 *
 * \pre           IterPtr has been initialized
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_NextIter(
  CONSTP2VAR(Dem_Cfg_EventEnableConditionGroupIterType, AUTOMATIC, AUTOMATIC) IterPtr
  );

/* ****************************************************************************
 * Dem_EnableCondition_Group_Init
 *****************************************************************************/
/*!
 * \brief         Initialize enable condition group
 *
 * \details       Initialize enable condition group
 *
 * \param[in]     GroupId
 *                Enable condition group identifier
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_Init(
  Dem_EnableCondition_Group_IdentifierType GroupId
);

/* ****************************************************************************
 * Dem_EnableCondition_Group_MainFunctionProcessQueuedChanges
 *****************************************************************************/
/*!
 * \brief         Process queued enable condition group changes.
 *
 * \details       Process queued enable condition group changes.
 *
 * \param[in]     GroupId
 *                Enable condition group identifier
 *
 * \pre           Can only be called within Critical Section StateManager
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_MainFunctionProcessQueuedChanges(
  Dem_EnableCondition_Group_IdentifierType GroupId
);

/* ****************************************************************************
 * Dem_EnableCondition_Group_MainFunctionUpdateState
 *****************************************************************************/
/*!
 * \brief         Update enable condition group state.
 *
 * \details       Update enable condition group state.
 *
 * \param[in]     GroupId
 *                Enable condition group identifier
 *
 * \pre           Can only be called within Critical Section StateManager
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_MainFunctionUpdateState(
  Dem_EnableCondition_Group_IdentifierType GroupId
);


/* ****************************************************************************
 * Dem_EnableCondition_Group_SetFulfilled
 *****************************************************************************/
/*!
 * \brief         Update enable condition group state.
 *
 * \details       Update enable condition group state.
 *
 * \param[in]     GroupId
 *                Enable condition group identifier
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_SetFulfilled(
  Dem_EnableCondition_Group_IdentifierType GroupId
);

/* ****************************************************************************
 * Dem_EnableCondition_Group_ResetFulfilled
 *****************************************************************************/
/*!
 * \brief         Clear enable condition group state.
 *
 * \details       Clear enable condition group state.
 *
 * \param[in]     GroupId
 *                Enable condition group identifier
 *
 * \pre           Dem_EnableCondition_Group_SetState
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_ResetFulfilled(
  Dem_EnableCondition_Group_IdentifierType GroupId
);



/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/

/* ****************************************************************************
 % Dem_Condition_EnableCondition_GetGlobalGroupCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_Condition_EnableCondition_GetGlobalGroupCount(
  void
  )
{
  return (uint8)Dem_Cfg_GetSizeOfEnableConditionGroupTable();
}

/* ****************************************************************************
 % Dem_Conditions_EnableCondition_GetEnableConditionInitValue
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Conditions_EnableCondition_GetEnableConditionInitValue(
  CONST(uint8, AUTOMATIC) EnableCondition
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EnableCondition)                                                                              /* PRQA S 3112 */ /* MD_DEM_14.2 */
    return Dem_Cfg_IsFulfilledAtInitOfEnableConditionInitTable(EnableCondition);
}

/* ****************************************************************************
 % Dem_EnableCondition_Group_GetState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_EnableCondition_Group_GetState(
  CONST(Dem_EnableCondition_Group_IdentifierType, AUTOMATIC)  EnableConditionGroupId
  )
{
  return Dem_Cfg_GetEnableConditionGroupState(EnableConditionGroupId);                                                           /* PRQA S 2843 */ /* MD_DEM_Dir4.1_ReadOperation */
}

/* ****************************************************************************
 % Dem_EnableCondition_Group_GetNotFulfilledCount
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_EnableCondition_Group_GetNotFulfilledCount(
  CONST(Dem_EnableCondition_Group_IdentifierType, AUTOMATIC)  EnableConditionGroupId

  )
{
  return Dem_Cfg_GetEnableConditionGroupCounter(EnableConditionGroupId);
}

/* ****************************************************************************
 % Dem_Conditions_EnableCondition_SetInternallyFulfilled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Conditions_EnableCondition_SetInternallyFulfilled(
  CONST(uint8, AUTOMATIC)  EnableConditionID,
  CONST(boolean, AUTOMATIC)  ActiveStatus
  )
{
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (EnableConditionID >= Dem_Cfg_GetSizeOfEnableConditionFulfilled())
  {
    Dem_Error_RunTimeCheckFailed(DEM_ENABLECONDITION_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#endif
  {
    Dem_Cfg_SetEnableConditionFulfilled(EnableConditionID, ActiveStatus);                                                        /* SBSW_DEM_ARRAY_WRITE_ENABLECONDITIONFULFILLED */
  }
}

/* ****************************************************************************
 % Dem_EnableCondition_Group_SetState
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_SetState(
  CONST(Dem_EnableCondition_Group_IdentifierType, AUTOMATIC)  EnableConditionGroupId,
  CONST(uint8, AUTOMATIC)  State
  )
{
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (EnableConditionGroupId >= Dem_Cfg_GetSizeOfEnableConditionGroupState())
  {
    Dem_Error_RunTimeCheckFailed(DEM_ENABLECONDITION_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#endif
  {
    Dem_Cfg_SetEnableConditionGroupState(EnableConditionGroupId, State);                                                         /* SBSW_DEM_ARRAY_WRITE_ENABLECONDITIONGROUPSTATE */
  }
}

/* ****************************************************************************
 % Dem_EnableCondition_Group_SetCounter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_SetCounter(
  CONST(Dem_EnableCondition_Group_IdentifierType, AUTOMATIC)  EnableConditionGroupId,
  CONST(uint8, AUTOMATIC)  Counter
  )
{
#if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
  if (EnableConditionGroupId >= Dem_Cfg_GetSizeOfEnableConditionGroupCounter())
  {
    Dem_Error_RunTimeCheckFailed(DEM_ENABLECONDITION_IMPLEMENTATION_FILENAME, __LINE__);                                         /* SBSW_DEM_POINTER_RUNTIME_CHECK */
  }
  else
#endif
  {
    Dem_Cfg_SetEnableConditionGroupCounter(EnableConditionGroupId, Counter);                                                     /* SBSW_DEM_ARRAY_WRITE_ENABLECONDITIONGROUPCOUNTER */
  }
}

/* ****************************************************************************
 % Dem_EnableCondition_Group_ConditionNotFulfilled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_ConditionNotFulfilled(
  CONST(Dem_EnableCondition_Group_IdentifierType, AUTOMATIC)  EnableConditionGroupId)
{
  Dem_EnableCondition_Group_SetCounter(EnableConditionGroupId,
    (uint8)(Dem_EnableCondition_Group_GetNotFulfilledCount(EnableConditionGroupId) + 1u));
}

/* ****************************************************************************
 % Dem_EnableCondition_Group_ConditionFulfilled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_ConditionFulfilled(
  CONST(Dem_EnableCondition_Group_IdentifierType, AUTOMATIC)  EnableConditionGroupId)
{
  Dem_EnableCondition_Group_SetCounter(EnableConditionGroupId,
    (uint8)(Dem_EnableCondition_Group_GetNotFulfilledCount(EnableConditionGroupId) - 1u));
}

/* ****************************************************************************
 % Dem_Conditions_EnableCondition_TaskFunction
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Conditions_EnableCondition_TaskFunction(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
)
{
  Dem_EnableCondition_Group_IdentifierType lEnableConditionGroupId;
  Dem_EnableCondition_GroupState_StateType lGroupState;

  lEnableConditionGroupId = Dem_Core_Event_GetEnableConditionGroupId(EventId);
  lGroupState = Dem_EnableCondition_Group_GetState(lEnableConditionGroupId);

  /* Transition to Inactive */
  if (Dem_EnableCondition_GroupState_TestProcessDeactivationTransition(lGroupState))
  {
    Dem_SatelliteCore_ProcessEnableConditionTransitionToInactive(EventId);
  }

  /* Transition to Active */
  if (Dem_EnableCondition_GroupState_TestProcessActivationTransition(lGroupState))
  {
    Dem_SatelliteCore_ProcessEnableConditionTransitionToActive(EventId);

    if ((Dem_Conditions_EnableCondition_IsNotifyEnableConditionSupported() == TRUE))
    {
      Dem_Notifications_InitMonitorForEvent(EventId, DEM_INIT_MONITOR_REENABLED);
      Dem_Notifications_InitMonitorForFunction(EventId);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_EnableCondition_Group_IterExists
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_EnableCondition_Group_IterExists(
  CONSTP2CONST(Dem_Cfg_EventEnableConditionGroupIterType, AUTOMATIC, AUTOMATIC) IterPtr                                          /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  return (boolean)(IterPtr->mIdx < IterPtr->mEndIdx);
}


/* ****************************************************************************
 % Dem_EnableCondition_Group_GetIter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(Dem_EnableCondition_Group_IdentifierType, DEM_CODE)
Dem_EnableCondition_Group_GetIter(
  CONSTP2CONST(Dem_Cfg_EventEnableConditionGroupIterType, AUTOMATIC, AUTOMATIC) IterPtr                                          /* PRQA S 3206 */ /* MD_DEM_3206 */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(IterPtr)                                                                                      /* PRQA S 3112 */ /* MD_DEM_14.2 */
  return Dem_Cfg_GetEnableConditionGroupTableInd(IterPtr->mIdx);
}

/* ****************************************************************************
 % Dem_EnableCondition_Group_InitIter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_InitIter(
  CONST(uint8, AUTOMATIC)  EnableConditionID,                                                                                    /* PRQA S 3206 */ /* MD_DEM_3206 */
  CONSTP2VAR(Dem_Cfg_EventEnableConditionGroupIterType, AUTOMATIC, AUTOMATIC) IterPtr                                            /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EnableConditionID)                                                                            /* PRQA S 3112 */ /* MD_DEM_14.2 */
  IterPtr->mIdx = Dem_Cfg_GetEnableConditionGroupTableIndStartIdxOfEnableConditionTable(EnableConditionID);                      /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
  IterPtr->mEndIdx = Dem_Cfg_GetEnableConditionGroupTableIndEndIdxOfEnableConditionTable(EnableConditionID);                     /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
}

/* ****************************************************************************
 % Dem_EnableCondition_Group_NextIter
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_NextIter(
  CONSTP2VAR(Dem_Cfg_EventEnableConditionGroupIterType, AUTOMATIC, AUTOMATIC) IterPtr                                            /* PRQA S 3206, 3673 */ /* MD_DEM_3206, MD_DEM_8.13_fp */
  )
{
  IterPtr->mIdx += 1u;                                                                                                           /* SBSW_DEM_POINTER_WRITE_ARGUMENT */
} 

/* ****************************************************************************
 % Dem_EnableCondition_Group_Init
 *****************************************************************************/
/*!
 * Internal comment removed. *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_Init(
  Dem_EnableCondition_Group_IdentifierType GroupId
)
{
  Dem_EnableCondition_GroupState_StateType lGroupState;

  Dem_EnableCondition_Group_ConditionNotFulfilled(GroupId);

  lGroupState = Dem_EnableCondition_GroupState_ResetActive(Dem_EnableCondition_Group_GetState(GroupId));
  Dem_EnableCondition_Group_SetState(GroupId, lGroupState);
}

/* ****************************************************************************
 % Dem_EnableCondition_Group_MainFunctionProcessQueuedChanges
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_MainFunctionProcessQueuedChanges(
  Dem_EnableCondition_Group_IdentifierType GroupId
)
{
  Dem_EnableCondition_GroupState_StateType lGroupState;
  lGroupState = Dem_EnableCondition_Group_GetState(GroupId);
  if (Dem_EnableCondition_GroupState_TestQueueDeactivation(lGroupState))
  {
    lGroupState = Dem_EnableCondition_GroupState_ResetQueueDeactivation(lGroupState);
    if (Dem_EnableCondition_Group_GetNotFulfilledCount(GroupId) == 1u)
    {
      lGroupState = Dem_EnableCondition_GroupState_SetProcessDeactivationTransition(lGroupState);
    }
  }
  if (Dem_EnableCondition_GroupState_TestQueueActivation(lGroupState))
  {
    lGroupState = Dem_EnableCondition_GroupState_ResetQueueActivation(lGroupState);
    if (Dem_EnableCondition_Group_GetNotFulfilledCount(GroupId) == 0u)
    {
      lGroupState = Dem_EnableCondition_GroupState_SetProcessActivationTransition(lGroupState);
    }
  }
  Dem_EnableCondition_Group_SetState(GroupId, lGroupState);
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_EnableCondition_Group_MainFunctionUpdateState
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_MainFunctionUpdateState(
  Dem_EnableCondition_Group_IdentifierType GroupId
)
{
  Dem_EnableCondition_GroupState_StateType lGroupState;
  lGroupState = Dem_EnableCondition_Group_GetState(GroupId);
  if (Dem_EnableCondition_GroupState_TestProcessActivationTransition(lGroupState))
  {
    lGroupState = Dem_EnableCondition_GroupState_SetActive(lGroupState);
    lGroupState = Dem_EnableCondition_GroupState_ResetProcessActivationTransition(lGroupState);
  }
  lGroupState = Dem_EnableCondition_GroupState_ResetProcessDeactivationTransition(lGroupState);
  Dem_EnableCondition_Group_SetState(GroupId, lGroupState);
}

/* ****************************************************************************
 % Dem_EnableCondition_Group_SetFulfilled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_SetFulfilled(
  Dem_EnableCondition_Group_IdentifierType GroupId
)
{
  Dem_EnableCondition_GroupState_StateType lGroupState;

  if (Dem_EnableCondition_Group_GetNotFulfilledCount(GroupId) > 0u)
  {
    Dem_EnableCondition_Group_ConditionFulfilled(GroupId);
  }

  lGroupState = Dem_EnableCondition_Group_GetState(GroupId);
  lGroupState = Dem_EnableCondition_GroupState_SetQueueActivation(lGroupState);
  Dem_EnableCondition_Group_SetState(GroupId, lGroupState);
}

/* ****************************************************************************
 % Dem_EnableCondition_Group_ResetFulfilled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_EnableCondition_Group_ResetFulfilled(
  Dem_EnableCondition_Group_IdentifierType GroupId
)
{
  Dem_EnableCondition_GroupState_StateType lGroupState;

  Dem_EnableCondition_Group_ConditionNotFulfilled(GroupId);

  lGroupState = Dem_EnableCondition_Group_GetState(GroupId);
  lGroupState = Dem_EnableCondition_GroupState_ResetActive(lGroupState);
  lGroupState = Dem_EnableCondition_GroupState_SetQueueDeactivation(lGroupState);
  lGroupState = Dem_EnableCondition_GroupState_ResetQueueActivation(lGroupState);
  Dem_EnableCondition_Group_SetState(GroupId, lGroupState);
}



/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_EnableCondition_Public
 * \{
 */

/* ****************************************************************************
 % Dem_Conditions_EnableCondition_SetFulfilled
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
  Dem_Conditions_EnableCondition_SetFulfilled(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(uint8, AUTOMATIC)  EnableConditionID
  )
{
  if (Dem_Conditions_EnableCondition_IsEnableConditionSupported() == TRUE)                                                       /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    boolean lEnableConditionGroupStatePending;

    lEnableConditionGroupStatePending = FALSE;

    /* >>>> -------------------------------- Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();

    if (Dem_Conditions_EnableCondition_TestFulfilled(EnableConditionID) == FALSE)
    {
      Dem_Cfg_EventEnableConditionGroupIterType lEnableConditionIter;
      for (Dem_EnableCondition_Group_InitIter(EnableConditionID, &lEnableConditionIter);                                         /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_EnableCondition_Group_IterExists(&lEnableConditionIter) == TRUE;                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_EnableCondition_Group_NextIter(&lEnableConditionIter))                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        Dem_EnableCondition_Group_SetFulfilled(Dem_EnableCondition_Group_GetIter(&lEnableConditionIter));                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
        lEnableConditionGroupStatePending = TRUE;
      }
      Dem_Conditions_EnableCondition_SetInternallyFulfilled(EnableConditionID, TRUE);
    }

    Dem_LeaveCritical_StateManager();
    /* <<<< -------------------------------- Leave Critical Section: StateManager */

    if (lEnableConditionGroupStatePending == TRUE)
    {
      Dem_Scheduler_EnableTaskOnce(Dem_Scheduler_Task_EnableCondition);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Conditions_EnableCondition_ResetFulfilled
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
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Conditions_EnableCondition_ResetFulfilled(                                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(uint8, AUTOMATIC)  EnableConditionID
  )
{
  if (Dem_Conditions_EnableCondition_IsEnableConditionSupported() == TRUE)                                                       /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    boolean lEnableConditionGroupStatePending;

    lEnableConditionGroupStatePending = FALSE;

    /* >>>> -------------------------------- Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();

    if (Dem_Conditions_EnableCondition_TestFulfilled(EnableConditionID) == TRUE)
    {
      Dem_Cfg_EventEnableConditionGroupIterType lEnableConditionIter;
      for (Dem_EnableCondition_Group_InitIter(EnableConditionID, &lEnableConditionIter);                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_EnableCondition_Group_IterExists(&lEnableConditionIter) == TRUE;                                                      /* SBSW_DEM_CALL_ITERATOR_POINTER */
        Dem_EnableCondition_Group_NextIter(&lEnableConditionIter))                                                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
      {
        Dem_EnableCondition_Group_ResetFulfilled(Dem_EnableCondition_Group_GetIter(&lEnableConditionIter));                       /* SBSW_DEM_CALL_ITERATOR_POINTER */


        lEnableConditionGroupStatePending = TRUE;
      }
      Dem_Conditions_EnableCondition_SetInternallyFulfilled(EnableConditionID, FALSE);
    }

    Dem_LeaveCritical_StateManager();
    /* <<<< -------------------------------- Leave Critical Section: StateManager */

    if (lEnableConditionGroupStatePending == TRUE)
    {
      Dem_Scheduler_EnableTaskOnce(Dem_Scheduler_Task_EnableCondition);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_Conditions_EnableCondition_MainFunction
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
Dem_Conditions_EnableCondition_MainFunction(
  void
  )
{
  if (Dem_Conditions_EnableCondition_IsEnableConditionSupported() == TRUE)                                                       /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_EnableCondition_Group_IdentifierType lEnableConditionGroupId;
    Dem_EventIdType lEventId;

    /* >>>> -------------------------------- Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();

    for (lEnableConditionGroupId = 0u; 
         lEnableConditionGroupId < Dem_Condition_EnableCondition_GetGlobalGroupCount();
         ++lEnableConditionGroupId)
    {
      Dem_EnableCondition_Group_MainFunctionProcessQueuedChanges(lEnableConditionGroupId);
    }

    Dem_LeaveCritical_StateManager();
    /* <<<< -------------------------------- Leave Critical Section: StateManager */

    for (lEventId = DEM_EVENT_FIRST; lEventId < Dem_Cfg_GlobalEventCount(); ++lEventId)
    {
      if (Dem_Event_TestEventAvailable(lEventId) == TRUE)
      {
        /* Process enable condition groups for every event */
        Dem_Conditions_EnableCondition_TaskFunction(lEventId);
      }
    }

    /* >>>> -------------------------------- Enter Critical Section: StateManager */
    Dem_EnterCritical_StateManager();

    /* Remove process flags from group state of all enable condition groups */
    for (lEnableConditionGroupId = 0u; 
         lEnableConditionGroupId < Dem_Condition_EnableCondition_GetGlobalGroupCount(); 
         ++lEnableConditionGroupId)
    {
      Dem_EnableCondition_Group_MainFunctionUpdateState(lEnableConditionGroupId);
    }

    Dem_LeaveCritical_StateManager();
    /* <<<< -------------------------------- Leave Critical Section: StateManager */
  }
}                                                                                                                                /* PRQA S 6080, 6050 */ /* MD_MSR_STMIF, MD_MSR_STCAL */


/* ****************************************************************************
 % Dem_Conditions_EnableCondition_PreInit
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Conditions_EnableCondition_PreInit(
  void
)
{
  if (Dem_Conditions_EnableCondition_IsEnableConditionSupported() == TRUE)
  {
    Dem_EnableCondition_GroupState_StateType lGroupState;
    Dem_EnableCondition_Group_IdentifierType lEnableConditionGroupId;
    uint8 lEnableCondition;

    /* Initialize enable and storage conditions (all enabled due to init phase) */
    for (lEnableCondition = 0u;
      lEnableCondition < Dem_Cfg_GetSizeOfEnableConditionFulfilled();
      ++lEnableCondition)
    {
      Dem_Conditions_EnableCondition_SetInternallyFulfilled(lEnableCondition, TRUE);
    }

    for (lEnableConditionGroupId = 0u;
      lEnableConditionGroupId < Dem_Cfg_GetSizeOfEnableConditionGroupCounter();
      ++lEnableConditionGroupId)
    {
      Dem_EnableCondition_Group_SetCounter(lEnableConditionGroupId, 0u);
    }

    lGroupState = 0u;
    lGroupState = Dem_EnableCondition_GroupState_SetActive(lGroupState);
    for (lEnableConditionGroupId = 0u;
      lEnableConditionGroupId < Dem_Cfg_GetSizeOfEnableConditionGroupState();
      ++lEnableConditionGroupId)
    {
      Dem_EnableCondition_Group_SetState(lEnableConditionGroupId, lGroupState);
    }
  }
}


/* ****************************************************************************
 % Dem_Conditions_EnableCondition_Init
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
Dem_Conditions_EnableCondition_Init(
  void
  )
{
  if (Dem_Conditions_EnableCondition_IsEnableConditionSupported() == TRUE)
  {
    Dem_EnableCondition_GroupState_StateType lGroupState;
    Dem_EnableCondition_Group_IdentifierType lEnableConditionGroupId;
    uint8 lEnableCondition;

    for (lEnableConditionGroupId = 0u;
      lEnableConditionGroupId < Dem_Cfg_GetSizeOfEnableConditionGroupCounter();
      ++lEnableConditionGroupId)
    {
      Dem_EnableCondition_Group_SetCounter(lEnableConditionGroupId, 0u);
    }

    lGroupState = 0u;
    lGroupState = Dem_EnableCondition_GroupState_SetActive(lGroupState);
    for (lEnableConditionGroupId = 0u;
      lEnableConditionGroupId < Dem_Cfg_GetSizeOfEnableConditionGroupState();
      ++lEnableConditionGroupId)
    {
      Dem_EnableCondition_Group_SetState(lEnableConditionGroupId, lGroupState);
    }

    for (lEnableCondition = 0u;
      lEnableCondition < Dem_Conditions_EnableCondition_GetGlobalEnableConditionCount();
      ++lEnableCondition)
    {
      boolean lEnableConditionInitState;
      Dem_Cfg_EventEnableConditionGroupIterType lEnableConditionIter;

      lEnableConditionInitState = Dem_Conditions_EnableCondition_GetEnableConditionInitValue(lEnableCondition);

      if (lEnableConditionInitState == FALSE)
      {
        for (Dem_EnableCondition_Group_InitIter(lEnableCondition, &lEnableConditionIter);                                          /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_EnableCondition_Group_IterExists(&lEnableConditionIter) == TRUE;                                                     /* SBSW_DEM_CALL_ITERATOR_POINTER */
          Dem_EnableCondition_Group_NextIter(&lEnableConditionIter))                                                               /* SBSW_DEM_CALL_ITERATOR_POINTER */
        {
          Dem_EnableCondition_Group_Init(Dem_EnableCondition_Group_GetIter(&lEnableConditionIter));                                /* SBSW_DEM_CALL_ITERATOR_POINTER */
        }
      }
      Dem_Conditions_EnableCondition_SetInternallyFulfilled(lEnableCondition, lEnableConditionInitState);
    }
  }
}                                                                                                                                /* PRQA S 6050 */ /* MD_MSR_STCAL */

/* ****************************************************************************
 % Dem_Conditions_EnableCondition_TestEventEnabled
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
Dem_Conditions_EnableCondition_TestEventEnabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  if (Dem_Conditions_EnableCondition_IsEnableConditionSupported() == TRUE)
  {
    Dem_EnableCondition_Group_IdentifierType lEnableConditionGroupId;
    Dem_EnableCondition_GroupState_StateType lGroupState;

    lEnableConditionGroupId = Dem_Core_Event_GetEnableConditionGroupId(EventId);
    lGroupState = Dem_EnableCondition_Group_GetState(lEnableConditionGroupId);

    return (boolean)Dem_EnableCondition_GroupState_TestActive(lGroupState);
  }
  else
  {
    return TRUE;
  }
}                                                                                                                              

/* ****************************************************************************
 % Dem_Conditions_EnableCondition_TestFulfilled
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
  Dem_Conditions_EnableCondition_TestFulfilled(
  CONST(uint8, AUTOMATIC)  EnableCondition
  )
{
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EnableCondition)                                                                                    /* PRQA S 1338, 3112 */ /* MD_MSR_DummyStmt, MD_DEM_14.2 */
#if ((DEM_CFG_SUPPORT_ENABLE_CONDITIONS == STD_ON) || (DEM_CFG_SUPPORT_DCM == STD_ON))
  return Dem_Cfg_IsEnableConditionFulfilled(EnableCondition);
#else
  return TRUE;
#endif
}

/* ****************************************************************************
 % Dem_Condition_EnableCondition_TestEnableConditionStateOfEvent
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_Condition_EnableCondition_TestEnableConditionStateOfEvent(                                                                   /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  )
{
  boolean ConditionFulfilled;
  ConditionFulfilled = FALSE;
  if (Dem_Conditions_EnableCondition_IsEnableConditionSupported() == TRUE)                                                       /* COV_DEM_UNIT_ENTRY_CONFIGURATION_CHECK */
  {
    Dem_EnableCondition_Group_IdentifierType lEnableConditionGroupId;
    uint8 lCounter;

    lEnableConditionGroupId = Dem_Core_Event_GetEnableConditionGroupId(EventId);
    lCounter = Dem_EnableCondition_Group_GetNotFulfilledCount(lEnableConditionGroupId);

    /* all enable conditions fulfilled*/
    if (lCounter == 0u)
    {
      ConditionFulfilled = TRUE;
    }
    /* all enable conditions fulfilled except of ControlDTCSetting */
    else if ((Dem_Conditions_EnableCondition_TestFulfilled(DEM_ENABLECONDITION_CONTROLDTCSETTING) == FALSE)
      && (lCounter == 1u))
    {
      ConditionFulfilled = TRUE;
    }
    else
    {
      /* MISRA */
    }
  }
  return ConditionFulfilled;
}


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */


#endif /* DEM_ENABLECONDITION_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_EnableCondition_Implementation.h
 *********************************************************************************************************************/
