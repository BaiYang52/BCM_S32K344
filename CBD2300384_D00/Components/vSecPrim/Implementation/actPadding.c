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
/*!        \file  actPadding.c
 *        \brief  Padding implementation.
 *
 *      \details This file is part of the embedded systems library actCLib
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

#define ACTPADDING_SOURCE

/***********************************************************************************************************************
 *  INCLUDES
 **********************************************************************************************************************/

#include "actPadding.h"
#include "actUtilities.h"

#if (VSECPRIM_ACTPADDING_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/**********************************************************************************************************************
  actPaddingInit()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actPaddingInit(actU8 mode)
{
  actRETURNCODE result;

  /* Check padding mode */
  switch (mode)
  {
    case actPADDING_PM_OFF:
    case actPADDING_PM_ONEWITHZEROES:
    case actPADDING_PM_PKCS7:
    {
      result = actOK;

      break;
    }
    default:
    {
      result = actEXCEPTION_ALGOID;

      break;
    }
  }

  return result;
}

/**********************************************************************************************************************
  actPaddingPad()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actPaddingPad(
   actU8 mode, actLengthType buffer_size, VSECPRIM_P2VAR_PARA(actLengthType) buffer_used,
   VSECPRIM_P2VAR_PARA(actU8) buffer)
{
  actRETURNCODE result;
  actLengthType pad;
  actLengthType used;

  used = *buffer_used;

  if (buffer_size <= used)
  {
    result = actEXCEPTION_LENGTH;
  }
  else
  {
    result = actOK;
    pad = buffer_size - used;

    /* Padding mode */
    switch (mode)
    {
      case actPADDING_PM_OFF:
      {
        if (used != 0u)
        {
          result = actEXCEPTION_LENGTH;
        }

        pad = 0u;

        break;
      }
      case actPADDING_PM_ONEWITHZEROES:
      {
        actMemset(&buffer[used], 0x00u, pad); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */
        buffer[used] = 0x80u; /* SBSW_VSECPRIM_WRITE_ARRAY */

        break;
      }
      case actPADDING_PM_PKCS7:
      {
        actMemset(&buffer[used], (actU8) (pad & 0xFFu), pad); /* PRQA S 0315 */ /* MD_VSECPRIM_P2VOID_CAST */ /* SBSW_VSECPRIM_CALL_FUNCTION */

        break;
      }
      default:
      {
        result = actEXCEPTION_PADDING;

        break;
      }
    }

    *buffer_used = used + pad; /* SBSW_VSECPRIM_WRITE_POINTER */
  }

  return result;
}

/**********************************************************************************************************************
  actPaddingUnpad()
 **********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actPaddingUnpad(
   actU8 mode, actLengthType buffer_size, VSECPRIM_P2VAR_PARA(actLengthType) buffer_used,
   VSECPRIM_P2CONST_PARA(actU8) buffer)
{
  actRETURNCODE result;
  actU8 pad;
  actLengthType unpad;
  actLengthType index;

  result = actOK;
  unpad = 0u; /* PRQA S 2982 */ /* MD_VSECPRIM_2.2 */

  /* Padding mode */
  switch (mode)
  {
    case actPADDING_PM_OFF:
    {
      unpad = buffer_size;

      break;
    }
    case actPADDING_PM_ONEWITHZEROES:
    {
      for (index = (buffer_size - 1u); index > 0u; index--)
      {
        if (buffer[index] != 0x00u)
        {
          break;
        }
      }

      if (buffer[index] != 0x80u)
      {
        result = actEXCEPTION_PADDING;
      }
      else
      {
        unpad = index;
      }

      break;
    }
    case actPADDING_PM_PKCS7:
    {
      /* PKCS7 unpad of the block */
      pad = buffer[buffer_size - 1u];

      if (pad > buffer_size)
      {
        result = actEXCEPTION_PADDING;
      }
      else
      {
        unpad = buffer_size - pad;

        for (index = unpad; index < buffer_size; index++)
        {
          if (buffer[index] != pad)
          {
            result = actEXCEPTION_PADDING;
          }
        }
      }

      break;
    }
    default:
    {
      result = actEXCEPTION_PADDING;

      break;
    }
  }

  if (result == actOK)
  {
    *buffer_used = unpad; /* SBSW_VSECPRIM_WRITE_POINTER */
  }

  return result;
} /* PRQA S 6030 */ /* MD_MSR_STCYC */

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTPADDING_ENABLED == STD_ON) */

/***********************************************************************************************************************
 *  END OF FILE: ACTPADDING.C
 **********************************************************************************************************************/
