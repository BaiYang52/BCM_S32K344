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
/*!        \file  actIECKey.c
 *        \brief  Implementation file for actIECKey.h
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
#define ACTIECKEY_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actIECKey.h"
#include "actECTools.h"
#include "actECKey.h"
#include "actECPoint.h"
#include "actECLengthInfo.h"
#include "actBigNum.h"
#include "actUtilities.h"
#include "actConfig.h"

#if (VSECPRIM_ACTIECKEY_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actECGetPrivateKeyLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actLengthType) actECGetPrivateKeyLength(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain)
{
   actLengthType p_bytes, n_bytes;
   actRETURNCODE returncode = actECGetPrimeAndOrderBytes(domain, &p_bytes, &n_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
   return (returncode != actOK) ? 0u : n_bytes;
}

/**********************************************************************************************************************
 *  actECGetPublicKeyCompLength()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actLengthType) actECGetPublicKeyCompLength(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain)
{
   actLengthType p_bytes, n_bytes;
   actRETURNCODE returncode = actECGetPrimeAndOrderBytes(domain, &p_bytes, &n_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
   return (returncode != actOK) ? 0u : p_bytes;
}

/**********************************************************************************************************************
 *  actECInitGenerateKeyPair()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actECInitGenerateKeyPair(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2ROMCONST_PARA(actU8) domain_ext,
  VSECPRIM_P2ROMCONST_PARA(actU8) speedup_ext,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
  actLengthType wksp_len)
{
   return actECInit(domain, domain_ext, speedup_ext, actEC_ALGO_FLAG_KG, &(wksp->baseData), wksp_len); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
}

/**********************************************************************************************************************
 *  actECGenerateKeyPair()
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
 */
VSECPRIM_FUNC(actRETURNCODE) actECGenerateKeyPair(
  boolean generate_privatekey,
  VSECPRIM_P2VAR_PARA(actU8) privatekey,
  VSECPRIM_P2VAR_PARA(actU8) publickey_x,
  VSECPRIM_P2VAR_PARA(actU8) publickey_y,
  VSECPRIM_P2VAR_PARA(actECPSTRUCT) wksp,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
   actRETURNCODE returncode = actOK;
   actRETURNCODE rng_callback_error; /* rng_error is an error code of the actPRNG_GET callback function */

   VSECPRIM_P2VAR_PARA(actECCURVE) Curve = &(wksp->baseData.Curve);
   actLengthType p_length = Curve->p_field.m_length;
   actLengthType p_bytes = Curve->p_field.m_byte_length;
   actLengthType n_length = Curve->n_field.m_length;
   actLengthType n_bytes = Curve->n_field.m_byte_length;
   actECPOINT Q;
   VSECPRIM_P2VAR_PARA(actBNDIGIT) d;

   /* assign workspace members to local key pointer variables */
   d   = wksp->dataPair.privKeys.privKey; /* private key, n_length */
   Q.x = wksp->multG.kG.xcoord; /* buffer to store d*G, p_length in each coordinate */
   Q.y = wksp->multG.kG.ycoord;
   Q.z = wksp->multG.kG.zcoord;

   /* initialization of d */
   if (generate_privatekey == TRUE)
   {
      /* generate d at random */
      rng_callback_error = actBNModRandomize(d, &(Curve->n_field), Curve->t, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      if (rng_callback_error != actOK)
      {
        returncode = rng_callback_error;
      }
      else
      {
        /* privatekey = d */
        actBNOctetString(privatekey, n_bytes, d, n_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      }
   }
   else
   {
      /* d = privatekey */
      actBNSetOctetString(d, n_length, privatekey, n_bytes); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
      /* check 0 < d < n before continue */
      if (actBNIsZero(d, n_length) == TRUE) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
      {
         returncode = actEXCEPTION_PRIVKEY;
      }
      if (actBNCompare(d, Curve->n_field.m, n_length) >= actCOMPARE_EQUAL) /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
      {
         returncode = actEXCEPTION_PRIVKEY;
      }
   }

     /* call ECkgp and extract affine public key */
   if (returncode == actOK)
   {
     /* calculate Q = d * G */
     returncode = actECkgp(d, &Q, wksp, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

     /* write coordinates to publickey */
     if (returncode == actOK)
     {
       actBNOctetString(publickey_x, p_bytes, Q.x, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
       actBNOctetString(publickey_y, p_bytes, Q.y, p_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
     }

     /* wipe local d */
     actBNSetZero(d, n_length); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
   }

   return returncode;
} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTIECKEY_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actIECKey.c
 *********************************************************************************************************************/
