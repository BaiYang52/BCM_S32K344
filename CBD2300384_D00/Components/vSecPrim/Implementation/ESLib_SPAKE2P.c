/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under intellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  ESLib_SPAKE2P.c
 *        \brief  SPAKE2+ implementation.
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

#define ESLIB_SPAKE2P_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ESLib.h"
#include "ESLib_Helper.h"
#include "ESLib_SPAKE2PConstants.h"

/* actCLib includes */
#include "actISPAKE2P.h"

#if (VSECPRIM_SPAKE2_P_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

VSECPRIM_ROM(VSECPRIM_NONE, eslt_Byte) esl_SPAKE2P_pointM_P256[65] =
{
  0x04,
  0x88U, 0x6eU, 0x2fU, 0x97U, 0xacU, 0xe4U, 0x6eU, 0x55U, 0xbaU, 0x9dU, 0xd7U, 0x24U, 0x25U, 0x79U, 0xf2U, 0x99U,
  0x3bU, 0x64U, 0xe1U, 0x6eU, 0xf3U, 0xdcU, 0xabU, 0x95U, 0xafU, 0xd4U, 0x97U, 0x33U, 0x3dU, 0x8fU, 0xa1U, 0x2fU,
  0x5fU, 0xf3U, 0x55U, 0x16U, 0x3eU, 0x43U, 0xceU, 0x22U, 0x4eU, 0x0bU, 0x0eU, 0x65U, 0xffU, 0x02U, 0xacU, 0x8eU,
  0x5cU, 0x7bU, 0xe0U, 0x94U, 0x19U, 0xc7U, 0x85U, 0xe0U, 0xcaU, 0x54U, 0x7dU, 0x55U, 0xa1U, 0x2eU, 0x2dU, 0x20U
};

VSECPRIM_ROM(VSECPRIM_NONE, eslt_Byte) esl_SPAKE2P_pointN_P256[65] =
{
  0x04,
  0xd8U, 0xbbU, 0xd6U, 0xc6U, 0x39U, 0xc6U, 0x29U, 0x37U, 0xb0U, 0x4dU, 0x99U, 0x7fU, 0x38U, 0xc3U, 0x77U, 0x07U,
  0x19U, 0xc6U, 0x29U, 0xd7U, 0x01U, 0x4dU, 0x49U, 0xa2U, 0x4bU, 0x4fU, 0x98U, 0xbaU, 0xa1U, 0x29U, 0x2bU, 0x49U,
  0x07U, 0xd6U, 0x0aU, 0xa6U, 0xbfU, 0xadU, 0xe4U, 0x50U, 0x08U, 0xa6U, 0x36U, 0x33U, 0x7fU, 0x51U, 0x68U, 0xc6U,
  0x4dU, 0x9bU, 0xd3U, 0x60U, 0x34U, 0x80U, 0x8cU, 0xd5U, 0x64U, 0x49U, 0x0bU, 0x1eU, 0x65U, 0x6eU, 0xdbU, 0xe7U
};

# define VSECPRIM_STOP_SEC_CONST_8BIT
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  esl_calcSPAKE2PPreamble_inputCheck()
 *********************************************************************************************************************/
/*! \brief          Parameter input check for SPAKE2+ preamble phase.
 *  \details        -
 *  \param[in]      wsSpakePre         algorithm context buffer, initialized by esl_initSPAKE2PPreamble(..)
 *  \param[in]      pbkdfOutput        PBKDF output, [w0s || w1s] = PBKDF(...)
 *  \param[in]      pbkdfOutputLength  length of the buffer referenced by PBKDFOutput
 *  \param[in]      w0                 buffer to store the result w0 = w0s mod p
 *  \param[in]      w0Length           length of buffer referenced by w0
 *  \param[in]      w1                 buffer to store the result w1 = w1s mod p
 *  \param[in]      w1Length           length of buffer referenced by w1
 *  \param[in]      pointL             buffer to store the coordinates of point L = w1*P
 *  \param[in]      pointLLength       length of the buffer referenced by pointL
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_WS_STATE_INVALID        wrong work space state
 *                  ESL_ERC_INVALID_LENGTH          length of pbkdfOutputLength invalid (i.e., odd)
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_calcSPAKE2PPreamble_inputCheck(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceSPAKE2PPreamble) wsSpakePre,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pbkdfOutput,
  const eslt_Length pbkdfOutputLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) w0,
  VSECPRIM_P2CONST_PARA(eslt_Length) w0Length,
  VSECPRIM_P2CONST_PARA(eslt_Byte) w1,
  VSECPRIM_P2CONST_PARA(eslt_Length) w1Length,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointL,
  VSECPRIM_P2CONST_PARA(eslt_Length) pointLLength);

/***********************************************************************************************************************
 *  esl_initSPAKE2P_inputCheck()
 **********************************************************************************************************************/
/*! \brief       Checks input parameters for SPAKE2+ init function.
 *  \details     -
 *  \param[in]      workSpace    algorithm context buffer
 *  \param[in]      domain       pointer to domain parameter structure
 *  \param[in]      domainExt    pointer to domain parameter extension structure
 *  \param[in]      pointM       Point M on the curve in uncompressed representation, including the encoding byte 0x04
 *  \param[in]      mlength      length of point M in uncompressed representation, including the encoding byte 0x04
 *  \param[in]      pointN       Point N on the curve in uncompressed representation, including the encoding byte 0x04
 *  \param[in]      nlength      length of point N in uncompressed representation, including the encoding byte 0x04
 *  \param[in]      ciphersuite  parameter specifying the ciphersuite (hash function, key derivation function,
 *                               mac algorithm) and the version used by SPAKE (order of keys)
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL or point encoding is incorrect
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_MODE_INVALID            value of ciphersuite is not consistent with supported ciphersuite
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initSPAKE2P_inputCheck(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointM,
  const eslt_Length mlength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointN,
  const eslt_Length nlength,
  eslt_SPAKE2PMode ciphersuite);

/***********************************************************************************************************************
 *  esl_initSPAKE2P_pointCheck()
 **********************************************************************************************************************/
/*! \brief       Checks the formatting of points M and N for SPAKE2+ init function.
 *  \details     -
 *  \param[in]      pointM       Point M on the curve in uncompressed representation, including the encoding byte 0x04
 *  \param[in]      mlength      length of point M in uncompressed representation, including the encoding byte 0x04
 *  \param[in]      pointN       Point N on the curve in uncompressed representation, including the encoding byte 0x04
 *  \param[in]      nlength      length of point N in uncompressed representation, including the encoding byte 0x04
 *  \return         ESL_ERC_PARAMETER_INVALID       a length value is 0, a length value is even or the encoding byte
 *                                                  of one point is incorrect
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            All provided pointers must be valid.
 *  \context     TASK
 *  \reentrant   TRUE
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initSPAKE2P_pointCheck(
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointM,
  const eslt_Length mlength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointN,
  const eslt_Length nlength);

/**********************************************************************************************************************
 *  esl_setPreambleDataPartyBSPAKE2P_inputCheck()
 *********************************************************************************************************************/
/*! \brief          Parameter input check for SPAKE2+ preamble data.
 *  \details        -
 *  \param[in]      workSpace          algorithm context buffer
 *  \param[in]      w0                 buffer storing the value w0
 *  \param[in]      pointL             buffer storing the coordinates of point L = w1*P
 *  \param[in]      pointLLength       length of the buffer referenced by pointL
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL or encoding of pointL is invalid or
 *                                                  pointLLength is invalid
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_WS_STATE_INVALID        wrong work space state
 *                  ESL_ERC_STATE_INVALID           preamble data was already set in this workspace
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_setPreambleDataPartyBSPAKE2P_inputCheck(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) w0,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointL,
  const eslt_Length pointLLength);

/**********************************************************************************************************************
 *  esl_calcSharedSecretSPAKE2P_inputCheck()
 *********************************************************************************************************************/
/*! \brief        Parameter input check for SPAKE2+ calculation of the shared secret.
 *  \details      -
 *  \param[in]    workSpace              algorithm context buffer, initialized by esl_initSPAKE2P(..)
 *  \param[in]    pubVal_x               x-coordinate of the other party's public value
 *  \param[in]    pubVal_y               y-coordinate of the other party's public value
 *  \param[in]    sharedSecret           buffer to store the secret Ke
 *  \param[in]    sharedSecretLength     length of buffer referenced by sharedSecret
 *  \param[in]    confirmationMAC        buffer to store confirmation MAC
 *  \param[in]    confirmationMACLength  length of buffer referenced by confirmationMAC
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL or pubValLength is invalid
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_WS_STATE_INVALID        wrong work space state
 *                  ESL_ERC_STATE_INVALID           esl_calcPubValSPAKE2P() was not called before
 *                  ESL_ERC_BUFFER_TOO_SMALL        memory provided by output buffers not sufficient
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_calcSharedSecretSPAKE2P_inputCheck(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pubVal_x,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pubVal_y,
  VSECPRIM_P2CONST_PARA(eslt_Byte) sharedSecret,
  VSECPRIM_P2CONST_PARA(eslt_Length) sharedSecretLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) confirmationMAC,
  VSECPRIM_P2CONST_PARA(eslt_Length) confirmationMACLength);

/**********************************************************************************************************************
 *  esl_confirmKeySPAKE2P_inputCheck()
 *********************************************************************************************************************/
/*! \brief          Parameter input check for SPAKE2+ key confirmation phase.
 *  \details        -
 *  \param[in]      workSpace          algorithm context buffer
 *  \param[in]      mac                MAC to be confirmed during the key confirmation phase
 *  \param[in]      macLength          length of the MAC to be confirmed
 *  \return         ESL_ERC_PARAMETER_INVALID       input parameter is NULL
 *                  ESL_ERC_WS_TOO_SMALL            work space too small
 *                  ESL_ERC_WS_STATE_INVALID        wrong work space state
 *                  ESL_ERC_STATE_INVALID           shared secret not computed before entering the key confirmation
 *                                                  phase
 *                  ESL_ERC_INVALID_LENGTH          macLength invalid
 *                  ESL_ERC_NO_ERROR                else
 *  \pre            workSpace is initialized by esl_initWorkSpaceHeader(..)
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_confirmKeySPAKE2P_inputCheck(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) mac,
  const eslt_Length macLength);

/***********************************************************************************************************************
 *  esl_SPAKE2P_get_ERC_from_actEXCEPTION()
 **********************************************************************************************************************/
/*! \brief       Maps act return values to esl return values.
 *  \details     -
 *  \param[in]   actRetVal    return value from act-function to be mapped to an esl error code
 *  \return      ESL_ERC_ECC_INTERNAL_ERROR - actRetVal was actEXCEPTION_UNKNOWN
 *               ESL_ERC_PARAMETER_INVALID - actRetVal was actEXCEPTION_NULL
 *               ESL_ERC_WS_TOO_SMALL - actRetVal was actEXCEPTION_MEMORY
 *               ESL_ERC_ECC_DOMAIN_INVALID - actRetVal was actEXCEPTION_DOMAIN
 *               ESL_ERC_ECC_DOMAINEXT_INVALID - actRetVal was actEXCEPTION_DOMAIN_EXT:
 *               ESL_ERC_ECC_INTERNAL_ERROR - actRetVal was actEXCEPTION_POINT:
 *               ESL_ERC_CURVE_NOT_SUPPORTED - actRetVal was actEXCEPTION_LENGTH:
 *               ESL_ERC_PARAMETER_INVALID - actRetVal was actEXCEPTION_INPUT_LENGTH:
 *               ESL_ERC_ECC_INTERNAL_ERROR - actRetVal was actEXCEPTION_POINT
 *               ESL_ERC_BUFFER_TOO_SMALL - actRetVal was actEXCEPTION_OUTPUT_BUFFER
 *               ESL_ERC_ERROR  all other cases
 *  \pre         -
 *  \context     TASK
 *  \reentrant   TRUE, for different workspaces
 *  \synchronous TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_SPAKE2P_get_ERC_from_actEXCEPTION(
  actRETURNCODE actRetVal);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  esl_calcSPAKE2PPreamble_inputCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_calcSPAKE2PPreamble_inputCheck(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceSPAKE2PPreamble) wsSpakePre,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pbkdfOutput,
  const eslt_Length pbkdfOutputLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) w0,
  VSECPRIM_P2CONST_PARA(eslt_Length) w0Length,
  VSECPRIM_P2CONST_PARA(eslt_Byte) w1,
  VSECPRIM_P2CONST_PARA(eslt_Length) w1Length,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointL,
  VSECPRIM_P2CONST_PARA(eslt_Length) pointLLength)
{
  eslt_ErrorCode retVal;

  if ((wsSpakePre == NULL_PTR) || (pbkdfOutput == NULL_PTR) ||
      (w0 == NULL_PTR) || (w0Length == NULL_PTR) ||
      (w1 == NULL_PTR) || (w1Length == NULL_PTR) ||
      (pointL == NULL_PTR) || (pointLLength == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&wsSpakePre->header, ESL_SIZEOF_WS_SPAKE2PPRE, ESL_WST_ALGO_SPAKE2P_PREAMBLE)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else if (Esl_ValueIsOdd(pbkdfOutputLength) == TRUE)
  {
    retVal = ESL_ERC_INVALID_LENGTH;
  }
  else
  {
    retVal = ESL_ERC_NO_ERROR;
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_initSPAKE2P_inputCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initSPAKE2P_inputCheck(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointM,
  const eslt_Length mlength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointN,
  const eslt_Length nlength,
  eslt_SPAKE2PMode ciphersuite)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (domain == NULL_PTR) || (domainExt == NULL_PTR) ||
      (pointM == NULL_PTR) || (pointN == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_SIZEOF_WS_SPAKE2P)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  else if ((ciphersuite != ESL_SPAKE2P_MODE_CIPHERSUITE_8_1) && (ciphersuite != ESL_SPAKE2P_MODE_CIPHERSUITE_8_2))
  {
    retVal = ESL_ERC_MODE_INVALID;
  }
  else if ((retVal = esl_initSPAKE2P_pointCheck(pointM, mlength, pointN, nlength)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    retVal = ESL_ERC_NO_ERROR;
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_initSPAKE2P_pointCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_initSPAKE2P_pointCheck(
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointM,
  const eslt_Length mlength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointN,
  const eslt_Length nlength)
{
  eslt_ErrorCode retVal;

  if ((mlength == 0u) || (nlength == 0u))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((Esl_ValueIsEven(mlength) == TRUE) || (Esl_ValueIsEven(nlength) == TRUE))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((pointM[0] != ESL_UNCOMPRESSED_POINT_ENCODING_BYTE) || (pointN[0] != ESL_UNCOMPRESSED_POINT_ENCODING_BYTE))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else
  {
    retVal = ESL_ERC_NO_ERROR;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_setPreambleDataPartyBSPAKE2P_inputCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_setPreambleDataPartyBSPAKE2P_inputCheck(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) w0,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointL,
  const eslt_Length pointLLength)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (w0 == NULL_PTR) || (pointL == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((pointL[0] != ESL_UNCOMPRESSED_POINT_ENCODING_BYTE) || (Esl_ValueIsEven(pointLLength) == TRUE))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_SIZEOF_WS_SPAKE2P, ESL_WST_ALGO_SPAKE2P)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else if (workSpace->workSpaceSpake.algoState != ESL_SPAKE2P_PREAMBLE_DATA_NOT_SET)
  {
    retVal = ESL_ERC_STATE_INVALID;
  }
  else
  {
    retVal = ESL_ERC_NO_ERROR;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_calcSharedSecretSPAKE2P_inputCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_calcSharedSecretSPAKE2P_inputCheck(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pubVal_x,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pubVal_y,
  VSECPRIM_P2CONST_PARA(eslt_Byte) sharedSecret,
  VSECPRIM_P2CONST_PARA(eslt_Length) sharedSecretLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) confirmationMAC,
  VSECPRIM_P2CONST_PARA(eslt_Length) confirmationMACLength)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (pubVal_x == NULL_PTR) || (pubVal_y == NULL_PTR) ||
    (sharedSecret == NULL_PTR) || (sharedSecretLength == NULL_PTR) ||
    (confirmationMAC == NULL_PTR) || (confirmationMACLength == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_SIZEOF_WS_SPAKE2P, ESL_WST_ALGO_SPAKE2P)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else if (workSpace->workSpaceSpake.algoState != ESL_SPAKE2P_PUBLIC_VALUE_CALCULATED)
  {
    retVal = ESL_ERC_STATE_INVALID;
  }
  else if ((*sharedSecretLength < (ESL_SIZEOF_SHA256_DIGEST / 2u)) || (*confirmationMACLength < ESL_SIZEOF_AES128_BLOCK))
  {
    retVal = ESL_ERC_BUFFER_TOO_SMALL;
  }
  else
  {
    retVal = ESL_ERC_NO_ERROR;
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_confirmKeySPAKE2P_inputCheck()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_confirmKeySPAKE2P_inputCheck(
  VSECPRIM_P2CONST_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) mac,
  const eslt_Length macLength)
{
  eslt_ErrorCode retVal;

  if ((workSpace == NULL_PTR) || (mac == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_SIZEOF_WS_SPAKE2P, ESL_WST_ALGO_SPAKE2P)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else if (workSpace->workSpaceSpake.algoState != ESL_SPAKE2P_SHARED_SECRET_CALCULATED)
  {
    retVal = ESL_ERC_STATE_INVALID;
  }
  else if (macLength != ESL_SIZEOF_AES128_BLOCK)
  {
    retVal = ESL_ERC_INVALID_LENGTH;
  }
  else
  {
    retVal = ESL_ERC_NO_ERROR;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_SPAKE2P_get_ERC_from_actEXCEPTION()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(eslt_ErrorCode) esl_SPAKE2P_get_ERC_from_actEXCEPTION(
  actRETURNCODE actRetVal)
{
  eslt_ErrorCode retVal;

  switch (actRetVal)
  {
  case actEXCEPTION_NULL:
    retVal = ESL_ERC_PARAMETER_INVALID;
    break;

  case actEXCEPTION_MEMORY:
    retVal = ESL_ERC_WS_TOO_SMALL;
    break;

  case actEXCEPTION_DOMAIN:
    retVal = ESL_ERC_ECC_DOMAIN_INVALID;
    break;

  case actEXCEPTION_DOMAIN_EXT:
    retVal = ESL_ERC_ECC_DOMAINEXT_INVALID;
    break;

  case actEXCEPTION_LENGTH:
    retVal = ESL_ERC_CURVE_NOT_SUPPORTED;
    break;

  case actEXCEPTION_INPUT_LENGTH:
    retVal = ESL_ERC_PARAMETER_INVALID;
    break;

  case actEXCEPTION_POINT:
    retVal = ESL_ERC_ECC_INTERNAL_ERROR;
    break;

  case actEXCEPTION_OUTPUT_BUFFER:
    retVal = ESL_ERC_BUFFER_TOO_SMALL;
    break;

  default:
    retVal = ESL_ERC_ERROR;
    break;
  }

  return retVal;
}

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  esl_initSPAKE2PPreamble()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_initSPAKE2PPreamble(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2PPreamble) wsSpakePre,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  if ((wsSpakePre == NULL_PTR) || (domain == NULL_PTR) || (domainExt == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&wsSpakePre->header, ESL_SIZEOF_WS_SPAKE2PPRE)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    actRetVal = actSPAKE2PPreambleInit(&wsSpakePre->wsSPAKEPreamble, domain, domainExt, ESL_SIZEOF_WS_SPAKE2PPRE); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal != actOK)
    {
      retVal = esl_SPAKE2P_get_ERC_from_actEXCEPTION(actRetVal);
    }
    else
    {
      /* set workSpace state */
      esl_SetWorkspaceStatus(&wsSpakePre->header, ESL_WST_ALGO_SPAKE2P_PREAMBLE); /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_calcSPAKE2PPreamble()
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
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_calcSPAKE2PPreamble(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2PPreamble) wsSpakePre,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pbkdfOutput,
  const eslt_Length pbkdfOutputLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) w0,
  VSECPRIM_P2VAR_PARA(eslt_Length) w0Length,
  VSECPRIM_P2VAR_PARA(eslt_Byte) w1,
  VSECPRIM_P2VAR_PARA(eslt_Length) w1Length,
  VSECPRIM_P2VAR_PARA(eslt_Byte) pointL,
  VSECPRIM_P2VAR_PARA(eslt_Length) pointLLength)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  actLengthType w0_length, w1_length, pointL_length;

  if ((retVal = esl_calcSPAKE2PPreamble_inputCheck(wsSpakePre, pbkdfOutput, pbkdfOutputLength, w0, w0Length, w1, w1Length, pointL, pointLLength)) != ESL_ERC_NO_ERROR)  /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    w0_length = (actLengthType)(*w0Length);
    w1_length = (actLengthType)(*w1Length);
    pointL_length = (actLengthType)(*pointLLength);

    actRetVal = actSPAKE2PPreambleCalc(&wsSpakePre->wsSPAKEPreamble, pbkdfOutput, (actLengthType)pbkdfOutputLength, w0, &w0_length, w1, &w1_length, pointL, &pointL_length, wsSpakePre->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal != actOK)
    {
      if (actRetVal == actEXCEPTION_MEMORY)
      {
        retVal = ESL_ERC_BUFFER_TOO_SMALL;
      }
      else if (actRetVal == actEXCEPTION_INPUT_LENGTH)
      {
        retVal = ESL_ERC_INVALID_LENGTH;
      }
      else
      {
        retVal = ESL_ERC_ERROR;
      }

      *w0Length = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
      *w1Length = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
      *pointLLength = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else
    {
      *w0Length = (eslt_Length)(w0_length); /* SBSW_VSECPRIM_WRITE_POINTER */
      *w1Length = (eslt_Length)(w1_length); /* SBSW_VSECPRIM_WRITE_POINTER */
      *pointLLength = (eslt_Length)(pointL_length); /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_initSPAKE2P()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initSPAKE2P(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomain) domain,
  VSECPRIM_P2ROMCONST_PARA(eslt_EccDomainExt) domainExt,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointM,
  const eslt_Length mlength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointN,
  const eslt_Length nlength,
  eslt_SPAKE2PMode ciphersuite)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  if ((retVal = esl_initSPAKE2P_inputCheck(workSpace, domain, domainExt, pointM, mlength, pointN, nlength, ciphersuite)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    /* Initialize actCLib SPAKE2+ */
    actRetVal = actSPAKE2PInit(&workSpace->workSpaceSpake, domain, domainExt, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      pointM, mlength, pointN, nlength, ESL_SIZEOF_WS_SPAKE2P);

    if (actRetVal != actOK)
    {

      retVal = esl_SPAKE2P_get_ERC_from_actEXCEPTION(actRetVal);
    }
    else
    {
      retVal = ESL_ERC_NO_ERROR;

      /* set ciphersuite, state and workSpace status */
      workSpace->workSpaceSpake.ciphersuite = ciphersuite; /* SBSW_VSECPRIM_WRITE_POINTER */
      workSpace->workSpaceSpake.algoState = ESL_SPAKE2P_PREAMBLE_DATA_NOT_SET; /* SBSW_VSECPRIM_WRITE_POINTER */
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_SPAKE2P); /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_setPreambleDataPartyASPAKE2P()
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
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_setPreambleDataPartyASPAKE2P(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) w0,
  const eslt_Length w0length,
  VSECPRIM_P2CONST_PARA(eslt_Byte) w1,
  const eslt_Length w1length)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  if ((workSpace == NULL_PTR) || (w0 == NULL_PTR) || (w1 == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_SIZEOF_WS_SPAKE2P, ESL_WST_ALGO_SPAKE2P)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else if (workSpace->workSpaceSpake.algoState != ESL_SPAKE2P_PREAMBLE_DATA_NOT_SET)
  {
    retVal = ESL_ERC_STATE_INVALID;
  }
  else
  {
    actRetVal = actSPAKE2PSetPreambleData(&workSpace->workSpaceSpake, w0, w0length, w1, w1length, NULL_PTR, 0); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal != actOK)
    {
      retVal = ESL_ERC_PARAMETER_INVALID;
    }
    else
    {
      workSpace->workSpaceSpake.algoState = ESL_SPAKE2P_PREAMBLE_DATA_SET; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_setPreambleDataPartyBSPAKE2P()
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
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_setPreambleDataPartyBSPAKE2P(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) w0,
  const eslt_Length w0length,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pointL,
  const eslt_Length pointLLength)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  if ((retVal = esl_setPreambleDataPartyBSPAKE2P_inputCheck(workSpace, w0, pointL, pointLLength)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    actRetVal = actSPAKE2PSetPreambleData(&workSpace->workSpaceSpake, w0, w0length, NULL_PTR, 0, pointL, pointLLength); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal != actOK)
    {
      retVal = ESL_ERC_PARAMETER_INVALID;
    }
    else
    {
      workSpace->workSpaceSpake.algoState = ESL_SPAKE2P_PREAMBLE_DATA_SET; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_calcPubValSPAKE2P()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_calcPubValSPAKE2P(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2VAR_PARA(eslt_Byte) pubVal_x,
  VSECPRIM_P2VAR_PARA(eslt_Byte) pubVal_y,
  VSECPRIM_P2VAR_PARA(eslt_Length) pubValLength)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  actLengthType pubVal_length;

  if ((workSpace == NULL_PTR) || (pubVal_x == NULL_PTR) || (pubVal_y == NULL_PTR) || (pubValLength == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_SIZEOF_WS_SPAKE2P, ESL_WST_ALGO_SPAKE2P)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else if (workSpace->workSpaceSpake.algoState != ESL_SPAKE2P_PREAMBLE_DATA_SET)
  {
    retVal = ESL_ERC_STATE_INVALID;
  }
  else
  {
    pubVal_length = (actLengthType)(*pubValLength);
    actRetVal = actSPAKE2PCalcPubVal(&(workSpace->workSpaceSpake), pubVal_x, pubVal_y, &pubVal_length, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal != actOK)
    {
      retVal = esl_SPAKE2P_get_ERC_from_actEXCEPTION(actRetVal);
    }
    else
    {
      workSpace->workSpaceSpake.algoState = ESL_SPAKE2P_PUBLIC_VALUE_CALCULATED; /* SBSW_VSECPRIM_WRITE_POINTER */
      *pubValLength = (eslt_Length)pubVal_length; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
}

/**********************************************************************************************************************
 *  esl_setAdditionalInformationSPAKE2P()
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
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_setAdditionalInformationSPAKE2P(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) infoA,
  const eslt_Length infoALength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) infoB,
  const eslt_Length infoBLength,
  VSECPRIM_P2CONST_PARA(eslt_Byte) aad,
  const eslt_Length aadLength)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_SIZEOF_WS_SPAKE2P, ESL_WST_ALGO_SPAKE2P)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else if (workSpace->workSpaceSpake.algoState >= ESL_SPAKE2P_SHARED_SECRET_CALCULATED)
  {
    retVal = ESL_ERC_STATE_INVALID;
  }
  else
  {
    actRetVal = actSPAKE2PSetAdditionalInformation(&workSpace->workSpaceSpake, &workSpace->workSpaceSHA, infoA, infoALength, infoB, infoBLength, aad, aadLength, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if (actRetVal == actEXCEPTION_INPUT_LENGTH)
    {
      retVal = ESL_ERC_INVALID_LENGTH;
    }
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_calcSharedSecretSPAKE2P()
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
VSECPRIM_FUNC(eslt_ErrorCode) esl_calcSharedSecretSPAKE2P(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pubVal_x,
  VSECPRIM_P2CONST_PARA(eslt_Byte) pubVal_y,
  const eslt_Length pubValLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) sharedSecret,
  VSECPRIM_P2VAR_PARA(eslt_Length) sharedSecretLength,
  VSECPRIM_P2VAR_PARA(eslt_Byte) confirmationMAC,
  VSECPRIM_P2VAR_PARA(eslt_Length) confirmationMACLength)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;

  if ((retVal = esl_calcSharedSecretSPAKE2P_inputCheck(workSpace, pubVal_x, pubVal_y, sharedSecret, sharedSecretLength, confirmationMAC, confirmationMACLength)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    actRetVal = actSPAKE2PCalcSharedSecret(&(workSpace->workSpaceSpake), &(workSpace->workSpaceSHA),
                                           &(workSpace->workSpaceHKDF), &(workSpace->workSpaceCMAC), /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                                           pubVal_x, pubVal_y, pubValLength,
                                           sharedSecret, confirmationMAC, workSpace->header.watchdog);

    if (actRetVal != actOK)
    {
      retVal = esl_SPAKE2P_get_ERC_from_actEXCEPTION(actRetVal);

      *sharedSecretLength = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
      *confirmationMACLength = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else
    {
      *sharedSecretLength = ESL_SIZEOF_SHA256_DIGEST / 2u; /* SBSW_VSECPRIM_WRITE_POINTER */
      *confirmationMACLength = ESL_SIZEOF_AES128_BLOCK; /* SBSW_VSECPRIM_WRITE_POINTER */
      workSpace->workSpaceSpake.algoState = ESL_SPAKE2P_SHARED_SECRET_CALCULATED; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }

  return retVal;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

/**********************************************************************************************************************
 *  esl_confirmKeySPAKE2P()
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
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_confirmKeySPAKE2P(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceSPAKE2P) workSpace,
  VSECPRIM_P2CONST_PARA(eslt_Byte) mac,
  const eslt_Length macLength)
{
  eslt_ErrorCode retVal;
  actRETURNCODE actRetVal;
  eslt_Byte confirmationMac[actAES_BLOCK_SIZE];

  if ((retVal = esl_confirmKeySPAKE2P_inputCheck(workSpace, mac, macLength)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal already set */
  }
  else
  {
    actRetVal = actSPAKE2PConfirmKey(&workSpace->workSpaceSpake, &workSpace->workSpaceCMAC, mac, confirmationMac, workSpace->header.watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_AND_LOCAL_BUFFER */

    if (actRetVal != actOK)
    {
      retVal = ESL_ERC_INCORRECT_MAC;
    }

    esl_ResetAndClearWorkspace(&workSpace->header, &workSpace->workSpaceSpake); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_SPAKE2P_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: ESLib_SPAKE2P.c
 *********************************************************************************************************************/
