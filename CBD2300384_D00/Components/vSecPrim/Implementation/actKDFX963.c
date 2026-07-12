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
/*!        \file  actKDFX963.c
 *        \brief  KDFX963 key derivation function according to ANSI X9.63 using SHA-1 as underlying hash function.
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

#define ACTKDFX963_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actIKDFX963.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTKDFX963_ENABLED == STD_ON)

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
 *  actKDFX963Derive
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(void) actKDFX963Derive(
  VSECPRIM_P2VAR_PARA(actKDFX963STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) passwd, actLengthType passwd_length,
  VSECPRIM_P2CONST_PARA(actU8) salt, actLengthType salt_length,
  VSECPRIM_P2VAR_PARA(actU8) key, actLengthType key_length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2VAR_PARA(actSHASTRUCT) sha = &info->actwsHash;
  VSECPRIM_P2VAR_PARA(actU8) keyPtr = key;
  actU8 counter[4];
  actLengthType l, r, i;

  l = (key_length + actHASH_SIZE_SHA - 1u) / actHASH_SIZE_SHA;
  r = key_length - ((l - 1u) * actHASH_SIZE_SHA);
  /* derive key */
  for (i = 1u; i <= l; ++i)
  {
    /* hash (passwd|counter|salt) */
    (void)actSHAInit(sha); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    (void)actSHAUpdate(sha, passwd, passwd_length, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    actU32toBE(counter, (actU32)i); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    (void)actSHAUpdate(sha, counter, sizeof(counter), watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    (void)actSHAUpdate(sha, salt, salt_length, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

    /* test: is last block? */
    if (i != l)
    {
      (void)actSHAFinalize(sha, &keyPtr[actHASH_SIZE_SHA * (i - 1u)], watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
    }
    else
    {
      (void)actSHAFinalize(sha, info->hashBuf, watchdog); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      actMemCpyByteArray(&keyPtr[actHASH_SIZE_SHA * (i - 1u)], info->hashBuf, r); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
  }

} /* PRQA S 6060 */ /* MD_VSECPRIM_STPAR */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTKDFX963_ENABLED == STD_ON) */
