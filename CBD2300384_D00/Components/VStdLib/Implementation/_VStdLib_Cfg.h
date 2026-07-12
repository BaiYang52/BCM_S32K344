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
/*!        \file  VStdLib_Cfg.h
 *        \brief  Configuration of the generic Vector Standard Library (VStdLib_GenericAsr)
 *      \details  Template file that can be adapted by the user.
 *                This file contains the project specific configuration of the VStdLib. It also provides the possibility
 *                of overwriting the public APIs with project specific optimized implementations.
 *
 *********************************************************************************************************************/

#if !defined (VSTDLIB_CFG_H)
# define VSTDLIB_CFG_H

/**
 * Additional include files
 *
 * If necessary include additional files below (e.g. if VSTDLIB_USE_LIBRARY_FUNCTIONS is defined to STD_ON).
 */
/* # include ... */

/**********************************************************************************************************************
 *  General configuration
 *********************************************************************************************************************/

/**
 * VSTDLIB_USE_LIBRARY_FUNCTIONS
 *
 * If set to STD_ON the memory initialization, memory copy and memory comparison APIs are mapped to external functions
 * (e.g. compiler libraries or other implementations that are optimized for the target platform).
 * As the generic VStdLib implementation provides optimized routines only for 32bit platforms, this is recommended when
 * using MCUs with other architectures.
 *
 * Values:
 * STD_ON  - Use external memory functions - requires additional configuration, see below.
 * STD_OFF - Use generic memory functions provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_LIBRARY_FUNCTIONS STD_OFF

/**
 * VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION
 *
 * If set to STD_ON the memory copy with endianness swap API is mapped to an external function
 * (e.g. compiler libraries or other implementations that are optimized for the target platform).
 * As the generic VStdLib implementation provides optimized routines only for 32bit platforms, this is recommended when
 * using MCUs with other architectures.
 *
 * Values:
 * STD_ON  - Use external memory copy with endianness swap function - requires additional configuration, see below.
 * STD_OFF - Use generic memory copy with endianness swap function provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION STD_OFF

/**
 * VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION
 *
 * If set to STD_ON, the memory check API is mapped to an external function (e.g. compiler libraries
 * or other implementations that are optimized for the target platform).
 * As the generic VStdLib implementation provides optimized routines only for 32bit platforms, this is recommended when
 * using MCUs with other architectures.
 *
 * Values:
 * STD_ON  - Use external memory check function - requires additional configuration, see below.
 * STD_OFF - Use generic memory check function provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION STD_OFF

/**
 * VSTDLIB_RUNTIME_OPTIMIZATION
 *
 * If set to STD_ON optimized routines are used to increase the performance of the memory initialization, memory copy
 * and memory comparison APIs.
 * The setting of this define is only relevant if VSTDLIB_USE_LIBRARY_FUNCTIONS == STD_OFF or 
 * VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION == STD_OFF.
 *
 * Values:
 * STD_ON  - Use optimized routines for memory operations (runtime efficient, but increases code size)
 * STD_OFF - Use simple routines for memory operations (code efficient, but significantly increases runtime)
 *
 * Default:
 * STD_ON
 */
# define VSTDLIB_RUNTIME_OPTIMIZATION STD_ON

/**
 * VSTDLIB_USE_JUMPTABLES
 *
 * If set to STD_ON jump tables are used to increase the performance of the memory initialization and memory copy APIs.
 * The setting of this define is only relevant if VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON and
 * VSTDLIB_USE_LIBRARY_FUNCTIONS == STD_OFF or
 * VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION == STD_OFF.
 *
 * Values:
 * STD_ON  - Use jump tables for memory operations (runtime efficient in general, but may depend on compiler)
 * STD_OFF - Use loops for memory operations (code efficient, also use this if the compiler generates no efficient
 *           code for the jump tables)
 *
 * Default:
 * STD_ON
 */
# define VSTDLIB_USE_JUMPTABLES STD_ON

/**
 * VSTDLIB_DEV_ERROR_DETECT
 *
 * If set to STD_ON the development error detection is enabled. In this case the pointer arguments of all global
 * module functions are checked. If any NULL_PTR is passed, these functions return without performing any action.
 * Please note that this setting has no influence on MICROSAR SafeBSW.
 *
 * Values:
 * STD_ON  - Check function arguments
 * STD_OFF - Disable development error detection
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_DEV_ERROR_DETECT STD_OFF

/**
 * VSTDLIB_DEV_ERROR_REPORT
 *
 * If set to STD_ON the development error reporting is enabled. In this case Det_ReportError() is called if any
 * development error is detected.
 *
 * Values:
 * STD_ON  - Enable calls of Det_ReportError() - requires VSTDLIB_DEV_ERROR_DETECT also to be defined to STD_ON
 * STD_OFF - Disable development error reporting
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_DEV_ERROR_REPORT STD_OFF

/**
 * VSTDLIB_VERSION_INFO_API
 *
 * If set to STD_ON the API VStdLib_GetVersionInfo() is provided.
 *
 * Values:
 * STD_ON  - Provide the version info API
 * STD_OFF - Do not provide the version info API
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_VERSION_INFO_API STD_OFF

/**
 * VSTDLIB_DUMMY_STATEMENT
 *
 * Expression that is used for dummy statements to avoid compile warnings about unused identifiers.
 *
 * Example values:
 * (void)(v)
 * (v)=(v)
 * or leave the definition empty to disable the usage of dummy statements
 *
 * Default:
 * (void)(v)
 */
# define VSTDLIB_DUMMY_STATEMENT(v) (void)(v) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

/**
 * VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS
 *
 * If set to STD_ON all 8-bit bit search APIs are mapped to external functions (e.g. compiler libraries or
 * other implementations that are optimized for the target platform).
 *
 * Values:
 * STD_ON  - Use external bit search functions - requires additional configuration, see below.
 * STD_OFF - Use generic 8-bit bit search functions provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS STD_OFF

/**
 * VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS
 *
 * If set to STD_ON all 16-bit bit search APIs are mapped to external functions (e.g. compiler libraries or
 * other implementations that are optimized for the target platform).
 *
 * Values:
 * STD_ON  - Use external bit search functions - requires additional configuration, see below.
 * STD_OFF - Use generic 16-bit bit search functions provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS STD_OFF

/**
 * VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS
 *
 * If set to STD_ON all 32-bit bit search APIs are mapped to external functions (e.g. compiler libraries or
 * other implementations that are optimized for the target platform).
 *
 * Values:
 * STD_ON  - Use external bit search functions - requires additional configuration, see below.
 * STD_OFF - Use generic 32-bit bit search functions provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS STD_OFF

/**
 * VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS
 *
 * If set to STD_ON all 64-bit bit search APIs are mapped to external functions (e.g. compiler libraries or
 * other implementations that are optimized for the target platform).
 * The setting of this define is only allowed if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *
 * Values:
 * STD_ON  - Use external bit search functions - requires additional configuration, see below.
 * STD_OFF - Use generic 64-bit bit search functions provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS STD_OFF

/**
 * VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS
 *
 * If set to STD_ON all 16-bit deserialization and serialization APIs are mapped to external functions
 * (e.g. compiler libraries or other implementations that are optimized for the target platform).
 *
 * Values:
 * STD_ON  - Use external (de-)serialization functions - requires additional configuration, see below.
 * STD_OFF - Use generic 16-bit (de-)serialization functions provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS STD_OFF

/**
 * VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS
 *
 * If set to STD_ON all 32-bit deserialization and serialization APIs are mapped to external functions
 * (e.g. compiler libraries or other implementations that are optimized for the target platform).
 *
 * Values:
 * STD_ON  - Use external (de-)serialization functions - requires additional configuration, see below.
 * STD_OFF - Use generic 32-bit (de-)serialization functions provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS STD_OFF

/**
 * VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS
 *
 * If set to STD_ON all 64-bit deserialization and serialization APIs are mapped to external functions
 * (e.g. compiler libraries or other implementations that are optimized for the target platform).
 * The setting of this define is only allowed if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *
 * Values:
 * STD_ON  - Use external (de-)serialization functions - requires additional configuration, see below.
 * STD_OFF - Use generic 64-bit (de-)serialization functions provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS STD_OFF

/**
 * VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS
 *
 * If set to STD_ON all 16-bit endianness swap APIs are mapped to external functions (e.g. compiler libraries
 * or other implementations that are optimized for the target platform).
 *
 * Values:
 * STD_ON  - Use external endianness swap functions - requires additional configuration, see below.
 * STD_OFF - Use generic 16-bit endianness swap functions provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS STD_OFF

/**
 * VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS
 *
 * If set to STD_ON all 32-bit endianness swap APIs are mapped to external functions (e.g. compiler libraries
 * or other implementations that are optimized for the target platform).
 *
 * Values:
 * STD_ON  - Use external endianness swap functions - requires additional configuration, see below.
 * STD_OFF - Use generic 32-bit endianness swap functions provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS STD_OFF

/**
 * VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS
 *
 * If set to STD_ON all 64-bit endianness swap APIs are mapped to external functions (e.g. compiler libraries
 * or other implementations that are optimized for the target platform).
 * The setting of this define is only allowed if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *
 * Values:
 * STD_ON  - Use external endianness swap functions - requires additional configuration, see below.
 * STD_OFF - Use generic 64-bit endianness swap functions provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS STD_OFF

/**
 * VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS
 *
 * If set to STD_ON all bit copy sawtooth APIs are mapped to external functions (e.g. compiler libraries
 * or other implementations that are optimized for the target platform).
 *
 * Values:
 * STD_ON  - Use external bit copy functions with sawtooth bit numbering
 * STD_OFF - Use generic bit copy functions with sawtooth bit numbering provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS STD_OFF

/**
 * VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS
 *
 * If set to STD_ON all bit copy monotone APIs are mapped to external functions (e.g. compiler libraries
 * or other implementations that are optimized for the target platform).
 *
 * Values:
 * STD_ON  - Use external bit copy functions with monotone bit numbering
 * STD_OFF - Use generic bit copy functions with monotone bit numbering provided by VStdLib
 *
 * Default:
 * STD_OFF
 */
# define VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS STD_OFF

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_LIBRARY_FUNCTIONS == STD_ON
 *********************************************************************************************************************/

# if (VSTDLIB_USE_LIBRARY_FUNCTIONS == STD_ON)
/**
 * Memory library functions.
 *
 * If VSTDLIB_USE_LIBRARY_FUNCTIONS is set to STD_ON it is necessary to specify the memory initialization, memory
 * copy and memory comparison APIs by overwriting the definitions below. The error directive has to be removed.
 *
 * NOTE:
 * - If the external functionality is not able to handle more than 65535 bytes it is necessary to define
 *   VSTDLIB_SUPPORT_LARGE_DATA to STD_OFF.
 * - The specified functions must behave synchronously.
 * - It has to be ensured that the specified memory initialization and memory copy functions are able to copy from
 *   and to all memory locations independently of the pointer length.
 */
#  error "Verify the setting for VSTDLIB_SUPPORT_LARGE_DATA and fill the macros in this section with correct values!"

#  define VSTDLIB_SUPPORT_LARGE_DATA             STD_ON

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* Set nCnt bytes to zero at pDst (any alignment) */
#  define VStdLib_MemClr(pDst, nCnt)             memset((pDst), 0, (nCnt))

/* Set nCnt bytes to the character nPattern at pDst (any alignment) */
#  define VStdLib_MemSet(pDst, nPattern, nCnt)   memset((pDst), (nPattern), (nCnt))

/* Copy nCnt bytes from pSrc to pDst (any alignment) */
#  define VStdLib_MemCpy(pDst, pSrc, nCnt)       memcpy((pDst), (pSrc), (nCnt))

/* Copy nCnt 16bit blocks from pSrc to pDst (both pointers 16-bit aligned) */
#  define VStdLib_MemCpy16(pDst, pSrc, nCnt)     memcpy((pDst), (pSrc), ((nCnt)<<1))

/* Copy nCnt 32bit blocks from pSrc to pDst (both pointers 32-bit aligned) */
#  define VStdLib_MemCpy32(pDst, pSrc, nCnt)     memcpy((pDst), (pSrc), ((nCnt)<<2))

/*
 * Compares the two buffers for the specified number of bytes and returns whether they are equal or not.
 *
 * The memory comparison API has to return the datatype sint8 with the following values
 * < 0, if buffer1 contains a smaller value than buffer2.
 * = 0, if buffer1 and buffer2 are equal for the number of compared bytes.
 * > 0, if buffer1 contains a greater value than buffer2.
 *
 * Attention: The shown example is only possible, if memcmp() returns values which are in the range of datatype sint8!
 *
*/
#  define VStdLib_MemCmp(pBuf1, pBuf2, nCnt)     ((sint8)memcmp((pBuf1), (pBuf2), (nCnt)))

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* VSTDLIB_USE_LIBRARY_FUNCTIONS == STD_ON */

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION == STD_ON
 *********************************************************************************************************************/

# if (VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION == STD_ON)
/**
 * Memory copy with endianness swap API library function.
 *
 * If VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION is set to STD_ON it is necessary to specify the
 * memory copy with endianness swap API by overwriting the definition below. The error directive has to be removed.
 *
 * NOTE:
 * - If the external functionality is not able to handle more than 65535 bytes it is necessary to define
 *   VSTDLIB_SUPPORT_LARGE_DATA to STD_OFF.
 * - The specified function must behave synchronously.
 * - It has to be ensured that the specified function is able to copy from and to all memory locations independently
 *   of the pointer length.
 */
#  error "Verify the setting for VSTDLIB_SUPPORT_LARGE_DATA and fill the macro in this section with the correct value!"

#  if !defined (VSTDLIB_SUPPORT_LARGE_DATA)
#   define VSTDLIB_SUPPORT_LARGE_DATA         STD_ON
#  endif

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* 
 * Copies nCnt bytes from pSrc (any alignment) to pDst (integer type alignment) with swapped endianness according to
 * the integer type intType.
 *
 * Possible values for parameter inType are:
 * - 1 for integer type uint8/sint8
 * - 2 for integer type uint16/sint16
 * - 4 for integer type uint32/sint32
 * - 8 for integer type uint64/sint64
 */
#  define VStdLib_MemCpySwapEndian(pDst, pSrc, nCnt, intType)

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION == STD_ON */

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION == STD_ON
 *********************************************************************************************************************/

# if (VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION == STD_ON)
/**
 * Memory check library function.
 *
 * If VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION is set to STD_ON it is necessary to define the memory check function by overwriting the definition below.
 * The error directive has to be removed.
 *
 * NOTE:
 * - If the external functionality is not able to handle more than 65535 bytes it is necessary to define
 *   VSTDLIB_SUPPORT_LARGE_DATA to STD_OFF.
 * - The specified function must behave synchronously.
 */
#  error "Verify the setting for VSTDLIB_SUPPORT_LARGE_DATA and fill the macro in this section with the correct value!"

#  if !defined (VSTDLIB_SUPPORT_LARGE_DATA)
#   define VSTDLIB_SUPPORT_LARGE_DATA         STD_ON
#  endif

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* Compares nCnt bytes of memory starting at pBuf to the specified 8-bit nPattern. If the content at pBuf is equal to the nPattern, E_OK is returned. 
   Otherwise, the errorCode passed as argument is returned. */
#  define VStdLib_MemCheck(pBuf, nPattern, nCnt, errorCode)

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION == STD_ON */

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON
 *********************************************************************************************************************/

# if (VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON)
/**
 * Bit search library functions.
 *
 * If VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS is set to STD_ON it is necessary to specify 8-bit bit search APIs by
 * overwriting the definitions below. The error directive has to be removed.
 *
 * The 8-bit search APIs to get the highest/lowest bit position of a set/unset bit have to use the return value
 * VSTDLIB_BITPOS_NA, if the value contains no set/unset bit.
 *
 * The 8-bit search APIs to get the bit count of leading/trailing ones/zeros have to use the return value
 * VSTDLIB_BITCNT_NA, if the value contains no unset/set bit.
 *
 */
#  error "Fill the macros in this section with correct values!"

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* Returns the highest bit position of a set bit in the specified 8-bit value */
#  define VStdLib_GetHighestBitPosOne8(value)

/* Returns the highest bit position of an unset bit in the specified 8-bit value */
#  define VStdLib_GetHighestBitPosZero8(value)

/* Returns the lowest bit position of a set bit in the specified 8-bit value */
#  define VStdLib_GetLowestBitPosOne8(value)

/* Returns the lowest bit position of an unset bit in the specified 8-bit value */
#  define VStdLib_GetLowestBitPosZero8(value)

/* Returns the count of set bits preceding the highest unset bit in the specified 8-bit value */
#  define VStdLib_GetCountLeadingOnes8(value)

/* Returns the count of unset bits preceding the highest set bit in the specified 8-bit value */
#  define VStdLib_GetCountLeadingZeros8(value)

/* Returns the count of set bits following the lowest unset bit in the specified 8-bit value */
#  define VStdLib_GetCountTrailingOnes8(value)

/* Returns the count of unset bits following the lowest set bit in the specified 8-bit value */
#  define VStdLib_GetCountTrailingZeros8(value)

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON */

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON
 *********************************************************************************************************************/

# if (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON)
/**
 * Bit search library functions.
 *
 * If VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS is set to STD_ON it is necessary to specify 16-bit bit search APIs by
 * overwriting the definitions below. The error directive has to be removed.
 *
 * The 16-bit search APIs to get the highest/lowest bit position of a set/unset bit have to use the return value
 * VSTDLIB_BITPOS_NA, if the value contains no set/unset bit.
 *
 * The 16-bit search APIs to get the bit count of leading/trailing ones/zeros have to use the return value
 * VSTDLIB_BITCNT_NA, if the value contains no unset/set bit.
 *
 */
#  error "Fill the macros in this section with correct values!"

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* Returns the highest bit position of a set bit in the specified 16-bit value */
#  define VStdLib_GetHighestBitPosOne16(value)

/* Returns the highest bit position of an unset bit in the specified 16-bit value */
#  define VStdLib_GetHighestBitPosZero16(value)

/* Returns the lowest bit position of a set bit in the specified 16-bit value */
#  define VStdLib_GetLowestBitPosOne16(value)

/* Returns the lowest bit position of an unset bit in the specified 16-bit value */
#  define VStdLib_GetLowestBitPosZero16(value)

/* Returns the count of set bits preceding the highest unset bit in the specified 16-bit value */
#  define VStdLib_GetCountLeadingOnes16(value)

/* Returns the count of unset bits preceding the highest set bit in the specified 16-bit value */
#  define VStdLib_GetCountLeadingZeros16(value)

/* Returns the count of set bits following the lowest unset bit in the specified 16-bit value */
#  define VStdLib_GetCountTrailingOnes16(value)

/* Returns the count of unset bits following the lowest set bit in the specified 16-bit value */
#  define VStdLib_GetCountTrailingZeros16(value)

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON */

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON
 *********************************************************************************************************************/

# if (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON)
/**
 * Bit search library functions.
 *
 * If VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS is set to STD_ON it is necessary to specify 32-bit bit search APIs by
 * overwriting the definitions below. The error directive has to be removed.
 *
 * The 32-bit search APIs to get the highest/lowest bit position of a set/unset bit have to use the return value
 * VSTDLIB_BITPOS_NA, if the value contains no set/unset bit.
 *
 * The 32-bit search APIs to get the bit count of leading/trailing ones/zeros have to use the return value
 * VSTDLIB_BITCNT_NA, if the value contains no unset/set bit.
 *
 */
#  error "Fill the macros in this section with correct values!"

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* Returns the highest bit position of a set bit in the specified 32-bit value */
#  define VStdLib_GetHighestBitPosOne32(value)

/* Returns the highest bit position of an unset bit in the specified 32-bit value */
#  define VStdLib_GetHighestBitPosZero32(value)

/* Returns the lowest bit position of a set bit in the specified 32-bit value */
#  define VStdLib_GetLowestBitPosOne32(value)

/* Returns the lowest bit position of an unset bit in the specified 32-bit value */
#  define VStdLib_GetLowestBitPosZero32(value)

/* Returns the count of set bits preceding the highest unset bit in the specified 32-bit value */
#  define VStdLib_GetCountLeadingOnes32(value)

/* Returns the count of unset bits preceding the highest set bit in the specified 32-bit value */
#  define VStdLib_GetCountLeadingZeros32(value)

/* Returns the count of set bits following the lowest unset bit in the specified 32-bit value */
#  define VStdLib_GetCountTrailingOnes32(value)

/* Returns the count of unset bits following the lowest set bit in the specified 32-bit value */
#  define VStdLib_GetCountTrailingZeros32(value)

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON */

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON
 *********************************************************************************************************************/

# if ((defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON))
/**
 * Bit search library functions.
 *
 * If VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS is set to STD_ON it is necessary to specify 64-bit bit search APIs by
 * overwriting the definitions below. The error directive has to be removed.
 *
 * The 64-bit search APIs to get the highest/lowest bit position of a set/unset bit have to use the return value
 * VSTDLIB_BITPOS_NA, if the value contains no set/unset bit.
 *
 * The 64-bit search APIs to get the bit count of leading/trailing ones/zeros have to use the return value
 * VSTDLIB_BITCNT_NA, if the value contains no unset/set bit.
 *
 */
#  error "Fill the macros in this section with correct values!"

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* Returns the highest bit position of a set bit in the specified 64-bit value */
#  define VStdLib_GetHighestBitPosOne64(value)

/* Returns the highest bit position of an unset bit in the specified 64-bit value */
#  define VStdLib_GetHighestBitPosZero64(value)

/* Returns the lowest bit position of a set bit in the specified 64-bit value */
#  define VStdLib_GetLowestBitPosOne64(value)

/* Returns the lowest bit position of an unset bit in the specified 64-bit value */
#  define VStdLib_GetLowestBitPosZero64(value)

/* Returns the count of set bits preceding the highest unset bit in the specified 64-bit value */
#  define VStdLib_GetCountLeadingOnes64(value)

/* Returns the count of unset bits preceding the highest set bit in the specified 64-bit value */
#  define VStdLib_GetCountLeadingZeros64(value)

/* Returns the count of set bits following the lowest unset bit in the specified 64-bit value */
#  define VStdLib_GetCountTrailingOnes64(value)

/* Returns the count of unset bits following the lowest set bit in the specified 64-bit value */
#  define VStdLib_GetCountTrailingZeros64(value)

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* (defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON) */

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON
 *********************************************************************************************************************/

# if (VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON)
/**
 * (De-)serialization library functions.
 *
 * If VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS is set to STD_ON it is necessary to specify the 16-bit
 * deserialization and serialization APIs by overwriting the definitions below. The error directive has to be removed.
 *
 */
#  error "Fill the macros in this section with correct values!"

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* Converts the specified unsigned 16-bit value to an unsigned 8-bit array according to big endian */
#  define VStdLib_ConvertUint16ToUint8ArrayBigEndian(src, dst)

/* Converts the specified unsigned 16-bit value to an unsigned 8-bit array according to little endian */
#  define VStdLib_ConvertUint16ToUint8ArrayLittleEndian(src, dst)

/* Converts the specified signed 16-bit value to an unsigned 8-bit array according to big endian */
#  define VStdLib_ConvertSint16ToUint8ArrayBigEndian(src, dst)

/* Converts the specified signed 16-bit value to an unsigned 8-bit array according to little endian */
#  define VStdLib_ConvertSint16ToUint8ArrayLittleEndian(src, dst)

/* Converts the specified unsigned 8-bit array to an unsigned 16-bit value according to big endian */
#  define VStdLib_ConvertUint8ArrayToUint16BigEndian(src, dst)

/* Converts the specified unsigned 8-bit array to an unsigned 16-bit value according to little endian */
#  define VStdLib_ConvertUint8ArrayToUint16LittleEndian(src, dst)

/* Converts the specified unsigned 8-bit array to an signed 16-bit value according to big endian */
#  define VStdLib_ConvertUint8ArrayToSint16BigEndian(src, dst)

/* Converts the specified unsigned 8-bit array to an signed 16-bit value according to little endian */
#  define VStdLib_ConvertUint8ArrayToSint16LittleEndian(src, dst)

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON */

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON
 *********************************************************************************************************************/

# if (VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON)
/**
 * (De-)serialization library functions.
 *
 * If VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS is set to STD_ON it is necessary to specify the 32-bit
 * deserialization and serialization APIs by overwriting the definitions below. The error directive has to be removed.
 *
 */
#  error "Fill the macros in this section with correct values!"

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* Converts the specified unsigned 32-bit value to an unsigned 8-bit array according to big endian */
#  define VStdLib_ConvertUint32ToUint8ArrayBigEndian(src, dst)

/* Converts the specified unsigned 32-bit value to an unsigned 8-bit array according to little endian */
#  define VStdLib_ConvertUint32ToUint8ArrayLittleEndian(src, dst)

/* Converts the specified signed 32-bit value to an unsigned 8-bit array according to big endian */
#  define VStdLib_ConvertSint32ToUint8ArrayBigEndian(src, dst)

/* Converts the specified signed 32-bit value to an unsigned 8-bit array according to little endian */
#  define VStdLib_ConvertSint32ToUint8ArrayLittleEndian(src, dst)

/* Converts the specified unsigned 8-bit array to an unsigned 32-bit value according to big endian */
#  define VStdLib_ConvertUint8ArrayToUint32BigEndian(src, dst)

/* Converts the specified unsigned 8-bit array to an unsigned 32-bit value according to little endian */
#  define VStdLib_ConvertUint8ArrayToUint32LittleEndian(src, dst)

/* Converts the specified unsigned 8-bit array to an signed 32-bit value according to big endian */
#  define VStdLib_ConvertUint8ArrayToSint32BigEndian(src, dst)

/* Converts the specified unsigned 8-bit array to an signed 32-bit value according to little endian */
#  define VStdLib_ConvertUint8ArrayToSint32LittleEndian(src, dst)

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON */

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON
 *********************************************************************************************************************/

# if ((defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON))
/**
 * (De-)serialization library functions.
 *
 * If VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS is set to STD_ON it is necessary to specify the 64-bit
 * deserialization and serialization APIs by overwriting the definitions below. The error directive has to be removed.
 *
 */
#  error "Fill the macros in this section with correct values!"

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* Converts the specified unsigned 64-bit value to an unsigned 8-bit array according to big endian */
#  define VStdLib_ConvertUint64ToUint8ArrayBigEndian(src, dst)

/* Converts the specified unsigned 64-bit value to an unsigned 8-bit array according to little endian */
#  define VStdLib_ConvertUint64ToUint8ArrayLittleEndian(src, dst)

/* Converts the specified signed 64-bit value to an unsigned 8-bit array according to big endian */
#  define VStdLib_ConvertSint64ToUint8ArrayBigEndian(src, dst)

/* Converts the specified signed 64-bit value to an unsigned 8-bit array according to little endian */
#  define VStdLib_ConvertSint64ToUint8ArrayLittleEndian(src, dst)

/* Converts the specified unsigned 8-bit array to an unsigned 64-bit value according to big endian */
#  define VStdLib_ConvertUint8ArrayToUint64BigEndian(src, dst)

/* Converts the specified unsigned 8-bit array to an unsigned 64-bit value according to little endian */
#  define VStdLib_ConvertUint8ArrayToUint64LittleEndian(src, dst)

/* Converts the specified unsigned 8-bit array to an signed 64-bit value according to big endian */
#  define VStdLib_ConvertUint8ArrayToSint64BigEndian(src, dst)

/* Converts the specified unsigned 8-bit array to an signed 64-bit value according to little endian */
#  define VStdLib_ConvertUint8ArrayToSint64LittleEndian(src, dst)

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* (defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON) */

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON
 *********************************************************************************************************************/

# if (VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON)
/**
 * Endianness swap library functions.
 *
 * If VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS is set to STD_ON it is necessary to specify the 16-bit
 * endianness swap APIs by overwriting the definitions below. The error directive has to be removed.
 *
 */
#  error "Fill the macros in this section with correct values!"

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* Swaps the endianness of the specified unsigned 16-bit value */
#  define VStdLib_SwapEndianUint16(value)

/* Swaps the endianness of the specified signed 16-bit value */
#  define VStdLib_SwapEndianSint16(value)

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON */

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON
 *********************************************************************************************************************/

# if (VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON)
/**
 * Endianness swap library functions.
 *
 * If VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS is set to STD_ON it is necessary to specify the 32-bit
 * endianness swap APIs by overwriting the definitions below. The error directive has to be removed.
 *
 */
#  error "Fill the macros in this section with correct values!"

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* Swaps the endianness of the specified unsigned 32-bit value */
#  define VStdLib_SwapEndianUint32(value)

/* Swaps the endianness of the specified signed 32-bit value */
#  define VStdLib_SwapEndianSint32(value)

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON */

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON
 *********************************************************************************************************************/

# if ((defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON))
/**
 * Endianness swap library functions.
 *
 * If VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS is set to STD_ON it is necessary to specify the 64-bit
 * endianness swap APIs by overwriting the definitions below. The error directive has to be removed.
 *
 */
#  error "Fill the macros in this section with correct values!"

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* Swaps the endianness of the specified unsigned 64-bit value */
#  define VStdLib_SwapEndianUint64(value)

/* Swaps the endianness of the specified signed 64-bit value */
#  define VStdLib_SwapEndianSint64(value)

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* (defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON) */

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS == STD_ON
 *********************************************************************************************************************/

# if (VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS == STD_ON)
/**
 * Bitcopy sawtooth library functions.
 *
 * If VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS is set to STD_ON it is necessary to specify the Bitcopy
 * sawtooth APIs by overwriting the definitions below. The error directive has to be removed.
 *
 */
#  error "Fill the macros in this section with correct values!"

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* Copy nCnt bits from pSrc to pDst (any offset) with big data endianness 
 * Bit numbering of source and destination is sawtooth */
#  define VStdLib_BitCpySawBigToBigEndian(pDst, nDstBitOffsetSaw, pSrc, nSrcBitOffsetSaw, nCnt)

/* Copy nCnt bits from pSrc to pDst (any offset) with swapped data endianness (big to little) 
 * Bit numbering of source and destination is sawtooth */
#  define VStdLib_BitCpySawBigToLittleEndian(pDst, nDstBitOffsetSaw, pSrc, nSrcBitOffsetSaw, nCnt)

/* Copy nCnt bits from pSrc to pDst (any offset) with swapped data endianness (little to big)
 * Bit numbering of source and destination is sawtooth */
#  define VStdLib_BitCpySawLittleToBigEndian(pDst, nDstBitOffsetSaw, pSrc, nSrcBitOffsetSaw, nCnt)

/* Copy nCnt bits from pSrc to pDst (any offset) with little data endianness
 * Bit numbering of source and destination is sawtooth */
#  define VStdLib_BitCpySawLittleToLittleEndian(pDst, nDstBitOffsetSaw, pSrc, nSrcBitOffsetSaw, nCnt)

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* (VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS == STD_ON) */

/**********************************************************************************************************************
 *  Additional configuration if VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS == STD_ON
 *********************************************************************************************************************/

# if (VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS == STD_ON)
/**
 * Bitcopy sawtooth library functions.
 *
 * If VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS is set to STD_ON it is necessary to specify the Bitcopy
 * sawtooth APIs by overwriting the definitions below. The error directive has to be removed.
 *
 */
#  error "Fill the macros in this section with correct values!"

/* PRQA S 3453 FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

/* Copy nCnt bits from pSrc to pDst (any offset) with big data endianness
 * Bit numbering of source and destination is monotone */
#  define VStdLib_BitCpyMonBigToBigEndian(pDst, nDstBitOffsetMon, pSrc, nSrcBitOffsetMon, nCnt)

/* Copy nCnt bits from pSrc to pDst (any offset) with swapped data endianness (big to little) 
 * Bit numbering of source and destination is monotone */
#  define VStdLib_BitCpyMonBigToLittleEndian(pDst, nDstBitOffsetMon, pSrc, nSrcBitOffsetMon, nCnt)

/* Copy nCnt bits from pSrc to pDst (any offset) with swapped data endianness (little to big)
 * Bit numbering of source and destination is monotone */
#  define VStdLib_BitCpyMonLittleToBigEndian(pDst, nDstBitOffsetMon, pSrc, nSrcBitOffsetMon, nCnt)

/* Copy nCnt bits from pSrc to pDst (any offset) with little data endianness
 * Bit numbering of source and destination is monotone */
#  define VStdLib_BitCpyMonLittleToLittleEndian(pDst, nDstBitOffsetMon, pSrc, nSrcBitOffsetMon, nCnt)

/* PRQA L:FUNCTION_LIKE_MACRO */

# endif /* (VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS == STD_ON) */

/**********************************************************************************************************************
 * Compatibility for legacy Vector modules
 *********************************************************************************************************************/

# if defined(VSTD_ENABLE_INTCTRL_HANDLING) || defined(VSTD_ENABLE_DEFAULT_INTCTRL) || \
     defined(VSTD_ENABLE_OSEK_INTCTRL) || defined(VSTD_ENABLE_APPL_INTCTRL)
/**
 * Compatibility for legacy Vector modules.
 *
 * Legacy Vector modules that are used in a MICROSAR stack might rely on interrupt handling functionalities that are
 * no longer supported by VStdLib. Map relevant calls to functionalities that are provided by the MICROSAR stack or
 * application and remove the error directive. VStdSuspendAllInterrupts() and VStdResumeAllInterrupts() can be mapped
 * to OS services or an exclusive area that is provided by the BSW scheduler for example.
 */
#  error "VStdLib_GenericAsr does not support interrupt control handling, provide a mapping in this section!"

# endif

/**********************************************************************************************************************
 *  Do not modify anything below
 *********************************************************************************************************************/

# define VSTDLIB_CFG_MAJOR_VERSION     (3u)
# define VSTDLIB_CFG_MINOR_VERSION     (4u)

#endif /* VSTDLIB_CFG_H */

/**********************************************************************************************************************
 *  END OF FILE: VStdLib_Cfg.h
 *********************************************************************************************************************/
