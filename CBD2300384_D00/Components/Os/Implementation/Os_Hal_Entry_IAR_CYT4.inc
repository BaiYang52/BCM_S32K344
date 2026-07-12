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
 * \file  Os_Hal_Entry_IAR_CYT4.inc
 * \brief  This component handles hardware exceptions and interrupts.
 * \internal
 *  Hardware manuals: See derivative specific header (Os_Hal_Derivative_CYT4Int.h)
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

#ifndef OS_HAL_ENTRY_CYT4_INC
# define OS_HAL_ENTRY_CYT4_INC

  OS_HAL_ASM_CODE_SECTION(OS_CODE)
  OS_HAL_ASM_IMPORT(OsCfg_Hal_Core_PhysicalCore0_SystemInterruptVectorTable)
  OS_HAL_ASM_IMPORT(OsCfg_Hal_Core_PhysicalCore1_SystemInterruptVectorTable)
  OS_HAL_ASM_IMPORT(Os_UnhandledIrq)
  OS_HAL_ASM_EXPORT(Os_Hal_UnhandledIRQ_SystemInterrupt)

/***********************************************************************************************************************
 *  Os_Hal_UnhandledIRQ_SystemInterrupt()
 **********************************************************************************************************************/
/*! \brief        Second level UnhandledIRQ handler.
 *  \details      Branches to Os_HandledIrq with function parameters saved by the first level interrupt controller
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
  bl   Os_UnhandledIrq                 ; call the main ISR wrapper
  pop  {r1-r2, r4, lr}                 ; restore the interrupt level
  mov  r0, sp                          ; prepare the exception frame pointer for RFE
  msr  basepri, r2                     ; restore the interrupt level before interrupt
  OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME   ; use SvCall to unstack the original exception frame
  b    .
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
Os_Hal_Entry_SystemInterrupt MACRO JumpTarget
  bl   OS_HAL_ASM_MACRO_PARAM_IN_BODY(JumpTarget) ; call the user ISR
  pop  {r1-r2, r4, lr}                            ; restore the interrupt level
  mov  r0, sp                                     ; prepare the exception frame pointer for RFE
  msr  basepri, r2                                ; restore the interrupt level before interrupt
  OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME              ; use SvCall to unstack the original exception frame
  b    .
  ENDM

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
Os_Hal_Cat2Entry_SystemInterrupt MACRO IsrConfig
  Os_Hal_Asm_Load_Register_Immediate  r0, OS_HAL_ASM_MACRO_PARAM_IN_BODY(IsrConfig)
  bl  Os_IsrRun                           ; call the system ISR handler
  pop  {r1-r2, r4, lr}                    ; restore the interrupt level
  mov  r0, sp                             ; prepare the exception frame pointer for RFE
  msr  basepri, r2                        ; restore the interrupt level before interrupt
  OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME      ; use SvCall to unstack the original exception frame
  b    .
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_Cm7_0_Int_Status_Address_Label,OS_HAL_CPUSS_CM7_0_INT_STATUS_ASM)
/***********************************************************************************************************************
 *  Os_Hal_Entry_Interrupt_CYT4_CM7_0()
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

Os_Hal_Entry_Interrupt_CYT4_CM7_0 MACRO InternalInterruptId
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3        ; disable all interrupts
  mrs  r1, control
  ldr  r12, [sp, #0x18]   ; get ReturnAddress from exception frame
  push {r1-r2,r4,lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20      ; reserve the space for the frame copy
  mov  r3, #0x1000000     ; prepare xPSR with only T bit set
  str  r3, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9    ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  ldr  r0, =OS_HAL_ASM_MACRO_PARAM_IN_BODY(InternalInterruptId)
  Os_Hal_Asm_Load_Register_Label r1, Os_Hal_Cm7_0_Int_Status_Address_Label
  ldr r2, [r1, r0, lsl #2]   ; prepare the address of CM7_0_INT_STATUS[InternalInterruptId] in r2
  clz  r3, r2                ; Read the negated bit value of CM7_0_INT_STATUS.SYSTEM_INT_VALID[31] in r3
  str  r3, [sp, #0xC]        ; store the CM7_0_INT_STATUS.SYSTEM_INT_VALID[31] in r3 of the new exception frame
  movw r3, #0x3FF            ; prepare a bit mask with bits [9:0] set to 1 in r3
  and r2,r2,r3               ; Read the value of CM7_0_NT_STATUS.SYSTEM_INT_IDX[9:0] in r2 (ExternalInterruptSource)

  str  r12, [sp, #0x4]    ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp, #0x20]   ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1       ; r12 = r12 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]    ; store privilege level in r2 of the new one
  str  r2, [sp, #0x0]     ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8       ; Dependend on the placement of the previous instruction
                          ; the result will be address of the "nop"
                          ; or the "ldr" instruction
  str  r1, [sp, #0x18]    ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore0_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]
  cmp   r3, #0x0          ; check interrupt id valid bit.
  it    eq

  bxeq  r4                            ; Jump to the SystemInterruptVectorTable Entry, if it is a valid interrupt id
  pop  {r1-r2, r4, lr}                ; restore the interrupt level
  mov  r0, sp                         ; prepare the exception frame pointer for RFE
  msr  basepri, r2                    ; restore the interrupt level before interrupt
  OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME  ; use SvCall to unstack the original exception frame
  b    .
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_Cm7_1_Int_Status_Address_Label,OS_HAL_CPUSS_CM7_1_INT_STATUS_ASM)
/***********************************************************************************************************************
 *  Os_Hal_Entry_Interrupt_CYT4_CM7_1()
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

Os_Hal_Entry_Interrupt_CYT4_CM7_1 MACRO InternalInterruptId
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3        ; disable all interrupts
  mrs  r1, control
  ldr  r12, [sp, #0x18]   ; get ReturnAddress from exception frame
  push {r1-r2,r4,lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20      ; reserve the space for the frame copy
  mov  r3, #0x1000000     ; prepare xPSR with only T bit set
  str  r3, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9    ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  ldr  r0, =OS_HAL_ASM_MACRO_PARAM_IN_BODY(InternalInterruptId)
  Os_Hal_Asm_Load_Register_Label r1, Os_Hal_Cm7_1_Int_Status_Address_Label
  ldr r2, [r1, r0, lsl #2]   ; prepare the address of CM7_1_INT_STATUS[InternalInterruptId] in r2
  clz  r3, r2                ; Read the negated bit value of CM7_1_INT_STATUS.SYSTEM_INT_VALID[31] in r3
  str  r3, [sp, #0xC]        ; store the CM7_1_INT_STATUS.SYSTEM_INT_VALID[31] in r3 of the new exception frame
  movw r3, #0x3FF            ; prepare a bit mask with bits [9:0] set to 1 in r3
  and r2,r2,r3               ; Read the value of CM7_1_NT_STATUS.SYSTEM_INT_IDX[9:0] in r2 (ExternalInterruptSource)

  str  r12, [sp, #0x4]    ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp, #0x20]   ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1       ; r12 = r12 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]    ; store privilege level in r2 of the new one
  str  r2, [sp, #0x0]     ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8       ; Dependend on the placement of the previous instruction
                          ; the result will be address of the "nop"
                          ; or the "ldr" instruction
  str  r1, [sp, #0x18]    ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore1_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]
  cmp   r3, #0x0          ; check interrupt id valid bit.
  it    eq

  bxeq  r4                              ; Jump to the SystemInterruptVectorTable Entry, if it is a valid interrupt id
  pop  {r1-r2, r4, lr}                  ; restore the interrupt level
  mov  r0, sp                           ; prepare the exception frame pointer for RFE
  msr  basepri, r2                      ; restore the interrupt level before interrupt
  OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME    ; use SvCall to unstack the original exception frame
  b    .
  ENDM   

#endif /* defined(OS_HAL_ENTRY_CYT4_INC) */

/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Entry_IAR_CYT4.inc
 *********************************************************************************************************************/
