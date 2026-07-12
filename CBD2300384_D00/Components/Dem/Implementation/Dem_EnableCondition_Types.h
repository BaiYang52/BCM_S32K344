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
 *  \file       Dem_EnableCondition_Types.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_ENABLECONDITION_TYPES_H)
#define DEM_ENABLECONDITION_TYPES_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                        /* Dem module header */
/* ------------------------------------------------------------------------- */
#include "Dem_Int.h"
/* ------------------------------------------------------------------------- */


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Handle for enable condition 'ControlDTCSetting' */
#define DEM_ENABLECONDITION_CONTROLDTCSETTING    0u

/*!
* \defgroup  Dem_EnableCondition_Group_State Macros encoding internal enable condition group state
* Macros encoding enable condition group state
* \{
*/
#define DEM_ENABLECONDITION_ACTIVE_STATE                                  0x01U  /*!< Flag to signalizes the current state of the enable condition group */
#define DEM_ENABLECONDITION_ACTIVE_STATE_MASK                             0xfeU  /*!< Inverse mask of the active enable condition group state */

#define DEM_ENABLECONDITION_QUEUED_DEACTIVATION_STATE                     0x02U  /*!< Flag which indicates a queued transition of an enable condition, which result in a planned deactivation of the active state */
#define DEM_ENABLECONDITION_QUEUED_DEACTIVATION_STATE_MASK                0xfdU  /*!< Inverse mask of the queued deactivation of an enable condition */

#define DEM_ENABLECONDITION_QUEUED_ACTIVATION_STATE                       0x04U  /*!< Flag which indicates a queued transition of an enable condition, which result in a planned activation of the active state */
#define DEM_ENABLECONDITION_QUEUED_ACTIVATION_STATE_MASK                  0xfbU  /*!< Inverse mask of the queued activation of an enable condition */

#define DEM_ENABLECONDITION_PROCESS_DEACTIVATION_TRANSITION_STATE         0x08U  /*!< Flag for process a deactivation transition from task function of the enable condition group state */
#define DEM_ENABLECONDITION_PROCESS_DEACTIVATION_TRANSITION_STATE_MASK    0xf7U  /*!< Inverse mask of the process a deactivation transition from task function flag of an enable condition */

#define DEM_ENABLECONDITION_PROCESS_ACTIVATION_TRANSITION_STATE           0x10U  /*!< Flag for process a activation transition from task function of the enable condition group state */
#define DEM_ENABLECONDITION_PROCESS_ACTIVATION_TRANSITION_STATE_MASK      0xefU  /*!< Inverse mask of the process a activation transition from task function flag of an enable condition */
/*!
* \}
*/

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! Iterator type for iteration over enable conditions of an enable condition group */
typedef struct Dem_Cfg_ComplexIterType_s Dem_Cfg_EventEnableConditionGroupIterType;

typedef uint8  Dem_EnableCondition_GroupState_StateType;         /*! Enable Condition Group State Type */
typedef uint16 Dem_EnableCondition_Group_IdentifierType;         /*! Enable Condition Group Identifier Type */


/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/

#endif /* DEM_ENABLECONDITION_TYPES_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_EnableCondition_Types.h
 *********************************************************************************************************************/
