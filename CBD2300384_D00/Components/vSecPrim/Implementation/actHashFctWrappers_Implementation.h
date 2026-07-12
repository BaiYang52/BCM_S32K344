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
/*!        \file  actHashFctWrappers_Implementation.h
 *        \brief  Provides wrapper functions for init, update and finalize routines of different hash functions.
 *
 *      \details  This file provides wrapper functions for init, update and finalize routines for hash functions like
 *                different SHA versions or RIPEMD.
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

#ifndef ACTHASHFCTWRAPPERS_INT_H
# define ACTHASHFCTWRAPPERS_INT_H

# ifdef VSECPRIM_WRAPPER_SHA1  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  include "actISHA.h"
# endif
# if (defined(VSECPRIM_WRAPPER_SHA256) || defined(VSECPRIM_WRAPPER_SHA224))  /* COV_VSECPRIM_NO_SAFE_CONFIG TF tf xf */
#  include "actISHA2_32.h"
# endif
# if (defined(VSECPRIM_WRAPPER_SHA384) || defined(VSECPRIM_WRAPPER_SHA512) || defined(VSECPRIM_WRAPPER_SHA512_224) || defined(VSECPRIM_WRAPPER_SHA512_256)) /* COV_VSECPRIM_NO_SAFE_CONFIG XF xf xf xf xf */
#  include "actISHA2_64.h"
# endif
# ifdef VSECPRIM_WRAPPER_RIPEMD160  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
#  include "actIRMD160.h"
# endif

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/
/* macros to extract actSHAStructs and actRMDStruct from  hashWorkSpaceUnion */
# define ACT_GET_SHA1STRUCT(hashWSU)                                  (&(hashWSU->SHA1ws.wsSHA1))
# define ACT_GET_SHA224STRUCT(hashWSU)                                (&(hashWSU->SHA224ws.wsSHA224))
# define ACT_GET_SHA256STRUCT(hashWSU)                                (&(hashWSU->SHA256ws.wsSHA256))
# define ACT_GET_SHA384STRUCT(hashWSU)                                (&(hashWSU->SHA384ws.wsSHA384))
# define ACT_GET_SHA512STRUCT(hashWSU)                                (&(hashWSU->SHA512ws.wsSHA512))
# define ACT_GET_SHA512_224STRUCT(hashWSU)                            (&(hashWSU->SHA512_224ws.wsSHA512_224))
# define ACT_GET_SHA512_256STRUCT(hashWSU)                            (&(hashWSU->SHA512_256ws.wsSHA512_256))
# define ACT_GET_RMD160STRUCT(hashWSU)                                (&(hashWSU->RIPEMD160ws.wsRIPEMD160))

# define ACT_GET_WATCHDOG(hashWSU, hashWorkSpace)                     (hashWSU->hashWorkSpace.header.watchdog)

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 actHashInit()
 *********************************************************************************************************************/
/*! \brief      Unified interface for calling the "Init" step of different hash functions.
 *  \details    This function forwards its input arguments to the hash init function corresponding to the value of
 *              "hashID", if this function is made available by preprocessor switches.
 *  \param[in,out] hashWSU (in)  Union of workspaces for different hash functions
 *                         (out) Union of workspaces for different hash functions, buffer "H" of actHASHStruct
 *                               contains hashing constants; hi_count, low_count, buffer used are set to 0
 *  \param[in]     hashID        parameter to identify the desired hash algorithm
 *  \return   actEXCEPTION_ALGOID  value of hashID does not correspond to a hash function made available by this
 *                                 wrapper
 *  \return   actOK                Operation successful
 *  \context    TASK
 *  \reentrant  TRUE
 *  \synchronous    TRUE
 *  \pre        hashWSU has to be a valid pointer
 *********************************************************************************************************************/
static VSECPRIM_FUNC(actRETURNCODE) actHashInit(
  VSECPRIM_P2VAR_PARA(eslt_WorkSpaceUnionHashFunctions) hashWSU,
  eslt_HashAlgorithm hashID);

/**********************************************************************************************************************
  actHashUpdate()
 *********************************************************************************************************************/
/*! \brief      Unified interface for calling the "Update" step of different hash functions.
 *  \details    This function forwards its input arguments to the hash update function corresponding to the value of
 *              "hashID", if this function is made available by preprocessor switches.
 *  \param[in,out] hashWSU (in)  Union of workspaces for different hash functions
 *                         (out) Union of workspaces for different hash functions, buffer of actHASHStruct contains
 *  \param[in]     dataIN        data to be hashed
 *  \param[in]     length        length of the buffer storing dataIN
 *  \param[in]     hashID        parameter to identify the desired hash algorithm
 *  \return   actEXCEPTION_ALGOID  value of hashID does not correspond to a hash function made available by this
 *                                 wrapper
 *  \return   actOK                Operation successful
 *  \return   actEXCEPTION_LENGTH  dataIN is longer than maximal data length for hash function
 *  \context    TASK
 *  \reentrant  TRUE
 *  \synchronous    TRUE
 *  \pre        hashWSU, dataIN have to be valid pointers
 *********************************************************************************************************************/
static VSECPRIM_FUNC(actRETURNCODE) actHashUpdate(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceUnionHashFunctions) hashWSU,
VSECPRIM_P2CONST_PARA(actU8) dataIN,
actLengthType length,
eslt_HashAlgorithm hashID);

/**********************************************************************************************************************
  actHashFinalize()
 *********************************************************************************************************************/
/*! \brief      Unified interface for calling the "Finalize" step of different hash functions.
 *  \details    This function forwards its input arguments to the hash finalize function corresponding to the value of
 *              "hashID", if this function is made available by preprocessor switches.
 *  \param[in,out] hashWSU (in)  Union of workspaces for different hash functions
 *                         (out) Union of workspaces for different hash functions, buffer of actHASHStruct set to 0
 *  \param[in,out] hash    (in)  buffer to store the final hash value
 *                         (out) final hash value
 *  \param[in]     hashID        parameter to identify the desired hash algorithm
 *  \return   actEXCEPTION_ALGOID  value of hashID does not correspond to a hash function made available by this
 *                                 wrapper
 *  \return   actOK                Operation successful
 *  \context    TASK
 *  \reentrant  TRUE
 *  \synchronous    TRUE
 *  \pre        hashWSU has to be a valid pointer
 *********************************************************************************************************************/
static VSECPRIM_FUNC(actRETURNCODE) actHashFinalize(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceUnionHashFunctions) hashWSU,
VSECPRIM_P2VAR_PARA(actU8) hash,
eslt_HashAlgorithm hashID);

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  actHashInit()
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
static VSECPRIM_FUNC(actRETURNCODE) actHashInit(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceUnionHashFunctions) hashWSU,
eslt_HashAlgorithm hashID)
{
  actRETURNCODE returnValue;
  switch(hashID)
  {
# ifdef VSECPRIM_WRAPPER_SHA1  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA1:
      returnValue = actSHAInit(ACT_GET_SHA1STRUCT(hashWSU)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA224  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_224:
      returnValue = actSHA224Init(ACT_GET_SHA224STRUCT(hashWSU)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA256
    case ESL_HA_SHA2_256:
      returnValue = actSHA256Init(ACT_GET_SHA256STRUCT(hashWSU)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA384  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_384:
      returnValue = actSHA384Init(ACT_GET_SHA384STRUCT(hashWSU)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA512  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_512:
      returnValue = actSHA512Init(ACT_GET_SHA512STRUCT(hashWSU)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA512_224  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_512_224:
      returnValue = actSHA512_224Init(ACT_GET_SHA512_224STRUCT(hashWSU)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA512_256  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_512_256:
      returnValue = actSHA512_256Init(ACT_GET_SHA512_256STRUCT(hashWSU)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_RIPEMD160  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_RMD160:
      returnValue = actRMD160Init(ACT_GET_RMD160STRUCT(hashWSU)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
    default: /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
      returnValue = actEXCEPTION_ALGOID;
      break;
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  actHashUpdate()
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
static VSECPRIM_FUNC(actRETURNCODE) actHashUpdate(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceUnionHashFunctions) hashWSU,
VSECPRIM_P2CONST_PARA(actU8) dataIN,
actLengthType length,
eslt_HashAlgorithm hashID)
{
  actRETURNCODE returnValue;
  switch(hashID)
  {
# ifdef VSECPRIM_WRAPPER_SHA1  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA1:
      returnValue = actSHAUpdate(ACT_GET_SHA1STRUCT(hashWSU), dataIN, length, ACT_GET_WATCHDOG(hashWSU, SHA1ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA224 /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_224:
      returnValue = actSHA224Update(ACT_GET_SHA224STRUCT(hashWSU), dataIN, length, ACT_GET_WATCHDOG(hashWSU, SHA224ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA256
    case ESL_HA_SHA2_256:
      returnValue = actSHA256Update(ACT_GET_SHA256STRUCT(hashWSU), dataIN, length, ACT_GET_WATCHDOG(hashWSU, SHA256ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA384 /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_384:
      returnValue = actSHA384Update(ACT_GET_SHA384STRUCT(hashWSU), dataIN, length, ACT_GET_WATCHDOG(hashWSU, SHA384ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA512 /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_512:
      returnValue = actSHA512Update(ACT_GET_SHA512STRUCT(hashWSU), dataIN, length, ACT_GET_WATCHDOG(hashWSU, SHA512ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA512_224  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_512_224:
      returnValue = actSHA512_224Update(ACT_GET_SHA512_224STRUCT(hashWSU), dataIN, length, ACT_GET_WATCHDOG(hashWSU, SHA512_224ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA512_256  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_512_256:
      returnValue = actSHA512_256Update(ACT_GET_SHA512_256STRUCT(hashWSU), dataIN, length, ACT_GET_WATCHDOG(hashWSU, SHA512_256ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_RIPEMD160  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_RMD160:
      returnValue = actRMD160Update(ACT_GET_RMD160STRUCT(hashWSU), dataIN, length, ACT_GET_WATCHDOG(hashWSU, RIPEMD160ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
    default: /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
      returnValue = actEXCEPTION_ALGOID;
      break;
  }
  return returnValue;
}

/**********************************************************************************************************************
 *  actHashFinalize()
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
static VSECPRIM_FUNC(actRETURNCODE) actHashFinalize(
VSECPRIM_P2VAR_PARA(eslt_WorkSpaceUnionHashFunctions) hashWSU,
VSECPRIM_P2VAR_PARA(actU8) hash,
eslt_HashAlgorithm hashID)
{
  actRETURNCODE returnValue;
  switch(hashID)
  {
# ifdef VSECPRIM_WRAPPER_SHA1  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
  case ESL_HA_SHA1:
      returnValue = actSHAFinalize(ACT_GET_SHA1STRUCT(hashWSU), hash, ACT_GET_WATCHDOG(hashWSU, SHA1ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA224 /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_224:
      returnValue = actSHA224Finalize(ACT_GET_SHA224STRUCT(hashWSU), hash, ACT_GET_WATCHDOG(hashWSU, SHA224ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA256
    case ESL_HA_SHA2_256:
      returnValue = actSHA256Finalize(ACT_GET_SHA256STRUCT(hashWSU), hash, ACT_GET_WATCHDOG(hashWSU, SHA256ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA384 /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_384:
      returnValue = actSHA384Finalize(ACT_GET_SHA384STRUCT(hashWSU), hash, ACT_GET_WATCHDOG(hashWSU, SHA384ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA512 /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_512:
      returnValue = actSHA512Finalize(ACT_GET_SHA512STRUCT(hashWSU), hash, ACT_GET_WATCHDOG(hashWSU, SHA512ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA512_224  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_512_224:
      returnValue = actSHA512_224Finalize(ACT_GET_SHA512_224STRUCT(hashWSU), hash, ACT_GET_WATCHDOG(hashWSU, SHA512_224ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_SHA512_256  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_SHA2_512_256:
      returnValue = actSHA512_256Finalize(ACT_GET_SHA512_256STRUCT(hashWSU), hash, ACT_GET_WATCHDOG(hashWSU, SHA512_256ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
# ifdef VSECPRIM_WRAPPER_RIPEMD160  /* COV_VSECPRIM_NO_SAFE_CONFIG XF */
    case ESL_HA_RMD160:
      returnValue = actRMD160Finalize(ACT_GET_RMD160STRUCT(hashWSU), hash, ACT_GET_WATCHDOG(hashWSU, RIPEMD160ws)); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      break;
# endif
    default: /* PRQA S 2016 */ /* MD_MSR_EmptyClause */
      returnValue = actEXCEPTION_ALGOID;
      break;
  }
  return returnValue;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTHASHFCTWRAPPERS_INT_H */
/**********************************************************************************************************************
 *  END OF FILE: actHashFctWrappers_Implementation.h
 *********************************************************************************************************************/
