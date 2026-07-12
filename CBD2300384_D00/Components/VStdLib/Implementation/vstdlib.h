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
/*!        \file  vstdlib.h
 *        \brief  Interface of the generic Vector Standard Library (VStdLib_GenericAsr)
 *      \details  This is the header file of the VStdLib. It contains the public (inline) APIs and global data of
 *                the VStdLib.
 *                It has to be included by other modules to access the APIs of the VStdLib.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Version   Date        Author  Change Id      Description
 *  -------------------------------------------------------------------------------------------------------------------
 *  01.00.00  2012-05-08  vistvi                 Creation
 *  01.00.01  2013-09-04  vistvi  ESCAN00064708: Inconsistent file names
 *  01.01.00  2013-12-10  vistvi  ESCAN00073131: MISRA Improvements
 *  02.00.00  2015-01-12  vistkr  ESCAN00080277: FEAT-427: SafeBSW Step I
 *                                ESCAN00081720: Extended version check is not available for MSR4
 *            2015-05-04  vistkr  ESCAN00081765: FEAT-1275: SafeBSW Step 2
 *                                ESCAN00082267: FEAT-1379: Optimized copy operation for large data
 *  02.00.01  2015-12-02  vistkr  ESCAN00086942: Compiler error: Last line of file ends without a newline
 *  02.00.02  2016-04-14  vistkr  ESCAN00089506: Remove trace links for memory manipulation services
 *  03.00.00  2018-10-23  vistkr  STORYC-6844:   MISRA-C:2012: VStdLib
 *  03.01.00  2021-01-18  visgaz  CAN-1574:      [VStdLib] Support bit search functions
 *  03.02.00  2021-03-15  visgaz  CAN-1579:      [VStdLib] Support memory comparison functions
 *                                CAN-1577:      [VStdLib] Support serialization functions
 *  03.03.00  2021-04-12  visgaz  CAN-2834:      [VStdLib] Optimize memory comparison functions
 *                                CAN-2831:      [VStdLib] Optimize serialization functions
 *                                CAN-1576:      [VStdLib] Support endianess conversion functions
 *                                ESCAN00108975: Compiler warning: 'type cast': pointer truncation from 'uint8 *' to 'uint32_least'
 *  03.04.00  2021-06-07  visgaz  CANCORE-325:   [VStdLib] Convert CReqSpec, CAD and TestSpec to the new textbased format
 *                                ESCAN00109680: Compiler warning: Conversion from 'uint8 *' to 'uint64' is sign-extended
 *  03.05.00  2022-03-29  visgaz  CANCORE-455:   [VStdLib] Support MemCpy with endianness conversion functions
 *                                ESCAN00110377: Compiler warning: Constant out of range
 *                                ESCAN00110415: Compiler warning: Narrowing or signed-to-unsigned type conversion found
 *                                -              Refactored description for endianness conversion and (de-)serialization feature
 *            2022-07-07  visnaj  CANHAL-1215:   [VStdLib] Support VStdLib_MemCheck()
 *  03.06.00  2023-01-10  visgaz  CANCORE-1197:  [VStdLib] Finite Execution Time Analysis
 *            2023-01-25          CANCORE-1379:  [VStdLib] Safety analysis
 *  03.07.00  2023-02-14  vismte  CANCORE-1647:  [VStdLib] Implementation of sawtooth functions for BitCopy
 *                                CANCORE-1699:  [VStdLib] Implementation of monotone functions for BitCopy
 *            2023-03-23  vismte  ESCAN00114258: Incomplete description for FETA-ID FETA_VSTDLIB_CALL_LOC_MEMCHECK_8_REMAINING
 *********************************************************************************************************************/

#ifndef VSTDLIB_H
#define VSTDLIB_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "Std_Types.h"

# include "VStdLib_Cfg.h"

# if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
#  include "Det.h"
# endif

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* Vendor and module identification */
# define VSTDLIB_VENDOR_ID                   (30u)
# if !defined (VSTDLIB_MODULE_ID)
#  define VSTDLIB_MODULE_ID                  (255u)
# endif

/* Component version information (decimal version of ALM implementation package) */
# define VSTDLIB_SW_MAJOR_VERSION            (3u)
# define VSTDLIB_SW_MINOR_VERSION            (7u)
# define VSTDLIB_SW_PATCH_VERSION            (0u)

# if !defined (VSTDLIB_INSTANCE_ID_DET)
#  define VSTDLIB_INSTANCE_ID_DET            (0u)
# endif

/* API service IDs */
# define VSTDLIB_SID_MEM_SET                 (0x00u) /*!< Service ID: VStdLib_MemClr(), VStdLib_MemSet() */
# define VSTDLIB_SID_MEM_COPY                (0x01u) /*!< Service ID: VStdLib_MemCpy(), VStdLib_MemCpyEndianPlatformToBig(), VStdLib_MemCpyEndianPlatformToLittle(),
                                                                      VStdLib_MemCpyEndianBigToPlatform(), VStdLib_MemCpyEndianLittleToPlatform() */
# define VSTDLIB_SID_MEM_COPY_16             (0x02u) /*!< Service ID: VStdLib_MemCpy16() */
# define VSTDLIB_SID_MEM_COPY_32             (0x03u) /*!< Service ID: VStdLib_MemCpy32() */
# define VSTDLIB_SID_MEM_COPY_S              (0x04u) /*!< Service ID: VStdLib_MemCpy_s(), VStdLib_MemCpyEndianPlatformToBig_s(), VStdLib_MemCpyEndianPlatformToLittle_s(),
                                                                      VStdLib_MemCpyEndianBigToPlatform_s(), VStdLib_MemCpyEndianLittleToPlatform_s() */
# define VSTDLIB_SID_GET_VERSION_INFO        (0x05u) /*!< Service ID: VStdLib_GetVersionInfo() */
# define VSTDLIB_SID_MEM_CMP                 (0x06u) /*!< Service ID: VStdLib_MemCmp() */
# define VSTDLIB_SID_CNV_UINT16_TO_ARRAY_BE  (0x07u) /*!< Service ID: VStdLib_ConvertUint16ToUint8ArrayBigEndian(), VStdLib_ConvertSint16ToUint8ArrayBigEndian() */
# define VSTDLIB_SID_CNV_UINT16_TO_ARRAY_LE  (0x08u) /*!< Service ID: VStdLib_ConvertUint16ToUint8ArrayLittleEndian(), VStdLib_ConvertSint16ToUint8ArrayLittleEndian() */
# define VSTDLIB_SID_CNV_ARRAY_TO_UINT16_BE  (0x09u) /*!< Service ID: VStdLib_ConvertUint8ArrayToUint16BigEndian(), VStdLib_ConvertUint8ArrayToSint16BigEndian() */
# define VSTDLIB_SID_CNV_ARRAY_TO_UINT16_LE  (0x0Au) /*!< Service ID: VStdLib_ConvertUint8ArrayToUint16LittleEndian(), VStdLib_ConvertUint8ArrayToSint16LittleEndian() */
# define VSTDLIB_SID_CNV_UINT32_TO_ARRAY_BE  (0x0Bu) /*!< Service ID: VStdLib_ConvertUint32ToUint8ArrayBigEndian(), VStdLib_ConvertSint32ToUint8ArrayBigEndian() */
# define VSTDLIB_SID_CNV_UINT32_TO_ARRAY_LE  (0x0Cu) /*!< Service ID: VStdLib_ConvertUint32ToUint8ArrayLittleEndian(), VStdLib_ConvertSint32ToUint8ArrayLittleEndian() */
# define VSTDLIB_SID_CNV_ARRAY_TO_UINT32_BE  (0x0Du) /*!< Service ID: VStdLib_ConvertUint8ArrayToUint32BigEndian(), VStdLib_ConvertUint8ArrayToSint32BigEndian() */
# define VSTDLIB_SID_CNV_ARRAY_TO_UINT32_LE  (0x0Eu) /*!< Service ID: VStdLib_ConvertUint8ArrayToUint32LittleEndian(), VStdLib_ConvertUint8ArrayToSint32LittleEndian() */
# define VSTDLIB_SID_CNV_UINT64_TO_ARRAY_BE  (0x0Fu) /*!< Service ID: VStdLib_ConvertUint64ToUint8ArrayBigEndian(), VStdLib_ConvertSint64ToUint8ArrayBigEndian() */
# define VSTDLIB_SID_CNV_UINT64_TO_ARRAY_LE  (0x10u) /*!< Service ID: VStdLib_ConvertUint64ToUint8ArrayLittleEndian(), VStdLib_ConvertSint64ToUint8ArrayLittleEndian() */
# define VSTDLIB_SID_CNV_ARRAY_TO_UINT64_BE  (0x11u) /*!< Service ID: VStdLib_ConvertUint8ArrayToUint64BigEndian(), VStdLib_ConvertUint8ArrayToSint64BigEndian() */
# define VSTDLIB_SID_CNV_ARRAY_TO_UINT64_LE  (0x12u) /*!< Service ID: VStdLib_ConvertUint8ArrayToUint64LittleEndian(), VStdLib_ConvertUint8ArrayToSint64LittleEndian() */
# define VSTDLIB_SID_MEM_COPY_SWAP_ENDIAN    (0x13u) /*!< Service ID: VStdLib_MemCpySwapEndian(), VStdLib_MemCpyEndianPlatformToBig(),
                                                                      VStdLib_MemCpyEndianPlatformToLittle(), VStdLib_MemCpyEndianBigToPlatform(),
                                                                      VStdLib_MemCpyEndianLittleToPlatform() */
# define VSTDLIB_SID_MEM_COPY_SWAP_ENDIAN_S  (0x14u) /*!< Service ID: VStdLib_MemCpySwapEndian_s(), VStdLib_MemCpyEndianPlatformToBig_s(),
                                                                      VStdLib_MemCpyEndianPlatformToLittle_s(), VStdLib_MemCpyEndianBigToPlatform_s(),
                                                                      VStdLib_MemCpyEndianLittleToPlatform_s() */
# define VSTDLIB_SID_MEM_CHECK               (0x15u) /*!< Service ID: VStdLib_MemCheck() */

# define VSTDLIB_SID_BIT_COPY_SAW            (0x16u) /*!< Service ID: VStdLib_BitCpySawBigToBigEndian(), VStdLib_BitCpySawBigToLittleEndian(),
                                                                      VStdLib_BitCpySawLittleToBigEndian(), VStdLib_BitCpySawLittleToLittleEndian() */
# define VSTDLIB_SID_BIT_COPY_MON            (0x17u) /*!< Service ID: VStdLib_BitCpyMonBigToBigEndian(), VStdLib_BitCpyMonBigToLittleEndian(),
                                                                      VStdLib_BitCpyMonLittleToBigEndian(), VStdLib_BitCpyMonLittleToLittleEndian() */
# define VSTDLIB_SID_BIT_COPY_SAW_S          (0x18u) /*!< Service ID: VStdLib_BitCpySawBigToBigEndian_s(), VStdLib_BitCpySawBigToLittleEndian_s(),
                                                                      VStdLib_BitCpySawLittleToBigEndian_s(), VStdLib_BitCpySawLittleToLittleEndian_s() */
# define VSTDLIB_SID_BIT_COPY_MON_S          (0x19u) /*!< Service ID: VStdLib_BitCpyMonBigToBigEndian_s(), VStdLib_BitCpyMonBigToLittleEndian_s(),
                                                                      VStdLib_BitCpyMonLittleToBigEndian_s(), VStdLib_BitCpyMonLittleToLittleEndian_s() */

/* Error codes */
# define VSTDLIB_E_NO_ERROR                  (0x00u) /*!< Used to check if no error occurred */
# define VSTDLIB_E_PARAM_POINTER             (0x01u) /*!< Error code: API used with invalid pointer parameter (e.g. NULL) */
# define VSTDLIB_E_PARAM_SIZE                (0x02u) /*!< Error code: API used with invalid destination buffer size */
# define VSTDLIB_E_PARAM_INTTYPE             (0x03u) /*!< Error code: API used with invalid integer type parameter */

/* Large data support */
# if !defined (VSTDLIB_SUPPORT_LARGE_DATA)
#  define VSTDLIB_SUPPORT_LARGE_DATA         STD_ON
# endif

/* Return value from bit search APIs, if bit position or bit count is not available */
# define VSTDLIB_BITPOS_NA                   (0x80u)
# define VSTDLIB_BITCNT_NA                   VSTDLIB_BITPOS_NA

/*! Possible values for the integer type parameter of the memory copy with endianness conversion APIs */
# define VSTDLIB_INTTYPE_8                   (1u)
# define VSTDLIB_INTTYPE_16                  (2u)
# define VSTDLIB_INTTYPE_32                  (4u)
# if defined (PLATFORM_SUPPORT_SINT64_UINT64)
#  define VSTDLIB_INTTYPE_64                 (8u)
# endif

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
 
/* Module specific definition of LOCAL_INLINE */
# if !defined (VSTDLIB_LOCAL_INLINE)
#  define VSTDLIB_LOCAL_INLINE               LOCAL_INLINE
# endif

/*! Data type for nCnt API parameters */
# if (VSTDLIB_SUPPORT_LARGE_DATA == STD_ON)
typedef uint32_least VStdLib_CntType;
# else
typedef uint16_least VStdLib_CntType;
# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS (MACRO API)
 *********************************************************************************************************************/

/* PRQA S 3453 VSTDLIB_FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

# if (VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_MemClr()
 *********************************************************************************************************************/
/*! \brief       Initializes memory to zero.
 *  \details     Sets nCnt bytes starting at pDst to zero.
 *  \param[out]  pDst          Pointer to the memory location to be initialized, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to initialize, pDst must be valid for this amount.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_LIBRARY_FUNCTIONS == STD_ON,
 *               else it is realized by a call to VStdLib_MemSet() with 'nPattern' == 0.
 *               The compatible definition VStdLib_MemClrLarge() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area that is writable from the calling
 *               context.
 *********************************************************************************************************************/
#  define VStdLib_MemClr(pDst, nCnt)                       VStdLib_MemSet((pDst), 0, (nCnt))

# endif /* VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON */

# if (VSTDLIB_SUPPORT_LARGE_DATA == STD_ON)

/**********************************************************************************************************************
 *  VStdLib_MemClrLarge()
 *********************************************************************************************************************/
/*! \brief       Initializes memory to zero.
 *  \details     Sets nCnt bytes starting at pDst to zero.
 *  \param[out]  pDst          Pointer to the memory location to be initialized, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to initialize, pDst must be valid for this amount.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area that is writable from the calling
 *               context.
 *********************************************************************************************************************/
#  define VStdLib_MemClrLarge(pDst, nCnt)                    VStdLib_MemClr((pDst), (nCnt))

/**********************************************************************************************************************
 *  VStdLib_MemSetLarge()
 *********************************************************************************************************************/
/*! \brief       Initializes memory to a specified pattern.
 *  \details     Sets nCnt bytes starting at pDst to the character nPattern.
 *  \param[out]  pDst          Pointer to the memory location to be initialized, must not be NULL.
 *  \param[in]   nPattern      The character to be used to initialize the memory.
 *  \param[in]   nCnt          Number of bytes to initialize, pDst must be valid for this amount.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area that is writable from the calling
 *               context.
 *********************************************************************************************************************/
#  define VStdLib_MemSetLarge(pDst, nPattern, nCnt)          VStdLib_MemSet((pDst), (nPattern), (nCnt))

/**********************************************************************************************************************
 *  VStdLib_MemCpyLarge()
 *********************************************************************************************************************/
/*! \brief       Copies data from one memory location to another.
 *  \details     Copies nCnt bytes starting at pSrc to another memory location starting at pDst.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area that is writable from the calling
 *               context.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyLarge(pDst, pSrc, nCnt)              VStdLib_MemCpy((pDst), (pSrc), (nCnt))

/**********************************************************************************************************************
 *  VStdLib_MemCpy16Large()
 *********************************************************************************************************************/
/*! \brief       Copies data from one memory location to another.
 *  \details     Copies nCnt 16-bit blocks starting at pSrc to another memory location starting at pDst.
 *  \param[out]  pDst          Pointer to the memory location to copy to, 16-bit aligned and not NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, 16-bit aligned and not NULL.
 *  \param[in]   nCnt          Number of 16-bit blocks to copy, pDst must be valid for this amount.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area that is writable from the calling
 *               context.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
#  define VStdLib_MemCpy16Large(pDst, pSrc, nCnt)            VStdLib_MemCpy16((pDst), (pSrc), (nCnt))

/**********************************************************************************************************************
 *  VStdLib_MemCpy32Large()
 *********************************************************************************************************************/
/*! \brief       Copies data from one memory location to another.
 *  \details     Copies nCnt 32-bit blocks starting at pSrc to another memory location starting at pDst.
 *  \param[out]  pDst          Pointer to the memory location to copy to, 32-bit aligned and not NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, 32-bit aligned and not NULL.
 *  \param[in]   nCnt          Number of 32-bit blocks to copy, pDst must be valid for this amount.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area that is writable from the calling
 *               context.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
#  define VStdLib_MemCpy32Large(pDst, pSrc, nCnt)            VStdLib_MemCpy32((pDst), (pSrc), (nCnt))

/**********************************************************************************************************************
 *  VStdLib_MemCpyLarge_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies data from one memory location to another.
 *  \details     Uses VStdLib_MemCpy() to copy nCnt bytes starting at pSrc to another memory location starting
 *               at pDst, if nDstSize is greater than or equal to nCnt.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize      Maximum number of bytes available in the destination (typically the size of the
 *                             destination object).
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nDstSize'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nDstSize' have to define a valid memory area that is writable from the
 *               calling context.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyLarge_s(pDst, nDstSize, pSrc, nCnt)  VStdLib_MemCpy_s((pDst), (nDstSize), (pSrc), (nCnt))

/**********************************************************************************************************************
 *  VStdLib_MemCmpLarge()
 *********************************************************************************************************************/
/*! \brief       Compares two buffers for the specified number of bytes and returns whether they are equal or not.
 *  \details     -
 *  \param[in]   pBuf1     Pointer to first buffer for comparison, must not be NULL.
 *  \param[in]   pBuf2     Pointer to second buffer for comparison, must not be NULL.
 *  \param[in]   nCnt      Number of bytes to compare, pBuf1 and pBuf2 must be valid for this amount.
 *  \return      < 0       The first buffer contains a smaller value than the second buffer.
 *  \return      0         The two buffers are equal for the number of compared bytes.
 *  \return      > 0       The first buffer contains a greater value than the second buffer.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pBuf1' and 'pBuf2' have to point to valid memory areas. Each memory area has to be
 *               valid for at least the byte count defined in parameter 'nCnt'.
 *               The memory areas defined by parameters 'pBuf1', 'pBuf2' and 'nCnt' must not be changed by another
 *               context whilethe comparison is being executed.
 *********************************************************************************************************************/
#  define VStdLib_MemCmpLarge(pBuf1, pBuf2, nCnt)        VStdLib_MemCmp((pBuf1), (pBuf2), (nCnt))

/**********************************************************************************************************************
 *  VStdLib_MemCpySwapEndianLarge()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bytes starting at pSrc to another memory location starting at pDst in the swapped
 *               endianness according to the integer type intType.
 *  \details     Depending on the configuration (VSTDLIB_RUNTIME_OPTIMIZATION) and the passed arguments for parameter
 *               intType and nCnt different memory access sizes are used (see TechRef).
 *               intType value VSTDLIB_INTTYPE_64 is only supported if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType') and writable from the calling context.
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
#  define VStdLib_MemCpySwapEndianLarge(pDst, pSrc, nCnt, intType) \
    VStdLib_MemCpySwapEndian((pDst), (pSrc), (nCnt), (intType))

/**********************************************************************************************************************
 *  VStdLib_MemCpySwapEndianLarge_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies data from one memory location to another in the swapped
 *               endianness according to the integer type intType.
 *  \details     If nDstSize is greater than or equal to nCnt, it uses VStdLib_MemCpySwapEndian() to copy and swap nCnt
 *               bytes starting at pSrc to another memory location starting at pDst.
 *               Otherwise no copying is performed and the passed errorCode is returned.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize      Maximum number of bytes available in the destination (typically the size of the
 *                             destination object).
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \param[in]   errorCode     Value to be returned, if nCnt exceeds nDstSize.
 *                             Should not be equal to E_OK.
 *  \return      E_OK          Copying was performed.
 *  \return      errorCode     nCnt exceeds nDstSize. No copying was performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nDstSize'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nDstSize' have to define a valid memory area that is writable from the
 *               calling context.
 *               The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType').
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
#  define VStdLib_MemCpySwapEndianLarge_s(pDst, nDstSize, pSrc, nCnt, intType, errorCode) \
    VStdLib_MemCpySwapEndian_s((pDst), (nDstSize), (pSrc), (nCnt), (intType), (errorCode));

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianPlatformToBigLarge()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bytes starting at pSrc (platform endianness) to another memory location starting at pDst
 *               in big endian according to the integer type intType.
 *  \details     Depending on the configuration (VSTDLIB_RUNTIME_OPTIMIZATION) and the passed arguments for parameter 
 *               intType and nCnt different memory access sizes are used (see TechRef).
 *               intType value VSTDLIB_INTTYPE_64 is only supported if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType') and writable from the calling context.
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyEndianPlatformToBigLarge(pDst, pSrc, nCnt, intType) \
    VStdLib_MemCpyEndianPlatformToBig((pDst), (pSrc), (nCnt), (intType))

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianPlatformToBigLarge_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies data from one memory location (platform endianness) to
 *               another in big endian according to the integer type intType.
 *  \details     If nDstSize is greater than or equal to nCnt, it uses VStdLib_MemCpy()/VStdLib_MemCpySwapEndian() to
 *               copy nCnt bytes starting at pSrc to another memory location starting at pDst in big endian.
 *               Otherwise no copying is performed and the passed errorCode is returned.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize      Maximum number of bytes available in the destination (typically the size of the
 *                             destination object).
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \param[in]   errorCode     Value to be returned, if nCnt exceeds nDstSize.
 *                             Should not be equal to E_OK.
 *  \return      E_OK          Copying was performed.
 *  \return      errorCode     nCnt exceeds nDstSize. No copying was performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nDstSize'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nDstSize' have to define a valid memory area that is writable from the
 *               calling context.
 *               The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType').
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyEndianPlatformToBigLarge_s(pDst, nDstSize, pSrc, nCnt, intType, errorCode) \
    VStdLib_MemCpyEndianPlatformToBig_s((pDst), (nDstSize), (pSrc), (nCnt), (intType), (errorCode))

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianPlatformToLittleLarge()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bytes starting at pSrc (platform endianness) to another memory location starting at pDst
 *               in little endian according to the integer type intType.
 *  \details     Depending on the configuration (VSTDLIB_RUNTIME_OPTIMIZATION) and the passed arguments for parameter
 *               intType and nCnt different memory access sizes are used (see TechRef).
 *               intType value VSTDLIB_INTTYPE_64 is only supported if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType') and writable from the calling context.
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyEndianPlatformToLittleLarge(pDst, pSrc, nCnt, intType) \
    VStdLib_MemCpyEndianPlatformToLittle((pDst), (pSrc), (nCnt), (intType))

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianPlatformToLittleLarge_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies data from one memory location (platform endianness) to
 *               another in little endian according to the integer type intType.
 *  \details     If nDstSize is greater than or equal to nCnt, it uses VStdLib_MemCpy()/VStdLib_MemCpySwapEndian() to
 *               copy nCnt bytes starting at pSrc to another memory location starting at pDst in little endian.
 *               Otherwise no copying is performed and the passed errorCode is returned.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize      Maximum number of bytes available in the destination (typically the size of the
 *                             destination object).
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \param[in]   errorCode     Value to be returned, if nCnt exceeds nDstSize.
 *                             Should not be equal to E_OK.
 *  \return      E_OK          Copying was performed.
 *  \return      errorCode     nCnt exceeds nDstSize. No copying was performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nDstSize'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nDstSize' have to define a valid memory area.
 *               The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType') that is writable from the
 *               calling context.
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyEndianPlatformToLittleLarge_s(pDst, nDstSize, pSrc, nCnt, intType, errorCode) \
    VStdLib_MemCpyEndianPlatformToLittle_s((pDst), (nDstSize), (pSrc), (nCnt), (intType), (errorCode))

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianBigToPlatformLarge()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bytes starting at pSrc (big endian) to another memory location starting at pDst in the
 *               platform endianness according to the integer type intType.
 *  \details     Depending on the configuration (VSTDLIB_RUNTIME_OPTIMIZATION) and the passed arguments for parameter
 *               intType and nCnt different memory access sizes are used (see TechRef).
 *               intType value VSTDLIB_INTTYPE_64 is only supported if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType') and writable from the calling context.
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyEndianBigToPlatformLarge(pDst, pSrc, nCnt, intType) \
    VStdLib_MemCpyEndianBigToPlatform((pDst), (pSrc), (nCnt), (intType))

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianBigToPlatformLarge_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies data from one memory location (big endian) to another in the
 *               platform endianness according to the integer type intType.
 *  \details     If nDstSize is greater than or equal to nCnt, it uses VStdLib_MemCpy()/VStdLib_MemCpySwapEndian() to
 *               copy nCnt bytes starting at pSrc to another memory location starting at pDst in the platform
 *               endianness Otherwise no copying is performed and the passed errorCode is returned.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize      Maximum number of bytes available in the destination (typically the size of the
 *                             destination object).
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \param[in]   errorCode     Value to be returned, if nCnt exceeds nDstSize.
 *                             Should not be equal to E_OK.
 *  \return      E_OK          Copying was performed.
 *  \return      errorCode     nCnt exceeds nDstSize. No copying was performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nDstSize'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nDstSize' have to define a valid memory area that is writable from the
 *               calling context.
 *               The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType').
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyEndianBigToPlatformLarge_s(pDst, nDstSize, pSrc, nCnt, intType, errorCode) \
    VStdLib_MemCpyEndianBigToPlatform_s((pDst), (nDstSize), (pSrc), (nCnt), (intType), (errorCode))

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianLittleToPlatformLarge()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bytes starting at pSrc (little endian) to another memory location starting at pDst in the
 *               platform endianness according to the integer type intType.
 *  \details     Depending on the configuration (VSTDLIB_RUNTIME_OPTIMIZATION) and the passed arguments for parameter
 *               intType and nCnt different memory access sizes are used (see TechRef).
 *               intType value VSTDLIB_INTTYPE_64 is only supported if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType') and writable from the calling context.
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyEndianLittleToPlatformLarge(pDst, pSrc, nCnt, intType) \
    VStdLib_MemCpyEndianLittleToPlatform((pDst), (pSrc), (nCnt), (intType))

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianLittleToPlatformLarge_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies data from one memory location (little endian) to another in
 *               the platform endianness according to the integer type intType.
 *  \details     If nDstSize is greater than or equal to nCnt, it uses VStdLib_MemCpy()/VStdLib_MemCpySwapEndian() to
 *               copy nCnt bytes starting at pSrc to another memory location starting at pDst in the platform
 *               endianness. Otherwise no copying is performed and the passed errorCode is returned.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize      Maximum number of bytes available in the destination (typically the size of the
 *                             destination object).
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \param[in]   errorCode     Value to be returned, if nCnt exceeds nDstSize.
 *                             Should not be equal to E_OK.
 *  \return      E_OK          Copying was performed.
 *  \return      errorCode     nCnt exceeds nDstSize. No copying was performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nDstSize'
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pDst' and 'nDstSize' have to define a valid memory area that is writable from the
 *               calling context.
 *               The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType').
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyEndianLittleToPlatformLarge_s(pDst, nDstSize, pSrc, nCnt, intType, errorCode) \
    VStdLib_MemCpyEndianLittleToPlatform_s((pDst), (nDstSize), (pSrc), (nCnt), (intType), (errorCode))

/**********************************************************************************************************************
 *  VStdLib_MemCheckLarge()
 *********************************************************************************************************************/
/*! \brief       Compares the content of a memory area to a specified pattern.
 *  \details     Compares nCnt bytes of memory starting at pBuf to the character nPattern.
 *  \param[in]   pBuf          Pointer to the memory location to be checked, must not be NULL.
 *  \param[in]   nPattern      The character to be checked for in the memory.
 *  \param[in]   nCnt          Number of bytes to be checked, pBuf must be valid for this amount.
 *  \param[in]   errorCode     Value to be returned, if content of pBuf is not equal to pattern.
 *                             Should not be equal to E_OK.
 *  \return      E_OK          Content of pBuf is equal to pattern.
 *  \return      errorCode     Content of pBuf is not equal to pattern.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      VSTDLIB_SUPPORT_LARGE_DATA == STD_ON
 *  \pre         The parameters 'pBuf' and 'nCnt' have to define a valid memory area.
 *               The memory area defined by parameters 'pBuf' and 'nCnt' must not be changed by another context while
 *               the comparison is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
#  define VStdLib_MemCheckLarge(pBuf, nPattern, nCnt, errorCode)  VStdLib_MemCheck((pBuf), (nPattern), (nCnt), (errorCode))

# endif /* VSTDLIB_SUPPORT_LARGE_DATA == STD_ON */

# if (VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_ConvertSint16ToUint8ArrayBigEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 16-bit value to an unsigned 8-bit array according to big endian.
 *  \details     -
 *  \param[in]   src   The signed 16-bit value, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 8-bit array, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'dst' has to point to an unsigned 8-bit array of at least two elements that is writable
 *               from the calling context.
 *********************************************************************************************************************/
# define VStdLib_ConvertSint16ToUint8ArrayBigEndian(src, dst)  VStdLib_ConvertUint16ToUint8ArrayBigEndian(((uint16)(src)), (dst))

/**********************************************************************************************************************
 *  VStdLib_ConvertSint16ToUint8ArrayLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 16-bit value to an unsigned 8-bit array according to little endian.
 *  \details     -
 *  \param[in]   src   The signed 16-bit value, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 8-bit array, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'dst' has to point to an unsigned 8-bit array of at least two elements that is writable
 *               from the calling context.
 *********************************************************************************************************************/
# define VStdLib_ConvertSint16ToUint8ArrayLittleEndian(src, dst)  VStdLib_ConvertUint16ToUint8ArrayLittleEndian(((uint16)(src)), (dst))

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToSint16BigEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 8-bit array to a signed 16-bit value according to big endian.
 *  \details     -
 *  \param[in]   src   Pointer to an unsigned 8-bit array, which shall be converted.
 *  \param[out]  dst   Pointer to a signed 16-bit variable, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'src' has to point to an unsigned 8-bit array of at least two elements.
 *               The parameter 'dst' has to point to a signed 16-bit variable that is writable from the calling
 *               context.
 *********************************************************************************************************************/
# define VStdLib_ConvertUint8ArrayToSint16BigEndian(src, dst)  VStdLib_ConvertUint8ArrayToUint16BigEndian((src), ((uint16*)(dst)))

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToSint16LittleEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 8-bit array to a signed 16-bit value according to little endian.
 *  \details     -
 *  \param[in]   src   Pointer to an unsigned 8-bit array, which shall be converted.
 *  \param[out]  dst   Pointer to a signed 16-bit variable, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'src' has to point to an unsigned 8-bit array of at least two elements.
 *               The parameter 'dst' has to point to a signed 16-bit variable that is writable from the calling
 *               context.
 *********************************************************************************************************************/
# define VStdLib_ConvertUint8ArrayToSint16LittleEndian(src, dst)  VStdLib_ConvertUint8ArrayToUint16LittleEndian((src), ((uint16*)(dst)))

# endif /* VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON */

# if (VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_ConvertSint32ToUint8ArrayBigEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 32-bit value to an unsigned 8-bit array according to big endian.
 *  \details     -
 *  \param[in]   src   The signed 32-bit value, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 8-bit array, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'dst' has to point to an unsigned 8-bit array of at least four elements that is writable
 *               from the calling context.
 *********************************************************************************************************************/
# define VStdLib_ConvertSint32ToUint8ArrayBigEndian(src, dst)  VStdLib_ConvertUint32ToUint8ArrayBigEndian(((uint32)(src)), (dst))

/**********************************************************************************************************************
 *  VStdLib_ConvertSint32ToUint8ArrayLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 32-bit value to an unsigned 8-bit array according to little endian.
 *  \details     -
 *  \param[in]   src   The signed 32-bit value, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 8-bit array, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'dst' has to point to an unsigned 8-bit array of at least four elements that is writable
 *               from the calling context.
 *********************************************************************************************************************/
# define VStdLib_ConvertSint32ToUint8ArrayLittleEndian(src, dst)  VStdLib_ConvertUint32ToUint8ArrayLittleEndian(((uint32)(src)), (dst))

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToSint32BigEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 8-bit array to a signed 32-bit value according to big endian.
 *  \details     -
 *  \param[in]   src   Pointer to an unsigned 8-bit array, which shall be converted.
 *  \param[out]  dst   Pointer to a signed 32-bit variable, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'src' has to point to an unsigned 8-bit array of at least two elements.
 *               The parameter 'dst' has to point to a signed 32-bit variable that is writable from the calling
 *               context.
 *********************************************************************************************************************/
# define VStdLib_ConvertUint8ArrayToSint32BigEndian(src, dst)  VStdLib_ConvertUint8ArrayToUint32BigEndian(src, ((uint32*)(dst)))

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToSint32LittleEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 8-bit array to a signed 32-bit value according to little endian.
 *  \details     -
 *  \param[in]   src   Pointer to an unsigned 8-bit array, which shall be converted.
 *  \param[out]  dst   Pointer to a signed 32-bit variable, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'src' has to point to an unsigned 8-bit array of at least two elements.
 *               The parameter 'dst' has to point to a signed 32-bit variable that is writable from the calling
 *               context.
 *********************************************************************************************************************/
# define VStdLib_ConvertUint8ArrayToSint32LittleEndian(src, dst)  VStdLib_ConvertUint8ArrayToUint32LittleEndian((src), ((uint32*)(dst)))

# endif /* VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON */

# if (defined (PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON))

/**********************************************************************************************************************
 *  VStdLib_ConvertSint64ToUint8ArrayBigEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 64-bit value to an unsigned 8-bit array according to big endian.
 *  \details     -
 *  \param[in]   src   The signed 64-bit value, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 8-bit array, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'dst' has to point to an unsigned 8-bit array of at least eight elements that is writable
 *               from the calling context.
 *********************************************************************************************************************/
#  define VStdLib_ConvertSint64ToUint8ArrayBigEndian(src, dst)  VStdLib_ConvertUint64ToUint8ArrayBigEndian(((uint64)(src)), (dst))

/**********************************************************************************************************************
 *  VStdLib_ConvertSint64ToUint8ArrayLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 64-bit value to an unsigned 8-bit array according to little endian.
 *  \details     -
 *  \param[in]   src   The signed 64-bit value, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 8-bit array, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'dst' has to point to an unsigned 8-bit array of at least eight elements that is writable
 *               from the calling context.
 *********************************************************************************************************************/
#  define VStdLib_ConvertSint64ToUint8ArrayLittleEndian(src, dst)  VStdLib_ConvertUint64ToUint8ArrayLittleEndian(((uint64)(src)), (dst))

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToSint64BigEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 8-bit array to a signed 64-bit value according to big endian.
 *  \details     -
 *  \param[in]   src   Pointer to an unsigned 8-bit array, which shall be converted.
 *  \param[out]  dst   Pointer to a signed 64-bit variable, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'src' has to point to an unsigned 8-bit array of at least eight elements.
 *               The parameter 'dst' has to point to a signed 64-bit variable that is writable from the calling
 *               context.
 *********************************************************************************************************************/
#  define VStdLib_ConvertUint8ArrayToSint64BigEndian(src, dst)  VStdLib_ConvertUint8ArrayToUint64BigEndian((src), ((uint64*)(dst)))

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToSint64LittleEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 8-bit array to a signed 64-bit value according to little endian.
 *  \details     -
 *  \param[in]   src   Pointer to an unsigned 8-bit array, which shall be converted.
 *  \param[out]  dst   Pointer to a signed 64-bit variable, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'src' has to point to an unsigned 8-bit array of at least eight elements.
 *               The parameter 'dst' has to point to a signed 64-bit variable that is writable from the calling
 *               context.
 *********************************************************************************************************************/
#  define VStdLib_ConvertUint8ArrayToSint64LittleEndian(src, dst)  VStdLib_ConvertUint8ArrayToUint64LittleEndian((src), ((uint64*)(dst)))

# endif /* defined (PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON) */

# if (VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_SwapEndianSint16()
 *********************************************************************************************************************/
/*! \brief       Swaps the endianness of the specified signed 16-bit value.
 *  \details     -
 *  \param[in]   value   Signed 16-bit value, whose endianness shall be swapped.
 *  \return      Value in the swapped endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
#  define VStdLib_SwapEndianSint16(value)  ((sint16)VStdLib_SwapEndianUint16(((uint16)(value))))

# endif /* VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON */

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianPlatformToBigUint16()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 16-bit value from the platform endianness to big endian.
 *  \details     -
 *  \param[in]   value   Unsigned 16-bit value according to the platform endianness, which shall be converted to big
 *                       endian.
 *  \return      Value in big endian.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
#  define VStdLib_ConvertEndianPlatformToBigUint16(value)  (value)
# else
#  define VStdLib_ConvertEndianPlatformToBigUint16(value)  VStdLib_SwapEndianUint16((value))
# endif

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianPlatformToBigSint16()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 16-bit value from the platform endianness to big endian.
 *  \details     -
 *  \param[in]   value   Signed 16-bit value according to the platform endianness, which shall be converted to big
 *                       endian.
 *  \return      Value in big endian.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# define VStdLib_ConvertEndianPlatformToBigSint16(value)  ((sint16)VStdLib_ConvertEndianPlatformToBigUint16(((uint16)(value))))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianPlatformToLittleUint16()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 16-bit value from the platform endianness to little endian.
 *  \details     -
 *  \param[in]   value   Unsigned 16-bit value according to the platform endianness, which shall be converted to little
 *                       endian.
 *  \return      Value in little endian.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#  define VStdLib_ConvertEndianPlatformToLittleUint16(value)  (value)
# else
#  define VStdLib_ConvertEndianPlatformToLittleUint16(value)  VStdLib_SwapEndianUint16((value))
# endif

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianPlatformToLittleSint16()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 16-bit value from the platform endianness to little endian.
 *  \details     -
 *  \param[in]   value   Signed 16-bit value according to the platform endianness, which shall be converted to little
 *                       endian.
 *  \return      Value in little endian.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# define VStdLib_ConvertEndianPlatformToLittleSint16(value)  ((sint16)VStdLib_ConvertEndianPlatformToLittleUint16(((uint16)(value))))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianBigToPlatformUint16()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 16-bit value from big endian to the platform endianness.
 *  \details     -
 *  \param[in]   value   Unsigned 16-bit value according to big endian, which shall be converted to the platform
 *                       endianness.
 *  \return      Value in the platform endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# define VStdLib_ConvertEndianBigToPlatformUint16(value)  VStdLib_ConvertEndianPlatformToBigUint16((value))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianBigToPlatformSint16()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 16-bit value from big endian to the platform endianness.
 *  \details     -
 *  \param[in]   value   Signed 16-bit value according to big endian, which shall be converted to the platform.
 *                       endianness.
 *  \return      Value in the platform endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# define VStdLib_ConvertEndianBigToPlatformSint16(value)  VStdLib_ConvertEndianPlatformToBigSint16((value))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianLittleToPlatformUint16()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 16-bit value from little endian to the platform endianness.
 *  \details     -
 *  \param[in]   value   Unsigned 16-bit value according to little endian, which shall be converted to the platform
 *                       endianness.
 *  \return      Value in the platform endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# define VStdLib_ConvertEndianLittleToPlatformUint16(value)  VStdLib_ConvertEndianPlatformToLittleUint16((value))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianLittleToPlatformSint16()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 16-bit value from little endian to the platform endianness.
 *  \details     -
 *  \param[in]   value   Signed 16-bit value according to little endian, which shall be converted to the platform
 *                       endianness.
 *  \return      Value in the platform endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# define VStdLib_ConvertEndianLittleToPlatformSint16(value)  VStdLib_ConvertEndianPlatformToLittleSint16((value))

# if (VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_SwapEndianSint32()
 *********************************************************************************************************************/
/*! \brief       Swaps the endianness of the specified signed 32-bit value.
 *  \details     -
 *  \param[in]   value   Signed 32-bit value, whose endianness shall be swapped.
 *  \return      Value in the swapped endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
#  define VStdLib_SwapEndianSint32(value)  ((sint32)VStdLib_SwapEndianUint32(((uint32)(value))))

# endif /* VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON */

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianPlatformToBigUint32()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 32-bit value from the platform endianness to big endian.
 *  \details     -
 *  \param[in]   value   Unsigned 32-bit value according to the platform endianness, which shall be converted to big
 *                       endian.
 *  \return      Value in big endian.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
#  define VStdLib_ConvertEndianPlatformToBigUint32(value)  (value)
# else
#  define VStdLib_ConvertEndianPlatformToBigUint32(value)  VStdLib_SwapEndianUint32((value))
# endif

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianPlatformToBigSint32()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 32-bit value from the platform endianness to big endian.
 *  \details     -
 *  \param[in]   value   Signed 32-bit value according to the platform endianness, which shall be converted to big
 *                       endian.
 *  \return      Value in big endian.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# define VStdLib_ConvertEndianPlatformToBigSint32(value)  ((sint32)VStdLib_ConvertEndianPlatformToBigUint32(((uint32)(value))))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianPlatformToLittleUint32()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 32-bit value from the platform endianness to little endian.
 *  \details     -
 *  \param[in]   value   Unsigned 32-bit value according to the platform endianness, which shall be converted to little
 *                       endian.
 *  \return      Value in little endian.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#  define VStdLib_ConvertEndianPlatformToLittleUint32(value)  (value)
# else
#  define VStdLib_ConvertEndianPlatformToLittleUint32(value)  VStdLib_SwapEndianUint32((value))
# endif

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianPlatformToLittleSint32()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 32-bit value from the platform endianness to little endian.
 *  \details     -
 *  \param[in]   value   Signed 32-bit value according to the platform endianness, which shall be converted to little
 *                       endian.
 *  \return      Value in little endian.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# define VStdLib_ConvertEndianPlatformToLittleSint32(value)  ((sint32)VStdLib_ConvertEndianPlatformToLittleUint32(((uint32)(value))))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianBigToPlatformUint32()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 32-bit value from big endian to the platform endianness.
 *  \details     -
 *  \param[in]   value   Unsigned 32-bit value according to big endian, which shall be converted to the platform
 *                       endianness.
 *  \return      Value in the platform endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# define VStdLib_ConvertEndianBigToPlatformUint32(value)  VStdLib_ConvertEndianPlatformToBigUint32((value))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianBigToPlatformSint32()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 32-bit value from big endian to the platform endianness.
 *  \details     -
 *  \param[in]   value   Signed 32-bit value according to big endian, which shall be converted to the platform
 *                       endianness.
 *  \return      Value in the platform endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# define VStdLib_ConvertEndianBigToPlatformSint32(value)  VStdLib_ConvertEndianPlatformToBigSint32((value))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianLittleToPlatformUint32()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 32-bit value from little endian to the platform endianness.
 *  \details     -
 *  \param[in]   value   Unsigned 32-bit value according to little endian, which shall be converted to the platform
 *                       endianness.
 *  \return      Value in the platform endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# define VStdLib_ConvertEndianLittleToPlatformUint32(value)  VStdLib_ConvertEndianPlatformToLittleUint32((value))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianLittleToPlatformSint32()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 32-bit value from little endian to the platform endianness.
 *  \details     -
 *  \param[in]   value   Signed 32-bit value according to little endian, which shall be converted to the platform
 *                       endianness.
 *  \return      Value in the platform endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      -
 *  \pre         -
 *********************************************************************************************************************/
# define VStdLib_ConvertEndianLittleToPlatformSint32(value)  VStdLib_ConvertEndianPlatformToLittleSint32((value))

# if defined (PLATFORM_SUPPORT_SINT64_UINT64)
#  if (VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_SwapEndianSint64()
 *********************************************************************************************************************/
/*! \brief       Swaps the endianness of the specified signed 64-bit value.
 *  \details     -
 *  \param[in]   value   Signed 64-bit value, whose endianness shall be swapped.
 *  \return      Value in the swapped endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
#   define VStdLib_SwapEndianSint64(value)  ((sint64)VStdLib_SwapEndianUint64(((uint64)(value))))

#  endif /* VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON */

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianPlatformToBigUint64()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 64-bit value from the platform endianness to big endian.
 *  \details     -
 *  \param[in]   value   Unsigned 64-bit value according to the platform endianness, which shall be converted to big
 *                       endian.
 *  \return      Value in big endian.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \pre         -
 *********************************************************************************************************************/
#  if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
#   define VStdLib_ConvertEndianPlatformToBigUint64(value)  (value)
#  else
#   define VStdLib_ConvertEndianPlatformToBigUint64(value)  VStdLib_SwapEndianUint64((value))
#  endif

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianPlatformToBigSint64()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 64-bit value from the platform endianness to big endian.
 *  \details     -
 *  \param[in]   value   Signed 64-bit value according to the platform endianness, which shall be converted to big
 *                       endian.
 *  \return      Value in big endian.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \pre         -
 *********************************************************************************************************************/
#  define VStdLib_ConvertEndianPlatformToBigSint64(value)  ((sint64)VStdLib_ConvertEndianPlatformToBigUint64(((uint64)(value))))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianPlatformToLittleUint64()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 64-bit value from the platform endianness to little endian.
 *  \details     -
 *  \param[in]   value   Unsigned 64-bit value according to the platform endianness, which shall be converted to little
 *                       endian.
 *  \return      Value in little endian.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \pre         -
 *********************************************************************************************************************/
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#   define VStdLib_ConvertEndianPlatformToLittleUint64(value)  (value)
#  else
#   define VStdLib_ConvertEndianPlatformToLittleUint64(value)  VStdLib_SwapEndianUint64((value))
#  endif

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianPlatformToLittleSint64()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 64-bit value from the platform endianness to little endian.
 *  \details     -
 *  \param[in]   value   Signed 64-bit value according to the platform endianness, which shall be converted to little
 *                       endian.
 *  \return      Value in little endian.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \pre         -
 *********************************************************************************************************************/
#  define VStdLib_ConvertEndianPlatformToLittleSint64(value)  ((sint64)VStdLib_ConvertEndianPlatformToLittleUint64(((uint64)(value))))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianBigToPlatformUint64()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 64-bit value from big endian to the platform endianness.
 *  \details     -
 *  \param[in]   value   Unsigned 64-bit value according to big endian, which shall be converted to the platform
 *                       endianness.
 *  \return      Value in the platform endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \pre         -
 *********************************************************************************************************************/
#  define VStdLib_ConvertEndianBigToPlatformUint64(value)  VStdLib_ConvertEndianPlatformToBigUint64((value))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianBigToPlatformSint64()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 64-bit value from big endian to the platform endianness.
 *  \details     -
 *  \param[in]   value   Signed 64-bit value according to big endian, which shall be converted to the platform
 *                       endianness.
 *  \return      Value in the platform endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \pre         -
 *********************************************************************************************************************/
#  define VStdLib_ConvertEndianBigToPlatformSint64(value)  VStdLib_ConvertEndianPlatformToBigSint64((value))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianLittleToPlatformUint64()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 64-bit value from little endian to the platform endianness.
 *  \details     -
 *  \param[in]   value   Unsigned 64-bit value according to little endian, which shall be converted to the platform
 *                       endianness.
 *  \return      Value in the platform endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \pre         -
 *********************************************************************************************************************/
#  define VStdLib_ConvertEndianLittleToPlatformUint64(value)  VStdLib_ConvertEndianPlatformToLittleUint64((value))

/**********************************************************************************************************************
 *  VStdLib_ConvertEndianLittleToPlatformSint64()
 *********************************************************************************************************************/
/*! \brief       Converts the specified signed 64-bit value from little endian to the platform endianness.
 *  \details     -
 *  \param[in]   value   Signed 64-bit value according to little endian, which shall be converted to the platform
 *                       endianness.
 *  \return      Value in the platform endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \pre         -
 *********************************************************************************************************************/
#  define VStdLib_ConvertEndianLittleToPlatformSint64(value)  VStdLib_ConvertEndianPlatformToLittleSint64((value))

# endif /* defined (PLATFORM_SUPPORT_SINT64_UINT64) */

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianPlatformToBig()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bytes starting at pSrc (platform endianness) to another memory location starting at pDst
 *               in big endian according to the integer type intType.
 *  \details     Depending on the configuration (VSTDLIB_RUNTIME_OPTIMIZATION) and the passed arguments for parameter
 *               intType and nCnt different memory access sizes are used (see TechRef).
 *               intType value VSTDLIB_INTTYPE_64 is only supported if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      The compatible definition VStdLib_MemCpyEndianPlatformToBigLarge() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType') and writable from the calling context.
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
# if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
#  define VStdLib_MemCpyEndianPlatformToBig(pDst, pSrc, nCnt, intType)  VStdLib_MemCpy((pDst), (pSrc), (nCnt))
# else
#  define VStdLib_MemCpyEndianPlatformToBig(pDst, pSrc, nCnt, intType)  VStdLib_MemCpySwapEndian((pDst), (pSrc), (nCnt), (intType))
# endif

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianPlatformToLittle()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bytes starting at pSrc (platform endianness) to another memory location starting at pDst
 *               in little endian according to the integer type intType.
 *  \details     Depending on the configuration (VSTDLIB_RUNTIME_OPTIMIZATION) and the passed arguments for parameter
 *               intType and nCnt different memory access sizes are used (see TechRef).
 *               intType value VSTDLIB_INTTYPE_64 is only supported if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      The compatible definition VStdLib_MemCpyEndianPlatformToLittleLarge() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType') and writable from the calling context.
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
# if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
#  define VStdLib_MemCpyEndianPlatformToLittle(pDst, pSrc, nCnt, intType) \
    VStdLib_MemCpy((pDst), (pSrc), (nCnt))
# else
#  define VStdLib_MemCpyEndianPlatformToLittle(pDst, pSrc, nCnt, intType) \
    VStdLib_MemCpySwapEndian((pDst), (pSrc), (nCnt), (intType))
# endif

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianBigToPlatform()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bytes starting at pSrc (big endian) to another memory location starting at pDst in the
 *               platform endianness according to the integer type intType.
 *  \details     Depending on the configuration (VSTDLIB_RUNTIME_OPTIMIZATION) and the passed arguments for parameter
 *               intType and nCnt different memory access sizes are used (see TechRef).
 *               intType value VSTDLIB_INTTYPE_64 is only supported if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      The compatible definition VStdLib_MemCpyEndianBigToPlatformLarge() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType') and writable from the calling context.
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyEndianBigToPlatform(pDst, pSrc, nCnt, intType) \
    VStdLib_MemCpyEndianPlatformToBig((pDst), (pSrc), (nCnt), (intType))

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianBigToPlatform_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies data from one memory location (big endian) to another in the
 *               platform endianness according to the integer type intType.
 *  \details     If nDstSize is greater than or equal to nCnt, it uses VStdLib_MemCpy()/VStdLib_MemCpySwapEndian() to
 *               copy nCnt bytes starting at pSrc to another memory location starting at pDst in the platform
 *               endianness. Otherwise no copying is performed and the passed errorCode is returned.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize      Maximum number of bytes available in the destination (typically the size of the
 *                             destination object).
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \param[in]   errorCode     Value to be returned, if nCnt exceeds nDstSize.
 *                             Should not be equal to E_OK.
 *  \return      E_OK          Copying was performed.
 *  \return      errorCode     nCnt exceeds nDstSize. No copying was performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nDstSize'
 *  \synchronous TRUE
 *  \config      The compatible definition VStdLib_MemCpyEndianBigToPlatformLarge_s() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nDstSize' have to define a valid memory area that is writable from the
 *               calling context.
 *               The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType').
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyEndianBigToPlatform_s(pDst, nDstSize, pSrc, nCnt, intType, errorCode) \
    VStdLib_MemCpyEndianPlatformToBig_s((pDst), (nDstSize), (pSrc), (nCnt), (intType), (errorCode))

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianLittleToPlatform()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bytes starting at pSrc (little endian) to another memory location starting at pDst in the
 *               platform endianness according to the integer type intType.
 *  \details     Depending on the configuration (VSTDLIB_RUNTIME_OPTIMIZATION) and the passed arguments for parameter
 *               intType and nCnt different memory access sizes are used (see TechRef).
 *               intType value VSTDLIB_INTTYPE_64 is only supported if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      The compatible definition VStdLib_MemCpyEndianLittleToPlatformLarge() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType') and writable from the calling context.
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyEndianLittleToPlatform(pDst, pSrc, nCnt, intType) \
    VStdLib_MemCpyEndianPlatformToLittle((pDst), (pSrc), (nCnt), (intType))

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianLittleToPlatform_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies data from one memory location (little endian) to another in
 *               the platform endianness according to the integer type intType.
 *  \details     If nDstSize is greater than or equal to nCnt, it uses VStdLib_MemCpy()/VStdLib_MemCpySwapEndian() to
 *               copy nCnt bytes starting at pSrc to another memory location starting at pDst in the platform
 *               endianness. Otherwise no copying is performed and the passed errorCode is returned.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize      Maximum number of bytes available in the destination (typically the size of the
 *                             destination object).
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \param[in]   errorCode     Value to be returned, if nCnt exceeds nDstSize.
 *                             Should not be equal to E_OK.
 *  \return      E_OK          Copying was performed.
 *  \return      errorCode     nCnt exceeds nDstSize. No copying was performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nDstSize'
 *  \synchronous TRUE
 *  \config      The compatible definition VStdLib_MemCpyEndianLittleToPlatformLarge_s() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nDstSize' have to define a valid memory area that is writable from the
 *               calling context.
 *               The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType').
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyEndianLittleToPlatform_s(pDst, nDstSize, pSrc, nCnt, intType, errorCode) \
    VStdLib_MemCpyEndianPlatformToLittle_s((pDst), (nDstSize), (pSrc), (nCnt), (intType), (errorCode))

/* PRQA L:VSTDLIB_FUNCTION_LIKE_MACRO */

# if !defined (VStdLib_MemClrMacro)

/**********************************************************************************************************************
 *  VStdLib_MemClrMacro()
 *********************************************************************************************************************/
/*! \brief       Initializes memory to zero (macro implementation).
 *  \details     Sets nCnt blocks starting at pDst to zero (block-size is given by the type of pDst).
 *  \param[out]  pDst          Any typed pointer to the memory location to be initialized, must be aligned
 *                             corresponding to its type and not be NULL.
 *  \param[in]   nCnt          Number of blocks to initialize, pDst must be valid for this amount.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas  defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area that is writable from the calling
 *               context.
 *********************************************************************************************************************/
#  define VStdLib_MemClrMacro(pDst, nCnt) \
    { VStdLib_CntType __i; for ( __i = 0u; __i < (nCnt); ++__i) { (pDst)[__i] = 0; } } /* FETA_VSTDLIB_MEMSET_MACRO */

# endif

# if !defined (VStdLib_MemSetMacro)

/**********************************************************************************************************************
 *  VStdLib_MemSetMacro()
 *********************************************************************************************************************/
/*! \brief       Initializes memory to a specified pattern (macro implementation).
 *  \details     Sets nCnt blocks starting at pDst to nPattern (block-size is given by the type of pDst).
 *  \param[out]  pDst          Any typed pointer to the memory location to be initialized, must be aligned
 *                             corresponding to its type and not be NULL.
 *  \param[in]   nPattern      The pattern to be used to initialize the memory (consider the correlation between its
 *                             type and the type of pDst).
 *  \param[in]   nCnt          Number of blocks to initialize, pDst must be valid for this amount.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area that is writable from the calling
 *               context.
 *********************************************************************************************************************/
#  define VStdLib_MemSetMacro(pDst, nPattern, nCnt) \
    { VStdLib_CntType __i; for ( __i = 0u; __i < (nCnt); ++__i) { (pDst)[__i] = (nPattern); } } /* FETA_VSTDLIB_MEMSET_MACRO */

# endif

# if !defined (VStdLib_MemCpyMacro)

/**********************************************************************************************************************
 *  VStdLib_MemCpyMacro()
 *********************************************************************************************************************/
/*! \brief       Copies data from one memory location to another (macro implementation).
 *  \details     Copies nCnt blocks starting at pSrc to another memory location starting at pDst (block-size is given
 *               by the type of pDst).
 *  \param[out]  pDst          Any typed pointer to the memory location to copy to, must be aligned corresponding
 *                             to its type and not be NULL.
 *  \param[in]   pSrc          Any typed pointer to the memory location to copy from, must be aligned corresponding
 *                             to its type and not be NULL.
 *  \param[in]   nCnt          Number of blocks to copy, pDst must be valid for this amount.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area that is writable from the calling
 *               context.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyMacro(pDst, pSrc, nCnt) \
    { VStdLib_CntType __i; for ( __i = 0u; __i < (nCnt); ++__i) { (pDst)[__i] = (pSrc)[__i]; } } /* FETA_VSTDLIB_MEMCPY_MACRO */

# endif

# if !defined (VStdLib_MemCpyMacro_s)

/**********************************************************************************************************************
 *  VStdLib_MemCpyMacro_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies data from one memory location to another (macro
 *               implementation).
 *  \details     Uses VStdLib_MemCpyMacro() to copy nCnt blocks starting at pSrc to another memory location starting
 *               at pDst (block-size is given by the type of pDst), if nDstSize is greater than or equal to nCnt.
 *  \param[out]  pDst          Any typed pointer to the memory location to copy to, must be aligned corresponding
 *                             to its type and not be NULL.
 *  \param[in]   nDstSize      Maximum number of blocks available in the destination  (typically the size of the
 *                             destination object).
 *  \param[in]   pSrc          Any typed pointer to the memory location to copy from, must be aligned corresponding
 *                             to its type and not be NULL.
 *  \param[in]   nCnt          Number of blocks to copy.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nDstSize'
 *  \synchronous TRUE
 *  \pre         The parameters 'pDst' and 'nDstSize' have to define a valid memory area that is writable from the
 *               calling context.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
#  define VStdLib_MemCpyMacro_s(pDst, nDstSize, pSrc, nCnt) \
    { if ((nDstSize) >= (nCnt)) { VStdLib_MemCpyMacro((pDst), (pSrc), (nCnt)); } } /* FETA_VSTDLIB_CALL_MEMCPY_MACRO */

# endif

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES (FUNCTION API)
 *********************************************************************************************************************/

# define VSTDLIB_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if (VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_MemSet()
 *********************************************************************************************************************/
/*! \brief       Initializes memory to a specified pattern.
 *  \details     Sets nCnt bytes starting at pDst to the character nPattern.
 *  \param[out]  pDst          Pointer to the memory location to be initialized, must not be NULL.
 *  \param[in]   nPattern      The character to be used to initialize the memory.
 *  \param[in]   nCnt          Number of bytes to initialize, pDst must be valid for this amount.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_LIBRARY_FUNCTIONS == STD_ON.
 *               The compatible definition VStdLib_MemSetLarge() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area that is writable from the calling
 *               context.
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_MemSet(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
                                        uint8 nPattern,
                                        VStdLib_CntType nCnt);

/**********************************************************************************************************************
 *  VStdLib_MemCpy()
 *********************************************************************************************************************/
/*! \brief       Copies data from one memory location to another.
 *  \details     Copies nCnt bytes starting at pSrc to another memory location starting at pDst.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_LIBRARY_FUNCTIONS == STD_ON.
 *               The compatible definition VStdLib_MemCpyLarge() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area that is writable from the calling
 *               context.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_MemCpy(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
                                        P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
                                        VStdLib_CntType nCnt);

/**********************************************************************************************************************
 *  VStdLib_MemCpy16()
 *********************************************************************************************************************/
/*! \brief       Copies data from one memory location to another.
 *  \details     Copies nCnt 16-bit blocks starting at pSrc to another memory location starting at pDst.
 *  \param[out]  pDst          Pointer to the memory location to copy to, 16-bit aligned and not NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, 16-bit aligned and not NULL.
 *  \param[in]   nCnt          Number of 16-bit blocks to copy, pDst must be valid for this amount.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_LIBRARY_FUNCTIONS == STD_ON.
 *               The compatible definition VStdLib_MemCpy16Large() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area that is writable from the calling
 *               context.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_MemCpy16(P2VAR(uint16, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
                                          P2CONST(uint16, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
                                          VStdLib_CntType nCnt);

/**********************************************************************************************************************
 *  VStdLib_MemCpy32()
 *********************************************************************************************************************/
/*! \brief       Copies data from one memory location to another.
 *  \details     Copies nCnt 32-bit blocks starting at pSrc to another memory location starting at pDst.
 *  \param[out]  pDst          Pointer to the memory location to copy to, 32-bit aligned and not NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, 32-bit aligned and not NULL.
 *  \param[in]   nCnt          Number of 32-bit blocks to copy, pDst must be valid for this amount.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_LIBRARY_FUNCTIONS == STD_ON.
 *               The compatible definition VStdLib_MemCpy32Large() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a valid memory area that is writable from the calling
 *               context.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_MemCpy32(P2VAR(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
                                          P2CONST(uint32, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
                                          VStdLib_CntType nCnt);

# endif /* VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON */

/**********************************************************************************************************************
 *  VStdLib_MemCpy_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies data from one memory location to another.
 *  \details     Uses VStdLib_MemCpy() to copy nCnt bytes starting at pSrc to another memory location starting
 *               at pDst, if nDstSize is greater than or equal to nCnt.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize      Maximum number of bytes available in the destination (typically the size of the
 *                             destination object).
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nDstSize'
 *  \synchronous TRUE
 *  \config      The compatible definition VStdLib_MemCpyLarge_s() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nDstSize' have to define a valid memory area that is writable from the
 *               calling context.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_MemCpy_s(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
                                          VStdLib_CntType nDstSize,
                                          P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
                                          VStdLib_CntType nCnt);

/**********************************************************************************************************************
 *  VStdLib_MemCpySwapEndian_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies data from one memory location to another in the swapped
 *               endianness according to the integer type intType.
 *  \details     If nDstSize is greater than or equal to nCnt, it uses VStdLib_MemCpySwapEndian() to copy and swap nCnt
 *               bytes starting at pSrc to another memory location starting at pDst.
 *               Otherwise no copying is performed and the passed errorCode is returned.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize      Maximum number of bytes available in the destination (typically the size of the
 *                             destination object).
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \param[in]   errorCode     Value to be returned, if nCnt exceeds nDstSize.
 *                             Should not be equal to E_OK.
 *  \return      E_OK          Copying was performed.
 *  \return      errorCode     nCnt exceeds nDstSize. No copying was performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nDstSize'
 *  \synchronous TRUE
 *  \config      The compatible definition VStdLib_MemCpySwapEndianLarge_s() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nDstSize' have to define a valid memory area that is writable from the
 *               calling context.
 *               The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType').
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_MemCpySwapEndian_s(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
                                                              VStdLib_CntType nDstSize,
                                                              P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
                                                              VStdLib_CntType nCnt,
                                                              uint8 intType,
                                                              Std_ReturnType errorCode);

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianPlatformToBig_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies data from one memory location (platform endianness) to
 *               another in big endian according to the integer type intType.
 *  \details     If nDstSize is greater than or equal to nCnt, it uses VStdLib_MemCpy()/VStdLib_MemCpySwapEndian() to
 *               copy nCnt bytes starting at pSrc to another memory location starting at pDst in big endian.
 *               Otherwise no copying is performed and the passed errorCode is returned.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize      Maximum number of bytes available in the destination (typically the size of the
 *                             destination object).
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \param[in]   errorCode     Value to be returned, if nCnt exceeds nDstSize.
 *                             Should not be equal to E_OK.
 *  \return      E_OK          Copying was performed.
 *  \return      errorCode     nCnt exceeds nDstSize. No copying was performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nDstSize'
 *  \synchronous TRUE
 *  \config      The compatible definition VStdLib_MemCpyEndianPlatformToBigLarge_s() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nDstSize' have to define a valid memory area that is writable from the
 *               calling context.
 *               The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType').
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_MemCpyEndianPlatformToBig_s(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nCnt,
  uint8 intType,
  Std_ReturnType errorCode);

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianPlatformToLittle_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies data from one memory location (platform endianness) to
 *               another in little endian according to the integer type intType.
 *  \details     If nDstSize is greater than or equal to nCnt, it uses VStdLib_MemCpy()/VStdLib_MemCpySwapEndian() to
 *               copy nCnt bytes starting at pSrc to another memory location starting at pDst in little endian.
 *               Otherwise no copying is performed and the passed errorCode is returned.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize      Maximum number of bytes available in the destination (typically the size of the
 *                             destination object).
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \param[in]   errorCode     Value to be returned, if nCnt exceeds nDstSize.
 *                             Should not be equal to E_OK.
 *  \return      E_OK          Copying was performed.
 *  \return      errorCode     nCnt exceeds nDstSize. No copying was performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nDstSize'
 *  \synchronous TRUE
 *  \config      The compatible definition VStdLib_MemCpyEndianPlatformToLittleLarge_s() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nDstSize' have to define a valid memory area that is writable from the
 *               calling context.
 *               The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType').
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_MemCpyEndianPlatformToLittle_s(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nCnt,
  uint8 intType,
  Std_ReturnType errorCode);

# if (VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_MemCmp()
 *********************************************************************************************************************/
/*! \brief       Compares two buffers for the specified number of bytes and returns whether they are equal or not.
 *  \details     -
 *  \param[in]   pBuf1     Pointer to first buffer for comparison, must not be NULL.
 *  \param[in]   pBuf2     Pointer to second buffer for comparison, must not be NULL.
 *  \param[in]   nCnt      Number of bytes to compare, pBuf1 and pBuf2 must be valid for this amount.
 *  \return      < 0       The first buffer contains a smaller value than the second buffer.
 *  \return      0         The two buffers are equal for the number of compared bytes.
 *  \return      > 0       The first buffer contains a greater value than the second buffer.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_LIBRARY_FUNCTIONS == STD_ON.
 *               The compatible definition VStdLib_MemCmpLarge() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pBuf1' and 'pBuf2' have to point to valid memory areas. Each memory area has to be
 *               valid for at least the byte count defined in parameter 'nCnt'.
 *               The memory areas defined by parameters 'pBuf1', 'pBuf2' and 'nCnt' must not be changed by another
 *               context while the comparison is being executed.
 *********************************************************************************************************************/
FUNC(sint8, VSTDLIB_CODE) VStdLib_MemCmp(P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf1,
                                         P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf2,
                                         VStdLib_CntType nCnt);

# endif /* VSTDLIB_USE_LIBRARY_FUNCTIONS != STD_ON */

# if (VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_MemCpySwapEndian()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bytes starting at pSrc to another memory location starting at pDst in the swapped
 *               endianness according to the integer type intType.
 *  \details     Depending on the configuration (VSTDLIB_RUNTIME_OPTIMIZATION) and the passed arguments for parameter
 *               intType and nCnt different memory access sizes are used (see TechRef).
 *               intType value VSTDLIB_INTTYPE_64 is only supported if PLATFORM_SUPPORT_SINT64_UINT64 is defined.
 *  \param[out]  pDst          Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   pSrc          Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nCnt          Number of bytes to copy, pDst must be valid for this amount.
 *  \param[in]   intType       Integer type with which the memory locations are interpreted
 *                             (VSTDLIB_INTTYPE_[8, 16, 32, 64]).
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst' and 'nCnt'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION == STD_ON.
 *               The compatible definition VStdLib_MemCpySwapEndianLarge() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pDst' and 'nCnt' have to define a memory area that is valid for a multiple of the
 *               integer type size (parameter 'intType') and writable from the calling context.
 *               The parameter 'pDst' is aligned according to the integer type 'intType'.
 *               The source memory area defined by parameters 'pSrc' and 'nCnt' must not be changed by another context
 *               while the copying is being executed.
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_MemCpySwapEndian(P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
                                                  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
                                                  VStdLib_CntType nCnt,
                                                  uint8 intType);

# endif /* VSTDLIB_USE_MEM_CPY_ENDIAN_CONVERSION_LIBRARY_FUNCTION != STD_ON */

# if(VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_BitCpySawBigToBigEndian()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bits of big endian data from one memory location to another memory location with big
 *               endian data.
 *  \details     -
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetSaw'.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffetSaw' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameters 'pDst', 'nDstBitOffsetSaw' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetSaw' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpySawBigToBigEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt);

/**********************************************************************************************************************
 *  VStdLib_BitCpySawBigToLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bits of big endian data from one memory location to another memory location with little
 *               endian data.
 *  \details     -
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetSaw'.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffetSaw' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameters 'pDst', 'nDstBitOffsetSaw' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetSaw' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpySawBigToLittleEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt);

/**********************************************************************************************************************
 *  VStdLib_BitCpySawLittleToBigEndian()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bits of little endian data from one memory location to another memory location with big
 *               endian data.
 *  \details     -
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetSaw'.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffetSaw' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameters 'pDst', 'nDstBitOffsetSaw' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetSaw' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpySawLittleToBigEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt);

/**********************************************************************************************************************
 *  VStdLib_BitCpySawLittleToLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bits of little endian data from one memory location to another memory location with
 *               little endian data.
 *  \details     -
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetSaw'.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffetSaw' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameters 'pDst', 'nDstBitOffsetSaw' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetSaw' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpySawLittleToLittleEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt);

# endif /* VSTDLIB_USE_BITCPYSAWTOOTH_LIBRARY_FUNCTIONS != STD_ON */

/**********************************************************************************************************************
 *  VStdLib_BitCpySawBigToBigEndian_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies nCnt bits of big endian data from one memory location to
 *               another memory location with big endian data.
 *  \details     Uses VStdLib_BitCpySawBigToBigEndian() to copy data, if nDstSize is greater than or equal to the last
 *               byte to copy to.
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize             Maximum number of bytes available in the destination (typically the size of the
 *                                    destination object).
 *  \param[in]   nDstBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to 
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetSaw'.
 *  \param[in]   errorCode            Value to be returned, if 'nCnt' bits starting at 'nDstBitOffsetSaw' exceeds
 *                                    'nDstSize'. Should not be equal to E_OK.
 *  \return      E_OK                 Copying was performed.
 *  \return      errorCode            'nCnt' bits starting at 'nDstBitOffsetSaw'' exceeds 'nDstSize'. No copying was 
 *                                    performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffetSaw' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      -
 *  \pre         The parameters 'pDst', 'nDstBitOffsetSaw' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetSaw' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpySawBigToBigEndian_s(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode);

/**********************************************************************************************************************
 *  VStdLib_BitCpySawBigToLittleEndian_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies nCnt bits of big endian data from one memory location to
 *               another memory location with little endian data.
 *  \details     Uses VStdLib_BitCpySawBigToLittleEndian() to copy data, if nDstSize is greater than or equal to the
 *               last byte to copy to.
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize             Maximum number of bytes available in the destination (typically the size of the
 *                                    destination object).
 *  \param[in]   nDstBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to 
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetSaw'.
 *  \param[in]   errorCode            Value to be returned, if 'nCnt' bits starting at 'nDstBitOffsetSaw' exceeds
 *                                    'nDstSize'. Should not be equal to E_OK.
 *  \return      E_OK                 Copying was performed.
 *  \return      errorCode            'nCnt' bits starting at 'nDstBitOffsetSaw' exceeds 'nDstSize'. No copying was 
 *                                    performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffetSaw' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      -
 *  \pre         The parameters 'pDst', 'nDstBitOffsetSaw' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetSaw' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpySawBigToLittleEndian_s(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode);

/**********************************************************************************************************************
 *  VStdLib_BitCpySawLittleToBigEndian_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies nCnt bits of little endian data from one memory location to
 *               another memory location with big endian data. 
 *  \details     Uses VStdLib_BitCpySawLittleToBigEndian() to copy data, if nDstSize is greater than or equal to the
 *               last byte to copy to.
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize             Maximum number of bytes available in the destination (typically the size of the
 *                                    destination object).
 *  \param[in]   nDstBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetSaw'.
 *  \param[in]   errorCode            Value to be returned, if 'nCnt' bits starting at 'nDstBitOffsetSaw' exceeds
 *                                    'nDstSize'. Should not be equal to E_OK.
 *  \return      E_OK                 Copying was performed.
 *  \return      errorCode            'nCnt' bits starting at 'nDstBitOffsetSaw' exceeds 'nDstSize'. No copying was 
 *                                    performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffetSaw' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      -
 *  \pre         The parameters 'pDst', 'nDstBitOffsetSaw' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetSaw' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpySawLittleToBigEndian_s(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode);

/**********************************************************************************************************************
 *  VStdLib_BitCpySawLittleToLittleEndian_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies nCnt bits of little endian data from one memory location to
 *               another memory location with little endian data.
 *  \details     Uses VStdLib_BitCpySawLittleToLittleEndian() to copy data, if nDstSize is greater than or equal to
 *               the last byte to copy to.
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize             Maximum number of bytes available in the destination (typically the size of the
 *                                    destination object).
 *  \param[in]   nDstBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to 
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetSaw     Bit offset (it can even be greater than 7) in sawtooth numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetSaw'.
 *  \param[in]   errorCode            Value to be returned, if 'nCnt' bits starting at 'nDstBitOffsetSaw' exceeds
 *                                    'nDstSize'. Should not be equal to E_OK.
 *  \return      E_OK                 Copying was performed.
 *  \return      errorCode            'nCnt' bits starting at 'nDstBitOffsetSaw' exceeds 'nDstSize'. No copying was
 *                                    performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffetSaw' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      -
 *  \pre         The parameters 'pDst', 'nDstBitOffsetSaw' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetSaw' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpySawLittleToLittleEndian_s(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetSaw,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetSaw,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode);

# if(VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_BitCpyMonBigToBigEndian()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bits of big endian data from one memory location to another memory location with big
 *               endian data.
 *  \details     -
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetMon'.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetMon' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpyMonBigToBigEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt);

/**********************************************************************************************************************
 *  VStdLib_BitCpyMonBigToLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bits of big endian data from one memory location to another memory location with little
 *               endian data.
 *  \details     -
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetMon    Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetMon'.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetMon' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpyMonBigToLittleEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt);

/**********************************************************************************************************************
 *  VStdLib_BitCpyMonLittleToBigEndian()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bits of little endian data from one memory location to another memory location with big
 *               endian data.
 *  \details     -
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetMon'.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetMon' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpyMonLittleToBigEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt);

/**********************************************************************************************************************
 *  VStdLib_BitCpyMonLittleToLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Copies nCnt bits of little endian data from one memory location to another memory location with
 *               little endian data.
 *  \details     -
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetMon'.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetMon' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_BitCpyMonLittleToLittleEndian(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt);

# endif /* VSTDLIB_USE_BITCPYMONOTONE_LIBRARY_FUNCTIONS != STD_ON */

/**********************************************************************************************************************
 *  VStdLib_BitCpyMonBigToBigEndian_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies nCnt bits of big endian data from one memory location to
 *               another memory location with big endian data.
 *  \details     Uses VStdLib_BitCpyMonBigToBigEndian() to copy data, if nDstSize is greater than or equal to the last
 *               byte to copy to.
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize             Maximum number of bytes available in the destination (typically the size of the
 *                                    destination object).
 *  \param[in]   nDstBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetMon'.
 *  \param[in]   errorCode            Value to be returned, if 'nCnt' bits starting at 'nDstBitOffsetMon' exceeds
 *                                    'nDstSize'. Should not be equal to E_OK.
 *  \return      E_OK                 Copying was performed.
 *  \return      errorCode            'nCnt' bits starting at 'nDstBitOffsetMon' exceeds 'nDstSize'. No copying was
 *                                    performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      -
 *  \pre         The parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetMon' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpyMonBigToBigEndian_s(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode);

/**********************************************************************************************************************
 *  VStdLib_BitCpyMonBigToLittleEndian_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies nCnt bits of big endian data from one memory location to
 *               another memory location with little endian data.
 *  \details     Uses VStdLib_BitCpyMonBigToLittleEndian() to copy data, if nDstSize is greater than or equal to the
 *               last byte to copy to.
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize             Maximum number of bytes available in the destination (typically the size of the
 *                                    destination object).
 *  \param[in]   nDstBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetMon'.
 *  \param[in]   errorCode            Value to be returned, if 'nCnt' bits starting at 'nDstBitOffsetMon' exceeds
 *                                    'nDstSize'. Should not be equal to E_OK.
 *  \return      E_OK                 Copying was performed.
 *  \return      errorCode            'nCnt' bits starting at 'nDstBitOffsetMon' exceeds 'nDstSize'. No copying was
 *                                    performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      -
 *  \pre         The parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetMon' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpyMonBigToLittleEndian_s(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode);

/**********************************************************************************************************************
 *  VStdLib_BitCpyMonLittleToBigEndian_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies nCnt bits of little endian data from one memory location to
 *               another memory location with big endian data
 *  \details     Uses VStdLib_BitCpyMonLittleToBigEndian() to copy data, if nDstSize is greater than or equal to the
 *               last byte to copy to.
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize             Maximum number of bytes available in the destination (typically the size of the
 *                                    destination object).
 *  \param[in]   nDstBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetMon'.
 *  \param[in]   errorCode            Value to be returned, if 'nCnt' bits starting at 'nDstBitOffsetMon' exceeds
 *                                    'nDstSize'. Should not be equal to E_OK.
 *  \return      E_OK                 Copying was performed.
 *  \return      errorCode            'nCnt' bits starting at 'nDstBitOffsetMon' exceeds 'nDstSize'. No copying was
 *                                    performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      -
 *  \pre         The parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetMon' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpyMonLittleToBigEndian_s(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode);

/**********************************************************************************************************************
 *  VStdLib_BitCpyMonLittleToLittleEndian_s()
 *********************************************************************************************************************/
/*! \brief       Verifies the destination size and copies nCnt bits of little endian data from one memory location to
 *               another memory location with little endian data.
 *  \details     Uses VStdLib_BitCpyMonLittleToLittleEndian() to copy data, if nDstSize is greater than or equal to
 *               the last byte to copy to.
 *  \param[out]  pDst                 Pointer to the memory location to copy to, must not be NULL.
 *  \param[in]   nDstSize             Maximum number of bytes available in the destination (typically the size of the
 *                                    destination object).
 *  \param[in]   nDstBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pDst'. Bits are copied to exactly this bit position.
 *  \param[in]   pSrc                 Pointer to the memory location to copy from, must not be NULL.
 *  \param[in]   nSrcBitOffsetMon     Bit offset (it can even be greater than 7) in monotone numbering is added to
 *                                    'pSrc'. Bits are copied from exactly this bit position.
 *  \param[in]   nCnt                 Number of bits to copy starting at 'nDstBitOffsetMon'.
 *  \param[in]   errorCode            Value to be returned, if 'nCnt' bits starting at 'nDstBitOffsetMon' exceeds
 *                                    'nDstSize'. Should not be equal to E_OK.
 *  \return      E_OK                 Copying was performed.
 *  \return      errorCode            'nCnt' bits starting at 'nDstBitOffsetMon' exceeds 'nDstSize'. No copying was
 *                                    performed.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt'.
 *  \synchronous TRUE
 *  \config      -
 *  \pre         The parameters 'pDst', 'nDstBitOffsetMon' and 'nCnt' have to define a valid memory area that is
 *               writable from the calling context.
 *               The source memory area defined by parameters 'pSrc', 'nSrcBitOffsetMon' and 'nCnt' must not
 *               be changed by another context while the copying is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_BitCpyMonLittleToLittleEndian_s(
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  VStdLib_CntType nDstBitOffsetMon,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nSrcBitOffsetMon,
  VStdLib_CntType nCnt,
  Std_ReturnType errorCode);

# if (VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_MemCheck()
 *********************************************************************************************************************/
/*! \brief       Compares the content of a memory area to a specified pattern.
 *  \details     Compares nCnt bytes of memory starting at pBuf to the character nPattern.
 *  \param[in]   pBuf          Pointer to the memory location to be checked, must not be NULL.
 *  \param[in]   nPattern      The character to be checked for in the memory.
 *  \param[in]   nCnt          Number of bytes to be checked, pBuf must be valid for this amount.
 *  \param[in]   errorCode     Value to be returned, if content of pBuf is not equal to pattern.
 *                             Should not be equal to E_OK.
 *  \return      E_OK          Content of pBuf is equal to pattern.
 *  \return      errorCode     Content of pBuf is not equal to pattern.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_MEMCHECK_FUNCTION == STD_ON.
 *               The compatible definition VStdLib_MemCheckLarge() exists additionally (and solely)
 *               if VSTDLIB_SUPPORT_LARGE_DATA == STD_ON.
 *  \pre         The parameters 'pBuf' and 'nCnt' have to define a valid memory area.
 *               The memory area defined by parameters 'pBuf' and 'nCnt' must not be changed by another context while
 *               the comparison is being executed.
 *               The parameter 'errorCode' must not be E_OK.
 *********************************************************************************************************************/
FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_MemCheck(P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pBuf, 
                                                    uint8 nPattern, 
                                                    VStdLib_CntType nCnt,
                                                    Std_ReturnType errorCode);

# endif /* VSTDLIB_USE_MEM_CHECK_LIBRARY_FUNCTION != STD_ON */

# if (VSTDLIB_VERSION_INFO_API == STD_ON)

/**********************************************************************************************************************
 *  VStdLib_GetVersionInfo()
 *********************************************************************************************************************/
/*! \brief       Returns the version information of this module.
 *  \details     Returns version information, vendor ID and AUTOSAR module ID of the component.
 *  \param[out]  versioninfo   Pointer to where to store the version information, must not be NULL.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      VSTDLIB_VERSION_INFO_API == STD_ON
 *  \pre         The parameter 'versioninfo' has to be valid and reference an object of type Std_VersionInfoType.
 *  \trace       CREQ-VStdLib-VersionInformation
 *********************************************************************************************************************/
FUNC(void, VSTDLIB_CODE) VStdLib_GetVersionInfo(P2VAR(Std_VersionInfoType, AUTOMATIC, VSTDLIB_APPL_VAR) versioninfo);

# endif /* VSTDLIB_VERSION_INFO_API == STD_ON */

# if (VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) 

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosOne8()
 *********************************************************************************************************************/
/*! \brief       Returns the highest bit position of a set bit in the specified 8-bit value.
 *  \details     -
 *  \param[in]   value              The 8-bit value in which the highest set bit shall be found.
 *  \return      0-7                Bit position of the highest set bit.
 *  \return      VSTDLIB_BITPOS_NA  8-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosOne8(uint8 value);

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosZero8()
 *********************************************************************************************************************/
/*! \brief       Returns the highest bit position of an unset bit in the specified 8-bit value.
 *  \details     -
 *  \param[in]   value              The 8-bit value in which the highest unset bit shall be found.
 *  \return      0-7                Bit position of the highest unset bit.
 *  \return      VSTDLIB_BITPOS_NA  8-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosZero8(uint8 value);

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosOne8()
 *********************************************************************************************************************/
/*! \brief       Returns the lowest bit position of a set bit in the specified 8-bit value.
 *  \details     -
 *  \param[in]   value              The 8-bit value in which the lowest set bit shall be found.
 *  \return      0-7                Bit position of the lowest set bit.
 *  \return      VSTDLIB_BITPOS_NA  8-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosOne8(uint8 value);

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosZero8()
 *********************************************************************************************************************/
/*! \brief       Returns the lowest bit position of an unset bit in the specified 8-bit value.
 *  \details     -
 *  \param[in]   value              The 8-bit value in which the lowest unset bit shall be found.
 *  \return      0-7                Bit position of the lowest unset bit.
 *  \return      VSTDLIB_BITPOS_NA  8-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosZero8(uint8 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingOnes8()
 *********************************************************************************************************************/
/*! \brief       Returns the count of set bits preceding the highest unset bit in the specified 8-bit value.
 *  \details     -
 *  \param[in]   value              The 8-bit value in which the count of leading ones shall be determined.
 *  \return      0-7                Count of leading ones.
 *  \return      VSTDLIB_BITCNT_NA  8-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingOnes8(uint8 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingZeros8()
 *********************************************************************************************************************/
/*! \brief       Returns the count of unset bits preceding the highest set bit in the specified 8-bit value.
 *  \details     -
 *  \param[in]   value              The 8-bit value in which the count of leading zeros shall be determined.
 *  \return      0-7                Count of leading zeros.
 *  \return      VSTDLIB_BITCNT_NA  8-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingZeros8(uint8 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingOnes8()
 *********************************************************************************************************************/
/*! \brief       Returns the count of set bits following the lowest unset bit in the specified 8-bit value.
 *  \details     -
 *  \param[in]   value              The 8-bit value in which the count of trailing ones shall be determined.
 *  \return      0-7                Count of trailing ones.
 *  \return      VSTDLIB_BITCNT_NA  8-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingOnes8(uint8 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingZeros8()
 *********************************************************************************************************************/
/*! \brief       Returns the count of unset bits following the lowest set bit in the specified 8-bit value.
 *  \details     -
 *  \param[in]   value              The 8-bit value in which the count of trailing zeros shall be determined.
 *  \return      0-7                Count of trailing zeros.
 *  \return      VSTDLIB_BITCNT_NA  8-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingZeros8(uint8 value);

# endif /* VSTDLIB_USE_8BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON */

# if (VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosOne16()
 *********************************************************************************************************************/
/*! \brief       Returns the highest bit position of a set bit in the specified 16-bit value.
 *  \details     -
 *  \param[in]   value              The 16-bit value in which the highest set bit shall be found.
 *  \return      0-15               Bit position of the highest set bit.
 *  \return      VSTDLIB_BITPOS_NA  16-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosOne16(uint16 value);

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosZero16()
 *********************************************************************************************************************/
/*! \brief       Returns the highest bit position of an unset bit in the specified 16-bit value.
 *  \details     -
 *  \param[in]   value              The 16-bit value in which the highest unset bit shall be found.
 *  \return      0-15               Bit position of the highest unset bit.
 *  \return      VSTDLIB_BITPOS_NA  16-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosZero16(uint16 value);

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosOne16()
 *********************************************************************************************************************/
/*! \brief       Returns the lowest bit position of a set bit in the specified 16-bit value.
 *  \details     -
 *  \param[in]   value              The 16-bit value in which the lowest set bit shall be found.
 *  \return      0-15               Bit position of the lowest set bit.
 *  \return      VSTDLIB_BITPOS_NA  16-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosOne16(uint16 value);

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosZero16()
 *********************************************************************************************************************/
/*! \brief       Returns the lowest bit position of an unset bit in the specified 16-bit value.
 *  \details     -
 *  \param[in]   value              The 16-bit value in which the lowest unset bit shall be found.
 *  \return      0-15               Bit position of the lowest unset bit.
 *  \return      VSTDLIB_BITPOS_NA  16-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosZero16(uint16 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingOnes16()
 *********************************************************************************************************************/
/*! \brief       Returns the count of set bits preceding the highest unset bit in the specified 16-bit value.
 *  \details     -
 *  \param[in]   value              The 16-bit value in which the count of leading ones shall be determined.
 *  \return      0-15               Count of leading ones.
 *  \return      VSTDLIB_BITCNT_NA  16-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingOnes16(uint16 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingZeros16()
 *********************************************************************************************************************/
/*! \brief       Returns the count of unset bits preceding the highest set bit in the specified 16-bit value.
 *  \details     -
 *  \param[in]   value              The 16-bit value in which the count of leading zeros shall be determined.
 *  \return      0-15               Count of leading zeros.
 *  \return      VSTDLIB_BITCNT_NA  16-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingZeros16(uint16 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingOnes16()
 *********************************************************************************************************************/
/*! \brief       Returns the count of set bits following the lowest unset bit in the specified 16-bit value.
 *  \details     -
 *  \param[in]   value              The 16-bit value in which the count of trailing ones shall be determined.
 *  \return      0-15               Count of trailing ones.
 *  \return      VSTDLIB_BITCNT_NA  16-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingOnes16(uint16 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingZeros16()
 *********************************************************************************************************************/
/*! \brief       Returns the count of unset bits following the lowest set bit in the specified 16-bit value.
 *  \details     -
 *  \param[in]   value              The 16-bit value in which the count of trailing zeros shall be determined.
 *  \return      0-15               Count of trailing zeros.
 *  \return      VSTDLIB_BITCNT_NA  16-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingZeros16(uint16 value);

# endif /* VSTDLIB_USE_16BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON */

# if (VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosOne32()
 *********************************************************************************************************************/
/*! \brief       Returns the highest bit position of a set bit in the specified 32-bit value.
 *  \details     -
 *  \param[in]   value              The 32-bit value in which the highest set bit shall be found.
 *  \return      0-31               Bit position of the highest set bit.
 *  \return      VSTDLIB_BITPOS_NA  32-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosOne32(uint32 value);

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosZero32()
 *********************************************************************************************************************/
/*! \brief       Returns the highest bit position of an unset bit in the specified 32-bit value.
 *  \details     -
 *  \param[in]   value              The 32-bit value in which the highest unset bit shall be found.
 *  \return      0-31               Bit position of the highest unset bit.
 *  \return      VSTDLIB_BITPOS_NA  32-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosZero32(uint32 value);

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosOne32()
 *********************************************************************************************************************/
/*! \brief       Returns the lowest bit position of a set bit in the specified 32-bit value.
 *  \details     -
 *  \param[in]   value              The 32-bit value in which the lowest set bit shall be found.
 *  \return      0-31               Bit position of the lowest set bit.
 *  \return      VSTDLIB_BITPOS_NA  32-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosOne32(uint32 value);

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosZero32()
 *********************************************************************************************************************/
/*! \brief       Returns the lowest bit position of an unset bit in the specified 32-bit value.
 *  \details     -
 *  \param[in]   value              The 32-bit value in which the lowest unset bit shall be found.
 *  \return      0-31               Bit position of the lowest unset bit.
 *  \return      VSTDLIB_BITPOS_NA  32-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosZero32(uint32 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingOnes32()
 *********************************************************************************************************************/
/*! \brief       Returns the count of set bits preceding the highest unset bit in the specified 32-bit value.
 *  \details     -
 *  \param[in]   value              The 32-bit value in which the count of leading ones shall be determined.
 *  \return      0-31               Count of leading ones.
 *  \return      VSTDLIB_BITCNT_NA  32-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingOnes32(uint32 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingZeros32()
 *********************************************************************************************************************/
/*! \brief       Returns the count of unset bits preceding the highest set bit in the specified 32-bit value.
 *  \details     -
 *  \param[in]   value              The 32-bit value in which the count of leading zeros shall be determined.
 *  \return      0-31               Count of leading zeros.
 *  \return      VSTDLIB_BITCNT_NA  32-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingZeros32(uint32 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingOnes32()
 *********************************************************************************************************************/
/*! \brief       Returns the count of set bits following the lowest unset bit in the specified 32-bit value.
 *  \details     -
 *  \param[in]   value              The 32-bit value in which the count of trailing ones shall be determined.
 *  \return      0-31               Count of trailing ones.
 *  \return      VSTDLIB_BITCNT_NA  32-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingOnes32(uint32 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingZeros32()
 *********************************************************************************************************************/
/*! \brief       Returns the count of unset bits following the lowest set bit in the specified 32-bit value.
 *  \details     -
 *  \param[in]   value              The 32-bit value in which the count of trailing zeros shall be determined.
 *  \return      0-31               Count of trailing zeros.
 *  \return      VSTDLIB_BITCNT_NA  32-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingZeros32(uint32 value);

# endif /* VSTDLIB_USE_32BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON */

# if ((defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON))

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosOne64()
 *********************************************************************************************************************/
/*! \brief       Returns the highest bit position of a set bit in the specified 64-bit value.
 *  \details     -
 *  \param[in]   value              The 64-bit value in which the highest set bit shall be found.
 *  \return      0-63               Bit position of the highest set bit.
 *  \return      VSTDLIB_BITPOS_NA  64-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosOne64(uint64 value);

/**********************************************************************************************************************
 *  VStdLib_GetHighestBitPosZero64()
 *********************************************************************************************************************/
/*! \brief       Returns the highest bit position of an unset bit in the specified 64-bit value.
 *  \details     -
 *  \param[in]   value              The 64-bit value in which the highest unset bit shall be found.
 *  \return      0-63               Bit position of the highest unset bit.
 *  \return      VSTDLIB_BITPOS_NA  64-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetHighestBitPosZero64(uint64 value);

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosOne64()
 *********************************************************************************************************************/
/*! \brief       Returns the lowest bit position of a set bit in the specified 64-bit value.
 *  \details     -
 *  \param[in]   value              The 64-bit value in which the lowest set bit shall be found.
 *  \return      0-63               Bit position of the lowest set bit.
 *  \return      VSTDLIB_BITPOS_NA  64-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosOne64(uint64 value);

/**********************************************************************************************************************
 *  VStdLib_GetLowestBitPosZero64()
 *********************************************************************************************************************/
/*! \brief       Returns the lowest bit position of an unset bit in the specified 64-bit value.
 *  \details     -
 *  \param[in]   value              The 64-bit value in which the lowest unset bit shall be found.
 *  \return      0-63               Bit position of the lowest unset bit.
 *  \return      VSTDLIB_BITPOS_NA  64-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetLowestBitPosZero64(uint64 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingOnes64()
 *********************************************************************************************************************/
/*! \brief       Returns the count of set bits preceding the highest unset bit in the specified 64-bit value.
 *  \details     -
 *  \param[in]   value              The 64-bit value in which the count of leading ones shall be determined.
 *  \return      0-63               Count of leading ones.
 *  \return      VSTDLIB_BITCNT_NA  64-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingOnes64(uint64 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountLeadingZeros64()
 *********************************************************************************************************************/
/*! \brief       Returns the count of unset bits preceding the highest set bit in the specified 64-bit value.
 *  \details     -
 *  \param[in]   value              The 64-bit value in which the count of leading zeros shall be determined.
 *  \return      0-63               Count of leading zeros.
 *  \return      VSTDLIB_BITCNT_NA  64-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountLeadingZeros64(uint64 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingOnes64()
 *********************************************************************************************************************/
/*! \brief       Returns the count of set bits following the lowest unset bit in the specified 64-bit value.
 *  \details     -
 *  \param[in]   value              The 64-bit value in which the count of trailing ones shall be determined.
 *  \return      0-63               Count of trailing ones.
 *  \return      VSTDLIB_BITCNT_NA  64-bit value contains no unset bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingOnes64(uint64 value);

/**********************************************************************************************************************
 *  VStdLib_GetCountTrailingZeros64()
 *********************************************************************************************************************/
/*! \brief       Returns the count of unset bits following the lowest set bit in the specified 64-bit value.
 *  \details     -
 *  \param[in]   value              The 64-bit value in which the count of trailing zeros shall be determined.
 *  \return      0-63               Count of trailing zeros.
 *  \return      VSTDLIB_BITCNT_NA  64-bit value contains no set bit.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
FUNC(uint8, VSTDLIB_CODE) VStdLib_GetCountTrailingZeros64(uint64 value);

# endif /* (defined PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_SEARCH_LIBRARY_FUNCTIONS != STD_ON) */

# if (VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_ConvertUint16ToUint8ArrayBigEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 16-bit value to an unsigned 8-bit array according to big endian.
 *  \details     -
 *  \param[in]   src   The unsigned 16-bit value which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 8-bit array, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'dst' has to point to an unsigned 8-bit array of at least two elements that is writable
 *               from the calling context.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint16ToUint8ArrayBigEndian(
  uint16 src,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) dst);

/**********************************************************************************************************************
 *  VStdLib_ConvertUint16ToUint8ArrayLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 16-bit value to an unsigned 8-bit array according to little endian.
 *  \details     -
 *  \param[in]   src   The unsigned 16-bit value, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 8-bit array, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'dst' has to point to an unsigned 8-bit array of at least two elements that is writable
 *               from the calling context.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint16ToUint8ArrayLittleEndian(
  uint16 src,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) dst);

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToUint16BigEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 8-bit array to an unsigned 16-bit value according to big endian.
 *  \details     -
 *  \param[in]   src   Pointer to an unsigned 8-bit array, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 16-bit variable, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'src' has to point to an unsigned 8-bit array of at least two elements.
 *               The parameter 'dst' has to point to an unsigned 16-bit variable that is writable from the calling
 *               context.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint8ArrayToUint16BigEndian(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) src,
  P2VAR(uint16, AUTOMATIC, VSTDLIB_APPL_VAR) dst);

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToUint16LittleEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 8-bit array to an unsigned 16-bit value according to little endian.
 *  \details     -
 *  \param[in]   src   Pointer to an unsigned 8-bit array, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 16-bit variable, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'src' has to point to an unsigned 8-bit array of at least two elements.
 *               The parameter 'dst' has to point to an unsigned 16-bit variable that is writable from the calling
 *               context.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint8ArrayToUint16LittleEndian(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) src,
  P2VAR(uint16, AUTOMATIC, VSTDLIB_APPL_VAR) dst);

# endif /* VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON */

# if (VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_ConvertUint32ToUint8ArrayBigEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 32-bit value to an unsigned 8-bit array according to big endian.
 *  \details     -
 *  \param[in]   src   The unsigned 32-bit value, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 8-bit array, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'dst' has to point to an unsigned 8-bit array of at least four elements that is writable
 *               from the calling context.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint32ToUint8ArrayBigEndian(
  uint32 src,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) dst);

/**********************************************************************************************************************
 *  VStdLib_ConvertUint32ToUint8ArrayLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 32-bit value to an unsigned 8-bit array according to little endian.
 *  \details     -
 *  \param[in]   src   The unsigned 32-bit value, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 8-bit array, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'dst' has to point to an unsigned 8-bit array of at least four elements that is writable
 *               from the calling context.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint32ToUint8ArrayLittleEndian(
  uint32 src,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) dst);

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToUint32BigEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 8-bit array to an unsigned 32-bit value according to big endian.
 *  \details     -
 *  \param[in]   src   Pointer to an unsigned 8-bit array, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 32-bit variable, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'src' has to point to an unsigned 8-bit array of at least four elements.
 *               The parameter 'dst' has to point to an unsigned 32-bit variable that is writable from the calling
 *               context.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint8ArrayToUint32BigEndian(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) src,
  P2VAR(uint32, AUTOMATIC, VSTDLIB_APPL_VAR) dst);

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToUint32LittleEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 8-bit array to an unsigned 32-bit value according to little endian.
 *  \details     -
 *  \param[in]   src   Pointer to an unsigned 8-bit array, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 32-bit variable, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'src' has to point to an unsigned 8-bit array of at least two elements.
 *               The parameter 'dst' has to point to an unsigned 32-bit variable that is writable from the calling
 *               context.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint8ArrayToUint32LittleEndian(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) src,
  P2VAR(uint32, AUTOMATIC, VSTDLIB_APPL_VAR) dst);

# endif /* VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON */

# if (defined (PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON))

/**********************************************************************************************************************
 *  VStdLib_ConvertUint64ToUint8ArrayBigEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 64-bit value to an unsigned 8-bit array according to big endian.
 *  \details     -
 *  \param[in]   src   The unsigned 64-bit value, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 8-bit array, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'dst' has to point to an unsigned 8-bit array of at least eight elements that is writable
 *               from the calling context.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint64ToUint8ArrayBigEndian(
  uint64 src,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) dst);

/**********************************************************************************************************************
 *  VStdLib_ConvertUint64ToUint8ArrayLittleEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 64-bit value to an unsigned 8-bit array according to little endian.
 *  \details     -
 *  \param[in]   src   The unsigned 64-bit value, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 8-bit array, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'dst' has to point to an unsigned 8-bit array of at least eight elements that is writable
 *               from the calling context.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint64ToUint8ArrayLittleEndian(
  uint64 src,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) dst);

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToUint64BigEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 8-bit array to an unsigned 64-bit value according to big endian.
 *  \details     -
 *  \param[in]   src   Pointer to an unsigned 8-bit array, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 64-bit variable, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'src' has to point to an unsigned 8-bit array of at least eight elements.
 *               The parameter 'dst' has to point to an unsigned 64-bit variable that is writable from the calling
 *               context.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint8ArrayToUint64BigEndian(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) src,
  P2VAR(uint64, AUTOMATIC, VSTDLIB_APPL_VAR) dst);

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToUint64LittleEndian()
 *********************************************************************************************************************/
/*! \brief       Converts the specified unsigned 8-bit array to an unsigned 64-bit value according to little endian.
 *  \details     -
 *  \param[in]   src   Pointer to an unsigned 8-bit array, which shall be converted.
 *  \param[out]  dst   Pointer to an unsigned 64-bit variable, where the conversion result will be stored.
 *  \context     ANY
 *  \reentrant   TRUE for different memory areas defined by parameter 'dst'
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined. This service is implemented
 *               externally if VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         The parameter 'src' has to point to an unsigned 8-bit array of at least eight elements.
 *               The parameter 'dst' has to point to an unsigned 64-bit variable that is writable from the calling
 *               context.
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint8ArrayToUint64LittleEndian(
  P2CONST(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) src,
  P2VAR(uint64, AUTOMATIC, VSTDLIB_APPL_VAR) dst);

# endif /* defined (PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON) */

# if (VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_SwapEndianUint16()
 *********************************************************************************************************************/
/*! \brief       Swaps the endianness of the specified unsigned 16-bit value.
 *  \details     -
 *  \param[in]   value   Unsigned 16-bit value, whose endianness shall be swapped.
 *  \return      Value in the swapped endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint16, VSTDLIB_CODE) VStdLib_SwapEndianUint16(uint16 value);

# endif /* VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON */

# if (VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_SwapEndianUint32()
 *********************************************************************************************************************/
/*! \brief       Swaps the endianness of the specified unsigned 32-bit value.
 *  \details     -
 *  \param[in]   value   Unsigned 32-bit value, whose endianness shall be swapped.
 *  \return      Value in the swapped endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is implemented externally if VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint32, VSTDLIB_CODE) VStdLib_SwapEndianUint32(uint32 value);

# endif /* VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON */

# if (defined (PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON))

/**********************************************************************************************************************
 *  VStdLib_SwapEndianUint64()
 *********************************************************************************************************************/
/*! \brief       Swaps the endianness of the specified unsigned 64-bit value.
 *  \details     -
 *  \param[in]   value   Unsigned 64-bit value, whose endianness shall be swapped.
 *  \return      Value in the swapped endianness.
 *  \context     ANY
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \config      This service is only available if PLATFORM_SUPPORT_SINT64_UINT64 is defined.This service is implemented
 *               externally if VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS == STD_ON.
 *  \pre         -
 *********************************************************************************************************************/
VSTDLIB_LOCAL_INLINE FUNC(uint64, VSTDLIB_CODE) VStdLib_SwapEndianUint64(uint64 value);

# endif /* defined (PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON) */

# define VSTDLIB_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSTDLIB_START_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if( VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_ConvertUint16ToUint8ArrayBigEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint16ToUint8ArrayBigEndian( /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
  uint16 src,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) dst)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameter 'dst' */
  if(dst == NULL_PTR)
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Convert unsigned 16-bit value to big endian byte array */
#  if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
    /* Optimization possible, if target platform optimized VStdLib_MemCpy() is used */
    VStdLib_MemCpy(dst, &src, 2u); /* PRQA S 0315, 1340 */ /* MD_MSR_VStdLibCopy, MD_VStdLib_Para2ConstPtr */ /* SBSW_VSTDLIB_CALL_MEM_CPY */
#  else
    dst[0u] = (uint8)((src & 0xFF00u) >>  8u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[1u] = (uint8)( src & 0x00FFu);         /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
#  endif /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if(errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_CNV_UINT16_TO_ARRAY_BE, errorId);
  }
#  else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
#  endif
}

/**********************************************************************************************************************
 *  VStdLib_ConvertUint16ToUint8ArrayLittleEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint16ToUint8ArrayLittleEndian( /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
  uint16 src,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) dst)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameter 'dst' */
  if(dst == NULL_PTR)
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Convert unsigned 16-bit value to lillte endian byte array */
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Optimization possible, if target platform optimized VStdLib_MemCpy() is used */
    VStdLib_MemCpy(dst, &src, 2u); /* PRQA S 0315, 1340 */ /* MD_MSR_VStdLibCopy, MD_VStdLib_Para2ConstPtr */ /* SBSW_VSTDLIB_CALL_MEM_CPY */
#  else
    dst[0u] = (uint8)( src & 0x00FFu);         /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[1u] = (uint8)((src & 0xFF00u) >>  8u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
#  endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if(errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_CNV_UINT16_TO_ARRAY_LE, errorId);
  }
#  else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
#  endif
}

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToUint16BigEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint8ArrayToUint16BigEndian( /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
  P2CONST(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) src,
  P2VAR(uint16, AUTOMATIC, VSTDLIB_APPL_VAR) dst)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameter 'dst' */
  if((src == NULL_PTR) || (dst == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Convert big endian byte array to unsigned 16-bit value */
#  if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
    /* Optimization possible, if target platform optimized VStdLib_MemCpy() is used */
    VStdLib_MemCpy(dst, src, 2u); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_VSTDLIB_CALL_MEM_CPY */
#  else
    (*dst) = (uint16)((((uint16)src[0u]) << 8u) |/* SBSW_VSTDLIB_ACCESS_PASSED_INT */
                      ((uint16)src[1u]));
#  endif /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if(errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_CNV_ARRAY_TO_UINT16_BE, errorId);
  }
#  else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
#  endif
}

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToUint16LittleEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint8ArrayToUint16LittleEndian( /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
  P2CONST(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) src,
  P2VAR(uint16, AUTOMATIC, VSTDLIB_APPL_VAR) dst)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'src' and 'dst' */
  if((src == NULL_PTR) || (dst == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Convert lillte endian byte array to unsigned 16-bit value */
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Optimization possible, if target platform optimized VStdLib_MemCpy() is used */
    VStdLib_MemCpy(dst, src, 2u); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_VSTDLIB_CALL_MEM_CPY */
#  else
    (*dst) = (uint16)(((uint16)src[0u]) | /* SBSW_VSTDLIB_ACCESS_PASSED_INT */
                      (((uint16)src[1u]) <<  8u));
#  endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if(errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_CNV_ARRAY_TO_UINT16_LE, errorId);
  }
#  else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
#  endif
}

# endif /* VSTDLIB_USE_16BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON */

# if (VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_ConvertUint32ToUint8ArrayBigEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint32ToUint8ArrayBigEndian( /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
  uint32 src,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) dst)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameter 'dst' */
  if(dst == NULL_PTR)
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Convert unsigned 32-bit value to big endian byte array */
#  if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
    /* Optimization possible, if target platform optimized VStdLib_MemCpy() is used */
    VStdLib_MemCpy(dst, &src, 4u); /* PRQA S 0315, 1340 */ /* MD_MSR_VStdLibCopy, MD_VStdLib_Para2ConstPtr */ /* SBSW_VSTDLIB_CALL_MEM_CPY */
#  else
    dst[0u] = (uint8)((src & 0xFF000000u) >> 24u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[1u] = (uint8)((src & 0x00FF0000u) >> 16u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[2u] = (uint8)((src & 0x0000FF00u) >>  8u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[3u] = (uint8)( src & 0x000000FFu);         /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
#  endif /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if(errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_CNV_UINT32_TO_ARRAY_BE, errorId);
  }
#  else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
#  endif
}

/**********************************************************************************************************************
 *  VStdLib_ConvertUint32ToUint8ArrayLittleEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint32ToUint8ArrayLittleEndian( /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
  uint32 src,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) dst)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameter 'dst' */
  if(dst == NULL_PTR)
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Convert unsigned 32-bit value to lillte endian byte array */
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Optimization possible, if target platform optimized VStdLib_MemCpy() is used */
    VStdLib_MemCpy(dst, &src, 4u); /* PRQA S 0315, 1340 */ /* MD_MSR_VStdLibCopy, MD_VStdLib_Para2ConstPtr */ /* SBSW_VSTDLIB_CALL_MEM_CPY */
#  else
    dst[0u] = (uint8)( src & 0x000000FFu);         /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[1u] = (uint8)((src & 0x0000FF00u) >>  8u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[2u] = (uint8)((src & 0x00FF0000u) >> 16u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[3u] = (uint8)((src & 0xFF000000u) >> 24u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
#  endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if(errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_CNV_UINT32_TO_ARRAY_LE, errorId);
  }
#  else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
#  endif
}

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToUint32BigEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint8ArrayToUint32BigEndian( /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
  P2CONST(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) src,
  P2VAR(uint32, AUTOMATIC, VSTDLIB_APPL_VAR) dst)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'src' and 'dst' */
  if((src == NULL_PTR) || (dst == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Convert big endian byte array to unsigned 32-bit value */
#  if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
    /* Optimization possible, if target platform optimized VStdLib_MemCpy() is used */
    VStdLib_MemCpy(dst, src, 4u); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_VSTDLIB_CALL_MEM_CPY */
#  else
    (*dst) = (uint32)((((uint32)src[0u]) << 24u) | /* SBSW_VSTDLIB_ACCESS_PASSED_INT */
                      (((uint32)src[1u]) << 16u) |
                      (((uint32)src[2u]) <<  8u) |
                      ((uint32)src[3u])
                     );
#  endif /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if(errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_CNV_ARRAY_TO_UINT32_BE, errorId);
  }
#  else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
#  endif
}

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToUint32LittleEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint8ArrayToUint32LittleEndian( /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
  P2CONST(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) src,
  P2VAR(uint32, AUTOMATIC, VSTDLIB_APPL_VAR) dst)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'src' and 'dst' */
  if((src == NULL_PTR) || (dst == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Convert lillte endian byte array to unsigned 32-bit value */
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Optimization possible, if target platform optimized VStdLib_MemCpy() is used */
    VStdLib_MemCpy(dst, src, 4u); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_VSTDLIB_CALL_MEM_CPY */
#  else
    (*dst) = (uint32)( ((uint32)src[0u])         | /* SBSW_VSTDLIB_ACCESS_PASSED_INT */
                      (((uint32)src[1u]) <<  8u) |
                      (((uint32)src[2u]) << 16u) |
                      (((uint32)src[3u]) << 24u)
                     );
#  endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if(errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_CNV_ARRAY_TO_UINT32_LE, errorId);
  }
#  else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
#  endif
}

# endif /* VSTDLIB_USE_32BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON */

# if (defined (PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON))

/**********************************************************************************************************************
 *  VStdLib_ConvertUint64ToUint8ArrayBigEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint64ToUint8ArrayBigEndian( /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
  uint64 src,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) dst)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameter 'dst' */
  if(dst == NULL_PTR)
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Convert unsigned 64-bit value to big endian byte array */
#  if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
    /* Optimization possible, if target platform optimized VStdLib_MemCpy() is used */
    VStdLib_MemCpy(dst, &src, 8u); /* PRQA S 0315, 1340 */ /* MD_MSR_VStdLibCopy, MD_VStdLib_Para2ConstPtr */ /* SBSW_VSTDLIB_CALL_MEM_CPY */
#  else
    dst[0u] = (uint8)((src & 0xFF00000000000000uLL) >> 56u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[1u] = (uint8)((src & 0x00FF000000000000uLL) >> 48u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[2u] = (uint8)((src & 0x0000FF0000000000uLL) >> 40u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[3u] = (uint8)((src & 0x000000FF00000000uLL) >> 32u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[4u] = (uint8)((src & 0x00000000FF000000uLL) >> 24u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[5u] = (uint8)((src & 0x0000000000FF0000uLL) >> 16u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[6u] = (uint8)((src & 0x000000000000FF00uLL) >>  8u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[7u] = (uint8)( src & 0x00000000000000FFuLL);         /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
#  endif /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if(errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_CNV_UINT64_TO_ARRAY_BE, errorId);
  }
#  else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
#  endif
}

/**********************************************************************************************************************
 *  VStdLib_ConvertUint64ToUint8ArrayLittleEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint64ToUint8ArrayLittleEndian( /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
  uint64 src,
  P2VAR(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) dst)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameter 'dst' */
  if(dst == NULL_PTR)
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Convert unsigned 64-bit value to lillte endian byte array */
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Optimization possible, if target platform optimized VStdLib_MemCpy() is used */
    VStdLib_MemCpy(dst, &src, 8u); /* PRQA S 0315, 1340 */ /* MD_MSR_VStdLibCopy, MD_VStdLib_Para2ConstPtr */ /* SBSW_VSTDLIB_CALL_MEM_CPY */
#  else
    dst[0u] = (uint8)( src & 0x00000000000000FFuLL);         /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[1u] = (uint8)((src & 0x000000000000FF00uLL) >>  8u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[2u] = (uint8)((src & 0x0000000000FF0000uLL) >> 16u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[3u] = (uint8)((src & 0x00000000FF000000uLL) >> 24u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[4u] = (uint8)((src & 0x000000FF00000000uLL) >> 32u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[5u] = (uint8)((src & 0x0000FF0000000000uLL) >> 40u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[6u] = (uint8)((src & 0x00FF000000000000uLL) >> 48u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
    dst[7u] = (uint8)((src & 0xFF00000000000000uLL) >> 56u); /* SBSW_VSTDLIB_ACCESS_PASSED_ARRAY */
#  endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if(errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_CNV_UINT64_TO_ARRAY_LE, errorId);
  }
#  else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
#  endif
}

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToUint64BigEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint8ArrayToUint64BigEndian( /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
  P2CONST(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) src,
  P2VAR(uint64, AUTOMATIC, VSTDLIB_APPL_VAR) dst)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'src' and 'dst' */
  if((src == NULL_PTR) || (dst == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Convert big endian byte array to unsigned 64-bit value */
#  if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
    /* Optimization possible, if target platform optimized VStdLib_MemCpy() is used */
    VStdLib_MemCpy(dst, src, 8u); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_VSTDLIB_CALL_MEM_CPY */
#  else
    (*dst) = (uint64)((((uint64)src[0u]) << 56u) | /* SBSW_VSTDLIB_ACCESS_PASSED_INT */
                      (((uint64)src[1u]) << 48u) |
                      (((uint64)src[2u]) << 40u) |
                      (((uint64)src[3u]) << 32u) |
                      (((uint64)src[4u]) << 24u) |
                      (((uint64)src[5u]) << 16u) |
                      (((uint64)src[6u]) <<  8u) |
                       ((uint64)src[7u])
                     );
#  endif /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if(errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_CNV_ARRAY_TO_UINT64_BE, errorId);
  }
#  else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
#  endif
}

/**********************************************************************************************************************
 *  VStdLib_ConvertUint8ArrayToUint64LittleEndian()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(void, VSTDLIB_CODE) VStdLib_ConvertUint8ArrayToUint64LittleEndian( /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
  P2CONST(uint8, AUTOMATIC, VSTDLIB_APPL_VAR) src,
  P2VAR(uint64, AUTOMATIC, VSTDLIB_APPL_VAR) dst)
{
  /* ----- Local Variables ---------------------------------------------- */
  uint8 errorId = VSTDLIB_E_NO_ERROR;

  /* ----- Development Error Checks ------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_DETECT == STD_ON)
  /* #10 Check parameters 'src' and 'dst' */
  if((src == NULL_PTR) || (dst == NULL_PTR))
  {
    errorId = VSTDLIB_E_PARAM_POINTER;
  }
  else
#  endif
  {
    /* ----- Implementation ----------------------------------------------- */
    /* #100 Convert lillte endian byte array to unsigned 64-bit value */
#  if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
    /* Optimization possible, if target platform optimized VStdLib_MemCpy() is used */
    VStdLib_MemCpy(dst, src, 8u); /* PRQA S 0315 */ /* MD_MSR_VStdLibCopy */ /* SBSW_VSTDLIB_CALL_MEM_CPY */
#  else
    (*dst) = (uint64)( ((uint64)src[0u])         | /* SBSW_VSTDLIB_ACCESS_PASSED_INT */
                      (((uint64)src[1u]) <<  8u) |
                      (((uint64)src[2u]) << 16u) |
                      (((uint64)src[3u]) << 24u) |
                      (((uint64)src[4u]) << 32u) |
                      (((uint64)src[5u]) << 40u) |
                      (((uint64)src[6u]) << 48u) |
                      (((uint64)src[7u]) << 56u)
                     );
#  endif /* CPU_BYTE_ORDER == LOW_BYTE_FIRST */
  }

  /* ----- Development Error Report --------------------------------------- */
#  if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if(errorId != VSTDLIB_E_NO_ERROR)
  {
    (void)Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_CNV_ARRAY_TO_UINT64_LE, errorId);
  }
#  else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 1338, 2983, 3122 */ /* MD_MSR_DummyStmt */
#  endif
}

# endif /* defined (PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_DE_SERIALIZATION_LIBRARY_FUNCTIONS != STD_ON) */

# if (VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_SwapEndianUint16()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(uint16, VSTDLIB_CODE) VStdLib_SwapEndianUint16(uint16 value) /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
{
  /* ----- Implementation ----------------------------------------------- */

  /* #100 Swap endianness */
  return (uint16)(((value & 0xFF00u) >> 8u) | ((value & 0x00FFu) << 8u));
}

# endif /* VSTDLIB_USE_16BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON */

# if (VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON)

/**********************************************************************************************************************
 *  VStdLib_SwapEndianUint32()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(uint32, VSTDLIB_CODE) VStdLib_SwapEndianUint32(uint32 value) /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
{
  /* ----- Implementation ----------------------------------------------- */

  /* #100 Swap endianness */
  return (uint32)(((value & 0xFF000000u) >> 24u) | /* 3 to 0 */
                  ((value & 0x00FF0000u) >>  8u) | /* 2 to 1 */
                  ((value & 0x0000FF00u) <<  8u) | /* 1 to 2 */
                  ((value & 0x000000FFu) << 24u)   /* 0 to 3 */
                 );
}

# endif /* VSTDLIB_USE_32BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON */

# if (defined (PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON))

/**********************************************************************************************************************
 *  VStdLib_SwapEndianUint64()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSTDLIB_LOCAL_INLINE FUNC(uint64, VSTDLIB_CODE) VStdLib_SwapEndianUint64(uint64 value) /* PRQA S 3219 */ /* MD_VStdLib_NotUsedInUnit */
{
  /* ----- Implementation ----------------------------------------------- */

  /* #100 Swap endianness */
  return (uint64)(((value & 0xFF00000000000000uLL) >> 56u) | /* 7 to 0 */
                  ((value & 0x00FF000000000000uLL) >> 40u) | /* 6 to 1 */
                  ((value & 0x0000FF0000000000uLL) >> 24u) | /* 5 to 2 */
                  ((value & 0x000000FF00000000uLL) >>  8u) | /* 4 to 3 */
                  ((value & 0x00000000FF000000uLL) <<  8u) | /* 3 to 4 */
                  ((value & 0x0000000000FF0000uLL) << 24u) | /* 2 to 5 */
                  ((value & 0x000000000000FF00uLL) << 40u) | /* 1 to 6 */
                  ((value & 0x00000000000000FFuLL) << 56u)   /* 0 to 7 */
                 );
}

# endif /* defined (PLATFORM_SUPPORT_SINT64_UINT64) && (VSTDLIB_USE_64BIT_ENDIAN_CONVERSION_LIBRARY_FUNCTIONS != STD_ON) */

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianPlatformToBig_s()
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
VSTDLIB_LOCAL_INLINE FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_MemCpyEndianPlatformToBig_s( /* PRQA S 3219, 6060 */ /* MD_VStdLib_NotUsedInUnit, MD_MSR_STPAR */
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nCnt,
  uint8 intType,
  Std_ReturnType errorCode)
{
# if (CPU_BYTE_ORDER == HIGH_BYTE_FIRST)
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
    /* #20 Call VStdLib_MemCpy() */
    VStdLib_MemCpy(pDst, pSrc, nCnt); /* SBSW_VSTDLIB_CALL_UNMODIFIED */
    /* retval is already set to E_OK */
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void) Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_MEM_COPY_S, errorId);
  }
#else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  VSTDLIB_DUMMY_STATEMENT(intType); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
  return retval;

# else /* CPU_BYTE_ORDER == LOW_BYTE_FIRST) */
  /* #30 Call VStdLib_MemCpySwapEndian_s() */
  return VStdLib_MemCpySwapEndian_s(pDst, nDstSize, pSrc, nCnt, intType, errorCode); /* SBSW_VSTDLIB_CALL_UNMODIFIED */
# endif /* CPU_BYTE_ORDER */
}

/**********************************************************************************************************************
 *  VStdLib_MemCpyEndianPlatformToLittle_s()
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
VSTDLIB_LOCAL_INLINE FUNC(Std_ReturnType, VSTDLIB_CODE) VStdLib_MemCpyEndianPlatformToLittle_s( /* PRQA S 3219, 6060 */ /* MD_VStdLib_NotUsedInUnit, MD_MSR_STPAR */
  P2VAR(void, AUTOMATIC, VSTDLIB_VAR_FAR) pDst,
  VStdLib_CntType nDstSize,
  P2CONST(void, AUTOMATIC, VSTDLIB_VAR_FAR) pSrc,
  VStdLib_CntType nCnt,
  uint8 intType,
  Std_ReturnType errorCode)
{
# if (CPU_BYTE_ORDER == LOW_BYTE_FIRST)
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
    /* #20 Call VStdLib_MemCpy() */
    VStdLib_MemCpy(pDst, pSrc, nCnt); /* SBSW_VSTDLIB_CALL_UNMODIFIED */
    /* retval is already set to E_OK */
  }

  /* ----- Development Error Report ------------------------------------- */
#if (VSTDLIB_DEV_ERROR_REPORT == STD_ON)
  if (errorId != VSTDLIB_E_NO_ERROR)
  {
    (void) Det_ReportError(VSTDLIB_MODULE_ID, VSTDLIB_INSTANCE_ID_DET, VSTDLIB_SID_MEM_COPY_S, errorId);
  }
#else
  VSTDLIB_DUMMY_STATEMENT(errorId); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
#endif
  VSTDLIB_DUMMY_STATEMENT(intType); /* PRQA S 2983, 3112 */ /* MD_MSR_DummyStmt */
  return retval;

# else /* CPU_BYTE_ORDER == HIGH_BYTE_FIRST) */
  /* #30 Call VStdLib_MemCpySwapEndian_s() */
  return VStdLib_MemCpySwapEndian_s(pDst, nDstSize, pSrc, nCnt, intType, errorCode); /* SBSW_VSTDLIB_CALL_UNMODIFIED */
# endif /* CPU_BYTE_ORDER */
}

# define VSTDLIB_STOP_SEC_CODE
# include "MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  COMPATIBILITY MACROS
 *********************************************************************************************************************/

# define VSTDLIB__COREHLL_VERSION            (0x0308u) /* last core version this component was based on */
# define VSTDLIB__COREHLL_RELEASE_VERSION    (0x00u)
# define VSTDLIB_GENERICASR_VERSION          ((VSTDLIB_SW_MAJOR_VERSION << 8u) | VSTDLIB_SW_MINOR_VERSION)
# define VSTDLIB_GENERICASR_RELEASE_VERSION  VSTDLIB_SW_PATCH_VERSION
# define kVStdMainVersion                    VSTDLIB_SW_MAJOR_VERSION
# define kVStdSubVersion                     VSTDLIB_SW_MINOR_VERSION
# define kVStdReleaseVersion                 VSTDLIB_SW_PATCH_VERSION

# define kVStdErrorIntDisableTooOften        (0x01u)
# define kVStdErrorIntRestoreTooOften        (0x02u)
# define kVStdErrorMemClrInvalidParameter    (0x03u)
# define kVStdErrorMemCpyInvalidParameter    (0x04u)
# define kVStdErrorFunctionNotSupportedByHw  (0x05u)
# define kVStdErrorMemSetInvalidParameter    (0x06u)
# define kVStdErrorUnexpectedLockState       (0x07u)

# define VSTD_HL_DISABLE_SUPPORT_MEM_FCT
# define VSTD_HL_DISABLE_MEM_FCT_MAPPING
# define VSTD_HL_USE_VSTD_MEMCLR
# define VSTD_HL_USE_VSTD_MEMSET
# define VSTD_HL_USE_VSTD_RAMMEMCPY
# define VSTD_HL_USE_VSTD_ROMMEMCPY
# define VSTD_HL_USE_VSTD_WORD_MEMCPY
# define VSTD_HL_USE_VSTD_DWORD_MEMCPY

# define VStdInitPowerOn()

/* PRQA S 3453 VSTDLIB_FUNCTION_LIKE_MACRO */ /* MD_MSR_FctLikeMacro */

# if !defined (VStdLib_MemCpyRom)
#  define VStdLib_MemCpyRom(d, s, c)         VStdLib_MemCpy((d), (s), (c))
# endif

# define VStdMemClr(d, c)                    VStdLib_MemClr((d), (c))
# define VStdMemNearClr(d, c)                VStdLib_MemClr((d), (c))
# define VStdMemFarClr(d, c)                 VStdLib_MemClr((d), (c))
# define VStdMemSet(d, p, c)                 VStdLib_MemSet((d), (p), (c))
# define VStdMemNearSet(d, p, c)             VStdLib_MemSet((d), (p), (c))
# define VStdMemFarSet(d, p, c)              VStdLib_MemSet((d), (p), (c))

# define VStdMemCpy(d, s, c)                 VStdLib_MemCpy((d), (s), (c))
# define VStdMemCpyRom(d, s, c)              VStdLib_MemCpyRom((d), (s), (c))
# define VStdMemCpyRamToRam(d, s, c)         VStdLib_MemCpy((d), (s), (c))
# define VStdMemCpyRomToRam(d, s, c)         VStdLib_MemCpyRom((d), (s), (c))
# define VStdMemCpyRamToNearRam(d, s, c)     VStdLib_MemCpy((d), (s), (c))
# define VStdMemCpyRomToNearRam(d, s, c)     VStdLib_MemCpyRom((d), (s), (c))
# define VStdMemCpyRamToFarRam(d, s, c)      VStdLib_MemCpy((d), (s), (c))
# define VStdMemCpyRomToFarRam(d, s, c)      VStdLib_MemCpyRom((d), (s), (c))
# define VStdMemCpyFarRamToRam(d, s, c)      VStdLib_MemCpy((d), (s), (c))
# define VStdMemCpyFarRamToFarRam(d, s, c)   VStdLib_MemCpy((d), (s), (c))
# define VStdMemCpyFarRomToFarRam(d, s, c)   VStdLib_MemCpyRom((d), (s), (c))
# define VStdRamMemCpy(d, s, c)              VStdLib_MemCpy((d), (s), (c))
# define VStdRomMemCpy(d, s, c)              VStdLib_MemCpyRom((d), (s), (c))

# define VStdMemCpy16(d, s, c)               VStdLib_MemCpy16((d), (s), (c))
# define VStdMemCpy16RamToRam(d, s, c)       VStdLib_MemCpy16((d), (s), (c))
# define VStdMemCpy16RamToNearRam(d, s, c)   VStdLib_MemCpy16((d), (s), (c))
# define VStdMemCpy16NearRamToRam(d, s, c)   VStdLib_MemCpy16((d), (s), (c))
# define VStdMemCpy16RamToFarRam(d, s, c)    VStdLib_MemCpy16((d), (s), (c))
# define VStdMemCpy16FarRamToRam(d, s, c)    VStdLib_MemCpy16((d), (s), (c))
# define VStdRamMemCpy16(d, s, c)            VStdLib_MemCpy16((d), (s), (c))

# define VStdMemCpy32(d, s, c)               VStdLib_MemCpy32((d), (s), (c))
# define VStdMemCpy32RamToRam(d, s, c)       VStdLib_MemCpy32((d), (s), (c))
# define VStdMemCpy32RamToNearRam(d, s, c)   VStdLib_MemCpy32((d), (s), (c))
# define VStdMemCpy32NearRamToRam(d, s, c)   VStdLib_MemCpy32((d), (s), (c))
# define VStdMemCpy32RamToFarRam(d, s, c)    VStdLib_MemCpy32((d), (s), (c))
# define VStdMemCpy32FarRamToRam(d, s, c)    VStdLib_MemCpy32((d), (s), (c))
# define VStdRamMemCpy32(d, s, c)            VStdLib_MemCpy32((d), (s), (c))

# if !defined (VStdLib_MemCpyRomMacro)
#  define VStdLib_MemCpyRomMacro(d, s, c)    VStdLib_MemCpyMacro((d), (s), (c))
# endif

# define VStdMemClr_Macro(d, c)              VStdLib_MemClrMacro((d), (c))
# define VStdMemSet_Macro(d, p, c)           VStdLib_MemSetMacro((d), (p), (c))
# define VStdMemCpy_Macro(d, s, c)           VStdLib_MemCpyMacro((d), (s), (c))
# define VStdMemCpyRom_Macro(d, s, c)        VStdLib_MemCpyRomMacro((d), (s), (c))

/* PRQA L:VSTDLIB_FUNCTION_LIKE_MACRO */

#endif /* VSTDLIB_H */

/**********************************************************************************************************************
 *  END OF FILE: vstdlib.h
 *********************************************************************************************************************/
