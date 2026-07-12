/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2020 by Vector Informatik GmbH.                                              All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  Platform_Types.h
 *    Component:  -
 *       Module:  -
 *    Generator:  -
 *
 *  Description:  Provision of Platform Types for CANoeEmu
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2014-03-17  visml                 Creation based on template V1.03.01.
 *  02.00.00  2015-02-11  viswge                FEAT 254: Renaming to VTT.
 *            2015-02-11  viswge                Adding uint64 and sint64 data types.
 *  02.00.01  2018-08-14  virsmn                MISRA 2012 conformance.
 *  02.00.02  2019-09-12  virsmn                Update for boolean usage.
 *  02.00.03  2019-11-21  visas                 Update for 64bit support.
 *  02.00.04  2020-08-06  virsmn                Updated typedef for uint32 and sint32, update for MISRA.
 *  02.01.00  2020-11-18  visbwa                Removed AUTHOR IDENTITY, changed filter mechanism (Product instead of
 *                                              AutosarVersion)
 *  02.01.01  2021-12-17  visto                 TAR-3952 fix MISRA warnings for TRUE/FALSE constants
*********************************************************************************************************************/

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject : CommonAsr_Vtt CQComponent : Impl_PlatformTypes */
#define COMMONASR_VTT_IMPL_PLATFORMTYPES_VERSION         0x0201
#define COMMONASR_VTT_IMPL_PLATFORMTYPES_RELEASE_VERSION 0x00

#define PLATFORM_VENDOR_ID    30u   /* SREQ00015439, SREQ00015413 */
#define PLATFORM_MODULE_ID    199u  /* SREQ00015439, SREQ00015413 */

/* AUTOSAR Software Specification Document Version Information */

/* AUTOSAR release 4.0 R3 */
#define PLATFORM_AR_RELEASE_MAJOR_VERSION      (4u)
#define PLATFORM_AR_RELEASE_MINOR_VERSION      (0u)
#define PLATFORM_AR_RELEASE_REVISION_VERSION   (3u)

/* Component Version Information */
#define PLATFORM_SW_MAJOR_VERSION       (2u)
#define PLATFORM_SW_MINOR_VERSION       (1u)
#define PLATFORM_SW_PATCH_VERSION       (1u)

#define CPU_TYPE_8       (8u)
#define CPU_TYPE_16      (16u)
#define CPU_TYPE_32      (32u)
#define CPU_TYPE_64      (64u)

#define MSB_FIRST        (0u)  /* big endian bit ordering */
#define LSB_FIRST        (1u)  /* little endian bit ordering */

#define HIGH_BYTE_FIRST  (0u)  /* big endian byte ordering */
#define LOW_BYTE_FIRST   (1u)  /* little endian byte ordering */

#ifdef __PRQA__
# ifndef TRUE
#  define TRUE          (_Bool)1
# endif

# ifndef FALSE
#  define FALSE         (_Bool)0
# endif
#else
# ifndef TRUE
  #define TRUE   (1u)
# endif

# ifndef FALSE
  #define FALSE  (0u)
# endif
#endif /* PRQA */

#if defined(_M_IX86)
  /* Visual C++ compiler targeting 32 bit code for x86 */
  #define CPU_TYPE         CPU_TYPE_32
#elif defined(_M_AMD64)
  /* Visual C++ compiler targeting 64 bit code for x64 */
  #define CPU_TYPE         CPU_TYPE_64
#elif defined(__x86_64__)
  /* gcc or clang compiler targeting 64 bit code for x64 */
  #define CPU_TYPE         CPU_TYPE_64
#elif defined(__i386__)
  /* gcc or clang compiler targeting 32 bit code for x86 */
  #define CPU_TYPE         CPU_TYPE_32
#else
  #error "Unknown processor architecture"
#endif

#define CPU_BIT_ORDER    LSB_FIRST       /*little endian bit ordering*/

#define CPU_BYTE_ORDER   LOW_BYTE_FIRST  /*little endian byte ordering*/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
#ifdef __PRQA__
typedef _Bool boolean;
#else
typedef unsigned char         boolean;       /*        TRUE .. FALSE           */
#endif
typedef signed char           sint8;         /*        -127 .. +127            */
typedef unsigned char         uint8;         /*           0 .. 255             */
typedef signed short          sint16;        /*      -32767 .. +32767          */
typedef unsigned short        uint16;        /*           0 .. 65535           */
typedef signed int            sint32;        /* -2147483647 .. +2147483647     */
typedef unsigned int          uint32;        /*           0 .. 4294967295      */

typedef signed char           sint8_least;   /* At least 7 bit + 1 bit sign    */
typedef unsigned char         uint8_least;   /* At least 8 bit                 */
typedef signed short          sint16_least;  /* At least 15 bit + 1 bit sign   */
typedef unsigned short        uint16_least;  /* At least 16 bit                */
typedef signed int            sint32_least;  /* At least 31 bit + 1 bit sign   */
typedef unsigned int          uint32_least;  /* At least 32 bit                */

typedef float                 float32;
typedef double                float64;

#define PLATFORM_SUPPORT_SINT64_UINT64
#if defined(_WIN32) && (defined(_MSC_VER) || defined (RC_INVOKED) || defined(__MINGW32__) || defined(__MINGW64__))
  /* Visual Studio C++ compiler, Visual Studio resource compiler or Mingw-w64 on Windows platform */
  typedef signed long long    sint64;
  typedef unsigned long long  uint64;
#elif defined(__linux__) && (defined(__clang__) || defined(__GNUC__))
  /* clang or gcc compiler on Linux platform */
  typedef signed long         sint64;
  typedef unsigned long       uint64;
#else
  #error "Unknown compiler"
#endif

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#endif /* PLATFORM_TYPES_H */

/**********************************************************************************************************************
 *  END OF FILE: Platform_Types.h
 *********************************************************************************************************************/
