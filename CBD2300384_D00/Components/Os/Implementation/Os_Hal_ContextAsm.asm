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
 *  \file       Os_Hal_ContextAsm.asm
 *  \brief      Assembler parts of context switching primitives for ARM-M.
 *  \internal
 *   Hardware manuals: See derivative specific header (Os_Hal_Derivative_*Int.h)
 *   Errata sheets: See derivative specific header
 *   Specifics: -
 *   Used registers and bits: General Purpose and System Registers
 *   Operating modes: -
 *   Hardware features related to independence or partitioning: -
 *   Access mechanism: Core special function registers
 *   Hardware diagnostics: -
 *  \endinternal
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  REVISION HISTORY
 *  --------------------------------------------------------------------------------------------------------------------
 *  Refer to Os_Hal_Os.h.
 **********************************************************************************************************************/

# include "Os_Hal_Derivative.h"
# if defined(OS_CFG_DERIVATIVEGROUP_CYT4BFX) || defined(OS_CFG_DERIVATIVEGROUP_CYT4BBX) || defined(OS_CFG_DERIVATIVEGROUP_CYT4DNX)
# include "Os_Hal_ContextAsm_CYT4Bx.inc"
# elif defined(OS_CFG_DERIVATIVEGROUP_S32X) || defined(OS_CFG_DERIVATIVEGROUP_S32G3X)
# include "Os_Hal_ContextAsm_S32x.inc"
# elif defined(OS_CFG_DERIVATIVEGROUP_S32K3XX)
# include "Os_Hal_ContextAsm_S32K3x.inc"
# elif defined(OS_CFG_DERIVATIVEGROUP_TDA2X)
# include "Os_Hal_ContextAsm_TDA2x.inc"
# elif defined(OS_CFG_DERIVATIVEGROUP_IMX8XDUALCORE)
# include "Os_Hal_ContextAsm_IMX8xDualCore.inc"
# elif defined(OS_CFG_DERIVATIVEGROUP_IMXRT117X)
# include "Os_Hal_ContextAsm_IMXRT.inc"
# elif defined(OS_HAL_ARCH_V8M)
# include "Os_Hal_ContextAsm_v8m.inc"
# elif defined(OS_HAL_ARCH_V7M)
# include "Os_Hal_ContextAsm_v7m.inc"
# else
# error "Undefined or unsupported platform"
# endif
  OS_HAL_ASM_MODULE_END                                                                                                 /* COMP_WARN_OS_HAL_PADDING */
