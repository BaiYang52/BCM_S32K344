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
/**        \file  vItaHlp_TestCom.h
 *        \brief  Service API declaration for the Link Layer Abstraction Layer.
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

#if !defined (VITAHLP_TESTCOM_H)
# define VITAHLP_TESTCOM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Std_Types.h"
# include "ComStack_Types.h"
# include "vItaHlp_TestCom_Cfg.h"

/* Link layer specific includes */
# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN)
#  include "CanIf_Cbk.h"
# endif
# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH)
#  include "EthIf_Cbk.h"
# endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

typedef enum
{
  VITAHLP_TESTCOM_LINKDOWN,
  VITAHLP_TESTCOM_LINKUP
} vItaHlp_TestComLinkStatusType;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ----- GENERAL FUNCTIONS ----- */

/**********************************************************************************************************************
 *  vItaHlp_TestCom_PreInit()
 *********************************************************************************************************************/
/*! \brief       Initialize global vars.
 *  \details     -
 *  \pre         -
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_PreInit(void);

/**********************************************************************************************************************
 *  vItaHlp_TestCom_Init()
 *********************************************************************************************************************/
/*! \brief       Initialization for vItaHlp_TestCom.
 *  \details     Initialize the the configured link layer.
 *               Optional: register link layer specific main or background functions at the BRS
 *  \pre         Function must be called after vItaHlp_TestCom_PreInit and when OS is already started.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_Init(void);

/* ----- FUNCTIONS CALLED BY LINK LAYER ----- */

/**********************************************************************************************************************
 *  vItaHlp_TestCom_DataReceived()
 *********************************************************************************************************************/
/*! \brief       Forwards incoming test communication data to the upper layer.
 *  \details     -
 *  \param[in]   pduInfoPtr               Pointer to the service request information.
 *  \pre         vItaHlp_TestCom and upper layer need to be initialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_DataReceived(P2CONST(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr);

# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN)
/**********************************************************************************************************************
 *  vItaHlp_TestCom_CanDataReceived()
 *********************************************************************************************************************/
/*! \brief       Forwards incoming test communication data to the upper layer.
 *  \details     -
 *  \param[in]   hrh       Hardware handle the PDU was received in.
 *  \param[in]   canId     CAN identifier of the received PDU.
 *  \param[in]   canDlc    Data Length Code of the received CAN message.
 *  \param[in]   canSduPtr Sdu pointer containing the CAN message data.
 *  \return      TRUE:     Received CAN message is a valid test communication message which was further processed.
 *               FALSE:    Received CAN message is not a valid test communication message.
 *  \pre         vItaHlp_TestCom and upper layer need to be initialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled and test communication link layer is CAN.
 *********************************************************************************************************************/
FUNC(boolean, VITAHLP_CODE) vItaHlp_TestCom_CanDataReceived(
  VAR(CanIf_HwHandleType, AUTOMATIC) hrh,
  VAR(Can_IdType, AUTOMATIC) canId,
  VAR(uint8, AUTOMATIC) canDlc,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) canSduPtr);

/**********************************************************************************************************************
 *  vItaHlp_TestCom_IsTxCanPdu()
 *********************************************************************************************************************/
/*! \brief       Evaluates if canTxPduId is used on the transmit path for the test communication on CAN.
 *  \details     -
 *  \param[in]   canTxPduId PDU ID of the CAN frame
 *  \return      TRUE:      canTxPduId is used for the test communication.
 *               FALSE:     canTxPduId is not used for the test communication.
 *  \pre         vItaHlp_TestCom and upper layer need to be initialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled and test communication link layer is CAN.
 *********************************************************************************************************************/
FUNC(boolean, VITAHLP_CODE) vItaHlp_TestCom_IsTxCanPdu(VAR(PduIdType, AUTOMATIC) canTxPduId);

/**********************************************************************************************************************
 * vItaHlp_TestCom_ControllerModeIndication()
 *********************************************************************************************************************/
/*! \brief       User application specific callback, when desired CAN mode is changed.
 *  \details     The CAN driver notifies CanIf by CanIf_ControllerModeIndication, when the CAN Controller transitions
 *               to a new mode.
 *               The CanIf stub calls this user callback function accordingly. The link state is updated
 *               if a CAN controller for the TestCom switches to another mode.
 *  \param[in]   controllerId           Channel where the mode transition happened.
 *  \param[in]   controllerMode         Controller mode to which the CAN controller transitioned.
 *  \pre         vItaHlp_TestCom and upper layer need to be initialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled and test communication link layer is CAN.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_ControllerModeIndication(
  VAR(uint8, AUTOMATIC) canIfControllerId,
  VAR(CanIf_ControllerModeType, AUTOMATIC) controllerMode);
# endif /* (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN) */

# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH)
/**********************************************************************************************************************
 *  vItaHlp_TestCom_EthDataReceived()
 *********************************************************************************************************************/
/*! \brief       Forwards incoming test communication data to the upper layer.
 *  \details     -
 *  \param[in]   ctrlIdx            Identifier of the Eth Controller which is unique within one Eth Driver module.
 *  \param[in]   frameType          Ethernet frame type.
 *  \param[in]   isBroadcast        Indicates if the received frame is a broadcast frame.
 *  \param[in]   physAddrPtr        MAC address of the received frame.
 *  \param[in]   dataPtr            Pointer to the received frame.
 *  \param[in]   lenByte            Length of the received frame.
 *  \return      TRUE:     Received Ethernet message is a valid test communication message which was further processed.
 *               FALSE:    Received Ethernet message is not a valid test communication message.
 *  \pre         vItaHlp_TestCom and upper layer need to be initialized
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled and test communication link layer is Ethernet.
 *********************************************************************************************************************/
FUNC(boolean, VITAHLP_CODE) vItaHlp_TestCom_EthDataReceived(
  VAR(uint8, AUTOMATIC) ctrlIdx,
  VAR(Eth_FrameType, AUTOMATIC) frameType,
  VAR(boolean, AUTOMATIC) isBroadcast,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) physAddrPtr,
  P2VAR(Eth_DataType, AUTOMATIC, VITAHLP_APPL_VAR) dataPtr,
  VAR(uint16, AUTOMATIC) lenByte);
# endif /* (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH) */

/**********************************************************************************************************************
 *  vItaHlp_TestCom_SetLinkStatus()
 *********************************************************************************************************************/
/*! \brief       Lower layer can signal if it is available or not.
 *  \details     -
 *  \param[in]   linkState                Tells if the link is UP or DOWN
 *  \pre         Call only after vItaHlp_TestCom_PreInit, otherwise value is overwritten
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_SetLinkStatus(VAR(vItaHlp_TestComLinkStatusType, AUTOMATIC) linkState);

/* ----- FUNCTIONS CALLED BY UPPER LAYER ----- */

/**********************************************************************************************************************
 *  vItaHlp_TestCom_GetLinkStatus()
 *********************************************************************************************************************/
/*! \brief       Function tells the upper layer if the link is available.
 *  \details     Function uses an exclusive area, thus do not call inside the same exclusive area
 *  \return      The current link status
 *  \pre         Call only after vItaHlp_TestCom_PreInit otherwise status is invalid / not initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(vItaHlp_TestComLinkStatusType, VITAHLP_CODE) vItaHlp_TestCom_GetLinkStatus(void);

/**********************************************************************************************************************
 *  vItaHlp_TestCom_TransmitData()
 *********************************************************************************************************************/
/*! \brief        Triggers a transmission of the provided data on the link layer.
 *  \details     Only if the link layer is available.
 *  \param[in]   pduInfoPtr               Pointer to the service request information.
 *                                        Restriction: The data length is required to be '8' bytes,
 *                                                     otherwise the transmission is discarded.
 *  \pre         vItaHlp_TestCom and link layer need to be initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_TransmitData(P2VAR(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr);

/* ----- FUNCTIONS IMPLEMENTED BY UPPER LAYER ----- */

/**********************************************************************************************************************
 *  ApplTestComDataReceivedCbk()
 *********************************************************************************************************************/
/*! \brief       CBK API to be implemented by the upper layer.
 *  \details     Called for each received link layer data.
 *  \param[in]   pduInfoPtr               Pointer to the service request information.
 *  \pre         vItaHlp_TestCom and upper layer need to be initialized.
 *  \context     ANY
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
extern FUNC(void, VITAHLP_CODE) ApplTestComDataReceivedCbk(
  P2CONST(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr);

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* VITAHLP_TESTCOM_H */
