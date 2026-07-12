/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2006 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actDES.c
 *        \brief  This file contains a DES implementation.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library cvActLib/ES
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

#define ACTDES_SOURCE

#include "actDES.h"
#include "actUtilities.h"
#include "actWatchdog.h"
#include "actConfig.h"

#if (VSECPRIM_ACTDES_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

 /* PRQA S 6010, 6030, 6060 EOF */ /* MD_VSECPRIM_QM_ONLY_METRIC */
 /* PRQA S 2889, 2024 EOF */ /* MD_VSECPRIM_QM_ONLY_RETURN_PATHS */
 /* PRQA S 3200 EOF */ /* MD_VSECPRIM_QM_ONLY_UNUSED_FUNCTION */
 /* PRQA S 2981, 2983, 2985 EOF */ /* MD_VSECPRIM_REDUNDANT_ASSIGNMENT */
 /* PRQA S 0488 EOF */ /* MD_VSECPRIM_QM_ONLY_POINTER_ARITHMETIC */
 /* PRQA S 3387, 3440, 1338 EOF */ /* MD_VSECPRIM_QM_ONLY_SHORT_DECREMENT_INCREMENT_OPERATION */
 /* PRQA S 2463, 3418 EOF */ /* MD_VSECPRIM_QM_ONLY_VARIABLE_IN_LOOP */
 /* PRQA S 3218 EOF */ /* MD_VSECPRIM_8.7 */
 /* PRQA S 3673, 3678 EOF */ /* MD_VSECPRIM_CONST_POINTER */
 /* PRQA S 0310, 3305 EOF */ /* MD_VSECPRIM_QM_ONLY_POINTER_CAST_TO_LARGER_TYPE */

/****************************************************************************
 ** Look up tables
 ***************************************************************************/
# define VSECPRIM_START_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) PC1[56] =
{ /* permutation choice 1  value-1 */
      56, 48, 40, 32, 24, 16,  8,  0, 57, 49, 41, 33, 25, 17,
       9,  1, 58, 50, 42, 34, 26, 18, 10,  2, 59, 51, 43, 35,
      62, 54, 46, 38, 30, 22, 14,  6, 61, 53, 45, 37, 29, 21,
      13,  5, 60, 52, 44, 36, 28, 20, 12,  4, 27, 19, 11,  3
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) PC2[48] =
{ /* permutation choice 2  value-1 */
      13, 16, 10, 23,  0,  4,  2, 27, 14,  5, 20,  9,
      22, 18, 11,  3, 25,  7, 15,  6, 26, 19, 12,  1,
      40, 51, 30, 36, 46, 54, 29, 39, 50, 44, 32, 47,
      43, 48, 38, 55, 33, 52, 45, 41, 49, 35, 28, 31
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) bytebit[8] =
{ /* bit Mask: MSB to LSB */
      0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) bigbyte[24] =
{ /* bit Mask: MSB to LSB */
      0x00800000, 0x00400000, 0x00200000, 0x00100000,
      0x00080000, 0x00040000, 0x00020000, 0x00010000,
      0x00008000, 0x00004000, 0x00002000, 0x00001000,
      0x00000800, 0x00000400, 0x00000200, 0x00000100,
      0x00000080, 0x00000040, 0x00000020, 0x00000010,
      0x00000008, 0x00000004, 0x00000002, 0x00000001
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU8) totrotation[16] =
{
      1, 2, 4, 6, 8, 10, 12, 14, 15, 17, 19, 21, 23, 25, 27, 28
};

/****************************************************************************
 ** SBox tables
 ***************************************************************************/
VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) SP1[64] =
{
   0x01010400U, 0x00000000U, 0x00010000U, 0x01010404U,
   0x01010004U, 0x00010404U, 0x00000004U, 0x00010000U,
   0x00000400U, 0x01010400U, 0x01010404U, 0x00000400U,
   0x01000404U, 0x01010004U, 0x01000000U, 0x00000004U,
   0x00000404U, 0x01000400U, 0x01000400U, 0x00010400U,
   0x00010400U, 0x01010000U, 0x01010000U, 0x01000404U,
   0x00010004U, 0x01000004U, 0x01000004U, 0x00010004U,
   0x00000000U, 0x00000404U, 0x00010404U, 0x01000000U,
   0x00010000U, 0x01010404U, 0x00000004U, 0x01010000U,
   0x01010400U, 0x01000000U, 0x01000000U, 0x00000400U,
   0x01010004U, 0x00010000U, 0x00010400U, 0x01000004U,
   0x00000400U, 0x00000004U, 0x01000404U, 0x00010404U,
   0x01010404U, 0x00010004U, 0x01010000U, 0x01000404U,
   0x01000004U, 0x00000404U, 0x00010404U, 0x01010400U,
   0x00000404U, 0x01000400U, 0x01000400U, 0x00000000U,
   0x00010004U, 0x00010400U, 0x00000000U, 0x01010004U
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) SP2[64] =
{
   0x80108020U, 0x80008000U, 0x00008000U, 0x00108020U,
   0x00100000U, 0x00000020U, 0x80100020U, 0x80008020U,
   0x80000020U, 0x80108020U, 0x80108000U, 0x80000000U,
   0x80008000U, 0x00100000U, 0x00000020U, 0x80100020U,
   0x00108000U, 0x00100020U, 0x80008020U, 0x00000000U,
   0x80000000U, 0x00008000U, 0x00108020U, 0x80100000U,
   0x00100020U, 0x80000020U, 0x00000000U, 0x00108000U,
   0x00008020U, 0x80108000U, 0x80100000U, 0x00008020U,
   0x00000000U, 0x00108020U, 0x80100020U, 0x00100000U,
   0x80008020U, 0x80100000U, 0x80108000U, 0x00008000U,
   0x80100000U, 0x80008000U, 0x00000020U, 0x80108020U,
   0x00108020U, 0x00000020U, 0x00008000U, 0x80000000U,
   0x00008020U, 0x80108000U, 0x00100000U, 0x80000020U,
   0x00100020U, 0x80008020U, 0x80000020U, 0x00100020U,
   0x00108000U, 0x00000000U, 0x80008000U, 0x00008020U,
   0x80000000U, 0x80100020U, 0x80108020U, 0x00108000U
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) SP3[64] =
{
   0x00000208U, 0x08020200U, 0x00000000U, 0x08020008U,
   0x08000200U, 0x00000000U, 0x00020208U, 0x08000200U,
   0x00020008U, 0x08000008U, 0x08000008U, 0x00020000U,
   0x08020208U, 0x00020008U, 0x08020000U, 0x00000208U,
   0x08000000U, 0x00000008U, 0x08020200U, 0x00000200U,
   0x00020200U, 0x08020000U, 0x08020008U, 0x00020208U,
   0x08000208U, 0x00020200U, 0x00020000U, 0x08000208U,
   0x00000008U, 0x08020208U, 0x00000200U, 0x08000000U,
   0x08020200U, 0x08000000U, 0x00020008U, 0x00000208U,
   0x00020000U, 0x08020200U, 0x08000200U, 0x00000000U,
   0x00000200U, 0x00020008U, 0x08020208U, 0x08000200U,
   0x08000008U, 0x00000200U, 0x00000000U, 0x08020008U,
   0x08000208U, 0x00020000U, 0x08000000U, 0x08020208U,
   0x00000008U, 0x00020208U, 0x00020200U, 0x08000008U,
   0x08020000U, 0x08000208U, 0x00000208U, 0x08020000U,
   0x00020208U, 0x00000008U, 0x08020008U, 0x00020200U
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) SP4[64] =
{
   0x00802001U, 0x00002081U, 0x00002081U, 0x00000080U,
   0x00802080U, 0x00800081U, 0x00800001U, 0x00002001U,
   0x00000000U, 0x00802000U, 0x00802000U, 0x00802081U,
   0x00000081U, 0x00000000U, 0x00800080U, 0x00800001U,
   0x00000001U, 0x00002000U, 0x00800000U, 0x00802001U,
   0x00000080U, 0x00800000U, 0x00002001U, 0x00002080U,
   0x00800081U, 0x00000001U, 0x00002080U, 0x00800080U,
   0x00002000U, 0x00802080U, 0x00802081U, 0x00000081U,
   0x00800080U, 0x00800001U, 0x00802000U, 0x00802081U,
   0x00000081U, 0x00000000U, 0x00000000U, 0x00802000U,
   0x00002080U, 0x00800080U, 0x00800081U, 0x00000001U,
   0x00802001U, 0x00002081U, 0x00002081U, 0x00000080U,
   0x00802081U, 0x00000081U, 0x00000001U, 0x00002000U,
   0x00800001U, 0x00002001U, 0x00802080U, 0x00800081U,
   0x00002001U, 0x00002080U, 0x00800000U, 0x00802001U,
   0x00000080U, 0x00800000U, 0x00002000U, 0x00802080U
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) SP5[64] =
{
   0x00000100U, 0x02080100U, 0x02080000U, 0x42000100U,
   0x00080000U, 0x00000100U, 0x40000000U, 0x02080000U,
   0x40080100U, 0x00080000U, 0x02000100U, 0x40080100U,
   0x42000100U, 0x42080000U, 0x00080100U, 0x40000000U,
   0x02000000U, 0x40080000U, 0x40080000U, 0x00000000U,
   0x40000100U, 0x42080100U, 0x42080100U, 0x02000100U,
   0x42080000U, 0x40000100U, 0x00000000U, 0x42000000U,
   0x02080100U, 0x02000000U, 0x42000000U, 0x00080100U,
   0x00080000U, 0x42000100U, 0x00000100U, 0x02000000U,
   0x40000000U, 0x02080000U, 0x42000100U, 0x40080100U,
   0x02000100U, 0x40000000U, 0x42080000U, 0x02080100U,
   0x40080100U, 0x00000100U, 0x02000000U, 0x42080000U,
   0x42080100U, 0x00080100U, 0x42000000U, 0x42080100U,
   0x02080000U, 0x00000000U, 0x40080000U, 0x42000000U,
   0x00080100U, 0x02000100U, 0x40000100U, 0x00080000U,
   0x00000000U, 0x40080000U, 0x02080100U, 0x40000100U
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) SP6[64] =
{
   0x20000010U, 0x20400000U, 0x00004000U, 0x20404010U,
   0x20400000U, 0x00000010U, 0x20404010U, 0x00400000U,
   0x20004000U, 0x00404010U, 0x00400000U, 0x20000010U,
   0x00400010U, 0x20004000U, 0x20000000U, 0x00004010U,
   0x00000000U, 0x00400010U, 0x20004010U, 0x00004000U,
   0x00404000U, 0x20004010U, 0x00000010U, 0x20400010U,
   0x20400010U, 0x00000000U, 0x00404010U, 0x20404000U,
   0x00004010U, 0x00404000U, 0x20404000U, 0x20000000U,
   0x20004000U, 0x00000010U, 0x20400010U, 0x00404000U,
   0x20404010U, 0x00400000U, 0x00004010U, 0x20000010U,
   0x00400000U, 0x20004000U, 0x20000000U, 0x00004010U,
   0x20000010U, 0x20404010U, 0x00404000U, 0x20400000U,
   0x00404010U, 0x20404000U, 0x00000000U, 0x20400010U,
   0x00000010U, 0x00004000U, 0x20400000U, 0x00404010U,
   0x00004000U, 0x00400010U, 0x20004010U, 0x00000000U,
   0x20404000U, 0x20000000U, 0x00400010U, 0x20004010U
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) SP7[64] =
{
   0x00200000U, 0x04200002U, 0x04000802U, 0x00000000U,
   0x00000800U, 0x04000802U, 0x00200802U, 0x04200800U,
   0x04200802U, 0x00200000U, 0x00000000U, 0x04000002U,
   0x00000002U, 0x04000000U, 0x04200002U, 0x00000802U,
   0x04000800U, 0x00200802U, 0x00200002U, 0x04000800U,
   0x04000002U, 0x04200000U, 0x04200800U, 0x00200002U,
   0x04200000U, 0x00000800U, 0x00000802U, 0x04200802U,
   0x00200800U, 0x00000002U, 0x04000000U, 0x00200800U,
   0x04000000U, 0x00200800U, 0x00200000U, 0x04000802U,
   0x04000802U, 0x04200002U, 0x04200002U, 0x00000002U,
   0x00200002U, 0x04000000U, 0x04000800U, 0x00200000U,
   0x04200800U, 0x00000802U, 0x00200802U, 0x04200800U,
   0x00000802U, 0x04000002U, 0x04200802U, 0x04200000U,
   0x00200800U, 0x00000000U, 0x00000002U, 0x04200802U,
   0x00000000U, 0x00200802U, 0x04200000U, 0x00000800U,
   0x04000002U, 0x04000800U, 0x00000800U, 0x00200002U
};

VSECPRIM_ROM(VSECPRIM_LOCAL, actU32) SP8[64] =
{
   0x10001040U, 0x00001000U, 0x00040000U, 0x10041040U,
   0x10000000U, 0x10001040U, 0x00000040U, 0x10000000U,
   0x00040040U, 0x10040000U, 0x10041040U, 0x00041000U,
   0x10041000U, 0x00041040U, 0x00001000U, 0x00000040U,
   0x10040000U, 0x10000040U, 0x10001000U, 0x00001040U,
   0x00041000U, 0x00040040U, 0x10040040U, 0x10041000U,
   0x00001040U, 0x00000000U, 0x00000000U, 0x10040040U,
   0x10000040U, 0x10001000U, 0x00041040U, 0x00040000U,
   0x00041040U, 0x00040000U, 0x10041000U, 0x00001000U,
   0x00000040U, 0x10040040U, 0x00001000U, 0x00041040U,
   0x10001000U, 0x00000040U, 0x10000040U, 0x10040000U,
   0x10040040U, 0x10000000U, 0x00040000U, 0x10001040U,
   0x00000000U, 0x10041040U, 0x00040040U, 0x10000040U,
   0x10040000U, 0x10001000U, 0x10001040U, 0x00000000U,
   0x10041040U, 0x00041000U, 0x00041000U, 0x00001040U,
   0x00001040U, 0x00040040U, 0x10000000U, 0x10041000U
};

# define VSECPRIM_STOP_SEC_CONST_UNSPECIFIED
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */


VSECPRIM_LOCAL_FUNC(void) usekey(VSECPRIM_P2VAR_PARA(actU32) from, VSECPRIM_P2VAR_PARA(actU32) returned_key);

VSECPRIM_LOCAL_FUNC(void) cookey(VSECPRIM_P2VAR_PARA(actU32) raw1, VSECPRIM_P2VAR_PARA(actU32) returned_key);

VSECPRIM_LOCAL_FUNC(void) scrunch(VSECPRIM_P2CONST_PARA(actU8) outof, VSECPRIM_P2VAR_PARA(actU32) into);

VSECPRIM_LOCAL_FUNC(void) unscrun(VSECPRIM_P2VAR_PARA(actU32) outof, VSECPRIM_P2VAR_PARA(actU8) into);
/****************************************************************************
 ** LOCAL FUNCTIONS
 ***************************************************************************/


/****************************************************************************
 ** Macros for rotate, get block ... etc
 ***************************************************************************/

/**********************************************************************************************************************
 *  usekey()
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) usekey(VSECPRIM_P2VAR_PARA(actU32) from, VSECPRIM_P2VAR_PARA(actU32) returned_key)
{
  VSECPRIM_P2VAR_PARA(actU32) to;
  VSECPRIM_P2VAR_PARA(actU32) endp;

  to = returned_key;
  endp = &returned_key[32];
  while (to < endp)
  {
    *to++ = *from++;
  }
}

/**********************************************************************************************************************
 *  cookey()
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) cookey(VSECPRIM_P2VAR_PARA(actU32) raw1, VSECPRIM_P2VAR_PARA(actU32) returned_key)
{
  VSECPRIM_P2VAR_PARA(actU32) cook;
  VSECPRIM_P2VAR_PARA(actU32) raw0;
  actU32 dough[32];
  actLengthType i;

  for (i = 0u; i < 32u; i++)
  {
    dough[i] = 0u;
  }

  cook = dough;
  for (i = 0u; i < 16u; i++, raw1++)
  {
    raw0 = raw1++;
    *cook = (*raw0 & 0x00fc0000UL) << 6u;
    *cook |= (*raw0 & 0x00000fc0UL) << 10u;
    *cook |= (*raw1 & 0x00fc0000UL) >> 10u;
    *cook++ |= (*raw1 & 0x00000fc0UL) >> 6u;
    *cook = (*raw0 & 0x0003f000UL) << 12u;
    *cook |= (*raw0 & 0x0000003fUL) << 16u;
    *cook |= (*raw1 & 0x0003f000UL) >> 4u;
    *cook++ |= (*raw1 & 0x0000003fUL);
  }
  usekey(dough, returned_key);
}

/**********************************************************************************************************************
 *  scrunch()
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) scrunch(VSECPRIM_P2CONST_PARA(actU8) outof, VSECPRIM_P2VAR_PARA(actU32) into)
{
  *into = (*outof++ & 0xffUL) << 24u;
  *into |= (*outof++ & 0xffUL) << 16u;
  *into |= (*outof++ & 0xffUL) << 8u;
  *into++ |= (*outof++ & 0xffUL);
  *into = (*outof++ & 0xffUL) << 24u;
  *into |= (*outof++ & 0xffUL) << 16u;
  *into |= (*outof++ & 0xffUL) << 8u;
  *into |= (*outof & 0xffUL);
}

/**********************************************************************************************************************
 *  unscrun()
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) unscrun(VSECPRIM_P2VAR_PARA(actU32) outof, VSECPRIM_P2VAR_PARA(actU8) into)
{
  *into++ = (actU8)((*outof >> 24u) & 0xffu);
  *into++ = (actU8)((*outof >> 16u) & 0xffu);
  *into++ = (actU8)((*outof >> 8u) & 0xffu);
  *into++ = (actU8)(*outof++ & 0xffu);
  *into++ = (actU8)((*outof >> 24u) & 0xffu);
  *into++ = (actU8)((*outof >> 16u) & 0xffu);
  *into++ = (actU8)((*outof >> 8u) & 0xffu);
  *into = (actU8)(*outof & 0xffu);
}
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actDESInit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 * 
 */
VSECPRIM_FUNC(actRETURNCODE) actDESInit(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key,
   actU8 crypt_mode, VSECPRIM_P2CONST_PARA(actU8) iv, actU8 block_mode,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   /* initialization mode, buffers, etc. */
   info->mode = block_mode;
   info->buffer_used = 0u;
   switch (block_mode)
   {
   case actDES_ECB:
      break;
   case actDES_CBC:
      if (iv == NULL_PTR)
      {
         actMemset(info->prev_block, 0u, actDES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      }
      else
      {
         actMemCpyByteArray(info->prev_block, iv, actDES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      }
      break;
   default:
      return actEXCEPTION_ALGOID;
   }

   /* key setup */
   return actDESKeySetup(info, key, crypt_mode, watchdog);
}

/**********************************************************************************************************************
 *  actDESKeySetup()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 * 
 */
VSECPRIM_FUNC(actRETURNCODE) actDESKeySetup(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key,
   actU8 crypt_mode, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   if (key == NULL_PTR)
   {
      return actEXCEPTION_NULL;
   }
   else
   {
      actLengthType i, j, l, m;
      actU8 key_pc1[56], key_buff[56];
      actU32 subkeyn[32];

      /* user key permutation with PC1 */
      for (j = 0u; j < 56u; j++)
      {
         l = PC1[j];
         m = l & 0x07u;
         key_pc1[j] = (((key[l >> 3u] & bytebit[m]) != 0u) ? 1u : 0u);
      }

      /* generate 16 subkeys */
      for (i = 0u; i < 16u; i++)
      {
         actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

         if (crypt_mode == DE1)
         {
           m = (15u - i) << 1u;
         }
         else
         {
           m = i << 1u;
         }

         subkeyn[m] = 0UL;
         subkeyn[m + 1u] = 0UL;

         /* shift upper 28 bits */
         for (j = 0u; j < 28u; j++)
         {
            l = j + totrotation[i];
            if (l < 28u)
            {
              key_buff[j] = key_pc1[l];
            }
            else
            {
              key_buff[j] = key_pc1[l - 28u];
            }
         }
         /* shift lower 28 bits */
         for (j = 28u; j < 56u; j++)
         {
            l = j + totrotation[i];
            if (l < 56u)
            {
              key_buff[j] = key_pc1[l];
            }
            else
            {
              key_buff[j] = key_pc1[l - 28u];
            }
         }

         /* subkey permutation with PC2 */
         for (j = 0u; j < 24u; j++)
         {
           if (key_buff[PC2[j]] != 0u)
           {
             subkeyn[m] |= bigbyte[j];
           }
           if (key_buff[PC2[j + 24u]] != 0u)
           {
             subkeyn[m + 1u] |= bigbyte[j];
           }
         }
      }
      cookey(subkeyn, info->n_keys);

      actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
      return actOK;
   }
}

/**********************************************************************************************************************
 *  actDESInitEncrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */
VSECPRIM_FUNC(actRETURNCODE) actDESInitEncrypt(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key,
   VSECPRIM_P2CONST_PARA(actU8) iv, actU8 mode,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   return actDESInit(info, key, EN0, iv, mode, watchdog);
}

/**********************************************************************************************************************
 *  actDESEncrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 * 
 */
VSECPRIM_FUNC(actRETURNCODE) actDESEncrypt(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
   boolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   /* out_size = blocks*8 */
   actLengthType out_size = ((in_length + info->buffer_used)/actDES_BLOCK_SIZE)*actDES_BLOCK_SIZE;

   /* check out_buffer size */
   if (in_length > (0xFFFFFFFFuL - actDES_BLOCK_SIZE))
   {
     return actEXCEPTION_LENGTH;
   }

   if (final == FALSE)
   {
      if (*out_length < out_size)
      {
         return actEXCEPTION_LENGTH;
      }
      *out_length = out_size;
   }
   else
   {
      if (*out_length < (out_size + actDES_BLOCK_SIZE))
      {
         return actEXCEPTION_LENGTH;
      }
      *out_length = out_size + actDES_BLOCK_SIZE;
   }

   /* transform input */
   if (in_length > 0u)
   {
      actDESTransform(info, in, in_length, out_buffer, actDESEncryptMultipleBlocks, watchdog);
   }

   /* finalize ? */
   if (final == TRUE)
   {
      /* PKCS 5 padding of last buffered block */
      actU8 pad = (actU8)(actDES_BLOCK_SIZE - info->buffer_used);
      actMemset(info->buffer + info->buffer_used, pad, (actLengthType)pad); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      /* encryption of last block */
      actDESEncryptMultipleBlocks(info, info->buffer, out_buffer+out_size, 1u, watchdog);
   }

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
   return actOK;
}

/**********************************************************************************************************************
 *  actDESInitDecrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */
VSECPRIM_FUNC(actRETURNCODE) actDESInitDecrypt(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key,
   VSECPRIM_P2CONST_PARA(actU8) iv, actU8 mode,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   return actDESInit(info, key, DE1, iv, mode, watchdog);
}

/**********************************************************************************************************************
 *  actDESDecrypt()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 * 
 */
VSECPRIM_FUNC(actRETURNCODE) actDESDecrypt(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, VSECPRIM_P2VAR_PARA(actLengthType) out_length,
  boolean final, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actLengthType total_bytes = in_length + info->buffer_used;
  actLengthType out_size;

   /* if finalize: check (in_length+buffer_length)%8==0 */
   if (final == TRUE)
   {
      if ((total_bytes == 0u) || ((total_bytes%actDES_BLOCK_SIZE) != 0u))
      {
         return actEXCEPTION_INPUT_LENGTH;
      }
   }

   if (total_bytes > 0u)
   {
     /* out_size = blocks*8 */
     out_size = ((total_bytes-1u)/actDES_BLOCK_SIZE)*actDES_BLOCK_SIZE;
   }
   else
   {
     /* No ouput data will be produced as no input data present */
     out_size = 0u;
   }

   /* check out_buffer size */
   if (*out_length < out_size)
   {
      return actEXCEPTION_LENGTH;
   }

   /* transform input */
   if (in_length>0u)
   {
      if (total_bytes <= actDES_BLOCK_SIZE)
      {
         actMemCpyByteArray(info->buffer + info->buffer_used, in, (actLengthType)in_length); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
         info->buffer_used += in_length;
      }
      else
      {
         if ((total_bytes%actDES_BLOCK_SIZE) == 0u)
         {
            actDESTransform(info, in, in_length - actDES_BLOCK_SIZE, out_buffer, actDESDecryptMultipleBlocks, watchdog);
            actMemCpyByteArray(info->buffer, in + in_length - actDES_BLOCK_SIZE, actDES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
            info->buffer_used = actDES_BLOCK_SIZE;
         }
         else
         {
            actDESTransform(info, in, in_length, out_buffer, actDESDecryptMultipleBlocks, watchdog);
         }
      }
   }

   if (final == TRUE)
   {
      /* decryption of the last buffered block */
      actDESDecryptMultipleBlocks(info, info->buffer, info->buffer, 1u, watchdog);
      {
         /* PKCS 5 unpad of the last block */
         actU8 unpad = (actU8)(actDES_BLOCK_SIZE - info->buffer[actDES_BLOCK_SIZE-1u]);
         if (unpad >= actDES_BLOCK_SIZE)
         {
            return actEXCEPTION_PADDING;
         }
         else
         {
            if (*out_length < (out_size+unpad))
            {
               return actEXCEPTION_LENGTH;
            }
            actMemCpyByteArray(out_buffer + out_size, info->buffer, (actLengthType)unpad); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
            *out_length = out_size + unpad;
         }
      }
   }
   else
   {
      *out_length = out_size;
   }

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
   return actOK;
}

/**********************************************************************************************************************
 *  actDESInitEncryptBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 * 
 */
VSECPRIM_FUNC(actRETURNCODE) actDESInitEncryptBlock(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   return actDESKeySetup(info, key, EN0, watchdog);
}

/**********************************************************************************************************************
 *  actDESInitDecryptBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 * 
 */
VSECPRIM_FUNC(actRETURNCODE) actDESInitDecryptBlock(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info, VSECPRIM_P2CONST_PARA(actU8) key,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   return actDESKeySetup(info, key, DE1, watchdog);
}

/**********************************************************************************************************************
 *  actDESCryptBlock()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 * 
 */
VSECPRIM_FUNC(void) actDESCryptBlock(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in_block, VSECPRIM_P2VAR_PARA(actU8) out_block,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   actU32 fval, work, right, leftt;
   VSECPRIM_P2VAR_PARA(actU32) key;
   actU32 inBlocktmp[2], outBlocktmp[2];
   actLengthType roundd;

   scrunch(in_block, inBlocktmp);

   leftt = inBlocktmp[0];
   right = inBlocktmp[1];
   key = info->n_keys;

   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

   /* initial permutation */
   work   = ((leftt >> 4u)  ^ right) & 0x0f0f0f0fu;
   right ^=    work;
   leftt ^=   (work << 4u);
   work   = ((leftt >> 16u) ^ right) & 0x0000ffffu;
   right ^=    work;
   leftt ^=   (work << 16u);
   work   = ((right >> 2u)  ^ leftt) & 0x33333333u;
   leftt ^=    work;
   right ^=   (work << 2u);
   work   = ((right >> 8u)  ^ leftt) & 0x00ff00ffu;
   leftt ^=    work;
   right ^=   (work << 8u);
   right  = ((right << 1u) | ((right >> 31u) & 1u)) & 0xffffffffu;
   work   = (leftt ^ right) & 0xaaaaaaaau;
   leftt ^=    work;
   right ^=    work;
   leftt  = ((leftt << 1u) | ((leftt >> 31u) & 1u)) & 0xffffffffu;


   for (roundd = 0u; roundd < 8u; roundd++)
   {
      work  = (right << 28u) | (right >> 4u);
      work ^= *key++;
      fval  = SP7[ work        & 0x3fu];
      fval |= SP5[(work >>  8u) & 0x3fu];
      fval |= SP3[(work >> 16u) & 0x3fu];
      fval |= SP1[(work >> 24u) & 0x3fu];
      work  = right ^ *key++;
      fval |= SP8[ work        & 0x3fu];
      fval |= SP6[(work >>  8u) & 0x3fu];
      fval |= SP4[(work >> 16u) & 0x3fu];
      fval |= SP2[(work >> 24u) & 0x3fu];
      leftt ^= fval;

      work  = (leftt << 28u) | (leftt >> 4u);
      work ^= *key++;
      fval  = SP7[ work        & 0x3fu];
      fval |= SP5[(work >>  8u) & 0x3fu];
      fval |= SP3[(work >> 16u) & 0x3fu];
      fval |= SP1[(work >> 24u) & 0x3fu];
      work  = leftt ^ *key++;
      fval |= SP8[ work        & 0x3fu];
      fval |= SP6[(work >>  8u) & 0x3fu];
      fval |= SP4[(work >> 16u) & 0x3fu];
      fval |= SP2[(work >> 24u) & 0x3fu];
      right ^= fval;

      actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
   }

   right  = (right << 31u) | (right >> 1u);
   work   = (leftt ^ right) & 0xaaaaaaaau;
   leftt ^= work;
   right ^= work;
   leftt  = (leftt << 31u) | (leftt >> 1u);
   work   = ((leftt >> 8u) ^ right) & 0x00ff00ffu;
   right ^= work;
   leftt ^= (work << 8u);
   work   = ((leftt >> 2u) ^ right) & 0x33333333u;
   right ^= work;
   leftt ^= (work << 2u);
   work   = ((right >> 16u) ^ leftt) & 0x0000ffffu;
   leftt ^= work;
   right ^= (work << 16u);
   work   = ((right >> 4u) ^ leftt) & 0x0f0f0f0fu;
   leftt ^= work;
   right ^= (work << 4u);

   outBlocktmp[1] = leftt;
   outBlocktmp[0] = right;

   unscrun(outBlocktmp, out_block);
   actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
}

/**********************************************************************************************************************
 *  actDESEncryptMultipleBlocks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actDESEncryptMultipleBlocks(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
   actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2VAR_PARA(actU8) prev_block = info->prev_block;
   actLengthType i, bcount;

   switch (info->mode)
   {
   case actDES_CBC:
      for (bcount=0u; bcount<blocks; ++bcount)
      {
         /* out_block = prev_block ^ in_block */
         for (i=0u; i<actDES_BLOCK_SIZE; ++i)
         {
            out_buffer[i] = (actU8)(prev_block[i] ^ *in++);
         }
         /* out_block = Encrypt(out_block) */
         actDESCryptBlock(info, out_buffer, out_buffer, watchdog);
         /* prev_block = out_block */
         for (i=0u; i<actDES_BLOCK_SIZE; ++i)
         {
            prev_block[i] = *out_buffer++;
         }
      }
      break;
   default:   /* ECB */
      for (bcount=0u; bcount<blocks; ++bcount, in += actDES_BLOCK_SIZE, out_buffer += actDES_BLOCK_SIZE)
      {
         /* out_block = Encrypt(in_block) */
         actDESCryptBlock(info, in, out_buffer, watchdog);
      }
      break;
   }
}

/**********************************************************************************************************************
 *  actDESDecryptMultipleBlocks()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actDESDecryptMultipleBlocks(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, VSECPRIM_P2VAR_PARA(actU8) out_buffer,
   actLengthType blocks, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   VSECPRIM_P2VAR_PARA(actU8) prev_block = info->prev_block;
   actLengthType i, bcount;

   switch (info->mode)
   {
   case actDES_CBC:
      for (bcount=0u; bcount<blocks; ++bcount)
      {
         /* out_block = Decrypt(in_block) */
         actDESCryptBlock(info, in, out_buffer, watchdog);
         /* out_block = prev_block ^ out_block */
         /* prev_block = in_block */
         for (i=0u; i<actDES_BLOCK_SIZE; ++i)
         {
            *out_buffer++ ^= prev_block[i];
            prev_block[i] = *in++;
         }
      }
      break;
   default:   /* ECB */
      for (bcount=0u; bcount<blocks; ++bcount, in += actDES_BLOCK_SIZE, out_buffer += actDES_BLOCK_SIZE)
      {
         /* out_block = Decrypt(in_block) */
         actDESCryptBlock(info, in, out_buffer, watchdog);
      }
   break;
   }
}


/**********************************************************************************************************************
 *  actDESTransform()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actDESTransform(
   VSECPRIM_P2VAR_PARA(actDESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) in, actLengthType in_length,
   VSECPRIM_P2VAR_PARA(actU8) out_buffer, actDESTransformFctType transform,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   actLengthType blocks, bytes, diff=0u;

   /* left overs of the last transform */
   if ((info->buffer_used > 0u) && (info->buffer_used <= actDES_BLOCK_SIZE))
   {
      diff = actMin(in_length, actDES_BLOCK_SIZE - info->buffer_used);
      actMemCpyByteArray(&info->buffer[info->buffer_used], in, (actLengthType)diff); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
      info->buffer_used += diff;
      if (info->buffer_used < actDES_BLOCK_SIZE)
      {
         /* no complete block */
         return;
      }
      else
      {
         /* transform buffered block */
# ifdef actNO_INDIRECT_CALLS
        if (transform == actDESEncryptMultipleBlocks)
        {
          actDESEncryptMultipleBlocks(info, info->buffer, out_buffer, 1u, watchdog);
        }
        else
        {
          actDESDecryptMultipleBlocks(info, info->buffer, out_buffer, 1u, watchdog);
        }
# else
         transform(info, info->buffer, out_buffer, 1u, watchdog);
# endif
         in += diff; out_buffer += actDES_BLOCK_SIZE;
      }
   }

   /* transform blocks */
   blocks = (in_length-diff)/actDES_BLOCK_SIZE;
   bytes = actDES_BLOCK_SIZE*blocks;
# ifdef actNO_INDIRECT_CALLS
   if (transform == actDESEncryptMultipleBlocks)
   {
     actDESEncryptMultipleBlocks(info, in, out_buffer, blocks, watchdog);
   }
   else
   {
     actDESDecryptMultipleBlocks(info, in, out_buffer, blocks, watchdog);
   }
# else
   transform(info, in, out_buffer, blocks, watchdog);
# endif

   /* copy left overs into info->buffer */
   info->buffer_used = in_length - (diff + bytes);
   actMemCpyByteArray(info->buffer, in + bytes, (actLengthType)(info->buffer_used)); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTDES_ENABLED == STD_ON) */

