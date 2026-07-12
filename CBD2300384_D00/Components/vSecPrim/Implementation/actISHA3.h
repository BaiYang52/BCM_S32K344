/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 1999 - 2020 cv cryptovision GmbH.                                                All rights reserved.
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
/*!        \file  actISHA3.h
 *        \brief  Interface for SHA3 hash algorithm.
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

#ifndef ACTISHA3_H
# define ACTISHA3_H

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
# include "actITypes.h"

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus /* COV_VSECPRIM_CPLUSPLUS XF */
    extern "C" {
# endif

/**********************************************************************************************************************
 *  actSHA3Init()
 *********************************************************************************************************************/
/*! \brief          Initialize the SHA3 algorithm.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to the KECCAK[c] context
 *                               (out) initialized KECCAK[c] context structure
 *  \param[in]      strength     security strength (BITs) - equals the digest size
 *  \return         actEXCEPTION_LENGTH   when strength is no multiple of 32
 *                  (2* strength MUST be a multiple of LANE size (64))
 *                  actOK                 otherwise
 *  \pre            info must be a valid pointer.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA3Init(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  const actU32 strength);

/**********************************************************************************************************************
 *  actSHA3Update()
 *********************************************************************************************************************/
/*! \brief          This function hashes the given data and can be called arbitrarily
 *                  often between an initialize and finalize of the SHA3 algorithm.
 *                  Uses any data already in the actKECCAKcSTRUCT structure and leaves
 *                  any partial data block there.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to the KECCAK[c] context
 *                               (out) updated KECCAK[c] context structure
 *  \param[in]      data         pointer to data to be hashed
 *  \param[in]      dataLength   length of data (bytes)
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \return         actEXCEPTION_MODE   when called AFTER finalize
 *                  actOK               otherwise
 *  \pre            actSHA3Init() is called once before calling this function
 *                  All pointers must be valid.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA3Update(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2CONST_PARA(actU8) data,
  const actLengthType dataLength,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actSHA3Finalize()
 *********************************************************************************************************************/
/*! \brief          Finalize the SHA3 algorithm and deliver the digest.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to the KECCAK[c] context
 *                               (out) finalized KECCAK[c] context structure
 *  \param[in,out]  digest       (in)  pointer to byte array to hold the digest
 *                               (out)  pointer to byte array storing the digest
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \return         actOK always
 *  \pre            actSHA3Init() is called once before calling this function
 *                  All pointers must be valid.
 *                  The buffer referenced by digest must provide at least (info->strength / 8) many bytes.
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actSHA3Finalize(
  VSECPRIM_P2VAR_PARA(actKECCAKcSTRUCT) info,
  VSECPRIM_P2VAR_PARA(actU8) digest,
  VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus /* COV_VSECPRIM_CPLUSPLUS XF */
    }  /*  extern "C"  */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /*  ACTISHA3_H  */

/**********************************************************************************************************************
 *  END OF FILE: actISHA3.h
 *********************************************************************************************************************/
