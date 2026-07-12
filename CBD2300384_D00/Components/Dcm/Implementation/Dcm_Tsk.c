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
/**        \file  Dcm_Tsk.c
 *         \unit  Tsk
 *        \brief  Contains the implementation of Task Manager unit.
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
#define DCM_TSK_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Dcm_Tsk.h"
#include "Dcm_Debug.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/
/*! Task event representing that no events are available */
#define DCM_TSK_EV_NONE                                              ((Dcm_TskTaskEvMemType)0x00u)

/*! Task execution types */
#define DCM_TSK_ATR_EXEC_ONCE                                        ((Dcm_TskTaskAttributeType)0x00u) /*!< Task will be executed only once per Dcm_MainFunction() call */
#define DCM_TSK_ATR_EXEC_MULTI                                       ((Dcm_TskTaskAttributeType)0x01u) /*!< Task can be executed multiple times per Dcm_MainFunction() call if still has any active events */

/*! Task termination ability types */
#define DCM_TSK_ATR_KILL_NEVER                                       ((Dcm_TskTaskAttributeType)0x00u) /*!< Task can never be terminated on a kill-task signal */
#define DCM_TSK_ATR_KILL_ALWAYS                                      ((Dcm_TskTaskAttributeType)0x02u) /*!< Task will be terminated on a kill task signal */

/*! Task thread ability types */
#define DCM_TSK_ATR_THREAD_SINGLE                                    ((Dcm_TskTaskAttributeType)0x00u) /*!< Task can not support multi threading */
#define DCM_TSK_ATR_THREAD_MULTI                                     ((Dcm_TskTaskAttributeType)0x04u) /*!< Task can support multi threading */

/*! Definition and calculation the total number of system task contexts */
#define DCM_TSK_CONTEXT_NUM_DIAG_RECOVERY                            (DCM_TSK_NUM_DIAG_RECOVERY   * 1u)
#define DCM_TSK_CONTEXT_NUM_NET_TA_MONITOR                           (DCM_TSK_NUM_NET_TA_MONITOR  * 1u)
#define DCM_TSK_CONTEXT_NUM_NET_RX                                   (DCM_TSK_NUM_NET_RX          * DCM_NUM_THREADS)
#define DCM_TSK_CONTEXT_NUM_TIMER                                    (DCM_TSK_NUM_TIMER           * 1u)
#define DCM_TSK_CONTEXT_NUM_DIAG_RX                                  (DCM_TSK_NUM_DIAG_RX         * DCM_NUM_THREADS)
#define DCM_TSK_CONTEXT_NUM_MODE_MONITOR                             (DCM_TSK_NUM_MODE_MONITOR    * 1u)
#define DCM_TSK_CONTEXT_NUM_DIAG_FBLRES                              (DCM_TSK_NUM_DIAG_FBLRES     * 1u)
#define DCM_TSK_CONTEXT_NUM_SVC27                                    (DCM_TSK_NUM_SVC27           * 1u)
#define DCM_TSK_CONTEXT_NUM_DIAG_WORK                                (DCM_TSK_NUM_DIAG_WORK       * DCM_NUM_THREADS)
#define DCM_TSK_CONTEXT_NUM_PGDBUF                                   (DCM_TSK_NUM_PGDBUF          * DCM_NUM_THREADS)
#define DCM_TSK_CONTEXT_NUM_ROE                                      (DCM_TSK_NUM_ROE             * 1u)
#define DCM_TSK_CONTEXT_NUM_DIAG_GARB_COL                            (DCM_TSK_NUM_DIAG_GARB_COL   * DCM_NUM_THREADS)
#define DCM_TSK_CONTEXT_NUM_DIAG_TX                                  (DCM_TSK_NUM_DIAG_TX         * DCM_NUM_THREADS)
#define DCM_TSK_CONTEXT_NUM_SVC2A_SCHEDULER                          (DCM_TSK_NUM_SVC2A_SCHEDULER * 1u)
#define DCM_TSK_CONTEXT_NUM_NET_TX                                   (DCM_TSK_NUM_NET_TX          * 1u)

/*! Pre-compile total number of system tasks calculation */
#define DCM_TSK_NUM_TASKS                                            ((Dcm_TskTaskIdMemType)( DCM_TSK_NUM_TIMER \
                                                                     + DCM_TSK_NUM_DIAG_RX \
                                                                     + DCM_TSK_NUM_MODE_MONITOR \
                                                                     + DCM_TSK_NUM_DIAG_FBLRES \
                                                                     + DCM_TSK_NUM_DIAG_WORK \
                                                                     + DCM_TSK_NUM_PGDBUF \
                                                                     + DCM_TSK_NUM_SVC27 \
                                                                     + DCM_TSK_NUM_ROE \
                                                                     + DCM_TSK_NUM_DIAG_TX \
                                                                     + DCM_TSK_NUM_NET_TX \
                                                                     + DCM_TSK_NUM_NET_RX \
                                                                     + DCM_TSK_NUM_SVC2A_SCHEDULER \
                                                                     + DCM_TSK_NUM_NET_TA_MONITOR \
                                                                     + DCM_TSK_NUM_DIAG_RECOVERY \
                                                                     + DCM_TSK_NUM_DIAG_GARB_COL) )

/*! Pre-compile total number of system task contexts calculation */
#define DCM_TSK_NUM_TASKS_CONTEXT                                    ((Dcm_TskTaskIdMemType)( DCM_TSK_CONTEXT_NUM_TIMER \
                                                                     + DCM_TSK_CONTEXT_NUM_DIAG_RX \
                                                                     + DCM_TSK_CONTEXT_NUM_MODE_MONITOR \
                                                                     + DCM_TSK_CONTEXT_NUM_DIAG_FBLRES \
                                                                     + DCM_TSK_CONTEXT_NUM_DIAG_WORK \
                                                                     + DCM_TSK_CONTEXT_NUM_PGDBUF \
                                                                     + DCM_TSK_CONTEXT_NUM_SVC27 \
                                                                     + DCM_TSK_CONTEXT_NUM_ROE \
                                                                     + DCM_TSK_CONTEXT_NUM_DIAG_TX \
                                                                     + DCM_TSK_CONTEXT_NUM_NET_TX \
                                                                     + DCM_TSK_CONTEXT_NUM_NET_RX \
                                                                     + DCM_TSK_CONTEXT_NUM_SVC2A_SCHEDULER \
                                                                     + DCM_TSK_CONTEXT_NUM_NET_TA_MONITOR \
                                                                     + DCM_TSK_CONTEXT_NUM_DIAG_RECOVERY \
                                                                     + DCM_TSK_CONTEXT_NUM_DIAG_GARB_COL) )

/*! Calculation of all task context IDs */
#define DCM_TSK_CONTEXT_ID_DIAG_RECOVERY                             ((Dcm_TskTaskIdMemType)(0u + 0u))                                                                  /*!< The task context of the Dcm_DiagTaskRecovery() task */
#define DCM_TSK_CONTEXT_ID_NET_TA_MONITOR                            ((Dcm_TskTaskIdMemType)(DCM_TSK_CONTEXT_ID_DIAG_RECOVERY + DCM_TSK_CONTEXT_NUM_DIAG_RECOVERY))     /*!< The task context of the Dcm_NetTaskTaMonitor() task */
#define DCM_TSK_CONTEXT_ID_NET_RX                                    ((Dcm_TskTaskIdMemType)(DCM_TSK_CONTEXT_ID_NET_TA_MONITOR + DCM_TSK_CONTEXT_NUM_NET_TA_MONITOR))   /*!< The task context of the Dcm_NetTaskRx task() */
#define DCM_TSK_CONTEXT_ID_TIMER                                     ((Dcm_TskTaskIdMemType)(DCM_TSK_CONTEXT_ID_NET_RX + DCM_TSK_CONTEXT_NUM_NET_RX))                   /*!< The task context of the Dcm_TmrTaskTimer() task */
#define DCM_TSK_CONTEXT_ID_DIAG_RX                                   ((Dcm_TskTaskIdMemType)(DCM_TSK_CONTEXT_ID_TIMER + DCM_TSK_CONTEXT_NUM_TIMER))                     /*!< The task context of the Dcm_DiagTaskRx() task */
#define DCM_TSK_CONTEXT_ID_MODE_MONITOR                              ((Dcm_TskTaskIdMemType)(DCM_TSK_CONTEXT_ID_DIAG_RX + DCM_TSK_CONTEXT_NUM_DIAG_RX))                 /*!< The task context of the Dcm_ModeTaskMonitor() task */
#define DCM_TSK_CONTEXT_ID_DIAG_FBLRES                               ((Dcm_TskTaskIdMemType)(DCM_TSK_CONTEXT_ID_MODE_MONITOR + DCM_TSK_CONTEXT_NUM_MODE_MONITOR))       /*!< The task context of the Dcm_DiagTaskFblRes() task */
#define DCM_TSK_CONTEXT_ID_SVC27                                     ((Dcm_TskTaskIdMemType)(DCM_TSK_CONTEXT_ID_DIAG_FBLRES + DCM_TSK_CONTEXT_NUM_DIAG_FBLRES))         /*!< The task context of the Dcm_Svc27Task() task */
#define DCM_TSK_CONTEXT_ID_DIAG_WORK                                 ((Dcm_TskTaskIdMemType)(DCM_TSK_CONTEXT_ID_SVC27 + DCM_TSK_CONTEXT_NUM_SVC27))                     /*!< The task context of the Dcm_DiagTaskWorker() task */
#define DCM_TSK_CONTEXT_ID_PGDBUF                                    ((Dcm_TskTaskIdMemType)(DCM_TSK_CONTEXT_ID_DIAG_WORK + DCM_TSK_CONTEXT_NUM_DIAG_WORK))             /*!< The task context of the Dcm_PagedBufferTask() task */
#define DCM_TSK_CONTEXT_ID_ROE                                       ((Dcm_TskTaskIdMemType)(DCM_TSK_CONTEXT_ID_PGDBUF + DCM_TSK_CONTEXT_NUM_PGDBUF))                   /*!< The task context of the Dcm_ExtSvc86Task() task */
#define DCM_TSK_CONTEXT_ID_DIAG_GARB_COL                             ((Dcm_TskTaskIdMemType)(DCM_TSK_CONTEXT_ID_ROE + DCM_TSK_CONTEXT_NUM_ROE))                         /*!< The task context of the Dcm_DiagTaskGarbageCollector() task */
#define DCM_TSK_CONTEXT_ID_DIAG_TX                                   ((Dcm_TskTaskIdMemType)(DCM_TSK_CONTEXT_ID_DIAG_GARB_COL + DCM_TSK_CONTEXT_NUM_DIAG_GARB_COL))     /*!< The task context of the Dcm_DiagTaskTx() task */
#define DCM_TSK_CONTEXT_ID_SVC2A_SCHEDULER                           ((Dcm_TskTaskIdMemType)(DCM_TSK_CONTEXT_ID_DIAG_TX + DCM_TSK_CONTEXT_NUM_DIAG_TX))                 /*!< The task context of the Dcm_Svc2ATaskSchdProcess() task */
#define DCM_TSK_CONTEXT_ID_NET_TX                                    ((Dcm_TskTaskIdMemType)(DCM_TSK_CONTEXT_ID_SVC2A_SCHEDULER + DCM_TSK_CONTEXT_NUM_SVC2A_SCHEDULER)) /*!< The task context of the Dcm_NetTaskTx() task */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef uint8 Dcm_TskTaskAttributeType;

/*! Function pointer prototype for a system task handler */
typedef P2FUNC(void, DCM_CODE, Dcm_TskTaskFuncType) (
  Dcm_ContextPtrType pContext,  /* IN: Pointer to context */
  Dcm_TskEventContextPtrType pEventContext  /* INOUT: Events to be activated at the current and the next main-function cycle */
  );

/*! Single task runtime data */
struct DCM_TSKTASKCONTEXTTYPE_TAG
{
  Dcm_TskTaskEvMemType  TaskEvents;  /*!< Task related events */
#if (DCM_TSK_TASK_COMPLEX_KILL_ENABLED == STD_ON)
  boolean               IsKilled;    /*!< Whether the task is killed or not */
#endif
};
typedef struct DCM_TSKTASKCONTEXTTYPE_TAG Dcm_TskTaskContextType;
typedef volatile P2VAR(Dcm_TskTaskContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_TskTaskContextPtrType;

/*! Task scheduler (one per task priority) control data */
struct DCM_TSKSCHEDULERCONTEXTTYPE_TAG
{
  boolean SchdIsActive;  /*!< Activity state of scheduler (TRUE - scheduler has at least one active task, FALSE - scheduler is suspended) */
};
typedef struct DCM_TSKSCHEDULERCONTEXTTYPE_TAG Dcm_TskSchedulerContextType;
typedef volatile P2VAR(Dcm_TskSchedulerContextType, TYPEDEF, DCM_VAR_NOINIT) Dcm_TskSchedulerContextPtrType;

/*! Single task configuration data */
struct DCM_TSKTASKINFOTYPE_TAG
{
  Dcm_TskTaskFuncType      TskFunc;         /*!< Pointer to the task handler Dcm_<SubComp>Task<Name>() (e.g. Dcm_NetTaskRx()) */
  Dcm_TskTaskAttributeType TskAtr;          /*!< Configuration attributes (e.g. can be terminated, number of executions per Dcm_MainFunction() call etc.) */
  Dcm_TskTaskPrioMemType   TskPrio;         /*!< The priority of the task (e.g. DEFAULT, HIGH, LOW etc.) */
  Dcm_TskTaskEvMemType     NonKillEvents;   /*!< If a task is determined as a killable one (DCM_TSK_ATR_KILL_ALWAYS) here are kept all events that shall not be reset during task termination */
  Dcm_TskTaskIdMemType     TaskContextRef;  /*!< Reference to Dcm_TskMgrSingletonContext.TaskContext[] */
};
typedef struct DCM_TSKTASKINFOTYPE_TAG Dcm_TskTaskInfoType;
typedef P2CONST(Dcm_TskTaskInfoType, TYPEDEF, DCM_CONST) Dcm_TskTaskInfoPtrType;

/*! Control states of the task manager sub-component */
struct DCM_TSKMGRSINGLETONCONTEXTTYPE_TAG
{
  volatile Dcm_TskSchedulerContextType TaskScheduler[DCM_TSK_NUM_PRIOS];        /*!< Control data of each scheduler (one per priority) */
  volatile Dcm_TskTaskContextType      TaskContext[DCM_TSK_NUM_TASKS_CONTEXT];  /*!< Runtime data of each available task (priority independent) */
};
typedef struct DCM_TSKMGRSINGLETONCONTEXTTYPE_TAG Dcm_TskMgrSingletonContextType;

/**********************************************************************************************************************
 *  LOCAL FUNCTION DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_TskGetTaskContext()
 *********************************************************************************************************************/
/*! \brief          Returns a task context element of the given task ID.
 *  \details        -
 *  \param[in]      taskId          The task ID the context will be returned
 *  \param[out]     pTaskContext    The task context element of the given task ID
 *  \return         DCM_E_OK        The operation was successful
 *  \return         DCM_E_NOT_OK    The given task ID was invalid (if runtime checks enabled)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_TskGetTaskContext(
  Dcm_TskTaskIdOptType taskId,
  CONSTP2VAR(Dcm_TskTaskContextPtrType, AUTOMATIC, AUTOMATIC) pTaskContext
  );

/**********************************************************************************************************************
 *  Dcm_TskGetSchedulerContext()
 *********************************************************************************************************************/
/*! \brief          Returns a scheduler context element of the given priority.
 *  \details        -
 *  \param[in]      schedulerPrio        Scheduler priority matching to a specific scheduler context
 *  \param[out]     pSchedulerContext    The scheduler context
 *  \return         DCM_E_OK             The operation was successful
 *  \return         DCM_E_NOT_OK         The given index was invalid (if runtime checks enabled)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_TskGetSchedulerContext(
  Dcm_TskTaskPrioOptType schedulerPrio,
  CONSTP2VAR(Dcm_TskSchedulerContextPtrType, AUTOMATIC, AUTOMATIC) pSchedulerContext
  );

/**********************************************************************************************************************
 *  Dcm_TskGetTaskInfo()
 *********************************************************************************************************************/
/*! \brief          Returns a task info element of the given index.
 *  \details        -
 *  \param[in]      index           Unique handle to a task info
 *  \param[out]     pTaskInfo       The task info element
 *  \return         DCM_E_OK        The operation was successful
 *  \return         DCM_E_NOT_OK    The given index was invalid (if runtime checks enabled)
 *  \context        TASK
 *  \reentrant      TRUE
 *  \pre            -
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_TskGetTaskInfo(
  Dcm_TskTaskIdOptType index,
  CONSTP2VAR(Dcm_TskTaskInfoPtrType, AUTOMATIC, AUTOMATIC) pTaskInfo
  );

/**********************************************************************************************************************
 *  Dcm_TskExecuteActiveTask()
 *********************************************************************************************************************/
/*! \brief          Executes a specific task until all its jobs are finished.
 *  \details        -
 *  \param[in]      taskId    The ID of the task to be executed.
 *  \param[in]      taskEv    A current snapshot of the task's events.
 *  \param[in]      threadId  Active thread
 *  \context        TASK
 *  \reentrant      TRUE only in case split task is supported.
 *  \pre            Must be called only from the Dcm_TskScheduler().
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_TskExecuteActiveTask(
  Dcm_TskTaskIdOptType taskId,
  Dcm_TskTaskEvOptType taskEv,
  Dcm_ThreadIdOptType threadId
  );

/**********************************************************************************************************************
 *  Dcm_TskKillTask()
 *********************************************************************************************************************/
/*! \brief          Kills a killable task.
 *  \details        Suspends all internal tasks according to their configuration in Dcm_TskTaskInfo[].
 *  \param[in]      taskId    ID of the to be killed task
 *  \param[in]      threadId  Active thread
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_TskKillTask(
  Dcm_TskTaskIdOptType taskId,
  Dcm_ThreadIdOptType threadId
  );

#if (DCM_TSK_TASK_COMPLEX_KILL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_TskReviveTask()
 *********************************************************************************************************************/
/*! \brief          Revives a killed task.
 *  \details        Resumes all internal tasks according to their configuration in Dcm_TskTaskInfo[].
 *  \param[in]      taskId    ID of the to be killed task
 *  \param[in]      threadId  Active thread
 *  \context        ISR1|ISR2|TASK
 *  \reentrant      TRUE
 *  \pre            Global interrupts shall be disabled.
 *********************************************************************************************************************/
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_TskReviveTask(
  Dcm_TskTaskIdOptType taskId,
  Dcm_ThreadIdOptType threadId
  );
#endif
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL DATA DECLARATIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Singleton context of TskMgr unit */
DCM_LOCAL VAR(Dcm_TskMgrSingletonContextType, DCM_VAR_NOINIT) Dcm_TskMgrSingletonContext;
#define DCM_STOP_SEC_VAR_NO_INIT_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define DCM_START_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*! Configuration of all DCM internal tasks */
DCM_LOCAL CONST(Dcm_TskTaskInfoType, DCM_CONST) Dcm_TskTaskInfo[DCM_TSK_NUM_TASKS]=                                                                  /* PRQA S 3218 */ /* MD_Dcm_CodingRule_3218 */
{
  /* first to be processed */
#if (DCM_TSK_NUM_DIAG_RECOVERY > 0u)
  {Dcm_DiagTaskRecovery,     (DCM_TSK_ATR_EXEC_MULTI | DCM_TSK_ATR_KILL_NEVER | DCM_TSK_ATR_THREAD_SINGLE), DCM_TSK_PRIO_LOW,  DCM_TSK_EV_NONE, DCM_TSK_CONTEXT_ID_DIAG_RECOVERY}, /* Task to recover an external stored DCM state */
#endif
#if (DCM_TSK_NUM_NET_TA_MONITOR > 0u)
  {Dcm_NetTaskTaMonitor,     (DCM_TSK_ATR_EXEC_ONCE  | DCM_TSK_ATR_KILL_NEVER | DCM_TSK_ATR_THREAD_SINGLE), DCM_TSK_PRIO_HIGH, DCM_TSK_EV_NONE, DCM_TSK_CONTEXT_ID_NET_TA_MONITOR}, /* Requests with a foreign N_TA will be handled here */
#endif
  {Dcm_NetTaskRx,            (DCM_TSK_ATR_EXEC_ONCE  | DCM_TSK_ATR_KILL_NEVER | DCM_TSK_ATR_THREAD_MULTI), DCM_TSK_PRIO_HIGH, DCM_TSK_EV_NONE, DCM_TSK_CONTEXT_ID_NET_RX}, /* The P2 timer will be started here - put this task prior timer task. NetRx task must always work */
  {Dcm_TmrTaskTimer,         (DCM_TSK_ATR_EXEC_ONCE  | DCM_TSK_ATR_KILL_NEVER | DCM_TSK_ATR_THREAD_SINGLE), DCM_TSK_PRIO_HIGH, DCM_TSK_EV_NONE, DCM_TSK_CONTEXT_ID_TIMER}, /* process timers first, the tasks will do their job according to the event priorities. Never kill timer task! */
  {Dcm_DiagTaskRx,           (DCM_TSK_ATR_EXEC_ONCE  | DCM_TSK_ATR_KILL_ALWAYS | DCM_TSK_ATR_THREAD_MULTI), DCM_TSK_PRIO_HIGH, (DCM_TSK_EV_DIAG_RX_TMR_P2_TO | DCM_TSK_EV_DIAG_RX_NEW_REQ), DCM_TSK_CONTEXT_ID_DIAG_RX},
#if (DCM_TSK_NUM_MODE_MONITOR > 0u)
  {Dcm_ModeTaskMonitor,      (DCM_TSK_ATR_EXEC_MULTI | DCM_TSK_ATR_KILL_NEVER | DCM_TSK_ATR_THREAD_SINGLE), DCM_TSK_PRIO_LOW,  DCM_TSK_EV_NONE, DCM_TSK_CONTEXT_ID_MODE_MONITOR}, /* let monitors work in the background */
#endif
#if (DCM_TSK_NUM_DIAG_FBLRES > 0u)
  {Dcm_DiagTaskFblRes,       (DCM_TSK_ATR_EXEC_MULTI | DCM_TSK_ATR_KILL_NEVER | DCM_TSK_ATR_THREAD_SINGLE), DCM_TSK_PRIO_LOW,  DCM_TSK_EV_NONE, DCM_TSK_CONTEXT_ID_DIAG_FBLRES},/* let FBL work in the background till end */
#endif
#if (DCM_TSK_NUM_SVC27 > 0u)
  {Dcm_Svc27Task,            (DCM_TSK_ATR_EXEC_ONCE  | DCM_TSK_ATR_KILL_NEVER | DCM_TSK_ATR_THREAD_SINGLE), DCM_TSK_PRIO_LOW,  DCM_TSK_EV_NONE, DCM_TSK_CONTEXT_ID_SVC27}, /* Collects the attempt counter values */
#endif
  {Dcm_DiagTaskWorker,       (DCM_TSK_ATR_EXEC_MULTI | DCM_TSK_ATR_KILL_ALWAYS | DCM_TSK_ATR_THREAD_MULTI), DCM_TSK_PRIO_LOW,  (DCM_TSK_EV_DIAG_WORK_KILL_JOBS|DCM_TSK_EV_DIAG_WORK_INT_SET2DEF|DCM_TSK_EV_DIAG_WORK_NEW_REQ|DCM_TSK_EV_DIAG_WORK_TX_END_FINAL|DCM_TSK_EV_DIAG_WORK_IDLE_CONN|DCM_TSK_EV_DIAG_WORK_EXT_SETROLE), DCM_TSK_CONTEXT_ID_DIAG_WORK},
#if (DCM_PAGED_BUFFER_ENABLED == STD_ON)
  {Dcm_PagedBufferTask,      (DCM_TSK_ATR_EXEC_MULTI | DCM_TSK_ATR_KILL_ALWAYS | DCM_TSK_ATR_THREAD_MULTI), DCM_TSK_PRIO_LOW,  DCM_TSK_EV_NONE, DCM_TSK_CONTEXT_ID_PGDBUF},
#endif
#if (DCM_TSK_NUM_ROE > 0u)
  {Dcm_ExtSvc86Task,         (DCM_TSK_ATR_ROE_EXEC   | DCM_TSK_ATR_KILL_NEVER | DCM_TSK_ATR_THREAD_SINGLE), DCM_TSK_PRIO_LOW,  DCM_TSK_EV_NONE, DCM_TSK_CONTEXT_ID_ROE}, /* let RoE work in the background - if RoE triggered the client prioritization will take place */
#endif
#if (DCM_TSK_NUM_DIAG_GARB_COL > 0u)
  {Dcm_DiagTaskGarbageCollector, (DCM_TSK_ATR_EXEC_ONCE | DCM_TSK_ATR_KILL_NEVER | DCM_TSK_ATR_THREAD_SINGLE),  DCM_TSK_PRIO_HIGH, DCM_TSK_EV_NONE, DCM_TSK_CONTEXT_ID_DIAG_GARB_COL},
#endif
  {Dcm_DiagTaskTx,           (DCM_TSK_ATR_EXEC_MULTI | DCM_TSK_ATR_KILL_ALWAYS | DCM_TSK_ATR_THREAD_MULTI), DCM_TSK_PRIO_HIGH, DCM_TSK_EV_DIAG_TX_SEND_RCRRP, DCM_TSK_CONTEXT_ID_DIAG_TX},
#if (DCM_TSK_NUM_SVC2A_SCHEDULER > 0u)
  {Dcm_Svc2ATaskSchdProcess, (DCM_TSK_ATR_EXEC_ONCE  | DCM_TSK_ATR_KILL_NEVER | DCM_TSK_ATR_THREAD_SINGLE), DCM_TSK_PRIO_LOW,  DCM_TSK_EV_NONE, DCM_TSK_CONTEXT_ID_SVC2A_SCHEDULER},/* let 0x2A work in the background if needed to be stopped -> the stop all API shall be used */
#endif
  {Dcm_NetTaskTx,            (DCM_TSK_ATR_EXEC_MULTI | DCM_TSK_ATR_KILL_NEVER | DCM_TSK_ATR_THREAD_SINGLE), DCM_TSK_PRIO_HIGH, DCM_TSK_EV_NONE, DCM_TSK_CONTEXT_ID_NET_TX}
  /* last to be processed */
};
#define DCM_STOP_SEC_CONST_UNSPECIFIED
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_TskGetTaskContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_TskGetTaskContext(
  Dcm_TskTaskIdOptType taskId,
  CONSTP2VAR(Dcm_TskTaskContextPtrType, AUTOMATIC, AUTOMATIC) pTaskContext
  )
{
  Std_ReturnType lResult;
                                                                                                                                                     /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */
  if(Dcm_DebugDetectRuntimeError(taskId >= DCM_TSK_NUM_TASKS_CONTEXT))                                                                               /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = DCM_E_NOT_OK;
  }
  else
  {
    *pTaskContext = &Dcm_TskMgrSingletonContext.TaskContext[taskId];                                                                                 /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_OK;
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_TskGetSchedulerContext()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_TskGetSchedulerContext(
  Dcm_TskTaskPrioOptType schedulerPrio,
  CONSTP2VAR(Dcm_TskSchedulerContextPtrType, AUTOMATIC, AUTOMATIC) pSchedulerContext
  )
{
  Std_ReturnType lResult;

  if(Dcm_DebugDetectRuntimeError(schedulerPrio >= DCM_TSK_NUM_PRIOS))                                                                                /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = DCM_E_NOT_OK;
  }
  else
  {
    *pSchedulerContext = &Dcm_TskMgrSingletonContext.TaskScheduler[schedulerPrio];                                                                   /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_OK;
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_TskGetTaskInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DCM_LOCAL_INLINE FUNC(Std_ReturnType, DCM_CODE) Dcm_TskGetTaskInfo(
  Dcm_TskTaskIdOptType index,
  CONSTP2VAR(Dcm_TskTaskInfoPtrType, AUTOMATIC, AUTOMATIC) pTaskInfo
  )
{
  Std_ReturnType lResult;

  if(Dcm_DebugDetectRuntimeError(index >= DCM_TSK_NUM_TASKS))                                                                                        /* PRQA S 2992 */ /* MD_Dcm_ConstExpr */
  {
    Dcm_DebugReportError(DCM_SID_INTERNAL, DCM_E_CRITICAL_ERROR);
    lResult = DCM_E_NOT_OK;
  }
  else
  {
    *pTaskInfo = &Dcm_TskTaskInfo[index];                                                                                                            /* SBSW_DCM_PARAM_PTR_WRITE */
    lResult = DCM_E_OK;
  }

  return lResult;
}

/**********************************************************************************************************************
 *  Dcm_TskKillTask()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_TskKillTask(
  Dcm_TskTaskIdOptType taskId,
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_TskTaskContextPtrType pTaskContext;
  Dcm_TskTaskIdOptType      lTaskContextId;

  /*
   * This function is called for killable tasks only. All of them are instantiated per thread.
   * Therefore it is safe to add the threadId to the TaskContextRef without checking first whether the task is
   * instantiated per thread or not. No debug assertion is added here to keep the critical section short.
   */
  lTaskContextId = (Dcm_TskTaskIdOptType)(Dcm_TskTaskInfo[taskId].TaskContextRef + threadId);

  if(Dcm_TskGetTaskContext(lTaskContextId, &pTaskContext) == DCM_E_OK)                                                                               /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */ /* SBSW_DCM_POINTER_FORWARD_STACK */
  {
    pTaskContext->TaskEvents &= Dcm_TskTaskInfo[taskId].NonKillEvents; /* clear only killable events */                                              /* SBSW_DCM_POINTER_WRITE_TASKCONTEXT */

#if (DCM_TSK_TASK_COMPLEX_KILL_ENABLED == STD_ON)
    pTaskContext->IsKilled = TRUE;                                                                                                                   /* SBSW_DCM_POINTER_WRITE_TASKCONTEXT */
#endif
  }
}

#if (DCM_TSK_TASK_COMPLEX_KILL_ENABLED == STD_ON)
/**********************************************************************************************************************
 *  Dcm_TskReviveTask()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_TskReviveTask(
  Dcm_TskTaskIdOptType taskId,
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_TskTaskContextPtrType pTaskContext;
  Dcm_TskTaskIdOptType      lTaskContextId;

  /*
   * This function is called for killable tasks only. All of them are instantiated per thread.
   * Therefore it is safe to add the threadId to the TaskContextRef without checking first whether the task is
   * instantiated per thread or not. No debug assertion is added here to keep the critical section short.
   */
  lTaskContextId = (Dcm_TskTaskIdOptType)(Dcm_TskTaskInfo[taskId].TaskContextRef + threadId);

  if(Dcm_TskGetTaskContext(lTaskContextId, &pTaskContext) == DCM_E_OK)                                                                               /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */ /* SBSW_DCM_POINTER_FORWARD_STACK */
  {
    pTaskContext->IsKilled = FALSE;                                                                                                                  /* SBSW_DCM_POINTER_WRITE_TASKCONTEXT */
  }
}
#endif

/**********************************************************************************************************************
 *  Dcm_TskExecuteActiveTask()
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
DCM_LOCAL_INLINE FUNC(void, DCM_CODE) Dcm_TskExecuteActiveTask(
  Dcm_TskTaskIdOptType taskId,
  Dcm_TskTaskEvOptType taskEv,
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_TskTaskInfoPtrType pTaskInfo;
  Std_ReturnType         lResult;

  lResult = Dcm_TskGetTaskInfo(taskId, &pTaskInfo);                                                                                                  /* SBSW_DCM_POINTER_FORWARD_STACK */

  if(lResult == DCM_E_OK)                                                                                                                            /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    Dcm_TskEventContextType  lEventContext;
    Dcm_ContextPtrType       pContext;

    pContext = Dcm_GetThreadContext(threadId);

    lEventContext.Ev = (Dcm_TskTaskEvMemType)taskEv;
    lEventContext.PostEv = DCM_TSK_EV_NONE;

    /* Handle task execution: */
    do
    {
      /* Clear all already known global events */
      Dcm_TskClrEventByThread(taskId, lEventContext.Ev, threadId);

      /* Execute the task */
      pTaskInfo->TskFunc(pContext, &lEventContext);                                                                                                  /* SBSW_DCM_CALL_FUNCPTR_TASKINFO */ /* SBSW_DCM_COMB_PTR_FORWARD */

      /* If the task supports multiple activations per scheduler invocation, do get any new events sent to this task */
      if((pTaskInfo->TskAtr & DCM_TSK_ATR_EXEC_MULTI) != 0u)
      {
        lEventContext.Ev = Dcm_TskGetEventByThread(taskId, threadId);
      }
      else
      {
        /* Otherwise do not re-activate the task in this scheduler invocation */
        lEventContext.Ev = DCM_TSK_EV_NONE;
      }
    }
    /* As long as the task needs the be re-activated */
    while (lEventContext.Ev != DCM_TSK_EV_NONE);

    /* Register any new events of the already finished task that are scheduled for its next activation */
    if(lEventContext.PostEv != DCM_TSK_EV_NONE)
    {
      Dcm_TskSetEventByThread(taskId, lEventContext.PostEv, threadId);
    }
  }
}                                                                                                                                                    /* PRQA S 6050 */ /* MD_MSR_STCAL */
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
#define DCM_START_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/**********************************************************************************************************************
 *  Dcm_TskInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_TskInit(
  void
  )
{
  /* Initialize priority related states */
  {
    Dcm_TskTaskPrioOptType tskPrioIter;

    for(tskPrioIter = 0; tskPrioIter < DCM_TSK_NUM_PRIOS; ++tskPrioIter)
    {
      Dcm_TskSchedulerContextPtrType pSchedulerContext;

      if(Dcm_TskGetSchedulerContext(tskPrioIter, &pSchedulerContext) == DCM_E_OK)                                                                    /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */ /* SBSW_DCM_POINTER_FORWARD_STACK */
      {
        pSchedulerContext->SchdIsActive = FALSE;                                                                                                     /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_SCHEDULERCONTEXT */
      }
    }
  }

  /* Initialize task related events */
  {
    Dcm_TskTaskIdOptType tskIter;

    for(tskIter = 0; tskIter < DCM_TSK_NUM_TASKS_CONTEXT; ++tskIter)
    {
      Dcm_TskTaskContextPtrType pTaskContext;

      if(Dcm_TskGetTaskContext(tskIter, &pTaskContext) == DCM_E_OK)                                                                                  /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */ /* SBSW_DCM_POINTER_FORWARD_STACK */
      {
        pTaskContext->TaskEvents = DCM_TSK_EV_NONE;                                                                                                  /* SBSW_DCM_POINTER_WRITE_TASKCONTEXT */
#if (DCM_TSK_TASK_COMPLEX_KILL_ENABLED == STD_ON)
        pTaskContext->IsKilled  = FALSE;                                                                                                             /* SBSW_DCM_POINTER_WRITE_TASKCONTEXT */
#endif
      }
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_TskScheduler()
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
FUNC(void, DCM_CODE) Dcm_TskScheduler(
  Dcm_TskTaskPrioMemType tskPrio
  )
{
  Dcm_TskSchedulerContextPtrType pSchedulerContext;
  Std_ReturnType                 lResult;

  lResult = Dcm_TskGetSchedulerContext(tskPrio, &pSchedulerContext);                                                                                 /* SBSW_DCM_POINTER_FORWARD_STACK */

  if (lResult == DCM_E_OK)                                                                                                                           /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    if(pSchedulerContext->SchdIsActive == TRUE)                                                                                                      /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */
    {
      Dcm_TskTaskIdOptType  lTaskIter;
      Dcm_TskTaskEvOptType  lEv;

      /*
       * No interrupt protection needed since:
       * If a SetEvent is called and interrupts this assignment, then it sets this flag to true. Two scenarios are possible:
       * - The TRUE signal may be gone - it does not matter since the scheduler is already activated.
       * - The TRUE signal remains - in the worst case the scheduler will be executed once more later but will not have any active tasks.
       */
      pSchedulerContext->SchdIsActive = FALSE; /* per default - no active tasks any more. If any SetEvent executed during the task execution -> the flag will be set again */ /* PRQA S 2841 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_SCHEDULERCONTEXT */

      for(lTaskIter = 0; lTaskIter < DCM_TSK_NUM_TASKS; ++lTaskIter)
      {
#if (DCM_SPLIT_TASKS_ENABLED == STD_ON)
        if(tskPrio == Dcm_TskTaskInfo[lTaskIter].TskPrio)
#endif
        {
          Dcm_ThreadIdOptType lThreadIter;

          for(lThreadIter = 0; lThreadIter < DCM_NUM_THREADS; ++lThreadIter)
          {
            lEv = Dcm_TskGetEventByThread(lTaskIter, lThreadIter);                                                                                   /* PRQA S 2985 */ /* MD_Dcm_Redundant_2985 */

            if(lEv != DCM_TSK_EV_NONE)
            {
              Dcm_TskExecuteActiveTask(lTaskIter, lEv, lThreadIter);
            }

#if (DCM_MULTI_THREAD_ENABLED == STD_ON)
            if ((Dcm_TskTaskInfo[lTaskIter].TskAtr & DCM_TSK_ATR_THREAD_MULTI) == 0u) /* Single Thread Task */
            {
              break;
            }
#endif
          }
        }
      }
    }
  }
}                                                                                                                                                    /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  Dcm_TskGetEventByThread()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Dcm_TskTaskEvMemType, DCM_CODE) Dcm_TskGetEventByThread(
  Dcm_TskTaskIdOptType taskId,
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_DebugAssert((threadId < DCM_NUM_THREADS), DCM_SID_INTERNAL, DCM_E_PARAM);                                                                      /* COV_DCM_RTM_DEV_DEBUG XF */
  Dcm_DebugAssert( ((Dcm_TskTaskInfo[taskId].TskAtr & DCM_TSK_ATR_THREAD_MULTI) != 0u)
                 ||(threadId == DCM_THREAD_ID_BASIC), DCM_SID_INTERNAL, DCM_E_PARAM);                                                                /* COV_DCM_RTM_DEV_DEBUG XF */

  return Dcm_TskMgrSingletonContext.TaskContext[Dcm_TskTaskInfo[taskId].TaskContextRef + threadId].TaskEvents;
}

/**********************************************************************************************************************
 *  Dcm_TskSetEventByThread()
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
FUNC(void, DCM_CODE) Dcm_TskSetEventByThread(
  Dcm_TskTaskIdOptType taskId,
  Dcm_TskTaskEvMemType ev,
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_TskTaskInfoPtrType pTaskInfo;
  Std_ReturnType         lResult;
  Dcm_TskTaskEvMemType   lEv = ev;

  /* No zero events are allowed! Possibly the Dcm_TskTaskEvMemType is too small to hold the value? */
  Dcm_DebugAssert((lEv != DCM_TSK_EV_NONE), DCM_SID_INTERNAL, DCM_E_PARAM);                                                                          /* COV_DCM_RTM_DEV_DEBUG XF */

  lResult = Dcm_TskGetTaskInfo(taskId, &pTaskInfo);                                                                                                  /* SBSW_DCM_POINTER_FORWARD_STACK */

  if(lResult == DCM_E_OK)                                                                                                                            /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    Dcm_TskSchedulerContextPtrType pSchedulerContext;

    lResult = Dcm_TskGetSchedulerContext(pTaskInfo->TskPrio, &pSchedulerContext);                                                                    /* SBSW_DCM_POINTER_FORWARD_STACK */

    if(lResult == DCM_E_OK)                                                                                                                          /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
    {
      Dcm_TskTaskContextPtrType pTaskContext;
      Dcm_TskTaskIdOptType      lTaskContextId;

      Dcm_DebugAssert((threadId < DCM_NUM_THREADS), DCM_SID_INTERNAL, DCM_E_PARAM);                                                                  /* COV_DCM_RTM_DEV_DEBUG XF */
      Dcm_DebugAssert( ((Dcm_TskTaskInfo[taskId].TskAtr & DCM_TSK_ATR_THREAD_MULTI) != 0u)
                     ||(threadId == DCM_THREAD_ID_BASIC), DCM_SID_INTERNAL, DCM_E_PARAM);                                                            /* COV_DCM_RTM_DEV_DEBUG XF */

      lTaskContextId = (Dcm_TskTaskIdOptType)(pTaskInfo->TaskContextRef + threadId);
      lResult        = Dcm_TskGetTaskContext(lTaskContextId, &pTaskContext);                                                                         /* SBSW_DCM_POINTER_FORWARD_STACK */

      if(lResult == DCM_E_OK)                                                                                                                        /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
      {
        /* Enter critical section (Reason: a task event can be set during any ISR or higher priority task processing.) */
        Dcm_UtiEnterCriticalSection();
        /*=================================*
          BEGIN CRITICAL SECTION
         *=================================*/
#if (DCM_TSK_TASK_COMPLEX_KILL_ENABLED == STD_ON)
        /* If the task is marked as suspended, do set only those new events that are non-killable */
        if(pTaskContext->IsKilled)
        {
          /* mask all to be set events, except the non-killable ones! */
          lEv &= Dcm_TskTaskInfo[taskId].NonKillEvents;
        }
#endif
        /* Add any masked/unmasked new events */
        Dcm_UtiBitOpSet(Dcm_TskTaskEvMemType, pTaskContext->TaskEvents, lEv);                                                                        /* SBSW_DCM_POINTER_WRITE_TASKCONTEXT */
        /* Activate the scheduler associated with the task */
        pSchedulerContext->SchdIsActive = TRUE;                                                                                                      /* PRQA S 2841, 2842 */ /* MD_Dcm_DerefInvalidPointer */ /* SBSW_DCM_POINTER_WRITE_SCHEDULERCONTEXT */
        /*=================================*
          END CRITICAL SECTION
         *=================================*/
        Dcm_UtiLeaveCriticalSection();
      }
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_TskClrEventByThread()
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
FUNC(void, DCM_CODE) Dcm_TskClrEventByThread(
  Dcm_TskTaskIdOptType taskId,
  Dcm_TskTaskEvMemType ev,
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_TskTaskInfoPtrType pTaskInfo;
  Std_ReturnType         lResult;

  /* No zero events are allowed! Possibly the Dcm_TskTaskEvMemType is too small to hold the value? */
  Dcm_DebugAssert((ev != DCM_TSK_EV_NONE), DCM_SID_INTERNAL, DCM_E_PARAM);                                                                           /* COV_DCM_RTM_DEV_DEBUG XF */

  lResult = Dcm_TskGetTaskInfo(taskId, &pTaskInfo);                                                                                                  /* SBSW_DCM_POINTER_FORWARD_STACK */

  if (lResult == DCM_E_OK)                                                                                                                           /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
  {
    Dcm_TskTaskIdOptType      lTaskContextId;
    Dcm_TskTaskContextPtrType pTaskContext;

    Dcm_DebugAssert((threadId < DCM_NUM_THREADS), DCM_SID_INTERNAL, DCM_E_PARAM);                                                                    /* COV_DCM_RTM_DEV_DEBUG XF */
    Dcm_DebugAssert( ((Dcm_TskTaskInfo[taskId].TskAtr & DCM_TSK_ATR_THREAD_MULTI) != 0u)
                   ||(threadId == DCM_THREAD_ID_BASIC), DCM_SID_INTERNAL, DCM_E_PARAM);                                                              /* COV_DCM_RTM_DEV_DEBUG XF */

    lTaskContextId = (Dcm_TskTaskIdOptType)(pTaskInfo->TaskContextRef + threadId);
    lResult        = Dcm_TskGetTaskContext(lTaskContextId, &pTaskContext);                                                                           /* SBSW_DCM_POINTER_FORWARD_STACK */

    if (lResult == DCM_E_OK)                                                                                                                         /* PRQA S 2991, 2995 */ /* MD_Dcm_ConstExpr */ /* COV_DCM_RTM_RUNTIME_CHECK TX */
    {
      Dcm_UtiEnterCriticalSection();
      /*=================================*
        BEGIN CRITICAL SECTION
       *=================================*/
      Dcm_UtiBitOpClr(Dcm_TskTaskEvMemType, pTaskContext->TaskEvents, ev);                                                                           /* SBSW_DCM_POINTER_WRITE_TASKCONTEXT */
      /*=================================*
        END CRITICAL SECTION
       *=================================*/
      Dcm_UtiLeaveCriticalSection();
    }
  }
}

/**********************************************************************************************************************
 *  Dcm_TskGetEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(Dcm_TskTaskEvMemType, DCM_CODE) Dcm_TskGetEvent(
  Dcm_TskTaskIdOptType taskId
  )
{
  Dcm_DebugAssert(((Dcm_TskTaskInfo[taskId].TskAtr & DCM_TSK_ATR_THREAD_MULTI) == 0u), DCM_SID_INTERNAL, DCM_E_PARAM);                               /* COV_DCM_RTM_DEV_DEBUG XF */

  return Dcm_TskGetEventByThread(taskId, DCM_THREAD_ID_BASIC);
}

/**********************************************************************************************************************
 *  Dcm_TskSetEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_TskSetEvent(
  Dcm_TskTaskIdOptType taskId,
  Dcm_TskTaskEvMemType ev
  )
{
  Dcm_DebugAssert(((Dcm_TskTaskInfo[taskId].TskAtr & DCM_TSK_ATR_THREAD_MULTI) == 0u), DCM_SID_INTERNAL, DCM_E_PARAM);                               /* COV_DCM_RTM_DEV_DEBUG XF */

  Dcm_TskSetEventByThread(taskId, ev, DCM_THREAD_ID_BASIC);
}

/**********************************************************************************************************************
 *  Dcm_TskClrEvent()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_TskClrEvent(
  Dcm_TskTaskIdOptType taskId,
  Dcm_TskTaskEvMemType ev
  )
{
  Dcm_DebugAssert(((Dcm_TskTaskInfo[taskId].TskAtr & DCM_TSK_ATR_THREAD_MULTI) == 0u), DCM_SID_INTERNAL, DCM_E_PARAM);                               /* COV_DCM_RTM_DEV_DEBUG XF */

  Dcm_TskClrEventByThread(taskId, ev, DCM_THREAD_ID_BASIC);
}

/**********************************************************************************************************************
 *  Dcm_TskKillAllTasks()
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
FUNC(void, DCM_CODE) Dcm_TskKillAllTasks(
  Dcm_ThreadIdOptType threadId
  )
{
  Dcm_TskTaskIdOptType tskIter;

  /* Enter critical section (Reason: A task event can be set during any ISR processing.) */
  Dcm_UtiEnterCriticalSection();
  /*=================================*
    BEGIN CRITICAL SECTION
   *=================================*/

  for(tskIter = 0; tskIter < DCM_TSK_NUM_TASKS; ++tskIter)
  {
    if((Dcm_TskTaskInfo[tskIter].TskAtr & DCM_TSK_ATR_KILL_ALWAYS) != 0u)
    {
      Dcm_TskKillTask(tskIter, threadId);
    }
  }

  /*=================================*
    END CRITICAL SECTION
   *=================================*/
  Dcm_UtiLeaveCriticalSection();
}

/**********************************************************************************************************************
 *  Dcm_TskReviveAllTasks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
FUNC(void, DCM_CODE) Dcm_TskReviveAllTasks(
  Dcm_ThreadIdOptType threadId
  )
{
#if (DCM_TSK_TASK_COMPLEX_KILL_ENABLED == STD_ON)
   Dcm_TskTaskIdOptType tskIter;

   for(tskIter = 0; tskIter < DCM_TSK_NUM_TASKS; ++tskIter)
   {
     if((Dcm_TskTaskInfo[tskIter].TskAtr & DCM_TSK_ATR_KILL_ALWAYS) != 0u)
     {
       Dcm_TskReviveTask(tskIter, threadId);
     }
   }
#else
   DCM_IGNORE_UNREF_PARAM(threadId);                                                                                                                 /* PRQA S 3112 */ /* MD_MSR_DummyStmt */
#endif
}
#define DCM_STOP_SEC_CODE
#include "Dcm_MemMap.h"                                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  END OF FILE: Dcm_Tsk.c
 *********************************************************************************************************************/
