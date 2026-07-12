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
 *  \file       Os_Hal_CompilerInt.inc
 *  \brief      Additional services defined here are available to the user.
 *  \details
 *  The kernel should be able to give the compiler meta information to generate more efficient code.
 *  It also needs to tell the linker where to put data and code to implement Memory Protection.
 *  In multi core systems there may be the need for data/code duplication.
 *  The keywords to provide this information are not part of ANSI-C and are highly compiler dependent.
 *
 *  AUTOSAR already defines concepts to abstract compiler specifics:
 *  - [AUTOSAR Compiler Abstraction](http://www.autosar.org/fileadmin/files/releases/4-2/software-architecture/implementation-integration/standard/AUTOSAR_SWS_CompilerAbstraction.pdf)
 *  - [AUTOSAR Memory Mapping](http://www.autosar.org/fileadmin/files/releases/3-2/software-architecture/implementation-integration/standard/AUTOSAR_SWS_MemoryMapping.pdf)
 *
 *  They are used in MICROSAR OS. This module defines additional compiler abstraction keywords for MICROSAR OS.
 *  They are relevant to provide OS functionality or improve efficiency.
 *
 *
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

#ifndef OS_HAL_COMPILERINT_INC
# define OS_HAL_COMPILERINT_INC

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
# include "Os_Cfg.h"

# if defined (OS_CFG_COMPILER_GREENHILLS)
#  include "Os_Hal_Compiler_GreenhillsInt.inc"
# elif defined (OS_CFG_COMPILER_IAR)
#  include "Os_Hal_Compiler_IarInt.inc"
# elif defined (OS_CFG_COMPILER_TEXASINSTRUMENTS)
#  include "Os_Hal_Compiler_TiInt.inc"
# elif defined (OS_CFG_COMPILER_ARM)
#  include "Os_Hal_Compiler_ArmInt.inc"
# elif defined (OS_CFG_COMPILER_LINARO)
#  include "Os_Hal_Compiler_GccInt.inc"
# elif defined (OS_CFG_COMPILER_ARM6)
#  include "Os_Hal_Compiler_Arm6Int.inc"
# elif defined (OS_CFG_COMPILER_TASKING)
#  include "Os_Hal_Compiler_TaskingInt.inc"
# elif defined (OS_CFG_COMPILER_GNU)
#  include "Os_Hal_Compiler_HighTecInt.inc"
#  elif defined (OS_CFG_COMPILER_GCCNXP)
#   include "Os_Hal_Compiler_GccNxpInt.inc"
#  elif defined (OS_CFG_COMPILER_LLVMDIAB)
#   include "Os_Hal_Compiler_LlvmDiabInt.inc"
#  elif defined (OS_CFG_COMPILER_LLVMTEXASINSTRUMENTS)
#   include "Os_Hal_Compiler_LlvmTIInt.inc"
#  elif defined (OS_CFG_COMPILER_LLVMHIGHTEC)
#   include "Os_Hal_Compiler_LlvmHighTecInt.inc"
#  elif defined (OS_CFG_COMPILER_MICROCHIP)
#   include "Os_Hal_Compiler_MicrochipInt.inc"
# else
#  error "Undefined or unsupported compiler"
# endif


#endif /* OS_HAL_COMPILERINT_INC */

/*!
 * \}
 */

/***********************************************************************************************************************
 *  END OF FILE: Os_Hal_CompilerInt.inc
 **********************************************************************************************************************/
