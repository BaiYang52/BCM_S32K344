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
/*!        \file  actSHA2_64.c
 *        \brief  SHA-384 and SHA-512 implementation.
 *
 *      \details Supports SHA-512/224 and SHA-512/256 as well.
 *               This file is part of the embedded systems library cvActLib
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
#define ACTSHA2_64_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actISHA2_64.h"
#include "actUtilities.h"
#include "actWatchdog.h"
#include "actConfig.h"

#if (VSECPRIM_ACTSHA2_64_ENABLED == STD_ON)

/****************************************************************************
 ** Types and constants
 ***************************************************************************/
# define actSHA512_WORD_SIZE                                          (8u)

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/* The SHA2_64 functions */
# if defined(ACT_PLATFORM_UINT64_AVAILABLE)
#  define S0(x)                                                       (actRotr64(x, 28u)^actRotr64(x, 34u)^actRotr64(x, 39u))
#  define S1(x)                                                       (actRotr64(x, 14u)^actRotr64(x, 18u)^actRotr64(x, 41u))
#  define s0(x)                                                       (actRotr64(x, 1u)^actRotr64(x, 8u)^((x)>>7u))
#  define s1(x)                                                       (actRotr64(x, 19u)^actRotr64(x, 61u)^((x)>>6u))

/* basic macros */
#  define blk0(i)                                                     (W[(i)])
#  define blk2(i)                                                     (W[(i)&15u]+=s1(W[((i)-2u)&15u])+W[((i)-7u)&15u]+s0(W[((i)-15u)&15u]))

#  define Ch(x, y, z)                                                 ((z)^((x)&((y)^(z))))
#  define Maj(x, y, z)                                                (((x)&(y))|((z)&((x)|(y))))

#  define a(i)                                                        (T[(0u-(i)) & 7u])
#  define b(i)                                                        (T[(1u-(i)) & 7u])
#  define c(i)                                                        (T[(2u-(i)) & 7u])
#  define d(i)                                                        (T[(3u-(i)) & 7u])
#  define e(i)                                                        (T[(4u-(i)) & 7u])
#  define f(i)                                                        (T[(5u-(i)) & 7u])
#  define g(i)                                                        (T[(6u-(i)) & 7u])
#  define h(i)                                                        (T[(7u-(i)) & 7u])

#  define Ra(i, j)                                                    if (j > 0u) {blk2(i);} \
                                                                      (h(i) += S1(e(i)) + Ch(e(i), f(i), g(i)) + ((actU64)(actSHA2_64_K[(i) + (j)]) + ((j > 0u) ? W[(i)&15u] : blk0(i))))
#  define Ra0(i, j)                                                   (h(i) += S1(e(i)) + Ch(e(i), f(i), g(i)) + ((actU64)(actSHA2_64_K[(i) + (j)]) +  blk0(i)))
#  define Ra2(i, j)                                                   blk2(i); \
                                                                      (h(i) += S1(e(i)) + Ch(e(i), f(i), g(i)) + ((actU64)(actSHA2_64_K[(i) + (j)]) +  W[(i)&15u]))
#  define Rb(i)                                                       (d(i) += h(i))
#  define Rc(i)                                                       (h(i) += S0(a(i)) + Maj(a(i), b(i), c(i)))
# else
#  define clr_64(tgt)                                                 do { \
                                                                      (tgt)[0] =       \
                                                                      (tgt)[1] = 0;     \
                                                                      } while (0)

#  define cpy_64(tgt, src)                                            do { \
                                                                      (tgt)[0] = (src)[0];   \
                                                                      (tgt)[1] = (src)[1];   \
                                                                      } while (0)

#  define add_64(sum, add)                                            do { \
                                                                      if (((sum)[1] + (add)[1]) < (sum)[1]) \
                                                                      /* carry from LSW */ \
                                                                      (sum)[0]++;       \
                                                                      (sum)[1] += (add)[1];   \
                                                                      (sum)[0] += (add)[0];   \
                                                                      } while (0)

#  define xor_64(tgt, val)                                            do { \
                                                                      (tgt)[0] ^= (val)[0];   \
                                                                      (tgt)[1] ^= (val)[1];   \
                                                                      } while (0)

#  define shR_64(tgt, x, n)                                           do { \
                                                                      (tgt)[0] =  ((x)[0] >> (n)); \
                                                                      (tgt)[1] = (((x)[1] >> (n)) | ((x)[0] << (32 - (n)))); \
                                                                      } while (0)

#  define shL_64(tgt, x, n)                                           do { \
                                                                      (tgt)[0] = (((x)[0] << (n)) | ((x)[1] >> (32 - (n)))); \
                                                                      (tgt)[1] =  ((x)[1] << (n)); \
                                                                      } while (0)

#  define rotR_64(tgt, x, n)                                          do { \
                                                                      (tgt)[0] = (((x)[0] >> (n)) | ((x)[1] << (32 - (n)))); \
                                                                      (tgt)[1] = (((x)[1] >> (n)) | ((x)[0] << (32 - (n)))); \
                                                                      } while (0)

#  define rotL_64(tgt, x, n)                                          do { \
                                                                      (tgt)[0] = (((x)[0] << (n)) | ((x)[1] >> (32 - (n)))); \
                                                                      (tgt)[1] = (((x)[1] << (n)) | ((x)[0] >> (32 - (n)))); \
                                                                      } while (0)

#  define xor_rotR(tgt, x, n)                                         do { \
                                                                      (tgt)[0] ^= (((x)[0] >> (n)) | ((x)[1] << (32 - (n)))); \
                                                                      (tgt)[1] ^= (((x)[1] >> (n)) | ((x)[0] << (32 - (n)))); \
                                                                      } while (0)

#  define xor_rotL(tgt, x, n)                                         do { \
                                                                      (tgt)[0] ^= (((x)[0] << (n)) | ((x)[1] >> (32 - (n)))); \
                                                                      (tgt)[1] ^= (((x)[1] << (n)) | ((x)[0] >> (32 - (n)))); \
                                                                      } while (0)

/* SHA-512 functions */
/* add the calculated value to tgt */
#  define add_Ch(tgt, x, y, z)                                        do { \
                                                                      actU32 tmp [2];         \
                                                                      tmp[0] = ((z)[0] ^ ((x)[0] & ((y)[0] ^ (z)[0]))); \
                                                                      tmp[1] = ((z)[1] ^ ((x)[1] & ((y)[1] ^ (z)[1]))); \
                                                                      add_64 ((tgt), tmp);     \
                                                                      } while (0)

#  define add_Maj(tgt, x, y, z)                                       do { \
                                                                      actU32 tmp [2];         \
                                                                      tmp[0] = (((x)[0] & (y)[0]) | ((z)[0] & ((x)[0] | (y)[0]))); \
                                                                      tmp[1] = (((x)[1] & (y)[1]) | ((z)[1] & ((x)[1] | (y)[1]))); \
                                                                      add_64 ((tgt), tmp);     \
                                                                      } while (0)

#  define add_S0(tgt, x)                                              do { \
                                                                      actU32 tmp [2];       \
                                                                      rotR_64  (tmp, (x), 28); \
                                                                      xor_rotL (tmp, (x), 30); /* rotR (34) */ \
                                                                      xor_rotL (tmp, (x), 25); /* rotR (39) */ \
                                                                      add_64   ((tgt), tmp);   \
                                                                      } while (0)

#  define add_S1(tgt, x)                                              do { \
                                                                      actU32 tmp [2];       \
                                                                      rotR_64  (tmp, (x), 14); \
                                                                      xor_rotR (tmp, (x), 18); \
                                                                      xor_rotL (tmp, (x), 23); /* rotR (41) */ \
                                                                      add_64   ((tgt), tmp);   \
                                                                      } while (0)

#  define add_s0(tgt, x)                                              do { \
                                                                      actU32 tmp [2];       \
                                                                      shR_64   (tmp, (x),  7); \
                                                                      xor_rotR (tmp, (x),  1); \
                                                                      xor_rotR (tmp, (x),  8); \
                                                                      add_64   ((tgt), tmp);   \
                                                                      } while (0)

#  define add_s1(tgt, x)                                              do { \
                                                                      actU32 tmp [2];       \
                                                                      shR_64   (tmp, (x),  6); \
                                                                      xor_rotR (tmp, (x), 19); \
                                                                      xor_rotL (tmp, (x),  3); /* rotR (61) */ \
                                                                      add_64   ((tgt), tmp);   \
                                                                      } while (0)
# endif

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# if defined(ACT_PLATFORM_UINT64_AVAILABLE)
VSECPRIM_ROM(VSECPRIM_LOCAL, actU64) actSHA2_64_K[80] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */
{
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL, 0xe9b5dba58189dbbcULL,
    0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL, 0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL,
    0xd807aa98a3030242ULL, 0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL, 0xc19bf174cf692694ULL,
    0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL, 0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL,
    0x2de92c6f592b0275ULL, 0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL, 0xbf597fc7beef0ee4ULL,
    0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL, 0x06ca6351e003826fULL, 0x142929670a0e6e70ULL,
    0x27b70a8546d22ffcULL, 0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL, 0x92722c851482353bULL,
    0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL, 0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL,
    0xd192e819d6ef5218ULL, 0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL, 0x34b0bcb5e19b48a8ULL,
    0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL, 0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL,
    0x748f82ee5defb2fcULL, 0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL, 0xc67178f2e372532bULL,
    0xca273eceea26619cULL, 0xd186b8c721c0c207ULL, 0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL,
    0x06f067aa72176fbaULL, 0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL, 0x431d67c49c100d4cULL,
    0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL, 0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};
# else
VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) actSHA2_64_K[160] =  /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */
{
    0x428a2f98UL, 0xd728ae22UL, 0x71374491UL, 0x23ef65cdUL, 0xb5c0fbcfUL, 0xec4d3b2fUL, 0xe9b5dba5UL, 0x8189dbbcUL,
    0x3956c25bUL, 0xf348b538UL, 0x59f111f1UL, 0xb605d019UL, 0x923f82a4UL, 0xaf194f9bUL, 0xab1c5ed5UL, 0xda6d8118UL,
    0xd807aa98UL, 0xa3030242UL, 0x12835b01UL, 0x45706fbeUL, 0x243185beUL, 0x4ee4b28cUL, 0x550c7dc3UL, 0xd5ffb4e2UL,
    0x72be5d74UL, 0xf27b896fUL, 0x80deb1feUL, 0x3b1696b1UL, 0x9bdc06a7UL, 0x25c71235UL, 0xc19bf174UL, 0xcf692694UL,
    0xe49b69c1UL, 0x9ef14ad2UL, 0xefbe4786UL, 0x384f25e3UL, 0x0fc19dc6UL, 0x8b8cd5b5UL, 0x240ca1ccUL, 0x77ac9c65UL,
    0x2de92c6fUL, 0x592b0275UL, 0x4a7484aaUL, 0x6ea6e483UL, 0x5cb0a9dcUL, 0xbd41fbd4UL, 0x76f988daUL, 0x831153b5UL,
    0x983e5152UL, 0xee66dfabUL, 0xa831c66dUL, 0x2db43210UL, 0xb00327c8UL, 0x98fb213fUL, 0xbf597fc7UL, 0xbeef0ee4UL,
    0xc6e00bf3UL, 0x3da88fc2UL, 0xd5a79147UL, 0x930aa725UL, 0x06ca6351UL, 0xe003826fUL, 0x14292967UL, 0x0a0e6e70UL,
    0x27b70a85UL, 0x46d22ffcUL, 0x2e1b2138UL, 0x5c26c926UL, 0x4d2c6dfcUL, 0x5ac42aedUL, 0x53380d13UL, 0x9d95b3dfUL,
    0x650a7354UL, 0x8baf63deUL, 0x766a0abbUL, 0x3c77b2a8UL, 0x81c2c92eUL, 0x47edaee6UL, 0x92722c85UL, 0x1482353bUL,
    0xa2bfe8a1UL, 0x4cf10364UL, 0xa81a664bUL, 0xbc423001UL, 0xc24b8b70UL, 0xd0f89791UL, 0xc76c51a3UL, 0x0654be30UL,
    0xd192e819UL, 0xd6ef5218UL, 0xd6990624UL, 0x5565a910UL, 0xf40e3585UL, 0x5771202aUL, 0x106aa070UL, 0x32bbd1b8UL,
    0x19a4c116UL, 0xb8d2d0c8UL, 0x1e376c08UL, 0x5141ab53UL, 0x2748774cUL, 0xdf8eeb99UL, 0x34b0bcb5UL, 0xe19b48a8UL,
    0x391c0cb3UL, 0xc5c95a63UL, 0x4ed8aa4aUL, 0xe3418acbUL, 0x5b9cca4fUL, 0x7763e373UL, 0x682e6ff3UL, 0xd6b2b8a3UL,
    0x748f82eeUL, 0x5defb2fcUL, 0x78a5636fUL, 0x43172f60UL, 0x84c87814UL, 0xa1f0ab72UL, 0x8cc70208UL, 0x1a6439ecUL,
    0x90befffaUL, 0x23631e28UL, 0xa4506cebUL, 0xde82bde9UL, 0xbef9a3f7UL, 0xb2c67915UL, 0xc67178f2UL, 0xe372532bUL,
    0xca273eceUL, 0xea26619cUL, 0xd186b8c7UL, 0x21c0c207UL, 0xeada7dd6UL, 0xcde0eb1eUL, 0xf57d4f7fUL, 0xee6ed178UL,
    0x06f067aaUL, 0x72176fbaUL, 0x0a637dc5UL, 0xa2c898a6UL, 0x113f9804UL, 0xbef90daeUL, 0x1b710b35UL, 0x131c471bUL,
    0x28db77f5UL, 0x23047d84UL, 0x32caab7bUL, 0x40c72493UL, 0x3c9ebe0aUL, 0x15c9bebcUL, 0x431d67c4UL, 0x9c100d4cUL,
    0x4cc5d4beUL, 0xcb3e42b6UL, 0x597f299cUL, 0xfc657e2aUL, 0x5fcb6fabUL, 0x3ad6faecUL, 0x6c44198cUL, 0x4a475817UL
};
# endif

# define VSECPRIM_STOP_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
# ifndef ACT_PLATFORM_UINT64_AVAILABLE

/**********************************************************************************************************************
 *  actSHA2_64_sha512Round()
 **********************************************************************************************************************/
/*! \brief      Performs SHA-512 round
 *  \details    This function performs an unrolled SHA-512 round.
 *  \param[in]  a  Element a
 *  \param[in]  b  Element b
 *  \param[in]  c  Element c
 *  \param[in,out] d  Element d
 *  \param[in]  e  Element e
 *  \param[in]  f  Element f
 *  \param[in]  g  Element g
 *  \param[in,out] h  Element h
 *  \param[in]  t  Element t
 *  \param[out] W  Output buffer W
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        All elements have to be valid input.
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actSHA2_64_sha512Round(
  VSECPRIM_P2CONST_PARA(actU32) a,
  VSECPRIM_P2CONST_PARA(actU32) b,
  VSECPRIM_P2CONST_PARA(actU32) c,
  VSECPRIM_P2VAR_PARA(actU32) d,
  VSECPRIM_P2CONST_PARA(actU32) e,
  VSECPRIM_P2CONST_PARA(actU32) f,
  VSECPRIM_P2CONST_PARA(actU32) g,
  VSECPRIM_P2VAR_PARA(actU32) h,
  actU8 t,
  VSECPRIM_P2VAR_PARA(actU32) W);
# endif

/**********************************************************************************************************************
  actSHA2_32_Transform()
 **********************************************************************************************************************/
/*! \brief      SHA2 transformation.
 *  \details    This function hashes one 64 byte block of data.
 *  \param[in]      in  Pointer to 64 byte input data.
 *  \param[in,out]  H   Current hash state.
 *  \param[in]      watchdog  Pointer to watchdog function.
 *  \context    TASK
 *  \reentrant  TRUE
 *  \pre        Input data must be 64 byte
 *********************************************************************************************************************/
# if defined(ACT_PLATFORM_UINT64_AVAILABLE)
VSECPRIM_LOCAL_FUNC(void) actSHA2_64_Transform(
  VSECPRIM_P2CONST_PARA(actU8) in,
  VSECPRIM_P2VAR_PARA(actU64) H,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));
# else
VSECPRIM_LOCAL_FUNC(void) actSHA2_64_Transform (
  VSECPRIM_P2CONST_PARA(actU8) in,
  VSECPRIM_P2VAR_PARA(actU32) H,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));
# endif

/**********************************************************************************************************************
  actSHA2_32_Update()
 **********************************************************************************************************************/
/*! \brief      This function is the generic SHA2 update implementation.
 *  \details    This function hashes the given data and can be called arbitrarily often between an initialize and
 *              finalize of the SHA-2 algorithm. Uses any data already in the actSHA256STRUCT structure and leaves
 *              any partial data block there.
 *  \param[in,out]  info  In: Pointer to hash context structure. Out: Actualized hash context structure.
 *  \param[in]  dataIN  Pointer to data to be hashed.
 *  \param[in]  length  Length of data in bytes.
 *  \param[in]  watchdog  Pointer to watchdog trigger function
 *  \context    TASK
 *  \reentrant  TRUE
 *  \return     actEXCEPTION_LENGTH   total input more than 2^64 - 1 bit
 *              actOK                 else
 *  \pre        actSHAxxxInit() has been called before calling this function. dataIN != NULL is a valid pointer
                length >= 0
                info must be a valid pointer
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actSHA2_64_Update(
  VSECPRIM_P2VAR_PARA(actSHA512STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
  actSHA2_64_Finalize()
 **********************************************************************************************************************/
/*! \brief      This function finalizes the SHA algorithm and delivers the hash value.
 *  \details    This function applies padding and processes the leftovers of the input data to generate the message
 *              digest.
 *  \param[in,out]  info  In: Pointer to hash context structure. Out: Finalized hash context structure.
 *  \param[out]  hash  Buffer where the hash value shall be stored.
 *  \param[in]  watchdog  Pointer to watchdog trigger function.
 *  \context    TASK
 *  \reentrant  TRUE
 *  \return     actOK  Operation successful.
 *  \pre        actSHAxxxInit() has been called before calling this function.
 *              info must be a valid pointer
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actSHA2_64_Finalize(
  VSECPRIM_P2VAR_PARA(actSHA512STRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

# ifndef ACT_PLATFORM_UINT64_AVAILABLE

/**********************************************************************************************************************
 *  actSHA2_64_sha512Round()
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
VSECPRIM_LOCAL_FUNC(void) actSHA2_64_sha512Round (
  VSECPRIM_P2CONST_PARA(actU32) a,
  VSECPRIM_P2CONST_PARA(actU32) b,
  VSECPRIM_P2CONST_PARA(actU32) c,
  VSECPRIM_P2VAR_PARA(actU32) d,
  VSECPRIM_P2CONST_PARA(actU32) e,
  VSECPRIM_P2CONST_PARA(actU32) f,
  VSECPRIM_P2CONST_PARA(actU32) g,
  VSECPRIM_P2VAR_PARA(actU32) h,
  actU8 t,
  VSECPRIM_P2VAR_PARA(actU32) W)
{
  if (t >= 16u)
  {
    actU32 *w = &(W [(t *2u) & 0x1Eu]);

    /* calculate Wt */
    /* The next line is required to be compliant to the standard, but is already handeled by the upper code (*w = &(W [(t *2u) & 0x1Eu])). */
    /* cpy_64 (w, &(W [((t-16) *2) &0x1E])); */ /* W[t] = W[t-16] in place! */

    add_s1 (w, &(W [((t - 2u) * 2u) &0x1Eu])); /* W[t] += sigma1 (W[t -2]) */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
    add_64 (w, &(W [((t - 7u) * 2u) &0x1Eu])); /* W[t] +=         W[t -7] */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
    add_s0 (w, &(W [((t - 15u) * 2u) &0x1Eu])); /* W[t] += sigma0 (W[t-15]) */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
  }

  /* T1 = h + ... */
  add_S1  (h, e); /* T1 (h) = h + Sigma1 (e) */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
  add_Ch  (h, e, f, g); /* T1 (h) = h + Sigma1 (e) + Ch (e, f, g) */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
  add_64  (h, &(actSHA2_64_K [(t * 2u)])); /* T1 (h) = h + Sigma1 (e) + Ch (e, f, g) + Kt */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
  add_64  (h, &(W [(t * 2u) & 0x1Eu])); /* T1 (h) = h + Sigma1 (e) + Ch (e, f, g) + Kt + Wt */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */

  /* d += T1 */
  add_64  (d, h); /* d += T1 (h) */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */

  /* h (T1) += T2 */
  add_S0  (h, a); /* h (T1) += Sigma0 (a) */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
  add_Maj (h, a, b, c); /* h (T1) += Sigma0 (a) + Maj (a, b, c) */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
} /* PRQA S 6010, 6030, 6060 */ /* MD_MSR_STPTH, MD_MSR_STCYC, MD_VSECPRIM_STPAR */
# endif

/**********************************************************************************************************************
 *  actSHA2_64_Transform()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
# ifdef ACT_PLATFORM_UINT64_AVAILABLE
VSECPRIM_LOCAL_FUNC(void) actSHA2_64_Transform(
  VSECPRIM_P2CONST_PARA(actU8) in,
  VSECPRIM_P2VAR_PARA(actU64) H,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actU64 W[16];
  actU64 T[8];
  actU8 i, j;

  /* Copy state to working vars */
  for (i = 0u; i < 8u; ++i)
  {
    T[i] = H[i]; /* SBSW_VSECPRIM_WRITE_ARRAY */
  }

  actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* Copy 128 byte block 'in' (big endian format) to workspace W[] */
  actCpyBEtoU64(W, in, actHASH_BLOCK_SIZE_SHA512); /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* Perform SHA2 Rounds */
  for (j = 0u; j < 80u; j += 16u) /*lint -e{661, 662} */
  {
#  if (actSHA512_SPEED_UP > 0) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    /* rounds unrolled */
    if (j == 0u) {
      /* PRQA S 2910 8 */ /* MD_VSECPRIM_WRAPAROUND */
      Ra0(0u, j); Rb(0u); Rc(0u); Ra0(1u, j); Rb(1u); Rc(1u);       /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      Ra0(2u, j); Rb(2u); Rc(2u); Ra0(3u, j); Rb(3u); Rc(3u);       /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      Ra0(4u, j); Rb(4u); Rc(4u); Ra0(5u, j); Rb(5u); Rc(5u);       /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      Ra0(6u, j); Rb(6u); Rc(6u); Ra0(7u, j); Rb(7u); Rc(7u);       /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      Ra0(8u, j); Rb(8u); Rc(8u); Ra0(9u, j); Rb(9u); Rc(9u);       /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      Ra0(10u, j); Rb(10u); Rc(10u); Ra0(11u, j); Rb(11u); Rc(11u); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      Ra0(12u, j); Rb(12u); Rc(12u); Ra0(13u, j); Rb(13u); Rc(13u); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      Ra0(14u, j); Rb(14u); Rc(14u); Ra0(15u, j); Rb(15u); Rc(15u); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
    }
    else
    {
      /* PRQA S 2910 8 */ /* MD_VSECPRIM_WRAPAROUND */
      Ra2(0u, j); Rb(0u); Rc(0u); Ra2(1u, j); Rb(1u); Rc(1u);       /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */ /* PRQA S 2986 */ /* MD_VSECPRIM_REDUNDANT_OP */
      Ra2(2u, j); Rb(2u); Rc(2u); Ra2(3u, j); Rb(3u); Rc(3u);       /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      Ra2(4u, j); Rb(4u); Rc(4u); Ra2(5u, j); Rb(5u); Rc(5u);       /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      Ra2(6u, j); Rb(6u); Rc(6u); Ra2(7u, j); Rb(7u); Rc(7u);       /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      Ra2(8u, j); Rb(8u); Rc(8u); Ra2(9u, j); Rb(9u); Rc(9u);       /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      Ra2(10u, j); Rb(10u); Rc(10u); Ra2(11u, j); Rb(11u); Rc(11u); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      Ra2(12u, j); Rb(12u); Rc(12u); Ra2(13u, j); Rb(13u); Rc(13u); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      Ra2(14u, j); Rb(14u); Rc(14u); Ra2(15u, j); Rb(15u); Rc(15u); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
    }
#  else
    if (j == 0u) {
      for (i = 0u; i < 16u; ++i)
      {
        Ra0(i, j); /* PRQA S 2985, 3226 */ /* MD_VSECPRIM_REDUNDANT_OP, MD_VSECPRIM_ASSIGNMENT_RESULT_IN_OP */ /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */ /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
        Rb(i);                                                                                                /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
        Rc(i);    /* PRQA S 2986 */ /* MD_VSECPRIM_REDUNDANT_OP */                                            /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
        actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
      }
    }
    else
    {
      for (i = 0u; i < 16u; ++i)
      {
        Ra2(i, j); /* PRQA S 2985, 3226 */ /* MD_VSECPRIM_REDUNDANT_OP, MD_VSECPRIM_ASSIGNMENT_RESULT_IN_OP */ /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */ /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
        Rb(i);                                                                                                /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
        Rc(i);    /* PRQA S 2986 */ /* MD_VSECPRIM_REDUNDANT_OP */                                            /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_MASKED_INDEX */
        actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
      }
    }
#  endif
    actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* Update the HASH state with current results */
  for (i = 0u; i < 8u; ++i)
  {
    H[i] += T[i]; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_VARIABLE_INDEX_CHECK */
  }
}
# else
VSECPRIM_LOCAL_FUNC(void) actSHA2_64_Transform (
   VSECPRIM_P2CONST_PARA(actU8) in,
   VSECPRIM_P2VAR_PARA(actU32) H,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actU32 W [32];
  actU32 T [16];
  actU8 t, i;

  /* Copy state to working vars */
  for (i = 0u; i < 16u; ++i) {
    T [i] = H [i]; /* SBSW_VSECPRIM_WRITE_ARRAY */
  }

  actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* Copy 128 byte block 'in' (big endian format) to workspace W[] */
  actCpyBEtoU32 (W, in, actHASH_BLOCK_SIZE_SHA512); /* SBSW_VSECPRIM_CALL_FUNCTION */

  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* 80 operations of 16 rounds */
  for (t = 0u; t < 80u; t += 8u) {
#  if (actSHA512_SPEED_UP > 0) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    /* rounds unrolled */
    actSHA2_64_sha512Round(&T[ 0], &T[ 2], &T[ 4], &T[ 6], &T[ 8], &T[10], &T[12], &T[14], t + 0u, W); /* SBSW_VSECPRIM_CALL_FUNCTION */
    actSHA2_64_sha512Round(&T[14], &T[ 0], &T[ 2], &T[ 4], &T[ 6], &T[ 8], &T[10], &T[12], t + 1u, W); /* SBSW_VSECPRIM_CALL_FUNCTION */
    actSHA2_64_sha512Round(&T[12], &T[14], &T[ 0], &T[ 2], &T[ 4], &T[ 6], &T[ 8], &T[10], t + 2u, W); /* SBSW_VSECPRIM_CALL_FUNCTION */
    actSHA2_64_sha512Round(&T[10], &T[12], &T[14], &T[ 0], &T[ 2], &T[ 4], &T[ 6], &T[ 8], t + 3u, W); /* SBSW_VSECPRIM_CALL_FUNCTION */
    actSHA2_64_sha512Round(&T[ 8], &T[10], &T[12], &T[14], &T[ 0], &T[ 2], &T[ 4], &T[ 6], t + 4u, W); /* SBSW_VSECPRIM_CALL_FUNCTION */
    actSHA2_64_sha512Round(&T[ 6], &T[ 8], &T[10], &T[12], &T[14], &T[ 0], &T[ 2], &T[ 4], t + 5u, W); /* SBSW_VSECPRIM_CALL_FUNCTION */
    actSHA2_64_sha512Round(&T[ 4], &T[ 6], &T[ 8], &T[10], &T[12], &T[14], &T[ 0], &T[ 2], t + 6u, W); /* SBSW_VSECPRIM_CALL_FUNCTION */
    actSHA2_64_sha512Round(&T[ 2], &T[ 4], &T[ 6], &T[ 8], &T[10], &T[12], &T[14], &T[ 0], t + 7u, W); /* SBSW_VSECPRIM_CALL_FUNCTION */
    actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
#  else
    for (i = 0u; i < 8u; ++i)
    {
      actU32 tmp [2];

      actSHA2_64_sha512Round(&T[ 0], &T[ 2], &T[ 4], &T[ 6], &T[ 8], &T[10], &T[12], &T[14], t + i, W); /* SBSW_VSECPRIM_CALL_FUNCTION */

      cpy_64 (tmp, &T[14]); /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
      cpy_64 (&T[14], &T[12]); /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
      cpy_64 (&T[12], &T[10]); /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
      cpy_64 (&T[10], &T[ 8]); /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
      cpy_64 (&T[ 8], &T[ 6]); /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
      cpy_64 (&T[ 6], &T[ 4]); /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
      cpy_64 (&T[ 4], &T[ 2]); /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
      cpy_64 (&T[ 2], &T[ 0]); /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
      cpy_64 (&T[ 0],  tmp); /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
      actL3trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
    }
#  endif
  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* add the working vars back into state H[] */
  for (i = 0u; i <= (16u - 2u); i += 2u) {
    add_64 (&H [i], &T [i]); /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */ /* SBSW_VSECPRIM_WRITE_ARRAY */
  }
}  /* PRQA S 6010, 6030 */ /* MD_MSR_STPTH, MD_MSR_STCYC */
# endif

/**********************************************************************************************************************
 *  actSHA2_64_Update()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actSHA2_64_Update(
  VSECPRIM_P2VAR_PARA(actSHA512STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
# if defined(ACT_PLATFORM_UINT64_AVAILABLE)
   VSECPRIM_P2VAR_PARA(actU64) Hptr = info->H;
# else
   VSECPRIM_P2VAR_PARA(actU32) Hptr = info->H;
# endif
  VSECPRIM_P2VAR_PARA(actU8) bufferPtr = info->buffer;
  actRETURNCODE retVal = actOK;
  actLengthType diff = 0u;
  actLengthType blocks;
  actLengthType bcount;

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* Update byte count */
# if defined(ACT_PLATFORM_UINT64_AVAILABLE)
  if ((info->count_L + (actU64) length) < info->count_L)
  {
    info->count_H++; /* SBSW_VSECPRIM_WRITE_POINTER */
    if ((info->count_H) == ((actU64)1u << 61)) /* add carry */
    {
      retVal = actEXCEPTION_LENGTH; /* overflow: 2^128 bit input */
    }
  }
  info->count_L += (actU64)length; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
# else
  if ((info->count_L + (actU32)length) < info->count_L)
  {
    ++info->count_2; /* SBSW_VSECPRIM_WRITE_POINTER */
    if (info->count_2 == 0u)
    {
      ++info->count_3; /* SBSW_VSECPRIM_WRITE_POINTER */
      if (info->count_3 == 0u)
      {
        ++info->count_H;  /* SBSW_VSECPRIM_WRITE_POINTER */
        if ((info->count_H) == ((actU32) 1u << 29))
        {
          retVal = actEXCEPTION_LENGTH; /* overflow: 2^128 bit input */
        }
      }
    }
  }
  info->count_L += (actU32)length; /* SBSW_VSECPRIM_WRITE_POINTER */
# endif
  if (retVal == actOK)
  {
    actU8 finished = FALSE;

    /* Left overs of the last update */
    if (info->buffer_used > 0u)
    {
      diff = (actLengthType)actMin(length, (actHASH_BLOCK_SIZE_SHA512 - info->buffer_used));
      actMemCpyByteArray(&bufferPtr[info->buffer_used], dataIN, diff); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
      info->buffer_used += (actU16)(diff & 0xFFu); /* SBSW_VSECPRIM_WRITE_POINTER */
      if (info->buffer_used < actHASH_BLOCK_SIZE_SHA512)
      {
        finished = TRUE;
      }
      else
      {
        actSHA2_64_Transform(bufferPtr, Hptr, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
      }
    }

    if (finished == FALSE)
    {
      /* Complete blocks */
      blocks = (actLengthType)((length - diff) / actHASH_BLOCK_SIZE_SHA512);

      for (bcount = diff; bcount < (diff + (blocks*actHASH_BLOCK_SIZE_SHA512)); bcount += actHASH_BLOCK_SIZE_SHA512)
      {
        actSHA2_64_Transform(&dataIN[bcount], Hptr, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
      }

      /* Copy left overs into buffer */
      diff += (actHASH_BLOCK_SIZE_SHA512 * blocks);
      info->buffer_used = (actLengthType)(length - diff); /* actHASH_BLOCK_SIZE_SHA512 (128) fits in actU16 */ /* SBSW_VSECPRIM_WRITE_POINTER */
      actMemCpyByteArray(bufferPtr, &dataIN[diff], info->buffer_used); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

      actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
    }
  }

  return retVal;
}

/***********************************************************************************************************************
 *  actSHAFinalize
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actSHA2_64_Finalize(
  VSECPRIM_P2VAR_PARA(actSHA512STRUCT) info, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actLengthType len = (actLengthType)(info->count_L & 0x7Fu);
  const actLengthType pad_len = (actLengthType)(actHASH_BLOCK_SIZE_SHA512 - (2u * actSHA512_WORD_SIZE));
  VSECPRIM_P2VAR_PARA(actU8) tempBuffer = (VSECPRIM_P2VAR_PARA(actU8))info->buffer;
# ifdef ACT_PLATFORM_UINT64_AVAILABLE
  actU64 count_Lptr = info->count_L << 3;
  actU64 count_Hptr = (info->count_H << 3) | (info->count_L >> 61);
  VSECPRIM_P2VAR_PARA(actU64) digest = info->H;
# else
  actU32 count[4];
  VSECPRIM_P2VAR_PARA(actU32) digest = info->H;
  count[3] = info->count_L << 3; /* SBSW_VSECPRIM_WRITE_ARRAY */
  count[2] = ((info->count_2 << 3) | (info->count_L >> 29)); /* SBSW_VSECPRIM_WRITE_ARRAY */
  count[1] = ((info->count_3 << 3) | (info->count_2 >> 29)); /* SBSW_VSECPRIM_WRITE_ARRAY */
  count[0] = ((info->count_H << 3) | (info->count_3 >> 29)); /* SBSW_VSECPRIM_WRITE_ARRAY */
# endif

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* Pad byte '0x80' */
  tempBuffer[len] = 0x80u; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  len = len + 1u;

  /* Pad zeros */
  actMemset(&tempBuffer[len], 0u, (actLengthType)(actHASH_BLOCK_SIZE_SHA512 - len)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  if (len > pad_len)
  {
    actSHA2_64_Transform((VSECPRIM_P2CONST_PARA(actU8))tempBuffer, digest, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    actMemset(tempBuffer, 0u, (actLengthType)pad_len); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  /* Pad 128-bit input count to last 16 bytes in big endian format */
# ifdef ACT_PLATFORM_UINT64_AVAILABLE
  actU64toBE(&tempBuffer[pad_len], count_Hptr);                       /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */
  actU64toBE(&tempBuffer[pad_len + actSHA512_WORD_SIZE], count_Lptr); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SUBADDRESS */
# else
  actU32toBE(&tempBuffer[112], count[0]); /* SBSW_VSECPRIM_CALL_FUNCTION */
  actU32toBE(&tempBuffer[116], count[1]); /* SBSW_VSECPRIM_CALL_FUNCTION */
  actU32toBE(&tempBuffer[120], count[2]); /* SBSW_VSECPRIM_CALL_FUNCTION */
  actU32toBE(&tempBuffer[124], count[3]); /* SBSW_VSECPRIM_CALL_FUNCTION */
# endif

  /* Transform last block */
  actSHA2_64_Transform((VSECPRIM_P2CONST_PARA(actU8))tempBuffer, digest, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* Convert little endian SHA-512-Digest to big endian byte order */
# ifdef ACT_PLATFORM_UINT64_AVAILABLE
  actCpyU64toBE((VSECPRIM_P2VAR_PARA(actU8))digest, digest, actHASH_SIZE_SHA512 /actSHA512_WORD_SIZE); /* PRQA S 0310 */ /* MD_VSECPRIM_11.4 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
# else
  actCpyU32toBE((VSECPRIM_P2VAR_PARA(actU8))digest, digest, 16u); /* PRQA S 0310 */ /* MD_VSECPRIM_11.4 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
# endif

  /* Write message digest to output 'hash' */
  actMemCpyByteArray(hash, (VSECPRIM_P2CONST_PARA(actU8))(&digest[0]), info->result_length); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* Wipe buffer */
  actMemset((VSECPRIM_P2VAR_PARA(actU8))&info->buffer[0], 0u, actHASH_BLOCK_SIZE_SHA512); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  return actOK;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actSHA512_224Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHA512_224Init(VSECPRIM_P2VAR_PARA(actSHA512_224STRUCT) info)
{
  /* Initialize message digest */
# if defined (ACT_PLATFORM_UINT64_AVAILABLE)
  info->H[0] = 0x8C3D37C819544DA2ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[1] = 0x73E1996689DCD4D6ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[2] = 0x1DFAB7AE32FF9C82ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[3] = 0x679DD514582F9FCFULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[4] = 0x0F6D2B697BD44DA8ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[5] = 0x77E36F7304C48942ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[6] = 0x3F9D85A86A1D36C8ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[7] = 0x1112E6AD91D692A1ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
# else
  info->H[0] = 0x8C3D37C8UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[1] = 0x19544DA2UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[2] = 0x73E19966UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[3] = 0x89DCD4D6UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[4] = 0x1DFAB7AEUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[5] = 0x32FF9C82UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[6] = 0x679DD514UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[7] = 0x582F9FCFUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[8] = 0x0F6D2B69UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[9] = 0x7BD44DA8UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[10] = 0x77E36F73UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[11] = 0x04C48942UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[12] = 0x3F9D85A8UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[13] = 0x6A1D36C8UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[14] = 0x1112E6ADUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[15] = 0x91D692A1UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
# endif

  /* Initialize count variables */
  info->count_L = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
# ifndef ACT_PLATFORM_UINT64_AVAILABLE
  info->count_2 = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->count_3 = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
# endif
  info->count_H = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->buffer_used = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->result_length = actHASH_SIZE_SHA512_224; /* SBSW_VSECPRIM_WRITE_POINTER */

  return actOK;
}

/***********************************************************************************************************************
 *  actSHA512_224Update
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHA512_224Update(
  VSECPRIM_P2VAR_PARA(actSHA512_224STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void))
{
  return actSHA2_64_Update((VSECPRIM_P2VAR_PARA(actSHA512STRUCT)) info, dataIN, length, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/***********************************************************************************************************************
 *  actSHA512_224Finalize
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHA512_224Finalize(
  VSECPRIM_P2VAR_PARA(actSHA512_224STRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  return actSHA2_64_Finalize((VSECPRIM_P2VAR_PARA(actSHA512STRUCT)) info, hash, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  actSHA512_256Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHA512_256Init(VSECPRIM_P2VAR_PARA(actSHA512_256STRUCT) info)
{
  /* initialize message digest */
# if defined (ACT_PLATFORM_UINT64_AVAILABLE)
  info->H[0] = 0x22312194FC2BF72CULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[1] = 0x9F555FA3C84C64C2ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[2] = 0x2393B86B6F53B151ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[3] = 0x963877195940EABDULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[4] = 0x96283EE2A88EFFE3ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[5] = 0xBE5E1E2553863992ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[6] = 0x2B0199FC2C85B8AAULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[7] = 0x0EB72DDC81C52CA2ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
# else
  info->H[0] = 0x22312194UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[1] = 0xFC2BF72CUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[2] = 0x9F555FA3UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[3] = 0xC84C64C2UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[4] = 0x2393B86BUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[5] = 0x6F53B151UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[6] = 0x96387719UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[7] = 0x5940EABDUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[8] = 0x96283EE2UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[9] = 0xA88EFFE3UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[10] = 0xBE5E1E25UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[11] = 0x53863992UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[12] = 0x2B0199FCUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[13] = 0x2C85B8AAUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[14] = 0x0EB72DDCUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[15] = 0x81C52CA2UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
# endif

  /* initialize count variables */
  info->count_L = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
# ifndef ACT_PLATFORM_UINT64_AVAILABLE
  info->count_2 = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->count_3 = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
# endif
  info->count_H = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->buffer_used = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->result_length = actHASH_SIZE_SHA512_256; /* SBSW_VSECPRIM_WRITE_POINTER */

  return actOK;
}

/***********************************************************************************************************************
 *  actSHA512_256Update
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHA512_256Update(
  VSECPRIM_P2VAR_PARA(actSHA512_256STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  return actSHA2_64_Update((VSECPRIM_P2VAR_PARA(actSHA512STRUCT)) info, dataIN, length, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/***********************************************************************************************************************
 *  actSHA512_256Finalize
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHA512_256Finalize(
  VSECPRIM_P2VAR_PARA(actSHA512_256STRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  return actSHA2_64_Finalize((VSECPRIM_P2VAR_PARA(actSHA512STRUCT)) info, hash, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  actSHA384Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHA384Init(VSECPRIM_P2VAR_PARA(actSHA384STRUCT) info)
{
  /* Initialize message digest */
# if defined (ACT_PLATFORM_UINT64_AVAILABLE)
  info->H[0] = 0xcbbb9d5dc1059ed8ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[1] = 0x629a292a367cd507ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[2] = 0x9159015a3070dd17ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[3] = 0x152fecd8f70e5939ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[4] = 0x67332667ffc00b31ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[5] = 0x8eb44a8768581511ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[6] = 0xdb0c2e0d64f98fa7ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[7] = 0x47b5481dbefa4fa4ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
# else
  info->H[0] = 0xcbbb9d5dUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[1] = 0xc1059ed8UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[2] = 0x629a292aUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[3] = 0x367cd507UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[4] = 0x9159015aUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[5] = 0x3070dd17UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[6] = 0x152fecd8UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[7] = 0xf70e5939UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[8] = 0x67332667UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[9] = 0xffc00b31UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[10] = 0x8eb44a87UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[11] = 0x68581511UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[12] = 0xdb0c2e0dUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[13] = 0x64f98fa7UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[14] = 0x47b5481dUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[15] = 0xbefa4fa4UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
# endif

  /* Initialize count variables */
  info->count_L = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
# ifndef ACT_PLATFORM_UINT64_AVAILABLE
  info->count_2 = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->count_3 = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
# endif
  info->count_H = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->buffer_used = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->result_length = actHASH_SIZE_SHA384; /* SBSW_VSECPRIM_WRITE_POINTER */

  return actOK;
}

/***********************************************************************************************************************
 *  actSHA384Update
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHA384Update(
  VSECPRIM_P2VAR_PARA(actSHA384STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  return actSHA2_64_Update((VSECPRIM_P2VAR_PARA(actSHA512STRUCT)) info, dataIN, length, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/***********************************************************************************************************************
 *  actSHA384Finalize
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHA384Finalize(
  VSECPRIM_P2VAR_PARA(actSHA384STRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  return actSHA2_64_Finalize((VSECPRIM_P2VAR_PARA(actSHA512STRUCT)) info, hash, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  actSHA512Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHA512Init(VSECPRIM_P2VAR_PARA(actSHA512STRUCT) info)
{
  /* initialize message digest */
# if defined (ACT_PLATFORM_UINT64_AVAILABLE)
  info->H[0] = 0x6a09e667f3bcc908ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[1] = 0xbb67ae8584caa73bULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[2] = 0x3c6ef372fe94f82bULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[3] = 0xa54ff53a5f1d36f1ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[4] = 0x510e527fade682d1ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[5] = 0x9b05688c2b3e6c1fULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[6] = 0x1f83d9abfb41bd6bULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[7] = 0x5be0cd19137e2179ULL; /* SBSW_VSECPRIM_WRITE_ARRAY */
# else
  info->H[0] = 0x6a09e667UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[1] = 0xf3bcc908UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[2] = 0xbb67ae85UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[3] = 0x84caa73bUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[4] = 0x3c6ef372UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[5] = 0xfe94f82bUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[6] = 0xa54ff53aUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[7] = 0x5f1d36f1UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[8] = 0x510e527fUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[9] = 0xade682d1UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[10] = 0x9b05688cUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[11] = 0x2b3e6c1fUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[12] = 0x1f83d9abUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[13] = 0xfb41bd6bUL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[14] = 0x5be0cd19UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
  info->H[15] = 0x137e2179UL; /* SBSW_VSECPRIM_WRITE_ARRAY */
# endif

  /* initialize count variables */
  info->count_L       = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
# ifndef ACT_PLATFORM_UINT64_AVAILABLE
  info->count_2       = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->count_3       = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
# endif
  info->count_H       = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->buffer_used   = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->result_length = actHASH_SIZE_SHA512; /* SBSW_VSECPRIM_WRITE_POINTER */

  return actOK;
}

/***********************************************************************************************************************
 *  actSHA512Update
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHA512Update(
  VSECPRIM_P2VAR_PARA(actSHA512STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  return actSHA2_64_Update((VSECPRIM_P2VAR_PARA(actSHA512STRUCT)) info, dataIN, length, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/***********************************************************************************************************************
 *  actSHA512Finalize
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSHA512Finalize(
  VSECPRIM_P2VAR_PARA(actSHA512STRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  return actSHA2_64_Finalize((VSECPRIM_P2VAR_PARA(actSHA512STRUCT)) info, hash, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTSHA2_64_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actISHA2_32.c
 *********************************************************************************************************************/
