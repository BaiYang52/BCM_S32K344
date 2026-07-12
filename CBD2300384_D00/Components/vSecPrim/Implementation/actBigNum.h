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
/*!        \file  actBigNum.h
 *        \brief  Basic integer and module arithmetic used for elliptic curve point arithmetic.
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

#ifndef ACTBIGNUM_H
# define ACTBIGNUM_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

# include "actITypes.h"
# include "actConfig.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/

/* DDIGIT arithmetic constant (0x00010000 for actBN_BITS_PER_DIGIT==16) */
# define actBN_BASE                                                   ((actBNDDIGIT)(1) << actBN_BITS_PER_DIGIT)
/* DIGIT arithmetic constant (0xFFFF for actBN_BITS_PER_DIGIT==16) */
# define actBN_MAX_DIGIT                                              ((actBNDIGIT)(actBN_BASE - 1))
/* DIGIT arithmetic constant (0x8000 for actBN_BITS_PER_DIGIT==16) */
# define actBN_MAX_BIT                                                ((actBNDDIGIT)(1u) << (actBN_BITS_PER_DIGIT-1u))
/* DDIGIT arithmetic constant (0xFFFF0000 for actBN_BITS_PER_DIGIT==16) */
# define actBN_HIGH_MAX                                               ((actBNDDIGIT)(actBN_MAX_DIGIT) << actBN_BITS_PER_DIGIT)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actBNSetZero()
 *********************************************************************************************************************/
/*! \brief       Sets a BigNum to zero.
 *  \details     This function sets a given BigNum number to zero.
 *  \param[in,out]   a  The input number
 *  \param[in]   length  Length of a
 *  \pre         a must be a valid pointer
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
# define actBNSetZero(a, length)                                      actBNSet ((a), (length), ((actBNDIGIT) 0))

/**********************************************************************************************************************
 *  actBNSetOne()
 *********************************************************************************************************************/
/*! \brief       Sets a BigNum to ONE(1).
 *  \details     This function sets a given BigNum number to one.
 *  \param[in,out]   a  The input number
 *  \param[in]   length  Length of a
 *  \pre         a must be a valid pointer
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
# define actBNSetOne(a, length)                                       actBNSet ((a), (length), ((actBNDIGIT) 1))

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

/****************************************************************************
 ** Macros
 ***************************************************************************/

/* calculate digit length from byte length */
# define actBNGetDigitLength(byte_len)                                \
                                                                      (((byte_len)+actBN_BYTES_PER_DIGIT-1u)/actBN_BYTES_PER_DIGIT)

/* test a bit of a digit */
# define actBNGetBitOfDigit(digit, i)                                 \
                                                                      ( (((digit) & ((actBNDIGIT)(1u)<<(i))) != (actBNDIGIT)0u)  ? TRUE:FALSE)

/* test a bit of a BigNum */
# define actBNGetBit(a, i)                                            \
                                                                      (actBNGetBitOfDigit((a)[(i)/actBN_BITS_PER_DIGIT], ((i)%actBN_BITS_PER_DIGIT)))
# define actBNIsOdd(a)                                                ( ((a[0] & (actBNDIGIT)1u) == ((actBNDIGIT)1u)) ? TRUE:FALSE)
# define actBNIsEven(a)                                               ( ((a[0] & (actBNDIGIT)1u) != ((actBNDIGIT)1u)) ? TRUE:FALSE)

/* Get the lower word of a ddigit */
# define actBNLoWord(ddigit)                                          ((actBNDIGIT)(ddigit))

/* Get the higher word of a dword */
# define actBNHiWord(ddigit)                                          ((actBNDIGIT)((ddigit) >> actBN_BITS_PER_DIGIT))

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
 *  actBNLoadLE()
 *********************************************************************************************************************/
/*! \brief       Byte array to actBIGNUM.
 *  \details     This function loads little endian byte string into little endian actBIGNUM
 *  \param[in,out]  bigNum  Pointer to actBIGNUM variable.
 *  \param[in]  bytes  Pointer to byte array.
 *  \param[in]  byteLength  Number of elements in provided byte array.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \pre         byteLength must be a multiple of actBN_BYTES_PER_DIGIT.
 *               provided pointers must be valid
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actBNLoadLE(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) bigNum,
  VSECPRIM_P2CONST_PARA(actU8) bytes,
  const actU32 byteLength);

/**********************************************************************************************************************
 *  actBNStoreLE()
 *********************************************************************************************************************/
/*! \brief       actBIGNUM to byte array.
 *  \details     This function stores little endian actBIGNUM into little endian byte string.
 *  \param[in,out]  bytes  Pointer to byte array
 *  \param[in]  bigNum  Pointer to bigNum
 *  \param[in]  bnLength  Length of bigNum
 *  \pre         Byte array must have size (bnLength * actBN_BYTES_PER_DIGIT)
 *               provided pointers must be valid
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
extern VSECPRIM_FUNC(void) actBNStoreLE(
  VSECPRIM_P2VAR_PARA(actU8) bytes,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) bigNum,
  const actLengthType bnLength);

/**********************************************************************************************************************
 *  actBNCopy()
 *********************************************************************************************************************/
/*! \brief       Copy b in a.
 *  \details     This function copies the BigNum b into a.
 *  \param[in,out]  a  The destination number
 *  \param[in]  b  The source number
 *  \param[in]  length  Length of a and b
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \pre         a, b must have at least size of length
 *               provided pointers must be valid
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNCopy(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  const actLengthType length);

/**********************************************************************************************************************
 *  actBNGetBitLength()
 *********************************************************************************************************************/
/*! \brief       Provide length in bits.
 *  \details     This function calculates exact length in bits of a BigNum.
 *  \param[in]   a  The number to be evaluated
 *  \param[in]   length  Length of a
 *  \return      length in bits
 *  \pre         a must be a valid pointer
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actLengthType) actBNGetBitLength(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  const actLengthType length);

/**********************************************************************************************************************
 *  actBNSetBit()
 *********************************************************************************************************************/
/*! \brief       Sets a bit of a BigNum.
 *  \details     This function sets a bit in a BigNum number.
 *  \param[in,out]   a  The input number
 *  \param[in]   length  Length of a
 *  \param[in]   bit_pos  Position of the bit to set
 *  \return      actOK  -  Success
 *               actEXCEPTION_UNKNOWN  -  Bit position out of range
 *  \pre         a must be at least the size of 'length'.
 *               a must be a valid pointer.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actBNSetBit(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  const actLengthType length,
  const actLengthType bit_pos);

/**********************************************************************************************************************
 *  actBNSet()
 *********************************************************************************************************************/
/*! \brief       Sets a BigNum to the given value.
 *  \details     Set least significant digit of bignum "value". Set all other digits to 0u.
 *  \param[in,out]   a  The input number
 *  \param[in]   length  Length of a
 *  \param[in]   value   The value which shall be set.
 *  \pre         a must be a valid pointer
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNSet(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  const actLengthType  length,
  const actBNDIGIT   value);

/**********************************************************************************************************************
 *  actBNIsZero()
 *********************************************************************************************************************/
/*! \brief       Compares a BigNum against zero.
 *  \details     This function compares the given BigNum against zero.
 *  \param[in]   a  The input number
 *  \param[in]   length  Length of a
 *  \return      TRUE  -  BigNum is zero
 *               FALSE  -  BigNum is not zero
 *  \pre         a must be a valid pointer
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actU8) actBNIsZero(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  const actLengthType length);

/**********************************************************************************************************************
 *  actBNCompare()
 *********************************************************************************************************************/
/*! \brief       Compares two BigNums.
 *  \details     This function compares the given BigNum a against b.
 *  \param[in]   a  First number
 *  \param[in]   b  Second number
 *  \param[in]   length  Length of a and b
 *  \return      actCOMPARE_SMALLER  -  a < b
 *               actCOMPARE_EQUAL  -  a == b
 *               actCOMPARE_LARGER  -  a > b
 *  \pre         a, b must be valid pointers
 *               a, b must be at least of size length
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actBNCompare(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  const actLengthType length);

/**********************************************************************************************************************
 *  actBNSetOctetString()
 *********************************************************************************************************************/
/*! \brief       Big endian byte array to BigNum.
 *  \details     This function converts big endian byte array to BigNum according to IEEE 1363 OS2FEP
 *               (octet string to field element presentation).
 *  \param[in,out]  a  Destination number
 *  \param[in]   length  Length of a
 *  \param[in]   in  Source octet string
 *  \param[in]   in_len  The length in bytes of the octet string
 *  \pre         a must have at least size of 'length'. a must be a valid pointer.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNSetOctetString(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  actLengthType length,
  VSECPRIM_P2CONST_PARA(actU8) in,
  const actLengthType in_len);

/**********************************************************************************************************************
 *  actBNSetOctetStringROM()
 *********************************************************************************************************************/
/*! \brief       Big endian byte array to BigNum in ROM.
 *  \details     This function converts big endian byte array to BigNum in ROM according to IEEE 1363 OS2FEP
 *               (octet string to field element presentation).
 *  \param[in,out]  a  Destination number
 *  \param[in]   length  Length of a
 *  \param[in]   in  Source octet string
 *  \param[in]   in_len  The length in bytes of the octet string
 *  \pre         a must have at least size of 'length'. a must be a valid pointer.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNSetOctetStringROM(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  actLengthType length,
  VSECPRIM_P2CONST_PARA(actU8) in,
  const actLengthType in_len);

/**********************************************************************************************************************
 *  actBNOctetString()
 *********************************************************************************************************************/
/*! \brief       BigNum to big endian byte array.
 *  \details     This function converts BigNum into big endian byte array according to IEEE 1363 OS2FEP
 *               (octet string to field element presentation).
 *  \param[in,out]  out  Pointer to byte array
 *  \param[in]   out_len  Size of byte array
 *  \param[in]   a  BigNum source
 *  \param[in]   length  The length in bytes of the BigNum
 *  \pre         provided pointers must be valid
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNOctetString(
  VSECPRIM_P2VAR_PARA(actU8) out,
  actLengthType out_len,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  const actLengthType length);

/**********************************************************************************************************************
 *  actBNAdd()
 *********************************************************************************************************************/
/*! \brief       BigNum Addition (c = a+b)
 *  \details     This function adds two BigNum numbers a and b to provide c.
 *  \param[in]   a  First number
 *  \param[in]   b  Second number
 *  \param[in,out]  c (in)  Buffer to store the result of the addition
 *                    (out) result of the addition
 *  \param[in]   length  Length of a, b and c
 *  \return      The last carry (0 or 1)
 *  \pre         c must have at least size of 'length'.
 *               all provided pointers must be valid
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        c may overlap with a or b (a+=b or b+=a is possible)
 *********************************************************************************************************************/
VSECPRIM_FUNC(actU8) actBNAdd(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) c,
  const actLengthType length);

/**********************************************************************************************************************
 *  actBNSub()
 *********************************************************************************************************************/
/*! \brief       BigNum Subtraction (c = a-b for a>=b)
 *  \details     This function subtracts b from a.
 *  \param[in]   a  First number
 *  \param[in]   b  Second number
 *  \param[in,out]  c (in) Buffer to store the result of the subtraction
 *                    (out) result of the subtraction
 *  \param[in]   length  Length of a, b and c
 *  \pre         all provided pointers must be valid
 *               a, b, and c must provide arrays of 'length' elements
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        c may overlap with a or b (a-=b or b-=a is possible)
 *               a>=b means the last carry is always zero, thus actBNSub(..) is of type void
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNSub(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) c,
  const actLengthType length);

/**********************************************************************************************************************
 *  actBNSubDigit()
 *********************************************************************************************************************/
/*! \brief      Subtract a single digit from a long number c = a - b, if a >= b.
 *  \details     -
 *  \param[in]   a  First number, 'length' many digits long
 *  \param[in]   b  Second number, 1 digit long
 *  \param[in,out]  c (in) Buffer to store the result of the subtraction
 *                    (out) result of the subtraction
 *  \param[in]   length  Length of a and c
 *  \pre         all provided pointers must be valid
 *               a, and c must provide arrays of 'length' elements
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        c may overlap with a (a -= b is possible)
 *               a>=b means the last carry is always zero, thus actBNSub(..) is of type void
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNSubDigit(
  VSECPRIM_P2CONST_PARA(actBNDIGIT)  a,
  const actBNDIGIT b,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) c,
  const actBNLENGTH length);

/**********************************************************************************************************************
 *  actBNMult()
 *********************************************************************************************************************/
/*! \brief       BigNum Multiply (c = a*b).
 *  \details     This function multiplies two BigNums.
 *  \param[in]   a  First number
 *  \param[in]   a_length  Length of BigNum a
 *  \param[in]   b  Second number
 *  \param[in]   b_length  Length of BigNum b
 *  \param[in,out]  c (in)  Buffer to store the result of the multiplication
 *                    (out) result of the multiplication
 *  \param[in]   watchdog  Watchdog trigger function pointer
 *  \pre         All provided pointers must be valid.
 *               The buffer referenced by c must provide at least a_length + b_length many actBNDIGITs.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        c cannot overlap with a or b (a*=b or b*=a is not possible)
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNMult(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  const actLengthType a_length,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  const actLengthType b_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) c,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actBNDiv2()
 *********************************************************************************************************************/
/*! \brief       Division by 2 (a /= 2).
 *  \details     This function performs the BigNum division by 2. A possible decimal in case a is odd is neglected.
 *  \param[in,out]   a (in)  BigNum which shall be divided by 2
 *                     (out) BigNum a divided by 2
 *  \param[in]   first_carry  leading bit of a
 *  \param[in]   length  Length of a in BigNums
 *  \pre         a must be a valid pointer referencing a buffer of length elements
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNDiv2(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  actU8 first_carry,
  actLengthType length);

/**********************************************************************************************************************
 *  actBNReduce()
 *********************************************************************************************************************/
/*! \brief       BigNum reduction (r = a mod b).
 *  \details     This function performs the reduction of a BigNum by r = a mod b.
 *  \param[in,out]  a  (in)  First number
 *                     (out) the normalized result a mod b
 *  \param[in]   a_length  Length of a
 *  \param[in,out]  b  (in) Second number
 *                     (out) second number (will be returned const but touched by the normalization)
 *  \param[in]   b_length  Length of b
 *  \param[in,out]  r (in)  Result buffer
 *                    (out) the result a mod b
 *  \param[in]   watchdog  Watchdog trigger function pointer
 *  \return      actEXCEPTION_LENGTH - invalid length, b is too short
 *               actOK - success
 *  \pre         provided pointers must be valid
 *               a must provide a buffer of a_length + 1 elements
 *               b must provide a buffer of b_length elements
 *               r must provide a buffer of b_length + 1 elements
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        a, b and r cannot overlap
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actBNReduce(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  actLengthType a_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) b,
  actLengthType b_length,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actBNModAdd()
 *********************************************************************************************************************/
/*! \brief       BigNum modular addition (c = (a + b) mod m, m = Ring->m).
 *  \details     This function performs the BigNum modular addition within given ring.
 *  \param[in]   a  First ring element
 *  \param[in]   b  Second ring element
 *  \param[in,out]   c  (in)  Ring element to store the result
 *                      (out) result of the modular addition
 *  \param[in]   Ring  Pointer to underlying ring
 *  \param[in]   watchdog  Watchdog trigger function pointer
 *  \pre         Provided pointers must be valid
 *               Provided buffers must be of size Ring->m_length
 *               a + b must be < 2m, otherwise the result c is not necessarily < m. (This is why a, b, and c are also
 *               referred to as 'ring elements'.) This is not checked within the function but during review for every
 *               call to actBNModAdd().
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        The actBNRING structure parameter 'Field' holds all necessary information. It has to be initialized
 *               as far as listed above.
 *               c may overlap with a or b (a+=b or b+=a is possible).
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNModAdd(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) c,
  VSECPRIM_P2CONST_PARA(actBNRING) Ring,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actBNModSub()
 *********************************************************************************************************************/
/*! \brief       BigNum modular subtraction (c = (a - b) mod m, m = Ring->m).
 *  \details     This function performs the BigNum modular subtraction.
 *  \param[in]   a  First ring element
 *  \param[in]   b  Second ring element
 *  \param[in,out]   c (in) Ring element to store the result
 *                     (out) subtraction result
 *  \param[in]   Ring  Pointer to underlying ring
 *  \param[in]   watchdog  Watchdog trigger function pointer
 *  \pre         Provided pointers must be valid
 *               Provided buffers must be of size Ring->m_length
 *               In order for the result c to fulfill c < m, a and b should be < m (sufficient condition).
 *               (This is why a, b, and c are also referred to as 'ring elements'.) This is not checked within the
 *               function but during review for every call to actBNModSub().
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        The actBNRING structure parameter 'Field' holds all necessary information. It has to be initialized
 *               as far as listed above.
 *               c may overlap with a or b (a-=b or b-=a is possible).
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNModSub(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) c,
  VSECPRIM_P2CONST_PARA(actBNRING) Ring,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actBNModDiv2()
 *********************************************************************************************************************/
/*! \brief       BigNum modular division by 2 (a /= 2 mod m, m=Ring->m).
 *  \details     This function performs the BigNum modular division.
 *               It computes (a / 2) mod m, where m=Ring->m, for a in [0,2*(m-1)], if a is even,
 *               and a in [0,m-2], if a is odd.
 *               Otherwise, a is still divided by 2, but the result is not necessarily < m.
 *  \param[in,out]  a (in)  Ring element to be divided
 *                    (out) result of the modular division
 *  \param[in]   Ring  The underlying ring
 *  \param[in]   watchdog  Watchdog trigger function pointer
 *  \pre         Provided pointers must be valid
 *               Provided buffers must be of size Ring->m_length
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        The actBNRING structure parameter 'Ring' holds all necessary information. It has to be initialized
 *               as far as listed above.
 *               If a is not less than m, a is still divided by 2, but the result is not necessarily < m.
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNModDiv2(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNRING) Ring,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actBNMontMul()
 *********************************************************************************************************************/
/*! \brief       BigNum Montgomery multiplication (A = (x * y * R^-1) mod m, m = Ring->m is odd).
 *  \details     This function performs the BigNum Montgomery multiplication according to MOV'97, algo 14.36.
 *               (MOV '97: Menezes, van Oorschot, Vanstone - Handbook of Applied Cryptography)
 *               That is, this function combines the multiplication of x and y with the Montgomery reduction, i.e.,
 *               multiplying the product by R^(-1) and reducing modulo m.
 *               R is given as R = 2^(number of digits * bits per digit) (digit = actBNDIGIT)
 *  \param[in]     x         First factor of the desired product
 *  \param[in]     y         Second factor of the desired product
 *  \param[in,out] A   (in)  BigNum buffer to store the result (x*y*R^-1) mod m
 *                     (out) result (x*y*R^-1) mod m
 *  \param[in]     Ring      Pointer to the underlying ring
 *  \param[in]     watchdog  Watchdog trigger function pointer
 *  \pre         Ring->m must be odd
 *               Provided pointers must be valid
 *               Provided buffers must be of the following sizes:
 *               x must be of size Ring->m_length
 *               y must be of size Ring->m_length
 *               A must be of size Ring->m_length + 1
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        The actBNRING structure parameter 'Field' holds all necessary information. Ring->m, Ring->m_length,
 *               Ring->m_bar and Ring->prime_structure (only for actBN_MONT_MUL_VERSION 1) have to be initialized.
 *               For actBN_MONT_MUL_VERSION 1:
 *               if m is the curve prime
 *                   Ring->prime_structure==curveid
 *               else
 *                   Ring->prime_structure==0
 *               A cannot overlap with x or y (x*=y or y*=x is not possible)
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNMontMul(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) x,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) y,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) A,
  VSECPRIM_P2CONST_PARA(actBNRING) Ring,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actBNMontMulCopy()
 *********************************************************************************************************************/
/*! \brief       BigNum Montgomery multiplication (see actBNMontMul, the result is copied to x).
 *  \details     This function performs the BigNum Montgomery multiplication with result copied to x.
 *  \param[in,out]   x (in)  First ring element, ring element to store the result
 *                     (out) result (x*y*R^-1) mod m
 *  \param[in]   y  Second ring element
 *  \param[in,out]   A (in)  Ring element to store the result
 *                     (out) result (x*y*R^-1) mod m
 *  \param[in]   Ring  Pointer to the underlying ring
 *  \param[in]   watchdog  Watchdog trigger function pointer
 *  \pre         Ring->m must be odd.
 *               Provided pointers must be valid.
 *               Provided buffers x and y must be of size Ring->m_length.
 *               Provided buffer A must be of size (Ring->m_length + 1).
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        The actBNRING structure parameter 'Field' holds all necessary information. It has to be initialized.
 *               A cannot overlap with x or y (x *= y or y *= x is not possible)
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNMontMulCopy(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) x,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) y,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) A,
  VSECPRIM_P2CONST_PARA(actBNRING) Ring,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actBNModExp()
 *********************************************************************************************************************/
/*! \brief       BigNum modular exponentiation (AR = gR^e mod m, m = Ring->m).
 *  \details     This function performs BigNum modular exponentiation. The in- and output is in Montgomery
 *               representation.
 *  \param[in]   gR  Pointer to basis which shall be exponentiated
 *  \param[in]   e  Pointer to exponent (e > 0)
 *  \param[in,out]  AR (in)  Ring element to store the result
 *                     (out) resukt of the modular exponentiation
 *  \param[in]   Ring  Pointer to the underlying ring, providing the modulus m
 *  \param[in,out]   t (in)  Temporary BigNum variables
 *                     (out) Temporary BigNum variables
 *  \param[in]   watchdog  Watchdog trigger function pointer
 *  \pre         Ring->m must be odd.
 *               e > 0
 *               Provided pointers must be valid
 *               Provided buffers AR, t[0] must be of size Ring->m_length + 1
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        The actBNRING structure parameter 'Ring' holds all necessary information. It has to be initialized
 *               as far as listed above.
 *               Ring->m, Ring->m_length, Ring->RR, Ring->m_bar have to be initialized.
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNModExp(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) gR,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) e,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) AR,
  VSECPRIM_P2CONST_PARA(actBNRING) Ring,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) t[],
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actBNModRandomize()
 *********************************************************************************************************************/
/*! \brief       BigNum modular randomization.
 *  \details     This function will provide a BigNum modular random value.
 *               x will randomized with 0 < x < m, m = Ring->m.
 *  \param[in,out]  x         (in)  Pointer to the ring element to store the result
 *                            (out) result
 *  \param[in]      Ring      Pointer to the underlying ring
 *  \param[in,out]  t         (in)  Pointer to two temporary BigNum values
 *                            (out) Pointer to two temporary BigNum values
 *  \param[in]      watchdog  Watchdog trigger function pointer
 *  \return      actOK - success
 *               actEXCEPTION_UNKNOWN - else
 *  \pre         Ring->m must be odd
 *               Provided pointers must be valid
 *               Provided buffers must be of size Ring->m_length
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        The actBNRING structure parameter 'Ring' holds all necessary information. Ring->m , Ring->m_length,
 *               Ring->RR, Ring->m_bar and Ring->prime_structure have to be initialized.
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actBNModRandomize(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) x,
  VSECPRIM_P2CONST_PARA(actBNRING) Ring,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) t[],
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actBNFieldInversion()
 *********************************************************************************************************************/
/*! \brief       BigNum field inversion.
 *  \details     This function performs BigNum field inversion g_invR = gR^-1 mod p, p=Ring->m must be prime.
 *               The in- and output is in Montgomery Representation.
 *  \param[in]       gR        Pointer to field element to be inverted (gR != 0)
 *  \param[in,out]   g_invR    (in)  Pointer to the field element to store the result
 *                             (out) result of the field inversion
 *  \param[in]       Field     Pointer to the underlying field GF(p)
 *  \param[in,out]   t (in)    Pointer to temporary BigNum variables
 *                     (out)   Pointer to two temporary BigNum values
 *  \param[in]       watchdog  Watchdog trigger function pointer
 *  \pre         Field->m must be prime
 *               gR != 0
 *               Provided pointers must be valid
 *               Provided buffers must be of the following sizes:
 *               g_invR must be of size Field->m_length + 1
 *               t must be of size 5*(Field->m_length + 1), if 'actEUCLID' is defined,
 *               and of size (2^(k-1)+1)*(Field->m_length + 1) otherwise
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *  \note        The actBNRING structure parameter 'Field' holds all necessary information. Field->m, Field->m_length,
 *               Field->RR, Field->m_bar and Field->prime_structure have to be initialized.
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNFieldInversion(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) gR,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) g_invR,
  VSECPRIM_P2CONST_PARA(actBNRING) Field,
  VSECPRIM_P2VAR_PARA(actBNDIGIT) t[],
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actBNOddInvModBase()
 *********************************************************************************************************************/
/*! \brief       BigNum modular inversion of an odd digit.
 *  \details     Compute the inverse of 'value' modulo 2^(bits per digit).
 *               This function is only called once with a single digit 'value' and a single digit 'inverse'
 *               to calculate the 'm_bar'-member of actBNRING
 *  \param[in,out] inverse (in)  Pointer to BigNum output
 *                         (out) output
 *  \param[in]   value  Odd digit to be inverted
 *  \pre         Pointer 'inverse' must be valid
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(void) actBNOddInvModBase(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) inverse,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) value);

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTBIGNUM_H */

/**********************************************************************************************************************
 *  END OF FILE: actBigNum.h
 *********************************************************************************************************************/
