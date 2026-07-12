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
/**        \file  vItaHlp.c
 *        \brief  Source file which contains the general parts of vItaHlp.
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
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define VITAHLP_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vItaHlp.h"
#include "vItaHlp_CommonUtils.h"
#include "BrsMain.h"
#include "SchM_vItaHlp.h"

#if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
# include "vItaHlp_EventQueue.h"
# include "vItaHlp_TestCom.h"
#endif

#if (VITAHLP_CAN == STD_ON)
# include "vItaHlp_Can.h"
#endif
#if (VITAHLP_OS == STD_ON)
# include "vItaHlp_Os.h"
#endif
#if (VITAHLP_ETH == STD_ON)
# include "vItaHlp_Eth.h"
#endif
#if (VITAHLP_FLS == STD_ON)
# include "vItaHlp_Fls.h"
#endif
#if (VITAHLP_MCU == STD_ON)
# include "vItaHlp_Mcu.h"
#endif
#if (VITAHLP_LIN == STD_ON)
# include "vItaHlp_Lin.h"
#endif
#if (VITAHLP_WDG == STD_ON)
# include "vItaHlp_Wdg.h"
#endif

#if (VITAHLP_WDGSERVICE == STD_ON)
# include "vItaHlp_WdgService.h"
#endif

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Check the version of vItaHlp header file */
#if (  (VITAHLP_SW_MAJOR_VERSION != (6u)) \
    || (VITAHLP_SW_MINOR_VERSION != (0u)) \
    || (VITAHLP_SW_PATCH_VERSION != (2u)) )
# error "Vendor specific version numbers of vItaHlp.c and vItaHlp.h are inconsistent"
#endif

/* Check the version of the configuration header file */
#if (  (VITAHLP_CFG_MAJOR_VERSION != (6u)) \
    || (VITAHLP_CFG_MINOR_VERSION != (0u)) )
# error "Version numbers of vItaHlp.c and vItaHlp_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#if !defined (VITAHLP_LOCAL)
# define VITAHLP_LOCAL static
#endif

#if !defined (VITAHLP_LOCAL_INLINE)
# define VITAHLP_LOCAL_INLINE LOCAL_INLINE
#endif

/* ------ Definitions used for the test communication protocol ------ */

/* Test Communication Message Layout */
typedef enum
{
  VITAHLP_TESTCOMMUNICATION_MESSAGE_POS_COMPONENTIDENTIFIER = 0u,
  VITAHLP_TESTCOMMUNICATION_MESSAGE_POS_SERVICEIDENTIFIER   = 1u,
  VITAHLP_TESTCOMMUNICATION_MESSAGE_POS_PAYLOAD_BEGIN       = 2u
} vItaHlp_TestCommunicationMessageLayoutType;

/* Byte 1: SI - Command Service Identifier */
typedef enum
{
  VITAHLP_CMD_SERVICEIDENTIFIER_GET_VERSION_INFO            = 0u,
  VITAHLP_CMD_SERVICEIDENTIFIER_SET_SETUPTEST_STATE         = 1u,
  VITAHLP_CMD_SERVICEIDENTIFIER_SET_WDG_ALIVE_EVENT_STATUS  = 2u,
  VITAHLP_CMD_SERVICEIDENTIFIER_UNKNOWN                     = 0xFFu
} vItaHlp_TestCommunicationCmdServiceIdentifierType;

/* Byte 1: SI - Event Service Identifier */
typedef enum
{
  VITAHLP_EVENT_SERVICEIDENTIFIER_GET_VERSION_INFO          = 0u,
  VITAHLP_EVENT_SERVICEIDENTIFIER_VITAHLP_ERROR             = 0xFDu,
  VITAHLP_EVENT_SERVICEIDENTIFIER_MODULE_UNDER_TEST_ERROR   = 0xFEu,
  VITAHLP_EVENT_SERVICEIDENTIFIER_DET_ERROR                 = 0xFFu
} vItaHlp_TestCommunicationEventServiceIdentifierType;

typedef enum
{
  VITAHLP_EVENT_GET_VERSION_INFO_MULTIPLEXER_GENERAL        = 0u,
  VITAHLP_EVENT_GET_VERSION_INFO_MULTIPLEXER_SW_VERSION     = 1u
} vItaHlp_TestCommunicationEventGetVersionInfoMultiplexerType;

typedef enum
{
  VITAHLP_SETUPTEST_UNINITIALIZED = 0u,
  VITAHLP_SETUPTEST_DISABLED      = 1u,
  VITAHLP_SETUPTEST_ENABLED       = 2u
} vItaHlp_SetupTestType;

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

#define VITAHLP_START_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/* Flag contains information if the setup test is runnable for the cyclic processing or not
 * No exclusive area because no read/modify write of global variable.
 */
VAR(vItaHlp_SetupTestType, VITAHLP_VAR_NOINIT) vItaHlp_SetupTestEnabled;
#define VITAHLP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define VITAHLP_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_DispatchService()
 *********************************************************************************************************************/
/*! \brief       Dispatches the test communication message based on the serviceIdentifier.
 *  \details     Evaluates the service identifier of the general test communication command and calls the specific
 *               command function for further processing.
 *  \param[in]   serviceIdentifier     Identifier of the requested service.
 *  \param[in]   payloadData           Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_DispatchService(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_CmdGetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Function handles the command for GetVersionInfo and triggers the event containing the
 *               vItaHlp module version.
 *  \details     -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_CmdGetVersionInfo(void);

/**********************************************************************************************************************
 *  vItaHlp_CmdSetSetupTestState()
 *********************************************************************************************************************/
/*! \brief       Function handles the command for SetSetupTestState and sets the corresponding state
 *               for the setup test. This makes is possible to enable/disable the setup test for the configured MCAL
 *               modules.
 *  \details     -
 *  \param[in]   payloadData Contains the CI/SI specific payload of the test communication message.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_CmdSetSetupTestState(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_CmdSetWdgAliveEventStatus()
 *********************************************************************************************************************/
/*! \brief       Function handles the command for SetWdgAliveEventStatus and sets the corresponding status.
 *               
 *  \details     The corresponding status of the WDG alive event handling is set. This makes is possible to
 *               enable/disable the reporting of WDG alive events during a test.
 *               If the WdgService mechanism provided by vItaHlp is disabled (no WDG module configured), the function
 *               returns immediately.
 *  \param[in]   payloadData Contains the CI/SI specific payload of the test communication message.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_CmdSetWdgAliveEventStatus(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_EventGetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Function creates event content for GetVersionInfo event and processes it.
 *  \details     -
 *  \param[in]   versioninfo Pointer which contains the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_EventGetVersionInfo(
  P2CONST(Std_VersionInfoType, AUTOMATIC, VITAHLP_APPL_VAR) versionInfo);

/**********************************************************************************************************************
 *  vItaHlp_EventDetError()
 *********************************************************************************************************************/
/*! \brief       Function creates event content for a DET error and processes it.
 *  \details     -
 *  \param[in]   moduleId   Module ID for which the DET occurred.
 *  \param[in]   instanceId Instance ID of the module for which the DET occurred.
 *  \param[in]   apiId      API ID of the module for which the DET occurred.
 *  \param[in]   errorId    Error ID of the module for which the DET occurred.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_EventDetError(
  VAR(uint16, AUTOMATIC) moduleId,
  VAR(uint8, AUTOMATIC) instanceId,
  VAR(uint8, AUTOMATIC) apiId,
  VAR(uint8, AUTOMATIC) errorId);

/**********************************************************************************************************************
 *  vItaHlp_SendEvent()
 *********************************************************************************************************************/
/*! \brief       Function sends one event out of the Event Queue.
 *  \details     If the event queue is not empty and if test communication link is established, the next
 *               event is sent. The index of the event to be send is defined in vItaHlp_EventIndexNextToSend.
 *               The function needs to be called cyclic.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_SendEvent(void);

/**********************************************************************************************************************
 *  vItaHlp_CreateVItaHlpErrorMessage()
 *********************************************************************************************************************/
/*! \brief       Function creates fills the test communication message buffer with the content for a vItaHlp Error.
 *  \details     -
 *  \pre         -
 *  \param[out]  messageBuffer       Gets filled with the given content for the vItaHlp error.
 *  \param[in]   componentIdentifier Identifier of the component.
 *  \param[in]   serviceIdentifier   Identifier of the service.
 *  \param[in]   isCmd               0: SI is given for an event 1: SI is given for a command.
 *  \param[in]   returnCode          return code which is defined for vItaHlp in header file.
 *  \param[in]   optionalData        optional data which can be used for further information for the given return code
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_CreateVItaHlpErrorMessage(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) messageBuffer,
  VAR(uint8, AUTOMATIC) componentIdentifier,
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  VAR(uint8, AUTOMATIC) isCmd,
  VAR(uint8, AUTOMATIC) returnCode,
  VAR(uint16, AUTOMATIC) optionalData);

/**********************************************************************************************************************
 *  vItaHlp_SendEventLL()
 *********************************************************************************************************************/
/*! \brief       Function transmits the event to the lower layer module.
 *  \details     -
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL_INLINE FUNC(void, VITAHLP_CODE) vItaHlp_SendEventLL(P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) event);

/**********************************************************************************************************************
 *  vItaHlp_HandleEventOverflowCounter()
 *********************************************************************************************************************/
/*! \brief       Function creates event message for the given overflow counter and transmits it to the lower layer.
 *               Attention: Overflow counter is not evaluated, it is also send if the value is equal to zero.
 *  \details     -
 *  \pre         -
 *  \param[in]   overflowCounter counter which contains the number of overflowed events.
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL_INLINE FUNC(void, VITAHLP_CODE) vItaHlp_HandleEventOverflowCounter(
  VAR(uint8, AUTOMATIC) overflowCounter);

#endif /* VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_DispatchService()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_DispatchService(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  switch (serviceIdentifier)
  {
    case VITAHLP_CMD_SERVICEIDENTIFIER_GET_VERSION_INFO:
      vItaHlp_CmdGetVersionInfo();
      break;
    case VITAHLP_CMD_SERVICEIDENTIFIER_SET_SETUPTEST_STATE:
      vItaHlp_CmdSetSetupTestState(payloadData);
      break;
    case VITAHLP_CMD_SERVICEIDENTIFIER_SET_WDG_ALIVE_EVENT_STATUS:
      vItaHlp_CmdSetWdgAliveEventStatus(payloadData);
      break;
    default:
      vItaHlp_EventVItaHlpError(
        VITAHLP_COMPONENTIDENTIFIER_GENERAL,
        serviceIdentifier,
        VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
        VITAHLP_E_UNKNOWN_CMD,
        VITAHLP_TESTCOMMUNICATION_UNKNOWN_SERVICEIDENTIFIER);
      break;
  }
}

/**********************************************************************************************************************
 *  vItaHlp_CmdGetVersionInfo()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_CmdGetVersionInfo(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_VersionInfoType versionInfoType;

  /* ----- Implementation ----------------------------------------------- */
  vItaHlp_GetVersionInfo(&versionInfoType);
  vItaHlp_EventGetVersionInfo(&versionInfoType);
}

/**********************************************************************************************************************
 *  vItaHlp_CmdSetSetupTestState()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_CmdSetSetupTestState(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean receivedSetupTestState;

  /* ----- Implementation ----------------------------------------------- */
  receivedSetupTestState = (boolean) payloadData[0u];
  if (receivedSetupTestState == FALSE)
  {
    vItaHlp_SetupTestEnabled = VITAHLP_SETUPTEST_DISABLED;
  }
  else
  {
    vItaHlp_SetupTestEnabled = VITAHLP_SETUPTEST_ENABLED;
  }
}

/**********************************************************************************************************************
 *  vItaHlp_CmdSetWdgAliveEventStatus()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_CmdSetWdgAliveEventStatus(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
# if(VITAHLP_WDGSERVICE == STD_ON)
  /* ----- Local Variables ---------------------------------------------- */
  boolean receivedWdgAliveEventStatus;

  /* ----- Implementation ----------------------------------------------- */
  receivedWdgAliveEventStatus = (boolean)payloadData[0u];
  if (receivedWdgAliveEventStatus == FALSE)
  {
    vItaHlp_WdgService_SetAliveEventStatus(VITAHLP_WDG_ALIVE_EVENT_DISABLED);
  }
  else
  {
    vItaHlp_WdgService_SetAliveEventStatus(VITAHLP_WDG_ALIVE_EVENT_ENABLED);
  }
# else
  VITAHLP_DUMMY_STATEMENT_CONST(payloadData);
# endif
}

/**********************************************************************************************************************
 *  vItaHlp_EventGetVersionInfo()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_EventGetVersionInfo(
  P2CONST(Std_VersionInfoType, AUTOMATIC, VITAHLP_APPL_VAR) versionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  /* Create event multiplex part I - vendor and module id */
  payloadBuffer[0u] = VITAHLP_EVENT_GET_VERSION_INFO_MULTIPLEXER_GENERAL;
  vItaHlp_CommonUtils_Uint16ToByteArrayBigEndian(&payloadBuffer[1u], versionInfo->vendorID);
  vItaHlp_CommonUtils_Uint16ToByteArrayBigEndian(&payloadBuffer[3u], versionInfo->moduleID);
  vItaHlp_SetEvent(
    VITAHLP_COMPONENTIDENTIFIER_GENERAL,
    VITAHLP_EVENT_SERVICEIDENTIFIER_GET_VERSION_INFO,
    (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadBuffer);

  /* Create event multiplex part II - sw version info */
  payloadBuffer[0u] = VITAHLP_EVENT_GET_VERSION_INFO_MULTIPLEXER_SW_VERSION;
  payloadBuffer[1u] = versionInfo->sw_major_version;
  payloadBuffer[2u] = versionInfo->sw_minor_version;
  payloadBuffer[3u] = versionInfo->sw_patch_version;
  vItaHlp_SetEvent(
    VITAHLP_COMPONENTIDENTIFIER_GENERAL,
    VITAHLP_EVENT_SERVICEIDENTIFIER_GET_VERSION_INFO,
    (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadBuffer);
}

/**********************************************************************************************************************
 *  vItaHlp_EventDetError()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_EventDetError(
  VAR(uint16, AUTOMATIC) moduleId,
  VAR(uint8, AUTOMATIC) instanceId,
  VAR(uint8, AUTOMATIC) apiId,
  VAR(uint8, AUTOMATIC) errorId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  /* Prepare payloadBuffer with DET Error event data */
  vItaHlp_CommonUtils_Uint16ToByteArrayBigEndian(payloadBuffer, moduleId);
  payloadBuffer[2u] = instanceId;
  payloadBuffer[3u] = apiId;
  payloadBuffer[4u] = errorId;
  vItaHlp_SetEvent(
    VITAHLP_COMPONENTIDENTIFIER_GENERAL,
    VITAHLP_EVENT_SERVICEIDENTIFIER_DET_ERROR,
    (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadBuffer);
}

/**********************************************************************************************************************
 *  vItaHlp_SendEvent()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_SendEvent(void)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* Send next event, if test communication link is established. */
  if (vItaHlp_TestCom_GetLinkStatus() == VITAHLP_TESTCOM_LINKUP)
  {
    boolean validEvent = FALSE;
    uint8 nextEvent[VITAHLP_TESTCOMMUNICATION_MESSAGE_LENGTH];
    uint8 overflowCounter = 0u;
    validEvent = vItaHlp_EventQueue_Dequeue(nextEvent, &overflowCounter);
    if (validEvent == TRUE)
    {
      vItaHlp_SendEventLL(nextEvent);
      if (overflowCounter > 0u)
      {
        vItaHlp_HandleEventOverflowCounter(overflowCounter);
      }
    }
  }
}

/**********************************************************************************************************************
 *  vItaHlp_CreateVItaHlpErrorMessage()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_CreateVItaHlpErrorMessage(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) messageBuffer,
  VAR(uint8, AUTOMATIC) componentIdentifier,
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  VAR(uint8, AUTOMATIC) isCmd,
  VAR(uint8, AUTOMATIC) returnCode,
  VAR(uint16, AUTOMATIC) optionalData)
{
  messageBuffer[0u] = VITAHLP_COMPONENTIDENTIFIER_GENERAL;
  messageBuffer[1u] = VITAHLP_EVENT_SERVICEIDENTIFIER_VITAHLP_ERROR;
  messageBuffer[2u] = componentIdentifier;
  messageBuffer[3u] = serviceIdentifier;
  messageBuffer[4u] = isCmd;
  messageBuffer[5u] = returnCode;
  vItaHlp_CommonUtils_Uint16ToByteArrayBigEndian(&messageBuffer[6u], optionalData);
}

/**********************************************************************************************************************
 *  vItaHlp_SendEventLL()
 *********************************************************************************************************************/
VITAHLP_LOCAL_INLINE FUNC(void, VITAHLP_CODE) vItaHlp_SendEventLL(P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) event)
{
  /* ----- Local Variables ---------------------------------------------- */
  PduInfoType pduInfoPtr;
  pduInfoPtr.SduLength = VITAHLP_TESTCOMMUNICATION_MESSAGE_LENGTH;
  pduInfoPtr.SduDataPtr = event;
  /* ----- Implementation ----------------------------------------------- */
  vItaHlp_TestCom_TransmitData(&pduInfoPtr);
}

/**********************************************************************************************************************
 *  vItaHlp_HandleEventOverflowCounter()
 *********************************************************************************************************************/
VITAHLP_LOCAL_INLINE FUNC(void, VITAHLP_CODE) vItaHlp_HandleEventOverflowCounter(VAR(uint8, AUTOMATIC) overflowCounter)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 overflowEvent[VITAHLP_TESTCOMMUNICATION_MESSAGE_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  vItaHlp_CreateVItaHlpErrorMessage(
    overflowEvent,
    VITAHLP_COMPONENTIDENTIFIER_GENERAL,
    VITAHLP_EVENT_SERVICEIDENTIFIER_VITAHLP_ERROR,
    VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_EVENT,
    VITAHLP_E_EVENT_OVERFLOW,
    overflowCounter);
  vItaHlp_SendEventLL(overflowEvent);
}

#endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_PreInit()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_PreInit(void)
{
  /* ------ Initialize global variables to default startup values ------ */
  /* Setup test is disabled for preinit phase.
     Must not be activated before ECU startup phase 2 (MCAL modules must already be initialized)
   */
  vItaHlp_SetupTestEnabled = VITAHLP_SETUPTEST_UNINITIALIZED;

#if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
  vItaHlp_TestCom_PreInit();
  /* Initializes the event queue at startup phase 1 in order to catch events from an early stage */
  vItaHlp_EventQueue_Init();
#endif
#if (VITAHLP_ETH == STD_ON)
  /* Preparation of test environment before initialization of modules under test */
  vItaHlp_Eth_PreInit();
#endif
}

/**********************************************************************************************************************
 *  vItaHlp_Init()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Init(void)
{
  if (GetCoreID() == OS_CORE_ID_MASTER)
  {
    /* Initialization for Module Runtime tests.
       Start communication modules.
     */
#if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
    vItaHlp_TestCom_Init();
#endif
#if (VITAHLP_CAN == STD_ON)
    vItaHlp_Can_Init();
#endif
#if (VITAHLP_ETH == STD_ON)
    vItaHlp_Eth_Init();
#endif
#if (VITAHLP_FLS == STD_ON)
    vItaHlp_Fls_Init();
#endif
#if (VITAHLP_LIN == STD_ON)
    vItaHlp_Lin_Init();
#endif
#if (VITAHLP_WDGSERVICE == STD_ON)
    vItaHlp_WdgService_Init();
#endif

    /* Setup test is runnable, MCAL modules and vItaHlp_<Testmodules> shall be initialized */
    vItaHlp_SetupTestEnabled = VITAHLP_SETUPTEST_ENABLED;
  }
}

/**********************************************************************************************************************
 *  vItaHlp_GetVersionInfo()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, VITAHLP_APPL_VAR) versionInfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 vItaHlpError = VITAHLP_E_NO_ERROR;
  /* ----- Development Error Checks ------------------------------------- */
  if (versionInfo == NULL_PTR)
  {
    vItaHlpError = VITAHLP_E_PUBLIC_API_ERROR;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    versionInfo->vendorID = (VITAHLP_VENDOR_ID);
    versionInfo->moduleID = (VITAHLP_MODULE_ID);
    versionInfo->sw_major_version = (VITAHLP_SW_MAJOR_VERSION);
    versionInfo->sw_minor_version = (VITAHLP_SW_MINOR_VERSION);
    versionInfo->sw_patch_version = (VITAHLP_SW_PATCH_VERSION);
  }
#if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
  if (vItaHlpError != VITAHLP_E_NO_ERROR)
  {
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_GENERAL,
      VITAHLP_CMD_SERVICEIDENTIFIER_GET_VERSION_INFO,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_PUBLIC_API_ERROR,
      0u);
  }
#else
  VITAHLP_DUMMY_STATEMENT(vItaHlpError);
#endif
}

/**********************************************************************************************************************
 *  vItaHlp_MainFunction()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_MainFunction(void)
{
  /* Trigger standard procedure for watchdog service */
#if (VITAHLP_WDGSERVICE == STD_ON)
  vItaHlp_WdgService_MainFunction();
#endif

#if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
  /* During setup test no events shall be send and created */
  if (vItaHlp_SetupTestEnabled == VITAHLP_SETUPTEST_DISABLED)
  {
    /* Handle here the VitaEventWorker to process the event queue */
    vItaHlp_SendEvent();
  }

  /* Trigger main functions of configured test modules for Standard Tests */
# if (VITAHLP_FLS == STD_ON)
  vItaHlp_Fls_MainFunction();
# endif
#endif

  /* Trigger main functions of configured test modules */
#if (VITAHLP_CAN == STD_ON)
  vItaHlp_Can_MainFunction();
#endif
}

/**********************************************************************************************************************
 *  vItaHlp_TriggerSetupTest()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_TriggerSetupTest(void)
{
  /* Trigger setup tests of configured test modules */
  if (vItaHlp_SetupTestEnabled == VITAHLP_SETUPTEST_ENABLED)
  {
#if (VITAHLP_CAN == STD_ON)
    vItaHlp_Can_TriggerSetupTest();
#endif
#if (VITAHLP_ETH == STD_ON)
    vItaHlp_Eth_TriggerSetupTest();
#endif
#if (VITAHLP_LIN == STD_ON)
    vItaHlp_Lin_TriggerSetupTest();
#endif
  }
}

#if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_DispatchComponent()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_DispatchComponent(P2CONST(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 vItaHlpError = VITAHLP_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  if (pduInfoPtr == NULL_PTR)
  {
    vItaHlpError = VITAHLP_E_PUBLIC_API_ERROR;
  }
  else if (pduInfoPtr->SduLength < (VITAHLP_TESTCOMMUNICATION_MESSAGE_POS_COMPONENTIDENTIFIER + 1u))
  {
    vItaHlpError = VITAHLP_E_PUBLIC_API_ERROR;
  }
  else if (pduInfoPtr->SduLength < (VITAHLP_TESTCOMMUNICATION_MESSAGE_POS_SERVICEIDENTIFIER + 1u))
  {
    vItaHlpError = VITAHLP_E_PUBLIC_API_ERROR;
  }
  else if (pduInfoPtr->SduLength <
    (VITAHLP_TESTCOMMUNICATION_MESSAGE_POS_PAYLOAD_BEGIN + VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH))
  {
    vItaHlpError = VITAHLP_E_PUBLIC_API_ERROR;
  }
  else if (pduInfoPtr->SduLength != VITAHLP_TESTCOMMUNICATION_MESSAGE_LENGTH)
  {
    vItaHlpError = VITAHLP_E_PUBLIC_API_ERROR;
  }
  else if (pduInfoPtr->SduDataPtr == NULL_PTR)
  {
    vItaHlpError = VITAHLP_E_PUBLIC_API_ERROR;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    uint8 componentIdentifier;
    uint8 serviceIdentifier;
    P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadStartAddr;

    componentIdentifier = pduInfoPtr->SduDataPtr[VITAHLP_TESTCOMMUNICATION_MESSAGE_POS_COMPONENTIDENTIFIER];
    serviceIdentifier = pduInfoPtr->SduDataPtr[VITAHLP_TESTCOMMUNICATION_MESSAGE_POS_SERVICEIDENTIFIER];
    payloadStartAddr = &pduInfoPtr->SduDataPtr[VITAHLP_TESTCOMMUNICATION_MESSAGE_POS_PAYLOAD_BEGIN];

    /* Dispatch according to Component Identifier */
    switch (componentIdentifier)
    {
      case VITAHLP_COMPONENTIDENTIFIER_GENERAL:
        vItaHlp_DispatchService(serviceIdentifier, (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadStartAddr);
        break;
# if (VITAHLP_CAN == STD_ON)
      case VITAHLP_COMPONENTIDENTIFIER_CAN:
        vItaHlp_Can_DispatchService(serviceIdentifier, (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadStartAddr);
        break;
# endif
# if (VITAHLP_OS == STD_ON)
      case VITAHLP_COMPONENTIDENTIFIER_OS:
        vItaHlp_Os_DispatchService(serviceIdentifier, (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadStartAddr);
        break;
# endif
# if (VITAHLP_ETH == STD_ON)
      case VITAHLP_COMPONENTIDENTIFIER_ETH:
        vItaHlp_Eth_DispatchService(serviceIdentifier, (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadStartAddr);
        break;
# endif
# if (VITAHLP_FLS == STD_ON)
      case VITAHLP_COMPONENTIDENTIFIER_FLS:
        vItaHlp_Fls_DispatchService(serviceIdentifier, (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadStartAddr);
        break;
# endif
# if (VITAHLP_MCU == STD_ON)
      case VITAHLP_COMPONENTIDENTIFIER_MCU:
        vItaHlp_Mcu_DispatchService(serviceIdentifier);
        break;
# endif
# if (VITAHLP_LIN == STD_ON)
      case VITAHLP_COMPONENTIDENTIFIER_LIN:
        vItaHlp_Lin_DispatchService(serviceIdentifier, (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadStartAddr);
        break;
# endif
# if (VITAHLP_WDG == STD_ON)
      case VITAHLP_COMPONENTIDENTIFIER_WDG:
        vItaHlp_Wdg_DispatchService(serviceIdentifier, (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadStartAddr);
        break;
# endif
      default:
        vItaHlp_EventVItaHlpError(
          componentIdentifier,
          serviceIdentifier,
          VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
          VITAHLP_E_UNKNOWN_CMD,
          VITAHLP_TESTCOMMUNICATION_UNKNOWN_COMPONENTIDENTIFIER);
        break;
    }
  }
  if (vItaHlpError != VITAHLP_E_NO_ERROR)
  {
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_UNKNOWN,
      VITAHLP_CMD_SERVICEIDENTIFIER_UNKNOWN,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_PUBLIC_API_ERROR,
      0u);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_SetEvent()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_SetEvent(
    VAR(uint8, AUTOMATIC) componentIdentifier,
    VAR(uint8, AUTOMATIC) serviceIdentifier,
    P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadEventBuffer)
{
  /* ----- Local Variables ---------------------------------------------- */

  /* ----- Implementation ----------------------------------------------- */
  /* Events shall only be generated, if the setup test is disabled
   * or error events gets triggered (e.g. during start-up)
   * For the watchdog tests we have to pass at least all Wdg_AliveCounter
   * events at any time.
   */
  if ( (vItaHlp_SetupTestEnabled == VITAHLP_SETUPTEST_DISABLED) ||
    (serviceIdentifier >= VITAHLP_EVENT_SERVICEIDENTIFIER_VITAHLP_ERROR) ||
    (componentIdentifier == VITAHLP_COMPONENTIDENTIFIER_WDG))
  {
    vItaHlp_EventQueue_Enqueue(componentIdentifier, serviceIdentifier, payloadEventBuffer);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_EventVItaHlpError()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EventVItaHlpError(
  VAR(uint8, AUTOMATIC) componentIdentifier,
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  VAR(uint8, AUTOMATIC) isCmd,
  VAR(uint8, AUTOMATIC) returnCode,
  VAR(uint16, AUTOMATIC) optionalData)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 messageBuffer[VITAHLP_TESTCOMMUNICATION_MESSAGE_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  vItaHlp_CreateVItaHlpErrorMessage(
    messageBuffer,
    componentIdentifier,
    serviceIdentifier,
    isCmd,
    returnCode,
    optionalData);

  vItaHlp_SetEvent(
    VITAHLP_COMPONENTIDENTIFIER_GENERAL,
    VITAHLP_EVENT_SERVICEIDENTIFIER_VITAHLP_ERROR,
    (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) &messageBuffer[2u]);
}

/**********************************************************************************************************************
 *  vItaHlp_EventModuleUnderTestError()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EventModuleUnderTestError(
  VAR(uint16, AUTOMATIC) moduleId,
  VAR(uint8, AUTOMATIC) instanceId,
  VAR(uint8, AUTOMATIC) apiId,
  VAR(uint8, AUTOMATIC) returnCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  vItaHlp_CommonUtils_Uint16ToByteArrayBigEndian(payloadBuffer, moduleId);
  payloadBuffer[2u] = instanceId;
  payloadBuffer[3u] = apiId;
  payloadBuffer[4u] = returnCode;

  vItaHlp_SetEvent(
    VITAHLP_COMPONENTIDENTIFIER_GENERAL,
    VITAHLP_EVENT_SERVICEIDENTIFIER_MODULE_UNDER_TEST_ERROR,
    (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadBuffer);
}
#endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  APPLICATION SPECIFIC CALLOUT AND CALLBACK FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
*  vItaHlp_EcuM_CheckWakeup_Callout()
*********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EcuM_CheckWakeup_Callout(
  VAR(uint32, AUTOMATIC) wakeupSource)
{
  /* ----- Implementation ----------------------------------------------- */
# if (VITAHLP_LIN == STD_ON)
  vItaHlp_EcuM_Lin_CheckWakeup_Callout(wakeupSource);
# else
  VITAHLP_DUMMY_STATEMENT(wakeupSource);
# endif /* (VITAHLP_LIN == STD_ON) */
}

/**********************************************************************************************************************
*  vItaHlp_EcuM_SetWakeupEvent_Callout()
*********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EcuM_SetWakeupEvent_Callout(
  VAR(uint32, AUTOMATIC) wakeupSource)
{
  /* ----- Implementation ----------------------------------------------- */
# if (VITAHLP_LIN == STD_ON)
  vItaHlp_EcuM_Lin_SetWakeupEvent_Callout(wakeupSource);
# else
  VITAHLP_DUMMY_STATEMENT(wakeupSource);
# endif /* (VITAHLP_LIN == STD_ON) */
}

#if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_DetReportError()
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VITAHLP_CODE) vItaHlp_Det_ReportError_Callout(
  VAR(uint16, AUTOMATIC) moduleId,
  VAR(uint8, AUTOMATIC) instanceId,
  VAR(uint8, AUTOMATIC) apiId,
  VAR(uint8, AUTOMATIC) errorId)
{
  /* Send DET event */
  vItaHlp_EventDetError(moduleId, instanceId, apiId, errorId);
  return E_OK;
}

/**********************************************************************************************************************
 *  vItaHlp_DetReportRuntimeError()
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VITAHLP_CODE) vItaHlp_Det_ReportRuntimeError_Callout(
  VAR(uint16, AUTOMATIC) moduleId,
  VAR(uint8, AUTOMATIC) instanceId,
  VAR(uint8, AUTOMATIC) apiId,
  VAR(uint8, AUTOMATIC) errorId)
{
  /* No action provided */
  VITAHLP_DUMMY_STATEMENT(moduleId);
  VITAHLP_DUMMY_STATEMENT(instanceId);
  VITAHLP_DUMMY_STATEMENT(apiId);
  VITAHLP_DUMMY_STATEMENT(errorId);
  return E_OK;
}

/**********************************************************************************************************************
 *  ApplTestComDataReceivedCbk()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) ApplTestComDataReceivedCbk(P2CONST(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr)
{
  /* no verification of pointer since this is done by vItaHlp_DispatchComponent */
  vItaHlp_DispatchComponent(pduInfoPtr);
}
#endif  /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

#define VITAHLP_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp.c
 *********************************************************************************************************************/
