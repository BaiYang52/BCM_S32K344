/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2019 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  vItaHlp_Mcu.c
 *        \brief  vItaHlp functionalities concerning MCU.
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

#define VITAHLP_MCU_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vItaHlp.h"

#if ((VITAHLP_MCU == STD_ON) && (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON))
# include "vstdlib.h"
# include "vItaHlp_Mcu.h"
# include "Mcu.h"  /* Include Mcu.h here, as no function pointer array is used in vItaHlp_Mcu_Cfg.c */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Definitions of API function IDs and module ID from AUTOSAR specification of OS component are located in vItaHlp.h */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/ 
# if !defined (VITAHLP_LOCAL)
#  define VITAHLP_LOCAL static
# endif

/* Service Identifiers for MCU test module commands */
typedef enum
{
  VITAHLP_MCU_CMD_PERFORM_RESET = 0u,
  VITAHLP_MCU_CMD_GET_RESET_REASON = 1u,
  VITAHLP_MCU_CMD_SET_REL_ALARM = 2u,
  VITAHLP_MCU_CMD_CANCEL_ALARM = 3u,
  VITAHLP_MCU_CMD_GET_TASK_ACTIVATION_COUNTER = 4u,
  VITAHLP_MCU_CMD_SERVICEIDENTIFIER_UNKNOWN = 0xFFu
} vItaHlp_Mcu_TestCommunicationCmdServiceIdentifierType;

/* Service Identifiers for MCU test module events */
typedef enum
{
  VITAHLP_MCU_EVENT_GET_RESET_REASON_VALUE = 0u,
  VITAHLP_MCU_EVENT_GET_TASK_ACT_COUNTER_VALUE = 1u
} vItaHlp_Mcu_TestCommunicationEventServiceIdentifierType;

/* Reset Indicator values */
typedef enum
{
  VITAHLP_MCU_EVENT_RESET_INDICATOR_INITIAL = 0u,
  VITAHLP_MCU_EVENT_RESET_INDICATOR_MCU_RESET_TRIGGERED = 1u
} vItaHlp_Mcu_ResetIndicatorType;

 /**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
# define VITAHLP_START_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VAR(vItaHlp_Mcu_ResetIndicatorType, VITAHLP_VAR_ZERO_INIT) vItaHlp_Mcu_ResetIndicator;
VAR(uint8, VITAHLP_VAR_ZERO_INIT) vItaHlp_Mcu_TaskActivationCounter;

# define VITAHLP_STOP_SEC_VAR_ZERO_INIT_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

 /**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  vItaHlp_Mcu_GetTaskActivationCounterValue()
 *********************************************************************************************************************/
/*! \brief       Reads the variable TaskAktivationCounterValue and sends the data back to the tester via an Event.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Mcu_GetTaskActivationCounterValue(void);

/**********************************************************************************************************************
 *  vItaHlp_Mcu_PerformReset()
 *********************************************************************************************************************/
/*! \brief       Triggers the MCU service Mcu_PerformReset.
 *  \details     Sets the Reset Indicator variable to value VITAHLP_MCU_EVENT_RESET_INDICATOR_MCU_RESET_TRIGGERED
 *               and calls the Mcu_PerformReset API.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Mcu_PerformReset(void);

/**********************************************************************************************************************
 *  vItaHlp_Mcu_GetResetReason()
 *********************************************************************************************************************/
/*! \brief       Retrieve the reason for the reset from DUT
 *  \details     Read the MCU reset reason by call of Mcu_GetResetReason API function and send the corresponding data
 *               back to the tester.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Mcu_GetResetReason(void);

/**********************************************************************************************************************
 *  vItaHlp_Mcu_SetRelAlarm()
 *********************************************************************************************************************/
/*! \brief       Triggers the OS service function SetRelAlarm to activate the MCU test task.
 *  \details     The Cycle parameter is set to trigger the alarm every 10ms.
 *               The Increment parameter is set to 1 tick.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Mcu_SetRelAlarm(void);

/**********************************************************************************************************************
 *  vItaHlp_Mcu_CancelAlarm()
 *********************************************************************************************************************/
/*! \brief       Triggers the OS service function CancelAlarm.
 *  \details     The OS API CancelAlarm is triggered. The corresponding alarm action (activate the MCU test task)
 *               is therefore not triggered again.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Mcu_CancelAlarm(void);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_Mcu_PerformReset()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Mcu_PerformReset(void)
{
  /* Set the Reset Indicator variable */
  vItaHlp_Mcu_ResetIndicator = VITAHLP_MCU_EVENT_RESET_INDICATOR_MCU_RESET_TRIGGERED;

  /* Call the MCU API to perform the Reset */
  Mcu_PerformReset();
}
/**********************************************************************************************************************
 *  vItaHlp_Mcu_GetResetReason()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Mcu_GetResetReason(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];
  Mcu_ResetType resetReason;
  
  /* Call the MCU API to perform the Reset */
  resetReason = Mcu_GetResetReason();

  /* Build the response event */
  payloadBuffer[0u] = (uint8)resetReason;
  /* Sent the Reset Indicator value */
  payloadBuffer[1u] = (uint8)vItaHlp_Mcu_ResetIndicator;
  
  /* Forward to EventHandler */
  vItaHlp_SetEvent(
    VITAHLP_COMPONENTIDENTIFIER_MCU,
    VITAHLP_MCU_EVENT_GET_RESET_REASON_VALUE,
    (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadBuffer);
}

/**********************************************************************************************************************
 *  vItaHlp_Mcu_SetRelAlarm()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Mcu_SetRelAlarm(void)
{
# if defined (VITAHLP_MCU_ALARM_CONFIGURED)
  /* ----- Local Variables ---------------------------------------------- */
  StatusType alarmStatus;

  /* ----- Implementation ----------------------------------------------- */
  vItaHlp_Mcu_TaskActivationCounter = 0u;

  /* Trigger alarm cyclically each 10ms */
  alarmStatus = SetRelAlarm(vItaHlp_Mcu_TestAlarm, 1u, OS_MS2TICKS_SystemTimer(10u));
  /* Send module event error in case the SetRelAlarm function results in a problem */
  if (alarmStatus != OS_STATUS_OK)
  {
    vItaHlp_EventModuleUnderTestError(
      VITAHLP_OS_MODULE_ID,
      0u,
      VITAHLP_OS_SID_SETRELALARM_ID,
      alarmStatus);
  }
# else
  /* Send general error event if MCU SetRelAlarm Cmd is called even if the alarm is not configured */
  vItaHlp_EventVItaHlpError(
    VITAHLP_COMPONENTIDENTIFIER_MCU,
    VITAHLP_MCU_CMD_SET_REL_ALARM,
    VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
    VITAHLP_E_INVALID_CONFIGURATION,
    0u);
# endif
}

/**********************************************************************************************************************
 *  vItaHlp_Mcu_CancelAlarm()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Mcu_CancelAlarm(void)
{
# if defined (VITAHLP_MCU_ALARM_CONFIGURED)
  /* ----- Local Variables ---------------------------------------------- */
  StatusType alarmStatus;

  /* ----- Implementation ----------------------------------------------- */
  alarmStatus = CancelAlarm(vItaHlp_Mcu_TestAlarm);
  /* Send module event error in case the CancelAlarm function results in a problem */
  if (alarmStatus != OS_STATUS_OK)
  {
    vItaHlp_EventModuleUnderTestError(
      VITAHLP_OS_MODULE_ID,
      0u,
      VITAHLP_OS_SID_CANCELALARM_ID,
      alarmStatus);
  }
# else
  /* Send general error event if MCU SetRelAlarm Cmd is called even if the alarm is not configured */
  vItaHlp_EventVItaHlpError(
    VITAHLP_COMPONENTIDENTIFIER_MCU,
    VITAHLP_MCU_CMD_CANCEL_ALARM,
    VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
    VITAHLP_E_INVALID_CONFIGURATION,
    0u);
# endif
}

/**********************************************************************************************************************
 *  vItaHlp_Mcu_GetTaskActivationCounterValue()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Mcu_GetTaskActivationCounterValue(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* Build the response event */
  payloadBuffer[0u] = vItaHlp_Mcu_TaskActivationCounter;

  /* Forward to EventHandler */
  vItaHlp_SetEvent(
    VITAHLP_COMPONENTIDENTIFIER_MCU,
    VITAHLP_MCU_EVENT_GET_TASK_ACT_COUNTER_VALUE,
    (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadBuffer);
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_Mcu_IncrementTaskActivationCounter()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Mcu_IncrementTaskActivationCounter(void)
{
  /* Increment the Task activation counter */
  vItaHlp_Mcu_TaskActivationCounter++;
}

/**********************************************************************************************************************
 *  vItaHlp_Mcu_DispatchService()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Mcu_DispatchService(VAR(uint8, AUTOMATIC) serviceIdentifier)
{
  switch (serviceIdentifier)
  {
    case VITAHLP_MCU_CMD_PERFORM_RESET:
         vItaHlp_Mcu_PerformReset();
         break;
    case VITAHLP_MCU_CMD_GET_RESET_REASON:
         vItaHlp_Mcu_GetResetReason();
         break;
    case VITAHLP_MCU_CMD_SET_REL_ALARM:
         vItaHlp_Mcu_SetRelAlarm();
         break;
    case VITAHLP_MCU_CMD_CANCEL_ALARM:
         vItaHlp_Mcu_CancelAlarm();
         break;
    case VITAHLP_MCU_CMD_GET_TASK_ACTIVATION_COUNTER:
         vItaHlp_Mcu_GetTaskActivationCounterValue();
         break;
    default:
         vItaHlp_EventVItaHlpError(
           VITAHLP_COMPONENTIDENTIFIER_MCU,
           serviceIdentifier,
           VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
           VITAHLP_E_UNKNOWN_CMD,
           VITAHLP_TESTCOMMUNICATION_UNKNOWN_SERVICEIDENTIFIER);
         break;
  }
}
# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ((VITAHLP_MCU == STD_ON) && (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)) */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_Mcu.c
 *********************************************************************************************************************/
