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
 *  \defgroup   Dem_Queue Master Queue
 *  \{
 *  \file       Dem_Queue_Interface.h
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

#if !defined (DEM_QUEUE_INTERFACE_H)
#define DEM_QUEUE_INTERFACE_H

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Queue_Types.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \defgroup Dem_Queue_PublicProperties Public Properties
 * \{
 */



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
 * \defgroup Dem_Queue_Private Private Methods
 * \{
 */

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
 * \defgroup Dem_Queue_Public Public Methods
 * \{
 */
/* ****************************************************************************
 * Dem_Timer_Init
 *****************************************************************************/
/*!
 * \brief         Initializes the timer functionality
 *
 * \details       This function initializes all global variables required for
 *                timer handling, e.g. engine run time.
 *
 * \pre           May only be called during module initialization
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Timer_Init(
  void
  );

/* ****************************************************************************
 * Dem_Timer_MainFunction
 *****************************************************************************/
/*!
 * \brief         Processes timer calculations
 *
 * \details       This function processes global timers, e.g. for engine run
 *                times. Must be called cyclically according to the configured
 *                cycle time.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Timer_MainFunction(
  void
  );

                                                                    /* Queue */
/* ------------------------------------------------------------------------- */

/* ****************************************************************************
 * Dem_Queue_Init
 *****************************************************************************/
/*!
 * \brief         Initializes the Dem queue states
 *
 * \details       This function initializes all global variables required to
 *                defer asynchronous requests to be processed on the Dem task
 *                function.
 *
 * \pre           May be called only during initialization
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Queue_Init(
  void
  );

/* ****************************************************************************
 * Dem_Queue_ProcessOdometer
 *****************************************************************************/
/*!
 * \brief         Processes all tasks related to odometer updates
 *
 * \details       This function fetches the current odometer data and updates
 *                odometer related statistic data, e.g. PIDs or the permanent
 *                memory activation.
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Queue_ProcessOdometer(
  void
  );



#if (DEM_FEATURE_NEED_PID4D == STD_ON)
/* ****************************************************************************
 * Dem_Queue_CalculatePid4D
 *****************************************************************************/
/*!
 * \brief         Calculates PID 0x4D
 *
 * \details       Calculates PID 0x4D
 *
 * \param[in]     EngineRunTimeTicks
 *                Current engine run time since engine start
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_PID4D == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Queue_CalculatePid4D(
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
  );
#endif

#if (DEM_FEATURE_NEED_PID4E == STD_ON)
/* ****************************************************************************
 * Dem_Queue_CalculatePid4E
 *****************************************************************************/
/*!
 * \brief         Calculates PID 0x4E
 *
 * \details       Calculates PID 0x4E
 *
 * \param[in]     EngineRunTimeTicks
 *                Current engine run time since engine start
 *
 * \pre           -
 * \config        DEM_FEATURE_NEED_PID4E == STD_ON
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Queue_CalculatePid4E(
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
  );
#endif

/* ****************************************************************************
 * Dem_Queue_ProcessAgingTimer
 *****************************************************************************/
/*!
 * \brief         Calculates the 200h aging timer
 *
 * \details       This function processes aging of WWH-OBD DTCs after 200h of
 *                engine run time if WwhObd is enabled.
 *
 * \param[in]     EngineRunTimeTicks
 *                Current engine run time since engine start
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Queue_ProcessAgingTimer(
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
  );

/* ****************************************************************************
 * Dem_Queue_ProcessEngineRuntime
 *****************************************************************************/
/*!
 * \brief         Processes all tasks related to engine runtime
 *
 * \details       This function processes all engine runtime data related
 *                tasks, e.g. for PID 0x4D, PID 0x4E, DTC aging or B1Counter
 *
 * \pre           -
 *
 * \context       TASK
 * \synchronous   TRUE
 * \reentrant     FALSE
 *****************************************************************************/
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Queue_ProcessEngineRuntime(
  void
  );

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_QUEUE_INTERFACE_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Queue_Interface.h
 *********************************************************************************************************************/
