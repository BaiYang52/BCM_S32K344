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
/*!        \file  actBigNum.c
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

#define ACTBIGNUM_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actBigNum.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTBIGNUM_ENABLED == STD_ON)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  actBNLoadLE()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actBNLoadLE(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) bigNum,
  VSECPRIM_P2CONST_PARA(actU8) bytes,
  const actU32 byteLength)
{
  actLengthType b;
  actLengthType offset = 0u;

  for (b = (byteLength / actBN_BYTES_PER_DIGIT); b > 0u; b--)
  {
    actBNDIGIT tmp;

# if (4 == actBN_BYTES_PER_DIGIT)
    tmp = ((actBNDIGIT)(bytes[offset]));
    tmp += ((actBNDIGIT)(bytes[offset + 1u])) << 8;
    tmp += ((actBNDIGIT)(bytes[offset + 2u])) << 16;
    tmp += ((actBNDIGIT)(bytes[offset + 3u])) << 24;
    offset += 4u;
# elif (2 == actBN_BYTES_PER_DIGIT) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
    tmp = ((actBNDIGIT)(bytes[offset]));
    offset++;
    tmp += ((actBNDIGIT)(bytes[offset])) << 8;
    offset++;
# elif (1 == actBN_BYTES_PER_DIGIT)
    tmp = ((actBNDIGIT)(bytes[offset]));
    offset++;
# else
#  error actBN_BYTES_PER_DIGIT value not supported
# endif

    bigNum[(byteLength / actBN_BYTES_PER_DIGIT) - b] = tmp; /* SBSW_VSECPRIM_WRITE_ARRAY */
  }
}

/**********************************************************************************************************************
 *  actBNStoreLE()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actBNStoreLE(
  VSECPRIM_P2VAR_PARA(actU8) bytes,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) bigNum,
  const actLengthType bnLength)
{
  VSECPRIM_REGISTER actU32 b;
  VSECPRIM_P2VAR_PARA(actU8) bytesPtr = bytes;

  for (b = bnLength; b > 0u; b--)
  {
    VSECPRIM_REGISTER actBNDIGIT tmp = *(&bigNum[(bnLength - b)]);

# if (actBN_BYTES_PER_DIGIT == 4)
    bytesPtr[0] = (actU8) (tmp); /* SBSW_VSECPRIM_WRITE_ARRAY */
    bytesPtr[1] = (actU8) (tmp >> 8); /* SBSW_VSECPRIM_WRITE_ARRAY */
    bytesPtr[2] = (actU8) (tmp >> 16); /* SBSW_VSECPRIM_WRITE_ARRAY */
    bytesPtr[3] = (actU8) (tmp >> 24); /* SBSW_VSECPRIM_WRITE_ARRAY */
    bytesPtr = &bytesPtr[4];
# elif (actBN_BYTES_PER_DIGIT == 2) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
    bytesPtr[0] = (actU8) (tmp); /* SBSW_VSECPRIM_WRITE_ARRAY */
    bytesPtr[1] = (actU8) (tmp >> 8); /* SBSW_VSECPRIM_WRITE_ARRAY */
    bytesPtr = &bytesPtr[2];
# elif (actBN_BYTES_PER_DIGIT == 1)
    bytesPtr[(bnLength - b)] = (actU8) (tmp); /* SBSW_VSECPRIM_WRITE_ARRAY */
# else
#  error actBN_BYTES_PER_DIGIT value not supported
# endif
  }
}

/**********************************************************************************************************************
 *  actBNCopy()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actBNCopy(VSECPRIM_P2VAR_PARA(actBNDIGIT) a, VSECPRIM_P2CONST_PARA(actBNDIGIT) b, const actLengthType length)
{
  actLengthType i;

  for (i = 0u; i < length; ++i)
  {
    a[i] = b[i]; /* SBSW_VSECPRIM_WRITE_ARRAY */
  }
}

/**********************************************************************************************************************
 *  actBNGetBitLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actLengthType) actBNGetBitLength(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  const actLengthType length)
{
  actLengthType bits = 0u;

  if (length > 0u)
  {
    actBNDIGIT high_digit;
    actLengthType i = length - 1u;
    while((a[i] == 0u) && (i > 0u))
    {
      --i;
    } /* skip leading zero digits */
    high_digit = a[i];
    bits = i * actBN_BITS_PER_DIGIT;
    while (high_digit > 0u)
    {
      ++bits;
      high_digit >>= 1u;
    }
  }

  return bits;
}

/**********************************************************************************************************************
 *  actBNSetBit()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actBNSetBit(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  const actLengthType length,
  const actLengthType bit_pos)
{
  actRETURNCODE retVal = actEXCEPTION_UNKNOWN;
  if (bit_pos >= (actLengthType)(length * actBN_BITS_PER_DIGIT))
  {
    /* Error code already set */
  }
  else
  {
    a[bit_pos / actBN_BITS_PER_DIGIT] |= (((actBNDIGIT) 1) << (bit_pos % actBN_BITS_PER_DIGIT)); /* SBSW_VSECPRIM_WRITE_ARRAY */
    retVal = actOK;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  actBNSet()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC( void ) actBNSet(
  VSECPRIM_P2VAR_PARA( actBNDIGIT ) a,
  const actLengthType length,
  const actBNDIGIT  value)
{
   actLengthType i;

   /* Set least significant digit of bignum "value" */
   /* Set all other digits to 0u */
   a[0] = value;                /* a [0] = value */ /* SBSW_VSECPRIM_WRITE_ARRAY */
   for (i = 1u; i<length; ++i)
   {
      a[i] = 0u;    /* a [i] = 0 */ /* SBSW_VSECPRIM_WRITE_ARRAY */
   }
}

/**********************************************************************************************************************
 *  actBNIsZero()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actU8) actBNIsZero(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  const actLengthType length)
{
  actU8 retVal = TRUE;
  actLengthType i;

  for (i = 0u; i < length; ++i)
  {
    if (a[i] != 0u)
    {
      retVal = FALSE;
      break;
    }
  }
  return retVal;
}

/**********************************************************************************************************************
 *  actBNCompare()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actBNCompare(
  VSECPRIM_P2CONST_PARA(actBNDIGIT) a,
  VSECPRIM_P2CONST_PARA(actBNDIGIT) b,
  const actLengthType length)
{
  actRETURNCODE retVal = actCOMPARE_EQUAL;
  actLengthType i;

  for (i = 0u; i < length; i++) /* PRQA S 0771 */ /* MD_VSECPRIM_14.6 */
  {
    if (a[((length - 1u) - i)] < b[((length - 1u) - i)])
    {
      retVal = actCOMPARE_SMALLER;
      break;
    }
    else if (a[((length - 1u) - i)] > b[((length - 1u) - i)])
    {
      retVal = actCOMPARE_LARGER;
      break;
    }
    else
    {
      /* no action */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  actBNSetOctetString()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actBNSetOctetString(
  VSECPRIM_P2VAR_PARA(actBNDIGIT) a,
  actLengthType length,
  VSECPRIM_P2CONST_PARA(actU8) in,
  const actLengthType in_len)
{
  actLengthType i, digits = in_len / actBN_BYTES_PER_DIGIT;
  actLengthType offset = in_len - 1u;

  actBNSetZero(a, length); /* SBSW_VSECPRIM_CALL_FUNCTION */
  digits = actMin((actLengthType)length, digits);

  /* copy bytes to digits */
  for (i = 0u; i < digits; ++i)
  {
# if (actBN_BYTES_PER_DIGIT == 1) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    a[i] = (actBNDIGIT)in[offset - i]; /* SBSW_VSECPRIM_WRITE_ARRAY */
# elif (actBN_BYTES_PER_DIGIT == 2)
    a[i] = (actBNDIGIT)(((actBNDIGIT)(in[offset - 1u]) << 8) | ((actBNDIGIT)(in[offset]))); /* SBSW_VSECPRIM_WRITE_ARRAY */
    offset -= 2u;
# elif (actBN_BYTES_PER_DIGIT == 4)
    a[i] = (actBNDIGIT)(((((actBNDIGIT)(in[offset - 3u]) << 24) | ((actBNDIGIT)(in[offset - 2u]) << 16)) | ((actBNDIGIT)(in[offset - 1u]) << 8)) | ((actBNDIGIT)(in[offset]))); /* SBSW_VSECPRIM_WRITE_ARRAY */
    offset -= 4u;
# else
#  error actBN_BYTES_PER_DIGIT value not supported
# endif
  }

  /* handle (in_len%actBN_BYTES_PER_DIGIT) remaining bytes */
# if (actBN_BYTES_PER_DIGIT != 1) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
  if (in_len <= ((actLengthType)length * actBN_BYTES_PER_DIGIT))
  {
    for(i = in_len % actBN_BYTES_PER_DIGIT; i > 0u; --i)
    {
      a[digits] <<= 8; /* SBSW_VSECPRIM_WRITE_ARRAY */
      a[digits] |= in[((offset - i) + 1u)]; /* SBSW_VSECPRIM_WRITE_ARRAY */
    }
  }
# endif
}

/**********************************************************************************************************************
 *  actBNSetOctetStringROM()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actBNSetOctetStringROM(
   VSECPRIM_P2VAR_PARA(actBNDIGIT) a, actLengthType length,
   VSECPRIM_P2CONST_PARA(actU8) in, const actLengthType in_len)
{
  actLengthType i, digits = in_len / actBN_BYTES_PER_DIGIT;
  actLengthType offset = in_len - 1u;

  actBNSetZero(a, length); /* SBSW_VSECPRIM_CALL_FUNCTION */
  digits = actMin((actLengthType)length, digits);

  /* copy bytes to digits */
  for (i = 0; i < digits; i++)
  {
# if (actBN_BYTES_PER_DIGIT == 1) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    a[i] = (actBNDIGIT)ACT_ROM_BYT(&in[offset - i]); /* SBSW_VSECPRIM_WRITE_ARRAY */
# elif (actBN_BYTES_PER_DIGIT == 2)
    a[i] = (actBNDIGIT)(((actBNDIGIT)ACT_ROM_BYT(&in[offset - 1u]) << 8) | (actBNDIGIT)(ACT_ROM_BYT(&in[offset]))); /* SBSW_VSECPRIM_WRITE_ARRAY */
    offset -= 2u;
# elif (actBN_BYTES_PER_DIGIT == 4)
    a[i] = (actBNDIGIT)(((((actBNDIGIT)ACT_ROM_BYT(&in[offset - 3u]) << 24) | ((actBNDIGIT)ACT_ROM_BYT(&in[offset - 2u]) << 16)) | ((actBNDIGIT)ACT_ROM_BYT(&in[offset - 1u]) << 8)) | ((actBNDIGIT)ACT_ROM_BYT(&in[offset]))); /* SBSW_VSECPRIM_WRITE_ARRAY */
    offset -= 4u;
# else
#  error actBN_BYTES_PER_DIGIT value not supported
# endif
  }

  /* handle (in_len%actBN_BYTES_PER_DIGIT) remaining bytes */
# if (actBN_BYTES_PER_DIGIT != 1) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
  if (in_len <= (actLengthType)(length * actBN_BYTES_PER_DIGIT))
  {
    for(i = in_len % actBN_BYTES_PER_DIGIT; i > 0u; --i)
    {
      a[digits] <<= 8; /* SBSW_VSECPRIM_WRITE_ARRAY */
      a[digits] |= ACT_ROM_BYT(&in[((offset - i) + 1u)]); /* SBSW_VSECPRIM_WRITE_ARRAY */
    }
  }
# endif
}

/**********************************************************************************************************************
 *  actBNOctetString()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(void) actBNOctetString(
   VSECPRIM_P2VAR_PARA(actU8) out, actLengthType out_len,
   VSECPRIM_P2CONST_PARA(actBNDIGIT) a, const actLengthType length)
{
  actLengthType i, digits = out_len / actBN_BYTES_PER_DIGIT;
# if (actBN_BYTES_PER_DIGIT != 1) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
  actBNDIGIT tmp;
# endif
  actLengthType offset = out_len;

  actMemset(out, 0u, out_len); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  digits = actMin((actLengthType)length, digits);

  /* copy digits to bytes */
  for (i = 0; i < digits; ++i)
  {
# if (actBN_BYTES_PER_DIGIT == 1) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    offset--;
    out[offset] = a[i]; /* SBSW_VSECPRIM_WRITE_ARRAY */
# elif (actBN_BYTES_PER_DIGIT == 2)
    tmp = a[i];
    offset--;
    out[offset] = ((actU8) tmp); /* SBSW_VSECPRIM_WRITE_ARRAY */
    tmp >>= 8;
    offset--;
    out[offset] = ((actU8) tmp); /* SBSW_VSECPRIM_WRITE_ARRAY */
# elif (actBN_BYTES_PER_DIGIT == 4)
    tmp = a[i];
    offset--;
    out[offset] = ((actU8) tmp); /* SBSW_VSECPRIM_WRITE_ARRAY */
    tmp >>= 8;
    offset--;
    out[offset] = ((actU8) tmp); /* SBSW_VSECPRIM_WRITE_ARRAY */
    tmp >>= 8;
    offset--;
    out[offset] = ((actU8) tmp); /* SBSW_VSECPRIM_WRITE_ARRAY */
    tmp >>= 8;
    offset--;
    out[offset] = ((actU8) tmp); /* SBSW_VSECPRIM_WRITE_ARRAY */
# else
#  error actBN_BYTES_PER_DIGIT value not supported
# endif
  }

  /* handle (out_len%actBN_BYTES_PER_DIGIT) remaining bytes */
# if (actBN_BYTES_PER_DIGIT != 1) /* COV_VSECPRIM_NO_SAFE_CONFIG TX */
  if (out_len <= (actLengthType)(length * actBN_BYTES_PER_DIGIT))
  {
    if ((out_len % actBN_BYTES_PER_DIGIT) > 0u)
    {
      tmp = a[i];

      for(i = out_len % actBN_BYTES_PER_DIGIT; i > 0u; --i)
      {
        offset--;
        out[offset] = ((actU8) tmp); /* SBSW_VSECPRIM_WRITE_ARRAY */
        tmp >>= 8;  /* PRQA S 2983 */ /* MD_VSECPRIM_REDUNDANT_ASSIGNMENT */
      }
    }
  }
# endif
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTBIGNUM_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actBigNum.c
 *********************************************************************************************************************/
