/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2016 cv cryptovision GmbH.                                                All rights reserved.
 *
 *  For modifications by Vector Informatik GmbH:
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under intellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actUtilities.h
 *        \brief  Interface for useful helper functions.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library actCLib
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to module's header file.
 *********************************************************************************************************************/

#ifndef ACTUTILITIES_H
# define ACTUTILITIES_H

# include "actITypes.h"
# include "actConfig.h"
# include "vstdlib.h"

 /**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
# define actMin(x, y)                                                 (((x)<(y))? (x):(y))
# define actMax(x, y)                                                 (((x)>(y))? (x):(y))

# define actBitsToBytes(x)                                            (x >> 3u)
# define actBytesToBits(x)                                            (x << 3u)

/* get a byte out of a long */
# define actGetByte(x, n)                                             ((actU8)(((x)>>((n)<<3)) & 0xFFuL))

# define actGetLowByteFromU16(x)                                      ((actU8) actLOW_BYTE (x))
# define actGetU8fromU16(x, n)                                        ((actU8) actLOW_BYTE (((x) >> ((n) <<2u))))
# define actGetU8fromU32(x, n)                                        ((actU8) actLOW_BYTE (((x) >> ((n) <<3u))))
# define actGetU8fromU64(x, n)                                        ((actU8) actLOW_BYTE (((x) >> ((n) <<4u))))

/* rotate functions for dwords */
# define actRotr(x, n)                                                (((x) >> ((actU32)(n))) | ((x) << (32uL - (actU32)(n))))
# define actRotl(x, n)                                                (((x) << ((actU32)(n))) | ((x) >> (32uL - (actU32)(n))))

/* rotate functions for words */
# define actRotr16(x, n)                                              (((x) >> ((actU32)(n))) | ((x) << (16uL - (actU32)(n))))
# define actRotl16(x, n)                                              (((x) << ((actU32)(n))) | ((x) >> (16uL - (actU32)(n))))

/* rotate functions for qwords */
# define actRotr64(x, n)                                              (((x) >> ((actU32)(n))) | ((x) << (64uL - (actU32)(n))))
# define actRotl64(x, n)                                              (((x) << ((actU32)(n))) | ((x) >> (64uL - (actU32)(n))))

/* byte to qwords */
# define actU8toU64(x)                                                \
                                                                      (((actU64)((x)[0])) | ((actU64)((x)[1]) << 8) | \
                                                                      ((actU64)((x)[2]) << 16) | ((actU64)((x)[3]) << 24) | \
                                                                      ((actU64)((x)[4]) << 32) | ((actU64)((x)[5]) << 40) | \
                                                                      ((actU64)((x)[6]) << 48) | ((actU64)((x)[7]) << 56))

# if (VSECPRIM_USE_VSTD_LIB == STD_ON)

/* use VStdLib */
#  define actMemset(dest, fill, count)                                (VStdLib_MemSet((dest), (fill), (count))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define actMemClear(dest, count)                                    (VStdLib_MemClr((dest), (count))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  define actMemCpyByteArray(dest, src, count)                        (VStdLib_MemCpy((dest), (src), (count))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */

#  if (actBN_BITS_PER_DIGIT == 32u)
#   define actMemCpyWordArray(dest, src, count)                       (VStdLib_MemCpy32((VSECPRIM_P2VAR_PARA(actBNDIGIT))(dest), (VSECPRIM_P2CONST_PARA(actBNDIGIT))(src), (count))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  endif

#  if (actBN_BITS_PER_DIGIT == 16u)
#   define actMemCpyWordArray(dest, src, count)                       (VStdLib_MemCpy16((VSECPRIM_P2VAR_PARA(actBNDIGIT))(dest), (VSECPRIM_P2CONST_PARA(actBNDIGIT))(src), (count))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  endif

#  if (actBN_BITS_PER_DIGIT == 8u) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#   define actMemCpyWordArray(dest, src, count)                       (VStdLib_MemCpy((VSECPRIM_P2VAR_PARA(actBNDIGIT))(dest), (VSECPRIM_P2CONST_PARA(actBNDIGIT))(src), (count))) /* PRQA S 3453 */ /* MD_MSR_FctLikeMacro */
#  endif

# else

/* use internal implementation */
#  define actMemset(dest, fill, count)                                (actMemSet((dest), fill, count))
#  define actMemClear(dest, count)                                    (actMemSet((dest), 0u, count))
#  define actMemCpyByteArray(dest, src, count)                        (actMemcpy((dest), (src), count))
#  define actMemCpyWordArray(dest, src, count)                        (actBNCopy((VSECPRIM_P2VAR_PARA(actBNDIGIT))(dest), (VSECPRIM_P2CONST_PARA(actBNDIGIT))(src), count))

# endif

/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  actWatchdogTriggerFunction()
 *********************************************************************************************************************/
/*! \brief        Triggers watchdog function.
 *  \details       Triggers the watchdog through the given function pointer.
 *  \param[in]     watchdog  Function pointer to watchdog trigger function.
 *  \pre           -
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actWatchdogTriggerFunction(VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actMemSet()
 *********************************************************************************************************************/
/*! \brief        Sets the values of an array.
 *  \details       Sets the values of an array to a certain value.
 *  \param[out]    dest  Pointer to the array which shall be set.
 *  \param[in]     fill  fill value which shall be written to the array.
 *  \param[in]     count Number of bytes which shall be set.
 *  \pre           dest must be a valid pointer
 *                 dest must hold count amount of bytes
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actMemSet(VSECPRIM_P2VAR_PARA(void) dest, actU8 fill, actLengthType count);

/**********************************************************************************************************************
 *  actMemcpy()
 *********************************************************************************************************************/
/*! \brief        Copies source array to destination array.
 *  \details       Copies count bytes from source array to destination array.
 *  \param[out]    dest  Pointer to the array which shall be overwritten.
 *  \param[in]     src   Source pointer of the data
 *  \param[in]     count Number of bytes which shall be copied.
 *  \pre           dest, src must be valid pointers
 *                 dest must hold count amount of bytes
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actMemcpy(VSECPRIM_P2VAR_PARA(void) dest, VSECPRIM_P2CONST_PARA(void) src, actU32 count);

/**********************************************************************************************************************
 *  actMemcpy_ROM()
 *********************************************************************************************************************/
/*! \brief        Copies source array to destination array.
 *  \details       Copies count bytes from ROM source array to destination array.
 *  \param[out]    dest  Pointer to the array which shall be overwritten.
 *  \param[in]     src   Source pointer of the data
 *  \param[in]     count Number of bytes which shall be copied.
 *  \pre           dest, src must be valid pointers
 *                 dest must hold count amount of bytes
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actMemcpy_ROM(VSECPRIM_P2VAR_PARA(void) dest, VSECPRIM_P2CONST_PARA(void) src, actLengthType count);

/**********************************************************************************************************************
 *  actMemcpy_ROMRAM()
 *********************************************************************************************************************/
/*! \brief        Copies source array to destination array.
 *  \details       Copies count bytes from ROM source array to RAM destination array.
 *  \param[out]    dest  Pointer to the array which shall be overwritten.
 *  \param[in]     src   Source pointer of the data
 *  \param[in]     count Number of bytes which shall be copied.
 *  \pre           dest, src must be valid pointers
 *                 dest must hold count amount of bytes
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actMemcpy_ROMRAM(VSECPRIM_P2VAR_PARA(void) dest, VSECPRIM_P2ROMCONST_PARA(actU8) src, actLengthType count);

/**********************************************************************************************************************
 *  actMemcmp()
 *********************************************************************************************************************/
/*! \brief        Compares two arrays.
 *  \details       Compares the first count values of two arrays for equality.
 *  \param[in]     buf1  Pointer to first array.
 *  \param[in]     buf2  Pointer to second array.
 *  \param[in]     count Number of bytes which shall compared.
 *  \return        TRUE - equal
 *                 FALSE - else
 *  \pre           buf1, buf2 must be valid pointers
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(actU8) actMemcmp(VSECPRIM_P2CONST_PARA(void) buf1, VSECPRIM_P2CONST_PARA(void) buf2, actLengthType count);

/**********************************************************************************************************************
 *  actMemcmp_ROM()
 *********************************************************************************************************************/
/*! \brief        Compares two arrays located in ROM.
 *  \details       Compares the first count values of two arrays located in ROM for equality.
 *  \param[in]     buf1  Pointer to first array.
 *  \param[in]     buf2  Pointer to second array.
 *  \param[in]     count Number of bytes which shall compared.
 *  \return        TRUE - equal
 *                 FALSE - else
 *  \pre           buf1, buf2 must be valid pointers
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(actU8) actMemcmp_ROM(VSECPRIM_P2ROMCONST_PARA(actU8) buf1, VSECPRIM_P2ROMCONST_PARA(actU8) buf2, actLengthType count);

/**********************************************************************************************************************
 *  actMemcmp_ROMRAM()
 *********************************************************************************************************************/
/*! \brief        Compares an array lying in ROM with an array lying in RAM.
 *  \details       Compares the first count values of two arrays for equality.
 *  \param[in]     buf1  Pointer to first array lying in RAM.
 *  \param[in]     buf2  Pointer to second array lying in ROM.
 *  \param[in]     count Number of bytes which shall compared.
 *  \return        TRUE - equal
 *                 FALSE - else
 *  \pre           buf1, buf2 must be valid pointers
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(actU8) actMemcmp_ROMRAM(VSECPRIM_P2CONST_PARA(void) buf1, VSECPRIM_P2ROMCONST_PARA(actU8) buf2, actLengthType count);

/**********************************************************************************************************************
 *  actXOR()
 *********************************************************************************************************************/
/*! \brief         Xors an array bytewise to another.
 *  \details        Xors an array bytewise to another. The number of bytes to be xored is defined in parameter count. XOR (buf1 ^= buf2).
 *  \param[in,out]  buf1  In-Out buffer which gets Xored.
 *  \param[in]      buf2  Input pointer of the XOR values.
 *  \param[in]      count Number of bytes which shall xored.
 *  \pre            buf1, buf2 must be valid pointers
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actXOR(VSECPRIM_P2VAR_PARA(void) buf1, VSECPRIM_P2CONST_PARA(void) buf2, actU32 count);

/**********************************************************************************************************************
 *  actAddBE()
 *********************************************************************************************************************/
/*! \brief       Addition of two big endian arrays
 *  \details     This function adds two big endian arrays. It accepts and returns a carry.
 *  \param[in,out]   tgt  Pointer to u8 array, will be written with the result
 *  \param[in]   src  Pointer to u8 array
 *  \param[in]   length  Size of tgt/src array in bytes
 *  \param[in]   carry  Carry as input
 *  \return      carry of the result
 *  \pre         Pointers must be valid.
 *               src must be 1 byte long.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(actU8) actAddBE(VSECPRIM_P2VAR_PARA(actU8) tgt, VSECPRIM_P2CONST_PARA(actU8) src, const actU32 length, const actU8 carry);

/**********************************************************************************************************************
 *  actLEtoU8()
 *********************************************************************************************************************/
/*! \brief       Copy u8 little endian array into u8 value
 *  \details     This function copies u8 value into u8 array in little endian format.
 *  \param[in]   src  Pointer to u8 array
 *  \return      u8 value
 *  \pre         Pointer must be valid.
 *               src must be 1 byte long.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(actU8) actLEtoU8(VSECPRIM_P2CONST_PARA(actU8) src);

/**********************************************************************************************************************
 *  actU8toLE()
 *********************************************************************************************************************/
/*! \brief       Copy u8 value to u8 little endian array
 *  \details     This function copies u8 value into u8 array in little endian format.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   in   u8 value input
 *  \pre         Pointers must be valid.
 *               tgt must be 1 byte long.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actU8toLE(VSECPRIM_P2VAR_PARA(actU8) tgt, const actU8 in);

/**********************************************************************************************************************
 *  actCpyLEtoU8()
 *********************************************************************************************************************/
/*! \brief       Copy u8 little endian array to u8 array
 *  \details     This function copies u8 array in little endian format into u8 array.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  Pointer to source array
 *  \param[in]   length  Size of tgt/src array in bytes
 *  \pre         Pointers must be valid.
 *               Array sizes must be correct.
 *               elements of src = elements of tgt
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actCpyLEtoU8(VSECPRIM_P2VAR_PARA(actU8) tgt, VSECPRIM_P2CONST_PARA(actU8) src, const actU32 length);

/**********************************************************************************************************************
 *  actCpyU8toLE()
 *********************************************************************************************************************/
/*! \brief       Copy u8 to u8 little endian array
 *  \details     This function copies u8 array into u8 array in little endian format.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  Pointer to source array
 *  \param[in]   length  Size of tgt/src array in bytes
 *  \pre         Pointers must be valid.
 *               Array sizes must be correct.
 *               elements of tgt = elements of src
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actCpyU8toLE(VSECPRIM_P2VAR_PARA(actU8) tgt, VSECPRIM_P2CONST_PARA(actU8) src, const actU32 length);

/**********************************************************************************************************************
 *  actLEtoU16()
 *********************************************************************************************************************/
/*! \brief       Copy u8 little endian array into u16 value
 *  \details     This function copies u16 value into u8 array in little endian format.
 *  \param[in]   src  Pointer to u8 array
 *  \return      u16 value
 *  \pre         Pointer must be valid.
 *               src array must be 2 bytes long.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(actU16) actLEtoU16(VSECPRIM_P2CONST_PARA(actU8) src);

/**********************************************************************************************************************
 *  actU16toLE()
 *********************************************************************************************************************/
/*! \brief       Copy u16 value to u8 little endian array
 *  \details     This function copies u16 value into u8 array in little endian format.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  u16 value input
 *  \pre         Pointers must be valid.
 *               tgt array must be 2 bytes long.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actU16toLE(VSECPRIM_P2VAR_PARA(actU8) tgt, const actU16 src);

/**********************************************************************************************************************
 *  actCpyLEtoU16()
 *********************************************************************************************************************/
/*! \brief       Copy u8 little endian array to u16 array
 *  \details     This function copies u8 array in little endian format into u16 array.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  Pointer to source array
 *  \param[in]   length  Size of tgt/src array in bytes
 *  \pre         Pointers must be valid.
 *               Array sizes must be correct.
 *               elements of src * 2 = elements of tgt
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actCpyLEtoU16(VSECPRIM_P2VAR_PARA(actU16) tgt, VSECPRIM_P2CONST_PARA(actU8) src, const actU32 length);

/**********************************************************************************************************************
 *  actCpyU16toLE()
 *********************************************************************************************************************/
/*! \brief       Copy u16 to u8 little endian array
 *  \details     This function copies u16 array into u8 array in little endian format.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  Pointer to source array
 *  \param[in]   length  Size of tgt/src array in bytes
 *  \pre         Pointers must be valid.
 *               Array sizes must be correct.
 *               elements of tgt = 2 * elements of src
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actCpyU16toLE(VSECPRIM_P2VAR_PARA(actU8) tgt, VSECPRIM_P2CONST_PARA(actU16) src, const actU32 length);

/**********************************************************************************************************************
 *  actBEtoU32()
 *********************************************************************************************************************/
/*! \brief       Copy u8 big endian array into u32 value
 *  \details     This function copies u32 value into u8 array in big endian format.
 *  \param[in]   src  Pointer to u8 array
 *  \return      u32 value
 *  \pre         Pointer must be valid.
 *               src array must be 4 bytes long.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(actU32) actBEtoU32(VSECPRIM_P2CONST_PARA(actU8) src);

/**********************************************************************************************************************
 *  actLEtoU32()
 *********************************************************************************************************************/
/*! \brief       Copy u8 little endian array into u32 value
 *  \details     This function copies u32 value into u8 array in little endian format.
 *  \param[in]   src  Pointer to u8 array
 *  \return      u32 value
 *  \pre         Pointer must be valid.
 *               src array must be 4 bytes long.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(actU32) actLEtoU32(VSECPRIM_P2CONST_PARA(actU8) src);

/**********************************************************************************************************************
 *  actU32toBE()
 *********************************************************************************************************************/
/*! \brief       Copy u32 value to u8 big endian array
 *  \details     This function copies u32 value into u8 array in big endian format.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  u32 value input
 *  \pre         Pointers must be valid.
 *               tgt array must be 4 bytes long.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actU32toBE(VSECPRIM_P2VAR_PARA(actU8) tgt, const actU32 src);

/**********************************************************************************************************************
 *  actU32toLE()
 *********************************************************************************************************************/
/*! \brief       Copy u32 value to u8 little endian array
 *  \details     This function copies u32 value into u8 array in little endian format.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  u32 value input
 *  \pre         Pointers must be valid.
 *               tgt array must be 4 bytes long.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actU32toLE(VSECPRIM_P2VAR_PARA(actU8) tgt, const actU32 src);

/**********************************************************************************************************************
 *  actCpyBEtoU32()
 *********************************************************************************************************************/
/*! \brief       Copy u8 big endian array to u32 array
 *  \details     This function copies u8 array in big endian format into u32 array.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  Pointer to source array
 *  \param[in]   length  Size of tgt/src array in bytes
 *  \pre         Pointers must be valid.
 *               Array sizes must be correct.
 *               elements of src * 4 = elements of tgt
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actCpyBEtoU32(
  VSECPRIM_P2VAR_PARA(actU32) tgt,
  VSECPRIM_P2CONST_PARA(actU8) src,
  const actU32 length);

/**********************************************************************************************************************
 *  actCpyLEtoU32()
 *********************************************************************************************************************/
/*! \brief       Copy u8 little endian array to u32 array
 *  \details     This function copies u8 array in little endian format into u32 array.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  Pointer to source array
 *  \param[in]   length  Size of tgt/src array in bytes
 *  \pre         Pointers must be valid.
 *               Array sizes must be correct.
 *               elements of src * 4 = elements of tgt
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actCpyLEtoU32(
  VSECPRIM_P2VAR_PARA(actU32) tgt,
  VSECPRIM_P2CONST_PARA(actU8) src,
  const actU32 length);

/**********************************************************************************************************************
 *  actCpyU32toBE()
 *********************************************************************************************************************/
/*! \brief       Copy u32 to u8 big endian array
 *  \details     This function copies u32 array into u8 array in big endian format.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  Pointer to source array
 *  \param[in]   length  Size of tgt/src array in bytes
 *  \pre         Pointers must be valid.
 *               Array sizes must be correct.
 *               elements of tgt = 4 * elements of src
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actCpyU32toBE(
  VSECPRIM_P2VAR_PARA(actU8) tgt,
  VSECPRIM_P2CONST_PARA(actU32) src,
  const actU32 length);

/**********************************************************************************************************************
 *  actCpyU32toLE()
 *********************************************************************************************************************/
/*! \brief       Copy u32 to u8 little endian array
 *  \details     This function copies u32 array into u8 array in little endian format.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  Pointer to source array
 *  \param[in]   length  Size of tgt/src array in bytes
 *  \pre         Pointers must be valid.
 *               Array sizes must be correct.
 *               elements of tgt = 4 * elements of src
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actCpyU32toLE(
  VSECPRIM_P2VAR_PARA(actU8) tgt,
  VSECPRIM_P2CONST_PARA(actU32) src,
  const actU32 length);

# ifdef ACT_PLATFORM_UINT64_AVAILABLE
/**********************************************************************************************************************
 *  actBEtoU64()
 *********************************************************************************************************************/
/*! \brief       Copy u8 big endian array into u64 value
 *  \details     This function copies u64 value into u8 array in big endian format.
 *  \param[in]   src  Pointer to u8 array
 *  \return      u64 value
 *  \pre         Pointers must be valid.
 *               src array must be 8 bytes long.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(actU64) actBEtoU64 (VSECPRIM_P2CONST_PARA(actU8) src);

/**********************************************************************************************************************
 *  actU64toBE()
 *********************************************************************************************************************/
/*! \brief       Copy u64 value to u8 big endian array
 *  \details     This function copies u64 value into u8 array in big endian format.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  u64 value input
 *  \pre         Pointers must be valid.
 *               tgt array must be 8 bytes long.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actU64toBE(VSECPRIM_P2VAR_PARA(actU8) tgt, const actU64 src);

/**********************************************************************************************************************
 *  actCpyBEtoU64()
 *********************************************************************************************************************/
/*! \brief       Copy u8 big endian array to u64 array
 *  \details     This function copies u8 array in big endian format into u64 array.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  Pointer to source array
 *  \param[in]   length  Size of tgt/src array in bytes
 *  \pre         Pointers must be valid.
 *               Array sizes must be correct.
 *               elements of src * 8 = elements of tgt
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actCpyBEtoU64(
  VSECPRIM_P2VAR_PARA(actU64) tgt,
  VSECPRIM_P2CONST_PARA(actU8) src,
  const actU32 length);

/**********************************************************************************************************************
 *  actCpyU64toBE()
 *********************************************************************************************************************/
/*! \brief       Copy u64 to u8 big endian array
 *  \details     This function copies u64 array into u8 array in big endian format.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  Pointer to source array
 *  \param[in]   length  Size of tgt/src array in bytes
 *  \pre         Pointers must be valid.
 *               Array sizes must be correct.
 *               elements of tgt = 8 * elements of src
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actCpyU64toBE(
  VSECPRIM_P2VAR_PARA(actU8) tgt,
  VSECPRIM_P2CONST_PARA(actU64) src,
  const actU32 length);

/**********************************************************************************************************************
 *  actLEtoU64()
 *********************************************************************************************************************/
/*! \brief       Copy u8 little endian array into u64 value
 *  \details     This function copies u64 value into u8 array in little endian format.
 *  \param[in]   src  Pointer to u8 array
 *  \return      u64 value
 *  \pre         Pointers must be valid.
 *               src array must be 8 bytes long.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(actU64) actLEtoU64(VSECPRIM_P2CONST_PARA(actU8) src);

/**********************************************************************************************************************
 *  actU64toLE()
 *********************************************************************************************************************/
/*! \brief       Copy u64 value to u8 little endian array
 *  \details     This function copies u64 value into u8 array in little endian format.
 *  \param[out]  tgt  Pointer to target array
 *  \param[in]   src  u64 value input
 *  \pre         Pointers must be valid.
 *               tgt array must be 8 bytes long.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_INLINE_FUNC(void) actU64toLE(VSECPRIM_P2VAR_PARA(actU8) tgt, const actU64 src);
# endif

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}                               /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTUTILITIES_H */

/**********************************************************************************************************************
 *  END OF FILE: actUtilities.h
 *********************************************************************************************************************/
