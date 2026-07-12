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
/**        \file  Dcm_Tmr.h
 *         \unit  Tmr
 *        \brief  Contains public inline function definitions of Timer Manager unit.
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
#if !defined(DCM_TMR_H)
# define DCM_TMR_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_TmrTypes.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Calculation of all system timer IDs.
  Note: The same order defined here must be kept during the initialization of the table Dcm_TmrTimerInfo */
# define DCM_TMR_ID_P2                                               ((Dcm_TmrTimerIdMemType)(0u + 0u))                                                                 /*!< The timer ID of the P2 timer */
# define DCM_TMR_ID_S3                                               ((Dcm_TmrTimerIdMemType)(DCM_TMR_ID_P2 + DCM_TMR_NUM_P2))                                          /*!< The timer ID of the S3 timer */
# define DCM_TMR_ID_FBLRES                                           ((Dcm_TmrTimerIdMemType)(DCM_TMR_ID_S3 + DCM_TMR_NUM_S3))                                          /*!< The timer ID of the ComM going to FullCommMode after ECU power-on/reset timer */
# define DCM_TMR_ID_SECURITY                                         ((Dcm_TmrTimerIdMemType)(DCM_TMR_ID_FBLRES + DCM_TMR_NUM_FBLRES))                                  /*!< The timer ID of the diagnostic service 0x27 failed attempt delay base timer */
# define DCM_TMR_ID_PERIODIC_TX                                      ((Dcm_TmrTimerIdMemType)(DCM_TMR_ID_SECURITY + DCM_TMR_NUM_SECURITY))                              /*!< The timer ID of the periodic message transmission base timer */
# define DCM_TMR_ID_SVC2A_SCHEDULER                                  ((Dcm_TmrTimerIdMemType)(DCM_TMR_ID_PERIODIC_TX + DCM_TMR_NUM_PERIODIC_TX))                        /*!< The timer ID of the diagnostic service 0x2A periodic DID scheduler base timer */
# define DCM_TMR_ID_SVC86_SCHEDULER                                  ((Dcm_TmrTimerIdMemType)(DCM_TMR_ID_SVC2A_SCHEDULER + DCM_TMR_NUM_SVC2A_SCHEDULER))                /*!< The timer ID of the diagnostic service 0x86 intermessage timer */
# define DCM_TMR_ID_KEEP_ALIVE                                       ((Dcm_TmrTimerIdMemType)(DCM_TMR_ID_SVC86_SCHEDULER + DCM_TMR_NUM_SVC86_SCHEDULER))  /*!< The timer ID of the extended keep-alive timer */
# define DCM_TMR_ID_AUTHENTICATION                                   ((Dcm_TmrTimerIdMemType)(DCM_TMR_ID_KEEP_ALIVE + DCM_TMR_NUM_KEEP_ALIVE))                          /*!< The timer ID of the authentication manager connection session base timer */

/*
 * Definition of the total number of system timers
 */
/*! A single timer for P2 monitoring is always needed and thread specific */
# define DCM_TMR_NUM_P2                                              1u

/*! A single timer for S3 monitoring is needed */
# define DCM_TMR_NUM_S3                                              1u

/*! A single timer for monitoring the time between ECU start up and ComM going into FullComMode is needed */
# if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
#  define DCM_TMR_NUM_FBLRES                                         1u
# else
#  define DCM_TMR_NUM_FBLRES                                         0u /*!< Just a bridge to accomplish the amount of timers calculation */
# endif

/*! A single timer for monitoring all security access delay times is needed */
# if (DCM_SVC_27_SUPPORT_ENABLED == STD_ON)
#  define DCM_TMR_NUM_SECURITY                                       1u
# else
#  define DCM_TMR_NUM_SECURITY                                       0u
# endif

/*! A single timer for monitoring all periodic message transmission times is needed */
# if (DCM_NET_PERIODIC_TX_ENABLED == STD_ON)
#  define DCM_TMR_NUM_PERIODIC_TX                                    1u
# else
#  define DCM_TMR_NUM_PERIODIC_TX                                    0u /*!< Just a bridge to accomplish the amount of timers calculation */
# endif

/*! A single timer for monitoring periodic DID scheduler timers is needed */
# if (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON) && \
     (DCM_SPLIT_TASKS_ENABLED    == STD_ON)
#  define DCM_TMR_NUM_SVC2A_SCHEDULER                                1u
# else
#  define DCM_TMR_NUM_SVC2A_SCHEDULER                                0u /*!< Just a bridge to accomplish the amount of timers calculation */
# endif

/*! A single timer for managing the RoE transmission intermessage time is needed */
# if (DCM_SVC_86_TIMER_ENABLED == STD_ON)
#  define DCM_TMR_NUM_SVC86_SCHEDULER                                1u
# else
#  define DCM_TMR_NUM_SVC86_SCHEDULER                                0u /*!< Just a bridge to accomplish the amount of timers calculation */
# endif

/*! A single timer for monitoring the extended keep-alive delay time is needed */
# if (DCM_NET_KEEP_ALIVE_TIME_ENABLED == STD_ON)
#  define DCM_TMR_NUM_KEEP_ALIVE                                     1u
# else
#  define DCM_TMR_NUM_KEEP_ALIVE                                     0u /*!< Just a bridge to accomplish the amount of timers calculation */
# endif

/*! A single timer for monitoring all authentication session time is needed */
# if (DCM_AUTHMGR_DEFAULT_SESSION_TIMEOUT_ENABLED == STD_ON)
#  define DCM_TMR_NUM_AUTHENTICATION                                 1u
# else
#  define DCM_TMR_NUM_AUTHENTICATION                                 0u /*!< Just a bridge to accomplish the amount of timers calculation */
# endif

/*! Pre-compile total number of system timers calculation.
    Note: Do not cast the sum, since it is used within a pre-processor arithmetic expression (refer to Dcm_TmrMaskMemType definition section) */
# define DCM_TMR_NUM_TIMERS                                          ( DCM_TMR_NUM_P2 \
                                                                     + DCM_TMR_NUM_S3 \
                                                                     + DCM_TMR_NUM_FBLRES \
                                                                     + DCM_TMR_NUM_SECURITY \
                                                                     + DCM_TMR_NUM_PERIODIC_TX \
                                                                     + DCM_TMR_NUM_SVC2A_SCHEDULER \
                                                                     + DCM_TMR_NUM_SVC86_SCHEDULER \
                                                                     + DCM_TMR_NUM_KEEP_ALIVE \
                                                                     + DCM_TMR_NUM_AUTHENTICATION)

/*! The casted version of the DCM_TMR_NUM_TIMERS within the C-code */
# define DCM_TMR_NUM_TIMERS_CASTED                                   ((Dcm_TmrTimerIdMemType) DCM_TMR_NUM_TIMERS)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# define Dcm_TmrSetTimer(timerId, timerTicks)                        (Dcm_TmrSetTimerByThread((timerId), (timerTicks), DCM_THREAD_ID_BASIC))         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_TmrStartTimer(timerId, timerTicks)                      (Dcm_TmrStartTimerByThread((timerId), (timerTicks), DCM_THREAD_ID_BASIC))       /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_TmrStopTimer(timerId)                                   (Dcm_TmrStopTimerByThread((timerId), DCM_THREAD_ID_BASIC))                      /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_TmrIsTimerRunning(timerId)                              (Dcm_TmrIsTimerRunningByThread((timerId), DCM_THREAD_ID_BASIC))                 /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
# define Dcm_TmrResumeTimer(timerId)                                 (Dcm_TmrResumeTimerByThread((timerId), DCM_THREAD_ID_BASIC))                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Base data type for system timer handles  (no more than 32 timers supported for now: see Dcm_TmrMaskMemType definition section) */
typedef uint8        Dcm_TmrTimerIdMemType;
typedef uint8_least  Dcm_TmrTimerIdOptType;

/*! Single timer runtime data */
struct DCM_TMRTIMERCONTEXTTYPE_TAG
{
  volatile Dcm_TmrTimerCntrMemType  Timer;         /*!< Remaining time of all running system timers */
           Dcm_TmrTimerIdMemType    TimerInfoRef;  /*!< Reference to Dcm_TmrTimerInfo[] */
};
typedef struct DCM_TMRTIMERCONTEXTTYPE_TAG Dcm_TmrTimerContextType;
typedef P2VAR(Dcm_TmrTimerContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_TmrTimerContextPtrType;
typedef P2CONST(Dcm_TmrTimerContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_TmrTimerContextConstPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_TmrSetTimerByThread()
 *********************************************************************************************************************/
/*! \brief          Sets the value of a timer of the given index.
 *  \details        -
 *  \param[in]      timerId      The timer ID
 *  \param[in]      timeTicks    The concrete time to elapse
 *  \param[in]      threadId     The thread ID
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Called only within a critical section
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TmrSetTimerByThread(
  Dcm_TmrTimerIdOptType timerId,
  Dcm_TmrTimerCntrMemType timeTicks,
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_TmrStartTimerByThread()
 *********************************************************************************************************************/
/*! \brief          Starts a timer with a given time value.
 *  \details        Starts a timer with the given timer ID with the given time value.
 *  \param[in]      timerId      The timer ID
 *  \param[in]      timeTicks    The concrete time to elapse
 *  \param[in]      threadId     The thread ID
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TmrStartTimerByThread(
  Dcm_TmrTimerIdOptType timerId,
  Dcm_TmrTimerCntrMemType timeTicks,
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_TmrStopTimerByThread()
 *********************************************************************************************************************/
/*! \brief          Stops a timer.
 *  \details        Stops a timer with the given timer ID.
 *  \param[in]      timerId    The timer ID
 *  \param[in]      threadId     The thread ID
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TmrStopTimerByThread(
  Dcm_TmrTimerIdOptType timerId,
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_TmrIsTimerRunningByThread()
 *********************************************************************************************************************/
/*! \brief          Checks whether the timer with the given ID is running.
 *  \details        -
 *  \param[in]      timerId      The timer ID
 *  \param[in]      threadId     The thread ID
 *  \return         TRUE         The timer is running
 *  \return         FALSE        The timer is not running
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(boolean, DCM_CODE) Dcm_TmrIsTimerRunningByThread(
  Dcm_TmrTimerIdOptType timerId,
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_TmrResumeTimerByThread()
 *********************************************************************************************************************/
/*! \brief          Resumes a timer.
 *  \details        Resumes a timer with the given timer ID.
 *  \param[in]      timerId    The timer ID
 *  \param[in]      threadId     The thread ID
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TmrResumeTimerByThread(
  Dcm_TmrTimerIdOptType timerId,
  Dcm_ThreadIdOptType threadId
  );

# if (DCM_TMR_NUM_SVC86_SCHEDULER > 0u)
/**********************************************************************************************************************
 *  Dcm_ExtOnTimeoutSvc86Scheduler()
 *********************************************************************************************************************/
/*! \brief          Service 0x86 timer function to trigger sampling for onDTCStatusChange events.
 *  \details        -
 *  \param[in]      threadId  Active thread
 *  \return         Number of reload ticks
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_ExtOnTimeoutSvc86Scheduler(
  Dcm_ThreadIdMemType threadId
  );
# endif

/**********************************************************************************************************************
 *  Dcm_TmrInit()
 *********************************************************************************************************************/
/*! \brief          Timer initialization.
 *  \details        Initialization of the timer manager sub-module.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TmrInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_OnTimeoutP2()
 *********************************************************************************************************************/
/*! \brief          Timeout notification function.
 *  \details        This function will be called when a P2 timeout occurs.
 *  \param[in]      threadId  Active thread
 *  \return         Returns always 0 -> no reload
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutP2(
  Dcm_ThreadIdMemType threadId
  );

/**********************************************************************************************************************
 *  Dcm_OnTimeoutS3()
 *********************************************************************************************************************/
/*! \brief          Timeout notification function.
 *  \details        This function will be called when a S3 timeout occurs.
 *  \param[in]      threadId  Active thread
 *  \return         Returns always 0 -> no reload
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutS3(
  Dcm_ThreadIdMemType threadId
  );

# if (DCM_TMR_NUM_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutFblRes()
 *********************************************************************************************************************/
/*! \brief          Timeout notification function.
 *  \details        This function will be called when the P2* time expires after FBL-to-APPL transition.
 *  \param[in]      threadId  Active thread
 *  \return         Returns always 0 -> no reload
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutFblRes(
  Dcm_ThreadIdMemType threadId
  );
# endif

# if (DCM_SVC_27_SUPPORT_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutSecurityAccess()
 *********************************************************************************************************************/
/*! \brief          Timeout notification function.
 *  \details        This function will be called when a security access timeout occurs.
 *  \param[in]      threadId  Active thread
 *  \return         Number of reload ticks
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutSecurityAccess(
  Dcm_ThreadIdMemType threadId
  );
# endif

# if (DCM_TMR_NUM_PERIODIC_TX > 0u)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutPeriodicTx()
 *********************************************************************************************************************/
/*! \brief          Timeout notification function.
 *  \details        The periodic transmission timeout is reached
 *  \param[in]      threadId  Active thread
 *  \return         Number of reload ticks
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutPeriodicTx(
  Dcm_ThreadIdMemType threadId
  );
# endif

# if (DCM_TMR_NUM_SVC2A_SCHEDULER > 0u)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutSvc2AScheduler()
 *********************************************************************************************************************/
/*! \brief          Timeout notification function.
 *  \details        This function will be called when a service 0x2A scheduler timeout occurs.
 *  \param[in]      threadId  Active thread
 *  \return         Number of reload ticks
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutSvc2AScheduler(
  Dcm_ThreadIdMemType threadId
  );
# endif

# if (DCM_TMR_NUM_KEEP_ALIVE > 0u)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutKeepAlive()
 *********************************************************************************************************************/
/*! \brief          Timeout notification function.
 *  \details        This function will be called when a keep alive timeout occurs.
 *  \param[in]      threadId  Active thread
 *  \return         Number of reload ticks
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutKeepAlive(
  Dcm_ThreadIdMemType threadId
  );
# endif

# if (DCM_TMR_NUM_AUTHENTICATION > 0u)
/**********************************************************************************************************************
 *  Dcm_OnTimeoutAuthentication()
 *********************************************************************************************************************/
/*! \brief          Timeout notification function.
 *  \details        This function will be called when a authentication timeout occurs.
 *  \return         Number of reload ticks
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_TmrTimerCntrMemType, DCM_CODE) Dcm_OnTimeoutAuthentication(
  Dcm_ThreadIdMemType threadId
  );
# endif
# define DCM_STOP_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL DATA DECLARATIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#endif /* !defined(DCM_TMR_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Tmr.h
 *********************************************************************************************************************/
