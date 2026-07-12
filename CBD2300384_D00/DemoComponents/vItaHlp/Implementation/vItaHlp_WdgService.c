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
/**        \file  vItaHlp_WdgService.c
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

#define VITAHLP_WDGSERVICE_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vItaHlp.h"

#if (VITAHLP_WDGSERVICE == STD_ON)
# include "vItaHlp_CommonUtils.h"
# include "vstdlib.h"
# include "vItaHlp_WdgService.h"
# if (VITAHLP_WDG == STD_ON)
#  include "vItaHlp_Wdg.h"
# endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

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
 *  LOCAL DATA
 *********************************************************************************************************************/
# define VITAHLP_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
VAR(vItaHlp_WdgService_DriverSpecificTimingConfigType, VITAHLP_VAR) \
    vItaHlp_WdgService_DriverSpecificTimingConfig[vItaHlp_Wdg_GetNumberOfDrivers()];
/* Flag contains information, whether the WDG alive event shall be sent or not.
   No exlusive area protection required for this parameter, as the parameter is only set during execution of dedicated
   vItaHlp scenarios, just after reception of the related command message.
   The default status after ECU reset is VITAHLP_WDG_ALIVE_EVENT_ENABLED
 */
VAR(vItaHlp_WdgAliveEventStatusType, VITAHLP_VAR_NOINIT) vItaHlp_WdgAliveEventStatus;
# define VITAHLP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define VITAHLP_START_SEC_VAR_NOINIT_16BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (VITAHLP_WDG == STD_ON)
VAR(uint16, VITAHLP_VAR) vItaHlp_WdgService_aliveCounter;
# endif
VAR(uint16, VITAHLP_VAR) vItaHlp_WdgService_mainCycleCounter;
# define VITAHLP_STOP_SEC_VAR_NOINIT_16BIT
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
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_WdgService_Init()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_WdgService_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least driverInstanceID;

  /* ----- Implementation ----------------------------------------------- */
  vItaHlp_WdgAliveEventStatus = VITAHLP_WDG_ALIVE_EVENT_ENABLED;
  vItaHlp_WdgService_mainCycleCounter = 0;
# if (VITAHLP_WDG == STD_ON)
  vItaHlp_WdgService_aliveCounter = 0;
#  if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
  vItaHlp_Wdg_SetEventAliveCounter(vItaHlp_WdgService_aliveCounter);
#  endif
# endif
  /* Initialize the standard cylceTime and triggerTimeout values for all configured WDG drivers */
  for (driverInstanceID = 0; driverInstanceID < vItaHlp_Wdg_GetNumberOfDrivers(); driverInstanceID++)
  {
    vItaHlp_WdgService_CycleTime(driverInstanceID) = (uint16)VITAHLP_WDGSERVICE_STANDARD_CYCLE_TIME;
    vItaHlp_WdgService_TriggerTimeout(driverInstanceID) = (uint16)(VITAHLP_WDGSERVICE_STANDARD_TRIGGER_TIMEOUT);
    vItaHlp_WdgService_TimeUntilNextWdgDrvServe(driverInstanceID) =
      vItaHlp_WdgService_CycleTime(driverInstanceID) / VITAHLP_WDGSERVICE_MAIN_CYCLE_TIME;
  }
}

/**********************************************************************************************************************
 *  vItaHlp_WdgService_MainFunction()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_WdgService_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least driverInstanceID;

  /* ----- Implementation ----------------------------------------------- */
  vItaHlp_WdgService_mainCycleCounter++;

  for (driverInstanceID = 0u; driverInstanceID < vItaHlp_Wdg_GetNumberOfDrivers(); driverInstanceID++)
  {
    if ((vItaHlp_WdgService_mainCycleCounter % vItaHlp_WdgService_TimeUntilNextWdgDrvServe(driverInstanceID)) == 0u)
    {
# if ((VITAHLP_WDG == STD_ON) && (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON))
      if (vItaHlp_WdgService_aliveCounter < VITAHLP_WDGSERVICE_MAX_ALIVE_COUNTER)
      {
        vItaHlp_WdgService_aliveCounter++;
      }
      if (vItaHlp_WdgAliveEventStatus == VITAHLP_WDG_ALIVE_EVENT_ENABLED)
      {
        vItaHlp_Wdg_SetEventAliveCounter(vItaHlp_WdgService_aliveCounter);
      }
# endif
      vItaHlp_WdgService_ServeWdgStandardProcedure(driverInstanceID);
    }
  }

  /* Reset the counter every 100s to avoid a variable overflow */
  if (vItaHlp_WdgService_mainCycleCounter == VITAHLP_WDGSERVICE_MAX_MAIN_CYCLE_COUNTER)
  {
    vItaHlp_WdgService_mainCycleCounter = 0u;
  }
}

/**********************************************************************************************************************
 *  vItaHlp_WdgService_SetAliveEventStatus()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_WdgService_SetAliveEventStatus(
  VAR(vItaHlp_WdgAliveEventStatusType, AUTOMATIC) statusNew)
{
  vItaHlp_WdgAliveEventStatus = statusNew;
}

# if (VITAHLP_WDG == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_WdgService_SetTriggerConditionTimeout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_WdgService_SetTriggerConditionTimeout(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  VAR(uint16, AUTOMATIC) triggerTimeout)
{
  vItaHlp_WdgService_TriggerTimeout(driverInstanceID) = triggerTimeout;
}

/**********************************************************************************************************************
 *  vItaHlp_WdgService_SetCycleTime()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_WdgService_SetCycleTime(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  VAR(uint16, AUTOMATIC) cycleTime)
{
  vItaHlp_WdgService_CycleTime(driverInstanceID) = cycleTime;
  vItaHlp_WdgService_TimeUntilNextWdgDrvServe(driverInstanceID) = cycleTime / VITAHLP_WDGSERVICE_MAIN_CYCLE_TIME;
}
# endif /* (VITAHLP_WDG == STD_ON) */

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VITAHLP_WDGSERVICE == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_WdgService.c
 *********************************************************************************************************************/
