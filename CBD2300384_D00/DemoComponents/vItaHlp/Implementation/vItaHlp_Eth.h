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
/**        \file  vItaHlp_Eth.h
 *        \brief  Service API declaration for Eth test module.
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

#if !defined (VITAHLP_ETH_H)
# define VITAHLP_ETH_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vItaHlp_Cfg.h"
# include "vItaHlp_Eth_Cfg.h"

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
 *  vItaHlp_Eth_Init()
 *********************************************************************************************************************/
/*! \brief       Initialize Eth test module.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \note        Called by vItaHlp_Init
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Eth_Init(void);

/**********************************************************************************************************************
 *  vItaHlp_Eth_TriggerSetupTest()
 *********************************************************************************************************************/
/*! \brief       Execute Eth setup test.
 *  \details     Transmit one Eth message frame on every configured Eth channel.
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Eth_TriggerSetupTest(void);

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Eth_DispatchService()
 *********************************************************************************************************************/
/*! \brief       Dispatches the test communication message based on the serviceIdentifier.
 *  \details     Evaluates the service identifier of the Eth test communication command and calls the specific
 *               command function for further processing.
 *  \param[in]   serviceIdentifier  Identifier of the requested service.
 *  \param[in]   payloadData        Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Eth_DispatchService(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_EthIf_TxConfirmation_Callout
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when Eth frame is transmitted.
 *  \details     The Eth driver notifies EthIf by EthIf_TxConfirmation, when a message frame is successfully sent.
 *               The EthIf stub calls this user callback function accordingly.
 *  \param[in]   CtrlIdx            Identifier of the Eth Controller which is unique within one Eth Driver module.
 *  \param[in]   BufIdx             Identifier of the buffer resource which was used for the transmission
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EthIf_TxConfirmation_Callout(
  VAR(uint8, AUTOMATIC) CtrlIdx,
  VAR(uint8, AUTOMATIC) BufIdx);

/**********************************************************************************************************************
 *  vItaHlp_EthIf_RxIndication_Callout
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when Eth frame is received.
 *  \details     The Eth driver notifies EthIf by EthIf_RxIndication, when a message frame is successfully received.
 *               The EthIf stub calls this user callback function accordingly.
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
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EthIf_RxIndication_Callout(
  VAR(uint8, AUTOMATIC) ctrlIdx,
  VAR(Eth_FrameType, AUTOMATIC) frameType,
  VAR(boolean, AUTOMATIC) isBroadcast,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) physAddrPtr,
  P2VAR(Eth_DataType, AUTOMATIC, VITAHLP_APPL_VAR) dataPtr,
  VAR(uint16, AUTOMATIC) lenByte);

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h"

#endif /* VITAHLP_ETH_H */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_Eth.h
 *********************************************************************************************************************/
