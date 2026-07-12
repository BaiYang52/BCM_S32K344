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
/**        \file  Dcm_Svc29.c
 *         \unit  Svc29
 *        \brief  Contains the implementation of Service 0x29 unit.
 *
 *      \details  MICROSAR DCM based on AR 4.0.3
 *
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *********************************************************************************************************************/
#define DCM_SVC29_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc29.h"
#if (DCM_SVC_29_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_Uti.h"
# include "Dcm_Debug.h"
# include "Dcm_Net.h"
# include "Dcm_Mode.h"
# include "Dcm_AuthMgr.h"
# include "Dcm_CfgWrap.h"
# include "Dcm_DidMgr.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Service 0x29 processing steps */
# define DCM_SVC29_PROGRESS_SUBFUNCTION                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
# define DCM_SVC29_PROGRESS_WAITCERTVERIFY                           ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))
# define DCM_SVC29_PROGRESS_CERTVERIFIED                             ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 2u))
# define DCM_SVC29_PROGRESS_WAITCHALLENGEGEN                         ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 3u))
# define DCM_SVC29_PROGRESS_CHALLENGEGEN                             ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 4u))
# define DCM_SVC29_PROGRESS_SUBFUNC02_WAITCHALLENGESIGNED            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 5u))
# define DCM_SVC29_PROGRESS_SUBFUNC02_CHALLENGESIGNED                ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 6u))
# define DCM_SVC29_PROGRESS_SUBFUNC03_WAITSIGVERIFY                  ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 7u))
# define DCM_SVC29_PROGRESS_SUBFUNC03_ROLEREAD                       ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 8u))
# define DCM_SVC29_PROGRESS_SUBFUNC03_SERVICEWHITELISTREAD           ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 9u))
# define DCM_SVC29_PROGRESS_SUBFUNC03_DIDWHITELISTREAD               ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 10u))
# define DCM_SVC29_PROGRESS_SUBFUNC03_RIDWHITELISTREAD               ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 11u))
# define DCM_SVC29_PROGRESS_SUBFUNC03_MEMWHITELISTREAD               ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 12u))
# define DCM_SVC29_PROGRESS_PERSIST_AUTHENTICATION                   ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 13u))

/*! Service 0x29 subfunction references */
# define DCM_SVC29_SUBFUNC_DEAUTHENTICATE                            ((uint8)0x00u)
# define DCM_SVC29_SUBFUNC_VERIFY_CERT_UNIDIR                        ((uint8)0x01u)
# define DCM_SVC29_SUBFUNC_VERIFY_CERT_BIDIR                         ((uint8)0x02u)
# define DCM_SVC29_SUBFUNC_PROOF_OF_OWNERSHIP                        ((uint8)0x03u)
# define DCM_SVC29_SUBFUNC_TRANSMIT_CERTIFICATE                      ((uint8)0x04u)

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/*! Service 0x29 UDS AuthenticationReturnParameter */
#  define DCM_SVC29_AUTH_CONFIG_APCE                                 ((uint8)0x02u)
#  define DCM_SVC29_DEAUTH_SUCCESSFUL                                ((uint8)0x10u)
#  define DCM_SVC29_OWNERSHIP_VERIFICATION_NECESSARY                 ((uint8)0x11u)
#  define DCM_SVC29_OWNERSHIP_VERIFIED_AUTHENTICATION_COMPLETE       ((uint8)0x12u)
#  define DCM_SVC29_CERTIFICATE_VERIFIED                             ((uint8)0x13u)

/*! Service 0x29 A_Data fields byte length */
#  define DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN                         ((uint32)0x02u)
#  define DCM_SVC29_CHALLENGE_LEN_INDEX                              (0x01u)
#  define DCM_SVC29_CHALLENGE_DATA_START_INDEX                       (0x03u)

/*! Service 0x29 processing states for asynchronous callback handling */
#  define DCM_SVC29_CBK_PROC_STATE_IDLE                              ((Dcm_Svc29ProcessingState)0x00u)
#  define DCM_SVC29_CBK_PROC_STATE_CSM_CANCELLED                     ((Dcm_Svc29ProcessingState)0x01u)
#  define DCM_SVC29_CBK_PROC_STATE_KEYM_CANCELLED                    ((Dcm_Svc29ProcessingState)0x02u)
#  define DCM_SVC29_CBK_PROC_STATE_VERIFY_CERT                       ((Dcm_Svc29ProcessingState)0x03u)
#  define DCM_SVC29_CBK_PROC_STATE_VERIFY_CERT_DONE                  ((Dcm_Svc29ProcessingState)0x04u)
#  define DCM_SVC29_CBK_PROC_STATE_GENERATE_CHALLENGE                ((Dcm_Svc29ProcessingState)0x05u)
#  define DCM_SVC29_CBK_PROC_STATE_GENERATE_CHALLENGE_DONE           ((Dcm_Svc29ProcessingState)0x06u)
#  define DCM_SVC29_CBK_PROC_STATE_SIGN_CHALLENGE                    ((Dcm_Svc29ProcessingState)0x07u)
#  define DCM_SVC29_CBK_PROC_STATE_SIGN_CHALLENGE_DONE               ((Dcm_Svc29ProcessingState)0x08u)
#  define DCM_SVC29_CBK_PROC_STATE_VERIFY_POOW                       ((Dcm_Svc29ProcessingState)0x09u)
#  define DCM_SVC29_CBK_PROC_STATE_VERIFY_POOW_DONE                  ((Dcm_Svc29ProcessingState)0x0Au)
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
#  define Dcm_Svc29GetPendingKeyMCertId()                            (Dcm_Svc29SingletonContext.PendingKeyMCertId)                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29SetPendingKeyMCertId(certId)                      (Dcm_Svc29SingletonContext.PendingKeyMCertId = (certId))                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29GetPendingCsmJobId()                              (Dcm_Svc29SingletonContext.PendingCsmJobId)                                     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29SetPendingCsmJobId(jobId)                         (Dcm_Svc29SingletonContext.PendingCsmJobId = (jobId))                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29GetSignatureVerifyResult()                        (Dcm_Svc29SingletonContext.SignatureVerifyResult)                               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29SetSignatureVerifyResult(result)                  (Dcm_Svc29SingletonContext.SignatureVerifyResult = (result))                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29GetLengthAsyncCbk()                               (Dcm_Svc29SingletonContext.LengthAsyncCbk)                                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29SetLengthAsyncCbk(length)                         (Dcm_Svc29SingletonContext.LengthAsyncCbk = (length))                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29GetLengthChallengeSwapBuffer()                    (Dcm_Svc29SingletonContext.LengthChallengeSwapBuffer)                           /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29SetLengthChallengeSwapBuffer(length)              (Dcm_Svc29SingletonContext.LengthChallengeSwapBuffer = (length))                /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29GetCbkProcessingState()                           (Dcm_Svc29SingletonContext.CbkProcessingState)                                  /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29SetCbkProcessingState(state)                      (Dcm_Svc29SingletonContext.CbkProcessingState = (state))                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29SetNextCbkProcessingState()                       (++Dcm_Svc29GetCbkProcessingState())                                            /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29GetBufferHdl()                                    (Dcm_NetGetProtObjOfConnection(Dcm_NetGetConnHdlOfRxPduId(Dcm_Svc29GetRxPduId()))->RxTxBufferRef) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#  define Dcm_Svc29IsCbkStateActive(state)                           (Dcm_Svc29GetCbkProcessingState() == (state))                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29IsAsyncCsmJobActive()                             (Dcm_Svc29IsCbkStateActive(DCM_SVC29_CBK_PROC_STATE_GENERATE_CHALLENGE) || Dcm_Svc29IsCbkStateActive(DCM_SVC29_CBK_PROC_STATE_SIGN_CHALLENGE) || Dcm_Svc29IsCbkStateActive(DCM_SVC29_CBK_PROC_STATE_VERIFY_POOW)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_Svc29IsAsyncKeyMJobActive()                            (Dcm_Svc29IsCbkStateActive(DCM_SVC29_CBK_PROC_STATE_VERIFY_CERT))               /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# endif

# define Dcm_Svc29GetRxPduId()                                       (Dcm_Svc29SingletonContext.RxPduId)                                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_Svc29SetRxPduId(id)                                     (Dcm_Svc29SingletonContext.RxPduId = (id))                                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# define Dcm_Svc29SetOwnershipRequestExpected()                      (Dcm_Svc29SingletonContext.OwnershipRequestExpected = TRUE)                     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_Svc29ResetOwnershipRequestExpected()                    (Dcm_Svc29SingletonContext.OwnershipRequestExpected = FALSE)                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_Svc29IsOwnershipRequestExpected()                       (Dcm_Svc29SingletonContext.OwnershipRequestExpected == TRUE)                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
typedef uint8 Dcm_Svc29ProcessingState;
# endif

/*! Context for request sequence and async callback handling */
struct DCM_SVC29SINGLETONCONTEXTTYPE_TAG
{
# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
  KeyM_CertificateIdType        PendingKeyMCertId;                                    /*!< Key Manager pending certificate ID */
  Dcm_Svc29ProcessingState      CbkProcessingState;                                   /*!< Asynchronous callback processing state */
  KeyM_CertificateStatusType    KeyMCbkResult;                                        /*!< Key Manager callback result */
# endif
# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
  uint32                        PendingCsmJobId;                                      /*!< Crypto Service Manager pending Job ID */
  Crypto_VerifyResultType       SignatureVerifyResult;                                /*!< Store result for signature verify */
  uint32                        LengthAsyncCbk;                                       /*!< Length storage for async operations */
  uint16                        LengthChallengeSwapBuffer;                            /*!< Length of the generated challenge data */
  uint8                         ChallengeSwapBuffer[DCM_AUTHMGR_CHALLENGE_MAX_SIZE];  /*!< Pointer to generated challenge data */
  Crypto_ResultType             CsmCbkResult;                                         /*!< Crypto Service Manager callback result */
# endif
  PduIdType                     RxPduId;                                              /*!< Pdu ID associated with the sequence flag */
  boolean                       OwnershipRequestExpected;                             /*!< Last subfunction that processed was 01 or 02 (0 = was failed or not processed) */
};
typedef struct DCM_SVC29SINGLETONCONTEXTTYPE_TAG Dcm_Svc29SingletonContextType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc29Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29CbkResultDispatcher()
 *********************************************************************************************************************/
/*! \brief          Dispatch the asynchronous callback result.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29CbkResultDispatcher(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );
# endif

/**********************************************************************************************************************
 *  Dcm_Svc29SubFuncHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 SubFunction Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send positive response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29SubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29KeyMSetCertificate()
 *********************************************************************************************************************/
/*! \brief          Set certificate wrapper function.
 *  \details        Stores certificate in KeyM and handles possible return values.
 *  \param[in]      certId                The certificate identifier
 *  \param[in]      pCertificateData      Pointer to struct containing certificate data and length
 *  \param[out]     ErrorCode             The NRC
 *  \return         DCM_E_NOT_OK          Send negative response
 *  \return         DCM_E_OK              Successful
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29KeyMSetCertificate(
  KeyM_CertificateIdType certId,
  P2CONST(KeyM_CertDataType, AUTOMATIC, DCM_APPL_DATA) pCertificateData,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29KeyMVerifyCertificate()
 *********************************************************************************************************************/
/*! \brief          Verify stored certificate wrapper function.
 *  \details        Verifies stored certificate in KeyM and handles possible return values.
 *  \param[in]      pContext              Pointer to the context
 *  \param[in]      certId                The certificate identifier
 *  \param[out]     ErrorCode             The NRC
 *  \return         DCM_E_NOT_OK          Send negative response
 *  \return         DCM_E_PENDING         Retry later
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29KeyMVerifyCertificate(
  Dcm_ContextPtrType pContext,
  KeyM_CertificateIdType certId,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29GeneralNrcCheck()
 *********************************************************************************************************************/
/*! \brief          Changes Certificate Verification Failed NRC to General NRC.
 *  \details        If NRC pertains to Certificate Verification Failed, it is changed to General NRC.
 *  \param[in]      pContext              Pointer to the context
 *  \param[in,out]  ErrorCode             The NRC
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc29GeneralNrcCheck(
  Dcm_ContextPtrType pContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29FinalizeSequenceStartReq()
 *********************************************************************************************************************/
/*! \brief          Finalize request processing of subfunction 0x01 and 0x02.
 *  \details        -
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \return         DCM_E_OK                Send positive response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29FinalizeSequenceStartReq(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_SVC_29_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29GetServerCertificate()
 *********************************************************************************************************************/
/*! \brief          Gets the server certificate of KeyM.
 *  \details        -
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Server certificate retreived
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29GetServerCertificate(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29ValidateCsmCbkResult()
 *********************************************************************************************************************/
/*! \brief          Validates the callback result of Csm.
 *  \details        -
 *  \param[out]     ErrorCode               The NRC
 *  \return         DCM_E_OK                Successful callback result
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29ValidateCsmCbkResult(
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29ValidateKeyMCbkResult()
 *********************************************************************************************************************/
/*! \brief          Validates the callback result of KeyM.
 *  \details        -
 *  \param[out]     ErrorCode               The NRC
 *  \return         DCM_E_OK                Successful callback result
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29ValidateKeyMCbkResult(
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29CertificateStoreVerify()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 sub-function 0x01, 0x02 and 0x04 storage and verfication step.
 *  \details        Stores and verifies client certificate from request.
 *  \param[in]      clientCertId            Certificate Id to be stored
 *  \param[in]      clientCertLength        Client certificate length
 *  \param[in]      pContext                Pointer to the context
 *  \param[in,out]  pMsgContext             The message context
 *  \param[out]     ErrorCode               The NRC
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29CertificateStoreVerify(
  uint16 clientCertId,
  uint16 clientCertLength,
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29CertificateVerificationDoneHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 sub-function 0x01 and 0x02 stored certificate verification step.
 *  \details        Stored certificate verification.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             The message context
 *  \param[in,out]  pRepContext             The repeater context
 *  \param[out]     ErrorCode               The NRC
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29CertificateVerificationDoneHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29GenerateServerChallenge()
 *********************************************************************************************************************/
/*! \brief          Challenge generation wrapper function.
 *  \details        Triggers the Csm to generate the server challenge.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in,out]  pMsgContext             The message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             The repeater context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29GenerateServerChallenge(
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29ChallengeGenerationDoneHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 certificate signing step.
 *  \details        Certificate signing after successful challenge generation.
 *  \param[in]      pContext                   Pointer to the context
 *  \param[in]      opStatus                   The operations status
 *  \param[in,out]  pMsgContext                The message context
 *  \param[out]     ErrorCode                  The NRC
 *  \param[in,out]  pRepContext                The repeater context
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_PENDING              Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29ChallengeGenerationDoneHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_29_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29GenerateSignature()
 *********************************************************************************************************************/
/*! \brief          Signature generation wrapper function.
 *  \details        Evaluates previous operation result and generates challenge.
 *  \param[in]      pContext                   Pointer to the context
 *  \param[in,out]  pMsgContext                The message context
 *  \param[out]     ErrorCode                  The NRC
 *  \param[in,out]  pRepContext                The repeater context
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_PENDING              Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29GenerateSignature(
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_29_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29_02ChallengeSigningDoneHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 sub-function 0x02 final step.
 *  \details        Sends positive response if challege signing and certificate retrieval was successful.
 *  \param[in]      pContext                   Pointer to the context
 *  \param[in]      opStatus                   The operations status
 *  \param[in,out]  pMsgContext                The message context
 *  \param[out]     ErrorCode                  The NRC
 *  \param[in,out]  pRepContext                The repeater context
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Send positive response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29_02ChallengeSigningDoneHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );

# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29_03RoleRead()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 sub-function 0x03 role retrieval step.
 *  \details        Retrieve role from key manager.
 *  \param[in]      pContext                   Pointer to the context
 *  \param[in]      opStatus                   The operation status
 *  \param[in,out]  pMsgContext                The message context
 *  \param[out]     ErrorCode                  The NRC
 *  \param[in,out]  pRepContext                The repeater context
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_LOOP                 Repeat Repeater Proxy immediately
 *  \return         DCM_E_PENDING              Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29_03RoleRead(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29_03ServiceWL()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 sub-function 0x03 SERVICE white list retrieval step.
 *  \details        Retrieve service white list from key manager.
 *  \param[in]      pContext                   Pointer to the context
 *  \param[in]      opStatus                   The operation status
 *  \param[in,out]  pMsgContext                The message context
 *  \param[out]     ErrorCode                  The NRC
 *  \param[in,out]  pRepContext                The repeater context
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_LOOP                 Repeat Repeater Proxy immediately
 *  \return         DCM_E_PENDING              Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29_03ServiceWL(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29_03DidWL()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 sub-function 0x03 DID white list retrieval step.
 *  \details        Retrieve DID white list from key manager.
 *  \param[in]      pContext                   Pointer to the context
 *  \param[in]      opStatus                   The operation status
 *  \param[in,out]  pMsgContext                The message context
 *  \param[out]     ErrorCode                  The NRC
 *  \param[in,out]  pRepContext                The repeater context
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_LOOP                 Repeat Repeater Proxy immediately
 *  \return         DCM_E_PENDING              Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29_03DidWL(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29_03RidWL()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 sub-function 0x03 RID white list retrieval step.
 *  \details        Retrieve RID white list from key manager.
 *  \param[in]      pContext                   Pointer to the context
 *  \param[in]      opStatus                   The operation status
 *  \param[in,out]  pMsgContext                The message context
 *  \param[out]     ErrorCode                  The NRC
 *  \param[in,out]  pRepContext                The repeater context
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_LOOP                 Repeat Repeater Proxy immediately
 *  \return         DCM_E_PENDING              Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29_03RidWL(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29_03MemWL()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 sub-function 0x03 MEM white list retrieval step.
 *  \details        Retrieve MEM white list from key manager.
 *  \param[in]      pContext                   Pointer to the context
 *  \param[in]      opStatus                   The operation status
 *  \param[in,out]  pMsgContext                The message context
 *  \param[out]     ErrorCode                  The NRC
 *  \param[in,out]  pRepContext                The repeater context
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Send positive response
 *  \return         DCM_E_LOOP                 Repeat Repeater Proxy immediately
 *  \return         DCM_E_PENDING              Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29_03MemWL(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29PersistAuthentication()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 authentication status persistence.
 *  \details        Call NvMWriteBlock in order to persist authentication data.
 *  \param[in]      opStatus                   The operation status
 *  \param[in,out]  pMsgContext                The message context
 *  \param[out]     ErrorCode                  The NRC
 *  \param[in,out]  pRepContext                The repeater context
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_OK                   Send positive response
 *  \return         DCM_E_PENDING              Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29PersistAuthentication(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  );
# endif

/**********************************************************************************************************************
 *  Dcm_Svc29ReqLengthGetter()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 sub-function length getter.
 *  \details        Always returns the sub-function specific length.
 *  \param[in]      sfRef                The sub-function zero-based index
 *  \return         Expected sub-function specific length
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc29ReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  );

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29VerifySignature()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 sub-function 0x03 Signature verify.
 *  \details        Triggers the verify signature job of Csm.
 *  \param[in]      proofOfOwnershipLength     Proof of Ownership length
 *  \param[in]      pContext                   Pointer to the context
 *  \param[in,out]  pMsgContext                The message context
 *  \param[out]     ErrorCode                  The NRC
 *  \return         DCM_E_NOT_OK               Send negative response
 *  \return         DCM_E_PENDING              Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29VerifySignature(
  uint16 proofOfOwnershipLength,
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

/**********************************************************************************************************************
 *  Dcm_Svc29SubFuncSequenceChecker()
 *********************************************************************************************************************/
/*! \brief          Service 0x29 sub-function sequence checker.
 *  \details        Checks whether the sub-function sequence is correct.
 *  \param[in,out]  pMsgContext                 The message context
 *  \param[in]      subSvcRef                   The sub-function zero-based index
 *  \param[out]     ErrorCode                   The NRC
 *  \return         DCM_E_NOT_OK                Wrong sequence
 *  \return         DCM_E_OK                    Successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29SubFuncSequenceChecker(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DiagSubServiceRefOptType subSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Singleton context of Svc29 unit */
DCM_LOCAL VAR(Dcm_Svc29SingletonContextType, DCM_VAR_NOINIT) Dcm_Svc29SingletonContext;
# define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc29Handler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
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
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType                lStdResult;
  Dcm_DiagSubServiceRefOptType  lSubSvcRef;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
  /* Check if callback processing state is idle */
  if(Dcm_Svc29GetCbkProcessingState() != DCM_SVC29_CBK_PROC_STATE_IDLE)
  {
    /* The asynchronous callback processing is still active for another tester */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  else
# endif
  {
    /* Get subfunction id before it gets consumed */
    pRepContext->SubSvcId = Dcm_UtiGetReqDataAsU8(pMsgContext);                                                                                      /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_WRITE */

    lStdResult = Dcm_DiagVerifyServiceWithSubFunction(pMsgContext
                                                     ,Dcm_CfgSvc29SubFuncLookUpTable
                                                     ,Dcm_CfgSvc29SubFuncExecPrecondTable
                                                     ,Dcm_Svc29ReqLengthGetter
                                                     ,Dcm_Svc29SubFuncSequenceChecker
                                                     ,&lSubSvcRef
                                                     ,ErrorCode);                                                                                    /* SBSW_DCM_COMB_PTR_FORWARD */

    /* Initialize service processor context for further processing */
    pRepContext->SubSvcRef = (Dcm_DiagSubServiceRefMemType)lSubSvcRef; /* Store working sub-function reference (also in negative response cases) */  /* SBSW_DCM_PARAM_PTR_WRITE */
# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
    pRepContext->AuthInfoHdl = Dcm_NetGetAuthInfoRef(Dcm_DiagGetTranspObj(pContext)->ConnHdl);                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
# endif

    Dcm_Svc29SetRxPduId(pMsgContext->rxPduId);

    if (lStdResult == DCM_E_OK)
    {
      Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_SUBFUNCTION); /* delegate job */                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_LOOP; /* speed up processing */
    }
  }
  return lStdResult;
}

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29CbkResultDispatcher()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29CbkResultDispatcher(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_LOOP;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  switch(Dcm_Svc29GetCbkProcessingState())
  {
  case DCM_SVC29_CBK_PROC_STATE_VERIFY_CERT_DONE:
    Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_CERTVERIFIED);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_SVC29_CBK_PROC_STATE_GENERATE_CHALLENGE_DONE:
      Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_CHALLENGEGEN);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#  if (DCM_SVC_29_02_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC29_CBK_PROC_STATE_SIGN_CHALLENGE_DONE:
    Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_SUBFUNC02_CHALLENGESIGNED);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
#  endif
  case DCM_SVC29_CBK_PROC_STATE_VERIFY_POOW_DONE:
    Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_SUBFUNC03_ROLEREAD);                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  default:
    /* Callback not triggered yet, retry later */
    lStdResult = DCM_E_PENDING;
    break;
  }
  return lStdResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc29SubFuncHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29SubFuncHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                 lStdResult;
  Dcm_CfgSvc29SubFuncInfoPtrType pSubFuncInfo;

  lStdResult = Dcm_CfgWrapSvc29SubFuncInfoGetEntry(pRepContext->SubSvcRef, &pSubFuncInfo, ErrorCode);                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)                                                                                                                        /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    lStdResult = pSubFuncInfo->SubSvcFunc(pContext, opStatus, pMsgContext, ErrorCode);                                                               /* SBSW_DCM_CALL_FUNCPTR_SVC29SUBFUNC */
  }

  return lStdResult;
}

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29KeyMSetCertificate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29KeyMSetCertificate(
  KeyM_CertificateIdType certId,
  P2CONST(KeyM_CertDataType, AUTOMATIC, DCM_APPL_DATA) pCertificateData,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  /* Attempt to store cerificate */
  Std_ReturnType lKeyMResult = KeyM_SetCertificate(certId, pCertificateData);                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* Evalute result and respond accordingly */
  switch(lKeyMResult)
  {
  case E_OK:
    lStdResult = DCM_E_OK;
    break;
  case KEYM_E_KEY_CERT_SIZE_MISMATCH:
    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  default:
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    break;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29KeyMVerifyCertificate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29KeyMVerifyCertificate(
  Dcm_ContextPtrType pContext,
  KeyM_CertificateIdType certId,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  Std_ReturnType lKeyMResult;

  /* Set callback processing state to verify certificate */
  Dcm_Svc29SetCbkProcessingState(DCM_SVC29_CBK_PROC_STATE_VERIFY_CERT);

  /* Attempt to start async job to verify the certificate */
  lKeyMResult = KeyM_VerifyCertificate(certId);

  /* Evaluate result and respond accordingly */
  switch(lKeyMResult)
  {
  case E_OK:
    /* If async job was accepted change progress state */
    Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_WAITCERTVERIFY);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_PENDING;
    break;
  case KEYM_E_BUSY:
    *ErrorCode = DCM_E_BUSYREPEATREQUEST;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  default:
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    break;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29GeneralNrcCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc29GeneralNrcCheck(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
#  if (DCM_MODE_AUTH_GENERAL_NRC_ENABLED == STD_ON)
  Dcm_NegativeResponseCodeType lNrcIgnore;
#  endif

  /* Check if the error pertains to Certificate Verification Failed */
  switch(*ErrorCode)
  {
    case DCM_E_CVF_INVALIDTIMEPERIOD:
    case DCM_E_CVF_INVALIDSIGNATURE:
    case DCM_E_CVF_INVALIDCHAINOFTRUST:
    case DCM_E_CVF_INVALIDTYPE:
    case DCM_E_CVF_INVALIDFORMAT:
    case DCM_E_CVF_INVALIDCONTENT:
    case DCM_E_CVF_INVALIDSCOPE:
    case DCM_E_CVF_INVALIDCERTIFICATEREVOKED:
#  if (DCM_MODE_AUTH_GENERAL_NRC_ENABLED == STD_ON)
      /* If mode rule enables General NRC, override Error Code to General NRC */
      if(Dcm_ModeCheck(DCM_MODE_AUTH_SEND_GENERAL_NRC_MODERULE_REF, &lNrcIgnore) == DCM_E_OK)                                                        /* SBSW_DCM_POINTER_FORWARD_STACK */
      {
        *ErrorCode = DCM_AUTHMGR_GENERAL_NRC;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
      }
#  endif
     /* fall through */
    case DCM_E_OWNERSHIPVERIFICATIONFAILED:                                                                                                          /* PRQA S 2003 */ /* MD_Dcm_Optimize_2003 */
      /* Set IdsM security event */
      Dcm_DebugReportSecurityEvent(pContext->ThreadId, DCM_DEBUG_SEV_CERTIFICATE_FAILURE);
      break;

    default:
      /* Otherwise - proceed with negative response code */
    break;
  }
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29FinalizeSequenceStartReq()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29FinalizeSequenceStartReq(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  /* If response buffer can hold the EPK length field */
  if(pMsgContext->resMaxDataLen >= DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN)
  {
    /* Append lengthOfEphemeralPublicKeyServer bytes(uint16) on response */
    Dcm_UtiProvideResDataAsU16(pMsgContext, 0x00u);                                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Get challenge length from request */
    Dcm_Svc29SetLengthChallengeSwapBuffer(Dcm_UtiGetResDataAsU16At(pMsgContext, DCM_SVC29_CHALLENGE_LEN_INDEX));

    /* Copy challenge data from response buffer to special challenge buffer (needed for subfunction 0x03) */
    Dcm_UtiMemCopySafe(Dcm_UtiGetResDataAt(pMsgContext, DCM_SVC29_CHALLENGE_DATA_START_INDEX)
                      ,Dcm_Svc29SingletonContext.ChallengeSwapBuffer
                      ,0u
                      ,DCM_AUTHMGR_CHALLENGE_MAX_SIZE
                      ,Dcm_Svc29GetLengthChallengeSwapBuffer());                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_POINTER_WRITE_CHALLENGE_SWAP_BUFFER */
  }
  else
  {
    /* Otherwise, send NRC 0x14 (DCM_E_RESPONSETOOLONG) */
    *ErrorCode = DCM_E_RESPONSETOOLONG;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_29_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29GetServerCertificate()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29GetServerCertificate(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;
  KeyM_CertDataType lEcuCertificate;
  Std_ReturnType lKeyMResult;

  /* Set location to write certificate data */
  lEcuCertificate.certData = Dcm_UtiGetResDataRel(pMsgContext, DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN);                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
  /* Provide KeyM_GetCertificate with all available buffer length (without 2Bytes for the lengthField) */
  lEcuCertificate.certDataLength = pMsgContext->resMaxDataLen - DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN;

  lKeyMResult = KeyM_GetCertificate(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->EcuCertificateId
                                   ,&lEcuCertificate);                                                                                               /* SBSW_DCM_POINTER_WRITE_CERTIFICATE_DATA */


  /* If result is successful continue processing */
  if(lKeyMResult == DCM_E_OK)
  {
    /* Provide size of server certificate in response */
    Dcm_UtiProvideResDataAsU16(pMsgContext, (uint16)lEcuCertificate.certDataLength);                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
    /* Notify message context about server certificate data written from KeyM_GetCertificate */
    Dcm_UtiProvideResData(pMsgContext, lEcuCertificate.certDataLength);                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else if(lKeyMResult == KEYM_E_KEY_CERT_SIZE_MISMATCH)
  {
    *ErrorCode = DCM_E_RESPONSETOOLONG;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29ValidateCsmCbkResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29ValidateCsmCbkResult(
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;

  /* Switch according to Csm callback result and set NRC */
  switch(Dcm_Svc29SingletonContext.CsmCbkResult)
  {
  case E_OK:
    lStdResult = DCM_E_OK;
    break;
  case E_SMALL_BUFFER:
    *ErrorCode = DCM_E_RESPONSETOOLONG;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  default:
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    break;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29ValidateKeyMCbkResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29ValidateKeyMCbkResult(
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;

  /* Switch according to Key Manager callback result and set NRC if required */
  switch(Dcm_Svc29SingletonContext.KeyMCbkResult)
  {
  case KEYM_CERTIFICATE_VALID:
    lStdResult = DCM_E_OK;
    break;
  case KEYM_E_CERTIFICATE_SIGNATURE_FAIL:
    *ErrorCode = DCM_E_CVF_INVALIDSIGNATURE;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  case KEYM_E_CERTIFICATE_INVALID_CHAIN_OF_TRUST:
    *ErrorCode = DCM_E_CVF_INVALIDCHAINOFTRUST;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  case KEYM_E_CERTIFICATE_INVALID_TYPE:
    *ErrorCode = DCM_E_CVF_INVALIDTYPE;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  case KEYM_E_CERTIFICATE_INVALID_FORMAT:
    *ErrorCode = DCM_E_CVF_INVALIDFORMAT;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  case KEYM_E_CERTIFICATE_INVALID_CONTENT:
    *ErrorCode = DCM_E_CVF_INVALIDSCOPE;                                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  case KEYM_E_CERTIFICATE_REVOKED:
    *ErrorCode = DCM_E_CVF_INVALIDCERTIFICATEREVOKED;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  case KEYM_E_CERTIFICATE_VALIDITY_PERIOD_FAIL:
    *ErrorCode = DCM_E_CVF_INVALIDTIMEPERIOD;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  default:
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    break;
  }

  return lStdResult;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29CertificateStoreVerify()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29CertificateStoreVerify(
  uint16 clientCertId,
  uint16 clientCertLength,
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  KeyM_CertDataType lClientCertificate;

  /* Create certificate structure from request to be passed on to Key Manager */
  lClientCertificate.certDataLength = (uint32)clientCertLength;
  lClientCertificate.certData = Dcm_UtiGetReqData(pMsgContext);                                                                                      /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

  /* Store CertId in Dcm_SingletonContext to process when KeyM is finished */
  Dcm_Svc29SetPendingKeyMCertId(clientCertId);

  /* Store certificate on Key Manager and get operation result */
  lStdResult = Dcm_Svc29KeyMSetCertificate(Dcm_Svc29GetPendingKeyMCertId(), &lClientCertificate, ErrorCode);                                         /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)
  {
    /* If certificate storage was sucessfull attemp to verify */
    lStdResult = Dcm_Svc29KeyMVerifyCertificate(pContext, Dcm_Svc29GetPendingKeyMCertId(), ErrorCode);                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
  }/* Otherwise, send the already set NRC */

  return lStdResult;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29CertificateVerificationDoneHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
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
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29CertificateVerificationDoneHandler(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Check for callback result of KeyM */
  lStdResult = Dcm_Svc29ValidateKeyMCbkResult(ErrorCode);                                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)
  {
    switch (pRepContext->SubSvcId)
    {
#  if (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
      case DCM_SVC29_SUBFUNC_TRANSMIT_CERTIFICATE:
        Dcm_UtiProvideResDataAsU8(pMsgContext, DCM_SVC29_CERTIFICATE_VERIFIED);                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
#   if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
        if (Dcm_CfgWrapAuthMgrTransmitCertificateInfo(pRepContext->ClientCertIdx)->Function == DCM_AUTHMGR_CERT_FUNCTION_CODING)
        {
          Dcm_DidMgrSecureCodingStart();
        }
#   endif
        break;
#  endif
#  if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
      case DCM_SVC29_SUBFUNC_VERIFY_CERT_UNIDIR:
      case DCM_SVC29_SUBFUNC_VERIFY_CERT_BIDIR:
        Dcm_UtiProvideResDataAsU8(pMsgContext, DCM_SVC29_OWNERSHIP_VERIFICATION_NECESSARY);                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
        /* Generate the server challenge */
        lStdResult = Dcm_Svc29GenerateServerChallenge(pContext, pMsgContext, ErrorCode, pRepContext);                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
        break;
#  endif
      default:                                                                                                                                       /* COV_DCM_RTM_UNREACHABLE X */
        lStdResult = DCM_E_NOT_OK;
        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
        break;
    }
  } /* else ErrorCode already set */

  return lStdResult;
}
# endif


# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29GenerateServerChallenge()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29GenerateServerChallenge(
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;
  Std_ReturnType lCsmResult;

  Dcm_Svc29SetPendingCsmJobId(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->RandomJobId);
  Dcm_Svc29SetLengthAsyncCbk(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->EcuChallengeLength);

  /* Provide size of server challenge in response */
  Dcm_UtiProvideResDataAsU16(pMsgContext, (uint16)Dcm_Svc29GetLengthAsyncCbk());                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

  lStdResult = Dcm_UtiMsgContextVerifySize(pMsgContext, Dcm_Svc29GetLengthAsyncCbk(), ErrorCode);                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* Check if buffer can hold the server challenge */
  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    /* Set callback processing state to generate challenge */
    Dcm_Svc29SetCbkProcessingState(DCM_SVC29_CBK_PROC_STATE_GENERATE_CHALLENGE);

    /* Start asynchronous challenge generation */
    lCsmResult = Csm_RandomGenerate(Dcm_Svc29GetPendingCsmJobId()
                                   ,Dcm_UtiGetResData(pMsgContext)                                                                                   /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
                                   ,&Dcm_Svc29GetLengthAsyncCbk());                                                                                  /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

    if(lCsmResult == DCM_E_OK)
    {
      /* If async job was accepted change progress state */
      Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_WAITCHALLENGEGEN);                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_PENDING;
    }
    /* If async job was rejected send NRC and dont loop */
    else if(lCsmResult == CRYPTO_E_BUSY)
    {
      *ErrorCode = DCM_E_BUSYREPEATREQUEST;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
    else
    {
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    }
  }
  return lStdResult;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29ChallengeGenerationDoneHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
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
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29ChallengeGenerationDoneHandler(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Validate the callback result */
  lStdResult = Dcm_Svc29ValidateCsmCbkResult(ErrorCode);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* Check if ecu challenge length copied succesfully */
  if(lStdResult == DCM_E_OK)
  {
    /* Check if expected challenge length is generated */
    if(Dcm_DebugDetectRuntimeError(Dcm_Svc29GetLengthAsyncCbk() != Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->EcuChallengeLength))     /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK XF */
    {
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
    else
    {
      /* Notify message context about server challenge data written from previous async operation */
      Dcm_UtiProvideResData(pMsgContext, Dcm_Svc29GetLengthAsyncCbk());                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */

      switch(pRepContext->SubSvcId)
      {
#  if (DCM_SVC_29_01_SUPPORT_ENABLED == STD_ON)
        case DCM_SVC29_SUBFUNC_VERIFY_CERT_UNIDIR:
          lStdResult = Dcm_Svc29FinalizeSequenceStartReq(pMsgContext, ErrorCode);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
          break;
#  endif
#  if (DCM_SVC_29_02_SUPPORT_ENABLED == STD_ON)
        case DCM_SVC29_SUBFUNC_VERIFY_CERT_BIDIR:
          lStdResult = Dcm_Svc29GetServerCertificate(pMsgContext, ErrorCode, pRepContext);                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

          if(lStdResult == DCM_E_OK)
          {
            lStdResult = Dcm_Svc29GenerateSignature(pContext, pMsgContext, ErrorCode, pRepContext);                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
          } /* else ErrorCode already set */
          break;
#  endif
        default:
          *ErrorCode = DCM_E_PANIC_NRC;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
          Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
          break;
      }
    }
  } /* else ErrorCode already set */

  return lStdResult;
}
# endif

# if (DCM_SVC_29_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29GenerateSignature()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29GenerateSignature(
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;
  Std_ReturnType lCsmResult;

  /* Check if response buffer can hold the proof of ownership length field */
  if (pMsgContext->resMaxDataLen > DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN)
  {
    Dcm_Svc29SetPendingCsmJobId(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->SignClientChallengeJobId);
    /* Provide Csm_SignatureGenerate with all available buffer length (without 2Bytes for the lengthField) */
    Dcm_Svc29SetLengthAsyncCbk((pMsgContext->resMaxDataLen - DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN));

    Dcm_Svc29SetCbkProcessingState(DCM_SVC29_CBK_PROC_STATE_SIGN_CHALLENGE);

    /* Sign client challenge */
    lCsmResult = Csm_SignatureGenerate(Dcm_Svc29GetPendingCsmJobId()
                                      ,CRYPTO_OPERATIONMODE_SINGLECALL
                                      ,Dcm_Svc29SingletonContext.ChallengeSwapBuffer
                                      ,(uint32)Dcm_Svc29GetLengthChallengeSwapBuffer()
                                      ,Dcm_UtiGetResDataRel(pMsgContext, DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN)                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
                                      ,&Dcm_Svc29GetLengthAsyncCbk());                                                                               /* SBSW_DCM_COMB_PTR_FORWARD */ /* SBSW_DCM_POINTER_WRITE_MESSAGE_CONTEXT_RESBUFFER */
    if(lCsmResult == E_OK)
    {
      /* If async job was accepted change progress state and keep looping */
      Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_SUBFUNC02_WAITCHALLENGESIGNED);                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_PENDING;
    }
    /* If async job was rejected send response and dont loop */
    else if(lCsmResult == CRYPTO_E_BUSY)
    {
      *ErrorCode = DCM_E_BUSYREPEATREQUEST;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
    else if(lCsmResult == CRYPTO_E_SMALL_BUFFER)
    {
      *ErrorCode = DCM_E_RESPONSETOOLONG;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
    else
    {
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    }
  }
  /* If res buffer is out of space send NRC and stop looping */
  else
  {
    *ErrorCode = DCM_E_RESPONSETOOLONG;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  return lStdResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

# if (DCM_SVC_29_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29_02ChallengeSigningDoneHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29_02ChallengeSigningDoneHandler(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_Svc29ValidateCsmCbkResult(ErrorCode);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* If callback was successful continue processing: */
  if(lStdResult == DCM_E_OK)
  {
    /* Provide server generated signature data length */
    Dcm_UtiProvideResDataAsU16(pMsgContext, (uint16)Dcm_Svc29GetLengthAsyncCbk());                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    /* Notify message context about server generated signature data written from previous async operation */
    Dcm_UtiProvideResData(pMsgContext, Dcm_Svc29GetLengthAsyncCbk());                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdResult = Dcm_Svc29FinalizeSequenceStartReq(pMsgContext, ErrorCode);                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
  } /* else ErrorCode already set */

  return lStdResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc29ReqLengthGetter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc29ReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  )
{
  /* Return sub-function specific length */
  return Dcm_CfgWrapSvc29SubFuncInfo(sfRef)->ReqLength;
}

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29_03RoleRead()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29_03RoleRead(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{

  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_Svc29ValidateCsmCbkResult(ErrorCode);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* If callback was successful continue processing: */
  if(lStdResult == DCM_E_OK)
  {
  /* Verify the verify pointer from the CSM signature verify job */
    if(Dcm_Svc29GetSignatureVerifyResult() == CRYPTO_E_VER_OK)
    {
      /* Read the role from the certificate via KeyM */
      lStdResult = Dcm_AuthMgrGetCertRole(opStatus, ErrorCode, pRepContext);                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

      /* If role read from certificate was successful */
      if(lStdResult == DCM_E_OK)
      {
        /* Move on to further processing of service 0x29 */
        Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_SUBFUNC03_SERVICEWHITELISTREAD);                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdResult = DCM_E_LOOP;
      }
    }
    else
    {
      /* CSM signature verify job failed, send NRC 0x58 */
      *ErrorCode = DCM_E_OWNERSHIPVERIFICATIONFAILED;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  } /* else ErrorCode already set */

  return lStdResult;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29_03ServiceWL()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29_03ServiceWL(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
  lStdResult = DCM_E_OK;

  /* If white list supported by tester */
  if(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLServicesId != DCM_AUTHMGR_INVALID_WL_KEYM_ELEMENT_ID)
  {
    /* Read elements from Service white list */
    lStdResult = Dcm_AuthMgrServiceWLReadList(pRepContext, ErrorCode);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if(lStdResult == DCM_E_OK)
#  else
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#  endif
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_SUBFUNC03_DIDWHITELISTREAD);                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;
  }

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return lStdResult;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29_03DidWL()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29_03DidWL(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_AUTHMGR_WHITELIST_DID_ENABLED == STD_ON)
  lStdResult = DCM_E_OK;

  /* If white list supported by tester */
  if(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLDidId != DCM_AUTHMGR_INVALID_WL_KEYM_ELEMENT_ID)
  {
    lStdResult = Dcm_AuthMgrDidWLReadList(pRepContext, ErrorCode);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  if(lStdResult == DCM_E_OK)
#  else
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#  endif
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_SUBFUNC03_RIDWHITELISTREAD);                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;
  }

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return lStdResult;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29_03RidWL()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29_03RidWL(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_AUTHMGR_WHITELIST_RID_ENABLED == STD_ON)
  lStdResult = DCM_E_OK;

  /* If white list supported by tester */
  if(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLRidId != DCM_AUTHMGR_INVALID_WL_KEYM_ELEMENT_ID)
  {
    lStdResult = Dcm_AuthMgrRidWLReadList(pRepContext, ErrorCode);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  if(lStdResult == DCM_E_OK)
#  else
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#  endif
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_SUBFUNC03_MEMWHITELISTREAD);                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;
  }

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  return lStdResult;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29_03MemWL()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29_03MemWL(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_AUTHMGR_WHITELIST_MEM_ENABLED == STD_ON)
  lStdResult = DCM_E_OK;

  /* If white list supported by tester */
  if (Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->CertWLMemId != DCM_AUTHMGR_INVALID_WL_KEYM_ELEMENT_ID)
  {
    lStdResult = Dcm_AuthMgrMemWLReadList(pRepContext, ErrorCode);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if (lStdResult == DCM_E_OK)
#  else
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#  endif
  {
    /* Set connection state to authenticated */
    pRepContext->ConnContext.AuthState = DCM_AUTHENTICATED;                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */

    /* Provide authenticationReturnParameter with authentication complete */
    Dcm_UtiProvideResDataAsU8(pMsgContext, DCM_SVC29_OWNERSHIP_VERIFIED_AUTHENTICATION_COMPLETE);                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Provide session key info length field */
    Dcm_UtiProvideResDataAsU16(pMsgContext, (uint16)0x00);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Change to next processing state and loop over */
    Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_PERSIST_AUTHENTICATION);                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29PersistAuthentication()
 *********************************************************************************************************************/
 /*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29PersistAuthentication(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdReturn = DCM_E_OK;                                                                                                              /* PRQA S 2981 */ /* MD_Dcm_RetVal */
#  if (DCM_AUTHMGR_PERSIST_STATE_ENABLED == STD_ON)
  Std_ReturnType lModeCheck = DCM_E_OK;
#  endif

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_AUTHMGR_PERSIST_STATE_ENABLED == STD_ON)
  if (opStatus == DCM_INITIAL)
  {
    /* Write authentication state to NvM if mode rule is activated. */
    lModeCheck = Dcm_ModeCheck(DCM_MODE_AUTH_PERSIST_STATE_MODERULE_REF, ErrorCode);                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if (lModeCheck == DCM_E_OK)
  {
    /* Trigger NvM to write the corresponding block */
    lStdReturn = Dcm_UtiNvMWrite(opStatus
                                ,&pRepContext->ConnContext                                                                                           /* PRQA S 0315 */ /* MD_MSR_Dir1.1 */ /* SBSW_DCM_SVC29_NV_ACCESS */
                                ,Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->NvMBlockId);

    if (lStdReturn == DCM_E_NOT_OK)
    {
      /* NvM storage failed */
      *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }

  if (lStdReturn == DCM_E_OK)
#  endif
  {
#  if(DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON)
    Dcm_AuthMgrWLContextPtrType pConnWLContext;

    lStdReturn = Dcm_AuthMgrGetWLContext(pRepContext->AuthInfoHdl, &pConnWLContext, ErrorCode);                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

    if (lStdReturn == DCM_E_OK)                                                                                                                      /* COV_DCM_RTM_RUNTIME_CHECK TX */
#  endif
    {
#  if(DCM_AUTHMGR_WHITELISTS_ENABLED == STD_ON)
      *pConnWLContext = pRepContext->ConnContext.WLContext;                                                                                          /* SBSW_DCM_POINTER_WRITE_AUTHNVMDATA */
#  endif
      Dcm_AuthMgrUpdateActiveRole(pRepContext->AuthInfoHdl, pRepContext->ConnContext.ActiveRole);
      Dcm_AuthMgrSetAuthentication(pRepContext->AuthInfoHdl, pRepContext->ConnContext.AuthState);

      Dcm_Svc29ResetOwnershipRequestExpected();

      if (pRepContext->ConnContext.AuthState == DCM_AUTHENTICATED)
      {
        Dcm_DebugReportSecurityEvent(pMsgContext->threadId, DCM_DEBUG_SEV_AUTHENTICATION_SUCCESSFUL);
      }
    }
  }

  return lStdReturn;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc29VerifySignature()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29VerifySignature(
  uint16 proofOfOwnershipLength,
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  Std_ReturnType lCsmResult;

  Dcm_Svc29RepeaterProxyContextConstPtrType pRepContext = &pContext->Repeater.Context.Svc29;

  Dcm_Svc29SetPendingCsmJobId(Dcm_CfgWrapAuthMgrNetConnInfo(pRepContext->AuthInfoHdl)->ProofOfOwnershipJobId);

  Dcm_Svc29SetSignatureVerifyResult(CRYPTO_E_VER_NOT_OK);

  /* Set callback processing state to verify signature */
  Dcm_Svc29SetCbkProcessingState(DCM_SVC29_CBK_PROC_STATE_VERIFY_POOW);
  /* Trigger the Csm signature verify job */
  lCsmResult = Csm_SignatureVerify(Dcm_Svc29GetPendingCsmJobId()
                                  ,CRYPTO_OPERATIONMODE_SINGLECALL
                                  ,Dcm_Svc29SingletonContext.ChallengeSwapBuffer
                                  ,Dcm_Svc29GetLengthChallengeSwapBuffer()
                                  ,Dcm_UtiGetReqData(pMsgContext)                                                                                    /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                  ,(uint32)proofOfOwnershipLength
                                  ,&Dcm_Svc29GetSignatureVerifyResult());                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_COMB_PTR_FORWARD */

  switch(lCsmResult)
  {
  case E_OK:
    Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_SUBFUNC03_WAITSIGVERIFY);                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_PENDING;
    break;
  case CRYPTO_E_BUSY:
    *ErrorCode = DCM_E_BUSYREPEATREQUEST;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  case CRYPTO_E_SMALL_BUFFER:
    *ErrorCode = DCM_E_RESPONSETOOLONG;                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  case CRYPTO_E_KEY_NOT_VALID:
    *ErrorCode = DCM_E_OWNERSHIPVERIFICATIONFAILED;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    break;
  default:
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    break;
  }
  return lStdResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc29SubFuncSequenceChecker()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc29SubFuncSequenceChecker(
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DiagSubServiceRefOptType subSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  DCM_IGNORE_UNREF_PARAM(subSvcRef);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if(Dcm_UtiGetReqDataAsU8(pMsgContext) == DCM_SVC29_SUBFUNC_PROOF_OF_OWNERSHIP)                                                                     /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
  {
    if(Dcm_Svc29IsOwnershipRequestExpected())
    {
      if(Dcm_NetGetConnHdlOfRxPduId(Dcm_Svc29GetRxPduId()) != Dcm_NetGetConnHdlOfRxPduId(pMsgContext->rxPduId))
      {
        /* This causes NRC 0x22 to be sent back */
        lStdResult = DCM_E_NOT_OK;
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
      }/* else return DCM_E_OK */
    }
    else
    {
      /* This causes NRC 24 to be sent back. Wrong sequence */
      lStdResult = DCM_E_NOT_OK;
      *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }/* else return DCM_E_OK */

  return lStdResult;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_KeyMAsyncCertificateVerifyFinished()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
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
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_KeyMAsyncCertificateVerifyFinished(
  KeyM_CertificateIdType CertId,
  KeyM_CertificateStatusType Result
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))                                                                                          /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_Svc29GetPendingKeyMCertId() != CertId))                                                                           /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
    BEGIN CRITICAL SECTION
    *=================================*/
    if(Dcm_Svc29IsAsyncKeyMJobActive())
    {
      Dcm_Svc29SingletonContext.KeyMCbkResult = Result;
      Dcm_Svc29SetNextCbkProcessingState();
    }
    else if(Dcm_Svc29IsCbkStateActive(DCM_SVC29_CBK_PROC_STATE_KEYM_CANCELLED))
    {
      /* Reset callback processing state */
      Dcm_Svc29SetCbkProcessingState(DCM_SVC29_CBK_PROC_STATE_IDLE);
    }
    else
    {
      /* Callback is not expected */
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    }
    /*=================================*
    END CRITICAL SECTION
    *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_KEYMASYNCCERTIFICATEVERIFYFINISHED, lErrorId);
  return DCM_E_OK;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_CsmAsyncJobFinished()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
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
 */
FUNC(void, DCM_CODE) Dcm_CsmAsyncJobFinished(
  P2CONST(Crypto_JobType, AUTOMATIC, DCM_APPL_DATA) job,
  Crypto_ResultType result
  )
{
  uint8 lErrorId = DCM_E_NO_ERROR;

  /* ----- Development Error Checks ---------------------------------------- */
  if(Dcm_DebugDetectError(!Dcm_DebugIsModuleInitialized()))                                                                                          /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    lErrorId = DCM_E_UNINIT;
  }
  else if(Dcm_DebugDetectError(Dcm_Svc29GetPendingCsmJobId() != job->jobId))                                                                         /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    lErrorId = DCM_E_PARAM;
  }
  else
  {
    /* ----- Implementation ------------------------------------------------ */
    Dcm_UtiEnterCriticalSection();
    /*=================================*
      BEGIN CRITICAL SECTION
     *=================================*/
    switch(Dcm_Svc29GetCbkProcessingState())
    {
    case DCM_SVC29_CBK_PROC_STATE_GENERATE_CHALLENGE:
    case DCM_SVC29_CBK_PROC_STATE_SIGN_CHALLENGE:
    case DCM_SVC29_CBK_PROC_STATE_VERIFY_POOW:
      Dcm_Svc29SingletonContext.CsmCbkResult = result;
      Dcm_Svc29SetNextCbkProcessingState();
      break;
    case DCM_SVC29_CBK_PROC_STATE_CSM_CANCELLED:
      /* Remove service 0x29 as user of the buffer since the Csm callback is triggered and released the buffer */
      Dcm_NetBufferUsageLeave(Dcm_Svc29GetBufferHdl());
      /* Reset callback processing state */
      Dcm_Svc29SetCbkProcessingState(DCM_SVC29_CBK_PROC_STATE_IDLE);
      break;
    default:                                                                                                                                         /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
      /* Callback is not expected */
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
      break;
    }
    /*=================================*
      END CRITICAL SECTION
     *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_CSMASYNCJOBFINISHED, lErrorId);
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Service29Init()
*********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Service29Init(
  void
  )
{
  Dcm_Svc29ResetOwnershipRequestExpected();
# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc29SetCbkProcessingState(DCM_SVC29_CBK_PROC_STATE_IDLE);
# endif
}

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service29_00Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service29_00Processor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc29;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Set connection state to deauthenticated */
  pRepContext->ConnContext.AuthState = DCM_DEAUTHENTICATED;                                                                                          /* SBSW_DCM_GLOBAL_PTR_WRITE */
  pRepContext->ConnContext.ActiveRole = DCM_AUTHMGR_DEAUTHENTICATED_ROLE;                                                                            /* SBSW_DCM_GLOBAL_PTR_WRITE */

  /* Set the authenticationReturnParameter of the response to DeAuthentication successful */
  Dcm_UtiProvideResDataAsU8(pMsgContext, DCM_SVC29_DEAUTH_SUCCESSFUL);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
  Dcm_RepeaterNextStep(pContext, DCM_SVC29_PROGRESS_PERSIST_AUTHENTICATION);                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

  return DCM_E_LOOP;
}
# endif

# if (DCM_SVC_29_01_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service29_01Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service29_01Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  boolean        lIsReqLenValid = FALSE;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Check that request contains COCO(1Byte) and length field for client certificate */
  if(pMsgContext->reqDataLen > (1u + DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN))
  {
    uint8 lCocoField;
    uint16 lClientCertLength;

    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lCocoField);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &lClientCertLength);                                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */

    /* Check that request contains length field for client challenge */
    if( (lClientCertLength > 0u)
      &&(pMsgContext->reqDataLen == (lClientCertLength + DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN)) )
    {
      uint16 lChallengeClientLength = Dcm_UtiGetReqDataAsU16Rel(pMsgContext, lClientCertLength);                                                     /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

      /* Check if length of challenge client is 0 */
      if(lChallengeClientLength == 0u)
      {
        lIsReqLenValid = TRUE;

        /* If COCO field equals 0x00 start request processing */
        if (lCocoField == 0u)
        {
          uint16 lClientCertId = Dcm_CfgWrapAuthMgrNetConnInfo(pContext->Repeater.Context.Svc29.AuthInfoHdl)->ConnectionCertificateId;
          lStdResult = Dcm_Svc29CertificateStoreVerify(lClientCertId, lClientCertLength, pContext, pMsgContext, ErrorCode);                          /* SBSW_DCM_PARAM_PTR_FORWARD */
        }
        /* If COCO field doesnt equal 0x00 send NRC 0x31 and stop looping */
        else
        {
          *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
      }
    }
  }

  if(lIsReqLenValid == FALSE)
  {
    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_29_02_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service29_02Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
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
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service29_02Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  boolean        lIsReqLenValid = FALSE;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Check that request contains COCO(1Byte) and length field for client certificate */
  if(pMsgContext->reqDataLen > (1u + DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN))
  {
    uint8  lCocoField;
    uint16 lClientCertLength;

    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lCocoField);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &lClientCertLength);                                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */

    /* Check that request contains length field for client challenge */
    if( (lClientCertLength > 0u)
      &&(pMsgContext->reqDataLen > (lClientCertLength + DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN)) )
    {
      uint16 lChallengeClientLength = Dcm_UtiGetReqDataAsU16Rel(pMsgContext, lClientCertLength);                                                     /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

      /* Check if challenge client is valid and the request length is as expected */
      if( (lChallengeClientLength > 0u)
        &&(lChallengeClientLength <= DCM_AUTHMGR_CHALLENGE_MAX_SIZE)
        &&(pMsgContext->reqDataLen == (lClientCertLength + DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN + lChallengeClientLength)) )
      {
        lIsReqLenValid = TRUE;

        /* Copy client challenge in the special buffer */
        Dcm_UtiMemCopySafe(Dcm_UtiGetReqDataRel(pMsgContext, lClientCertLength + DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN)
                          ,Dcm_Svc29SingletonContext.ChallengeSwapBuffer
                          ,0
                          ,DCM_AUTHMGR_CHALLENGE_MAX_SIZE
                          ,lChallengeClientLength);                                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_POINTER_WRITE_CHALLENGE_SWAP_BUFFER */

        Dcm_Svc29SetLengthChallengeSwapBuffer(lChallengeClientLength);

        /* If COCO field equals 0x00 start request processing */
        if (lCocoField == 0u)
        {
          uint16 lClientCertId = Dcm_CfgWrapAuthMgrNetConnInfo(pContext->Repeater.Context.Svc29.AuthInfoHdl)->ConnectionCertificateId;
          lStdResult = Dcm_Svc29CertificateStoreVerify(lClientCertId, lClientCertLength, pContext, pMsgContext, ErrorCode);                          /* SBSW_DCM_PARAM_PTR_FORWARD */
        }
        /* If COCO field doesnt equal 0x00 send NRC 0x31 and stop looping */
        else
        {
          *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
      }
    }
  }

  if(lIsReqLenValid == FALSE)
  {
    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service29_03Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service29_03Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  boolean lIsReqLenValid = FALSE;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Check that request contains length field for proof of ownership */
  if(pMsgContext->reqDataLen > DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN)
  {
    uint16 lProofOfOwnershipLength;
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &lProofOfOwnershipLength);                                                                               /* SBSW_DCM_COMB_PTR_FORWARD */

    /* Check that request contains length field for the ephemeral public key */
    if( (lProofOfOwnershipLength > 0u)
      &&(pMsgContext->reqDataLen == (lProofOfOwnershipLength + DCM_SVC29_REQ_LENGTH_FIELD_BYTELEN)) )
    {
      uint16 lLengthEPK = Dcm_UtiGetReqDataAsU16Rel(pMsgContext, lProofOfOwnershipLength);                                                           /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

      /* Only an ephemeral public key length of 0 is supported */
      if(lLengthEPK == 0u)
      {
        lIsReqLenValid = TRUE;
        lStdResult = Dcm_Svc29VerifySignature(lProofOfOwnershipLength, pContext, pMsgContext, ErrorCode);                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
    }
  }

  if(lIsReqLenValid == FALSE)
  {
    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service29_04Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
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
 */

FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service29_04Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc29;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if (pMsgContext->reqDataLen >= 5u)
  {
    uint16        lClientCertLength = 0u;
    uint16        lCertEvaluationId = 0u;
    sint16_least  lResult;

    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &lCertEvaluationId);                                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &lClientCertLength);                                                                                     /* SBSW_DCM_COMB_PTR_FORWARD */

    /* Check that request contains complete certificate */
    if (pMsgContext->reqDataLen == lClientCertLength)
    {
      /* Check that request contains supported certificateEvaluationId(2Byte) */
      lResult = Dcm_UtiLookUpUint16(Dcm_CfgAuthMgrEvaluationIdLookUpTable, lCertEvaluationId);                                                       /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

      if (lResult >= 0)
      {
        Dcm_CfgAuthMgrTransmitCertificateInfoPtrType pCertInfo = Dcm_CfgWrapAuthMgrTransmitCertificateInfo(lResult);

        pRepContext->ClientCertIdx = (uint8)lResult;                                                                                                 /* SBSW_DCM_GLOBAL_PTR_WRITE */

#  if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
        if (Dcm_DidMgrSecureCodingIsBufferLocked())
        {
          *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
        }
        else
#  endif
        {
          lStdResult = Dcm_Svc29CertificateStoreVerify(pCertInfo->ClientCertId
                                                      ,lClientCertLength
                                                      ,pContext
                                                      ,pMsgContext
                                                      ,ErrorCode);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
        }
      }
      else
      {
        *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
    }
    else
    {
      *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }
  else
  {
    *ErrorCode = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }

  return lStdResult;
}
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service29_08Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service29_08Processor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Set the authenticationReturnParameter of the response to Authentication Configuration APCE */
  Dcm_UtiProvideResDataAsU8(pMsgContext, DCM_SVC29_AUTH_CONFIG_APCE);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

  return DCM_E_OK;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Service29Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service29Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  Dcm_Svc29RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc29;

  /* Proccess according to stage */
  switch (Dcm_RepeaterGetProgress(pContext))
  {
    /* Verifies subfunction validity e.g length and sequence */
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc29Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
    /* Calls initial subfunction processor function */
  case DCM_SVC29_PROGRESS_SUBFUNCTION:
    lStdResult = Dcm_Svc29SubFuncHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                   /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
    /* Evaluates certificate verification result if positive starts challenge generation common to 0x01 and 0x02 */
  case DCM_SVC29_PROGRESS_CERTVERIFIED:
    lStdResult = Dcm_Svc29CertificateVerificationDoneHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                               /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
    /* Evaluate challenge generation result if positive starts signing of client challenge for subfunction 0x02 */
  case DCM_SVC29_PROGRESS_CHALLENGEGEN:
    lStdResult = Dcm_Svc29ChallengeGenerationDoneHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                   /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
#  if (DCM_SVC_29_02_SUPPORT_ENABLED == STD_ON)
    /* Evaluates challenge signing result if positive sends response for subfunction 0x02 */
  case DCM_SVC29_PROGRESS_SUBFUNC02_CHALLENGESIGNED:
    lStdResult = Dcm_Svc29_02ChallengeSigningDoneHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                   /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
#  endif
  case DCM_SVC29_PROGRESS_SUBFUNC03_ROLEREAD:
    lStdResult = Dcm_Svc29_03RoleRead(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC29_PROGRESS_SUBFUNC03_SERVICEWHITELISTREAD:
    lStdResult = Dcm_Svc29_03ServiceWL(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                     /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC29_PROGRESS_SUBFUNC03_DIDWHITELISTREAD:
    lStdResult = Dcm_Svc29_03DidWL(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC29_PROGRESS_SUBFUNC03_RIDWHITELISTREAD:
    lStdResult = Dcm_Svc29_03RidWL(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC29_PROGRESS_SUBFUNC03_MEMWHITELISTREAD:
    lStdResult = Dcm_Svc29_03MemWL(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                         /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC29_PROGRESS_PERSIST_AUTHENTICATION:
    lStdResult = Dcm_Svc29PersistAuthentication(opStatus, pMsgContext, ErrorCode, pRepContext);                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
    /* On pending stages just loop */
  case DCM_SVC29_PROGRESS_SUBFUNC03_WAITSIGVERIFY:
  case DCM_SVC29_PROGRESS_WAITCHALLENGEGEN:
#  if (DCM_SVC_29_02_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC29_PROGRESS_SUBFUNC02_WAITCHALLENGESIGNED:
#  endif
# endif
# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
  case DCM_SVC29_PROGRESS_WAITCERTVERIFY:
    lStdResult = Dcm_Svc29CbkResultDispatcher(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                              /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
  /* If processing result is a fail */
  if(lStdResult == DCM_E_NOT_OK)
  {
    /* Check if NRC is a Certificate Verification Failed NRC and change to General NRC if mode rule allows it */
    Dcm_Svc29GeneralNrcCheck(pContext, ErrorCode);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
# endif
  return lStdResult;
}                                                                                                                                                    /* PRQA S 6030, 6050 */ /* MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Dcm_Service29PostProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
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
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service29PostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_ConfirmationStatusType status
  )
{
  Dcm_Svc29RepeaterProxyContextConstPtrType pRepContext = &pContext->Repeater.Context.Svc29;

  /* If a positive response was successfully sent */
  if(status == DCM_RES_POS_OK)
  {
    /* If it was a unidirectional or bidirectional verify request: */
    if( (pRepContext->SubSvcId == DCM_SVC29_SUBFUNC_VERIFY_CERT_UNIDIR)
      ||(pRepContext->SubSvcId == DCM_SVC29_SUBFUNC_VERIFY_CERT_BIDIR) )
    {
      /* Set sequence flag for request 0x03 */
      Dcm_Svc29SetOwnershipRequestExpected();
    }
  }
  else if((DCM_DIAG_RES_NEG_ANY & status) != 0u) /* Otherwise, if any negative response was sent: */
  {
    /* Reset the sequence */
# if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
    if(pRepContext->SubSvcId != DCM_SVC29_SUBFUNC_TRANSMIT_CERTIFICATE)
# endif
    {
      Dcm_Svc29ResetOwnershipRequestExpected();
    }
  }
  else /* POS_NOK */
  {
    /* Otherwise no change required */
  }

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
  /* Only set state back to idle if request processing is not cancelled */
  if( (!Dcm_Svc29IsCbkStateActive(DCM_SVC29_CBK_PROC_STATE_CSM_CANCELLED))
    &&(!Dcm_Svc29IsCbkStateActive(DCM_SVC29_CBK_PROC_STATE_KEYM_CANCELLED)) )
  {
    /* Set callback processing state to idle */
    Dcm_Svc29SetCbkProcessingState(DCM_SVC29_CBK_PROC_STATE_IDLE);
  }
# endif
}

/**********************************************************************************************************************
 *  Dcm_Service29Cancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
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
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Service29Cancel(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
  Std_ReturnType lStdResult = DCM_E_OK;
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
  if(Dcm_Svc29IsAsyncCsmJobActive())
  {
    lStdResult = Csm_CancelJob(Dcm_Svc29GetPendingCsmJobId(), CRYPTO_OPERATIONMODE_SINGLECALL);
  }
# endif

# if (DCM_SVC_29_APCE_INTERNAL == STD_ON) || (DCM_SVC_29_04_SUPPORT_ENABLED == STD_ON)
  Dcm_UtiEnterCriticalSection();
  /*=================================*
  BEGIN CRITICAL SECTION
  *=================================*/
  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
  {
    if(Dcm_Svc29IsAsyncKeyMJobActive())
    {
      /* No user to the buffer needs to be added, since buffer is not locked by KeyM */
      Dcm_Svc29SetCbkProcessingState(DCM_SVC29_CBK_PROC_STATE_KEYM_CANCELLED);
    }
#  if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
    else
    {
      /* Csm job is cancelled, reset the callback processing state to idle */
      Dcm_Svc29SetCbkProcessingState(DCM_SVC29_CBK_PROC_STATE_IDLE);
    }
#  endif
  }
#  if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
  else
  {
    /* Recheck if any Csm job is still active to assure that the callback is not triggered in the meantime */
    if(Dcm_Svc29IsAsyncCsmJobActive())
    {
      /* Cancellation of Csm job not possible, wait for callback */
      Dcm_Svc29SetCbkProcessingState(DCM_SVC29_CBK_PROC_STATE_CSM_CANCELLED);
      /* Add service 0x29 as user of the buffer to prevent releasing of the buffer since asynchronous Csm job has still locked the buffer */
      Dcm_NetBufferUsageEnter(Dcm_Svc29GetBufferHdl());                                                                                              /* PRQA S 2840, 3387 */ /* MD_Dcm_DerefInvalidPointer, MD_Dcm_OptimizedInlineCode_3387 */ /* SBSW_DCM_POINTER_WRITE_BUFFERCONTEXT */
    }
    else
    {
      /* Callback is triggered in the meantime, reset the callback processing state to idle */
      Dcm_Svc29SetCbkProcessingState(DCM_SVC29_CBK_PROC_STATE_IDLE);
    }
  }
#  endif
  /*=================================*
  END CRITICAL SECTION
  *=================================*/
  Dcm_UtiLeaveCriticalSection();
# endif
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Reset request sequence */
  Dcm_Svc29ResetOwnershipRequestExpected();
}
# if (DCM_SVC_29_APCE_INTERNAL == STD_ON)
/**********************************************************************************************************************
 *  Dcm_AuthMgrModeSwitchAuthentication()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_AuthMgrModeSwitchAuthentication(
  Dcm_CfgNetAuthInfoRefMemType authInfoRef,
  uint8 mode
  )
{
  if (Dcm_DebugDetectRuntimeError(authInfoRef >= DCM_CFGAUTHMGRNETCONNINFO_SIZE))                                                                    /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
  {
    (void)Dcm_CfgWrapAuthMgrNetConnInfo(authInfoRef)->AuthStateModeSwitchOpFunc(mode);                                                               /* SBSW_DCM_CALL_FUNCPTR_AUTHMODESWITCH */
  }
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_29_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc29.c
 *********************************************************************************************************************/
