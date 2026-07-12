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
 *  \addtogroup   Os_Hal_Context
 *  \{
 *
 *  \file
 *  \brief  This component contains Cortex-M specific assembly functions.
 *  \details
 *  \internal
 *   Hardware manuals:
 *   - ARM Architecture Reference Manual ARMv8-M edition
 *     ARM DDI 0553B.i (ID02102019), 02 October 2019
 *   - ARM Cortex-M33 Processor Technical Reference Manual, revision: r0p4, 18 April 2018
 *   Errata sheets: -
 *   Access mechanism: -
 *   Used registers: -
 *   Hardware features related to independence or partitioning: -
 *   Operating modes: -
 *   Hardware diagnostics: -
 *   Specifics: -
 *  \endinternal
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
#ifndef OS_HAL_CONTEXTASM_V8M_INC
# define OS_HAL_CONTEXTASM_V8M_INC

# include "Os_Hal_MemoryProtection_Cfg.h"
# include "Os_Hal_DerivativeInt.h"
# include "Os_Hal_Compiler.inc"
# include "Os_Hal_Entry_Cfg.h"
# include "Os_Hal_ContextAsm_ExceptionContext.inc"

  OS_HAL_ASM_PRESERVE8

  OS_HAL_ASM_CODE_SECTION(OS_CODE)

  OS_HAL_ASM_THUMB_CODE

  OS_HAL_ASM_EXPORT(Os_Hal_StorePermanentRegisters)
  OS_HAL_ASM_EXPORT(Os_Hal_SysCallHandler)
  OS_HAL_ASM_EXPORT(Os_Hal_UnhandledIRQ)
  OS_HAL_ASM_EXPORT(Os_Hal_UnhandledEXC)
  OS_HAL_ASM_EXPORT(Os_Hal_MemFault)
  OS_HAL_ASM_EXPORT(Os_Hal_StartNewThread)

  OS_HAL_ASM_IMPORT(Os_TrapHandler)
  OS_HAL_ASM_IMPORT(Os_UnhandledSysCall)
  OS_HAL_ASM_IMPORT(Os_UnhandledIrq)
  OS_HAL_ASM_IMPORT(Os_UnhandledExc)
  OS_HAL_ASM_IMPORT(OsCfg_Stack_KernelStacks)
  OS_HAL_ASM_IMPORT(Os_MemFault)
  OS_HAL_ASM_IMPORT(OS_HAL_MP_STACKS_START)
  OS_HAL_ASM_IMPORT(OS_HAL_MP_STACKS_END)

OS_HAL_ASM_INIT_LABEL(Os_Hal_Mpu_Base_Address_Label,OS_HAL_MPU_BASE_ADDRESS_ASM)

/***********************************************************************************************************************
 *  Os_Hal_StartNewThread()
 **********************************************************************************************************************/
/*! \brief        Set up stack pointer and control register, enable interrupts and jump to the entry point of a newly
 *                created thread.
 *  \details      Usage of the registers:
 *                R0 = Parameter
 *                R1 = User Stack Pointer
 *                R2 = Not used
 *                R3 = User Entry Point
 *                R12 = Control Register
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_StartNewThread)
  mov  sp, r1
  cpsie i
  msr  control, r12
  bx   r3
  OS_HAL_ASM_FUNCTION_END(Os_Hal_StartNewThread)


/***********************************************************************************************************************
 *  Os_Hal_StorePermanentRegisters()
 **********************************************************************************************************************/
/*! \brief        Stores the permanent registers (r4-r11) to the address given as argument
 *  \details
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
  OS_HAL_ASM_THUMB_FUNCTION
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_StorePermanentRegisters)
  stm  r0, {r4-r11}
  bx   lr
  OS_HAL_ASM_FUNCTION_END(Os_Hal_StorePermanentRegisters)

/***********************************************************************************************************************
 *  Os_Hal_SysCallHandler()
 **********************************************************************************************************************/
/*! \brief        Low level system call handler which saves minimum of interrupted context to be able to call a
 *                C-function.
 *  \details
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
  OS_HAL_ASM_THUMB_FUNCTION
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_SysCallHandler)
  ldr  r0, [sp, #0x00]    /* For security reason some platforms clear argument registers. */
  ldr  r1, [sp, #0x04]
  ldr  r3, [sp, #0x18]    /* get ReturnAddress from exception frame. */
  ldrb r3, [r3, #-2]      /* Read the x from svc #x */
  cmp  r3, #1
  beq  Os_Hal_ContextSvcallHandler
  cmp  r3, #2
  beq  ReturnFromTrap
  cmp  r3, #0
  beq  CallTrap
  sub  sp, sp, #0x20
  Os_Hal_Asm_Load_Register_Immediate r0, Os_UnhandledSysCall
  str  r0, [sp, #0x18]
  mov  r1, #0x1000000     /* prepare xPSR with only T bit set */
  str  r1, [sp, #0x1C]
  ldr  r2, [sp, #0x38]    /* get ReturnAddress from first exception frame. */
  str  r2, [sp, #0x0]     /* store ReturnAddress of the first exception frame in r0 of the new one. */
  mrs  r3, control
  and  r3, r3, #1         /* r3 = CONTROL.nPRIV bit --- 1(User) 0(Supervisor) */
  sub  r3, r3, #1         /* r3 = r3 - 1 --- 0(User) !0(Supervisor) */
  str  r3, [sp, #0x4]     /* store privileged level before exception in r1 of the new one. */
  mov  r1, #0
  msr   control, r1
  dsb
  bx   lr
OS_HAL_ASM_DEFINE_LABEL(ReturnFromTrap)
  ldr  lr, [sp, #0x14]
  msr  control, r1
  mov  sp, r0
  bx   lr
OS_HAL_ASM_DEFINE_LABEL(CallTrap)
  mrs  r1, control
  mrs  r2, basepri
  push {r1-r2, lr}
  mvn  lr, #0x47          /* LR = 0xFFFFFFB8 to perform next exception return from basic frame. */
  mov  r3, #0
  msr  control, r3
  add  r1, sp, #4
  sub  sp, sp, #0x20      /* reserve the space for the frame copy */
  str  r0, [sp]           /* preserve the parameter to the TrapPacket */
  str  r1, [sp, #0x4]     /* store the pointer to the basepri content */
  mov  r1, #0x1000000     /* prepare xPSR with only T bit set */
  str  r1, [sp, #0x1C]
  mov  r1, pc                                                                                                           /* COMP_WARN_OS_HAL_MOV_PC_BIT_ZERO_NOT_SET */
  add  r1, r1, #0xC       /* Dependend on the placement of this instruction */
                          /* the result will be address of the "nop" */
                          /* or the "bl" instruction */
  str  r1, [sp, #0x18]    /* replace the PC in the exceptions frame */
  dsb
  bx   lr

  /* continue in thread mode here */
  nop
  /* or here */
  bl   Os_TrapHandler
  pop  {r1-r2, lr}
  mov  r0, sp                                        /* prepare the exception frame pointer for RFE */
  msr  basepri, r2
  OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME                 /* use SvCall to unstack the original exception frame */
  OS_HAL_ASM_FUNCTION_END(Os_Hal_SysCallHandler)


/***********************************************************************************************************************
 *  Os_Hal_ContextSvcallHandler()
 **********************************************************************************************************************/
/*! \brief        Assembly function to finish the context switch.
 *  \details
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
  OS_HAL_ASM_THUMB_FUNCTION
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_ContextSvcallHandler)
  /* r0 = Pointer to the Os_Hal_ContextSwitchInfoType structure */
  /* [r0 +  0] = NextSprPtr:        Pointer to Sp, Lr, Primask. */
  /* [r0 +  4] = PermRegsPtr:       Pointer to the permanent registers */
  /* [r0 +  8] = Basepri:           New BASEPRI */
  /* [r0 +  C] = SaveSprPtr:        Address to store the current Sp and Lr */
  /* [r0 + 10] = MpuStackRegionPtr: Pointer to the MPU stack region configuration */
  mov   r1, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr   basepri, r1           /* Disable global interrupt handling */
  ldr   r1, [r0, #0xC]
  cmp   r1, #0
  itt   ne
  strne sp, [r1]
  strne lr, [r1, #0x4]
# if defined(OS_CFG_HAL_MEMORY_PROTECTION_ASM)
  ldr   r1, [r0, #0x10]
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_Mpu_Base_Address_Label
  mov   r2, #OS_HAL_MP_STACK_PROTECTION_REGION_0_NUMBER_ASM
  str   r2, [r3, #0x8]        /* MPU_RNR = OS_HAL_MP_STACK_PROTECTION_REGION_0_NUMBER_ASM */
  bic   r2, r2, #0x1
  str   r2, [r3, #0x10]       /* MPU_RLAR.EN = 0 */
  ldr   r2, =OS_HAL_MP_STACKS_START
  ldr   r2, [r2]
  orr   r2, r2, #0x6          /* OS_HAL_MP_ACCESS_EXECUTE_FORBIDDEN | OS_HAL_MP_ACCESS_S_R_U_R */
  str   r2, [r3, #0xC]        /* MPU_RBAR = Stack regions start address */
  ldr   r2, [r1]              /* Next->StackRegionStart */
  sub   r2, r2, #0x20         /* Align stack protection end address */
  orr   r2, r2, #1            /* OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_NON_CACHEABLE_OUTER_NON_CACHEABLE | OS_HAL_MP_REGION_SIZE_ENABLE */
  str   r2, [r3, #0x10]       /* MPU_RLAR = Next->StackRegionStart */
  isb
  mov   r2, #OS_HAL_MP_STACK_REGION_NUMBER_ASM
  str   r2, [r3, #0x8]        /* MPU_RNR = OS_HAL_MP_STACK_REGION_NUMBER_ASM */
  str   r2, [r3, #0x10]       /* MPU_RLAR.EN = 0 */
  ldr   r2, [r1]
  orr   r2, r2, #0x3          /* OS_HAL_MP_ACCESS_EXECUTE_FORBIDDEN | OS_HAL_MP_ACCESS_S_RW_U_RW */
  str   r2, [r3, #0xC]        /* MPU_RBAR = Next->StackRegionStart */
  ldr   r2, [r1, #4]
  add   r2, r2, #0x20
  orr   r2, r2, #1            /* OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_NON_CACHEABLE_OUTER_NON_CACHEABLE | OS_HAL_MP_REGION_SIZE_ENABLE */
  str   r2, [r3, #0x10]       /* MPU_RLAR = Next->StackRegionInfo */
  isb
  mov   r2, #OS_HAL_MP_STACK_PROTECTION_REGION_1_NUMBER_ASM
  str   r2, [r3, #0x8]        /* MPU_RNR = OS_HAL_MP_STACK_PROTECTION_REGION_1_NUMBER_ASM */
  bic   r2, r2, #0x2
  str   r2, [r3, #0x10]       /* MPU_RLAR.EN = 0 */
  ldr   r2, [r1, #4]
  add   r2, r2, #0x40
  orr   r2, r2, #0x6          /* OS_HAL_MP_ACCESS_EXECUTE_FORBIDDEN | OS_HAL_MP_ACCESS_S_R_U_R */
  str   r2, [r3, #0xC]        /* MPU_RBAR = Stack regions end address */
  ldr   r2, =OS_HAL_MP_STACKS_END
  ldr   r2, [r2, #4]
  orr   r2, r2, #1            /* OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_NON_CACHEABLE_OUTER_NON_CACHEABLE | OS_HAL_MP_REGION_SIZE_ENABLE */
  str   r2, [r3, #0x10]       /* MPU_RLAR = Next->StackRegionStart */
  dsb
  isb
# endif /* OS_CFG_HAL_MEMORY_PROTECTION_ASM */
  ldm   r0, {r1-r3}           /* (r1-r3)=(NextSprPtr,PermRegsPtr,Basepri) */
  ldm   r1, {r5-r7}           /* (r5-r6)=(Sp,Lr, Primask) */
  msr   primask, r7           /* Change the PRIMASK */
  mov   sp, r5                /* Change the current stack */
  mov   lr, r6                /* Change the current link register */
  ldm   r2, {r4-r11}          /* Restore the permanent registers */
  msr   basepri, r3           /* Restore the interrupt level */
  bx    lr                    /* "Return" from exception */
  OS_HAL_ASM_FUNCTION_END(Os_Hal_ContextSvcallHandler)


/***********************************************************************************************************************
 *  Os_Hal_UnhandledIRQ()
 **********************************************************************************************************************/
/*! \brief        Low level exception handler for unhandeld IRQs.
 *  \details
 *  This function calls given function.
 *  \param[in]    JumpTarget  Function which shall be called.
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
  OS_HAL_ASM_THUMB_FUNCTION
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_UnhandledIRQ)
  mov   r1, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr   basepri, r1     /* Disable global interrupt handling, to avoid re-entering the same peripheral interrupt */
  ldr  r3, [sp, #0x18]  /* get ReturnAddress from exception frame. */
  sub  sp, sp, #0x20
  Os_Hal_Asm_Load_Register_Immediate r0, Os_UnhandledIrq
  str  r0, [sp, #0x18]
  mov  r1, #0x01000000    /* prepare xPSR with only T bit set */
  str  r1, [sp, #0x1C]
  str  r3, [sp, #4]       /* store ReturnAddress of the first exception frame in r1 of the new one. */
  mrs  r3, control
  and  r3, r3, #1         /* r3 = CONTROL.nPRIV bit --- 1(User) 0(Supervisor) */
  sub  r3, r3, #1         /* r3 = r3 - 1 --- 0(User) !0(Supervisor) */
  str  r3, [sp, #0x8]     /* store privilege level in r2 of the new one. */
  mov  r1, #0
  msr  control, r1
  mrs  r1, ipsr           /* get ISPR */
  movw r2, #0x1FF
  and  r1, r1, r2         /* get the source number */
  str  r1, [sp, #0x0]     /* prepare it in the argument register */
  mvn  lr, #0x47          /* prepare LR(r14) with the value 0xFFFFFFB8, needed by exception return mechanism */
  dsb
  bx   lr
  OS_HAL_ASM_FUNCTION_END(Os_Hal_UnhandledIRQ)

/***********************************************************************************************************************
 *  Os_Hal_UnhandledEXC()
 **********************************************************************************************************************/
/*! \brief        Low level exception handler for unhandeld exceptions.
 *  \details
 *  This function calls given function.
 *  \param[in]    JumpTarget  Function which shall be called.
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/   
  OS_HAL_ASM_THUMB_FUNCTION
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_UnhandledEXC)
  mov r3, #0                  /* Load core Id of 0 */
  bl Os_Hal_Context_SaveExcContext
  Os_Hal_Asm_Load_Register_Immediate r0, OsCfg_Stack_KernelStacks
  ldr  r1, [r0]
# if defined(OS_CFG_HAL_MEMORY_PROTECTION_ASM)
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_Mpu_Base_Address_Label
  mov   r2, #OS_HAL_MP_STACK_PROTECTION_REGION_0_NUMBER_ASM
  str   r2, [r3, #0x8]        /* MPU_RNR = OS_HAL_MP_STACK_PROTECTION_REGION_0_NUMBER_ASM */
  bic   r2, r2, #0x1
  str   r2, [r3, #0x10]       /* MPU_RLAR.EN = 0 */
  ldr   r2, =OS_HAL_MP_STACKS_START
  ldr   r2, [r2]
  orr   r2, r2, #0x6          /* OS_HAL_MP_ACCESS_EXECUTE_FORBIDDEN | OS_HAL_MP_ACCESS_S_R_U_R */
  str   r2, [r3, #0xC]        /* MPU_RBAR = Stack regions start address */
  ldr   r2, [r1, #4]          /* Next->StackRegionStart */
  sub   r2, r2, #0x20         /* Align stack protection end address */
  orr   r2, r2, #1            /* OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_NON_CACHEABLE_OUTER_NON_CACHEABLE | OS_HAL_MP_REGION_SIZE_ENABLE */
  str   r2, [r3, #0x10]       /* MPU_RLAR = Next->StackRegionStart */
  isb
  mov   r2, #OS_HAL_MP_STACK_REGION_NUMBER_ASM
  str   r2, [r3, #0x8]        /* MPU_RNR = OS_HAL_MP_STACK_REGION_NUMBER_ASM */
  str   r2, [r3, #0x10]       /* MPU_RLAR.EN = 0 */
  ldr   r2, [r1, #4]
  orr   r2, r2, #0x3          /* OS_HAL_MP_ACCESS_EXECUTE_FORBIDDEN | OS_HAL_MP_ACCESS_S_RW_U_RW */
  str   r2, [r3, #0xC]        /* MPU_RBAR = Next->StackRegionStart */
  ldr   r2, [r1, #8]
  add   r2, r2, #0x20
  orr   r2, r2, #1            /* OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_NON_CACHEABLE_OUTER_NON_CACHEABLE | OS_HAL_MP_REGION_SIZE_ENABLE */
  str   r2, [r3, #0x10]       /* MPU_RLAR = Next->StackRegionInfo */
  isb
  mov   r2, #OS_HAL_MP_STACK_PROTECTION_REGION_1_NUMBER_ASM
  str   r2, [r3, #0x8]        /* MPU_RNR = OS_HAL_MP_STACK_PROTECTION_REGION_1_NUMBER_ASM */
  bic   r2, r2, #0x2
  str   r2, [r3, #0x10]       /* MPU_RLAR.EN = 0 */
  ldr   r2, [r1, #8]
  add   r2, r2, #0x40
  orr   r2, r2, #0x6          /* OS_HAL_MP_ACCESS_EXECUTE_FORBIDDEN | OS_HAL_MP_ACCESS_S_R_U_R */
  str   r2, [r3, #0xC]        /* MPU_RBAR = Stack regions end address */
  ldr   r2, =OS_HAL_MP_STACKS_END
  ldr   r2, [r2, #4]
  orr   r2, r2, #1            /* OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_NON_CACHEABLE_OUTER_NON_CACHEABLE | OS_HAL_MP_REGION_SIZE_ENABLE */
  str   r2, [r3, #0x10]       /* MPU_RLAR = Next->StackRegionStart */
  dsb
  isb
# endif /* # if defined(OS_CFG_HAL_MEMORY_PROTECTION_ASM) */
  ldr  r0, [r0]
  ldr  r3, [sp, #0x18]        /* get ReturnAddress from exception frame. */
  ldr  sp, [r0]               /* sp = OsCfg_Hal_Stack_OsCore0_Kernel->StackRegionEnd */
  sub  sp, sp, #0x20
  str  r3, [sp, #0x4]         /* store ReturnAddress of the first exception frame in r1 of the new one. */
  mrs  r3, control
  and  r3, r3, #1             /* r3 = CONTROL.nPRIV bit --- 1(User) 0(Supervisor) */
  sub  r3, r3, #1             /* r3 = r3 - 1 --- 0(User) !0(Supervisor) */
  str  r3, [sp, #0x8]         /* store privilege level in r2 of the new one. */
  Os_Hal_Asm_Load_Register_Immediate r0, Os_UnhandledExc
  str  r0, [sp, #0x18]
  mov  r1, #0x01000000        /* prepare xPSR with only T bit set */
  str  r1, [sp, #0x1C]
  mov  r1, #0
  msr  control, r1
  mrs  r1, ipsr               /* get ISPR */
  movw r2, #0x1FF
  and  r1, r1, r2             /* get the source number */
  str  r1, [sp, #0x0]         /* prepare it in the argument register */
  mvn  lr, #0x47              /* prepare LR(r14) with the value 0xFFFFFFB8, needed by exception return mechanism */
  dsb
  bx   lr
  OS_HAL_ASM_FUNCTION_END(Os_Hal_UnhandledEXC)


/***********************************************************************************************************************
 *  Os_Hal_MemFault()
 **********************************************************************************************************************/
/*! \brief        Low level exception handler for memory faults.
 *  \details
 *  This function switches to kernel stack of the current core (including stack region) and calls given function.
 *  \param[in]    JumpTarget  Function which shall be called.
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
  OS_HAL_ASM_THUMB_FUNCTION
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_MemFault)
  mov r3, #0                  /* Load core Id of 0 */
  bl Os_Hal_Context_SaveExcContext
  Os_Hal_Asm_Load_Register_Immediate r0, OsCfg_Stack_KernelStacks
  ldr   r1, [r0]
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_Mpu_Base_Address_Label
  mov   r2, #OS_HAL_MP_STACK_PROTECTION_REGION_0_NUMBER_ASM
  str   r2, [r3, #0x8]        /* MPU_RNR = OS_HAL_MP_STACK_PROTECTION_REGION_0_NUMBER_ASM */
  bic   r2, r2, #0x1
  str   r2, [r3, #0x10]       /* MPU_RLAR.EN = 0 */
  ldr   r2, =OS_HAL_MP_STACKS_START
  ldr   r2, [r2]
  orr   r2, r2, #0x6          /* OS_HAL_MP_ACCESS_EXECUTE_FORBIDDEN | OS_HAL_MP_ACCESS_S_R_U_R */
  str   r2, [r3, #0xC]        /* MPU_RBAR = Stack regions start address */
  ldr   r2, [r1, #4]          /* Next->StackRegionStart */
  sub   r2, r2, #0x20         /* Align stack protection end address */
  orr   r2, r2, #1            /* OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_NON_CACHEABLE_OUTER_NON_CACHEABLE | OS_HAL_MP_REGION_SIZE_ENABLE */
  str   r2, [r3, #0x10]       /* MPU_RLAR = Next->StackRegionStart */
  isb
  mov   r2, #OS_HAL_MP_STACK_REGION_NUMBER_ASM
  str   r2, [r3, #0x8]        /* MPU_RNR = OS_HAL_MP_STACK_REGION_NUMBER_ASM */
  str   r2, [r3, #0x10]       /* MPU_RLAR.EN = 0 */
  ldr   r2, [r1, #4]
  orr   r2, r2, #0x3          /* OS_HAL_MP_ACCESS_EXECUTE_FORBIDDEN | OS_HAL_MP_ACCESS_S_RW_U_RW */
  str   r2, [r3, #0xC]        /* MPU_RBAR = Next->StackRegionStart */
  ldr   r2, [r1, #8]
  add   r2, r2, #0x20
  orr   r2, r2, #1            /* OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_NON_CACHEABLE_OUTER_NON_CACHEABLE | OS_HAL_MP_REGION_SIZE_ENABLE */
  str   r2, [r3, #0x10]       /* MPU_RLAR = Next->StackRegionInfo */
  isb
  mov   r2, #OS_HAL_MP_STACK_PROTECTION_REGION_1_NUMBER_ASM
  str   r2, [r3, #0x8]        /* MPU_RNR = OS_HAL_MP_STACK_PROTECTION_REGION_1_NUMBER_ASM */
  bic   r2, r2, #0x2
  str   r2, [r3, #0x10]       /* MPU_RLAR.EN = 0 */
  ldr   r2, [r1, #8]
  add   r2, r2, #0x40
  orr   r2, r2, #0x6          /* OS_HAL_MP_ACCESS_EXECUTE_FORBIDDEN | OS_HAL_MP_ACCESS_S_R_U_R */
  str   r2, [r3, #0xC]        /* MPU_RBAR = Stack regions end address */
  ldr   r2, =OS_HAL_MP_STACKS_END
  ldr   r2, [r2, #4]
  orr   r2, r2, #1            /* OS_HAL_MP_ACCESS_TYPE_NORMAL_MEMORY_INNER_NON_CACHEABLE_OUTER_NON_CACHEABLE | OS_HAL_MP_REGION_SIZE_ENABLE */
  str   r2, [r3, #0x10]       /* MPU_RLAR = Next->StackRegionStart */
  dsb
  isb
  ldr  r0, [r0]
  ldr  r2, [sp, #0x18]        /* get ReturnAddress from exception frame. */
  ldr  sp, [r0]               /* sp = OsCfg_Hal_Stack_OsCore0_Kernel->StackRegionEnd */
  sub  sp, sp, #0x20
  Os_Hal_Asm_Load_Register_Immediate r0, Os_MemFault
  str  r0, [sp, #0x18]
  mov  r1, #0x01000000        /* prepare xPSR with only T bit set */
  str  r1, [sp, #0x1C]
  mov  r1, #0
  mrs  r3, control
  and  r3, r3, #1             /* r3 = CONTROL.nPRIV bit --- 1(User) 0(Supervisor) */
  sub  r3, r3, #1             /* r3 = r3 - 1 --- 0(User) !0(Supervisor) */
  str  r3, [sp, #0x4]         /* store privilege level in r1 of the new one. */
  msr  control, r1
  msr  control, r1
  mvn  lr, #0x47              /* LR = 0xFFFFFFB8 to perform next exception return from basic frame. */
  str  r2, [sp, #0]           /* store ReturnAddress at exception entry in the argument register. */
  dsb
  bx   lr
  OS_HAL_ASM_FUNCTION_END(Os_Hal_MemFault)
  OS_HAL_ASM_SECTION_END

#endif /* defined(OS_HAL_CONTEXTASM_V8M_INC) */

/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_ContextAsm_v8m.inc
 *********************************************************************************************************************/
