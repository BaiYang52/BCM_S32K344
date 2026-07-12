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
/*!        \file  ESLib_RSA_V15_DecCRT.c
 *        \brief  RSA V1.5 (Decryption) implementation.
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
#define ESLIB_RSA_V15_DEC_CRT_SOURCE

#include "ESLib.h"
#include "ESLib_Helper.h"
#include "ESLib_ASN_1.h"

#include "actIRSA.h"

#if (VSECPRIM_RSA_V15_ENABLED == STD_ON) /* COV_VSECPRIM_NO_SAFE_CONFIG XF */

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

# define ESL_RSA_V15_NO_PADDING_BYTE                                  0u
# define ESL_RSA_V15_PADDING_BYTE_AT_MIDDLE_INDEX                     1u
# define ESL_RSA_V15_PADDING_BYTE_AT_LAST_INDEX                       2u

/****************************************************************************
 ** Global Functions
 ***************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

 /**********************************************************************************************************************
  *  esl_initDecryptRSACRT_V15()
  *********************************************************************************************************************/
/*!
 *
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initDecryptRSACRT_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTdec) workSpace,
                                                        eslt_Length keyPairPrimePSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeP,
                                                        eslt_Length keyPairPrimeQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) keyPairPrimeQ,
                                                        eslt_Length privateKeyExponentDPSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDP,
                                                        eslt_Length privateKeyExponentDQSize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyExponentDQ,
                                                        eslt_Length privateKeyInverseQISize, VSECPRIM_P2CONST_PARA(eslt_Byte) privateKeyInverseQI)
{
  eslt_ErrorCode returnValue;
  eslt_Length keyPairModuleSize;

  if (workSpace == NULL_PTR)
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    keyPairModuleSize = (eslt_Length)(keyPairPrimePSize + keyPairPrimeQSize);

    if (workSpace->header.size < ESL_MAXSIZEOF_WS_RSA_CRT_DEC)
    {
      returnValue = ESL_ERC_WS_TOO_SMALL;
    }
    else
    {
      workSpace->wsRSACRTDecPrim.header.size = (eslt_Length)(ESL_MAXSIZEOF_WS_RSA_CRT_DEC_PRIM);
      workSpace->wsRSACRTDecPrim.header.watchdog = workSpace->header.watchdog;
      returnValue = esl_initDecryptRSACRT_prim(&(workSpace->wsRSACRTDecPrim),
      keyPairPrimePSize, keyPairPrimeP,
      keyPairPrimeQSize, keyPairPrimeQ,
      privateKeyExponentDPSize, privateKeyExponentDP, privateKeyExponentDQSize, privateKeyExponentDQ, privateKeyInverseQISize, privateKeyInverseQI);
    }
    if (ESL_ERC_NO_ERROR == returnValue)
    {
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_RSA); /* SBSW_VSECPRIM_CALL_FUNCTION */
      /* store message buffer length in workspace */
      workSpace->primeSizesInformation = keyPairModuleSize;
    }
  }
  return returnValue;
} /* PRQA S 6060 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

/**********************************************************************************************************************
 *  esl_decryptRSACRT_V15()
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
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_decryptRSACRT_V15(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceRSACRTdec) workSpace,
                                                    eslt_Length cipherSize,
                                                    VSECPRIM_P2CONST_PARA(eslt_Byte) cipher,
                                                    VSECPRIM_P2VAR_PARA(eslt_Length) messageSize,
                                                    VSECPRIM_P2VAR_PARA(eslt_Byte) message)
{

  eslt_ErrorCode returnValue;
  eslt_Length keyPairModuleSize;
  eslt_Length i;
  eslt_Byte checkPaddingByte = ESL_RSA_V15_NO_PADDING_BYTE;

  VSECPRIM_P2VAR_PARA(eslt_Byte) messageBuf;

  if ((workSpace == NULL_PTR) || (messageSize == NULL_PTR) || (message == NULL_PTR))
  {
    returnValue = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    /* get underlying workspace pointer */
    keyPairModuleSize = (eslt_Length) (workSpace->wsRSACRTDecPrim.CRTStruct.n_bytes);

    if (ESL_WST_ALGO_RSA != (workSpace->header.status & ESL_WST_M_ALGO))
    {
      returnValue = ESL_ERC_WS_STATE_INVALID;
    }
    else if (0u == (workSpace->header.status & ESL_WST_M_RUNNING))
    {
      returnValue = ESL_ERC_WS_STATE_INVALID;
    }
    else if (keyPairModuleSize < ASN1_SIZEOF_MINIMAL_PADDING)
    {
      returnValue = ESL_ERC_RSA_MODULE_OUT_OF_RANGE;
    }
    else
    {
      i = keyPairModuleSize;
      messageBuf = workSpace->wsEncoding;

      returnValue = esl_decryptRSACRT_prim(&(workSpace->wsRSACRTDecPrim), cipherSize, cipher, &i, messageBuf);

      if (ESL_ERC_NO_ERROR == returnValue)
      {
        if ((i != keyPairModuleSize) || (0x00u != messageBuf[0]) || (0x02u != messageBuf[1]))
        {
          returnValue = ESL_ERC_RSA_ENCODING_INVALID;
        }
        else
        {
          /* Search messageBuf for padding byte 0x00 */
          for (i = 2u; i < keyPairModuleSize; i++)
          {
            if (messageBuf[i] == 0x00u)
            {
              if (i < (keyPairModuleSize - 1u))
              {
                checkPaddingByte = ESL_RSA_V15_PADDING_BYTE_AT_MIDDLE_INDEX;
              }
              else
              {
                checkPaddingByte = ESL_RSA_V15_PADDING_BYTE_AT_LAST_INDEX;
              }
              break;
            }
          }

          switch (checkPaddingByte)
          {
            case ESL_RSA_V15_PADDING_BYTE_AT_MIDDLE_INDEX:

              i++;

              if (ASN1_SIZEOF_MINIMAL_PADDING <= i)
              {
                if (*messageSize < (keyPairModuleSize - i))
                {
                  returnValue = ESL_ERC_OUTPUT_SIZE_TOO_SHORT;
                }
                else
                {
                  *messageSize = keyPairModuleSize - i;
                  actMemCpyByteArray(message, &messageBuf[(keyPairModuleSize - *messageSize)], *messageSize); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */
                  /* returnValue = ESL_ERC_NO_ERROR already set */
                }
              }
              else
              {
                returnValue = ESL_ERC_RSA_ENCODING_INVALID;
              }
              break;

            case ESL_RSA_V15_PADDING_BYTE_AT_LAST_INDEX:

              /* messageBuf only contains padding bytes, the actual message is empty
               * returnValue = ESL_ERC_NO_ERROR already set */
              *messageSize = 0u;
              break;

            case ESL_RSA_V15_NO_PADDING_BYTE:
            default:

              returnValue = ESL_ERC_RSA_ENCODING_INVALID;
              break;

          }
        }
      }
    }
  }
  return returnValue;
} /* PRQA S 6080, 6030 */ /* MD_VSECPRIM_QM_ONLY_METRIC */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_RSA_V15_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_RSA_V15_DecCRT.c
 *********************************************************************************************************************/
