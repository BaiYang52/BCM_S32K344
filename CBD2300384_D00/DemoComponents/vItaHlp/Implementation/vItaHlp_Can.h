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
/**        \file  vItaHlp_Can.h
 *        \brief  Service API declaration for CAN test module.
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

#if !defined (VITAHLP_CAN_H)
# define VITAHLP_CAN_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vItaHlp_Cfg.h"
# include "vItaHlp_Can_Cfg.h"
# include "CanIf.h"
# include "CanIf_Cbk.h"

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
 *  vItaHlp_Can_Init()
 *********************************************************************************************************************/
/*! \brief       Initialize CAN test module.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by vItaHlp_Init
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Can_Init(void);

/**********************************************************************************************************************
 *  vItaHlp_Can_MainFunction()
 *********************************************************************************************************************/
/*! \brief       Main Function of vItaHlp_Can.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by vItaHlp_MainFunction
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Can_MainFunction(void);

/**********************************************************************************************************************
 *  vItaHlp_Can_TriggerSetupTest()
 *********************************************************************************************************************/
/*! \brief       Execute CAN setup test.
 *  \details     Transmit one CAN message frame and increment each byte of the payload afterwards.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Can_TriggerSetupTest(void);

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Can_DispatchService()
 *********************************************************************************************************************/
/*! \brief       Dispatches the test communication message based on the serviceIdentifier.
 *  \details     Evaluates the service identifier of the CAN test communication command and calls the specific
 *               command function for further processing.
 *  \param[in]   serviceIdentifier     Identifier of the requested service.
 *  \param[in]   payloadData           Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Can_DispatchService(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_CanIf_RxIndication_Callout()
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when CAN message is received.
 *  \details     The CAN driver notifies CanIf by CanIf_RxIndication, when a message frame is received. The CanIf
 *               stub calls this user callback function accordingly.
 *               The number and types of input parameters is based on the corresponding MSR implementation of the
 *               CanIf_RxIndication (different to ASR).
 *  \param[in]   hrh             Hardware handle the PDU was received in.
 *  \param[in]   canId           CAN identifier of the received PDU.
 *  \param[in]   canDlc          Data length code of the received PDU.
 *  \param[in]   canSduPtr       Pointer to hardware or temporary buffer containing the data of the received PDU.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_CanIf_RxIndication_Callout(
  VAR(CanIf_HwHandleType, AUTOMATIC) hrh,
  VAR(Can_IdType, AUTOMATIC) canId,
  VAR(uint8, AUTOMATIC) canDlc,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) canSduPtr);

/**********************************************************************************************************************
 *  vItaHlp_CanIf_TxConfirmation_Callout
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when CAN message is transmitted.
 *  \details     The CAN driver notifies CanIf by CanIf_TxConfirmation, when a message frame is successfully sent.
 *               The CanIf stub calls this user callback function accordingly.
 *  \param[in]   canTxPduId             ID of the successfully transmitted PDU.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_CanIf_TxConfirmation_Callout(VAR(PduIdType, AUTOMATIC) canTxPduId);

/**********************************************************************************************************************
 *  vItaHlp_CanIf_ControllerModeIndication_Callout
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when desired CAN mode is changed.
 *  \details     The CAN driver notifies CanIf by CanIf_ControllerModeIndication, when the CAN Controller transitions
 *               to a new mode.
 *               The CanIf stub calls this user callback function accordingly.
 *  \param[in]   controllerId           Channel where the mode transition happened.
 *  \param[in]   controllerMode         Controller mode to which the CAN controller transitioned.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_CanIf_ControllerModeIndication_Callout(
  VAR(uint8, AUTOMATIC) controllerId,
  VAR(CanIf_ControllerModeType, AUTOMATIC) controllerMode);

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h"

#endif /* VITAHLP_CAN_H */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_Can.h
 *********************************************************************************************************************/
