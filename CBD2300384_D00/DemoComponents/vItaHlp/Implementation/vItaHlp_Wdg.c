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
/**        \file  vItaHlp_Wdg.c
 *        \brief  vItaHlp functionalities concerning WDG.
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

#define VITAHLP_WDG_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vItaHlp.h"

#if ((VITAHLP_WDG == STD_ON) && (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON))
# include "vItaHlp_CommonUtils.h"
# include "vstdlib.h"
# include "vItaHlp_Wdg.h"
# include "vItaHlp_WdgService.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Definitions of API function IDs and module ID from AUTOSAR specification of WDG component */
# define VITAHLP_WDG_SID_SET_MODE_ID                        (0x01u)
# define VITAHLP_WDG_MODULE_ID                              (102u)

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

/* Service Identifiers for WDG test module commands */
typedef enum
{
  VITAHLP_WDG_CMD_TRIGGER_SET_TRIGGER_CONDITION = 0u,
  VITAHLP_WDG_CMD_SET_CYCLE_TIME = 1u,
  VITAHLP_WDG_CMD_SET_TRIGGER_CONDITION_TIMEOUT = 2u,
  VITAHLP_WDG_CMD_TRIGGER_SET_MODE = 3u,
  VITAHLP_WDG_CMD_RESTORE_INITAL_STATE = 4u,
  VITAHLP_WDG_CMD_SERVICEIDENTIFIER_UNKNOWN = 0xFFu
} vItaHlp_Wdg_TestCommunicationCmdServiceIdentifierType;

typedef enum
{
  VITAHLP_WDG_TESTCOMMUNICATION_MESSAGE_GENERAL_POS_DRIVER_INSTANCE_ID = 0u,
  VITAHLP_WDG_TESTCOMMUNICATION_MESSAGE_GENERAL_POS_PAYLOAD_BEGIN = 1u
} vItaHlp_Wdg_TestCommunicationMessageGeneralLayoutType;

/* Service Identifiers for WDG test module events */
typedef enum
{
  VITAHLP_WDG_EVENT_ALIVE_COUNTER = 0u,
  VITAHLP_WDG_EVENT_SET_MODE_RESULT = 1u
} vItaHlp_Wdg_TestCommunicationEventServiceIdentifierType;

# define VITAHLP_WDG_COMMAND_RESPONSE_EXPECTED (1u)

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  vItaHlp_Wdg_Trigger_SetTriggerCondition()
 *********************************************************************************************************************/
/*! \brief         Calls the WDG driver API function Wdg_SetTriggerCondition(timeout)
 *  \details       -
 *  \param[in]     driverInstanceID   Instance ID of the WDG driver.
 *  \param[in]     payloadData        Pointer to a subset of the initial payloadData.
 *  \return        -
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Wdg_Trigger_SetTriggerCondition(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Wdg_Trigger_SetMode()
 *********************************************************************************************************************/
/*! \brief         Calls the WDG driver API function Wdg_SetMode(mode)
 *  \details       -
 *  \param[in]     driverInstanceID   Instance ID of the WDG driver.
 *  \param[in]     payloadData        Pointer to a subset of the initial payloadData.
 *  \return        -
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Wdg_Trigger_SetMode(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Wdg_SetTriggerConditionTimeout()
 *********************************************************************************************************************/
/*! \brief         Stores a new triggerTimeout value for the WDG driver
 *  \details       The triggerTimeout value is received within the payloadData. It is set at each periodic call of
 *                 the Wdg_SetTriggerCondition() API call
 *  \param[in]     driverInstanceID   Instance ID of the WDG driver.
 *  \param[in]     payloadData        Pointer to a subset of the initial payloadData.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Wdg_SetTriggerConditionTimeout(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Wdg_SetCycleTime()
 *********************************************************************************************************************/
/*! \brief         Sets the periodic cycle time in which the specified WDG driver is served
 *  \details       The cycleTime value is received within the payloadData. The new value gets effective immediately
 *                 at start of next cycle of watchdog standard procedure
 *  \param[in]     driverInstanceID   Instance ID of the WDG driver.
 *  \param[in]     payloadData        Pointer to a subset of the initial payloadData.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Wdg_SetCycleTime(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Wdg_RestoreInitialState()
 *********************************************************************************************************************/
/*! \brief         Restores the initial state of the specified wdg drv 
 *  \details       -
 *  \param[in]     driverInstanceID    Instance ID of the WDG driver.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *  \config        Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Wdg_RestoreInitialState(
  VAR(uint8, AUTOMATIC) driverInstanceID);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_Wdg_Trigger_SetTriggerCondition()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Wdg_Trigger_SetTriggerCondition(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 triggerTimeout;

  /* ----- Implementation ----------------------------------------------- */
  triggerTimeout = vItaHlp_CommonUtils_ByteArrayToUint16BigEndian(payloadData);
  vItaHlp_Wdg_SetTriggerCondition(driverInstanceID, triggerTimeout);
}

/**********************************************************************************************************************
 *  vItaHlp_Wdg_SetTriggerConditionTimeout()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Wdg_SetTriggerConditionTimeout(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 triggerTimeout;

  /* ----- Implementation ----------------------------------------------- */
  triggerTimeout = vItaHlp_CommonUtils_ByteArrayToUint16BigEndian(payloadData);
  vItaHlp_WdgService_SetTriggerConditionTimeout(driverInstanceID, triggerTimeout);
}

/**********************************************************************************************************************
 *  vItaHlp_Wdg_SetCycleTime()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Wdg_SetCycleTime(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 cycleTime;

  /* ----- Implementation ----------------------------------------------- */
  cycleTime = vItaHlp_CommonUtils_ByteArrayToUint16BigEndian(payloadData);
  vItaHlp_WdgService_SetCycleTime(driverInstanceID, cycleTime);
}

/**********************************************************************************************************************
 *  vItaHlp_Wdg_Trigger_SetMode()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Wdg_Trigger_SetMode(
  VAR(uint8, AUTOMATIC) driverInstanceID,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];
  Std_ReturnType modeReturnValue;
  uint8 mode;

  /* ----- Implementation ----------------------------------------------- */
  mode = payloadData[0u];
  modeReturnValue = vItaHlp_Wdg_SetMode(driverInstanceID, (WdgIf_ModeType) mode);

  if (payloadData[1u] == VITAHLP_WDG_COMMAND_RESPONSE_EXPECTED)
  {
    /* Build the response event payload and notify the tester */
    payloadBuffer[0u] = driverInstanceID;
    payloadBuffer[1u] = (uint8)modeReturnValue;

    vItaHlp_SetEvent(
      VITAHLP_COMPONENTIDENTIFIER_WDG,
      VITAHLP_WDG_EVENT_SET_MODE_RESULT,
      payloadBuffer);
  }
  else
  {
    if (modeReturnValue == E_NOT_OK)
    {
      vItaHlp_EventModuleUnderTestError(
        VITAHLP_WDG_MODULE_ID,
        driverInstanceID,
        VITAHLP_WDG_SID_SET_MODE_ID,
        modeReturnValue);
    }
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Wdg_RestoreInitialState()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Wdg_RestoreInitialState(
  VAR(uint8, AUTOMATIC) driverInstanceID)
{
  vItaHlp_WdgService_SetCycleTime(driverInstanceID, (uint16)VITAHLP_WDGSERVICE_STANDARD_CYCLE_TIME);
  vItaHlp_WdgService_SetTriggerConditionTimeout(driverInstanceID, (uint16)VITAHLP_WDGSERVICE_STANDARD_TRIGGER_TIMEOUT);
  vItaHlp_Wdg_SetMode(driverInstanceID, vItaHlp_Wdg_GetDefaultMode(driverInstanceID));
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_Wdg_SetEventAliveCounter()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Wdg_SetEventAliveCounter(
  VAR(uint16, AUTOMATIC) aliveCounter)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  /* Build the response event payload and notify the tester */
  vItaHlp_CommonUtils_Uint16ToByteArrayBigEndian(payloadBuffer, aliveCounter);

  vItaHlp_SetEvent(
    VITAHLP_COMPONENTIDENTIFIER_WDG,
    VITAHLP_WDG_EVENT_ALIVE_COUNTER,
    payloadBuffer);
}

/**********************************************************************************************************************
 *  vItaHlp_Wdg_DispatchService()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Wdg_DispatchService(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 driverInstanceId;

  /* ----- Implementation ----------------------------------------------- */
  driverInstanceId = payloadData[VITAHLP_WDG_TESTCOMMUNICATION_MESSAGE_GENERAL_POS_DRIVER_INSTANCE_ID];
  switch (serviceIdentifier)
  {
  case VITAHLP_WDG_CMD_TRIGGER_SET_TRIGGER_CONDITION:
    vItaHlp_Wdg_Trigger_SetTriggerCondition(driverInstanceId,
      &payloadData[VITAHLP_WDG_TESTCOMMUNICATION_MESSAGE_GENERAL_POS_PAYLOAD_BEGIN]);
    break;
  case VITAHLP_WDG_CMD_TRIGGER_SET_MODE:
    vItaHlp_Wdg_Trigger_SetMode(driverInstanceId,
      &payloadData[VITAHLP_WDG_TESTCOMMUNICATION_MESSAGE_GENERAL_POS_PAYLOAD_BEGIN]);
    break;
  case VITAHLP_WDG_CMD_SET_TRIGGER_CONDITION_TIMEOUT:
    vItaHlp_Wdg_SetTriggerConditionTimeout(driverInstanceId,
      &payloadData[VITAHLP_WDG_TESTCOMMUNICATION_MESSAGE_GENERAL_POS_PAYLOAD_BEGIN]);
    break;
  case VITAHLP_WDG_CMD_SET_CYCLE_TIME:
    vItaHlp_Wdg_SetCycleTime(driverInstanceId,
      &payloadData[VITAHLP_WDG_TESTCOMMUNICATION_MESSAGE_GENERAL_POS_PAYLOAD_BEGIN]);
    break;
  case VITAHLP_WDG_CMD_RESTORE_INITAL_STATE:
    vItaHlp_Wdg_RestoreInitialState(driverInstanceId);
    break;
  default:
  vItaHlp_EventVItaHlpError(
    VITAHLP_COMPONENTIDENTIFIER_WDG,
    serviceIdentifier,
    VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
    VITAHLP_E_UNKNOWN_CMD,
    VITAHLP_TESTCOMMUNICATION_UNKNOWN_SERVICEIDENTIFIER);
  break;
  }
}

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ((VITAHLP_WDG == STD_ON) && (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)) */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_Wdg.c
 *********************************************************************************************************************/
