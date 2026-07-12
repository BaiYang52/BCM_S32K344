/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2016 cv cryptovision GmbH.                                                All rights reserved.
 *
 *  For modifications by Vector Informatik GmbH:
 *  Copyright (c) 2023 by Vector Informatik GmbH.                                                  All rights reserved.
 *
 *                This software is protected under actLengthTypeellectual property laws and proprietary to cv cryptovision GmbH
 *                and/or Vector Informatik GmbH.
 *                No right or license is granted save as expressly set out in the applicable license conditions.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/*!        \file  actECLengthInfo.c
 *        \brief  The EC length info functions.
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

#define ACTECLENGTHINFO_SOURCE

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "actECLengthInfo.h"
#include "actITypes.h"
#include "actUtilities.h"
#include "actBigNum.h"
#include "actECPoint.h"
#include "actECTools.h"
#include "actECLengthMacros.h"

#if (VSECPRIM_ACTECLENGTHINFO_ENABLED == STD_ON)

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

/* ----------------------------------------------------------------------
 |  the long number scratch required (in actBNDIGIT units)
 |  is calculated from the context input vars:
 |    p_length, n_length
 --------------------------------------------------------------------- */

/**********************************************************************************************************************
 *  actECBasicWksp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 *
 */
VSECPRIM_FUNC(actLengthType) actECBasicWksp(
  actLengthType p_length,
  actLengthType n_length)
{
   return ( actEC_BASIC_WKSP(p_length, n_length) ); /* PRQA S 3494 */ /* MD_VSECPRIM_CONST_BOOL_OPERAND */ /*lint !e506 */
}

/**********************************************************************************************************************
 *  actECKgWksp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actLengthType) actECKgWksp(
  actLengthType p_length,
  actLengthType n_length)
{
   return ( actEC_KG_WKSP(p_length, n_length) ); /* PRQA S 3494 */ /* MD_VSECPRIM_CONST_BOOL_OPERAND */ /*lint !e506 */
}

/**********************************************************************************************************************
 *  actECDHpWksp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actLengthType) actECDHpWksp(
  actLengthType p_length,
  actLengthType n_length)
{
   return ( actEC_DHp_WKSP(p_length, n_length) ); /* PRQA S 3494 */ /* MD_VSECPRIM_CONST_BOOL_OPERAND */ /*lint !e506 */
}

/**********************************************************************************************************************
 *  actECDHWksp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actLengthType) actECDHWksp(
  actLengthType p_length,
  actLengthType n_length)
{
   return ( actEC_DH_WKSP(p_length, n_length) ); /* PRQA S 3494 */ /* MD_VSECPRIM_CONST_BOOL_OPERAND */ /*lint !e506 */
}

/**********************************************************************************************************************
 *  actECDSASpWksp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actLengthType) actECDSASpWksp(
  actLengthType p_length,
  actLengthType n_length)
{
   return ( actEC_DSASp_WKSP(p_length, n_length) ); /* PRQA S 3494 */ /* MD_VSECPRIM_CONST_BOOL_OPERAND */ /*lint !e506 */
}

/**********************************************************************************************************************
 *  actECDSAVpWksp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC( actLengthType ) actECDSAVpWksp(
  actLengthType p_length,
  actLengthType n_length )
{
   return ( actEC_DSAVp_WKSP(p_length, n_length) ); /* PRQA S 3494 */ /* MD_VSECPRIM_CONST_BOOL_OPERAND */ /*lint !e506 */
}

/**********************************************************************************************************************
 *  actECBDWksp()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC( actLengthType ) actECBDWksp(
  actLengthType p_length,
  actLengthType n_length )
{
   return ( actEC_BDKA_WKSP(p_length, n_length) ); /* PRQA S 3494 */ /* MD_VSECPRIM_CONST_BOOL_OPERAND */ /*lint !e506 */
}

/**********************************************************************************************************************
 *  actECGetPrimeAndOrderBytes()
 *********************************************************************************************************************/
/*!
 * Internal comment removed.
 *
 *
 *
 */
VSECPRIM_FUNC(actRETURNCODE) actECGetPrimeAndOrderBytes(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2VAR_PARA(actLengthType) p_bytes,
  VSECPRIM_P2VAR_PARA(actLengthType) n_bytes)
{
   return actECParseDomain(domain, /* SBSW_VSECPRIM_FCT_CALL_PASSED_BUFFER */
                            p_bytes,
                            (VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)))NULL_PTR,
                            (VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)))NULL_PTR,
                            n_bytes,
                            (VSECPRIM_P2VAR_PARA(VSECPRIM_P2CONST_PARA(actU8)))NULL_PTR);
}

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* (VSECPRIM_ACTECLENGTHINFO_ENABLED == STD_ON) */

/**********************************************************************************************************************
 *  END OF FILE: actECLengthInfo.c
 *********************************************************************************************************************/
