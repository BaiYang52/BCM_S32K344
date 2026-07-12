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
/*!        \file  ESLib_ECDH.c
 *        \brief  ECDH implementation.
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

#define ESLIB_ECDH_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "ESLib_Config.h"
#include "ESLib.h"

/* actCLib includes */
#include "actIX25519.h"

#if (VSECPRIM_ECDH_25519_ENABLED == STD_ON)

# include "ESLib_Helper.h"

# ifndef ESL_SUPPORT_CURVE25519
#  error "Curve25519 Support must be enabled"
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  esl_validateKeyLength()
 **********************************************************************************************************************/
/*! \brief         Validate key length pointer.
 *  \details       This function validates the public key length pointer.
 *  \param[in]     publicKeyLength  Reference to the public key length.
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_validateKeyLength(eslt_LengthConstRef publicKeyLength);

/**********************************************************************************************************************
 *  esl_validateCSLength()
 **********************************************************************************************************************/
/*! \brief         Validate shared secret length pointer.
 *  \details       This function validates the common shared secret length pointer.
 *  \param[in]     sharedSecretLength  Reference to the shared secret length.
 *  \return        ESL_ERC_NO_ERROR  Operation successful
 *                 ESL_ERC_PARAMETER_INVALID  Input parameter is NULL
 *                 ESL_ERC_WS_STATE_INVALID  Work space state invalid
 *  \pre           Workspace needs to be initialized.
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_validateCSLength(eslt_LengthConstRef sharedSecretLength);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  esl_validateKeyLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_validateKeyLength(eslt_LengthConstRef publicKeyLength)
{
  /* Call common check length references function for publicKeyLength */
  return esl_checkLengthRef(publicKeyLength, ESL_SIZEOF_X25519_PUBLIC_KEY); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  esl_validateCSLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_validateCSLength(eslt_LengthConstRef sharedSecretLength)
{
  /* Call common check length references function for sharedSecretLength */
  return esl_checkLengthRef(sharedSecretLength, ESL_SIZEOF_X25519_SHARED_SECRET); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  esl_initECDH()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initECDH(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceX25519) workSpace,
  eslt_Size16 curve)
{
  eslt_ErrorCode retVal;

  if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_SIZEOF_WS_X25519)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  else if (curve != ESL_Curve25519)
  {
    retVal = ESL_ERC_CURVE_NOT_SUPPORTED;
  }
  else
  {
    VSECPRIM_P2VAR_PARA(eslt_WorkSpaceX25519) wsPtr = (VSECPRIM_P2VAR_PARA(eslt_WorkSpaceX25519)) workSpace;

    /* Initialize actCLib X25519 */
    (void)actX25519Init(&wsPtr->wsX25519buffer); /* SBSW_VSECPRIM_CALL_FUNCTION */

    /* Provide backward compatible pointer */
    workSpace->wsX25519 = (VSECPRIM_P2VAR_PARA(eslt_Byte))&(workSpace->wsX25519buffer); /* PRQA S 0310 */ /* MD_VSECPRIM_11.4 */ /* SBSW_VSECPRIM_WRITE_POINTER */

    /* Set workSpace state */
    esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_X25519); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_generateEphemeralKeyPairECDH()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_generateEphemeralKeyPairECDH(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceX25519) workSpace, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2VAR_PARA(eslt_Byte) publicKey,
  VSECPRIM_P2VAR_PARA(eslt_Length) publicKeyLength)
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (publicKey == NULL_PTR) || (publicKeyLength == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_SIZEOF_WS_X25519, ESL_WST_ALGO_X25519)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else if (ESL_ERC_NO_ERROR != (retVal = esl_validateKeyLength(publicKeyLength))) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  else
  {
    /* Key pair generation */
    switch (actX25519GenKeyPair(&workSpace->wsX25519buffer, publicKey, workSpace->header.watchdog)) /* SBSW_VSECPRIM_CALL_FUNCTION */
    {
      case actOK:
      {
        *publicKeyLength = ESL_SIZEOF_X25519_PUBLIC_KEY; /* SBSW_VSECPRIM_WRITE_POINTER */
        break;
      }

      case actEXCEPTION_PRIVKEY:
      {
        retVal = ESL_ERC_RNG_FAILED;
        break;
      }

      default:
      {
        retVal = ESL_ERC_UNRECOVERABLE_ERROR;
        break;
      }
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * esl_importStaticPrivateKeyECDH()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_importStaticPrivateKeyECDH(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceX25519) workSpace, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2CONST_PARA(eslt_Byte) privateKey)
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (privateKey == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_SIZEOF_WS_X25519, ESL_WST_ALGO_X25519)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    /* Import private key */
    if (actX25519ImportPrivateKey(&workSpace->wsX25519buffer, privateKey, workSpace->header.watchdog) != actOK) /* SBSW_VSECPRIM_CALL_FUNCTION */
    {
      retVal = ESL_ERC_UNRECOVERABLE_ERROR;
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 * esl_generateSharedSecretECDH()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_generateSharedSecretECDH(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceX25519) workSpace, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2VAR_PARA(eslt_Byte) publicKey, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2VAR_PARA(eslt_Byte) sharedSecret,
  VSECPRIM_P2VAR_PARA(eslt_Length) sharedSecretLength)
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (publicKey == NULL_PTR) || (sharedSecret == NULL_PTR) || (sharedSecretLength == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_SIZEOF_WS_X25519, ESL_WST_ALGO_X25519)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else if (ESL_ERC_NO_ERROR != (retVal = esl_validateCSLength(sharedSecretLength))) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  else
  {
    /* Generate shared secret */
    (void)actX25519GenSecret(&workSpace->wsX25519buffer, publicKey, sharedSecret, workSpace->header.watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    *sharedSecretLength = ESL_SIZEOF_X25519_SHARED_SECRET; /* SBSW_VSECPRIM_WRITE_POINTER */
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ECDH_25519_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_ECDH.c
 *********************************************************************************************************************/
