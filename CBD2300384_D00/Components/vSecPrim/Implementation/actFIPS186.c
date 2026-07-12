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
/*!        \file  actFIPS186.c
 *        \brief  FIPS 186 PRNG implementation.
 *
 *      \details Currently the actClib version is used.
 *               This file is part of the embedded systems library actCLib
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

#define ACTFIPS186_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include "actFIPS186.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTFIPS186_ENABLED == STD_ON)

/* PRQA S 0850 EOF */ /* MD_MSR_MacroArgumentEmpty */

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
  pseudoRandom()
 **********************************************************************************************************************/
/*! \brief      PRNG implementation
 *  \details    This function is the actual PRNG implementation according to FIPS-186.2.
 *  \param[in,out] info     In: Pointer to the FIPS-186.2 context structure. Out: Updated context structure.
 *  \param[in]     watchdog Pointer to watchdog trigger function
 *  \pre           Workspace must be initialized
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 *********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) pseudoRandom(
   VSECPRIM_P2VAR_PARA(actFIPS186STRUCT) info, VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
  pseudoRandom()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) pseudoRandom(
  VSECPRIM_P2VAR_PARA(actFIPS186STRUCT) info,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2VAR_PARA(actSHASTRUCT) sha1 = &(info->sha1); /* PRQA S 0781 */ /* MD_VSECPRIM_5.6 */
  VSECPRIM_P2VAR_PARA(actU8) X = info->sha1.buffer;
  VSECPRIM_P2VAR_PARA(actU8) X_KEY = info->X_KEY; /* PRQA S 0781 */ /* MD_VSECPRIM_5.6 */

  /* X = XKEY mod 2^b, b = 160 */
  actMemCpyByteArray(X, X_KEY, actFIPS186_BASE_LENGTH); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* X = SHA1(X) */

  /* We don't finalize SHA-1 and use the zero padding of X */
  if (actOK == actSHAInit(sha1)) /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    if (actOK == actSHAUpdate(sha1, (VSECPRIM_P2CONST_PARA(actU8)) X, actHASH_BLOCK_SIZE_SHA, watchdog)) /* SBSW_VSECPRIM_CALL_FUNCTION */
    {
      /* DON'T finalize SHA-1 but get current digest */
      actSHAGetDigest(sha1, X, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

      /* XKEY = (XKEY + X + 1) mod 2^160 */
      (void)actAddBE(X_KEY, (VSECPRIM_P2CONST_PARA(actU8))X, actFIPS186_BASE_LENGTH, 1u); /* SBSW_VSECPRIM_CALL_FUNCTION */

      /* one block of random available */
      info->bytes_remain = actFIPS186_BASE_LENGTH; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
  }
}

/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/**********************************************************************************************************************
  actInitializeFIPS186()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actInitializeFIPS186(VSECPRIM_P2VAR_PARA(actFIPS186STRUCT) info, VSECPRIM_P2CONST_PARA(actU8) seed) /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
{
  /* Prepare workspaces */
  actMemCpyByteArray(info->X_KEY, seed, actFIPS186_BASE_LENGTH); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actMemset(info->sha1.buffer, 0u, actHASH_BLOCK_SIZE_SHA); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  info->bytes_remain = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */
}

/**********************************************************************************************************************
  actGetFIPS186()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(void) actGetFIPS186(
  VSECPRIM_P2VAR_PARA(actFIPS186STRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) output, actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2CONST_PARA(actU8) X = info->sha1.buffer;
  actLengthType i = 0u;

  while (i < length)
  {
    if (info->bytes_remain > 0u)
    {
      output[i] = X[actFIPS186_BASE_LENGTH - info->bytes_remain]; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
      i++;
      info->bytes_remain--; /* SBSW_VSECPRIM_WRITE_POINTER */
    }
    else
    {
      pseudoRandom(info, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
  }
}

/**********************************************************************************************************************
  actWriteFIPS186()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(void) actWriteFIPS186(
  VSECPRIM_P2VAR_PARA(actFIPS186STRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) new_seed)
{
  actU8 i;

  /* XOR new seed with seed state */
  for (i = 0u; i < actFIPS186_BASE_LENGTH; ++i)
  {
    info->X_KEY[i] ^= new_seed[i]; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  }
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTFIPS186_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actFIPS186.c
 *********************************************************************************************************************/
