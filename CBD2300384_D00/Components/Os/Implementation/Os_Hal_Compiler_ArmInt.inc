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
 *  \addtogroup Os_Hal_Compiler
 *  \{
 *
 *  \file       Os_Hal_Compiler_ArmInt.inc
 *  \brief      Additional ARM assembler compiler abstraction.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_COMPILERARMINT_INC
# define OS_HAL_COMPILERARMINT_INC

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/*! \brief    Directive used for assembler parameter definition inside the function declaration.
 */
# define OS_HAL_ASM_MACRO_PARAM_IN_HEADER(Param)               $Param

/*! \brief    Directive used for assembler parameter definition inside the function body.
 */
# define OS_HAL_ASM_MACRO_PARAM_IN_BODY(Param)                 $Param

/*! \brief    Directive used to mark beginning of a macro without Param
 */
# define OS_HAL_ASM_MACRO_BEGIN_DEFINE(MacroName)            MACRO MacroName

/*! \brief    Directive used to mark the beginning of a macro
 */
# define OS_HAL_ASM_MACRO_BEGIN_1(MacroName, Params)

/*! \brief    Directive used to mark the beginning of a macro
 */
# define OS_HAL_ASM_MACRO_BEGIN_2(MacroName, Params) MACRO

/*! \brief    Directive used to mark the beginning of a macro
 */
# define OS_HAL_ASM_MACRO_BEGIN_3(MacroName, Params) MacroName Params

/*! \brief    Directive used to mark the end of a macro
 */
# define OS_HAL_ASM_MACRO_END                        MEND

/*! \brief    Causes the identifier Label to be visible externally.
 *  \details  If the identifier is defined in the current module, this directive allows the linker to resolve
 *            references by other modules. If the identifier is not defined in the current module,
 *            the assembler resolves it externally.
 */
# define OS_HAL_ASM_EXPORT(Label)                    EXPORT Label

/*! \brief     You usually use this directive when you want to explicitly declare Label as defined in another module.
 *  \details   The assembler does not require this usage, however.
 */
# define OS_HAL_ASM_IMPORT(Label)                    IMPORT Label

/*! \brief     Directs assembly output into the section named SectionName, which should have memory allocated for it.
 *             and contains executable code.
 */
# define OS_HAL_ASM_CODE_SECTION(SectionName)        AREA SectionName, CODE, READONLY, ALIGN=3

/*! \brief     Directs assembly output into the section named SectionName, which should have memory allocated for it.
 */
# define OS_HAL_ASM_CONST_SECTION(SectionName)       AREA SectionName, DATA, READONLY

/*! \brief     Directive used to mark the end of a section in assembler code.
 */
# define OS_HAL_ASM_SECTION_END


/*! \brief     Initialize a label with name Name and value Value.
 */
# define OS_HAL_ASM_INIT_LABEL(Name,Value)           Name equ Value

/*! \brief     Definition of a label.
 */
# define OS_HAL_ASM_DEFINE_LABEL(Label)              Label

/*! \brief    Switches to mixed 16-bit and 32-bit Thumb-2 code
 */
# define OS_HAL_ASM_THUMB_CODE                       THUMB

/*! \brief    Directive used to mark the beginning of an assembler function.
 */
# define OS_HAL_ASM_FUNCTION_BEGIN(FunctionName)     FunctionName FUNCTION

/*! \brief    Directive used to mark the end of an assembler function.
 */
# define OS_HAL_ASM_FUNCTION_END(FunctionName)       ENDFUNC

/*! \brief    Directive used to mark an assembler function as a thumb function.
 */
# define OS_HAL_ASM_THUMB_FUNCTION

/*! \brief    Directive used to mark the end of an assembler file.
 */
# define OS_HAL_ASM_MODULE_END                       END

/*! \brief    Macro used to load a register with an immediate value
 */
  MACRO
  Os_Hal_Asm_Load_Register_Immediate $Register, $Immediate
  ldr $Register, =$Immediate
  MEND

/*! \brief    Macro used to load a register with a label value
 */
  MACRO
  Os_Hal_Asm_Load_Register_Label $Register, $Label
  ldr $Register, =$Label
  MEND

/*! \brief    Directive used to specify that the current file preserves 8-bytes alignment of the stack.
 */
# define OS_HAL_ASM_PRESERVE8                        PRESERVE8

/*! \brief    Directive used to specify that the current file requires 8-bytes alignment of the stack.
 */
# define OS_HAL_ASM_REQUIRE8                         REQUIRE8

/*! \brief    Directive used to specify the alignment of 4.
 */
# define OS_HAL_ASM_ALIGN4                           ALIGN 4

/*! \brief    Dummy Unconditional branch.
    \details  The unconditional branch is used before ltorg instruction to solve A1471W: 
              Directive ltorg may be in an executable position
 */
# define OS_HAL_ASM_DUMMY_BRANCH                     b .

/*! \brief    Directive used to place an additional literal pool in the code.
    \details  Alignment is required to solve A1581W: Added 2 bytes padding after the ltorg instruction */
# define OS_HAL_ASM_LTORG                            LTORG

/*! \brief    Directive used to restore a given exception frame from stack to the core registers.
    \details  R0 must contain the pointer to the exception frame that has to be restored. */
# define OS_HAL_ASM_UNSTACK_EXCEPTION_FRAME          svc #2

/*! \brief    Directive used for no operation (NOP).
 */
# define OS_HAL_ASM_DUMMY_NOP  NOP

#endif /* OS_HAL_COMPILERARMINT_INC */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_CompilerArmInt.inc
 **********************************************************************************************************************/

