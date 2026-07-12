/* ********************************************************************************************************************
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
 *  -------------------------------------------------------------------------------------------------------------------
 */
/*! \addtogroup Dem_Queue
 *  \{
 *  \file       Dem_Queue_Implementation.h
 *  \brief      Diagnostic Event Manager (Dem) header file
 *  \details
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  -------------------------------------------------------------------------------------------------------------------
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  REFER TO DEM.H
 *********************************************************************************************************************/

#if !defined (DEM_QUEUE_IMPLEMENTATION_H)
#define DEM_QUEUE_IMPLEMENTATION_H

/* ********************************************************************************************************************
 *  GLOBAL MISRA JUSTIFICATIONS
 *********************************************************************************************************************/
/* PRQA S 2880 EOF */ /* MD_DEM_2.1_UNREACHABLE_STATEMENT */
/* PRQA S 2982 EOF */ /* MD_DEM_2982 */
/* PRQA S 2983 EOF */ /* MD_DEM_2983 */
/* PRQA S 2985 EOF */ /* MD_DEM_2985 */
/* PRQA S 2986 EOF */ /* MD_DEM_2986 */
/* PRQA S 2987 EOF */ /* MD_DEM_2987 */
/* PRQA S 2991 EOF */ /* MD_DEM_2991 */
/* PRQA S 2992 EOF */ /* MD_DEM_2992 */
/* PRQA S 2995 EOF */ /* MD_DEM_2995 */
/* PRQA S 2996 EOF */ /* MD_DEM_2996 */
/* PRQA S 0759 EOF */ /* MD_MSR_Union */

/* ********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

                                                  /* Own subcomponent header */
/* ------------------------------------------------------------------------- */
#include "Dem_Queue_Interface.h"

/* Includes to access public functions belonging to other (top level)/ (logical)/- units to be used by this Unit */
/* ------------------------------------------------------------------------- */
#include "Dem_Infrastructure_Interface.h"
#include "Dem_Core_Interface.h"
#include "Dem_FaultMemory_Interface.h"
#include "Dem_DiagnosticObserver_Interface.h"
#include "Dem_Data_Interface.h"
#include "Dem_Readiness_Interface.h"
#include "Dem_GlobalState_Interface.h"


/* ********************************************************************************************************************
 *  SUBCOMPONENT CONSTANT MACROS
 *********************************************************************************************************************/

/*! Filename declaration */
#define DEM_QUEUE_IMPLEMENTATION_FILENAME "Dem_Queue_Implementation.h"

/* ********************************************************************************************************************
 *  SUBCOMPONENT FUNCTION MACROS
 *********************************************************************************************************************/

/* ********************************************************************************************************************
 *  SUBCOMPONENT DATA
 *********************************************************************************************************************/
#define DEM_START_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! DCY qualification flag */
DEM_LOCAL VAR(boolean, DEM_VAR_NOINIT)           Dem_Queue_DcyCycleFlag;

#define DEM_STOP_SEC_VAR_NOINIT_8BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define DEM_START_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/*! Engine run-time update poll timer */
DEM_LOCAL VAR(uint32, DEM_VAR_NOINIT)            Dem_Timer_PollRateEngineRunTime;



#define DEM_STOP_SEC_VAR_NOINIT_16BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */


#define DEM_START_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (DEM_FEATURE_NEED_PID4D == STD_ON) || (DEM_FEATURE_NEED_PID4E == STD_ON) || (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
/*! Last received engine run-time */
DEM_LOCAL VAR(uint32, DEM_VAR_NOINIT)            Dem_Timer_LastEngineRunTime;
#endif

#define DEM_STOP_SEC_VAR_NOINIT_32BIT
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT OBJECT FUNCTION DEFINITIONS
 *********************************************************************************************************************/
/* ********************************************************************************************************************
 *  SUBCOMPONENT PUBLIC PROPERTIES DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Queue_PublicProperties
 * \{
 */



/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT PRIVATE FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Queue_Private
 * \{
 */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ********************************************************************************************************************
 *  SUBCOMPONENT API FUNCTION DEFINITIONS
 *********************************************************************************************************************/
#define DEM_START_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */
/*!
 * \addtogroup Dem_Queue_Public
 * \{
 */

                                                                    /* Timer */
/* ------------------------------------------------------------------------- */
/* ****************************************************************************
 % Dem_Timer_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Timer_Init(
  void
  )
{
#if ((DEM_FEATURE_NEED_PID4D == STD_ON) || (DEM_FEATURE_NEED_PID4E == STD_ON))
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
#elif (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
#endif
  {
#if (DEM_FEATURE_NEED_ENGINE_RUN_TIME == STD_ON)
    Dem_Timer_PollRateEngineRunTime = Dem_Cfg_TimerEngineRunTimePollRate();
    Dem_Timer_LastEngineRunTime = DEM_MEM_SET_ENGRUNTIME_INVALID(Dem_Timer_LastEngineRunTime);
#endif
  }
}

/* ****************************************************************************
 % Dem_Timer_MainFunction
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Timer_MainFunction(
  void
  )
{

#if ((DEM_FEATURE_NEED_PID4D == STD_ON) || (DEM_FEATURE_NEED_PID4E == STD_ON))
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
#elif (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
  if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
#endif
  {
#if (DEM_FEATURE_NEED_ENGINE_RUN_TIME == STD_ON)
    --Dem_Timer_PollRateEngineRunTime;

    if (Dem_Timer_PollRateEngineRunTime == 0u)
    {
      Dem_Scheduler_EnableTaskOnce(Dem_Scheduler_Task_EngineRuntime);

      Dem_Timer_PollRateEngineRunTime = Dem_Cfg_TimerEngineRunTimePollRate();
    }
#endif
  }

  Dem_Memory_TimeSeries_Sampler_ProcessTimerTask();
}

                                                                    /* Queue */
/* ------------------------------------------------------------------------- */

/* ****************************************************************************
 % Dem_Queue_Init
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Queue_Init(
  void
  )
{
#if (DEM_FEATURE_NEED_TIME_SERIES == STD_ON)
  Dem_Scheduler_EnableTask(Dem_Scheduler_Task_TimeSeries);
#endif
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
#if (DEM_FEATURE_NEED_OBD_ODOMETER == STD_ON)
    Dem_Scheduler_EnableTask(Dem_Scheduler_Task_Odometer);
#endif
    Dem_Readiness_Completion_TriggerCompletionRecalculationAll();
  }
}

/* ****************************************************************************
 % Dem_Queue_ProcessOdometer
 *****************************************************************************/
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Queue_ProcessOdometer(
  void
  )
{
#if (DEM_FEATURE_NEED_OBD_ODOMETER == STD_ON)
  if (Dem_Cfg_IsObdSupportedInVariant() == TRUE)
  {
    boolean lFetchOdometer;

# if (DEM_FEATURE_NEED_PID21 == STD_ON)
    /* Check if the Odometer must be fetched from application */
    if (DEM_MEM_TEST_ODOMETER_ISINVALID(Dem_Mem_GetOdometerMilOn()) == TRUE)
    {
      lFetchOdometer = TRUE;
    }
    else
# endif
# if (DEM_FEATURE_NEED_PID31 == STD_ON)
    /* Check if the Odometer must be fetched from application */
    if (DEM_MEM_TEST_ODOMETER_ISINVALID(Dem_Mem_GetOdometerAtClear()) == TRUE)
    {
      lFetchOdometer = TRUE;
    }
    else
# endif
# if (DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON)
    /* Check if the Odometer must be fetched from application */
    if ( (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
      && (Dem_Mem_GetPermanentActivationState() == FALSE) )
    {
      lFetchOdometer = TRUE;
    }
    else
# endif
    {
      lFetchOdometer = FALSE;
    }

    if (lFetchOdometer == TRUE)
    {
      uint32 lCurrentOdometer;

      if (Dem_Data_GetObdOdometer(&lCurrentOdometer) == E_OK)                                                                    /* SBSW_DEM_POINTER_FORWARD_STACK */
      { /* valid odometer, proceed */
# if (DEM_FEATURE_NEED_PID21 == STD_ON)
        if (DEM_MEM_TEST_ODOMETER_ISINVALID(Dem_Mem_GetOdometerMilOn()) == TRUE)
        { /* dependent on the current MIL state the absolute odometer or the difference between current and stored odometer must be stored here */
/* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
          Dem_EnterCritical_NonAtomic32bit();

          if (Dem_GlobalState_Indicator_TestMilReportedActive() == TRUE)
          { /* MIL is considered active, so the absolute value must be stored here */
            Dem_Mem_SetOdometerMilOn(lCurrentOdometer);
          }
          else
          { /* MIL is considered inactive, store difference */
            Dem_Mem_SetOdometerMilOn(lCurrentOdometer - DEM_MEM_RESET_ODOMETER_INVALID(Dem_Mem_GetOdometerMilOn()));
          }

          Dem_LeaveCritical_NonAtomic32bit();
/* <<<< -------------------------------- Leave Critical Section: NonAtomic32bit */
        }
# endif
# if (DEM_FEATURE_NEED_PID31 == STD_ON)
        if (DEM_MEM_TEST_ODOMETER_ISINVALID(Dem_Mem_GetOdometerAtClear()) == TRUE)
        { /* no valid odometer received until yet or Clear event occurred, store current odometer */

/* >>>> -------------------------------- Enter Critical Section: NonAtomic32bit */
          Dem_EnterCritical_NonAtomic32bit();
          Dem_Mem_SetOdometerAtClear(lCurrentOdometer);
          Dem_LeaveCritical_NonAtomic32bit();
/* <<<< -------------------------------- Leave Critical Section: NonAtomic32bit */
        }
# endif
# if (DEM_CFG_SUPPORT_PERMANENTDTC_ACTIVATION_DISTANCE == STD_ON)
        if ( (Dem_Cfg_IsObdIIOrObdOnUdsSupportedInVariant() == TRUE)
          && (Dem_Mem_GetPermanentActivationState() == FALSE) )
        {
          uint32 lPermanentActivationDistance;

          lPermanentActivationDistance = Dem_Mem_GetPermanentActivationDistance();

          if (DEM_MEM_TEST_ODOMETER_ISINVALID(lPermanentActivationDistance) == FALSE)
          { /* target odometer value set, check if distance is reached */
            if (lCurrentOdometer > lPermanentActivationDistance)
            {
              (void)Dem_EnablePermanentStorage();
            } /* else: activation distance not yet reached */
          }
          else
          { /* no target odometer set, calculate and store now */
            lPermanentActivationDistance = (uint32)(lCurrentOdometer + Dem_Cfg_GlobalPermanentDtcRelActivationDist());
            Dem_Mem_SetPermanentActivationDistance(lPermanentActivationDistance);
          }
        }
# endif
      }
    }
  }
#endif
}                                                                                                                                /* PRQA S 6010, 6030, 6050, 6080 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_MSR_STCAL, MD_MSR_STMIF */


#if (DEM_FEATURE_NEED_PID4D == STD_ON)
/* ****************************************************************************
 % Dem_Queue_CalculatePid4D
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Queue_CalculatePid4D(
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
  )
{
  if (Dem_GlobalState_Indicator_TestMilReportedActive() == TRUE)
  { /* Always increment if MIL is on and latch/ don't count if MIL is off */
    uint32 lPid4D;  /* intentionally uint32 to avoid an overflow */

    lPid4D = Dem_Mem_GetEngineTimeMilOn();

    if (lPid4D < 0xffffU)
    {
      lPid4D = lPid4D + EngineRunTimeTicks;

      if (lPid4D > 0xffffU)
      { /* latch at 0xffff */
        lPid4D = 0xffffU;
      }

      Dem_Mem_SetEngineTimeMilOn((uint16)lPid4D);
    }
  }
}
#endif

#if (DEM_FEATURE_NEED_PID4E == STD_ON)
/* ****************************************************************************
 % Dem_Queue_CalculatePid4E
 *****************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Queue_CalculatePid4E(
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
  )
{
  uint32 lPid4E; /* intentionally uint32 to avoid an overflow */

  lPid4E = Dem_Mem_GetEngineTimeSinceClear();

  if (lPid4E < 0xffffU)
  {
    lPid4E = lPid4E + EngineRunTimeTicks;

    if (lPid4E > 0xffffU)
    { /* latch at 0xffff */
      lPid4E = 0xffffU;
    }

    Dem_Mem_SetEngineTimeSinceClear((uint16)lPid4E);
  }
}
#endif                                                                                                                               /* PRQA S 6030 */ /* MD_MSR_STCYC */

/* ****************************************************************************
 % Dem_Queue_ProcessAgingTimer
 *****************************************************************************/
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Queue_ProcessAgingTimer(                                                                                                     /* PRQA S 3219 */ /* MD_DEM_14.1_CONFIGURATION_SWITCH */
  CONST(uint32, AUTOMATIC)  EngineRunTimeTicks
  )
{
#if (DEM_CFG_SUPPORT_WWHOBD == STD_ON)
  uint32 lCumulativeEngineRuntime;
  Dem_Memory_MemEntryIterType MemEntryIter;

  lCumulativeEngineRuntime = Dem_Mem_GetCumulativeEngRuntime();
  lCumulativeEngineRuntime = lCumulativeEngineRuntime + EngineRunTimeTicks;
  Dem_Mem_SetCumulativeEngRuntime(lCumulativeEngineRuntime);

  /* Check for all OBD events in primary memory if they are currently in aging process and if aging time has exceeded */

  for (Dem_Memory_MemEntry_IterInit(DEM_CFG_MEMORYID_PRIMARY, &MemEntryIter);                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Memory_MemEntry_IterExists(&MemEntryIter) == TRUE;                                                                    /* SBSW_DEM_CALL_ITERATOR_POINTER */
       Dem_Memory_MemEntry_IterNext(&MemEntryIter))                                                                              /* SBSW_DEM_CALL_ITERATOR_POINTER */
  {
    Dem_Cfg_MemoryEntryHandleType lMemoryEntryId;
    Dem_Cfg_MemoryIndexType lMemoryIndex;
    Dem_EventIdType lEventId;

    lMemoryIndex = Dem_Memory_MemEntry_IterGetMemoryIndex(&MemEntryIter);                                                        /* SBSW_DEM_CALL_ITERATOR_POINTER */
    lMemoryEntryId = Dem_MemoryEntry_GetId(lMemoryIndex);
    lEventId = Dem_MemoryEntry_GetEventId(lMemoryEntryId);
    if (Dem_Event_TestValidHandle(lEventId) == TRUE)
    {
      if ( (Dem_Core_DTC_IsObdRelated(lEventId) == TRUE)
        && (Dem_Cfg_EventSupportAging(lEventId) == TRUE) )                                                                       
      {
        if (Dem_DTCInternalStatus_GetStoredStatus(Dem_DTC_GetInternalStatus(lEventId)) == Dem_DTCInternalStatus_StoredStatus_Aging)
        { /* Event is currently aging */
          if (lCumulativeEngineRuntime >= Dem_MemoryEntry_GetAgingTimer(lMemoryEntryId))
          { /* Aging time reached, so this event has aged */
            Dem_Event_DataUpdateStart(lEventId);
            Dem_DTC_Aged(lEventId, lMemoryIndex,FALSE);
            Dem_Event_DataUpdateFinish(lEventId);
          }
        }
      }
    }
  }
#else
  DEM_IGNORE_UNUSED_CONST_ARGUMENT(EngineRunTimeTicks)                                                                           /* PRQA S 3112 */ /* MD_DEM_14.2 */
#endif
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/* ****************************************************************************
 % Dem_Queue_ProcessEngineRuntime
 *****************************************************************************/
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
DEM_LOCAL_INLINE FUNC(void, DEM_CODE)
Dem_Queue_ProcessEngineRuntime(
  void
)
{
#if (DEM_FEATURE_NEED_ENGINE_RUN_TIME == STD_ON)
# if (DEM_DEV_RUNTIME_CHECKS == STD_ON)
    if ((Dem_Cfg_ObdEngineRunTime() >= Dem_Cfg_GlobalDataElementCount())
      || (Dem_Cfg_DataIsStoredInNV(Dem_Cfg_ObdEngineRunTime()) == FALSE)
      || (Dem_Cfg_DataCallbackType(Dem_Cfg_ObdEngineRunTime()) != DEM_CFG_DATA_FROM_CS_PORT)
      || (Dem_Cfg_DataCbkRead(Dem_Cfg_ObdEngineRunTime()) == NULL_PTR)
      || (Dem_Cfg_DataSize(Dem_Cfg_ObdEngineRunTime()) != 4u))
    {
      Dem_Error_RunTimeCheckFailed(DEM_QUEUE_IMPLEMENTATION_FILENAME, __LINE__);                                                 /* SBSW_DEM_POINTER_RUNTIME_CHECK */
    }
    else
# endif
    {
      uint8 lBuffer[4];
      if ((*((Dem_ReadDataUint8FPtrType)Dem_Cfg_DataCbkRead(Dem_Cfg_ObdEngineRunTime())))(&lBuffer[0]) == E_OK)               /* PRQA S 0313 */ /* MD_DEM_11.1 */ /* SBSW_DEM_CALL_LOCAL_DESTINATIONBUFFER */
      { /* calculate passed engine run time (resolution 1min/bit)*/
        uint32 lTotalEngineRunTime;
        uint32 lEngineRunTimeDelta;

        lTotalEngineRunTime = Dem_Make32Bit(lBuffer[0], lBuffer[1], lBuffer[2], lBuffer[3]);

        if (DEM_MEM_TEST_ENGRUNTIME_ISINVALID(Dem_Timer_LastEngineRunTime) == TRUE)
        { /* initial reception of the engine runtime signal in this power cycle */
          lEngineRunTimeDelta = 0u;
        }
        else
          if (Dem_Timer_LastEngineRunTime <= lTotalEngineRunTime)
          { /* same engine run, delta is [Dem_Timer_LastEngineRunTime, lNewEngineRunTime] */
            lEngineRunTimeDelta = lTotalEngineRunTime - Dem_Timer_LastEngineRunTime;
          }
          else
          { /* else: engine restart, delta is [0, lNewEngineRunTime] */
            lEngineRunTimeDelta = lTotalEngineRunTime;
          }

        if (lEngineRunTimeDelta != 0u)
        { /* one or more engine run time minutes have passed, process dependent active timers */
# if (DEM_FEATURE_NEED_PID4D == STD_ON)
          Dem_Queue_CalculatePid4D(lEngineRunTimeDelta);
# endif
# if (DEM_FEATURE_NEED_PID4E == STD_ON)
          Dem_Queue_CalculatePid4E(lEngineRunTimeDelta);
# endif


          Dem_GlobalState_ActivationMode_ProcessEngineRunTime(lEngineRunTimeDelta);
          if (Dem_Cfg_IsWwhObdSupportedInVariant() == TRUE)
          {
            Dem_Queue_ProcessAgingTimer(lEngineRunTimeDelta);
          }
      }
        Dem_Timer_LastEngineRunTime = lTotalEngineRunTime;
    }
      else
      {
        /* Could not read the Engine Run Time from the application - Try again next cycle */
        Dem_Scheduler_EnableTaskOnce(Dem_Scheduler_Task_EngineRuntime);
      }
    }
#endif /* DEM_FEATURE_NEED_ENGINE_RUN_TIME == STD_ON */
}                                                                                                                                /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/*!
 * \}
 */
#define DEM_STOP_SEC_CODE
#include "MemMap.h"                                                                                                              /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* DEM_QUEUE_IMPLEMENTATION_H */

/*!
 * \}
 */
/* ********************************************************************************************************************
 *  END OF FILE: Dem_Queue_Implementation.h
 *********************************************************************************************************************/
