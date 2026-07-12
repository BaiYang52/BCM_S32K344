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
/*!        \file  actECLengthInfo.h
 *        \brief  Interface for Length information for internal use.
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

#ifndef ACTECLENGTHINFO_H
# define ACTECLENGTHINFO_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  actECBasicWksp()
 *********************************************************************************************************************/
/*! \brief          Returns the required workspace size in actBNDIGITs for the EC basic workspace used
 *                   by all EC primitives except ECDecompPoint.
 *  \details        -
 *  \param[in]      p_length  length of the prime p in actBNDIGITs of the prime field, over which the curve is defined
 *  \param[in]      n_length  length of the order of the base point in actBNDIGITs
 *  \return         required workspace size in actBNDIGITs
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actLengthType ) actECBasicWksp(
  actLengthType p_length,
  actLengthType n_length);

/**********************************************************************************************************************
 *  actECKgWksp()
 *********************************************************************************************************************/
/*! \brief          Returns the required workspace size in actBNDIGITs for EC key generation.
 *  \details        -
 *  \param[in]      p_length  length of the prime p in actBNDIGITs of the prime field, over which the curve is defined
 *  \param[in]      n_length  length of the order of the base point in actBNDIGITs
 *  \return         required workspace size in actBNDIGITs
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actLengthType ) actECKgWksp(actLengthType p_length, actLengthType n_length);

/**********************************************************************************************************************
 *  actECDHpWksp()
 *********************************************************************************************************************/
/*! \brief          Returns the required workspace size in actBNDIGITs for the Diffie Hellman key generation primitive.
 *  \details        -
 *  \param[in]      p_length  length of the prime p in actBNDIGITs of the prime field, over which the curve is defined
 *  \param[in]      n_length  length of the order of the base point in actBNDIGITs
 *  \return         required workspace size in actBNDIGITs
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actLengthType ) actECDHpWksp(actLengthType p_length, actLengthType n_length);

/**********************************************************************************************************************
 *  actECDHWksp()
 *********************************************************************************************************************/
/*! \brief          Returns the required workspace size in actBNDIGITs for the Diffie Hellman key exchange protocol.
 *  \details        -
 *  \param[in]      p_length  length of the prime p in actBNDIGITs of the prime field, over which the curve is defined
 *  \param[in]      n_length  length of the order of the base point in actBNDIGITs
 *  \return         required workspace size in actBNDIGITs
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actLengthType ) actECDHWksp(actLengthType p_length, actLengthType n_length);

/**********************************************************************************************************************
 *  actECDSASpWksp()
 *********************************************************************************************************************/
/*! \brief          Returns the required workspace size in actBNDIGITs for the ECDSA signature generation protocol.
 *  \details        -
 *  \param[in]      p_length  length of the prime p in actBNDIGITs of the prime field, over which the curve is defined
 *  \param[in]      n_length  length of the order of the base point in actBNDIGITs
 *  \return         required workspace size in actBNDIGITs
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actLengthType ) actECDSASpWksp(actLengthType p_length, actLengthType n_length);

/**********************************************************************************************************************
 *  actECDSAVpWksp()
 *********************************************************************************************************************/
/*! \brief          Returns the required workspace size in actBNDIGITs for the ECDSA signature verification protocol.
 *  \details        -
 *  \param[in]      p_length  length of the prime p in actBNDIGITs of the prime field, over which the curve is defined
 *  \param[in]      n_length  length of the order of the base point in actBNDIGITs
 *  \return         required workspace size in actBNDIGITs
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC( actLengthType ) actECDSAVpWksp( actLengthType p_length, actLengthType n_length );

/**********************************************************************************************************************
 *  actECBDWksp()
 *********************************************************************************************************************/
/*! \brief          Returns the required workspace size in actBNDIGITs for the Burmester Desmedt key
 *                  exchange protocol.
 *  \details        -
 *  \param[in]      p_length  length of the prime p in actBNDIGITs of the prime field, over which the curve is defined
 *  \param[in]      n_length  length of the order of the base point in actBNDIGITs
 *  \return         required workspace size in actBNDIGITs
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC( actLengthType ) actECBDWksp( actLengthType p_length, actLengthType n_length );

/**********************************************************************************************************************
 *  actECGetPrimeAndOrderBytes()
 *********************************************************************************************************************/
/*! \brief          Extracts the length in bytes of the prime p and the basepoint order n for the given
 *                   domain parameters
 *  \details        -
 *  \param[in]      domain  pointer to curve's domain parameter
 *  \param[in,out]  p_bytes (in)  buffer to store the length of p or null pointer if length of p is not of interest
 *                          (out) length of p if p_bytes was not a null pointer
 *  \param[in,out]  n_bytes (in)  buffer to store the length of n or null pointer if length of n is not of interest
 *                          (out) length of n if n_bytes was not a null pointer
 *  \return         actEXCEPTION_DOMAIN    domain decoding error
 *                  actOK                  else
 *  \pre            domain must be a valid pointer.
 *  \context        TASK
 *  \reentrant      TRUE
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actECGetPrimeAndOrderBytes(
  VSECPRIM_P2ROMCONST_PARA(actU8) domain,
  VSECPRIM_P2VAR_PARA(actLengthType ) p_bytes,
  VSECPRIM_P2VAR_PARA(actLengthType ) n_bytes);

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
} /* extern "C" */
# endif
# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTECLENGTHINFO_H */

/**********************************************************************************************************************
 *  END OF FILE: actECLengthInfo.h
 *********************************************************************************************************************/
