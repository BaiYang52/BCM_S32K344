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
/**        \file  Dcm_Tsk.h
 *         \unit  Tsk
 *        \brief  Contains public inline function definitions of Task Manager unit.
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
#if !defined(DCM_TSK_H)
# define DCM_TSK_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "Dcm_TskTypes.h"
# include "Dcm_Uti.h"
# include "Dcm_Int.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
# if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
#  define DCM_TSK_TASK_COMPLEX_KILL_ENABLED                          STD_ON
# else
#  define DCM_TSK_TASK_COMPLEX_KILL_ENABLED                          STD_OFF
# endif

/*! Task priorities */
# if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
#  define DCM_TSK_PRIO_NONE                                          /* must not be used */       /*!< Always use a concrete priority! */
#  define DCM_TSK_PRIO_LOW                                           ((Dcm_TskTaskPrioMemType)0)  /*!< Low priority - can be interrupted by higher priority tasks and ISRs */
#  define DCM_TSK_PRIO_HIGH                                          ((Dcm_TskTaskPrioMemType)1)  /*!< High priority - cannot be interrupted by any other tasks, only ISRs */
#  define DCM_TSK_NUM_PRIOS                                          ((Dcm_TskTaskPrioMemType)2)  /*!< The number of different task priorities */
# else
#  define DCM_TSK_PRIO_NONE                                          ((Dcm_TskTaskPrioMemType)0)  /*!< Default priority - to be used only in case no task-splitting is used */
#  define DCM_TSK_PRIO_LOW                                           DCM_TSK_PRIO_NONE            /*!< Low priority - can be interrupted by ISRs */
#  define DCM_TSK_PRIO_HIGH                                          DCM_TSK_PRIO_NONE            /*!< High priority - shares the same priority as low */
#  define DCM_TSK_NUM_PRIOS                                          ((Dcm_TskTaskPrioMemType)1)  /*!< The number of different task priorities */
# endif

/*
 * Definition and calculation the total number of system tasks
 */
/*!< A single timer task (Dcm_TmrTaskTimer()) is always needed (manages all system timers) */
# define DCM_TSK_NUM_TIMER                                           1u

/*!< A network tasks for managing the message transmission is always available */
# define DCM_TSK_NUM_NET_TX                                          1u

/*!< A diagnostic tasks for managing the response message preparation is always available */
# define DCM_TSK_NUM_DIAG_TX                                         1u

/*!< A message reception network task (Dcm_NetTaskRx) is needed (i.e. for message prioritization before starting its processing) */
# define DCM_TSK_NUM_NET_RX                                          1u

/*! Dcm_ExtSvc86Task() configuration */
# if (DCM_SVC_86_SUPPORT_ENABLED == STD_ON)
#  if defined (DCM_EXT_TSK_NUM_ROE)
#   define DCM_TSK_NUM_ROE                                           DCM_EXT_TSK_NUM_ROE       /*!< Inherits the Diagnostic service 0x86 needs for a dedicated task (Dcm_ExtSvc86Task()) for additional jobs to be executed out of service execution context */
#   if defined (DCM_EXT_TSK_ATR_ROE_EXEC)
#    define DCM_TSK_ATR_ROE_EXEC                                     DCM_EXT_TSK_ATR_ROE_EXEC  /*!< Inherits the attributes of the external RoE task */
#   else
#    error "Missing DCM extension value: DCM_EXT_TSK_ATR_ROE_EXEC!"
#   endif
#  else
#   error "Missing DCM extension value: DCM_EXT_TSK_NUM_ROE!"
#  endif
# else
#  define DCM_TSK_NUM_ROE                                            0u      /*!< Just a bridge to accomplish the amount of tasks calculation */
# endif /* (DCM_SVC_86_SUPPORT_ENABLED == STD_ON) */

/*! The paged-buffer task (Dcm_PagedBufferTask()) is required */
# if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
#  define DCM_TSK_NUM_PGDBUF                                         1u
# else
#  define DCM_TSK_NUM_PGDBUF                                         0u  /*!< Just a bridge to accomplish the amount of tasks calculation */
# endif

/*! A diagnostic tasks (Dcm_DiagTaskWorker()) for diagnostic service processing is always available */
# define DCM_TSK_NUM_DIAG_WORK                                       1u

/*! Diagnostic service 0x27 needs dedicated task (Dcm_Svc27Task()) for additional jobs to be executed out of service execution context */
# if (DCM_SVC_27_SUPPORT_ENABLED == STD_ON) && \
     (DCM_STATE_SEC_ATT_CNTR_EXT_STORAGE_ENABLED == STD_ON)
#  define DCM_TSK_NUM_SVC27                                          1u
# else
#  define DCM_TSK_NUM_SVC27                                          0u  /*!< Just a bridge to accomplish the amount of tasks calculation */
# endif

/*! One task (Dcm_DiagTaskFblRes()) for the final response after an FBL to APPL and vice versa transition */
# if (DCM_DIAG_JUMPFROMFBL_ENABLED == STD_ON)
#  define DCM_TSK_NUM_DIAG_FBLRES                                    1u  /*!< One task (Dcm_DiagTaskFblRes()) for the final response after an FBL to APPL and vice versa transition */
# else
#  define DCM_TSK_NUM_DIAG_FBLRES                                    0u  /*!< Just a bridge to accomplish the amount of tasks calculation */
# endif

/*! One task (Dcm_ModeTaskMonitor()) for the background monitoring of mode changes that shall reset the ECU to its default state */
# if (DCM_MODEMGR_MONITORING_ENABLED == STD_ON)
#  define DCM_TSK_NUM_MODE_MONITOR                                   1u
# else
#  define DCM_TSK_NUM_MODE_MONITOR                                   0u  /*!< Just a bridge to accomplish the amount of tasks calculation */
# endif

/*! Diagnostic service 0x2A needs dedicated task (Dcm_Svc2ATaskSchdProcess()) for additional jobs to be executed out of service execution context */
# if (DCM_SVC_2A_SUPPORT_ENABLED == STD_ON)
#  define DCM_TSK_NUM_SVC2A_SCHEDULER                                1u
# else
#  define DCM_TSK_NUM_SVC2A_SCHEDULER                                0u  /*!< Just a bridge to accomplish the amount of tasks calculation */
# endif

/*! One task (Dcm_DiagTaskRx()) for the diagnostic message reception synchronization is always required */
# define DCM_TSK_NUM_DIAG_RX                                         1u

/*! One task (Dcm_ModeTaskMonitor()) for the background monitoring of diagnostic requests to other ECUs */
# if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
#  define DCM_TSK_NUM_NET_TA_MONITOR                                 1u
# else
#  define DCM_TSK_NUM_NET_TA_MONITOR                                 0u  /*!< Just a bridge to accomplish the amount of tasks calculation */
# endif

/*! One task (Dcm_DiagTaskRecovery()) for the full diagnostic state recovery after ECU reset /power down */
# if (DCM_DIAG_STATE_RECOVERY_ENABLED == STD_ON)
#  define DCM_TSK_NUM_DIAG_RECOVERY                                  1u
# else
#  define DCM_TSK_NUM_DIAG_RECOVERY                                  0u  /*!< Just a bridge to accomplish the amount of tasks calculation */
# endif

/*! One task (Dcm_DiagTaskGarbageCollector()) for Releasing all obsolete transport objects after prioritization */
# if (DCM_DIAG_GARB_COL_ENABLED == STD_ON)
#  define DCM_TSK_NUM_DIAG_GARB_COL                                  1u
# else
#  define DCM_TSK_NUM_DIAG_GARB_COL                                  0u /* just a bridge */
# endif

/*! Calculation of all system tasks IDs.
  Note: The order is important since it determines the order of task activations in the corresponding scheduler! Lowest Id will be executed first.
        The same order defined here must be kept during the initialization of the table Dcm_TskTaskInfo */
# define DCM_TSK_ID_DIAG_RECOVERY                                    ((Dcm_TskTaskIdMemType)(0u + 0u))                                                  /*!< The task ID of the Dcm_DiagTaskRecovery() task */
# define DCM_TSK_ID_NET_TA_MONITOR                                   ((Dcm_TskTaskIdMemType)(DCM_TSK_ID_DIAG_RECOVERY + DCM_TSK_NUM_DIAG_RECOVERY))     /*!< The task ID of the Dcm_NetTaskTaMonitor() task */
# define DCM_TSK_ID_NET_RX                                           ((Dcm_TskTaskIdMemType)(DCM_TSK_ID_NET_TA_MONITOR + DCM_TSK_NUM_NET_TA_MONITOR))   /*!< The task ID of the Dcm_NetTaskRx task() */
# define DCM_TSK_ID_TIMER                                            ((Dcm_TskTaskIdMemType)(DCM_TSK_ID_NET_RX + DCM_TSK_NUM_NET_RX))                   /*!< The task ID of the Dcm_TmrTaskTimer() task */
# define DCM_TSK_ID_DIAG_RX                                          ((Dcm_TskTaskIdMemType)(DCM_TSK_ID_TIMER + DCM_TSK_NUM_TIMER))                     /*!< The task ID of the Dcm_DiagTaskRx() task */
# define DCM_TSK_ID_MODE_MONITOR                                     ((Dcm_TskTaskIdMemType)(DCM_TSK_ID_DIAG_RX + DCM_TSK_NUM_DIAG_RX))                 /*!< The task ID of the Dcm_ModeTaskMonitor() task */
# define DCM_TSK_ID_DIAG_FBLRES                                      ((Dcm_TskTaskIdMemType)(DCM_TSK_ID_MODE_MONITOR + DCM_TSK_NUM_MODE_MONITOR))       /*!< The task ID of the Dcm_DiagTaskFblRes() task */
# define DCM_TSK_ID_SVC27                                            ((Dcm_TskTaskIdMemType)(DCM_TSK_ID_DIAG_FBLRES + DCM_TSK_NUM_DIAG_FBLRES))         /*!< The task ID of the Dcm_Svc27Task() task */
# define DCM_TSK_ID_DIAG_WORK                                        ((Dcm_TskTaskIdMemType)(DCM_TSK_ID_SVC27 + DCM_TSK_NUM_SVC27))                     /*!< The task ID of the Dcm_DiagTaskWorker() task */
# define DCM_TSK_ID_PGDBUF                                           ((Dcm_TskTaskIdMemType)(DCM_TSK_ID_DIAG_WORK + DCM_TSK_NUM_DIAG_WORK))             /*!< The task ID of the Dcm_PagedBufferTask() task */
# define DCM_TSK_ID_ROE                                              ((Dcm_TskTaskIdMemType)(DCM_TSK_ID_PGDBUF + DCM_TSK_NUM_PGDBUF))                   /*!< The task ID of the Dcm_ExtSvc86Task() task */
# define DCM_TSK_ID_DIAG_GARB_COL                                    ((Dcm_TskTaskIdMemType)(DCM_TSK_ID_ROE + DCM_TSK_NUM_ROE))                         /*!< The task ID of the Dcm_DiagTaskGarbageCollector() task */
# define DCM_TSK_ID_DIAG_TX                                          ((Dcm_TskTaskIdMemType)(DCM_TSK_ID_DIAG_GARB_COL + DCM_TSK_NUM_DIAG_GARB_COL))     /*!< The task ID of the Dcm_DiagTaskTx() task */
# define DCM_TSK_ID_SVC2A_SCHEDULER                                  ((Dcm_TskTaskIdMemType)(DCM_TSK_ID_DIAG_TX + DCM_TSK_NUM_DIAG_TX))                 /*!< The task ID of the Dcm_Svc2ATaskSchdProcess() task */
# define DCM_TSK_ID_NET_TX                                           ((Dcm_TskTaskIdMemType)(DCM_TSK_ID_SVC2A_SCHEDULER + DCM_TSK_NUM_SVC2A_SCHEDULER)) /*!< The task ID of the Dcm_NetTaskTx() task */

/*
 * Definition of all sub-components' tasks related events
 */

/*! Events of the Dcm_TmrTaskTimer() */
# define DCM_TSK_EV_TIMER_ACTIVE                                     ((Dcm_TskTaskEvMemType)0x01u)  /*!< Activate the task (on any timer activation and as long as a timer is running) */

/*! Events of the Dcm_DiagTaskRx() */
# define DCM_TSK_EV_DIAG_RX_TMR_P2_TO                                ((Dcm_TskTaskEvMemType)0x01u)  /*!< Time-out of the P2 timer occurred */
# define DCM_TSK_EV_DIAG_RX_NEW_REQ                                  ((Dcm_TskTaskEvMemType)0x02u)  /*!< New request has been received and is ready for processing */

/*! Events of the Dcm_DiagTaskWorker() */
# define DCM_TSK_EV_DIAG_WORK_TX_END_FINAL                           ((Dcm_TskTaskEvMemType)0x0001u)  /*!< Current diagnostic service processing just finished */
# define DCM_TSK_EV_DIAG_WORK_TX_END_RCRRP                           ((Dcm_TskTaskEvMemType)0x0002u)  /*!< A RCR-RP response transmission just finished */
# define DCM_TSK_EV_DIAG_WORK_EXT_SET2DEF                            ((Dcm_TskTaskEvMemType)0x0004u)  /*!< An external request for entering default-session (via Dcm_ResetToDefaultSession()) */
# define DCM_TSK_EV_DIAG_WORK_INT_SET2DEF                            ((Dcm_TskTaskEvMemType)0x0008u)  /*!< An internal request for entering default session (e.g. on protocol preemption) */
# define DCM_TSK_EV_DIAG_WORK_REPEAT                                 ((Dcm_TskTaskEvMemType)0x0010u)  /*!< A diagnostic request operation needs to be (re-)executed */
# define DCM_TSK_EV_DIAG_WORK_NEW_REQ                                ((Dcm_TskTaskEvMemType)0x0020u)  /*!< A new diagnostic request is to be processed */
# define DCM_TSK_EV_DIAG_WORK_CANCEL_OP                              ((Dcm_TskTaskEvMemType)0x0040u)  /*!< A background diagnostic request operation (e.g. PagedBuffer processing) cancelation */
# define DCM_TSK_EV_DIAG_WORK_EXT_SETSECBYPASS                       ((Dcm_TskTaskEvMemType)0x0080u)  /*!< An external request for enabling security bypass (via Dcm_SetSecurityBypass()) */
# define DCM_TSK_EV_DIAG_WORK_KILL_JOBS                              ((Dcm_TskTaskEvMemType)0x0100u)  /*!< Any diagnostic service processing currently in progress has to be terminated */
# define DCM_TSK_EV_DIAG_WORK_IDLE_CONN                              ((Dcm_TskTaskEvMemType)0x0200u)  /*!< A diagnostic client is idle and shall be deauthenticated */
# define DCM_TSK_EV_DIAG_WORK_EXT_SETROLE                            ((Dcm_TskTaskEvMemType)0x0400u)  /*!< An external request for setting the deauthenticated role (via Dcm_SetDeauthenticatedRole()) */

/*! Events of the Dcm_Svc27Task() */
# define DCM_TSK_EV_SVC_27_ATTEMPT_CNTR_READ                         ((Dcm_TskTaskEvMemType)0x01u)  /*!< Initiate attempt counter reading */
# define DCM_TSK_EV_SVC_27_ATTEMPT_CNTR_WRITE                        ((Dcm_TskTaskEvMemType)0x02u)  /*!< Initiate attempt counter writing */

/*! Events of the Dcm_PagedBufferTask() */
# if (DCM_TSK_NUM_PGDBUF > 0u)
#  define DCM_TSK_EV_PGDBUF_UPDATE_PAGE                              ((Dcm_TskTaskEvMemType)0x01u)  /*!< Request of next data portion of the paged-buffer response */
# endif

/*! Events of the Dcm_DiagTaskRecovery() */
# if (DCM_TSK_NUM_DIAG_RECOVERY > 0u)
#  define DCM_TSK_EV_DIAG_RECOVERY_GETSTATE                          ((Dcm_TskTaskEvMemType)0x01u)  /*!< Initiate recovery state retrieval */
#  define DCM_TSK_EV_DIAG_RECOVERY_PROCESS                           ((Dcm_TskTaskEvMemType)0x02u)  /*!< Processing of recovery info */
#  define DCM_TSK_EV_DIAG_RECOVERY_FINISH                            ((Dcm_TskTaskEvMemType)0x04u)  /*!< Gathering recovery information finished */
# endif

/*! Events of the Dcm_DiagTaskFblRes() */
# if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
#  define DCM_TSK_EV_DIAG_FBLRES_SEND                                ((Dcm_TskTaskEvMemType)0x01u)  /*!< Initiate FBL/APPL final response activity processing */
#  define DCM_TSK_EV_DIAG_FBLRES_CANCEL                              ((Dcm_TskTaskEvMemType)0x02u)  /*!< Cancels any further final response processing */
#  define DCM_TSK_EV_DIAG_FBLRES_WAITTXCOMM                          ((Dcm_TskTaskEvMemType)0x04u)  /*!< Waiting for ComM feedback */
#  define DCM_TSK_EV_DIAG_FBLRES_RSTFLAGS                            ((Dcm_TskTaskEvMemType)0x08u)  /*!< Reset the relevant flags to avoid re-sending of the response next boot */
#  define DCM_TSK_EV_DIAG_FBLRES_TXCONF                              ((Dcm_TskTaskEvMemType)0x10u)  /*!< Final response is sent */
# endif

/*! Events of the Dcm_DiagTaskTx() */
# define DCM_TSK_EV_DIAG_TX_SEND_LINEAR                              ((Dcm_TskTaskEvMemType)0x01u)  /*!< Initiate final positive response using linear buffer data provision */
# define DCM_TSK_EV_DIAG_TX_SEND_PAGED                               ((Dcm_TskTaskEvMemType)0x02u)  /*!< Initiate final positive response using paged-buffer data provision */
# define DCM_TSK_EV_DIAG_TX_SEND_RCRRP                               ((Dcm_TskTaskEvMemType)0x04u)  /*!< Initiate RCR-RP response transmission */

/*! Events of the Dcm_NetTaskTx() */
# define DCM_TSK_EV_NET_TX_SEND_USDT                                 ((Dcm_TskTaskEvMemType)0x01u)  /*!< Activate the task by notifying at least one USDT response transmission request is available */
# define DCM_TSK_EV_NET_TX_SEND_UUDT                                 ((Dcm_TskTaskEvMemType)0x02u)  /*!< Activate the task by notifying at least one UUDT response transmission request is available */

/* DiagGarbCol Task - identical  to Dcm_NetRxTask() */
# define DCM_TSK_EV_DIAG_GARB_COL_MASK                               DCM_TSK_EV_NET_RX_MASK

/*! Events of the Dcm_ModeTaskMonitor() */
# if (DCM_TSK_NUM_MODE_MONITOR > 0u)
#  define DCM_TSK_EV_MODE_MON_SVC85                                  ((Dcm_TskTaskEvMemType)0x01u)  /*!< Initiate monitoring on the auto-recovery mode rule for diagnostic service 0x85 */
#  define DCM_TSK_EV_MODE_MON_SVC28                                  ((Dcm_TskTaskEvMemType)0x02u)  /*!< Initiate monitoring on the auto-recovery mode rule for diagnostic service 0x28 */
# endif

/*! Events of the Dcm_Svc2ATaskSchdProcess() */
# if (DCM_TSK_NUM_SVC2A_SCHEDULER > 0u)
#  define DCM_TSK_EV_SVC2A_SCHEDULER_PROCESS                         ((Dcm_TskTaskEvMemType)0x01u)  /*!< At lest one periodic DID timed out and shall be processed */
#  define DCM_TSK_EV_SVC2A_SCHEDULER_TMR_UPD                         ((Dcm_TskTaskEvMemType)0x02u)  /*!< All periodic DID timers shall be updated by one tick */
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
/*! Tests whether at least one event specified by ev is set in task local register evReg */
# define Dcm_TskIsLocalEventSet(evReg, ev)                           (Dcm_UtiBitOpTest(Dcm_TskTaskEvMemType, (evReg), (ev)))                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Sets task local event(s) (for global setting use Dcm_TskSetEvent()) */
# define Dcm_TskSetLocalEvent(evReg, ev)                             (Dcm_UtiBitOpSet( Dcm_TskTaskEvMemType, (evReg), (ev)))                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! Clears task local event(s) (for global clearing use Dcm_TskClrEvent()) */
# define Dcm_TskClrLocalEvent(evReg, ev)                             (Dcm_UtiBitOpClr( Dcm_TskTaskEvMemType, (evReg), (ev)))                         /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
/*! A generic task event scanner/iterator */
# define DCM_TSK_EVENT_SCAN_LOOP(evReg)                              DCM_UTI_LOOP_BIT_SCAN(evReg)                                                    /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/*! Single task context configuration data */
struct DCM_TSKEVENTCONTEXTTYPE_TAG
{
  Dcm_TskTaskEvMemType  Ev;     /*!< Currently active events */
  Dcm_TskTaskEvMemType  PostEv; /*!< Events to be activated at next main-function cycle */
};
typedef struct DCM_TSKEVENTCONTEXTTYPE_TAG Dcm_TskEventContextType;

/*! Base data type for a generic pointer to a task event context register */
typedef P2VAR(Dcm_TskEventContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_TskEventContextPtrType;

/**********************************************************************************************************************
 *  GLOBAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
# define DCM_START_SEC_CODE
# include "Dcm_MemMap.h"                                                                                                                             /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_TskInit()
 *********************************************************************************************************************/
/*! \brief          Task initialization.
 *  \details        Initialization of the task manager sub-module.
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TskInit(
  void
  );

/**********************************************************************************************************************
 *  Dcm_TskScheduler()
 *********************************************************************************************************************/
/*! \brief          Task scheduling.
 *  \details        Monitors for any tasks to be activated.
 *  \param[in]      tskPrio    The tasks priority to be monitored and activated
 *  \context        TASK
 *  \reentrant      TRUE only in case split task is supported.
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TskScheduler(
  Dcm_TskTaskPrioMemType tskPrio
  );

/**********************************************************************************************************************
 *  Dcm_TskGetEventByThread()
 *********************************************************************************************************************/
/*! \brief          Get event(s) of a task of a specific thread.
 *  \details        Return the given event for a specific task.
 *  \param[in]      taskId    The task handle
 *  \param[in]      threadId  Active thread
 *  \return         Event of the specified task
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(Dcm_TskTaskEvMemType, DCM_CODE) Dcm_TskGetEventByThread(
  Dcm_TskTaskIdOptType taskId,
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_TskSetEventByThread()
 *********************************************************************************************************************/
/*! \brief          Notifies a task for events of a specific thread.
 *  \details        Sets an event for a specific task.
 *  \param[in]      taskId    The task handle
 *  \param[in]      ev        The event(s) to be cleared
 *  \param[in]      threadId  Active thread
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TskSetEventByThread(
  Dcm_TskTaskIdOptType taskId,
  Dcm_TskTaskEvMemType ev,
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_TskClrEventByThread()
 *********************************************************************************************************************/
/*! \brief          Clears event(s) of a task of a specific thread.
 *  \details        Clears the given events for a specific task.
 *  \param[in]      taskId    The task handle
 *  \param[in]      ev        The event(s) to be cleared
 *  \param[in]      threadId  Active thread
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TskClrEventByThread(
  Dcm_TskTaskIdOptType taskId,
  Dcm_TskTaskEvMemType ev,
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_TskGetEvent()
 *********************************************************************************************************************/
/*! \brief          Get event(s) of a task.
 *  \details        Return the given event for a specific task.
 *  \param[in]      taskId    The task handle
 *  \return         Event of the specified task
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Allowed to be called for single threaded tasks only
 *********************************************************************************************************************/
FUNC(Dcm_TskTaskEvMemType, DCM_CODE) Dcm_TskGetEvent(
  Dcm_TskTaskIdOptType taskId
  );

/**********************************************************************************************************************
 *  Dcm_TskSetEvent()
 *********************************************************************************************************************/
/*! \brief          Notifies a task for events.
 *  \details        Sets an event for a specific task.
 *  \param[in]      taskId    The task handle
 *  \param[in]      ev        The event(s) to be cleared
 *  \param[in]      threadId  Active thread
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Allowed to be called for single threaded tasks only
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TskSetEvent(
  Dcm_TskTaskIdOptType taskId,
  Dcm_TskTaskEvMemType ev
  );

/**********************************************************************************************************************
 *  Dcm_TskClrEvent()
 *********************************************************************************************************************/
/*! \brief          Clears event(s) of a task.
 *  \details        Clears the given events for a specific task.
 *  \param[in]      taskId    The task handle
 *  \param[in]      ev        The event(s) to be cleared
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Allowed to be called for single threaded tasks only
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TskClrEvent(
  Dcm_TskTaskIdOptType taskId,
  Dcm_TskTaskEvMemType ev
  );

/**********************************************************************************************************************
 *  Dcm_TskKillAllTasks()
 *********************************************************************************************************************/
/*! \brief          Kills all killable tasks of a specific thread.
 *  \details        Suspends all internal tasks according to their configuration in Dcm_TskTaskInfo[].
 *  \param[in]      threadId  Active thread
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TskKillAllTasks(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_TskReviveAllTasks()
 *********************************************************************************************************************/
/*! \brief          Revives all killed tasks of a specific thread.
 *  \details        -
 *  \param[in]      threadId  Active thread
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *  \pre            Must be called within a critical section.
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TskReviveAllTasks(
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_TmrTaskTimer()
 *********************************************************************************************************************/
/*! \brief          Timer task scheduling.
 *  \details        Scheduling of the timer manager sub-module.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_TmrTaskTimer(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );

# if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_PagedBufferTask()
 *********************************************************************************************************************/
/*! \brief          Lo priority Tx task of the paged-buffer sub-module.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_PagedBufferTask(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_TSK_NUM_ROE > 0u)
/**********************************************************************************************************************
 *  Dcm_ExtSvc86Task()
 *********************************************************************************************************************/
/*! \brief          Service 0x86 task.
 *  \details        Low priority task of the RoE sub-module.
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ExtSvc86Task(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

/**********************************************************************************************************************
 *  Dcm_DiagTaskRx()
 *********************************************************************************************************************/
/*! \brief          High priority Rx task of the network sub-module.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagTaskRx(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );

/**********************************************************************************************************************
 *  Dcm_DiagTaskWorker()
 *********************************************************************************************************************/
/*! \brief          Low priority task of the diagnostic sub-module.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagTaskWorker(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );

# if (DCM_TSK_NUM_DIAG_GARB_COL > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagTaskGarbageCollector()
 *********************************************************************************************************************/
/*! \brief          High priority task of the diagnostic sub-module.
 *  \details        Releases all obsolete transport objects after prioritization.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagTaskGarbageCollector(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagTaskFblRes()
 *********************************************************************************************************************/
/*! \brief          Low priority task of the diagnostic sub-module.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagTaskFblRes(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

/**********************************************************************************************************************
 *  Dcm_DiagTaskTx()
 *********************************************************************************************************************/
/*! \brief          High priority Tx task of the diagnostic sub-module.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagTaskTx(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );

/**********************************************************************************************************************
 *  Dcm_NetTaskTx()
 *********************************************************************************************************************/
/*! \brief          High priority task of the network sub-module for transmissions.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetTaskTx(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );

/**********************************************************************************************************************
 *  Dcm_NetTaskRx()
 *********************************************************************************************************************/
/*! \brief          High priority task of the network sub-module for request prioritization.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetTaskRx(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );

# if (DCM_NET_MONITOR_FOREIGN_N_TA_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_NetTaskTaMonitor()
 *********************************************************************************************************************/
/*! \brief          High priority task of the network sub-module for request processing suppression.
 *  \details        This function is used to prevent a request processing or to cancel an ongoing request processing.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_NetTaskTaMonitor(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_TSK_NUM_MODE_MONITOR > 0u)
/**********************************************************************************************************************
 *  Dcm_ModeTaskMonitor()
 *********************************************************************************************************************/
/*! \brief          Lo priority monitoring task of the Mode manager sub-module.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_ModeTaskMonitor(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_TSK_NUM_SVC2A_SCHEDULER > 0u)
/**********************************************************************************************************************
 *  Dcm_Svc2ATaskSchdProcess()
 *********************************************************************************************************************/
/*! \brief          Low priority task.
 *  \details        Task for service 0x2A scheduling.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc2ATaskSchdProcess(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_TSK_NUM_SVC27 > 0u)
/**********************************************************************************************************************
 *  Dcm_Svc27Task()
 *********************************************************************************************************************/
/*! \brief          Low priority task.
 *  \details        Task for service 0x27.
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_Svc27Task(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
  );
# endif

# if (DCM_TSK_NUM_DIAG_RECOVERY > 0u)
/**********************************************************************************************************************
 *  Dcm_DiagTaskRecovery()
 *********************************************************************************************************************/
/*! \brief          Low priority task of the diagnostic sub-module to recover an external stored DCM state.
 *  \details        -
 *  \param[in]      pContext       Pointer to the context
 *  \param[in,out]  pEventContext  Pointer to the event context
 *  \context        TASK
 *  \reentrant      FALSE
 *  \synchronous    TRUE
 *  \pre            -
 *********************************************************************************************************************/
FUNC(void, DCM_CODE) Dcm_DiagTaskRecovery(
  Dcm_ContextPtrType pContext,
  Dcm_TskEventContextPtrType pEventContext
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

#endif /* !defined(DCM_TSK_H) */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Tsk.h
 *********************************************************************************************************************/
