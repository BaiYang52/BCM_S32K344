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
/*!        \file  actIHashMAC.h
 *        \brief  Interface for SHA-1 based Hash MAC algorithm.
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

#ifndef ACTIHASHMAC_H
# define ACTIHASHMAC_H

# include "actITypes.h"
# include "actISHA.h"

/****************************************************************************
 ** Types and constants
 ***************************************************************************/

/****************************************************************************
 ** Function Prototypes
 ***************************************************************************/

# define VSECPRIM_START_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
extern "C"
{
# endif

/**********************************************************************************************************************
 *  actHashMACInit()
 *********************************************************************************************************************/
/*! \brief          This function initializes the HashMAC algorithm.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to context structure
 *                               (out) initialized context structure
 *  \param[in]      key          MAC key
 *  \param[in]      key_length   length of key in bytes
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \return         actEXCEPTION_LENGTH    if key_len < 1
 *                  actOK                  else
 *  \pre            -
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actHashMACInit(
   VSECPRIM_P2VAR_PARA(actHASHMACSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) key, actLengthType key_length,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actHashMACUpdate()
 *********************************************************************************************************************/
/*! \brief          This function hashes the given data.
 *  \details        This function hashes the given data and can be called arbitrarily
 *                  often between an initialize and finalize of the HashMAC algorithm.
 *                  Uses any data already in the actSHASTRUCT structure and leaves
 *                  any partial data block there.
 *  \param[in,out]  info         (in) pointer to context structure
 *                               (out) actualized context structure
 *  \param[in]      dataIN       pointer to data to be hashed
 *  \param[in]      length       length of data in bytes
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \return         actEXCEPTION_LENGTH   total input more than 2^64 - 1 bit
 *                  actOK                 else
 *  \pre            actHashMACInit() is called once before calling this function
 *                  dataIN != NULL is a valid pointer
 *                  length >= 0
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actHashMACUpdate(
   VSECPRIM_P2VAR_PARA(actHASHMACSTRUCT) info,
   VSECPRIM_P2CONST_PARA(actU8) dataIN, actLengthType length,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

/**********************************************************************************************************************
 *  actHashMACFinalize()
 *********************************************************************************************************************/
/*! \brief          This function finalizes the HashMAC algorithm and
 *                  delivers the MAC.
 *  \details        -
 *  \param[in,out]  info         (in) pointer to context structure
 *                               (out) finalized context structure
 *  \param[in,out]  hash         (in) byte array to contain the hash value
 *                               (out) the final hash value,
 *                               (big endian of length actHASH_SIZE_SHA)
 *  \param[in]      watchdog     pointer to watchdog reset function
 *  \return         actOK always
 *  \pre            actHashMACInit() is called once before calling this function
 *  \context        TASK
 *  \reentrant      TRUE, for different workspaces
 *  \synchronous    TRUE
 *********************************************************************************************************************/
VSECPRIM_FUNC(actRETURNCODE) actHashMACFinalize(
   VSECPRIM_P2VAR_PARA(actHASHMACSTRUCT) info, VSECPRIM_P2VAR_PARA(actU8) hash,
   VSECPRIM_P2FUNC(VSECPRIM_NONE, void, watchdog)(void));

# ifdef __cplusplus                                       /* COV_VSECPRIM_CPLUSPLUS XF */
}                               /* extern "C" */
# endif

# define VSECPRIM_STOP_SEC_CODE
# include "vSecPrim_MemMap.h" /* PRQA S 5087 */ /* MD_MSR_MemMap */

#endif /* ACTIHASHMAC_H */

/**********************************************************************************************************************
 *  END OF FILE: actIHashMAC.h
 *********************************************************************************************************************/
