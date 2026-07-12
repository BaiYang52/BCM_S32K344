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
 * \file  Os_Hal_Entry.inc
 * \brief  This component handles hardware exceptions and interrupts.
 * \internal
 *  Hardware manuals: See derivative specific header (Os_Hal_Derivative_*Int.h)
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

#ifndef OS_HAL_ENTRY_INC
# define OS_HAL_ENTRY_INC

# include "Os_Hal_Derivative.h"
# include "Os_Hal_Entry_Cfg.h"
# include "Os_Hal_MemoryProtection_Cfg.h"
# include "Os_Hal_Compiler.inc"

  OS_HAL_ASM_PRESERVE8

  OS_HAL_ASM_CODE_SECTION(OS_CODE)

  OS_HAL_ASM_THUMB_CODE
  OS_HAL_ASM_IMPORT(Os_IsrRun)

  OS_HAL_ASM_IMPORT(Os_UnhandledIrq)

# if defined(OS_CFG_DERIVATIVEGROUP_CYT2BX) || \
     defined(OS_CFG_DERIVATIVEGROUP_CYT2CX)
#  include "Os_Hal_Entry_CYT2.inc"
# elif defined(OS_CFG_DERIVATIVEGROUP_CYT4BFX) || \
       defined(OS_CFG_DERIVATIVEGROUP_CYT4BBX) || \
       defined(OS_CFG_DERIVATIVEGROUP_CYT4DNX)
#  include "Os_Hal_Entry_CYT4.inc"
# elif defined(OS_CFG_DERIVATIVEGROUP_IMX8X) || \
       defined(OS_CFG_DERIVATIVEGROUP_IMX8XDUALCORE) || \
       defined(OS_CFG_DERIVATIVEGROUP_IMX8DXL)
#  include "Os_Hal_Entry_IMX8x.inc"
# endif /* OS_CFG_DERIVATIVE_<> */

# if defined (OS_HAL_ARCH_V7M)
#  include "Os_Hal_Entry_v7m.inc"
# elif defined (OS_HAL_ARCH_V8M)                                                                                        /* COV_OS_DERIVATIVEBETAMODULE */
#  include "Os_Hal_Entry_v8m.inc"
# else
#  error "Undefined or unsupported architecture"
# endif


#endif /* defined(OS_HAL_ENTRY_INC) */

/**********************************************************************************************************************
 *  END OF FILE: Os_Hal_Entry.inc
 *********************************************************************************************************************/
