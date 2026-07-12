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
 *  \defgroup   Dem_Indicator Indicator
 *  \{
 *  \file       Dem_Indicator_Interface.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details    Manages the User and Special indicator states
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_INDICATOR_INTERFACE_H)
#define DEM_INDICATOR_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Indicator_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_GlobalState_Indicator_PublicProperties Public Properties
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalState_Indicator_GetGlobalMilIndicatorId
 *****************************************************************************/
/*!
 * \brief         Get the unique handle of indicator 'Malfunction Indicator Lamp'
 *
 * \details       Get the unique handle of indicator 'Malfunction Indicator Lamp'
 *
 * \return        Unique handle of the indicator.
 *                DEM_CFG_GLOBAL_INDICATOR_COUNT if no such indicator is configured
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalState_Indicator_GetGlobalMilIndicatorId(
  void
  );

/* ****************************************************************************
 * Dem_GlobalState_Indicator_GetGlobalPlIndicatorId
 *****************************************************************************/
/*!
 * \brief         Get the unique handle of indicator 'Protection Lamp'
 *
 * \details       Get the unique handle of indicator 'Protection Lamp'
 *
 * \return        Unique handle of the indicator.
 *                DEM_CFG_GLOBAL_INDICATOR_COUNT if no such indicator is configured
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalState_Indicator_GetGlobalPlIndicatorId(
  void
  );

/* ****************************************************************************
 * Dem_GlobalState_Indicator_GetGlobalAwlIndicatorId
 *****************************************************************************/
/*!
 * \brief         Get the unique handle of indicator 'Amber Warning Lamp'
 *
 * \details       Get the unique handle of indicator 'Amber Warning Lamp'
 *
 * \return        Unique handle of the indicator.
 *                DEM_CFG_GLOBAL_INDICATOR_COUNT if no such indicator is configured
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalState_Indicator_GetGlobalAwlIndicatorId(
  void
  );


/* ****************************************************************************
 * Dem_GlobalState_Indicator_GetGlobalRslIndicatorId
 *****************************************************************************/
/*!
 * \brief         Get the unique handle of indicator 'Red Stop Lamp'
 *
 * \details       Get the unique handle of indicator 'Red Stop Lamp'
 *
 * \return        Unique handle of the indicator.
 *                DEM_CFG_GLOBAL_INDICATOR_COUNT if no such indicator is configured
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalState_Indicator_GetGlobalRslIndicatorId(
  void
  );

/* ****************************************************************************
 * Dem_GlobalState_Indicator_EventIndicatorIterGetIndex
 *****************************************************************************/
/*!
 * \brief         Get the event counter index from event indicator iterator
 *
 * \details       Get the index in IndicatorTable, which is the same as in 
 *                IndicatorEventCounters table, the iterator is pointing to
 *
 * \param[in]     IterPtr
 *                
 * \param[out]    IndicatorIndex
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_Cfg_IndicatorIndexType, DEM_CODE)
Dem_GlobalState_Indicator_EventIndicatorIterGetIndex(
  CONSTP2CONST(Dem_Cfg_ComplexIterType, AUTOMATIC, AUTOMATIC)  IterPtr                                              /* PRQA S 3206 */ /* MD_DEM_3206 */
  );

/* ****************************************************************************
 * Dem_GlobalState_Indicator_GetIndicatorIdOfIndicatorTable
 *****************************************************************************/
/*!
 * \brief         Get the indicator ID of an indicator
 *
 * \details       Get the indicator ID for the indicator at given index 
 *
 * \param[in]     Index of indicator in Indicator table
 *
 * \return        Indicator ID
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(uint8, DEM_CODE)
Dem_GlobalState_Indicator_GetIndicatorIdOfIndicatorTable(
  CONST(uint16, AUTOMATIC)  Index
);

/* ****************************************************************************
 * Dem_GlobalState_Indicator_GetBehaviorOfIndicatorTable
 *****************************************************************************/
/*!
 * \brief         Get the behavior of an indicator
 *
 * \details       Get the behavior for the indicator at given index 
 *
 * \param[in]     Index of indicator in Indicator table
 *
 * \return        Indicator behavior
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalState_Indicator_GetBehaviorOfIndicatorTable(
  CONST(uint16, AUTOMATIC)  Index
);

/* ****************************************************************************
 * Dem_GlobalState_Indicator_IsIndicatorIdValid
 *****************************************************************************/
/*!
 * \brief         Test if the Indicator Id is valid
 *
 * \details       Test if the Indicator Id is in range [0,DEM_CFG_GLOBAL_INDICATOR_COUNT[
 *
 * \return        TRUE: Indicator Id is valid
 *                FALSE: Indicator Id is invalid
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalState_Indicator_IsIndicatorIdValid(
  uint8 IndicatorId
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_GlobalState_Indicator_Public Public Methods
 * \{
 */

/* ****************************************************************************
 * Dem_GlobalState_Indicator_ProcessEventWirEnabled
 *****************************************************************************/
/*!
 * \brief         Updates indicator states on an event WIR bit ON trigger
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique identifier of the event
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalState_Indicator_ProcessEventWirEnabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId
  );

/* ****************************************************************************
 * Dem_GlobalState_Indicator_ProcessEventWirDisabled
 *****************************************************************************/
/*!
 * \brief         Updates indicator states on an event WIR bit OFF trigger
 *
 * \details       -
 *
 * \param[in]     EventId
 *                Unique identifier of the event
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL FUNC(void, DEM_CODE)
Dem_GlobalState_Indicator_ProcessEventWirDisabled(
  CONST(Dem_EventIdType, AUTOMATIC)  EventId 
  );

/* ****************************************************************************
 * Dem_GlobalState_Indicator_GetIndicatorState
 *****************************************************************************/
/*!
 * \brief         Gets the indicator state for a given J1939 Node or all Nodes
 *
 * \details       -
 *
 * \param[in]     IndicatorId
 *                Unique handle of the indicator
 *
 * \param[in]     NodeIndex
 *                Index of the J1939 Node
 *
 * \param[in]     NodeIndependent
 *                TRUE: NodeIndex is not evaluated
 *                FALSE: NodeIndex is evaluated
 *
 * \return        The indicator state
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(Dem_IndicatorStatusType, DEM_CODE)
Dem_GlobalState_Indicator_GetIndicatorState(
  CONST(uint8, AUTOMATIC)  IndicatorId,
  CONST(uint8, AUTOMATIC)  NodeIndex,
  CONST(boolean, AUTOMATIC) NodeIndependent
);


/* ****************************************************************************
 * Dem_GlobalState_Indicator_Init
 *****************************************************************************/
/*!
 * \brief         Initialize indicator state based on event state.
 *
 * \details       If indicators are configured, initialize all indicator state
 *                counters based on the status of all events available in this
 *                variant.
 *
 *
 * \pre           -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_GlobalState_Indicator_Init(void);

/* ****************************************************************************
 * Dem_GlobalState_Indicator_TestMilReportedActive
 *****************************************************************************/
/*!
 * \brief         Returns if the MIL is considered as active.
 *
 * \details       The MIL is always considered as active (for reporting in 
 *                PID 0x01, PID 0x21 and PID 0x4D), if it is continuous (or 
 *                continuous blinking). 
 *                In configurations with 'Report Blinking Mil As Active' 
 *                enabled, the Mil is also considered as active if it is only
 *                blinking or flashing. 
 *
 * \return        TRUE
 *                The MIL is considered as active.
 * \return        FALSE
 *                The MIL is not considered as active
 *
 * \pre           -
 * \config        -
 *
 * \context       TASK|ISR2
 * \synchronous   TRUE
 * \reentrant     TRUE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(boolean, DEM_CODE)
Dem_GlobalState_Indicator_TestMilReportedActive(
  void
  );


/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_INDICATOR_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Indicator_Interface.h
 *********************************************************************************************************************/
