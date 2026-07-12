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
/** \file  File:  ARMBrsHw_CortexM.h
 *      Project:  Vector Basic Runtime System
 *       Module:  BrsHw for all platforms with ARM core Cortex-M
 *
 *  \brief Description:  This is a global, platform-independent header file for the ARM-BRS.
 *                       This file includes all non-platform dependent functions.
 *                       All the (platform depending) rest needs to be defined in BrsHw.c
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
 *  01.00.00  2020-05-20  visbwa  New code basis for vBaseEnv 2.0 (based on zBrs_ArmCommon ARMBrsHw_CortexM.c 4.09.02)
 *  01.00.01  2020-06-05  visbwa  Fixed support for Tasking compiler in ARMStartup_CortexM.c
 *  01.00.02  2020-06-08  visbwa  Fixed usage of struct vLinkGen_MemArea (vLinkGen_ZeroInit_Early_Blocks,
 *                                vLinkGen_ZeroInit_Early_Groups) -> size is 16Byte per entry in ARMStartup_CortexM.c
 *  01.00.03  2020-06-09  visbwa  Fixed IAR code to load stack symbols (Brs_IarLoadStackSymbols_cx()) in
 *                                ARMStartup_CortexM.c
 *  01.00.04  2020-07-03  visbwa  Fixed MultiCore define, added check for derivatives with more than 2 cores in
 *                                ARMStartup_CortexM.c
 *  01.00.05  2020-07-27  vismun  Extended MultiCore implementation up to 3 cores in ARMStartup_CortexM.c
 *  01.00.06  2020-07-28  visbwa  Experimental support for FBL Legacy UseCase w/o OS (will became Brs_Template 1.01.00)
 *            2020-07-28  visbwa  Initial creation of ARMBrsHwIntTb_CortexM.c
 *            2020-08-24  visjhr  HALBE-1770 Problems with ARM common startup code with Hightec compiler
 *  01.01.00  2020-09-18  visbwa  Review according to Brs_Template 1.01.00
 *  01.01.01  2020-10-06  visbwa  Fixed BRS_ISR_KEYWORD of Arm5, Gnu and Iar compiler abstraction in
 *                                ARMBrsHw_CortexM.h, update to Brs_Template 1.01.01, removed AUTHOR IDENTITY
 *  01.00.02  2020-10-09  visbwa  Fixed Iar support (use default label for stack) in ARMStartup_CortexM.c
 *  01.00.03  2020-10-13  visbwa  Fixed thumb support for entry brsStartupEntry in ARMStartup_CortexM.c
 *  01.00.04  2020-10-15  visbwa  Fixed thumb support for entry brsStartupEntry in ARMStartup_CortexM.c
 *  01.00.05  2020-10-21  visbwa  Fixed TI support (use default label for stack) in ARMStartup_CortexM.c
 *  01.01.06  2020-11-09  visbwa  Updates in ALM package only, to support GccGnu
 *  01.01.07  2020-12-14  visrgm  Added PUSH & POP for ARM5 inline assembly helper functions
 *  01.01.08  2021-01-14  visbwa  Fixed BRANCH to startup_block_zero_init_loop_start for ARM compilers in
 *                                ARMStartup_CortexM.c
 *  01.02.00  2021-04-22  vismaa  Added support for compiler GccNxp
 *            2021-05-04  vishci  Renamed HIGHTEC to GCCHIGHTEC
 *            2021-04-05  visbwa  Update to Brs_Template 1.02.01, moved revision history of all .c and .h files to here
 *  01.02.01  2021-06-07  vishci  Fixed Arm5 compiler support for Cortex-M0+
 *  01.02.02  2021-07-12  visbwa  Fixed SP reset value and Reset Handler for IAR compiler in ARMBrsHwIntTb_CortexM.c,
 *                                fixed STACK_Cx label handling for IAR compiler in ARMStartup_CortexM.c
 *  01.03.00  2021-07-21  visjhr  Added support for compiler LlvmDiab
 *  01.03.01  2021-08-17  visjhr  Fixed support for Generic Platform Cortex-M with Arm6 compiler
 *  01.03.02  2021-08-30  visjhr  HALBE-5176 Brs CortexM0: Interrupt functions rework
 *  01.03.03  2021-09-02  visjhr  Added push/pop of registers used in asm code within BrsHwDisableInterruptAtPowerOn
 *                                (HALBE-5306) 
 *  01.03.04  2021-09-07  visjhr  Fixed HALBE-5306 for Arm5 compiler
 *  01.03.05  2021-09-15  visbwa  Fixed Stack_C0 label for Arm5+Arm6 compiler in ARMStartup_CortexM.c,
 *                                fixed Arm6 support in ARMBrsHwIntTb_CortexM.c
 *  02.00.00  2021-09-20  virrlu  Added support for BrsMain_CoreConfig based initialization.
 *            2021-09-27  virjas  TAR-1911 Adaptions for TI compiler in BrsHwDisableInterruptAtPowerOn.
 *            2021-10-18  visjhr  Removed unneccessary asm code in ARMStartup_CortexM.c memory initialization
 *            2021-10-22  visbwa  Finalized update to Brs_Template 1.03; added configuration of VTOR in StartupCode
 *  02.01.00  2021-10-26  vishci  added support for LlvmTexasInstruments
 *            2021-10-28  visjhr  ARMStartup_CortexM.c: Enable L1 cache on Cortex-M7 cores (HALBE-4742)
 *            2021-12-01  visbwa  Added Arm6 compiler support for Brs_Template 1.03 stack and excvect/intvect macros,
 *                                fixed TI compiler support in ARMStartup_CortexM.c
 *            2021-12-01  visrgm  Fixed loading of BrsExcvec and BrsMain_CoreConfig_Size for IAR compiler and added
 *                                assembler code for branching BrsMainExceptionStartup for ARM5 compiler within
 *                                ARMStartup_CortexM.c
 *            2021-12-01  visbwa  Fixed L1 cache enabling of Cortex-M7 cores in ARMStartup_CortexM.c
 *            2021-12-02  visbwa  Added _Brs_ExcVectRam_START and STACK_C0 for FBL UseCase with Arm5/Arm6 compiler
 *            2021-12-03  visbwa  Removed buggy TI compiler Cortex-M0/M0+ support, added a check for it
 *            2021-12-07  visbwa  Update to Brs_Template 1.03.01
 *  02.01.01  2021-12-08  virjas  Added missing function prototypes to support options like --require_prototypes (IAR)
 *  02.01.02  2021-12-21  vismaa  Added missing PUSH/POP for all compiler in BrsHwDisableInterruptAtPowerOn
 *  02.01.03  2022-02-10  visdri  Fixed LlvmTi support in ARMStartup_CortexM.c
 *  02.01.04  2022-02-10  visdri  Fixed LlvmTi support in ARMStartup_CortexM.c
 *  02.01.05  2022-02-18  visbwa  Update to Brs_Template 1.03.03 (mainly encapsulation in ARMBrsHwIntTb_CortexM.c),
 *                                fixed revision history and ARMBRSHW_CORTEXM_BUGFIX_VERSION
 *  02.01.06  2022-02-28  visbwa  ESCAN00111379: Fixed zero_init loops in ARMStartup_CortexM.c, renamed area_zero_init
 *                                to group_zero_init, introduced BRS_BRANCH_GREATER_THAN_OR_EQUAL
 *  02.01.07  2022-03-11  visbwa  ESCAN00111472: StartupCode is not mapped into specific section "brsStartup" for Arm5
 *                                compiler (ARMStartup_CortexM.c),
 *                                ESCAN00111475: StartupCode is not mapped into specific section "brsStartup" for Arm6
 *                                compiler (ARMStartup_CortexM.c),
 *                                removed unused BRS_SECTION_DATA fragments from some compiler abstractions
 *  02.01.08  2022-03-16  visjhr  Implemented 8 byte aligned ECC initialization based on usage of alignment parameter 
 *                                (HALBE-4611)
 *            2022-03-18  visbwa  ESCAN00111520: ExceptionVectorTable section .brsExcVect is not mapped into specific
 *                                memory region block for Tasking compiler
 *                                -> introduced new brsExcVectConst in ARMBrsHwIntTb_CortexM.c
 *  02.02.00  2022-03-23  visbpz  Added support for MicroChip compiler
 *  02.02.01  2022-03-24  visbwa  ESCAN00111560: BrsHwDisableInterruptAtPowerOn() is not disabling all available
 *                                interrupt sources and/or causing an exception during NVIC_IPR access;
 *                                introduced BRSHW_MCORE_NVIC_MAX_IPR_NO, changed BRSHW_MCORE_NVIC_IPR usage,
 *                                added a check for supported cores, fixed support for Cortex-M23
 *  02.02.02  2022-03-25  visrgm  Added BRSHW_PRE_ASM_STARTUP_HOOK_AVAILABLE define to ARMStartup_CortexM.c
 *            2022-03-25  visjhr  Fixed BrsHwDisableInterruptAtPowerOn (HALBE-5486)
 *            2022-04-04  visbwa  Added usage of brsIdx for brsNvicIprTmpValue calculation (prevent compiler warning)
 *  02.02.03  2022-04-05  visrgm  Added LDR R0, =brsPreAsmStartupHook instruction to ARMStartup_CortexM.c
 *  02.02.04  2022-05-17  visrgm  Corrected loading of BrsMain_CoreConfig_Size in Brs_IarLoadCoreConfigSizeSymbol()
 *  02.02.05  2022-05-18  visrgm  Fixed version defines
 *  02.03.00  2022-05-05  visjhr  Mapping of exception table via MemMap (HALBE-6474)
 *  02.03.01  2022-06-28  virjas  ESCAN00112189 - LLVMTexasInstruments can't return from Brs_EnableFpu
 *  02.03.02  2022-07-19  visrgm  Added support of brsPreAsmStartupHook for ARM5 compiler
 *  02.03.03  2022-08-02  visrgm  Correction for ARM5
 *  02.03.04  2022-08-09  visbwa  Update to Brs_Template 1.03.05; UsageType=external in ALM (DemoComponents concept)
 *  02.04.00  2022-08-19  visbwa  Added support for LlvmHightec compiler
 *  02.04.01  2022-10-05  vismaa  Combined ARM5 and ARM6 for Macros used in vBRS generated vBrs_Lcfg.c
 *            2022-10-11  visrgm  Added encapsulation to BrsHwSoftwareResetECU()
 *  02.04.02  2022-11-10  mhu     Fixed support for ARM6 compiler and Cortex-M0
 *            2022-11-15  visbwa  Simplified and fixed Arm-v6M implementation in ARMStartup_CortexM.c
 *  02.04.03  2022-11-18  visbwa  Fixed support for GHS compiler and Cortex-M0 in ARMStartup_CortexM.c
 *  02.04.04  2023-01-09  visjhr  Changed BrsHwSoftwareResetECU to generate a reset on some Cortex-M4 (HSM) cores
 *  02.04.05  2023-03-07  vishci  Added Llvmti support for Cortex-M0 and Cortex-M0Plus cores
 *********************************************************************************************************************/

#ifndef _ARMBRSHW_CORTEX_M_H_
#define _ARMBRSHW_CORTEX_M_H_

/**********************************************************************************************************************
 *  MODULE VERSION
 *********************************************************************************************************************/
/*
 * Description: This is the BrsHw main and bug fix version. The version numbers are BCD coded.
 *              E.g. a main version of 1.23 is coded with 0x0123, a bug fix version of 9 is coded 0x09.
 */
#define ARMBRSHW_CORTEXM_VERSION        0x0204u
#define ARMBRSHW_CORTEXM_BUGFIX_VERSION 0x00u

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/*
 * Description: The BrsCfg header is used to configure different types of
 *              tests and system setups. Therefore it must be included first
 *              in each BRS and test module.
 *              This file is part of the BRS.
 */
#include "vBrsCfg.h"

#include "BrsMain.h"

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/
/* Configuration checks performed within platform specific code (BrsHw) */

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/*
 * Description: Macro for access to IO addresses
 */
#define BRSHW_IOS(type, address) (*((volatile type *)(address)))

#define BRSHWNOP10() do { \
  __asm(" NOP");          \
  __asm(" NOP");          \
  __asm(" NOP");          \
  __asm(" NOP");          \
  __asm(" NOP");          \
  __asm(" NOP");          \
  __asm(" NOP");          \
  __asm(" NOP");          \
  __asm(" NOP");          \
  __asm(" NOP");          \
  } while(0)

/* 
 * Description: Macros used in vBRS generated vBrs_Lcfg.c.
 */
  #define BRSHW_DEFINE_STARTUP_STACK(x)  _STACK_C##x##_LIMIT

  /* No separate ExcVec table on cortexM! OS is defining one int table for exceptions + interrupts */
  #define BRSHW_DEFINE_EXCVEC(x)         _OS_INTVEC_CORE##x##_CONST_START
  #define BRSHW_DEFINE_INTVEC(x)         _OS_INTVEC_CORE##x##_CONST_START

/* Special value, needed for SingleCore UseCase w/o OS (e.g. FBL). vBRS will generate "BRSHW_DEFINE_STARTUP_STACK(BRSHW_INIT_CORE_ID)"
   for this. Configure here the valid value for the StartupStack pointer label of the boot core */
#define _STACK_CBRSHW_INIT_CORE_ID_LIMIT _STACK_C0_LIMIT /* this macro is not used with Arm5/Arm6 compilers */

/**********************************************************
 * NVIC Interrupt Controller Macros
 *********************************************************/
/* If default address for NVIC_BASE is not sufficient for the depending platform,
 * the NVIC_BASE define for NVIC Base address definition has to be declared inside
 * platform specific BrsHw.h, just before ArmBrsHw.h is included. */
#if !defined (NVIC_BASE)
  #define NVIC_BASE  0xE000E000UL  /* NVIC Base address definition */
#endif
  #define NVIC_ISER(x)  BRSHW_IOS(uint32, NVIC_BASE + 0x100UL + x*4U) /* Interrupt Set-Enable Registers */
  #define NVIC_ICER(x)  BRSHW_IOS(uint32, NVIC_BASE + 0x180UL + x*4U) /* Interrupt Clear-Enable Registers */
  #define NVIC_ISPR(x)  BRSHW_IOS(uint32, NVIC_BASE + 0x200UL + x*4U) /* Interrupt Set-Pending Registers */
  #define NVIC_ICPR(x)  BRSHW_IOS(uint32, NVIC_BASE + 0x280UL + x*4U) /* Interrupt Clear-Pending Registers */
  #define NVIC_IPR(x)   BRSHW_IOS(uint32, NVIC_BASE + 0x400UL + ((x/4uL) * 4uL)) /* Interrupt Priority Registers */

  #define NVIC_VTOR     (0xE000ED08) /* Vector Table Offset Register, to be used in assembly StartupCode */
  #define NVIC_VTOR_LOW  0xED08 /* Low word of Vector Table Offset Register, to be used in TI assembly StartupCode */
  #define NVIC_VTOR_HIGH 0xE000 /* High word of Vector Table Offset Register, to be used in TI assembly StartupCode */

/**********************************************************************************************************************
 * Compiler abstraction
 *********************************************************************************************************************/
#if defined (BRS_COMP_IAR)
  #define ___asm(c)                __asm_(c)
  #define __asm_(c)                #c "\n"
  #define __as1(c, d)              __as1_(c, d)
  #define __as1_(c, d)             #c " , "  #d "\n"
  #define __as2(c, d, e)           __as2_(c, d, e)
  #define __as2_(c, d, e)          #c " , " #d " , " #e "\n"
  #define __as3(c, d, e, f)        __as3_(c, d, e, f)
  #define __as3_(c, d, e, f)       #c " , " #d " , " #e " , " #f "\n"
  #define __as4(c, d, e, f, g)     __as4_(c, d, e, f, g)
  #define __as4_(c, d, e, f, g)    #c " , " #d " , " #e " , " #f  " , " #g "\n"
  #define __as5(c, d, e, f, g, h)  __as5_(c, d, e, f, g, h)
  #define __as5_(c, d, e, f, g, h) #c " , " #d " , " #e " , " #f  " , " #g " , " #h "\n"

  #define BRS_MULTILINE_ASM_BEGIN() asm("\n"
  #define BRS_MULTILINE_ASM_END() "\n");

  #define BRS_ASM_EQU(Label, Value) ___asm(Label .equ Value);

  #define BRS_ISR_KEYWORD

  #define BRS_SECTION_CODE(c)     /* empty by intension */

  #define BRS_GLOBAL(c)           __task void c (void) {
  #define BRS_LOCAL_PROTOTYPE(c)  __task void c (void);
  #define BRS_LABEL(c)            #c ": \n"
  #define BRS_GLOBAL_END()  }

  /* Unconditional branch to c */
  #define BRS_BRANCH(c)           "B " #c " \n"
  #define BRS_BRANCH_LINK(c)      "BL " #c " \n"
  #define BRS_EXTERN_BRANCH(c)    c();

  /* Branch to e if c and d are equal */
  #define BRS_BRANCH_EQUAL(c,d,e)   "CMP " #c " , " #d "\n" \
                                    "BEQ " #e "\n"

  /* Branch to e if c and d are NOT equal */
  #define BRS_BRANCH_NOT_EQUAL(c,d,e)   __as1(CMP   c, d) \
                                        ___asm(BNE   e)

  /* Branch to e if c is greater than d */
  #define BRS_BRANCH_GREATER_THAN(c,d,e)   "CMP " #c " , " #d "\n" \
                                           "BGT " #e "\n"

  /* Branch to e if c is greater or equal than d */
  #define BRS_BRANCH_GREATER_THAN_OR_EQUAL(c,d,e)  "CMP " #c " , " #d "\n" \
                                                   "BGE " #e "\n"

#else
  #error "Compiler not yet supported"
#endif /*BRS_COMP_x*/

/**********************************************************************************************************************
 *  BrsHW configuration
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL VARIABLES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/*****************************************************************************/
/* @brief      Disable the global system interrupt.
 * @pre        Must be the first function call in main@BrsMain
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from main@BrsMain at power on initialization
 *****************************************************************************/
void BrsHwDisableInterruptAtPowerOn(void);

/*****************************************************************************/
/* @brief      Program the vector table base addresses
 * @pre        -
 * @param[in]  ExcVecLabel - address of the core exception table
 *             IntVecLabel - address of the interrupt vector table
 *             Use BRS_DEFINE_ADDRESS_UNUSED for unused values
 * @param[out] -
 * @return     -
 * @context    Function is called from main@BrsMain at power on initialization
 *****************************************************************************/
void BrsHw_ExceptionTable_Init(Brs_AddressOfConstType, Brs_AddressOfConstType);

/*****************************************************************************/
/* @brief      Restart ECU (issue a software reset or jump to startup code)
 * @pre        -
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from e.g. ECU state handling
 *****************************************************************************/
void BrsHwSoftwareResetECU(void);

/*****************************************************************************/
/* @brief      This API is used for the BRS time measurement support to get a
 *             default time value for all measurements with this platform to
 *             be able to compare time measurements on different dates based
 *             on this time result.
 * @pre        Should be called with interrupts global disabled
 * @param[in]  -
 * @param[out] -
 * @return     -
 * @context    Function is called from e.g. component testsuits for calibration
 *****************************************************************************/
void BrsHwTime100NOP(void);

/*****************************************************************************/
/* @brief      This API is used to enable an interrupt source in the core
 *             interrupt controller (NVIC).
 * @pre        -
 * @param[in]  Source to be enabled.
 * @param[in]  Priority level to be set.
 * @param[out] -
 * @return     -
 * @context    Function is called from HlpTest and other test environments.
 *****************************************************************************/
void BrsHw_EnableInterrupt(uint32, uint8);

/*****************************************************************************/
/* @brief      This API is used to disable an interrupt source in the core
 *             interrupt controller (NVIC).
 * @pre        -
 * @param[in]  Source to be disabled.
 * @param[out] -
 * @return     -
 * @context    Function is called from HlpTest and other test environments.
 *****************************************************************************/
void BrsHw_DisableInterrupt(uint32);

/*****************************************************************************/
/* @brief      This API is used to trigger the given software interrupt source.
 * @pre        -
 * @param[in]  Source to be triggered.
 *             Some derivatives only support few software triggerable sources,
 *             check for their validity.
 * @param[out] -
 * @return     -
 * @context    Function is called from HlpTest and other test environments.
 *****************************************************************************/
void BrsHw_TriggerSoftwareInterrupt(uint32);

#endif /*_ARMBRSHW_CORTEX_M_H_*/
