/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 *  \addtogroup Os_Hal_Os
 *  \{
 *
 *  \file
 *  \brief      HAL interfaces which are visible to the user.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  --------------------------------------------------------------------------------------------------------------------
 *  02.00.00  2017-04-06  virbse  WORKITEM15149 Initial version for ARM Cortex-M
 *  02.01.00  2017-05-04  vismkk  -             Internal improvements.
 *            2017-05-22  virbse  STORY-993     Internal improvements.
 *                        virsmn  STORY-1029    Memory protection support for S32K and MWCT101x devices
 *                                ESCAN00095068 Compiler error: Multiple defined MPU functions
 *  02.01.01  2017-05-31  visrk   -             Internal improvements.
 *  02.02.00  2017-06-01  visdfe  ESCAN00094046 Wrong validation of the maximum available MPU regions
 *  02.03.00  2017-06-29  visces  STORYC-1302   Support for cat. 0 ISRs
 *  02.04.00  2017-07-13  visshk  -             Internal improvements
 *  02.05.00  2017-08-15  virsmn  ESCAN00094867 Compiler warning: warning #1514-D:
 *                                              incorrect GHS #pragma: small data area is not supported in this configuration
 *                        visrk   -             Internal improvements
 *                        visdhe  STORY-1752    Support for ARM compiler with Cortex-M platform
 *                        virbiv  ESCAN00096081 Compiler Warning  #32-D: expression must have arithmetic type in
 *                                              Os_Hal_Context_Lcfg.c
 *                        virsmn  ESCAN00095677 CortexM CoreMPU wrongly reprogrammed
 *                        virsso  STORYC-1891   Support for TI compiler as BETA
 *  02.06.00  2017-09-05  virsso  ESCAN00096420 Compiler error: "OS_HAL_MP_STACK_MAXSIZE_AND_ATTRIBUTES" not defined
 *  02.07.00  2017-10-10  visces  ESCAN00095552 Free Running Timers configured as non High Resolution Counters do not
 *                                              work properly.
 *  02.08.00  2017-10-18  virsso  ESCAN00096902 Compiler warning: OS_HAL_COMPILER_TINT_H is used as a header guard,
 *                                              followed by #define of a different macro [-Wheader-guard]
 *                                ESCAN00096476 "OS_CODE" memory section is not properly linked
 *                        virbse  FEAT-2816     Support shared stack for non-preemptive Basic tasks
 *                                FEAT-2815     Support shared stack for tasks with same internal resource
 *                        virsso  ESCAN00097137 The pre-config of MPU region for non-trusted application contains the
 *                                              wrong accessing rights
 *  02.09.00  2017-11-03  virsmn  ESCAN00096854 Usage of High Resolution Timer and Master Protection Mechanism of INTC
 *                                              could lead to protection violation.
 *                        visdhe  STORYC-2762   Support OS SC1/SC2 with GCC Compiler Production QM
 *                        virbiv  ESCAN00097206 Stack corruption during context switch if FPU is used
 *  02.10.00  2017-11-22  visshk  STORYC-1751   Performance improvements for some IOC configurations
 *                        visbpz  STORY-2769    OS Interrupt API optimization by replacement of a call sequence for
 *                                              modification of the global interrupt flag.
 *                        virsso  STORYC-3110   Support for TDA2Eco derivative.
 *                        virsso  ESCAN00094835 Spinlock API may cause a fatal exception with IAR compiler
 *  02.11.00  2017-01-10  virsso  STORYC-3218   Support for user configurable FPU cotext saving
 *                        virjas  ESCAN00098049 Undefined behavior of OS after context switch
 *                        virsso  ESCAN00097806 Spinlock API may cause a fatal exception with GCC Linaro compiler
 *  02.12.00  2017-01-29  virjas  -             Internal improvements
 *                        virsmn  STORYC-3282   Support for user Msr settings.
 *  02.12.02  2018-03-05  virsso  ESCAN00098567 Stack corruption in trap handler if hardware fpu is used
 *  02.13.00  2018-03-09  virsmn  STORYC-3042   Support for OS internal exception detection.
 *                        virsso  ESCAN00098061 FPU registers not correctly saved/restored during context switch
 *                        virsso  ESCAN00096334 BETA version - the BSW module has a feature with BETA state
 *                                              (Support for TI compiler)
 *                        virsso  ESCAN00098749 Os platform specific functions are wrongly mapped in case of TI compiler
 *  02.14.00  2018-03-23  visbpz  STORYC-4068   Performance improvment by avoiding traps
 *  02.15.00  2018-04-23  virsmn  STORYC-4799   Added coverage justifications for S32K derivatives.
 *            2018-05-03  virsmn  STORYC-3965   Added support for derivatives without interrupt level support.
 *  02.16.00  2018-05-14  virsmn  STORYC-3320   Added support for interrupt mapping feature.
 *            2018-05-15  virsso  ESCAN00099062 Wrong interrupt level restored after non trusted function call
 *                                              because of IAR compiler optimization.
 *  02.17.00  2018-06-11  virsso  ESCAN00099374 Not all the MPU regions can be used on S32K148.
 *            2018-06-14  visbpz  STORY-5018    Unhandled Exception not available for SC >= SC3
 *            2018-06-27  virsso  STORYC-5587   Support for Traveo2(CYT2Bx) peripheral interrupt handling.
 *                                ESCAN00099836 Compiler reports error because of typo in include file name.
 *            2018-06-29  virsmn  STORYC-5594   Added support for aggregated stack configuration view.
 *  02.18.00  2018-07-20  visbpz  STORYC-5671   Added support for IMX8x peripheral interrupt handling.
 *                        visbpz  ESCAN00102260 Not all Cortex-M MPU access rights can be configured.
 *  02.19.00  2018-08-03  virsso  STORYC-6091   Added support for S32x peripheral interrupt handling and multicore.
 *            2018-08-08  virsmn  STORYC-4094   Achieve 100 percent variant coverage.
 *            2018-08-16  virsso  ESCAN00100283 Store immediate overlapping exception return might vector to incorrect
 *                                              interrupt
 *            2018-08-16  visbpz  ESCAN00100164 BETA version - the BSW module has a feature with BETA state
 *                                              (IMX8x support)
 *  02.20.00  2018-09-14  visbpz  -             Internal Improvements.
 *  02.21.00  2018-10-11  visror  STORYC-4537   MISRA-C 2012 Compliance
 *  02.21.01  2018-10-25  visdfe  -             Internal improvements.
 *  02.22.00  2018-11-06  virsmn  STORYC-4796   Interrupt API optimization.
 *            2018-11-13  virsso  ESCAN00099842 BETA version - the BSW module has a feature with BETA state
 *                                              (CYT2x support)
 *  02.23.00  2018-12-05  virsso  PSC-1306      Adapt SafeBSW counter measures for hardware access
 *            2018-12-12  virsso  STORYC-7851   As user I want to implement my own exception handler without any Os
                                                wrapper in between.
 *  02.24.00  2019-01-07  visror  -             Internal improvement
 *  02.25.00  2019-01-17  visaev  -             Internal improvement
 *            2019-02-07  virsso  ESCAN00101862 Compiler error: Expression out of range in interrupt entry assembler
 *                                              macro
 *            2019-02-08  visaev  STORYC-7166   Added support for initial enabling of interrupt sources.
 *  02.26.00  2019-02-27  virsso  ESCAN00102264 Compiler reports error because of wrong OS_HAL_ASM_DUMMY_BRANCH syntax.
 *            2019-02-28  visaev  STORYC-7720   Internal improvement on compiler warning justification
 *            2019-03-08  vismun  STORYC-7850   Support for Traveo2 Body High (CYT4Bx)
 *  02.27.00  2019-03-13  virsmn  ESCAN00102127 Compiler Error: Unresolved Symbols.
 *            2019-03-15  visrk   STORYC-2545   Configuration of FRT timer size.
 *            2019-03-26  virsso  STORYC-8020   Support for ARM6 compiler
 *  02.28.00  2019-05-10  virsmn  STORYC-7025   Update for hierarchical interrupt controller support.
 *            2019-05-14  virsso  ESCAN00102433 Support for Traveo2 Body High(CYT4Bx) Production QM.
 *            2019-05-27  visbpz  ESCAN00103016 Wrong OsSystemIntCtrlMaxSources for CYT2B9x derivatives.
 *  02.29.00  2019-06-17  virsso  -             Internal Improvements.
 *            2019-06-18  visdqk  FIX-6174      Update for generic stack gaps
 *            2019-06-25  vismun  STORYC-8326   As a user I want MICROSAR OS to support Timing Protection on Imx8
 *            2019-06-26  virsso  ESCAN00103511 Os_UnhandledIrq is executed in case of spurious interrupts by iMx8
 *  02.30.00  2019-07-08  visdqk  -             Updated version number.
 *            2019-06-26  virsso  ESCAN00097293 Missed timer ticks are not reported on a RTT timer when configured as
 *                                              not high resolution
 *            2019-06-26  virsso  ESCAN00094834 Prescaler initialization for RTT (Real-Time Timer) module does not
 *                                              match the requirement in the hardware manual if HRT is configured.
 *            2019-06-26  virsso  ESCAN00103553 High Resolution Timer Isrs could be delayed or lost on iMx8 derivatives
 *            2019-07-25  virsso  ESCAN00103835 Compiler error: Instruction requires VFP support
 *  02.31.00  2019-08-12  virsso  PSC-2623      Support for Tasking compiler (Production).
 *  02.32.00  2019-09-17  virsmn  OSC-3472      Derivative specific FRT configuration.
 *  02.33.00  2019-10-07  visbpz  OSC-5148      Support SC4 on Traveo2 BE.
 *  02.34.00  2019-10-25  virsmn  OSC-5192      Updated prescaler for FTM module.
 *            2019-11-13  vismun  HALBE-5       S32G274A support
 *  02.35.00  2019-12-09  virrlu  HALBE-224     Support SC4 on S32G274A.
 *            2019-12-09  visto   HALBE-320     Fix ESCAN00104103 BETA version - the BSW module has a feature with
 *                                              BETA state (iMx8 derivatives support).
 *  02.36.00  2019-20-12  visdqk  OSC-5248      Updated version number due to generator changes.
 *                        vismun  -             Added coverage justifications for interrupt handling.
 *  02.37.00  2020-01-15  visbpz  HALBE-783     Internal improvement.
 *  02.38.00  2020-02-14  visror  OSC-5389      Internal improvement.
 *            2020-03-03  visbpz  HALBE-987     Support for S32K2TV.
 *                        vismun  HALBE-226     Production QM support for S32G274A.
 *                                ESCAN00105136 OS switches to invalid stack in case of memory protection exception.
 *                        virrlu  HALBE-1144    Add support for HighTec compiler.
 *  02.39.00  2020-03-16  virsmn  OSC-5361      Increase number of configurable OS Applications.
 *                        visbpz  HALBE-1178    Multicore support for iMX8x.
 *  02.40.00  2020-04-14  visror  -             Updated versions due to BSWMD change.
 *            2020-04-24  vismun  HALBE-1532    Production QM support for iMX8xDualCore.
 *  02.41.00  2020-05-19  visdri  HALBE-1794    Provide new exception context APIs.
 *            2020-05-18  visbpz  HALBE-1863    Support SC4 on CYT4Bx.
 *            2020-05-02  visdri  HALBE-1358    Support GetExceptionContext API for Cortex-M.
 *  02.42.00  2020-06-16  visbpz  HALBE-2097    Internal improvement.
 *            2020-06-24  visbpz  HALBE-2110    Internal improvement.
 *            2020-06-24  vismaa  HALBE-2067    Updating reference manuals.
 *  02.43.00  2020-07-02  visbpz  -             Internal improvement.
 *            2020-07-15  visbpz  HALBE-1826    Support IMX8DXL.
 *  02.44.00  2020-08-10  virleh  -             Updated SW version.
 *            2020-08-13  visdri  HALBE-2475    Support for iMXRT - SC4 - Development Release.
 *            2020-08-13  visdri  ESCAN00106953 TCPWM Timer interrupt wrongly initialized.
 *  02.45.00  2020-09-03  virsmn  -             Updated SW version.
 *            2020-09-08  vismaa  HALBE-2740    Support for IMX8x_DualCore - SC4 - Development Release.
 *  02.46.00  2020-09-23  visdri  HALBE-2639    OS Support for i.MX RT - SC4 - QM Release.
 *            2020-09-10  virleh  OSC-6102      Justify all MISRA deviations after update to new QAC version.
 *            2020-09-14  virrlu  HALBE-2734    Production ASIL support for CYT2Bx.
 *            2020-10-14  virsmn  OSC-6462      Added Interrupt Post Action support.
 *            2020-10-14  visdqk  OSC-6655      Internal improvement.
 *            2020-10-16  virrlu  -             Added coverage justifications for Interrupt Post Action.
 *  02.47.00  2020-10-28  virleh  OSC-6639      Internal improvement.
 *  02.48.00  2020-12-02  visbpz  ESCAN00107806 Compiler warning.
 *            2020-12-09  virrlu  HALBE-1276    Os S32K3xx: Implement new NVIC routing.
 *  02.49.00  2020-12-23  virsmn  OSC-5882      ESCAN00088830, ESCAN00104942, ESCAN00089701.
 *  02.50.00  2021-02-11  visbpz  ESCAN00108438 Spinlocks can be acquired by multiple cores simultaneously.
 *            2021-02-23  visdqk  -             Internal improvement for MISRA.
 *  02.51.00  2020-03-18  visdqk  ESCAN00108354 Removed obsolete HAL API.
 *            2021-03-24  vismaa  HALBE-3107    Added OS_HAL_ASM_NOP for preventing emtpy object files(TI).
 *  02.52.00  2021-04-09  vismun  HALBE-4203    S32R45 support.
 *            2021-04-23  visbpz  HALBE-1692    SC4 support for S32K3xx.
 *  02.53.00  2021-05-10  visbpz  HALBE-4118    ASIL release for CYT4Bx.
 *            2021-05-27  vismun  HALBE-2863    Cortex-M support for GCC NXP compiler.
 *  02.54.00  2021-06-18  visto   HALBE-4628    Compiler error: Os does not compile with Tasking v4.2 and older.
 *            2021-06-21  vismun  HALBE-4749    Support for S32K3xx multicore.
 *  02.55.00  2021-07-14  vismun  HALBE-4528    Support for Arm Cortex-Mv8 architecture (Cortex-M33).
 *            2021-07-16  virrlu  HALBE-4828    Cortex-M support for LLVM Diab compiler.
 *..02.56.00  2021-08-02  vismun  ESCAN00109849 Incorrect xSignal configuration for CYT4Dx and S32K3x.
 *            2021-08-16  vismun  HALBE-4934    Support memory protection for Arm Cortex-M33.
 *  02.57.00  2021-09-06  vismun  HALBE-5233    ASIL release for Arm Cortex-M33.
 *  02.58.00  2021-09-23  visdqk  OSC-7919      MISRA migration to new QAC version.
 *  02.59.00  2021-10-21  virrlu  HALBE-149     Internal improvement.
 *  02.60.00  2021-11-24  visrk   OSC-8178      Internal improvement.
 *            2021-12-06  visrk   OSC-6113      64 bit support for VttOs.
 *  02.61.00  2021-12-16  virsmn  OSC-8186      Updates for new QAC version (MISRA).
 *  02.62.00  2022-02-03  vismun  HALBE-4598    ASIL release for S32K3xx.
 *                        visbpz  HALBE-4996    Cortex-M support for LlvmTexasInstruments compiler.
 *  02.63.00  2022-02-21  vismaa  HALBE-6054    New subfamily CYT2Cx added.
 *  02.64.00  2022-03-11  visbpz  HALBE-6052    Cortex-M support for LLVM HighTec compiler.
 *  02.65.00  2022-04-20  vismun                Internal improvement.
 *            2022-04-25  virmid  HALBE-6687    Support for SAF85xx.
 *  02.66.00  2022-06-02  vismun  HALBE-6985    Updated ASIL release for Arm Cortex-M33.
 *            2022-06-03  virmid  HALBE-6785    Support for Diab v7.0.2.1 compiler.
 *            2022-06-10  virmid  HALBE-6920    Switching to the kernel stack when unhandled exception occurs.
 *            2022-06-10  visqli  HALBE-6483    Handling category 1 interrupts in handler mode.
 *            2022-06-15  vismun  HALBE-6427    Multicore support for iMXRT.
 *  02.67.00  2022-06-30  vismun  HALBE-7184    Support for S32G3x and updated multicore support for iMXRT.
 *            2022-07-14  viswip  HALBE-5670    Production QM support for IMX8QuadXPlus with GccGnu Compiler.
 *  02.68.00  2022-09-16  vismun  ESCAN00112775 Assert that MPU region number is not overwritten.
 *  02.69.00  2022-09-26  virrlu  HALBE-7617    Support for S32K39/8x.
 *            2022-09-28  visqli  ESCAN00111711 Restoring of an incorrect interrupt level (with IAR compiler < v9.20.2).
 *            2022-10-13  virrlu  HALBE-7475    ASIL release for S32Gxx.
 *  02.70.00  2022-11-10  vismun                Internal improvement.
 *  02.71.00  2022-12-05  hmg     HALBE-4270    Internal improvement.
 *            2022-12-15  vismun  HALBE-7667    Added Finite Execution Time Analysis (FETA) justifications.
 *            2022-12-22  virrlu                Internal improvement.
 *  02.72.00  2023-02-02  vismun  HALBE-8576    Provided HAL Interface Os_Hal_TimerFrtGetCompareValue().
 *            2023-02-06  vismun  HALBE-8217    ASIL release for S32K39x.
 *            2023-03-24  vimmoi  HALBE-7166    Update S32K3xx hardware manuals.
 *            2023-04-04  virxic  HALBE-7808    QM release for iMXRT.
 **********************************************************************************************************************/

#ifndef OS_HAL_OS_H
# define OS_HAL_OS_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Hal_OsInt.h"

/* Os kernel module dependencies */
# include "Os_Lcfg.h"

/* Os hal dependencies */


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/
/* ----- Component version information ----- */
# define OS_HAL_SW_MAJOR_VERSION                     (2u)
# define OS_HAL_SW_MINOR_VERSION                     (72u)
# define OS_HAL_SW_PATCH_VERSION                     (0u)

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#endif /* OS_HAL_OS_H */


/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Os.h
 **********************************************************************************************************************/
