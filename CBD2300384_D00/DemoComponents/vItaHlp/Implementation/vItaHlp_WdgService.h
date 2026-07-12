/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  vItaHlp_WdgService.h
 *        \brief  Service API declaration for WDG standard procedure handling.
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

#if !defined (VITAHLP_WDGSERVICE_H)
# define VITAHLP_WDGSERVICE_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vItaHlp_Cfg.h"
# include "vItaHlp_Wdg_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* Default timing values for the watchdog standard procedure (time values in multiples of 10ms) */
# define VITAHLP_WDGSERVICE_STANDARD_CYCLE_TIME 1000ul
# define VITAHLP_WDGSERVICE_STANDARD_TRIGGER_TIMEOUT 1200ul

/* Maximum values of counters used for the watchdog standard procedure */
# define VITAHLP_WDGSERVICE_MAX_ALIVE_COUNTER 0xFFFFul
# define VITAHLP_WDGSERVICE_MAX_MAIN_CYCLE_COUNTER 10000ul

/* The vItaHlp_MainFunction() cycle time */
# define VITAHLP_WDGSERVICE_MAIN_CYCLE_TIME 10u

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# define vItaHlp_WdgService_CycleTime(driverInstanceID) \
  (vItaHlp_WdgService_DriverSpecificTimingConfig[driverInstanceID].cycleTime)
# define vItaHlp_WdgService_TriggerTimeout(driverInstanceID) \
  (vItaHlp_WdgService_DriverSpecificTimingConfig[driverInstanceID].triggerTimeout)
# define vItaHlp_WdgService_TimeUntilNextWdgDrvServe(driverInstanceID) \
  (vItaHlp_WdgService_DriverSpecificTimingConfig[driverInstanceID].timeUntilNextWdgDrvServe)

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
typedef struct svItaHlp_WdgService_DriverSpecificTimingConfigType
{
  uint16 cycleTime;
  uint16 triggerTimeout;
  sint16 timeUntilNextWdgDrvServe;
} vItaHlp_WdgService_DriverSpecificTimingConfigType;

typedef enum
{
  VITAHLP_WDG_ALIVE_EVENT_DISABLED = 0u,
  VITAHLP_WDG_ALIVE_EVENT_ENABLED  = 1u
} vItaHlp_WdgAliveEventStatusType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  vItaHlp_WdgService_Init()
 *********************************************************************************************************************/
/*! \brief         Inits the driver specific timing config for each configured watchdog driver
 *  \details       -
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        -
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_WdgService_Init(void);

/**********************************************************************************************************************
 *  vItaHlp_WdgService_MainFunction()
 *********************************************************************************************************************/
/*! \brief         Main function of the vItaHlp WDG service
 *  \details       Serves the WDG driver in configured periods, therefore the function calls the
 *                 vItaHlp_WdgService_ServeWdgStandardProcedure function
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_WdgService_MainFunction(void);

/**********************************************************************************************************************
 *  vItaHlp_WdgService_SetAliveEventStatus()
 *********************************************************************************************************************/
/*! \brief         Sets the flag for WDG alive event reporting
 *  \details       -
 *  \param[in]     statusNew          New status which is set for the WDG alive event reporting flag.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_WdgService_SetAliveEventStatus(
  VAR(vItaHlp_WdgAliveEventStatusType, AUTOMATIC) statusNew);

#if (VITAHLP_WDG == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_WdgService_SetTriggerConditionTimeout()
 *********************************************************************************************************************/
/*! \brief         Stores the triggerTimeout value
 *  \details       The triggerTimeout value is given to the WDG driver in the periodic Wdg_SetTriggerCondition call
 *  \param[in]     driverInstanceID   ID of the driver for which the timeout value shall be applied
 *  \param[in]     triggerTimeout     timeout value used for cyclic call of Wdg_SetTriggerCondition
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_WdgService_SetTriggerConditionTimeout(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  VAR(uint16, AUTOMATIC) triggerTimeout);

/**********************************************************************************************************************
 *  vItaHlp_WdgService_SetCycleTime()
 *********************************************************************************************************************/
/*! \brief         Sets the periodic cycle time in which the specified WDG driver should be served
 *  \details       -
 *  \param[in]     driverInstanceID   ID of the driver for which the cycleTime value shall be applied
 *  \param[in]     cycleTime          Time period in which the Wdg_SetTriggerCondition() function shall be called for
 *                                    the specified WDG driver
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_WdgService_SetCycleTime(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  VAR(uint16, AUTOMATIC) cycleTime);

#endif /* (VITAHLP_WDG == STD_ON) */

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h"

#endif /* VITAHLP_WDGSERVICE_H */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_WdgService.h
 *********************************************************************************************************************/
