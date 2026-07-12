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
 * \addtogroup   Os_Hal_Entry
 * \{
 *
 * \file  Os_Hal_Entry_CYT2.inc
 * \brief  This component handles hardware exceptions and interrupts.
 * \internal
 *  Hardware manuals: See derivative specific header (Os_Hal_Derivative_CYT2Int.h)
 *  Errata sheets:    See derivative specific header
 *  Specifics: -
 *  Used registers and bits: General Purpose and System Registers
 *  Operating modes: -
 *  Hardware features related to independence or partitioning: -
 *  Access mechanism: Core special function registers
 *  Hardware diagnostics: -
 * \endinternal
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_ENTRY_CYT2_INC
# define OS_HAL_ENTRY_CYT2_INC

  OS_HAL_ASM_IMPORT(OsCfg_Hal_Core_PhysicalCore0_SystemInterruptVectorTable)
  OS_HAL_ASM_EXPORT(Os_Hal_UnhandledIRQ_SystemInterrupt)

/***********************************************************************************************************************
 *  Os_Hal_UnhandledIRQ_SystemInterrupt()
 **********************************************************************************************************************/
/*! \brief        Second level UnhandledIRQ handler.
 *  \details      Branches to Os_HandledIrq with function paramters saved by the first level interrupt controller
 *                specific handler.
 *                Restores the context saved by the first level interrupt controller specific handler.
 *
 *  \context      ANY
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
  OS_HAL_ASM_THUMB_FUNCTION
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_UnhandledIRQ_SystemInterrupt)
  bl   Os_UnhandledIrq          /* call the main ISR wrapper */
  pop  {r1-r2, r4, lr}                              /* restore the interrupt level */
  mov  r0, sp                                       /* prepare the exception frame pointer for RFE */
  msr  basepri, r2                                  /* restore the interrupt level before interrupt */
  OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME                /* use SvCall to unstack the original exception frame */
  OS_HAL_ASM_DUMMY_BRANCH                           /* Dummy unconditional branch: never executed. */
  OS_HAL_ASM_ALIGN4  
  OS_HAL_ASM_LTORG                                  /* ESCAN00101862 fix  */
  OS_HAL_ASM_FUNCTION_END(Os_Hal_UnhandledIRQ_SystemInterrupt)

/***********************************************************************************************************************
 *  Os_Hal_Entry_SystemInterrupt()
 **********************************************************************************************************************/
/*! \brief        Second level CAT 0 and 1 interrupt handler.
 *  \details      Branches to given JumpTarget. Used for Cat1 and Cat0 Interrupts.
 *                Restores the context saved by the first level interrupt controller specific handler.
 *
 *  \param[in]    JumpTarget  Address of function which shall be executed.
 *
 *  \context      ANY
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_HAL_ASM_MACRO_BEGIN_1(Os_Hal_Entry_SystemInterrupt, OS_HAL_ASM_MACRO_PARAM_IN_HEADER(JumpTarget))
  OS_HAL_ASM_MACRO_BEGIN_2(Os_Hal_Entry_SystemInterrupt, OS_HAL_ASM_MACRO_PARAM_IN_HEADER(JumpTarget))
  OS_HAL_ASM_MACRO_BEGIN_3(Os_Hal_Entry_SystemInterrupt, OS_HAL_ASM_MACRO_PARAM_IN_HEADER(JumpTarget))
  bl   OS_HAL_ASM_MACRO_PARAM_IN_BODY(JumpTarget) /* call the user ISR */
  pop  {r1-r2, r4, lr}                           /* restore the interrupt level */
  mov  r0, sp                                    /* prepare the exception frame pointer for RFE */
  msr  basepri, r2                               /* restore the interrupt level before interrupt */
  OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME             /* use SvCall to unstack the original exception frame */
  OS_HAL_ASM_DUMMY_BRANCH                        /* Dummy unconditional branch: never executed. */
  OS_HAL_ASM_ALIGN4  
  OS_HAL_ASM_LTORG                               /* ESCAN00101862 fix  */
  OS_HAL_ASM_MACRO_END

/***********************************************************************************************************************
 *  Os_Hal_Entry_Cat2SystemInterrupt()
 **********************************************************************************************************************/
/*! \brief        Second level CAT 2 interrupt handler.
 *  \details      Branches to Os_IsrRun with the given IsrConfig. Used for Cat2 Interrupts.
 *                Restores the context saved by the first level interrupt controller specific handler.
 *
 *  \param[in]    IsrConfig  ISR configuration which shall be executed by high level ISR handler.
 *
 *  \context      ANY
 *
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *
 *  \pre          -
 **********************************************************************************************************************/
OS_HAL_ASM_MACRO_BEGIN_1(Os_Hal_Cat2Entry_SystemInterrupt, OS_HAL_ASM_MACRO_PARAM_IN_HEADER(IsrConfig))
  OS_HAL_ASM_MACRO_BEGIN_2(Os_Hal_Cat2Entry_SystemInterrupt, OS_HAL_ASM_MACRO_PARAM_IN_HEADER(IsrConfig))
  OS_HAL_ASM_MACRO_BEGIN_3(Os_Hal_Cat2Entry_SystemInterrupt, OS_HAL_ASM_MACRO_PARAM_IN_HEADER(IsrConfig))
  Os_Hal_Asm_Load_Register_Immediate  r0, OS_HAL_ASM_MACRO_PARAM_IN_BODY(IsrConfig)
  bl   Os_IsrRun                                     /* call the system ISR handler */
  pop  {r1-r2, r4, lr}                               /* restore the interrupt level */
  mov  r0, sp                                        /* prepare the exception frame pointer for RFE */
  msr  basepri, r2                                   /* restore the interrupt level before interrupt */
  OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME                 /* use SvCall to unstack the original exception frame */
  OS_HAL_ASM_DUMMY_BRANCH                            /* Dummy unconditional branch: never executed. */
  OS_HAL_ASM_ALIGN4  
  OS_HAL_ASM_LTORG                                   /* ESCAN00101862 fix  */
  OS_HAL_ASM_MACRO_END

OS_HAL_ASM_INIT_LABEL(Os_Hal_Cm4_Int_Status_Address_Label,OS_HAL_CPUSS_CM4_SYSTEM_INT_STATUS_ASM)
/***********************************************************************************************************************
 *  Os_Hal_Entry_Interrupt_CYT2()
 **********************************************************************************************************************/
/*! \brief        Low level ISR handler which saves minimum of interrupted context to be able to call high level ISR
 *                handler.
 *  \details      In case of Traveo2 interrupt controller, the system interrupt source has to be read in order to
 *                execute the correct system interrupt handler.
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/

OS_HAL_ASM_MACRO_BEGIN_1(Os_Hal_Entry_Interrupt_CYT2, OS_HAL_ASM_MACRO_PARAM_IN_HEADER(InternalInterruptId))
  OS_HAL_ASM_MACRO_BEGIN_2(Os_Hal_Entry_Interrupt_CYT2, OS_HAL_ASM_MACRO_PARAM_IN_HEADER(InternalInterruptId))
  OS_HAL_ASM_MACRO_BEGIN_3(Os_Hal_Entry_Interrupt_CYT2, OS_HAL_ASM_MACRO_PARAM_IN_HEADER(InternalInterruptId))
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3        /* disable all interrupts */
  mrs  r1, control
  ldr  r12, [sp, #0x18]   /* get ReturnAddress from exception frame */
  push {r1-r2,r4,lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20      /* reserve the space for the frame copy */
  mov  r3, #0x1000000     /* prepare xPSR with only T bit set */
  str  r3, [sp, #0x1C]
  mvn  lr, #0x6           /* prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism */

  ldr  r0, =OS_HAL_ASM_MACRO_PARAM_IN_BODY(InternalInterruptId)
  Os_Hal_Asm_Load_Register_Label r1, Os_Hal_Cm4_Int_Status_Address_Label
  ldr r2, [r1, r0, lsl #2]   /* prepare the address of CM4_INT_STATUS[InternalInterruptId] in r2 */
  clz  r3, r2                /* Read the negated bit value of CM4_INT_STATUS.SYSTEM_INT_VALID[31] in r3 */
  str  r3, [sp, #0xC]        /* store the CM4_INT_STATUS.SYSTEM_INT_VALID[31] in r3 of the new exception frame */
  movw r3, #0x3FF            /* prepare a bit mask with bits [9:0] set to 1 in r3 */
  and r2,r2,r3               /* Read the value of CM4_INT_STATUS.SYSTEM_INT_IDX[9:0] in r2 (ExternalInterruptSource) */

  str  r12, [sp, #0x4]       /* store ReturnAddress of the first exception frame in r1 of the new one */
  ldr  r12, [sp, #0x20]      /* load CONTROL.nPRIV bit at exception entry */
  sub  r12, r12, #1          /* r3 = r3 - 1 --- 0(User) !0(Supervisor) */
  str  r12, [sp, #0x8]       /* store privilege level in r2 of the new one */
  str  r2, [sp, #0x0]        /* store the external interrupt id in r0 of the new exception frame */

  mov  r1, pc
  add  r1, r1, #0xC       /* Dependend on the placement of the previous instruction */
                          /* the result will be address of the "nop" */
                          /* or the "ldr" instruction */
  str  r1, [sp, #0x18]    /* replace the PC in the exceptions frame */
  dsb
  bx   lr

  /* continue in thread mode here */
  nop
  /* or here */

  ldr r4, =OsCfg_Hal_Core_PhysicalCore0_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]
  cmp   r3, #0x0           /* check interrupt id valid bit. */
  it    eq

  bxeq  r4                            /* Jump to the SystemInterruptVectorTable Entry, if it is a valid interrupt id */
  pop  {r1-r2, r4, lr}                /* restore the interrupt level */
  mov  r0, sp                         /* prepare the exception frame pointer for RFE */
  msr  basepri, r2                    /* restore the interrupt level before interrupt */
  OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME  /* use SvCall to unstack the original exception frame */
  OS_HAL_ASM_DUMMY_BRANCH             /* Dummy unconditional branch: never executed. */
  OS_HAL_ASM_ALIGN4  
  OS_HAL_ASM_LTORG                    /* ESCAN00101862 fix  */
  OS_HAL_ASM_MACRO_END

#endif /* defined(OS_HAL_ENTRY_CYT2_INC) */

/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Entry_CYT2.inc
 *********************************************************************************************************************/
