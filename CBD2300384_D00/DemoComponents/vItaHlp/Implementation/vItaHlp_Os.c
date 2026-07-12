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
/**        \file  vItaHlp_Os.c
 *        \brief  vItaHlp functionalities concerning OS.
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

#define VITAHLP_OS_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vItaHlp.h"

#if ((VITAHLP_OS == STD_ON) && (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON))
# include "vstdlib.h"
# include "vItaHlp_Os.h"
# include "vItaHlp_CommonUtils.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Definitions of API function IDs and module ID from AUTOSAR specification of OS component are located in vItaHlp.h */

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

/* OS Test Communication Message Layout */
# define VITAHLP_OS_TESTCOMMUNICATION_MESSAGE_PAYLOAD_SIZE          (5u)

typedef enum
{
  VITAHLP_OS_TESTCOMMUNICATION_MESSAGE_POS_PAYLOAD_CORE_ID = 0u,
  VITAHLP_OS_TESTCOMMUNICATION_MESSAGE_POS_PAYLOAD_BEGIN_OTHER = 1u
} vItaHlp_Os_TestCommunicationMessageLayoutType;

/* Service Identifiers for OS test module commands */
typedef enum
{
  VITAHLP_OS_CMD_SETRELALARM = 0u,
  VITAHLP_OS_CMD_GETCOREID = 1u,
  VITAHLP_OS_CMD_SERVICEIDENTIFIER_UNKNOWN = 0xFFu
} vItaHlp_Os_TestCommunicationCmdServiceIdentifierType;

/* Service Identifiers for OS test module events */
typedef enum
{
  VITAHLP_OS_EVENT_TASKACTIVATION = 0u,
  VITAHLP_OS_EVENT_GETCOREID_RESULT = 1u
} vItaHlp_Os_TestCommunicationEventServiceIdentifierType;

/* The parameters of a OS test module command */
typedef struct svItaHlp_Os_TestCommunicationCmdDataType
{
  uint8 serviceIdentifier;
  uint8 payload[VITAHLP_OS_TESTCOMMUNICATION_MESSAGE_PAYLOAD_SIZE];
} vItaHlp_Os_TestCommunicationCmdDataType;

/* For exchange of received commands between different cores */
# define VITAHLP_START_SEC_GLOBALSHARED_VAR_UNSPECIFIED
# include "MemMap.h"
VAR(vItaHlp_Os_TestCommunicationCmdDataType, VITAHLP_VAR_NOINIT_UNSPECIFIED)
  osCoreSpecificCmd[vItaHlp_Os_GetNumberOfCoresUnderTest()];
# define VITAHLP_STOP_SEC_GLOBALSHARED_VAR_UNSPECIFIED
# include "MemMap.h"

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  vItaHlp_Os_CopyTestCommunicationCmdToSharedMemory()
 *********************************************************************************************************************/
/*! \brief       Copies the content of a test communication command to a shared memory.
 *  \details     A test communication command for the OS test module is addressed to one of the cores of the
 *               HW platform. Therefore the relevant data of a command is copied to a shared memory, which can be
 *               accessed from each core.
 *
 *  \param[in]   serviceIdentifier  Identifier of the requested service.
 *  \param[in]   logicalCoreId      The core identifier related to the data to be stored.
 *  \param[in]   payloadData        Pointer to the payloadData. The payload data comprises the 5 bytes data
 *                                  of the OS test module command request.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Os_CopyTestCommunicationCmdToSharedMemory(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  VAR(vItaHlp_Os_CoreIdType, AUTOMATIC) logicalCoreId,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Os_GetTestCommunicationCmdFromSharedMemory()
 *********************************************************************************************************************/
/*! \brief       Read the content of a test communication command from the shared memory.
 *  \details     Depending on the core identifier, the corresponding entry of the test command is read from the
 *               shared memory and is returned to the calling function.
 *
 *  \param[in]   logicalCoreId  The core identifier for which the data shall be read.
 *  \param[out]  osSpecificCmd  The pointer to the command structure.
 *  \return      TRUE           If a command could be read from shared memory for the core on which this function
 *                              is called.
 *               FALSE          If no appropriate entry is found in the shared memory array.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Os_GetTestCommunicationCmdFromSharedMemory(
  VAR(vItaHlp_Os_CoreIdType, AUTOMATIC) logicalCoreId,
  P2VAR(vItaHlp_Os_TestCommunicationCmdDataType, AUTOMATIC, VITAHLP_APPL_VAR) osSpecificCmd);

/**********************************************************************************************************************
 *  vItaHlp_Os_GetTestCommunicationTaskId()
 *********************************************************************************************************************/
/*! \brief       Gets the test communication task identifier.
 *  \details     Gets the test communication task identifier from the internal vItaHlp_Os_CoreSpecificData map,
 *               depending on the logical core identifier.
 *               If no valid entry exists, the returned test communication task identifier is invalid.
 *
 *  \param[in]   logicalCoreId            The core identifier for which the test communication task identifier is
 *                                        searched.
 *  \param[out]  testCommunicationTaskId  The pointer to the test communication task identifier.
 *  \return      TRUE                     If a valid test communication task identifier was found.
 *               FALSE                    The given input cannot be found in internal configuration array.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Os_GetTestCommunicationTaskId(
  VAR(vItaHlp_Os_CoreIdType, AUTOMATIC) logicalCoreId,
  P2VAR(vItaHlp_Os_TaskType, AUTOMATIC, VITAHLP_APPL_VAR) testCommunicationTaskId);

/**********************************************************************************************************************
 *  vItaHlp_Os_GetIndexCoreSpecificData()
 *********************************************************************************************************************/
/*! \brief       Gets the index within the internal config array for a given core identifier.
 *  \details     Verifies if the given OS core instance is given in the internal configuration array and returns the
 *               corresponding index.
 *               If no valid entry is found, the returned index is invalid.
 *
 *  \param[in]   logicalCoreId  The core identifier for which the index is searched.
 *  \param[out]  index          Pointer to the internal configuration array index which contains the entry for the
 *                              given OS core identifier.
 *  \return      TRUE           If a valid test communication task identifier was found.
 *               FALSE          The given input cannot be found in internal configuration array.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Os_GetIndexCoreSpecificData(
  VAR(vItaHlp_Os_CoreIdType, AUTOMATIC) logicalCoreId,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) index);

# if defined (VITAHLP_OS_TASK_CONFIGURED)
/**********************************************************************************************************************
 *  vItaHlp_Os_GetIndexTaskIdMap()
 *********************************************************************************************************************/
/*! \brief       Gets the index within the internal config array for a given task identifier.
 *  \details     Verifies if the given OS task identifier is given in the internal configuration array and returns the
 *               corresponding index.
 *               If no valid entry is found, the returned index is invalid.
 *
 *  \param[in]   taskId  The task identifier for which the index is searched.
 *  \param[out]  index   Pointer to the internal configuration array index which contains the entry for the
 *                       given OS task identifier.
 *  \return      TRUE    If a valid task identifier was found.
 *               FALSE   The given input cannot be found in internal configuration array.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Os_GetIndexTaskIdMap(
  VAR(vItaHlp_Os_TaskType, AUTOMATIC) taskId,
  P2VAR(uint16, AUTOMATIC, VITAHLP_APPL_VAR) index);
# endif 

/**********************************************************************************************************************
 *  vItaHlp_Os_GetCoreId()
 *********************************************************************************************************************/
/*! \brief       Triggers a vItaHlp event message, which contains the core identifier.
 *  \details     Reads the unique core identifier by call of the OS GetCoreID() function. The retrieved core identifier
 *               is mapped into the corresponding event message. The event is forwarded to the event queue handler.
 *
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Os_GetCoreId(void);

/**********************************************************************************************************************
 *  vItaHlp_Os_SetRelAlarm()
 *********************************************************************************************************************/
/*! \brief       Triggers the Os service function SetRelAlarm.
 *  \details     The alarm identifier which is associated with the counter under test is derived from the payload
 *               data information. The Cycle parameter is set to zero, means the alarm is triggered once.
 *               The Increment parameter is set to tick value for one millisecond. 
 *  \param[in]   payloadData  Pointer to the payloadData, length is ensured by caller to be 5.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Os_SetRelAlarm(P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_Os_CopyTestCommunicationCmdToSharedMemory()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Os_CopyTestCommunicationCmdToSharedMemory(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  VAR(vItaHlp_Os_CoreIdType, AUTOMATIC) logicalCoreId,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 index;
  boolean validIndex;

  /* ----- Implementation ----------------------------------------------- */
  /* Copy the command to the array in shared memory.
     The key (index) of this array list is the same
     as used for the internal configuration array 'vItaHlp_Os_CoreSpecificData'. */
  validIndex = vItaHlp_Os_GetIndexCoreSpecificData(logicalCoreId, &index);
  if (validIndex == TRUE)
  {
    osCoreSpecificCmd[index].serviceIdentifier = serviceIdentifier;
    VStdLib_MemCpy(&osCoreSpecificCmd[index].payload, payloadData, VITAHLP_OS_TESTCOMMUNICATION_MESSAGE_PAYLOAD_SIZE);
  }
  else
  {
    /* Send general error event in case the referenced logicalCoreId is not contained in the internal map.
       This can only happen in case of a invalid generated code. */
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_OS,
      serviceIdentifier,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_INVALID_CONFIGURATION,
      0u);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Os_GetTestCommunicationCmdFromSharedMemory()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Os_GetTestCommunicationCmdFromSharedMemory(
  VAR(vItaHlp_Os_CoreIdType, AUTOMATIC) logicalCoreId,
  P2VAR(vItaHlp_Os_TestCommunicationCmdDataType, AUTOMATIC, VITAHLP_APPL_VAR) osSpecificCmd)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 index;
  boolean validIndex;

  /* ----- Implementation ----------------------------------------------- */
  validIndex = vItaHlp_Os_GetIndexCoreSpecificData(logicalCoreId, &index);
  if (validIndex == TRUE)
  {
    osSpecificCmd[0u] = osCoreSpecificCmd[index];
  }
  return validIndex;
}

/**********************************************************************************************************************
 *  vItaHlp_Os_GetTestCommunicationTaskId()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Os_GetTestCommunicationTaskId(
  VAR(vItaHlp_Os_CoreIdType, AUTOMATIC) logicalCoreId,
  P2VAR(vItaHlp_Os_TaskType, AUTOMATIC, VITAHLP_APPL_VAR) testCommunicationTaskId)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean taskIdFound = FALSE;
  uint8_least coreIdx;

  /* ----- Implementation ----------------------------------------------- */
  for (coreIdx = 0u; coreIdx < vItaHlp_Os_GetNumberOfCoresUnderTest(); coreIdx++)
  {
    if (vItaHlp_Os_CoreSpecificData[coreIdx].osLogicalCoreId == logicalCoreId)
    {
      testCommunicationTaskId[0u] = vItaHlp_Os_CoreSpecificData[coreIdx].osTestCommunicationTaskId;
      taskIdFound = TRUE;
      break;
    }
  }
  return taskIdFound;
}

/**********************************************************************************************************************
 *  vItaHlp_Os_GetIndexCoreSpecificData()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Os_GetIndexCoreSpecificData(
  VAR(vItaHlp_Os_CoreIdType, AUTOMATIC) logicalCoreId,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) index)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8_least coreIdx;
  boolean coreIdFound = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  for (coreIdx = 0u; coreIdx < vItaHlp_Os_GetNumberOfCoresUnderTest(); coreIdx++)
  {
    if (vItaHlp_Os_CoreSpecificData[coreIdx].osLogicalCoreId == logicalCoreId)
    {
      index[0u] = coreIdx;
      coreIdFound = TRUE;
      break;
    }
  }
  return coreIdFound;
}

# if defined (VITAHLP_OS_TASK_CONFIGURED)
/**********************************************************************************************************************
 *  vItaHlp_Os_GetIndexTaskIdMap()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Os_GetIndexTaskIdMap(
  VAR(vItaHlp_Os_TaskType, AUTOMATIC) taskId,
  P2VAR(uint16, AUTOMATIC, VITAHLP_APPL_VAR) index)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16_least taskIdx;
  boolean taskIndexFound = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  for (taskIdx = 0u; taskIdx < vItaHlp_Os_GetNumberOfTasksUnderTest(); taskIdx++)
  {
    if (vItaHlp_Os_TaskIdMap[taskIdx] == taskId)
    {
      index[0u] = taskIdx;
      taskIndexFound = TRUE;
      break;
    }
  }
  return taskIndexFound;
}
# endif

/**********************************************************************************************************************
 *  vItaHlp_Os_GetCoreId()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Os_GetCoreId(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];
  vItaHlp_Os_CoreIdType coreId;

  /* ----- Implementation ----------------------------------------------- */
  coreId = GetCoreID();

  /* Build the response event */
  payloadBuffer[0u] = (uint8)coreId;

  /* Forward to EventHandler */
  vItaHlp_SetEvent(
    VITAHLP_COMPONENTIDENTIFIER_OS,
    VITAHLP_OS_EVENT_GETCOREID_RESULT,
    (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadBuffer);
}

/**********************************************************************************************************************
 *  vItaHlp_Os_SetRelAlarm()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Os_SetRelAlarm(P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
# if defined (VITAHLP_OS_ALARM_CONFIGURED)
  /* ----- Local Variables ---------------------------------------------- */
  vItaHlp_Os_AlarmType alarmID;
  StatusType osResult;

  /* ----- Implementation ----------------------------------------------- */
  alarmID = (vItaHlp_Os_AlarmType)vItaHlp_CommonUtils_ByteArrayToUint16BigEndian(payloadData);
  if (alarmID < vItaHlp_Os_GetNumberOfAlarmsUnderTest())
  {
    osResult = SetRelAlarm(vItaHlp_Os_AlarmIdMap[alarmID], 1u, 0u);

    /* Send module event error in case the SetRelAlarm function results in a problem */
    if (osResult != OS_STATUS_OK)
    {
      vItaHlp_EventModuleUnderTestError(
        VITAHLP_OS_MODULE_ID,
        0u,
        VITAHLP_OS_SID_SETRELALARM_ID,
        osResult);
    }
  }
  else
  {
# endif
    /* Send general error event in case the referenced alarmID is not contained in the internal map.
    This can only happen in case of a invalid generated code. */
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_OS,
      VITAHLP_OS_CMD_SETRELALARM,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_INVALID_REQUESTED_PARAM,
      0u);
# if defined (VITAHLP_OS_ALARM_CONFIGURED)
  }
# else
    VITAHLP_DUMMY_STATEMENT_CONST(payloadData);
# endif
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  vItaHlp_Os_EventTaskActivation()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Os_EventTaskActivation(VAR(vItaHlp_Os_TaskType, AUTOMATIC) taskID)
{
# if defined (VITAHLP_OS_TASK_CONFIGURED)
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];
  boolean validIndex;
  uint16 taskIdIndex;

  /* ----- Implementation ----------------------------------------------- */
  validIndex = vItaHlp_Os_GetIndexTaskIdMap(taskID, &taskIdIndex);
  if (validIndex == TRUE)
  {
    /* Build the response event payload */
    vItaHlp_CommonUtils_Uint16ToByteArrayBigEndian(payloadBuffer, taskIdIndex);

    /* Forward to EventHandler */
    vItaHlp_SetEvent(
      VITAHLP_COMPONENTIDENTIFIER_OS,
      VITAHLP_OS_EVENT_TASKACTIVATION,
      (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadBuffer);
  }
  else
  {
# endif
    /* Send general error event in case the referenced taskID is not contained in the internal map.
    This can only happen in case of a invalid generated code. */
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_OS,
      VITAHLP_OS_EVENT_TASKACTIVATION,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_EVENT,
      VITAHLP_E_INVALID_CONFIGURATION,
      0u);
# if defined (VITAHLP_OS_TASK_CONFIGURED)
  }
# else 
    VITAHLP_DUMMY_STATEMENT(taskID);
# endif
}

/**********************************************************************************************************************
 *  vItaHlp_Os_DispatchService()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Os_DispatchService(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  vItaHlp_Os_TaskType osTestCommunicationTaskId;
  vItaHlp_Os_CoreIdType logicalCoreId;
  StatusType osResult;
  boolean validCoreId;

  /* ----- Implementation ----------------------------------------------- */
  /* Verify that the received logical core identifier is valid for the tests */
  logicalCoreId = (vItaHlp_Os_CoreIdType)payloadData[VITAHLP_OS_TESTCOMMUNICATION_MESSAGE_POS_PAYLOAD_CORE_ID];
  validCoreId = vItaHlp_Os_GetTestCommunicationTaskId(logicalCoreId, &osTestCommunicationTaskId);
  if (validCoreId == TRUE)
  {
    /* Write service parameters to the shared memory */
    vItaHlp_Os_CopyTestCommunicationCmdToSharedMemory(
      serviceIdentifier,
      logicalCoreId,
      &payloadData[VITAHLP_OS_TESTCOMMUNICATION_MESSAGE_POS_PAYLOAD_BEGIN_OTHER]);

    /* Activate the core specific dispatcher task */
    osResult = ActivateTask(osTestCommunicationTaskId);
    if (osResult != OS_STATUS_OK)
    {
      vItaHlp_EventModuleUnderTestError(
        VITAHLP_OS_MODULE_ID,
        0u,
        VITAHLP_OS_SID_ACTIVATETASK_ID,
        osResult);
    }
  }
  else
  {
    /* Send general error event in case the referenced logicalCoreId is not contained in the internal map.
       This can only happen in case of a invalid generated code. */
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_OS,
      serviceIdentifier,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_INVALID_CONFIGURATION,
      0u);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Os_Core_DispatchService()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Os_Core_DispatchService(VAR(vItaHlp_Os_CoreIdType, AUTOMATIC) logicalCoreId)
{
  /* ----- Local Variables ---------------------------------------------- */
  vItaHlp_Os_TestCommunicationCmdDataType osSpecificCmd;
  boolean validCommand;

  /* ----- Implementation ----------------------------------------------- */
  /* Read the command service parameters from shared memory for this core */
  validCommand = vItaHlp_Os_GetTestCommunicationCmdFromSharedMemory(logicalCoreId, &osSpecificCmd);

  if (validCommand == TRUE)
  {
    switch (osSpecificCmd.serviceIdentifier)
    {
      case VITAHLP_OS_CMD_SETRELALARM:
        vItaHlp_Os_SetRelAlarm(osSpecificCmd.payload);
        break;
      case VITAHLP_OS_CMD_GETCOREID:
        vItaHlp_Os_GetCoreId();
        break;
      default:
        vItaHlp_EventVItaHlpError(
          VITAHLP_COMPONENTIDENTIFIER_OS,
          osSpecificCmd.serviceIdentifier,
          VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
          VITAHLP_E_UNKNOWN_CMD,
          VITAHLP_TESTCOMMUNICATION_UNKNOWN_SERVICEIDENTIFIER);
        break;
    }
    /* No error event is sent, if the command is invalid at all.
       This should not happen. In addition there is a check, before the command is stored in shared memory. */
  }
}

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ((VITAHLP_OS == STD_ON) && (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)) */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_Os.c
 *********************************************************************************************************************/
