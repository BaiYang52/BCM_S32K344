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
 *  \ingroup    Os_Hal
 *  \defgroup   Os_Hal_Compiler  HAL Compiler
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
 *  \file       Os_Hal_CompilerInt.h
 *  \brief      Additional services defined here are available to the user.
 *
 **********************************************************************************************************************/
/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/
                                                                                                                        /* PRQA S 0388  EOF */ /* MD_MSR_Dir1.1 */
#ifndef OS_HAL_COMPILERINT_H
# define OS_HAL_COMPILERINT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
/* AUTOSAR includes */
# include "Std_Types.h"

/* Os module declarations */

/* Os kernel module dependencies */
# include "Os_Cfg.h"
# include "Os_CommonInt.h"

/* Os hal dependencies */
# if defined(OS_STATIC_CODE_ANALYSIS)                                                                                   /* COV_OS_STATICCODEANALYSIS */
#  include "Os_Hal_StaticCodeAnalysis.h"
# else
#  include "Os_Hal_Compiler_Microsoft.h"
# endif


/***********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 **********************************************************************************************************************/


/***********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 **********************************************************************************************************************/


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

 MD_Os_Hal_Dir1.1_0647:
      Reason:     The CANoe task id type is unique for each core. An id can be reused on a different core.
      Risk:       Comparing two CANoe task from different cores may falsely evaluated to true.
      Prevention: CANoe task are only checked for equality if they run on the same core.

 MD_Os_Hal_Rule1.3_0602:
      Reason:     This is a linker label which starts with the '_' character. It is not an reserved identifier used
                  by a library.
      Risk:       None.
      Prevention: None.

 MD_Os_Hal_Rule1.3_3437:
      Reason:     The setjmp(), longjmp() function are the only way to efficiently perform context save/restore
                  operation, without involving expensive thread primitives, of the underlying POSIX layer.
                  They are conforming to the standards POSIX.1-2008 and C99.
      Risk:       None.
      Prevention: None.

 MD_Os_Hal_Rule8.13_3673_Stubs:
      Reason:     This function has an empty implementation, the real definition is provided by not analyzed external
                  libraries. The usage of pointers makes no sense to be analyzed on stubs.
      Risk:       None.
      Prevention: None.

 MD_Os_Hal_Rule11.1_0313:
      Reason:     A function without parameters is casted to one with the same return value and one pointer
                  parameter. Calling the casted one with a dummy argument cannot lead to wrong stack usage by the
                  compiler.
      Risk:       None.
      Prevention: None.

 MD_Os_Hal_Rule11.8_0311:
      Reason:     The purpose of this cast is to change the qualification only at certain instances (as needed by
                  library functions(longjmp) and CANoeEmu) and keeping the rest as constant.
      Risk:       None.
      Prevention: None.

 MD_Os_Hal_Rule17.1_1337_StaticCodeAnalysis:
      Reason:     The function with variable arguments originates from the CANoe API and is defined there.
                  To abstract the implementation details of the function for the MISRA analysis, it
                  is necessary to stub it. Because of that, a function with a variable number of parameters
                  needs to be defined.
      Risk:       None.
      Prevention: None.

 MD_Os_Hal_Rule18.4_0488:
      Reason:     Pointer arithmetic is used here to calculate the stack usage. The returned value is never used as a pointer.
      Risk:       None.
      Prevention: None.

 MD_Os_Hal_Rule_21.2_SetJmpLib:
      Reason:     The type, function or macro is part of the setjmp library. This library is not included for static code
                  analysis, which is why the identifier needs to be declared.
      Risk:       None.
      Prevention: None.

 MD_Os_Hal_Rule21.4_5137:
      Reason:     The setjmp(), longjmp() function are the only way to efficiently perform context save/restore
                  operation, without involving expensive thread primitives, of the underlying POSIX layer.
                  They are conforming to the standards POSIX.1-2008 and C99.
      Risk:       None.
      Prevention: None.
*/

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_CompilerInt.h
 **********************************************************************************************************************/

