/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**
 *  \ingroup    Os_Hal
 *  \addtogroup Os_Hal_Compiler  HAL Compiler
 *  \brief      Abstraction of compiler specific keywords to allow the compiler to generate more efficient code.
 *  \details
 *  The kernel should be able to give the compiler meta information to generate more efficient code.
 *  It also needs to tell the linker where to put data and code to implement Memory Protection.
 *  In multi core systems there may be the need for data/code duplication.
 *  The keywords to provide this information are not part of ANSI-C and are highly compiler dependent.
 *
 *  AUTOSAR already defines concepts to abstract compiler specifics:
 *  - [AUTOSAR Compiler Abstraction](http:\\www.autosar.org/fileadmin/files/releases/4-2/software-architecture/implementation-integration/standard/AUTOSAR_SWS_CompilerAbstraction.pdf)
 *  - [AUTOSAR Memory Mapping](http:\\www.autosar.org/fileadmin/files/releases/3-2/software-architecture/implementation-integration/standard/AUTOSAR_SWS_MemoryMapping.pdf)
 *
 *  They are used in MICROSAR OS. This module defines additional compiler abstraction keywords for MICROSAR OS.
 *  They are relevant to provide OS functionality or improve efficiency.
 *
 *  \{
 *
 *  \file
 *  \brief      Additional services defined here are available to the user.
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_COMPILERINT_H
# define OS_HAL_COMPILERINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* Os kernel module dependencies */
# include "Os_Cfg.h"
# include "Os_CommonInt.h"

# include "Os_Hal_Derivative.h"

# if defined(OS_STATIC_CODE_ANALYSIS)                                                                                   /* COV_OS_STATICCODEANALYSIS */
#  include "Os_Hal_StaticCodeAnalysis.h"
# else
#  if defined (OS_CFG_COMPILER_GREENHILLS)
#   include "Os_Hal_Compiler_GreenhillsInt.h"
#  elif defined (OS_CFG_COMPILER_IAR)
#   include "Os_Hal_Compiler_IarInt.h"
#  elif defined (OS_CFG_COMPILER_TEXASINSTRUMENTS)
#   include "Os_Hal_Compiler_TiInt.h"
#  elif defined (OS_CFG_COMPILER_ARM)
#   include "Os_Hal_Compiler_ArmInt.h"
#  elif defined (OS_CFG_COMPILER_LINARO)
#   include "Os_Hal_Compiler_GccInt.h"
#  elif defined (OS_CFG_COMPILER_ARM6)
#   include "Os_Hal_Compiler_Arm6Int.h"
#  elif defined (OS_CFG_COMPILER_TASKING)
#   include "Os_Hal_Compiler_TaskingInt.h"
#  elif defined (OS_CFG_COMPILER_GNU)
#   include "Os_Hal_Compiler_HighTecInt.h"
#  elif defined (OS_CFG_COMPILER_GCCNXP)
#   include "Os_Hal_Compiler_GccNxpInt.h"
#  elif defined (OS_CFG_COMPILER_LLVMDIAB)
#   include "Os_Hal_Compiler_LlvmDiabInt.h"
#  elif defined (OS_CFG_COMPILER_LLVMTEXASINSTRUMENTS)
#   include "Os_Hal_Compiler_LlvmTIInt.h"
#  elif defined (OS_CFG_COMPILER_LLVMHIGHTEC)
#   include "Os_Hal_Compiler_LlvmHighTecInt.h"
#  elif defined (OS_CFG_COMPILER_MICROCHIP)
#   include "Os_Hal_Compiler_MicrochipInt.h"
# else
#   error "Undefined or unsupported compiler"
#  endif
# endif


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/

/*! \brief      Tells the compiler that the conditional statement is likely to be true.
 *  \details    The compiler, in turn, can then perform block reordering, and other optimization to
 *              improve the performance of conditional branches.
 *  \param[in]  CONDITION   The condition which is likely to be true
 */
# define OS_LIKELY(CONDITION)       (CONDITION)

/*! \brief      Tells the compiler that the conditional statement is likely to be false.
 *  \details    The compiler, in turn, can then perform block reordering, and other optimization to
 *              improve the performance of conditional branches.
 *  \param[in]  CONDITION   The condition which is likely to be false
 */
# define OS_UNLIKELY(CONDITION)     (CONDITION)


# if !defined (OS_ALWAYS_INLINE)
#  error "OS_ALWAYS_INLINE is undefined!"
# endif


# if !defined (OS_NORETURN)
#  error "OS_NORETURN is undefined!"
# endif


# if !defined (OS_FLATTEN)
#  error "OS_FLATTEN undefined"
# endif


# if !defined (OS_NOSIDEEFFECTS)
#  error "OS_NOSIDEEFFECTS undefined"
# endif


# if !defined (OS_PURE)
#  error "OS_PURE undefined"
# endif


# if !defined (OS_NAKED)
#  error "OS_NAKED undefined"
# endif


# if !defined (OS_LIKELY)
#  error "OS_LIKELY(Condition) undefined"
# endif


# if !defined (OS_UNLIKELY)
#  error "OS_UNLIKELY(Condition) undefined"
# endif


/***********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

#endif /* OS_HAL_COMPILERINT_H */

/* module specific MISRA deviations:

  MD_Os_Hal_Rule1.2_1039:
      Reason:     Some compilers treat an array of length one declared as the final member of a struct as a flexible
                  array member.
      Risk:       None, as an array of length one is also a generally valid C construct.
      Prevention: None.

 MD_Os_Hal_Rule8.13_3673:
      Reason:     The parameter may be modified in other HAL implementations, thus it is necessary to keep it
                  as "pointer to non-const".
      Risk:       None.
      Prevention: None.

  MD_Os_Hal_Rule8.13_3673_NotUsed:
      Reason:     This input parameter is not used in this HAL implementation, thus it is not modified. However, since
                  it may be used or modified in other platforms, it is unavoidable to keep it as "pointer to non-const".
      Risk:       None.
      Prevention: None.

  MD_Os_Hal_Rule11.1_0305:
      Reason:     This statement converts a function pointer to an integral address or vice versa. This is unavoidable to
                  use the function pointer address.
      Risk:       The risk for incompatibility is low here as the code is used on a specific hardware platform only with
                  known sizes of pointer types and integral types.
      Prevention: The code is tested with all compilers it shall be compiled with.

 MD_Os_Hal_Rule11.2_0324:
      Reason:     This statement converts a pointer to incomplete type to an integral type or vice versa. This is
                  unavoidable to use the linker symbol.
      Risk:       The risk for incompatibility is low here as the code is used on a specific hardware platform only with
                  known sizes of pointer types and integral types.
      Prevention: The code is tested with all compilers it shall be compiled with.

 MD_Os_Hal_Rule11.4_0303:
      Reason:     This statement converts an integral address to a pointer or vice versa. This is unavoidable for
                  memory access of that address or put a pointer to a core register.
      Risk:       None.
      Prevention: None.

 MD_Os_Hal_Rule11.4_0306:
      Reason:     This statement converts an integral address to a pointer or vice versa. This is unavoidable for
                  memory access of that address or put a pointer to a core register.
      Risk:       The risk for incompatibility is low here as the code is used on a specific hardware platform only with
                  known sizes of pointer types and integral types.
      Prevention: The code is tested with all compilers it shall be compiled with.

 */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_CompilerInt.h
 **********************************************************************************************************************/

