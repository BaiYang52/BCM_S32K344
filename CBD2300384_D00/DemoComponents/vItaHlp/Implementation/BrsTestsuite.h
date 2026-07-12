/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2019 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  BrsTestsuite.h
 *        \brief  Declarations required for vBrs testsuite support.
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

#ifndef _BRSTESTSUITE_H_
# define _BRSTESTSUITE_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "vItaHlp.h"
# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
#  include "vItaHlp_TestCom.h"
#  if ( (VITAHLP_CAN == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN) || (VITAHLP_ETH == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH) )
#   include "vItaHlp_ComIf.h"
#  endif
#  if (VITAHLP_LIN == STD_ON)
#   include "vItaHlp_Lin.h"
#  endif
# endif

/**********************************************************************************************************************
 *  GLOBAL MACROS
 *********************************************************************************************************************/

/* General Brs Testsuite functions */
# define BrsTestsuiteInit()                             /* not used by vItaHlp */

# if (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON)
/* Callout functions */
#  define DET_STUB_REPORTERROR_CALLOUT
#  define DET_STUB_REPORTERROR_SKIP_DEFAULT_IMPLEMENTATION
#  define BrsTestsuite_Det_ReportError_Callout(moduleId, instanceId, apiId, errorId) \
vItaHlp_Det_ReportError_Callout(moduleId, instanceId, apiId, errorId)
#  define DET_STUB_REPORTRUNTIMEERROR_CALLOUT
#  define DET_STUB_REPORTRUNTIMEERROR_SKIP_DEFAULT_IMPLEMENTATION
#  define BrsTestsuite_Det_ReportRuntimeError_Callout(moduleId, instanceId, apiId, errorId) \
vItaHlp_Det_ReportRuntimeError_Callout(moduleId, instanceId, apiId, errorId)
#  define ECUM_STUB_CHECKWAKEUP_CALLOUT
#  define BrsTestsuite_EcuM_CheckWakeup_Callout(wakeupSource) \
vItaHlp_EcuM_CheckWakeup_Callout(wakeupSource)
#  define ECUM_STUB_SETWAKEUPEVENT_CALLOUT
#  define BrsTestsuite_EcuM_SetWakeupEvent_Callout(wakeupSource) \
vItaHlp_EcuM_SetWakeupEvent_Callout(wakeupSource)

#  if ( (VITAHLP_CAN == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN) )
#   define CANIF_STUB_CONTROLLERMODEINDICATION_CALLOUT
#   define CANIF_STUB_RXINDICATION_CALLOUT
#   define CANIF_STUB_TXCONFIRMATION_CALLOUT
#   define BrsTestsuite_CanIf_ControllerModeIndication_Callout(controllerId, controllerMode) \
vItaHlp_ComIf_CanIfControllerModeIndication_Callout(controllerId, controllerMode)
#   define BrsTestsuite_CanIf_RxIndication_Callout(hrh, canId, canDlc, canSduPtr) \
vItaHlp_ComIf_CanIfRxIndication_Callout(hrh, canId, canDlc, canSduPtr)
#   define BrsTestsuite_CanIf_TxConfirmation_Callout(canTxPduId) \
vItaHlp_ComIf_CanIfTxConfirmation_Callout(canTxPduId)
#  endif /* ( (VITAHLP_CAN == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_CAN) ) */

#  if ( (VITAHLP_ETH == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH) )
#   define ETHIF_STUB_TXCONFIRMATION_CALLOUT
#   define BrsTestsuite_EthIf_TxConfirmation_Callout(ctrlIdx, bufIdx) \
vItaHlp_ComIf_EthIfTxConfirmation_Callout(ctrlIdx, bufIdx)
#   define ETHIF_STUB_RXINDICATION_CALLOUT
#   define BrsTestsuite_EthIf_RxIndication_Callout(ctrlIdx, frameType, isBroadcast, physAddrPtr, dataPtr, lenByte) \
vItaHlp_ComIf_EthIfRxIndication_Callout(ctrlIdx, frameType, isBroadcast, physAddrPtr, dataPtr, lenByte)
#  endif /* ( (VITAHLP_ETH == STD_ON) || (VITAHLP_TESTCOM_LINKLAYER == VITAHLP_TESTCOM_LL_ETH) ) */

#  if (VITAHLP_LIN == STD_ON)
#   define LINIF_STUB_WAKEUPCONFIRMATION_CALLOUT
#   define BrsTestsuite_LinIf_WakeupConfirmation_Callout(wakeupSource) \
vItaHlp_LinIf_WakeupConfirmation_Callout(wakeupSource)
#   if (VITAHLP_LINSLAVE == STD_ON)
#    define LINIF_STUB_HEADERINDICATION_CALLOUT
#    define BrsTestsuite_LinIf_HeaderIndication_Callout(channel, pduPtr) \
vItaHlp_LinIf_HeaderIndication_Callout(channel, pduPtr)
#    define LINIF_STUB_TXCONFIRMATION_CALLOUT
#    define BrsTestsuite_LinIf_TxConfirmation_Callout(channel) \
vItaHlp_LinIf_TxConfirmation_Callout(channel)
#    define LINIF_STUB_RXINDICATION_CALLOUT
#    define BrsTestsuite_LinIf_RxIndication_Callout(channel, linSduPtr) \
vItaHlp_LinIf_RxIndication_Callout(channel, linSduPtr)
#    define LINIF_STUB_LINERRORINDICATION_CALLOUT
#    define   BrsTestsuite_LinIf_LinErrorIndication_Callout(channel, errorStatus) \
vItaHlp_LinIf_LinErrorIndication_Callout(channel, errorStatus)
#   endif /* (VITAHLP_LINSLAVE == STD_ON) */
#  endif /* (VITAHLP_LIN == STD_ON) */

# endif /* (VITAHLP_TESTCOMMUNICATION_ENABLED == STD_ON) */

#endif /* _BRSTESTSUITE_H_ */

/**********************************************************************************************************************
 *  END OF FILE: BrsTestsuite.h
 *********************************************************************************************************************/
