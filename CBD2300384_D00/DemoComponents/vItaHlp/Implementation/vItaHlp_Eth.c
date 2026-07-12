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
/**        \file  vItaHlp_Eth.c
 *        \brief  vItaHlp functionalities concerning ETH.
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

#define VITAHLP_ETH_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vItaHlp.h"

#if (VITAHLP_ETH == STD_ON)
# include "vItaHlp_CommonUtils.h"
# include "vItaHlp_Eth.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define VITAHLP_ETH_SETUPTEST_PAYLOADLENGTH 46

/* Definitions of API function IDs and module ID from AUTOSAR specification of ETH component */
# define VITAHLP_ETH_SID_CONTROLLER_INIT_ID                 (0x02u)
# define VITAHLP_ETH_SID_SET_CONTROLLER_MODE_ID             (0x03u)
# define VITAHLP_ETH_SID_PROVIDE_TX_BUFFER_ID               (0x09u)
# define VITAHLP_ETH_SID_TRANSMIT_ID                        (0x0Au)
# define VITAHLP_ETH_MODULE_ID                              (88u)

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

/* Service Identifiers for ETH test module commands */
typedef enum
{
  VITAHLP_ETH_CMD_TRANSMISSION                     = 0u,
  VITAHLP_ETH_CMD_SERVICEIDENTIFIER_UNKNOWN        = 0xFFu
} vItaHlp_Eth_TestCommunicationCmdServiceIdentifierType;

/* Service Identifiers for ETH test module events */
typedef enum
{
  VITAHLP_ETH_EVENT_RX_INDICATION                  = 0u,
  VITAHLP_ETH_EVENT_TX_CONFIRMATION                = 1u
} vItaHlp_Eth_TestCommunicationEventServiceIdentifierType;

typedef enum
{
  VITAHLP_ETH_TESTCOMMUNICATION_MESSAGE_TRANSMISSION_POS_PAYLOAD_CTRL_IDX = 0u,
  VITAHLP_ETH_TESTCOMMUNICATION_MESSAGE_TRANSMISSION_POS_PAYLOAD_LENGTH   = 1u
} vItaHlp_Eth_TestCommunicationMessageTransmissionLayoutType;

/**********************************************************************************************************************
 *  LOCAL DATA
 *********************************************************************************************************************/
# define VITAHLP_START_SEC_VAR_NOINIT_UNSPECIFIED
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

CONST(uint8, VITAHLP_VAR) vItaHlp_Eth_DestinationMacAddress[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
CONST(Eth_FrameType, VITAHLP_VAR) vItaHlp_Eth_FrameType        = 0xBEEF;
VAR(boolean, VITAHLP_VAR_NOINIT) vItaHlp_Eth_SetupTestRampMode;

# define VITAHLP_STOP_SEC_VAR_NOINIT_UNSPECIFIED
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
 *  vItaHlp_Eth_FillBufferWithRampPattern()
 *********************************************************************************************************************/
/*! \brief         Function fills each byte of the provided buffer with a ramp pattern.
 *  \details       Either increasing or decreasing ramp:
 *                 Mode 0: 0x00   0x01   0x02   0x03   0x04   0x05   0x06   ....
 *                 Mode 1: 0xFF   0xFE   0xFD   0xFC   0xFB   0xFA   0xF9   .....
 *                 If the length exceeds 255 bytes the pattern will be repeated for the following bytes (saw tooth).
 *  \param[in,out] buffer           Data buffer which gets filled with the ramp pattern.
 *  \param[in]     length           Length of the buffer.
 *  \param[in]     mode             0: increasing ramp starting with 0
 *                                  1: decreasing ramp starting with 255
 *  \pre           -
 *  \context       TASK
 *  \reentrant     FALSE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Eth_FillBufferWithRampPattern(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) buffer,
  VAR(uint16, AUTOMATIC) length,
  VAR(boolean, AUTOMATIC) mode);

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Eth_GetCtrlMapIdx()
 *********************************************************************************************************************/
/*! \brief       Verifies if the given Eth controller instance exists in the internal config struct array and
 *               returns the corresponding index.
 *  \details     Allthough the Eth controller ID is unique over all Eth Driver modules, it is not possible to use
 *               the ctrlIdx as index for this config array. There might be gaps within the list of 
 *               Eth controller IDs, because not each Eth controller is necessarily used as unit under test.
 *  \param[in]   ctrlIdx            Index of the Eth controller which is unique over all Eth Driver modules.
 *  \param[out]  ctrlMapIdx         Index of the internal config array which contains the entry for the given
 *                                  Eth controller index.
 *  \return      TRUE:              ctrlMapIdx for given input combination found
 *               FALSE:             given input cannot be found in internal config array
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Eth_GetCtrlMapIdx(
  VAR(uint8, AUTOMATIC) ctrlIdx,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) ctrlMapIdx);

/**********************************************************************************************************************
 *  vItaHlp_Eth_Transmission()
 *********************************************************************************************************************/
/*! \brief       Prepare and send an Eth frame.
 *  \details     The identification of the Eth Controller over which the message frame shall be send and the
 *               message length are derived from the payload data information. The payload content is filled with
 *               incrementing byte values (ramp pattern).
 *  \param[in]   payloadData        Pointer to the payloadData, length is ensured by caller to be 6.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Eth_Transmission(
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData);

/**********************************************************************************************************************
 *  vItaHlp_Eth_CompareReceivedEthFramePayloadWithPattern()
 *********************************************************************************************************************/
/*! \brief       Compare the received Eth frame payload with expected pattern (ramp pattern).
 *  \details     -
 *  \param[in]   ethPayloadPointer  Pointer to the Eth frame payload to be compared.
 *  \param[in]   ethPayloadLength   Length of the Eth frame payload to be compared.
 *  \return      TRUE:              The received payload is equal to the expected payload content.
 *               FALSE:             The received payload contains unexpected payload content.
 *  \pre         -
 *  \context     TASK|ISR
 *  \reentrant   FALSE
 *  \synchronous TRUE
 *  \config      Test communication is enabled.
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE)vItaHlp_Eth_CompareReceivedEthFramePayloadWithPattern(
  P2VAR(Eth_DataType, AUTOMATIC, VITAHLP_APPL_VAR) ethPayloadPointer,
  VAR(uint16, AUTOMATIC) ethPayloadLength);

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_Eth_FillBufferWithRampPattern()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Eth_FillBufferWithRampPattern(
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) buffer,
  VAR(uint16, AUTOMATIC) length,
  VAR(boolean, AUTOMATIC) mode)
{
  uint16_least index;
  uint8 value = 0u - mode;

  for (index = 0u; index < length; index++)
  {
    *(buffer + index) = value;
    if (mode == 0u) /* Increasing mode */
    {
      value++;
    }
    else /* Decreasing mode */
    {
      value--;
    }
  }
}

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Eth_GetCtrlMapIdx()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE) vItaHlp_Eth_GetCtrlMapIdx(
  VAR(uint8, AUTOMATIC) ctrlIdx,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) ctrlMapIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean mapIdxExists = FALSE;
  uint8 index;

  /* ----- Implementation ----------------------------------------------- */
  for (index = 0u; index < vItaHlp_Eth_GetNumberOfControllers(); index++)
  {
    if (vItaHlp_Eth_ControllerSpecificData[index].ethControllerId == ctrlIdx)
    {
      ctrlMapIdx[0u] = index;
      mapIdxExists = TRUE;
      break;
    }
  }
  return mapIdxExists;
}

/**********************************************************************************************************************
 *  vItaHlp_Eth_Transmission()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(void, VITAHLP_CODE) vItaHlp_Eth_Transmission(P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType ethTransmitResult;
  BufReq_ReturnType ethBufferReqResult;
  uint16 txBufferLength;
  uint16 payloadLength;
  uint8 bufferId;
  uint8 ctrlMapIdx;
  uint8 ctrlIdx;
  uint8* bufferPointer;

  /* ----- Implementation ----------------------------------------------- */
  /* Retrieve the test input data*/
  ctrlIdx = payloadData[VITAHLP_ETH_TESTCOMMUNICATION_MESSAGE_TRANSMISSION_POS_PAYLOAD_CTRL_IDX];
  payloadLength = vItaHlp_CommonUtils_ByteArrayToUint16BigEndian(
                    &payloadData[VITAHLP_ETH_TESTCOMMUNICATION_MESSAGE_TRANSMISSION_POS_PAYLOAD_LENGTH]);
  txBufferLength = payloadLength;

  /* Verify if the indicated Eth controller is configured as unit under test */
  if (vItaHlp_Eth_GetCtrlMapIdx(ctrlIdx, &ctrlMapIdx) == TRUE)
  {
    /* Try to get a buffer with sufficent length */
    ethBufferReqResult = vItaHlp_Eth_ProvideTxBuffer(
      ctrlMapIdx,
      &bufferId,
      (Eth_DataType **)&bufferPointer,
      &txBufferLength);
    if (ethBufferReqResult == BUFREQ_OK)
    {
      /* Copy a ramp pattern to the tx buffer. Increment values */
      vItaHlp_Eth_FillBufferWithRampPattern(bufferPointer, payloadLength, FALSE);
      /* Transmit the frame. Request Tx Confirmation */
      ethTransmitResult = vItaHlp_Eth_Transmit(
        ctrlMapIdx,
        bufferId,
        vItaHlp_Eth_FrameType,
        TRUE,
        payloadLength,
        vItaHlp_Eth_DestinationMacAddress);
      if (ethTransmitResult != E_OK)
      {
        vItaHlp_EventModuleUnderTestError(
          VITAHLP_ETH_MODULE_ID,
          vItaHlp_Eth_GetDriverSpecficDataIndex(ctrlMapIdx),
          VITAHLP_ETH_SID_TRANSMIT_ID,
          ethTransmitResult);
      }
    }
    else
    {
      vItaHlp_EventModuleUnderTestError(
        VITAHLP_ETH_MODULE_ID,
        vItaHlp_Eth_GetDriverSpecficDataIndex(ctrlMapIdx),
        VITAHLP_ETH_SID_PROVIDE_TX_BUFFER_ID,
        ethBufferReqResult);
    }
  }
  else
  {
    /* Send general error event in case the referenced Eth ctrlIdx is not in the configured
       controller specific array (out of range) - this can only happen in case of a invalid generated code */
    vItaHlp_EventVItaHlpError(
      VITAHLP_COMPONENTIDENTIFIER_ETH,
      VITAHLP_ETH_CMD_TRANSMISSION,
      VITAHLP_TESTCOMMUNICATION_SERVICEIDENTIFIER_CMD,
      VITAHLP_E_INVALID_CONFIGURATION,
      0u);
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Eth_CompareReceivedEthFramePayloadWithPattern()
 *********************************************************************************************************************/
VITAHLP_LOCAL FUNC(boolean, VITAHLP_CODE)vItaHlp_Eth_CompareReceivedEthFramePayloadWithPattern(
  P2VAR(Eth_DataType, AUTOMATIC, VITAHLP_APPL_VAR) ethPayloadPointer,
  VAR(uint16, AUTOMATIC) ethPayloadLength)
{
  /* ----- Local Variables ---------------------------------------------- */
  boolean validPayload = FALSE;
  uint8 expectedValue = 0;
  uint16_least index;
  uint8* rxBufferPtr = (uint8*)ethPayloadPointer;

  /* ----- Implementation ----------------------------------------------- */
  for (index = 0u; index < ethPayloadLength; index++)
  {
    if (rxBufferPtr[index] == expectedValue)
    {
      validPayload = TRUE;
    }
    else
    {
      validPayload = FALSE;
      break;
    }
    expectedValue++;
  }
  return validPayload;
}

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  vItaHlp_Eth_Init()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Eth_Init(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType controllerInitResult;
  Std_ReturnType controllerModeResult;
  uint8_least ctrlMapIdx;

  /* ----- Implementation ----------------------------------------------- */
  /* Initialize test pattern bit for setup test */
  vItaHlp_Eth_SetupTestRampMode = FALSE;

  /* Initialize and activate each configured ETH controller for each configured ETH driver */
  for (ctrlMapIdx = 0u; ctrlMapIdx < vItaHlp_Eth_GetNumberOfControllers(); ctrlMapIdx++)
  {
    controllerInitResult = vItaHlp_Eth_ControllerInit(ctrlMapIdx, 0u);
    if (controllerInitResult == E_OK)
    {
      controllerModeResult = vItaHlp_Eth_SetControllerMode(ctrlMapIdx, ETH_MODE_ACTIVE);
# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
      if (controllerModeResult != E_OK)
      {
        vItaHlp_EventModuleUnderTestError(
          VITAHLP_ETH_MODULE_ID,
          vItaHlp_Eth_GetDriverSpecficDataIndex(ctrlMapIdx),
          VITAHLP_ETH_SID_SET_CONTROLLER_MODE_ID,
          controllerModeResult);
      }
# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */
    }
# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
    else
    {
      vItaHlp_EventModuleUnderTestError(
        VITAHLP_ETH_MODULE_ID,
        vItaHlp_Eth_GetDriverSpecficDataIndex(ctrlMapIdx),
        VITAHLP_ETH_SID_CONTROLLER_INIT_ID,
        controllerInitResult);
    }
# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */
  }
}

/**********************************************************************************************************************
 *  vItaHlp_Eth_TriggerSetupTest()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Eth_TriggerSetupTest(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint16 txBufferLength = VITAHLP_ETH_SETUPTEST_PAYLOADLENGTH;
  uint8 bufferId;
  uint8* bufferPointer;
  uint8_least ctrlMapIdx;

  /* ----- Implementation ----------------------------------------------- */

  /* Transmit a frame for each configured ETH controller */
  for (ctrlMapIdx = 0u; ctrlMapIdx < vItaHlp_Eth_GetNumberOfControllers(); ctrlMapIdx++)
  {
    /* Try to get a buffer with sufficent length */
    if (BUFREQ_OK == vItaHlp_Eth_ProvideTxBuffer(
      ctrlMapIdx,
      &bufferId,
      (Eth_DataType **)&bufferPointer,
      &txBufferLength))
    {
      /* Check if provided buffer has sufficent length */
      if (txBufferLength >= VITAHLP_ETH_SETUPTEST_PAYLOADLENGTH)
      {
        /* Copy a ramp pattern to the tx buffer */
        vItaHlp_Eth_FillBufferWithRampPattern(
          bufferPointer,
          VITAHLP_ETH_SETUPTEST_PAYLOADLENGTH,
          vItaHlp_Eth_SetupTestRampMode);
        /* transmit the frame */
        vItaHlp_Eth_Transmit(
          ctrlMapIdx,
          bufferId,
          vItaHlp_Eth_FrameType,
          FALSE,
          VITAHLP_ETH_SETUPTEST_PAYLOADLENGTH,
          vItaHlp_Eth_DestinationMacAddress);
      }
    }
  }
  /* switch ramp mode for next transmission */
  vItaHlp_Eth_SetupTestRampMode = !vItaHlp_Eth_SetupTestRampMode;
}

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  vItaHlp_Eth_DispatchService()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_Eth_DispatchService(
  VAR(uint8, AUTOMATIC) serviceIdentifier,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) payloadData)
{
  switch (serviceIdentifier)
  {
    case VITAHLP_ETH_CMD_TRANSMISSION:
      vItaHlp_Eth_Transmission(payloadData);
      break;
    default:
      vItaHlp_EventVItaHlpError(
        VITAHLP_COMPONENTIDENTIFIER_ETH,
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
 *  vItaHlp_EthIf_TxConfirmation_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EthIf_TxConfirmation_Callout(
  VAR(uint8, AUTOMATIC) ctrlIdx,
  VAR(uint8, AUTOMATIC) bufIdx)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];

  /* ----- Implementation ----------------------------------------------- */
  /* As a requirement of vItaHlp, the ctrlIdx is unique over all configured Eth Drivers.
     We may use it as unique identifier within the Tx Confirmation event to the tester */
  payloadBuffer[0u] = ctrlIdx;

  /* Forward to EventHandler */
  vItaHlp_SetEvent(
    VITAHLP_COMPONENTIDENTIFIER_ETH,
    VITAHLP_ETH_EVENT_TX_CONFIRMATION,
    (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadBuffer);

  /* The buffer index is not forwarded to the tester */
  VITAHLP_DUMMY_STATEMENT(bufIdx);
}

/**********************************************************************************************************************
 *  vItaHlp_EthIf_RxIndication_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_EthIf_RxIndication_Callout(
  VAR(uint8, AUTOMATIC) ctrlIdx,
  VAR(Eth_FrameType, AUTOMATIC) frameType,
  VAR(boolean, AUTOMATIC) isBroadcast,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) physAddrPtr,
  P2VAR(Eth_DataType, AUTOMATIC, VITAHLP_APPL_VAR) dataPtr,
  VAR(uint16, AUTOMATIC) lenByte)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 payloadBuffer[VITAHLP_TESTCOMMUNICATION_PAYLOAD_LENGTH];
  boolean validPayload = FALSE;
  uint8 ctrlMapIdx;

  /* ----- Implementation ----------------------------------------------- */
  if (frameType == vItaHlp_Eth_FrameType)
  {
    /* Verify if the indicated Eth controller is configured as unit under test */
    if (vItaHlp_Eth_GetCtrlMapIdx(ctrlIdx, &ctrlMapIdx) == TRUE)
    {
      /* As a requirement of vItaHlp, the ctrlIdx is unique over all configured Eth Drivers.
         We may use it as unique identifier within the Rx Indication event to the tester */
      payloadBuffer[0u] = ctrlIdx;
      validPayload = vItaHlp_Eth_CompareReceivedEthFramePayloadWithPattern(dataPtr, lenByte);
      payloadBuffer[1u] = ((uint8)validPayload) << 4u | (uint8)isBroadcast;
      vItaHlp_CommonUtils_Uint16ToByteArrayBigEndian(&payloadBuffer[2u], lenByte);
      vItaHlp_CommonUtils_Uint16ToByteArrayBigEndian(&payloadBuffer[4u], (uint16)frameType);

      /* Forward to EventHandler */
      vItaHlp_SetEvent(
        VITAHLP_COMPONENTIDENTIFIER_ETH,
        VITAHLP_ETH_EVENT_RX_INDICATION,
        (P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR)) payloadBuffer);
    }
  }

  /* The physical source address pointer is not forwarded to the tester */
  VITAHLP_DUMMY_STATEMENT(physAddrPtr);
}

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VITAHLP_ETH == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_Eth.c
 *********************************************************************************************************************/
