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
/** \file  File:  ARMStartup_CortexM.c
 *      Project:  Vector Basic Runtime System
 *       Module:  BrsHw for all platforms with ARM core Cortex-M
 *     Template:  This file is reviewed according to Brs_Template@Implementation[1.03.05]
 *
 *  \brief Description:  This file contains the assembler part of the BRS StartUpCode.
 *
 *  \attention Please note:
 *    The demo and example programs only show special aspects of the software. With regard to the fact
 *    that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *    expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to ARMBrsHw_CortexM.h.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  EXAMPLE CODE ONLY
 *  -------------------------------------------------------------------------------------------------------------------
 *  This Example Code is only intended for illustrating an example of a possible BSW integration and BSW configuration.
 *  The Example Code has not passed any quality control measures and may be incomplete. The Example Code is neither
 *  intended nor qualified for use in series production. The Example Code as well as any of its modifications and/or
 *  implementations must be tested with diligent care and must comply with all quality requirements which are necessary
 *  according to the state of the art before their use.
 *********************************************************************************************************************/

#define ARMSTARTUP_CORTEXM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "BrsHw.h"

#if defined (BRS_FIRST_EXECUTION_INSTANCE)
/* This code is only needed for the first instance/executable in the system */

#include "vBrs_Lcfg.h"
#include "vLinkGen_Lcfg.h"

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/
#if defined (BRS_COMP_GHS)                  

#else
  #error "Unknown compiler specified!"
#endif


#if (VLINKGEN_CFG_MAJOR_VERSION != 2u)
  #error "This StartUpCode is dependent to the vLinkGen version! vLinkGen major version does not fit!"
#else
# if (VLINKGEN_CFG_MINOR_VERSION < 5u)
  #error "This StartUpCode is dependent to the vLinkGen version! Your vLinkGen minor version is too old!"
# endif
#endif

/**********************************************************************************************************************
 *  DEFINITION + MACROS
 *********************************************************************************************************************/
extern uint32 intvect_CoreExceptions;

#if defined (BRS_CPU_CORE_CORTEX_M0) || defined (BRS_CPU_CORE_CORTEX_M0PLUS)
  /* feature set from Arm-v6M differs to Arm-v7M/Arm-v8M */
  #define BRS_CPU_CORE_ARCHITECTURE_ARM_V6M
#endif

/**********************************************************************************************************************
 *  PROTOTYPES OF LOCAL FUNCTIONS
 *********************************************************************************************************************/
#if defined (BRS_FPU_USED)
void Brs_EnableFpu(void);
#endif
BRS_LOCAL_PROTOTYPE(_start)
BRS_LOCAL_PROTOTYPE(brsStartupEntry)
BRS_LOCAL_PROTOTYPE(brsStartupZeroInitLoop)
BRS_LOCAL_PROTOTYPE(brsStartupStackSearch)
BRS_LOCAL_PROTOTYPE(coreRegisterInit)
BRS_LOCAL_PROTOTYPE(coreRegisterInit2)
BRS_LOCAL_PROTOTYPE(coreRegisterInit3)
BRS_LOCAL_PROTOTYPE(stackPointerInit)
BRS_LOCAL_PROTOTYPE(brsPreAsmStartupHook)


#if defined (BRS_FPU_USED)
#define BRSHW_CPACR (*((volatile uint32 *)(0xE000ED88UL)))
#define BRSHW_FPCCR (*((volatile uint32 *)(0xE000EF34UL)))
/* Enable the FPU coprocessors, because some HwSpec functions and tests use floating point numbers. */
/* must be called from startup code as Os_Init expects the FPU registers to be setup by the customer. */
void Brs_EnableFpu(void)
{
  BRSHW_CPACR = BRSHW_CPACR | (0xFUL << 20);
  BRSHW_FPCCR = (BRSHW_FPCCR & ~(0x1UL << 30)) | (0x1UL << 31); /* Set FPCCR.ASPEN, Reset FPCCR.LSPEN. */
}
#endif /*BRS_FPU_USED*/

/* =========================================================================== */
/*                                                                             */
/* Description: Entry point for all cores                                      */
/*                                                                             */
/* =========================================================================== */



BRS_SECTION_CODE(brsStartup)
 BRS_GLOBAL(brsStartupEntry)
BRS_MULTILINE_ASM_BEGIN()
BRS_LABEL(brsStartupEntry)
#if defined (BRSHW_PRE_ASM_STARTUP_HOOK_AVAILABLE)
#endif


 BRS_BRANCH(brsPreAsmStartupHook)
BRS_MULTILINE_ASM_END()
BRS_GLOBAL_END()


/* =========================================================================== */
/*                                                                             */
/* Description: Optional hook for platform specific tasks                      */
/*                                                                             */
/* =========================================================================== */
#if !defined (BRSHW_PRE_ASM_STARTUP_HOOK_AVAILABLE)
 BRS_GLOBAL(brsPreAsmStartupHook)
BRS_MULTILINE_ASM_BEGIN()
BRS_LABEL(brsPreAsmStartupHook)
/* Nothing to do here */

 BRS_BRANCH(coreRegisterInit)
BRS_MULTILINE_ASM_END()
BRS_GLOBAL_END()
#endif /*!BRSHW_PRE_ASM_STARTUP_HOOK_AVAILABLE*/

/* =========================================================================== */
/*                                                                             */
/* Description: Initialize core ID independent core registers                  */
/*                                                                             */
/* =========================================================================== */
 BRS_GLOBAL(coreRegisterInit)
BRS_MULTILINE_ASM_BEGIN()
BRS_LABEL(coreRegisterInit)
/* Nothing to do for Cortex-M */

 BRS_BRANCH(coreRegisterInit2)
BRS_MULTILINE_ASM_END()
BRS_GLOBAL_END()

/* =========================================================================== */
/*                                                                             */
/* Description: Initialize all core ID dependent core registers                */
/*              Configure VTOR address.                                        */
/*                                                                             */
/* =========================================================================== */
 BRS_GLOBAL(coreRegisterInit2)
BRS_MULTILINE_ASM_BEGIN()
BRS_LABEL(coreRegisterInit2)

 __as1(LDR  R1, =NVIC_VTOR)
 __as1(LDR  R2, =intvect_CoreExceptions)

 __as1(STR R2, [R1])

 BRS_BRANCH(brsStartupZeroInitLoop)
BRS_MULTILINE_ASM_END()
BRS_GLOBAL_END()

/* =========================================================================== */
/*                                                                             */
/* Description: Initialize memory blocks and groups with zero                  */
/*                                                                             */
/* =========================================================================== */
 BRS_GLOBAL(brsStartupZeroInitLoop)
BRS_MULTILINE_ASM_BEGIN()
BRS_LABEL(brsStartupZeroInitLoop)

 BRS_READ_COREID(R0)

/* Initialize memory sections blocks with zeros */
#if defined (VLINKGEN_CFG_NUM_ZERO_INIT_EARLY_BLOCKS)
# if (VLINKGEN_CFG_NUM_ZERO_INIT_EARLY_BLOCKS>1uL)
 __as1(LDR  R1, =vLinkGen_ZeroInit_Early_Blocks)

BRS_LABEL(startup_block_zero_init_start)
 __as1(MOV  R2, R1)

#   if defined (BRS_COMP_GHS) 
 __as2(ADD  R1, R1, 16)
#   else
 __as2(ADD  R1, R1, #16)
#   endif

 __as1(LDR  R3, [R2])      /* vLinkGen_ZeroInit_Early_Blocks->start */
 __as2(LDR  R4, [R2, #4])  /* vLinkGen_ZeroInit_Early_Blocks->end */
 __as2(LDR  R5, [R2, #8])  /* vLinkGen_ZeroInit_Early_Blocks->core */
 __as2(LDR  R6, [R2, #12]) /* vLinkGen_ZeroInit_Early_Blocks->alignment */

/* Verify if the end of struct vLinkGen_ZeroInit_Early_Blocks is reached, by checking if start == 0, end == 0 and core == 0 */
#  if defined (BRS_CPU_CORE_ARCHITECTURE_ARM_V6M)
 __as1(MOVS  R7, #0)
#  else
#   if defined (BRS_COMP_GHS) 
 __as1(MOV  R7, 0)
#   else
 __as1(MOV  R7, #0)
#   endif
#  endif

#  if defined (BRS_COMP_GHS) && defined (BRS_CPU_CORE_ARCHITECTURE_ARM_V6M)
  __as1(ORR  R7, R3) /* Or with vLinkGen_ZeroInit_Early_Blocks->start */
  __as1(ORR  R7, R4) /* Or with vLinkGen_ZeroInit_Early_Blocks->end */
  __as1(ORR  R7, R5) /* Or with vLinkGen_ZeroInit_Early_Blocks->core */
#  else
  __as2(ORR  R7, R7, R3) /* Or with vLinkGen_ZeroInit_Early_Blocks->start */
  __as2(ORR  R7, R7, R4) /* Or with vLinkGen_ZeroInit_Early_Blocks->end */
  __as2(ORR  R7, R7, R5) /* Or with vLinkGen_ZeroInit_Early_Blocks->core */
#  endif

#  if defined (BRS_COMP_GHS) 
 BRS_BRANCH_EQUAL(R7, 0, startup_block_zero_init_end) /* If start address, end address and core are equal to zero -> Finished */
#  else
 BRS_BRANCH_EQUAL(R7, #0, startup_block_zero_init_end) /* If start address, end address and core are equal to zero -> Finished */
#  endif

 BRS_BRANCH_NOT_EQUAL(R0, R5, startup_block_zero_init_start) /* If InitCore is not running -> go to the next array entry */

#  if defined (BRS_CPU_CORE_ARCHITECTURE_ARM_V6M)
 __as1(MOVS  R2, #0) /* set R2 to '0', as value to write in memory later */
 __as1(MOVS  R7, #0) /* set R7 to '0', as value to write in memory later */
#  else
#   if defined (BRS_COMP_GHS) 
 __as1(MOV  R2, 0) /* set R2 to '0', as value to write in memory later */
 __as1(MOV  R7, 0) /* set R7 to '0', as value to write in memory later */
#   else
 __as1(MOV  R2, #0) /* set R2 to '0', as value to write in memory later */
 __as1(MOV  R7, #0) /* set R7 to '0', as value to write in memory later */
#   endif
#  endif

 BRS_BRANCH_EQUAL(R6, #8, startup_block_zero_init_8byte_loop_start) /* if block is 8-Byte aligned, use specific init loop */

BRS_LABEL(startup_block_zero_init_loop_start)
 BRS_BRANCH_GREATER_THAN_OR_EQUAL(R3, R4, startup_block_zero_init_start) /* if start address is same with or higher than end address-> Finished. */

 __as1(STR  R7, [R3]) /* write value of R7 ('0') to address of R3 (4-byte access); must be an aligned memory access! */

#   if defined (BRS_COMP_GHS) 
 __as2(ADD  R3, R3, 4) /* increase address value of R3 by '4' */
#   else
 __as2(ADD  R3, R3, #4) /* increase address value of R3 by '4' */
#   endif

/* End address of this block was not yet reached. Run through the loop again */
 BRS_BRANCH(startup_block_zero_init_loop_start)

/* ================================================================ */
/* 8-Byte aligned initialization, to support 8-Byte aligned ECC RAM */
/* ================================================================ */
BRS_LABEL(startup_block_zero_init_8byte_loop_start)
 BRS_BRANCH_GREATER_THAN_OR_EQUAL(R3, R4, startup_block_zero_init_start) /* if start address is same with or higher than end address-> Finished. */

/* Address stored in R3 must be 8 Byte aligned at this point! */
 __as2(STMIA R3!, {R2, R7}) /* 8 Byte alignment store for ECC, R3 would be automatically updated after the write */

/* End address of this block was not yet reached. Run through the loop again */
 BRS_BRANCH(startup_block_zero_init_8byte_loop_start)

/* Zero-Init loop of blocks end label */
BRS_LABEL(startup_block_zero_init_end)
# endif /*VLINKGEN_CFG_NUM_ZERO_INIT_EARLY_BLOCKS>1uL*/

#else
  #error "Mandatory define VLINKGEN_CFG_NUM_ZERO_INIT_EARLY_BLOCKS missing within vLinkGen configuration!"
#endif /*VLINKGEN_CFG_NUM_ZERO_INIT_EARLY_BLOCKS*/

/* Initialize memory sections groups with zeros */
#if defined (VLINKGEN_CFG_NUM_ZERO_INIT_EARLY_GROUPS)
# if (VLINKGEN_CFG_NUM_ZERO_INIT_EARLY_GROUPS>1uL)
 __as1(LDR  R1, =vLinkGen_ZeroInit_Early_Groups)

BRS_LABEL(startup_group_zero_init_start)
 __as1(MOV  R2, R1)

#   if defined (BRS_COMP_GHS) 
 __as2(ADD  R1, R1, 16)
#   else
 __as2(ADD  R1, R1, #16)
#   endif

 __as1(LDR  R3, [R2])      /* vLinkGen_ZeroInit_Early_Groups->start */
 __as2(LDR  R4, [R2, #4])  /* vLinkGen_ZeroInit_Early_Groups->end */
 __as2(LDR  R5, [R2, #8])  /* vLinkGen_ZeroInit_Early_Groups->core */
 __as2(LDR  R6, [R2, #12]) /* vLinkGen_ZeroInit_Early_Groups->alignment */

/* Verify if the end of struct vLinkGen_ZeroInit_Early_Groups is reached, by checking if start == 0, end == 0 and core == 0 */
#  if defined (BRS_CPU_CORE_ARCHITECTURE_ARM_V6M)
 __as1(MOVS  R7, #0)
#  else
#   if defined (BRS_COMP_GHS) 
 __as1(MOV  R7, 0)
#   else
 __as1(MOV  R7, #0)
#   endif
#  endif

#  if defined (BRS_COMP_GHS) && defined (BRS_CPU_CORE_ARCHITECTURE_ARM_V6M)
  __as1(ORR  R7, R3) /* Or with vLinkGen_ZeroInit_Early_Groups->start */
  __as1(ORR  R7, R4) /* Or with vLinkGen_ZeroInit_Early_Groups->end */
  __as1(ORR  R7, R5) /* Or with vLinkGen_ZeroInit_Early_Groups->core */
#  else
  __as2(ORR  R7, R7, R3) /* Or with vLinkGen_ZeroInit_Early_Groups->start */
  __as2(ORR  R7, R7, R4) /* Or with vLinkGen_ZeroInit_Early_Groups->end */
  __as2(ORR  R7, R7, R5) /* Or with vLinkGen_ZeroInit_Early_Groups->core */
#  endif

#  if defined (BRS_COMP_GHS) 
 BRS_BRANCH_EQUAL(R7, 0,  startup_group_zero_init_end) /* If start address, end address and core are equal to zero -> Finished */
#  else
 BRS_BRANCH_EQUAL(R7, #0,  startup_group_zero_init_end) /* If start address, end address and core are equal to zero -> Finished */
#  endif

 BRS_BRANCH_NOT_EQUAL(R0, R5, startup_group_zero_init_start) /* If InitCore is not running -> go to the next array entry */

#  if defined (BRS_CPU_CORE_ARCHITECTURE_ARM_V6M)
 __as1(MOVS  R2, #0) /* set R2 to '0', as value to write in memory later */
 __as1(MOVS  R7, #0) /* set R7 to '0', as value to write in memory later */
#  else
#   if defined (BRS_COMP_GHS) 
 __as1(MOV  R2, 0) /* set R2 to '0', as value to write in memory later */
 __as1(MOV  R7, 0) /* set R7 to '0', as value to write in memory later */
#   else
 __as1(MOV  R2, #0) /* set R2 to '0', as value to write in memory later */
 __as1(MOV  R7, #0) /* set R7 to '0', as value to write in memory later */
#   endif
#  endif

 BRS_BRANCH_EQUAL(R6, #8, startup_group_zero_init_8byte_loop_start) /* if group is 8-Byte aligned, use specific init loop */

BRS_LABEL(startup_group_zero_init_loop_start)
 BRS_BRANCH_GREATER_THAN_OR_EQUAL(R3, R4, startup_group_zero_init_start) /* if start address is same with or higher than end address-> Finished. */

 __as1(STR  R7, [R3]) /* write value of R7 ('0') to address of R3 (4-byte access); must be an aligned memory access! */

#   if defined (BRS_COMP_GHS) 
 __as2(ADD  R3, R3, 4) /* increase address value of R3 by '4' */
#   else
 __as2(ADD  R3, R3, #4) /* increase address value of R3 by '4' */
#   endif

/* End address of this group was not yet reached. Run through the loop again */
 BRS_BRANCH(startup_group_zero_init_loop_start)

/* ================================================================ */
/* 8-Byte aligned initialization, to support 8-Byte aligned ECC RAM */
/* ================================================================ */
BRS_LABEL(startup_group_zero_init_8byte_loop_start)
 BRS_BRANCH_GREATER_THAN_OR_EQUAL(R3, R4, startup_group_zero_init_start) /* if start address is same with or higher than end address-> Finished. */

/* Address stored in R3 must be 8 Byte aligned at this point! */
 __as2(STMIA R3!, {R2, R7}) /* 8 Byte alignment store for ECC, R3 would be automatically updated after the write */

/* End address of this group was not yet reached. Run through the loop again */
 BRS_BRANCH(startup_group_zero_init_8byte_loop_start)

/* Zero-Init loop of groups end label */
BRS_LABEL(startup_group_zero_init_end)
# endif /*VLINKGEN_CFG_NUM_ZERO_INIT_EARLY_GROUPS>1uL*/

#else
  #error "Mandatory define VLINKGEN_CFG_NUM_ZERO_INIT_EARLY_GROUPS missing within vLinkGen configuration!"
#endif /*VLINKGEN_CFG_NUM_ZERO_INIT_EARLY_GROUPS*/

/* Jump to routine to search for valid startup stack pointer of actual running core */
 BRS_BRANCH(brsStartupStackSearch)
BRS_MULTILINE_ASM_END()
BRS_GLOBAL_END()

/* =========================================================================== */
/*                                                                             */
/* Description: Search for valid startup stack pointer of actual running core  */
/*              (vBRS is genearing the core specific configuration into the    */
/*               struct BrsMain_CoreConfig in vBrs_Lcfg.c)                     */
/*                                                                             */
/* =========================================================================== */
 BRS_GLOBAL(brsStartupStackSearch)
BRS_MULTILINE_ASM_BEGIN()
BRS_LABEL(brsStartupStackSearch)

 __as1(LDR  R1, =BrsMain_CoreConfig_Size)

BRS_LABEL(core_config_size_init)
 __as1(LDR  R5, [R1])   /* R5 = BrsMain_CoreConfig_Size */

 __as1(LDR  R1, =BrsMain_CoreConfig)

 BRS_READ_COREID(R0)

BRS_LABEL(core_config_init_start)

/* Use R2 as count register in the loop. Initialize with zero. */
#if defined (BRS_CPU_CORE_ARCHITECTURE_ARM_V6M)
 __as1(MOVS  R2, #0)
#else
# if defined (BRS_COMP_GHS) 
 __as1(MOV  R2, 0)
# else
 __as1(MOV  R2, #0)
# endif
#endif

BRS_LABEL(brs_coreconfig_loop)

 __as2(LDR  R4, [R1,#4])  /* BrsMain_CoreConfig->PhysicalCoreId */
/* Check if core id (R0) matches to physical core id of BrsMain_CoreConfig entry (R4). */
 BRS_BRANCH_EQUAL(R0, R4, stackPointerInit)

/* Increase count register by one. */
# if defined (BRS_COMP_GHS) 
 __as2(ADD  R2, R2, 1)
# else
 __as2(ADD  R2, R2, #1)
# endif

/* Set R1 to point to next entry of BrsMain_CoreConfig. */
# if defined (BRS_COMP_GHS) 
 __as2(ADD  R1, R1, 28)
# else
 __as2(ADD  R1, R1, #28)
# endif

/* Check if end of BrsMain_CoreConfig has been reached. */
 BRS_BRANCH_NOT_EQUAL(R2, R5, brs_coreconfig_loop)

BRS_MULTILINE_ASM_END()

/* Branch to BrsMainExceptionStartup if there are no more entries in the array. */
 BRS_EXTERN_BRANCH(BrsMainExceptionStartup)
BRS_GLOBAL_END()

/* =========================================================================== */
/*                                                                             */
/* Description: Initialize all core registers of actual running core with      */
/*              specific init values                                           */
/*                                                                             */
/* =========================================================================== */
 BRS_GLOBAL(stackPointerInit)
BRS_MULTILINE_ASM_BEGIN()
BRS_LABEL(stackPointerInit)
 __as2(LDR  R4, [R1,#8]) /* BrsMain_CoreConfig->StartupStackEndLabel */
 __as1(MOV  SP, R4)

 BRS_BRANCH(coreRegisterInit3)

BRS_MULTILINE_ASM_END()
BRS_GLOBAL_END()

/* =========================================================================== */
/*                                                                             */
/* Description: Initialize additional core registers                           */
/*                                                                             */
/* =========================================================================== */
 BRS_GLOBAL(coreRegisterInit3)
BRS_MULTILINE_ASM_BEGIN()
BRS_LABEL(coreRegisterInit3)

/* =========================================================== */
/* Description: Enable L1 instruction cache on Cortex-M7 cores */
/* =========================================================== */
#if defined (BRS_CPU_CORE_CORTEX_M7)
 __as1(MOV  R0, #0x0)
 /* ICIALLU register is at 0xE000EF50; invalidate the entire instruction cache */
 __as1(MOVW  R1, #0xEF50) /* load low word of ICIALLU register */
 __as1(MOVT  R1, #0xE000) /* load high word of ICIALLU register */
 __as1(STR  R0, [R1])

 /* issue an ISB instruction to flush the pipeline */
 ___asm(ISB)

 /* CCR register is at 0xE000ED14; set CCR.IC field to enable instruction cache */
 __as1(MOVW  R1, #0xED14) /* load low word of CCR register */
 __as1(MOVT  R1, #0xE000) /* load low word of CCR register */

 __as1(LDR  R0, [R1])
 __as2(ORR  R0, R0, #0x20000)
 __as1(STR  R0, [R1])

 /* issue an ISB instruction to flush the pipeline */
 ___asm(ISB)
#endif /*BRS_CPU_CORE_CORTEX_M7*/

BRS_MULTILINE_ASM_END()

#if defined (BRS_FPU_USED)
/* =========================================================== */
/* Description: Enable Floating Point Unit                     */
/* =========================================================== */
BRS_EXTERN_BRANCH(Brs_EnableFpu)
#endif

/* =========================================================================== */
/*                                                                             */
/* Description: Jump to Brs_PreMainStartup() (BrsMainStartup.c)                */
/*                                                                             */
/* =========================================================================== */
 BRS_EXTERN_BRANCH(Brs_PreMainStartup)

/* =========================================================================== */
/*                                                                             */
/* Description: Jump to BrsMainExceptionStartup() in case of an unexpected     */
/*              return from PreMain/main                                       */
/*                                                                             */
/* =========================================================================== */
 BRS_EXTERN_BRANCH(BrsMainExceptionStartup)

BRS_GLOBAL_END()



#endif /*BRS_FIRST_EXECUTION_INSTANCE*/
