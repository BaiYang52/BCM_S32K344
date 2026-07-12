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
/**        \file  vItaHlp.h
 *        \brief  Contains the service API for vItaHlp.
 *
 *      \details  -
 *
 *    \attention  Please note:
 *                The demo and example programs only show special aspects of the software. With regard to the fact
 *                that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *                expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  AUTHOR IDENTITY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Name                          Initials      Company
 *  -------------------------------------------------------------------------------------------------------------------
 *  Philipp Kallenberg            vispkg        Vector Informatik GmbH
 *  Peter Pokorny                 visppy        Vector Informatik GmbH
 *  Stefan Rombach                visros        Vector Informatik GmbH
 *  Thomas Urban                  vistun        Vector Informatik GmbH
 *  Andreas Zenk                  virazk        Vector Informatik GmbH
 *  Till Fauth                    vistif        Vector Informatik GmbH
 *  Tom Wessels                   viswom        Vector Informatik GmbH
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  03.00.00  2019-02-13  visppy  FEAT-5320     vItaHlp Basic Functionality
 *                        visros  FEAT-5312     Testmodule CAN - Setup Test
 *                        visppy  FEAT-5315     Testmodule CAN - Standard Test for Communication
 *                        visros  FEAT-5313     Test Communication Link Layer CAN support
 *                        vispkg  FEAT-5314     Test Communication Link Layer TCM support
 *  03.00.01  2019-03-27  visros  WORKITEM22082 Rename vItaHlp MemMap and Compiler to indicate that it is a template
 *  04.00.00  2019-04-23  visppy  FEAT-5933     Testmodule OS - Test Communication Multi Core
 *                        visppy  FEAT-5665     Testmodule OS - Standard Test for Core StartUp
 *                        vistun  FEAT-5927     Testmodule OS - Standard Test for Counter
 *  04.01.00  2019-07-05  vispkg  FEAT-5582     Testmodule Eth - Setup Test
 *                        visppy  FEAT-5583     Testmodule Eth - Standard Test for Transmission
 *                        vislit  FEAT-6149     Testmodule ETH - Standard Test for Reception
 *  05.00.00  2019-08-28  virazk  FEAT-6188     Testmodule FLS - Standard Test Write Flash Memory
 *                        visppy  FEAT-6189     Testmodule FLS - Standard Test Read Flash Memory
 *                        visppy  FEAT-6192     Testmodule FLS - Standard Test Compare Flash Memory
 *            2019-10-01  visppy  FEAT-6190     Testmodule MCU - Standard Test Perform Reset
 *                        visppy  FEAT-6191     Testmodule MCU - Standard Test Clock Timing
 *  05.01.00  2019-11-26  visppy  PTSC-45       Testmodule FLS - Job Result Polling Support
 *            2019-12-06  visppy  PTSC-86       Testmodule FLS - Remove Locking Mechanism
 *            2019-12-11  vistif  PTSC-62       Testmodule LIN - Standard Test Master Response
 *                        vistif  PTSC-63       Testmodule LIN - Standard Test Slave Response
 *                        vistif  PTSC-64       Testmodule LIN - Standard Test Master Wakeup
 *                        visppy  PTSC-65       Testmodule LIN - Standard Test Slave Wakeup
 *            2020-01-31  viswom  PTSC-92       Testmodule WDG - Standard Test Immediate Reset
 *                        visppy  PTSC-93       Testmodule WDG - Standard Test Disable Allowed
 *                        visppy  PTSC-94       Testmodule WDG - Standard Test Disable Not Allowed
 *                        visppy  PTSC-95       Testmodule WDG - Standard Test Timing Reset
 *            2020-02-20  visppy  PTSC-51       Testmodule FLS - Standard Test Erase and BlankCheck
 *  05.01.01  2020-03-11  visppy  PTSC-166      Lin Driver ASR Abstraction (4.3 vs 4.4)
 *  06.00.00  2020-03-10  visppy  PTSC-158      Testmodule LIN Slave - Standard Test Slave Response
 *            2020-03-19  visppy  PTSC-159      Testmodule LIN Slave - Standard Test Master Response
 *            2020-03-26  visppy  PTSC-160      Testmodule LIN Slave - Standard Test Ignore Response
 *            2020-03-27  visppy  PTSC-161      Testmodule LIN Slave - Standard Test Master Wakeup
 *            2020-03-31  visppy  PTSC-162      Testmodule LIN Slave - Standard Test Slave Wakeup
 *            2020-04-07  visppy  ESCAN00106058 Det_ReportRuntimeError not handled in vItaHlp
 *            2020-04-24  visppy  PTSC-224      Improve include hierarchy for test module files
 *            2020-04-27  visppy  PTSC-226      Improve Transmission Test Handling for CAN
 *            2020-05-12  visppy  ESCAN00106179 Compiler error: Undeclared identifier 'flsGetJobResultFctPtr'
 *  06.00.01  2020-09-01  visppy  ESCAN00107134 Compiler error: Cannot open source file "Eth.h"
 *  06.00.02  2021-03-12  visppy  ESCAN00107256 Compiler error: Eth_ModeType is unknown
 *            2021-03-15  visppy  ESCAN00108733 Compiler error: Missing infix for header include in vItaHlp_TestCom_Cfg.h
 *********************************************************************************************************************/

#if !defined (VITAHLP_H)
# define VITAHLP_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vItaHlp_Cfg.h"

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
#  include "ComStack_Types.h"
# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define VITAHLP_VENDOR_ID                                                     (30u)
# define VITAHLP_MODULE_ID                                                 (0xF1DAu)

/* ----- Component version information (decimal version of ALM implementation package) ----- */
# define VITAHLP_SW_MAJOR_VERSION                                               (6u)
# define VITAHLP_SW_MINOR_VERSION                                               (0u)
# define VITAHLP_SW_PATCH_VERSION                                               (2u)

/* ------ Definitions used for the test communication protocol for all test modules ------ */
# define VITAHLP_TESTCOMMUNICATION_MESSAGE_LENGTH                               (8u)
# define VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH                               (6u)

/* Define for flag which indicates if the SI is used for a command or an event */
# define VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_EVENT      (0u)
# define VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD        (1u)

/* Define for flag which indicates if the component or service identifier is unknown */
# define VITAHLP_TESTCOMMUNICATION_UNKNOWN_COMPONENTIDENTIFIER  (0u)
# define VITAHLP_TESTCOMMUNICATION_UNKNOWN_SERVICEIDENTIFIER    (1u)

/* Return codes for vItaHlp errors */
# define VITAHLP_E_NO_ERROR                                    (0x00u)
# define VITAHLP_E_PUBLIC_API_ERROR                            (0x01u)
# define VITAHLP_E_INVALID_REQUESTED_PARAM                     (0x02u)
# define VITAHLP_E_INVALID_UNEXPECTED_CALLBACK_RECEIVED        (0x03u)
# define VITAHLP_E_INVALID_CONFIGURATION                       (0x04u)
# define VITAHLP_E_SERVICE_BUSY                                (0x05u)
# define VITAHLP_E_EVENT_OVERFLOW                              (0xFEu)
# define VITAHLP_E_UNKNOWN_CMD                                 (0xFFu)

/* Definitions of API function IDs and module ID from AUTOSAR specification of OS component */
# define VITAHLP_OS_SID_ACTIVATETASK_ID                     (0xD9u)
# define VITAHLP_OS_SID_SETRELALARM_ID                      (0xE5u)
# define VITAHLP_OS_SID_CANCELALARM_ID                      (0xE7u)
# define VITAHLP_OS_MODULE_ID                               (0x01u)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* Byte 0: CI - Component Identifier */
typedef enum
{
  VITAHLP_COMPONENTIDENTIFIER_GENERAL = 0u,
  VITAHLP_COMPONENTIDENTIFIER_CAN     = 1u,
  VITAHLP_COMPONENTIDENTIFIER_OS      = 2u,
  VITAHLP_COMPONENTIDENTIFIER_ETH     = 3u,
  VITAHLP_COMPONENTIDENTIFIER_FLS     = 4u,
  VITAHLP_COMPONENTIDENTIFIER_MCU     = 5u,
  VITAHLP_COMPONENTIDENTIFIER_LIN     = 6u,
  VITAHLP_COMPONENTIDENTIFIER_WDG     = 7u,
  VITAHLP_COMPONENTIDENTIFIER_UNKNOWN = 0xFFu
} vItaHlp_TestCommunicationComponentIdentifierType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  vItaHlp_PreInit()
 *********************************************************************************************************************/
/*! \brief       Initializes the vItaHlp module.
 *  \details     Initialization of internal Det/Dem Error report mechanism.
 *               Support user defined function code for vItaHlp_Eth test module.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_PreInit(void);

/**********************************************************************************************************************
 *  vItaHlp_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization for vItaHlp and its configured test modules.
 *  \details     Depending on enabled test modules, the function initializes general internal parameters used by
 *               the test modules.
 *               Start of communication module driver.
 *  \pre         PreInit function of module has been called before.
 *  \pre         Function must be called if OS is already started.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Init(void);

/**********************************************************************************************************************
 *  vItaHlp_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information.
 *  \details     vItaHlp_GetVersionInfo() returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo             Pointer to where to store the version information. Parameter must not be NULL.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, VITAHLP_APPL_VAR) versionInfo);

/**********************************************************************************************************************
 *  vItaHlp_MainFunction()
 *********************************************************************************************************************/
/*! \brief       Main Function of vItaHlp.
 *  \details     Triggers the setup test and the main function of the configured test modules.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Needs to be called cyclic in 10ms intervals by application specific task.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_MainFunction(void);

/**********************************************************************************************************************
 *  vItaHlp_TriggerSetupTest()
 *********************************************************************************************************************/
/*! \brief       Function triggers the setup test of the configured test modules.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Needs to be called cyclic in 500ms intervals by application specific task.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_TriggerSetupTest(void);

/**********************************************************************************************************************
 *  vItaHlp_EcuM_CheckWakeup_Callout()
 *********************************************************************************************************************/
/*! \brief       Function passes the incoming wakeup source to the vItaHlp component specific CheckWakeup functions.
 *  \details     The component driver notifies EcuM by EcuM_CheckWakeup, when a wakeup pulse is noticed.
 *  \param[in]   wakeupSource   Wakeup source for which the wakeup check is to be performed.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EcuM_CheckWakeup_Callout(
  VAR(uint32, AUTOMATIC) wakeupSource);

/**********************************************************************************************************************
*  vItaHlp_EcuM_SetWakeupEvent_Callout()
*********************************************************************************************************************/
/*! \brief       Function passes the incoming wakeup source to the vItaHlp component specific SetWakeupEvent functions.
*  \details     The component driver notifies EcuM by EcuM_SetWakeupEvent, when a wakeup pulse is successfully received.
*  \param[in]   wakeupSource   Wakeup source for which the component specific wakeup sequences are performed.
*  \pre         -
*  \context     TASK|ISR
*  \reentrant   TRUE
*  \synchronous TRUE
*********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EcuM_SetWakeupEvent_Callout(
  VAR(uint32, AUTOMATIC) wakeupSource);

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_DispatchComponent()
 *********************************************************************************************************************/
/*! \brief       Dispatches an incoming test communication command.
 *  \details     The function dispatches the incoming test communication command based on the component identifier
 *               and forwards the command to the corresponding component.
 *               The function is called when a control message is received e.g. via TCM board.
 *  \param[in]   pduInfoPtr           Pointer to the service request information.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_DispatchComponent(P2CONST(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr);

/**********************************************************************************************************************
 *  vItaHlp_SetEvent()
 *********************************************************************************************************************/
/*! \brief       Sets an event to the internal event queue which is processed cyclically.
 *  \details     -
 *  \param[in]   componentIdentifier      Identifier of the requested component.
 *  \param[in]   serviceIdentifier        Identifier of the requested service.
 *  \param[in]   payloadEventBuffer       Pointer which contains the payload content for an event.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_SetEvent(
  VAR(uint8, AUTOMATIC) componentIdentifier,
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadEventBuffer);

/**********************************************************************************************************************
 *  vItaHlp_EventVItaHlpError()
 *********************************************************************************************************************/
/*! \brief       Function creates event content for an error of vItaHlp during processing of a command or event.
 *  \details     -
 *  \param[in]   componentIdentifier Identifier of the component.
 *  \param[in]   serviceIdentifier   Identifier of the service.
 *  \param[in]   isCmd               0: SI is given for an event 1: SI is given for a command.
 *  \param[in]   returnCode          return code which is defined for vItaHlp in header file.
 *  \param[in]   optionalData        optional data which can be used for further information for the given return code
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EventVItaHlpError(
  VAR(uint8, AUTOMATIC) componentIdentifier,
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  VAR(uint8, AUTOMATIC) isCmd,
  VAR(uint8, AUTOMATIC) returnCode,
  VAR(uint16, AUTOMATIC) optionalData);

/**********************************************************************************************************************
 *  vItaHlp_EventModuleUnderTestError()
 *********************************************************************************************************************/
/*! \brief       Function creates event content for an error by calling an extern service API of the
 *               module under test.
 *  \details     -
 *  \param[in]   moduleId      AUTOSAR Module ID of the module which causes the error.
 *  \param[in]   instanceId    AUTOSAR Instance ID of the module.
 *  \param[in]   apiId         AUTOSAR API ID of the module.
 *  \param[in]   returnCode    AUTOSAR return code of the API which results in an error.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EventModuleUnderTestError(
  VAR(uint16, AUTOMATIC) moduleId,
  VAR(uint8, AUTOMATIC) instanceId,
  VAR(uint8, AUTOMATIC) apiId,
  VAR(uint8, AUTOMATIC) returnCode);

/**********************************************************************************************************************
 *  vItaHlp_Det_ReportError_Callout()
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when DET error is reported.
 *  \details     Function triggers an event which contains the DET related information.
 *  \param[in]   moduleId   Module ID for which the DET occurred.
 *  \param[in]   instanceId Instance ID of the module for which the DET occurred.
 *  \param[in]   apiId      API ID of the module for which the DET occurred.
 *  \param[in]   errorId    Error ID of the module for which the DET occurred.
 *  \return      returns always E_OK to the upper layer
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VITAHLP_CODE) vItaHlp_Det_ReportError_Callout(
  VAR(uint16, AUTOMATIC) moduleId,
  VAR(uint8, AUTOMATIC) instanceId,
  VAR(uint8, AUTOMATIC) apiId,
  VAR(uint8, AUTOMATIC) errorId);

/**********************************************************************************************************************
 *  vItaHlp_Det_ReportRuntimeError_Callout()
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when DET runtime error is reported.
 *  \details     The function currently returns immediately with E_OK.
 *  \param[in]   moduleId   Module ID for which the runtime error occurred.
 *  \param[in]   instanceId Instance ID of the module for which the runtime error occurred.
 *  \param[in]   apiId      API ID of the module for which the runtime error occurred.
 *  \param[in]   errorId    Error ID of the module for which the runtime error occurred.
 *  \return      returns always E_OK to the upper layer
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VITAHLP_CODE) vItaHlp_Det_ReportRuntimeError_Callout(
  VAR(uint16, AUTOMATIC) moduleId,
  VAR(uint8, AUTOMATIC) instanceId,
  VAR(uint8, AUTOMATIC) apiId,
  VAR(uint8, AUTOMATIC) errorId);
# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* VITAHLP_H */

/**********************************************************************************************************************
 *  EXCLUSIVE AREAS OF THE MODULE
 *********************************************************************************************************************/
/*
 * \exclusivearea VITAHLP_CAN_EXCLUSIVE_AREA
 * Ensures consistency while validate/modify the global flag which indicates if the sleep/start sequence is active.
 * Ensures consistency for vItaHlp_Can_DriverUnderTest/vItaHlp_Can_ControllerUnderTest if sleep start
 * sequence is active.
 * Reading of vItaHlp_Can_DriverUnderTest/vItaHlp_Can_ControllerUnderTest is always protected by checking the state
 * of vItaHlp_Can_SleepStartSequenceActive.
 * \protects vItaHlp_Can_SleepStartSequenceActive
 * \usedin vItaHlp_Can_Sleep_Start
 * \exclude vItaHlp_Can_Init (startup-phase is deterministic and only executed once),
 *          vItaHlp_Can_SleepStartSequenceControllerModeIndication (state machine for controller handling in
 *          function shall only be executed if sleep/start sequence is active and mode indication for controller
 *          is triggered before in vItaHlp_Can_Sleep_Start. If mode indication is triggered in vItaHlp_Can_Sleep_Start
 *          it is ensured by the exclusive area, that the protected variable is already set to TRUE.
 * \length SHORT This exclusive area covers the check and setting of variables.
 * \endexclusivearea
 */

/*
 * \exclusivearea VITAHLP_EVENTQUEUE_EXCLUSIVE_AREA
 * Ensures consistency of global event queue elements during validation and modification. All elements must be in
 * consistent to each other.
 * \protects vItaHlp_EventQueue_NumberOfStoredEvents vItaHlp_EventQueue_Queue
 * \protects vItaHlp_EventQueue_NumberOfStoredEvents
 * \protects vItaHlp_EventQueue_ReadIndex
 * \protects vItaHlp_EventQueue_WriteIndex
 * \protects vItaHlp_EventQueue_OverflowCounter
 * \usedin vItaHlp_EventQueue_IsEmpty, vItaHlp_EventQueue_Dequeue, vItaHlp_EventQueue_Enqueue
 * \exclude vItaHlp_EventQueue_Init, function is called during startup phase,
 *          whereas other functions are called during runtime
 *          vItaHlp_EventQueue_Init must be called and executed prior to other modules service functions
 * \length MEDIUM This exclusive area covers the check and setting of variables, length depends on size of event queue.
 * \endexclusivearea
 */

/*
 * \exclusivearea VITAHLP_TESTCOM_EXCLUSIVE_AREA
 * Ensures consistency of global link status flag during modification. No need of protection during reading of this
 * flag, since we have no "read, modify, write" operation at all.
 * \protects vItaHlp_TestComLinkStatus
 * \usedin vItaHlp_TestCom_SetLinkStatus
 * \exclude vItaHlp_TestCom_GetLinkStatus
 * \length SHORT Covers only writing of variable
 * \endexclusivearea
 */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp.h
 *********************************************************************************************************************/
