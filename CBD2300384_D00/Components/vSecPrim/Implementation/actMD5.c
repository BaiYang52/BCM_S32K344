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
/*!        \file  actMD5.c
 *        \brief  This file contains an MD5 implementation.
 *
 *      \details  Currently the actClib version is used.
 *                This file is part of the embedded systems library cvActLib/ES
 *********************************************************************************************************************/
#define ACTMD5_SOURCE

#include "actIMD5.h"
#include "actUtilities.h"
#include "actWatchdog.h"
#include "actConfig.h"

#if (VSECPRIM_ACTMD5_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

 /* PRQA S 2889 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

# define actMD5_WORD_SIZE                                             4u

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

 /**********************************************************************************************************************
 *  actMD5Transform()
 *********************************************************************************************************************/
/*! \brief          This function hashes one 64 byte block of data.
 *  \details        -
 *  \param[in]      in           block (64 byte) of input data
 *  \param[in,out]  digest       (in) pointer to (intermediate) digest
 *                               (out) updated (intermediate) digest
 *  \param[in]      watchdog     pointer to watchdog function
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/

 VSECPRIM_LOCAL_FUNC(void) actMD5Transform(
   VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU32) digest,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/****************************************************************************
 ** Local functions and macros
 ***************************************************************************/

/* rotl rotates x left n bits. */
# define rotl(x, n)                                                   (((x) << (n)) | ((x) >> (32 - (n))))

/* F, G, H and I are basic MD5 functions. */
# define F(x, y, z)                                                   ((z) ^ ((x) & ((y) ^ (z))))
# define G(x, y, z)                                                   F((z), (x), (y))
# define H(x, y, z)                                                   ((x) ^ (y) ^ (z))
# define I(x, y, z)                                                   ((y) ^ ((x) | (~z)))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
   Rotation is separate from addition to prevent recomputation. */
# define FF(a, b, c, d, x, s, ac)                                     { \
                                                                      (a) += F ((b), (c), (d)) + (x) + (actU32) (ac); \
                                                                      (a) = rotl ((a), (s)); \
                                                                      (a) += (b); \
                                                                      }

# define GG(a, b, c, d, x, s, ac)                                     { \
                                                                      (a) += G ((b), (c), (d)) + (x) + (actU32) (ac); \
                                                                      (a) = rotl ((a), (s)); \
                                                                      (a) += (b); \
                                                                      }

# define HH(a, b, c, d, x, s, ac)                                     { \
                                                                      (a) += H ((b), (c), (d)) + (x) + (actU32) (ac); \
                                                                      (a) = rotl ((a), (s)); \
                                                                      (a) += (b); \
                                                                      }

# define II(a, b, c, d, x, s, ac)                                     { \
                                                                      (a) += I ((b), (c), (d)) + (x) + (actU32) (ac); \
                                                                      (a) = rotl ((a), (s)); \
                                                                      (a) += (b); \
                                                                      }

/* 4294967296 * abs (sin (i)) */
# define SINE__1                                                      0xd76aa478UL
# define SINE__2                                                      0xe8c7b756UL
# define SINE__3                                                      0x242070dbUL
# define SINE__4                                                      0xc1bdceeeUL
# define SINE__5                                                      0xf57c0fafUL
# define SINE__6                                                      0x4787c62aUL
# define SINE__7                                                      0xa8304613UL
# define SINE__8                                                      0xfd469501UL
# define SINE__9                                                      0x698098d8UL
# define SINE_10                                                      0x8b44f7afUL
# define SINE_11                                                      0xffff5bb1UL
# define SINE_12                                                      0x895cd7beUL
# define SINE_13                                                      0x6b901122UL
# define SINE_14                                                      0xfd987193UL
# define SINE_15                                                      0xa679438eUL
# define SINE_16                                                      0x49b40821UL
# define SINE_17                                                      0xf61e2562UL
# define SINE_18                                                      0xc040b340UL
# define SINE_19                                                      0x265e5a51UL
# define SINE_20                                                      0xe9b6c7aaUL
# define SINE_21                                                      0xd62f105dUL
# define SINE_22                                                      0x02441453UL
# define SINE_23                                                      0xd8a1e681UL
# define SINE_24                                                      0xe7d3fbc8UL
# define SINE_25                                                      0x21e1cde6UL
# define SINE_26                                                      0xc33707d6UL
# define SINE_27                                                      0xf4d50d87UL
# define SINE_28                                                      0x455a14edUL
# define SINE_29                                                      0xa9e3e905UL
# define SINE_30                                                      0xfcefa3f8UL
# define SINE_31                                                      0x676f02d9UL
# define SINE_32                                                      0x8d2a4c8aUL
# define SINE_33                                                      0xfffa3942UL
# define SINE_34                                                      0x8771f681UL
# define SINE_35                                                      0x6d9d6122UL
# define SINE_36                                                      0xfde5380cUL
# define SINE_37                                                      0xa4beea44UL
# define SINE_38                                                      0x4bdecfa9UL
# define SINE_39                                                      0xf6bb4b60UL
# define SINE_40                                                      0xbebfbc70UL
# define SINE_41                                                      0x289b7ec6UL
# define SINE_42                                                      0xeaa127faUL
# define SINE_43                                                      0xd4ef3085UL
# define SINE_44                                                      0x04881d05UL
# define SINE_45                                                      0xd9d4d039UL
# define SINE_46                                                      0xe6db99e5UL
# define SINE_47                                                      0x1fa27cf8UL
# define SINE_48                                                      0xc4ac5665UL
# define SINE_49                                                      0xf4292244UL
# define SINE_50                                                      0x432aff97UL
# define SINE_51                                                      0xab9423a7UL
# define SINE_52                                                      0xfc93a039UL
# define SINE_53                                                      0x655b59c3UL
# define SINE_54                                                      0x8f0ccc92UL
# define SINE_55                                                      0xffeff47dUL
# define SINE_56                                                      0x85845dd1UL
# define SINE_57                                                      0x6fa87e4fUL
# define SINE_58                                                      0xfe2ce6e0UL
# define SINE_59                                                      0xa3014314UL
# define SINE_60                                                      0x4e0811a1UL
# define SINE_61                                                      0xf7537e82UL
# define SINE_62                                                      0xbd3af235UL
# define SINE_63                                                      0x2ad7d2bbUL
# define SINE_64                                                      0xeb86d391UL

# if (actMD5_SPEED_UP < 2)

#  define VSECPRIM_START_SEC_CONST_UNSPECIFIED
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) sine[] = /* PRQA S 3218 */ /* MD_VSECPRIM_8.7 */
{
   SINE__1, SINE__2, SINE__3, SINE__4, SINE__5, SINE__6, SINE__7, SINE__8,
   SINE__9, SINE_10, SINE_11, SINE_12, SINE_13, SINE_14, SINE_15, SINE_16,
   SINE_17, SINE_18, SINE_19, SINE_20, SINE_21, SINE_22, SINE_23, SINE_24,
   SINE_25, SINE_26, SINE_27, SINE_28, SINE_29, SINE_30, SINE_31, SINE_32,
   SINE_33, SINE_34, SINE_35, SINE_36, SINE_37, SINE_38, SINE_39, SINE_40,
   SINE_41, SINE_42, SINE_43, SINE_44, SINE_45, SINE_46, SINE_47, SINE_48,
   SINE_49, SINE_50, SINE_51, SINE_52, SINE_53, SINE_54, SINE_55, SINE_56,
   SINE_57, SINE_58, SINE_59, SINE_60, SINE_61, SINE_62, SINE_63, SINE_64
};

#  define VSECPRIM_STOP_SEC_CONST_UNSPECIFIED
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
# endif

/* Constants for rotates. */
# define R11                                                          7
# define R12                                                          12
# define R13                                                          17
# define R14                                                          22
# define R21                                                          5
# define R22                                                          9
# define R23                                                          14
# define R24                                                          20
# define R31                                                          4
# define R32                                                          11
# define R33                                                          16
# define R34                                                          23
# define R41                                                          6
# define R42                                                          10
# define R43                                                          15
# define R44                                                          21

# if (actMD5_SPEED_UP < 1)
#  define VSECPRIM_START_SEC_CONST_UNSPECIFIED
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) rotateR1[] =
{
   R11, R12, R13, R14,
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) rotateR2[] =
{
   R21, R22, R23, R24,
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) rotateR3[] =
{
   R31, R32, R33, R34,
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) rotateR4[] =
{
   R41, R42, R43, R44
};

#  define VSECPRIM_STOP_SEC_CONST_UNSPECIFIED
#  include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
# endif

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actMD5Transform()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 * 
 */
VSECPRIM_LOCAL_FUNC(void) actMD5Transform(
   VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU32) digest,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   actU32         W [16];
   actLengthType   i;

# if (actMD5_SPEED_UP < 2)
   actLengthType   r, s;
# endif

# if (actMD5_SPEED_UP < 1)
   actU32         t;
# endif

   /* Copy digest [] to working variables */
   actU32         Aval = digest [0];
   actU32         Bval = digest [1];
   actU32         Cval = digest [2];
   actU32         Dval = digest [3];

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* interpret block 'in' as little endian 32 bit values */
   actCpyLEtoU32 (W, in, actHASH_BLOCK_SIZE_MD5);

# if (actMD5_SPEED_UP == 2)

   /* Round 1 */
   FF (Aval, Bval, Cval, Dval, W [ 0], R11, SINE__1);
   FF (Dval, Aval, Bval, Cval, W [ 1], R12, SINE__2);
   FF (Cval, Dval, Aval, Bval, W [ 2], R13, SINE__3);
   FF (Bval, Cval, Dval, Aval, W [ 3], R14, SINE__4);
   FF (Aval, Bval, Cval, Dval, W [ 4], R11, SINE__5);
   FF (Dval, Aval, Bval, Cval, W [ 5], R12, SINE__6);
   FF (Cval, Dval, Aval, Bval, W [ 6], R13, SINE__7);
   FF (Bval, Cval, Dval, Aval, W [ 7], R14, SINE__8);
   FF (Aval, Bval, Cval, Dval, W [ 8], R11, SINE__9);
   FF (Dval, Aval, Bval, Cval, W [ 9], R12, SINE_10);
   FF (Cval, Dval, Aval, Bval, W [10], R13, SINE_11);
   FF (Bval, Cval, Dval, Aval, W [11], R14, SINE_12);
   FF (Aval, Bval, Cval, Dval, W [12], R11, SINE_13);
   FF (Dval, Aval, Bval, Cval, W [13], R12, SINE_14);
   FF (Cval, Dval, Aval, Bval, W [14], R13, SINE_15);
   FF (Bval, Cval, Dval, Aval, W [15], R14, SINE_16);

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* Round 2 */
   GG (Aval, Bval, Cval, Dval, W [ 1], R21, SINE_17);
   GG (Dval, Aval, Bval, Cval, W [ 6], R22, SINE_18);
   GG (Cval, Dval, Aval, Bval, W [11], R23, SINE_19);
   GG (Bval, Cval, Dval, Aval, W [ 0], R24, SINE_20);
   GG (Aval, Bval, Cval, Dval, W [ 5], R21, SINE_21);
   GG (Dval, Aval, Bval, Cval, W [10], R22, SINE_22);
   GG (Cval, Dval, Aval, Bval, W [15], R23, SINE_23);
   GG (Bval, Cval, Dval, Aval, W [ 4], R24, SINE_24);
   GG (Aval, Bval, Cval, Dval, W [ 9], R21, SINE_25);
   GG (Dval, Aval, Bval, Cval, W [14], R22, SINE_26);
   GG (Cval, Dval, Aval, Bval, W [ 3], R23, SINE_27);
   GG (Bval, Cval, Dval, Aval, W [ 8], R24, SINE_28);
   GG (Aval, Bval, Cval, Dval, W [13], R21, SINE_29);
   GG (Dval, Aval, Bval, Cval, W [ 2], R22, SINE_30);
   GG (Cval, Dval, Aval, Bval, W [ 7], R23, SINE_31);
   GG (Bval, Cval, Dval, Aval, W [12], R24, SINE_32);

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* Round 3 */
   HH (Aval, Bval, Cval, Dval, W [ 5], R31, SINE_33);
   HH (Dval, Aval, Bval, Cval, W [ 8], R32, SINE_34);
   HH (Cval, Dval, Aval, Bval, W [11], R33, SINE_35);
   HH (Bval, Cval, Dval, Aval, W [14], R34, SINE_36);
   HH (Aval, Bval, Cval, Dval, W [ 1], R31, SINE_37);
   HH (Dval, Aval, Bval, Cval, W [ 4], R32, SINE_38);
   HH (Cval, Dval, Aval, Bval, W [ 7], R33, SINE_39);
   HH (Bval, Cval, Dval, Aval, W [10], R34, SINE_40);
   HH (Aval, Bval, Cval, Dval, W [13], R31, SINE_41);
   HH (Dval, Aval, Bval, Cval, W [ 0], R32, SINE_42);
   HH (Cval, Dval, Aval, Bval, W [ 3], R33, SINE_43);
   HH (Bval, Cval, Dval, Aval, W [ 6], R34, SINE_44);
   HH (Aval, Bval, Cval, Dval, W [ 9], R31, SINE_45);
   HH (Dval, Aval, Bval, Cval, W [12], R32, SINE_46);
   HH (Cval, Dval, Aval, Bval, W [15], R33, SINE_47);
   HH (Bval, Cval, Dval, Aval, W [ 2], R34, SINE_48);

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* Round 4 */
   II (Aval, Bval, Cval, Dval, W [ 0], R41, SINE_49);
   II (Dval, Aval, Bval, Cval, W [ 7], R42, SINE_50);
   II (Cval, Dval, Aval, Bval, W [14], R43, SINE_51);
   II (Bval, Cval, Dval, Aval, W [ 5], R44, SINE_52);
   II (Aval, Bval, Cval, Dval, W [12], R41, SINE_53);
   II (Dval, Aval, Bval, Cval, W [ 3], R42, SINE_54);
   II (Cval, Dval, Aval, Bval, W [10], R43, SINE_55);
   II (Bval, Cval, Dval, Aval, W [ 1], R44, SINE_56);
   II (Aval, Bval, Cval, Dval, W [ 8], R41, SINE_57);
   II (Dval, Aval, Bval, Cval, W [15], R42, SINE_58);
   II (Cval, Dval, Aval, Bval, W [ 6], R43, SINE_59);
   II (Bval, Cval, Dval, Aval, W [13], R44, SINE_60);
   II (Aval, Bval, Cval, Dval, W [ 4], R41, SINE_61);
   II (Dval, Aval, Bval, Cval, W [11], R42, SINE_62);
   II (Cval, Dval, Aval, Bval, W [ 2], R43, SINE_63);
   II (Bval, Cval, Dval, Aval, W [ 9], R44, SINE_64);

# elif (actMD5_SPEED_UP == 1)

   s = 0u;

   /* Round 1 */
   i = 0u;
   for (r = 4u; (r != 0u); r--)
   {
     FF(Aval, Bval, Cval, Dval, W[i], R11, sine[s]); s++; i++;
     FF(Dval, Aval, Bval, Cval, W[i], R12, sine[s]); s++; i++;
     FF(Cval, Dval, Aval, Bval, W[i], R13, sine[s]); s++; i++;
     FF(Bval, Cval, Dval, Aval, W[i], R14, sine[s]); s++; i++;
   }

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* Round 2 */
   i = 1u;
   for (r = 4u; (r != 0u); r--)
   {
     GG (Aval, Bval, Cval, Dval, W [i], R21, sine [s]); s++; i += 5u; i &= 0x0Fu;
     GG (Dval, Aval, Bval, Cval, W [i], R22, sine [s]); s++; i += 5u; i &= 0x0Fu;
     GG (Cval, Dval, Aval, Bval, W [i], R23, sine [s]); s++; i += 5u; i &= 0x0Fu;
     GG (Bval, Cval, Dval, Aval, W [i], R24, sine [s]); s++; i += 5u; i &= 0x0Fu;
   }

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* Round 3 */
   i = 5u;
   for (r = 4u; (r != 0u); r--)
   {
     HH (Aval, Bval, Cval, Dval, W [i], R31, sine [s]); s++; i += 3u; i &= 0x0Fu;
     HH (Dval, Aval, Bval, Cval, W [i], R32, sine [s]); s++; i += 3u; i &= 0x0Fu;
     HH (Cval, Dval, Aval, Bval, W [i], R33, sine [s]); s++; i += 3u; i &= 0x0Fu;
     HH (Bval, Cval, Dval, Aval, W [i], R34, sine [s]); s++; i += 3u; i &= 0x0Fu;
   }

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* Round 4 */
   i = 0u;
   for (r = 4u; (r != 0u); r--)
   {
     II (Aval, Bval, Cval, Dval, W [i], R41, sine [s]); s++; i += 7u; i &= 0x0Fu;
     II (Dval, Aval, Bval, Cval, W [i], R42, sine [s]); s++; i += 7u; i &= 0x0Fu;
     II (Cval, Dval, Aval, Bval, W [i], R43, sine [s]); s++; i += 7u; i &= 0x0Fu;
     II (Bval, Cval, Dval, Aval, W [i], R44, sine [s]); s++; i += 7u; i &= 0x0Fu;
   }

# else /* no optimizations */

   s = 0;

   /* Round 1 */
   i = 0;
   for (r = 16; r; r--)
   {
     FF (Aval, Bval, Cval, Dval, W [i], rotateR1 [s & 3], sine [s]); s++; i++;
     t = Dval; Dval = Cval; Cval = Bval; Bval = Aval; Aval = t;
   }

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* Round 2 */
   i = 1;
   for (r = 16; r; r--)
   {
     GG (Aval, Bval, Cval, Dval, W [i], rotateR2 [s & 3], sine [s]); s++; i += 5; i &= 0x0F;
     t = Dval; Dval = Cval; Cval = Bval; Bval = Aval; Aval = t;
   }

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* Round 3 */
   i = 5;
   for (r = 16; r; r--)
   {
     HH (Aval, Bval, Cval, Dval, W [i], rotateR3 [s & 3], sine [s]); s++; i += 3; i &= 0x0F;
     t = Dval; Dval = Cval; Cval = Bval; Bval = Aval; Aval = t;
   }

   actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* Round 4 */
   i = 0;
   for (r = 16; r; r--)
   {
     II (Aval, Bval, Cval, Dval, W [i], rotateR4 [s & 3], sine [s]); s++; i += 7; i &= 0x0F;
     t = Dval; Dval = Cval; Cval = Bval; Bval = Aval; Aval = t;
   }

# endif

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* Add the working vars back into digest [] */
   digest [0] += Aval;
   digest [1] += Bval;
   digest [2] += Cval;
   digest [3] += Dval;
}

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/**********************************************************************************************************************
 *  actMD5Init()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 * 
 */
VSECPRIM_FUNC(actRETURNCODE) actMD5Init (VSECPRIM_P2VAR_PARA(actMD5STRUCT) info)
{
   /* init message digest */
   info->digest [0] = 0x67452301UL;
   info->digest [1] = 0xEFCDAB89UL;
   info->digest [2] = 0x98BADCFEUL;
   info->digest [3] = 0x10325476UL;

   /* init count variables */
   info->count[0] = 0u;
   info->count[1] = 0u;
   info->buffer_used = 0u;

   return actOK;
}

/**********************************************************************************************************************
 *  actMD5Update()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 * 
 */
VSECPRIM_FUNC(actRETURNCODE) actMD5Update(
   VSECPRIM_P2VAR_PARA(actMD5STRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) dataIN, actLengthType length,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2VAR_PARA(actU32) digest = info->digest;
   VSECPRIM_P2VAR_PARA(actU8)  buffer = info->buffer;
   actLengthType diff = 0, lastBlockEnd, bcount;

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* update byte count */
   info->count[0] += (actU32)length;
   if (info->count [0] < (actU32) length)
   {
      ++info->count [1];
   }
   /*  no overflow handling necessary (-> RFC 1321) */

   /* left overs of the last update */
   if (info->buffer_used > 0u)
   {
      diff = actMin (length, (actHASH_BLOCK_SIZE_MD5-info->buffer_used));
      actMemCpyByteArray(&buffer[info->buffer_used], dataIN, diff); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      info->buffer_used += diff;

      if (info->buffer_used < actHASH_BLOCK_SIZE_MD5)
      {
         return actOK;
      }
      else
      {
         actMD5Transform(buffer, digest, watchdog);
      }
   }

   /* complete blocks */
   lastBlockEnd = (length - diff) / actHASH_BLOCK_SIZE_MD5;
   lastBlockEnd = (lastBlockEnd * actHASH_BLOCK_SIZE_MD5) + diff;
   for (bcount=diff; bcount < lastBlockEnd; bcount+=actHASH_BLOCK_SIZE_MD5)
   {
      actMD5Transform(&dataIN[bcount], digest, watchdog);
   }

   /* copy left overs into buffer */
   info->buffer_used = length - lastBlockEnd;
   actMemCpyByteArray(buffer, &dataIN[lastBlockEnd], (info->buffer_used)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   return actOK;
}

/**********************************************************************************************************************
 *  actMD5Finalize()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 * 
 */
VSECPRIM_FUNC(actRETURNCODE) actMD5Finalize(
   VSECPRIM_P2VAR_PARA(actMD5STRUCT) info, VSECPRIM_P2VAR_PARA(actU8) hash,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   actU32   count [2];

   VSECPRIM_P2VAR_PARA(actU8)  buffer = info->buffer;
   VSECPRIM_P2VAR_PARA(actU32) digest = info->digest;
   actLengthType len = info->count [0] & 0x3Fu;

# define pad_len                                                      (actHASH_BLOCK_SIZE_MD5 - (2u * actMD5_WORD_SIZE))

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   count [0] =  info->count [0] << 3;
   count [1] = (info->count [1] << 3) | (info->count [0] >> 29);

   /* append padding */
   buffer[len] = 0x80u;
   len += 1u;
   actMemClear(&buffer[len], (actHASH_BLOCK_SIZE_MD5 - len)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
   if (len > pad_len)
   {
      actMD5Transform(buffer, digest, watchdog);
      actMemset(buffer, 0u, pad_len); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
   }

  /* pad input bit count to last 8 bytes in little endian format */
   actU32toLE (&buffer [pad_len],                   count [0]);
   actU32toLE (&buffer [pad_len +actMD5_WORD_SIZE], count [1]);

   /* transform last block */
   actMD5Transform(buffer, digest, watchdog);

   /* store digest in little endian byte order */
   actCpyU32toLE (hash, digest, actHASH_SIZE_MD5 / actMD5_WORD_SIZE);

   /* wipe buffer */
   actMemClear(info->buffer, actHASH_BLOCK_SIZE_MD5); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   return actOK;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTMD5_ENABLED == STD_ON) */
