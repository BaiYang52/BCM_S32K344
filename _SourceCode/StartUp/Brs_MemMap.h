/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/** \file  File:  Brs_MemMap.h
 *      Project:  Vector Basic Runtime System
 *       Module:  BrsMain
 *
 *  \brief Description:  Module specific MemMap Header to allow linking of specific parts of BRS into specific
 *                       memory sections.
 *
 *  \attention Please note:
 *    The demo and example programs only show special aspects of the software. With regard to the fact
 *    that these programs are meant for demonstration purposes only, Vector Informatik liability shall be
 *    expressly excluded in cases of ordinary negligence, to the extent admissible by law or statute.
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to BrsMain.h.
 *********************************************************************************************************************/

#ifndef BRS_MEMMAP_ERROR
  #define BRS_MEMMAP_ERROR
#endif

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/
#if (BRSHW_SOURCECODE_TEMPLATE_VERSION >= 0x0103u)
/* vBaseEnv platforms, beginning with Brs_Template 1.03, are using MemMap to use several specific sections */
# if defined (BRS_COMP_IAR)                  

# else
  #error "Unknown compiler specified! Pragmas for mapping into specific sections not yet implemented for your compiler."
# endif
#endif /*BRSHW_SOURCECODE_TEMPLATE_VERSION >= 0x0103u*/

#if defined (BRS_ENABLE_FBL_SUPPORT)
#error "Add your compiler-specific pragma to allocate code in a section that will later be configured in the linker script to run from RAM"
#endif

/**********************************************************************************************************************
 *  CODE sections
 *********************************************************************************************************************/
/* FBL specific section to copy parts of BRS into RAM (everything, that is needed during re-flashing) */
#ifdef BRS_START_SEC_RAM_CODE
  /* #pragma <enter your compiler specific pragma> */

  /* Sample for PPC and DiabData: */
  /*#pragma section CODE "RAMCODE"*/

  #undef BRS_START_SEC_RAM_CODE
  #undef BRS_MEMMAP_ERROR
#endif

#ifdef BRS_STOP_SEC_RAM_CODE
  /* #pragma <enter your compiler specific pragma> */

  /* Sample for PPC and DiabData: */
  /*#pragma section CODE*/

  #undef BRS_STOP_SEC_RAM_CODE
  #undef BRS_MEMMAP_ERROR
#endif

#if (BRSHW_SOURCECODE_TEMPLATE_VERSION >= 0x0103u)
/* vBaseEnv platforms, beginning with Brs_Template 1.03, are using MemMap to use several specific sections */

/*****************************************************************************/
/* StartupCode part, located in BrsMain, which is only used by first execution
 *   instance (e.g. Brs_PreMainStartup())
 *****************************************************************************/
# ifdef BRS_START_SEC_STARTUP_CODE
#  if defined (BRS_COMP_IAR)
  #pragma default_function_attributes = @ ".brsMainStartup"

#  endif

  #undef BRS_START_SEC_STARTUP_CODE
  #undef BRS_MEMMAP_ERROR
# endif

# ifdef BRS_STOP_SEC_STARTUP_CODE
#  if defined (BRS_COMP_IAR)
  #pragma default_function_attributes = 

#  endif

  #undef BRS_STOP_SEC_STARTUP_CODE
  #undef BRS_MEMMAP_ERROR
# endif

/*****************************************************************************/
/* Code Exception table, configured during startup from first execution
 *   instance (on platforms, where exception table is code)
 *****************************************************************************/
# ifdef BRS_START_SEC_EXCVECT_CODE
#  if defined (BRS_COMP_IAR)
  #pragma default_function_attributes = @ ".brsExcVect"

#  endif

  #undef BRS_START_SEC_EXCVECT_CODE
  #undef BRS_MEMMAP_ERROR
# endif

# ifdef BRS_STOP_SEC_EXCVECT_CODE
#  if defined (BRS_COMP_IAR)
  #pragma default_function_attributes = 

#  endif

  #undef BRS_STOP_SEC_EXCVECT_CODE
  #undef BRS_MEMMAP_ERROR
# endif

/*****************************************************************************/
/* Code Exception table that is located in RAM, configured from FBL execution
 *   instance (on platforms, where exception table is code)
 *****************************************************************************/
# ifdef BRS_START_SEC_EXCVECTRAM_CODE
#  if defined (BRS_COMP_IAR)
  #pragma default_function_attributes = @ ".brsExcVectRam"

#  endif

  #undef BRS_START_SEC_EXCVECTRAM_CODE
  #undef BRS_MEMMAP_ERROR
# endif

# ifdef BRS_STOP_SEC_EXCVECTRAM_CODE
#  if defined (BRS_COMP_IAR)
  #pragma default_function_attributes = 

#  endif

  #undef BRS_STOP_SEC_EXCVECTRAM_CODE
  #undef BRS_MEMMAP_ERROR
# endif
#endif /*BRSHW_SOURCECODE_TEMPLATE_VERSION >= 0x0103u*/

/**********************************************************************************************************************
 *  CONST sections
 *********************************************************************************************************************/
/* FBL specific section to copy parts of BRS into RAM (everything, that is needed during re-flashing) */
#ifdef BRS_START_SEC_RAM_CONST
  /* #pragma <enter your compiler specific pragma> */

  /* Sample for PPC and DiabData: */
  /*#pragma section CONST "RAMCODE"*/

  #undef BRS_START_SEC_RAM_CONST
  #undef BRS_MEMMAP_ERROR
#endif

#ifdef BRS_STOP_SEC_RAM_CONST
  /* #pragma <enter your compiler specific pragma> */

  /* Sample for PPC and DiabData: */
  /*#pragma section CONST*/

  #undef BRS_STOP_SEC_RAM_CONST
  #undef BRS_MEMMAP_ERROR
#endif

#if (BRSHW_SOURCECODE_TEMPLATE_VERSION >= 0x0103u)
/* vBaseEnv platforms, beginning with Brs_Template 1.03, are using MemMap to use several specific sections */

/*****************************************************************************/
/* Constants, that are shared between several execution instances
 *****************************************************************************/
# ifdef BRS_START_SEC_SHARED_CONST
#  if defined (BRS_COMP_IAR)
  #pragma default_variable_attributes = @ ".brsSharedConst"

#  endif

  #undef BRS_START_SEC_SHARED_CONST
  #undef BRS_MEMMAP_ERROR
# endif

# ifdef BRS_STOP_SEC_SHARED_CONST
#  if defined (BRS_COMP_IAR)
  #pragma default_variable_attributes = 

#  endif

  #undef BRS_STOP_SEC_SHARED_CONST
  #undef BRS_MEMMAP_ERROR
# endif

/*****************************************************************************/
/* Const Exception table, configured during startup from first execution
 *   instance (on platforms, where exception table is const)
 *****************************************************************************/
# ifdef BRS_START_SEC_EXCVECT_CONST
#  if defined (BRS_COMP_IAR)
  #pragma default_variable_attributes = @ ".brsExcVectConst"

#  endif

  #undef BRS_START_SEC_EXCVECT_CONST
  #undef BRS_MEMMAP_ERROR
# endif

# ifdef BRS_STOP_SEC_EXCVECT_CONST
#  if defined (BRS_COMP_IAR)
  #pragma default_variable_attributes = 

#  endif

  #undef BRS_STOP_SEC_EXCVECT_CONST
  #undef BRS_MEMMAP_ERROR
# endif

/*****************************************************************************/
/* Const Exception table that is located in RAM, configured from FBL execution
 *   instance (on platforms, where exception table is const)
 *****************************************************************************/
# ifdef BRS_START_SEC_EXCVECTRAM_CONST
#  if defined (BRS_COMP_IAR)
  #pragma default_variable_attributes = @ ".brsExcVectRam"

#  endif

  #undef BRS_START_SEC_EXCVECTRAM_CONST
  #undef BRS_MEMMAP_ERROR
# endif

# ifdef BRS_STOP_SEC_EXCVECTRAM_CONST
#  if defined (BRS_COMP_IAR)
  #pragma default_variable_attributes = 

#  endif

  #undef BRS_STOP_SEC_EXCVECTRAM_CONST
  #undef BRS_MEMMAP_ERROR
# endif
#endif /*BRSHW_SOURCECODE_TEMPLATE_VERSION >= 0x0103u*/

/**********************************************************************************************************************
 *  VAR sections
 *********************************************************************************************************************/
#if (BRSHW_SOURCECODE_TEMPLATE_VERSION >= 0x0103u)
/* vBaseEnv platforms, beginning with Brs_Template 1.03, are using MemMap to use several specific sections */

/*****************************************************************************/
/* Variables, that are shared between several execution instances
 *   (e.g. brsMain_ResetReason)
 *****************************************************************************/
# ifdef BRS_START_SEC_SHARED_VAR
#  if defined (BRS_COMP_IAR)
  #pragma default_variable_attributes = @ ".brsSharedVar"

#  endif

  #undef BRS_START_SEC_SHARED_VAR
  #undef BRS_MEMMAP_ERROR
# endif

# ifdef BRS_STOP_SEC_SHARED_VAR
#  if defined (BRS_COMP_IAR)
  #pragma default_variable_attributes = 

#  endif

  #undef BRS_STOP_SEC_SHARED_VAR
  #undef BRS_MEMMAP_ERROR
# endif
#endif /*BRSHW_SOURCECODE_TEMPLATE_VERSION >= 0x0103u*/

#if defined (BRS_MEMMAP_ERROR)
  #error "Error in Brs_MemMap.h: MemMap section is not included!"
#endif
