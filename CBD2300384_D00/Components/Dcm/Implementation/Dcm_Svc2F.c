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
/**        \file  Dcm_Svc2F.c
 *         \unit  Svc2F
 *        \brief  Contains the implementation of Service 0x2F unit.
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
#define DCM_SVC2F_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc2F.h"
#if (DCM_SVC_2F_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_Debug.h"
# include "Dcm_Uti.h"
# include "Dcm_RsrcMgr.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define DCM_SVC2F_PROGRESS_CHECKACCESS                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
# define DCM_SVC2F_PROGRESS_EXECUTEOP                                ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))
# define DCM_SVC2F_PROGRESS_GETLENGTH                                ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 2u))
# define DCM_SVC2F_PROGRESS_READDATA                                 ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 3u))

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# define Dcm_Svc2FSetDidActive(ioDidHdl)                             (Dcm_UtiGenericBitSetSetBit(Dcm_Svc2FSingletonContext.ActiveIoDids,(ioDidHdl))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Context for the handling of IODIDs */
struct DCM_SVC2FSINGLETONCONTEXTTYPE_TAG
{
  Dcm_DiagDataContextType DataContext;                                                /*!< Data context */
# if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
  Dcm_UtiBitSetBaseType   ActiveIoDids[Dcm_UtiGenericBitSetCalcSize(DCM_NUM_IODIDS)]; /*!< Bit masks of active IO operations */
  boolean                 HasAnyActiveIoDid;                                          /*!< Active IODID operation flag */
# endif
};
typedef struct DCM_SVC2FSINGLETONCONTEXTTYPE_TAG Dcm_Svc2FSingletonContextType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2FHandler()
 *********************************************************************************************************************/
/*! \brief          Service 0x2F Handler.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \return         DCM_E_PENDING           Final result is pending, retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2FHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  );

# if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2FGetActiveIoDids()
 *********************************************************************************************************************/
/*! \brief          Return the requested active IODIDs bit mask
 *  \details        -
 *  \param[in]      ioDidIndex              Index of the IODID
 *  \return         The active IODIDs bit mask
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_UtiBitSetBasePtrType, DCM_CODE) Dcm_Svc2FGetActiveIoDids(
  Dcm_CfgDidMgrIoDidHandleOptType ioDidIndex
  );
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2FExtractCEMR()
 *********************************************************************************************************************/
/*! \brief          Extracts the CEMR from the request.
 *  \details        Extracts the CEMR from a byte stream into a 32bit variable. If CEMRsize > 4 -> value is 0.
 *  \param[in]      cemrStream   Points to the beginning of the CEMR in the request
 *  \param[in]      cemrSize     Specifies the CEMR total length
 *  \return         0            If the CEMR size > 4 or no bit has been set. This value is invalid and will only
 *                               be used as initialization of the CEMR, but not passed to the application for NBit CEMR.
 *  \return         >0           Extracted CEMR. Note: For 3Byte CEMR the result is MSB bound!
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(uint32, DCM_CODE) Dcm_Svc2FExtractCEMR(
  Dcm_ReadOnlyMsgType cemrStream,
  Dcm_CfgDidMgrIoDidCemrLengthMemType cemrSize
  );

/**********************************************************************************************************************
 *  Dcm_Svc2FProcessCEMR()
 *********************************************************************************************************************/
/*! \brief          Processes CEMR of IO DID request if the IO DID supports CEMR.
 *  \details        On IO DIDs with CEMR the CEMR will be extracted and verified for non-zero value.
 *  \param[in,out]  pMsgContext        The current request context
 *  \param[in,out]  pRepContext        Current repeater proxy context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2FProcessCEMR(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2FCalculateExpectedReqLength()
 *********************************************************************************************************************/
/*! \brief          Calculates the expected request length including DID data and CEMR.
 *  \details        -
 *  \param[out]     expLengthMin        Returns the expected request minimum length.
 *  \param[out]     expLengthMax        Returns the expected request maximum length.
 *  \param[in,out]  pRepContext         Current repeater proxy context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2FCalculateExpectedReqLength(
  Dcm_DidMgrDidLengthPtrType expLengthMin,
  Dcm_DidMgrDidLengthPtrType expLengthMax,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2FCheckReqLength()
 *********************************************************************************************************************/
/*! \brief          Verifies the request length.
 *  \details        Verifies that the request length is valid for the concrete DID and IO operation incl. CEMR.
 *  \param[in,out]  pMsgContext        The current request context
 *  \param[in,out]  pRepContext        Current repeater proxy context
 *  \return         DCM_E_POSITIVERESPONSE            Request length is valid.
 *  \return         !=DCM_E_POSITIVERESPONSE          A validation error has occurred.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_NegativeResponseCodeType, DCM_CODE) Dcm_Svc2FCheckReqLength(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2FIoDidOperationProcessor()
 *********************************************************************************************************************/
/*! \brief          Process a concrete IODID and its requested operation.
 *  \details        -
 *  \param[in]      pContext         Pointer to the context
 *  \param[in]      pMsgContext      Pointer to current message context
 *  \param[out]     ErrorCode        The NRC
 *  \param[out]     pRepContext      Current repeater proxy context
 *  \return         DCM_E_NOT_OK     Send negative response
 *  \return         DCM_E_LOOP       Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2FIoDidOperationProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2FCheckAccess()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x2F.
 *  \details        Checks the service 0x2F access.
 *  \param[in]      pContext         Pointer to the context
 *  \param[in]      opStatus         Current operation status
 *  \param[in]      pMsgContext      Pointer to current message context
 *  \param[out]     ErrorCode        The NRC
 *  \param[in]      pRepContext      Current repeater proxy context
 *  \return         DCM_E_LOOP       Repeat immediately
 *  \return         DCM_E_PENDING    Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2FCheckAccess(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2FExecuteOp()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x2F.
 *  \details        Executes a specific service 0x2F operation.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in]      opStatus       Current operation status
 *  \param[in]      pMsgContext    Pointer to current message context
 *  \param[out]     ErrorCode        The NRC
 *  \param[in,out]  pRepContext      Current repeater proxy context
 *  \return         Next action to perform
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2FExecuteOp(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2FGetLength()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x2F.
 *  \details        Reads the length of a requested DID.
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_LOOP              Repeat immediately
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2FGetLength(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Svc2FReadData()
 *********************************************************************************************************************/
/*! \brief          A sub-function utility of SID 0x2F.
 *  \details        -
 *  \param[in]      opStatus       Current operation status
 *  \param[in]      pMsgContext    Pointer to current message context
 *  \param[out]     ErrorCode      The NRC
 *  \param[out]     pRepContext    Current repeater proxy context
 *  \return         DCM_E_OK       Send response
 *  \return         DCM_E_NOT_OK   Send negative response
 *  \return         DCM_E_PENDING  Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2FReadData(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  );
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Singleton context of service 2F unit */
DCM_LOCAL VAR(Dcm_Svc2FSingletonContextType, DCM_VAR_NOINIT) Dcm_Svc2FSingletonContext;                                                              /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
# define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CONST_8
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
DCM_LOCAL CONST(Dcm_DidMgrOpMemType, DCM_CONST) Dcm_Svc2FCtrlOp2DidOpTypeMap[4] =                                                                    /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU
 ,DCM_DIDMGR_OPTYPE_IO_RST2DEF
 ,DCM_DIDMGR_OPTYPE_IO_FRZCURSTATE
 ,DCM_DIDMGR_OPTYPE_IO_SHRTTRMADJ
};
# define DCM_STOP_SEC_CONST_8
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc2FHandler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2FHandler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;

  if (opStatus == DCM_INITIAL)
  {
    /* Min Length already checked in DiagDispatcher and expected to be 3 !!! */
    Dcm_UtiConsumeReqDataAsU16(pMsgContext, &(pRepContext->DidInfoContext.Did));                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  lStdResult = Dcm_DidMgrGetDidInfo(opStatus
                                   ,&(pRepContext->DidInfoContext)
                                   ,&(pRepContext->DidOpTypeContext)
                                   ,DCM_DIDMGR_OP_IO);                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */

  switch (lStdResult)
  {
  case DCM_E_OK:
  {
    uint8 lControlOp;
    Dcm_UtiConsumeReqDataAsU8(pMsgContext, &lControlOp);                                                                                             /* SBSW_DCM_COMB_PTR_FORWARD */

    /* convert from UDS to internal operation representation */
    if (lControlOp < 4u)
    {
      pRepContext->OpType = Dcm_Svc2FCtrlOp2DidOpTypeMap[lControlOp]; /* use interpreted value */                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
    {
      pRepContext->OpType = (Dcm_DidMgrOpMemType)0x00u; /* no supported operation */                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
    }

    /* Check for supported sub-operation */
    if (Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&pRepContext->DidInfoContext), pRepContext->OpType))                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    {
      /* Commit response header */
      Dcm_UtiProvideResData(pMsgContext, 3);                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

      lStdResult = Dcm_Svc2FIoDidOperationProcessor(pContext, pMsgContext, ErrorCode, pRepContext);                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }
  break;
  case DCM_E_NOT_OK:
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  default: /* DCM_E_PENDING -> try again */
    break;
  }

  return lStdResult;
}

# if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2FGetActiveIoDids()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_UtiBitSetBasePtrType, DCM_CODE) Dcm_Svc2FGetActiveIoDids(
  Dcm_CfgDidMgrIoDidHandleOptType ioDidIndex
  )
{
  Dcm_UtiBitSetBasePtrType pActiveIoDids;

  if (Dcm_DebugDetectRuntimeError(ioDidIndex >= Dcm_UtiGenericBitSetCalcSize(DCM_NUM_IODIDS)))                                                       /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    pActiveIoDids = &Dcm_Svc2FSingletonContext.ActiveIoDids[0];
  }
  else
  {
    pActiveIoDids = &Dcm_Svc2FSingletonContext.ActiveIoDids[ioDidIndex];
  }

  return pActiveIoDids;
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc2FExtractCEMR()
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
DCM_LOCAL_INLINE FUNC(uint32, DCM_CODE) Dcm_Svc2FExtractCEMR(
  Dcm_ReadOnlyMsgType cemrStream,
  Dcm_CfgDidMgrIoDidCemrLengthMemType cemrSize
  )
{
  uint32 lResult = 0;

  if (cemrSize <= 4u)
  {
    Dcm_CfgDidMgrIoDidCemrLengthOptType lCemrIter;

    /* Extract the CEMR byte-wise */
    for (lCemrIter = 0; lCemrIter < cemrSize; ++lCemrIter)
    {
      lResult <<= 8u;
      lResult |= cemrStream[lCemrIter];
    }

    /* For a 3 byte CEMR a 32bit value will be passed -> move to the MSB to fulfill the IO C/S API convention */
    if (cemrSize == 3u)
    {
      lResult <<= 8u;
    }
  }/* else - leave the result = 0 (invalid value) */

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc2FProcessCEMR()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2FProcessCEMR(
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  )
{
  Dcm_CfgDidMgrIoDidCemrLengthMemType lCemrSize;

  /* Consider requests with and without CEMR */
  lCemrSize = Dcm_DidMgrGetCtrlEnblMaskLength(Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DidInfoContext)));                                              /* SBSW_DCM_PARAM_PTR_FORWARD */

  if (lCemrSize != 0u)
  {
    /*
    * Subtract CEMR length from request length to get only the requested DID size in order to get the concrete length
    * of the last DID signal (if variable length)
    */
    Dcm_UtiUpdateReqLength(pMsgContext, lCemrSize);                                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
    pRepContext->DidOpTypeContext.OpType.Io.EnableMaskPtr = Dcm_UtiGetReqDataRel(pMsgContext, pMsgContext->reqDataLen);                              /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Extract CEMR as a value */
    pRepContext->DidOpTypeContext.OpType.Io.ControlEnableMaskRecord = Dcm_Svc2FExtractCEMR(pRepContext->DidOpTypeContext.OpType.Io.EnableMaskPtr     /* SBSW_DCM_PARAM_PTR_WRITE */ /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                                                          ,lCemrSize);
  }
  else
  {
    /* else - the request does not contain any CEMR */
    pRepContext->DidOpTypeContext.OpType.Io.EnableMaskPtr = NULL_PTR;                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
  }
}

/**********************************************************************************************************************
 *  Dcm_Svc2FCalculateExpectedReqLength()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc2FCalculateExpectedReqLength(
  Dcm_DidMgrDidLengthPtrType expLengthMin,
  Dcm_DidMgrDidLengthPtrType expLengthMax,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_CfgDidMgrIoDidCemrLengthMemType lCemrSize;

  if (Dcm_UtiBitOpTest(Dcm_DidMgrOpMemType, pRepContext->OpType, DCM_DIDMGR_OPTYPE_IO_SHRTTRMADJ))
  {
    /* Initialize DID handler */
    Dcm_DidMgrInitOpTypeHandler(&(pRepContext->DidInfoContext), &(pRepContext->DidOpTypeContext));                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* short term adjustment - consider optional enable mask record and the DID data! */
    *expLengthMax = (Dcm_DidMgrDidLengthType)Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DidInfoContext))->MaxLength;                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    *expLengthMin = (Dcm_DidMgrDidLengthType)Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DidInfoContext))->MinLength;                                     /* SBSW_DCM_PARAM_PTR_WRITE */
    if (*expLengthMin == 0u) /* IO DID with dynamic length AND single signal */
    {
      *expLengthMin = 1;                                                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }
  else
  {
    *expLengthMin = 0u;                                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
    *expLengthMax = 0u;                                                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  lCemrSize = Dcm_DidMgrGetCtrlEnblMaskLength(Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DidInfoContext)));                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
  *expLengthMin += lCemrSize;                                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
  *expLengthMax += lCemrSize;                                                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
}

/**********************************************************************************************************************
 *  Dcm_Svc2FCheckReqLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_NegativeResponseCodeType, DCM_CODE) Dcm_Svc2FCheckReqLength(
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  )
{
  Dcm_NegativeResponseCodeType lNrc = DCM_E_POSITIVERESPONSE;
  Dcm_DidMgrDidLengthType expMinReqLen;
  Dcm_DidMgrDidLengthType expMaxReqLen;

  Dcm_Svc2FCalculateExpectedReqLength(&expMinReqLen, &expMaxReqLen, pRepContext);                                                                    /* SBSW_DCM_COMB_PTR_FORWARD */

  /* Check DID specific length */
  if ((pMsgContext->reqDataLen < expMinReqLen)
    || (pMsgContext->reqDataLen > expMaxReqLen))
  {
    lNrc = DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT;
  }

  return lNrc;
}

/**********************************************************************************************************************
 *  Dcm_Svc2FIoDidOperationProcessor()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2FIoDidOperationProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType  lStdResult = DCM_E_NOT_OK;
  Dcm_CfgStateRefOptType lStateRef;

  /* Initialize the IO-Control operation class from now for any checks that may come
  * (i.e. within Dcm_Svc2FCheckReqLength or later for the IO Control execution)  */
  Dcm_DidMgrInitOpClassInfo(&(pRepContext->DidInfoContext)
    , pRepContext->OpType);                                                                                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */

  /* Validate request length */
  *ErrorCode = Dcm_Svc2FCheckReqLength(pMsgContext, pRepContext);                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_PARAM_PTR_WRITE */

  if (*ErrorCode == DCM_E_POSITIVERESPONSE)
  {
    lStateRef = Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->DidInfoContext))->ExecCondRef;

    /* Validate remaining state preconditions */
    lStdResult = Dcm_DidMgrStaticDidStateCheck(Dcm_NetGetConnHdlOfRxPduId(pMsgContext->rxPduId)                                                      /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_PARAM_PTR_FORWARD */
                                              ,pRepContext->DidInfoContext.Did
                                              ,DCM_DIDMGR_OP_IO
                                              ,lStateRef
                                              ,ErrorCode);
    if (lStdResult == DCM_E_OK)
    {
      /* Process any eventually supported CEMR */
      Dcm_Svc2FProcessCEMR(pMsgContext, pRepContext);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_RepeaterNextStep(pContext, DCM_SVC2F_PROGRESS_CHECKACCESS); /* delegate job */                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */

      lStdResult = DCM_E_LOOP;/* speed up processing */
    }/* else DCM_E_NOT_OK (ErrorCode already set) */
  }/* else DCM_E_NOT_OK (ErrorCode already set) */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc2FCheckAccess()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2FCheckAccess(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,                                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_RsrcMgrGetDidLock(opStatus, pRepContext->DidInfoContext.Did, DCM_RSRCMGR_DIDLOCK_OWNER_SID2F);

  if (lStdResult == DCM_E_OK)
  {
    Dcm_RepeaterNextStep(pContext, DCM_SVC2F_PROGRESS_EXECUTEOP); /* delegate job */                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP;/* speed up processing */
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc2FExecuteOp()
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
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2FExecuteOp(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_DidMgrIoControl(opStatus
                                  ,pMsgContext
                                  ,&(pRepContext->DidInfoContext)
                                  ,&(pRepContext->DidOpTypeContext)
                                  ,ErrorCode);                                                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)
  {
# if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
    if (pRepContext->OpType != DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU)
    {
      /*
      * After sucessful execution of a control operation other than "ReturnControlToEcu", register the entire IODID for
      * automatic reset on a session/security transition (only if "ReturnControlToEcu" is supported by that IODID)
      */
      if (Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->DidInfoContext)),
                                      DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU))                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
      {
        Dcm_CfgDidMgrCombinedOpRefType lOpRef;

        lOpRef = Dcm_DidMgrOpInfoRedirector(Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DidInfoContext))->OpRef, OpRefIoControl);

        /* Must have a valid index! */
        Dcm_DebugAssert(lOpRef < DCM_NUM_IODIDS, DCM_SID_INTERNAL, DCM_E_INVALID_CONFIG);                                                            /* COV_DCM_RTM_DEV_DEBUG XF */ /* SBSW_DCM_PARAM_PTR_FORWARD */

        Dcm_Svc2FSetDidActive(lOpRef);                                                                                                               /* SBSW_DCM_POINTER_WRITE_BITSET */

        Dcm_Svc2FSingletonContext.HasAnyActiveIoDid = TRUE;
      }
    }
# endif
    /* Decide whether any response data has to be sent back to the client */
    if (Dcm_DidMgrIsOpSupported(Dcm_CfgDidMgrGetDidInfo(&(pRepContext->DidInfoContext)), DCM_DIDMGR_OP_READ))
    {
      Dcm_DidMgrInitOpInfo(&(pRepContext->DidInfoContext), DCM_DIDMGR_OP_READ);                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */

      Dcm_RepeaterNextStep(pContext, DCM_SVC2F_PROGRESS_GETLENGTH);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdResult = DCM_E_LOOP;
    } /* Otherwise, IO DID does not support reading operation - continue with an empty positive response */
  } /* DCM_E_PENDING | DCM_E_NOT_OK */

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Svc2FGetLength()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2FGetLength(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType lStdReturn = DCM_E_OK;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if (Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(&(pRepContext->DidInfoContext)), DCM_DIDMGR_OPTYPE_READLENGTH))
  {
    Dcm_DidMgrInitOpClassInfo(&(pRepContext->DidInfoContext), DCM_DIDMGR_OPTYPE_READLENGTH);                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdReturn = Dcm_DidMgrReadLength(opStatus
                                     ,&(pRepContext->DidInfoContext)
                                     ,&(pRepContext->DidOpTypeContext)
                                     ,ErrorCode);                                                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if (lStdReturn == DCM_E_OK)
  {
    /* Delegate the job to the data reader */
    Dcm_RepeaterNextStep(pContext, DCM_SVC2F_PROGRESS_READDATA);                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdReturn = DCM_E_LOOP;
  } /* else DCM_E_PENDING or DCM_E_NOT_OK (ErrorCode already set) */

  return lStdReturn;
}

/**********************************************************************************************************************
 *  Dcm_Svc2FReadData()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2FReadData(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType          lStdResult;

  if (opStatus == DCM_INITIAL)
  {
    /* Initialize data context for asynchronous IO DIDs only initially (not in pending status) */
    Dcm_UtiInitDataContext(&Dcm_Svc2FSingletonContext.DataContext, Dcm_UtiGetResData(pMsgContext), pMsgContext->resMaxDataLen);                      /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* Read DID  */
  Dcm_DidMgrInitOpClassInfo(&(pRepContext->DidInfoContext), DCM_DIDMGR_OPTYPE_READ);                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  lStdResult = Dcm_DidMgrReadDid(opStatus
                                ,&Dcm_Svc2FSingletonContext.DataContext
                                ,&(pRepContext->DidInfoContext)
                                ,&(pRepContext->DidOpTypeContext)
                                ,ErrorCode);                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */

  if (lStdResult == DCM_E_OK)
  {
    /* Commit data and go on with response */
    Dcm_UtiProvideResData(pMsgContext, pRepContext->DidInfoContext.DidLength);                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  return lStdResult;
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2FReturnControlToEcu()
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
FUNC(void, DCM_CODE) Dcm_Svc2FReturnControlToEcu(
  void
  )
{
  if (Dcm_Svc2FSingletonContext.HasAnyActiveIoDid == TRUE)
  {
    Dcm_CfgDidMgrDidOpClassHandleOptType opInfoRefPage = 0;
    Dcm_CfgDidMgrIoDidHandleOptType ioDidIter;

    Dcm_Svc2FSingletonContext.HasAnyActiveIoDid = FALSE;

    for (ioDidIter = 0; ioDidIter < Dcm_UtiGenericBitSetCalcSize(DCM_NUM_IODIDS); ioDidIter++)
    {
      Dcm_UtiBitSetBasePtrType pActiveIoDids = Dcm_Svc2FGetActiveIoDids(ioDidIter);
      Dcm_CfgDidMgrDidOpClassHandleOptType opInfoRef = opInfoRefPage;
      DCM_UTI_LOOP_BIT_SCAN(*pActiveIoDids)                                                                                                          /* SBSW_DCM_POINTER_WRITE_2FACTIVEIODIDS */
      {
        if (Dcm_UtiBitOpTest(Dcm_UtiBitSetBaseType, *pActiveIoDids, 0x01u))
        {
          Dcm_DidMgrIoControlRtrnCtrl2Ecu(Dcm_DidMgrGetCtrlOpInfoRef(opInfoRef));
        }
        ++opInfoRef;
      }
      opInfoRefPage += (Dcm_CfgDidMgrDidOpClassHandleOptType)Dcm_UtiGetNumBitsOfXintType(Dcm_UtiBitSetBaseType);/* next bunch of IoDids */           /* PRQA S 2983 */ /* MD_Dcm_Redundant_2983 */
    }
  }
}
# endif

# if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2FOnStateChanged()
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
FUNC(void, DCM_CODE) Dcm_Svc2FOnStateChanged(
  void
  )
{
  if (Dcm_Svc2FSingletonContext.HasAnyActiveIoDid == TRUE)
  {
    Dcm_CfgDidMgrDidOpClassHandleOptType opInfoRefPage = 0;
    Dcm_CfgDidMgrIoDidHandleOptType ioDidIter;

    Dcm_Svc2FSingletonContext.HasAnyActiveIoDid = FALSE; /* Assume all IODIDs will be deactivated */

    for (ioDidIter = 0; ioDidIter < Dcm_UtiGenericBitSetCalcSize(DCM_NUM_IODIDS); ioDidIter++)
    {
      Dcm_UtiBitSetBasePtrType pActiveIoDids = Dcm_Svc2FGetActiveIoDids(ioDidIter);
      Dcm_UtiBitSetBaseType lActiveIoDids = *pActiveIoDids;
      Dcm_UtiBitSetBaseType lActiveIoDidScanner = 0x01u;
      Dcm_CfgDidMgrDidOpClassHandleOptType opInfoRef = opInfoRefPage;

      DCM_UTI_LOOP_BIT_SCAN(lActiveIoDids)
      {
        if (Dcm_UtiBitOpTest(Dcm_UtiBitSetBaseType, *pActiveIoDids, lActiveIoDidScanner))
        {
          Dcm_NegativeResponseCodeType lNrc;
          Std_ReturnType lStdReturn;

          /*
          * No Authentication check needed as service 2F is only allowed in the extended session,
          * and therefore no other tester has access to the DCM during that time.
          */
          lStdReturn = Dcm_DidMgrStaticDidNoAuthStateCheck(Dcm_DidMgrGetCtrlOpExecCondRef(opInfoRef), &lNrc);                                        /* SBSW_DCM_POINTER_FORWARD_STACK */
          if (lStdReturn == DCM_E_NOT_OK)
          {
            Dcm_UtiBitOpClr(Dcm_UtiBitSetBaseType, *pActiveIoDids, lActiveIoDidScanner);                                                             /* SBSW_DCM_POINTER_WRITE_2FACTIVEIODIDS */
            Dcm_DidMgrIoControlRtrnCtrl2Ecu(Dcm_DidMgrGetCtrlOpInfoRef(opInfoRef));
          }
        }

        lActiveIoDidScanner <<= 1;
        ++opInfoRef;
      }

      /* If still any IODID active in this row: */
      if (*pActiveIoDids != 0u)
      {
        /* Update IODID activity statistics */
        Dcm_Svc2FSingletonContext.HasAnyActiveIoDid = TRUE;
      }

      opInfoRefPage += (Dcm_CfgDidMgrDidOpClassHandleOptType)Dcm_UtiGetNumBitsOfXintType(Dcm_UtiBitSetBaseType);/* next bunch of IoDids */           /* PRQA S 2983 */ /* MD_Dcm_Redundant_2983 */
    }
  }
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Service2FInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Service2FInit(
  void
  )
{
# if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
  Dcm_CfgDidMgrIoDidHandleOptType ioDidIter;

  Dcm_Svc2FSingletonContext.HasAnyActiveIoDid = FALSE;
  for (ioDidIter = 0; ioDidIter < Dcm_UtiGenericBitSetCalcSize(DCM_NUM_IODIDS); ++ioDidIter)
  {
    Dcm_UtiBitSetBasePtrType pActiveIoDids = Dcm_Svc2FGetActiveIoDids(ioDidIter);
    *pActiveIoDids = 0u;                                                                                                                             /* SBSW_DCM_POINTER_WRITE_2FACTIVEIODIDS */
  }
# endif
}

/**********************************************************************************************************************
 *  Dcm_Service2FProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service2FProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc2FRepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc2F;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Svc2FHandler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_SVC2F_PROGRESS_CHECKACCESS:
    lStdResult = Dcm_Svc2FCheckAccess(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_SVC2F_PROGRESS_EXECUTEOP:
    lStdResult = Dcm_Svc2FExecuteOp(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_SVC2F_PROGRESS_GETLENGTH:
    lStdResult = Dcm_Svc2FGetLength(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  case DCM_SVC2F_PROGRESS_READDATA:
    lStdResult = Dcm_Svc2FReadData(opStatus, pMsgContext, ErrorCode, pRepContext);                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    break;
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Service2FPostProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service2FPostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_APIStd_3673 */
  Dcm_ConfirmationStatusType status
  )
{
  DCM_IGNORE_UNREF_PARAM(status);                                                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID2F);
}

/**********************************************************************************************************************
 *  Dcm_Service2FCancel()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service2FCancel(
  Dcm_ContextPtrType pContext                                                                                                                        /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SID2F);
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_2F_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc2F.c
 *********************************************************************************************************************/
