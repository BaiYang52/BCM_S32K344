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
/*!        \file  ESLib_FIPS186.c
 *        \brief  FIPS186 implementation.
 *
 *      \details  Currently the actClib version is used.
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

#define ESLIB_FIPS186_SOURCE

#include "ESLib.h"
#include "ESLib_RNG.h"
#include "ESLib_Helper.h"

/* actCLib includes */
#include "actFIPS186.h"
#include "actISHA.h"

#if (VSECPRIM_FIPS186_ENABLED == STD_ON)

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Hash initialization function */
# define ESL_FIPS_INIT_HASH_FCT(workspace)                            actSHAInit(&((workspace)->wsFIPS186buffer.sha1))
/* Hash update function */
# define ESL_FIPS_UPDATE_HASH_FCT(workspace, inputSize, input)        actSHAUpdate(&((workspace)->wsFIPS186buffer.sha1), (VSECPRIM_P2CONST_PARA(actU8))(input), (actLengthType)(inputSize), (workspace)->header.watchdog)
/* Hash finalization function */
# define ESL_FIPS_FINALIZE_HASH_FCT(workspace, messageDigest)         actSHAFinalize(&((workspace)->wsFIPS186buffer.sha1), (VSECPRIM_P2VAR_PARA(actU8))(messageDigest), (workspace)->header.watchdog)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */
/****************************************************************************
 ** Global Functions
 ***************************************************************************/

/**********************************************************************************************************************
 *  esl_initFIPS186()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_initFIPS186(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceFIPS186) workSpace,
                                              const eslt_Length entropyLength, VSECPRIM_P2CONST_PARA(eslt_Byte) entropy, VSECPRIM_P2VAR_PARA(eslt_Byte) savedState)
{
  eslt_ErrorCode retVal;

  if (workSpace == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeader(&workSpace->header, ESL_MAXSIZEOF_WS_FIPS186)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  {
    /* retVal already set */
  }
  else if (entropy == NULL_PTR)
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if (entropyLength < ESL_SIZEOF_RNGFIPS186_STATE)
  {
    retVal = ESL_ERC_RNG_ENTROPY_TOO_SMALL;
  }
  else
  {
    /* #-- Hash entropy into a 160 bit seed value */
    (void)(ESL_FIPS_INIT_HASH_FCT(workSpace)); /* SBSW_VSECPRIM_CALL_FUNCTION */

    if (ESL_FIPS_UPDATE_HASH_FCT(workSpace, entropyLength, entropy) != actOK)/* SBSW_VSECPRIM_CALL_FUNCTION */
    {
      /* must be actEXCEPTION_LENGTH */
      retVal = ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW;
    }

    if ((retVal == ESL_ERC_NO_ERROR) && (savedState != NULL_PTR))
    {
      if (ESL_FIPS_UPDATE_HASH_FCT(workSpace, ESL_SIZEOF_RNGFIPS186_STATE, savedState) != actOK) /* SBSW_VSECPRIM_CALL_FUNCTION */
      {
        /* must be actEXCEPTION_LENGTH */
        retVal = ESL_ERC_SHA1_TOTAL_LENGTH_OVERFLOW;
      }
    }

    if (retVal == ESL_ERC_NO_ERROR)
    {
      eslt_Byte seed[ESL_SIZEOF_RNGFIPS186_STATE];
      actMemClear(seed, ESL_SIZEOF_RNGFIPS186_STATE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_LOCAL_BUFFER_FIXED_SIZE */

      (void)ESL_FIPS_FINALIZE_HASH_FCT(workSpace, seed); /*lint -e645 */ /* SBSW_VSECPRIM_CALL_FUNCTION */

      /* #-- Init actCLib FIPS */
      actInitializeFIPS186(&workSpace->wsFIPS186buffer, seed); /* SBSW_VSECPRIM_CALL_FUNCTION */

      /* #-- Output a savedState if desired */
      if (savedState != NULL_PTR)
      {
        actGetFIPS186(&workSpace->wsFIPS186buffer, savedState, ESL_SIZEOF_RNGFIPS186_STATE, workSpace->header.watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
      }

      /* Provide backward compatible pointer */
      workSpace->wsFIPS186 = (VSECPRIM_P2VAR_PARA(eslt_Byte))&(workSpace->wsFIPS186buffer); /* PRQA S 0310 */ /* MD_VSECPRIM_11.4 */ /* SBSW_VSECPRIM_WRITE_POINTER */

      /* #-- Set workSpace state */
      esl_SetWorkspaceStatus(&workSpace->header, ESL_WST_ALGO_FIPS186); /* SBSW_VSECPRIM_CALL_FUNCTION */
    }
  }

  return retVal;
} /* PRQA S 6050, 6080 */ /* MD_MSR_STCAL, MD_MSR_STMIF */

/**********************************************************************************************************************
 *  esl_getBytesFIPS186()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_getBytesFIPS186(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceFIPS186) workSpace, const eslt_Length targetLength, VSECPRIM_P2VAR_PARA(eslt_Byte) target) /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (target == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_FIPS186, ESL_WST_ALGO_FIPS186)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    /* #-- Call actCLib FIPS */
    actGetFIPS186(&workSpace->wsFIPS186buffer, target, (actLengthType)targetLength, workSpace->header.watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }
  return retVal;
}

/**********************************************************************************************************************
 *  esl_stirFIPS186()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(eslt_ErrorCode) esl_stirFIPS186(VSECPRIM_P2VAR_PARA(eslt_WorkSpaceFIPS186) workSpace, const eslt_Length inputLength, VSECPRIM_P2VAR_PARA(eslt_Byte) input) /* PRQA S 3673 */ /* MD_VSECPRIM_16.7 */
{
  eslt_ErrorCode retVal;
  if ((workSpace == NULL_PTR) || (input == NULL_PTR))
  {
    retVal = ESL_ERC_PARAMETER_INVALID;
  }
  else if ((retVal = esl_CheckWorkSpaceHeaderAndState(&workSpace->header, ESL_MAXSIZEOF_WS_FIPS186, ESL_WST_ALGO_FIPS186)) != ESL_ERC_NO_ERROR) /* PRQA S 3326 */ /* MD_VSECPRIM_13.1 */ /* SBSW_VSECPRIM_FCT_CALL_P2CONST_PARAM */
  {
    /* retVal was already set */
  }
  else
  {
    eslt_Byte savedState[ESL_SIZEOF_RNGFIPS186_STATE];
    VSECPRIM_P2CONST_PARA(actU8) origState;
    actLengthType i;
    origState = (workSpace->wsFIPS186buffer.X_KEY);
    /* Save the internal state of the FIPS generator */

    for (i = 0; i < actFIPS186_BASE_LENGTH; ++i)
    {
      savedState[i] = origState[i]; /* SBSW_VSECPRIM_WRITE_ARRAY */
    }
    retVal = esl_initFIPS186(workSpace, inputLength, input, savedState); /* SBSW_VSECPRIM_CALL_FUNCTION */
  }

  return retVal;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_FIPS186_ENABLED == STD_ON) */
/**********************************************************************************************************************
 *  END OF FILE: ESLib_FIPS186.c
 *********************************************************************************************************************/
