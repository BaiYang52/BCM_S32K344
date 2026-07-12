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
/*!        \file  vstdlib.c
 *        \brief  Implementation of the generic Vector Standard Library (VStdLib_GenericAsr)
 *      \details  This is the source file of the VStdLib. It contains the hardware independent implementation of the
 *                public APIs.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define VSTDLIB_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "vstdlib.h"

/**********************************************************************************************************************
 *  VERSION CHECK
 *********************************************************************************************************************/

/* Checks the version of the interface header file */
#if (  (VSTDLIB_SW_MAJOR_VERSION != (3u)) \
    || (VSTDLIB_SW_MINOR_VERSION != (7u)) \
    || (VSTDLIB_SW_PATCH_VERSION != (0u)) )
# error "Version numbers of vstdlib.c and vstdlib.h are inconsistent!"
#endif

/* Checks the version of the configuration header file */
#if (  (VSTDLIB_CFG_MAJOR_VERSION != (3u)) \
    || (VSTDLIB_CFG_MINOR_VERSION != (4u)) )
# error "Version numbers of vstdlib.c and VStdLib_Cfg.h are inconsistent!"
#endif

/**********************************************************************************************************************
 *  CONFIGURATION CHECK
 *********************************************************************************************************************/

#if !defined (VSTDLIB_USE_LIBRARY_FUNCTIONS)
# error "VSTDLIB_USE_LIBRARY_FUNCTIONS has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION)
# error "VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION)
# error "VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_RUNTIME_OPTIMIZATION)
# error "VSTDLIB_RUNTIME_OPTIMIZATION has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_USE_JUMPTABLES)
# error "VSTDLIB_USE_JUMPTABLES has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_DEV_ERROR_DETECT)
# error "VSTDLIB_DEV_ERROR_DETECT has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_DEV_ERROR_REPORT)
# error "VSTDLIB_DEV_ERROR_REPORT has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_VERSION_INFO_API)
# error "VSTDLIB_VERSION_INFO_API has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_DUMMY_STATEMENT)
# error "VSTDLIB_DUMMY_STATEMENT has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS)
# error "VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS)
# error "VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS)
# error "VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS)
# error "VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS)
# error "VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS)
# error "VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS)
# error "VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS)
# error "VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS)
# error "VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS has to be defined.. check VStdLib_Cfg.h!"
#endif

#if !defined (VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS)
# error "VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS has to be defined.. check VStdLib_Cfg.h!"
#endif

#if (!defined (PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON))
# error "The 64-bit bit search functions can only be overwritten, if PLATFORM_SUPPORT_SINT64_UINT64 is defined!"
#endif

#if (!defined (PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON))
# error "The 64-bit (De-)serialization functions can only be overwritten, if PLATFORM_SUPPORT_SINT64_UINT64 is defined!"
#endif

#if (!defined (PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON))
# error "The 64-bit endianness conversion functions can only be overwritten, if PLATFORM_SUPPORT_SINT64_UINT64 is defined!"
#endif

#if ((VSTDLIB_DEV_ERROR_REPORT == STD_ON) && (VSTDLIB_DEV_ERROR_DETECT != STD_ON))
# error "VSTDLIB_DEV_ERROR_REPORT defined to STD_ON requires VSTDLIB_DEV_ERROR_DETECT also to be defined to STD_ON!"
#endif

#if (VSTDLIB_USE_LIBRARY_FUNCTIONS == STD_ON)
# if (!defined (VStdLib_MemClr) || !defined (VStdLib_MemSet) || !defined (VStdLib_MemCpy) || \
      !defined (VStdLib_MemCpy16) || !defined (VStdLib_MemCpy32) || !defined (VStdLib_MemCmp))
#  error "Memory function API mapping has to be defined!"
# endif
#endif

#if (VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION == STD_ON)
# if (!defined (VStdLib_MemCpySwapEndian))
#  error "Memory copy with endianness swap API mapping has to be defined"
# endif
#endif

#if (VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION == STD_ON)
# if (!defined (VStdLib_MemCheck))
#  error "Memory check API mapping has to be defined"
# endif
#endif

#if (((VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON)) && \
     (VSTDLIB_SUPPORT_LARGE_DATA != STD_ON) && (VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON))
# error "VSTDLIB_RUNTIME_OPTIMIZATION must not be enabled if large data support is not given!"
#endif

#if (VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON)
# if (!defined (VStdLib_GetHighestBitPosOne8) || !defined (VStdLib_GetHighestBitPosZero8) || \
      !defined (VStdLib_GetLowestBitPosOne8) || !defined (VStdLib_GetLowestBitPosZero8) || \
      !defined (VStdLib_GetCountLeadingOnes8) || !defined (VStdLib_GetCountLeadingZeros8) || \
      !defined (VStdLib_GetCountTrailingOnes8) || !defined (VStdLib_GetCountTrailingZeros8))
#  error "Bit search API mapping for 8-bit values has to be defined!"
# endif
#endif

#if (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON)
# if (!defined (VStdLib_GetHighestBitPosOne16) || !defined (VStdLib_GetHighestBitPosZero16) || \
      !defined (VStdLib_GetLowestBitPosOne16) || !defined (VStdLib_GetLowestBitPosZero16) || \
      !defined (VStdLib_GetCountLeadingOnes16) || !defined (VStdLib_GetCountLeadingZeros16) || \
      !defined (VStdLib_GetCountTrailingOnes16) || !defined (VStdLib_GetCountTrailingZeros16))
#  error "Bit search API mapping for 16-bit values has to be defined!"
# endif
#endif

#if (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON)
# if (!defined (VStdLib_GetHighestBitPosOne32) || !defined (VStdLib_GetHighestBitPosZero32) || \
      !defined (VStdLib_GetLowestBitPosOne32) || !defined (VStdLib_GetLowestBitPosZero32) || \
      !defined (VStdLib_GetCountLeadingOnes32) || !defined (VStdLib_GetCountLeadingZeros32) || \
      !defined (VStdLib_GetCountTrailingOnes32) || !defined (VStdLib_GetCountTrailingZeros32))
#  error "Bit search API mapping for 32-bit values has to be defined!"
# endif
#endif

#if (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON)
# if (!defined (VStdLib_GetHighestBitPosOne64) || !defined (VStdLib_GetHighestBitPosZero64) || \
      !defined (VStdLib_GetLowestBitPosOne64) || !defined (VStdLib_GetLowestBitPosZero64) || \
      !defined (VStdLib_GetCountLeadingOnes64) || !defined (VStdLib_GetCountLeadingZeros64) || \
      !defined (VStdLib_GetCountTrailingOnes64) || !defined (VStdLib_GetCountTrailingZeros64))
#  error "Bit search API mapping for 64-bit values has to be defined!"
# endif
#endif

#if (VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON)
# if (!defined (VStdLib_ConvertUint16ToUint8ArrayBigEndian) || !defined (VStdLib_ConvertUint16ToUint8ArrayLittleEndian) || \
      !defined (VStdLib_ConvertSint16ToUint8ArrayBigEndian) || !defined (VStdLib_ConvertSint16ToUint8ArrayLittleEndian) || \
      !defined (VStdLib_ConvertUint8ArrayToUint16BigEndian) || !defined (VStdLib_ConvertUint8ArrayToUint16LittleEndian) || \
      !defined (VStdLib_ConvertUint8ArrayToSint16BigEndian) || !defined (VStdLib_ConvertUint8ArrayToSint16LittleEndian))
#  error "(De-)serialization API mapping for 16-bit values has to be defined!"
# endif
#endif

#if (VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON)
# if (!defined (VStdLib_ConvertUint32ToUint8ArrayBigEndian) || !defined (VStdLib_ConvertUint32ToUint8ArrayLittleEndian) || \
      !defined (VStdLib_ConvertSint32ToUint8ArrayBigEndian) || !defined (VStdLib_ConvertSint32ToUint8ArrayLittleEndian) || \
      !defined (VStdLib_ConvertUint8ArrayToUint32BigEndian) || !defined (VStdLib_ConvertUint8ArrayToUint32LittleEndian) || \
      !defined (VStdLib_ConvertUint8ArrayToSint32BigEndian) || !defined (VStdLib_ConvertUint8ArrayToSint32LittleEndian))
#  error "(De-)serialization API mapping for 32-bit values has to be defined!"
# endif
#endif

#if (VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON)
# if (!defined (VStdLib_ConvertUint64ToUint8ArrayBigEndian) || !defined (VStdLib_ConvertUint64ToUint8ArrayLittleEndian) || \
      !defined (VStdLib_ConvertSint64ToUint8ArrayBigEndian) || !defined (VStdLib_ConvertSint64ToUint8ArrayLittleEndian) || \
      !defined (VStdLib_ConvertUint8ArrayToUint64BigEndian) || !defined (VStdLib_ConvertUint8ArrayToUint64LittleEndian) || \
      !defined (VStdLib_ConvertUint8ArrayToSint64BigEndian) || !defined (VStdLib_ConvertUint8ArrayToSint64LittleEndian))
#  error "(De-)serialization API mapping for 64-bit values has to be defined!"
# endif
#endif

#if (VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON)
# if (!defined (VStdLib_SwapEndianUint16) || !defined (VStdLib_SwapEndianSint16))
#  error "Endianness swap API mapping for 16-bit values has to be defined"
# endif
#endif

#if (VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON)
# if (!defined (VStdLib_SwapEndianUint32) || !defined (VStdLib_SwapEndianSint32))
#  error "Endianness swap API mapping for 32-bit values has to be defined"
# endif
#endif

#if (VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON)
# if (!defined (VStdLib_SwapEndianUint64) || !defined (VStdLib_SwapEndianSint64))
#  error "Endianness swap API mapping for 64-bit values has to be defined"
# endif
#endif

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Masks internal parameters to ensure an upper limit of 3, respectively 31 */
#define VSTDLIB_MASK_3                       (0x03u)
#define VSTDLIB_MASK_31                      (0x1Fu)

/* Bitmask for lower three bits */
#define VSTDLIB_MASK_7                       (0x07u)

/* Bitmasks for search and endianness swap algorithms */
#define VSTDLIB_MASK_UPPER_NIBBLE            (0xF0u)
#define VSTDLIB_MASK_LOWER_NIBBLE            (0x0Fu)
#define VSTDLIB_MASK_UPPER_BYTE              (0xFF00u)
#define VSTDLIB_MASK_LOWER_BYTE              (0x00FFu)
#define VSTDLIB_MASK_UPPER_TWO_BYTES         (0xFFFF0000u)
#define VSTDLIB_MASK_LOWER_TWO_BYTES         (0x0000FFFFu)
#define VSTDLIB_MASK_UPPER_FOUR_BYTES        (0xFFFFFFFF00000000uLL)
#define VSTDLIB_MASK_LOWER_FOUR_BYTES        (0x00000000FFFFFFFFuLL)
#define VSTDLIB_MASK_ALLBITS_IN_ONE_BYTE     (0xFFu)
#define VSTDLIB_MASK_ALLBITS_IN_TWO_BYTES    (0xFFFFu)
#define VSTDLIB_MASK_ALLBITS_IN_FOUR_BYTES   (0xFFFFFFFFu)
#define VSTDLIB_MASK_ALLBITS_IN_EIGHT_BYTES  (0xFFFFFFFFFFFFFFFFuLL)

/* Return values for VStdLib_Loc_GetMemCmpNeqResult(), VStdLib_MemCmp(), VStdLib_Loc_MemCmp_8(),
   VStdLib_Loc_MemCmp_32_Aligned(), VStdLib_Loc_MemCmp_32_Unaligned() */
#define VSTDLIB_MEM_CMP_RES_EQ               (0)
#define VSTDLIB_MEM_CMP_RES_G                (1)
#define VSTDLIB_MEM_CMP_RES_L                (-1)

/* Not each Platform_Types.h defines CPU_TYPE_64 */
#if !defined (CPU_TYPE_64)
# define CPU_TYPE_64                         (64u)
#endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/* Returns the number of bytes (0 - 3) that are missing to the next 32-bit boundary
   Note: VSTDLIB_MASK_7 required to mask lower three bits, which ensures that macro returns 0 for 32-bit aligned pointer.
*/
#if (CPU_TYPE == CPU_TYPE_64)
/* ESCAN00108975: Compiler warning: 'type cast': pointer truncation from 'uint8 *' to 'uint32_least'
   ESCAN00109680: Compiler warning: Conversion from 'uint8 *' to 'uint64' is sign-extended
*/
# define VSTDLIB_ALIGN_OFFSET(ptr) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */ \
  (uint8_least)((4u - ((uint64)(ptr) & VSTDLIB_MASK_7)) & VSTDLIB_MASK_3)
#else
# define VSTDLIB_ALIGN_OFFSET(ptr) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */ \
  (uint8_least)((4u - ((uint32_least)(ptr) & VSTDLIB_MASK_7)) & VSTDLIB_MASK_3)
#endif

#if ((VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || \
     (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON))

/* Returns the bit position of a set bit in the specified nibble by using a lookup table */
# define VStdLib_Loc_GetHighestBitPosOneInNibble(value) VStdLib_HighestBitPosOneInNibble[(value)]
# define VStdLib_Loc_GetLowestBitPosOneInNibble(value)  VStdLib_LowestBitPosOneInNibble[(value)]

#endif /* (VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) ||
          (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) */

/* Converts a byte array (src) to the integer type value (dst) in the swapped endianness.
   The endianness swap is implemented by using the opposite service function to the current platform endianness.
*/
#if (VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON)
# if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
#  define VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(src, dst) VStdLib_ConvertUint8ArrayToUint16LittleEndian((src), (dst))
#  define VStdLib_Loc_ConvertUint8ArrayToUint32SwapEndian(src, dst) VStdLib_ConvertUint8ArrayToUint32LittleEndian((src), (dst))
#  if defined (PLATFORM_SUPPORT_SINT64_UINT64)
#   define VStdLib_Loc_ConvertUint8ArrayToUint64SwapEndian(src, dst) VStdLib_ConvertUint8ArrayToUint64LittleEndian((src), (dst))
#  endif
# else /* (CPU_BYTE_ORDER == LOW_BYTE_FIRST */
#  define VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(src, dst) VStdLib_ConvertUint8ArrayToUint16BigEndian((src), (dst))
#  define VStdLib_Loc_ConvertUint8ArrayToUint32SwapEndian(src, dst) VStdLib_ConvertUint8ArrayToUint32BigEndian((src), (dst))
#  if defined (PLATFORM_SUPPORT_SINT64_UINT64)
#   define VStdLib_Loc_ConvertUint8ArrayToUint64SwapEndian(src, dst) VStdLib_ConvertUint8ArrayToUint64BigEndian((src), (dst))
#  endif
# endif /* CPU_BYTE_ORDER */
#endif /* VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON */

/**********************************************************************************************************************
 *  LOCAL TYPES AND STRUCTURES
 *********************************************************************************************************************/

/* Module specific definition of LOCAL */
#if !defined (VSTDLIB_LOCAL)
# define VSTDLIB_LOCAL  static
#endif

#if ((VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON) || \
     (VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON))

/* Struct for iterations in source and destination for bit copy algorithm */
typedef struct
{
  VStdLib_CntType startByte;
  VStdLib_CntType lastByte;
  uint8 bitOffsetStartByte;
  uint8 bitOffsetEndByte;
} VStdLib_BitCpyIterType;

#endif /* (VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON) ||
          (VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON) */

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/
#if ((VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || \
     (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON))

# define VSTDLIB_START_SEC_CONST_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* Lookup table of nibble to bit position of highest set bit in nibble */
VSTDLIB_LOCAL CONST(uint8, VSTDLIB_CONST) VStdLib_HighestBitPosOneInNibble[16u] = /* PRQA S 3218 */ /* MD_VStdLib_FileScope */
{
  /* Bit position     / Nibble value */
  VSTDLIB_BITPOS_NA,  /* 0000 */ /* not possible, is catched directly via API */
  0u,                 /* 0001 */
  1u,                 /* 0010 */
  1u,                 /* 0011 */
  2u,                 /* 0100 */
  2u,                 /* 0101 */
  2u,                 /* 0110 */
  2u,                 /* 0111 */
  3u,                 /* 1000 */
  3u,                 /* 1001 */
  3u,                 /* 1010 */
  3u,                 /* 1011 */
  3u,                 /* 1100 */
  3u,                 /* 1101 */
  3u,                 /* 1110 */
  3u                  /* 1111 */
};

/* Lookup table of nibble to bit position of lowest set bit in nibble */
VSTDLIB_LOCAL CONST(uint8, VSTDLIB_CONST) VStdLib_LowestBitPosOneInNibble[16u] = /* PRQA S 3218 */ /* MD_VStdLib_FileScope */
{
  /* Bit position     / Nibble value */
  VSTDLIB_BITPOS_NA,  /* 0000 */ /* not possible, is catched directly via API */
  0u,                 /* 0001 */
  1u,                 /* 0010 */
  0u,                 /* 0011 */
  2u,                 /* 0100 */
  0u,                 /* 0101 */
  1u,                 /* 0110 */
  0u,                 /* 0111 */
  3u,                 /* 1000 */
  0u,                 /* 1001 */
  1u,                 /* 1010 */
  0u,                 /* 1011 */
  2u,                 /* 1100 */
  0u,                 /* 1101 */
  1u,                 /* 1110 */
  0u                  /* 1111 */
};

# define VSTDLIB_STOP_SEC_CONST_8BIT
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /*(VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) ||
         (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

#define VSTDLIB_START_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#if (VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_Loc_GetMemCmpNeqResult()
 *********************************************************************************************************************/
/*! \brief         Returns whether the specified buffer1 value is greater or smaller than the specified buffer2 value.
 *  \details       -
 *  \param[in]     buf1Value      Buffer1 value for comparison.
 *  \param[in]     buf2Value      Buffer2 value for comparison.
 *  \return        < 0            Buffer1 value is greater than buffer2 value.
 *  \return        > 0            Buffer1 value is smaller than buffer2 value.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           The parameters 'buf1Value' and 'buf2Value' have to contain unequal values.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(sint8, VSTDLIB_CODE) VStdLib_Loc_GetMemCmpNeqResult(uint32 buf1Value, uint32 buf2Value);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCmp_8()
 *********************************************************************************************************************/
/*! \brief         Compares two buffers byte-wise for the specified number of bytes and returns whether they are equal
 *                 or not (any alignment).
 *  \details       -
 *  \param[in]     pBuf1_8        8-bit pointer to first buffer for comparison.
 *  \param[in]     pBuf2_8        8-bit pointer to second buffer for comparison.
 *  \param[in]     nCnt           Number of bytes to compare, pBuf1_8 and pBuf2_8 must be valid for this amount.
 *  \return        < 0            The first buffer contains a smaller value than the second buffer.
 *  \return        0              The two buffers are equal for the number of compared bytes.
 *  \return        > 0            The first buffer contains a greater value than the second buffer.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           The parameters 'pBuf1_8' and 'pBuf2_8' have to point to valid memory areas. Each memory area has to
 *                 be valid for at least the byte count defined in parameter 'nCnt'.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(sint8, VSTDLIB_CODE) VStdLib_Loc_MemCmp_8(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf1_8,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf2_8,
  uint32_least nCnt);

# if (VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON)

/**********************************************************************************************************************
 *  VStdLib_Loc_MemSet_8_3()
 *********************************************************************************************************************/
/*! \brief         Initializes up to 3 bytes at pDst to the character nPattern (any alignment).
 *  \details       Depending on VSTDLIB_USE_JUMPTABLES either with a jumptable or a for loop.
 *  \param[out]    pDst8         8-bit pointer to the memory location to be initialized.
 *  \param[in]     nPattern      The character to be used to initialize the memory.
 *  \param[in]     nCnt          Number of bytes to initialize, pDst must be valid for this amount.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pDst8' and 'nCnt' have to define a valid memory area.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemSet_8_3(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  uint8 nPattern,
  uint8_least nCnt);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemSet_8_31()
 *********************************************************************************************************************/
/*! \brief         Initializes up to 31 bytes at pDst to the character nPattern (any alignment).
 *  \details       Depending on VSTDLIB_USE_JUMPTABLES either with a jumptable or a for loop.
 *  \param[out]    pDst8         8-bit pointer to the memory location to be initialized.
 *  \param[in]     nPattern      The character to be used to initialize the memory.
 *  \param[in]     nCnt          Number of bytes to initialize, pDst must be valid for this amount.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pDst8' and 'nCnt' have to define a valid memory area.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemSet_8_31(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  uint8 nPattern,
  uint8_least nCnt);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemSet_32_Aligned()
 *********************************************************************************************************************/
/*! \brief         Initializes memory at pDst to the character nPattern (32-bit aligned).
 *  \details       With a for loop, the tail depending on VSTDLIB_USE_JUMPTABLES either with a jumptable or a for loop.
 *  \param[out]    pDst32        32-bit pointer to the memory location to be initialized.
 *  \param[in]     nPattern      The character to be used to initialize the memory.
 *  \param[in]     nCnt          Number of bytes to initialize, pDst must be valid for this amount.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pDst32' and 'nCnt' have to define a valid memory area.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemSet_32_Aligned(
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32,
  uint8 nPattern,
  uint32_least nCnt);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCpy_8_3()
 *********************************************************************************************************************/
/*! \brief         Copies up to 3 bytes from pSrc to pDst (any alignment).
 *  \details       Depending on VSTDLIB_USE_JUMPTABLES either with a jumptable or a for loop.
 *  \param[out]    pDst8         8-bit pointer to the memory location to copy to.
 *  \param[in]     pSrc8         8-bit pointer to the memory location to copy from.
 *  \param[in]     nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pDst8' and 'nCnt' have to define a valid memory area.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpy_8_3(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  uint8_least nCnt);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCpy_8_31()
 *********************************************************************************************************************/
/*! \brief         Copies up to 31 bytes from pSrc to pDst (any alignment).
 *  \details       Depending on VSTDLIB_USE_JUMPTABLES either with a jumptable or a for loop.
 *  \param[out]    pDst8         8-bit pointer to the memory location to copy to.
 *  \param[in]     pSrc8         8-bit pointer to the memory location to copy from.
 *  \param[in]     nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pDst8' and 'nCnt' have to define a valid memory area.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpy_8_31(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  uint8_least nCnt);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCpy_32_Aligned()
 *********************************************************************************************************************/
/*! \brief         Copies 32-bit aligned data from pSrc to pDst.
 *  \details       With a for loop, the tail depending on VSTDLIB_USE_JUMPTABLES either with a jumptable or a for loop.
 *  \param[out]    pDst32        32-bit pointer to the memory location to copy to.
 *  \param[in]     pSrc32        32-bit pointer to the memory location to copy from.
 *  \param[in]     nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pDst32' and 'nCnt' have to define a valid memory area.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpy_32_Aligned(
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc32,
  uint32_least nCnt);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCpy_8_PartialWord()
 *********************************************************************************************************************/
/*! \brief         Copies a partial word of 1 to 3 bytes from pSrc to pDst to reach a 32-bit boundary (any alignment).
 *  \details       Depending on VSTDLIB_USE_JUMPTABLES either with a jumptable or a for loop.
 *  \param[out]    pDst8         8-bit pointer to the memory location to copy to.
 *  \param[in]     nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in,out] pPrev         Word to copy from (is modified and used by caller afterwards).
 *  \param[in,out] pDPos         Index for pDst8 to copy to (is modified and used by caller afterwards).
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pDst8' and 'nCnt' have to define a valid memory area.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpy_8_PartialWord(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  uint8_least nCnt,
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pPrev,
  P2VAR(uint32_least, AUTOMATIC, VSTDLIB_VAR_FAR) pDPos);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCpy_32_Unaligned()
 *********************************************************************************************************************/
/*! \brief         Copies data from 32-bit aligned pSrc to unaligned pDst.
 *  \details       With a for loop using 32-bit accesses, the head and tail depending on VSTDLIB_USE_JUMPTABLES either
                   with a jumptable or a for loop.
 *  \param[out]    pDst8         8-bit pointer to the memory location to copy to.
 *  \param[in]     pSrc32        32-bit pointer to the memory location to copy from.
 *  \param[in]     nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in]     nDstOffset    pDst offset to the next 32-bit boundary.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pDst8' and 'nCnt' have to define a valid memory area.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpy_32_Unaligned(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc32,
  uint32_least nCnt,
  uint8_least nDstOffset);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCmp_32_Aligned()
 *********************************************************************************************************************/
/*! \brief         Compares two 32-bit aligned buffers for the specified number of bytes and returns whether they are
 *                 equal or not.
 *  \details       The buffer values are compared 4-bytes-wise until the number of not compared bytes is smaller than 4.
 *                 The remaining bytes are compared byte-wise using VStdLib_Loc_MemCmp_8().
 *  \param[in]     pBuf1_32       32-bit pointer to first buffer for comparison.
 *  \param[in]     pBuf2_32       32-bit pointer to second buffer for comparison.
 *  \param[in]     nCnt           Number of bytes to compare, pBuf1_32 and pBuf2_32 must be valid for this amount.
 *  \return        < 0            The first buffer contains a smaller value than the second buffer.
 *  \return        0              The two buffers are equal for the number of compared bytes.
 *  \return        > 0            The first buffer contains a greater value than the second buffer.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pBuf1_32' and 'pBuf2_32' have to point to valid memory areas. Each memory area has
 *                 to be valid for at least the byte count defined in parameter 'nCnt'.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(sint8, VSTDLIB_CODE) VStdLib_Loc_MemCmp_32_Aligned(
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf1_32,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf2_32,
  uint32_least nCnt);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCmp_32_Unaligned()
 *********************************************************************************************************************/
/*! \brief         Compares a 32-bit aligned buffer with an unaligned buffer for the specified number of bytes and
 *                 returns whether they are equal or not.
 *  \details       The buffer values are compared 4-bytes-wise by correcting the byte displacement until the number of
 *                 not compared bytes is smaller than 4. The remaining bytes are compared byte-wise using
 *                 VStdLib_Loc_MemCmp_8().
 *  \param[in]     pBuf1_32       32-bit pointer to first buffer for comparison.
 *  \param[in]     pBuf2_8        8-bit pointer to second buffer for comparison.
 *  \param[in]     nCnt           Number of bytes to compare, pBuf1_32 and pBuf2_8 must be valid for this amount.
 *  \param[in]     nBuf2Offset    pBuf2_8 offset to the next 32-bit boundary.
 *  \return        < 0            The first buffer contains a smaller value than the second buffer.
 *  \return        0              The two buffers are equal for the number of compared bytes.
 *  \return        > 0            The first buffer contains a greater value than the second buffer.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pBuf1_32' and 'pBuf2_8' have to point to valid memory areas. Each memory area has
 *                 to be valid for at least the byte count defined in parameter 'nCnt'.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(sint8, VSTDLIB_CODE) VStdLib_Loc_MemCmp_32_Unaligned(
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf1_32,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf2_8,
  uint32_least nCnt,
  uint8_least nBuf2Offset);

# endif /* VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON */
#endif /* VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON */

#if (VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON)
# if (VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON)

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCpySwapEndian_IntType_32()
 *********************************************************************************************************************/
/*! \brief         Copies and swaps up to 32 bytes from pSrc8 (any alignment) to pDst (intType alignment) according to
 *                 the integer type intType.
 *  \details       Depending on VSTDLIB_USE_JUMPTABLES either with a jumptable or a for loop.
 *  \param[out]    pDst          Pointer to the memory location to copy to.
 *  \param[in]     pSrc8         8-bit pointer to the memory location to copy from.
 *  \param[in]     nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in]     intType       Integer type with which the memory locations are interpreted
 *                               (VSTDLIB_INTTYPE_[16, 32, 64]).
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *                 integer type size (parameter 'intType').
 *                 The parameter 'pDst' is aligned according to the integer type 'intType'.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndian_IntType_32(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  uint8_least nCnt,
  uint8 intType);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCpySwapEndian_Int16_32()
 *********************************************************************************************************************/
/*! \brief         Copies and swaps up to 32 bytes from pSrc8 (any alignment) to pDst16 (16-bit alignment) according to
 *                 the integer type VSTDLIB_INTTYPE_16.
 *  \details       Depending on VSTDLIB_USE_JUMPTABLES either with a jumptable or a for loop.
 *  \param[out]    pDst16        16-bit pointer to the memory location to copy to.
 *  \param[in]     pSrc8         8-bit pointer to the memory location to copy from.
 *  \param[in]     nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pDst16' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *                 integer type VSTDLIB_INTTYPE_16 size.
 *                 The parameter 'pDst16' is 16-bit aligned.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndian_Int16_32(
  P2VAR(uint16, AUTOMATIC, VSTDLIB_VAR_FAR) pDst16,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  uint8_least nCnt);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCpySwapEndian_Int32_32()
 *********************************************************************************************************************/
/*! \brief         Copies and swaps up to 32 bytes from pSrc8 (any alignment) to pDst32 (32-bit alignment) according to
 *                 the integer type VSTDLIB_INTTYPE_32.
 *  \details       Depending on VSTDLIB_USE_JUMPTABLES either with a jumptable or a for loop.
 *  \param[out]    pDst32        32-bit pointer to the memory location to copy to.
 *  \param[in]     pSrc8         8-bit pointer to the memory location to copy from.
 *  \param[in]     nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pDst32' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *                 integer type VSTDLIB_INTTYPE_32 size.
 *                 The parameter 'pDst32' is 32-bit aligned.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndian_Int32_32(
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  uint8_least nCnt);

#  if defined (PLATFORM_SUPPORT_SINT64_UINT64)

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCpySwapEndian_Int64_32()
 *********************************************************************************************************************/
/*! \brief         Copies and swaps up to 32 bytes from pSrc8 (any alignment) to pDst64 (64-bit alignment) according to
 *                 the integer type VSTDLIB_INTTYPE_64.
 *  \details       Depending on VSTDLIB_USE_JUMPTABLES either with a jumptable or a for loop.
 *  \param[out]    pDst64        64-bit pointer to the memory location to copy to.
 *  \param[in]     pSrc8         8-bit pointer to the memory location to copy from.
 *  \param[in]     nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *                 && defined PLATFORM_SUPPORT_SINT64_UINT64
 *  \pre           The parameters 'pDst64' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *                 integer type VSTDLIB_INTTYPE_64 size.
 *                 The parameter 'pDst64' is 64-bit aligned.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndian_Int64_32(
  P2VAR(uint64, AUTOMATIC, VSTDLIB_VAR_FAR) pDst64,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  uint8_least nCnt);

#  endif /* defined PLATFORM_SUPPORT_SINT64_UINT64 */

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCpySwapEndian_32_Aligned()
 *********************************************************************************************************************/
/*! \brief         Copies and swaps 32-bit aligned data from pSrc32 to pDst32 according to the integer type intType.
 *  \details       With a for loop. The tail depending on VSTDLIB_USE_JUMPTABLES either with a jumptable (32-bit access)
 *                 or a for loop (integer type dependent access).
 *  \param[out]    pDst32        32-bit pointer to the memory location to copy to.
 *  \param[in]     pSrc32        32-bit pointer to the memory location to copy from.
 *  \param[in]     nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in]     intType       Integer type with which the memory locations are interpreted
 *                               (VSTDLIB_INTTYPE_[16, 32, 64]).
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pDst32' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *                 integer type size (parameter 'intType').
 *                 The parameter 'pDst32' is 32-bit aligned and aligned according to the integer type 'intType'.
 *                 The parameter 'pSrc32' is 32-bit aligned.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndian_32_Aligned(
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc32,
  uint32_least nCnt,
  uint8 intType);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCpySwapEndianInt16_32_Aligned()
 *********************************************************************************************************************/
/*! \brief         Copies and swaps 32-bit aligned data from pSrc32 to pDst32 according to the integer type
 *                 VSTDLIB_INTTYPE_16.
 *  \details       With a for loop. The tail depending on VSTDLIB_USE_JUMPTABLES either with a jumptable (32-bit access)
 *                 or a for loop (16-bit access).
 *  \param[out]    pDst32        32-bit pointer to the memory location to copy to.
 *  \param[in]     pSrc32        32-bit pointer to the memory location to copy from.
 *  \param[in]     nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pDst32' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *                 integer type VSTDLIB_INTTYPE_16 size.
 *                 The parameter 'pDst32' is 16-bit and 32-bit aligned.
 *                 The parameter 'pSrc32' is 32-bit aligned.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndianInt16_32_Aligned(
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc32,
  uint32_least nCnt);

/**********************************************************************************************************************
 *  VStdLib_Loc_SwapEndianInt16_32()
 *********************************************************************************************************************/
/*! \brief         Returns two 16-bit values with a swapped endianness in a 32-bit value.
 *  \details       -
 *  \param[in]     src32       32-bit value containing two 16-bit values.
 *  \return        32-bit value containing the two 16-bit values with a swapped endianness.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint32, VSTDLIB_CODE) VStdLib_Loc_SwapEndianInt16_32(uint32 src32);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCpySwapEndianInt32_32_Aligned()
 *********************************************************************************************************************/
/*! \brief         Copies and swaps 32-bit aligned data from pSrc32 to pDst32 according to the integer type
 *                 VSTDLIB_INTTYPE_32.
 *  \details       With a for loop. The tail depending on VSTDLIB_USE_JUMPTABLES either with a jumptable (32-bit access)
 *                 or a for loop (32-bit access).
 *  \param[out]    pDst32        32-bit pointer to the memory location to copy to.
 *  \param[in]     pSrc32        32-bit pointer to the memory location to copy from.
 *  \param[in]     nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pDst32' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *                 integer type VSTDLIB_INTTYPE_32 size.
 *                 The parameter 'pDst32' is 32-bit aligned.
 *                 The parameter 'pSrc32' is 32-bit aligned.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndianInt32_32_Aligned(
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc32,
  uint32_least nCnt);

#  if defined (PLATFORM_SUPPORT_SINT64_UINT64)

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCpySwapEndianInt64_32_Aligned()
 *********************************************************************************************************************/
/*! \brief         Copies and swaps 32-bit aligned data from pSrc32 to pDst32 according to the integer type
 *                 VSTDLIB_INTTYPE_64.
 *  \details       With a for loop. The tail depending on VSTDLIB_USE_JUMPTABLES either with a jumptable (32-bit access)
 *                 or a for loop (64-bit access).
 *  \param[out]    pDst32        32-bit pointer to the memory location to copy to.
 *  \param[in]     pSrc32        32-bit pointer to the memory location to copy from.
 *  \param[in]     nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *                 && defined PLATFORM_SUPPORT_SINT64_UINT64
 *  \pre           The parameters 'pDst32' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *                 integer type VSTDLIB_INTTYPE_64 size.
 *                 The parameter 'pDst32' is 32-bit and 64-bit aligned.
 *                 The parameter 'pSrc32' is 32-bit aligned.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndianInt64_32_Aligned(
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc32,
  uint32_least nCnt);

#  endif /* defined PLATFORM_SUPPORT_SINT64_UINT64 */
# endif /* VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON */

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCpySwapEndian()
 *********************************************************************************************************************/
/*! \brief         Copies and swaps nCnt bytes from pSrc8 (any alignment) to pDst (intType alignment) according to
 *                 the integer type intType.
 *  \details       -
 *  \param[out]    pDst          Pointer to the memory location to copy to.
 *  \param[in]     pSrc8         8-bit pointer to the memory location to copy from.
 *  \param[in]     nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in]     intType       Integer type with which the memory locations are interpreted
 *                               (VSTDLIB_INTTYPE_[16, 32, 64]).
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON
 *  \pre           The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *                 integer type size (parameter 'intType').
 *                 The parameter 'pDst' is aligned according to the integer type 'intType'.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  uint32_least nCnt,
  uint8 intType);

#endif /* VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON */

/**********************************************************************************************************************
 *   VStdLib_Loc_BitCpyConvertBitCounting()
 *********************************************************************************************************************/
/*! \brief         Converts sawtooth to monotone bit position numbering and vice versa.
 *  \details       Is used by _s BitCpy and normal BitCpy APIs. The conversion from sawtooth to monotone bit position
 *                 numbering in case of big endian data simplifies the calculation of the last byte to copy to or
 *                 from and the calculation of its bit offset. Same applies for the conversion of monotone to sawtooth
 *                 bit position numbering in case of little endian data.
 *  \param[in]     bitPos      Bit position numbered as sawtooth or monotone.
 *  \return        Bit position with converted bit position numbering.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *                 VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(VStdLib_CntType, VSTDLIB_CODE) VStdLib_Loc_BitCpyConvertBitCounting(VStdLib_CntType bitPos);

/**********************************************************************************************************************
 *   VStdLib_Loc_BitCpyGetLastIterByte()
 *********************************************************************************************************************/
/*! \brief         Returns last iteration byte.
 *  \details       Is used by _s     BitCpy and normal BitCpy APIs to get the last iteration byte.
 *  \param[in]     lengthInBits      Sum of bits to be copied plus the offset.
 *  \return        Last iteration byte.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *                 VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(VStdLib_CntType, VSTDLIB_CODE) VStdLib_Loc_BitCpyGetLastIterByte(
  VStdLib_CntType lengthInBits);

#if ((VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON) || \
     (VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON))

/**********************************************************************************************************************
 *   VStdLib_Loc_BitCpyIter()
 *********************************************************************************************************************/
/*! \brief         Sets all relevant interation parameters to iterate over a memory location.
 *  \details       -
 *  \param[in]     pIter           Pointer to the iteration structure of a memory location.
 *  \param[in]     nBitOffset      Bit offset numbered as sawtooth or monotone.
 *  \param[in]     nCnt            Number of bits to copy.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *                 VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyIter(
  P2VAR(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) pIter,
  VStdLib_CntType nBitOffset,
  VStdLib_CntType nCnt);

/**********************************************************************************************************************
 *   VStdLib_Loc_BitCpyGetOffsetInBitsEndByte()
 *********************************************************************************************************************/
/*! \brief         Returns bit offset in the last byte.
 *  \details       Converts bit numbering from sawtooth to monotone and vice versa. It is used to shift the bit mask
 *                 for the last destination byte and to determine the source byte shift if the memory locations have
 *                 different data endianness.
 *  \param[in]     lengthInBits  Sum of bits to be copied plus the bit offset.
 *  \return        0-7           Bit offset in last byte.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *                 VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_BitCpyGetOffsetInBitsEndByte(
  VStdLib_CntType lengthInBits);

/**********************************************************************************************************************
 *   VStdLib_Loc_BitCpyBigEndianSrc_7()
 *********************************************************************************************************************/
/*! \brief         Copies big endian data from one memory location into another memory location.
 *  \details       Uses VStdLib_Loc_BitCpyBigEndianSrc() and a bit mask to copy 1 to 7 bits starting at pSrc8
 *                 to another memory location starting at pDst8.
 *  \param[out]    pDst8         Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]     dstByteIdx    Index to iterate over all bytes in the memory location to copy to.
 *  \param[in]     pSrc8         Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]     srcByteIdx    Index to iterate over all bytes in the memory location to copy from.
 *  \param[in]     srcIter       Pointer to the iteration structure of the memory location to copy from.
 *  \param[in]     srcShift      Variable to indicate if right/left or no shift is needed for the relevant source byte.
 *  \param[in]     dstBitMask    Mask to be able to copy number of bits < 8.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *                 VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyBigEndianSrc_7(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  VStdLib_CntType dstByteIdx,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift,
  uint8 dstBitMask);

/**********************************************************************************************************************
 *   VStdLib_Loc_BitCpyLittleEndianSrc_7()
 *********************************************************************************************************************/
/*! \brief         Copies little endian data from one memory location into another memory location.
 *  \details       Uses VStdLib_Loc_BitCpyLittleEndianSrc() and a bit mask to copy 1 to 7 bits starting at pSrc8
 *                 to another memory location starting at pDst8.
 *  \param[out]    pDst8         Pointer to the memory location to copy to.
 *  \param[in]     dstByteIdx    Index to iterate over all dstBytes.
 *  \param[in]     pSrc8         Pointer to the memory location to copy from.
 *  \param[in]     srcByteIdx    Index to iterate over all bytes in the memory location to copy from.
 *  \param[in]     srcIter       Pointer to the iteration structure of the memory location to copy from.
 *  \param[in]     srcShift      Variable to indicate if right/left or no shift is needed for the relevant source byte.
 *  \param[in]     dstBitMask    Mask to be able to copy number of bits < 8.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *                 VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyLittleEndianSrc_7(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  VStdLib_CntType dstByteIdx,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift,
  uint8 dstBitMask);

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyBigEndianSrc()
 *********************************************************************************************************************/
/*! \brief         Returns byte (big endian data) with relevant bits to copy to a memory location.
 *  \details       Uses VStdLib_Loc_BitCpyShiftRight_BigEndianSrc() or VStdLib_Loc_BitCpyShiftLeft_BigEndianSrc() to
 *                 get source byte with relevant bits.
 *  \param[in]     pSrc8         Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]     srcByteIdx    Index to indicate which source byte to use.
 *  \param[in]     srcIter       Pointer to the iteration structure of the memory location to copy from.
 *  \param[in]     srcShift      Variable to indicate if right/left or no shift is needed for the relevant source byte.
 *  \return        Source byte to copy.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *                 VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_BitCpyBigEndianSrc(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift);

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyLittleEndianSrc()
 *********************************************************************************************************************/
/*! \brief         Returns byte (little endian data) with relevant bits to copy to a memory location.
 *  \details       Uses VStdLib_Loc_BitCpyShiftRight_LittleEndianSrc() or VStdLib_Loc_BitCpyShiftLeft_LittleEndianSrc()
 *                 to get source byte with relevant bits.
 *  \param[in]     pSrc8         Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]     srcByteIdx    Index to indicate which source byte to use.
 *  \param[in]     srcIter       Pointer to the iteration structure of the memory location to copy from.
 *  \param[in]     srcShift      Variable to indicate if right/left or no shift is needed for the relevant source byte.
 *  \return        Source byte to copy.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *                 VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_BitCpyLittleEndianSrc(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift);

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyShiftRight_BigEndianSrc()
 *********************************************************************************************************************/
/*! \brief         Prepares byte (big endian data) with relevant bits to copy to a memory location.
 *  \details       To align the offsets of the memory locations the relevant byte in the source is shifted
 *                 and merged with the missing bits of the previous relevant source byte.
 *  \param[in]     pSrc8         Pointer to the memory location to copy from, must not be NULL.
 *  \param[out]    srcByte       Pointer to byte to be modified.
 *  \param[in]     srcByteIdx    Index to indicate which source byte to use.
 *  \param[in]     srcIter       Pointer to the iteration structure of the memory location to copy from.
 *  \param[in]     srcShift      Variable that indicates that right shift is needed for the relevant source byte.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *                 VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyShiftRight_BigEndianSrc(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) srcByte,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift);

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyShiftLeft_BigEndianSrc()
 *********************************************************************************************************************/
/*! \brief         Prepares byte (big endian data) with relevant bits to copy to a memory location.
 *  \details       To align the offsets of the memory locations the relevant byte in the source is shifted
 *                 and merged with the missing bits of the next relevant source byte.
 *  \param[in]     pSrc8         Pointer to the memory location to copy from, must not be NULL.
 *  \param[out]    srcByte       Pointer to byte to be modified.
 *  \param[in]     srcByteIdx    Index to indicate which source byte to use.
 *  \param[in]     srcIter       Pointer to the iteration structure of the memory location to copy from.
 *  \param[in]     srcShift      Variable that indicates that left shift is needed for the relevant source byte.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *                 VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyShiftLeft_BigEndianSrc(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) srcByte,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift);

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyShiftRight_LittleEndianSrc()
 *********************************************************************************************************************/
/*! \brief         Prepares byte (big endian data) with relevant bits to copy to a memory location.
 *  \details       To align the offsets of the memory locations the relevant byte in the source is shifted
 *                 and merged with the missing bits of the next relevant source byte.
 *  \param[in]     pSrc8         Pointer to the memory location to copy from, must not be NULL.
 *  \param[out]    srcByte       Pointer to byte to be modified.
 *  \param[in]     srcByteIdx    Index to indicate which source byte to use.
 *  \param[in]     srcIter       Pointer to the iteration structure of the memory location to copy from.
 *  \param[in]     srcShift      Variable that indicates that right shift is needed for the relevant source byte.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *                 VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyShiftRight_LittleEndianSrc(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) srcByte,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift);

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyShiftLeft_LittleEndianSrc()
 *********************************************************************************************************************/
/*! \brief         Prepares byte (big endian data) with relevant bits to copy to a memory location.
 *  \details       To align the offsets of the memory locations the relevant byte in the source is shifted
 *                 and merged with the missing bits of the previous relevant source byte.
 *  \param[in]     pSrc8         Pointer to the memory location to copy from, must not be NULL.
 *  \param[out]    srcByte       Pointer to byte to be modified.
 *  \param[in]     srcByteIdx    Index to indicate which source byte to use.
 *  \param[in]     srcIter       Pointer to the iteration structure of the memory location to copy from.
 *  \param[in]     srcShift      Variable that indicates that left shift is needed for the relevant source byte.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *                 VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyShiftLeft_LittleEndianSrc(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) srcByte,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift);

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpySwapEndian()
 *********************************************************************************************************************/
/*! \brief         Copies and swaps nCnt bits from one memory location into another memory location.
 *  \details       Copies bits byte-wise starting at pSrc8 to another memory location starting at pDst8.
 *  \param[out]    pDst8         Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]     dstIter       Pointer to the iteration structure of the memory location to copy to.
 *  \param[in]     pSrc8         Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]     srcIter       Pointer to the iteration structure of the memory location to copy from.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *                 VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre           -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpySwapEndian(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) dstIter,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter);

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyBigToBigEndian()
 *********************************************************************************************************************/
/*! \brief       Copies big endian data from one memory location to another memory location with big endian data.
 *  \details     Copies nCnt bits starting at pSrc to another memory location starting at pDst.
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to the 
 *                                    destination-byte-pointer. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to the
 *                                    source-byte-pointer. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *               VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyBigToBigEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt);

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyBigToLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Copies big endian data from one memory location to another memory location with little endian data.
 *  \details     Copies nCnt bits starting at pSrc to another memory location starting at pDst.
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to the 
 *                                    destination-byte-pointer. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to the
 *                                    source-byte-pointer. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *               VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyBigToLittleEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt);

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyLittleToBigEndian()
 *********************************************************************************************************************/
/*! \brief       Copies little endian data from one memory location to another memory location with big endian data.
 *  \details     Copies nCnt bits starting at pSrc to another memory location starting at pDst.
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to the 
 *                                    destination-byte-pointer. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to the
 *                                    source-byte-pointer. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *               VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyLittleToBigEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt);

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyLittleToLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Copies little endian data from one memory location to another memory location with little endian data.
 *  \details     Copies nCnt bits starting at pSrc to another memory location starting at pDst.
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to the 
 *                                    destination-byte-pointer. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to the
 *                                    source-byte-pointer. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON ||
 *               VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyLittleToLittleEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt);

#endif /* (VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON) ||
          (VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON) */

#if (VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_Loc_GetMemCheckResult()
 *********************************************************************************************************************/
/*! \brief         Checks if a buffer value is equal to a pattern.
 *  \details       Assigns a passed errorCode to the retVal in case of mismatch.
 *  \param[in]     nBufValue      Value to be checked.
 *  \param[in]     nPattern      The character to be checked against.
 *  \param[in]     pRetVal       Pointer to the result.
 *  \param[in]     errorCode     Value to be assigned as the result, if bufValue is not equal to pattern.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION != STD_ON
 *  \pre           errorCode != E_OK
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_GetMemCheckResult(
  uint32 nBufValue,
  uint32 nPattern,
  P2VAR(Std_ReturnType, AUTOMATIC, VSTDLIB_VAR_FAR) pRetVal,
  Std_ReturnType errorCode);

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCheck_8()
 *********************************************************************************************************************/
/*! \brief         Checks byte-wise if the data at pBuf8 matches the nPattern (any alignment).
 *  \details       -
 *  \param[out]    pBuf8         8-bit pointer to the memory location to be checked.
 *  \param[in]     nPattern      The character to be checked for in the memory.
 *  \param[in]     nCnt          Number of bytes to check, pBuf8 must be valid for this amount.
 *  \param[in]     errorCode     Value to be returned, if content of pBuf8 is not equal to pattern.
 *  \return        E_OK          pBuf8 consists only of the pattern.
 *  \return        errorCode     pBuf8 contains atleast one value that is not equal to pattern.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pBuf8' and 'nCnt' have to define a valid memory area and (errorCode != E_OK).
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_Loc_MemCheck_8(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf8,
  uint8 nPattern,
  uint32_least nCnt,
  Std_ReturnType errorCode);

# if (VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON)

/**********************************************************************************************************************
 *  VStdLib_Loc_MemCheck_32_Aligned()
 *********************************************************************************************************************/
/*! \brief         Checks data at pBuf(32-bit aligned) for the nPattern.
 *  \details       The buffer data is checked 4-bytes-wise until less than 4 bytes are left to be checked.
 *                 The remaining bytes are compared byte-wise using VStdLib_Loc_MemCheck_8()
 *  \param[out]    pBuf32        32-bit pointer to the memory location to be checked.
 *  \param[in]     nPattern      The character to checked for in the memory.
 *  \param[in]     nCnt          Number of bytes to check, pBuf must be valid for this amount.
 *  \param[in]     errorCode     Value to be returned, if content of pBuf is not equal to pattern.
 *  \return        E_OK          pBuf consists only of the pattern.
 *  \return        errorCode     pBuf contains atleast one value that is not equal to pattern.
 *  \context       ANY
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *  \config        VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION != STD_ON && VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON
 *  \pre           The parameters 'pBuf32' and 'nCnt' have to define a valid memory area and (errorCode != E_OK).
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_Loc_MemCheck_32_Aligned(
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf32,
  uint8 nPattern,
  uint32_least nCnt,
  Std_ReturnType errorCode);

# endif /* VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON */
#endif /* VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION != STD_ON */

#if ((VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || \
     (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || ((defined PLATFORM_SUPPORT_SINT64_UINT64) && \
     (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON))) 

/**********************************************************************************************************************
 *  VStdLib_Loc_GetHighestBitPosOneInByte()
 *********************************************************************************************************************/
/*! \brief       Returns the highest bit position of a set bit in the specified byte value.
 *  \details     -
 *  \param[in]   value         The byte value in which the highest set bit shall be found.
 *  \return      0-7           Bit position of the highest set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON ||
 *               VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON ||
 *               VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON ||
 *               (defined PLATFORM_SUPPORT_SINT64_UINT64 && VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)
 *  \pre         The parameters 'value' has to be greater than zero (contains at least one set bit).
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetHighestBitPosOneInByte(uint8 value);

/**********************************************************************************************************************
 *  VStdLib_Loc_GetLowestBitPosOneInByte()
 *********************************************************************************************************************/
/*! \brief       Returns the lowest bit position of a set bit in the specified byte value.
 *  \details     -
 *  \param[in]   value         The byte value in which the lowest set bit shall be found.
 *  \return      0-7           Bit position of the lowest set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON ||
 *               VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON ||
 *               VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON ||
 *               (defined PLATFORM_SUPPORT_SINT64_UINT64 && VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)
 *  \pre         The parameters 'value' has to be smaller than its maximum value (contains at least one unset bit).
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetLowestBitPosOneInByte(uint8 value);

#endif /* (VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) ||
          (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || ((defined PLATFORM_SUPPORT_SINT64_UINT64) && \
          (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)) */

#if ((VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || \
     ((defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)))

/**********************************************************************************************************************
 *  VStdLib_Loc_GetHighestBitPosOneInTwoBytes()
 *********************************************************************************************************************/
/*! \brief       Returns the highest bit position of a set bit in the specified two byte value.
 *  \details     -
 *  \param[in]   value         The two byte value in which the highest set bit shall be found.
 *  \return      0-15          Bit position of the highest set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON ||
 *               VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON ||
 *               (defined PLATFORM_SUPPORT_SINT64_UINT64 && VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)
 *  \pre         The parameters 'value' has to be greater than zero (contains at least one set bit).
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetHighestBitPosOneInTwoBytes(uint16 value);

/**********************************************************************************************************************
 *  VStdLib_Loc_GetLowestBitPosOneInTwoBytes()
 *********************************************************************************************************************/
/*! \brief       Returns the lowest bit position of a set bit in the specified two byte value.
 *  \details     -
 *  \param[in]   value         The two byte value in which the lowest set bit shall be found.
 *  \return      0-15          Bit position of the lowest set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON ||
 *               VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON ||
 *               (defined PLATFORM_SUPPORT_SINT64_UINT64 && VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)
 *  \pre         The parameters 'value' has to be smaller than its maximum value (contains at least one unset bit).
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetLowestBitPosOneInTwoBytes(uint16 value);

#endif /* (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) ||
          ((defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)) */

#if ((VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || ((defined PLATFORM_SUPPORT_SINT64_UINT64) && \
     (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)))

/**********************************************************************************************************************
 *  VStdLib_Loc_GetHighestBitPosOneInFourBytes()
 *********************************************************************************************************************/
/*! \brief       Returns the highest bit position of a set bit in the specified four byte value.
 *  \details     -
 *  \param[in]   value         The four byte value in which the highest set bit shall be found.
 *  \return      0-31          Bit position of the highest set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON ||
 *               (defined PLATFORM_SUPPORT_SINT64_UINT64 && VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)
 *  \pre         The parameters 'value' has to be greater than zero (contains at least one set bit).
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetHighestBitPosOneInFourBytes(uint32 value);

/**********************************************************************************************************************
 *  VStdLib_Loc_GetLowestBitPosOneInFourBytes()
 *********************************************************************************************************************/
/*! \brief       Returns the lowest bit position of a set bit in the specified four byte value.
 *  \details     -
 *  \param[in]   value         The four byte value in which the lowest set bit shall be found.
 *  \return      0-31          Bit position of the lowest set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON ||
 *               (defined PLATFORM_SUPPORT_SINT64_UINT64 && VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)
 *  \pre         The parameters 'value' has to be smaller than its maximum value (contains at least one unset bit).
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetLowestBitPosOneInFourBytes(uint32 value);

#endif /* (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || ((defined PLATFORM_SUPPORT_SINT64_UINT64) &&
          (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)) */

#if ((defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON))

/**********************************************************************************************************************
 *  VStdLib_Loc_GetHighestBitPosOneInEightBytes()
 *********************************************************************************************************************/
/*! \brief       Returns the highest bit position of a set bit in the specified eight byte value.
 *  \details     -
 *  \param[in]   value         The eight byte value in which the highest set bit shall be found.
 *  \return      0-63          Bit position of the highest set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      defined PLATFORM_SUPPORT_SINT64_UINT64 && VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON
 *  \pre         The parameters 'value' has to be greater than zero (contains at least one set bit).
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetHighestBitPosOneInEightBytes(uint64 value);

/**********************************************************************************************************************
 *  VStdLib_Loc_GetLowestBitPosOneInEightBytes()
 *********************************************************************************************************************/
/*! \brief       Returns the lowest bit position of a set bit in the specified eight byte value.
 *  \details     -
 *  \param[in]   value         The eight byte value in which the lowest set bit shall be found.
 *  \return      0-63          Bit position of the lowest set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      defined PLATFORM_SUPPORT_SINT64_UINT64 && VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON
 *  \pre         The parameters 'value' has to be smaller than its maximum value (contains at least one unset bit).
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetLowestBitPosOneInEightBytes(uint64 value);

#endif /* (defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) */

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

#if (VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 * VStdLib_Loc_GetMemCmpNeqResult()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(sint8, VSTDLIB_CODE) VStdLib_Loc_GetMemCmpNeqResult(uint32 buf1Value, uint32 buf2Value)
{
  /* ----- Local Variables ---------------------------------------------- */
  sint8 result;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Set result depending, if buffer1 value is greater or smaller than buffer2 value */
  if (buf1Value > buf2Value)
  {
    result = VSTDLIB_MEM_CMP_RES_G;
  }
  else /* (buf1Value < buf2Value) */
  {
    result = VSTDLIB_MEM_CMP_RES_L;
  }

  return result;
}

/**********************************************************************************************************************
 * VStdLib_Loc_MemCmp_8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(sint8, VSTDLIB_CODE) VStdLib_Loc_MemCmp_8(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf1_8,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf2_8,
  uint32_least nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32_least pos;
  sint8 result = VSTDLIB_MEM_CMP_RES_EQ;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Linearly compare buffers byte-wise in a loop */
  for (pos = 0u; pos < nCnt; pos++) /* FETA_VSTDLIB_LOC_MEMCMP_X_X */
  {
    /* #110 If current buffer values are unequal */
    if (pBuf1_8[pos] != pBuf2_8[pos])
    {
      /* #120 Set corresponding result value and break the comparison */
      result = VStdLib_Loc_GetMemCmpNeqResult((uint32)pBuf1_8[pos], (uint32)pBuf2_8[pos]);
      break;
    }
  }

  return result;
}

# if (VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON)

/**********************************************************************************************************************
 * VStdLib_Loc_MemSet_8_3()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemSet_8_3(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  uint8 nPattern,
  uint8_least nCnt)
{
  /* ----- Implementation ----------------------------------------------- */
#  if (VSTDLIB_USE_JUMPTABLES == STD_ON)

  /* #10 Initialize up to 3 bytes using a jump table */
  /* PRQA S 2003 VSTDLIB_NO_CASE_BREAK */ /* MD_VStdLib_NoCaseBreak */
  switch (nCnt)
  {
  case 3:
    pDst8[2] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 2:
    pDst8[1] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 1:
    pDst8[0] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  default:
    /* nothing to do */
    break;
  }
  /* PRQA L:VSTDLIB_NO_CASE_BREAK */

#  else /* VSTDLIB_USE_JUMPTABLES == STD_OFF */

  uint8_least idx;

  /* #20 Linearly initialize the bytes using a loop */
  for (idx = 0u; idx < nCnt; idx++) /* FETA_VSTDLIB_LOC_MEMSET_X_X */
  {
    pDst8[idx] = nPattern; /* SBSW_VSTDLIB_ACCESS_LOOP */
  }

#  endif /* VSTDLIB_USE_JUMPTABLES */
} /* VStdLib_Loc_MemSet_8_3() */

/**********************************************************************************************************************
 * VStdLib_Loc_MemSet_8_31()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6030 1 */ /* MD_VStdLib_Metrics */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemSet_8_31(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  uint8 nPattern,
  uint8_least nCnt)
{
  /* ----- Implementation ----------------------------------------------- */
#  if (VSTDLIB_USE_JUMPTABLES == STD_ON)

  /* #10 Initialize up to 31 bytes using a jump table */
  /* PRQA S 2003 VSTDLIB_NO_CASE_BREAK */ /* MD_VStdLib_NoCaseBreak */
  switch (nCnt)
  {
  case 31:
    pDst8[30] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 30:
    pDst8[29] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 29:
    pDst8[28] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 28:
    pDst8[27] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 27:
    pDst8[26] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 26:
    pDst8[25] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 25:
    pDst8[24] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 24:
    pDst8[23] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 23:
    pDst8[22] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 22:
    pDst8[21] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 21:
    pDst8[20] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 20:
    pDst8[19] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 19:
    pDst8[18] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 18:
    pDst8[17] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 17:
    pDst8[16] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 16:
    pDst8[15] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 15:
    pDst8[14] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 14:
    pDst8[13] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 13:
    pDst8[12] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 12:
    pDst8[11] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 11:
    pDst8[10] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 10:
    pDst8[9] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 9:
    pDst8[8] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 8:
    pDst8[7] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 7:
    pDst8[6] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 6:
    pDst8[5] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 5:
    pDst8[4] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 4:
    pDst8[3] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 3:
    pDst8[2] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 2:
    pDst8[1] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 1:
    pDst8[0] = nPattern; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  default:
    /* nothing to do */
    break;
  }
  /* PRQA L:VSTDLIB_NO_CASE_BREAK */

#  else /* VSTDLIB_USE_JUMPTABLES == STD_OFF */

  uint8_least idx;

  /* #20 Linearly initialize the bytes using a loop */
  for (idx = 0u; idx < nCnt; idx++) /* FETA_VSTDLIB_LOC_MEMSET_X_X */
  {
    pDst8[idx] = nPattern;  /* SBSW_VSTDLIB_ACCESS_LOOP */
  }

#  endif /* VSTDLIB_USE_JUMPTABLES */
} /* VStdLib_Loc_MemSet_8_31() */

/**********************************************************************************************************************
 * VStdLib_Loc_MemSet_32_Aligned()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemSet_32_Aligned(
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32,
  uint8 nPattern,
  uint32_least nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32_least dPos = 0u;
  uint32_least remaining;
  uint32 nPattern32 =
    (uint32)(((uint32)nPattern << 24u) | ((uint32)nPattern << 16u) | ((uint32)nPattern << 8u) | nPattern);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Linearly initialize 16-byte blocks using 32-bit accesses in a loop */
  for (remaining = nCnt; remaining >= 16u; remaining -= 16u) /* FETA_VSTDLIB_LOC_MEMSET_X_X */
  {
    pDst32[dPos] = nPattern32; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 1u] = nPattern32; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 2u] = nPattern32; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 3u] = nPattern32; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    dPos += 4u;
  }

#  if (VSTDLIB_USE_JUMPTABLES == STD_ON)

  /* #20 Initialize the remaining block using a jump table */
  /* PRQA S 2003 VSTDLIB_NO_CASE_BREAK */ /* MD_VStdLib_NoCaseBreak */
  switch (remaining)
  {
    /* #30 Use 32-bit access for remaining four byte tuples */
  case 15:
  case 14:
  case 13:
  case 12:
    pDst32[dPos] = nPattern32; /* SBSW_VSTDLIB_ACCESS_32_JUMPTABLE */
    dPos++;
    /* intentionally no break */
  case 11:
  case 10:
  case 9:
  case 8:
    pDst32[dPos] = nPattern32; /* SBSW_VSTDLIB_ACCESS_32_JUMPTABLE */
    dPos++;
    /* intentionally no break */
  case 7:
  case 6:
  case 5:
  case 4:
    pDst32[dPos] = nPattern32; /* SBSW_VSTDLIB_ACCESS_32_JUMPTABLE */
    dPos++;
    /* intentionally no break */
  case 3:
  case 2:
  case 1:
    /* #40 Use 8-bit access for remaining three bytes */
    VStdLib_Loc_MemSet_8_3( /* SBSW_VSTDLIB_CALL_REMAINING_JUMPTABLE */ /* FETA_VSTDLIB_CALL_LOC_MEMSET_8_X_REMAINING */
      (P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pDst32[dPos],
      nPattern,
      (uint8_least)(remaining & VSTDLIB_MASK_3));
    /* intentionally no break */
  default:
    /* nothing to do */
    break;
  }
  /* PRQA L:VSTDLIB_NO_CASE_BREAK */

#  else /* VSTDLIB_USE_JUMPTABLES == STD_OFF */

  /* #50 Initialize the remaining block using a byte-wise algorithm */
  VStdLib_Loc_MemSet_8_31( /* SBSW_VSTDLIB_CALL_REMAINING */ /* FETA_VSTDLIB_CALL_LOC_MEMSET_8_X_REMAINING */
    (P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pDst32[dPos],
    nPattern,
    (uint8_least)remaining);

#  endif /* VSTDLIB_USE_JUMPTABLES */
} /* VStdLib_Loc_MemSet_32_Aligned() */

/**********************************************************************************************************************
 * VStdLib_Loc_MemCpy_8_3()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpy_8_3(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  uint8_least nCnt)
{
  /* ----- Implementation ----------------------------------------------- */
#  if (VSTDLIB_USE_JUMPTABLES == STD_ON)

  /* #10 Copy up to 3 bytes using a jump table */
  /* PRQA S 2003 VSTDLIB_NO_CASE_BREAK */ /* MD_VStdLib_NoCaseBreak */
  switch (nCnt)
  {
  case 3:
    pDst8[2] = pSrc8[2]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 2:
    pDst8[1] = pSrc8[1]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 1:
    pDst8[0] = pSrc8[0]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  default:
    /* nothing to do */
    break;
  }
  /* PRQA L:VSTDLIB_NO_CASE_BREAK */

#  else /* VSTDLIB_USE_JUMPTABLES == STD_OFF */

  uint8_least idx;

  /* #20 Linearly copy the bytes using a loop */
  for (idx = 0u; idx < nCnt; idx++) /* FETA_VSTDLIB_LOC_MEMCPY_X_X */
  {
    pDst8[idx] = pSrc8[idx]; /* SBSW_VSTDLIB_ACCESS_LOOP */
  }

#  endif /* VSTDLIB_USE_JUMPTABLES */
} /* VStdLib_Loc_MemCpy_8_3() */

/**********************************************************************************************************************
 * VStdLib_Loc_MemCpy_8_31()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6030 1 */ /* MD_VStdLib_Metrics */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpy_8_31(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  uint8_least nCnt)
{
  /* ----- Implementation ----------------------------------------------- */
#  if (VSTDLIB_USE_JUMPTABLES == STD_ON)

  /* #10 Copy up to 31 bytes using a jump table */
  /* PRQA S 2003 VSTDLIB_NO_CASE_BREAK */ /* MD_VStdLib_NoCaseBreak */
  switch (nCnt)
  {
  case 31:
    pDst8[30] = pSrc8[30]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 30:
    pDst8[29] = pSrc8[29]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 29:
    pDst8[28] = pSrc8[28]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 28:
    pDst8[27] = pSrc8[27]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 27:
    pDst8[26] = pSrc8[26]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 26:
    pDst8[25] = pSrc8[25]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 25:
    pDst8[24] = pSrc8[24]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 24:
    pDst8[23] = pSrc8[23]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 23:
    pDst8[22] = pSrc8[22]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 22:
    pDst8[21] = pSrc8[21]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 21:
    pDst8[20] = pSrc8[20]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 20:
    pDst8[19] = pSrc8[19]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 19:
    pDst8[18] = pSrc8[18]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 18:
    pDst8[17] = pSrc8[17]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 17:
    pDst8[16] = pSrc8[16]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 16:
    pDst8[15] = pSrc8[15]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 15:
    pDst8[14] = pSrc8[14]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 14:
    pDst8[13] = pSrc8[13]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 13:
    pDst8[12] = pSrc8[12]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 12:
    pDst8[11] = pSrc8[11]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 11:
    pDst8[10] = pSrc8[10]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 10:
    pDst8[9] = pSrc8[9]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 9:
    pDst8[8] = pSrc8[8]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 8:
    pDst8[7] = pSrc8[7]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 7:
    pDst8[6] = pSrc8[6]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 6:
    pDst8[5] = pSrc8[5]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 5:
    pDst8[4] = pSrc8[4]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 4:
    pDst8[3] = pSrc8[3]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 3:
    pDst8[2] = pSrc8[2]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 2:
    pDst8[1] = pSrc8[1]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  case 1:
    pDst8[0] = pSrc8[0]; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    /* intentionally no break */
  default:
    /* nothing to do */
    break;
  }
  /* PRQA L:VSTDLIB_NO_CASE_BREAK */

#  else /* VSTDLIB_USE_JUMPTABLES == STD_OFF */

  uint8_least idx;

  /* #20 Linearly copy the bytes using a loop */
  for (idx = 0u; idx < nCnt; idx++) /* FETA_VSTDLIB_LOC_MEMCPY_X_X */
  {
    pDst8[idx] = pSrc8[idx]; /* SBSW_VSTDLIB_ACCESS_LOOP */
  }

#  endif /* VSTDLIB_USE_JUMPTABLES */
} /* VStdLib_Loc_MemCpy_8_31() */

/**********************************************************************************************************************
 * VStdLib_Loc_MemCpy_32_Aligned()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpy_32_Aligned(
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc32,
  uint32_least nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32_least dPos = 0u; /* == sPos */
  uint32_least remaining;
  uint32 x0, x1, x2, x3;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Linearly copy 16-byte blocks using 32-bit accesses in a loop */
  for (remaining = nCnt; remaining >= 16u; remaining -= 16u) /* FETA_VSTDLIB_LOC_MEMCPY_X_X */
  {
    x0 = pSrc32[dPos];
    x1 = pSrc32[dPos + 1u];
    x2 = pSrc32[dPos + 2u];
    x3 = pSrc32[dPos + 3u];
    pDst32[dPos] = x0; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 1u] = x1; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 2u] = x2; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 3u] = x3; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    dPos += 4u;
  }

#  if (VSTDLIB_USE_JUMPTABLES == STD_ON)

  /* #20 Copy the remaining block using a jump table */
  /* PRQA S 2003 VSTDLIB_NO_CASE_BREAK */ /* MD_VStdLib_NoCaseBreak */
  switch (remaining)
  {
    /* #30 Use 32-bit access for remaining four byte tuples */
  case 15:
  case 14:
  case 13:
  case 12:
    pDst32[dPos] = pSrc32[dPos]; /* SBSW_VSTDLIB_ACCESS_32_JUMPTABLE */
    dPos++;
    /* intentionally no break */
  case 11:
  case 10:
  case 9:
  case 8:
    pDst32[dPos] = pSrc32[dPos]; /* SBSW_VSTDLIB_ACCESS_32_JUMPTABLE */
    dPos++;
    /* intentionally no break */
  case 7:
  case 6:
  case 5:
  case 4:
    pDst32[dPos] = pSrc32[dPos]; /* SBSW_VSTDLIB_ACCESS_32_JUMPTABLE */
    dPos++;
    /* intentionally no break */
  case 3:
  case 2:
  case 1:
    /* #40 Use 8-bit access for remaining three bytes */
    VStdLib_Loc_MemCpy_8_3( /* SBSW_VSTDLIB_CALL_REMAINING_JUMPTABLE */ /* FETA_VSTDLIB_CALL_LOC_MEMCPY_8_X_REMAINING */
      (P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pDst32[dPos],
      (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pSrc32[dPos],
      (uint8_least)(remaining & VSTDLIB_MASK_3));
    /* intentionally no break */
  default:
    /* nothing to do */
    break;
  }
  /* PRQA L:VSTDLIB_NO_CASE_BREAK */

#  else /* VSTDLIB_USE_JUMPTABLES == STD_OFF */

  /* #50 Copy the remaining block using a byte-wise algorithm */
  VStdLib_Loc_MemCpy_8_31( /* SBSW_VSTDLIB_CALL_REMAINING */ /* FETA_VSTDLIB_CALL_LOC_MEMCPY_8_X_REMAINING */
    (P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pDst32[dPos],
    (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pSrc32[dPos],
    (uint8_least)remaining);

#  endif /* VSTDLIB_USE_JUMPTABLES */
} /* VStdLib_Loc_MemCpy_32_Aligned() */

/**********************************************************************************************************************
 * VStdLib_Loc_MemCpy_8_PartialWord()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpy_8_PartialWord(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  uint8_least nCnt,
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pPrev,
  P2VAR(uint32_least, AUTOMATIC, VSTDLIB_VAR_FAR) pDPos)
{
  /* ----- Implementation ----------------------------------------------- */
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#   if (VSTDLIB_USE_JUMPTABLES == STD_ON)

  /* #10 Copy 1 to 3 bytes using a jump table and adapt '*pDPos' and '*pPrev' for little endian byte order */
  /* PRQA S 2003 VSTDLIB_NO_CASE_BREAK */ /* MD_VStdLib_NoCaseBreak */
  switch (nCnt)
  {
  case 3:
    pDst8[*pDPos] = (uint8)*pPrev; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    (*pDPos)++; /* SBSW_VSTDLIB_ACCESS_LOCAL */
    (*pPrev) >>= 8u; /* SBSW_VSTDLIB_ACCESS_LOCAL */
    /* intentionally no break */
  case 2:
    pDst8[*pDPos] = (uint8)*pPrev; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    (*pDPos)++; /* SBSW_VSTDLIB_ACCESS_LOCAL */
    (*pPrev) >>= 8u; /* SBSW_VSTDLIB_ACCESS_LOCAL */
    /* intentionally no break */
  default: /* case 1 - nCnt is always in range [1..3] */
    pDst8[*pDPos] = (uint8)*pPrev; /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    (*pDPos)++; /* SBSW_VSTDLIB_ACCESS_LOCAL */
    (*pPrev) >>= 8u; /* SBSW_VSTDLIB_ACCESS_LOCAL */
    break;
  }
  /* PRQA L:VSTDLIB_NO_CASE_BREAK */

#   else /* VSTDLIB_USE_JUMPTABLES == STD_OFF */

  uint8_least idx;

  /* #20 Linearly copy the bytes using a loop and adapt '*pDPos' and '*pPrev' for little endian byte order */
  for (idx = 0u; idx < nCnt; idx++) /* FETA_VSTDLIB_LOC_MEMCPY_X_X */
  {
    pDst8[*pDPos] = (uint8)(uint8)*pPrev; /* SBSW_VSTDLIB_ACCESS_LOOP */
    (*pDPos)++; /* SBSW_VSTDLIB_ACCESS_LOCAL */
    (*pPrev) >>= 8u; /* SBSW_VSTDLIB_ACCESS_LOCAL */
  }

#   endif /* VSTDLIB_USE_JUMPTABLES */
#  else /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */
#   if (VSTDLIB_USE_JUMPTABLES == STD_ON)

  /* #30 Copy 1 to 3 bytes using a jump table and adapt '*pDPos' and '*pPrev' for big endian byte order */
  /* PRQA S 2003 VSTDLIB_NO_CASE_BREAK */ /* MD_VStdLib_NoCaseBreak */
  switch (nCnt)
  {
  case 3:
    pDst8[*pDPos] = (uint8)((*pPrev) >> 24u); /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    (*pDPos)++; /* SBSW_VSTDLIB_ACCESS_LOCAL */
    (*pPrev) <<= 8u; /* SBSW_VSTDLIB_ACCESS_LOCAL */
    /* intentionally no break */
  case 2:
    pDst8[*pDPos] = (uint8)((*pPrev) >> 24u); /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    (*pDPos)++; /* SBSW_VSTDLIB_ACCESS_LOCAL */
    (*pPrev) <<= 8u; /* SBSW_VSTDLIB_ACCESS_LOCAL */
    /* intentionally no break */
  default: /* case 1 -  nCnt is always in range [1..3] */
    pDst8[*pDPos] = (uint8)((*pPrev) >> 24u); /* SBSW_VSTDLIB_ACCESS_8_JUMPTABLE */
    (*pDPos)++; /* SBSW_VSTDLIB_ACCESS_LOCAL */
    (*pPrev) <<= 8u; /* SBSW_VSTDLIB_ACCESS_LOCAL */
    break;
  }
  /* PRQA L:VSTDLIB_NO_CASE_BREAK */

#   else /* VSTDLIB_USE_JUMPTABLES == STD_OFF */

  uint8_least idx;

  /* #40 Linearly copy the bytes using a loop and adapt '*pDPos' and '*pPrev' for big endian byte order */
  for (idx = 0u; idx < nCnt; idx++) /* FETA_VSTDLIB_LOC_MEMCPY_X_X */
  {
    pDst8[*pDPos] = (uint8)((*pPrev) >> 24u); /* SBSW_VSTDLIB_ACCESS_LOOP */
    (*pDPos)++; /* SBSW_VSTDLIB_ACCESS_LOCAL */
    (*pPrev) <<= 8u; /* SBSW_VSTDLIB_ACCESS_LOCAL */
  }

#   endif /* VSTDLIB_USE_JUMPTABLES */
#  endif /* CPU_BYTE_ORDER */
} /* VStdLib_Loc_MemCpy_8_PartialWord() */

/**********************************************************************************************************************
 * VStdLib_Loc_MemCpy_32_Unaligned()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpy_32_Unaligned(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc32,
  uint32_least nCnt,
  uint8_least nDstOffset)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32_least dPos = 0u;
  uint32_least sPos = 0u;
  uint32_least remaining = nCnt;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 If more than 32 bytes have to be copied */
  if (remaining >= 32u)
  {
    P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32;
    uint32 prev;

    /* #20 Copy the first partial word to reach the 32-bit boundary and adjust the destination pointer */
    prev = pSrc32[sPos];
    remaining -= 4u;
    VStdLib_Loc_MemCpy_8_PartialWord( /* SBSW_VSTDLIB_CALL_MODIFIED_CNT */ /* FETA_VSTDLIB_CALL_LOC_MEMCPY_8_PARTIALWORD */
      pDst8,
      nDstOffset,
      &prev,
      &dPos);

    /* At this point 'pDst8[dPos]' is aligned to a 32-bit boundary */
    pDst32 = (P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR)) &pDst8[dPos]; /* PRQA S 0310, 3305 */ /* MD_VStdLib_PtrCast */
    dPos = 0u;
    sPos++;

    /* #30 Linearly copy 16-byte blocks using 32-bit accesses by correcting the byte displacement of dst and src */
    if (nDstOffset == 3u) /* 24-bit displacement */
    {
      for (; remaining >= 16u; remaining -= 16u) /* FETA_VSTDLIB_LOC_MEMCPY_32_UNALIGNED */
      {
        uint32 x0, x1, x2, x3, x4;
        x1 = pSrc32[sPos];
        x2 = pSrc32[sPos + 1u];
        x3 = pSrc32[sPos + 2u];
        x4 = pSrc32[sPos + 3u];
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        x0 = prev | (x1 << 8u);
        x1 = (x1 >> 24u) | (x2 << 8u);
        x2 = (x2 >> 24u) | (x3 << 8u);
        x3 = (x3 >> 24u) | (x4 << 8u);
        prev = x4 >> 24u;
#  else /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */
        x0 = prev | (x1 >> 8u);
        x1 = (x1 << 24u) | (x2 >> 8u);
        x2 = (x2 << 24u) | (x3 >> 8u);
        x3 = (x3 << 24u) | (x4 >> 8u);
        prev = x4 << 24;
#  endif /* CPU_BYTE_ORDER */
        pDst32[dPos] = x0; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
        pDst32[dPos + 1u] = x1; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
        pDst32[dPos + 2u] = x2; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
        pDst32[dPos + 3u] = x3; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
        dPos += 4u;
        sPos += 4u;
      }
    }
    else if (nDstOffset == 2u) /* 16-bit displacement */
    {
      for (; remaining >= 16u; remaining -= 16u) /* FETA_VSTDLIB_LOC_MEMCPY_32_UNALIGNED */
      {
        uint32 x0, x1, x2, x3, x4;
        x1 = pSrc32[sPos];
        x2 = pSrc32[sPos + 1u];
        x3 = pSrc32[sPos + 2u];
        x4 = pSrc32[sPos + 3u];
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        x0 = prev | (x1 << 16u);
        x1 = (x1 >> 16u) | (x2 << 16u);
        x2 = (x2 >> 16u) | (x3 << 16u);
        x3 = (x3 >> 16u) | (x4 << 16u);
        prev = x4 >> 16u;
#  else /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */
        x0 = prev | (x1 >> 16);
        x1 = (x1 << 16u) | (x2 >> 16u);
        x2 = (x2 << 16u) | (x3 >> 16u);
        x3 = (x3 << 16u) | (x4 >> 16u);
        prev = x4 << 16;
#  endif /* CPU_BYTE_ORDER */
        pDst32[dPos] = x0; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
        pDst32[dPos + 1u] = x1; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
        pDst32[dPos + 2u] = x2; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
        pDst32[dPos + 3u] = x3; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
        dPos += 4u;
        sPos += 4u;
      }

    }
    else /* nDstOffset == 1 - 8-bit displacement */
    {
      for (; remaining >= 16u; remaining -= 16u) /* FETA_VSTDLIB_LOC_MEMCPY_32_UNALIGNED */
      {
        uint32 x0, x1, x2, x3, x4;
        x1 = pSrc32[sPos];
        x2 = pSrc32[sPos + 1u];
        x3 = pSrc32[sPos + 2u];
        x4 = pSrc32[sPos + 3u];
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        x0 = prev | (x1 << 24u);
        x1 = (x1 >> 8u) | (x2 << 24u);
        x2 = (x2 >> 8u) | (x3 << 24u);
        x3 = (x3 >> 8u) | (x4 << 24u);
        prev = x4 >> 8u;
#  else /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */
        x0 = prev | (x1 >> 24u);
        x1 = (x1 << 8u) | (x2 >> 24u);
        x2 = (x2 << 8u) | (x3 >> 24u);
        x3 = (x3 << 8u) | (x4 >> 24u);
        prev = x4 << 8u;
#  endif /* CPU_BYTE_ORDER */
        pDst32[dPos] = x0; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
        pDst32[dPos + 1u] = x1; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
        pDst32[dPos + 2u] = x2; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
        pDst32[dPos + 3u] = x3; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
        dPos += 4u;
        sPos += 4u;
      }
    }

    /* #40 Linearly copy the remaining words using a loop */
    {
      uint8_least prevShift = (nDstOffset << 3u) & VSTDLIB_MASK_31;
      uint8_least xShift = 32u - prevShift;
      /* At this point remaining is < 16 byte */
      for (; remaining >= 4u; remaining -= 4u) /* FETA_VSTDLIB_LOC_MEMCPY_32_UNALIGNED */
      {
        uint32 x0, x1;
        x1 = pSrc32[sPos];
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        x0 = prev | (x1 << xShift);
        prev = x1 >> prevShift;
#  else /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */
        x0 = prev | (x1 >> xShift);
        prev = x1 << prevShift;
#  endif /* CPU_BYTE_ORDER */
        pDst32[dPos] = x0; /* SBSW_VSTDLIB_ACCESS_32_LOOP_REMAINING */
        dPos++;
        sPos++;
      }
    }

    /* #50 Copy the remaining partial word using a byte-wise algorithm */
    pDst8 = (P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pDst32[dPos]; /* PRQA S 1338 */ /* MD_VStdLib_ParamMod */
    dPos = 0u;
    VStdLib_Loc_MemCpy_8_PartialWord( /* SBSW_VSTDLIB_CALL_REMAINING_PARTIAL */ /* FETA_VSTDLIB_CALL_LOC_MEMCPY_8_PARTIALWORD_CALC */
      pDst8,
      (4u - nDstOffset),
      &prev,
      &dPos);
  }

  /* #60 Copy the remaining bytes using a byte-wise algorithm */
  VStdLib_Loc_MemCpy_8_31( /* SBSW_VSTDLIB_CALL_REMAINING */ /* FETA_VSTDLIB_CALL_LOC_MEMCPY_8_31_REMAINING */
    &pDst8[dPos],
    (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pSrc32[sPos],
    (uint8_least)remaining);
} /* VStdLib_Loc_MemCpy_32_Unaligned() */

/**********************************************************************************************************************
 * VStdLib_Loc_MemCmp_32_Aligned()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(sint8, VSTDLIB_CODE) VStdLib_Loc_MemCmp_32_Aligned(
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf1_32,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf2_32,
  uint32_least nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32_least pos = 0u;
  uint32_least remaining = nCnt;
  sint8 result = VSTDLIB_MEM_CMP_RES_EQ;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Linearly compare buffers 4-bytes-wise using 32-bit accesses in a loop */
  for (; remaining >= 4u; remaining -= 4u) /* FETA_VSTDLIB_LOC_MEMCMP_X_X */
  {
    /* #110 If current buffer values are unequal */
    if (pBuf1_32[pos] != pBuf2_32[pos])
    {
      /* #120 Set corresponding result value and break the comparison */
      result = VStdLib_Loc_GetMemCmpNeqResult(pBuf1_32[pos], pBuf2_32[pos]);
      break;
    }
    pos++;
  }

  /* #130 If the buffers are still equal */
  if (result == VSTDLIB_MEM_CMP_RES_EQ)
  {
    /* #140 Compare the remaining bytes using byte-wise memcmp */
    result = VStdLib_Loc_MemCmp_8( /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */ /* FETA_VSTDLIB_CALL_LOC_MEMCMP_8_ALIGNED_REMAINING */
      (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pBuf1_32[pos],
      (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pBuf2_32[pos],
      remaining);
  }

  return result;
}

/**********************************************************************************************************************
 * VStdLib_Loc_MemCmp_32_Unaligned()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(sint8, VSTDLIB_CODE) VStdLib_Loc_MemCmp_32_Unaligned(
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf1_32,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf2_8,
  uint32_least nCnt,
  uint8_least nBuf2Offset)
{
  /* ----- Local Variables ---------------------------------------------- */
  sint8 result;

  /* ----- Implementation ----------------------------------------------- */

  /* #100 Compare the first bytes to reach the 32-bit boundary for buffer2 */
  result = VStdLib_Loc_MemCmp_8((P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR))&pBuf1_32[0u], /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */ /* FETA_VSTDLIB_CALL_LOC_MEMCMP_8_NBUF2OFFSET */
    pBuf2_8, nBuf2Offset);

  /* #110 If buffers are still equal */
  if (result == VSTDLIB_MEM_CMP_RES_EQ)
  {
    /* #120 Adjust pointer for buffer2 */
    P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf2_32 = \
      (P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR))&pBuf2_8[nBuf2Offset]; /* PRQA S 0310, 3305 */ /* MD_VStdLib_PtrCast */
    uint32_least buf1Pos = 1u; /* 32-bit value at position 0 was already used in comparison above */
    uint32_least buf2Pos = 0u; /* First 32-bit value in 32 bit aligned buffer2 */
    uint32_least remaining = nCnt - 4u; /* 4 bytes were already used in comparison above (not compared bytes will be stored in 'prev') */

    /* #130 Determine byte displacement properties for the 4-byte-wise comparison */

    /* Calculate byte displacement dependent shift values */
    uint8_least prevShift = (nBuf2Offset << 3u) & VSTDLIB_MASK_31;
    uint8_least xShift = 32u - prevShift;

    /* Save the not compared bytes, because of byte displacement */
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    uint32 prev = pBuf1_32[0u] >> prevShift;
#  else /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */
    uint32 prev = pBuf1_32[0u] << prevShift;
#  endif

    /* #140 Linearly compare buffers 4-byte-wise using 32-bit accesses in a loop by correcting the byte displacement of buffer1 and buffer2 */
    for (; remaining >= 4u; remaining -= 4u) /* FETA_VSTDLIB_LOC_MEMCMP_32_UNALIGNED */
    {
      uint32 x0, x1;
      x1 = pBuf1_32[buf1Pos]; /* Save current 32-bit value */

      /* Compare value: not compared bytes from previous comparison 'prev' plus bytes from current 32-bit value */
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
      x0 = prev | (x1 << xShift);
#  else /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */
      x0 = prev | (x1 >> xShift);
#  endif

      /* #150 If current buffer values are unequal */
      if (x0 != pBuf2_32[buf2Pos])
      {
        /* #160 Set corresponding result value and break the comparison */
        result = VStdLib_Loc_GetMemCmpNeqResult(x0, pBuf2_32[buf2Pos]);
        break;
      }
      else
      {
        buf1Pos++;
        buf2Pos++;

        /* Save the not compared bytes, because of byte displacement */
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
        prev = x1 >> prevShift;
#  else /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */
        prev = x1 << prevShift;
#  endif
      }
    }

    /* #170 If the buffers are still equal */
    if (result == VSTDLIB_MEM_CMP_RES_EQ)
    {
      /* #180 Compare the remaining bytes from the byte displacement correction using byte-wise memcmp */
      /* At this point (4 - nBuf2Offset) bytes of buffer2 are not compared, which are contained in 'prev' from buffer1 */
      pBuf2_8 = (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR))&pBuf2_32[buf2Pos]; /* PRQA S 1338 */ /* MD_VStdLib_ParamMod */
      result = VStdLib_Loc_MemCmp_8((P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR))&prev, /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */ /* FETA_VSTDLIB_CALL_LOC_MEMCMP_8_CALC */
        pBuf2_8, (4u - (uint32_least)nBuf2Offset));
    }

    /* #190 If the buffers are still equal */
    /* After the last 32-bit alignment of buffer1, there may be 0 to 3 bytes that were not compared by the 4-byte-wise comparison */
    if (result == VSTDLIB_MEM_CMP_RES_EQ)
    {
      /* #200 Compare the remaining bytes using byte-wise memcmp */
      result = VStdLib_Loc_MemCmp_8( /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */ /* FETA_VSTDLIB_CALL_LOC_MEMCMP_8_UNALIGNED_REMAINING */
        (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pBuf1_32[buf1Pos], /* First byte after last 32-bit alignment */
        &pBuf2_8[(4u - nBuf2Offset)], /* First byte after 'prev' comparison */
        remaining);
    }
  }

  return result;
}

# endif /* VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON  */
#endif /* VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON */


#if (VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON)
# if (VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON)

/**********************************************************************************************************************
 * VStdLib_Loc_MemCpySwapEndian_IntType_32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndian_IntType_32(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  uint8_least nCnt,
  uint8 intType)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Execute integer type dependent copying and swapping */
  switch (intType)
  {
  case VSTDLIB_INTTYPE_16:
    VStdLib_Loc_MemCpySwapEndian_Int16_32( /* SBSW_VSTDLIB_CALL_UNMODIFIED */ /* FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INTX_X */
      (P2VAR(uint16, AUTOMATIC, VSTDLIB_VAR_FAR))pDst, /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
      pSrc8,
      nCnt);
    break;

  case VSTDLIB_INTTYPE_32:
    VStdLib_Loc_MemCpySwapEndian_Int32_32( /* SBSW_VSTDLIB_CALL_UNMODIFIED */ /* FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INTX_X */
      (P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR))pDst, /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
      pSrc8,
      nCnt);
    break;

#  if defined (PLATFORM_SUPPORT_SINT64_UINT64)
  case VSTDLIB_INTTYPE_64:
    VStdLib_Loc_MemCpySwapEndian_Int64_32( /* SBSW_VSTDLIB_CALL_UNMODIFIED */ /* FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INTX_X */
      (P2VAR(uint64, AUTOMATIC, VSTDLIB_VAR_FAR))pDst, /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
      pSrc8,
      nCnt);
    break;
#  endif

  default:
    /* #20 Do not copy and swap for any unsupported intType */
#  if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
    errorId = VSTDLIB_E_PARAM_INTTYPE;
#  endif
    break;
  }
  /* ----- Development Error Report ------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_MEM_COPY_SWAP_ENDIAN, errorId);
  }
#  else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif
} /* VStdLib_Loc_MemCpySwapEndian_8_32() */

/**********************************************************************************************************************
 * VStdLib_Loc_MemCpySwapEndian_Int16_32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
/* PRQA S 6030 1 */ /* MD_VStdLib_Metrics */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndian_Int16_32(
  P2VAR(uint16, AUTOMATIC, VSTDLIB_VAR_FAR) pDst16,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  uint8_least nCnt)
{
  /* ----- Implementation ----------------------------------------------- */
#  if (VSTDLIB_USE_JUMPTABLES == STD_ON)

  /* #10 Copy and swap up to 32 bytes using a jump table */
  /* PRQA S 2003 VSTDLIB_NO_CASE_BREAK */ /* MD_VStdLib_NoCaseBreak */
  switch (nCnt)
  {
  case 32:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[30], &pDst16[15]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 30:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[28], &pDst16[14]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 28:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[26], &pDst16[13]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 26:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[24], &pDst16[12]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 24:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[22], &pDst16[11]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 22:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[20], &pDst16[10]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 20:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[18], &pDst16[9]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 18:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[16], &pDst16[8]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 16:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[14], &pDst16[7]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 14:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[12], &pDst16[6]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 12:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[10], &pDst16[5]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 10:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[8], &pDst16[4]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 8:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[6], &pDst16[3]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 6:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[4], &pDst16[2]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 4:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[2], &pDst16[1]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 2:
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[0], &pDst16[0]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  default:
    /* nothing to do */
    break;
  }
  /* PRQA L:VSTDLIB_NO_CASE_BREAK */

#  else /* VSTDLIB_USE_JUMPTABLES == STD_OFF */

  uint8_least idxSrc, idxDst = 0u;

  /* #20 Linearly copy and swap the bytes using a loop */
  for (idxSrc = 0u; idxSrc < nCnt; idxSrc += 2u) /* FETA_VSTDLIB_LOC_MEMCPYSWAPENDIAN_X_X */
  {
    VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian(&pSrc8[idxSrc], &pDst16[idxDst]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    idxDst++;
  }

#  endif /* VSTDLIB_USE_JUMPTABLES */
} /* VStdLib_Loc_MemCpySwapEndian_Int16_32() */

/**********************************************************************************************************************
 * VStdLib_Loc_MemCpySwapEndian_Int32_32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndian_Int32_32(
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  uint8_least nCnt)
{
  /* ----- Implementation ----------------------------------------------- */
#  if (VSTDLIB_USE_JUMPTABLES == STD_ON)

  /* #10 Copy and swap up to 32 bytes using a jump table */
  /* PRQA S 2003 VSTDLIB_NO_CASE_BREAK */ /* MD_VStdLib_NoCaseBreak */
  switch (nCnt)
  {
  case 32:
    VStdLib_Loc_ConvertUint8ArrayToUint32SwapEndian(&pSrc8[28], &pDst32[7]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 28:
    VStdLib_Loc_ConvertUint8ArrayToUint32SwapEndian(&pSrc8[24], &pDst32[6]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 24:
    VStdLib_Loc_ConvertUint8ArrayToUint32SwapEndian(&pSrc8[20], &pDst32[5]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 20:
    VStdLib_Loc_ConvertUint8ArrayToUint32SwapEndian(&pSrc8[16], &pDst32[4]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 16:
    VStdLib_Loc_ConvertUint8ArrayToUint32SwapEndian(&pSrc8[12], &pDst32[3]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 12:
    VStdLib_Loc_ConvertUint8ArrayToUint32SwapEndian(&pSrc8[8], &pDst32[2]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 8:
    VStdLib_Loc_ConvertUint8ArrayToUint32SwapEndian(&pSrc8[4], &pDst32[1]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 4:
    VStdLib_Loc_ConvertUint8ArrayToUint32SwapEndian(&pSrc8[0], &pDst32[0]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  default:
    /* nothing to do */
    break;
  }
  /* PRQA L:VSTDLIB_NO_CASE_BREAK */

#  else /* VSTDLIB_USE_JUMPTABLES == STD_OFF */

  uint8_least idxSrc, idxDst = 0u;

  /* #20 Linearly copy and swap the bytes using a loop */
  for (idxSrc = 0u; idxSrc < nCnt; idxSrc += 4u) /* FETA_VSTDLIB_LOC_MEMCPYSWAPENDIAN_X_X */
  {
    VStdLib_Loc_ConvertUint8ArrayToUint32SwapEndian(&pSrc8[idxSrc], &pDst32[idxDst]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    idxDst++;
  }

#  endif /* VSTDLIB_USE_JUMPTABLES */
} /* VStdLib_Loc_MemCpySwapEndian_Int32_32() */

#  if defined (PLATFORM_SUPPORT_SINT64_UINT64)

/**********************************************************************************************************************
 * VStdLib_Loc_MemCpySwapEndian_Int64_32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndian_Int64_32(
  P2VAR(uint64, AUTOMATIC, VSTDLIB_VAR_FAR) pDst64,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  uint8_least nCnt)
{
  /* ----- Implementation ----------------------------------------------- */
#   if (VSTDLIB_USE_JUMPTABLES == STD_ON)

  /* #10 Copy and swap up to 32 bytes using a jump table */
  /* PRQA S 2003 VSTDLIB_NO_CASE_BREAK */ /* MD_VStdLib_NoCaseBreak */
  switch (nCnt)
  {
  case 32:
    VStdLib_Loc_ConvertUint8ArrayToUint64SwapEndian(&pSrc8[24], &pDst64[3]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 24:
    VStdLib_Loc_ConvertUint8ArrayToUint64SwapEndian(&pSrc8[16], &pDst64[2]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 16:
    VStdLib_Loc_ConvertUint8ArrayToUint64SwapEndian(&pSrc8[8], &pDst64[1]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  case 8:
    VStdLib_Loc_ConvertUint8ArrayToUint64SwapEndian(&pSrc8[0], &pDst64[0]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    /* intentionally no break */
  default:
    /* nothing to do */
    break;
  }
  /* PRQA L:VSTDLIB_NO_CASE_BREAK */

#  else /* VSTDLIB_USE_JUMPTABLES == STD_OFF */

  uint8_least idxSrc, idxDst = 0u;

  /* #20 Linearly copy and swap the bytes using a loop */
  for (idxSrc = 0u; idxSrc < nCnt; idxSrc += 8u) /* FETA_VSTDLIB_LOC_MEMCPYSWAPENDIAN_X_X */
  {
    VStdLib_Loc_ConvertUint8ArrayToUint64SwapEndian(&pSrc8[idxSrc], &pDst64[idxDst]); /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
    idxDst++;
  }

#   endif /* VSTDLIB_USE_JUMPTABLES */
} /* VStdLib_Loc_MemCpySwapEndianInt64_8_32() */

#  endif /* defined PLATFORM_SUPPORT_SINT64_UINT64 */

/**********************************************************************************************************************
 * VStdLib_Loc_MemCpySwapEndian_32_Aligned()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndian_32_Aligned(
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc32,
  uint32_least nCnt,
  uint8 intType)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Execute integer type dependent copying and swapping */
  switch (intType)
  {
  case VSTDLIB_INTTYPE_16:
    VStdLib_Loc_MemCpySwapEndianInt16_32_Aligned(pDst32, pSrc32, nCnt); /* SBSW_VSTDLIB_CALL_UNMODIFIED */ /* FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INTX_X */
    break;

  case VSTDLIB_INTTYPE_32:
    VStdLib_Loc_MemCpySwapEndianInt32_32_Aligned(pDst32, pSrc32, nCnt); /* SBSW_VSTDLIB_CALL_UNMODIFIED */ /* FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INTX_X */
    break;

#  if defined (PLATFORM_SUPPORT_SINT64_UINT64)
  case VSTDLIB_INTTYPE_64:
    VStdLib_Loc_MemCpySwapEndianInt64_32_Aligned(pDst32, pSrc32, nCnt); /* SBSW_VSTDLIB_CALL_UNMODIFIED */ /* FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INTX_X */
    break;
#  endif

  default:
    /* #20 Do not copy and swap for any unsupported intType */
#  if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
    errorId = VSTDLIB_E_PARAM_INTTYPE;
#  endif
    break;
  }
  /* ----- Development Error Report ------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_MEM_COPY_SWAP_ENDIAN, errorId);
  }
#  else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#  endif
} /* VStdLib_Loc_MemCpySwapEndian_32_Aligned() */

/**********************************************************************************************************************
 * VStdLib_Loc_MemCpySwapEndianInt16_32_Aligned()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndianInt16_32_Aligned(
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc32,
  uint32_least nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32_least dPos = 0u; /* == sPos */
  uint32_least remaining;
  uint32 x0, x1, x2, x3;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Linearly copy and swap 16-byte blocks using 32-bit accesses in a loop */
  for (remaining = nCnt; remaining >= 16u; remaining -= 16u) /* FETA_VSTDLIB_LOC_MEMCPYSWAPENDIAN_X_X */
  {
    x0 = VStdLib_Loc_SwapEndianInt16_32(pSrc32[dPos]);
    x1 = VStdLib_Loc_SwapEndianInt16_32(pSrc32[dPos + 1u]);
    x2 = VStdLib_Loc_SwapEndianInt16_32(pSrc32[dPos + 2u]);
    x3 = VStdLib_Loc_SwapEndianInt16_32(pSrc32[dPos + 3u]);
    pDst32[dPos] = x0; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 1u] = x1; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 2u] = x2; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 3u] = x3; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    dPos += 4u;
  }

#  if (VSTDLIB_USE_JUMPTABLES == STD_ON)

  /* #20 Copy and swap the remaining block using a jump table */
  /* Only a multiple of VSTDLIB_INTTYPE_16 is possible (precondition) - remaining count can be only 14, 12, 10, 8, 6, 4, 2 or 0 here */
  /* PRQA S 2003 VSTDLIB_NO_CASE_BREAK */ /* MD_VStdLib_NoCaseBreak */
  switch (remaining)
  {
  case 12:
    pDst32[dPos] = VStdLib_Loc_SwapEndianInt16_32(pSrc32[dPos]); /* SBSW_VSTDLIB_ACCESS_32_JUMPTABLE */
    dPos++;
    /* intentionally no break */
  case 8:
    pDst32[dPos] = VStdLib_Loc_SwapEndianInt16_32(pSrc32[dPos]); /* SBSW_VSTDLIB_ACCESS_32_JUMPTABLE */
    dPos++;
    /* intentionally no break */
  case 4:
    pDst32[dPos] = VStdLib_Loc_SwapEndianInt16_32(pSrc32[dPos]); /* SBSW_VSTDLIB_ACCESS_32_JUMPTABLE */
    break;
  default: /* case 14, 10, 6, 2, 0 - 32-bit access too complex due to the next two 16-bit values are shared over two different 32-bit source values */
    /* Destination is also expected to be 16-bit aligned as described in precondition */
    VStdLib_Loc_MemCpySwapEndian_Int16_32( /* SBSW_VSTDLIB_CALL_REMAINING */ /* FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INT16_32_REMAINING */
      (P2VAR(uint16, AUTOMATIC, VSTDLIB_VAR_FAR)) &pDst32[dPos], /* PRQA S 0310 */ /* MD_VStdLib_PtrCast */
      (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pSrc32[dPos],
      (uint8_least)remaining);
    break;
  }
  /* PRQA L:VSTDLIB_NO_CASE_BREAK */

#  else /* VSTDLIB_USE_JUMPTABLES == STD_OFF */

  /* #30 Copy and swap the remaining block using a 16-bit-wise algorithm */
  /* Destination is also expected to be 16-bit aligned as described in precondition */
  VStdLib_Loc_MemCpySwapEndian_Int16_32( /* SBSW_VSTDLIB_CALL_REMAINING */ /* FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INT16_32_REMAINING */
    (P2VAR(uint16, AUTOMATIC, VSTDLIB_VAR_FAR)) &pDst32[dPos], /* PRQA S 0310 */ /* MD_VStdLib_PtrCast */
    (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pSrc32[dPos],
    (uint8_least)remaining);

#  endif /* VSTDLIB_USE_JUMPTABLES */
} /* VStdLib_Loc_MemCpySwapEndianInt16_32_Aligned */

/**********************************************************************************************************************
 * VStdLib_Loc_SwapEndianInt16_32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(uint32, VSTDLIB_CODE) VStdLib_Loc_SwapEndianInt16_32(uint32 src32)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32 dst;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Swap endianness of two 16-bit values in a 32-bit value */
  dst = VStdLib_SwapEndianUint16((uint16)((src32 & VSTDLIB_MASK_UPPER_TWO_BYTES) >> 16u));
  dst = ((uint32)dst << 16u) | (uint32)VStdLib_SwapEndianUint16((uint16)(src32 & VSTDLIB_MASK_LOWER_TWO_BYTES));

  return dst;
} /* VStdLib_Loc_SwapEndianInt16_32 */

/**********************************************************************************************************************
 * VStdLib_Loc_MemCpySwapEndianInt32_32_Aligned()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndianInt32_32_Aligned(
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc32,
  uint32_least nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32_least dPos = 0u; /* == sPos */
  uint32_least remaining;
  uint32 x0, x1, x2, x3;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Linearly copy and swap 16-byte blocks using 32-bit accesses in a loop */
  for (remaining = nCnt; remaining >= 16u; remaining -= 16u) /* FETA_VSTDLIB_LOC_MEMCPYSWAPENDIAN_X_X */
  {
    x0 = VStdLib_SwapEndianUint32(pSrc32[dPos]);
    x1 = VStdLib_SwapEndianUint32(pSrc32[dPos + 1u]);
    x2 = VStdLib_SwapEndianUint32(pSrc32[dPos + 2u]);
    x3 = VStdLib_SwapEndianUint32(pSrc32[dPos + 3u]);
    pDst32[dPos] = x0; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 1u] = x1; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 2u] = x2; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 3u] = x3; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    dPos += 4u;
  }

#  if (VSTDLIB_USE_JUMPTABLES == STD_ON)

  /* #20 Copy and swap the remaining block using a jump table */
  /* Only a multiple of VSTDLIB_INTTYPE_32 is possible (precondition) - remaining count can be only 12, 8, 4 or 0 here */
  /* PRQA S 2003 VSTDLIB_NO_CASE_BREAK */ /* MD_VStdLib_NoCaseBreak */
  switch (remaining)
  {
  case 12:
    pDst32[dPos] = VStdLib_SwapEndianUint32(pSrc32[dPos]); /* SBSW_VSTDLIB_ACCESS_32_JUMPTABLE */
    dPos++;
    /* intentionally no break */
  case 8:
    pDst32[dPos] = VStdLib_SwapEndianUint32(pSrc32[dPos]); /* SBSW_VSTDLIB_ACCESS_32_JUMPTABLE */
    dPos++;
    /* intentionally no break */
  case 4:
    pDst32[dPos] = VStdLib_SwapEndianUint32(pSrc32[dPos]); /* SBSW_VSTDLIB_ACCESS_32_JUMPTABLE */
    /* intentionally no break */
  default:
    /* nothing to do */
    break;
  }
  /* PRQA L:VSTDLIB_NO_CASE_BREAK */

#  else /* VSTDLIB_USE_JUMPTABLES == STD_OFF */

  /* #30 Copy and swap the remaining block using a 32-bit-wise algorithm */
  VStdLib_Loc_MemCpySwapEndian_Int32_32( /* SBSW_VSTDLIB_CALL_REMAINING */ /* FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INT32_32_REMAINING */
    &pDst32[dPos],
    (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pSrc32[dPos],
    (uint8_least)remaining);

#  endif /* VSTDLIB_USE_JUMPTABLES */
}

#  if defined (PLATFORM_SUPPORT_SINT64_UINT64)

/**********************************************************************************************************************
 * VStdLib_Loc_MemCpySwapEndianInt64_32_Aligned()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndianInt64_32_Aligned(
  P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc32,
  uint32_least nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32_least dPos = 0u; /* == sPos */
  uint32_least remaining;
  uint32 x0, x1, x2, x3;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Linearly copy and swap 16-byte blocks using 32-bit accesses in a loop */
  for (remaining = nCnt; remaining >= 16u; remaining -= 16u) /* FETA_VSTDLIB_LOC_MEMCPYSWAPENDIAN_X_X */
  {
    /* 64-bit value is shared over two 32-bit values - swap each 32-bit value and then the order of both values */
    /* 64-bit value */
    x1 = VStdLib_SwapEndianUint32(pSrc32[dPos]);
    x0 = VStdLib_SwapEndianUint32(pSrc32[dPos + 1u]);

    /* 64-bit value */
    x3 = VStdLib_SwapEndianUint32(pSrc32[dPos + 2u]);
    x2 = VStdLib_SwapEndianUint32(pSrc32[dPos + 3u]);

    pDst32[dPos] = x0; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 1u] = x1; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 2u] = x2; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    pDst32[dPos + 3u] = x3; /* SBSW_VSTDLIB_ACCESS_32_LOOP */
    dPos += 4u;
  }

#   if (VSTDLIB_USE_JUMPTABLES == STD_ON)

  /* #20 Copy and swap the remaining block using a jump table */
  /* Only a multiple of VSTDLIB_INTTYPE_64 is possible (precondition) - remaining count can be only 8 or 0 here */
  /* PRQA S 2003 VSTDLIB_NO_CASE_BREAK */ /* MD_VStdLib_NoCaseBreak */
  switch (remaining)
  {
  case 8:
    pDst32[dPos + 1u] = VStdLib_SwapEndianUint32(pSrc32[dPos]); /* SBSW_VSTDLIB_ACCESS_32_JUMPTABLE */
    pDst32[dPos] = VStdLib_SwapEndianUint32(pSrc32[dPos + 1u]); /* SBSW_VSTDLIB_ACCESS_32_JUMPTABLE */
    /* intentionally no break */
  default:
    /* nothing to do */
    break;
  }
  /* PRQA L:VSTDLIB_NO_CASE_BREAK */

#   else /* VSTDLIB_USE_JUMPTABLES == STD_OFF */

  /* #30 Copy and swap the remaining block using a 64-bit-wise algorithm */
  /* Destination is also expected to be 64-bit aligned as described in precondition */
  VStdLib_Loc_MemCpySwapEndian_Int64_32( /* SBSW_VSTDLIB_CALL_REMAINING */ /* FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INT64_32_REMAINING */
    (P2VAR(uint64, AUTOMATIC, VSTDLIB_VAR_FAR)) &pDst32[dPos], /* PRQA S 0310, 3305 */ /* MD_VStdLib_PtrCast */
    (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pSrc32[dPos],
    (uint8_least)remaining);

#   endif /* VSTDLIB_USE_JUMPTABLES */
} /* VStdLib_Loc_MemCpySwapEndianInt64_32_Aligned */

#  endif /* defined PLATFORM_SUPPORT_SINT64_UINT64 */
# endif /* VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON */ 

/**********************************************************************************************************************
 * VStdLib_Loc_MemCpySwapEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_MemCpySwapEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  uint32_least nCnt,
  uint8 intType)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint32_least idxSrc, idxDst = 0u;
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Linearly copy and swap nCnt bytes using a loop */
  switch (intType)
  {
  case VSTDLIB_INTTYPE_16:
  {
    P2VAR(uint16, AUTOMATIC, VSTDLIB_VAR_FAR) pDst16 = \
      (P2VAR(uint16, AUTOMATIC, VSTDLIB_VAR_FAR)) pDst; /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */

    for (idxSrc = 0u; idxSrc < nCnt; idxSrc += 2u) /* FETA_VSTDLIB_LOC_MEMCPYSWAPENDIAN_X_X */
    {
      VStdLib_Loc_ConvertUint8ArrayToUint16SwapEndian( /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
        &pSrc8[idxSrc],
        &pDst16[idxDst]);

      idxDst++;
    }
  }
  break;

  case VSTDLIB_INTTYPE_32:
  {
    P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst32 = \
      (P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR)) pDst; /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */

    for (idxSrc = 0u; idxSrc < nCnt; idxSrc += 4u) /* FETA_VSTDLIB_LOC_MEMCPYSWAPENDIAN_X_X */
    {
      VStdLib_Loc_ConvertUint8ArrayToUint32SwapEndian( /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
        &pSrc8[idxSrc],
        &pDst32[idxDst]);

      idxDst++;
    }
  }
  break;

# if defined (PLATFORM_SUPPORT_SINT64_UINT64)
  case VSTDLIB_INTTYPE_64:
  {
    P2VAR(uint64, AUTOMATIC, VSTDLIB_VAR_FAR) pDst64 = \
      (P2VAR(uint64, AUTOMATIC, VSTDLIB_VAR_FAR)) pDst; /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */

    for (idxSrc = 0u; idxSrc < nCnt; idxSrc += 8u) /* FETA_VSTDLIB_LOC_MEMCPYSWAPENDIAN_X_X */
    {
      VStdLib_Loc_ConvertUint8ArrayToUint64SwapEndian( /* SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT */
        &pSrc8[idxSrc],
        &pDst64[idxDst]);

      idxDst++;
    }
  }
  break;
# endif /* defined PLATFORM_SUPPORT_SINT64_UINT64 */

  default:
    /* #20 Do not copy and swap for any unsupported intType */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
    errorId = VSTDLIB_E_PARAM_INTTYPE;
# endif
    break;
  }
  /* ----- Development Error Report ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_MEM_COPY_SWAP_ENDIAN, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
} /* VStdLib_Loc_MemCpySwapEndian() */

#endif /* VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON */

/**********************************************************************************************************************
 *   VStdLib_Loc_BitCpyConvertBitCounting()
 **********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(VStdLib_CntType, VSTDLIB_CODE) VStdLib_Loc_BitCpyConvertBitCounting(VStdLib_CntType bitPos)
{
  VStdLib_CntType bitPosConverted = (VStdLib_CntType)((7u - (bitPos & 7u)) + (8u * (bitPos >> 3u)));

  return bitPosConverted;
}

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyGetLastIterByte()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(VStdLib_CntType, VSTDLIB_CODE) VStdLib_Loc_BitCpyGetLastIterByte(
  VStdLib_CntType lengthInBits)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #100 Calculate the last iteration byte from the length in bits */
  VStdLib_CntType lastIterByte = lengthInBits / 8u;

  /* #110 If the length in bits is not divisible by 8, increment the last iteration byte by 1 */
  if ((lengthInBits % 8u) != 0u)
  {
    lastIterByte++;
  }

  return lastIterByte;
}

#if ((VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON) || \
     (VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON))

/**********************************************************************************************************************
 *   VStdLib_Loc_BitCpyIter()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyIter(
  P2VAR(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) pIter,
  VStdLib_CntType nBitOffset,
  VStdLib_CntType nCnt)
{
  /* ----- Implementation ----------------------------------------------- */
  /*  #10 Initialize all iteration parameters */
  pIter->startByte = (VStdLib_CntType)(nBitOffset / 8u); /* SBSW_VSTDLIB_ACCESS_STRUCT_LOCAL */
  pIter->lastByte = VStdLib_Loc_BitCpyGetLastIterByte(nBitOffset + nCnt); /* SBSW_VSTDLIB_ACCESS_STRUCT_LOCAL */
  pIter->bitOffsetStartByte = (uint8)(nBitOffset % 8u); /* SBSW_VSTDLIB_ACCESS_STRUCT_LOCAL */
  /* converts monotone to sawtooth bit position and vice versa */
  pIter->bitOffsetEndByte = VStdLib_Loc_BitCpyGetOffsetInBitsEndByte(pIter->bitOffsetStartByte + nCnt); /* SBSW_VSTDLIB_ACCESS_STRUCT_LOCAL */
}

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyGetOffsetInBitsEndByte()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_BitCpyGetOffsetInBitsEndByte(VStdLib_CntType lengthInBits)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 lengthMod = (uint8)(lengthInBits % 8u);
  uint8 offset = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 If the bit length is a multiple of 8 */
  if (lengthMod != 0u)
  {
    /* #110 Calculate the offset in last byte to copy to or from */
    offset = 8u - lengthMod;
  }

  return offset;
}

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyBigEndianSrc_7()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyBigEndianSrc_7( /* PRQA S 6060 */ /* MD_MSR_STPAR */
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  VStdLib_CntType dstByteIdx,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift,
  uint8 dstBitMask)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read source byte */
  uint8 srcByte = VStdLib_Loc_BitCpyBigEndianSrc(
    pSrc8,
    srcByteIdx,
    srcIter,
    srcShift); /* SBSW_VSTDLIB_CALL_UNMODIFIED */

  /* #20 Read destination byte */
  uint8 tmpByte = pDst8[dstByteIdx];

  /* #30 Get relevant bits to copy */
  srcByte &= dstBitMask;
  tmpByte &= (~dstBitMask);
  tmpByte |= srcByte;

  /* #40 Copy the bits */
  pDst8[dstByteIdx] = tmpByte; /* SBSW_VSTDLIB_ACCESS_8 */
}

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyLittleEndianSrc_7()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyLittleEndianSrc_7( /* PRQA S 6060 */ /* MD_MSR_STPAR */
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  VStdLib_CntType dstByteIdx,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift,
  uint8 dstBitMask)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 Read source byte */
  uint8 srcByte = VStdLib_Loc_BitCpyLittleEndianSrc(
    pSrc8,
    srcByteIdx,
    srcIter,
    srcShift); /* SBSW_VSTDLIB_CALL_UNMODIFIED */

  /* #20 Read destination byte */
  uint8 tmpByte = pDst8[dstByteIdx];

  /* #30 Get relevant bits to copy */
  srcByte &= dstBitMask;
  tmpByte &= (~dstBitMask);
  tmpByte |= srcByte;

  /* #40 Copy the bits */
  pDst8[dstByteIdx] = tmpByte; /* SBSW_VSTDLIB_ACCESS_8 */
}

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyBigEndianSrc()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_BitCpyBigEndianSrc(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 srcByte = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check parameter 'srcByteIdx' against the index of the last iteration source byte */
  if (srcByteIdx < srcIter->lastByte)
  {
    srcByte = pSrc8[srcByteIdx];
  }
  /* #20 If the source shift is not 0 */
  if (srcShift != 0)
  {
    /* #30 If the source shift is bigger than 0, use VStdLib_Loc_BitCpyShiftRight_BigEndianSrc()
     *     to shift the source byte to the right and merge with missing bits from the previous relevant source byte */
    if (srcShift > 0)
    {
      VStdLib_Loc_BitCpyShiftRight_BigEndianSrc(pSrc8, &srcByte, srcByteIdx, srcIter, srcShift); /* SBSW_VSTDLIB_CALL_MODIFIABLE_SRCBYTE */
    }
    /* #40 Else, use VStdLib_Loc_BitCpyShiftLeft_BigEndianSrc() to shift the source byte to the left
     *     and merge with missing bits from the next relevant source byte */
    else
    {
      VStdLib_Loc_BitCpyShiftLeft_BigEndianSrc(pSrc8, &srcByte, srcByteIdx, srcIter, srcShift); /* SBSW_VSTDLIB_CALL_MODIFIABLE_SRCBYTE */
    }
  }

  return srcByte;
}

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyLittleEndianSrc()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_BitCpyLittleEndianSrc(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 srcByte = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check parameter 'srcByteIdx' against the index of the last iteration source byte */
  if (srcByteIdx < srcIter->lastByte)
  {
    srcByte = pSrc8[srcByteIdx];
  }
  /* #20 If the source shift is not 0 */
  if (srcShift != 0)
  {
    /* #30 If the source shift is bigger than 0, use VStdLib_Loc_BitCpyShiftRight_LittleEndianSrc()
     *     to shift the source byte to the right and merge with missing bits from the next relevant source byte */
    if (srcShift > 0)
    {
      VStdLib_Loc_BitCpyShiftRight_LittleEndianSrc(pSrc8, &srcByte, srcByteIdx, srcIter, srcShift); /* SBSW_VSTDLIB_CALL_MODIFIABLE_SRCBYTE */
    }
    /* #40 Else, use VStdLib_Loc_BitCpyShiftLeft_LittleEndianSrc() to shift the source byte to the left
     *     and merge with missing bits from the previous relevant source byte */
    else
    {
      VStdLib_Loc_BitCpyShiftLeft_LittleEndianSrc(pSrc8, &srcByte, srcByteIdx, srcIter, srcShift); /* SBSW_VSTDLIB_CALL_MODIFIABLE_SRCBYTE */
    }
  }

  return srcByte;
}

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyShiftRight_BigEndianSrc()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyShiftRight_BigEndianSrc(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) srcByte,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 unsignedScrcShift = (uint8)srcShift;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Shift the source byte to the right */
  *(srcByte) >>= unsignedScrcShift; /* SBSW_VSTDLIB_ACCESS_LOCAL */

  /* #20 If bits are missing to fill destination byte get missing bits from previous relevant source byte
   *     and merge them with current source byte */
  if (srcByteIdx > srcIter->startByte)
  {
    uint8 missingBits = (pSrc8[srcByteIdx - 1u] << (8u - unsignedScrcShift));
    *(srcByte) |= missingBits; /* SBSW_VSTDLIB_ACCESS_LOCAL */
  }
}

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyShiftLeft_BigEndianSrc()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyShiftLeft_BigEndianSrc(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) srcByte,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8   unsignedScrcShift = (uint8)(-srcShift);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Shift the source byte to the left */
  *(srcByte) <<= unsignedScrcShift; /* SBSW_VSTDLIB_ACCESS_LOCAL */

  /* #20 If bits are missing to fill destination byte get missing bits from next relevant source byte
   *     and merge them with current source byte */
  if ((srcByteIdx + 1u) < srcIter->lastByte)
  {
    uint8 missingBits = (pSrc8[srcByteIdx + 1u] >> (8u - unsignedScrcShift));
    *(srcByte) |= missingBits; /* SBSW_VSTDLIB_ACCESS_LOCAL */
  }
}

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyShiftRight_LittleEndianSrc()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyShiftRight_LittleEndianSrc(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) srcByte,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 unsignedScrcShift = (uint8)srcShift;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Shift the source byte to the right */
  *(srcByte) >>= unsignedScrcShift; /* SBSW_VSTDLIB_ACCESS_LOCAL */

  /* #20 If bits are missing to fill destination byte get missing bits from next relevant source byte
   *     and merge them with current source byte */
  if ((srcByteIdx + 1u) < srcIter->lastByte)
  {
    uint8 missingBits = (pSrc8[srcByteIdx + 1u] << (8u - unsignedScrcShift));
    *(srcByte) |= missingBits; /* SBSW_VSTDLIB_ACCESS_LOCAL */
  }
}

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyShiftLeft_LittleEndianSrc()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyShiftLeft_LittleEndianSrc(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) srcByte,
  VStdLib_CntType srcByteIdx,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter,
  sint8 srcShift)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 unsignedScrcShift = (uint8)(-srcShift);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Shift the source byte to the left */
  *(srcByte) <<= unsignedScrcShift; /* SBSW_VSTDLIB_ACCESS_LOCAL */

  /* #20 If bits are missing to fill destination byte get missing bits from previous relevant source byte
   *     and merge them with current source byte */
  if (srcByteIdx > srcIter->startByte)
  {
    uint8 missingBits = (pSrc8[srcByteIdx - 1u] >> (8u - unsignedScrcShift));
    *(srcByte) |= missingBits; /* SBSW_VSTDLIB_ACCESS_LOCAL */
  }
}

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpySwapEndian()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpySwapEndian(
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) dstIter,
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8,
  P2CONST(VStdLib_BitCpyIterType, AUTOMATIC, VSTDLIB_VAR_FAR) srcIter)
{
  VStdLib_CntType idx; /* Explicitly not initialized. Used as for-loop counter (first usage in for-initialization statement) */
  VStdLib_CntType srcIdx = srcIter->lastByte - 1u;

  for (idx = dstIter->startByte; idx < dstIter->lastByte; idx++) /* FETA_VSTDLIB_LOC_BITCPYSWAPENDIAN */
  {
    pDst8[idx] = pSrc8[srcIdx]; /* SBSW_VSTDLIB_ACCESS_LOOP_ENDIDX */
    srcIdx--;
  }
}

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyBigToBigEndian()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyBigToBigEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt)
{
  /* #10 If parameters 'nDstBitOffsetMon' and 'nSrcBitOffsetMon' are both 0 and parameter 'nCnt' is a multiple of 8 */
  if ((nDstBitOffsetMon == 0u) && (nSrcBitOffsetMon == 0u) && ((nCnt % 8u) == 0u))
  {
    VStdLib_CntType nCntInBytes = nCnt / 8u;

    /* #20 Use VStdLib_MemCpy algorithm to copy data */
    VStdLib_MemCpy(pDst, pSrc, nCntInBytes); /* SBSW_VSTDLIB_CALL_CONVERTED_BITCNT */ /* FETA_VSTDLIB_CALL_MEMCPY_IN_LOC_BITCPY */
  }
  /* #30 Else */
  else
  {
    VStdLib_BitCpyIterType srcIter; /* Struct is explicitly not initialized. Is initialized with call of VStdLib_Loc_BitCpyIter() by passing address of struct */
    VStdLib_BitCpyIterType dstIter; /* Struct is explicitly not initialized. Is initialized with call of VStdLib_Loc_BitCpyIter() by passing address of struct */
    VStdLib_CntType dstByteIdx; /* Explicitly not initialized. Is initialized after call of VStdLib_Loc_BitCpyIter() */
    VStdLib_CntType srcByteIdx; /* Explicitly not initialized. Is initialized after call of VStdLib_Loc_BitCpyIter() */
    P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8 = \
      (P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) pDst;  /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
    P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8 = \
      (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) pSrc;  /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
    sint8 srcShift = 0;
    uint8 bitMaskStart = 0xFFu;
    uint8 bitMaskEnd = 0xFFu;

    /* #40 Use VStdLib_Loc_BitCpyIter to set the iteration parameters */
    VStdLib_Loc_BitCpyIter(
      &srcIter,
      nSrcBitOffsetMon,
      nCnt); /* SBSW_VSTDLIB_CALL_MODIFIABLE_ITER */

    VStdLib_Loc_BitCpyIter(
      &dstIter,
      nDstBitOffsetMon,
      nCnt); /* SBSW_VSTDLIB_CALL_MODIFIABLE_ITER */

    dstByteIdx = dstIter.startByte;
    srcByteIdx = srcIter.startByte;

    /* #50 Set the source shift if the bit offset of the leftmost destination and source byte are not equal */
    if (dstIter.bitOffsetStartByte != srcIter.bitOffsetStartByte)
    {
      srcShift = (sint8)dstIter.bitOffsetStartByte - (sint8)srcIter.bitOffsetStartByte;
    }

    /* Set the bit masks for the first and last destination byte */
    bitMaskStart >>= dstIter.bitOffsetStartByte;
    bitMaskEnd <<= dstIter.bitOffsetEndByte;

    /* #60 If the first destination byte to copy to is equal to the last byte to copy to, copy relevant bits to only this
     *     destination byte */
    if (dstIter.startByte == (dstIter.lastByte - 1u))
    {
      /* Adjust start bit mask */
      bitMaskStart &= bitMaskEnd;

      VStdLib_Loc_BitCpyBigEndianSrc_7(
        pDst8,
        dstByteIdx,
        pSrc8,
        srcByteIdx,
        &srcIter,
        srcShift,
        bitMaskStart); /* SBSW_VSTDLIB_CALL_MODIFIED_IDX */
    }
    /* #70 Else */
    else
    {
      /* #80 Use VStdLib_Loc_BitCpyBigEndianSrc_7() to copy the first bits separately */
      VStdLib_Loc_BitCpyBigEndianSrc_7(
        pDst8,
        dstByteIdx,
        pSrc8,
        srcByteIdx,
        &srcIter,
        srcShift,
        bitMaskStart); /* SBSW_VSTDLIB_CALL_MODIFIED_IDX */

      srcByteIdx++;

      /* #90 Use VStdLib_Loc_BitCpyBigEndianSrc() to copy the bits inbetween the first and last source byte  byte-wise */
      for (dstByteIdx += 1u; dstByteIdx < (dstIter.lastByte - 1u); dstByteIdx++) /* FETA_VSTDLIB_LOC_BITCPY */
      {
        pDst8[dstByteIdx] = VStdLib_Loc_BitCpyBigEndianSrc(
          pSrc8,
          srcByteIdx,
          &srcIter,
          srcShift); /* SBSW_VSTDLIB_ACCESS_LOOP_ENDIDX */ /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */

        srcByteIdx++;
      }

      /* #100 Use VStdLib_Loc_BitCpyBigEndianSrc_7() to copy the last bits separately */
      VStdLib_Loc_BitCpyBigEndianSrc_7(
        pDst8,
        dstByteIdx,
        pSrc8,
        srcByteIdx,
        &srcIter,
        srcShift,
        bitMaskEnd); /* SBSW_VSTDLIB_CALL_MODIFIED_IDX */
    }
  }
} /* VStdLib_Loc_BitCpyBigToBigEndian() */

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyBigToLittleEndian()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyBigToLittleEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8 = \
    (P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) pDst;  /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8 = \
    (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) pSrc;  /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
  VStdLib_BitCpyIterType srcIter; /* Struct is explicitly not initialized. Is initialized with call of VStdLib_Loc_BitCpyIter() by passing address of struct */
  VStdLib_BitCpyIterType dstIter; /* Struct is explicitly not initialized. Is initialized with call of VStdLib_Loc_BitCpyIter() by passing address of struct */

  /* #10 Use VStdLib_Loc_BitCpyIter to set the iteration parameters */
  VStdLib_Loc_BitCpyIter(
    &srcIter,
    nSrcBitOffsetMon,
    nCnt); /* SBSW_VSTDLIB_CALL_MODIFIABLE_ITER */

  VStdLib_Loc_BitCpyIter(
    &dstIter,
    nDstBitOffsetSaw,
    nCnt); /* SBSW_VSTDLIB_CALL_MODIFIABLE_ITER */

  /* #20 If parameter 'nDstBitOffsetSaw' and 'nSrcBitOffsetMon' are both 0 and parameter 'nCnt' is a multiple of 8 */
  if ((nDstBitOffsetSaw == 0u) && (nSrcBitOffsetMon == 0u) && ((nCnt % 8u) == 0u))
  {
    /* #30 Use VStdLib_Loc_BitCpySwapEndian algorithm to copy data in reverse order */
    VStdLib_Loc_BitCpySwapEndian(pDst8, &dstIter, pSrc8, &srcIter); /* SBSW_VSTDLIB_CALL_MODIFIED_IDX */ /* FETA_VSTDLIB_CALL_LOC_BITCPYSWAPENDIAN */
  }
  /* #40 Else */
  else
  {
    sint8 srcShift = 0;
    VStdLib_CntType dstByteIdx = dstIter.startByte;
    VStdLib_CntType srcByteIdx = srcIter.lastByte - 1u;
    uint8 bitMaskStart = 0xFFu;
    uint8 bitMaskEnd = 0xFFu;

    /* #50 Set the source shift if the bit offset of the leftmost destination byte and the bit offset of
     *     the rightmost source byte are not equal */
    if (dstIter.bitOffsetStartByte != srcIter.bitOffsetEndByte)
    {
      srcShift = (sint8)srcIter.bitOffsetEndByte - (sint8)dstIter.bitOffsetStartByte;
    }

    /* Set the bit masks for the first and last destination byte */
    bitMaskStart <<= dstIter.bitOffsetStartByte;
    bitMaskEnd >>= dstIter.bitOffsetEndByte;

    /* #60 If the first destination byte to copy to is equal to the last byte to copy to, copy relevant bits to only this
     *     destination byte */
    if (dstIter.startByte == (dstIter.lastByte - 1u))
    {
      /* Adjust start bit mask */
      bitMaskStart &= bitMaskEnd;

      VStdLib_Loc_BitCpyBigEndianSrc_7(
        pDst8,
        dstByteIdx,
        pSrc8,
        srcByteIdx,
        &srcIter,
        srcShift,
        bitMaskStart); /* SBSW_VSTDLIB_CALL_MODIFIED_IDX */
    }
    /* #70 Else */
    else
    {
      /* #80 Use VStdLib_Loc_BitCpyBigEndianSrc_7() to copy the first bits separately */
      VStdLib_Loc_BitCpyBigEndianSrc_7(
        pDst8,
        dstByteIdx,
        pSrc8,
        srcByteIdx,
        &srcIter,
        srcShift,
        bitMaskStart); /* SBSW_VSTDLIB_CALL_MODIFIED_IDX */

      srcByteIdx--;

      /* #90 Use VStdLib_Loc_BitCpyBigEndianSrc() to copy the bits inbetween the first and last source byte  byte-wise */
      for (dstByteIdx += 1u; dstByteIdx < (dstIter.lastByte - 1u); dstByteIdx++) /* FETA_VSTDLIB_LOC_BITCPY */
      {
        pDst8[dstByteIdx] = VStdLib_Loc_BitCpyBigEndianSrc(
          pSrc8,
          srcByteIdx,
          &srcIter,
          srcShift); /* SBSW_VSTDLIB_ACCESS_LOOP_ENDIDX */ /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */

        srcByteIdx--;
      }

      /* #100 Use VStdLib_Loc_BitCpyBigEndianSrc_7() to copy the last bits separately */
      VStdLib_Loc_BitCpyBigEndianSrc_7(
        pDst8,
        dstByteIdx,
        pSrc8,
        srcByteIdx,
        &srcIter,
        srcShift,
        bitMaskEnd); /* SBSW_VSTDLIB_CALL_MODIFIED_IDX */
    }
  }
} /* VStdLib_Loc_BitCpyBigToLittleEndian() */

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyLittleToBigEndian()
 *********************************************************************************************************************/
 /*!
   * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
   */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyLittleToBigEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8 = \
    (P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) pDst;  /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8 = \
    (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) pSrc;  /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
  VStdLib_BitCpyIterType srcIter; /* Struct is explicitly not initialized. Is initialized with call of VStdLib_Loc_BitCpyIter() by passing address of struct */
  VStdLib_BitCpyIterType dstIter; /* Struct is explicitly not initialized. Is initialized with call of VStdLib_Loc_BitCpyIter() by passing address of struct */

   /* #10 Use VStdLib_Loc_BitCpyIter to set the iteration parameters */
  VStdLib_Loc_BitCpyIter(
    &srcIter,
    nSrcBitOffsetSaw,
    nCnt); /* SBSW_VSTDLIB_CALL_MODIFIABLE_ITER */

  VStdLib_Loc_BitCpyIter(
    &dstIter,
    nDstBitOffsetMon,
    nCnt); /* SBSW_VSTDLIB_CALL_MODIFIABLE_ITER */

  /* #20 If parameter 'nDstBitOffsetSaw' and 'nSrcBitOffsetMon' are both 0 and parameter 'nCnt' is a multiple of 8 */
  if ((nDstBitOffsetMon == 0u) && (nSrcBitOffsetSaw == 0u) && ((nCnt % 8u) == 0u))
  {
    /* #30 Use VStdLib_Loc_BitCpySwapEndian algorithm to copy data in reverse order */
    VStdLib_Loc_BitCpySwapEndian(pDst8, &dstIter, pSrc8, &srcIter); /* SBSW_VSTDLIB_CALL_MODIFIED_IDX */ /* FETA_VSTDLIB_CALL_LOC_BITCPYSWAPENDIAN */
  }
  /* #40 Else */
  else
  {
    sint8 srcShift = 0;
    VStdLib_CntType dstByteIdx = dstIter.startByte;
    VStdLib_CntType srcByteIdx = srcIter.lastByte - 1u;
    uint8 bitMaskStart = 0xFFu;
    uint8 bitMaskEnd = 0xFFu;

    /* #50 Set the source shift if the bit offset of the leftmost destination byte and the bit offset of
     *     the rightmost source byte are not equal */
    if (dstIter.bitOffsetStartByte != srcIter.bitOffsetEndByte)
    {
      srcShift = (sint8)dstIter.bitOffsetStartByte - (sint8)srcIter.bitOffsetEndByte;
    }

    /* Set the bit masks for the first and last destination byte */
    bitMaskStart >>= dstIter.bitOffsetStartByte;
    bitMaskEnd <<= dstIter.bitOffsetEndByte;

    /* #60 If the first destination byte to copy to is equal to the last byte to copy to, copy relevant bits to only this
     *     destination byte */
    if (dstIter.startByte == (dstIter.lastByte - 1u))
    {
      /* Adjust start bit mask */
      bitMaskStart &= bitMaskEnd;

      VStdLib_Loc_BitCpyLittleEndianSrc_7(
        pDst8,
        dstByteIdx,
        pSrc8,
        srcByteIdx,
        &srcIter,
        srcShift,
        bitMaskStart); /* SBSW_VSTDLIB_CALL_MODIFIED_IDX */
    }
    /* #70 Else */
    else
    {
      /* #80 Use VStdLib_Loc_BitCpyLittleEndianSrc_7() to copy the first bits separately */
      VStdLib_Loc_BitCpyLittleEndianSrc_7(
        pDst8,
        dstByteIdx,
        pSrc8,
        srcByteIdx,
        &srcIter,
        srcShift,
        bitMaskStart); /* SBSW_VSTDLIB_CALL_MODIFIED_IDX */

      srcByteIdx--;

      /* #90 Use VStdLib_Loc_BitCpyLittleEndianSrc() to copy the bits inbetween the first and last source byte  byte-wise */
      for (dstByteIdx += 1u; dstByteIdx < (dstIter.lastByte - 1u); dstByteIdx++) /* FETA_VSTDLIB_LOC_BITCPY */
      {
        pDst8[dstByteIdx] = VStdLib_Loc_BitCpyLittleEndianSrc(
          pSrc8,
          srcByteIdx,
          &srcIter,
          srcShift); /* SBSW_VSTDLIB_ACCESS_LOOP_ENDIDX */ /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */

        srcByteIdx--;
      }

      /* #100 Use VStdLib_Loc_BitCpyLittleEndianSrc_7()  to copy the last bits separately */
      VStdLib_Loc_BitCpyLittleEndianSrc_7(
        pDst8,
        dstByteIdx,
        pSrc8,
        srcByteIdx,
        &srcIter,
        srcShift,
        bitMaskEnd); /* SBSW_VSTDLIB_CALL_MODIFIED_IDX */
    }
  }
} /* VStdLib_Loc_BitCpyLittleToBigEndian() */

/**********************************************************************************************************************
 *  VStdLib_Loc_BitCpyLittleToLittleEndian()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
  */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_BitCpyLittleToLittleEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt)
{
  /* #10 If parameters 'nDstBitOffsetSaw' and 'nSrcBitOffsetSaw' are both 0 and parameter 'nCnt' is a multiple of 8 */
  if ((nDstBitOffsetSaw == 0u) && (nSrcBitOffsetSaw == 0u) && ((nCnt % 8u) == 0u))
  {
    VStdLib_CntType nCntInBytes = nCnt / 8u;

    /* #20 Use VStdLib_MemCpy algorithm to copy data */
    VStdLib_MemCpy(pDst, pSrc, nCntInBytes); /* SBSW_VSTDLIB_CALL_CONVERTED_BITCNT */ /* FETA_VSTDLIB_CALL_MEMCPY_IN_LOC_BITCPY */
  }
  /* #30 Else */
  else
  {
    VStdLib_BitCpyIterType srcIter; /* Struct is explicitly not initialized. Is initialized with call of VStdLib_Loc_BitCpyIter() by passing address of struct */
    VStdLib_BitCpyIterType dstIter; /* Struct is explicitly not initialized. Is initialized with call of VStdLib_Loc_BitCpyIter() by passing address of struct */
    VStdLib_CntType dstByteIdx; /* Explicitly not initialized. Is initialized after call of VStdLib_Loc_BitCpyIter() */
    VStdLib_CntType srcByteIdx; /* Explicitly not initialized. Is initialized after call of VStdLib_Loc_BitCpyIter() */
    P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8 = \
      (P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) pDst;  /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
    P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8 = \
      (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) pSrc;  /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
    sint8 srcShift = 0;
    uint8 bitMaskStart = 0xFFu;
    uint8 bitMaskEnd = 0xFFu;

    /* #40 Use VStdLib_Loc_BitCpyIter to set the iteration parameters */
    VStdLib_Loc_BitCpyIter(
      &srcIter,
      nSrcBitOffsetSaw,
      nCnt); /* SBSW_VSTDLIB_CALL_MODIFIABLE_ITER */

    VStdLib_Loc_BitCpyIter(
      &dstIter,
      nDstBitOffsetSaw,
      nCnt); /* SBSW_VSTDLIB_CALL_MODIFIABLE_ITER */

    dstByteIdx = dstIter.startByte;
    srcByteIdx = srcIter.startByte;


    /* #50 Set the source shift if the bit offset of the leftmost destination and source byte are not equal */
    if (dstIter.bitOffsetStartByte != srcIter.bitOffsetStartByte)
    {
      srcShift = (sint8)srcIter.bitOffsetStartByte - (sint8)dstIter.bitOffsetStartByte;
    }

    /* Set the bit masks for the first and last destination byte */
    bitMaskStart <<= dstIter.bitOffsetStartByte;
    bitMaskEnd >>= dstIter.bitOffsetEndByte;

    /* #60 If the first destination byte to copy to is equal to the last byte to copy to, copy relevant bits to only this
     *     destination byte */
    if (dstIter.startByte == (dstIter.lastByte - 1u))
    {
      /* Adjust start bit mask */
      bitMaskStart &= bitMaskEnd;

      VStdLib_Loc_BitCpyLittleEndianSrc_7(
        pDst8,
        dstByteIdx,
        pSrc8,
        srcByteIdx,
        &srcIter,
        srcShift,
        bitMaskStart); /* SBSW_VSTDLIB_CALL_MODIFIED_IDX */
    }
    /* #70 Else */
    else
    {
      /* #80 Use VStdLib_Loc_BitCpyLittleEndianSrc_7() to copy the first bits separately */
      VStdLib_Loc_BitCpyLittleEndianSrc_7(
        pDst8,
        dstByteIdx,
        pSrc8,
        srcByteIdx,
        &srcIter,
        srcShift,
        bitMaskStart); /* SBSW_VSTDLIB_CALL_MODIFIED_IDX */

      srcByteIdx++;

      /* #90 Use VStdLib_Loc_BitCpyLittleEndianSrc() to copy the bits inbetween the first and last source byte  byte-wise */
      for (dstByteIdx += 1u; dstByteIdx < (dstIter.lastByte - 1u); dstByteIdx++) /* FETA_VSTDLIB_LOC_BITCPY */
      {
        pDst8[dstByteIdx] = VStdLib_Loc_BitCpyLittleEndianSrc(
          pSrc8,
          srcByteIdx,
          &srcIter,
          srcShift); /* SBSW_VSTDLIB_ACCESS_LOOP_ENDIDX */ /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */

        srcByteIdx++;
      }

      /* #100 Use VStdLib_Loc_BitCpyLittleEndianSrc_7() to copy the last bits separately */
      VStdLib_Loc_BitCpyLittleEndianSrc_7(
        pDst8,
        dstByteIdx,
        pSrc8,
        srcByteIdx,
        &srcIter,
        srcShift,
        bitMaskEnd); /* SBSW_VSTDLIB_CALL_MODIFIED_IDX */
    }
  }
} /* VStdLib_Loc_BitCpyLittleToLittleEndian() */

#endif /* (VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON) || \
          (VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON) */

#if (VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION != STD_ON)

/**********************************************************************************************************************
* VStdLib_Loc_GetMemCheckResult()
*********************************************************************************************************************/
/*!
* Internal comment removed.
 *
 *
*/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_Loc_GetMemCheckResult(
  uint32 nBufValue,
  uint32 nPattern,
  P2VAR(Std_ReturnType, AUTOMATIC, VSTDLIB_VAR_FAR) pRetVal,
  Std_ReturnType errorCode)
{
  /* ----- Implementation ----------------------------------------------- */
  /* #10 If the buffer value and pattern are not equal, store errorCode at pRetVal */
  if (nBufValue != nPattern)
  {
    *(pRetVal) = errorCode; /* SBSW_VSTDLIB_ACCESS_PASSED_INT */
  }
}

/**********************************************************************************************************************
 * VStdLib_Loc_MemCheck_8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_Loc_MemCheck_8(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf8,
  uint8 nPattern,
  uint32_least nCnt,
  Std_ReturnType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint32_least idx = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Linearly check the bytes using a loop */
  for (; ((idx < nCnt) && (retVal == E_OK)); idx++) /* FETA_VSTDLIB_LOC_MEMCHECK_X_NCNT, FETA_VSTDLIB_LOC_MEMCHECK_X_RETVAL */
  {
    VStdLib_Loc_GetMemCheckResult( /* SBSW_VSTDLIB_CALL_MODIFIABLE_RETVAL */
      pBuf8[idx],
      nPattern,
      &retVal,
      errorCode);
  }

  return retVal;
} /* VStdLib_Loc_MemCheck_8() */

# if (VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON)

/**********************************************************************************************************************
 * VStdLib_Loc_MemCheck_32_Aligned()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_Loc_MemCheck_32_Aligned(
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf32,
  uint8 nPattern,
  uint32_least nCnt,
  Std_ReturnType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint32_least dPos = 0u;
  uint32_least remaining = nCnt;
  uint32 nPattern32 =
    (uint32)(((uint32)nPattern << 24u) | ((uint32)nPattern << 16u) | ((uint32)nPattern << 8u) | nPattern);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Linearly check 4-byte-wise using 32-bit accesses in a loop */
  for (; ((remaining >= 4u) && (retVal == E_OK)); remaining -= 4u) /* FETA_VSTDLIB_LOC_MEMCHECK_X_NCNT, FETA_VSTDLIB_LOC_MEMCHECK_X_RETVAL */
  {
    VStdLib_Loc_GetMemCheckResult( /* SBSW_VSTDLIB_CALL_MODIFIABLE_RETVAL */
      pBuf32[dPos],
      nPattern32,
      &retVal,
      errorCode);
    dPos++;
  }

  /* #20 If no mismatch was found, check the remaining bytes using a byte-wise algorithm */
  if (retVal == E_OK)
  {
    retVal = VStdLib_Loc_MemCheck_8( /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */ /* FETA_VSTDLIB_CALL_LOC_MEMCHECK_8_REMAINING */
      (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) &pBuf32[dPos],
      nPattern,
      remaining,
      errorCode);
  }

  return retVal;
} /* VStdLib_Loc_MemCheck_32_Aligned() */

# endif /* VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON */
#endif /* VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTIONS != STD_ON */

#if ((VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || \
     (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || ((defined PLATFORM_SUPPORT_SINT64_UINT64) && \
     (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)))

/**********************************************************************************************************************
 * VStdLib_Loc_GetHighestBitPosOneInByte()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetHighestBitPosOneInByte(uint8 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Does the upper nibble contain a set bit? */
  if ((value & VSTDLIB_MASK_UPPER_NIBBLE) != 0u)
  {
    /* #110 Shift upper nibble in lower nibble and adapt postion */
    value >>= 4u; /* PRQA S 1338 */ /* MD_VStdLib_ParamValueMod */
    pos = 4u;
  }

  /* #200 Determine bit position in lower nibble */
  pos += VStdLib_Loc_GetHighestBitPosOneInNibble(value);

  return pos;
}

/**********************************************************************************************************************
 * VStdLib_Loc_GetLowestBitPosOneInByte()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetLowestBitPosOneInByte(uint8 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Does the lower nibble contain a set bit? */
  if ((value & VSTDLIB_MASK_LOWER_NIBBLE) != 0u)
  {
    /* #110 Mask lower nibble */
    value &= VSTDLIB_MASK_LOWER_NIBBLE; /* PRQA S 1338 */ /* MD_VStdLib_ParamValueMod */
  }
  else
  {
    /* #120 Shift upper nibble in lower nibble and adapt postion */
    value >>= 4u; /* PRQA S 1338 */ /* MD_VStdLib_ParamValueMod */
    pos = 4u;
  }

  /* #200 Determine bit position in lower nibble */
  pos += VStdLib_Loc_GetLowestBitPosOneInNibble(value);

  return pos;
}

#endif /* (VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) ||
          (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || ((defined PLATFORM_SUPPORT_SINT64_UINT64) &&
          (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)) */

#if ((VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || \
     ((defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)))

/**********************************************************************************************************************
 * VStdLib_Loc_GetHighestBitPosOneInTwoBytes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetHighestBitPosOneInTwoBytes(uint16 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Does the upper byte contain a set bit? */
  if ((value & VSTDLIB_MASK_UPPER_BYTE) != 0u)
  {
    /* #110 Shift upper byte in lower byte and adapt postion */
    value >>= 8u; /* PRQA S 1338 */ /* MD_VStdLib_ParamValueMod */
    pos = 8u;
  }

  /* #200 Determine bit position in lower byte */
  pos += VStdLib_Loc_GetHighestBitPosOneInByte((uint8)(value));

  return pos;
}

/**********************************************************************************************************************
 * VStdLib_Loc_GetLowestBitPosOneInTwoBytes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetLowestBitPosOneInTwoBytes(uint16 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Does the lower byte contain a set bit? */
  if ((value & VSTDLIB_MASK_LOWER_BYTE) != 0u)
  {
    /* #110 Mask lower byte */
    value &= VSTDLIB_MASK_LOWER_BYTE; /* PRQA S 1338 */ /* MD_VStdLib_ParamValueMod */
  }
  else
  {
    /* #120 Shift upper byte in lower byte and adapt postion */
    value >>= 8u; /* PRQA S 1338 */ /* MD_VStdLib_ParamValueMod */
    pos = 8u;
  }

  /* #200 Determine bit position in lower byte */
  pos += VStdLib_Loc_GetLowestBitPosOneInByte((uint8)(value));

  return pos;
}

#endif /* (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) ||
          ((defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)) */

#if ((VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || ((defined PLATFORM_SUPPORT_SINT64_UINT64) && \
     (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)))


/**********************************************************************************************************************
 * VStdLib_Loc_GetHighestBitPosOneInFourBytes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetHighestBitPosOneInFourBytes(uint32 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Does the upper two bytes contain a set bit? */
  if ((value & VSTDLIB_MASK_UPPER_TWO_BYTES) != 0u)
  {
    /* #110 Shift upper two bytes in lower two bytes and adapt postion */
    value >>= 16u; /* PRQA S 1338 */ /* MD_VStdLib_ParamValueMod */
    pos = 16u;
  }

  /* #200 Determine bit position in lower two bytes */
  pos += VStdLib_Loc_GetHighestBitPosOneInTwoBytes((uint16)(value));

  return pos;
}

/**********************************************************************************************************************
 * VStdLib_Loc_GetLowestBitPosOneInFourBytes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetLowestBitPosOneInFourBytes(uint32 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Does the lower two bytes contain a set bit? */
  if ((value & VSTDLIB_MASK_LOWER_TWO_BYTES) != 0u)
  {
    /* #110 Mask lower two bytes */
    value &= VSTDLIB_MASK_LOWER_TWO_BYTES; /* PRQA S 1338 */ /* MD_VStdLib_ParamValueMod */
  }
  else
  {
    /* #120 Shift upper two bytes in lower two bytes and adapt postion */
    value >>= 16u; /* PRQA S 1338 */ /* MD_VStdLib_ParamValueMod */
    pos = 16u;
  }

  /* #200 Determine bit position in lower two bytes */
  pos += VStdLib_Loc_GetLowestBitPosOneInTwoBytes((uint16)(value));

  return pos;
}

#endif /* (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) || ((defined PLATFORM_SUPPORT_SINT64_UINT64) &&
          (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)) */

#if ((defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON))

/**********************************************************************************************************************
 * VStdLib_Loc_GetHighestBitPosOneInEightBytes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetHighestBitPosOneInEightBytes(uint64 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Does the upper four bytes contain a set bit? */
  if ((value & VSTDLIB_MASK_UPPER_FOUR_BYTES) != 0u)
  {
    /* #110 Shift upper four bytes in lower four bytes and adapt postion */
    value >>= 32u; /* PRQA S 1338 */ /* MD_VStdLib_ParamValueMod */
    pos = 32u;
  }

  /* #200 Determine bit position in lower four bytes */
  pos += VStdLib_Loc_GetHighestBitPosOneInFourBytes((uint32)(value));

  return pos;
}

/**********************************************************************************************************************
 * VStdLib_Loc_GetLowestBitPosOneInEightBytes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(uint8, VSTDLIB_CODE) VStdLib_Loc_GetLowestBitPosOneInEightBytes(uint64 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = 0u;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Does the lower four bytes contain a set bit? */
  if ((value & VSTDLIB_MASK_LOWER_FOUR_BYTES) != 0u)
  {
    /* #110 Mask lower four bytes */
    value &= VSTDLIB_MASK_LOWER_FOUR_BYTES; /* PRQA S 1338 */ /* MD_VStdLib_ParamValueMod */
  }
  else
  {
    /* #120 Shift upper four bytes in lower four bytes and adapt postion */
    value >>= 32u; /* PRQA S 1338 */ /* MD_VStdLib_ParamValueMod */
    pos = 32u;
  }

  /* #200 Determine bit position in lower four bytes */
  pos += VStdLib_Loc_GetLowestBitPosOneInFourBytes((uint32)(value));

  return pos;
}

#endif /* (defined PLATFORM_SUPPORT_SINT64_UINT64 ) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

#if (VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 * VStdLib_MemSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, VSTDLIB_CODE) VStdLib_MemSet(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  uint8 nPattern,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8 = \
    (P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) pDst; /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameter 'pDst' */
  if (pDst == NULL_PTR)
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON)

    /* #20 If less than 32 bytes have to be initialized */
    if (nCnt < 32u)
    {
      /* #30 Use a byte-wise algorithm */
      VStdLib_Loc_MemSet_8_31( /* SBSW_VSTDLIB_CALL_UNMODIFIED */ /* FETA_VSTDLIB_CALL_LOC_MEMSET_X_X */
        pDst8,
        nPattern,
        (uint8_least)nCnt);
    }
    /* #40 Else */
    else
    {
      /* #50 Get the destination pointer offset to the next 32-bit boundary */
      uint8_least nDstOffset = VSTDLIB_ALIGN_OFFSET(pDst8); /* PRQA S 0306 */ /* MD_VStdLib_PtrCastToInt */

      /* #60 If the destination is not aligned */
      if (nDstOffset != 0u)
      {
        /* #70 Initialize the amount of bytes to reach the alignment boundary and adjust the pointer */
        VStdLib_Loc_MemSet_8_3( /* SBSW_VSTDLIB_CALL_MODIFIED_CNT */ /* FETA_VSTDLIB_CALL_LOC_MEMSET_8_3 */
          pDst8,
          nPattern,
          nDstOffset);

        pDst8 = &pDst8[nDstOffset];
        nCnt -= nDstOffset; /* PRQA S 1338 */ /* MD_VStdLib_ParamMod */
      }

      /* #80 The pointer is aligned, use the 32-bit aligned memset */
      VStdLib_Loc_MemSet_32_Aligned( /* SBSW_VSTDLIB_CALL_MODIFIED_PTR_CNT */ /* FETA_VSTDLIB_CALL_LOC_MEMSET_X_X */
        (P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR)) pDst8, /* PRQA S 0310, 3305 */ /* MD_VStdLib_PtrCast */
        nPattern,
        nCnt);
    }

# else /* VSTDLIB_RUNTIME_OPTIMIZATION == STD_OFF */

    VStdLib_CntType idx;

    /* #90 Linearly initialize nCnt bytes using a loop */
    for (idx = 0u; idx < nCnt; idx++) /* FETA_VSTDLIB_MEMSET */
    {
      pDst8[idx] = nPattern; /* SBSW_VSTDLIB_ACCESS_LOOP */
    }

# endif /* VSTDLIB_RUNTIME_OPTIMIZATION */
  }

  /* ----- Development Error Report ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_MEM_SET, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
} /* VStdLib_MemSet() */

/**********************************************************************************************************************
 * VStdLib_MemCpy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(void, VSTDLIB_CODE) VStdLib_MemCpy(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8 = \
    (P2VAR(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) pDst; /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8 = \
    (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) pSrc; /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'pDst' and 'pSrc' */
  if ((pDst == NULL_PTR) || (pSrc == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON)

    /* #20 If less than 32 bytes have to be copied */
    if (nCnt < 32u)
    {
      /* #30 Use a byte-wise algorithm */
      VStdLib_Loc_MemCpy_8_31( /* SBSW_VSTDLIB_CALL_UNMODIFIED */ /* FETA_VSTDLIB_CALL_LOC_MEMCPY_X_X */
        pDst8,
        pSrc8,
        (uint8_least)nCnt);
    }
    /* #40 Else */
    else
    {
      /* #50 Get the source pointer offset to the next 32-bit boundary */
      uint8_least nSrcOffset = VSTDLIB_ALIGN_OFFSET(pSrc8); /* PRQA S 0306 */ /* MD_VStdLib_PtrCastToInt */
      uint8_least nDstOffset;

      /* #60 If the source is not aligned */
      if (nSrcOffset != 0u)
      {
        /* #70 Copy the amount of bytes to reach the alignment boundary and adjust the pointers */
        VStdLib_Loc_MemCpy_8_3( /* SBSW_VSTDLIB_CALL_MODIFIED_CNT */ /* FETA_VSTDLIB_CALL_LOC_MEMCPY_8_3 */
          pDst8,
          pSrc8,
          nSrcOffset);

        pSrc8 = &pSrc8[nSrcOffset];
        pDst8 = &pDst8[nSrcOffset];
        nCnt -= nSrcOffset; /* PRQA S 1338 */ /* MD_VStdLib_ParamMod */
      }

      /* #80 Get the destination pointer offset to the next 32-bit boundary */
      nDstOffset = VSTDLIB_ALIGN_OFFSET(pDst8); /* PRQA S 0306 */ /* MD_VStdLib_PtrCastToInt */

      /* #90 If destination is aligned */
      if (nDstOffset == 0u)
      {
        /* #100 Both pointers are aligned, use the 32-bit aligned memcpy */
        VStdLib_Loc_MemCpy_32_Aligned( /* SBSW_VSTDLIB_CALL_MODIFIED_PTR_CNT */ /* FETA_VSTDLIB_CALL_LOC_MEMCPY_X_X */
          (P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR)) pDst8, /* PRQA S 0310, 3305 */ /* MD_VStdLib_PtrCast */
          (P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR)) pSrc8, /* PRQA S 0310, 3305 */ /* MD_VStdLib_PtrCast */
          nCnt);
      }
      /* #110 Else */
      else
      {
        /* #120 Use the 32-bit unaligned memcpy */
        VStdLib_Loc_MemCpy_32_Unaligned( /* SBSW_VSTDLIB_CALL_MODIFIED_PTR_CNT */ /* FETA_VSTDLIB_CALL_LOC_MEMCPY_X_X, FETA_VSTDLIB_CALL_LOC_MEMCPY_32_UNALIGNED_NDSTOFFSET */
          pDst8,
          (P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR)) pSrc8, /* PRQA S 0310, 3305 */ /* MD_VStdLib_PtrCast */
          nCnt,
          nDstOffset);
      }
    }

# else /* VSTDLIB_RUNTIME_OPTIMIZATION == STD_OFF */

    VStdLib_CntType idx;

    /* #130 Linearly copy nCnt bytes using a loop */
    for (idx = 0u; idx < nCnt; idx++) /* FETA_VSTDLIB_MEMCPY */
    {
      pDst8[idx] = pSrc8[idx]; /* SBSW_VSTDLIB_ACCESS_LOOP */
    }

# endif /* VSTDLIB_RUNTIME_OPTIMIZATION */
  }

  /* ----- Development Error Report ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_MEM_COPY, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
} /* VStdLib_MemCpy() */

/**********************************************************************************************************************
 * VStdLib_MemCpy16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, VSTDLIB_CODE) VStdLib_MemCpy16(P2VAR(uint16, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  P2CONST(uint16, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  VStdLib_CntType idx;
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'pDst' and 'pSrc' */
  if ((pDst == NULL_PTR) || (pSrc == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Linearly copy nCnt 16-bit blocks using a loop */
    for (idx = 0u; idx < nCnt; idx++) /* FETA_VSTDLIB_MEMCPY */
    {
      pDst[idx] = pSrc[idx]; /* SBSW_VSTDLIB_ACCESS_LOOP */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_MEM_COPY_16, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
} /* VStdLib_MemCpy16() */

/**********************************************************************************************************************
 * VStdLib_MemCpy32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, VSTDLIB_CODE) VStdLib_MemCpy32(P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  VStdLib_CntType idx;
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'pDst' and 'pSrc' */
  if ((pDst == NULL_PTR) || (pSrc == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Linearly copy nCnt 32-bit blocks using a loop */
    for (idx = 0u; idx < nCnt; idx++) /* FETA_VSTDLIB_MEMCPY */
    {
      pDst[idx] = pSrc[idx]; /* SBSW_VSTDLIB_ACCESS_LOOP */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_MEM_COPY_32, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
} /* VStdLib_MemCpy32() */

#endif /* VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON */

/**********************************************************************************************************************
 * VStdLib_MemCpy_s()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, VSTDLIB_CODE) VStdLib_MemCpy_s(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check parameter 'nDstSize' (independently of development error detection) */
  if (nCnt > nDstSize)
  {
    errorId = VSTDLIB_E_PARAM_SIZE;
  }
  else
  {
    /* #20 Call VStdLib_MemCpy() */
    VStdLib_MemCpy(pDst, pSrc, nCnt); /* SBSW_VSTDLIB_CALL_UNMODIFIED */ /* FETA_VSTDLIB_CALL_MEMCPY */
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_MEM_COPY_S, errorId);
  }
#else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
} /* VStdLib_MemCpy_s() */

#if (VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_MemCmp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(sint8, VSTDLIB_CODE) VStdLib_MemCmp(
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf1,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf2,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf1_8 = \
    (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR))pBuf1; /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf2_8 = \
    (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR))pBuf2; /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
  uint8 errorId = VSTDLIB_E_NO_ERROR;
  sint8 result = VSTDLIB_MEM_CMP_RES_EQ;

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameter 'pBuf1' */
  if (pBuf1 == NULL_PTR)
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
    result = VSTDLIB_MEM_CMP_RES_L;
  }
  /* #20 Check parameter 'pBuf2' */
  else if (pBuf2 == NULL_PTR)
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
    result = VSTDLIB_MEM_CMP_RES_G;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 If buffer1 and buffer2 can be unequal */
    if (pBuf1 != pBuf2)
    {
# if (VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON)
      /* #110 If less than 32 bytes have to be compared */
      if (nCnt < 32u)
      {
        /* #120 Use byte-wise memcmp */
        result = VStdLib_Loc_MemCmp_8(pBuf1_8, pBuf2_8, nCnt); /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */ /* FETA_VSTDLIB_CALL_LOC_MEMCMP_X_X */
      }
      /* #130 Else */
      else
      {
        /* #140 Get the pointer offset from buffer1 to the next 32-bit boundary */
        uint8_least nBuf1Offset = VSTDLIB_ALIGN_OFFSET(pBuf1_8); /* PRQA S 0306 */ /* MD_VStdLib_PtrCastToInt */

        /* #150 If buffer1 is not aligned */
        if (nBuf1Offset != 0u)
        {
          /* #160 Compare the amount of bytes to reach the alignment boundary of buffer1 */
          result = VStdLib_Loc_MemCmp_8(pBuf1_8, pBuf2_8, nBuf1Offset); /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */ /* FETA_VSTDLIB_CALL_LOC_MEMCMP_8_NBUF1OFFSET */
        }

        /* #170 If buffers are still equal */
        if (result == VSTDLIB_MEM_CMP_RES_EQ)
        {
          uint8_least nBuf2Offset;

          /* #180 Adjust the pointers for buffer1 and buffer2 */
          pBuf1_8 = &pBuf1_8[nBuf1Offset];
          pBuf2_8 = &pBuf2_8[nBuf1Offset];
          nCnt -= nBuf1Offset; /* PRQA S 1338 */ /* MD_VStdLib_ParamMod */

          /* #190 Get the pointer offset from buffer2 to the next 32-bit boundary */
          nBuf2Offset = VSTDLIB_ALIGN_OFFSET(pBuf2_8); /* PRQA S 0306 */ /* MD_VStdLib_PtrCastToInt */

          /* #200 If buffer2 is aligned */
          if (nBuf2Offset == 0u)
          {
            /* #210 Both pointers are aligned, use the 32-bit aligned memcmp */
            result = VStdLib_Loc_MemCmp_32_Aligned( /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */ /* FETA_VSTDLIB_CALL_LOC_MEMCMP_X_X */
              (P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR)) pBuf1_8, /* PRQA S 0310, 3305 */ /* MD_VStdLib_PtrCast */
              (P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR)) pBuf2_8, /* PRQA S 0310, 3305 */ /* MD_VStdLib_PtrCast */
              nCnt);
          }
          /* #220 Else buffer2 is not aligned and buffer 1 is aligned */
          else
          {
            /* #230 Use the 32-bit unaligned memcmp */
            result = VStdLib_Loc_MemCmp_32_Unaligned( /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */ /* FETA_VSTDLIB_CALL_LOC_MEMCMP_X_X, FETA_VSTDLIB_CALL_LOC_MEMCMP_32_UNALIGNED_NBUF2OFFSET */
              (P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR)) pBuf1_8, /* PRQA S 0310, 3305 */ /* MD_VStdLib_PtrCast */
              pBuf2_8,
              nCnt,
              nBuf2Offset);
          }
        }
      }
# else /* VSTDLIB_RUNTIME_OPTIMIZATION == STD_OFF */

      /* #240 Use byte-wise memcmp */
      result = VStdLib_Loc_MemCmp_8(pBuf1_8, pBuf2_8, nCnt); /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */ /* FETA_VSTDLIB_CALL_LOC_MEMCMP_X_X */

# endif /* VSTDLIB_RUNTIME_OPTIMIZATION */
    }
  }

  /* ----- Development Error Report --------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_MEM_CMP, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
# endif

  return result;
}

#endif /* VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON */

#if (VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON)

/**********************************************************************************************************************
 * VStdLib_MemCpySwapEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
/* PRQA S 6080 1 */ /* MD_MSR_STMIF */
FUNC(void, VSTDLIB_CODE) VStdLib_MemCpySwapEndian(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nCnt,
  uint8 intType)
{
  /* ----- Local Variables ---------------------------------------------- */
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc8 = \
    (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) pSrc; /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
# if ((VSTDLIB_DEV_ERROR_DETECT == STD_ON) || (VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON))
  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pDst8 = \
    (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) pDst; /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
# endif
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'pDst' and 'pSrc' */
  if ((pDst == NULL_PTR) || (pSrc == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  /* #20 Check parameter 'pDst' is aligned according to the integer type */
#  if (CPU_TYPE == CPU_TYPE_64)
  else if (((uint64)pDst8 % (uint64)intType) != 0u) /* PRQA S 0306 */ /* MD_VStdLib_PtrCastToInt */
#  else
  else if (((uint32_least)pDst8 % (uint32_least)intType) != 0u) /* PRQA S 0306 */ /* MD_VStdLib_PtrCastToInt */
#  endif
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  /* #30 Check parameter 'nCnt' is a multiple of integer type size */
  else if ((nCnt % (VStdLib_CntType)intType) != 0u)
  {
    errorId = VSTDLIB_E_PARAM_SIZE;
  }
  /* Parameter intType is checked in the local functions. No copy and swap is executed for any unsupported intType
     and VSTDLIB_E_PARAM_INTTYPE is reported if VSTDLIB_DEV_ERROR_REPORT is enabled */
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 If integer type is VSTDLIB_INTTYPE_8, call VStdLib_MemCpy() */
    if (intType == VSTDLIB_INTTYPE_8)
    {
      VStdLib_MemCpy(pDst, pSrc, nCnt); /* SBSW_VSTDLIB_CALL_UNMODIFIED */ /* FETA_VSTDLIB_CALL_MEMCPY */
    }
    else
    {
# if (VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON)
      /* #110 If less than 33 bytes have to be copied and swapped */
      if (nCnt < 33u)
      {
        /* #120 Use a integer-type-wise algorithm (for dst) */
        VStdLib_Loc_MemCpySwapEndian_IntType_32( /* SBSW_VSTDLIB_CALL_UNMODIFIED */ /* FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_X_X */
          pDst,
          pSrc8,
          (uint8_least)nCnt,
          intType);
      }
      /* #130 If destination and source are 32-bit aligned */
      else if ((VSTDLIB_ALIGN_OFFSET(pDst8) == 0u) && (VSTDLIB_ALIGN_OFFSET(pSrc8) == 0u)) /* PRQA S 0306 */ /* MD_VStdLib_PtrCastToInt */
      {
        /* #140 Use a 32-bit-wise algorithm (for dst and src) */
        VStdLib_Loc_MemCpySwapEndian_32_Aligned( /* SBSW_VSTDLIB_CALL_UNMODIFIED */ /* FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_X_X */
          (P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR)) pDst, /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
          (P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR)) pSrc, /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */
          nCnt,
          intType);
      }
      /* #150 Else destination or source is not aligned */
      else
      {
        /* #160 Linearly copy and swap nCnt bytes using a loop (integer type access to dst) */
        VStdLib_Loc_MemCpySwapEndian(pDst, pSrc8, nCnt, intType); /* SBSW_VSTDLIB_CALL_UNMODIFIED */ /* FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_X_X */
      }
# else  /* VSTDLIB_RUNTIME_OPTIMIZATION == STD_OFF */
      /* #170 Linearly copy and swap nCnt bytes using a loop (integer type access to dst) */
      VStdLib_Loc_MemCpySwapEndian(pDst, pSrc8, nCnt, intType); /* SBSW_VSTDLIB_CALL_UNMODIFIED */
# endif /* VSTDLIB_RUNTIME_OPTIMIZATION */
    }
  }
  /* ----- Development Error Report ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_MEM_COPY_SWAP_ENDIAN, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
} /* VStdLib_MemCpySwapEndian() */

#endif /* VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON */

/**********************************************************************************************************************
 * VStdLib_MemCpySwapEndian_s()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_MemCpySwapEndian_s( /* PRQA S 6060 */ /* MD_MSR_STPAR */
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nCnt,
  uint8 intType,
  Std_ReturnType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;
  Std_ReturnType retval = E_OK;

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check parameter 'nDstSize' (independently of development error detection) */
  if (nCnt > nDstSize)
  {
    errorId = VSTDLIB_E_PARAM_SIZE;
    retval = errorCode;
  }
  else
  {
    /* #20 Call VStdLib_MemCpySwapEndian() */
    VStdLib_MemCpySwapEndian(pDst, pSrc, nCnt, intType); /* SBSW_VSTDLIB_CALL_UNMODIFIED */ /* FETA_VSTDLIB_CALL_MEMCPYSWAPENDIAN */
    /* retval is already set to E_OK */
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_MEM_COPY_SWAP_ENDIAN_S, errorId);
  }
#else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retval;
} /* VStdLib_MemCpySwapEndian_s() */

#if (VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_BitCpySawBigToBigEndian()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpySawBigToBigEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* For big endian data use monotone bit counting */
  VStdLib_CntType nDstBitOffsetSawToMon = VStdLib_Loc_BitCpyConvertBitCounting(nDstBitOffsetSaw);
  VStdLib_CntType nSrcBitOffsetSawToMon = VStdLib_Loc_BitCpyConvertBitCounting(nSrcBitOffsetSaw);

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'pDst' and 'pSrc' */
  if ((pDst == NULL_PTR) || (pSrc == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  /* #20 Use VStdLib_Loc_BitCpyBigToBigEndian() to copy data if parameter 'nCnt' is bigger than 0 */
  else
# endif
  {
    /* Catch the case, if there is nothing to copy. If it is not catched here, it needs to be catch in
       VStdLib_Loc_BitCpyBigToBigEndian() */
    if (nCnt > 0u)
    {
      VStdLib_Loc_BitCpyBigToBigEndian( /* FETA_VSTDLIB_CALL_LOC_BITCPY */
        pDst,
        nDstBitOffsetSawToMon,
        pSrc,
        nSrcBitOffsetSawToMon,
        nCnt); /* SBSW_VSTDLIB_CALL_CONVERTED_BITOFFSET */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_SAW, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/**********************************************************************************************************************
 *  VStdLib_BitCpySawBigToLittleEndian()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpySawBigToLittleEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* For big endian data use monotone bit counting */
  VStdLib_CntType nSrcBitOffsetSawToMon = VStdLib_Loc_BitCpyConvertBitCounting(nSrcBitOffsetSaw);

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'pDst' and 'pSrc' */
  if ((pDst == NULL_PTR) || (pSrc == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  /* #20 Use VStdLib_Loc_BitCpyBigToLittleEndian() to copy data if parameter 'nCnt' is bigger than 0 */
  else
# endif
  {
    /* Catch the case, if there is nothing to copy. If it is not catched here, it needs to be catch in
       VStdLib_Loc_BitCpyBigToLittleEndian() */
    if (nCnt > 0u)
    {
      VStdLib_Loc_BitCpyBigToLittleEndian( /* FETA_VSTDLIB_CALL_LOC_BITCPY */
        pDst,
        nDstBitOffsetSaw,
        pSrc,
        nSrcBitOffsetSawToMon,
        nCnt); /* SBSW_VSTDLIB_CALL_CONVERTED_BITOFFSET */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_SAW, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/**********************************************************************************************************************
 *  VStdLib_BitCpySawLittleToBigEndian()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpySawLittleToBigEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* For big endian data use monotone bit counting */
  VStdLib_CntType nDstBitOffsetSawToMon = VStdLib_Loc_BitCpyConvertBitCounting(nDstBitOffsetSaw);

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'pDst' and 'pSrc' */
  if ((pDst == NULL_PTR) || (pSrc == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  /* #20 Use VStdLib_Loc_BitCpyLittleToBigEndian() to copy data if parameter 'nCnt' is bigger than 0 */
  else
# endif
  {
    /* Catch the case, if there is nothing to copy. If it is not catched here, it needs to be catch in
       VStdLib_Loc_BitCpyLittleToBigEndian() */
    if (nCnt > 0u)
    {
      VStdLib_Loc_BitCpyLittleToBigEndian( /* FETA_VSTDLIB_CALL_LOC_BITCPY */
        pDst,
        nDstBitOffsetSawToMon,
        pSrc,
        nSrcBitOffsetSaw,
        nCnt); /* SBSW_VSTDLIB_CALL_CONVERTED_BITOFFSET */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_SAW, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/**********************************************************************************************************************
 *  VStdLib_BitCpySawLittleToLittleEndian()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpySawLittleToLittleEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'pDst' and 'pSrc' */
  if ((pDst == NULL_PTR) || (pSrc == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  /* #20 Use VStdLib_Loc_BitCpyLittleToLittleEndian() to copy data if parameter 'nCnt' is bigger than 0 */
  else
# endif
  {
    /* Catch the case, if there is nothing to copy. If it is not catched here, it needs to be catch in
       VStdLib_Loc_BitCpyLittleToLittleEndian() */
    if (nCnt > 0u)
    {
      VStdLib_Loc_BitCpyLittleToLittleEndian( /* FETA_VSTDLIB_CALL_LOC_BITCPY */
        pDst,
        nDstBitOffsetSaw,
        pSrc,
        nSrcBitOffsetSaw,
        nCnt); /* SBSW_VSTDLIB_CALL_UNMODIFIED */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_SAW, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

#endif /* VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON */

/**********************************************************************************************************************
 * VStdLib_BitCpySawBigToBigEndian_s()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpySawBigToBigEndian_s( /* PRQA S 6060 */ /* MD_MSR_STPAR */
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint8 errorId = VSTDLIB_E_NO_ERROR;
  /* For big endian destination data use monotone bit counting */
  VStdLib_CntType nDstBitOffsetSawToMon = VStdLib_Loc_BitCpyConvertBitCounting(nDstBitOffsetSaw);
  VStdLib_CntType dstEndByte = VStdLib_Loc_BitCpyGetLastIterByte(nCnt + nDstBitOffsetSawToMon);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check parameter 'nDstSize' (independently of development error detection) */
  if (dstEndByte > nDstSize)
  {
    errorId = VSTDLIB_E_PARAM_SIZE;
    retVal = errorCode;
  }
  else
  {
    /* #20 Call VStdLib_BitCpySawBigToBigEndian() */
    VStdLib_BitCpySawBigToBigEndian( /* SBSW_VSTDLIB_CALL_UNMODIFIED */
      pDst,
      nDstBitOffsetSaw,
      pSrc,
      nSrcBitOffsetSaw,
      nCnt);
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_SAW_S, errorId);
  }
#else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * VStdLib_BitCpySawBigToLittleEndian_s()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpySawBigToLittleEndian_s( /* PRQA S 6060 */ /* MD_MSR_STPAR */
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode)

{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint8 errorId = VSTDLIB_E_NO_ERROR;
  VStdLib_CntType dstEndByte = VStdLib_Loc_BitCpyGetLastIterByte(nCnt + nDstBitOffsetSaw);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check parameter 'nDstSize' (independently of development error detection) */
  if (dstEndByte > nDstSize)
  {
    errorId = VSTDLIB_E_PARAM_SIZE;
    retVal = errorCode;
  }
  else
  {
    /* #20 Call VStdLib_BitCpySawBigToLittleEndian() */
    VStdLib_BitCpySawBigToLittleEndian( /* SBSW_VSTDLIB_CALL_UNMODIFIED */
      pDst,
      nDstBitOffsetSaw,
      pSrc,
      nSrcBitOffsetSaw,
      nCnt);
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_SAW_S, errorId);
  }
#else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * VStdLib_BitCpySawLittleToBigEndian_s()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpySawLittleToBigEndian_s( /* PRQA S 6060 */ /* MD_MSR_STPAR */
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode)

{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint8 errorId = VSTDLIB_E_NO_ERROR;
  /* For big endian destination data use monotone bit counting */
  VStdLib_CntType nDstBitOffsetSawToMon = VStdLib_Loc_BitCpyConvertBitCounting(nDstBitOffsetSaw);
  VStdLib_CntType dstEndByte = VStdLib_Loc_BitCpyGetLastIterByte(nCnt + nDstBitOffsetSawToMon);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check parameter 'nDstSize' (independently of development error detection) */
  if (dstEndByte > nDstSize)
  {
    errorId = VSTDLIB_E_PARAM_SIZE;
    retVal = errorCode;
  }
  else
  {
    /* #20 Call VStdLib_BitCpySawLittleToBigEndian() */
    VStdLib_BitCpySawLittleToBigEndian( /* SBSW_VSTDLIB_CALL_UNMODIFIED */
      pDst,
      nDstBitOffsetSaw,
      pSrc,
      nSrcBitOffsetSaw,
      nCnt);
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_SAW_S, errorId);
  }
#else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * VStdLib_BitCpySawLittleToLittleEndian_s()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpySawLittleToLittleEndian_s( /* PRQA S 6060 */ /* MD_MSR_STPAR */
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode)

{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint8 errorId = VSTDLIB_E_NO_ERROR;
  VStdLib_CntType dstEndByte = VStdLib_Loc_BitCpyGetLastIterByte(nCnt + nDstBitOffsetSaw);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check parameter 'nDstSize' (independently of development error detection) */
  if (dstEndByte > nDstSize)
  {
    errorId = VSTDLIB_E_PARAM_SIZE;
    retVal = errorCode;
  }
  else
  {
    /* #20 Call VStdLib_BitCpySawLittleToLittleEndian() */
    VStdLib_BitCpySawLittleToLittleEndian( /* SBSW_VSTDLIB_CALL_UNMODIFIED */
      pDst,
      nDstBitOffsetSaw,
      pSrc,
      nSrcBitOffsetSaw,
      nCnt);
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_SAW_S, errorId);
  }
#else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
}

#if (VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_BitCpyMonBigToBigEndian()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpyMonBigToBigEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'pDst' and 'pSrc' */
  if ((pDst == NULL_PTR) || (pSrc == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  /* #20 Use VStdLib_Loc_BitCpyBigToBigEndian() to copy data if parameter 'nCnt' is bigger than 0 */
  else
# endif
  {
    /* Catch the case, if there is nothing to copy. If it is not catched here, it needs to be catch in
       VStdLib_Loc_BitCpyBigToBigEndian() */
    if (nCnt > 0u)
    {
      VStdLib_Loc_BitCpyBigToBigEndian( /* FETA_VSTDLIB_CALL_LOC_BITCPY */
        pDst,
        nDstBitOffsetMon,
        pSrc,
        nSrcBitOffsetMon,
        nCnt); /* SBSW_VSTDLIB_CALL_UNMODIFIED */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_MON, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/**********************************************************************************************************************
 *  VStdLib_BitCpyMonBigToLittleEndian()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpyMonBigToLittleEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* For little endian data use sawtooth bit counting */
  VStdLib_CntType nDstBitOffsetMonToSaw = VStdLib_Loc_BitCpyConvertBitCounting(nDstBitOffsetMon);

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'pDst' and 'pSrc' */
  if ((pDst == NULL_PTR) || (pSrc == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  /* #20 Use VStdLib_Loc_BitCpyBigToLittleEndian() to copy data if parameter 'nCnt' is bigger than 0 */
  else
# endif
  {
    /* Catch the case, if there is nothing to copy. If it is not catched here, it needs to be catch in
       VStdLib_Loc_BitCpyBigToLittleEndian() */
    if (nCnt > 0u)
    {
      VStdLib_Loc_BitCpyBigToLittleEndian( /* FETA_VSTDLIB_CALL_LOC_BITCPY */
        pDst,
        nDstBitOffsetMonToSaw,
        pSrc,
        nSrcBitOffsetMon,
        nCnt); /* SBSW_VSTDLIB_CALL_CONVERTED_BITOFFSET */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_MON, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/**********************************************************************************************************************
 *  VStdLib_BitCpyMonLittleToBigEndian()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpyMonLittleToBigEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* For little endian data use sawtooth bit counting */
  VStdLib_CntType nSrcBitOffsetMonToSaw = VStdLib_Loc_BitCpyConvertBitCounting(nSrcBitOffsetMon);

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'pDst' and 'pSrc' */
  if ((pDst == NULL_PTR) || (pSrc == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  /* #20 Use VStdLib_Loc_BitCpyLittleToBigEndian() to copy data if parameter 'nCnt' is bigger than 0 */
  else
# endif
  {
    /* Catch the case, if there is nothing to copy. If it is not catched here, it needs to be catch in
       VStdLib_Loc_BitCpyLittleToBigEndian() */
    if (nCnt > 0u)
    {
      VStdLib_Loc_BitCpyLittleToBigEndian( /* FETA_VSTDLIB_CALL_LOC_BITCPY */
        pDst,
        nDstBitOffsetMon,
        pSrc,
        nSrcBitOffsetMonToSaw,
        nCnt); /* SBSW_VSTDLIB_CALL_CONVERTED_BITOFFSET */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_MON, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

/**********************************************************************************************************************
 *  VStdLib_BitCpyMonLittleToLittleEndian()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpyMonLittleToLittleEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;
  /* For little endian data use sawtooth bit counting */
  VStdLib_CntType nDstBitOffsetMonToSaw = VStdLib_Loc_BitCpyConvertBitCounting(nDstBitOffsetMon);
  VStdLib_CntType nSrcBitOffsetMonToSaw = VStdLib_Loc_BitCpyConvertBitCounting(nSrcBitOffsetMon);

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'pDst' and 'pSrc' */
  if ((pDst == NULL_PTR) || (pSrc == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  /* #20 Use VStdLib_Loc_BitCpyLittleToLittleEndian() to copy data if parameter 'nCnt' is bigger than 0 */
  else
# endif
  {
    /* Catch the case, if there is nothing to copy. If it is not catched here, it needs to be catch in
       VStdLib_Loc_BitCpyLittleToLittleEndian() */
    if (nCnt > 0u)
    {
      VStdLib_Loc_BitCpyLittleToLittleEndian( /* FETA_VSTDLIB_CALL_LOC_BITCPY */
        pDst,
        nDstBitOffsetMonToSaw,
        pSrc,
        nSrcBitOffsetMonToSaw,
        nCnt); /* SBSW_VSTDLIB_CALL_CONVERTED_BITOFFSET */
    }
  }

  /* ----- Development Error Report ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_MON, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
}

#endif /* VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON */

/**********************************************************************************************************************
 * VStdLib_BitCpyMonBigToBigEndian_s()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpyMonBigToBigEndian_s( /* PRQA S 6060 */ /* MD_MSR_STPAR */
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;
  Std_ReturnType retVal = E_OK;
  VStdLib_CntType dstEndByte = VStdLib_Loc_BitCpyGetLastIterByte(nCnt + nDstBitOffsetMon);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check parameter 'nDstSize' (independently of development error detection) */
  if (dstEndByte > nDstSize)
  {
    errorId = VSTDLIB_E_PARAM_SIZE;
    retVal = errorCode;
  }
  else
  {
    /* #20 Call VStdLib_BitCpyMonBigToBigEndian() */
    VStdLib_BitCpyMonBigToBigEndian( /* SBSW_VSTDLIB_CALL_UNMODIFIED */
      pDst,
      nDstBitOffsetMon,
      pSrc,
      nSrcBitOffsetMon,
      nCnt);
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_MON_S, errorId);
  }
#else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * VStdLib_BitCpyMonBigToLittleEndian_s()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpyMonBigToLittleEndian_s( /* PRQA S 6060 */ /* MD_MSR_STPAR */
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode)

{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint8 errorId = VSTDLIB_E_NO_ERROR;
  /* For little endian destination data use sawtooth bit counting */
  VStdLib_CntType nDstBitOffsetMonToSaw = VStdLib_Loc_BitCpyConvertBitCounting(nDstBitOffsetMon);
  VStdLib_CntType dstEndByte = VStdLib_Loc_BitCpyGetLastIterByte(nCnt + nDstBitOffsetMonToSaw);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check parameter 'nDstSize' (independently of development error detection) */
  if (dstEndByte > nDstSize)
  {
    errorId = VSTDLIB_E_PARAM_SIZE;
    retVal = errorCode;
  }
  else
  {
    /* #20 Call VStdLib_BitCpyMonBigToLittleEndian() */
    VStdLib_BitCpyMonBigToLittleEndian( /* SBSW_VSTDLIB_CALL_UNMODIFIED */
      pDst,
      nDstBitOffsetMon,
      pSrc,
      nSrcBitOffsetMon,
      nCnt);
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_MON_S, errorId);
  }
#else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * VStdLib_BitCpyMonLittleToBigEndian_s()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpyMonLittleToBigEndian_s( /* PRQA S 6060 */ /* MD_MSR_STPAR */
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode)

{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint8 errorId = VSTDLIB_E_NO_ERROR;
  VStdLib_CntType dstEndByte = VStdLib_Loc_BitCpyGetLastIterByte(nCnt + nDstBitOffsetMon);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check parameter 'nDstSize' (independently of development error detection) */
  if (dstEndByte > nDstSize)
  {
    errorId = VSTDLIB_E_PARAM_SIZE;
    retVal = errorCode;
  }
  else
  {
    /* #20 Call VStdLib_BitCpyMonLittleToBigEndian() */
    VStdLib_BitCpyMonLittleToBigEndian( /* SBSW_VSTDLIB_CALL_UNMODIFIED */
      pDst,
      nDstBitOffsetMon,
      pSrc,
      nSrcBitOffsetMon,
      nCnt);
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_MON_S, errorId);
  }
#else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
}

/**********************************************************************************************************************
 * VStdLib_BitCpyMonLittleToLittleEndian_s()
 *********************************************************************************************************************/
 /*!
  * Internal comment removed.
 *
 *
 *
  */
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpyMonLittleToLittleEndian_s( /* PRQA S 6060 */ /* MD_MSR_STPAR */
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode)

{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;
  uint8 errorId = VSTDLIB_E_NO_ERROR;
  /* For little endian destination data use sawtooth bit counting */
  VStdLib_CntType nDstBitOffsetMonToSaw = VStdLib_Loc_BitCpyConvertBitCounting(nDstBitOffsetMon);
  VStdLib_CntType dstEndByte = VStdLib_Loc_BitCpyGetLastIterByte(nCnt + nDstBitOffsetMonToSaw);

  /* ----- Implementation ----------------------------------------------- */
  /* #10 Check parameter 'nDstSize' (independently of development error detection) */
  if (dstEndByte > nDstSize)
  {
    errorId = VSTDLIB_E_PARAM_SIZE;
    retVal = errorCode;
  }
  else
  {
    /* #20 Call VStdLib_BitCpyMonLittleToLittleEndian() */
    VStdLib_BitCpyMonLittleToLittleEndian( /* SBSW_VSTDLIB_CALL_UNMODIFIED */
      pDst,
      nDstBitOffsetMon,
      pSrc,
      nSrcBitOffsetMon,
      nCnt);
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_BIT_COPY_MON_S, errorId);
  }
#else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif

  return retVal;
}

#if (VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION != STD_ON)

/**********************************************************************************************************************
 * VStdLib_MemCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_MemCheck(
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf,
  uint8 nPattern,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode)
{
  /* ----- Local Variables ---------------------------------------------- */
  Std_ReturnType retVal = E_OK;

  P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf8 = \
    (P2CONST(uint8, AUTOMATIC, VSTDLIB_VAR_FAR)) pBuf; /* PRQA S 0316 */ /* MD_VStdLib_PtrCast */

  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check the parameter 'pBuf' */
  if (pBuf == NULL_PTR)
  {
    retVal = errorCode;
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
# if (VSTDLIB_RUNTIME_OPTIMIZATION == STD_ON)
    /* #20 If less than 32 bytes of memory have to be checked for nPattern */
    if (nCnt < 32u)
    {
      /* #30 Use a byte-wise memcheck */
      retVal = VStdLib_Loc_MemCheck_8( /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */ /* FETA_VSTDLIB_CALL_LOC_MEMCHECK_X */
        pBuf8,
        nPattern,
        nCnt,
        errorCode);
    }
    /* #40 Else */
    else
    {
      /* #50 Get the pointer offset to the next 32-bit boundary */
      uint8_least nBufOffset = VSTDLIB_ALIGN_OFFSET(pBuf8); /* PRQA S 0306 */ /* MD_VStdLib_PtrCastToInt */

      /* #60 If the pointer is not aligned */
      if (nBufOffset != 0u)
      {
        /* #70 Check the amount of bytes to reach the alignment boundary and adjust the pointer */
        retVal = VStdLib_Loc_MemCheck_8( /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */ /* FETA_VSTDLIB_CALL_LOC_MEMCHECK_8_NBUFOFFSET */
          pBuf8,
          nPattern,
          nBufOffset,
          errorCode);

        pBuf8 = &pBuf8[nBufOffset];
        nCnt -= nBufOffset; /* PRQA S 1338 */ /* MD_VStdLib_ParamMod */
      }

      if (retVal == E_OK)
      {
        /* #80 The pointer is aligned, use the 32-bit aligned memcheck */
        retVal = VStdLib_Loc_MemCheck_32_Aligned( /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */ /* FETA_VSTDLIB_CALL_LOC_MEMCHECK_X */
          (P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR)) pBuf8, /* PRQA S 0310, 3305 */ /* MD_VStdLib_PtrCast */
          nPattern,
          nCnt,
          errorCode);
      }
    }

# else /* VSTDLIB_RUNTIME_OPTIMIZATION == STD_OFF */

    /* #90 Use byte-wise memcheck */
    retVal = VStdLib_Loc_MemCheck_8( /* SBSW_VSTDLIB_CALL_NO_PTR_WRITE */ /* FETA_VSTDLIB_CALL_LOC_MEMCHECK_X */
      pBuf8,
      nPattern,
      nCnt,
      errorCode);

# endif /* VSTDLIB_RUNTIME_OPTIMIZATION */
  }

  /* ----- Development Error Report ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_MEM_CHECK, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif

  return retVal;
} /* VStdLib_MemCheck() */

#endif /* VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION != STD_ON */

#if (VSTDLIB_VERSION_INFO_API == STD_ON)

/**********************************************************************************************************************
 *  VStdLib_GetVersionInfo()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(void, VSTDLIB_CODE) VStdLib_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, VSTDLIB_APPL_VAR) versioninfo)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameter 'versioninfo' */
  if (versioninfo == NULL_PTR)
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
# endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #20 Write component data to 'versioninfo' */
    versioninfo->vendorID = VSTDLIB_VENDOR_ID;                /* SBSW_VSTDLIB_ACCESS_VERSIONINFO */
    versioninfo->moduleID = VSTDLIB_MODULE_ID;                /* SBSW_VSTDLIB_ACCESS_VERSIONINFO */
    versioninfo->sw_major_version = VSTDLIB_SW_MAJOR_VERSION; /* SBSW_VSTDLIB_ACCESS_VERSIONINFO */
    versioninfo->sw_minor_version = VSTDLIB_SW_MINOR_VERSION; /* SBSW_VSTDLIB_ACCESS_VERSIONINFO */
    versioninfo->sw_patch_version = VSTDLIB_SW_PATCH_VERSION; /* SBSW_VSTDLIB_ACCESS_VERSIONINFO */
  }

  /* ----- Development Error Report ------------------------------------- */
# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_GET_VERSION_INFO, errorId);
  }
# else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
# endif
} /* VStdLib_GetVersionInfo() */

#endif /* VSTDLIB_VERSION_INFO_API == STD_ON */

#if (VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosOne8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosOne8(uint8 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get the bit position of the highest set bit */
    pos = VStdLib_Loc_GetHighestBitPosOneInByte(value);
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosZero8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosZero8(uint8 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_ONE_BYTE)
  {
    /* #110 Get the bit position of the highest unset bit */
    pos = VStdLib_Loc_GetHighestBitPosOneInByte((uint8)(~value));
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosOne8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosOne8(uint8 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get the bit position of the lowest set bit */
    pos = VStdLib_Loc_GetLowestBitPosOneInByte(value);
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosZero8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosZero8(uint8 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_ONE_BYTE)
  {
    /* #110 Get the bit position of the lowest unset bit */
    pos = VStdLib_Loc_GetLowestBitPosOneInByte((uint8)(~value));
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingOnes8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingOnes8(uint8 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_ONE_BYTE)
  {
    /* #110 Get count of leading ones */
    cnt = 7u - VStdLib_Loc_GetHighestBitPosOneInByte((uint8)(~value));
  }

  return cnt;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingZeros8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingZeros8(uint8 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get count of leading zeros */
    cnt = 7u - VStdLib_Loc_GetHighestBitPosOneInByte(value);
  }

  return cnt;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingOnes8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingOnes8(uint8 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_ONE_BYTE)
  {
    /* #110 Get count of trailing ones */
    cnt = VStdLib_Loc_GetLowestBitPosOneInByte((uint8)(~value));
  }

  return cnt;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingZeros8()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingZeros8(uint8 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get count of trailing zeros */
    cnt = VStdLib_Loc_GetLowestBitPosOneInByte(value);
  }

  return cnt;
}

#endif /* VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON */

#if (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosOne16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosOne16(uint16 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get the bit position of the highest set bit */
    pos = VStdLib_Loc_GetHighestBitPosOneInTwoBytes(value);
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosZero16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosZero16(uint16 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_TWO_BYTES)
  {
    /* #110 Get the bit position of the highest unset bit */
    pos = VStdLib_Loc_GetHighestBitPosOneInTwoBytes((uint16)(~value));
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosOne16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosOne16(uint16 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get the bit position of the lowest set bit */
    pos = VStdLib_Loc_GetLowestBitPosOneInTwoBytes(value);
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosZero16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosZero16(uint16 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_TWO_BYTES)
  {
    /* #110 Get the bit position of the lowest unset bit */
    pos = VStdLib_Loc_GetLowestBitPosOneInTwoBytes((uint16)(~value));
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingOnes16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingOnes16(uint16 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_TWO_BYTES)
  {
    /* #110 Get count of leading ones */
    cnt = 15u - VStdLib_Loc_GetHighestBitPosOneInTwoBytes((uint16)(~value));
  }

  return cnt;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingZeros16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingZeros16(uint16 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get count of leading zeros */
    cnt = 15u - VStdLib_Loc_GetHighestBitPosOneInTwoBytes(value);
  }

  return cnt;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingOnes16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingOnes16(uint16 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_TWO_BYTES)
  {
    /* #110 Get count of trailing ones */
    cnt = VStdLib_Loc_GetLowestBitPosOneInTwoBytes((uint16)(~value));
  }

  return cnt;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingZeros16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingZeros16(uint16 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get count of trailing zeros */
    cnt = VStdLib_Loc_GetLowestBitPosOneInTwoBytes(value);
  }

  return cnt;
}

#endif /* VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON */

#if (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosOne32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosOne32(uint32 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get the bit position of the highest set bit */
    pos = VStdLib_Loc_GetHighestBitPosOneInFourBytes(value);
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosZero32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosZero32(uint32 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_FOUR_BYTES)
  {
    /* #110 Get the bit position of the highest unset bit */
    pos = VStdLib_Loc_GetHighestBitPosOneInFourBytes((uint32)(~value));
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosOne32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosOne32(uint32 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get the bit position of the lowest set bit */
    pos = VStdLib_Loc_GetLowestBitPosOneInFourBytes(value);
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosZero32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosZero32(uint32 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_FOUR_BYTES)
  {
    /* #110 Get the bit position of the lowest unset bit */
    pos = VStdLib_Loc_GetLowestBitPosOneInFourBytes((uint32)(~value));
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingOnes32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingOnes32(uint32 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_FOUR_BYTES)
  {
    /* #110 Get count of leading ones */
    cnt = 31u - VStdLib_Loc_GetHighestBitPosOneInFourBytes((uint32)(~value));
  }

  return cnt;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingZeros32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingZeros32(uint32 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get count of leading zeros */
    cnt = 31u - VStdLib_Loc_GetHighestBitPosOneInFourBytes(value);
  }

  return cnt;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingOnes32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingOnes32(uint32 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_FOUR_BYTES)
  {
    /* #110 Get count of trailing ones */
    cnt = VStdLib_Loc_GetLowestBitPosOneInFourBytes((uint32)(~value));
  }

  return cnt;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingZeros32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingZeros32(uint32 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get count of trailing zeros */
    cnt = VStdLib_Loc_GetLowestBitPosOneInFourBytes(value);
  }

  return cnt;
}

#endif /* VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON */

#if ((defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON))

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosOne64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosOne64(uint64 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get the bit position of the highest set bit */
    pos = VStdLib_Loc_GetHighestBitPosOneInEightBytes(value);
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosZero64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosZero64(uint64 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_EIGHT_BYTES)
  {
    /* #110 Get the bit position of the highest unset bit */
    pos = VStdLib_Loc_GetHighestBitPosOneInEightBytes((uint64)(~value));
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosOne64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosOne64(uint64 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get the bit position of the lowest set bit */
    pos = VStdLib_Loc_GetLowestBitPosOneInEightBytes(value);
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosZero64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosZero64(uint64 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 pos = VSTDLIB_BITPOS_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_EIGHT_BYTES)
  {
    /* #110 Get the bit position of the lowest unset bit */
    pos = VStdLib_Loc_GetLowestBitPosOneInEightBytes((uint64)(~value));
  }

  return pos;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingOnes64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingOnes64(uint64 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_EIGHT_BYTES)
  {
    /* #110 Get count of leading ones */
    cnt = 63u - VStdLib_Loc_GetHighestBitPosOneInEightBytes((uint64)(~value));
  }

  return cnt;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingZeros64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingZeros64(uint64 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get count of leading zeros */
    cnt = 63u - VStdLib_Loc_GetHighestBitPosOneInEightBytes(value);
  }

  return cnt;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingOnes64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingOnes64(uint64 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one unset bit */
  if (value != VSTDLIB_MASK_ALLBITS_IN_EIGHT_BYTES)
  {
    /* #110 Get count of trailing ones */
    cnt = VStdLib_Loc_GetLowestBitPosOneInEightBytes((uint64)(~value));
  }

  return cnt;
}

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingZeros64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingZeros64(uint64 value)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 cnt = VSTDLIB_BITCNT_NA;

  /* ----- Implementation ----------------------------------------------- */
  /* #100 Verify that the passed value contains at least one set bit */
  if (value != 0u)
  {
    /* #110 Get count of trailing zeros */
    cnt = VStdLib_Loc_GetLowestBitPosOneInEightBytes(value);
  }

  return cnt;
}

#endif /* (defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) */

#define VSTDLIB_STOP_SEC_CODE
#include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/***********************************************************************************************************************
 *  MISRA JUSTIFICATIONS
 **********************************************************************************************************************/

/* Justification for module-specific MISRA deviations:

 MD_VStdLib_NoCaseBreak:
   Reason:     Break statements are omitted due to performance reasons when using jump tables.
   Risk:       Resulting code is misleading.
   Prevention: Code comments, code inspection and test of the different variants in the component test.

 MD_VStdLib_ParamMod:
   Reason:     Pointers and counts given as function parameters are modified during different algorithm stages.
   Risk:       Wrong pointer access is performed.
   Prevention: Silent analysis, code inspection and test of the different variants in the component test.

 MD_VStdLib_ParamValueMod:
   Reason:     The direct modification of the function parameter leads to code which is easier to read and maintain.
   Risk:       Incorrect expectation of the value of the modified function parameter.
   Prevention: Code inspection and test of the different variants in the component test.

 MD_VStdLib_PtrCast:
   Reason:     Pointer is casted as different memory access sizes are utilized due to performance reasons.
   Risk:       Wrong pointer access is performed.
   Prevention: Code inspection and test of the different variants in the component test.

 MD_VStdLib_PtrCastToInt:
   Reason:     Pointer is casted to an integral type in order to get the target alignment.
   Risk:       None, as only LSBs are evaluated.
   Prevention: None.

 MD_VStdLib_Para2ConstPtr
   Reason:     The address of a parameter is stored in a constant pointer in order to use the VStdLib_MemCpy().
   Risk:       None.
   Prevention: None.

 MD_VStdLib_NotUsedInUnit:
   Reason:     Static inline function is not used within this translation unit, because it is intended to be used as
               a inline function by other modules.
   Risk:       None.
   Prevention: None.

 MD_VStdLib_FileScope:
   Reason:     Constant tables are kept at a central location for a better overview and maintenance.
   Risk:       None.
   Prevention: None.

 MD_VStdLib_Metrics:
   Reason:     No separation of functionality due to performance reasons when using jump tables.
   Risk:       None, as the complexity arises from one single switch statement.
   Prevention: None.

*/

/***********************************************************************************************************************
 *  SILENTBSW JUSTIFICATIONS
 **********************************************************************************************************************/

/* SBSW_JUSTIFICATION_BEGIN

 \ID SBSW_VSTDLIB_CALL_UNMODIFIED
   \DESCRIPTION A function is called with unmodified 'pDst' and 'nCnt' parameters.
   \COUNTERMEASURE \N The caller of the function ensures that 'pDst' and 'nCnt' are valid. These parameters are passed
                      to the callee without modifications. If the parameters are casted there are no side effects.

 \ID SBSW_VSTDLIB_CALL_MODIFIED_CNT
   \DESCRIPTION A static function is called with unmodified 'pDst' parameter and a modified 'nCnt' parameter.
   \COUNTERMEASURE \N The caller of the function ensures that 'pDst' and 'nCnt' are valid. The offset value is
                      always smaller than 4 due to VSTDLIB_ALIGN_OFFSET() and 'nCnt' is 32 or higher at this point.
                      Therefore 'pDst' and 'nCnt' for the callee are valid.

 \ID SBSW_VSTDLIB_CALL_MODIFIED_IDX
   \DESCRIPTION A static function is called with unmodified 'pDst' parameter starting at a specific offset and a modified
                'nCnt' parameter.
   \COUNTERMEASURE \N The caller of the function ensures that 'pDst' starting at offset and 'nCnt' are valid. The index
                      is always smaller equal to 'nCnt' bits starting at offset. Therefore 'pDst' starting at a
                      specific offset and 'nCnt' are valid for the callee.
 
 \ID SBSW_VSTDLIB_CALL_CONVERTED_BITCNT
   \DESCRIPTION A static function is called with unmodified 'pDst' parameter starting at a specific offset and a modified
                'nCnt' parameter.
   \COUNTERMEASURE \N The caller of the function ensures that 'pDst' and 'nCnt' are valid. 'nCntInBytes' is 'nCnt'
                      devided by 8. The devision by 8 converts the bit count in a valid byte count where the byte count
                      defines the same range as the bit count in a different format. Therefore 'pDst' starting at a 
                      specific offset and 'nCnt' are valid for the callee.
 
 \ID SBSW_VSTDLIB_CALL_CONVERTED_BITOFFSET
   \DESCRIPTION A static function is called with unmodified 'pDst' starting at a specific offset and 'nCnt' parameters 
                but converted offset parameter(s).
   \COUNTERMEASURE \N The caller of the function ensures that 'pDst' starting at offset is valid for 'nCnt' bits.
                      The conversion changes offset numbering from sawtooth to monotone or from monotone to sawtooth.
                      The change of the value of the offset from one numbering method to the other is 7 at most. The 
                      offset is therfore still within the same byte. Thus the start byte and the end byte don't change
                      when converting bit counting numbering. Therefore 'pDst' starting at a specific offset and 'nCnt'
                      are valid for the callee.

 \ID SBSW_VSTDLIB_CALL_MODIFIED_PTR_CNT
   \DESCRIPTION A static function can be called with unmodified or modified 'pDst' and 'nCnt' parameters.
   \COUNTERMEASURE \N The caller of the function ensures that 'pDst' and 'nCnt' are valid. In case the function is
                      called with modified parameters (pointer alignment was performed), 'pDst' is incremented by
                      the value of 'nDstOffset' and 'nCnt' is decremented by the same value. Therefore 'pDst'
                      and 'nCnt' for the callee are valid.

 \ID SBSW_VSTDLIB_ACCESS_LOOP
   \DESCRIPTION Linear memory access using a for loop.
   \COUNTERMEASURE \N The caller of this function ensures that 'pDst' and 'nCnt' are valid. The for loop ensures that
                      the loop index is always smaller than nCnt. Therefore only valid memory is accessed.
 
 \ID SBSW_VSTDLIB_ACCESS_LOOP_ENDIDX
   \DESCRIPTION Linear memory access using a for loop.
   \COUNTERMEASURE \N The caller of this function ensures that 'pDst' starting at a specific offset and 'nCnt' bits are 
                      valid. The for loop ensures that the loop index is always smaller than the length of the first and
                      last byte to copy to (number of iterations) which is determined with 'nCnt' bits starting a specific
                      destination offset. Therefore only valid memory is accessed.

 \ID SBSW_VSTDLIB_ACCESS_8_JUMPTABLE
   \DESCRIPTION Linear 8-bit-access using a switch statement.
   \COUNTERMEASURE \N The caller of this function ensures that 'pDst' and 'nCnt' are valid. The constant indices are
                      always smaller than the value of 'nCnt'. Therefore only valid memory is accessed.
 
 \ID SBSW_VSTDLIB_ACCESS_8
   \DESCRIPTION Linear 8-bit-access.
   \COUNTERMEASURE \N The caller of this function ensures that 'pDst' starting at a specific offset and 'dstByteIdx' are
                      valid. Therefore only valid memory is accessed.

 \ID SBSW_VSTDLIB_ACCESS_32_LOOP
   \DESCRIPTION Linear 32-bit-access using a for loop.
   \COUNTERMEASURE \N The caller of this function ensures that 'pDst' and 'nCnt' are valid. The access index 'dPos'
                      is initialized with 0. The loop control variable 'remaining' is decremented by 16 per iteration.
                      The exit condition ensures that at least 16 bytes are valid per iteration. The access index
                      'dPos' plus constant offset ensures that exactly 16 valid bytes are accessed. 'dPos' is
                      incremented by 4 (that equals 16 bytes) after the access.

 \ID SBSW_VSTDLIB_ACCESS_32_LOOP_REMAINING
   \DESCRIPTION Linear 32-bit-access using a for loop.
   \COUNTERMEASURE \N The caller of this function ensures that 'pDst' and 'nCnt' are valid. After the for loop above
                      'remaining' bytes of valid memory start at the address that is indicated by 'dPos'. The loop
                      control variable 'remaining' is decremented by 4 per iteration. The exit condition ensures that
                      at least 4 bytes are valid per iteration. The access index 'dPos' ensures that exactly 4 valid
                      bytes are accessed. 'dPos' is incremented by 1 (that equals 4 bytes) after the access.

 \ID SBSW_VSTDLIB_ACCESS_32_JUMPTABLE
   \DESCRIPTION Linear 32-bit-access using a switch statement.
   \COUNTERMEASURE \N The caller of this function ensures that 'pDst' and 'nCnt' are valid. After the for loop above
                      'remaining' bytes of valid memory start at the address that is indicated by 'dPos'. The switch
                      statement ensures that an access is only made if at least 4 byte are valid. 'dPos' is
                      incremented by 4 after the access.

 \ID SBSW_VSTDLIB_CALL_REMAINING_JUMPTABLE
   \DESCRIPTION A static function is called to access the remaining bytes at the end of processing a 32-bit jump table.
   \COUNTERMEASURE \N The caller of this function ensures that 'pDst' and 'nCnt' are valid. At this point 0-3
                      bytes of valid memory (indicated by the LSBs of 'remaining') start at the address that is
                      indicated by 'dPos'. Due to the usage of VSTDLIB_MASK_3 'pDst' and 'nCnt' for the callee are
                      valid.

 \ID SBSW_VSTDLIB_CALL_REMAINING
   \DESCRIPTION A static function is called to access the remaining bytes after 32-bit operations.
   \COUNTERMEASURE \N The caller of this function ensures that 'pDst' and 'nCnt' are valid. After the for loop above
                      'remaining' bytes of valid memory start at the address that is indicated by 'dPos'. Therefore
                      'pDst' and 'nCnt' for the callee are valid.

 \ID SBSW_VSTDLIB_CALL_REMAINING_PARTIAL
   \DESCRIPTION A static function is called to access the remaining partial word after 32-bit displacement operations.
   \COUNTERMEASURE \N The caller of this function ensures that 'pDst' and 'nCnt' are valid. At this point at least
                      (4 - 'nDstOffset') bytes of valid memory start at the address that is indicated by 'dPos'.
                      Therefore 'pDst' and 'nCnt' for the callee are valid.

 \ID SBSW_VSTDLIB_ACCESS_LOCAL
   \DESCRIPTION Access to a pointer referencing a local variable.
   \COUNTERMEASURE \N The function is only called with valid pointers to local variables.

 \ID SBSW_VSTDLIB_ACCESS_STRUCT_LOCAL
   \DESCRIPTION Access to a pointer referencing a local structure.
   \COUNTERMEASURE \N The function is only called with valid pointers to local structures. The structure elements
                      are modified.

 \ID SBSW_VSTDLIB_CALL_NO_PTR_WRITE
   \DESCRIPTION The static function is called with pointer parameters. The pointers are only accessed for reading.
   \COUNTERMEASURE \N None.

 \ID SBSW_VSTDLIB_CALL_MODIFIABLE_RETVAL
   \DESCRIPTION A static function is called with a pointer parameter 'pRetval' which can be possibly modified.
   \COUNTERMEASURE \N The caller of the function ensures that 'pRetVal' is valid.

\ID SBSW_VSTDLIB_CALL_MODIFIABLE_SRCBYTE
   \DESCRIPTION A static function is called with a pointer parameter 'pSrcByte' which is modified.
   \COUNTERMEASURE \N The caller of the function ensures that 'pSrcByte' is valid.

 \ID SBSW_VSTDLIB_CALL_MODIFIABLE_ITER
   \DESCRIPTION A static function is called with a pointer parameter to a structure 'pSrcIter' or 'pDstIter' where 
                the structure elements are modified.
   \COUNTERMEASURE \N The caller of the function ensures that 'pSrcIter' or 'pDstIter is valid. 

 \ID SBSW_VSTDLIB_ACCESS_VERSIONINFO
   \DESCRIPTION The function VStdLib_GetVersionInfo() writes to the object referenced by parameter 'versioninfo'.
   \COUNTERMEASURE \N The caller ensures that the pointer passed to the parameter 'versioninfo' is valid and references
                      an object of type Std_VersionInfoType.

 \ID SBSW_VSTDLIB_ACCESS_PASSED_ARRAY
   \DESCRIPTION Write access to an element of an array.
                The array is referenced by a pointer parameter. The element is accessed by a static index.
   \COUNTERMEASURE \N [CM_VSTDLIB_ARRAY]

 \ID SBSW_VSTDLIB_ACCESS_PASSED_INT
   \DESCRIPTION Write access to the integer variable which is referenced by a pointer parameter.
   \COUNTERMEASURE \N The caller ensures that the pointer references a valid variable.

 \ID SBSW_VSTDLIB_CALL_MEM_CPY
   \DESCRIPTION The function VStdLib_MemCpy is called with the pointer parameter to an array from the caller
                and a static size for the number of bytes to copy.
   \COUNTERMEASURE \N [CM_VSTDLIB_ARRAY]

 \ID SBSW_VSTDLIB_CALL_CNV_ARRAY_TO_UINT
   \DESCRIPTION The function VStdLib_ConvertUint8ArrayToUint[16,32,64][Big,Little]Endian is called with the pointer
                parameter to an array from the caller.
   \COUNTERMEASURE \N [CM_VSTDLIB_ARRAY]

SBSW_JUSTIFICATION_END */

/*
\CM CM_VSTDLIB_ARRAY The caller ensures that the referenced array is valid for the required size.
*/

/**********************************************************************************************************************
 *  FETA JUSTIFICATIONS
 *********************************************************************************************************************/

/* FETA_JUSTIFICATION_BEGIN

\ID FETA_VSTDLIB_LOC_MEMSET_X_X
\DESCRIPTION Function parameter [nCnt] specifies the number of iterations to use to initialize the destination with a
             pattern value.
\COUNTERMEASURE \N [CM_VSTDLIB_PARA_NCNT]

\ID FETA_VSTDLIB_CALL_LOC_MEMSET_8_X_REMAINING
\DESCRIPTION Function parameter [nCnt] from the callee (VStdLib_Loc_MemSet_8_3 or VStdLib_Loc_MemSet_8_31) specifies
             the number of iteration to use to initialize the destination with a pattern value.
             The value for [nCnt] is determined within this caller. The correctness of the function parameter [nCnt]
             from the caller used for the determination is ensured by FETA_VSTDLIB_CALL_LOC_MEMSET_X_X.
\COUNTERMEASURE \D VStdLib_Loc_MemSet_32_Aligned

\ID FETA_VSTDLIB_CALL_LOC_MEMSET_8_3
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_Loc_MemSet_8_3() specifies the number of iterations to use
             to initialize the destination with a pattern value.
             The value for [nCnt] is determined within this caller.
\COUNTERMEASURE \D VStdLib_MemSet

\ID FETA_VSTDLIB_CALL_LOC_MEMSET_X_X
\DESCRIPTION Function parameter [nCnt] from callee (VStdLib_Loc_MemSet_8_31 or VStdLib_Loc_MemSet_32_Aligned) specifies
             the number of iterations to use to initialize the destination with a pattern value.
             The value for [nCnt] is the value of the caller's function parameter [nCnt].
             The user of the caller (service function VStdLib_MemSet) has to ensure the correctness of value for [nCnt].
\COUNTERMEASURE \S SMI-494148

\ID FETA_VSTDLIB_MEMSET
\DESCRIPTION Function parameter [nCnt] specifies the number of iteration to use to initialize the destination with a
             pattern value.
             The user of the service function VStdLib_MemSet has to ensure the correctness of value for [nCnt].
\COUNTERMEASURE \S SMI-494148

\ID FETA_VSTDLIB_MEMSET_MACRO
\DESCRIPTION Function parameter [nCnt] specifies the number of iteration to use to initialize the destination with a
             pattern value.
             The user of the macro has to ensure the correctness of value for [nCnt].
\COUNTERMEASURE \S SMI-494148


\ID FETA_VSTDLIB_LOC_MEMCPY_X_X
\DESCRIPTION Function parameter [nCnt] specifies the number of iterations to use to copy data from source to destination.
\COUNTERMEASURE \N [CM_VSTDLIB_PARA_NCNT]

\ID FETA_VSTDLIB_CALL_LOC_MEMCPY_8_X_REMAINING
\DESCRIPTION Function parameter [nCnt] from the callee (VStdLib_Loc_MemCpy_8_3 or VStdLib_Loc_MemCpy_8_31) specifies
             the number of iteration to use to copy data from source to destination.
             The value for [nCnt] is determined within this caller. The correctness of the function parameter [nCnt]
             from the caller used for the determination is ensured by FETA_VSTDLIB_CALL_LOC_MEMCPY_X_X.
\COUNTERMEASURE \D VStdLib_Loc_MemCpy_32_Aligned

\ID FETA_VSTDLIB_CALL_LOC_MEMCPY_8_31_REMAINING
\DESCRIPTION Function parameter [nCnt] from calee VStdLib_Loc_MemCpy_8_31 specifies the number of iteration to use to
             copy data from source to destination.
             The value for [nCnt] is determined within this caller. The correctness of the function parameter [nCnt]
             from the caller used for the determination is ensured by FETA_VSTDLIB_CALL_LOC_MEMCPY_X_X.
\COUNTERMEASURE \D VStdLib_Loc_MemCpy_32_Unaligned

\ID FETA_VSTDLIB_CALL_LOC_MEMCPY_8_3
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_Loc_MemCpy_8_3() specifies the number of iterations to use
             to copy data from source to destination.
             The value for [nCnt] is determined within this caller.
\COUNTERMEASURE \D VStdLib_MemCpy

\ID FETA_VSTDLIB_CALL_LOC_MEMCPY_X_X
\DESCRIPTION Function parameter [nCnt] from callee (VStdLib_Loc_MemCpy_8_31 or VStdLib_Loc_MemCpy_32_Aligned or
             VStdLib_Loc_MemCpy_32_Unaligned) specifies the number of iterations to use to copy data from source to
             destination.
             The value for [nCnt] is the value of the caller's function parameter [nCnt].
             The user of the caller (service function VStdLib_MemCpy) has to ensure the correctness of value for [nCnt].
\COUNTERMEASURE \S SMI-494148

\ID FETA_VSTDLIB_CALL_LOC_MEMCPY_8_PARTIALWORD
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_Loc_MemCpy_8_PartialWord specifies the number of iteration
             to use to copy data from source to destination.
             The value for [nCnt] is set by the caller to the value of its function parameter [nDstOffset].
\COUNTERMEASURE \N CM_VSTDLIB_PARA_NCNT

\ID FETA_VSTDLIB_CALL_LOC_MEMCPY_8_PARTIALWORD_CALC
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_Loc_MemCpy_8_PartialWord specifies the number of iteration
             to use to copy data from source to destination.
             The value for [nCnt] is determined (4u - nDstOffset) within this caller. The correctness of the function
             parameter [nDstOffset] from the caller used for the determination is ensured by
             FETA_VSTDLIB_CALL_LOC_MEMCPY_32_UNALIGNED_NDSTOFFSET.
\COUNTERMEASURE \D VStdLib_Loc_MemCpy_32_Unaligned

\ID FETA_VSTDLIB_LOC_MEMCPY_32_UNALIGNED
\DESCRIPTION Local variable [remaining] specifies the number of iteration to use to copy data from source to destination.
             The value of [remaining] is determined within this caller. The correctness of the function
             parameter [nCnt] from the caller used for the determination is ensured by FETA_VSTDLIB_CALL_LOC_MEMCPY_X_X.
\COUNTERMEASURE \D VStdLib_Loc_MemCpy_32_Unaligned

\ID FETA_VSTDLIB_CALL_LOC_MEMCPY_32_UNALIGNED_NDSTOFFSET
\DESCRIPTION Function parameter [nDstOffset] from VStdLib_Loc_MemCpy_32_Unaligned specifies the number of iteration to
             use to copy data from source to destination.
             The value (0, 1, 2 or 3) for [nDstOffset] is determined within this caller.
\COUNTERMEASURE \D VStdLib_MemCpy

\ID FETA_VSTDLIB_MEMCPY
\DESCRIPTION Function parameter [nCnt] specifies the number of iterations to use to copy data from source to destination.
             The user of the service function VStdLib_MemCpy or VStdLib_MemCpy16 or VStdLib_MemCpy32 has to ensure the
             correctness of value for [nCnt].
\COUNTERMEASURE \S SMI-494148

\ID FETA_VSTDLIB_CALL_MEMCPY
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_MemCpy specifies the number of iterations to use to copy
             data from source to destination.
             The value for [nCnt] is the value of the caller's function parameter [nCnt].
             The user of the caller (service function VStdLib_MemCpy_s or VStdLib_MemCpySwapEndian) has to ensure the
             correctness of value for [nCnt].
\COUNTERMEASURE \S SMI-494148

\ID FETA_VSTDLIB_MEMCPY_MACRO
\DESCRIPTION Function parameter [nCnt] specifies the number of iterations to use to copy data from source to destination.
             The user of the macro has to ensure the correctness of value for [nCnt].
\COUNTERMEASURE \S SMI-494148

\ID FETA_VSTDLIB_CALL_MEMCPY_MACRO
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_MemCpyMacro specifies the number of iterations to use to
             copy data from source to destination.
             The value for [nCnt] is the value of the caller's function parameter [nCnt].
             The user of the caller (macro VStdLib_MemCpyMacro_s) has to ensure the correctness of value for [nCnt].
\COUNTERMEASURE \S SMI-494148


\ID FETA_VSTDLIB_LOC_MEMCMP_X_X
\DESCRIPTION Function parameter [nCnt] specifies the number of iterations to use to compare data from buffer1 with
             buffer2.
\COUNTERMEASURE \N CM_VSTDLIB_PARA_NCNT

\ID FETA_VSTDLIB_CALL_LOC_MEMCMP_8_ALIGNED_REMAINING
\DESCRIPTION Function parameter [nCnt] from the callee VStdLib_Loc_MemCmp_32_Aligned specifies the number of iteration
             to use to compare data from buffer1 with buffer2.
             The value for [nCnt] is determined within this caller. The correctness of the function parameter [nCnt]
             from the caller used for the determination is ensured by FETA_VSTDLIB_CALL_LOC_MEMCMP_X_X.
\COUNTERMEASURE \D VStdLib_Loc_MemCmp_32_Aligned

\ID FETA_VSTDLIB_CALL_LOC_MEMCMP_X_X
\DESCRIPTION Function parameter [nCnt] from callee (VStdLib_Loc_MemCmp_8 or VStdLib_MemCmp_32_Aligned or
             VStdLib_MemCmp_32_Unaligned) specifies the number of iterations to use to compare data from buffer1 with
             buffer2.
             The value for [nCnt] is the value of the caller's function parameter [nCnt].
             The user of the caller (service function VStdLib_MemCmp) has to ensure the correctness of value for [nCnt].
\COUNTERMEASURE \S SMI-494148

\ID FETA_VSTDLIB_CALL_LOC_MEMCMP_8_NBUF2OFFSET
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_Loc_MemCmp_8 specifies the number of iteration to use to
             compare data from buffer1 with buffer2.
             The value for [nCnt] is set by the caller to the value of its function parameter [nBuf2Offset].
\COUNTERMEASURE \N CM_VSTDLIB_PARA_NCNT

\ID FETA_VSTDLIB_CALL_LOC_MEMCMP_8_CALC
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_Loc_MemCmp_8 specifies the number of iteration to use to
             compare data from buffer1 with buffer2.
             The value for [nCnt] is determined (4u - nBuf2Offset) within this caller. The correctness of the function
             parameter [nBuf2Offset] from the caller used for the determination is ensured by
             FETA_VSTDLIB_CALL_LOC_MEMCMP_32_UNALIGNED_NBUF2OFFSET.
\COUNTERMEASURE \D VStdLib_Loc_MemCmp_32_Unaligned

\ID FETA_VSTDLIB_CALL_LOC_MEMCMP_8_UNALIGNED_REMAINING
\DESCRIPTION Function parameter [nCnt] from the callee VStdLib_Loc_MemCmp_32_Unaligned specifies the number of iteration
             to use to compare data from buffer1 with buffer2.
             The value for [nCnt] is determined within this caller. The correctness of the function parameter [nCnt]
             from the caller used for the determination is ensured by FETA_VSTDLIB_CALL_LOC_MEMCMP_X_X.
\COUNTERMEASURE \D VStdLib_Loc_MemCmp_32_Unaligned

\ID FETA_VSTDLIB_CALL_LOC_MEMCMP_8_NBUF1OFFSET
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_Loc_MemCmp_8 specifies the number of iteration to use to
             compare data from buffer1 with buffer2.
             The value (0, 1, 2 or 3) for [nCnt] is determined within this caller.
\COUNTERMEASURE \D VStdLib_MemCmp

\ID FETA_VSTDLIB_LOC_MEMCMP_32_UNALIGNED
\DESCRIPTION Local variable [remaining] specifies the number of iteration to use to compare data from buffer1 with
             buffer2.
             The value of [remaining] is determined within this caller. The correctness of the function
             parameter [nCnt] from the caller used for the determination is ensured by FETA_VSTDLIB_CALL_LOC_MEMCMP_X_X.
\COUNTERMEASURE \D VStdLib_Loc_MemCpy_32_Unaligned

\ID FETA_VSTDLIB_CALL_LOC_MEMCMP_32_UNALIGNED_NBUF2OFFSET
\DESCRIPTION Function parameter [nBuf2Offset] from VStdLib_Loc_MemCmp_32_Unaligned specifies the number of iteration to
             compare data from buffer1 with buffer2.
             The value (0, 1, 2 or 3) for [nBuf2Offset] is determined within this caller.
\COUNTERMEASURE \D VStdLib_MemCmp


\ID FETA_VSTDLIB_LOC_MEMCPYSWAPENDIAN_X_X
\DESCRIPTION Function parameter [nCnt] specifies the number of iterations to use to copy and swap data from source to
             destination.
\COUNTERMEASURE \N [CM_VSTDLIB_PARA_NCNT]

\ID FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INTX_X
\DESCRIPTION Function parameter [nCnt] from callee (VStdLib_Loc_MemCpySwapEndian_Int16_32 or
             VStdLib_Loc_MemCpySwapEndian_Int32_32 or VStdLib_Loc_MemCpySwapEndian_Int64_32 or
             VStdLib_Loc_MemCpySwapEndianInt16_32_Aligned or VStdLib_Loc_MemCpySwapEndianInt32_32_Aligned or
             VStdLib_Loc_MemCpySwapEndianInt64_32_Aligned) specifies the number of iterations to use to copy and swap
             data from source to destination.
             The value for [nCnt] is set by the caller to the value of its function parameter [nCnt].
\COUNTERMEASURE \N [CM_VSTDLIB_PARA_NCNT]

\ID FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INT16_32_REMAINING
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_Loc_MemCpySwapEndian_Int16_32 specifies the number of
             iterations to use to copy and swap data from source to destination.
             The value for [nCnt] is determined within this caller. The correctness of the function parameter [nCnt]
             from the caller used for the determination is ensured by FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INTX_X.
\COUNTERMEASURE \D VStdLib_Loc_MemCpySwapEndianInt16_32_Aligned

\ID FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INT32_32_REMAINING
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_Loc_MemCpySwapEndian_Int32_32 specifies the number of
             iterations to use to copy and swap data from source to destination.
             The value for [nCnt] is determined within this caller. The correctness of the function parameter [nCnt]
             from the caller used for the determination is ensured by FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INTX_X.
\COUNTERMEASURE \D VStdLib_Loc_MemCpySwapEndianInt32_32_Aligned

\ID FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INT64_32_REMAINING
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_Loc_MemCpySwapEndian_Int64_32 specifies the number of
             iterations to use to copy and swap data from source to destination.
             The value for [nCnt] is determined within this caller. The correctness of the function parameter [nCnt]
             from the caller used for the determination is ensured by FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_INTX_X.
\COUNTERMEASURE \D VStdLib_Loc_MemCpySwapEndianInt64_32_Aligned

\ID FETA_VSTDLIB_CALL_LOC_MEMCPYSWAPENDIAN_X_X
\DESCRIPTION Function parameter [nCnt] from callee (VStdLib_Loc_MemCpySwapEndian_IntType_32 or 
             VStdLib_Loc_MemCpySwapEndian_32_Aligned or VStdLib_Loc_MemCpySwapEndian) specifies the number of
             iterations to use to compare data from buffer1 with buffer2.
             The value for [nCnt] is the value of the caller's function parameter [nCnt].
             The user of the caller (service function VStdLib_MemCpySwapEndian) has to ensure the correctness of value
             for [nCnt].
\COUNTERMEASURE \S SMI-494148

\ID FETA_VSTDLIB_CALL_MEMCPYSWAPENDIAN
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_MemCpySwapEndian specifies the number of iterations to use
             to copy and swap data from source to destination.
             The value for [nCnt] is the value of the caller's function parameter [nCnt].
             The user of the caller (service function VStdLib_MemCpySwapEndian_s) has to ensure the
             correctness of value for [nCnt].
\COUNTERMEASURE \S SMI-494148


\ID FETA_VSTDLIB_LOC_MEMCHECK_X_NCNT
\DESCRIPTION Function parameter [nCnt] specifies the number of iterations to use to check data from buffer against a
             pattern.
\COUNTERMEASURE \N CM_VSTDLIB_PARA_NCNT

\ID FETA_VSTDLIB_LOC_MEMCHECK_X_RETVAL
\DESCRIPTION Local variable [retVal] is used to break the loop before [nCnt] iterations when the check detects a
             mismatch.
             The value for [retVal] is determined for each iteration by the called function
             VStdLib_Loc_GetMemCheckResult.
\COUNTERMEASURE \D DSGN-VStdLib-MemCheck

\ID FETA_VSTDLIB_CALL_LOC_MEMCHECK_8_REMAINING
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_Loc_MemCheck_8 specifies the number of iterations to use to
             check data from buffer against a pattern.
             The value for [nCnt] is determined within this caller. The correctness of the function parameter [nCnt]
             from the caller used for the determination is ensured by FETA_VSTDLIB_CALL_LOC_MEMCHECK_X.
\COUNTERMEASURE \D VStdLib_Loc_MemCheck_32_Aligned

\ID FETA_VSTDLIB_CALL_LOC_MEMCHECK_8_NBUFOFFSET
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_Loc_MemCheck_8 specifies the number of iterations to use to
             check data from buffer against a pattern.
             The value (0, 1, 2 or 3) for [nCnt] is determined within this caller.
\COUNTERMEASURE \D VStdLib_MemCheck

\ID FETA_VSTDLIB_CALL_LOC_MEMCHECK_X
\DESCRIPTION Function parameter [nCnt] from callee (VStdLib_Loc_MemCheck_8 or VStdLib_Loc_MemCheck_32_Aligned)
             specifies the number of iterations to use to check data from buffer against a pattern.
             The value for [nCnt] is the value of the caller's function parameter [nCnt].
             The user of the caller (service function VStdLib_MemCheck) has to ensure the correctness of value for
             [nCnt].
\COUNTERMEASURE \S SMI-494148


\ID FETA_VSTDLIB_LOC_BITCPYSWAPENDIAN
\DESCRIPTION The function parameter [dstIter] is a pointer to a local struct. The struct holds the integer element
             [lastByte]. [lastByte] specifies the number of iterations to use to copy data from source to destination.
\COUNTERMEASURE \N Caller ensures correctness of value [lastByte] from function parameter [dstIter]

\ID FETA_VSTDLIB_CALL_LOC_BITCPYSWAPENDIAN
\DESCRIPTION The function parameter [dstIter] from the callee VStdLib_Loc_BitCpySwapEndian is a pointer to a local struct.
             The struct holds the integer element [lastByte]. [lastByte] specifies the number of iterations to use to copy
             data from source to destination. The value for [lastByte] is determined within this caller by the local 
             function VStdLib_Loc_BitCpyIter. The correctness of the function parameters [nCnt] and
             [nDstBitOffsetSaw/nDstBitOffsetMon] from the caller used for the determination is ensured by
             FETA_VSTDLIB_CALL_LOC_BITCPY.
\COUNTERMEASURE \D VStdLib_Loc_BitCpyBigToLittleEndian or VStdLib_Loc_BitCpyLittleToBigEndian

\ID FETA_VSTDLIB_CALL_MEMCPY_IN_LOC_BITCPY
\DESCRIPTION Function parameter [nCnt] from callee VStdLib_MemCpy specifies the number of iterations to use to copy data
             from source to destination. The value for [nCnt] is determined within this caller. The correctness of the
             function parameter [nCnt] from the caller used for the determination is ensured by FETA_VSTDLIB_CALL_LOC_BITCPY.
\COUNTERMEASURE \D VStdLib_Loc_BitCpyBigToBigEndian or VStdLib_Loc_BitCpyLittleToLittleEndian

\ID FETA_VSTDLIB_LOC_BITCPY
\DESCRIPTION Function parameters [nCnt] and [nDstBitOffsetSaw/nDstBitOffsetMon] are used by the local function VStdLib_Loc_BitCpyIter
             to determine number of iterations to use to copy data from source to destination. The correctness of the function
             parameters [nCnt] and [nDstBitOffsetSaw/nDstBitOffsetMon] from the caller used for the determination is ensured by
             FETA_VSTDLIB_CALL_LOC_BITCPY.
\COUNTERMEASURE \D DSGN-VStdLib-BitCpy

\ID FETA_VSTDLIB_CALL_LOC_BITCPY
\DESCRIPTION Function parameters [nCnt] and [nDstBitOffsetSaw/nDstBitOffsetMon] from the callee are used to determine the number
             of iterations to use to copy data from source to destination. The value for [nCnt] is the value of the caller's function
             parameter [nCnt]. The user of the caller (service functions for BitCpy) has to ensure the correctness of
             value for [nCnt] and [nDstBitOffsetSaw/nDstBitOffsetMon].
\COUNTERMEASURE \S SMI-494148

FETA_JUSTIFICATION_END */

/*
\CM CM_VSTDLIB_PARA_NCNT Caller ensures correctness of value for function parameter [nCnt].
*/

/**********************************************************************************************************************
 *  END OF FILE: vstdlib.c
 *********************************************************************************************************************/
