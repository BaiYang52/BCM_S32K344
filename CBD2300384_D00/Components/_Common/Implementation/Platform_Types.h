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
 *  Description:  Provision of Platform Types for Arm32
 *
 *  -------------------------------------------------------------------------------------------------------------------
 *  MISRA VIOLATIONS
 *  -------------------------------------------------------------------------------------------------------------------
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id     Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2007-11-06  Jk                    Template creation
 *  01.00.01  2008-12-03  Ces                   Update version of AUTOSAR software specification document in template
 *  01.00.02  2010-01-07  Ht                    comment about handling of float 64 in case floating point type with size 8 
 *                                              byte is not supported by a platform/Compiler
 *  01.01.00  2011-03-04 visht                  support ASR 4.0R1
 *  01.02.00  2012-06-29 visht                  support ASR 4.0R3 ( no change )
 *  01.03.00  2012-08-23 visht                  version define corrected - replace PATCH by REVISION - (PLATFORM_AR_RELEASE_REVISION_VERSION)
 *  01.03.01  2012-11-05 visseu                 PLATFORM_MODULE_ID added as specified in "AUTOSAR_TR_BSWModuleList.pdf" (R4.0 Rev 3), component version corrected
 *  02.00.00  2015-03-13 vispl                  Setup for CommonAsr_Arm32
 *  02.00.01  2015-09-04 vispl                  Add suffix "u" in compliance with MISRA-C:2004 rule 10.6.
 *                                              AR4-667: Support sint64 and uint64
 *  02.01.00  2020-11-16 visbwa                 Removed AUTHOR IDENTITY, changed filter mechanism (Product instead of
 *                                              AutosarVersion)
 *  02.01.01  2020-11-17 visbwa                 Fix of PLATFORM_SW_MINOR_VERSION
 *********************************************************************************************************************/

#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
/* ##V_CFG_MANAGEMENT ##CQProject : CommonAsr_Arm32 CQComponent : Impl_PlatformTypes */
#define COMMONASR_ARM32_IMPL_PLATFORMTYPES_VERSION 0x0201
#define COMMONASR_ARM32_IMPL_PLATFORMTYPES_RELEASE_VERSION 0x01

#define PLATFORM_VENDOR_ID    30u    /* SREQ00015439, SREQ00015413 */
#define PLATFORM_MODULE_ID    199u   /* SREQ00015439, SREQ00015413 */

/* AUTOSAR Software Specification Document Version Information */

/* AUTOSAR release 4.0 R3 */
#define PLATFORM_AR_RELEASE_MAJOR_VERSION      (4u)
#define PLATFORM_AR_RELEASE_MINOR_VERSION      (0u)
#define PLATFORM_AR_RELEASE_REVISION_VERSION   (3u)

/* Component Version Information */
#define PLATFORM_SW_MAJOR_VERSION       (2u)
#define PLATFORM_SW_MINOR_VERSION       (1u)
#define PLATFORM_SW_PATCH_VERSION       (1u)

#define CPU_TYPE_8       8u
#define CPU_TYPE_16      16u
#define CPU_TYPE_32      32u

#define MSB_FIRST        0u  /* big endian bit ordering */
#define LSB_FIRST        1u  /* little endian bit ordering */

#define HIGH_BYTE_FIRST  0u  /* big endian byte ordering */
#define LOW_BYTE_FIRST   1u  /* little endian byte ordering */

#ifndef TRUE
  #define TRUE   1u
#endif

#ifndef FALSE
  #define FALSE  0u
#endif

#define CPU_TYPE            CPU_TYPE_32
#define CPU_BIT_ORDER       LSB_FIRST
#define CPU_BYTE_ORDER      LOW_BYTE_FIRST

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
typedef unsigned char       boolean;        /* for use with TRUE/FALSE        */

typedef signed char         sint8;          /*        -128 .. +127            */
typedef unsigned char       uint8;          /*           0 .. 255             */
typedef signed short        sint16;         /*      -32768 .. +32767          */
typedef unsigned short      uint16;         /*           0 .. 65535           */
typedef signed long         sint32;         /* -2147483648 .. +2147483647     */
typedef unsigned long       uint32;         /*           0 .. 4294967295      */
#define PLATFORM_SUPPORT_SINT64_UINT64
typedef signed long long    sint64;         /* \brief 64-bit unsigned integer */
typedef unsigned long long  uint64;
typedef unsigned long       uint8_least;    /* At least 8 bit                 */
typedef unsigned long       uint16_least;   /* At least 16 bit                */
typedef unsigned long       uint32_least;   /* At least 32 bit                */
typedef signed long         sint8_least;    /* At least 7 bit + 1 bit sign    */
typedef signed long         sint16_least;   /* At least 15 bit + 1 bit sign   */
typedef signed long         sint32_least;   /* At least 31 bit + 1 bit sign   */

typedef float               float32;
typedef double              float64;

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
