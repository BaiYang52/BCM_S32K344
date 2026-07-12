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
/**        \file  vItaHlp_Can.c
 *        \brief  vItaHlp functionalities concerning CAN.
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

#define VITAHLP_CAN_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vItaHlp.h"

#if (VITAHLP_CAN == STD_ON)
# include "vItaHlp_CommonUtils.h"
# include "SchM_vItaHlp.h"
# include "vstdlib.h"
# include "vItaHlp_Can.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/* ----------------- Definitions used for the setup test ------------ */
# define VITAHLP_CAN_SETUPTEST_DLC                                (8u)

/* Number of Can_MainFunction calls per second */
# define VITAHLP_CAN_TICKS_PER_SECOND                           (100u)

/* Definitions of API function IDs and module ID from AUTOSAR specification of CAN component */
# define VITAHLP_CAN_SID_SETCTR_ID                          (0x03u)
# define VITAHLP_CAN_SID_WRITE_ID                           (0x06u)
# define VITAHLP_CAN_MODULE_ID                              (80u)

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

/* Service Identifiers for CAN test module commands */
typedef enum
{
  VITAHLP_CAN_CMD_CAN_WRITE   = 0u,
  VITAHLP_CAN_CMD_SLEEP_START = 1u,
  VITAHLP_CAN_CMD_SERVICEIDENTIFIER_UNKNOWN = 0xFFu
} vItaHlp_Can_TestCommunicationCmdServiceIdentifierType;

/* Service Identifiers for CAN test module events */
typedef enum
{
  VITAHLP_CAN_EVENT_RX_INDICATION              = 0u,
  VITAHLP_CAN_EVENT_TX_CONFIRMATION            = 1u,
  VITAHLP_CAN_EVENT_CONTROLLER_MODE_INDICATION = 2u
} vItaHlp_Can_TestCommunicationEventServiceIdentifierType;

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/

# define VITAHLP_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* Contains the driver/controller under test for the current sleep/start sequence
 * Global variable is not initialized, because it is ensured that it is written before it is read.
 *   - Only read if vItaHlp_Can_SleepStartSequenceActive is TRUE, which is initialized as FALSE.
 * Consistent value for variables is ensured by writing variables before setting vItaHlp_Can_SleepStartSequenceActive
 * to TRUE (protected by exclusive area).
 */
VAR(vItaHlp_Can_DriverIndexType, VITAHLP_VAR_NOINIT) vItaHlp_Can_DriverUnderTest;
VAR(vItaHlp_Can_DriverIndexType, VITAHLP_VAR_NOINIT) vItaHlp_Can_DriverUnderTestDataIndex;
VAR(vItaHlp_Can_ControllerIndexType, VITAHLP_VAR_NOINIT) vItaHlp_Can_ControllerUnderTest;

VAR(Can_StateTransitionType, VITAHLP_VAR_NOINIT) vItaHlp_Can_ControllerModeAfterStop;
/* Contains flag if timer for the sleep/start sequence is active
 * Global variable is not protected by exclusive area.
 * Reason:
 *   - Only written in:
 *     - vItaHlp_Can_Init -> deterministic startup-behavior, other accessing functions run later
 *     - vItaHlp_Can_StartTimerForCanStart -> only set once to true if already protected
 *       vItaHlp_Can_SleepStartSequenceActive is TRUE and once occurring mode indication is received
 *       for given controller and driver under test
 *     - vItaHlp_Can_SleepStartSequenceHandleTimer -> only set back to FALSE if previous checked that
 *       state is TRUE. Between read and write, state cannot be changed, because no other function can
 *       set state back to FALSE. vItaHlp_Can_SleepStartSequenceHandleTimer runs in context of a single
 *       task as main-function.
 *  - Only read in:
 *    - vItaHlp_Can_SleepStartSequenceHandleTimer -> see argumentation above
 */
VAR(boolean, VITAHLP_VAR_NOINIT) vItaHlp_Can_SleepStartRestartTimerActive;
/* Flag which contains information if sleep/start sequence is active or not.
 * If the sequence is active, no other sequence can be started until it is finished.
 */
VAR(boolean, VITAHLP_VAR_NOINIT) vItaHlp_Can_SleepStartSequenceActive;
/* Variable which has 2 valid states in order to indicate which of the both patterns for the CAN setup test
 * shall be applied for the next run.
 *           Setup Test CAN message:
 *           Byte0  Byte1  Byte2  Byte3  Byte4  Byte5  Byte6  Byte7
 * FALSE:     0x00   0x00   0x00   0x00   0xFF   0xFF   0xFF   0xFF
 * TRUE:      0xFF   0xFF   0xFF   0xFF   0x00   0x00   0x00   0x00
 */
VAR(boolean, VITAHLP_VAR_NOINIT) vItaHlp_Can_SetupTestPatternIndicationToggleBit;
# define VITAHLP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
# define VITAHLP_START_SEC_VAR_NOINIT_16BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
VAR(uint16, VITAHLP_VAR_NOINIT_16BIT) vItaHlp_Can_NumberTicks;
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
 *  vItaHlp_Can_SetSetupTestSduData()
 *********************************************************************************************************************/
/*! \brief           Function sets the sdu data for the setup test dependent on the previous send data.
 *  \details         Sdu data for the setup test toggles for each run between following two different patterns.
 *                   Pattern 1: 0x00   0x00   0x00   0x00   0xFF   0xFF   0xFF   0xFF
 *                   Pattern 2: 0xFF   0xFF   0xFF   0xFF   0x00   0x00   0x00   0x00
 *                   The global state of the patterns toggles by calling this function.
 *  \param[in,out]   sduData           Data buffer which gets filled by the pattern for the next setup test run.
 *  \pre             -
 *  \context         TASK
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Can_SetSetupTestSduData(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) sduData);

/**********************************************************************************************************************
 *  vItaHlp_Can_GetIndexDriverSpecificData()
 *********************************************************************************************************************/
/*! \brief       Verifies if the given CAN driver instance is given in the internal config struct array and
 *               returns the corresponding index.
 *  \details     -
 *  \param[in]   canDriverInstanceId abstract Instance ID of the CAN driver.
 *  \param[out]  canDriverSpecificDataIndex   Index of the internal config array which contains the entry for the given
 *                                            CAN driver instance ID.
 *  \return      TRUE:   canDriverSpecificDataIndex for given input combination found
 *               FALSE:  given input cannot be found in internal config array
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Can_GetIndexDriverSpecificData(
  VAR(vItaHlp_Can_DriverIndexType, AUTOMATIC) canDriverInstanceId,
  P2VAR(vItaHlp_Can_DriverIndexType, AUTOMATIC, VITAHLP_APPL_VAR) canDriverSpecificDataIndex);

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Can_Write()
 *********************************************************************************************************************/
/*! \brief           Prepare and send a CAN message frame.
 *  \details         The identification of the Can Controller over which the message frame shall be send and the
 *                   message content itself are derived from the payload data information.
 *  \param[in]       payloadData           Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *  \config          Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Can_Write(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Can_Sleep_Start()
 *********************************************************************************************************************/
/*! \brief           Trigger Goto Sleep for CAN Controller.
 *  \details         The identification of the Can Controller over which the message frame shall be send and the
 *                   message content itself are derived from the payload data information.
 *  \param[in]       payloadData           Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *  \config          Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Can_Sleep_Start(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Can_SleepStartSequenceControllerModeIndication()
 *********************************************************************************************************************/
/*! \brief           Sends an event for the given controller mode indication and handles sleep/start sequence
 *                   as defined in design documentation if indication is received for controller/driver under test.
 *  \details         -
 *  \param[in]       canDriverInstanceId       Instance ID of CAN driver for which the mode transition occurred.
 *  \param[in]       canControllerId           CAN Controller ID for which the mode transition occurred.
 *  \param[in]       canControllerMode         CAN Controller mode to which the CAN controller transitioned.
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *  \config          Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Can_SleepStartSequenceControllerModeIndication(
  VAR(uint8, AUTOMATIC) canDriverInstanceId,
  VAR(vItaHlp_Can_ControllerIndexType, AUTOMATIC) canControllerId,
  VAR(uint8, AUTOMATIC) canControllerMode);

/**********************************************************************************************************************
 *  vItaHlp_Can_SetControllerModeForActiveController()
 *********************************************************************************************************************/
/*! \brief           Calls the driver specific SetControllerMode service API of the CAN driver for the
 *                   controller/driver under test of the sleep/start sequence. The controller/driver under test
 *                   retrieved from a global variable which is set by the command to trigger the sleep/start sequence.
 *  \details         -
 *  \pre             Sleep start sequence is active and CANIF_CS_STOPPED mode indication is received for
 *                   controller/driver under test.
 *  \context         TASK|ISR
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *  \config          Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Can_SetControllerModeForActiveController(void);

/**********************************************************************************************************************
 *  vItaHlp_Can_StartTimerForCanStart()
 *********************************************************************************************************************/
/*! \brief           Starts the timer for the sleep/start sequence.
 *  \details         The CAN controller returns from sleep (wakeup) if the timer is finished.
 *  \pre             Sleep start sequence is active and CANIF_CS_SLEEP mode indication is received for
 *                   controller/driver under test.
 *  \context         TASK|ISR
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *  \config          Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL_INLINE FUNC(void, VITAHLP_CODE) vItaHlp_Can_StartTimerForCanStart(void);

/**********************************************************************************************************************
 *  vItaHlp_Can_SleepStartSequenceHandleTimer()
 *********************************************************************************************************************/
/*! \brief           Decrements counter of timer and wakeup CAN controller under test if timer is finished.
 *  \details         -
 *  \pre             -
 *  \context         TASK|ISR
 *  \reentrant       FALSE
 *  \synchronous     TRUE
 *  \config          Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Can_SleepStartSequenceHandleTimer(void);

#  if defined (VITAHLP_CAN_RX_HW_OBJECT_AVAILABLE)
/**********************************************************************************************************************
 *  vItaHlp_Can_GetRxMapId()
 *********************************************************************************************************************/
/*! \brief       Returns the mapId of the Rx hardware object with the given hrh and canId.
 *  \details     -
 *  \param[in]   hrh     Hardware handle the PDU was received in.
 *  \param[in]   canId   CAN identifier of the received PDU.
 *  \param[out]  mapId   Explicit ID which identifier a combination of hardware object ID and driver ID.
 *  \return      TRUE:   mapId for given input combination found
 *               FALSE:  given input combination is not valid, cannot be matched with a mapId
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Can_GetRxMapId(
  VAR(CanIf_HwHandleType, AUTOMATIC) hrh,
  VAR(Can_IdType, AUTOMATIC) canId,
  P2VAR(uint16, AUTOMATIC, VITAHLP_APPL_VAR) mapId);
#  endif

/**********************************************************************************************************************
 *  vItaHlp_Can_GetIndexCanIfChannelMap()
 *********************************************************************************************************************/
/*! \brief       Verifies if the given abstract CanIf controller is given in the internal config struct array and
 *               returns the corresponding index.
 *  \details     -
 *  \param[in]   canIfAbstractControllerId Abstract CanIf controller ID.
 *  \param[out]  canIfChannelMapIndex      Index of the internal config array which contains the entry for the given
 *                                         abstract CanIf controller ID.
 *  \return      TRUE:   canIfChannelMapIndex for given input combination found
 *               FALSE:  given input cannot be found in internal config array
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Can_GetIndexCanIfChannelMap(
  VAR(uint8, AUTOMATIC) canIfAbstractControllerId,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) canIfChannelMapIndex);
# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_Can_SetSetupTestSduData()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Can_SetSetupTestSduData(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) sduData)
{
  if (vItaHlp_Can_SetupTestPatternIndicationToggleBit == FALSE)
  {
    VStdLib_MemSet(&sduData[0u], 0x00u, 4u);
    VStdLib_MemSet(&sduData[4u], 0xFFu, 4u);
    vItaHlp_Can_SetupTestPatternIndicationToggleBit = TRUE;
  }
  else
  {
    VStdLib_MemSet(&sduData[0u], 0xFFu, 4u);
    VStdLib_MemSet(&sduData[4u], 0x00u, 4u);
    vItaHlp_Can_SetupTestPatternIndicationToggleBit = FALSE;
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Can_GetIndexDriverSpecificData()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Can_GetIndexDriverSpecificData(
  VAR(vItaHlp_Can_DriverIndexType, AUTOMATIC) canDriverInstanceId,
  P2VAR(vItaHlp_Can_DriverIndexType, AUTOMATIC, VITAHLP_APPL_VAR) canDriverSpecificDataIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 dataIndex;
  boolean result = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  for (dataIndex = 0u; dataIndex < vItaHlp_Can_GetNumberOfDrivers(); dataIndex++)
  {
    if (vItaHlp_Can_DriverSpecificData[dataIndex].canDriverInstanceId == canDriverInstanceId)
    {
      canDriverSpecificDataIndex[0u] = dataIndex;
      result = TRUE;
      break;
    }
  }
  return result;
}

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Can_Write()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Can_Write(P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  vItaHlp_Can_ObjectDataType canTxObjectData;
  vItaHlp_Can_WriteFctPtrType canWriteFctPtr;
  uint16 hardwareObjectNumber;
  Can_PduType canTxPdu;
  uint8 vItaHlp_Can_TxSdu[4u];
  Can_ReturnType canWriteResult;
  uint8 canDriverSpecificDataIndex;
  boolean validCanDriverInstanceId;

  /* ----- Implementation ----------------------------------------------- */
  /* Retrieve the hardwareObjectNumber */
  hardwareObjectNumber = vItaHlp_CommonUtils_ByteArrayToUint16BigEndian(payloadData);

  /* If the signalled hardwareObjectNumber is supported */
  if (hardwareObjectNumber < vItaHlp_Can_GetNumberOfTxHardwareObjects())
  {
    /* Tx data related to the received HwObjectNumber */
    canTxObjectData = vItaHlp_Can_TxObjectData[hardwareObjectNumber];

    /* Set Tx data */
    VStdLib_MemCpy(vItaHlp_Can_TxSdu, &payloadData[2u], 4u);
    canTxPdu.length = 4u;
    canTxPdu.sdu = vItaHlp_Can_TxSdu; /* Desired CAN message payload, received by command request */
    canTxPdu.id = canTxObjectData.canId;
    canTxPdu.swPduHandle = (PduIdType) hardwareObjectNumber;

    /* Send CAN frame */
    validCanDriverInstanceId = vItaHlp_Can_GetIndexDriverSpecificData(
      canTxObjectData.canDriverInstanceId,
      &canDriverSpecificDataIndex);
    if (validCanDriverInstanceId == TRUE)
    {
      canWriteFctPtr = vItaHlp_Can_DriverSpecificData[canDriverSpecificDataIndex].canWriteFctPtr;
      canWriteResult = canWriteFctPtr(canTxObjectData.canObjectId, &canTxPdu);
      /* Send general error event in case the Can_Write function results in an problem */
      if (canWriteResult != CAN_OK)
      {
        vItaHlp_EventModuleUnderTestError(
          VITAHLP_CAN_MODULE_ID,
          canTxObjectData.canDriverInstanceId,
          VITAHLP_CAN_SID_WRITE_ID,
          canWriteResult);
      }
    }
    else
    {
      /* Send general error event in case the referenced canDriverInstanceId is not in the
       * configured driver specific array (out of range) - this can only happen in case of a invalid generated code */
      vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_CAN,
      VITAHLP_CAN_CMD_CAN_WRITE,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_INVALID_CONFIGURATION,
      0u);
    }
  }
  else
  {
    /* Send general error event in case requested hardware object is not valid (out of range) */
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_CAN,
      VITAHLP_CAN_CMD_CAN_WRITE,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_INVALID_REQUESTED_PARAM,
      0u);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Can_Sleep_Start()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Can_Sleep_Start(P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  vItaHlp_Can_SetControllerModePtrType canSetControllerModeFctPtr;
  Can_ReturnType canSetControllerModeResult;
  uint8 canDriverInstanceId;
  uint8 canControllerId;
  uint8 canDriverSpecificDataIndex;
  boolean validCanDriverInstanceId;

  /* ----- Implementation ----------------------------------------------- */
  SchM_Enter_vItaHlp_VITAHLP_CAN_EXCLUSIVE_AREA();
  if (vItaHlp_Can_SleepStartSequenceActive == FALSE)
  {
    /* Save the CAN Driver and CAN Controller index used for the test */
    canDriverInstanceId = payloadData[0u];
    validCanDriverInstanceId = vItaHlp_Can_GetIndexDriverSpecificData(canDriverInstanceId, &canDriverSpecificDataIndex);
    if (validCanDriverInstanceId == TRUE)
    {
      canControllerId = payloadData[1u];
      /* Set test control parameters */
      vItaHlp_Can_ControllerModeAfterStop = CAN_T_SLEEP; /* The expected final mode of the CAN Controller */
      vItaHlp_Can_ControllerUnderTest = canControllerId;
      vItaHlp_Can_DriverUnderTest = canDriverInstanceId;
      vItaHlp_Can_DriverUnderTestDataIndex = canDriverSpecificDataIndex;
      vItaHlp_Can_SleepStartSequenceActive = TRUE;
      SchM_Exit_vItaHlp_VITAHLP_CAN_EXCLUSIVE_AREA();

      /* Trigger Goto Sleep - Set CanController mode */
      canSetControllerModeFctPtr = 
        vItaHlp_Can_DriverSpecificData[canDriverSpecificDataIndex].canSetControllerModeFctPtr;
      canSetControllerModeResult = canSetControllerModeFctPtr(canControllerId, CAN_T_STOP);
      /* Send general error event in case the Can_SetControllerMode function results in an problem */
      if (canSetControllerModeResult != CAN_OK)
      {
        vItaHlp_EventModuleUnderTestError(
          VITAHLP_CAN_MODULE_ID,
          canDriverInstanceId,
          VITAHLP_CAN_SID_SETCTR_ID,
          canSetControllerModeResult);
      }
    }
    else
    {
      /* Send general error event of vItaHlp in case the requested instance Id is out of range */
      SchM_Exit_vItaHlp_VITAHLP_CAN_EXCLUSIVE_AREA();
      vItaHlp_EventVItaHlpError(
        VITAHLP_COMPONENTIDENTIFIER_CAN,
        VITAHLP_CAN_CMD_SLEEP_START,
        VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
        VITAHLP_E_INVALID_REQUESTED_PARAM,
        0u);
    }
  }
  else
  {
    SchM_Exit_vItaHlp_VITAHLP_CAN_EXCLUSIVE_AREA();
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Can_SleepStartSequenceControllerModeIndication()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Can_SleepStartSequenceControllerModeIndication(
  VAR(uint8, AUTOMATIC) canDriverInstanceId,
  VAR(vItaHlp_Can_ControllerIndexType, AUTOMATIC) canControllerId,
  VAR(uint8, AUTOMATIC) canControllerMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* Apply defined sleep/start sequence of design if mode indication is received
   * for the expected driver (driver under test)
   */
  if (vItaHlp_Can_SleepStartSequenceActive)
  {
    if ((canDriverInstanceId == vItaHlp_Can_DriverUnderTest) && (canControllerId == vItaHlp_Can_ControllerUnderTest))
    {
      /* Depending on CAN Controller mode, which is signalled to CanIf (stub) */
      switch (canControllerMode)
      {
        case CANIF_CS_STOPPED:
        {
          vItaHlp_Can_SetControllerModeForActiveController();
          break;
        }
        case CANIF_CS_STARTED:
        {
          vItaHlp_Can_SleepStartSequenceActive = FALSE;
          break;
        }
        case CANIF_CS_SLEEP:
        {
          vItaHlp_Can_StartTimerForCanStart();
          break;
        }
        default:
        {
          /* No action */
        }
      }
    }
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Can_SetControllerModeForActiveController()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Can_SetControllerModeForActiveController(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  vItaHlp_Can_SetControllerModePtrType canSetControllerModeFctPtr;
  Can_ReturnType canResult;

  /* ----- Implementation ----------------------------------------------- */
  /* Trigger next mode change, if necessary - Set CanController mode */
  /* If there is a difference between internal stored CanDriverUnderTest (CanControllerUnderTest)
   * and the ones reported in the ModeIndication, it is recognized by TestControl
   */
  canSetControllerModeFctPtr =
    vItaHlp_Can_DriverSpecificData[vItaHlp_Can_DriverUnderTestDataIndex].canSetControllerModeFctPtr;
  canResult = canSetControllerModeFctPtr(vItaHlp_Can_ControllerUnderTest, vItaHlp_Can_ControllerModeAfterStop);
  /* Send general error event in case the Can_SetControllerMode function results in an problem */
  if (canResult != CAN_OK)
  {
    vItaHlp_EventModuleUnderTestError(
      VITAHLP_CAN_MODULE_ID,
      vItaHlp_Can_DriverUnderTest,
      VITAHLP_CAN_SID_SETCTR_ID,
      canResult);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Can_StartTimerForCanStart()
 *********************************************************************************************************************/
VITAHLP_LOCAL_INLINE FUNC(void, VITAHLP_CODE) vItaHlp_Can_StartTimerForCanStart(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ----------------------------------------------- */
  /* Trigger a 5s waiting condition, before CAN Controller is started again. Handled by vItaHlp_Can_MainFunction() */
  vItaHlp_Can_NumberTicks = (uint16) (5u * VITAHLP_CAN_TICKS_PER_SECOND);
  vItaHlp_Can_SleepStartRestartTimerActive = TRUE;
}

/**********************************************************************************************************************
 *  vItaHlp_Can_SleepStartSequenceHandleTimer()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Can_SleepStartSequenceHandleTimer(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  vItaHlp_Can_SetControllerModePtrType canSetControllerModeFctPtr;
  Can_ReturnType canResult;

  /* ----- Implementation ----------------------------------------------- */
  if (vItaHlp_Can_SleepStartRestartTimerActive)
  {
    /* If end of waiting time is reached, trigger Internal Wakeup of CAN Controller */
    if (vItaHlp_Can_NumberTicks == 0u)
    {
      vItaHlp_Can_ControllerModeAfterStop = CAN_T_START;
      canSetControllerModeFctPtr =
      vItaHlp_Can_DriverSpecificData[vItaHlp_Can_DriverUnderTestDataIndex].canSetControllerModeFctPtr;
      canResult = canSetControllerModeFctPtr(vItaHlp_Can_ControllerUnderTest, CAN_T_WAKEUP);
      /* Send general error event in case the Can_SetControllerMode function results in an problem */
      if (canResult != CAN_OK)
      {
        vItaHlp_EventModuleUnderTestError(
          VITAHLP_CAN_MODULE_ID,
          vItaHlp_Can_DriverUnderTest,
          VITAHLP_CAN_SID_SETCTR_ID,
          canResult);
      }
      vItaHlp_Can_SleepStartRestartTimerActive = FALSE;
    }
    else
    {
      vItaHlp_Can_NumberTicks--;
    }
  }
}

#  if defined (VITAHLP_CAN_RX_HW_OBJECT_AVAILABLE)
/**********************************************************************************************************************
 *  vItaHlp_Can_GetRxMapId()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Can_GetRxMapId(
  VAR(CanIf_HwHandleType, AUTOMATIC) hrh,
  VAR(Can_IdType, AUTOMATIC) canId,
  P2VAR(uint16, AUTOMATIC, VITAHLP_APPL_VAR) mapId)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean mapIdDetected = FALSE;
  uint16 rxHwObjectIdx;

  /* ----- Implementation ----------------------------------------------- */
  for (rxHwObjectIdx = 0u; rxHwObjectIdx < vItaHlp_Can_GetNumberOfRxHardwareObjects(); rxHwObjectIdx++)
  {
    if ( (vItaHlp_Can_RxObjectData[rxHwObjectIdx].canObjectId == hrh)
      && (vItaHlp_Can_RxObjectData[rxHwObjectIdx].canId == canId) )
    {
      mapId[0u] = rxHwObjectIdx;
      mapIdDetected = TRUE;
      break;
    }
  }
  return mapIdDetected;
}
#  endif

/**********************************************************************************************************************
 *  vItaHlp_Can_GetIndexCanIfChannelMap()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Can_GetIndexCanIfChannelMap(
  VAR(uint8, AUTOMATIC) canIfAbstractControllerId,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) canIfChannelMapIndex)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 mapIndex;
  boolean result = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  for (mapIndex = 0u; mapIndex < vItaHlp_Can_GetNumberOfCanIfChannels(); mapIndex++)
  {
    if (vItaHlp_Can_CanIfChannelMap[mapIndex].canIfAbstractControllerId == canIfAbstractControllerId)
    {
      canIfChannelMapIndex[0u] = mapIndex;
      result = TRUE;
      break;
    }
  }
  return result;
}
# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_Can_Init()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Can_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  vItaHlp_Can_DriverIndexType canDriverInstanceId;
  vItaHlp_Can_DriverIndexType canDriverSpecificDataIndex;
  vItaHlp_Can_ControllerIndexType canIfChMapIdx;
  vItaHlp_Can_ControllerIndexType canControllerId;
  vItaHlp_Can_SetControllerModePtrType canSetControllerModeFctPtr;
  Can_ReturnType canResult;
  boolean validCanDriverSpecificDataIndex;

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize test pattern bit for setup test */
  vItaHlp_Can_SetupTestPatternIndicationToggleBit = FALSE;

  /* Set default handling for vItaHlp_Can_MainFunction() */
  vItaHlp_Can_SleepStartRestartTimerActive = FALSE;
  vItaHlp_Can_SleepStartSequenceActive = FALSE;

  /* Start each configured CAN controller for each configured CAN driver */
  for (canIfChMapIdx = 0u; canIfChMapIdx < vItaHlp_Can_GetNumberOfCanIfChannels(); canIfChMapIdx++)
  {
    canDriverInstanceId = vItaHlp_Can_CanIfChannelMap[canIfChMapIdx].canDriverInstanceId;
    canControllerId = vItaHlp_Can_CanIfChannelMap[canIfChMapIdx].canControllerId;
    validCanDriverSpecificDataIndex =
    vItaHlp_Can_GetIndexDriverSpecificData(canDriverInstanceId, &canDriverSpecificDataIndex);
    if (validCanDriverSpecificDataIndex == TRUE)
    {
      canSetControllerModeFctPtr =
        vItaHlp_Can_DriverSpecificData[canDriverSpecificDataIndex].canSetControllerModeFctPtr;
      canResult = canSetControllerModeFctPtr(canControllerId, CAN_T_START);
# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
      if (canResult != CAN_OK)
      {
        vItaHlp_EventModuleUnderTestError(
          VITAHLP_CAN_MODULE_ID,
          canDriverInstanceId,
          VITAHLP_CAN_SID_SETCTR_ID,
          canResult);
      }
# else
      VITAHLP_DUMMY_STATEMENT(canResult);
# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */
    }
# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
    else
    {
      /* Send general error event in case the referenced canDriverInstanceId is not in the
      * configured driver specific array (out of range) - this can only happen in case of a invalid generated code */
      vItaHlp_EventVItaHlpError(
        VITAHLP_COMPONENTIDENTIFIER_CAN,
        VITAHLP_CAN_CMD_SERVICEIDENTIFIER_UNKNOWN,
        VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
        VITAHLP_E_INVALID_CONFIGURATION,
        0u);
    }
# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Can_TriggerSetupTest()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Can_TriggerSetupTest(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16_least txObjectPduId;
  uint8 canSduData[VITAHLP_CAN_SETUPTEST_DLC];
  vItaHlp_Can_DriverIndexType canDriverSpecificDataIndex;

  /* ----- Implementation ----------------------------------------------- */
  /* Fill sdu data with test pattern */
  vItaHlp_Can_SetSetupTestSduData(canSduData);

  /* Send all Tx hardware objects configured for the setup test */
  for (txObjectPduId = 0u; txObjectPduId < vItaHlp_Can_GetNumberOfSetupTestTxObjects(); txObjectPduId++)
  {
    vItaHlp_Can_ObjectDataType canTxObjectData;
    Can_PduType pduType;
    vItaHlp_Can_WriteFctPtrType canWriteFctPtr;

    /* Prepare pdu data */
    canTxObjectData = vItaHlp_Can_TxObjectData[txObjectPduId];
    pduType.id = canTxObjectData.canId;
    pduType.length = VITAHLP_CAN_SETUPTEST_DLC;
    pduType.sdu = canSduData;
    pduType.swPduHandle = (PduIdType) txObjectPduId;
    /* Send Tx hardware object */
    if (vItaHlp_Can_GetIndexDriverSpecificData(canTxObjectData.canDriverInstanceId, &canDriverSpecificDataIndex))
    {
      canWriteFctPtr = vItaHlp_Can_DriverSpecificData[canDriverSpecificDataIndex].canWriteFctPtr;
      (void)canWriteFctPtr(canTxObjectData.canObjectId, &pduType);
    }
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Can_MainFunction()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Can_MainFunction(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  /* ----- Implementation ---------------------------------------------- */
# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
  vItaHlp_Can_SleepStartSequenceHandleTimer();
# endif
}

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Can_DispatchService()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Can_DispatchService(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  switch (serviceIdentifier)
  {
    case VITAHLP_CAN_CMD_CAN_WRITE:
      vItaHlp_Can_Write(payloadData);
      break;
    case VITAHLP_CAN_CMD_SLEEP_START:
      vItaHlp_Can_Sleep_Start(payloadData);
      break;
    default:
      vItaHlp_EventVItaHlpError(
        VITAHLP_COMPONENTIDENTIFIER_CAN,
        serviceIdentifier,
        VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
        VITAHLP_E_UNKNOWN_CMD,
        VITAHLP_TESTCOMMUNICATION_UNKNOWN_SERVICEIDENTIFIER);
      break;
  }
}

/**********************************************************************************************************************
 *  APPLICATION SPECIFIC CALLBACK FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_CanIf_RxIndication_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_CanIf_RxIndication_Callout(
  VAR(CanIf_HwHandleType, AUTOMATIC) hrh,
  VAR(Can_IdType, AUTOMATIC) canId,
  VAR(uint8, AUTOMATIC) canDlc,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) canSduPtr)
{
#  if defined (VITAHLP_CAN_RX_HW_OBJECT_AVAILABLE)
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];
  uint16 mapId;
  boolean expectedCanMessage = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* Verify if received PDU has the size of at least 4 bytes which contains the payload of the CAN test message */
  if (canDlc >= 4u)
  {
    /* HwObjectMapId */
    expectedCanMessage = vItaHlp_Can_GetRxMapId(hrh, canId, &mapId);
    if (expectedCanMessage)
    {
      vItaHlp_CommonUtils_Uint16ToByteArrayBigEndian(payloadBuffer, mapId);

      /* Payload from received CAN frame */
      VStdLib_MemCpy(&payloadBuffer[2u], canSduPtr, 4u);

      /* Forward to EventHandler */
      vItaHlp_SetEvent(
        VITAHLP_COMPONENTIDENTIFIER_CAN,
        VITAHLP_CAN_EVENT_RX_INDICATION,
        (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadBuffer);
    }
  }
  /* Send general error event in case the hrh and canId combination is not given
  * in the hardware object mapId table or if the DLC is too small
  */
  if (!expectedCanMessage)
  {
#  endif
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_CAN,
      VITAHLP_CAN_EVENT_RX_INDICATION,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_EVENT,
      VITAHLP_E_INVALID_UNEXPECTED_CALLBACK_RECEIVED,
      0u);
#  if defined (VITAHLP_CAN_RX_HW_OBJECT_AVAILABLE)
  }
#  else
    VITAHLP_DUMMY_STATEMENT(hrh);
    VITAHLP_DUMMY_STATEMENT(canId);
    VITAHLP_DUMMY_STATEMENT(canDlc);
    VITAHLP_DUMMY_STATEMENT_CONST(canSduPtr);
#  endif
}

/**********************************************************************************************************************
 *  vItaHlp_CanIf_TxConfirmation_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_CanIf_TxConfirmation_Callout(VAR(PduIdType, AUTOMATIC) canTxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  /* Copy CanTxPduId to payload (CanTxPduId type can be configured as uint8 or uint16 in EcucPduCollection,
   *  maximum size is uint16, therefore 2 Bytes are used in the protocol as defined in the design specification)
   */
  vItaHlp_CommonUtils_Uint16ToByteArrayBigEndian(payloadBuffer, (uint16)canTxPduId);

  /* Forward to EventHandler */
  vItaHlp_SetEvent(
    VITAHLP_COMPONENTIDENTIFIER_CAN,
    VITAHLP_CAN_EVENT_TX_CONFIRMATION,
    (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadBuffer);
}

/**********************************************************************************************************************
 *  vItaHlp_CanIf_ControllerModeIndication_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_CanIf_ControllerModeIndication_Callout(
  VAR(uint8, AUTOMATIC) controllerId,
  VAR(CanIf_ControllerModeType, AUTOMATIC) controllerMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];
  uint8 canDriverInstanceId;
  vItaHlp_Can_ControllerIndexType canControllerId;
  uint8 canIfChannelMapIndex;
  boolean validCanIfCtrlId;

  /* ----- Implementation ----------------------------------------------- */
  validCanIfCtrlId = vItaHlp_Can_GetIndexCanIfChannelMap(controllerId, &canIfChannelMapIndex);
  if (validCanIfCtrlId == TRUE)
  {
    /* Fill payload according to protocol specification */
    canDriverInstanceId = vItaHlp_Can_CanIfChannelMap[canIfChannelMapIndex].canDriverInstanceId;
    canControllerId = vItaHlp_Can_CanIfChannelMap[canIfChannelMapIndex].canControllerId;
    payloadBuffer[0u] = canDriverInstanceId;
    payloadBuffer[1u] = canControllerId;
    payloadBuffer[2u] = (uint8)controllerMode;

    /* Always send event to inform about mode indication,
     * receiver/tester can analyse if mode indication for this driver and controller is expected
     */
    vItaHlp_SetEvent(
      VITAHLP_COMPONENTIDENTIFIER_CAN,
      VITAHLP_CAN_EVENT_CONTROLLER_MODE_INDICATION,
      (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadBuffer);

    vItaHlp_Can_SleepStartSequenceControllerModeIndication(canDriverInstanceId, canControllerId, controllerMode);
  }
  /* Send general error event in case the abstract controller ID of the CanIf is unknown/invalid */
  else
  {
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_CAN,
      VITAHLP_CAN_EVENT_CONTROLLER_MODE_INDICATION,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_EVENT,
      VITAHLP_E_INVALID_UNEXPECTED_CALLBACK_RECEIVED,
      0u);
  }
}
# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VITAHLP_CAN == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_Can.c
 *********************************************************************************************************************/
