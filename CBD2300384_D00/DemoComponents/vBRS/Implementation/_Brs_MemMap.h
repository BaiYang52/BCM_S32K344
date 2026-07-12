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

#if defined (_MICROSOFT_C_VTT_)
  /* VTT/CANoeEmu is unsing Ansi as compiler value (means Gnu), but does not contain a Makefile which sets BRS_COMP_ */
  /* The vVIRTUALtarget default compiler options do not even support #warning:
     fatal error C1021: invalid preprocessor command 'warning'
     So no warnings show up for this platform */
  #define BRS_COMP_ANSI
#endif

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/
#if defined (BRS_COMP_GHS)                  

#else
  #error "Unknown compiler specified! Pragmas for mapping into specific sections not yet implemented for your compiler."
#endif

/**********************************************************************************************************************
 *  CODE sections
 *********************************************************************************************************************/

/*****************************************************************************/
/* StartupCode part, located in BrsMain, which is only used by first execution
 *   instance (e.g. Brs_PreMainStartup())
 *****************************************************************************/
#ifdef BRS_START_SEC_STARTUP_CODE
# if defined (BRS_COMP_GHS)
  #pragma ghs section text = ".brsMainStartup"

# endif

  #undef BRS_START_SEC_STARTUP_CODE
  #undef BRS_MEMMAP_ERROR
#endif

#ifdef BRS_STOP_SEC_STARTUP_CODE
# if defined (BRS_COMP_GHS)
  #pragma ghs section text = default

# endif

  #undef BRS_STOP_SEC_STARTUP_CODE
  #undef BRS_MEMMAP_ERROR
#endif

/*****************************************************************************/
/* Brs_ApplicationEntry section, located in BrsMainStartup, which is used by
 *   every execution instance (Brs_ApplicationEntry()). This allows a fix
 *   address of application entry, needed by some bootloader use-cases.
 *****************************************************************************/
#ifdef BRS_START_SEC_BRSAPPLENTRY_CODE
# if defined (BRS_COMP_GHS)
  #pragma ghs section text = ".brsApplicationEntry"

# endif

  #undef BRS_START_SEC_BRSAPPLENTRY_CODE
  #undef BRS_MEMMAP_ERROR
#endif

#ifdef BRS_STOP_SEC_BRSAPPLENTRY_CODE
# if defined (BRS_COMP_GHS)
  #pragma ghs section text = default

# endif

  #undef BRS_STOP_SEC_BRSAPPLENTRY_CODE
  #undef BRS_MEMMAP_ERROR
#endif

/*****************************************************************************/
/* Code Exception table, configured during startup from first execution
 *   instance (on platforms, where exception table is code)
 *****************************************************************************/
#ifdef BRS_START_SEC_EXCVECT_CODE
# if defined (BRS_COMP_GHS)
#  if defined (BRS_MEMMAP_INLINE_ASSEMBLER_USED)
  /* GHS compiler does not link inline assembly code by usage of #pragma */
  __asm("  .section  \".brsExcVect\",\"ax\"");
  #undef BRS_MEMMAP_INLINE_ASSEMBLER_USED
#  else
  #pragma ghs section text = ".brsExcVect"
#  endif

# endif

  #undef BRS_START_SEC_EXCVECT_CODE
  #undef BRS_MEMMAP_ERROR
#endif

#ifdef BRS_STOP_SEC_EXCVECT_CODE
# if defined (BRS_COMP_GHS)
#  if defined (BRS_MEMMAP_INLINE_ASSEMBLER_USED)
  /* GHS compiler does not link inline assembly code by usage of #pragma */
  __asm("  .text");
  #undef BRS_MEMMAP_INLINE_ASSEMBLER_USED
#  else
  #pragma ghs section text = default
#  endif

# endif

  #undef BRS_STOP_SEC_EXCVECT_CODE
  #undef BRS_MEMMAP_ERROR
#endif

/*****************************************************************************/
/* Code Exception table that is located in RAM, configured from FBL execution
 *   instance (on platforms, where exception table is code)
 *****************************************************************************/
#ifdef BRS_START_SEC_EXCVECTRAM_CODE
# if defined (BRS_COMP_GHS)
#  if defined (BRS_MEMMAP_INLINE_ASSEMBLER_USED)
  /* GHS compiler does not link inline assembly code by usage of #pragma */
  __asm("  .section  \".brsExcVectRam\",\"ax\"");
  #undef BRS_MEMMAP_INLINE_ASSEMBLER_USED
#  else
  #pragma ghs section text = ".brsExcVectRam"
#  endif

# endif

  #undef BRS_START_SEC_EXCVECTRAM_CODE
  #undef BRS_MEMMAP_ERROR
#endif

#ifdef BRS_STOP_SEC_EXCVECTRAM_CODE
# if defined (BRS_COMP_GHS)
#  if defined (BRS_MEMMAP_INLINE_ASSEMBLER_USED)
  /* GHS compiler does not link inline assembly code by usage of #pragma */
  __asm("  .text");
  #undef BRS_MEMMAP_INLINE_ASSEMBLER_USED
#  else
  #pragma ghs section text = default
#  endif

# endif

  #undef BRS_STOP_SEC_EXCVECTRAM_CODE
  #undef BRS_MEMMAP_ERROR
#endif

#if defined (BRS_ENABLE_FBL_SUPPORT)
/*****************************************************************************/
/* FBL specific section, to copy parts of BRS into RAM
 *   (everything, that is needed during re-flashing)
 *****************************************************************************/
# ifdef BRS_START_SEC_RAM_CODE
#  if defined (BRS_COMP_GHS)
  #pragma ghs section text = ".RamCodeSection"

#  endif

  #undef BRS_START_SEC_RAM_CODE
  #undef BRS_MEMMAP_ERROR
# endif

# ifdef BRS_STOP_SEC_RAM_CODE
#  if defined (BRS_COMP_GHS)
  #pragma ghs section text = default

#  endif

  #undef BRS_STOP_SEC_RAM_CODE
  #undef BRS_MEMMAP_ERROR
# endif
#endif /*BRS_ENABLE_FBL_SUPPORT*/

/**********************************************************************************************************************
 *  CONST sections
 *********************************************************************************************************************/

/*****************************************************************************/
/* Constants, that are shared between several execution instances
 *****************************************************************************/
#ifdef BRS_START_SEC_SHARED_CONST
# if defined (BRS_COMP_GHS)
  #pragma ghs section rodata = ".brsSharedConst"

# endif

  #undef BRS_START_SEC_SHARED_CONST
  #undef BRS_MEMMAP_ERROR
#endif

#ifdef BRS_STOP_SEC_SHARED_CONST
# if defined (BRS_COMP_GHS)
  # pragma ghs section rodata = default

# endif

  #undef BRS_STOP_SEC_SHARED_CONST
  #undef BRS_MEMMAP_ERROR
#endif

/*****************************************************************************/
/* Const Exception table, configured during startup from first execution
 *   instance (on platforms, where exception table is const)
 *****************************************************************************/
#ifdef BRS_START_SEC_EXCVECT_CONST
# if defined (BRS_COMP_GHS)
  #pragma ghs section rodata = ".brsExcVectConst"

# endif

  #undef BRS_START_SEC_EXCVECT_CONST
  #undef BRS_MEMMAP_ERROR
#endif

#ifdef BRS_STOP_SEC_EXCVECT_CONST
# if defined (BRS_COMP_GHS)
  #pragma ghs section rodata = default

# endif

  #undef BRS_STOP_SEC_EXCVECT_CONST
  #undef BRS_MEMMAP_ERROR
#endif

/*****************************************************************************/
/* Const Exception table that is located in RAM, configured from FBL execution
 *   instance (on platforms, where exception table is const)
 *****************************************************************************/
#ifdef BRS_START_SEC_EXCVECTRAM_CONST
# if defined (BRS_COMP_GHS)
  #pragma ghs section rodata = ".brsExcVectRam"

# endif

  #undef BRS_START_SEC_EXCVECTRAM_CONST
  #undef BRS_MEMMAP_ERROR
#endif

#ifdef BRS_STOP_SEC_EXCVECTRAM_CONST
# if defined (BRS_COMP_GHS)
  # pragma ghs section rodata = default

# endif

  #undef BRS_STOP_SEC_EXCVECTRAM_CONST
  #undef BRS_MEMMAP_ERROR
#endif

#if defined (BRS_ENABLE_FBL_SUPPORT)
/*****************************************************************************/
/* FBL specific section to copy parts of BRS into RAM
 *   (everything, that is needed during re-flashing)
 *****************************************************************************/
# ifdef BRS_START_SEC_RAM_CONST
#  if defined (BRS_COMP_GHS)
  #pragma ghs section rodata = ".RamConstSection"

#  endif

  #undef BRS_START_SEC_RAM_CONST
  #undef BRS_MEMMAP_ERROR
# endif

# ifdef BRS_STOP_SEC_RAM_CONST
#  if defined (BRS_COMP_GHS)
  #pragma ghs section rodata = default

#  endif

  #undef BRS_STOP_SEC_RAM_CONST
  #undef BRS_MEMMAP_ERROR
# endif

# if defined (BRS_FBL_INSTANCE_FBL)
/*****************************************************************************/
/* FBL specific section to store the FBL instance presence pattern into a
 *   dedicated section
 *****************************************************************************/
#  ifdef BRS_START_SEC_FBL_PP_CONST
#   if defined (BRS_COMP_GHS)
  #pragma ghs section rodata = ".FblPresencePatternSection"

#   endif

  #undef BRS_START_SEC_FBL_PP_CONST
  #undef BRS_MEMMAP_ERROR
#  endif

#  ifdef BRS_STOP_SEC_FBL_PP_CONST
#   if defined (BRS_COMP_GHS)
  #pragma ghs section rodata = default

#   endif

  #undef BRS_STOP_SEC_FBL_PP_CONST
  #undef BRS_MEMMAP_ERROR
#  endif
# endif /*BRS_FBL_INSTANCE_FBL*/
#endif /*BRS_ENABLE_FBL_SUPPORT*/

/**********************************************************************************************************************
 *  VAR sections
 *********************************************************************************************************************/

/*****************************************************************************/
/* Variables, that are shared between several execution instances
 *   (e.g. brsMain_ResetReason)
 *****************************************************************************/
#ifdef BRS_START_SEC_SHARED_VAR
# if defined (BRS_COMP_GHS)
  #pragma ghs section bss = ".brsSharedVar"

#  endif

  #undef BRS_START_SEC_SHARED_VAR
  #undef BRS_MEMMAP_ERROR
# endif

#ifdef BRS_STOP_SEC_SHARED_VAR
# if defined (BRS_COMP_GHS)
  #pragma ghs section bss = default

# endif

  #undef BRS_STOP_SEC_SHARED_VAR
  #undef BRS_MEMMAP_ERROR
#endif

#if defined (BRS_MEMMAP_ERROR)
  #error "Error in Brs_MemMap.h: MemMap section is not included!"
#endif
