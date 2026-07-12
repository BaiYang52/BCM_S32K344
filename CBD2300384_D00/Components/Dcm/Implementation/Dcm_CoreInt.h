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
/**        \file  Dcm_CoreInt.h
 *         \unit  NoUnit
 *        \brief  Private data type definition and interface of DCM
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
#if !defined(DCM_COREINT_H)
# define DCM_COREINT_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_TskTypes.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Basic thread ID used for thread independent processes */
# define DCM_THREAD_ID_BASIC                                         ((Dcm_ThreadIdMemType)0u)             /*!< Thread ID which shall be used for thread independent processes */
/*! The diagnostic sub-component internal states
  Note: Bit-mapped states chose, since multiple states can be active at a time (e.g. an still in processing and on transmission for paged-buffer responses) */
# define DCM_DIAG_STATE_IDLE                                         ((Dcm_DiagProcessorStateType)0x00u) /*!< No diagnostic service processing in progress (yet) */
# define DCM_DIAG_STATE_PROCESS                                      ((Dcm_DiagProcessorStateType)0x01u) /*!< A diagnostic request is in processing */
# define DCM_DIAG_STATE_ON_TX                                        ((Dcm_DiagProcessorStateType)0x02u) /*!< A diagnostic response is on transmission */
# define DCM_DIAG_STATE_POST_PROCESS                                 ((Dcm_DiagProcessorStateType)0x04u) /*!< A diagnostic request is in the post-processing phase */
# if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
#  if (DCM_DCM_AR_VERSION == DCM_DCM_AR_VERSION_421) || \
      (DCM_DCM_AR_VERSION == DCM_DCM_AR_VERSION_422)
#   define DCM_ROE_AR_STATE_ACTIVE                                   RTE_MODE_DcmResponseOnEvent_EVENT_STARTED
#   define DCM_ROE_AR_STATE_INACTIVE                                 RTE_MODE_DcmResponseOnEvent_EVENT_STOPPED
#   define DCM_ROE_AR_STATE_CLEARED                                  RTE_MODE_DcmResponseOnEvent_EVENT_CLEARED
#   define Dcm_RoEOnEventStateChange(eventId, state)                 (Dcm_ModeSwitchRoE((eventId), (state)))                                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  else
#   error "Not supported AR version for RoE!"
#  endif
# endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! Splittask related critical section management */
# if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
#  define Dcm_SplitTaskEnterCS()                                     Dcm_UtiEnterCriticalSection()                                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define Dcm_SplitTaskLeaveCS()                                     Dcm_UtiLeaveCriticalSection()                                                   /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define Dcm_SplitTaskEnterCS()                                     /* Not used */
#  define Dcm_SplitTaskLeaveCS()                                     /* Not used */
# endif
/*! Getter/Setter of the transport object carrying the diagnostic request currently in processing */
# define Dcm_DiagGetTranspObj(pContext)                              (Dcm_NetGetTransportObject(pContext->Diag.TObjHdl))                             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_DiagSetTranspObj(pContext, tObjPtr)                     (pContext->Diag.TObjHdl = (tObjPtr)->Handle)                                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Control data for diagnostic service post-processing */
struct DCM_DIAGPOSTPROCESSORCONTEXTTYPE_TAG
{
  uint16 SrcAddr; /*!< Source address */
  uint8  Sid;     /*!< Service identifier */
  uint8  ReqType; /*!< Request type (physical/functional) */
};
typedef struct DCM_DIAGPOSTPROCESSORCONTEXTTYPE_TAG Dcm_DiagPostProcessorContextType;

/*! A sub-service length (i.e. sub-function and eventually some data byte) must always fit the linear buffer size (no paged-buffer request reception) */
typedef Dcm_CfgNetBufferSizeMemType Dcm_DiagSubServiceLengthType;

typedef P2VAR(Dcm_DiagSubServiceRefOptType, TYPEDEF, DCM_VAR_NOINIT)  Dcm_DiagSubServiceRefOptPtrType;

/*! Base data type for the diagnostic sub-component internal state machine */
typedef uint8 Dcm_DiagProcessorStateType;
/*! Service 0x10 temporary processing context */
struct DCM_SVC10REPEATERPROXYCONTEXTTYPE_TAG
{
# if (DCM_SVC_10_JMP2BOOT_ENABLED == STD_ON)
  Dcm_ProgConditionsType    ProgConditions;          /*!< Programming condition for further processing steps */
  Rte_ModeType_DcmEcuReset  ResetMode;               /*!< Reset mode for Ecu reset */
# endif
  Dcm_StateIndexMemType     SesStateIdx;             /*!< Requested new Session State to execute transition in post processor */
};
typedef struct DCM_SVC10REPEATERPROXYCONTEXTTYPE_TAG Dcm_Svc10RepeaterProxyContextType;
typedef P2VAR(Dcm_Svc10RepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc10RepeaterProxyContextPtrType;
# if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
/*! Service 0x2C temporary processing context */
struct DCM_SVC2CREPEATERPROXYCONTEXTTYPE_TAG
{
  Dcm_DidMgrDidInfoContextType           DynDidInfoContext;      /*!< DynDID info context */
  Dcm_DidMgrDidInfoContextType           SrcItemDidInfoContext;  /*!< Source item DID info context */
  Dcm_DidMgrDidOpTypeContextType         DynDidOpTypeContext;    /*!< DynDID operation type context */
  Dcm_CfgDidMgrDynDidSrcItemIdxMemType   SrcItemIndex;           /*!< Index of currently processed source item */
  Dcm_CfgDidMgrDynDidHandleMemType       DynDidHandle;           /*!< Handle of the currently processed DynDID */
  Dcm_DidMgrDidLengthType                MaxAllowedLength;       /*!< Maximum allowed response length (depending on the protocol) */
  Dcm_DidMgrDidLengthType                DynDidLength;           /*!< Length of the DynDID */
  uint8                                  DynDidItemCount;        /*!< Number of source items in current request */
  uint8                                  ReqNumOfItems;          /*!< Number of requested items that are not yet processed */
  Dcm_DiagSubServiceRefMemType           SubSvcRef;              /*!< Index of the requested subfunction */
};
typedef struct DCM_SVC2CREPEATERPROXYCONTEXTTYPE_TAG Dcm_Svc2CRepeaterProxyContextType;
typedef P2VAR(Dcm_Svc2CRepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc2CRepeaterProxyContextPtrType;
typedef P2CONST(Dcm_Svc2CRepeaterProxyContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_Svc2CRepeaterProxyContextConstPtrType;
# endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
# if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
typedef P2CONST(Dcm_CfgSvc86SubFuncInfoType, TYPEDEF, DCM_CONST) Dcm_CfgSvc86SubFuncInfoPtrType;
# endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */

/*! Base data type for the application notification levels */
typedef uint8 Dcm_DiagApplNotificationType;

/*! Base data type for the diagnostic kernel processing state flags */
typedef uint8 Dcm_DiagProcessorFlagType;

/*! Diagnostic kernel synchronization data */
struct DCM_DIAGSYNCHDATATYPE_TAG
{
  Dcm_CfgNetTObjHandleMemType      TObjHdl;        /*!< A single element queue for the transport object handle that holds the request data to be processed at next Dcm_DiagTaskWorker() activation */
# if (DCM_DIAG_GARB_COL_ENABLED == STD_ON)
  Dcm_TskTaskEvMemType             QueuedTObjects; /*!< Bitmask of queued transport objects */
# endif
  Dcm_DiagProcessorFlagType        Flags;          /*!< The diagnostic kernel status flags (Valid values: DCM_DIAG_QUEUE_FLAG_*) */
};
typedef struct DCM_DIAGSYNCHDATATYPE_TAG Dcm_DiagSynchDataType;

/*! Control thread specific data of the sub-component diagnostic kernel */
struct DCM_DIAGTHREADCONTEXTTYPE_TAG
{
  Dcm_MsgContextType               MsgContext;            /*!< The diagnostic service/sub-service processor message context */
  volatile Dcm_DiagSynchDataType   QueuedSet;             /*!< Diagnostic kernel processing synchronization data */
  Dcm_DiagPostProcessorContextType PostProcessorContext;  /*!< Post-processing context */
  Dcm_CfgNetTObjHandleMemType      TObjHdl;               /*!< The transport object handle of the diagnostic request in processing */
  Dcm_DiagP2TimingsType            P2Timings;             /*!< Currently active set of the P2/P2* timings */
  Dcm_NetTransmissionResultType    TxStatus;              /*!< Transports the (RCR-RP and final) USDT response transmission result to the Dcm_DiagTaskWorker() */
  Dcm_DiagApplNotificationType     ApplNotification;      /*!< Application notification level reached for current diagnostic request (Valid values: DCM_DIAG_APPL_NOTIFICATION_*) */
  volatile Dcm_DiagProcessorStateType State;              /*!< Keeps track of the diagnostic kernel processing state per diagnostic request */
  Dcm_NegativeResponseCodeType     ErrorRegister;         /*!< Keeps the NRC for the current diagnostic service in processing */
  uint8                            SidIndex;              /*!< Reference to Dcm_CfgDiagServiceInfo[] entry */
# if (DCM_DIAG_RCRRP_LIMIT_ENABLED == STD_ON)
  uint8                            NumRcrRpTimeouts;      /*!< Keeps track of the number of RCR-RPs sent for the current diagnostic service processing */
# endif
  boolean                          ProcessServiceExtern;  /*!< Diagnostic service dispatching */
  boolean                          DoSuppressResponse;    /*!< Used dedicated flag instead of "NRC 0x11 and func_req" combination with the same effect, because of possible application confirmation functions! */
# if (DCM_NET_INTERNAL_REQ_ENABLED == STD_ON)
  boolean                          IsInternRequest;       /*!< Specifies whether current request in processing is internal i.e. send via Dcm_ProcessVirtualRequest() or internal Dcm_NetRxIndInternal() API (TRUE - internal, FALSE - external (normal)) */
# endif
};
typedef struct DCM_DIAGTHREADCONTEXTTYPE_TAG Dcm_DiagThreadContextType;

/*! Function pointer prototype for a diagnostic service specific length getter */
typedef P2FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE, Dcm_DiagServiceLengthGetterFuncType) (
  Dcm_DiagSubServiceRefOptType pSubSvcRef  /* IN: Abstract (diagnostic service processor specific) sub-function handle */
  );

/*! Function pointer prototype for a diagnostic service with sub-function specific sequence checker */
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_DiagSubFuncSequenceCheckerFuncType) (
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DiagSubServiceRefOptType pSubSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/*! Context information of service processor repeater of each service processor */
union DCM_REPEATERPROXYSVCCONTEXTTYPE_TAG
{                                                                                                                                                    /* PRQA S 0750 */ /* MD_Dcm_Rule19.2 */
# if (DCM_SVC_01_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc01RepeaterProxyContextType Svc01;  /*!< Service 0x01 temporary processing context */
# endif
# if (DCM_SVC_02_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc02RepeaterProxyContextType Svc02;  /*!< Service 0x02 temporary processing context */
# endif
# if (DCM_SVC_03_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc03RepeaterProxyContextType Svc03;  /*!< Service 0x03 temporary processing context */
# endif
# if (DCM_SVC_04_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc04RepeaterProxyContextType Svc04;  /*!< Service 0x04 temporary processing context */
# endif
# if (DCM_SVC_06_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc06RepeaterProxyContextType Svc06;  /*!< Service 0x06 temporary processing context */
# endif
# if (DCM_SVC_07_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc07RepeaterProxyContextType Svc07;  /*!< Service 0x07 temporary processing context */
# endif
# if (DCM_SVC_08_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc08RepeaterProxyContextType Svc08;  /*!< Service 0x08 temporary processing context */
# endif
# if (DCM_SVC_09_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc09RepeaterProxyContextType Svc09;  /*!< Service 0x09 temporary processing context */
# endif
# if (DCM_SVC_0A_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc0ARepeaterProxyContextType Svc0A;  /*!< Service 0x0A temporary processing context */
# endif
  Dcm_Svc10RepeaterProxyContextType Svc10;  /*!< Service 0x10 temporary processing context */
# if (DCM_SVC_11_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc11RepeaterProxyContextType Svc11;  /*!< Service 0x11 temporary processing context */
# endif
# if (DCM_SVC_14_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc14RepeaterProxyContextType Svc14;  /*!< Service 0x14 temporary processing context */
# endif
# if (DCM_SVC_19_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc19RepeaterProxyContextType Svc19;  /*!< Service 0x19 temporary processing context */
# endif
# if (DCM_SVC_22_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc22RepeaterProxyContextType Svc22;  /*!< Service 0x22 temporary processing context */
# endif
# if (DCM_SVC_23_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc23RepeaterProxyContextType Svc23;  /*!< Service 0x23 temporary processing context */
# endif
# if (DCM_SVC_24_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc24RepeaterProxyContextType Svc24;  /*!< Service 0x24 temporary processing context */
# endif
# if (DCM_SVC_27_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc27RepeaterProxyContextType Svc27;  /*!< Service 0x27 temporary processing context */
# endif
# if (DCM_SVC_28_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc28RepeaterProxyContextType Svc28;  /*!< Service 0x28 temporary processing context */
# endif
# if (DCM_SVC_29_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc29RepeaterProxyContextType Svc29;  /*!< Service 0x29 temporary processing context */
# endif
# if (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc2ARepeaterProxyContextType Svc2A;  /*!< Service 0x2A temporary processing context */
# endif
# if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc2CRepeaterProxyContextType Svc2C;  /*!< Service 0x2C temporary processing context */
# endif
# if (DCM_SVC_2E_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc2ERepeaterProxyContextType Svc2E;  /*!< Service 0x2E temporary processing context */
# endif
# if (DCM_SVC_2F_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc2FRepeaterProxyContextType Svc2F;  /*!< Service 0x2F temporary processing context */
# endif
# if (DCM_SVC_31_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc31RepeaterProxyContextType Svc31;  /*!< Service 0x31 temporary processing context */
# endif
# if (DCM_SVC_34_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc34RepeaterProxyContextType Svc34;  /*!< Service 0x34 temporary processing context */
# endif
# if (DCM_SVC_36_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc36RepeaterProxyContextType Svc36;  /*!< Service 0x36 temporary processing context */
# endif
# if (DCM_SVC_37_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc37RepeaterProxyContextType Svc37;  /*!< Service 0x37 temporary processing context */
# endif
# if (DCM_SVC_3D_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc3DRepeaterProxyContextType Svc3D;  /*!< Service 0x3D temporary processing context */
# endif
# if (DCM_SVC_85_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc85RepeaterProxyContextType Svc85;  /*!< Service 0x85 temporary processing context */
# endif
# if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
  Dcm_Svc86RepeaterProxyContextType Svc86;  /*!< Service 0x86 temporary processing context */
# endif
};
typedef union DCM_REPEATERPROXYSVCCONTEXTTYPE_TAG Dcm_RepeaterProxySvcContextType;

/*! Diagnostic service processor repeater context */
struct DCM_REPEATERTHREADCONTEXTTYPE_TAG
{
  Dcm_RepeaterProxySvcContextType Context;     /*!< Service specific repeater contexts */
  uint8                           CalleeIdx;   /*!< Index of the next function to call */
  Dcm_OpStatusType                OpStatus;    /*!< Current operation status */
  Dcm_RepeaterProgressType        Progress;    /*!< Progress of the repeater */
  Dcm_RepeaterProgressType        SubProgress; /*!< Progress of a sub-repeater */
};
typedef struct DCM_REPEATERTHREADCONTEXTTYPE_TAG Dcm_RepeaterThreadContextType;
/*! Data container with all thread specific elements (i.e. for parallel Diagnostic service handling)  */
struct DCM_CONTEXTTYPE_TAG
{
# if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
  Dcm_PagedBufferContextType     PagedBuffer;  /*!< Paged-buffer manager sub-component per-instance memory */
# endif
  Dcm_DiagThreadContextType      Diag;         /*!< Diagnostic kernel sub-component per-thread memory */
  Dcm_RepeaterThreadContextType  Repeater;     /*!< Diagnostic service processor repeater sub-component per-thread memory */
  Dcm_ThreadIdMemType            ThreadId;     /*!< Active thread */
};

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferStart()
 *********************************************************************************************************************/
/*! \brief          Activate paged-buffer write process.
 *  \details        -
 *  \param[in,out]  pMsgContext   Current request context
 *  \param[in]      resDataLen    Specifies how many bytes will be copied using the update page function.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_PagedBufferStart(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_MsgLenType resDataLen
  );
# endif
/**********************************************************************************************************************
 *  Dcm_GetThreadContext()
 *********************************************************************************************************************/
/*! \brief          Returns a thread context element of the given index.
 *  \details        -
 *  \param[in]      threadId    The active thread identifier
 *  \return         Valid thread context.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_ContextPtrType, DCM_CODE) Dcm_GetThreadContext(
  Dcm_ThreadIdOptType threadId
  );
/**********************************************************************************************************************
 *  Dcm_DiagIsKernelBusy()
 *********************************************************************************************************************/
/*! \brief          Checks if the diagnostic kernel is busy with a diagnostic job on any thread.
 *  \details        -
 *  \return         TRUE     Diagnostic kernel is busy
 *  \return         FALSE    Diagnostic kernel is not busy
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_DiagIsKernelBusy(
  void
  );

# if (DCM_VARMGR_MULTI_SVC_ANY_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagIsEnabledInActiveVariants()
 *********************************************************************************************************************/
/*! \brief          Checks if a diagnostic object is active in at least one variant.
 *  \details        -
 *  \param[in]      cfgVariantRef    Reference to the variant configuration
 *  \return         TRUE             Diagnostic object is active in at least one variant
 *  \return         FALSE            Diagnostic object is not active
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_DiagIsEnabledInActiveVariants(
  Dcm_CfgStateRefOptType cfgVariantRef
  );
# endif

/**********************************************************************************************************************
 *  Dcm_DiagVariantLookUpFilter()
 *********************************************************************************************************************/
/*! \brief          Performs a lookup result filtering.
 *  \details        Checks whether a diagnostic object is supported in the active variant.
 *  \param[in]      lookUpTable          Pointer to the table to be scanned (first element is table-size!)
 *  \param[in]      lookUpFilterTable    List of filters
 *  \param[in]      value                Value to be found
 *  \return         -1     Diagnostic entity not active in current variant
 *  \return         >=0    Success, index of the matched position
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(sint16_least, DCM_CODE) Dcm_DiagVariantLookUpFilter(
  Dcm_Uint8ConstDataPtrType lookUpTable,
  Dcm_VarRefMemPtrType lookUpFilterTable,
  uint8 value
  );

/**********************************************************************************************************************
 *  Dcm_DiagGetDemClientId()
 *********************************************************************************************************************/
/*! \brief          Get DEM Client Id.
 *  \details        -
 *  \param[in]      pMsgContext     Message-related information for one diagnostic protocol identifier 
 *  \return         DEM Client Id
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(uint8, DCM_CODE) Dcm_DiagGetDemClientId(
  Dcm_ReadOnlyMsgContextPtrType pMsgContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagGetCancelFunc()
 *********************************************************************************************************************/
/*! \brief          Returns a pointer to a cancellation function of the given index.
 *  \details        -
 *  \param[in]      index    Unique handle to a cancellation function
 *  \return         Paged-buffer cancel-handler
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_DiagSvcCancelFuncType, DCM_CODE) Dcm_DiagGetCancelFunc(
  uint8_least index
  );

/**********************************************************************************************************************
 *  Dcm_DiagSvcWithOnlySubFuncReqLengthGetter()
 *********************************************************************************************************************/
/*! \brief          Default sub-function length getter.
 *  \details        Always returns length of 1 byte (sub-function byte only expected)
 *  \param[in]      sfRef                The sub-function zero-based index.
 *  \return         Expected sub-function specific length (1 byte only)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_DiagSvcWithOnlySubFuncReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  );

/**********************************************************************************************************************
 *  Dcm_DiagNoSequenceChecker()
 *********************************************************************************************************************/
/*! \brief          Default sub-function sequence checker.
 *  \details        Always returns DCM_E_OK.
 *  \param[in,out]  pMsgContext          The current request context
 *  \param[in]      subSvcRef            The sub-function zero-based index.
 *  \param[out]     ErrorCode            The NRC
 *  \return         DCM_E_OK             Validation passed
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagNoSequenceChecker(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_DiagSubServiceRefOptType subSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  Dcm_DiagVerifyServiceWithSubFunction()
 *********************************************************************************************************************/
/*! \brief          Verifies a service with sub-function.
 *  \details        Implements the ISO14229-1:2013 algorithm for sub-function validation
 *  \param[in,out]  pMsgContext            Current request context
 *  \param[in]      lookUpTable            The sub-service lookup table
 *  \param[in]      lookUpFilterTable      List of sub-service related filters
 *  \param[in]      svcLengthGetter        Functor for accessing a sub-service specific length
 *  \param[in]      subFuncSequenceChecker Function for accessing a service specific sequence check
 *  \param[out]     pSubSvcRef             A zero-based sub-service index for further processing
 *  \param[out]     ErrorCode              The NRC
 *  \return         DCM_E_OK               Validation passed
 *  \return         DCM_E_NOT_OK           Validation failed with the returned ErrorCode
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_DiagVerifyServiceWithSubFunction(
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_Uint8ConstDataPtrType lookUpTable,
  Dcm_VarRefMemPtrType lookUpFilterTable,
  Dcm_DiagServiceLengthGetterFuncType svcLengthGetter,
  Dcm_DiagSubFuncSequenceCheckerFuncType subFuncSequenceChecker,
  Dcm_DiagSubServiceRefOptPtrType pSubSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

# if (DCM_STATE_SECURITY_BYPASS_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_DiagSetIsSecurityBypass()
 *********************************************************************************************************************/
/*! \brief          Sets the IsSecurityBypass value.
 *  \details        -
 *  \param[in]      isSecurityBypass       The value to be set
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagSetIsSecurityBypass(
  boolean isSecurityBypass
  );
# endif
# if (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON)
#  if (DCM_DIDMGR_DYNDID_SRCITEM_CHECK_STATE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc2CStateCheckSrcItems()
 *********************************************************************************************************************/
/*! \brief          Check state and mode group preconditions of each source item.
 *  \details        -
 *  \param[in]      connHdl         The connection identifier
 *  \param[in]      dynDidHandle    DynDID reference to be checked
 *  \param[out]     ErrorCode       The NRC
 *  \return         DCM_E_OK        Check was successful
 *  \return         DCM_E_OK        Check was not successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc2CStateCheckSrcItems(
  Dcm_NetConnRefMemType connHdl,
  Dcm_CfgDidMgrDynDidHandleOptType dynDidHandle,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );
#  endif

/**********************************************************************************************************************
 *  Dcm_Svc2CIsDidDefined()
 *********************************************************************************************************************/
/*! \brief          Check whether DynDid is defined.
 *  \details        -
 *  \param[in]      pDidInfo        The DID information
 *  \return         true            Did is a DynDid
 *  \return         false           DID is not a DynDid
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_Svc2CIsDidDefined(
  Dcm_CfgDidMgrDidInfoConstPtrType pDidInfo
  );
# endif /* (DCM_SVC_2C_SUPPORT_ENABLED == STD_ON) */
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
#  if (DCM_SVC_86_RST_ON_DSC_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_ExtSvc86OnChangeSession()
 *********************************************************************************************************************/
/*! \brief          Service 0x86 utility function to notify RoE about a session change.
 *  \details        -
 *  \param[in]      oldSession    The old session
 *  \param[in]      newSession    The old session
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ExtSvc86OnChangeSession(
  Dcm_CfgStateGroupOptType oldSession,
  Dcm_CfgStateGroupOptType newSession
  );
#  endif
# endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#endif /* !defined(DCM_COREINT_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_CoreInt.h
 *********************************************************************************************************************/
