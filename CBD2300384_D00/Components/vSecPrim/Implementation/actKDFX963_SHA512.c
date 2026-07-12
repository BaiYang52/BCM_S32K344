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
/*!        \file  actKDFX963_SHA512.c
 *        \brief  KDFX963 key derivation function according to ANSI X9.63 using SHA-512 as underlying hash function.
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

#define ACTKDFX963_SHA512_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actIKDFX963_SHA512.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTKDFX963SHA512_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  LOCAL CONSTANT MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION MACROS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

 /***********************************************************************************************************************
 *  actKDFX963_SHA512_Derive
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(void) actKDFX963_SHA512_Derive(
  VSECPRIM_P2VAR_PARA(actKDFX963SHA512) info,
  VSECPRIM_P2CONST_PARA(actU8) passwd, actLengthType passwd_length,
  VSECPRIM_P2CONST_PARA(actU8) salt, actLengthType salt_length,
  VSECPRIM_P2VAR_PARA(actU8) key, actLengthType key_length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2VAR_PARA(actSHA512STRUCT) sha = &info->actwsHash;

  actU8   counter[4];
  actLengthType     blocks, remain, b;

  blocks = (key_length + actHASH_SIZE_SHA512 - 1u) / actHASH_SIZE_SHA512;
  remain = key_length % actHASH_SIZE_SHA512;
  /* derive key */
  for (b = 1u; b <= blocks; ++b) {
    /* hash (passwd|counter|salt) */
    actU32toBE(counter, (actU32)b); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    (void)actSHA512Init(sha); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    (void)actSHA512Update(sha, passwd, passwd_length, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    (void)actSHA512Update(sha, counter, sizeof(counter), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    (void)actSHA512Update(sha, salt, salt_length, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    if ((b != blocks)       /* last block? */
      || (0u == remain)) {    /* complete last block required? */
          /* complete (last) block */
      (void)actSHA512Finalize(sha, &key[actHASH_SIZE_SHA512 * (b - 1u)], watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
    else
    {
      /* partial last block */
      (void)actSHA512Finalize(sha, info->hashBuf, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      actMemCpyByteArray(&key[actHASH_SIZE_SHA512 * (b - 1u)], info->hashBuf, remain);  /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
  }

} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTKDFX963SHA512_ENABLED == STD_ON) */
