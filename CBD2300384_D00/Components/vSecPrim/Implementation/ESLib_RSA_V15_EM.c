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
/*!        \file  ESLib_RSA_V15_EM.c
 *        \brief  Crypto library - PKCS #1 encoding methods
 *
 *      \details Encoding methods for RSA encryption / decryption and signature verification / generation
 *               according to PKCS #1 v2.1
 *               Currently the actClib version is used.
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
#define ESLIB_RSA_V15_EM_SOURCE

#include "ESLib.h"
#include "ESLib_ASN_1.h"

#if (VSECPRIM_RSA_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
/*****************************************************************************
  Types and constants
 *****************************************************************************/

# define ASN1_PADDING_BLOCK_TYPE_INVALID                              (0xFFu)

/****************************************************************************
 ** Global Functions
 ***************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

 /**********************************************************************************************************************
  *  esl_verifyPaddingRSAEM_V15()
  *********************************************************************************************************************/
/*!
 *
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_verifyPaddingRSAEM_V15(VSECPRIM_P2CONST_PARA(eslt_Byte) message, VSECPRIM_P2VAR_PARA(eslt_Length) paddingLength, eslt_Byte blockType)
{

  eslt_ErrorCode returnValue;
  eslt_Length length;
  eslt_Length index;
  eslt_Length offset;
  eslt_Length newOffset;
  eslt_Length oldOffset;
  eslt_Byte allowedMask;
  eslt_Byte allowedValue;
  eslt_Byte allowedResult;
  eslt_Byte deniedMask;
  eslt_Byte deniedValue;
  eslt_Byte deniedResult;
  eslt_Byte localBlockType;

  returnValue = ESL_ERC_NO_ERROR;

  if ((message == NULL_PTR) || (paddingLength == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }

  if (returnValue == ESL_ERC_NO_ERROR)
  {
    length = *paddingLength;
    localBlockType = blockType;

    /* Provided message has to be large enough to hold minimal padding */
    if (length < ASN1_SIZEOF_MINIMAL_PADDING)
    {
      returnValue = ESL_ERC_PARAMETER_INVALID;
    }
    else
    {
      /* Check PKCS header and expected block type */
      if ((0x00u != message[0]) || (blockType != message[1]))
      {
        /* No valid header present, use "invalid" block type */
        localBlockType = ASN1_PADDING_BLOCK_TYPE_INVALID;
      }

      /* Set masks for padding check */
      switch (localBlockType)
      {
      case ASN1_PADDING_BLOCK_TYPE_PRIVATE_ZERO:
      {
        /* Padding bytes shall be zero */
        allowedMask = 0xFFu;
        allowedValue = 0x00u;
        deniedMask = 0x00u;
        deniedValue = 0xFFu;

        break;
      }
      case ASN1_PADDING_BLOCK_TYPE_PRIVATE:
      {
        /* Padding bytes shall be 0xFF */
        allowedMask = 0xFFu;
        allowedValue = 0xFFu;
        deniedMask = 0x00u;
        deniedValue = 0xFFu;

        break;
      }
      case ASN1_PADDING_BLOCK_TYPE_PUBLIC:
      {
        /* Padding bytes shall be non-zero value */
        allowedMask = 0x00u;
        allowedValue = 0x00u;
        deniedMask = 0xFFu;
        deniedValue = 0x00u;

        break;
      }
      case ASN1_PADDING_BLOCK_TYPE_INVALID:
      default:
      {
        /* All values forbidden */
        allowedMask = 0x00u;
        allowedValue = 0xFFu;
        deniedMask = 0x00u;
        deniedValue = 0x00u;

        /* Unoptimized to prevent timing analysis */
        returnValue = ESL_ERC_RSA_ENCODING_INVALID;

        break;
      }
      }

      offset = 1u;

      for (index = 2u; index < length; index++)
      {
        newOffset = offset + 1u;
        oldOffset = offset;

        /* Check against allowed values */
        if ((message[index] & allowedMask) == allowedValue)
        {
          allowedResult = 0x00u;
        }
        else
        {
          allowedResult = 0x01u;
        }

        /* Check against forbidden values */
        if ((message[index] & deniedMask) != deniedValue)
        {
          deniedResult = 0x00u;
        }
        else
        {
          deniedResult = 0x01u;
        }

        if (0x00u == (allowedResult | deniedResult))
        {
          /* Unoptimized to prevent timing analysis */
          if (newOffset == index)
          {
            offset = newOffset;
          }
          else
          {
            offset = oldOffset;
          }
        }
      }

      /* Block type 0 already includes trailing zero */
      if ((ASN1_PADDING_BLOCK_TYPE_PRIVATE_ZERO != localBlockType) && (offset < (length - 1u)))
      {
        /* Note: This function is called with length = keyPairModuleSize.
         * Now, the value of offset is at most (length - 1) (or (keyPairModuleSize - 1))
         * and in this case, the last byte of message will be evaluated. */
        offset++;
      }

      /* Check minimal padding length and trailing zero */
      if (((ASN1_SIZEOF_MINIMAL_PADDING - 1u) > offset) || (0x00u != message[offset]))
      {
        returnValue = ESL_ERC_RSA_ENCODING_INVALID;
      }

      if (ESL_ERC_NO_ERROR == returnValue)
      {
        /* In case offset = (keyPairModuleSize - 1), the value of paddingLength = keyPairModuleSize. */
        *paddingLength = offset + 1u;
      }
    }
  }

  return returnValue;
} /* PRQA S 6010, 6030, 6080 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RSA_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_V15_EM.c
 *********************************************************************************************************************/
