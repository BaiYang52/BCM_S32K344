/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2022 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  SecOC.c
 *        \brief  MICROSAR  Secure Onboard Communication source file
 *
 *      \details  This is the implementation of the MICROSAR Secure Onboard Communication module.
 *                The basic software module is based on the AUTOSAR Communication specification.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define SECOC_SOURCE
/**********************************************************************************************************************
 *  LOCAL MISRA / PCLINT JUSTIFICATION
 *********************************************************************************************************************/
/* *INDENT-OFF* */
/* PRQA  S 3453 EOF */ /* MD_MSR_FctLikeMacro */
/* PRQA  S 4304 EOF */ /* MD_MSR_AutosarBoolean */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "SecOC_Cbk.h"
#include "SecOC.h"
#include "SchM_SecOC.h"
#include "vstdlib.h"
#include "Rte_SecOC.h"
#include "Csm.h"
#if (SECOC_DEV_ERROR_REPORT == STD_ON)
# include "Det.h"
#endif
#include "PduR_SecOC.h"
#if(SECOC_USE_INIT_POINTER == STD_ON)
# include "EcuM_Error.h"
#endif
#if (SECOC_CONTAINERHEADERSIZEOFRXPDUINFO == STD_ON)
# include "IpduM.h"
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 **********************************************************************************************************************/
#if (SECOC_DEV_ERROR_REPORT == STD_ON)
# define SecOC_ReportDetError(API_ID, ERROR_CODE) ((void) Det_ReportError(SECOC_MODULE_ID, SECOC_INSTANCE_ID_DET, (API_ID), (ERROR_CODE)))
# define SecOC_ReportRuntimeDetError(API_ID, ERROR_CODE) ((void) Det_ReportRuntimeError( SECOC_MODULE_ID, SECOC_INSTANCE_ID_DET, (API_ID), (ERROR_CODE)))
#else
# define SecOC_ReportRuntimeDetError(API_ID, ERROR_CODE)
#endif
#if (SECOC_RXPDUINFO == STD_ON)
# define SecOC_IsRxPduStateUnverified(rxPduId) ((SecOC_GetRxPduState(rxPduId) & SECOC_UNVERIFIED_RXPDUSTATE) == SECOC_UNVERIFIED_RXPDUSTATE)
#endif
#if (SECOC_TXPDUINFO == STD_ON)
# define SecOC_IsTxPduStateUnauthenticated(txPduId) ((SecOC_GetTxPduState(txPduId) & SECOC_UNAUTHENTICATED_TXPDUSTATE) == SECOC_UNAUTHENTICATED_TXPDUSTATE)
#endif
/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/
typedef enum
{
  SECOC_E_VER_OK = 0u,          /* PRQA S 3205 */ /* MD_SecOC_3205 */ /* !< Verified */
  SECOC_E_VER_NOT_OK = 1u,      /* PRQA S 3205 */ /* MD_SecOC_3205 */ /* !< not Verified */
  SECOC_E_VER_BUSY = 2u /* PRQA S 3205 */ /* MD_SecOC_3205 */ /* !< CSM or CAL is busy */
} SecOC_VerifyResultType;       /* PRQA S 3205 */ /* MD_SecOC_3205 */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 **********************************************************************************************************************/
#if(SECOC_USE_INIT_POINTER == STD_ON)
# define SECOC_START_SEC_VAR_CLEARED_UNSPECIFIED
# include "SecOC_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
P2CONST(SecOC_ConfigType, SECOC_VAR_CLEARED, SECOC_PBCFG) SecOC_ConfigDataPtr = NULL_PTR;
# define SECOC_STOP_SEC_VAR_CLEARED_UNSPECIFIED
# include "SecOC_MemMap.h" /* PRQA S 5087 */  /* MD_MSR_MemMap */
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA
 **********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/
#define SECOC_START_SEC_CODE
#include "SecOC_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_FreshnessValue_GetVerifyValueFromCallout
**********************************************************************************************************************/
/*! \brief      Gets the freshness verify value form the configured callout.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \param[out] freshnessValue       freshness verify value buffer
    \param[in]  freshnessValueBufferLength freshness verify value buffer length
    \return     E_OK request successful
                E_NOT_OK request failed
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_FreshnessValue_GetVerifyValueFromCallout(SecOC_RxPduInfoIterType rxPduId, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) freshnessValue, uint8 freshnessValueBufferLength);
#endif

#if(SECOC_AUTHHEADERLENGTHOFRXSECUREDPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetLengthFromHeader
**********************************************************************************************************************/
/*! \brief      Gets the authentic pdu length form the header.
    \details    -
    \pre        -
    \param[in]  headerData  pointer to header.
    \param[in]  headerLength length of the header in bytes.
    \return     deserialized authentic pdu length.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(PduLengthType, SECOC_CODE) SecOC_GetLengthFromHeader(P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) headerData, uint8 headerLength);
#endif

#if(SECOC_SECUREDPDUDATACONTAINERAUTHPDUHEADERLENGTHUSEDOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_SerializeLengthToHeader
**********************************************************************************************************************/
/*! \brief      Gets the authentic pdu length form the header.
    \details    -
    \pre        -
    \param[out] dest  pointer to header.
    \param[in]  headerLength length of the header in bytes.
    \param[[in] authenticPduLength
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_SerializeLengthToHeader(P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) dest, uint8 headerLength, PduLengthType authenticPduLength);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_RxIf_InitPdus
**********************************************************************************************************************/
/*! \brief      Init IF rx PDUs.
    \details    -
    \pre        -
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_RxIf_InitPdus(void);
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_InitPdus
**********************************************************************************************************************/
/*! \brief      Init IF tx PDUs.
    \details    -
    \pre        -
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_InitPdus(void);
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_Verify_MACUsingCSM43
**********************************************************************************************************************/
/*! \brief      Verifies the MAC of a Secured I-Pdu using the CSM.
    \details  -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \return     verification result.
    \trace      SPEC-2697662
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_VerifyResultType, SECOC_CODE) SecOC_Verify_MACUsingCSM43(SecOC_RxPduInfoIterType rxPduId);
#endif

#if ((SECOC_RXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_ON))
/**********************************************************************************************************************
  SecOC_Verify_MACUsingCustomCallout
**********************************************************************************************************************/
/*! \brief      Verifies the MAC of a Secured I-Pdu using the custom callout.
    \details  -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \return     verification result.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_VerifyResultType, SECOC_CODE) SecOC_Verify_MACUsingCustomCallout(SecOC_RxPduInfoIterType rxPduId);
#endif

#if (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_GetVerificationResultOfOverrideStatus
**********************************************************************************************************************/
/*! \brief      Translates the verify override status in a verification result.
    \details  -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \return     verification result.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_VerifyResultType, SECOC_CODE) SecOC_Verify_GetVerificationResultOfOverrideStatus(SecOC_RxPduInfoIterType rxPduId);
#endif

#if (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_ProcessVerificationResult
**********************************************************************************************************************/
/*! \brief      Processes the passed verification result.
    \details    -
    \pre        -
    \param[in]  verificationResult verification result.
    \param[in]  rxPduId  rx PduId
    \param[in]  verifyOverrideStatus verify override status
    \trace      SPEC-2697654, SPEC-2697659, SPEC-2968995
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_ProcessVerificationResult(SecOC_VerifyResultType verificationResult, SecOC_RxPduInfoIterType rxPduId, SecOC_VerifyOverrideStatusType verifyOverrideStatus);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_ProcessSuccessfulVerificationResult
**********************************************************************************************************************/
/*! \brief      Processes the successful verification result.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \param[in]  verificationResultType verification result type
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_ProcessSuccessfulVerificationResult(SecOC_RxPduInfoIterType rxPduId, SecOC_VerificationResultType verificationResultType);
#endif

#if ((SECOC_IGNORE_VERIFICATION_RESULT == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_Verify_ProcessUnsuccessfulVerificationResult
**********************************************************************************************************************/
/*! \brief      Processes the unsuccessful verification result.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_ProcessUnsuccessfulVerificationResult(SecOC_RxPduInfoIterType rxPduId);
#endif

#if (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_ProcessNoVerificationResult
**********************************************************************************************************************/
/*! \brief      Process the 'no verification' result to the upper layer.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_ProcessNoVerificationResult(SecOC_RxPduInfoIterType rxPduId);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_RxIf_ForwardPduToUpperLayer
**********************************************************************************************************************/
/*! \brief      Forwards the Pdu to the upper layer module.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \param[in]  pduInfoPtr PduInfo pointer
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_RxIf_ForwardPduToUpperLayer(SecOC_RxPduInfoIterType rxPduId, P2VAR(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) pduInfoPtr);
#endif

#if((SECOC_DEVMODEENABLED == STD_ON) && (SECOC_RXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_RxIf_ForwardPduToUpperLayerDevMode
**********************************************************************************************************************/
/*! \brief      Forwards the Pdu to the upper layer module in case development mode is active and a non-recoverrable error was detected.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_RxIf_ForwardPduToUpperLayerDevMode(SecOC_RxPduInfoIterType rxPduId);
#endif

#if (SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfRxSecuredPduDataContainer
**********************************************************************************************************************/
/*! \brief      Returns the start index of the secured PDU data container.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \return     Start index of the secured PDU data container.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerSecuredPduStartIdxOfRxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfRxSecuredPduDataContainer(SecOC_RxPduInfoIterType rxPduId);
#endif

#if (SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfRxAuthenticPduDataContainer
**********************************************************************************************************************/
/*! \brief      Returns the start index of the authentic PDU data container.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \return     Start index of the authentic PDU data container.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_AuthenticPduDataContainerAuthenticPduStartIdxOfRxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfRxAuthenticPduDataContainer(SecOC_RxPduInfoIterType rxPduId);
#endif

#if (SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetLengthOfRxAuthenticPduDataContainer
**********************************************************************************************************************/
/*! \brief      Returns the length of the authentic PDU data container.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \return     length of the authentic PDU data container.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_AuthenticPduDataContainerAuthenticPduLengthOfRxPduInfoType, SECOC_CODE) SecOC_GetLengthOfRxAuthenticPduDataContainer(SecOC_RxPduInfoIterType rxPduId);
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_GetStartIdxOfRxAuthenticPduDataContainerDataToAuthenticate
**********************************************************************************************************************/
/*! \brief      Returns the start index of the authentic PDU data container.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \return     start index of the authentic PDU data container.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_AuthenticPduDataContainerDataToAuthenticateStartIdxOfRxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfRxAuthenticPduDataContainerDataToAuthenticate(SecOC_RxPduInfoIterType rxPduId);
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_GetLengthOfRxRxAuthenticPduDataContainerDataToAuthenticate
**********************************************************************************************************************/
/*! \brief      Returns the length of the authentic PDU data container.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \return     length of the authentic PDU data container.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_AuthenticPduDataContainerDataToAuthenticateLengthOfRxPduInfoType, SECOC_CODE) SecOC_GetLengthOfRxRxAuthenticPduDataContainerDataToAuthenticate(SecOC_RxPduInfoIterType rxPduId);
#endif

#if ((SECOC_RXPDUINFO == STD_ON) && (SECOC_TRANSPORT_PROTOCOL == STD_ON))
/**********************************************************************************************************************
  SecOC_GetStartIdxOfRxSecuredPduDataContainerSecuredPdu
**********************************************************************************************************************/
/*! \brief      Returns the start index of the Secured Pdu.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \return     start index of the Secured Pdu.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerSecuredPduStartIdxOfRxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfRxSecuredPduDataContainerSecuredPdu(SecOC_RxPduInfoIterType rxPduId);
#endif

#if (((SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO == STD_ON) && (SECOC_VERIFICATIONSTATUSCALLOUTWITHSECUREDPDU == STD_ON)) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_GetLengthOfRxSecuredPduDataContainerSecuredPdu
**********************************************************************************************************************/
/*! \brief      Returns the length of the Secured Pdu.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \return     length of the Secured Pdu.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerSecuredPduLengthOfRxPduInfoType, SECOC_CODE) SecOC_GetLengthOfRxSecuredPduDataContainerSecuredPdu(SecOC_RxPduInfoIterType rxPduId);
#endif

#if ((SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO == STD_ON) && (SECOC_TRANSPORT_PROTOCOL == STD_ON))
/**********************************************************************************************************************
  SecOC_GetLengthOfRxTpSecuredPduDataContainerSecuredPdu
**********************************************************************************************************************/
/*! \brief      Returns the length of the Secured Tp Pdu.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \return     length of the Secured Pdu.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerTpSecuredPduLengthOfRxPduInfoType, SECOC_CODE) SecOC_GetLengthOfRxTpSecuredPduDataContainerSecuredPdu(SecOC_RxPduInfoIterType rxPduId);
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_AddRxDataIdsToDataToAuthenticate
**********************************************************************************************************************/
/*! \brief      Adds the Data Id to data to authenticate.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_AddRxDataIdsToDataToAuthenticate(SecOC_RxPduInfoIterType rxPduId);
#endif

#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_AddTxDataIdsToDataToAuthenticate
**********************************************************************************************************************/
/*! \brief      Adds the Data Id to data to authenticate.
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_AddTxDataIdsToDataToAuthenticate(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_AUTHENTICPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfTxAuthenticPduDataContainerData
**********************************************************************************************************************/
/*! \brief      Returns the start index of the authentic PDU data container.
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
    \return     start index of the authentic PDU data container.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_AuthenticPduDataContainerAuthenticPduStartIdxOfTxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfTxAuthenticPduDataContainerData(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_SECUREDPDUDATACONTAINERAUTHPDUHEADERLENGTHUSEDOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfTxAuthPduHeader
**********************************************************************************************************************/
/*! \brief      Returns the start index of the Authentic Pdu Header.
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
    \return     start index of the authentic PDU data container.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerAuthPduHeaderLengthStartIdxOfTxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfTxAuthPduHeader(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_AUTHENTICPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfTxSecuredPduDataContainerData
**********************************************************************************************************************/
/*! \brief      Returns the start index of the secured PDU data container.
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
    \return     start index of the secured PDU data container.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerAuthenticPduStartIdxOfTxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfTxSecuredPduDataContainerData(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_AUTHENTICPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetLengthOfTxSecuredPduDataContainerData
**********************************************************************************************************************/
/*! \brief      Returns the length of the secured PDU data container.
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
    \return     length of the secured PDU data container.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerAuthenticPduLengthOfTxPduInfoType, SECOC_CODE) SecOC_GetLengthOfTxSecuredPduDataContainerData(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_SECUREDPDUDATACONTAINERSECUREDPDUSTARTIDXOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfTxSecuredPdu
**********************************************************************************************************************/
/*! \brief      Returns the start index of the secured PDU.
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
    \return     start index of the secured PDU data container.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerSecuredPduStartIdxOfTxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfTxSecuredPdu(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_SECUREDPDUDATACONTAINERSECUREDPDUSTARTIDXOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetLengthOfTxSecuredPdu
**********************************************************************************************************************/
/*! \brief      Returns the length of the secured PDU.
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
    \return     length of the secured PDU data container.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerSecuredPduLengthOfTxPduInfoType, SECOC_CODE) SecOC_GetLengthOfTxSecuredPdu(SecOC_TxPduInfoIterType txPduId);
#endif

#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_GetStartIdxOfTxDataToAuthenticate
**********************************************************************************************************************/
/*! \brief      Returns the start index of data to authenticate.
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
    \return     start index of data to authenticate.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_AuthenticPduDataContainerDataToAuthenticateStartIdxOfTxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfTxDataToAuthenticate(SecOC_TxPduInfoIterType txPduId);
#endif

#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_GetLengthOfDataToAuthenticate
**********************************************************************************************************************/
/*! \brief      Returns the length of the data to authenticate.
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
    \return     length of data to authenticate.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_AuthenticPduDataContainerDataToAuthenticateLengthOfTxPduInfoType, SECOC_CODE) SecOC_GetLengthOfDataToAuthenticate(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_MESSAGELINKLENOFTXSPLITPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfTxMessageLinkerAuthSplitPdu
**********************************************************************************************************************/
/*! \brief      Returns the start index of the message linker.
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
    \return     start index of the message linker.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerAuthenticSplitPduStartIdxOfTxSplitPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfTxMessageLinkerAuthSplitPdu(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_TXSPLITPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfTxAuthenticSplitPdu
**********************************************************************************************************************/
/*! \brief      Returns the start index of authenticate split.
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
    \return     start index of the authenticate split.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerAuthenticSplitPduStartIdxOfTxSplitPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfTxAuthenticSplitPdu(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_TXSPLITPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetLengthOfTxAuthenticSplitPdu
**********************************************************************************************************************/
/*! \brief      Returns the length of authentic split pdu.
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
    \return     length of authentic split pdu.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerAuthenticSplitPduLengthOfTxSplitPduInfoType, SECOC_CODE) SecOC_GetLengthOfTxAuthenticSplitPdu(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_CONTAINERHEADERSIZEOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_RxIf_RemoveCanFdContainerPadding
**********************************************************************************************************************/
/*! \brief      Removes the Can Fd Padding from the related container PDU.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_RxIf_RemoveCanFdContainerPadding(SecOC_RxPduInfoIterType rxPduId);
#endif

#if (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_MAC_DoUnverifiedProcessing
**********************************************************************************************************************/
/*! \brief      Do the processing of the state Unverified.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_MAC_DoUnverifiedProcessing(SecOC_RxPduInfoIterType rxPduId);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_RxIf_HandlePdus
**********************************************************************************************************************/
/*! \brief      Handle IF rx PDUs.
    \details    -
    \pre        -
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_RxIf_HandlePdus(void);
#endif

#if (SECOC_CSMJOBTIMEOUTCNTOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_DlMon_RxCSMTimeoutSupervision
**********************************************************************************************************************/
/*! \brief      Processes an active CSM timeout counter.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_DlMon_RxCSMTimeoutSupervision(SecOC_RxPduInfoIterType rxPduId);
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_FreshnessValue_CopyVerifyValueToBuffer
******************************************************************************************** **************************/
/*! \brief      Copys the FreshnessVerifyValue to the Data To Authenticate Buffer.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \param[in]  freshnessVerifyValue  the freshness value to verify
    \return     parsed freshness value
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_CopyVerifyValueToBuffer(SecOC_RxPduInfoIterType rxPduId, P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) freshnessVerifyValue);
#endif

#if (((SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFRXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF)) && (SECOC_USE_MSB_ALIGNED_TRUNCATED_FRESHNESS_VALUE_IN_FVM == STD_ON))
/**********************************************************************************************************************
  SecOC_FreshnessValue_GetParsedFreshnessValueMSBAligned
**********************************************************************************************************************/
/*! \brief      Returns the parsed freshness value.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \param[out] parsedFreshnessValue parsed freshness value.
    \param[in]  parsedFreshnessValueBufferLength parsed freshness value buffer length
    \return     parsed freshness value
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_GetParsedFreshnessValueMSBAligned(SecOC_RxPduInfoIterType rxPduId, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) parsedFreshnessValue, uint8 parsedFreshnessValueBufferLength);
#endif

#if (((SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFRXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF)) && (SECOC_USE_MSB_ALIGNED_TRUNCATED_FRESHNESS_VALUE_IN_FVM == STD_OFF))
/**********************************************************************************************************************
  SecOC_FreshnessValue_GetParsedFreshnessValue
**********************************************************************************************************************/
/*! \brief      Returns the parsed freshness value.
    \details    -
    \pre        -
    \param[in]  rxPduId  rx PduId
    \param[out] parsedFreshnessValue parsed freshness value.
    \param[in]  parsedFreshnessValueBufferLength parsed freshness value buffer length
    \return     parsed freshness value
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_GetParsedFreshnessValue(SecOC_RxPduInfoIterType rxPduId, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) parsedFreshnessValue, uint8 parsedFreshnessValueBufferLength);
#endif

#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_Authenticate_GenerateAuthenticatorUsingCSM43
**********************************************************************************************************************/
/*! \brief      generates the MAC using the CSM
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
    \return     Std_ReturnType
                E_OK:      generation of the MAC using CSM was successful
                E_NOT_OK:  generation of the MAC using CSM was not successful
				SECOC_E_BUSY: generation of the MAC using CSM was not successful because CSM was busy
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_Authenticate_GenerateAuthenticatorUsingCSM43(SecOC_TxPduInfoIterType txPduId);
#endif

#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_ON))
/**********************************************************************************************************************
  SecOC_Authenticate_GenerateAuthenticatorUsingCustomCallout
**********************************************************************************************************************/
/*! \brief      generates the MAC using a customer specific callout
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
    \return     Std_ReturnType
                E_OK:      generation of the MAC was successful
                E_NOT_OK:  generation of the MAC was not successful
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_Authenticate_GenerateAuthenticatorUsingCustomCallout(SecOC_TxPduInfoIterType txPduId);
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_FreshnessValue_CalculateVerifyValueFromCallout
**********************************************************************************************************************/
/*! \brief      Gets the freshness verify value for a Rx Pdu form a configured callout
    \details    -
    \pre        -
    \param[in]  rxPduId  Rx Pdu Id
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_CalculateVerifyValueFromCallout(SecOC_RxPduInfoIterType rxPduId);
#endif

#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_FreshnessValue_GetValueForTxPduOfCallout
**********************************************************************************************************************/
/*! \brief      Gets the freshness verify value for a Tx Pdu form a configured callout
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
    \param[out] freshnessValue freshness verify value
    \param[in]  freshnessValueBufferLength length of the freshness verify value buffer
    \param[out] truncatedFreshnessValue truncated transmit value
    \param[in]  truncatedFreshnessValueBufferLength length of the truncated freshness value buffer
    \return     STD_ON  if successful
                STD_OFF if failed
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_FreshnessValue_GetValueForTxPduOfCallout(SecOC_TxPduInfoIterType txPduId, uint8 * freshnessValue, uint8 freshnessValueBufferLength, uint8 * truncatedFreshnessValue, uint8 truncatedFreshnessValueBufferLength);
#endif

#if ((SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFTXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_FreshnessValue_WriteTruncFreshnessValue
**********************************************************************************************************************/
/*! \brief      Calls either the copy freshness array value or the copy freshness pattern value function
    \details    -
    \pre        -
    \param[in]  txPduId              tx PduId
    \param[in]  trucFreshnessValue   truncated freshness value
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_WriteTruncFreshnessValue(SecOC_TxPduInfoIterType txPduId, P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) trucFreshnessValue);
#endif


#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_FreshnessValue_CopyTxFreshnessValues
**********************************************************************************************************************/
/*! \brief      Copys the freshness value to the Data To Authenticate Buffer.
    \details    -
    \pre        -
    \param[in]  txPduId              tx PduId
    \return     E_OK if successful, E_NOT_OK if failed.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_FreshnessValue_CopyTxFreshnessValues(SecOC_TxPduInfoIterType txPduId);
#endif

#if ((SECOC_RXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_Verify_PropagateVerificationStatus
**********************************************************************************************************************/
/*! \brief      Calls all verification status Callouts.
    \details    -
    \pre        -
    \param[in]  rxPduId              rx PduId
    \param[in]  verificationStatus   verification status
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_PropagateVerificationStatus(SecOC_RxPduInfoIterType rxPduId, SecOC_VerificationResultType verificationStatus);
#endif

#if ((SECOC_RXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_Verify_PropagateVerificationStatusWithSecuredPdu
**********************************************************************************************************************/
/*! \brief      Calls all verification status Callouts with information about secured PDU and freshness value.
    \details    -
    \pre        -
    \param[in]  rxPduId              rx PduId
    \param[in]  verificationStatus   verification status
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_PropagateVerificationStatusWithSecuredPdu(SecOC_RxPduInfoIterType rxPduId, SecOC_VerificationResultType verificationStatus);
#endif

#if ((SECOC_RXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_Verify_PropagateClientServerVerificationStatus
**********************************************************************************************************************/
/*! \brief      Calls the verification status Client Server RTE interface with information about secured PDU and freshness value.
    \details    -
    \pre        -
    \param[in]  rxPduId              rx PduId
    \param[in]  verificationStatus   verification status
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_PropagateClientServerVerificationStatus(SecOC_RxPduInfoIterType rxPduId, SecOC_VerificationResultType verificationStatus);
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Authenticate_GenerateAuthenticator
**********************************************************************************************************************/
/*! \brief      Generates the authenticator of the secured PDU.
    \details    -
    \pre        -
    \param[in]  txPduId              tx PduId
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Authenticate_GenerateAuthenticator(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_HandlePdu_ReauthenticationNeeded
**********************************************************************************************************************/
/*! \brief      Processes the REAUTHENTICATION_NEEDED state of the passed Tx Pdu.
 *  \details    -
 *  \pre        -
    \param[in]  txPduId  Pdu Id
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_HandlePdu_ReauthenticationNeeded(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_EXISTS_TPPDU_PDUTYPEOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_HandlePdu_TPPDU
**********************************************************************************************************************/
/*! \brief      Handle the passed Tp Pdu.
 *  \details    -
 *  \pre        -
    \param[in]  txPduId  Pdu Id
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_HandlePdu_TPPDU(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_TXSPLITPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_HandlePdu_SplitPdu
**********************************************************************************************************************/
/*! \brief      Handle the transmission of the related split Pdu.
    \details    -
    \pre        -
    \param[in]  txPduId  Pdu Id
    \return     E_OK if transmit successful
                E_NOT_OK if transmit not successful
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_TxIf_HandlePdu_SplitPdu(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_ADDCANFDCONTAINERPDUPADDINGOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_CalculatePaddingSize
**********************************************************************************************************************/
/*! \brief      Calculates the padding size.
    \details    -
    \pre        -
    \param[in]  securedPduLength secured Pdu length
    \return     calculated padding size
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(uint8, SECOC_CODE) SecOC_TxIf_CalculatePaddingSize(SecOC_SecuredPduDataContainerAuthenticPduLengthOfTxPduInfoType securedPduLength);
#endif

#if (SECOC_ADDCANFDCONTAINERPDUPADDINGOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_AddCANFDPadding
**********************************************************************************************************************/
/*! \brief      Adds the CAN FD Padding bytes to the Secured Pdu
    \details    -
    \pre        -
    \param[in]  txPduId Pdu id
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_AddCANFDPadding(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_HandlePdus
**********************************************************************************************************************/
/*! \brief      Handles Tx Pdus.
    \details    -
    \pre        -
    \trace      SPEC-2697631, SPEC-2697636, SPEC-2697642, SPEC-2697701
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_HandlePdus(void);
#endif

#if (SECOC_CSMJOBTIMEOUTCNTOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_DlMon_TxCSMTimeoutSupervision
**********************************************************************************************************************/
/*! \brief      Processes an active CSM timeout counter.
    \details    -
    \pre        -
    \param[in]  txPduId  tx PduId
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_DlMon_TxCSMTimeoutSupervision(SecOC_TxPduInfoIterType txPduId);
#endif


#if (SECOC_CURRENTMESSAGELINKER == STD_ON)
/**********************************************************************************************************************
  SecOC_Split_CheckMessageLinker
**********************************************************************************************************************/
/*! \brief          Compares the passed message linker values.
 *                  If the new value is different, the old value is replaced with the new.
 *  \details        -
 *  \pre            -
    \param[in]      msgLinkNew    new message linker value.
 *  \param[in,out]  msgLinkOld    old message linker value.
 *  \param[in]      msgLinkLenInBytes   message linker length in bytes.
 *  \param[in]      currentIsNewStatus  current is new status.
 *  \return         TRUE          if the new message linker is different.
 *                  FALSE         if the message linker are equal.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(boolean, SECOC_CODE) SecOC_Split_CheckMessageLinker(P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) msgLinkNew, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) msgLinkOld, SecOC_CurrentMessageLinkerLengthOfRxSecuredPduInfoType msgLinkLenInBytes, boolean currentIsNewStatus);
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) || (SECOC_CURRENTMESSAGELINKER == STD_ON) || (SECOC_MESSAGELINKLENOFTXSPLITPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_GetArrayByteAligned
**********************************************************************************************************************/
/*! \brief          Copies the data form the source buffer to the dest buffer considering source and destination offsets.
 *  \details  -
    \param[in]      srcBuffer    source buffer
 *  \param[out]     destBuffer   destination buffer
 *  \param[in]      copyBitLen   length in bits.
 *  \param[in]      srcStartBitInByte  source bit in byte offset.
 *  \param[in]      destBitOffset   dest bit in byte offset.
 *  \param[in]      isSrcRightAligned  TRUE if source bits are right aligned FALSE if source is bits are left aligned
 *  \pre             Module must be initialized
 *  \context      TASK|ISR2
 *  \synchronous TRUE
 *  \reentrant  TRUE
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_GetArrayByteAligned(P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) srcBuffer, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) destBuffer, uint16 copyBitLen, uint8 srcStartBitInByte, uint8 destBitOffset, boolean isSrcRightAligned);
#endif

#if (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_IncrementBuildAttempts
**********************************************************************************************************************/
/*! \brief      Increments Build Attempts and checks if build attempts threshold is reached. This function is only used internally!
 *  \details    -
 *  \param[in]  rxPduId  Rx Pdu Id
 *  \pre         Module must be initialized
 *  \context     TASK|ISR2
 *  \synchronous TRUE
 *  \reentrant  TRUE, for different handle IDs.
 *  \return     E_OK     build attempts threshold not reached.
 *              E_NOT_OK build attempts threshold reached.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_IncrementBuildAttempts(SecOC_RxPduInfoIterType rxPduId);
#endif

#if ((SECOC_RXSECUREDPDUINFO == STD_ON) && (SECOC_COMMUNICATION_INTERFACE == STD_ON))
/**********************************************************************************************************************
  SecOC_RxIndication_PDUProcessing
**********************************************************************************************************************/
/*! \brief      Processes a received PDU.
 *  \details    -
 *  \param[in]  id  Rx Pdu Id
 *  \param[in]  info     A pointer to a structure with Secured I-PDU related data:
 *                       data length and pointer to I-SDU buffer.
 *  \pre         Module must be initialized
 *  \context     TASK|ISR2
 *  \synchronous TRUE
 *  \reentrant  TRUE, for different handle IDs.
 *  \return     TRUE     PDU was processed.
 *              FALSE    PDU was not processed.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(boolean, SECOC_CODE) SecOC_RxIndication_PDUProcessing(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info);
#endif

#if (SECOC_PDUSWITHQUEUESIDXOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Queue_IsQueueEmpty
**********************************************************************************************************************/
/*! \brief      Checks if the queue is empty.
 *  \details    -
 *  \param[in]  pdusWithQueuesIdx  Index of PdusWithQueues
 *  \pre         Module must be initialized
 *  \context     TASK|ISR2
 *  \synchronous TRUE
 *  \reentrant  TRUE, for different handle IDs.
 *  \return     TRUE     Queue is empty.
 *              FALSE    Queue is not empty.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(boolean, SECOC_CODE) SecOC_Queue_IsQueueEmpty(SecOC_PdusWithQueuesIdxOfRxPduInfoType pdusWithQueuesIdx);
#endif

#if (SECOC_PDUSWITHQUEUESIDXOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Queue_AddPDUToQueue
**********************************************************************************************************************/
/*! \brief      Adds the passed PDU to the queue.
 *  \details    -
 *  \param[in]  rxPduInfoIdx  Rx Pdu Info Id
 *  \param[in]  info     A pointer to a structure with Secured I-PDU related data:
 *                       data length and pointer to I-SDU buffer.
 *  \pre         Module must be initialized
 *  \context     TASK|ISR2
 *  \synchronous TRUE
 *  \reentrant  TRUE, for different handle IDs.
 *  \return     TRUE     PDU was queued.
 *              FALSE    PDU was not queued.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(boolean, SECOC_CODE) SecOC_Queue_AddPDUToQueue(SecOC_RxPduInfoIterType rxPduInfoIdx, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info);
#endif

#if (SECOC_PDUSWITHQUEUESIDXOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Queue_LoadNextQueueElement
**********************************************************************************************************************/
/*! \brief      Load PDU from queue to processing buffer.
 *  \details    -
 *  \param[in]  rxPduInfoIdx  Rx Pdu Info Id
 *  \pre         Module must be initialized
 *  \context     TASK|ISR2
 *  \synchronous TRUE
 *  \reentrant  TRUE, for different handle IDs.
 *  \return     TRUE     New element was loaded.
 *              FALSE    No new element was loaded.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(boolean, SECOC_CODE) SecOC_Queue_LoadNextQueueElement(SecOC_RxPduInfoIterType rxPduInfoIdx);
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Authenticate_IncrementAndCheckBuildAttempts
**********************************************************************************************************************/
/*! \brief      Increments the build attempts counter and checks if the configured build attempts threshold is reached.
 *  \details    -
 *  \param[in]  txPduId              tx PduId
 *  \pre         Module must be initialized
 *  \context     TASK|ISR2
 *  \synchronous TRUE
 *  \reentrant  TRUE, for different handle IDs.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Authenticate_IncrementAndCheckBuildAttempts(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_ProcessTxPdu
**********************************************************************************************************************/
/*! \brief      Handles Tx Pdu.
 *  \details    -
 *  \param[in]  txPduId             Handle of Tx Pdu to be processed.
 *  \pre         Module must be initialized
 *  \context     TASK|ISR2
 *  \synchronous TRUE
 *  \return     E_OK     tramsmit to lower layer was successful.
 *              E_NOT_OK transmit to lower layer was not successful.
 *  \reentrant  TRUE, for different handle IDs.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_TxIf_ProcessTxPdu(SecOC_TxPduInfoIterType txPduId);
#endif

#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_SENDDEFAULTAUTHENTICATION == STD_ON))
/**********************************************************************************************************************
  SecOC_CopyAuthenticationPatternIntoMacAndFvMValue
**********************************************************************************************************************/
/*! \brief      Overwrites the MAC and the FvM value with the authenticationPattern.
 *  \details  -
 *  \param[in]  txPduId  Pdu Id
 *  \pre         Module must be initialized
 *  \context     TASK|ISR2
 *  \synchronous TRUE
 *  \reentrant  TRUE, for different handle IDs.
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_CopyAuthenticationPatternIntoMacAndFvMValue(SecOC_TxPduInfoIterType txPduId);
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_MAC
**********************************************************************************************************************/
/*! \brief       Verifies the MAC of a Secured I-Pdu.
 *  \details     -
 *  \param[in]   rxPduId  rx PduId
 *  \pre         Module must be initialized
 *  \context     TASK|ISR2
 *  \synchronous TRUE
 *  \reentrant   TRUE, for diferrent Handle IDs
 *  \trace       SPEC-2977504
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_MAC(SecOC_RxPduInfoIterType rxPduId);
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_FreshnessValue_GetValueForTxPduOfCallout
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_FreshnessValue_GetValueForTxPduOfCallout(SecOC_TxPduInfoIterType txPduId, uint8 * freshnessValue, uint8 freshnessValueBufferLength, uint8 * truncatedFreshnessValue, uint8 truncatedFreshnessValueBufferLength)     /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
  Std_ReturnType ret = E_NOT_OK;
  uint32 freshnessValueLength;
  freshnessValueLength = ((uint32) (SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueLengthOfTxPduInfo(txPduId)) << 3u);

# if ((SECOC_USE_GET_TX_FRESHNESS_TRUNC_DATA_API == STD_ON) && (SECOC_USE_GET_TX_FRESHNESS_API == STD_ON))
  if(SecOC_GetProvideTruncTxFreshnessOfTxPduInfo(txPduId) == SECOC_PROVIDE_PROVIDETRUNCTXFRESHNESSOFTXPDUINFO)
# endif
# if (SECOC_USE_GET_TX_FRESHNESS_TRUNC_DATA_API == STD_ON)
  {
    uint32 truncatedFreshnessValueLength;
    truncatedFreshnessValueLength = (uint32) SecOC_GettruncatedFreshnessValueLengthOfTxPduInfo(txPduId);
    if((truncatedFreshnessValueLength <= ((uint32) (truncatedFreshnessValueBufferLength) << 3u)) && (freshnessValueLength <= ((uint32) (freshnessValueBufferLength) << 3u)))    /* COV_SECOC_SAFEBSW_CHECK */
    {
      /* *INDENT-OFF* */
      ret = GetTxFreshnessTruncData_FuncName(
          SecOC_GetExternalFreshnessIdOfTxPduInfo(txPduId),
          freshnessValue,
          &freshnessValueLength,
          truncatedFreshnessValue,
          &truncatedFreshnessValueLength
      ); /* SBSW_SECOC_PTR2FUNC_6 */
      /* *INDENT-ON* */
    }
  }
# endif
# if ((SECOC_USE_GET_TX_FRESHNESS_TRUNC_DATA_API == STD_ON) && (SECOC_USE_GET_TX_FRESHNESS_API == STD_ON))
  else
# endif
# if (SECOC_USE_GET_TX_FRESHNESS_API == STD_ON)
  {
    if(freshnessValueLength <= ((uint32) (freshnessValueBufferLength) << 3u))   /* COV_SECOC_SAFEBSW_CHECK */
    {
      /* *INDENT-OFF* */
      ret = GetTxFreshness_FuncName(
          SecOC_GetExternalFreshnessIdOfTxPduInfo(txPduId),
          freshnessValue,
          &freshnessValueLength
      ); /* SBSW_SECOC_PTR2FUNC_6 */
      /* *INDENT-ON* */
#  if (SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFTXPDUINFO == STD_ON)
      VStdLib_MemCpy_s(truncatedFreshnessValue, truncatedFreshnessValueBufferLength, &(freshnessValue[SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueLengthOfTxPduInfo(txPduId) - SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueLengthOfTxPduInfo(txPduId)]), SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueLengthOfTxPduInfo(txPduId));       /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_VSTDLIB_SAFE */
#  endif
    }
  }
# endif
  SECOC_DUMMY_STATEMENT(truncatedFreshnessValueBufferLength);   /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(truncatedFreshnessValue);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  return ret;
}
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_FreshnessValue_CalculateVerifyValueFromCallout
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_CalculateVerifyValueFromCallout(SecOC_RxPduInfoIterType rxPduId)
{
  Std_ReturnType result = E_OK; /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 freshnessVerifyValue[SECOC_FRESHNESS_SIZE_RXPDUS] = { 0u };
  result = SecOC_FreshnessValue_GetVerifyValueFromCallout(rxPduId, freshnessVerifyValue, SECOC_FRESHNESS_SIZE_RXPDUS);  /* SBSW_SECOC_PTR2FUNC_2 */
  if(result == E_OK)
  {
    SecOC_FreshnessValue_CopyVerifyValueToBuffer(rxPduId, freshnessVerifyValue);        /* SBSW_SECOC_PTR2FUNC_2 */
    SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_RXPDUSTATE);  /* SBSW_SECOC_CSL02 */
  }
  else if(result == SECOC_E_BUSY)
  {
    /* Increment Build attempts and check state */
    SecOC_Verify_IncrementBuildAttempts(rxPduId);
  }
  else
  {
    SecOC_Verify_PropagateVerificationStatus(rxPduId, SECOC_FRESHNESSFAILURE);
    SecOC_Verify_PropagateVerificationStatusWithSecuredPdu(rxPduId, SECOC_FRESHNESSFAILURE);
    SecOC_ReportRuntimeDetError(SECOC_SID_MAIN_FUNCTION_RX, SECOC_E_FRESHNESS_FAILURE);
# if(SECOC_EXISTS_TPPDU_PDUTYPEOFRXPDUINFO == STD_ON)
#  if(SECOC_DEVMODEENABLED == STD_ON)
    if(!SecOC_IsDevModeEnabled())
#  endif
    {
      if(SecOC_GetPduTypeOfRxPduInfo(rxPduId) == SECOC_TPPDU_PDUTYPEOFRXPDUINFO)        /* COV_SECOC_RX_TP_UP_ONLY */
      {
        PduR_SecOCTpRxIndication(SecOC_GetAuthenticExternalIdOfRxPduInfo(rxPduId), E_NOT_OK);
      }
    }
# endif
    SecOC_SetRxPduState(rxPduId, SECOC_ABORTED_RXPDUSTATE);     /* SBSW_SECOC_CSL02 */
  }
}
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_FreshnessValue_CopyVerifyValueToBuffer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_CopyVerifyValueToBuffer(SecOC_RxPduInfoIterType rxPduId, P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) freshnessVerifyValue)
{
  /* get the index of the last octet of the freshness value in buffer */
  SecOC_AuthenticPduDataContainerFreshnessVerifyValueEndIdxOfRxPduInfoType destValueidx = SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueStartIdxOfRxPduInfo(rxPduId);
  uint8 srcIdx = 0;

  /* iterate from the LSB to the MSB. The address of the MSB will then be smaller then the address of the LSB */
  for(; destValueidx < SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueEndIdxOfRxPduInfo(rxPduId); destValueidx++)
  {
    SecOC_SetAuthenticPduDataContainer(destValueidx, freshnessVerifyValue[srcIdx]);     /* SBSW_SECOC_CSL03 */
    srcIdx++;
  }
}
#endif

#if ((SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFTXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_FreshnessValue_WriteTruncFreshnessValue
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_WriteTruncFreshnessValue(SecOC_TxPduInfoIterType txPduId, P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) trucFreshnessValue)
{
  uint8 firstByteShift = SecOC_GetTruncFreshnessValueFirstByteShiftOfTxPduInfo(txPduId);
  uint8 secondByteShift = SecOC_GetTruncFreshnessValueSecondByteShiftOfTxPduInfo(txPduId);
  SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfTxPduInfoType iterValue = SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueEndIdxOfTxPduInfo(txPduId);
  SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfTxPduInfoType srcValueIdx = (SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfTxPduInfoType) (SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueLengthOfTxPduInfo(txPduId)) - 1u;
  SecOC_truncatedFreshnessValueLengthOfTxPduInfoType bitsToCopy = SecOC_GettruncatedFreshnessValueLengthOfTxPduInfo(txPduId);

  for(; iterValue > SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueStartIdxOfTxPduInfo(txPduId); iterValue--)
  {
    SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfTxPduInfoType destValueIdx = iterValue - 1u;
    if((bitsToCopy & 7u) != 0u)
    {
      uint8 bitOffset = (uint8) (bitsToCopy & 7u);
      uint8 byteToCopy = (uint8) (trucFreshnessValue[srcValueIdx] << firstByteShift);
      uint8 copyMask = (uint8) (255u >> secondByteShift);

      byteToCopy = (SecOC_GetSecuredPduDataContainer(destValueIdx) & copyMask) | byteToCopy;
      SecOC_SetSecuredPduDataContainer(destValueIdx, byteToCopy);       /* SBSW_SECOC_CSL03 */

      bitsToCopy = bitsToCopy - bitOffset;
    }
    else
    {
      uint8 byteToCopy = (uint8) (trucFreshnessValue[srcValueIdx] << firstByteShift);

      if(secondByteShift != 0u)
      {
        byteToCopy = byteToCopy | ((uint8) (trucFreshnessValue[srcValueIdx + 1u] >> secondByteShift));
      }

      SecOC_SetSecuredPduDataContainer(destValueIdx, byteToCopy);       /* SBSW_SECOC_CSL03 */
      bitsToCopy = bitsToCopy - 8u;
    }
    srcValueIdx--;
  }
}
#endif


#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_FreshnessValue_CopyTxFreshnessValues
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_FreshnessValue_CopyTxFreshnessValues(SecOC_TxPduInfoIterType txPduId)
{
  uint8 freshnessValue[SECOC_FRESHNESS_SIZE_TXPDUS] = { 0u };
  uint8 truncatedFreshnessValue[SECOC_TRUNC_FRESHNESS_SIZE_TXPDUS] = { 0u };
  Std_ReturnType ret = E_OK;    /* PRQA S 2981 */ /* MD_MSR_RetVal */

  ret = SecOC_FreshnessValue_GetValueForTxPduOfCallout(txPduId, freshnessValue, SECOC_FRESHNESS_SIZE_TXPDUS, truncatedFreshnessValue, SECOC_TRUNC_FRESHNESS_SIZE_TXPDUS);       /* SBSW_SECOC_PTR2FUNC_2 */

  if(ret == E_OK)
  {
    {
      /* get the index of the last octet of the freshness value in buffer */
      SecOC_AuthenticPduDataContainerFreshnessVerifyValueEndIdxOfTxPduInfoType valueidx = SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueStartIdxOfTxPduInfo(txPduId);

      /* iterate from the LSB to the MSB. The address of the MSB will then be smaller then the address of the LSB. */
      for(; valueidx < SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueEndIdxOfTxPduInfo(txPduId); valueidx++)
      {
        SecOC_SetAuthenticPduDataContainer(valueidx, freshnessValue[valueidx - SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueStartIdxOfTxPduInfo(txPduId)]);    /* SBSW_SECOC_CSL03 */
      }
    }
# if (SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFTXPDUINFO == STD_ON)
    SecOC_FreshnessValue_WriteTruncFreshnessValue(txPduId, truncatedFreshnessValue);    /* SBSW_SECOC_PTR2FUNC_2 */
# endif
  }
  return ret;
}
#endif

#if (((SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFRXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF)) && (SECOC_USE_MSB_ALIGNED_TRUNCATED_FRESHNESS_VALUE_IN_FVM == STD_ON))
/**********************************************************************************************************************
  SecOC_FreshnessValue_GetParsedFreshnessValueMSBAligned
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_GetParsedFreshnessValueMSBAligned(SecOC_RxPduInfoIterType rxPduId, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) parsedFreshnessValue, uint8 parsedFreshnessValueBufferLength)
{
  if(parsedFreshnessValueBufferLength >= SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueLengthOfRxPduInfo(rxPduId))     /* COV_SECOC_SAFEBSW_CHECK */
  {
    SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfRxPduInfoType iterValueidx = SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueStartIdxOfRxPduInfo(rxPduId);
    SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfRxPduInfoType destValueIdx = 0u;
    SecOC_truncatedFreshnessValueLengthOfRxPduInfoType truncatedFreshnessValueBitsToCopy = SecOC_GettruncatedFreshnessValueLengthOfRxPduInfo(rxPduId);

    for(; iterValueidx < SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueEndIdxOfRxPduInfo(rxPduId); iterValueidx++)
    {
      uint8 mask = 0xFFu;
      if(truncatedFreshnessValueBitsToCopy < 8u)
      {
        mask = mask << (8u - truncatedFreshnessValueBitsToCopy);
        truncatedFreshnessValueBitsToCopy = 0u;
      }
      else
      {
        truncatedFreshnessValueBitsToCopy = truncatedFreshnessValueBitsToCopy - 8u;
      }
      parsedFreshnessValue[destValueIdx] = (uint8) (SecOC_GetSecuredPduDataContainer(iterValueidx) & mask);     /* SBSW_SECOC_PTR2FUNC_4 */
      destValueIdx++;
    }
  }
}
#endif


#if (((SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFRXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF)) && (SECOC_USE_MSB_ALIGNED_TRUNCATED_FRESHNESS_VALUE_IN_FVM == STD_OFF))
/**********************************************************************************************************************
  SecOC_FreshnessValue_GetParsedFreshnessValue
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_FreshnessValue_GetParsedFreshnessValue(SecOC_RxPduInfoIterType rxPduId, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) parsedFreshnessValue, uint8 parsedFreshnessValueBufferLength)
{
  if(parsedFreshnessValueBufferLength >= SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueLengthOfRxPduInfo(rxPduId))     /* COV_SECOC_SAFEBSW_CHECK */
  {
    SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfRxPduInfoType iterValueidx = SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueEndIdxOfRxPduInfo(rxPduId);
    SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfRxPduInfoType destValueIdx = (SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfRxPduInfoType) (SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueLengthOfRxPduInfo(rxPduId)) - 1u;
    SecOC_truncatedFreshnessValueLengthOfRxPduInfoType truncatedFreshnessValueBitsToCopy = SecOC_GettruncatedFreshnessValueLengthOfRxPduInfo(rxPduId);

    uint8 firstByteShift = SecOC_GetTruncFreshnessValueFirstByteShiftOfRxPduInfo(rxPduId);
    uint8 secondByteShift = SecOC_GetTruncFreshnessValueSecondByteShiftOfRxPduInfo(rxPduId);

    for(; iterValueidx > SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueStartIdxOfRxPduInfo(rxPduId); iterValueidx--)
    {
      SecOC_SecuredPduDataContainerTruncatedFreshnessValueEndIdxOfRxPduInfoType srcValueidx = iterValueidx - 1u;
      if(truncatedFreshnessValueBitsToCopy >= 8u)
      {
        uint8 byteToCopy = (uint8) (SecOC_GetSecuredPduDataContainer(srcValueidx) >> firstByteShift);

        if(secondByteShift != 0u)
        {
          byteToCopy = byteToCopy | ((uint8) (SecOC_GetSecuredPduDataContainer(srcValueidx - 1u) << secondByteShift));
        }

        parsedFreshnessValue[destValueIdx] = byteToCopy;        /* SBSW_SECOC_PTR2FUNC_4 */
        truncatedFreshnessValueBitsToCopy = truncatedFreshnessValueBitsToCopy - 8u;
      }
      else if(truncatedFreshnessValueBitsToCopy > 0u)   /* COV_SECOC_MISRA */
      {
        parsedFreshnessValue[destValueIdx] = (uint8) (SecOC_GetSecuredPduDataContainer(srcValueidx) >> firstByteShift); /* SBSW_SECOC_PTR2FUNC_4 */
        truncatedFreshnessValueBitsToCopy = 0u;
      }
      else
      {
        /* COV_SECOC_MISRA */
      }
      destValueIdx--;
    }
  }
}
#endif

#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_Authenticate_GenerateAuthenticatorUsingCSM43
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_Authenticate_GenerateAuthenticatorUsingCSM43(SecOC_TxPduInfoIterType txPduId)
{
  Std_ReturnType ret = E_NOT_OK;        /* PRQA S 2981 */ /* MD_MSR_RetVal */
  uint8 *dataPtr = SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetStartIdxOfTxDataToAuthenticate(txPduId));    /* PRQA S 3678 */ /* MD_MSR_Rule8.13 */
  uint32 dataLength;

# if (SECOC_TXSECUREDPDUAREAINFOUSEDOFTXPDUINFO == STD_ON)
  if(SecOC_IsTxSecuredPduAreaInfoUsedOfTxPduInfo(txPduId))
  {
    SecOC_TxSecuredPduAreaInfoIdxOfTxPduInfoType securedAreaInfoIdx = SecOC_GetTxSecuredPduAreaInfoIdxOfTxPduInfo(txPduId);
    dataLength = 2u + (uint32) SecOC_GetLengthOfTxSecuredPduAreaInfo(securedAreaInfoIdx) + (uint32) SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueLengthOfTxPduInfo(txPduId);

    /* Silent check, shall never fail for valid configuration */
    if(dataLength <= SecOC_GetSizeOfTxSecuredPduAreaBuffer())   /* COV_SECOC_SILENT_CHECK */
    {
      uint8 *securedAreaBuffer = SecOC_GetAddrTxSecuredPduAreaBuffer(0u);

      /* Copy Data Id */
      VStdMemCpyRamToRam(securedAreaBuffer, dataPtr, 2u);       /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_FCTCALL_SECAREA */

      /* Copy Secured Area */
      VStdMemCpyRamToRam(&securedAreaBuffer[2u], &dataPtr[2u + SecOC_GetOffsetOfTxSecuredPduAreaInfo(securedAreaInfoIdx)], SecOC_GetLengthOfTxSecuredPduAreaInfo(securedAreaInfoIdx));  /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_FCTCALL_SECAREA */

      /* Copy Freshness value */
      VStdMemCpyRamToRam(&securedAreaBuffer[2u + SecOC_GetLengthOfTxSecuredPduAreaInfo(securedAreaInfoIdx)], SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueStartIdxOfTxPduInfo(txPduId)), SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueLengthOfTxPduInfo(txPduId));     /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_FCTCALL_SECAREA */

      dataPtr = securedAreaBuffer;
    }
    else
    {
      SecOC_ReportRuntimeDetError(SECOC_SID_MAIN_FUNCTION_TX, SECOC_E_SECURED_AREA);
    }
  }
  else
# endif
  {
    dataLength = SecOC_GetLengthOfDataToAuthenticate(txPduId);
  }
  SecOC_SetTxResultLength(txPduId, SecOC_GetAuthenticatorCacheLengthOfTxPduInfo(txPduId));      /* SBSW_SECOC_CSL02 */
  /* *INDENT-OFF* */
  ret = Csm_MacGenerate(
      SecOC_GetAuthenticationLayerIdOfTxPduInfo(txPduId),
      CRYPTO_OPERATIONMODE_SINGLECALL,
      dataPtr,
      dataLength,
      SecOC_GetAddrAuthenticatorCache(SecOC_GetAuthenticatorCacheStartIdxOfTxPduInfo(txPduId)),
      SecOC_GetAddrTxResultLength(txPduId)); /* SBSW_SECOC_PTR2FUNC_5 */
  /* *INDENT-ON* */

  if((ret != E_OK) && (ret != SECOC_E_BUSY))
  {
    if(ret == CRYPTO_E_QUEUE_FULL)
    {
      ret = SECOC_E_BUSY;
    }
    else
    {
      ret = E_NOT_OK;
    }
  }

  return ret;
}
#endif

#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_ON))
/**********************************************************************************************************************
  SecOC_Authenticate_GenerateAuthenticatorUsingCustomCallout
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_Authenticate_GenerateAuthenticatorUsingCustomCallout(SecOC_TxPduInfoIterType txPduId)
{
  Std_ReturnType ret;
  PduInfoType txPduInfo;
  SecOC_SecuredPduDataContainerTruncatedFreshnessValueStartIdxOfTxPduInfoType startIdxTruncatedFreshnessValue;
  uint32 txAuthenticatorLength;

  startIdxTruncatedFreshnessValue = SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueStartIdxOfTxPduInfo(txPduId);
  txAuthenticatorLength = (uint32) SecOC_GetAuthInfoLengthOfTxPduInfo(txPduId);

  txPduInfo.SduDataPtr = SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetStartIdxOfTxAuthenticPduDataContainerData(txPduId));
  txPduInfo.SduLength = SecOC_GetSecuredPduDataContainerAuthenticPduLengthOfTxPduInfo(txPduId);

  ret = SecOC_GenerateAuthenticationInfo(SecOC_GetExternalFreshnessIdOfTxPduInfo(txPduId), &txPduInfo, SecOC_GetAddrSecuredPduDataContainer(startIdxTruncatedFreshnessValue), txAuthenticatorLength);   /* SBSW_SECOC_PTR2FUNC_5 */
  if((ret != E_OK) && (ret != SECOC_E_BUSY))
  {
    if(ret == CRYPTO_E_QUEUE_FULL)
    {
      ret = SECOC_E_BUSY;
    }
    else
    {
      ret = E_NOT_OK;
    }
  }
  return ret;
}
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Authenticate_GenerateAuthenticator
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080, 6050 1 */ /* MD_MSR_STMIF, MD_MSR_STCAL */
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Authenticate_GenerateAuthenticator(SecOC_TxPduInfoIterType txPduId)
{
  Std_ReturnType ret = E_NOT_OK;        /* PRQA S 2981 */ /* MD_MSR_RetVal */
  SecOC_TxPduStateType storedState = SecOC_GetTxPduState(txPduId);

# if (SECOC_WAITFORCALLBACKOFTXPDUINFO == STD_ON)
  if(SecOC_IsWaitForCallbackOfTxPduInfo(txPduId))
  {
    SecOC_SetTxPduState(txPduId, SECOC_UNAUTHENTICATED_FINISHED_WAIT_FOR_CALLBACK_TXPDUSTATE);  /* SBSW_SECOC_CSL02 */
  }
# endif
# if (SECOC_CUSTOM_AUTHENTICATION_API == STD_ON)
  ret = SecOC_Authenticate_GenerateAuthenticatorUsingCustomCallout(txPduId);
# else
  SecOC_AddTxDataIdsToDataToAuthenticate(txPduId);
  ret = SecOC_Authenticate_GenerateAuthenticatorUsingCSM43(txPduId);
# endif

  if(ret == E_OK)
  {
# if (SECOC_WAITFORCALLBACKOFTXPDUINFO == STD_ON)
    if(!SecOC_IsWaitForCallbackOfTxPduInfo(txPduId))
# endif
    {
# if (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF)
      SecOC_GetArrayByteAligned(SecOC_GetAddrAuthenticatorCache(SecOC_GetAuthenticatorCacheStartIdxOfTxPduInfo(txPduId)), SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerTruncatedAuthenticatorStartIdxOfTxPduInfo(txPduId)), (uint16) SecOC_GettruncatedAuthenticationLengthOfTxPduInfo(txPduId), 0u, (uint8) ((8u - SecOC_GetTruncAuthStartBitInByteOfTxPduInfo(txPduId)) & 7u), FALSE);        /* SBSW_SECOC_PTR2FUNC_6 */
# endif
      SecOC_SetTxPduState(txPduId, SECOC_UNAUTHENTICATED_FINISHED_TXPDUSTATE);  /* SBSW_SECOC_CSL02 */
    }
# if (SECOC_CSMJOBTIMEOUTCNTOFTXPDUINFO == STD_ON)
    else
    {
      SecOC_SetTxCSMJobTimeoutCntValue(txPduId, SecOC_GetCsmJobTimeoutCntOfTxPduInfo(txPduId)); /* SBSW_SECOC_CSL02 */
    }
# endif
  }
  else if(ret == SECOC_E_BUSY)
  {
    SecOC_SetTxPduState(txPduId, storedState);  /* SBSW_SECOC_CSL02 */
    SecOC_Authenticate_IncrementAndCheckBuildAttempts(txPduId);
  }
  else if(ret == E_NOT_OK)      /* PRQA S 2991, 2995 */ /* MD_SecOC_2991, MD_SecOC_2995 */ /* COV_SECOC_MISRA */
  {
    SecOC_SetTxPduState(txPduId, storedState);  /* SBSW_SECOC_CSL02 */
# if (SECOC_SENDDEFAULTAUTHENTICATION == STD_ON)
    if(SecOC_IsSendDefaultAuthentication(txPduId))
    {
      SecOC_SetTxPduState(txPduId, SECOC_DEVMODE_AUTHENTICATION_PATTERN_NEEDED_TXPDUSTATE);     /* SBSW_SECOC_CSL02 */
    }
    else
# endif
    {
      SecOC_SetTxPduState(txPduId, SECOC_INVALID_SECURED_PDU_TXPDUSTATE);       /* SBSW_SECOC_CSL02 */
      SecOC_SetTransmitRequest(txPduId, SECOC_IDLE_TRANSMITREQUEST);    /* SBSW_SECOC_CSL02 */
# if (SECOC_TXSPLITPDUINFO == STD_ON)
      if(SecOC_IsTxSplitPduInfoUsedOfTxPduInfo(txPduId))        /* COV_SECOC_SPLIT_ONLY_CONFIG */
      {
        SecOC_SizeOfTxSplitPduInfoType splitTxPduId = SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduId);
        SecOC_SetAuthenticSplitPduTransmit(splitTxPduId, SECOC_IDLE_AUTHENTICSPLITPDUTRANSMIT); /* SBSW_SECOC_CSL80 */
        SecOC_SetCryptographicPduTransmit(splitTxPduId, SECOC_IDLE_AUTHENTICSPLITPDUTRANSMIT);  /* SBSW_SECOC_CSL80 */
      }
# endif
# if(SECOC_EXISTS_TPPDU_PDUTYPEOFTXPDUINFO == STD_ON)
      if(SecOC_GetPduTypeOfTxPduInfo(txPduId) == SECOC_TPPDU_PDUTYPEOFTXPDUINFO)        /* COV_SECOC_TX_TP_UP_ONLY */
      {
        PduR_SecOCTpTxConfirmation(SecOC_GetAuthenticExternalIdOfTxPduInfo(txPduId), E_NOT_OK);
      }
# endif
    }
  }
  else
  {
    /* COV_SECOC_MISRA */
  }
}
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_HandlePdu_ReauthenticationNeeded
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_HandlePdu_ReauthenticationNeeded(SecOC_TxPduInfoIterType txPduId)
{
  if(SecOC_GetTxPduState(txPduId) == SECOC_PRE_REAUTHENTICATION_NEEDED_TXPDUSTATE)      /* COV_SECOC_CUSTOM_AUTH_API */
  {
    SecOC_SetTxPduState(txPduId, SECOC_REAUTHENTICATION_NEEDED_TXPDUSTATE);     /* SBSW_SECOC_CSL02 */
  }
  if(SecOC_GetTxPduState(txPduId) == SECOC_REAUTHENTICATION_NEEDED_TXPDUSTATE)
  {
# if (SECOC_CUSTOM_AUTHENTICATION_API == STD_ON)
    SecOC_SetTxPduState(txPduId, SECOC_UNAUTHENTICATED_TO_BE_STARTED_TXPDUSTATE);       /* SBSW_SECOC_CSL02 */
# else
    Std_ReturnType ret = SecOC_FreshnessValue_CopyTxFreshnessValues(txPduId);
    if(ret == E_OK)
    {
      SecOC_SetTxPduState(txPduId, SECOC_UNAUTHENTICATED_TO_BE_STARTED_TXPDUSTATE);     /* SBSW_SECOC_CSL02 */
    }
    else if(ret == E_NOT_OK)
    {
#  if (SECOC_SENDDEFAULTAUTHENTICATION == STD_ON)
      if(SecOC_IsSendDefaultAuthentication(txPduId))
      {
        SecOC_SetTxPduState(txPduId, SECOC_DEVMODE_AUTHENTICATION_PATTERN_NEEDED_TXPDUSTATE);   /* SBSW_SECOC_CSL02 */
      }
      else
#  endif
      {
        SecOC_SetTxPduState(txPduId, SECOC_INVALID_SECURED_PDU_TXPDUSTATE);     /* SBSW_SECOC_CSL02 */
        SecOC_SetTransmitRequest(txPduId, SECOC_IDLE_TRANSMITREQUEST);  /* SBSW_SECOC_CSL02 */
#  if (SECOC_TXSPLITPDUINFO == STD_ON)
        if(SecOC_IsTxSplitPduInfoUsedOfTxPduInfo(txPduId))      /* COV_SECOC_SPLIT_ONLY_CONFIG */
        {
          SecOC_SizeOfTxSplitPduInfoType splitTxPduId = SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduId);
          SecOC_SetAuthenticSplitPduTransmit(splitTxPduId, SECOC_IDLE_AUTHENTICSPLITPDUTRANSMIT);       /* SBSW_SECOC_CSL80 */
          SecOC_SetCryptographicPduTransmit(splitTxPduId, SECOC_IDLE_AUTHENTICSPLITPDUTRANSMIT);        /* SBSW_SECOC_CSL80 */
        }
#  endif
#  if(SECOC_EXISTS_TPPDU_PDUTYPEOFTXPDUINFO == STD_ON)
        if(SecOC_GetPduTypeOfTxPduInfo(txPduId) == SECOC_TPPDU_PDUTYPEOFTXPDUINFO)      /* COV_SECOC_TX_TP_UP_ONLY */
        {
          PduR_SecOCTpTxConfirmation(SecOC_GetAuthenticExternalIdOfTxPduInfo(txPduId), E_NOT_OK);
        }
#  endif
      }
    }
    else
    {
      SecOC_Authenticate_IncrementAndCheckBuildAttempts(txPduId);
    }
# endif
  }
}
#endif

#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_SENDDEFAULTAUTHENTICATION == STD_ON))
/**********************************************************************************************************************
  SecOC_CopyAuthenticationPatternIntoMacAndFvMValue
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_CopyAuthenticationPatternIntoMacAndFvMValue(SecOC_TxPduInfoIterType txPduId)
{
  /* Copy pattern into the MAC and FvM value */
  SecOC_SecuredPduDataContainerAuthInfoDataStartIdxOfTxPduInfoType destIdx = SecOC_GetSecuredPduDataContainerAuthInfoDataStartIdxOfTxPduInfo(txPduId);

  for(; destIdx < SecOC_GetSecuredPduDataContainerAuthInfoDataEndIdxOfTxPduInfo(txPduId); destIdx++)
  {
    SecOC_SetSecuredPduDataContainer(destIdx, SECOC_DEFAULT_AUTHENTICATION_INFORMATION_PATTERN);        /* SBSW_SECOC_CSL03 */
  }

# if(SECOC_AUTHENTICPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFTXPDUINFO ==  STD_ON)
  VStdMemCpyRamToRam(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetStartIdxOfTxSecuredPduDataContainerData(txPduId)), SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetStartIdxOfTxAuthenticPduDataContainerData(txPduId)), SecOC_GetLengthOfTxSecuredPduDataContainerData(txPduId));    /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_CPY_2RAM */
# endif
  SecOC_SetTxPduState(txPduId, SECOC_AUTHENTICATED_TXPDUSTATE); /* SBSW_SECOC_CSL02 */
}
#endif

#if (SECOC_EXISTS_TPPDU_PDUTYPEOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_HandlePdu_TPPDU
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_HandlePdu_TPPDU(SecOC_TxPduInfoIterType txPduId)
{
  if(SecOC_GetTxPduState(txPduId) == SECOC_GET_DATA_TXPDUSTATE)
  {
    PduInfoType pduInfo;
    PduLengthType availableData;
    BufReq_ReturnType retVal;
    pduInfo.SduDataPtr = SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetStartIdxOfTxAuthenticPduDataContainerData(txPduId));
    pduInfo.SduLength = (PduLengthType) (SecOC_GetAuthenticPduDataContainerAuthenticPduLengthOfTxPduInfo(txPduId) - SecOC_GetTxAuthenticPduSizeOffset(txPduId));

    retVal = PduR_SecOCTpCopyTxData(SecOC_GetAuthenticExternalIdOfTxPduInfo(txPduId), &pduInfo, NULL_PTR, &availableData);      /* SBSW_SECOC_PTR2FUNC_2 */
    if(retVal == BUFREQ_OK)
    {
      /* Reset Tx Sync Attempts Counter */
      SecOC_SetTxBuildAttemptsCounter(txPduId, 0);      /* SBSW_SECOC_PTR2FUNC_2 */
      /* --- Set the Tx Pdu State to "Reauthentication needed" --- */
      SecOC_SetTxPduState(txPduId, SECOC_REAUTHENTICATION_NEEDED_TXPDUSTATE);   /* SBSW_SECOC_CSL80 */
      /* Set the Transmit Request state of the corresponding Pdu to Transmit */
      /* --- Set the Transmit request --- */
      SecOC_SetTransmitRequest(txPduId, SECOC_TRANSMIT_TRANSMITREQUEST);        /* SBSW_SECOC_PTR2FUNC_2 */
    }
    else if(retVal == BUFREQ_E_NOT_OK)
    {
      SecOC_SetTxPduState(txPduId, SECOC_INVALID_SECURED_PDU_TXPDUSTATE);       /* SBSW_SECOC_CSL02 */
      SecOC_SetTransmitRequest(txPduId, SECOC_IDLE_TRANSMITREQUEST);    /* SBSW_SECOC_CSL02 */
      PduR_SecOCTpTxConfirmation(SecOC_GetAuthenticExternalIdOfTxPduInfo(txPduId), E_NOT_OK);
    }
    else
    {
      /* Retry with next main function call */
    }
  }
}
#endif

#if (SECOC_TXSPLITPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_HandlePdu_SplitPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_TxIf_HandlePdu_SplitPdu(SecOC_TxPduInfoIterType txPduId)
{
  SecOC_SizeOfTxSplitPduInfoType splitTxPduId = SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduId);
  Std_ReturnType retVal;
  Std_ReturnType retValAuthPdu;
  Std_ReturnType retValCryptoPdu;
  if(SecOC_GetAuthenticSplitPduTransmit(splitTxPduId) == SECOC_TRANSMIT_AUTHENTICSPLITPDUTRANSMIT)      /* COV_SECOC_RETRY_FAILED_TRANSMIT_REQUEST_DISABLED */
  {
    PduInfoType pduInfo;
    pduInfo.SduDataPtr = SecOC_GetAddrSecuredPduDataContainer(SecOC_GetStartIdxOfTxAuthenticSplitPdu(txPduId));
    pduInfo.SduLength = (PduLengthType) SecOC_GetLengthOfTxAuthenticSplitPdu(txPduId);
    SecOC_SetTxPduState(txPduId, SECOC_TRANSMISSION_INITIATED_TXPDUSTATE);      /* SBSW_SECOC_CSL02 */
    retValAuthPdu = PduR_SecOCTransmit(SecOC_GetAuthSplitPduExternalIdOfTxSplitPduInfo(splitTxPduId), &pduInfo);        /* SBSW_SECOC_PTR2FUNC_2 */
# if (SECOC_RETRY_FAILED_TRANSMIT_REQUEST == STD_ON)
    if(retValAuthPdu == E_OK)
# endif
    {
      SecOC_SetAuthenticSplitPduTransmit(splitTxPduId, SECOC_IDLE_AUTHENTICSPLITPDUTRANSMIT);   /* SBSW_SECOC_CSL80 */
    }
  }
  else
  {
    retValAuthPdu = E_OK;
  }

  if(SecOC_GetCryptographicPduTransmit(splitTxPduId) == SECOC_TRANSMIT_CRYPTOGRAPHICPDUTRANSMIT)        /* COV_SECOC_RETRY_FAILED_TRANSMIT_REQUEST_DISABLED */
  {
    PduInfoType pduInfo;
    pduInfo.SduDataPtr = SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerCryptographicPduStartIdxOfTxSplitPduInfo(splitTxPduId));
    pduInfo.SduLength = (PduLengthType) SecOC_GetSecuredPduDataContainerCryptographicPduLengthOfTxSplitPduInfo(splitTxPduId);

# if(SECOC_MESSAGELINKLENOFTXSPLITPDUINFO == STD_ON)
    if(SecOC_IsSecuredPduDataContainerMessageLinkerCryptoPduUsedOfTxSplitPduInfo(splitTxPduId))
    {
      /* *INDENT-OFF* */
      SecOC_GetArrayByteAligned(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetStartIdxOfTxMessageLinkerAuthSplitPdu(txPduId)), /* PRQA S 2985 */ /* MD_SecOC_2985 */
                                       SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerMessageLinkerCryptoPduStartIdxOfTxSplitPduInfo(splitTxPduId)),
                                       (uint16) SecOC_GetMessageLinkLenOfTxSplitPduInfo(splitTxPduId),
                                       (uint8) ((8u - ((SecOC_GetMessageLinkPosOfTxSplitPduInfo(splitTxPduId) + 1u) & 7u)) & 7u),
                                       SecOC_GetMessagesLinkCryptoPduDestBitOffsetOfTxSplitPduInfo(splitTxPduId), TRUE); /* SBSW_SECOC_PTR2FUNC_1 */
      /* *INDENT-ON* */
    }
# endif
    SecOC_SetTxPduState(txPduId, SECOC_TRANSMISSION_INITIATED_TXPDUSTATE);      /* SBSW_SECOC_CSL02 */
    retValCryptoPdu = PduR_SecOCTransmit(SecOC_GetCyptographicPduExternalIdOfTxSplitPduInfo(splitTxPduId), &pduInfo);   /* SBSW_SECOC_PTR2FUNC_2 */
# if (SECOC_RETRY_FAILED_TRANSMIT_REQUEST == STD_ON)
    if(retValCryptoPdu == E_OK)
# endif
    {
      SecOC_SetCryptographicPduTransmit(splitTxPduId, SECOC_IDLE_CRYPTOGRAPHICPDUTRANSMIT);     /* SBSW_SECOC_CSL80 */
    }
  }
  else
  {
    retValCryptoPdu = E_OK;
  }

  if((retValAuthPdu == E_OK) && (retValCryptoPdu == E_OK))
  {
    retVal = E_OK;
  }
  else
  {
    retVal = E_NOT_OK;
  }

  return retVal;
}
#endif

#if (SECOC_ADDCANFDCONTAINERPDUPADDINGOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_CalculatePaddingSize
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(uint8, SECOC_CODE) SecOC_TxIf_CalculatePaddingSize(SecOC_SecuredPduDataContainerAuthenticPduLengthOfTxPduInfoType securedPduLength)
{
  uint8 paddingSize = 0u;
  SecOC_CanFDLengthCompValueInfoIterType cmpValueInfoIdx = 0;

  for(; cmpValueInfoIdx < SecOC_GetSizeOfCanFDLengthCompValueInfo(); cmpValueInfoIdx++)
  {
    SecOC_LowerLengthValueOfCanFDLengthCompValueInfoType lower = SecOC_GetLowerLengthValueOfCanFDLengthCompValueInfo(cmpValueInfoIdx);
    SecOC_UpperLengthValueOfCanFDLengthCompValueInfoType upper = SecOC_GetUpperLengthValueOfCanFDLengthCompValueInfo(cmpValueInfoIdx);
    if((securedPduLength > lower) && (securedPduLength < upper))
    {
      paddingSize = (uint8) (upper - securedPduLength);
      break;
    }
  }
  return paddingSize;
}
#endif

#if (SECOC_ADDCANFDCONTAINERPDUPADDINGOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_AddCANFDPadding
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_AddCANFDPadding(SecOC_TxPduInfoIterType txPduId)
{
  uint8 paddingSize = SecOC_TxIf_CalculatePaddingSize(SecOC_GetLengthOfTxSecuredPdu(txPduId));

  if(paddingSize > 0u)
  {
    SecOC_SecuredPduDataContainerAuthenticPduStartIdxOfTxPduInfoType authenticPduBufferIdx = SecOC_GetStartIdxOfTxSecuredPduDataContainerData(txPduId);

    for(; authenticPduBufferIdx < SecOC_GetSecuredPduDataContainerAuthenticPduEndIdxOfTxPduInfo(txPduId); authenticPduBufferIdx++)
    {
      SecOC_SecuredPduDataContainerAuthenticPduStartIdxOfTxPduInfoType newAuthenticPduBufferIdx = (SecOC_SecuredPduDataContainerAuthenticPduStartIdxOfTxPduInfoType) (authenticPduBufferIdx - paddingSize);
      if(newAuthenticPduBufferIdx < SecOC_GetSecuredPduDataContainerAuthenticPduEndIdxOfTxPduInfo(txPduId))     /* COV_SECOC_SAFEBSW_CHECK */
      {
        SecOC_SetSecuredPduDataContainer(newAuthenticPduBufferIdx, SecOC_GetSecuredPduDataContainer(authenticPduBufferIdx));    /* SBSW_SECOC_CALCULATED_INDEX */
      }
      SecOC_SetSecuredPduDataContainer(authenticPduBufferIdx, 0);       /* SBSW_SECOC_CSL01 */
    }
    SecOC_SetTxAuthenticPduSizeOffset(txPduId, (SecOC_GetTxAuthenticPduSizeOffset(txPduId) - paddingSize));     /* SBSW_SECOC_CSL01 */
  }
}
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_HandlePdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_HandlePdus(void)
{
  SecOC_TxPduInfoIterType txPduId = 0;
  for(; txPduId < SecOC_GetSizeOfTxPduInfo(); txPduId++)
  {
# if (SECOC_TXIMMEDIATEPROCESSINGONGOING == STD_ON)
    if(!SecOC_IsTxImmediateProcessingOngoing(txPduId))  /* COV_SECOC_IMMEDIATE_CHECK */
# endif
    {
# if (SECOC_CSMJOBTIMEOUTCNTOFTXPDUINFO == STD_ON)
      SecOC_DlMon_TxCSMTimeoutSupervision(txPduId);
# endif
      (void) SecOC_TxIf_ProcessTxPdu(txPduId);
    }
  }
}

# if (SECOC_CSMJOBTIMEOUTCNTOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_DlMon_TxCSMTimeoutSupervision
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_DlMon_TxCSMTimeoutSupervision(SecOC_TxPduInfoIterType txPduId)
{
  boolean cancelCsmJob = FALSE;

  if(SecOC_GetTxPduState(txPduId) == SECOC_UNAUTHENTICATED_FINISHED_WAIT_FOR_CALLBACK_TXPDUSTATE)
  {
    SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();
    if(SecOC_GetTxCSMJobTimeoutCntValue(txPduId) > 0u)
    {
      SecOC_DecTxCSMJobTimeoutCntValue(txPduId);        /* SBSW_SECOC_CSL02 */
      if(SecOC_GetTxCSMJobTimeoutCntValue(txPduId) == 0u)
      {
        /* timeout occured */
#  if (SECOC_SENDDEFAULTAUTHENTICATION == STD_ON)
        if(SecOC_IsSendDefaultAuthentication(txPduId))
        {
          SecOC_SetTxPduState(txPduId, SECOC_DEVMODE_AUTHENTICATION_PATTERN_NEEDED_TXPDUSTATE); /* SBSW_SECOC_CSL02 */
        }
        else
#  endif
        {
          SecOC_SetTxPduState(txPduId, SECOC_INVALID_SECURED_PDU_TXPDUSTATE);   /* SBSW_SECOC_CSL02 */
        }
        cancelCsmJob = TRUE;
      }
    }
    SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();
    if(cancelCsmJob)
    {
      Std_ReturnType ret = Csm_CancelJob(SecOC_GetAuthenticationLayerIdOfTxPduInfo(txPduId), CRYPTO_OPERATIONMODE_SINGLECALL);
      if(ret == E_NOT_OK)
      {
        SecOC_ReportRuntimeDetError(SECOC_SID_MAIN_FUNCTION_TX, SECOC_E_CSM_CANCEL_JOB_FAILED);
      }
    }
  }
}
# endif

/**********************************************************************************************************************
  SecOC_TxIf_ProcessTxPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6050, 6010, 6030 1 */ /* MD_MSR_STCAL, MD_MSR_STPTH, MD_MSR_STCYC */
SECOC_LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_TxIf_ProcessTxPdu(SecOC_TxPduInfoIterType txPduId)
{
  Std_ReturnType retVal = E_OK;

# if (SECOC_CANCEL_TRANSMIT_API == STD_ON)
  if(SecOC_IsCancelTxTransmit(txPduId))
  {
    SecOC_SetCancelTxTransmit(txPduId, FALSE);  /* SBSW_SECOC_CSL02 */
    SecOC_SetTxPduState(txPduId, SECOC_INVALID_SECURED_PDU_TXPDUSTATE); /* SBSW_SECOC_CSL02 */
    SecOC_SetTransmitRequest(txPduId, SECOC_IDLE_TRANSMITREQUEST);      /* SBSW_SECOC_CSL02 */
  }
# endif
# if (SECOC_EXISTS_TPPDU_PDUTYPEOFTXPDUINFO == STD_ON)
  SecOC_TxIf_HandlePdu_TPPDU(txPduId);
# endif
  SecOC_TxIf_HandlePdu_ReauthenticationNeeded(txPduId);

  if(SecOC_IsTxPduStateUnauthenticated(txPduId))
  {
    /* Generates MAC and copies the truncated MAC to the Secured Pdu Buffer */
# if (SECOC_WAITFORCALLBACKOFTXPDUINFO == STD_ON)
    if(SecOC_GetTxPduState(txPduId) == SECOC_UNAUTHENTICATED_TO_BE_STARTED_TXPDUSTATE)
# endif
    {
      SecOC_Authenticate_GenerateAuthenticator(txPduId);
    }
    if(SecOC_GetTxPduState(txPduId) == SECOC_UNAUTHENTICATED_FINISHED_TXPDUSTATE)
    {
# if(SECOC_AUTHENTICPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFTXPDUINFO == STD_ON)
      VStdMemCpyRamToRam(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetStartIdxOfTxSecuredPduDataContainerData(txPduId)), SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetStartIdxOfTxAuthenticPduDataContainerData(txPduId)), SecOC_GetLengthOfTxSecuredPduDataContainerData(txPduId));        /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_CPY_2RAM */
# endif
      SecOC_SetTxPduState(txPduId, SECOC_AUTHENTICATED_TXPDUSTATE);     /* SBSW_SECOC_CSL02 */
    }
  }

# if(SECOC_SENDDEFAULTAUTHENTICATION == STD_ON)
  if(SecOC_IsSendDefaultAuthentication(txPduId) && (SecOC_GetTxPduState(txPduId) == SECOC_DEVMODE_AUTHENTICATION_PATTERN_NEEDED_TXPDUSTATE))
  {
    /* Copy authentication pattern into FvM value and authenticator */
    SecOC_CopyAuthenticationPatternIntoMacAndFvMValue(txPduId);
  }
# endif

  if((SecOC_GetTxPduState(txPduId) == SECOC_AUTHENTICATED_TXPDUSTATE) && (SecOC_GetTransmitRequest(txPduId) == SECOC_TRANSMIT_TRANSMITREQUEST)) /* COV_SECOC_TX_STATE_NOT_REACHABLE */
  {

# if (SECOC_TXSPLITPDUINFO == STD_ON)
    if(SecOC_IsTxSplitPduInfoUsedOfTxPduInfo(txPduId))  /* COV_SECOC_SPLIT_ONLY_CONFIG */
    {
      retVal = SecOC_TxIf_HandlePdu_SplitPdu(txPduId);
    }
    else
# endif
    {
# if (SECOC_ADDCANFDCONTAINERPDUPADDINGOFTXPDUINFO == STD_ON)
      if(SecOC_IsAddCANFDContainerPduPaddingOfTxPduInfo(txPduId))
      {
        SecOC_TxIf_AddCANFDPadding(txPduId);
      }
# endif
# if(SECOC_SECUREDPDUDATACONTAINERSECUREDPDUSTARTIDXOFTXPDUINFO == STD_ON)
      {
        PduInfoType pduInfo;
        pduInfo.SduDataPtr = SecOC_GetAddrSecuredPduDataContainer(SecOC_GetStartIdxOfTxSecuredPdu(txPduId));
        pduInfo.SduLength = (PduLengthType) SecOC_GetLengthOfTxSecuredPdu(txPduId);
        SecOC_SetTxPduState(txPduId, SECOC_TRANSMISSION_INITIATED_TXPDUSTATE);  /* SBSW_SECOC_CSL02 */
        retVal = PduR_SecOCTransmit(SecOC_GetSecuredExternalIdOfTxPduInfo(txPduId), &pduInfo);  /* SBSW_SECOC_PTR2FUNC_2 */
      }
# else
      retVal = E_OK;
# endif
    }
    if(retVal == E_OK)
    {
# if (SECOC_SPDUTXCONFIRMATIONNEEDEDOFTXPDUINFO == STD_ON)
      if(SecOC_IsSPduTxConfirmationNeededOfTxPduInfo(txPduId))
      {
        SPduTxConfirmation_FuncName(SecOC_GetExternalFreshnessIdOfTxPduInfo(txPduId));
      }
# endif
      SecOC_SetTransmitRequest(txPduId, SECOC_IDLE_TRANSMITREQUEST);    /* SBSW_SECOC_CSL02 */
    }
    else
    {
# if (SECOC_RETRY_FAILED_TRANSMIT_REQUEST == STD_OFF)
      SecOC_SetTransmitRequest(txPduId, SECOC_IDLE_TRANSMITREQUEST);    /* SBSW_SECOC_CSL02 */
# else
      retVal = E_OK;
# endif
      SecOC_SetTxPduState(txPduId, SECOC_AUTHENTICATED_TXPDUSTATE);     /* SBSW_SECOC_CSL02 */
# if ((SECOC_EXISTS_TPPDU_PDUTYPEOFTXPDUINFO == STD_ON) && (SECOC_RETRY_FAILED_TRANSMIT_REQUEST == STD_OFF))
      if(SecOC_GetPduTypeOfTxPduInfo(txPduId) == SECOC_TPPDU_PDUTYPEOFTXPDUINFO)        /* COV_SECOC_TX_TP_UP_ONLY */
      {
        PduR_SecOCTpTxConfirmation(SecOC_GetAuthenticExternalIdOfTxPduInfo(txPduId), E_NOT_OK);
      }
# endif
    }
  }
  return retVal;
}
#endif

#if ((SECOC_RXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_Verify_PropagateVerificationStatus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_PropagateVerificationStatus(SecOC_RxPduInfoIterType rxPduId, SecOC_VerificationResultType verificationStatus)
{
  SecOC_VerificationStatusType verificationStatusStruct;
  verificationStatusStruct.verificationStatus = verificationStatus;
  verificationStatusStruct.freshnessValueID = SecOC_GetExternalFreshnessIdOfRxPduInfo(rxPduId);
  verificationStatusStruct.secOCDataId = SecOC_GetDataIdOfRxPduInfo(rxPduId);

# if (SECOC_VERIFICATIONSTATUSCALLOUT == STD_ON)
  {
    SecOC_VerificationStatusCalloutIterType calloutIdx = 0;

    for(; calloutIdx < SecOC_GetSizeOfVerificationStatusCallout(); calloutIdx++)
    {
      SecOC_GetVerificationStatusCallout(calloutIdx) (verificationStatusStruct);        /* SBSW_SECOC_VERIFY_STATUS_CALLOUT */
    }
  }
# endif
  (void) Rte_Write_VerificationStatus_verificationStatus(&verificationStatusStruct);    /* SBSW_SECOC_PTR2FUNC_2 */
}
#endif

#if ((SECOC_RXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_Verify_PropagateVerificationStatus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_PropagateVerificationStatusWithSecuredPdu(SecOC_RxPduInfoIterType rxPduId, SecOC_VerificationResultType verificationStatus)
{
  SecOC_VerificationStatusType verificationStatusStruct;
  verificationStatusStruct.verificationStatus = verificationStatus;
  verificationStatusStruct.freshnessValueID = SecOC_GetExternalFreshnessIdOfRxPduInfo(rxPduId);
  verificationStatusStruct.secOCDataId = SecOC_GetDataIdOfRxPduInfo(rxPduId);

# if (SECOC_VERIFICATIONSTATUSCALLOUTWITHSECUREDPDU == STD_ON)
  {
    SecOC_VerificationStatusCalloutWithSecuredPduIterType securedCalloutIdx = 0;

    uint32 freshnessValueLength = ((uint32) (SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueLengthOfRxPduInfo(rxPduId)) << 3u);
    uint8 *freshnessVerifyValue = SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueStartIdxOfRxPduInfo(rxPduId));

    PduInfoType receivedSecuredPDU;
#  if(SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO ==  STD_ON)
    receivedSecuredPDU.SduDataPtr = SecOC_GetAddrSecuredPduDataContainer(SecOC_GetStartIdxOfRxSecuredPduDataContainer(rxPduId));
    receivedSecuredPDU.SduLength = (PduLengthType) SecOC_GetLengthOfRxSecuredPduDataContainerSecuredPdu(rxPduId);
#  else
    receivedSecuredPDU.SduDataPtr = NULL_PTR;
    receivedSecuredPDU.SduLength = 0u;
#  endif

    for(; securedCalloutIdx < SecOC_GetSizeOfVerificationStatusCalloutWithSecuredPdu(); securedCalloutIdx++)
    {
      SecOC_GetVerificationStatusCalloutWithSecuredPdu(securedCalloutIdx) (verificationStatusStruct, &receivedSecuredPDU, freshnessVerifyValue, freshnessValueLength);  /* SBSW_SECOC_VERIFY_STATUS_CALLOUT */
    }
  }
# endif
  SECOC_DUMMY_STATEMENT(verificationStatusStruct);      /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}
#endif

#if ((SECOC_RXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_Verify_PropagateClientServerVerificationStatus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_PropagateClientServerVerificationStatus(SecOC_RxPduInfoIterType rxPduId, SecOC_VerificationResultType verificationStatus)
{
  SecOC_VerificationStatusType verificationStatusStruct;
  verificationStatusStruct.verificationStatus = verificationStatus;
  verificationStatusStruct.freshnessValueID = SecOC_GetExternalFreshnessIdOfRxPduInfo(rxPduId);
  verificationStatusStruct.secOCDataId = SecOC_GetDataIdOfRxPduInfo(rxPduId);

  (void) Rte_Call_VerificationStatusNotification_VerifyStatus(&verificationStatusStruct);    /* SBSW_SECOC_PTR2FUNC_2 */
}
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_Verify_MACUsingCSM43
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6010, 6030 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
SECOC_LOCAL_INLINE FUNC(SecOC_VerifyResultType, SECOC_CODE) SecOC_Verify_MACUsingCSM43(SecOC_RxPduInfoIterType rxPduId)
{
  Std_ReturnType retValue = E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Crypto_VerifyResultType csmVerifyResult = CSM_E_VER_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */
  SecOC_VerifyResultType secOCVerifyResult = SECOC_E_VER_NOT_OK;        /* PRQA S 2981 */ /* MD_MSR_RetVal */
# if (SECOC_RXSECUREDPDUAREAINFOUSEDOFRXPDUINFO == STD_ON)
  Std_ReturnType securedAreaCheck = E_NOT_OK;
# endif

# if(SECOC_WAITFORCALLBACKOFRXPDUINFO == STD_ON)
  if(SecOC_GetRxPduState(rxPduId) == SECOC_UNVERIFIED_VERIFICATION_FINISHED_VERIFICATION_RESULT_READY_RXPDUSTATE)
  {
    retValue = E_OK;
    csmVerifyResult = SecOC_GetVerifyResult(SecOC_GetVerifyResultIdxOfRxPduInfo(rxPduId));
    SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_VERIFICATION_FINISHED_RXPDUSTATE);    /* SBSW_SECOC_CSL02 */
  }
  else
# endif
  {
    uint8 *dataPtr;             /* PRQA S 3678 */ /* MD_MSR_Rule8.13 */
    uint32 dataLength;
    SecOC_RxPduStateType storedState;

    SecOC_GetArrayByteAligned(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerTruncatedAuthenticatorStartIdxOfRxPduInfo(rxPduId)), SecOC_GetAddrAuthenticatorCache(SecOC_GetAuthenticatorCacheStartIdxOfRxPduInfo(rxPduId)), (uint16) SecOC_GettruncatedAuthenticationLengthOfRxPduInfo(rxPduId), (uint8) SecOC_GetTruncAuthStartBitInByteOfRxPduInfo(rxPduId), 0u, TRUE); /* SBSW_SECOC_PTR2FUNC_6 */

    SecOC_AddRxDataIdsToDataToAuthenticate(rxPduId);
    dataPtr = SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetStartIdxOfRxAuthenticPduDataContainerDataToAuthenticate(rxPduId));
    dataLength = SecOC_GetLengthOfRxRxAuthenticPduDataContainerDataToAuthenticate(rxPduId);
    storedState = SecOC_GetRxPduState(rxPduId);
# if (SECOC_RXSECUREDPDUAREAINFOUSEDOFRXPDUINFO == STD_ON)
    if(SecOC_IsRxSecuredPduAreaInfoUsedOfRxPduInfo(rxPduId))
    {
      SecOC_RxSecuredPduAreaInfoIdxOfRxPduInfoType securedAreaInfoIdx = SecOC_GetRxSecuredPduAreaInfoIdxOfRxPduInfo(rxPduId);
      uint32 dataLengthWithSecuredArea = 2u + (uint32) SecOC_GetLengthOfRxSecuredPduAreaInfo(securedAreaInfoIdx) + (uint32) SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueLengthOfRxPduInfo(rxPduId);

      /* Silent check, shall never fail for valid configuration */
      if(dataLengthWithSecuredArea <= SecOC_GetSizeOfRxSecuredPduAreaBuffer())  /* COV_SECOC_SILENT_CHECK */
      {

        /* Check if offset + length will fit in provided authentic pdu data */ /* \trace SWS_SecOC_00314 */
        if(dataLength >= (SecOC_GetOffsetOfRxSecuredPduAreaInfo(securedAreaInfoIdx) + dataLengthWithSecuredArea))
        {
          uint8 *securedAreaBuffer = SecOC_GetAddrRxSecuredPduAreaBuffer(0u);

          /* Copy Data Id */
          VStdMemCpyRamToRam(securedAreaBuffer, dataPtr, 2u);   /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_FCTCALL_SECAREA */

          /* Copy Secured Area */
          VStdMemCpyRamToRam(&securedAreaBuffer[2u], &dataPtr[2u + SecOC_GetOffsetOfRxSecuredPduAreaInfo(securedAreaInfoIdx)], SecOC_GetLengthOfRxSecuredPduAreaInfo(securedAreaInfoIdx));      /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_FCTCALL_SECAREA */

          /* Copy Freshness value */
          VStdMemCpyRamToRam(&securedAreaBuffer[2u + SecOC_GetLengthOfRxSecuredPduAreaInfo(securedAreaInfoIdx)], SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueStartIdxOfRxPduInfo(rxPduId)), SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueLengthOfRxPduInfo(rxPduId)); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_FCTCALL_SECAREA */

          dataPtr = securedAreaBuffer;
          dataLength = dataLengthWithSecuredArea;
          securedAreaCheck = E_OK;
        }
      }
      else
      {
        SecOC_ReportRuntimeDetError(SECOC_SID_MAIN_FUNCTION_RX, SECOC_E_SECURED_AREA);
      }
    }
    else
    {
      securedAreaCheck = E_OK;
    }

    if(securedAreaCheck == E_OK)
# endif
    {
# if(SECOC_WAITFORCALLBACKOFRXPDUINFO == STD_ON)
      if(SecOC_IsWaitForCallbackOfRxPduInfo(rxPduId))
      {
        SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_VERIFICATION_FINISHED_WAIT_FOR_CALLBACK_RXPDUSTATE);      /* SBSW_SECOC_CSL02 */
      }
# endif
    /* *INDENT-OFF* */
      retValue = Csm_MacVerify(
          SecOC_GetAuthenticationLayerIdOfRxPduInfo(rxPduId),
          CRYPTO_OPERATIONMODE_SINGLECALL,
          dataPtr,
          dataLength,
          SecOC_GetAddrAuthenticatorCache(SecOC_GetAuthenticatorCacheStartIdxOfRxPduInfo(rxPduId)),
          SecOC_GettruncatedAuthenticationLengthOfRxPduInfo(rxPduId),
          SecOC_GetAddrVerifyResult(SecOC_GetVerifyResultIdxOfRxPduInfo(rxPduId))); /* SBSW_SECOC_PTR2FUNC_1 */
      /* *INDENT-ON* */
    }
    if(retValue == E_OK)
    {
# if(SECOC_WAITFORCALLBACKOFRXPDUINFO == STD_ON)
      if(!SecOC_IsWaitForCallbackOfRxPduInfo(rxPduId))
# endif
      {
        csmVerifyResult = SecOC_GetVerifyResult(SecOC_GetVerifyResultIdxOfRxPduInfo(rxPduId));
        SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_VERIFICATION_FINISHED_RXPDUSTATE);        /* SBSW_SECOC_CSL02 */
      }
# if(SECOC_CSMJOBTIMEOUTCNTOFRXPDUINFO == STD_ON)
      else
      {
        SecOC_SetRxCSMJobTimeoutCntValue(rxPduId, SecOC_GetCsmJobTimeoutCntOfRxPduInfo(rxPduId));       /* SBSW_SECOC_CSL02 */
      }
# endif
    }
    else
    {
      SecOC_SetRxPduState(rxPduId, storedState);        /* SBSW_SECOC_CSL02 */
    }

  }


  if(retValue == E_NOT_OK)
  {
    /* CSM returned none recoverable return value */
    SecOC_SetRxPduState(rxPduId, SECOC_ABORTED_RXPDUSTATE);     /* SBSW_SECOC_CSL02 */
  }
  else if(retValue != E_OK)
  {
    /* CSM returned recoverable return value */
    SecOC_SetRxPduState(rxPduId, SECOC_FRESHNESS_VALUE_NEEDED_RXPDUSTATE);      /* SBSW_SECOC_CSL02 */
    SecOC_Verify_IncrementBuildAttempts(rxPduId);
  }
  else
  {
    /* CSM API call was successful */
  }

  if(csmVerifyResult == CRYPTO_E_VER_OK)
  {
    secOCVerifyResult = SECOC_E_VER_OK;
  }

  return secOCVerifyResult;
}
#endif

#if ((SECOC_RXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_ON))
/**********************************************************************************************************************
  SecOC_Verify_MACUsingCustomCallout
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
/* PRQA S 6010, 6030 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
SECOC_LOCAL_INLINE FUNC(SecOC_VerifyResultType, SECOC_CODE) SecOC_Verify_MACUsingCustomCallout(SecOC_RxPduInfoIterType rxPduId)
{
  Crypto_VerifyResultType verifyResult = CRYPTO_E_VER_NOT_OK;
  SecOC_VerifyResultType verificationResult = SECOC_E_VER_NOT_OK;

# if(SECOC_WAITFORCALLBACKOFRXPDUINFO == STD_ON)
  if(SecOC_GetRxPduState(rxPduId) == SECOC_UNVERIFIED_VERIFICATION_FINISHED_VERIFICATION_RESULT_READY_RXPDUSTATE)
  {
    verifyResult = SecOC_GetVerifyResult(SecOC_GetVerifyResultIdxOfRxPduInfo(rxPduId));
    SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_VERIFICATION_FINISHED_RXPDUSTATE);    /* SBSW_SECOC_CSL02 */
  }
  else
# endif
  {
    Std_ReturnType retVal;
    uint32 rxAuthenticatorLength = (uint32) SecOC_GetAuthInfoLengthOfRxPduInfo(rxPduId);
    PduInfoType rxAuthenticPduInfo;

    rxAuthenticPduInfo.SduDataPtr = SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetStartIdxOfRxAuthenticPduDataContainer(rxPduId));
    rxAuthenticPduInfo.SduLength = (PduLengthType) SecOC_GetLengthOfRxAuthenticPduDataContainer(rxPduId);

    retVal = SecOC_VerifyAuthenticationInfo(SecOC_GetExternalFreshnessIdOfRxPduInfo(rxPduId), &rxAuthenticPduInfo, SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerTruncatedFreshnessValueStartIdxOfRxPduInfo(rxPduId)), rxAuthenticatorLength, SecOC_GetAddrVerifyResult(SecOC_GetVerifyResultIdxOfRxPduInfo(rxPduId)));  /* SBSW_SECOC_PTR2FUNC_5 */

    if(retVal == E_OK)
    {
# if(SECOC_WAITFORCALLBACKOFRXPDUINFO == STD_ON)
      if(SecOC_IsWaitForCallbackOfRxPduInfo(rxPduId))
      {
        SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_VERIFICATION_FINISHED_WAIT_FOR_CALLBACK_RXPDUSTATE);      /* SBSW_SECOC_CSL02 */
      }
      else
# endif
      {
        verifyResult = SecOC_GetVerifyResult(SecOC_GetVerifyResultIdxOfRxPduInfo(rxPduId));
        SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_VERIFICATION_FINISHED_RXPDUSTATE);        /* SBSW_SECOC_CSL02 */
      }
    }
    else if(retVal == E_NOT_OK)
    {
      SecOC_SetRxPduState(rxPduId, SECOC_ABORTED_RXPDUSTATE);   /* SBSW_SECOC_CSL02 */
    }
    else
    {
      SecOC_Verify_IncrementBuildAttempts(rxPduId);
    }
  }

  if(verifyResult == CRYPTO_E_VER_OK)
  {
    verificationResult = SECOC_E_VER_OK;
  }

  return verificationResult;
}
#endif


#if (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_GetVerificationResultOfOverrideStatus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_VerifyResultType, SECOC_CODE) SecOC_Verify_GetVerificationResultOfOverrideStatus(SecOC_RxPduInfoIterType rxPduId)
{
  SecOC_VerifyResultType verificationResult = SECOC_E_VER_NOT_OK;
  SecOC_VerifyOverrideStatusType verifyOverrideStatus = SecOC_GetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduId));

  SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
  switch (verifyOverrideStatus)
  {
# if (SECOC_ENABLE_FORCED_PASS_OVERRIDESTATUS == STD_ON)
    case SECOC_PASS_VERIFYOVERRIDESTATUS:
    case SECOC_OVERRIDE_SKIP_UNTIL_NOTICE_VERIFYOVERRIDESTATUS:
      verificationResult = SECOC_E_VER_OK;
      break;
    case SECOC_PASS_N_TIMES_VERIFYOVERRIDESTATUS:
    case SECOC_OVERRIDE_SKIP_UNTIL_LIMIT_VERIFYOVERRIDESTATUS:
      verificationResult = SECOC_E_VER_OK;
      SecOC_DecNumberOfMessagesToOverride(SecOC_GetNumberOfMessagesToOverrideIdxOfRxPduInfo(rxPduId));  /* SBSW_SECOC_CSL03 */
      if(SecOC_GetNumberOfMessagesToOverride(SecOC_GetNumberOfMessagesToOverrideIdxOfRxPduInfo(rxPduId)) == 0u)
      {
        SecOC_SetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduId), SECOC_CANCEL_VERIFYOVERRIDESTATUS); /* SBSW_SECOC_CSL03 */
      }
      break;
# endif
    case SECOC_FAIL_N_TIMES_VERIFYOVERRIDESTATUS:
      SecOC_DecNumberOfMessagesToOverride(SecOC_GetNumberOfMessagesToOverrideIdxOfRxPduInfo(rxPduId));  /* SBSW_SECOC_CSL03 */
      if(SecOC_GetNumberOfMessagesToOverride(SecOC_GetNumberOfMessagesToOverrideIdxOfRxPduInfo(rxPduId)) == 0u)
      {
        SecOC_SetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduId), SECOC_CANCEL_VERIFYOVERRIDESTATUS); /* SBSW_SECOC_CSL03 */
      }
      break;
    default:   /* COV_SECOC_MISRA */
      break;
  }
  SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_VERIFICATION_FINISHED_RXPDUSTATE);      /* SBSW_SECOC_CSL02 */
  SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
  return verificationResult;
}
#endif

#if (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_ProcessVerificationResult
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_ProcessVerificationResult(SecOC_VerifyResultType verificationResult, SecOC_RxPduInfoIterType rxPduId, SecOC_VerifyOverrideStatusType verifyOverrideStatus)
{
  switch (verifyOverrideStatus)
  {
# if (SECOC_ENABLE_FORCED_PASS_OVERRIDESTATUS == STD_ON)
    case SECOC_PASS_VERIFYOVERRIDESTATUS:
    case SECOC_PASS_N_TIMES_VERIFYOVERRIDESTATUS:
# endif
    case SECOC_CANCEL_VERIFYOVERRIDESTATUS:
      if(verificationResult == SECOC_E_VER_OK)  /* COV_SECOC_IGNORE_VERIFICATION_RESULT_CONFIG */
      {
        SecOC_Verify_ProcessSuccessfulVerificationResult(rxPduId, SECOC_VERIFICATIONSUCCESS);
      }
# if (SECOC_IGNORE_VERIFICATION_RESULT == STD_OFF)
      else
      {
        SecOC_Verify_ProcessUnsuccessfulVerificationResult(rxPduId);
      }
# endif
      break;
# if (SECOC_ENABLE_FORCED_PASS_OVERRIDESTATUS == STD_ON)
    case SECOC_OVERRIDE_SKIP_UNTIL_NOTICE_VERIFYOVERRIDESTATUS:
    case SECOC_OVERRIDE_SKIP_UNTIL_LIMIT_VERIFYOVERRIDESTATUS:
      SecOC_Verify_ProcessSuccessfulVerificationResult(rxPduId, SECOC_NO_VERIFICATION);
      break;
# endif
    case SECOC_FAIL_N_TIMES_VERIFYOVERRIDESTATUS:
    case SECOC_FAIL_INFINETE_VERIFYOVERRIDESTATUS:
      SecOC_Verify_ProcessNoVerificationResult(rxPduId);
      break;
# if (SECOC_ENABLE_FORCED_PASS_OVERRIDESTATUS == STD_ON)
    case SECOC_PASS_UNTIL_NOTICE_VERIFYOVERRIDESTATUS:
    case SECOC_PASS_UNTIL_LIMIT_VERIFYOVERRIDESTATUS:
      if(verificationResult == SECOC_E_VER_OK)  /* COV_SECOC_IGNORE_VERIFICATION_RESULT_CONFIG */
      {
        SecOC_Verify_ProcessSuccessfulVerificationResult(rxPduId, SECOC_VERIFICATIONSUCCESS);
      }
      else
      {
        SecOC_Verify_ProcessSuccessfulVerificationResult(rxPduId, SECOC_VERIFICATIONFAILURE_OVERWRITTEN);
      }
      break;
# endif
    default:   /* COV_SECOC_MISRA */
      break;
  }
}
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_ProcessSuccessfulVerificationResult
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_ProcessSuccessfulVerificationResult(SecOC_RxPduInfoIterType rxPduId, SecOC_VerificationResultType verificationResultType)
{
  /* --- Hand the Authentic Pdu to the Upper Layer --- */
  PduInfoType pduInfo;
# if (SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO == STD_ON)
  PduLengthType sduLength = (PduLengthType) SecOC_GetLengthOfRxAuthenticPduDataContainer(rxPduId);

#  if (SECOC_AUTHENTICPDUDATACONTAINERAUTHENTICPDUMETADATAUSEDOFRXPDUINFO ==  STD_ON)
  if(SecOC_IsSecuredPduDataContainerSecuredPDUMetaDataUsedOfRxPduInfo(rxPduId))
  {
    VStdMemCpyRamToRam(SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerAuthenticPDUMetaDataStartIdxOfRxPduInfo(rxPduId)), SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerSecuredPDUMetaDataStartIdxOfRxPduInfo(rxPduId)), SecOC_GetAuthenticPduDataContainerAuthenticPDUMetaDataLengthOfRxPduInfo(rxPduId));     /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_CPY_2RAM */
    sduLength = (PduLengthType) (sduLength + SecOC_GetAuthenticPduDataContainerAuthenticPDUMetaDataLengthOfRxPduInfo(rxPduId));
  }
#  endif

  pduInfo.SduDataPtr = SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetStartIdxOfRxAuthenticPduDataContainer(rxPduId));
  pduInfo.SduLength = sduLength;
# else
  pduInfo.SduDataPtr = NULL_PTR;
  pduInfo.SduLength = 0u;
# endif
# if ((SECOC_EXISTS_BOTH_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
  if(SecOC_GetVerificationStatusPropagationModeOfRxPduInfo(rxPduId) == SECOC_BOTH_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO) /* COV_SECOC_VERIFICATION_STATUS_PROPAGATION_BOTH_ONLY_CONFIG */
  {
    SecOC_Verify_PropagateVerificationStatus(rxPduId, verificationResultType);
  }
# endif

# if ((SECOC_EXISTS_BOTH_VERIFICATIONSTATUSWITHSECUREDPDUPROPAGATIONMODEOFRXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
  if(SecOC_GetVerificationStatusWithSecuredPduPropagationModeOfRxPduInfo(rxPduId) == SECOC_BOTH_VERIFICATIONSTATUSWITHSECUREDPDUPROPAGATIONMODEOFRXPDUINFO)     /* COV_SECOC_VERIFICATION_STATUS_PROPAGATION_BOTH_ONLY_CONFIG */
  {
    SecOC_Verify_PropagateVerificationStatusWithSecuredPdu(rxPduId, verificationResultType);
  }
# endif

# if ((SECOC_EXISTS_BOTH_CLIENTSERVERVERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
  if(SecOC_GetClientServerVerificationStatusPropagationModeOfRxPduInfo(rxPduId) == SECOC_BOTH_CLIENTSERVERVERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO)     /* COV_SECOC_VERIFICATION_STATUS_PROPAGATION_BOTH_ONLY_CONFIG */
  {
    SecOC_Verify_PropagateClientServerVerificationStatus(rxPduId, verificationResultType);
  }
# endif

  SecOC_RxIf_ForwardPduToUpperLayer(rxPduId, &pduInfo); /* SBSW_SECOC_PTR2FUNC_2 */

# if (SECOC_EXISTS_AUTHENTIC_SPLIT_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
  if(SecOC_IsRxSplitMessageCounterUsedOfRxPduInfo(rxPduId))     /* COV_SECOC_SPLIT_ONLY_CONFIG */
  {
    SecOC_SetRxSplitMessageCounter(SecOC_GetRxSplitMessageCounterIdxOfRxPduInfo(rxPduId), 0);   /* SBSW_SECOC_CSL03 */
  }
# endif
  SecOC_SetRxPduState(rxPduId, SECOC_VERIFIED_RXPDUSTATE);      /* SBSW_SECOC_CSL02 */
  SECOC_DUMMY_STATEMENT(verificationResultType);        /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}
#endif

#if ((SECOC_IGNORE_VERIFICATION_RESULT == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_Verify_ProcessUnsuccessfulVerificationResult
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6050, 6010 1 */ /* MD_MSR_STCAL, MD_MSR_STPTH */
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_ProcessUnsuccessfulVerificationResult(SecOC_RxPduInfoIterType rxPduId)
{
# if(SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF)
  SecOC_IncVerifyAttemptsCounter(rxPduId);      /* SBSW_SECOC_CSL80 */
#  if ((SECOC_EXISTS_BOTH_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON) || (SECOC_EXISTS_FAILURE_ONLY_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON))
#   if(SECOC_EXISTS_NONE_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON)
  if(SecOC_GetVerificationStatusPropagationModeOfRxPduInfo(rxPduId) != SECOC_NONE_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO)
#   endif
  {
#   if(SECOC_PROPAGATE_ONLY_FINAL_VERIFICATION_STATUS == STD_ON)
    if(SecOC_GetVerifyAttemptsCounter(rxPduId) >= SecOC_GetVerifyAttemptsOfRxPduInfo(rxPduId))
#   endif
    {
      SecOC_Verify_PropagateVerificationStatus(rxPduId, SECOC_VERIFICATIONFAILURE);
    }
  }
#  endif

#  if ((SECOC_EXISTS_BOTH_VERIFICATIONSTATUSWITHSECUREDPDUPROPAGATIONMODEOFRXPDUINFO == STD_ON) || (SECOC_EXISTS_FAILURE_ONLY_VERIFICATIONSTATUSWITHSECUREDPDUPROPAGATIONMODEOFRXPDUINFO == STD_ON))
#   if(SECOC_EXISTS_NONE_VERIFICATIONSTATUSWITHSECUREDPDUPROPAGATIONMODEOFRXPDUINFO == STD_ON)
  if(SecOC_GetVerificationStatusWithSecuredPduPropagationModeOfRxPduInfo(rxPduId) != SECOC_NONE_VERIFICATIONSTATUSWITHSECUREDPDUPROPAGATIONMODEOFRXPDUINFO)
#   endif
  {
#   if(SECOC_PROPAGATE_ONLY_FINAL_VERIFICATION_STATUS == STD_ON)
    if(SecOC_GetVerifyAttemptsCounter(rxPduId) >= SecOC_GetVerifyAttemptsOfRxPduInfo(rxPduId))
#   endif
    {
      SecOC_Verify_PropagateVerificationStatusWithSecuredPdu(rxPduId, SECOC_VERIFICATIONFAILURE);
    }
  }
#  endif

#  if ((SECOC_EXISTS_BOTH_CLIENTSERVERVERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON) || (SECOC_EXISTS_FAILURE_ONLY_CLIENTSERVERVERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON))
#   if(SECOC_EXISTS_NONE_CLIENTSERVERVERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON)
  if(SecOC_GetClientServerVerificationStatusPropagationModeOfRxPduInfo(rxPduId) != SECOC_NONE_CLIENTSERVERVERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO)
#   endif
  {
#   if(SECOC_PROPAGATE_ONLY_FINAL_VERIFICATION_STATUS == STD_ON)
    if(SecOC_GetVerifyAttemptsCounter(rxPduId) >= SecOC_GetVerifyAttemptsOfRxPduInfo(rxPduId))
#   endif
    {
      SecOC_Verify_PropagateClientServerVerificationStatus(rxPduId, SECOC_VERIFICATIONFAILURE);
    }
  }
#  endif

  if(SecOC_GetVerifyAttemptsCounter(rxPduId) < SecOC_GetVerifyAttemptsOfRxPduInfo(rxPduId))     /* COV_SECOC_IGNORE_VERIFICATION_RESULT_CONFIG */
  {
    SecOC_FreshnessValue_CalculateVerifyValueFromCallout(rxPduId);
    SecOC_SetRxBuildAttemptsCounter(rxPduId, 0);        /* SBSW_SECOC_CSL02 */
    SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_VERIFICATION_TO_BE_STARTED_RXPDUSTATE);       /* SBSW_SECOC_CSL02 */
  }
  else
# endif
  {
# if(SECOC_EXISTS_TPPDU_PDUTYPEOFRXPDUINFO == STD_ON)
#  if(SECOC_DEVMODEENABLED == STD_ON)
    if(!SecOC_IsDevModeEnabled())
#  endif
    {
      if(SecOC_GetPduTypeOfRxPduInfo(rxPduId) == SECOC_TPPDU_PDUTYPEOFRXPDUINFO)        /* COV_SECOC_RX_TP_UP_ONLY */
      {
        PduR_SecOCTpRxIndication(SecOC_GetAuthenticExternalIdOfRxPduInfo(rxPduId), E_NOT_OK);
      }
    }
# endif
    SecOC_SetRxPduState(rxPduId, SECOC_ABORTED_RXPDUSTATE);     /* SBSW_SECOC_CSL02 */
  }
}
#endif

#if (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_ProcessNoVerificationResult
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_ProcessNoVerificationResult(SecOC_RxPduInfoIterType rxPduId)
{
# if(SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF)
#  if ((SECOC_EXISTS_BOTH_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON) || (SECOC_EXISTS_FAILURE_ONLY_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON))
#   if(SECOC_EXISTS_NONE_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON)
  if(SecOC_GetVerificationStatusPropagationModeOfRxPduInfo(rxPduId) != SECOC_NONE_VERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO)
#   endif
  {
    SecOC_Verify_PropagateVerificationStatus(rxPduId, SECOC_NO_VERIFICATION);
  }
#  endif

#  if ((SECOC_EXISTS_BOTH_VERIFICATIONSTATUSWITHSECUREDPDUPROPAGATIONMODEOFRXPDUINFO == STD_ON) || (SECOC_EXISTS_FAILURE_ONLY_VERIFICATIONSTATUSWITHSECUREDPDUPROPAGATIONMODEOFRXPDUINFO == STD_ON))
#   if(SECOC_EXISTS_NONE_VERIFICATIONSTATUSWITHSECUREDPDUPROPAGATIONMODEOFRXPDUINFO == STD_ON)
  if(SecOC_GetVerificationStatusWithSecuredPduPropagationModeOfRxPduInfo(rxPduId) != SECOC_NONE_VERIFICATIONSTATUSWITHSECUREDPDUPROPAGATIONMODEOFRXPDUINFO)
#   endif
  {
    SecOC_Verify_PropagateVerificationStatusWithSecuredPdu(rxPduId, SECOC_NO_VERIFICATION);
  }
#  endif

#  if ((SECOC_EXISTS_BOTH_CLIENTSERVERVERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON) || (SECOC_EXISTS_FAILURE_ONLY_CLIENTSERVERVERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON))
#   if(SECOC_EXISTS_NONE_CLIENTSERVERVERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO == STD_ON)
  if(SecOC_GetClientServerVerificationStatusPropagationModeOfRxPduInfo(rxPduId) != SECOC_NONE_CLIENTSERVERVERIFICATIONSTATUSPROPAGATIONMODEOFRXPDUINFO)
#   endif
  {
    SecOC_Verify_PropagateClientServerVerificationStatus(rxPduId, SECOC_NO_VERIFICATION);
  }
#  endif
# endif

  {
# if(SECOC_EXISTS_TPPDU_PDUTYPEOFRXPDUINFO == STD_ON)
#  if(SECOC_DEVMODEENABLED == STD_ON)
    if(!SecOC_IsDevModeEnabled())
#  endif
    {
      if(SecOC_GetPduTypeOfRxPduInfo(rxPduId) == SECOC_TPPDU_PDUTYPEOFRXPDUINFO)        /* COV_SECOC_RX_TP_UP_ONLY */
      {
        PduR_SecOCTpRxIndication(SecOC_GetAuthenticExternalIdOfRxPduInfo(rxPduId), E_NOT_OK);
      }
    }
# endif
    SecOC_SetRxPduState(rxPduId, SECOC_IDLE_RXPDUSTATE);        /* SBSW_SECOC_CSL02 */
  }
}
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_RxIf_ForwardPduToUpperLayer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_RxIf_ForwardPduToUpperLayer(SecOC_RxPduInfoIterType rxPduId, P2VAR(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) pduInfoPtr)       /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
  switch (SecOC_GetPduTypeOfRxPduInfo(rxPduId))
  {
# if(SECOC_EXISTS_IFPDU_PDUTYPEOFRXPDUINFO == STD_ON)
    case SECOC_IFPDU_PDUTYPEOFRXPDUINFO:
      PduR_SecOCRxIndication(SecOC_GetAuthenticExternalIdOfRxPduInfo(rxPduId), pduInfoPtr);     /* SBSW_SECOC_PTR2FUNC_2 */
      break;
# endif
# if(SECOC_EXISTS_TPPDU_PDUTYPEOFRXPDUINFO == STD_ON)
    case SECOC_TPPDU_PDUTYPEOFRXPDUINFO:
    {
      BufReq_ReturnType ret;
      PduLengthType bufferSize = 0;
      ret = PduR_SecOCTpCopyRxData(SecOC_GetAuthenticExternalIdOfRxPduInfo(rxPduId), pduInfoPtr, &bufferSize);  /* SBSW_SECOC_PTR2FUNC_2 */
      if(ret == BUFREQ_OK)
      {
        /* Rx indication with positive result */
        PduR_SecOCTpRxIndication(SecOC_GetAuthenticExternalIdOfRxPduInfo(rxPduId), E_OK);
      }
      else
      {
        /* Rx indication with negative result */
        PduR_SecOCTpRxIndication(SecOC_GetAuthenticExternalIdOfRxPduInfo(rxPduId), E_NOT_OK);
      }
    }
      break;
# endif
    default:   /* COV_SECOC_MISRA */
      break;
  }

  SECOC_DUMMY_STATEMENT(pduInfoPtr);    /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}
#endif

#if((SECOC_DEVMODEENABLED == STD_ON) && (SECOC_RXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_RxIf_ForwardPduToUpperLayerDevMode
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_RxIf_ForwardPduToUpperLayerDevMode(SecOC_RxPduInfoIterType rxPduId)
{
  PduInfoType pduInfo;
# if (SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO == STD_ON)
  PduLengthType sduLength = (PduLengthType) SecOC_GetLengthOfRxAuthenticPduDataContainer(rxPduId);

  pduInfo.SduDataPtr = SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetStartIdxOfRxAuthenticPduDataContainer(rxPduId));
  pduInfo.SduLength = sduLength;
# else
  pduInfo.SduDataPtr = NULL_PTR;
  pduInfo.SduLength = 0u;
# endif
  SecOC_RxIf_ForwardPduToUpperLayer(rxPduId, &pduInfo); /* SBSW_SECOC_PTR2FUNC_2 */
  SecOC_SetRxPduState(rxPduId, SECOC_VERIFIED_RXPDUSTATE);      /* SBSW_SECOC_CSL02 */
}
#endif

#if (SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfRxSecuredPduDataContainer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerSecuredPduStartIdxOfRxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfRxSecuredPduDataContainer(SecOC_RxPduInfoIterType rxPduId)
{
  return (SecOC_SecuredPduDataContainerSecuredPduStartIdxOfRxPduInfoType) (SecOC_GetSecuredPduDataContainerSecuredPduStartIdxOfRxPduInfo(rxPduId) + SecOC_GetRxAuthenticPduSizeOffset(rxPduId));
}
#endif

#if (SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfRxAuthenticPduDataContainer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_AuthenticPduDataContainerAuthenticPduStartIdxOfRxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfRxAuthenticPduDataContainer(SecOC_RxPduInfoIterType rxPduId)
{
  return (SecOC_AuthenticPduDataContainerAuthenticPduStartIdxOfRxPduInfoType) (SecOC_GetAuthenticPduDataContainerAuthenticPduStartIdxOfRxPduInfo(rxPduId) + SecOC_GetRxAuthenticPduSizeOffset(rxPduId));
}
#endif

#if (SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetLengthOfRxAuthenticPduDataContainer
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_AuthenticPduDataContainerAuthenticPduLengthOfRxPduInfoType, SECOC_CODE) SecOC_GetLengthOfRxAuthenticPduDataContainer(SecOC_RxPduInfoIterType rxPduId)
{
  return (SecOC_AuthenticPduDataContainerAuthenticPduLengthOfRxPduInfoType) (SecOC_GetAuthenticPduDataContainerAuthenticPduLengthOfRxPduInfo(rxPduId) - SecOC_GetRxAuthenticPduSizeOffset(rxPduId));
}
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_GetStartIdxOfRxAuthenticPduDataContainerDataToAuthenticate
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_AuthenticPduDataContainerDataToAuthenticateStartIdxOfRxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfRxAuthenticPduDataContainerDataToAuthenticate(SecOC_RxPduInfoIterType rxPduId)
{
  return (SecOC_AuthenticPduDataContainerDataToAuthenticateStartIdxOfRxPduInfoType) (SecOC_GetAuthenticPduDataContainerDataToAuthenticateStartIdxOfRxPduInfo(rxPduId) + SecOC_GetRxAuthenticPduSizeOffset(rxPduId));
}
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_GetLengthOfRxRxAuthenticPduDataContainerDataToAuthenticate
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_AuthenticPduDataContainerDataToAuthenticateLengthOfRxPduInfoType, SECOC_CODE) SecOC_GetLengthOfRxRxAuthenticPduDataContainerDataToAuthenticate(SecOC_RxPduInfoIterType rxPduId)
{
  return (SecOC_AuthenticPduDataContainerDataToAuthenticateLengthOfRxPduInfoType) (SecOC_GetAuthenticPduDataContainerDataToAuthenticateLengthOfRxPduInfo(rxPduId) - SecOC_GetRxAuthenticPduSizeOffset(rxPduId));
}
#endif

#if ((SECOC_RXPDUINFO == STD_ON) && (SECOC_TRANSPORT_PROTOCOL == STD_ON))
/**********************************************************************************************************************
  SecOC_GetStartIdxOfRxSecuredPduDataContainerSecuredPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerSecuredPduStartIdxOfRxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfRxSecuredPduDataContainerSecuredPdu(SecOC_RxPduInfoIterType rxPduId)
{
  return (SecOC_SecuredPduDataContainerSecuredPduStartIdxOfRxPduInfoType) (SecOC_GetSecuredPduDataContainerSecuredPduStartIdxOfRxPduInfo(rxPduId) + SecOC_GetRxAuthenticPduSizeOffset(rxPduId));
}
#endif

#if (((SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO == STD_ON) && (SECOC_VERIFICATIONSTATUSCALLOUTWITHSECUREDPDU == STD_ON)) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_GetLengthOfRxSecuredPduDataContainerSecuredPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerSecuredPduLengthOfRxPduInfoType, SECOC_CODE) SecOC_GetLengthOfRxSecuredPduDataContainerSecuredPdu(SecOC_RxPduInfoIterType rxPduId)
{
  return (SecOC_SecuredPduDataContainerSecuredPduLengthOfRxPduInfoType) (SecOC_GetSecuredPduDataContainerSecuredPduLengthOfRxPduInfo(rxPduId) - SecOC_GetRxAuthenticPduSizeOffset(rxPduId));
}
#endif

#if ((SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO == STD_ON) && (SECOC_TRANSPORT_PROTOCOL == STD_ON))
/**********************************************************************************************************************
  SecOC_GetLengthOfRxTpSecuredPduDataContainerSecuredPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerTpSecuredPduLengthOfRxPduInfoType, SECOC_CODE) SecOC_GetLengthOfRxTpSecuredPduDataContainerSecuredPdu(SecOC_RxPduInfoIterType rxPduId)
{
  return (SecOC_SecuredPduDataContainerTpSecuredPduLengthOfRxPduInfoType) (SecOC_GetSecuredPduDataContainerTpSecuredPduLengthOfRxPduInfo(rxPduId) - SecOC_GetRxAuthenticPduSizeOffset(rxPduId));
}
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_AddRxDataIdsToDataToAuthenticate
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_AddRxDataIdsToDataToAuthenticate(SecOC_RxPduInfoIterType rxPduId)
{
  SecOC_AuthenticPduDataContainerDataToAuthenticateEndIdxOfRxPduInfoType dataIdFirstByteIdx = SecOC_GetStartIdxOfRxAuthenticPduDataContainerDataToAuthenticate(rxPduId);
  SecOC_AuthenticPduDataContainerDataToAuthenticateEndIdxOfRxPduInfoType dataIdSecondByteIdx = (SecOC_GetStartIdxOfRxAuthenticPduDataContainerDataToAuthenticate(rxPduId) + 1u);

  if(dataIdFirstByteIdx < SecOC_GetAuthenticPduDataContainerDataToAuthenticateEndIdxOfRxPduInfo(rxPduId))       /* COV_SECOC_SAFEBSW_CHECK */
  {
    SecOC_SetAuthenticPduDataContainer(dataIdFirstByteIdx, SecOC_GetDataIdFirstByteOfRxPduInfo(rxPduId));       /* SBSW_SECOC_CALCULATED_INDEX */
  }
  if(dataIdSecondByteIdx < SecOC_GetAuthenticPduDataContainerDataToAuthenticateEndIdxOfRxPduInfo(rxPduId))      /* COV_SECOC_SAFEBSW_CHECK */
  {
    SecOC_SetAuthenticPduDataContainer(dataIdSecondByteIdx, SecOC_GetDataIdSecondByteOfRxPduInfo(rxPduId));     /* SBSW_SECOC_CALCULATED_INDEX */
  }
}
#endif

#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_AddTxDataIdsToDataToAuthenticate
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_AddTxDataIdsToDataToAuthenticate(SecOC_TxPduInfoIterType txPduId)
{
  SecOC_AuthenticPduDataContainerDataToAuthenticateEndIdxOfTxPduInfoType dataIdFirstByteIdx = SecOC_GetStartIdxOfTxDataToAuthenticate(txPduId);
  SecOC_AuthenticPduDataContainerDataToAuthenticateEndIdxOfTxPduInfoType dataIdSecondByteIdx = (SecOC_GetStartIdxOfTxDataToAuthenticate(txPduId) + 1u);

  if(dataIdFirstByteIdx < SecOC_GetAuthenticPduDataContainerDataToAuthenticateEndIdxOfTxPduInfo(txPduId))       /* COV_SECOC_SAFEBSW_CHECK */
  {
    SecOC_SetAuthenticPduDataContainer(dataIdFirstByteIdx, SecOC_GetDataIdFirstByteOfTxPduInfo(txPduId));       /* SBSW_SECOC_CALCULATED_INDEX */
  }
  if(dataIdSecondByteIdx < SecOC_GetAuthenticPduDataContainerDataToAuthenticateEndIdxOfTxPduInfo(txPduId))      /* COV_SECOC_SAFEBSW_CHECK */
  {
    SecOC_SetAuthenticPduDataContainer(dataIdSecondByteIdx, SecOC_GetDataIdSecondByteOfTxPduInfo(txPduId));     /* SBSW_SECOC_CALCULATED_INDEX */
  }
}
#endif

#if (SECOC_AUTHENTICPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfTxAuthenticPduDataContainerData
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_AuthenticPduDataContainerAuthenticPduStartIdxOfTxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfTxAuthenticPduDataContainerData(SecOC_TxPduInfoIterType txPduId)
{
  return (SecOC_AuthenticPduDataContainerAuthenticPduStartIdxOfTxPduInfoType) (SecOC_GetAuthenticPduDataContainerAuthenticPduStartIdxOfTxPduInfo(txPduId) + SecOC_GetTxAuthenticPduSizeOffset(txPduId));
}
#endif

#if (SECOC_SECUREDPDUDATACONTAINERAUTHPDUHEADERLENGTHUSEDOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfTxAuthPduHeader
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerAuthPduHeaderLengthStartIdxOfTxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfTxAuthPduHeader(SecOC_TxPduInfoIterType txPduId)
{
  return (SecOC_SecuredPduDataContainerAuthPduHeaderLengthStartIdxOfTxPduInfoType) (SecOC_GetSecuredPduDataContainerAuthPduHeaderLengthStartIdxOfTxPduInfo(txPduId) + SecOC_GetTxAuthenticPduSizeOffset(txPduId));
}
#endif

#if (SECOC_AUTHENTICPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfTxSecuredPduDataContainerData
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerAuthenticPduStartIdxOfTxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfTxSecuredPduDataContainerData(SecOC_TxPduInfoIterType txPduId)
{
  return (SecOC_SecuredPduDataContainerAuthenticPduStartIdxOfTxPduInfoType) (SecOC_GetSecuredPduDataContainerAuthenticPduStartIdxOfTxPduInfo(txPduId) + SecOC_GetTxAuthenticPduSizeOffset(txPduId));
}
#endif

#if (SECOC_AUTHENTICPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetLengthOfTxSecuredPduDataContainerData
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerAuthenticPduLengthOfTxPduInfoType, SECOC_CODE) SecOC_GetLengthOfTxSecuredPduDataContainerData(SecOC_TxPduInfoIterType txPduId)
{
  return (SecOC_SecuredPduDataContainerAuthenticPduLengthOfTxPduInfoType) (SecOC_GetSecuredPduDataContainerAuthenticPduLengthOfTxPduInfo(txPduId) - SecOC_GetTxAuthenticPduSizeOffset(txPduId));
}
#endif

#if (SECOC_SECUREDPDUDATACONTAINERSECUREDPDUSTARTIDXOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfTxSecuredPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerSecuredPduStartIdxOfTxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfTxSecuredPdu(SecOC_TxPduInfoIterType txPduId)
{
  return (SecOC_SecuredPduDataContainerSecuredPduStartIdxOfTxPduInfoType) (SecOC_GetSecuredPduDataContainerSecuredPduStartIdxOfTxPduInfo(txPduId) + SecOC_GetTxAuthenticPduSizeOffset(txPduId));
}
#endif

#if (SECOC_SECUREDPDUDATACONTAINERSECUREDPDUSTARTIDXOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetLengthOfTxSecuredPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerSecuredPduLengthOfTxPduInfoType, SECOC_CODE) SecOC_GetLengthOfTxSecuredPdu(SecOC_TxPduInfoIterType txPduId)
{
  return (SecOC_SecuredPduDataContainerSecuredPduLengthOfTxPduInfoType) (SecOC_GetSecuredPduDataContainerSecuredPduLengthOfTxPduInfo(txPduId) - SecOC_GetTxAuthenticPduSizeOffset(txPduId));
}
#endif

#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_GetStartIdxOfTxDataToAuthenticate
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_AuthenticPduDataContainerDataToAuthenticateStartIdxOfTxPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfTxDataToAuthenticate(SecOC_TxPduInfoIterType txPduId)
{
  return (SecOC_AuthenticPduDataContainerDataToAuthenticateStartIdxOfTxPduInfoType) (SecOC_GetAuthenticPduDataContainerDataToAuthenticateStartIdxOfTxPduInfo(txPduId) + SecOC_GetTxAuthenticPduSizeOffset(txPduId));
}
#endif

#if ((SECOC_TXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
/**********************************************************************************************************************
  SecOC_GetLengthOfDataToAuthenticate
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_AuthenticPduDataContainerDataToAuthenticateLengthOfTxPduInfoType, SECOC_CODE) SecOC_GetLengthOfDataToAuthenticate(SecOC_TxPduInfoIterType txPduId)
{
  return (SecOC_AuthenticPduDataContainerDataToAuthenticateLengthOfTxPduInfoType) (SecOC_GetAuthenticPduDataContainerDataToAuthenticateLengthOfTxPduInfo(txPduId) - SecOC_GetTxAuthenticPduSizeOffset(txPduId));
}
#endif

#if (SECOC_MESSAGELINKLENOFTXSPLITPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfTxMessageLinkerAuthSplitPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerAuthenticSplitPduStartIdxOfTxSplitPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfTxMessageLinkerAuthSplitPdu(SecOC_TxPduInfoIterType txPduId)
{
  return (SecOC_SecuredPduDataContainerAuthenticSplitPduStartIdxOfTxSplitPduInfoType) (SecOC_GetSecuredPduDataContainerMessageLinkerAuthSplitPduStartIdxOfTxSplitPduInfo(SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduId)) + SecOC_GetTxAuthenticPduSizeOffset(txPduId));
}
#endif

#if (SECOC_TXSPLITPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetStartIdxOfTxAuthenticSplitPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerAuthenticSplitPduStartIdxOfTxSplitPduInfoType, SECOC_CODE) SecOC_GetStartIdxOfTxAuthenticSplitPdu(SecOC_TxPduInfoIterType txPduId)
{
  return (SecOC_SecuredPduDataContainerAuthenticSplitPduStartIdxOfTxSplitPduInfoType) (SecOC_GetSecuredPduDataContainerAuthenticSplitPduStartIdxOfTxSplitPduInfo(SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduId)) + SecOC_GetTxAuthenticPduSizeOffset(txPduId));
}
#endif

#if (SECOC_TXSPLITPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetLengthOfTxAuthenticSplitPdu
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(SecOC_SecuredPduDataContainerAuthenticSplitPduLengthOfTxSplitPduInfoType, SECOC_CODE) SecOC_GetLengthOfTxAuthenticSplitPdu(SecOC_TxPduInfoIterType txPduId)
{
  return (SecOC_SecuredPduDataContainerAuthenticSplitPduLengthOfTxSplitPduInfoType) (SecOC_GetSecuredPduDataContainerAuthenticSplitPduLengthOfTxSplitPduInfo(SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduId)) - SecOC_GetTxAuthenticPduSizeOffset(txPduId));
}
#endif

#if (SECOC_CONTAINERHEADERSIZEOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_RxIf_RemoveCanFdContainerPadding
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_RxIf_RemoveCanFdContainerPadding(SecOC_RxPduInfoIterType rxPduId)
{
  PduInfoType pduInfo;
  uint32 actualLength;
  uint32 paddingSize;
  pduInfo.SduDataPtr = SecOC_GetAddrSecuredPduDataContainer(SecOC_GetStartIdxOfRxSecuredPduDataContainer(rxPduId));
  pduInfo.SduLength = (PduLengthType) SecOC_GetLengthOfRxAuthenticPduDataContainer(rxPduId);

  actualLength = IpduM_CalculateSizeOfContainer((PduLengthType) SecOC_GetContainerHeaderSizeOfRxPduInfo(rxPduId), &pduInfo);    /* SBSW_SECOC_FUNC_CALL */
  if(actualLength > SecOC_GetLengthOfRxAuthenticPduDataContainer(rxPduId))
  {
    actualLength = SecOC_GetLengthOfRxAuthenticPduDataContainer(rxPduId);
  }
  paddingSize = SecOC_GetLengthOfRxAuthenticPduDataContainer(rxPduId) - actualLength;

  if(paddingSize > 0u)
  {
    if(actualLength > 0u)
    {
      SecOC_SecuredPduDataContainerAuthenticPduStartIdxOfRxPduInfoType endIdx = (SecOC_SecuredPduDataContainerAuthenticPduStartIdxOfRxPduInfoType) (SecOC_GetStartIdxOfRxSecuredPduDataContainer(rxPduId) + actualLength);
      for(; endIdx > SecOC_GetStartIdxOfRxSecuredPduDataContainer(rxPduId); endIdx--)
      {
        SecOC_SecuredPduDataContainerAuthenticPduStartIdxOfRxPduInfoType srcIdx = endIdx - 1u;
        SecOC_SecuredPduDataContainerIterType newAuthenticPduBufferIdx = (SecOC_SecuredPduDataContainerIterType) (srcIdx) + (SecOC_SecuredPduDataContainerIterType) (paddingSize);
        if(newAuthenticPduBufferIdx < SecOC_GetSecuredPduDataContainerAuthenticPduEndIdxOfRxPduInfo(rxPduId))   /* COV_SECOC_SAFEBSW_CHECK */
        {
          SecOC_SetSecuredPduDataContainer(newAuthenticPduBufferIdx, SecOC_GetSecuredPduDataContainer(srcIdx)); /* SBSW_SECOC_CALCULATED_INDEX */
        }
      }
    }
    SecOC_SetRxAuthenticPduSizeOffset(rxPduId, (SecOC_RxAuthenticPduSizeOffsetType) (SecOC_GetRxAuthenticPduSizeOffset(rxPduId) + paddingSize));        /* SBSW_SECOC_CSL01 */
  }
}
#endif

#if (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_MAC_DoUnverifiedProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_MAC_DoUnverifiedProcessing(SecOC_RxPduInfoIterType rxPduId)
{
  boolean wasOverride = FALSE;  /* PRQA S 2981 */ /* MD_MSR_RetVal */
  do
  {
    SecOC_VerifyResultType verificationResult = SECOC_E_VER_NOT_OK;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
    SecOC_VerifyOverrideStatusType verifyOverrideStatus = SecOC_GetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduId));

    if((verifyOverrideStatus == SECOC_CANCEL_VERIFYOVERRIDESTATUS)
# if (SECOC_ENABLE_FORCED_PASS_OVERRIDESTATUS == STD_ON)
       || (verifyOverrideStatus == SECOC_PASS_UNTIL_LIMIT_VERIFYOVERRIDESTATUS) || (verifyOverrideStatus == SECOC_PASS_UNTIL_NOTICE_VERIFYOVERRIDESTATUS)
# endif
      )
    {
# if (SECOC_CUSTOM_AUTHENTICATION_API == STD_ON)
      verificationResult = SecOC_Verify_MACUsingCustomCallout(rxPduId); /* PRQA S 2982 */ /* MD_SecOC_2982 */
# else
      verificationResult = SecOC_Verify_MACUsingCSM43(rxPduId); /* PRQA S 2982 */ /* MD_SecOC_2982 */
# endif
      wasOverride = FALSE;
# if (SECOC_ENABLE_FORCED_PASS_OVERRIDESTATUS == STD_ON)
      if(verifyOverrideStatus == SECOC_PASS_UNTIL_LIMIT_VERIFYOVERRIDESTATUS)
      {
        SecOC_DecNumberOfMessagesToOverride(SecOC_GetNumberOfMessagesToOverrideIdxOfRxPduInfo(rxPduId));        /* SBSW_SECOC_CSL03 */
        if(SecOC_GetNumberOfMessagesToOverride(SecOC_GetNumberOfMessagesToOverrideIdxOfRxPduInfo(rxPduId)) == 0u)
        {
          SecOC_SetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduId), SECOC_CANCEL_VERIFYOVERRIDESTATUS);       /* SBSW_SECOC_CSL03 */
        }
      }
# endif
    }
    else
    {
      verificationResult = SecOC_Verify_GetVerificationResultOfOverrideStatus(rxPduId); /* PRQA S 2982 */ /* MD_SecOC_2982 */
      wasOverride = TRUE;
    }

    if(SecOC_GetRxPduState(rxPduId) == SECOC_UNVERIFIED_VERIFICATION_FINISHED_RXPDUSTATE)
    {
# if(SECOC_IGNORE_VERIFICATION_RESULT == STD_ON)
      /* \trace SPEC-2969140, SPEC-2969077 */
      verificationResult = SECOC_E_VER_OK;
# endif
      SecOC_Verify_ProcessVerificationResult(verificationResult, rxPduId, verifyOverrideStatus);
    }
    if((wasOverride == TRUE) && (verificationResult == SECOC_E_VER_NOT_OK))     /* PRQA S 2995 */ /* MD_SecOC_2995 */ /* COV_SECOC_IGNORE_VERIFICATION_RESULT */
    {
# if (SECOC_EXISTS_AUTHENTIC_SPLIT_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
      if(SecOC_IsRxSplitMessageCounterUsedOfRxPduInfo(rxPduId)) /* COV_SECOC_SPLIT_ONLY_CONFIG */
      {
        SecOC_SetRxSplitMessageCounter(SecOC_GetRxSplitMessageCounterIdxOfRxPduInfo(rxPduId), 0);       /* SBSW_SECOC_CSL03 */
      }
# endif
      SecOC_SetRxPduState(rxPduId, SECOC_ABORTED_RXPDUSTATE);   /* SBSW_SECOC_CSL02 */
    }
  }
  while(SecOC_GetRxPduState(rxPduId) == SECOC_UNVERIFIED_VERIFICATION_TO_BE_STARTED_RXPDUSTATE);        /* COV_SECOC_VERIFICATION_TO_BE_STARTED_LOOP */
}
#endif


#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_MAC
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6050, 6010, 6030, 6080 1 */ /* MD_MSR_STCAL, MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_MAC(SecOC_RxPduInfoIterType rxPduId)
{
  if(SecOC_GetRxPduState(rxPduId) == SECOC_PRE_VERIFY_START_RXPDUSTATE) /* COV_SECOC_CUSTOM_AUTH_API */
  {
    SecOC_SetRxPduState(rxPduId, SECOC_COPY_AUTHENTIC_PDU_TO_DATA_TO_AUTHENTICATE_RXPDUSTATE);  /* SBSW_SECOC_CSL80 */
  }
  if(SecOC_GetRxPduState(rxPduId) == SECOC_COPY_AUTHENTIC_PDU_TO_DATA_TO_AUTHENTICATE_RXPDUSTATE)       /* COV_SECOC_CSM_SYNCHRONOUS_MODE_ONLY */
  {
# if (SECOC_CONTAINERHEADERSIZEOFRXPDUINFO == STD_ON)
    if(SecOC_GetContainerHeaderSizeOfRxPduInfo(rxPduId) != 0u)
    {
      SecOC_RxIf_RemoveCanFdContainerPadding(rxPduId);
    }
# endif
# if(SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO == STD_ON)
    VStdMemCpyRamToRam(SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetStartIdxOfRxAuthenticPduDataContainer(rxPduId)), SecOC_GetAddrSecuredPduDataContainer(SecOC_GetStartIdxOfRxSecuredPduDataContainer(rxPduId)), SecOC_GetLengthOfRxAuthenticPduDataContainer(rxPduId));    /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_CPY_2RAM */
# endif

# if (SECOC_CUSTOM_AUTHENTICATION_API == STD_ON)
    SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_RXPDUSTATE);  /* SBSW_SECOC_CSL02 */
  }
# else
    SecOC_SetRxPduState(rxPduId, SECOC_FRESHNESS_VALUE_NEEDED_RXPDUSTATE);      /* SBSW_SECOC_CSL02 */
  }

#  if (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON)
  if(SecOC_IsSecuredRxPduVerficationOfRxPduInfo(rxPduId))
  {
    if(SecOC_GetRxPduState(rxPduId) == SECOC_FRESHNESS_VALUE_NEEDED_RXPDUSTATE) /* COV_SECOC_SYNC_CSM_ONLY_CONFIG */
    {
#   if (SECOC_ENABLE_FORCED_PASS_OVERRIDESTATUS == STD_ON)
      SecOC_VerifyOverrideStatusType verifyOverrideStatus = SecOC_GetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduId));
      if((verifyOverrideStatus == SECOC_OVERRIDE_SKIP_UNTIL_LIMIT_VERIFYOVERRIDESTATUS) || (verifyOverrideStatus == SECOC_OVERRIDE_SKIP_UNTIL_NOTICE_VERIFYOVERRIDESTATUS))
      {
        SecOC_SetRxPduState(rxPduId, SECOC_UNVERIFIED_RXPDUSTATE);      /* SBSW_SECOC_CSL02 */
      }
      else
#   endif
      {
        /* --- calculate FreshnessVerifyValue and copy it to the buffer --- */
        SecOC_FreshnessValue_CalculateVerifyValueFromCallout(rxPduId);
      }
    }
  }
  else
  {
    SecOC_SetRxPduState(rxPduId, SECOC_VERIFIED_RXPDUSTATE);    /* SBSW_SECOC_CSL02 */
    SecOC_Verify_ProcessSuccessfulVerificationResult(rxPduId, SECOC_VERIFICATIONSUCCESS);
  }
#  else
  {
    SecOC_SetRxPduState(rxPduId, SECOC_VERIFIED_RXPDUSTATE);    /* SBSW_SECOC_CSL02 */
    SecOC_Verify_ProcessSuccessfulVerificationResult(rxPduId, SECOC_VERIFICATIONSUCCESS);
  }
#  endif
# endif

  /* --- calculate MAC and verify the Secured Pdu --- */

  /* calculate the MAC with the referenced key and Crypto Library and compare the calculated MAC with the received MAC of the Secured PDU. */
# if (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON)
  if(SecOC_IsRxPduStateUnverified(rxPduId))     /* COV_SECOC_CUSTOM_AUTH_API */
  {
    SecOC_Verify_MAC_DoUnverifiedProcessing(rxPduId);
  }
# endif
  if(SecOC_GetRxPduState(rxPduId) == SECOC_ABORTED_RXPDUSTATE)  /* COV_SECOC_NO_VERIFICATION_ENABLED */
  {
# if(SECOC_DEVMODEENABLED == STD_ON)
    if(SecOC_IsDevModeEnabled())
    {
      SecOC_RxIf_ForwardPduToUpperLayerDevMode(rxPduId);
    }
# endif
    SecOC_SetRxPduState(rxPduId, SECOC_IDLE_RXPDUSTATE);        /* SBSW_SECOC_CSL02 */
  }
  if(SecOC_GetRxPduState(rxPduId) == SECOC_VERIFIED_RXPDUSTATE) /* COV_SECOC_NO_VERIFICATION_ENABLED */
  {
    SecOC_SetRxPduState(rxPduId, SECOC_IDLE_RXPDUSTATE);        /* SBSW_SECOC_CSL02 */
  }
}
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_RxIf_HandlePdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_RxIf_HandlePdus(void)
{
  /* Verify the received Secured I-Pdus and hand the Authentic I-Pdus to the UpperLayer. */
  SecOC_RxPduInfoIterType rxPduId = 0;
  for(; rxPduId < SecOC_GetSizeOfRxPduInfo(); rxPduId++)
  {
    if(SecOC_GetRxPduState(rxPduId) != SECOC_IDLE_RXPDUSTATE)
    {
# if (SECOC_RXIMMEDIATEPROCESSINGONGOING == STD_ON)
      if(!SecOC_IsRxImmediateProcessingOngoing(rxPduId))        /* COV_SECOC_IMMEDIATE_CHECK */
# endif
      {
# if (SECOC_CSMJOBTIMEOUTCNTOFRXPDUINFO == STD_ON)
        SecOC_DlMon_RxCSMTimeoutSupervision(rxPduId);
# endif
# if (SECOC_PDUSWITHQUEUESIDXOFRXPDUINFO == STD_ON)
        do
        {
          SecOC_Verify_MAC(rxPduId);
        }
        while(SecOC_Queue_LoadNextQueueElement(rxPduId) == TRUE);
# else
        SecOC_Verify_MAC(rxPduId);
# endif
      }
    }
  }
}
#endif

#if (SECOC_CSMJOBTIMEOUTCNTOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_DlMon_RxCSMTimeoutSupervision
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_DlMon_RxCSMTimeoutSupervision(SecOC_RxPduInfoIterType rxPduId)
{
  boolean cancelCsmJob = FALSE;

  if(SecOC_GetRxPduState(rxPduId) == SECOC_UNVERIFIED_VERIFICATION_FINISHED_WAIT_FOR_CALLBACK_RXPDUSTATE)
  {
    SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
    if(SecOC_GetRxCSMJobTimeoutCntValue(rxPduId) > 0u)
    {
      SecOC_DecRxCSMJobTimeoutCntValue(rxPduId);        /* SBSW_SECOC_CSL02 */
      if(SecOC_GetRxCSMJobTimeoutCntValue(rxPduId) == 0u)
      {
        /* timeout occured */
        SecOC_SetRxPduState(rxPduId, SECOC_ABORTED_RXPDUSTATE); /* SBSW_SECOC_CSL02 */
        cancelCsmJob = TRUE;
      }
    }
    SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
    if(cancelCsmJob)
    {
      Std_ReturnType ret = Csm_CancelJob(SecOC_GetAuthenticationLayerIdOfRxPduInfo(rxPduId), CRYPTO_OPERATIONMODE_SINGLECALL);
      if(ret == E_NOT_OK)
      {
        SecOC_ReportRuntimeDetError(SECOC_SID_MAIN_FUNCTION_RX, SECOC_E_CSM_CANCEL_JOB_FAILED);
      }
    }
  }
}
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) && (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_FreshnessValue_GetVerifyValueFromCallout
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(Std_ReturnType, SECOC_CODE) SecOC_FreshnessValue_GetVerifyValueFromCallout(SecOC_RxPduInfoIterType rxPduId, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) freshnessValue, uint8 freshnessValueBufferLength)
{
  Std_ReturnType ret = E_NOT_OK;
  uint32 freshnessValueLength = ((uint32) (SecOC_GetAuthenticPduDataContainerFreshnessVerifyValueLengthOfRxPduInfo(rxPduId)) << 3u);
  if(freshnessValueLength <= ((uint32) (freshnessValueBufferLength) << 3u))     /* COV_SECOC_SAFEBSW_CHECK */
  {
# if ((SECOC_USE_GET_RX_FRESHNESS_AUTH_DATA_API == STD_ON) && (SECOC_USE_GET_RX_FRESHNESS_API == STD_ON))
    if(SecOC_GetAuthDataFreshnessOfRxPduInfo(rxPduId) == SECOC_USE_AUTHDATAFRESHNESSOFRXPDUINFO)
# endif
# if (SECOC_USE_GET_RX_FRESHNESS_AUTH_DATA_API == STD_ON)
    {
      uint8 truncatedFreshnessValue[SECOC_TRUNC_FRESHNESS_SIZE_RXPDUS] = { 0u };        /* PRQA S 3678 */ /* MD_MSR_Rule8.13 */

#  if (SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFRXPDUINFO == STD_ON)
#   if (SECOC_USE_MSB_ALIGNED_TRUNCATED_FRESHNESS_VALUE_IN_FVM == STD_OFF)
      SecOC_FreshnessValue_GetParsedFreshnessValue(rxPduId, truncatedFreshnessValue, SECOC_TRUNC_FRESHNESS_SIZE_RXPDUS);        /* SBSW_SECOC_PTR2FUNC_2 */
#   else
      SecOC_FreshnessValue_GetParsedFreshnessValueMSBAligned(rxPduId, truncatedFreshnessValue, SECOC_TRUNC_FRESHNESS_SIZE_RXPDUS);      /* SBSW_SECOC_PTR2FUNC_2 */
#   endif
#  endif
      /* *INDENT-OFF* */
      ret = GetRxFreshnessAuthData_FuncName(
          SecOC_GetExternalFreshnessIdOfRxPduInfo(rxPduId),
          truncatedFreshnessValue,
          SecOC_GettruncatedFreshnessValueLengthOfRxPduInfo(rxPduId),
          SecOC_GetAddrSecuredPduDataContainer(SecOC_GetStartIdxOfRxSecuredPduDataContainer(rxPduId) + SecOC_GetAuthDataFreshnessStartBytePositionOfRxPduInfo(rxPduId)),
          SecOC_GetAuthDataFreshnessLenOfRxPduInfo(rxPduId),
          (uint16) SecOC_GetVerifyAttemptsCounter(rxPduId),
          freshnessValue,
          &freshnessValueLength
      ); /* SBSW_SECOC_PTR2FUNC_8 */
      /* *INDENT-ON* */
    }
# endif
# if ((SECOC_USE_GET_RX_FRESHNESS_AUTH_DATA_API == STD_ON) && (SECOC_USE_GET_RX_FRESHNESS_API == STD_ON))
    else
# endif
# if (SECOC_USE_GET_RX_FRESHNESS_API == STD_ON)
    {
      uint8 truncatedFreshnessValue[SECOC_TRUNC_FRESHNESS_SIZE_RXPDUS] = { 0u };        /* PRQA S 3678 */ /* MD_MSR_Rule8.13 */

#  if (SECOC_SECUREDPDUDATACONTAINERTRUNCATEDFRESHNESSVALUEUSEDOFRXPDUINFO == STD_ON)
#   if (SECOC_USE_MSB_ALIGNED_TRUNCATED_FRESHNESS_VALUE_IN_FVM == STD_OFF)
      SecOC_FreshnessValue_GetParsedFreshnessValue(rxPduId, truncatedFreshnessValue, SECOC_TRUNC_FRESHNESS_SIZE_RXPDUS);        /* SBSW_SECOC_PTR2FUNC_2 */
#   else
      SecOC_FreshnessValue_GetParsedFreshnessValueMSBAligned(rxPduId, truncatedFreshnessValue, SECOC_TRUNC_FRESHNESS_SIZE_RXPDUS);      /* SBSW_SECOC_PTR2FUNC_2 */
#   endif
#  endif
      /* *INDENT-OFF* */
      ret = GetRxFreshness_FuncName(
          SecOC_GetExternalFreshnessIdOfRxPduInfo(rxPduId),
          truncatedFreshnessValue,
          SecOC_GettruncatedFreshnessValueLengthOfRxPduInfo(rxPduId),
          (uint16) SecOC_GetVerifyAttemptsCounter(rxPduId),
          freshnessValue,
          &freshnessValueLength
      ); /* SBSW_SECOC_PTR2FUNC_6 */
      /* *INDENT-ON* */
    }
# endif
  }
  return ret;
}
#endif

#if(SECOC_AUTHHEADERLENGTHOFRXSECUREDPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_GetLengthFromHeader
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(PduLengthType, SECOC_CODE) SecOC_GetLengthFromHeader(P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) headerData, uint8 headerLength)
{
  uint32 ret = 0;
  uint8 i = 0;

  for(; i < headerLength; i++)
  {
    ret = ret << 8u;    /* PRQA S 2982 */ /* MD_SecOC_2982 */
    ret = ret | headerData[i];
  }

  return (PduLengthType) ret;
}
#endif

#if(SECOC_SECUREDPDUDATACONTAINERAUTHPDUHEADERLENGTHUSEDOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_SerializeLengthToHeader
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_SerializeLengthToHeader(P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) dest, uint8 headerLength, PduLengthType authenticPduLength)
{
  uint8 i = headerLength;
  uint32 workingAuthenticPduLength = (uint32) authenticPduLength;
  for(; i > 0u; i--)
  {
    uint8 destIdx = (uint8) (i - 1);    /* PRQA S 1860 */ /* MD_SecOC_1860 */
    dest[destIdx] = (uint8) (workingAuthenticPduLength & 0xFFu);        /* SBSW_SECOC_ARRAY_WRITE_ACCESS_WITH_LENGTH */
    workingAuthenticPduLength = workingAuthenticPduLength >> 8u;
  }

}
#endif

#if (SECOC_RXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_RxIf_InitPdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_RxIf_InitPdus(void)
{
  SecOC_RxPduInfoIterType rxPduId = 0;
  for(; rxPduId < SecOC_GetSizeOfRxPduInfo(); rxPduId++)
  {
    SecOC_SetVerifyAttemptsCounter(rxPduId, 0); /* SBSW_SECOC_CSL02 */
    SecOC_SetRxBuildAttemptsCounter(rxPduId, 0);        /* SBSW_SECOC_CSL02 */
    SecOC_SetRxTpWrittenBytesCounter(rxPduId, 0);       /* SBSW_SECOC_CSL02 */
    SecOC_SetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduId), SECOC_CANCEL_VERIFYOVERRIDESTATUS);     /* SBSW_SECOC_CSL02 */
    SecOC_SetRxAuthenticPduSizeOffset(rxPduId, 0);      /* SBSW_SECOC_CSL02 */
# if (SECOC_RXIMMEDIATEPROCESSINGONGOING == STD_ON)
    SecOC_SetRxImmediateProcessingOngoing(rxPduId, FALSE);      /* SBSW_SECOC_CSL02 */
# endif
# if (SECOC_CSMJOBTIMEOUTCNTOFRXPDUINFO ==  STD_ON)
    SecOC_SetRxCSMJobTimeoutCntValue(rxPduId, 0);       /* SBSW_SECOC_CSL02 */
# endif
  }
# if (SECOC_EXISTS_AUTHENTIC_SPLIT_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
  {
    SecOC_SizeOfRxSplitMessageCounterType splitCounterIndex = 0;
    for(; splitCounterIndex < SecOC_GetSizeOfRxSplitMessageCounter(); splitCounterIndex++)
    {
      SecOC_SetRxSplitMessageCounter(splitCounterIndex, 0);     /* SBSW_SECOC_CSL01 */
    }
  }
# endif
}
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxIf_InitPdus
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_TxIf_InitPdus(void)
{
  SecOC_TxPduInfoIterType txPduId = 0;
  for(; txPduId < SecOC_GetSizeOfTxPduInfo(); txPduId++)
  {
    SecOC_SetTxBuildAttemptsCounter(txPduId, 0);        /* SBSW_SECOC_CSL02 */
    SecOC_SetTxTpWrittenBytesCounter(txPduId, 0);       /* SBSW_SECOC_CSL02 */
    SecOC_SetTxAuthenticPduSizeOffset(txPduId, 0);      /* SBSW_SECOC_CSL02 */
# if (SECOC_TXIMMEDIATEPROCESSINGONGOING == STD_ON)
    SecOC_SetTxImmediateProcessingOngoing(txPduId, FALSE);      /* SBSW_SECOC_CSL02 */
# endif
# if (SECOC_CANCEL_TRANSMIT_API == STD_ON)
    SecOC_SetCancelTxTransmit(txPduId, FALSE);  /* SBSW_SECOC_CSL02 */
# endif
# if (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF)
    SecOC_AddTxDataIdsToDataToAuthenticate(txPduId);
# endif
# if (SECOC_CSMJOBTIMEOUTCNTOFTXPDUINFO == STD_ON)
    SecOC_SetTxCSMJobTimeoutCntValue(txPduId, 0);       /* SBSW_SECOC_CSL02 */
# endif
  }
}
#endif

#if (SECOC_CURRENTMESSAGELINKER == STD_ON)
/**********************************************************************************************************************
  SecOC_Split_CheckMessageLinker
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(boolean, SECOC_CODE) SecOC_Split_CheckMessageLinker(P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) msgLinkNew, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) msgLinkOld, SecOC_CurrentMessageLinkerLengthOfRxSecuredPduInfoType msgLinkLenInBytes, boolean currentIsNewStatus)
{
  boolean currentIsNewStatusIntern = currentIsNewStatus;
  boolean msgLinkerState = TRUE;
  uint16_least i = 0u;

  for(; i < msgLinkLenInBytes; i++)
  {
    if(msgLinkNew[i] != msgLinkOld[i])
    {
      msgLinkerState = FALSE;
      break;
    }
  }

  if(msgLinkerState == FALSE)
  {
    currentIsNewStatusIntern = FALSE;
    VStdMemCpyRamToRam(msgLinkOld, msgLinkNew, msgLinkLenInBytes);      /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_PTR2FUNC_3 */
  }

  return currentIsNewStatusIntern;
}
#endif

#if ((SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF) || (SECOC_CURRENTMESSAGELINKER == STD_ON) || (SECOC_MESSAGELINKLENOFTXSPLITPDUINFO == STD_ON))
/**********************************************************************************************************************
  SecOC_GetArrayByteAligned
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
/* PRQA S 6060, 6030 1 */ /* MD_MSR_STPAR, MD_MSR_STCYC */
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_GetArrayByteAligned(P2CONST(uint8, AUTOMATIC, SECOC_APPL_DATA) srcBuffer, P2VAR(uint8, AUTOMATIC, SECOC_APPL_DATA) destBuffer, uint16 copyBitLen, uint8 srcStartBitInByte, uint8 destBitOffset, boolean isSrcRightAligned)
{
  uint16_least i = 0u;

  uint16 msgLinkByte = copyBitLen >> 3u;
  uint16 bitsToBeCopied = copyBitLen;

  if((copyBitLen & 7u) != 0u) /* COV_SECOC_ONLY_TX_AUTHINFO */
  {
    msgLinkByte++;
  }

  for(; i < msgLinkByte; i++)
  {
    uint8 byteToCopy = 0u;      /* PRQA S 2981 */ /* MD_MSR_RetVal */

    if(srcStartBitInByte != 0u) /* COV_SECOC_ONLY_TX_AUTHINFO */
    {
      if(isSrcRightAligned == TRUE)     /* COV_SECOC_ONLY_TX_AUTHINFO */
      {
        uint8 numLeftShift = (uint8) srcStartBitInByte;
        uint8 numRightShift = (uint8) (8u - srcStartBitInByte);
        byteToCopy = (uint8) (srcBuffer[i] << numLeftShift);
        if(bitsToBeCopied > numRightShift)      /* COV_SECOC_MSGLINK_FIX_LENGTH */
        {
          byteToCopy = byteToCopy | ((uint8) (srcBuffer[i + 1u] >> numRightShift));
        }
      }
      else
      {
        uint8 numLeftShift = (uint8) (8u - srcStartBitInByte);
        uint8 numRightShift = (uint8) srcStartBitInByte;
        byteToCopy = (uint8) (srcBuffer[i] >> numRightShift);
        if(bitsToBeCopied > numLeftShift)
        {
          byteToCopy = byteToCopy | ((uint8) (srcBuffer[i + 1u] << numLeftShift));
        }
      }
    }
    else
    {
      byteToCopy = srcBuffer[i];
    }

    if(destBitOffset != 0u)     /* COV_SECOC_NO_MESSAGE_LINKER */
    {
      uint8 firstByteBitCount = destBitOffset;
      uint8 secondByteBitCount = 8u - destBitOffset;
      uint8 firstCopyByte;
      if(bitsToBeCopied >= firstByteBitCount)   /* COV_SECOC_NO_MESSAGE_LINKER */
      {
        firstCopyByte = byteToCopy;
        bitsToBeCopied = (uint16) (bitsToBeCopied - firstByteBitCount);
      }
      else
      {
        firstCopyByte = byteToCopy & (255u << (8u - bitsToBeCopied));
        bitsToBeCopied = 0u;
      }

      firstCopyByte = firstCopyByte >> secondByteBitCount;
      firstCopyByte = firstCopyByte | (destBuffer[i] & (255u << firstByteBitCount));
      destBuffer[i] = firstCopyByte;    /* SBSW_SECOC_PTR2FUNC_4 */

      if(bitsToBeCopied >= secondByteBitCount)  /* COV_SECOC_NO_MESSAGE_LINKER */
      {
        uint8 secondCopyByte = byteToCopy << firstByteBitCount;
        secondCopyByte = secondCopyByte | (destBuffer[i + 1u] & (255u >> secondByteBitCount));
        destBuffer[i + 1u] = secondCopyByte;    /* SBSW_SECOC_PTR2FUNC_4 */
        bitsToBeCopied = (uint16) (bitsToBeCopied - secondByteBitCount);
      }
    }
    else
    {
      if(bitsToBeCopied >= 8u)  /* COV_SECOC_ONLY_TX_AUTHINFO */
      {
        bitsToBeCopied = (uint16) (bitsToBeCopied - 8u);
      }
      else
      {
        uint8 rightShiftCount = (uint8) bitsToBeCopied;
        uint8 leftShiftCount = (uint8) (8u - bitsToBeCopied);
        byteToCopy = byteToCopy & (255u << leftShiftCount);
        byteToCopy = byteToCopy | (destBuffer[i] & (255u >> rightShiftCount));
        bitsToBeCopied = 0u;
      }
      destBuffer[i] = byteToCopy;       /* SBSW_SECOC_PTR2FUNC_4 */
    }
  }
}
#endif

#if (SECOC_TXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Authenticate_IncrementAndCheckBuildAttempts
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Authenticate_IncrementAndCheckBuildAttempts(SecOC_TxPduInfoIterType txPduId)  /* PRQA S 1505 */ /* MD_SecOC_1505 */
{
  SecOC_IncTxBuildAttemptsCounter(txPduId);     /* SBSW_SECOC_CSL02 */
  if(SecOC_GetTxBuildAttemptsCounter(txPduId) >= SecOC_GetBuildAttemptsOfTxPduInfo(txPduId))
  {
# if (SECOC_SENDDEFAULTAUTHENTICATION == STD_ON)
    if(SecOC_IsSendDefaultAuthentication(txPduId))
    {
      SecOC_SetTxPduState(txPduId, SECOC_DEVMODE_AUTHENTICATION_PATTERN_NEEDED_TXPDUSTATE);     /* SBSW_SECOC_CSL02 */
    }
    else
# endif
    {
      SecOC_SetTxPduState(txPduId, SECOC_INVALID_SECURED_PDU_TXPDUSTATE);       /* SBSW_SECOC_CSL02 */
      SecOC_SetTransmitRequest(txPduId, SECOC_IDLE_TRANSMITREQUEST);    /* SBSW_SECOC_CSL02 */
# if (SECOC_TXSPLITPDUINFO == STD_ON)
      if(SecOC_IsTxSplitPduInfoUsedOfTxPduInfo(txPduId))        /* COV_SECOC_SPLIT_ONLY_CONFIG */
      {
        SecOC_SizeOfTxSplitPduInfoType splitTxPduId = SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduId);
        SecOC_SetAuthenticSplitPduTransmit(splitTxPduId, SECOC_IDLE_AUTHENTICSPLITPDUTRANSMIT); /* SBSW_SECOC_CSL03 */
        SecOC_SetCryptographicPduTransmit(splitTxPduId, SECOC_IDLE_AUTHENTICSPLITPDUTRANSMIT);  /* SBSW_SECOC_CSL03 */
      }
# endif
# if(SECOC_EXISTS_TPPDU_PDUTYPEOFTXPDUINFO == STD_ON)
      if(SecOC_GetPduTypeOfTxPduInfo(txPduId) == SECOC_TPPDU_PDUTYPEOFTXPDUINFO)        /* COV_SECOC_TX_TP_UP_ONLY */
      {
        PduR_SecOCTpTxConfirmation(SecOC_GetAuthenticExternalIdOfTxPduInfo(txPduId), E_NOT_OK);
      }
# endif
      SecOC_ReportRuntimeDetError(SECOC_SID_MAIN_FUNCTION_TX, SECOC_E_CRYPTO_FAILURE);
    }
  }
}
#endif

#if (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Verify_IncrementBuildAttempts
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(void, SECOC_CODE) SecOC_Verify_IncrementBuildAttempts(SecOC_RxPduInfoIterType rxPduId)        /* PRQA S 1505 */ /* MD_SecOC_1505 */
{
  SecOC_IncRxBuildAttemptsCounter(rxPduId);     /* SBSW_SECOC_CSL02 */
  if(SecOC_GetRxBuildAttemptsCounter(rxPduId) >= SecOC_GetBuildAttemptsOfRxPduInfo(rxPduId))
  {
# if (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF)
    SecOC_Verify_PropagateVerificationStatus(rxPduId, SECOC_AUTHENTICATIONBUILDFAILURE);
    SecOC_Verify_PropagateVerificationStatusWithSecuredPdu(rxPduId, SECOC_AUTHENTICATIONBUILDFAILURE);
    SecOC_Verify_PropagateClientServerVerificationStatus(rxPduId, SECOC_AUTHENTICATIONBUILDFAILURE);
# endif
    SecOC_SetRxPduState(rxPduId, SECOC_ABORTED_RXPDUSTATE);     /* SBSW_SECOC_CSL02 */
  }
}
#endif

#if ((SECOC_RXSECUREDPDUINFO == STD_ON) && (SECOC_COMMUNICATION_INTERFACE == STD_ON))
/**********************************************************************************************************************
  SecOC_RxIndication_PDUProcessing
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030, 6010 1 */ /* MD_MSR_STCYC, MD_MSR_STPTH */
SECOC_LOCAL_INLINE FUNC(boolean, SECOC_CODE) SecOC_RxIndication_PDUProcessing(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info)
{
  SecOC_RxPduInfoIterType rxPduInfoIdx = SecOC_GetRxPduInfoIdxOfRxSecuredPduInfo(id);
  boolean wasProcessed = FALSE;
# if(SECOC_EXISTS_IMMEDIATE_PDUPROCESSINGOFRXPDUINFO)
  boolean doImmediateProcessing = FALSE;
# endif
  SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
  if((SecOC_GetRxPduState(rxPduInfoIdx) == SECOC_IDLE_RXPDUSTATE) || ((SecOC_GetRxPduState(rxPduInfoIdx) == SECOC_PRE_VERIFY_START_RXPDUSTATE)
# if (SECOC_PDUSWITHQUEUESIDXOFRXPDUINFO == STD_ON)
                                                                      && (!SecOC_IsPdusWithQueuesUsedOfRxPduInfo(rxPduInfoIdx))
# endif
     )) /* COV_SECOC_TP_ONLY */
  {
# if (SECOC_EXISTS_AUTHENTIC_SPLIT_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
    boolean isNewCompletMsg = TRUE;
# endif
    PduLengthType sduLength;
    uint8 headerOffset = 0u;
    wasProcessed = TRUE;
# if(SECOC_AUTHHEADERLENGTHOFRXSECUREDPDUINFO == STD_ON)
    if(SecOC_GetAuthHeaderLengthOfRxSecuredPduInfo(id) > 0u)
    {
      sduLength = SecOC_GetLengthFromHeader(info->SduDataPtr, SecOC_GetAuthHeaderLengthOfRxSecuredPduInfo(id)) + SecOC_GetPduFixLengthOfRxSecuredPduInfo(id);   /* SBSW_SECOC_PTR2FUNC_3 */
      headerOffset = SecOC_GetAuthHeaderLengthOfRxSecuredPduInfo(id);
      if((sduLength > SecOC_GetSecuredPduDataContainerLengthOfRxSecuredPduInfo(id)))
      {
        sduLength = (PduLengthType) SecOC_GetSecuredPduDataContainerLengthOfRxSecuredPduInfo(id);
      }
    }
    else
# endif
    {
      if((info->SduLength > SecOC_GetSecuredPduDataContainerLengthOfRxSecuredPduInfo(id)))
      {
        sduLength = (PduLengthType) SecOC_GetSecuredPduDataContainerLengthOfRxSecuredPduInfo(id);
      }
      else
      {
        sduLength = info->SduLength;
      }
    }
    switch (SecOC_GetMessageTypeOfRxSecuredPduInfo(id))
    {
# if(SECOC_EXISTS_NORMAL_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
      case SECOC_NORMAL_MESSAGETYPEOFRXSECUREDPDUINFO:
      {
        SecOC_SetRxAuthenticPduSizeOffset(rxPduInfoIdx, ((SecOC_RxAuthenticPduSizeOffsetType) (SecOC_GetSecuredPduDataContainerLengthOfRxSecuredPduInfo(id)) - (SecOC_RxAuthenticPduSizeOffsetType) sduLength));        /* SBSW_SECOC_CSL02 */
        VStdMemCpyRamToRam(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerStartIdxOfRxSecuredPduInfo(id) + SecOC_GetRxAuthenticPduSizeOffset(rxPduInfoIdx)), &(info->SduDataPtr[headerOffset]), sduLength);       /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_CPY_2RAM */
        break;
      }
# endif
# if (SECOC_EXISTS_AUTHENTIC_SPLIT_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
      case SECOC_AUTHENTIC_SPLIT_MESSAGETYPEOFRXSECUREDPDUINFO:
      {
        SecOC_SetRxAuthenticPduSizeOffset(rxPduInfoIdx, ((SecOC_RxAuthenticPduSizeOffsetType) (SecOC_GetSecuredPduDataContainerLengthOfRxSecuredPduInfo(id)) - (SecOC_RxAuthenticPduSizeOffsetType) sduLength));        /* SBSW_SECOC_CSL02 */
        SecOC_IncRxSplitMessageCounter(SecOC_GetRxSplitMessageCounterIdxOfRxSecuredPduInfo(id));        /* SBSW_SECOC_CSL03 */
        {
#  if (SECOC_ENABLE_FORCED_PASS_OVERRIDESTATUS == STD_ON)
          SecOC_VerifyOverrideStatusType verifyOverrideStatus = SecOC_GetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduInfoIdx));
          if((verifyOverrideStatus == SECOC_OVERRIDE_SKIP_UNTIL_LIMIT_VERIFYOVERRIDESTATUS) || (verifyOverrideStatus == SECOC_OVERRIDE_SKIP_UNTIL_NOTICE_VERIFYOVERRIDESTATUS))
          {
            isNewCompletMsg = TRUE;
          }
          else
#  endif
          {
            if(SecOC_GetRxSplitMessageCounter(SecOC_GetRxSplitMessageCounterIdxOfRxSecuredPduInfo(id)) < 2u)
            {
              isNewCompletMsg = FALSE;
            }
#  if (SECOC_CURRENTMESSAGELINKER == STD_ON)
            if(SecOC_IsCurrentMessageLinkerUsedOfRxSecuredPduInfo(id)) /* COV_SECOC_ALLWAYS_MESSAGE_LINKER */
            {
              uint8 messageLinker[SECOC_MESSAGE_LINKER_SIZE_RXPDUS] = { 0u };
              uint8 *authenticPDUStart = &(info->SduDataPtr[headerOffset]);     /* PRQA S 3678 */ /* MD_MSR_Rule8.13 */
              SecOC_GetArrayByteAligned(&(authenticPDUStart[((PduLengthType) (SecOC_GetMessageLinkPosOfRxSecuredPduInfo(id) >> 3u))]), messageLinker, (uint16) SecOC_GetMessageLinkLenOfRxSecuredPduInfo(id), (uint8) ((8u - ((SecOC_GetMessageLinkPosOfRxSecuredPduInfo(id) + 1u) & 7u)) & 7u), 0u, TRUE);     /* SBSW_SECOC_PTR2FUNC_6 */
              isNewCompletMsg = SecOC_Split_CheckMessageLinker(messageLinker, SecOC_GetAddrCurrentMessageLinker(SecOC_GetCurrentMessageLinkerStartIdxOfRxSecuredPduInfo(id)), SecOC_GetCurrentMessageLinkerLengthOfRxSecuredPduInfo(id), isNewCompletMsg);      /* SBSW_SECOC_PTR2FUNC_5 */
            }
#  endif
          }
        }
        VStdMemCpyRamToRam(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerStartIdxOfRxSecuredPduInfo(id) + SecOC_GetRxAuthenticPduSizeOffset(rxPduInfoIdx)), &(info->SduDataPtr[headerOffset]), sduLength);       /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_CPY_2RAM */
        break;
      }
# endif
# if (SECOC_EXISTS_CRYPTOGRAPHIC_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
      case SECOC_CRYPTOGRAPHIC_MESSAGETYPEOFRXSECUREDPDUINFO:
      {
        SecOC_IncRxSplitMessageCounter(SecOC_GetRxSplitMessageCounterIdxOfRxSecuredPduInfo(id));        /* SBSW_SECOC_CSL03 */
        if(SecOC_GetRxSplitMessageCounter(SecOC_GetRxSplitMessageCounterIdxOfRxSecuredPduInfo(id)) < 2u)
        {
          isNewCompletMsg = FALSE;
        }
#  if (SECOC_CURRENTMESSAGELINKER == STD_ON)
        if(SecOC_IsCurrentMessageLinkerUsedOfRxSecuredPduInfo(id)) /* COV_SECOC_ALLWAYS_MESSAGE_LINKER */
        {
          uint8 messageLinker[SECOC_MESSAGE_LINKER_SIZE_RXPDUS] = { 0u };
          SecOC_GetArrayByteAligned(&(info->SduDataPtr[SecOC_GetMessageLinkerStartByteInCryptoPduOfRxSecuredPduInfo(id)]), messageLinker, (uint16) SecOC_GetMessageLinkLenOfRxSecuredPduInfo(id), SecOC_GetMessagesLinkCryptoPduSrcBitOffsetOfRxSecuredPduInfo(id), 0u, TRUE);  /* SBSW_SECOC_PTR2FUNC_6 */
          isNewCompletMsg = SecOC_Split_CheckMessageLinker(messageLinker, SecOC_GetAddrCurrentMessageLinker(SecOC_GetCurrentMessageLinkerStartIdxOfRxSecuredPduInfo(id)), SecOC_GetCurrentMessageLinkerLengthOfRxSecuredPduInfo(id), isNewCompletMsg);  /* SBSW_SECOC_PTR2FUNC_7 */
        }
#  endif
        VStdMemCpyRamToRam(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerStartIdxOfRxSecuredPduInfo(id)), info->SduDataPtr, SecOC_GetSecuredPduDataContainerLengthOfRxSecuredPduInfo(id));       /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_CPY_2RAM */
        break;
      }
# endif
      default: /* COV_SECOC_MISRA */
        break;
    }
# if (SECOC_EXISTS_AUTHENTIC_SPLIT_MESSAGETYPEOFRXSECUREDPDUINFO == STD_ON)
    if(isNewCompletMsg == TRUE)
# endif
    {
      /* --- Set the state of the corresponding PDU to Unverified --- */
      SecOC_SetVerifyAttemptsCounter(rxPduInfoIdx, 0);  /* SBSW_SECOC_CSL80 */
      SecOC_SetRxBuildAttemptsCounter(rxPduInfoIdx, 0); /* SBSW_SECOC_CSL80 */
# if(SECOC_EXISTS_IMMEDIATE_PDUPROCESSINGOFRXPDUINFO)
      if(SecOC_GetPduProcessingOfRxPduInfo(rxPduInfoIdx) == SECOC_IMMEDIATE_PDUPROCESSINGOFRXPDUINFO)
      {
        SecOC_SetRxImmediateProcessingOngoing(rxPduInfoIdx, TRUE);      /* SBSW_SECOC_CSL02 */
        SecOC_SetRxPduState(rxPduInfoIdx, SECOC_PRE_VERIFY_START_RXPDUSTATE);   /* SBSW_SECOC_CSL80 */
        doImmediateProcessing = TRUE;
      }
      else
# endif
      {
        SecOC_SetRxPduState(rxPduInfoIdx, SECOC_PRE_VERIFY_START_RXPDUSTATE);   /* SBSW_SECOC_CSL80 */
      }
    }
  }
  SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
# if(SECOC_EXISTS_IMMEDIATE_PDUPROCESSINGOFRXPDUINFO)
  if(doImmediateProcessing == TRUE)
  {
    SecOC_Verify_MAC(rxPduInfoIdx);
    SecOC_SetRxImmediateProcessingOngoing(rxPduInfoIdx, FALSE); /* SBSW_SECOC_CSL02 */
  }
# endif
  return wasProcessed;
}
#endif

#if (SECOC_PDUSWITHQUEUESIDXOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Queue_IsQueueEmpty
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(boolean, SECOC_CODE) SecOC_Queue_IsQueueEmpty(SecOC_PdusWithQueuesIdxOfRxPduInfoType pdusWithQueuesIdx)
{
  boolean isQueueEmpty = FALSE;
  if(SecOC_GetWriteIdxQueueElement(pdusWithQueuesIdx) == SecOC_GetReadIdxQueueElement(pdusWithQueuesIdx))
  {
    SecOC_WriteIdxQueueElementType writeQueueElementInfoIdx = SecOC_GetWriteIdxQueueElement(pdusWithQueuesIdx) + ((SecOC_WriteIdxQueueElementType) SecOC_GetQueueElementInfoStartIdxOfPdusWithQueues(pdusWithQueuesIdx));
    if(SecOC_GetQueuedElementLength(writeQueueElementInfoIdx) == 0u)
    {
      isQueueEmpty = TRUE;
    }
  }
  return isQueueEmpty;
}
#endif

#if (SECOC_PDUSWITHQUEUESIDXOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Queue_AddPDUToQueue
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(boolean, SECOC_CODE) SecOC_Queue_AddPDUToQueue(SecOC_RxPduInfoIterType rxPduInfoIdx, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info)
{

  boolean wasAddedToQueue = FALSE;
  if(SecOC_IsPdusWithQueuesUsedOfRxPduInfo(rxPduInfoIdx))
  {
    SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
    {
      /* queue element */
      SecOC_PdusWithQueuesIdxOfRxPduInfoType pdusWithQueuesIdx = SecOC_GetPdusWithQueuesIdxOfRxPduInfo(rxPduInfoIdx);
      SecOC_WriteIdxQueueElementType writeQueueElementInfoIdx = SecOC_GetWriteIdxQueueElement(pdusWithQueuesIdx) + ((SecOC_WriteIdxQueueElementType) SecOC_GetQueueElementInfoStartIdxOfPdusWithQueues(pdusWithQueuesIdx));
      if(writeQueueElementInfoIdx < SecOC_GetQueueElementInfoEndIdxOfPdusWithQueues(pdusWithQueuesIdx)) /* COV_SECOC_SAFEBSW_CHECK */
      {
        if(SecOC_GetQueuedElementLength(writeQueueElementInfoIdx) == 0u)
        {
          /* next queue element is empty */
          PduLengthType sduLength;
          if((info->SduLength > SecOC_GetQueueArrayLengthOfQueueElementInfo(writeQueueElementInfoIdx)))
          {
            sduLength = (PduLengthType) SecOC_GetQueueArrayLengthOfQueueElementInfo(writeQueueElementInfoIdx);
          }
          else
          {
            sduLength = info->SduLength;
          }
          /* insert To Queue */
          VStdMemCpyRamToRam(SecOC_GetAddrQueueArray(SecOC_GetQueueArrayStartIdxOfQueueElementInfo(writeQueueElementInfoIdx)), info->SduDataPtr, sduLength);    /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_CPY_2RAM_PARAMCKECK */
          SecOC_SetQueuedElementLength(writeQueueElementInfoIdx, sduLength);    /* SBSW_SECOC_CALCULATED_INDEX */
          SecOC_SetWriteIdxQueueElement(pdusWithQueuesIdx, ((SecOC_GetWriteIdxQueueElement(pdusWithQueuesIdx) + 1u) % ((SecOC_WriteIdxQueueElementType) SecOC_GetQueueElementInfoLengthOfPdusWithQueues(pdusWithQueuesIdx))));     /* SBSW_SECOC_CSL02 */
          wasAddedToQueue = TRUE;
        }
      }
    }
    SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
  }
  return wasAddedToQueue;
}
#endif

#if (SECOC_PDUSWITHQUEUESIDXOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_Queue_LoadNextQueueElement
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
SECOC_LOCAL_INLINE FUNC(boolean, SECOC_CODE) SecOC_Queue_LoadNextQueueElement(SecOC_RxPduInfoIterType rxPduInfoIdx)
{
  boolean nexQueueElementLoaded = FALSE;
  if(SecOC_IsPdusWithQueuesUsedOfRxPduInfo(rxPduInfoIdx))
  {
    SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
    if(SecOC_GetRxPduState(rxPduInfoIdx) == SECOC_IDLE_RXPDUSTATE)
    {
      SecOC_PdusWithQueuesIdxOfRxPduInfoType pdusWithQueuesIdx = SecOC_GetPdusWithQueuesIdxOfRxPduInfo(rxPduInfoIdx);
      if(SecOC_Queue_IsQueueEmpty(pdusWithQueuesIdx) == FALSE)
      {
        /* load next queue element */
        SecOC_ReadIdxQueueElementType readQueueElementInfoIdx = SecOC_GetReadIdxQueueElement(pdusWithQueuesIdx) + ((SecOC_ReadIdxQueueElementType) SecOC_GetQueueElementInfoStartIdxOfPdusWithQueues(pdusWithQueuesIdx));
        if(readQueueElementInfoIdx < SecOC_GetQueueElementInfoEndIdxOfPdusWithQueues(pdusWithQueuesIdx)) /* COV_SECOC_SAFEBSW_CHECK */
        {
          PduInfoType info;
          info.SduDataPtr = SecOC_GetAddrQueueArray(SecOC_GetQueueArrayStartIdxOfQueueElementInfo(readQueueElementInfoIdx));
          info.SduLength = SecOC_GetQueuedElementLength(readQueueElementInfoIdx);
          (void) SecOC_RxIndication_PDUProcessing((PduIdType) SecOC_GetRxSecuredPduInfoIdxOfRxPduInfo(rxPduInfoIdx), &info);    /* SBSW_SECOC_PTR2FUNC_6 */

          SecOC_SetQueuedElementLength(readQueueElementInfoIdx, 0u);    /* SBSW_SECOC_CALCULATED_INDEX */
          SecOC_SetReadIdxQueueElement(pdusWithQueuesIdx, ((SecOC_GetReadIdxQueueElement(pdusWithQueuesIdx) + 1u) % ((SecOC_ReadIdxQueueElementType) SecOC_GetQueueElementInfoLengthOfPdusWithQueues(pdusWithQueuesIdx))));       /* SBSW_SECOC_CSL02 */

          nexQueueElementLoaded = TRUE;
        }
      }
    }
    SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
  }
  return nexQueueElementLoaded;
}
#endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/* Internal functions */
#if (SECOC_WAITFORCALLBACKOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_RxCsmCallback
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_RxCsmCallback(PduIdType rxHandleId, Csm_ResultType result)
{
  SecOC_RxPduInfoIterType rxPduInfoIdx = SecOC_GetRxPduInfoIdxOfRxSecuredPduInfo(rxHandleId);
  SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
  if(SecOC_GetRxPduState(rxPduInfoIdx) == SECOC_UNVERIFIED_VERIFICATION_FINISHED_WAIT_FOR_CALLBACK_RXPDUSTATE)
  {
# if (SECOC_CSMJOBTIMEOUTCNTOFRXPDUINFO == STD_ON)
    SecOC_SetRxCSMJobTimeoutCntValue(rxPduInfoIdx, 0);  /* SBSW_SECOC_CSL02 */
# endif
    if(result == E_OK)
    {
# if (SECOC_IMMEDIATECBKPROCESSINGOFRXPDUINFO == STD_ON)
      if(SecOC_IsImmediateCbkProcessingOfRxPduInfo(rxPduInfoIdx))
      {
        if(SecOC_GetVerifyResult(SecOC_GetVerifyResultIdxOfRxPduInfo(rxPduInfoIdx)) == CRYPTO_E_VER_OK)
        {
          SecOC_SetRxImmediateProcessingOngoing(rxPduInfoIdx, TRUE);    /* SBSW_SECOC_CSL02 */
          SecOC_SetRxPduState(rxPduInfoIdx, SECOC_UNVERIFIED_VERIFICATION_FINISHED_VERIFICATION_RESULT_READY_RXPDUSTATE);       /* SBSW_SECOC_CSL80_GENERATED_HANDLES */
          SecOC_Verify_MAC(rxPduInfoIdx);
          SecOC_SetRxImmediateProcessingOngoing(rxPduInfoIdx, FALSE);   /* SBSW_SECOC_CSL02 */
        }
        else
        {
          SecOC_SetRxPduState(rxPduInfoIdx, SECOC_UNVERIFIED_VERIFICATION_FINISHED_VERIFICATION_RESULT_READY_RXPDUSTATE);       /* SBSW_SECOC_CSL80_GENERATED_HANDLES */
        }
      }
      else
# endif
      {
        SecOC_SetRxPduState(rxPduInfoIdx, SECOC_UNVERIFIED_VERIFICATION_FINISHED_VERIFICATION_RESULT_READY_RXPDUSTATE); /* SBSW_SECOC_CSL80_GENERATED_HANDLES */
      }
    }
    else
    {
      SecOC_SetRxPduState(rxPduInfoIdx, SECOC_ABORTED_RXPDUSTATE);      /* SBSW_SECOC_CSL80_GENERATED_HANDLES */
    }
    SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
# if ((SECOC_EXISTS_TPPDU_PDUTYPEOFRXPDUINFO == STD_ON) || (SECOC_IMMEDIATECBKPROCESSINGOFRXPDUINFO == STD_ON))
    if(SecOC_GetRxPduState(rxPduInfoIdx) == SECOC_ABORTED_RXPDUSTATE)
    {
#  if (SECOC_IMMEDIATECBKPROCESSINGOFRXPDUINFO == STD_ON)
      if(SecOC_IsImmediateCbkProcessingOfRxPduInfo(rxPduInfoIdx))
      {
        SecOC_SetRxImmediateProcessingOngoing(rxPduInfoIdx, TRUE);      /* SBSW_SECOC_CSL02 */
        SecOC_Verify_MAC(rxPduInfoIdx);
        SecOC_SetRxImmediateProcessingOngoing(rxPduInfoIdx, FALSE);     /* SBSW_SECOC_CSL02 */
      }
#  endif
#  if (SECOC_EXISTS_TPPDU_PDUTYPEOFRXPDUINFO == STD_ON)
#   if(SECOC_DEVMODEENABLED == STD_ON)
      if(!SecOC_IsDevModeEnabled())
#   endif
      {
        if(SecOC_GetPduTypeOfRxPduInfo(rxPduInfoIdx) == SECOC_TPPDU_PDUTYPEOFRXPDUINFO) /* COV_SECOC_RX_TP_UP_ONLY */
        {
          PduR_SecOCTpRxIndication(SecOC_GetAuthenticExternalIdOfRxPduInfo(rxPduInfoIdx), E_NOT_OK);
        }
      }
#  endif
    }
# endif
  }
  else
  {
    SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
  }
}
#endif

#if (SECOC_WAITFORCALLBACKOFTXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_TxCsmCallback
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, SECOC_CODE) SecOC_TxCsmCallback(PduIdType txHandleId, Csm_ResultType result)
{
  SecOC_TxPduInfoIterType txPduInfoIdx = SecOC_GetTxPduInfoIdxOfTxAuthenticPduInfo(txHandleId);
  SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();
  if(SecOC_GetTxPduState(txPduInfoIdx) == SECOC_UNAUTHENTICATED_FINISHED_WAIT_FOR_CALLBACK_TXPDUSTATE)
  {
    if(result == E_OK)
    {
# if (SECOC_IMMEDIATECBKPROCESSINGOFTXPDUINFO == STD_ON)
      if(SecOC_IsImmediateCbkProcessingOfTxPduInfo(txPduInfoIdx))
      {
        SecOC_SetTxImmediateProcessingOngoing(txPduInfoIdx, TRUE);      /* SBSW_SECOC_CSL02 */
      }
# endif
# if (SECOC_CSMJOBTIMEOUTCNTOFTXPDUINFO == STD_ON)
      SecOC_SetTxCSMJobTimeoutCntValue(txPduInfoIdx, 0);        /* SBSW_SECOC_CSL02 */
# endif
# if (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF)
      SecOC_GetArrayByteAligned(SecOC_GetAddrAuthenticatorCache(SecOC_GetAuthenticatorCacheStartIdxOfTxPduInfo(txPduInfoIdx)), SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerTruncatedAuthenticatorStartIdxOfTxPduInfo(txPduInfoIdx)), (uint16) SecOC_GettruncatedAuthenticationLengthOfTxPduInfo(txPduInfoIdx), 0u, (uint8) ((8u - SecOC_GetTruncAuthStartBitInByteOfTxPduInfo(txPduInfoIdx)) & 7u), FALSE);    /* SBSW_SECOC_PTR2FUNC_6 */
# endif
      SecOC_SetTxPduState(txPduInfoIdx, SECOC_UNAUTHENTICATED_FINISHED_TXPDUSTATE);     /* SBSW_SECOC_CSL80_GENERATED_HANDLES */
# if (SECOC_IMMEDIATECBKPROCESSINGOFTXPDUINFO == STD_ON)
      if(SecOC_IsImmediateCbkProcessingOfTxPduInfo(txPduInfoIdx))
      {
        (void) SecOC_TxIf_ProcessTxPdu(txPduInfoIdx);
        SecOC_SetTxImmediateProcessingOngoing(txPduInfoIdx, FALSE);     /* SBSW_SECOC_CSL02 */
      }
# endif
    }
    else
    {
# if (SECOC_DEVMODEENABLED == STD_ON)
      if(SecOC_IsDevModeEnabled())
      {
        SecOC_SetTxPduState(txPduInfoIdx, SECOC_DEVMODE_AUTHENTICATION_PATTERN_NEEDED_TXPDUSTATE);      /* SBSW_SECOC_CSL02 */
#  if (SECOC_IMMEDIATECBKPROCESSINGOFTXPDUINFO == STD_ON)
        if(SecOC_IsImmediateCbkProcessingOfTxPduInfo(txPduInfoIdx))
        {
          SecOC_SetTxImmediateProcessingOngoing(txPduInfoIdx, TRUE);    /* SBSW_SECOC_CSL02 */
          (void) SecOC_TxIf_ProcessTxPdu(txPduInfoIdx);
          SecOC_SetTxImmediateProcessingOngoing(txPduInfoIdx, FALSE);   /* SBSW_SECOC_CSL02 */
        }
#  endif
      }
      else
# endif
# if (SECOC_SENDDEFAULTAUTHENTICATION == STD_ON)
      if(SecOC_IsSendDefaultAuthentication(txPduInfoIdx))
      {
        SecOC_SetTxPduState(txPduInfoIdx, SECOC_DEVMODE_AUTHENTICATION_PATTERN_NEEDED_TXPDUSTATE);      /* SBSW_SECOC_CSL02 */
#  if (SECOC_IMMEDIATECBKPROCESSINGOFTXPDUINFO == STD_ON)
        if(SecOC_IsImmediateCbkProcessingOfTxPduInfo(txPduInfoIdx))
        {
          SecOC_SetTxImmediateProcessingOngoing(txPduInfoIdx, TRUE);    /* SBSW_SECOC_CSL02 */
          (void) SecOC_TxIf_ProcessTxPdu(txPduInfoIdx);
          SecOC_SetTxImmediateProcessingOngoing(txPduInfoIdx, FALSE);   /* SBSW_SECOC_CSL02 */
        }
#  endif
      }
      else
# endif
      {
        SecOC_SetTxPduState(txPduInfoIdx, SECOC_INVALID_SECURED_PDU_TXPDUSTATE);        /* SBSW_SECOC_CSL80_GENERATED_HANDLES */
        SecOC_SetTransmitRequest(txPduInfoIdx, SECOC_IDLE_TRANSMITREQUEST);     /* SBSW_SECOC_CSL80_GENERATED_HANDLES */
# if(SECOC_EXISTS_TPPDU_PDUTYPEOFTXPDUINFO == STD_ON)
        if(SecOC_GetPduTypeOfTxPduInfo(txPduInfoIdx) == SECOC_TPPDU_PDUTYPEOFTXPDUINFO) /* COV_SECOC_TX_TP_UP_ONLY */
        {
          PduR_SecOCTpTxConfirmation(SecOC_GetAuthenticExternalIdOfTxPduInfo(txPduInfoIdx), E_NOT_OK);
        }
# endif
      }
    }
  }
  SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();
}

#endif

/* Service functions */

/**********************************************************************************************************************
  SecOC_InitMemory
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_InitMemory(void)
{
  /* ----- Implementation ----------------------------------------------- */
  SecOC_SetInitialized(FALSE);  /* SBSW_SECOC_CSL_VAR_ACCESS */
#if(SECOC_USE_INIT_POINTER == STD_ON)
  SecOC_ConfigDataPtr = NULL_PTR;
#endif
}

/**********************************************************************************************************************
  SecOC_DeInit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_DeInit(void)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if component is initialized */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    SecOC_SetInitialized(FALSE);        /* SBSW_SECOC_CSL_VAR_ACCESS */
#if(SECOC_USE_INIT_POINTER == STD_ON)
    SecOC_ConfigDataPtr = NULL_PTR;
#endif
  }

#if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_DE_INIT, errorId);
  }
#endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */
}

/**********************************************************************************************************************
  SecOC_Init
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_Init(P2CONST(SecOC_ConfigType, AUTOMATIC, SECOC_PBCFG) config)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;
#if(SECOC_USE_INIT_POINTER == STD_ON)
  SecOC_ConfigDataPtr = config;
#endif

  /* ----- Development Error Checks ------------------------------------- */
  if(SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
  /* --- parameter check --- */
#if(SECOC_USE_INIT_POINTER == STD_ON)
  /* Check validtity of Config pointer */
  else if(SecOC_ConfigDataPtr == NULL_PTR)
  {
# if(SECOC_USE_ECUM_BSW_ERROR_HOOK == STD_ON)   /* COV_SECOC_VAR_DEPENDENT_SWITCH */
    EcuM_BswErrorHook(SECOC_MODULE_ID, ECUM_BSWERROR_NULLPTR);
# endif
    errorId = SECOC_E_PARAM_POINTER;
  }
# if (SECOC_FINALMAGICNUMBER == STD_ON) /* COV_SECOC_VAR_DEPENDENT_SWITCH */
  else if(SecOC_GetFinalMagicNumber() != SECOC_FINAL_MAGIC_NUMBER)
  {
#  if(SECOC_USE_ECUM_BSW_ERROR_HOOK == STD_ON)  /* COV_SECOC_VAR_DEPENDENT_SWITCH */
    EcuM_BswErrorHook(SECOC_MODULE_ID, ECUM_BSWERROR_MAGICNUMBER);
#  endif
    errorId = SECOC_E_PARAM_POINTER;
    SecOC_ConfigDataPtr = NULL_PTR;
  }
# endif
#else
  if(config != NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
#endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#if (SECOC_RXPDUINFO == STD_ON)
    SecOC_RxIf_InitPdus();
#endif
#if(SECOC_TXPDUINFO == STD_ON)
    SecOC_TxIf_InitPdus();
#endif
#if (SECOC_RXPDUSTATE == STD_ON)
    {
      SecOC_RxPduStateIterType pduStateIdx = 0;
      for(; pduStateIdx < SecOC_GetSizeOfRxPduState(); pduStateIdx++)
      {
        SecOC_SetRxPduState(pduStateIdx, SECOC_IDLE_RXPDUSTATE);        /* SBSW_SECOC_CSL01 */
      }
    }
#endif
#if (SECOC_TXPDUSTATE == STD_ON)
    {
      SecOC_TxPduStateIterType pduStateIdx = 0;
      for(; pduStateIdx < SecOC_GetSizeOfTxPduState(); pduStateIdx++)
      {
        SecOC_SetTxPduState(pduStateIdx, SECOC_INVALID_SECURED_PDU_TXPDUSTATE); /* SBSW_SECOC_CSL01 */
      }
    }
#endif
#if (SECOC_SENDDEFAULTAUTHENTICATION == STD_ON)
    {
      SecOC_SendDefaultAuthenticationIterType sendDefaultAuthenticationIdx = 0;
      for(; sendDefaultAuthenticationIdx < SecOC_GetSizeOfSendDefaultAuthentication(); sendDefaultAuthenticationIdx++)
      {
        SecOC_SetSendDefaultAuthentication(sendDefaultAuthenticationIdx, FALSE);        /* SBSW_SECOC_CSL01 */
      }
    }
#endif

#if (SECOC_VERIFYRESULT == STD_ON)
    {
      SecOC_VerifyResultIterType verifyResultIdx = 0;
      for(; verifyResultIdx < SecOC_GetSizeOfVerifyResult(); verifyResultIdx++)
      {
        SecOC_SetVerifyResult(verifyResultIdx, CSM_E_VER_NOT_OK);       /* SBSW_SECOC_CSL01 */
      }
    }
#endif

#if (SECOC_PDUSWITHQUEUESIDXOFRXPDUINFO == STD_ON)
    VStdLib_MemClr(SecOC_GetAddrQueuedElementLength(0u), SecOC_GetSizeOfQueuedElementLength()); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_SET */
    VStdLib_MemClr(SecOC_GetAddrReadIdxQueueElement(0u), SecOC_GetSizeOfReadIdxQueueElement()); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_SET */
    VStdLib_MemClr(SecOC_GetAddrWriteIdxQueueElement(0u), SecOC_GetSizeOfWriteIdxQueueElement());       /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_SET */
#endif

#if(SECOC_DEVMODEENABLED == STD_ON)
    SecOC_SetDevModeEnabled(FALSE);     /* SBSW_SECOC_CSL_VAR_ACCESS */
#endif
    SecOC_SetInitialized(TRUE); /* SBSW_SECOC_CSL_VAR_ACCESS */
  }

#if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_INIT, errorId);
  }
#endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

#if (SECOC_VERSION_INFO_API == STD_ON)
/**********************************************************************************************************************
  SecOC_GetVersionInfo
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, SECOC_APPL_DATA) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if(SECOC_DEV_ERROR_DETECT == STD_ON)  /* COV_SECOC_VAR_DEV_ERROR_DETECT */
  if(versioninfo == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    versioninfo->vendorID = (SECOC_VENDOR_ID);  /* SBSW_SECOC_PTR2FUNC_3 */
    versioninfo->moduleID = (SECOC_MODULE_ID);  /* SBSW_SECOC_PTR2FUNC_3 */
    versioninfo->sw_major_version = (SECOC_SW_MAJOR_VERSION);   /* SBSW_SECOC_PTR2FUNC_3 */
    versioninfo->sw_minor_version = (SECOC_SW_MINOR_VERSION);   /* SBSW_SECOC_PTR2FUNC_3 */
    versioninfo->sw_patch_version = (SECOC_SW_PATCH_VERSION);   /* SBSW_SECOC_PTR2FUNC_3 */
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_GET_VERSION_INFO, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}
#endif

/**********************************************************************************************************************
  SecOC_Transmit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6030, 6080, 6010, 6050 1 */ /* MD_MSR_STCYC, MD_MSR_STMIF, MD_MSR_STPTH, MD_MSR_STCAL */
FUNC(Std_ReturnType, SECOC_CODE) SecOC_Transmit(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType ret = E_NOT_OK;        /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
#if(SECOC_DEV_ERROR_DETECT == STD_ON)   /* COV_SECOC_VAR_DEV_ERROR_DETECT */
# if(SECOC_TXAUTHENTICPDUINFO == STD_ON)
  else if(id >= SecOC_GetSizeOfTxAuthenticPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#  if(SECOC_INVALIDHNDOFTXAUTHENTICPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfTxAuthenticPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#  endif
  else if(info == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if((info->SduDataPtr == NULL_PTR) && (info->SduLength > 0u))
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
# endif
# if(SECOC_AUTHENTICPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFTXPDUINFO == STD_ON)
  else if(info->SduLength > SecOC_GetAuthenticPduDataContainerFullAuthenticPduLengthOfTxPduInfo(SecOC_GetTxPduInfoIdxOfTxAuthenticPduInfo(id)))
  {
    errorId = SECOC_E_PARAM;
  }
  else if(info->SduLength < SecOC_GetMinimumPduLengthOfTxAuthenticPduInfo(id))  /* COV_SECOC_NO_META_DATA */
  {
    errorId = SECOC_E_PARAM;
  }
# else
  else if(info->SduLength > 0u)
  {
    errorId = SECOC_E_PARAM;
  }
# endif
#endif
#if ((SECOC_TXSECUREDPDUAREAINFOUSEDOFTXPDUINFO == STD_ON) && (SECOC_CUSTOM_AUTHENTICATION_API == STD_OFF))
  else if((SecOC_IsTxSecuredPduAreaInfoUsedOfTxPduInfo(SecOC_GetTxPduInfoIdxOfTxAuthenticPduInfo(id))) && (info->SduLength < (SecOC_GetLengthOfTxSecuredPduAreaInfo(SecOC_GetTxSecuredPduAreaInfoIdxOfTxPduInfo(SecOC_GetTxPduInfoIdxOfTxAuthenticPduInfo(id))) + SecOC_GetOffsetOfTxSecuredPduAreaInfo(SecOC_GetTxSecuredPduAreaInfoIdxOfTxPduInfo(SecOC_GetTxPduInfoIdxOfTxAuthenticPduInfo(id))))))
  {
    errorId = SECOC_E_PARAM;    /* \trace SWS_SecOC_00313 */
  }
#endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#if(SECOC_TXAUTHENTICPDUINFO == STD_ON)
    SecOC_TxPduInfoIterType txPduInfoIdx = SecOC_GetTxPduInfoIdxOfTxAuthenticPduInfo(id);
# if(SECOC_EXISTS_IMMEDIATE_PDUPROCESSINGOFTXPDUINFO)
    boolean isImmediateProcessingRequired = FALSE;
# endif
    /* --- Reevaluate the Pdu state --- */
    SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();
# if(SECOC_EXISTS_IFPDU_LOWERLAYERPDUTYPEOFTXPDUINFO ==  STD_ON)
    if(SecOC_GetLowerLayerPduTypeOfTxPduInfo(txPduInfoIdx) == SECOC_IFPDU_LOWERLAYERPDUTYPEOFTXPDUINFO) /* COV_SECOC_TX_IF_LOW_ONLY */
    {
      if((SecOC_GetTxPduState(txPduInfoIdx) == SECOC_TRANSMISSION_INITIATED_TXPDUSTATE) || ((SecOC_GetTxPduState(txPduInfoIdx) == SECOC_AUTHENTICATED_TXPDUSTATE) && (SecOC_GetTransmitRequest(txPduInfoIdx) == SECOC_IDLE_TRANSMITREQUEST)))   /* COV_SECOC_TX_STATE_NOT_REACHABLE */
      {
        SecOC_SetTxPduState(txPduInfoIdx, SECOC_INVALID_SECURED_PDU_TXPDUSTATE);        /* SBSW_SECOC_CSL80 */
      }
    }
# endif
    if((SecOC_GetTxPduState(txPduInfoIdx) == SECOC_INVALID_SECURED_PDU_TXPDUSTATE) || (SecOC_GetTxPduState(txPduInfoIdx) == SECOC_PRE_REAUTHENTICATION_NEEDED_TXPDUSTATE))      /* COV_SECOC_SYNC_CSM_ONLY_CONFIG */
    {
      switch (SecOC_GetPduTypeOfTxPduInfo(txPduInfoIdx))
      {
# if (SECOC_EXISTS_IFPDU_PDUTYPEOFTXPDUINFO == STD_ON)
        case SECOC_IFPDU_PDUTYPEOFTXPDUINFO:
        {
#  if(SECOC_EXISTS_IMMEDIATE_PDUPROCESSINGOFTXPDUINFO)
          if(SecOC_GetPduProcessingOfTxPduInfo(txPduInfoIdx) == SECOC_IMMEDIATE_PDUPROCESSINGOFTXPDUINFO)
          {
            isImmediateProcessingRequired = TRUE;
            SecOC_SetTxImmediateProcessingOngoing(txPduInfoIdx, TRUE);  /* SBSW_SECOC_CSL02 */
          }
#  endif
#  if(SECOC_AUTHENTICPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFTXPDUINFO == STD_ON)
          SecOC_SetTxAuthenticPduSizeOffset(txPduInfoIdx, ((SecOC_TxAuthenticPduSizeOffsetType) (SecOC_GetAuthenticPduDataContainerFullAuthenticPduLengthOfTxPduInfo(txPduInfoIdx)) - (SecOC_TxAuthenticPduSizeOffsetType) (info->SduLength))); /* SBSW_SECOC_CSL02 */
#  else
          SecOC_SetTxAuthenticPduSizeOffset(txPduInfoIdx, 0);   /* SBSW_SECOC_CSL02 */
#  endif
          /* --- Copy the Authentic Pdu into an internal Buffer --- */
#  if (SECOC_AUTHENTICPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFTXPDUINFO ==  STD_ON)
          if(info->SduLength > 0u)
          {
            VStdMemCpyRamToRam(SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetStartIdxOfTxAuthenticPduDataContainerData(txPduInfoIdx)), info->SduDataPtr, info->SduLength);    /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_CPY_2RAM */
#   if (SECOC_SECUREDPDUDATACONTAINERSECUREDPDUMETADATAUSEDOFTXPDUINFO == STD_ON)
            if(SecOC_IsSecuredPduDataContainerSecuredPDUMetaDataUsedOfTxPduInfo(txPduInfoIdx))
            {
              VStdMemCpyRamToRam(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerSecuredPDUMetaDataStartIdxOfTxPduInfo(txPduInfoIdx)), SecOC_GetAddrAuthenticPduDataContainer(SecOC_GetAuthenticPduDataContainerAuthenticPDUMetaDataStartIdxOfTxPduInfo(txPduInfoIdx)), SecOC_GetSecuredPduDataContainerSecuredPDUMetaDataLengthOfTxPduInfo(txPduInfoIdx));        /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_CPY_2RAM */
            }
#   endif
          }
#  endif
#  if (SECOC_SECUREDPDUDATACONTAINERAUTHPDUHEADERLENGTHUSEDOFTXPDUINFO == STD_ON)
          if(SecOC_IsSecuredPduDataContainerAuthPduHeaderLengthUsedOfTxPduInfo(txPduInfoIdx))
          {
            SecOC_SerializeLengthToHeader(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetStartIdxOfTxAuthPduHeader(txPduInfoIdx)), (uint8) SecOC_GetSecuredPduDataContainerAuthPduHeaderLengthLengthOfTxPduInfo(txPduInfoIdx), info->SduLength); /* SBSW_SECOC_PTR2FUNC_3 */
          }
#  endif
          /* Reset Tx Sync Attempts Counter */
          SecOC_SetTxBuildAttemptsCounter(txPduInfoIdx, 0);     /* SBSW_SECOC_PTR2FUNC_2 */
          /* --- Set the Tx Pdu State to "Reauthentication needed" --- */
          SecOC_SetTxPduState(txPduInfoIdx, SECOC_PRE_REAUTHENTICATION_NEEDED_TXPDUSTATE);      /* SBSW_SECOC_CSL80 */
          /* Set the Transmit Request state of the corresponding Pdu to Transmit */
          /* --- Set the Transmit request --- */
          SecOC_SetTransmitRequest(txPduInfoIdx, SECOC_TRANSMIT_TRANSMITREQUEST);       /* SBSW_SECOC_PTR2FUNC_2 */
#  if (SECOC_TXSPLITPDUINFO == STD_ON)
          if(SecOC_IsTxSplitPduInfoUsedOfTxPduInfo(txPduInfoIdx))       /* COV_SECOC_SPLIT_ONLY_CONFIG */
          {
            SecOC_SizeOfTxSplitPduInfoType splitTxPduId = SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduInfoIdx);
            SecOC_SetAuthenticSplitPduTransmit(splitTxPduId, SECOC_TRANSMIT_AUTHENTICSPLITPDUTRANSMIT); /* SBSW_SECOC_CSL80 */
            SecOC_SetCryptographicPduTransmit(splitTxPduId, SECOC_TRANSMIT_AUTHENTICSPLITPDUTRANSMIT);  /* SBSW_SECOC_CSL80 */
            SecOC_SetTxSplitMessageCounter(splitTxPduId, 0);    /* SBSW_SECOC_CSL80 */
          }
#  endif
        }
          break;
# endif
# if (SECOC_EXISTS_TPPDU_PDUTYPEOFTXPDUINFO == STD_ON)
        case SECOC_TPPDU_PDUTYPEOFTXPDUINFO:
        {
          SecOC_SetTxAuthenticPduSizeOffset(txPduInfoIdx, ((SecOC_TxAuthenticPduSizeOffsetType) (SecOC_GetAuthenticPduDataContainerFullAuthenticPduLengthOfTxPduInfo(txPduInfoIdx)) - (SecOC_TxAuthenticPduSizeOffsetType) (info->SduLength))); /* SBSW_SECOC_CSL02 */

#  if (SECOC_SECUREDPDUDATACONTAINERAUTHPDUHEADERLENGTHUSEDOFTXPDUINFO == STD_ON)
          if(SecOC_IsSecuredPduDataContainerAuthPduHeaderLengthUsedOfTxPduInfo(txPduInfoIdx))
          {
            SecOC_SerializeLengthToHeader(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetStartIdxOfTxAuthPduHeader(txPduInfoIdx)), (uint8) SecOC_GetSecuredPduDataContainerAuthPduHeaderLengthLengthOfTxPduInfo(txPduInfoIdx), info->SduLength); /* SBSW_SECOC_PTR2FUNC_3 */
          }
#  endif
          /* --- Set the Tx Pdu State to "Get Data" --- */
          SecOC_SetTxPduState(txPduInfoIdx, SECOC_GET_DATA_TXPDUSTATE); /* SBSW_SECOC_CSL80 */
        }
          break;
# endif
        default:       /* COV_SECOC_MISRA */
          break;
      }
      /* --- return E_OK --- */
      ret = E_OK;
    }
    SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();

# if(SECOC_EXISTS_IMMEDIATE_PDUPROCESSINGOFTXPDUINFO)
    if(isImmediateProcessingRequired == TRUE)
    {
      ret = SecOC_TxIf_ProcessTxPdu(txPduInfoIdx);
      SecOC_SetTxImmediateProcessingOngoing(txPduInfoIdx, FALSE);       /* SBSW_SECOC_CSL02 */
    }
# endif
#else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
#endif
  }

  /* ----- Development Error Report --------------------------------------- */
#if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)       /* PRQA S 2991, 2995 */ /* MD_SecOC_2991, MD_SecOC_2995 */ /* COV_SECOC_RX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_TRANSMIT, errorId);
  }
#endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(id);    /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(info);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */

  return ret;
}

#if (SECOC_COMMUNICATION_INTERFACE == STD_ON)   /* COV_SECOC_ALWAYS_ON */
/**********************************************************************************************************************
  SecOC_RxIndication
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080, 6010, 6050 1 */ /* MD_MSR_STMIF, MD_MSR_STPTH, MD_MSR_STCAL */
FUNC(void, SECOC_CODE) SecOC_RxIndication(PduIdType id, P2CONST(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
# if(SECOC_DEV_ERROR_DETECT == STD_ON)  /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#  if(SECOC_RXSECUREDPDUINFO == STD_ON)
  else if(id >= SecOC_GetSizeOfRxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   if(SECOC_INVALIDHNDOFRXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfRxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
  else if(info == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if(info->SduDataPtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
#  endif
# endif
# if(SECOC_RXSECUREDPDUINFO == STD_ON)
  else if(info->SduLength < SecOC_GetMinimumPduLengthOfRxSecuredPduInfo(id))
  {
    errorId = SECOC_E_PARAM;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
# if(SECOC_RXSECUREDPDUINFO == STD_ON)
    boolean wasProcessed = SecOC_RxIndication_PDUProcessing(id, info);  /* SBSW_SECOC_PTR2FUNC_3 */

    if(wasProcessed == FALSE)
    {
#  if (SECOC_PDUSWITHQUEUESIDXOFRXPDUINFO == STD_ON)
      boolean wasAddedToQueue = SecOC_Queue_AddPDUToQueue(SecOC_GetRxPduInfoIdxOfRxSecuredPduInfo(id), info);   /* SBSW_SECOC_PTR2FUNC_3 */
      if(wasAddedToQueue == FALSE)
#  endif
      {
        SecOC_ReportRuntimeDetError(SECOC_SID_RX_INDICATION, SECOC_E_BUFFER_BUSY);
      }
    }
# else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)       /* PRQA S 2991, 2995 */ /* MD_SecOC_2991, MD_SecOC_2995 */ /* COV_SECOC_TX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_RX_INDICATION, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(id);    /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(info);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */

}

# if (SECOC_TX_CONFIRMATION_API == STD_ON)
/**********************************************************************************************************************
  SecOC_TxConfirmation
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, SECOC_CODE) SecOC_TxConfirmation(PduIdType id)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
#  if(SECOC_DEV_ERROR_DETECT == STD_ON) /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#   if(SECOC_TXSECUREDPDUINFO == STD_ON)        /* COV_SECOC_VAR_DEPENDENT_SWITCH */
  else if(id >= SecOC_GetSizeOfTxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#    if(SECOC_INVALIDHNDOFTXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfTxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#    endif
#    if(SECOC_EXISTS_TPPDU_LOWERLAYERPDUTYPEOFTXPDUINFO == STD_ON)
  else if(SecOC_GetLowerLayerPduTypeOfTxPduInfo(SecOC_GetTxPduInfoIdxOfTxSecuredPduInfo(id)) == SECOC_TPPDU_LOWERLAYERPDUTYPEOFTXPDUINFO)
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#    endif
#   endif
#  endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#  if(SECOC_EXISTS_IFPDU_LOWERLAYERPDUTYPEOFTXPDUINFO == STD_ON)        /* COV_SECOC_VAR_DEPENDENT_SWITCH */
    /* --- Action: SecOC_TxConfirmation --- */
    /* --- Call PduR_SecOCTxConfirmation --- */
#   if( SECOC_CONFIRMATIONNEEDEDOFTXPDUINFO == STD_ON)
    SecOC_TxPduInfoIterType txPduInfoIdx = SecOC_GetTxPduInfoIdxOfTxSecuredPduInfo(id);
    if(SecOC_IsConfirmationNeededOfTxPduInfo(txPduInfoIdx))
    {
      PduR_SecOCTxConfirmation(SecOC_GetAuthenticExternalIdOfTxPduInfo(txPduInfoIdx));
    }
#   endif
#  else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
#  endif
  }

  /* ----- Development Error Report --------------------------------------- */
#  if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_TX_CONFIRMATION, errorId);
  }
#  endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(id);    /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}
# endif

# if (SECOC_TRIGGER_TRANSMIT_API == STD_ON)
/**********************************************************************************************************************
  SecOC_TriggerTransmit
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *

 *
**********************************************************************************************************************/
/* PRQA S 6080, 6030, 6050 1 */ /* MD_MSR_STMIF, MD_MSR_STCYC, MD_MSR_STCAL */
FUNC(Std_ReturnType, SECOC_CODE) SecOC_TriggerTransmit(PduIdType id, P2VAR(PduInfoType, AUTOMATIC, SECOC_APPL_VAR) info)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;
  Std_ReturnType ret = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if component is initialized */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
#  if(SECOC_DEV_ERROR_DETECT == STD_ON) /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#   if(SECOC_TXSECUREDPDUINFO == STD_ON)        /* COV_SECOC_VAR_DEPENDENT_SWITCH */
  else if(id >= SecOC_GetSizeOfTxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#    if(SECOC_INVALIDHNDOFTXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfTxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#    endif
#    if(SECOC_EXISTS_TPPDU_LOWERLAYERPDUTYPEOFTXPDUINFO == STD_ON)
  else if(SecOC_GetLowerLayerPduTypeOfTxPduInfo(SecOC_GetTxPduInfoIdxOfTxSecuredPduInfo(id)) == SECOC_TPPDU_LOWERLAYERPDUTYPEOFTXPDUINFO)
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#    endif
#   endif
  else if(info == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if(info->SduDataPtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
#  endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
#  if(SECOC_EXISTS_IFPDU_LOWERLAYERPDUTYPEOFTXPDUINFO == STD_ON)        /* COV_SECOC_VAR_DEPENDENT_SWITCH */
    SecOC_TxPduInfoIterType txPduInfoIdx = SecOC_GetTxPduInfoIdxOfTxSecuredPduInfo(id);
    PduLengthType pduLength = 0u;
    SecOC_SizeOfSecuredPduDataContainerType pduStartIdx = 0u;

    SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();

    switch (SecOC_GetTxSecuredPduTypeOfTxSecuredPduInfo(id))
    {
#   if(SECOC_EXISTS_NORMAL_TXSECUREDPDUTYPEOFTXSECUREDPDUINFO == STD_ON)
      case SECOC_NORMAL_TXSECUREDPDUTYPEOFTXSECUREDPDUINFO:
        pduLength = (PduLengthType) SecOC_GetLengthOfTxSecuredPdu(txPduInfoIdx);
        pduStartIdx = (SecOC_SizeOfSecuredPduDataContainerType) SecOC_GetStartIdxOfTxSecuredPdu(txPduInfoIdx);
        break;
#   endif
#   if(SECOC_EXISTS_CRYPTO_TXSECUREDPDUTYPEOFTXSECUREDPDUINFO == STD_ON)
      case SECOC_CRYPTO_TXSECUREDPDUTYPEOFTXSECUREDPDUINFO:
      {
        SecOC_SizeOfTxSplitPduInfoType splitTxPduId = SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduInfoIdx);
        pduLength = (PduLengthType) SecOC_GetSecuredPduDataContainerCryptographicPduLengthOfTxSplitPduInfo(splitTxPduId);
        pduStartIdx = (SecOC_SizeOfSecuredPduDataContainerType) SecOC_GetSecuredPduDataContainerCryptographicPduStartIdxOfTxSplitPduInfo(splitTxPduId);
      }
        break;
#   endif
#   if(SECOC_EXISTS_AUTHENTIC_TXSECUREDPDUTYPEOFTXSECUREDPDUINFO == STD_ON)
      case SECOC_AUTHENTIC_TXSECUREDPDUTYPEOFTXSECUREDPDUINFO:
        pduLength = (PduLengthType) SecOC_GetLengthOfTxAuthenticSplitPdu(txPduInfoIdx);
        pduStartIdx = (SecOC_SizeOfSecuredPduDataContainerType) SecOC_GetStartIdxOfTxAuthenticSplitPdu(txPduInfoIdx);
        break;
#   endif
      default: /* COV_SECOC_MISRA */
        break;
    }
    /* --- Action: SecOC_TriggerTransmit --- */
    /* --- Check state --- */
    /* return E_NOT_OK if the Pdu has not the state "Authenticated" or "transmission initiated" */
    if(info->SduLength >= pduLength)
    {
      if((SecOC_GetTxPduState(txPduInfoIdx) == SECOC_AUTHENTICATED_TXPDUSTATE) || (SecOC_GetTxPduState(txPduInfoIdx) == SECOC_TRANSMISSION_INITIATED_TXPDUSTATE))       /* COV_SECOC_RETRY_FAILED_TRANSMIT_REQUEST_DISABLED */
      {
        /* --- copy data --- */
        /* --- update state and Freshness Value if required --- */
        VStdMemCpyRamToRam(info->SduDataPtr, SecOC_GetAddrSecuredPduDataContainer(pduStartIdx), pduLength);     /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_CPY_2RAM_PARAMCKECK */
        info->SduLength = (PduLengthType) pduLength;    /* SBSW_SECOC_PTR2FUNC_4 */

        ret = E_OK;

        switch (SecOC_GetTxSecuredPduTypeOfTxSecuredPduInfo(id))
        {
#   if(SECOC_EXISTS_NORMAL_TXSECUREDPDUTYPEOFTXSECUREDPDUINFO == STD_ON)
          case SECOC_NORMAL_TXSECUREDPDUTYPEOFTXSECUREDPDUINFO:
            SecOC_SetTxBuildAttemptsCounter(txPduInfoIdx, 0u);  /* SBSW_SECOC_CSL80 */
            SecOC_SetTxPduState(txPduInfoIdx, SECOC_PRE_REAUTHENTICATION_NEEDED_TXPDUSTATE);    /* SBSW_SECOC_CSL80 */
            break;
#   endif
#   if(SECOC_EXISTS_CRYPTO_TXSECUREDPDUTYPEOFTXSECUREDPDUINFO == STD_ON)
          case SECOC_CRYPTO_TXSECUREDPDUTYPEOFTXSECUREDPDUINFO:
          {
            SecOC_SizeOfTxSplitPduInfoType splitTxPduId = SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduInfoIdx);
            SecOC_IncTxSplitMessageCounter(splitTxPduId);       /* SBSW_SECOC_CSL02 */
            if(SecOC_GetTxSplitMessageCounter(splitTxPduId) >= 2u)
            {
              SecOC_SetTxSplitMessageCounter(splitTxPduId, 0u); /* SBSW_SECOC_CSL80 */
              SecOC_SetTxBuildAttemptsCounter(txPduInfoIdx, 0u);        /* SBSW_SECOC_CSL80 */
              SecOC_SetTxPduState(txPduInfoIdx, SECOC_PRE_REAUTHENTICATION_NEEDED_TXPDUSTATE);  /* SBSW_SECOC_CSL80 */
            }
          }
            break;
#   endif
#   if(SECOC_EXISTS_AUTHENTIC_TXSECUREDPDUTYPEOFTXSECUREDPDUINFO == STD_ON)
          case SECOC_AUTHENTIC_TXSECUREDPDUTYPEOFTXSECUREDPDUINFO:
          {
            SecOC_SizeOfTxSplitPduInfoType splitTxPduId = SecOC_GetTxSplitPduInfoIdxOfTxPduInfo(txPduInfoIdx);
            SecOC_IncTxSplitMessageCounter(splitTxPduId);       /* SBSW_SECOC_CSL02 */
            if(SecOC_GetTxSplitMessageCounter(splitTxPduId) >= 2u)
            {
              SecOC_SetTxSplitMessageCounter(splitTxPduId, 0u); /* SBSW_SECOC_CSL80 */
              SecOC_SetTxBuildAttemptsCounter(txPduInfoIdx, 0u);        /* SBSW_SECOC_CSL80 */
              SecOC_SetTxPduState(txPduInfoIdx, SECOC_PRE_REAUTHENTICATION_NEEDED_TXPDUSTATE);  /* SBSW_SECOC_CSL80 */
            }
          }
            break;
#   endif
          default:     /* COV_SECOC_MISRA */
            break;
        }
      }
    }
    SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_TXSTATE();
#  else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
#  endif
  }

  /* ----- Development Error Report --------------------------------------- */
#  if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_TRIGGER_TRANSMIT, errorId);
  }
#  endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */

  return ret;
}
# endif
#endif

#if (SECOC_TRANSPORT_PROTOCOL == STD_ON)
/**********************************************************************************************************************
  SecOC_TpRxIndication
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, SECOC_CODE) SecOC_TpRxIndication(PduIdType id, Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
# if(SECOC_DEV_ERROR_DETECT == STD_ON)  /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#  if(SECOC_RXSECUREDPDUINFO == STD_ON)
  else if(id >= SecOC_GetSizeOfRxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   if(SECOC_INVALIDHNDOFRXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfRxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
#  endif
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
# if (SECOC_RXSECUREDPDUINFO == STD_ON)
    SecOC_RxPduInfoIterType rxPduInfoIdx = SecOC_GetRxPduInfoIdxOfRxSecuredPduInfo(id);
    if(SecOC_GetRxPduState(rxPduInfoIdx) == SECOC_COPYING_RXPDUSTATE)
    {
      if(result == E_OK)
      {
        SecOC_SetRxPduState(rxPduInfoIdx, SECOC_COPY_AUTHENTIC_PDU_TO_DATA_TO_AUTHENTICATE_RXPDUSTATE); /* SBSW_SECOC_CSL80 */
      }
      else
      {
#  if(SECOC_EXISTS_TPPDU_PDUTYPEOFRXPDUINFO == STD_ON)
        if(SecOC_GetPduTypeOfRxPduInfo(rxPduInfoIdx) == SECOC_TPPDU_PDUTYPEOFRXPDUINFO) /* COV_SECOC_RX_TP_UP_ONLY */
        {
          PduR_SecOCTpRxIndication(SecOC_GetAuthenticExternalIdOfRxPduInfo(rxPduInfoIdx), E_NOT_OK);
        }
#  endif
        SecOC_SetRxPduState(rxPduInfoIdx, SECOC_IDLE_RXPDUSTATE);       /* SBSW_SECOC_CSL80 */
      }
    }
    {
      SecOC_SetRxTpWrittenBytesCounter(rxPduInfoIdx, 0);        /* SBSW_SECOC_CSL80 */
    }
# else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)       /* COV_SECOC_TX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_TP_RX_INDICATION, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(id);    /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(result);        /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
  SecOC_TpTxConfirmation
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, SECOC_CODE) SecOC_TpTxConfirmation(PduIdType id, Std_ReturnType result)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
# if(SECOC_DEV_ERROR_DETECT == STD_ON)  /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#  if(SECOC_TXSECUREDPDUINFO == STD_ON)
  else if(id >= SecOC_GetSizeOfTxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   if(SECOC_INVALIDHNDOFTXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfTxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
#   if(SECOC_EXISTS_IFPDU_LOWERLAYERPDUTYPEOFTXPDUINFO == STD_ON)
  else if(SecOC_GetLowerLayerPduTypeOfTxPduInfo(SecOC_GetTxPduInfoIdxOfTxSecuredPduInfo(id)) == SECOC_IFPDU_LOWERLAYERPDUTYPEOFTXPDUINFO)       /* COV_SECOC_ONLY_INCORRECT_PDU_TYPE */
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
#  endif
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
# if(SECOC_EXISTS_TPPDU_LOWERLAYERPDUTYPEOFTXPDUINFO == STD_ON)
    SecOC_TxPduInfoIterType txPduInfoIdx = SecOC_GetTxPduInfoIdxOfTxSecuredPduInfo(id);
    if((SecOC_GetTxPduState(txPduInfoIdx) == SECOC_TRANSMISSION_INITIATED_TXPDUSTATE) || (SecOC_GetTxPduState(txPduInfoIdx) == SECOC_COPYING_TXPDUSTATE))
    {
      SecOC_SetTxPduState(txPduInfoIdx, SECOC_INVALID_SECURED_PDU_TXPDUSTATE);  /* SBSW_SECOC_CSL80 */
#  if(SECOC_EXISTS_TPPDU_PDUTYPEOFTXPDUINFO == STD_ON)
      if(SecOC_GetPduTypeOfTxPduInfo(txPduInfoIdx) == SECOC_TPPDU_PDUTYPEOFTXPDUINFO)   /* COV_SECOC_TX_TP_UP_ONLY */
      {
        PduR_SecOCTpTxConfirmation(SecOC_GetAuthenticExternalIdOfTxPduInfo(txPduInfoIdx), result);
      }
#  endif
    }
    SecOC_SetTxTpWrittenBytesCounter(txPduInfoIdx, 0);  /* SBSW_SECOC_CSL80 */

# else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)       /* PRQA S 2991, 2995 */ /* MD_SecOC_2991, MD_SecOC_2995 */ /* COV_SECOC_RX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_TP_TX_CONFIRMATION, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(result);        /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(id);    /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}

/**********************************************************************************************************************
  SecOC_CopyRxData
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_CopyRxData(PduIdType id, P2VAR(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info, P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) bufferSizePtr) /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if component is initialized */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
# if(SECOC_DEV_ERROR_DETECT == STD_ON)  /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#  if(SECOC_RXSECUREDPDUINFO == STD_ON)
  else if(id >= SecOC_GetSizeOfRxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   if(SECOC_INVALIDHNDOFRXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfRxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
  else if(info == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if(info->SduDataPtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if(bufferSizePtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
#  endif
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
# if (SECOC_SECUREDPDUDATACONTAINERAUTHENTICPDUSTARTIDXOFRXPDUINFO == STD_ON)
    SecOC_RxPduInfoIterType rxPduInfoIdx = SecOC_GetRxPduInfoIdxOfRxSecuredPduInfo(id);

    if((info->SduLength <= (SecOC_GetLengthOfRxTpSecuredPduDataContainerSecuredPdu(rxPduInfoIdx) - SecOC_GetRxTpWrittenBytesCounter(rxPduInfoIdx))) && (SecOC_GetRxPduState(rxPduInfoIdx) == SECOC_COPYING_RXPDUSTATE))
    {
      VStdMemCpyRamToRam(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetStartIdxOfRxSecuredPduDataContainerSecuredPdu(rxPduInfoIdx) + SecOC_GetRxTpWrittenBytesCounter(rxPduInfoIdx)), info->SduDataPtr, info->SduLength);       /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_PTR_WITH_OFFSET */
      SecOC_SetRxTpWrittenBytesCounter(rxPduInfoIdx, (SecOC_GetRxTpWrittenBytesCounter(rxPduInfoIdx) + info->SduLength));       /* SBSW_SECOC_CSL80 */
      *bufferSizePtr = (PduLengthType) (SecOC_GetLengthOfRxTpSecuredPduDataContainerSecuredPdu(rxPduInfoIdx) - SecOC_GetRxTpWrittenBytesCounter(rxPduInfoIdx)); /* SBSW_SECOC_PTR2FUNC_4 */
      retVal = BUFREQ_OK;
    }
# else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)       /* COV_SECOC_TX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_COPY_RX_DATA, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(id);    /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(info);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(bufferSizePtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */

  return retVal;
}

/**********************************************************************************************************************
  SecOC_CopyTxData
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080, 6030 1 */ /* MD_MSR_STMIF, MD_MSR_STCYC */
FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_CopyTxData(PduIdType id, P2VAR(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info, P2VAR(RetryInfoType, AUTOMATIC, SECOC_APPL_DATA) retry, P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) availableDataPtr)      /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if component is initialized */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
# if(SECOC_DEV_ERROR_DETECT == STD_ON)  /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#  if(SECOC_TXSECUREDPDUINFO == STD_ON)
  else if(id >= SecOC_GetSizeOfTxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   if(SECOC_INVALIDHNDOFTXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfTxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
  else if(info == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if(info->SduDataPtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
  else if(availableDataPtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
#   if(SECOC_EXISTS_IFPDU_LOWERLAYERPDUTYPEOFTXPDUINFO == STD_ON)
  else if(SecOC_GetLowerLayerPduTypeOfTxPduInfo(SecOC_GetTxPduInfoIdxOfTxSecuredPduInfo(id)) == SECOC_IFPDU_LOWERLAYERPDUTYPEOFTXPDUINFO)       /* COV_SECOC_ONLY_INCORRECT_PDU_TYPE */
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
#  endif
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
# if(SECOC_EXISTS_TPPDU_LOWERLAYERPDUTYPEOFTXPDUINFO == STD_ON)
    boolean processPdu = TRUE;
    if(retry != NULL_PTR)
    {
      if(retry->TpDataState == TP_DATARETRY)
      {
        processPdu = FALSE;
      }
    }
    if(processPdu == TRUE)
    {
      SecOC_TxPduInfoIterType txPduInfoIdx = SecOC_GetTxPduInfoIdxOfTxSecuredPduInfo(id);
      if(SecOC_GetTxPduState(txPduInfoIdx) == SECOC_TRANSMISSION_INITIATED_TXPDUSTATE)
      {
        SecOC_SetTxPduState(txPduInfoIdx, SECOC_COPYING_TXPDUSTATE);    /* SBSW_SECOC_CSL80 */
      }
      if(SecOC_GetTxPduState(txPduInfoIdx) == SECOC_COPYING_TXPDUSTATE)
      {
        if(info->SduLength <= (SecOC_GetLengthOfTxSecuredPdu(txPduInfoIdx) - SecOC_GetTxTpWrittenBytesCounter(txPduInfoIdx)))
        {
          VStdMemCpyRamToRam(info->SduDataPtr, SecOC_GetAddrSecuredPduDataContainer(SecOC_GetStartIdxOfTxSecuredPdu(txPduInfoIdx) + SecOC_GetTxTpWrittenBytesCounter(txPduInfoIdx)), info->SduLength);  /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_PTR_WITH_OFFSET */
          SecOC_SetTxTpWrittenBytesCounter(txPduInfoIdx, (SecOC_GetTxTpWrittenBytesCounter(txPduInfoIdx) + info->SduLength));   /* SBSW_SECOC_CSL80 */
          *availableDataPtr = (PduLengthType) (SecOC_GetLengthOfTxSecuredPdu(txPduInfoIdx) - SecOC_GetTxTpWrittenBytesCounter(txPduInfoIdx));   /* SBSW_SECOC_PTR2FUNC_4 */
          retVal = BUFREQ_OK;
        }
      }
    }
# else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
# endif
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)       /* PRQA S 2991, 2995 */ /* MD_SecOC_2991, MD_SecOC_2995 */ /* COV_SECOC_RX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_COPY_TX_DATA, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(retry); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(info);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(availableDataPtr);      /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(id);    /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */

  return retVal;
}

/**********************************************************************************************************************
  SecOC_StartOfReception
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080, 6030 1 */ /* MD_MSR_STMIF, MD_MSR_STCYC */
FUNC(BufReq_ReturnType, SECOC_CODE) SecOC_StartOfReception(PduIdType id, P2VAR(PduInfoType, AUTOMATIC, SECOC_APPL_DATA) info, PduLengthType TpSduLength, P2VAR(PduLengthType, AUTOMATIC, SECOC_APPL_DATA) bufferSizePtr)        /* PRQA S 3673 */ /* MD_MSR_Rule8.13 */
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  BufReq_ReturnType retVal = BUFREQ_E_NOT_OK;   /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if component is initialized */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
# if(SECOC_DEV_ERROR_DETECT == STD_ON)  /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#  if(SECOC_RXSECUREDPDUINFO == STD_ON)
  else if(id >= SecOC_GetSizeOfRxSecuredPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   if(SECOC_INVALIDHNDOFRXSECUREDPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfRxSecuredPduInfo(id))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
  else if(bufferSizePtr == NULL_PTR)
  {
    errorId = SECOC_E_PARAM_POINTER;
  }
#  endif
# endif
# if (SECOC_RXSECUREDPDUINFO == STD_ON)
  else if(TpSduLength < SecOC_GetMinimumPduLengthOfRxSecuredPduInfo(id))
  {
    errorId = SECOC_E_PARAM;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
# if (SECOC_RXSECUREDPDUINFO == STD_ON)
    SecOC_RxPduInfoIterType rxPduInfoIdx = SecOC_GetRxPduInfoIdxOfRxSecuredPduInfo(id);
    if(SecOC_GetRxPduState(rxPduInfoIdx) == SECOC_IDLE_RXPDUSTATE)
    {
      if(SecOC_GetSecuredPduDataContainerTpSecuredPduLengthOfRxPduInfo(rxPduInfoIdx) >= TpSduLength)
      {
        SecOC_RxAuthenticPduSizeOffsetType offset = ((SecOC_RxAuthenticPduSizeOffsetType) (SecOC_GetSecuredPduDataContainerTpSecuredPduLengthOfRxPduInfo(rxPduInfoIdx)) - (SecOC_RxAuthenticPduSizeOffsetType) TpSduLength);
#  if (SECOC_EXISTS_TPPDU_PDUTYPEOFRXPDUINFO == STD_ON)
        boolean process = TRUE;
        if(SecOC_GetPduTypeOfRxPduInfo(rxPduInfoIdx) == SECOC_TPPDU_PDUTYPEOFRXPDUINFO) /* COV_SECOC_RX_TP_UP_ONLY */
        {
          retVal = PduR_SecOCTpStartOfReception(SecOC_GetAuthenticExternalIdOfRxPduInfo(rxPduInfoIdx), info, ((PduLengthType) (SecOC_GetAuthenticPduDataContainerAuthenticPduLengthOfRxPduInfo(rxPduInfoIdx)) - ((PduLengthType) offset)), bufferSizePtr);      /* SBSW_SECOC_PTR2FUNC_3 */
          if(retVal != BUFREQ_OK)
          {
            process = FALSE;
          }
        }
        if(process == TRUE)
#  endif
        {
          SecOC_SetRxAuthenticPduSizeOffset(rxPduInfoIdx, offset);      /* SBSW_SECOC_CSL02 */
          *bufferSizePtr = (PduLengthType) SecOC_GetSecuredPduDataContainerTpSecuredPduLengthOfRxPduInfo(rxPduInfoIdx) - SecOC_GetRxTpWrittenBytesCounter(rxPduInfoIdx);        /* SBSW_SECOC_PTR2FUNC_4 */

#  if (SECOC_AUTHENTICPDUDATACONTAINERAUTHENTICPDUMETADATAUSEDOFRXPDUINFO == STD_ON)
          if(SecOC_IsSecuredPduDataContainerSecuredPDUMetaDataUsedOfRxPduInfo(rxPduInfoIdx))
          {
            if(info != NULL_PTR)
            {
              if((info->SduDataPtr != NULL_PTR) && (info->SduLength >= SecOC_GetSecuredPduDataContainerSecuredPDUMetaDataLengthOfRxPduInfo(rxPduInfoIdx)))
              {
                PduLengthType metadataStartPosition = (PduLengthType) (info->SduLength - SecOC_GetSecuredPduDataContainerSecuredPDUMetaDataLengthOfRxPduInfo(rxPduInfoIdx));
                VStdMemCpyRamToRam(SecOC_GetAddrSecuredPduDataContainer(SecOC_GetSecuredPduDataContainerSecuredPDUMetaDataStartIdxOfRxPduInfo(rxPduInfoIdx)), &(info->SduDataPtr[metadataStartPosition]), SecOC_GetSecuredPduDataContainerSecuredPDUMetaDataLengthOfRxPduInfo(rxPduInfoIdx));     /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_SECOC_MEM_CPY_2RAM */
              }
              else
              {
                errorId = SECOC_E_PARAM;
              }
            }
            else
            {
              errorId = SECOC_E_PARAM_POINTER;
            }
          }
          if(errorId == SECOC_E_NO_ERROR)
#  endif
          {
            SecOC_SetVerifyAttemptsCounter(rxPduInfoIdx, 0);    /* SBSW_SECOC_CSL80 */
            SecOC_SetRxBuildAttemptsCounter(rxPduInfoIdx, 0);   /* SBSW_SECOC_CSL80 */
            retVal = BUFREQ_OK;
            SecOC_SetRxPduState(rxPduInfoIdx, SECOC_COPYING_RXPDUSTATE);        /* SBSW_SECOC_CSL80 */
          }
        }
      }
      else
      {
        retVal = BUFREQ_E_OVFL;
      }
    }
# else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
# endif
  }
  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)       /* COV_SECOC_TX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_START_OF_RECEPTION, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(info);  /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(id);    /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(TpSduLength);   /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(bufferSizePtr); /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */

  return retVal;
}
#endif

#if (SECOC_CANCEL_TRANSMIT_API == STD_ON)
/***********************************************************************************************************************
 *  SecOC_CancelTransmit()
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 **********************************************************************************************************************/
FUNC(Std_ReturnType, SECOC_CODE) SecOC_CancelTransmit(PduIdType TxPduId)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType ret = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
# if(SECOC_DEV_ERROR_DETECT == STD_ON)  /* COV_SECOC_VAR_DEV_ERROR_DETECT */
#  if(SECOC_TXAUTHENTICPDUINFO == STD_ON)
  else if(TxPduId >= SecOC_GetSizeOfTxAuthenticPduInfo())
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   if(SECOC_INVALIDHNDOFTXAUTHENTICPDUINFO == STD_ON)
  else if(SecOC_IsInvalidHndOfTxAuthenticPduInfo(TxPduId))
  {
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
  }
#   endif
#  endif
# endif
  else
  {
# if(SECOC_TXAUTHENTICPDUINFO == STD_ON)
    SecOC_TxPduInfoIterType txPduInfoIdx = SecOC_GetTxPduInfoIdxOfTxAuthenticPduInfo(TxPduId);
#  if(SECOC_EXISTS_TPPDU_PDUTYPEOFTXPDUINFO == STD_ON)
    if(SecOC_GetTxPduState(txPduInfoIdx) == SECOC_GET_DATA_TXPDUSTATE)  /* COV_SECOC_TP_ONLY */
    {
      if(SecOC_GetPduTypeOfTxPduInfo(txPduInfoIdx) == SECOC_TPPDU_PDUTYPEOFTXPDUINFO)   /* COV_SECOC_TX_TP_UP_ONLY */
      {
        PduR_SecOCTpTxConfirmation(SecOC_GetAuthenticExternalIdOfTxPduInfo(txPduInfoIdx), E_NOT_OK);
      }
    }
#  endif
    SecOC_SetCancelTxTransmit(txPduInfoIdx, TRUE);      /* SBSW_SECOC_CSL02 */
    ret = PduR_SecOCCancelTransmit(SecOC_GetSecuredExternalIdOfTxPduInfo(txPduInfoIdx));
# else
    errorId = SECOC_E_INVALID_PDU_SDU_ID;
# endif
  }
  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)       /* PRQA S 2991, 2995 */ /* MD_SecOC_2991, MD_SecOC_2995 */ /* COV_SECOC_TX_ONLY_CONFIG */
  {
    SecOC_ReportDetError(SECOC_SID_CANCEL_TRANSMIT, errorId);
  }
# endif
  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  SECOC_DUMMY_STATEMENT(TxPduId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  return ret;
}
#endif

#if (SECOC_SECUREDRXPDUVERFICATIONOFRXPDUINFO == STD_ON)
/**********************************************************************************************************************
  SecOC_VerifyStatusOverride
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 *
 *
**********************************************************************************************************************/
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(Std_ReturnType, SECOC_CODE) SecOC_VerifyStatusOverride(uint16 freshnessValueID, uint8 overrideStatus, uint8 numberOfMessagesToOverride)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType ret = E_NOT_OK;        /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
  /* #10 Check if component is initialized */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
# if(SECOC_DEV_ERROR_DETECT == STD_ON)  /* COV_SECOC_VAR_DEV_ERROR_DETECT */
  else if(!((overrideStatus == SECOC_CANCEL_VERIFYOVERRIDESTATUS) || (overrideStatus == SECOC_FAIL_N_TIMES_VERIFYOVERRIDESTATUS) || (overrideStatus == SECOC_FAIL_INFINETE_VERIFYOVERRIDESTATUS)
#  if (SECOC_ENABLE_FORCED_PASS_OVERRIDESTATUS == STD_ON)
            || (overrideStatus == SECOC_PASS_N_TIMES_VERIFYOVERRIDESTATUS) || (overrideStatus == SECOC_PASS_VERIFYOVERRIDESTATUS) || (overrideStatus == SECOC_OVERRIDE_SKIP_UNTIL_LIMIT_VERIFYOVERRIDESTATUS) || (overrideStatus == SECOC_OVERRIDE_SKIP_UNTIL_NOTICE_VERIFYOVERRIDESTATUS) || (overrideStatus == SECOC_PASS_UNTIL_LIMIT_VERIFYOVERRIDESTATUS) || (overrideStatus == SECOC_PASS_UNTIL_NOTICE_VERIFYOVERRIDESTATUS)
#  endif
          ))
  {
    errorId = SECOC_E_PARAM;
  }
# endif
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    SecOC_RxPduInfoIterType rxPduId = 0;
    SchM_Enter_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
    for(; rxPduId < SecOC_GetSizeOfRxPduInfo(); rxPduId++)
    {
      if(freshnessValueID == SecOC_GetExternalFreshnessIdOfRxPduInfo(rxPduId))
      {
        if((overrideStatus == SECOC_FAIL_N_TIMES_VERIFYOVERRIDESTATUS)
# if (SECOC_ENABLE_FORCED_PASS_OVERRIDESTATUS == STD_ON)
           || (overrideStatus == SECOC_PASS_N_TIMES_VERIFYOVERRIDESTATUS) || (overrideStatus == SECOC_OVERRIDE_SKIP_UNTIL_LIMIT_VERIFYOVERRIDESTATUS) || (overrideStatus == SECOC_PASS_UNTIL_LIMIT_VERIFYOVERRIDESTATUS)
# endif
          )
        {
          SecOC_SetNumberOfMessagesToOverride(SecOC_GetNumberOfMessagesToOverrideIdxOfRxPduInfo(rxPduId), numberOfMessagesToOverride);  /* SBSW_SECOC_CSL03 */
        }
        SecOC_SetVerifyOverrideStatus(SecOC_GetVerifyOverrideStatusIdxOfRxPduInfo(rxPduId), overrideStatus);    /* SBSW_SECOC_CSL03 */
      }
    }
    SchM_Exit_SecOC_SECOC_EXCLUSIVE_AREA_RXSTATE();
    ret = E_OK;
  }

  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_VERIFY_STATUS_OVERRIDE, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */

  return ret;
}
#endif

#if(SECOC_DEVMODEENABLED == STD_ON)
/***********************************************************************************************************************
 *  SecOC_SetDevelopmentMode()
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
 */
FUNC(void, SECOC_CODE) SecOC_SetDevelopmentMode(boolean enableDevMode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;     /* PRQA S 2981 */ /* MD_MSR_RetVal */

  /* ----- Development Error Checks ------------------------------------- */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    SecOC_SetDevModeEnabled(enableDevMode);     /* SBSW_SECOC_CSL_VAR_ACCESS */

# if (SECOC_SENDDEFAULTAUTHENTICATION == STD_ON)
    {
      SecOC_SendDefaultAuthenticationIterType sendDefaultAuthenticationIdx = 0;
      for(; sendDefaultAuthenticationIdx < SecOC_GetSizeOfSendDefaultAuthentication(); sendDefaultAuthenticationIdx++)
      {
        SecOC_SetSendDefaultAuthentication(sendDefaultAuthenticationIdx, enableDevMode);        /* SBSW_SECOC_CSL01 */
      }
    }
# endif
  }
  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_SET_DEVELOPMENT_MODE, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
}
#endif


#if(SECOC_SENDDEFAULTAUTHENTICATION == STD_ON)
/***********************************************************************************************************************
 *  SecOC_SendDefaultAuthenticationInformation()
 **********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, SECOC_CODE) SecOC_SendDefaultAuthenticationInformation(uint16 freshnessValueID, boolean sendDefaultAuthenticationInformation)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = SECOC_E_NO_ERROR;     /* PRQA S 2981 */ /* MD_MSR_RetVal */
  Std_ReturnType ret = E_NOT_OK;

  /* ----- Development Error Checks ------------------------------------- */
  if(!SecOC_IsInitialized())
  {
    errorId = SECOC_E_UNINIT;
  }
  else
  {
    /* ----- Implementation ----------------------------------------------- */
    SecOC_SendDefaultAuthenticationIterType sendDefaultAuthenticationIdx = 0;
    for(; sendDefaultAuthenticationIdx < SecOC_GetSizeOfSendDefaultAuthentication(); sendDefaultAuthenticationIdx++)
    {
      if(freshnessValueID == SecOC_GetExternalFreshnessIdOfTxPduInfo(sendDefaultAuthenticationIdx))
      {
        SecOC_SetSendDefaultAuthentication(sendDefaultAuthenticationIdx, sendDefaultAuthenticationInformation); /* SBSW_SECOC_CSL01 */
        ret = E_OK;
      }
    }
  }
  /* ----- Development Error Report --------------------------------------- */
# if(SECOC_DEV_ERROR_REPORT == STD_ON)
  if(errorId != SECOC_E_NO_ERROR)
  {
    SecOC_ReportDetError(SECOC_SID_SEND_DEFAULT_AUTHENTICATION_INFORMATION, errorId);
  }
# endif

  SECOC_DUMMY_STATEMENT(errorId);       /* PRQA S 1338, 2983, 3112 */ /* MD_MSR_DummyStmt */ /* lint -e{438} */
  return ret;

}
#endif

/**********************************************************************************************************************
  SecOC_MainFunctionRx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_MainFunctionRx(void)
{
#if (SECOC_RXPDUINFO == STD_ON)
  /* #10 Check if component is initialized */
  if(SecOC_IsInitialized())
  {
    /* ----- Implementation ----------------------------------------------- */
    SecOC_RxIf_HandlePdus();
  }
#endif
}

/**********************************************************************************************************************
  SecOC_MainFunctionTx
**********************************************************************************************************************/
/*! Internal comment removed.
 *
 *
 *
**********************************************************************************************************************/
FUNC(void, SECOC_CODE) SecOC_MainFunctionTx(void)
{
#if (SECOC_TXPDUINFO == STD_ON)
  /* #10 Check if component is initialized */
  if(SecOC_IsInitialized())
  {
    /* ----- Implementation ----------------------------------------------- */
    SecOC_TxIf_HandlePdus();
  }
#endif
}

#define SECOC_STOP_SEC_CODE
#include "SecOC_MemMap.h" /* PRQA S 5087 */   /* MD_MSR_MemMap */

/* *INDENT-OFF* */
/* module specific MISRA deviations:

  MD_SecOC_2982:  Misra Rule 2.2
     Reason:      It depends on the configuration if the value is used before being modified or not.
     Risk:        No risk.
     Prevention:  No prevention necessary.

  MD_SecOC_3205: Misra Rule 2.3
     Reason:      It depends on the configuration if the identifier is used or not.
     Risk:        No risk.
     Prevention:  No prevention necessary.

  MD_SecOC_2991: Misra Rule 14.3
     Reason:      The value of the if-controlling expression depends on the configuration.
     Risk:        No risk.
     Prevention:  No prevention necessary.

  MD_SecOC_2995: Misra Rule 2.2
     Reason:      The value of the logical operation depends on the configuration.
     Risk:        No risk.
     Prevention:  No prevention necessary.

  MD_SecOC_1505:  Misra Rule 8.7
     Reason:      It depends on the configuration if the function is only referenced within in the translation unit or not.
     Risk:        No risk.
     Prevention:  No prevention necessary.

  MD_SecOC_2985:  Misra Rule 2.2
     Reason:      It depends on the configuration if the operation is redundant.
     Risk:        No risk.
     Prevention:  No prevention necessary.

  MD_SecOC_1860:  Misra Rule 10.4
     Reason:      The condition before the subtraction ensures that the result can't be negative..
     Risk:        No risk.
     Prevention:  No prevention necessary.
*/

/* SBSW_JUSTIFICATION_BEGIN

  \ID SBSW_SECOC_CSL01
    \DESCRIPTION Index Access is using ComStackLib
    \COUNTERMEASURE  \N Qualified use-case CSL01 of ComStackLib.

  \ID SBSW_SECOC_CSL02
    \DESCRIPTION  Access of Table1, via indirection over Table2 with same index using ComStackLib.
    \COUNTERMEASURE \N Qualified use-case CSL02 of ComStackLib

  \ID SBSW_SECOC_CSL03
    \DESCRIPTION Access to Table1 via indirection over Table2.
    \COUNTERMEASURE \N Qualified use-case CSL03 of ComStackLib

  \ID SBSW_SECOC_CSL80
    \DESCRIPTION Access of Table1, via indirection over Table2 with same index via indirection over Table 3.
    \COUNTERMEASURE \N Qualified use-case CSL02 and CSL03 of ComStackLib

  \ID SBSW_SECOC_CSL80_GENERATED_HANDLES
    \DESCRIPTION Access of Table1, via indirection over Table2 with same index via indirection over Table 3.
    \COUNTERMEASURE \N Qualified use-case CSL02 and CSL03 of ComStackLib. The source handle for the indirection is generated and therefore valid.

  \ID SBSW_SECOC_PTR2FUNC_1
    \DESCRIPTION A pointer is passed to a function.
    \COUNTERMEASURE \N CM_SECOC_READ_WITH_CSL_MACRO

  \ID SBSW_SECOC_PTR2FUNC_2
    \DESCRIPTION A pointer is passed to a function.
    \COUNTERMEASURE \N CM_SECOC_LOCAL_STACK_VAR

  \ID SBSW_SECOC_PTR2FUNC_3
    \DESCRIPTION A pointer is passed to a function.
    \COUNTERMEASURE \N CM_SECOC_PTR_HANDED_THROUGH

  \ID SBSW_SECOC_PTR2FUNC_4
    \DESCRIPTION Write access to a passed pointer.
    \COUNTERMEASURE \N CM_SECOC_PTR_HANDED_THROUGH

  \ID SBSW_SECOC_PTR2FUNC_5
    \DESCRIPTION A pointer is passed to a function.
    \COUNTERMEASURE \N CM_SECOC_READ_WITH_CSL_MACRO
                       CM_SECOC_LOCAL_STACK_VAR

  \ID SBSW_SECOC_PTR2FUNC_6
    \DESCRIPTION A pointer is passed to a function.
    \COUNTERMEASURE \N CM_SECOC_LOCAL_STACK_VAR
                       CM_SECOC_PTR_HANDED_THROUGH

  \ID SBSW_SECOC_ARRAY_WRITE_ACCESS_WITH_LENGTH
    \DESCRIPTION Write access to a passed pointer.
    \COUNTERMEASURE \N The Length of the array is passed and checked during runtime.

  \ID SBSW_SECOC_FUNC_CALL
    \DESCRIPTION A function is called.
    \COUNTERMEASURE \N The called function verifies the parameter.

  \ID SBSW_SECOC_PTR2FUNC_7
    \DESCRIPTION A pointer is passed to a function.
    \COUNTERMEASURE \N CM_SECOC_READ_WITH_CSL_MACRO
                       CM_SECOC_PTR_HANDED_THROUGH

  \ID SBSW_SECOC_PTR2FUNC_8
    \DESCRIPTION A pointer is passed to a function.
    \COUNTERMEASURE \N CM_SECOC_READ_WITH_CSL_MACRO
                       CM_SECOC_LOCAL_STACK_VAR
                       CM_SECOC_PTR_HANDED_THROUGH

  \ID SBSW_SECOC_VERIFY_STATUS_CALLOUT
    \DESCRIPTION A function pointer is called.
    \COUNTERMEASURE \S see SMI-41233

  \ID SBSW_SECOC_PTR_WITH_OFFSET
    \DESCRIPTION Two pointer are passed to a function.
    \COUNTERMEASURE \R One pointer is read with a generated macro of the ComStackLib and an offset which is checked for out-of-bounds access.
                       The other pointer is handed through from the caller and therefore assumed valid. It is also checked that it is no NULL_PTR.

  \ID SBSW_SECOC_VSTDLIB_SAFE
    \DESCRIPTION    Pointer is passed as function argument.
    \COUNTERMEASURE \N The VStdLib_MemCpy_s guarantees that the destination pointer is not overwritten.

  \ID SBSW_SECOC_MEM_CPY_2RAM
    \DESCRIPTION    Function call to copy data to RAM.
    \COUNTERMEASURE \N The address of the destination and the amount of bytes to be copied are
                       retrieved through use-case CSL03 of ComStackLib indirection access macros.
  \ID SBSW_SECOC_MEM_CPY_2RAM_PARAMCKECK
    \DESCRIPTION    Function call to copy data to RAM.
    \COUNTERMEASURE \N The address of the destination and the amount of bytes to be copied are
                       checked by the parameter checks.

  \ID SBSW_SECOC_MEM_SET
    \DESCRIPTION    Function call to write data to RAM.
    \COUNTERMEASURE \N The address of the destination is retrieved through use-case CSL03 of ComStackLib indirection access macros.
                       The amount of bytes to be copied are	validated to be sufficient

  \ID SBSW_SECOC_CALCULATED_INDEX
    \DESCRIPTION    The Array is accessed with a calculated index.
    \COUNTERMEASURE \R The calculated index is checked during runtime.

  \ID SBSW_SECOC_FCTCALL_SECAREA
    \DESCRIPTION    Memcopy is called with a function
    \COUNTERMEASURE \R Runtime check asserts written pointer is large enough.

  \ID SBSW_SECOC_CSL_VAR_ACCESS
    \DESCRIPTION A ComStackLib generated variable is accessed.
    \COUNTERMEASURE \N The variable is written using a generated macro of the CSL. It immediately dereferences the variables address without pointer arithmetic.

   SBSW_JUSTIFICATION_END */
/*
\CM CM_SECOC_READ_WITH_CSL_MACRO The pointer is read with generated macros of the ComStackLib. Therefore it is always valid.
\CM CM_SECOC_LOCAL_STACK_VAR The pointer references a local stack variable. Therefore it is always a valid memory location.
\CM CM_SECOC_PTR_HANDED_THROUGH The pointer is handed through from the caller. The caller has to ensure it points to a valid memory location and it is of sufficient size.
*/
/* COV_JUSTIFICATION_BEGIN

  \ID COV_SECOC_VAR_DEV_ERROR_DETECT
    \ACCEPT TX
    \REASON Det error detection must always be enabled for SafeBsw.

  \ID COV_SECOC_VAR_DEPENDENT_SWITCH
    \ACCEPT TX
    \REASON The nested switch depends on a previous switch.

  \ID COV_SECOC_TX_ONLY_CONFIG
    \ACCEPT TX
    \REASON In Tx only configs the API shall always return an error.

  \ID COV_SECOC_RX_ONLY_CONFIG
    \ACCEPT TX
    \REASON In Rx only configs the API shall always return an error.

\ID COV_SECOC_SPLIT_ONLY_CONFIG
    \ACCEPT TX
    \REASON In configurations with only Split PDUs this check will always evaluate to true.

  \ID COV_SECOC_VERIFICATION_STATUS_PROPAGATION_BOTH_ONLY_CONFIG
    \ACCEPT TX
    \REASON This configuration only has Pdus with verification status propagation set to BOTH.

  \ID COV_SECOC_SYNC_CSM_ONLY_CONFIG
    \ACCEPT TX
    \ACCEPT TF tf xf
    \ACCEPT TX tx xx
    \REASON This is always true in an interface only config with only synchronous Csm calls.

  \ID COV_SECOC_NO_TRIGGER_TX_CONFIG
    \ACCEPT TF tf tx
    \ACCEPT TF tf tf xf tf
    \REASON In configs without trigger transmit the transmit request is always set by the SecOC_Transmit API.

  \ID COV_SECOC_IGNORE_VERIFICATION_RESULT_CONFIG
    \ACCEPT TX
    \ACCEPT XX
    \REASON If IGNORE_VERIFICATION_RESULT is configured to true the verification result is always SECOC_E_VER_OK.

  \ID COV_SECOC_NO_TRUNCATED_FRESHNESS_VALUE
    \ACCEPT TX
    \ACCEPT XF
    \REASON If no Rx Pdu has a truncated Freshness Value configured, the truncated MAC is always >= 8 and multiple of 8 Bits. Therefore no Byte offset is existing.

  \ID COV_SECOC_ALWAYS_ON
    \ACCEPT TX
    \REASON This preprocessor switch is always on if Tp or If Pdus are configured.

  \ID COV_SECOC_SILENT_CHECK
    \ACCEPT TX
    \REASON This check shall always evaluate to TRUE for a valid configuration.

  \ID COV_SECOC_MISRA
    \ACCEPT X
    \ACCEPT TX
    \REASON [MSR_COV_MISRA]

  \ID COV_SECOC_NOT_SUPPORTED
    \ACCEPT XF
    \REASON Message linker with not byte aligned length is not supported.

  \ID COV_SECOC_SAFEBSW_CHECK
    \ACCEPT TX
    \ACCEPT TX tx tx
    \REASON This condition is required for safeBSW, the if will only evaluate to false if the generated code would be incorrect.

  \ID COV_SECOC_VERIFICATION_TO_BE_STARTED_LOOP
    \ACCEPT   XF tf xf
    \REASON This condition evaluates always to false if "ignore verification result" is enabled.

  \ID COV_SECOC_RX_TP_UP_ONLY
    \ACCEPT   TX
    \REASON statement might always be true if only Rx PDUs with PduType set to TP are configured.

  \ID COV_SECOC_TX_TP_UP_ONLY
    \ACCEPT   TX
    \REASON statement might always be true if only Tx PDUs with PduType set to TP are configured.

  \ID COV_SECOC_TX_IF_LOW_ONLY
    \ACCEPT   TX
    \REASON statement might always be true if only Tx PDUs with lower PduType set to IF are configured.

  \ID COV_SECOC_TP_ONLY
    \ACCEPT   TX
    \ACCEPT   TF tf xf
    \REASON the condition might not be reached if only TP PDUs are configured.

  \ID COV_SECOC_CUSTOM_AUTH_API
    \ACCEPT   TX
    \ACCEPT   XF
    \REASON the condition might not be reached if the custom authentication API is configured.

  \ID COV_SECOC_CSM_SYNCHRONOUS_MODE_ONLY
    \ACCEPT   TX
    \REASON statement might always be true if the configuration contains only PDUs in sycnhronous mode.

  \ID COV_SECOC_TX_STATE_NOT_REACHABLE
    \ACCEPT TF tf tx
    \ACCEPT TF tf tf tx
    \ACCEPT TF tf tf xf
    \REASON Not all sub conditions are reachable if only Tx PDUs with PduType set to TP are configured or if retry failed transmit request is disabled. In configs without trigger transmit the transmit request is always set by the SecOC_Transmit API

  \ID COV_SECOC_IGNORE_VERIFICATION_RESULT
     \ACCEPT XF tf xf
     \ACCEPT TF tf tx
     \REASON Not reachable if ignore verification result is enabled.

  \ID COV_SECOC_ONLY_INCORRECT_PDU_TYPE
     \ACCEPT TX
     \REASON Not reachable if configuration contains only PDUs with the incorrect PduType.

  \ID COV_SECOC_NO_MESSAGE_LINKER
     \ACCEPT XF
     \ACCEPT XX
     \ACCEPT TX
     \REASON Not reachable if configuration contains no Secured Pdu Collections with message linker.

  \ID COV_SECOC_ALLWAYS_MESSAGE_LINKER
     \ACCEPT TX
     \REASON Not reachable if configuration contains only Secured Pdu Collections with message linker.

  \ID COV_SECOC_NO_VERIFICATION_ENABLED
     \ACCEPT XF
     \ACCEPT TX
     \REASON Statement is not fully covered in configurations which has only RX PDUs with verification disabled.

    \ID COV_SECOC_ONLY_TX_AUTHINFO
     \ACCEPT XF
     \ACCEPT TX
     \ACCEPT XX
     \REASON Not reachable if configuration contains no freshness value and no message linker.

    \ID COV_SECOC_IMMEDIATE_CHECK
     \ACCEPT TX
     \REASON This condition can't evaluate to FALSE in unit test suite. This check is required to avoid race conditions.

    \ID COV_SECOC_MSGLINK_FIX_LENGTH
     \ACCEPT TX
     \REASON This condition can't evaluate to FALSE in configurations with a full byte message linker length.

    \ID COV_SECOC_RETRY_FAILED_TRANSMIT_REQUEST_DISABLED
     \ACCEPT TX
     \ACCEPT TF xf tf
     \REASON Statement will always be true in configurations where retry failed transmit request is disbaled.

   \ID COV_SECOC_NO_META_DATA
     \ACCEPT XF
     \REASON Statement will never be reached in configurations with no configured meta data.


COV_JUSTIFICATION_END */
/* *INDENT-ON* */

/**********************************************************************************************************************
 *  END OF FILE: SecOC.c
 *********************************************************************************************************************/
