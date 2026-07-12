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
/**        \file  vItaHlp_ComIf.h
 *        \brief  Service API declaration for the vItaHlp ComIf.
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

#if !defined (VITAHLP_COMIF_H)
# define VITAHLP_COMIF_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vItaHlp_Cfg.h"
# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
#  include "Std_Types.h"
#  include "ComStack_Types.h"
#  include "vItaHlp_TestCom.h"
/* Link layer specific includes */
#  if ( (VITAHLP_CAN == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN) )
#   include "CanIf_Cbk.h"
#   include "CanIf.h"
#  endif
#  if ( (VITAHLP_ETH == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH) )
#   include "EthIf_Cbk.h"
#  endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#  define VITAHLP_START_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#  if ( (VITAHLP_CAN == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN) )
/**********************************************************************************************************************
 *  vItaHlp_ComIf_CanIfRxIndication_Callout()
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when CAN message is received.
 *  \details     The CAN driver notifies CanIf by CanIf_RxIndication, when a message frame is received. The CanIf
 *               stub calls this user callback function accordingly.
 *               The number and types of input parameters is based on the corresponding MSR implementation of the
 *               CanIf_RxIndication (different to ASR).
 *               The ComIf dispatches the callout. The indication is forwarded to the vItaHlp_TestCom layer if
 *               the test communication is configured via CAN. Otherwise, or if the received indication is
 *               not processed by the vItaHlp_TestCom layer, the indication is forwarded to the CAN module under test
 *               of vItaHlp.
 *  \param[in]   hrh             Hardware handle the PDU was received in.
 *  \param[in]   canId           CAN identifier of the received PDU.
 *  \param[in]   canDlc          Data length code of the received PDU.
 *  \param[in]   canSduPtr       Pointer to hardware or temporary buffer containing the data of the received PDU.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication via CAN is enabled or CAN is used as module under test for vItaHlp.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_ComIf_CanIfRxIndication_Callout(
  VAR(CanIf_HwHandleType, AUTOMATIC) hrh,
  VAR(Can_IdType, AUTOMATIC) canId,
  VAR(uint8, AUTOMATIC) canDlc,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) canSduPtr);

/**********************************************************************************************************************
 *  vItaHlp_ComIf_CanIfTxConfirmation_Callout
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when CAN message is transmitted.
 *  \details     The CAN driver notifies CanIf by CanIf_TxConfirmation, when a message frame is successfully sent.
 *               The CanIf stub calls this user callback function accordingly.
 *               The ComIf dispatches the callout. If the received confirmation is not used by the vItaHlp_TestCom
 *               layer it is forwarded to the CAN module under test of vItaHlp.
 *  \param[in]   canTxPduId             ID of the successfully transmitted PDU.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication via CAN is enabled or CAN is used as module under test for vItaHlp.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_ComIf_CanIfTxConfirmation_Callout(VAR(PduIdType, AUTOMATIC) canTxPduId);

/**********************************************************************************************************************
 *  vItaHlp_ComIf_CanIfControllerModeIndication_Callout
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when desired CAN mode is changed.
 *  \details     The CAN driver notifies CanIf by CanIf_ControllerModeIndication, when the CAN Controller transitions
 *               to a new mode.
 *               The CanIf stub calls this user callback function accordingly.
 *               The indication is forwarded to the vItaHlp_TestCom and CAN module under test of vItaHlp.
 *  \param[in]   controllerId           Channel where the mode transition happened.
 *  \param[in]   controllerMode         Controller mode to which the CAN controller transitioned.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication via CAN is enabled or CAN is used as module under test for vItaHlp.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_ComIf_CanIfControllerModeIndication_Callout(
  VAR(uint8, AUTOMATIC) controllerId,
  VAR(CanIf_ControllerModeType, AUTOMATIC) controllerMode);
#  endif /* ( (VITAHLP_CAN == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN) ) */

#  if ( (VITAHLP_ETH == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH) )
/**********************************************************************************************************************
 *  vItaHlp_ComIf_EthIfTxConfirmation_Callout()
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when Eth frame is transmitted.
 *  \details     The Eth driver notifies EthIf by EthIf_TxConfirmation, when a message frame is successfully sent.
 *               The EthIf stub calls this user callback function accordingly.
 *  \param[in]   ctrlIdx            Identifier of the Eth Controller which is unique within one Eth Driver module.
 *  \param[in]   bufIdx             Identifier of the buffer resource which was used for the transmission
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication via Ethernet is enabled or Ethernet is used as module under test for vItaHlp.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_ComIf_EthIfTxConfirmation_Callout(
  VAR(uint8, AUTOMATIC) ctrlIdx,
  VAR(uint8, AUTOMATIC) bufIdx);

/**********************************************************************************************************************
 *  vItaHlp_ComIf_EthIfRxIndication_Callout()
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when Eth frame is received.
 *  \details     The Eth driver notifies EthIf by EthIf_RxIndication, when a message frame is successfully received.
 *               The EthIf stub calls this user callback function accordingly.
 *               The ComIf dispatches the callout. The indication is forwarded to the vItaHlp_TestCom layer if
 *               the test communication is configured via Ethernet. Otherwise, or if the received indication is
 *               not processed by the vItaHlp_TestCom layer, the indication is forwarded
 *               to the Ethernet module under test of vItaHlp.
 *  \param[in]   ctrlIdx            Identifier of the Eth Controller which is unique within one Eth Driver module.
 *  \param[in]   frameType          Ethernet type, according to type field of IEEE802.3.
 *  \param[in]   isBroadcast        Indicates if the received frame is a broadcast frame.
 *  \param[in]   physAddrPtr        MAC address of the received frame.
 *  \param[in]   dataPtr            Pointer to the received frame.
 *  \param[in]   lenByte            Length of the received frame.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication via Ethernet is enabled or Ethernet is used as module under test for vItaHlp.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_ComIf_EthIfRxIndication_Callout(
  VAR(uint8, AUTOMATIC) ctrlIdx,
  VAR(Eth_FrameType, AUTOMATIC) frameType,
  VAR(boolean, AUTOMATIC) isBroadcast,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) physAddrPtr,
  P2VAR(Eth_DataType, AUTOMATIC, VITAHLP_APPL_VAR) dataPtr,
  VAR(uint16, AUTOMATIC) lenByte);
#  endif /* ( (VITAHLP_ETH == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH) ) */

#  define VITAHLP_STOP_SEC_CODE
#  include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

#endif /* VITAHLP_COMIF_H */
