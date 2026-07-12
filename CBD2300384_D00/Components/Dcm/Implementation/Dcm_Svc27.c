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
/**        \file  Dcm_Svc27.c
 *         \unit  Svc27
 *        \brief  Contains the implementation of Service 0x27 unit.
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
#define DCM_SVC27_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Svc27.h"
#if (DCM_SVC_27_SUPPORT_ENABLED == STD_ON)
# include "Dcm_Repeater.h"
# include "Dcm_Debug.h"
# include "Dcm_Uti.h"
# include "Dcm_Tmr.h"
# include "Dcm_Tsk.h"
# include "Dcm_State.h"
# include "Dcm_CfgWrap.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Service 0x27 processing steps */
# define DCM_SVC27_PROGRESS_SEEDPROCESSOR                            ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 0u))
# define DCM_SVC27_PROGRESS_KEYPROCESSOR                             ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 1u))
# define DCM_SVC27_PROGRESS_SETATTEMPTCNTR                           ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 2u))
# define DCM_SVC27_PROGRESS_SEEDREPEATER                             ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 3u))
# define DCM_SVC27_PROGRESS_KEYREPEATER                              ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 4u))
# define DCM_SVC27_PROGRESS_CHECKATTEMPTSEXCEEDED                    ((Dcm_RepeaterProgressType)(DCM_REPEATER_PROGRESS_USERS_FIRST + 5u))

/*! Get-Seed port operation classes  */
# define DCM_SVC27_OPCLS_SEED_WITHOUT_ADR                            ((Dcm_Svc27OpClassType)0)
# define DCM_SVC27_OPCLS_SEED_WITH_ADR                               ((Dcm_Svc27OpClassType)1)

/*! Security level value for which a seed was sent */
# define DCM_SVC_27_LEVEL_MASK                                       (Dcm_UtiGetMaskFromXintType(uint32, DCM_STATE_SECURITY_NUM_LEVELS))
# define DCM_SVC_27_SEED_LVL_INVALID                                 ((uint8)0x00u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
# define Dcm_Svc27ResetSequence()                                    (Dcm_Svc27SingletonContext.SeedLevel = DCM_SVC_27_SEED_LVL_INVALID)             /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_Svc27IsKeyRequestExpected()                             ((boolean)(Dcm_Svc27SingletonContext.SeedLevel != DCM_SVC_27_SEED_LVL_INVALID)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_Svc27GetSecLvlRef(subSvcRef)                            ((uint8)((subSvcRef) / 2u))                                                     /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_Svc27IsSeedRequest(subSvcRef)                           ((boolean)(((subSvcRef) & 0x01u) == 0u)) /* Even subSvcRef means "request seed" request */ /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_Svc27IsKeyRequest(subSvcRef)                            ((boolean)(Dcm_Svc27IsSeedRequest(subSvcRef) == FALSE)) /* Odd subSvcRef means "send key" request */ /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_Svc27IsThisKeyRequestExpected(secLvlRef)                ((boolean)((Dcm_CfgWrapStateSecurityInfo(secLvlRef))->Value == Dcm_Svc27SingletonContext.SeedLevel)) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

# if (DCM_SVC_27_MULTI_ATTEMPT_COUNTER_ENABLED == STD_ON)
#  define Dcm_Svc27GetCounterIdx(secLvl)                             (secLvl)                                                                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define Dcm_Svc27GetCounterIdx(secLvl)                             0u
# endif

# if (DCM_SVC_27_MULTI_DELAY_TIMER_ENABLED == STD_ON)
#  define Dcm_Svc27GetTimerIdx(secLvl)                               (secLvl)                                                                        /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# else
#  define Dcm_Svc27GetTimerIdx(secLvl)                               0u
# endif

# define Dcm_Svc27CounterGet(secLev)                                 (Dcm_Svc27SingletonContext.AttemptCount[Dcm_Svc27GetCounterIdx((secLev))])      /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_Svc27CounterReset(secLev)                               (Dcm_Svc27CounterSet((secLev), 0u))                                             /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_Svc27TimerGet(secLev)                                   (Dcm_Svc27SingletonContext.DelayTime[Dcm_Svc27GetTimerIdx((secLev))])           /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_Svc27TimerReset(secLev)                                 (Dcm_Svc27TimerSet((secLev), 0u))                                               /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_Svc27TimerActive(secLev)                                ((boolean)(Dcm_Svc27TimerGet((secLev)) != 0u))                                  /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_Svc27TimerJustExpired(mask, secLev)                     ((boolean)(Dcm_UtiBitOpTest(uint32, (mask), Dcm_UtiGetBitFromIndex(uint32_least, Dcm_Svc27GetTimerIdx((secLev)))))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_Svc27ReloadReadFailedGet(secLev)                        (Dcm_UtiBitOpTest(uint32, Dcm_Svc27SingletonContext.ReloadMask, Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvOptType, (secLev)))) /* PRQA S 3453 */ /* MD_MSR_19.7 */
# define Dcm_Svc27ReloadReadFailedSet(secLev)                        (Dcm_UtiBitOpSet(uint32, Dcm_Svc27SingletonContext.ReloadMask, Dcm_UtiGetBitFromIndex(Dcm_TskTaskEvOptType, (secLev)))) /* PRQA S 3453 */ /* MD_MSR_19.7 */

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Context for attempt counter handling */
struct DCM_SVC27SINGLETONCONTEXTTYPE_TAG
{
# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
  uint32                            GetAttCntrEventMask;                            /*!< State of attempt counter read operations */
  uint32                            SetAttCntrEventMask;                            /*!< State of attempt counter write operations */
# endif
  uint32                            ReloadMask;                                     /*!< Infinite delay timer mask */
  volatile Dcm_TmrTimerCntrMemType  DelayTime[DCM_SVC_27_NUM_DELAY_TIMERS];         /*!< Array of delay timers for UUDT messages */
  uint8                             AttemptCount[DCM_SVC_27_NUM_ATTEMPT_COUNTERS];  /*!< Array of attempt counters */
# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
  Dcm_OpStatusType                  SetAttOpStatus;                                 /*!< Operation state of attempt counter write operation */
  Dcm_OpStatusType                  GetAttOpStatus;                                 /*!< Operation state of attempt counter read operation */
# endif
  uint8                             SeedLevel;                                      /*!< Last requested security level for which a seed was sent (0 = invalid (no seed) -> 1..7F - active seed) */
};
typedef struct DCM_SVC27SINGLETONCONTEXTTYPE_TAG Dcm_Svc27SingletonContextType;

/*! Function pointer prototype for a Get-Seed handler with Security-Access-Data-Record */
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_Svc27OpFuncSeedWAdr)(
  Dcm_MsgType seed,
  Dcm_OpStatusType opStatus,
  Dcm_MsgType adr,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/*! Function pointer prototype for a Get-Seed handler without Security-Access-Data-Record */
typedef P2FUNC(Std_ReturnType, DCM_CODE, Dcm_Svc27OpFuncSeedWoAdr)(
  Dcm_OpStatusType opStatus,
  Dcm_MsgType seed,
  Dcm_NegativeResponseCodePtrType ErrorCode
  );

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc27CounterSet()
 *********************************************************************************************************************/
/*! \brief          Sets the value of a specific security level attempt counter.
 *  \details        -
 *  \param[in]      secLev      Zero based security level index
 *  \param[in]      value       Attempt counter value
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc27CounterSet(
  uint8_least secLev,
  uint8 value
  );

/**********************************************************************************************************************
 *  Dcm_Svc27CounterInc()
 *********************************************************************************************************************/
/*! \brief          Increments the value of a specific security level attempt counter.
 *  \details        -
 *  \param[in]      secLev      Zero based security level index
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc27CounterInc(
  uint8_least secLev
  );

/**********************************************************************************************************************
 *  Dcm_Svc27TimerSet()
 *********************************************************************************************************************/
/*! \brief          Sets the value of a specific security level timer.
 *  \details        -
 *  \param[in]      secLev      Zero based security level index
 *  \param[in]      value       Timer value
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc27TimerSet(
  uint8_least secLev,
  Dcm_TmrTimerCntrMemType value
  );

/**********************************************************************************************************************
 *  Dcm_Svc27TimerStart()
 *********************************************************************************************************************/
/*! \brief          Sets the value of a specific security level timer.
 *  \details        Starts the base security level timer.
 *  \param[in]      secLev      Zero based security level index
 *  \param[in]      value       Timer value for the specific security level
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc27TimerStart(
  uint8_least secLev,
  Dcm_TmrTimerCntrMemType value
  );

/**********************************************************************************************************************
 *  Dcm_Service27Handler()
 *********************************************************************************************************************/
/*! \brief          Service 0x27 Handler.
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Service27Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc27RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Service27SeedProcessor()
 *********************************************************************************************************************/
/*! \brief          Service 0x27 get application generated seed.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \return         DCM_E_OK                Request was successful
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Service27SeedProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc27RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Service27KeyProcessor()
 *********************************************************************************************************************/
/*! \brief          Service 0x27 get application key comparator.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                The operations status
 *  \param[in,out]  pMsgContext             Current message context
 *  \param[out]     ErrorCode               Negative response code in case return value is DCM_E_NOT_OK
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Service27KeyProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc27RepeaterProxyContextPtrType pRepContext
  );

# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc27UtiGetAttemptCntr()
 *********************************************************************************************************************/
/*! \brief          Restores the attempt counter values from the application.
 *  \details        -
 *  \param[in]      opStatus                Current operation status
 *  \param[in,out]  levelMask               Pointer to a security level mask.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc27UtiGetAttemptCntr(
  Dcm_OpStatusType opStatus,
  P2VAR(uint32, AUTOMATIC, DCM_VAR_NOINIT) levelMask
  );
# endif

# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc27ReadAttemptCounter()
 *********************************************************************************************************************/
/*! \brief          Manages reading of an attempt counter value.
 *  \details        If a diagnostic job is ongoing the appropriate action will be taken to close it deterministically.
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc27ReadAttemptCounter(
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc27WriteAttemptCounter()
 *********************************************************************************************************************/
/*! \brief          Manages writing of an attempt counter value.
 *  \details        If a diagnostic job is ongoing the appropriate action will be taken to close it deterministically.
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc27WriteAttemptCounter(
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

/**********************************************************************************************************************
 *  Dcm_Service27SeedRepeater()
 *********************************************************************************************************************/
/*! \brief          Service 0x27 get seed service port accessor.
 *  \details        -
 *  \param[in]      opStatus                Current operation status
 *  \param[in,out]  pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Service27SeedRepeater(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc27RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Service27KeyRepeater()
 *********************************************************************************************************************/
/*! \brief          Service 0x27 send key service port accessor.
 *  \details        -
 *  \param[in]      pContext                Pointer to the context
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send neagative response
 *  \return         DCM_E_PENDING           Retry later
 *  \return         DCM_E_LOOP              Repeat Repeater Proxy immediately
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Service27KeyRepeater(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc27RepeaterProxyContextPtrType pRepContext
  );

/**********************************************************************************************************************
 *  Dcm_Service27CheckAttemptsExceededRepeater()
 *********************************************************************************************************************/
/*! \brief          Checks whether the number of allowed attempts were reached.
 *  \details        Determines the final negative response code (0x35 or 0x36).
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Service27CheckAttemptsExceededRepeater(
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc27RepeaterProxyContextPtrType pRepContext
  );

# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service27SetAttemptCntrRepeater()
 *********************************************************************************************************************/
/*! \brief          Service 0x27 set attempt counter service port accessor.
 *  \details        -
 *  \param[in]      opStatus                Current operation status
 *  \param[in]      pMsgContext             Pointer to current message context
 *  \param[out]     ErrorCode               The NRC
 *  \param[in,out]  pRepContext             Current repeater proxy context
 *  \return         DCM_E_OK                Send response
 *  \return         DCM_E_NOT_OK            Send negative response
 *  \return         DCM_E_PENDING           Retry later
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Service27SetAttemptCntrRepeater(
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc27RepeaterProxyContextPtrType pRepContext
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc27UtiSetAttemptCntr()
 *********************************************************************************************************************/
/*! \brief          Notifies the application about changed attempt counter values.
 *  \details        -
 *  \param[in]      opStatus                Current operation status
 *  \param[in,out]  levelMask               Pointer to a security level mask.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_Svc27UtiSetAttemptCntr(
  Dcm_OpStatusType opStatus,
  P2VAR(uint32, AUTOMATIC, DCM_VAR_NOINIT) levelMask
  );
# endif

/**********************************************************************************************************************
 *  Dcm_Svc27ReqLengthGetter()
 *********************************************************************************************************************/
/*! \brief          Service 0x27 sub-function length getter.
 *  \details        Always returns the sub-function specific length.
 *  \param[in]      sfRef                The sub-function zero-based index.
 *  \return         Expected sub-function specific length.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc27ReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  );

/**********************************************************************************************************************
 *  Dcm_Svc27SubFuncSequenceChecker()
 *********************************************************************************************************************/
/*! \brief          Service 0x27 sub-function sequence checker.
 *  \details        Checks whether the sub-function sequence is correct.
 *  \param[in]      pMsgContext          Pointer to current message context
 *  \param[in]      subSvcRef            The sub-function zero-based index.
 *  \param[out]     ErrorCode            The NRC
 *  \return         DCM_E_OK             Validation passed
 *  \return         DCM_E_NOT_OK         Send negative response
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc27SubFuncSequenceChecker(
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
/*! Singleton context of service 2F unit */
DCM_LOCAL VAR(Dcm_Svc27SingletonContextType, DCM_VAR_NOINIT) Dcm_Svc27SingletonContext;                                                              /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
# define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Svc27CounterSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc27CounterSet(
  uint8_least secLev,
  uint8 value
  )
{
  uint8_least lCounterIdx = Dcm_Svc27GetCounterIdx(secLev);

  DCM_IGNORE_UNREF_PARAM(secLev);                                                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

# if (DCM_SVC_27_MULTI_ATTEMPT_COUNTER_ENABLED == STD_ON)
  if (Dcm_DebugDetectRuntimeError(lCounterIdx >= DCM_SVC_27_NUM_ATTEMPT_COUNTERS))                                                                   /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
# endif
  {
    Dcm_Svc27SingletonContext.AttemptCount[lCounterIdx] = value;                                                                                     /* SBSW_DCM_POINTER_WRITE_ATTEMPTCOUNT */
  }
}

/**********************************************************************************************************************
 *  Dcm_Svc27CounterInc()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc27CounterInc(
  uint8_least secLev
  )
{
  uint8 lValue = Dcm_Svc27CounterGet(secLev);

  if (lValue < Dcm_UtiMaxValueOfUintType(uint8))
  {
    Dcm_Svc27CounterSet(secLev, (uint8)(lValue + 1u));
  }
}

/**********************************************************************************************************************
 *  Dcm_Svc27TimerSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc27TimerSet(
  uint8_least secLev,
  Dcm_TmrTimerCntrMemType value
  )
{
  uint8_least lTimerIdx = Dcm_Svc27GetTimerIdx(secLev);

  DCM_IGNORE_UNREF_PARAM(secLev);                                                                                                                    /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

# if (DCM_SVC_27_MULTI_DELAY_TIMER_ENABLED == STD_ON)
  if (Dcm_DebugDetectRuntimeError(lTimerIdx >= DCM_SVC_27_NUM_DELAY_TIMERS))                                                                         /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
  }
  else
# endif
  {
    /* Enter critical section (Reason: Avoid interrupt from Dcm_OnTimeoutSecurityAccess) */
    Dcm_SplitTaskEnterCS();
    /*=================================*
    BEGIN CRITICAL SECTION
    *=================================*/

    Dcm_Svc27SingletonContext.DelayTime[lTimerIdx] = value;                                                                                          /* SBSW_DCM_POINTER_WRITE_DELAYTIME_SEC */

    /*=================================*
    END CRITICAL SECTION
    *=================================*/
    Dcm_SplitTaskLeaveCS();
  }
}

/**********************************************************************************************************************
 *  Dcm_Svc27TimerStart()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc27TimerStart(
  uint8_least secLev,
  Dcm_TmrTimerCntrMemType value
  )
{
  /* Start the appropriate security timer */
  Dcm_Svc27TimerSet(secLev, value);

  if (!Dcm_TmrIsTimerRunning(DCM_TMR_ID_SECURITY))
  {
    /* Start the base security timer */
    Dcm_TmrStartTimer(DCM_TMR_ID_SECURITY, 1u);
  }
}

/**********************************************************************************************************************
 *  Dcm_Service27Handler()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Service27Handler(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc27RepeaterProxyContextPtrType pRepContext
  )
{
  Std_ReturnType                lStdResult;
  Dcm_DiagSubServiceRefOptType  lSubSvcRef;
  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lStdResult = Dcm_DiagVerifyServiceWithSubFunction(pMsgContext
                                                   ,Dcm_CfgSvc27SubFuncLookUpTable
                                                   ,Dcm_CfgSvc27SubFuncExecPrecondTable
                                                   ,Dcm_Svc27ReqLengthGetter
                                                   ,Dcm_Svc27SubFuncSequenceChecker
                                                   ,&lSubSvcRef
                                                   ,ErrorCode);                                                                                      /* SBSW_DCM_COMB_PTR_FORWARD */

  /* Initialize service processor context for further processing */
  pRepContext->ErrorCode = DCM_E_POSITIVERESPONSE;                                                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->SubSvcRef = (Dcm_DiagSubServiceRefMemType)lSubSvcRef;/* Store working sub-function reference (also in negative response cases) */     /* SBSW_DCM_PARAM_PTR_WRITE */
  pRepContext->SecLvlRef = Dcm_Svc27GetSecLvlRef(lSubSvcRef);                                                                                        /* SBSW_DCM_PARAM_PTR_WRITE */

  if (lStdResult == DCM_E_OK)
  {
    if (Dcm_Svc27IsSeedRequest(pRepContext->SubSvcRef) == TRUE)                                                                                      /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    {
      Dcm_RepeaterNextStep(pContext, DCM_SVC27_PROGRESS_SEEDPROCESSOR);                                                                              /* SBSW_DCM_PARAM_PTR_FORWARD */
    }
    else
    {
      Dcm_RepeaterNextStep(pContext, DCM_SVC27_PROGRESS_KEYPROCESSOR);                                                                               /* SBSW_DCM_PARAM_PTR_FORWARD */
    }

    lStdResult = DCM_E_LOOP; /* Speed up processing */
  }
  /* If the requested key has wrong length or the sub-function sequence was not correct: */
  else if ((*ErrorCode == DCM_E_INCORRECTMESSAGELENGTHORINVALIDFORMAT)
         ||(*ErrorCode == DCM_E_REQUESTSEQUENCEERROR))
  {
    if ((Dcm_Svc27IsKeyRequest(pRepContext->SubSvcRef) == TRUE) /* wrong key length is an invalid key! */                                            /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      &&(Dcm_Svc27IsKeyRequestExpected() == TRUE))                                                                                                   /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    {
# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
      /* Prevent multiple calls to application due to overflow */
      if (Dcm_Svc27CounterGet(pRepContext->SecLvlRef) < Dcm_UtiMaxValueOfUintType(uint8))
      {
        Dcm_RepeaterNextStep(pContext, DCM_SVC27_PROGRESS_SETATTEMPTCNTR);                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
        pRepContext->ErrorCode = *ErrorCode; /* used in next step */                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
        lStdResult = DCM_E_LOOP;/* Speed up processing */
      }
# endif
      /* Increment counter after notification check */
      Dcm_Svc27CounterInc(pRepContext->SecLvlRef);
    }
    /* Send prepared NRC immediately */
  }
  else
  {
    /* Send prepared NRC immediately */
  }

  return lStdResult;
}

/**********************************************************************************************************************
 *  Dcm_Service27SeedProcessor()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service27SeedProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc27RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* If the attempt counters are currently read or written by the application: */
# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
  if (Dcm_TskGetEvent(DCM_TSK_ID_SVC27) != 0u)
  {
    lStdResult = DCM_E_PENDING;
  }
  else
# endif
  {
    pRepContext->IsZeroSeed = FALSE;                                                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */

    /* If the security timer is active: */
    if (Dcm_Svc27TimerActive(pRepContext->SecLvlRef) == TRUE)                                                                                        /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    {
      /* Send the NRC 0x37 */
      *ErrorCode = DCM_E_REQUIREDTIMEDELAYNOTEXPIRED;                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
      lStdResult = DCM_E_NOT_OK;
    }
    else
    {
      /* If seed level not yet unlocked: */
      if (0u == (Dcm_StateGetSecurity() & Dcm_UtiGetBitFromIndex(Dcm_CfgStateGroupMemType, (pRepContext->SecLvlRef + 1u))))
      {
        /* New seed request / repetition of the same seed request. Get application generated seed */
        Dcm_RepeaterNextStep(pContext, DCM_SVC27_PROGRESS_SEEDREPEATER);                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
        lStdResult = DCM_E_LOOP;/* Speed up processing */
      }
      else
      {
        Dcm_CfgNetBufferSizeMemType lSeedSize = Dcm_CfgWrapSvc27SecLevelInfo(pRepContext->SecLvlRef)->SeedResLength;

        lStdResult = Dcm_UtiMsgContextVerifySize(pMsgContext, lSeedSize, ErrorCode);                                                                 /* SBSW_DCM_PARAM_PTR_FORWARD */

        if (lStdResult == DCM_E_OK)                                                                                                                  /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
        {
          /* return zero seed */
          pRepContext->IsZeroSeed = TRUE;                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
          Dcm_UtiMemSetUintX(Dcm_UtiGetResData(pMsgContext), Dcm_MsgItemType, 0x00u, lSeedSize);                                                     /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */ /* SBSW_DCM_POINTER_WRITE_MESSAGE_CONTEXT_RESBUFFER */
          Dcm_UtiProvideResData(pMsgContext, lSeedSize);                                                                                             /* SBSW_DCM_PARAM_PTR_FORWARD */
        }
      }
    }
  }
  return lStdResult;
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_Service27KeyProcessor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service27KeyProcessor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,                                                                                                         /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_Svc27RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType lStdResult;

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(ErrorCode);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pRepContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* If the attempt counters are currently read or written by the application: */
# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
  if (Dcm_TskGetEvent(DCM_TSK_ID_SVC27) != 0u)
  {
    lStdResult = DCM_E_PENDING;
  }
  else
# endif
  {
    /* Get application key comparator */
    Dcm_RepeaterNextStep(pContext, DCM_SVC27_PROGRESS_KEYREPEATER);                                                                                  /* SBSW_DCM_PARAM_PTR_FORWARD */
    lStdResult = DCM_E_LOOP; /* Speed up processing */
  }
  return lStdResult;
}

# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc27UtiGetAttemptCntr()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc27UtiGetAttemptCntr(
  Dcm_OpStatusType opStatus,
  P2VAR(uint32, AUTOMATIC, DCM_VAR_NOINIT) levelMask
  )
{
  Std_ReturnType  lResult;
  uint8           lAttemptCount;
  uint8_least     lSecLvlIter;

  /* For each security level: */
  for (lSecLvlIter = 0u; lSecLvlIter < DCM_CFGSVC27SECLEVELINFO_SIZE; ++lSecLvlIter)
  {
    /* If the attempt counter for that specific security level is not already read: */
    if (Dcm_UtiBitOpTest(uint32, *levelMask, Dcm_UtiGetBitFromIndex(uint32, lSecLvlIter)))
    {
      Dcm_CfgSvc27SecLevelInfoPtrType pSecLevelInfo;
      Dcm_NegativeResponseCodeType    lNrc;

      lResult = Dcm_CfgWrapSvc27SecLevelInfoGetEntry(lSecLvlIter, &pSecLevelInfo, &lNrc);                                                            /* SBSW_DCM_COMB_PTR_FORWARD */

      if (lResult == DCM_E_OK)                                                                                                                       /* COV_DCM_RTM_RUNTIME_CHECK TX */ /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
      {
        /* Try to read the attempt counter from the application */
        lResult = pSecLevelInfo->GetAttemptCntrFunc(opStatus, &lAttemptCount);                                                                       /* SBSW_DCM_CALL_FUNCPTR_SVC27SECLEVEL */

        if (lResult == DCM_E_OK)
        {
          /*
           * Note: Using critical sections is not necessary here because the features "power on delay" and
           *       "external attempt counter storage" are used mutually exclusive per security level.
           */

          /* Mark the security level to prevent that this API is called again */
          Dcm_UtiBitOpClr(uint32, *levelMask, Dcm_UtiGetBitFromIndex(uint32, lSecLvlIter));                                                          /* SBSW_DCM_PARAM_PTR_WRITE */

          /* If the already initialized attempt counter has to be updated: */
          if (lAttemptCount != 0u)
          {
            /* Store the attempt counter */
            Dcm_Svc27CounterSet(lSecLvlIter, lAttemptCount);

            /* If the current attempt counter value exceeded the maximum number of allowed attempts: */
            if (lAttemptCount >= (Dcm_CfgWrapStateSecurityInfo(lSecLvlIter))->NumAttempts)
            {
              /* Set the delay timer value. The timer will be started later with that value */
              Dcm_Svc27TimerSet(lSecLvlIter, (Dcm_CfgWrapStateSecurityInfo(lSecLvlIter))->DelayTimeInvKey);
            }
          }
        }/* If more time needed: */
        else if (lResult == DCM_E_PENDING)
        {
          /* Try it later */
        }
        else /* If something went wrong: */
        {
          Dcm_DebugAssert((lResult == DCM_E_NOT_OK), DCM_SID_GETSECURITYATTEMPTCOUNTER, DCM_E_INTERFACE_RETURN_VALUE);

          /* Mark the security level to prevent that this API is called again */
          Dcm_UtiBitOpClr(uint32, *levelMask, Dcm_UtiGetBitFromIndex(uint32, lSecLvlIter));                                                          /* SBSW_DCM_PARAM_PTR_WRITE */

          /* Set the attempt counter to uninitialized value */
          Dcm_Svc27CounterSet(lSecLvlIter, 0xFF);

          /* If delay time on failed read of attempt counter is enabled: */
          if ((Dcm_CfgWrapStateSecurityInfo(lSecLvlIter))->ReloadOnZeroDelay == TRUE)
          {
            /* If the configured delay time is zero: */
            if ((Dcm_CfgWrapStateSecurityInfo(lSecLvlIter))->ReloadTime == 0u)
            {
              /* Set the delay timer value to infinity */
              Dcm_Svc27ReloadReadFailedSet(lSecLvlIter);
              Dcm_Svc27TimerSet(lSecLvlIter, 1u);
            }
            else
            {
              /* Otherwise set the configured delay timer value */
              Dcm_Svc27TimerSet(lSecLvlIter, (Dcm_CfgWrapStateSecurityInfo(lSecLvlIter))->ReloadTime);
            }
          }
        }
      }
    }
  }
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */
# endif

# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc27ReadAttemptCounter()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc27ReadAttemptCounter(
  Dcm_TskEventContextPtrType pEventContext
  )
{
  /* Restore the attempt counter values from the application */
  Dcm_Svc27UtiGetAttemptCntr(Dcm_Svc27SingletonContext.GetAttOpStatus
                            ,&Dcm_Svc27SingletonContext.GetAttCntrEventMask);                                                                        /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

  /* If all attempt counters were read */
  if (Dcm_Svc27SingletonContext.GetAttCntrEventMask == 0u)
  {
    uint8_least lTmrIter;

    for (lTmrIter = 0; lTmrIter < DCM_SVC_27_NUM_DELAY_TIMERS; ++lTmrIter)
    {
      /* If the corresponding security timer value is not zero: */
      if (Dcm_Svc27TimerGet(lTmrIter) != 0u)
      {
        /* Start the base timer which starts the appropriate security timers */
        Dcm_TmrStartTimer(DCM_TMR_ID_SECURITY, 1u);
        break;
      }
    }
    /* Reset operation status */
    Dcm_Svc27SingletonContext.GetAttOpStatus = DCM_INITIAL;
  }
  else
  {
    /* Otherwise resume the job next task cycle */
    Dcm_TskSetLocalEvent(pEventContext->PostEv, pEventContext->Ev);                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_Svc27SingletonContext.GetAttOpStatus = DCM_PENDING;
  }
}
# endif

# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc27WriteAttemptCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_Svc27WriteAttemptCounter(
  Dcm_TskEventContextPtrType pEventContext
  )
{
  /* Notify the application about changed attempt counters */
  Dcm_Svc27UtiSetAttemptCntr(Dcm_Svc27SingletonContext.SetAttOpStatus
                            ,&Dcm_Svc27SingletonContext.SetAttCntrEventMask);                                                                        /* SBSW_DCM_POINTER_FORWARD_GLOBAL */

  /* If all attempt counters were written: */
  if (Dcm_Svc27SingletonContext.SetAttCntrEventMask == 0u)
  {
    /* Finish the job and reset the opStatus for next write cycle */
    Dcm_Svc27SingletonContext.SetAttOpStatus = DCM_INITIAL;
  }
  else
  {
    /* Otherwise resume the job next task cycle and notify application for the subsequent call via opStatus parameter */
    Dcm_TskSetLocalEvent(pEventContext->PostEv, pEventContext->Ev);                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_Svc27SingletonContext.SetAttOpStatus = DCM_PENDING;
  }
}
# endif

/**********************************************************************************************************************
 *  Dcm_Service27SeedRepeater()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Service27SeedRepeater(
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc27RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                  lResult;
  Dcm_CfgSvc27SecLevelInfoPtrType pSecLevelInfo;

  lResult = Dcm_CfgWrapSvc27SecLevelInfoGetEntry(pRepContext->SecLvlRef, &pSecLevelInfo, ErrorCode);                                                 /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lResult == DCM_E_OK)                                                                                                                           /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    lResult = Dcm_UtiMsgContextVerifySize(pMsgContext, pSecLevelInfo->SeedResLength, ErrorCode);                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */

    if (lResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
    {
      /* Read the seed of the appropriate security level */
      switch (pSecLevelInfo->GetSeedFuncClass)
      {
      case DCM_SVC27_OPCLS_SEED_WITHOUT_ADR:
        lResult = ((Dcm_Svc27OpFuncSeedWoAdr)pSecLevelInfo->GetSeedFunc)(opStatus                                                                    /* PRQA S 0313 */ /* MD_Dcm_0313 */
                                                                        ,Dcm_UtiGetResData(pMsgContext)                                              /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                                        ,ErrorCode);                                                                 /* SBSW_DCM_CALL_FUNCPTR_SVC27GETSEED */
        break;
      case DCM_SVC27_OPCLS_SEED_WITH_ADR:
        lResult = ((Dcm_Svc27OpFuncSeedWAdr)pSecLevelInfo->GetSeedFunc)(Dcm_UtiGetReqData(pMsgContext)                                               /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */
                                                                       ,opStatus
                                                                       ,Dcm_UtiGetResData(pMsgContext)                                               /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */ /* SBSW_DCM_PARAM_PTR_FORWARD */
                                                                       ,ErrorCode);                                                                  /* SBSW_DCM_CALL_FUNCPTR_SVC27GETSEED */
        break;
      default:                                                                                                                                       /* COV_DCM_RTM_UNREACHABLE X */
        DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                            /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        lResult = DCM_E_NOT_OK;
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INVALID_CONFIG);
        break;
      }

      switch (lResult)
      {
      case DCM_E_OK:
        /* Send response */
        Dcm_UtiProvideResData(pMsgContext, pSecLevelInfo->SeedResLength);                                                                            /* SBSW_DCM_PARAM_PTR_FORWARD */
        break;
      case DCM_E_PENDING:
        /* Retry later */
        break;
      case DCM_E_NOT_OK:
        Dcm_UtiHandleApplNrc(lResult, ErrorCode, DCM_E_CONDITIONSNOTCORRECT);                                                                        /* SBSW_DCM_PARAM_PTR_FORWARD */
        break;
      default:
        Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);
        *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                /* SBSW_DCM_PARAM_PTR_WRITE */
        lResult = DCM_E_NOT_OK;
        break;
      }
    }
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_Service27KeyRepeater()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Service27KeyRepeater(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc27RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType                  lResult;
  Dcm_NegativeResponseCodeType    lNrc;
  Dcm_CfgSvc27SecLevelInfoPtrType pSecLevelInfo;

  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  lResult = Dcm_CfgWrapSvc27SecLevelInfoGetEntry(pRepContext->SecLvlRef, &pSecLevelInfo, ErrorCode);                                                 /* SBSW_DCM_COMB_PTR_FORWARD */

  if (lResult == DCM_E_OK)                                                                                                                           /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    lNrc = DCM_E_POSITIVERESPONSE;

    /* Let the application compare the given key data */
    lResult = pSecLevelInfo->CompareKeyFunc(Dcm_UtiGetReqData(pMsgContext)                                                                           /* PRQA S 0313, 2985 */ /* MD_Dcm_0313, MD_Dcm_Redundant_2985 */
                                           ,opStatus
                                           ,&lNrc);                                                                                                  /* SBSW_DCM_CALL_FUNCPTR_SVC27SECLEVEL */

    switch (lResult)
    {
      /*---------------------------*/
    case DCM_E_OK:
      /*---------------------------*/
# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
      if (Dcm_Svc27CounterGet(pRepContext->SecLvlRef) != 0u)
      {
        /* Notify the application about new attempt counter */
        Dcm_RepeaterNextStep(pContext, DCM_SVC27_PROGRESS_SETATTEMPTCNTR);                                                                           /* SBSW_DCM_PARAM_PTR_FORWARD */
        lResult = DCM_E_LOOP;
      }
# endif
      /* Reset the attempt counter of this security level after notification check */
      Dcm_Svc27CounterReset(pRepContext->SecLvlRef);

      break;
      /*---------------------------*/
    case DCM_E_PENDING:
      /*---------------------------*/
      /* Try later */
      break;
      /*---------------------------*/
    case DCM_E_NOT_OK:
      /*---------------------------*/
      /* If an error occurred set the NRC given by the application */
      Dcm_UtiHandleApplNrc(lResult, &lNrc, DCM_E_CONDITIONSNOTCORRECT);                                                                              /* SBSW_DCM_POINTER_FORWARD_STACK */
      pRepContext->ErrorCode = lNrc; /* will be used in next step */                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
      /* fall through */
    case DCM_E_COMPARE_KEY_FAILED:                                                                                                                   /* PRQA S 2003 */ /* MD_Dcm_Optimize_2003 */
      /* Increment the attempt counter */
      Dcm_Svc27CounterInc(pRepContext->SecLvlRef);

      /* Notify application about new attempt counter */
      /* No prevention needed, since calling with the same counter is rare */
      Dcm_RepeaterNextStep(pContext, DCM_SVC27_PROGRESS_CHECKATTEMPTSEXCEEDED);                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
      lResult = DCM_E_LOOP;
      break;
      /*---------------------------*/
    default:
      /*---------------------------*/
      Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_INTERFACE_RETURN_VALUE);

      *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                  /* SBSW_DCM_PARAM_PTR_WRITE */
      lResult = DCM_E_NOT_OK;
      break;
    }
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_Service27CheckAttemptsExceededRepeater()
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
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Service27CheckAttemptsExceededRepeater(
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc27RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType  lResult;
# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
  uint32          lMask = Dcm_UtiGetBitFromIndex(uint32, pRepContext->SecLvlRef);
# endif

  DCM_IGNORE_UNREF_PARAM(opStatus);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
  /* Notify the application about the new attempt counter value */
  Dcm_Svc27UtiSetAttemptCntr(opStatus, &lMask);                                                                                                      /* SBSW_DCM_POINTER_FORWARD_STACK */

  /* If the setting of the attempt counter was not successful: */
  if (lMask != 0u)
  {
    /* Try it later */
    lResult = DCM_E_PENDING;
  }
  else
# endif
  {
    /* If the number of allowed attempt were exceeded: */
    if (((Dcm_CfgWrapStateSecurityInfo(pRepContext->SecLvlRef))->NumAttempts != 0u) /* Consider only levels with attempt monitoring */
      && (Dcm_Svc27CounterGet(pRepContext->SecLvlRef) >= (Dcm_CfgWrapStateSecurityInfo(pRepContext->SecLvlRef))->NumAttempts))
    {
      /* Set NRC 0x36 if not already set and start the timer */
      /* This NRC will be ignored if ErrorCode is already set due to failed compare key function */
      *ErrorCode = DCM_E_EXCEEDNUMBEROFATTEMPTS;                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */

      Dcm_Svc27TimerStart(pRepContext->SecLvlRef, (Dcm_CfgWrapStateSecurityInfo(pRepContext->SecLvlRef))->DelayTimeInvKey);
    }
    else
    {
      /* Otherwise set NRC 0x35 if not already set */
      /* This NRC will be ignored if ErrorCode is already set due to failed compare key function */
      *ErrorCode = DCM_E_INVALIDKEY;                                                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
    }

    if (pRepContext->ErrorCode != DCM_E_POSITIVERESPONSE)
    {
      /* Use previously set ErrorCode if available */
      *ErrorCode = pRepContext->ErrorCode;                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
    }

    lResult = DCM_E_NOT_OK;
  }

  return lResult;
}

# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Service27SetAttemptCntrRepeater()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_Service27SetAttemptCntrRepeater(
  Dcm_OpStatusType opStatus,
  Dcm_ReadOnlyMsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode,
  Dcm_Svc27RepeaterProxyContextPtrType pRepContext                                                                                                   /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  Std_ReturnType  lResult;
  uint32          lMask = Dcm_UtiGetBitFromIndex(uint32, pRepContext->SecLvlRef);

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* Notify the application about the new attempt counter value */
  Dcm_Svc27UtiSetAttemptCntr(opStatus, &lMask);                                                                                                      /* SBSW_DCM_POINTER_FORWARD_STACK */

  /* If the setting of the attempt counter was successful: */
  if (lMask == 0u)
  {
    /* Finish the job */
    if (pRepContext->ErrorCode == DCM_E_POSITIVERESPONSE)
    {
      lResult = DCM_E_OK;
    }
    else
    {
      *ErrorCode = pRepContext->ErrorCode;                                                                                                           /* SBSW_DCM_PARAM_PTR_WRITE */
      lResult = DCM_E_NOT_OK;
    }
  }
  else
  {
    /* Otherwise try it later */
    lResult = DCM_E_PENDING;
  }

  return lResult;
}
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_Svc27UtiSetAttemptCntr()
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
DCM_LOCAL FUNC(void, DCM_CODE) Dcm_Svc27UtiSetAttemptCntr(
  Dcm_OpStatusType opStatus,
  P2VAR(uint32, AUTOMATIC, DCM_VAR_NOINIT) levelMask
  )
{
  uint8_least lSecLvlIter;

  /* For each security level: */
  for (lSecLvlIter = 0u; lSecLvlIter < DCM_CFGSVC27SECLEVELINFO_SIZE; ++lSecLvlIter)
  {
    /* If the attempt counter for that specific security level is not already written: */
    if (Dcm_UtiBitOpTest(uint32, *levelMask, Dcm_UtiGetBitFromIndex(uint32, lSecLvlIter)))
    {
      Dcm_CfgSvc27SecLevelInfoPtrType pSecLevelInfo;
      Std_ReturnType                  lResult;
      Dcm_NegativeResponseCodeType    lNrc;

      lResult = Dcm_CfgWrapSvc27SecLevelInfoGetEntry(lSecLvlIter, &pSecLevelInfo, &lNrc);                                                            /* SBSW_DCM_COMB_PTR_FORWARD */

      if (lResult == DCM_E_OK)                                                                                                                       /* COV_DCM_RTM_RUNTIME_CHECK TX */ /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr, MD_Dcm_ConstExpr */
      {
        /* Notify the application about the new attempt counter value */
        lResult = pSecLevelInfo->SetAttemptCntrFunc(opStatus, Dcm_Svc27CounterGet(lSecLvlIter));                                                     /* SBSW_DCM_CALL_FUNCPTR_SVC27SECLEVEL */

        if (lResult == DCM_E_PENDING)
        {
          /* Otherwise try it later */
        }
        else
        {
          Dcm_DebugAssert(((lResult == DCM_E_OK) || (lResult == DCM_E_NOT_OK)), DCM_SID_SETSECURITYATTEMPTCOUNTER, DCM_E_INTERFACE_RETURN_VALUE);
          /*
          * If the attempt counter was written successfully or an error occurred, don't try to write the attempt
          * counter again.
          */
          Dcm_SplitTaskEnterCS(); /* Avoid interrupt from Dcm_OnTimeoutSecurityAccess */
          /*=================================*
          BEGIN CRITICAL SECTION
          *=================================*/
          Dcm_UtiBitOpClr(uint32                                                                                                                     /* SBSW_DCM_PARAM_PTR_WRITE */
                         ,*levelMask
                         ,Dcm_UtiGetBitFromIndex(uint32, lSecLvlIter));
          /*=================================*
          END CRITICAL SECTION
          *=================================*/
          Dcm_SplitTaskLeaveCS();
        }
      }
    }
  }
}
# endif

/**********************************************************************************************************************
 *  Dcm_Svc27ReqLengthGetter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL FUNC(Dcm_DiagSubServiceLengthType, DCM_CODE) Dcm_Svc27ReqLengthGetter(
  Dcm_DiagSubServiceRefOptType sfRef
  )
{
  /* Return sub-function specific length */
  return (Dcm_CfgWrapSvc27SubFuncInfo(sfRef))->ReqLength;
}

/**********************************************************************************************************************
 *  Dcm_Svc27SubFuncSequenceChecker()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL FUNC(Std_ReturnType, DCM_CODE) Dcm_Svc27SubFuncSequenceChecker(
  Dcm_MsgContextPtrType pMsgContext,                                                                                                                 /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_DiagSubServiceRefOptType subSvcRef,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult = DCM_E_OK;

  DCM_IGNORE_UNREF_PARAM(pMsgContext);                                                                                                               /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  if((Dcm_Svc27IsKeyRequest(subSvcRef) == TRUE)                                                                                                      /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    &&(Dcm_Svc27IsThisKeyRequestExpected(Dcm_Svc27GetSecLvlRef(subSvcRef)) == FALSE))                                                                /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
  {
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_REQUESTSEQUENCEERROR;                                                                                                         /* SBSW_DCM_PARAM_PTR_WRITE */
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
# if (DCM_TSK_NUM_SVC27 > 0u)
/**********************************************************************************************************************
 *  Dcm_Svc27Task()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc27Task(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_TskEventContextPtrType pEventContext                                                                                                           /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  )
{
  DCM_IGNORE_UNREF_PARAM(pContext);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* If the attempt counters should be restored: */
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_SVC_27_ATTEMPT_CNTR_READ))
  {
    /* Process attempt counter reading */
    Dcm_Svc27ReadAttemptCounter(pEventContext);                                                                                                      /* SBSW_DCM_PARAM_PTR_FORWARD */
  }

  /* If the attempt counters should be stored: */
  if (Dcm_TskIsLocalEventSet(pEventContext->Ev, DCM_TSK_EV_SVC_27_ATTEMPT_CNTR_WRITE))
  {
    /* Process attempt counter writing */
    Dcm_Svc27WriteAttemptCounter(pEventContext);                                                                                                     /* SBSW_DCM_PARAM_PTR_FORWARD */
  }
}
# endif

/**********************************************************************************************************************
 *  Dcm_OnTimeoutSecurityAccess()
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
 *
 *
 *
 *
 *
 *
 */
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutSecurityAccess(
  Dcm_ThreadIdMemType threadId
  )
{
  Dcm_TmrTimerCntrMemType lReloadTick = 0u;
  uint8_least             lTmrIter;
# if (DCM_STATE_SEC_RESET_ATT_CNTR_ON_TIMEOUT_ENABLED == STD_ON)
  uint32_least            lJustExpiredMask = 0u;
# endif

  DCM_IGNORE_UNREF_PARAM(threadId);                                                                                                                  /* PRQA S 3112 */ /* MD_MSR_DummyStmt */

  /* For each software timer: */
  for (lTmrIter = 0; lTmrIter < DCM_SVC_27_NUM_DELAY_TIMERS; ++lTmrIter)
  {
# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
    /* If delay time is set to infinity: */
    if ((boolean)(Dcm_Svc27ReloadReadFailedGet(lTmrIter)) == TRUE)                                                                                   /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    {
      /* Reload forever */
      lReloadTick = 1u;
    }
    else
# endif
    {
      /* Otherwise, if the software timer is active: */
      if (Dcm_Svc27TimerActive(lTmrIter) == TRUE)                                                                                                    /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      {
        /* Decrement the timer value */
        Dcm_Svc27TimerSet(lTmrIter, (Dcm_TmrTimerCntrMemType)(Dcm_Svc27TimerGet(lTmrIter) - 1u));

        /* If the software timer is still active */
        if (Dcm_Svc27TimerActive(lTmrIter) == TRUE)                                                                                                  /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
        {
          /* Reload the hardware timer */
          lReloadTick = 1u;
        }
# if (DCM_STATE_SEC_RESET_ATT_CNTR_ON_TIMEOUT_ENABLED == STD_ON)
        else
        {
          /* Otherwise, remember the index of the just expired timer */
          lJustExpiredMask |= Dcm_UtiGetBitFromIndex(uint32_least, lTmrIter);
        }
# endif
      }
    }
  }

# if (DCM_STATE_SEC_RESET_ATT_CNTR_ON_TIMEOUT_ENABLED == STD_ON)
  {
    uint8_least lSecLvlIter;

    /* For each security level */
    for (lSecLvlIter = 0; lSecLvlIter < DCM_STATE_SECURITY_NUM_LEVELS; ++lSecLvlIter)
    {
      /* If the corresponding timer of the given security level is just expired */
      if(Dcm_Svc27TimerJustExpired(lJustExpiredMask, lSecLvlIter) == TRUE)                                                                           /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
      {
        /*
         * Reset the corresponding attempt counter of the given security level (if only single counter used, it will
         * be overwritten multiple times and application will be informed for each security level).
         * If multiple counter used, only changed counters will be reset and informed.
         */
#  if (DCM_SVC_27_MULTI_ATTEMPT_COUNTER_ENABLED == STD_ON)
        if (Dcm_Svc27CounterGet(lSecLvlIter) != 0u)
#  endif
        {
          Dcm_Svc27CounterReset(lSecLvlIter);
#  if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
          Dcm_Svc27SingletonContext.SetAttCntrEventMask |= Dcm_UtiGetBitFromIndex(uint32, lSecLvlIter);
#  endif
        }
      }
    }
  }

#  if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
  /* If at least one attempt counter was reset */
  if (Dcm_Svc27SingletonContext.SetAttCntrEventMask != 0u)
  {
    /* Notify the application about changed attempt counter values */
    Dcm_TskSetEvent(DCM_TSK_ID_SVC27, DCM_TSK_EV_SVC_27_ATTEMPT_CNTR_WRITE);
  }
#  endif

# endif

  return lReloadTick;
}
/**********************************************************************************************************************
 *  Dcm_Svc27OnStateChanged()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_Svc27OnStateChanged(void)
{
  Dcm_Svc27ResetSequence();
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_Service27Init()
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
FUNC(void, DCM_CODE) Dcm_Service27Init(
  void
  )
{
  uint8_least lSecLvlIter;
  Dcm_Svc27ResetSequence();

  /* For each security level: */
  for (lSecLvlIter = 0; lSecLvlIter < DCM_STATE_SECURITY_NUM_LEVELS; ++lSecLvlIter)
  {
    /* Initialize the security timer and counter */
    Dcm_Svc27TimerReset(lSecLvlIter);
    Dcm_Svc27CounterReset(lSecLvlIter);

# if (DCM_STATE_SEC_DELAY_ON_BOOT_ENABLED == STD_ON)
    /* If a delay time on boot is supported: */
    if((Dcm_CfgWrapStateSecurityInfo(lSecLvlIter))->DelayTimeOnBoot != 0u)                                                                           /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
    {
      /* Start the appropriate security timer */
      Dcm_Svc27TimerStart(lSecLvlIter, (Dcm_CfgWrapStateSecurityInfo(lSecLvlIter))->DelayTimeOnBoot);                                                /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
    }
# endif
  }

  /* Initialize the bit masks related to Get- and Set-AttemptCounter APIs */
# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
  Dcm_Svc27SingletonContext.GetAttCntrEventMask = DCM_SVC_27_LEVEL_MASK;
  Dcm_Svc27SingletonContext.SetAttCntrEventMask = 0u;
  Dcm_Svc27SingletonContext.ReloadMask = 0u;
  Dcm_Svc27SingletonContext.GetAttOpStatus = DCM_INITIAL;
  Dcm_Svc27SingletonContext.SetAttOpStatus = DCM_INITIAL;

  /* Trigger the reading of the attempt counter values */
  Dcm_TskSetEvent(DCM_TSK_ID_SVC27, DCM_TSK_EV_SVC_27_ATTEMPT_CNTR_READ);
# endif
}

/**********************************************************************************************************************
 *  Dcm_Service27Processor()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, DCM_CALLOUT_CODE) Dcm_Service27Processor(
  Dcm_ContextPtrType pContext,
  Dcm_OpStatusType opStatus,
  Dcm_MsgContextPtrType pMsgContext,
  Dcm_NegativeResponseCodePtrType ErrorCode
  )
{
  Std_ReturnType lStdResult;

  Dcm_Svc27RepeaterProxyContextPtrType pRepContext = &pContext->Repeater.Context.Svc27;

  switch (Dcm_RepeaterGetProgress(pContext))
  {
  case DCM_REPEATER_PROGRESS_INITIAL:
    lStdResult = Dcm_Service27Handler(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                      /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC27_PROGRESS_SEEDPROCESSOR:
    lStdResult = Dcm_Service27SeedProcessor(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC27_PROGRESS_KEYPROCESSOR:
    lStdResult = Dcm_Service27KeyProcessor(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# if (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
  case DCM_SVC27_PROGRESS_SETATTEMPTCNTR:
    lStdResult = Dcm_Service27SetAttemptCntrRepeater(opStatus, pMsgContext, ErrorCode, pRepContext);                                                 /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
# endif
  case DCM_SVC27_PROGRESS_SEEDREPEATER:
    lStdResult = Dcm_Service27SeedRepeater(opStatus, pMsgContext, ErrorCode, pRepContext);                                                           /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC27_PROGRESS_KEYREPEATER:
    lStdResult = Dcm_Service27KeyRepeater(pContext, opStatus, pMsgContext, ErrorCode, pRepContext);                                                  /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  case DCM_SVC27_PROGRESS_CHECKATTEMPTSEXCEEDED:
    lStdResult = Dcm_Service27CheckAttemptsExceededRepeater(opStatus, pMsgContext, ErrorCode, pRepContext);                                          /* SBSW_DCM_COMB_PTR_FORWARD */
    break;
  default:                                                                                                                                           /* COV_DCM_RTM_UNREACHABLE X */
    lStdResult = DCM_E_NOT_OK;
    *ErrorCode = DCM_E_PANIC_NRC;                                                                                                                    /* SBSW_DCM_PARAM_PTR_WRITE */
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_ILLEGAL_STATE);
    break;
  }

  return lStdResult;
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */

/**********************************************************************************************************************
 *  Dcm_Service27PostProcessor()
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
FUNC(void, DCM_CALLOUT_CODE) Dcm_Service27PostProcessor(
  Dcm_ContextPtrType pContext,                                                                                                                       /* PRQA S 3673 */ /* MD_Dcm_Design_3673 */
  Dcm_ConfirmationStatusType status
  )
{
  Dcm_Svc27RepeaterProxyContextConstPtrType pRepContext = &pContext->Repeater.Context.Svc27;

  /* If a positive response was successfully sent */
  if (status == DCM_RES_POS_OK)
  {
    /* If it was a compare key request: */
    if (Dcm_Svc27IsKeyRequest(pRepContext->SubSvcRef) == TRUE)                                                                                       /* PRQA S 4304 */ /* MD_MSR_AutosarBoolean */
    {
      /* Apply the new security state and reset sequence */
      Dcm_StateSetSecurity((Dcm_StateIndexOptType)(Dcm_Svc27GetSecLvlRef(pRepContext->SubSvcRef)) + 1u); /* locked = 0x00 -> level 1 = 0x01, etc */

      /* Set IdsM security event */
      Dcm_DebugReportSecurityEvent(pContext->ThreadId, DCM_DEBUG_SEV_ECU_UNLOCK_SUCCESSFUL);
    }
    else
    {
      /* If this was no zero seed response: */
      if (pRepContext->IsZeroSeed == FALSE)
      {
        /* Prepare for key acceptance */
        Dcm_Svc27SingletonContext.SeedLevel = (Dcm_CfgWrapStateSecurityInfo((Dcm_Svc27GetSecLvlRef(pRepContext->SubSvcRef))))->Value;
      }
      else
      {
        /* Otherwise, there is no key expected in that case after zero seed */
        Dcm_Svc27ResetSequence();
      }
    }
  }
  else if ((DCM_DIAG_RES_NEG_ANY & status) != 0u) /* Otherwise, if any negative response was sent: */
  {
    /* Reset the key-seed-sequence */
    Dcm_Svc27ResetSequence();
  }
  else /* POS_NOK */
  {
    /*
     * Otherwise no change required:
     *   - on SeedReq: the client shall retry the seed request or send another one
     *                 -> do not move to expect key for a certain level
     *   - on KeyReq:  the client may retry the key request to get a response
     *                 -> do not invalidate expect key for this level
     */
  }
}
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (DCM_SVC_27_SUPPORT_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Svc27.c
 *********************************************************************************************************************/
