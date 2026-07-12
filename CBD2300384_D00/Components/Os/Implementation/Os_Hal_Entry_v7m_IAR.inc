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
 * \file  Os_Hal_Entry_v7m_IAR.inc
 * \brief  This component handles hardware exceptions and interrupts for ARMv7-M.
 * \internal
 *  Hardware manuals:
 *  - ARMv7-M Architecture Reference Manual
 *    ARM DDI 0403E.b (ID120114), 2. December 2014
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

#ifndef OS_HAL_ENTRY_V7M_IAR_INC
# define OS_HAL_ENTRY_V7M_IAR_INC


/***********************************************************************************************************************
 *  Os_Hal_Cat2Entry_Interrupt()
 **********************************************************************************************************************/
/*! \brief        Low level ISR handler which saves minimum of interrupted context to be able to call high level ISR
 *                handler.
 *  \details
 *  \param[in]    IsrConfig  ISR configuration which shall be executed by high level ISR handler.
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
Os_Hal_Cat2Entry_Interrupt MACRO IsrConfig
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3        ; disable all interrupts
  mrs  r1, control
  push {r1-r2, lr}
  mov  r2, #0
  msr  control, r2

  sub  sp, sp, #0x20      ; reserve the space for the frame copy
  mov  r1, #0x1000000     ; prepare xPSR with only T bit set
  str  r1, [sp, #0x1C]
  mov  lr, #0xFFFFFFF9
  add  r1, pc, #0x8       ; Dependend on the placement of this instruction
                          ; the result will be address of the "nop"
                          ; or the "ldr" instruction
  str  r1, [sp, #0x18]    ; replace the PC in the exceptions frame
  dsb
  bx   lr

  ; continue in thread mode here
  nop
  ; or here
  ldr  r0, =OS_HAL_ASM_MACRO_PARAM_IN_BODY(IsrConfig)
  bl   Os_IsrRun                          ; call the main ISR wrapper
  pop  {r1-r2, lr}                        ; restore the interrupt level
  mov  r0, sp                             ; prepare the exception frame pointer for RFE
  msr  basepri, r2                        ; restore the interrupt level before interrupt
  OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME      ; use SvCall to unstack the original exception frame
  OS_HAL_ASM_DUMMY_BRANCH
  OS_HAL_ASM_LTORG                        ; ESCAN00101862 fix
  ENDM


/***********************************************************************************************************************
 *  Os_Hal_Entry_Interrupt()
 **********************************************************************************************************************/
/*! \brief        Branches to given JumpTarget.
 *  \details      Used for Cat1 and Cat0 Interrupts.
 *  \param[in]    JumpTarget  Address of function which shall be executed.
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
Os_Hal_Entry_Interrupt MACRO JumpTarget
  mrs  r2, basepri
  mov  r3, #OS_CFG_HAL_GLOBAL_DISABLE_LEVEL
  msr  basepri, r3                               ; disable all interrupts
  mrs  r1, control
  push {r1-r2, lr}
  bl   OS_HAL_ASM_MACRO_PARAM_IN_BODY(JumpTarget); call the main ISR wrapper
  pop  {r1-r2, lr}                               ; restore the interrupt level
  msr  basepri, r2                               ; restore the interrupt level before interrupt
  msr  control, r1
  bx   lr
  OS_HAL_ASM_LTORG                               ; ESCAN00101862 fix
  ENDM

/***********************************************************************************************************************
 *  Os_Hal_Entry_Exception()
 **********************************************************************************************************************/
/*! \brief        Branches to given JumpTarget.
 *  \details      Used for exception handlers. No additional wrapper implemented for this case.
 *  \param[in]    JumpTarget  Address of function which shall be executed.
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
Os_Hal_Entry_Exception MACRO JumpTarget
  b   OS_HAL_ASM_MACRO_PARAM_IN_BODY(JumpTarget); call the main Exception wrapper
  OS_HAL_ASM_LTORG                               ; ESCAN00101862 fix
  ENDM

#endif /* defined(OS_HAL_ENTRY_V7M_IAR_INC) */

/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Entry_v7m_IAR.inc
 *********************************************************************************************************************/
