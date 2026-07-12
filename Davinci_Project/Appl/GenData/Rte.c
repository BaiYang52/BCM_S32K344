/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *             File:  Rte.c
 *           Config:  BCM_S32K344.dpa
 *      ECU-Project:  BCM_S32K344
 *
 *        Generator:  MICROSAR RTE Generator Version 4.32.0
 *                    RTE Core Version 4.32.0
 *          License:  CBD2300384
 *
 *      Description:  RTE implementation file
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* PRQA S 0777, 0779, 0857 EOF */ /* MD_MSR_Rule5.1, MD_MSR_Rule5.2, MD_MSR_Dir1.1 */

#define RTE_CORE
#include "Os.h" /* PRQA S 0828, 0883 */ /* MD_MSR_Dir1.1, MD_Rte_Os */
#include "Rte_Type.h"
#include "Rte_Main.h"

#include "Rte_BswM.h"
#include "Rte_CtAppSwc.h"
#include "Rte_Det.h"
#include "Rte_EcuM.h"
#include "Rte_Os_OsCore0_swc.h"
#include "SchM_BswM.h"
#include "SchM_Det.h"
#include "SchM_EcuM.h"
#include "SchM_Mcu.h"

#include "Rte_Hook.h"

/* AUTOSAR 3.x compatibility */
#if !defined (RTE_LOCAL)
# define RTE_LOCAL static
#endif


/**********************************************************************************************************************
 * API for enable / disable interrupts global
 *********************************************************************************************************************/

#if defined(osDisableGlobalKM) && !defined(RTE_DISABLE_ENHANCED_INTERRUPT_LOCK_API)
# define Rte_DisableAllInterrupts() osDisableGlobalKM()   /* MICROSAR OS */
#else
# define Rte_DisableAllInterrupts() DisableAllInterrupts()   /* AUTOSAR OS */
#endif

#if defined(osEnableGlobalKM) && !defined(RTE_DISABLE_ENHANCED_INTERRUPT_LOCK_API)
# define Rte_EnableAllInterrupts() osEnableGlobalKM()   /* MICROSAR OS */
#else
# define Rte_EnableAllInterrupts() EnableAllInterrupts()   /* AUTOSAR OS */
#endif

/**********************************************************************************************************************
 * API for enable / disable interrupts up to the systemLevel
 *********************************************************************************************************************/

#if defined(osDisableLevelKM) && !defined(RTE_DISABLE_ENHANCED_INTERRUPT_LOCK_API)
# define Rte_DisableOSInterrupts() osDisableLevelKM()   /* MICROSAR OS */
#else
# define Rte_DisableOSInterrupts() SuspendOSInterrupts()   /* AUTOSAR OS */
#endif

#if defined(osEnableLevelKM) && !defined(RTE_DISABLE_ENHANCED_INTERRUPT_LOCK_API)
# define Rte_EnableOSInterrupts() osEnableLevelKM()   /* MICROSAR OS */
#else
# define Rte_EnableOSInterrupts() ResumeOSInterrupts()   /* AUTOSAR OS */
#endif


/**********************************************************************************************************************
 * Timer handling
 *********************************************************************************************************************/

#if defined OS_US2TICKS_SystemTimer
# define RTE_USEC_SystemTimer OS_US2TICKS_SystemTimer
#else
# define RTE_USEC_SystemTimer(val) ((TickType)RTE_CONST_USEC_SystemTimer_##val) /* PRQA S 0342 */ /* MD_MSR_Rule20.10_0342 */
#endif

#if defined OS_MS2TICKS_SystemTimer
# define RTE_MSEC_SystemTimer OS_MS2TICKS_SystemTimer
#else
# define RTE_MSEC_SystemTimer(val) ((TickType)RTE_CONST_MSEC_SystemTimer_##val) /* PRQA S 0342 */ /* MD_MSR_Rule20.10_0342 */
#endif

#if defined OS_SEC2TICKS_SystemTimer
# define RTE_SEC_SystemTimer OS_SEC2TICKS_SystemTimer
#else
# define RTE_SEC_SystemTimer(val)  ((TickType)RTE_CONST_SEC_SystemTimer_##val) /* PRQA S 0342 */ /* MD_MSR_Rule20.10_0342 */
#endif

#define RTE_CONST_MSEC_SystemTimer_0U (0UL)
#define RTE_CONST_MSEC_SystemTimer_10U (10UL)

#define RTE_CONST_SEC_SystemTimer_0U (0UL)
#define RTE_CONST_SEC_SystemTimer_1U (1000UL)


/**********************************************************************************************************************
 * Internal definitions
 *********************************************************************************************************************/

#define RTE_TASK_TIMEOUT_EVENT_MASK   ((EventMaskType)0x01)
#define RTE_TASK_WAITPOINT_EVENT_MASK ((EventMaskType)0x02)

/**********************************************************************************************************************
 * RTE life cycle API
 *********************************************************************************************************************/

#define RTE_START_SEC_CODE
#include "Rte_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

FUNC(void, RTE_CODE) SchM_Start(void)
{
}

FUNC(void, RTE_CODE) SchM_Init(void)
{
  /* activate the tasks */
  (void)ActivateTask(AppRunTask); /* PRQA S 3417 */ /* MD_Rte_Os */

}

FUNC(void, RTE_CODE) SchM_StartTiming(void)
{
  /* activate the alarms used for TimingEvents */
  (void)SetRelAlarm(Rte_Al_TE2_AppRunTask_0_10ms, RTE_MSEC_SystemTimer(0U) + (TickType)1U, RTE_MSEC_SystemTimer(10U)); /* PRQA S 3417, 1840 */ /* MD_Rte_Os, MD_Rte_Os */

}

/* PRQA S 6050 1 */ /* MD_MSR_STCAL */
FUNC(Std_ReturnType, RTE_CODE) Rte_Start(void)
{
  /* activate the tasks */
  (void)ActivateTask(SwcInitTask); /* PRQA S 3417 */ /* MD_Rte_Os */

  /* activate the alarms used for TimingEvents */
  (void)SetRelAlarm(Rte_Al_TE_CtAppSwc_RCtAppSwc_1000ms, RTE_SEC_SystemTimer(0U) + (TickType)1U, RTE_SEC_SystemTimer(1U)); /* PRQA S 3417, 1840 */ /* MD_Rte_Os, MD_Rte_Os */
  (void)SetRelAlarm(Rte_Al_TE_CtAppSwc_RCtAppSwc_10ms, RTE_MSEC_SystemTimer(0U) + (TickType)1U, RTE_MSEC_SystemTimer(10U)); /* PRQA S 3417, 1840 */ /* MD_Rte_Os, MD_Rte_Os */

  return RTE_E_OK;
} /* PRQA S 6050 */ /* MD_MSR_STCAL */

FUNC(Std_ReturnType, RTE_CODE) Rte_Stop(void)
{
  /* deactivate alarms */
  (void)CancelAlarm(Rte_Al_TE_CtAppSwc_RCtAppSwc_1000ms); /* PRQA S 3417 */ /* MD_Rte_Os */
  (void)CancelAlarm(Rte_Al_TE_CtAppSwc_RCtAppSwc_10ms); /* PRQA S 3417 */ /* MD_Rte_Os */

  return RTE_E_OK;
}

FUNC(void, RTE_CODE) SchM_Deinit(void)
{
  /* deactivate alarms */
  (void)CancelAlarm(Rte_Al_TE2_AppRunTask_0_10ms); /* PRQA S 3417 */ /* MD_Rte_Os */

}

FUNC(void, RTE_CODE) Rte_InitMemory(void)
{
}


/**********************************************************************************************************************
 * Exclusive area access
 *********************************************************************************************************************/

FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_00(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_00(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_01(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_01(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_02(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_02(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_03(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_03(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_04(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_04(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_05(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_05(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_06(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_06(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_07(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_07(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_08(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_08(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_09(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_09(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_10(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_10(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_11(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_11(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_12(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_12(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_13(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_13(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_14(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_14(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_15(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_15(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_16(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_16(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_17(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_17(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_18(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_18(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_19(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_19(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_20(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_20(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_21(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_21(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_22(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_22(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_23(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_23(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_24(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_24(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_25(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_25(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_26(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_26(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_27(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_27(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_28(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_28(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_29(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_29(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


FUNC(void, RTE_CODE) SchM_Enter_Mcu_MCU_EXCLUSIVE_AREA_30(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  SuspendAllInterrupts();
}

FUNC(void, RTE_CODE) SchM_Exit_Mcu_MCU_EXCLUSIVE_AREA_30(void)
{
  /* RteAnalyzer(ExclusiveArea, ALL_INTERRUPT_BLOCKING) */
  ResumeAllInterrupts();
}


#define RTE_STOP_SEC_CODE
#include "Rte_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Task bodies for RTE controlled tasks
 *********************************************************************************************************************/
#define RTE_START_SEC_APPRUNTASK_CODE
#include "Rte_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Task:     AppRunTask
 * Priority: 15
 * Schedule: FULL
 *********************************************************************************************************************/
/* PRQA S 6010, 6030, 6050, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
TASK(AppRunTask) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_Unreachable */
{
  EventMaskType ev;

  for(;;) /* FETA_RTE_EXTENDED_TASK */
  {
    (void)WaitEvent(Rte_Ev_Cyclic2_AppRunTask_0_10ms | Rte_Ev_Run_CtAppSwc_RCtAppSwc_1000ms | Rte_Ev_Run_CtAppSwc_RCtAppSwc_10ms); /* PRQA S 3417 */ /* MD_Rte_Os */
    (void)GetEvent(AppRunTask, &ev); /* PRQA S 3417 */ /* MD_Rte_Os */
    (void)ClearEvent(ev & (Rte_Ev_Cyclic2_AppRunTask_0_10ms | Rte_Ev_Run_CtAppSwc_RCtAppSwc_1000ms | Rte_Ev_Run_CtAppSwc_RCtAppSwc_10ms)); /* PRQA S 3417 */ /* MD_Rte_Os */

    if ((ev & Rte_Ev_Cyclic2_AppRunTask_0_10ms) != (EventMaskType)0)
    {
      /* call runnable */
      BswM_MainFunction(); /* PRQA S 2987 */ /* MD_Rte_2987 */

      /* call runnable */
      EcuM_MainFunction(); /* PRQA S 2987 */ /* MD_Rte_2987 */
    }

    if ((ev & Rte_Ev_Run_CtAppSwc_RCtAppSwc_10ms) != (EventMaskType)0)
    {
      /* call runnable */
      RCtAppSwc_10ms(); /* PRQA S 2987 */ /* MD_Rte_2987 */
    }

    if ((ev & Rte_Ev_Run_CtAppSwc_RCtAppSwc_1000ms) != (EventMaskType)0)
    {
      /* call runnable */
      RCtAppSwc_1000ms(); /* PRQA S 2987 */ /* MD_Rte_2987 */
    }
  }
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

#define RTE_STOP_SEC_APPRUNTASK_CODE
#include "Rte_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#define RTE_START_SEC_SWCINITTASK_CODE
#include "Rte_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 * Task:     SwcInitTask
 * Priority: 30
 * Schedule: FULL
 *********************************************************************************************************************/
/* PRQA S 6010, 6030, 6050, 6080 1 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */
TASK(SwcInitTask) /* PRQA S 3408, 1503 */ /* MD_Rte_3408, MD_MSR_Unreachable */
{

  /* call runnable */
  RCtAppSwc_Init(); /* PRQA S 2987 */ /* MD_Rte_2987 */

  (void)TerminateTask(); /* PRQA S 3417 */ /* MD_Rte_Os */
} /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */

#define RTE_STOP_SEC_SWCINITTASK_CODE
#include "Rte_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 MISRA 2012 violations and justifications
 *********************************************************************************************************************/

/* module specific MISRA deviations:
   MD_Rte_2987:  MISRA rule: Rule2.2
     Reason:     Used to simplify code generation.
     Risk:       No functional risk. There is no side effect.
     Prevention: Not required.

   MD_Rte_3408:  MISRA rule: Rule8.4
     Reason:     For the purpose of monitoring during calibration or debugging it is necessary to use non-static declarations.
                 This is covered in the MISRA C compliance section of the Rte specification.
     Risk:       No functional risk.
     Prevention: Not required.

   MD_Rte_Os:
     Reason:     This justification is used as summary justification for all deviations caused by the MICROSAR OS
                 which is for testing of the RTE. Those deviations are no issues in the RTE code.
     Risk:       No functional risk.
     Prevention: Not required.

*/

/**********************************************************************************************************************
  Finite Execution Time Analysis justifications
 *********************************************************************************************************************/

/* COV_JUSTIFICATION_BEGIN

  \ID FETA_RTE_EXTENDED_TASK
  \DESCRIPTION    An extended task is used if more than one runnable entity is mapped to a task with different activation periods
  \COUNTERMEASURE \S SMI-2063

COV_JUSTIFICATION_END */
