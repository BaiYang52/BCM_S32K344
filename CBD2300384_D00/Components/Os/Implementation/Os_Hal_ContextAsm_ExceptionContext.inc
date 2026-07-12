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
 *   - ARM Architecture Reference Manual ARMv7-M edition
 *     ARM DDI 0403E.b (ID120114), 2. Dezember 2014
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

#ifndef OS_HAL_EXCEPTION_CONTEXT_INC
# define OS_HAL_EXCEPTION_CONTEXT_INC

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
# include "Os_Hal_Compiler.inc"
# include "Os_Hal_Core_Cfg.h"

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

# define OS_HAL_EXC_CONTEXT_CSFR_REG_ADDRESS_LOW       (0xED28)
# define OS_HAL_EXC_CONTEXT_CSFR_REG_ADDRESS_HIGH      (0xE000)
# define OS_HAL_EXC_CONTEXT_CSFR_REG_BIT4_BIT12_MASK   (0x1010)
# define OS_HAL_EXC_CONTEXT_STACKED_XPSR_REG_BIT9_MASK (0x200) 

/** NXP MPU macros               */
/** ---------------------------- */

/*! Region descriptors start offset */
# define OS_HAL_SMPU_RGD_DESCRIPTOR_OFFSET_ASM         (0x400)

/*! Length of a region descriptor in Byte */
# define OS_HAL_SMPU_RGD_DESCRIPTOR_LENGTH_ASM         (0x10)

/*! Offset of Region Descriptor Word 1 (SMPU_RGDn_WORD1) */
# define OS_HAL_SMPU_RGD_DESCRIPTOR_WORD_1_OFFSET_ASM  (0x04)
  
/**********************************************************************************************************************
 *  ...
 *********************************************************************************************************************/
  OS_HAL_ASM_PRESERVE8

  OS_HAL_ASM_CODE_SECTION(OS_CODE)

  OS_HAL_ASM_THUMB_CODE

OS_HAL_ASM_INIT_LABEL(Os_Hal_Exc_Context_Mpu_Base_Address_Label,OS_HAL_MPU_BASE_ADDRESS_ASM)

/***********************************************************************************************************************
 *  Os_Hal_Context_SaveExcContext()
 **********************************************************************************************************************/
/*! \brief        Save the exception context in a variable of type structure.
 *  \details      Used for exception handlers.
 *  \param[in]    r3 = CoreId
 *  \context      ANY
 *  \reentrant    FALSE
 *  \synchronous  TRUE
 *  \pre          -
 **********************************************************************************************************************/
   OS_HAL_ASM_THUMB_FUNCTION
OS_HAL_ASM_FUNCTION_BEGIN(Os_Hal_Context_SaveExcContext)

  /* Get the address of the destination struct var in r12 */
  /* ---------------------------------------------------- */
  OS_HAL_ASM_ALIGN4
OS_HAL_ASM_DEFINE_LABEL(Get_Destination_Struct_Var_Address) /* r12 : will hold the destination struct var address */
  
  OS_HAL_ASM_IMPORT(OsCfg_Hal_Context_ExceptionContextRef)  /* Declare OsCfg_Hal_Context_ExceptionContextRef defined in Os_Hal_Context_Lcfg.c */ 
  Os_Hal_Asm_Load_Register_Immediate r12, OsCfg_Hal_Context_ExceptionContextRef  /* r12 gets the address of the array OsCfg_Hal_Context_ExceptionContextRef[] */
  
  /* (1) r3 = CoreId << 2 = (4 * CoreId) : r3 gets the offset to &OsCfg_Hal_Context_OsCoreX_ExceptionContext element in OsCfg_Hal_Context_ExceptionContextRef[] array */
  /* (2) r12 += r3 = &OsCfg_Hal_Context_ExceptionContextRef[0] + (4 * CoreId) = &OsCfg_Hal_Context_ExceptionContextRef[CoreId] */
  /* (3) r12 = OsCfg_Hal_Context_ExceptionContextRef[CoreId] = &OsCfg_Hal_Context_OsCoreX_ExceptionContext ; X = CoreId */
  ldr  r12, [r12, r3, lsl #2]

  /* Copy r0, r1, r2, r4-r11 to the destination structure */
  /* ---------------------------------------------------- */
  
  str r0, [r12, #0x04]   /* StructVar.R0 = r0 content */ 
  str r1, [r12, #0x08]   /* StructVar.R1 = r1 content */
  str r2, [r12, #0x0C]   /* StructVar.R2 = r2 content */
  
  add r1, r12, #0x7C  /* Once r1 content is copied, use it to store the structure element start address of the next registers r4-r11 to be copied */
  stm r1, {r4-r11}    /* Copy the content of r4-r11 to destination structure */
  
  /* Check stack frame validity                           */
  /* ---------------------------------------------------- */

  movw r4, #OS_HAL_EXC_CONTEXT_CSFR_REG_ADDRESS_LOW      /* Put the address of CFSR (0xE000ED28) register in r4 using mow, movt */
  movt r4, #OS_HAL_EXC_CONTEXT_CSFR_REG_ADDRESS_HIGH     /* Same as above */
  ldr  r4, [r4]                                          /* Load the content of CFSR register in r4 */ 
  movw r5, #OS_HAL_EXC_CONTEXT_CSFR_REG_BIT4_BIT12_MASK  /* Check CFSR[4] = MMFSR.MSTKERR and CFSR[12] = BFSR.STKERR to check if the pushing of context frame to stack on exception entry failed */
  and  r4, r4, r5                                        /* Same as above */
  cmp  r4, #0                                            /* Same as above */ 
  beq  Valid_Stack_Frame  /* If a HW failure occurs when pushing registers to the stack occured on exception entry, continue with next instruction, else branch to ValidStackFrame */
  
  /* Invalid stack frame                                  */
  /* ---------------------------------------------------- */
  
  /* Note: r0, r1, r2 were already directly copied from registers above */
  
  mov r5, #0            
  str r5, [r12]           /* StructVar.ValidStackFrame = 0 (The stack frame is invalid) */
    
  movw r5, #0xDEAD        /* r5 = 0xDEAD */
  
  str r5, [r12, #0x10]    /* StructVar.R3  = 0xDEAD */
  str r5, [r12, #0x14]    /* StructVar.R12 = 0xDEAD */
  
  movw r5, #0xBADF        /* r5 = 0xBADF */
  
  str r5, [r12, #0x18]    /* StructVar.LR            = 0xBADF */
  str r5, [r12, #0x1C]    /* StructVar.ReturnAddress = 0xBADF */
  str r5, [r12, #0x20]    /* StructVar.XPSR          = 0xBADF */
    
# if defined(OS_CFG_THREAD_FPU_CONTEXT_ENABLED_ASM) 

  mov  r6, #0x24              /* r6 = StructVar.S0 offset. r6 is used to hold the offset of the FP registers in the destination struct var */
  
  OS_HAL_ASM_ALIGN4
OS_HAL_ASM_DEFINE_LABEL(Bad_Frame_FP_Lower_Registers_Loop)

  str r5, [r12, r6]           /* StructVar.S0 .. StructVar.S15 are to be filled with 0xBADF */
  add r6, r6, #4              /* Increment the offset by 4 bytes for the next element in the destination struct var */
  cmp r6, #0x64               /* Check if StructVar.S15 was reached and filled */
  bne Bad_Frame_FP_Lower_Registers_Loop /* Stay in the loop till StructVar.S0 .. StructVar.S15 are filled with 0xBADF in the destination struct var */
  
  str r5, [r12, #0x64]        /* StructVar.FPSR = 0xBADF */
    
# endif

  str SP, [r12, #0x68]        /* Store the current SP in struct.Original_SP. */
    
  b Copy_Special_Purpose_Regs

  /* Valid stack frame                                    */
  /* ---------------------------------------------------- */
  
  /* Note: r0, r1, r2 were already directly copied from registers at the start of this function */

  OS_HAL_ASM_ALIGN4  
OS_HAL_ASM_DEFINE_LABEL(Valid_Stack_Frame)
 
  mov  r5, #1 
  str  r5, [r12]         /* StructVar.ValidStackFrame = 1 (The stack frame is valid) */
    
  ldr r5, [SP, #0x0C]    /* r5 gets the content of the stacked R3 */
  str r5, [r12, #0x10]   /* Store stacked R3 content to structVar.R3 */
  
  ldr r5, [SP, #0x10]     
  str r5, [r12, #0x14]   /* StructVar.R12 = stacked R12 content */
  
  ldr r5, [SP, #0x14]    
  str r5, [r12, #0x18]   /* StructVar.LR = stacked LR content */
  
  ldr r5, [SP, #0x18]    
  str r5, [r12, #0x1C]   /* StructVar.ReturnAddress = stacked ReturnAddress */
  
  ldr r5, [SP, #0x1C]    
  str r5, [r12, #0x20]   /* StructVar.XPSR = stacked xPSR content */

# if defined(OS_CFG_THREAD_FPU_CONTEXT_ENABLED_ASM)

  /* Valid stack frame -> Copy lower FP registers from stack to destination Struct-Var if FP context is supported */
  /* ---------------------------------------------------- */

  mov r6, #0x20  /* r6 gets the offset of stacked S0. r6 is used in the loop to hold the offset of the FP registers in the stack frame and in destination struct var */

  OS_HAL_ASM_ALIGN4
OS_HAL_ASM_DEFINE_LABEL(Valid_Frame_Copy_Lower_FP_Regs_Loop)

  ldr r5, [SP, r6]     /* r5 gets the content of the stacked Sx */
  add r6, r6, #4       /* r6 gets the offset of Sx in the destination struct var */
  str r5, [r12, r6]    /* StructVar.Sx = stacked Sx content */
  cmp r6, #0x60
  
  bne Valid_Frame_Copy_Lower_FP_Regs_Loop /* Stay in the loop till lower FP registers are copied to StructVar.S0 .. StructVar.S15 */
  
  ldr r5, [SP, #0x60]    
  str r5, [r12, #0x64] /* StructVar.FPSR = stacked FPSR content */

# endif
  
  /* Valid stack frame -> Copy Original SP from stack     */
  /* ---------------------------------------------------- */
    
  ldr r5, [SP, #0x1C]  /* r5 gets the content of the stacked XPSR */
   
  mov r6, #0           /* Initialize r6 with 0 (i.e. No 4 bytes alignment are inserted) */
  and r5, r5, #OS_HAL_EXC_CONTEXT_STACKED_XPSR_REG_BIT9_MASK  /* Check stacked XPSR[9] to check if a 4-bytes-alignement-offset is inserted in the stack between the Original SP and the exception stack frame */
  cmp r5, #0                                                  /* Same as above */
  it ne                                                       /* Same as above */
  movne r6, #4         /* 4 bytes alignment were inserted -> save the 4 byte alignment offset in r6 */
  
# if !defined(OS_CFG_THREAD_FPU_CONTEXT_ENABLED_ASM) 
    
  add r5, SP, #0x20    /* r5 gets the address of the end of the basic stack frame */
  
# else
  
  add r5, SP, #0x68    /* r5 gets the address of the end of the extended stack frame */
  
# endif

  add r5, r5, r6        /* r5 gets the address of the Original SP using r6 which holds the offset value between stack frame end and Original SP */
  str r5, [r12, #0x68]  /* Store the Original SP in StructVar.Original_SP */

  /* Copy the content of special purpose registers        */
  /* ---------------------------------------------------- */
  OS_HAL_ASM_ALIGN4
OS_HAL_ASM_DEFINE_LABEL(Copy_Special_Purpose_Regs)
  
  mrs r5, primask
  str r5, [r12, #0x6C]   /* StructVar.PRIMASK = primask */
  
  mrs r5, basepri
  str r5, [r12, #0x70]   /* StructVar.BASEPRI = basepri */
  
  mrs r5, faultmask
  str r5, [r12, #0x74]   /* StructVar.FAULTMASK = faultmask */
  
  mrs r5, control
  str r5, [r12, #0x78]   /* StructVar.CONTROL = control */
  
# if defined(OS_CFG_THREAD_FPU_CONTEXT_ENABLED_ASM) 
  
  /* Copy FPU upper registers content to destination Struct-Var if FP context is supported */
  /* ------------------------------------------------------ */
    
  add r5, r12, #0x9C
  vstm r5, {s16-s31}

# endif
  
# if !defined(OS_CFG_THREAD_FPU_CONTEXT_ENABLED_ASM)

  /* FPU Cotext not supported -> write DEAD in FP registers */
  /* ------------------------------------------------------ */

  movw r5, #0xDEAD
  
  mov  r6, #0x24              /* r6 = StructVar.S0 offset - r6 is used to hold the offset of the FP registers in the destination struct var */
  
  OS_HAL_ASM_ALIGN4
OS_HAL_ASM_DEFINE_LABEL(Dead_FP_Lower_Registers_Loop)
  
  str r5, [r12, r6]           /* StructVar.S0 .. StructVar.S15 are to be filled with 0xDEAD */
  add r6, r6, #4              /* Increment the offset by 4 bytes for the next element in the destination struct var */
  cmp r6, #0x64               /* Check if StructVar.S15 was reached and filled */
  bne Dead_FP_Lower_Registers_Loop /* Stay in the loop till StructVar.S0 .. StructVar.S15 are filled with 0xDEAD in the destination struct var */

  str r5, [r12, #0x64]        /* Write 0xDEAD to StructVar.FPSR register if FP context is not supported */
  
  mov  r6, #0x9C              /* r6 = StructVar.S16 offset - r6 is used to hold the offset of the FP registers in the destination struct var */
  
  OS_HAL_ASM_ALIGN4
OS_HAL_ASM_DEFINE_LABEL(Dead_FP_Upper_Registers_Loop)
  
  str r5, [r12, r6]           /* StructVar.S16 .. StructVar.S31 are to be filled with 0xDEAD */
  add r6, r6, #4              /* Increment the offset by 4 bytes for the next element in the destination struct var */
  cmp r6, #0xDC               /* Check if StructVar.S31 was reached and filled */
  bne Dead_FP_Upper_Registers_Loop /* Stay in the loop till StructVar.S0 .. StructVar.S15 are filled with 0xDEAD in the destination struct var */
  
# endif

# if defined(OS_CFG_HAL_MEMORY_PROTECTION_ASM)

  /* Copy MPU info if MPU is supported                      */
  /* ------------------------------------------------------ */
  
#  if defined(OS_HAL_MEMORY_PROTECTION_PMSAV7M) || defined(OS_HAL_MEMORY_PROTECTION_PMSAV8M)

  mov r6, #OS_HAL_MP_STACK_REGION_NUMBER_ASM             /* r6 gets the MPU region number needed to select the MPU Region for reading RBAR, RASR */

  Os_Hal_Asm_Load_Register_Label r7, Os_Hal_Exc_Context_Mpu_Base_Address_Label /* r7 = OS_HAL_MPU_BASE_ADDRESS_ASM = 0xE000ED90 */
  
  str  r6, [r7, #0x8]                                    /* Write STACK_REGION_NUMBER to RNR to select the MPU Region for reading RBAR, RASR */
  
  ldr  r5, [r7, #0xC]                                    /* r5 gets the MPU_RBAR register content */
  str  r5, [r12, #0xDC]                                  /* Store RBAR register content in StructVar.StackRegionStart */
  
  ldr  r5, [r7, #0x10]                                   /* r5 gets the MPU_RASR register content */
  str  r5, [r12, #0xE0]                                  /* Store RBAR register content in StructVar.StackRegionInfo */

#  elif defined(OS_HAL_MEMORY_PROTECTION_NXP)

  /*  StackRegionStart register address                                                                                                                    */
  /*  = OS_HAL_MPU_BASE_ADDRESS_ASM + OS_HAL_SMPU_RGD_DESCRIPTOR_OFFSET_ASM + (OS_HAL_MP_STACK_REGION_NUMBER_ASM * OS_HAL_SMPU_RGD_DESCRIPTOR_LENGTH_ASM)  */

  mov r6, #OS_HAL_SMPU_RGD_DESCRIPTOR_LENGTH_ASM                                /* r6 = OS_HAL_SMPU_RGD_DESCRIPTOR_LENGTH_ASM */
  mov r5, #OS_HAL_MP_STACK_REGION_NUMBER_ASM                                    /* r5 = OS_HAL_MP_STACK_REGION_NUMBER_ASM */
  mul r5, r5, r6                                                                /* r5 = (OS_HAL_MP_STACK_REGION_NUMBER_ASM * OS_HAL_SMPU_RGD_DESCRIPTOR_LENGTH_ASM) */
  add r5, r5, #OS_HAL_SMPU_RGD_DESCRIPTOR_OFFSET_ASM                            /* r5 = OS_HAL_SMPU_RGD_DESCRIPTOR_OFFSET_ASM + (OS_HAL_MP_STACK_REGION_NUMBER_ASM * OS_HAL_SMPU_RGD_DESCRIPTOR_LENGTH_ASM) */
  Os_Hal_Asm_Load_Register_Label r6, Os_Hal_Exc_Context_Mpu_Base_Address_Label  /* r6 = OS_HAL_MPU_BASE_ADDRESS_ASM */
  add r5, r5, r6                                                                /* r5 = StackRegionStart register address */
  ldr r6, [r5]                                                                  /* r6 gets the current thread stack region start address from MPU */  
  str r6, [r12, #0xDC]                                                          /* Store thread stack region start address in StructVar.StackRegionStartInfo */
  
  /*  StackRegionEnd register address                                                      */
  /*  = StackRegionStart register address + OS_HAL_SMPU_RGD_DESCRIPTOR_WORD_1_OFFSET_ASM   */
  
  add r5, r5, #OS_HAL_SMPU_RGD_DESCRIPTOR_WORD_1_OFFSET_ASM                     /* r5 = StackRegionEnd register address = r5 + OS_HAL_SMPU_RGD_DESCRIPTOR_WORD_1_OFFSET_ASM */
  ldr r5, [r5]                                                                  /* r5 gets the current thread stack region end address from MPU */    
  str r5, [r12, #0xE0]                                                          /* Store thread stack region end address in StructVar.StackRegionEndInfo */
  
#  endif /* # if defined(OS_HAL_MEMORY_PROTECTION_PMSAV7M) */

# else   /* MPU is not configured/supported -> Write 0xDEAD to StackMpuRegionStart/EndInfo */

  movw r5, #0xDEAD       /* r5 = 0xDEAD */
  str  r5, [r12, #0xDC]  /* StructVar.StackRegionStartInfo = 0xDEAD */ 
  str  r5, [r12, #0xE0]  /* StructVar.StackRegionEndInfo   = 0xDEAD */
  
# endif  /* #  if defined(OS_CFG_HAL_MEMORY_PROTECTION_ASM) */

  bx  lr
  
  OS_HAL_ASM_ALIGN4
  OS_HAL_ASM_FUNCTION_END(Os_Hal_Context_SaveExcContext)
  
  OS_HAL_ASM_SECTION_END
  
#endif /* defined(OS_HAL_EXCEPTION_CONTEXT_INC) */

/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_ContextAsm_ExceptionContext.inc
 *********************************************************************************************************************/
