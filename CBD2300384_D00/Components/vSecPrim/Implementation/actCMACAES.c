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
/*!        \file  actCMACAES.c
 *        \brief  Crypto library -  RFC 4493 AES-CMAC
 *
 *      \details  This file is part of the actCLibrary.
 *                Implements AES-CMAC generation according to RFC 4493.
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  REVISION HISTORY
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the module's header file.
 *
 *  FILE VERSION
 *  -------------------------------------------------------------------------------------------------------------------
 *  Refer to the VERSION CHECK below.
 *********************************************************************************************************************/

#define ACTCMACAES_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "actICMACAES.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTCMACAES_ENABLED == STD_ON)

/* PRQA S 0850 EOF */ /* MD_MSR_MacroArgumentEmpty */

/***********************************************************************************************************************
 *  DEFINES
 **********************************************************************************************************************/

/* Constant for sub-key generation */
# define CMAC_CONST_RB                                                (0x87u)
/* Most significant bit */
# define CMAC_MSB                                                     (0x80u)
/* Not final flag */
# define CMAC_NOT_FINAL                                               (0x00u)
/* Final flag */
# define CMAC_FINAL                                                   (0x01u)

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/***********************************************************************************************************************
 *  actCMACAESProcess
 **********************************************************************************************************************/
/*! \brief         Update CMAC calculation.
 *  \details       Helper function used by actual update with new data and finalization with sub-key.
 *  \param[in]     info  Pointer to context structure
 *  \param[in]     dataIN  Pointer to input data / sub-key
 *  \param[in]     length  Length of input data
 *  \param[in]     final  Finalization trigger
 *  \param[in]     watchdog  Watchdog trigger function
 *  \return        actOK  Operation successful
 *  \pre           Workspace must be initialized
 **********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actCMACAESProcess(
  VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actLengthType length,
  actU8 final,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/***********************************************************************************************************************
 *  actCMACAESGenerateSubKey
 **********************************************************************************************************************/
/*! \brief         Generate sub-key.
 *  \details       Generate sub-key used during finalization.
 *  \param[in]     info  Pointer to context structure
 *  \param[out]    subKey  Pointer to sub-key buffer
 *  \param[in]     watchdog  Watchdog trigger function
 *  \pre           Workspace must be initialized, subkey must point to a buffer of at least 16 bytes size.
 *  \context       TASK
 *  \reentrant     TRUE, for different workspaces
 *  \synchronous   TRUE
 **********************************************************************************************************************/
VSECPRIM_LOCAL_FUNC(void) actCMACAESGenerateSubKey(
  VSECPRIM_P2CONST_PARA(actCMACAESSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) subKey,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/***********************************************************************************************************************
 *  LOCAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  actCMACAESProcess
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_LOCAL_FUNC(actRETURNCODE) actCMACAESProcess(
  VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) dataIN,
  actLengthType length,
  actU8 final,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE result = actOK;

  actLengthType used;
  actLengthType xorLength;
  actLengthType offset = 0u;
  actLengthType rest = length;
  boolean processLastBlock = FALSE;

  if (final == CMAC_NOT_FINAL)
  {
    used = info->buffer_used;
  }
  else
  {
    processLastBlock = TRUE;
    used = 0u;
  }

  /* Calculate CMAC from input data */
  while ((rest > 0u) || (processLastBlock == TRUE))
  {
    if (used == actAES_BLOCK_SIZE)
    {
      actAESEncryptBlock(info, info->buffer, info->buffer, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
      used = 0u;
      processLastBlock = FALSE;
    }
    else
    {
      xorLength = actMin(actAES_BLOCK_SIZE - used, rest);

      actXOR(&info->buffer[used], &dataIN[offset], (actU32)xorLength); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

      used += xorLength;
      offset += xorLength;
      rest -= xorLength; /* PRQA S 2912 */ /* MD_VSECPRIM_21.1 */
    }
  }

  info->buffer_used = used; /* SBSW_VSECPRIM_WRITE_POINTER */

  return result;
}

/***********************************************************************************************************************
 *  actCMACAESGenerateSubKey
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_LOCAL_FUNC(void) actCMACAESGenerateSubKey(
  VSECPRIM_P2CONST_PARA(actCMACAESSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) subKey,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actU8 msb;
  actU8 index;
  actU8 keyIndex;
  actU8 keyCount;

  /* Prepare sub key buffer */
  actMemset(subKey, 0u, actAES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  actAESEncryptBlock(info, subKey, subKey, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  if (info->buffer_used == actAES_BLOCK_SIZE)
  {
    keyCount = 1u;
  }
  else
  {
    keyCount = 2u;
  }

  /* Generate subkey due to CMAC Spec */
  for (keyIndex = 0u; keyIndex < keyCount; keyIndex++)
  {
    msb = subKey[0] & CMAC_MSB;

    for (index = 0; index < (actAES_BLOCK_SIZE - 1u); index++)
    {
      subKey[index] = (actU8) (((subKey[index] << 1) | (subKey[index + 1u] >> 7)) & 0xFFu); /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
    }

    subKey[index] = (actU8) ((subKey[index] << 1) & 0xFFu); /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */

    if (msb != 0u)
    {
      subKey[index] ^= CMAC_CONST_RB; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
    }
  }
}

/***********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 **********************************************************************************************************************/

/***********************************************************************************************************************
 *  actCMACAESInit
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actCMACAESInit (
   VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_length,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  actRETURNCODE result;

  /* Perform AES init */
  result = actAESInitEncryptBlock(info, key, key_length, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* Prepare workspace buffer */
  actMemset(info->buffer, 0u, actAES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
  info->buffer_used = 0u; /* SBSW_VSECPRIM_WRITE_POINTER */

  return result;
}

/***********************************************************************************************************************
 *  actCMACAESUpdate
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actCMACAESUpdate (
   VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) dataIN, actLengthType length,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  /* Process CMAC */
  return actCMACAESProcess(info, dataIN, length, CMAC_NOT_FINAL, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */
}

/***********************************************************************************************************************
 *  actCMACAESFinalize
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actCMACAESFinalize (
   VSECPRIM_P2VAR_PARA(actCMACAESSTRUCT) info, VSECPRIM_P2VAR_PARA(actU8) mac,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void))
{
  VSECPRIM_P2VAR_PARA(actU8) subKey;
  actRETURNCODE result;
  actLengthType used;

  /* Use previous block buffer for sub-key generation */
  subKey = info->prev_block;
  used = info->buffer_used;

  actCMACAESGenerateSubKey(info, subKey, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  if (used < actAES_BLOCK_SIZE)
  {
    subKey[used] ^= CMAC_MSB; /* SBSW_VSECPRIM_WRITE_ARRAY_NO_CHECK */
  }

  /* Finalize CMAC operation, actCMACAESProcess always returns actOK */
  result = actCMACAESProcess(info, subKey, actAES_BLOCK_SIZE, CMAC_FINAL, watchdog); /* SBSW_VSECPRIM_CALL_FUNCTION */

  /* Copy MAC from buffer to output pointer */
  actMemCpyByteArray(mac, info->buffer, actAES_BLOCK_SIZE); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

  return result;
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTCMACAES_ENABLED == STD_ON) */

/***********************************************************************************************************************
 *  END OF FILE: actCMACAES.c
 **********************************************************************************************************************/
