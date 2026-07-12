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
/*!        \file  actIECDSA.c
 *        \brief  Implementation file for actIECDSA.h
 *
 *      \details This file is part of the embedded systems library cvActLib
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

#define ACTIECDSA_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actIECDSA.h"
#include "actIECKey.h"
#include "actECTools.h"
#include "actECDSA.h"
#include "actECPoint.h"
#include "actECLengthInfo.h"
#include "actUtilities.h"
#include "actConfig.h"

#if (VSECPRIM_ACTIECDSA_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actECDSAGetSignatureCompLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actLengthType) actECDSAGetSignatureCompLength(VSECPRIM_P2ROMCONST_PARA(actU8) domain)
{
  return actECGetPrivateKeyLength(domain); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actECDSAInitVerify()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actECDSAInitVerify(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
  actLengthType wksp_len)
{
  return actECInit(domain, domain_ext, (VSECPRIM_P2CONST_PARA(actU8)) NULL_PTR, actEC_ALGO_FLAG_VP, &(wksp->baseData), wksp_len); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actECDSAVerify()
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
VSECPRIM_FUNC(actRETURNCODE) actECDSAVerify(
  VSECPRIM_P2CONST_PARA(actU8) message,
  actLengthType message_len,
  VSECPRIM_P2CONST_PARA(actU8) signature_r,
  actLengthType signature_r_len,
  VSECPRIM_P2CONST_PARA(actU8) signature_s,
  actLengthType signature_s_len,
  VSECPRIM_P2CONST_PARA(actU8) publickey_x,
  VSECPRIM_P2CONST_PARA(actU8) publickey_y,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal = actOK;
  actRETURNCODE resultCompareInputX, resultCompareInputY;
  actLengthType p_length = wksp->baseData.Curve.p_field.m_length;
  actLengthType p_bytes  = wksp->baseData.Curve.p_field.m_byte_length;
  actLengthType n_length = wksp->baseData.Curve.n_field.m_length;
  actLengthType n_bytes  = wksp->baseData.Curve.n_field.m_byte_length;
  actECPOINT Q;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) e;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) s;

  e = wksp->msg.hashed_message;
  r = wksp->signature_r;
  s = wksp->signature_s;
  Q.x = wksp->public_key_x;
  Q.y = wksp->public_key_y;

  /* initialization of e */
  if ((message_len <= 0u) || (message_len > n_bytes))
  {
    retVal = actEXCEPTION_LENGTH;
  }

  if (retVal == actOK)
  {
    actBNSetOctetString(e, n_length, message, message_len); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    if (message_len == n_bytes)
    {
      if (actBNGetBitLength(e, n_length) > actBNGetBitLength(wksp->baseData.Curve.n_field.m, n_length)) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
      {
        retVal = actEXCEPTION_LENGTH;
      }
    }
  }

  if (retVal == actOK)
  {
    if ((signature_r_len <= 0u) || (signature_r_len > n_bytes) || (signature_s_len <= 0u) || (signature_s_len > n_bytes))
    {
      retVal = actSIGNATURE_OUT_OF_RANGE;
    }
    else
    {
      /* initialization of r, s */
      actBNSetOctetString(r, n_length, signature_r, signature_r_len); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      actBNSetOctetString(s, n_length, signature_s, signature_s_len); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

      /* range checks for r and s */
      resultCompareInputX = actBNCompare(r, wksp->baseData.Curve.n_field.m, n_length); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
      resultCompareInputY = actBNCompare(s, wksp->baseData.Curve.n_field.m, n_length); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */

      if ((actBNIsZero(r, n_length) == TRUE) || (resultCompareInputX >= actCOMPARE_EQUAL)) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
      {
        retVal = actSIGNATURE_OUT_OF_RANGE;
      }
      else if ((actBNIsZero(s, n_length) == TRUE) || (resultCompareInputY >= actCOMPARE_EQUAL)) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
      {
        retVal = actSIGNATURE_OUT_OF_RANGE;
      }
      else
      {
        /* initialization of Q */
        actBNSetOctetString(Q.x, p_length, publickey_x, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        actBNSetOctetString(Q.y, p_length, publickey_y, p_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        Q.is_affine = TRUE;
        Q.is_infinity = FALSE;

        /* range check: assert that Qx, Qy are in [0, p-1] */
        resultCompareInputX = actBNCompare(Q.x, wksp->baseData.Curve.p_field.m, p_length); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
        resultCompareInputY = actBNCompare(Q.y, wksp->baseData.Curve.p_field.m, p_length); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */

        if ((resultCompareInputX < actCOMPARE_EQUAL) && (resultCompareInputY < actCOMPARE_EQUAL))
        {
          /* that's it: finally call ECDSA vp */
          retVal = actECDSAvp(e, r, s, &Q, wksp, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        }
        else
        {
          retVal = actEXCEPTION_PUBKEY;
        }
      }
    }
  }

  return retVal;
} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  actECDSAInitSign()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actECDSAInitSign(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext,
  VSECPRIM_P2ROMCONST_PARA(actU8) speedup_ext,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
  actLengthType wksp_len)
{
  return actECInit(domain, domain_ext, speedup_ext, actEC_ALGO_FLAG_SP, &(wksp->baseData), wksp_len); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actECDSASign()
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
VSECPRIM_FUNC(actRETURNCODE) actECDSASign(
  VSECPRIM_P2CONST_PARA(actU8) message,
  actLengthType message_len,
  VSECPRIM_P2CONST_PARA(actU8) privatekey,
  VSECPRIM_P2VAR_PARA(actU8) signature_r,
  VSECPRIM_P2VAR_PARA(actLengthType) signature_r_len,
  VSECPRIM_P2VAR_PARA(actU8) signature_s,
  VSECPRIM_P2VAR_PARA(actLengthType) signature_s_len,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE retVal = actOK;
  actRETURNCODE resultCompareDN;
  actLengthType n_length = wksp->baseData.Curve.n_field.m_length;
  actLengthType n_bytes = wksp->baseData.Curve.n_field.m_byte_length;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) e;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) d;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) r;
  VSECPRIM_P2VAR_PARA(actBNDIGIT) s;

  /* check workspace and signature buffer length */
  if ((*signature_r_len < n_bytes) || (*signature_s_len < n_bytes))
  {
    retVal = actEXCEPTION_MEMORY;
  }
  else if ((message_len <= 0u) || (message_len > n_bytes))
  {
    retVal = actEXCEPTION_LENGTH;
  }
  else
  {
    e = wksp->msg.hashed_message;
    d = wksp->dataPair.privKeys.privKey;
    r = wksp->signature_r;
    s = wksp->signature_s;

    /* initialization of d and e */
    actBNSetOctetString(d, n_length, privatekey, n_bytes);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    actBNSetOctetString(e, n_length, message, message_len); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */

    /* check 0 < d < n before continue */
    resultCompareDN = actBNCompare(d, wksp->baseData.Curve.n_field.m, n_length); /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */

    if ((actBNIsZero(d, n_length) == TRUE) || (resultCompareDN >= actCOMPARE_EQUAL)) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
    {
      retVal = actEXCEPTION_PRIVKEY;
    }
    else if (message_len == n_bytes)
    {
      if (actBNGetBitLength(e, n_length) > actBNGetBitLength(wksp->baseData.Curve.n_field.m, n_length)) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
      {
        retVal = actEXCEPTION_LENGTH;
      }
    }
    else
    {
      /* PRQA S 2013 */ /* MD_MSR_EmptyClause */
    }

    if (retVal == actOK)
    {
      /* call ECDSA sp and calculate signature (r, s) */
      retVal = actECDSAsp(e, d, r, s, wksp, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

      /* if ok, write (r, s) to output */
      if (retVal == actOK)
      {
        actBNOctetString(signature_r, n_bytes, r, n_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        actBNOctetString(signature_s, n_bytes, s, n_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
        *signature_r_len = n_bytes; /* SBSW_VSECPRIM_WRITE_POINTER */
        *signature_s_len = n_bytes; /* SBSW_VSECPRIM_WRITE_POINTER */
      }

      /* wipe local d */
      actBNSetZero(d, n_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    }
  }

  return retVal;

} /* PRQA S 6060, 6080 */ /* MD_VSECPRIM_STPAR, MD_MSR_STMIF */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTIECDSA_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actIECDSA.c
 *********************************************************************************************************************/
