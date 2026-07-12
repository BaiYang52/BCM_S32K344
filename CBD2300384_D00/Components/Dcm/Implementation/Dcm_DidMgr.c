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
/**        \file  Dcm_DidMgr.c
 *         \unit  DidMgr
 *        \brief  Contains the implementation of DID Manager unit.
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
#define DCM_DIDMGR_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_DidMgr.h"
#if (DCM_DIDMGR_SUPPORT_ENABLED == STD_ON)
# include "Dcm_CfgWrap.h"
# include "Dcm_Debug.h"
# include "Dcm_RsrcMgr.h"
# include "Dcm_State.h"
# include "Dcm_Uti.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
# define DCM_DIDMGR_OPCLS_READCHK_COND_SYNC                          ((Dcm_DidMgrOpClassType)0x0200)
# define DCM_DIDMGR_OPCLS_READCHK_COND_ASYNC                         ((Dcm_DidMgrOpClassType)0x0201)

# define DCM_DIDMGR_OPCLS_READLENGTH_RANGE                           ((Dcm_DidMgrOpClassType)0x0100)
# define DCM_DIDMGR_OPCLS_READLENGTH_SYNC                            ((Dcm_DidMgrOpClassType)0x0101)
# define DCM_DIDMGR_OPCLS_READLENGTH_ASYNC                           ((Dcm_DidMgrOpClassType)0x0102)

# define DCM_DIDMGR_OPCLS_ISDIDAVAILABLE                             ((Dcm_DidMgrOpClassType)0x5400)

# define DCM_DIDMGR_LOOKUP_STATE_DIDRANGE                            ((Dcm_DidMgrDidLookUpStateType)0)
# define DCM_DIDMGR_LOOKUP_STATE_CONCRETEDID                         ((Dcm_DidMgrDidLookUpStateType)1)
# define DCM_DIDMGR_LOOKUP_STATE_DIDFILTER_EXTERN                    ((Dcm_DidMgrDidLookUpStateType)2)

# define DCM_DIDMGR_RANGELOOKUPRSLT_NOMATCH                          ((Dcm_DidMgrRangeLookUpResultType)0x00) /* viswse: to be clarified */
# define DCM_DIDMGR_RANGELOOKUPRSLT_MATCHED_SUPPORTED                ((Dcm_DidMgrRangeLookUpResultType)0x01)
# define DCM_DIDMGR_RANGELOOKUPRSLT_MATCHED_UNSUPPORTED              ((Dcm_DidMgrRangeLookUpResultType)0x02)
# define DCM_DIDMGR_RANGELOOKUPRSLT_MATCHED_ANY                      ((Dcm_DidMgrRangeLookUpResultType)(DCM_DIDMGR_RANGELOOKUPRSLT_MATCHED_SUPPORTED | DCM_DIDMGR_RANGELOOKUPRSLT_MATCHED_UNSUPPORTED))
# define DCM_DIDMGR_RANGELOOKUPRSLT_PENDING                          ((Dcm_DidMgrRangeLookUpResultType)0x04)

/* Service port operation classes */
# define DCM_DIDMGR_OPCLS_READ_RANGE                                 ((Dcm_DidMgrOpClassType)0x0000)
# define DCM_DIDMGR_OPCLS_READ_SYNC                                  ((Dcm_DidMgrOpClassType)0x0001)
# define DCM_DIDMGR_OPCLS_READ_ASYNC                                 ((Dcm_DidMgrOpClassType)0x0002)
# define DCM_DIDMGR_OPCLS_READ_PAGED                                 ((Dcm_DidMgrOpClassType)0x0003)
# define DCM_DIDMGR_OPCLS_READ_VID                                   ((Dcm_DidMgrOpClassType)0x0004)
# define DCM_DIDMGR_OPCLS_READ_GAP                                   ((Dcm_DidMgrOpClassType)0x0006)
# define DCM_DIDMGR_OPCLS_READ_WRAPPER_ASYNC                         ((Dcm_DidMgrOpClassType)0x0007)
# define DCM_DIDMGR_OPCLS_READ_WRAPPER_SYNC                          ((Dcm_DidMgrOpClassType)0x0008)

# define DCM_DIDMGR_OPCLS_WRITE_RANGE                                ((Dcm_DidMgrOpClassType)0x1000)
# define DCM_DIDMGR_OPCLS_WRITE_NLEN_SYNC_NRES_ERROR                 ((Dcm_DidMgrOpClassType)0x1001)
# define DCM_DIDMGR_OPCLS_WRITE_NLEN_ASYNC_NRES_ERROR                ((Dcm_DidMgrOpClassType)0x1002)
# define DCM_DIDMGR_OPCLS_WRITE_LEN_SYNC_NRES_ERROR                  ((Dcm_DidMgrOpClassType)0x1003)
# define DCM_DIDMGR_OPCLS_WRITE_LEN_ASYNC_NRES_ERROR                 ((Dcm_DidMgrOpClassType)0x1004)
# define DCM_DIDMGR_OPCLS_WRITE_NLEN_SYNC_NRES_NERROR                ((Dcm_DidMgrOpClassType)0x1005)
# define DCM_DIDMGR_OPCLS_WRITE_GAP                                  ((Dcm_DidMgrOpClassType)0x1006)

# define DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_NCEMR_ERROR         ((Dcm_DidMgrOpClassType)0x2000)
# define DCM_DIDMGR_OPCLS_IO_REQ_NLEN_SYNC_NRES_NCEMR_ERROR          ((Dcm_DidMgrOpClassType)0x2001)
# define DCM_DIDMGR_OPCLS_IO_REQ_LEN_SYNC_NRES_NCEMR_ERROR           ((Dcm_DidMgrOpClassType)0x2002)
# define DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_NCEMR_ERROR        ((Dcm_DidMgrOpClassType)0x2003)
# define DCM_DIDMGR_OPCLS_IO_REQ_NLEN_ASYNC_NRES_NCEMR_ERROR         ((Dcm_DidMgrOpClassType)0x2004)
# define DCM_DIDMGR_OPCLS_IO_REQ_LEN_ASYNC_NRES_NCEMR_ERROR          ((Dcm_DidMgrOpClassType)0x2005)

# define DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_CEMR8_ERROR         ((Dcm_DidMgrOpClassType)0x2006)
# define DCM_DIDMGR_OPCLS_IO_REQ_NLEN_SYNC_NRES_CEMR8_ERROR          ((Dcm_DidMgrOpClassType)0x2007)
# define DCM_DIDMGR_OPCLS_IO_REQ_LEN_SYNC_NRES_CEMR8_ERROR           ((Dcm_DidMgrOpClassType)0x2008)
# define DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_CEMR8_ERROR        ((Dcm_DidMgrOpClassType)0x2009)
# define DCM_DIDMGR_OPCLS_IO_REQ_NLEN_ASYNC_NRES_CEMR8_ERROR         ((Dcm_DidMgrOpClassType)0x200A)
# define DCM_DIDMGR_OPCLS_IO_REQ_LEN_ASYNC_NRES_CEMR8_ERROR          ((Dcm_DidMgrOpClassType)0x200B)

# define DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_CEMR16_ERROR        ((Dcm_DidMgrOpClassType)0x200C)
# define DCM_DIDMGR_OPCLS_IO_REQ_NLEN_SYNC_NRES_CEMR16_ERROR         ((Dcm_DidMgrOpClassType)0x200D)
# define DCM_DIDMGR_OPCLS_IO_REQ_LEN_SYNC_NRES_CEMR16_ERROR          ((Dcm_DidMgrOpClassType)0x200E)
# define DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_CEMR16_ERROR       ((Dcm_DidMgrOpClassType)0x200F)
# define DCM_DIDMGR_OPCLS_IO_REQ_NLEN_ASYNC_NRES_CEMR16_ERROR        ((Dcm_DidMgrOpClassType)0x2010)
# define DCM_DIDMGR_OPCLS_IO_REQ_LEN_ASYNC_NRES_CEMR16_ERROR         ((Dcm_DidMgrOpClassType)0x2011)

# define DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_CEMR32_ERROR        ((Dcm_DidMgrOpClassType)0x2012)
# define DCM_DIDMGR_OPCLS_IO_REQ_NLEN_SYNC_NRES_CEMR32_ERROR         ((Dcm_DidMgrOpClassType)0x2013)
# define DCM_DIDMGR_OPCLS_IO_REQ_LEN_SYNC_NRES_CEMR32_ERROR          ((Dcm_DidMgrOpClassType)0x2014)
# define DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_CEMR32_ERROR       ((Dcm_DidMgrOpClassType)0x2015)
# define DCM_DIDMGR_OPCLS_IO_REQ_NLEN_ASYNC_NRES_CEMR32_ERROR        ((Dcm_DidMgrOpClassType)0x2016)
# define DCM_DIDMGR_OPCLS_IO_REQ_LEN_ASYNC_NRES_CEMR32_ERROR         ((Dcm_DidMgrOpClassType)0x2017)

# define DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_CEMRN_ERROR         ((Dcm_DidMgrOpClassType)0x2018)
# define DCM_DIDMGR_OPCLS_IO_REQ_NLEN_SYNC_NRES_CEMRN_ERROR          ((Dcm_DidMgrOpClassType)0x2019)
# define DCM_DIDMGR_OPCLS_IO_REQ_LEN_SYNC_NRES_CEMRN_ERROR           ((Dcm_DidMgrOpClassType)0x201A)
# define DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_CEMRN_ERROR        ((Dcm_DidMgrOpClassType)0x201B)
# define DCM_DIDMGR_OPCLS_IO_REQ_NLEN_ASYNC_NRES_CEMRN_ERROR         ((Dcm_DidMgrOpClassType)0x201C)
# define DCM_DIDMGR_OPCLS_IO_REQ_LEN_ASYNC_NRES_CEMRN_ERROR          ((Dcm_DidMgrOpClassType)0x201D)

# define DCM_DIDMGR_OPCLS_IO_GAP                                     ((Dcm_DidMgrOpClassType)0x201E)

# define DCM_DIDMGR_OPCLS_GETSCALING_SYNC                            ((Dcm_DidMgrOpClassType)0x3000)
# define DCM_DIDMGR_OPCLS_GETSCALING_ASYNC                           ((Dcm_DidMgrOpClassType)0x3001)

# define DCM_DIDMGR_OPCLS_DEFINE                                     ((Dcm_DidMgrOpClassType)0x4000)

# define DCM_DIDMGR_IO_CEMR_HANDLING_NO                              ((Dcm_DidMgrIoDidCemrHandlingType)0)
# define DCM_DIDMGR_IO_CEMR_HANDLING_INTERNAL                        ((Dcm_DidMgrIoDidCemrHandlingType)1)
# define DCM_DIDMGR_IO_CEMR_HANDLING_EXTERNAL                        ((Dcm_DidMgrIoDidCemrHandlingType)2)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# define Dcm_DidMgrGetCtrlEnblMaskHandling(pDidInfo)                 (Dcm_CfgWrapDidMgrOpInfoIoControl(Dcm_DidMgrOpInfoRedirector((pDidInfo)->OpRef, OpRefIoControl)).CtrlEnblMaskHandling) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/* Read CheckCondition port operations */
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncReadCheckCondSyncType)(
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncReadCheckCondAsyncType)(
  Dcm_OpStatusType opStatus,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/* Read Length port operations */
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncReadDidRangeLengthType)(
  uint16 did,
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidLengthPtrType Length
  );

typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncReadLengthSyncType)(
  Dcm_DidMgrDidLengthPtrType Length
  );

typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncReadLengthAsyncType)(
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidLengthPtrType Length
  );

# if (DCM_DIDMGR_OPTYPE_RANGE_ISAVAIL_ENABLED == STD_ON)
/* IsAvailable range DID port operations */
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncRangeIsAvailDidType)(
  uint16 did,
  Dcm_OpStatusType opStatus,
  P2VAR(Dcm_DidSupportedType, AUTOMATIC, DCM_VAR_NOINIT) isAvailable
  );
# endif

/* ScalingInformation port operations */
# if (DCM_DIDMGR_OPTYPE_GETSCALINGINFO_ENABLED == STD_ON)
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncGetScalingInfoSyncType)(
  Dcm_MsgType data,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncGetScalingInfoAsyncType)(
  Dcm_OpStatusType opStatus,
  Dcm_MsgType data,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_DIDMGR_OPTYPE_WRITE_ENABLED == STD_ON)
/* Write port operations */
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncWriteNLenSyncNResErrorType)(
  Dcm_MsgType data,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncWriteNLenAsyncNResErrorType)(
  Dcm_MsgType data,
  Dcm_OpStatusType opStatus,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncWriteLenAsyncNResErrorType)(
  Dcm_MsgType data,
  Dcm_DidMgrDidLengthType Length,
  Dcm_OpStatusType opStatus,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncWriteLenSyncNResErrorType)(
  Dcm_MsgType data,
  Dcm_DidMgrDidLengthType Length,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncWriteNLenSyncNResNErrorType)(
  Dcm_MsgType data
  );
# endif

# if (DCM_DIDMGR_OPTYPE_WRITE_ENABLED == STD_ON)
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncWriteDidRangeType)(
  uint16 did,
  Dcm_MsgType data,
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidLengthType Length,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

/* Read port operations */
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncReadSyncType)(
  Dcm_MsgType data
  );

typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncReadAsyncType)(
  Dcm_OpStatusType opStatus,
  Dcm_MsgType data
  );

# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncReadPagedDidType)(
  Dcm_OpStatusType opStatus,
  Dcm_MsgType data,
  Dcm_DidMgrDidLengthPtrType length
  );
# endif

typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncReadDidRangeType)(
  uint16 did,
  Dcm_MsgType data,
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidLengthPtrType length,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncReadWrapperAsyncType)(
  uint16 did,
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext
  );

typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncReadWrapperSyncType)(
  Dcm_DiagDataContextPtrType pDataContext
  );

typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncReadVidType)(
  Dcm_OpStatusType opStatus,
  Dcm_MsgType data
# if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
  ,Dcm_Uint8VarDataPtrType Length
# endif
  );

# if (DCM_SVC_2F_SUPPORT_ENABLED == STD_ON)
/* IO Control port operations */

/* No CEMR */
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoNReqNLenSyncNCemrErrorType)(
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoNReqNLenASyncNCemrErrorType)(
  Dcm_OpStatusType opStatus,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqNLenSyncNCemrErrorType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqLenSyncNCemrErrorType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_DidMgrDidLengthType Length,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqNLenASyncNCemrErrorType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_OpStatusType opStatus,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqLenASyncNCemrErrorType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_DidMgrDidLengthType Length,
  Dcm_OpStatusType opStatus,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
/* CEMR 8Bit */
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoNReqNLenSyncCemr8ErrorType)(
  uint8 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoNReqNLenASyncCemr8ErrorType)(
  Dcm_OpStatusType opStatus,
  uint8 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqNLenSyncCemr8ErrorType)(
  Dcm_ReadOnlyMsgType data,
  uint8 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqLenSyncCemr8ErrorType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_DidMgrDidLengthType Length,
  uint8 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqNLenASyncCemr8ErrorType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_OpStatusType opStatus,
  uint8 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqLenASyncCemr8ErrorType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_DidMgrDidLengthType Length,
  Dcm_OpStatusType opStatus,
  uint8 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
/* CEMR 16Bit */
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoNReqNLenSyncCemr16ErrorType)(
  uint16 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoNReqNLenASyncCemr16ErrorType)(
  Dcm_OpStatusType opStatus,
  uint16 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode);
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqNLenSyncCemr16ErrorType)(
  Dcm_ReadOnlyMsgType data,
  uint16 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqLenSyncCemr16ErrorType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_DidMgrDidLengthType Length,
  uint16 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode);
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqNLenASyncCemr16ErrorType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_OpStatusType opStatus,
  uint16 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode);
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqLenASyncCemr16ErrorType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_DidMgrDidLengthType Length,
  Dcm_OpStatusType opStatus,
  uint16 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
/* CEMR 32Bit */
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoNReqNLenSyncCemr32ErrorType)(
  uint32 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoNReqNLenASyncCemr32ErrorType)(
  Dcm_OpStatusType opStatus,
  uint32 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqNLenSyncCemr32ErrorType)(
  Dcm_ReadOnlyMsgType data,
  uint32 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqLenSyncCemr32ErrorType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_DidMgrDidLengthType Length,
  uint32 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqNLenASyncCemr32ErrorType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_OpStatusType opStatus,
  uint32 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqLenASyncCemr32ErrorType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_DidMgrDidLengthType Length,
  Dcm_OpStatusType opStatus,
  uint32 cemr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
/* CEMR NBit */
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoNReqNLenSyncCemrNErrorType)(
  Dcm_ReadOnlyMsgType data,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoNReqNLenASyncCemrNErrorType)(
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgType data,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqNLenSyncCemrNErrorType)(
  Dcm_ReadOnlyMsgType reqData,
  Dcm_ReadOnlyMsgType data,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqLenSyncCemrNErrorType)(
  Dcm_ReadOnlyMsgType reqData,
  Dcm_DidMgrDidLengthType Length,
  Dcm_ReadOnlyMsgType data,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqNLenASyncCemrNErrorType)(
  Dcm_ReadOnlyMsgType reqData,
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgType data,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DidMgrOpFuncIoReqLenASyncCemrNErrorType)(
  Dcm_ReadOnlyMsgType reqData,
  Dcm_DidMgrDidLengthType Length,
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgType data,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
/*! DID manager Secure Coding Type */
struct DCM_DIDMGRSECURECODINGTYPE_TAG
{
  Dcm_DiagDataContextType    DataContext;                                   /*!< Utility data context that stores current usage of the local Secure Coding buffer */
  uint8                      Buffer[DCM_DIDMGR_SECURE_CODING_BUFFER_SIZE];  /*!< Local Secure Coding buffer */
  boolean                    Started;                                       /*!< Secure Coding Sequence State (Started = TRUE or FALSE) */
  boolean                    Locked;                                        /*!< Secure Coding Sequence State (Locked = TRUE or FALSE) */
  uint8                      NumDids;                                       /*!< Number of stored DIDs */
};
typedef struct DCM_DIDMGRSECURECODINGTYPE_TAG Dcm_DidMgrSecureCodingType;
# endif

# if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
/*! DID manager context */
struct DCM_DIDMGRCONTEXTTYPE_TAG
{
  Dcm_DidMgrSecureCodingType SecureCoding;  /*!< Keeps the Secure Coding state. */
};
typedef struct DCM_DIDMGRCONTEXTTYPE_TAG Dcm_DidMgrSingletonContextType;
# endif

typedef P2CONST(Dcm_CfgDidMgrDidInfoType, TYPEDEF, DCM_CONST) Dcm_DidMgrDidInfoPtrType;
typedef P2CONST(Dcm_CfgDidMgrDidOpInfoType, TYPEDEF, DCM_CONST) Dcm_DidMgrDidOpInfoPtrType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_DidMgrReadCheckCondSignal()
 *********************************************************************************************************************/
/*! \brief          Reads the check condition of a specific signal of a DID.
 *  \details        -
 *  \param[in]      opStatus           The operation status
 *  \param[in,out]  pDidInfoContext    The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext  The context of the DID operation
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK           Check condition of signal failed
 *  \return         DCM_E_PENDING      Check condition not yet finished
 *  \return         DCM_E_NOT_OK       Check condition of signal succeeded
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadCheckCondSignal(
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_DidMgrReadLengthSignal()
 *********************************************************************************************************************/
/*! \brief          Returns the length of a signal.
 *  \details        Reads the length of a specific signal of a DID.
 *  \param[in]      opStatus           The operation status
 *  \param[in,out]  pDidInfoContext    The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext  The context of the DID operation
 *  \param[out]     pResLength         The returned length
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_PENDING      Job processing is not yet finished
 *  \return         DCM_E_OK           Job processing finished, send positive response
 *  \return         DCM_E_NOT_OK       Job processing finished, send NRC from the ErrorCode
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadLengthSignal(
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_DidMgrDidLengthPtrType pResLength,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_DidMgrFilterDidLookUp()
 *********************************************************************************************************************/
/*! \brief          Verifies whether a supported DID is currently enabled for a specific DID operation.
 *  \details        -
 *  \param[in]      pDidInfoContext    The DID configuration information
 *  \param[in]      didOp              The lookup DID operation context
 *  \return         DCM_E_OK           Positive look up result
 *  \return         DCM_E_NOT_OK       Negative look up result
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrFilterDidLookUp(
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrOpOptType didOp
  );

# if (DCM_DIDMGR_OPTYPE_RANGE_ISAVAIL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrFilterDidRangeGap()
 *********************************************************************************************************************/
/*! \brief          Verifies whether the DID is supported within a range.
 *  \details        -
 *  \param[in]      opStatus                    Current operation status
 *  \param[in]      pDidInfoContext             The DID configuration context
 *  \param[in]      pDidOpTypeContext           The context of the DID operation
 *  \return         DCM_E_OK                    Positive look up result
 *  \return         DCM_E_NOT_OK                Negative look up result
 *  \return         DCM_E_LOOKUP_MATCH_FOUND    DID range match, but found a gap
 *  \return         DCM_E_PENDING               Final result is pending, retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrFilterDidRangeGap(
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext
  );
# endif

/**********************************************************************************************************************
 *  Dcm_DidMgrDidRangeLookUp()
 *********************************************************************************************************************/
/*! \brief          Searches for the DID in a range to get its corresponding handle (up to 32768 items).
 *  \details        -
 *  \param[in]      opStatus                    Current operation status
 *  \param[in,out]  pDidInfoContext             The DID to be found and its context
 *  \param[in,out]  pDidOpTypeContext           The context of the DID operation
 *  \param[in]      didOp                       The DID operation
 *  \param[in]      filter                      Whether the DID shall be filterable by the application
 *  \return         DCM_E_OK                    Positive look up result
 *  \return         DCM_E_NOT_OK                Negative look up result
 *  \return         DCM_E_LOOKUP_MATCH_FOUND    DID range match, but found a gap
 *  \return         DCM_E_PENDING               Final result is pending, retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrDidRangeLookUp(
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_DidMgrOpOptType didOp,
  boolean filter
  );

# if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrIoControlRtrnCtrl2EcuSignal()
 *********************************************************************************************************************/
/*! \brief          Optimized function for ReturnControlToEcu only!.
 *  \details        -
 *  \param[in]      opInfoRef          Operation information
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DidMgrIoControlRtrnCtrl2EcuSignal(
  Dcm_CfgDidMgrSignalOpClassRefOptType opInfoRef
  );
# endif

# if (DCM_SVC_2F_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrIoControlSignal()
 *********************************************************************************************************************/
/*! \brief          DID IO control operator.
 *  \details        -
 *  \param[in]      opStatus            The operation status
 *  \param[in,out]  pMsgContext         The current request context
 *  \param[in,out]  pDidInfoContext     The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext   The context of the DID operation
 *  \param[out]     ErrorCode           Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK            Operation is done positive
 *  \return         DCM_E_NOT_OK        Operation is done negative
 *  \return         DCM_E_PENDING       Final result is pending, retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrIoControlSignal(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_SVC_2F_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrIoControlWithMask()
 *********************************************************************************************************************/
/*! \brief          DID IO control with control enable mask record support operator.
 *  \details        -
 *  \param[in]      opStatus           The operation status
 *  \param[in,out]  pMsgContext        The current request context
 *  \param[in,out]  pDidInfoContext    The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext  The context of the DID operation
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \return         DCM_E_PENDING      Final result is pending, retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrIoControlWithMask(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_DIDMGR_OPTYPE_WRITE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrWriteAtomicSRResultInterpreter()
 *********************************************************************************************************************/
/*! \brief          Converts a ModeSwitchAck API return value to a DCM standard result.
 *  \details        -
 *  \param[in]      rteResult          The RTE result
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrWriteAtomicSRResultInterpreter(
  Std_ReturnType rteResult,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_DIDMGR_OPTYPE_WRITE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrWriteSignal()
 *********************************************************************************************************************/
/*! \brief          Writes a single DID signal for service 0x2E.
 *  \details        -
 *  \param[in]      opStatus           The operation status
 *  \param[in,out]  pMsgContext        The current request context
 *  \param[in,out]  pDidInfoContext    The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext  The context of the DID operation
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \return         DCM_E_PENDING      Final result is pending, retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrWriteSignal(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_DIDMGR_OPTYPE_GETSCALINGINFO_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrGetScalingInfoSignal()
 *********************************************************************************************************************/
/*! \brief          Returns a scaling information item of a given DID.
 *  \details        -
 *  \param[in]      opStatus           The operation status
 *  \param[in,out]  pMsgContext        The current request context
 *  \param[in,out]  pDidInfoContext    The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext  The context of the DID operation
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \return         DCM_E_PENDING      Final result is pending, retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrGetScalingInfoSignal(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_DIDMGR_OPTYPE_READ_ENABLED == STD_ON)                                                                                                      /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_DidMgrGetSignalLength()
 *********************************************************************************************************************/
/*! \brief          Returns the length of the current signal.
 *  \details        -
 *  \param[in,out]  pDidInfoContext    The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext  The context of the DID operation
 *  \return         Signal length
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_CfgNetBufferSizeOptType, DCM_CODE) Dcm_DidMgrGetSignalLength(
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext
  );
# endif

# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED== STD_ON) && \
     (DCM_DIDMGR_RTE_IMPLICIT_COM_ENABLED  == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrGetRequiredSignalBufSize()
 *********************************************************************************************************************/
/*! \brief          Returns the required buffer size for a read callout of a signal.
 *  \details        -
 *  \param[in]      pDidInfoContext    The context of the DID configuration
 *  \param[in]      pDidOpTypeContext  The context of the DID operation
 *  \return         The required buffer size
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            OpClass of the requested DID must be set.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Dcm_CfgNetBufferSizeOptType, DCM_CODE) Dcm_DidMgrGetRequiredSignalBufSize(
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext
  );
# endif

/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignalRange()
 *********************************************************************************************************************/
/*! \brief          Reads a specific DID range.
 *  \details        -
 *  \param[in]      opStatus           The operation status
 *  \param[in]      pSignalOpClassInfo The pointer to the signal operation class information
 *  \param[in,out]  pDataContext       The pointer to the data context
 *  \param[in,out]  pDidInfoContext    The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext  The context of the DID operation
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \return         DCM_E_PENDING      Final result is pending, retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignalRange(
  Dcm_OpStatusType opStatus,
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

# if (DCM_DIDMGR_OPTYPE_READ_ENABLED == STD_ON)                                                                                                      /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignalAsync()
 *********************************************************************************************************************/
/*! \brief          Reads an asynchronous signal of a DID.
 *  \details        -
 *  \param[in]      opStatus           The operation status
 *  \param[in]      pSignalOpClassInfo The pointer to the signal operation class information
 *  \param[in,out]  pDataContext       The pointer to the data context
 *  \param[in,out]  pDidInfoContext    The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext  The context of the DID operation
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \return         DCM_E_PENDING      Final result is pending, retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignalAsync(
  Dcm_OpStatusType opStatus,
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_DIDMGR_OPTYPE_READ_ENABLED == STD_ON)                                                                                                      /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignalSync()
 *********************************************************************************************************************/
/*! \brief          Reads a synchronous signal of a DID.
 *  \details        -
 *  \param[in]      opStatus           The operation status
 *  \param[in]      pSignalOpClassInfo The pointer to the signal operation class information
 *  \param[in,out]  pDataContext       The pointer to the data context
 *  \param[in,out]  pDidInfoContext    The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext  The context of the DID operation
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignalSync(
  Dcm_OpStatusType opStatus,
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignalPaged()
 *********************************************************************************************************************/
/*! \brief          Reads a paged signal of a DID.
 *  \details        -
 *  \param[in]      opStatus           The operation status
 *  \param[in]      pSignalOpClassInfo The pointer to the signal operation class information
 *  \param[in,out]  pDataContext       The pointer to the data context
 *  \param[in,out]  pDidInfoContext    The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext  The context of the DID operation
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \return         DCM_E_PENDING      Final result is pending, retry later
 *  \return         DCM_E_BUFFERTOOLOW Retry later, because of no available space any more
 *  \return         DCM_E_LOOP         Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignalPaged(
  Dcm_OpStatusType opStatus,
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignalVid()
 *********************************************************************************************************************/
/*! \brief          Reads a paged signal of a VID.
 *  \details        -
 *  \param[in]      opStatus           The operation status
 *  \param[in]      pSignalOpClassInfo The pointer to the signal operation class information
 *  \param[in,out]  pDataContext       The pointer to the data context
 *  \param[in,out]  pDidInfoContext    The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext  The context of the DID operation
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \return         DCM_E_PENDING      Final result is pending, retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignalVid(
  Dcm_OpStatusType opStatus,
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

# if (DCM_DIDMGR_OPTYPE_READ_ENABLED == STD_ON)                                                                                                      /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignalWrapperAsync()
 *********************************************************************************************************************/
/*! \brief          Reads a signal of a DID using a generated wrapper.
 *  \details        -
 *  \param[in]      opStatus           The operation status
 *  \param[in]      pSignalOpClassInfo The pointer to the signal operation class information
 *  \param[in,out]  pDataContext       The pointer to the data context
 *  \param[in,out]  pDidInfoContext    The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext  The context of the DID operation
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \return         DCM_E_PENDING      Final result is pending, retry later
 *  \return         DCM_E_FORCE_RCRRP  Forces a RCR-RP response
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignalWrapperAsync(
  Dcm_OpStatusType opStatus,
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_DIDMGR_OPTYPE_READ_ENABLED == STD_ON)                                                                                                      /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignalWrapperSync()
 *********************************************************************************************************************/
/*! \brief          Reads a signal of a DID using a generated wrapper.
 *  \details        -
 *  \param[in]      opStatus           The operation status
 *  \param[in]      pSignalOpClassInfo The pointer to the signal operation class information
 *  \param[in,out]  pDataContext       The pointer to the data context
 *  \param[in,out]  pDidInfoContext    The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext  The context of the DID operation
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignalWrapperSync(
  Dcm_OpStatusType opStatus,
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_DIDMGR_OPTYPE_READ_ENABLED == STD_ON)                                                                                                      /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignal()
 *********************************************************************************************************************/
/*! \brief          Reads a signal of a DID.
 *  \details        -
 *  \param[in]      opStatus           The operation status
 *  \param[in,out]  pDataContext       The pointer to the data context
 *  \param[in,out]  pDidInfoContext    The context of the DID configuration
 *  \param[in,out]  pDidOpTypeContext  The context of the DID operation
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \return         DCM_E_PENDING      Final result is pending, retry later
 *  \return         DCM_E_FORCE_RCRRP  Forces a RCR-RP response
 *  \return         DCM_E_BUFFERTOOLOW Retry later, because of no available space any more
 *  \return         DCM_E_LOOP         Repeat immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignal(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
# endif

# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrCheckSignalSize()
 *********************************************************************************************************************/
/*! \brief          Checks whether the DCM buffer is large enough to read the next signal.
 *  \details        -
 *  \param[in]      pDataContext       The pointer to the data context
 *  \param[in,out]  pDidInfoContext    The DID configuration context
 *  \param[in,out]  pDidOpTypeContext  The DID operation context
 *  \return         TRUE               The buffer is large enough
 *  \return         FALSE              The buffer is too small
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_DidMgrCheckSignalSize(
  Dcm_DiagDataContextConstPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext
  );
# endif

/**********************************************************************************************************************
 *  Dcm_DidMgrReadMultiSignal()
 *********************************************************************************************************************/
/*! \brief          Reads all signals for a specific DID using the paged-buffer.
 *  \details        -
 *  \param[in,out]  pDataContext       The pointer to the data context
 *  \param[in,out]  pDidInfoContext    The DID configuration context
 *  \param[in,out]  pDidOpTypeContext  The DID operation context
 *  \param[out]     ErrorCode          Negative response code in case return value is DCM_E_NOT_OK
 *  \return         DCM_E_OK           Operation is done positive
 *  \return         DCM_E_NOT_OK       Operation is done negative
 *  \return         DCM_E_PENDING      Final result is pending, retry later
 *  \return         DCM_E_FORCE_RCRRP  Forces a RCR-RP response
 *  \return         DCM_E_BUFFERTOOLOW Retry later, because of no available space any more
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadMultiSignal(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

# if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrRestoreDidInfo()
 *********************************************************************************************************************/
/*! \brief          Restores the information gathered by Dcm_DidMgrGetDidInfo() without filtering.
 *  \details        Preconditions (e.g. session or security check) are not evaluated.
 *  \param[in,out]  pDidInfoContext      Contains DID Information
 *  \param[in,out]  pDidOpTypeContext    Contains DID operation state
 *  \param[in]      didOp                The lookup DID operation context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    FALSE
 *  \pre            The DID number must be set by the caller (pDidInfoContext->Did is valid).
 *                  Not allowed to be called for DIDs with variable length, since only the minimum and maximal length
 *                  will be restored.
 *                  Shall only be called if Dcm_DidMgrGetDidInfo() was sucessfully called before for the same DID.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DidMgrRestoreDidInfo(
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_DidMgrOpOptType didOp
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_VAR_NO_INIT_8
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
/*!< CEMR session mask (only used in case of ReturnControlToEcu) */
DCM_LOCAL Dcm_MsgItemType Dcm_DidMgrIoControlCemrSessionMask[DCM_DIDMGR_IODID_MAX_EXT_CEMR_SIZE];
# endif
# define DCM_STOP_SEC_VAR_NO_INIT_8
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
/*! Singleton context of DidMgr unit */
DCM_LOCAL VAR(Dcm_DidMgrSingletonContextType, DCM_VAR_NOINIT) Dcm_DidMgrSingletonContext;
# endif
# define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_DidMgrReadCheckCondSignal()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadCheckCondSignal(
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo;

  DCM_IGNORE_UNREF_PARAM(pDidInfoContext);                                                                                                           /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_CfgWrapDidMgrSignalOpClassInfoGetEntry(pDidOpTypeContext->Base.SignalOpClassInfoRef
                                                         ,&pSignalOpClassInfo
                                                         ,ErrorCode);                                                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    switch(pSignalOpClassInfo->FuncClass)
    {
    case DCM_DIDMGR_OPCLS_READCHK_COND_SYNC:
      lStdResult = ((Dcm_DidMgrOpFuncReadCheckCondSyncType)pSignalOpClassInfo->OpFunc)(ErrorCode);                                                   /* PRQA S 0313 */ /* MD_Dcm_0313 */ /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;
    case DCM_DIDMGR_OPCLS_READCHK_COND_ASYNC:
      lStdResult = ((Dcm_DidMgrOpFuncReadCheckCondAsyncType)pSignalOpClassInfo->OpFunc)(opStatus                                                     /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                       ,ErrorCode);                                                  /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;
    default:
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INVALID_CONFIG);
      break;
    }

    if(opStatus == DCM_CANCEL)
    {
      *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }

    /* Catch any RTE or unexpected specific errors */
    if( (lStdResult != DCM_E_OK)
      &&(lStdResult != DCM_E_PENDING)
      &&(lStdResult != DCM_E_NOT_OK)
      )
    {
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }

    Dcm_UtiHandleApplNrc(lStdResult, ErrorCode, DCM_E_CONDITIONSNOTCORRECT);                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_DidMgrReadLengthSignal()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadLengthSignal(
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidLengthPtrType pResLength,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo;

  DCM_IGNORE_UNREF_PARAM(pDidInfoContext);                                                                                                           /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_CfgWrapDidMgrSignalOpClassInfoGetEntry(pDidOpTypeContext->Base.SignalOpClassInfoRef
                                                         ,&pSignalOpClassInfo
                                                         ,ErrorCode);                                                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    switch(pSignalOpClassInfo->FuncClass)
    {
    case DCM_DIDMGR_OPCLS_READLENGTH_SYNC:
      lStdResult = ((Dcm_DidMgrOpFuncReadLengthSyncType)pSignalOpClassInfo->OpFunc)(pResLength);                                                     /* PRQA S 0313 */ /* MD_Dcm_0313 */ /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;

    case DCM_DIDMGR_OPCLS_READLENGTH_ASYNC:
      lStdResult = ((Dcm_DidMgrOpFuncReadLengthAsyncType)pSignalOpClassInfo->OpFunc)(opStatus                                                        /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                    ,pResLength);                                                    /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;

    case DCM_DIDMGR_OPCLS_READLENGTH_RANGE:
      lStdResult = ((Dcm_DidMgrOpFuncReadDidRangeLengthType)pSignalOpClassInfo->OpFunc)(pDidInfoContext->Did                                         /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                       ,opStatus
                                                                                       ,pResLength);                                                 /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;

    default:
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INVALID_CONFIG);
      break;
    }

    if(opStatus == DCM_CANCEL)
    {
      *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }

    if(lStdResult == DCM_E_NOT_OK)
    {
      *ErrorCode = DCM_E_CONDITIONSNOTCORRECT; /* return NRC CNC since no error code supported by the application API */                             /* SBSW_DCM_PARAM_PTR_WRITE */
    }

    /* Catch any RTE or unexpected specific errors */
    if( (lStdResult != DCM_E_OK)
      &&(lStdResult != DCM_E_PENDING)
      &&(lStdResult != DCM_E_NOT_OK)
      )
    {
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_DidMgrFilterDidLookUp()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrFilterDidLookUp(
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrOpOptType didOp
  )
{
  Std_ReturnType             lStdReturn = DCM_E_OK;
  Dcm_DidMgrDidOpInfoPtrType pDidOpInfoPtr;

  DCM_IGNORE_UNREF_PARAM(didOp);                                                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  pDidOpInfoPtr = Dcm_CfgDidMgrGetDidOpInfo(pDidInfoContext);

# if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
  if(Dcm_DiagIsEnabledInActiveVariants(pDidOpInfoPtr->ExecCondRef) == FALSE)                                                                         /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
  {
    /* Not supported in current variant */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
# endif
  /* Check session state */
  if(!Dcm_StateIsSupportedInSession(Dcm_StateGetPreconditionStates(pDidOpInfoPtr->ExecCondRef)))
  {
    /* Not supported in current session */
    lStdReturn = DCM_E_NOT_OK;
  }
  else
  {
# if (DCM_SVC_22_OBD_CALIBRATION_ENABLED == STD_ON) && \
     (DCM_OBDUDSIDMGR_SUPPORT_ENABLED == STD_ON)
    /* Perform this check only on supported DIDs ! */
    if( (pDidInfoContext->Did >= (uint16)0xF400u)
      &&(pDidInfoContext->Did <= (uint16)0xF8FFu)
#  if (DCM_OBDUDSIDMGR_UDSMID_SUPPORT_BY_DEM_ENABLED == STD_ON)
      /* Already filtered in the IsDidAvailable operation of the 0xF6xx DID range.
       * Note: Dcm_CfgSvc22SupportedIdMask does not contain any information for this DID range -> exclude from indexing.
       */
      &&((pDidInfoContext->Did & 0xF600u) != 0xF600u)
#  endif
      )
    {
      if(Dcm_ObdIdMgrIsIdEnabled(Dcm_UtiGetLoByte(pDidInfoContext->Did),
                                 Dcm_CfgWrapSvc22SupportedIdMask(Dcm_CfgWrapSvc22SupportedIdMaskOffset(Dcm_UtiGetHiByte(pDidInfoContext->Did)-(uint8)0xF4u))) == FALSE) /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
      {
        /* OBD DID not enabled */
        lStdReturn = DCM_E_NOT_OK;
      }/* else - still enabled */
    }
    else/* not an (WWH-)OBD DID */
# endif
    {
# if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
      if(didOp != DCM_DIDMGR_OP_DEFINE) /* isolate SID 0x2C - Define or Clear on not defined DDDID is always allowed! */
      {
        if(!Dcm_Svc2CIsDidDefined(Dcm_CfgWrapDidMgrDidInfo(pDidInfoContext->Descriptor.DidInfoIdx)))                                                 /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
        {
          /* not yet defined for other operations than DEFINE */
          lStdReturn = DCM_E_NOT_OK;
        }/* else - defined for other operations */
      }
# endif
    }
  }

  return lStdReturn;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */

# if (DCM_DIDMGR_OPTYPE_RANGE_ISAVAIL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrFilterDidRangeGap()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrFilterDidRangeGap(
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  /* If the DID range supports gaps: */
  if(Dcm_DidMgrIsOpTypeSupported(Dcm_CfgDidMgrGetDidOpInfo(pDidInfoContext), DCM_DIDMGR_OPTYPE_RANGEISDIDAVAILABLE))
  {
    Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo;
    Dcm_DidSupportedType               lIsAvailable = DCM_DID_NOT_SUPPORTED;
    Dcm_NegativeResponseCodeType       lNrc;

    /* Execute operation for evaluation gap match */
    Dcm_DidMgrInitOpClassInfo(pDidInfoContext, DCM_DIDMGR_OPTYPE_RANGEISDIDAVAILABLE);                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    Dcm_DidMgrInitOpTypeHandler(pDidInfoContext, pDidOpTypeContext);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

    lStdResult = Dcm_CfgWrapDidMgrSignalOpClassInfoGetEntry(pDidOpTypeContext->Base.SignalOpClassInfoRef
                                                           ,&pSignalOpClassInfo
                                                           ,&lNrc);                                                                                  /* SBSW_DCM_COMB_PTR_FORWARD */

    if(lStdResult == DCM_E_OK)                                                                                                                       /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
    {
      Dcm_DebugAssert(pSignalOpClassInfo->FuncClass == DCM_DIDMGR_OPCLS_ISDIDAVAILABLE
                     ,DCM_SID_INTERNAL
                     ,DCM_E_ILLEGAL_STATE);

      lStdResult = ((Dcm_DidMgrOpFuncRangeIsAvailDidType)pSignalOpClassInfo->OpFunc)(pDidInfoContext->Did                                            /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                    ,opStatus
                                                                                    ,&lIsAvailable);                                                 /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */ /* SBSW_DCM_PARAM_PTR_FORWARD */

      switch (lStdResult)
      {
      case DCM_E_OK:
        /* If acknowledges that the DID is still valid: */
        if (lIsAvailable != DCM_DID_SUPPORTED)
        {
          lStdResult = DCM_E_LOOKUP_MATCH_FOUND;
        }
        break;
      case DCM_E_PENDING:
        /* If operation needs more time (DCM_E_PENDING) */
        /* nothing to do */
        break;
      case DCM_E_NOT_OK:
        lStdResult = DCM_E_LOOKUP_MATCH_FOUND;
        break;
      default:
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
        lStdResult = DCM_E_LOOKUP_MATCH_FOUND;
        break;
      }
    }
  }
  /* Otherwise: all DIDs within that range are valid: return DCM_E_OK */
  return lStdResult;
}
# endif

/**********************************************************************************************************************
 *  Dcm_DidMgrDidRangeLookUp()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrDidRangeLookUp(
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrOpOptType didOp,
  boolean filter
  )
{
  Std_ReturnType lStdReturn;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pDidOpTypeContext);                                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(filter);                                                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

# if (DCM_DIDMGR_OPTYPE_RANGE_ISAVAIL_ENABLED == STD_ON)
  lStdReturn = DCM_E_OK;

  if(opStatus == DCM_INITIAL)
# endif
  {
    sint16_least lResult;

    lStdReturn = DCM_E_NOT_OK;

    /* Try finding a range that encloses this DID */
    lResult = Dcm_UtiLookUpRangeUint16(Dcm_CfgDidMgrDidRanges, pDidInfoContext->Did);                                                                /* SBSW_DCM_POINTER_FORWARD_GLOBAL */
    /* If a match found:  */
    if(lResult >= 0)
    {
      Dcm_DidMgrDidInfoPtrType pDidInfo = Dcm_CfgWrapDidMgrDidInfo(Dcm_CfgDidMgrRangeDidInfoRefs[lResult]);

      /* If the matched range supports the requested DID operation: */
      if(Dcm_DidMgrIsOpSupported(pDidInfo, didOp))
      {
        /* Prepare for positive final result; initialize DID context */
        pDidInfoContext->Descriptor.DidInfoIdx = Dcm_CfgDidMgrRangeDidInfoRefs[lResult];                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
        /* Set DID length. Later the concrete length will be stored here. Take the maximum length! */
        pDidInfoContext->DidLength = (Dcm_DidMgrDidLengthType)pDidInfo->MaxLength;                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
        pDidInfoContext->ReservedLength = (Dcm_DidMgrDidLengthType)pDidInfo->ReservedLength;                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
        /* Store DID operation information */
        Dcm_DidMgrInitOpInfo(pDidInfoContext, didOp);                                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

        lStdReturn = DCM_E_OK;
      }
      /* Otherwise: Operation not supported -> DID not supported in context of the requested operation */
    }
  }
  /* Otherwise: keep current result: DCM_E_OK */

# if (DCM_DIDMGR_OPTYPE_RANGE_ISAVAIL_ENABLED == STD_ON)
  /* If DID match found: */
  if( (lStdReturn == DCM_E_OK)
    &&(filter == TRUE) )
  {
    /* Process potentially available DID range gaps, resp. re-call any pending operation */
    lStdReturn = Dcm_DidMgrFilterDidRangeGap(opStatus, pDidInfoContext, pDidOpTypeContext);                                                          /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  /* Otherwise: return final result: DCM_E_NOT_OK, DCM_E_PENDING */
# endif

  return lStdReturn;
}

# if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrIoControlRtrnCtrl2EcuSignal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DidMgrIoControlRtrnCtrl2EcuSignal(
  Dcm_CfgDidMgrSignalOpClassRefOptType opInfoRef
  )
{
  Std_ReturnType                     lStdResult;
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo;
  Dcm_NegativeResponseCodeType       lNrc;

  lStdResult = Dcm_CfgWrapDidMgrSignalOpClassInfoGetEntry(opInfoRef
                                                         ,&pSignalOpClassInfo
                                                         ,&lNrc);                                                                                    /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    switch(pSignalOpClassInfo->FuncClass)
    {
    case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_NCEMR_ERROR:
      lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenSyncNCemrErrorType)pSignalOpClassInfo->OpFunc)(                                                       /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                      &lNrc);                                                        /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;
    case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_NCEMR_ERROR:
      lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenASyncNCemrErrorType)pSignalOpClassInfo->OpFunc)(DCM_INITIAL                                           /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                      ,&lNrc);                                                       /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;

    case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_CEMR8_ERROR:
      lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenSyncCemr8ErrorType)pSignalOpClassInfo->OpFunc)(0xFFu                                                  /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                     ,&lNrc);                                                        /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;
    case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_CEMR8_ERROR:
      lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenASyncCemr8ErrorType)pSignalOpClassInfo->OpFunc)(DCM_INITIAL                                           /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                      ,0xFFu
                                                                                      ,&lNrc);                                                       /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;
    case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_CEMR16_ERROR:
      lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenSyncCemr16ErrorType)pSignalOpClassInfo->OpFunc)(0xFFFFu                                               /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                      ,&lNrc);                                                       /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;
    case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_CEMR16_ERROR:
      lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenASyncCemr16ErrorType)pSignalOpClassInfo->OpFunc)(DCM_INITIAL                                          /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                       ,0xFFFFu
                                                                                       ,&lNrc);                                                      /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;
    case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_CEMR32_ERROR:
      lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenSyncCemr32ErrorType)pSignalOpClassInfo->OpFunc)(0xFFFFFFFFu                                           /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                      ,&lNrc);                                                       /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;
    case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_CEMR32_ERROR:
      lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenASyncCemr32ErrorType)pSignalOpClassInfo->OpFunc)(DCM_INITIAL                                          /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                       ,0xFFFFFFFFu
                                                                                       ,&lNrc);                                                      /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;

    case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_CEMRN_ERROR:
      lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenSyncCemrNErrorType)pSignalOpClassInfo->OpFunc)(                                                       /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                     Dcm_DidMgrIoControlCemrSessionMask
                                                                    ,&lNrc);                                                                         /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;
    case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_CEMRN_ERROR:
      lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenASyncCemrNErrorType)pSignalOpClassInfo->OpFunc)(DCM_INITIAL                                           /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                    ,Dcm_DidMgrIoControlCemrSessionMask
                                                                    ,&lNrc);                                                                         /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
      break;

    case DCM_DIDMGR_OPCLS_IO_GAP:
      /* Just a gap, do nothing */
      lStdResult = DCM_E_OK;
      break;
    default:
      lStdResult = DCM_E_NOT_OK;
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INVALID_CONFIG);
      break;
    }
    Dcm_DebugAssert(lStdResult != DCM_E_PENDING, DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);

    /*
    No need to handle application NRC since not used!
    Dcm_UtiHandleApplNrc(lStdResult, &lNrc, DCM_E_CONDITIONSNOTCORRECT);
    */
  }
  DCM_IGNORE_UNREF_PARAM(lStdResult);                                                                                                                /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
}                                                                                                                                                    /* PRQA S 6030 */ /* MD_MSR_STCYC */
# endif

# if (DCM_SVC_2F_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrIoControlSignal()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrIoControlSignal(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo;

  lStdResult = Dcm_CfgWrapDidMgrSignalOpClassInfoGetEntry(pDidOpTypeContext->Base.SignalOpClassInfoRef
                                                         ,&pSignalOpClassInfo
                                                         ,ErrorCode);                                                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    lStdResult = Dcm_UtiMsgContextVerifySize(pMsgContext, pSignalOpClassInfo->MaxLength, ErrorCode);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

    if(lStdResult == DCM_E_OK)                                                                                                                       /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
    {
      switch(pSignalOpClassInfo->FuncClass)
      {
    /* No CEMR */
      case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_NCEMR_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenSyncNCemrErrorType)pSignalOpClassInfo->OpFunc)(ErrorCode);                                          /* PRQA S 0313 */ /* MD_Dcm_0313 */ /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_NCEMR_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenASyncNCemrErrorType)pSignalOpClassInfo->OpFunc)(opStatus                                            /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                                ,ErrorCode);                                         /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_NLEN_SYNC_NRES_NCEMR_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqNLenSyncNCemrErrorType)pSignalOpClassInfo->OpFunc)(                                                      /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                           Dcm_UtiGetReqData(pMsgContext)                            /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                                          ,ErrorCode);                                               /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_LEN_SYNC_NRES_NCEMR_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqLenSyncNCemrErrorType)pSignalOpClassInfo->OpFunc)(                                                       /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                          Dcm_UtiGetReqData(pMsgContext)                                             /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                         ,(Dcm_DidMgrDidLengthType)pMsgContext->reqDataLen
                                                                         ,ErrorCode);                                                                /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_NLEN_ASYNC_NRES_NCEMR_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqNLenASyncNCemrErrorType)pSignalOpClassInfo->OpFunc)(                                                     /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                           Dcm_UtiGetReqData(pMsgContext)                            /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                                          ,opStatus
                                                                                          ,ErrorCode);                                               /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_LEN_ASYNC_NRES_NCEMR_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqLenASyncNCemrErrorType)pSignalOpClassInfo->OpFunc)(                                                      /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                          Dcm_UtiGetReqData(pMsgContext)                                             /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                         ,(Dcm_DidMgrDidLengthType)pMsgContext->reqDataLen
                                                                         ,opStatus
                                                                         ,ErrorCode);                                                                /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;

    /* CEMR 8Bit */
      case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_CEMR8_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenSyncCemr8ErrorType)pSignalOpClassInfo->OpFunc)(                                                     /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                               (uint8)pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                              ,ErrorCode);                                                                           /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_CEMR8_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenASyncCemr8ErrorType)pSignalOpClassInfo->OpFunc)(opStatus                                            /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                              ,(uint8)pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                              ,ErrorCode);                                                                           /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_NLEN_SYNC_NRES_CEMR8_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqNLenSyncCemr8ErrorType)pSignalOpClassInfo->OpFunc)(Dcm_UtiGetReqData(pMsgContext)                        /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */
                                                              ,(uint8)pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                              ,ErrorCode);                                                                           /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_LEN_SYNC_NRES_CEMR8_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqLenSyncCemr8ErrorType)pSignalOpClassInfo->OpFunc)(Dcm_UtiGetReqData(pMsgContext)                         /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */
                                                              ,(Dcm_DidMgrDidLengthType)pMsgContext->reqDataLen
                                                              ,(uint8)pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                              ,ErrorCode);                                                                           /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_NLEN_ASYNC_NRES_CEMR8_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqNLenASyncCemr8ErrorType)pSignalOpClassInfo->OpFunc)(Dcm_UtiGetReqData(pMsgContext)                       /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */
                                                              ,opStatus
                                                              ,(uint8)pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                              ,ErrorCode);                                                                           /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_LEN_ASYNC_NRES_CEMR8_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqLenASyncCemr8ErrorType)pSignalOpClassInfo->OpFunc)(Dcm_UtiGetReqData(pMsgContext)                        /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */
                                                              ,(Dcm_DidMgrDidLengthType)pMsgContext->reqDataLen
                                                              ,opStatus
                                                              ,(uint8)pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                              ,ErrorCode);                                                                           /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;

    /* CEMR 16Bit */
      case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_CEMR16_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenSyncCemr16ErrorType)pSignalOpClassInfo->OpFunc)(                                                    /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                              (uint16)pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                             ,ErrorCode);                                                                            /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_CEMR16_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenASyncCemr16ErrorType)pSignalOpClassInfo->OpFunc)(opStatus                                           /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                             ,(uint16)pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                             ,ErrorCode);                                                                            /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_NLEN_SYNC_NRES_CEMR16_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqNLenSyncCemr16ErrorType)pSignalOpClassInfo->OpFunc)(Dcm_UtiGetReqData(pMsgContext)                       /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */
                                                             ,(uint16)pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                             ,ErrorCode);                                                                            /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_LEN_SYNC_NRES_CEMR16_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqLenSyncCemr16ErrorType)pSignalOpClassInfo->OpFunc)(                                                      /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                              Dcm_UtiGetReqData(pMsgContext)                                                         /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                             ,(Dcm_DidMgrDidLengthType)pMsgContext->reqDataLen
                                                             ,(uint16)pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                             ,ErrorCode);                                                                            /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_NLEN_ASYNC_NRES_CEMR16_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqNLenASyncCemr16ErrorType)pSignalOpClassInfo->OpFunc)(                                                    /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                              Dcm_UtiGetReqData(pMsgContext)                                                         /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                             ,opStatus
                                                             ,(uint16)pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                             ,ErrorCode);                                                                            /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_LEN_ASYNC_NRES_CEMR16_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqLenASyncCemr16ErrorType)pSignalOpClassInfo->OpFunc)(                                                     /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                              Dcm_UtiGetReqData(pMsgContext)                                                         /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                             ,(Dcm_DidMgrDidLengthType)pMsgContext->reqDataLen
                                                             ,opStatus
                                                             ,(uint16)pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                             ,ErrorCode);                                                                            /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;

    /* CEMR 32Bit */
      case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_CEMR32_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenSyncCemr32ErrorType)pSignalOpClassInfo->OpFunc)(                                                    /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                      pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                                     ,ErrorCode);                                                                    /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_CEMR32_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenASyncCemr32ErrorType)pSignalOpClassInfo->OpFunc)(opStatus                                           /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                     ,pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                                     ,ErrorCode);                                                                    /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_NLEN_SYNC_NRES_CEMR32_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqNLenSyncCemr32ErrorType)pSignalOpClassInfo->OpFunc)(                                                     /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                      Dcm_UtiGetReqData(pMsgContext)                                                 /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                     ,pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                                     ,ErrorCode);                                                                    /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_LEN_SYNC_NRES_CEMR32_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqLenSyncCemr32ErrorType)pSignalOpClassInfo->OpFunc)(                                                      /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                      Dcm_UtiGetReqData(pMsgContext)                                                 /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                     ,(Dcm_DidMgrDidLengthType)pMsgContext->reqDataLen
                                                                     ,pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                                     ,ErrorCode);                                                                    /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_NLEN_ASYNC_NRES_CEMR32_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqNLenASyncCemr32ErrorType)pSignalOpClassInfo->OpFunc)(                                                    /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                      Dcm_UtiGetReqData(pMsgContext)                                                 /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                     ,opStatus
                                                                     ,pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                                     ,ErrorCode);                                                                    /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_LEN_ASYNC_NRES_CEMR32_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqLenASyncCemr32ErrorType)pSignalOpClassInfo->OpFunc)(                                                     /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                      Dcm_UtiGetReqData(pMsgContext)                                                 /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                     ,(Dcm_DidMgrDidLengthType)pMsgContext->reqDataLen
                                                                     ,opStatus
                                                                     ,pDidOpTypeContext->OpType.Io.ControlEnableMaskRecord
                                                                     ,ErrorCode);                                                                    /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;

    /* CEMR NBit */
      case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_SYNC_NRES_CEMRN_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenSyncCemrNErrorType)pSignalOpClassInfo->OpFunc)(                                                     /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                pDidOpTypeContext->OpType.Io.EnableMaskPtr
                                                                               ,ErrorCode);                                                          /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_NREQ_NLEN_ASYNC_NRES_CEMRN_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoNReqNLenASyncCemrNErrorType)pSignalOpClassInfo->OpFunc)(opStatus                                            /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                               ,pDidOpTypeContext->OpType.Io.EnableMaskPtr
                                                                               ,ErrorCode);                                                          /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_NLEN_SYNC_NRES_CEMRN_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqNLenSyncCemrNErrorType)pSignalOpClassInfo->OpFunc)(                                                      /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */
                                                                                Dcm_UtiGetReqData(pMsgContext)                                       /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                               ,pDidOpTypeContext->OpType.Io.EnableMaskPtr
                                                                               ,ErrorCode);                                                          /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_LEN_SYNC_NRES_CEMRN_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqLenSyncCemrNErrorType)pSignalOpClassInfo->OpFunc)(                                                       /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                          Dcm_UtiGetReqData(pMsgContext)                                             /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                         ,(Dcm_DidMgrDidLengthType)pMsgContext->reqDataLen
                                                                         ,pDidOpTypeContext->OpType.Io.EnableMaskPtr
                                                                         ,ErrorCode);                                                                /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_NLEN_ASYNC_NRES_CEMRN_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqNLenASyncCemrNErrorType)pSignalOpClassInfo->OpFunc)(                                                     /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                Dcm_UtiGetReqData(pMsgContext)                                       /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                               ,opStatus
                                                                               ,pDidOpTypeContext->OpType.Io.EnableMaskPtr
                                                                               ,ErrorCode);                                                          /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_REQ_LEN_ASYNC_NRES_CEMRN_ERROR:
        lStdResult = ((Dcm_DidMgrOpFuncIoReqLenASyncCemrNErrorType)pSignalOpClassInfo->OpFunc)(                                                      /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                          Dcm_UtiGetReqData(pMsgContext)                                             /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                         ,(Dcm_DidMgrDidLengthType)pMsgContext->reqDataLen
                                                                         ,opStatus
                                                                         ,pDidOpTypeContext->OpType.Io.EnableMaskPtr
                                                                         ,ErrorCode);                                                                /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */
        break;
      case DCM_DIDMGR_OPCLS_IO_GAP:
        /* Just a gap, do nothing */
        lStdResult = DCM_E_OK;
        break;
      default:
        DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                            /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
        DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
        DCM_IGNORE_UNREF_PARAM(pDidInfoContext);                                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
        DCM_IGNORE_UNREF_PARAM(pDidOpTypeContext);                                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INVALID_CONFIG);
        break;
      }

      if(opStatus == DCM_CANCEL)
      {
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }

      if(lStdResult == DCM_E_OK)
      {
        /* Consuming request data is not necessary for the dynamic signal because it is the last signal */
        if(pSignalOpClassInfo->MinLength != 0u)
        {
          Dcm_UtiConsumeReqData(pMsgContext, pSignalOpClassInfo->MaxLength);                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
        }
      }

      /* Catch any RTE or unexpected specific errors */
      if( (lStdResult != DCM_E_OK)
        &&(lStdResult != DCM_E_PENDING)
        &&(lStdResult != DCM_E_NOT_OK)
        )
      {
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }

      Dcm_UtiHandleApplNrc(lStdResult, ErrorCode, DCM_E_CONDITIONSNOTCORRECT);                                                                       /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
# endif

# if (DCM_SVC_2F_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrIoControlWithMask()
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
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrIoControlWithMask(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;
  Dcm_OpStatusType lOpStatus = opStatus;

  if (lOpStatus == DCM_INITIAL)
  {
    /* DID handler is already initialized in Dcm_DidMgrIoControl() */
    pDidOpTypeContext->OpType.Io.OperExecuted= FALSE;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
    pDidOpTypeContext->OpType.Io.SignalIter  = 0u;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    pDidOpTypeContext->OpType.Io.BitScanner  = 0x80u;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
  }

  DCM_UTI_LOOP_FOR_EVER                                                                                                                              /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
  {
    if(Dcm_UtiBitOpTest(Dcm_MsgItemType
                       ,pDidOpTypeContext->OpType.Io.EnableMaskPtr[0]
                       ,pDidOpTypeContext->OpType.Io.BitScanner))
    {
      pDidOpTypeContext->OpType.Io.OperExecuted = TRUE;                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */

      lStdResult = Dcm_DidMgrIoControlSignal(lOpStatus
                                            ,pMsgContext
                                            ,pDidInfoContext
                                            ,pDidOpTypeContext
                                            ,ErrorCode);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */

      if(lStdResult != DCM_E_OK)
      {
        break;
      }
    }
    else
    {
      /* signal not allowed -> skip */
      Dcm_UtiConsumeReqData(pMsgContext
                           ,Dcm_CfgWrapDidMgrSignalOpClassInfo(pDidOpTypeContext->Base.SignalOpClassInfoRef)->MaxLength);                            /* SBSW_DCM_PARAM_PTR_FORWARD */
    }

    /* next signal */
    pDidOpTypeContext->OpType.Io.SignalIter++;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
    if(pDidOpTypeContext->OpType.Io.SignalIter >= pDidOpTypeContext->Base.NumSignals)
    {
      break;
    }

    /* Gaps are not considered as signals by control enable mask, so skip them here */
    if(Dcm_CfgWrapDidMgrSignalOpClassInfo(pDidOpTypeContext->Base.SignalOpClassInfoRef)->FuncClass != DCM_DIDMGR_OPCLS_IO_GAP)
    {
      pDidOpTypeContext->OpType.Io.BitScanner >>= 1;                                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
      if(pDidOpTypeContext->OpType.Io.BitScanner == 0u)
      {
        pDidOpTypeContext->OpType.Io.BitScanner = 0x80u;                                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
        Dcm_UtiNextItemByPtr(pDidOpTypeContext->OpType.Io.EnableMaskPtr,1);                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      }
    }

    /* Go on with signal to be handled now */
    pDidOpTypeContext->Base.SignalOpClassInfoRef++;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    lOpStatus = DCM_INITIAL;/* reset op status */
  }

  if(pDidOpTypeContext->OpType.Io.OperExecuted == FALSE)
  {
    /* the enable mask record did not enable any signal */
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  return lStdResult;
}                                                                                                                                                    /* PRQA S 6010 */ /* MD_MSR_STPTH */
# endif

# if (DCM_DIDMGR_OPTYPE_WRITE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrWriteAtomicSRResultInterpreter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrWriteAtomicSRResultInterpreter(
  Std_ReturnType rteResult,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  switch(rteResult)
  {
  case RTE_E_OK:
    /* switch accomplished */
    lStdResult = DCM_E_OK;
    break;
  case RTE_E_INVALID:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_GENERALPROGRAMMINGFAILURE;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  case RTE_E_HARD_TRANSFORMER_ERROR:
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_REQUESTOUTOFRANGE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  default: /* any error situation */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    break;
  }
  return lStdResult;
}
# endif

# if (DCM_DIDMGR_OPTYPE_WRITE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrWriteSignal()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrWriteSignal(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo;

  lStdResult = Dcm_CfgWrapDidMgrSignalOpClassInfoGetEntry(pDidOpTypeContext->Base.SignalOpClassInfoRef
                                                         ,&pSignalOpClassInfo
                                                         ,ErrorCode);                                                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    switch(pSignalOpClassInfo->FuncClass)
    {
    case DCM_DIDMGR_OPCLS_WRITE_RANGE:
      lStdResult = ((Dcm_DidMgrOpFuncWriteDidRangeType)pSignalOpClassInfo->OpFunc)(pDidInfoContext->Did                                              /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                       ,Dcm_UtiGetReqData(pMsgContext)                                               /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                       ,opStatus
                                                                       ,(Dcm_DidMgrDidLengthType)pMsgContext->reqDataLen
                                                                       ,ErrorCode);                                                                  /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */ /* SBSW_DCM_COMB_PTR_FORWARD */
      break;

    case DCM_DIDMGR_OPCLS_WRITE_LEN_SYNC_NRES_ERROR:
      lStdResult = ((Dcm_DidMgrOpFuncWriteLenSyncNResErrorType)pSignalOpClassInfo->OpFunc)(                                                          /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                        Dcm_UtiGetReqData(pMsgContext)                                               /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                       ,(Dcm_DidMgrDidLengthType)pMsgContext->reqDataLen
                                                                       ,ErrorCode);                                                                  /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */ /* SBSW_DCM_COMB_PTR_FORWARD */
      break;

    case DCM_DIDMGR_OPCLS_WRITE_LEN_ASYNC_NRES_ERROR:
      lStdResult = ((Dcm_DidMgrOpFuncWriteLenAsyncNResErrorType)pSignalOpClassInfo->OpFunc)(                                                         /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                        Dcm_UtiGetReqData(pMsgContext)                                               /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                       ,(Dcm_DidMgrDidLengthType)pMsgContext->reqDataLen
                                                                       ,opStatus
                                                                       ,ErrorCode);                                                                  /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */ /* SBSW_DCM_COMB_PTR_FORWARD */
      break;

    case DCM_DIDMGR_OPCLS_WRITE_NLEN_SYNC_NRES_ERROR:
      lStdResult = ((Dcm_DidMgrOpFuncWriteNLenSyncNResErrorType)pSignalOpClassInfo->OpFunc)(                                                         /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                         Dcm_UtiGetReqData(pMsgContext)                              /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                                        ,ErrorCode);                                                 /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */ /* SBSW_DCM_COMB_PTR_FORWARD */
      break;

    case DCM_DIDMGR_OPCLS_WRITE_NLEN_ASYNC_NRES_ERROR:
      lStdResult = ((Dcm_DidMgrOpFuncWriteNLenAsyncNResErrorType)pSignalOpClassInfo->OpFunc)(                                                        /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                         Dcm_UtiGetReqData(pMsgContext)                              /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                                                                                        ,opStatus
                                                                                        ,ErrorCode);                                                 /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */ /* SBSW_DCM_COMB_PTR_FORWARD */
      break;
    case DCM_DIDMGR_OPCLS_WRITE_NLEN_SYNC_NRES_NERROR:
      lStdResult = ((Dcm_DidMgrOpFuncWriteNLenSyncNResNErrorType)pSignalOpClassInfo->OpFunc)(                                                        /* PRQA S 0313 */ /* MD_Dcm_0313 */ /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */ /* SBSW_DCM_COMB_PTR_FORWARD */
                                                                                       Dcm_UtiGetReqData(pMsgContext));                              /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

      lStdResult = Dcm_DidMgrWriteAtomicSRResultInterpreter(lStdResult, ErrorCode);                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;
    case DCM_DIDMGR_OPCLS_WRITE_GAP:
      /* Just a gap, do nothing */
      lStdResult = DCM_E_OK;
      break;

    default:
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INVALID_CONFIG);
      break;
    }

    if(opStatus == DCM_CANCEL)
    {
      *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }

    if(lStdResult == DCM_E_OK)
    {
      /* Consuming request data is not necessary for the last signal. */
      if(pSignalOpClassInfo->MinLength != 0u)
      {
        Dcm_UtiConsumeReqData(pMsgContext, pSignalOpClassInfo->MaxLength);                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
    }

    /* Catch any RTE or unexpected specific errors */
    if( (lStdResult != DCM_E_OK)
      &&(lStdResult != DCM_E_PENDING)
      &&(lStdResult != DCM_E_NOT_OK)
      )
    {
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
# endif

# if (DCM_DIDMGR_OPTYPE_GETSCALINGINFO_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrGetScalingInfoSignal()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrGetScalingInfoSignal(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo;

  lStdResult = Dcm_CfgWrapDidMgrSignalOpClassInfoGetEntry(pDidOpTypeContext->Base.SignalOpClassInfoRef
                                                         ,&pSignalOpClassInfo
                                                         ,ErrorCode);                                                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    lStdResult = Dcm_UtiMsgContextVerifySize(pMsgContext, pSignalOpClassInfo->MaxLength, ErrorCode);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

    if(lStdResult == DCM_E_OK)                                                                                                                       /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
    {
      switch(pSignalOpClassInfo->FuncClass)
      {
      case DCM_DIDMGR_OPCLS_GETSCALING_SYNC:
        DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                            /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
        lStdResult = ((Dcm_DidMgrOpFuncGetScalingInfoSyncType)pSignalOpClassInfo->OpFunc)(Dcm_UtiGetResData(pMsgContext)                             /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                                                         ,ErrorCode);                                                /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */ /* SBSW_DCM_COMB_PTR_FORWARD */
        break;
      case DCM_DIDMGR_OPCLS_GETSCALING_ASYNC:
        lStdResult = ((Dcm_DidMgrOpFuncGetScalingInfoAsyncType)pSignalOpClassInfo->OpFunc)(opStatus                                                  /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                          ,Dcm_UtiGetResData(pMsgContext)                            /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                                                          ,ErrorCode);                                               /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */ /* SBSW_DCM_COMB_PTR_FORWARD */
        break;
      default:
        DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                            /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
        DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
        DCM_IGNORE_UNREF_PARAM(pDidInfoContext);                                                                                                     /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INVALID_CONFIG);
        break;
      }

      if(opStatus == DCM_CANCEL)
      {
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }

      if (lStdResult == DCM_E_OK)
      {
        Dcm_UtiProvideResData(pMsgContext, pSignalOpClassInfo->MaxLength);                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
      } /* Catch any RTE or unexpected specific errors */
      else if( (lStdResult != DCM_E_NOT_OK)
             &&(lStdResult != DCM_E_PENDING)
        )
      {
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_NOT_OK;
      }
      else
      {
        /* Do nothing */
      }
    }
  }

  return lStdResult;
}
# endif

# if (DCM_DIDMGR_OPTYPE_READ_ENABLED == STD_ON)                                                                                                      /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_DidMgrGetSignalLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Dcm_CfgNetBufferSizeOptType, DCM_CODE) Dcm_DidMgrGetSignalLength(
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext                                                                                                /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_CfgNetBufferSizeOptType        lSignalLength;
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo;
  Std_ReturnType                     lStdResult;
  Dcm_NegativeResponseCodeType       lNrc;

  lStdResult = Dcm_CfgWrapDidMgrSignalOpClassInfoGetEntry(pDidOpTypeContext->Base.SignalOpClassInfoRef
                                                         ,&pSignalOpClassInfo
                                                         ,&lNrc);                                                                                    /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    /* If the signal has a variable length: */
    if(pSignalOpClassInfo->MinLength == 0u)
    {
      /* Calculate the length of the signal. A signal with variable length is always the last signal of the DID */
      lSignalLength = (Dcm_CfgNetBufferSizeOptType)(pDidInfoContext->DidLength)
                    - (Dcm_CfgNetBufferSizeOptType)(pDidOpTypeContext->OpType.Read.Progress);
    }
    else
    {
      /* Otherwise return the signal length */
      lSignalLength = (Dcm_CfgNetBufferSizeOptType)pSignalOpClassInfo->MinLength;
    }
  }
  else
  {
    lSignalLength = 0u;
  }

  return lSignalLength;
}
# endif

# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON) && \
     (DCM_DIDMGR_RTE_IMPLICIT_COM_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrGetRequiredSignalBufSize()
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
DCM_LOCAL_INLINE FUNC(Dcm_CfgNetBufferSizeOptType, DCM_CODE) Dcm_DidMgrGetRequiredSignalBufSize(
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext                                                                                                /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_CfgNetBufferSizeOptType        lSignalLength;
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo;
  Std_ReturnType                     lStdResult;
  Dcm_NegativeResponseCodeType       lNrc;

  DCM_IGNORE_UNREF_PARAM(pDidInfoContext);                                                                                                           /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_CfgWrapDidMgrSignalOpClassInfoGetEntry(pDidOpTypeContext->Base.SignalOpClassInfoRef
                                                         ,&pSignalOpClassInfo
                                                         ,&lNrc);                                                                                    /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    /* If the signal is part of a paged DID (Note: OpClass needs to be set -> precondition) */
    if (pSignalOpClassInfo->FuncClass == DCM_DIDMGR_OPCLS_READ_PAGED)
    {
      /* Return size of dedicated read buffer as it determines the page size */
      lSignalLength = (Dcm_CfgNetBufferSizeOptType)DCM_DIDMGR_READ_PAGE_SIZE;
    }
    else /* Signal cannot be part of a DynDID because DynDIDs are not allowed when paged buffer is enabled */
    {
      /* Return the maximal signal length */
      lSignalLength = (Dcm_CfgNetBufferSizeOptType)pSignalOpClassInfo->MaxLength;
    }
  }
  else
  {
    lSignalLength = (Dcm_CfgNetBufferSizeOptType)Dcm_UtiMaxValueOfUintType(Dcm_CfgNetBufferSizeMemType);
  }

  return lSignalLength;
}
# endif

/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignalRange()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignalRange(
  Dcm_OpStatusType opStatus,
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;
  Dcm_DidMgrDidLengthType            lResLength = 0u; /* Set just in case not set by application */

  DCM_IGNORE_UNREF_PARAM(pDidOpTypeContext);                                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_UtiDataContextVerifySize(pDataContext, pSignalOpClassInfo->MaxLength, ErrorCode);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    /* Call the appropriate application callback for that DID range signal */
    lStdResult = ((Dcm_DidMgrOpFuncReadDidRangeType)pSignalOpClassInfo->OpFunc)(pDidInfoContext->Did                                                 /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                              ,Dcm_UtiGetDataContextBuffer(pDataContext)
                                                                              ,opStatus
                                                                              ,&lResLength
                                                                              ,ErrorCode);                                                           /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */ /* SBSW_DCM_COMB_PTR_FORWARD */

    switch (lStdResult)
    {
    case DCM_E_OK:
      /* Update final response length only in positive/final case */
      if(Dcm_DebugDetectError(lResLength > pDidInfoContext->DidLength))                                                                              /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
      {
        lStdResult = DCM_E_NOT_OK;
        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        Dcm_DebugReportError(DCM_SID_SVCPORT_DATASVCS_OP_READDIDRANGE, DCM_E_INTERFACE_BUFFER_OVERFLOW);
      }
      else
      {
        pDidInfoContext->DidLength = lResLength;/* update current DID length */                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */

        Dcm_UtiCommitData(pDataContext, lResLength);                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
      }
      break;
    case DCM_E_PENDING:
    case DCM_E_NOT_OK:
      /* nothing to do */
      break;
    default:
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      break;
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6060 */ /* MD_MSR_STPAR */

# if (DCM_DIDMGR_OPTYPE_READ_ENABLED == STD_ON)                                                                                                      /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignalAsync()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignalAsync(
  Dcm_OpStatusType opStatus,
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;

  lStdResult = Dcm_UtiDataContextVerifySize(pDataContext, pSignalOpClassInfo->MaxLength, ErrorCode);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    Dcm_CfgNetBufferSizeOptType lSignalLen;

    /* Call the appropriate application callback for that asynchronous signal */
    lStdResult = ((Dcm_DidMgrOpFuncReadAsyncType)pSignalOpClassInfo->OpFunc)(opStatus                                                                /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                            ,Dcm_UtiGetDataContextBuffer(pDataContext));                             /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */ /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */

    switch (lStdResult)
    {
    case DCM_E_OK:
      /* Update the remaining available length */
      lSignalLen = Dcm_DidMgrGetSignalLength(pDidInfoContext, pDidOpTypeContext);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiCommitData(pDataContext, (Dcm_MsgLenType)lSignalLen);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;
    case DCM_E_NOT_OK:
      /* Handle NRC here since no NRC can be passed by the application on this interface */
      *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    case DCM_E_PENDING:
      /*  do nothing */
      break;
    default:
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      break;
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6060 */ /* MD_MSR_STPAR */
# endif

# if (DCM_DIDMGR_OPTYPE_READ_ENABLED == STD_ON)                                                                                                      /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignalSync()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignalSync(
  Dcm_OpStatusType opStatus,
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_UtiDataContextVerifySize(pDataContext, pSignalOpClassInfo->MaxLength, ErrorCode);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    /* Call the appropriate application callback for that synchronous signal */
    lStdResult = ((Dcm_DidMgrOpFuncReadSyncType)pSignalOpClassInfo->OpFunc)(Dcm_UtiGetDataContextBuffer(pDataContext));                              /* PRQA S 0313 */ /* MD_Dcm_0313 */ /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */ /* SBSW_DCM_POINTER_WRITE_DATA_CONTEXT */

    if(lStdResult == DCM_E_OK)
    {
      Dcm_CfgNetBufferSizeOptType lSignalLen;

      /* Update the remaining available length */
      lSignalLen = Dcm_DidMgrGetSignalLength(pDidInfoContext, pDidOpTypeContext);                                                                    /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_UtiCommitData(pDataContext, (Dcm_MsgLenType)lSignalLen);                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
       Dcm_DebugAssert(lStdResult == DCM_E_NOT_OK, DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);

      /* Handle NRC here since no NRC can be passed by the application through this interface */
      *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6060 */ /* MD_MSR_STPAR */
# endif

# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignalPaged()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignalPaged(
  Dcm_OpStatusType opStatus,
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;
  Dcm_DidMgrDidLengthType            lTmpAvailLen;
#  if (DCM_DIDMGR_RTE_IMPLICIT_COM_ENABLED == STD_ON)                                                                                                /* COV_DCM_REQUIRED_BY_SAFEBSW TX */
  boolean                            lLoopPossible;
#  endif

  DCM_IGNORE_UNREF_PARAM(pDidInfoContext);                                                                                                           /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pDidOpTypeContext);                                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

#  if (DCM_DIDMGR_RTE_IMPLICIT_COM_ENABLED == STD_ON)                                                                                                /* COV_DCM_REQUIRED_BY_SAFEBSW TX */
  lStdResult = Dcm_UtiDataContextVerifySize(pDataContext, DCM_DIDMGR_READ_PAGE_SIZE, ErrorCode);                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
#  endif
  {
    lTmpAvailLen = (Dcm_DidMgrDidLengthType)pDataContext->AvailLen;

#  if (DCM_DIDMGR_RTE_IMPLICIT_COM_ENABLED == STD_ON)                                                                                                /* COV_DCM_REQUIRED_BY_SAFEBSW TX */
    if (lTmpAvailLen > DCM_DIDMGR_READ_PAGE_SIZE)
    {
      lTmpAvailLen = DCM_DIDMGR_READ_PAGE_SIZE;
      lLoopPossible = TRUE;
    }
    else
    {
      lLoopPossible = FALSE;
    }
#  endif

    /* Call the appropriate application callback for that paged signal */
    lStdResult = ((Dcm_DidMgrOpFuncReadPagedDidType)pSignalOpClassInfo->OpFunc)(opStatus                                                             /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                             ,Dcm_UtiGetDataContextBuffer(pDataContext)
                                                                             ,&lTmpAvailLen);                                                        /* SBSW_DCM_COMB_PTR_FORWARD */ /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */

    switch (lStdResult)
    {
      case DCM_E_BUFFERTOOLOW:
#  if (DCM_DIDMGR_RTE_IMPLICIT_COM_ENABLED == STD_ON)                                                                                                /* COV_DCM_REQUIRED_BY_SAFEBSW TX */
        if (lLoopPossible == TRUE)
        {
          lStdResult = DCM_E_LOOP;
        }
#  endif
        /* fall through */
      case DCM_E_OK:                                                                                                                                 /* PRQA S 2003 */ /* MD_Dcm_Optimize_2003 */
        Dcm_UtiCommitData(pDataContext, lTmpAvailLen);                                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
        break;

      case DCM_E_NOT_OK:
        /* Handle NRC here since no NRC can be passed by the application on this interface */
        *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
        break;

      case DCM_E_PENDING:
        /* Nothing to do */
        break;

      default:
         Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
         *ErrorCode = DCM_E_PANIC_NRC;                                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
         lStdResult = DCM_E_NOT_OK;
        break;
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6060 */ /* MD_MSR_STPAR */
# endif

/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignalVid()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignalVid(
  Dcm_OpStatusType opStatus,
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;

  DCM_IGNORE_UNREF_PARAM(pDidOpTypeContext);                                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_UtiDataContextVerifySize(pDataContext, pSignalOpClassInfo->MaxLength, ErrorCode);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    uint8 lResLength = (uint8)pSignalOpClassInfo->MaxLength;

    /* Call the appropriate application callback for that VID signal */
    lStdResult = ((Dcm_DidMgrOpFuncReadVidType)pSignalOpClassInfo->OpFunc)(opStatus                                                                  /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                          ,Dcm_UtiGetDataContextBuffer(pDataContext)
# if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
                                                                          ,&lResLength
# endif
                                                                          );                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_COMB_PTR_FORWARD */ /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */

    /* Handle NRC here since no NRC can be passed by the application on this interface */
    if(lStdResult == DCM_E_OK)
    {
      /* If VID signal with dynamic length supported (Autosar Version >= 4.2.2) */
# if (DCM_DCM_AR_VERSION >= DCM_DCM_AR_VERSION_422)
      /* Update final response length only in positive/final case */
      if(Dcm_DebugDetectError( (lResLength == 0u) /* At least the NODI byte has to be written */                                                     /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
                             ||(lResLength > pSignalOpClassInfo->MaxLength) ))
      {
        lStdResult = DCM_E_NOT_OK;
        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        Dcm_DebugReportError(DCM_SID_SVCPORT_DATASVCS_OP_READVID, DCM_E_INTERFACE_BUFFER_OVERFLOW);
      }
      else
# endif
      {
        /* Decrement length of signal if NODI byte reporting disabled */
# if (DCM_DIDMGR_REPORT_NODIBYTE_ENABLED == STD_OFF)
        lResLength--;

        /* Remove NODI byte from VID signal if NODI byte reporting disabled */
        Dcm_UtiProvideDataAsUN(pDataContext, &Dcm_UtiGetDataContextBuffer(pDataContext)[1], lResLength);                                             /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_COMB_PTR_FORWARD */
# else
        Dcm_UtiCommitData(pDataContext, lResLength);                                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
# endif

        pDidInfoContext->DidLength = lResLength; /* Update current DID length */                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
      }
    }
    else if(lStdResult == DCM_E_PENDING)
    {
      /* Retry later */
    }
    else
    {
      Dcm_DebugAssert((lStdResult == DCM_E_NOT_OK), DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);

      *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6060 */ /* MD_MSR_STPAR */

# if (DCM_DIDMGR_OPTYPE_READ_ENABLED == STD_ON)                                                                                                      /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignalWrapperAsync()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignalWrapperAsync(
  Dcm_OpStatusType opStatus,
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;

  DCM_IGNORE_UNREF_PARAM(pDidOpTypeContext);                                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_UtiDataContextVerifySize(pDataContext, pSignalOpClassInfo->MaxLength, ErrorCode);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    /* Call the appropriate application callback for that asynchronous signal */
    lStdResult = ((Dcm_DidMgrOpFuncReadWrapperAsyncType)pSignalOpClassInfo->OpFunc)(pDidInfoContext->Did                                             /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                                    ,opStatus
                                                                                    ,pDataContext);                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */

    switch (lStdResult)
    {
    case DCM_E_OK:
    case DCM_E_PENDING:
#  if (DCM_SVC_2C_02_SUPPORT_ENABLED == STD_ON)
    case DCM_E_FORCE_RCRRP:
#  endif
      break;
    case DCM_E_NOT_OK:
      /* Handle NRC here since no NRC can be passed by the application on this interface */
      *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    default:
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      break;
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6060 */ /* MD_MSR_STPAR */
# endif

# if (DCM_DIDMGR_OPTYPE_READ_ENABLED == STD_ON)                                                                                                      /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignalWrapperSync()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignalWrapperSync(
  Dcm_OpStatusType opStatus,
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pDidInfoContext);                                                                                                           /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pDidOpTypeContext);                                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_UtiDataContextVerifySize(pDataContext, pSignalOpClassInfo->MaxLength, ErrorCode);                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    /* Call the appropriate application callback for that asynchronous signal */
    lStdResult = ((Dcm_DidMgrOpFuncReadWrapperSyncType)pSignalOpClassInfo->OpFunc)(pDataContext);                                                    /* PRQA S 0313 */ /* MD_Dcm_0313 */ /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_CALL_FUNCPTR_DIDMGROPFUNC */

    switch (lStdResult)
    {
    case DCM_E_OK:
      break;
    case DCM_E_NOT_OK:
      /* Handle NRC here since no NRC can be passed by the application on this interface */
      *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      break;
    default:
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      break;
    }
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6060 */ /* MD_MSR_STPAR */
# endif

# if (DCM_DIDMGR_OPTYPE_READ_ENABLED == STD_ON)                                                                                                      /* COV_DCM_SUPPORT_ALWAYS TX */
/**********************************************************************************************************************
 *  Dcm_DidMgrReadSignal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadSignal(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType                     lStdResult;
  Dcm_CfgDidMgrSignalOpClassInfoPtrType pSignalOpClassInfo;

  lStdResult = Dcm_CfgWrapDidMgrSignalOpClassInfoGetEntry(pDidOpTypeContext->Base.SignalOpClassInfoRef
                                                         ,&pSignalOpClassInfo
                                                         ,ErrorCode);                                                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    switch(pSignalOpClassInfo->FuncClass)
    {
    case DCM_DIDMGR_OPCLS_READ_RANGE:
      lStdResult = Dcm_DidMgrReadSignalRange(opStatus, pSignalOpClassInfo, pDataContext, pDidInfoContext, pDidOpTypeContext, ErrorCode);             /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;

    case DCM_DIDMGR_OPCLS_READ_ASYNC:
      lStdResult = Dcm_DidMgrReadSignalAsync(opStatus, pSignalOpClassInfo, pDataContext, pDidInfoContext, pDidOpTypeContext, ErrorCode);             /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;

    case DCM_DIDMGR_OPCLS_READ_SYNC:
      lStdResult = Dcm_DidMgrReadSignalSync(opStatus, pSignalOpClassInfo, pDataContext, pDidInfoContext, pDidOpTypeContext, ErrorCode);              /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;

#  if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
    case DCM_DIDMGR_OPCLS_READ_PAGED:
      lStdResult = Dcm_DidMgrReadSignalPaged(opStatus, pSignalOpClassInfo, pDataContext, pDidInfoContext, pDidOpTypeContext, ErrorCode);             /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;
#  endif

    case DCM_DIDMGR_OPCLS_READ_VID:
      lStdResult = Dcm_DidMgrReadSignalVid(opStatus, pSignalOpClassInfo, pDataContext, pDidInfoContext, pDidOpTypeContext, ErrorCode);               /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;

    case DCM_DIDMGR_OPCLS_READ_WRAPPER_ASYNC:
      lStdResult = Dcm_DidMgrReadSignalWrapperAsync(opStatus, pSignalOpClassInfo, pDataContext, pDidInfoContext, pDidOpTypeContext, ErrorCode);      /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;

    case DCM_DIDMGR_OPCLS_READ_WRAPPER_SYNC:
      lStdResult = Dcm_DidMgrReadSignalWrapperSync(opStatus, pSignalOpClassInfo, pDataContext, pDidInfoContext, pDidOpTypeContext, ErrorCode);       /* SBSW_DCM_PARAM_PTR_FORWARD */
      break;

    default:
      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INVALID_CONFIG);
      break;
    }

    if(opStatus == DCM_CANCEL)
    {
      *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }

    Dcm_UtiHandleApplNrc(lStdResult, ErrorCode, DCM_E_CONDITIONSNOTCORRECT);                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6010, 6030, 6050 */ /* MD_MSR_STPTH,  MD_MSR_STCYC,  MD_MSR_STCAL */
# endif

# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrCheckSignalSize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(boolean, DCM_CODE) Dcm_DidMgrCheckSignalSize(
  Dcm_DiagDataContextConstPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext                                                                                                /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Dcm_CfgNetBufferSizeOptType lRequiredBufSize;

#  if (DCM_DIDMGR_RTE_IMPLICIT_COM_ENABLED == STD_ON)                                                                                                /* COV_DCM_REQUIRED_BY_SAFEBSW TX */
  lRequiredBufSize = Dcm_DidMgrGetRequiredSignalBufSize(pDidInfoContext, pDidOpTypeContext);                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
#  else
  /* Calculate the remaining available length */
  lRequiredBufSize = Dcm_DidMgrGetSignalLength(pDidInfoContext, pDidOpTypeContext);                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
#  endif

  /*
   * Note: An additional check against AvailLen is not necessary here, since AvailLen is always equal to Size.
   *       The only exception is the last page during paged buffering, because AvailLen is than reduced to the
   *       remaining data length. As such it is already guaranteed that the signal fits into AvailLen.
   */
  return Dcm_UtiDataContextCheckSize(pDataContext, (Dcm_MsgLenType)lRequiredBufSize);                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif

/**********************************************************************************************************************
 *  Dcm_DidMgrReadMultiSignal()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadMultiSignal(                                                                           /* PRQA S 2889 */ /* MD_Dcm_Optimize_2889 */
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lResult;

  /* For each signal of the DID: */
  do
  {
# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
    if (Dcm_DidMgrCheckSignalSize(pDataContext, pDidInfoContext, pDidOpTypeContext) == FALSE)                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
    {
      /* Try it the next task cycle */
      lResult = DCM_E_BUFFERTOOLOW;
    }
    else
# endif
    {
      Dcm_CfgNetBufferSizeOptType lAvailLen;

      lAvailLen = (Dcm_CfgNetBufferSizeOptType)(pDataContext->AvailLen);

      /* Read the signal data considering gaps */
      lResult = Dcm_DidMgrReadSignal(pDidOpTypeContext->OpType.Read.OpStatus
                                    ,pDataContext
                                    ,pDidInfoContext
                                    ,pDidOpTypeContext
                                    ,ErrorCode);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

      switch(lResult)
      {
        case DCM_E_OK:
          pDidOpTypeContext->Base.NumSignals--;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
          if(pDidOpTypeContext->Base.NumSignals == 0u)
          {
            return DCM_E_OK;                                                                                                                         /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
          }

          pDidOpTypeContext->OpType.Read.Progress += (Dcm_CfgDidMgrOptimizedDidLengthType)(lAvailLen - pDataContext->AvailLen);                      /* SBSW_DCM_PARAM_PTR_WRITE */

          /* Go on with next signal */
          pDidOpTypeContext->Base.SignalOpClassInfoRef++;                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
          pDidOpTypeContext->OpType.Read.OpStatus = DCM_INITIAL; /* reset op status */                                                               /* SBSW_DCM_PARAM_PTR_WRITE */
          break;
# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON)
        case DCM_E_BUFFERTOOLOW:
#  if (DCM_DIDMGR_RTE_IMPLICIT_COM_ENABLED == STD_ON)                                                                                                /* COV_DCM_REQUIRED_BY_SAFEBSW TX */
        case DCM_E_LOOP:
#  endif
          pDidOpTypeContext->OpType.Read.Progress += (Dcm_CfgDidMgrOptimizedDidLengthType)(lAvailLen - pDataContext->AvailLen);                      /* SBSW_DCM_PARAM_PTR_WRITE */
          /* fall through */
# endif
        case DCM_E_PENDING:                                                                                                                          /* PRQA S 2003 */ /* MD_Dcm_Optimize_2003 */
          pDidOpTypeContext->OpType.Read.OpStatus = DCM_PENDING;                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
          break;
        default: /* DCM_E_FORCE_RCRRP, DCM_E_NOT_OK */
          break;
      }
    }
  } while((lResult == DCM_E_OK)
# if (DCM_DIDMGR_PAGED_SUPPORT_ENABLED == STD_ON) && (DCM_DIDMGR_RTE_IMPLICIT_COM_ENABLED == STD_ON)
         ||(lResult == DCM_E_LOOP)
# endif
         );
  return lResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */

# if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrRestoreDidInfo()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_DidMgrRestoreDidInfo(
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrOpOptType didOp
  )
{
  Std_ReturnType lStdReturn;

  lStdReturn = Dcm_DidMgrDidRangeLookUp(DCM_INITIAL, pDidInfoContext, pDidOpTypeContext, didOp, FALSE);                                              /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdReturn == DCM_E_NOT_OK)
  {
    lStdReturn = Dcm_DidMgrConcreteDidLookUp(pDidInfoContext, didOp);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  Dcm_DebugAssert(lStdReturn == DCM_E_OK, DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
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
 *  Dcm_DidMgrInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DidMgrInit(
  void
  )
{
# if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
  /* Set default ReturnControlToEcu values to 0xFF...FF */
  Dcm_UtiMemSetUintX(Dcm_DidMgrIoControlCemrSessionMask                                                                                              /* SBSW_DCM_POINTER_WRITE_IO_CTRL_CEMR_SESSION_MASK */
                    ,Dcm_MsgItemType
                    ,0xFFu
                    ,DCM_DIDMGR_IODID_MAX_EXT_CEMR_SIZE); /* DCM_DIDMGR_IODID_MAX_EXT_CEMR_SIZE will become 1 if IoControlCemrSessionMask not used */
# endif

# if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
  /* Initialize the DataContext of the Secure Coding context to the respective buffer. Once and for all. */
  Dcm_UtiInitDataContext(&Dcm_DidMgrSingletonContext.SecureCoding.DataContext
                        ,Dcm_DidMgrSingletonContext.SecureCoding.Buffer
                        ,DCM_DIDMGR_SECURE_CODING_BUFFER_SIZE);                                                                                      /* SBSW_DCM_GLOBAL_PTR_WRITE */

  Dcm_DidMgrSecureCodingReset();
# endif
}

/**********************************************************************************************************************
 *  Dcm_DidMgrInitOpInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DidMgrInitOpInfo(
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrOpOptType didOp
  )
{
  Dcm_DidMgrOpOptType lDidOp = didOp;

  pDidInfoContext->Descriptor.DidOpInfoIdx = Dcm_CfgDidMgrGetDidInfo(pDidInfoContext)->OpBaseIdx;                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
  /* 0x01 ->0x00 -> already found
  * 0x02 ->0x01 -> iterate once
  * .Etc */
  --lDidOp; /* there is always at least one operation configured */

  /* clear all flags above this operation index (not relevant) */
  lDidOp &= Dcm_CfgDidMgrGetDidInfo(pDidInfoContext)->Operations;

  while (lDidOp != 0u)
  {
    pDidInfoContext->Descriptor.DidOpInfoIdx++;                                                                                                      /* SBSW_DCM_PARAM_PTR_WRITE */
    lDidOp &= (Dcm_DidMgrOpOptType)(lDidOp - 1u); /* clear the least significant bit set */
  }
}

/**********************************************************************************************************************
 *  Dcm_DidMgrInitOpTypeHandler()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DidMgrInitOpTypeHandler(
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext
  )
{
  /* Number of operations to be iterated (number signals) */
  pDidOpTypeContext->Base.NumSignals = (Dcm_CfgDidMgrSignalIterMemType)
                                       ( Dcm_CfgWrapDidMgrDidOpClassInfoGetEntry((Dcm_CfgDidMgrDidOpClassHandleOptType)(pDidInfoContext->Descriptor.DidOpClassInfoIdx) + 1u)->OpClassRef
                                       - Dcm_CfgWrapDidMgrDidOpClassInfoGetEntry((Dcm_CfgDidMgrDidOpClassHandleOptType)(pDidInfoContext->Descriptor.DidOpClassInfoIdx) + 0u)->OpClassRef); /* SBSW_DCM_PARAM_PTR_WRITE */ /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

  /* Start position of the operations to be called */
  pDidOpTypeContext->Base.SignalOpClassInfoRef = Dcm_CfgDidMgrGetDidOpClassInfo(pDidInfoContext)->OpClassRef;                                        /* SBSW_DCM_PARAM_PTR_WRITE */
}

/**********************************************************************************************************************
 *  Dcm_DidMgrGetDidInfo()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrGetDidInfo(
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrOpOptType didOp
  )
{
  Std_ReturnType lStdReturn;
  Dcm_OpStatusType lOpStatus = opStatus;

  DCM_IGNORE_UNREF_PARAM(lOpStatus);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pDidOpTypeContext);                                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdReturn = DCM_E_NOT_OK;

  /* On the very first call: */
  if (lOpStatus == DCM_INITIAL)
  {
    /* Determine the lookup engine to run first (DID range or concrete DID) */
    pDidOpTypeContext->OpType.LookUp.State = DCM_DIDMGR_LOOKUP_STATE_DIDRANGE;                                                                       /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  /* Otherwise: proceed with next steps */

  /* If current progress is to (still) lookup for a DID range: */
  if(pDidOpTypeContext->OpType.LookUp.State == DCM_DIDMGR_LOOKUP_STATE_DIDRANGE)
  {
    /* Delegate search to the DID range lookup engine */
    lStdReturn = Dcm_DidMgrDidRangeLookUp(lOpStatus, pDidInfoContext, pDidOpTypeContext, didOp, TRUE);                                               /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Evaluate result: */
    if(lOpStatus == DCM_CANCEL)
    {
      /* If the current call is a cancellation event: Terminate immediately any further processing */
      lStdReturn = DCM_E_NOT_OK;
    }
    else if(lStdReturn == DCM_E_NOT_OK)
    {
      /* Otherwise: If no match found: Continue with concrete DID look up */
      pDidOpTypeContext->OpType.LookUp.State = DCM_DIDMGR_LOOKUP_STATE_CONCRETEDID;                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    }
    else
# if (DCM_DIDMGR_OPTYPE_RANGE_ISAVAIL_ENABLED == STD_ON)
    if(lStdReturn == DCM_E_LOOKUP_MATCH_FOUND)
    {
      /* Otherwise: If range match found but it was a gap: Report DID as not supported */
      lStdReturn = DCM_E_NOT_OK;
    }
    else
# endif
# if (DCM_DIDMGR_EXTENDED_LOOKUP_ENABLED == STD_ON)
#  if (DCM_DIDMGR_OPTYPE_RANGE_ISAVAIL_ENABLED == STD_ON)
    if(lStdReturn == DCM_E_OK)                                                                                                                       /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
#  endif
    {
      lOpStatus = DCM_INITIAL; /* Prepare for new asynchronous calling sequence */
      /* Otherwise: If match found: Go on with optional DID look up result filtering */
      pDidOpTypeContext->OpType.LookUp.State = DCM_DIDMGR_LOOKUP_STATE_DIDFILTER_EXTERN;                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
    }
#  if (DCM_DIDMGR_OPTYPE_RANGE_ISAVAIL_ENABLED == STD_ON)
    else
#  endif
# endif
    {
      /*
       * Otherwise: Just return the final result: (DCM_E_PENDING | DCM_E_OK (if no extended look up) |
       * DCM_E_NOT_OK (if no gaps) )
       */
    }
  }
  /* Otherwise - keep current result: DCM_E_NOT_OK */

  /* If current progress is to lookup for a concrete DID: */
  if(pDidOpTypeContext->OpType.LookUp.State == DCM_DIDMGR_LOOKUP_STATE_CONCRETEDID)
  {
    /* Start concrete DID lookup engine */
    lStdReturn = Dcm_DidMgrConcreteDidLookUp(pDidInfoContext, didOp);                                                                                /* SBSW_DCM_PARAM_PTR_FORWARD */

    /* Evaluate result: */
# if (DCM_DIDMGR_EXTENDED_LOOKUP_ENABLED == STD_ON)
    if(lStdReturn == DCM_E_OK)
    {
      /* If valid DID found: proceed with optional lookup result filtering */
      lOpStatus = DCM_INITIAL; /* Prepare for new asynchronous calling sequence */
      pDidOpTypeContext->OpType.LookUp.State = DCM_DIDMGR_LOOKUP_STATE_DIDFILTER_EXTERN;                                                             /* SBSW_DCM_PARAM_PTR_WRITE */
    }
# endif
  }
  /* Otherwise - keep current result: DCM_E_OK, DCM_E_NOT_OK, DCM_E_PENDING */

# if (DCM_DIDMGR_EXTENDED_LOOKUP_ENABLED == STD_ON)
  /* If any valid DID(range) found: */
  if(pDidOpTypeContext->OpType.LookUp.State == DCM_DIDMGR_LOOKUP_STATE_DIDFILTER_EXTERN)
  {
    /* Proceed with optional lookup result filtering */
    lStdReturn = Dcm_FilterDidLookUpResult(lOpStatus, pDidInfoContext->Did, (Dcm_DidOpType)didOp);

    /* Evaluate result: */
    if(lOpStatus == DCM_CANCEL)
    {
      /* If the current call is a cancellation event: Terminate immediately any further processing */
      lStdReturn = DCM_E_NOT_OK;
    }

    /* If an valid/unexpected return value: */
    if(Dcm_DebugDetectError( (lStdReturn != DCM_E_OK)
                           &&(lStdReturn != DCM_E_PENDING)
                           &&(lStdReturn != DCM_E_NOT_OK) ))
    {
      /* Unexpected return value is treated as "DID not supported" */
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
      lStdReturn = DCM_E_NOT_OK;
    }
  }
  /* Otherwise - keep current result: DCM_E_OK, DCM_E_NOT_OK, DCM_E_PENDING */
# endif

  if(lStdReturn == DCM_E_OK)
  {
    /* Always perform standard filtering (variant, session, etc.) */
    lStdReturn = Dcm_DidMgrFilterDidLookUp(pDidInfoContext, didOp);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */

  }

  return lStdReturn;
}                                                                                                                                                    /* PRQA S 6010, 6030, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_DidMgrInitOpClassInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DidMgrInitOpClassInfo(
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrOpCallOptType opType
  )
{
  Dcm_DidMgrOpCallOptType lOpType = opType;
  pDidInfoContext->Descriptor.DidOpClassInfoIdx = Dcm_CfgDidMgrGetDidOpInfo(pDidInfoContext)->OpTypeBaseIdx;                                         /* SBSW_DCM_PARAM_PTR_WRITE */

  /* 0x01 ->0x00 -> already found
   * 0x02 ->0x01 -> iterate once
   * .Etc */
  --lOpType; /* there is always at least one operation configured */

  /* clear all flags above this operation index (not relevant) */
  lOpType &= Dcm_CfgDidMgrGetDidOpInfo(pDidInfoContext)->CallTypes;

  while (lOpType != 0u)
  {
    pDidInfoContext->Descriptor.DidOpClassInfoIdx++;                                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
    lOpType &= (Dcm_DidMgrOpOptType)(lOpType - 1u); /* clear the least significant bit set */
  }
}

/**********************************************************************************************************************
 *  Dcm_DidMgrReadCheckCond()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadCheckCond(
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType   lStdResult = DCM_E_NOT_OK;                                                                                                        /* PRQA S 2981 */ /* MD_Dcm_RetVal */
  Dcm_OpStatusType lOpStatus = opStatus;

  if (lOpStatus == DCM_INITIAL)
  {
    /* Initialize DID handler */
    Dcm_DidMgrInitOpTypeHandler(pDidInfoContext, pDidOpTypeContext);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  DCM_UTI_LOOP_FOR_EVER
  {
    lStdResult = Dcm_DidMgrReadCheckCondSignal(lOpStatus
                                              ,pDidInfoContext
                                              ,pDidOpTypeContext
                                              ,ErrorCode);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    if(lStdResult == DCM_E_OK)
    {
      pDidOpTypeContext->Base.NumSignals--;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
    }

    if ((lStdResult != DCM_E_OK) || (pDidOpTypeContext->Base.NumSignals == 0u))
    {
      break;                                                                                                                                         /* PRQA S 0771 */ /* MD_Dcm_Optimize_0771 */
    }

    /* next signal */
    pDidOpTypeContext->Base.SignalOpClassInfoRef++;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    lOpStatus = DCM_INITIAL;/* reset op status */
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6060 */ /* MD_MSR_STPAR */

/**********************************************************************************************************************
 *  Dcm_DidMgrReadDid()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadDid(
  Dcm_OpStatusType opStatus,
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lResult;

  if(opStatus == DCM_CANCEL)
  {
    lResult = DCM_E_NOT_OK;
    /* If DID signal already in processing: */
    if(pDidOpTypeContext->OpType.Read.OpStatus != DCM_INITIAL)
    {
      /* Notify application about cancellation */
      (void)Dcm_DidMgrReadSignal(opStatus
                                ,pDataContext
                                ,pDidInfoContext
                                ,pDidOpTypeContext
                                ,ErrorCode);                                                                                                         /* SBSW_DCM_PARAM_PTR_FORWARD */
    } /* Otherwise no cancellation shall be notified */
  }
  else
  {
    if(opStatus == DCM_INITIAL)
    {
      /* Otherwise, initialize DID handler */
      Dcm_DidMgrInitOpTypeHandler(pDidInfoContext, pDidOpTypeContext);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */

      pDidOpTypeContext->OpType.Read.OpStatus = DCM_INITIAL;                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
      pDidOpTypeContext->OpType.Read.Progress = 0u;                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    }

    /* Read all signals */
    lResult = Dcm_DidMgrReadMultiSignal(pDataContext, pDidInfoContext, pDidOpTypeContext, ErrorCode);                                                /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_DidMgrReadLength()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrReadLength(
  Dcm_OpStatusType opStatus,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType          lStdResult;
  Dcm_DidMgrDidLengthType lSignalLength;

  if(opStatus == DCM_INITIAL)
  {
    /* Initialize DID handler */
    Dcm_DidMgrInitOpTypeHandler(pDidInfoContext, pDidOpTypeContext);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  lStdResult = Dcm_DidMgrReadLengthSignal(opStatus
                                         ,pDidInfoContext
                                         ,pDidOpTypeContext
                                         ,&lSignalLength
                                         ,ErrorCode);                                                                                                /* SBSW_DCM_COMB_PTR_FORWARD */

  if(lStdResult == DCM_E_OK)
  {
    /* Set DID length to concrete DID length */
    pDidInfoContext->DidLength = (Dcm_DidMgrDidLengthType)(Dcm_CfgDidMgrGetDidInfo(pDidInfoContext)->MinLength + lSignalLength);                     /* SBSW_DCM_PARAM_PTR_WRITE */

    /* If no additional buffer space has to be reserved for that DID: */
    if(Dcm_CfgDidMgrGetDidInfo(pDidInfoContext)->ReservedLength == 0u)
    {
      /* Set required buffer space to concrete DID length */
      pDidInfoContext->ReservedLength = pDidInfoContext->DidLength;                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    }
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_DidMgrConcreteDidLookUp()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrConcreteDidLookUp(
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrOpOptType didOp
  )
{
  Std_ReturnType lStdReturn = DCM_E_NOT_OK;
  /* Search for the DID in the database */
  sint16_least  lResult = Dcm_UtiLookUpUint16(Dcm_CfgDidMgrDidLookUpTable, pDidInfoContext->Did);                                                    /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

  /* Check DID lookup result: */
  if(lResult >= 0)
  {
    /* If found, check if the DID supports the required operation: */
    if(Dcm_DidMgrIsOpSupported(Dcm_CfgWrapDidMgrDidInfo(lResult), didOp))
    {
      /* If operation supported, initialize DID information (set DID as supported) */
      pDidInfoContext->Descriptor.DidInfoIdx = (Dcm_CfgDidMgrDidInfoRefType)lResult;                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
      /* Set DID length. Later the concrete length will be stored here. Take the maximum/concrete length! */
      pDidInfoContext->DidLength      = (Dcm_DidMgrDidLengthType)Dcm_CfgWrapDidMgrDidInfo(lResult)->MaxLength;                                       /* SBSW_DCM_PARAM_PTR_WRITE */
      pDidInfoContext->ReservedLength = (Dcm_DidMgrDidLengthType)Dcm_CfgWrapDidMgrDidInfo(lResult)->ReservedLength;                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      /* Initialize DID operation information  */
      Dcm_DidMgrInitOpInfo(pDidInfoContext, didOp);                                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
      lStdReturn = DCM_E_OK;
    }/* else - not a supported DID in the operation context! */
  }/* else - not a supported DID at all! */

  return lStdReturn;
}

# if (DCM_DIDMGR_OPTYPE_IO_RETCTRL2ECU_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrIoControlRtrnCtrl2Ecu()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DidMgrIoControlRtrnCtrl2Ecu(
  Dcm_CfgDidMgrDidOpClassHandleOptType opInfoRef
  )
{
  Dcm_CfgDidMgrSignalOpClassRefOptType lSignalIter;

  for(lSignalIter = Dcm_CfgWrapDidMgrDidOpClassInfoGetEntry(opInfoRef + 0u)->OpClassRef;                                                             /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
      lSignalIter < Dcm_CfgWrapDidMgrDidOpClassInfoGetEntry(opInfoRef + 1u)->OpClassRef;
      ++lSignalIter)
  {
    Dcm_DidMgrIoControlRtrnCtrl2EcuSignal(lSignalIter);
  }
}
# endif

# if (DCM_SVC_2F_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrIoControl()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrIoControl(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_NOT_OK;
  Dcm_OpStatusType lOpStatus = opStatus;

  if (lOpStatus == DCM_INITIAL)
  {
    /* Initialize DID handler */
    Dcm_DidMgrInitOpTypeHandler(pDidInfoContext, pDidOpTypeContext);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  if(Dcm_DidMgrGetCtrlEnblMaskHandling(Dcm_CfgDidMgrGetDidInfo(pDidInfoContext)) == DCM_DIDMGR_IO_CEMR_HANDLING_INTERNAL)
  {
    lStdResult = Dcm_DidMgrIoControlWithMask(lOpStatus, pMsgContext, pDidInfoContext, pDidOpTypeContext, ErrorCode);                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
  else
  {
    while(pDidOpTypeContext->Base.NumSignals != 0u)
    {
      lStdResult = Dcm_DidMgrIoControlSignal(lOpStatus
                                            ,pMsgContext
                                            ,pDidInfoContext
                                            ,pDidOpTypeContext
                                            ,ErrorCode);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
      if(lStdResult != DCM_E_OK)
      {
        /* on pending or not OK -> just leave */
        break;
      }

      pDidOpTypeContext->Base.NumSignals--;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */
      if(pDidOpTypeContext->Base.NumSignals != 0u)
      {
        /* Go on with next signal */
        pDidOpTypeContext->Base.SignalOpClassInfoRef++;                                                                                              /* SBSW_DCM_PARAM_PTR_WRITE */
      }
      lOpStatus = DCM_INITIAL; /* reset op status */
    }
  }

  return lStdResult;
}
# endif

# if (DCM_DIDMGR_OPTYPE_WRITE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrWrite()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrWrite(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode                                                                                                          /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;
  Dcm_OpStatusType lOpStatus = opStatus;

  if (lOpStatus == DCM_INITIAL)
  {
    /* Initialize DID handler */
    Dcm_DidMgrInitOpTypeHandler(pDidInfoContext, pDidOpTypeContext);                                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  do
  {
    lStdResult = Dcm_DidMgrWriteSignal(lOpStatus
                                      ,pMsgContext
                                      ,pDidInfoContext
                                      ,pDidOpTypeContext
                                      ,ErrorCode);                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
    if(lStdResult == DCM_E_OK)
    {
      pDidOpTypeContext->Base.NumSignals--;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */

      if(pDidOpTypeContext->Base.NumSignals == 0u)
      {
        break;
      }

      /* Go on with next signal */
      pDidOpTypeContext->Base.SignalOpClassInfoRef++;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
      lOpStatus = DCM_INITIAL; /* reset op status */
    }
  }
  while(lStdResult == DCM_E_OK);

  return lStdResult;
}
# endif

# if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrWriteSecured()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrWriteSecured(
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_DidMgrDidInfoContextConstPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextConstPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  Dcm_MsgLenType lRequiredLength;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pDidOpTypeContext);                                                                                                         /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Calculate the required buffer size to store this DID in the local Secure Coding buffer (DID ID + data) */
  lRequiredLength = pMsgContext->reqDataLen + 2u;

  if ( (Dcm_DidMgrSingletonContext.SecureCoding.Started == FALSE)
     ||(Dcm_DidMgrSingletonContext.SecureCoding.Locked == TRUE)
     ||(Dcm_UtiDataContextCheckSize(&Dcm_DidMgrSingletonContext.SecureCoding.DataContext, lRequiredLength) == FALSE) )                               /* PRQA S 3415 */ /* MD_Dcm_Rule13.5 */ /* SBSW_DCM_GLOBAL_PTR_WRITE */
  {
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
    /* Copy the DID and the data to the local Secure Coding buffer. */
    Dcm_UtiProvideDataAsU16(&Dcm_DidMgrSingletonContext.SecureCoding.DataContext, pDidInfoContext->Did);                                             /* SBSW_DCM_GLOBAL_PTR_WRITE */

    Dcm_UtiProvideDataAsUN(&Dcm_DidMgrSingletonContext.SecureCoding.DataContext
                          ,Dcm_UtiGetReqData(pMsgContext)                                                                                            /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */
                          ,pMsgContext->reqDataLen);                                                                                                 /* SBSW_DCM_GLOBAL_PTR_WRITE */

    ++Dcm_DidMgrSingletonContext.SecureCoding.NumDids;

    lStdResult = DCM_E_OK;
  }

  return lStdResult;
}
# endif

# if (DCM_DIDMGR_OPTYPE_GETSCALINGINFO_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrGetScalingInfo()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrGetScalingInfo(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,                                                                                               /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;
  Dcm_OpStatusType lOpStatus = opStatus;

  if (lOpStatus == DCM_INITIAL)
  {
    /* Initialize DID handler */
    Dcm_DidMgrInitOpTypeHandler(pDidInfoContext, pDidOpTypeContext);                                                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
  }

  do
  {
    lStdResult = Dcm_DidMgrGetScalingInfoSignal(lOpStatus
                                               ,pMsgContext
                                               ,pDidInfoContext
                                               ,pDidOpTypeContext
                                               ,ErrorCode);                                                                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    if(lStdResult == DCM_E_OK)
    {
      pDidOpTypeContext->Base.NumSignals--;                                                                                                          /* SBSW_DCM_PARAM_PTR_WRITE */

      if(pDidOpTypeContext->Base.NumSignals == 0u)
      {
        break;
      }

      /* Go on with next signal */
      pDidOpTypeContext->Base.SignalOpClassInfoRef++;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
      lOpStatus = DCM_INITIAL; /* reset op status */
    }
  }
  while(lStdResult == DCM_E_OK);

  return lStdResult;
}
# endif

# if (DCM_AUTHMGR_WHITELIST_SERVICES_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrGetSidFromOp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(uint8, DCM_CODE) Dcm_DidMgrGetSidFromOp(
  Dcm_DidOpType didOp
  )
{
  uint8 lSid;

  switch(didOp)
  {
    case DCM_DID_OP_READ:
      lSid = DCM_DIDMGR_SVC_READ;
      break;
    case DCM_DID_OP_WRITE:
      lSid = DCM_DIDMGR_SVC_WRITE;
      break;
    case DCM_DID_OP_IO:
      lSid = DCM_DIDMGR_SVC_IOCTRL;
      break;
    case DCM_DID_OP_SCALINGINFO:
      lSid = DCM_DIDMGR_SVC_SCALING;
      break;
    case DCM_DID_OP_DEFINE:
      lSid = DCM_DIDMGR_SVC_DEFINE;
      break;
    default:
      lSid = DCM_DIDMGR_SVC_NONE;
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
      break;
  }

  return lSid;
}
# endif

/**********************************************************************************************************************
 *  Dcm_DidMgrDynDidStateCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrDynDidStateCheck(
  Dcm_NetConnRefMemType connHdl,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  lStdResult = Dcm_DidMgrStaticDidStateCheck(connHdl                                                                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
                                            ,pDidInfoContext->Did
                                            ,DCM_DIDMGR_OP_READ
                                            ,Dcm_CfgDidMgrGetDidOpInfo(pDidInfoContext)->ExecCondRef
                                            ,ErrorCode);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */

# if (DCM_DIDMGR_DYNDID_SRCITEM_CHECK_STATE_ENABLED == STD_ON) && \
     (DCM_DIDMGR_DYNDID_CLR_ON_STATE_CHG_ENABLED    == STD_OFF)
  if (lStdResult == DCM_E_OK)
  {
    if(Dcm_DidMgrIsOpSupported(Dcm_CfgDidMgrGetDidInfo(pDidInfoContext)
                              ,DCM_DIDMGR_OP_DEFINE)) /* for DynDID check source states */
    {
      lStdResult = Dcm_Svc2CStateCheckSrcItems(connHdl
                                              ,(Dcm_CfgDidMgrDynDidHandleMemType)(Dcm_CfgDidMgrGetDidInfo(pDidInfoContext)->OpRef)
                                              ,ErrorCode);                                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
  }/* else - ErrorCode already set */
# endif

  return lStdResult;
}

# if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrSecureCodingReset()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DidMgrSecureCodingReset(void)
{
  Dcm_DidMgrSingletonContext.SecureCoding.Started = FALSE;
  Dcm_DidMgrSingletonContext.SecureCoding.Locked  = FALSE;
  Dcm_DidMgrSingletonContext.SecureCoding.NumDids = 0u;

  /* Reset usage and available length of the Secure Coding data context */
  Dcm_UtiResetDataContext(&Dcm_DidMgrSingletonContext.SecureCoding.DataContext);                                                                     /* SBSW_DCM_GLOBAL_PTR_WRITE */
}
# endif

# if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrSecureCodingStart()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_DidMgrSecureCodingStart(
  void
  )
{
  Dcm_DidMgrSecureCodingReset();

  Dcm_DidMgrSingletonContext.SecureCoding.Started = TRUE;
}
# endif

# if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrSecureCodingWrite()
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
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrSecureCodingWrite(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DidMgrDidInfoContextPtrType pDidInfoContext,
  Dcm_DidMgrDidOpTypeContextPtrType pDidOpTypeContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  if (Dcm_RsrcMgrHasDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SECURE_CODING) == FALSE)
  {
    lStdResult = Dcm_RsrcMgrGetDidLock(opStatus, pDidInfoContext->Did, DCM_RSRCMGR_DIDLOCK_OWNER_SECURE_CODING);

    if(lStdResult == DCM_E_OK)
    {
      Dcm_DidMgrRestoreDidInfo(pDidInfoContext, pDidOpTypeContext, DCM_DIDMGR_OP_WRITE);                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
      Dcm_DidMgrInitOpClassInfo(pDidInfoContext, DCM_DIDMGR_OPTYPE_WRITE);                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */

      /* Simulate a request buffer containing only one DID */
      Dcm_UtiSetReqLength(pMsgContext, pDidInfoContext->DidLength);                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */

      lStdResult = DCM_E_LOOP;
    }
  }

  if (lStdResult == DCM_E_OK)
  {
    lStdResult = Dcm_DidMgrWrite(opStatus
                                ,pMsgContext
                                ,pDidInfoContext
                                ,pDidOpTypeContext
                                ,ErrorCode);                                                                                                         /* SBSW_DCM_COMB_PTR_FORWARD */

    if (lStdResult == DCM_E_PENDING)
    {
      /* Retry later */
    }
    else
    {
      if (lStdResult == DCM_E_OK)
      {
        --Dcm_DidMgrSingletonContext.SecureCoding.NumDids;

        if (Dcm_DidMgrSingletonContext.SecureCoding.NumDids != 0u)
        {
          /* Prepare for next DID. Consume only the DID ID since the data is already consumed by Dcm_DidMgrWrite() */
          Dcm_UtiConsumeReqDataAsU16(pMsgContext, &pDidInfoContext->Did);                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */

          lStdResult = DCM_E_LOOP;
        }
      }

      Dcm_RsrcMgrReleaseDidLock(DCM_RSRCMGR_DIDLOCK_OWNER_SECURE_CODING);
    }
  }

  return lStdResult;
}
# endif

# if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrSecureCodingIsBufferLocked()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(boolean, DCM_CODE) Dcm_DidMgrSecureCodingIsBufferLocked(
  void
  )
{
  return Dcm_DidMgrSingletonContext.SecureCoding.Locked;
}
# endif

# if (DCM_DIDMGR_SECURE_CODING_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrSecureCodingLockBuffer()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrSecureCodingLockBuffer(
  Dcm_DiagDataContextPtrType pDataContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  if ( (Dcm_DidMgrSingletonContext.SecureCoding.Started == FALSE)
     ||(Dcm_DidMgrSingletonContext.SecureCoding.DataContext.Usage == 0u)
     ||(Dcm_DidMgrSingletonContext.SecureCoding.Locked == TRUE) )
  {
    *ErrorCode = DCM_E_CONDITIONSNOTCORRECT;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
    lStdResult = DCM_E_NOT_OK;
  }
  else
  {
    Dcm_DidMgrSingletonContext.SecureCoding.Locked = TRUE;

    *pDataContext = Dcm_DidMgrSingletonContext.SecureCoding.DataContext;                                                                             /* SBSW_DCM_PARAM_PTR_WRITE */

    lStdResult = DCM_E_OK;
  }

  return lStdResult;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_DidMgrDidNoOp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrDidNoOp(
  Dcm_MsgType data                                                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(data);                                                                                                                      /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  return DCM_E_OK;
}

# if (DCM_DIDMGR_NVM_READ_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrReadNvMSignal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrNvMReadSignal(
  Dcm_OpStatusType opStatus,
  Dcm_MsgType data,
  uint16 blockId
  )
{
  return Dcm_UtiNvMRead(opStatus, data, blockId);                                                                                                    /* PRQA S 0315 */ /* MD_MSR_Dir1.1 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif

# if (DCM_DIDMGR_NVM_WRITE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrNvMWriteSignal()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrNvMWriteSignal(
  Dcm_ReadOnlyMsgType data,
  Dcm_OpStatusType opStatus,
  Dcm_NegativeResponseCodePtrType errorCode,
  uint16 blockId
  )
{
  Std_ReturnType lStdResult;

  lStdResult = Dcm_UtiNvMSwcDataWrite(opStatus, data, blockId);                                                                                      /* PRQA S 0315 */ /* MD_MSR_Dir1.1 */ /* SBSW_DCM_PARAM_PTR_FORWARD */

  if(lStdResult == DCM_E_NOT_OK)
  {
    *errorCode = DCM_E_GENERALPROGRAMMINGFAILURE;                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
  }
  return lStdResult;
}
# endif

# if (DCM_DIDMGR_SR_IO_CONTROL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrIoCtrlSRHandler_Control()
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
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrIoCtrlSRHandler_Control(
  Dcm_CfgDidMgrIoCtrlSRHandlersRefOptType srEntryIndex,
  uint8 ioOperation,
  Dcm_ReadOnlyMsgType reqData,
  Dcm_OpStatusType opStatus,
  uint32 cemr,
  Dcm_NegativeResponseCodePtrType errorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;
  Dcm_IOOperationResponseType ioResponse;
  Dcm_DidMgrIoCtrlSRHandlersConstPtrType pIoControlSrHandlers = Dcm_CfgWrapDidMgrIoControlSenderReceiverHandlersGetEntry(srEntryIndex);

  *errorCode = DCM_E_POSITIVERESPONSE;                                                                                                               /* SBSW_DCM_PARAM_PTR_WRITE */

  /* ignore parameter reqData if no IO Did supports ShortTermAdjustment */
  DCM_IGNORE_UNREF_PARAM(reqData);                                                                                                                   /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if (opStatus == DCM_INITIAL)
  {
    /* Clear IsUpdated flag */
    lStdResult = pIoControlSrHandlers->IoCtrlResRead(&ioResponse);                                                                                   /* SBSW_DCM_CALL_FUNCPTR_IOCTRLSRHANDLER */ /* SBSW_DCM_POINTER_FORWARD_STACK */

    if ( (ioOperation == 0x03u)
       &&(lStdResult == DCM_E_OK))
    {
      /* Transfer requested data */
      lStdResult = pIoControlSrHandlers->IoCtrlReqCtrlStateWrite(reqData);                                                                           /* SBSW_DCM_CALL_FUNCPTR_IOCTRLSRHANDLER */
    }

    if (lStdResult == DCM_E_OK)
    {
      /* Transfer new IO operation and new CEMR  */
      lStdResult = pIoControlSrHandlers->IoCtrlReqIoOpReqWrite(ioOperation, cemr);                                                                   /* SBSW_DCM_CALL_FUNCPTR_IOCTRLSRHANDLER */
    }
  }

  /* OpStatus == DCM_CANCEL has no negative effect -> leave it unhandled */

  /* Get acknowledge */
  if (lStdResult == DCM_E_OK)
  {
    if (pIoControlSrHandlers->IoCtrlResIsUpdated() == TRUE)                                                                                          /* SBSW_DCM_CALL_FUNCPTR_IOCTRLSRHANDLER */
    {
      lStdResult = pIoControlSrHandlers->IoCtrlResRead(&ioResponse);                                                                                 /* SBSW_DCM_CALL_FUNCPTR_IOCTRLSRHANDLER */ /* SBSW_DCM_POINTER_FORWARD_STACK */
      if (lStdResult == DCM_E_OK)
      {
        switch (ioResponse)
        {
        case DCM_POSITIVE_RESPONSE:
        {
          uint32 underControl;
          lStdResult = pIoControlSrHandlers->IoCtrlReqUnderControlRead(&underControl);                                                               /* SBSW_DCM_CALL_FUNCPTR_IOCTRLSRHANDLER */ /* SBSW_DCM_POINTER_FORWARD_STACK */
          if (lStdResult == DCM_E_OK)
          {
            lStdResult = pIoControlSrHandlers->IoCtrlReqUnderControlWrite(underControl | cemr);                                                      /* SBSW_DCM_CALL_FUNCPTR_IOCTRLSRHANDLER */
          }
          /* Set IO Operation to Idle (0xFF); return value is not evaulated due to final response of application */
          (void)pIoControlSrHandlers->IoCtrlReqIoOpReqWrite(0xFFu, cemr);                                                                            /* SBSW_DCM_CALL_FUNCPTR_IOCTRLSRHANDLER */
          break;
        }
        case DCM_RESPONSE_PENDING:
          lStdResult = DCM_E_PENDING;
          break;
        default: /* any NRC */
          /* Set IO Operation to Idle (0xFF); return value is not evaulated due to final response of application */
          (void)pIoControlSrHandlers->IoCtrlReqIoOpReqWrite(0xFFu, cemr);                                                                            /* SBSW_DCM_CALL_FUNCPTR_IOCTRLSRHANDLER */
          *errorCode = ioResponse;                                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
          lStdResult = DCM_E_NOT_OK;
          break;
        }
      }
    }
    else
    {
      lStdResult = DCM_E_PENDING;
    }
  }

  if ( (lStdResult == DCM_E_NOT_OK)
     &&(*errorCode == DCM_E_POSITIVERESPONSE))
  {
    *errorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
  }/* else:RTE error codes are handled in DCM core */

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6010, 6030, 6060, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STPAR, MD_MSR_STMIF */
# endif

# if (DCM_DIDMGR_SR_IO_CONTROL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgrIoCtrlSRHandler_Reset()
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
FUNC(Std_ReturnType, DCM_CODE) Dcm_DidMgrIoCtrlSRHandler_Reset(
  Dcm_CfgDidMgrIoCtrlSRHandlersRefOptType srEntryIndex,
  uint32 cemr,
  Dcm_NegativeResponseCodePtrType errorCode
  )
{
  Std_ReturnType lStdResult;
  uint32         underControl;

  Dcm_DidMgrIoCtrlSRHandlersConstPtrType pIoControlSrHandlers = Dcm_CfgWrapDidMgrIoControlSenderReceiverHandlersGetEntry(srEntryIndex);

  /* Update under control elements */
  lStdResult = pIoControlSrHandlers->IoCtrlReqUnderControlRead(&underControl);                                                                       /* SBSW_DCM_CALL_FUNCPTR_IOCTRLSRHANDLER */ /* SBSW_DCM_POINTER_FORWARD_STACK */

  if (lStdResult == DCM_E_OK)
  {
    lStdResult = pIoControlSrHandlers->IoCtrlReqUnderControlWrite(underControl & ~cemr);                                                             /* SBSW_DCM_CALL_FUNCPTR_IOCTRLSRHANDLER */
  }

  if (lStdResult == DCM_E_OK)
  {
    /* Transfer new IO operation and new CEMR  */
    lStdResult = pIoControlSrHandlers->IoCtrlReqIoOpReqWrite(0x00u, cemr);                                                                           /* SBSW_DCM_CALL_FUNCPTR_IOCTRLSRHANDLER */
  }

  if (lStdResult == DCM_E_NOT_OK)
  {
    *errorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
  }/* else:RTE error codes are handled in DCM core */

  return lStdResult;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CALLOUT_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_DIDMGR_DID_F186_READ_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DidMgr_F186_ReadData()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_DidMgr_F186_ReadData(
  P2VAR(uint8, AUTOMATIC, DCM_VAR_NOINIT) Data
  )
{
  return Dcm_GetSesCtrlType(Data);                                                                                                                   /* SBSW_DCM_PARAM_PTR_FORWARD */
}
# endif
# define DCM_STOP_SEC_CALLOUT_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_DIDMGR_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_DidMgr.c
 *********************************************************************************************************************/
