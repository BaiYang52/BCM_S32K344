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
/**        \file  Dcm_RidMgr.c
 *         \unit  RidMgr
 *        \brief  Contains the implementation of RID Manager unit.
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
#define DCM_RIDMGR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_RidMgr.h"
#if (DCM_RIDMGR_SUPPORT_ENABLED == STD_ON)
# include "Dcm_ObdIdMgr.h"
# include "Dcm_Uti.h"
# include "Dcm_State.h"
# include "Dcm_Debug.h"
# include "Dcm_CfgWrap.h"
# include "Dcm_DidMgr.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
/*! Secure Coding Validation states */
#  define DCM_RIDMGR_SECURE_CODING_STATE_IDLE                        ((Dcm_RepeaterProgressType)DCM_REPEATER_PROGRESS_INITIAL + 0u)
#  define DCM_RIDMGR_SECURE_CODING_STATE_VERIFY                      ((Dcm_RepeaterProgressType)DCM_REPEATER_PROGRESS_INITIAL + 1u)
#  define DCM_RIDMGR_SECURE_CODING_STATE_WRITE                       ((Dcm_RepeaterProgressType)DCM_REPEATER_PROGRESS_INITIAL + 2u)

/*! Secure Coding Validation results */
#  define DCM_RIDMGR_SECURE_CODING_RESULT_OK                         0x00
#  define DCM_RIDMGR_SECURE_CODING_RESULT_NOT_OK                     0x01
#  define DCM_RIDMGR_SECURE_CODING_RESULT_SIGNATURE_NOT_OK           0x02
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Routine operation function prototype with no request and response data */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RidMgrOpFuncNoParamsType)(
  Dcm_OpStatusType opStatus,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/*! Routine operation function prototype with only request data both with static length */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RidMgrOpFuncReqType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_OpStatusType opStatus,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/*! Routine operation function prototype with request and response data with static length */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RidMgrOpFuncReqResType)(
  Dcm_ReadOnlyMsgType reqData,
  Dcm_OpStatusType opStatus,
  Dcm_MsgType resData,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/*! Routine operation function prototype with only response data with static length */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RidMgrOpFuncResType)(
  Dcm_OpStatusType opStatus,
  Dcm_MsgType data,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/*! Routine operation function prototype with only request data with dynamic length */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RidMgrOpFuncReqDynLenType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_OpStatusType opStatus,
  Dcm_RidMgrRidLengthType Length,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/*! Routine operation function prototype with request data with dynamic length and response data with static length */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RidMgrOpFuncReqDynLenResType)(
  Dcm_ReadOnlyMsgType reqData,
  Dcm_OpStatusType opStatus,
  Dcm_MsgType resData,
  Dcm_RidMgrRidLengthType Length,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/*! Routine operation function prototype with request data with dynamic/static length and response data with dynamic length */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RidMgrOpFuncReqAnyLenResDynLenType)(
  Dcm_ReadOnlyMsgType reqData,
  Dcm_OpStatusType opStatus,
  Dcm_MsgType resData,
  Dcm_RidMgrRidLengthPtrType Length,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/*! Routine operation function prototype with response data with dynamic length */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RidMgrOpFuncResDynLenType)(
  Dcm_OpStatusType opStatus,
  Dcm_MsgType data,
  Dcm_RidMgrRidLengthPtrType Length,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/*! Generic routine operation function prototype which implementation will be generated depending on the RID configuration */
typedef P2FUNC(Std_ReturnType, DCM_APPL_CODE, Dcm_RidMgrOpFuncWrapperType)(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_RidMgrRidLengthPtrType Length,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
struct DCM_RIDMGR_SECURECODINGTYPE_TAG
{
  Dcm_MsgContextType              MsgContext;        /*!< The message context used for the write operation */
  Dcm_DidMgrDidInfoContextType    DidInfoContext;    /*!< The DID info context of the current Secure Coding DID */
  Dcm_DidMgrDidOpTypeContextType  DidOpTypeContext;  /*!< Did configuration operation context */
  uint8                           State;             /*!< Keep current state of the Secure Coding Validation Routine */
  Crypto_VerifyResultType         VerifyResult;      /*!< Store result for signature verify */
  Crypto_ResultType               CsmCbkResult;      /*!< Crypto Service Manager callback result */
  boolean                         CsmJobActive;      /*!< Whether the asynchronous CSM job is active or not */
};
typedef struct DCM_RIDMGR_SECURECODINGTYPE_TAG Dcm_RidMgrSecureCodingType;
# endif

# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
/*! Context information of the RidMgr for Secure Coding */
struct DCM_RIDMGRSINGLETONCONTEXTTYPE_TAG
{
  Dcm_RidMgrSecureCodingType  SecureCoding;  /*! Context for the Secure Coding Validation Routine */
};
typedef struct DCM_RIDMGRSINGLETONCONTEXTTYPE_TAG Dcm_RidMgrSingletonContextType;
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RidMgrRidLookUpFilter()
 *********************************************************************************************************************/
/*! \brief          Filters the RID look up result.
 *  \details        If the look-up has found a match, this filter may override this result and specify the RID as
 *                  not-supported.
 *  \param[in]      rid             The RID to be found
 *  \param[in]      ridInfoIdx      Index to the RID information
 *  \return         DCM_E_NOT_OK    No RID has been found
 *  \return         DCM_E_OK        Look up success
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_RidMgrRidLookUpFilter(
  uint16 rid,
  Dcm_CfgRidMgrInfoHandleOptType ridInfoIdx
  );

# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RidMgrSecureCodingReset()
 *********************************************************************************************************************/
/*! \brief          Resets the Secure Coding Routine
 *  \details        -
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_RidMgrSecureCodingReset(
  void
  );
# endif

# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RidMgrSecureCodingInit()
 *********************************************************************************************************************/
/*! \brief          Starts the Secure Coding validation.
 *  \details        -
 *  \param[in]      opStatus       The operation status - not used
 *  \param[in,out]  pMsgContext    Message-related information for one diagnostic protocol identifier
 *  \param[out]     ErrorCode      Negative response code
 *  \return         DCM_E_NOT_OK    Initialization failed
 *  \return         DCM_E_LOOP      Initialization performed and validation has been started, proceed with next step
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_RidMgrSecureCodingInit(
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RidMgrSecureCodingVerify()
 *********************************************************************************************************************/
/*! \brief          Waits for the result of the Secure Coding validation and checks the result
 *  \details        -
 *  \param[in]      opStatus       The operation status
 *  \param[in,out]  pMsgContext    Message-related information for one diagnostic protocol identifier
 *  \param[out]     ErrorCode      Negative response code - not used
 *  \return         DCM_E_OK         Wrong signature for secure coding data.
 *  \return         DCM_E_NOT_OK     In case the operation shall be cancelled.
 *  \return         DCM_E_PENDING    Signature verification not done yet
 *  \return         DCM_E_LOOP       Signature verification was successful, proceed with next step
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_RidMgrSecureCodingVerify(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RidMgrSecureCodingWrite()
 *********************************************************************************************************************/
/*! \brief          Writes the data stored in the Secure Coding buffer into the application
 *  \details        -
 *  \param[in]      opStatus       The operation status
 *  \param[in,out]  pMsgContext    Message-related information for one diagnostic protocol identifier
 *  \param[out]     ErrorCode      Negative response code
 *  \return         DCM_E_OK         Secure Coding write finished (successfully or unsuccessfully)
 *  \return         DCM_E_PENDING    Job processing is not yet finished
 *  \return         DCM_E_LOOP       Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_RidMgrSecureCodingWrite(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
/*! Singleton context of RidMgr unit */
DCM_LOCAL VAR(Dcm_RidMgrSingletonContextType, DCM_VAR_NOINIT) Dcm_RidMgrSingletonContext;
# endif
# define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CONST_8
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! RID sub-function ID to operation type value conversion map */
CONST(Dcm_RidMgrOpType, DCM_CONST) Dcm_RidMgrSubFunc2OpMap[4] =                                                                                      /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  DCM_RIDMGR_OP_NONE,
  DCM_RIDMGR_OP_START,
  DCM_RIDMGR_OP_STOP,
  DCM_RIDMGR_OP_REQRSLTS
};
# define DCM_STOP_SEC_CONST_8
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RidMgrRidLookUpFilter()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_RidMgrRidLookUpFilter(
  uint16 rid,
  Dcm_CfgRidMgrInfoHandleOptType ridInfoIdx
  )
{
  Std_ReturnType lStdReturn = DCM_E_OK;

  DCM_IGNORE_UNREF_PARAM(rid);                                                                                                                       /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ridInfoIdx);                                                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
# if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
  if (Dcm_DiagIsEnabledInActiveVariants(Dcm_CfgRidMgrRidInfoExecCondRef(ridInfoIdx)) == FALSE)                                                       /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
  {
    lStdReturn = DCM_E_NOT_OK;
  }
  else
# endif
  /* check session state */
  if (!Dcm_StateIsSupportedInSession(Dcm_StateGetPreconditionStates(Dcm_CfgRidMgrRidInfoExecCondRef(ridInfoIdx))))
  {
    lStdReturn = DCM_E_NOT_OK;
  }
  else
  {
# if (DCM_SVC_31_OBD_CALIBRATION_ENABLED == STD_ON) && (DCM_OBDUDSIDMGR_SUPPORT_ENABLED == STD_ON)
    if( (rid >= (uint16)0xE000u)
      &&(rid <= (uint16)0xE1FFu) )
    {
      if(Dcm_ObdIdMgrIsIdEnabled(Dcm_UtiGetLoByte(rid),
                                 Dcm_CfgWrapSvc31SupportedIdMask(Dcm_CfgWrapSvc31SupportedIdMaskOffset(Dcm_UtiGetHiByte(rid)-(uint8)0xE0u))) == FALSE) /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
      {
        lStdReturn = DCM_E_NOT_OK;
      }/* else - an enabled (WWH-)OBD RID */
    }/* else - a non (WWH-)OBD RID */
# endif
  }
  return lStdReturn;
}

# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RidMgrSecureCodingReset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_RidMgrSecureCodingReset(
  void
  )
{
  Dcm_DidMgrSecureCodingReset();

  Dcm_RidMgrSingletonContext.SecureCoding.State = DCM_RIDMGR_SECURE_CODING_STATE_IDLE;
}
# endif

# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RidMgrSecureCodingInit()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_RidMgrSecureCodingInit(
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType           lStdResult;
  Dcm_DiagDataContextType  lDataContext;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_DidMgrSecureCodingLockBuffer(&lDataContext, ErrorCode);                                                                           /* SBSW_DCM_POINTER_INIT_DATA_CONTEXT */

  if (lStdResult == DCM_E_OK)
  {
    Std_ReturnType lCsmResult;

    Dcm_RidMgrSingletonContext.SecureCoding.VerifyResult = CRYPTO_E_VER_NOT_OK;
    Dcm_RidMgrSingletonContext.SecureCoding.CsmCbkResult = E_NOT_OK;
    Dcm_RidMgrSingletonContext.SecureCoding.CsmJobActive = TRUE;

    lCsmResult = Csm_SignatureVerify(DCM_AUTHMGR_SECURE_CODING_CSM_JOB_ID
                                    ,CRYPTO_OPERATIONMODE_SINGLECALL
                                    ,lDataContext.Buffer
                                    ,lDataContext.Usage
                                    ,Dcm_UtiGetReqData(pMsgContext)                                                                                  /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */
                                    ,pMsgContext->reqDataLen
                                    ,&Dcm_RidMgrSingletonContext.SecureCoding.VerifyResult);                                                         /* SBSW_DCM_COMB_PTR_FORWARD */

    switch(lCsmResult)
    {
    case E_OK:
      /* The signature verification procedure was started OK, result will come later. */
      Dcm_RidMgrSingletonContext.SecureCoding.State = DCM_RIDMGR_SECURE_CODING_STATE_VERIFY;

      /* Redirect pMsgContext to the Secure Coding buffer, CSM still works with the signature stored in the reception buffer */
      Dcm_RidMgrSingletonContext.SecureCoding.MsgContext = *pMsgContext;

      Dcm_RidMgrSingletonContext.SecureCoding.MsgContext.reqData    = lDataContext.Buffer;
      Dcm_RidMgrSingletonContext.SecureCoding.MsgContext.reqDataLen = lDataContext.Usage;
      Dcm_RidMgrSingletonContext.SecureCoding.MsgContext.reqBufSize = lDataContext.Size;
      Dcm_RidMgrSingletonContext.SecureCoding.MsgContext.reqIndex   = 0u;

      /* Init DID for first write */
      Dcm_UtiConsumeReqDataAsU16(&Dcm_RidMgrSingletonContext.SecureCoding.MsgContext
                                ,&Dcm_RidMgrSingletonContext.SecureCoding.DidInfoContext.Did);                                                       /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

      lStdResult = DCM_E_LOOP;
      break;
    case CRYPTO_E_BUSY:
    case CRYPTO_E_SMALL_BUFFER:
    case CRYPTO_E_KEY_NOT_VALID:
      /* All other values should never occur, therefore reset the sequence and send NRC 0x10 */
      Dcm_RidMgrSecureCodingReset();

      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      break;
    default:
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
      break;
    }
  }

  return lStdResult;
}
# endif

# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RidMgrSecureCodingVerify()
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
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_RidMgrSecureCodingVerify(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if (opStatus == DCM_CANCEL)
  {
    lStdResult = Csm_CancelJob(DCM_AUTHMGR_SECURE_CODING_CSM_JOB_ID, CRYPTO_OPERATIONMODE_SINGLECALL);

    if (lStdResult == E_OK)
    {
      Dcm_RidMgrSecureCodingReset();
    }
    else
    {
      Dcm_UtiEnterCriticalSection();
      /*=================================*
        BEGIN CRITICAL SECTION
       *=================================*/
      if(Dcm_RidMgrSingletonContext.SecureCoding.CsmJobActive == TRUE)
      {
        /*
         * The CSM closing callback notification was not called yet, so wait for it.
         * Since the Secure Coding buffer is still locked and therefore still guarded, the state of the Secure Coding
         * Validation routine can now be reset as an indication, that the CSM closing callback notification is responsible
         * to reset the sequence later.
         */
        Dcm_RidMgrSingletonContext.SecureCoding.State = DCM_RIDMGR_SECURE_CODING_STATE_IDLE;
      }
      else
      {
        Dcm_RidMgrSecureCodingReset();
      }
      /*=================================*
        END CRITICAL SECTION
       *=================================*/
      Dcm_UtiLeaveCriticalSection();
    }

    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
    if (Dcm_RidMgrSingletonContext.SecureCoding.CsmJobActive == TRUE)
    {
      /* Wait until the CSM closing notification callback is called */
      lStdResult = DCM_E_PENDING;
    }
    else
    {
      /* Csm Job is finished, check the Csm callback result */
      if ( (Dcm_RidMgrSingletonContext.SecureCoding.CsmCbkResult == E_OK)
         &&(Dcm_RidMgrSingletonContext.SecureCoding.VerifyResult == CRYPTO_E_VER_OK) )
      {
        /* The signature of the data has been verified ok, the data can be used - start the write DID state */
        Dcm_RidMgrSingletonContext.SecureCoding.State = DCM_RIDMGR_SECURE_CODING_STATE_WRITE;

        lStdResult = DCM_E_LOOP;
      }
      else
      {
        /* CSM signature verify job failed, return 0x02 as routine result */
        Dcm_UtiSetResDataAsU8Rel(pMsgContext, 0u, DCM_RIDMGR_SECURE_CODING_RESULT_SIGNATURE_NOT_OK);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

        Dcm_RidMgrSecureCodingReset();

        lStdResult = DCM_E_OK;
      }
    }
  }

  return lStdResult;
}
# endif

# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RidMgrSecureCodingWrite()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_RidMgrSecureCodingWrite(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
)
{
  Std_ReturnType lStdResult;

  lStdResult = Dcm_DidMgrSecureCodingWrite(opStatus
                                          ,&Dcm_RidMgrSingletonContext.SecureCoding.MsgContext
                                          ,&Dcm_RidMgrSingletonContext.SecureCoding.DidInfoContext
                                          ,&Dcm_RidMgrSingletonContext.SecureCoding.DidOpTypeContext
                                          ,ErrorCode);                                                                                               /* SBSW_DCM_COMB_PTR_FORWARD */

  switch (lStdResult)
  {
  case DCM_E_OK:
    Dcm_UtiSetResDataAsU8Rel(pMsgContext, 0u, DCM_RIDMGR_SECURE_CODING_RESULT_OK);                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_DebugReportSecurityEvent(pMsgContext->threadId, DCM_DEBUG_SEV_WRITE_DATA);
    Dcm_RidMgrSecureCodingReset();
    break;
  case DCM_E_NOT_OK:
    Dcm_UtiSetResDataAsU8Rel(pMsgContext, 0u, DCM_RIDMGR_SECURE_CODING_RESULT_NOT_OK);                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_DebugReportSecurityEvent(pMsgContext->threadId, DCM_DEBUG_SEV_WRITE_INV_DATA);
    Dcm_RidMgrSecureCodingReset();
    lStdResult = DCM_E_OK;
    break;
  default: /* DCM_E_PENDING, DCM_E_LOOP */
    break;
  }

  return lStdResult;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_RidMgrInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_RidMgrInit(
  void
  )
{
# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
  Dcm_RidMgrSecureCodingReset();
# endif
}

/**********************************************************************************************************************
 *  Dcm_RidMgrGetOpInfo()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_RidMgrGetOpInfo(
  Dcm_CfgRidMgrInfoHandleOptType ridInfoIdx,
  Dcm_RidMgrOpType ridOp,
  Dcm_CfgRidMgrOpHandlePtrType pRidOpInfoIdx
  )
{
  Std_ReturnType lResult = DCM_E_NOT_OK;
  Dcm_RidMgrOpType lRidOp = ridOp;

  /* If the requested operation is supported at all */
  if ((Dcm_CfgRidMgrRidInfoOperations(ridInfoIdx) & lRidOp) != 0u)
  {
    Dcm_CfgRidMgrOpHandleOptType opHandle = Dcm_CfgRidMgrRidInfoOpBaseIdx(ridInfoIdx);
    do
    {
      lRidOp >>= 1;/* pre-decrement! */
      if ((Dcm_CfgRidMgrRidInfoOperations(ridInfoIdx) & lRidOp) != 0u)
      {
        ++opHandle;
      }
    }
    while (lRidOp != 0u);
    *pRidOpInfoIdx = opHandle;                                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_OK;
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_RidMgrExecuteRoutine()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_RidMgrExecuteRoutine(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc31RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType           lStdResult;
  Dcm_RidMgrOpInfoPtrType  pRidOpInfo;

  lStdResult = Dcm_CfgWrapRidMgrOpInfoGetEntry(pRepContext->RidOpInfoIdx, &pRidOpInfo, ErrorCode);                                                   /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    lStdResult = Dcm_UtiMsgContextVerifySize(pMsgContext, pRidOpInfo->ResMaxLength, ErrorCode);                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */

    if(lStdResult == DCM_E_OK)                                                                                                                       /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
    {
      Dcm_RidMgrRidLengthType lReqResLen = (Dcm_RidMgrRidLengthType)(pMsgContext->reqDataLen - pRidOpInfo->ReqMinLength);

      /* Call appropriate function prototype */
      switch(pRidOpInfo->OpType)
      {
      case DCM_RIDMGR_OPTYPE_NONE:
        lStdResult = ((Dcm_RidMgrOpFuncNoParamsType)pRidOpInfo->OpFunc)(opStatus                                                                     /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                       ,ErrorCode);                                                                  /* SBSW_DCM_CALL_FUNCPTR_SVC31OPFUNC */
        break;
      case DCM_RIDMGR_OPTYPE_REQ:
        lStdResult = ((Dcm_RidMgrOpFuncReqType)pRidOpInfo->OpFunc)(Dcm_UtiGetReqData(pMsgContext)                                                    /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */
                                                                  ,opStatus
                                                                  ,ErrorCode);                                                                       /* SBSW_DCM_CALL_FUNCPTR_SVC31OPFUNC */
        break;
      case DCM_RIDMGR_OPTYPE_REQ_RES:
        lStdResult = ((Dcm_RidMgrOpFuncReqResType)pRidOpInfo->OpFunc)(Dcm_UtiGetReqData(pMsgContext)                                                 /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */
                                                                     ,opStatus
                                                                     ,Dcm_UtiGetResData(pMsgContext)                                                 /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                                     ,ErrorCode);                                                                    /* SBSW_DCM_CALL_FUNCPTR_SVC31OPFUNC */
        break;
      case DCM_RIDMGR_OPTYPE_RES:
        lStdResult = ((Dcm_RidMgrOpFuncResType)pRidOpInfo->OpFunc)(opStatus                                                                          /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                  ,Dcm_UtiGetResData(pMsgContext)                                                    /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                                  ,ErrorCode);                                                                       /* SBSW_DCM_CALL_FUNCPTR_SVC31OPFUNC */
        break;
      case DCM_RIDMGR_OPTYPE_REQ_DYNLEN:
        lStdResult = ((Dcm_RidMgrOpFuncReqDynLenType)pRidOpInfo->OpFunc)(Dcm_UtiGetReqData(pMsgContext)                                              /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */
                                                                        ,opStatus
                                                                        ,lReqResLen
                                                                        ,ErrorCode);                                                                 /* SBSW_DCM_CALL_FUNCPTR_SVC31OPFUNC */
        break;
      case DCM_RIDMGR_OPTYPE_REQ_DYNLEN_RES:
        lStdResult = ((Dcm_RidMgrOpFuncReqDynLenResType)pRidOpInfo->OpFunc)(Dcm_UtiGetReqData(pMsgContext)                                           /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */
                                                                           ,opStatus
                                                                           ,Dcm_UtiGetResData(pMsgContext)                                           /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                                           ,lReqResLen
                                                                           ,ErrorCode);                                                              /* SBSW_DCM_CALL_FUNCPTR_SVC31OPFUNC */
        break;
      case DCM_RIDMGR_OPTYPE_RES_DYNLEN:
        lStdResult = ((Dcm_RidMgrOpFuncResDynLenType)pRidOpInfo->OpFunc)(opStatus                                                                    /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                        ,Dcm_UtiGetResData(pMsgContext)                                              /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                                        ,&lReqResLen
                                                                        ,ErrorCode);                                                                 /* SBSW_DCM_CALL_FUNCPTR_SVC31OPFUNC */
        break;
      case DCM_RIDMGR_OPTYPE_REQ_DYNLEN_RES_DYNLEN:
        /* fall through */
      case DCM_RIDMGR_OPTYPE_REQ_RES_DYNLEN:                                                                                                         /* PRQA S 2003 */ /* MD_Dcm_Optimize_2003 */
        lStdResult = ((Dcm_RidMgrOpFuncReqAnyLenResDynLenType)pRidOpInfo->OpFunc)(Dcm_UtiGetReqData(pMsgContext)                                     /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */
                                                                                 ,opStatus
                                                                                 ,Dcm_UtiGetResData(pMsgContext)                                     /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                                                 ,&lReqResLen
                                                                                 ,ErrorCode);                                                        /* SBSW_DCM_CALL_FUNCPTR_SVC31OPFUNC */
        break;
      case DCM_RIDMGR_OPTYPE_WRAPPER:
        lStdResult = ((Dcm_RidMgrOpFuncWrapperType)pRidOpInfo->OpFunc)(opStatus                                                                      /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                      ,pMsgContext
                                                                      ,&lReqResLen
                                                                      ,ErrorCode);                                                                   /* SBSW_DCM_CALL_FUNCPTR_SVC31OPFUNC */
        break;
      default:
        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INVALID_CONFIG);
        break;
      }

      /* If the operation result is success: */
      if(lStdResult == DCM_E_OK)
      {
        /* Provide the data to the response message, including the variable length part */
        Dcm_UtiProvideResData(pMsgContext, Dcm_CfgRidMgrOpInfoResMinLength(pRepContext->RidOpInfoIdx));                                              /* SBSW_DCM_PARAM_PTR_FORWARD */

        if(pRidOpInfo->ResMinLength != pRidOpInfo->ResMaxLength)
        {
          Dcm_UtiProvideResData(pMsgContext, lReqResLen); /* Register response length of the last signal  */                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
        }
      }

      /* Catch any RTE specific or unexpected errors in order to return a valid value to the core */
      if( (lStdResult != DCM_E_OK)
        &&(lStdResult != DCM_E_PENDING)
        &&(lStdResult != DCM_E_NOT_OK)
        &&(lStdResult != DCM_E_FORCE_RCRRP)
        &&(lStdResult != DCM_E_LOOP) )
      {
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }

      Dcm_UtiHandleApplNrc(lStdResult, ErrorCode, DCM_E_CONDITIONSNOTCORRECT);                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Dcm_RidMgrRidLookUp()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_RidMgrRidLookUp(
  Dcm_OpStatusType opStatus,
  uint16 rid,
  Dcm_CfgRidMgrInfoHandlePtrType pRidInfoIdx
  )
{
  Std_ReturnType lStdReturn;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdReturn = DCM_E_OK;

# if (DCM_RIDMGR_EXTENDED_LOOKUP_ENABLED == STD_ON)
  if(opStatus == DCM_INITIAL)
# endif
  {
    sint16_least lResult;

    lResult = Dcm_UtiLookUpUint16(Dcm_CfgRidMgrRidLookUpTable, rid);                                                                                 /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
    if(lResult >= 0)
    {
      *pRidInfoIdx = (Dcm_CfgRidMgrInfoHandleOptType)lResult;                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      lStdReturn = DCM_E_NOT_OK;
    }
  }

# if (DCM_RIDMGR_EXTENDED_LOOKUP_ENABLED == STD_ON)
  if(lStdReturn == DCM_E_OK)
  {
    /* If RID is suppressed by application filter: */
    lStdReturn = Dcm_FilterRidLookUpResult(opStatus, rid);

    /* If that was last call (DCM_CANCEL), skip any further processing and return any valid value */
    if(opStatus == DCM_CANCEL)
    {
      /* Skip any further processing */
      lStdReturn = DCM_E_NOT_OK;
    }

    /* Catch any unexpected errors in order to return a valid value to the core */
    if(Dcm_DebugDetectError( (lStdReturn != DCM_E_OK)
                           &&(lStdReturn != DCM_E_PENDING)
                           &&(lStdReturn != DCM_E_NOT_OK) ))
    {
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
      lStdReturn = DCM_E_NOT_OK;
    }
  }/* else - DCM_E_NOT_OK */
# endif

  if(lStdReturn == DCM_E_OK)
  {
    lStdReturn = Dcm_RidMgrRidLookUpFilter(rid, *pRidInfoIdx);
  }/* else - DCM_E_NOT_OK, DCM_E_PENDING */
  return lStdReturn;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_RidMgr_SecureCodingValidation_Start()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_RidMgr_SecureCodingValidation_Start(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_RidMgrRidLengthPtrType DataLength,                                                                                                             /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(DataLength);                                                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  switch (Dcm_RidMgrSingletonContext.SecureCoding.State)
  {
  case DCM_RIDMGR_SECURE_CODING_STATE_IDLE:
    lStdResult = Dcm_RidMgrSecureCodingInit(opStatus, pMsgContext, ErrorCode);                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_RIDMGR_SECURE_CODING_STATE_VERIFY:
    lStdResult = Dcm_RidMgrSecureCodingVerify(opStatus, pMsgContext, ErrorCode);                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_RIDMGR_SECURE_CODING_STATE_WRITE:
    lStdResult = Dcm_RidMgrSecureCodingWrite(opStatus, pMsgContext, ErrorCode);                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  default:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CALLOUT_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_RIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_CsmSecureCodingValidationFinished()
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
FUNC(void, DCM_CODE) Dcm_CsmSecureCodingValidationFinished(
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
  else if(Dcm_DebugDetectError(job->jobId != DCM_AUTHMGR_SECURE_CODING_CSM_JOB_ID))
  {
    lErrorId = DCM_E_PARAM;
  }
  else if(Dcm_DebugDetectError(Dcm_RidMgrSingletonContext.SecureCoding.CsmJobActive == FALSE))
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
    Dcm_RidMgrSingletonContext.SecureCoding.CsmCbkResult = result;
    Dcm_RidMgrSingletonContext.SecureCoding.CsmJobActive = FALSE;

    if (Dcm_RidMgrSingletonContext.SecureCoding.State != DCM_RIDMGR_SECURE_CODING_STATE_VERIFY)
    {
      Dcm_RidMgrSecureCodingReset();
    }
    /*=================================*
      END CRITICAL SECTION
      *=================================*/
    Dcm_UtiLeaveCriticalSection();
  }

  /* ----- Development Error Report ---------------------------------------- */
  Dcm_DebugReportError(DCM_SID_CSMSECURECODINGVALIDATIONFINISHED, lErrorId);
}
# endif
# define DCM_STOP_SEC_CALLOUT_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_RIDMGR_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_RidMgr.c
 *********************************************************************************************************************/
