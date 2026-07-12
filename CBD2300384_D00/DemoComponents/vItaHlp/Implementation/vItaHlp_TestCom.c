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
/**        \file  vItaHlp_TestCom.c
 *        \brief  Source file which contains link layer part of test communication.
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

#define VITAHLP_TESTCOM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vItaHlp_Cfg.h"

#if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
# include "vItaHlp_TestCom.h"
# include "SchM_vItaHlp.h"
# include "vstdlib.h"

/* Link layer specific includes */
# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_TCM)
#  include "TcmDriver.h"
# endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH)
# define VITAHLP_TESTCOM_ETH_PAYLOADLENGTH      46u
# endif
# define VITAHLP_TESTCOM_COMMAND_LENGTH         VITAHLP_TESTCOM_LL_MESSAGE_MAX_LENGTH

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

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
# define VITAHLP_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
VAR(vItaHlp_TestComLinkStatusType, VITAHLP_VAR_NOINIT) vItaHlp_TestComLinkStatus;

# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH)
CONST(uint8, VITAHLP_VAR) vItaHlp_TestCom_EthDestinationMacAddress[6]     = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
CONST(Eth_FrameType, VITAHLP_VAR) vItaHlp_TestCom_EthCommandFrameType     = 0xFEED;
CONST(Eth_FrameType, VITAHLP_VAR) vItaHlp_TestCom_EthEventFrameType       = 0xBACC;
# endif
# define VITAHLP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_TCM)
/**********************************************************************************************************************
 *  vItaHlp_TestCom_TcmInit()
 *********************************************************************************************************************/
/*!  \brief       Initialize TCM Link Layer Module.
 *   \details     Also register the TCM background and main function at the BRS.
 *   \pre         -
 *   \context     TASK
 *   \reentrant   FALSE
 *   \synchronous TRUE
 *   \config      Test communication link layer is TCM.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_TcmInit(void);

/**********************************************************************************************************************
 *  vItaHlp_TestCom_TcmTransmitData()
 *********************************************************************************************************************/
/*! \brief       Triggers a transmission of the provided data via TCM.
 *  \details     -
 *  \param[in]   pduInfoPtr               Pointer to the service request information.
 *                                        Restriction: The data length is required to be '8' bytes,
 *                                                     otherwise the transmission is discarded.
 *  \pre         TCM needs to be initialized.
 *  \context     TASK
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication link layer is TCM.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_TcmTransmitData(
  P2VAR(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr);
# endif

# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN)
#  if (VITAHLP_TESTCOM_CAN_INIT == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_TestCom_CanInit()
 *********************************************************************************************************************/
/*!  \brief       Initialize CAN Link Layer Module.
 *   \details     -
 *   \pre         -
 *   \context     TASK
 *   \reentrant   FALSE
 *   \synchronous TRUE
 *   \config      Test communication link layer is CAN.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_CanInit(void);
#  endif /* (VITAHLP_TESTCOM_CAN_INIT == STD_ON) */

/**********************************************************************************************************************
 *  vItaHlp_TestCom_CanTransmitData()
 *********************************************************************************************************************/
/*!  \brief       Triggers a transmission of the provided data via CAN.
 *   \details     -
 *   \param[in]   pduInfoPtr               Pointer to the service request information.
 *                                         Restriction: The data length is required to be '8' bytes,
 *                                                      otherwise the transmission is discarded
 *   \pre         CAN needs to be initialized and in started mode.
 *   \context     TASK
 *   \reentrant   FALSE
 *   \synchronous TRUE
 *   \config      Test communication link layer is CAN driver.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_CanTransmitData(
  P2VAR(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr);
# endif /* (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN) */

# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH)
#  if (VITAHLP_TESTCOM_ETH_INIT == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_TestCom_EthInit()
 *********************************************************************************************************************/
/*!  \brief       Initialize Ethernet Link Layer Module.
 *   \details     -
 *   \pre         -
 *   \context     TASK
 *   \reentrant   FALSE
 *   \synchronous TRUE
 *   \config      Test communication link layer is Ethernet and Ethernet is not used as module under test for vItaHlp.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_EthInit(void);
#  endif /* (VITAHLP_TESTCOM_ETH_INIT == STD_ON) */

/**********************************************************************************************************************
 *  vItaHlp_TestCom_EthTransmitData()
 *********************************************************************************************************************/
/*!  \brief       Triggers a transmission of the provided data via Ethernet.
 *   \details     -
 *   \param[in]   pduInfoPtr               Pointer to the service request information.
 *                                         Restriction: The data length is required to be '8' bytes,
 *                                         otherwise the transmission is discarded
 *   \pre         Ethernet driver needs to be initialized and in active mode.
 *   \context     TASK
 *   \reentrant   FALSE
 *   \synchronous TRUE
 *   \config      Test communication link layer is Ethernet.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_EthTransmitData(
  P2VAR(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr);

/**********************************************************************************************************************
 *  vItaHlp_TestCom_EthUpdateLinkStatus()
 *********************************************************************************************************************/
/*!  \brief       Updates the test communication link status.
 *   \details     Unlike test communication via CAN, with Ethernet as test communication link layer
 *                the link status update is not triggered by interrupt,
 *                but by calling this update function, when the vItaHlp_TestCom_GetLinkStatus is called.
 *   \pre         -
 *   \context     TASK
 *   \reentrant   FALSE
 *   \synchronous TRUE
 *   \config      Test communication link layer is Ethernet.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_EthUpdateLinkStatus(void);

/**********************************************************************************************************************
 *  vItaHlp_TestCom_EthFillBufferWithEvent()
 *********************************************************************************************************************/
/*!  \brief       Function fills the provided Tx buffer with an event message.
 *   \details     -
 *   \param[in]   bufferPointer            Pointer to the Ethernet transmission buffer.
 *   \param[in]   pduInfoPtr               Pointer to the pdu, that contains the event message.
 *   \pre         Ethernet driver needs to confirm an available Tx buffer.
 *   \context     TASK
 *   \reentrant   FALSE
 *   \synchronous TRUE
 *   \config      Test communication link layer is Ethernet.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_EthFillBufferWithEvent(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) bufferPointer,
  P2VAR(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr);

# endif /* (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_TCM)
/**********************************************************************************************************************
 * vItaHlp_TestCom_TcmInit()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_TcmInit(void)
{
  Tcm_Init();
  BrsMainRegisterCyclic(&Tcm_BackgroundFunction, BRSMAIN_CYCLETIME_1MS);
  BrsMainRegisterCyclic(&Tcm_MainFunction, BRSMAIN_CYCLETIME_10MS);
}

/**********************************************************************************************************************
 * vItaHlp_TestCom_TcmTransmitData()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_TcmTransmitData(
  P2VAR(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr)
{
  Tcm_Transmit(
    VITAHLP_TESTCOM_TCM_TX_CANID,
    pduInfoPtr->SduLength,
    pduInfoPtr->SduDataPtr,
    VITAHLP_TESTCOM_TCM_TX_PRIORITY_HIGH);
}
# endif /* (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_TCM) */

# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN)
#  if (VITAHLP_TESTCOM_CAN_INIT == STD_ON)
/**********************************************************************************************************************
 * vItaHlp_TestCom_CanInit()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_CanInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  vItaHlp_TestCom_SetControllerModePtrType canSetControllerModeFctPtr;

  /* ----- Implementation ----------------------------------------------- */
  /* Start CAN controller used for the test communication */
  canSetControllerModeFctPtr = vItaHlp_TestCom_CanControllerData.canSetControllerModeFctPtr;
  (void) canSetControllerModeFctPtr( vItaHlp_TestCom_CanControllerData.canControllerId, CAN_T_START);
}
#  endif /* (VITAHLP_TESTCOM_CAN_INIT == STD_ON) */

/**********************************************************************************************************************
 * vItaHlp_TestCom_CanTransmitData()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_CanTransmitData(
  P2VAR(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  Can_PduType canTxPdu;

  /* ----- Implementation ----------------------------------------------- */
  canTxPdu.id = vItaHlp_TestCom_CanTxObjectData.canId;
  canTxPdu.length = pduInfoPtr->SduLength;
  canTxPdu.sdu = pduInfoPtr->SduDataPtr;
  canTxPdu.swPduHandle = vItaHlp_TestCom_CanTxObjectData.pduId;

  (void)vItaHlp_TestCom_CanTxObjectData.canWriteFctPtr(vItaHlp_TestCom_CanTxObjectData.canHwHandle, &canTxPdu);
}
# endif /* (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN) */

# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH)
#  if (VITAHLP_TESTCOM_ETH_INIT == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_TestCom_EthInit()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_EthInit(void)
{
  (void)vItaHlp_TestCom_EthControllerInit(vItaHlp_TestCom_EthControllerId, 0u);
  (void)vItaHlp_TestCom_EthSetControllerMode(vItaHlp_TestCom_EthControllerId, ETH_MODE_ACTIVE);
}
#  endif /* (VITAHLP_TESTCOM_ETH_INIT == STD_ON) */

/**********************************************************************************************************************
 *  vItaHlp_TestCom_EthTransmitData()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_EthTransmitData(
  P2VAR(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  BufReq_ReturnType ethBufferReqResult;
  uint16 txBufferLength;
  uint16 payloadLength;
  uint8 bufferId;
  uint8* bufferPointer;

  /* ----- Implementation ----------------------------------------------- */
  payloadLength = VITAHLP_TESTCOM_ETH_PAYLOADLENGTH;
  txBufferLength = payloadLength;


  /* Try to get a buffer with sufficent length */
  ethBufferReqResult = vItaHlp_TestCom_EthProvideTxBuffer(
    vItaHlp_TestCom_EthControllerId,
    &bufferId,
    (Eth_DataType **)&bufferPointer,
    &txBufferLength);

  if (ethBufferReqResult == BUFREQ_OK)
  {
    /* Copy event payload to the tx buffer */
    vItaHlp_TestCom_EthFillBufferWithEvent(bufferPointer, pduInfoPtr);
    /* Transmit the frame without Tx Confirmation */
    (void)vItaHlp_TestCom_EthTransmit(
      vItaHlp_TestCom_EthControllerId,
      bufferId,
      vItaHlp_TestCom_EthEventFrameType,
      FALSE,
      payloadLength,
      vItaHlp_TestCom_EthDestinationMacAddress);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_TestCom_EthUpdateLinkStatus()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_EthUpdateLinkStatus(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  vItaHlp_TestComLinkStatusType linkStatus = VITAHLP_TESTCOM_LINKDOWN;
  Eth_ModeType controllerMode = ETH_MODE_DOWN;
  
  /* ----- Implementation ----------------------------------------------- */
  /* Check if controller for test communication is active and update the link status */
  (void)vItaHlp_TestCom_EthGetControllerMode(vItaHlp_TestCom_EthControllerId, &controllerMode);

  if (controllerMode == ETH_MODE_ACTIVE)
  {
    linkStatus = VITAHLP_TESTCOM_LINKUP;
  }
  vItaHlp_TestCom_SetLinkStatus(linkStatus);
}

/**********************************************************************************************************************
 *  vItaHlp_TestCom_EthFillBufferWithEvent()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_EthFillBufferWithEvent(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) bufferPointer,
  P2VAR(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr)
  {
    /* ----- Local Variables ---------------------------------------------- */
    uint8_least pos;

    /* ----- Implementation ----------------------------------------------- */
    for (pos = 0; pos < pduInfoPtr->SduLength; pos++)
    {
      bufferPointer[pos] = pduInfoPtr->SduDataPtr[pos];
    }
  }
# endif /* (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_TestCom_PreInit()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_PreInit(void)
{
  vItaHlp_TestCom_SetLinkStatus(VITAHLP_TESTCOM_LINKDOWN);
}

/**********************************************************************************************************************
 *  vItaHlp_TestCom_Init()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_Init(void)
{
# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_TCM)
  vItaHlp_TestCom_TcmInit();
# elif (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN)
#  if (VITAHLP_TESTCOM_CAN_INIT == STD_ON)
  vItaHlp_TestCom_CanInit();
#  endif
# elif (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH)
#  if (VITAHLP_TESTCOM_ETH_INIT == STD_ON)
  vItaHlp_TestCom_EthInit();
#  endif
# endif
}

/**********************************************************************************************************************
 *  vItaHlp_TestCom_DataReceived()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_DataReceived(P2CONST(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr)
{
  /* No need to check length etc. since this is in concern of the application -> data is simply passed 'as it is' */
  ApplTestComDataReceivedCbk(pduInfoPtr);
}

/**********************************************************************************************************************
 *  vItaHlp_TestCom_SetLinkStatus()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_SetLinkStatus(VAR(vItaHlp_TestComLinkStatusType, AUTOMATIC) linkState)
{
  SchM_Enter_vItaHlp_VITAHLP_TESTCOM_EXCLUSIVE_AREA();
  vItaHlp_TestComLinkStatus = linkState;
  SchM_Exit_vItaHlp_VITAHLP_TESTCOM_EXCLUSIVE_AREA();
}

/**********************************************************************************************************************
 *  vItaHlp_TestCom_GetLinkStatus()
 *********************************************************************************************************************/
FUNC(vItaHlp_TestComLinkStatusType, VITAHLP_CODE) vItaHlp_TestCom_GetLinkStatus(void)
{
# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH)
  vItaHlp_TestCom_EthUpdateLinkStatus();
# endif /* (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH) */

  return vItaHlp_TestComLinkStatus;
}

/**********************************************************************************************************************
 *  vItaHlp_TestCom_TransmitData()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_TransmitData(P2VAR(PduInfoType, AUTOMATIC, VITAHLP_APPL_VAR) pduInfoPtr)
{
  /* Only transmit if the link layer is available */
  if (vItaHlp_TestCom_GetLinkStatus() == VITAHLP_TESTCOM_LINKUP)
  {
    /* Ensure that the applications pdu fits in the link layer pdu */
    /* Could also be done by lower layer, but currently not handled by all lower layers, for example TCM */
    if (pduInfoPtr->SduLength <= VITAHLP_TESTCOM_LL_MESSAGE_MAX_LENGTH)
    {
# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_TCM)
      vItaHlp_TestCom_TcmTransmitData(pduInfoPtr);
# endif
# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN)
      vItaHlp_TestCom_CanTransmitData(pduInfoPtr);
# endif
# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH)
      vItaHlp_TestCom_EthTransmitData(pduInfoPtr);
# endif
    }
  }
}

# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN)
/**********************************************************************************************************************
 * vItaHlp_TestCom_CanDataReceived()
 *********************************************************************************************************************/
FUNC(boolean, VITAHLP_CODE) vItaHlp_TestCom_CanDataReceived(
  VAR(CanIf_HwHandleType, AUTOMATIC) hrh,
  VAR(Can_IdType, AUTOMATIC) canId,
  VAR(uint8, AUTOMATIC) canDlc,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) canSduPtr)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isCanTestComMessage = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* Verify if the received CAN message is a test communication message */
  if ((hrh == vItaHlp_TestCom_CanRxObjectData.canHwHandle) &&
    (canId == vItaHlp_TestCom_CanRxObjectData.canId) &&
    (canDlc <= 8u))
  {
    /* Forward received CAN message */
    uint8 messageData[VITAHLP_TESTCOM_COMMAND_LENGTH];
    PduInfoType pduInfo;
    VStdLib_MemCpy(messageData, canSduPtr, canDlc);
    pduInfo.SduDataPtr = messageData;
    pduInfo.SduLength = canDlc;
    vItaHlp_TestCom_DataReceived(&pduInfo);
    isCanTestComMessage = TRUE;
  }
  return isCanTestComMessage;
}

/**********************************************************************************************************************
 * vItaHlp_TestCom_IsTxCanPdu()
 *********************************************************************************************************************/
FUNC(boolean, VITAHLP_CODE) vItaHlp_TestCom_IsTxCanPdu(VAR(PduIdType, AUTOMATIC) canTxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isTxCanPdu = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* Verify if the received CAN message is a test communication message */
  if (canTxPduId == vItaHlp_TestCom_CanTxObjectData.pduId)
  {
    isTxCanPdu = TRUE;
  }
  return isTxCanPdu;
}

/**********************************************************************************************************************
 * vItaHlp_TestCom_ControllerModeIndication()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_TestCom_ControllerModeIndication(
  VAR(uint8, AUTOMATIC) canIfControllerId,
  VAR(CanIf_ControllerModeType, AUTOMATIC) controllerMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  vItaHlp_TestComLinkStatusType linkStatus = VITAHLP_TESTCOM_LINKDOWN;

  /* ----- Implementation ----------------------------------------------- */
  /* Check if controllerMode indication is given for a CAN controller used for the test communication */
  if (vItaHlp_TestCom_CanControllerData.canIfAbstractControllerId == canIfControllerId)
  {
    /* Check if controller for test communication is started and update the link status */
    if (controllerMode == CANIF_CS_STARTED)
    {
      linkStatus = VITAHLP_TESTCOM_LINKUP;
    }
    vItaHlp_TestCom_SetLinkStatus(linkStatus);
  }
}
# endif /* (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN) */

# if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH)
/**********************************************************************************************************************
 * vItaHlp_TestCom_EthDataReceived()
 *********************************************************************************************************************/
FUNC(boolean, VITAHLP_CODE) vItaHlp_TestCom_EthDataReceived(
  VAR(uint8, AUTOMATIC) ctrlIdx,
  VAR(Eth_FrameType, AUTOMATIC) frameType,
  VAR(boolean, AUTOMATIC) isBroadcast,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) physAddrPtr,
  P2VAR(Eth_DataType, AUTOMATIC, VITAHLP_APPL_VAR) dataPtr,
  VAR(uint16, AUTOMATIC) lenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean isEthTestComMessage = FALSE;

  /* ----- Implementation ----------------------------------------------- */
  /* Verify if the received Ethernet message is a test communication message */
  if ((ctrlIdx == vItaHlp_TestCom_EthControllerId) &&
    (frameType == vItaHlp_TestCom_EthCommandFrameType) &&
    (lenByte == VITAHLP_TESTCOM_ETH_PAYLOADLENGTH))
  {
    /* Forward received Ethernet message */
    uint8 messageData[VITAHLP_TESTCOM_COMMAND_LENGTH];
    PduInfoType pduInfo;
    VStdLib_MemCpy(messageData, dataPtr, VITAHLP_TESTCOM_LL_MESSAGE_MAX_LENGTH);
    pduInfo.SduDataPtr = messageData;
    pduInfo.SduLength = VITAHLP_TESTCOM_LL_MESSAGE_MAX_LENGTH;
    vItaHlp_TestCom_DataReceived(&pduInfo);
    isEthTestComMessage = TRUE;
  }

  VITAHLP_DUMMY_STATEMENT(isBroadcast);
  VITAHLP_DUMMY_STATEMENT(physAddrPtr);

  return isEthTestComMessage;
}

# endif /* (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH) */

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_TestCom.c
 *********************************************************************************************************************/
