/***********************************************************************************************************************
 *  COPYRIGHT
 *  --------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2021 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  --------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  ------------------------------------------------------------------------------------------------------------------*/
/**
 * \ingroup      Os_Hal_Compiler
 * \{
 *
 * \file         Os_Hal_Compiler_Microsoft.h
 * \brief        Compiler abstraction file for Microsoft compiler.
 *
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388  EOF */ /* MD_MSR_Dir1.1 */
#ifndef OS_HAL_COMPILER_MICROSOFT_H
# define OS_HAL_COMPILER_MICROSOFT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */
# include "Os_Hal_Compiler_Microsoft_Types.h"

/* Os kernel module dependencies */

/* Os HAL dependencies */


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/*! \brief    Force the compiler to inline a function.
 *  \details  Not available for Microsoft compiler.
 */
# define OS_ALWAYS_INLINE


/*! \brief    This attribute tells the compiler that a function never returns.
 *  \details
 *  It can then optimize without regard to what would happen if the function ever did return.
 *  This makes slightly better code. More importantly, it helps avoid spurious warnings of uninitialized variables.
 */
# define OS_NORETURN              __declspec(noreturn)


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/
/*! \brief      Declares a function with a compiler specific attribute.
 *  \details    The implementations prototype has to be created with OS_FUNC_ATTRIBUTE_DEFINITION.
 *
 *  \param[in]  rettype       The return type of the function.
 *  \param[in]  memclass      The memory class of the function.
 *  \param[in]  attribute     The function attribute {OS_ALWAYS_INLINE; OS_PURE, ...}.
 *  \param[in]  functionName  The function name.
 *  \param[in]  arguments     The function arguments in brackets. E.g. (uint8 a, uint16 b) or (void).
 */
#define OS_FUNC_ATTRIBUTE_DECLARATION(rettype, memclass, attribute, functionName, arguments)                            /* PRQA S 3410 */ /* MD_Os_Rule20.7_3410_Decl */ \
      attribute FUNC(rettype, memclass) functionName arguments

/*! \brief      Provides a function prototype for function definitions with a compiler specific attribute.
 *  \details    The declaration has to be created with OS_FUNC_ATTRIBUTE_DECLARE.
 *
 *  \param[in]  rettype       The return type of the function.
 *  \param[in]  memclass      The memory class of the function.
 *  \param[in]  attribute     The function attribute {OS_ALWAYS_INLINE; OS_PURE, ...}.
 *  \param[in]  functionName  The function name.
 *  \param[in]  arguments     The function arguments in brackets. E.g. (uint8 a, uint16 b) or (void).
 */
#define OS_FUNC_ATTRIBUTE_DEFINITION(rettype, memclass, attribute, functionName, arguments)                             /* PRQA S 3410 */ /* MD_Os_Rule20.7_3410_Decl */ \
      FUNC(rettype, memclass)  functionName arguments

/*! \brief    Tells the compiler that the conditional statement is likely to be true.
 *  \details
 *  The compiler, in turn, can then perform block reordering, and other optimization to
 *  improve the performance of conditional branches.
 *
 *  \param[in]  CONDITION   The condition which is likely to be true
 */
# define OS_LIKELY(CONDITION)           (CONDITION)

/*! \brief    Tells the compiler that the conditional statement is likely to be false.
 *  \details
 *  The compiler, in turn, can then perform block reordering, and other optimization to
 *  improve the performance of conditional branches.
 *
 *  \param[in]  CONDITION   The condition which is likely to be false
 */
# define OS_UNLIKELY(CONDITION)         (CONDITION)




/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/


#endif /* OS_HAL_COMPILER_MICROSOFT_H */

/*!
 * \}
 */
/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_Compiler_Microsoft.h
 **********************************************************************************************************************/
