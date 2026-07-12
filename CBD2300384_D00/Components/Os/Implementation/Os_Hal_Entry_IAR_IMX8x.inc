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
 * \file  Os_Hal_Entry_IAR_IMX8x.inc
 * \brief  This component handles hardware exceptions and interrupts.
 * \internal
 *  Hardware manuals: See derivative specific header (Os_Hal_Derivative_IMX8xInt.h)
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

#ifndef OS_HAL_ENTRY_IMX8X_INC
# define OS_HAL_ENTRY_IMX8X_INC


OS_HAL_ASM_IMPORT(OsCfg_Hal_Core_PhysicalCore0_SystemInterruptVectorTable)
OS_HAL_ASM_IMPORT(OsCfg_Hal_Core_PhysicalCore1_SystemInterruptVectorTable)
OS_HAL_ASM_EXPORT(Os_Hal_UnhandledIRQ_SystemInterrupt)

/***********************************************************************************************************************
 *  Os_Hal_SpuriousIRQ_Handler()
 **********************************************************************************************************************/
/*! \brief        Handler for spurious interrupts.
 *  \details      Restores the context saved by hardware at exception entry.
 *
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
  OS_HAL_ASM_THUMB_FUNCTION
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_SpuriousIRQ_Handler)
  add  sp, sp, #0x20      ; undo frame copy space reservation
  pop  {r1-r2, r4, lr}    ; restore modified registers values
  msr  basepri, r2        ; restore the interrupt level before interrupt
  bx   lr                 ; return from exception to unstack the original exception frame
  OS_HAL_ASM_FUNCTION_END(Os_Hal_SpuriousIRQ_Handler)

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
  bl   Os_UnhandledIrq                 ; call the main ISR wrapper
  pop  {r1-r11, lr}                    ; restore the interrupt level
  mov  r0, sp                          ; prepare the exception frame pointer for RFE
  msr  basepri, r2                     ; restore the interrupt level before interrupt
  OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME   ; use SvCall to unstack the original exception frame
  OS_HAL_ASM_DUMMY_BRANCH
  OS_HAL_ASM_LTORG                     ; ESCAN00101862 fix
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
  bl   OS_HAL_ASM_MACRO_PARAM_IN_BODY(JumpTarget) ; call the main ISR wrapper
  pop  {r1-r11, lr}                               ; restore the interrupt level
  mov  r0, sp                                     ; prepare the exception frame pointer for RFE
  msr  basepri, r2                                ; restore the interrupt level before interrupt
  OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME              ; use SvCall to unstack the original exception frame
  OS_HAL_ASM_DUMMY_BRANCH
  OS_HAL_ASM_LTORG                                ; ESCAN00101862 fix
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
  bl   Os_IsrRun                       ; call the main ISR wrapper
  pop  {r1-r11, lr}                    ; restore the interrupt level
  mov  r0, sp                          ; prepare the exception frame pointer for RFE
  msr  basepri, r2                     ; restore the interrupt level before interrupt
  OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME   ; use SvCall to unstack the original exception frame
  OS_HAL_ASM_DUMMY_BRANCH
  OS_HAL_ASM_LTORG                     ; ESCAN00101862 fix
  ENDM

OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_15_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x3C))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_14_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x38))
/***********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_32()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Current implementation uses the greater of the two values, could lead to starvation. Room for
*                improvement. Implementation of some form of fair scheduling ?
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_32 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0x0                      ; Addition constant for Interrupt calculation for Register 15
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_15_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0x20               ; Addition constant for Interrupt calculation for Register 14
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_14_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1         ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore0_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_13_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x34))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_12_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x30))
/***********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_33()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_33 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0x40                      ; Addition constant for Interrupt calculation for Register 13
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_13_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0x60               ; Addition constant for Interrupt calculation for Register 12
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_12_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1         ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore0_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_11_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x2C))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_10_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x28))
/***********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_34()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_34 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0x80                      ; Addition constant for Interrupt calculation for Register 11
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_11_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0xA0               ; Addition constant for Interrupt calculation for Register 10
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_10_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1         ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore0_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_9_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x24))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_8_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x20))
/***********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_35()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_35 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0xC0                      ; Addition constant for Interrupt calculation for Register 9
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_9_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0xE0               ; Addition constant for Interrupt calculation for Register 8
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_8_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1         ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore0_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_7_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x1C))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_6_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x18))
/***********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_36()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_36 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0x100                      ; Addition constant for Interrupt calculation for Register 7
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_7_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0x120               ; Addition constant for Interrupt calculation for Register 6
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_6_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1         ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore0_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM



OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_5_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x14))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_4_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x10))
/***********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_37()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_37 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0x140                      ; Addition constant for Interrupt calculation for Register 5
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_5_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0x160               ; Addition constant for Interrupt calculation for Register 4
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_4_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1         ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore0_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_3_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x0C))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_2_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x08))
/**********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_38()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_38 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0x180                      ; Addition constant for Interrupt calculation for Register 3
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_3_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0x1A0               ; Addition constant for Interrupt calculation for Register 2
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_2_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1         ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore0_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_1_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x04))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_0_Steer_Status_Register_0_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x00))
/***********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_39()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_0_ExternalInterrupt_39 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0x1C0                      ; Addition constant for Interrupt calculation for Register 1
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_1_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0x1E0               ; Addition constant for Interrupt calculation for Register 0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_0_Steer_Status_Register_0_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1        ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore0_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_15_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x1003C))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_14_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x10038))
/***********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_32()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Current implementation uses the greater of the two values, could lead to starvation. Room for
*                improvement. Implementation of some form of fair scheduling ?
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_32 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0x0                      ; Addition constant for Interrupt calculation for Register 15
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_15_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0x20               ; Addition constant for Interrupt calculation for Register 14
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_14_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1         ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore1_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_13_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x10034))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_12_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x10030))
/***********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_33()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_33 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0x40                      ; Addition constant for Interrupt calculation for Register 13
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_13_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0x60               ; Addition constant for Interrupt calculation for Register 12
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_12_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1         ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore1_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_11_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x1002C))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_10_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x10028))
/***********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_34()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_34 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0x80                      ; Addition constant for Interrupt calculation for Register 11
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_11_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0xA0               ; Addition constant for Interrupt calculation for Register 10
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_10_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1         ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore1_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_9_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x10024))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_8_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x10020))
/***********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_35()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_35 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0xC0                      ; Addition constant for Interrupt calculation for Register 9
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_9_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0xE0               ; Addition constant for Interrupt calculation for Register 8
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_8_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1         ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore1_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_7_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x1001C))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_6_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x10018))
/***********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_36()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_36 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0x100                      ; Addition constant for Interrupt calculation for Register 7
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_7_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0x120               ; Addition constant for Interrupt calculation for Register 6
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_6_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1         ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore1_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM



OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_5_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x10014))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_4_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x10010))
/***********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_37()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_37 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0x140                      ; Addition constant for Interrupt calculation for Register 5
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_5_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0x160               ; Addition constant for Interrupt calculation for Register 4
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_4_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1         ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore1_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_3_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x1000C))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_2_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x10008))
/**********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_38()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_38 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0x180                      ; Addition constant for Interrupt calculation for Register 3
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_3_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0x1A0               ; Addition constant for Interrupt calculation for Register 2
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_2_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1         ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore1_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM


OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_1_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x10004))
OS_HAL_ASM_INIT_LABEL(Os_Hal_CM4_1_Steer_Status_Register_0_Label, (OS_HAL_STEER_CHN_STATUS_BASE + 0x10000))
/***********************************************************************************************************************
*  Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_39()
**********************************************************************************************************************/
/*! \brief       First level ISR handler which saves minimum of interrupted context and calls the correct system level
*                handler.
*  \details      In case of IMX8x interrupt controller, the system interrupt source has to be read from two status
*                registers per each external interrupt source. The registers are read and the greater of the two
*                interrupt sources is used to execute the correct system interrupt handler.
*
*                Formula used for Interrupt calculation
*                interruptTriggered = ((31uL - (Os_Hal_CountLeadingZeros(OS_HAL_INTC_STEER->chn_status[N]))) + (M*32));
*
*  \context      ANY
*
*  \reentrant    FALSE
*  \synchronous  TRUE
*
*  \pre          -
**********************************************************************************************************************/
Os_Hal_Entry_Interrupt_IMX8x_CM4_1_ExternalInterrupt_39 MACRO
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, r4, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20                             ; reserve the space for the frame copy
  mov  r1, #0x1000000                            ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9               ; prepare LR(r14) with the value 0xFFFFFFF9, needed by exception return mechanism

  mov r1 , #0x1C0                      ; Addition constant for Interrupt calculation for Register 1
  mov r2 , #0x1F                      ; Substraction constant for Interrupt Calculation
  mov r4 , #0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_1_Label ; Load STEER status register address for the first 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Calculate Interrupt value if there is an interrupt bit set in the status register
  subne r4, r2, r3
  addne r4, r4, r1

  mov r1 , #0x1E0               ; Addition constant for Interrupt calculation for Register 0
  Os_Hal_Asm_Load_Register_Label r3, Os_Hal_CM4_1_Steer_Status_Register_0_Label ; Load STEER status register address for the second 32 interrupts
  ldr r3, [r3, #0]
  clz r3,r3
  cmp r3, 0x20
  itt ne
  ; Overwrite first value if there is an interrupt bit set in the second status register
  subne r4, r2, r3
  addne r4, r4, r1

  cmp r4, 0x0              ; r4 = 0 is only possible if both 32 bit status registers were 0 (spurious interrupt)
  it  eq
  beq Os_Hal_SpuriousIRQ_Handler

  ldr  r12, [sp, #0x38]    ; get ReturnAddress of the first exception frame
  str  r12, [sp, #4]       ; store ReturnAddress of the first exception frame in r1 of the new one
  ldr  r12, [sp,#0x28]     ; load CONTROL.nPRIV bit at exception entry
  sub  r12, r12, #1        ; r3 = r3 - 1 --- 0(User) !0(Supervisor)
  str  r12, [sp, #0x8]     ; store privilege level in r2 of the new one
  str  r4, [sp, #0x0]      ; store the external interrupt id in r0 of the new exception frame

  add  r1, pc, #0x8        ; Dependend on the placement of this instruction
  ; the result will be address of the "nop"
  ; or the "bl" instruction
  str  r1, [sp, #0x18]                           ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here

  ldr r4, =OsCfg_Hal_Core_PhysicalCore1_SystemInterruptVectorTable
  add r4, r4, r0, lsl#2
  ldr r4, [r4,#0x0]

  bx  r4        ; Jump to the SystemInterruptVectorTable Entry
  ENDM

#endif /* defined(OS_HAL_ENTRY_IMX8X_INC) */

/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Entry_IAR_IMX8x.inc
 *********************************************************************************************************************/
