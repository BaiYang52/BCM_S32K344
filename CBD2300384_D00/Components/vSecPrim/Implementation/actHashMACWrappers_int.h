/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actHashMACWrappers_int.h
 *        \brief  Provides wrapper function for init, update and finalize routines of HMAC-SHA.
 *
 *      \details  This file provides wrapper functions for init, update and finalize routines for HMAC with different
 *                underlying SHA versions which unify those functions so tables of function pointers for init, update
 *                and finalize functions can be set up. Also, default wrappers which return an error code are provided.
 *                This file is part of the embedded systems library cvActLib/ES
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

#ifndef ACTHASHMACWRAPPERS_INT_H
# define ACTHASHMACWRAPPERS_INT_H

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/
# include "actITypes.h"

# ifdef VSECPRIM_WRAPPER_HMAC_SHA1 /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  include "actIHashMAC.h"
# endif
# ifdef VSECPRIM_WRAPPER_HMAC_SHA256
#  include "actIHashMACSHA256.h"
# endif
# ifdef VSECPRIM_WRAPPER_HMAC_SHA384 /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  include "actIHashMACSHA384.h"
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 **********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  actHashMACInitWrapper()
 *********************************************************************************************************************/
/*! \brief          Unified interface for calling the "init" step of different HMAC functions.
 *  \details        This function forwards its input arguments to the HMAC init function corresponding to the value of
 *                  "hashMACidx", if this function is made available by preprocessor switches.
 *  \param[in,out]  info         (in) pointer to HashMACUnion
 *                               (out) initialized context structure
 *  \param[in]      key          MAC key
 *  \param[in]      key_length   length of key in bytes
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \param[in]      hashMACidx   parameter to identify the desired HMAC algorithm
 *  \return         actEXCEPTION_LENGTH  key_len < 1 
 *                  actEXCEPTION_ALGOID  value of hashMACidx does not correspond to a HMAC function made
 *                                       available by this wrapper
 *                  actOK                else
 *  \pre            All pointers need to be valid.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
static VSECPRIM_FUNC(actRETURNCODE) actHashMACInitWrapper(
  VSECPRIM_P2VAR_PARA(actHASHMACUNION) info,
  VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void),
  actU8 hashMACidx);

/**********************************************************************************************************************
 *  actHashMACUpdateWrapper()
 *********************************************************************************************************************/
/*! \brief          Unified interface for calling the "update" step of different HMAC functions.
 *  \details        This function forwards its input arguments to the HMAC update function corresponding to the value of
 *                  "hashMACidx", if this function is made available by preprocessor switches.
 *  \param[in,out]  info         (in) pointer to HashMACUnion
 *                               (out) actualized context structure
 *  \param[in]      dataIN       pointer to data to be hashed
 *  \param[in]      length       length of data in bytes
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \param[in]      hashMACidx   parameter to identify the desired HMAC algorithm
 *  \return         actEXCEPTION_LENGTH  total input more than 2^64 - 1 bit
 *                  actEXCEPTION_ALGOID  value of hashMACidx does not correspond to a HMAC function made
 *                                       available by this wrapper
 *                  actOK                else
 *  \pre            actHashMACInitWrapper() is called once before calling this function
 *                  All pointers need to be valid.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
static VSECPRIM_FUNC(actRETURNCODE) actHashMACUpdateWrapper(
  VSECPRIM_P2VAR_PARA(actHASHMACUNION) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN, actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void),
  actU8 hashMACidx);

/**********************************************************************************************************************
 *  actHashMACFinalizeWrapper()
 *********************************************************************************************************************/
/*! \brief          Unified interface for calling the "finalize" step of different HMAC functions.
 *  \details        This function forwards its input arguments to the HMAC finalize function corresponding to the
 *                  value of "hashMACidx", if this function is made available by preprocessor switches.
 *  \param[in,out]  info         (in) pointer to HashMACUnion
 *                               (out) finalized context structure
 *  \param[in,out]  hash         (in) byte array to contain the hash value
 *                               (out) the final hash value, (big endian of length actHASH_SIZE_SHA)
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \param[in]      hashMACidx   parameter to identify the desired HMAC algorithm
 *  \return         actEXCEPTION_ALGOID  value of hashMACidx does not correspond to a HMAC function made
 *                                       available by this wrapper
 *                  actOK                else
 *  \pre            actHashMACInitWrapper() is called once before calling this function
 *                  All pointers need to be valid.
 *                  The buffer referenced by hash must provide at least actHASH_SIZE_SHA/actHASH_SIZE_SHA256/
 *                  actHASH_SIZE_SHA384 many bytes, depending on the value of hashMACidx.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
static VSECPRIM_FUNC(actRETURNCODE) actHashMACFinalizeWrapper(
  VSECPRIM_P2VAR_PARA(actHASHMACUNION) info, VSECPRIM_P2VAR_PARA(actU8) hash,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void),
  actU8 hashMACidx);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/
/**********************************************************************************************************************
 *  actHashMACInitWrapper
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
static VSECPRIM_FUNC(actRETURNCODE) actHashMACInitWrapper(
  VSECPRIM_P2VAR_PARA(actHASHMACUNION) info, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void),
  actU8 hashMACidx)
{
  actRETURNCODE returnValue;

  switch (hashMACidx)
  {
# ifdef VSECPRIM_WRAPPER_HMAC_SHA1 /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  case actHASHMACSHA1_ALGO_ID:
    returnValue = actHashMACInit(&(info->hmac_sha1), key, key_length, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

# ifdef VSECPRIM_WRAPPER_HMAC_SHA256
  case actHASHMACSHA256_ALGO_ID:
    returnValue = actHashMACSHA256Init(&(info->hmac_sha256), key, key_length, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

# ifdef VSECPRIM_WRAPPER_HMAC_SHA384 /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  case actHASHMACSHA384_ALGO_ID:
    returnValue = actHashMACSHA384Init(&(info->hmac_sha384), key, key_length, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

  default:
    returnValue = actEXCEPTION_ALGOID;
    break;
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  actHashMACUpdateWrapper
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
static VSECPRIM_FUNC(actRETURNCODE) actHashMACUpdateWrapper(
  VSECPRIM_P2VAR_PARA(actHASHMACUNION) info, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2CONST_PARA(actU8) dataIN, actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void),
  actU8 hashMACidx)
{
  actRETURNCODE returnValue;

  switch (hashMACidx)
  {
# ifdef VSECPRIM_WRAPPER_HMAC_SHA1 /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  case actHASHMACSHA1_ALGO_ID:
    returnValue = actHashMACUpdate(&(info->hmac_sha1), dataIN, length, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

# ifdef VSECPRIM_WRAPPER_HMAC_SHA256
  case actHASHMACSHA256_ALGO_ID:
    returnValue = actHashMACSHA256Update(&(info->hmac_sha256), dataIN, length, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

# ifdef VSECPRIM_WRAPPER_HMAC_SHA384 /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  case actHASHMACSHA384_ALGO_ID:
    returnValue = actHashMACSHA384Update(&(info->hmac_sha384), dataIN, length, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

  default:
    returnValue = actEXCEPTION_ALGOID;
    break;
  }

  return returnValue;
}

/**********************************************************************************************************************
 *  actHashMACFinalizeWrapper
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
static VSECPRIM_FUNC(actRETURNCODE) actHashMACFinalizeWrapper(
  VSECPRIM_P2VAR_PARA(actHASHMACUNION) info, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2VAR_PARA(actU8) hash, /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void),
  actU8 hashMACidx)
{
  actRETURNCODE returnValue;

  switch (hashMACidx)
  {
# ifdef VSECPRIM_WRAPPER_HMAC_SHA1 /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  case actHASHMACSHA1_ALGO_ID:
    returnValue = actHashMACFinalize(&(info->hmac_sha1), hash, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

# ifdef VSECPRIM_WRAPPER_HMAC_SHA256
  case actHASHMACSHA256_ALGO_ID:
    returnValue = actHashMACSHA256Finalize(&(info->hmac_sha256), hash, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

# ifdef VSECPRIM_WRAPPER_HMAC_SHA384 /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  case actHASHMACSHA384_ALGO_ID:
    returnValue = actHashMACSHA384Finalize(&(info->hmac_sha384), hash, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    break;
# endif

  default:
    returnValue = actEXCEPTION_ALGOID;
    break;
  }

  return returnValue;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTHASHMACWRAPPERS_INT_H */
