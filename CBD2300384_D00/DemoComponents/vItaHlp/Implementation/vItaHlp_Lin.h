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
/**        \file  vItaHlp_Lin.h
 *        \brief  Service API declaration for Lin test module.
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

#if !defined (VITAHLP_LIN_H)
# define VITAHLP_LIN_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vItaHlp_Cfg.h"
# include "vItaHlp_Lin_Cfg.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  vItaHlp_Lin_Init()
 *********************************************************************************************************************/
/*! \brief       Initialize Lin test module.
 *  \details     Initializes all driver channel pairings that are configured and deposited in the channelMap table.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by vItaHlp_Init
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Lin_Init(void);

/**********************************************************************************************************************
 *  vItaHlp_Lin_TriggerSetupTest()
 *********************************************************************************************************************/
/*! \brief       Execute Lin setup test.
 *  \details     Transmit one Lin message frame on every configured Lin channel (on every configured Lin driver).
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Lin_TriggerSetupTest(void);

/**********************************************************************************************************************
 *  vItaHlp_EcuM_Lin_CheckWakeup_Callout()
 *********************************************************************************************************************/
/*! \brief       vItaHlp component specific callback, when Lin wakeup pulse is noticed.
 *  \details     The Lin driver notifies EcuM by EcuM_CheckWakeup, when a wakeup pulse is noticed.
 *               The EcuM stub calls this user callback function accordingly.
 *  \param[in]   wakeupSource       Wakeup source of the wakeup pulse.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EcuM_Lin_CheckWakeup_Callout(
  VAR(uint32, AUTOMATIC) wakeupSource);

/**********************************************************************************************************************
 *  vItaHlp_EcuM_Lin_SetWakeupEvent_Callout()
 *********************************************************************************************************************/
/*! \brief       vItaHlp component specific callback, when Lin wakeup pulse is received.
 *  \details     The Lin driver notifies EcuM by EcuM_SetWakeupEvent, when a wakeup pulse is successfully received.
 *               The EcuM stub calls this user callback function accordingly.
 *  \param[in]   wakeupSource       Wakeup source of the wakeup pulse.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EcuM_Lin_SetWakeupEvent_Callout(
  VAR(uint32, AUTOMATIC) wakeupSource);

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Lin_DispatchService()
 *********************************************************************************************************************/
/*! \brief       Dispatches the test communication message based on the serviceIdentifier.
 *  \details     Evaluates the service identifier of the Lin test communication command and calls the specific
 *               command function for further processing.
 *  \param[in]   serviceIdentifier  Identifier of the requested service.
 *  \param[in]   payloadData        Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Lin_DispatchService(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_LinIf_WakeupConfirmation_Callout()
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when external Lin wakeup pulse is received.
 *  \details     The LIN driver notifies LinIf by LinIf_WakeupConfirmation, when a wakeup pulse is successfully received.
 *               The LinIf stub calls this user callback function accordingly.
 *  \param[in]   wakeupSource       Wakeup source of the wakeup pulse.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_LinIf_WakeupConfirmation_Callout(
  VAR(uint32, AUTOMATIC) wakeupSource);

#  if (VITAHLP_LINSLAVE == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_LinIf_HeaderIndication_Callout()
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when a LIN frame header is received on LIN slave.
 *  \details     The LIN driver notifies LinIf by LinIf_HeaderIndication, when a LIN frame header from LIN Master is
 *               received.
 *  \param[in]   channel            Identification of the LIN channel.
 *  \param[in]   pduPtr             Pointer to PDU providing the received PID and pointer to the SDU data buffer
 *                                  as in parameter.
 *  \return      This function returns always with E_OK.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled. At least one LIN Slave node configured.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VITAHLP_CODE) vItaHlp_LinIf_HeaderIndication_Callout(
  VAR(NetworkHandleType, AUTOMATIC) channel,
  P2VAR(Lin_PduType, AUTOMATIC, VITAHLP_APPL_VAR) pduPtr);

/**********************************************************************************************************************
 *  vItaHlp_LinIf_TxConfirmation_Callout()
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when a response from LIN Slave is transmitted succesfully.
 *  \details     The LIN driver notifies LinIf by LinIf_TxConfirmation, when a response from LIN Slave is successfully
 *               sent.
 *  \param[in]   channel            Identification of the LIN channel.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled. At least one LIN Slave node configured.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_LinIf_TxConfirmation_Callout(
  VAR(NetworkHandleType, AUTOMATIC) channel);

/**********************************************************************************************************************
 *  vItaHlp_LinIf_RxIndication_Callout()
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when a LIN frame is received on LIN slave.
 *  \details     The LIN driver notifies LinIf by LinIf_RxIndication, when a LIN frame from LIN Master is received.
 *  \param[in]   channel            Identification of the LIN channel.
 *  \param[in]   sduPtr             Pointer to a buffer where the current SDU is stored.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled. At least one LIN Slave node configured.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_LinIf_RxIndication_Callout(
  VAR(NetworkHandleType, AUTOMATIC) channel,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) sduPtr);

/**********************************************************************************************************************
 *  vItaHlp_LinIf_LinErrorIndication_Callout()
 *********************************************************************************************************************/
/*! \brief       User application specific callback, to report errors of a LIN Slave channel.
 *  \details     The LIN driver notifies LinIf by LinIf_LinErrorIndication, to report a detected error during header
 *               or response processing of a LIN Slave channel.
 *  \param[in]   channel            Identification of the LIN channel.
 *  \param[in]   errorStatus        Type of detected error.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled. At least one LIN Slave node configured.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_LinIf_LinErrorIndication_Callout(
  VAR(NetworkHandleType, AUTOMATIC) channel,
  VAR(Lin_SlaveErrorType, AUTOMATIC) errorStatus);
#  endif /* (VITAHLP_LINSLAVE == STD_ON) */

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h"

#endif /* VITAHLP_LIN_H */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_Lin.h
 *********************************************************************************************************************/
