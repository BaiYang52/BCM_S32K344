/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file  File:  BrsMain.h
 *      Project:  Vector Basic Runtime System
 *       Module:  BrsMain
 *
 *  \brief Description:  Main header of BRS contains
 *                       - Main function (called from StartUpCode and calls stack entry EcuM_Init())
 *                       - Call of BrsHw HW initialization routines
 *                       - 1ms handler, with support of cycle functions with several cycle times
 *                       - Default_Init_Task, Main and Background Task which can be used by the operating system
 *
 *  \attention Please note:
 *    The demo and example programs only show special aspects of the software. With regard to the fact
 *    that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *    expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Description
 *  --------  ----------  ------  -------------------------------------------------------------------------------------
 *  01.00.00  2018-03-20  visbwa  Initial creation of new BRS code basis
 *            2018-03-27  visbwa  Renamed BrsAsrStartup into BrsStartup, added version check, reworked includes,
 *                                added example code text in BrsMainStartup.c
 *            2018-05-08  visbwa  Introduced support for Generic Testsuite (BRS_ENABLE_TESTSUITE_SUPPORT),
 *                                removed support for all other testsuites,
 *                                introduced BRS_ENABLE_1MS_HANDLER and brsMain_CallCounter1sec
 *            2018-05-16  visbwa  Added extern declaration of brsHwZeroRamAreaSet and brsHwInitRamAreaSet in
 *                                BrsMainStartup.c
 *  01.00.01  2018-06-22  visbwa  Added CAN stack exclusive area workaround from former BswInit_Callout_Stubs.c
 *  01.00.02  2018-06-25  visbwa  Added configuration check for OS_MS2TICKS_SystemTimer into BrsMain.h
 *  01.00.03  2018-07-02  visces  Removed outdated brsHwZeroRamAreaSet and brsHwInitRamAreaSet in BrsMainStartup.c
 *  01.00.04  2018-07-18  visbwa  Moved BrsHwDisableInterruptAtPowerOn() from BrsMainInit() into main()
 *                                (OS specifies, this has to be called before Os_Init() and for every Core)
 *  01.00.05  2018-07-26  visbwa  No changes in sourcecode, only within ALM package
 *  01.00.06  2018-07-30  visbwa  Encapsuled call of BrsHwDisableInterruptAtPowerOn() with usecase != VTT
 *  01.01.00  2018-08-16  visbwa  Replaced include of RamInitSections.h by vLinkGen_RamInitSections.h in
 *                                BrsMainStartup.c to support vLinkGen
 *  01.02.00  2018-10-04  visbwa  No changes in sourcecode, only within ALM package
 *  01.03.00  2018-10-08  visbwa  Changed include of BrsCfg.h into vBrsCfg.h (vBaseEnv 1.1.0)
 *  01.03.01  2018-10-10  visbwa  Moved BrsTestsuiteInit() from BrsMainInit() into Default_Init_Task(),
 *                                removed usage of BRS_ENABLE_HLPTEST_SUPPORT (generic Testsuite concept!)
 *  01.03.02  2018-11-05  visbwa  Fixed include order (Can.h previous to BrsTestsuite.h)
 *  01.03.03  2018-11-13  visbwa  Implemented Default_Init_Tasks for projects with up to 6 Cores,
 *                                fixed VTT support within IdleTasks (CANoeAPI_ConsumeTicks)
 *  01.04.00  2018-11-21  visbwa  Added testsuite callouts into every function
 *  01.04.01  2018-11-30  visbwa  Removed include of Rte_Main.h (Rte_Start() not called any more)
 *  01.05.00  2018-12-21  visbwa  Added call of new API Os_InitialEnableInterruptSources() within every core
 *                                Default_Init_Task
 *  01.05.01  2019-03-06  visbwa  Added hint for DrvCan using infix, added wrapper for Can_30_Mcan
 *  01.06.00  2019-05-06  visbwa  Support for vLinkGen 1.04; renamed struct names of vlinkGen areas in BrsMainStartup.c
 *                                - added support for zero init blocks
 *                                - renamed Brs_RamZeroInit() to Brs_MemoryZeroInit() and Brs_RomToRamInit() to
 *                                  Brs_MemoryInit()
 *                                - check for missing hard reset detection
 *  01.06.01  2019-05-10  visbwa  Changed include of vLinkGen header to support new vLinkGen file structure in
 *                                BrsMainStartup.c
 *  01.06.02  2019-06-13  vismaa  Added additional Default_Init_Task_Core6
 *  01.06.03  2019-06-26  visbwa  Moved call of BrsMainInit() previous to call of Os_Init()
 *  01.07.00  2019-07-02  visbwa  Enhanced FBL support (encapsulations, call of fblmain())
 *  01.07.01  2019-07-22  visbwa  Added check for VLINKGEN_ZERO_INIT_BLOCK_COUNT_HARD_RESET in BrsMainStartup.c
 *  01.07.02  2019-07-29  visbwa  Added implementation of BrsMainExceptionStartup() for calls from assembly StartUpCode
 *  01.08.00  2019-08-08  visbwa  Implemented support for different FBL usecases Legacy, WithoutOSScheduling and
 *                                OSScheduling, added BrsMainExceptionHandler module code for BrsMainStartup, changed
 *                                FBL call from fblmain() to FblMain() for legacy mode, removed include of fbl_main.h,
 *                                added InitPattern parameter to Brs_MemoryZeroInit() and added the new configuration
 *                                values for the init pattern to the depending calls in BrsMainStartup.c,
 *                                added sample implementation for Hard-reset blocks and areas in BrsMainStartup.c
 *  01.09.00  2019-08-28  visbwa  Changed encapsulation of task implementations (not used for FBL usecases Legacy and
 *                                WithoutOSScheduling any more)
 *            2019-10-07  visbwa  Added support of platform individual ZeroInit function BrsHw_AsmMemoryZeroInitLoop()
 *                                in Brs_MemoryZeroInit() in BrsMainStartup.c
 *            2019-10-09  visbwa  Enhanced support for VTT platform
 *  01.09.01  2019-11-04  visbwa  Changed include of Can.h into CanIf.h (get rid of DrvCan with infix (Can_30_Mcan.h)),
 *                                enhanced MemMap + SchM exclusive area abstraction for Mcan driver (uses infixes)
 *  01.09.02  2020-11-04  visbwa  No changes in sourcecode, only within ALM package
 *  01.10.00  2019-11-25  visbwa  Implemented Default_Init_Task_Trusted and Default_Init_Task_Corex_Trusted to allow
 *                                the SafeContext partitioning UseCase
 *            2019-11-27  visbwa  Changed include structure for vBrsCfg.h in BrsMain.h (does now also exist for VTT)
 *            2019-11-29  visbwa  Enhanced encapsulation with _MICROSOFT_C_VTT_ for DualTarget UseCase (no BrsHw!)
 *            2019-12-04  visbwa  Fixed encapsulation of DrvCan parts
 *  01.11.00  2020-02-11  visbwa  Support for UseCase HSM (BRS_ENABLE_HSM_SUPPORT, no EcuM),
 *                                added filtering of MultiCore parts through C_BRSASR_USECASE_SINGLECORE_COMMENT,
 *                                support for generated DrvCan macros to support modules with infix names,
 *                                removed error directive at usage of BrsHwGetResetReason() in BrsMainStartup.c
 *  01.12.00  2020-03-12  visbwa  Encapsulation of SchM implementation with MemMap defines for UseCase FBL,
 *                                encapsulation of struct brsMain_Cyclic_Callbacks with MemMap and 1ms-handler defines
 *  01.12.01  2020-04-22  visbwa  Added conditional call of BrsHwDisableEccErrorReporting() for FBL UseCase
 *  02.00.00  2020-05-29  visbwa  Major updates for vBaseEnv 2.0.0
 *                                - added BrsMainTogglePin() (moved from BrsHw.c BrsHwTogglePin())
 *                                - introduction of BrsMain_Callout_Stubs.c
 *                                - updated vLinkGen define and struct names to new vLinkGen 2.0 naming schema,
 *                                  added version check for vLinkGen version in BrsMainStartup.c
 *  02.01.00  2020-06-29  visbwa  Added support for preferred PLL and Watchdog init in BrsMainInit() and
 *                                BrsMain_MemoryInit_StageOne_Hook(), reduced calls of BrsHw_GetCore() inside
 *                                Brs_PreMainStartup() of BrsMainStartup.c to a single call
 *  02.01.01  2020-07-24  visbwa  Soft increase for FBL_Legacy support
 *  02.02.00  2020-08-10  visbwa  Support for FBL Legacy UseCase w/o OS, usage of new FBL UseCase defines;
 *                                MemMap encapsulation of BrsMainExceptionHandler(), enabled 1ms handler also for FBL,
 *                                BrsMainCyclic1ms() usable from extern, removed usage of BrsAsrApplCanInterruptLockCtr
 *                                for FBL (FBL is always in polling mode),
 *                                added BrsMain_SoftwareResetECU_Hook() in BrsMain_Callout_Stubs.c,
 *                                encapsuled memPtr in Brs_MemoryZeroInit() of BrsMainStartup.c (compiler warnings)
 *  02.02.01  2020-10-29  visbwa  No changes in sourcecode, only within ALM package
 *  02.02.02  2020-10-29  visbwa  No changes in sourcecode, only within ALM package
 *  02.02.03  2020-10-30  visbwa  Added include of Os.h for VTT (usage of ShutdownOS()), removed AUTHOR IDENTITY
 *  02.02.04  2020-12-07  visbwa  Increased for loop variable in Brs_MemoryZeroInit() and Brs_MemoryInit() from uint8
 *                                to uint32 (depending datatypes are increased in vLinkGen) in BrsMainStartup.c
 *  02.02.05  2021-01-14  visbwa  Adapted main() to also call Os_Init() for FBL with VTT
 *  02.03.00  2021-03-17  visbwa  Moved revision history of all .c and .h files into BrsMain.h, elimination of
 *                                BrsMainStartup.h, added support for new OS UseCase INTERRUPT_ONLY and call of
 *                                BrsMainExceptionHandler() at the end of main() for IllegalReturnFromMain
 *  02.03.01  2021-03-22  visbwa  Added definition of _BRSMAIN_C_ in BrsMain.c, to solve issues in vBrsCfg.h if DIO
 *                                channels are used for TogglePin functionality
 *  02.04.00  2021-04-07  visbwa  Added implementation of BrsMainSuspendAllInterrupts() and
 *                                BrsMainResumeAllInterrupts() for new OS UseCase INTERRUPT_ONLY
 *  02.04.01  2021-04-13  visbwa  Encapsulation of int main(void) declaration to not appear for VTT, as VTT expects
 *                                void main(void)
 *  02.05.00  2021-07-02  visbwa  Added support for upcoming Brs_Template 1.03:
 *                                - call of BrsHw_ExceptionTable_Init() in main()
 *                                - introduction of BrsMain_Types.h
 *                                - new MainExceptionHandler ErrorCode kBrsInvalidCoreConfig
 *  02.05.01  2021-07-19  visbwa  ESCAN00109765 - Fixed VTT encapsulation in main() to solve compiler error
 *  02.05.02  2021-08-02  visbwa  Fixed BrsHw_ExceptionTable_Init() call in main() for FBL Ram exception table
 *  02.05.03  2021-08-02  visbwa  Fixed BRSMAIN_BUGFIX_VERSION in several files of the package
 *  02.06.00  2021-08-05  visjhr  In BrsMain_Types.h: Renamed StartupStackLabel -> StartupStackEndLabel, added
 *                                StartupStackSize (HALBE-5072)
 *            2021-10-11  visbwa  Introduction of Brs_ApplicationEntry(), BRS_FIRST_INSTANCE usage and MemMap concept
 *                                in BrsMainStartup.c; brsMain_ResetReasonType in BrsMain_Types.h;
 *                                moved BrsMainExceptionStartup() from BrsMain.c into BrsMainStartup.c and map it into
 *                                StartupCode section; swap order of HardResetOnly and InitOne in BrsMainStartup.c,
 *                                moved preferred PLL and Watchdog init from InitOne hook into HardResetOnly hook to
 *                                fit with vLinkGen 2.04; several small adaptions to reduce compiler warnings;
 *                                introduction of Brs_memMap.h
 *  02.06.01  2021-10-20  visbwa  Brs_memMap.h fix for GHS
 *  02.06.02  2021-10-28  visbwa  Encapsulation of _Brs_ExcVectRam_START extern declaration, for better VTT support
 *  02.07.00  2021-11-09  visbwa  Encapsulation of BrsHw_ExceptionTable_Init() in main() for FBL UseCase with new
 *                                parameter BRS_FBL_EXCEPTIONTABLE_IN_RAM (vBRSFblSupportExceptionTableInRam);
 *                                use DrvCan MemMap for mapping of BrsAsrApplCanInterruptLockCtr and module specific
 *                                MemMap sections for exclusive area implementation in BrsMain.c (including CAN driver
 *                                name infix wrapper, provided by vBRS generator in vBrsCfg.h)
 *  02.07.01  2021-11-25  virjas  Added missing function prototypes
 *  02.07.02  2021-12-02  visbwa  Added support for Arm6/TI and warning for GccGnu/Generic compiler in Brs_MemMap.h
 *  02.08.00  2022-01-12  visbwa  Changed call of BrsHw_ExceptionTable_Init() in main() for FBL UseCase
 *                                (_Brs_ExcVectRam_START is now generated into BrsMain_CoreConfig),
 *                                added a check for the vBrs generator version (BRS_GENERATED_HW_CONFIG_VERSION)
 *  02.08.01  2022-02-04  visrgm  Added support for Renesas compiler in Brs_MemMap.h
 *  02.08.02  2022-02-18  visbwa  Enhanced encapsulation of uint32 i in main() to prevent compiler warnings
 *  02.08.03  2022-02-18  visbwa  Added encapsulation to BrsMainExceptionStartup() (1st execution instance)
 *  02.08.04  2022-03-23  visbpz  Added support for Microchip compiler in Brs_MemMap.h
 *  02.08.05  2022-03-23  visbpz  Incrementing software version
 *  02.08.06  2022-03-30  visbwa  Added _BRSMAINSTARTUP_C_ to BrsMainStartup.c
 *  02.08.07  2022-04-06  visjhr  Added Default_Init_Task_Core0 and Default_Init_Task_Core0_Trusted (HALBE-6489)
 *  02.08.08  2022-05-12  visbwa  Added functionality to override BRSMAIN_CYCLIC_MAX_CALLBACKS in BrsMain.c
 *  02.09.00  2022-05-05  visbwa  Added sections for mapping of exception table const/code in Brs_MemMap.h
 *            2022-05-30  visbwa  Added support for LlvmDiab/LlvmTI compiler and fixed support for GccGnu (to allow at
 *                                least correct mapping for all brsExcVect) in Brs_MemMap.h
 *  02.09.01  2022-05-31  visbwa  Fixed LlvmDiab support in Brs_MemMap.h
 *  02.09.02  2022-07-04  visbwa  Removed call of BrsHwDisableEccErrorReporting() in BrsMainInit() (is called from FBL)
 *  02.09.03  2022-07-07  visbwa  Readded call of BrsHwDisableEccErrorReporting() and adapted encapsulation of BrsHw
 *                                init function calls for FBL UseCases in BrsMainInit()
 *  02.10.00  2022-07-14  visbwa  Changes in ALM content settings only (usage type partly set to external)
 *  02.10.01  2022-08-04  visbwa  Added LlvmHightec support in Brs_MemMap.h
 *  02.11.00  2022-12-07  visbwa  Removed support for BrsHw packages with BRSHW_SOURCECODE_TEMPLATE_VERSION < 0x0103u;
 *                                brsMain_ResetReason in Brs_PreMainStartup() only set by initCore;
 *                                introduction of BrsMain_Appl.c, call of BrsMain_Appl_Init() from BrsMainInit(),
 *                                moved UserDefined ExclusiveArea handling for CAN channels into BrsMain_Appl.c,
 *                                implementation of FBL Instance Presence Pattern in BrsMain_Appl.c;
 *                                support for Ansi/Gnu compiler with VTT/CANoeEmu in Brs_MemMap.h
 *            2022-12-13  visdri  Fixed LlvmTI compiler support in Brs_MemMap.h
 *  02.11.01  2023-01-11  visbwa  Fixed single core platform support in Brs_PreMainStartup() of BrsMainStartup.c
 *  02.11.02  2023-01-11  visbwa  Fixed MemMap defines for initialized variables in BrsMainTogglePin()
 *  02.12.00  2023-02-16  visjhr  Removed unused IdleTask{_CoreX},removed filtering ifdefs for
 *                                C_BRSASR_USECASE_STARTAPPLICATION_COMMENT and C_BSWMODULE_ASR_* (HALBE-8511)
 *            2023-02-24  visbwa  Introduction of BRS_MEMMAP_INLINE_ASSEMBLER_USED in Brs_MemMap.h for GHS compiler
 *            2023-02-27  visbwa  Added call to BrsHwApplicationInit() at beginning of Brs_ApplicationEntry() in
 *                                BrsMainStartup.c, to support platform specific initialization per execution instance
 *            2023-03-07  visbwa  Added usage of BrsMain_Appl for UseCase VTT as well
 *  02.13.00  2023-03-08  visjhr  Added BRS_START/STOP_SEC_BRSAPPLENTRY_CODE for locating Brs_ApplicationEntry() to
 *                                the new section "brsApplicationEntry"
 *********************************************************************************************************************/

#ifndef _BRSMAIN_H_
#define _BRSMAIN_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "Compiler.h"

#include "vBrsCfg.h"

#if !defined (_MICROSOFT_C_VTT_)
  #include "BrsMain_Types.h"

  #include "BrsHw.h"
#endif

#if !defined (BRS_ENABLE_FBL_SUPPORT)
  #include "Os.h"
#endif

#if defined (BRS_ENABLE_HSM_SUPPORT)
  #include "vHsm.h"
#else
# if !defined (BRS_FBL_NO_ECUMINIT)
/* Ecu State Manager has to be available in system (BSW module or BRS Stub) */
  #include "EcuM.h"
# endif
#endif

/**********************************************************************************************************************
 *  MODULE VERSION
 *********************************************************************************************************************/
/*
 * Description: This is the BrsMain main and bug fix version. The version numbers are BCD coded.
 *              E.g. a main version of 1.23 is coded with 0x0123, a bug fix version of 9 is coded 0x09.
 */
#define BRSMAIN_VERSION        0x0210u
#define BRSMAIN_BUGFIX_VERSION 0x01u

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL TYPE DEFINITIONS
 *********************************************************************************************************************/
#if defined (BRS_ENABLE_TESTSUITE_SUPPORT) || defined (BRS_ENABLE_SUPPORT_LEDS) || \
    defined (BRS_ENABLE_SUPPORT_TOGGLE_WD_PIN) || defined (BRS_ENABLE_SUPPORT_TOGGLE_CUSTOM_PIN)
  #define BRS_ENABLE_1MS_HANDLER
#endif

#if defined (BRS_ENABLE_1MS_HANDLER)
typedef enum
{
  BRSMAIN_CYCLETIME_BACKGROUND,
  BRSMAIN_CYCLETIME_1MS,
  BRSMAIN_CYCLETIME_10MS,
  BRSMAIN_CYCLETIME_100MS,
  BRSMAIN_CYCLETIME_250MS,
  BRSMAIN_CYCLETIME_500MS,
  BRSMAIN_CYCLETIME_1000MS
}brsMain_Cyclic_Cycletime;
#endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/*******************************************************************************
 * ModuleCode for BrsMainExceptionHandler
 ******************************************************************************/
#define BRSERROR_MODULE_BRSHW           0x01
#define BRSERROR_MODULE_BRSMAIN         0x02
#define BRSERROR_MODULE_BRSSTARTUP      0x03
#define BRSERROR_MODULE_BRSMAINSTARTUP  0x04

/*******************************************************************************
 * ErrorCodes for BrsMainExceptionHandler
 * 0x00 to 0x7f are assigned to the BRS
 * 0x80 to 0xff are assigned to the application
 ******************************************************************************/
/* General error which is not assigned to a special error class */
#define kBrsUndefinedError                   0x00

/* No handler is installed in interrupt vector table for given interrupt */
#define kBrsInterruptHandlerNotInstalled     0x01

/* A GenTool setting is different to the excepted value */
#define kBrsWrongGenToolSettings             0x02

/* BrsHwRestoreInterrupt is called more often than BrsHwDisableInterrupt */
#define kBrsIllegalInterruptRestoration      0x03

/* Hardware configuration like clock setting is wrong */
#define kBrsIllegalHardwareConfiguration     0x04

/* Illegal parameter used like wrong channel parameter */
#define kBrsIllegalParameter                 0x05

/* Illegal TCC channel used */
#define kBrsTccChannelOutOfRange             0x06

/* Interrupt from unknown source occurred */
#define kBrsUnknownInterrupt                 0x07

/* CAN1 and CAN2 interrupts cannot be distinguished from each other */
#define kBrsAmbiguousInterruptSource         0x08

/* Execution of library version check in Common_SipVersionCheck failed */
#define kBrsLibraryVersionCheckFailed        0x09

/* OS ErrorHook called */
#define kBrsOsErrorHookCall                  0x0A

/* BRS not properly initialized */
#define kBrsConfigUninit                     0x0B

/* Application software returned to main */
#define kBrsIllegalReturnFromMain            0x0C

/* Assembler code reached, which should never be handled */
#define kBrsIllegalAssemblerReached          0x0D

/* No Core config foud for actual physical core */
#define kBrsInvalidCoreConfig                0x0E

/**********************************************************************************************************************
 *  GLOBAL VARIABLES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
#if !defined (_MICROSOFT_C_VTT_)
# if (BRSHW_SOURCECODE_TEMPLATE_VERSION >= 0x0103u)
  #define BRS_START_SEC_STARTUP_CODE
  #include "Brs_MemMap.h"
# endif
/*****************************************************************************/
/* @brief      Unified routine for Pre Main() Startup.
 * @pre        Stack pointer needs to be initilialized in StartUpCode before.
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from assembler startup code.
 *             Called by all cores from the booting execution instance.
 *****************************************************************************/
void Brs_PreMainStartup(void); /* implemented in BrsMainStartup.c */
# if (BRSHW_SOURCECODE_TEMPLATE_VERSION >= 0x0103u)
  #define BRS_STOP_SEC_STARTUP_CODE
  #include "Brs_MemMap.h"
# endif

/*****************************************************************************/
/* @brief      Unified application entry for Pre Main() Startup.
 * @pre        Stack pointer needs to be initilialized in StartUpCode before.
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from Brs_PreMainStartup() of the booting
 *             execution instance or is the direct application entry of
 *             non-booting execution instances (e.g. FBL, with a previously
 *             running BootManager). All APIs are called with current Core ID.
 *****************************************************************************/
void Brs_ApplicationEntry(void); /* implemented in BrsMainStartup.c */

# if defined (BRS_ENABLE_SUPPORT_LEDS)
void BrsMainWrapperTogglePinLED(void); /* implemented in BrsMain.c */
# endif
# if defined (BRS_ENABLE_SUPPORT_TOGGLE_WD_PIN)
void BrsMainWrapperTogglePinWD(void); /* implemented in BrsMain.c */
# endif
# if defined (BRS_ENABLE_SUPPORT_TOGGLE_CUSTOM_PIN)
void BrsMainWrapperTogglePinCustom(void); /* implemented in BrsMain.c */
# endif
#endif /*!_MICROSOFT_C_VTT_*/

/*****************************************************************************/
/* @brief      Main initialization routine.
 *             Contains configuration specific initialization of BrsHw parts
 *             and BrsMain specific mechanisms.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from main
 *****************************************************************************/
void BrsMainInit(void);

/*****************************************************************************/
/* @brief      This API is an optional callout
 * @pre        -
 * @param[in]  coreID must declare the ID of the actual running core
 * @param[out] -
 * @return     -
 * @context    Function is called from Brs_PreMainStartup() during StartUp,
 *             previous to the memory initialization, stage zero.
 *****************************************************************************/
void BrsMain_MemoryInit_StageZero_Hook(uint32); /* implemented in BrsMainCallout_Stubs.c */

/*****************************************************************************/
/* @brief      This API is an optional callout
 * @pre        -
 * @param[in]  coreID must declare the ID of the actual running core
 * @param[out] -
 * @return     -
 * @context    Function is called from Brs_PreMainStartup() during StartUp,
 *             previous to the memory initialization, stage hard reset.
 *****************************************************************************/
void BrsMain_MemoryInit_StageHardReset_Hook(uint32); /* implemented in BrsMainCallout_Stubs.c */

/*****************************************************************************/
/* @brief      This API is an optional callout
 * @pre        -
 * @param[in]  coreID must declare the ID of the actual running core
 * @param[out] -
 * @return     -
 * @context    Function is called from Brs_PreMainStartup() during StartUp,
 *             previous to the memory initialization, stage one.
 *****************************************************************************/
void BrsMain_MemoryInit_StageOne_Hook(uint32); /* implemented in BrsMainCallout_Stubs.c */

/*****************************************************************************/
/* @brief      This API is an optional callout
 * @pre        -
 * @param[in]  coreID must declare the ID of the actual running core
 * @param[out] -
 * @return     -
 * @context    Function is called from Brs_PreMainStartup() during StartUp,
 *             previous to the memory initialization, stage two.
 *****************************************************************************/
void BrsMain_MemoryInit_StageTwo_Hook(uint32); /* implemented in BrsMainCallout_Stubs.c */

/*****************************************************************************/
/* @brief      This API is an optional callout
 * @pre        -
 * @param[in]  coreID must declare the ID of the actual running core
 * @param[out] -
 * @return     -
 * @context    Function is called from Brs_PreMainStartup() during StartUp,
 *             previous to the memory initialization, stage three.
 *****************************************************************************/
void BrsMain_MemoryInit_StageThree_Hook(uint32); /* implemented in BrsMainCallout_Stubs.c */

/*****************************************************************************/
/* @brief      This API is an optional callout
 * @pre        -
 * @param[in]  coreID must declare the ID of the actual running core
 * @param[out] -
 * @return     -
 * @context    Function is called from Brs_PreMainStartup() during StartUp,
 *             after the memory initialization was done.
 *****************************************************************************/
void BrsMain_PreMainHook(uint32); /* implemented in BrsMainCallout_Stubs.c */

/*****************************************************************************/
/* @brief      This API is an optional callout
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from BrsHwSoftwareResetECU(), before a
 *             SW reset is triggered. Implement code here to also reset
 *             peripherals, if necessary for your UseCase.
 *****************************************************************************/
void BrsMain_SoftwareResetECU_Hook(void); /* implemented in BrsMainCallout_Stubs.c */

#if defined (BRS_ENABLE_1MS_HANDLER)
/*****************************************************************************/
/* @brief      Routine to register cyclic callbacks.
 * @pre        Initialization of BrsMain was done threw call of BrsMainInit().
 * @param[in]  FunctionPointer has to be a pointer to a function of type
 *             void function(void).
 * @param[in]  Cycletime described the cycletime, the callback should be triggered.
 * @param[out] -
 * @return     -
 * @context    Function is called from modules that need cyclic callbacks.
 *****************************************************************************/
void BrsMainRegisterCyclic(void (*)(void), brsMain_Cyclic_Cycletime);

/*****************************************************************************/
/* @brief      One millisecond handler for BrsMain
 *               - Executes retransmission of BRS TCC messages
 *               - Toggling of LED (alive signal)
 *               - BRS Test code (1s cyclic negative TCC response message)
 * @pre        Initialization of BrsMain was done threw call of BrsMainInit().
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called each millisecond either from the main loop
 *             or from the BRS main task (TASK(mainTask))
 *****************************************************************************/
void BrsMainCyclic1ms(void);

# if (!defined (BRS_ENABLE_FBL_SUPPORT)) && (!defined (BRS_ENABLE_OS_INTERRUPT_ONLY))
TASK(BrsMainTask);
TASK(BrsMainBackgroundTask);
# endif
#endif

#if (!defined (BRS_ENABLE_FBL_SUPPORT)) && (!defined (BRS_ENABLE_OS_INTERRUPT_ONLY))
TASK(Default_Init_Task);
TASK(Default_Init_Task_Trusted);
#endif

#if defined (BRS_ENABLE_OS_MULTICORESUPPORT)
TASK(Default_Init_Task_Core0);
TASK(Default_Init_Task_Core0_Trusted);
TASK(Default_Init_Task_Core1);
TASK(Default_Init_Task_Core1_Trusted);
TASK(Default_Init_Task_Core2);
TASK(Default_Init_Task_Core2_Trusted);
TASK(Default_Init_Task_Core3);
TASK(Default_Init_Task_Core3_Trusted);
TASK(Default_Init_Task_Core4);
TASK(Default_Init_Task_Core4_Trusted);
TASK(Default_Init_Task_Core5);
TASK(Default_Init_Task_Core5_Trusted);
TASK(Default_Init_Task_Core6);
TASK(Default_Init_Task_Core6_Trusted);
#endif


#if defined (BRS_ENABLE_SUPPORT_LEDS)          || \
    defined (BRS_ENABLE_SUPPORT_TOGGLE_WD_PIN) || \
    defined (BRS_ENABLE_SUPPORT_TOGGLE_CUSTOM_PIN)
typedef enum
{
  BRSMAIN_TOGGLEPIN_LED,
  BRSMAIN_TOGGLEPIN_WD,
  BRSMAIN_TOGGLEPIN_CUSTOM
}brsMain_TogglePin;

# if defined (BRS_ENABLE_FBL_SUPPORT)
  #define BRS_START_SEC_RAM_CODE
  #include "Brs_MemMap.h"
# endif
/*****************************************************************************/
/* @brief      This API is used to toggle a PortPin.
 *             Per default, the following parameters are available:
 *               BRSMAIN_TOGGLEPIN_LED
 *               BRSMAIN_TOGGLEPIN_WD
 *               BRSMAIN_TOGGLEPIN_CUSTOM
 *             Depending pins must be configured and initialized within BrsHw.
 * @pre        -
 * @param[in]  Pin to be toggled
 * @param[out] -
 * @return     -
 * @context    Function is called from all modules to set or clear a PortPin
 *****************************************************************************/
void BrsMainTogglePin(brsMain_TogglePin);
# if defined (BRS_ENABLE_FBL_SUPPORT)
  #define BRS_STOP_SEC_RAM_CODE
  #include "Brs_MemMap.h"
# endif
#endif /*BRS_ENABLE_SUPPORT_LEDS||BRS_ENABLE_SUPPORT_TOGGLE_WD_PIN||BRS_ENABLE_SUPPORT_TOGGLE_CUSTOM_PIN*/

#if defined (BRS_ENABLE_FBL_SUPPORT)
  #define BRS_START_SEC_RAM_CODE
  #include "Brs_MemMap.h"
#endif
void BrsMainExceptionHandler(uint8 ErrorCode, uint8 ModuleCode, uint16 LineNumber);
#if defined (BRS_ENABLE_FBL_SUPPORT)
  #define BRS_STOP_SEC_RAM_CODE
  #include "Brs_MemMap.h"
#endif

#if !defined (_MICROSOFT_C_VTT_)
# if defined (BRS_FIRST_EXECUTION_INSTANCE)
  /* This code is only needed for the first instance in the system */
#  if (BRSHW_SOURCECODE_TEMPLATE_VERSION >= 0x0103u)
  #define BRS_START_SEC_STARTUP_CODE
  #include "Brs_MemMap.h"
#  endif
/*****************************************************************************/
/* @brief      This is the central exeption/issue handler of BRS, in startup
 *             phase.
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    This function implements an endless loop. The recommendation is
 *             to set a breakpoint on top of this function to see if any check
 *             has failed during the startup code execution.
 *****************************************************************************/
void BrsMainExceptionStartup(void);
#  if (BRSHW_SOURCECODE_TEMPLATE_VERSION >= 0x0103u)
  #define BRS_STOP_SEC_STARTUP_CODE
  #include "Brs_MemMap.h"
#  endif
# endif /*BRS_FIRST_EXECUTION_INSTANCE*/

/*****************************************************************************/
/* @brief      Get reset reason
 * @pre        BrsHwGetResetReasonStartup() is called first and the rest reason
 *             is saved in global variable brsMain_ResetReason.
 * @param[in]  -
 * @param[out] -
 * @return     Reset reason
 * @context    This is a wrapper function for previous BrsHwGetResetReason().
 *             This is to prevent multiple selection of reset reason
 *             registers, as this is not supported on all platforms.
 *             The HW registers for the reset reason are evaluated via a call
 *             of BrsHwGetResetReasonStartup() in Brs_PreMainStartup().
 *             The result is stored by BrsMainStartup in the global variable
 *             brsMain_ResetReason.
 *****************************************************************************/
brsMain_ResetReasonType BrsMainGetResetReason(void);

int main(void);
#endif /*_MICROSOFT_C_VTT_*/

#if defined (BRS_ENABLE_OS_INTERRUPT_ONLY)
void BrsMainSuspendAllInterrupts(void);
void BrsMainResumeAllInterrupts(void);
#endif

#if defined (BRS_ENABLE_CAN_SUPPORT)
/***********************************************************************************************************************
 * UserDefined ExclusiveArea handling for CAN channels, according to AN-ISC-8-1149_ErrorHook_E_OS_DISABLED_INT.pdf
 **********************************************************************************************************************/
/* Provide default definitions if vBRS generator did not generate them */
#ifndef BRS_DRVCAN_EXCLUSIVE_AREA_INFIX
  #define BRS_DRVCAN_EXCLUSIVE_AREA_INFIX(action, area) void SchM_##action##_Can_CAN_##area(void)
#endif
#ifndef BRS_DRVCAN_HEADER_FILENAME
  #define BRS_DRVCAN_HEADER_FILENAME "Can.h"
#endif
/* Macro is generated in vBrsCfg.h, to support drivers with infix.
   Sample w/o infix: void SchM_Enter_Can_CAN_EXCLUSIVE_AREA_0(void)
                     Can_DisableControllerInterrupts(0u);
   Sample w/ infix:  void SchM_Enter_Can_30_Mcan_CAN_30_MCAN_EXCLUSIVE_AREA_0(void)
                     Can_30_Mcan_DisableControllerInterrupts(0u);*/

#define CAN_START_SEC_CODE
/* to support CAN driver name infix, vBRS generator provides a wrapper
   for usage of the driver specific MemMap.h define in vBrsCfg.h */
#include "vBrsCfg.h"
BRS_DRVCAN_EXCLUSIVE_AREA_INFIX(Enter,EXCLUSIVE_AREA_0);
BRS_DRVCAN_EXCLUSIVE_AREA_INFIX(Exit,EXCLUSIVE_AREA_0);
BRS_DRVCAN_EXCLUSIVE_AREA_INFIX(Enter,EXCLUSIVE_AREA_6);
BRS_DRVCAN_EXCLUSIVE_AREA_INFIX(Exit,EXCLUSIVE_AREA_6);
#define CAN_STOP_SEC_CODE
/* to support CAN driver name infix, vBRS generator provides a wrapper
   for usage of the driver specific MemMap.h define in vBrsCfg.h */
#include "vBrsCfg.h"
#endif /*BRS_ENABLE_CAN_SUPPORT*/

#if defined (BRS_ENABLE_CAN_SUPPORT)
#define CANIF_START_SEC_CODE
#include "MemMap.h"
void SchM_Enter_CanIf_CANIF_EXCLUSIVE_AREA_0(void);
void SchM_Exit_CanIf_CANIF_EXCLUSIVE_AREA_0(void);
#define CANIF_STOP_SEC_CODE
#include "MemMap.h"
#endif /*BRS_ENABLE_CAN_SUPPORT*/

#if defined (BRS_ENABLE_CAN_SUPPORT)
#define CANSM_START_SEC_CODE
#include "MemMap.h"
void SchM_Enter_CanSM_CANSM_EXCLUSIVE_AREA_4(void);
void SchM_Exit_CanSM_CANSM_EXCLUSIVE_AREA_4(void);
void SchM_Enter_CanSM_CANSM_EXCLUSIVE_AREA_1(void);
void SchM_Exit_CanSM_CANSM_EXCLUSIVE_AREA_1(void);
#define CANSM_STOP_SEC_CODE
#include "MemMap.h"
#endif /*BRS_ENABLE_CAN_SUPPORT*/

#define COMM_START_SEC_CODE
#include "MemMap.h"
void SchM_Enter_ComM_COMM_EXCLUSIVE_AREA_1(void);
void SchM_Exit_ComM_COMM_EXCLUSIVE_AREA_1(void);
#define COMM_STOP_SEC_CODE
#include "MemMap.h"

#endif /*_BRSMAIN_H_*/
