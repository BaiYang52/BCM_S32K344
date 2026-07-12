/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *
 *                 This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                 Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                 All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  LICENSE
 *  -------------------------------------------------------------------------------------------------------------------
 *            Module: Os
 *           Program: VAG AR4 (MSR_Vag_SLP5)
 *          Customer: Faurecia Clarion Electronics US
 *       Expiry Date: Not restricted
 *  Ordered Derivat.: S32K314NHT1VPBST
 *    License Scope : The usage is restricted to CBD2300384_D00
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *              File: Os_Trace_Lcfg.h
 *   Generation Time: 2026-07-12 12:56:23
 *           Project: BCM_S32K344 - Version 1.0
 *          Delivery: CBD2300384_D00
 *      Tool Version: DaVinci Configurator Classic (beta) 5.26.40 SP3
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 ! BETA VERSION                                                                                                       !
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 ! This version of DaVinci Configurator Classic and/or the related Basic Software Package is BETA software.               !
 ! BETA Software is basically operable, but not sufficiently tested, verified and/or qualified for use in series      !
 ! production and/or in vehicles operating on public or non-public roads.                                             !
 ! In particular, without limitation, BETA Software may cause unpredictable ECU behavior, may not provide all         !
 ! functions necessary for use in series production and/or may not comply with quality requirements which are         !
 ! necessary according to the state of the art. BETA Software must not be used in series production.                  !
 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
**********************************************************************************************************************/

#ifndef OS_TRACE_LCFG_H
# define OS_TRACE_LCFG_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Trace_Types.h"

/* Os kernel module dependencies */

/* Os hal dependencies */

/* User file includes */


/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/*! \brief    Represents the identification of OS services for an external debugger supporting the
 *            OSEK Runtime Interface (ORTI) or AUTOSAR Runtime Interface (ARTI).
 *  \details  Start and end of a service are coded in the least significant bit according to the
 *            ORTI standard. Therefore, all IDs are even.
 *            We trace the service in a uint8. Therefore, at most 127 services are possible to
 *            trace currently.
 */
typedef enum
{
  OsOrtiApiIdGetApplicationID = 0x02u, /*!< GetApplicationID() */
  OsOrtiApiIdGetISRID = 0x04u, /*!< GetISRID() */
  OsOrtiApiIdCallTrustedFunction = 0x06u, /*!< CallTrustedFunction() */
  OsOrtiApiIdCheckISRMemoryAccess = 0x08u, /*!< CheckISRMemoryAccess() */
  OsOrtiApiIdCheckTaskMemoryAccess = 0x0Au, /*!< CheckTaskMemoryAccess() */
  OsOrtiApiIdCheckObjectAccess = 0x0Cu, /*!< CheckObjectAccess() */
  OsOrtiApiIdCheckObjectOwnership = 0x0Eu, /*!< CheckObjectOwnership() */
  OsOrtiApiIdStartScheduleTableRel = 0x10u, /*!< StartScheduleTableRel() */
  OsOrtiApiIdStartScheduleTableAbs = 0x12u, /*!< StartScheduleTableAbs() */
  OsOrtiApiIdStopScheduleTable = 0x14u, /*!< StopScheduleTable() */
  OsOrtiApiIdNextScheduleTable = 0x16u, /*!< NextScheduleTable() */
  OsOrtiApiIdStartScheduleTableSynchron = 0x18u, /*!< StartScheduleTableSynchron() */
  OsOrtiApiIdSyncScheduleTable = 0x1Au, /*!< SyncScheduleTable() */
  OsOrtiApiIdSetScheduleTableAsync = 0x1Cu, /*!< SetScheduleTableAsync() */
  OsOrtiApiIdGetScheduleTableStatus = 0x1Eu, /*!< GetScheduleTableStatus() */
  OsOrtiApiIdIncrementCounter = 0x20u, /*!< IncrementCounter() */
  OsOrtiApiIdGetCounterValue = 0x22u, /*!< GetCounterValue() */
  OsOrtiApiIdGetElapsedValue = 0x24u, /*!< GetElapsedValue() */
  OsOrtiApiIdTerminateApplication = 0x26u, /*!< TerminateApplication() */
  OsOrtiApiIdAllowAccess = 0x28u, /*!< AllowAccess() */
  OsOrtiApiIdGetApplicationState = 0x2Au, /*!< GetApplicationState() */
  OsOrtiApiIdGetNumberOfActivatedCores = 0x2Cu, /*!< GetNumberOfActivatedCores() */
  OsOrtiApiIdGetCoreID = 0x2Eu, /*!< GetCoreID() */
  OsOrtiApiIdStartCore = 0x30u, /*!< StartCore() */
  OsOrtiApiIdStartNonAutosarCore = 0x32u, /*!< StartNonAutosarCore() */
  OsOrtiApiIdGetSpinlock = 0x34u, /*!< GetSpinlock() */
  OsOrtiApiIdReleaseSpinlock = 0x36u, /*!< ReleaseSpinlock() */
  OsOrtiApiIdTryToGetSpinlock = 0x38u, /*!< TryToGetSpinlock() */
  OsOrtiApiIdShutdownAllCores = 0x3Au, /*!< ShutdownAllCores() */
  OsOrtiApiIdControlIdle = 0x3Cu, /*!< ControlIdle() */
  OsOrtiApiIdIocSend = 0x3Eu, /*!< IocSend() */
  OsOrtiApiIdIocWrite = 0x40u, /*!< IocWrite() */
  OsOrtiApiIdIocSendGroup = 0x42u, /*!< IocSendGroup() */
  OsOrtiApiIdIocWriteGroup = 0x44u, /*!< IocWriteGroup() */
  OsOrtiApiIdIocReceive = 0x46u, /*!< IocReceive() */
  OsOrtiApiIdIocRead = 0x48u, /*!< IocRead() */
  OsOrtiApiIdIocReceiveGroup = 0x4Au, /*!< IocReceiveGroup() */
  OsOrtiApiIdIocReadGroup = 0x4Cu, /*!< IocReadGroup() */
  OsOrtiApiIdIocEmptyQueue = 0x4Eu, /*!< IocEmptyQueue() */
  OsOrtiApiIdGetCurrentApplicationID = 0x50u, /*!< GetCurrentApplicationID() */
  OsOrtiApiIdCallNonTrustedFunction = 0x52u, /*!< CallNonTrustedFunction() */
  OsOrtiApiIdGetTaskStackUsage = 0x54u, /*!< Os_GetTaskStackUsage() */
  OsOrtiApiIdGetISRStackUsage = 0x56u, /*!< Os_GetISRStackUsage() */
  OsOrtiApiIdGetKernelStackUsage = 0x58u, /*!< Os_GetKernelStackUsage() */
  OsOrtiApiIdGetStartupHookStackUsage = 0x5Au, /*!< Os_GetStartupHookStackUsage() */
  OsOrtiApiIdGetErrorHookStackUsage = 0x5Cu, /*!< Os_GetErrorHookStackUsage() */
  OsOrtiApiIdGetShutdownHookStackUsage = 0x5Eu, /*!< Os_GetShutdownHookStackUsage() */
  OsOrtiApiIdGetProtectionHookStackUsage = 0x60u, /*!< Os_GetProtectionHookStackUsage() */
  OsOrtiApiIdGetSemaphore = 0x62u, /*!< GetSemaphore() */
  OsOrtiApiIdReleaseSemaphore = 0x64u, /*!< ReleaseSemaphore() */
  OsOrtiApiIdStartOS = 0x66u, /*!< StartOS() */
  OsOrtiApiIdGetActiveApplicationMode = 0x68u, /*!< GetActiveApplicationMode() */
  OsOrtiApiIdDisableAllInterrupts = 0x6Au, /*!< DisableAllInterrupts() */
  OsOrtiApiIdEnableAllInterrupts = 0x6Cu, /*!< EnableAllInterrupts() */
  OsOrtiApiIdSuspendAllInterrupts = 0x6Eu, /*!< SuspendAllInterrupts() */
  OsOrtiApiIdResumeAllInterrupts = 0x70u, /*!< ResumeAllInterrupts() */
  OsOrtiApiIdSuspendOSInterrupts = 0x72u, /*!< SuspendOSInterrupts() */
  OsOrtiApiIdResumeOSInterrupts = 0x74u, /*!< ResumeOSInterrupts() */
  OsOrtiApiIdActivateTask = 0x76u, /*!< ActivateTask() */
  OsOrtiApiIdTerminateTask = 0x78u, /*!< TerminateTask() */
  OsOrtiApiIdChainTask = 0x7Au, /*!< ChainTask() */
  OsOrtiApiIdGetTaskID = 0x7Cu, /*!< GetTaskID() */
  OsOrtiApiIdGetTaskState = 0x7Eu, /*!< GetTaskState() */
  OsOrtiApiIdSchedule = 0x80u, /*!< Schedule() */
  OsOrtiApiIdSetEvent = 0x82u, /*!< SetEvent() */
  OsOrtiApiIdClearEvent = 0x84u, /*!< ClearEvent() */
  OsOrtiApiIdGetEvent = 0x86u, /*!< GetEvent() */
  OsOrtiApiIdWaitEvent = 0x88u, /*!< WaitEvent() */
  OsOrtiApiIdGetAlarmBase = 0x8Au, /*!< GetAlarmBase() */
  OsOrtiApiIdGetAlarm = 0x8Cu, /*!< GetAlarm() */
  OsOrtiApiIdSetRelAlarm = 0x8Eu, /*!< SetRelAlarm() */
  OsOrtiApiIdSetAbsAlarm = 0x90u, /*!< SetAbsAlarm() */
  OsOrtiApiIdCancelAlarm = 0x92u, /*!< CancelAlarm() */
  OsOrtiApiIdGetResource = 0x94u, /*!< GetResource() */
  OsOrtiApiIdReleaseResource = 0x96u, /*!< ReleaseResource() */
  OsOrtiApiIdGetVersionInfo = 0x98u, /*!< GetVersionInfo() */
  OsOrtiApiIdEnterPreStartTask = 0x9Eu, /*!< EnterPreStartTask() */
  OsOrtiApiIdShutdownOS = 0xA0u, /*!< ShutdownOS() */
  OsOrtiApiIdGetUnhandledIrq = 0xA2u, /*!< GetUnhandledIrq() */
  OsOrtiApiIdGetUnhandledExc = 0xA4u, /*!< GetUnhandledExc() */
  OsOrtiApiIdDisableOSInterrupts = 0xA6u, /*!< DisableOSInterrupts() */
  OsOrtiApiIdEnableOSInterrupts = 0xA8u, /*!< EnableOSInterrupts() */
  OsOrtiApiIdReadPeripheral8 = 0xAAu, /*!< Os_ReadPeripheral8() */
  OsOrtiApiIdReadPeripheral16 = 0xACu, /*!< Os_ReadPeripheral16() */
  OsOrtiApiIdReadPeripheral32 = 0xAEu, /*!< Os_ReadPeripheral32() */
  OsOrtiApiIdWritePeripheral8 = 0xB0u, /*!< Os_WritePeripheral8() */
  OsOrtiApiIdWritePeripheral16 = 0xB2u, /*!< Os_WritePeripheral16() */
  OsOrtiApiIdWritePeripheral32 = 0xB4u, /*!< Os_WritePeripheral32() */
  OsOrtiApiIdModifyPeripheral8 = 0xB6u, /*!< Os_ModifyPeripheral8() */
  OsOrtiApiIdModifyPeripheral16 = 0xB8u, /*!< Os_ModifyPeripheral16() */
  OsOrtiApiIdModifyPeripheral32 = 0xBAu, /*!< Os_ModifyPeripheral32() */
  OsOrtiApiIdGetDetailedError = 0xBCu, /*!< GetDetailedError() */
  OsOrtiApiIdOSErrorGetServiceId = 0xBEu, /*!< OSErrorGetServiceId() */
  OsOrtiApiIdDisableInterruptSource = 0xC0u, /*!< Os_DisableInterruptSource() */
  OsOrtiApiIdEnableInterruptSource = 0xC2u, /*!< Os_EnableInterruptSource() */
  OsOrtiApiIdClearPendingInterrupt = 0xC4u, /*!< Os_ClearPendingInterrupt() */
  OsOrtiApiIdIsInterruptSourceEnabled = 0xC6u, /*!< Os_IsInterruptSourceEnabled() */
  OsOrtiApiIdIsInterruptPending = 0xC8u, /*!< Os_IsInterruptPending() */
  OsOrtiApiIdCallFastTrustedFunction = 0xCAu, /*!< Os_CallFastTrustedFunction() */
  OsOrtiApiIdBarrierSynchronize = 0xCCu, /*!< Os_BarrierSynchronize() */
  OsOrtiApiIdGetExceptionContext = 0xCEu, /*!< Os_GetExceptionContext() */
  OsOrtiApiIdSetExceptionContext = 0xD0u, /*!< Os_SetExceptionContext() */
  OsOrtiApiIdInitialEnableInterruptSources = 0xD2u, /*!< Os_InitialEnableInterruptSources() */
  OsOrtiApiIdGetCoreStartState = 0xD4u, /*!< Os_GetCoreStartState() */
  OsOrtiApiIdGetExceptionAddress = 0xD6u, /*!< Os_GetExceptionAddress() */
  OsOrtiApiIdGetNonTrustedFunctionStackUsage = 0xD8u, /*!< Os_GetNonTrustedFunctionStackUsage() */
  OsOrtiApiIdEnableInterruptsPreStart = 0xDAu /*!< Os_EnableInterruptsPreStart() */
} OsOrtiApiIdType;


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define OS_START_SEC_PUBLIC_CORE0_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define OS_STOP_SEC_PUBLIC_CORE0_VAR_NOINIT_UNSPECIFIED
# include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  GLOBAL CONSTANT DATA PROTOTYPES
 *********************************************************************************************************************/

# define OS_START_SEC_CONST_UNSPECIFIED
# include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define OS_STOP_SEC_CONST_UNSPECIFIED
# include "Os_MemMap_OsSections.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/


#endif /* OS_TRACE_LCFG_H */

/**********************************************************************************************************************
 *  END OF FILE: Os_Trace_Lcfg.h
 *********************************************************************************************************************/
