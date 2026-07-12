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
/*!       \file  actAES.c
 *       \brief  AES implementation.
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

#define ACTAES_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actAES.h"
#include "actIAES.h"
#include "actUtilities.h"
#include "actWatchdog.h"
#include "actConfig.h"

#if (VSECPRIM_ACTAES_ENABLED == STD_ON)

/* PRQA S 3218 EOF */ /* MD_VSECPRIM_8.7 */

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/* 'ff_mult' macro used in speed up levels 0, 1:
   ---------------------------------------------
   Original code:
      #define ff_mult(a, b) (a && b ? pow_tab[ (log_tab[a]+log_tab[b]) % 255 ] : 0)
   Optimizations:
   - Use that the ff_mult(a, b) is called exactly once for a==2, 3, 9, 11, 13, 14
   - Replace the reduction mod 255 with a subtraction. (The subtraction is save,
     because the added values of log_tab are <= 254)
 */

/* ff_mult for a==2, log_tab[2]==0x19 */
# define ff_mult_2(b)                                                 ((actU32)(((b)==0u) ? 0u : (ACT_ROM_R_BYT(pow_tab[ ((ACT_ROM_R_BYT(log_tab[(b)])) >= 0xe6u) ? ((ACT_ROM_R_BYT(log_tab[(b)])) - 0xe6u) : (0x19u + (ACT_ROM_R_BYT(log_tab[(b)]))) ]))))
/* ff_mult for a==3, log_tab[3]==0x01 */
# define ff_mult_3(b)                                                 ((actU32)(((b)==0u) ? 0u : (ACT_ROM_R_BYT(pow_tab[ ((ACT_ROM_R_BYT(log_tab[(b)])) == 0xfeu) ? 0u : (0x01u + (ACT_ROM_R_BYT(log_tab[(b)]))) ]))))
/* ff_mult for a==9, log_tab[9]==0xc7 */
# define ff_mult_9(b)                                                 ((actU32)(((b)==0u) ? 0u : (ACT_ROM_R_BYT(pow_tab[ ((ACT_ROM_R_BYT(log_tab[(b)])) >= 0x38u) ? ((ACT_ROM_R_BYT(log_tab[(b)])) - 0x38u) : (0xc7u + (ACT_ROM_R_BYT(log_tab[(b)]))) ]))))
/* ff_mult for a==11, log_tab[11]==0x68 */
# define ff_mult_11(b)                                                ((actU32)(((b)==0u) ? 0u : (ACT_ROM_R_BYT(pow_tab[ ((ACT_ROM_R_BYT(log_tab[(b)])) >= 0x97u) ? ((ACT_ROM_R_BYT(log_tab[(b)])) - 0x97u) : (0x68u + (ACT_ROM_R_BYT(log_tab[(b)]))) ]))))
/* ff_mult for a==13, log_tab[13]==0xee */
# define ff_mult_13(b)                                                ((actU32)(((b)==0u) ? 0u : (ACT_ROM_R_BYT(pow_tab[ ((ACT_ROM_R_BYT(log_tab[(b)])) >= 0x11u) ? ((ACT_ROM_R_BYT(log_tab[(b)])) - 0x11u) : (0xeeu + (ACT_ROM_R_BYT(log_tab[(b)]))) ]))))
/* ff_mult for a==14, log_tab[14]==0xdf */
# define ff_mult_14(b)                                                ((actU32)(((b)==0u) ? 0u : (ACT_ROM_R_BYT(pow_tab[ ((ACT_ROM_R_BYT(log_tab[(b)])) >= 0x20u) ? ((ACT_ROM_R_BYT(log_tab[(b)])) - 0x20u) : (0xdfu + (ACT_ROM_R_BYT(log_tab[(b)]))) ]))))

/* Macros used for key expansion */

# define star_x(x)                                                    ((((x) & 0x7f7f7f7fuL) << 1u) ^ ((((x) & 0x80808080uL) >> 7u) * 0x1bu))

# define ls_box(x)                                                    \
                                                                      (((actU32)ACT_ROM_R_BYT(sbx_tab[actGetByte((x), 0u)])) ^    \
                                                                      ((actU32)ACT_ROM_R_BYT(sbx_tab[actGetByte((x), 1u)]) <<  8u) ^    \
                                                                      ((actU32)ACT_ROM_R_BYT(sbx_tab[actGetByte((x), 2u)]) << 16u) ^    \
                                                                      ((actU32)ACT_ROM_R_BYT(sbx_tab[actGetByte((x), 3u)]) << 24u))

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/****************************************************************************
 ** Look up tables
 ***************************************************************************/
VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) sbx_tab[256] =
{
  0x63u, 0x7cu, 0x77u, 0x7bu, 0xf2u, 0x6bu, 0x6fu, 0xc5u, 0x30u, 0x01u, 0x67u, 0x2bu, 0xfeu, 0xd7u, 0xabu, 0x76u,
  0xcau, 0x82u, 0xc9u, 0x7du, 0xfau, 0x59u, 0x47u, 0xf0u, 0xadu, 0xd4u, 0xa2u, 0xafu, 0x9cu, 0xa4u, 0x72u, 0xc0u,
  0xb7u, 0xfdu, 0x93u, 0x26u, 0x36u, 0x3fu, 0xf7u, 0xccu, 0x34u, 0xa5u, 0xe5u, 0xf1u, 0x71u, 0xd8u, 0x31u, 0x15u,
  0x04u, 0xc7u, 0x23u, 0xc3u, 0x18u, 0x96u, 0x05u, 0x9au, 0x07u, 0x12u, 0x80u, 0xe2u, 0xebu, 0x27u, 0xb2u, 0x75u,
  0x09u, 0x83u, 0x2cu, 0x1au, 0x1bu, 0x6eu, 0x5au, 0xa0u, 0x52u, 0x3bu, 0xd6u, 0xb3u, 0x29u, 0xe3u, 0x2fu, 0x84u,
  0x53u, 0xd1u, 0x00u, 0xedu, 0x20u, 0xfcu, 0xb1u, 0x5bu, 0x6au, 0xcbu, 0xbeu, 0x39u, 0x4au, 0x4cu, 0x58u, 0xcfu,
  0xd0u, 0xefu, 0xaau, 0xfbu, 0x43u, 0x4du, 0x33u, 0x85u, 0x45u, 0xf9u, 0x02u, 0x7fu, 0x50u, 0x3cu, 0x9fu, 0xa8u,
  0x51u, 0xa3u, 0x40u, 0x8fu, 0x92u, 0x9du, 0x38u, 0xf5u, 0xbcu, 0xb6u, 0xdau, 0x21u, 0x10u, 0xffu, 0xf3u, 0xd2u,
  0xcdu, 0x0cu, 0x13u, 0xecu, 0x5fu, 0x97u, 0x44u, 0x17u, 0xc4u, 0xa7u, 0x7eu, 0x3du, 0x64u, 0x5du, 0x19u, 0x73u,
  0x60u, 0x81u, 0x4fu, 0xdcu, 0x22u, 0x2au, 0x90u, 0x88u, 0x46u, 0xeeu, 0xb8u, 0x14u, 0xdeu, 0x5eu, 0x0bu, 0xdbu,
  0xe0u, 0x32u, 0x3au, 0x0au, 0x49u, 0x06u, 0x24u, 0x5cu, 0xc2u, 0xd3u, 0xacu, 0x62u, 0x91u, 0x95u, 0xe4u, 0x79u,
  0xe7u, 0xc8u, 0x37u, 0x6du, 0x8du, 0xd5u, 0x4eu, 0xa9u, 0x6cu, 0x56u, 0xf4u, 0xeau, 0x65u, 0x7au, 0xaeu, 0x08u,
  0xbau, 0x78u, 0x25u, 0x2eu, 0x1cu, 0xa6u, 0xb4u, 0xc6u, 0xe8u, 0xddu, 0x74u, 0x1fu, 0x4bu, 0xbdu, 0x8bu, 0x8au,
  0x70u, 0x3eu, 0xb5u, 0x66u, 0x48u, 0x03u, 0xf6u, 0x0eu, 0x61u, 0x35u, 0x57u, 0xb9u, 0x86u, 0xc1u, 0x1du, 0x9eu,
  0xe1u, 0xf8u, 0x98u, 0x11u, 0x69u, 0xd9u, 0x8eu, 0x94u, 0x9bu, 0x1eu, 0x87u, 0xe9u, 0xceu, 0x55u, 0x28u, 0xdfu,
  0x8cu, 0xa1u, 0x89u, 0x0du, 0xbfu, 0xe6u, 0x42u, 0x68u, 0x41u, 0x99u, 0x2du, 0x0fu, 0xb0u, 0x54u, 0xbbu, 0x16u
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) isb_tab[256] =
{
   0x52u, 0x09u, 0x6au, 0xd5u, 0x30u, 0x36u, 0xa5u, 0x38u, 0xbfu, 0x40u, 0xa3u, 0x9eu, 0x81u, 0xf3u, 0xd7u, 0xfbu,
   0x7cu, 0xe3u, 0x39u, 0x82u, 0x9bu, 0x2fu, 0xffu, 0x87u, 0x34u, 0x8eu, 0x43u, 0x44u, 0xc4u, 0xdeu, 0xe9u, 0xcbu,
   0x54u, 0x7bu, 0x94u, 0x32u, 0xa6u, 0xc2u, 0x23u, 0x3du, 0xeeu, 0x4cu, 0x95u, 0x0bu, 0x42u, 0xfau, 0xc3u, 0x4eu,
   0x08u, 0x2eu, 0xa1u, 0x66u, 0x28u, 0xd9u, 0x24u, 0xb2u, 0x76u, 0x5bu, 0xa2u, 0x49u, 0x6du, 0x8bu, 0xd1u, 0x25u,
   0x72u, 0xf8u, 0xf6u, 0x64u, 0x86u, 0x68u, 0x98u, 0x16u, 0xd4u, 0xa4u, 0x5cu, 0xccu, 0x5du, 0x65u, 0xb6u, 0x92u,
   0x6cu, 0x70u, 0x48u, 0x50u, 0xfdu, 0xedu, 0xb9u, 0xdau, 0x5eu, 0x15u, 0x46u, 0x57u, 0xa7u, 0x8du, 0x9du, 0x84u,
   0x90u, 0xd8u, 0xabu, 0x00u, 0x8cu, 0xbcu, 0xd3u, 0x0au, 0xf7u, 0xe4u, 0x58u, 0x05u, 0xb8u, 0xb3u, 0x45u, 0x06u,
   0xd0u, 0x2cu, 0x1eu, 0x8fu, 0xcau, 0x3fu, 0x0fu, 0x02u, 0xc1u, 0xafu, 0xbdu, 0x03u, 0x01u, 0x13u, 0x8au, 0x6bu,
   0x3au, 0x91u, 0x11u, 0x41u, 0x4fu, 0x67u, 0xdcu, 0xeau, 0x97u, 0xf2u, 0xcfu, 0xceu, 0xf0u, 0xb4u, 0xe6u, 0x73u,
   0x96u, 0xacu, 0x74u, 0x22u, 0xe7u, 0xadu, 0x35u, 0x85u, 0xe2u, 0xf9u, 0x37u, 0xe8u, 0x1cu, 0x75u, 0xdfu, 0x6eu,
   0x47u, 0xf1u, 0x1au, 0x71u, 0x1du, 0x29u, 0xc5u, 0x89u, 0x6fu, 0xb7u, 0x62u, 0x0eu, 0xaau, 0x18u, 0xbeu, 0x1bu,
   0xfcu, 0x56u, 0x3eu, 0x4bu, 0xc6u, 0xd2u, 0x79u, 0x20u, 0x9au, 0xdbu, 0xc0u, 0xfeu, 0x78u, 0xcdu, 0x5au, 0xf4u,
   0x1fu, 0xddu, 0xa8u, 0x33u, 0x88u, 0x07u, 0xc7u, 0x31u, 0xb1u, 0x12u, 0x10u, 0x59u, 0x27u, 0x80u, 0xecu, 0x5fu,
   0x60u, 0x51u, 0x7fu, 0xa9u, 0x19u, 0xb5u, 0x4au, 0x0du, 0x2du, 0xe5u, 0x7au, 0x9fu, 0x93u, 0xc9u, 0x9cu, 0xefu,
   0xa0u, 0xe0u, 0x3bu, 0x4du, 0xaeu, 0x2au, 0xf5u, 0xb0u, 0xc8u, 0xebu, 0xbbu, 0x3cu, 0x83u, 0x53u, 0x99u, 0x61u,
   0x17u, 0x2bu, 0x04u, 0x7eu, 0xbau, 0x77u, 0xd6u, 0x26u, 0xe1u, 0x69u, 0x14u, 0x63u, 0x55u, 0x21u, 0x0cu, 0x7du
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) rco_tab[10] =
{
   0x00000001uL, 0x00000002uL, 0x00000004uL, 0x00000008uL, 0x00000010uL, 0x00000020uL, 0x00000040uL, 0x00000080uL,
   0x0000001buL, 0x00000036uL
};

# if (actAES_SPEED_UP <= VSECPRIM_SPEED_UP_LEVEL_1) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) pow_tab[256] =
{
  0x01u, 0x03u, 0x05u, 0x0fu, 0x11u, 0x33u, 0x55u, 0xffu, 0x1au, 0x2eu, 0x72u, 0x96u, 0xa1u, 0xf8u, 0x13u, 0x35u,
  0x5fu, 0xe1u, 0x38u, 0x48u, 0xd8u, 0x73u, 0x95u, 0xa4u, 0xf7u, 0x02u, 0x06u, 0x0au, 0x1eu, 0x22u, 0x66u, 0xaau,
  0xe5u, 0x34u, 0x5cu, 0xe4u, 0x37u, 0x59u, 0xebu, 0x26u, 0x6au, 0xbeu, 0xd9u, 0x70u, 0x90u, 0xabu, 0xe6u, 0x31u,
  0x53u, 0xf5u, 0x04u, 0x0cu, 0x14u, 0x3cu, 0x44u, 0xccu, 0x4fu, 0xd1u, 0x68u, 0xb8u, 0xd3u, 0x6eu, 0xb2u, 0xcdu,
  0x4cu, 0xd4u, 0x67u, 0xa9u, 0xe0u, 0x3bu, 0x4du, 0xd7u, 0x62u, 0xa6u, 0xf1u, 0x08u, 0x18u, 0x28u, 0x78u, 0x88u,
  0x83u, 0x9eu, 0xb9u, 0xd0u, 0x6bu, 0xbdu, 0xdcu, 0x7fu, 0x81u, 0x98u, 0xb3u, 0xceu, 0x49u, 0xdbu, 0x76u, 0x9au,
  0xb5u, 0xc4u, 0x57u, 0xf9u, 0x10u, 0x30u, 0x50u, 0xf0u, 0x0bu, 0x1du, 0x27u, 0x69u, 0xbbu, 0xd6u, 0x61u, 0xa3u,
  0xfeu, 0x19u, 0x2bu, 0x7du, 0x87u, 0x92u, 0xadu, 0xecu, 0x2fu, 0x71u, 0x93u, 0xaeu, 0xe9u, 0x20u, 0x60u, 0xa0u,
  0xfbu, 0x16u, 0x3au, 0x4eu, 0xd2u, 0x6du, 0xb7u, 0xc2u, 0x5du, 0xe7u, 0x32u, 0x56u, 0xfau, 0x15u, 0x3fu, 0x41u,
  0xc3u, 0x5eu, 0xe2u, 0x3du, 0x47u, 0xc9u, 0x40u, 0xc0u, 0x5bu, 0xedu, 0x2cu, 0x74u, 0x9cu, 0xbfu, 0xdau, 0x75u,
  0x9fu, 0xbau, 0xd5u, 0x64u, 0xacu, 0xefu, 0x2au, 0x7eu, 0x82u, 0x9du, 0xbcu, 0xdfu, 0x7au, 0x8eu, 0x89u, 0x80u,
  0x9bu, 0xb6u, 0xc1u, 0x58u, 0xe8u, 0x23u, 0x65u, 0xafu, 0xeau, 0x25u, 0x6fu, 0xb1u, 0xc8u, 0x43u, 0xc5u, 0x54u,
  0xfcu, 0x1fu, 0x21u, 0x63u, 0xa5u, 0xf4u, 0x07u, 0x09u, 0x1bu, 0x2du, 0x77u, 0x99u, 0xb0u, 0xcbu, 0x46u, 0xcau,
  0x45u, 0xcfu, 0x4au, 0xdeu, 0x79u, 0x8bu, 0x86u, 0x91u, 0xa8u, 0xe3u, 0x3eu, 0x42u, 0xc6u, 0x51u, 0xf3u, 0x0eu,
  0x12u, 0x36u, 0x5au, 0xeeu, 0x29u, 0x7bu, 0x8du, 0x8cu, 0x8fu, 0x8au, 0x85u, 0x94u, 0xa7u, 0xf2u, 0x0du, 0x17u,
  0x39u, 0x4bu, 0xddu, 0x7cu, 0x84u, 0x97u, 0xa2u, 0xfdu, 0x1cu, 0x24u, 0x6cu, 0xb4u, 0xc7u, 0x52u, 0xf6u, 0x01u
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) log_tab[256] =
{
  0x00u, 0x00u, 0x19u, 0x01u, 0x32u, 0x02u, 0x1au, 0xc6u, 0x4bu, 0xc7u, 0x1bu, 0x68u, 0x33u, 0xeeu, 0xdfu, 0x03u,
  0x64u, 0x04u, 0xe0u, 0x0eu, 0x34u, 0x8du, 0x81u, 0xefu, 0x4cu, 0x71u, 0x08u, 0xc8u, 0xf8u, 0x69u, 0x1cu, 0xc1u,
  0x7du, 0xc2u, 0x1du, 0xb5u, 0xf9u, 0xb9u, 0x27u, 0x6au, 0x4du, 0xe4u, 0xa6u, 0x72u, 0x9au, 0xc9u, 0x09u, 0x78u,
  0x65u, 0x2fu, 0x8au, 0x05u, 0x21u, 0x0fu, 0xe1u, 0x24u, 0x12u, 0xf0u, 0x82u, 0x45u, 0x35u, 0x93u, 0xdau, 0x8eu,
  0x96u, 0x8fu, 0xdbu, 0xbdu, 0x36u, 0xd0u, 0xceu, 0x94u, 0x13u, 0x5cu, 0xd2u, 0xf1u, 0x40u, 0x46u, 0x83u, 0x38u,
  0x66u, 0xddu, 0xfdu, 0x30u, 0xbfu, 0x06u, 0x8bu, 0x62u, 0xb3u, 0x25u, 0xe2u, 0x98u, 0x22u, 0x88u, 0x91u, 0x10u,
  0x7eu, 0x6eu, 0x48u, 0xc3u, 0xa3u, 0xb6u, 0x1eu, 0x42u, 0x3au, 0x6bu, 0x28u, 0x54u, 0xfau, 0x85u, 0x3du, 0xbau,
  0x2bu, 0x79u, 0x0au, 0x15u, 0x9bu, 0x9fu, 0x5eu, 0xcau, 0x4eu, 0xd4u, 0xacu, 0xe5u, 0xf3u, 0x73u, 0xa7u, 0x57u,
  0xafu, 0x58u, 0xa8u, 0x50u, 0xf4u, 0xeau, 0xd6u, 0x74u, 0x4fu, 0xaeu, 0xe9u, 0xd5u, 0xe7u, 0xe6u, 0xadu, 0xe8u,
  0x2cu, 0xd7u, 0x75u, 0x7au, 0xebu, 0x16u, 0x0bu, 0xf5u, 0x59u, 0xcbu, 0x5fu, 0xb0u, 0x9cu, 0xa9u, 0x51u, 0xa0u,
  0x7fu, 0x0cu, 0xf6u, 0x6fu, 0x17u, 0xc4u, 0x49u, 0xecu, 0xd8u, 0x43u, 0x1fu, 0x2du, 0xa4u, 0x76u, 0x7bu, 0xb7u,
  0xccu, 0xbbu, 0x3eu, 0x5au, 0xfbu, 0x60u, 0xb1u, 0x86u, 0x3bu, 0x52u, 0xa1u, 0x6cu, 0xaau, 0x55u, 0x29u, 0x9du,
  0x97u, 0xb2u, 0x87u, 0x90u, 0x61u, 0xbeu, 0xdcu, 0xfcu, 0xbcu, 0x95u, 0xcfu, 0xcdu, 0x37u, 0x3fu, 0x5bu, 0xd1u,
  0x53u, 0x39u, 0x84u, 0x3cu, 0x41u, 0xa2u, 0x6du, 0x47u, 0x14u, 0x2au, 0x9eu, 0x5du, 0x56u, 0xf2u, 0xd3u, 0xabu,
  0x44u, 0x11u, 0x92u, 0xd9u, 0x23u, 0x20u, 0x2eu, 0x89u, 0xb4u, 0x7cu, 0xb8u, 0x26u, 0x77u, 0x99u, 0xe3u, 0xa5u,
  0x67u, 0x4au, 0xedu, 0xdeu, 0xc5u, 0x31u, 0xfeu, 0x18u, 0x0du, 0x63u, 0x8cu, 0x80u, 0xc0u, 0xf7u, 0x70u, 0x07u
};

# else

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) ft_tab[4][256] =
{
  {
    0xa56363c6uL, 0x847c7cf8uL, 0x997777eeuL, 0x8d7b7bf6uL, 0x0df2f2ffuL, 0xbd6b6bd6uL, 0xb16f6fdeuL, 0x54c5c591uL,
    0x50303060uL, 0x03010102uL, 0xa96767ceuL, 0x7d2b2b56uL, 0x19fefee7uL, 0x62d7d7b5uL, 0xe6abab4duL, 0x9a7676ecuL,
    0x45caca8fuL, 0x9d82821fuL, 0x40c9c989uL, 0x877d7dfauL, 0x15fafaefuL, 0xeb5959b2uL, 0xc947478euL, 0x0bf0f0fbuL,
    0xecadad41uL, 0x67d4d4b3uL, 0xfda2a25fuL, 0xeaafaf45uL, 0xbf9c9c23uL, 0xf7a4a453uL, 0x967272e4uL, 0x5bc0c09buL,
    0xc2b7b775uL, 0x1cfdfde1uL, 0xae93933duL, 0x6a26264cuL, 0x5a36366cuL, 0x413f3f7euL, 0x02f7f7f5uL, 0x4fcccc83uL,
    0x5c343468uL, 0xf4a5a551uL, 0x34e5e5d1uL, 0x08f1f1f9uL, 0x937171e2uL, 0x73d8d8abuL, 0x53313162uL, 0x3f15152auL,
    0x0c040408uL, 0x52c7c795uL, 0x65232346uL, 0x5ec3c39duL, 0x28181830uL, 0xa1969637uL, 0x0f05050auL, 0xb59a9a2fuL,
    0x0907070euL, 0x36121224uL, 0x9b80801buL, 0x3de2e2dfuL, 0x26ebebcduL, 0x6927274euL, 0xcdb2b27fuL, 0x9f7575eauL,
    0x1b090912uL, 0x9e83831duL, 0x742c2c58uL, 0x2e1a1a34uL, 0x2d1b1b36uL, 0xb26e6edcuL, 0xee5a5ab4uL, 0xfba0a05buL,
    0xf65252a4uL, 0x4d3b3b76uL, 0x61d6d6b7uL, 0xceb3b37duL, 0x7b292952uL, 0x3ee3e3dduL, 0x712f2f5euL, 0x97848413uL,
    0xf55353a6uL, 0x68d1d1b9uL, 0x00000000uL, 0x2cededc1uL, 0x60202040uL, 0x1ffcfce3uL, 0xc8b1b179uL, 0xed5b5bb6uL,
    0xbe6a6ad4uL, 0x46cbcb8duL, 0xd9bebe67uL, 0x4b393972uL, 0xde4a4a94uL, 0xd44c4c98uL, 0xe85858b0uL, 0x4acfcf85uL,
    0x6bd0d0bbuL, 0x2aefefc5uL, 0xe5aaaa4fuL, 0x16fbfbeduL, 0xc5434386uL, 0xd74d4d9auL, 0x55333366uL, 0x94858511uL,
    0xcf45458auL, 0x10f9f9e9uL, 0x06020204uL, 0x817f7ffeuL, 0xf05050a0uL, 0x443c3c78uL, 0xba9f9f25uL, 0xe3a8a84buL,
    0xf35151a2uL, 0xfea3a35duL, 0xc0404080uL, 0x8a8f8f05uL, 0xad92923fuL, 0xbc9d9d21uL, 0x48383870uL, 0x04f5f5f1uL,
    0xdfbcbc63uL, 0xc1b6b677uL, 0x75dadaafuL, 0x63212142uL, 0x30101020uL, 0x1affffe5uL, 0x0ef3f3fduL, 0x6dd2d2bfuL,
    0x4ccdcd81uL, 0x140c0c18uL, 0x35131326uL, 0x2fececc3uL, 0xe15f5fbeuL, 0xa2979735uL, 0xcc444488uL, 0x3917172euL,
    0x57c4c493uL, 0xf2a7a755uL, 0x827e7efcuL, 0x473d3d7auL, 0xac6464c8uL, 0xe75d5dbauL, 0x2b191932uL, 0x957373e6uL,
    0xa06060c0uL, 0x98818119uL, 0xd14f4f9euL, 0x7fdcdca3uL, 0x66222244uL, 0x7e2a2a54uL, 0xab90903buL, 0x8388880buL,
    0xca46468cuL, 0x29eeeec7uL, 0xd3b8b86buL, 0x3c141428uL, 0x79dedea7uL, 0xe25e5ebcuL, 0x1d0b0b16uL, 0x76dbdbaduL,
    0x3be0e0dbuL, 0x56323264uL, 0x4e3a3a74uL, 0x1e0a0a14uL, 0xdb494992uL, 0x0a06060cuL, 0x6c242448uL, 0xe45c5cb8uL,
    0x5dc2c29fuL, 0x6ed3d3bduL, 0xefacac43uL, 0xa66262c4uL, 0xa8919139uL, 0xa4959531uL, 0x37e4e4d3uL, 0x8b7979f2uL,
    0x32e7e7d5uL, 0x43c8c88buL, 0x5937376euL, 0xb76d6ddauL, 0x8c8d8d01uL, 0x64d5d5b1uL, 0xd24e4e9cuL, 0xe0a9a949uL,
    0xb46c6cd8uL, 0xfa5656acuL, 0x07f4f4f3uL, 0x25eaeacfuL, 0xaf6565cauL, 0x8e7a7af4uL, 0xe9aeae47uL, 0x18080810uL,
    0xd5baba6fuL, 0x887878f0uL, 0x6f25254auL, 0x722e2e5cuL, 0x241c1c38uL, 0xf1a6a657uL, 0xc7b4b473uL, 0x51c6c697uL,
    0x23e8e8cbuL, 0x7cdddda1uL, 0x9c7474e8uL, 0x211f1f3euL, 0xdd4b4b96uL, 0xdcbdbd61uL, 0x868b8b0duL, 0x858a8a0fuL,
    0x907070e0uL, 0x423e3e7cuL, 0xc4b5b571uL, 0xaa6666ccuL, 0xd8484890uL, 0x05030306uL, 0x01f6f6f7uL, 0x120e0e1cuL,
    0xa36161c2uL, 0x5f35356auL, 0xf95757aeuL, 0xd0b9b969uL, 0x91868617uL, 0x58c1c199uL, 0x271d1d3auL, 0xb99e9e27uL,
    0x38e1e1d9uL, 0x13f8f8ebuL, 0xb398982buL, 0x33111122uL, 0xbb6969d2uL, 0x70d9d9a9uL, 0x898e8e07uL, 0xa7949433uL,
    0xb69b9b2duL, 0x221e1e3cuL, 0x92878715uL, 0x20e9e9c9uL, 0x49cece87uL, 0xff5555aauL, 0x78282850uL, 0x7adfdfa5uL,
    0x8f8c8c03uL, 0xf8a1a159uL, 0x80898909uL, 0x170d0d1auL, 0xdabfbf65uL, 0x31e6e6d7uL, 0xc6424284uL, 0xb86868d0uL,
    0xc3414182uL, 0xb0999929uL, 0x772d2d5auL, 0x110f0f1euL, 0xcbb0b07buL, 0xfc5454a8uL, 0xd6bbbb6duL, 0x3a16162cuL
  },
  {
    0x6363c6a5uL, 0x7c7cf884uL, 0x7777ee99uL, 0x7b7bf68duL, 0xf2f2ff0duL, 0x6b6bd6bduL, 0x6f6fdeb1uL, 0xc5c59154uL,
    0x30306050uL, 0x01010203uL, 0x6767cea9uL, 0x2b2b567duL, 0xfefee719uL, 0xd7d7b562uL, 0xabab4de6uL, 0x7676ec9auL,
    0xcaca8f45uL, 0x82821f9duL, 0xc9c98940uL, 0x7d7dfa87uL, 0xfafaef15uL, 0x5959b2ebuL, 0x47478ec9uL, 0xf0f0fb0buL,
    0xadad41ecuL, 0xd4d4b367uL, 0xa2a25ffduL, 0xafaf45eauL, 0x9c9c23bfuL, 0xa4a453f7uL, 0x7272e496uL, 0xc0c09b5buL,
    0xb7b775c2uL, 0xfdfde11cuL, 0x93933daeuL, 0x26264c6auL, 0x36366c5auL, 0x3f3f7e41uL, 0xf7f7f502uL, 0xcccc834fuL,
    0x3434685cuL, 0xa5a551f4uL, 0xe5e5d134uL, 0xf1f1f908uL, 0x7171e293uL, 0xd8d8ab73uL, 0x31316253uL, 0x15152a3fuL,
    0x0404080cuL, 0xc7c79552uL, 0x23234665uL, 0xc3c39d5euL, 0x18183028uL, 0x969637a1uL, 0x05050a0fuL, 0x9a9a2fb5uL,
    0x07070e09uL, 0x12122436uL, 0x80801b9buL, 0xe2e2df3duL, 0xebebcd26uL, 0x27274e69uL, 0xb2b27fcduL, 0x7575ea9fuL,
    0x0909121buL, 0x83831d9euL, 0x2c2c5874uL, 0x1a1a342euL, 0x1b1b362duL, 0x6e6edcb2uL, 0x5a5ab4eeuL, 0xa0a05bfbuL,
    0x5252a4f6uL, 0x3b3b764duL, 0xd6d6b761uL, 0xb3b37dceuL, 0x2929527buL, 0xe3e3dd3euL, 0x2f2f5e71uL, 0x84841397uL,
    0x5353a6f5uL, 0xd1d1b968uL, 0x00000000uL, 0xededc12cuL, 0x20204060uL, 0xfcfce31fuL, 0xb1b179c8uL, 0x5b5bb6eduL,
    0x6a6ad4beuL, 0xcbcb8d46uL, 0xbebe67d9uL, 0x3939724buL, 0x4a4a94deuL, 0x4c4c98d4uL, 0x5858b0e8uL, 0xcfcf854auL,
    0xd0d0bb6buL, 0xefefc52auL, 0xaaaa4fe5uL, 0xfbfbed16uL, 0x434386c5uL, 0x4d4d9ad7uL, 0x33336655uL, 0x85851194uL,
    0x45458acfuL, 0xf9f9e910uL, 0x02020406uL, 0x7f7ffe81uL, 0x5050a0f0uL, 0x3c3c7844uL, 0x9f9f25bauL, 0xa8a84be3uL,
    0x5151a2f3uL, 0xa3a35dfeuL, 0x404080c0uL, 0x8f8f058auL, 0x92923faduL, 0x9d9d21bcuL, 0x38387048uL, 0xf5f5f104uL,
    0xbcbc63dfuL, 0xb6b677c1uL, 0xdadaaf75uL, 0x21214263uL, 0x10102030uL, 0xffffe51auL, 0xf3f3fd0euL, 0xd2d2bf6duL,
    0xcdcd814cuL, 0x0c0c1814uL, 0x13132635uL, 0xececc32fuL, 0x5f5fbee1uL, 0x979735a2uL, 0x444488ccuL, 0x17172e39uL,
    0xc4c49357uL, 0xa7a755f2uL, 0x7e7efc82uL, 0x3d3d7a47uL, 0x6464c8acuL, 0x5d5dbae7uL, 0x1919322buL, 0x7373e695uL,
    0x6060c0a0uL, 0x81811998uL, 0x4f4f9ed1uL, 0xdcdca37fuL, 0x22224466uL, 0x2a2a547euL, 0x90903babuL, 0x88880b83uL,
    0x46468ccauL, 0xeeeec729uL, 0xb8b86bd3uL, 0x1414283cuL, 0xdedea779uL, 0x5e5ebce2uL, 0x0b0b161duL, 0xdbdbad76uL,
    0xe0e0db3buL, 0x32326456uL, 0x3a3a744euL, 0x0a0a141euL, 0x494992dbuL, 0x06060c0auL, 0x2424486cuL, 0x5c5cb8e4uL,
    0xc2c29f5duL, 0xd3d3bd6euL, 0xacac43efuL, 0x6262c4a6uL, 0x919139a8uL, 0x959531a4uL, 0xe4e4d337uL, 0x7979f28buL,
    0xe7e7d532uL, 0xc8c88b43uL, 0x37376e59uL, 0x6d6ddab7uL, 0x8d8d018cuL, 0xd5d5b164uL, 0x4e4e9cd2uL, 0xa9a949e0uL,
    0x6c6cd8b4uL, 0x5656acfauL, 0xf4f4f307uL, 0xeaeacf25uL, 0x6565caafuL, 0x7a7af48euL, 0xaeae47e9uL, 0x08081018uL,
    0xbaba6fd5uL, 0x7878f088uL, 0x25254a6fuL, 0x2e2e5c72uL, 0x1c1c3824uL, 0xa6a657f1uL, 0xb4b473c7uL, 0xc6c69751uL,
    0xe8e8cb23uL, 0xdddda17cuL, 0x7474e89cuL, 0x1f1f3e21uL, 0x4b4b96dduL, 0xbdbd61dcuL, 0x8b8b0d86uL, 0x8a8a0f85uL,
    0x7070e090uL, 0x3e3e7c42uL, 0xb5b571c4uL, 0x6666ccaauL, 0x484890d8uL, 0x03030605uL, 0xf6f6f701uL, 0x0e0e1c12uL,
    0x6161c2a3uL, 0x35356a5fuL, 0x5757aef9uL, 0xb9b969d0uL, 0x86861791uL, 0xc1c19958uL, 0x1d1d3a27uL, 0x9e9e27b9uL,
    0xe1e1d938uL, 0xf8f8eb13uL, 0x98982bb3uL, 0x11112233uL, 0x6969d2bbuL, 0xd9d9a970uL, 0x8e8e0789uL, 0x949433a7uL,
    0x9b9b2db6uL, 0x1e1e3c22uL, 0x87871592uL, 0xe9e9c920uL, 0xcece8749uL, 0x5555aaffuL, 0x28285078uL, 0xdfdfa57auL,
    0x8c8c038fuL, 0xa1a159f8uL, 0x89890980uL, 0x0d0d1a17uL, 0xbfbf65dauL, 0xe6e6d731uL, 0x424284c6uL, 0x6868d0b8uL,
    0x414182c3uL, 0x999929b0uL, 0x2d2d5a77uL, 0x0f0f1e11uL, 0xb0b07bcbuL, 0x5454a8fcuL, 0xbbbb6dd6uL, 0x16162c3auL
  },
  {
    0x63c6a563uL, 0x7cf8847cuL, 0x77ee9977uL, 0x7bf68d7buL, 0xf2ff0df2uL, 0x6bd6bd6buL, 0x6fdeb16fuL, 0xc59154c5uL,
    0x30605030uL, 0x01020301uL, 0x67cea967uL, 0x2b567d2buL, 0xfee719feuL, 0xd7b562d7uL, 0xab4de6abuL, 0x76ec9a76uL,
    0xca8f45cauL, 0x821f9d82uL, 0xc98940c9uL, 0x7dfa877duL, 0xfaef15fauL, 0x59b2eb59uL, 0x478ec947uL, 0xf0fb0bf0uL,
    0xad41ecaduL, 0xd4b367d4uL, 0xa25ffda2uL, 0xaf45eaafuL, 0x9c23bf9cuL, 0xa453f7a4uL, 0x72e49672uL, 0xc09b5bc0uL,
    0xb775c2b7uL, 0xfde11cfduL, 0x933dae93uL, 0x264c6a26uL, 0x366c5a36uL, 0x3f7e413fuL, 0xf7f502f7uL, 0xcc834fccuL,
    0x34685c34uL, 0xa551f4a5uL, 0xe5d134e5uL, 0xf1f908f1uL, 0x71e29371uL, 0xd8ab73d8uL, 0x31625331uL, 0x152a3f15uL,
    0x04080c04uL, 0xc79552c7uL, 0x23466523uL, 0xc39d5ec3uL, 0x18302818uL, 0x9637a196uL, 0x050a0f05uL, 0x9a2fb59auL,
    0x070e0907uL, 0x12243612uL, 0x801b9b80uL, 0xe2df3de2uL, 0xebcd26ebuL, 0x274e6927uL, 0xb27fcdb2uL, 0x75ea9f75uL,
    0x09121b09uL, 0x831d9e83uL, 0x2c58742cuL, 0x1a342e1auL, 0x1b362d1buL, 0x6edcb26euL, 0x5ab4ee5auL, 0xa05bfba0uL,
    0x52a4f652uL, 0x3b764d3buL, 0xd6b761d6uL, 0xb37dceb3uL, 0x29527b29uL, 0xe3dd3ee3uL, 0x2f5e712fuL, 0x84139784uL,
    0x53a6f553uL, 0xd1b968d1uL, 0x00000000uL, 0xedc12ceduL, 0x20406020uL, 0xfce31ffcuL, 0xb179c8b1uL, 0x5bb6ed5buL,
    0x6ad4be6auL, 0xcb8d46cbuL, 0xbe67d9beuL, 0x39724b39uL, 0x4a94de4auL, 0x4c98d44cuL, 0x58b0e858uL, 0xcf854acfuL,
    0xd0bb6bd0uL, 0xefc52aefuL, 0xaa4fe5aauL, 0xfbed16fbuL, 0x4386c543uL, 0x4d9ad74duL, 0x33665533uL, 0x85119485uL,
    0x458acf45uL, 0xf9e910f9uL, 0x02040602uL, 0x7ffe817fuL, 0x50a0f050uL, 0x3c78443cuL, 0x9f25ba9fuL, 0xa84be3a8uL,
    0x51a2f351uL, 0xa35dfea3uL, 0x4080c040uL, 0x8f058a8fuL, 0x923fad92uL, 0x9d21bc9duL, 0x38704838uL, 0xf5f104f5uL,
    0xbc63dfbcuL, 0xb677c1b6uL, 0xdaaf75dauL, 0x21426321uL, 0x10203010uL, 0xffe51affuL, 0xf3fd0ef3uL, 0xd2bf6dd2uL,
    0xcd814ccduL, 0x0c18140cuL, 0x13263513uL, 0xecc32fecuL, 0x5fbee15fuL, 0x9735a297uL, 0x4488cc44uL, 0x172e3917uL,
    0xc49357c4uL, 0xa755f2a7uL, 0x7efc827euL, 0x3d7a473duL, 0x64c8ac64uL, 0x5dbae75duL, 0x19322b19uL, 0x73e69573uL,
    0x60c0a060uL, 0x81199881uL, 0x4f9ed14fuL, 0xdca37fdcuL, 0x22446622uL, 0x2a547e2auL, 0x903bab90uL, 0x880b8388uL,
    0x468cca46uL, 0xeec729eeuL, 0xb86bd3b8uL, 0x14283c14uL, 0xdea779deuL, 0x5ebce25euL, 0x0b161d0buL, 0xdbad76dbuL,
    0xe0db3be0uL, 0x32645632uL, 0x3a744e3auL, 0x0a141e0auL, 0x4992db49uL, 0x060c0a06uL, 0x24486c24uL, 0x5cb8e45cuL,
    0xc29f5dc2uL, 0xd3bd6ed3uL, 0xac43efacuL, 0x62c4a662uL, 0x9139a891uL, 0x9531a495uL, 0xe4d337e4uL, 0x79f28b79uL,
    0xe7d532e7uL, 0xc88b43c8uL, 0x376e5937uL, 0x6ddab76duL, 0x8d018c8duL, 0xd5b164d5uL, 0x4e9cd24euL, 0xa949e0a9uL,
    0x6cd8b46cuL, 0x56acfa56uL, 0xf4f307f4uL, 0xeacf25eauL, 0x65caaf65uL, 0x7af48e7auL, 0xae47e9aeuL, 0x08101808uL,
    0xba6fd5bauL, 0x78f08878uL, 0x254a6f25uL, 0x2e5c722euL, 0x1c38241cuL, 0xa657f1a6uL, 0xb473c7b4uL, 0xc69751c6uL,
    0xe8cb23e8uL, 0xdda17cdduL, 0x74e89c74uL, 0x1f3e211fuL, 0x4b96dd4buL, 0xbd61dcbduL, 0x8b0d868buL, 0x8a0f858auL,
    0x70e09070uL, 0x3e7c423euL, 0xb571c4b5uL, 0x66ccaa66uL, 0x4890d848uL, 0x03060503uL, 0xf6f701f6uL, 0x0e1c120euL,
    0x61c2a361uL, 0x356a5f35uL, 0x57aef957uL, 0xb969d0b9uL, 0x86179186uL, 0xc19958c1uL, 0x1d3a271duL, 0x9e27b99euL,
    0xe1d938e1uL, 0xf8eb13f8uL, 0x982bb398uL, 0x11223311uL, 0x69d2bb69uL, 0xd9a970d9uL, 0x8e07898euL, 0x9433a794uL,
    0x9b2db69buL, 0x1e3c221euL, 0x87159287uL, 0xe9c920e9uL, 0xce8749ceuL, 0x55aaff55uL, 0x28507828uL, 0xdfa57adfuL,
    0x8c038f8cuL, 0xa159f8a1uL, 0x89098089uL, 0x0d1a170duL, 0xbf65dabfuL, 0xe6d731e6uL, 0x4284c642uL, 0x68d0b868uL,
    0x4182c341uL, 0x9929b099uL, 0x2d5a772duL, 0x0f1e110fuL, 0xb07bcbb0uL, 0x54a8fc54uL, 0xbb6dd6bbuL, 0x162c3a16uL
  },
  {
    0xc6a56363uL, 0xf8847c7cuL, 0xee997777uL, 0xf68d7b7buL, 0xff0df2f2uL, 0xd6bd6b6buL, 0xdeb16f6fuL, 0x9154c5c5uL,
    0x60503030uL, 0x02030101uL, 0xcea96767uL, 0x567d2b2buL, 0xe719fefeuL, 0xb562d7d7uL, 0x4de6ababuL, 0xec9a7676uL,
    0x8f45cacauL, 0x1f9d8282uL, 0x8940c9c9uL, 0xfa877d7duL, 0xef15fafauL, 0xb2eb5959uL, 0x8ec94747uL, 0xfb0bf0f0uL,
    0x41ecadaduL, 0xb367d4d4uL, 0x5ffda2a2uL, 0x45eaafafuL, 0x23bf9c9cuL, 0x53f7a4a4uL, 0xe4967272uL, 0x9b5bc0c0uL,
    0x75c2b7b7uL, 0xe11cfdfduL, 0x3dae9393uL, 0x4c6a2626uL, 0x6c5a3636uL, 0x7e413f3fuL, 0xf502f7f7uL, 0x834fccccuL,
    0x685c3434uL, 0x51f4a5a5uL, 0xd134e5e5uL, 0xf908f1f1uL, 0xe2937171uL, 0xab73d8d8uL, 0x62533131uL, 0x2a3f1515uL,
    0x080c0404uL, 0x9552c7c7uL, 0x46652323uL, 0x9d5ec3c3uL, 0x30281818uL, 0x37a19696uL, 0x0a0f0505uL, 0x2fb59a9auL,
    0x0e090707uL, 0x24361212uL, 0x1b9b8080uL, 0xdf3de2e2uL, 0xcd26ebebuL, 0x4e692727uL, 0x7fcdb2b2uL, 0xea9f7575uL,
    0x121b0909uL, 0x1d9e8383uL, 0x58742c2cuL, 0x342e1a1auL, 0x362d1b1buL, 0xdcb26e6euL, 0xb4ee5a5auL, 0x5bfba0a0uL,
    0xa4f65252uL, 0x764d3b3buL, 0xb761d6d6uL, 0x7dceb3b3uL, 0x527b2929uL, 0xdd3ee3e3uL, 0x5e712f2fuL, 0x13978484uL,
    0xa6f55353uL, 0xb968d1d1uL, 0x00000000uL, 0xc12cededuL, 0x40602020uL, 0xe31ffcfcuL, 0x79c8b1b1uL, 0xb6ed5b5buL,
    0xd4be6a6auL, 0x8d46cbcbuL, 0x67d9bebeuL, 0x724b3939uL, 0x94de4a4auL, 0x98d44c4cuL, 0xb0e85858uL, 0x854acfcfuL,
    0xbb6bd0d0uL, 0xc52aefefuL, 0x4fe5aaaauL, 0xed16fbfbuL, 0x86c54343uL, 0x9ad74d4duL, 0x66553333uL, 0x11948585uL,
    0x8acf4545uL, 0xe910f9f9uL, 0x04060202uL, 0xfe817f7fuL, 0xa0f05050uL, 0x78443c3cuL, 0x25ba9f9fuL, 0x4be3a8a8uL,
    0xa2f35151uL, 0x5dfea3a3uL, 0x80c04040uL, 0x058a8f8fuL, 0x3fad9292uL, 0x21bc9d9duL, 0x70483838uL, 0xf104f5f5uL,
    0x63dfbcbcuL, 0x77c1b6b6uL, 0xaf75dadauL, 0x42632121uL, 0x20301010uL, 0xe51affffuL, 0xfd0ef3f3uL, 0xbf6dd2d2uL,
    0x814ccdcduL, 0x18140c0cuL, 0x26351313uL, 0xc32fececuL, 0xbee15f5fuL, 0x35a29797uL, 0x88cc4444uL, 0x2e391717uL,
    0x9357c4c4uL, 0x55f2a7a7uL, 0xfc827e7euL, 0x7a473d3duL, 0xc8ac6464uL, 0xbae75d5duL, 0x322b1919uL, 0xe6957373uL,
    0xc0a06060uL, 0x19988181uL, 0x9ed14f4fuL, 0xa37fdcdcuL, 0x44662222uL, 0x547e2a2auL, 0x3bab9090uL, 0x0b838888uL,
    0x8cca4646uL, 0xc729eeeeuL, 0x6bd3b8b8uL, 0x283c1414uL, 0xa779dedeuL, 0xbce25e5euL, 0x161d0b0buL, 0xad76dbdbuL,
    0xdb3be0e0uL, 0x64563232uL, 0x744e3a3auL, 0x141e0a0auL, 0x92db4949uL, 0x0c0a0606uL, 0x486c2424uL, 0xb8e45c5cuL,
    0x9f5dc2c2uL, 0xbd6ed3d3uL, 0x43efacacuL, 0xc4a66262uL, 0x39a89191uL, 0x31a49595uL, 0xd337e4e4uL, 0xf28b7979uL,
    0xd532e7e7uL, 0x8b43c8c8uL, 0x6e593737uL, 0xdab76d6duL, 0x018c8d8duL, 0xb164d5d5uL, 0x9cd24e4euL, 0x49e0a9a9uL,
    0xd8b46c6cuL, 0xacfa5656uL, 0xf307f4f4uL, 0xcf25eaeauL, 0xcaaf6565uL, 0xf48e7a7auL, 0x47e9aeaeuL, 0x10180808uL,
    0x6fd5babauL, 0xf0887878uL, 0x4a6f2525uL, 0x5c722e2euL, 0x38241c1cuL, 0x57f1a6a6uL, 0x73c7b4b4uL, 0x9751c6c6uL,
    0xcb23e8e8uL, 0xa17cdddduL, 0xe89c7474uL, 0x3e211f1fuL, 0x96dd4b4buL, 0x61dcbdbduL, 0x0d868b8buL, 0x0f858a8auL,
    0xe0907070uL, 0x7c423e3euL, 0x71c4b5b5uL, 0xccaa6666uL, 0x90d84848uL, 0x06050303uL, 0xf701f6f6uL, 0x1c120e0euL,
    0xc2a36161uL, 0x6a5f3535uL, 0xaef95757uL, 0x69d0b9b9uL, 0x17918686uL, 0x9958c1c1uL, 0x3a271d1duL, 0x27b99e9euL,
    0xd938e1e1uL, 0xeb13f8f8uL, 0x2bb39898uL, 0x22331111uL, 0xd2bb6969uL, 0xa970d9d9uL, 0x07898e8euL, 0x33a79494uL,
    0x2db69b9buL, 0x3c221e1euL, 0x15928787uL, 0xc920e9e9uL, 0x8749ceceuL, 0xaaff5555uL, 0x50782828uL, 0xa57adfdfuL,
    0x038f8c8cuL, 0x59f8a1a1uL, 0x09808989uL, 0x1a170d0duL, 0x65dabfbfuL, 0xd731e6e6uL, 0x84c64242uL, 0xd0b86868uL,
    0x82c34141uL, 0x29b09999uL, 0x5a772d2duL, 0x1e110f0fuL, 0x7bcbb0b0uL, 0xa8fc5454uL, 0x6dd6bbbbuL, 0x2c3a1616uL
  }
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) it_tab[4][256] =
{
  {
    0x50a7f451uL, 0x5365417euL, 0xc3a4171auL, 0x965e273auL, 0xcb6bab3buL, 0xf1459d1fuL, 0xab58faacuL, 0x9303e34buL,
    0x55fa3020uL, 0xf66d76aduL, 0x9176cc88uL, 0x254c02f5uL, 0xfcd7e54fuL, 0xd7cb2ac5uL, 0x80443526uL, 0x8fa362b5uL,
    0x495ab1deuL, 0x671bba25uL, 0x980eea45uL, 0xe1c0fe5duL, 0x02752fc3uL, 0x12f04c81uL, 0xa397468duL, 0xc6f9d36buL,
    0xe75f8f03uL, 0x959c9215uL, 0xeb7a6dbfuL, 0xda595295uL, 0x2d83bed4uL, 0xd3217458uL, 0x2969e049uL, 0x44c8c98euL,
    0x6a89c275uL, 0x78798ef4uL, 0x6b3e5899uL, 0xdd71b927uL, 0xb64fe1beuL, 0x17ad88f0uL, 0x66ac20c9uL, 0xb43ace7duL,
    0x184adf63uL, 0x82311ae5uL, 0x60335197uL, 0x457f5362uL, 0xe07764b1uL, 0x84ae6bbbuL, 0x1ca081feuL, 0x942b08f9uL,
    0x58684870uL, 0x19fd458fuL, 0x876cde94uL, 0xb7f87b52uL, 0x23d373abuL, 0xe2024b72uL, 0x578f1fe3uL, 0x2aab5566uL,
    0x0728ebb2uL, 0x03c2b52fuL, 0x9a7bc586uL, 0xa50837d3uL, 0xf2872830uL, 0xb2a5bf23uL, 0xba6a0302uL, 0x5c8216eduL,
    0x2b1ccf8auL, 0x92b479a7uL, 0xf0f207f3uL, 0xa1e2694euL, 0xcdf4da65uL, 0xd5be0506uL, 0x1f6234d1uL, 0x8afea6c4uL,
    0x9d532e34uL, 0xa055f3a2uL, 0x32e18a05uL, 0x75ebf6a4uL, 0x39ec830buL, 0xaaef6040uL, 0x069f715euL, 0x51106ebduL,
    0xf98a213euL, 0x3d06dd96uL, 0xae053edduL, 0x46bde64duL, 0xb58d5491uL, 0x055dc471uL, 0x6fd40604uL, 0xff155060uL,
    0x24fb9819uL, 0x97e9bdd6uL, 0xcc434089uL, 0x779ed967uL, 0xbd42e8b0uL, 0x888b8907uL, 0x385b19e7uL, 0xdbeec879uL,
    0x470a7ca1uL, 0xe90f427cuL, 0xc91e84f8uL, 0x00000000uL, 0x83868009uL, 0x48ed2b32uL, 0xac70111euL, 0x4e725a6cuL,
    0xfbff0efduL, 0x5638850fuL, 0x1ed5ae3duL, 0x27392d36uL, 0x64d90f0auL, 0x21a65c68uL, 0xd1545b9buL, 0x3a2e3624uL,
    0xb1670a0cuL, 0x0fe75793uL, 0xd296eeb4uL, 0x9e919b1buL, 0x4fc5c080uL, 0xa220dc61uL, 0x694b775auL, 0x161a121cuL,
    0x0aba93e2uL, 0xe52aa0c0uL, 0x43e0223cuL, 0x1d171b12uL, 0x0b0d090euL, 0xadc78bf2uL, 0xb9a8b62duL, 0xc8a91e14uL,
    0x8519f157uL, 0x4c0775afuL, 0xbbdd99eeuL, 0xfd607fa3uL, 0x9f2601f7uL, 0xbcf5725cuL, 0xc53b6644uL, 0x347efb5buL,
    0x7629438buL, 0xdcc623cbuL, 0x68fcedb6uL, 0x63f1e4b8uL, 0xcadc31d7uL, 0x10856342uL, 0x40229713uL, 0x2011c684uL,
    0x7d244a85uL, 0xf83dbbd2uL, 0x1132f9aeuL, 0x6da129c7uL, 0x4b2f9e1duL, 0xf330b2dcuL, 0xec52860duL, 0xd0e3c177uL,
    0x6c16b32buL, 0x99b970a9uL, 0xfa489411uL, 0x2264e947uL, 0xc48cfca8uL, 0x1a3ff0a0uL, 0xd82c7d56uL, 0xef903322uL,
    0xc74e4987uL, 0xc1d138d9uL, 0xfea2ca8cuL, 0x360bd498uL, 0xcf81f5a6uL, 0x28de7aa5uL, 0x268eb7dauL, 0xa4bfad3fuL,
    0xe49d3a2cuL, 0x0d927850uL, 0x9bcc5f6auL, 0x62467e54uL, 0xc2138df6uL, 0xe8b8d890uL, 0x5ef7392euL, 0xf5afc382uL,
    0xbe805d9fuL, 0x7c93d069uL, 0xa92dd56fuL, 0xb31225cfuL, 0x3b99acc8uL, 0xa77d1810uL, 0x6e639ce8uL, 0x7bbb3bdbuL,
    0x097826cduL, 0xf418596euL, 0x01b79aecuL, 0xa89a4f83uL, 0x656e95e6uL, 0x7ee6ffaauL, 0x08cfbc21uL, 0xe6e815efuL,
    0xd99be7bauL, 0xce366f4auL, 0xd4099feauL, 0xd67cb029uL, 0xafb2a431uL, 0x31233f2auL, 0x3094a5c6uL, 0xc066a235uL,
    0x37bc4e74uL, 0xa6ca82fcuL, 0xb0d090e0uL, 0x15d8a733uL, 0x4a9804f1uL, 0xf7daec41uL, 0x0e50cd7fuL, 0x2ff69117uL,
    0x8dd64d76uL, 0x4db0ef43uL, 0x544daaccuL, 0xdf0496e4uL, 0xe3b5d19euL, 0x1b886a4cuL, 0xb81f2cc1uL, 0x7f516546uL,
    0x04ea5e9duL, 0x5d358c01uL, 0x737487fauL, 0x2e410bfbuL, 0x5a1d67b3uL, 0x52d2db92uL, 0x335610e9uL, 0x1347d66duL,
    0x8c61d79auL, 0x7a0ca137uL, 0x8e14f859uL, 0x893c13ebuL, 0xee27a9ceuL, 0x35c961b7uL, 0xede51ce1uL, 0x3cb1477auL,
    0x59dfd29cuL, 0x3f73f255uL, 0x79ce1418uL, 0xbf37c773uL, 0xeacdf753uL, 0x5baafd5fuL, 0x146f3ddfuL, 0x86db4478uL,
    0x81f3afcauL, 0x3ec468b9uL, 0x2c342438uL, 0x5f40a3c2uL, 0x72c31d16uL, 0x0c25e2bcuL, 0x8b493c28uL, 0x41950dffuL,
    0x7101a839uL, 0xdeb30c08uL, 0x9ce4b4d8uL, 0x90c15664uL, 0x6184cb7buL, 0x70b632d5uL, 0x745c6c48uL, 0x4257b8d0uL
  },
  {
    0xa7f45150uL, 0x65417e53uL, 0xa4171ac3uL, 0x5e273a96uL, 0x6bab3bcbuL, 0x459d1ff1uL, 0x58faacabuL, 0x03e34b93uL,
    0xfa302055uL, 0x6d76adf6uL, 0x76cc8891uL, 0x4c02f525uL, 0xd7e54ffcuL, 0xcb2ac5d7uL, 0x44352680uL, 0xa362b58fuL,
    0x5ab1de49uL, 0x1bba2567uL, 0x0eea4598uL, 0xc0fe5de1uL, 0x752fc302uL, 0xf04c8112uL, 0x97468da3uL, 0xf9d36bc6uL,
    0x5f8f03e7uL, 0x9c921595uL, 0x7a6dbfebuL, 0x595295dauL, 0x83bed42duL, 0x217458d3uL, 0x69e04929uL, 0xc8c98e44uL,
    0x89c2756auL, 0x798ef478uL, 0x3e58996buL, 0x71b927dduL, 0x4fe1beb6uL, 0xad88f017uL, 0xac20c966uL, 0x3ace7db4uL,
    0x4adf6318uL, 0x311ae582uL, 0x33519760uL, 0x7f536245uL, 0x7764b1e0uL, 0xae6bbb84uL, 0xa081fe1cuL, 0x2b08f994uL,
    0x68487058uL, 0xfd458f19uL, 0x6cde9487uL, 0xf87b52b7uL, 0xd373ab23uL, 0x024b72e2uL, 0x8f1fe357uL, 0xab55662auL,
    0x28ebb207uL, 0xc2b52f03uL, 0x7bc5869auL, 0x0837d3a5uL, 0x872830f2uL, 0xa5bf23b2uL, 0x6a0302bauL, 0x8216ed5cuL,
    0x1ccf8a2buL, 0xb479a792uL, 0xf207f3f0uL, 0xe2694ea1uL, 0xf4da65cduL, 0xbe0506d5uL, 0x6234d11fuL, 0xfea6c48auL,
    0x532e349duL, 0x55f3a2a0uL, 0xe18a0532uL, 0xebf6a475uL, 0xec830b39uL, 0xef6040aauL, 0x9f715e06uL, 0x106ebd51uL,
    0x8a213ef9uL, 0x06dd963duL, 0x053eddaeuL, 0xbde64d46uL, 0x8d5491b5uL, 0x5dc47105uL, 0xd406046fuL, 0x155060ffuL,
    0xfb981924uL, 0xe9bdd697uL, 0x434089ccuL, 0x9ed96777uL, 0x42e8b0bduL, 0x8b890788uL, 0x5b19e738uL, 0xeec879dbuL,
    0x0a7ca147uL, 0x0f427ce9uL, 0x1e84f8c9uL, 0x00000000uL, 0x86800983uL, 0xed2b3248uL, 0x70111eacuL, 0x725a6c4euL,
    0xff0efdfbuL, 0x38850f56uL, 0xd5ae3d1euL, 0x392d3627uL, 0xd90f0a64uL, 0xa65c6821uL, 0x545b9bd1uL, 0x2e36243auL,
    0x670a0cb1uL, 0xe757930fuL, 0x96eeb4d2uL, 0x919b1b9euL, 0xc5c0804fuL, 0x20dc61a2uL, 0x4b775a69uL, 0x1a121c16uL,
    0xba93e20auL, 0x2aa0c0e5uL, 0xe0223c43uL, 0x171b121duL, 0x0d090e0buL, 0xc78bf2aduL, 0xa8b62db9uL, 0xa91e14c8uL,
    0x19f15785uL, 0x0775af4cuL, 0xdd99eebbuL, 0x607fa3fduL, 0x2601f79fuL, 0xf5725cbcuL, 0x3b6644c5uL, 0x7efb5b34uL,
    0x29438b76uL, 0xc623cbdcuL, 0xfcedb668uL, 0xf1e4b863uL, 0xdc31d7cauL, 0x85634210uL, 0x22971340uL, 0x11c68420uL,
    0x244a857duL, 0x3dbbd2f8uL, 0x32f9ae11uL, 0xa129c76duL, 0x2f9e1d4buL, 0x30b2dcf3uL, 0x52860decuL, 0xe3c177d0uL,
    0x16b32b6cuL, 0xb970a999uL, 0x489411fauL, 0x64e94722uL, 0x8cfca8c4uL, 0x3ff0a01auL, 0x2c7d56d8uL, 0x903322efuL,
    0x4e4987c7uL, 0xd138d9c1uL, 0xa2ca8cfeuL, 0x0bd49836uL, 0x81f5a6cfuL, 0xde7aa528uL, 0x8eb7da26uL, 0xbfad3fa4uL,
    0x9d3a2ce4uL, 0x9278500duL, 0xcc5f6a9buL, 0x467e5462uL, 0x138df6c2uL, 0xb8d890e8uL, 0xf7392e5euL, 0xafc382f5uL,
    0x805d9fbeuL, 0x93d0697cuL, 0x2dd56fa9uL, 0x1225cfb3uL, 0x99acc83buL, 0x7d1810a7uL, 0x639ce86euL, 0xbb3bdb7buL,
    0x7826cd09uL, 0x18596ef4uL, 0xb79aec01uL, 0x9a4f83a8uL, 0x6e95e665uL, 0xe6ffaa7euL, 0xcfbc2108uL, 0xe815efe6uL,
    0x9be7bad9uL, 0x366f4aceuL, 0x099fead4uL, 0x7cb029d6uL, 0xb2a431afuL, 0x233f2a31uL, 0x94a5c630uL, 0x66a235c0uL,
    0xbc4e7437uL, 0xca82fca6uL, 0xd090e0b0uL, 0xd8a73315uL, 0x9804f14auL, 0xdaec41f7uL, 0x50cd7f0euL, 0xf691172fuL,
    0xd64d768duL, 0xb0ef434duL, 0x4daacc54uL, 0x0496e4dfuL, 0xb5d19ee3uL, 0x886a4c1buL, 0x1f2cc1b8uL, 0x5165467fuL,
    0xea5e9d04uL, 0x358c015duL, 0x7487fa73uL, 0x410bfb2euL, 0x1d67b35auL, 0xd2db9252uL, 0x5610e933uL, 0x47d66d13uL,
    0x61d79a8cuL, 0x0ca1377auL, 0x14f8598euL, 0x3c13eb89uL, 0x27a9ceeeuL, 0xc961b735uL, 0xe51ce1eduL, 0xb1477a3cuL,
    0xdfd29c59uL, 0x73f2553fuL, 0xce141879uL, 0x37c773bfuL, 0xcdf753eauL, 0xaafd5f5buL, 0x6f3ddf14uL, 0xdb447886uL,
    0xf3afca81uL, 0xc468b93euL, 0x3424382cuL, 0x40a3c25fuL, 0xc31d1672uL, 0x25e2bc0cuL, 0x493c288buL, 0x950dff41uL,
    0x01a83971uL, 0xb30c08deuL, 0xe4b4d89cuL, 0xc1566490uL, 0x84cb7b61uL, 0xb632d570uL, 0x5c6c4874uL, 0x57b8d042uL
  },
  {
    0xf45150a7uL, 0x417e5365uL, 0x171ac3a4uL, 0x273a965euL, 0xab3bcb6buL, 0x9d1ff145uL, 0xfaacab58uL, 0xe34b9303uL,
    0x302055fauL, 0x76adf66duL, 0xcc889176uL, 0x02f5254cuL, 0xe54ffcd7uL, 0x2ac5d7cbuL, 0x35268044uL, 0x62b58fa3uL,
    0xb1de495auL, 0xba25671buL, 0xea45980euL, 0xfe5de1c0uL, 0x2fc30275uL, 0x4c8112f0uL, 0x468da397uL, 0xd36bc6f9uL,
    0x8f03e75fuL, 0x9215959cuL, 0x6dbfeb7auL, 0x5295da59uL, 0xbed42d83uL, 0x7458d321uL, 0xe0492969uL, 0xc98e44c8uL,
    0xc2756a89uL, 0x8ef47879uL, 0x58996b3euL, 0xb927dd71uL, 0xe1beb64fuL, 0x88f017aduL, 0x20c966acuL, 0xce7db43auL,
    0xdf63184auL, 0x1ae58231uL, 0x51976033uL, 0x5362457fuL, 0x64b1e077uL, 0x6bbb84aeuL, 0x81fe1ca0uL, 0x08f9942buL,
    0x48705868uL, 0x458f19fduL, 0xde94876cuL, 0x7b52b7f8uL, 0x73ab23d3uL, 0x4b72e202uL, 0x1fe3578fuL, 0x55662aabuL,
    0xebb20728uL, 0xb52f03c2uL, 0xc5869a7buL, 0x37d3a508uL, 0x2830f287uL, 0xbf23b2a5uL, 0x0302ba6auL, 0x16ed5c82uL,
    0xcf8a2b1cuL, 0x79a792b4uL, 0x07f3f0f2uL, 0x694ea1e2uL, 0xda65cdf4uL, 0x0506d5beuL, 0x34d11f62uL, 0xa6c48afeuL,
    0x2e349d53uL, 0xf3a2a055uL, 0x8a0532e1uL, 0xf6a475ebuL, 0x830b39ecuL, 0x6040aaefuL, 0x715e069fuL, 0x6ebd5110uL,
    0x213ef98auL, 0xdd963d06uL, 0x3eddae05uL, 0xe64d46bduL, 0x5491b58duL, 0xc471055duL, 0x06046fd4uL, 0x5060ff15uL,
    0x981924fbuL, 0xbdd697e9uL, 0x4089cc43uL, 0xd967779euL, 0xe8b0bd42uL, 0x8907888buL, 0x19e7385buL, 0xc879dbeeuL,
    0x7ca1470auL, 0x427ce90fuL, 0x84f8c91euL, 0x00000000uL, 0x80098386uL, 0x2b3248eduL, 0x111eac70uL, 0x5a6c4e72uL,
    0x0efdfbffuL, 0x850f5638uL, 0xae3d1ed5uL, 0x2d362739uL, 0x0f0a64d9uL, 0x5c6821a6uL, 0x5b9bd154uL, 0x36243a2euL,
    0x0a0cb167uL, 0x57930fe7uL, 0xeeb4d296uL, 0x9b1b9e91uL, 0xc0804fc5uL, 0xdc61a220uL, 0x775a694buL, 0x121c161auL,
    0x93e20abauL, 0xa0c0e52auL, 0x223c43e0uL, 0x1b121d17uL, 0x090e0b0duL, 0x8bf2adc7uL, 0xb62db9a8uL, 0x1e14c8a9uL,
    0xf1578519uL, 0x75af4c07uL, 0x99eebbdduL, 0x7fa3fd60uL, 0x01f79f26uL, 0x725cbcf5uL, 0x6644c53buL, 0xfb5b347euL,
    0x438b7629uL, 0x23cbdcc6uL, 0xedb668fcuL, 0xe4b863f1uL, 0x31d7cadcuL, 0x63421085uL, 0x97134022uL, 0xc6842011uL,
    0x4a857d24uL, 0xbbd2f83duL, 0xf9ae1132uL, 0x29c76da1uL, 0x9e1d4b2fuL, 0xb2dcf330uL, 0x860dec52uL, 0xc177d0e3uL,
    0xb32b6c16uL, 0x70a999b9uL, 0x9411fa48uL, 0xe9472264uL, 0xfca8c48cuL, 0xf0a01a3fuL, 0x7d56d82cuL, 0x3322ef90uL,
    0x4987c74euL, 0x38d9c1d1uL, 0xca8cfea2uL, 0xd498360buL, 0xf5a6cf81uL, 0x7aa528deuL, 0xb7da268euL, 0xad3fa4bfuL,
    0x3a2ce49duL, 0x78500d92uL, 0x5f6a9bccuL, 0x7e546246uL, 0x8df6c213uL, 0xd890e8b8uL, 0x392e5ef7uL, 0xc382f5afuL,
    0x5d9fbe80uL, 0xd0697c93uL, 0xd56fa92duL, 0x25cfb312uL, 0xacc83b99uL, 0x1810a77duL, 0x9ce86e63uL, 0x3bdb7bbbuL,
    0x26cd0978uL, 0x596ef418uL, 0x9aec01b7uL, 0x4f83a89auL, 0x95e6656euL, 0xffaa7ee6uL, 0xbc2108cfuL, 0x15efe6e8uL,
    0xe7bad99buL, 0x6f4ace36uL, 0x9fead409uL, 0xb029d67cuL, 0xa431afb2uL, 0x3f2a3123uL, 0xa5c63094uL, 0xa235c066uL,
    0x4e7437bcuL, 0x82fca6cauL, 0x90e0b0d0uL, 0xa73315d8uL, 0x04f14a98uL, 0xec41f7dauL, 0xcd7f0e50uL, 0x91172ff6uL,
    0x4d768dd6uL, 0xef434db0uL, 0xaacc544duL, 0x96e4df04uL, 0xd19ee3b5uL, 0x6a4c1b88uL, 0x2cc1b81fuL, 0x65467f51uL,
    0x5e9d04eauL, 0x8c015d35uL, 0x87fa7374uL, 0x0bfb2e41uL, 0x67b35a1duL, 0xdb9252d2uL, 0x10e93356uL, 0xd66d1347uL,
    0xd79a8c61uL, 0xa1377a0cuL, 0xf8598e14uL, 0x13eb893cuL, 0xa9ceee27uL, 0x61b735c9uL, 0x1ce1ede5uL, 0x477a3cb1uL,
    0xd29c59dfuL, 0xf2553f73uL, 0x141879ceuL, 0xc773bf37uL, 0xf753eacduL, 0xfd5f5baauL, 0x3ddf146fuL, 0x447886dbuL,
    0xafca81f3uL, 0x68b93ec4uL, 0x24382c34uL, 0xa3c25f40uL, 0x1d1672c3uL, 0xe2bc0c25uL, 0x3c288b49uL, 0x0dff4195uL,
    0xa8397101uL, 0x0c08deb3uL, 0xb4d89ce4uL, 0x566490c1uL, 0xcb7b6184uL, 0x32d570b6uL, 0x6c48745cuL, 0xb8d04257uL
  },
  {
    0x5150a7f4uL, 0x7e536541uL, 0x1ac3a417uL, 0x3a965e27uL, 0x3bcb6babuL, 0x1ff1459duL, 0xacab58fauL, 0x4b9303e3uL,
    0x2055fa30uL, 0xadf66d76uL, 0x889176ccuL, 0xf5254c02uL, 0x4ffcd7e5uL, 0xc5d7cb2auL, 0x26804435uL, 0xb58fa362uL,
    0xde495ab1uL, 0x25671bbauL, 0x45980eeauL, 0x5de1c0feuL, 0xc302752fuL, 0x8112f04cuL, 0x8da39746uL, 0x6bc6f9d3uL,
    0x03e75f8fuL, 0x15959c92uL, 0xbfeb7a6duL, 0x95da5952uL, 0xd42d83beuL, 0x58d32174uL, 0x492969e0uL, 0x8e44c8c9uL,
    0x756a89c2uL, 0xf478798euL, 0x996b3e58uL, 0x27dd71b9uL, 0xbeb64fe1uL, 0xf017ad88uL, 0xc966ac20uL, 0x7db43aceuL,
    0x63184adfuL, 0xe582311auL, 0x97603351uL, 0x62457f53uL, 0xb1e07764uL, 0xbb84ae6buL, 0xfe1ca081uL, 0xf9942b08uL,
    0x70586848uL, 0x8f19fd45uL, 0x94876cdeuL, 0x52b7f87buL, 0xab23d373uL, 0x72e2024buL, 0xe3578f1fuL, 0x662aab55uL,
    0xb20728ebuL, 0x2f03c2b5uL, 0x869a7bc5uL, 0xd3a50837uL, 0x30f28728uL, 0x23b2a5bfuL, 0x02ba6a03uL, 0xed5c8216uL,
    0x8a2b1ccfuL, 0xa792b479uL, 0xf3f0f207uL, 0x4ea1e269uL, 0x65cdf4dauL, 0x06d5be05uL, 0xd11f6234uL, 0xc48afea6uL,
    0x349d532euL, 0xa2a055f3uL, 0x0532e18auL, 0xa475ebf6uL, 0x0b39ec83uL, 0x40aaef60uL, 0x5e069f71uL, 0xbd51106euL,
    0x3ef98a21uL, 0x963d06dduL, 0xddae053euL, 0x4d46bde6uL, 0x91b58d54uL, 0x71055dc4uL, 0x046fd406uL, 0x60ff1550uL,
    0x1924fb98uL, 0xd697e9bduL, 0x89cc4340uL, 0x67779ed9uL, 0xb0bd42e8uL, 0x07888b89uL, 0xe7385b19uL, 0x79dbeec8uL,
    0xa1470a7cuL, 0x7ce90f42uL, 0xf8c91e84uL, 0x00000000uL, 0x09838680uL, 0x3248ed2buL, 0x1eac7011uL, 0x6c4e725auL,
    0xfdfbff0euL, 0x0f563885uL, 0x3d1ed5aeuL, 0x3627392duL, 0x0a64d90fuL, 0x6821a65cuL, 0x9bd1545buL, 0x243a2e36uL,
    0x0cb1670auL, 0x930fe757uL, 0xb4d296eeuL, 0x1b9e919buL, 0x804fc5c0uL, 0x61a220dcuL, 0x5a694b77uL, 0x1c161a12uL,
    0xe20aba93uL, 0xc0e52aa0uL, 0x3c43e022uL, 0x121d171buL, 0x0e0b0d09uL, 0xf2adc78buL, 0x2db9a8b6uL, 0x14c8a91euL,
    0x578519f1uL, 0xaf4c0775uL, 0xeebbdd99uL, 0xa3fd607fuL, 0xf79f2601uL, 0x5cbcf572uL, 0x44c53b66uL, 0x5b347efbuL,
    0x8b762943uL, 0xcbdcc623uL, 0xb668fceduL, 0xb863f1e4uL, 0xd7cadc31uL, 0x42108563uL, 0x13402297uL, 0x842011c6uL,
    0x857d244auL, 0xd2f83dbbuL, 0xae1132f9uL, 0xc76da129uL, 0x1d4b2f9euL, 0xdcf330b2uL, 0x0dec5286uL, 0x77d0e3c1uL,
    0x2b6c16b3uL, 0xa999b970uL, 0x11fa4894uL, 0x472264e9uL, 0xa8c48cfcuL, 0xa01a3ff0uL, 0x56d82c7duL, 0x22ef9033uL,
    0x87c74e49uL, 0xd9c1d138uL, 0x8cfea2cauL, 0x98360bd4uL, 0xa6cf81f5uL, 0xa528de7auL, 0xda268eb7uL, 0x3fa4bfaduL,
    0x2ce49d3auL, 0x500d9278uL, 0x6a9bcc5fuL, 0x5462467euL, 0xf6c2138duL, 0x90e8b8d8uL, 0x2e5ef739uL, 0x82f5afc3uL,
    0x9fbe805duL, 0x697c93d0uL, 0x6fa92dd5uL, 0xcfb31225uL, 0xc83b99acuL, 0x10a77d18uL, 0xe86e639cuL, 0xdb7bbb3buL,
    0xcd097826uL, 0x6ef41859uL, 0xec01b79auL, 0x83a89a4fuL, 0xe6656e95uL, 0xaa7ee6ffuL, 0x2108cfbcuL, 0xefe6e815uL,
    0xbad99be7uL, 0x4ace366fuL, 0xead4099fuL, 0x29d67cb0uL, 0x31afb2a4uL, 0x2a31233fuL, 0xc63094a5uL, 0x35c066a2uL,
    0x7437bc4euL, 0xfca6ca82uL, 0xe0b0d090uL, 0x3315d8a7uL, 0xf14a9804uL, 0x41f7daecuL, 0x7f0e50cduL, 0x172ff691uL,
    0x768dd64duL, 0x434db0efuL, 0xcc544daauL, 0xe4df0496uL, 0x9ee3b5d1uL, 0x4c1b886auL, 0xc1b81f2cuL, 0x467f5165uL,
    0x9d04ea5euL, 0x015d358cuL, 0xfa737487uL, 0xfb2e410buL, 0xb35a1d67uL, 0x9252d2dbuL, 0xe9335610uL, 0x6d1347d6uL,
    0x9a8c61d7uL, 0x377a0ca1uL, 0x598e14f8uL, 0xeb893c13uL, 0xceee27a9uL, 0xb735c961uL, 0xe1ede51cuL, 0x7a3cb147uL,
    0x9c59dfd2uL, 0x553f73f2uL, 0x1879ce14uL, 0x73bf37c7uL, 0x53eacdf7uL, 0x5f5baafduL, 0xdf146f3duL, 0x7886db44uL,
    0xca81f3afuL, 0xb93ec468uL, 0x382c3424uL, 0xc25f40a3uL, 0x1672c31duL, 0xbc0c25e2uL, 0x288b493cuL, 0xff41950duL,
    0x397101a8uL, 0x08deb30cuL, 0xd89ce4b4uL, 0x6490c156uL, 0x7b6184cbuL, 0xd570b632uL, 0x48745c6cuL, 0xd04257b8uL
  }
};
# endif

/* Array with numeric value 1 used to increase the counter in CTR mode */
VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) one[actAES_BLOCK_SIZE] =
{ 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x01U };

# define VSECPRIM_STOP_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 actAESInitializePrevBlock()
 **********************************************************************************************************************/
/*! \brief       Initialize info->prev_block with initialization vector or counter
 *  \details     This function initializes the initialization vector (CBC mode) or the counter (CTR mode) for the
 *               AES algorithm, stored in info->prev_block. If iv is a null pointer, all entries of prev_block are
 *               set to 0.
 *  \param[in,out] info    (In)  Pointer to AES context structure
 *                         (Out) AES context structure, prev_block initialized
 *  \param[in]     iv      Pointer to initialization vector (in CBC mode) or counter (in CTR mode) or a null pointer
 *  \pre         info must be a valid workspace pointer
 *               if iv is not a null pointer, the buffer referenced by iv must provide at least actAES_BLOCK_SIZE byte
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actAESInitializePrevBlock(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) iv);

/**********************************************************************************************************************
 actAESEncryptMultipleBlocksCBC()
 **********************************************************************************************************************/
/*! \brief       AES encrypt multiple blocks in CBC mode
 *  \details     This function performs AES encryption on multiple input blocks in CBC mode
 *  \param[in,out] info       (in)  Pointer to AES context structure
 *                            (out) Pointer to AES context structure
 *  \param[in]     in         Pointer to blocks to be encrypted
 *  \param[in,out] out_buffer (in)  Pointer to blocks to be encrypted
 *                            (out) Encrypted output blocks
 *  \param[in]     blocks     Number of blocks
 *  \param[in]     watchdog   Pointer to watchdog trigger function
 *  \pre         out buffer must point to a buffer with at least 16 bytes.
 *               info must be a valid workspace pointer
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actAESEncryptMultipleBlocksCBC(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer,
  actLengthType blocks,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void));

/**********************************************************************************************************************
 actAESDecryptMultipleBlocksCBC()
 **********************************************************************************************************************/
/*! \brief       AES decrypt multiple blocks in CBC mode
 *  \details     This function performs AES decryption on multiple input blocks in CBC mode
 *  \param[in,out] info       (in)  Pointer to AES context structure
 *                            (out) Pointer to AES context structure
 *  \param[in]     in         Pointer to blocks to be decrypted
 *  \param[in,out] out_buffer (in)  Pointer to blocks to be decrypted
 *                            (out) Decrypted output blocks
 *  \param[in]     blocks     Number of blocks
 *  \param[in]     watchdog   Pointer to watchdog trigger function
 *  \pre         info must be a valid workspace pointer
 *               out buffer must point to a buffer with at least 16 bytes.
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actAESDecryptMultipleBlocksCBC(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer,
  actLengthType blocks,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void));

/**********************************************************************************************************************
 actAESProcessMultipleBlocksCTR()
 **********************************************************************************************************************/
/*! \brief       AES encrypt/decrypt multiple blocks in CTR mode
 *  \details     This function performs AES encryption or decryption on multiple input blocks in CTR mode.
 *               (Encryption and decryption is the same in CTR mode.)
 *  \param[in,out] info       (in)  Pointer to AES context structure
 *                            (out) Pointer to AES context structure
 *  \param[in]     in         Pointer to blocks to be en- or decrypted
 *  \param[in,out] out_buffer (in)  Pointer to blocks to be en- or decrypted
 *                            (out) En- or decrypted output blocks
 *  \param[in]     blocks     Number of blocks
 *  \param[in]     watchdog   Pointer to watchdog trigger function
 *  \pre         info must be a valid workspace pointer
 *               out_buffer must point to a buffer with at least 16 bytes. If an incomplete block shall be
 *               en-/decrypted, out_buffer must point to a buffer with at least info->buffer_used bytes.
 *
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actAESProcessMultipleBlocksCTR(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer,
  actLengthType blocks,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void));

/**********************************************************************************************************************
 actAESCTRIncrementCounter()
 **********************************************************************************************************************/
/*! \brief      Increments counter for AES CTR mode
 *  \details    This function increments the value in counter by adding 1. This is required for AES CTR mode during
 *              en- and decryption. If adding 1 to the value in counter would create a carry, this carry is added
 *              to the least significant byte in counter.
 *  \param[in,out] counter (in)  pointer to buffer storing the original counter
 *                         (out) pointer to buffer, storing the incremented counter
 *  \pre         The buffer referenced by counter must provide at least actAES_BLOCK_SIZE byte.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actAESCTRIncrementCounter(
  VSECPRIM_P2VAR_PARA(actU8) counter);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  initializePrevBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actAESInitializePrevBlock(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) iv)
{
  /* Copy IV to workspace; If null pointer use a zero array */
  if (iv == NULL_PTR)
  {
    actMemset(info->prev_block, 0u, actAES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }
  else
  {
    actMemCpyByteArray(info->prev_block, iv, actAES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }
}

/**********************************************************************************************************************
 *  actAESEncryptMultipleBlocksCBC()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actAESEncryptMultipleBlocksCBC(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer,
  actLengthType blocks,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void)) /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */
{
  actLengthType bcount;
  actLengthType i;
  VSECPRIM_P2VAR_PARA(actU8) tempPrevBlock = info->prev_block;

  for (bcount = 0u; bcount < (blocks*actAES_BLOCK_SIZE); bcount += actAES_BLOCK_SIZE)
  {
    /* out_block = prev_block ^ in_block */
    for (i = bcount; i < (bcount + actAES_BLOCK_SIZE); ++i)
    {
      out_buffer[i] = (actU8)(tempPrevBlock[i - bcount] ^ in[i]); /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
    }
    /* out_block = Encrypt(out_block) */
    actAESEncryptBlock(info, &out_buffer[bcount], &out_buffer[bcount], watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* prev_block = out_block */
    for (i = 0u; i < actAES_BLOCK_SIZE; ++i)
    {
      tempPrevBlock[i] = out_buffer[bcount + i]; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
    }
  }
}

/**********************************************************************************************************************
 *  actAESDecryptMultipleBlocksCBC()
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
VSECPRIM_LOCAL_FUNC(void) actAESDecryptMultipleBlocksCBC(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer,
  actLengthType blocks,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void)) /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */
{
  actLengthType bcount, i;
  VSECPRIM_P2VAR_PARA(actU8) tempPrevBlock = info->prev_block;
  actU8 temp[actAES_BLOCK_SIZE];

  for (bcount = 0u; bcount < (blocks*actAES_BLOCK_SIZE); bcount += actAES_BLOCK_SIZE)
  {
    /* Create copy of input data to allow in-place decryption */
    actMemCpyByteArray(temp, &in[bcount], actAES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

    /* out_block = Decrypt(in_block) */
    actAESDecryptBlock(info, &in[bcount], &out_buffer[bcount], watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    /* out_block = prev_block ^ out_block */
    /* prev_block = in_block */
    for (i = 0u; i < actAES_BLOCK_SIZE; ++i)
    {
      out_buffer[bcount + i] ^= tempPrevBlock[i]; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
      tempPrevBlock[i] = temp[i]; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
    }

  }
}

/**********************************************************************************************************************
 *  actAESProcessMultipleBlocksCTR()
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
VSECPRIM_LOCAL_FUNC(void) actAESProcessMultipleBlocksCTR(
  VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) in,
  VSECPRIM_P2VAR_PARA(actU8) out_buffer,
  actLengthType blocks,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void)) /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */
{
  actLengthType bcount, i, overallInLength;
  VSECPRIM_P2VAR_PARA(actU8) tempPrevBlock = info->prev_block;
  actU8 encryptedCounter[actAES_BLOCK_SIZE];

  for (bcount = 0u; bcount < (blocks*actAES_BLOCK_SIZE); bcount += actAES_BLOCK_SIZE)
  {
    actAESEncryptBlock(info, tempPrevBlock, encryptedCounter, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

    /* overallInLength = number of bytes in in_block that shall be xor-ed with encryptedCounter */
    if ((blocks == 1u) && ((info->buffer_used % actAES_BLOCK_SIZE) != 0u))
    {
      /* in_block not necessarily a complete block, usually the case during the call to esl_finalizeEncryptAES */
      overallInLength = info->buffer_used;
    }
    else
    {
      /* in_block is one or more complete blocks */
      overallInLength = bcount + actAES_BLOCK_SIZE;
    }

    /* out_block = encryptedCounter ^ in_block */
    for (i = bcount; i < overallInLength; ++i)
    {
      out_buffer[i] = (actU8)(encryptedCounter[i - bcount] ^ in[i]); /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
    }
    /* increment counter */
    actAESCTRIncrementCounter(tempPrevBlock); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }
}

/**********************************************************************************************************************
 *  actAESCTRIncrementCounter()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actAESCTRIncrementCounter(VSECPRIM_P2VAR_PARA(actU8) counter)
{
  actLengthType j;
  actU16 tmp = 0u;

  /* increase counter by adding 1 */
  for (j = 1u; j <= actAES_BLOCK_SIZE; j++)
  {
    tmp = ((actU16)counter[actAES_BLOCK_SIZE - j]) + one[actAES_BLOCK_SIZE - j] + actGetU8fromU16(tmp, 2u);
    counter[actAES_BLOCK_SIZE - j] = actGetLowByteFromU16(tmp); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
  }
  /* If there is a carry, add it to the least significant byte of counter */
  counter[actAES_BLOCK_SIZE - 1u] += actGetU8fromU16(tmp, 2u); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  actAESEncryptionRound()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actAESEncryptionRound(VSECPRIM_P2VAR_PARA(actU32) bo, VSECPRIM_P2CONST_PARA(actU32) bi, VSECPRIM_P2CONST_PARA(actU32) k)
{
  /* Performing an AES encryption round according to the AES spec */

# if (actAES_SPEED_UP == VSECPRIM_SPEED_UP_LEVEL_0) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  actU32 t;
  actU8 i, n;
  for (n = 0u; n < 4u; ++n)
  {
    bo[n] = k[n];
    for (i = 0u; i < 4u; ++i)
    {
      t = sbx_tab[actGetByte(bi[(n + i) & 3u], i)];
      t = ((((ff_mult_2(t)) | (t << 8u)) | (t << 16u)) | (ff_mult_3(t) << 24u));
      bo[n] ^= ((i == 0u) ? t : actRotl(t, ((actU32)i << 3u)));
    }
  }

# elif (actAES_SPEED_UP == VSECPRIM_SPEED_UP_LEVEL_1) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  actU32 t[4];
  actU8 n;

  for (n = 0u; n < 4u; ++n)
  {
    t[0] = ACT_ROM_R_BYT(sbx_tab[actGetByte(bi[n], 0u)]);                                     /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
    t[0] = ((((ff_mult_2(t[0])) | (t[0] << 8u)) | (t[0] << 16u)) | (ff_mult_3(t[0]) << 24u));
    t[1] = ACT_ROM_R_BYT(sbx_tab[actGetByte(bi[(n + 1u) & 3u], 1u)]);
    t[1] = ((((ff_mult_3(t[1])) | (ff_mult_2(t[1]) << 8u)) | (t[1] << 16u)) | (t[1] << 24u));
    t[2] = ACT_ROM_R_BYT(sbx_tab[actGetByte(bi[(n + 2u) & 3u], 2u)]);
    t[2] = ((((t[2]) | (ff_mult_3(t[2]) << 8u)) | (ff_mult_2(t[2]) << 16u)) | (t[2] << 24u));
    t[3] = ACT_ROM_R_BYT(sbx_tab[actGetByte(bi[(n + 3u) & 3u], 3u)]);                         /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
    t[3] = ((((t[3]) | (t[3] << 8u)) | (ff_mult_3(t[3]) << 16u)) | (ff_mult_2(t[3]) << 24u));
    bo[n] = t[0] ^ t[1] ^ t[2] ^ t[3] ^ k[n];
  }

# elif (actAES_SPEED_UP == VSECPRIM_SPEED_UP_LEVEL_2) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  actU8 n;

  for (n=0u; n < 4u; ++n)
  {
    bo[n] = ft_tab[0][actGetByte(bi[n], 0u)] ^                /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
            ft_tab[1][actGetByte(bi[(n+1u)&3u], 1u)] ^
            ft_tab[2][actGetByte(bi[(n+2u)&3u], 2u)] ^
            ft_tab[3][actGetByte(bi[(n+3u)&3u], 3u)] ^ k[n];  /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
  }

# elif (actAES_SPEED_UP==3)
   bo[0] =  ft_tab[0][actGetByte(bi[0], 0u)] ^       /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            ft_tab[1][actGetByte(bi[1], 1u)] ^
            ft_tab[2][actGetByte(bi[2], 2u)] ^
            ft_tab[3][actGetByte(bi[3], 3u)] ^ k[0]; /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
   bo[1] =  ft_tab[0][actGetByte(bi[1], 0u)] ^       /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            ft_tab[1][actGetByte(bi[2], 1u)] ^
            ft_tab[2][actGetByte(bi[3], 2u)] ^
            ft_tab[3][actGetByte(bi[0], 3u)] ^ k[1]; /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
   bo[2] =  ft_tab[0][actGetByte(bi[2], 0u)] ^       /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            ft_tab[1][actGetByte(bi[3], 1u)] ^
            ft_tab[2][actGetByte(bi[0], 2u)] ^
            ft_tab[3][actGetByte(bi[1], 3u)] ^ k[2];  /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
   bo[3] =  ft_tab[0][actGetByte(bi[3], 0u)] ^        /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            ft_tab[1][actGetByte(bi[0], 1u)] ^
            ft_tab[2][actGetByte(bi[1], 2u)] ^
            ft_tab[3][actGetByte(bi[2], 3u)] ^ k[3];  /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
# else
#  error actAES_SPEED_UP value not supported
# endif
}

/**********************************************************************************************************************
 *  actAESDecryptionRound()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actAESDecryptionRound(VSECPRIM_P2VAR_PARA(actU32) bo, VSECPRIM_P2CONST_PARA(actU32) bi, VSECPRIM_P2CONST_PARA(actU32) k)
{
  /* Performing a AES decryption round according to the AES spec */

# if (actAES_SPEED_UP == VSECPRIM_SPEED_UP_LEVEL_0) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  actU32 t;
  actU8 i, n;
  for (n = 0u; n < 4u; ++n)
  {
    bo[n] = k[n];
    for (i = 0u; i < 4u; ++i)
    {
      t = isb_tab[actGetByte(bi[((actU8)(n - i)) & 3u], i)];
      t = ((((ff_mult_14(t)) | (ff_mult_9(t) << 8u)) | (ff_mult_13(t) << 16u)) | (ff_mult_11(t) << 24u));
      bo[n] ^= ((i == 0u) ? t : actRotl(t, ((actU32)i << 3u)));
    }
  }

# elif (actAES_SPEED_UP == VSECPRIM_SPEED_UP_LEVEL_1) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  actU32 t[4];
  actU8 n;

  for (n = 0u; n < 4u; ++n)
  {
    t[0] = ACT_ROM_R_BYT(isb_tab[actGetByte(bi[n], 0u)]);                                                               /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
    t[0] = ((((ff_mult_14(t[0])) | (ff_mult_9(t[0]) << 8u)) | (ff_mult_13(t[0]) << 16u)) | (ff_mult_11(t[0]) << 24u));
    t[1] = ACT_ROM_R_BYT(isb_tab[actGetByte(bi[(n + 3u) & 3u], 1u)]);
    t[1] = ((((ff_mult_11(t[1])) | (ff_mult_14(t[1]) << 8u)) | (ff_mult_9(t[1]) << 16u)) | (ff_mult_13(t[1]) << 24u));
    t[2] = ACT_ROM_R_BYT(isb_tab[actGetByte(bi[(n + 2u) & 3u], 2u)]);
    t[2] = ((((ff_mult_13(t[2])) | (ff_mult_11(t[2]) << 8u)) | (ff_mult_14(t[2]) << 16u)) | (ff_mult_9(t[2]) << 24u));
    t[3] = ACT_ROM_R_BYT(isb_tab[actGetByte(bi[(n + 1u) & 3u], 3u)]);                                                   /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
    t[3] = ((((ff_mult_9(t[3])) | (ff_mult_13(t[3]) << 8u)) | (ff_mult_11(t[3]) << 16u)) | (ff_mult_14(t[3]) << 24u));
    bo[n] = t[0] ^ t[1] ^ t[2] ^ t[3] ^ k[n];
  }

# elif (actAES_SPEED_UP == VSECPRIM_SPEED_UP_LEVEL_2) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  actU8 n;

  for (n = 0u; n < 4u; ++n)
  {
    bo[n] = it_tab[0][actGetByte(bi[n], 0u)] ^                /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
            it_tab[1][actGetByte(bi[(n+3u)&3u], 1u)] ^
            it_tab[2][actGetByte(bi[(n+2u)&3u], 2u)] ^
            it_tab[3][actGetByte(bi[(n+1u)&3u], 3u)] ^ k[n];  /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
  }

# elif (actAES_SPEED_UP==3)
   bo[0] =  it_tab[0][actGetByte(bi[0], 0u)] ^       /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            it_tab[1][actGetByte(bi[3], 1u)] ^
            it_tab[2][actGetByte(bi[2], 2u)] ^
            it_tab[3][actGetByte(bi[1], 3u)] ^ k[0]; /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
   bo[1] =  it_tab[0][actGetByte(bi[1], 0u)] ^       /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            it_tab[1][actGetByte(bi[0], 1u)] ^
            it_tab[2][actGetByte(bi[3], 2u)] ^
            it_tab[3][actGetByte(bi[2], 3u)] ^ k[1]; /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
   bo[2] =  it_tab[0][actGetByte(bi[2], 0u)] ^       /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            it_tab[1][actGetByte(bi[1], 1u)] ^
            it_tab[2][actGetByte(bi[0], 2u)] ^
            it_tab[3][actGetByte(bi[3], 3u)] ^ k[2]; /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
   bo[3] =  it_tab[0][actGetByte(bi[3], 0u)] ^       /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            it_tab[1][actGetByte(bi[2], 1u)] ^
            it_tab[2][actGetByte(bi[1], 2u)] ^
            it_tab[3][actGetByte(bi[0], 3u)] ^ k[3]; /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
# else
#  error actAES_SPEED_UP value not supported
# endif
}

/**********************************************************************************************************************
 *  actAESEncryptionFinalRound()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actAESEncryptionFinalRound(VSECPRIM_P2VAR_PARA(actU32) bo, VSECPRIM_P2CONST_PARA(actU32) bi, VSECPRIM_P2CONST_PARA(actU32) k)
{
  /* Performing final AES encryption round according to the AES spec */

# if (actAES_SPEED_UP <= VSECPRIM_SPEED_UP_LEVEL_1) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  actU8 i, n;
  for (n = 0u; n < 4u; ++n)
  {
    bo[n] = k[n];
    for (i = 0u; i < 4u; ++i)
    {
      bo[n] ^= (((actU32) ACT_ROM_R_BYT(sbx_tab[actGetByte(bi[(n + i) & 3u], i)])) << (i << 3u));
    }
  }

# elif (actAES_SPEED_UP == VSECPRIM_SPEED_UP_LEVEL_2) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
   actU8 n;
   for (n=0u; n < 4u; ++n)
   {
     /* PRQA S 2985 4 */ /* MD_VSECPRIM_REDUNDANT_OP */
      bo[n] =  (actU32)sbx_tab[actGetByte(bi[n], 0u)] ^
               actRotl(((actU32)sbx_tab[actGetByte(bi[(n+1u)&3u], 1u)]),  8u) ^
               actRotl(((actU32)sbx_tab[actGetByte(bi[(n+2u)&3u], 2u)]), 16u) ^
               actRotl(((actU32)sbx_tab[actGetByte(bi[(n+3u)&3u], 3u)]), 24u) ^ k[n];
   }

# elif (actAES_SPEED_UP==3)
   /* PRQA S 2985 16 */ /* MD_VSECPRIM_REDUNDANT_OP */
   bo[0] =           (actU32)sbx_tab[actGetByte(bi[0], 0u)] ^ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            actRotl(((actU32)sbx_tab[actGetByte(bi[1], 1u)]),  8u) ^         /*lint !e572 */
            actRotl(((actU32)sbx_tab[actGetByte(bi[2], 2u)]), 16u) ^         /*lint !e572 */
            actRotl(((actU32)sbx_tab[actGetByte(bi[3], 3u)]), 24u) ^ k[0];   /*lint !e572 */
   bo[1] =           (actU32)sbx_tab[actGetByte(bi[1], 0u)] ^ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            actRotl(((actU32)sbx_tab[actGetByte(bi[2], 1u)]),  8u) ^         /*lint !e572 */
            actRotl(((actU32)sbx_tab[actGetByte(bi[3], 2u)]), 16u) ^         /*lint !e572 */
            actRotl(((actU32)sbx_tab[actGetByte(bi[0], 3u)]), 24u) ^ k[1];   /*lint !e572 */
   bo[2] =           (actU32)sbx_tab[actGetByte(bi[2], 0u)] ^ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            actRotl(((actU32)sbx_tab[actGetByte(bi[3], 1u)]),  8u) ^         /*lint !e572 */
            actRotl(((actU32)sbx_tab[actGetByte(bi[0], 2u)]), 16u) ^         /*lint !e572 */
            actRotl(((actU32)sbx_tab[actGetByte(bi[1], 3u)]), 24u) ^ k[2];   /*lint !e572 */
   bo[3] =           (actU32)sbx_tab[actGetByte(bi[3], 0u)] ^ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            actRotl(((actU32)sbx_tab[actGetByte(bi[0], 1u)]),  8u) ^         /*lint !e572 */
            actRotl(((actU32)sbx_tab[actGetByte(bi[1], 2u)]), 16u) ^         /*lint !e572 */
            actRotl(((actU32)sbx_tab[actGetByte(bi[2], 3u)]), 24u) ^ k[3];  /*lint !e572 */
# else
#  error actAES_SPEED_UP value not supported
# endif
}

/**********************************************************************************************************************
 *  actAESDecryptionFinalRound()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actAESDecryptionFinalRound(VSECPRIM_P2VAR_PARA(actU32) bo, VSECPRIM_P2CONST_PARA(actU32) bi, VSECPRIM_P2CONST_PARA(actU32) k)
{
  /* Performing final AES decryption round according to the AES spec */
# if (actAES_SPEED_UP <= VSECPRIM_SPEED_UP_LEVEL_1) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  actU8 i, n;
  for (n = 0u; n < 4u; ++n)
  {
    bo[n] = k[n];
    for (i = 0u; i < 4u; ++i)
    {
      bo[n] ^= (((actU32) ACT_ROM_R_BYT(isb_tab[actGetByte(bi[((actU8)(n - i)) & 3u], i)])) << (i << 3u));
    }
  }

# elif (actAES_SPEED_UP == VSECPRIM_SPEED_UP_LEVEL_2) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
   actU8 n;
   for (n = 0u; n < 4u; ++n)
   {
     /* PRQA S 2985 4 */ /* MD_VSECPRIM_REDUNDANT_OP */
      bo[n] =           (actU32)isb_tab[actGetByte(bi[n], 0u)] ^
               actRotl(((actU32)isb_tab[actGetByte(bi[(n+3u) & 3u], 1u)]),  8u) ^
               actRotl(((actU32)isb_tab[actGetByte(bi[(n+2u) & 3u], 2u)]), 16u) ^
               actRotl(((actU32)isb_tab[actGetByte(bi[(n+1u) & 3u], 3u)]), 24u) ^ k[n];
   }

# elif (actAES_SPEED_UP == 3)
   /* PRQA S 2985 16 */ /* MD_VSECPRIM_REDUNDANT_OP */
   bo[0] =           (actU32)isb_tab[actGetByte(bi[0], 0u)] ^ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            actRotl(((actU32)isb_tab[actGetByte(bi[3], 1u)]),  8u) ^         /*lint !e572 */
            actRotl(((actU32)isb_tab[actGetByte(bi[2], 2u)]), 16u) ^         /*lint !e572 */
            actRotl(((actU32)isb_tab[actGetByte(bi[1], 3u)]), 24u) ^ k[0];   /*lint !e572 */
   bo[1] =           (actU32)isb_tab[actGetByte(bi[1], 0u)] ^ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            actRotl(((actU32)isb_tab[actGetByte(bi[0], 1u)]),  8u) ^         /*lint !e572 */
            actRotl(((actU32)isb_tab[actGetByte(bi[3], 2u)]), 16u) ^         /*lint !e572 */
            actRotl(((actU32)isb_tab[actGetByte(bi[2], 3u)]), 24u) ^ k[1];   /*lint !e572 */
   bo[2] =           (actU32)isb_tab[actGetByte(bi[2], 0u)] ^ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            actRotl(((actU32)isb_tab[actGetByte(bi[1], 1u)]),  8u) ^         /*lint !e572 */
            actRotl(((actU32)isb_tab[actGetByte(bi[0], 2u)]), 16u) ^         /*lint !e572 */
            actRotl(((actU32)isb_tab[actGetByte(bi[3], 3u)]), 24u) ^ k[2];   /*lint !e572 */
   bo[3] =           (actU32)isb_tab[actGetByte(bi[3], 0u)] ^ /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
            actRotl(((actU32)isb_tab[actGetByte(bi[2], 1u)]),  8u) ^         /*lint !e572 */
            actRotl(((actU32)isb_tab[actGetByte(bi[1], 2u)]), 16u) ^         /*lint !e572 */
            actRotl(((actU32)isb_tab[actGetByte(bi[0], 3u)]), 24u) ^ k[3];   /*lint !e572 */
# else
#  error actAES_SPEED_UP value not supported
# endif
}

/**********************************************************************************************************************
 *  actAESKeySetup()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actAESKeySetup(VSECPRIM_P2VAR_PARA(actAESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void)) /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */
{
  actRETURNCODE retVal = actOK;

  if (key == NULL_PTR)
  {
    retVal = actEXCEPTION_NULL;
  }
  else if ((key_len != 16u) && (key_len != 24u) && (key_len != 32u))
  {
    retVal = actEXCEPTION_LENGTH;
  }
  else
  {
    /* encryption key expansion */
    VSECPRIM_P2VAR_PARA(actU32) e_key = info->e_key; /* PRQA S 0781 */ /* MD_VSECPRIM_5.6 */
    actU32 t;
    actLengthType i, j, key_dword_len, iterations; /* PRQA S 0781 */ /* MD_VSECPRIM_5.6 */

    /* read key into dwords */
    key_dword_len = key_len / 4u;
    info->key_dword_len = key_dword_len; /* SBSW_VSECPRIM_WRITE_POINTER */
    for (i = 0u; i < key_dword_len; ++i)
    {
      e_key[i] = key[i * 4u] | ((actU32) (key[((i * 4u) + 1u)]) << 8u) | ((actU32) (key[((i * 4u) + 2u)]) << 16u) | ((actU32) (key[((i * 4u) + 3u)]) << 24u); /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
    }

    /* key expansion loop */
    iterations = (((4u * key_dword_len) + 24u) / key_dword_len) % 11u;
    t = e_key[key_dword_len - 1u];
    for (i = 0u; (i < (iterations % 11u)); ++i)
    {
      actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

      t = actRotr(t, 8u);
      t = ls_box(t) ^ ACT_ROM_R_DWD(rco_tab[(i % 10u)]);  /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
      for (j = 0u; j < key_dword_len; ++j)
      {
        if ((j == 4u) && (key_dword_len == 8u))
        {
          t = e_key[((8u * i) + 4u) % 64u] ^ ls_box(t); /* PRQA S 2985 */ /* MD_VSECPRIM_REDUNDANT_OP */
          e_key[((8u * i) + 12u) % 64u] = t; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
        }
        else
        {
          t ^= e_key[(key_dword_len * i) + j];
          e_key[(key_dword_len * i) + key_dword_len + j] = t; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
        }
      }
    }

    actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
  }

  return retVal;
} /* PRQA S 6080 */ /* MD_MSR_STMIF */

/**********************************************************************************************************************
 *  actAESMakeDecryptionKey()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actAESMakeDecryptionKey(VSECPRIM_P2VAR_PARA(actAESSTRUCT) info)
{
  VSECPRIM_P2VAR_PARA(actU32) tempKey = info->e_key;
  actU32 t, u, v, w;
  actLengthType i, iterations = (4u * info->key_dword_len) + 24u;

  for (i = 4u; i < iterations; ++i)
  {
    u = star_x(tempKey[i]);
    v = star_x(u);
    w = star_x(v);
    t = w ^ tempKey[i];
    tempKey[i] = u ^ v ^ w ^ actRotr(u ^ t, 8) ^ actRotr(v ^ t, 16) ^ actRotr(t, 24); /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  }
}

/**********************************************************************************************************************
 *  actAESInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actAESInit(VSECPRIM_P2VAR_PARA(actAESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_len,
                                        VSECPRIM_P2CONST_PARA(actU8) iv, actU8 mode, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void)) /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */
{
  actRETURNCODE result = actOK;
  /* initialization mode, buffers, etc. */
  info->mode = mode; /* SBSW_VSECPRIM_WRITE_POINTER */
  info->buffer_used = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */

  switch (mode & actAES_BM_MASK)
  {
    case actAES_BM_ECB:
    {
      break;
    }
    case actAES_BM_CBC:
    case actAES_BM_CTR:
    {
      actAESInitializePrevBlock(info, iv); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
    }
    default:
    {
      result = actEXCEPTION_ALGOID;

      break;
    }
  }

  /* Key setup */
  if (result == actOK)
  {
    result = actAESKeySetup(info, key, key_len, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return result;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  actAESEncryptMultipleBlocks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actAESEncryptMultipleBlocks(VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
                                                VSECPRIM_P2CONST_PARA(actU8) in,
                                                VSECPRIM_P2VAR_PARA(actU8) out_buffer,
                                                actLengthType blocks,
                                                VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void)) /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */
{
  actLengthType bcount;

  switch (info->mode & actAES_BM_MASK)
  {
    case actAES_BM_CBC:
    {
      actAESEncryptMultipleBlocksCBC(info, in, out_buffer, blocks, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
    }
    case actAES_BM_CTR:
    {
      actAESProcessMultipleBlocksCTR(info, in, out_buffer, blocks, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
    }
    /* ECB */
    default:
    {
      for (bcount = 0u; bcount < (blocks*actAES_BLOCK_SIZE); bcount += actAES_BLOCK_SIZE)
      {
        /* out_block = Encrypt(in_block) */
        actAESEncryptBlock(info, &in[bcount], &out_buffer[bcount], watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
      }
      break;
    }
  }
} /* PRQA S 6030 */ /* MD_MSR_STCYC */

/**********************************************************************************************************************
 *  actAESDecryptMultipleBlocks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actAESDecryptMultipleBlocks(VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
                                                VSECPRIM_P2CONST_PARA(actU8) in,
                                                VSECPRIM_P2VAR_PARA(actU8) out_buffer,
                                                actLengthType blocks,
                                                VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void)) /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */
{
  actLengthType bcount;

  switch (info->mode & actAES_BM_MASK)
  {
    case actAES_BM_CBC:
    {
      actAESDecryptMultipleBlocksCBC(info, in, out_buffer, blocks, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
    }
    case actAES_BM_CTR:
    {
      actAESProcessMultipleBlocksCTR(info, in, out_buffer, blocks, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
    }
    /* ECB */
    default:
    {
      for (bcount = 0u; bcount < (blocks*actAES_BLOCK_SIZE); bcount += actAES_BLOCK_SIZE)
      {
        /* out_block = Decrypt(in_block) */
        actAESDecryptBlock(info, &in[bcount], &out_buffer[bcount], watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
      }

      break;
    }
  }
} /* PRQA S 6030 */ /* MD_MSR_STCYC */

/**********************************************************************************************************************
 *  actAESTransform()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(void) actAESTransform(VSECPRIM_P2VAR_PARA(actAESSTRUCT) info,
                                    VSECPRIM_P2CONST_PARA(actU8) in,
                                    actLengthType in_length,
                                    VSECPRIM_P2VAR_PARA(actU8) out_buffer,
                                    actAESTransformFctType transform,
                                    VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void)) /* PRQA S 0850 */ /* MD_MSR_MacroArgumentEmpty */
{
  actLengthType blocks, bytes, diff = 0u;
  actU8 transformBlock = TRUE;
# ifndef actNO_INDIRECT_CALLS
  actU8  out_buffer_idx = 0;
# endif

  /* Handle left overs from last transform */
  if (info->buffer_used > 0u)
  {
    diff = actMin(in_length, (actLengthType)(actAES_BLOCK_SIZE - info->buffer_used));
    actMemCpyByteArray(&info->buffer[info->buffer_used], in, diff); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    info->buffer_used += diff; /* SBSW_VSECPRIM_WRITE_POINTER */
    if (info->buffer_used < actAES_BLOCK_SIZE)
    {
      /* no complete block */
      transformBlock = FALSE;
    }
    else
    {
      /* transform buffered block */
# ifdef actNO_INDIRECT_CALLS
      if (transform == actAESEncryptMultipleBlocks)
      {
        actAESEncryptMultipleBlocks(info, info->buffer, out_buffer, 1, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }
      else
      {
        actAESDecryptMultipleBlocks(info, info->buffer, out_buffer, 1, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }
# else
      transform(info, info->buffer, out_buffer, 1, watchdog); /* SBSW_VSECPRIM_FUNCTION_POINTER_CALL_TRANSFORM */
      out_buffer_idx = actAES_BLOCK_SIZE;
# endif
    }
  }

  if (transformBlock == TRUE)
  {
    /* transform blocks */
    blocks = (in_length - diff) / actAES_BLOCK_SIZE;
    bytes = actAES_BLOCK_SIZE * blocks;
# ifdef actNO_INDIRECT_CALLS
    if (transform == actAESEncryptMultipleBlocks)
    {
      actAESEncryptMultipleBlocks(info, &in[diff], out_buffer, blocks, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
    else
    {
      actAESDecryptMultipleBlocks(info, &in[diff], out_buffer, blocks, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
# else
    transform(info, &in[diff], &out_buffer[out_buffer_idx], blocks, watchdog); /* SBSW_VSECPRIM_FUNCTION_POINTER_CALL_TRANSFORM */
# endif

    /* copy left overs into info->buffer */
    info->buffer_used = in_length - (diff + bytes); /* SBSW_VSECPRIM_WRITE_POINTER */
    actMemCpyByteArray(info->buffer, &in[diff + bytes], info->buffer_used); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  }
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTAES_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actAES.c
 *********************************************************************************************************************/
