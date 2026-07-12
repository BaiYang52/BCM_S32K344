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
/**        \file  vItaHlp_ComIf.c
 *        \brief  vItaHlp ComIf dispatchs the communication interface callouts (to TestCom or module under test).
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

#define VITAHLP_COMIF_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vItaHlp_ComIf.h"

#if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
# if (VITAHLP_CAN == STD_ON)
#  include "vItaHlp_Can.h"
# endif
# if (VITAHLP_ETH == STD_ON)
#  include "vItaHlp_Eth.h"
# endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

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

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VITAHLP_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


# if ( (VITAHLP_CAN == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN) )
/**********************************************************************************************************************
 *  vItaHlp_ComIf_CanIfRxIndication_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_ComIf_CanIfRxIndication_Callout(
  VAR(CanIf_HwHandleType, AUTOMATIC) hrh,
  VAR(Can_IdType, AUTOMATIC) canId,
  VAR(uint8, AUTOMATIC) canDlc,
  P2CONST(uint8, AUTOMATIC, VITAHLP_APPL_VAR) canSduPtr)
{
#  if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN)
  if (!vItaHlp_TestCom_CanDataReceived(hrh, canId, canDlc, canSduPtr))
  {
#  endif
#  if (VITAHLP_CAN == STD_ON)
    vItaHlp_CanIf_RxIndication_Callout(hrh, canId, canDlc, canSduPtr);
#  endif
#  if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN)
  }
#  endif
}

/**********************************************************************************************************************
 *  vItaHlp_ComIf_CanIfTxConfirmation_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_ComIf_CanIfTxConfirmation_Callout(VAR(PduIdType, AUTOMATIC) canTxPduId)
{
#  if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN)
  if (!vItaHlp_TestCom_IsTxCanPdu(canTxPduId))
  {
#  endif
#  if (VITAHLP_CAN == STD_ON)
  vItaHlp_CanIf_TxConfirmation_Callout(canTxPduId);
#  endif
#  if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN)
  }
#  endif
}

/**********************************************************************************************************************
 *  vItaHlp_ComIf_CanIfControllerModeIndication_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_ComIf_CanIfControllerModeIndication_Callout(
  VAR(uint8, AUTOMATIC) canIfControllerId,
  VAR(CanIf_ControllerModeType, AUTOMATIC) controllerMode)
{
#  if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN)
  vItaHlp_TestCom_ControllerModeIndication(canIfControllerId, controllerMode);
#  endif
#  if (VITAHLP_CAN == STD_ON)
  vItaHlp_CanIf_ControllerModeIndication_Callout(canIfControllerId, controllerMode);
#  endif
}
# endif /* ( (VITAHLP_CAN == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN) ) */

# if ( (VITAHLP_ETH == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH) )
/**********************************************************************************************************************
 *  vItaHlp_ComIf_EthIfTxConfirmation_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_ComIf_EthIfTxConfirmation_Callout(
  VAR(uint8, AUTOMATIC) ctrlIdx,
  VAR(uint8, AUTOMATIC) bufIdx)
{
#  if (VITAHLP_ETH == STD_ON)
  vItaHlp_EthIf_TxConfirmation_Callout(ctrlIdx, bufIdx);
#  else
  VITAHLP_DUMMY_STATEMENT(ctrlIdx);
  VITAHLP_DUMMY_STATEMENT(bufIdx);
#  endif /* (VITAHLP_ETH == STD_ON) */
}

/**********************************************************************************************************************
 *  vItaHlp_ComIf_EthIfRxIndication_Callout()
 *********************************************************************************************************************/
FUNC(void, VITAHLP_CODE) vItaHlp_ComIf_EthIfRxIndication_Callout(
  VAR(uint8, AUTOMATIC) ctrlIdx,
  VAR(Eth_FrameType, AUTOMATIC) frameType,
  VAR(boolean, AUTOMATIC) isBroadcast,
  P2VAR(uint8, AUTOMATIC, VITAHLP_APPL_VAR) physAddrPtr,
  P2VAR(Eth_DataType, AUTOMATIC, VITAHLP_APPL_VAR) dataPtr,
  VAR(uint16, AUTOMATIC) lenByte)
{
#  if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH)
  if (!vItaHlp_TestCom_EthDataReceived(ctrlIdx, frameType, isBroadcast, physAddrPtr, dataPtr, lenByte))
  {
#  endif
#  if (VITAHLP_ETH == STD_ON)
    vItaHlp_EthIf_RxIndication_Callout(ctrlIdx, frameType, isBroadcast, physAddrPtr, dataPtr, lenByte);
#  endif
#  if (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH)
  }
#  endif
}
# endif /* ( (VITAHLP_ETH == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH) ) */

# define VITAHLP_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: vItaHlp_ComIf.c
 *********************************************************************************************************************/
