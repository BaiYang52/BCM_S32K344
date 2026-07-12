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
/*!        \file  actSipHash.c
 *        \brief  Implementation of SipHash
 *
 *      \details  Implements SipHash according to 'SipHash: a short-input PRF' from Jean-Philippe Aumasson and Daniel
 *                Bernstein
 *                This file is part of the embedded systems library cvActLib
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
#define ACTSIPHASH_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "actISipHash.h"
#include "actUtilities.h"
#include "actWatchdog.h"

#if (VSECPRIM_ACTSIPHASH_ENABLED == STD_ON)

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Check that 64 bit type is supported by platform. */
# ifndef ACT_PLATFORM_UINT64_AVAILABLE
#  error SipHahs is only running on platforms supporting uint64. Make sure platforms supports it or disable SipHash
# endif

/* SipHash constants */
# define SIP_HASH_CONST_1                                             (0x736f6d6570736575uLL)
# define SIP_HASH_CONST_2                                             (0x646f72616e646f6duLL)
# define SIP_HASH_CONST_3                                             (0x6c7967656e657261uLL)
# define SIP_HASH_CONST_4                                             (0x7465646279746573uLL)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  actSipHashBlockUpdate
 **********************************************************************************************************************/
/*! \brief          Updates SipHash calculation
 *  \details        -
 *  \param[in,out] info  Pointer to context structure
 *  \param[in]      dataIN  Data block to be processed
 *  \param[in]      rounds  Round count
 *  \pre            info must be a valid workspace pointer
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces.
 *  \synchronous    TRUE
 **********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actSipHashBlockUpdate(
  VSECPRIM_P2VAR_PARA(actSipHashSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actU8 rounds);

/***********************************************************************************************************************
 *  actSipHashRound
 **********************************************************************************************************************/
/*! \brief         Performs the SipHash round calculations
 *  \details       -
 *  \param[in,out] pv0  Pointer to v0
 *  \param[in,out] pv1  Pointer to v1
 *  \param[in,out] pv2  Pointer to v2
 *  \param[in,out] pv3  Pointer to v3
 *  \param[in]     rounds  Round count
 *  \pre           pv0, pv1, pv2 and pv3 must point to variables of size 64 bit
 *  \context       TASK
 *  \reentrant     TRUE
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actSipHashRound(
  VSECPRIM_P2VAR_PARA(actU64) pv0,
  VSECPRIM_P2VAR_PARA(actU64) pv1,
  VSECPRIM_P2VAR_PARA(actU64) pv2,
  VSECPRIM_P2VAR_PARA(actU64) pv3,
  actU8 rounds);

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  actSipHashBlockUpdate
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actSipHashBlockUpdate(
  VSECPRIM_P2VAR_PARA(actSipHashSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actU8 rounds)
{
  actU64 m;
  /* Process block data */
  m = actU8toU64(dataIN);
  info->v3 ^= m;                                                        /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */

  actSipHashRound(&info->v0, &info->v1, &info->v2, &info->v3, rounds);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */

  info->v0 ^= m;                                                        /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
}

/***********************************************************************************************************************
 *  actSipHashRound
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actSipHashRound(
  VSECPRIM_P2VAR_PARA(actU64) pv0,
  VSECPRIM_P2VAR_PARA(actU64) pv1,
  VSECPRIM_P2VAR_PARA(actU64) pv2,
  VSECPRIM_P2VAR_PARA(actU64) pv3,
  actU8 rounds)
{
  actU8 i;

  /* Perform the SipHash round calculations */
  for (i = 0; i < rounds; i++)
  {
    *pv0 += *pv1;               /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    *pv1 = actRotl64(*pv1, 13); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    *pv1 ^= *pv0;               /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    *pv0 = actRotl64(*pv0, 32); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    *pv2 += *pv3;               /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    *pv3 = actRotl64(*pv3, 16); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    *pv3 ^= *pv2;               /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    *pv0 += *pv3;               /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    *pv3 = actRotl64(*pv3, 21); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    *pv3 ^= *pv0;               /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    *pv2 += *pv1;               /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    *pv1 = actRotl64(*pv1, 17); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    *pv1 ^= *pv2;               /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    *pv2 = actRotl64(*pv2, 32); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
  }
}

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  actSipHashInit
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSipHashInit(
  VSECPRIM_P2VAR_PARA(actSipHashSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) key,
  actLengthType key_length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog) (void))
{
  actRETURNCODE result;
  actU64 k[2];

  if (key == NULL_PTR)
  {
    result = actEXCEPTION_NULL;
  }
  else if (key_length != 16u)
  {
    result = actEXCEPTION_LENGTH;
  }
  else
  {
    k[0] = actU8toU64(key);     /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */
    k[1] = actU8toU64(&key[8]); /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_LOCAL_BUFFER_FIXED_INDEX */

    actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

    /* Initialize internal states with the SipHash constants and key k */
    info->v0 = SIP_HASH_CONST_1 ^ k[0];  /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    info->v1 = SIP_HASH_CONST_2 ^ k[1];  /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    info->v2 = SIP_HASH_CONST_3 ^ k[0];  /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    info->v3 = SIP_HASH_CONST_4 ^ k[1];  /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */

    /* Initialize variables */
    actMemset(info->buffer, 0u, actSIPHASH_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
    info->buffer_used = 0u;                             /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
    info->length = 0u;                                  /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */

    result = actOK;
  }
  return result;
}

/***********************************************************************************************************************
 *  actSipHashUpdate
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSipHashUpdate(
  VSECPRIM_P2VAR_PARA(actSipHashSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN, actLengthType length,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE result = actEXCEPTION_UNKNOWN;
  actLengthType diff = 0u;
  actLengthType lastBlockEnd;
  actLengthType bcount;

  if (dataIN == NULL_PTR)
  {
    result = actEXCEPTION_NULL;
  }
  else
  {
    /* Update length */
    info->length = info->length + (length % 256u);  /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */

    /* Process left overs of last update */
    if (info->buffer_used > 0u)
    {
      diff = actMin((actLengthType)length, (actLengthType)actSIPHASH_BLOCK_SIZE - info->buffer_used);
      /* Fill up buffer */
      actMemCpyByteArray((VSECPRIM_P2VAR_PARA(void))&info->buffer[info->buffer_used], (VSECPRIM_P2CONST_PARA(void)) dataIN, diff); /* PRQA S 0314 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SUBADDRESS_VARIABLE_SIZE */
      info->buffer_used += (actU8) diff;  /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */

      if (info->buffer_used < actSIPHASH_BLOCK_SIZE)
      {
        /* no complete block */
        result = actOK;
      }
      else
      {
        /* Process block and set buffer to zero */
        actSipHashBlockUpdate(info, info->buffer, ACT_SIPHASH_CROUNDS); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
        actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
        info->buffer_used = 0u; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
      }
    }

    if (result != actOK)
    {
      lastBlockEnd = (actLengthType)((length - diff) / actSIPHASH_BLOCK_SIZE);
      lastBlockEnd = (actSIPHASH_BLOCK_SIZE * lastBlockEnd) + diff;

      /* Loop over data blocks */
      for (bcount = diff; bcount < lastBlockEnd; bcount += actSIPHASH_BLOCK_SIZE)
      {
        /* Process block */
        actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */
        actSipHashBlockUpdate(info, &dataIN[bcount], ACT_SIPHASH_CROUNDS);  /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
      }

      /* Copy left overs into buffer */
      info->buffer_used = (actU8)(length - lastBlockEnd); /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
      actMemCpyByteArray((VSECPRIM_P2VAR_PARA(void)) info->buffer, (VSECPRIM_P2CONST_PARA(void)) &dataIN[lastBlockEnd], info->buffer_used); /* PRQA S 0314 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_VARIABLE_SIZE */
      result = actOK;
    }
  }
  return result;
}

/***********************************************************************************************************************
 *  actSipHashFinalize
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actSipHashFinalize(
  VSECPRIM_P2VAR_PARA(actSipHashSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU64) mac,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actU8 i;

  /* Handle remainders */
  for (i = info->buffer_used; i < actSIPHASH_BLOCK_SIZE; i++)
  {
    info->buffer[i] = 0u; /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_VARIABLE_INDEX_CHECK */
  }

  /* Write length information */
  info->buffer[actSIPHASH_BLOCK_SIZE - 1u] = (actU8)info->length;  /* SBSW_VSECPRIM_ARRAY_WRITE_ACCESS_PASSED_BUFFER_FIXED_INDEX */

  /* Process remainings */
  actSipHashBlockUpdate(info, info->buffer, ACT_SIPHASH_CROUNDS); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER_FIXED_SIZE */
  info->buffer_used = 0u; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */
  actL2trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */ /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* XOR the constant */
  info->v2 ^= 0xFFuLL; /* PRQA S 2982 */ /* MD_VSECPRIM_ALGORITHM_REQUIRED_ASSIGNMENT */ /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */

  /* Perform dRounds */
  actSipHashRound(&info->v0, &info->v1, &info->v2, &info->v3, ACT_SIPHASH_DROUNDS); /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
  actL1trigger(watchdog); /* PRQA S 1338, 2983, 3112  */ /* MD_MSR_DummyStmt */ /*lint -e438 */  /* SBSW_VSECPRIM_FUNCTION_CALL_WATCHDOG */

  /* Return result */
  *mac = info->v0 ^ info->v1 ^ info->v2 ^ info->v3; /* SBSW_VSECPRIM_PTR_WRITE_ACCESS_PASSED_BUFFER */

  return actOK;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTSIPHASH_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actSipHash.c
 *********************************************************************************************************************/
